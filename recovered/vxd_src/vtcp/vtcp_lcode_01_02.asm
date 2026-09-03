; vtcp_lcode_01_02.asm -- VTCP object 1 segment _LTEXT  (part 2/3)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_487C:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    esi, dword ptr [ebp + 8]
loc_1_488E:
        mov    edx, dword ptr [esi + 0a8h]
        mov    dword ptr [esi + 54h], edx
        or     dword ptr [esi + 58h], 80h
        mov    ecx, dword ptr [esi + 0ach]
        lea    eax, [ebp - 18h]
        push   eax
        push   dword ptr [ecx + 8]
        lea    eax, [ebp - 4]
        push   eax
        push   edx
        push   dword ptr [ecx + 0ch]
        push   620h
        push   dword ptr [esi + 0b8h]
        push   dword ptr [esi + 0e0h]
        call   dword ptr [esi + 0dch]
        add    esp, 20h
        and    dword ptr [esi + 58h], 0ffffff7fh
        cmp    eax, 18h
        je     SHORT loc_1_4944
        cmp    eax, 17h
        jne    SHORT loc_1_48E5
        mov    dword ptr [ebp - 4], 0
loc_1_48E5:
        mov    eax, dword ptr [ebp - 4]
        sub    dword ptr [esi + 54h], eax
        mov    edi, dword ptr [esi + 0a8h]
        mov    eax, dword ptr [esi + 0ach]
        cmp    dword ptr [ebp - 4], edi
        je     NEAR PTR loc_1_4A0C
        push   dword ptr [ebp - 4]
        push   eax
        call   sub_1_40F0
        add    esp, 8
        cmp    dword ptr [esi + 0a0h], 0
        jne    NEAR PTR loc_1_4A50
        mov    dword ptr [esi + 0ach], eax
        mov    eax, dword ptr [ebp - 4]
        sub    dword ptr [esi + 0a8h], eax
        cmp    dword ptr [esi + 54h], 0
        jne    NEAR PTR loc_1_4A81
        cmp    dword ptr [esi + 0dch], 0
        jne    NEAR PTR loc_1_488E
        jmp    NEAR PTR loc_1_4A81
loc_1_4944:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ecx + 20h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 18h], eax
        mov    eax, dword ptr [ebp - 8]
        movzx  eax, word ptr [eax]
        mov    dword ptr [ecx + 14h], eax
        mov    dword ptr [ecx + 10h], 0
        mov    dword ptr [ecx + 0ch], 0
        mov    eax, dword ptr [esi + 0a0h]
        or     eax, eax
        jne    SHORT loc_1_499C
        mov    dword ptr [esi + 0a0h], ecx
        mov    dword ptr [esi + 0a4h], ecx
        mov    dword ptr [ecx], 0
        jmp    SHORT loc_1_49A4
        db  08Bh, 0FFh
loc_1_499C:
        mov    dword ptr [ecx], eax
        mov    dword ptr [esi + 0a0h], ecx
loc_1_49A4:
        mov    dword ptr [esi + 50h], ecx
        db  0C7h, 046h, 060h, 054h, 043h, 000h, 000h
        mov    edi, dword ptr [esi + 0a8h]
        mov    eax, dword ptr [esi + 0ach]
        mov    dword ptr [esi + 0a8h], 0
        mov    dword ptr [esi + 0ach], 0
        mov    ecx, dword ptr [ebp - 4]
        sub    dword ptr [esi + 54h], ecx
        sub    edi, dword ptr [ebp - 4]
        je     SHORT loc_1_49FC
        push   dword ptr [ebp - 4]
        push   eax
        call   sub_1_40F0
        add    esp, 8
        push   edi
        push   eax
        push   800h
        push   esi
        call   sub_1_4354
        add    esp, 10h
        pop    edi
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_49FC:
        push   eax
        call   sub_1_2A58
        add    esp, 4
        pop    edi
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_4A0C:
        mov    dword ptr [esi + 0a8h], 0
        mov    dword ptr [esi + 0ach], 0
        cmp    dword ptr [esi + 0a0h], 0
        jne    SHORT loc_1_4A3C
        cmp    dword ptr [esi + 0dch], 0
        je     SHORT loc_1_4A43
        db  0C7h, 046h, 060h, 020h, 046h, 000h, 000h
        jmp    SHORT loc_1_4A43
        db  090h
loc_1_4A3C:
        db  0C7h, 046h, 060h, 054h, 043h, 000h, 000h
loc_1_4A43:
        push   eax
        call   sub_1_2A58
        add    esp, 4
        jmp    SHORT loc_1_4A81
        db  08Bh, 0FFh
loc_1_4A50:
        db  0C7h, 046h, 060h, 054h, 043h, 000h, 000h
        mov    dword ptr [esi + 0a8h], 0
        mov    dword ptr [esi + 0ach], 0
        sub    edi, dword ptr [ebp - 4]
        push   edi
        push   eax
        push   800h
        push   esi
        call   sub_1_4354
        add    esp, 10h
        add    dword ptr [ebp - 4], eax
loc_1_4A81:
        push   dword ptr [ebp + 0ch]
        call   sub_1_408C
        add    esp, 4
        pop    edi
        pop    esi
        leave
        ret
sub_1_4A90:
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 10h], 0
        mov    edi, dword ptr [esp + 40h]
        or     edi, edi
        mov    esi, dword ptr [esp + 3ch]
        je     SHORT loc_1_4AD8
        cmp    dword ptr [esi + 0fch], 0
        jne    SHORT loc_1_4ABA
        test   byte ptr [esi + 59h], 40h
        je     SHORT loc_1_4B0C
loc_1_4ABA:
        add    esi, 0fch
        cmp    dword ptr [esi], 0
        je     SHORT loc_1_4ACC
loc_1_4AC5:
        mov    esi, dword ptr [esi]
        cmp    dword ptr [esi], 0
        jne    SHORT loc_1_4AC5
loc_1_4ACC:
        mov    dword ptr [esi], edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
        db  08Bh, 0FFh
loc_1_4AD8:
        mov    edi, dword ptr [esi + 0fch]
        or     edi, edi
        je     NEAR PTR loc_1_4DCE
        test   byte ptr [esi + 59h], 40h
        jne    NEAR PTR loc_1_4DCE
        mov    ebx, dword ptr [esi + 100h]
        mov    dword ptr [esi + 0fch], 0
        mov    dword ptr [esi + 100h], 0
        jmp    SHORT loc_1_4B10
loc_1_4B0C:
        mov    ebx, dword ptr [esp + 44h]
loc_1_4B10:
        inc    byte ptr [esi + 82h]
        or     dword ptr [esi + 58h], 4000h
        or     dword ptr [esi + 5ch], 1
loc_1_4B21:
        mov    dword ptr [esp + 10h], 0
        mov    ebp, dword ptr [esi + 0f8h]
        or     ebp, ebp
        je     SHORT loc_1_4B40
        mov    eax, dword ptr [ebp]
        mov    dword ptr [esi + 0f8h], eax
        jmp    SHORT loc_1_4BB6
        db  08Bh, 0FFh
loc_1_4B40:
        lea    ecx, [esi + 0a0h]
        mov    ebp, dword ptr [ecx]
        or     ebp, ebp
        je     NEAR PTR loc_1_4C04
loc_1_4B50:
        test   byte ptr [ebp + 14h], 40h
        je     SHORT loc_1_4B5C
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_1_4B68
loc_1_4B5C:
        mov    ecx, ebp
        mov    ebp, dword ptr [ebp]
        or     ebp, ebp
        jne    SHORT loc_1_4B50
        jmp    SHORT loc_1_4BB6
        db  090h
loc_1_4B68:
        cmp    dword ptr [esi + 0a4h], ebp
        jne    SHORT loc_1_4B88
        cmp    dword ptr [esi + 0a0h], ebp
        mov    dword ptr [esi + 0a4h], 0
        je     SHORT loc_1_4B88
        mov    dword ptr [esi + 0a4h], ecx
loc_1_4B88:
        mov    eax, dword ptr [ebp]
        mov    dword ptr [ecx], eax
        cmp    dword ptr [esi + 50h], ebp
        jne    SHORT loc_1_4BB6
        mov    dword ptr [esi + 50h], eax
        or     eax, eax
        jne    SHORT loc_1_4BB6
        cmp    dword ptr [esi + 0dch], 0
        je     SHORT loc_1_4BAF
        cmp    dword ptr [esi + 54h], 0
        db  0C7h, 046h, 060h, 020h, 046h, 000h, 000h
        je     SHORT loc_1_4BB6
loc_1_4BAF:
        db  0C7h, 046h, 060h, 0D4h, 042h, 000h, 000h
loc_1_4BB6:
        or     ebp, ebp
        je     SHORT loc_1_4C04
        push   0
        push   ebx
        push   dword ptr [ebp + 20h]
        push   edi
        call   sub_1_1DD0
        add    esp, 10h
        mov    dword ptr [esp + 10h], eax
        push   eax
        push   0
        push   dword ptr [ebp + 8]
        call   dword ptr [ebp + 4]
        add    esp, 0ch
        push   ebp
        call   sub_1_408C
        add    esp, 4
        mov    eax, dword ptr [esi + 104h]
        mov    ecx, eax
        mov    edx, dword ptr [esp + 10h]
        sub    ecx, edx
        sbb    ebp, ebp
        and    ecx, ebp
        add    ecx, edx
        sub    eax, ecx
        mov    dword ptr [esi + 104h], eax
        jmp    NEAR PTR loc_1_4D0D
        db  090h
loc_1_4C04:
        cmp    dword ptr [esi + 104h], 0
        jne    NEAR PTR loc_1_4D96
        mov    ecx, dword ptr [esi + 0b8h]
        mov    eax, dword ptr [esi + 0cch]
        or     eax, eax
        je     NEAR PTR loc_1_4D8C
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        je     NEAR PTR loc_1_4D96
        test   byte ptr [eax + 32h], 80h
        je     NEAR PTR loc_1_4D96
        mov    edx, dword ptr [eax + 8ch]
        or     edx, edx
        je     NEAR PTR loc_1_4D96
        mov    eax, dword ptr [eax + 90h]
        mov    dword ptr [esi + 104h], ebx
        lea    ebp, [esp + 24h]
        push   ebp
        push   dword ptr [edi + 8]
        lea    ebp, [esp + 18h]
        push   ebp
        push   ebx
        push   dword ptr [edi + 0ch]
        push   640h
        push   ecx
        push   eax
        call   edx
        add    esp, 20h
        cmp    eax, 18h
        jne    SHORT loc_1_4CE4
        cmp    ebx, dword ptr [esp + 10h]
        mov    eax, 0
        je     SHORT loc_1_4CA3
        push   dword ptr [esp + 10h]
        push   edi
        call   sub_1_40F0
        add    esp, 8
        mov    edi, eax
        push   0
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 2ch]
        push   edi
        call   sub_1_1DD0
        add    esp, 10h
loc_1_4CA3:
        add    dword ptr [esp + 10h], eax
        mov    ecx, dword ptr [esi + 104h]
        mov    edx, ecx
        mov    ebp, dword ptr [esp + 10h]
        mov    dword ptr [esp + 14h], ebp
        sub    ecx, ebp
        mov    ebp, dword ptr [esp + 18h]
        sbb    dword ptr [esp + 18h], ebp
        and    ecx, dword ptr [esp + 18h]
        add    ecx, dword ptr [esp + 14h]
        sub    edx, ecx
        mov    dword ptr [esi + 104h], edx
        push   eax
        push   0
        push   dword ptr [esp + 38h]
        call   dword ptr [esp + 38h]
        add    esp, 0ch
        jmp    SHORT loc_1_4D0D
        db  08Dh, 049h, 000h
loc_1_4CE4:
        cmp    eax, 17h
        jne    SHORT loc_1_4CF1
        mov    dword ptr [esp + 10h], 0
loc_1_4CF1:
        mov    eax, dword ptr [esi + 104h]
        mov    ecx, eax
        mov    edx, dword ptr [esp + 10h]
        sub    eax, edx
        sbb    ebp, ebp
        and    eax, ebp
        add    eax, edx
        sub    ecx, eax
        mov    dword ptr [esi + 104h], ecx
loc_1_4D0D:
        cmp    ebx, dword ptr [esp + 10h]
        jne    SHORT loc_1_4D30
        push   edi
        call   sub_1_2A58
        add    esp, 4
        mov    edi, dword ptr [esi + 0fch]
        lea    ebp, [esi + 100h]
        mov    ebx, dword ptr [ebp]
        jmp    SHORT loc_1_4D70
        db  08Dh, 049h, 000h
loc_1_4D30:
        push   dword ptr [esp + 10h]
        push   edi
        call   sub_1_40F0
        add    esp, 8
        mov    edi, eax
        lea    ebp, [esi + 100h]
        sub    ebx, dword ptr [esp + 10h]
        add    ebx, dword ptr [ebp]
        lea    eax, [esi + 0fch]
        mov    dword ptr [esp + 1ch], eax
        cmp    dword ptr [esi + 0fch], 0
        je     SHORT loc_1_4D70
        push   edi
        call   sub_1_40DC
        add    esp, 4
        mov    ecx, dword ptr [esi + 0fch]
        mov    dword ptr [eax], ecx
loc_1_4D70:
        or     edi, edi
        mov    dword ptr [esi + 0fch], 0
        mov    dword ptr [ebp], 0
        jne    NEAR PTR loc_1_4B21
        jmp    SHORT loc_1_4DB3
        db  090h
loc_1_4D8C:
        mov    ecx, dword ptr [esp + 20h]
        mov    eax, dword ptr [esp + 48h]
        mov    dword ptr [eax], ecx
loc_1_4D96:
        push   edi
        call   sub_1_40DC
        add    esp, 4
        mov    ecx, dword ptr [esi + 0fch]
        mov    dword ptr [eax], ecx
        mov    dword ptr [esi + 0fch], edi
        add    dword ptr [esi + 100h], ebx
loc_1_4DB3:
        mov    al, byte ptr [esi + 82h]
        and    dword ptr [esi + 58h], 0ffffbfffh
        dec    al
        mov    byte ptr [esi + 82h], al
        jne    SHORT loc_1_4DCE
        and    dword ptr [esi + 5ch], 0fffffffeh
loc_1_4DCE:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
        db  08Bh, 0FFh
sub_1_4DD8:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    esi, dword ptr [edx + 0a0h]
        mov    eax, esi
        or     eax, eax
        je     SHORT loc_1_4DF6
loc_1_4DE9:
        or     dword ptr [eax + 14h], 80000000h
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_4DE9
loc_1_4DF6:
        mov    ecx, dword ptr [edx + 0f8h]
        mov    eax, ecx
        or     eax, eax
        je     SHORT loc_1_4E0F
loc_1_4E02:
        or     dword ptr [eax + 14h], 80000000h
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_4E02
loc_1_4E0F:
        or     esi, esi
        jne    SHORT loc_1_4E17
        or     ecx, ecx
        je     SHORT loc_1_4E25
loc_1_4E17:
        push   100h
        push   edx
        call   sub_1_2388
        add    esp, 8
loc_1_4E25:
        pop    esi
        ret
        db  090h
sub_1_4E28:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 24h]
        or     edi, edi
        mov    esi, dword ptr [esp + 20h]
        je     SHORT loc_1_4E94
        lea    eax, [esi + 10h]
        push   eax
        call   sub_1_90F6
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_1_4E78
        mov    dword ptr [ebx + 0ch], esi
        mov    dword ptr [ebx + 4], 3
        lea    eax, [ebx + 10h]
        mov    dword ptr [ebx + 8], eax
        mov    dword ptr [ebx], 0
        push   dword ptr [esp + 1ch]
        push   esi
        push   dword ptr [esp + 1ch]
        push   eax
        call   sub_1_1E94
        add    esp, 10h
        mov    dword ptr [edi], ebx
        jmp    SHORT loc_1_4E94
        db  08Bh, 0FFh
loc_1_4E78:
        mov    dword ptr [edi], 0
        mov    edi, dword ptr [esp + 28h]
        or     edi, edi
        je     SHORT loc_1_4EF2
        mov    dword ptr [edi], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_4E94:
        mov    edi, dword ptr [esp + 28h]
        or     edi, edi
        je     SHORT loc_1_4EF2
        mov    ebx, dword ptr [esp + 18h]
        sub    ebx, dword ptr [esp + 1ch]
        sub    ebx, esi
        lea    eax, [ebx + 10h]
        push   eax
        call   sub_1_90F6
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_1_4EEC
        mov    dword ptr [ebp + 0ch], ebx
        mov    dword ptr [ebp + 4], 3
        lea    eax, [ebp + 10h]
        mov    dword ptr [ebp + 8], eax
        mov    dword ptr [ebp], 0
        mov    ecx, dword ptr [esp + 1ch]
        add    ecx, esi
        push   ecx
        push   ebx
        push   dword ptr [esp + 1ch]
        push   eax
        call   sub_1_1E94
        add    esp, 10h
        mov    dword ptr [edi], ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_4EEC:
        mov    dword ptr [edi], 0
loc_1_4EF2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
sub_1_4EF8:
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 48h]
        mov    ecx, dword ptr [esi + 58h]
        test   ch, 20h
        je     SHORT loc_1_4F3C
        mov    edi, dword ptr [esp + 4ch]
        mov    ebp, dword ptr [edi + 0ch]
        or     ebp, ebp
        je     SHORT loc_1_4F2C
        mov    eax, dword ptr [esp + 54h]
        cmp    dword ptr [eax], ebp
        jb     SHORT loc_1_4F2C
        mov    dword ptr [esp + 14h], 1
        dec    ebp
        jmp    SHORT loc_1_4F58
        db  08Dh, 049h, 000h
loc_1_4F2C:
        pop    ebp
        and    dword ptr [edi + 10h], 0ffffdfffh
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  090h
loc_1_4F3C:
        sub    ebp, ebp
        mov    edi, dword ptr [esp + 4ch]
        mov    edx, dword ptr [edi + 0ch]
        inc    edx
        mov    eax, dword ptr [esp + 54h]
        mov    eax, dword ptr [eax]
        sub    edx, eax
        sbb    ebx, ebx
        and    edx, ebx
        add    eax, edx
        mov    dword ptr [esp + 14h], eax
loc_1_4F58:
        mov    eax, dword ptr [esp + 54h]
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [esp + 14h]
        sub    eax, ebp
        mov    dword ptr [esp + 24h], eax
        mov    eax, dword ptr [edi]
        add    eax, ebp
        mov    dword ptr [esp + 1ch], eax
        mov    edx, dword ptr [esp + 14h]
        mov    dword ptr [esp + 28h], edx
        lea    eax, [edx + eax - 1]
        mov    dword ptr [esp + 18h], eax
        test   ecx, 4000000h
        jne    NEAR PTR loc_1_5120
        mov    dword ptr [esp + 10h], 0
        test   byte ptr [esi + 59h], 80h
        je     NEAR PTR loc_1_5058
        mov    edx, dword ptr [esi + 40h]
        mov    ebx, dword ptr [esi + 108h]
        mov    eax, edx
        sub    eax, ebx
        jns    NEAR PTR loc_1_5058
        mov    ebp, dword ptr [esi + 10ch]
        mov    ecx, ebp
        sub    ecx, ebx
        inc    ecx
        mov    eax, dword ptr [esp + 54h]
        mov    eax, dword ptr [eax]
        sub    ebx, edx
        cmp    ebx, eax
        jl     SHORT loc_1_4FCA
        mov    ebx, eax
loc_1_4FCA:
        sub    edx, ebp
        lea    ebp, [edx + eax]
        or     ebp, ebp
        jne    SHORT loc_1_4FD5
        sub    ebp, ebp
loc_1_4FD5:
        or     ebp, ebp
        lea    edx, [esp + 10h]
        jne    SHORT loc_1_4FDF
        sub    edx, edx
loc_1_4FDF:
        push   edx
        push   0
        push   ecx
        push   ebx
        push   eax
        push   dword ptr [esp + 64h]
        call   sub_1_4E28
        add    esp, 18h
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_5041
        mov    eax, dword ptr [esi + 10ch]
        inc    eax
        mov    dword ptr [esp + 30h], eax
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esp + 40h], eax
        mov    eax, dword ptr [esp + 18h]
        sub    eax, dword ptr [esp + 30h]
        mov    dword ptr [esp + 3ch], eax
        test   byte ptr [esi + 59h], 20h
        je     SHORT loc_1_5020
        inc    dword ptr [esp + 3ch]
loc_1_5020:
        push   ebp
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esp + 38h], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esp + 3ch], eax
        push   dword ptr [esp + 14h]
        lea    eax, [esp + 38h]
        push   eax
        push   esi
        call   sub_1_2C28
        add    esp, 10h
loc_1_5041:
        mov    eax, dword ptr [esp + 54h]
        mov    dword ptr [eax], ebx
        and    dword ptr [edi + 10h], 0ffffdfffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  08Bh, 0FFh
loc_1_5058:
        lea    eax, [esp + 10h]
        cmp    dword ptr [esp + 24h], 0
        jne    SHORT loc_1_5065
        sub    eax, eax
loc_1_5065:
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        push   dword ptr [esp + 1ch]
        push   ebp
        mov    eax, dword ptr [esp + 64h]
        push   dword ptr [eax]
        push   dword ptr [esp + 64h]
        call   sub_1_4E28
        add    esp, 18h
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_50C5
        mov    eax, dword ptr [edi]
        add    eax, dword ptr [esp + 28h]
        add    eax, ebp
        mov    dword ptr [esp + 30h], eax
        mov    eax, dword ptr [edi + 10h]
        and    eax, 0ffffdfffh
        mov    dword ptr [esp + 40h], eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esp + 34h], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esp + 38h], eax
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 14h]
        lea    eax, [esp + 38h]
        push   eax
        push   esi
        call   sub_1_2C28
        add    esp, 10h
loc_1_50C5:
        cmp    dword ptr [esp + 20h], 0
        je     SHORT loc_1_5110
        mov    eax, dword ptr [esi + 58h]
        test   ah, 80h
        jne    SHORT loc_1_50E6
        or     eax, 8000h
        mov    dword ptr [esi + 58h], eax
        inc    byte ptr [esi + 82h]
        or     dword ptr [esi + 5ch], 1
loc_1_50E6:
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [esi + 108h], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esi + 10ch], eax
        lea    eax, [esp + 2ch]
        push   eax
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 28h]
        push   esi
        call   sub_1_4A90
        add    esp, 10h
loc_1_5110:
        mov    eax, dword ptr [esp + 54h]
        mov    dword ptr [eax], ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  08Bh, 0FFh
