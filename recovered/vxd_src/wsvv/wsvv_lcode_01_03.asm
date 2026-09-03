; wsvv_lcode_01_03.asm -- WSVV object 1 segment _LTEXT  (part 3/5)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_9588:
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        or     esi, esi
        mov    eax, dword ptr [esp + 8]
        je     SHORT loc_1_95B8
loc_1_9595:
        mov    ecx, dword ptr [eax + 0ch]
        mov    edx, ecx
        cmp    edx, esi
        jb     SHORT loc_1_95A0
        mov    edx, esi
loc_1_95A0:
        sub    esi, edx
        add    dword ptr [eax + 8], edx
        sub    ecx, edx
        mov    dword ptr [eax + 0ch], ecx
        jne    SHORT loc_1_95B4
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        je     SHORT loc_1_95B8
        mov    eax, ecx
loc_1_95B4:
        or     esi, esi
        jne    SHORT loc_1_9595
loc_1_95B8:
        pop    esi
        ret
sub_1_95BA:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0ch]
        or     eax, eax
        je     SHORT loc_1_95E3
        mov    edi, 5
loc_1_95C9:
        cmp    dword ptr [eax + 4], edi
        jne    SHORT loc_1_95DD
        mov    esi, dword ptr [eax]
        push   eax
        call   sub_1_F396
        add    esp, 4
        mov    eax, esi
        jmp    SHORT loc_1_95DF
loc_1_95DD:
        mov    eax, dword ptr [eax]
loc_1_95DF:
        or     eax, eax
        jne    SHORT loc_1_95C9
loc_1_95E3:
        pop    edi
        pop    esi
        ret
sub_1_95E6:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esi + 0d2h]
        mov    ebx, dword ptr [esi + 48h]
loc_1_95F7:
        or     edi, edi
        je     SHORT loc_1_9653
        mov    ecx, dword ptr [edi + 4]
        mov    eax, ebx
        sub    eax, ecx
        js     SHORT loc_1_9653
        mov    eax, dword ptr [edi + 0ch]
        and    eax, 100h
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        add    eax, dword ptr [edi + 8]
        add    eax, ecx
        mov    ecx, ebx
        sub    ecx, eax
        js     SHORT loc_1_965A
        push   dword ptr [edi + 14h]
        call   sub_1_95BA
        add    esp, 4
        mov    ebp, dword ptr [edi]
        push   edi
        call   sub_1_F396
        add    esp, 4
        mov    edi, ebp
        mov    dword ptr [esi + 0d2h], ebp
        or     ebp, ebp
        jne    SHORT loc_1_95F7
        mov    al, byte ptr [esi + 0bah]
        dec    al
        mov    byte ptr [esi + 0bah], al
        jne    SHORT loc_1_9653
        and    dword ptr [esi + 64h], 0fffffffeh
loc_1_9653:
        sub    eax, eax
loc_1_9655:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_965A:
        mov    ebp, ebx
        mov    ecx, dword ptr [esp + 18h]
        sub    ebp, dword ptr [edi + 4]
        mov    dword ptr [ecx], ebx
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [ecx + 0ch], eax
        cmp    dword ptr [edi + 8], ebp
        je     SHORT loc_1_9691
        push   ebp
        push   dword ptr [edi + 14h]
        call   sub_1_7B05
        add    esp, 8
        mov    ebx, eax
        mov    ecx, dword ptr [edi + 8]
        mov    eax, dword ptr [esp + 1ch]
        sub    ecx, ebp
        mov    dword ptr [eax], ecx
        jmp    SHORT loc_1_96AB
loc_1_9691:
        push   dword ptr [edi + 14h]
        call   sub_1_95BA
        db  0BBh, 048h, 038h, 001h, 000h
        mov    eax, dword ptr [esp + 20h]
        add    esp, 4
        mov    dword ptr [eax], 0
loc_1_96AB:
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0d2h], eax
        or     eax, eax
        jne    SHORT loc_1_96CB
        mov    al, byte ptr [esi + 0bah]
        dec    al
        mov    byte ptr [esi + 0bah], al
        jne    SHORT loc_1_96CB
        and    dword ptr [esi + 64h], 0fffffffeh
loc_1_96CB:
        push   edi
        call   sub_1_F396
        add    esp, 4
        mov    eax, ebx
        jmp    NEAR PTR loc_1_9655
sub_1_96DB:
        push   ebx
        push   esi
        push   edi
        push   1ch
        call   sub_1_F390
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_96F2
        sub    eax, eax
        jmp    SHORT loc_1_976C
loc_1_96F2:
        mov    ebx, dword ptr [esp + 1ch]
        lea    eax, [ebx + 10h]
        push   eax
        call   sub_1_F390
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_9715
        push   esi
        call   sub_1_F396
        add    esp, 4
        sub    eax, eax
        jmp    SHORT loc_1_976C
loc_1_9715:
        mov    dword ptr [edi + 4], 5
        mov    dword ptr [edi + 0ch], ebx
        mov    dword ptr [edi], 0
        lea    eax, [edi + 10h]
        mov    dword ptr [edi + 8], eax
        or     ebx, ebx
        je     SHORT loc_1_973F
        push   0
        push   ebx
        push   dword ptr [esp + 1ch]
        push   eax
        call   sub_1_44E1
        add    esp, 10h
loc_1_973F:
        mov    ecx, dword ptr [esp + 18h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [ecx + 10h]
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [esi + 8], ebx
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [esi + 10h], eax
        mov    dword ptr [esi + 14h], edi
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esi + 18h], edi
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi], ecx
        mov    dword ptr [eax], esi
        mov    eax, 1
loc_1_976C:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_9770:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 28h]
        mov    eax, dword ptr [eax + 10h]
        and    eax, 100h
        cmp    eax, 1
        sbb    ecx, ecx
        inc    ecx
        mov    eax, dword ptr [esp + 28h]
        mov    esi, dword ptr [esp + 24h]
        add    ecx, dword ptr [eax]
        mov    eax, dword ptr [esp + 30h]
        add    eax, ecx
        mov    dword ptr [esp + 1ch], eax
        mov    edi, dword ptr [esi + 0d2h]
        add    esi, 0d2h
        or     edi, edi
        je     NEAR PTR loc_1_990A
loc_1_97B1:
        mov    edx, dword ptr [edi + 0ch]
        and    edx, 100h
        lea    ecx, [edi + 4]
        mov    dword ptr [esp + 14h], ecx
        mov    ebp, dword ptr [ecx]
        lea    eax, [edi + 8]
        mov    dword ptr [esp + 18h], eax
        cmp    edx, 1
        sbb    ecx, ecx
        inc    ecx
        add    ecx, dword ptr [eax]
        mov    eax, dword ptr [esp + 28h]
        lea    ebx, [ebp + ecx]
        mov    ecx, dword ptr [eax]
        mov    eax, ecx
        sub    eax, ebx
        or     eax, eax
        jg     NEAR PTR loc_1_98FA
        mov    eax, dword ptr [esp + 1ch]
        sub    eax, ebp
        js     NEAR PTR loc_1_990A
        cmp    dword ptr [esp + 30h], 0
        mov    eax, dword ptr [esp + 28h]
        je     NEAR PTR loc_1_99CE
        mov    eax, dword ptr [eax + 10h]
        test   ah, 1
        je     SHORT loc_1_9819
        mov    edx, dword ptr [esp + 1ch]
        sub    edx, ebx
        or     edx, edx
        jle    NEAR PTR loc_1_993F
loc_1_9819:
        sub    ebp, ecx
        mov    dword ptr [esp + 10h], ebp
        or     ebp, ebp
        jle    SHORT loc_1_98A2
        test   ah, 20h
        jne    SHORT loc_1_9852
        test   byte ptr [edi + 0dh], 20h
        je     SHORT loc_1_9852
        push   ebp
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 34h]
        push   esi
        call   sub_1_96DB
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_9848
        mov    eax, dword ptr [esi]
        mov    edi, dword ptr [eax]
loc_1_9848:
        mov    dword ptr [esp + 10h], 0
        jmp    SHORT loc_1_98A2
loc_1_9852:
        lea    eax, [ebp + 10h]
        push   eax
        call   sub_1_F390
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_1_993F
        mov    dword ptr [esi + 4], 5
        mov    dword ptr [esi + 0ch], ebp
        lea    eax, [esi + 10h]
        push   0
        push   ebp
        mov    dword ptr [esi + 8], eax
        push   dword ptr [esp + 34h]
        push   eax
        call   sub_1_44E1
        mov    eax, dword ptr [esp + 28h]
        add    esp, 10h
        add    dword ptr [eax], ebp
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [edi + 14h], esi
        mov    ecx, dword ptr [esp + 14h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
loc_1_98A2:
        mov    edx, dword ptr [edi + 0ch]
        test   dh, 1
        jne    NEAR PTR loc_1_9947
        mov    eax, dword ptr [esp + 28h]
        mov    esi, dword ptr [esp + 30h]
        mov    ecx, dword ptr [eax]
        mov    eax, ecx
        sub    eax, ebx
        add    esi, eax
        or     esi, esi
        jle    NEAR PTR loc_1_9994
        mov    eax, dword ptr [esp + 28h]
        test   byte ptr [eax + 11h], 20h
        je     SHORT loc_1_9949
        test   dh, 20h
        jne    SHORT loc_1_9949
        cmp    dword ptr [esp + 10h], 0
        jg     SHORT loc_1_9949
        sub    ebx, ecx
        push   ebx
        push   dword ptr [esp + 30h]
        call   sub_1_7B05
        mov    dword ptr [esp + 34h], eax
        mov    eax, dword ptr [esp + 30h]
        add    esp, 8
        add    dword ptr [eax], ebx
        sub    dword ptr [eax + 0ch], ebx
        jmp    SHORT loc_1_98FE
loc_1_98FA:
        or     edx, edx
        jne    SHORT loc_1_993F
loc_1_98FE:
        mov    esi, edi
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    NEAR PTR loc_1_97B1
loc_1_990A:
        mov    eax, dword ptr [esp + 24h]
        push   dword ptr [esp + 30h]
        mov    edi, dword ptr [eax + 0d2h]
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 34h]
        push   esi
        call   sub_1_96DB
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_993F
        or     edi, edi
        jne    SHORT loc_1_993F
        mov    eax, dword ptr [esp + 24h]
        inc    byte ptr [eax + 0bah]
        or     dword ptr [eax + 64h], 1
loc_1_993F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_9947:
        sub    esi, esi
loc_1_9949:
        or     esi, esi
        jle    SHORT loc_1_9994
        lea    eax, [esi + 10h]
        push   eax
        call   sub_1_F390
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_1_9994
        mov    dword ptr [ebp + 4], 5
        mov    dword ptr [ebp + 0ch], esi
        lea    ecx, [ebp + 10h]
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [ebp + 8], ecx
        sub    ebx, dword ptr [eax]
        push   ebx
        push   esi
        push   dword ptr [esp + 34h]
        push   ecx
        call   sub_1_44E1
        add    esp, 10h
        mov    eax, dword ptr [edi + 18h]
        add    dword ptr [edi + 8], esi
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebp], ecx
        mov    dword ptr [eax], ebp
        mov    dword ptr [edi + 18h], ebp
loc_1_9994:
        mov    eax, dword ptr [esp + 28h]
        test   byte ptr [eax + 11h], 20h
        je     SHORT loc_1_993F
        mov    edx, dword ptr [eax + 0ch]
        add    edx, dword ptr [eax]
        mov    eax, dword ptr [edi + 0ch]
        test   ah, 20h
        je     SHORT loc_1_99BB
        mov    eax, dword ptr [edi + 4]
        add    eax, dword ptr [edi + 10h]
        mov    ecx, edx
        sub    ecx, eax
        jns    SHORT loc_1_99C3
        mov    edx, eax
        jmp    SHORT loc_1_99C3
loc_1_99BB:
        or     eax, 2000h
        mov    dword ptr [edi + 0ch], eax
loc_1_99C3:
        sub    edx, dword ptr [edi + 4]
        mov    dword ptr [edi + 10h], edx
        jmp    NEAR PTR loc_1_993F
loc_1_99CE:
        test   byte ptr [eax + 11h], 1
        je     NEAR PTR loc_1_993F
        mov    ecx, dword ptr [edi + 0ch]
        test   ch, 1
        jne    NEAR PTR loc_1_993F
        cmp    dword ptr [eax], ebx
        jne    NEAR PTR loc_1_993F
        or     ecx, 100h
        mov    dword ptr [edi + 0ch], ecx
        jmp    NEAR PTR loc_1_993F
sub_1_99FA:
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 3ch]
        cmp    dword ptr [esp + 40h], 3ch
        mov    ebp, dword ptr [ebx + 8]
        jb     NEAR PTR loc_1_A3E9
        mov    ecx, dword ptr [ebp + 2bh]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    eax, 8
        or     edx, eax
        shr    ecx, 18h
        or     edx, ecx
        mov    dword ptr [esp + 18h], edx
        mov    ecx, dword ptr [ebp + 2fh]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    eax, 8
        or     edx, eax
        shr    ecx, 18h
        or     edx, ecx
        mov    dword ptr [esp + 1ch], edx
        mov    ax, word ptr [ebp + 35h]
        mov    cx, ax
        and    ecx, 0ffh
        shr    ax, 8
        shl    ecx, 8
        movzx  eax, ax
        or     ecx, eax
        mov    dword ptr [esp + 20h], ecx
        mov    ax, word ptr [ebp + 39h]
        mov    cx, ax
        and    ecx, 0ffh
        shr    ax, 8
        shl    ecx, 8
        movzx  eax, ax
        or     ecx, eax
        mov    dword ptr [esp + 24h], ecx
        movzx  eax, word ptr [ebp + 33h]
        mov    dword ptr [esp + 28h], eax
        mov    ecx, dword ptr [ebp + 23h]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    ecx, 10h
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        mov    dword ptr [esp + 2ch], edx
        mov    al, byte ptr [ebp + 1]
        mov    byte ptr [esp + 34h], al
        mov    al, byte ptr [ebp + 33h]
        and    al, 0f3h
        shr    al, 2
        movzx  eax, al
        cmp    eax, dword ptr [esp + 40h]
        ja     NEAR PTR loc_1_A3E9
        sub    dword ptr [esp + 40h], eax
        sub    dword ptr [ebx + 0ch], eax
        add    dword ptr [ebx + 8], eax
        push   dword ptr [esp + 34h]
        lea    esi, [ebp + 3]
        push   dword ptr [esp + 48h]
        push   esi
        lea    eax, [ebp + 13h]
        push   eax
        call   sub_1_C92C
        add    esp, 10h
        mov    edi, eax
        or     edi, edi
        je     NEAR PTR loc_1_A673
        db  0A1h, 030h, 038h, 001h, 000h
        mov    dword ptr [edi + 0ceh], eax
        mov    eax, dword ptr [esp + 18h]
        cmp    dword ptr [edi + 48h], eax
        jne    NEAR PTR loc_1_9D5A
        mov    eax, dword ptr [esp + 28h]
        and    eax, 3700h
        cmp    eax, dword ptr [edi + 64h]
        jne    NEAR PTR loc_1_9D5A
        sub    esi, esi
        inc    dword ptr [edi + 0a6h]
        mov    ecx, dword ptr [edi + 0ch]
        mov    eax, ecx
        sub    eax, dword ptr [esp + 1ch]
        jns    NEAR PTR loc_1_9C4F
        mov    eax, dword ptr [esp + 1ch]
        sub    eax, dword ptr [edi + 14h]
        cmp    eax, esi
        jg     NEAR PTR loc_1_9C4F
        push   dword ptr [esp + 1ch]
        push   edi
        call   sub_1_930B
        add    esp, 8
        mov    ecx, dword ptr [edi + 0c4h]
        cmp    ecx, esi
        je     SHORT loc_1_9BF3
        mov    eax, dword ptr [esp + 1ch]
        sub    eax, dword ptr [edi + 0aah]
        cmp    eax, esi
        jle    SHORT loc_1_9BF3
        db  0A1h, 030h, 038h, 001h, 000h
        sub    ax, cx
        movsx  ecx, ax
        mov    dword ptr [edi + 0c4h], esi
        mov    eax, dword ptr [edi + 0aeh]
        mov    edx, eax
        shr    edx, 3
        sub    ecx, edx
        lea    edx, [eax + ecx]
        mov    dword ptr [edi + 0aeh], edx
        movsx  ecx, cx
        mov    eax, dword ptr [edi + 0b2h]
        mov    esi, eax
        shr    esi, 3
        sub    eax, esi
        add    eax, ecx
        mov    dword ptr [edi + 0b2h], eax
        shr    edx, 2
        add    edx, eax
        shr    edx, 1
        cmp    edx, 0ah
        ja     SHORT loc_1_9BE0
        mov    edx, 0ah
loc_1_9BE0:
        cmp    edx, 3e8h
        jb     SHORT loc_1_9BED
        mov    edx, 3e8h
loc_1_9BED:
        mov    dword ptr [edi + 0a2h], edx
loc_1_9BF3:
        mov    esi, dword ptr [edi + 24h]
        mov    eax, dword ptr [edi + 9eh]
        mov    ecx, dword ptr [edi + 20h]
        cmp    esi, ecx
        jae    SHORT loc_1_9C16
        cmp    dword ptr [edi + 28h], esi
        jbe    SHORT loc_1_9C0A
        jmp    SHORT loc_1_9C11
loc_1_9C0A:
        imul   eax, eax
        sub    edx, edx
        div    esi
loc_1_9C11:
        add    esi, eax
        mov    dword ptr [edi + 24h], esi
loc_1_9C16:
        mov    byte ptr [edi + 0bdh], 0
        mov    eax, dword ptr [esp + 1ch]
        cmp    dword ptr [edi + 14h], eax
        jne    SHORT loc_1_9C32
        mov    dword ptr [edi + 0c8h], 0
        jmp    SHORT loc_1_9C3E
loc_1_9C32:
        mov    eax, dword ptr [edi + 0a2h]
        mov    dword ptr [edi + 0c8h], eax
loc_1_9C3E:
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [edi + 18h], eax
        mov    eax, dword ptr [esp + 20h]
        cmp    eax, ecx
        ja     SHORT loc_1_9CA3
        jmp    SHORT loc_1_9CA1
loc_1_9C4F:
        mov    eax, dword ptr [esp + 1ch]
        sub    eax, dword ptr [edi + 14h]
        or     eax, eax
        jle    SHORT loc_1_9C6D
        lea    eax, [esp + 18h]
        push   edi
        push   eax
        call   sub_1_92D5
        add    esp, 8
        jmp    NEAR PTR loc_1_A3E9
loc_1_9C6D:
        cmp    dword ptr [esp + 1ch], ecx
        jne    SHORT loc_1_9CC1
        mov    eax, dword ptr [edi + 50h]
        mov    ecx, eax
        sub    ecx, dword ptr [esp + 18h]
        js     SHORT loc_1_9C8F
        cmp    dword ptr [esp + 18h], eax
        jne    SHORT loc_1_9CC1
        mov    eax, dword ptr [edi + 54h]
        sub    eax, dword ptr [esp + 1ch]
        or     eax, eax
        jg     SHORT loc_1_9CC1
loc_1_9C8F:
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [edi + 18h], eax
        mov    eax, dword ptr [edi + 20h]
        mov    ecx, dword ptr [esp + 20h]
        cmp    eax, ecx
        ja     SHORT loc_1_9CA3
loc_1_9CA1:
        mov    eax, ecx
loc_1_9CA3:
        mov    dword ptr [edi + 20h], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [edi + 50h], eax
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [edi + 54h], eax
        cmp    dword ptr [edi + 1ch], 1
        cmc
        sbb    esi, esi
        and    esi, 400h
loc_1_9CC1:
        mov    ebp, dword ptr [edi + 4ch]
        mov    eax, dword ptr [esp + 40h]
        cmp    ebp, eax
        jb     SHORT loc_1_9CCE
        mov    ebp, eax
loc_1_9CCE:
        or     ebp, ebp
        jne    SHORT loc_1_9CE3
        cmp    dword ptr [esp + 40h], 1
        cmc
        sbb    eax, eax
        and    eax, 200h
        or     esi, eax
        jmp    SHORT loc_1_9D43
loc_1_9CE3:
        push   ebp
        push   ebx
        or     dword ptr [edi + 64h], 2
        push   dword ptr [esp + 30h]
        push   edi
        call   dword ptr [edi + 68h]
        add    esp, 10h
        add    dword ptr [edi + 48h], eax
        sub    dword ptr [edi + 4ch], eax
        and    dword ptr [edi + 64h], 0fffffffdh
        mov    edx, dword ptr [edi + 60h]
        mov    ecx, edx
        and    ecx, 800000h
        cmp    ecx, 1
        cmc
        sbb    ecx, ecx
        and    ecx, 400h
        or     esi, ecx
        and    edx, 0ff7fffffh
        mov    dword ptr [edi + 60h], edx
        test   dh, 8
        jne    SHORT loc_1_9D3D
        cmp    ebp, eax
        jne    SHORT loc_1_9D3D
        or     edx, 800h
        mov    dword ptr [edi + 60h], edx
        mov    word ptr [edi + 0cch], 3
        jmp    SHORT loc_1_9D43
loc_1_9D3D:
        or     esi, 200h
loc_1_9D43:
        or     esi, esi
        je     NEAR PTR loc_1_A3E0
        push   esi
        push   edi
        call   sub_1_8EB3
        add    esp, 8
        jmp    NEAR PTR loc_1_A3E0
loc_1_9D5A:
        mov    al, byte ptr [edi + 0bch]
        cmp    al, 3
        jne    SHORT loc_1_9D6E
        test   byte ptr [edi + 60h], 4
        je     NEAR PTR loc_1_A3E9
loc_1_9D6E:
        cmp    al, 4
        je     SHORT loc_1_9D7D
        test   byte ptr [edi + 64h], 2
        je     SHORT loc_1_9D7D
        jmp    NEAR PTR loc_1_A3E9
loc_1_9D7D:
        or     al, al
        je     NEAR PTR loc_1_A661
        test   byte ptr [edi + 0beh], 1
        jne    NEAR PTR loc_1_A661
        mov    ecx, dword ptr [edi + 60h]
        and    ecx, 1200000h
        cmp    ecx, 1000000h
        je     NEAR PTR loc_1_A661
        cmp    al, 2
        jne    NEAR PTR loc_1_9EFD
        test   byte ptr [esp + 29h], 10h
        je     SHORT loc_1_9DD1
        mov    eax, dword ptr [esp + 1ch]
        cmp    dword ptr [edi + 10h], eax
        je     SHORT loc_1_9DD1
        push   dword ptr [esp + 44h]
        push   ebp
        call   sub_1_AEAA
        add    esp, 8
        jmp    NEAR PTR loc_1_A3E9
loc_1_9DD1:
        test   byte ptr [esp + 29h], 4
        je     SHORT loc_1_9E11
        movzx  ecx, byte ptr [edi + 0bdh]
        db  03Bh, 00Dh, 010h, 02Bh, 001h, 000h
        jne    SHORT loc_1_9E02
        push   0eh
        push   edi
        call   sub_1_8FCD
        add    esp, 8
        push   10h
        push   edi
        call   sub_1_4B72
        add    esp, 8
        jmp    NEAR PTR loc_1_A3E9
loc_1_9E02:
        mov    dword ptr [edi + 0c8h], 5
        jmp    NEAR PTR loc_1_A3E9
loc_1_9E11:
        test   byte ptr [esp + 29h], 2
        je     NEAR PTR loc_1_A3E9
        inc    dword ptr [edi + 0a6h]
        inc    dword ptr [esp + 18h]
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [edi + 48h], eax
        test   byte ptr [esp + 29h], 20h
        je     SHORT loc_1_9E49
        cmp    dword ptr [esp + 24h], 0
        je     SHORT loc_1_9E41
        dec    dword ptr [esp + 24h]
        jmp    SHORT loc_1_9E49
loc_1_9E41:
        and    dword ptr [esp + 28h], 0ffffdfffh
loc_1_9E49:
        db  0A1h, 0A0h, 036h, 001h, 000h
        sub    eax, 3ch
        mov    dword ptr [edi + 0b6h], eax
        mov    dword ptr [edi + 9eh], eax
        mov    byte ptr [edi + 0bdh], 0
        mov    dword ptr [edi + 0c8h], 0
        test   byte ptr [esp + 29h], 10h
        je     SHORT loc_1_9EE2
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [edi + 0ch], eax
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [edi + 18h], eax
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [edi + 20h], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [edi + 50h], eax
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [edi + 54h], eax
        push   edi
        call   sub_1_B0F6
        add    esp, 4
        push   0
        push   edi
        call   sub_1_8FCD
        add    esp, 8
        push   edi
        call   sub_1_ABE1
        add    esp, 4
        test   byte ptr [edi + 0beh], 1
        jne    NEAR PTR loc_1_A3E0
        and    dword ptr [esp + 28h], 0fffffdffh
        mov    eax, dword ptr [esp + 28h]
        and    ah, 37h
        cmp    ah, 10h
        jne    SHORT loc_1_9F09
        cmp    dword ptr [esp + 40h], 0
        jne    SHORT loc_1_9F09
        jmp    NEAR PTR loc_1_A3E0
loc_1_9EE2:
        mov    byte ptr [edi + 0bch], 3
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [edi + 10h], eax
        push   edi
        call   sub_1_A929
        add    esp, 4
        jmp    NEAR PTR loc_1_A3E0
loc_1_9EFD:
        lea    eax, [edi + 0a6h]
        inc    dword ptr [edi + 0a6h]
loc_1_9F09:
        cmp    dword ptr [edi + 4ch], 0
        mov    eax, dword ptr [esp + 18h]
        je     NEAR PTR loc_1_9FFF
        sub    eax, dword ptr [edi + 48h]
        jns    SHORT loc_1_9F93
        test   byte ptr [esp + 29h], 2
        je     SHORT loc_1_9F33
        and    dword ptr [esp + 28h], 0fffffdffh
        inc    dword ptr [esp + 18h]
        dec    dword ptr [esp + 24h]
loc_1_9F33:
        mov    ecx, dword ptr [edi + 48h]
        sub    ecx, dword ptr [esp + 18h]
        mov    eax, dword ptr [esp + 28h]
        and    eax, 100h
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        add    eax, dword ptr [esp + 40h]
        cmp    eax, ecx
        ja     SHORT loc_1_9F64
        lea    eax, [esp + 18h]
        push   edi
        push   eax
        call   sub_1_92D5
        add    esp, 8
        jmp    NEAR PTR loc_1_A3E9
loc_1_9F64:
        sub    dword ptr [esp + 40h], ecx
        add    dword ptr [esp + 18h], ecx
        sub    dword ptr [esp + 24h], ecx
        push   ecx
        push   ebx
        call   sub_1_9588
        add    esp, 8
        mov    ebx, eax
        cmp    dword ptr [esp + 24h], 0
        jge    SHORT loc_1_9F93
        mov    dword ptr [esp + 24h], 0
        and    dword ptr [esp + 28h], 0ffffdfffh
loc_1_9F93:
        mov    ecx, dword ptr [esp + 28h]
        and    ecx, 100h
        mov    eax, dword ptr [esp + 28h]
        and    eax, 200h
        cmp    eax, 1
        sbb    edx, edx
        inc    edx
        cmp    ecx, 1
        sbb    eax, eax
        inc    eax
        add    edx, eax
        add    edx, dword ptr [esp + 40h]
        je     SHORT loc_1_9FBB
        dec    edx
