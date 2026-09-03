; wsvv_lcode_01_04.asm -- WSVV object 1 segment _LTEXT  (part 4/5)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_E730:
        db  0C7h, 005h, 034h, 039h, 001h, 000h, 0A4h, 036h, 001h, 000h
        mov    eax, dword ptr [esp + 8]
        db  0A3h, 0B4h, 036h, 001h, 000h
        db  0FFh, 035h, 034h, 039h, 001h, 000h
        call   sub_1_E75D
        add    esp, 4
        or     eax, eax
        mov    eax, 0fffffef9h
        jne    SHORT loc_1_E75C
        sub    eax, eax
loc_1_E75C:
        ret
sub_1_E75D:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        mov    esi, dword ptr [esp + 18h]
        sub    ebx, ebx
        cmp    dword ptr [esi], ebx
        je     SHORT loc_1_E775
        jmp    NEAR PTR loc_1_E835
loc_1_E775:
        push   esi
        db  068h, 092h, 0E6h, 000h, 000h
        call   sub_1_F870
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_E78C
        jmp    NEAR PTR loc_1_E835
loc_1_E78C:
        mov    dword ptr [esi + 1ch], 0afc8h
        lea    eax, [esi + 14h]
        push   eax
        call   sub_1_EE98
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_E7A8
        jmp    NEAR PTR loc_1_E835
loc_1_E7A8:
        cmp    dword ptr [esi], 0
        jne    SHORT loc_1_E7B2
        jmp    NEAR PTR loc_1_E835
loc_1_E7B2:
        mov    dword ptr [esi + 4], 0
        lea    eax, [esp + 10h]
        push   eax
        call   sub_1_F845
        add    esp, 4
        mov    ebp, eax
        cmp    ebp, -2
        je     SHORT loc_1_E7FD
loc_1_E7CD:
        push   0
        push   0
        push   ebp
        push   12h
        call   sub_1_ED99
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_E7E8
        mov    ebx, 1
        add    dword ptr [esi + 8], ebx
loc_1_E7E8:
        lea    eax, [esp + 10h]
        push   eax
        push   ebp
        call   sub_1_F7D0
        add    esp, 8
        mov    ebp, eax
        cmp    ebp, -2
        jne    SHORT loc_1_E7CD
loc_1_E7FD:
        or     ebx, ebx
        je     SHORT loc_1_E81E
        mov    eax, dword ptr [esi + 8]
        add    eax, 9
        imul   eax, eax, 1f4h
        mov    dword ptr [esi + 1ch], eax
        lea    eax, [esi + 14h]
        push   eax
        call   sub_1_EE98
        add    esp, 4
        jmp    SHORT loc_1_E829
loc_1_E81E:
        mov    dword ptr [esi], 0
        mov    edi, 0fffffefbh
loc_1_E829:
        cmp    dword ptr [esi], 0
        mov    eax, edi
        jne    SHORT loc_1_E83A
        call   sub_1_F05F
loc_1_E835:
        mov    eax, 0fffffef9h
loc_1_E83A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_E842:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        mov    esi, dword ptr [esp + 18h]
        sub    ebx, ebx
        cmp    dword ptr [esi], ebx
        jne    SHORT loc_1_E85C
        mov    eax, 0fffffef9h
        jmp    SHORT loc_1_E8AE
loc_1_E85C:
        lea    eax, [esp + 10h]
        push   eax
        call   sub_1_F845
        add    esp, 4
        mov    ebp, eax
        cmp    ebp, -2
        je     SHORT loc_1_E89D
loc_1_E870:
        push   0
        push   0
        push   ebp
        push   12h
        call   sub_1_ED99
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_E888
        mov    ebx, 1
loc_1_E888:
        lea    eax, [esp + 10h]
        push   eax
        push   ebp
        call   sub_1_F7D0
        add    esp, 8
        mov    ebp, eax
        cmp    ebp, -2
        jne    SHORT loc_1_E870
loc_1_E89D:
        or     ebx, ebx
        jne    SHORT loc_1_E8AC
        mov    dword ptr [esi], 0
        mov    edi, 0fffffefbh
loc_1_E8AC:
        mov    eax, edi
loc_1_E8AE:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_E8C0:
        push   esi
        db  08Bh, 035h, 034h, 039h, 001h, 000h
        push   dword ptr [esp + 0ch]
        call   sub_1_EE10
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_E8DE
        mov    eax, 0fffffefdh
        jmp    SHORT loc_1_E90D
loc_1_E8DE:
        mov    dword ptr [eax + 20h], 1
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [eax + 1ch], ecx
        mov    ecx, dword ptr [esi + 10h]
        or     ecx, ecx
        mov    dword ptr [eax + 24h], ecx
        je     SHORT loc_1_E90B
        push   0
        push   0
        push   0e9h
        push   dword ptr [eax + 4]
        call   ecx
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_E90D
loc_1_E90B:
        sub    eax, eax
loc_1_E90D:
        pop    esi
        ret
sub_1_E90F:
        push   esi
        push   edi
        sub    edi, edi
        db  08Bh, 035h, 034h, 039h, 001h, 000h
        push   dword ptr [esp + 10h]
        call   sub_1_EE10
        add    esp, 4
        cmp    eax, edi
        jne    SHORT loc_1_E930
        mov    eax, 2b09h
        jmp    SHORT loc_1_E954
loc_1_E930:
        mov    edx, dword ptr [esi + 10h]
        or     edx, edx
        je     SHORT loc_1_E952
        push   dword ptr [esp + 18h]
        push   0
        push   0ech
        push   dword ptr [eax + 4]
        call   edx
        add    esp, 10h
        mov    edi, eax
        or     edi, edi
        mov    eax, edi
        jne    SHORT loc_1_E954
loc_1_E952:
        mov    eax, edi
loc_1_E954:
        pop    edi
        pop    esi
        ret
sub_1_E957:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esi + 4]
        cmp    ecx, -1fh
        jae    SHORT loc_1_E988
        push   esi
        push   8
        lea    eax, [esi + 8]
        push   eax
        lea    eax, [esi + 10h]
        push   eax
        push   ecx
        call   sub_1_F3E8
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_1_E988
        push   eax
        call   sub_1_EE44
        add    esp, 4
        jmp    SHORT loc_1_E9D6
loc_1_E988:
        lea    edi, [esi + 14h]
        lea    eax, [esi + 8]
        push   eax
        push   esi
        db  068h, 080h, 0DEh, 000h, 000h
        push   edi
        push   dword ptr [esi + 4]
        call   sub_1_101B3
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_1_E9B0
        push   eax
        call   sub_1_EE4A
        add    esp, 4
        jmp    SHORT loc_1_E9D6
loc_1_E9B0:
        push   dword ptr [esi + 59h]
        push   dword ptr [esi + 38h]
        push   dword ptr [edi]
        call   sub_1_107D2
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_E9CF
        push   eax
        call   sub_1_EE4A
        add    esp, 4
        jmp    SHORT loc_1_E9D6
loc_1_E9CF:
        mov    dword ptr [esi + 18h], 1
loc_1_E9D6:
        pop    edi
        pop    esi
        ret
sub_1_E9D9:
        push   dword ptr [esp + 8]
        call   sub_1_EE10
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_E9F0
        mov    eax, 0fffffefeh
        jmp    SHORT loc_1_EA11
loc_1_E9F0:
        db  08Bh, 00Dh, 034h, 039h, 001h, 000h
        mov    ecx, dword ptr [ecx + 10h]
        or     ecx, ecx
        je     SHORT loc_1_EA0F
        push   dword ptr [eax + 1ch]
        push   0
        push   0f0h
        push   dword ptr [eax + 4]
        call   ecx
        add    esp, 10h
loc_1_EA0F:
        sub    eax, eax
loc_1_EA11:
        ret
sub_1_EA12:
        push   esi
        push   dword ptr [esp + 0ch]
        call   sub_1_EE10
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_EA2C
        mov    eax, 0fffffefeh
        jmp    SHORT loc_1_EA5A
loc_1_EA2C:
        db  0A1h, 034h, 039h, 001h, 000h
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        je     SHORT loc_1_EA4A
        push   dword ptr [esi + 1ch]
        push   0
        push   0fch
        push   dword ptr [esi + 4]
        call   eax
        add    esp, 10h
loc_1_EA4A:
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 1ch], 0
        sub    eax, eax
loc_1_EA5A:
        pop    esi
        ret
sub_1_EA5C:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   dword ptr [esi + 14h]
        call   sub_1_FBE0
        add    esp, 4
        push   dword ptr [esi + 10h]
        call   sub_1_F483
        add    esp, 4
        mov    dword ptr [esi + 10h], 0
        mov    dword ptr [esi + 14h], 0
        push   esi
        call   sub_1_EB85
        add    esp, 4
        pop    esi
        ret
sub_1_EA90:
        push   esi
        push   edi
        db  083h, 03Dh, 028h, 037h, 001h, 000h, 000h
        je     NEAR PTR loc_1_EB2C
        db  0A1h, 028h, 037h, 001h, 000h
        db  08Bh, 034h, 085h, 028h, 037h, 001h, 000h
        mov    edx, 0
        jbe    SHORT loc_1_EAC7
        db  0B8h, 02Ch, 037h, 001h, 000h
loc_1_EAB7:
        cmp    dword ptr [eax], esi
        je     SHORT loc_1_EAFE
        add    eax, 4
        inc    edx
        db  03Bh, 015h, 028h, 037h, 001h, 000h
        jb     SHORT loc_1_EAB7
loc_1_EAC7:
        db  083h, 03Dh, 0ECh, 036h, 001h, 000h, 00Ah
        jae    SHORT loc_1_EAE2
        db  0A1h, 0ECh, 036h, 001h, 000h
        db  089h, 034h, 085h, 0F0h, 036h, 001h, 000h
        db  0FFh, 005h, 0ECh, 036h, 001h, 000h
loc_1_EAE2:
        push   esi
        call   sub_1_ED66
        add    esp, 4
        mov    dword ptr [esi + 8], 8
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [esi + 4], eax
        mov    eax, esi
loc_1_EAFB:
        pop    edi
        pop    esi
        ret
loc_1_EAFE:
        db  0A1h, 028h, 037h, 001h, 000h
        dec    eax
        cmp    eax, edx
        jbe    SHORT loc_1_EB24
        db  08Dh, 03Ch, 095h, 02Ch, 037h, 001h, 000h
loc_1_EB0F:
        mov    ecx, dword ptr [edi + 4]
        lea    eax, [edi + 4]
        mov    dword ptr [edi], ecx
        mov    edi, eax
        inc    edx
        db  0A1h, 028h, 037h, 001h, 000h
        dec    eax
        cmp    eax, edx
        ja     SHORT loc_1_EB0F
loc_1_EB24:
        db  0FFh, 00Dh, 028h, 037h, 001h, 000h
        jmp    SHORT loc_1_EAC7
loc_1_EB2C:
        push   61h
        call   sub_1_F390
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_EB40
        sub    eax, eax
        jmp    SHORT loc_1_EAFB
loc_1_EB40:
        mov    dword ptr [esi + 59h], 0c350h
        push   0c350h
        call   sub_1_F390
        add    esp, 4
        mov    dword ptr [esi + 38h], eax
        mov    edx, dword ptr [esi + 59h]
        mov    edi, eax
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        cmp    dword ptr [esi + 38h], eax
        jne    NEAR PTR loc_1_EAC7
        push   esi
        call   sub_1_EC1B
        add    esp, 4
        sub    eax, eax
        jmp    NEAR PTR loc_1_EAFB
sub_1_EB85:
        push   esi
        push   edi
        sub    edx, edx
        db  039h, 015h, 0ECh, 036h, 001h, 000h
        jbe    NEAR PTR loc_1_EC18
        db  0B8h, 0F0h, 036h, 001h, 000h
        mov    ecx, dword ptr [esp + 0ch]
loc_1_EB9E:
        cmp    dword ptr [eax], ecx
        je     SHORT loc_1_EBB0
        add    eax, 4
        inc    edx
        db  03Bh, 015h, 0ECh, 036h, 001h, 000h
        jb     SHORT loc_1_EB9E
        jmp    SHORT loc_1_EC18
loc_1_EBB0:
        sub    esi, esi
        db  039h, 035h, 0ECh, 036h, 001h, 000h
        jbe    SHORT loc_1_EBFD
        db  0B8h, 0F0h, 036h, 001h, 000h
loc_1_EBBF:
        cmp    dword ptr [eax], ecx
        je     SHORT loc_1_EBD1
        add    eax, 4
        inc    esi
        db  03Bh, 035h, 0ECh, 036h, 001h, 000h
        jb     SHORT loc_1_EBBF
        jmp    SHORT loc_1_EBFD
loc_1_EBD1:
        db  0A1h, 0ECh, 036h, 001h, 000h
        dec    eax
        cmp    eax, esi
        jbe    SHORT loc_1_EBF7
        db  08Dh, 03Ch, 0B5h, 0F0h, 036h, 001h, 000h
loc_1_EBE2:
        mov    edx, dword ptr [edi + 4]
        lea    eax, [edi + 4]
        mov    dword ptr [edi], edx
        mov    edi, eax
        inc    esi
        db  0A1h, 0ECh, 036h, 001h, 000h
        dec    eax
        cmp    eax, esi
        ja     SHORT loc_1_EBE2
loc_1_EBF7:
        db  0FFh, 00Dh, 0ECh, 036h, 001h, 000h
loc_1_EBFD:
        db  083h, 03Dh, 028h, 037h, 001h, 000h, 00Ah
        jae    SHORT loc_1_EC18
        db  0A1h, 028h, 037h, 001h, 000h
        db  089h, 00Ch, 085h, 02Ch, 037h, 001h, 000h
        db  0FFh, 005h, 028h, 037h, 001h, 000h
loc_1_EC18:
        pop    edi
        pop    esi
        ret
sub_1_EC1B:
        push   esi
        push   edi
        sub    ecx, ecx
        db  039h, 00Dh, 0ECh, 036h, 001h, 000h
        jbe    SHORT loc_1_EC42
        db  0B8h, 0F0h, 036h, 001h, 000h
        mov    esi, dword ptr [esp + 0ch]
loc_1_EC30:
        cmp    dword ptr [eax], esi
        je     SHORT loc_1_EC6E
        add    eax, 4
        inc    ecx
        db  03Bh, 00Dh, 0ECh, 036h, 001h, 000h
        jb     SHORT loc_1_EC30
        jmp    SHORT loc_1_EC46
loc_1_EC42:
        mov    esi, dword ptr [esp + 0ch]
loc_1_EC46:
        sub    eax, eax
        db  039h, 005h, 028h, 037h, 001h, 000h
        jbe    NEAR PTR loc_1_ED63
        db  0B9h, 02Ch, 037h, 001h, 000h
loc_1_EC59:
        cmp    dword ptr [ecx], esi
        je     SHORT loc_1_EC6E
        add    ecx, 4
        inc    eax
        db  039h, 005h, 028h, 037h, 001h, 000h
        ja     SHORT loc_1_EC59
        jmp    NEAR PTR loc_1_ED63
loc_1_EC6E:
        mov    eax, dword ptr [esi + 38h]
        or     eax, eax
        je     SHORT loc_1_EC7E
        push   eax
        call   sub_1_F396
        add    esp, 4
loc_1_EC7E:
        sub    ecx, ecx
        db  039h, 00Dh, 0ECh, 036h, 001h, 000h
        jbe    SHORT loc_1_ECEC
        db  0B8h, 0F0h, 036h, 001h, 000h
loc_1_EC8D:
        cmp    dword ptr [eax], esi
        je     SHORT loc_1_EC9F
        add    eax, 4
        inc    ecx
        db  03Bh, 00Dh, 0ECh, 036h, 001h, 000h
        jb     SHORT loc_1_EC8D
        jmp    SHORT loc_1_ECEC
loc_1_EC9F:
        sub    edx, edx
        db  039h, 015h, 0ECh, 036h, 001h, 000h
        jbe    SHORT loc_1_ECEC
        db  0B8h, 0F0h, 036h, 001h, 000h
loc_1_ECAE:
        cmp    dword ptr [eax], esi
        je     SHORT loc_1_ECC0
        add    eax, 4
        inc    edx
        db  039h, 015h, 0ECh, 036h, 001h, 000h
        ja     SHORT loc_1_ECAE
        jmp    SHORT loc_1_ECEC
loc_1_ECC0:
        db  0A1h, 0ECh, 036h, 001h, 000h
        dec    eax
        cmp    eax, edx
        jbe    SHORT loc_1_ECE6
        db  08Dh, 03Ch, 095h, 0F0h, 036h, 001h, 000h
loc_1_ECD1:
        mov    ecx, dword ptr [edi + 4]
        lea    eax, [edi + 4]
        mov    dword ptr [edi], ecx
        mov    edi, eax
        inc    edx
        db  0A1h, 0ECh, 036h, 001h, 000h
        dec    eax
        cmp    eax, edx
        ja     SHORT loc_1_ECD1
loc_1_ECE6:
        db  0FFh, 00Dh, 0ECh, 036h, 001h, 000h
loc_1_ECEC:
        sub    ecx, ecx
        db  039h, 00Dh, 028h, 037h, 001h, 000h
        jbe    SHORT loc_1_ED5A
        db  0B8h, 02Ch, 037h, 001h, 000h
loc_1_ECFB:
        cmp    dword ptr [eax], esi
        je     SHORT loc_1_ED0D
        add    eax, 4
        inc    ecx
        db  039h, 00Dh, 028h, 037h, 001h, 000h
        ja     SHORT loc_1_ECFB
        jmp    SHORT loc_1_ED5A
loc_1_ED0D:
        sub    edx, edx
        db  039h, 015h, 028h, 037h, 001h, 000h
        jbe    SHORT loc_1_ED5A
        db  0B8h, 02Ch, 037h, 001h, 000h
loc_1_ED1C:
        cmp    dword ptr [eax], esi
        je     SHORT loc_1_ED2E
        add    eax, 4
        inc    edx
        db  03Bh, 015h, 028h, 037h, 001h, 000h
        jb     SHORT loc_1_ED1C
        jmp    SHORT loc_1_ED5A
loc_1_ED2E:
        db  0A1h, 028h, 037h, 001h, 000h
        dec    eax
        cmp    eax, edx
        jbe    SHORT loc_1_ED54
        db  08Dh, 03Ch, 095h, 02Ch, 037h, 001h, 000h
loc_1_ED3F:
        mov    ecx, dword ptr [edi + 4]
        lea    eax, [edi + 4]
        mov    dword ptr [edi], ecx
        mov    edi, eax
        inc    edx
        db  0A1h, 028h, 037h, 001h, 000h
        dec    eax
        cmp    eax, edx
        ja     SHORT loc_1_ED3F
loc_1_ED54:
        db  0FFh, 00Dh, 028h, 037h, 001h, 000h
loc_1_ED5A:
        push   esi
        call   sub_1_F396
        add    esp, 4
loc_1_ED63:
        pop    edi
        pop    esi
        ret
sub_1_ED66:
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 10h]
        mov    edi, edx
        mov    esi, dword ptr [edx + 38h]
        mov    ebx, dword ptr [edx + 59h]
        sub    eax, eax
        mov    ecx, 18h
        rep stosd
        stosb
        mov    dword ptr [edx + 38h], esi
        mov    dword ptr [edx + 59h], ebx
        mov    edi, esi
        mov    ecx, ebx
        shr    ecx, 2
        rep stosd
        mov    ecx, ebx
        and    ecx, 3
        rep stosb
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_ED99:
        push   esi
        push   edi
        sub    esi, esi
        mov    edi, dword ptr [esp + 10h]
        push   edi
        call   sub_1_EE10
        add    esp, 4
        cmp    eax, esi
        jne    SHORT loc_1_EDE2
        push   edi
        call   sub_1_EA90
        add    esp, 4
        mov    edi, eax
        cmp    edi, esi
        jne    SHORT loc_1_EDC4
        mov    eax, 0fffffefah
        jmp    SHORT loc_1_EDE4
loc_1_EDC4:
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [edi], eax
        push   edi
        call   sub_1_E957
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_EDE2
        push   edi
        call   sub_1_EB85
        add    esp, 4
loc_1_EDE2:
        mov    eax, esi
loc_1_EDE4:
        pop    edi
        pop    esi
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 074h, 024h, 00Ch, 0FFh, 074h, 024h
        db  00Ch, 0E8h, 0D3h, 009h, 000h, 000h, 083h, 0C4h, 008h, 0C3h, 0FFh, 074h, 024h, 008h, 0E8h, 03Bh
        db  00Ah, 000h, 000h, 083h, 0C4h, 004h, 0C3h, 000h, 000h
sub_1_EE10:
        push   esi
        sub    ecx, ecx
        db  039h, 00Dh, 0ECh, 036h, 001h, 000h
        jbe    SHORT loc_1_EE37
        db  0BAh, 0F0h, 036h, 001h, 000h
        mov    eax, dword ptr [esp + 8]
loc_1_EE24:
        mov    esi, dword ptr [edx]
        cmp    dword ptr [esi + 4], eax
        je     SHORT loc_1_EE3B
        add    edx, 4
        inc    ecx
        db  039h, 00Dh, 0ECh, 036h, 001h, 000h
        ja     SHORT loc_1_EE24
loc_1_EE37:
        sub    eax, eax
loc_1_EE39:
        pop    esi
        ret
loc_1_EE3B:
        db  08Bh, 004h, 08Dh, 0F0h, 036h, 001h, 000h
        jmp    SHORT loc_1_EE39
sub_1_EE44:
        mov    eax, 0ffffffffh
        ret
sub_1_EE4A:
        mov    eax, 0ffffffffh
        ret
loc_1_EE50:
        sub    esp, 4
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        add    eax, 4
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_EE75
        mov    dword ptr [eax], 0
        push   dword ptr [esp]
        call   sub_1_EE79
        add    esp, 4
loc_1_EE75:
        add    esp, 4
        ret
sub_1_EE79:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_1_EE94
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 028h, 000h, 001h, 000h
loc_1_EE94:
        mov    esp, ebp
        pop    ebp
        ret
sub_1_EE98:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 8], 0
        je     NEAR PTR loc_1_EF43
        cmp    dword ptr [esi], 0
        jne    NEAR PTR loc_1_EF43
        cmp    dword ptr [esi + 4], 0
        jne    NEAR PTR loc_1_EF43
        push   0
        call   sub_1_129B0
        mov    dword ptr [esi], eax
        or     eax, eax
        je     SHORT loc_1_EEEF
        push   esi
        push   dword ptr [esi + 8]
        db  068h, 050h, 0EEh, 000h, 000h
        call   sub_1_129CC
        mov    dword ptr [esi + 4], eax
        or     eax, eax
        jne    SHORT loc_1_EEEF
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
loc_1_EEEF:
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     SHORT loc_1_EF43
        cmp    dword ptr [esi + 4], 0
        je     SHORT loc_1_EF43
        push   4
        push   eax
        call   sub_1_129C0
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_1_EF2F
        push   eax
        call   sub_1_129E8
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        mov    dword ptr [esi + 4], 0
        mov    dword ptr [esi], 0
        sub    eax, eax
        jmp    SHORT loc_1_EF48
loc_1_EF2F:
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        mov    dword ptr [esi], 0
loc_1_EF43:
        mov    eax, 1
loc_1_EF48:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h, 000h
sub_1_EF50:
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   dword ptr [esp + 18h]
        call   sub_1_EE10
        mov    ebx, eax
        add    esp, 4
        mov    ebp, dword ptr [ebx + 59h]
        mov    eax, dword ptr [esp + 20h]
        cmp    ebp, eax
        jb     SHORT loc_1_EF6F
        mov    ebp, eax
loc_1_EF6F:
        or     ebx, ebx
        jne    SHORT loc_1_EF7A
        mov    eax, 0fffffefeh
        jmp    SHORT loc_1_EFA7
loc_1_EF7A:
        mov    edi, dword ptr [esp + 1ch]
        mov    esi, dword ptr [ebx + 3ch]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        push   dword ptr [esp + 18h]
        call   sub_1_35D1
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_EFA2
        inc    dword ptr [eax + 3dh]
loc_1_EFA2:
        add    dword ptr [ebx + 3ch], ebp
        mov    eax, ebp
loc_1_EFA7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
sub_1_EFC0:
        push   esi
        push   edi
        call   sub_1_F05F
        call   sub_1_F0F0
        db  0A1h, 0ECh, 036h, 001h, 000h
        db  08Dh, 034h, 085h, 0ECh, 036h, 001h, 000h
        or     eax, eax
        jbe    SHORT loc_1_EFFB
        sub    edi, edi
loc_1_EFDE:
        push   dword ptr [esi]
        call   sub_1_EC1B
        add    esp, 4
        mov    dword ptr [esi], edi
        sub    esi, 4
        db  0FFh, 00Dh, 0ECh, 036h, 001h, 000h
        db  039h, 03Dh, 0ECh, 036h, 001h, 000h
        ja     SHORT loc_1_EFDE
loc_1_EFFB:
        db  0A1h, 028h, 037h, 001h, 000h
        db  08Dh, 034h, 085h, 028h, 037h, 001h, 000h
        or     eax, eax
        jbe    SHORT loc_1_F02A
        sub    edi, edi
loc_1_F00D:
        push   dword ptr [esi]
        call   sub_1_EC1B
        add    esp, 4
        mov    dword ptr [esi], edi
        sub    esi, 4
        db  0FFh, 00Dh, 028h, 037h, 001h, 000h
        db  039h, 03Dh, 028h, 037h, 001h, 000h
        ja     SHORT loc_1_F00D
loc_1_F02A:
        pop    edi
        pop    esi
        ret
sub_1_F02D:
        db  083h, 03Dh, 054h, 036h, 001h, 000h, 000h
        jne    SHORT loc_1_F038
        jmp    SHORT loc_1_F05C