loc_1_5120:
        and    dword ptr [edi + 10h], 0ffffdfffh
        test   ch, 80h
        je     SHORT loc_1_5144
        mov    eax, dword ptr [esp + 18h]
        sub    eax, dword ptr [esi + 10ch]
        or     eax, eax
        jg     SHORT loc_1_5157
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  08Bh, 0FFh
loc_1_5144:
        or     ecx, 8000h
        mov    dword ptr [esi + 58h], ecx
        inc    byte ptr [esi + 82h]
        or     dword ptr [esi + 5ch], 1
loc_1_5157:
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [esi + 108h], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esi + 10ch], eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  090h
loc_1_5174:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 18h]
        push   dword ptr [esi]
        call   sub_1_5F38
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_53DC
        mov    edi, dword ptr [eax]
        or     edi, edi
        je     NEAR PTR loc_1_53CC
        mov    ebp, dword ptr [esp + 1ch]
        mov    bx, word ptr [ebp]
        mov    al, byte ptr [edi + 84h]
        cmp    al, 4
        jb     SHORT loc_1_51B1
        cmp    al, 6
        jbe    SHORT loc_1_51D5
loc_1_51B1:
        cmp    dword ptr [edi + 0a8h], 0
        je     SHORT loc_1_51BF
        test   bl, 20h
        jne    SHORT loc_1_51D5
loc_1_51BF:
        cmp    dword ptr [edi + 100h], 0
        je     NEAR PTR loc_1_53BC
        test   bl, 40h
        je     NEAR PTR loc_1_53BC
loc_1_51D5:
        test   byte ptr [edi + 86h], 1
        jne    NEAR PTR loc_1_53BC
        call   sub_1_40A0
        or     eax, eax
        je     NEAR PTR loc_1_53AC
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [esp + 24h]
        mov    dword ptr [eax + 20h], ecx
        mov    ecx, dword ptr [esp + 20h]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [eax + 1ch], ecx
        mov    dword ptr [eax + 18h], ebp
        mov    dword ptr [eax + 10h], 0
        mov    dword ptr [eax + 0ch], 0
        movzx  ecx, bx
        mov    dword ptr [eax + 14h], ecx
        mov    cx, bx
        and    cl, 60h
        cmp    cl, 40h
        je     NEAR PTR loc_1_5348
        mov    dword ptr [eax], 0
        cmp    dword ptr [edi + 0a0h], 0
        jne    SHORT loc_1_5248
        mov    dword ptr [edi + 0a0h], eax
        jmp    SHORT loc_1_5250
loc_1_5248:
        mov    ecx, dword ptr [edi + 0a4h]
        mov    dword ptr [ecx], eax
loc_1_5250:
        mov    dword ptr [edi + 0a4h], eax
        test   bl, 40h
        je     SHORT loc_1_5294
        cmp    dword ptr [edi + 100h], 0
        je     SHORT loc_1_5294
        inc    dword ptr [edi + 74h]
        lea    eax, [esp + 10h]
        push   eax
        push   0
        push   0
        push   edi
        call   sub_1_4A90
        add    esp, 10h
        push   dword ptr [esp + 10h]
        push   edi
        call   sub_1_63D8
        add    esp, 8
        mov    eax, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
loc_1_5294:
        cmp    dword ptr [edi + 50h], 0
        jne    NEAR PTR loc_1_5338
        mov    dword ptr [edi + 50h], eax
        test   byte ptr [edi + 58h], 80h
        jne    NEAR PTR loc_1_5338
        db  0C7h, 047h, 060h, 054h, 043h, 000h, 000h
        mov    eax, dword ptr [edi + 0ach]
        or     eax, eax
        jne    SHORT loc_1_52CC
        mov    eax, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_52CC:
        mov    ecx, dword ptr [edi + 0a8h]
        mov    dword ptr [edi + 0ach], 0
        mov    dword ptr [edi + 0a8h], 0
        inc    dword ptr [edi + 74h]
        push   ecx
        push   eax
        push   800h
        push   edi
        call   sub_1_4354
        add    esp, 10h
        mov    esi, dword ptr [edi + 44h]
        push   edi
        call   sub_1_7DDC
        add    esp, 4
        cmp    eax, esi
        je     SHORT loc_1_5317
        push   200h
        push   edi
        call   sub_1_2388
        add    esp, 8
loc_1_5317:
        push   dword ptr [esp + 10h]
        push   edi
        call   sub_1_63D8
        add    esp, 8
        call   sub_1_22C8
        mov    eax, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_5338:
        pop    ebp
        mov    eax, 0ffh
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_5348:
        lea    ecx, [edi + 0f8h]
        cmp    dword ptr [ecx], 0
        je     SHORT loc_1_535B
loc_1_5354:
        mov    ecx, dword ptr [ecx]
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_1_5354
loc_1_535B:
        mov    dword ptr [eax], 0
        mov    dword ptr [ecx], eax
        cmp    dword ptr [edi + 0fch], 0
        je     SHORT loc_1_539C
        inc    dword ptr [edi + 74h]
        lea    eax, [esp + 10h]
        push   eax
        push   0
        push   0
        push   edi
        call   sub_1_4A90
        add    esp, 10h
        push   dword ptr [esp + 10h]
        push   edi
        call   sub_1_63D8
        add    esp, 8
        mov    eax, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
loc_1_539C:
        pop    ebp
        mov    eax, 0ffh
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_53AC:
        pop    ebp
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_53BC:
        pop    ebp
        mov    eax, 19h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_53CC:
        pop    ebp
        mov    eax, 19h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_53DC:
        pop    ebp
        mov    eax, 0fh
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_53EC:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [ecx + 4]
        test   byte ptr [edx + 86h], 1
        je     SHORT loc_1_540C
        mov    esi, dword ptr [esp + 0ch]
        mov    dword ptr [esi], 1
        jmp    SHORT loc_1_541A
        db  08Dh, 049h, 000h
loc_1_540C:
        movzx  eax, byte ptr [edx + 84h]
        inc    eax
        mov    esi, dword ptr [esp + 0ch]
        mov    dword ptr [esi], eax
loc_1_541A:
        mov    eax, dword ptr [edx + 68h]
        mov    dword ptr [esi + 4], eax
        movzx  eax, word ptr [edx + 6eh]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [edx + 64h]
        mov    dword ptr [esi + 0ch], eax
        movzx  eax, word ptr [edx + 6ch]
        mov    dword ptr [esi + 10h], eax
        mov    eax, dword ptr [edx]
        or     eax, eax
        je     SHORT loc_1_5444
        mov    dword ptr [ecx + 4], eax
        mov    eax, 1
        pop    esi
        ret
loc_1_5444:
        mov    edx, dword ptr [ecx]
        inc    edx
        cmp    edx, 20h
        jae    SHORT loc_1_5464
        inc    eax
        db  08Dh, 004h, 095h, 0BCh, 094h, 000h, 000h
loc_1_5454:
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_5478
        add    eax, 4
        inc    edx
        db  03Dh, 03Ch, 095h, 000h, 000h
        jb     SHORT loc_1_5454
loc_1_5464:
        sub    eax, eax
        mov    dword ptr [ecx], 0
        mov    dword ptr [ecx + 4], 0
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_5478:
        pop    esi
        db  08Bh, 004h, 095h, 0BCh, 094h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx], edx
        mov    eax, 1
        ret
        db  090h
loc_1_548C:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    esi, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        or     esi, esi
        jne    SHORT loc_1_54D8
        or     eax, eax
        jne    SHORT loc_1_54D8
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], 1
        inc    eax
        db  08Dh, 004h, 0B5h, 0BCh, 094h, 000h, 000h
loc_1_54B0:
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        jne    SHORT loc_1_54C5
        add    eax, 4
        inc    esi
        db  03Dh, 03Ch, 095h, 000h, 000h
        jb     SHORT loc_1_54B0
        or     ecx, ecx
        je     SHORT loc_1_54D4
loc_1_54C5:
        mov    dword ptr [edx], esi
        mov    dword ptr [edx + 4], ecx
        mov    eax, 1
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_54D4:
        sub    eax, eax
        pop    esi
        ret
loc_1_54D8:
        cmp    esi, 20h
        jae    SHORT loc_1_54F2
        db  08Bh, 00Ch, 0B5h, 0BCh, 094h, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_1_54F2
loc_1_54E8:
        cmp    ecx, eax
        je     SHORT loc_1_5500
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    SHORT loc_1_54E8
loc_1_54F2:
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], 0
        sub    eax, eax
        pop    esi
        ret
loc_1_5500:
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], 1
        mov    eax, 1
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_1_5514:
        mov    eax, dword ptr [esp + 4]
        db  08Bh, 004h, 085h, 0BCh, 094h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_5532
        inc    ecx
        mov    ecx, dword ptr [esp + 8]
loc_1_5528:
        cmp    eax, ecx
        ja     SHORT loc_1_5532
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_5528
loc_1_5532:
        ret
        db  090h
sub_1_5534:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 10h]
        mov    edx, eax
        mov    ecx, dword ptr [esp + 0ch]
        sub    edx, dword ptr [ecx + 4]
        mov    dword ptr [ecx + 8], eax
        cmp    dword ptr [ecx + 0ch], eax
        je     SHORT loc_1_5552
        and    dword ptr [ecx + 58h], 0efffffffh
loc_1_5552:
        mov    al, byte ptr [ecx + 84h]
        cmp    al, 3
        jbe    SHORT loc_1_55AF
        cmp    al, 0ah
        je     SHORT loc_1_55AF
        mov    esi, dword ptr [ecx + 38h]
        mov    eax, esi
        sub    eax, ecx
        cmp    eax, 38h
        je     SHORT loc_1_55AF
        or     edx, edx
        je     SHORT loc_1_557D
loc_1_5570:
        mov    eax, dword ptr [esi + 20h]
        cmp    edx, eax
        jb     SHORT loc_1_557D
        sub    edx, eax
        mov    esi, dword ptr [esi]
        jne    SHORT loc_1_5570
loc_1_557D:
        mov    dword ptr [ecx + 28h], esi
        mov    eax, dword ptr [esi + 20h]
        sub    eax, edx
        mov    dword ptr [ecx + 34h], eax
        mov    eax, dword ptr [esi + 28h]
        mov    esi, dword ptr [esi + 24h]
        or     edx, edx
        je     SHORT loc_1_55A6
loc_1_5592:
        mov    edi, dword ptr [eax + 0ch]
        sub    edi, esi
        cmp    edx, edi
        jb     SHORT loc_1_55A6
        sub    edx, edi
        mov    esi, 0
        mov    eax, dword ptr [eax]
        jne    SHORT loc_1_5592
loc_1_55A6:
        mov    dword ptr [ecx + 2ch], eax
        lea    eax, [edx + esi]
        mov    dword ptr [ecx + 30h], eax
loc_1_55AF:
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_55B4:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 18h], 0
        db  0FFh, 005h, 06Ch, 096h, 000h, 000h
        mov    dword ptr [esp + 10h], 0
loc_1_55D1:
        mov    eax, dword ptr [esp + 10h]
        db  08Bh, 034h, 085h, 0BCh, 094h, 000h, 000h
        or     esi, esi
        je     NEAR PTR loc_1_59A6
loc_1_55E4:
        lea    eax, [esi + 84h]
        mov    dword ptr [esp + 1ch], eax
        mov    cl, byte ptr [eax]
        or     cl, cl
        je     NEAR PTR loc_1_599C
        test   byte ptr [esi + 86h], 1
        jne    NEAR PTR loc_1_599C
        mov    ax, word ptr [esi + 90h]
        or     ax, ax
        je     NEAR PTR loc_1_57EC
        dec    ax
        mov    word ptr [esi + 90h], ax
        jne    NEAR PTR loc_1_57EC
        mov    al, byte ptr [esi + 85h]
        inc    al
        mov    byte ptr [esi + 85h], al
        cmp    cl, 2
        jne    SHORT loc_1_5640
        db  08Bh, 01Dh, 04Ch, 096h, 000h, 000h
        jmp    SHORT loc_1_5646
        db  090h
loc_1_5640:
        db  08Bh, 01Dh, 054h, 096h, 000h, 000h
loc_1_5646:
        and    eax, 0ffh
        cmp    eax, ebx
        ja     NEAR PTR loc_1_590C
        mov    dword ptr [esi + 8ch], 0
        movzx  eax, word ptr [esi + 72h]
        add    eax, eax
        cmp    eax, 960h
        jl     SHORT loc_1_566F
        mov    eax, 960h
loc_1_566F:
        push   dword ptr [esi + 4]
        mov    word ptr [esi + 72h], ax
        push   esi
        call   sub_1_5534
        add    esp, 8
        lea    edi, [esi + 58h]
        mov    eax, dword ptr [edi]
        test   eax, 80000h
        jne    SHORT loc_1_56B8
        movzx  ecx, word ptr [esi + 70h]
        mov    edx, dword ptr [esi + 10h]
        mov    eax, dword ptr [esi + 1ch]
        sub    edx, eax
        sbb    ebp, ebp
        and    edx, ebp
        add    edx, eax
        shr    edx, 1
        lea    eax, [ecx*2]
        sub    edx, eax
        sbb    ebp, ebp
        not    ebp
        and    edx, ebp
        add    edx, eax
        mov    dword ptr [esi + 20h], edx
        mov    dword ptr [esi + 1ch], ecx
        jmp    SHORT loc_1_56BF
loc_1_56B8:
        or     eax, 400000h
        mov    dword ptr [edi], eax
loc_1_56BF:
        db  083h, 03Dh, 098h, 096h, 000h, 000h, 000h
        je     NEAR PTR loc_1_576E
        lea    eax, [ebx + 1]
        shr    eax, 1
        movzx  ecx, byte ptr [esi + 85h]
        cmp    eax, ecx
        jne    NEAR PTR loc_1_576E
        mov    ebp, dword ptr [edi]
        mov    edx, ebp
        and    edx, 1000h
        jne    SHORT loc_1_5700
        test   byte ptr [esi + 0efh], 2
        je     SHORT loc_1_576E
        mov    eax, dword ptr [esi + 0ch]
        sub    eax, dword ptr [esi + 4]
        cmp    eax, 8
        jle    SHORT loc_1_576E
loc_1_5700:
        mov    al, byte ptr [esi + 0d9h]
        lea    ecx, [eax + 1]
        mov    byte ptr [esi + 0d9h], cl
        cmp    al, 2
        jae    SHORT loc_1_5740
        or     edx, edx
        jne    SHORT loc_1_5729
        or     ebp, 1000h
        mov    dword ptr [edi], ebp
        inc    byte ptr [esi + 82h]
        or     dword ptr [esi + 5ch], 1
loc_1_5729:
        mov    al, byte ptr [esi + 85h]
        and    byte ptr [esi + 0efh], 0fdh
        dec    al
        mov    byte ptr [esi + 85h], al
        jmp    SHORT loc_1_576E
loc_1_5740:
        or     edx, edx
        je     SHORT loc_1_5760
        and    ebp, 0ffffefffh
        mov    dword ptr [edi], ebp
        mov    al, byte ptr [esi + 82h]
        dec    al
        mov    byte ptr [esi + 82h], al
        jne    SHORT loc_1_5760
        and    dword ptr [esi + 5ch], 0fffffffeh
loc_1_5760:
        or     byte ptr [esi + 0efh], 2
        mov    byte ptr [esi + 0d9h], 0
loc_1_576E:
        db  083h, 03Dh, 064h, 096h, 000h, 000h, 000h
        je     SHORT loc_1_5794
        movzx  eax, byte ptr [esi + 85h]
        inc    ebx
        shr    ebx, 1
        cmp    eax, ebx
        jne    SHORT loc_1_5794
        push   dword ptr [esi + 68h]
        push   dword ptr [esi + 64h]
        db  0FFh, 015h, 0B4h, 095h, 000h, 000h
        add    esp, 8
loc_1_5794:
        mov    eax, dword ptr [esp + 1ch]
        movzx  eax, byte ptr [eax]
        sub    eax, 2
        cmp    eax, 8
        ja     SHORT loc_1_57EC
        db  0FFh, 024h, 085h, 008h, 05Ah, 000h, 000h
        db  08Bh, 0FFh
loc_1_57AC:
        push   dword ptr [esp + 14h]
        push   esi
        call   sub_1_7E10
        add    esp, 8
        push   esi
        push   dword ptr [esp + 14h]
        call   sub_1_5514
        add    esp, 8
        mov    esi, eax
        jmp    NEAR PTR loc_1_599E
        db  08Dh, 049h, 000h
loc_1_57D0:
        or     dword ptr [edi], 10000h
loc_1_57D6:
        push   400h
        mov    dword ptr [esp + 1ch], 1
        push   esi
        call   sub_1_2388
        add    esp, 8
loc_1_57EC:
        mov    ax, word ptr [esi + 0dah]
        or     ax, ax
        je     SHORT loc_1_5820
        dec    ax
        mov    word ptr [esi + 0dah], ax
        jne    SHORT loc_1_5820
        or     dword ptr [esi + 58h], 400000h
        mov    dword ptr [esp + 18h], 1
        push   400h
        push   esi
        call   sub_1_2388
        add    esp, 8
loc_1_5820:
        mov    al, byte ptr [esi + 83h]
        or     al, al
        je     SHORT loc_1_5845
        dec    al
        mov    byte ptr [esi + 83h], al
        jne    SHORT loc_1_5845
        push   esi
        call   sub_1_4DD8
        add    esp, 4
        mov    dword ptr [esp + 18h], 1
loc_1_5845:
        mov    ax, word ptr [esi + 92h]
        or     ax, ax
        je     SHORT loc_1_5872
        dec    ax
        mov    word ptr [esi + 92h], ax
        jne    SHORT loc_1_5872
        mov    dword ptr [esp + 18h], 1
        push   200h
        push   esi
        call   sub_1_2388
        add    esp, 8
loc_1_5872:
        mov    eax, dword ptr [esp + 1ch]
        cmp    byte ptr [eax], 4
        jne    NEAR PTR loc_1_595B
        test   byte ptr [esi + 5bh], 2
        je     NEAR PTR loc_1_595B
        db  08Bh, 00Dh, 06Ch, 096h, 000h, 000h
        sub    ecx, dword ptr [esi + 98h]
        db  03Bh, 00Dh, 01Ch, 096h, 000h, 000h
        jbe    NEAR PTR loc_1_5954
        db  02Bh, 00Dh, 01Ch, 096h, 000h, 000h
        movzx  eax, byte ptr [esi + 87h]
        db  08Bh, 015h, 0FCh, 095h, 000h, 000h
        imul   edx, eax
        cmp    edx, ecx
        jae    NEAR PTR loc_1_595B
        db  039h, 005h, 054h, 096h, 000h, 000h
        jbe    SHORT loc_1_590C
        push   dword ptr [esp + 14h]
        push   esi
        call   sub_1_7FBC
        add    esp, 8
        push   esi
        push   dword ptr [esp + 14h]
        call   sub_1_5514
        add    esp, 8
        mov    esi, eax
        jmp    NEAR PTR loc_1_599E
loc_1_58E8:
        push   dword ptr [esp + 14h]
        push   1
        push   esi
        call   sub_1_6398
        add    esp, 0ch
        push   esi
        push   dword ptr [esp + 14h]
        call   sub_1_5514
        add    esp, 8
        mov    esi, eax
        jmp    NEAR PTR loc_1_599E
        db  090h
loc_1_590C:
        inc    dword ptr [esi + 74h]
        push   dword ptr [esp + 14h]
        push   20h
        push   esi
        call   sub_1_6398
        add    esp, 0ch
        push   esi
        call   sub_1_60F4
        add    esp, 4
        push   15h
        push   0
        push   esi
        call   sub_1_6FB8
        add    esp, 0ch
        push   dword ptr [esp + 20h]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        push   esi
        push   dword ptr [esp + 14h]
        call   sub_1_5514
        add    esp, 8
        mov    esi, eax
        jmp    SHORT loc_1_599E
        db  08Bh, 0FFh
loc_1_5954:
        mov    byte ptr [esi + 87h], 0
loc_1_595B:
        test   dword ptr [esi + 58h], 20010008h
        je     SHORT loc_1_599C
        mov    ecx, dword ptr [esi + 0b4h]
        mov    ax, word ptr [ecx + 1ah]
        or     ax, ax
        je     SHORT loc_1_599C
        dec    ax
        mov    word ptr [ecx + 1ah], ax
        jne    SHORT loc_1_599C
        push   dword ptr [esp + 14h]
        push   20h
        push   esi
        call   sub_1_6398
        add    esp, 0ch
        push   esi
        push   dword ptr [esp + 14h]
        call   sub_1_5514
        add    esp, 8
        mov    esi, eax
        jmp    SHORT loc_1_599E
        db  090h
loc_1_599C:
        mov    esi, dword ptr [esi]
loc_1_599E:
        or     esi, esi
        jne    NEAR PTR loc_1_55E4
loc_1_59A6:
        inc    dword ptr [esp + 10h]
        cmp    dword ptr [esp + 10h], 20h
        jb     NEAR PTR loc_1_55D1
        db  0A1h, 03Ch, 095h, 000h, 000h
        db  039h, 005h, 06Ch, 096h, 000h, 000h
        mov    esi, 0
        jne    SHORT loc_1_59D3
        mov    esi, 1
        db  083h, 005h, 03Ch, 095h, 000h, 000h, 00Ah
loc_1_59D3:
        push   0
        db  068h, 0B4h, 055h, 000h, 000h
        push   64h
        db  068h, 02Ch, 096h, 000h, 000h
        call   sub_1_90D8
        add    esp, 10h
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_1_59F5
        call   sub_1_22C8
loc_1_59F5:
        or     esi, esi
        je     SHORT loc_1_59FE
        call   sub_1_23C8
loc_1_59FE:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Bh, 0FFh, 0ACh, 057h, 000h, 000h, 0ACh, 057h, 000h, 000h, 0D6h, 057h, 000h, 000h, 0D0h, 057h
        db  000h, 000h, 0ECh, 057h, 000h, 000h, 0D6h, 057h, 000h, 000h, 0D0h, 057h, 000h, 000h, 0D0h, 057h
        db  000h, 000h, 0E8h, 058h, 000h, 000h
loc_1_5A2C:
        push   esi
        mov    eax, dword ptr [esp + 10h]
        mov    edx, dword ptr [eax]
        mov    eax, dword ptr [esp + 0ch]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [esp + 8]
        cmp    dword ptr [ecx + 64h], eax
        jne    SHORT loc_1_5A8A
        cmp    dword ptr [ecx + 68h], edx
        jne    SHORT loc_1_5A8A
        test   byte ptr [ecx + 0efh], 2
        je     SHORT loc_1_5A8A
        mov    eax, dword ptr [esp + 14h]
        mov    esi, dword ptr [eax]
        movzx  eax, byte ptr [ecx + 0ech]
        sub    esi, eax
        movzx  eax, word ptr [ecx + 80h]
        sub    eax, esi
        sbb    edx, edx
        and    eax, edx
        add    ax, si
        mov    word ptr [ecx + 70h], ax
        and    eax, 0ffffh
        cmp    dword ptr [ecx + 1ch], eax
        jae    SHORT loc_1_5A8A
        mov    dword ptr [ecx + 1ch], eax
        add    eax, eax
        cmp    dword ptr [ecx + 20h], eax
        jae    SHORT loc_1_5A8A
        mov    dword ptr [ecx + 20h], eax