loc_1_9FBB:
        mov    eax, dword ptr [edi + 48h]
        mov    esi, dword ptr [edi + 4ch]
        lea    eax, [eax + esi - 1]
        mov    esi, dword ptr [esp + 18h]
        sub    esi, eax
        lea    eax, [edx + esi]
        or     eax, eax
        jle    SHORT loc_1_A043
        or     esi, esi
        jle    SHORT loc_1_9FE9
        lea    eax, [esp + 18h]
        push   edi
        push   eax
        call   sub_1_92D5
        add    esp, 8
        jmp    NEAR PTR loc_1_A3E9
loc_1_9FE9:
        or     ecx, ecx
        je     SHORT loc_1_9FF6
        and    dword ptr [esp + 28h], 0fffffeffh
        dec    edx
loc_1_9FF6:
        lea    eax, [edx + esi]
        sub    dword ptr [esp + 40h], eax
        jmp    SHORT loc_1_A043
loc_1_9FFF:
        cmp    dword ptr [edi + 48h], eax
        je     SHORT loc_1_A017
        lea    eax, [esp + 18h]
        push   edi
        push   eax
        call   sub_1_92D5
        add    esp, 8
        jmp    NEAR PTR loc_1_A3E9
loc_1_A017:
        cmp    dword ptr [esp + 40h], 0
        jne    SHORT loc_1_A025
        test   byte ptr [esp + 29h], 3
        je     SHORT loc_1_A043
loc_1_A025:
        and    dword ptr [esp + 28h], 0fffffcffh
        mov    dword ptr [esp + 40h], 0
        push   200h
        push   edi
        call   sub_1_8EB3
        add    esp, 8
loc_1_A043:
        test   byte ptr [esp + 29h], 4
        je     SHORT loc_1_A08F
        cmp    byte ptr [edi + 0bch], 3
        jne    SHORT loc_1_A05D
        test   byte ptr [edi + 60h], 8
        je     SHORT loc_1_A05D
        mov    al, 10h
        jmp    SHORT loc_1_A05F
loc_1_A05D:
        mov    al, 80h
loc_1_A05F:
        push   eax
        push   edi
        call   sub_1_4B72
        add    esp, 8
        cmp    byte ptr [edi + 0bch], 0ah
        je     NEAR PTR loc_1_A3E0
        push   edi
        call   sub_1_48EE
        add    esp, 4
        push   14h
        push   edi
        call   sub_1_78CF
        add    esp, 8
        jmp    NEAR PTR loc_1_A3E0
loc_1_A08F:
        test   byte ptr [esp + 29h], 2
        je     SHORT loc_1_A0B9
        dec    dword ptr [edi + 0a6h]
        push   40h
        push   edi
        call   sub_1_4B72
        add    esp, 8
        push   dword ptr [esp + 44h]
        push   ebp
        call   sub_1_AEAA
        add    esp, 8
        jmp    NEAR PTR loc_1_A3E9
loc_1_A0B9:
        test   byte ptr [esp + 29h], 10h
        je     NEAR PTR loc_1_A3E0
        cmp    byte ptr [edi + 0bch], 3
        mov    ecx, dword ptr [edi + 0ch]
        mov    eax, ecx
        jne    SHORT loc_1_A145
        sub    eax, dword ptr [esp + 1ch]
        jns    SHORT loc_1_A12A
        mov    eax, dword ptr [esp + 1ch]
        sub    eax, dword ptr [edi + 14h]
        or     eax, eax
        jg     SHORT loc_1_A12A
        mov    byte ptr [edi + 0bdh], 0
        mov    dword ptr [edi + 0c8h], 0
        inc    ecx
        mov    dword ptr [edi + 0ch], ecx
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [edi + 18h], eax
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [edi + 20h], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [edi + 50h], eax
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [edi + 54h], eax
        push   edi
        call   sub_1_B0F6
        add    esp, 4
        push   0
        push   edi
        call   sub_1_8FCD
        jmp    NEAR PTR loc_1_A3CF
loc_1_A12A:
        push   edi
        call   sub_1_4BB1
        add    esp, 4
        push   dword ptr [esp + 44h]
        push   ebp
        call   sub_1_AEAA
        add    esp, 8
        jmp    NEAR PTR loc_1_A3E9
loc_1_A145:
        sub    eax, dword ptr [esp + 1ch]
        jns    NEAR PTR loc_1_A2C7
        mov    eax, dword ptr [esp + 1ch]
        sub    eax, dword ptr [edi + 14h]
        or     eax, eax
        jg     NEAR PTR loc_1_A2C7
        push   dword ptr [esp + 1ch]
        push   edi
        call   sub_1_930B
        add    esp, 8
        mov    ecx, dword ptr [edi + 0c4h]
        or     ecx, ecx
        je     SHORT loc_1_A1F1
        mov    eax, dword ptr [esp + 1ch]
        sub    eax, dword ptr [edi + 0aah]
        or     eax, eax
        jle    SHORT loc_1_A1F1
        db  0A1h, 030h, 038h, 001h, 000h
        sub    ax, cx
        mov    dword ptr [edi + 0c4h], 0
        mov    edx, dword ptr [edi + 0aeh]
        mov    ecx, edx
        shr    ecx, 3
        sub    ax, cx
        movsx  eax, ax
        lea    ecx, [edx + eax]
        mov    dword ptr [edi + 0aeh], ecx
        cdq
        xor    eax, edx
        sub    eax, edx
        mov    esi, dword ptr [edi + 0b2h]
        mov    edx, esi
        shr    edx, 3
        sub    ax, dx
        movsx  eax, ax
        add    eax, esi
        mov    dword ptr [edi + 0b2h], eax
        shr    ecx, 2
        add    ecx, eax
        shr    ecx, 1
        cmp    ecx, 0ah
        ja     SHORT loc_1_A1DE
        mov    ecx, 0ah
loc_1_A1DE:
        cmp    ecx, 3e8h
        jb     SHORT loc_1_A1EB
        mov    ecx, 3e8h
loc_1_A1EB:
        mov    dword ptr [edi + 0a2h], ecx
loc_1_A1F1:
        mov    ecx, dword ptr [edi + 24h]
        mov    esi, dword ptr [edi + 20h]
        cmp    esi, ecx
        jbe    SHORT loc_1_A220
        cmp    dword ptr [edi + 28h], ecx
        jbe    SHORT loc_1_A208
        add    ecx, dword ptr [edi + 9eh]
        jmp    SHORT loc_1_A217
loc_1_A208:
        mov    eax, dword ptr [edi + 9eh]
        imul   eax, eax
        sub    edx, edx
        div    ecx
        add    ecx, eax
loc_1_A217:
        cmp    esi, ecx
        jb     SHORT loc_1_A21D
        mov    esi, ecx
loc_1_A21D:
        mov    dword ptr [edi + 24h], esi
loc_1_A220:
        mov    byte ptr [edi + 0bdh], 0
        mov    ecx, dword ptr [edi + 14h]
        cmp    dword ptr [esp + 1ch], ecx
        lea    esi, [edi + 0c8h]
        je     SHORT loc_1_A240
        mov    eax, dword ptr [edi + 0a2h]
        mov    dword ptr [esi], eax
        jmp    SHORT loc_1_A246
loc_1_A240:
        mov    dword ptr [esi], 0
loc_1_A246:
        mov    eax, dword ptr [edi + 60h]
        test   eax, 20000000h
        je     SHORT loc_1_A274
        cmp    dword ptr [esp + 1ch], ecx
        jne    SHORT loc_1_A274
        and    eax, 0dffeffffh
        mov    dword ptr [edi + 60h], eax
        movzx  eax, byte ptr [edi + 0bch]
        cmp    eax, 5
        je     SHORT loc_1_A27E
        cmp    eax, 8
        je     SHORT loc_1_A2A3
        cmp    eax, 9
        je     SHORT loc_1_A2B5
loc_1_A274:
        mov    eax, 1
        jmp    NEAR PTR loc_1_A310
loc_1_A27E:
        mov    byte ptr [edi + 0bch], 6
        push   0
        push   edi
        call   sub_1_8FCD
        add    esp, 8
        db  0A0h, 018h, 02Bh, 001h, 000h
        mov    byte ptr [edi + 0bdh], al
        mov    dword ptr [esi], 0ah
        jmp    SHORT loc_1_A274
loc_1_A2A3:
        push   0
        push   1
        push   edi
        call   sub_1_7998
        add    esp, 0ch
        jmp    NEAR PTR loc_1_A3E9
loc_1_A2B5:
        push   0
        push   0
        push   edi
        call   sub_1_7998
        add    esp, 0ch
        jmp    NEAR PTR loc_1_A3E9
loc_1_A2C7:
        mov    eax, dword ptr [esp + 1ch]
        sub    eax, dword ptr [edi + 14h]
        or     eax, eax
        jle    SHORT loc_1_A2E5
        lea    eax, [esp + 18h]
        push   edi
        push   eax
        call   sub_1_92D5
        add    esp, 8
        jmp    NEAR PTR loc_1_A3E9
loc_1_A2E5:
        cmp    dword ptr [esp + 1ch], ecx
        jne    SHORT loc_1_A30E
        mov    eax, dword ptr [edi + 50h]
        mov    ecx, eax
        sub    ecx, dword ptr [esp + 18h]
        js     SHORT loc_1_A307
        cmp    dword ptr [esp + 18h], eax
        jne    SHORT loc_1_A30E
        mov    eax, dword ptr [edi + 54h]
        sub    eax, dword ptr [esp + 1ch]
        or     eax, eax
        jg     SHORT loc_1_A30E
loc_1_A307:
        mov    eax, 1
        jmp    SHORT loc_1_A310
loc_1_A30E:
        sub    eax, eax
loc_1_A310:
        or     eax, eax
        je     NEAR PTR loc_1_A3D2
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [edi + 18h], eax
        mov    eax, dword ptr [edi + 20h]
        mov    ecx, dword ptr [esp + 20h]
        cmp    eax, ecx
        ja     SHORT loc_1_A32C
        mov    eax, ecx
loc_1_A32C:
        mov    dword ptr [edi + 20h], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [edi + 50h], eax
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [edi + 54h], eax
        cmp    dword ptr [esp + 20h], 0
        jne    SHORT loc_1_A38B
        mov    eax, dword ptr [edi + 40h]
        sub    eax, edi
        cmp    eax, 40h
        je     NEAR PTR loc_1_A3D2
        mov    eax, dword ptr [edi + 60h]
        and    eax, 0fffffbffh
        mov    dword ptr [edi + 60h], eax
        mov    byte ptr [edi + 0bdh], 0
        mov    ecx, dword ptr [edi + 0a2h]
        mov    dword ptr [edi + 0c8h], ecx
        test   eax, 80000h
        jne    SHORT loc_1_A3D2
        or     eax, 80000h
        mov    dword ptr [edi + 60h], eax
        inc    byte ptr [edi + 0bah]
        or     dword ptr [edi + 64h], 1
        jmp    SHORT loc_1_A3D2
loc_1_A38B:
        mov    eax, dword ptr [edi + 60h]
        test   eax, 80000h
        je     SHORT loc_1_A3B8
        mov    byte ptr [edi + 0bdh], 0
        and    eax, 0ffb7ffffh
        mov    dword ptr [edi + 60h], eax
        mov    al, byte ptr [edi + 0bah]
        dec    al
        mov    byte ptr [edi + 0bah], al
        jne    SHORT loc_1_A3B8
        and    dword ptr [edi + 64h], 0fffffffeh
loc_1_A3B8:
        cmp    dword ptr [edi + 1ch], 0
        jne    SHORT loc_1_A3C4
        test   byte ptr [edi + 62h], 1
        je     SHORT loc_1_A3D2
loc_1_A3C4:
        push   400h
        push   edi
        call   sub_1_8EB3
loc_1_A3CF:
        add    esp, 8
loc_1_A3D2:
        cmp    dword ptr [esp + 40h], 0
        ja     SHORT loc_1_A3F3
        test   byte ptr [esp + 29h], 1
        jne    SHORT loc_1_A3F3
loc_1_A3E0:
        push   edi
loc_1_A3E1:
        call   sub_1_4BB1
loc_1_A3E6:
        add    esp, 4
loc_1_A3E9:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
loc_1_A3F3:
        mov    cl, byte ptr [edi + 0bch]
        cmp    cl, 4
        jb     NEAR PTR loc_1_A64E
        cmp    cl, 6
        ja     NEAR PTR loc_1_A64E
        mov    eax, dword ptr [edi + 60h]
        and    eax, 1000000h
        jne    NEAR PTR loc_1_A64E
        mov    edx, dword ptr [esp + 18h]
        cmp    dword ptr [edi + 48h], edx
        jne    NEAR PTR loc_1_A603
        mov    eax, dword ptr [edi + 64h]
        test   al, 2
        je     SHORT loc_1_A42F
        jmp    SHORT loc_1_A3E0
loc_1_A42F:
        or     eax, 2
        mov    dword ptr [edi + 64h], eax
        mov    esi, dword ptr [esp + 14h]
        mov    ebp, 2000h
loc_1_A43E:
        test   dword ptr [esp + 28h], ebp
        je     SHORT loc_1_A465
        lea    eax, [esp + 40h]
        push   eax
        lea    eax, [esp + 1ch]
        push   ebx
        push   eax
        push   edi
        call   sub_1_8832
        add    esp, 10h
        test   byte ptr [edi + 0beh], 1
        jne    NEAR PTR loc_1_A5D6
loc_1_A465:
        cmp    dword ptr [esp + 40h], 0
        jbe    SHORT loc_1_A4C4
        push   dword ptr [esp + 40h]
        push   ebx
        push   dword ptr [esp + 30h]
        push   edi
        call   dword ptr [edi + 68h]
        add    esp, 10h
        mov    ebx, eax
        add    dword ptr [edi + 48h], ebx
        sub    dword ptr [edi + 4ch], ebx
        mov    eax, dword ptr [edi + 60h]
        test   ah, 8
        je     SHORT loc_1_A49C
        push   200h
        push   edi
        call   sub_1_8EB3
        add    esp, 8
        jmp    SHORT loc_1_A4AD
loc_1_A49C:
        or     eax, 800h
        mov    dword ptr [edi + 60h], eax
        mov    word ptr [edi + 0cch], 3
loc_1_A4AD:
        cmp    ebx, dword ptr [esp + 40h]
        jne    NEAR PTR loc_1_A548
        test   byte ptr [edi + 0beh], 1
        jne    NEAR PTR loc_1_A5D6
loc_1_A4C4:
        mov    ecx, dword ptr [edi + 60h]
        test   ch, 80h
        je     SHORT loc_1_A51F
        test   ecx, 4000000h
        jne    SHORT loc_1_A4EF
        mov    eax, dword ptr [edi + 48h]
        cmp    dword ptr [edi + 12eh], eax
        jne    SHORT loc_1_A4EB
        mov    eax, dword ptr [edi + 132h]
        inc    eax
        mov    dword ptr [edi + 48h], eax
        jmp    SHORT loc_1_A4F5
loc_1_A4EB:
        sub    esi, esi
        jmp    SHORT loc_1_A4F5
loc_1_A4EF:
        db  08Bh, 035h, 008h, 02Bh, 001h, 000h
loc_1_A4F5:
        mov    eax, dword ptr [edi + 48h]
        sub    eax, dword ptr [edi + 132h]
        sub    eax, esi
        je     SHORT loc_1_A51F
        and    ecx, 0ffff7fffh
        mov    dword ptr [edi + 60h], ecx
        mov    al, byte ptr [edi + 0bah]
        dec    al
        mov    byte ptr [edi + 0bah], al
        jne    SHORT loc_1_A51F
        and    dword ptr [edi + 64h], 0fffffffeh
loc_1_A51F:
        test   byte ptr [esp + 29h], 1
        jne    SHORT loc_1_A555
        lea    eax, [esp + 40h]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        push   edi
        call   sub_1_95E6
        add    esp, 0ch
        mov    ebx, eax
        or     ebx, ebx
        jne    NEAR PTR loc_1_A43E
        jmp    NEAR PTR loc_1_A5D6
loc_1_A548:
        push   200h
        push   edi
        call   sub_1_8EB3
        jmp    SHORT loc_1_A5D3
loc_1_A555:
        sub    esi, esi
        inc    dword ptr [edi + 48h]
        push   200h
        push   edi
        call   sub_1_8EB3
        add    esp, 8
        push   edi
        call   sub_1_8726
        add    esp, 4
        movzx  eax, byte ptr [edi + 0bch]
        cmp    eax, 3
        jl     SHORT loc_1_A5C7
        cmp    eax, 4
        jle    SHORT loc_1_A58E
        cmp    eax, 5
        je     SHORT loc_1_A5A6
        cmp    eax, 6
        je     SHORT loc_1_A5B4
        jmp    SHORT loc_1_A5C7
loc_1_A58E:
        mov    byte ptr [edi + 0bch], 7
        inc    byte ptr [edi + 0bah]
        or     dword ptr [edi + 64h], 1
        mov    esi, 1
        jmp    SHORT loc_1_A5C7
loc_1_A5A6:
        mov    byte ptr [edi + 0bch], 8
        mov    esi, 1
        jmp    SHORT loc_1_A5C7
loc_1_A5B4:
        inc    dword ptr [edi + 0a6h]
        push   1
        push   1
        push   edi
        call   sub_1_7998
        add    esp, 0ch
loc_1_A5C7:
        or     esi, esi
        je     SHORT loc_1_A5D6
        push   16h
        push   edi
        call   sub_1_78CF
loc_1_A5D3:
        add    esp, 8
loc_1_A5D6:
        and    dword ptr [edi + 64h], 0fffffffdh
        mov    eax, dword ptr [edi + 60h]
        test   eax, 800000h
        je     NEAR PTR loc_1_A3E0
        and    eax, 0ff7fffffh
        mov    dword ptr [edi + 60h], eax
        push   400h
        push   edi
        call   sub_1_8EB3
        add    esp, 8
        jmp    NEAR PTR loc_1_A3E0
loc_1_A603:
        cmp    cl, 4
        jb     SHORT loc_1_A63B
        cmp    cl, 6
        ja     SHORT loc_1_A63B
        or     eax, eax
        jne    SHORT loc_1_A63B
        push   dword ptr [esp + 40h]
        lea    eax, [esp + 1ch]
        push   ebx
        push   eax
        push   edi
        call   sub_1_9770
        add    esp, 10h
        push   edi
        call   sub_1_ABE1
        add    esp, 4
        push   edi
        call   sub_1_4BB1
        add    esp, 4
        jmp    NEAR PTR loc_1_A3E9
loc_1_A63B:
        lea    eax, [esp + 18h]
        push   edi
        push   eax
        call   sub_1_92D5
        add    esp, 8
        jmp    NEAR PTR loc_1_A3E9
loc_1_A64E:
        lea    eax, [esp + 18h]
        push   edi
        push   eax
        call   sub_1_92D5
        add    esp, 8
        jmp    NEAR PTR loc_1_A3E9
loc_1_A661:
        push   dword ptr [esp + 44h]
        push   ebp
        call   sub_1_AEAA
        add    esp, 8
        jmp    NEAR PTR loc_1_A3E9
loc_1_A673:
        test   byte ptr [esp + 29h], 4
        je     SHORT loc_1_A67F
        jmp    NEAR PTR loc_1_A3E9
loc_1_A67F:
        mov    eax, dword ptr [esp + 28h]
        and    ah, 12h
        cmp    ah, 2
        jne    NEAR PTR loc_1_A793
        push   0
        mov    al, byte ptr [ebp + 2]
        push   eax
        lea    eax, [ebp + 13h]
        push   dword ptr [esp + 4ch]
        push   eax
        call   sub_1_337D
        add    esp, 10h
        or     eax, eax
        je     NEAR PTR loc_1_A793
        push   dword ptr [esp + 44h]
        push   esi
        push   eax
        call   sub_1_902C
        add    esp, 0ch
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_1_A793
        lea    edi, [ebx + 7ch]
        movsd
        movsd
        movsd
        movsd
        lea    edi, [ebx + 6ch]
        lea    esi, [ebp + 13h]
        movsd
        movsd
        movsd
        movsd
        mov    eax, dword ptr [esp + 44h]
        mov    dword ptr [ebx + 8ch], eax
        mov    eax, dword ptr [esp + 30h]
        mov    dword ptr [ebx + 9ah], eax
        mov    al, byte ptr [esp + 34h]
        mov    byte ptr [ebx + 98h], al
        inc    dword ptr [esp + 18h]
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [ebx + 48h], eax
        db  0A1h, 0A0h, 036h, 001h, 000h
        sub    eax, 3ch
        mov    dword ptr [ebx + 0b6h], eax
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [ebx + 18h], eax
        mov    byte ptr [ebx + 0bch], 3
        or     dword ptr [ebx + 64h], 2
        push   ebx
        call   sub_1_CCC6
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_A74D
        push   13h
        push   ebx
        call   sub_1_8FCD
        add    esp, 8
        dec    dword ptr [ebx + 0a6h]
        push   40h
        push   ebx
        call   sub_1_4B72
        add    esp, 8
        jmp    NEAR PTR loc_1_A3E9
loc_1_A74D:
        and    dword ptr [ebx + 64h], 0fffffffdh
        mov    eax, dword ptr [ebx + 60h]
        test   eax, 800000h
        je     SHORT loc_1_A771
        and    eax, 0ff7fffffh
        mov    dword ptr [ebx + 60h], eax
        push   400h
        push   ebx
        call   sub_1_8EB3
        add    esp, 8
loc_1_A771:
        test   byte ptr [ebx + 60h], 4
        jne    SHORT loc_1_A788
        push   0
        push   ebx
        call   sub_1_8FCD
        add    esp, 8
        push   ebx
        jmp    NEAR PTR loc_1_A3E1
loc_1_A788:
        push   ebx
        call   sub_1_7670
        jmp    NEAR PTR loc_1_A3E6
loc_1_A793:
        push   dword ptr [esp + 44h]
        push   ebp
        call   sub_1_AEAA
        add    esp, 8
        jmp    NEAR PTR loc_1_A3E9
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_A7B0:
        db  0A1h, 024h, 038h, 001h, 000h
        or     eax, eax
        je     SHORT loc_1_A7C1
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 024h, 038h, 001h, 000h
loc_1_A7C1:
        ret
sub_1_A7C2:
        mov    ecx, dword ptr [esp + 4]
        db  0A1h, 024h, 038h, 001h, 000h
        mov    dword ptr [ecx], eax
        db  089h, 00Dh, 024h, 038h, 001h, 000h
        ret
sub_1_A7D4:
        mov    ecx, dword ptr [esp + 4]
        db  0A1h, 020h, 038h, 001h, 000h
        mov    dword ptr [ecx], eax
        db  089h, 00Dh, 020h, 038h, 001h, 000h
        ret
sub_1_A7E6:
        db  0A1h, 020h, 038h, 001h, 000h
        or     eax, eax
        je     SHORT loc_1_A7F9
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 020h, 038h, 001h, 000h
        jmp    SHORT loc_1_A827
loc_1_A7F9:
        db  08Bh, 00Dh, 090h, 038h, 001h, 000h
        db  039h, 00Dh, 0D8h, 02Ah, 001h, 000h
        mov    eax, 0
        jbe    SHORT loc_1_A81C
        push   34h
        call   sub_1_F390
        add    esp, 4
        db  08Bh, 00Dh, 090h, 038h, 001h, 000h
loc_1_A81C:
        or     eax, eax
        je     SHORT loc_1_A827
        inc    ecx
        db  089h, 00Dh, 090h, 038h, 001h, 000h
loc_1_A827:
        ret
sub_1_A828:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        or     esi, esi
        je     NEAR PTR loc_1_A8D5
        mov    eax, dword ptr [esp + 18h]
        sub    ecx, ecx
        mov    edi, dword ptr [eax]
        movzx  eax, word ptr [esi + 8]
loc_1_A844:
        cmp    ecx, eax
        jae    SHORT loc_1_A84D
        mov    edi, dword ptr [edi]
        inc    ecx
        jmp    SHORT loc_1_A844
loc_1_A84D:
        sub    ebx, ebx
loc_1_A84F:
        movzx  eax, word ptr [esi + 0ah]
        cmp    eax, ebx
        jbe    SHORT loc_1_A864
        mov    eax, edi
        mov    edi, dword ptr [edi]
        push   eax
        call   sub_1_1704
        inc    ebx
        jmp    SHORT loc_1_A84F
loc_1_A864:
        mov    eax, dword ptr [esi]
        sub    edi, edi
        cmp    dword ptr [esi + 4], edi
        jbe    SHORT loc_1_A8D5
        db  08Bh, 01Dh, 000h, 038h, 001h, 000h
loc_1_A873:
        mov    ebp, dword ptr [eax]
        mov    edx, dword ptr [eax + 18h]
        dec    edx
        mov    dword ptr [eax + 18h], edx
        or     edx, edx
        mov    ecx, 0
        setl   cl
        or     edx, edx
        mov    edx, 0
        setle  dl
        add    ecx, edx
        je     SHORT loc_1_A8C7
        mov    ecx, dword ptr [eax + 2ch]
        or     ecx, ecx
        je     SHORT loc_1_A8A8
        mov    dword ptr [ecx], 0
        mov    dword ptr [eax + 2ch], 0
loc_1_A8A8:
        db  08Bh, 00Dh, 024h, 037h, 001h, 000h
        or     ebx, 2
        mov    dword ptr [ecx], eax
        db  08Bh, 00Dh, 024h, 037h, 001h, 000h
        mov    dword ptr [eax + 4], ecx
        db  0A3h, 024h, 037h, 001h, 000h
        db  0C7h, 000h, 020h, 037h, 001h, 000h
loc_1_A8C7:
        db  089h, 01Dh, 000h, 038h, 001h, 000h
        mov    eax, ebp
        inc    edi
        cmp    dword ptr [esi + 4], edi
        ja     SHORT loc_1_A873
loc_1_A8D5:
        push   dword ptr [esp + 18h]
        call   sub_1_A7C2
        add    esp, 4
        db  0A1h, 000h, 038h, 001h, 000h
        test   al, 2
        jne    SHORT loc_1_A8EE
        test   al, 1
        je     SHORT loc_1_A8F3
loc_1_A8EE:
        call   sub_1_8EF7
loc_1_A8F3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_A8F8:
        push   esi
        db  0A1h, 008h, 02Bh, 001h, 000h
        mov    ecx, dword ptr [esp + 8]
        mov    edx, eax
        sub    edx, dword ptr [ecx + 0deh]
        mov    esi, dword ptr [ecx + 9eh]
        shr    eax, 1
        cmp    esi, eax
        jb     SHORT loc_1_A918
        mov    esi, eax
loc_1_A918:
        mov    eax, edx
        sub    eax, dword ptr [ecx + 4ch]
        cmp    esi, eax
        ja     SHORT loc_1_A924
        mov    dword ptr [ecx + 4ch], edx
loc_1_A924:
        mov    eax, dword ptr [ecx + 4ch]
        pop    esi
        ret