loc_1_F038:
        db  0C7h, 005h, 054h, 036h, 001h, 000h, 000h, 000h, 000h, 000h
        cmp    dword ptr [esp + 8], 0
        je     SHORT loc_1_F05C
        call   sub_1_F05F
        db  0FFh, 035h, 034h, 039h, 001h, 000h
        call   sub_1_F0C3
        add    esp, 4
loc_1_F05C:
        sub    eax, eax
        ret
sub_1_F05F:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        lea    eax, [esp + 0ch]
        push   eax
        call   sub_1_F845
        add    esp, 4
        mov    edi, eax
        cmp    edi, -2
        je     SHORT loc_1_F0BA
        sub    ebx, ebx
loc_1_F07B:
        push   edi
        call   sub_1_EE10
        add    esp, 4
        mov    esi, eax
        cmp    esi, ebx
        je     SHORT loc_1_F0A5
        cmp    dword ptr [esi + 20h], ebx
        je     SHORT loc_1_F09C
        push   dword ptr [esi + 4]
        push   dword ptr [esi]
        call   sub_1_E9D9
        add    esp, 8
loc_1_F09C:
        push   esi
        call   sub_1_EA5C
        add    esp, 4
loc_1_F0A5:
        lea    eax, [esp + 0ch]
        push   eax
        push   edi
        call   sub_1_F7D0
        add    esp, 8
        mov    edi, eax
        cmp    edi, -2
        jne    SHORT loc_1_F07B
loc_1_F0BA:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_F0C3:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi], 0
        jne    SHORT loc_1_F0D4
        mov    eax, 0fffffef9h
        jmp    SHORT loc_1_F0EE
loc_1_F0D4:
        call   sub_1_F05F
        push   esi
        call   sub_1_FAEA
        add    esp, 4
        cmp    dword ptr [esi], 0
        mov    eax, 0fffffef9h
        jne    SHORT loc_1_F0EE
        sub    eax, eax
loc_1_F0EE:
        pop    esi
        ret
sub_1_F0F0:
        call   sub_1_FC70
        jmp    NEAR PTR loc_1_FAC0
        db  000h, 000h, 0CDh, 020h, 002h, 080h, 052h, 004h, 000h, 000h
WSVV_Service_00:
        mov    ax, 100h
        clc
        ret
WSVV_Control:
        cmp    eax, 1
        db  00Fh, 084h, 06Dh, 017h, 0FDh, 0FFh
        cmp    eax, 1bh
        db  00Fh, 084h, 057h, 018h, 0FDh, 0FFh
        cmp    eax, 1ch
        je     SHORT loc_1_F12C
        cmp    eax, 23h
        je     NEAR PTR loc_1_D6D0
        clc
        ret
loc_1_F12C:
        db  0E8h, 0CFh, 010h, 0FDh, 0FFh
        or     eax, eax
        je     SHORT loc_1_F137
        clc
        ret
loc_1_F137:
        stc
        ret
        db  000h, 000h, 000h
loc_1_F13C:
        xchg   al, ah
        ror    eax, 10h
        xchg   al, ah
        ror    eax, 10h
        mov    ecx, eax
        ror    ecx, 10h
        add    eax, ecx
        shr    eax, 10h
        ret
sub_1_F151:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 10h]
        jecxz  loc_1_F1CC
        test   esi, 3
        je     SHORT loc_1_F1AC
        test   esi, 1
        je     SHORT loc_1_F197
        movzx  edx, byte ptr [esi]
        add    eax, edx
        adc    eax, 0
        db  0BAh, 03Ch, 0F1h, 000h, 000h
        xchg   dword ptr [esp], edx
        push   edx
        xchg   al, ah
        ror    eax, 10h
        xchg   al, ah
        ror    eax, 10h
        inc    esi
        dec    ecx
        je     SHORT loc_1_F1CC
        test   esi, 3
        je     SHORT loc_1_F1AC
loc_1_F197:
        cmp    ecx, 1
        je     SHORT loc_1_F1F9
        movzx  edx, word ptr [esi]
        add    eax, edx
        adc    eax, 0
        add    esi, 2
        sub    ecx, 2
        je     SHORT loc_1_F1CC
loc_1_F1AC:
        cmp    ecx, 4
        jb     SHORT loc_1_F1C8
        mov    edx, ecx
        shr    ecx, 2
        and    dl, 3
        mov    eax, eax
loc_1_F1BC:
        add    eax, dword ptr [esi]
        adc    eax, 0
        add    esi, 4
        loop   loc_1_F1BC
        mov    ecx, edx
loc_1_F1C8:
        test   cl, cl
        jne    SHORT loc_1_F1D8
loc_1_F1CC:
        pop    esi
        mov    ecx, eax
        ror    ecx, 10h
        add    eax, ecx
        shr    eax, 10h
        ret
loc_1_F1D8:
        cmp    cl, 1
        je     SHORT loc_1_F1F9
        movzx  edx, word ptr [esi]
        add    eax, edx
        adc    eax, 0
        add    esi, 2
        sub    cl, 2
        jne    SHORT loc_1_F1F9
        pop    esi
        mov    ecx, eax
        ror    ecx, 10h
        add    eax, ecx
        shr    eax, 10h
        ret
loc_1_F1F9:
        movzx  edx, byte ptr [esi]
        add    eax, edx
        adc    eax, 0
        pop    esi
        mov    ecx, eax
        ror    ecx, 10h
        add    eax, ecx
        shr    eax, 10h
        ret
        db  000h, 000h, 000h, 0CDh, 020h, 001h, 080h, 088h, 004h, 0CDh, 020h, 002h, 080h, 088h, 004h
sub_1_F21C:
        db  0CDh, 020h, 03Fh, 080h, 001h, 000h
        db  0CDh, 020h, 003h, 080h, 088h, 004h
        db  0CDh, 020h, 004h, 080h, 088h, 004h
        db  0CDh, 020h, 00Dh, 080h, 040h, 000h
        db  0CDh, 020h, 00Eh, 080h, 040h, 000h
        db  0CDh, 020h, 011h, 080h, 040h, 000h
        db  0CDh, 020h, 005h, 080h, 088h, 004h
        db  0CDh, 020h, 006h, 080h, 088h, 004h
        db  0A1h, 090h, 02Bh, 001h, 000h
        db  0CDh, 020h, 009h, 080h, 088h, 004h
        db  0A1h, 090h, 02Bh, 001h, 000h
        db  0CDh, 020h, 00Ch, 080h, 088h, 004h
        db  0A1h, 090h, 02Bh, 001h, 000h
        db  0CDh, 020h, 00Bh, 080h, 088h, 004h
        add    byte ptr [eax], al
        db  000h
sub_1_F270:
        push   ebx
        push   edi
        push   esi
        mov    eax, dword ptr [esp + 10h]
        mov    ebx, dword ptr [esp + 14h]
        mov    ecx, eax
        or     ecx, ebx
        je     SHORT loc_1_F2AD
        movzx  ecx, ax
        and    ch, 0fh
        mov    esi, ecx
        add    ebx, ecx
        add    ebx, 0fffh
        shr    ebx, 0ch
        shr    eax, 0ch
        push   40000000h
        push   ebx
        push   eax
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_F2B1
        add    eax, esi
loc_1_F2AD:
        pop    esi
        pop    edi
        pop    ebx
        ret
loc_1_F2B1:
        xor    eax, eax
        jmp    SHORT loc_1_F2AD
sub_1_F2B5:
        push   ebx
        push   edi
        push   esi
        mov    eax, dword ptr [esp + 10h]
        mov    ebx, dword ptr [esp + 14h]
        mov    ecx, eax
        or     ecx, ebx
        je     SHORT loc_1_F2EE
        movzx  ecx, ax
        and    ch, 0fh
        add    ebx, ecx
        add    ebx, 0fffh
        shr    ebx, 0ch
        shr    eax, 0ch
        push   40000000h
        push   ebx
        push   eax
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_F2F2
loc_1_F2EE:
        pop    esi
        pop    edi
        pop    ebx
        ret
loc_1_F2F2:
        xor    eax, eax
        jmp    SHORT loc_1_F2EE
        db  000h, 000h
sub_1_F2F8:
        push   ebx
        push   ecx
        push   edx
        push   esi
        mov    ebx, dword ptr [esp + 14h]
        movzx  eax, word ptr [esp + 18h]
        movzx  esi, word ptr [esp + 1ch]
        or     eax, eax
        je     SHORT loc_1_F334
        test   dword ptr [ebx], 20h
        je     SHORT loc_1_F339
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_1_F334
        add    eax, esi
        cmp    eax, 110000h
        jae    SHORT loc_1_F334
loc_1_F331:
        add    eax, dword ptr [ebx + 4]
loc_1_F334:
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret
loc_1_F339:
        shl    eax, 4
        add    eax, esi
        jmp    SHORT loc_1_F331
loc_1_F340:
        ret
sub_1_F341:
        push   ebx
        push   edi
        push   esi
        push   0
        db  068h, 040h, 0F3h, 000h, 000h
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 24h]
        db  0CDh, 020h, 006h, 000h, 017h, 000h
        add    esp, 18h
        pop    esi
        pop    edi
        pop    ebx
        ret
        db  0FFh, 074h, 024h, 004h, 0FFh, 074h, 024h, 010h, 0FFh, 074h, 024h, 010h, 0CDh, 020h, 002h, 000h
        db  02Ah, 000h, 083h, 0C4h, 00Ch, 0C3h
sub_1_F37E:
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    eax, edi
        pop    edi
        ret
sub_1_F389:
        db  0CDh, 020h, 00Dh, 080h, 02Ah, 000h
        db  000h
sub_1_F390:
        db  0CDh, 020h, 00Dh, 080h, 040h, 000h
sub_1_F396:
        cmp    dword ptr [esp + 4], 0
        je     SHORT loc_1_F3A3
        db  0CDh, 020h, 00Eh, 080h, 040h, 000h
loc_1_F3A3:
        ret
sub_1_F3A4:
        db  0CDh, 020h, 011h, 080h, 040h, 000h
        add    byte ptr [eax], al
sub_1_F3AC:
        db  0CDh, 020h, 003h, 080h, 03Eh, 000h
sub_1_F3B2:
        db  0CDh, 020h, 004h, 080h, 03Eh, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_F3C0:
        sub    eax, eax
        db  039h, 005h, 0A8h, 038h, 001h, 000h
        jbe    SHORT loc_1_F3E7
        db  08Bh, 00Dh, 0A4h, 038h, 001h, 000h
        add    ecx, 8
        mov    edx, dword ptr [esp + 4]
loc_1_F3D7:
        cmp    dword ptr [ecx], edx
        je     SHORT loc_1_F3E7
        add    ecx, 0ch
        inc    eax
        db  039h, 005h, 0A8h, 038h, 001h, 000h
        ja     SHORT loc_1_F3D7
loc_1_F3E7:
        ret
sub_1_F3E8:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        push   edi
        call   sub_1_F3C0
        add    esp, 4
        mov    esi, eax
        db  0A1h, 0A8h, 038h, 001h, 000h
        dec    eax
        cmp    eax, esi
        jb     SHORT loc_1_F47B
        push   edi
        call   sub_1_F796
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_F417
        mov    eax, 0fffffeffh
        jmp    SHORT loc_1_F480
loc_1_F417:
        push   edi
        call   sub_1_F4C9
        mov    ecx, dword ptr [esp + 14h]
        add    esp, 4
        mov    dword ptr [ecx], eax
        mov    edx, eax
        or     edx, edx
        jne    SHORT loc_1_F433
        mov    eax, 0fffffefah
        jmp    SHORT loc_1_F480
loc_1_F433:
        imul   esi, esi, 0ch
        db  0A1h, 0A4h, 038h, 001h, 000h
        add    eax, esi
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_F449
        mov    eax, 0fffffef9h
        jmp    SHORT loc_1_F480
loc_1_F449:
        cmp    dword ptr [eax + 4], 0
        jne    SHORT loc_1_F456
        mov    eax, 0fffffefdh
        jmp    SHORT loc_1_F480
loc_1_F456:
        mov    dword ptr [edx + 8], 1
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [edx + 4], ecx
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [edx + 0ch], eax
        db  0A1h, 0A4h, 038h, 001h, 000h
        mov    eax, dword ptr [esi + eax + 4]
        mov    dword ptr [ecx + 4], eax
        sub    eax, eax
        jmp    SHORT loc_1_F480
loc_1_F47B:
        mov    eax, 0fffffefeh
loc_1_F480:
        pop    edi
        pop    esi
        ret
sub_1_F483:
        sub    eax, eax
        db  039h, 005h, 090h, 037h, 001h, 000h
        jbe    SHORT loc_1_F4A8
        mov    ecx, dword ptr [esp + 4]
loc_1_F491:
        cmp    eax, 0ah
        jae    SHORT loc_1_F4A8
        db  039h, 00Ch, 085h, 094h, 037h, 001h, 000h
        je     SHORT loc_1_F4AE
        inc    eax
        db  03Bh, 005h, 090h, 037h, 001h, 000h
        jb     SHORT loc_1_F491
loc_1_F4A8:
        mov    eax, 0fffffef8h
loc_1_F4AD:
        ret
loc_1_F4AE:
        mov    dword ptr [ecx + 8], 0
        mov    dword ptr [ecx + 4], 0
        push   ecx
        call   sub_1_F57B
        add    esp, 4
        sub    eax, eax
        jmp    SHORT loc_1_F4AD
sub_1_F4C9:
        push   esi
        push   edi
        db  083h, 03Dh, 0C0h, 037h, 001h, 000h, 000h
        je     NEAR PTR loc_1_F567
        db  0A1h, 0C0h, 037h, 001h, 000h
        db  08Bh, 034h, 085h, 0C0h, 037h, 001h, 000h
        mov    edi, 0
        jbe    SHORT loc_1_F502
loc_1_F4EB:
        cmp    edi, 0ah
        jae    SHORT loc_1_F502
        db  039h, 034h, 0BDh, 0C4h, 037h, 001h, 000h
        je     SHORT loc_1_F531
        inc    edi
        db  03Bh, 03Dh, 0C0h, 037h, 001h, 000h
        jb     SHORT loc_1_F4EB
loc_1_F502:
        db  083h, 03Dh, 090h, 037h, 001h, 000h, 00Ah
        jae    SHORT loc_1_F51D
        db  0A1h, 090h, 037h, 001h, 000h
        db  089h, 034h, 085h, 094h, 037h, 001h, 000h
        db  0FFh, 005h, 090h, 037h, 001h, 000h
loc_1_F51D:
        push   esi
        call   sub_1_F776
        mov    eax, dword ptr [esp + 10h]
        add    esp, 4
        mov    dword ptr [esi], eax
        mov    eax, esi
loc_1_F52E:
        pop    edi
        pop    esi
        ret
loc_1_F531:
        db  0A1h, 0C0h, 037h, 001h, 000h
        dec    eax
        cmp    eax, edi
        jbe    SHORT loc_1_F55F
        db  08Dh, 014h, 0BDh, 0C4h, 037h, 001h, 000h
loc_1_F542:
        db  081h, 0FAh, 0E8h, 037h, 001h, 000h
        jae    SHORT loc_1_F55F
        mov    ecx, dword ptr [edx + 4]
        lea    eax, [edx + 4]
        mov    dword ptr [edx], ecx
        mov    edx, eax
        inc    edi
        db  0A1h, 0C0h, 037h, 001h, 000h
        dec    eax
        cmp    eax, edi
        ja     SHORT loc_1_F542
loc_1_F55F:
        db  0FFh, 00Dh, 0C0h, 037h, 001h, 000h
        jmp    SHORT loc_1_F502
loc_1_F567:
        push   20h
        call   sub_1_F390
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_F502
        sub    eax, eax
        jmp    SHORT loc_1_F52E
sub_1_F57B:
        push   esi
        push   edi
        sub    eax, eax
        db  039h, 005h, 090h, 037h, 001h, 000h
        jbe    NEAR PTR loc_1_F61E
        mov    edx, dword ptr [esp + 0ch]
loc_1_F58F:
        cmp    eax, 0ah
        jae    NEAR PTR loc_1_F61E
        db  039h, 014h, 085h, 094h, 037h, 001h, 000h
        je     SHORT loc_1_F5AC
        inc    eax
        db  03Bh, 005h, 090h, 037h, 001h, 000h
        jb     SHORT loc_1_F58F
        jmp    SHORT loc_1_F61E
loc_1_F5AC:
        sub    edi, edi
        db  039h, 03Dh, 090h, 037h, 001h, 000h
        jbe    SHORT loc_1_F603
loc_1_F5B6:
        cmp    edi, 0ah
        jae    SHORT loc_1_F603
        db  039h, 014h, 0BDh, 094h, 037h, 001h, 000h
        je     SHORT loc_1_F5CF
        inc    edi
        db  03Bh, 03Dh, 090h, 037h, 001h, 000h
        jb     SHORT loc_1_F5B6
        jmp    SHORT loc_1_F603
loc_1_F5CF:
        db  0A1h, 090h, 037h, 001h, 000h
        dec    eax
        cmp    eax, edi
        jbe    SHORT loc_1_F5FD
        db  08Dh, 034h, 0BDh, 094h, 037h, 001h, 000h
loc_1_F5E0:
        db  081h, 0FEh, 0B8h, 037h, 001h, 000h
        jae    SHORT loc_1_F5FD
        mov    ecx, dword ptr [esi + 4]
        lea    eax, [esi + 4]
        mov    dword ptr [esi], ecx
        mov    esi, eax
        inc    edi
        db  0A1h, 090h, 037h, 001h, 000h
        dec    eax
        cmp    eax, edi
        ja     SHORT loc_1_F5E0
loc_1_F5FD:
        db  0FFh, 00Dh, 090h, 037h, 001h, 000h
loc_1_F603:
        db  083h, 03Dh, 0C0h, 037h, 001h, 000h, 00Ah
        jae    SHORT loc_1_F61E
        db  0A1h, 0C0h, 037h, 001h, 000h
        db  089h, 014h, 085h, 0C4h, 037h, 001h, 000h
        db  0FFh, 005h, 0C0h, 037h, 001h, 000h
loc_1_F61E:
        pop    edi
        pop    esi
        ret
        db  056h, 057h, 02Bh, 0C0h, 039h, 005h, 090h, 037h, 001h, 000h, 08Bh, 04Ch, 024h, 00Ch, 076h, 017h
        db  083h, 0F8h, 00Ah, 073h, 012h, 039h, 00Ch, 085h, 094h, 037h, 001h, 000h, 074h, 037h, 040h, 03Bh
        db  005h, 090h, 037h, 001h, 000h, 072h, 0E9h, 02Bh, 0C0h, 039h, 005h, 0C0h, 037h, 001h, 000h, 00Fh
        db  086h, 01Dh, 001h, 000h, 000h, 083h, 0F8h, 00Ah, 00Fh, 083h, 014h, 001h, 000h, 000h, 039h, 00Ch
        db  085h, 0C4h, 037h, 001h, 000h, 074h, 00Eh, 040h, 03Bh, 005h, 0C0h, 037h, 001h, 000h, 072h, 0E5h
        db  0E9h, 0FDh, 000h, 000h, 000h, 02Bh, 0C0h, 039h, 005h, 090h, 037h, 001h, 000h, 076h, 070h, 083h
        db  0F8h, 00Ah, 073h, 06Bh, 039h, 00Ch, 085h, 094h, 037h, 001h, 000h, 074h, 00Bh, 040h, 03Bh, 005h
        db  090h, 037h, 001h, 000h, 072h, 0E9h, 0EBh, 057h, 02Bh, 0FFh, 039h, 03Dh, 090h, 037h, 001h, 000h
        db  076h, 04Dh, 083h, 0FFh, 00Ah, 073h, 048h, 039h, 00Ch, 0BDh, 094h, 037h, 001h, 000h, 074h, 00Bh
        db  047h, 03Bh, 03Dh, 090h, 037h, 001h, 000h, 072h, 0E9h, 0EBh, 034h, 0A1h, 090h, 037h, 001h, 000h
        db  048h, 03Bh, 0C7h, 076h, 024h, 08Dh, 034h, 0BDh, 094h, 037h, 001h, 000h, 081h, 0FEh, 0B8h, 037h
        db  001h, 000h, 073h, 015h, 08Bh, 056h, 004h, 08Dh, 046h, 004h, 089h, 016h, 08Bh, 0F0h, 047h, 0A1h
        db  090h, 037h, 001h, 000h, 048h, 03Bh, 0C7h, 077h, 0E3h, 0FFh, 00Dh, 090h, 037h, 001h, 000h, 02Bh
        db  0C0h, 039h, 005h, 0C0h, 037h, 001h, 000h, 076h, 070h, 083h, 0F8h, 00Ah, 073h, 06Bh, 039h, 00Ch
        db  085h, 0C4h, 037h, 001h, 000h, 074h, 00Bh, 040h, 03Bh, 005h, 0C0h, 037h, 001h, 000h, 072h, 0E9h
        db  0EBh, 057h, 02Bh, 0FFh, 039h, 03Dh, 0C0h, 037h, 001h, 000h, 076h, 04Dh, 083h, 0FFh, 00Ah, 073h
        db  048h, 039h, 00Ch, 0BDh, 0C4h, 037h, 001h, 000h, 074h, 00Bh, 047h, 03Bh, 03Dh, 0C0h, 037h, 001h
        db  000h, 072h, 0E9h, 0EBh, 034h, 0A1h, 0C0h, 037h, 001h, 000h, 048h, 03Bh, 0C7h, 076h, 024h, 08Dh
        db  034h, 0BDh, 0C4h, 037h, 001h, 000h, 081h, 0FEh, 0E8h, 037h, 001h, 000h, 073h, 015h, 08Bh, 056h
        db  004h, 08Dh, 046h, 004h, 089h, 016h, 08Bh, 0F0h, 047h, 0A1h, 0C0h, 037h, 001h, 000h, 048h, 03Bh
        db  0C7h, 077h, 0E3h, 0FFh, 00Dh, 0C0h, 037h, 001h, 000h, 051h, 0E8h, 026h, 0FCh, 0FFh, 0FFh, 083h
        db  0C4h, 004h, 05Fh, 05Eh, 0C3h
sub_1_F776:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0ffffffffh
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0ch], 0
        ret
sub_1_F796:
        push   esi
        sub    ecx, ecx
        db  039h, 00Dh, 090h, 037h, 001h, 000h
        jbe    SHORT loc_1_F7BC
        db  0BAh, 094h, 037h, 001h, 000h
        mov    eax, dword ptr [esp + 8]
loc_1_F7AA:
        mov    esi, dword ptr [edx]
        cmp    dword ptr [esi], eax
        je     SHORT loc_1_F7C0
        add    edx, 4
        inc    ecx
        db  03Bh, 00Dh, 090h, 037h, 001h, 000h
        jb     SHORT loc_1_F7AA
loc_1_F7BC:
        sub    eax, eax
loc_1_F7BE:
        pop    esi
        ret
loc_1_F7C0:
        mov    eax, 1
        jmp    SHORT loc_1_F7BE
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_F7D0:
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        or     esi, esi
        je     SHORT loc_1_F7DF
        mov    dword ptr [esi], 0fffffffeh
loc_1_F7DF:
        mov    eax, dword ptr [esp + 8]
        cmp    eax, -1
        jne    SHORT loc_1_F7EC
        sub    eax, eax
        jmp    SHORT loc_1_F7F6
loc_1_F7EC:
        push   eax
        call   sub_1_F3C0
        add    esp, 4
        inc    eax
loc_1_F7F6:
        db  039h, 005h, 0A8h, 038h, 001h, 000h
        jbe    SHORT loc_1_F81A
        mov    ecx, eax
        imul   ecx, ecx, 0ch
        db  003h, 00Dh, 0A4h, 038h, 001h, 000h
loc_1_F809:
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_1_F821
        add    ecx, 0ch
        inc    eax
        db  039h, 005h, 0A8h, 038h, 001h, 000h
        ja     SHORT loc_1_F809
loc_1_F81A:
        mov    eax, 0fffffffeh
loc_1_F81F:
        pop    esi
        ret
loc_1_F821:
        or     esi, esi
        je     SHORT loc_1_F836
        mov    ecx, eax
        imul   ecx, ecx, 0ch
        db  08Bh, 015h, 0A4h, 038h, 001h, 000h
        mov    ecx, dword ptr [ecx + edx + 8]
        mov    dword ptr [esi], ecx
loc_1_F836:
        imul   eax, eax, 0ch
        db  08Bh, 00Dh, 0A4h, 038h, 001h, 000h
        mov    eax, dword ptr [eax + ecx + 8]
        jmp    SHORT loc_1_F81F
sub_1_F845:
        db  083h, 03Dh, 0A8h, 038h, 001h, 000h, 000h
        mov    eax, 0fffffffeh
        je     SHORT loc_1_F861
        push   dword ptr [esp + 4]
        push   -1
        call   sub_1_F7D0
        add    esp, 8
loc_1_F861:
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_F870:
        push   1
        db  068h, 094h, 02Bh, 001h, 000h
        db  068h, 05Eh, 0F9h, 000h, 000h
        call   sub_1_12910
        add    esp, 0ch
        db  0A3h, 094h, 035h, 001h, 000h
        or     eax, eax
        mov    eax, 0ffffffffh
        je     SHORT loc_1_F8C4
        mov    eax, dword ptr [esp + 4]
        db  0A3h, 0C4h, 038h, 001h, 000h
        mov    eax, dword ptr [esp + 8]
        db  0A3h, 0C0h, 038h, 001h, 000h
        sub    eax, eax
        db  0C7h, 005h, 0A8h, 038h, 001h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 098h, 038h, 001h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0BCh, 038h, 001h, 000h, 000h, 000h, 000h, 000h
loc_1_F8C4:
        ret
loc_1_F8C5:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    edi, 1
        cmp    dword ptr [esi + 0ch], 0
        je     SHORT loc_1_F923
        db  083h, 03Dh, 098h, 038h, 001h, 000h, 000h
        je     SHORT loc_1_F900
        db  083h, 03Dh, 0C4h, 038h, 001h, 000h, 000h
        je     SHORT loc_1_F900
        db  0FFh, 035h, 0C0h, 038h, 001h, 000h
        push   0
        push   7d2h
        push   0
        db  0FFh, 015h, 0C4h, 038h, 001h, 000h
        add    esp, 10h
