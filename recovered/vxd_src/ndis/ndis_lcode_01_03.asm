; ndis_lcode_01_03.asm -- NDIS object 1 segment _LTEXT  (part 3/3)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_8750:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        push   ebp
        mov    edi, dword ptr [esi + 50h]
        lea    ebp, [edi + 2eh]
        cmp    byte ptr [ebp], 0
        je     SHORT loc_1_8768
        xor    bl, bl
        jmp    SHORT loc_1_879B
loc_1_8768:
        mov    ebx, 1
        mov    byte ptr [ebp], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_879B
        db  03Bh, 03Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_879B
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 052h, 000h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_1_879B:
        test   bl, bl
        je     NEAR PTR loc_1_885D
        test   byte ptr [edi + 3ch], 2
        jne    SHORT loc_1_8813
        push   0
        mov    eax, dword ptr [esi + 4ch]
        push   0
        push   eax
        push   0
        call   dword ptr [esi + 48h]
        mov    ecx, edi
        call   dword ptr [edi + 118h]
        test   bl, bl
        je     NEAR PTR loc_1_8870
        mov    byte ptr [ebp], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_87F9
        db  03Bh, 03Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_87F9
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 084h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_87F9:
        cmp    byte ptr [edi + 2ch], 1
        jne    SHORT loc_1_8870
        mov    byte ptr [edi + 2ch], 0
        push   ebp
        call   sub_1_689B
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    10h
loc_1_8813:
        test   bl, bl
        je     SHORT loc_1_885D
        mov    byte ptr [ebp], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_884A
        db  03Bh, 03Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_884A
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 058h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_884A:
        cmp    byte ptr [edi + 2ch], 1
        jne    SHORT loc_1_885D
        mov    byte ptr [edi + 2ch], 0
        push   ebp
        call   sub_1_689B
        add    esp, 4
loc_1_885D:
        push   0
        add    esi, 28h
        push   esi
        mov    edx, 7
        mov    ecx, edi
        call   dword ptr [edi + 120h]
loc_1_8870:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    10h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_8880:
        mov    eax, dword ptr [esp + 4]
        push   eax
        call   sub_1_85C0
        mov    ecx, dword ptr [esp + 8]
        mov    byte ptr [ecx], al
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_88A0:
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 4]
        push   eax
        push   ecx
        mov    eax, dword ptr [edx]
        push   eax
        call   sub_1_37C0
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_88C0:
        push   ebx
        xor    eax, eax
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        test   byte ptr [esi + 3ch], 40h
        jne    NEAR PTR loc_1_8A57
        lea    edi, [esi + 2eh]
        cmp    byte ptr [edi], al
        je     SHORT loc_1_88DE
        xor    bl, bl
        jmp    SHORT loc_1_8910
loc_1_88DE:
        mov    ebx, 1
        mov    byte ptr [edi], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_8910
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_8910
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 03Ch, 001h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_1_8910:
        test   bl, bl
        je     NEAR PTR loc_1_8A57
        mov    ecx, dword ptr [esi + 3ch]
        test   ecx, 0c0000000h
        jne    NEAR PTR loc_1_8A0E
        mov    ecx, dword ptr [esi + 8]
        mov    ecx, dword ptr [ecx + 20h]
        test   ecx, ecx
        je     SHORT loc_1_8937
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        call   ecx
loc_1_8937:
        test   al, al
        jne    SHORT loc_1_89A3
        mov    ecx, dword ptr [esi + 3ch]
        test   ch, 80h
        jne    SHORT loc_1_895E
        cmp    dword ptr [esi + 1e0h], 0
        je     SHORT loc_1_895E
        test   ch, 2
        je     SHORT loc_1_8955
        mov    al, 1
        jmp    SHORT loc_1_899F
loc_1_8955:
        or     ecx, 200h
        mov    dword ptr [esi + 3ch], ecx
loc_1_895E:
        test   byte ptr [esi + 3dh], 40h
        jne    SHORT loc_1_8980
        mov    edx, dword ptr [esi + 1a8h]
        test   edx, edx
        je     SHORT loc_1_8980
        mov    cl, byte ptr [edx + 1dh]
        test   cl, 2
        je     SHORT loc_1_897A
        mov    al, 1
        jmp    SHORT loc_1_899F
loc_1_897A:
        or     cl, 2
        mov    byte ptr [edx + 1dh], cl
loc_1_8980:
        test   byte ptr [esi + 3eh], 1
        jne    SHORT loc_1_899F
        mov    cl, byte ptr [esi + 170h]
        cmp    cl, 1
        jne    SHORT loc_1_8995
        mov    al, 1
        jmp    SHORT loc_1_899F
loc_1_8995:
        jbe    SHORT loc_1_899F
        dec    cl
        mov    byte ptr [esi + 170h], cl
loc_1_899F:
        test   al, al
        je     SHORT loc_1_89B3
loc_1_89A3:
        push   0
        mov    edx, 1
        push   esi
        mov    ecx, esi
        call   dword ptr [esi + 11ch]
loc_1_89B3:
        mov    ecx, esi
        call   dword ptr [esi + 118h]
        test   bl, bl
        je     NEAR PTR loc_1_8A57
        mov    byte ptr [edi], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_89F5
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_89F5
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0C7h, 001h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_89F5:
        cmp    byte ptr [esi + 2ch], 1
        jne    SHORT loc_1_8A57
        mov    byte ptr [esi + 2ch], 0
        push   edi
        call   sub_1_689B
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret    10h
loc_1_8A0E:
        test   bl, bl
        je     SHORT loc_1_8A57
        mov    byte ptr [edi], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_8A44
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_8A44
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 045h, 001h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_8A44:
        cmp    byte ptr [esi + 2ch], 1
        jne    SHORT loc_1_8A57
        mov    byte ptr [esi + 2ch], 0
        push   edi
        call   sub_1_689B
        add    esp, 4
loc_1_8A57:
        pop    edi
        pop    esi
        pop    ebx
        ret    10h
        db  08Dh, 049h, 000h
sub_1_8A60:
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 14h]
        push   edi
        mov    edi, dword ptr [esi + 34h]
        mov    eax, dword ptr [edi + 3ch]
        test   al, 1
        jne    SHORT loc_1_8ACA
        test   al, 40h
        jne    SHORT loc_1_8A96
        mov    eax, dword ptr [edi + 0ch]
        lea    ecx, [esp + 0eh]
        lea    edx, [esp + 0fh]
        push   eax
        push   ecx
        push   edx
        call   dword ptr [esi + 0ch]
        cmp    byte ptr [esp + 0eh], 0
        je     NEAR PTR loc_1_8B16
        jmp    SHORT loc_1_8AD9
loc_1_8A96:
        cmp    byte ptr [esi + 4ch], 0
        jne    SHORT loc_1_8AB7
        cmp    byte ptr [esi + 4dh], 0
        jne    SHORT loc_1_8AB7
        test   al, 2
        jne    SHORT loc_1_8AB7
        mov    eax, dword ptr [edi + 0ch]
        mov    ebx, dword ptr [edi + 8]
        push   eax
        call   dword ptr [ebx + 24h]
        mov    byte ptr [esp + 0fh], 1
        jmp    SHORT loc_1_8B16
loc_1_8AB7:
        mov    eax, dword ptr [edi + 0ch]
        lea    ecx, [esp + 0eh]
        lea    edx, [esp + 0fh]
        push   eax
        push   ecx
        push   edx
        call   dword ptr [esi + 0ch]
        jmp    SHORT loc_1_8B16
loc_1_8ACA:
        mov    eax, dword ptr [edi + 0ch]
        mov    ebx, dword ptr [edi + 8]
        push   eax
        call   dword ptr [ebx + 24h]
        mov    byte ptr [esp + 0fh], 1
loc_1_8AD9:
        lea    ebx, [esi + 14h]
        inc    dword ptr [esi + 38h]
        push   ebx
        call   sub_1_8630
        test   al, al
        jne    SHORT loc_1_8B16
        dec    dword ptr [esi + 38h]
        test   byte ptr [edi + 3ch], 40h
        je     SHORT loc_1_8B16
        cmp    byte ptr [esi + 38h], 0
        jne    SHORT loc_1_8B16
        mov    edi, ebx
        xor    eax, eax
        mov    ecx, 8
        push   ebx
        rep stosd
        db  0C7h, 046h, 020h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 3ch]
        mov    dword ptr [esi + 24h], eax
        call   sub_1_8630