sub_1_A929:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_A7B0
        mov    ebp, dword ptr [esp + 18h]
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [ebp + 0a2h]
        mov    dword ptr [ebp + 0c8h], eax
        cmp    dword ptr [esp + 10h], 0
        je     NEAR PTR loc_1_AA91
        mov    eax, dword ptr [esp + 10h]
        mov    ebx, dword ptr [eax + 4]
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 0ch], 3ch
        mov    al, byte ptr [ebp + 98h]
        lea    esi, [ebp + 6ch]
        mov    byte ptr [ebx], al
        mov    byte ptr [ebx + 1], al
        mov    eax, dword ptr [ebp + 94h]
        mov    dword ptr [ebx + 23h], eax
        lea    edi, [ebx + 3]
        mov    al, byte ptr [ebp + 99h]
        mov    byte ptr [ebx + 2], al
        movsd
        movsd
        movsd
        movsd
        lea    esi, [ebp + 7ch]
        lea    edi, [ebx + 13h]
        movsd
        movsd
        movsd
        movsd
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    eax, 8
        or     edx, eax
        mov    eax, ecx
        shr    eax, 18h
        or     edx, eax
        mov    dword ptr [ebx + 2bh], edx
        inc    ecx
        mov    dword ptr [ebp + 10h], ecx
        mov    eax, ecx
        sub    eax, dword ptr [ebp + 14h]
        or     eax, eax
        jle    SHORT loc_1_A9D4
        mov    dword ptr [ebp + 14h], ecx
loc_1_A9D4:
        mov    ecx, dword ptr [ebp + 48h]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    eax, 8
        or     edx, eax
        shr    ecx, 18h
        or     edx, ecx
        mov    dword ptr [ebx + 2fh], edx
        cmp    byte ptr [ebp + 0bch], 3
        mov    word ptr [ebx + 33h], 12f0h
        je     SHORT loc_1_AA12
        mov    word ptr [ebx + 33h], 2f0h
loc_1_AA12:
        push   dword ptr [ebp + 8ch]
        call   sub_1_35D1
        add    esp, 4
        mov    esi, eax
        push   dword ptr [esp + 10h]
        mov    ax, word ptr [ebp + 4ch]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx + 35h], cx
        mov    eax, dword ptr [ebp + 2ch]
        add    eax, 3c00h
        push   eax
        call   sub_1_4380
        add    esp, 8
        not    ax
        push   2
        push   0
        push   3ch
        mov    word ptr [ebx + 37h], ax
        push   dword ptr [esp + 1ch]
        push   dword ptr [ebp + 8ch]
        push   12h
        call   sub_1_D7E0
        add    esp, 18h
        cmp    eax, 3ch
        je     SHORT loc_1_AA8A
        push   2
        push   2
        push   0
        push   0
        push   0
        push   3ch
        push   dword ptr [esp + 28h]
        push   esi
        push   dword ptr [ebp + 8ch]
        call   sub_1_3A0F
        add    esp, 24h
        jmp    SHORT loc_1_AA91
loc_1_AA8A:
        mov    dword ptr [esi + 41h], 0
loc_1_AA91:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_AA99:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_A7B0
        mov    dword ptr [esp + 10h], eax
        or     eax, eax
        je     NEAR PTR loc_1_ABD9
        mov    ebx, dword ptr [eax + 4]
        mov    dword ptr [eax], 0
        mov    ebp, dword ptr [esp + 18h]
        mov    dword ptr [eax + 0ch], 3dh
        mov    ecx, dword ptr [ebp + 0ch]
        dec    ecx
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    eax, 8
        or     edx, eax
        shr    ecx, 18h
        or     edx, ecx
        mov    dword ptr [ebx + 2bh], edx
        mov    ecx, dword ptr [ebp + 48h]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    eax, 8
        or     edx, eax
        shr    ecx, 18h
        or     edx, ecx
        mov    dword ptr [ebx + 2fh], edx
        mov    word ptr [ebx + 33h], 10f0h
        push   ebp
        call   sub_1_A8F8
        add    esp, 4
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx + 35h], cx
        inc    byte ptr [ebp + 0bfh]
        mov    al, byte ptr [ebp + 98h]
        mov    byte ptr [ebx], al
        lea    esi, [ebp + 6ch]
        mov    byte ptr [ebx + 1], al
        mov    eax, dword ptr [ebp + 94h]
        mov    dword ptr [ebx + 23h], eax
        mov    al, byte ptr [ebp + 99h]
        lea    edi, [ebx + 3]
        mov    byte ptr [ebx + 2], al
        movsd
        movsd
        movsd
        movsd
        lea    esi, [ebp + 7ch]
        lea    edi, [ebx + 13h]
        movsd
        movsd
        movsd
        movsd
        push   dword ptr [esp + 10h]
        mov    eax, dword ptr [ebp + 2ch]
        add    eax, 3d00h
        push   eax
        call   sub_1_4380
        add    esp, 8
        not    ax
        mov    word ptr [ebx + 37h], ax
        push   dword ptr [ebp + 8ch]
        call   sub_1_35D1
        add    esp, 4
        push   2
        push   0
        push   3dh
        mov    esi, eax
        push   dword ptr [esp + 1ch]
        push   dword ptr [ebp + 8ch]
        push   12h
        call   sub_1_D7E0
        add    esp, 18h
        cmp    eax, 3dh
        je     SHORT loc_1_ABD2
        push   2
        push   2
        push   0
        push   0
        push   0
        push   3dh
        push   dword ptr [esp + 28h]
        push   esi
        push   dword ptr [ebp + 8ch]
        call   sub_1_3A0F
        add    esp, 24h
        jmp    SHORT loc_1_ABD9
loc_1_ABD2:
        mov    dword ptr [esi + 41h], 0
loc_1_ABD9:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_ABE1:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_A7B0
        mov    dword ptr [esp + 10h], eax
        or     eax, eax
        je     NEAR PTR loc_1_AD72
        mov    ebx, dword ptr [eax + 4]
        mov    dword ptr [eax], 0
        mov    ebp, dword ptr [esp + 1ch]
        mov    dword ptr [eax + 0ch], 3ch
        mov    ecx, dword ptr [ebp + 48h]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    eax, 8
        or     edx, eax
        shr    ecx, 18h
        or     edx, ecx
        mov    dword ptr [ebx + 2fh], edx
        push   dword ptr [ebp + 8ch]
        call   sub_1_35D1
        mov    dword ptr [esp + 18h], eax
        add    esp, 4
        mov    eax, dword ptr [ebp + 18h]
        or     eax, eax
        je     SHORT loc_1_AC63
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 0ch]
        lea    eax, [eax + edx - 1]
        mov    edx, ecx
        sub    edx, eax
        js     SHORT loc_1_AC66
        mov    ecx, eax
        jmp    SHORT loc_1_AC66
loc_1_AC63:
        mov    ecx, dword ptr [ebp + 0ch]
loc_1_AC66:
        test   byte ptr [ebp + 63h], 20h
        je     SHORT loc_1_AC7C
        mov    eax, dword ptr [ebp + 14h]
        sub    eax, ecx
        cmp    eax, 1
        mov    word ptr [ebx + 33h], 11f0h
        je     SHORT loc_1_AC82
loc_1_AC7C:
        mov    word ptr [ebx + 33h], 10f0h
loc_1_AC82:
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    eax, 8
        or     edx, eax
        shr    ecx, 18h
        or     edx, ecx
        mov    dword ptr [ebx + 2bh], edx
        push   ebp
        call   sub_1_A8F8
        add    esp, 4
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx + 35h], cx
        and    dword ptr [ebp + 60h], 0fffff5ffh
        mov    word ptr [ebp + 0cch], 0
        mov    al, byte ptr [ebp + 98h]
        lea    esi, [ebp + 6ch]
        mov    byte ptr [ebx], al
        mov    byte ptr [ebx + 1], al
        mov    eax, dword ptr [ebp + 94h]
        mov    dword ptr [ebx + 23h], eax
        lea    edi, [ebx + 3]
        mov    al, byte ptr [ebp + 99h]
        mov    byte ptr [ebx + 2], al
        movsd
        movsd
        movsd
        movsd
        lea    esi, [ebp + 7ch]
        lea    edi, [ebx + 13h]
        movsd
        movsd
        movsd
        movsd
        push   dword ptr [esp + 10h]
        mov    eax, dword ptr [ebp + 2ch]
        add    eax, 3c00h
        push   eax
        call   sub_1_4380
        add    esp, 8
        not    ax
        mov    word ptr [ebx + 37h], ax
        cmp    byte ptr [ebp + 0bch], 8
        mov    esi, 6
        je     SHORT loc_1_AD26
        sub    esi, esi
loc_1_AD26:
        push   esi
        push   0
        push   3ch
        push   dword ptr [esp + 1ch]
        push   dword ptr [ebp + 8ch]
        push   12h
        call   sub_1_D7E0
        add    esp, 18h
        cmp    eax, 3ch
        je     SHORT loc_1_AD67
        push   2
        push   esi
        push   0
        push   0
        push   0
        push   3ch
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 30h]
        push   dword ptr [ebp + 8ch]
        call   sub_1_3A0F
        add    esp, 24h
        jmp    SHORT loc_1_AD72
loc_1_AD67:
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax + 41h], 0
loc_1_AD72:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_AD7A:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_A7B0
        mov    dword ptr [esp + 10h], eax
        or     eax, eax
        je     NEAR PTR loc_1_AEA2
        mov    ebx, dword ptr [eax + 4]
        mov    dword ptr [eax], 0
        mov    ebp, dword ptr [esp + 1ch]
        mov    dword ptr [eax + 0ch], 3ch
        mov    ecx, dword ptr [ebp + 14h]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    eax, 8
        or     edx, eax
        shr    ecx, 18h
        or     edx, ecx
        mov    dword ptr [ebx + 2bh], edx
        mov    word ptr [ebx + 33h], 4f0h
        mov    word ptr [ebx + 35h], 0
        mov    al, byte ptr [ebp + 98h]
        mov    byte ptr [ebx], al
        mov    byte ptr [ebx + 1], al
        mov    eax, dword ptr [ebp + 94h]
        mov    dword ptr [ebx + 23h], eax
        mov    al, byte ptr [ebp + 99h]
        mov    byte ptr [ebx + 2], al
        push   dword ptr [ebp + 8ch]
        call   sub_1_35D1
        mov    dword ptr [esp + 18h], eax
        add    esp, 4
        lea    esi, [ebp + 6ch]
        lea    edi, [ebx + 3]
        movsd
        movsd
        movsd
        movsd
        lea    esi, [ebp + 7ch]
        lea    edi, [ebx + 13h]
        movsd
        movsd
        movsd
        movsd
        push   dword ptr [esp + 10h]
        mov    eax, dword ptr [ebp + 2ch]
        add    eax, 3c00h
        push   eax
        call   sub_1_4380
        add    esp, 8
        not    ax
        push   2
        push   0
        push   3ch
        mov    word ptr [ebx + 37h], ax
        push   dword ptr [esp + 1ch]
        push   dword ptr [ebp + 8ch]
        push   12h
        call   sub_1_D7E0
        add    esp, 18h
        cmp    eax, 3ch
        je     SHORT loc_1_AE7D
        push   2
        push   2
        push   0
        push   0
        push   0
        push   3ch
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 30h]
        push   dword ptr [ebp + 8ch]
        call   sub_1_3A0F
        add    esp, 24h
        jmp    SHORT loc_1_AE88
loc_1_AE7D:
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax + 41h], 0
loc_1_AE88:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_1_AEA2
        mov    al, byte ptr [ebp + 98h]
        push   eax
        push   dword ptr [esp + 18h]
        call   sub_1_3B6C
        add    esp, 8
loc_1_AEA2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_AEAA:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 18h]
        test   byte ptr [ebp + 34h], 4
        jne    NEAR PTR loc_1_B0EE
        push   dword ptr [esp + 1ch]
        call   sub_1_35D1
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_AEDC
        mov    cl, byte ptr [ebp + 1]
        push   ecx
        push   eax
        call   sub_1_3B6C
        add    esp, 8
loc_1_AEDC:
        mov    esi, 3ch
        call   sub_1_A7B0
        mov    dword ptr [esp + 10h], eax
        or     eax, eax
        je     NEAR PTR loc_1_B0EE
        mov    ebx, dword ptr [eax + 4]
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 0ch], esi
        mov    ax, word ptr [ebp + 33h]
        test   ah, 2
        je     SHORT loc_1_AF0C
        mov    esi, 3dh
loc_1_AF0C:
        test   ah, 1
        je     SHORT loc_1_AF12
        inc    esi
loc_1_AF12:
        test   ah, 10h
        je     SHORT loc_1_AF25
        mov    eax, dword ptr [ebp + 2fh]
        mov    dword ptr [ebx + 2bh], eax
        mov    word ptr [ebx + 33h], 4f0h
        jmp    SHORT loc_1_AF80
loc_1_AF25:
        mov    dword ptr [ebx + 2bh], 0
        mov    ecx, dword ptr [ebp + 2bh]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    eax, 8
        or     edx, eax
        shr    ecx, 18h
        or     edx, ecx
        lea    ecx, [esi + edx]
        mov    edx, ecx
        shl    edx, 10h
        mov    eax, ecx
        and    eax, 0ff00h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    eax, 8
        or     edx, eax
        shr    ecx, 18h
        or     edx, ecx
        mov    dword ptr [ebx + 2fh], edx
        mov    word ptr [ebx + 33h], 14f0h
loc_1_AF80:
        mov    word ptr [ebx + 35h], 0
        mov    al, byte ptr [ebp]
        mov    byte ptr [ebx], al
        mov    al, byte ptr [ebp + 1]
        lea    esi, [ebp + 13h]
        mov    byte ptr [ebx + 1], al
        mov    al, byte ptr [ebp + 2]
        mov    byte ptr [ebx + 2], al
        mov    eax, dword ptr [ebp + 23h]
        lea    edi, [ebx + 3]
        mov    dword ptr [ebx + 23h], eax
        movsd
        movsd
        movsd
        lea    ecx, [ebx + 13h]
        movsd
        lea    esi, [ebp + 3]
        mov    edi, ecx
        movsd
        movsd
        movsd
        movsd
        movzx  edx, byte ptr [ebx + 5]
        movzx  eax, byte ptr [ebx + 16h]
        add    edx, eax
        push   dword ptr [esp + 10h]
        movzx  eax, byte ptr [ebx + 14h]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 15h]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 4]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 6]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 17h]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 7]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 18h]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 8]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 19h]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 9]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 1ah]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 0ah]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 1bh]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 0bh]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 1ch]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 0ch]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 1dh]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 0dh]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 1eh]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 0eh]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 1fh]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 0fh]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 20h]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 21h]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 10h]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 11h]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 22h]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 12h]
        add    edx, eax
        movzx  eax, byte ptr [ecx]
        add    edx, eax
        movzx  eax, byte ptr [ebx + 3]
        add    edx, eax
        add    edx, 8c00h
        push   edx
        call   sub_1_4380
        add    esp, 8
        not    ax
        push   2
        push   0
        push   3ch
        mov    word ptr [ebx + 37h], ax
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 2ch]
        push   12h
        call   sub_1_D7E0
        add    esp, 18h
        mov    esi, eax
        push   dword ptr [esp + 1ch]
        call   sub_1_35D1
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_B0C3
        mov    al, byte ptr [ebp + 1]
        push   eax
        push   edi
        call   sub_1_3B6C
        add    esp, 8
loc_1_B0C3:
        cmp    esi, 3ch
        je     SHORT loc_1_B0E7
        push   2
        push   2
        push   0
        push   0
        push   0
        push   3ch
        push   dword ptr [esp + 28h]
        push   edi
        push   dword ptr [esp + 3ch]
        call   sub_1_3A0F
        add    esp, 24h
        jmp    SHORT loc_1_B0EE
loc_1_B0E7:
        mov    dword ptr [edi + 41h], 0
loc_1_B0EE:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_B0F6:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 9eh]
        mov    dword ptr [eax + 24h], ecx
        mov    dword ptr [eax + 28h], 0ffffffffh
        mov    byte ptr [eax + 0bch], 4
        mov    cl, byte ptr [eax + 0bah]
        dec    cl
        mov    byte ptr [eax + 0bah], cl
        jne    SHORT loc_1_B125
        and    dword ptr [eax + 64h], 0fffffffeh
loc_1_B125:
        and    dword ptr [eax + 60h], 0fffffff7h
        ret
sub_1_B12A:
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx + 10h], 3e8h
        mov    dword ptr [ecx + 0ch], 3e8h
        mov    dword ptr [ecx + 14h], 3e8h
        db  0A1h, 008h, 02Bh, 001h, 000h
        mov    dword ptr [ecx + 4ch], eax
        mov    dword ptr [ecx + 0c0h], 0
        movzx  eax, byte ptr [ecx + 80h]
        movzx  edx, byte ptr [ecx + 6fh]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 81h]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 79h]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 8ah]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 7ah]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 8bh]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 7bh]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 7ch]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 7dh]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 6ch]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 7eh]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 6dh]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 6eh]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 7fh]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 70h]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 71h]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 82h]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 72h]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 83h]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 73h]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 84h]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 74h]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 85h]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 75h]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 86h]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 76h]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 87h]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 77h]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 88h]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 78h]
        add    edx, eax
        movzx  eax, byte ptr [ecx + 89h]
        add    edx, eax
        add    edx, 5000h
        mov    dword ptr [ecx + 2ch], edx
        mov    byte ptr [ecx + 0bdh], 0
        mov    dword ptr [ecx + 0c4h], 0
        mov    dword ptr [ecx + 0aeh], 0
        db  0A1h, 044h, 039h, 001h, 000h
        mov    edx, eax
        imul   edx, edx, 0fh
        mov    dword ptr [ecx + 0b2h], edx
        imul   eax, eax, 5
        mov    dword ptr [ecx + 0a2h], eax
        mov    dword ptr [ecx + 0c8h], 0
        mov    word ptr [ecx + 0cch], 0
        ret
sub_1_B288:
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 10h]
        mov    ebx, dword ptr [esp + 14h]
        mov    al, byte ptr [ecx + 98h]
        mov    byte ptr [ebx], al
        mov    byte ptr [ebx + 1], al
        mov    al, byte ptr [ecx + 99h]
        mov    byte ptr [ebx + 2], al
        mov    edx, dword ptr [ecx + 94h]
        mov    esi, edx
        shl    esi, 10h
        mov    eax, edx
        and    eax, 0ff00h
        or     esi, eax
        shl    esi, 8
        mov    eax, edx
        shr    eax, 10h
        and    edx, 0ff0000h
        or     eax, edx
        shr    eax, 8
        or     esi, eax
        mov    dword ptr [ebx + 23h], esi
        mov    edx, dword ptr [ecx + 9ah]
        mov    esi, edx
        shl    esi, 10h
        mov    eax, edx
        and    eax, 0ff00h
        or     esi, eax
        shl    esi, 8
        mov    eax, edx
        shr    eax, 10h
        and    edx, 0ff0000h
        or     eax, edx
        shr    eax, 8
        or     esi, eax
        mov    dword ptr [ebx + 27h], esi
        lea    esi, [ecx + 6ch]
        lea    edi, [ebx + 3]
        movsd
        movsd
        movsd
        movsd
        lea    esi, [ecx + 7ch]
        lea    edi, [ebx + 13h]
        movsd
        movsd
        movsd
        movsd
        mov    edx, dword ptr [ecx + 10h]
        mov    esi, edx
        shl    esi, 10h
        mov    eax, edx
        and    eax, 0ff00h
        or     esi, eax
        shl    esi, 8
        mov    eax, edx
        and    eax, 0ff0000h
        shr    eax, 8
        or     esi, eax
        shr    edx, 18h
        or     esi, edx
        mov    dword ptr [ebx + 2bh], esi
        mov    edx, dword ptr [ecx + 48h]
        mov    esi, edx
        shl    esi, 10h
        mov    eax, edx
        and    eax, 0ff00h
        or     esi, eax
        shl    esi, 8
        mov    eax, edx
        and    eax, 0ff0000h
        shr    eax, 8
        or     esi, eax
        shr    edx, 18h
        or     esi, edx
        mov    dword ptr [ebx + 2fh], esi
        mov    word ptr [ebx + 33h], 10f0h
        push   ecx
        call   sub_1_A8F8
        add    esp, 4
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx + 35h], cx
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_B37E:
        sub    esp, 44h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 58h]
        mov    eax, dword ptr [esi + 60h]
        test   eax, 40000h
        jne    NEAR PTR loc_1_B8D5
        test   byte ptr [esi + 64h], 2
        jne    NEAR PTR loc_1_B8D5
        or     eax, 40000h
        mov    dword ptr [esi + 60h], eax
        test   eax, 10000000h
        jne    NEAR PTR loc_1_B7EE
loc_1_B3B4:
        mov    ecx, dword ptr [esi + 10h]
        sub    ecx, dword ptr [esi + 0ch]
        mov    eax, dword ptr [esi + 1ch]
        sub    eax, ecx
        mov    dword ptr [esp + 4ch], eax
        mov    edx, dword ptr [esi + 24h]
        mov    eax, dword ptr [esi + 18h]
        cmp    edx, eax
        jb     SHORT loc_1_B3CF
        mov    edx, eax
loc_1_B3CF:
        sub    edx, ecx
        mov    edi, dword ptr [esi + 60h]
        mov    eax, edi
        and    eax, 400000h
        shr    eax, 16h
        cmp    eax, edx
        jg     SHORT loc_1_B3E4
        mov    eax, edx
loc_1_B3E4:
        mov    dword ptr [esp + 48h], eax
        mov    eax, dword ptr [esi + 9eh]
        mov    ecx, dword ptr [esp + 4ch]
        mov    edx, dword ptr [esp + 48h]
        cmp    ecx, edx
        jb     SHORT loc_1_B3FC
        mov    ecx, edx
loc_1_B3FC:
        cmp    ecx, eax
        jb     SHORT loc_1_B402
        mov    ecx, eax
loc_1_B402:
        mov    dword ptr [esp + 30h], ecx
        cmp    ecx, eax
        je     SHORT loc_1_B438
        or     ecx, ecx
        je     SHORT loc_1_B416
        mov    eax, ecx
        cmp    dword ptr [esp + 4ch], eax
        je     SHORT loc_1_B438
loc_1_B416:
        cmp    dword ptr [esp + 48h], 0
        je     NEAR PTR loc_1_B7FA
        test   edi, 410000h
        jne    SHORT loc_1_B438
        mov    eax, dword ptr [esi + 20h]
        shr    eax, 1
        cmp    eax, dword ptr [esp + 30h]
        ja     NEAR PTR loc_1_B7FA
loc_1_B438:
        call   sub_1_A7B0
        mov    dword ptr [esp + 44h], eax
        or     eax, eax
        je     NEAR PTR loc_1_B874
        mov    edi, eax
        mov    eax, dword ptr [esi + 30h]
        mov    dword ptr [esp + 34h], eax
        mov    eax, dword ptr [esp + 44h]
        mov    ebp, dword ptr [eax + 4]
        mov    dword ptr [esp + 38h], ebp
        lea    edx, [ebp + 3ch]
        mov    dword ptr [esp + 14h], edx
        push   ebp
        push   esi
        call   sub_1_B288
        mov    edx, dword ptr [esp + 1ch]
        add    esp, 8
        mov    word ptr [edx + 8], 0
        mov    word ptr [edx + 0ah], 0
        mov    dword ptr [edx + 4], 0
        mov    eax, dword ptr [esp + 30h]
        or     eax, eax
        mov    dword ptr [esp + 10h], eax
        je     NEAR PTR loc_1_B671
        mov    eax, dword ptr [esp + 34h]
        mov    dword ptr [edx], eax
        sub    ebx, ebx
loc_1_B49D:
        mov    eax, dword ptr [esp + 34h]
        mov    edx, dword ptr [esp + 14h]
        inc    dword ptr [eax + 18h]
        inc    dword ptr [edx + 4]
        mov    ecx, dword ptr [esi + 38h]
        cmp    ecx, ebx
        jne    SHORT loc_1_B4EC
        mov    ebp, dword ptr [esi + 3ch]
        cmp    dword ptr [esp + 10h], ebp
        jb     SHORT loc_1_B4EC
        cmp    word ptr [edx + 0ah], bx
        jne    SHORT loc_1_B4EC
        cmp    dword ptr [eax + 2ch], ebx
        jne    SHORT loc_1_B4EC
        mov    eax, dword ptr [esi + 34h]
        mov    dword ptr [edi], eax
loc_1_B4CB:
        mov    edx, dword ptr [esp + 14h]
        inc    word ptr [edx + 8]
        mov    edi, dword ptr [edi]
        cmp    dword ptr [edi], ebx
        jne    SHORT loc_1_B4CB
        mov    eax, dword ptr [esp + 34h]
        sub    dword ptr [esp + 10h], ebp
        mov    dword ptr [eax + 2ch], edi
        mov    dword ptr [esi + 3ch], ebx
        jmp    NEAR PTR loc_1_B5B6
loc_1_B4EC:
        lea    eax, [esi + 34h]
        mov    eax, dword ptr [esi + 34h]
        mov    dword ptr [esp + 20h], eax
        mov    dword ptr [esp + 18h], ecx
loc_1_B4FA:
        lea    eax, [esp + 1ch]
        push   eax
        lea    eax, [esp + 30h]
        push   eax
        push   dword ptr [esp + 28h]
        call   sub_1_173B
        mov    eax, dword ptr [esp + 18h]
        sub    dword ptr [esp + 1ch], eax
        mov    ebp, dword ptr [esp + 1ch]
        mov    eax, dword ptr [esp + 10h]
        cmp    ebp, eax
        jb     SHORT loc_1_B523
        mov    ebp, eax
loc_1_B523:
        mov    eax, dword ptr [esp + 2ch]
        add    eax, dword ptr [esp + 18h]
        push   ebp
        push   eax
        lea    eax, [esp + 2ch]
        db  0FFh, 035h, 08Ch, 037h, 001h, 000h
        push   eax
        lea    eax, [esp + 38h]
        push   eax
        call   sub_1_1640
        cmp    dword ptr [esp + 28h], ebx
        mov    edx, dword ptr [esp + 14h]
        jne    SHORT loc_1_B57D
        inc    word ptr [edx + 0ah]
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [edi], eax
        mov    edi, dword ptr [esp + 24h]
        cmp    dword ptr [esp + 1ch], ebp
        ja     SHORT loc_1_B570
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [esp + 18h], ebx
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 20h], eax
        jmp    SHORT loc_1_B574
loc_1_B570:
        add    dword ptr [esp + 18h], ebp
loc_1_B574:
        sub    dword ptr [esi + 3ch], ebp
        sub    dword ptr [esp + 10h], ebp
        jmp    SHORT loc_1_B599
loc_1_B57D:
        cmp    word ptr [edx + 0ah], bx
        jne    SHORT loc_1_B58D
        cmp    word ptr [edx + 8], bx
        je     NEAR PTR loc_1_B864
loc_1_B58D:
        mov    eax, dword ptr [esp + 10h]
        sub    dword ptr [esp + 30h], eax
        mov    dword ptr [esp + 10h], ebx
loc_1_B599:
        cmp    dword ptr [esp + 10h], ebx
        je     SHORT loc_1_B5A8
        cmp    dword ptr [esi + 3ch], ebx
        jne    NEAR PTR loc_1_B4FA