loc_1_F900:
        push   dword ptr [esi + 8]
        push   dword ptr [esi]
        call   sub_1_F9D8
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_F923
        mov    eax, dword ptr [esi + 8]
        db  0A3h, 0A8h, 038h, 001h, 000h
        mov    eax, dword ptr [esi + 4]
        db  0A3h, 098h, 038h, 001h, 000h
        jmp    SHORT loc_1_F925
loc_1_F923:
        sub    edi, edi
loc_1_F925:
        db  089h, 03Dh, 09Ch, 038h, 001h, 000h
        db  083h, 03Dh, 0C4h, 038h, 001h, 000h, 000h
        je     SHORT loc_1_F94B
        db  0FFh, 035h, 0C0h, 038h, 001h, 000h
        push   edi
        push   7d0h
        push   0
        db  0FFh, 015h, 0C4h, 038h, 001h, 000h
        add    esp, 10h
loc_1_F94B:
        or     edi, edi
        jne    SHORT loc_1_F959
        push   0
        call   sub_1_FAEA
        add    esp, 4
loc_1_F959:
        sub    eax, eax
        pop    edi
        pop    esi
        ret
loc_1_F95E:
        db  068h, 0CCh, 02Bh, 001h, 000h
        push   0
        push   10040h
        call   sub_1_12918
        add    esp, 0ch
        db  0A3h, 0C6h, 02Bh, 001h, 000h
        db  068h, 0C4h, 02Bh, 001h, 000h
        push   6
        push   0a6h
        db  068h, 0D8h, 02Bh, 001h, 000h
        call   sub_1_12928
        add    esp, 10h
        db  0A3h, 094h, 038h, 001h, 000h
        db  0FFh, 035h, 0C6h, 02Bh, 001h, 000h
        call   sub_1_12920
        add    esp, 4
        db  083h, 03Dh, 094h, 038h, 001h, 000h, 020h
        jge    SHORT loc_1_F9D7
        db  083h, 03Dh, 0C4h, 038h, 001h, 000h, 000h
        je     SHORT loc_1_F9CD
        db  0FFh, 035h, 0C0h, 038h, 001h, 000h
        push   0
        push   7d0h
        push   0
        db  0FFh, 015h, 0C4h, 038h, 001h, 000h
        add    esp, 10h
loc_1_F9CD:
        db  0C7h, 005h, 094h, 038h, 001h, 000h, 000h, 000h, 000h, 000h
loc_1_F9D7:
        ret
sub_1_F9D8:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        mov    esi, dword ptr [esp + 1ch]
        cmp    esi, edi
        je     NEAR PTR loc_1_FAAD
        db  039h, 035h, 0A8h, 038h, 001h, 000h
        je     SHORT loc_1_FA09
        db  0FFh, 035h, 0A4h, 038h, 001h, 000h
        call   sub_1_F396
        add    esp, 4
        db  089h, 03Dh, 0A4h, 038h, 001h, 000h
loc_1_FA09:
        db  083h, 03Dh, 0A4h, 038h, 001h, 000h, 000h
        jne    SHORT loc_1_FA35
        mov    eax, esi
        imul   eax, eax, 0ch
        push   eax
        call   sub_1_F390
        add    esp, 4
        db  0A3h, 0A4h, 038h, 001h, 000h
        db  083h, 03Dh, 0A4h, 038h, 001h, 000h, 000h
        jne    SHORT loc_1_FA35
        mov    eax, 0fffffefah
        jmp    SHORT loc_1_FAB2
loc_1_FA35:
        mov    eax, esi
        imul   eax, eax, 44h
        mov    dword ptr [esp + 10h], eax
        push   eax
        push   dword ptr [esp + 1ch]
        call   sub_1_F270
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_FA51
        jmp    SHORT loc_1_FAAD
loc_1_FA51:
        or     esi, esi
        je     SHORT loc_1_FA94
        lea    ebx, [eax + 4]
        sub    edx, edx
loc_1_FA5A:
        mov    ebp, dword ptr [ebx]
        db  08Bh, 00Dh, 0A4h, 038h, 001h, 000h
        mov    dword ptr [edx + ecx], ebp
        db  08Bh, 02Dh, 0A4h, 038h, 001h, 000h
        add    ebp, edx
        cmp    dword ptr [ebp], 0
        je     SHORT loc_1_FA8B
        mov    ecx, dword ptr [ebx + 14h]
        mov    dword ptr [ebp + 4], ecx
        mov    ecx, dword ptr [ebx + 3ch]
        db  08Bh, 03Dh, 0A4h, 038h, 001h, 000h
        mov    dword ptr [edx + edi + 8], ecx
        mov    edi, 1
loc_1_FA8B:
        add    ebx, 44h
        add    edx, 0ch
        dec    esi
        jne    SHORT loc_1_FA5A
loc_1_FA94:
        push   dword ptr [esp + 10h]
        push   eax
        call   sub_1_F2B5
        add    esp, 8
        cmp    edi, 1
        sbb    eax, eax
        and    eax, 0fffffef9h
        jmp    SHORT loc_1_FAB2
loc_1_FAAD:
        mov    eax, 0fffffef9h
loc_1_FAB2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h
loc_1_FAC0:
        db  083h, 03Dh, 09Ch, 038h, 001h, 000h, 000h
        je     SHORT loc_1_FAE7
        push   0
        call   sub_1_FAEA
        add    esp, 4
        cmp    eax, 0fffffef7h
        jne    SHORT loc_1_FAE7
        db  068h, 0B0h, 038h, 001h, 000h
        call   sub_1_EE98
        add    esp, 4
loc_1_FAE7:
        sub    eax, eax
        ret
sub_1_FAEA:
        sub    esp, 4
        push   edi
        mov    eax, dword ptr [esp + 0ch]
        db  0A3h, 0C0h, 038h, 001h, 000h
        db  083h, 03Dh, 094h, 038h, 001h, 000h, 000h
        je     SHORT loc_1_FB16
        push   0
        push   7d2h
        call   sub_1_FBC0
        add    esp, 8
        mov    eax, 0fffffef7h
        jmp    SHORT loc_1_FB2D
loc_1_FB16:
        lea    edi, [esp + 4]
        sub    eax, eax
        push   eax
        push   eax
        stosd
        push   dword ptr [esp + 0ch]
        call   sub_1_FB32
        add    esp, 0ch
        sub    eax, eax
loc_1_FB2D:
        pop    edi
        add    esp, 4
        ret
sub_1_FB32:
        db  083h, 03Dh, 0BCh, 038h, 001h, 000h, 000h
        jne    SHORT loc_1_FB8C
        db  0FFh, 005h, 0BCh, 038h, 001h, 000h
        db  083h, 03Dh, 0A4h, 038h, 001h, 000h, 000h
        je     SHORT loc_1_FB58
        db  0FFh, 035h, 0A4h, 038h, 001h, 000h
        call   sub_1_F396
        add    esp, 4
loc_1_FB58:
        db  083h, 03Dh, 094h, 038h, 001h, 000h, 000h
        je     SHORT loc_1_FB86
        db  083h, 03Dh, 0C4h, 038h, 001h, 000h, 000h
        je     SHORT loc_1_FB86
        mov    eax, dword ptr [esp + 4]
        db  0FFh, 035h, 0C0h, 038h, 001h, 000h
        push   dword ptr [eax]
        push   7d1h
        push   0
        db  0FFh, 015h, 0C4h, 038h, 001h, 000h
        add    esp, 10h
loc_1_FB86:
        db  0FFh, 00Dh, 0BCh, 038h, 001h, 000h
loc_1_FB8C:
        db  0C7h, 005h, 0A4h, 038h, 001h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0A8h, 038h, 001h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 094h, 038h, 001h, 000h, 000h, 000h, 000h, 000h
        db  068h, 0B0h, 038h, 001h, 000h
        call   sub_1_EE79
        add    esp, 4
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_FBC0:
        push   dword ptr [esp + 8]
        push   dword ptr [esp + 8]
        push   111h
        db  0FFh, 035h, 098h, 038h, 001h, 000h
        call   sub_1_F341
        add    esp, 10h
        ret
        db  000h, 000h, 000h, 000h
sub_1_FBE0:
        push   esi
        push   edi
        sub    edi, edi
        sub    eax, eax
        db  039h, 005h, 004h, 02Fh, 001h, 000h
        jbe    SHORT loc_1_FC6B
        mov    esi, dword ptr [esp + 0ch]
loc_1_FBF2:
        cmp    eax, 0ah
        jae    SHORT loc_1_FC6B
        db  039h, 034h, 085h, 008h, 02Fh, 001h, 000h
        je     SHORT loc_1_FC0B
        inc    eax
        db  03Bh, 005h, 004h, 02Fh, 001h, 000h
        jb     SHORT loc_1_FBF2
        jmp    SHORT loc_1_FC6B
loc_1_FC0B:
        db  083h, 03Dh, 05Ch, 02Fh, 001h, 000h, 000h
        je     SHORT loc_1_FC29
        push   dword ptr [esi + 19eh]
        call   sub_1_129E8
        mov    dword ptr [esi + 19eh], 0
loc_1_FC29:
        mov    eax, dword ptr [esi + 14h]
        or     eax, eax
        je     SHORT loc_1_FC62
        cmp    eax, -15h
        jae    SHORT loc_1_FC62
        cmp    dword ptr [esi + 4ch], 0
        je     SHORT loc_1_FC44
        push   esi
        call   sub_1_1182F
        add    esp, 4
loc_1_FC44:
        push   0
        push   0
        push   0effffff2h
        push   dword ptr [esi + 14h]
        call   sub_1_12958
        add    esp, 10h
        cmp    eax, 1
        je     SHORT loc_1_FC62
        mov    edi, 4
loc_1_FC62:
        push   esi
        call   sub_1_FEB9
        add    esp, 4
loc_1_FC6B:
        mov    eax, edi
        pop    edi
        pop    esi
        ret
sub_1_FC70:
        push   esi
        push   edi
        sub    edi, edi
        db  039h, 03Dh, 004h, 02Fh, 001h, 000h
        je     SHORT loc_1_FC9F
        db  0A1h, 004h, 02Fh, 001h, 000h
        db  08Dh, 034h, 085h, 030h, 02Fh, 001h, 000h
loc_1_FC88:
        push   dword ptr [esi]
        call   sub_1_FBE0
        add    esp, 4
        mov    edi, eax
        sub    esi, 4
        db  081h, 0FEh, 030h, 02Fh, 001h, 000h
        jne    SHORT loc_1_FC88
loc_1_FC9F:
        db  083h, 03Dh, 030h, 02Fh, 001h, 000h, 000h
        je     SHORT loc_1_FCC6
        sub    esi, esi
loc_1_FCAA:
        db  0A1h, 030h, 02Fh, 001h, 000h
        db  0FFh, 034h, 085h, 030h, 02Fh, 001h, 000h
        call   sub_1_FF5F
        add    esp, 4
        db  039h, 035h, 030h, 02Fh, 001h, 000h
        jne    SHORT loc_1_FCAA
loc_1_FCC6:
        mov    eax, edi
        pop    edi
        pop    esi
        ret
        db  056h, 02Bh, 0C9h, 039h, 00Dh, 004h, 02Fh, 001h, 000h, 076h, 01Ch, 0BAh, 008h, 02Fh, 001h, 000h
        db  08Bh, 044h, 024h, 008h, 08Bh, 032h, 039h, 046h, 018h, 074h, 010h, 083h, 0C2h, 004h, 041h, 03Bh
        db  00Dh, 004h, 02Fh, 001h, 000h, 072h, 0EDh, 02Bh, 0C0h, 05Eh, 0C3h, 08Bh, 004h, 08Dh, 008h, 02Fh
        db  001h, 000h, 0EBh, 0F5h
sub_1_FCFF:
        push   esi
        push   edi
        db  083h, 03Dh, 030h, 02Fh, 001h, 000h, 000h
        je     SHORT loc_1_FD7A
        db  0A1h, 030h, 02Fh, 001h, 000h
        db  08Bh, 034h, 085h, 030h, 02Fh, 001h, 000h
        mov    edi, 0
        jbe    NEAR PTR loc_1_FE3E
loc_1_FD21:
        cmp    edi, 0ah
        jae    NEAR PTR loc_1_FE3E
        db  039h, 034h, 0BDh, 034h, 02Fh, 001h, 000h
        je     SHORT loc_1_FD41
        inc    edi
        db  03Bh, 03Dh, 030h, 02Fh, 001h, 000h
        jb     SHORT loc_1_FD21
        jmp    NEAR PTR loc_1_FE3E
loc_1_FD41:
        db  0A1h, 030h, 02Fh, 001h, 000h
        dec    eax
        cmp    eax, edi
        jbe    SHORT loc_1_FD6F
        db  08Dh, 014h, 0BDh, 034h, 02Fh, 001h, 000h
loc_1_FD52:
        db  081h, 0FAh, 058h, 02Fh, 001h, 000h
        jae    SHORT loc_1_FD6F
        mov    ecx, dword ptr [edx + 4]
        lea    eax, [edx + 4]
        mov    dword ptr [edx], ecx
        mov    edx, eax
        inc    edi
        db  0A1h, 030h, 02Fh, 001h, 000h
        dec    eax
        cmp    eax, edi
        ja     SHORT loc_1_FD52
loc_1_FD6F:
        db  0FFh, 00Dh, 030h, 02Fh, 001h, 000h
        jmp    NEAR PTR loc_1_FE3E
loc_1_FD7A:
        push   1bah
        call   sub_1_F390
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_FD94
        sub    eax, eax
        jmp    NEAR PTR loc_1_FEB6
loc_1_FD94:
        push   9c40h
        call   sub_1_F390
        add    esp, 4
        mov    dword ptr [esi + 0a4h], eax
        or     eax, eax
        jne    SHORT loc_1_FDBB
        push   esi
        call   sub_1_FF5F
        add    esp, 4
        sub    eax, eax
        jmp    NEAR PTR loc_1_FEB6
loc_1_FDBB:
        mov    dword ptr [esi + 0a8h], 9c40h
        push   9c40h
        call   sub_1_F390
        add    esp, 4
        mov    dword ptr [esi + 0ach], eax
        mov    dword ptr [esi + 0b4h], eax
        or     eax, eax
        jne    SHORT loc_1_FDF2
        push   esi
        call   sub_1_FF5F
        add    esp, 4
        sub    eax, eax
        jmp    NEAR PTR loc_1_FEB6
loc_1_FDF2:
        mov    dword ptr [esi + 0b0h], 9c40h
        mov    dword ptr [esi + 0b8h], 9c40h
        mov    dword ptr [esi + 10h], 0
        push   9c40h
        call   sub_1_F390
        add    esp, 4
        mov    dword ptr [esi + 0e0h], eax
        or     eax, eax
        jne    SHORT loc_1_FE34
        push   esi
        call   sub_1_FF5F
        add    esp, 4
        sub    eax, eax
        jmp    NEAR PTR loc_1_FEB6
loc_1_FE34:
        mov    dword ptr [esi + 0e4h], 9c40h
loc_1_FE3E:
        push   esi
        call   sub_1_100ED
        add    esp, 4
        or     esi, esi
        je     SHORT loc_1_FEB4
        db  083h, 03Dh, 004h, 02Fh, 001h, 000h, 00Ah
        jae    SHORT loc_1_FE66
        db  0A1h, 004h, 02Fh, 001h, 000h
        db  089h, 034h, 085h, 008h, 02Fh, 001h, 000h
        db  0FFh, 005h, 004h, 02Fh, 001h, 000h
loc_1_FE66:
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [esi + 18h], eax
        mov    dword ptr [esi + 70h], 3e8h
        mov    dword ptr [esi + 80h], 1
        mov    dword ptr [esi + 58h], 1
        push   1
        push   esi
        call   sub_1_11032
        add    esp, 8
        mov    eax, dword ptr [esi + 0b4h]
        add    eax, 9c3fh
        mov    dword ptr [esi + 0c0h], eax
        mov    dword ptr [esi + 0d8h], 0ea60h
        push   esi
        call   sub_1_11F60
        add    esp, 4
loc_1_FEB4:
        mov    eax, esi
loc_1_FEB6:
        pop    edi
        pop    esi
        ret
sub_1_FEB9:
        push   esi
        push   edi
        sub    eax, eax
        db  039h, 005h, 004h, 02Fh, 001h, 000h
        jbe    NEAR PTR loc_1_FF5C
        mov    edx, dword ptr [esp + 0ch]
loc_1_FECD:
        cmp    eax, 0ah
        jae    NEAR PTR loc_1_FF5C
        db  039h, 014h, 085h, 008h, 02Fh, 001h, 000h
        je     SHORT loc_1_FEEA
        inc    eax
        db  03Bh, 005h, 004h, 02Fh, 001h, 000h
        jb     SHORT loc_1_FECD
        jmp    SHORT loc_1_FF5C
loc_1_FEEA:
        sub    edi, edi
        db  039h, 03Dh, 004h, 02Fh, 001h, 000h
        jbe    SHORT loc_1_FF41
loc_1_FEF4:
        cmp    edi, 0ah
        jae    SHORT loc_1_FF41
        db  039h, 014h, 0BDh, 008h, 02Fh, 001h, 000h
        je     SHORT loc_1_FF0D
        inc    edi
        db  03Bh, 03Dh, 004h, 02Fh, 001h, 000h
        jb     SHORT loc_1_FEF4
        jmp    SHORT loc_1_FF41
loc_1_FF0D:
        db  0A1h, 004h, 02Fh, 001h, 000h
        dec    eax
        cmp    eax, edi
        jbe    SHORT loc_1_FF3B
        db  08Dh, 034h, 0BDh, 008h, 02Fh, 001h, 000h
loc_1_FF1E:
        db  081h, 0FEh, 02Ch, 02Fh, 001h, 000h
        jae    SHORT loc_1_FF3B
        mov    ecx, dword ptr [esi + 4]
        lea    eax, [esi + 4]
        mov    dword ptr [esi], ecx
        mov    esi, eax
        inc    edi
        db  0A1h, 004h, 02Fh, 001h, 000h
        dec    eax
        cmp    eax, edi
        ja     SHORT loc_1_FF1E
loc_1_FF3B:
        db  0FFh, 00Dh, 004h, 02Fh, 001h, 000h
loc_1_FF41:
        db  083h, 03Dh, 030h, 02Fh, 001h, 000h, 00Ah
        jae    SHORT loc_1_FF5C
        db  0A1h, 030h, 02Fh, 001h, 000h
        db  089h, 014h, 085h, 034h, 02Fh, 001h, 000h
        db  0FFh, 005h, 030h, 02Fh, 001h, 000h
loc_1_FF5C:
        pop    edi
        pop    esi
        ret
sub_1_FF5F:
        push   esi
        push   edi
        sub    eax, eax
        db  039h, 005h, 004h, 02Fh, 001h, 000h
        mov    esi, dword ptr [esp + 0ch]
        jbe    SHORT loc_1_FF86
loc_1_FF6F:
        cmp    eax, 0ah
        jae    SHORT loc_1_FF86
        db  039h, 034h, 085h, 008h, 02Fh, 001h, 000h
        je     SHORT loc_1_FFB4
        inc    eax
        db  03Bh, 005h, 004h, 02Fh, 001h, 000h
        jb     SHORT loc_1_FF6F
loc_1_FF86:
        sub    eax, eax
        db  039h, 005h, 030h, 02Fh, 001h, 000h
        jbe    NEAR PTR loc_1_100EA
loc_1_FF94:
        cmp    eax, 0ah
        jae    NEAR PTR loc_1_100EA
        db  039h, 034h, 085h, 034h, 02Fh, 001h, 000h
        je     SHORT loc_1_FFB4
        inc    eax
        db  039h, 005h, 030h, 02Fh, 001h, 000h
        ja     SHORT loc_1_FF94
        jmp    NEAR PTR loc_1_100EA
loc_1_FFB4:
        mov    eax, dword ptr [esi + 0a4h]
        or     eax, eax
        je     SHORT loc_1_FFC7
        push   eax
        call   sub_1_F396
        add    esp, 4
loc_1_FFC7:
        mov    eax, dword ptr [esi + 0b4h]
        or     eax, eax
        je     SHORT loc_1_FFDA
        push   eax
        call   sub_1_F396
        add    esp, 4
loc_1_FFDA:
        mov    eax, dword ptr [esi + 0e0h]
        or     eax, eax
        je     SHORT loc_1_FFED
        push   eax
        call   sub_1_F396
        add    esp, 4
loc_1_FFED:
        push   esi
        call   sub_1_F396
        add    esp, 4
        sub    eax, eax
        db  039h, 005h, 004h, 02Fh, 001h, 000h
        jbe    SHORT loc_1_10070
loc_1_10000:
        cmp    eax, 0ah
        jae    SHORT loc_1_10070
        db  039h, 034h, 085h, 008h, 02Fh, 001h, 000h
        je     SHORT loc_1_10019
        inc    eax
        db  03Bh, 005h, 004h, 02Fh, 001h, 000h
        jb     SHORT loc_1_10000
        jmp    SHORT loc_1_10070
loc_1_10019:
        sub    edi, edi
        db  039h, 03Dh, 004h, 02Fh, 001h, 000h
        jbe    SHORT loc_1_10070
loc_1_10023:
        cmp    edi, 0ah
        jae    SHORT loc_1_10070
        db  039h, 034h, 0BDh, 008h, 02Fh, 001h, 000h
        je     SHORT loc_1_1003C
        inc    edi
        db  03Bh, 03Dh, 004h, 02Fh, 001h, 000h
        jb     SHORT loc_1_10023
        jmp    SHORT loc_1_10070
loc_1_1003C:
        db  0A1h, 004h, 02Fh, 001h, 000h
        dec    eax
        cmp    eax, edi
        jbe    SHORT loc_1_1006A
        db  08Dh, 014h, 0BDh, 008h, 02Fh, 001h, 000h
loc_1_1004D:
        db  081h, 0FAh, 02Ch, 02Fh, 001h, 000h
        jae    SHORT loc_1_1006A
        mov    ecx, dword ptr [edx + 4]
        lea    eax, [edx + 4]
        mov    dword ptr [edx], ecx
        mov    edx, eax
        inc    edi
        db  0A1h, 004h, 02Fh, 001h, 000h
        dec    eax
        cmp    eax, edi
        ja     SHORT loc_1_1004D
loc_1_1006A:
        db  0FFh, 00Dh, 004h, 02Fh, 001h, 000h
loc_1_10070:
        sub    eax, eax
        db  039h, 005h, 030h, 02Fh, 001h, 000h
        jbe    SHORT loc_1_100EA
loc_1_1007A:
        cmp    eax, 0ah
        jae    SHORT loc_1_100EA
        db  039h, 034h, 085h, 034h, 02Fh, 001h, 000h
        je     SHORT loc_1_10093
        inc    eax
        db  039h, 005h, 030h, 02Fh, 001h, 000h
        ja     SHORT loc_1_1007A
        jmp    SHORT loc_1_100EA
loc_1_10093:
        sub    edi, edi
        db  039h, 03Dh, 030h, 02Fh, 001h, 000h
        jbe    SHORT loc_1_100EA
loc_1_1009D:
        cmp    edi, 0ah
        jae    SHORT loc_1_100EA
        db  039h, 034h, 0BDh, 034h, 02Fh, 001h, 000h
        je     SHORT loc_1_100B6
        inc    edi
        db  03Bh, 03Dh, 030h, 02Fh, 001h, 000h
        jb     SHORT loc_1_1009D
        jmp    SHORT loc_1_100EA
loc_1_100B6:
        db  0A1h, 030h, 02Fh, 001h, 000h
        dec    eax
        cmp    eax, edi
        jbe    SHORT loc_1_100E4
        db  08Dh, 014h, 0BDh, 034h, 02Fh, 001h, 000h
loc_1_100C7:
        db  081h, 0FAh, 058h, 02Fh, 001h, 000h
        jae    SHORT loc_1_100E4
        mov    ecx, dword ptr [edx + 4]
        lea    eax, [edx + 4]
        mov    dword ptr [edx], ecx
        mov    edx, eax
        inc    edi
        db  0A1h, 030h, 02Fh, 001h, 000h
        dec    eax
        cmp    eax, edi
        ja     SHORT loc_1_100C7
loc_1_100E4:
        db  0FFh, 00Dh, 030h, 02Fh, 001h, 000h
loc_1_100EA:
        pop    edi
        pop    esi
        ret
sub_1_100ED:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 20h]
        mov    eax, dword ptr [edx + 0a4h]
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [edx + 0ach]
        mov    edi, edx
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [edx + 0e0h]
        mov    dword ptr [esp + 14h], eax
        mov    esi, dword ptr [edx + 0a8h]
        sub    eax, eax
        mov    ebx, dword ptr [edx + 0b0h]
        mov    ebp, dword ptr [edx + 0e4h]
        mov    ecx, 6eh
        rep stosd
        stosw
        mov    edi, dword ptr [esp + 18h]
        mov    ecx, esi
        shr    ecx, 2
        rep stosd
        mov    ecx, esi
        and    ecx, 3
        rep stosb
        mov    edi, dword ptr [esp + 10h]
        mov    ecx, ebx
        shr    ecx, 2
        rep stosd
        mov    ecx, ebx
        and    ecx, 3
        rep stosb
        mov    edi, dword ptr [esp + 14h]
        mov    ecx, ebp
        shr    ecx, 2
        rep stosd
        mov    ecx, ebp
        and    ecx, 3
        rep stosb
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [edx + 0a4h], eax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [edx + 0ach], eax
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [edx + 0e0h], eax
        mov    dword ptr [edx + 0a8h], esi
        mov    dword ptr [edx + 0b0h], ebx
        mov    dword ptr [edx + 0e4h], ebp
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [edx + 0b4h], eax
        mov    dword ptr [edx + 0b8h], ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_1_101B3:
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    esi, esi
        mov    ebx, dword ptr [esp + 14h]
        push   ebx
        call   sub_1_FCFF
        mov    edi, dword ptr [esp + 1ch]
        add    esp, 4
        mov    dword ptr [edi], eax
        mov    ecx, eax
        cmp    ecx, esi
        jne    SHORT loc_1_101DC
        mov    esi, 3
        jmp    NEAR PTR loc_1_1025F