loc_1_5A8A:
        pop    esi
        mov    eax, 1
        ret
        db  08Dh, 049h, 000h
loc_1_5A94:
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 68h], ecx
        jne    SHORT loc_1_5AA8
        sub    eax, eax
        ret
        db  08Bh, 0FFh
loc_1_5AA8:
        mov    eax, 1
        ret
        db  08Bh, 0FFh
sub_1_5AB0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 14h]
loc_1_5AC1:
        db  08Bh, 02Ch, 09Dh, 0BCh, 094h, 000h, 000h
        or     ebp, ebp
        je     SHORT loc_1_5B3B
loc_1_5ACC:
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 28h]
        push   ebp
        call   dword ptr [esp + 2ch]
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_5B34
        test   byte ptr [ebp + 86h], 1
        jne    SHORT loc_1_5B25
        inc    dword ptr [ebp + 74h]
        push   esi
        push   40h
        push   ebp
        call   sub_1_6398
        add    esp, 0ch
        push   ebp
        call   sub_1_60F4
        add    esp, 4
        cmp    byte ptr [ebp + 84h], 0ah
        je     SHORT loc_1_5B1B
        push   13h
        push   0
        push   ebp
        call   sub_1_6FB8
        add    esp, 0ch
loc_1_5B1B:
        push   edi
        push   ebp
        call   sub_1_63D8
        add    esp, 8
loc_1_5B25:
        push   ebp
        push   ebx
        call   sub_1_5514
        add    esp, 8
        mov    ebp, eax
        jmp    SHORT loc_1_5B37
        db  090h
loc_1_5B34:
        mov    ebp, dword ptr [ebp]
loc_1_5B37:
        or     ebp, ebp
        jne    SHORT loc_1_5ACC
loc_1_5B3B:
        inc    ebx
        cmp    ebx, 20h
        jb     NEAR PTR loc_1_5AC1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
sub_1_5B50:
        push   esi
        push   edi
        db  0A1h, 018h, 096h, 000h, 000h
        or     eax, eax
        mov    edx, dword ptr [esp + 0ch]
        je     SHORT loc_1_5B88
        cmp    dword ptr [eax + 68h], edx
        jne    SHORT loc_1_5B88
        mov    ecx, dword ptr [esp + 10h]
        cmp    dword ptr [eax + 64h], ecx
        mov    cx, word ptr [esp + 14h]
        jne    SHORT loc_1_5B8D
        cmp    word ptr [eax + 6ch], cx
        mov    si, word ptr [esp + 18h]
        jne    SHORT loc_1_5B92
        cmp    word ptr [eax + 6eh], si
        jne    SHORT loc_1_5B92
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_5B88:
        mov    cx, word ptr [esp + 14h]
loc_1_5B8D:
        mov    si, word ptr [esp + 18h]
loc_1_5B92:
        movzx  edi, byte ptr [esp + 12h]
        movzx  eax, byte ptr [esp + 13h]
        add    edi, eax
        movzx  eax, byte ptr [esp + 11h]
        add    edi, eax
        movzx  eax, byte ptr [esp + 10h]
        add    edi, eax
        and    edi, 1fh
        db  08Bh, 004h, 0BDh, 0BCh, 094h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_5BE5
loc_1_5BBA:
        mov    edi, dword ptr [esp + 10h]
        cmp    dword ptr [eax + 64h], edi
        jne    SHORT loc_1_5BD4
        cmp    dword ptr [eax + 68h], edx
        jne    SHORT loc_1_5BD4
        cmp    word ptr [eax + 6ch], cx
        jne    SHORT loc_1_5BD4
        cmp    word ptr [eax + 6eh], si
        je     SHORT loc_1_5BE0
loc_1_5BD4:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_5BBA
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_5BE0:
        db  0A3h, 018h, 096h, 000h, 000h
loc_1_5BE5:
        pop    edi
        pop    esi
        ret
sub_1_5BE8:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        movzx  eax, byte ptr [esi + 66h]
        movzx  edi, byte ptr [esi + 65h]
        add    edi, eax
        movzx  eax, byte ptr [esi + 67h]
        add    edi, eax
        movzx  eax, byte ptr [esi + 64h]
        add    edi, eax
        and    edi, 1fh
        mov    ax, word ptr [esi + 6eh]
        push   eax
        mov    ax, word ptr [esi + 6ch]
        push   eax
        push   dword ptr [esi + 64h]
        push   dword ptr [esi + 68h]
        call   sub_1_5B50
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_5C28
        sub    eax, eax
        pop    edi
        pop    esi
        ret
loc_1_5C28:
        db  08Dh, 00Ch, 0BDh, 0BCh, 094h, 000h, 000h
        cmp    dword ptr [ecx], 0
        je     SHORT loc_1_5C41
loc_1_5C34:
        mov    eax, dword ptr [ecx]
        cmp    esi, eax
        jbe    SHORT loc_1_5C41
        mov    ecx, eax
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_1_5C34
loc_1_5C41:
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi], eax
        mov    dword ptr [ecx], esi
        or     dword ptr [esi + 58h], 80000000h
        db  0FFh, 005h, 078h, 095h, 000h, 000h
        mov    eax, 1
        pop    edi
        pop    esi
        ret
sub_1_5C5C:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [ecx + 58h]
        test   edx, 80000000h
        je     SHORT loc_1_5CAF
        movzx  esi, byte ptr [ecx + 64h]
        movzx  eax, byte ptr [ecx + 65h]
        add    esi, eax
        movzx  eax, byte ptr [ecx + 66h]
        add    esi, eax
        movzx  eax, byte ptr [ecx + 67h]
        add    esi, eax
        and    esi, 1fh
        db  08Dh, 034h, 0B5h, 0BCh, 094h, 000h, 000h
loc_1_5C8C:
        mov    eax, dword ptr [esi]
        cmp    ecx, eax
        je     SHORT loc_1_5C9C
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_5C8C
        jmp    SHORT loc_1_5CAF
        db  08Bh, 0FFh
loc_1_5C9C:
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi], eax
        and    edx, 7fffffffh
        mov    dword ptr [ecx + 58h], edx
        db  0FFh, 00Dh, 078h, 095h, 000h, 000h
loc_1_5CAF:
        db  039h, 00Dh, 018h, 096h, 000h, 000h
        jne    SHORT loc_1_5CC1
        db  0C7h, 005h, 018h, 096h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_5CC1:
        pop    esi
        ret
        db  090h
sub_1_5CC4:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_90E4
        sub    edx, edx
        sub    esi, esi
        db  0B9h, 0BCh, 094h, 000h, 000h
loc_1_5CD9:
        mov    ebx, ecx
        mov    edi, dword ptr [ecx]
        or     edi, edi
        je     SHORT loc_1_5D1F
loc_1_5CE1:
        cmp    byte ptr [edi + 84h], 0ah
        jne    SHORT loc_1_5D17
        cmp    dword ptr [edi + 74h], 0
        jne    SHORT loc_1_5D17
        test   byte ptr [edi + 86h], 1
        jne    SHORT loc_1_5D17
        or     esi, esi
        je     SHORT loc_1_5D09
        mov    ebp, eax
        sub    ebp, dword ptr [edi + 98h]
        cmp    ebp, edx
        jbe    SHORT loc_1_5D17
loc_1_5D09:
        mov    esi, edi
        mov    dword ptr [esp + 10h], ebx
        mov    edx, eax
        sub    edx, dword ptr [edi + 98h]
loc_1_5D17:
        mov    ebx, edi
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    SHORT loc_1_5CE1
loc_1_5D1F:
        add    ecx, 4
        db  081h, 0F9h, 03Ch, 095h, 000h, 000h
        jb     SHORT loc_1_5CD9
        or     esi, esi
        je     SHORT loc_1_5D64
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax], ecx
        and    dword ptr [esi + 58h], 7fffffffh
        push   dword ptr [esi + 0f0h]
        db  0FFh, 015h, 0A4h, 095h, 000h, 000h
        add    esp, 4
        db  0FFh, 00Dh, 078h, 095h, 000h, 000h
        db  039h, 035h, 018h, 096h, 000h, 000h
        jne    SHORT loc_1_5D64
        db  0C7h, 005h, 018h, 096h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_5D64:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
sub_1_5D70:
        push   edi
        db  08Bh, 015h, 010h, 093h, 000h, 000h
        or     edx, edx
        je     SHORT loc_1_5D84
        mov    eax, dword ptr [edx]
        db  0A3h, 010h, 093h, 000h, 000h
        jmp    SHORT loc_1_5DC6
loc_1_5D84:
        db  0A1h, 01Ch, 093h, 000h, 000h
        db  03Bh, 005h, 014h, 093h, 000h, 000h
        ja     SHORT loc_1_5D9C
        call   sub_1_5CC4
        mov    edx, eax
        or     edx, edx
        jne    SHORT loc_1_5DC6
loc_1_5D9C:
        db  0A1h, 018h, 093h, 000h, 000h
        db  03Bh, 005h, 014h, 093h, 000h, 000h
        jbe    SHORT loc_1_5E00
        push   110h
        call   sub_1_90F6
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        jne    SHORT loc_1_5DC0
        mov    eax, edx
        pop    edi
        ret
loc_1_5DC0:
        db  0FFh, 005h, 014h, 093h, 000h, 000h
loc_1_5DC6:
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 44h
        rep stosd
        lea    eax, [edx + 38h]
        mov    dword ptr [eax], eax
        mov    dword ptr [edx + 3ch], eax
        mov    dword ptr [edx + 28h], 0
        db  0A1h, 06Ch, 096h, 000h, 000h
        mov    dword ptr [edx + 98h], eax
        mov    byte ptr [edx + 82h], 1
        mov    dword ptr [edx + 5ch], 1001h
        mov    eax, edx
        pop    edi
        ret
        db  08Dh, 049h, 000h
loc_1_5E00:
        sub    eax, eax
        pop    edi
        ret
sub_1_5E04:
        db  0A1h, 01Ch, 093h, 000h, 000h
        db  03Bh, 005h, 014h, 093h, 000h, 000h
        jae    SHORT loc_1_5E24
        db  0FFh, 00Dh, 014h, 093h, 000h, 000h
        push   dword ptr [esp + 4]
        call   sub_1_90FC
        add    esp, 4
        ret
loc_1_5E24:
        db  0A1h, 010h, 093h, 000h, 000h
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        db  089h, 00Dh, 010h, 093h, 000h, 000h
        ret
        db  08Bh, 0FFh
loc_1_5E38:
        ret
        db  08Dh, 049h, 000h
loc_1_5E3C:
        ret
        db  08Dh, 049h, 000h
sub_1_5E40:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esi + 14h]
        mov    ebx, dword ptr [esi + 18h]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_1_5E5E
        push   esi
        push   eax
        call   sub_1_64A8
        add    esp, 8
loc_1_5E5E:
        push   esi
        call   sub_1_90FC
        add    esp, 4
        push   0
        push   0
        push   ebx
        call   edi
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_5E78:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebp, ebp
        mov    esi, dword ptr [esp + 18h]
        mov    edi, dword ptr [esi + 14h]
        mov    ebx, dword ptr [esi + 18h]
        test   byte ptr [esi + 0dh], 1
        mov    word ptr [esi + 0eh], 1
        jne    SHORT loc_1_5EB0
        mov    eax, dword ptr [esi + 4]
        cmp    eax, ebp
        je     SHORT loc_1_5EA8
        push   esi
        push   eax
        call   sub_1_64A8
        add    esp, 8
loc_1_5EA8:
        and    byte ptr [esi + 0dh], 0fdh
        jmp    SHORT loc_1_5EB5
        db  08Bh, 0FFh
loc_1_5EB0:
        mov    ebp, 1
loc_1_5EB5:
        or     ebp, ebp
        mov    word ptr [esi + 0eh], 0
        je     SHORT loc_1_5ED4
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_1_5E40
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_5ED4:
        push   0
        push   0
        push   ebx
        call   edi
        add    esp, 0ch
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
sub_1_5EE8:
        db  08Bh, 00Dh, 0F4h, 096h, 000h, 000h
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], ecx
        db  0A3h, 0F4h, 096h, 000h, 000h
        ret
        db  08Bh, 0FFh
sub_1_5EFC:
        db  0A1h, 0F4h, 096h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_5F10
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 0F4h, 096h, 000h, 000h
        ret
        db  08Bh, 0FFh
loc_1_5F10:
        db  0A1h, 0F4h, 095h, 000h, 000h
        db  039h, 005h, 020h, 093h, 000h, 000h
        mov    eax, 0
        jbe    SHORT loc_1_5F2C
        push   1ch
        call   sub_1_90F6
        add    esp, 4
loc_1_5F2C:
        or     eax, eax
        je     SHORT loc_1_5F36
        db  0FFh, 005h, 0F4h, 095h, 000h, 000h
loc_1_5F36:
        ret
        db  090h
sub_1_5F38:
        mov    edx, dword ptr [esp + 4]
        mov    eax, edx
        and    eax, 0ffffffh
        db  039h, 005h, 028h, 096h, 000h, 000h
        jbe    SHORT loc_1_5F64
        db  08Bh, 00Dh, 024h, 093h, 000h, 000h
        mov    eax, dword ptr [ecx + eax*4]
        or     eax, eax
        je     SHORT loc_1_5F66
        shr    edx, 18h
        cmp    byte ptr [eax + 0ch], dl
        je     SHORT loc_1_5F66
        sub    eax, eax
        ret
        db  090h
loc_1_5F64:
        sub    eax, eax
loc_1_5F66:
        ret
        db  090h
sub_1_5F68:
        push   ebx
        push   esi
        push   edi
        push   ebp
loc_1_5F6C:
        db  08Bh, 015h, 024h, 093h, 000h, 000h
        db  08Bh, 01Dh, 028h, 096h, 000h, 000h
loc_1_5F78:
        sub    eax, eax
        db  08Bh, 035h, 088h, 096h, 000h, 000h
        cmp    ebx, eax
        je     SHORT loc_1_5F96
loc_1_5F84:
        cmp    ebx, esi
        jne    SHORT loc_1_5F8A
        sub    esi, esi
loc_1_5F8A:
        cmp    dword ptr [edx + esi*4], 0
        je     SHORT loc_1_5F96
        inc    esi
        inc    eax
        cmp    eax, ebx
        jb     SHORT loc_1_5F84
loc_1_5F96:
        cmp    eax, ebx
        db  089h, 015h, 024h, 093h, 000h, 000h
        db  089h, 01Dh, 028h, 096h, 000h, 000h
        jb     NEAR PTR loc_1_602C
        db  03Bh, 01Dh, 0E8h, 095h, 000h, 000h
        je     NEAR PTR loc_1_606C
        add    ebx, 10h
        db  0A1h, 0E8h, 095h, 000h, 000h
        sub    ebx, eax
        sbb    ecx, ecx
        and    ebx, ecx
        add    ebx, eax
        lea    eax, [ebx*4]
        push   eax
        call   sub_1_90F6
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        db  08Bh, 02Dh, 024h, 093h, 000h, 000h
        db  08Bh, 035h, 028h, 096h, 000h, 000h
        je     SHORT loc_1_6060
        lea    ecx, [ebx*4]
        mov    edi, edx
        sub    eax, eax
        shr    ecx, 2
        rep stosd
        lea    ecx, [esi*4]
        mov    edi, edx
        mov    esi, ebp
        shr    ecx, 2
        rep movsd
        db  089h, 015h, 024h, 093h, 000h, 000h
        db  089h, 01Dh, 028h, 096h, 000h, 000h
        cmp    ebp, eax
        je     NEAR PTR loc_1_5F78
        push   ebp
        call   sub_1_90FC
        add    esp, 4
        jmp    NEAR PTR loc_1_5F6C
        db  08Bh, 0FFh
loc_1_602C:
        db  089h, 015h, 024h, 093h, 000h, 000h
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [edx + esi*4], eax
        lea    ecx, [esi + 1]
        db  089h, 00Dh, 088h, 096h, 000h, 000h
        db  08Ah, 00Dh, 090h, 096h, 000h, 000h
        inc    cl
        db  088h, 00Dh, 090h, 096h, 000h, 000h
        mov    byte ptr [eax + 0ch], cl
        movzx  eax, cl
        shl    eax, 18h
        or     eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_6060:
        pop    ebp
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_606C:
        pop    ebp
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
sub_1_6078:
        mov    ecx, dword ptr [esp + 4]
        and    ecx, 0ffffffh
        db  0A1h, 024h, 093h, 000h, 000h
        mov    dword ptr [eax + ecx*4], 0
        ret
        db  090h
sub_1_6090:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 2afah
        je     SHORT loc_1_60B8
        cmp    eax, 2afbh
        je     SHORT loc_1_60C0
        cmp    eax, 2afch
        je     SHORT loc_1_60C8
        cmp    eax, 2afdh
        je     SHORT loc_1_60D0
        mov    eax, dword ptr [esp + 8]
        ret
        db  08Dh, 049h, 000h
loc_1_60B8:
        mov    eax, 1bh
        ret
        db  08Bh, 0FFh
loc_1_60C0:
        mov    eax, 1ch
        ret
        db  08Bh, 0FFh
loc_1_60C8:
        mov    eax, 1dh
        ret
        db  08Bh, 0FFh
loc_1_60D0:
        mov    eax, 1eh
        ret
        db  08Bh, 0FFh
sub_1_60D8:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0cch]
        or     ecx, ecx
        je     SHORT loc_1_60F0
        cmp    dword ptr [ecx], eax
        jne    SHORT loc_1_60F0
        mov    dword ptr [ecx], 0
loc_1_60F0:
        ret
        db  08Dh, 049h, 000h
sub_1_60F4:
        push   dword ptr [esp + 4]
        call   sub_1_60D8
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
sub_1_6104:
        sub    esp, 4
        push   esi
        sub    edx, edx
        mov    ecx, dword ptr [esp + 0ch]
        mov    esi, dword ptr [ecx + 0cch]
        cmp    esi, edx
        je     SHORT loc_1_6131
        mov    ax, word ptr [esi + 0eh]
        dec    ax
        mov    word ptr [esi + 0eh], ax
        jne    SHORT loc_1_6127
        mov    edx, dword ptr [esi + 1ch]
loc_1_6127:
        mov    dword ptr [ecx + 0cch], 0
loc_1_6131:
        or     edx, edx
        je     SHORT loc_1_613F
        push   dword ptr [esp + 4]
        push   esi
        call   edx
        add    esp, 8
loc_1_613F:
        pop    esi
        add    esp, 4
        ret
sub_1_6144:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        mov    bl, byte ptr [esi + 84h]
        or     bl, bl
        je     NEAR PTR loc_1_6393
        cmp    bl, 2
        je     SHORT loc_1_617C
        cmp    bl, 3
        je     SHORT loc_1_617C
        cmp    bl, 4
        je     SHORT loc_1_616E
        cmp    bl, 7
        jne    SHORT loc_1_6182
loc_1_616E:
        db  0FFh, 005h, 05Ch, 095h, 000h, 000h
        db  0FFh, 00Dh, 060h, 095h, 000h, 000h
        jmp    SHORT loc_1_6182
loc_1_617C:
        db  0FFh, 005h, 058h, 095h, 000h, 000h
loc_1_6182:
        push   esi
        mov    byte ptr [esi + 84h], 0
        call   sub_1_60D8
        add    esp, 4
        push   esi
        call   sub_1_5C5C
        add    esp, 4
        cmp    bl, 3
        jbe    SHORT loc_1_61B4
        cmp    bl, 9
        jae    SHORT loc_1_61B4
        test   byte ptr [esi + 5bh], 40h
        je     SHORT loc_1_61B4
        push   esi
        call   sub_1_81FC
        add    esp, 4
loc_1_61B4:
        lea    eax, [esi + 0e4h]
        push   eax
        db  0FFh, 015h, 0C4h, 095h, 000h, 000h
        add    esp, 4
        push   dword ptr [esi + 0f0h]
        db  0FFh, 015h, 0A4h, 095h, 000h, 000h
        add    esp, 4
        mov    al, byte ptr [esi + 0d8h]
        test   al, 80h
        je     SHORT loc_1_61E4
        mov    edi, 14h
        jmp    SHORT loc_1_621F
loc_1_61E4:
        test   al, 40h
        je     SHORT loc_1_61F0
        mov    edi, 13h
        jmp    SHORT loc_1_621F
        db  090h
loc_1_61F0:
        test   al, 20h
        je     SHORT loc_1_61F8
        push   15h
        jmp    SHORT loc_1_620F
loc_1_61F8:
        test   al, 10h
        je     SHORT loc_1_6204
        mov    edi, 0eh
        jmp    SHORT loc_1_621F
        db  090h
loc_1_6204:
        test   al, 8
        mov    edi, 0
        je     SHORT loc_1_621F
        push   1ch
loc_1_620F:
        push   dword ptr [esi + 88h]
        call   sub_1_6090
        add    esp, 8
        mov    edi, eax
loc_1_621F:
        mov    ebx, dword ptr [esi + 0b4h]
        or     ebx, ebx
        je     SHORT loc_1_623E
        push   0
        push   edi
        push   dword ptr [ebx + 0ch]
        call   dword ptr [ebx + 8]
        add    esp, 0ch
        push   ebx
        call   sub_1_5EE8
        add    esp, 4
loc_1_623E:
        mov    ebx, dword ptr [esi + 0f4h]
        or     ebx, ebx
        je     SHORT loc_1_625D
        push   0
        push   edi
        push   dword ptr [ebx + 0ch]
        call   dword ptr [ebx + 8]
        add    esp, 0ch
        push   ebx
        call   sub_1_5EE8
        add    esp, 4
loc_1_625D:
        lea    ebx, [esi + 38h]
        mov    eax, dword ptr [ebx]
        sub    eax, esi
        cmp    eax, 38h
        je     SHORT loc_1_62B8
loc_1_6269:
        mov    ebp, dword ptr [ebx]
        mov    eax, dword ptr [ebp]
        mov    dword ptr [ebx], eax
        mov    dword ptr [eax + 4], ebx
        mov    eax, dword ptr [ebp + 18h]
        dec    eax
        mov    dword ptr [ebp + 18h], eax
        or     eax, eax
        jg     SHORT loc_1_62AC
        mov    eax, dword ptr [ebp + 2ch]
        or     eax, eax
        je     SHORT loc_1_6292
        mov    dword ptr [eax], 0
        mov    dword ptr [ebp + 2ch], 0