loc_1_8B16:
        mov    al, byte ptr [esp + 0fh]
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_8B30:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        push   ebp
        mov    edi, dword ptr [esi + 34h]
        mov    eax, dword ptr [esi + 10h]
        test   byte ptr [edi + 3ch], 40h
        je     SHORT loc_1_8B61
        dec    dword ptr [esi + 38h]
        cmp    byte ptr [esi + 38h], 0
        jne    NEAR PTR loc_1_8C4B
        add    esi, 3ch
        push   esi
        call   sub_1_7380
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    10h
loc_1_8B61:
        lea    ebp, [edi + 2eh]
        cmp    byte ptr [ebp], 0
        je     SHORT loc_1_8B6E
        xor    bl, bl
        jmp    SHORT loc_1_8BA1
loc_1_8B6E:
        mov    ebx, 1
        mov    byte ptr [ebp], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_8BA1
        db  039h, 03Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_8BA1
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 08Dh, 002h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_1_8BA1:
        test   bl, bl
        jne    SHORT loc_1_8BBF
        push   0
        lea    eax, [edi + 70h]
        push   eax
        xor    edx, edx
        mov    ecx, edi
        call   dword ptr [edi + 11ch]
        pop    ebp
        dec    dword ptr [esi + 38h]
        pop    edi
        pop    esi
        pop    ebx
        ret    10h
loc_1_8BBF:
        mov    ecx, dword ptr [edi + 0ch]
        push   ecx
        call   eax
        dec    dword ptr [esi + 38h]
        test   byte ptr [edi + 3ch], 40h
        jne    SHORT loc_1_8BF2
        mov    eax, dword ptr [edi + 158h]
        test   eax, eax
        je     SHORT loc_1_8BE8
        mov    ecx, dword ptr [edi + 0ch]
        push   ecx
        push   eax
        mov    eax, dword ptr [edi + 38h]
        mov    edx, dword ptr [eax]
        push   edx
        call   sub_1_37C0
loc_1_8BE8:
        mov    ecx, edi
        call   dword ptr [edi + 118h]
        jmp    SHORT loc_1_8C01
loc_1_8BF2:
        cmp    byte ptr [esi + 38h], 0
        jne    SHORT loc_1_8C01
        add    esi, 3ch
        push   esi
        call   sub_1_7380
loc_1_8C01:
        test   bl, bl
        je     SHORT loc_1_8C4B
        mov    byte ptr [ebp], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_8C38
        db  039h, 03Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_8C38
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0C8h, 002h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_8C38:
        cmp    byte ptr [edi + 2ch], 1
        jne    SHORT loc_1_8C4B
        mov    byte ptr [edi + 2ch], 0
        push   ebp
        call   sub_1_689B
        add    esp, 4
loc_1_8C4B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    10h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_8C60:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        push   ebp
        mov    ebp, dword ptr [esi + 150h]
        test   byte ptr [esi + 3ch], 2
        jne    NEAR PTR loc_1_8D60
        lea    edi, [esi + 2eh]
        cmp    byte ptr [edi], 0
        je     SHORT loc_1_8C84
        xor    bl, bl
        jmp    SHORT loc_1_8CB6
loc_1_8C84:
        mov    ebx, 1
        mov    byte ptr [edi], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_8CB6
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_8CB6
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0F9h, 002h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_1_8CB6:
        test   bl, bl
        jne    SHORT loc_1_8CD1
        push   0
        lea    eax, [esi + 70h]
        push   eax
        xor    edx, edx
        mov    ecx, esi
        call   dword ptr [esi + 11ch]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    10h
loc_1_8CD1:
        mov    eax, dword ptr [esi + 154h]
        test   eax, eax
        je     SHORT loc_1_8CEB
        mov    ecx, dword ptr [esi + 0ch]
        push   ecx
        push   eax
        mov    eax, dword ptr [esi + 38h]
        mov    edx, dword ptr [eax]
        push   edx
        call   sub_1_37C0
loc_1_8CEB:
        test   ebp, ebp
        je     SHORT loc_1_8CF5
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        call   ebp
loc_1_8CF5:
        mov    eax, dword ptr [esi + 158h]
        test   eax, eax
        je     SHORT loc_1_8D0F
        mov    ecx, dword ptr [esi + 0ch]
        push   ecx
        push   eax
        mov    eax, dword ptr [esi + 38h]
        mov    edx, dword ptr [eax]
        push   edx
        call   sub_1_37C0
loc_1_8D0F:
        mov    ecx, esi
        call   dword ptr [esi + 118h]
        test   bl, bl
        je     SHORT loc_1_8D60
        mov    byte ptr [edi], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_8D4D
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_8D4D
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 02Dh, 003h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_8D4D:
        cmp    byte ptr [esi + 2ch], 1
        jne    SHORT loc_1_8D60
        mov    byte ptr [esi + 2ch], 0
        push   edi
        call   sub_1_689B
        add    esp, 4
loc_1_8D60:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    10h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_8D70:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        lea    edi, [esi + 2eh]
        cmp    byte ptr [edi], 0
        je     SHORT loc_1_8D83
        xor    bl, bl
        jmp    SHORT loc_1_8DB5
loc_1_8D83:
        mov    ebx, 1
        mov    byte ptr [edi], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_8DB5
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_8DB5
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 056h, 003h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_1_8DB5:
        test   bl, bl
        jne    SHORT loc_1_8DCD
        push   0fh
        mov    eax, dword ptr [esi + 128h]
        push   eax
        call   NDIS_Service_12
        pop    edi
        pop    esi
        pop    ebx
        ret    10h
loc_1_8DCD:
        mov    ecx, esi
        call   dword ptr [esi + 118h]
        test   bl, bl
        je     SHORT loc_1_8E1E
        mov    byte ptr [edi], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_8E0B
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_8E0B
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 07Bh, 003h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_8E0B:
        cmp    byte ptr [esi + 2ch], 1
        jne    SHORT loc_1_8E1E
        mov    byte ptr [esi + 2ch], 0
        push   edi
        call   sub_1_689B
        add    esp, 4
loc_1_8E1E:
        pop    edi
        pop    esi
        pop    ebx
        ret    10h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_8E30:
        mov    edx, dword ptr [esp + 8]
        test   edx, edx
        je     SHORT loc_1_8E67
        mov    eax, dword ptr [esp + 0ch]
        test   eax, eax
        je     SHORT loc_1_8E67
        mov    ecx, dword ptr [edx + 14h]
        cmp    dword ptr [ecx + 60h], 5
        jb     SHORT loc_1_8E56
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], 0c0000001h
        ret    0ch
loc_1_8E56:
        push   eax
        mov    eax, dword ptr [edx + 10h]
        push   eax
        call   dword ptr [edx + 3ch]
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        ret    0ch
loc_1_8E67:
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], 0c0000001h
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_8E80:
        mov    eax, dword ptr [esp + 8]
        test   eax, eax
        je     SHORT loc_1_8E9F
        cmp    dword ptr [eax + 8], 0
        je     SHORT loc_1_8E9F
        mov    ecx, dword ptr [esp + 0ch]
        push   ecx
        push   eax
        mov    eax, dword ptr [esp + 0ch]
        mov    edx, dword ptr [eax + 24h]
        push   edx
        call   dword ptr [eax + 44h]
loc_1_8E9F:
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_8EB0:
        mov    eax, dword ptr [esp + 20h]
        mov    ecx, dword ptr [esp + 1ch]
        mov    edx, dword ptr [esp + 18h]
        push   eax
        mov    eax, dword ptr [esp + 18h]
        push   ecx
        mov    ecx, dword ptr [esp + 18h]
        push   edx
        mov    edx, dword ptr [esp + 18h]
        push   eax
        mov    eax, dword ptr [esp + 18h]
        push   ecx
        push   edx
        mov    ecx, dword ptr [eax + 24h]
        push   ecx
        call   dword ptr [eax + 4ch]
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        ret    20h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_8EF0:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 24h]
        push   ecx
        call   dword ptr [eax + 50h]
        ret    4
        db  08Bh, 0FFh
loc_1_8F00:
        mov    edx, dword ptr [esp + 8]
        test   edx, edx
        je     SHORT loc_1_8F35
        mov    eax, dword ptr [esp + 18h]
        test   eax, eax
        je     SHORT loc_1_8F35
        mov    ecx, dword ptr [esp + 1ch]
        push   ecx
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [esp + 18h]
        push   eax
        push   ecx
        mov    eax, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [edx + 10h]
        push   eax
        push   ecx
        call   dword ptr [edx + 40h]
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        ret    1ch
loc_1_8F35:
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], 0c0000001h
        ret    1ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_8F50:
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        push   eax
        mov    eax, dword ptr [esp + 8]
        push   ecx
        push   edx
        mov    ecx, dword ptr [eax + 24h]
        push   ecx
        call   dword ptr [eax + 48h]
        ret    10h
        db  08Dh, 049h, 000h