loc_1_101DC:
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [ecx + 74h], eax
        mov    dword ptr [ecx + 50h], 0
        mov    dword ptr [ecx + 54h], 0
        cmp    ebx, -1fh
        jb     SHORT loc_1_101FD
        mov    esi, 4
        jmp    SHORT loc_1_1025F
loc_1_101FD:
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 14h], eax
        push   0
        db  068h, 070h, 015h, 001h, 000h
        push   0effffff1h
        push   eax
        call   sub_1_12958
        add    esp, 10h
        cmp    eax, 1
        je     SHORT loc_1_10224
        mov    esi, 4
        jmp    SHORT loc_1_1025F
loc_1_10224:
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [ecx + 1b2h], eax
        mov    ebx, dword ptr [esp + 20h]
        mov    dword ptr [ecx + 5ch], ebx
        push   ecx
        call   sub_1_1180D
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_1024B
        mov    esi, 4
        jmp    SHORT loc_1_1025F
loc_1_1024B:
        mov    ebp, dword ptr [edi]
        mov    dword ptr [ebp + 4ch], 1
        mov    dword ptr [ebp + 5ch], ebx
        call   sub_1_129A4
        mov    dword ptr [ebp], eax
loc_1_1025F:
        or     esi, esi
        je     SHORT loc_1_10272
        mov    eax, dword ptr [edi]
        or     eax, eax
        je     SHORT loc_1_10272
        push   eax
        call   sub_1_FBE0
        add    esp, 4
loc_1_10272:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_10279:
        sub    esp, 38h
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebp, ebp
        mov    dword ptr [esp + 18h], 30h
        mov    ebx, dword ptr [esp + 4ch]
        lea    eax, [esp + 18h]
        push   eax
        push   dword ptr [ebx + 14h]
        call   sub_1_12938
        lea    esi, [esp + 20h]
        add    esp, 8
        lea    edi, [ebx + 1ch]
        mov    ecx, 0ch
        rep movsd
        cmp    dword ptr [esp + 1ch], 2580h
        jae    SHORT loc_1_102BE
        mov    dword ptr [esp + 1ch], 2580h
loc_1_102BE:
        and    dword ptr [esp + 20h], 0fffff51dh
        or     dword ptr [esp + 20h], 119h
        mov    byte ptr [esp + 2eh], 8
        mov    byte ptr [esp + 2fh], 0
        mov    byte ptr [esp + 30h], 0
        lea    eax, [esp + 18h]
        push   73h
        push   eax
        push   dword ptr [ebx + 14h]
        call   sub_1_12930
        lea    eax, [esp + 1ch]
        add    esp, 0ch
        push   eax
        push   dword ptr [ebx + 0e4h]
        push   dword ptr [ebx + 0e0h]
        push   dword ptr [ebx + 0a8h]
        push   dword ptr [ebx + 0a4h]
        push   dword ptr [ebx + 14h]
        call   sub_1_12940
        add    esp, 18h
        or     eax, eax
        push   ebx
        jne    SHORT loc_1_1032B
        call   sub_1_10A0C
        add    esp, 4
        mov    ebp, 4
        jmp    SHORT loc_1_1037E
loc_1_1032B:
        db  068h, 010h, 015h, 001h, 000h
        push   50h
        push   dword ptr [ebx + 14h]
        call   sub_1_12990
        add    esp, 10h
        or     eax, eax
        push   ebx
        jne    SHORT loc_1_10351
        call   sub_1_10A0C
        add    esp, 4
        mov    ebp, 4
        jmp    SHORT loc_1_1037E
loc_1_10351:
        call   sub_1_11E7C
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_10364
        mov    ebp, 4
        jmp    SHORT loc_1_1037E
loc_1_10364:
        push   0
        push   0
        push   15h
        push   dword ptr [ebx + 14h]
        call   sub_1_12958
        add    esp, 10h
        push   ebx
        call   sub_1_10A0C
        add    esp, 4
loc_1_1037E:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
sub_1_10388:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   73h
        lea    eax, [esi + 1ch]
        push   eax
        push   dword ptr [esi + 14h]
        call   sub_1_12930
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_103AB
        push   esi
        call   sub_1_10A0C
        add    esp, 4
loc_1_103AB:
        push   0
        push   0
        push   dword ptr [esi + 14h]
        call   sub_1_12980
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_103C7
        push   esi
        call   sub_1_10A0C
        add    esp, 4
loc_1_103C7:
        push   0
        push   0
        push   -1
        push   dword ptr [esi + 14h]
        call   sub_1_12990
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_103E5
        push   esi
        call   sub_1_10A0C
        add    esp, 4
loc_1_103E5:
        push   0
        push   0
        push   10h
        push   dword ptr [esi + 14h]
        call   sub_1_12958
        add    esp, 10h
        sub    eax, eax
        pop    esi
        ret
sub_1_103FA:
        ret
        db  0C3h, 0C3h
sub_1_103FD:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 10h], esi
        push   4
        call   sub_1_12998
        db  066h, 083h, 03Dh, 020h, 035h, 001h, 000h, 000h
        je     SHORT loc_1_1042A
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        sub    eax, eax
        jmp    NEAR PTR loc_1_10654
loc_1_1042A:
        db  066h, 0FFh, 005h, 020h, 035h, 001h, 000h
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebx + 0e8h], 0
        sub    eax, eax
        db  039h, 005h, 004h, 02Fh, 001h, 000h
        jbe    NEAR PTR loc_1_10647
loc_1_10452:
        cmp    eax, 0ah
        jae    NEAR PTR loc_1_10647
        db  039h, 01Ch, 085h, 008h, 02Fh, 001h, 000h
        je     SHORT loc_1_10472
        inc    eax
        db  03Bh, 005h, 004h, 02Fh, 001h, 000h
        jb     SHORT loc_1_10452
        jmp    NEAR PTR loc_1_10647
loc_1_10472:
        cmp    dword ptr [ebx + 50h], 0
        je     NEAR PTR loc_1_10647
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_10628
        or     esi, esi
        je     NEAR PTR loc_1_10628
        cmp    dword ptr [ebx + 0f0h], 0
        jne    SHORT loc_1_104E5
        mov    eax, dword ptr [ebx + 80h]
        cmp    eax, 7
        je     NEAR PTR loc_1_10647
        cmp    eax, 8
        je     NEAR PTR loc_1_10647
        cmp    eax, 9
        je     NEAR PTR loc_1_10647
        mov    dword ptr [ebx + 0f0h], 1
        mov    dword ptr [ebx + 58h], 0
        push   ebx
        call   sub_1_11852
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_104E5
        mov    dword ptr [ebx + 0f0h], 0
        jmp    NEAR PTR loc_1_10647
loc_1_104E5:
        cmp    dword ptr [ebx + 0ech], 0
        je     SHORT loc_1_10528
        db  0BEh, 024h, 035h, 001h, 000h
        lea    edi, [ebp - 4]
        movsw
        mov    al, byte ptr [ebx + 0ech]
        mov    byte ptr [ebp - 4], al
        lea    eax, [ebp - 18h]
        push   eax
        push   1
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        call   sub_1_11B90
        add    esp, 10h
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_1_10602
        mov    dword ptr [ebx + 0ech], 0
loc_1_10528:
        sub    edi, edi
loc_1_1052A:
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp + 0ch]
        call   sub_1_10996
        add    esp, 10h
        mov    esi, eax
        cmp    esi, edi
        je     SHORT loc_1_10554
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 1], al
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], 10h
loc_1_10554:
        lea    eax, [ebp - 0ch]
        push   eax
        push   dword ptr [ebp - 14h]
        push   dword ptr [ebp + 0ch]
        push   ebx
        call   sub_1_11B90
        add    esp, 10h
        cmp    eax, edi
        je     NEAR PTR loc_1_10611
        cmp    esi, edi
        je     SHORT loc_1_1057B
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ecx], al
loc_1_1057B:
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [ebp - 0ch], eax
        jne    SHORT loc_1_105B3
        cmp    esi, edi
        je     SHORT loc_1_105B3
        cmp    byte ptr [ebp - 1], 10h
        je     SHORT loc_1_10591
        or     byte ptr [ebp - 1], 80h
loc_1_10591:
        lea    eax, [ebp - 18h]
        push   eax
        push   1
        lea    eax, [ebp - 1]
        push   eax
        push   ebx
        call   sub_1_11B90
        add    esp, 10h
        cmp    dword ptr [ebp - 18h], edi
        jne    SHORT loc_1_105B3
        movsx  eax, byte ptr [ebp - 1]
        mov    dword ptr [ebx + 0ech], eax
loc_1_105B3:
        mov    eax, dword ptr [ebp - 8]
        inc    eax
        mov    dword ptr [ebp + 0ch], eax
        mov    eax, dword ptr [ebp - 10h]
        sub    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 0ch], edi
        jne    SHORT loc_1_105D1
        push   ebx
        call   sub_1_10A0C
        add    esp, 4
loc_1_105D1:
        mov    eax, dword ptr [ebp - 0ch]
        add    dword ptr [ebx + 0e8h], eax
        cmp    esi, edi
        jne    NEAR PTR loc_1_1052A
        mov    eax, dword ptr [ebp + 14h]
        or     eax, eax
        je     SHORT loc_1_10647
        or     esi, esi
        jne    SHORT loc_1_10647
        cmp    dword ptr [ebx + 0f0h], 0
        je     SHORT loc_1_10647
        push   eax
        push   ebx
        call   sub_1_1065B
        add    esp, 8
        jmp    SHORT loc_1_10647
loc_1_10602:
        cmp    dword ptr [ebx + 1b6h], 0
        je     NEAR PTR loc_1_10528
        jmp    SHORT loc_1_1063D
loc_1_10611:
        or     esi, esi
        je     SHORT loc_1_1061D
        mov    eax, dword ptr [ebp - 8]
        mov    cl, byte ptr [ebp - 1]
        mov    byte ptr [eax], cl
loc_1_1061D:
        push   ebx
        call   sub_1_10A0C
        add    esp, 4
        jmp    SHORT loc_1_10647
loc_1_10628:
        cmp    dword ptr [ebx + 0f0h], 0
        je     SHORT loc_1_1063D
        push   dword ptr [ebp + 14h]
        push   ebx
        call   sub_1_1065B
        add    esp, 8
loc_1_1063D:
        mov    dword ptr [ebx + 0e8h], 0
loc_1_10647:
        db  066h, 0FFh, 00Dh, 020h, 035h, 001h, 000h
        mov    eax, dword ptr [ebx + 0e8h]
loc_1_10654:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_1065B:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        sub    eax, eax
        db  039h, 005h, 004h, 02Fh, 001h, 000h
        jbe    SHORT loc_1_10689
        mov    esi, dword ptr [esp + 18h]
loc_1_10672:
        cmp    eax, 0ah
        jae    SHORT loc_1_10689
        db  039h, 034h, 085h, 008h, 02Fh, 001h, 000h
        je     SHORT loc_1_10698
        inc    eax
        db  03Bh, 005h, 004h, 02Fh, 001h, 000h
        jb     SHORT loc_1_10672
loc_1_10689:
        mov    ebx, 5
loc_1_1068E:
        mov    eax, ebx
loc_1_10690:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_10698:
        mov    ebp, dword ptr [esp + 1ch]
        or     ebp, ebp
        jne    SHORT loc_1_106A4
        sub    eax, eax
        jmp    SHORT loc_1_10690
loc_1_106A4:
        mov    eax, ebp
        imul   eax, eax, 9
        mov    ecx, 0ffffffffh
        db  08Bh, 090h, 0B5h, 02Eh, 001h, 000h
        lea    eax, [esp + 10h]
        push   eax
        mov    edi, edx
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        push   ecx
        push   edx
        push   esi
        call   sub_1_11B90
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_10740
        cmp    dword ptr [esp + 10h], 2
        je     SHORT loc_1_106E1
        push   esi
        call   sub_1_10A0C
        add    esp, 4
loc_1_106E1:
        cmp    ebp, 1
        je     SHORT loc_1_106FA
        cmp    ebp, 2
        jb     SHORT loc_1_1068E
        cmp    ebp, 3
        jbe    SHORT loc_1_10729
        cmp    ebp, 4
        jb     SHORT loc_1_1068E
        cmp    ebp, 5
        ja     SHORT loc_1_1068E
loc_1_106FA:
        mov    dword ptr [esi + 0f0h], 0
        push   1
        push   esi
        call   sub_1_11032
        add    esp, 8
        mov    dword ptr [esi + 80h], 7
        push   esi
        call   sub_1_10A83
        add    esp, 4
        sub    eax, eax
        jmp    NEAR PTR loc_1_10690
loc_1_10729:
        mov    dword ptr [esi + 58h], 1
        push   3
        push   esi
        call   sub_1_11032
        add    esp, 8
        jmp    NEAR PTR loc_1_1068E
loc_1_10740:
        mov    ebx, 4
        jmp    NEAR PTR loc_1_1068E
sub_1_1074A:
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        mov    esi, dword ptr [esp + 30h]
        sub    ebp, ebp
        mov    eax, dword ptr [esi + 0e4h]
        mov    edi, dword ptr [esp + 34h]
        cmp    eax, edi
        jb     SHORT loc_1_10769
        mov    eax, edi
loc_1_10769:
        imul   eax, eax, 5
        mov    dword ptr [esp + 10h], eax
        mov    dword ptr [esp + 1ch], 0
loc_1_10778:
        push   0
        lea    eax, [esp + 24h]
        push   dword ptr [esp + 14h]
        push   eax
        call   sub_1_11CB8
        lea    eax, [esp + 20h]
        add    esp, 0ch
        push   eax
        push   dword ptr [esi + 14h]
        call   sub_1_12948
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_107C8
        cmp    dword ptr [esp + 1ch], edi
        jae    SHORT loc_1_107C1
        mov    eax, dword ptr [esp + 1ch]
        cmp    dword ptr [esi + 0e4h], eax
        jbe    SHORT loc_1_107C1
        cmp    ebp, eax
        jne    SHORT loc_1_107BB
        cmp    ebx, 3
        ja     SHORT loc_1_107C8
        inc    ebx
loc_1_107BB:
        mov    ebp, dword ptr [esp + 1ch]
        jmp    SHORT loc_1_10778
loc_1_107C1:
        mov    eax, 1
        jmp    SHORT loc_1_107CA
loc_1_107C8:
        sub    eax, eax
loc_1_107CA:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
sub_1_107D2:
        sub    eax, eax
        db  039h, 005h, 004h, 02Fh, 001h, 000h
        jbe    SHORT loc_1_107F7
        mov    ecx, dword ptr [esp + 4]
loc_1_107E0:
        cmp    eax, 0ah
        jae    SHORT loc_1_107F7
        db  039h, 00Ch, 085h, 008h, 02Fh, 001h, 000h
        je     SHORT loc_1_107FD
        inc    eax
        db  03Bh, 005h, 004h, 02Fh, 001h, 000h
        jb     SHORT loc_1_107E0
loc_1_107F7:
        mov    eax, 5
loc_1_107FC:
        ret
loc_1_107FD:
        mov    dword ptr [ecx + 80h], 1
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [ecx + 0b4h], eax
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [ecx + 0b8h], eax
        sub    eax, eax
        jmp    SHORT loc_1_107FC
        db  056h, 02Bh, 0C0h, 039h, 005h, 004h, 02Fh, 001h, 000h, 076h, 01Bh, 08Bh, 074h, 024h, 008h, 083h
        db  0F8h, 00Ah, 073h, 012h, 039h, 034h, 085h, 008h, 02Fh, 001h, 000h, 074h, 010h, 040h, 03Bh, 005h
        db  004h, 02Fh, 001h, 000h, 072h, 0E9h, 0B8h, 005h, 000h, 000h, 000h, 05Eh, 0C3h, 068h, 028h, 035h
        db  001h, 000h, 06Ah, 00Eh, 056h, 0E8h, 0F2h, 010h, 000h, 000h, 083h, 0C4h, 00Ch, 00Bh, 0C0h, 074h
        db  012h, 056h, 0E8h, 007h, 012h, 000h, 000h, 083h, 0C4h, 004h, 083h, 0F8h, 002h, 075h, 004h, 02Bh
        db  0C0h, 0EBh, 0D8h, 0B8h, 002h, 000h, 000h, 000h, 0EBh, 0D1h
sub_1_10879:
        sub    esp, 0ch
        push   esi
        sub    eax, eax
        db  039h, 005h, 004h, 02Fh, 001h, 000h
        jbe    SHORT loc_1_108A2
        mov    esi, dword ptr [esp + 14h]
loc_1_1088B:
        cmp    eax, 0ah
        jae    SHORT loc_1_108A2
        db  039h, 034h, 085h, 008h, 02Fh, 001h, 000h
        je     SHORT loc_1_108AC
        inc    eax
        db  03Bh, 005h, 004h, 02Fh, 001h, 000h
        jb     SHORT loc_1_1088B
loc_1_108A2:
        mov    eax, 5
loc_1_108A7:
        pop    esi
        add    esp, 0ch
        ret
loc_1_108AC:
        mov    ecx, dword ptr [esi + 0f0h]
        or     ecx, ecx
        jne    SHORT loc_1_108D2
        mov    eax, dword ptr [esi + 80h]
        cmp    eax, 7
        je     SHORT loc_1_108CB
        cmp    eax, 8
        je     SHORT loc_1_108CB
        cmp    eax, 9
        jne    SHORT loc_1_108D2
loc_1_108CB:
        mov    eax, 2
        jmp    SHORT loc_1_108A7
loc_1_108D2:
        cmp    dword ptr [esi + 7ch], 9
        jne    SHORT loc_1_108E3
        or     ecx, ecx
        je     SHORT loc_1_108E3
        mov    eax, 2
        jmp    SHORT loc_1_108A7
loc_1_108E3:
        lea    eax, [esp + 4]
        push   eax
        push   dword ptr [esi + 14h]
        call   sub_1_12948
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_108FB
        sub    eax, eax
        jmp    SHORT loc_1_108A7
loc_1_108FB:
        cmp    dword ptr [esp + 8], 0
        je     SHORT loc_1_10918
        mov    eax, dword ptr [esp + 18h]
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [eax], 1
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_10948
loc_1_10918:
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_1_10935
        mov    eax, dword ptr [esp + 18h]
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [eax], 2
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_10948
loc_1_10935:
        cmp    dword ptr [esi + 0f0h], 0
        je     SHORT loc_1_10952
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [eax], 3
loc_1_10948:
        mov    eax, 1
        jmp    NEAR PTR loc_1_108A7
loc_1_10952:
        sub    eax, eax
        jmp    NEAR PTR loc_1_108A7
        db  056h, 057h, 08Bh, 074h, 024h, 010h, 056h, 08Bh, 07Ch, 024h, 010h, 06Ah, 010h, 057h, 0E8h, 0C4h
        db  01Dh, 000h, 000h, 08Bh, 04Ch, 024h, 020h, 083h, 0C4h, 00Ch, 089h, 001h, 08Bh, 0C8h, 00Bh, 0C9h
        db  074h, 010h, 02Bh, 0CFh, 08Bh, 044h, 024h, 018h, 041h, 089h, 008h, 0B8h, 001h, 000h, 000h, 000h
        db  0EBh, 008h, 08Bh, 044h, 024h, 018h, 089h, 030h, 02Bh, 0C0h, 05Fh, 05Eh, 0C3h
sub_1_10996:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 18h]
        push   edi
        mov    esi, dword ptr [esp + 18h]
        push   10h
        sub    ebx, ebx
        push   esi
        call   sub_1_12730
        add    esp, 0ch
        cmp    eax, ebx
        mov    ebp, dword ptr [esp + 1ch]
        je     SHORT loc_1_109C5
        mov    dword ptr [ebp], eax
        sub    eax, esi
        mov    ebx, 1
        lea    edi, [eax + 1]
loc_1_109C5:
        push   edi
        push   11h
        push   esi
        call   sub_1_12730
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_109E2
        mov    dword ptr [ebp], eax
        sub    eax, esi
        mov    ebx, 1
        lea    edi, [eax + 1]
loc_1_109E2:
        push   edi
        push   13h
        push   esi
        call   sub_1_12730
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_109FF
        mov    dword ptr [ebp], eax
        sub    eax, esi
        mov    ebx, 1
        lea    edi, [eax + 1]
loc_1_109FF:
        mov    eax, dword ptr [esp + 20h]
        pop    ebp
        mov    dword ptr [eax], edi
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_10A0C:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        lea    edi, [esi + 1b6h]
        push   edi
        push   0
        push   dword ptr [esi + 14h]
        call   sub_1_12950
        add    esp, 0ch
        cmp    dword ptr [edi], 0
        je     SHORT loc_1_10A32
        mov    dword ptr [esi + 6ch], 4
loc_1_10A32:
        pop    edi
        pop    esi
        ret
sub_1_10A35:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 19eh]
        or     eax, eax
        je     SHORT loc_1_10A54
        push   eax
        call   sub_1_129E8
        mov    dword ptr [esi + 19eh], 0
loc_1_10A54:
        pop    esi
        ret
sub_1_10A56:
        push   esi
        mov    eax, dword ptr [esp + 8]
        cmp    dword ptr [eax + 19eh], 0
        lea    esi, [eax + 0f4h]
        jne    SHORT loc_1_10A81
        push   eax
        push   dword ptr [esi + 0b2h]
        db  068h, 0B0h, 00Ah, 001h, 000h
        call   sub_1_129CC
        mov    dword ptr [esi + 0aah], eax
loc_1_10A81:
        pop    esi
        ret
sub_1_10A83:
        push   esi
        mov    eax, dword ptr [esp + 8]
        cmp    dword ptr [eax + 19eh], 0
        lea    esi, [eax + 0f4h]
        jne    SHORT loc_1_10AAE
        push   eax
        push   dword ptr [esi + 0b2h]
        db  068h, 0B0h, 00Ah, 001h, 000h
        call   sub_1_129CC
        mov    dword ptr [esi + 0aah], eax
loc_1_10AAE:
        pop    esi
        ret
loc_1_10AB0:
        sub    esp, 10h
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 19eh], 0
        mov    eax, dword ptr [esp]
        cmp    dword ptr [eax + 0ch], 0
        jne    SHORT loc_1_10B1A
        lea    eax, [esp + 4]
        push   eax
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax + 14h]
        call   sub_1_12948
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_10B0F
        cmp    dword ptr [esp + 8], 0
        je     SHORT loc_1_10B1A
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 4], 1
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 8], 0
        mov    edx, dword ptr [esp]
        call   sub_1_10D56
        jmp    SHORT loc_1_10B26
loc_1_10B0F:
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 6ch], 1
loc_1_10B1A:
        push   dword ptr [esp]
        call   sub_1_10A56
        add    esp, 4
loc_1_10B26:
        add    esp, 10h
        ret
        db  08Bh, 044h, 024h, 004h, 08Bh, 080h, 088h, 000h, 000h, 000h, 048h, 083h, 0F8h, 001h, 01Bh, 0C0h
        db  0F7h, 0D8h, 0C3h
loc_1_10B3D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    dword ptr [ebp - 4], edx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 1a2h], 0
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0ch], 0
        je     SHORT loc_1_10B65
        push   eax
        call   sub_1_10BC5
        add    esp, 4
loc_1_10B65:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 88h], 1
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8ch]
        or     eax, eax
        je     SHORT loc_1_10B8B
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        db  0CDh, 020h, 028h, 000h, 001h, 000h
loc_1_10B8B:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0fah], 13h
        push   dword ptr [ebp - 4]
        call   sub_1_124D7
        mov    esp, ebp
        pop    ebp
        ret
sub_1_10BA4:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 1a2h]
        or     eax, eax
        je     SHORT loc_1_10BC3
        push   eax
        call   sub_1_129E8
        mov    dword ptr [esi + 1a2h], 0
loc_1_10BC3:
        pop    esi
        ret
sub_1_10BC5:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        cmp    dword ptr [esi + 1a2h], 0
        lea    edi, [esi + 0f4h]
        je     SHORT loc_1_10BE3
        push   esi
        call   sub_1_10BA4
        add    esp, 4
loc_1_10BE3:
        mov    dword ptr [esi + 88h], 0
        push   esi
        push   dword ptr [edi + 0b6h]
        db  068h, 03Dh, 00Bh, 001h, 000h
        call   sub_1_129CC
        mov    dword ptr [edi + 0aeh], eax
        pop    edi
        pop    esi
        ret
sub_1_10C07:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 24h]
        sub    ebp, ebp
        mov    dword ptr [ebx + 0c4h], ebp
        mov    dword ptr [ebx + 0bch], ebp
        mov    eax, dword ptr [ebx + 0b4h]
        mov    dword ptr [ebx + 0f4h], eax
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [ebx + 14h]
        call   sub_1_12948
        add    esp, 8
        cmp    eax, ebp
        jne    SHORT loc_1_10C53
        push   ebx
        call   sub_1_10A0C
        add    esp, 4
        mov    ebp, 3
        jmp    NEAR PTR loc_1_10D3E
loc_1_10C53:
        mov    edx, dword ptr [ebx + 0c4h]
        mov    ecx, dword ptr [ebx + 0b8h]
        sub    ecx, edx
        mov    eax, dword ptr [esp + 18h]
        cmp    ecx, eax
        jb     SHORT loc_1_10C6B
        mov    ecx, eax
loc_1_10C6B:
        lea    eax, [esp + 10h]
        push   eax
        push   ecx
        mov    eax, dword ptr [ebx + 0b4h]
        add    eax, edx
        push   eax
        push   ebx
        call   sub_1_11AB0
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_10C9B
        mov    ebp, 3
        cmp    dword ptr [ebx + 0c4h], 0
        je     NEAR PTR loc_1_10D3E
        jmp    SHORT loc_1_10CA5