loc_1_6292:
        push   0
        push   edi
        push   dword ptr [ebp + 0ch]
        call   dword ptr [ebp + 8]
        add    esp, 0ch
        push   ebp
        call   sub_1_7CD4
        add    esp, 4
        jmp    SHORT loc_1_62AF
        db  08Dh, 049h, 000h
loc_1_62AC:
        mov    dword ptr [ebp + 10h], edi
loc_1_62AF:
        mov    eax, dword ptr [ebx]
        sub    eax, esi
        cmp    eax, 38h
        jne    SHORT loc_1_6269
loc_1_62B8:
        cmp    dword ptr [esi + 0a0h], 0
        je     SHORT loc_1_62ED
loc_1_62C1:
        mov    ebx, dword ptr [esi + 0a0h]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi + 0a0h], eax
        push   0
        push   edi
        push   dword ptr [ebx + 8]
        call   dword ptr [ebx + 4]
        add    esp, 0ch
        push   ebx
        call   sub_1_408C
        add    esp, 4
        cmp    dword ptr [esi + 0a0h], 0
        jne    SHORT loc_1_62C1
loc_1_62ED:
        cmp    dword ptr [esi + 0f8h], 0
        je     SHORT loc_1_6322
loc_1_62F6:
        mov    ebx, dword ptr [esi + 0f8h]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi + 0f8h], eax
        push   0
        push   edi
        push   dword ptr [ebx + 8]
        call   dword ptr [ebx + 4]
        add    esp, 0ch
        push   ebx
        call   sub_1_408C
        add    esp, 4
        cmp    dword ptr [esi + 0f8h], 0
        jne    SHORT loc_1_62F6
loc_1_6322:
        mov    eax, dword ptr [esi + 0ach]
        or     eax, eax
        je     SHORT loc_1_6335
        push   eax
        call   sub_1_2A58
        add    esp, 4
loc_1_6335:
        mov    eax, dword ptr [esi + 0fch]
        or     eax, eax
        je     SHORT loc_1_6348
        push   eax
        call   sub_1_2A58
        add    esp, 4
loc_1_6348:
        cmp    dword ptr [esi + 9ch], 0
        je     SHORT loc_1_6381
loc_1_6351:
        mov    edi, dword ptr [esi + 9ch]
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 9ch], eax
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_636F
        push   eax
        call   sub_1_2A58
        add    esp, 4
loc_1_636F:
        push   edi
        call   sub_1_90FC
        add    esp, 4
        cmp    dword ptr [esi + 9ch], 0
        jne    SHORT loc_1_6351
loc_1_6381:
        push   esi
        call   sub_1_6104
        add    esp, 4
        push   esi
        call   sub_1_5E04
        add    esp, 4
loc_1_6393:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_6398:
        mov    eax, dword ptr [esp + 4]
        mov    cl, byte ptr [esp + 8]
        or     byte ptr [eax + 0d8h], cl
        mov    cl, byte ptr [eax + 86h]
        test   cl, 1
        jne    SHORT loc_1_63D7
        or     cl, 1
        mov    byte ptr [eax + 86h], cl
        inc    byte ptr [eax + 82h]
        or     dword ptr [eax + 5ch], 1
        cmp    dword ptr [eax + 74h], 0
        jne    SHORT loc_1_63D7
        push   dword ptr [esp + 0ch]
        push   eax
        call   sub_1_6144
        add    esp, 8
loc_1_63D7:
        ret
sub_1_63D8:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 74h]
        dec    ecx
        mov    dword ptr [eax + 74h], ecx
        jne    SHORT loc_1_6401
        mov    cl, byte ptr [eax + 86h]
        or     cl, cl
        je     SHORT loc_1_6401
        test   cl, 1
        je     SHORT loc_1_6401
        push   dword ptr [esp + 8]
        push   eax
        call   sub_1_6144
        add    esp, 8
loc_1_6401:
        ret
        db  08Bh, 0FFh
loc_1_6404:
        push   esi
        push   edi
        push   28h
        call   sub_1_90F6
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_1_64A0
        mov    edi, esi
        sub    eax, eax
        mov    ecx, 0ah
        rep stosd
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esi + 10h], eax
        push   esi
        call   sub_1_5F68
        add    esp, 4
        cmp    eax, -1
        je     SHORT loc_1_648C
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [ecx], eax
        mov    word ptr [esi + 0eh], 0
        mov    byte ptr [esi + 0dh], 0
        db  083h, 03Dh, 024h, 096h, 000h, 000h, 001h
        cmc
        sbb    eax, eax
        and    eax, 2000h
        or     eax, 20000h
        mov    dword ptr [esi + 20h], eax
        db  0A1h, 068h, 096h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_6478
        mov    dword ptr [esi + 24h], eax
        mov    byte ptr [esi + 0dh], 4
        jmp    SHORT loc_1_647F
loc_1_6478:
        mov    dword ptr [esi + 24h], 2000h
loc_1_647F:
        sub    eax, eax
        db  0C7h, 046h, 01Ch, 038h, 05Eh, 000h, 000h
        pop    edi
        pop    esi
        ret
        db  090h
loc_1_648C:
        push   esi
        call   sub_1_90FC
        add    esp, 4
        mov    eax, 1
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_64A0:
        pop    edi
        mov    eax, 1
        pop    esi
        ret
sub_1_64A8:
        mov    edx, dword ptr [esp + 4]
        add    edx, 44h
        cmp    dword ptr [edx + 8], 0
        je     SHORT loc_1_64D9
        mov    ecx, dword ptr [esp + 8]
loc_1_64B9:
        mov    eax, dword ptr [edx + 8]
        cmp    ecx, eax
        je     SHORT loc_1_64CC
        mov    edx, eax
        cmp    dword ptr [edx + 8], 0
        jne    SHORT loc_1_64B9
        ret
        db  08Dh, 049h, 000h
loc_1_64CC:
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [ecx + 4], 0
loc_1_64D9:
        ret
        db  08Bh, 0FFh
loc_1_64DC:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 14h]
        mov    ebx, dword ptr [edi]
        push   ebx
        call   sub_1_5F38
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_1_65A4
        push   ebx
        call   sub_1_6078
        add    esp, 4
        mov    cl, byte ptr [esi + 0dh]
        or     cl, 1
        mov    byte ptr [esi + 0dh], cl
        cmp    word ptr [esi + 0eh], 0
        je     SHORT loc_1_6584
        mov    edx, dword ptr [esi + 14h]
        mov    ebx, dword ptr [esi + 18h]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esi + 14h], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esi + 18h], eax
        db  0C7h, 046h, 01Ch, 040h, 05Eh, 000h, 000h
        test   cl, 2
        je     SHORT loc_1_653C
        push   0
        push   20h
        push   ebx
        call   edx
        add    esp, 0ch
loc_1_653C:
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     SHORT loc_1_6578
        cmp    byte ptr [eax + 84h], 0
        je     SHORT loc_1_6578
        or     dword ptr [eax + 58h], 40000000h
        test   byte ptr [eax + 86h], 1
        jne    SHORT loc_1_656A
        push   dword ptr [esp + 0ch]
        push   40h
        push   eax
        call   sub_1_6398
        add    esp, 0ch
loc_1_656A:
        pop    edi
        mov    eax, 0ffh
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_6578:
        pop    edi
        mov    eax, 0ffh
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_6584:
        db  0C7h, 046h, 014h, 03Ch, 05Eh, 000h, 000h
        push   dword ptr [esp + 0ch]
        push   esi
        call   sub_1_5E40
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_65A4:
        pop    edi
        mov    eax, 0fh
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_65B0:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [eax]
        push   dword ptr [esp + 10h]
        call   sub_1_01D4
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_65D4
        mov    eax, 6
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_65D4:
        push   esi
        call   sub_1_5F38
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_6604
        cmp    dword ptr [eax + 4], 0
        je     SHORT loc_1_65F0
        mov    eax, 10h
        pop    edi
        pop    esi
        ret
        db  090h
loc_1_65F0:
        mov    dword ptr [eax + 4], edi
        mov    ecx, dword ptr [edi + 4ch]
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [edi + 4ch], eax
        sub    eax, eax
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_6604:
        pop    edi
        mov    eax, 0fh
        pop    esi
        ret
loc_1_660C:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   dword ptr [esi]
        call   sub_1_5F38
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_6670
        mov    ecx, dword ptr [eax + 4]
        or     ecx, ecx
        je     SHORT loc_1_6668
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_6660
        cmp    word ptr [eax + 0eh], 0
        jne    SHORT loc_1_6640
        push   eax
        push   ecx
        call   sub_1_64A8
        add    esp, 8
        sub    eax, eax
        pop    esi
        ret
loc_1_6640:
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [eax + 14h], ecx
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [eax + 18h], ecx
        db  0C7h, 040h, 01Ch, 078h, 05Eh, 000h, 000h
        or     byte ptr [eax + 0dh], 2
        mov    eax, 0ffh
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_6660:
        pop    esi
        mov    eax, 12h
        ret
        db  090h
loc_1_6668:
        pop    esi
        mov    eax, 11h
        ret
        db  090h
loc_1_6670:
        pop    esi
        mov    eax, 0fh
        ret
        db  090h
sub_1_6678:
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   esi
        db  0FFh, 015h, 0B8h, 095h, 000h, 000h
        add    esp, 4
        mov    ecx, dword ptr [esp + 8]
        or     ecx, ecx
        je     SHORT loc_1_66C4
        mov    eax, dword ptr [ecx + 0ch]
        or     eax, eax
        je     SHORT loc_1_66C4
        push   esi
        push   dword ptr [ecx + 8]
        push   eax
        db  0FFh, 015h, 0C0h, 095h, 000h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_66C0
        cmp    eax, 2afeh
        jne    SHORT loc_1_66B8
        mov    eax, 1
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_66B8:
        pop    esi
        mov    eax, 0bh
        ret
        db  090h
loc_1_66C0:
        sub    eax, eax
        pop    esi
        ret
loc_1_66C4:
        sub    eax, eax
        pop    esi
        ret
sub_1_66C8:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    cl, byte ptr [esi + 0dh]
        test   cl, 3
        je     SHORT loc_1_66E0
        mov    eax, 0fh
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_66E0:
        cmp    dword ptr [esi], 0
        jne    NEAR PTR loc_1_67A4
        mov    edi, dword ptr [esi + 4]
        or     edi, edi
        je     NEAR PTR loc_1_679C
        mov    eax, dword ptr [edi + 2ch]
        mov    edx, dword ptr [esp + 10h]
        mov    dword ptr [edx + 68h], eax
        mov    ax, word ptr [edi + 30h]
        mov    word ptr [edx + 6eh], ax
        mov    eax, dword ptr [edi + 7ch]
        mov    dword ptr [edx + 0dch], eax
        mov    edi, dword ptr [edi + 80h]
        mov    dword ptr [edx + 0e0h], edi
        or     eax, eax
        db  0C7h, 042h, 060h, 0D4h, 042h, 000h, 000h
        je     SHORT loc_1_672D
        db  0C7h, 042h, 060h, 020h, 046h, 000h, 000h
loc_1_672D:
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [edx + 0b8h], eax
        mov    edi, dword ptr [esi + 20h]
        or     edi, dword ptr [edx + 58h]
        mov    dword ptr [edx + 58h], edi
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [edx + 94h], eax
        mov    dword ptr [edx + 44h], eax
        test   cl, 4
        je     SHORT loc_1_6756
        or     edi, 1
        mov    dword ptr [edx + 58h], edi
loc_1_6756:
        mov    ecx, dword ptr [edx + 58h]
        test   ecx, 2000000h
        je     SHORT loc_1_6773
        db  0A1h, 06Ch, 096h, 000h, 000h
        mov    dword ptr [edx + 98h], eax
        mov    byte ptr [edx + 87h], 0
loc_1_6773:
        mov    eax, dword ptr [esp + 14h]
        or     eax, eax
        je     SHORT loc_1_6787
        cmp    dword ptr [eax + 0ch], 0
        je     SHORT loc_1_6787
        or     ecx, 2
        mov    dword ptr [edx + 58h], ecx
loc_1_6787:
        add    edx, 0e4h
        push   edx
        push   eax
        call   sub_1_6678
        add    esp, 8
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_679C:
        pop    edi
        mov    eax, 11h
        pop    esi
        ret
loc_1_67A4:
        pop    edi
        mov    eax, 12h
        pop    esi
        ret
loc_1_67AC:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 24h]
        mov    esi, dword ptr [ebp]
        cmp    dword ptr [esp + 2ch], 0
        je     NEAR PTR loc_1_6A74
        mov    eax, dword ptr [esp + 2ch]
        mov    ecx, dword ptr [eax + 14h]
        or     ecx, ecx
        je     NEAR PTR loc_1_6A74
        lea    eax, [esp + 16h]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        push   ecx
        call   sub_1_0580
        add    esp, 0ch
        or     al, al
        je     NEAR PTR loc_1_6A74
        push   dword ptr [esp + 18h]
        db  0FFh, 015h, 0A8h, 095h, 000h, 000h
        add    esp, 4
        mov    byte ptr [esp + 13h], al
        cmp    al, 0ffh
        je     NEAR PTR loc_1_6A64
        test   al, 1
        jne    NEAR PTR loc_1_6A64
        cmp    word ptr [esp + 16h], 0
        je     NEAR PTR loc_1_6A64
        call   sub_1_5EFC
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_6834
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_6834:
        call   sub_1_5D70
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_6858
        push   edi
        call   sub_1_5EE8
        add    esp, 4
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
loc_1_6858:
        mov    eax, dword ptr [esp + 28h]
        or     eax, eax
        je     SHORT loc_1_688C
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_1_688C
        mov    ecx, 64h
        sub    edx, edx
        div    ecx
        inc    eax
        cmp    eax, 0ffffh
        jbe    SHORT loc_1_6884
        mov    eax, 0ffffh
        mov    word ptr [edi + 1ah], ax
        jmp    SHORT loc_1_6892
        db  08Bh, 0FFh
loc_1_6884:
        inc    eax
        mov    word ptr [edi + 1ah], ax
        jmp    SHORT loc_1_6892
        db  090h
loc_1_688C:
        mov    word ptr [edi + 1ah], 0
loc_1_6892:
        mov    eax, dword ptr [esp + 30h]
        mov    word ptr [edi + 18h], 0
        mov    dword ptr [edi + 14h], eax
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 0ch], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [ebx + 64h], eax
        mov    ax, word ptr [esp + 16h]
        mov    word ptr [ebx + 6ch], ax
        push   esi
        call   sub_1_5F38
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_1_6A40
        push   dword ptr [esp + 2ch]
        push   ebx
        push   esi
        call   sub_1_66C8
        add    esp, 0ch
        mov    ebp, eax
        or     ebp, ebp
        jne    NEAR PTR loc_1_6A45
        or     dword ptr [ebx + 58h], 8
        lea    eax, [ebx + 0e4h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esp + 1bh]
        push   eax
        lea    eax, [ebx + 0f0h]
        push   eax
        push   dword ptr [ebx + 68h]
        push   dword ptr [esp + 2ch]
        db  0FFh, 015h, 0A0h, 095h, 000h, 000h
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_1_6920
        mov    ebp, 1ch
        jmp    NEAR PTR loc_1_6A45
loc_1_6920:
        mov    dword ptr [esi], ebx
        inc    word ptr [esi + 0eh]
        mov    dword ptr [ebx + 0cch], esi
        cmp    dword ptr [ebx + 68h], 0
        jne    SHORT loc_1_6935
        mov    dword ptr [ebx + 68h], eax
loc_1_6935:
        db  0A1h, 000h, 096h, 000h, 000h
        sub    word ptr [esp + 14h], 14h
        or     eax, eax
        jne    SHORT loc_1_6978
        test   byte ptr [esp + 13h], 10h
        je     SHORT loc_1_6974
        lea    edx, [ebx + 70h]
        mov    cx, word ptr [esp + 14h]
        sub    cx, 218h
        sbb    ax, ax
        and    cx, ax
        movzx  ax, byte ptr [ebx + 0ech]
        add    cx, 218h
        sub    cx, ax
        mov    word ptr [edx], cx
        jmp    SHORT loc_1_6995
        db  090h
loc_1_6974:
        or     eax, eax
        je     SHORT loc_1_697F
loc_1_6978:
        mov    byte ptr [ebx + 0efh], 2
loc_1_697F:
        lea    edx, [ebx + 70h]
        mov    ax, word ptr [esp + 14h]
        movzx  cx, byte ptr [ebx + 0ech]
        sub    ax, cx
        mov    word ptr [edx], ax
loc_1_6995:
        push   ebx
        mov    ax, word ptr [edx]
        mov    word ptr [ebx + 80h], ax
        call   sub_1_8494
        add    esp, 4
        mov    dword ptr [ebx + 74h], 1
        mov    byte ptr [ebx + 84h], 2
        db  0FFh, 005h, 050h, 095h, 000h, 000h
        mov    dword ptr [ebx + 0b4h], edi
        push   ebx
        call   sub_1_5BE8
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_6A04
        mov    dword ptr [ebx + 0b4h], 0
        dec    dword ptr [ebx + 74h]
        push   dword ptr [esp + 1ch]
        push   40h
        push   ebx
        call   sub_1_6398
        add    esp, 0ch
        push   edi
        call   sub_1_5EE8
        add    esp, 4
        mov    eax, 2
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
loc_1_6A04:
        test   byte ptr [ebx + 86h], 1
        jne    SHORT loc_1_6A23
        cmp    byte ptr [ebx + 84h], 2
        jne    SHORT loc_1_6A23
        push   dword ptr [esp + 1ch]
        push   ebx
        call   sub_1_7E10
        add    esp, 8
loc_1_6A23:
        push   dword ptr [esp + 1ch]
        push   ebx
        call   sub_1_63D8
        add    esp, 8
        mov    eax, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
loc_1_6A40:
        mov    ebp, 0fh
loc_1_6A45:
        push   edi
        call   sub_1_5EE8
        add    esp, 4
        push   ebx
        call   sub_1_5E04
        add    esp, 4
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
loc_1_6A64:
        pop    ebp
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
loc_1_6A74:
        pop    ebp
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
loc_1_6A84:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 1ch]
        mov    esi, dword ptr [edi]
        cmp    dword ptr [esp + 24h], 0
        je     SHORT loc_1_6AF4
        mov    eax, dword ptr [esp + 24h]
        mov    ecx, dword ptr [eax + 14h]
        or     ecx, ecx
        je     SHORT loc_1_6AF4
        lea    eax, [esp + 12h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   ecx
        call   sub_1_0580
        add    esp, 0ch
        or     al, al
        jne    SHORT loc_1_6AC8
        mov    eax, 3
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  090h
loc_1_6AC8:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_1_6B03
        push   dword ptr [esp + 14h]
        db  0FFh, 015h, 0A8h, 095h, 000h, 000h
        add    esp, 4
        cmp    al, 0ffh
        je     SHORT loc_1_6AE4
        test   al, 1
        je     SHORT loc_1_6B03
loc_1_6AE4:
        pop    ebp
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_6AF4:
        mov    dword ptr [esp + 14h], 0
        mov    word ptr [esp + 12h], 0
loc_1_6B03:
        call   sub_1_5EFC
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_1_6B1C
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  090h
loc_1_6B1C:
        call   sub_1_5D70
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_6B40
        push   ebp
        call   sub_1_5EE8
        add    esp, 4
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_6B40:
        mov    ax, word ptr [esp + 20h]
        mov    word ptr [ebp + 18h], ax
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [ebp + 14h], eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ebp + 8], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ebp + 0ch], eax
        mov    dword ptr [ebx + 0b4h], ebp
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [ebx + 64h], eax
        mov    ax, word ptr [esp + 12h]
        mov    word ptr [ebx + 6ch], ax
        mov    byte ptr [ebx + 84h], 1
        push   esi
        call   sub_1_5F38
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_6BCC
        push   dword ptr [esp + 24h]
        push   ebx
        push   esi
        call   sub_1_66C8
        add    esp, 0ch
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_6BB8
        mov    dword ptr [esi], ebx
        mov    dword ptr [ebx + 0cch], esi
        inc    word ptr [esi + 0eh]
        mov    edi, 0ffh
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  090h
loc_1_6BB8:
        push   ebx
        call   sub_1_5E04
        add    esp, 4
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  090h
loc_1_6BCC:
        push   ebx
        call   sub_1_5E04
        add    esp, 4
        mov    edi, 0fh
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_6BE4:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        lea    eax, [esi + 0e4h]
        push   eax
        lea    eax, [esp + 0ah]
        push   eax
        lea    eax, [esp + 0dh]
        push   eax
        lea    eax, [esi + 0f0h]
        push   eax
        push   dword ptr [esi + 68h]
        push   dword ptr [esi + 64h]
        db  0FFh, 015h, 0A0h, 095h, 000h, 000h
        add    esp, 18h
        sub    word ptr [esp + 6], 14h
        db  0A1h, 000h, 096h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_1_6C64
        test   byte ptr [esp + 5], 10h
        je     SHORT loc_1_6C60
        mov    ax, word ptr [esp + 6]
        sub    ax, 218h
        sbb    cx, cx
        and    ax, cx
        add    ax, 218h
        movzx  ecx, ax
        movzx  eax, byte ptr [esi + 0ech]
        sub    ecx, eax
        movzx  eax, word ptr [esi + 80h]
        cmp    ecx, eax
        jl     SHORT loc_1_6C55
        mov    ecx, eax
loc_1_6C55:
        mov    word ptr [esi + 70h], cx
        pop    esi
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_6C60:
        or     eax, eax
        je     SHORT loc_1_6C6B
loc_1_6C64:
        mov    byte ptr [esi + 0efh], 2
loc_1_6C6B:
        movzx  ax, byte ptr [esi + 0ech]
        sub    word ptr [esp + 6], ax
        mov    ax, word ptr [esi + 80h]
        mov    cx, word ptr [esp + 6]
        sub    ax, cx
        sbb    dx, dx
        and    ax, dx
        add    ax, cx
        mov    word ptr [esi + 70h], ax
        pop    esi
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
sub_1_6C9C:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        push   esi
        call   sub_1_6BE4
        add    esp, 4
        push   esi
        call   sub_1_8494
        add    esp, 4
        push   esi
        call   sub_1_2024
        add    esp, 4
        mov    edi, dword ptr [esp + 10h]
        push   edi
        push   esi
        call   sub_1_7E10
        add    esp, 8
        push   edi
        push   esi
        call   sub_1_63D8
        add    esp, 8
        pop    edi
        pop    esi
        ret