sub_1_8F70:
        mov    eax, dword ptr [esp + 14h]
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        push   ebp
        mov    dword ptr [eax], esi
        cmp    dword ptr [esp + 2ch], esi
        je     NEAR PTR loc_1_905C
        lea    eax, [esp + 14h]
        push   esi
        lea    ecx, [esp + 20h]
        push   eax
        mov    edx, dword ptr [esp + 2ch]
        push   ecx
        push   esi
        push   edx
        call   sub_1_6CA0
        cmp    dword ptr [esp + 1ch], esi
        je     NEAR PTR loc_1_905C
        lea    eax, [esp + 10h]
        lea    ecx, [esp + 18h]
        mov    edx, dword ptr [esp + 14h]
        push   eax
        push   ecx
        push   edx
        call   sub_1_6E80
        cmp    dword ptr [esp + 2ch], esi
        je     NEAR PTR loc_1_9056
        mov    edi, dword ptr [esp + 28h]
        mov    ebx, dword ptr [esp + 30h]
loc_1_8FCF:
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_1_9002
        lea    eax, [esp + 14h]
        mov    ecx, dword ptr [esp + 14h]
        push   eax
        push   ecx
        call   sub_1_7150
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_1_9056
        lea    eax, [esp + 10h]
        lea    ecx, [esp + 18h]
        mov    edx, dword ptr [esp + 14h]
        push   eax
        push   ecx
        push   edx
        call   sub_1_6E80
        jmp    SHORT loc_1_904C
loc_1_9002:
        test   edi, edi
        je     SHORT loc_1_9024
        cmp    dword ptr [esp + 10h], edi
        jae    SHORT loc_1_901A
        sub    edi, dword ptr [esp + 10h]
        mov    dword ptr [esp + 10h], 0
        jmp    SHORT loc_1_904C
loc_1_901A:
        add    dword ptr [esp + 18h], edi
        sub    dword ptr [esp + 10h], edi
        xor    edi, edi
loc_1_9024:
        mov    ebp, dword ptr [esp + 2ch]
        mov    eax, dword ptr [esp + 10h]
        sub    ebp, esi
        cmp    ebp, eax
        jb     SHORT loc_1_9034
        mov    ebp, eax
loc_1_9034:
        mov    eax, dword ptr [esp + 18h]
        push   ebp
        push   eax
        add    esi, ebp
        push   ebx
        call   sub_1_64BC
        add    ebx, ebp
        add    dword ptr [esp + 18h], ebp
        sub    dword ptr [esp + 10h], ebp
loc_1_904C:
        cmp    esi, dword ptr [esp + 2ch]
        jb     NEAR PTR loc_1_8FCF
loc_1_9056:
        mov    eax, dword ptr [esp + 34h]
        mov    dword ptr [eax], esi
loc_1_905C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret    14h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_9070:
        sub    esp, 1ch
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [esp + 4], edx
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [eax + 4]
        mov    eax, dword ptr [ecx + 0fch]
        mov    ebp, ecx
        cmp    eax, 7
        ja     NEAR PTR loc_1_93A6
        xor    ecx, ecx
        db  08Ah, 088h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_1_90A1:
        test   byte ptr [ebp + 1a4h], 8
        je     SHORT loc_1_90E9
        test   byte ptr [esi], 1
        je     SHORT loc_1_90BA
        xor    ebx, ebx
        mov    byte ptr [esp + 13h], bl
        jmp    NEAR PTR loc_1_93AA
loc_1_90BA:
        mov    ecx, dword ptr [ebp + 0d8h]
        mov    eax, dword ptr [esi + 2]
        cmp    dword ptr [ecx + 2eh], eax
        jne    SHORT loc_1_90DD
        mov    ax, word ptr [esi]
        cmp    word ptr [ecx + 2ch], ax
        jne    SHORT loc_1_90DD
        mov    byte ptr [esp + 13h], 0
        xor    bl, bl
        jmp    NEAR PTR loc_1_93AA
loc_1_90DD:
        mov    byte ptr [esp + 13h], 0
        mov    bl, 1
        jmp    NEAR PTR loc_1_93AA
loc_1_90E9:
        mov    ecx, dword ptr [ebp + 0d8h]
        xor    ebx, ebx
        mov    al, byte ptr [esi]
        mov    byte ptr [esp + 13h], bl
        mov    edi, dword ptr [ecx + 0ch]
        test   al, 1
        je     SHORT loc_1_9139
        cmp    al, 0ffh
        jne    SHORT loc_1_910E
        test   edi, 8
        je     SHORT loc_1_9153
        mov    bl, 1
        jmp    SHORT loc_1_9153
loc_1_910E:
        test   edi, 4
        jne    SHORT loc_1_9135
        test   edi, 2
        je     SHORT loc_1_9153
        lea    eax, [esp + 20h]
        mov    edx, dword ptr [ecx + 4]
        push   eax
        mov    ecx, dword ptr [ecx + 24h]
        push   esi
        push   edx
        push   ecx
        call   sub_1_51D0
        test   al, al
        je     SHORT loc_1_9153
loc_1_9135:
        mov    bl, 1
        jmp    SHORT loc_1_9153
loc_1_9139:
        mov    eax, dword ptr [esi + 2]
        cmp    dword ptr [ecx + 2eh], eax
        jne    SHORT loc_1_9153
        mov    ax, word ptr [esi]
        cmp    word ptr [ecx + 2ch], ax
        jne    SHORT loc_1_9153
        mov    ebx, 1
        mov    byte ptr [esp + 13h], bl
loc_1_9153:
        test   edi, 0a0h
        je     NEAR PTR loc_1_93AA
        mov    bl, 1
        jmp    NEAR PTR loc_1_93AA
loc_1_9166:
        test   byte ptr [ebp + 1a4h], 8
        je     SHORT loc_1_91B0
        test   byte ptr [esi + 2], 80h
        je     SHORT loc_1_9180
        xor    ebx, ebx
        mov    byte ptr [esp + 13h], bl
        jmp    NEAR PTR loc_1_93AA
loc_1_9180:
        mov    ecx, dword ptr [ebp + 0d8h]
        mov    eax, dword ptr [esi + 4]
        cmp    dword ptr [ecx + 2eh], eax
        jne    SHORT loc_1_91A4
        mov    ax, word ptr [esi + 2]
        cmp    word ptr [ecx + 2ch], ax
        jne    SHORT loc_1_91A4
        mov    byte ptr [esp + 13h], 0
        xor    bl, bl
        jmp    NEAR PTR loc_1_93AA
loc_1_91A4:
        mov    byte ptr [esp + 13h], 0
        mov    bl, 1
        jmp    NEAR PTR loc_1_93AA
loc_1_91B0:
        xor    ebx, ebx
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 6]
        xor    eax, eax
        mov    al, byte ptr [esi + 4]
        mov    byte ptr [esp + 13h], bl
        shl    eax, 10h
        or     ecx, eax
        xor    eax, eax
        shl    ecx, 8
        mov    al, byte ptr [esi + 5]
        shl    eax, 10h
        mov    dl, byte ptr [esi + 2]
        or     ecx, eax
        xor    eax, eax
        mov    al, byte ptr [esi + 7]
        or     ecx, eax
        mov    al, dl
        and    al, byte ptr [esi + 4]
        test   al, 80h
        je     SHORT loc_1_91FD
        mov    eax, dword ptr [ebp + 0dch]
        cmp    dword ptr [eax + 8], ecx
        jne    SHORT loc_1_91FD
        cmp    dword ptr [eax + 0ch], ebx
        je     SHORT loc_1_91FD
        mov    bl, 1
        mov    ecx, dword ptr [esp + 1ch]
        jmp    SHORT loc_1_9263
loc_1_91FD:
        mov    edi, dword ptr [ebp + 0dch]
        test   byte ptr [esi + 8], 80h
        mov    ecx, dword ptr [edi + 10h]
        je     SHORT loc_1_9215
        test   cl, 10h
        je     SHORT loc_1_9215
        mov    bl, 1
        jmp    SHORT loc_1_9263
loc_1_9215:
        test   dl, 80h
        je     SHORT loc_1_924A
        mov    ax, word ptr [esi + 2]
        cmp    ax, 0ffffh
        je     SHORT loc_1_922A
        cmp    ax, 0c0h
        jne    SHORT loc_1_9232