loc_1_10C9B:
        mov    eax, dword ptr [esp + 10h]
        add    dword ptr [ebx + 0c4h], eax
loc_1_10CA5:
        db  0BFh, 02Ch, 035h, 001h, 000h
        mov    ecx, 2
        mov    esi, dword ptr [ebx + 0b4h]
        repe cmpsd
        jne    SHORT loc_1_10CBD
        mov    al, byte ptr [esi]
        cmp    al, byte ptr [edi]
loc_1_10CBD:
        je     SHORT loc_1_10D13
        db  0BFh, 038h, 035h, 001h, 000h
        mov    ecx, 3
        mov    esi, dword ptr [ebx + 0b4h]
        repe cmpsd
        jne    SHORT loc_1_10CD9
        mov    ax, word ptr [esi]
        cmp    ax, word ptr [edi]
loc_1_10CD9:
        je     SHORT loc_1_10D13
        db  0BFh, 048h, 035h, 001h, 000h
        mov    ecx, 2
        mov    esi, dword ptr [ebx + 0b4h]
        repe cmpsd
        jne    SHORT loc_1_10CFD
        mov    ax, word ptr [esi]
        cmp    ax, word ptr [edi]
        jne    SHORT loc_1_10CFD
        mov    al, byte ptr [esi + 2]
        cmp    al, byte ptr [edi + 2]
loc_1_10CFD:
        je     SHORT loc_1_10D13
        db  0BFh, 054h, 035h, 001h, 000h
        mov    ecx, 7
        mov    esi, dword ptr [ebx + 0b4h]
        sub    eax, eax
        repe cmpsb
loc_1_10D13:
        mov    eax, dword ptr [ebx + 0c4h]
        or     eax, eax
        je     SHORT loc_1_10D34
        mov    ecx, dword ptr [ebx + 0b4h]
        add    eax, ecx
        dec    eax
        mov    dword ptr [ebx + 0bch], eax
        mov    dword ptr [ebx + 0f4h], ecx
        jmp    SHORT loc_1_10D3E
loc_1_10D34:
        mov    dword ptr [ebx + 0bch], 0
loc_1_10D3E:
        or     ebp, ebp
        je     SHORT loc_1_10D4E
        push   ebx
        call   sub_1_10A0C
        add    esp, 4
        mov    dword ptr [ebx + 6ch], ebp
loc_1_10D4E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
sub_1_10D56:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 10h], edx
        mov    eax, dword ptr [esp + 10h]
        inc    dword ptr [eax + 0ch]
        mov    ecx, dword ptr [esp + 10h]
        add    ecx, 0ch
        mov    eax, dword ptr [ecx]
        cmp    eax, 1
        jg     NEAR PTR loc_1_10FD9
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 4]
        cmp    eax, 1
        je     SHORT loc_1_10D9D
        cmp    eax, 2
        je     NEAR PTR loc_1_10F29
        cmp    eax, 4
        je     NEAR PTR loc_1_10F70
        jmp    NEAR PTR loc_1_10F7C
loc_1_10D9D:
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [eax + 54h], 0
        je     NEAR PTR loc_1_10F0B
        lea    eax, [esp + 14h]
        push   eax
        mov    eax, dword ptr [esp + 14h]
        push   dword ptr [eax + 14h]
        call   sub_1_12948
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_1_10F0B
        cmp    dword ptr [esp + 18h], 0
        je     NEAR PTR loc_1_10F17
        push   dword ptr [esp + 10h]
        call   sub_1_11E18
        add    esp, 4
        push   dword ptr [esp + 10h]
        call   sub_1_10C07
        add    esp, 4
        mov    ebx, 32h
        push   dword ptr [esp + 10h]
        call   sub_1_122AE
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_1_10F17
        sub    ebp, ebp
loc_1_10E05:
        dec    ebx
        je     NEAR PTR loc_1_10F17
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 78h]
        cmp    eax, 1
        je     NEAR PTR loc_1_10ECD
        cmp    eax, 2
        je     SHORT loc_1_10E34
        cmp    eax, 3
        je     SHORT loc_1_10E56
        cmp    eax, 4
        je     NEAR PTR loc_1_10EFB
        jmp    NEAR PTR loc_1_10EE5
loc_1_10E34:
        push   dword ptr [esp + 10h]
        call   sub_1_1206F
        add    esp, 4
        cmp    eax, ebp
        je     NEAR PTR loc_1_10EE5
        push   dword ptr [esp + 10h]
        call   sub_1_123A0
        jmp    NEAR PTR loc_1_10EE2
loc_1_10E56:
        mov    eax, dword ptr [esp + 10h]
        db  0BFh, 05Ch, 035h, 001h, 000h
        mov    ecx, 2
        mov    esi, dword ptr [eax + 0b4h]
        repe cmpsd
        jne    SHORT loc_1_10E72
        mov    al, byte ptr [esi]
        cmp    al, byte ptr [edi]
loc_1_10E72:
        je     SHORT loc_1_10ECD
        push   dword ptr [esp + 10h]
        call   sub_1_11076
        mov    eax, dword ptr [esp + 14h]
        add    esp, 4
        mov    ecx, dword ptr [eax + 1b2h]
        cmp    ecx, ebp
        je     SHORT loc_1_10EBD
        cmp    dword ptr [eax + 0bch], ebp
        je     SHORT loc_1_10EE5
        mov    edx, dword ptr [eax + 0f4h]
        mov    eax, dword ptr [eax + 0b4h]
        cmp    edx, eax
        jbe    SHORT loc_1_10EE5
        sub    edx, eax
        mov    eax, dword ptr [esp + 10h]
        push   dword ptr [eax + 5ch]
        push   edx
        push   1
        push   dword ptr [esp + 1ch]
        call   ecx
        add    esp, 10h
        jmp    SHORT loc_1_10EE5
loc_1_10EBD:
        push   1
        push   dword ptr [esp + 14h]
        call   sub_1_11032
        add    esp, 8
        jmp    SHORT loc_1_10EE5
loc_1_10ECD:
        push   dword ptr [esp + 10h]
        call   sub_1_1206F
        add    esp, 4
        push   dword ptr [esp + 10h]
        call   sub_1_124D7
loc_1_10EE2:
        add    esp, 4
loc_1_10EE5:
        push   dword ptr [esp + 10h]
        call   sub_1_122AE
        add    esp, 4
        cmp    eax, ebp
        je     NEAR PTR loc_1_10E05
        jmp    SHORT loc_1_10F17
loc_1_10EFB:
        push   3
        push   dword ptr [esp + 14h]
        call   sub_1_11032
        add    esp, 8
        jmp    SHORT loc_1_10F17
loc_1_10F0B:
        push   dword ptr [esp + 10h]
        call   sub_1_10A0C
        add    esp, 4
loc_1_10F17:
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [eax + 7ch], 9
        je     SHORT loc_1_10F7C
        push   eax
        call   sub_1_10A56
        jmp    SHORT loc_1_10F79
loc_1_10F29:
        lea    eax, [esp + 14h]
        push   eax
        mov    eax, dword ptr [esp + 14h]
        push   dword ptr [eax + 14h]
        call   sub_1_12948
        add    esp, 8
        or     eax, eax
        mov    eax, dword ptr [esp + 10h]
        je     SHORT loc_1_10F67
        mov    eax, dword ptr [eax + 1b2h]
        or     eax, eax
        je     SHORT loc_1_10F7C
        mov    ecx, dword ptr [esp + 10h]
        push   dword ptr [ecx + 5ch]
        push   dword ptr [esp + 20h]
        push   2
        push   dword ptr [esp + 1ch]
        call   eax
        add    esp, 10h
        jmp    SHORT loc_1_10F7C
loc_1_10F67:
        mov    dword ptr [eax + 6ch], 1
        jmp    SHORT loc_1_10F7C
loc_1_10F70:
        push   dword ptr [esp + 10h]
        call   sub_1_10FF0
loc_1_10F79:
        add    esp, 4
loc_1_10F7C:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [eax + 54h], 0
        je     SHORT loc_1_10FC4
        lea    eax, [esp + 14h]
        push   eax
        mov    eax, dword ptr [esp + 14h]
        push   dword ptr [eax + 14h]
        call   sub_1_12948
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_10FC4
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_1_10FD0
        mov    eax, dword ptr [esp + 10h]
        dec    dword ptr [eax + 0ch]
        jmp    SHORT loc_1_10FDC
loc_1_10FC4:
        push   dword ptr [esp + 10h]
        call   sub_1_10A0C
        add    esp, 4
loc_1_10FD0:
        mov    eax, dword ptr [esp + 10h]
        dec    dword ptr [eax + 0ch]
        jmp    SHORT loc_1_10FE8
loc_1_10FD9:
        dec    eax
        mov    dword ptr [ecx], eax
loc_1_10FDC:
        push   dword ptr [esp + 10h]
        call   sub_1_11E1A
        add    esp, 4
loc_1_10FE8:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
sub_1_10FF0:
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esi + 8]
        test   edi, 1
        je     SHORT loc_1_1101B
        mov    dword ptr [esi + 4], 1
        mov    dword ptr [esi + 8], 0
        push   esi
        call   sub_1_10A56
        add    esp, 4
loc_1_1101B:
        lea    eax, [esp + 8]
        push   eax
        push   edi
        push   dword ptr [esi + 14h]
        call   sub_1_12968
        add    esp, 0ch
        pop    edi
        pop    esi
        add    esp, 4
        ret
sub_1_11032:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        cmp    edi, 1
        je     SHORT loc_1_1104C
        cmp    edi, 2
        je     SHORT loc_1_11061
        mov    esi, dword ptr [esp + 0ch]
loc_1_11046:
        mov    dword ptr [esi + 78h], edi
        pop    edi
        pop    esi
        ret
loc_1_1104C:
        mov    esi, dword ptr [esp + 0ch]
        cmp    dword ptr [esi + 78h], 3
        jne    SHORT loc_1_11046
        push   esi
        call   sub_1_103FA
        add    esp, 4
        jmp    SHORT loc_1_11046
loc_1_11061:
        mov    esi, dword ptr [esp + 0ch]
        cmp    dword ptr [esi + 78h], 3
        jne    SHORT loc_1_11046
        push   esi
        call   sub_1_103FA
        add    esp, 4
        jmp    SHORT loc_1_11046
sub_1_11076:
        sub    esp, 1ch
        push   esi
        push   edi
        mov    dword ptr [esp + 20h], 0
        mov    esi, dword ptr [esp + 28h]
        mov    ecx, dword ptr [esi + 0f4h]
        add    esi, 0f4h
        mov    edi, dword ptr [esp + 28h]
        mov    eax, dword ptr [edi + 0b4h]
        add    edi, 0b4h
        cmp    ecx, eax
        jbe    SHORT loc_1_110B7
        sub    ecx, eax
        push   ecx
        push   eax
        push   dword ptr [esp + 30h]
        call   sub_1_11470
        add    esp, 0ch
loc_1_110B7:
        mov    eax, dword ptr [edi]
        mov    dword ptr [esp + 0ch], eax
        mov    eax, dword ptr [esp + 28h]
        mov    eax, dword ptr [eax + 0bch]
        inc    eax
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [esp + 28h]
        cmp    dword ptr [eax + 0c8h], 0
        je     SHORT loc_1_1113B
        mov    eax, dword ptr [esp + 0ch]
        jmp    NEAR PTR loc_1_11179
loc_1_110E0:
        mov    dword ptr [esp + 14h], 1
        push   dword ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 0ch]
        push   dword ptr [esp + 2ch]
        call   dword ptr [eax + 1]
        mov    eax, dword ptr [esp + 10h]
        add    esp, 8
        cmp    byte ptr [eax], 10h
        je     SHORT loc_1_1111E
        mov    dword ptr [esp + 20h], 0
        mov    ecx, dword ptr [esp + 28h]
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [ecx + 0f4h], eax
        jmp    NEAR PTR loc_1_1122E
loc_1_1111E:
        inc    dword ptr [esp + 0ch]
loc_1_11122:
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_1_1112D
        inc    dword ptr [esp + 0ch]
loc_1_1112D:
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [eax + 0c8h], 0
loc_1_1113B:
        mov    eax, dword ptr [esp + 28h]
        add    eax, 0bch
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [esp + 0ch]
        inc    eax
        push   eax
        push   10h
        push   dword ptr [esp + 14h]
        call   sub_1_12730
        mov    dword ptr [esp + 18h], eax
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_1_1122E
        mov    eax, dword ptr [esp + 18h]
        mov    ecx, dword ptr [esp + 0ch]
        cmp    dword ptr [eax], ecx
        jbe    SHORT loc_1_111C7
        mov    eax, ecx
        inc    eax
loc_1_11179:
        mov    dword ptr [esp + 1ch], eax
        mov    dword ptr [esp + 14h], 0
        db  0C7h, 044h, 024h, 008h, 0B0h, 02Eh, 001h, 000h
        db  080h, 03Dh, 0B0h, 02Eh, 001h, 000h, 000h
        je     SHORT loc_1_11122
        mov    eax, dword ptr [esp + 1ch]
        mov    ecx, 9
        mov    al, byte ptr [eax]
        mov    byte ptr [esp + 10h], al
loc_1_111A5:
        mov    eax, dword ptr [esp + 8]
        mov    dl, byte ptr [esp + 10h]
        cmp    byte ptr [eax], dl
        je     NEAR PTR loc_1_110E0
        add    dword ptr [esp + 8], ecx
        mov    eax, dword ptr [esp + 8]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_111A5
        jmp    NEAR PTR loc_1_11122
loc_1_111C7:
        mov    edx, dword ptr [esp + 28h]
        mov    ecx, dword ptr [edx + 0bch]
        add    edx, 0bch
        cmp    dword ptr [esp + 0ch], ecx
        jne    SHORT loc_1_11223
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [esp + 20h], 1
        mov    eax, dword ptr [eax + 0b4h]
        cmp    dword ptr [esp + 0ch], eax
        jne    SHORT loc_1_11213
        mov    ecx, dword ptr [esp + 28h]
        mov    dword ptr [ecx + 0f4h], eax
        mov    eax, ecx
        mov    dword ptr [edx], 0
        mov    dword ptr [eax + 0c4h], 0
        jmp    SHORT loc_1_1122E
loc_1_11213:
        mov    eax, dword ptr [esp + 28h]
        dec    ecx
        dec    dword ptr [eax + 0c4h]
        mov    dword ptr [edx], ecx
        inc    ecx
        jmp    SHORT loc_1_11228
loc_1_11223:
        inc    ecx
        mov    eax, dword ptr [esp + 28h]
loc_1_11228:
        mov    dword ptr [eax + 0f4h], ecx
loc_1_1122E:
        mov    ecx, dword ptr [esp + 28h]
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [ecx + 0c8h], eax
        pop    edi
        pop    esi
        add    esp, 1ch
        ret
loc_1_11242:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 0c8h], 0
        jne    SHORT loc_1_1125E
        push   1
        push   dword ptr [esp + 0ch]
        push   eax
        call   sub_1_11470
        add    esp, 0ch
loc_1_1125E:
        ret
loc_1_1125F:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 0c8h], 1
        sbb    eax, eax
        and    eax, 1
        inc    eax
        push   eax
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_1_11470
        add    esp, 0ch
        mov    dword ptr [esi + 0f0h], 0
        push   1
        push   esi
        call   sub_1_11032
        add    esp, 8
        mov    dword ptr [esi + 80h], 7
        push   esi
        call   sub_1_10A83
        add    esp, 4
        mov    eax, dword ptr [esi + 1b2h]
        or     eax, eax
        je     SHORT loc_1_112BE
        push   dword ptr [esi + 5ch]
        push   4
        push   5
        push   esi
        call   eax
        add    esp, 10h
loc_1_112BE:
        pop    esi
        ret
loc_1_112C0:
        push   esi
        mov    eax, dword ptr [esp + 8]
        cmp    dword ptr [eax + 0c8h], 0
        jne    SHORT loc_1_112E0
        mov    esi, dword ptr [esp + 0ch]
        push   1
        push   esi
        push   eax
        call   sub_1_11470
        add    esp, 0ch
        jmp    SHORT loc_1_112E4
loc_1_112E0:
        mov    esi, dword ptr [esp + 0ch]
loc_1_112E4:
        and    byte ptr [esi], 11h
        pop    esi
        ret
loc_1_112E9:
        push   esi
        mov    eax, dword ptr [esp + 8]
        cmp    dword ptr [eax + 0c8h], 0
        jne    SHORT loc_1_11309
        mov    esi, dword ptr [esp + 0ch]
        push   1
        push   esi
        push   eax
        call   sub_1_11470
        add    esp, 0ch
        jmp    SHORT loc_1_1130D
loc_1_11309:
        mov    esi, dword ptr [esp + 0ch]
loc_1_1130D:
        and    byte ptr [esi], 13h
        pop    esi
        ret
loc_1_11312:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 0c8h], 1
        sbb    eax, eax
        and    eax, 1
        inc    eax
        push   eax
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_1_11470
        add    esp, 0ch
        mov    dword ptr [esi + 58h], 0
        push   3
        push   esi
        call   sub_1_11032
        add    esp, 8
        mov    eax, dword ptr [esi + 1b2h]
        or     eax, eax
        je     SHORT loc_1_1135B
        push   dword ptr [esi + 5ch]
        push   2
        push   5
        push   esi
        call   eax
        add    esp, 10h
loc_1_1135B:
        pop    esi
        ret
loc_1_1135D:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 0c8h], 1
        sbb    eax, eax
        and    eax, 1
        inc    eax
        push   eax
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_1_11470
        add    esp, 0ch
        mov    dword ptr [esi + 58h], 0
        push   3
        push   esi
        call   sub_1_11032
        add    esp, 8
        mov    eax, dword ptr [esi + 1b2h]
        or     eax, eax
        je     SHORT loc_1_113A6
        push   dword ptr [esi + 5ch]
        push   3
        push   5
        push   esi
        call   eax
        add    esp, 10h
loc_1_113A6:
        pop    esi
        ret
loc_1_113A8:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 0c8h], 1
        sbb    eax, eax
        and    eax, 1
        inc    eax
        push   eax
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_1_11470
        add    esp, 0ch
        mov    dword ptr [esi + 0f0h], 0
        push   1
        push   esi
        call   sub_1_11032
        add    esp, 8
        mov    dword ptr [esi + 80h], 7
        push   esi
        call   sub_1_10A83
        add    esp, 4
        mov    eax, dword ptr [esi + 1b2h]
        or     eax, eax
        je     SHORT loc_1_11407
        push   dword ptr [esi + 5ch]
        push   4
        push   5
        push   esi
        call   eax
        add    esp, 10h
loc_1_11407:
        pop    esi
        ret
loc_1_11409:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0c8h]
        or     eax, eax
        je     SHORT loc_1_1142F
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 1
        inc    eax
        push   eax
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_1_11470
        add    esp, 0ch
loc_1_1142F:
        mov    dword ptr [esi + 0f0h], 0
        push   1
        push   esi
        call   sub_1_11032
        add    esp, 8
        mov    dword ptr [esi + 80h], 8
        push   esi
        call   sub_1_10A83
        add    esp, 4
        mov    eax, dword ptr [esi + 1b2h]
        or     eax, eax
        je     SHORT loc_1_1146E
        push   dword ptr [esi + 5ch]
        push   5
        push   5
        push   esi
        call   eax
        add    esp, 10h
loc_1_1146E:
        pop    esi
        ret
sub_1_11470:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 18h]
        mov    edi, dword ptr [esp + 14h]
        mov    esi, dword ptr [esp + 10h]
        lea    ecx, [ebx + edi]
        mov    eax, dword ptr [esi + 0bch]
        cmp    eax, ecx
        jae    SHORT loc_1_114CD
        mov    eax, dword ptr [esi + 0b4h]
        cmp    edi, eax
        je     SHORT loc_1_114B1
        dec    edi
        mov    dword ptr [esi + 0bch], edi
        mov    ecx, edi
        sub    ecx, eax
        inc    ecx
        mov    dword ptr [esi + 0c4h], ecx
        inc    edi
        mov    dword ptr [esi + 0f4h], edi
        jmp    SHORT loc_1_11509
loc_1_114B1:
        mov    dword ptr [esi + 0f4h], eax
        mov    dword ptr [esi + 0bch], 0
        mov    dword ptr [esi + 0c4h], 0
        jmp    SHORT loc_1_11509
loc_1_114CD:
        sub    eax, ecx
        inc    eax
        push   eax
        push   ecx
        push   edi
        call   sub_1_12832
        add    esp, 0ch
        sub    dword ptr [esi + 0bch], ebx
        sub    dword ptr [esi + 0c4h], ebx
        mov    ecx, dword ptr [esi + 0f4h]
        cmp    edi, ecx
        ja     SHORT loc_1_11509
        sub    ecx, ebx
        mov    dword ptr [esi + 0f4h], ecx
        mov    eax, dword ptr [esi + 0b4h]
        cmp    ecx, eax
        ja     SHORT loc_1_11509
        mov    dword ptr [esi + 0f4h], eax
loc_1_11509:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h
loc_1_11510:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 0ch]
        inc    dword ptr [edx + 0ch]
        cmp    dword ptr [edx + 0ch], 1
        jg     SHORT loc_1_1154B
        mov    ebx, dword ptr [ebp + 10h]
        mov    dword ptr [edx + 4], ebx
        mov    ebx, dword ptr [ebp + 14h]
        mov    dword ptr [edx + 8], ebx
        mov    dword ptr [edx + 10h], 1
        mov    ebx, dword ptr [edx]
        db  0BEh, 056h, 00Dh, 001h, 000h
        mov    ecx, 0bh
        sub    eax, eax
        mov    edx, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
loc_1_1154B:
        mov    edx, dword ptr [ebp + 0ch]
        dec    dword ptr [edx + 0ch]
        pop    edi
        pop    esi
        leave
        ret
sub_1_11555:
        pushad
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    ecx, eax
loc_1_1155E:
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        sub    eax, ecx
        cmp    eax, 0c8h
        jb     SHORT loc_1_1155E
        popad
        ret
        db  000h
loc_1_11570:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 10h]
        cmp    eax, 1
        je     SHORT loc_1_11593
        cmp    eax, 9
        je     SHORT loc_1_115D4
        cmp    eax, 0ah
        je     SHORT loc_1_115EC
        jmp    NEAR PTR loc_1_11615
loc_1_11593:
        mov    dword ptr [ebp - 4], 0
        db  0C7h, 045h, 0F8h, 01Eh, 016h, 001h, 000h
        mov    dword ptr [ebp - 0ch], esi
        mov    dword ptr [ebp - 10h], 0bh
        mov    eax, dword ptr [esi]
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
        jmp    SHORT loc_1_11615
loc_1_115D4:
        mov    dword ptr [esi + 50h], 0
        mov    eax, dword ptr [esi + 1b2h]
        or     eax, eax
        je     SHORT loc_1_11615
        push   dword ptr [esi + 5ch]
        push   0
        jmp    SHORT loc_1_1160D
loc_1_115EC:
        push   1
        push   esi
        call   sub_1_11032
        add    esp, 8
        mov    dword ptr [esi + 50h], 1
        mov    eax, dword ptr [esi + 1b2h]
        or     eax, eax
        je     SHORT loc_1_11615
        push   dword ptr [esi + 5ch]
        push   1
loc_1_1160D:
        push   4
        push   esi
        call   eax
        add    esp, 10h
loc_1_11615:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_1161E:
        sub    esp, 4
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 0fah], 6
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 80h], 1
        push   dword ptr [esp]
        call   sub_1_124D7
        add    esp, 8
        ret
        db  000h, 000h
sub_1_11650:
        push   esi
        push   edi
        push   0
        push   0
        mov    esi, dword ptr [esp + 14h]
        push   0effffff5h
        push   dword ptr [esi + 14h]
        call   sub_1_12958
        add    esp, 10h
        cmp    eax, 1
        jne    SHORT loc_1_1169B
        mov    dword ptr [esi + 54h], 1
        push   esi
        call   sub_1_10A56
        add    esp, 4
        push   esi
        call   sub_1_10279
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_11697
        push   esi
        call   sub_1_11E18
        add    esp, 4
loc_1_11697:
        mov    eax, edi
        jmp    SHORT loc_1_116A0
loc_1_1169B:
        mov    eax, 2
loc_1_116A0:
        pop    edi
        pop    esi
        ret
sub_1_116A3:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 54h], 0
        jne    SHORT loc_1_116B0
        jmp    SHORT loc_1_11701
loc_1_116B0:
        push   esi
        call   sub_1_10388
        add    esp, 4
        push   esi
        call   sub_1_11E19
        add    esp, 4
        push   esi
        call   sub_1_10A35
        add    esp, 4
        push   0
        push   0
        push   10h
        push   dword ptr [esi + 14h]
        call   sub_1_12958
        add    esp, 10h
        push   0
        push   0
        push   0effffff6h
        push   dword ptr [esi + 14h]
        call   sub_1_12958
        add    esp, 10h
        cmp    eax, 1
        mov    eax, 2
        jne    SHORT loc_1_11703
        mov    dword ptr [esi + 54h], 0
loc_1_11701:
        sub    eax, eax