loc_1_B5A8:
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [esi + 34h], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esi + 38h], eax
loc_1_B5B6:
        mov    eax, dword ptr [esp + 34h]
        mov    cl, byte ptr [eax + 1ch]
        mov    dl, cl
        and    dl, 1
        cmp    dl, bl
        je     SHORT loc_1_B602
        mov    ebp, dword ptr [esi + 3ch]
        mov    eax, dword ptr [esi + 60h]
        and    eax, 2000h
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        sub    bp, ax
        mov    eax, dword ptr [esp + 30h]
        sub    bp, word ptr [esp + 10h]
        add    ax, bp
        mov    byte ptr [esp + 41h], al
        mov    byte ptr [esp + 40h], ah
        mov    ebp, dword ptr [esp + 38h]
        mov    ax, word ptr [esp + 40h]
        or     word ptr [ebp + 33h], 2000h
        mov    word ptr [ebp + 39h], ax
loc_1_B602:
        cmp    dword ptr [esi + 3ch], ebx
        jne    SHORT loc_1_B665
        mov    ebp, dword ptr [esp + 38h]
        mov    eax, dword ptr [esp + 34h]
        or     word ptr [ebp + 33h], 800h
        mov    ebp, dword ptr [eax]
        mov    eax, esi
        sub    eax, ebp
        cmp    eax, -40h
        je     SHORT loc_1_B652
        test   cl, 2
        je     SHORT loc_1_B62D
        mov    dword ptr [esp + 3ch], 2
loc_1_B62D:
        mov    dword ptr [esp + 34h], ebp
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [esi + 3ch], eax
        mov    eax, dword ptr [ebp + 24h]
        cmp    dl, bl
        mov    dword ptr [esi + 38h], eax
        mov    eax, dword ptr [ebp + 28h]
        mov    dword ptr [esi + 34h], eax
        mov    dword ptr [esi + 30h], ebp
        je     SHORT loc_1_B665
        test   byte ptr [ebp + 1ch], 1
        jne    SHORT loc_1_B665
        jmp    SHORT loc_1_B68F
loc_1_B652:
        test   cl, 2
        je     SHORT loc_1_B65F
        mov    dword ptr [esp + 3ch], 2
loc_1_B65F:
        mov    dword ptr [esi + 30h], ebx
        mov    dword ptr [esi + 34h], ebx
loc_1_B665:
        cmp    dword ptr [esp + 10h], ebx
        jne    NEAR PTR loc_1_B49D
        jmp    SHORT loc_1_B68F
loc_1_B671:
        test   byte ptr [esi + 62h], 1
        je     NEAR PTR loc_1_B892
        mov    edx, dword ptr [esp + 14h]
        mov    eax, dword ptr [esp + 44h]
        mov    dword ptr [edx], 0
        mov    dword ptr [eax], 0
loc_1_B68F:
        mov    eax, dword ptr [esp + 10h]
        sub    dword ptr [esp + 30h], eax
        mov    edi, dword ptr [esi + 10h]
        mov    edx, dword ptr [esp + 30h]
        add    edx, edi
        mov    dword ptr [esi + 10h], edx
        mov    eax, dword ptr [esi + 14h]
        cmp    edi, eax
        jne    SHORT loc_1_B6AC
        jmp    SHORT loc_1_B6B4
loc_1_B6AC:
        mov    ecx, edx
        sub    ecx, eax
        or     ecx, ecx
        jle    SHORT loc_1_B6D1
loc_1_B6B4:
        mov    dword ptr [esi + 14h], edx
        cmp    dword ptr [esi + 0c4h], 0
        jne    SHORT loc_1_B6D1
        db  0A1h, 030h, 038h, 001h, 000h
        mov    dword ptr [esi + 0c4h], eax
        mov    dword ptr [esi + 0aah], edi
loc_1_B6D1:
        mov    eax, dword ptr [esp + 30h]
        cmp    dword ptr [esp + 4ch], eax
        jne    SHORT loc_1_B717
        mov    ecx, dword ptr [esi + 60h]
        test   ecx, 10000h
        je     SHORT loc_1_B717
        cmp    eax, dword ptr [esp + 48h]
        jae    SHORT loc_1_B717
        mov    ebp, dword ptr [esp + 38h]
        inc    edx
        or     word ptr [ebp + 33h], 100h
        mov    dword ptr [esi + 10h], edx
        mov    dword ptr [esi + 14h], edx
        or     ecx, 30000000h
        mov    dword ptr [esi + 60h], ecx
        and    ecx, 0fffeffffh
        mov    dword ptr [esi + 60h], ecx
        mov    dword ptr [esp + 3ch], 6
loc_1_B717:
        add    dword ptr [esp + 30h], 3ch
        push   dword ptr [esp + 44h]
        sub    eax, eax
        mov    al, byte ptr [esp + 35h]
        mov    ah, byte ptr [esp + 34h]
        add    eax, dword ptr [esi + 2ch]
        push   eax
        call   sub_1_4380
        not    ax
        mov    ebp, dword ptr [esp + 40h]
        add    esp, 8
        mov    word ptr [ebp + 37h], ax
        cmp    dword ptr [esi + 0c8h], 0
        db  0A1h, 030h, 038h, 001h, 000h
        jne    SHORT loc_1_B75B
        mov    ecx, dword ptr [esi + 0a2h]
        mov    dword ptr [esi + 0c8h], ecx
loc_1_B75B:
        mov    word ptr [esi + 0cch], 0
        and    dword ptr [esi + 60h], 0ffbff5ffh
        mov    dword ptr [esi + 0ceh], eax
        mov    eax, dword ptr [esp + 44h]
        mov    word ptr [esi + 110h], 0
        mov    dword ptr [eax + 0ch], 3ch
        cmp    dword ptr [esp + 3ch], 2
        je     SHORT loc_1_B79B
        cmp    dword ptr [esp + 3ch], 6
        je     SHORT loc_1_B79B
        mov    dword ptr [esp + 3ch], 0
loc_1_B79B:
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 38h]
        push   dword ptr [esp + 50h]
        push   dword ptr [esi + 8ch]
        push   12h
        call   sub_1_D7E0
        add    esp, 18h
        cmp    eax, dword ptr [esp + 30h]
        push   dword ptr [esi + 8ch]
        jne    NEAR PTR loc_1_B8A3
        call   sub_1_35D1
        add    esp, 4
        mov    dword ptr [eax + 41h], 0
        mov    dword ptr [esi + 0c0h], 0
        test   byte ptr [esi + 63h], 10h
        je     NEAR PTR loc_1_B3B4
loc_1_B7EE:
        and    dword ptr [esi + 60h], 0ff3bfbffh
        jmp    NEAR PTR loc_1_B8DD
loc_1_B7FA:
        cmp    dword ptr [esi + 18h], 0
        je     SHORT loc_1_B81C
        cmp    dword ptr [esp + 30h], 0
        je     SHORT loc_1_B7EE
        cmp    word ptr [esi + 110h], 0
        jne    SHORT loc_1_B7EE
        mov    word ptr [esi + 110h], 0c8h
        jmp    SHORT loc_1_B7EE
loc_1_B81C:
        mov    eax, dword ptr [esi + 60h]
        test   eax, 80000h
        je     SHORT loc_1_B83D
        cmp    dword ptr [esi + 0c8h], 0
        jne    SHORT loc_1_B7EE
        mov    eax, dword ptr [esi + 0a2h]
        mov    dword ptr [esi + 0c8h], eax
        jmp    SHORT loc_1_B7EE
loc_1_B83D:
        or     eax, 80000h
        mov    dword ptr [esi + 60h], eax
        mov    byte ptr [esi + 0bdh], 0
        mov    eax, dword ptr [esi + 0a2h]
        mov    dword ptr [esi + 0c8h], eax
        inc    byte ptr [esi + 0bah]
        or     dword ptr [esi + 64h], 1
        jmp    SHORT loc_1_B7EE
loc_1_B864:
        push   dword ptr [esp + 44h]
        push   dword ptr [esp + 18h]
        call   sub_1_A828
        add    esp, 8
loc_1_B874:
        cmp    dword ptr [esi + 0c8h], 0
        jne    SHORT loc_1_B889
        mov    eax, dword ptr [esi + 0a2h]
        mov    dword ptr [esi + 0c8h], eax
loc_1_B889:
        and    dword ptr [esi + 60h], 0ffbbfbffh
        jmp    SHORT loc_1_B8DD
loc_1_B892:
        push   dword ptr [esp + 44h]
        call   sub_1_A7C2
        add    esp, 4
        jmp    NEAR PTR loc_1_B7EE
loc_1_B8A3:
        call   sub_1_35D1
        add    esp, 4
        push   2
        push   dword ptr [esp + 40h]
        push   0
        push   0
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 44h]
        push   dword ptr [esp + 5ch]
        push   eax
        push   dword ptr [esi + 8ch]
        call   sub_1_3A0F
        add    esp, 24h
        jmp    NEAR PTR loc_1_B7EE
loc_1_B8D5:
        or     eax, 800000h
        mov    dword ptr [esi + 60h], eax
loc_1_B8DD:
        push   esi
        call   sub_1_4BB1
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 44h
        ret
loc_1_B8EE:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 14h]
        push   dword ptr [ebx]
        call   sub_1_4777
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_BA24
        mov    esi, dword ptr [eax]
        or     esi, esi
        je     NEAR PTR loc_1_BA1D
        mov    al, byte ptr [esi + 0bch]
        cmp    al, 4
        je     SHORT loc_1_B924
        cmp    al, 7
        jne    NEAR PTR loc_1_BA16
loc_1_B924:
        test   byte ptr [esi + 0beh], 1
        jne    NEAR PTR loc_1_BA16
        mov    edi, dword ptr [esp + 1ch]
        or     edi, edi
        je     NEAR PTR loc_1_BA12
        call   sub_1_A7E6
        or     eax, eax
        je     NEAR PTR loc_1_BA0B
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [ebx + 8]
        mov    dword ptr [eax + 0ch], ecx
        mov    edx, dword ptr [esp + 20h]
        mov    dword ptr [eax + 28h], edx
        mov    dword ptr [eax + 14h], edi
        mov    dword ptr [eax + 20h], edi
        mov    dword ptr [eax + 18h], 1
        mov    dword ptr [eax + 24h], 0
        mov    dword ptr [eax + 2ch], 0
        db  08Bh, 00Dh, 030h, 038h, 001h, 000h
        mov    dword ptr [eax + 30h], ecx
        mov    byte ptr [eax + 1ch], 0
        mov    cx, word ptr [esp + 18h]
        test   cl, 20h
        je     SHORT loc_1_B993
        mov    byte ptr [eax + 1ch], 1
loc_1_B993:
        test   ch, 2
        je     SHORT loc_1_B99C
        or     byte ptr [eax + 1ch], 2
loc_1_B99C:
        mov    ebx, dword ptr [esi + 1ch]
        add    ebx, edi
        mov    dword ptr [esi + 1ch], ebx
        mov    ecx, dword ptr [esi + 40h]
        sub    ecx, esi
        sub    ecx, 40h
        cmp    ecx, 1
        sbb    ebp, ebp
        neg    ebp
        mov    ecx, dword ptr [esi + 44h]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [esi + 44h], eax
        lea    ecx, [esi + 40h]
        mov    dword ptr [eax], ecx
        cmp    dword ptr [esi + 30h], 0
        jne    SHORT loc_1_B9D9
        mov    dword ptr [esi + 30h], eax
        mov    dword ptr [esi + 34h], edx
        mov    dword ptr [esi + 38h], 0
        mov    dword ptr [esi + 3ch], edi
loc_1_B9D9:
        or     ebp, ebp
        je     SHORT loc_1_B9DF
        jmp    SHORT loc_1_B9F5
loc_1_B9DF:
        test   byte ptr [esi + 62h], 2
        je     SHORT loc_1_B9F5
        mov    eax, dword ptr [esi + 0ch]
        sub    eax, dword ptr [esi + 14h]
        add    eax, ebx
        cmp    eax, dword ptr [esi + 9eh]
        jae    SHORT loc_1_B9F5
loc_1_B9F5:
        inc    dword ptr [esi + 0a6h]
        push   esi
        call   sub_1_B37E
        add    esp, 4
        mov    eax, 0ffh
        jmp    SHORT loc_1_BA29
loc_1_BA0B:
        mov    eax, 1
        jmp    SHORT loc_1_BA29
loc_1_BA12:
        sub    eax, eax
        jmp    SHORT loc_1_BA29
loc_1_BA16:
        mov    eax, 19h
        jmp    SHORT loc_1_BA29
loc_1_BA1D:
        mov    eax, 19h
        jmp    SHORT loc_1_BA29
loc_1_BA24:
        mov    eax, 0fh
loc_1_BA29:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h
sub_1_BA30:
        mov    ecx, dword ptr [esp + 4]
        db  0A1h, 03Ch, 036h, 001h, 000h
        mov    dword ptr [ecx], eax
        db  089h, 00Dh, 03Ch, 036h, 001h, 000h
        ret
sub_1_BA42:
        mov    edx, dword ptr [esp + 4]
        mov    ax, word ptr [edx + 40h]
        test   al, 20h
        jne    SHORT loc_1_BA7C
        and    ax, 0fffbh
        mov    word ptr [edx + 40h], ax
        or     ax, 20h
        mov    word ptr [edx + 40h], ax
        db  0A1h, 084h, 037h, 001h, 000h
        lea    ecx, [edx + 18h]
        mov    dword ptr [eax], ecx
        db  0A1h, 084h, 037h, 001h, 000h
        mov    dword ptr [edx + 1ch], eax
        db  089h, 00Dh, 084h, 037h, 001h, 000h
        db  0C7h, 001h, 080h, 037h, 001h, 000h
loc_1_BA7C:
        ret
sub_1_BA7D:
        db  0A1h, 0FCh, 037h, 001h, 000h
        or     eax, eax
        je     SHORT loc_1_BA90
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 0FCh, 037h, 001h, 000h
        jmp    SHORT loc_1_BA9A
loc_1_BA90:
        push   36h
        call   sub_1_F390
        add    esp, 4
loc_1_BA9A:
        ret
sub_1_BA9B:
        mov    eax, dword ptr [esp + 4]
        db  08Bh, 00Dh, 0FCh, 037h, 001h, 000h
        mov    dword ptr [eax], ecx
        db  0A3h, 0FCh, 037h, 001h, 000h
        ret
sub_1_BAAD:
        db  0A1h, 01Ch, 037h, 001h, 000h
        or     eax, eax
        je     SHORT loc_1_BAC0
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 01Ch, 037h, 001h, 000h
        jmp    SHORT loc_1_BACA
loc_1_BAC0:
        push   32h
        call   sub_1_F390
        add    esp, 4
loc_1_BACA:
        ret
sub_1_BACB:
        mov    ecx, dword ptr [esp + 4]
        db  0A1h, 01Ch, 037h, 001h, 000h
        mov    dword ptr [ecx], eax
        db  089h, 00Dh, 01Ch, 037h, 001h, 000h
        ret
loc_1_BADD:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        db  0C7h, 045h, 0F8h, 033h, 0BBh, 000h, 000h
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 10h], 0bh
        call   sub_1_129A4
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 18h], 0
        mov    eax, dword ptr [ebp - 18h]
        mov    ebx, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    edx, dword ptr [ebp - 0ch]
        mov    esi, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp - 4]
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        mov    dword ptr [ebp - 1ch], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_BB33:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  081h, 03Dh, 09Ch, 035h, 001h, 000h, 09Ch, 035h, 001h, 000h
        je     SHORT loc_1_BB86
        db  0BAh, 09Ch, 035h, 001h, 000h
loc_1_BB48:
        db  08Bh, 00Dh, 09Ch, 035h, 001h, 000h
        mov    eax, ecx
        sub    ecx, 18h
        mov    eax, dword ptr [eax]
        db  0A3h, 09Ch, 035h, 001h, 000h
        mov    eax, dword ptr [ecx + 18h]
        lea    esi, [ecx + 18h]
        mov    dword ptr [eax + 4], edx
        db  0A1h, 09Ch, 036h, 001h, 000h
        mov    dword ptr [eax], esi
        db  0A1h, 09Ch, 036h, 001h, 000h
        mov    dword ptr [ecx + 1ch], eax
        db  089h, 035h, 09Ch, 036h, 001h, 000h
        db  0C7h, 006h, 098h, 036h, 001h, 000h
        db  039h, 015h, 09Ch, 035h, 001h, 000h
        jne    SHORT loc_1_BB48
loc_1_BB86:
        db  081h, 03Dh, 098h, 036h, 001h, 000h, 098h, 036h, 001h, 000h
        je     NEAR PTR loc_1_BC2F
        db  0BFh, 098h, 036h, 001h, 000h
loc_1_BB9B:
        db  08Bh, 035h, 098h, 036h, 001h, 000h
        mov    eax, esi
        sub    esi, 18h
        mov    eax, dword ptr [eax]
        db  0A3h, 098h, 036h, 001h, 000h
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [eax + 4], edi
        mov    ax, word ptr [esi + 40h]
        and    ax, 0ffdfh
        mov    word ptr [esi + 40h], ax
        test   al, 40h
        jne    SHORT loc_1_BC1B
        mov    ecx, dword ptr [esi + 10h]
        lea    edx, [esi + 10h]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, 10h
        je     SHORT loc_1_BC23
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    dword ptr [eax + 4], edx
        cmp    dword ptr [ecx + 32h], 3
        jne    SHORT loc_1_BC02
        inc    word ptr [esi + 46h]
        mov    ebx, dword ptr [ecx + 28h]
        mov    ebp, dword ptr [ecx + 2ch]
        push   ecx
        call   sub_1_BA9B
        add    esp, 4
        or     ebx, ebx
        je     SHORT loc_1_BC10
        push   0
        push   1ch
        push   ebp
        call   ebx
        add    esp, 0ch
        jmp    SHORT loc_1_BC10
loc_1_BC02:
        inc    word ptr [esi + 46h]
        push   ecx
        push   esi
        call   sub_1_BCEE
        add    esp, 8
loc_1_BC10:
        push   esi
        call   sub_1_434A
        add    esp, 4
        jmp    SHORT loc_1_BC23
loc_1_BC1B:
        or     ax, 4
        mov    word ptr [esi + 40h], ax
loc_1_BC23:
        db  039h, 03Dh, 098h, 036h, 001h, 000h
        jne    NEAR PTR loc_1_BB9B
loc_1_BC2F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_BC34:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        db  081h, 03Dh, 080h, 037h, 001h, 000h, 080h, 037h, 001h, 000h
        mov    edi, dword ptr [esi + 28h]
        mov    ebx, dword ptr [esi + 2ch]
        mov    bp, word ptr [esi + 30h]
        jne    SHORT loc_1_BC60
        push   dword ptr [esp + 18h]
        call   sub_1_BA30
        add    esp, 4
        jmp    SHORT loc_1_BCD0
loc_1_BC60:
        db  08Bh, 00Dh, 080h, 037h, 001h, 000h
        mov    eax, ecx
        sub    ecx, 18h
        mov    eax, dword ptr [eax]
        db  0A3h, 080h, 037h, 001h, 000h
        mov    eax, dword ptr [ecx + 18h]
        db  0C7h, 040h, 004h, 080h, 037h, 001h, 000h
        mov    edx, dword ptr [ecx + 10h]
        mov    eax, edx
        sub    eax, ecx
        cmp    eax, 10h
        je     SHORT loc_1_BCCA
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [edx + 24h], eax
        db  08Bh, 015h, 0A0h, 035h, 001h, 000h
        lea    eax, [ecx + 18h]
        mov    dword ptr [edx], eax
        db  08Bh, 015h, 0A0h, 035h, 001h, 000h
        mov    dword ptr [ecx + 1ch], edx
        db  0A3h, 0A0h, 035h, 001h, 000h
        db  0C7h, 000h, 09Ch, 035h, 001h, 000h
        push   0
        db  0A1h, 044h, 039h, 001h, 000h
        imul   eax, eax, 6
        push   eax
        db  068h, 0DDh, 0BAh, 000h, 000h
        call   sub_1_129CC
        db  0A3h, 00Ch, 038h, 001h, 000h
        jmp    SHORT loc_1_BCD0
loc_1_BCCA:
        and    word ptr [ecx + 40h], 0ffdfh
loc_1_BCD0:
        push   esi
        call   sub_1_BA9B
        add    esp, 4
        or     edi, edi
        je     SHORT loc_1_BCE9
        movzx  eax, bp
        push   eax
        push   0
        push   ebx
        call   edi
        add    esp, 0ch
loc_1_BCE9:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_BCEE:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 24h]
        push   dword ptr [ebx + 18h]
        call   sub_1_35D1
        mov    dword ptr [esp + 1ch], eax
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_BD24
        mov    ecx, eax
        cmp    byte ptr [ecx + 1ah], 0
        jne    SHORT loc_1_BD4B
        mov    ecx, eax
        cmp    dword ptr [ecx + 51h], 0
        jne    SHORT loc_1_BD4B
        mov    ecx, eax
        cmp    dword ptr [ecx + 4dh], 0
        jne    SHORT loc_1_BD4B
loc_1_BD24:
        push   0
        push   dword ptr [ebx + 1ch]
        push   dword ptr [ebx + 18h]
        push   12h
        call   sub_1_E8C0
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_BD4B
        cmp    eax, 2b0ah
        je     SHORT loc_1_BD4B
        mov    eax, 1
        jmp    NEAR PTR loc_1_BFDF
loc_1_BD4B:
        mov    eax, dword ptr [ebx + 24h]
        mov    dword ptr [esp + 14h], eax
        or     eax, eax
        jne    SHORT loc_1_BD76
        db  08Bh, 00Dh, 03Ch, 036h, 001h, 000h
        or     ecx, ecx
        je     SHORT loc_1_BD67
        mov    eax, dword ptr [ecx]
        db  0A3h, 03Ch, 036h, 001h, 000h
loc_1_BD67:
        mov    dword ptr [esp + 14h], ecx
        or     ecx, ecx
        je     NEAR PTR loc_1_BF08
        mov    dword ptr [ebx + 24h], ecx
loc_1_BD76:
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [ebx + 20h]
        mov    ebp, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 0ch], 25h
        mov    ax, word ptr [ebx + 30h]
        add    ax, 25h
        mov    word ptr [esp + 12h], ax
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebp + 21h], cx
        mov    byte ptr [ebp], 0
        mov    esi, dword ptr [esp + 20h]
        add    esi, 28h
        lea    edi, [ebp + 1]
        movsd
        movsd
        movsd
        movsd
        lea    esi, [ebx + 8]
        lea    edi, [ebp + 11h]
        movsd
        movsd
        movsd
        movsd
        push   dword ptr [esp + 14h]
        mov    word ptr [ebp + 23h], 0
        movzx  eax, byte ptr [ebp + 13h]
        movzx  edx, byte ptr [ebp + 2]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 3]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 14h]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 4]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 15h]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 5]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 16h]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 6]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 17h]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 7]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 18h]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 8]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 19h]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 9]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 1ah]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 0ah]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 1bh]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 0bh]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 1ch]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 0ch]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 1dh]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 0dh]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 1eh]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 0eh]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 1fh]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 0fh]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 20h]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 10h]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 12h]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 11h]
        add    edx, eax
        movzx  eax, byte ptr [ebp + 1]
        add    edx, eax
        movzx  eax, cx
        add    edx, eax
        add    edx, 5100h
        push   edx
        call   sub_1_4380
        add    esp, 8
        cmp    ax, 0ffffh
        je     SHORT loc_1_BE9F
        not    ax
loc_1_BE9F:
        movzx  esi, word ptr [esp + 12h]
        push   2
        push   ebx
        push   esi
        mov    word ptr [ebp + 23h], ax
        push   dword ptr [esp + 20h]
        push   dword ptr [ebx + 18h]
        push   12h
        call   sub_1_D7E0
        add    esp, 18h
        cmp    eax, esi
        je     NEAR PTR loc_1_BFAA
        db  0A3h, 02Ch, 039h, 001h, 000h
        db  089h, 035h, 0F8h, 037h, 001h, 000h
        mov    si, word ptr [ebx + 30h]
        movzx  ecx, si
        db  089h, 00Dh, 050h, 039h, 001h, 000h
        mov    ecx, dword ptr [esp + 18h]
        sub    si, ax
        mov    dword ptr [ecx + 41h], 0
        inc    dword ptr [ecx + 3dh]
        mov    word ptr [ebx + 30h], si
        mov    edx, dword ptr [ebx + 20h]
        or     eax, eax
        je     SHORT loc_1_BF34
loc_1_BEF9:
        mov    ecx, dword ptr [edx + 0ch]
        cmp    ecx, eax
        ja     SHORT loc_1_BF2D
        sub    eax, ecx
        mov    edx, dword ptr [edx]
        jne    SHORT loc_1_BEF9
        jmp    SHORT loc_1_BF34
loc_1_BF08:
        mov    ecx, dword ptr [esp + 20h]
        mov    eax, dword ptr [ecx + 10h]
        add    ecx, 10h
        mov    dword ptr [ebx], eax
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [ebx + 4], ecx
        mov    dword ptr [ecx], ebx
        push   dword ptr [esp + 20h]
        call   sub_1_BA42
        add    esp, 4
        jmp    NEAR PTR loc_1_BFDD
loc_1_BF2D:
        add    ax, si
        mov    word ptr [ebx + 30h], ax
loc_1_BF34:
        mov    dword ptr [ebx + 20h], edx
        inc    dword ptr [ebx + 32h]
        push   dword ptr [esp + 14h]
        call   sub_1_BA30
        add    esp, 4
        mov    dword ptr [ebx + 24h], 0
        mov    edx, dword ptr [esp + 20h]
        mov    eax, dword ptr [edx + 14h]
        mov    dword ptr [eax], ebx
        mov    dword ptr [ebx + 4], eax
        mov    eax, edx
        mov    dword ptr [edx + 14h], ebx
        add    eax, 10h
        mov    dword ptr [ebx], eax
        db  081h, 03Dh, 09Ch, 035h, 001h, 000h, 09Ch, 035h, 001h, 000h
        jne    SHORT loc_1_BF82
        mov    ecx, edx
        db  0A1h, 0A0h, 035h, 001h, 000h
        add    ecx, 18h
        mov    dword ptr [eax], ecx
        db  0A1h, 0A0h, 035h, 001h, 000h
        jmp    SHORT loc_1_BF99
loc_1_BF82:
        mov    ecx, dword ptr [esp + 20h]
        db  0A1h, 0A0h, 035h, 001h, 000h
        add    ecx, 18h
        mov    dword ptr [eax], ecx
        db  0A1h, 0A0h, 035h, 001h, 000h
        mov    edx, dword ptr [esp + 20h]
loc_1_BF99:
        mov    dword ptr [edx + 1ch], eax
        db  089h, 00Dh, 0A0h, 035h, 001h, 000h
        db  0C7h, 001h, 09Ch, 035h, 001h, 000h
        jmp    SHORT loc_1_BFDD
loc_1_BFAA:
        mov    ecx, dword ptr [esp + 18h]
        mov    edx, dword ptr [esp + 20h]
        mov    dword ptr [ecx + 41h], 0
        mov    eax, dword ptr [edx + 10h]
        add    edx, 10h
        mov    ecx, eax
        sub    ecx, dword ptr [esp + 20h]
        cmp    ecx, 10h
        je     SHORT loc_1_BFD3
        mov    eax, dword ptr [eax]
        mov    dword ptr [edx], eax
        mov    dword ptr [eax + 4], edx
        jmp    SHORT loc_1_BFDD