loc_1_922A:
        cmp    dword ptr [esi + 4], -1
        mov    al, 1
        je     SHORT loc_1_9234
loc_1_9232:
        xor    al, al
loc_1_9234:
        test   al, al
        je     SHORT loc_1_9241
        test   cl, 8
        je     SHORT loc_1_9263
        mov    bl, 1
        jmp    SHORT loc_1_9263
loc_1_9241:
        test   ch, 60h
        je     SHORT loc_1_9263
        mov    bl, 1
        jmp    SHORT loc_1_9263
loc_1_924A:
        mov    eax, dword ptr [esi + 4]
        cmp    dword ptr [edi + 1ah], eax
        jne    SHORT loc_1_9263
        mov    ax, word ptr [esi + 2]
        cmp    word ptr [edi + 18h], ax
        jne    SHORT loc_1_9263
        mov    byte ptr [esp + 13h], 0
        mov    bl, 1
loc_1_9263:
        test   cl, 0a0h
        je     NEAR PTR loc_1_93AA
        mov    bl, 1
        jmp    NEAR PTR loc_1_93AA
loc_1_9273:
        test   byte ptr [ebp + 1a4h], 8
        je     SHORT loc_1_92C2
        test   byte ptr [esi + 1], 1
        je     SHORT loc_1_928D
        xor    ebx, ebx
        mov    byte ptr [esp + 13h], bl
        jmp    NEAR PTR loc_1_93AA
loc_1_928D:
        mov    cx, word ptr [esi + 1]
        mov    eax, dword ptr [ebp + 0d8h]
        cmp    word ptr [eax + 2ch], cx
        jne    SHORT loc_1_92B6
        test   byte ptr [esi], 40h
        je     SHORT loc_1_92AA
        mov    eax, dword ptr [eax + 2eh]
        cmp    dword ptr [esi + 3], eax
        jne    SHORT loc_1_92B6
loc_1_92AA:
        mov    byte ptr [esp + 13h], 0
        xor    bl, bl
        jmp    NEAR PTR loc_1_93AA
loc_1_92B6:
        mov    byte ptr [esp + 13h], 0
        mov    bl, 1
        jmp    NEAR PTR loc_1_93AA
loc_1_92C2:
        mov    ecx, dword ptr [ebp + 0e0h]
        xor    ebx, ebx
        mov    al, byte ptr [esi + 1]
        mov    byte ptr [esp + 13h], bl
        mov    edi, dword ptr [ecx + 14h]
        test   al, 1
        je     SHORT loc_1_9349
        cmp    al, 0ffh
        jne    SHORT loc_1_92EF
        test   edi, 8
        je     NEAR PTR loc_1_9387
        mov    bl, 1
        jmp    NEAR PTR loc_1_9387
loc_1_92EF:
        test   edi, 4
        jne    SHORT loc_1_9345
        test   edi, 2
        je     NEAR PTR loc_1_9387
        test   byte ptr [esi], 40h
        je     SHORT loc_1_9327
        lea    eax, [esp + 20h]
        lea    edx, [esi + 1]
        push   eax
        push   edx
        mov    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ecx + 30h]
        push   eax
        push   ecx
        call   sub_1_6130
        test   al, al
        jne    SHORT loc_1_9345
        test   byte ptr [esi], 40h
        jne    SHORT loc_1_9387
loc_1_9327:
        mov    ecx, dword ptr [ebp + 0e0h]
        lea    eax, [esp + 20h]
        push   eax
        inc    esi
        push   esi
        mov    edx, dword ptr [ecx + 0ch]
        push   edx
        mov    eax, dword ptr [ecx + 34h]
        push   eax
        call   sub_1_61D0
        test   al, al
        je     SHORT loc_1_9387
loc_1_9345:
        mov    bl, 1
        jmp    SHORT loc_1_9387
loc_1_9349:
        test   byte ptr [esi], 40h
        je     SHORT loc_1_936B
        mov    ax, word ptr [esi + 1]
        cmp    word ptr [ecx + 3ch], ax
        jne    SHORT loc_1_9364
        mov    eax, dword ptr [ecx + 3eh]
        cmp    dword ptr [esi + 3], eax
        jne    SHORT loc_1_9364
        xor    eax, eax
        jmp    SHORT loc_1_937A
loc_1_9364:
        mov    eax, 1
        jmp    SHORT loc_1_937A
loc_1_936B:
        mov    ax, word ptr [ecx + 42h]
        sub    ax, word ptr [esi + 1]
        cmp    ax, 1
        sbb    eax, eax
        inc    eax
loc_1_937A:
        test   eax, eax
        jne    SHORT loc_1_9387
        mov    ebx, 1
        mov    byte ptr [esp + 13h], bl
loc_1_9387:
        test   edi, 0a0h
        je     SHORT loc_1_93AA
        mov    bl, 1
        jmp    SHORT loc_1_93AA
loc_1_9393:
        mov    edx, dword ptr [esp + 14h]
        mov    ecx, ebp
        call   sub_1_4990
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_1_93A6:
        mov    bl, byte ptr [esp + 1ch]
loc_1_93AA:
        test   bl, bl
        je     NEAR PTR loc_1_94A1
        lea    eax, [esp + 18h]
        mov    ecx, dword ptr [esp + 14h]
        push   eax
        push   0
        push   0
        push   0
        push   ecx
        call   sub_1_6CA0
        mov    eax, dword ptr [esp + 18h]
        push   0
        add    eax, 79h
        push   eax
        call   sub_1_6563
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        je     NEAR PTR loc_1_9495
        lea    ebx, [esi + 79h]
        mov    edi, esi
        xor    eax, eax
        mov    ecx, 1eh
        rep stosd
        stosb
        mov    eax, dword ptr [esp + 18h]
        lea    ecx, [esp + 24h]
        lea    edx, [esp + 28h]
        push   eax
        push   ebx
        push   0
        push   ecx
        push   edx
        call   sub_1_6D50
        cmp    dword ptr [esp + 28h], 0
        jne    SHORT loc_1_943D
        mov    eax, dword ptr [esp + 24h]
        lea    ecx, [esp + 1ch]
        push   ecx
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [esp + 28h]
        push   ebx
        mov    dword ptr [esi + 0ch], eax
        mov    eax, dword ptr [esp + 1ch]
        mov    word ptr [esi + 1eh], 49h
        mov    edx, dword ptr [esp + 20h]
        push   edx
        push   0
        push   eax
        call   sub_1_8F70
        jmp    SHORT loc_1_944A
loc_1_943D:
        push   0
        push   esi
        xor    esi, esi
        call   sub_1_65A5
        add    esp, 8
loc_1_944A:
        test   esi, esi
        je     SHORT loc_1_9495
        mov    eax, dword ptr [esp + 14h]
        or     byte ptr [eax + 1dh], 1
        cmp    dword ptr [ebp + 1b8h], 0
        jne    SHORT loc_1_9475
        push   0fh
        mov    eax, dword ptr [ebp + 128h]
        push   eax
        mov    dword ptr [ebp + 1b8h], esi
        call   NDIS_Service_12
        jmp    SHORT loc_1_947E
loc_1_9475:
        mov    eax, dword ptr [ebp + 1bch]
        mov    dword ptr [eax + 2ch], esi
loc_1_947E:
        push   0
        mov    edx, 5
        push   0
        mov    ecx, ebp
        mov    dword ptr [ebp + 1bch], esi
        call   dword ptr [ebp + 11ch]