loc_1_6CD8:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 24h]
        mov    esi, dword ptr [edi]
        call   sub_1_5EFC
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_6D00
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
loc_1_6D00:
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [ebx + 14h], eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ebx + 8], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ebx + 0ch], eax
        push   esi
        call   sub_1_5F38
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_6DB4
        mov    ebp, dword ptr [eax]
        or     ebp, ebp
        je     NEAR PTR loc_1_6DB4
        test   byte ptr [ebp + 86h], 1
        jne    SHORT loc_1_6DB4
        cmp    byte ptr [ebp + 84h], 3
        jne    SHORT loc_1_6DB4
        test   byte ptr [ebp + 58h], 4
        jne    SHORT loc_1_6DB4
        cmp    dword ptr [ebp + 0b4h], 0
        jne    SHORT loc_1_6DB4
        mov    eax, dword ptr [esp + 28h]
        or     eax, eax
        je     SHORT loc_1_6D8A
        cmp    dword ptr [eax + 0ch], 0
        je     SHORT loc_1_6D8A
        lea    ecx, [esp + 14h]
        push   ecx
        push   eax
        call   sub_1_6678
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_6DB4
        lea    edi, [ebp + 0e4h]
        push   edi
        db  0FFh, 015h, 0C4h, 095h, 000h, 000h
        add    esp, 4
        lea    esi, [esp + 14h]
        movsd
        movsd
        movsd
        or     dword ptr [ebp + 58h], 2
loc_1_6D8A:
        mov    dword ptr [ebp + 0b4h], ebx
        or     dword ptr [ebp + 58h], 4
        inc    dword ptr [ebp + 74h]
        push   dword ptr [esp + 10h]
        push   ebp
        call   sub_1_6C9C
        add    esp, 8
        mov    eax, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
loc_1_6DB4:
        push   ebx
        mov    esi, 0fh
        call   sub_1_5EE8
        add    esp, 4
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_6DCC:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 18h]
        push   dword ptr [ebp]
        call   sub_1_5F38
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_6F74
        mov    esi, dword ptr [eax]
        or     esi, esi
        je     NEAR PTR loc_1_6F74
        mov    bx, word ptr [esp + 20h]
        test   bl, 2
        je     SHORT loc_1_6E40
        cmp    byte ptr [esi + 84h], 0
        je     SHORT loc_1_6E30
        test   byte ptr [esi + 86h], 1
        jne    SHORT loc_1_6E26
        or     dword ptr [esi + 58h], 40000000h
        push   dword ptr [esp + 10h]
        push   40h
        push   esi
        call   sub_1_6398
        add    esp, 0ch
loc_1_6E26:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_6E30:
        pop    ebp
        mov    eax, 19h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_6E40:
        test   byte ptr [esi + 86h], 1
        je     SHORT loc_1_6E58
        mov    eax, 0fh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_6E58:
        call   sub_1_5EFC
        mov    edi, eax
        or     edi, edi
        je     NEAR PTR loc_1_6F64
        mov    word ptr [edi + 18h], 0
        mov    dword ptr [edi + 14h], 0
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 0ch], eax
        test   bl, 1
        jne    NEAR PTR loc_1_6F30
        mov    eax, dword ptr [esp + 1ch]
        or     eax, eax
        je     SHORT loc_1_6EBC
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_1_6EBC
        mov    ecx, 64h
        sub    edx, edx
        div    ecx
        inc    eax
        cmp    eax, 0ffffh
        jbe    SHORT loc_1_6EB4
        mov    eax, 0ffffh
        mov    word ptr [edi + 1ah], ax
        jmp    SHORT loc_1_6EC2
        db  090h
loc_1_6EB4:
        inc    eax
        mov    word ptr [edi + 1ah], ax
        jmp    SHORT loc_1_6EC2
        db  090h
loc_1_6EBC:
        mov    word ptr [edi + 1ah], 0
loc_1_6EC2:
        mov    al, byte ptr [esi + 84h]
        cmp    al, 4
        jne    SHORT loc_1_6EE0
        mov    byte ptr [esi + 84h], 5
        inc    byte ptr [esi + 82h]
        or     dword ptr [esi + 5ch], 1
        jmp    SHORT loc_1_6EEB
        db  090h
loc_1_6EE0:
        cmp    al, 7
        jne    SHORT loc_1_6F18
        mov    byte ptr [esi + 84h], 9
loc_1_6EEB:
        push   esi
        db  0FFh, 00Dh, 060h, 095h, 000h, 000h
        mov    dword ptr [esi + 0b4h], edi
        or     dword ptr [esi + 58h], 10000h
        inc    dword ptr [esi + 74h]
        call   sub_1_8824
        add    esp, 4
        mov    eax, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
loc_1_6F18:
        push   edi
        call   sub_1_5EE8
        add    esp, 4
        mov    eax, 19h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_6F30:
        mov    word ptr [edi + 1ah], 0
        cmp    dword ptr [esi + 0f4h], 0
        jne    SHORT loc_1_6F54
        mov    dword ptr [esi + 0f4h], edi
        mov    eax, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_6F54:
        pop    ebp
        mov    eax, 19h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_6F64:
        pop    ebp
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_6F74:
        pop    ebp
        mov    eax, 0fh
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
sub_1_6F84:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 0a8h], 0
        jne    SHORT loc_1_6FB4
        cmp    dword ptr [eax + 100h], 0
        jne    SHORT loc_1_6FB4
        cmp    dword ptr [eax + 0a0h], 0
        jne    SHORT loc_1_6FB4
        cmp    dword ptr [eax + 0f8h], 0
        jne    SHORT loc_1_6FB4
        mov    eax, 1
        ret
        db  08Bh, 0FFh
loc_1_6FB4:
        sub    eax, eax
        ret
        db  090h
sub_1_6FB8:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 14h]
        cmp    byte ptr [ebx + 84h], 3
        jbe    NEAR PTR loc_1_709D
        test   byte ptr [ebx + 58h], 10h
        jne    NEAR PTR loc_1_709D
        mov    esi, dword ptr [esp + 1ch]
        cmp    esi, 16h
        jne    SHORT loc_1_6FFC
        push   ebx
        call   sub_1_6F84
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_6FFC
        or     dword ptr [ebx + 58h], 100000h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_6FFC:
        mov    eax, dword ptr [ebx + 58h]
        or     eax, 10h
        mov    dword ptr [ebx + 58h], eax
        and    eax, 0ffefffffh
        mov    dword ptr [ebx + 58h], eax
        mov    edi, dword ptr [ebx + 0f4h]
        or     edi, edi
        je     SHORT loc_1_703C
        mov    dword ptr [ebx + 0f4h], 0
        push   0
        push   esi
        push   dword ptr [edi + 0ch]
        call   dword ptr [edi + 8]
        add    esp, 0ch
        push   edi
        call   sub_1_5EE8
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_703C:
        mov    edx, dword ptr [ebx + 0b8h]
        mov    eax, dword ptr [ebx + 0cch]
        or     eax, eax
        je     SHORT loc_1_709D
        mov    edi, dword ptr [eax + 4]
        or     edi, edi
        je     SHORT loc_1_709D
        mov    ecx, dword ptr [edi + 6ch]
        mov    ebx, dword ptr [edi + 70h]
        or     ecx, ecx
        je     SHORT loc_1_709D
        inc    word ptr [edi + 38h]
        mov    eax, dword ptr [esp + 18h]
        or     eax, eax
        je     SHORT loc_1_7074
        movzx  ebp, byte ptr [eax + 8]
        mov    eax, dword ptr [eax]
        jmp    SHORT loc_1_7078
        db  08Dh, 049h, 000h
loc_1_7074:
        sub    ebp, ebp
        sub    eax, eax
loc_1_7078:
        sub    esi, 16h
        cmp    esi, 1
        sbb    esi, esi
        and    esi, 2
        add    esi, 2
        push   esi
        push   eax
        push   ebp
        push   0
        push   0
        push   edx
        push   ebx
        call   ecx
        add    esp, 1ch
        push   edi
        call   sub_1_120C
        add    esp, 4
loc_1_709D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
sub_1_70A4:
        push   esi
        cmp    dword ptr [esp + 10h], 0
        mov    esi, dword ptr [esp + 8]
        je     SHORT loc_1_7120
        push   esi
        call   sub_1_6F84
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_70FC
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_1_70D8
        call   sub_1_90E4
        mov    dword ptr [esi + 98h], eax
        mov    byte ptr [esi + 84h], 0ah
        jmp    SHORT loc_1_70E7
loc_1_70D8:
        push   dword ptr [esp + 14h]
        push   0
        push   esi
        call   sub_1_6398
        add    esp, 0ch
loc_1_70E7:
        push   esi
        call   sub_1_60F4
        add    esp, 4
        push   16h
        push   0
        push   esi
        call   sub_1_6FB8
        jmp    SHORT loc_1_7166
loc_1_70FC:
        cmp    dword ptr [esp + 0ch], 1
        cmc
        sbb    eax, eax
        and    eax, 200000h
        or     eax, 1000000h
        or     dword ptr [esi + 58h], eax
        push   dword ptr [esp + 14h]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        pop    esi
        ret
loc_1_7120:
        mov    eax, dword ptr [esi + 58h]
        test   al, 10h
        je     SHORT loc_1_719C
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_1_7144
        call   sub_1_90E4
        mov    dword ptr [esi + 98h], eax
        mov    byte ptr [esi + 84h], 0ah
        jmp    SHORT loc_1_7153
        db  08Bh, 0FFh
loc_1_7144:
        push   dword ptr [esp + 14h]
        push   0
        push   esi
        call   sub_1_6398
        add    esp, 0ch
loc_1_7153:
        push   esi
        call   sub_1_60F4
        add    esp, 4
        push   0
        push   0
        push   esi
        call   sub_1_24B0
loc_1_7166:
        add    esp, 0ch
        test   byte ptr [esi + 86h], 1
        jne    SHORT loc_1_718B
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_1_718B
        mov    word ptr [esi + 90h], 960h
        push   esi
        call   sub_1_6104
        add    esp, 4
loc_1_718B:
        push   dword ptr [esp + 14h]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_719C:
        cmp    dword ptr [esp + 0ch], 1
        cmc
        sbb    ecx, ecx
        and    ecx, 200000h
        or     ecx, eax
        or     ecx, 1000000h
        mov    dword ptr [esi + 58h], ecx
        push   0
        push   0
        push   esi
        call   sub_1_24B0
        add    esp, 0ch
        push   dword ptr [esp + 14h]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_71D4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0e0h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        cmp    eax, 1
        je     SHORT loc_1_7214
        cmp    eax, 2
        je     SHORT loc_1_7220
        cmp    eax, 3
        je     NEAR PTR loc_1_7290
        cmp    eax, 4
        je     NEAR PTR loc_1_735C
        cmp    eax, 5
        je     NEAR PTR loc_1_7478
        mov    eax, 1fh
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_7214:
        pop    edi
        mov    eax, 1fh
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_7220:
        mov    dword ptr [ebp - 0e0h], 100h
        mov    dword ptr [ebp - 0dch], 0ffffffffh
        mov    dword ptr [ebp - 0d8h], 0
        mov    dword ptr [ebp - 0d4h], 0fff7h
        mov    dword ptr [ebp - 0d0h], 9afh
        mov    dword ptr [ebp - 0cch], 1
        mov    dword ptr [ebp - 0c8h], 0ffffh
        mov    dword ptr [ebp - 0c4h], 0
        db  0A1h, 0B8h, 094h, 000h, 000h
        mov    dword ptr [ebp - 0c0h], eax
        mov    dword ptr [ebp - 0bch], 0
        mov    esi, 28h
        jmp    NEAR PTR loc_1_7496
        db  090h
loc_1_7290:
        sub    eax, eax
        mov    esi, 1ah
        lea    edi, [ebp - 0e0h]
        stosd
        stosd
        stosd
        stosd
        stosd
        stosw
        mov    dword ptr [ebp - 0e0h], 1
        cmp    dword ptr [ebp + 18h], eax
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [eax]
        je     SHORT loc_1_7318
        call   sub_1_5F38
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_730C
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        je     SHORT loc_1_72E8
        mov    ax, word ptr [ecx + 6eh]
        push   eax
        push   dword ptr [ecx + 68h]
        lea    eax, [ebp - 0dch]
        push   eax
        call   sub_1_1F70
        add    esp, 0ch
        jmp    NEAR PTR loc_1_7496
        db  08Bh, 0FFh
loc_1_72E8:
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        je     SHORT loc_1_730C
        mov    cx, word ptr [eax + 30h]
        push   ecx
        push   dword ptr [eax + 2ch]
        lea    eax, [ebp - 0dch]
        push   eax
        call   sub_1_1F70
        add    esp, 0ch
        jmp    NEAR PTR loc_1_7496
        db  090h
loc_1_730C:
        pop    edi
        mov    eax, 0fh
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_7318:
        call   sub_1_01D4
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_7330
        mov    eax, 6
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_7330:
        test   byte ptr [eax + 32h], 80h
        jne    SHORT loc_1_7340
        mov    eax, 6
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_7340:
        mov    cx, word ptr [eax + 30h]
        push   ecx
        push   dword ptr [eax + 2ch]
        lea    eax, [ebp - 0dch]
        push   eax
        call   sub_1_1F70
        add    esp, 0ch
        jmp    NEAR PTR loc_1_7496
loc_1_735C:
        mov    esi, 31h
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [eax]
        call   sub_1_5F38
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_746C
        mov    ebx, dword ptr [eax]
        or     ebx, ebx
        je     NEAR PTR loc_1_746C
        lea    edi, [ebp - 0e0h]
        sub    eax, eax
        mov    ecx, 0ch
        rep stosd
        stosb
        movzx  eax, byte ptr [ebx + 84h]
        mov    dword ptr [ebp - 0e0h], eax
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   dword ptr [ebx + 68h]
        push   dword ptr [ebx + 64h]
        db  0FFh, 015h, 0B0h, 095h, 000h, 000h
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_73CC
        mov    dword ptr [ebp - 0c8h], 0ffffffffh
        mov    dword ptr [ebp - 0c4h], 0ffffffffh
        jmp    SHORT loc_1_743D
loc_1_73CC:
        sub    edx, edx
        mov    dword ptr [ebp - 0c4h], 0
        mov    ecx, 0ah
        mov    eax, dword ptr [ebx + 0c4h]
        div    ecx
        or     eax, eax
        je     SHORT loc_1_7434
        mov    ecx, dword ptr [ebx + 0bch]
        mov    dword ptr [ebp - 20h], ecx
        mov    ecx, dword ptr [ebx + 0c0h]
        mov    dword ptr [ebp - 1ch], ecx
        mov    dword ptr [ebp - 10h], 0
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 20h]
        mov    dword ptr [ebp - 18h], eax
        mov    dword ptr [ebp - 14h], ecx
        mov    eax, dword ptr [ebp - 18h]
        mov    edx, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 10h]
        div    dword ptr [ebp - 0ch]
        or     ecx, ecx
        je     SHORT loc_1_7420
        mov    dword ptr [ecx], edx
loc_1_7420:
        mov    ecx, dword ptr [ebp - 4]
        sub    ecx, eax
        sbb    edx, edx
        and    ecx, edx
        add    ecx, eax
        mov    dword ptr [ebp - 0c8h], ecx
        jmp    SHORT loc_1_743D
        db  090h
loc_1_7434:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0c8h], eax
loc_1_743D:
        mov    ax, word ptr [ebx + 7ch]
        shr    ax, 2
        movzx  ecx, ax
        movzx  eax, word ptr [ebx + 7eh]
        add    ecx, eax
        and    ecx, 0fffffffeh
        lea    ecx, [ecx + ecx*4]
        lea    ecx, [ecx + ecx*4]
        mov    dword ptr [ebp - 0c0h], ecx
        mov    dword ptr [ebp - 0c4h], 0
        jmp    SHORT loc_1_7496
        db  08Dh, 049h, 000h
loc_1_746C:
        pop    edi
        mov    eax, 0fh
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_7478:
        sub    eax, eax
        lea    edi, [ebp - 0e0h]
        mov    ecx, 30h
        rep stosd
        mov    dword ptr [ebp - 0e0h], 100h
        mov    esi, 0c0h
loc_1_7496:
        lea    edx, [ebp - 0e0h]
        mov    dword ptr [ebp - 8], 0
        mov    edi, dword ptr [ebp + 14h]
        mov    ebx, dword ptr [edi]
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, ebx
        sub    eax, esi
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, esi
        push   eax
        push   edx
        push   dword ptr [ebp + 10h]
        call   sub_1_1EF8
        add    esp, 10h
        cmp    ebx, esi
        jae    SHORT loc_1_74D4
        mov    eax, 9
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_74D4:
        mov    dword ptr [edi], esi
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_74E0:
        mov    eax, 0feh
        ret
        db  08Bh, 0FFh
loc_1_74E8:
        mov    eax, 0feh
        ret
        db  08Bh, 0FFh
sub_1_74F0:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 40h]
        mov    eax, dword ptr [edx]
        mov    dword ptr [esp + 14h], eax
        mov    dword ptr [esp + 10h], 0
        mov    ecx, dword ptr [esp + 38h]
        mov    esi, dword ptr [ecx]
        or     esi, esi
        jne    SHORT loc_1_7584
        mov    dword ptr [edx], 0
        cmp    dword ptr [ecx + 8], 100h
        jne    SHORT loc_1_7574
        cmp    dword ptr [ecx + 0ch], 100h
        jne    SHORT loc_1_7574
        cmp    dword ptr [ecx + 10h], 0
        jne    SHORT loc_1_7574
        db  0A1h, 03Ch, 097h, 000h, 000h
        shl    eax, 3
        cmp    eax, dword ptr [esp + 14h]
        jbe    SHORT loc_1_754C
        mov    eax, 21h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
loc_1_754C:
        mov    edx, dword ptr [esp + 40h]
        mov    dword ptr [edx], eax
        lea    ecx, [esp + 10h]
        push   ecx
        push   eax
        db  0FFh, 035h, 048h, 096h, 000h, 000h
        push   dword ptr [esp + 48h]
        call   sub_1_1EF8
        add    esp, 10h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
loc_1_7574:
        pop    ebp
        mov    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  08Dh, 049h, 000h
loc_1_7584:
        cmp    esi, 400h
        je     SHORT loc_1_75B4
        cmp    esi, 401h
        je     SHORT loc_1_75B4
        push   dword ptr [esp + 44h]
        push   dword ptr [esp + 44h]
        push   dword ptr [esp + 44h]
        push   ecx
        db  0FFh, 015h, 0C8h, 095h, 000h, 000h
        add    esp, 10h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  08Bh, 0FFh
loc_1_75B4:
        cmp    dword ptr [ecx + 4], 0
        je     SHORT loc_1_75C8
        mov    eax, 0feh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  090h
loc_1_75C8:
        mov    edx, dword ptr [esp + 40h]
        mov    dword ptr [edx], 0
        mov    eax, dword ptr [ecx + 8]
        cmp    eax, 100h
        jne    SHORT loc_1_764C
        cmp    dword ptr [ecx + 0ch], 100h
        jne    SHORT loc_1_763C
        cmp    dword ptr [ecx + 10h], 1
        jne    SHORT loc_1_763C
        cmp    dword ptr [esp + 14h], 4
        jb     SHORT loc_1_762C
        cmp    esi, 400h
        mov    eax, 0
        sete   al
        add    eax, 403h
        mov    dword ptr [esp + 1ch], eax
        lea    eax, [esp + 10h]
        push   eax
        push   4
        lea    eax, [esp + 24h]
        push   eax
        push   dword ptr [esp + 48h]
        call   sub_1_1EF8
        add    esp, 10h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  090h
loc_1_762C:
        pop    ebp
        mov    eax, 21h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  08Dh, 049h, 000h
loc_1_763C:
        pop    ebp
        mov    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  08Dh, 049h, 000h
loc_1_764C:
        cmp    eax, 200h
        jne    NEAR PTR loc_1_785C
        mov    eax, dword ptr [ecx + 0ch]
        cmp    eax, 300h
        jne    NEAR PTR loc_1_7724
        cmp    dword ptr [esp + 14h], 8
        jb     NEAR PTR loc_1_7714
        cmp    dword ptr [ecx + 10h], 5
        jne    NEAR PTR loc_1_7714
        mov    eax, dword ptr [esp + 34h]
        push   dword ptr [eax]
        call   sub_1_5F38
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_7704
        mov    edx, dword ptr [eax]
        or     edx, edx
        je     SHORT loc_1_7704
        mov    eax, dword ptr [edx + 58h]
        and    eax, 4008000h
        cmp    eax, 4008000h
        jne    SHORT loc_1_76C8
        mov    ecx, dword ptr [edx + 10ch]
        mov    eax, ecx
        sub    eax, dword ptr [edx + 108h]
        inc    eax
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [edx + 0a8h]
        sub    eax, dword ptr [edx + 40h]
        add    eax, ecx
        mov    dword ptr [esp + 20h], eax
        jmp    SHORT loc_1_76D8
        db  08Dh, 049h, 000h
loc_1_76C8:
        mov    dword ptr [esp + 1ch], 0
        mov    dword ptr [esp + 20h], 0
loc_1_76D8:
        mov    edx, dword ptr [esp + 40h]
        mov    dword ptr [edx], 8
        lea    eax, [esp + 10h]
        push   eax
        push   8
        lea    eax, [esp + 24h]
        push   eax
        push   dword ptr [esp + 48h]
        call   sub_1_1EF8
        add    esp, 10h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
loc_1_7704:
        pop    ebp
        mov    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  08Dh, 049h, 000h
loc_1_7714:
        pop    ebp
        mov    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  08Dh, 049h, 000h
loc_1_7724:
        cmp    eax, 100h
        je     SHORT loc_1_7738
        mov    eax, 1ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
loc_1_7738:
        mov    eax, dword ptr [ecx + 10h]
        cmp    eax, 1
        je     SHORT loc_1_7754
        cmp    eax, 101h
        je     SHORT loc_1_777C
        mov    eax, 1ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
loc_1_7754:
        mov    eax, 1
        cmp    esi, 401h
        jne    SHORT loc_1_7770
        mov    esi, 14h
        db  0BFh, 07Ch, 095h, 000h, 000h
        jmp    SHORT loc_1_779E
        db  08Dh, 049h, 000h
loc_1_7770:
        mov    esi, 3ch
        db  0BFh, 040h, 095h, 000h, 000h
        jmp    SHORT loc_1_779E
loc_1_777C:
        sub    eax, eax
        cmp    esi, 401h
        jne    SHORT loc_1_7794
        mov    esi, 8
        db  0BFh, 028h, 093h, 000h, 000h
        jmp    SHORT loc_1_779E
        db  08Bh, 0FFh
loc_1_7794:
        mov    esi, 14h
        db  0BFh, 030h, 093h, 000h, 000h
loc_1_779E:
        or     eax, eax
        je     SHORT loc_1_77DC
        cmp    esi, dword ptr [esp + 14h]
        jbe    SHORT loc_1_77B8
        mov    eax, 21h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  08Dh, 049h, 000h