loc_1_BFD3:
        mov    edx, dword ptr [esp + 20h]
        and    word ptr [edx + 40h], 0fffbh
loc_1_BFDD:
        sub    eax, eax
loc_1_BFDF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_1_BFE7:
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_BA7D
        mov    ebp, dword ptr [esp + 14h]
        mov    edi, eax
        push   dword ptr [ebp]
        call   sub_1_3150
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_1_C109
        test   byte ptr [esi + 40h], 80h
        je     NEAR PTR loc_1_C109
        or     edi, edi
        je     NEAR PTR loc_1_C102
        lea    ebx, [edi + 18h]
        lea    eax, [edi + 1ch]
        push   eax
        push   ebx
        lea    eax, [edi + 8]
        push   eax
        mov    eax, dword ptr [esp + 24h]
        push   dword ptr [eax + 14h]
        call   sub_1_3BB5
        add    esp, 10h
        or     al, al
        je     NEAR PTR loc_1_C0F2
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [edi + 28h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 2ch], eax
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [edi + 20h], eax
        mov    eax, dword ptr [esp + 1ch]
        mov    word ptr [edi + 30h], ax
        mov    dword ptr [edi + 32h], 0
        cmp    dword ptr [esi + 38h], -1
        jne    SHORT loc_1_C06D
        mov    eax, dword ptr [esi + 3ch]
        mov    dword ptr [ebx], eax
loc_1_C06D:
        mov    ax, word ptr [esi + 40h]
        test   al, 20h
        jne    SHORT loc_1_C0D4
        test   al, 40h
        jne    SHORT loc_1_C0CC
        db  08Bh, 00Dh, 03Ch, 036h, 001h, 000h
        or     ecx, ecx
        je     SHORT loc_1_C08A
        mov    eax, dword ptr [ecx]
        db  0A3h, 03Ch, 036h, 001h, 000h
loc_1_C08A:
        mov    dword ptr [edi + 24h], ecx
        or     ecx, ecx
        je     SHORT loc_1_C0C1
        inc    word ptr [esi + 46h]
        push   edi
        push   esi
        call   sub_1_BCEE
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_C0B1
        cmp    eax, 0ffh
        je     SHORT loc_1_C0B1
        mov    eax, 1
        jmp    SHORT loc_1_C11B
loc_1_C0B1:
        push   esi
        call   sub_1_434A
        add    esp, 4
        mov    eax, 0ffh
        jmp    SHORT loc_1_C11B
loc_1_C0C1:
        push   esi
        call   sub_1_BA42
        add    esp, 4
        jmp    SHORT loc_1_C0D4
loc_1_C0CC:
        or     ax, 4
        mov    word ptr [esi + 40h], ax
loc_1_C0D4:
        mov    dword ptr [edi + 24h], 0
        mov    eax, dword ptr [esi + 14h]
        add    esi, 10h
        mov    dword ptr [eax], edi
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [esi + 4], edi
        mov    dword ptr [edi], esi
        mov    eax, 0ffh
        jmp    SHORT loc_1_C11B
loc_1_C0F2:
        push   edi
        call   sub_1_BA9B
        add    esp, 4
        mov    eax, 3
        jmp    SHORT loc_1_C11B
loc_1_C102:
        mov    eax, 1
        jmp    SHORT loc_1_C11B
loc_1_C109:
        or     edi, edi
        je     SHORT loc_1_C116
        push   edi
        call   sub_1_BA9B
        add    esp, 4
loc_1_C116:
        mov    eax, 6
loc_1_C11B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_C120:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        call   sub_1_BAAD
        mov    ebx, dword ptr [esp + 14h]
        mov    edi, eax
        push   dword ptr [ebx]
        call   sub_1_3150
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_1_C1D2
        test   byte ptr [esi + 40h], 80h
        je     NEAR PTR loc_1_C1D2
        or     edi, edi
        je     SHORT loc_1_C1CB
        mov    ecx, dword ptr [esp + 18h]
        or     ecx, ecx
        je     SHORT loc_1_C17A
        cmp    dword ptr [ecx + 10h], 0
        je     SHORT loc_1_C17A
        lea    eax, [edi + 1ch]
        push   eax
        lea    eax, [edi + 18h]
        push   eax
        lea    eax, [edi + 8]
        push   eax
        push   dword ptr [ecx + 14h]
        call   sub_1_3BB5
        add    esp, 10h
        jmp    SHORT loc_1_C17E
loc_1_C17A:
        mov    al, byte ptr [esp + 0fh]
loc_1_C17E:
        or     al, al
        je     SHORT loc_1_C1BB
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [edi + 24h], eax
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [edi + 28h], eax
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [edi + 2ch], eax
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [edi + 20h], eax
        add    esi, 20h
        mov    eax, dword ptr [esp + 20h]
        mov    word ptr [edi + 30h], ax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [eax], edi
        mov    dword ptr [edi + 4], eax
        mov    eax, 0ffh
        mov    dword ptr [esi + 4], edi
        mov    dword ptr [edi], esi
        jmp    SHORT loc_1_C1E4
loc_1_C1BB:
        push   edi
        call   sub_1_BACB
        add    esp, 4
        mov    eax, 3
        jmp    SHORT loc_1_C1E4
loc_1_C1CB:
        mov    eax, 1
        jmp    SHORT loc_1_C1E4
loc_1_C1D2:
        or     edi, edi
        je     SHORT loc_1_C1DF
        push   edi
        call   sub_1_BACB
        add    esp, 4
loc_1_C1DF:
        mov    eax, 6
loc_1_C1E4:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_C1EB:
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 14h], 0
        mov    dword ptr [esp + 10h], 0
        mov    esi, dword ptr [esp + 44h]
        mov    ebp, dword ptr [esp + 54h]
        test   byte ptr [esi + 40h], 80h
        je     NEAR PTR loc_1_C3B4
        mov    ebx, dword ptr [esi + 20h]
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, -20h
        mov    edi, dword ptr [esp + 50h]
        je     NEAR PTR loc_1_C308
        mov    cl, byte ptr [edi]
loc_1_C22A:
        cmp    byte ptr [ebx + 8], cl
        jne    SHORT loc_1_C2A7
        mov    al, byte ptr [ebx + 9]
        cmp    byte ptr [edi + 1], al
        jne    SHORT loc_1_C2A7
        mov    al, byte ptr [ebx + 0ah]
        cmp    byte ptr [edi + 2], al
        jne    SHORT loc_1_C2A7
        mov    al, byte ptr [ebx + 0bh]
        cmp    byte ptr [edi + 3], al
        jne    SHORT loc_1_C2A7
        mov    al, byte ptr [ebx + 0ch]
        cmp    byte ptr [edi + 4], al
        jne    SHORT loc_1_C2A7
        mov    al, byte ptr [ebx + 0dh]
        cmp    byte ptr [edi + 5], al
        jne    SHORT loc_1_C2A7
        mov    al, byte ptr [ebx + 0eh]
        cmp    byte ptr [edi + 6], al
        jne    SHORT loc_1_C2A7
        mov    al, byte ptr [ebx + 0fh]
        cmp    byte ptr [edi + 7], al
        jne    SHORT loc_1_C2A7
        mov    al, byte ptr [ebx + 10h]
        cmp    byte ptr [edi + 8], al
        jne    SHORT loc_1_C2A7
        mov    al, byte ptr [ebx + 11h]
        cmp    byte ptr [edi + 9], al
        jne    SHORT loc_1_C2A7
        mov    al, byte ptr [ebx + 12h]
        cmp    byte ptr [edi + 0ah], al
        jne    SHORT loc_1_C2A7
        mov    al, byte ptr [ebx + 13h]
        cmp    byte ptr [edi + 0bh], al
        jne    SHORT loc_1_C2A7
        mov    al, byte ptr [ebx + 14h]
        cmp    byte ptr [edi + 0ch], al
        jne    SHORT loc_1_C2A7
        mov    al, byte ptr [ebx + 15h]
        cmp    byte ptr [edi + 0dh], al
        jne    SHORT loc_1_C2A7
        mov    al, byte ptr [ebx + 16h]
        cmp    byte ptr [edi + 0eh], al
        jne    SHORT loc_1_C2A7
        mov    al, byte ptr [edi + 0fh]
        cmp    byte ptr [ebx + 17h], al
        je     SHORT loc_1_C2B8
loc_1_C2A7:
        mov    ebx, dword ptr [ebx]
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, -20h
        jne    NEAR PTR loc_1_C22A
        jmp    SHORT loc_1_C308
loc_1_C2B8:
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [eax], ecx
        cmp    dword ptr [esi + 38h], -1
        jne    SHORT loc_1_C2CD
        mov    ebp, 0ffffffffh
loc_1_C2CD:
        push   25h
        movzx  eax, word ptr [ebx + 30h]
        push   eax
        push   dword ptr [ebx + 20h]
        push   dword ptr [esp + 54h]
        call   sub_1_442C
        add    esp, 10h
        mov    esi, eax
        push   ebp
        push   edi
        push   dword ptr [ebx + 24h]
        call   sub_1_463E
        add    esp, 0ch
        push   esi
        push   eax
        push   dword ptr [ebx + 2ch]
        call   dword ptr [ebx + 28h]
        add    esp, 0ch
        push   ebx
        call   sub_1_BACB
        jmp    NEAR PTR loc_1_C3B1
loc_1_C308:
        mov    ebx, dword ptr [esi + 7ah]
        or     ebx, ebx
        je     NEAR PTR loc_1_C3B4
        mov    eax, dword ptr [esi + 7eh]
        mov    dword ptr [esp + 18h], eax
        inc    word ptr [esi + 46h]
        cmp    dword ptr [esi + 38h], -1
        jne    SHORT loc_1_C329
        mov    ebp, 0ffffffffh
loc_1_C329:
        push   ebp
        lea    eax, [esp + 20h]
        push   edi
        push   eax
        call   sub_1_460F
        lea    eax, [esp + 1ch]
        add    esp, 0ch
        push   eax
        mov    eax, dword ptr [esp + 4ch]
        mov    eax, dword ptr [eax + 8]
        add    eax, 25h
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        mov    eax, dword ptr [esp + 58h]
        sub    eax, 25h
        push   eax
        mov    eax, dword ptr [esp + 58h]
        mov    eax, dword ptr [eax + 0ch]
        sub    eax, 25h
        push   eax
        push   200h
        push   0
        lea    eax, [esp + 38h]
        push   0
        push   eax
        push   22h
        push   dword ptr [esp + 40h]
        call   ebx
        add    esp, 2ch
        cmp    eax, 18h
        jne    SHORT loc_1_C3AB
        mov    eax, dword ptr [esp + 14h]
        add    eax, 25h
        push   eax
        mov    eax, dword ptr [esp + 14h]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        push   dword ptr [esp + 54h]
        call   sub_1_442C
        add    esp, 10h
        push   eax
        mov    eax, dword ptr [esp + 14h]
        push   0
        push   dword ptr [eax + 0ch]
        call   dword ptr [eax + 8]
        add    esp, 0ch
loc_1_C3AB:
        push   esi
        call   sub_1_4325
loc_1_C3B1:
        add    esp, 4
loc_1_C3B4:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
sub_1_C3BC:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esi + 8]
        mov    ax, word ptr [edi + 21h]
        mov    bx, ax
        and    ebx, 0ffh
        shr    ax, 8
        shl    ebx, 8
        movzx  eax, ax
        or     ebx, eax
        cmp    ebx, 25h
        jae    SHORT loc_1_C3E7
        jmp    SHORT loc_1_C436
loc_1_C3E7:
        mov    eax, dword ptr [esp + 18h]
        cmp    ebx, eax
        je     SHORT loc_1_C40D
        jae    SHORT loc_1_C436
        mov    eax, esi
        mov    edx, ebx
        or     esi, esi
        je     SHORT loc_1_C40D
loc_1_C3F9:
        mov    ecx, dword ptr [eax + 0ch]
        cmp    ecx, edx
        jb     SHORT loc_1_C402
        mov    ecx, edx
loc_1_C402:
        mov    dword ptr [eax + 0ch], ecx
        sub    edx, ecx
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_C3F9
loc_1_C40D:
        push   0
        mov    ebp, dword ptr [esp + 20h]
        push   51h
        push   ebp
        lea    eax, [edi + 11h]
        push   eax
        call   sub_1_337D
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_C428
        jmp    SHORT loc_1_C436
loc_1_C428:
        push   ebp
        inc    edi
        push   edi
        push   ebx
        push   esi
        push   eax
        call   sub_1_C1EB
        add    esp, 14h
loc_1_C436:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h
sub_1_C440:
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 10h]
        mov    ebx, dword ptr [ecx + 4]
        test   byte ptr [ebx + 0beh], 1
        je     SHORT loc_1_C45F
        mov    edx, dword ptr [esp + 14h]
        mov    dword ptr [edx], 1
        jmp    SHORT loc_1_C46D
loc_1_C45F:
        movzx  eax, byte ptr [ebx + 0bch]
        mov    edx, dword ptr [esp + 14h]
        inc    eax
        mov    dword ptr [edx], eax
loc_1_C46D:
        lea    esi, [ebx + 6ch]
        lea    edi, [edx + 4]
        movsd
        movsd
        movsd
        movsd
        lea    esi, [ebx + 7ch]
        lea    edi, [edx + 14h]
        movsd
        movsd
        movsd
        movsd
        mov    eax, dword ptr [ebx + 8ch]
        mov    dword ptr [edx + 24h], eax
        mov    al, byte ptr [ebx + 98h]
        mov    byte ptr [edx + 28h], al
        mov    eax, dword ptr [ebx]
        or     eax, eax
        je     SHORT loc_1_C4A3
        mov    dword ptr [ecx + 4], eax
        mov    eax, 1
        jmp    SHORT loc_1_C4D1
loc_1_C4A3:
        mov    edx, dword ptr [ecx]
        inc    edx
        cmp    edx, 20h
        jae    SHORT loc_1_C4C2
        db  08Dh, 004h, 095h, 0A4h, 035h, 001h, 000h
loc_1_C4B2:
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_C4D5
        add    eax, 4
        inc    edx
        db  03Dh, 024h, 036h, 001h, 000h
        jb     SHORT loc_1_C4B2
loc_1_C4C2:
        mov    dword ptr [ecx], 0
        mov    dword ptr [ecx + 4], 0
        sub    eax, eax
loc_1_C4D1:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_C4D5:
        db  08Bh, 004h, 095h, 0A4h, 035h, 001h, 000h
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx], edx
        mov    eax, 1
        jmp    SHORT loc_1_C4D1
loc_1_C4E8:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    esi, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        or     esi, esi
        jne    SHORT loc_1_C52C
        or     eax, eax
        jne    SHORT loc_1_C52C
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], 1
        db  08Dh, 004h, 0B5h, 0A4h, 035h, 001h, 000h
loc_1_C50B:
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        jne    SHORT loc_1_C520
        add    eax, 4
        inc    esi
        db  03Dh, 024h, 036h, 001h, 000h
        jb     SHORT loc_1_C50B
        or     ecx, ecx
        je     SHORT loc_1_C550
loc_1_C520:
        mov    dword ptr [edx], esi
        mov    dword ptr [edx + 4], ecx
        mov    eax, 1
        jmp    SHORT loc_1_C552
loc_1_C52C:
        cmp    esi, 20h
        jae    SHORT loc_1_C546
        db  08Bh, 00Ch, 0B5h, 0A4h, 035h, 001h, 000h
        or     ecx, ecx
        je     SHORT loc_1_C546
loc_1_C53C:
        cmp    eax, ecx
        je     SHORT loc_1_C554
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    SHORT loc_1_C53C
loc_1_C546:
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], 0
loc_1_C550:
        sub    eax, eax
loc_1_C552:
        pop    esi
        ret
loc_1_C554:
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], 1
        mov    eax, 1
        jmp    SHORT loc_1_C552
sub_1_C565:
        mov    eax, dword ptr [esp + 4]
        db  08Bh, 004h, 085h, 0A4h, 035h, 001h, 000h
        or     eax, eax
        je     SHORT loc_1_C582
        mov    ecx, dword ptr [esp + 8]
loc_1_C578:
        cmp    ecx, eax
        jb     SHORT loc_1_C582
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_C578
loc_1_C582:
        ret
sub_1_C583:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 0ch]
        mov    edx, eax
        sub    edx, dword ptr [ecx + 0ch]
        mov    dword ptr [ecx + 10h], eax
        cmp    dword ptr [ecx + 14h], eax
        je     SHORT loc_1_C5A1
        and    dword ptr [ecx + 60h], 0efffffffh
loc_1_C5A1:
        mov    al, byte ptr [ecx + 0bch]
        cmp    al, 3
        jbe    SHORT loc_1_C5FE
        cmp    al, 0ah
        je     SHORT loc_1_C5FE
        mov    edi, dword ptr [ecx + 40h]
        mov    eax, edi
        sub    eax, ecx
        cmp    eax, 40h
        je     SHORT loc_1_C5FE
        or     edx, edx
        je     SHORT loc_1_C5CC
loc_1_C5BF:
        mov    eax, dword ptr [edi + 20h]
        cmp    edx, eax
        jb     SHORT loc_1_C5CC
        sub    edx, eax
        mov    edi, dword ptr [edi]
        jne    SHORT loc_1_C5BF
loc_1_C5CC:
        mov    dword ptr [ecx + 30h], edi
        mov    eax, dword ptr [edi + 20h]
        sub    eax, edx
        mov    dword ptr [ecx + 3ch], eax
        mov    esi, dword ptr [edi + 28h]
        mov    edi, dword ptr [edi + 24h]
        or     edx, edx
        je     SHORT loc_1_C5F5
loc_1_C5E1:
        mov    eax, dword ptr [esi + 0ch]
        sub    eax, edi
        cmp    edx, eax
        jb     SHORT loc_1_C5F5
        sub    edx, eax
        mov    edi, 0
        mov    esi, dword ptr [esi]
        jne    SHORT loc_1_C5E1
loc_1_C5F5:
        mov    dword ptr [ecx + 34h], esi
        lea    eax, [edx + edi]
        mov    dword ptr [ecx + 38h], eax
loc_1_C5FE:
        pop    edi
        pop    esi
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        db  0C7h, 045h, 0F8h, 057h, 0C6h, 000h, 000h
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 10h], 0bh
        call   sub_1_129A4
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 18h], 0
        mov    eax, dword ptr [ebp - 18h]
        mov    ebx, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    edx, dword ptr [ebp - 0ch]
        mov    esi, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp - 4]
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        mov    dword ptr [ebp - 1ch], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_C657:
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebp, ebp
        db  0FFh, 005h, 030h, 038h, 001h, 000h
        sub    edi, edi
loc_1_C665:
        db  08Bh, 01Ch, 0BDh, 0A4h, 035h, 001h, 000h
        or     ebx, ebx
        je     NEAR PTR loc_1_C8F0
loc_1_C674:
        mov    cl, byte ptr [ebx + 0bch]
        lea    esi, [ebx + 0bch]
        or     cl, cl
        je     NEAR PTR loc_1_C8E6
        test   byte ptr [ebx + 0beh], 1
        jne    NEAR PTR loc_1_C8E6
        mov    eax, dword ptr [ebx + 0c8h]
        or     eax, eax
        je     NEAR PTR loc_1_C78F
        dec    eax
        mov    dword ptr [ebx + 0c8h], eax
        jne    NEAR PTR loc_1_C78F
        mov    al, byte ptr [ebx + 0bdh]
        inc    al
        mov    byte ptr [ebx + 0bdh], al
        cmp    cl, 2
        jne    SHORT loc_1_C6CB
        db  08Bh, 00Dh, 010h, 02Bh, 001h, 000h
        jmp    SHORT loc_1_C6D1
loc_1_C6CB:
        db  08Bh, 00Dh, 018h, 02Bh, 001h, 000h
loc_1_C6D1:
        movzx  eax, al
        cmp    eax, ecx
        ja     NEAR PTR loc_1_C86A
        mov    dword ptr [ebx + 0c4h], 0
        mov    eax, dword ptr [ebx + 0a2h]
        add    eax, eax
        cmp    eax, 3e8h
        jb     SHORT loc_1_C6FA
        mov    eax, 3e8h
loc_1_C6FA:
        mov    dword ptr [ebx + 0a2h], eax
        push   dword ptr [ebx + 0ch]
        push   ebx
        call   sub_1_C583
        add    esp, 8
        mov    eax, dword ptr [ebx + 60h]
        lea    ecx, [ebx + 60h]
        test   eax, 80000h
        jne    SHORT loc_1_C743
        mov    edx, dword ptr [ebx + 18h]
        mov    eax, dword ptr [ebx + 24h]
        cmp    edx, eax
        jb     SHORT loc_1_C725
        mov    edx, eax
loc_1_C725:
        shr    edx, 1
        mov    eax, dword ptr [ebx + 9eh]
        add    eax, eax
        cmp    edx, eax
        ja     SHORT loc_1_C735
        mov    edx, eax
loc_1_C735:
        mov    dword ptr [ebx + 28h], edx
        mov    eax, dword ptr [ebx + 9eh]
        mov    dword ptr [ebx + 24h], eax
        jmp    SHORT loc_1_C74A
loc_1_C743:
        or     eax, 400000h
        mov    dword ptr [ecx], eax
loc_1_C74A:
        movzx  eax, byte ptr [esi]
        sub    eax, 2
        cmp    eax, 8
        ja     SHORT loc_1_C78F
        db  0FFh, 024h, 085h, 008h, 0C9h, 000h, 000h
loc_1_C75C:
        push   ebx
        call   sub_1_A929
        add    esp, 4
        push   ebx
        push   edi
        call   sub_1_C565
        add    esp, 8
        mov    ebx, eax
        jmp    NEAR PTR loc_1_C8E8
loc_1_C776:
        or     dword ptr [ecx], 10000h
loc_1_C77C:
        mov    ebp, 1
        push   400h
        push   ebx
        call   sub_1_8EB3
        add    esp, 8
loc_1_C78F:
        mov    ax, word ptr [ebx + 110h]
        or     ax, ax
        je     SHORT loc_1_C7C0
        dec    ax
        mov    word ptr [ebx + 110h], ax
        jne    SHORT loc_1_C7C0
        or     dword ptr [ebx + 60h], 400000h
        mov    ebp, 1
        push   400h
        push   ebx
        call   sub_1_8EB3
        add    esp, 8
loc_1_C7C0:
        mov    ax, word ptr [ebx + 0cch]
        or     ax, ax
        je     SHORT loc_1_C7EA
        dec    ax
        mov    word ptr [ebx + 0cch], ax
        jne    SHORT loc_1_C7EA
        mov    ebp, 1
        push   200h
        push   ebx
        call   sub_1_8EB3
        add    esp, 8
loc_1_C7EA:
        cmp    byte ptr [esi], 4
        jne    NEAR PTR loc_1_C8AD
        test   byte ptr [ebx + 63h], 2
        je     NEAR PTR loc_1_C8AD
        db  08Bh, 00Dh, 030h, 038h, 001h, 000h
        sub    ecx, dword ptr [ebx + 0ceh]
        db  03Bh, 00Dh, 000h, 02Bh, 001h, 000h
        jbe    NEAR PTR loc_1_C8A6
        db  02Bh, 00Dh, 000h, 02Bh, 001h, 000h
        movzx  eax, byte ptr [ebx + 0bfh]
        mov    edx, eax
        db  00Fh, 0AFh, 015h, 004h, 02Bh, 001h, 000h
        cmp    edx, ecx
        jae    SHORT loc_1_C8AD
        db  039h, 005h, 018h, 02Bh, 001h, 000h
        jbe    SHORT loc_1_C86A
        push   ebx
        call   sub_1_AA99
        add    esp, 4
        push   ebx
        push   edi
        call   sub_1_C565
        add    esp, 8
        mov    ebx, eax
        jmp    NEAR PTR loc_1_C8E8
loc_1_C851:
        push   1
        push   ebx
        call   sub_1_4B72
        add    esp, 8
        push   ebx
        push   edi
        call   sub_1_C565
        add    esp, 8
        mov    ebx, eax
        jmp    SHORT loc_1_C8E8
loc_1_C86A:
        inc    dword ptr [ebx + 0a6h]
        push   ebx
        call   sub_1_48EE
        add    esp, 4
        push   15h
        push   ebx
        call   sub_1_78CF
        add    esp, 8
        push   20h
        push   ebx
        call   sub_1_4B72
        add    esp, 8
        push   ebx
        call   sub_1_4BB1
        add    esp, 4
        push   ebx
        push   edi
        call   sub_1_C565
        add    esp, 8
        mov    ebx, eax
        jmp    SHORT loc_1_C8E8
loc_1_C8A6:
        mov    byte ptr [ebx + 0bfh], 0
loc_1_C8AD:
        test   dword ptr [ebx + 60h], 20010008h
        je     SHORT loc_1_C8E6
        mov    eax, dword ptr [ebx + 0eah]
        mov    cx, word ptr [eax + 1ah]
        or     cx, cx
        je     SHORT loc_1_C8E6
        dec    cx
        mov    word ptr [eax + 1ah], cx
        jne    SHORT loc_1_C8E6
        push   20h
        push   ebx
        call   sub_1_4B72
        add    esp, 8
        push   ebx
        push   edi
        call   sub_1_C565
        add    esp, 8
        mov    ebx, eax
        jmp    SHORT loc_1_C8E8
loc_1_C8E6:
        mov    ebx, dword ptr [ebx]
loc_1_C8E8:
        or     ebx, ebx
        jne    NEAR PTR loc_1_C674
loc_1_C8F0:
        inc    edi
        cmp    edi, 20h
        jb     NEAR PTR loc_1_C665
        or     ebp, ebp
        je     SHORT loc_1_C903
        call   sub_1_8DFA
loc_1_C903:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  05Ch, 0C7h, 000h, 000h, 05Ch, 0C7h, 000h, 000h, 07Ch, 0C7h, 000h, 000h, 076h, 0C7h, 000h, 000h
        db  08Fh, 0C7h, 000h, 000h, 07Ch, 0C7h, 000h, 000h, 076h, 0C7h, 000h, 000h, 076h, 0C7h, 000h, 000h
        db  051h, 0C8h, 000h, 000h