loc_1_9495:
        mov    al, byte ptr [esp + 13h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_1_94A1:
        xor    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  090h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 001h, 002h, 004h, 004h, 004h, 004h, 003h, 08Dh, 064h, 024h
        db  000h, 08Dh, 064h, 024h, 000h
sub_1_94D0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        push   ebp
        xor    ebx, ebx
        mov    edi, dword ptr [esi + 1b8h]
        mov    dword ptr [esi + 1bch], ebx
        mov    dword ptr [esi + 1b8h], ebx
        test   edi, edi
        je     NEAR PTR loc_1_95B4
        mov    ebp, 0c000009ah
loc_1_94FA:
        xor    eax, eax
        mov    dword ptr [esp + 10h], edi
        mov    ax, word ptr [edi + 1eh]
        mov    dword ptr [eax + edi + 1ch], ebp
        lea    edx, [eax + edi]
        mov    ecx, dword ptr [esp + 10h]
        mov    edi, dword ptr [ecx + 2ch]
        lea    eax, [ecx + 79h]
        mov    dword ptr [esi + 1d4h], ecx
        mov    ecx, dword ptr [esi + 0fch]
        test   ecx, ecx
        je     SHORT loc_1_9531
        cmp    ecx, 1
        je     SHORT loc_1_9547
        cmp    ecx, 2
        je     SHORT loc_1_9572
        jmp    SHORT loc_1_9590
loc_1_9531:
        lea    eax, [esp + 10h]
        push   1
        mov    dword ptr [edx + 10h], 0eh
        push   eax
        push   esi
        call   sub_1_4E50
        jmp    SHORT loc_1_9590
loc_1_9547:
        mov    dword ptr [edx + 10h], 0eh
        test   byte ptr [eax + 8], 80h
        je     SHORT loc_1_9563
        mov    al, byte ptr [eax + 0eh]
        xor    ecx, ecx
        and    al, 1fh
        mov    cl, al
        add    ecx, 0eh
        mov    dword ptr [edx + 10h], ecx
loc_1_9563:
        lea    eax, [esp + 10h]
        push   1
        push   eax
        push   esi
        call   sub_1_5560
        jmp    SHORT loc_1_9590
loc_1_9572:
        mov    al, byte ptr [eax]
        push   1
        and    al, 40h
        lea    ecx, [esp + 14h]
        push   ecx
        cmp    al, 1
        sbb    eax, eax
        push   esi
        and    eax, 0fffffff8h
        add    eax, 0dh
        mov    dword ptr [edx + 10h], eax
        call   sub_1_5CD0
loc_1_9590:
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [eax + 8]
        push   ecx
        call   sub_1_6E40
        mov    ecx, dword ptr [esp + 10h]
        push   0
        push   ecx
        call   sub_1_65A5
        add    esp, 8
        test   edi, edi
        jne    NEAR PTR loc_1_94FA
loc_1_95B4:
        mov    dword ptr [esi + 1d4h], 0
        cmp    dword ptr [esi + 1b8h], 0
        je     SHORT loc_1_95C9
        mov    bl, 1
loc_1_95C9:
        mov    al, bl
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_95E0:
        xor    al, al
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_95F0:
        xor    al, al
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_9600:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        push   ebp
        and    dword ptr [esi + 1a4h], 0fffffffeh
        cmp    dword ptr [esi + 1c0h], 0
        je     NEAR PTR loc_1_98FB
loc_1_961D:
        cmp    dword ptr [esi + 1b0h], 0
        je     NEAR PTR loc_1_98FB
        mov    dword ptr [esp + 18h], 0
        mov    dword ptr [esp + 14h], 0
        cmp    dword ptr [esi + 1c8h], 0
        mov    eax, dword ptr [esi + 1c4h]
        mov    dword ptr [esp + 10h], eax
        jbe    NEAR PTR loc_1_974F
loc_1_9651:
        mov    edi, dword ptr [esi + 1b0h]
        test   edi, edi
        je     NEAR PTR loc_1_974F
        mov    eax, dword ptr [edi + 2ch]
        mov    ebp, dword ptr [esi + 1b4h]
        cmp    dword ptr [esp + 18h], 0
        mov    dword ptr [esi + 1b0h], eax
        jne    SHORT loc_1_9679
        mov    dword ptr [esp + 18h], ebp
loc_1_9679:
        test   byte ptr [esi + 1f8h], 8
        mov    dword ptr [esi + 1b4h], edi
        jne    SHORT loc_1_9695
        test   byte ptr [esi + 1a4h], 8
        je     NEAR PTR loc_1_9722
loc_1_9695:
        test   byte ptr [edi + 1dh], 1
        jne    NEAR PTR loc_1_9722
        mov    edx, edi
        mov    ecx, esi
        call   sub_1_9070
        test   al, al
        je     SHORT loc_1_9722
        mov    ebx, dword ptr [edi + 30h]
        mov    eax, dword ptr [esi + 1a4h]
        mov    dword ptr [esi + 1c0h], 0ffffffh
        test   al, 1
        je     SHORT loc_1_96E1
        and    eax, 0fffffffeh
        xor    ebp, ebp
        mov    dword ptr [esi + 1a4h], eax
        mov    eax, dword ptr [esi + 1a8h]
loc_1_96D4:
        cmp    edi, eax
        je     SHORT loc_1_96E1
        mov    ebp, eax
        mov    eax, dword ptr [eax + 2ch]
        test   eax, eax
        jne    SHORT loc_1_96D4
loc_1_96E1:
        mov    dword ptr [esi + 1b4h], ebp
        test   ebp, ebp
        mov    eax, dword ptr [edi + 2ch]
        jne    SHORT loc_1_96F6
        mov    dword ptr [esi + 1a8h], eax
        jmp    SHORT loc_1_9707
loc_1_96F6:
        mov    dword ptr [ebp + 2ch], eax
        cmp    dword ptr [esi + 1ach], edi
        jne    SHORT loc_1_9707
        mov    dword ptr [esi + 1ach], ebp
loc_1_9707:
        push   0
        mov    eax, dword ptr [ebx + 10h]
        push   edi
        mov    ebp, dword ptr [ebx + 8]
        push   eax
        call   dword ptr [ebp + 44h]
        dec    dword ptr [ebx + 34h]
        jne    SHORT loc_1_973F
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_9720:
        jmp    SHORT loc_1_973F
loc_1_9722:
        mov    eax, dword ptr [esp + 10h]
        inc    dword ptr [esp + 14h]
        mov    dword ptr [eax], edi
        xor    eax, eax
        add    dword ptr [esp + 10h], 4
        mov    ax, word ptr [edi + 1eh]
        mov    dword ptr [eax + edi + 1ch], 0
loc_1_973F:
        mov    eax, dword ptr [esp + 14h]
        cmp    dword ptr [esi + 1c8h], eax
        ja     NEAR PTR loc_1_9651
loc_1_974F:
        cmp    dword ptr [esp + 14h], 0
        je     NEAR PTR loc_1_98EE
        mov    edi, dword ptr [esi + 1c4h]
        mov    eax, dword ptr [esp + 14h]
        push   eax
        mov    edx, dword ptr [esi + 0ch]
        mov    ecx, dword ptr [edi]
        push   edi
        push   edx
        mov    ebx, dword ptr [ecx + 30h]
        call   dword ptr [ebx + 5ch]
        mov    edx, dword ptr [esi + 1a8h]
        cmp    dword ptr [edi], edx
        je     SHORT loc_1_97A4
        mov    eax, dword ptr [esi + 1a4h]
        test   al, 1
        je     SHORT loc_1_97A4
        and    eax, 0fffffffeh
        xor    ecx, ecx
        mov    dword ptr [esi + 1a4h], eax
        mov    eax, dword ptr [edi]
loc_1_9793:
        cmp    eax, edx
        je     SHORT loc_1_97A0
        mov    ecx, edx
        mov    edx, dword ptr [edx + 2ch]
        test   edx, edx
        jne    SHORT loc_1_9793
loc_1_97A0:
        mov    dword ptr [esp + 18h], ecx
loc_1_97A4:
        mov    eax, dword ptr [esp + 18h]
        xor    ebx, ebx
        mov    dword ptr [esp + 10h], eax
        cmp    dword ptr [esp + 14h], ebx
        je     NEAR PTR loc_1_98EE
loc_1_97B8:
        mov    edx, dword ptr [edi]
        xor    eax, eax
        mov    ax, word ptr [edx + 1eh]
        mov    ecx, dword ptr [eax + edx + 1ch]
        cmp    ecx, 103h
        jne    SHORT loc_1_97D9
        and    byte ptr [edx + 1dh], 0fdh
        mov    dword ptr [esp + 10h], edx
        jmp    NEAR PTR loc_1_9867
loc_1_97D9:
        cmp    ecx, 0c000009ah
        je     NEAR PTR loc_1_9877
        mov    dword ptr [esi + 1c0h], 0ffffffh
        mov    eax, dword ptr [edi]
        cmp    dword ptr [esi + 1a8h], eax
        jne    SHORT loc_1_9818
        mov    eax, dword ptr [eax + 2ch]
        mov    dword ptr [esi + 1a8h], eax
        mov    edx, dword ptr [edi]
        cmp    dword ptr [esi + 1b4h], edx
        jne    SHORT loc_1_9846
        mov    dword ptr [esi + 1b4h], 0
        jmp    SHORT loc_1_9846
loc_1_9818:
        mov    edx, dword ptr [eax + 2ch]
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 2ch], edx
        mov    ebp, dword ptr [edi]
        cmp    dword ptr [esi + 1ach], ebp
        jne    SHORT loc_1_9832
        mov    dword ptr [esi + 1ach], eax