loc_1_77B8:
        mov    edx, dword ptr [esp + 40h]
        mov    dword ptr [edx], esi
        lea    eax, [esp + 10h]
        push   eax
        push   esi
        push   edi
        push   dword ptr [esp + 48h]
        call   sub_1_1EF8
        add    esp, 10h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  090h
loc_1_77DC:
        sub    ebx, ebx
        lea    eax, [esp + 18h]
        push   eax
        push   dword ptr [esp + 48h]
        call   dword ptr [edi]
        add    esp, 8
        mov    ebp, eax
        cmp    dword ptr [esp + 18h], ebx
        jne    SHORT loc_1_7804
        mov    eax, 1ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  08Dh, 049h, 000h
loc_1_7804:
        or     ebp, ebp
        je     SHORT loc_1_7844
loc_1_7808:
        mov    eax, dword ptr [esp + 14h]
        sub    eax, ebx
        cmp    eax, esi
        jl     SHORT loc_1_7844
        lea    eax, [esp + 1ch]
        push   eax
        push   dword ptr [esp + 48h]
        call   dword ptr [edi + 4]
        add    esp, 8
        mov    ebp, eax
        add    ebx, esi
        lea    eax, [esp + 10h]
        push   eax
        push   esi
        lea    eax, [esp + 24h]
        push   eax
        push   dword ptr [esp + 48h]
        call   sub_1_1EF8
        add    esp, 10h
        mov    dword ptr [esp + 3ch], eax
        or     ebp, ebp
        jne    SHORT loc_1_7808
loc_1_7844:
        cmp    ebp, 1
        mov    edx, dword ptr [esp + 40h]
        mov    dword ptr [edx], ebx
        cmc
        sbb    eax, eax
        and    eax, 9
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  090h
loc_1_785C:
        cmp    eax, 300h
        mov    eax, 1ah
        jne    SHORT loc_1_7870
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
loc_1_7870:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
sub_1_7878:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 1ch]
        mov    eax, dword ptr [esi]
        cmp    eax, 400h
        je     SHORT loc_1_78AC
        cmp    eax, 401h
        je     SHORT loc_1_78AC
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 24h]
        push   esi
        db  0FFh, 015h, 0CCh, 095h, 000h, 000h
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  090h
loc_1_78AC:
        cmp    dword ptr [esi + 4], 0
        je     SHORT loc_1_78C0
        mov    eax, 0feh
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_78C0:
        mov    eax, dword ptr [esi + 8]
        cmp    eax, 100h
        jne    SHORT loc_1_78D8
        mov    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_78D8:
        cmp    eax, 200h
        jne    NEAR PTR loc_1_7B40
        mov    eax, dword ptr [esi + 0ch]
        cmp    eax, 300h
        jne    NEAR PTR loc_1_7A38
        mov    edi, 1ah
        cmp    dword ptr [esp + 24h], 4
        jae    SHORT loc_1_7908
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_7908:
        mov    eax, dword ptr [esp + 18h]
        push   dword ptr [eax]
        call   sub_1_5F38
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_7A2D
        sub    edi, edi
        mov    edx, dword ptr [esi + 10h]
        cmp    edx, 6
        jne    SHORT loc_1_79A0
        mov    ecx, dword ptr [esp + 20h]
        mov    ecx, dword ptr [ecx]
        cmp    ecx, 0ffffh
        jbe    SHORT loc_1_7944
        mov    edi, 1ah
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_7944:
        cmp    dword ptr [eax + 24h], ecx
        jb     SHORT loc_1_794E
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_7995
loc_1_794E:
        mov    esi, dword ptr [eax]
        or     byte ptr [eax + 0dh], 4
        mov    dword ptr [eax + 24h], ecx
        or     esi, esi
        je     SHORT loc_1_7995
        mov    al, byte ptr [esi + 84h]
        cmp    al, 4
        jb     SHORT loc_1_7995
        cmp    al, 6
        ja     SHORT loc_1_7995
        test   byte ptr [esi + 86h], 1
        jne    SHORT loc_1_7995
        or     dword ptr [esi + 58h], 1
        mov    dword ptr [esi + 94h], ecx
        inc    dword ptr [esi + 74h]
        push   esi
        call   sub_1_80C8
        add    esp, 4
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_1_63D8
        add    esp, 8
loc_1_7995:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_79A0:
        sub    esi, esi
        mov    ecx, dword ptr [esp + 20h]
        mov    ebx, dword ptr [ecx]
        cmp    edx, 1
        je     SHORT loc_1_79C4
        cmp    edx, 2
        je     SHORT loc_1_79D4
        cmp    edx, 3
        je     SHORT loc_1_79DC
        cmp    edx, 4
        je     SHORT loc_1_79E4
        mov    edi, 1ah
        jmp    SHORT loc_1_79E9
        db  090h
loc_1_79C4:
        cmp    ebx, 1
        sbb    ebx, ebx
        neg    ebx
        mov    esi, 20000h
        jmp    SHORT loc_1_79E9
        db  08Bh, 0FFh
loc_1_79D4:
        mov    esi, 2000000h
        jmp    SHORT loc_1_79E9
        db  090h
loc_1_79DC:
        mov    esi, 4000000h
        jmp    SHORT loc_1_79E9
        db  090h
loc_1_79E4:
        mov    esi, 2000h
loc_1_79E9:
        or     edi, edi
        jne    SHORT loc_1_7A2D
        or     ebx, ebx
        je     SHORT loc_1_79F8
        or     dword ptr [eax + 20h], esi
        jmp    SHORT loc_1_79FF
        db  08Bh, 0FFh
loc_1_79F8:
        mov    ecx, esi
        not    ecx
        and    dword ptr [eax + 20h], ecx
loc_1_79FF:
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_1_7A2D
        or     ebx, ebx
        je     SHORT loc_1_7A10
        or     dword ptr [eax + 58h], esi
        jmp    SHORT loc_1_7A15
        db  08Bh, 0FFh
loc_1_7A10:
        not    esi
        and    dword ptr [eax + 58h], esi
loc_1_7A15:
        cmp    edx, 2
        jne    SHORT loc_1_7A2D
        db  08Bh, 00Dh, 06Ch, 096h, 000h, 000h
        mov    dword ptr [eax + 98h], ecx
        mov    byte ptr [eax + 87h], 0
loc_1_7A2D:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_7A38:
        cmp    eax, 200h
        jne    SHORT loc_1_7A60
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 28h]
        push   dword ptr [esi + 10h]
        push   dword ptr [esp + 24h]
        call   sub_1_0F70
        add    esp, 10h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_7A60:
        cmp    eax, 100h
        je     SHORT loc_1_7A74
        mov    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  090h
loc_1_7A74:
        cmp    dword ptr [esi + 10h], 101h
        jne    NEAR PTR loc_1_7B34
        cmp    dword ptr [esp + 24h], 14h
        je     SHORT loc_1_7A94
        mov    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_7A94:
        mov    ecx, dword ptr [esp + 20h]
        cmp    dword ptr [ecx], 0ch
        je     SHORT loc_1_7AAC
        mov    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_7AAC:
        push   dword ptr [ecx + 8]
        push   dword ptr [ecx + 10h]
        push   dword ptr [ecx + 0ch]
        push   dword ptr [ecx + 4]
        call   sub_1_5B50
        add    esp, 10h
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_7B28
        inc    dword ptr [esi + 74h]
        cmp    byte ptr [esi + 84h], 0
        je     SHORT loc_1_7B10
        test   byte ptr [esi + 86h], 1
        jne    SHORT loc_1_7B10
        or     dword ptr [esi + 58h], 40000000h
        push   dword ptr [esp + 0ch]
        push   40h
        push   esi
        call   sub_1_6398
        add    esp, 0ch
        cmp    byte ptr [esi + 84h], 0ah
        je     SHORT loc_1_7B10
        push   esi
        call   sub_1_60F4
        add    esp, 4
        push   14h
        push   0
        push   esi
        call   sub_1_6FB8
        add    esp, 0ch
loc_1_7B10:
        push   dword ptr [esp + 0ch]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_7B28:
        pop    edi
        mov    eax, 1ah
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_7B34:
        pop    edi
        mov    eax, 1ah
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_7B40:
        cmp    eax, 300h
        mov    eax, 0feh
        jne    SHORT loc_1_7B54
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  090h
loc_1_7B54:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  090h
sub_1_7B5C:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0A1h, 038h, 093h, 000h, 000h
        db  03Bh, 005h, 008h, 096h, 000h, 000h
        jbe    NEAR PTR loc_1_7C74
        push   0ch
        call   sub_1_90F6
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_7B90
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Bh, 0FFh
loc_1_7B90:
        push   10h
        lea    eax, [esi + 4]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_1_91DE
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_1_7BBC
        push   esi
        call   sub_1_90FC
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Bh, 0FFh
loc_1_7BBC:
        db  08Bh, 03Dh, 094h, 095h, 000h, 000h
        add    edi, 20h
        mov    eax, edi
        shl    eax, 4
        push   eax
        call   sub_1_90F6
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_7BF4
        push   dword ptr [esi + 4]
        call   sub_1_91E4
        push   esi
        call   sub_1_90FC
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Bh, 0FFh
loc_1_7BF4:
        sub    ebp, ebp
        mov    dword ptr [esi + 8], eax
        db  083h, 005h, 008h, 096h, 000h, 000h, 010h
        db  08Bh, 00Dh, 08Ch, 096h, 000h, 000h
        mov    dword ptr [esi], ecx
        db  089h, 035h, 08Ch, 096h, 000h, 000h
        sub    ebx, ebx
        mov    dword ptr [esp + 18h], eax
loc_1_7C14:
        push   edi
        push   dword ptr [esp + 1ch]
        push   dword ptr [esi + 4]
        lea    eax, [esp + 1ch]
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        call   sub_1_91EA
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_1_7C5E
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 0ch], 14h
        or     ebx, ebx
        je     SHORT loc_1_7C50
        push   dword ptr [esp + 10h]
        call   sub_1_7CB8
        add    esp, 4
        jmp    SHORT loc_1_7C54
        db  090h
loc_1_7C50:
        mov    ebp, dword ptr [esp + 10h]
loc_1_7C54:
        inc    ebx
        add    dword ptr [esp + 18h], edi
        cmp    ebx, 10h
        jb     SHORT loc_1_7C14
loc_1_7C5E:
        mov    eax, ebp
        sub    ebx, 10h
        db  001h, 01Dh, 008h, 096h, 000h, 000h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Dh, 049h, 000h
loc_1_7C74:
        db  0A1h, 0F0h, 096h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_7C90
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 0F0h, 096h, 000h, 000h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Dh, 049h, 000h
loc_1_7C90:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Bh, 0FFh
sub_1_7C9C:
        db  0A1h, 0F0h, 096h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_7CB0
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 0F0h, 096h, 000h, 000h
        ret
        db  08Bh, 0FFh
loc_1_7CB0:
        jmp    NEAR PTR sub_1_7B5C
        db  08Dh, 049h, 000h
sub_1_7CB8:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0ch], 14h
        db  08Bh, 00Dh, 0F0h, 096h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  0A3h, 0F0h, 096h, 000h, 000h
        ret
        db  08Dh, 049h, 000h
sub_1_7CD4:
        db  08Bh, 00Dh, 070h, 096h, 000h, 000h
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], ecx
        db  0A3h, 070h, 096h, 000h, 000h
        ret
        db  08Bh, 0FFh
sub_1_7CE8:
        db  0A1h, 070h, 096h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_7CFC
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 070h, 096h, 000h, 000h
        ret
        db  08Bh, 0FFh
loc_1_7CFC:
        db  0A1h, 038h, 097h, 000h, 000h
        db  039h, 005h, 03Ch, 093h, 000h, 000h
        mov    eax, 0
        jbe    SHORT loc_1_7D18
        push   34h
        call   sub_1_90F6
        add    esp, 4
loc_1_7D18:
        or     eax, eax
        je     SHORT loc_1_7D22
        db  0FFh, 005h, 038h, 097h, 000h, 000h
loc_1_7D22:
        ret
        db  090h
sub_1_7D24:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        or     esi, esi
        je     NEAR PTR loc_1_7DBC
        mov    eax, dword ptr [esp + 14h]
        mov    edi, dword ptr [eax]
        mov    ax, word ptr [esi + 8]
        or     ax, ax
        je     SHORT loc_1_7D4D
        and    eax, 0ffffh
        nop
loc_1_7D48:
        mov    edi, dword ptr [edi]
        dec    eax
        jne    SHORT loc_1_7D48
loc_1_7D4D:
        sub    ebx, ebx
        cmp    word ptr [esi + 0ah], bx
        jbe    SHORT loc_1_7D68
loc_1_7D55:
        mov    eax, edi
        mov    edi, dword ptr [edi]
        push   eax
        call   sub_1_91F6
        inc    ebx
        movzx  eax, word ptr [esi + 0ah]
        cmp    eax, ebx
        ja     SHORT loc_1_7D55
loc_1_7D68:
        mov    edx, dword ptr [esi]
        sub    edi, edi
        cmp    dword ptr [esi + 4], edi
        jbe    SHORT loc_1_7DBC
loc_1_7D71:
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [edx + 18h]
        dec    eax
        mov    dword ptr [edx + 18h], eax
        or     eax, eax
        jg     SHORT loc_1_7DB4
        mov    eax, dword ptr [edx + 2ch]
        or     eax, eax
        je     SHORT loc_1_7D92
        mov    dword ptr [eax], 0
        mov    dword ptr [edx + 2ch], 0
loc_1_7D92:
        db  0A1h, 014h, 096h, 000h, 000h
        mov    dword ptr [eax], edx
        db  0A1h, 014h, 096h, 000h, 000h
        mov    dword ptr [edx + 4], eax
        db  089h, 015h, 014h, 096h, 000h, 000h
        db  0C7h, 002h, 010h, 096h, 000h, 000h
        db  083h, 00Dh, 050h, 096h, 000h, 000h, 002h
loc_1_7DB4:
        mov    edx, ecx
        inc    edi
        cmp    dword ptr [esi + 4], edi
        ja     SHORT loc_1_7D71
loc_1_7DBC:
        push   dword ptr [esp + 14h]
        call   sub_1_7CB8
        add    esp, 4
        db  0F6h, 005h, 050h, 096h, 000h, 000h, 002h
        je     SHORT loc_1_7DD6
        call   sub_1_23C8
loc_1_7DD6:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
sub_1_7DDC:
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0ch]
        mov    eax, dword ptr [edx + 94h]
        mov    esi, eax
        sub    esi, dword ptr [edx + 0a8h]
        movzx  edi, word ptr [edx + 70h]
        shr    eax, 1
        sub    edi, eax
        sbb    ecx, ecx
        and    edi, ecx
        add    edi, eax
        mov    eax, esi
        sub    eax, dword ptr [edx + 44h]
        cmp    edi, eax
        jg     SHORT loc_1_7E0A
        mov    dword ptr [edx + 44h], esi
loc_1_7E0A:
        pop    edi
        mov    eax, dword ptr [edx + 44h]
        pop    esi
        ret
sub_1_7E10:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        call   sub_1_7C9C
        mov    ebx, eax
        mov    esi, dword ptr [esp + 18h]
        mov    ax, word ptr [esi + 72h]
        mov    word ptr [esi + 90h], ax
        or     ebx, ebx
        je     NEAR PTR loc_1_7FB5
        mov    edi, dword ptr [ebx + 4]
        db  003h, 03Dh, 094h, 095h, 000h, 000h
        mov    dword ptr [ebx], 0
        mov    dword ptr [ebx + 0ch], 18h
        mov    ax, word ptr [esi + 6eh]
        mov    word ptr [edi], ax
        mov    ax, word ptr [esi + 6ch]
        mov    word ptr [edi + 2], ax
        mov    ecx, dword ptr [esi + 8]
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
        mov    dword ptr [edi + 4], edx
        inc    ecx
        mov    dword ptr [esi + 8], ecx
        sub    ecx, dword ptr [esi + 0ch]
        or     ecx, ecx
        jle    SHORT loc_1_7EA0
        db  0FFh, 005h, 068h, 095h, 000h, 000h
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [esi + 0ch], eax
        jmp    SHORT loc_1_7EA6
        db  08Dh, 049h, 000h
loc_1_7EA0:
        db  0FFh, 005h, 06Ch, 095h, 000h, 000h
loc_1_7EA6:
        mov    ecx, dword ptr [esi + 40h]
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
        mov    dword ptr [edi + 8], edx
        cmp    byte ptr [esi + 84h], 3
        mov    word ptr [edi + 0ch], 1260h
        je     SHORT loc_1_7EE4
        mov    word ptr [edi + 0ch], 260h
loc_1_7EE4:
        mov    ax, word ptr [esi + 44h]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [edi + 0eh], cx
        mov    word ptr [edi + 10h], 0
        lea    eax, [edi + 14h]
        mov    dword ptr [esp + 10h], eax
        lea    eax, [esp + 0eh]
        push   eax
        push   dword ptr [esi + 68h]
        db  0FFh, 015h, 0ACh, 095h, 000h, 000h
        add    esp, 8
        or     al, al
        jne    SHORT loc_1_7F24
        push   ebx
        call   sub_1_7CB8
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_7F24:
        mov    eax, dword ptr [esp + 10h]
        sub    word ptr [esp + 0eh], 14h
        mov    byte ptr [eax], 2
        inc    dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [eax], 4
        inc    dword ptr [esp + 10h]
        mov    ah, byte ptr [esp + 0eh]
        mov    al, byte ptr [esp + 0fh]
        mov    ecx, dword ptr [esp + 10h]
        mov    word ptr [ecx], ax
        inc    dword ptr [esi + 74h]
        push   ebx
        mov    eax, dword ptr [esi + 24h]
        add    eax, 1800h
        push   eax
        call   sub_1_1D1C
        add    esp, 8
        not    ax
        mov    word ptr [edi + 10h], ax
        push   dword ptr [esi + 0f0h]
        lea    eax, [esi + 0e4h]
        push   eax
        push   dword ptr [esi + 68h]
        push   dword ptr [esi + 64h]
        push   18h
        push   ebx
        push   0
        db  0FFh, 035h, 0D8h, 095h, 000h, 000h
        db  0FFh, 015h, 098h, 095h, 000h, 000h
        add    esp, 20h
        mov    dword ptr [esi + 88h], eax
        cmp    eax, 2bf7h
        je     SHORT loc_1_7FA8
        push   ebx
        call   sub_1_7CB8
        add    esp, 4
loc_1_7FA8:
        push   dword ptr [esp + 1ch]
        push   esi
        call   sub_1_63D8
        add    esp, 8
loc_1_7FB5:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_7FBC:
        push   ebx
        push   esi
        push   edi
        call   sub_1_7C9C
        mov    edi, eax
        or     edi, edi
        je     NEAR PTR loc_1_80C3
        mov    ebx, dword ptr [edi + 4]
        db  003h, 01Dh, 094h, 095h, 000h, 000h
        mov    dword ptr [edi], 0
        mov    dword ptr [edi + 0ch], 15h
        mov    esi, dword ptr [esp + 10h]
        mov    ax, word ptr [esi + 6eh]
        mov    word ptr [ebx], ax
        mov    ax, word ptr [esi + 6ch]
        mov    word ptr [ebx + 2], ax
        mov    ecx, dword ptr [esi + 4]
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
        mov    dword ptr [ebx + 4], edx
        db  0FFh, 005h, 06Ch, 095h, 000h, 000h
        mov    ecx, dword ptr [esi + 40h]
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
        mov    dword ptr [ebx + 8], edx
        mov    word ptr [ebx + 0ch], 1050h
        push   esi
        call   sub_1_7DDC
        add    esp, 4
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx + 0eh], cx
        mov    word ptr [ebx + 10h], 0
        push   edi
        mov    eax, dword ptr [esi + 24h]
        add    eax, 1500h
        push   eax
        call   sub_1_1D1C
        add    esp, 8
        not    ax
        mov    word ptr [ebx + 10h], ax
        inc    byte ptr [esi + 87h]
        push   dword ptr [esi + 0f0h]
        lea    eax, [esi + 0e4h]
        push   eax
        push   dword ptr [esi + 68h]
        push   dword ptr [esi + 64h]
        push   15h
        push   edi
        push   0
        db  0FFh, 035h, 0D8h, 095h, 000h, 000h
        db  0FFh, 015h, 098h, 095h, 000h, 000h
        add    esp, 20h
        cmp    eax, 2bf7h
        je     SHORT loc_1_80C3
        push   edi
        call   sub_1_7CB8
        add    esp, 4
loc_1_80C3:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
sub_1_80C8:
        push   ebx
        push   esi
        push   edi
        call   sub_1_7C9C
        mov    edi, eax
        or     edi, edi
        je     NEAR PTR loc_1_81F8
        mov    ebx, dword ptr [edi + 4]
        db  003h, 01Dh, 094h, 095h, 000h, 000h
        mov    dword ptr [edi], 0
        mov    esi, dword ptr [esp + 10h]
        mov    ax, word ptr [esi + 6eh]
        mov    word ptr [ebx], ax
        mov    ax, word ptr [esi + 6ch]
        mov    word ptr [ebx + 2], ax
        mov    ecx, dword ptr [esi + 40h]
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
        mov    dword ptr [ebx + 8], edx
        cmp    dword ptr [esi + 10h], 0
        je     SHORT loc_1_8130
        mov    ecx, dword ptr [esi + 8]
        jmp    SHORT loc_1_8133
loc_1_8130:
        mov    ecx, dword ptr [esi + 4]
loc_1_8133:
        test   byte ptr [esi + 5bh], 20h
        je     SHORT loc_1_8149
        mov    eax, dword ptr [esi + 0ch]
        sub    eax, ecx
        cmp    eax, 1
        mov    word ptr [ebx + 0ch], 1150h
        je     SHORT loc_1_814F
loc_1_8149:
        mov    word ptr [ebx + 0ch], 1050h
loc_1_814F:
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
        mov    dword ptr [ebx + 4], edx
        push   esi
        call   sub_1_7DDC
        add    esp, 4
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx + 0eh], cx
        mov    word ptr [ebx + 10h], 0
        push   edi
        mov    eax, dword ptr [esi + 24h]
        add    eax, 1400h
        push   eax
        call   sub_1_1D1C
        add    esp, 8
        not    ax
        mov    word ptr [ebx + 10h], ax
        and    dword ptr [esi + 58h], 0fffff5ffh
        mov    word ptr [esi + 92h], 0
        db  0FFh, 005h, 068h, 095h, 000h, 000h
        push   dword ptr [esi + 0f0h]
        lea    eax, [esi + 0e4h]
        push   eax
        push   dword ptr [esi + 68h]
        push   dword ptr [esi + 64h]
        push   14h
        push   edi
        push   0
        db  0FFh, 035h, 0D8h, 095h, 000h, 000h
        db  0FFh, 015h, 098h, 095h, 000h, 000h
        add    esp, 20h
        mov    dword ptr [esi + 88h], eax
        cmp    eax, 2bf7h
        je     SHORT loc_1_81F8
        push   edi
        call   sub_1_7CB8
        add    esp, 4