sub_1_C92C:
        push   ebx
        push   esi
        db  0A1h, 054h, 037h, 001h, 000h
        or     eax, eax
        mov    ecx, dword ptr [esp + 0ch]
        je     NEAR PTR loc_1_CADE
        mov    dl, byte ptr [ecx]
        cmp    byte ptr [eax + 6ch], dl
        jne    NEAR PTR loc_1_CADE
        mov    dl, byte ptr [ecx + 1]
        cmp    byte ptr [eax + 6dh], dl
        jne    NEAR PTR loc_1_CADE
        mov    dl, byte ptr [eax + 6eh]
        cmp    byte ptr [ecx + 2], dl
        jne    NEAR PTR loc_1_CADE
        mov    dl, byte ptr [eax + 6fh]
        cmp    byte ptr [ecx + 3], dl
        jne    NEAR PTR loc_1_CADE
        mov    dl, byte ptr [eax + 70h]
        cmp    byte ptr [ecx + 4], dl
        jne    NEAR PTR loc_1_CADE
        mov    dl, byte ptr [eax + 71h]
        cmp    byte ptr [ecx + 5], dl
        jne    NEAR PTR loc_1_CADE
        mov    dl, byte ptr [eax + 72h]
        cmp    byte ptr [ecx + 6], dl
        jne    NEAR PTR loc_1_CADE
        mov    dl, byte ptr [eax + 73h]
        cmp    byte ptr [ecx + 7], dl
        jne    NEAR PTR loc_1_CADE
        mov    dl, byte ptr [eax + 74h]
        cmp    byte ptr [ecx + 8], dl
        jne    NEAR PTR loc_1_CADE
        mov    dl, byte ptr [eax + 75h]
        cmp    byte ptr [ecx + 9], dl
        jne    NEAR PTR loc_1_CADE
        mov    dl, byte ptr [eax + 76h]
        cmp    byte ptr [ecx + 0ah], dl
        jne    NEAR PTR loc_1_CADE
        mov    dl, byte ptr [eax + 77h]
        cmp    byte ptr [ecx + 0bh], dl
        jne    NEAR PTR loc_1_CADE
        mov    dl, byte ptr [eax + 78h]
        cmp    byte ptr [ecx + 0ch], dl
        jne    NEAR PTR loc_1_CADE
        mov    dl, byte ptr [ecx + 0dh]
        cmp    byte ptr [eax + 79h], dl
        jne    NEAR PTR loc_1_CADE
        mov    dl, byte ptr [ecx + 0eh]
        cmp    byte ptr [eax + 7ah], dl
        jne    NEAR PTR loc_1_CADE
        mov    dl, byte ptr [ecx + 0fh]
        cmp    byte ptr [eax + 7bh], dl
        mov    edx, dword ptr [esp + 10h]
        jne    NEAR PTR loc_1_CAE2
        mov    bl, byte ptr [edx]
        cmp    byte ptr [eax + 7ch], bl
        jne    NEAR PTR loc_1_CAE2
        mov    bl, byte ptr [edx + 1]
        cmp    byte ptr [eax + 7dh], bl
        jne    NEAR PTR loc_1_CAE2
        mov    bl, byte ptr [eax + 7eh]
        cmp    byte ptr [edx + 2], bl
        jne    NEAR PTR loc_1_CAE2
        mov    bl, byte ptr [eax + 7fh]
        cmp    byte ptr [edx + 3], bl
        jne    NEAR PTR loc_1_CAE2
        mov    bl, byte ptr [eax + 80h]
        cmp    byte ptr [edx + 4], bl
        jne    NEAR PTR loc_1_CAE2
        mov    bl, byte ptr [eax + 81h]
        cmp    byte ptr [edx + 5], bl
        jne    NEAR PTR loc_1_CAE2
        mov    bl, byte ptr [eax + 82h]
        cmp    byte ptr [edx + 6], bl
        jne    NEAR PTR loc_1_CAE2
        mov    bl, byte ptr [eax + 83h]
        cmp    byte ptr [edx + 7], bl
        jne    SHORT loc_1_CAE2
        mov    bl, byte ptr [eax + 84h]
        cmp    byte ptr [edx + 8], bl
        jne    SHORT loc_1_CAE2
        mov    bl, byte ptr [eax + 85h]
        cmp    byte ptr [edx + 9], bl
        jne    SHORT loc_1_CAE2
        mov    bl, byte ptr [eax + 86h]
        cmp    byte ptr [edx + 0ah], bl
        jne    SHORT loc_1_CAE2
        mov    bl, byte ptr [eax + 87h]
        cmp    byte ptr [edx + 0bh], bl
        jne    SHORT loc_1_CAE2
        mov    bl, byte ptr [eax + 88h]
        cmp    byte ptr [edx + 0ch], bl
        jne    SHORT loc_1_CAE2
        mov    bl, byte ptr [eax + 89h]
        cmp    byte ptr [edx + 0dh], bl
        jne    SHORT loc_1_CAE2
        mov    bl, byte ptr [edx + 0eh]
        cmp    byte ptr [eax + 8ah], bl
        jne    SHORT loc_1_CAE2
        mov    bl, byte ptr [edx + 0fh]
        cmp    byte ptr [eax + 8bh], bl
        jne    SHORT loc_1_CAE2
        mov    esi, dword ptr [esp + 14h]
        cmp    dword ptr [eax + 8ch], esi
        jne    SHORT loc_1_CAE2
        mov    bl, byte ptr [esp + 18h]
        cmp    byte ptr [eax + 98h], bl
        jne    SHORT loc_1_CAE2
        jmp    NEAR PTR loc_1_CCC3
loc_1_CADE:
        mov    edx, dword ptr [esp + 10h]
loc_1_CAE2:
        movzx  esi, byte ptr [ecx + 2]
        movzx  eax, byte ptr [edx + 2]
        add    esi, eax
        movzx  eax, byte ptr [edx + 3]
        add    esi, eax
        movzx  eax, byte ptr [ecx + 3]
        add    esi, eax
        movzx  eax, byte ptr [edx + 1]
        add    esi, eax
        movzx  eax, byte ptr [ecx + 1]
        add    esi, eax
        movzx  eax, byte ptr [ecx]
        add    esi, eax
        movzx  eax, byte ptr [edx]
        add    esi, eax
        and    esi, 1fh
        db  08Bh, 004h, 0B5h, 0A4h, 035h, 001h, 000h
        or     eax, eax
        je     NEAR PTR loc_1_CCC3
loc_1_CB20:
        mov    bl, byte ptr [ecx]
        cmp    byte ptr [eax + 6ch], bl
        jne    NEAR PTR loc_1_CCB2
        mov    bl, byte ptr [ecx + 1]
        cmp    byte ptr [eax + 6dh], bl
        jne    NEAR PTR loc_1_CCB2
        mov    bl, byte ptr [ecx + 2]
        cmp    byte ptr [eax + 6eh], bl
        jne    NEAR PTR loc_1_CCB2
        mov    bl, byte ptr [ecx + 3]
        cmp    byte ptr [eax + 6fh], bl
        jne    NEAR PTR loc_1_CCB2
        mov    bl, byte ptr [eax + 70h]
        cmp    byte ptr [ecx + 4], bl
        jne    NEAR PTR loc_1_CCB2
        mov    bl, byte ptr [eax + 71h]
        cmp    byte ptr [ecx + 5], bl
        jne    NEAR PTR loc_1_CCB2
        mov    bl, byte ptr [eax + 72h]
        cmp    byte ptr [ecx + 6], bl
        jne    NEAR PTR loc_1_CCB2
        mov    bl, byte ptr [eax + 73h]
        cmp    byte ptr [ecx + 7], bl
        jne    NEAR PTR loc_1_CCB2
        mov    bl, byte ptr [eax + 74h]
        cmp    byte ptr [ecx + 8], bl
        jne    NEAR PTR loc_1_CCB2
        mov    bl, byte ptr [eax + 75h]
        cmp    byte ptr [ecx + 9], bl
        jne    NEAR PTR loc_1_CCB2
        mov    bl, byte ptr [eax + 76h]
        cmp    byte ptr [ecx + 0ah], bl
        jne    NEAR PTR loc_1_CCB2
        mov    bl, byte ptr [eax + 77h]
        cmp    byte ptr [ecx + 0bh], bl
        jne    NEAR PTR loc_1_CCB2
        mov    bl, byte ptr [eax + 78h]
        cmp    byte ptr [ecx + 0ch], bl
        jne    NEAR PTR loc_1_CCB2
        mov    bl, byte ptr [eax + 79h]
        cmp    byte ptr [ecx + 0dh], bl
        jne    NEAR PTR loc_1_CCB2
        mov    bl, byte ptr [eax + 7ah]
        cmp    byte ptr [ecx + 0eh], bl
        jne    NEAR PTR loc_1_CCB2
        mov    bl, byte ptr [eax + 7bh]
        cmp    byte ptr [ecx + 0fh], bl
        jne    NEAR PTR loc_1_CCB2
        mov    bl, byte ptr [edx]
        cmp    byte ptr [eax + 7ch], bl
        jne    NEAR PTR loc_1_CCB2
        mov    bl, byte ptr [edx + 1]
        cmp    byte ptr [eax + 7dh], bl
        jne    NEAR PTR loc_1_CCB2
        mov    bl, byte ptr [edx + 2]
        cmp    byte ptr [eax + 7eh], bl
        jne    NEAR PTR loc_1_CCB2
        mov    bl, byte ptr [edx + 3]
        cmp    byte ptr [eax + 7fh], bl
        jne    NEAR PTR loc_1_CCB2
        mov    bl, byte ptr [eax + 80h]
        cmp    byte ptr [edx + 4], bl
        jne    NEAR PTR loc_1_CCB2
        mov    bl, byte ptr [eax + 81h]
        cmp    byte ptr [edx + 5], bl
        jne    NEAR PTR loc_1_CCB2
        mov    bl, byte ptr [eax + 82h]
        cmp    byte ptr [edx + 6], bl
        jne    SHORT loc_1_CCB2
        mov    bl, byte ptr [eax + 83h]
        cmp    byte ptr [edx + 7], bl
        jne    SHORT loc_1_CCB2
        mov    bl, byte ptr [eax + 84h]
        cmp    byte ptr [edx + 8], bl
        jne    SHORT loc_1_CCB2
        mov    bl, byte ptr [eax + 85h]
        cmp    byte ptr [edx + 9], bl
        jne    SHORT loc_1_CCB2
        mov    bl, byte ptr [eax + 86h]
        cmp    byte ptr [edx + 0ah], bl
        jne    SHORT loc_1_CCB2
        mov    bl, byte ptr [eax + 87h]
        cmp    byte ptr [edx + 0bh], bl
        jne    SHORT loc_1_CCB2
        mov    bl, byte ptr [eax + 88h]
        cmp    byte ptr [edx + 0ch], bl
        jne    SHORT loc_1_CCB2
        mov    bl, byte ptr [eax + 89h]
        cmp    byte ptr [edx + 0dh], bl
        jne    SHORT loc_1_CCB2
        mov    bl, byte ptr [eax + 8ah]
        cmp    byte ptr [edx + 0eh], bl
        jne    SHORT loc_1_CCB2
        mov    bl, byte ptr [eax + 8bh]
        cmp    byte ptr [edx + 0fh], bl
        jne    SHORT loc_1_CCB2
        mov    esi, dword ptr [esp + 14h]
        cmp    dword ptr [eax + 8ch], esi
        jne    SHORT loc_1_CCB2
        mov    bl, byte ptr [esp + 18h]
        cmp    byte ptr [eax + 98h], bl
        je     SHORT loc_1_CCBE
loc_1_CCB2:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    NEAR PTR loc_1_CB20
        jmp    SHORT loc_1_CCC3
loc_1_CCBE:
        db  0A3h, 054h, 037h, 001h, 000h
loc_1_CCC3:
        pop    esi
        pop    ebx
        ret
sub_1_CCC6:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        lea    edx, [esi + 7ch]
        lea    ecx, [esi + 6ch]
        movzx  eax, byte ptr [esi + 7eh]
        movzx  edi, byte ptr [esi + 7dh]
        add    edi, eax
        movzx  eax, byte ptr [esi + 6dh]
        add    edi, eax
        movzx  eax, byte ptr [esi + 6eh]
        add    edi, eax
        movzx  eax, byte ptr [esi + 7fh]
        add    edi, eax
        movzx  eax, byte ptr [esi + 6fh]
        add    edi, eax
        movzx  eax, byte ptr [edx]
        add    edi, eax
        movzx  eax, byte ptr [ecx]
        add    edi, eax
        mov    al, byte ptr [esi + 98h]
        push   eax
        push   dword ptr [esi + 8ch]
        push   edx
        and    edi, 1fh
        push   ecx
        call   sub_1_C92C
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_CD20
        sub    eax, eax
        jmp    SHORT loc_1_CD4B
loc_1_CD20:
        db  08Dh, 00Ch, 0BDh, 0A4h, 035h, 001h, 000h
        cmp    dword ptr [ecx], 0
        je     SHORT loc_1_CD39
loc_1_CD2C:
        mov    eax, dword ptr [ecx]
        cmp    esi, eax
        jbe    SHORT loc_1_CD39
        mov    ecx, eax
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_1_CD2C
loc_1_CD39:
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi], eax
        mov    dword ptr [ecx], esi
        or     dword ptr [esi + 60h], 80000000h
        mov    eax, 1
loc_1_CD4B:
        pop    edi
        pop    esi
        ret
sub_1_CD4E:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [ecx + 60h]
        test   edx, 80000000h
        je     SHORT loc_1_CDB1
        movzx  esi, byte ptr [ecx + 7ch]
        movzx  eax, byte ptr [ecx + 7dh]
        add    esi, eax
        movzx  eax, byte ptr [ecx + 6ch]
        add    esi, eax
        movzx  eax, byte ptr [ecx + 7eh]
        add    esi, eax
        movzx  eax, byte ptr [ecx + 6dh]
        add    esi, eax
        movzx  eax, byte ptr [ecx + 6eh]
        add    esi, eax
        movzx  eax, byte ptr [ecx + 7fh]
        add    esi, eax
        movzx  eax, byte ptr [ecx + 6fh]
        add    esi, eax
        and    esi, 1fh
        db  08Dh, 034h, 0B5h, 0A4h, 035h, 001h, 000h
loc_1_CD96:
        mov    eax, dword ptr [esi]
        cmp    eax, ecx
        je     SHORT loc_1_CDA4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_CD96
        jmp    SHORT loc_1_CDB1
loc_1_CDA4:
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi], eax
        and    edx, 7fffffffh
        mov    dword ptr [ecx + 60h], edx
loc_1_CDB1:
        db  039h, 00Dh, 054h, 037h, 001h, 000h
        jne    SHORT loc_1_CDC3
        db  0C7h, 005h, 054h, 037h, 001h, 000h, 000h, 000h, 000h, 000h
loc_1_CDC3:
        pop    esi
        ret
sub_1_CDC5:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_F21C
        mov    edx, eax
        sub    esi, esi
        sub    eax, eax
        db  0B9h, 0A4h, 035h, 001h, 000h
loc_1_CDDC:
        mov    ebx, ecx
        mov    edi, dword ptr [ecx]
        or     edi, edi
        je     SHORT loc_1_CE25
loc_1_CDE4:
        cmp    byte ptr [edi + 0bch], 0ah
        jne    SHORT loc_1_CE1D
        cmp    dword ptr [edi + 0a6h], 0
        jne    SHORT loc_1_CE1D
        test   byte ptr [edi + 0beh], 1
        jne    SHORT loc_1_CE1D
        or     eax, eax
        je     SHORT loc_1_CE0F
        mov    ebp, edx
        sub    ebp, dword ptr [edi + 0ceh]
        cmp    ebp, esi
        jbe    SHORT loc_1_CE1D
loc_1_CE0F:
        mov    dword ptr [esp + 10h], ebx
        mov    eax, edi
        mov    esi, edx
        sub    esi, dword ptr [edi + 0ceh]
loc_1_CE1D:
        mov    ebx, edi
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    SHORT loc_1_CDE4
loc_1_CE25:
        add    ecx, 4
        db  081h, 0F9h, 024h, 036h, 001h, 000h
        jb     SHORT loc_1_CDDC
        or     eax, eax
        je     SHORT loc_1_CE55
        mov    ecx, dword ptr [esp + 10h]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        and    dword ptr [eax + 60h], 7fffffffh
        db  039h, 005h, 054h, 037h, 001h, 000h
        jne    SHORT loc_1_CE55
        db  0C7h, 005h, 054h, 037h, 001h, 000h, 000h, 000h, 000h, 000h
loc_1_CE55:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_CE5D:
        push   edi
        db  08Bh, 015h, 0E0h, 02Ah, 001h, 000h
        or     edx, edx
        je     SHORT loc_1_CE71
        mov    eax, dword ptr [edx]
        db  0A3h, 0E0h, 02Ah, 001h, 000h
        jmp    SHORT loc_1_CEB7
loc_1_CE71:
        db  0A1h, 0ECh, 02Ah, 001h, 000h
        db  03Bh, 005h, 0E4h, 02Ah, 001h, 000h
        ja     SHORT loc_1_CE89
        call   sub_1_CDC5
        mov    edx, eax
        or     edx, edx
        jne    SHORT loc_1_CEB7
loc_1_CE89:
        db  0A1h, 0E8h, 02Ah, 001h, 000h
        db  039h, 005h, 0E4h, 02Ah, 001h, 000h
        jae    NEAR PTR loc_1_CF1E
        push   136h
        call   sub_1_F390
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        jne    SHORT loc_1_CEB1
        mov    eax, edx
        jmp    SHORT loc_1_CF20
loc_1_CEB1:
        db  0FFh, 005h, 0E4h, 02Ah, 001h, 000h
loc_1_CEB7:
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 4dh
        rep stosd
        stosw
        mov    dword ptr [edx + 60h], eax
        db  083h, 03Dh, 01Ch, 02Bh, 001h, 000h, 001h
        cmc
        sbb    eax, eax
        and    eax, 2000h
        or     eax, 20000h
        mov    dword ptr [edx + 60h], eax
        db  0A1h, 0A0h, 036h, 001h, 000h
        sub    eax, 3ch
        mov    dword ptr [edx + 0b6h], eax
        mov    dword ptr [edx + 9eh], eax
        lea    eax, [edx + 40h]
        mov    dword ptr [eax], eax
        mov    dword ptr [edx + 44h], eax
        mov    dword ptr [edx + 30h], 0
        db  0A1h, 030h, 038h, 001h, 000h
        mov    dword ptr [edx + 0ceh], eax
        mov    eax, edx
        mov    byte ptr [edx + 0bah], 1
        mov    dword ptr [edx + 64h], 1001h
        jmp    SHORT loc_1_CF20
loc_1_CF1E:
        sub    eax, eax
loc_1_CF20:
        pop    edi
        ret
sub_1_CF22:
        db  0A1h, 0ECh, 02Ah, 001h, 000h
        db  03Bh, 005h, 0E4h, 02Ah, 001h, 000h
        jae    SHORT loc_1_CF43
        db  0FFh, 00Dh, 0E4h, 02Ah, 001h, 000h
        push   dword ptr [esp + 4]
        call   sub_1_F396
        add    esp, 4
        jmp    SHORT loc_1_CF54
loc_1_CF43:
        mov    ecx, dword ptr [esp + 4]
        db  0A1h, 0E0h, 02Ah, 001h, 000h
        mov    dword ptr [ecx], eax
        db  089h, 00Dh, 0E0h, 02Ah, 001h, 000h
loc_1_CF54:
        ret
sub_1_CF55:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0BFh, 0A4h, 035h, 001h, 000h
        mov    esi, dword ptr [esp + 14h]
loc_1_CF62:
        mov    ebp, dword ptr [edi]
        or     ebp, ebp
        je     SHORT loc_1_CFBE
        sub    ebx, ebx
loc_1_CF6A:
        cmp    byte ptr [ebp + 0bch], bl
        je     SHORT loc_1_CFB7
        test   byte ptr [ebp + 0beh], 1
        jne    SHORT loc_1_CFB7
        mov    eax, dword ptr [ebp + 8ch]
        cmp    eax, esi
        jne    SHORT loc_1_CFB7
        cmp    dword ptr [ebp + 0c8h], ebx
        je     SHORT loc_1_CFB7
        cmp    eax, esi
        jne    SHORT loc_1_CFB7
        mov    eax, dword ptr [ebp + 40h]
        sub    eax, ebp
        sub    eax, 40h
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        cmp    eax, ebx
        jne    SHORT loc_1_CFB7
        push   400h
        push   ebp
        call   sub_1_8EB3
        add    esp, 8
        call   sub_1_8DFA
loc_1_CFB7:
        mov    ebp, dword ptr [ebp]
        cmp    ebp, ebx
        jne    SHORT loc_1_CF6A
loc_1_CFBE:
        add    edi, 4
        db  081h, 0FFh, 024h, 036h, 001h, 000h
        jb     SHORT loc_1_CF62
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h
loc_1_CFD0:
        sub    esp, 0c4h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0d8h]
        cmp    eax, 1
        je     SHORT loc_1_D00F
        cmp    eax, 2
        je     SHORT loc_1_D019
        cmp    eax, 3
        je     NEAR PTR loc_1_D075
        cmp    eax, 4
        je     NEAR PTR loc_1_D13A
        cmp    eax, 5
        je     NEAR PTR loc_1_D1AF
        mov    eax, 1fh
        jmp    NEAR PTR loc_1_D20A
loc_1_D00F:
        mov    eax, 1fh
        jmp    NEAR PTR loc_1_D20A
loc_1_D019:
        mov    dword ptr [esp + 10h], 100h
        mov    dword ptr [esp + 14h], 0ffffffffh
        mov    dword ptr [esp + 18h], 0
        mov    dword ptr [esp + 1ch], 0ffdah
        mov    dword ptr [esp + 20h], 9afh
        mov    dword ptr [esp + 24h], 1
        mov    esi, 28h
        db  0A1h, 008h, 02Bh, 001h, 000h
        mov    dword ptr [esp + 28h], eax
        mov    dword ptr [esp + 2ch], 0
        db  0A1h, 098h, 035h, 001h, 000h
        mov    dword ptr [esp + 30h], eax
        mov    dword ptr [esp + 34h], 0
        jmp    NEAR PTR loc_1_D1C9
loc_1_D075:
        mov    esi, 26h
        lea    edi, [esp + 10h]
        sub    eax, eax
        mov    ecx, 8
        rep stosd
        stosw
        cmp    dword ptr [esp + 0e4h], eax
        mov    eax, dword ptr [esp + 0d4h]
        mov    dword ptr [esp + 10h], 1
        push   dword ptr [eax]
        je     SHORT loc_1_D0FB
        call   sub_1_4777
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_D0F1
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        je     SHORT loc_1_D0D1
        push   dword ptr [ecx + 8ch]
        add    ecx, 6ch
        lea    eax, [esp + 18h]
        push   ecx
        push   eax
        call   sub_1_460F
        add    esp, 0ch
        jmp    NEAR PTR loc_1_D1C9
loc_1_D0D1:
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        je     SHORT loc_1_D0F1
        push   dword ptr [eax + 38h]
        add    eax, 28h
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_1_460F
        add    esp, 0ch
        jmp    NEAR PTR loc_1_D1C9
loc_1_D0F1:
        mov    eax, 0fh
        jmp    NEAR PTR loc_1_D20A
loc_1_D0FB:
        call   sub_1_3150
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_D111
        mov    eax, 6
        jmp    NEAR PTR loc_1_D20A
loc_1_D111:
        test   byte ptr [eax + 40h], 80h
        jne    SHORT loc_1_D121
        mov    eax, 6
        jmp    NEAR PTR loc_1_D20A
loc_1_D121:
        push   dword ptr [eax + 38h]
        add    eax, 28h
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_1_460F
        add    esp, 0ch
        jmp    NEAR PTR loc_1_D1C9
loc_1_D13A:
        mov    eax, dword ptr [esp + 0d4h]
        mov    esi, 31h
        push   dword ptr [eax]
        call   sub_1_4777
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_D1A8
        mov    edx, dword ptr [eax]
        or     edx, edx
        je     SHORT loc_1_D1A8
        lea    edi, [esp + 10h]
        sub    eax, eax
        mov    ecx, 0ch
        rep stosd
        stosb
        movzx  eax, byte ptr [edx + 0bch]
        mov    dword ptr [esp + 10h], eax
        mov    dword ptr [esp + 28h], 0ffffffffh
        mov    dword ptr [esp + 2ch], 0ffffffffh
        mov    eax, dword ptr [edx + 0aeh]
        shr    eax, 2
        add    eax, dword ptr [edx + 0b2h]
        and    eax, 0fffffffeh
        imul   eax, eax, 32h
        shr    eax, 1
        mov    dword ptr [esp + 30h], eax
        mov    dword ptr [esp + 34h], 0
        jmp    SHORT loc_1_D1C9
loc_1_D1A8:
        mov    eax, 0fh
        jmp    SHORT loc_1_D20A
loc_1_D1AF:
        lea    edi, [esp + 10h]
        sub    eax, eax
        mov    ecx, 30h
        rep stosd
        mov    dword ptr [esp + 10h], 100h
        mov    esi, 0c0h
loc_1_D1C9:
        lea    edx, [esp + 10h]
        mov    ebx, dword ptr [esp + 0e0h]
        mov    dword ptr [esp + 0ch], 0
        mov    edi, dword ptr [ebx]
        lea    eax, [esp + 0ch]
        push   eax
        mov    ecx, esi
        cmp    ecx, edi
        mov    eax, dword ptr [esp + 0e0h]
        jb     SHORT loc_1_D1F2
        mov    ecx, edi
loc_1_D1F2:
        push   ecx
        push   edx
        push   eax
        call   sub_1_4540
        add    esp, 10h
        cmp    esi, edi
        mov    eax, 9
        ja     SHORT loc_1_D20A
        mov    dword ptr [ebx], esi
        sub    eax, eax
loc_1_D20A:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0c4h
        ret
loc_1_D214:
        mov    eax, 0feh
        ret
loc_1_D21A:
        mov    eax, 0feh
        ret
loc_1_D220:
        sub    esp, 38h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 58h]
        mov    ecx, dword ptr [esp + 50h]
        mov    eax, dword ptr [edx]
        mov    dword ptr [esp + 14h], eax
        mov    dword ptr [esp + 10h], 0
        mov    esi, dword ptr [ecx]
        or     esi, esi
        jne    SHORT loc_1_D2AA
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 8], 100h
        jne    NEAR PTR loc_1_D4BC
        cmp    dword ptr [ecx + 0ch], 100h
        jne    NEAR PTR loc_1_D4BC
        cmp    dword ptr [ecx + 10h], 0
        jne    NEAR PTR loc_1_D4BC
        db  0A1h, 018h, 039h, 001h, 000h
        shl    eax, 3
        cmp    eax, dword ptr [esp + 14h]
        jbe    SHORT loc_1_D285
        mov    eax, 21h
        jmp    NEAR PTR loc_1_D4C1
loc_1_D285:
        mov    edx, dword ptr [esp + 58h]
        lea    ecx, [esp + 10h]
        mov    dword ptr [edx], eax
        push   ecx
        push   eax
        db  0FFh, 035h, 0F0h, 037h, 001h, 000h
        push   dword ptr [esp + 60h]
        call   sub_1_4540
        add    esp, 10h
        sub    eax, eax
        jmp    NEAR PTR loc_1_D4C1
loc_1_D2AA:
        cmp    esi, 400h
        je     SHORT loc_1_D2C4
        cmp    esi, 401h
        je     SHORT loc_1_D2C4
        mov    eax, 0feh
        jmp    NEAR PTR loc_1_D4C1
loc_1_D2C4:
        cmp    dword ptr [ecx + 4], 0
        je     SHORT loc_1_D2D4
        mov    eax, 0feh
        jmp    NEAR PTR loc_1_D4C1