loc_1_9832:
        mov    eax, dword ptr [edi]
        cmp    dword ptr [esi + 1b4h], eax
        jne    SHORT loc_1_9846
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esi + 1b4h], eax
loc_1_9846:
        mov    eax, dword ptr [edi]
        push   ecx
        push   eax
        mov    ebp, dword ptr [eax + 30h]
        mov    ecx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 8]
        push   ecx
        call   dword ptr [eax + 44h]
        mov    eax, dword ptr [ebp + 34h]
        dec    eax
        mov    dword ptr [ebp + 34h], eax
        jne    SHORT loc_1_9867
        push   ebp
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_9867:
        inc    ebx
        add    edi, 4
        cmp    dword ptr [esp + 14h], ebx
        ja     NEAR PTR loc_1_97B8
        jmp    SHORT loc_1_9888
loc_1_9877:
        mov    dword ptr [esi + 1c0h], 0
        and    dword ptr [esi + 1a4h], 0fffffffeh
loc_1_9888:
        cmp    dword ptr [esp + 14h], ebx
        je     SHORT loc_1_98EE
        mov    edi, dword ptr [esp + 14h]
        lea    edx, [edi*4 - 4]
        dec    edi
        add    edx, dword ptr [esi + 1c4h]
        cmp    edi, ebx
        jbe    SHORT loc_1_98BE
        sub    edi, ebx
loc_1_98A6:
        lea    ecx, [edx - 4]
        dec    edi
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 1b4h], eax
        mov    edx, dword ptr [edx]
        mov    dword ptr [esi + 1b0h], edx
        mov    edx, ecx
        jne    SHORT loc_1_98A6
loc_1_98BE:
        mov    eax, dword ptr [edx]
        cmp    dword ptr [esi + 1a8h], eax
        jne    SHORT loc_1_98DC
        mov    dword ptr [esi + 1b4h], 0
        mov    eax, dword ptr [edx]
        mov    dword ptr [esi + 1b0h], eax
        jmp    SHORT loc_1_98EE
loc_1_98DC:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esi + 1b4h], eax
        mov    ecx, dword ptr [edx]
        mov    dword ptr [esi + 1b0h], ecx
loc_1_98EE:
        cmp    dword ptr [esi + 1c0h], 0
        jne    NEAR PTR loc_1_961D
loc_1_98FB:
        xor    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_9910:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        push   ebp
        and    dword ptr [esi + 1a4h], 0fffffffeh
        cmp    dword ptr [esi + 1c0h], 0
        je     NEAR PTR loc_1_9AAB
loc_1_992D:
        mov    ebx, dword ptr [esi + 1b0h]
        test   ebx, ebx
        je     NEAR PTR loc_1_9AA2
        cmp    dword ptr [esi + 0fch], 7
        mov    edi, dword ptr [ebx + 30h]
        jne    SHORT loc_1_995A
        push   ebx
        push   edi
        push   esi
        call   sub_1_47E0
        cmp    eax, 103h
        je     NEAR PTR loc_1_9ABE
loc_1_995A:
        mov    eax, dword ptr [ebx + 2ch]
        mov    ebp, dword ptr [esi + 1b4h]
        mov    dword ptr [esi + 1b0h], eax
        mov    dword ptr [esi + 1b4h], ebx
        test   byte ptr [esi + 1f8h], 8
        jne    SHORT loc_1_9981
        test   byte ptr [esi + 1a4h], 8
        je     SHORT loc_1_999E
loc_1_9981:
        test   byte ptr [ebx + 1dh], 1
        jne    SHORT loc_1_999E
        mov    edx, ebx
        mov    ecx, esi
        call   sub_1_9070
        test   al, al
        je     SHORT loc_1_999E
        mov    dword ptr [esp + 10h], 0
        jmp    SHORT loc_1_99B4
loc_1_999E:
        dec    dword ptr [esi + 1c0h]
        mov    eax, dword ptr [ebx + 18h]
        mov    ecx, dword ptr [edi + 14h]
        push   eax
        push   ebx
        push   ecx
        call   dword ptr [edi + 40h]
        mov    dword ptr [esp + 10h], eax
loc_1_99B4:
        cmp    dword ptr [esi + 0fch], 7
        jne    SHORT loc_1_99C4
        push   ebx
        push   esi
        call   sub_1_4900
loc_1_99C4:
        cmp    dword ptr [esp + 10h], 103h
        jne    SHORT loc_1_99D7
        and    byte ptr [ebx + 1dh], 0fdh
        jmp    NEAR PTR loc_1_9A95
loc_1_99D7:
        cmp    dword ptr [esp + 10h], 0c000009ah
        je     SHORT loc_1_9A57
        mov    dword ptr [esi + 1c0h], 0ffffffh
        mov    eax, dword ptr [esi + 1a4h]
        test   al, 1
        je     SHORT loc_1_9A13
        and    eax, 0fffffffeh
        xor    ebp, ebp
        mov    dword ptr [esi + 1a4h], eax
        mov    eax, dword ptr [esi + 1a8h]
loc_1_9A06:
        cmp    ebx, eax
        je     SHORT loc_1_9A13
        mov    ebp, eax
        mov    eax, dword ptr [eax + 2ch]
        test   eax, eax
        jne    SHORT loc_1_9A06
loc_1_9A13:
        mov    dword ptr [esi + 1b4h], ebp
        test   ebp, ebp
        mov    eax, dword ptr [ebx + 2ch]
        jne    SHORT loc_1_9A28
        mov    dword ptr [esi + 1a8h], eax
        jmp    SHORT loc_1_9A39
loc_1_9A28:
        mov    dword ptr [ebp + 2ch], eax
        cmp    dword ptr [esi + 1ach], ebx
        jne    SHORT loc_1_9A39
        mov    dword ptr [esi + 1ach], ebp
loc_1_9A39:
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [edi + 10h]
        push   eax
        push   ebx
        push   ecx
        mov    ebx, dword ptr [edi + 8]
        call   dword ptr [ebx + 44h]
        dec    dword ptr [edi + 34h]
        jne    SHORT loc_1_9A95
        push   edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_9A55:
        jmp    SHORT loc_1_9A95
loc_1_9A57:
        mov    eax, dword ptr [esi + 1a4h]
        test   al, 1
        je     SHORT loc_1_9A7F
        and    eax, 0fffffffeh
        xor    ebp, ebp
        mov    dword ptr [esi + 1a4h], eax
        mov    eax, dword ptr [esi + 1a8h]
loc_1_9A72:
        cmp    ebx, eax
        je     SHORT loc_1_9A7F
        mov    ebp, eax
        mov    eax, dword ptr [eax + 2ch]
        test   eax, eax
        jne    SHORT loc_1_9A72
loc_1_9A7F:
        mov    dword ptr [esi + 1b4h], ebp
        mov    dword ptr [esi + 1b0h], ebx
        mov    dword ptr [esi + 1c0h], 0
loc_1_9A95:
        cmp    dword ptr [esi + 1c0h], 0
        jne    NEAR PTR loc_1_992D
loc_1_9AA2:
        cmp    dword ptr [esi + 1c0h], 0
        jne    SHORT loc_1_9AC8
loc_1_9AAB:
        cmp    dword ptr [esi + 1b0h], 0
        je     SHORT loc_1_9AC8
        mov    al, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_9ABE:
        mov    al, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_9AC8:
        xor    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_9AE0:
        sub    esp, 8
        push   ebx
        push   esi
        lea    eax, [edx + 2ch]
        push   edi
        push   ebp
        mov    ebx, edx
        mov    ebp, dword ptr [ecx + 1b4h]
        mov    edi, ecx
        mov    esi, dword ptr [eax + 4]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esp + 14h], eax
        mov    dword ptr [edi + 1b0h], ecx
        mov    eax, dword ptr [edi + 1a4h]
        mov    dword ptr [edi + 1b4h], ebx
        and    eax, 0fffffffeh
        test   byte ptr [edi + 1f8h], 8
        mov    dword ptr [edi + 1a4h], eax
        jne    SHORT loc_1_9B25
        test   al, 8
        je     SHORT loc_1_9B42
loc_1_9B25:
        test   byte ptr [ebx + 1dh], 1
        jne    SHORT loc_1_9B42
        mov    edx, ebx
        mov    ecx, edi
        call   sub_1_9070
        test   al, al
        je     SHORT loc_1_9B42
        mov    dword ptr [esp + 10h], 0
        jmp    SHORT loc_1_9B58