loc_1_81F8:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_81FC:
        push   ebx
        push   esi
        push   edi
        call   sub_1_7C9C
        mov    edi, eax
        or     edi, edi
        je     NEAR PTR loc_1_82D2
        mov    ebx, dword ptr [edi + 4]
        db  003h, 01Dh, 094h, 095h, 000h, 000h
        mov    dword ptr [edi], 0
        mov    esi, dword ptr [esp + 10h]
        mov    ax, word ptr [esi + 6eh]
        mov    word ptr [ebx], ax
        mov    ax, word ptr [esi + 6ch]
        mov    word ptr [ebx + 2], ax
        cmp    dword ptr [esi + 10h], 0
        je     SHORT loc_1_823C
        mov    ecx, dword ptr [esi + 0ch]
        jmp    SHORT loc_1_823F
        db  090h
loc_1_823C:
        mov    ecx, dword ptr [esi + 4]
loc_1_823F:
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
        mov    dword ptr [ebx + 4], edx
        mov    word ptr [ebx + 0ch], 450h
        mov    word ptr [ebx + 0eh], 0
        mov    word ptr [ebx + 10h], 0
        push   edi
        mov    eax, dword ptr [esi + 24h]
        add    eax, 1400h
        push   eax
        call   sub_1_1D1C
        add    esp, 8
        not    ax
        mov    word ptr [ebx + 10h], ax
        db  0FFh, 005h, 068h, 095h, 000h, 000h
        db  0FFh, 005h, 074h, 095h, 000h, 000h
        push   dword ptr [esi + 0f0h]
        lea    eax, [esi + 0e4h]
        push   eax
        push   dword ptr [esi + 68h]
        push   dword ptr [esi + 64h]
        push   14h
        push   edi
        push   0
        db  0FFh, 035h, 0D8h, 095h, 000h, 000h
        db  0FFh, 015h, 098h, 095h, 000h, 000h
        add    esp, 20h
        cmp    eax, 2bf7h
        je     SHORT loc_1_82D2
        push   edi
        call   sub_1_7CB8
        add    esp, 4
loc_1_82D2:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
sub_1_82D8:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 20h]
        test   byte ptr [ebx + 0dh], 4
        jne    NEAR PTR loc_1_8454
        call   sub_1_7C9C
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_1_8454
        mov    edi, dword ptr [esi + 4]
        db  003h, 03Dh, 094h, 095h, 000h, 000h
        mov    dword ptr [esi], 0
        mov    ax, word ptr [ebx + 0ch]
        test   ah, 2
        mov    edx, dword ptr [esp + 24h]
        je     SHORT loc_1_8319
        inc    edx
loc_1_8319:
        test   ah, 1
        je     SHORT loc_1_831F
        inc    edx
loc_1_831F:
        test   ah, 10h
        je     SHORT loc_1_8334
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [edi + 4], eax
        mov    word ptr [edi + 0ch], 450h
        jmp    SHORT loc_1_8390
        db  08Bh, 0FFh
loc_1_8334:
        mov    dword ptr [edi + 4], 0
        mov    eax, dword ptr [ebx + 4]
        mov    ecx, eax
        and    ecx, 0ff00h
        mov    ebp, eax
        shl    ebp, 10h
        or     ecx, ebp
        shl    ecx, 8
        mov    ebp, eax
        and    ebp, 0ff0000h
        shr    ebp, 8
        or     ecx, ebp
        shr    eax, 18h
        or     ecx, eax
        add    ecx, edx
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
        mov    dword ptr [edi + 8], edx
        mov    word ptr [edi + 0ch], 1450h
loc_1_8390:
        mov    word ptr [edi + 0eh], 0
        mov    ax, word ptr [ebx]
        mov    word ptr [edi + 2], ax
        mov    ax, word ptr [ebx + 2]
        mov    word ptr [edi], ax
        mov    word ptr [edi + 10h], 0
        push   esi
        movzx  ecx, word ptr [esp + 32h]
        movzx  eax, word ptr [esp + 2eh]
        add    ecx, eax
        movzx  eax, word ptr [esp + 30h]
        add    ecx, eax
        movzx  eax, word ptr [esp + 2ch]
        add    ecx, eax
        add    ecx, 1a00h
        push   ecx
        call   sub_1_1D1C
        add    esp, 8
        not    ax
        mov    word ptr [edi + 10h], ax
        lea    eax, [esp + 10h]
        push   eax
        db  0FFh, 015h, 0B8h, 095h, 000h, 000h
        add    esp, 4
        mov    eax, dword ptr [esp + 30h]
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_8407
        push   0
        push   dword ptr [esp + 2ch]
        lea    ecx, [esp + 18h]
        push   ecx
        push   eax
        db  0FFh, 015h, 0BCh, 095h, 000h, 000h
        add    esp, 10h
loc_1_8407:
        db  0FFh, 005h, 068h, 095h, 000h, 000h
        db  0FFh, 005h, 074h, 095h, 000h, 000h
        push   0
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 34h]
        push   14h
        push   esi
        push   0
        db  0FFh, 035h, 0D8h, 095h, 000h, 000h
        db  0FFh, 015h, 098h, 095h, 000h, 000h
        add    esp, 20h
        cmp    eax, 2bf7h
        je     SHORT loc_1_8446
        push   esi
        call   sub_1_7CB8
        add    esp, 4
loc_1_8446:
        lea    eax, [esp + 10h]
        push   eax
        db  0FFh, 015h, 0C4h, 095h, 000h, 000h
        add    esp, 4
loc_1_8454:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_1_845C:
        mov    eax, dword ptr [esp + 4]
        movzx  ecx, word ptr [eax + 70h]
        mov    dword ptr [eax + 1ch], ecx
        mov    dword ptr [eax + 20h], 0ffffffffh
        mov    byte ptr [eax + 84h], 4
        mov    cl, byte ptr [eax + 82h]
        dec    cl
        mov    byte ptr [eax + 82h], cl
        jne    SHORT loc_1_8489
        and    dword ptr [eax + 5ch], 0fffffffeh
loc_1_8489:
        db  0FFh, 005h, 060h, 095h, 000h, 000h
        and    dword ptr [eax + 58h], 0fffffff7h
        ret
sub_1_8494:
        push   esi
        mov    esi, dword ptr [esp + 8]
        call   sub_1_90E4
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [esi + 88h], 0
        movzx  ecx, word ptr [esi + 68h]
        movzx  eax, word ptr [esi + 6ah]
        add    ecx, eax
        movzx  eax, word ptr [esi + 64h]
        add    ecx, eax
        movzx  eax, word ptr [esi + 66h]
        add    ecx, eax
        add    ecx, 600h
        mov    dword ptr [esi + 24h], ecx
        mov    byte ptr [esi + 85h], 0
        mov    dword ptr [esi + 8ch], 0
        mov    word ptr [esi + 7ch], 0
        mov    word ptr [esi + 7eh], 3ch
        mov    word ptr [esi + 72h], 1eh
        mov    word ptr [esi + 90h], 0
        mov    word ptr [esi + 92h], 0
        pop    esi
        ret
        db  090h
loc_1_8508:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        cmp    byte ptr [esp + 1ch], 0
        jne    NEAR PTR loc_1_86BC
        mov    edi, dword ptr [esp + 34h]
        mov    ax, word ptr [edi]
        push   eax
        mov    ax, word ptr [edi + 2]
        push   eax
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 34h]
        call   sub_1_5B50
        add    esp, 10h
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_1_8745
        mov    al, byte ptr [esi + 84h]
        or     al, al
        je     NEAR PTR loc_1_8745
        cmp    al, 0ah
        je     NEAR PTR loc_1_8745
        test   byte ptr [esi + 86h], 1
        jne    NEAR PTR loc_1_8745
        mov    ebx, dword ptr [esp + 20h]
        lea    eax, [ebx - 2afah]
        cmp    eax, 12h
        ja     NEAR PTR loc_1_8745
        db  0FFh, 024h, 085h, 04Ch, 087h, 000h, 000h
        db  08Dh, 049h, 000h
loc_1_8580:
        pop    edi
        mov    dword ptr [esi + 88h], ebx
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Dh, 049h, 000h
loc_1_8590:
        mov    dword ptr [esi + 88h], ebx
        inc    dword ptr [esi + 74h]
        push   dword ptr [esp + 10h]
        push   8
        push   esi
        call   sub_1_6398
        add    esp, 0ch
        push   esi
        call   sub_1_60F4
        add    esp, 4
        push   1ch
        push   ebx
        call   sub_1_6090
        add    esp, 8
        push   eax
        push   0
        push   esi
        call   sub_1_6FB8
        add    esp, 0ch
        push   dword ptr [esp + 14h]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_1_85DC:
        movzx  edx, word ptr [esi + 70h]
        mov    edi, dword ptr [esi + 10h]
        mov    eax, dword ptr [esi + 1ch]
        sub    edi, eax
        sbb    ecx, ecx
        and    edi, ecx
        add    edi, eax
        shr    edi, 1
        lea    eax, [edx*2]
        sub    edi, eax
        sbb    ecx, ecx
        not    ecx
        and    edi, ecx
        add    edi, eax
        mov    dword ptr [esi + 20h], edi
        mov    dword ptr [esi + 1ch], edx
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Bh, 0FFh
loc_1_8610:
        movzx  eax, byte ptr [esi + 0ech]
        sub    eax, 14h
        add    dword ptr [esp + 30h], eax
        mov    edx, dword ptr [edi + 4]
        mov    ecx, edx
        and    ecx, 0ff00h
        mov    eax, edx
        shl    eax, 10h
        or     ecx, eax
        shl    ecx, 8
        mov    eax, edx
        and    eax, 0ff0000h
        shr    eax, 8
        or     ecx, eax
        shr    edx, 18h
        or     ecx, edx
        mov    ax, word ptr [esp + 30h]
        cmp    word ptr [esi + 70h], ax
        jb     SHORT loc_1_867C
        mov    eax, ecx
        sub    eax, dword ptr [esi + 4]
        js     SHORT loc_1_867C
        mov    eax, ecx
        sub    eax, dword ptr [esi + 8]
        jns    SHORT loc_1_867C
        push   ecx
        push   esi
        call   sub_1_5534
        add    esp, 8
        mov    eax, dword ptr [esp + 30h]
        mov    dword ptr [esi + 1ch], eax
        push   400h
        push   esi
        call   sub_1_2388
        add    esp, 8
loc_1_867C:
        movzx  eax, word ptr [esi + 80h]
        mov    ecx, dword ptr [esp + 30h]
        sub    eax, ecx
        sbb    edx, edx
        and    eax, edx
        add    ax, cx
        mov    word ptr [esi + 70h], ax
        and    eax, 0ffffh
        cmp    dword ptr [esi + 1ch], eax
        jae    NEAR PTR loc_1_8745
        mov    dword ptr [esi + 1ch], eax
        add    eax, eax
        cmp    dword ptr [esi + 20h], eax
        jae    NEAR PTR loc_1_8745
        mov    dword ptr [esi + 20h], eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Bh, 0FFh
loc_1_86BC:
        mov    ebx, dword ptr [esp + 20h]
        cmp    ebx, 2b0dh
        je     SHORT loc_1_86D8
        cmp    ebx, 2b0eh
        je     SHORT loc_1_8708
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  090h
loc_1_86D8:
        mov    eax, dword ptr [esp + 30h]
        sub    eax, 14h
        mov    dword ptr [esp + 0ch], eax
        lea    eax, [esp + 0ch]
        push   eax
        lea    eax, [esp + 2ch]
        push   eax
        lea    eax, [esp + 2ch]
        push   eax
        db  068h, 02Ch, 05Ah, 000h, 000h
        call   sub_1_5AB0
        add    esp, 10h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Bh, 0FFh
loc_1_8708:
        push   0
        push   0
        push   0
        push   0
        push   6
        call   0
        add    esp, 14h
        push   0
        db  068h, 0ECh, 092h, 000h, 000h
        call   6
        add    esp, 8
        push   0
        db  068h, 064h, 001h, 000h, 000h
        call   sub_1_000C
        add    esp, 8
        db  068h, 0ECh, 092h, 000h, 000h
        call   sub_1_911A
        add    esp, 4
loc_1_8745:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  080h, 085h, 000h, 000h, 080h, 085h, 000h, 000h, 090h, 085h, 000h, 000h, 080h, 085h, 000h, 000h
        db  045h, 087h, 000h, 000h, 045h, 087h, 000h, 000h, 045h, 087h, 000h, 000h, 080h, 085h, 000h, 000h
        db  045h, 087h, 000h, 000h, 045h, 087h, 000h, 000h, 080h, 085h, 000h, 000h, 080h, 085h, 000h, 000h
        db  080h, 085h, 000h, 000h, 080h, 085h, 000h, 000h, 0DCh, 085h, 000h, 000h, 045h, 087h, 000h, 000h
        db  045h, 087h, 000h, 000h, 045h, 087h, 000h, 000h, 010h, 086h, 000h, 000h
sub_1_8798:
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 0ch]
        mov    ax, word ptr [ecx + 6eh]
        mov    esi, dword ptr [esp + 10h]
        mov    word ptr [esi], ax
        mov    ax, word ptr [ecx + 6ch]
        mov    word ptr [esi + 2], ax
        mov    edx, dword ptr [ecx + 8]
        mov    edi, edx
        and    edi, 0ff00h
        mov    eax, edx
        shl    eax, 10h
        or     edi, eax
        shl    edi, 8
        mov    eax, edx
        and    eax, 0ff0000h
        shr    eax, 8
        or     edi, eax
        shr    edx, 18h
        or     edi, edx
        mov    dword ptr [esi + 4], edi
        mov    edx, dword ptr [ecx + 40h]
        mov    edi, edx
        and    edi, 0ff00h
        mov    eax, edx
        shl    eax, 10h
        or     edi, eax
        shl    edi, 8
        mov    eax, edx
        and    eax, 0ff0000h
        shr    eax, 8
        or     edi, eax
        shr    edx, 18h
        or     edi, edx
        mov    dword ptr [esi + 8], edi
        mov    word ptr [esi + 0ch], 1050h
        mov    dword ptr [esi + 10h], 0
        push   ecx
        call   sub_1_7DDC
        add    esp, 4
        mov    ch, al
        mov    cl, ah
        mov    word ptr [esi + 0eh], cx
        pop    edi
        pop    esi
        ret
sub_1_8824:
        sub    esp, 40h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 54h]
        mov    eax, dword ptr [ebp + 58h]
        test   eax, 40000h
        jne    NEAR PTR loc_1_8DB8
        test   byte ptr [ebp + 5ch], 2
        jne    NEAR PTR loc_1_8DB8
        or     eax, 40000h
        mov    dword ptr [ebp + 58h], eax
        test   eax, 10000000h
        jne    NEAR PTR loc_1_8CE9
loc_1_885A:
        mov    edx, dword ptr [ebp + 8]
        sub    edx, dword ptr [ebp + 4]
        mov    eax, dword ptr [ebp + 14h]
        sub    eax, edx
        mov    dword ptr [esp + 40h], eax
        mov    esi, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [ebp + 10h]
        sub    esi, eax
        sbb    ecx, ecx
        and    esi, ecx
        add    esi, eax
        sub    esi, edx
        mov    edx, dword ptr [ebp + 58h]
        mov    eax, edx
        and    eax, 400000h
        shr    eax, 16h
        cmp    esi, eax
        jg     SHORT loc_1_888C
        mov    esi, eax
loc_1_888C:
        mov    dword ptr [esp + 3ch], esi
        movzx  esi, word ptr [ebp + 70h]
        mov    edi, dword ptr [esp + 3ch]
        mov    eax, dword ptr [esp + 40h]
        sub    edi, eax
        sbb    ecx, ecx
        and    edi, ecx
        add    edi, eax
        sub    edi, esi
        sbb    eax, eax
        and    edi, eax
        lea    ecx, [edi + esi]
        mov    dword ptr [esp + 2ch], ecx
        cmp    ecx, esi
        je     SHORT loc_1_88E1
        or     ecx, ecx
        je     SHORT loc_1_88BF
        cmp    ecx, dword ptr [esp + 40h]
        je     SHORT loc_1_88E1
loc_1_88BF:
        cmp    dword ptr [esp + 3ch], 0
        je     NEAR PTR loc_1_8CF8
        test   edx, 410000h
        jne    SHORT loc_1_88E1
        mov    eax, dword ptr [ebp + 18h]
        shr    eax, 1
        cmp    eax, dword ptr [esp + 2ch]
        ja     NEAR PTR loc_1_8CF8
loc_1_88E1:
        call   sub_1_7C9C
        mov    dword ptr [esp + 38h], eax
        or     eax, eax
        je     NEAR PTR loc_1_8D71
        mov    edi, eax
        mov    edx, dword ptr [ebp + 28h]
        mov    dword ptr [esp + 30h], edx
        mov    eax, dword ptr [eax + 4]
        db  003h, 005h, 094h, 095h, 000h, 000h
        mov    dword ptr [esp + 34h], eax
        mov    ebx, eax
        add    ebx, 14h
        push   eax
        push   ebp
        call   sub_1_8798
        add    esp, 8
        mov    word ptr [ebx + 8], 0
        mov    word ptr [ebx + 0ah], 0
        mov    dword ptr [ebx + 4], 0
        mov    esi, dword ptr [esp + 2ch]
        or     esi, esi
        je     NEAR PTR loc_1_8AFC
        mov    edx, dword ptr [esp + 30h]
        mov    dword ptr [ebx], edx
loc_1_893C:
        mov    edx, dword ptr [esp + 30h]
        inc    dword ptr [edx + 18h]
        inc    dword ptr [ebx + 4]
        mov    ecx, dword ptr [ebp + 30h]
        or     ecx, ecx
        jne    SHORT loc_1_8988
        mov    eax, dword ptr [ebp + 34h]
        cmp    esi, eax
        jb     SHORT loc_1_8988
        cmp    word ptr [ebx + 0ah], 0
        jne    SHORT loc_1_8988
        cmp    dword ptr [edx + 2ch], 0
        jne    SHORT loc_1_8988
        mov    ecx, dword ptr [ebp + 2ch]
        mov    dword ptr [edi], ecx
loc_1_8966:
        inc    word ptr [ebx + 8]
        mov    edi, dword ptr [edi]
        cmp    dword ptr [edi], 0
        jne    SHORT loc_1_8966
        mov    edx, dword ptr [esp + 30h]
        mov    dword ptr [edx + 2ch], edi
        sub    esi, eax
        mov    dword ptr [ebp + 34h], 0
        jmp    NEAR PTR loc_1_8A60
        db  08Bh, 0FFh
loc_1_8988:
        lea    eax, [ebp + 2ch]
        mov    dword ptr [esp + 44h], eax
        mov    eax, dword ptr [ebp + 2ch]
        mov    dword ptr [esp + 1ch], eax
        mov    dword ptr [esp + 14h], ecx
loc_1_899A:
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esp + 2ch]
        push   eax
        push   dword ptr [esp + 24h]
        call   sub_1_91FC
        mov    eax, dword ptr [esp + 14h]
        sub    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [esp + 18h]
        sub    eax, esi
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, esi
        mov    dword ptr [esp + 10h], eax
        push   eax
        mov    eax, dword ptr [esp + 2ch]
        add    eax, dword ptr [esp + 18h]
        push   eax
        db  0FFh, 035h, 040h, 097h, 000h, 000h
        lea    eax, [esp + 2ch]
        push   eax
        lea    eax, [esp + 34h]
        push   eax
        call   sub_1_91EA
        cmp    dword ptr [esp + 24h], 0
        jne    SHORT loc_1_8A2C
        inc    word ptr [ebx + 0ah]
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [edi], eax
        mov    edi, dword ptr [esp + 20h]
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [esp + 18h], eax
        ja     SHORT loc_1_8A18
        mov    eax, dword ptr [esp + 1ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 1ch], eax
        mov    dword ptr [esp + 14h], 0
        jmp    SHORT loc_1_8A20
        db  090h
loc_1_8A18:
        mov    eax, dword ptr [esp + 10h]
        add    dword ptr [esp + 14h], eax
loc_1_8A20:
        mov    eax, dword ptr [esp + 10h]
        sub    dword ptr [ebp + 34h], eax
        sub    esi, eax
        jmp    SHORT loc_1_8A44
        db  090h
loc_1_8A2C:
        cmp    word ptr [ebx + 0ah], 0
        jne    SHORT loc_1_8A3E
        cmp    word ptr [ebx + 8], 0
        je     NEAR PTR loc_1_8D64
loc_1_8A3E:
        sub    dword ptr [esp + 2ch], esi
        sub    esi, esi
loc_1_8A44:
        or     esi, esi
        je     SHORT loc_1_8A52
        cmp    dword ptr [ebp + 34h], 0
        jne    NEAR PTR loc_1_899A
loc_1_8A52:
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [ebp + 2ch], eax
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [ebp + 30h], eax
loc_1_8A60:
        mov    edx, dword ptr [esp + 30h]
        mov    al, byte ptr [edx + 1ch]
        and    al, 1
        je     SHORT loc_1_8A9F
        mov    edx, dword ptr [ebp + 34h]
        mov    ecx, dword ptr [ebp + 58h]
        and    ecx, 2000h
        or     ecx, ecx
        mov    cx, 0
        sete   cl
        sub    dx, cx
        sub    dx, si
        mov    ecx, dword ptr [esp + 2ch]
        add    cx, dx
        mov    dh, cl
        mov    dl, ch
        mov    ecx, dword ptr [esp + 34h]
        or     word ptr [ecx + 0ch], 2000h
        mov    word ptr [ecx + 12h], dx
loc_1_8A9F:
        cmp    dword ptr [ebp + 34h], 0
        jne    SHORT loc_1_8AF2
        mov    ecx, dword ptr [esp + 34h]
        or     word ptr [ecx + 0ch], 800h
        mov    edx, dword ptr [esp + 30h]
        mov    ecx, dword ptr [edx]
        mov    edx, ebp
        sub    edx, ecx
        cmp    edx, -38h
        je     SHORT loc_1_8AE4
        mov    dword ptr [esp + 30h], ecx
        mov    edx, dword ptr [ecx + 20h]
        mov    dword ptr [ebp + 34h], edx
        mov    edx, dword ptr [ecx + 24h]
        mov    dword ptr [ebp + 30h], edx
        mov    edx, dword ptr [ecx + 28h]
        mov    dword ptr [ebp + 2ch], edx
        mov    dword ptr [ebp + 28h], ecx
        or     al, al
        je     SHORT loc_1_8AF2
        test   byte ptr [ecx + 1ch], 1
        jne    SHORT loc_1_8AF2
        jmp    SHORT loc_1_8B16
        db  090h