loc_1_D2D4:
        mov    edx, dword ptr [esp + 58h]
        mov    dword ptr [edx], 0
        mov    eax, dword ptr [ecx + 8]
        cmp    eax, 100h
        jne    SHORT loc_1_D343
        cmp    dword ptr [ecx + 0ch], 100h
        jne    NEAR PTR loc_1_D4BC
        cmp    dword ptr [ecx + 10h], 1
        jne    NEAR PTR loc_1_D4BC
        cmp    dword ptr [esp + 14h], 4
        jb     SHORT loc_1_D339
        sub    esi, 400h
        cmp    esi, 1
        sbb    eax, eax
        add    eax, 408h
        mov    dword ptr [esp + 1ch], eax
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 20h]
        push   4
        push   eax
        push   dword ptr [esp + 60h]
        call   sub_1_4540
        add    esp, 10h
        sub    eax, eax
        jmp    NEAR PTR loc_1_D4C1
loc_1_D339:
        mov    eax, 21h
        jmp    NEAR PTR loc_1_D4C1
loc_1_D343:
        cmp    eax, 200h
        jne    NEAR PTR loc_1_D4B0
        mov    eax, dword ptr [ecx + 0ch]
        cmp    eax, 300h
        jne    NEAR PTR loc_1_D3FD
        cmp    dword ptr [esp + 14h], 8
        jb     NEAR PTR loc_1_D4BC
        cmp    dword ptr [ecx + 10h], 5
        jne    NEAR PTR loc_1_D4BC
        mov    eax, dword ptr [esp + 4ch]
        push   dword ptr [eax]
        call   sub_1_4777
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_D4BC
        mov    edx, dword ptr [eax]
        or     edx, edx
        je     NEAR PTR loc_1_D4BC
        mov    eax, dword ptr [edx + 60h]
        and    eax, 4008000h
        cmp    eax, 4008000h
        jne    SHORT loc_1_D3C4
        mov    ecx, dword ptr [edx + 132h]
        mov    eax, ecx
        sub    eax, dword ptr [edx + 12eh]
        inc    eax
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [edx + 0deh]
        sub    eax, dword ptr [edx + 48h]
        add    eax, ecx
        mov    dword ptr [esp + 20h], eax
        jmp    SHORT loc_1_D3D4
loc_1_D3C4:
        mov    dword ptr [esp + 1ch], 0
        mov    dword ptr [esp + 20h], 0
loc_1_D3D4:
        mov    edx, dword ptr [esp + 58h]
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 20h]
        push   8
        push   eax
        mov    dword ptr [edx], 8
        push   dword ptr [esp + 60h]
        call   sub_1_4540
        add    esp, 10h
        sub    eax, eax
        jmp    NEAR PTR loc_1_D4C1
loc_1_D3FD:
        cmp    eax, 100h
        je     SHORT loc_1_D409
        jmp    NEAR PTR loc_1_D4BC
loc_1_D409:
        mov    eax, dword ptr [ecx + 10h]
        cmp    eax, 1
        je     SHORT loc_1_D41D
        cmp    eax, 101h
        je     SHORT loc_1_D427
        jmp    NEAR PTR loc_1_D4BC
loc_1_D41D:
        mov    eax, 0feh
        jmp    NEAR PTR loc_1_D4C1
loc_1_D427:
        cmp    esi, 401h
        jne    SHORT loc_1_D43B
        mov    esi, 14h
        db  0BFh, 0F0h, 02Ah, 001h, 000h
        jmp    SHORT loc_1_D445
loc_1_D43B:
        mov    esi, 29h
        db  0BFh, 0F8h, 02Ah, 001h, 000h
loc_1_D445:
        lea    eax, [esp + 18h]
        push   eax
        sub    ebx, ebx
        push   dword ptr [esp + 60h]
        call   dword ptr [edi]
        add    esp, 8
        mov    ebp, eax
        cmp    dword ptr [esp + 18h], ebx
        jne    SHORT loc_1_D45F
        jmp    SHORT loc_1_D4BC
loc_1_D45F:
        or     ebp, ebp
        je     SHORT loc_1_D49F
loc_1_D463:
        mov    eax, dword ptr [esp + 14h]
        sub    eax, ebx
        cmp    eax, esi
        jl     SHORT loc_1_D49F
        lea    eax, [esp + 1ch]
        push   eax
        add    ebx, esi
        push   dword ptr [esp + 60h]
        call   dword ptr [edi + 4]
        add    esp, 8
        mov    ebp, eax
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 20h]
        push   esi
        push   eax
        push   dword ptr [esp + 60h]
        call   sub_1_4540
        mov    dword ptr [esp + 64h], eax
        add    esp, 10h
        or     ebp, ebp
        jne    SHORT loc_1_D463
loc_1_D49F:
        mov    edx, dword ptr [esp + 58h]
        cmp    ebp, 1
        mov    dword ptr [edx], ebx
        cmc
        sbb    eax, eax
        and    eax, 9
        jmp    SHORT loc_1_D4C1
loc_1_D4B0:
        cmp    eax, 300h
        mov    eax, 1ah
        je     SHORT loc_1_D4C1
loc_1_D4BC:
        mov    eax, 1ah
loc_1_D4C1:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
loc_1_D4C9:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        mov    eax, dword ptr [esi]
        cmp    eax, 400h
        je     SHORT loc_1_D4E5
        cmp    eax, 401h
        je     SHORT loc_1_D4E5
        jmp    NEAR PTR loc_1_D6BF
loc_1_D4E5:
        cmp    dword ptr [esi + 4], 0
        je     SHORT loc_1_D4F0
        jmp    NEAR PTR loc_1_D6BF
loc_1_D4F0:
        mov    eax, dword ptr [esi + 8]
        cmp    eax, 100h
        jne    SHORT loc_1_D504
        mov    eax, 1ah
        jmp    NEAR PTR loc_1_D6C4
loc_1_D504:
        cmp    eax, 200h
        jne    NEAR PTR loc_1_D6B3
        mov    eax, dword ptr [esi + 0ch]
        cmp    eax, 300h
        jne    NEAR PTR loc_1_D5CF
        mov    edi, 1ah
        cmp    dword ptr [esp + 1ch], 4
        jae    SHORT loc_1_D530
        mov    eax, edi
        jmp    NEAR PTR loc_1_D6C4
loc_1_D530:
        mov    eax, dword ptr [esp + 10h]
        push   dword ptr [eax]
        call   sub_1_4777
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_D5C8
        sub    edx, edx
        sub    edi, edi
        mov    esi, dword ptr [esi + 10h]
        cmp    esi, 1
        je     SHORT loc_1_D568
        cmp    esi, 2
        je     SHORT loc_1_D56F
        cmp    esi, 3
        je     SHORT loc_1_D576
        cmp    esi, 4
        je     SHORT loc_1_D57D
        mov    edi, 1ah
        jmp    SHORT loc_1_D582
loc_1_D568:
        mov    edx, 20000h
        jmp    SHORT loc_1_D582
loc_1_D56F:
        mov    edx, 2000000h
        jmp    SHORT loc_1_D582
loc_1_D576:
        mov    edx, 4000000h
        jmp    SHORT loc_1_D582
loc_1_D57D:
        mov    edx, 2000h
loc_1_D582:
        or     edi, edi
        jne    SHORT loc_1_D5C8
        mov    ecx, dword ptr [esp + 18h]
        mov    ebx, dword ptr [ecx]
        or     ebx, ebx
        je     SHORT loc_1_D595
        or     dword ptr [eax + 24h], edx
        jmp    SHORT loc_1_D59C
loc_1_D595:
        mov    ecx, edx
        not    ecx
        and    dword ptr [eax + 24h], ecx
loc_1_D59C:
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_1_D5C8
        or     ebx, ebx
        je     SHORT loc_1_D5AB
        or     dword ptr [eax + 60h], edx
        jmp    SHORT loc_1_D5B0
loc_1_D5AB:
        not    edx
        and    dword ptr [eax + 60h], edx
loc_1_D5B0:
        cmp    esi, 2
        jne    SHORT loc_1_D5C8
        db  08Bh, 00Dh, 030h, 038h, 001h, 000h
        mov    dword ptr [eax + 0ceh], ecx
        mov    byte ptr [eax + 0bfh], 0
loc_1_D5C8:
        mov    eax, edi
        jmp    NEAR PTR loc_1_D6C4
loc_1_D5CF:
        cmp    eax, 200h
        jne    SHORT loc_1_D5F2
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 20h]
        push   dword ptr [esi + 10h]
        push   dword ptr [esp + 1ch]
        call   sub_1_40B8
        add    esp, 10h
        jmp    NEAR PTR loc_1_D6C4
loc_1_D5F2:
        cmp    eax, 100h
        je     SHORT loc_1_D603
        mov    eax, 1ah
        jmp    NEAR PTR loc_1_D6C4
loc_1_D603:
        cmp    dword ptr [esi + 10h], 101h
        jne    NEAR PTR loc_1_D6AC
        cmp    dword ptr [esp + 1ch], 29h
        je     SHORT loc_1_D621
        mov    eax, 1ah
        jmp    NEAR PTR loc_1_D6C4
loc_1_D621:
        mov    ecx, dword ptr [esp + 18h]
        cmp    dword ptr [ecx], 0ch
        je     SHORT loc_1_D634
        mov    eax, 1ah
        jmp    NEAR PTR loc_1_D6C4
loc_1_D634:
        mov    al, byte ptr [ecx + 28h]
        push   eax
        push   dword ptr [ecx + 24h]
        lea    eax, [ecx + 14h]
        push   eax
        add    ecx, 4
        push   ecx
        call   sub_1_C92C
        add    esp, 10h
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_D6A5
        inc    dword ptr [esi + 0a6h]
        cmp    byte ptr [esi + 0bch], 0
        je     SHORT loc_1_D698
        test   byte ptr [esi + 0beh], 1
        jne    SHORT loc_1_D698
        or     dword ptr [esi + 60h], 40000000h
        push   40h
        push   esi
        call   sub_1_4B72
        add    esp, 8
        cmp    byte ptr [esi + 0bch], 0ah
        je     SHORT loc_1_D698
        push   esi
        call   sub_1_48EE
        add    esp, 4
        push   14h
        push   esi
        call   sub_1_78CF
        add    esp, 8
loc_1_D698:
        push   esi
        call   sub_1_4BB1
        add    esp, 4
        sub    eax, eax
        jmp    SHORT loc_1_D6C4
loc_1_D6A5:
        mov    eax, 1ah
        jmp    SHORT loc_1_D6C4
loc_1_D6AC:
        mov    eax, 1ah
        jmp    SHORT loc_1_D6C4
loc_1_D6B3:
        cmp    eax, 300h
        mov    eax, 0feh
        je     SHORT loc_1_D6C4
loc_1_D6BF:
        mov    eax, 0feh
loc_1_D6C4:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_D6D0:
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esi + 0ch]
        cmp    ecx, 100h
        jb     SHORT loc_1_D724
        cmp    ecx, 102h
        ja     SHORT loc_1_D724
        db  08Bh, 03Ch, 08Dh, 028h, 027h, 001h, 000h
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        je     SHORT loc_1_D704
        push   dword ptr [esi + 14h]
        push   eax
        call   sub_1_F270
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_D742
loc_1_D704:
        mov    dword ptr [esi + 10h], eax
        push   esi
        push   0
        push   eax
        call   edi
        add    esp, 0ch
        push   eax
        push   dword ptr [esi + 14h]
        push   dword ptr [esi + 10h]
        call   sub_1_F2B5
        add    esp, 8
        pop    eax
loc_1_D720:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_D724:
        cmp    ecx, 0
        je     SHORT loc_1_D735
        cmp    ecx, -1
        je     SHORT loc_1_D73E
        mov    eax, 1
        jmp    SHORT loc_1_D720
loc_1_D735:
        xor    eax, eax
        mov    ecx, 0
        jmp    SHORT loc_1_D720
loc_1_D73E:
        xor    eax, eax
        jmp    SHORT loc_1_D720
loc_1_D742:
        mov    eax, 6f8h
        jmp    SHORT loc_1_D720
WSVV_PM_API:
        push   ebp
        push   ebx
        push   esi
        push   edi
        movzx  eax, word ptr [ebp + 10h]
        push   eax
        movzx  eax, word ptr [ebp + 38h]
        push   eax
        push   ebx
        call   sub_1_F2F8
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_1_D7D6
        or     eax, eax
        je     SHORT loc_1_D77B
        movzx  ecx, word ptr [ebp + 18h]
        push   ecx
        push   eax
        call   sub_1_F270
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_D7D6
loc_1_D77B:
        mov    esi, eax
        mov    edx, eax
        movzx  ecx, word ptr [ebp + 1ch]
        db  08Bh, 03Ch, 08Dh, 028h, 027h, 001h, 000h
        db  00Fh, 0B6h, 089h, 034h, 02Ah, 001h, 000h
        jecxz  loc_1_D7B2
loc_1_D793:
        movzx  eax, word ptr [esi]
        push   eax
        movzx  eax, word ptr [esi + 2]
        push   eax
        push   ebx
        call   sub_1_F2F8
        add    esp, 0ch
        mov    dword ptr [esi], eax
        cmp    eax, -1
        je     SHORT loc_1_D7D6
        add    esi, 4
        dec    ecx
        jne    SHORT loc_1_D793
loc_1_D7B2:
        mov    esi, edx
        push   ebx
        push   edx
        call   edi
        add    esp, 8
        mov    word ptr [ebp + 1ch], ax
        movzx  ecx, word ptr [ebp + 18h]
        push   ecx
        push   esi
        call   sub_1_F2B5
        add    esp, 8
        and    dword ptr [ebp + 2ch], 0fffffffeh
loc_1_D7D1:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_D7D6:
        mov    word ptr [ebp + 1ch], 100h
        jmp    SHORT loc_1_D7D1
loc_1_D7DE:
        ret
        db  000h
sub_1_D7E0:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   dword ptr [esp + 2ch]
        call   sub_1_EE10
        mov    dword ptr [esp + 14h], eax
        add    esp, 4
        db  0A1h, 034h, 039h, 001h, 000h
        mov    dword ptr [esp + 20h], eax
        cmp    dword ptr [esp + 10h], 0
        je     NEAR PTR loc_1_DBFB
        push   dword ptr [esp + 2ch]
        call   sub_1_35D1
        add    esp, 4
        mov    esi, eax
        cmp    dword ptr [esp + 28h], 12h
        jne    NEAR PTR loc_1_DBEB
        mov    al, byte ptr [esi + 30h]
        cmp    al, 4
        jne    SHORT loc_1_D83A
        cmp    dword ptr [esp + 3ch], 4
        je     SHORT loc_1_D83A
        mov    dword ptr [esp + 3ch], 0
loc_1_D83A:
        cmp    al, 1
        je     SHORT loc_1_D857
        cmp    al, 4
        je     SHORT loc_1_D857
        cmp    al, 3
        je     NEAR PTR loc_1_DBFB
        cmp    al, 2
        jne    NEAR PTR loc_1_DBEB
        jmp    NEAR PTR loc_1_DBFB
loc_1_D857:
        cmp    dword ptr [esi + 39h], 0
        jne    SHORT loc_1_D86C
        mov    eax, dword ptr [esi + 31h]
        mov    dword ptr [esi + 39h], eax
        cmp    dword ptr [esi + 59h], 7bh
        je     SHORT loc_1_D86C
        mov    dword ptr [esi + 59h], eax
loc_1_D86C:
        cmp    dword ptr [esp + 3ch], 1
        je     NEAR PTR loc_1_DBEB
        cmp    dword ptr [esp + 3ch], 3
        je     NEAR PTR loc_1_DB9F
        cmp    dword ptr [esp + 3ch], 4
        je     SHORT loc_1_D8A9
        cmp    dword ptr [esp + 3ch], 5
        je     SHORT loc_1_D8A9
        cmp    dword ptr [esp + 3ch], 2
        je     SHORT loc_1_D8A9
        cmp    dword ptr [esp + 3ch], 0
        je     SHORT loc_1_D8A9
        cmp    dword ptr [esp + 3ch], 6
        jne    NEAR PTR loc_1_DBEB
loc_1_D8A9:
        or     esi, esi
        je     NEAR PTR loc_1_DB41
        mov    edi, dword ptr [esi + 45h]
        lea    ecx, [esi + 45h]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, 45h
        je     NEAR PTR loc_1_DB41
        inc    dword ptr [esi + 3dh]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [edi + 20h]
        add    eax, 0ch
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [esp + 18h], eax
        mov    ebp, dword ptr [edi + 0ch]
        push   dword ptr [esp + 14h]
        call   sub_1_F390
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_1_DBFB
        mov    dword ptr [ebx], 43434d20h
        mov    ecx, dword ptr [edi + 20h]
        and    ecx, 0ff00h
        mov    eax, dword ptr [edi + 20h]
        shl    eax, 10h
        or     ecx, eax
        shl    ecx, 8
        mov    eax, dword ptr [edi + 20h]
        and    eax, 0ff0000h
        mov    edx, dword ptr [edi + 20h]
        shr    edx, 10h
        or     eax, edx
        shr    eax, 8
        or     ecx, eax
        mov    dword ptr [ebx + 4], ecx
        push   dword ptr [edi + 20h]
        lea    eax, [ebx + 0ch]
        push   dword ptr [esp + 1ch]
        push   eax
        call   sub_1_45B0
        add    esp, 0ch
        movzx  ecx, word ptr [ebx + 2]
        movzx  eax, word ptr [ebx + 6]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0ch]
        add    ecx, eax
        movzx  eax, word ptr [ebx + 4]
        add    ecx, eax
        movzx  eax, word ptr [ebx]
        add    eax, ecx
        mov    ecx, eax
        and    ecx, 0ff00h
        mov    edx, eax
        shl    edx, 10h
        or     ecx, edx
        shl    ecx, 8
        mov    edx, eax
        and    edx, 0ff0000h
        shr    eax, 10h
        or     edx, eax
        shr    edx, 8
        or     ecx, edx
        mov    dword ptr [ebx + 8], ecx
        cmp    byte ptr [esi + 30h], 4
        jne    SHORT loc_1_D98C
        cmp    ebp, 4
        je     SHORT loc_1_D98C
        sub    ebp, ebp
loc_1_D98C:
        cmp    ebp, 1
        je     SHORT loc_1_D9C4
        cmp    ebp, 2
        jb     SHORT loc_1_D9A5
        cmp    ebp, 3
        jbe    SHORT loc_1_D9AF
        cmp    ebp, 4
        jb     SHORT loc_1_D9A5
        cmp    ebp, 5
        jbe    SHORT loc_1_D9C4
loc_1_D9A5:
        cmp    ebp, 6
        jne    SHORT loc_1_D9C8
        dec    dword ptr [esi + 3dh]
        jmp    SHORT loc_1_D9C8
loc_1_D9AF:
        cmp    dword ptr [esp + 3ch], 0
        je     SHORT loc_1_D9C4
        cmp    dword ptr [esp + 3ch], 6
        je     SHORT loc_1_D9C4
        cmp    dword ptr [esp + 3ch], 2
        jne    SHORT loc_1_D9C8
loc_1_D9C4:
        mov    dword ptr [esp + 3ch], ebp
loc_1_D9C8:
        push   0
        push   dword ptr [esp + 18h]
        mov    ecx, dword ptr [esp + 18h]
        push   ebx
        push   dword ptr [ecx + 14h]
        call   sub_1_103FD
        add    esp, 10h
        mov    ebp, eax
        push   ebx
        call   sub_1_F396
        add    esp, 4
        mov    eax, dword ptr [edi + 18h]
        cmp    ebp, dword ptr [esp + 14h]
        jne    SHORT loc_1_DA3B
        or     eax, eax
        je     SHORT loc_1_DA0A
        push   0
        push   dword ptr [edi + 1ch]
        mov    ecx, dword ptr [esp + 18h]
        push   dword ptr [edi + 14h]
        push   dword ptr [ecx + 4]
        call   eax
        add    esp, 10h
loc_1_DA0A:
        db  0A1h, 034h, 039h, 001h, 000h
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        je     SHORT loc_1_DA2D
        push   dword ptr [edi + 14h]
        push   dword ptr [edi + 10h]
        mov    ecx, dword ptr [esp + 18h]
        push   0e6h
        push   dword ptr [ecx + 4]
        call   eax
        add    esp, 10h
loc_1_DA2D:
        push   edi
        call   sub_1_313E
        add    esp, 4
        jmp    NEAR PTR loc_1_D8A9
loc_1_DA3B:
        or     eax, eax
        je     NEAR PTR loc_1_DB2C
        push   2b09h
        push   dword ptr [edi + 1ch]
        mov    ecx, dword ptr [esp + 18h]
        push   dword ptr [edi + 14h]
        push   dword ptr [ecx + 4]
        call   eax
        add    esp, 10h
        cmp    eax, 2b0eh
        je     NEAR PTR loc_1_DAFB
        cmp    eax, 2b0fh
        je     NEAR PTR loc_1_DB2C
        cmp    eax, 2b0dh
        jne    NEAR PTR loc_1_DBFB
        db  0A1h, 034h, 039h, 001h, 000h
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        je     SHORT loc_1_DA9E
        push   dword ptr [edi + 14h]
        push   dword ptr [edi + 10h]
        mov    ecx, dword ptr [esp + 18h]
        push   0e6h
        push   dword ptr [ecx + 4]
        call   eax
        add    esp, 10h
loc_1_DA9E:
        push   edi
        call   sub_1_313E
        add    esp, 4
        mov    eax, dword ptr [esi + 45h]
        lea    edi, [esi + 45h]
        sub    eax, esi
        cmp    eax, 45h
        je     NEAR PTR loc_1_DBFB
loc_1_DAB8:
        mov    ebx, dword ptr [edi]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [edi], eax
        mov    dword ptr [eax + 4], edi
        db  0A1h, 034h, 039h, 001h, 000h
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        je     SHORT loc_1_DAE4
        push   dword ptr [ebx + 14h]
        push   dword ptr [ebx + 10h]
        mov    ecx, dword ptr [esp + 18h]
        push   0e6h
        push   dword ptr [ecx + 4]
        call   eax
        add    esp, 10h
loc_1_DAE4:
        push   ebx
        call   sub_1_313E
        add    esp, 4
        mov    eax, dword ptr [edi]
        sub    eax, esi
        cmp    eax, 45h
        jne    SHORT loc_1_DAB8
        jmp    NEAR PTR loc_1_DBFB
loc_1_DAFB:
        db  0A1h, 034h, 039h, 001h, 000h
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        je     SHORT loc_1_DB1E
        push   dword ptr [edi + 14h]
        push   dword ptr [edi + 10h]
        mov    ecx, dword ptr [esp + 18h]
        push   0e6h
        push   dword ptr [ecx + 4]
        call   eax
        add    esp, 10h
loc_1_DB1E:
        push   edi
        call   sub_1_313E
        add    esp, 4
        jmp    NEAR PTR loc_1_DBFB
loc_1_DB2C:
        mov    eax, dword ptr [esi + 45h]
        add    esi, 45h
        mov    dword ptr [edi], eax
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [edi + 4], esi
        mov    dword ptr [esi], edi
        jmp    NEAR PTR loc_1_DBFB
loc_1_DB41:
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 3ch]
        call   sub_1_DC05
        add    esp, 18h
        mov    esi, eax
        cmp    dword ptr [esp + 34h], esi
        jne    NEAR PTR loc_1_DBFB
        cmp    dword ptr [esp + 3ch], 0
        je     SHORT loc_1_DBEF
        cmp    dword ptr [esp + 3ch], 6
        je     SHORT loc_1_DBEF
        mov    eax, dword ptr [esp + 20h]
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        je     SHORT loc_1_DBEF
        push   dword ptr [esp + 3ch]
        push   0
        mov    ecx, dword ptr [esp + 18h]
        push   0e5h
        push   dword ptr [ecx + 4]
        call   eax
        add    esp, 10h
        jmp    SHORT loc_1_DBEF
loc_1_DB9F:
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 3ch]
        call   sub_1_DC05
        add    esp, 18h
        mov    esi, eax
        cmp    dword ptr [esp + 34h], esi
        jne    SHORT loc_1_DBFB
        mov    eax, dword ptr [esp + 20h]
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        je     SHORT loc_1_DBEF
        push   dword ptr [esp + 3ch]
        push   0
        mov    ecx, dword ptr [esp + 18h]
        push   0e5h
        push   dword ptr [ecx + 4]
        call   eax
        add    esp, 10h
        jmp    SHORT loc_1_DBEF
loc_1_DBEB:
        mov    esi, dword ptr [esp + 1ch]
loc_1_DBEF:
        cmp    dword ptr [esp + 34h], esi
        jne    SHORT loc_1_DBFB
        mov    eax, dword ptr [esp + 34h]
        jmp    SHORT loc_1_DBFD
loc_1_DBFB:
        sub    eax, eax
loc_1_DBFD:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
sub_1_DC05:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 2ch]
        push   esi
        call   sub_1_EE10
        add    esp, 4
        mov    edi, eax
        push   esi
        call   sub_1_35D1
        add    esp, 4
        mov    esi, eax
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        push   dword ptr [edi + 14h]
        call   sub_1_10879
        mov    dword ptr [esp + 20h], eax
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_DC50
        cmp    dword ptr [esi + 39h], 0
        je     SHORT loc_1_DC50
        mov    dword ptr [esi + 39h], 0
loc_1_DC50:
        mov    ebx, dword ptr [esp + 3ch]
        cmp    ebx, 6
        jne    SHORT loc_1_DC5E
        dec    dword ptr [esi + 3dh]
        sub    ebx, ebx
loc_1_DC5E:
        cmp    dword ptr [esp + 34h], 0
        je     NEAR PTR loc_1_DD67
        mov    eax, dword ptr [esp + 34h]
        add    eax, 0ch
        mov    dword ptr [esp + 10h], eax
        or     esi, esi
        je     SHORT loc_1_DC7B
        inc    dword ptr [esi + 3dh]
loc_1_DC7B:
        push   dword ptr [esp + 10h]
        call   sub_1_F390
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_1_DC94
        sub    eax, eax
        jmp    NEAR PTR loc_1_DDB7
loc_1_DC94:
        mov    dword ptr [ebp], 43434d20h
        mov    ecx, dword ptr [esp + 34h]
        and    ecx, 0ff00h
        mov    eax, dword ptr [esp + 34h]
        shl    eax, 10h
        or     ecx, eax
        shl    ecx, 8
        mov    eax, dword ptr [esp + 34h]
        and    eax, 0ff0000h
        mov    edx, dword ptr [esp + 34h]
        shr    edx, 10h
        or     eax, edx
        shr    eax, 8
        or     ecx, eax
        push   dword ptr [esp + 34h]
        mov    dword ptr [ebp + 4], ecx
        push   dword ptr [esp + 34h]
        lea    eax, [ebp + 0ch]
        push   eax
        call   sub_1_45B0
        add    esp, 0ch
        movzx  ecx, word ptr [ebp + 2]
        movzx  eax, word ptr [ebp + 6]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0ch]
        add    ecx, eax
        movzx  eax, word ptr [ebp + 4]
        add    ecx, eax
        movzx  eax, word ptr [ebp]
        add    eax, ecx
        mov    ecx, eax
        and    ecx, 0ff00h
        mov    edx, eax
        shl    edx, 10h
        or     ecx, edx
        shl    ecx, 8
        mov    edx, eax
        and    edx, 0ff0000h
        shr    eax, 10h
        or     edx, eax
        shr    edx, 8
        or     ecx, edx
        push   ebx
        mov    dword ptr [ebp + 8], ecx
        push   dword ptr [esp + 14h]
        push   ebp
        push   dword ptr [edi + 14h]
        call   sub_1_103FD
        add    esp, 10h
        mov    ebx, eax
        push   ebp
        call   sub_1_F396
        add    esp, 4
        cmp    ebx, dword ptr [esp + 10h]
        jne    SHORT loc_1_DD8C
        db  0A1h, 034h, 039h, 001h, 000h
        mov    ecx, dword ptr [eax + 10h]
        or     ecx, ecx
        je     SHORT loc_1_DD8C
        push   dword ptr [esp + 30h]
        push   dword ptr [esp + 3ch]
        push   0e6h
        push   dword ptr [edi + 4]
        call   ecx
        add    esp, 10h
        jmp    SHORT loc_1_DD8C