loc_1_11703:
        pop    esi
        ret
        db  081h, 0ECh, 090h, 000h, 000h, 000h, 056h, 057h, 0BEh, 068h, 035h, 001h, 000h, 08Dh, 07Ch, 024h
        db  00Ch, 066h, 0A5h, 0A4h, 08Bh, 03Dh, 08Ch, 02Dh, 001h, 000h, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh
        db  0C0h, 0F2h, 0AEh, 0F7h, 0D1h, 02Bh, 0F9h, 08Bh, 0F7h, 08Bh, 0D1h, 08Dh, 07Ch, 024h, 00Ch, 0B9h
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0C0h, 0F2h, 0AEh, 04Fh, 08Bh, 0CAh, 0C1h, 0E9h, 002h, 0F3h, 0A5h
        db  08Bh, 0CAh, 083h, 0E1h, 003h, 0F3h, 0A4h, 0BEh, 06Ch, 035h, 001h, 000h, 08Dh, 07Ch, 024h, 00Ch
        db  0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0C0h, 0F2h, 0AEh, 04Fh, 066h, 0A5h, 0BFh, 0E0h, 02Bh, 001h
        db  000h, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0C0h, 0F2h, 0AEh, 0F7h, 0D1h, 02Bh, 0F9h, 08Bh, 0F7h
        db  08Bh, 0D1h, 08Dh, 07Ch, 024h, 00Ch, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0C0h, 0F2h, 0AEh, 04Fh
        db  08Bh, 0CAh, 0C1h, 0E9h, 002h, 0F3h, 0A5h, 08Bh, 0CAh, 083h, 0E1h, 003h, 0F3h, 0A4h, 08Dh, 07Ch
        db  024h, 00Ch, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0C0h, 0F2h, 0AEh, 0F7h, 0D1h, 08Dh, 044h, 024h
        db  008h, 08Dh, 079h, 0FFh, 050h, 08Dh, 044h, 024h, 010h, 057h, 08Bh, 0B4h, 024h, 0A4h, 000h, 000h
        db  000h, 050h, 0FFh, 076h, 014h, 0E8h, 0B1h, 011h, 000h, 000h, 083h, 0C4h, 010h, 03Bh, 07Ch, 024h
        db  008h, 076h, 00Bh, 0C7h, 046h, 06Ch, 002h, 000h, 000h, 000h, 02Bh, 0C0h, 0EBh, 031h, 0C7h, 046h
        db  070h, 01Bh, 000h, 000h, 000h, 0C7h, 046h, 07Ch, 001h, 000h, 000h, 000h, 06Ah, 002h, 056h, 0E8h
        db  049h, 0F8h, 0FFh, 0FFh, 083h, 0C4h, 008h, 056h, 0E8h, 07Bh, 002h, 000h, 000h, 083h, 0C4h, 004h
        db  083h, 0F8h, 002h, 0B8h, 000h, 000h, 000h, 000h, 075h, 005h, 0B8h, 001h, 000h, 000h, 000h, 05Fh
        db  05Eh, 081h, 0C4h, 090h, 000h, 000h, 000h, 0C3h
sub_1_1180D:
        mov    eax, dword ptr [esp + 4]
        push   0
        push   eax
        push   0effffff3h
        push   dword ptr [eax + 14h]
        call   sub_1_12958
        add    esp, 10h
        dec    eax
        cmp    eax, 1
        cmc
        sbb    eax, eax
        and    eax, 2
        ret
sub_1_1182F:
        push   0
        push   0
        mov    eax, dword ptr [esp + 0ch]
        push   0effffff4h
        push   dword ptr [eax + 14h]
        call   sub_1_12958
        add    esp, 10h
        dec    eax
        cmp    eax, 1
        cmc
        sbb    eax, eax
        and    eax, 2
        ret
sub_1_11852:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_11650
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_11866
        jmp    SHORT loc_1_118AD
loc_1_11866:
        push   0
        push   4
        push   esi
        call   sub_1_1194B
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_11892
        mov    dword ptr [esi + 7ch], 3
        push   esi
        call   sub_1_11A6D
        add    esp, 4
        cmp    dword ptr [esi + 7ch], 4
        mov    eax, 1
        je     SHORT loc_1_118AF
loc_1_11892:
        push   esi
        call   sub_1_116A3
        add    esp, 4
        mov    dword ptr [esi + 7ch], 0
        push   1
        push   esi
        call   sub_1_11032
        add    esp, 8
loc_1_118AD:
        sub    eax, eax
loc_1_118AF:
        pop    esi
        ret
        db  056h, 06Ah, 000h, 08Bh, 074h, 024h, 00Ch, 06Ah, 006h, 056h, 0E8h, 08Bh, 000h, 000h, 000h, 083h
        db  0C4h, 00Ch, 00Bh, 0C0h, 074h, 009h, 056h, 0E8h, 0A0h, 001h, 000h, 000h, 083h, 0C4h, 004h, 02Bh
        db  0C0h, 05Eh, 0C3h
sub_1_118D4:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_11650
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_118E8
        jmp    SHORT loc_1_1192B
loc_1_118E8:
        push   0
        push   5
        push   esi
        call   sub_1_1194B
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_11922
        push   1
        push   esi
        call   sub_1_11032
        add    esp, 8
        mov    dword ptr [esi + 7ch], 0
        mov    dword ptr [esi + 58h], 1
        push   esi
        call   sub_1_10A83
        add    esp, 4
        mov    eax, 1
        jmp    SHORT loc_1_1192D
loc_1_11922:
        push   esi
        call   sub_1_116A3
        add    esp, 4
loc_1_1192B:
        sub    eax, eax
loc_1_1192D:
        pop    esi
        ret
        db  06Ah, 000h, 06Ah, 012h, 0FFh, 074h, 024h, 00Ch, 0E8h, 00Fh, 000h, 000h, 000h, 083h, 0C4h, 00Ch
        db  00Bh, 0C0h, 0B8h, 001h, 000h, 000h, 000h, 075h, 002h, 02Bh, 0C0h, 0C3h
sub_1_1194B:
        sub    esp, 90h
        push   ebx
        push   esi
        push   edi
        db  0BEh, 070h, 035h, 001h, 000h
        lea    edi, [esp + 10h]
        movsw
        movsb
        mov    ebx, dword ptr [esp + 0a4h]
        db  08Bh, 03Ch, 09Dh, 020h, 02Dh, 001h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        lea    edi, [esp + 10h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        cmp    dword ptr [esp + 0a8h], 0
        je     SHORT loc_1_119D9
        mov    edi, dword ptr [esp + 0a8h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        lea    edi, [esp + 10h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
loc_1_119D9:
        db  0BFh, 0E0h, 02Bh, 001h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        lea    edi, [esp + 10h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        lea    edi, [esp + 10h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [esp + 0ch]
        lea    esi, [ecx - 1]
        push   eax
        lea    eax, [esp + 14h]
        push   esi
        mov    edi, dword ptr [esp + 0a8h]
        push   eax
        push   edi
        call   sub_1_11B90
        add    esp, 10h
        cmp    esi, dword ptr [esp + 0ch]
        jbe    SHORT loc_1_11A49
        mov    dword ptr [edi + 6ch], 2
        sub    eax, eax
        jmp    SHORT loc_1_11A63
loc_1_11A49:
        mov    dword ptr [edi + 70h], ebx
        mov    dword ptr [edi + 7ch], 1
        push   2
        push   edi
        call   sub_1_11032
        add    esp, 8
        mov    eax, 1
loc_1_11A63:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 90h
        ret
sub_1_11A6D:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_10A83
        add    esp, 4
        push   0
        push   dword ptr [esi + 1aah]
        lea    eax, [esi + 192h]
        push   eax
        call   sub_1_11CB8
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_11AA4
        mov    dword ptr [esi + 6ch], 2
        mov    dword ptr [esi + 7ch], 6
loc_1_11AA4:
        mov    eax, dword ptr [esi + 7ch]
        pop    esi
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_11AB0:
        sub    esp, 0ch
        push   esi
        push   edi
        mov    dword ptr [esp + 0ch], 0
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 24h]
        mov    esi, dword ptr [esp + 20h]
        push   dword ptr [esp + 24h]
        push   esi
        call   sub_1_11AF0
        add    esp, 10h
        mov    edi, eax
        lea    eax, [esp + 8]
        push   eax
        push   dword ptr [esi + 14h]
        call   sub_1_12948
        add    esp, 8
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 0ch
        ret
sub_1_11AF0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 24h]
        mov    ebx, 0ah
        mov    dword ptr [eax], 0
        mov    esi, dword ptr [esp + 18h]
        mov    edi, dword ptr [esp + 1ch]
        mov    ebp, dword ptr [esp + 20h]
loc_1_11B12:
        mov    eax, ebx
        dec    ebx
        or     eax, eax
        je     SHORT loc_1_11B86
        lea    eax, [esp + 10h]
        push   eax
        push   ebp
        push   edi
        push   dword ptr [esi + 14h]
        call   sub_1_12978
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_11B75
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_1_11B50
        push   esi
        call   sub_1_10A0C
        add    esp, 4
        mov    eax, dword ptr [esi + 1b6h]
        or     eax, eax
        je     SHORT loc_1_11B61
        cmp    eax, -1
        je     SHORT loc_1_11B86
        jmp    SHORT loc_1_11B12
loc_1_11B50:
        mov    eax, dword ptr [esp + 24h]
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax], ecx
        mov    eax, 1
        jmp    SHORT loc_1_11B88
loc_1_11B61:
        push   1
        push   dword ptr [esi + 14h]
        call   sub_1_12960
        add    esp, 8
        mov    eax, 1
        jmp    SHORT loc_1_11B88
loc_1_11B75:
        push   dword ptr [esi + 14h]
        call   sub_1_12988
        add    esp, 4
        mov    dword ptr [esi + 1b6h], eax
loc_1_11B86:
        sub    eax, eax
loc_1_11B88:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_11B90:
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        call   sub_1_11BA9
        add    esp, 10h
        ret
sub_1_11BA9:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 30h]
        lea    eax, [esp + 18h]
        mov    edi, dword ptr [esp + 34h]
        mov    dword ptr [esp + 14h], 0ah
        mov    dword ptr [edi], 0
        mov    esi, dword ptr [esp + 28h]
        push   eax
        push   dword ptr [esi + 14h]
        call   sub_1_12948
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_11C40
        test   byte ptr [esp + 18h], 3
        jne    SHORT loc_1_11C40
        mov    ebp, dword ptr [esp + 2ch]
loc_1_11BE9:
        lea    eax, [esp + 10h]
        push   eax
        push   ebx
        push   ebp
        push   dword ptr [esi + 14h]
        call   sub_1_12970
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_11C40
        mov    eax, dword ptr [edi]
        add    eax, dword ptr [esp + 10h]
        mov    dword ptr [edi], eax
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_1_11C1D
        push   esi
        call   sub_1_10A0C
        dec    dword ptr [esp + 18h]
        add    esp, 4
        jmp    SHORT loc_1_11C39
loc_1_11C1D:
        cmp    dword ptr [esp + 30h], eax
        jbe    SHORT loc_1_11C4A
        sub    ebx, dword ptr [esp + 10h]
        push   ebx
        push   esi
        call   sub_1_1074A
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_11C40
        add    ebp, dword ptr [esp + 10h]
loc_1_11C39:
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_1_11BE9
loc_1_11C40:
        sub    eax, eax
loc_1_11C42:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
loc_1_11C4A:
        mov    eax, 1
        jmp    SHORT loc_1_11C42
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   0
        call   sub_1_129B0
        mov    esi, dword ptr [ebp + 8]
        or     eax, eax
        mov    dword ptr [esi], eax
        je     SHORT loc_1_11CB1
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 8], eax
        mov    ecx, dword ptr [ebp + 10h]
        or     ecx, ecx
        jne    SHORT loc_1_11C89
        db  0B9h, 098h, 01Dh, 001h, 000h
loc_1_11C89:
        push   esi
        push   eax
        push   ecx
        call   sub_1_129CC
        mov    dword ptr [esi + 4], eax
        or     eax, eax
        je     SHORT loc_1_11C9F
        mov    eax, 1
        jmp    SHORT loc_1_11CB3
loc_1_11C9F:
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        sub    eax, eax
        jmp    SHORT loc_1_11CB3
loc_1_11CB1:
        sub    eax, eax
loc_1_11CB3:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_11CB8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        or     edi, edi
        je     NEAR PTR loc_1_11D6E
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi], 0
        jne    NEAR PTR loc_1_11D6E
        cmp    dword ptr [esi + 4], 0
        jne    NEAR PTR loc_1_11D6E
        mov    dword ptr [esi + 8], edi
        push   0
        call   sub_1_129B0
        mov    dword ptr [esi], eax
        or     eax, eax
        je     SHORT loc_1_11D1A
        mov    eax, dword ptr [ebp + 10h]
        or     eax, eax
        jne    SHORT loc_1_11CFD
        db  0B8h, 098h, 01Dh, 001h, 000h
loc_1_11CFD:
        push   esi
        push   edi
        push   eax
        call   sub_1_129CC
        mov    dword ptr [esi + 4], eax
        or     eax, eax
        jne    SHORT loc_1_11D1A
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
loc_1_11D1A:
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     SHORT loc_1_11D6E
        cmp    dword ptr [esi + 4], 0
        je     SHORT loc_1_11D6E
        push   4
        push   eax
        call   sub_1_129C0
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_1_11D5A
        push   eax
        call   sub_1_129E8
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        mov    dword ptr [esi + 4], 0
        mov    dword ptr [esi], 0
        sub    eax, eax
        jmp    SHORT loc_1_11D73
loc_1_11D5A:
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        mov    dword ptr [esi], 0
loc_1_11D6E:
        mov    eax, 1
loc_1_11D73:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_11D79:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_1_11D94
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 028h, 000h, 001h, 000h
loc_1_11D94:
        mov    esp, ebp
        pop    ebp
        ret
loc_1_11D98:
        sub    esp, 4
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        add    eax, 4
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_11DBD
        mov    dword ptr [eax], 0
        push   dword ptr [esp]
        call   sub_1_11D79
        add    esp, 4
loc_1_11DBD:
        add    esp, 4
        ret
        db  083h, 0ECh, 004h, 089h, 054h, 024h, 000h, 08Bh, 044h, 024h, 000h, 0C7h, 080h, 0D4h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0FFh, 074h, 024h, 000h, 0E8h, 0C4h, 0F8h, 0FFh, 0FFh, 083h, 0C4h
        db  004h, 08Bh, 044h, 024h, 000h, 0C7h, 080h, 0F0h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 08Bh
        db  044h, 024h, 000h, 0C7h, 040h, 07Ch, 000h, 000h, 000h, 000h, 08Bh, 044h, 024h, 000h, 0C7h, 080h
        db  080h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 06Ah, 001h, 0FFh, 074h, 024h, 004h, 0E8h, 01Eh
        db  0F2h, 0FFh, 0FFh, 083h, 0C4h, 00Ch, 0C3h
sub_1_11E18:
        ret
sub_1_11E19:
        ret
sub_1_11E1A:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 4], 1
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [ebp - 4], 0
        db  0C7h, 045h, 0F8h, 056h, 00Dh, 001h, 000h
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 10h], 0bh
        mov    eax, dword ptr [eax]
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
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_11E7C:
        push   esi
        push   edi
        sub    edi, edi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        push   edi
        push   3
        push   dword ptr [esi + 14h]
        call   sub_1_12958
        add    esp, 10h
        cmp    eax, edi
        jne    SHORT loc_1_11E9E
        mov    edi, 4
        jmp    SHORT loc_1_11EBF
loc_1_11E9E:
        push   0
        push   0
        push   5
        push   dword ptr [esi + 14h]
        call   sub_1_12958
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_11EBA
        mov    edi, 4
        jmp    SHORT loc_1_11EBF
loc_1_11EBA:
        call   sub_1_11555
loc_1_11EBF:
        mov    eax, edi
        pop    edi
        pop    esi
        ret
        db  083h, 0ECh, 01Ch, 053h, 056h, 057h, 055h, 0C7h, 044h, 024h, 010h, 000h, 000h, 000h, 000h, 02Bh
        db  0F6h, 08Bh, 06Ch, 024h, 030h, 02Bh, 0FFh, 08Bh, 09Dh, 0E4h, 000h, 000h, 000h, 08Bh, 044h, 024h
        db  034h, 03Bh, 0D8h, 072h, 002h, 08Bh, 0D8h, 06Bh, 0DBh, 00Ah, 0C7h, 044h, 024h, 018h, 000h, 000h
        db  000h, 000h, 083h, 0FBh, 028h, 076h, 005h, 0BBh, 064h, 000h, 000h, 000h, 06Ah, 000h, 08Dh, 044h
        db  024h, 024h, 053h, 050h, 0E8h, 0ABh, 0FDh, 0FFh, 0FFh, 08Dh, 044h, 024h, 020h, 083h, 0C4h, 00Ch
        db  050h, 0FFh, 075h, 014h, 0E8h, 02Bh, 00Ah, 000h, 000h, 083h, 0C4h, 008h, 00Bh, 0C0h, 074h, 02Eh
        db  08Bh, 044h, 024h, 034h, 039h, 044h, 024h, 018h, 073h, 01Ch, 08Bh, 044h, 024h, 018h, 039h, 085h
        db  0E4h, 000h, 000h, 000h, 076h, 010h, 03Bh, 0C7h, 075h, 006h, 083h, 0FEh, 00Ah, 077h, 00Fh, 046h
        db  08Bh, 07Ch, 024h, 018h, 0EBh, 0ACh, 0C7h, 044h, 024h, 010h, 001h, 000h, 000h, 000h, 08Bh, 044h
        db  024h, 010h, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 01Ch, 0C3h, 000h, 000h
sub_1_11F60:
        mov    ecx, dword ptr [esp + 4]
        lea    edx, [ecx + 0f4h]
        mov    eax, dword ptr [ecx + 0b4h]
        mov    dword ptr [edx], eax
        mov    byte ptr [edx + 4], 0
        mov    byte ptr [edx + 5], 0
        lea    eax, [edx + 0eh]
        mov    dword ptr [edx + 6], 0
        mov    dword ptr [edx + 0ah], 0
        mov    dword ptr [edx + 9ah], eax
        mov    dword ptr [edx + 0aah], 0
        mov    dword ptr [edx + 0aeh], 0
        mov    dword ptr [ecx + 1aeh], 0
        mov    dword ptr [edx + 0b2h], 1eh
        mov    dword ptr [edx + 0b6h], 1388h
        ret
        db  08Bh, 04Ch, 024h, 004h, 08Dh, 091h, 0F4h, 000h, 000h, 000h, 08Bh, 081h, 0B4h, 000h, 000h, 000h
        db  089h, 002h, 08Dh, 042h, 00Eh, 089h, 082h, 09Ah, 000h, 000h, 000h, 0C7h, 081h, 0AEh, 001h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0C3h, 083h, 0ECh, 008h, 080h, 07Ch, 024h, 00Ch, 020h, 07Ch, 015h
        db  080h, 07Ch, 024h, 00Ch, 07Eh, 07Fh, 00Eh, 08Ah, 044h, 024h, 00Ch, 088h, 044h, 024h, 000h, 08Dh
        db  044h, 024h, 001h, 0EBh, 05Fh, 00Fh, 0BEh, 044h, 024h, 00Ch, 083h, 0F8h, 00Ah, 074h, 031h, 083h
        db  0F8h, 00Dh, 0C6h, 044h, 024h, 000h, 03Ch, 074h, 038h, 08Ah, 044h, 024h, 00Ch, 0C0h, 0E8h, 004h
        db  083h, 0E0h, 00Fh, 08Ah, 080h, 074h, 035h, 001h, 000h, 088h, 044h, 024h, 001h, 08Ah, 044h, 024h
        db  00Ch, 083h, 0E0h, 00Fh, 08Ah, 080h, 074h, 035h, 001h, 000h, 088h, 044h, 024h, 002h, 0EBh, 01Bh
        db  0C6h, 044h, 024h, 000h, 03Ch, 0C6h, 044h, 024h, 001h, 06Ch, 0C6h, 044h, 024h, 002h, 066h, 0EBh
        db  00Ah, 0C6h, 044h, 024h, 001h, 063h, 0C6h, 044h, 024h, 002h, 072h, 0C6h, 044h, 024h, 003h, 03Eh
        db  08Dh, 044h, 024h, 004h, 0C6h, 000h, 000h, 083h, 0C4h, 008h, 0C3h
sub_1_1206F:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        lea    ebx, [esi + 0f9h]
        mov    dword ptr [esi + 0fah], 0
        cmp    dword ptr [esi + 1aeh], 0
        je     SHORT loc_1_120A2
        mov    eax, dword ptr [esi + 0f4h]
        cmp    byte ptr [eax], 0ah
        jne    SHORT loc_1_120A2
        inc    eax
        mov    dword ptr [esi + 0f4h], eax
loc_1_120A2:
        mov    dword ptr [esi + 1aeh], 0
        cmp    dword ptr [esi + 0feh], 1
        mov    edi, 1
        je     SHORT loc_1_120D3
        lea    edx, [esi + 102h]
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 23h
        rep stosd
        sub    edi, edi
        mov    dword ptr [esi + 18eh], edx
loc_1_120D3:
        mov    dword ptr [esi + 0feh], 0
        mov    ebp, 2
loc_1_120E2:
        push   esi
        call   sub_1_122AE
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_1_12219
        mov    al, byte ptr [ebx]
        mov    ecx, dword ptr [esi + 0f4h]
        mov    byte ptr [esi + 0f8h], al
        mov    al, byte ptr [ecx]
        inc    ecx
        mov    byte ptr [ebx], al
        mov    dword ptr [esi + 0f4h], ecx
        push   8
        movsx  eax, al
        push   eax
        call   sub_1_1275C
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_1_121EA
        cmp    byte ptr [esi + 0f8h], 0dh
        jne    SHORT loc_1_12153
        cmp    byte ptr [ebx], 0ah
        jne    SHORT loc_1_12153
        cmp    dword ptr [esi + 78h], ebp
        je     SHORT loc_1_120E2
        mov    dword ptr [esi + 0fah], 12h
        mov    dword ptr [esi + 0feh], 3
        mov    eax, 1
        jmp    NEAR PTR loc_1_12290
loc_1_12153:
        or     edi, edi
        je     SHORT loc_1_120E2
        mov    al, byte ptr [esi + 102h]
        push   eax
        call   sub_1_122FA
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_121AB
        lea    eax, [esi + 102h]
        push   eax
        call   sub_1_12325
        add    esp, 4
        mov    dword ptr [esi + 0fah], eax
        or     eax, eax
        jne    NEAR PTR loc_1_12229
        mov    dword ptr [esi + 0feh], ebp
        mov    al, byte ptr [ebx]
        cmp    al, 20h
        jne    NEAR PTR loc_1_12273
        mov    ecx, dword ptr [esi + 18eh]
        mov    byte ptr [ecx], al
        inc    ecx
        mov    dword ptr [esi + 18eh], ecx
        jmp    NEAR PTR loc_1_120E2
loc_1_121AB:
        push   80h
        movsx  eax, byte ptr [esi + 102h]
        push   eax
        call   sub_1_1275C
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_1_1227A
        mov    al, byte ptr [ebx]
        cmp    al, 0dh
        je     SHORT loc_1_12243
        cmp    al, 20h
        jne    NEAR PTR loc_1_12273
        mov    ecx, dword ptr [esi + 18eh]
        mov    byte ptr [ecx], al
        inc    ecx
        mov    dword ptr [esi + 18eh], ecx
        jmp    NEAR PTR loc_1_120E2
loc_1_121EA:
        mov    al, byte ptr [ebx]
        push   eax
        call   sub_1_122CF
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_120E2
        mov    edi, 1
        mov    ecx, dword ptr [esi + 18eh]
        mov    al, byte ptr [ebx]
        mov    byte ptr [ecx], al
        add    ecx, edi
        mov    dword ptr [esi + 18eh], ecx
        jmp    NEAR PTR loc_1_120E2
loc_1_12219:
        or     edi, edi
        je     SHORT loc_1_1228E
        mov    dword ptr [esi + 0feh], 1
        jmp    SHORT loc_1_1228E
loc_1_12229:
        mov    dword ptr [esi + 0feh], 3
        mov    eax, dword ptr [esi + 0f4h]
        cmp    dword ptr [esi + 0bch], eax
        jae    SHORT loc_1_12267
        jmp    SHORT loc_1_1225B
loc_1_12243:
        mov    dword ptr [esi + 0feh], 4
        mov    eax, dword ptr [esi + 0f4h]
        cmp    dword ptr [esi + 0bch], eax
        jae    SHORT loc_1_12267
loc_1_1225B:
        mov    dword ptr [esi + 1aeh], 1
        jmp    SHORT loc_1_12273
loc_1_12267:
        cmp    byte ptr [eax], 0ah
        jne    SHORT loc_1_12273
        inc    eax
        mov    dword ptr [esi + 0f4h], eax
loc_1_12273:
        mov    eax, 1
        jmp    SHORT loc_1_12290
loc_1_1227A:
        mov    dword ptr [esi + 0fah], 0
        mov    dword ptr [esi + 0feh], 0
loc_1_1228E:
        sub    eax, eax
loc_1_12290:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 044h, 024h, 004h, 08Bh, 088h, 0C0h, 000h, 000h, 000h, 039h, 088h, 0F4h, 000h, 000h, 000h
        db  0B8h, 000h, 000h, 000h, 000h, 00Fh, 097h, 0C0h, 0C3h
sub_1_122AE:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0bch]
        or     ecx, ecx
        je     SHORT loc_1_122C9
        cmp    dword ptr [eax + 0f4h], ecx
        mov    eax, 0
        jbe    SHORT loc_1_122CE
loc_1_122C9:
        mov    eax, 1
loc_1_122CE:
        ret
sub_1_122CF:
        push   ebx
        mov    bl, byte ptr [esp + 8]
        push   107h
        movsx  eax, bl
        push   eax
        call   sub_1_1275C
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_122F3
        cmp    bl, 2dh
        mov    eax, 0
        jne    SHORT loc_1_122F8
loc_1_122F3:
        mov    eax, 1
loc_1_122F8:
        pop    ebx
        ret
sub_1_122FA:
        push   ebx
        mov    bl, byte ptr [esp + 8]
        push   103h
        movsx  eax, bl
        push   eax
        call   sub_1_1275C
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_1231E
        cmp    bl, 2dh
        mov    eax, 0
        jne    SHORT loc_1_12323
loc_1_1231E:
        mov    eax, 1
loc_1_12323:
        pop    ebx
        ret