loc_1_8AE4:
        mov    dword ptr [ebp + 28h], 0
        mov    dword ptr [ebp + 2ch], 0
loc_1_8AF2:
        or     esi, esi
        jne    NEAR PTR loc_1_893C
        jmp    SHORT loc_1_8B16
loc_1_8AFC:
        test   byte ptr [ebp + 5ah], 1
        je     NEAR PTR loc_1_8DA4
        mov    dword ptr [ebx], 0
        mov    eax, dword ptr [esp + 38h]
        mov    dword ptr [eax], 0
loc_1_8B16:
        sub    dword ptr [esp + 2ch], esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esp + 2ch]
        lea    eax, [edx + esi]
        mov    dword ptr [ebp + 8], eax
        cmp    dword ptr [ebp + 0ch], esi
        jne    SHORT loc_1_8B40
        mov    dword ptr [ebp + 0ch], eax
        db  0FFh, 005h, 068h, 095h, 000h, 000h
        cmp    dword ptr [ebp + 8ch], 0
        jne    SHORT loc_1_8B73
        jmp    SHORT loc_1_8B65
loc_1_8B40:
        db  0FFh, 005h, 06Ch, 095h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        sub    eax, dword ptr [ebp + 0ch]
        or     eax, eax
        jle    SHORT loc_1_8B73
        db  0FFh, 005h, 068h, 095h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp + 0ch], eax
        cmp    dword ptr [ebp + 8ch], 0
        jne    SHORT loc_1_8B73
loc_1_8B65:
        db  0A1h, 06Ch, 096h, 000h, 000h
        mov    dword ptr [ebp + 8ch], eax
        mov    dword ptr [ebp + 78h], esi
loc_1_8B73:
        mov    ecx, dword ptr [esp + 2ch]
        cmp    ecx, dword ptr [esp + 40h]
        jne    SHORT loc_1_8BB4
        mov    edi, dword ptr [ebp + 58h]
        test   edi, 10000h
        je     SHORT loc_1_8BB4
        cmp    dword ptr [esp + 3ch], edx
        jle    SHORT loc_1_8BB4
        mov    ecx, dword ptr [esp + 34h]
        or     word ptr [ecx + 0ch], 100h
        mov    eax, dword ptr [ebp + 8]
        inc    eax
        mov    dword ptr [ebp + 8], eax
        mov    dword ptr [ebp + 0ch], eax
        or     edi, 30000000h
        mov    dword ptr [ebp + 58h], edi
        and    edi, 0fffeffffh
        mov    dword ptr [ebp + 58h], edi
loc_1_8BB4:
        sub    eax, eax
        add    dword ptr [esp + 2ch], 14h
        push   dword ptr [esp + 38h]
        mov    al, byte ptr [esp + 31h]
        mov    ah, byte ptr [esp + 30h]
        add    eax, dword ptr [ebp + 24h]
        push   eax
        call   sub_1_1D1C
        add    esp, 8
        not    ax
        mov    ecx, dword ptr [esp + 34h]
        mov    word ptr [ecx + 10h], ax
        cmp    word ptr [ebp + 90h], 0
        db  0A1h, 06Ch, 096h, 000h, 000h
        jne    SHORT loc_1_8BF8
        mov    cx, word ptr [ebp + 72h]
        mov    word ptr [ebp + 90h], cx
loc_1_8BF8:
        mov    word ptr [ebp + 92h], 0
        and    dword ptr [ebp + 58h], 0ffbff5ffh
        mov    word ptr [ebp + 0dah], 0
        mov    dword ptr [ebp + 98h], eax
        push   dword ptr [ebp + 0f0h]
        lea    eax, [ebp + 0e4h]
        push   eax
        push   dword ptr [ebp + 68h]
        push   dword ptr [ebp + 64h]
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 4ch]
        push   ebx
        db  0FFh, 035h, 0D8h, 095h, 000h, 000h
        db  0FFh, 015h, 098h, 095h, 000h, 000h
        add    esp, 20h
        mov    edi, eax
        mov    dword ptr [ebp + 88h], edi
        cmp    edi, 2bf7h
        je     NEAR PTR loc_1_8CDF
        push   dword ptr [esp + 38h]
        push   ebx
        call   sub_1_7D24
        add    esp, 8
        or     edi, edi
        je     SHORT loc_1_8CDF
        mov    eax, esi
        sub    eax, dword ptr [ebp + 4]
        js     SHORT loc_1_8C7F
        mov    eax, esi
        sub    eax, dword ptr [ebp + 8]
        jns    SHORT loc_1_8C7F
        push   esi
        push   ebp
        call   sub_1_5534
        add    esp, 8
loc_1_8C7F:
        cmp    word ptr [ebp + 90h], 0
        jne    SHORT loc_1_8C94
        mov    ax, word ptr [ebp + 72h]
        mov    word ptr [ebp + 90h], ax
loc_1_8C94:
        cmp    edi, 2b01h
        jne    SHORT loc_1_8CE9
        push   0
        lea    eax, [esp + 4ch]
        push   eax
        push   dword ptr [ebp + 68h]
        push   dword ptr [ebp + 64h]
        db  0FFh, 015h, 0B0h, 095h, 000h, 000h
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_8CE9
        movzx  eax, byte ptr [ebp + 0ech]
        sub    eax, 14h
        add    dword ptr [esp + 48h], eax
        mov    ax, word ptr [ebp + 80h]
        mov    ecx, dword ptr [esp + 48h]
        sub    ax, cx
        sbb    dx, dx
        and    ax, dx
        add    ax, cx
        mov    word ptr [ebp + 70h], ax
loc_1_8CDF:
        test   byte ptr [ebp + 5bh], 10h
        je     NEAR PTR loc_1_885A
loc_1_8CE9:
        and    dword ptr [ebp + 58h], 0ff3bfbffh
        jmp    NEAR PTR loc_1_8DC0
        db  08Dh, 049h, 000h
loc_1_8CF8:
        cmp    dword ptr [ebp + 10h], 0
        jne    SHORT loc_1_8D48
        mov    eax, dword ptr [ebp + 58h]
        test   eax, 80000h
        jne    SHORT loc_1_8D30
        or     eax, 80000h
        mov    dword ptr [ebp + 58h], eax
        mov    byte ptr [ebp + 85h], 0
        inc    byte ptr [ebp + 82h]
        mov    ax, word ptr [ebp + 72h]
        or     dword ptr [ebp + 5ch], 1
        mov    word ptr [ebp + 90h], ax
        jmp    SHORT loc_1_8CE9
        db  08Bh, 0FFh
loc_1_8D30:
        cmp    word ptr [ebp + 90h], 0
        jne    SHORT loc_1_8CE9
        mov    ax, word ptr [ebp + 72h]
        mov    word ptr [ebp + 90h], ax
        jmp    SHORT loc_1_8CE9
        db  090h
loc_1_8D48:
        cmp    dword ptr [esp + 2ch], 0
        je     SHORT loc_1_8CE9
        cmp    word ptr [ebp + 0dah], 0
        jne    SHORT loc_1_8CE9
        mov    word ptr [ebp + 0dah], 32h
        jmp    SHORT loc_1_8CE9
loc_1_8D64:
        push   dword ptr [esp + 38h]
        push   ebx
        call   sub_1_7D24
        add    esp, 8
loc_1_8D71:
        cmp    word ptr [ebp + 90h], 0
        jne    SHORT loc_1_8D86
        mov    ax, word ptr [ebp + 72h]
        mov    word ptr [ebp + 90h], ax
loc_1_8D86:
        and    dword ptr [ebp + 58h], 0ffbbfbffh
        push   dword ptr [esp + 4ch]
        push   ebp
        call   sub_1_63D8
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 40h
        ret
        db  08Bh, 0FFh
loc_1_8DA4:
        push   dword ptr [esp + 38h]
        call   sub_1_7CB8
        add    esp, 4
        jmp    NEAR PTR loc_1_8CE9
        db  08Dh, 049h, 000h
loc_1_8DB8:
        or     eax, 800000h
        mov    dword ptr [ebp + 58h], eax
loc_1_8DC0:
        push   dword ptr [esp + 4ch]
        push   ebp
        call   sub_1_63D8
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 40h
        ret
        db  08Dh, 049h, 000h
loc_1_8DD8:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 14h]
        push   dword ptr [ebx]
        call   sub_1_5F38
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_8F28
        mov    esi, dword ptr [eax]
        or     esi, esi
        je     NEAR PTR loc_1_8F1C
        mov    al, byte ptr [esi + 84h]
        cmp    al, 4
        je     SHORT loc_1_8E0E
        cmp    al, 7
        jne    NEAR PTR loc_1_8F10
loc_1_8E0E:
        test   byte ptr [esi + 86h], 1
        jne    NEAR PTR loc_1_8F10
        mov    edi, dword ptr [esp + 1ch]
        or     edi, edi
        je     NEAR PTR loc_1_8F08
        call   sub_1_7CE8
        or     eax, eax
        je     NEAR PTR loc_1_8EFC
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
        db  08Bh, 00Dh, 06Ch, 096h, 000h, 000h
        mov    dword ptr [eax + 30h], ecx
        test   byte ptr [esp + 18h], 20h
        setne  cl
        mov    byte ptr [eax + 1ch], cl
        mov    ebx, dword ptr [esi + 14h]
        add    ebx, edi
        mov    dword ptr [esi + 14h], ebx
        mov    ecx, dword ptr [esi + 38h]
        sub    ecx, esi
        cmp    ecx, 38h
        mov    ecx, 0
        sete   cl
        mov    ebp, dword ptr [esi + 3ch]
        mov    dword ptr [ebp], eax
        mov    dword ptr [eax + 4], ebp
        mov    dword ptr [esi + 3ch], eax
        lea    ebp, [esi + 38h]
        mov    dword ptr [eax], ebp
        cmp    dword ptr [esi + 28h], 0
        jne    SHORT loc_1_8EB5
        mov    dword ptr [esi + 28h], eax
        mov    dword ptr [esi + 2ch], edx
        mov    dword ptr [esi + 30h], 0
        mov    dword ptr [esi + 34h], edi
loc_1_8EB5:
        or     ecx, ecx
        je     SHORT loc_1_8ED0
        inc    dword ptr [esi + 74h]
        push   esi
        call   sub_1_8824
        add    esp, 4
        mov    eax, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_8ED0:
        test   byte ptr [esi + 5ah], 2
        je     SHORT loc_1_8EE6
        mov    ecx, dword ptr [esi + 4]
        sub    ecx, dword ptr [esi + 0ch]
        add    ecx, ebx
        movzx  eax, word ptr [esi + 70h]
        cmp    ecx, eax
        jb     SHORT loc_1_8EF2
loc_1_8EE6:
        push   esi
        inc    dword ptr [esi + 74h]
        call   sub_1_8824
        add    esp, 4
loc_1_8EF2:
        pop    ebp
        mov    eax, 0ffh
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_8EFC:
        pop    ebp
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_8F08:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_8F10:
        pop    ebp
        mov    eax, 19h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_8F1C:
        pop    ebp
        mov    eax, 19h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_8F28:
        pop    ebp
        mov    eax, 0fh
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_8F34:
        xchg   al, ah
        ror    eax, 10h
        xchg   al, ah
        ror    eax, 10h
        mov    ecx, eax
        ror    ecx, 10h
        add    eax, ecx
        shr    eax, 10h
        ret
sub_1_8F49:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 10h]
        jecxz  loc_1_8FC4
        test   esi, 3
        je     SHORT loc_1_8FA4
        test   esi, 1
        je     SHORT loc_1_8F8F
        movzx  edx, byte ptr [esi]
        add    eax, edx
        adc    eax, 0
        db  0BAh, 034h, 08Fh, 000h, 000h
        xchg   dword ptr [esp], edx
        push   edx
        xchg   al, ah
        ror    eax, 10h
        xchg   al, ah
        ror    eax, 10h
        inc    esi
        dec    ecx
        je     SHORT loc_1_8FC4
        test   esi, 3
        je     SHORT loc_1_8FA4
loc_1_8F8F:
        cmp    ecx, 1
        je     SHORT loc_1_8FF1
        movzx  edx, word ptr [esi]
        add    eax, edx
        adc    eax, 0
        add    esi, 2
        sub    ecx, 2
        je     SHORT loc_1_8FC4
loc_1_8FA4:
        cmp    ecx, 4
        jb     SHORT loc_1_8FC0
        mov    edx, ecx
        shr    ecx, 2
        and    dl, 3
        mov    eax, eax
loc_1_8FB4:
        add    eax, dword ptr [esi]
        adc    eax, 0
        add    esi, 4
        loop   loc_1_8FB4
        mov    ecx, edx
loc_1_8FC0:
        test   cl, cl
        jne    SHORT loc_1_8FD0
loc_1_8FC4:
        pop    esi
        mov    ecx, eax
        ror    ecx, 10h
        add    eax, ecx
        shr    eax, 10h
        ret
loc_1_8FD0:
        cmp    cl, 1
        je     SHORT loc_1_8FF1
        movzx  edx, word ptr [esi]
        add    eax, edx
        adc    eax, 0
        add    esi, 2
        sub    cl, 2
        jne    SHORT loc_1_8FF1
        pop    esi
        mov    ecx, eax
        ror    ecx, 10h
        add    eax, ecx
        shr    eax, 10h
        ret
loc_1_8FF1:
        movzx  edx, byte ptr [esi]
        add    eax, edx
        adc    eax, 0
        pop    esi
        mov    ecx, eax
        ror    ecx, 10h
        add    eax, ecx
        shr    eax, 10h
        ret
        db  000h, 000h, 000h
sub_1_9008:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        db  0BFh, 0ECh, 092h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    word ptr [ebp - 14h], cx
        mov    ax, cx
        mov    word ptr [ebp - 12h], ax
        db  0C7h, 045h, 0F0h, 0ECh, 092h, 000h, 000h
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_9172
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_9054
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_9054:
        db  0BEh, 004h, 094h, 000h, 000h
loc_1_9059:
        mov    edi, dword ptr [esi]
        mov    ebx, dword ptr [esi + 8]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    word ptr [ebp - 14h], cx
        mov    ax, cx
        mov    word ptr [ebp - 12h], ax
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 10h], eax
        push   0
        lea    eax, [ebp - 14h]
        push   eax
        push   dword ptr [ebp - 0ch]
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_9232
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_90A0
        cmp    dword ptr [esi + 0ch], 0
        jne    SHORT loc_1_90C8
        mov    eax, dword ptr [esi + 4]
        jmp    SHORT loc_1_90A6
loc_1_90A0:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
loc_1_90A6:
        mov    dword ptr [ebx], eax
        add    esi, 10h
        db  081h, 0FEh, 0B4h, 094h, 000h, 000h
        jb     SHORT loc_1_9059
        push   dword ptr [ebp - 0ch]
        call   sub_1_9238
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_90C8:
        push   dword ptr [ebp - 0ch]
        call   sub_1_9238
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
sub_1_90D8:
        db  0CDh, 020h, 001h, 080h, 088h, 004h
sub_1_90DE:
        db  0CDh, 020h, 002h, 080h, 088h, 004h
sub_1_90E4:
        db  0CDh, 020h, 03Fh, 080h, 001h, 000h
sub_1_90EA:
        db  0CDh, 020h, 003h, 080h, 088h, 004h
        db  0CDh, 020h, 004h, 080h, 088h, 004h
sub_1_90F6:
        db  0CDh, 020h, 00Dh, 080h, 040h, 000h
sub_1_90FC:
        db  0CDh, 020h, 00Eh, 080h, 040h, 000h
sub_1_9102:
        db  0CDh, 020h, 011h, 080h, 040h, 000h
        db  0CDh, 020h, 005h, 080h, 088h, 004h
        db  0CDh, 020h, 00Dh, 080h, 088h, 004h
        db  0CDh, 020h, 006h, 080h, 088h, 004h
sub_1_911A:
        db  0A1h, 0B4h, 094h, 000h, 000h
        db  0CDh, 020h, 009h, 080h, 088h, 004h
        db  0A1h, 0B4h, 094h, 000h, 000h
        db  0CDh, 020h, 00Ch, 080h, 088h, 004h
        db  0A1h, 0B4h, 094h, 000h, 000h
        db  0CDh, 020h, 00Bh, 080h, 088h, 004h
        add    ch, cl
        and    byte ptr [ecx], ch
        sub    byte ptr [eax], 0
        db  0CDh, 020h, 02Ah, 080h, 028h, 000h
        db  0CDh, 020h, 02Bh, 080h, 028h, 000h
        db  0CDh, 020h, 02Ch, 080h, 028h, 000h
        db  0CDh, 020h, 02Dh, 080h, 028h, 000h
        db  0CDh, 020h, 02Eh, 080h, 028h, 000h
        db  0CDh, 020h, 02Fh, 080h, 028h, 000h
        db  0CDh, 020h, 030h, 080h, 028h, 000h
        db  0CDh, 020h, 050h, 080h, 028h, 000h
sub_1_9172:
        db  0CDh, 020h, 051h, 080h, 028h, 000h
        db  0CDh, 020h, 052h, 080h, 028h, 000h
        db  0CDh, 020h, 053h, 080h, 028h, 000h
        db  0CDh, 020h, 000h, 080h, 028h, 000h
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
        db  0CDh, 020h, 010h, 080h, 028h, 000h
        db  0CDh, 020h, 011h, 080h, 028h, 000h
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
sub_1_91DE:
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
sub_1_91E4:
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
sub_1_91EA:
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
sub_1_91F6:
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_91FC:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
        db  0CDh, 020h, 027h, 080h, 028h, 000h
        db  0CDh, 020h, 028h, 080h, 028h, 000h
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_9232:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_9238:
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
        db  012h, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 00Ah, 003h, 08Ah, 004h, 003h, 000h, 000h, 000h, 04Dh, 053h, 054h, 043h
        db  050h, 020h, 020h, 020h, 000h, 0A0h, 000h, 054h, 018h, 000h, 000h, 000h, 02Ch, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  094h, 092h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  0FFh, 000h, 000h, 000h
loc_1_92EC:
        dec    ebp
        push   ebx
        push   esp
        inc    ebx
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
loc_1_931C:
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
loc_1_9328:
        js     SHORT loc_1_932D
        add    byte ptr [eax], al
        db  0E8h
loc_1_932D:
        add    al, byte ptr [eax]
        db  000h
loc_1_9330:
        mov    word ptr [eax + eax], ss
        in     al, dx
        push   ebx
        add    byte ptr [eax], al
loc_1_9338:
        inc    eax
        add    dword ptr [eax], eax
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
loc_1_9344:
        inc    esp
        popad
        inc    edi
        push   edi
        inc    esp
        db  065h, 074h, 065h
        arpl   word ptr [eax + eax], si
        add    byte ptr [eax], al
loc_1_9354:
        push   eax
        dec    ebp
        push   esp
        push   ebp
        inc    esp
        imul   esi, dword ptr [ebx + 63h], 7265766fh
        jns    SHORT loc_1_9362
loc_1_9362:
        add    byte ptr [eax], al
loc_1_9364:
        push   eax
        dec    ebp
        push   esp
        push   ebp
        inc    edx
        insb
        popad
        arpl   word ptr [ebx + 48h], bp
        outsd
        insb
        inc    esp
        db  065h, 074h, 065h
        db  063h, 074h, 000h
loc_1_9378:
        dec    ebx
        db  065h, 065h, 070h, 041h
        insb
        imul   esi, dword ptr [esi + 65h], 656d6954h
        add    byte ptr [eax], al
        add    byte ptr [ebx + 65h], cl
        jo     NEAR PTR loc_1_93CE
        insb
        imul   esi, dword ptr [esi + 65h], 65746e49h
        db  072h, 076h
        popad
        insb
        add    byte ptr [eax], al
        add    byte ptr [ebp + 66h], al
        popad
        db  075h, 06Ch
        je     SHORT loc_1_93F6
        arpl   word ptr [esi + 57h], si
        imul   ebp, dword ptr [esi + 64h], 776fh
        add    byte ptr [eax], al
loc_1_93B0:
        dec    ebp
        popad
        db  078h, 043h
        outsd
        outsb
        outsb
        arpl   word ptr gs:[ecx + ebp*2 + 6fh], si
        outsb
        jae    SHORT loc_1_93BF
loc_1_93BF:
        add    byte ptr [ebp + 61h], cl
        js     SHORT loc_1_9407
        outsd
        outsb
        outsb
        arpl   word ptr gs:[edx + edx*2 + 65h], si
        je     SHORT loc_1_9440
loc_1_93CE:
        imul   esp, dword ptr [ebp + 73h], 4d000000h
        popad
        db  078h, 044h
        popad
        db  074h, 061h
        push   edx
        db  065h, 074h, 072h
        imul   esp, dword ptr [ebp + 73h], 53420000h
        inc    esp
        push   ebp
        db  072h, 067h
        outsb
        je     SHORT loc_1_93EE
loc_1_93EE:
        add    byte ptr [eax], al
loc_1_93F0:
        push   eax
        jb     SHORT loc_1_9458
        insb
        outsd
        popad
loc_1_93F6:
        dec    eax
        popad
        db  070h, 043h
        outsd
        jne    SHORT loc_1_946D
        je     SHORT loc_1_9401
loc_1_9401:
        add    byte ptr [eax], al
        db  000h, 044h, 093h, 000h
loc_1_9407:
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        db  000h, 064h, 096h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 054h, 093h, 000h
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        db  000h, 000h
        xchg   esi, eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        xchg   ebx, eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        cwde
        xchg   esi, eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  078h, 093h
        add    byte ptr [eax], al
        add    ch, bl
        insd
        db  000h, 01Ch, 096h
        add    byte ptr [eax], al
loc_1_9440:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        mov    byte ptr [ebx + 3e80000h], dl
        add    byte ptr [eax], al
        cld
        xchg   ebp, eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        pushfd
        xchg   ebx, eax
        add    byte ptr [eax], al
loc_1_9458:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   96h
        add    byte ptr [eax], al
        db  000h, 0B0h, 093h, 000h, 000h, 064h
        add    byte ptr [eax], al
        db  000h, 0E8h
loc_1_946D:
        xchg   ebp, eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rcl    byte ptr [ebx + 30000h], 0
        db  000h, 04Ch, 096h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0D4h
        xchg   ebx, eax
        add    byte ptr [eax], al
        db  005h, 000h, 000h, 000h, 054h
        xchg   esi, eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        in     al, 93h
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        and    al, 96h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0F0h, 093h, 000h, 000h, 000h, 000h, 000h, 000h, 004h, 096h, 000h, 000h, 000h, 000h, 000h, 000h
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
        db  000h, 000h, 000h, 000h
_LTEXT ends
        end