loc_1_DD67:
        cmp    ebx, 2
        jne    SHORT loc_1_DD88
        mov    dword ptr [esp + 10h], 0
        push   ebx
        push   0
        push   0
        push   dword ptr [edi + 14h]
        call   sub_1_103FD
        add    esp, 10h
        mov    ebx, eax
        jmp    SHORT loc_1_DD8C
loc_1_DD88:
        mov    ebx, dword ptr [esp + 20h]
loc_1_DD8C:
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_1_DDA8
        cmp    dword ptr [esi + 39h], 0
        jne    SHORT loc_1_DDA8
        mov    eax, dword ptr [esi + 31h]
        mov    dword ptr [esi + 39h], eax
        cmp    dword ptr [esi + 59h], 7bh
        je     SHORT loc_1_DDA8
        mov    dword ptr [esi + 59h], eax
loc_1_DDA8:
        cmp    ebx, dword ptr [esp + 10h]
        mov    eax, 0
        jne    SHORT loc_1_DDB7
        mov    eax, dword ptr [esp + 34h]
loc_1_DDB7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  000h
sub_1_DDC0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    esi, esi
        mov    eax, dword ptr [esp + 20h]
        mov    ecx, eax
        mov    edx, dword ptr [esp + 1ch]
        sub    ecx, edx
        cmp    ecx, 4
        jb     NEAR PTR loc_1_DE78
        db  0C7h, 005h, 080h, 038h, 001h, 000h, 020h, 04Dh, 043h, 043h
        mov    ecx, dword ptr [esp + 18h]
loc_1_DDEC:
        mov    edi, dword ptr [ecx + 40h]
        mov    bl, byte ptr [edi]
        mov    byte ptr [esp + 10h], bl
        mov    edi, dword ptr [ecx + 40h]
        mov    bl, byte ptr [edi + 1]
        mov    byte ptr [esp + 11h], bl
        mov    edi, dword ptr [ecx + 40h]
        mov    bl, byte ptr [edi + 2]
        mov    byte ptr [esp + 12h], bl
        mov    edi, dword ptr [ecx + 40h]
        mov    bl, byte ptr [edi + 3]
        mov    byte ptr [esp + 13h], bl
        mov    edi, dword ptr [esp + 10h]
        db  089h, 03Dh, 074h, 038h, 001h, 000h
        shl    edi, 10h
        mov    ebx, dword ptr [esp + 10h]
        and    ebx, 0ff00h
        or     ebx, edi
        shl    ebx, 8
        mov    ebp, dword ptr [esp + 10h]
        and    ebp, 0ff0000h
        mov    edi, dword ptr [esp + 10h]
        shr    edi, 10h
        or     ebp, edi
        shr    ebp, 8
        or     ebx, ebp
        db  089h, 01Dh, 07Ch, 038h, 001h, 000h
        cmp    dword ptr [esp + 10h], 43434d20h
        jne    SHORT loc_1_DE5E
        mov    esi, 1
        jmp    SHORT loc_1_DE62
loc_1_DE5E:
        inc    edx
        inc    dword ptr [ecx + 40h]
loc_1_DE62:
        or     esi, esi
        jne    SHORT loc_1_DE71
        lea    edi, [eax - 3]
        cmp    edi, edx
        ja     NEAR PTR loc_1_DDEC
loc_1_DE71:
        cmp    esi, 1
        jne    SHORT loc_1_DE78
        mov    eax, edx
loc_1_DE78:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_DE80:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 28h]
        db  08Bh, 00Dh, 034h, 039h, 001h, 000h
        mov    dword ptr [esp + 14h], ecx
        mov    eax, dword ptr [esp + 20h]
        cmp    eax, 1
        je     SHORT loc_1_DEC7
        cmp    eax, 3
        je     NEAR PTR loc_1_E5C2
        cmp    eax, 4
        je     NEAR PTR loc_1_E607
        cmp    eax, 5
        je     NEAR PTR loc_1_E652
        cmp    eax, 6
        je     NEAR PTR loc_1_E66B
        jmp    NEAR PTR loc_1_E68A
loc_1_DEC7:
        mov    eax, dword ptr [ebx + 38h]
        mov    dword ptr [ebx + 40h], eax
        sub    ebp, ebp
        cmp    dword ptr [esp + 24h], ebp
        je     NEAR PTR loc_1_E68A
loc_1_DED9:
        cmp    dword ptr [ebx + 34h], 1
        je     NEAR PTR loc_1_E266
        cmp    dword ptr [ebx + 4ch], 0
        jne    NEAR PTR loc_1_E0F1
        cmp    dword ptr [ebx + 44h], 0
        jne    NEAR PTR loc_1_E069
        mov    eax, dword ptr [esp + 24h]
        sub    eax, ebp
        mov    dword ptr [esp + 10h], eax
        cmp    eax, 0dh
        jae    SHORT loc_1_DF45
        push   0dh
        call   sub_1_F390
        add    esp, 4
        mov    dword ptr [ebx + 4ch], eax
        mov    byte ptr [ebx + 54h], 0
        mov    edi, eax
        mov    dword ptr [ebx + 50h], edi
        mov    eax, dword ptr [esp + 10h]
        mov    esi, dword ptr [ebx + 40h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 10h]
        add    dword ptr [ebx + 50h], eax
        add    byte ptr [ebx + 54h], al
        add    dword ptr [ebx + 40h], eax
        add    ebp, eax
        jmp    NEAR PTR loc_1_E57C
loc_1_DF45:
        mov    ecx, dword ptr [ebx + 40h]
        mov    esi, dword ptr [ecx + 8]
        mov    edx, esi
        and    edx, 0ff00h
        mov    eax, esi
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, esi
        and    eax, 0ff0000h
        shr    esi, 10h
        or     eax, esi
        shr    eax, 8
        or     edx, eax
        movzx  eax, word ptr [ecx + 2]
        movzx  esi, byte ptr [ecx + 0ch]
        add    esi, eax
        movzx  eax, word ptr [ecx + 6]
        add    esi, eax
        movzx  eax, word ptr [ecx]
        add    esi, eax
        movzx  eax, word ptr [ecx + 4]
        add    esi, eax
        cmp    esi, edx
        je     SHORT loc_1_DF9D
        mov    dword ptr [ebx + 34h], 1
        inc    ecx
        mov    dword ptr [ebx + 40h], ecx
        jmp    NEAR PTR loc_1_E57C
loc_1_DF9D:
        mov    edx, dword ptr [ecx + 4]
        mov    ecx, edx
        and    ecx, 0ff00h
        mov    eax, edx
        shl    eax, 10h
        or     ecx, eax
        shl    ecx, 8
        mov    eax, edx
        and    eax, 0ff0000h
        shr    edx, 10h
        or     eax, edx
        shr    eax, 8
        or     ecx, eax
        mov    dword ptr [ebx + 55h], ecx
        push   ecx
        call   sub_1_F390
        add    esp, 4
        mov    dword ptr [ebx + 44h], eax
        mov    edi, eax
        mov    dword ptr [ebx + 48h], edi
        mov    dword ptr [ebx + 5dh], 0
        add    ebp, 0ch
        mov    esi, dword ptr [ebx + 40h]
        add    esi, 0ch
        mov    dword ptr [ebx + 40h], esi
        mov    edx, dword ptr [esp + 24h]
        sub    edx, ebp
        mov    eax, dword ptr [ebx + 55h]
        cmp    edx, eax
        jb     SHORT loc_1_DFF9
        mov    edx, eax
loc_1_DFF9:
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        add    dword ptr [ebx + 40h], edx
        add    dword ptr [ebx + 48h], edx
        mov    eax, dword ptr [ebx + 5dh]
        add    eax, edx
        mov    dword ptr [ebx + 5dh], eax
        add    ebp, edx
        mov    edx, dword ptr [ebx + 55h]
        cmp    eax, edx
        jne    NEAR PTR loc_1_E57C
        mov    ecx, dword ptr [esp + 14h]
        mov    esi, dword ptr [ecx + 10h]
        or     esi, esi
        je     SHORT loc_1_E044
        mov    eax, dword ptr [ebx + 44h]
        push   edx
        mov    cl, byte ptr [eax]
        mov    dword ptr [ebx + 3ch], eax
        movzx  eax, cl
        push   0
        push   eax
        push   dword ptr [ebx + 4]
        call   esi
        add    esp, 10h
loc_1_E044:
        push   dword ptr [ebx + 44h]
        call   sub_1_F396
        add    esp, 4
        mov    dword ptr [ebx + 5dh], 0
        mov    dword ptr [ebx + 44h], 0
        mov    dword ptr [ebx + 48h], 0
        jmp    NEAR PTR loc_1_E57C
loc_1_E069:
        mov    edx, dword ptr [ebx + 55h]
        sub    edx, dword ptr [ebx + 5dh]
        mov    eax, dword ptr [esp + 24h]
        sub    eax, ebp
        cmp    edx, eax
        jb     SHORT loc_1_E07B
        mov    edx, eax
loc_1_E07B:
        mov    edi, dword ptr [ebx + 48h]
        mov    esi, dword ptr [ebx + 40h]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        add    dword ptr [ebx + 48h], edx
        add    dword ptr [ebx + 40h], edx
        mov    eax, dword ptr [ebx + 5dh]
        add    eax, edx
        mov    dword ptr [ebx + 5dh], eax
        add    ebp, edx
        mov    edx, dword ptr [ebx + 55h]
        cmp    eax, edx
        jne    NEAR PTR loc_1_E57C
        mov    ecx, dword ptr [esp + 14h]
        mov    esi, dword ptr [ecx + 10h]
        or     esi, esi
        je     SHORT loc_1_E0CC
        mov    eax, dword ptr [ebx + 44h]
        push   edx
        mov    cl, byte ptr [eax]
        mov    dword ptr [ebx + 3ch], eax
        movzx  eax, cl
        push   0
        push   eax
        push   dword ptr [ebx + 4]
        call   esi
        add    esp, 10h
loc_1_E0CC:
        push   dword ptr [ebx + 44h]
        call   sub_1_F396
        add    esp, 4
        mov    dword ptr [ebx + 5dh], 0
        mov    dword ptr [ebx + 44h], 0
        mov    dword ptr [ebx + 48h], 0
        jmp    NEAR PTR loc_1_E57C
loc_1_E0F1:
        mov    eax, 0dh
        movzx  ecx, byte ptr [ebx + 54h]
        sub    eax, ecx
        mov    ecx, dword ptr [esp + 24h]
        sub    ecx, ebp
        cmp    eax, ecx
        jb     SHORT loc_1_E108
        mov    eax, ecx
loc_1_E108:
        mov    edi, dword ptr [ebx + 50h]
        mov    esi, dword ptr [ebx + 40h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, 0dh
        add    dword ptr [ebx + 50h], eax
        movzx  edx, byte ptr [ebx + 54h]
        sub    ecx, edx
        cmp    ecx, eax
        jbe    SHORT loc_1_E140
        mov    cl, byte ptr [ebx + 54h]
        add    cl, al
        mov    byte ptr [ebx + 54h], cl
        add    dword ptr [ebx + 40h], eax
        add    ebp, eax
        jmp    NEAR PTR loc_1_E57C
loc_1_E140:
        mov    edx, dword ptr [ebx + 4ch]
        mov    ecx, dword ptr [edx + 8]
        mov    esi, ecx
        and    esi, 0ff00h
        mov    edi, ecx
        shl    edi, 10h
        or     esi, edi
        shl    esi, 8
        mov    edi, ecx
        and    edi, 0ff0000h
        shr    ecx, 10h
        or     edi, ecx
        shr    edi, 8
        or     esi, edi
        movzx  ecx, word ptr [edx + 2]
        movzx  edi, byte ptr [edx + 0ch]
        add    edi, ecx
        movzx  ecx, word ptr [edx + 6]
        add    edi, ecx
        movzx  ecx, word ptr [edx]
        add    edi, ecx
        movzx  ecx, word ptr [edx + 4]
        add    edi, ecx
        cmp    edi, esi
        jne    NEAR PTR loc_1_E55A
        add    dword ptr [ebx + 40h], eax
        add    ebp, eax
        mov    edx, dword ptr [edx + 4]
        mov    ecx, edx
        and    ecx, 0ff00h
        mov    eax, edx
        shl    eax, 10h
        or     ecx, eax
        shl    ecx, 8
        mov    eax, edx
        and    eax, 0ff0000h
        shr    edx, 10h
        or     eax, edx
        shr    eax, 8
        or     ecx, eax
        mov    dword ptr [ebx + 55h], ecx
        push   ecx
        call   sub_1_F390
        add    esp, 4
        mov    dword ptr [ebx + 44h], eax
        mov    edi, eax
        mov    dword ptr [ebx + 48h], edi
        mov    dword ptr [ebx + 5dh], 0
        mov    esi, dword ptr [ebx + 4ch]
        add    esi, 0ch
        movsb
        inc    dword ptr [ebx + 48h]
        inc    dword ptr [ebx + 5dh]
        push   dword ptr [ebx + 4ch]
        call   sub_1_F396
        add    esp, 4
        mov    dword ptr [ebx + 50h], 0
        mov    dword ptr [ebx + 4ch], 0
        mov    byte ptr [ebx + 54h], 0
        mov    edx, dword ptr [ebx + 55h]
        sub    edx, dword ptr [ebx + 5dh]
        mov    eax, dword ptr [esp + 24h]
        sub    eax, ebp
        cmp    edx, eax
        jb     SHORT loc_1_E20F
        mov    edx, eax
loc_1_E20F:
        mov    edi, dword ptr [ebx + 48h]
        mov    esi, dword ptr [ebx + 40h]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        add    ebp, edx
        mov    eax, dword ptr [ebx + 5dh]
        add    eax, edx
        mov    dword ptr [ebx + 5dh], eax
        add    dword ptr [ebx + 48h], edx
        add    dword ptr [ebx + 40h], edx
        mov    edx, dword ptr [ebx + 55h]
        cmp    eax, edx
        jne    NEAR PTR loc_1_E57C
        mov    ecx, dword ptr [esp + 14h]
        mov    esi, dword ptr [ecx + 10h]
        or     esi, esi
        je     NEAR PTR loc_1_E538
        mov    eax, dword ptr [ebx + 44h]
        push   edx
        mov    cl, byte ptr [eax]
        mov    dword ptr [ebx + 3ch], eax
        movzx  eax, cl
        push   0
        push   eax
        push   dword ptr [ebx + 4]
        call   esi
        jmp    NEAR PTR loc_1_E535
loc_1_E266:
        cmp    dword ptr [ebx + 4ch], 0
        jne    NEAR PTR loc_1_E3C6
        push   dword ptr [esp + 24h]
        push   ebp
        push   ebx
        call   sub_1_DDC0
        add    esp, 0ch
        mov    ebp, eax
        cmp    ebp, dword ptr [esp + 24h]
        je     NEAR PTR loc_1_E580
        mov    eax, dword ptr [esp + 24h]
        sub    eax, ebp
        cmp    eax, 0dh
        jb     NEAR PTR loc_1_E58B
        mov    ecx, dword ptr [ebx + 40h]
        mov    esi, dword ptr [ecx + 8]
        mov    edx, esi
        and    edx, 0ff00h
        mov    eax, esi
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, esi
        and    eax, 0ff0000h
        shr    esi, 10h
        or     eax, esi
        shr    eax, 8
        or     edx, eax
        movzx  eax, word ptr [ecx + 2]
        movzx  esi, byte ptr [ecx + 0ch]
        add    esi, eax
        movzx  eax, word ptr [ecx + 6]
        add    esi, eax
        movzx  eax, word ptr [ecx]
        add    esi, eax
        movzx  eax, word ptr [ecx + 4]
        add    esi, eax
        cmp    esi, edx
        je     SHORT loc_1_E2F3
        mov    dword ptr [ebx + 34h], 1
        lea    eax, [ecx + ebp]
        mov    dword ptr [ebx + 40h], eax
        jmp    NEAR PTR loc_1_E57C
loc_1_E2F3:
        mov    dword ptr [ebx + 34h], 0
        mov    ecx, dword ptr [ecx + 4]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    ecx, 10h
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        mov    dword ptr [ebx + 55h], edx
        push   edx
        call   sub_1_F390
        add    esp, 4
        mov    dword ptr [ebx + 44h], eax
        mov    edi, eax
        mov    dword ptr [ebx + 48h], edi
        mov    dword ptr [ebx + 5dh], 0
        mov    esi, dword ptr [ebx + 40h]
        add    esi, 0ch
        mov    dword ptr [ebx + 40h], esi
        add    ebp, 0ch
        mov    edx, dword ptr [esp + 24h]
        sub    edx, ebp
        mov    eax, dword ptr [ebx + 55h]
        cmp    edx, eax
        jb     SHORT loc_1_E356
        mov    edx, eax
loc_1_E356:
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        add    dword ptr [ebx + 40h], edx
        add    dword ptr [ebx + 48h], edx
        mov    eax, dword ptr [ebx + 5dh]
        add    eax, edx
        mov    dword ptr [ebx + 5dh], eax
        add    ebp, edx
        mov    esi, dword ptr [ebx + 55h]
        cmp    eax, esi
        jne    NEAR PTR loc_1_E57C
        mov    ecx, dword ptr [esp + 14h]
        mov    edx, dword ptr [ecx + 10h]
        or     edx, edx
        je     SHORT loc_1_E3A1
        mov    eax, dword ptr [ebx + 44h]
        push   esi
        mov    cl, byte ptr [eax]
        mov    dword ptr [ebx + 3ch], eax
        movzx  eax, cl
        push   0
        push   eax
        push   dword ptr [ebx + 4]
        call   edx
        add    esp, 10h
loc_1_E3A1:
        push   dword ptr [ebx + 44h]
        call   sub_1_F396
        add    esp, 4
        mov    dword ptr [ebx + 5dh], 0
        mov    dword ptr [ebx + 44h], 0
        mov    dword ptr [ebx + 48h], 0
        jmp    NEAR PTR loc_1_E57C
loc_1_E3C6:
        mov    eax, 0dh
        movzx  ecx, byte ptr [ebx + 54h]
        sub    eax, ecx
        mov    ecx, dword ptr [esp + 24h]
        sub    ecx, ebp
        cmp    eax, ecx
        jb     SHORT loc_1_E3DD
        mov    eax, ecx
loc_1_E3DD:
        mov    edi, dword ptr [ebx + 50h]
        mov    esi, dword ptr [ebx + 40h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, 0dh
        add    dword ptr [ebx + 50h], eax
        movzx  edx, byte ptr [ebx + 54h]
        sub    ecx, edx
        cmp    ecx, eax
        jbe    SHORT loc_1_E415
        mov    cl, byte ptr [ebx + 54h]
        add    cl, al
        mov    byte ptr [ebx + 54h], cl
        add    dword ptr [ebx + 40h], eax
        add    ebp, eax
        jmp    NEAR PTR loc_1_E57C
loc_1_E415:
        mov    edx, dword ptr [ebx + 4ch]
        mov    ecx, dword ptr [edx + 8]
        mov    esi, ecx
        and    esi, 0ff00h
        mov    edi, ecx
        shl    edi, 10h
        or     esi, edi
        shl    esi, 8
        mov    edi, ecx
        and    edi, 0ff0000h
        shr    ecx, 10h
        or     edi, ecx
        shr    edi, 8
        or     esi, edi
        movzx  edi, word ptr [edx + 2]
        movzx  ecx, byte ptr [edx + 0ch]
        add    ecx, edi
        movzx  edi, word ptr [edx + 6]
        add    ecx, edi
        movzx  edi, word ptr [edx]
        add    ecx, edi
        movzx  edi, word ptr [edx + 4]
        add    ecx, edi
        cmp    ecx, esi
        jne    NEAR PTR loc_1_E55A
        mov    dword ptr [ebx + 34h], 0
        add    dword ptr [ebx + 40h], eax
        add    ebp, eax
        mov    ecx, dword ptr [edx + 4]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    ecx, 10h
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        mov    dword ptr [ebx + 55h], edx
        push   edx
        call   sub_1_F390
        add    esp, 4
        mov    dword ptr [ebx + 44h], eax
        mov    edi, eax
        mov    dword ptr [ebx + 48h], edi
        mov    dword ptr [ebx + 5dh], 0
        mov    esi, dword ptr [ebx + 4ch]
        add    esi, 0ch
        movsb
        inc    dword ptr [ebx + 48h]
        inc    dword ptr [ebx + 5dh]
        push   dword ptr [ebx + 4ch]
        call   sub_1_F396
        add    esp, 4
        mov    dword ptr [ebx + 50h], 0
        mov    dword ptr [ebx + 4ch], 0
        mov    byte ptr [ebx + 54h], 0
        mov    edx, dword ptr [ebx + 55h]
        sub    edx, dword ptr [ebx + 5dh]
        mov    eax, dword ptr [esp + 24h]
        sub    eax, ebp
        cmp    edx, eax
        jb     SHORT loc_1_E4EB
        mov    edx, eax
loc_1_E4EB:
        mov    edi, dword ptr [ebx + 48h]
        mov    esi, dword ptr [ebx + 40h]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        add    ebp, edx
        mov    eax, dword ptr [ebx + 5dh]
        add    eax, edx
        mov    dword ptr [ebx + 5dh], eax
        add    dword ptr [ebx + 48h], edx
        add    dword ptr [ebx + 40h], edx
        mov    esi, dword ptr [ebx + 55h]
        cmp    eax, esi
        jne    SHORT loc_1_E57C
        mov    ecx, dword ptr [esp + 14h]
        mov    edx, dword ptr [ecx + 10h]
        or     edx, edx
        je     SHORT loc_1_E538
        mov    eax, dword ptr [ebx + 44h]
        push   esi
        mov    cl, byte ptr [eax]
        mov    dword ptr [ebx + 3ch], eax
        movzx  eax, cl
        push   0
        push   eax
        push   dword ptr [ebx + 4]
        call   edx
loc_1_E535:
        add    esp, 10h
loc_1_E538:
        push   dword ptr [ebx + 44h]
        call   sub_1_F396
        add    esp, 4
        mov    dword ptr [ebx + 48h], 0
        mov    dword ptr [ebx + 44h], 0
        mov    dword ptr [ebx + 5dh], 0
        jmp    SHORT loc_1_E57C
loc_1_E55A:
        push   edx
        call   sub_1_F396
        add    esp, 4
        mov    dword ptr [ebx + 34h], 1
        mov    dword ptr [ebx + 50h], 0
        mov    dword ptr [ebx + 4ch], 0
        mov    byte ptr [ebx + 54h], 0
loc_1_E57C:
        cmp    ebp, dword ptr [esp + 24h]
loc_1_E580:
        jb     NEAR PTR loc_1_DED9
        jmp    NEAR PTR loc_1_E68A
loc_1_E58B:
        push   0dh
        call   sub_1_F390
        add    esp, 4
        mov    dword ptr [ebx + 4ch], eax
        mov    edi, eax
        mov    dword ptr [ebx + 50h], edi
        mov    eax, dword ptr [esp + 24h]
        sub    eax, ebp
        mov    esi, dword ptr [ebx + 40h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        add    byte ptr [ebx + 54h], al
        add    dword ptr [ebx + 50h], eax
        add    dword ptr [ebx + 40h], eax
        jmp    NEAR PTR loc_1_E68A
loc_1_E5C2:
        mov    esi, dword ptr [ebx + 4]
        push   esi
        call   sub_1_35D1
        add    esp, 4
        cmp    byte ptr [eax + 1ah], 0
        jne    NEAR PTR loc_1_E68A
        db  083h, 03Dh, 058h, 037h, 001h, 000h, 000h
        jne    NEAR PTR loc_1_E68A
        push   esi
        call   sub_1_5230
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_1_E68A
        push   1
        push   12h
        call   sub_1_F02D
        add    esp, 8
        jmp    NEAR PTR loc_1_E68A
loc_1_E607:
        db  0A1h, 034h, 039h, 001h, 000h
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        je     SHORT loc_1_E62D
        push   0
        cmp    dword ptr [esp + 28h], 1
        push   0
        sbb    ecx, ecx
        add    ecx, 0e8h
        push   ecx
        push   dword ptr [ebx + 4]
        call   eax
        add    esp, 10h
loc_1_E62D:
        mov    ecx, dword ptr [esp + 14h]
        mov    eax, dword ptr [ecx + 4]
        inc    eax
        mov    dword ptr [ecx + 4], eax
        cmp    dword ptr [ecx + 8], eax
        jne    SHORT loc_1_E68A
        mov    eax, ecx
        add    eax, 14h
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_E68A
        push   eax
        call   sub_1_EE79
        add    esp, 4
        jmp    SHORT loc_1_E68A
loc_1_E652:
        db  0A1h, 034h, 039h, 001h, 000h
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        je     SHORT loc_1_E68A
        push   dword ptr [esp + 24h]
        push   0
        push   0ffh
        jmp    SHORT loc_1_E682
loc_1_E66B:
        db  0A1h, 034h, 039h, 001h, 000h
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        je     SHORT loc_1_E68A
        push   dword ptr [esp + 24h]
        push   0
        push   0e3h
loc_1_E682:
        push   dword ptr [ebx + 4]
        call   eax
        add    esp, 10h
loc_1_E68A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_E692:
        mov    eax, dword ptr [esp + 8]
        cmp    eax, 7d0h
        je     SHORT loc_1_E6AD
        cmp    eax, 7d1h
        je     SHORT loc_1_E6D5
        cmp    eax, 7d2h
        je     SHORT loc_1_E71F
        jmp    SHORT loc_1_E724
loc_1_E6AD:
        mov    ecx, dword ptr [esp + 10h]
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_1_E6CA
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [ecx], eax
        add    ecx, 14h
        push   ecx
        call   sub_1_EE79
        add    esp, 4
        jmp    SHORT loc_1_E724
loc_1_E6CA:
        push   ecx
        call   sub_1_E842
        add    esp, 4
        jmp    SHORT loc_1_E724
loc_1_E6D5:
        cmp    dword ptr [esp + 0ch], 0
        jne    SHORT loc_1_E6EF
        call   sub_1_F05F
        db  0FFh, 035h, 034h, 039h, 001h, 000h
        call   sub_1_F0C3
        add    esp, 4
loc_1_E6EF:
        db  0C7h, 005h, 054h, 036h, 001h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 034h, 039h, 001h, 000h
        or     eax, eax
        je     SHORT loc_1_E724
        mov    ecx, eax
        add    eax, 20h
        mov    dword ptr [ecx], 0
        mov    dword ptr [eax - 18h], 0
        push   eax
        call   sub_1_EE79
        add    esp, 4
        jmp    SHORT loc_1_E724
loc_1_E71F:
        call   sub_1_F05F
loc_1_E724:
        sub    eax, eax
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