sub_1_12325:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 14h]
        push   103h
        movsx  eax, byte ptr [eax]
        push   eax
        call   sub_1_1275C
        add    esp, 8
        cmp    eax, 1
        sbb    edx, edx
        and    edx, 0fffffff8h
        add    edx, 9
        cmp    edx, 12h
        jg     SHORT loc_1_1238C
        db  08Dh, 02Ch, 095h, 060h, 02Eh, 001h, 000h
loc_1_12355:
        mov    ebx, dword ptr [ebp]
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    esi, dword ptr [esp + 14h]
        lea    eax, [ecx - 1]
        mov    ecx, eax
        shr    ecx, 2
        mov    edi, ebx
        repe cmpsd
        jne    SHORT loc_1_1237E
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb
loc_1_1237E:
        je     SHORT loc_1_12393
        add    ebp, 4
        inc    edx
        db  081h, 0FDh, 0A8h, 02Eh, 001h, 000h
        jbe    SHORT loc_1_12355
loc_1_1238C:
        sub    eax, eax
loc_1_1238E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_12393:
        mov    eax, edx
        jmp    SHORT loc_1_1238E
        db  0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_123A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 7ch]
        cmp    eax, 1
        je     SHORT loc_1_123C5
        cmp    eax, 3
        je     SHORT loc_1_12430
        cmp    eax, 7
        je     NEAR PTR loc_1_12493
        jmp    NEAR PTR loc_1_124D2
loc_1_123C5:
        mov    eax, dword ptr [esi + 0fah]
        cmp    eax, 10h
        jne    SHORT loc_1_12410
        mov    dword ptr [esi + 7ch], 2
        push   1
        push   esi
        call   sub_1_11032
        add    esp, 8
        mov    eax, dword ptr [esi + 8ch]
        or     eax, eax
        je     NEAR PTR loc_1_124C3
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        db  0CDh, 020h, 028h, 000h, 001h, 000h
        add    esi, 192h
        push   esi
        call   sub_1_11D79
        add    esp, 4
        jmp    NEAR PTR loc_1_124D2
loc_1_12410:
        cmp    eax, 0ch
        jne    NEAR PTR loc_1_124D2
        mov    dword ptr [esi + 7ch], 5
        push   1
        push   esi
        call   sub_1_11032
        add    esp, 8
        jmp    NEAR PTR loc_1_124C3
loc_1_12430:
        mov    eax, dword ptr [esi + 0fah]
        cmp    eax, 0ah
        jne    SHORT loc_1_12485
        mov    dword ptr [esi + 7ch], 4
        mov    dword ptr [esi + 0f0h], 1
        mov    dword ptr [esi + 58h], 0
        push   3
        push   esi
        call   sub_1_11032
        add    esp, 8
        mov    eax, dword ptr [esi + 8ch]
        or     eax, eax
        je     SHORT loc_1_124C3
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 028h, 000h, 001h, 000h
        add    esi, 192h
        push   esi
        call   sub_1_11D79
        add    esp, 4
        jmp    SHORT loc_1_124D2
loc_1_12485:
        cmp    eax, 0ch
        jne    SHORT loc_1_124D2
        mov    dword ptr [esi + 7ch], 5
        jmp    SHORT loc_1_124C3
loc_1_12493:
        mov    eax, dword ptr [esi + 0fah]
        cmp    eax, 4
        je     SHORT loc_1_124AA
        cmp    eax, 0ch
        je     SHORT loc_1_124B3
        cmp    eax, 10h
        je     SHORT loc_1_124BC
        jmp    SHORT loc_1_124D2
loc_1_124AA:
        mov    dword ptr [esi + 7ch], 8
        jmp    SHORT loc_1_124D2
loc_1_124B3:
        mov    dword ptr [esi + 7ch], 5
        jmp    SHORT loc_1_124C3
loc_1_124BC:
        mov    dword ptr [esi + 7ch], 2
loc_1_124C3:
        add    esi, 192h
        push   esi
        call   sub_1_11D79
        add    esp, 4
loc_1_124D2:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_124D7:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 80h]
        dec    eax
        cmp    eax, 8
        ja     NEAR PTR loc_1_12709
        db  0FFh, 024h, 085h, 00Bh, 027h, 001h, 000h
loc_1_124F3:
        mov    eax, dword ptr [esi + 0fah]
        cmp    eax, 6
        je     SHORT loc_1_12508
        cmp    eax, 0ch
        je     SHORT loc_1_1252D
        jmp    NEAR PTR loc_1_12709
loc_1_12508:
        push   esi
        call   sub_1_118D4
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_1251F
        mov    dword ptr [esi + 80h], 3
loc_1_1251F:
        push   esi
        call   sub_1_10BC5
        add    esp, 4
        jmp    NEAR PTR loc_1_12709
loc_1_1252D:
        mov    dword ptr [esi + 0f0h], 0
        push   1
        push   esi
        call   sub_1_11032
        add    esp, 8
        push   esi
        call   sub_1_116A3
        add    esp, 4
        jmp    NEAR PTR loc_1_12709
loc_1_12550:
        mov    eax, dword ptr [esi + 0fah]
        or     eax, eax
        je     NEAR PTR loc_1_1260C
        cmp    eax, 0ah
        je     NEAR PTR loc_1_1261A
        cmp    eax, 12h
        je     NEAR PTR loc_1_1260C
        cmp    dword ptr [esi + 0c4h], 7
        jne    SHORT loc_1_125DF
        mov    eax, dword ptr [esi + 0b4h]
        cmp    byte ptr [eax], 41h
        jne    SHORT loc_1_125DF
        cmp    byte ptr [eax + 1], 54h
        jne    SHORT loc_1_125DF
        cmp    byte ptr [eax + 2], 2dh
        jne    SHORT loc_1_125DF
        cmp    byte ptr [eax + 3], 53h
        jne    SHORT loc_1_125DF
        cmp    byte ptr [eax + 4], 41h
        jne    SHORT loc_1_125DF
        cmp    byte ptr [eax + 5], 43h
        jne    SHORT loc_1_125DF
        mov    dword ptr [esi + 0c4h], 0
        push   esi
        call   sub_1_11E19
        add    esp, 4
        push   esi
        call   sub_1_10A35
        add    esp, 4
        push   esi
        call   sub_1_10A83
        add    esp, 4
        push   esi
        call   sub_1_10BA4
        add    esp, 4
        mov    dword ptr [esi + 80h], 3
        jmp    NEAR PTR loc_1_12709
loc_1_125DF:
        mov    dword ptr [esi + 0f0h], 0
        mov    dword ptr [esi + 80h], 1
        push   1
        push   esi
        call   sub_1_11032
        add    esp, 8
        push   esi
        call   sub_1_116A3
        add    esp, 4
        jmp    NEAR PTR loc_1_12709
loc_1_1260C:
        push   esi
        call   sub_1_11E19
        add    esp, 4
        jmp    NEAR PTR loc_1_12709
loc_1_1261A:
        push   esi
        call   sub_1_10BA4
        add    esp, 4
        mov    dword ptr [esi + 0f0h], 1
        push   esi
        call   sub_1_11E19
        add    esp, 4
        mov    dword ptr [esi + 80h], 4
        mov    dword ptr [esi + 58h], 1
        push   3
        push   esi
        call   sub_1_11032
        add    esp, 8
        db  083h, 03Dh, 05Ch, 02Fh, 001h, 000h, 000h
        jne    SHORT loc_1_12664
        push   esi
        call   sub_1_10A35
        add    esp, 4
loc_1_12664:
        mov    eax, dword ptr [esi + 1b2h]
        or     eax, eax
        je     NEAR PTR loc_1_12709
        push   dword ptr [esi + 5ch]
        push   1
        push   6
        jmp    NEAR PTR loc_1_12703
loc_1_1267E:
        cmp    dword ptr [esi + 0feh], 4
        jne    SHORT loc_1_1269E
        lea    eax, [esi + 102h]
        push   eax
        call   sub_1_12876
        add    esp, 4
        mov    dword ptr [esi + 84h], eax
        jmp    SHORT loc_1_12709
loc_1_1269E:
        mov    dword ptr [esi + 80h], 1
        push   esi
        call   sub_1_116A3
        add    esp, 4
        jmp    SHORT loc_1_12709
loc_1_126B3:
        cmp    dword ptr [esi + 0feh], 4
        je     SHORT loc_1_12709
        mov    dword ptr [esi + 80h], 1
        jmp    SHORT loc_1_12709
loc_1_126C8:
        mov    eax, dword ptr [esi + 0fah]
        cmp    eax, 10h
        je     SHORT loc_1_126D8
        cmp    eax, 0ch
        jne    SHORT loc_1_12709
loc_1_126D8:
        push   esi
        call   sub_1_116A3
        add    esp, 4
        mov    dword ptr [esi + 80h], 1
        mov    dword ptr [esi + 7ch], 9
        mov    eax, dword ptr [esi + 1b2h]
        or     eax, eax
        je     SHORT loc_1_12709
        push   dword ptr [esi + 5ch]
        push   1
        push   3
loc_1_12703:
        push   esi
        call   eax
        add    esp, 10h
loc_1_12709:
        pop    esi
        ret
        db  0F3h, 024h, 001h, 000h, 009h, 027h, 001h, 000h, 050h, 025h, 001h, 000h, 009h, 027h, 001h, 000h
        db  07Eh, 026h, 001h, 000h, 0B3h, 026h, 001h, 000h, 0C8h, 026h, 001h, 000h, 0C8h, 026h, 001h, 000h
        db  0C8h, 026h, 001h, 000h, 000h
sub_1_12730:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 14h]
        or     edx, edx
        jne    SHORT loc_1_12740
        jmp    SHORT loc_1_12757
loc_1_12740:
        sub    esi, esi
        cmp    edx, esi
        je     SHORT loc_1_12757
        mov    ecx, dword ptr [esp + 10h]
loc_1_1274A:
        movsx  edi, byte ptr [eax]
        cmp    edi, ecx
        je     SHORT loc_1_12759
        inc    esi
        inc    eax
        cmp    esi, edx
        jb     SHORT loc_1_1274A
loc_1_12757:
        sub    eax, eax
loc_1_12759:
        pop    edi
        pop    esi
        ret
sub_1_1275C:
        mov    eax, dword ptr [esp + 8]
        cmp    eax, 4
        je     SHORT loc_1_12782
        cmp    eax, 8
        je     SHORT loc_1_12797
        cmp    eax, 80h
        je     SHORT loc_1_127B6
        cmp    eax, 103h
        je     SHORT loc_1_127DF
        cmp    eax, 107h
        je     SHORT loc_1_127FE
loc_1_1277F:
        sub    eax, eax
loc_1_12781:
        ret
loc_1_12782:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 30h
        jl     SHORT loc_1_1277F
        cmp    eax, 39h
        jg     SHORT loc_1_1277F
        mov    eax, 1
        jmp    SHORT loc_1_12781
loc_1_12797:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 20h
        je     SHORT loc_1_127AF
        cmp    eax, 0ah
        je     SHORT loc_1_127AF
        cmp    eax, 0dh
        je     SHORT loc_1_127AF
        cmp    eax, 9
        jne    SHORT loc_1_1277F
loc_1_127AF:
        mov    eax, 1
        jmp    SHORT loc_1_12781
loc_1_127B6:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 30h
        jl     SHORT loc_1_127C4
        cmp    eax, 39h
        jle    SHORT loc_1_127D8
loc_1_127C4:
        cmp    eax, 61h
        jl     SHORT loc_1_127CE
        cmp    eax, 66h
        jle    SHORT loc_1_127D8
loc_1_127CE:
        cmp    eax, 41h
        jl     SHORT loc_1_1277F
        cmp    eax, 46h
        jg     SHORT loc_1_1277F
loc_1_127D8:
        mov    eax, 1
        jmp    SHORT loc_1_12781
loc_1_127DF:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 61h
        jl     SHORT loc_1_127ED
        cmp    eax, 7ah
        jle    SHORT loc_1_127F7
loc_1_127ED:
        cmp    eax, 41h
        jl     SHORT loc_1_1277F
        cmp    eax, 5ah
        jg     SHORT loc_1_1277F
loc_1_127F7:
        mov    eax, 1
        jmp    SHORT loc_1_12781
loc_1_127FE:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 30h
        jl     SHORT loc_1_1280C
        cmp    eax, 39h
        jle    SHORT loc_1_12828
loc_1_1280C:
        cmp    eax, 61h
        jl     SHORT loc_1_12816
        cmp    eax, 7ah
        jle    SHORT loc_1_12828
loc_1_12816:
        cmp    eax, 41h
        jl     NEAR PTR loc_1_1277F
        cmp    eax, 5ah
        jg     NEAR PTR loc_1_1277F
loc_1_12828:
        mov    eax, 1
        jmp    NEAR PTR loc_1_12781
sub_1_12832:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        mov    eax, dword ptr [esp + 10h]
        cmp    esi, eax
        mov    edx, dword ptr [esp + 18h]
        jae    SHORT loc_1_1285E
        lea    edi, [eax + edx - 1]
        lea    ecx, [esi + edx - 1]
        sub    esi, esi
        jmp    SHORT loc_1_12872
        db  08Ah, 019h, 088h, 01Fh, 049h, 04Fh, 046h, 03Bh, 0F2h, 077h, 0F5h, 0EBh, 014h
loc_1_1285E:
        or     edx, edx
        je     SHORT loc_1_12872
        mov    edi, eax
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
loc_1_12872:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_12876:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        sub    edi, edi
        cmp    byte ptr [esi], 0
        je     NEAR PTR loc_1_1290B
        db  08Bh, 00Dh, 060h, 02Fh, 001h, 000h
        db  08Bh, 015h, 0F4h, 037h, 001h, 000h
loc_1_12893:
        cmp    ecx, 1
        jle    SHORT loc_1_128B4
        push   4
        movsx  eax, byte ptr [esi]
        push   eax
        call   sub_1_1275C
        add    esp, 8
        db  08Bh, 00Dh, 060h, 02Fh, 001h, 000h
        db  08Bh, 015h, 0F4h, 037h, 001h, 000h
        jmp    SHORT loc_1_128BE
loc_1_128B4:
        movsx  eax, byte ptr [esi]
        mov    ax, word ptr [edx + eax*2]
        and    eax, 4
loc_1_128BE:
        or     eax, eax
        je     SHORT loc_1_128CA
        movsx  eax, byte ptr [esi]
        sub    eax, 30h
        jmp    SHORT loc_1_12904
loc_1_128CA:
        cmp    ecx, 1
        jle    SHORT loc_1_128EE
        push   80h
        movsx  eax, byte ptr [esi]
        push   eax
        call   sub_1_1275C
        add    esp, 8
        db  08Bh, 00Dh, 060h, 02Fh, 001h, 000h
        db  08Bh, 015h, 0F4h, 037h, 001h, 000h
        jmp    SHORT loc_1_128FA
loc_1_128EE:
        movsx  eax, byte ptr [esi]
        mov    ax, word ptr [edx + eax*2]
        and    eax, 80h
loc_1_128FA:
        or     eax, eax
        je     SHORT loc_1_1290B
        movsx  eax, byte ptr [esi]
        sub    eax, 57h
loc_1_12904:
        add    edi, eax
        cmp    byte ptr [esi], 0
        jne    SHORT loc_1_12893
loc_1_1290B:
        mov    eax, edi
        pop    edi
        pop    esi
        ret
sub_1_12910:
        db  0CDh, 020h, 00Eh, 080h, 017h, 000h
        add    byte ptr [eax], al
sub_1_12918:
        db  0CDh, 020h, 013h, 080h, 017h, 000h
        add    byte ptr [eax], al
sub_1_12920:
        db  0CDh, 020h, 014h, 080h, 017h, 000h
        add    byte ptr [eax], al
sub_1_12928:
        db  0CDh, 020h, 018h, 080h, 017h, 000h
        add    byte ptr [eax], al
sub_1_12930:
        db  0CDh, 020h, 005h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_12938:
        db  0CDh, 020h, 006h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_12940:
        db  0CDh, 020h, 007h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_12948:
        db  0CDh, 020h, 00Ah, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_12950:
        db  0CDh, 020h, 00Bh, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_12958:
        db  0CDh, 020h, 00Eh, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_12960:
        db  0CDh, 020h, 00Fh, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_12968:
        db  0CDh, 020h, 011h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_12970:
        db  0CDh, 020h, 012h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_12978:
        db  0CDh, 020h, 013h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_12980:
        db  0CDh, 020h, 014h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_12988:
        db  0CDh, 020h, 015h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_12990:
        db  0CDh, 020h, 017h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_1_12998:
        pop    edx
        pop    ecx
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        jmp    edx
        db  000h, 000h
sub_1_129A4:
        push   ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
        db  000h
sub_1_129B0:
        pop    edx
        pop    ecx
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        cmc
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
        db  000h
sub_1_129C0:
        pop    edx
        pop    eax
        pop    ecx
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        jmp    edx
        db  000h
sub_1_129CC:
        push   esi
        mov    edx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        ret    0ch
        db  000h, 000h, 000h
sub_1_129E8:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        pop    esi
        ret    4
        db  000h
sub_1_129F8:
        db  0CDh, 020h, 0CFh, 080h, 001h, 000h
        add    byte ptr [eax], al
loc_1_12A00:
        and    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12A04:
        inc    eax
        add    byte ptr [eax], al
        db  000h
loc_1_12A08:
        db  02Dh, 03Ch, 000h, 000h, 087h
        aas
        add    byte ptr [eax], al
        fmul   qword ptr [ebx]
        db  000h, 072h, 04Ch
        add    byte ptr [eax], al
        dec    edx
        dec    ebp
        add    byte ptr [eax], al
        popfd
        dec    ebp
        add    byte ptr [eax], al
        mov    dh, 4eh
        add    byte ptr [eax], al
        cmp    byte ptr [edi], dh
        db  000h, 03Dh, 075h, 000h, 000h, 09Fh
        jbe    SHORT loc_1_12A2F
loc_1_12A2F:
        db  000h, 07Ch, 08Ah, 000h
        db  000h, 0EEh
        db  0B8h, 000h, 000h, 0E7h, 0BFh
        add    byte ptr [eax], al
        and    cl, al
        add    byte ptr [eax], al
        add    byte ptr [ecx], 0
        ror    bh, 1
        add    byte ptr [eax], al
        adc    al, 0d2h
        add    byte ptr [eax], al
        sbb    dl, dl
        add    byte ptr [eax], al
        and    dl, dl
        add    byte ptr [eax], al
        leave
        aam    0
        db  000h, 008h
        sub    al, byte ptr [ecx]
        db  000h
loc_1_12A5C:
        sub    byte ptr [esi], 0
        db  000h, 080h, 025h, 000h, 000h, 08Ch
        db  025h, 000h, 000h, 080h, 001h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   eax
        add    eax, dword ptr [eax]
        db  000h, 0D0h
        db  015h, 000h, 000h, 040h, 019h
        add    byte ptr [eax], al
        xor    byte ptr [edi], bl
        add    byte ptr [eax], al
        inc    eax
        add    eax, dword ptr [eax]
        db  000h, 000h
        add    byte ptr es:[eax], al
        push   27h
        add    byte ptr [eax], al
        adc    byte ptr [eax + eax], 0
        and    byte ptr [0], ah
        add    byte ptr [eax], al
loc_1_12A98:
        adc    al, byte ptr [ecx]
        push   eax
        adc    al, byte ptr [ecx]
        add    byte ptr [edx], dl
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
loc_1_12AA4:
        adc    al, byte ptr [edx]
        push   ecx
        adc    al, byte ptr [edx]
        add    byte ptr [edx], dl
        add    byte ptr [ecx], dl
        add    byte ptr [eax], al
loc_1_12AB0:
        adc    al, byte ptr [ebx]
        push   edx
        adc    al, byte ptr [ebx]
        add    byte ptr [edx], dl
        add    byte ptr [edx], dl
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 084h
loc_1_12AC9:
        add    byte ptr [eax], al
loc_1_12ACC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12AD0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
loc_1_12AEC:
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_12AF0:
        jecxz  loc_1_12B24
        add    byte ptr [eax], al
        dec    edi
        xor    al, byte ptr [eax]
        db  000h
loc_1_12AF8:
        db  0E8h, 0C4h, 000h, 000h, 040h
        les    eax, ptr [eax]
        add    byte ptr [eax], al
        db  0DDh, 06Dh, 000h
loc_1_12B04:
        call   sub_1_12B0C
        and    byte ptr [eax], al
        db  000h
sub_1_12B0C:
        add    byte ptr fs:[eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [ebp + 53h], cl
        push   esi
        push   esi
loc_1_12B24:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0DEh, 0D7h, 000h, 000h, 0C5h, 0F8h, 000h, 000h, 032h, 0FBh, 000h, 000h, 000h, 001h, 000h, 000h
        db  000h, 001h, 000h, 000h, 004h, 0F1h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 0F6h, 030h, 001h, 000h, 000h, 000h, 057h, 053h, 056h, 056h
        db  020h, 020h, 020h, 020h, 000h, 0C0h, 000h, 054h, 00Ah, 0F1h, 000h, 000h, 000h, 000h, 000h, 000h
        db  049h, 0D7h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  03Ch, 02Bh, 001h, 000h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h
loc_1_12B94:
        das
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], ah
        add    byte ptr [eax], al
        add    byte ptr [esi], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   esi
        push   esi
        inc    ebp
        pop    eax
        inc    ebp
        xor    esi, dword ptr [edx]
        inc    ebp
        pop    eax
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12BC4:
        add    dword ptr [eax], eax
loc_1_12BC6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12BCC:
        push   esi
        push   esi
        inc    ebp
        pop    eax
        inc    ebp
        xor    esi, dword ptr [edx]
        inc    ebp
        pop    eax
        inc    ebp
        db  000h
loc_1_12BD8:
        dec    ebx
        inc    ebp
        push   edx
        dec    esi
        inc    ebp
        dec    esp
        add    byte ptr [eax], al
loc_1_12BE0:
        or     eax, 13000000h
        add    byte ptr [eax], al
        add    byte ptr [ecx], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        db  000h
loc_1_12BF0:
        adc    byte ptr [eax], bl
        add    byte ptr [eax], al
loc_1_12BF4:
        adc    byte ptr [ebx], al
        add    byte ptr [eax], al
loc_1_12BF8:
        adc    byte ptr [edi], dl
        add    byte ptr [eax], al
loc_1_12BFC:
        adc    byte ptr [eax + eax], al
        add    byte ptr [eax], dl
        sbb    eax, dword ptr [eax]
        add    byte ptr [eax], dl
        xchg   ecx, eax
        add    byte ptr [eax], al
loc_1_12C08:
        adc    byte ptr [ebx + 30380000h], dl
        add    byte ptr [eax], al
loc_1_12C10:
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_1_12C18:
        dec    eax
        add    byte ptr [eax], al
        add    byte ptr [edx], bl
        add    byte ptr [eax], al
loc_1_12C20:
        sub    eax, 565653h
        add    byte ptr [eax], al
        add    byte ptr [434153h], ch
        add    byte ptr [eax], al
        add    byte ptr [32504953h], ch
        add    byte ptr [eax], al
        add    byte ptr [434953h], ch
        add    byte ptr [eax], al
        add    byte ptr [515353h], ch
        add    byte ptr [eax], al
        add    byte ptr [414453h], ch
        add    byte ptr [eax], al
        add    byte ptr [584653h], ch
        add    byte ptr [eax], al
        add    byte ptr [4d5253h], ch
        add    byte ptr [eax], al
        add    byte ptr [4d4553h], ch
        add    byte ptr [eax], al
        add    byte ptr [544d53h], ch
        add    byte ptr [eax], al
        add    byte ptr [ebx], ch
        inc    esi
        inc    ebx
        dec    esp
        inc    ecx
        push   ebx
        push   ebx
        cmp    eax, 0
loc_1_12C7C:
        sub    eax, 2d003058h
        push   ebx
        push   ecx
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12C88:
        sub    eax, 444353h
        add    byte ptr [eax], al
        add    byte ptr [3f524553h], ch
        add    byte ptr [eax], al
        add    byte ptr [505353h], ch
        add    byte ptr [eax], al
        add    byte ptr [525353h], ch
        add    byte ptr [eax], al
        add    byte ptr [ebx], ch
        inc    esi
        dec    esp
        dec    edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12CB0:
        sub    eax, dword ptr [esi + 50h]
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12CB8:
        sub    eax, dword ptr [esi + 4dh]
        dec    ecx
        aas
        add    byte ptr [eax], al
        add    byte ptr [ebx], ch
        inc    esi
        dec    ebp
        dec    ebp
        aas
        add    byte ptr [eax], al
        add    byte ptr [ebx], ch
        inc    esi
        dec    ebp
        push   edx
        aas
        add    byte ptr [eax], al
        add    byte ptr [esi], dl
        add    byte ptr [eax], al
loc_1_12CD4:
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [ecx], dl
        add    byte ptr [eax], al
loc_1_12CDC:
        inc    ebx
        add    byte ptr [eax], al
loc_1_12CE0:
        inc    esp
        add    byte ptr [eax], al
loc_1_12CE4:
        sub    eax, 524f53h
        add    byte ptr [eax], al
        add    byte ptr [4f4e53h], ch
        add    byte ptr [eax], al
        add    byte ptr [544453h], ch
        add    byte ptr [eax], al
        add    byte ptr [4d5453h], ch
        add    byte ptr [eax], al
        add    byte ptr [545453h], ch
        add    byte ptr [eax], al
        add    byte ptr [ebx + 30h], dl
        add    byte ptr [eax], al
loc_1_12D10:
        push   ebx
        xor    dword ptr [eax], eax
        add    byte ptr [ebx + 36h], dl
        add    byte ptr [eax], al