loc_1_9B42:
        dec    dword ptr [edi + 1c0h]
        mov    eax, dword ptr [ebx + 18h]
        mov    ecx, dword ptr [esi + 14h]
        push   eax
        push   ebx
        push   ecx
        call   dword ptr [esi + 40h]
        mov    dword ptr [esp + 10h], eax
loc_1_9B58:
        cmp    dword ptr [esp + 10h], 103h
        jne    SHORT loc_1_9B73
        mov    eax, 103h
        pop    ebp
        and    byte ptr [ebx + 1dh], 0fdh
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_9B73:
        mov    eax, dword ptr [edi + 1a4h]
        test   al, 1
        je     SHORT loc_1_9B9B
        and    eax, 0fffffffeh
        xor    ebp, ebp
        mov    dword ptr [edi + 1a4h], eax
        mov    eax, dword ptr [edi + 1a8h]
loc_1_9B8E:
        cmp    ebx, eax
        je     SHORT loc_1_9B9B
        mov    ebp, eax
        mov    eax, dword ptr [eax + 2ch]
        test   eax, eax
        jne    SHORT loc_1_9B8E
loc_1_9B9B:
        cmp    dword ptr [esp + 10h], 0c000009ah
        mov    dword ptr [edi + 1b4h], ebp
        je     SHORT loc_1_9BEA
        mov    eax, dword ptr [esp + 14h]
        test   ebp, ebp
        mov    dword ptr [edi + 1c0h], 0ffffffh
        mov    ecx, dword ptr [eax]
        jne    SHORT loc_1_9BC7
        mov    dword ptr [edi + 1a8h], ecx
        jmp    SHORT loc_1_9BD8
loc_1_9BC7:
        mov    dword ptr [ebp + 2ch], ecx
        cmp    dword ptr [edi + 1ach], ebx
        jne    SHORT loc_1_9BD8
        mov    dword ptr [edi + 1ach], ebp
loc_1_9BD8:
        mov    eax, dword ptr [esi + 34h]
        dec    eax
        mov    dword ptr [esi + 34h], eax
        jne    SHORT loc_1_9C02
        push   esi
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_9BE8:
        jmp    SHORT loc_1_9C02
loc_1_9BEA:
        mov    dword ptr [edi + 1c0h], 0
        mov    dword ptr [esp + 10h], 103h
        mov    dword ptr [edi + 1b0h], ebx
loc_1_9C02:
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_9C10:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [eax + 4]
        lea    esi, [edi + 2eh]
        cmp    byte ptr [esi], 0
        je     SHORT loc_1_9C27
        xor    bl, bl
        jmp    SHORT loc_1_9C59
loc_1_9C27:
        mov    ebx, 1
        mov    byte ptr [esi], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_9C59
        db  03Bh, 03Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_9C59
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0E9h, 006h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_1_9C59:
        mov    eax, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [esp + 18h]
        push   eax
        mov    edx, dword ptr [edi + 0ch]
        push   ecx
        mov    eax, dword ptr [edi + 8]
        push   edx
        call   dword ptr [eax + 48h]
        mov    ebp, eax
        test   bl, bl
        je     SHORT loc_1_9CC4
        mov    ecx, edi
        call   dword ptr [edi + 118h]
        test   bl, bl
        je     SHORT loc_1_9CC4
        mov    byte ptr [esi], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_9CB1
        db  03Bh, 03Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_9CB1
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0FBh, 006h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_9CB1:
        cmp    byte ptr [edi + 2ch], 1
        jne    SHORT loc_1_9CC4
        mov    byte ptr [edi + 2ch], 0
        push   esi
        call   sub_1_689B
        add    esp, 4
loc_1_9CC4:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
        db  08Dh, 049h, 000h, 0C2h, 00Ch, 000h, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h
        db  000h, 000h, 000h
loc_1_9CE0:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        mov    ebx, dword ptr [esp + 14h]
        or     dword ptr [esi + 1a4h], 1
        lea    edx, [ebx + 2ch]
        mov    eax, dword ptr [esi + 1a8h]
        cmp    ebx, eax
        jne    SHORT loc_1_9D1B
        mov    eax, dword ptr [edx]
        mov    dword ptr [esi + 1a8h], eax
        cmp    dword ptr [esi + 1b4h], ebx
        jne    SHORT loc_1_9D4B
        mov    dword ptr [esi + 1b4h], 0
        jmp    SHORT loc_1_9D4B
loc_1_9D1B:
        xor    ecx, ecx
loc_1_9D1D:
        cmp    ebx, eax
        je     SHORT loc_1_9D2A
        mov    ecx, eax
        mov    eax, dword ptr [eax + 2ch]
        test   eax, eax
        jne    SHORT loc_1_9D1D
loc_1_9D2A:
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx + 2ch], eax
        cmp    dword ptr [esi + 1ach], ebx
        jne    SHORT loc_1_9D3D
        mov    dword ptr [esi + 1ach], ecx
loc_1_9D3D:
        cmp    dword ptr [esi + 1b4h], ebx
        jne    SHORT loc_1_9D4B
        mov    dword ptr [esi + 1b4h], ecx
loc_1_9D4B:
        cmp    dword ptr [esi + 0fch], 7
        mov    edi, dword ptr [edx + 4]
        jne    SHORT loc_1_9D5E
        push   ebx
        push   esi
        call   sub_1_4900
loc_1_9D5E:
        mov    eax, dword ptr [esp + 18h]
        mov    ecx, dword ptr [edi + 10h]
        push   eax
        push   ebx
        push   ecx
        mov    ebx, dword ptr [edi + 8]
        call   dword ptr [ebx + 44h]
        mov    dword ptr [esi + 1c0h], 0ffffffh
        mov    eax, dword ptr [edi + 34h]
        dec    eax
        mov    dword ptr [edi + 34h], eax
        jne    SHORT loc_1_9D88
        push   edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_9D88:
        cmp    dword ptr [esi + 1b0h], 0
        je     SHORT loc_1_9DA2
        push   0
        mov    edx, 3
        push   0
        mov    ecx, esi
        call   dword ptr [esi + 11ch]
loc_1_9DA2:
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_9DB0:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [eax + 18h]
        test   ebx, ebx
        je     SHORT loc_1_9DD9
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 18h]
loc_1_9DC6:
        push   edi
        mov    eax, dword ptr [ebx + 10h]
        push   esi
        push   eax
        mov    eax, dword ptr [ebx + 8]
        call   dword ptr [eax + 44h]
        mov    ebx, dword ptr [ebx + 18h]
        test   ebx, ebx
        jne    SHORT loc_1_9DC6
loc_1_9DD9:
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
        db  090h, 0C2h, 004h, 000h, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h
        db  000h
loc_1_9DF0:
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx + 1c0h], 0ffffffh
        cmp    dword ptr [ecx + 1b0h], 0
        je     SHORT loc_1_9E16
        push   0
        mov    edx, 3
        push   0
        call   dword ptr [ecx + 11ch]
loc_1_9E16:
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_9E20:
        mov    eax, 103h
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_9E30:
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_9E40:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        mov    edx, dword ptr [esp + 0ch]
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 4]
        lea    ecx, [edx + 2ch]
        inc    dword ptr [eax + 34h]
        mov    dword ptr [ecx], 0
        mov    dword ptr [ecx + 4], eax
        cmp    dword ptr [esi + 1a8h], 0
        jne    SHORT loc_1_9E6E
        mov    dword ptr [esi + 1a8h], edx
        jmp    SHORT loc_1_9E77
loc_1_9E6E:
        mov    eax, dword ptr [esi + 1ach]
        mov    dword ptr [eax + 2ch], edx
loc_1_9E77:
        mov    dword ptr [esi + 1ach], edx
        and    byte ptr [edx + 1dh], 0feh
        cmp    dword ptr [esi + 1b0h], 0
        jne    SHORT loc_1_9E90
        mov    dword ptr [esi + 1b0h], edx
loc_1_9E90:
        push   0
        mov    edx, 3
        push   0
        mov    ecx, esi
        lea    edi, [esi + 2eh]
        call   dword ptr [esi + 11ch]
        cmp    byte ptr [edi], 0
        je     SHORT loc_1_9EAD
        xor    bl, bl
        jmp    SHORT loc_1_9EDF