loc_1_12D18:
        push   ebx
        xor    esi, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 010h
        sub    al, 1
        db  000h, 014h, 02Ch
        add    dword ptr [eax], eax
        sbb    byte ptr [ecx + eax], ch
        db  000h, 01Ch, 02Ch
        add    dword ptr [eax], eax
        and    byte ptr [ecx + eax], ch
        db  000h, 028h
        sub    al, 1
        db  000h, 030h
        sub    al, 1
        db  000h, 038h
        sub    al, 1
        db  000h, 040h, 02Ch
        add    dword ptr [eax], eax
        dec    eax
        sub    al, 1
        db  000h, 050h, 02Ch
        add    dword ptr [eax], eax
        pop    eax
        sub    al, 1
        db  000h, 060h, 02Ch
        add    dword ptr [eax], eax
        db  068h, 02Ch, 001h, 000h, 070h
        sub    al, 1
        db  000h, 07Ch, 02Ch, 001h
        db  000h, 080h, 02Ch, 001h, 000h, 088h
        sub    al, 1
        db  000h, 090h, 02Ch, 001h, 000h, 098h
        sub    al, 1
        db  000h, 0A0h, 02Ch, 001h, 000h, 0A8h
        sub    al, 1
        db  000h, 0B0h, 02Ch, 001h, 000h, 0B8h
        sub    al, 1
        db  000h, 0C0h
        sub    al, 1
        db  000h, 0C8h
        sub    al, 1
        db  000h, 0D0h
        sub    al, 1
        db  000h, 0D4h
        sub    al, 1
        db  000h, 0D8h
        sub    al, 1
        db  000h, 0DCh
        sub    al, 1
        db  000h, 0E0h
        sub    al, 1
        db  000h, 0E4h
        sub    al, 1
        db  000h, 0ECh
        sub    al, 1
        db  000h, 0F4h
        sub    al, 1
        db  000h, 0FCh
        sub    al, 1
        db  000h, 004h, 02Dh, 001h, 000h, 00Ch, 02Dh
        add    dword ptr [eax], eax
        db  010h, 02Dh, 001h, 000h, 014h, 02Dh
        add    dword ptr [eax], eax
        db  018h, 02Dh, 001h, 000h, 01Ch, 02Dh
        add    dword ptr [eax], eax
loc_1_12DC4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12DC8:
        sub    eax, 414653h
        add    byte ptr [eax], al
        add    byte ptr [444d53h], ch
        add    byte ptr [eax], al
        add    byte ptr [415253h], ch
        add    byte ptr [eax], al
        add    byte ptr [3a435253h], ch
        add    byte ptr [eax], al
        add    byte ptr [515253h], ch
        add    byte ptr [eax], al
        add    byte ptr [565353h], ch
        add    byte ptr [eax], al
        add    byte ptr [4f5453h], ch
        add    byte ptr [eax], al
        add    byte ptr [4d5653h], ch
        add    byte ptr [eax], al
        add    byte ptr [edx + 55h], al
        push   ebx
        pop    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12E10:
        inc    ebx
        dec    edi
        dec    esi
        dec    esi
        inc    ebp
        inc    ebx
        push   esp
        add    byte ptr [ebx + 4fh], al
        dec    esi
        dec    esi
        inc    ebp
        inc    ebx
        push   esp
        add    byte ptr [ebp + 52h], al
        push   edx
        dec    edi
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [esi + 4fh], cl
        and    byte ptr [ecx + 4eh], al
        push   ebx
        push   edi
        inc    ebp
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [esi + 4fh], cl
        and    byte ptr [ebx + 41h], al
        push   edx
        push   edx
        dec    ecx
        inc    ebp
        push   edx
        add    byte ptr [eax], al
loc_1_12E40:
        dec    esi
        dec    edi
        and    byte ptr [ecx + ecx*2 + 41h], al
        dec    esp
        push   esp
        dec    edi
        dec    esi
        inc    ebp
        add    byte ptr [edi + 4bh], cl
        add    byte ptr [eax], al
loc_1_12E50:
        push   edx
        dec    ecx
        dec    esi
        inc    edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12E58:
        or     eax, 0ah
        add    byte ptr [eax], al
        db  000h, 0C4h
        db  02Dh, 001h, 000h, 0C8h, 02Dh
        add    dword ptr [eax], eax
        db  0D0h, 02Dh, 001h, 000h, 0D8h, 02Dh
        add    dword ptr [eax], eax
        db  0E0h, 02Dh
        add    dword ptr [eax], eax
        db  0E8h, 02Dh, 001h, 000h, 0F0h
        db  02Dh, 001h, 000h, 0F8h, 02Dh
        add    dword ptr [eax], eax
        add    byte ptr [esi], ch
        add    dword ptr [eax], eax
        or     byte ptr [esi], ch
        add    dword ptr [eax], eax
        adc    byte ptr [esi], ch
        add    dword ptr [eax], eax
        sbb    byte ptr [esi], ch
        add    dword ptr [eax], eax
        and    byte ptr [esi], ch
        add    dword ptr [eax], eax
        sub    byte ptr [esi], ch
        add    dword ptr [eax], eax
        xor    al, 2eh
        add    dword ptr [eax], eax
        inc    eax
        add    dword ptr cs:[eax], eax
        dec    esp
        add    dword ptr cs:[eax], eax
        push   eax
        add    dword ptr cs:[eax], eax
loc_1_12EA8:
        pop    eax
        add    dword ptr cs:[eax], eax
        pop    esp
        add    dword ptr cs:[eax], eax
loc_1_12EB0:
        db  010h, 042h, 012h
        add    dword ptr [eax], eax
loc_1_12EB5:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  018h, 05Fh, 012h
        add    dword ptr [eax], eax
        lock sub eax, dword ptr [ecx]
        add    byte ptr [ebx], al
        adc    dl, byte ptr [ebx]
        add    dword ptr [eax], eax
        hlt
        sub    eax, dword ptr [ecx]
        add    byte ptr [edi], dl
        pop    ebp
        adc    eax, dword ptr [ecx]
        db  000h, 0F8h
        sub    eax, dword ptr [ecx]
        db  000h, 004h, 0A8h
        adc    eax, dword ptr [ecx]
        db  000h, 0FCh
        sub    eax, dword ptr [ecx]
        add    byte ptr [ebx], bl
        or     dword ptr [ecx + eax], edx
        db  000h, 000h
        sub    al, 1
        db  000h, 091h, 0C0h, 012h, 001h, 000h
        add    al, 2ch
        add    dword ptr [eax], eax
        xchg   ebx, eax
        db  0E9h, 012h, 001h, 000h, 008h
        db  02Ch, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_12F60:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_12F70:
        push   esi
        push   edi
        call   sub_1_132BD
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_12FDE
        mov    edi, dword ptr [esp + 0ch]
        push   edi
        call   sub_1_13377
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_12FE3
        cmp    edi, 1000h
        jbe    SHORT loc_1_12FC3
        push   edi
        push   esi
        call   sub_1_1348B
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_12FC3
        mov    dword ptr [esi + 28h], eax
        mov    dword ptr [esi + 30h], eax
        mov    dword ptr [esi + 2ch], 0
        mov    dword ptr [esi + 34h], 0
        mov    eax, esi
        mov    dword ptr [esi + 1ch], 80000000h
        jmp    SHORT loc_1_12FE0
loc_1_12FC3:
        db  0C7h, 006h, 068h, 036h, 001h, 000h
        db  0A1h, 06Ch, 036h, 001h, 000h
        mov    dword ptr [esi + 4], eax
        db  0A1h, 06Ch, 036h, 001h, 000h
        mov    dword ptr [eax], esi
        db  089h, 035h, 06Ch, 036h, 001h, 000h
loc_1_12FDE:
        sub    eax, eax
loc_1_12FE0:
        pop    edi
        pop    esi
        ret
loc_1_12FE3:
        lea    ecx, [esi + 10h]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [esi + 14h]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [ecx], eax
        mov    dword ptr [esi + 14h], eax
        mov    dword ptr [eax + 0ch], 0
        mov    ecx, dword ptr [eax + 20h]
        mov    dword ptr [eax + 18h], ecx
        mov    dword ptr [esi + 28h], eax
        mov    dword ptr [esi + 30h], eax
        mov    eax, esi
        mov    dword ptr [esi + 2ch], 0
        mov    dword ptr [esi + 34h], 0
        mov    dword ptr [esi + 1ch], 0
        jmp    SHORT loc_1_12FE0
sub_1_1301F:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        test   byte ptr [esi + 1fh], 80h
        jne    SHORT loc_1_1304F
        mov    ecx, dword ptr [esi + 30h]
        mov    edx, dword ptr [ecx + 8]
        or     edx, edx
        je     SHORT loc_1_13044
        mov    dword ptr [ecx], edx
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx + 4], ecx
        jmp    SHORT loc_1_1308D
loc_1_13044:
        push   ecx
        call   sub_1_1336A
        add    esp, 4
        jmp    SHORT loc_1_1308D
loc_1_1304F:
        mov    eax, dword ptr [esi + 10h]
        lea    edi, [esi + 10h]
        sub    eax, esi
        cmp    eax, 10h
        je     SHORT loc_1_1308D
loc_1_1305C:
        mov    edx, dword ptr [edi]
        mov    eax, dword ptr [edx]
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [edi], eax
        mov    ecx, dword ptr [edx + 8]
        or     ecx, ecx
        je     SHORT loc_1_1307B
        mov    dword ptr [edx], ecx
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 4], edx
        jmp    SHORT loc_1_13084
loc_1_1307B:
        push   edx
        call   sub_1_1336A
        add    esp, 4
loc_1_13084:
        mov    eax, dword ptr [edi]
        sub    eax, esi
        cmp    eax, 10h
        jne    SHORT loc_1_1305C
loc_1_1308D:
        db  0C7h, 006h, 068h, 036h, 001h, 000h
        db  0A1h, 06Ch, 036h, 001h, 000h
        mov    dword ptr [esi + 4], eax
        db  0A1h, 06Ch, 036h, 001h, 000h
        pop    edi
        mov    dword ptr [eax], esi
        db  089h, 035h, 06Ch, 036h, 001h, 000h
        pop    esi
        ret
sub_1_130AB:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 20h]
        mov    eax, dword ptr [esp + 24h]
        mov    edx, dword ptr [esi + 30h]
        mov    ebx, dword ptr [esi + 34h]
        mov    ecx, dword ptr [esi + 28h]
        mov    dword ptr [esp + 14h], ecx
        mov    ecx, dword ptr [esi + 2ch]
        mov    dword ptr [esp + 10h], ecx
        test   byte ptr [esi + 1fh], 80h
        jne    SHORT loc_1_13117
        mov    eax, ecx
        sub    eax, ebx
        mov    ecx, dword ptr [esp + 24h]
        cmp    eax, ecx
        jb     SHORT loc_1_130E2
        mov    eax, ecx
loc_1_130E2:
        or     eax, eax
        je     NEAR PTR loc_1_1318E
        mov    edi, dword ptr [esp + 1ch]
        lea    esi, [edx + ebx + 24h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        cmp    dword ptr [esp + 28h], 0
        jne    NEAR PTR loc_1_1318E
        mov    esi, dword ptr [esp + 20h]
        lea    ecx, [ebx + eax]
        mov    dword ptr [esi + 34h], ecx
        jmp    SHORT loc_1_1318E
loc_1_13117:
        cmp    dword ptr [esp + 24h], 0
        je     SHORT loc_1_13179
loc_1_1311E:
        cmp    dword ptr [esp + 14h], edx
        jne    SHORT loc_1_1312A
        mov    ecx, dword ptr [esp + 10h]
        jmp    SHORT loc_1_1312D
loc_1_1312A:
        mov    ecx, dword ptr [edx + 20h]
loc_1_1312D:
        sub    ecx, ebx
        mov    ebp, dword ptr [esp + 24h]
        cmp    ebp, ecx
        jb     SHORT loc_1_13139
        mov    ebp, ecx
loc_1_13139:
        or     ebp, ebp
        je     SHORT loc_1_13179
        mov    edi, dword ptr [esp + 1ch]
        lea    esi, [edx + ebx + 24h]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        add    ebx, ebp
        sub    dword ptr [esp + 24h], ebp
        cmp    dword ptr [esp + 14h], edx
        jne    SHORT loc_1_13165
        cmp    dword ptr [esp + 10h], ebx
        je     SHORT loc_1_13179
loc_1_13165:
        cmp    dword ptr [edx + 20h], ebx
        jne    SHORT loc_1_1316E
        sub    ebx, ebx
        mov    edx, dword ptr [edx]
loc_1_1316E:
        add    dword ptr [esp + 1ch], ebp
        cmp    dword ptr [esp + 24h], 0
        jne    SHORT loc_1_1311E
loc_1_13179:
        cmp    dword ptr [esp + 28h], 0
        jne    SHORT loc_1_1318A
        mov    esi, dword ptr [esp + 20h]
        mov    dword ptr [esi + 30h], edx
        mov    dword ptr [esi + 34h], ebx
loc_1_1318A:
        sub    eax, dword ptr [esp + 24h]
loc_1_1318E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_13196:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 14h]
        mov    edx, dword ptr [ecx + 28h]
        mov    ebx, dword ptr [ecx + 2ch]
        test   byte ptr [ecx + 1fh], 80h
        jne    SHORT loc_1_131E0
        mov    eax, dword ptr [edx + 20h]
        sub    eax, ebx
        mov    ecx, dword ptr [esp + 1ch]
        cmp    eax, ecx
        jb     SHORT loc_1_131B9
        mov    eax, ecx
loc_1_131B9:
        lea    edi, [edx + ebx + 24h]
        mov    esi, dword ptr [esp + 18h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [esp + 14h]
        add    ebx, eax
        add    dword ptr [ecx + 20h], eax
        mov    dword ptr [ecx + 2ch], ebx
        mov    dword ptr [edx + 18h], ebx
        jmp    SHORT loc_1_1324A
loc_1_131E0:
        mov    eax, dword ptr [esp + 1ch]
        or     eax, eax
        je     SHORT loc_1_13239
loc_1_131E8:
        mov    ebp, dword ptr [edx + 20h]
        sub    ebp, ebx
        mov    ecx, dword ptr [esp + 1ch]
        cmp    ebp, ecx
        jb     SHORT loc_1_131F7
        mov    ebp, ecx
loc_1_131F7:
        lea    edi, [edx + ebx + 24h]
        mov    esi, dword ptr [esp + 18h]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        add    ebx, ebp
        sub    dword ptr [esp + 1ch], ebp
        mov    dword ptr [edx + 18h], ebx
        cmp    dword ptr [edx + 20h], ebx
        jne    SHORT loc_1_1322A
        sub    ebx, ebx
        mov    edx, dword ptr [edx]
        mov    ecx, dword ptr [esp + 14h]
        sub    ecx, edx
        cmp    ecx, -10h
        je     SHORT loc_1_13237
loc_1_1322A:
        add    dword ptr [esp + 18h], ebp
        cmp    dword ptr [esp + 1ch], 0
        jne    SHORT loc_1_131E8
        jmp    SHORT loc_1_13239
loc_1_13237:
        sub    edx, edx
loc_1_13239:
        mov    ecx, dword ptr [esp + 14h]
        sub    eax, dword ptr [esp + 1ch]
        mov    dword ptr [ecx + 28h], edx
        mov    dword ptr [ecx + 2ch], ebx
        add    dword ptr [ecx + 20h], eax
loc_1_1324A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_1324F:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    esi, dword ptr [esp + 0ch]
        mov    dword ptr [edx + 20h], esi
        test   byte ptr [edx + 1fh], 80h
        jne    SHORT loc_1_13269
        mov    eax, dword ptr [edx + 30h]
        mov    dword ptr [edx + 28h], eax
        jmp    SHORT loc_1_1328E
loc_1_13269:
        mov    ecx, dword ptr [edx + 10h]
        mov    eax, edx
        sub    eax, ecx
        cmp    eax, -10h
        je     SHORT loc_1_13291
loc_1_13275:
        mov    eax, dword ptr [ecx + 20h]
        cmp    esi, eax
        jbe    SHORT loc_1_1328B
        sub    esi, eax
        mov    ecx, dword ptr [ecx]
        mov    eax, edx
        sub    eax, ecx
        cmp    eax, -10h
        jne    SHORT loc_1_13275
        jmp    SHORT loc_1_13291
loc_1_1328B:
        mov    dword ptr [edx + 28h], ecx
loc_1_1328E:
        mov    dword ptr [edx + 2ch], esi
loc_1_13291:
        pop    esi
        ret
sub_1_13293:
        push   edi
        call   sub_1_F3A4
        push   9eh
        call   sub_1_F390
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        je     SHORT loc_1_132B9
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 27h
        rep stosd
        stosw
loc_1_132B9:
        mov    eax, edx
        pop    edi
        ret
sub_1_132BD:
        db  081h, 03Dh, 068h, 036h, 001h, 000h, 068h, 036h, 001h, 000h
        je     SHORT loc_1_132F0
        db  0A1h, 068h, 036h, 001h, 000h
        mov    ecx, eax
        mov    ecx, dword ptr [ecx]
        db  0C7h, 041h, 004h, 068h, 036h, 001h, 000h
        db  089h, 00Dh, 068h, 036h, 001h, 000h
        mov    dword ptr [eax + 20h], 0
        lea    ecx, [eax + 10h]
        mov    dword ptr [eax + 14h], ecx
        mov    dword ptr [ecx], ecx
        jmp    SHORT loc_1_1331E
loc_1_132F0:
        db  081h, 03Dh, 018h, 037h, 001h, 000h, 0A0h, 000h, 000h, 000h
        jae    SHORT loc_1_1331C
        call   sub_1_13293
        or     eax, eax
        je     SHORT loc_1_1331C
        db  08Bh, 00Dh, 018h, 037h, 001h, 000h
        inc    ecx
        db  089h, 00Dh, 018h, 037h, 001h, 000h
        lea    ecx, [eax + 10h]
        mov    dword ptr [eax + 14h], ecx
        mov    dword ptr [ecx], ecx
        jmp    SHORT loc_1_1331E
loc_1_1331C:
        sub    eax, eax
loc_1_1331E:
        ret
sub_1_1331F:
        push   esi
        push   edi
        call   sub_1_F3A4
        mov    esi, dword ptr [esp + 0ch]
        lea    eax, [esi + 24h]
        push   eax
        call   sub_1_F390
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        je     SHORT loc_1_13365
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 9
        rep stosd
        stosb
        mov    dword ptr [edx + 20h], esi
        mov    dword ptr [edx + 1ch], 4655424eh
        mov    dword ptr [edx + 0ch], eax
        lea    eax, [edx + 24h]
        mov    dword ptr [edx + 18h], esi
        mov    dword ptr [edx + 10h], eax
        mov    dword ptr [edx + 14h], 0
loc_1_13365:
        mov    eax, edx
        pop    edi
        pop    esi
        ret
sub_1_1336A:
        push   dword ptr [esp + 4]
        call   sub_1_F396
        add    esp, 4
        ret
sub_1_13377:
        push   esi
        sub    eax, eax
        mov    esi, dword ptr [esp + 8]
        cmp    esi, 40h
        ja     SHORT loc_1_133D3
        db  081h, 03Dh, 064h, 037h, 001h, 000h, 064h, 037h, 001h, 000h
        je     SHORT loc_1_133AA
        db  0A1h, 064h, 037h, 001h, 000h
        mov    ecx, eax
        mov    ecx, dword ptr [ecx]
        db  0C7h, 041h, 004h, 064h, 037h, 001h, 000h
        db  089h, 00Dh, 064h, 037h, 001h, 000h
        jmp    NEAR PTR loc_1_13489
loc_1_133AA:
        db  083h, 03Dh, 028h, 038h, 001h, 000h, 050h
        jae    SHORT loc_1_133D3
        push   40h
        call   sub_1_1331F
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_133D3
        db  0FFh, 005h, 028h, 038h, 001h, 000h
        db  0C7h, 040h, 008h, 064h, 037h, 001h, 000h
        jmp    NEAR PTR loc_1_13489
loc_1_133D3:
        cmp    esi, 5e0h
        ja     SHORT loc_1_1342B
        db  081h, 03Dh, 090h, 036h, 001h, 000h, 090h, 036h, 001h, 000h
        je     SHORT loc_1_13402
        db  0A1h, 090h, 036h, 001h, 000h
        mov    ecx, eax
        mov    ecx, dword ptr [ecx]
        db  0C7h, 041h, 004h, 090h, 036h, 001h, 000h
        db  089h, 00Dh, 090h, 036h, 001h, 000h
        jmp    NEAR PTR loc_1_13489
loc_1_13402:
        db  083h, 03Dh, 01Ch, 039h, 001h, 000h, 028h
        jae    SHORT loc_1_1342B
        push   5e0h
        call   sub_1_1331F
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_1342B
        db  0FFh, 005h, 01Ch, 039h, 001h, 000h
        db  0C7h, 040h, 008h, 090h, 036h, 001h, 000h
        jmp    SHORT loc_1_13489
loc_1_1342B:
        cmp    esi, 1000h
        ja     SHORT loc_1_13480
        db  081h, 03Dh, 084h, 038h, 001h, 000h, 084h, 038h, 001h, 000h
        je     SHORT loc_1_13457
        db  0A1h, 084h, 038h, 001h, 000h
        mov    ecx, eax
        mov    ecx, dword ptr [ecx]
        db  0C7h, 041h, 004h, 084h, 038h, 001h, 000h
        db  089h, 00Dh, 084h, 038h, 001h, 000h
        jmp    SHORT loc_1_13489
loc_1_13457:
        db  083h, 03Dh, 038h, 036h, 001h, 000h, 014h
        jae    SHORT loc_1_13489
        push   1000h
        call   sub_1_1331F
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_13489
        db  0FFh, 005h, 038h, 036h, 001h, 000h
        db  0C7h, 040h, 008h, 084h, 038h, 001h, 000h
        jmp    SHORT loc_1_13489
loc_1_13480:
        push   esi
        call   sub_1_1331F
        add    esp, 4
loc_1_13489:
        pop    esi
        ret
sub_1_1348B:
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    edi, [esp + 10h]
        mov    ebp, dword ptr [esp + 40h]
        or     ebp, ebp
        mov    esi, dword ptr [esp + 3ch]
        je     SHORT loc_1_13515
loc_1_134A2:
        mov    ebx, ebp
        cmp    ebx, 1000h
        jb     SHORT loc_1_134B1
        mov    ebx, 1000h
loc_1_134B1:
        push   ebx
        call   sub_1_13377
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_134E2
        lea    ecx, [eax + 0ch]
        mov    dword ptr [edi + 0ch], ecx
        lea    ecx, [esi + 10h]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [esi + 14h]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [ecx], eax
        mov    edi, eax
        mov    dword ptr [esi + 14h], eax
        mov    ecx, dword ptr [eax + 20h]
        mov    dword ptr [eax + 18h], ecx
        sub    ebp, ebx
        jne    SHORT loc_1_134A2
        jmp    SHORT loc_1_13515
loc_1_134E2:
        mov    eax, dword ptr [esi + 10h]
        lea    edi, [esi + 10h]
        sub    eax, esi
        cmp    eax, 10h
        je     SHORT loc_1_13511
loc_1_134EF:
        mov    eax, dword ptr [edi]
        mov    ebx, eax
        mov    edx, dword ptr [eax]
        mov    dword ptr [edx + 4], edi
        mov    dword ptr [edi], edx
        mov    ecx, dword ptr [ebx + 8]
        sub    edx, esi
        mov    dword ptr [ebx], ecx
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [eax], ebx
        cmp    edx, 10h
        mov    dword ptr [ecx + 4], ebx
        jne    SHORT loc_1_134EF
loc_1_13511:
        sub    eax, eax
        jmp    SHORT loc_1_13518
loc_1_13515:
        mov    eax, dword ptr [esi + 10h]
loc_1_13518:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
        db  000h, 000h, 000h, 000h
loc_1_13524:
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13528:
        cmp    byte ptr [eax], dh
        add    byte ptr [eax], al
loc_1_1352C:
        or     eax, 5252450ah
        dec    edi
        push   edx
        or     eax, 0ah
loc_1_13538:
        or     eax, 204f4e0ah
        inc    ebx
        inc    ecx
        push   edx
        push   edx
        dec    ecx
        inc    ebp
        push   edx
        or     eax, 0d00000ah
        or     al, byte ptr [ebx + 4fh]
        dec    esi
        dec    esi
        inc    ebp
        inc    ebx
        push   esp
        or     eax, 5441000ah
        sub    eax, 0d434153h
        add    byte ptr [5252450ah], cl
        dec    edi
        push   edx
        or     eax, 0ah
loc_1_13568:
        inc    ecx
        push   esp
        add    byte ptr [eax], al
loc_1_1356C:
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13570:
        inc    ecx
        push   esp
        add    byte ptr [eax], al
loc_1_13574:
        xor    byte ptr [ecx], dh
        xor    dh, byte ptr [ebx]
        xor    al, 35h
        aaa
        cmp    byte ptr [ecx], bh
        popad
        bound  esp, qword ptr [ebx + 64h]
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
        and    al, 48h
        popad
        jb     NEAR PTR loc_1_13590
loc_1_13590:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_13594:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13598:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1359C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_135A0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_135A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13624:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1362C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13630:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13634:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13638:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1363C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13640:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13644:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13648:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1364C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13650:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13654:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13660:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13664:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13668:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1366C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13670:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13674:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13678:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1367C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13690:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13694:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13698:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1369C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_136A0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_136A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_136B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_136D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_136EC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_136F0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13718:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1371C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13720:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13724:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13728:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1372C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13754:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13758:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13764:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13768:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1376C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13770:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13778:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1377C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13780:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13784:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13788:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1378C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13790:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13794:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_137B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_137BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_137C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_137C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_137E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_137EC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_137F0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_137F4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_137F8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_137FC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13800:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1380C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13818:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1381C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13820:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13824:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13828:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1382C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13830:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13834:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13838:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1383C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13840:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13844:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13848:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1384C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13850:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13854:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13858:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1385C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13868:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13874:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13878:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1387C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13880:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13884:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13888:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1388C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13890:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13894:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_13898:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1389C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_138A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_138A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_138B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_138BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_138C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