loc_1_9EAD:
        mov    ebx, 1
        mov    byte ptr [edi], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_9EDF
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_9EDF
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 078h, 009h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_1_9EDF:
        test   bl, bl
        je     SHORT loc_1_9F34
        mov    ecx, esi
        call   dword ptr [esi + 118h]
        test   bl, bl
        je     SHORT loc_1_9F34
        mov    byte ptr [edi], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_9F21
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_9F21
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 086h, 009h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_9F21:
        cmp    byte ptr [esi + 2ch], 1
        jne    SHORT loc_1_9F34
        mov    byte ptr [esi + 2ch], 0
        push   edi
        call   sub_1_689B
        add    esp, 4
loc_1_9F34:
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        ret    8
        db  090h
loc_1_9F40:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        mov    ecx, dword ptr [esp + 0ch]
        push   esi
        mov    edx, dword ptr [esp + 14h]
        push   edi
        push   ebp
        mov    edi, dword ptr [eax + 10h]
        test   edx, edx
        mov    esi, dword ptr [edi + 4]
        je     SHORT loc_1_9FBE
        mov    ebx, 103h
loc_1_9F5F:
        mov    eax, dword ptr [ecx]
        xor    ebp, ebp
        mov    bp, word ptr [eax + 1eh]
        mov    dword ptr [ebp + eax + 1ch], ebx
        mov    eax, dword ptr [ecx]
        mov    dword ptr [eax + 2ch], 0
        mov    dword ptr [eax + 30h], edi
        add    eax, 2ch
        inc    dword ptr [edi + 34h]
        cmp    dword ptr [esi + 1a8h], 0
        mov    eax, dword ptr [ecx]
        jne    SHORT loc_1_9F90
        mov    dword ptr [esi + 1a8h], eax
        jmp    SHORT loc_1_9F99
loc_1_9F90:
        mov    ebp, dword ptr [esi + 1ach]
        mov    dword ptr [ebp + 2ch], eax
loc_1_9F99:
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 1ach], eax
        mov    eax, dword ptr [ecx]
        and    byte ptr [eax + 1dh], 0feh
        cmp    dword ptr [esi + 1b0h], 0
        jne    SHORT loc_1_9FB8
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 1b0h], eax
loc_1_9FB8:
        add    ecx, 4
        dec    edx
        jne    SHORT loc_1_9F5F
loc_1_9FBE:
        push   0
        mov    edx, 3
        push   0
        mov    ecx, esi
        lea    edi, [esi + 2eh]
        call   dword ptr [esi + 11ch]
        cmp    byte ptr [edi], 0
        je     SHORT loc_1_9FDB
        xor    bl, bl
        jmp    SHORT loc_1_A00D
loc_1_9FDB:
        mov    ebx, 1
        mov    byte ptr [edi], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_A00D
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_A00D
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0EBh, 009h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_1_A00D:
        test   bl, bl
        je     SHORT loc_1_A062
        mov    ecx, esi
        call   dword ptr [esi + 118h]
        test   bl, bl
        je     SHORT loc_1_A062
        mov    byte ptr [edi], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_A04F
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_A04F
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0F9h, 009h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_A04F:
        cmp    byte ptr [esi + 2ch], 1
        jne    SHORT loc_1_A062
        mov    byte ptr [esi + 2ch], 0
        push   edi
        call   sub_1_689B
        add    esp, 4
loc_1_A062:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_A070:
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_A080:
        mov    eax, 103h
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_A090:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [eax + 10h]
        mov    esi, dword ptr [edi + 4]
        cmp    byte ptr [esi + 2eh], 0
        je     SHORT loc_1_A0A8
        xor    bl, bl
        jmp    SHORT loc_1_A0DB
loc_1_A0A8:
        mov    ebx, 1
        mov    byte ptr [esi + 2eh], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_A0DB
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_A0DB
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 05Fh, 00Bh, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_1_A0DB:
        mov    ecx, dword ptr [esp + 18h]
        mov    edx, dword ptr [esp + 1ch]
        test   edx, edx
        je     SHORT loc_1_A13A
loc_1_A0E7:
        mov    eax, dword ptr [ecx]
        mov    dword ptr [eax + 2ch], 0
        mov    dword ptr [eax + 30h], edi
        add    eax, 2ch
        inc    dword ptr [edi + 34h]
        cmp    dword ptr [esi + 1a8h], 0
        mov    eax, dword ptr [ecx]
        jne    SHORT loc_1_A10C
        mov    dword ptr [esi + 1a8h], eax
        jmp    SHORT loc_1_A115
loc_1_A10C:
        mov    ebp, dword ptr [esi + 1ach]
        mov    dword ptr [ebp + 2ch], eax
loc_1_A115:
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 1ach], eax
        mov    eax, dword ptr [ecx]
        and    byte ptr [eax + 1dh], 0feh
        cmp    dword ptr [esi + 1b0h], 0
        jne    SHORT loc_1_A134
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 1b0h], eax
loc_1_A134:
        add    ecx, 4
        dec    edx
        jne    SHORT loc_1_A0E7
loc_1_A13A:
        push   0
        mov    edx, 3
        push   0
        mov    ecx, esi
        call   dword ptr [esi + 11ch]
        test   bl, bl
        je     SHORT loc_1_A1A4
        mov    ecx, esi
        call   dword ptr [esi + 118h]
        test   bl, bl
        je     SHORT loc_1_A1A4
        mov    byte ptr [esi + 2eh], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_A18E
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_A18E
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0A9h, 00Bh, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_A18E:
        cmp    byte ptr [esi + 2ch], 1
        jne    SHORT loc_1_A1A4
        mov    byte ptr [esi + 2ch], 0
        add    esi, 2eh
        push   esi
        call   sub_1_689B
        add    esp, 4
loc_1_A1A4:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h
loc_1_A1B0:
        mov    ecx, dword ptr [esp + 4]
        push   ebx
        push   esi
        xor    eax, eax
        push   edi
        mov    esi, 103h
        push   ebp
        mov    edi, dword ptr [ecx + 4]
        cmp    byte ptr [edi + 2eh], al
        je     SHORT loc_1_A1CB
        xor    bl, bl
        jmp    SHORT loc_1_A1FE
loc_1_A1CB:
        mov    ebx, 1
        mov    byte ptr [edi + 2eh], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_A1FE
        db  03Bh, 03Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_A1FE
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0C8h, 00Bh, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_1_A1FE:
        mov    edx, dword ptr [esp + 18h]
        inc    dword ptr [ecx + 34h]
        lea    ebp, [edx + 2ch]
        mov    dword ptr [ebp], 0
        mov    dword ptr [ebp + 4], ecx
        cmp    dword ptr [edi + 1a8h], 0
        jne    SHORT loc_1_A223
        mov    dword ptr [edi + 1a8h], edx
        jmp    SHORT loc_1_A22C
loc_1_A223:
        mov    ecx, dword ptr [edi + 1ach]
        mov    dword ptr [ecx + 2ch], edx
loc_1_A22C:
        mov    dword ptr [edi + 1ach], edx
        and    byte ptr [edx + 1dh], 0feh
        cmp    dword ptr [edi + 1b0h], 0
        jne    SHORT loc_1_A247
        mov    al, 1
        mov    dword ptr [edi + 1b0h], edx
loc_1_A247:
        test   bl, bl
        je     SHORT loc_1_A27B
        test   al, al
        je     SHORT loc_1_A260
        cmp    dword ptr [edi + 54h], 0
        jne    SHORT loc_1_A260
        mov    ecx, edi
        call   sub_1_9AE0
        mov    esi, eax
        jmp    SHORT loc_1_A28C
loc_1_A260:
        push   0
        mov    edx, 3
        push   0
        mov    ecx, edi
        call   dword ptr [edi + 11ch]
        mov    ecx, edi
        call   dword ptr [edi + 118h]
        jmp    SHORT loc_1_A28C
loc_1_A27B:
        push   0
        mov    edx, 3
        push   0
        mov    ecx, edi
        call   dword ptr [edi + 11ch]
loc_1_A28C:
        test   bl, bl
        je     SHORT loc_1_A2D9
        mov    byte ptr [edi + 2eh], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_A2C3
        db  03Bh, 03Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_A2C3
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 01Ch, 00Ch, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_A2C3:
        cmp    byte ptr [edi + 2ch], 1
        jne    SHORT loc_1_A2D9
        mov    byte ptr [edi + 2ch], 0
        add    edi, 2eh
        push   edi
        call   sub_1_689B
        add    esp, 4
loc_1_A2D9:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_A2F0:
        push   esi
        mov    edx, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        ret    8
        db  0CCh, 0CCh, 0CCh
sub_1_A308:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 012h, 000h, 001h, 000h
        pop    esi
        ret    4
        db  0CCh
_LTEXT ends
        end
