; pppmac_lcode_01_04.asm -- PPPMAC object 1 segment _LTEXT  (part 4/6)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_E830:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [ebp + 0ch], 0
        mov    ecx, dword ptr [ebp + 8]
        je     SHORT loc_1_E844
        or     word ptr [ecx + 0ah], 80h
        jmp    SHORT loc_1_E84A
loc_1_E844:
        and    word ptr [ecx + 0ah], 0ff7fh
loc_1_E84A:
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_1_E857
        or     word ptr [ecx + 0ah], 40h
        jmp    SHORT loc_1_E85D
loc_1_E857:
        and    word ptr [ecx + 0ah], 0ffbfh
loc_1_E85D:
        cmp    dword ptr [ebp + 14h], 0
        je     SHORT loc_1_E871
        or     word ptr [ecx + 0ah], 2
        mov    eax, 800h
        pop    ebp
        ret    10h
loc_1_E871:
        and    word ptr [ecx + 0ah], 0fffdh
        mov    eax, 800h
        pop    ebp
        ret    10h
sub_1_E880:
        push   ebp
        mov    ebp, esp
        sub    esp, 0a4h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [eax], 0
        mov    dword ptr [ebp - 44h], 202h
        mov    dword ptr [ebp - 38h], 100h
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_E9C1
        mov    edi, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        sub    ebx, ebx
        lea    esp, [esp]
loc_1_E8C0:
        mov    eax, dword ptr [edi]
        cmp    eax, 200h
        jne    NEAR PTR loc_1_E9AF
        mov    dword ptr [ebp - 44h], eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ebp - 40h], eax
        mov    dword ptr [ebp - 3ch], 100h
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 34h], 1
        mov    dword ptr [ebp - 4], 4
        mov    dword ptr [ebp - 20h], 4655424eh
        mov    dword ptr [ebp - 30h], ebx
        mov    dword ptr [ebp - 24h], 4
        mov    dword ptr [ebp - 2ch], eax
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 28h], ebx
        mov    dword ptr [eax], ebx
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax + 8], ebx
        mov    dword ptr [eax + 0ch], ebx
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 30h]
        push   eax
        lea    eax, [ebp - 44h]
        push   eax
        push   ebx
        call   dword ptr [esi + 48h]
        add    esp, 14h
        cmp    eax, ebx
        jne    NEAR PTR loc_1_E9C3
        cmp    dword ptr [ebp - 0ch], 202h
        jne    SHORT loc_1_E9AF
        mov    dword ptr [ebp - 3ch], 200h
        mov    dword ptr [ebp - 34h], 1
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 4], 5dh
        mov    dword ptr [eax], ebx
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax + 8], ebx
        mov    dword ptr [eax + 0ch], ebx
        lea    eax, [ebp - 0a4h]
        mov    dword ptr [ebp - 20h], 4655424eh
        mov    dword ptr [ebp - 30h], ebx
        mov    dword ptr [ebp - 24h], 5dh
        mov    dword ptr [ebp - 2ch], eax
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 28h], ebx
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 30h]
        push   eax
        lea    eax, [ebp - 44h]
        push   eax
        push   ebx
        call   dword ptr [esi + 48h]
        add    esp, 14h
        cmp    eax, ebx
        je     SHORT loc_1_E998
        cmp    eax, 9
        jne    SHORT loc_1_E9C3
loc_1_E998:
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 0a4h]
        cmp    dword ptr [eax + 4], ecx
        jne    SHORT loc_1_E9AF
        cmp    dword ptr [ebp - 94h], 6
        je     SHORT loc_1_E9D0
loc_1_E9AF:
        add    edi, 8
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        cmp    eax, dword ptr [ebp + 10h]
        jb     NEAR PTR loc_1_E8C0
loc_1_E9C1:
        sub    eax, eax
loc_1_E9C3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_E9D0:
        push   6
        lea    eax, [ebp - 90h]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        call   sub_1_083C
        mov    eax, dword ptr [ebp + 20h]
        mov    ecx, dword ptr [ebp - 0a4h]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp + 1ch]
        pop    edi
        mov    dword ptr [eax], 1
        sub    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  090h
sub_1_EA00:
        push   ebp
        mov    ebp, esp
        sub    esp, 1bch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 60h], 0
        mov    dword ptr [ebp - 5ch], 0
        mov    dword ptr [ebp - 58h], 100h
        lea    eax, [ebp - 1bch]
        mov    dword ptr [ebp - 54h], 100h
        mov    dword ptr [ebp - 50h], 0
        mov    dword ptr [ebp - 4], 100h
        mov    dword ptr [ebp - 3ch], 4655424eh
        mov    dword ptr [ebp - 4ch], 0
        mov    dword ptr [ebp - 40h], 100h
        mov    dword ptr [ebp - 48h], eax
        lea    eax, [ebp - 38h]
        mov    dword ptr [ebp - 44h], 0
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0ch], 0
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 4ch]
        push   eax
        lea    eax, [ebp - 60h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   0
        call   dword ptr [eax + 48h]
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_1_EAA1
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_EAA1:
        mov    eax, dword ptr [ebp - 4]
        shr    eax, 3
        mov    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp - 0ch], 0
        or     eax, eax
        je     NEAR PTR loc_1_EB4A
        lea    esi, [ebp - 1bch]
        sub    edi, edi
        mov    ebx, 100h
loc_1_EAC6:
        mov    eax, dword ptr [esi]
        cmp    eax, 301h
        jne    SHORT loc_1_EB38
        mov    dword ptr [ebp - 60h], eax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ebp - 5ch], eax
        mov    dword ptr [ebp - 58h], ebx
        lea    eax, [ebp - 18h]
        mov    dword ptr [ebp - 54h], ebx
        mov    dword ptr [ebp - 50h], 1
        mov    dword ptr [ebp - 4], 4
        mov    dword ptr [ebp - 3ch], 4655424eh
        mov    dword ptr [ebp - 4ch], edi
        mov    dword ptr [ebp - 40h], 4
        mov    dword ptr [ebp - 48h], eax
        lea    eax, [ebp - 38h]
        mov    dword ptr [ebp - 44h], edi
        mov    dword ptr [eax], edi
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [eax + 8], edi
        mov    dword ptr [eax + 0ch], edi
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 4ch]
        push   eax
        lea    eax, [ebp - 60h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   edi
        call   dword ptr [eax + 48h]
        add    esp, 14h
        cmp    eax, edi
        jne    SHORT loc_1_EB38
        cmp    dword ptr [ebp - 18h], 303h
        je     SHORT loc_1_EB4A
loc_1_EB38:
        add    esi, 8
        inc    dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 10h]
        cmp    eax, dword ptr [ebp - 0ch]
        ja     NEAR PTR loc_1_EAC6
loc_1_EB4A:
        mov    eax, dword ptr [ebp - 10h]
        cmp    eax, dword ptr [ebp - 0ch]
        jne    SHORT loc_1_EB60
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_1_EB60:
        mov    dword ptr [ebp - 58h], 200h
        mov    dword ptr [ebp - 50h], 1
        mov    dword ptr [ebp - 4], 5ch
        mov    dword ptr [ebp - 3ch], 4655424eh
        lea    eax, [ebp - 0bch]
        mov    dword ptr [ebp - 4ch], 0
        mov    dword ptr [ebp - 40h], 5ch
        mov    dword ptr [ebp - 48h], eax
        lea    eax, [ebp - 38h]
        mov    dword ptr [ebp - 44h], 0
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0ch], 0
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 4ch]
        push   eax
        lea    eax, [ebp - 60h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   0
        call   dword ptr [eax + 48h]
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_1_EBE0
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_EBE0:
        cmp    dword ptr [ebp - 68h], 1
        jae    SHORT loc_1_EBF2
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_EBF2:
        mov    eax, dword ptr [ebp - 68h]
        lea    eax, [eax + eax*2]
        shl    eax, 3
        mov    dword ptr [ebp - 1ch], eax
        mov    dword ptr [ebp - 4], eax
        db  0A1h, 050h, 082h, 001h, 000h
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_0756
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_EC28
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_EC28:
        mov    dword ptr [ebp - 50h], 102h
        mov    dword ptr [ebp - 3ch], 4655424eh
        mov    dword ptr [ebp - 4ch], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 40h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 48h], eax
        lea    eax, [ebp - 38h]
        mov    dword ptr [ebp - 44h], 0
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0ch], 0
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 4ch]
        push   eax
        lea    eax, [ebp - 60h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   0
        call   dword ptr [eax + 48h]
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_1_ECA5
        push   0
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_075C
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_ECA5:
        sub    esi, esi
        cmp    dword ptr [ebp - 68h], esi
        jbe    SHORT loc_1_ED00
        sub    edi, edi
        mov    ebx, dword ptr [ebp + 0ch]
loc_1_ECB1:
        lea    eax, [ebp - 20h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, edi
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 1bch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_E880
        add    esp, 1ch
        or     eax, eax
        jne    SHORT loc_1_ED1B
        cmp    dword ptr [ebp - 14h], 0
        je     SHORT loc_1_ECF7
        lea    ecx, [ebp - 28h]
        mov    eax, dword ptr [ebx]
        cmp    eax, dword ptr [ecx]
        jne    SHORT loc_1_ECF5
        mov    ax, word ptr [ebx + 4]
        cmp    ax, word ptr [ecx + 4]
loc_1_ECF5:
        je     SHORT loc_1_ED36
loc_1_ECF7:
        add    edi, 18h
        inc    esi
        cmp    dword ptr [ebp - 68h], esi
        ja     SHORT loc_1_ECB1
loc_1_ED00:
        push   0
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_075C
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_ED1B:
        push   0
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_075C
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_ED36:
        lea    eax, [esi + esi*2]
        lea    ecx, [eax*8]
        mov    eax, dword ptr [ebp - 8]
        mov    dx, word ptr [ecx + eax + 14h]
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [eax], dx
        mov    eax, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp + eax*8 - 1b8h]
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [ebp + 18h]
        mov    edx, dword ptr [ebp - 20h]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [ebp - 8]
        mov    edx, dword ptr [ecx + eax]
        mov    eax, dword ptr [ebp + 1ch]
        push   0
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + eax + 8]
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_075C
        mov    eax, 800h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_EDA0:
        push   ebp
        mov    ebp, esp
        test   byte ptr [ebp + 8], 80h
        jne    SHORT loc_1_EDB7
        test   byte ptr [ebp + 0ch], 80h
        jne    SHORT loc_1_EDB7
        mov    al, byte ptr [ebp + 0ch]
        cmp    al, byte ptr [ebp + 8]
        je     SHORT loc_1_EDF5
loc_1_EDB7:
        mov    al, byte ptr [ebp + 8]
        and    al, 0c0h
        cmp    al, 80h
        jne    SHORT loc_1_EDD6
        mov    al, byte ptr [ebp + 0ch]
        and    al, 0c0h
        cmp    al, 80h
        jne    SHORT loc_1_EDD6
        mov    eax, dword ptr [ebp + 0ch]
        xor    eax, dword ptr [ebp + 8]
        test   eax, 0ffffh
        je     SHORT loc_1_EDF5
loc_1_EDD6:
        mov    al, byte ptr [ebp + 8]
        and    al, 0e0h
        cmp    al, 0c0h
        jne    SHORT loc_1_EE00
        mov    al, byte ptr [ebp + 0ch]
        and    al, 0e0h
        cmp    al, 0c0h
        jne    SHORT loc_1_EE00
        mov    eax, dword ptr [ebp + 0ch]
        xor    eax, dword ptr [ebp + 8]
        test   eax, 0ffffffh
        jne    SHORT loc_1_EE00
loc_1_EDF5:
        mov    eax, 1
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_EE00:
        sub    eax, eax
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_EE10:
        push   ebp
        mov    ebp, esp
        sub    esp, 74h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 30h], 301h
        mov    esi, 0ah
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 2ch], eax
        mov    dword ptr [ebp - 28h], 200h
        mov    dword ptr [ebp - 24h], 100h
        sub    edi, edi
loc_1_EE3B:
        add    esi, esi
        mov    eax, esi
        shl    eax, 4
        mov    dword ptr [ebp - 20h], 101h
        lea    ebx, [eax + eax*2]
        lea    eax, [ebp - 1ch]
        mov    dword ptr [eax], edi
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [eax + 8], edi
        mov    dword ptr [eax + 0ch], edi
        db  0A1h, 050h, 082h, 001h, 000h
        push   eax
        lea    eax, [ebp - 8]
        push   edi
        push   ebx
        push   eax
        call   sub_1_0756
        cmp    dword ptr [ebp - 8], edi
        je     SHORT loc_1_EEB5
        mov    dword ptr [ebp - 34h], 4655424eh
        mov    dword ptr [ebp - 44h], edi
        mov    dword ptr [ebp - 38h], ebx
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 40h], eax
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 3ch], edi
        mov    dword ptr [ebp - 4], ebx
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 44h]
        push   eax
        lea    eax, [ebp - 30h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   edi
        call   dword ptr [eax + 48h]
        add    esp, 14h
        cmp    eax, 9
        jne    SHORT loc_1_EEC3
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_075C
        jmp    SHORT loc_1_EE3B
loc_1_EEB5:
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_EEC3:
        or     eax, eax
        je     SHORT loc_1_EEE1
        push   0
        push   ebx
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_075C
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_EEE1:
        mov    ecx, 30h
        mov    eax, dword ptr [ebp - 4]
        sub    edx, edx
        div    ecx
        mov    esi, dword ptr [ebp - 8]
        or     eax, eax
        je     NEAR PTR loc_1_EFF3
        mov    dword ptr [ebp - 0ch], eax
        mov    edi, dword ptr [ebp + 14h]
        mov    edi, edi
loc_1_EF00:
        mov    eax, dword ptr [esi]
        cmp    eax, edi
        je     NEAR PTR loc_1_EFE7
        cmp    dword ptr [esi + 18h], edi
        je     NEAR PTR loc_1_EFE7
        cmp    dword ptr [esi + 28h], -1
        je     NEAR PTR loc_1_EFE7
        cmp    al, 7fh
        je     NEAR PTR loc_1_EFE7
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [esi + 4], eax
        je     NEAR PTR loc_1_EFE7
        mov    dword ptr [ebp - 4], 30h
        mov    dword ptr [ebp - 20h], 101h
        cmp    dword ptr [ebp + 18h], 0
        je     SHORT loc_1_EF54
        cmp    dword ptr [esi + 8], 1
        jne    SHORT loc_1_EF75
        mov    dword ptr [esi + 8], 2
        jmp    SHORT loc_1_EF61
loc_1_EF54:
        cmp    dword ptr [esi + 8], 2
        jne    SHORT loc_1_EF75
        mov    dword ptr [esi + 8], 1
loc_1_EF61:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   esi
        lea    eax, [ebp - 30h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   0
        call   dword ptr [eax + 4ch]
        add    esp, 10h
loc_1_EF75:
        mov    eax, dword ptr [esi]
        push   eax
        push   edi
        call   sub_1_EDA0
        or     eax, eax
        je     SHORT loc_1_EFE7
        mov    dword ptr [ebp - 30h], 301h
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 2ch], eax
        mov    dword ptr [ebp - 28h], 200h
        mov    dword ptr [ebp - 24h], 100h
        mov    dword ptr [ebp - 20h], 101h
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 74h], eax
        mov    eax, dword ptr [esi + 28h]
        mov    dword ptr [ebp - 4ch], eax
        mov    dword ptr [ebp - 5ch], edi
        mov    dword ptr [ebp - 6ch], 1
        cmp    dword ptr [ebp + 18h], 1
        sbb    eax, eax
        add    eax, 3
        mov    dword ptr [ebp - 58h], eax
        mov    dword ptr [ebp - 54h], 2
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 70h], eax
        lea    eax, [ebp - 74h]
        push   30h
        push   eax
        lea    eax, [ebp - 30h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   0
        call   dword ptr [eax + 4ch]
        add    esp, 10h
loc_1_EFE7:
        add    esi, 30h
        dec    dword ptr [ebp - 0ch]
        jne    NEAR PTR loc_1_EF00
loc_1_EFF3:
        push   0
        push   ebx
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_075C
        mov    eax, 800h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 049h, 000h
sub_1_F010:
        push   ebp
        mov    ebp, esp
        sub    esp, 48h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 18h]
        or     ebx, ebx
        je     SHORT loc_1_F038
        push   ebx
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_EE10
        jmp    SHORT loc_1_F03E
loc_1_F038:
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
loc_1_F03E:
        mov    dword ptr [ebp - 18h], 301h
        mov    dword ptr [ebp - 14h], edi
        mov    dword ptr [ebp - 10h], 200h
        mov    dword ptr [ebp - 0ch], 100h
        cmp    ebx, 1
        mov    dword ptr [ebp - 8], 101h
        mov    dword ptr [ebp - 48h], 0
        mov    dword ptr [ebp - 20h], 0
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ebp - 30h], eax
        mov    dword ptr [ebp - 40h], 1
        sbb    eax, eax
        add    eax, 3
        mov    dword ptr [ebp - 2ch], eax
        mov    dword ptr [ebp - 28h], 2
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 44h], eax
        lea    eax, [ebp - 48h]
        push   30h
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        push   0
        call   dword ptr [esi + 4ch]
        add    esp, 10h
        mov    dword ptr [ebp - 4], eax
        or     ebx, ebx
        jne    SHORT loc_1_F0B9
        push   ebx
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   edi
        push   esi
        call   sub_1_EE10
loc_1_F0B9:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_F0D0
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_F0D0:
        mov    eax, 800h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
sub_1_F0E0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0ffh
        cmp    eax, 7fh
        ja     SHORT loc_1_F0F7
        mov    eax, 0ffh
        pop    ebp
        ret
loc_1_F0F7:
        cmp    eax, 0bfh
        ja     SHORT loc_1_F105
        mov    eax, 0ffffh
        pop    ebp
        ret
loc_1_F105:
        mov    eax, 0ffffffh
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
sub_1_F110:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 17bh]
        cmp    eax, edi
        je     NEAR PTR loc_1_F1F0
        or     eax, eax
        je     SHORT loc_1_F16D
        push   0
        push   0
        xor    eax, eax
        mov    ax, word ptr [esi + 16fh]
        push   eax
        db  0E8h, 0F5h, 028h, 0FDh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_F16D
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_F16D
        xor    eax, eax
        db  066h, 0A1h, 0BEh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0CAh, 02Ah, 0FDh, 0FFh
        add    esp, 8
loc_1_F16D:
        mov    dword ptr [esi + 183h], edi
        push   edi
        call   sub_1_F0E0
        add    esp, 4
        push   eax
        push   edi
        xor    eax, eax
        mov    ax, word ptr [esi + 16fh]
        push   eax
        db  0E8h, 0A8h, 028h, 0FDh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_F1F0
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_F1DE
        mov    ecx, edi
        and    ecx, 0ff00h
        mov    eax, edi
        shl    eax, 10h
        or     ecx, eax
        shl    ecx, 8
        mov    eax, edi
        and    eax, 0ff0000h
        shr    edi, 10h
        or     eax, edi
        shr    eax, 8
        or     ecx, eax
        push   ecx
        xor    eax, eax
        db  066h, 0A1h, 0C0h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 059h, 02Ah, 0FDh, 0FFh
        add    esp, 0ch
loc_1_F1DE:
        mov    eax, dword ptr [esi + 17bh]
        mov    dword ptr [esi + 183h], eax
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_1_F1F0:
        cmp    dword ptr [esi + 187h], 0
        je     NEAR PTR loc_1_F287
        test   byte ptr [esi + 18bh], 8
        jne    NEAR PTR loc_1_F287
        mov    eax, dword ptr [esi + 0feh]
        cmp    dword ptr [eax + 4], 1
        jne    SHORT loc_1_F287
        push   1
        push   edi
        mov    eax, dword ptr [esi + 177h]
        push   eax
        mov    eax, dword ptr [esi + 173h]
        push   eax
        mov    eax, dword ptr [esi + 16bh]
        push   eax
        call   sub_1_F010
        add    esp, 14h
        cmp    eax, 800h
        je     SHORT loc_1_F287
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_F287
        mov    ecx, edi
        and    ecx, 0ff00h
        mov    eax, edi
        shl    eax, 10h
        or     ecx, eax
        shl    ecx, 8
        mov    eax, edi
        and    eax, 0ff0000h
        shr    edi, 10h
        or     eax, edi
        shr    eax, 8
        or     ecx, eax
        push   ecx
        xor    eax, eax
        db  066h, 0A1h, 0C2h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0B0h, 029h, 0FDh, 0FFh
        add    esp, 0ch
loc_1_F287:
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_1_F290:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 16bh]
        mov    eax, dword ptr [edi + 20h]
        cmp    word ptr [edi + 16fh], 0
        je     NEAR PTR loc_1_F344
        push   2100h
        push   eax
        call   sub_1_15E0
        add    esp, 8
        mov    ebx, dword ptr [edi + 183h]
        cmp    dword ptr [edi + 17bh], ebx
        je     SHORT loc_1_F30C
        push   0
        push   0
        xor    eax, eax
        mov    ax, word ptr [edi + 16fh]
        push   eax
        db  0E8h, 056h, 027h, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [edi + 17bh]
        mov    dword ptr [edi + 183h], eax
        or     eax, eax
        je     SHORT loc_1_F30C
        mov    ecx, dword ptr [edi + 17fh]
        push   ecx
        push   eax
        xor    eax, eax
        mov    ax, word ptr [edi + 16fh]
        push   eax
        db  0E8h, 02Ch, 027h, 0FDh, 0FFh
        add    esp, 0ch
loc_1_F30C:
        cmp    dword ptr [edi + 187h], 0
        je     SHORT loc_1_F344
        test   byte ptr [edi + 18bh], 8
        jne    SHORT loc_1_F344
        mov    eax, dword ptr [edi + 0feh]
        cmp    dword ptr [eax + 4], 1
        jne    SHORT loc_1_F344
        push   0
        push   ebx
        mov    eax, dword ptr [edi + 177h]
        push   eax
        mov    eax, dword ptr [edi + 173h]
        push   eax
        push   esi
        call   sub_1_F010
        add    esp, 14h
loc_1_F344:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_F350:
        push   ebp
        mov    ebp, esp
        sub    esp, 4ch
        mov    dword ptr [ebp - 4ch], 4ch
        mov    word ptr [ebp - 48h], 4
        mov    word ptr [ebp - 46h], 3f80h
        mov    word ptr [ebp - 44h], 0d580h
        mov    dword ptr [ebp - 42h], 3fch
        mov    byte ptr [ebp - 3eh], 1
        mov    byte ptr [ebp - 3dh], 1
        mov    dword ptr [ebp - 3ch], 0
        db  0C7h, 045h, 0C8h, 02Ch, 08Ch, 001h, 000h
        db  0C7h, 045h, 0CCh, 050h, 0F4h, 000h, 000h
        db  0C7h, 045h, 0D0h, 0B0h, 0F4h, 000h, 000h
        db  0C7h, 045h, 0D4h, 010h, 0F5h, 000h, 000h
        db  0C7h, 045h, 0D8h, 090h, 0F5h, 000h, 000h
        db  0C7h, 045h, 0DCh, 0C0h, 0F5h, 000h, 000h
        mov    dword ptr [ebp - 20h], 0
        db  0C7h, 045h, 0E4h, 070h, 0F6h, 000h, 000h
        xor    eax, eax
        db  0C7h, 045h, 0E8h, 0F0h, 0F6h, 000h, 000h
        db  0C7h, 045h, 0ECh, 020h, 0F8h, 000h, 000h
        db  0C7h, 045h, 0F0h, 0F0h, 006h, 001h, 000h
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], 0
        db  066h, 0A1h, 0FEh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        mov    dword ptr [ebp - 3ch], eax
        lea    eax, [ebp - 4ch]
        push   eax
        call   PPPMAC_Service_01
        add    esp, 4
        db  0C7h, 005h, 020h, 08Ch, 001h, 000h, 001h, 000h, 000h, 000h
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_F420:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    byte ptr [eax + 140h], 0
        mov    byte ptr [eax + 141h], 0
        mov    byte ptr [eax + 142h], 0
        mov    byte ptr [eax + 143h], 0
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_F450:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        db  0E8h, 0FFh, 025h, 0FDh, 0FFh
        lea    edi, [esi + 4bh]
        or     al, al
        jne    SHORT loc_1_F470
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 049h, 000h
loc_1_F470:
        lea    eax, [edi + 30h]
        push   eax
        call   sub_1_F420
        add    edi, 2b8h
        push   edi
        call   sub_1_F420
        mov    eax, dword ptr [esi + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 0feh]
        cmp    dword ptr [eax + 4], 1
        jne    SHORT loc_1_F4A3
        mov    word ptr [esi + 27h], 2710h
        mov    word ptr [esi + 23h], 2710h
loc_1_F4A3:
        mov    eax, 800h
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_F4B0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4bh]
        lea    edi, [esi + 4bh]
        or     eax, eax
        je     SHORT loc_1_F4CE
        push   0
        mov    ecx, dword ptr [edi + 4]
        push   ecx
        push   eax
        call   sub_1_075C
loc_1_F4CE:
        mov    eax, dword ptr [edi + 10h]
        or     eax, eax
        je     SHORT loc_1_F4E1
        push   0
        mov    ecx, dword ptr [edi + 14h]
        push   ecx
        push   eax
        call   sub_1_075C
loc_1_F4E1:
        push   edi
        call   sub_1_10BC0
        call   sub_1_109E0
        db  083h, 03Dh, 020h, 08Ch, 001h, 000h, 004h
        jne    SHORT loc_1_F4FC
        mov    dword ptr [esi + 3fh], 801h
loc_1_F4FC:
        mov    eax, dword ptr [edi + 20h]
        push   eax
        db  0E8h, 0A7h, 026h, 0FDh, 0FFh
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_F510:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 4]
        add    edi, 4bh
        mov    esi, dword ptr [eax + 0ch]
        sub    ebx, ebx
        cmp    dword ptr [edi + 3f4h], ebx
        je     SHORT loc_1_F533
        mov    dword ptr [esi + 59h], 1
loc_1_F533:
        cmp    dword ptr [edi + 2b0h], 0
        je     SHORT loc_1_F547
        mov    dword ptr [esi + 5dh], 1
        add    dword ptr [esi + 1ch], 0ch
loc_1_F547:
        cmp    dword ptr [edi + 3f4h], 0
        je     SHORT loc_1_F562
        mov    eax, dword ptr [esi + 18h]
        cmp    eax, 5f2h
        jne    SHORT loc_1_F562
        mov    ebx, eax
        lea    eax, [ebx - 0ch]
        mov    dword ptr [esi + 18h], eax
loc_1_F562:
        push   3f00h
        push   esi
        call   PPPMAC_Service_05
        add    esp, 8
        or     ebx, ebx
        je     SHORT loc_1_F577
        mov    dword ptr [esi + 18h], ebx
loc_1_F577:
        push   edi
        call   sub_1_10B70
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_F590:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   3f00h
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    dword ptr [eax + 59h], 0
        mov    dword ptr [eax + 5dh], 0
        call   PPPMAC_Service_06
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_F5C0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [esi + 4bh]
        mov    eax, dword ptr [esi + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    word ptr [edi + 144h], 4
        mov    word ptr [edi + 146h], 0
        mov    byte ptr [edi + 168h], 0
        mov    byte ptr [edi + 171h], 1
        mov    eax, dword ptr [eax + 0feh]
        cmp    dword ptr [eax + 4], 1
        jne    SHORT loc_1_F620
        push   esi
        call   sub_1_10760
        cmp    eax, 800h
        jne    SHORT loc_1_F611
        mov    byte ptr [edi + 170h], 1
loc_1_F611:
        mov    word ptr [edi + 142h], 7
        jmp    SHORT loc_1_F659
        db  08Dh, 064h, 024h, 000h
loc_1_F620:
        cmp    dword ptr [eax + 14h], 0
        je     SHORT loc_1_F650
        call   sub_1_10970
        or     al, al
        je     SHORT loc_1_F650
        mov    dword ptr [edi + 24h], 1
        mov    dword ptr [edi + 28h], 0
        push   esi
        call   sub_1_10A20
        mov    word ptr [edi + 142h], 1
        jmp    SHORT loc_1_F659
        db  08Bh, 0FFh
loc_1_F650:
        mov    word ptr [edi + 142h], 3
loc_1_F659:
        lea    esi, [edi + 30h]
        add    edi, 174h
        mov    ecx, 51h
        rep movsd
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 049h, 000h
loc_1_F670:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 1bfh
        sub    dx, dx
        cmp    byte ptr [ecx + 140h], dl
        je     SHORT loc_1_F6A7
        xor    eax, eax
        mov    ax, word ptr [ecx + 110h]
        cmp    eax, 0eh
        jle    SHORT loc_1_F69C
        mov    dx, 0f0h
        sub    eax, 0eh
loc_1_F69C:
        imul   ax, ax, 11h
        add    ax, 2
        add    dx, ax
loc_1_F6A7:
        cmp    byte ptr [ecx + 141h], 0
        je     SHORT loc_1_F6C2
        mov    al, byte ptr [ecx + 138h]
        and    eax, 0ffff00ffh
        add    ax, 8
        add    dx, ax
loc_1_F6C2:
        cmp    byte ptr [ecx + 142h], 0
        je     SHORT loc_1_F6CF
        add    dx, 5
loc_1_F6CF:
        cmp    byte ptr [ecx + 143h], 0
        je     SHORT loc_1_F6DC
        add    dx, 2
loc_1_F6DC:
        mov    eax, dword ptr [ebp + 0ch]
        pop    ebp
        mov    word ptr [eax], dx
        mov    al, 1
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_F6F0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        add    edi, 1bfh
        sub    ecx, ecx
        cmp    byte ptr [edi + 140h], cl
        je     SHORT loc_1_F767
        xor    ebx, ebx
        mov    bx, word ptr [edi + 110h]
        cmp    ebx, 0eh
        mov    esi, dword ptr [ebp + 0ch]
        jle    SHORT loc_1_F73C
        mov    byte ptr [esi], 1
        inc    esi
        push   0eeh
        mov    byte ptr [esi], 0f0h
        inc    esi
        push   edi
        push   esi
        call   sub_1_083C
        add    esi, 0eeh
        mov    ecx, 0eh
        sub    ebx, 0eh
loc_1_F73C:
        mov    byte ptr [esi], 1
        inc    esi
        mov    dl, 11h
        mov    al, bl
        imul   dl
        add    al, 2
        mov    byte ptr [esi], al
        inc    esi
        mov    eax, ebx
        shl    eax, 4
        add    ebx, eax
        push   ebx
        mov    eax, ecx
        shl    eax, 4
        add    eax, ecx
        add    eax, edi
        push   eax
        push   esi
        call   sub_1_083C
        add    esi, ebx
        jmp    SHORT loc_1_F76A
loc_1_F767:
        mov    esi, dword ptr [ebp + 0ch]
loc_1_F76A:
        cmp    byte ptr [edi + 141h], 0
        je     SHORT loc_1_F7DC
        mov    byte ptr [esi], 2
        inc    esi
        mov    al, byte ptr [edi + 138h]
        add    al, 8
        mov    byte ptr [esi], al
        inc    esi
        mov    ax, word ptr [edi + 112h]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [esi], cx
        add    esi, 2
        mov    ax, word ptr [edi + 114h]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [esi], cx
        add    esi, 2
        mov    ax, word ptr [edi + 116h]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [esi], cx
        add    esi, 2
        mov    al, byte ptr [edi + 138h]
        or     al, al
        je     SHORT loc_1_F7DC
        and    eax, 0ffh
        push   eax
        lea    eax, [edi + 118h]
        push   eax
        push   esi
        call   sub_1_083C
        xor    eax, eax
        mov    al, byte ptr [edi + 138h]
        add    esi, eax
loc_1_F7DC:
        cmp    byte ptr [edi + 142h], 0
        je     SHORT loc_1_F807
        mov    byte ptr [esi], 3
        inc    esi
        mov    byte ptr [esi], 5
        inc    esi
        mov    ax, word ptr [edi + 139h]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [esi], cx
        add    esi, 2
        mov    al, byte ptr [edi + 13bh]
        mov    byte ptr [esi], al
        inc    esi
loc_1_F807:
        cmp    byte ptr [edi + 143h], 0
        je     SHORT loc_1_F817
        mov    byte ptr [esi], 4
        mov    byte ptr [esi + 1], 2
loc_1_F817:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_F820:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    ecx, [esi + 4bh]
        mov    eax, dword ptr [esi + 4]
        mov    edx, dword ptr [esi + 2fh]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    bl, 10h
        movzx  edi, byte ptr [edx]
        cmp    edi, 2
        jl     SHORT loc_1_F85B
        cmp    edi, 4
        jle    SHORT loc_1_F84F
        cmp    edi, 6
        jne    SHORT loc_1_F85B
loc_1_F84F:
        mov    al, byte ptr [edx + 1]
        cmp    byte ptr [esi + 15h], al
        jne    NEAR PTR loc_1_F972
loc_1_F85B:
        dec    edi
        cmp    edi, 6
        ja     SHORT loc_1_F868
        db  0FFh, 024h, 0BDh, 07Ch, 0F9h, 000h, 000h
loc_1_F868:
        mov    bl, 0ch
        mov    byte ptr [esi + 11h], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_F874:
        lea    eax, [esi + 39h]
        push   eax
        mov    eax, dword ptr [esi + 35h]
        push   eax
        mov    ax, word ptr [esi + 33h]
        push   eax
        push   edx
        push   esi
        call   sub_1_F9A0
        or     al, al
        je     SHORT loc_1_F898
        mov    bl, 6
        mov    byte ptr [esi + 11h], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_F898:
        cmp    byte ptr [esi + 11h], 10h
        je     NEAR PTR loc_1_F975
        mov    bl, 7
        mov    byte ptr [esi + 11h], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_1_F8B0:
        mov    ax, word ptr [esi + 33h]
        push   eax
        push   edx
        push   ecx
        call   sub_1_FF40
        or     al, al
        je     NEAR PTR loc_1_F972
        mov    bl, 8
        mov    byte ptr [esi + 11h], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_F8D0:
        mov    ax, word ptr [esi + 33h]
        push   eax
        push   edx
        push   ecx
        call   sub_1_10160
        cmp    eax, 1
        jne    SHORT loc_1_F8F0
        mov    bl, 9
        mov    byte ptr [esi + 11h], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h
loc_1_F8F0:
        cmp    eax, 2
        jne    SHORT loc_1_F972
        mov    byte ptr [esi + 11h], 3
        mov    byte ptr [esi + 10h], 9
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_F904:
        mov    ax, word ptr [esi + 33h]
        push   eax
        push   edx
        push   ecx
        call   sub_1_10440
        or     al, al
        je     SHORT loc_1_F972
        mov    bl, 9
        mov    byte ptr [esi + 11h], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_F920:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0feh]
        cmp    dword ptr [eax + 4], 2
        jne    SHORT loc_1_F951
        mov    byte ptr [esi + 11h], 0ah
        push   esi
        call   PPPMAC_Service_07
        add    esp, 4
        push   esi
        call   PPPMAC_Service_08
        add    esp, 4
        mov    bl, 5
        mov    byte ptr [esi + 11h], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_F951:
        mov    bl, 0ah
        mov    byte ptr [esi + 11h], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h
loc_1_F960:
        mov    bl, 0bh
        mov    byte ptr [esi + 11h], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_F970:
        mov    bl, 0dh
loc_1_F972:
        mov    byte ptr [esi + 11h], bl
loc_1_F975:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  074h, 0F8h, 000h, 000h, 0B0h, 0F8h, 000h, 000h, 0D0h, 0F8h, 000h, 000h, 004h, 0F9h, 000h, 000h
        db  020h, 0F9h, 000h, 000h, 060h, 0F9h, 000h, 000h, 070h, 0F9h, 000h, 000h, 08Dh, 064h, 024h, 000h
        db  08Dh, 064h, 024h, 000h
sub_1_F9A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 4bh
        mov    dword ptr [ebp - 14h], ecx
        add    ecx, 2b8h
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 174h
        mov    dword ptr [ebp - 30h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 1ch], ecx
        lea    esi, [eax + 4]
        mov    ax, word ptr [ebp + 10h]
        sub    ax, 4
        mov    word ptr [ebp - 6], ax
        mov    byte ptr [ebp - 1], 2
        push   ecx
        call   sub_1_F420
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 10h], 9
        jne    SHORT loc_1_FA37
        mov    edi, dword ptr [eax + 3bh]
        mov    dword ptr [eax + 3bh], 0
        mov    byte ptr [eax + 11h], 10h
        mov    ecx, dword ptr [edi + 1ch]
        add    ecx, dword ptr [edi + 20h]
        mov    esi, dword ptr [ebp + 10h]
        and    esi, 0ffffh
        mov    byte ptr [ecx], 2
        lea    eax, [esi - 1]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        inc    eax
        push   eax
        inc    ecx
        push   ecx
        call   sub_1_083C
        mov    dword ptr [edi + 24h], esi
        mov    word ptr [edi + 28h], 3f80h
        push   1
        push   edi
        call   sub_1_4360
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_FA37:
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 1]
        mov    byte ptr [edx + 1], al
        mov    eax, edx
        add    eax, 4
        mov    dword ptr [ebp - 28h], eax
        mov    edi, eax
        cmp    word ptr [ebp - 6], 0
        je     SHORT loc_1_FACE
loc_1_FA54:
        mov    al, byte ptr [esi]
        mov    byte ptr [ebp - 3], al
        mov    al, byte ptr [esi + 1]
        inc    esi
        mov    byte ptr [ebp - 2], al
        inc    esi
        and    ax, 0ffh
        mov    word ptr [ebp - 18h], ax
        mov    ebx, eax
        and    ebx, 0ffffh
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        cmp    eax, ebx
        jb     NEAR PTR loc_1_FEA0
        xor    eax, eax
        mov    al, byte ptr [ebp - 3]
        cmp    eax, 1
        je     NEAR PTR loc_1_FB10
        cmp    eax, 2
        je     NEAR PTR loc_1_FD70
        cmp    eax, 4
        je     NEAR PTR loc_1_FE10
loc_1_FA9F:
        cmp    byte ptr [ebp - 1], 4
        je     SHORT loc_1_FAAC
        mov    edi, dword ptr [ebp - 28h]
        mov    byte ptr [ebp - 1], 4
loc_1_FAAC:
        sub    esi, 2
        push   ebx
        push   esi
        push   edi
        call   sub_1_083C
loc_1_FAB7:
        add    edi, ebx
        add    esi, ebx
loc_1_FABB:
        mov    ax, word ptr [ebp - 6]
        sub    ax, word ptr [ebp - 18h]
        mov    word ptr [ebp - 6], ax
loc_1_FAC7:
        cmp    word ptr [ebp - 6], 0
        jne    SHORT loc_1_FA54
loc_1_FACE:
        cmp    byte ptr [ebp - 1], 2
        je     SHORT loc_1_FADD
        mov    ecx, dword ptr [ebp - 1ch]
        push   ecx
        call   sub_1_F420
loc_1_FADD:
        mov    eax, dword ptr [ebp + 18h]
        sub    edi, dword ptr [ebp + 14h]
        mov    word ptr [eax], di
        mov    edx, dword ptr [ebp + 14h]
        mov    cl, byte ptr [ebp - 1]
        mov    byte ptr [edx], cl
        mov    ax, word ptr [eax]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [edx + 2], cx
        mov    al, byte ptr [ebp - 1]
        sub    al, 2
        cmp    al, 1
        sbb    al, al
        neg    al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 049h, 000h
loc_1_FB10:
        mov    ecx, dword ptr [ebp - 14h]
        mov    eax, dword ptr [ecx + 24h]
        cmp    eax, 3
        je     NEAR PTR loc_1_FEB3
        or     eax, eax
        je     NEAR PTR loc_1_FA9F
        cmp    byte ptr [ebp - 1], 4
        jne    SHORT loc_1_FB33
        lea    esi, [ebx + esi - 2]
        jmp    SHORT loc_1_FABB
loc_1_FB33:
        cmp    eax, 4
        jne    SHORT loc_1_FBB0
        mov    ecx, dword ptr [ebp - 14h]
        mov    eax, dword ptr [ecx + 2ch]
        or     eax, eax
        jne    SHORT loc_1_FB51
        cmp    byte ptr [ebp - 1], 3
        jne    SHORT loc_1_FB51
        lea    esi, [ebx + esi - 2]
        jmp    NEAR PTR loc_1_FABB
loc_1_FB51:
        cmp    eax, 1
        jne    SHORT loc_1_FB79
        cmp    byte ptr [ebp - 1], 2
        jne    SHORT loc_1_FB63
        mov    byte ptr [ebp - 1], 3
        mov    edi, dword ptr [ebp - 28h]
loc_1_FB63:
        cmp    eax, 1
        jne    SHORT loc_1_FB79
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ecx + 28h], 0
        mov    dword ptr [ecx + 24h], 1
loc_1_FB79:
        sub    esi, 2
        push   ebx
        push   esi
        push   edi
        call   sub_1_083C
        cmp    byte ptr [ebp - 1], 2
        je     NEAR PTR loc_1_FAB7
        lea    ecx, [edi + 2]
        lea    eax, [edi + ebx]
        cmp    ecx, eax
        jae    NEAR PTR loc_1_FAB7
loc_1_FBA0:
        add    ecx, 10h
        mov    byte ptr [ecx], 0dh
        inc    ecx
        cmp    ecx, eax
        jb     SHORT loc_1_FBA0
        jmp    NEAR PTR loc_1_FAB7
loc_1_FBB0:
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 10h
        mov    dword ptr [ebp - 20h], eax
        mov    edx, dword ptr [eax]
        or     edx, edx
        je     SHORT loc_1_FBF2
        mov    ecx, dword ptr [ebp - 14h]
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ecx + 14h]
        and    eax, 0ffffh
        add    eax, 124h
        cmp    ecx, eax
        jae    SHORT loc_1_FBF2
        push   0
        push   ecx
        push   edx
        call   sub_1_075C
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [eax], 0
        mov    dword ptr [ecx + 14h], 0
loc_1_FBF2:
        mov    eax, dword ptr [ebp - 20h]
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_FC3B
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 14h]
        add    eax, 124h
        mov    dword ptr [ecx + 14h], eax
        db  08Bh, 00Dh, 050h, 082h, 001h, 000h
        push   ecx
        push   0
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        call   sub_1_0756
        mov    eax, dword ptr [ebp - 20h]
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     NEAR PTR loc_1_FEC6
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ecx + 18h], eax
        add    eax, 124h
        mov    dword ptr [ecx + 1ch], eax
loc_1_FC3B:
        mov    ecx, dword ptr [ebp - 14h]
        sub    eax, eax
        mov    ecx, dword ptr [ecx + 18h]
        mov    dword ptr [ebp - 2ch], ecx
        mov    ecx, dword ptr [ebp - 14h]
        mov    edi, dword ptr [ebp - 2ch]
        mov    edx, dword ptr [ecx + 1ch]
        mov    ecx, 49h
        rep stosd
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   edx
        call   sub_1_083C
        mov    ecx, dword ptr [ebp - 2ch]
        mov    dword ptr [ebp - 0ch], 0
        mov    word ptr [ecx], 1
        mov    edi, esi
        lea    eax, [ebx + esi - 2]
        mov    dword ptr [ebp - 24h], eax
        cmp    esi, eax
        jae    SHORT loc_1_FCB0
        add    ecx, 4
        mov    dword ptr [ebp - 10h], ecx
loc_1_FC8B:
        push   10h
        push   edi
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        call   sub_1_083C
        mov    al, byte ptr [edi + 10h]
        add    edi, 10h
        mov    ecx, dword ptr [ebp - 10h]
        inc    edi
        mov    byte ptr [ecx + 10h], al
        add    dword ptr [ebp - 10h], 12h
        inc    dword ptr [ebp - 0ch]
        cmp    edi, dword ptr [ebp - 24h]
        jb     SHORT loc_1_FC8B
loc_1_FCB0:
        mov    ecx, dword ptr [ebp - 2ch]
        mov    ax, word ptr [ebp - 0ch]
        mov    word ptr [ecx + 2], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_10AC0
        or     al, al
        jne    NEAR PTR loc_1_FEE0
        mov    byte ptr [ebp - 1], 3
        sub    esi, 2
        push   ebx
        push   esi
        mov    eax, dword ptr [ebp - 28h]
        push   eax
        call   sub_1_083C
        mov    ecx, dword ptr [ebp - 28h]
        add    ecx, 2
        mov    eax, dword ptr [ebp - 0ch]
        dec    dword ptr [ebp - 0ch]
        or     eax, eax
        je     SHORT loc_1_FD01
        mov    edi, edi
loc_1_FCF0:
        add    ecx, 10h
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx], 35h
        inc    ecx
        dec    dword ptr [ebp - 0ch]
        or     eax, eax
        jne    SHORT loc_1_FCF0
loc_1_FD01:
        mov    eax, dword ptr [ebp - 28h]
        lea    edi, [ebx + eax]
        mov    ax, word ptr [ebp - 6]
        sub    ax, word ptr [ebp - 18h]
        mov    word ptr [ebp - 6], ax
        add    esi, ebx
        mov    ecx, dword ptr [ebp - 14h]
        push   0
        mov    eax, dword ptr [ecx + 14h]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_075C
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [eax], 0
        mov    dword ptr [ecx + 14h], 0
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_FAC7
        xor    eax, eax
        db  066h, 0A1h, 0CAh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0D1h, 01Eh, 0FDh, 0FFh
        add    esp, 8
        jmp    NEAR PTR loc_1_FAC7
        db  005h, 000h, 000h, 000h, 000h
loc_1_FD70:
        cmp    byte ptr [ebp - 2], 8
        jb     NEAR PTR loc_1_FA9F
        cmp    byte ptr [ebp - 1], 2
        je     SHORT loc_1_FD89
        lea    esi, [ebx + esi - 2]
        jmp    NEAR PTR loc_1_FABB
loc_1_FD89:
        mov    cx, word ptr [esi]
        mov    ah, cl
        mov    al, ch
        mov    ecx, dword ptr [ebp - 1ch]
        mov    word ptr [ecx + 112h], ax
        mov    cx, word ptr [esi + 2]
        mov    ah, cl
        mov    al, ch
        mov    ecx, dword ptr [ebp - 1ch]
        mov    word ptr [ecx + 114h], ax
        mov    cx, word ptr [esi + 4]
        mov    ah, cl
        mov    al, ch
        mov    ecx, dword ptr [ebp - 1ch]
        mov    word ptr [ecx + 116h], ax
        mov    al, byte ptr [ebp - 2]
        sub    al, 8
        mov    byte ptr [ecx + 138h], al
        je     SHORT loc_1_FDE2
        and    eax, 0ffh
        push   eax
        lea    eax, [esi + 6]
        push   eax
        mov    eax, ecx
        add    eax, 118h
        push   eax
        call   sub_1_083C
loc_1_FDE2:
        mov    ecx, dword ptr [ebp - 1ch]
        mov    al, byte ptr [ebp - 3]
        mov    byte ptr [ecx + 141h], 1
        mov    byte ptr [edi], al
        inc    edi
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [edi], al
        inc    edi
        lea    eax, [ebx - 2]
        push   eax
        push   esi
        push   edi
        call   sub_1_083C
        lea    edi, [ebx + edi - 2]
        lea    esi, [ebx + esi - 2]
        jmp    NEAR PTR loc_1_FABB
loc_1_FE10:
        cmp    byte ptr [ebp - 2], 2
        jne    NEAR PTR loc_1_FA9F
        mov    eax, dword ptr [ebp - 30h]
        cmp    byte ptr [eax + 143h], 0
        jne    SHORT loc_1_FE67
        mov    byte ptr [eax + 143h], 1
        mov    dword ptr [eax + 13ch], 1
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 10h], 7
        jne    SHORT loc_1_FE67
        mov    byte ptr [eax + 10h], 6
        mov    byte ptr [eax + 11h], 4
        push   eax
        call   PPPMAC_Service_07
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   PPPMAC_Service_08
        add    esp, 4
        cmp    byte ptr [ebp - 1], 2
        je     NEAR PTR loc_1_FF19
loc_1_FE67:
        cmp    byte ptr [ebp - 1], 2
        je     SHORT loc_1_FE76
        lea    esi, [ebx + esi - 2]
        jmp    NEAR PTR loc_1_FABB
loc_1_FE76:
        mov    ecx, dword ptr [ebp - 1ch]
        mov    al, byte ptr [ebp - 3]
        mov    dword ptr [ecx + 13ch], 1
        mov    byte ptr [ecx + 143h], 1
        lea    esi, [ebx + esi - 2]
        mov    byte ptr [edi], al
        inc    edi
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [edi], al
        inc    edi
        jmp    NEAR PTR loc_1_FABB
        db  090h
loc_1_FEA0:
        mov    eax, dword ptr [ebp + 18h]
        pop    edi
        mov    word ptr [eax], 0
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_FEB3:
        mov    eax, dword ptr [ebp + 18h]
        pop    edi
        mov    word ptr [eax], 0
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_FEC6:
        mov    eax, dword ptr [ebp + 18h]
        pop    edi
        mov    word ptr [eax], 0
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_FEE0:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_FF06
        xor    eax, eax
        db  066h, 0A1h, 0CCh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 031h, 01Dh, 0FDh, 0FFh
        add    esp, 8
loc_1_FF06:
        mov    eax, dword ptr [ebp + 18h]
        pop    edi
        mov    word ptr [eax], 0
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_FF19:
        mov    eax, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    word ptr [eax], 0
        push   ecx
        call   sub_1_F420
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_FF40:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 174h
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 4
        cmp    byte ptr [ebx + 140h], 0
        je     NEAR PTR loc_1_10003
        xor    eax, eax
        mov    ax, word ptr [ebx + 110h]
        sub    ecx, ecx
        cmp    eax, 0eh
        jle    SHORT loc_1_FFB5
        mov    ecx, edx
        inc    edx
        cmp    byte ptr [ecx], 1
        jne    NEAR PTR loc_1_1014E
        mov    ecx, edx
        inc    edx
        cmp    byte ptr [ecx], 0f0h
        jne    NEAR PTR loc_1_1014E
        mov    ecx, 3bh
        mov    esi, edx
        mov    edi, ebx
        repe cmpsd
        jne    NEAR PTR loc_1_1014E
        mov    cx, word ptr [esi]
        cmp    cx, word ptr [edi]
        jne    NEAR PTR loc_1_1014E
        add    edx, 0eeh
        mov    ecx, 0eh
        sub    eax, 0eh
loc_1_FFB5:
        mov    esi, edx
        inc    edx
        cmp    byte ptr [esi], 1
        jne    NEAR PTR loc_1_1014E
        mov    esi, edx
        inc    edx
        mov    edi, eax
        shl    edi, 4
        add    eax, edi
        mov    edi, eax
        movzx  esi, byte ptr [esi]
        sub    edi, esi
        cmp    edi, -2
        jne    NEAR PTR loc_1_1014E
        mov    edi, ecx
        shl    edi, 4
        add    edi, ecx
        mov    ecx, eax
        shr    ecx, 2
        mov    esi, edx
        lea    edi, [edi + ebx]
        repe cmpsd
        jne    NEAR PTR loc_1_1014E
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb
        jne    NEAR PTR loc_1_1014E
        add    edx, eax
loc_1_10003:
        cmp    byte ptr [ebx + 141h], 0
        je     NEAR PTR loc_1_100EA
        mov    eax, edx
        inc    edx
        cmp    byte ptr [eax], 2
        jne    NEAR PTR loc_1_1014E
        mov    esi, edx
        mov    cl, byte ptr [ebx + 138h]
        xor    eax, eax
        mov    al, cl
        inc    edx
        mov    edi, eax
        movzx  esi, byte ptr [esi]
        sub    edi, esi
        cmp    edi, -8
        jne    NEAR PTR loc_1_1014E
        mov    si, word ptr [ebx + 112h]
        mov    di, si
        and    edi, 0ffh
        shl    edi, 8
        shr    si, 8
        and    esi, 0ffffh
        or     edi, esi
        xor    esi, esi
        mov    si, word ptr [edx]
        cmp    edi, esi
        jne    NEAR PTR loc_1_1014E
        mov    si, word ptr [ebx + 114h]
        mov    di, si
        and    edi, 0ffh
        shl    edi, 8
        shr    si, 8
        and    esi, 0ffffh
        or     edi, esi
        xor    esi, esi
        mov    si, word ptr [edx + 2]
        cmp    edi, esi
        jne    NEAR PTR loc_1_1014E
        mov    si, word ptr [ebx + 116h]
        mov    di, si
        and    edi, 0ffh
        shl    edi, 8
        shr    si, 8
        and    esi, 0ffffh
        or     edi, esi
        xor    esi, esi
        mov    si, word ptr [edx + 4]
        cmp    edi, esi
        jne    NEAR PTR loc_1_1014E
        add    edx, 6
        or     cl, cl
        je     SHORT loc_1_100E0
        lea    edi, [ebx + 118h]
        mov    ecx, eax
        shr    ecx, 2
        mov    esi, edx
        repe cmpsd
        jne    SHORT loc_1_1014E
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb
        jne    SHORT loc_1_1014E
loc_1_100E0:
        xor    eax, eax
        mov    al, byte ptr [ebx + 138h]
        add    edx, eax
loc_1_100EA:
        cmp    byte ptr [ebx + 142h], 0
        je     SHORT loc_1_10138
        mov    eax, edx
        inc    edx
        cmp    byte ptr [eax], 3
        jne    SHORT loc_1_1014E
        mov    eax, edx
        inc    edx
        cmp    byte ptr [eax], 5
        jne    SHORT loc_1_1014E
        mov    ax, word ptr [ebx + 139h]
        mov    cx, ax
        and    ecx, 0ffh
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0ffffh
        or     ecx, eax
        xor    eax, eax
        mov    ax, word ptr [edx]
        cmp    ecx, eax
        jne    SHORT loc_1_1014E
        mov    al, byte ptr [ebx + 13bh]
        cmp    byte ptr [edx + 2], al
        jne    SHORT loc_1_1014E
        add    edx, 3
loc_1_10138:
        cmp    byte ptr [ebx + 143h], 0
        je     SHORT loc_1_10157
        mov    eax, edx
        inc    edx
        cmp    byte ptr [eax], 4
        jne    SHORT loc_1_1014E
        cmp    byte ptr [edx], 2
        je     SHORT loc_1_10157
loc_1_1014E:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_1_10157:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
sub_1_10160:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 174h
        add    dword ptr [ebp + 0ch], 4
        sub    word ptr [ebp + 10h], 4
        je     NEAR PTR loc_1_103B9
loc_1_10181:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        cmp    eax, 1
        je     SHORT loc_1_101A4
        cmp    eax, 2
        je     NEAR PTR loc_1_10290
loc_1_10199:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_101A4:
        cmp    byte ptr [ebx + 140h], 0
        je     NEAR PTR loc_1_10266
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 2
        mov    dword ptr [ebp - 4], eax
        mov    cl, byte ptr [eax + 10h]
        mov    eax, dword ptr [ebp + 0ch]
        xor    edx, edx
        mov    dl, byte ptr [eax + 1]
        add    eax, edx
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 8], eax
        jbe    NEAR PTR loc_1_10276
loc_1_101D6:
        or     cl, cl
        je     SHORT loc_1_10251
        mov    eax, dword ptr [ebp - 4]
        cmp    byte ptr [eax + 0fh], 3
        jne    NEAR PTR loc_1_10407
        sub    edx, edx
        cmp    word ptr [ebx + 110h], dx
        jbe    SHORT loc_1_10213
        mov    eax, ebx
loc_1_101F4:
        mov    ecx, 4
        mov    esi, dword ptr [ebp - 4]
        mov    edi, eax
        repe cmpsd
        je     SHORT loc_1_10213
        add    eax, 11h
        inc    edx
        xor    ecx, ecx
        mov    cx, word ptr [ebx + 110h]
        cmp    ecx, edx
        ja     SHORT loc_1_101F4
loc_1_10213:
        xor    esi, esi
        mov    si, word ptr [ebx + 110h]
        cmp    edx, esi
        je     NEAR PTR loc_1_103D2
        sub    esi, edx
        cmp    esi, 1
        je     SHORT loc_1_1024A
        mov    eax, edx
        shl    eax, 4
        add    eax, edx
        lea    ecx, [ebx + eax]
        mov    eax, esi
        shl    eax, 4
        add    eax, esi
        sub    eax, 11h
        push   eax
        lea    eax, [ecx + 11h]
        push   eax
        push   ecx
        call   sub_1_083C
loc_1_1024A:
        dec    word ptr [ebx + 110h]
loc_1_10251:
        add    dword ptr [ebp - 4], 11h
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 8], eax
        mov    cl, byte ptr [eax + 10h]
        ja     NEAR PTR loc_1_101D6
        jmp    SHORT loc_1_10276
loc_1_10266:
        mov    byte ptr [ebx + 140h], 1
        mov    word ptr [ebx + 110h], 0
loc_1_10276:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffff00ffh
        sub    word ptr [ebp + 10h], ax
        jmp    NEAR PTR loc_1_103A6
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_10290:
        cmp    byte ptr [ebx + 141h], 0
        je     NEAR PTR loc_1_10390
        xor    edx, edx
        mov    dl, byte ptr [ebx + 138h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, edx
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        sub    ecx, eax
        cmp    ecx, -8
        jne    NEAR PTR loc_1_10199
        mov    ax, word ptr [ebx + 112h]
        mov    cx, ax
        and    ecx, 0ffh
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0ffffh
        or     ecx, eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        cmp    ecx, eax
        jne    NEAR PTR loc_1_10199
        mov    ax, word ptr [ebx + 114h]
        mov    cx, ax
        and    ecx, 0ffh
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0ffffh
        or     ecx, eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        cmp    ecx, eax
        jne    NEAR PTR loc_1_10199
        mov    ax, word ptr [ebx + 116h]
        mov    cx, ax
        and    ecx, 0ffh
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0ffffh
        or     ecx, eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        cmp    ecx, eax
        jne    NEAR PTR loc_1_10199
        cmp    byte ptr [ebx + 138h], 0
        je     SHORT loc_1_10382
        mov    esi, dword ptr [ebp + 0ch]
        add    esi, 8
        lea    edi, [ebx + 118h]
        mov    ecx, edx
        shr    ecx, 2
        repe cmpsd
        jne    NEAR PTR loc_1_10199
        mov    ecx, edx
        and    ecx, 3
        repe cmpsb
        jne    NEAR PTR loc_1_10199
loc_1_10382:
        mov    byte ptr [ebx + 141h], 0
        jmp    SHORT loc_1_10397
        db  005h, 000h, 000h, 000h, 000h
loc_1_10390:
        mov    byte ptr [ebx + 141h], 1
loc_1_10397:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffff00ffh
        add    word ptr [ebp + 10h], ax
loc_1_103A6:
        and    eax, 0ffffh
        add    dword ptr [ebp + 0ch], eax
        cmp    word ptr [ebp + 10h], 0
        jne    NEAR PTR loc_1_10181
loc_1_103B9:
        cmp    word ptr [ebp + 10h], 0
        jne    NEAR PTR loc_1_10199
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_103D2:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_10199
        xor    eax, eax
        db  066h, 0A1h, 0CEh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 03Bh, 018h, 0FDh, 0FFh
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_10407:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_1042D
        xor    eax, eax
        db  066h, 0A1h, 0D0h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 00Ah, 018h, 0FDh, 0FFh
        add    esp, 8
loc_1_1042D:
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h
sub_1_10440:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 174h
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 4
        mov    dx, word ptr [ebp + 10h]
        sub    dx, 4
        cmp    byte ptr [ebx + 140h], 0
        je     NEAR PTR loc_1_10549
        mov    cl, byte ptr [eax]
        cmp    cl, 1
        jne    NEAR PTR loc_1_10549
        xor    edi, edi
        mov    di, word ptr [ebx + 110h]
        sub    esi, esi
        mov    dword ptr [ebp - 4], edi
        cmp    edi, 0eh
        jle    SHORT loc_1_104D1
        inc    eax
        cmp    cl, 1
        jne    NEAR PTR loc_1_106DC
        mov    ecx, eax
        inc    eax
        cmp    byte ptr [ecx], 0f0h
        jne    NEAR PTR loc_1_106DC
        mov    ecx, 3bh
        mov    esi, eax
        mov    edi, ebx
        repe cmpsd
        jne    NEAR PTR loc_1_106DC
        mov    cx, word ptr [esi]
        cmp    cx, word ptr [edi]
        jne    NEAR PTR loc_1_106DC
        add    eax, 0eeh
        mov    esi, 0eh
        sub    dword ptr [ebp - 4], 0eh
        sub    dx, 0f0h
loc_1_104D1:
        mov    ecx, eax
        inc    eax
        cmp    byte ptr [ecx], 1
        jne    NEAR PTR loc_1_106DC
        mov    dword ptr [ebp - 18h], eax
        mov    ecx, dword ptr [ebp - 4]
        shl    ecx, 4
        add    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], ecx
        mov    edi, ecx
        mov    ecx, dword ptr [ebp - 18h]
        inc    eax
        mov    cl, byte ptr [ecx]
        and    ecx, 0ffh
        sub    edi, ecx
        cmp    edi, -2
        jne    NEAR PTR loc_1_106DC
        mov    edi, esi
        shl    edi, 4
        add    edi, esi
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 14h], ecx
        shr    ecx, 2
        mov    esi, eax
        lea    edi, [edi + ebx]
        repe cmpsd
        jne    NEAR PTR loc_1_106DC
        mov    ecx, dword ptr [ebp - 14h]
        and    ecx, 3
        repe cmpsb
        jne    NEAR PTR loc_1_106DC
        mov    ecx, dword ptr [ebp - 4]
        add    eax, dword ptr [ebp - 8]
        imul   cx, cx, 0ffefh
        sub    cx, 2
        add    dx, cx
        mov    byte ptr [ebx + 140h], 0
loc_1_10549:
        cmp    byte ptr [ebx + 141h], 0
        je     NEAR PTR loc_1_10657
        cmp    byte ptr [eax], 2
        jne    NEAR PTR loc_1_10657
        xor    ecx, ecx
        mov    cl, byte ptr [ebx + 138h]
        movzx  esi, byte ptr [eax + 1]
        mov    dword ptr [ebp - 10h], ecx
        sub    ecx, esi
        cmp    ecx, -8
        jne    NEAR PTR loc_1_106DC
        mov    cx, word ptr [ebx + 112h]
        mov    si, cx
        and    esi, 0ffh
        shl    esi, 8
        shr    cx, 8
        and    ecx, 0ffffh
        or     esi, ecx
        xor    ecx, ecx
        mov    cx, word ptr [eax + 2]
        cmp    esi, ecx
        jne    NEAR PTR loc_1_106DC
        mov    cx, word ptr [ebx + 114h]
        mov    si, cx
        and    esi, 0ffh
        shl    esi, 8
        shr    cx, 8
        and    ecx, 0ffffh
        or     esi, ecx
        xor    ecx, ecx
        mov    cx, word ptr [eax + 4]
        cmp    esi, ecx
        jne    NEAR PTR loc_1_106DC
        mov    cx, word ptr [ebx + 116h]
        mov    si, cx
        and    esi, 0ffh
        shl    esi, 8
        shr    cx, 8
        and    ecx, 0ffffh
        or     esi, ecx
        xor    ecx, ecx
        mov    cx, word ptr [eax + 6]
        cmp    esi, ecx
        jne    NEAR PTR loc_1_106DC
        add    eax, 8
        sub    dx, 8
        cmp    byte ptr [ebx + 138h], 0
        je     SHORT loc_1_10650
        lea    edi, [ebx + 118h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 0ch], ecx
        shr    ecx, 2
        mov    esi, eax
        repe cmpsd
        jne    NEAR PTR loc_1_106DC
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 3
        repe cmpsb
        jne    NEAR PTR loc_1_106DC
        mov    cl, byte ptr [ebx + 138h]
        and    ecx, 0ffff00ffh
        mov    esi, ecx
        and    esi, 0ffffh
        add    eax, esi
        sub    dx, cx
loc_1_10650:
        mov    byte ptr [ebx + 141h], 0
loc_1_10657:
        cmp    byte ptr [ebx + 142h], 0
        je     SHORT loc_1_106AD
        cmp    byte ptr [eax], 3
        jne    SHORT loc_1_106AD
        cmp    byte ptr [eax + 1], 5
        jne    SHORT loc_1_106DC
        mov    cx, word ptr [ebx + 139h]
        mov    si, cx
        and    esi, 0ffh
        shl    esi, 8
        shr    cx, 8
        and    ecx, 0ffffh
        or     esi, ecx
        xor    ecx, ecx
        mov    cx, word ptr [eax + 2]
        cmp    esi, ecx
        jne    SHORT loc_1_106DC
        mov    cl, byte ptr [ebx + 13bh]
        cmp    byte ptr [eax + 4], cl
        jne    SHORT loc_1_106DC
        add    eax, 5
        sub    dx, 5
        mov    byte ptr [ebx + 142h], 0
loc_1_106AD:
        cmp    byte ptr [ebx + 143h], 0
        je     SHORT loc_1_106CC
        cmp    byte ptr [eax], 4
        jne    SHORT loc_1_106CC
        cmp    byte ptr [eax + 1], 2
        jne    SHORT loc_1_106DC
        sub    dx, 2
        mov    byte ptr [ebx + 143h], 0
loc_1_106CC:
        or     dx, dx
        jne    SHORT loc_1_106DC
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_106DC:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_106F0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        add    ebx, 4bh
        cmp    dword ptr [esi + 8], 29ch
        jae    SHORT loc_1_10720
        mov    dword ptr [esi + 8], 29ch
        mov    dword ptr [esi + 0ch], 80bh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_10720:
        lea    edi, [esi + 14h]
        push   144h
        lea    eax, [ebx + 174h]
        push   eax
        push   edi
        call   sub_1_083C
        push   144h
        add    ebx, 2b8h
        push   ebx
        add    edi, 144h
        push   edi
        call   sub_1_083C
        mov    dword ptr [esi + 0ch], 800h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_10760:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 4]
        add    ebx, 4bh
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [ebx], 0
        mov    edx, dword ptr [eax + 20h]
        mov    dword ptr [ebp - 0ch], edx
        lea    eax, [ebx + 30h]
        mov    dword ptr [ebp - 18h], eax
        jne    SHORT loc_1_107BD
        mov    dword ptr [ebx + 4], 19ch
        db  0A1h, 050h, 082h, 001h, 000h
        push   eax
        push   0
        push   19ch
        push   ebx
        call   sub_1_0756
        mov    eax, dword ptr [ebx]
        or     eax, eax
        jne    SHORT loc_1_107B4
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_107B4:
        mov    dword ptr [ebx + 8], eax
        add    eax, 40h
        mov    dword ptr [ebx + 0ch], eax
loc_1_107BD:
        mov    esi, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [ebp - 8], eax
        mov    edi, esi
        sub    eax, eax
        mov    ecx, 10h
        rep stosd
        mov    edi, dword ptr [ebp - 8]
        mov    ecx, 57h
        rep stosd
        db  0E8h, 09Dh, 012h, 0FDh, 0FFh
        or     ax, ax
        jne    SHORT loc_1_107F3
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_107F3:
        mov    byte ptr [ebp - 1], 0
        mov    edi, eax
        and    edi, 0ffffh
        nop
loc_1_10800:
        mov    eax, 1
        mov    cl, byte ptr [ebp - 1]
        shl    eax, cl
        test   edi, eax
        je     SHORT loc_1_1087A
        mov    byte ptr [esi], 33h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 4], eax
        mov    al, cl
        mov    byte ptr [esi + 30h], al
        lea    eax, [esi + 0ah]
        mov    word ptr [esi + 8], 15ch
        mov    dword ptr [eax], 20202020h
        mov    dword ptr [eax + 4], 20202020h
        mov    dword ptr [eax + 8], 20202020h
        mov    dword ptr [eax + 0ch], 20202020h
        mov    byte ptr [eax], 2ah
        mov    byte ptr [esi + 0bh], 0
        mov    dword ptr [esi + 3ch], 0
        push   esi
        db  0E8h, 03Bh, 012h, 0FDh, 0FFh
        add    esp, 4
        cmp    byte ptr [esi + 1], 0
        jne    SHORT loc_1_1087A
        sub    ecx, ecx
        nop
loc_1_10860:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [ecx + eax]
        mov    edx, dword ptr [ebp - 0ch]
        cmp    byte ptr [ecx + edx + 8], al
        jne    SHORT loc_1_10875
        inc    ecx
        cmp    ecx, 6
        jl     SHORT loc_1_10860
loc_1_10875:
        cmp    ecx, 6
        je     SHORT loc_1_10895
loc_1_1087A:
        inc    byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 1], 10h
        jb     NEAR PTR loc_1_10800
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_10895:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 3ah]
        cmp    ax, 10h
        jbe    SHORT loc_1_108B0
        mov    eax, 802h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_108B0:
        mov    dword ptr [ebp - 14h], 0
        mov    dword ptr [ebp - 10h], 0
        or     ax, ax
        je     NEAR PTR loc_1_1094B
        mov    edi, dword ptr [ebp - 8]
        add    edi, 4dh
        mov    esi, dword ptr [ebp - 18h]
        add    esi, 10h
loc_1_108D3:
        mov    al, byte ptr [edi]
        and    al, 7
        cmp    al, 4
        jne    SHORT loc_1_10934
        push   10h
        lea    eax, [edi - 11h]
        push   eax
        lea    eax, [esi - 10h]
        push   eax
        call   sub_1_083C
        test   byte ptr [edi], 80h
        mov    byte ptr [esi], 2
        jne    SHORT loc_1_108F5
        mov    byte ptr [esi], 1
loc_1_108F5:
        cmp    byte ptr [ebx + 168h], 0
        jne    SHORT loc_1_1092E
        cmp    byte ptr [edi - 2], 0
        jne    SHORT loc_1_1092E
        sub    eax, eax
        cmp    byte ptr [edi - 11h], 20h
        je     SHORT loc_1_10928
        mov    edx, 20h
loc_1_10911:
        cmp    eax, 0fh
        jge    SHORT loc_1_10928
        mov    cl, byte ptr [edi + eax - 11h]
        mov    byte ptr [eax + ebx + 148h], cl
        inc    eax
        cmp    byte ptr [edi + eax - 11h], dl
        jne    SHORT loc_1_10911
loc_1_10928:
        mov    byte ptr [ebx + 168h], al
loc_1_1092E:
        add    esi, 11h
        inc    dword ptr [ebp - 14h]
loc_1_10934:
        add    edi, 12h
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 3ah]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 10h]
        jg     SHORT loc_1_108D3
loc_1_1094B:
        mov    ax, word ptr [ebp - 14h]
        mov    word ptr [ebx + 140h], ax
        mov    eax, 800h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_10970:
        push   ebp
        mov    ebp, esp
        sub    esp, 30h
        push   edi
        db  083h, 03Dh, 020h, 08Ch, 001h, 000h, 001h
        jne    SHORT loc_1_109C4
        db  0E8h, 025h, 011h, 0FDh, 0FFh
        or     eax, eax
        jne    SHORT loc_1_10990
        sub    al, al
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret
loc_1_10990:
        lea    edi, [ebp - 30h]
        sub    eax, eax
        mov    ecx, 0ch
        rep stosd
        db  0C7h, 005h, 020h, 08Ch, 001h, 000h, 002h, 000h, 000h, 000h
        lea    eax, [ebp - 30h]
        push   eax
        push   1
        db  0E8h, 06Dh, 011h, 0FDh, 0FFh
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_109D7
        call   sub_1_109E0
        sub    al, al
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret
loc_1_109C4:
        db  083h, 03Dh, 020h, 08Ch, 001h, 000h, 004h
        jne    SHORT loc_1_109D7
        db  0C7h, 005h, 020h, 08Ch, 001h, 000h, 003h, 000h, 000h, 000h
loc_1_109D7:
        mov    al, 1
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
sub_1_109E0:
        db  083h, 03Dh, 020h, 08Ch, 001h, 000h, 002h
        jne    SHORT loc_1_10A00
        db  0E8h, 019h, 011h, 0FDh, 0FFh
        db  0C7h, 005h, 020h, 08Ch, 001h, 000h, 001h, 000h, 000h, 000h
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_10A00:
        db  083h, 03Dh, 020h, 08Ch, 001h, 000h, 003h
        jne    SHORT loc_1_10A13
        db  0C7h, 005h, 020h, 08Ch, 001h, 000h, 004h, 000h, 000h, 000h
loc_1_10A13:
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_10A20:
        push   ebp
        mov    ebp, esp
        sub    esp, 44h
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        db  083h, 03Dh, 020h, 08Ch, 001h, 000h, 002h
        lea    esi, [ecx + 4bh]
        je     SHORT loc_1_10A40
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_10A40:
        mov    eax, dword ptr [esi + 24h]
        cmp    eax, 2
        jne    SHORT loc_1_10A51
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_10A51:
        cmp    eax, 1
        je     SHORT loc_1_10A60
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_10A60:
        mov    dword ptr [ebp - 14h], 0
        mov    dword ptr [ebp - 10h], 1
        db  0C7h, 045h, 0F4h, 020h, 00Ch, 001h, 000h
        mov    dword ptr [ebp - 8], ecx
        lea    eax, [ebp - 14h]
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 34h], eax
        mov    dword ptr [ebp - 30h], 14h
        lea    eax, [ebp - 44h]
        push   eax
        push   2
        db  0E8h, 087h, 010h, 0FDh, 0FFh
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_10AA7
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_10AA7:
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [esi + 28h], eax
        mov    dword ptr [esi + 24h], 2
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_10AC0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 6fh], 2
        lea    esi, [eax + 4bh]
        je     SHORT loc_1_10AD7
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
loc_1_10AD7:
        cmp    dword ptr [esi + 20h], 0
        jne    SHORT loc_1_10AFC
        push   eax
        db  068h, 010h, 00Bh, 001h, 000h
        call   sub_1_02D4
        add    esp, 8
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], 3
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
loc_1_10AFC:
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_10B10:
        push   ebp
        mov    ebp, esp
        sub    esp, 3ch
        mov    dword ptr [ebp - 4], edx
        cld
        mov    eax, dword ptr [ebp - 4]
        add    eax, 4bh
        mov    dword ptr [eax + 20h], 0
        mov    ecx, dword ptr [eax + 18h]
        mov    eax, dword ptr [eax + 28h]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 8], ecx
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 2ch], eax
        mov    dword ptr [ebp - 28h], 8
        lea    eax, [ebp - 3ch]
        push   eax
        push   5
        db  0E8h, 0D3h, 00Fh, 0FDh, 0FFh
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_10B67
        push   1
        push   467h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   0
        call   sub_1_10C20
        add    esp, 10h
loc_1_10B67:
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_1_10B70:
        push   ebp
        mov    ebp, esp
        sub    esp, 38h
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 24h], 4
        jne    SHORT loc_1_10BB3
        mov    eax, dword ptr [esi + 28h]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 4], 11h
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 28h], eax
        mov    dword ptr [ebp - 24h], 8
        lea    eax, [ebp - 38h]
        push   eax
        push   3
        db  0E8h, 079h, 00Fh, 0FDh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_10BB3
        mov    dword ptr [esi + 24h], 5
loc_1_10BB3:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_10BC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 34h
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 24h]
        cmp    eax, 3
        jb     SHORT loc_1_10C14
        cmp    eax, 5
        ja     SHORT loc_1_10C14
        mov    eax, dword ptr [esi + 28h]
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 24h], eax
        mov    dword ptr [ebp - 20h], 4
        mov    dword ptr [esi + 24h], 6
        db  0C7h, 005h, 020h, 08Ch, 001h, 000h, 003h, 000h, 000h, 000h
        lea    eax, [ebp - 34h]
        push   eax
        push   4
        db  0E8h, 018h, 00Fh, 0FDh, 0FFh
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_10C14
        mov    dword ptr [esi + 24h], 1
loc_1_10C14:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_10C20:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 4bh
        mov    eax, dword ptr [ebp + 10h]
        cmp    eax, 467h
        je     SHORT loc_1_10C45
        cmp    eax, 468h
        je     SHORT loc_1_10C60
        cmp    eax, 469h
        je     SHORT loc_1_10C71
        pop    esi
        pop    ebp
        ret
loc_1_10C45:
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ecx + 2ch], eax
        mov    edx, dword ptr [ebp + 0ch]
        db  0BEh, 0C0h, 00Ch, 001h, 000h
        call   sub_1_0185
        pop    esi
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_10C60:
        mov    dword ptr [ecx + 24h], 1
        mov    dword ptr [ecx + 28h], 0
        pop    esi
        pop    ebp
        ret
loc_1_10C71:
        mov    dword ptr [ecx + 24h], 1
        mov    dword ptr [ecx + 28h], 0
        db  083h, 03Dh, 020h, 08Ch, 001h, 000h, 004h
        db  0C7h, 005h, 020h, 08Ch, 001h, 000h, 002h, 000h, 000h, 000h
        jne    SHORT loc_1_10CAE
        mov    edx, 0
        db  0BEh, 0E0h, 009h, 001h, 000h
        call   sub_1_0185
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 3fh], 800h
        pop    esi
        pop    ebp
        ret
loc_1_10CAE:
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_10CC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], edx
        cld
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    esi, dword ptr [ebp - 8]
        add    esi, 4bh
        mov    dword ptr [esi + 24h], 4
        mov    word ptr [ebp - 2], 5dch
        push   5dch
        push   eax
        call   PPPMAC_Service_02
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_10D63
        mov    ebx, dword ptr [edi + 20h]
        mov    edx, dword ptr [esi + 1ch]
        add    ebx, dword ptr [edi + 1ch]
        mov    cx, word ptr [edx + 2]
        mov    ah, cl
        mov    al, ch
        lea    ecx, [ebp - 2]
        push   ecx
        push   ebx
        push   eax
        push   edx
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_F9A0
        or     al, al
        mov    eax, dword ptr [ebp - 8]
        je     SHORT loc_1_10D30
        mov    byte ptr [eax + 11h], 6
        jmp    SHORT loc_1_10D34
        db  005h, 000h, 000h, 000h, 000h
loc_1_10D30:
        mov    byte ptr [eax + 11h], 7
loc_1_10D34:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 3bh], edi
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 35h], ebx
        mov    ecx, dword ptr [ebp - 8]
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [ecx + 39h], ax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   PPPMAC_Service_07
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   PPPMAC_Service_08
        add    esp, 4
loc_1_10D63:
        push   0
        mov    eax, dword ptr [esi + 14h]
        push   eax
        mov    eax, dword ptr [esi + 10h]
        push   eax
        call   sub_1_075C
        mov    dword ptr [esi + 10h], 0
        mov    dword ptr [esi + 14h], 0
        mov    dword ptr [esi + 18h], 0
        mov    dword ptr [esi + 1ch], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_10DA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, 2
        mov    eax, dword ptr [esi + 10h]
        mov    edi, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [eax + 0cch]
        mov    bx, word ptr [esi + 24h]
        mov    byte ptr [edi], 16h
        inc    edi
        mov    word ptr [ebp - 2], 2
        mov    byte ptr [edi], 16h
        inc    edi
        mov    al, byte ptr [ecx + 1ch]
        mov    byte ptr [edi], al
        inc    edi
        mov    al, byte ptr [ecx + 1dh]
        mov    byte ptr [edi], al
        inc    edi
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 4
        xor    eax, eax
        mov    al, byte ptr [ecx + 1ch]
        cmp    eax, 1
        je     SHORT loc_1_10E02
        cmp    eax, 0f2h
        je     SHORT loc_1_10E02
        cmp    eax, 0f3h
        je     SHORT loc_1_10E36
        jmp    SHORT loc_1_10E48
loc_1_10E02:
        mov    byte ptr [edi], bl
        inc    edi
        push   4
        mov    byte ptr [edi], bh
        inc    edi
        push   2
        mov    eax, dword ptr [esi + 1ch]
        push   eax
        call   sub_1_11ED0
        mov    byte ptr [edi], al
        mov    byte ptr [edi + 1], ah
        inc    edi
        inc    edi
        mov    eax, ebx
        and    eax, 0ffffh
        add    edi, eax
        add    eax, 4
        mov    word ptr [ebp - 2], 8
        add    dword ptr [esi + 24h], eax
        mov    dx, bx
        jmp    SHORT loc_1_10E48
loc_1_10E36:
        mov    eax, ebx
        and    eax, 0ffffh
        add    edi, eax
        add    eax, 4
        mov    dword ptr [esi + 24h], eax
        lea    edx, [ebx + 2]
loc_1_10E48:
        mov    eax, edx
        and    eax, 0ffffh
        push   eax
        xor    eax, eax
        mov    ax, word ptr [ebp - 2]
        push   eax
        mov    eax, dword ptr [esi + 1ch]
        push   eax
        call   sub_1_11ED0
        mov    byte ptr [edi], al
        mov    byte ptr [edi + 1], ah
        mov    byte ptr [edi + 2], 4
        inc    edi
        add    dword ptr [esi + 24h], 3
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_10E80:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 2], 0
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [ebp - 4], 0
        cmp    dword ptr [esi], 2
        je     NEAR PTR loc_1_112D7
        mov    edx, dword ptr [esi + 61h]
        mov    dword ptr [ebp - 8], edx
        mov    edi, dword ptr [esi + 0cch]
loc_1_10EB0:
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 24h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_39B0
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_10F13
        test   byte ptr [ebp - 10h], 1
        je     SHORT loc_1_10ED3
        inc    dword ptr [esi + 0c4h]
loc_1_10ED3:
        test   byte ptr [ebp - 10h], 2
        je     SHORT loc_1_10EDF
        inc    dword ptr [esi + 0bch]
loc_1_10EDF:
        test   byte ptr [ebp - 10h], 8
        je     SHORT loc_1_10EEB
        inc    dword ptr [esi + 0c0h]
loc_1_10EEB:
        mov    word ptr [edi + 18h], 5
        mov    dword ptr [esi + 65h], 2
        mov    dword ptr [esi + 6dh], 0
        mov    dword ptr [esi + 75h], 0eh
        mov    dword ptr [esi + 71h], 0eh
        mov    word ptr [ebp - 4], 0
loc_1_10F13:
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [esi + 65h]
        mov    edx, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [edx + 1ch]
        add    eax, dword ptr [esi + 75h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_3A70
        cmp    dword ptr [ebp - 0ch], 1
        sbb    eax, eax
        and    eax, 103h
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 0ch]
        add    dword ptr [esi + 6dh], eax
        mov    eax, dword ptr [ebp - 0ch]
        add    dword ptr [esi + 75h], eax
        mov    eax, dword ptr [ebp - 0ch]
        sub    dword ptr [esi + 65h], eax
        mov    eax, dword ptr [ebp - 0ch]
        add    dword ptr [esi + 0a0h], eax
        cmp    dword ptr [esi + 65h], 0
        jne    NEAR PTR loc_1_11240
        mov    edx, dword ptr [ebp - 8]
        mov    cx, word ptr [esi + 6dh]
        mov    ebx, dword ptr [edx + 1ch]
        xor    eax, eax
        add    ebx, dword ptr [esi + 71h]
        mov    ax, word ptr [edi + 18h]
        or     eax, eax
        je     SHORT loc_1_10FA0
        cmp    eax, 1
        je     SHORT loc_1_10FD0
        cmp    eax, 2
        je     NEAR PTR loc_1_11140
        cmp    eax, 3
        je     NEAR PTR loc_1_111C1
        cmp    eax, 5
        je     NEAR PTR loc_1_11210
        jmp    NEAR PTR loc_1_11240
        db  08Dh, 049h, 000h
loc_1_10FA0:
        cmp    byte ptr [ebx], 16h
        jne    SHORT loc_1_10FC0
        cmp    byte ptr [ebx + 1], 16h
        jne    SHORT loc_1_10FC0
        mov    word ptr [edi + 18h], 1
        mov    dword ptr [esi + 65h], 2
        jmp    NEAR PTR loc_1_11240
        db  08Dh, 049h, 000h
loc_1_10FC0:
        mov    word ptr [ebp - 2], 1
        jmp    NEAR PTR loc_1_11240
        db  005h, 000h, 000h, 000h, 000h
loc_1_10FD0:
        mov    al, byte ptr [ebx + 2]
        mov    byte ptr [edi + 1ah], al
        and    eax, 0ffh
        cmp    eax, 16h
        jg     SHORT loc_1_11010
        je     NEAR PTR loc_1_11090
        cmp    eax, 1
        je     SHORT loc_1_11040
        cmp    eax, 6
        je     SHORT loc_1_11062
        cmp    eax, 0ah
        jl     SHORT loc_1_1102F
        cmp    eax, 0ch
        jle    SHORT loc_1_11062
        cmp    eax, 15h
        je     SHORT loc_1_11062
        mov    word ptr [ebp - 2], 1
        jmp    NEAR PTR loc_1_11240
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_11010:
        cmp    eax, 21h
        je     NEAR PTR loc_1_110D0
        cmp    eax, 0f2h
        je     NEAR PTR loc_1_110F0
        cmp    eax, 0f3h
        je     NEAR PTR loc_1_11110
loc_1_1102F:
        mov    word ptr [ebp - 2], 1
        jmp    NEAR PTR loc_1_11240
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_11040:
        mov    eax, ecx
        and    eax, 0ffffh
        mov    ecx, 8
        sub    ecx, eax
        mov    dword ptr [esi + 65h], ecx
        mov    word ptr [edi + 18h], 2
        mov    word ptr [ebp - 4], 1
        jmp    NEAR PTR loc_1_11240
loc_1_11062:
        mov    eax, ecx
        and    eax, 0ffffh
        mov    ecx, 7
        sub    ecx, eax
        mov    dword ptr [esi + 65h], ecx
        mov    word ptr [edi + 18h], 3
        mov    eax, dword ptr [esi + 75h]
        mov    edx, dword ptr [ebp - 8]
        sub    eax, 2
        mov    dword ptr [edx + 20h], eax
        mov    al, byte ptr [ebx + 3]
        mov    byte ptr [edi + 1bh], al
        jmp    NEAR PTR loc_1_11240
loc_1_11090:
        cmp    byte ptr [ebx + 2], 16h
        jne    SHORT loc_1_110C0
        cmp    byte ptr [ebx + 3], 16h
        jne    SHORT loc_1_110C0
        mov    word ptr [edi + 18h], 1
        mov    dword ptr [esi + 6dh], 2
        sub    dword ptr [esi + 75h], 2
        mov    dword ptr [esi + 65h], 2
        jmp    NEAR PTR loc_1_11240
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_110C0:
        mov    word ptr [ebp - 2], 1
        jmp    NEAR PTR loc_1_11240
        db  005h, 000h, 000h, 000h, 000h
loc_1_110D0:
        mov    eax, ecx
        and    eax, 0ffffh
        mov    ecx, 9
        sub    ecx, eax
        mov    dword ptr [esi + 65h], ecx
        mov    word ptr [edi + 18h], 2
        jmp    NEAR PTR loc_1_11240
        db  08Dh, 064h, 024h, 000h
loc_1_110F0:
        mov    eax, ecx
        and    eax, 0ffffh
        mov    ecx, 8
        sub    ecx, eax
        mov    dword ptr [esi + 65h], ecx
        mov    word ptr [edi + 18h], 2
        jmp    NEAR PTR loc_1_11240
        db  08Dh, 064h, 024h, 000h
loc_1_11110:
        mov    eax, ecx
        and    eax, 0ffffh
        mov    ecx, 9
        sub    ecx, eax
        mov    dword ptr [esi + 65h], ecx
        mov    word ptr [edi + 18h], 3
        mov    eax, dword ptr [esi + 75h]
        mov    edx, dword ptr [ebp - 8]
        sub    eax, 2
        mov    dword ptr [edx + 20h], eax
        mov    al, byte ptr [ebx + 3]
        mov    byte ptr [edi + 1bh], al
        jmp    NEAR PTR loc_1_11240
        db  08Bh, 0FFh
loc_1_11140:
        mov    eax, ecx
        and    eax, 0ffffh
        mov    dword ptr [ebp - 14h], eax
        sub    eax, 4
        push   eax
        push   2
        push   ebx
        call   sub_1_11F00
        or     ax, ax
        jne    SHORT loc_1_111B0
        mov    ecx, dword ptr [ebp - 14h]
        add    ecx, ebx
        xor    edx, edx
        mov    dl, byte ptr [ecx - 3]
        xor    eax, eax
        mov    al, byte ptr [ecx - 4]
        shl    edx, 8
        or     edx, eax
        add    edx, 3
        mov    dword ptr [esi + 65h], edx
        mov    eax, dword ptr [esi + 6dh]
        add    eax, edx
        cmp    eax, 267h
        ja     SHORT loc_1_111A0
        mov    word ptr [edi + 18h], 3
        mov    edx, dword ptr [ebp - 8]
        mov    eax, dword ptr [esi + 75h]
        mov    dword ptr [edx + 20h], eax
        mov    al, byte ptr [ecx - 5]
        mov    byte ptr [edi + 1bh], al
        jmp    NEAR PTR loc_1_11240
        db  005h, 000h, 000h, 000h, 000h
loc_1_111A0:
        mov    word ptr [ebp - 2], 1
        jmp    NEAR PTR loc_1_11240
        db  005h, 000h, 000h, 000h, 000h
loc_1_111B0:
        mov    word ptr [ebp - 2], 1
        inc    dword ptr [esi + 0b0h]
        jmp    NEAR PTR loc_1_11240
loc_1_111C1:
        mov    eax, ecx
        and    eax, 0ffffh
        cmp    byte ptr [ebx + eax - 1], 4
        je     SHORT loc_1_111D7
        mov    word ptr [ebp - 2], 1
        jmp    SHORT loc_1_11240
loc_1_111D7:
        mov    edx, dword ptr [ebp - 8]
        mov    eax, dword ptr [esi + 75h]
        sub    eax, dword ptr [edx + 20h]
        sub    eax, 3
        mov    dword ptr [edx + 24h], eax
        push   eax
        mov    eax, dword ptr [edx + 20h]
        sub    eax, dword ptr [esi + 71h]
        push   eax
        push   ebx
        call   sub_1_11F00
        or     ax, ax
        jne    SHORT loc_1_11201
        mov    word ptr [edi + 18h], 4
        jmp    SHORT loc_1_11240
loc_1_11201:
        mov    word ptr [ebp - 2], 1
        inc    dword ptr [esi + 0b0h]
        jmp    SHORT loc_1_11240
        db  090h
loc_1_11210:
        cmp    byte ptr [ebx], 16h
        jne    SHORT loc_1_1122A
        cmp    byte ptr [ebx + 1], 16h
        jne    SHORT loc_1_1122A
        mov    word ptr [edi + 18h], 1
        mov    dword ptr [esi + 65h], 2
        jmp    SHORT loc_1_11240
loc_1_1122A:
        mov    al, byte ptr [ebx + 1]
        mov    byte ptr [ebx], al
        mov    dword ptr [esi + 65h], 1
        mov    dword ptr [esi + 6dh], 1
        dec    dword ptr [esi + 75h]
loc_1_11240:
        cmp    word ptr [ebp - 2], 0
        je     SHORT loc_1_11288
        cmp    word ptr [ebp - 4], 0
        je     SHORT loc_1_11260
        mov    al, byte ptr [edi + 1bh]
        push   eax
        push   15h
        push   esi
        call   sub_1_11950
        mov    word ptr [ebp - 4], 0
loc_1_11260:
        mov    word ptr [ebp - 2], 0
        mov    word ptr [edi + 18h], 5
        mov    dword ptr [esi + 65h], 2
        mov    dword ptr [esi + 6dh], 0
        mov    dword ptr [esi + 75h], 0eh
        mov    dword ptr [esi + 71h], 0eh
loc_1_11288:
        cmp    word ptr [edi + 18h], 4
        jne    SHORT loc_1_112C7
        push   esi
        call   sub_1_11550
        mov    dword ptr [esi + 65h], 2
        mov    dword ptr [esi + 6dh], 0
        mov    dword ptr [esi + 75h], 0eh
        mov    dword ptr [esi + 71h], 0eh
        inc    dword ptr [esi + 0a8h]
        mov    word ptr [edi + 18h], 0
        mov    byte ptr [edi + 1ah], 0
        mov    word ptr [ebp - 4], 0
loc_1_112C7:
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_1_10EB0
        push   esi
        call   sub_1_46D0
loc_1_112D7:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_1_112E0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    ebx, dword ptr [esi + 0feh]
        call   sub_1_11380
        or     al, al
        jne    SHORT loc_1_11302
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_11302:
        mov    edi, dword ptr [esi + 0cch]
        mov    word ptr [edi + 2], 1
        mov    eax, dword ptr [ebx + 4]
        sub    eax, 2
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        mov    word ptr [edi], ax
        push   esi
        call   sub_1_11410
        cmp    word ptr [edi], 0
        jne    SHORT loc_1_11353
        mov    word ptr [edi + 8], 3
        push   esi
        call   sub_1_118B0
        mov    word ptr [edi + 0ah], 1
        push   2710h
        add    edi, 0ch
        push   edi
        call   sub_1_0732
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_11353:
        mov    byte ptr [edi + 6], 1
        mov    byte ptr [edi + 7], 2
        mov    word ptr [edi + 0ah], 2
        push   7530h
        add    edi, 0ch
        push   edi
        call   sub_1_0732
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_11380:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        db  0A1h, 050h, 082h, 001h, 000h
        lea    edi, [esi + 0cch]
        push   eax
        push   0
        push   23h
        push   edi
        call   sub_1_0756
        mov    eax, dword ptr [edi]
        or     eax, eax
        jne    SHORT loc_1_113B0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_113B0:
        mov    word ptr [eax], 0
        mov    word ptr [eax + 2], 0
        mov    word ptr [eax + 8], 0ah
        mov    word ptr [eax + 4], 0
        mov    byte ptr [eax + 6], 2
        mov    byte ptr [eax + 7], 1
        mov    word ptr [eax + 18h], 0
        mov    byte ptr [eax + 1ah], 0
        mov    byte ptr [eax + 1bh], 0
        mov    byte ptr [eax + 1ch], 0
        mov    byte ptr [eax + 1dh], 0
        mov    byte ptr [eax + 1eh], 1
        mov    dword ptr [eax + 0ch], 0
        db  0C7h, 040h, 010h, 060h, 01Dh, 001h, 000h
        mov    dword ptr [eax + 14h], esi
        mov    word ptr [eax + 0ah], 0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_11410:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 20h]
        mov    eax, dword ptr [ecx + 61h]
        mov    esi, dword ptr [ecx + 0cch]
        mov    dword ptr [eax + 0ch], edx
        mov    dword ptr [eax + 10h], ecx
        mov    eax, dword ptr [edx + 0c3h]
        mov    dword ptr [esi + 1fh], eax
        mov    dword ptr [edx + 0c3h], 258h
        mov    dword ptr [ecx + 18h], 258h
        mov    dword ptr [ecx + 1ch], 267h
        mov    byte ptr [ecx + 7bh], 0
        mov    byte ptr [ecx + 7ah], 0
        mov    byte ptr [ecx + 79h], 0
        mov    dword ptr [ecx + 65h], 2
        mov    dword ptr [ecx + 6dh], 0
        mov    dword ptr [ecx + 75h], 0eh
        mov    dword ptr [ecx + 71h], 0eh
        push   ecx
        call   sub_1_10E80
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  090h
sub_1_11480:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 0cch]
        or     edx, edx
        je     SHORT loc_1_114C7
        mov    ecx, dword ptr [esi + 20h]
        mov    eax, dword ptr [edx + 1fh]
        mov    dword ptr [ecx + 0c3h], eax
        lea    eax, [ebp - 1]
        push   eax
        add    edx, 0ch
        push   edx
        call   sub_1_0738
        push   0
        push   23h
        mov    eax, dword ptr [esi + 0cch]
        push   eax
        call   sub_1_075C
        mov    dword ptr [esi + 0cch], 0
loc_1_114C7:
        sub    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_1_114D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0cch]
        or     edi, edi
        jne    SHORT loc_1_114F0
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
loc_1_114F0:
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [edi + 0ch]
        push   eax
        call   sub_1_0738
        push   2b00h
        push   esi
        call   PPPMAC_Service_06
        add    esp, 8
        cmp    dword ptr [ebp + 0ch], 0
        jne    SHORT loc_1_11540
        cmp    word ptr [edi + 2], 2
        jne    SHORT loc_1_11540
        mov    word ptr [edi + 2], 3
        mov    al, byte ptr [edi + 1eh]
        dec    al
        push   eax
        push   0bh
        push   esi
        call   sub_1_11950
        mov    eax, 103h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_11540:
        mov    word ptr [edi + 2], 4
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
sub_1_11550:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 0cch]
        mov    ax, word ptr [esi + 4]
        or     ax, 1
        mov    word ptr [esi + 4], ax
        and    ax, 0bfffh
        mov    word ptr [esi + 4], ax
        xor    eax, eax
        mov    ax, word ptr [esi + 2]
        cmp    eax, 1
        je     SHORT loc_1_115A0
        cmp    eax, 2
        je     NEAR PTR loc_1_11629
        cmp    eax, 3
        je     NEAR PTR loc_1_1175B
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_115A0:
        xor    eax, eax
        mov    al, byte ptr [esi + 1ah]
        cmp    eax, 0bh
        je     SHORT loc_1_115C6
        cmp    eax, 15h
        je     SHORT loc_1_115E1
        cmp    eax, 0f2h
        je     SHORT loc_1_115F0
        cmp    eax, 0f3h
        je     SHORT loc_1_11610
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_115C6:
        mov    al, byte ptr [esi + 1bh]
        push   eax
        push   6
        push   edi
        call   sub_1_11950
        mov    word ptr [esi + 2], 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_115E1:
        mov    al, byte ptr [esi + 1bh]
        mov    byte ptr [esi + 1eh], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_115F0:
        cmp    word ptr [esi], 0
        je     NEAR PTR loc_1_117AC
        push   edi
        call   sub_1_11A90
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_11610:
        cmp    word ptr [esi], 0
        jne    NEAR PTR loc_1_117AC
        push   edi
        call   sub_1_117C0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_11629:
        xor    eax, eax
        mov    al, byte ptr [esi + 1ah]
        cmp    eax, 0ch
        jg     SHORT loc_1_11651
        je     NEAR PTR loc_1_116F0
        cmp    eax, 1
        je     SHORT loc_1_11673
        cmp    eax, 0ah
        je     SHORT loc_1_11682
        cmp    eax, 0bh
        je     SHORT loc_1_116B9
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_11651:
        cmp    eax, 15h
        je     NEAR PTR loc_1_11730
        cmp    eax, 21h
        je     SHORT loc_1_11673
        cmp    eax, 0f3h
        je     NEAR PTR loc_1_11740
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_11673:
        push   edi
        call   sub_1_11990
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_11682:
        lea    ebx, [esi + 0ch]
        lea    eax, [ebp - 1]
        push   eax
        push   ebx
        call   sub_1_0738
        and    word ptr [esi + 4], 7fffh
        push   1
        push   0ch
        push   edi
        call   sub_1_11950
        mov    word ptr [esi + 0ah], 3
        push   9c40h
        push   ebx
        call   sub_1_0732
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_116B9:
        mov    al, byte ptr [esi + 1bh]
        push   eax
        push   6
        push   edi
        call   sub_1_11950
        mov    word ptr [esi + 2], 4
        push   2b00h
        push   edi
        call   PPPMAC_Service_06
        add    esp, 8
        push   1
        push   edi
        call   sub_1_1690
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_116F0:
        cmp    word ptr [esi + 0ah], 4
        jne    NEAR PTR loc_1_117AC
        lea    edi, [esi + 0ch]
        lea    eax, [ebp - 1]
        push   eax
        push   edi
        call   sub_1_0738
        mov    word ptr [esi + 8], 0ah
        and    word ptr [esi + 4], 7fffh
        mov    word ptr [esi + 0ah], 3
        push   2710h
        push   edi
        call   sub_1_0732
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_11730:
        mov    al, byte ptr [esi + 1bh]
        mov    byte ptr [esi + 1eh], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_11740:
        cmp    word ptr [esi], 0
        jne    SHORT loc_1_117AC
        mov    al, byte ptr [esi + 1bh]
        push   eax
        push   6
        push   edi
        call   sub_1_11950
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1175B:
        xor    eax, eax
        mov    al, byte ptr [esi + 1ah]
        cmp    eax, 6
        je     SHORT loc_1_1176F
        cmp    eax, 0bh
        je     SHORT loc_1_11781
        cmp    eax, 0ch
        jne    SHORT loc_1_117AC
loc_1_1176F:
        push   edi
        call   sub_1_16C0
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_11781:
        mov    al, byte ptr [esi + 1bh]
        push   eax
        push   6
        push   edi
        call   sub_1_11950
        mov    word ptr [esi + 2], 4
        push   2b00h
        push   edi
        call   PPPMAC_Service_06
        add    esp, 8
        push   1
        push   edi
        call   sub_1_1690
        add    esp, 8
loc_1_117AC:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_117C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 61h]
        mov    edi, dword ptr [esi + 0cch]
        mov    ebx, dword ptr [eax + 1ch]
        add    ebx, dword ptr [eax + 20h]
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [edi + 0ch]
        push   eax
        call   sub_1_0738
        cmp    byte ptr [ebx + 3], 0
        je     SHORT loc_1_11820
        mov    al, byte ptr [edi + 1bh]
        push   eax
        push   6
        push   esi
        call   sub_1_11950
        mov    word ptr [edi + 2], 3
        mov    dword ptr [esi + 0d0h], 803h
        push   esi
        call   PPPMAC_Service_09
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_11820:
        mov    word ptr [edi + 2], 2
        mov    dword ptr [esi + 0d0h], 800h
        push   esi
        call   PPPMAC_Service_09
        add    esp, 4
        mov    al, byte ptr [ebx + 2]
        mov    byte ptr [edi + 6], al
        lea    eax, [esi + 90h]
        mov    dword ptr [eax], 0
        mov    word ptr [eax + 4], 0
        mov    al, byte ptr [ebx + 2]
        mov    byte ptr [esi + 93h], al
        mov    al, byte ptr [edi + 1bh]
        push   eax
        push   6
        push   esi
        call   sub_1_11950
        mov    word ptr [edi + 8], 0ah
        mov    word ptr [edi + 4], 0c000h
        mov    al, byte ptr [edi + 1eh]
        dec    al
        push   eax
        push   0ah
        push   esi
        call   sub_1_11950
        mov    word ptr [edi + 0ah], 4
        push   0bb8h
        lea    eax, [edi + 0ch]
        push   eax
        call   sub_1_0732
        push   2b00h
        push   esi
        call   PPPMAC_Service_05
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_118B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    esi, dword ptr [eax + 0cch]
        mov    edi, dword ptr [eax + 0feh]
        call   sub_1_11FD0
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_1_11948
        mov    byte ptr [esi + 1ch], 0f2h
        mov    al, byte ptr [esi + 1eh]
        mov    byte ptr [esi + 1dh], al
        inc    byte ptr [esi + 1eh]
        mov    esi, dword ptr [ebx + 1ch]
        add    esi, 8
        mov    dword ptr [ebx + 20h], 8
        mov    byte ptr [esi], 0
        mov    eax, dword ptr [edi + 20h]
        push   eax
        mov    eax, dword ptr [edi + 30h]
        push   eax
        inc    esi
        push   esi
        call   sub_1_083C
        mov    eax, dword ptr [edi + 20h]
        mov    byte ptr [esi + eax], 0
        mov    eax, dword ptr [edi + 20h]
        inc    eax
        add    esi, eax
        mov    eax, dword ptr [edi + 28h]
        push   eax
        mov    eax, dword ptr [edi + 38h]
        push   eax
        push   esi
        call   sub_1_083C
        mov    eax, dword ptr [edi + 28h]
        mov    byte ptr [esi + eax], 0
        mov    eax, dword ptr [edi + 28h]
        push   eax
        push   esi
        call   sub_1_11F50
        mov    eax, dword ptr [edi + 28h]
        inc    eax
        add    esi, eax
        mov    byte ptr [esi], 0
        inc    esi
        push   1
        mov    byte ptr [esi], 0
        inc    esi
        sub    esi, dword ptr [ebx + 1ch]
        sub    esi, 8
        mov    dword ptr [ebx + 24h], esi
        push   ebx
        call   sub_1_4360
loc_1_11948:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  090h
sub_1_11950:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    esi, dword ptr [eax + 0cch]
        call   sub_1_11FD0
        or     eax, eax
        je     SHORT loc_1_11989
        mov    dword ptr [eax + 20h], 0
        mov    dword ptr [eax + 24h], 0
        mov    cl, byte ptr [ebp + 0ch]
        mov    byte ptr [esi + 1ch], cl
        mov    cl, byte ptr [ebp + 10h]
        mov    byte ptr [esi + 1dh], cl
        push   1
        push   eax
        call   sub_1_4360
loc_1_11989:
        pop    esi
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_1_11990:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 61h]
        mov    eax, dword ptr [esi + 0cch]
        cmp    dword ptr [edi + 24h], 4
        jb     SHORT loc_1_11A18
        mov    al, byte ptr [eax + 1bh]
        push   eax
        push   6
        push   esi
        call   sub_1_11950
        add    dword ptr [edi + 20h], 4
        sub    dword ptr [edi + 24h], 4
        mov    ecx, dword ptr [edi + 1ch]
        mov    eax, dword ptr [edi + 20h]
        lea    ebx, [ecx + eax - 0eh]
        push   6
        lea    eax, [esi + 90h]
        push   eax
        push   ebx
        call   sub_1_083C
        push   6
        lea    eax, [esi + 96h]
        push   eax
        lea    eax, [ebx + 6]
        push   eax
        call   sub_1_083C
        mov    ecx, dword ptr [edi + 24h]
        mov    al, ch
        mov    ah, cl
        mov    word ptr [ebx + 0ch], ax
        lea    ecx, [ebx + 0eh]
        mov    eax, dword ptr [edi + 24h]
        push   eax
        push   eax
        push   ecx
        push   0eh
        push   ebx
        push   ebx
        mov    eax, dword ptr [esi + 20h]
        push   ecx
        mov    eax, dword ptr [eax + 3eh]
        push   eax
        call   sub_1_0300
        mov    eax, dword ptr [esi + 20h]
        mov    eax, dword ptr [eax + 3eh]
        push   eax
        call   sub_1_04E0
loc_1_11A18:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  090h
sub_1_11A20:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 10h]
        mov    edx, dword ptr [eax + 0cch]
        mov    byte ptr [edx + 1ch], 1
        mov    al, byte ptr [edx + 1eh]
        mov    byte ptr [edx + 1dh], al
        inc    byte ptr [edx + 1eh]
        mov    eax, dword ptr [esi + 1ch]
        add    eax, 8
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_1_11A50
        mov    byte ptr [eax], 0
        jmp    SHORT loc_1_11A55
        db  08Bh, 0FFh
loc_1_11A50:
        mov    cl, byte ptr [edx + 7]
        mov    byte ptr [eax], cl
loc_1_11A55:
        inc    eax
        mov    cl, byte ptr [edx + 6]
        mov    byte ptr [eax], cl
        mov    cl, byte ptr [esi + 25h]
        mov    byte ptr [eax + 1], cl
        mov    cl, byte ptr [esi + 24h]
        mov    byte ptr [eax + 2], cl
        inc    eax
        mov    dword ptr [esi + 20h], 8
        add    dword ptr [esi + 24h], 4
        mov    ax, word ptr [edx + 4]
        or     ax, 2
        mov    word ptr [edx + 4], ax
        and    ax, 0bfffh
        mov    word ptr [edx + 4], ax
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_11A90:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [ebp - 1]
        mov    edi, dword ptr [esi + 61h]
        push   eax
        mov    eax, dword ptr [esi + 0cch]
        add    eax, 0ch
        push   eax
        call   sub_1_0738
        mov    eax, dword ptr [edi + 24h]
        add    eax, 25h
        mov    dword ptr [ebp - 0ch], eax
        db  0A1h, 050h, 082h, 001h, 000h
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_0756
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_11B00
        mov    dword ptr [esi + 0d0h], 803h
        mov    dword ptr [esi + 0d4h], 0
        push   esi
        call   PPPMAC_Service_09
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_11B00:
        mov    eax, dword ptr [edi + 1ch]
        mov    ecx, dword ptr [edi + 20h]
        lea    ecx, [eax + ecx + 1]
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 18h], 0
        mov    eax, ecx
        inc    ecx
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_11B3E
loc_1_11B20:
        mov    edx, dword ptr [ebp - 8]
        mov    eax, dword ptr [edx + 18h]
        add    edx, 18h
        mov    ebx, dword ptr [edi + 24h]
        sub    ebx, 5
        cmp    ebx, eax
        jbe    SHORT loc_1_11B3E
        inc    eax
        mov    dword ptr [edx], eax
        mov    eax, ecx
        inc    ecx
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_11B20
loc_1_11B3E:
        mov    eax, dword ptr [ebp - 8]
        mov    edx, dword ptr [edi + 24h]
        sub    edx, dword ptr [eax + 18h]
        sub    edx, 5
        mov    dword ptr [eax + 20h], edx
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 20h]
        push   eax
        push   ecx
        call   sub_1_11F90
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 8], 1
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 10h], 800h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 1ch], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 14h], 0
        mov    ebx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [edi + 1ch]
        mov    eax, dword ptr [edi + 20h]
        lea    edi, [ecx + eax + 1]
        mov    eax, dword ptr [ebp - 8]
        add    ebx, 28h
        mov    eax, dword ptr [eax + 18h]
        push   eax
        push   edi
        push   ebx
        call   sub_1_083C
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 18h]
        add    ebx, eax
        lea    edi, [edi + eax + 1]
        mov    byte ptr [ebx], 0
        mov    eax, dword ptr [ebp - 8]
        inc    ebx
        mov    eax, dword ptr [eax + 20h]
        push   eax
        push   edi
        push   ebx
        call   sub_1_083C
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 20h]
        mov    byte ptr [ebx + eax], 0
        mov    dword ptr [esi + 0d0h], 800h
        mov    dword ptr [esi + 0d4h], 900h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0d8h], eax
        mov    dword ptr [esi + 0dch], esi
        db  0C7h, 086h, 0E0h, 000h, 000h, 000h, 010h, 01Ch, 001h, 000h
        push   esi
        call   PPPMAC_Service_09
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_11C10:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0cch]
        mov    ecx, dword ptr [esi + 0feh]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [esi + 0d8h]
        or     eax, eax
        je     SHORT loc_1_11C4B
        push   0
        mov    ecx, dword ptr [eax + 0ch]
        push   ecx
        push   eax
        call   sub_1_075C
        mov    dword ptr [esi + 0d8h], 0
loc_1_11C4B:
        mov    dword ptr [esi + 0d0h], 800h
        mov    dword ptr [esi + 0d4h], 0
        mov    dword ptr [esi + 0dch], 0
        mov    ebx, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 0e0h], 0
        mov    dword ptr [ebx + 10h], 801h
        push   esi
        call   sub_1_11FD0
        or     eax, eax
        je     NEAR PTR loc_1_11D56
        mov    byte ptr [edi + 1ch], 0f3h
        mov    cl, byte ptr [edi + 1eh]
        mov    byte ptr [edi + 1dh], cl
        inc    byte ptr [edi + 1eh]
        mov    dword ptr [eax + 20h], 4
        mov    dword ptr [eax + 24h], 2
        mov    edx, dword ptr [eax + 1ch]
        add    edx, 4
        cmp    dword ptr [ebx + 18h], 1
        cmc
        sbb    cl, cl
        and    cl, 2
        mov    byte ptr [edx], cl
        cmp    dword ptr [ebx + 18h], 1
        sbb    cl, cl
        and    cl, 4
        mov    byte ptr [edx + 1], cl
        push   1
        push   eax
        call   sub_1_4360
        cmp    dword ptr [ebx + 18h], 0
        mov    ecx, dword ptr [ebp - 4]
        je     SHORT loc_1_11D40
        mov    dword ptr [ecx + 10h], 1
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [ecx + 14h], eax
        mov    eax, dword ptr [ebx + 20h]
        add    edi, 0ch
        mov    dword ptr [ecx + 18h], eax
        mov    eax, dword ptr [ebx + 24h]
        mov    dword ptr [ecx + 1ch], eax
        mov    word ptr [edi - 0ah], 2
        mov    word ptr [edi - 2], 3
        push   9c40h
        push   edi
        call   sub_1_0732
        push   2b00h
        mov    eax, dword ptr [esi + 20h]
        push   eax
        call   sub_1_15E0
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_11D2D
        mov    ecx, dword ptr [ebx + 1ch]
        push   ecx
        mov    eax, dword ptr [eax + 4ah]
        push   eax
        db  0E8h, 063h, 0FCh, 0FCh, 0FFh
        add    esp, 8
loc_1_11D2D:
        push   2b00h
        push   esi
        call   PPPMAC_Service_05
        add    esp, 8
        jmp    SHORT loc_1_11D4D
        db  08Dh, 049h, 000h
loc_1_11D40:
        mov    dword ptr [ecx + 10h], 0
        mov    word ptr [edi + 2], 3
loc_1_11D4D:
        push   esi
        call   PPPMAC_Service_09
        add    esp, 4
loc_1_11D56:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
loc_1_11D60:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [edi + 0cch]
        cmp    word ptr [esi + 2], 4
        je     NEAR PTR loc_1_11EC4
        xor    eax, eax
        mov    ax, word ptr [esi + 0ah]
        cmp    eax, 1
        je     SHORT loc_1_11DA1
        cmp    eax, 2
        je     SHORT loc_1_11DF0
        cmp    eax, 3
        je     NEAR PTR loc_1_11E10
        cmp    eax, 4
        je     NEAR PTR loc_1_11E75
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
loc_1_11DA1:
        mov    ax, word ptr [esi + 8]
        or     ax, ax
        je     SHORT loc_1_11DD0
        dec    ax
        mov    word ptr [esi + 8], ax
        mov    word ptr [esi + 0ah], 1
        push   2710h
        add    esi, 0ch
        push   esi
        call   sub_1_0732
        push   edi
        call   sub_1_118B0
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
loc_1_11DD0:
        mov    word ptr [esi + 2], 3
        mov    dword ptr [edi + 0d0h], 802h
        push   edi
        call   PPPMAC_Service_09
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
        db  090h
loc_1_11DF0:
        mov    word ptr [esi + 2], 3
        mov    dword ptr [edi + 0d0h], 803h
        push   edi
        call   PPPMAC_Service_09
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
        db  090h
loc_1_11E10:
        mov    ax, word ptr [esi + 4]
        cmp    ax, 4000h
        je     SHORT loc_1_11E45
        mov    ax, word ptr [esi]
        dec    ax
        cmp    ax, 1
        mov    word ptr [esi + 4], 4000h
        sbb    eax, eax
        and    eax, 2710h
        add    eax, 2710h
        push   eax
        add    esi, 0ch
        push   esi
        call   sub_1_0732
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
loc_1_11E45:
        or     ax, 8000h
        mov    word ptr [esi + 4], ax
        mov    al, byte ptr [esi + 1eh]
        dec    al
        push   eax
        push   0ah
        push   edi
        call   sub_1_11950
        mov    word ptr [esi + 0ah], 4
        push   2710h
        add    esi, 0ch
        push   esi
        call   sub_1_0732
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
loc_1_11E75:
        mov    ax, word ptr [esi + 8]
        and    word ptr [esi + 4], 7fffh
        lea    ecx, [eax - 1]
        mov    word ptr [esi + 8], cx
        or     ax, ax
        je     SHORT loc_1_11EA5
        mov    word ptr [esi + 0ah], 3
        push   2710h
        add    esi, 0ch
        push   esi
        call   sub_1_0732
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
loc_1_11EA5:
        mov    word ptr [esi + 2], 4
        push   2b00h
        push   edi
        call   PPPMAC_Service_06
        add    esp, 8
        push   2
        push   edi
        call   sub_1_1690
        add    esp, 8
loc_1_11EC4:
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_11ED0:
        push   ebp
        mov    ebp, esp
        push   esi
        sub    ax, ax
        mov    esi, dword ptr [ebp + 8]
        add    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        or     ecx, ecx
        je     SHORT loc_1_11EF3
loc_1_11EE4:
        mov    dl, byte ptr [esi]
        and    edx, 0ffff00ffh
        sub    ax, dx
        inc    esi
        dec    ecx
        jne    SHORT loc_1_11EE4
loc_1_11EF3:
        pop    esi
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_11F00:
        push   ebp
        mov    ebp, esp
        push   esi
        sub    dx, dx
        mov    esi, dword ptr [ebp + 8]
        add    esi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 10h]
        or     eax, eax
        je     SHORT loc_1_11F23
loc_1_11F14:
        mov    cl, byte ptr [esi]
        and    ecx, 0ffff00ffh
        sub    dx, cx
        inc    esi
        dec    eax
        jne    SHORT loc_1_11F14
loc_1_11F23:
        lea    ecx, [esi + eax]
        mov    al, byte ptr [ecx + 1]
        and    eax, 0ffff00ffh
        shl    ax, 8
        mov    cl, byte ptr [ecx]
        and    ecx, 0ffff00ffh
        or     ax, cx
        sub    ax, dx
        pop    esi
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_11F50:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    bl, 0abh
        sub    dl, dl
        mov    esi, dword ptr [ebp + 0ch]
        or     esi, esi
        je     SHORT loc_1_11F7F
        mov    eax, dword ptr [ebp + 8]
loc_1_11F64:
        xor    ecx, ecx
        mov    cl, dl
        lea    edi, [eax + ecx]
        mov    cl, byte ptr [edi]
        xor    cl, bl
        mov    byte ptr [edi], cl
        mov    bl, dl
        xor    bl, cl
        inc    dl
        xor    ecx, ecx
        mov    cl, dl
        cmp    ecx, esi
        jb     SHORT loc_1_11F64
loc_1_11F7F:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_11F90:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    dl, 0abh
        sub    cl, cl
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_1_11FC1
        mov    esi, dword ptr [ebp + 8]
loc_1_11FA3:
        xor    eax, eax
        mov    al, cl
        lea    edi, [esi + eax]
        mov    al, byte ptr [edi]
        mov    bl, al
        xor    bl, dl
        mov    byte ptr [edi], bl
        mov    dl, cl
        xor    dl, al
        inc    cl
        xor    eax, eax
        mov    al, cl
        cmp    eax, dword ptr [ebp + 0ch]
        jb     SHORT loc_1_11FA3
loc_1_11FC1:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_11FD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   0
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   0
        or     eax, eax
        je     SHORT loc_1_11FF1
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_11FF1:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 18h], 100h
        jae    SHORT loc_1_12010
        push   eax
        call   sub_1_0080
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_12010:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 2bh], 0
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_12030:
        mov    eax, 1
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_12040:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    cl, byte ptr [eax + 4bh]
        mov    eax, dword ptr [ebp + 0ch]
        pop    ebp
        mov    byte ptr [eax], cl
        mov    eax, 1
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_12060:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        add    edx, 4bh
        sub    eax, eax
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 0ch]
        cmp    ecx, 1
        je     SHORT loc_1_12081
        cmp    ecx, 2
        je     SHORT loc_1_120A7
        cmp    ecx, 3
        je     SHORT loc_1_120C5
        pop    ebp
        ret
loc_1_12081:
        cmp    byte ptr [ebp + 14h], 3
        jne    SHORT loc_1_120C8
        mov    ecx, dword ptr [ebp + 10h]
        cmp    word ptr [ecx], 23c2h
        jne    SHORT loc_1_120C8
        mov    cl, byte ptr [ecx + 2]
        cmp    cl, 80h
        je     SHORT loc_1_1209E
        cmp    cl, 5
        jne    SHORT loc_1_120C8
loc_1_1209E:
        mov    byte ptr [edx], cl
        mov    eax, 1
        pop    ebp
        ret
loc_1_120A7:
        cmp    byte ptr [ebp + 14h], 3
        jne    SHORT loc_1_120C8
        mov    ecx, dword ptr [ebp + 10h]
        cmp    word ptr [ecx], 23c2h
        jne    SHORT loc_1_120C8
        mov    dl, byte ptr [edx]
        cmp    byte ptr [ecx + 2], dl
        jne    SHORT loc_1_120C8
        mov    eax, 1
        pop    ebp
        ret
loc_1_120C5:
        mov    byte ptr [edx], 0
loc_1_120C8:
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_120D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4ch
        mov    dword ptr [ebp - 4ch], 4ch
        mov    word ptr [ebp - 48h], 1
        mov    word ptr [ebp - 46h], 23c2h
        mov    word ptr [ebp - 44h], 0
        mov    dword ptr [ebp - 42h], 0bch
        mov    byte ptr [ebp - 3eh], 0
        mov    byte ptr [ebp - 3dh], 0
        mov    dword ptr [ebp - 3ch], 0
        mov    dword ptr [ebp - 38h], 0
        db  0C7h, 045h, 0CCh, 090h, 021h, 001h, 000h
        db  0C7h, 045h, 0D0h, 0B0h, 021h, 001h, 000h
        mov    dword ptr [ebp - 2ch], 0
        mov    dword ptr [ebp - 28h], 0
        db  0C7h, 045h, 0DCh, 060h, 022h, 001h, 000h
        mov    dword ptr [ebp - 20h], 0
        db  0C7h, 045h, 0E4h, 080h, 023h, 001h, 000h
        xor    eax, eax
        db  0C7h, 045h, 0E8h, 0E0h, 023h, 001h, 000h
        db  0C7h, 045h, 0ECh, 000h, 033h, 001h, 000h
        mov    dword ptr [ebp - 10h], 0
        db  0C7h, 045h, 0F4h, 030h, 020h, 001h, 000h
        db  0C7h, 045h, 0F8h, 040h, 020h, 001h, 000h
        db  0C7h, 045h, 0FCh, 060h, 020h, 001h, 000h
        db  066h, 0A1h, 004h, 001h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        mov    dword ptr [ebp - 3ch], eax
        lea    eax, [ebp - 4ch]
        push   eax
        call   PPPMAC_Service_01
        add    esp, 4
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_12190:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        add    eax, 4bh
        pop    ebp
        mov    byte ptr [eax], 80h
        mov    dword ptr [eax + 1], 1
        mov    eax, 800h
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_121B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 4]
        mov    edi, dword ptr [eax + 0ch]
        lea    esi, [ecx + 4bh]
        cmp    dword ptr [edi + 0dch], ecx
        jne    SHORT loc_1_1220A
        mov    dword ptr [edi + 0d4h], 0
        mov    dword ptr [edi + 0dch], 0
        mov    dword ptr [edi + 0e0h], 0
        mov    eax, dword ptr [edi + 0d8h]
        or     eax, eax
        je     SHORT loc_1_12200
        push   0
        mov    ecx, dword ptr [eax + 0ch]
        push   ecx
        push   eax
        call   sub_1_075C
loc_1_12200:
        mov    dword ptr [edi + 0d8h], 0
loc_1_1220A:
        cmp    dword ptr [esi + 0a4h], 0
        je     SHORT loc_1_1222D
        mov    dword ptr [esi + 0a4h], 0
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 98h]
        push   eax
        call   sub_1_0738
loc_1_1222D:
        cmp    dword ptr [esi + 0b4h], 0
        je     SHORT loc_1_12250
        mov    dword ptr [esi + 0b4h], 0
        lea    eax, [ebp - 2]
        push   eax
        add    esi, 0a8h
        push   esi
        call   sub_1_0738
loc_1_12250:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_12260:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    eax, dword ptr [esi + 4]
        lea    edx, [esi + 4bh]
        mov    cx, word ptr [esi + 2bh]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [edx + 94h], ecx
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [edx + 90h], eax
        mov    dword ptr [edx + 0a4h], 0
        mov    dword ptr [edx + 0b4h], 0
        mov    dword ptr [edx + 98h], 0
        db  0C7h, 082h, 09Ch, 000h, 000h, 000h, 010h, 023h, 001h, 000h
        mov    dword ptr [edx + 0a0h], esi
        mov    dword ptr [edx + 0a8h], 0
        db  0C7h, 082h, 0ACh, 000h, 000h, 000h, 0D0h, 022h, 001h, 000h
        mov    dword ptr [edx + 0b0h], esi
        pop    esi
        pop    ebp
        ret
loc_1_122D0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    dword ptr [esi + 0ffh], 0
        lea    eax, [esi + 4bh]
        je     SHORT loc_1_12307
        mov    dword ptr [eax + 0b4h], 0
        mov    byte ptr [esi + 10h], 6
        mov    byte ptr [esi + 11h], 5
        push   esi
        call   PPPMAC_Service_07
        add    esp, 4
        push   esi
        call   PPPMAC_Service_08
        add    esp, 4
loc_1_12307:
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h
loc_1_12310:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    dword ptr [esi + 0efh], 0
        lea    ecx, [esi + 4bh]
        je     SHORT loc_1_1237A
        mov    dword ptr [ecx + 0a4h], 0
        mov    al, byte ptr [esi + 10h]
        cmp    al, 6
        jb     SHORT loc_1_1237A
        cmp    al, 8
        ja     SHORT loc_1_1237A
        mov    eax, dword ptr [ecx + 94h]
        or     eax, eax
        lea    edx, [eax - 1]
        mov    dword ptr [ecx + 94h], edx
        je     SHORT loc_1_12360
        push   esi
        call   sub_1_12BC0
        add    esp, 4
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_12360:
        mov    byte ptr [esi + 10h], 6
        mov    byte ptr [esi + 11h], 5
        push   esi
        call   PPPMAC_Service_07
        add    esp, 4
        push   esi
        call   PPPMAC_Service_08
        add    esp, 4
loc_1_1237A:
        pop    esi
        pop    ebp
        ret    10h
        db  090h
loc_1_12380:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 4]
        add    ecx, 4bh
        mov    edx, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 14h], 0
        mov    edx, dword ptr [edx + 0feh]
        jne    SHORT loc_1_123A7
        mov    eax, dword ptr [ebp + 0ch]
        pop    ebp
        mov    word ptr [eax], 0
        sub    al, al
        ret
loc_1_123A7:
        xor    eax, eax
        mov    al, byte ptr [ecx]
        cmp    eax, 80h
        je     SHORT loc_1_123C4
        mov    eax, dword ptr [ebp + 0ch]
        pop    ebp
        mov    word ptr [eax], 0
        cmp    word ptr [eax], 1
        sbb    al, al
        inc    al
        ret
loc_1_123C4:
        mov    ecx, dword ptr [edx + 2ch]
        mov    eax, dword ptr [ebp + 0ch]
        inc    cx
        mov    word ptr [eax], cx
        cmp    word ptr [eax], 1
        sbb    al, al
        inc    al
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_123E0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    eax, dword ptr [ebp + 8]
        lea    ecx, [eax + 4bh]
        mov    esi, dword ptr [eax + 4]
        mov    edx, dword ptr [esi + 0ch]
        cmp    dword ptr [esi + 14h], 0
        mov    edx, dword ptr [edx + 0feh]
        je     SHORT loc_1_12425
        mov    bl, byte ptr [eax + 14h]
        mov    byte ptr [eax + 15h], bl
        xor    eax, eax
        mov    al, byte ptr [ecx]
        cmp    eax, 80h
        jne    SHORT loc_1_12425
        mov    ecx, dword ptr [ebp + 0ch]
        mov    al, byte ptr [edx + 2ch]
        mov    byte ptr [ecx], al
        inc    ecx
        mov    eax, dword ptr [edx + 2ch]
        push   eax
        mov    eax, dword ptr [edx + 3ch]
        push   eax
        push   ecx
        call   sub_1_083C
loc_1_12425:
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_12430:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 4bh
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    esi, dword ptr [eax + 0ch]
        xor    ecx, ecx
        mov    eax, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0feh]
        mov    eax, dword ptr [eax + 2fh]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 11h], 10h
        mov    cx, word ptr [eax + 33h]
        mov    eax, dword ptr [ebp - 8]
        lea    edx, [ecx - 5]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, edx
        jg     NEAR PTR loc_1_1257B
        mov    eax, dword ptr [edi + 28h]
        add    eax, dword ptr [edi + 20h]
        lea    ebx, [ecx + eax + 19h]
        db  0A1h, 050h, 082h, 001h, 000h
        push   eax
        push   0
        push   ebx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0756
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_1257B
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ch], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 10h], 800h
        mov    eax, dword ptr [ebp - 8]
        mov    cl, byte ptr [eax + 1]
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 14h], cl
        mov    eax, dword ptr [ebp - 0ch]
        mov    cl, byte ptr [eax]
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 15h], cl
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [edi + 20h]
        mov    word ptr [ecx + 16h], ax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [edi + 28h]
        mov    word ptr [ecx + 18h], ax
        mov    eax, dword ptr [ebp - 8]
        mov    cl, byte ptr [eax + 4]
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1ah], cl
        mov    ebx, dword ptr [ebp - 4]
        add    ebx, 1ch
        mov    eax, dword ptr [edi + 20h]
        inc    eax
        push   eax
        mov    eax, dword ptr [edi + 30h]
        push   eax
        push   ebx
        call   sub_1_083C
        mov    eax, dword ptr [edi + 20h]
        inc    eax
        add    ebx, eax
        mov    eax, dword ptr [edi + 28h]
        inc    eax
        push   eax
        mov    eax, dword ptr [edi + 38h]
        push   eax
        push   ebx
        call   sub_1_083C
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edi + 28h]
        mov    ax, word ptr [eax + 33h]
        and    eax, 0ffffh
        sub    eax, 5
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 5
        push   eax
        lea    eax, [ebx + ecx + 1]
        push   eax
        call   sub_1_083C
        mov    dword ptr [esi + 0d0h], 800h
        mov    dword ptr [esi + 0d4h], 904h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0d8h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0dch], eax
        db  0C7h, 086h, 0E0h, 000h, 000h, 000h, 090h, 025h, 001h, 000h
        push   esi
        call   PPPMAC_Service_09
        add    esp, 4
loc_1_1257B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_12590:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 4]
        mov    esi, dword ptr [eax + 0ch]
        lea    ebx, [edi + 4bh]
        mov    dword ptr [esi + 0d0h], 801h
        mov    dword ptr [esi + 0d4h], 0
        mov    dword ptr [esi + 0dch], 0
        mov    dword ptr [esi + 0e0h], 0
        mov    eax, dword ptr [esi + 0d8h]
        or     eax, eax
        je     SHORT loc_1_125EA
        push   0
        mov    ecx, dword ptr [eax + 0ch]
        push   ecx
        push   eax
        call   sub_1_075C
        mov    dword ptr [esi + 0d8h], 0
loc_1_125EA:
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 10h], 801h
        mov    al, byte ptr [esi + 14h]
        mov    byte ptr [edi + 15h], al
        mov    al, byte ptr [esi + 14h]
        mov    byte ptr [ebx + 86h], al
        xor    eax, eax
        mov    ax, word ptr [esi + 16h]
        push   eax
        lea    eax, [esi + 18h]
        push   eax
        lea    eax, [ebx + 5]
        push   eax
        call   sub_1_083C
        mov    al, byte ptr [esi + 16h]
        mov    byte ptr [ebx + 85h], al
        push   edi
        call   sub_1_12630
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
sub_1_12630:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        lea    esi, [edi + 4bh]
        mov    eax, dword ptr [edi + 4]
        mov    ecx, dword ptr [eax + 0ch]
        mov    al, byte ptr [esi + 85h]
        mov    ebx, dword ptr [ecx + 0feh]
        and    eax, 0ffff00ffh
        mov    dx, word ptr [ebx + 20h]
        lea    eax, [eax + edx + 5]
        mov    word ptr [ebp - 2], ax
        xor    eax, eax
        mov    al, byte ptr [esi]
        cmp    eax, 80h
        jne    SHORT loc_1_1267D
        mov    eax, dword ptr [ebx + 24h]
        cmp    eax, 1
        sbb    edx, edx
        inc    edx
        add    dx, ax
        add    word ptr [ebp - 2], dx
loc_1_1267D:
        xor    eax, eax
        mov    ax, word ptr [ebp - 2]
        push   eax
        push   ecx
        call   PPPMAC_Service_02
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        or     eax, eax
        je     NEAR PTR loc_1_127AC
        mov    eax, dword ptr [edi + 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax + 0eh]
        mov    word ptr [ecx + 28h], ax
        mov    eax, dword ptr [ecx + 20h]
        add    eax, dword ptr [ecx + 1ch]
        mov    dword ptr [ebp - 8], eax
        mov    byte ptr [eax], 2
        inc    dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [esi + 86h]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 8]
        mov    ah, byte ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [ebp - 1]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 8], 2
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [esi + 85h]
        mov    byte ptr [ecx], al
        xor    eax, eax
        inc    dword ptr [ebp - 8]
        mov    al, byte ptr [esi + 85h]
        push   eax
        lea    eax, [esi + 5]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_083C
        xor    eax, eax
        mov    al, byte ptr [esi + 85h]
        add    dword ptr [ebp - 8], eax
        xor    eax, eax
        mov    al, byte ptr [esi]
        cmp    eax, 80h
        jne    SHORT loc_1_12731
        mov    eax, dword ptr [ebx + 24h]
        or     eax, eax
        je     SHORT loc_1_12731
        push   eax
        mov    eax, dword ptr [ebx + 34h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_083C
        mov    eax, dword ptr [ebx + 24h]
        add    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], 5ch
        inc    dword ptr [ebp - 8]
loc_1_12731:
        mov    eax, dword ptr [ebx + 20h]
        push   eax
        mov    eax, dword ptr [ebx + 30h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_083C
        mov    eax, dword ptr [ebx + 20h]
        add    dword ptr [ebp - 8], eax
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [edi + 3bh], ecx
        mov    eax, dword ptr [ecx + 20h]
        add    eax, dword ptr [ecx + 1ch]
        mov    dword ptr [edi + 35h], eax
        mov    eax, dword ptr [ecx + 24h]
        mov    word ptr [edi + 39h], ax
        mov    al, byte ptr [esi + 86h]
        mov    byte ptr [edi + 15h], al
        mov    byte ptr [edi + 11h], 6
        push   edi
        call   PPPMAC_Service_07
        add    esp, 4
        push   edi
        call   PPPMAC_Service_08
        add    esp, 4
        mov    dword ptr [edi + 3bh], 0
        mov    dword ptr [edi + 35h], 0
        mov    word ptr [edi + 39h], 0
        push   1388h
        lea    eax, [esi + 98h]
        push   eax
        call   sub_1_0732
        mov    dword ptr [esi + 0a4h], 1
loc_1_127AC:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_127C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        lea    ecx, [esi + 4bh]
        mov    eax, dword ptr [esi + 2fh]
        mov    dl, byte ptr [esi + 15h]
        cmp    byte ptr [eax + 1], dl
        je     SHORT loc_1_127E1
        mov    byte ptr [esi + 11h], 10h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_1_127E1:
        cmp    dword ptr [ecx + 0a4h], 0
        je     SHORT loc_1_12804
        mov    dword ptr [ecx + 0a4h], 0
        lea    eax, [ebp - 1]
        push   eax
        add    ecx, 98h
        push   ecx
        call   sub_1_0738
loc_1_12804:
        cmp    byte ptr [esi + 10h], 9
        je     SHORT loc_1_12813
        mov    byte ptr [esi + 11h], 8
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_1_12813:
        mov    byte ptr [esi + 11h], 10h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
sub_1_12820:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 4bh
        mov    dword ptr [ebp - 0ch], ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 4]
        mov    di, word ptr [ecx + 33h]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 14h], eax
        mov    byte ptr [ecx + 11h], 10h
        mov    eax, dword ptr [ecx + 2fh]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 10h], 0
        mov    al, byte ptr [eax + 1]
        cmp    byte ptr [ecx + 15h], al
        jne    NEAR PTR loc_1_12B22
        mov    ecx, dword ptr [ebp - 0ch]
        cmp    dword ptr [ecx + 0a4h], 0
        je     SHORT loc_1_12886
        mov    dword ptr [ecx + 0a4h], 0
        lea    eax, [ebp - 1]
        push   eax
        mov    eax, ecx
        add    eax, 98h
        push   eax
        call   sub_1_0738
loc_1_12886:
        mov    ecx, dword ptr [ebp - 0ch]
        xor    eax, eax
        mov    al, byte ptr [ecx]
        cmp    eax, 5
        je     SHORT loc_1_128A0
        cmp    eax, 80h
        je     SHORT loc_1_128B2
        jmp    NEAR PTR loc_1_12A27
        db  08Bh, 0FFh
loc_1_128A0:
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [eax + 0d0h], 803h
        jmp    NEAR PTR loc_1_12A27
loc_1_128B2:
        mov    eax, dword ptr [ebp - 8]
        cmp    byte ptr [eax + 4], 45h
        jne    NEAR PTR loc_1_12A27
        cmp    byte ptr [eax + 5], 3dh
        jne    NEAR PTR loc_1_12A27
        add    eax, 6
        push   eax
        call   sub_1_134D0
        add    esp, 4
        mov    dword ptr [ebp - 18h], eax
        cmp    eax, 288h
        je     SHORT loc_1_128F0
        cmp    eax, 2b3h
        je     SHORT loc_1_12900
        jmp    NEAR PTR loc_1_12A27
        db  005h, 000h, 000h, 000h, 000h
loc_1_128F0:
        mov    dword ptr [ebp - 10h], 1
        jmp    NEAR PTR loc_1_12A27
        db  08Dh, 064h, 024h, 000h
loc_1_12900:
        mov    esi, 6
        cmp    di, si
        mov    eax, 0
        setae  al
        test   al, 1
        je     SHORT loc_1_1295B
        mov    edx, edi
        and    edx, 0ffffh
        mov    ecx, 52h
loc_1_12921:
        mov    eax, dword ptr [ebp - 8]
        add    eax, esi
        cmp    byte ptr [eax - 2], cl
        jne    SHORT loc_1_1293D
        cmp    byte ptr [eax - 1], 3dh
        jne    SHORT loc_1_1293D
        cmp    byte ptr [eax], 31h
        mov    dword ptr [ebp - 10h], 1
        je     SHORT loc_1_12944
loc_1_1293D:
        mov    dword ptr [ebp - 10h], 0
loc_1_12944:
        inc    esi
        cmp    esi, edx
        mov    eax, 0
        setle  al
        cmp    dword ptr [ebp - 10h], 1
        sbb    ebx, ebx
        neg    ebx
        test   ebx, eax
        jne    SHORT loc_1_12921
loc_1_1295B:
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_12A27
        mov    ecx, dword ptr [ebp - 0ch]
        inc    esi
        mov    byte ptr [ecx + 8fh], 0
        mov    ecx, edi
        and    ecx, 0ffffh
        cmp    esi, ecx
        jg     SHORT loc_1_12995
        mov    edx, 43h
loc_1_12981:
        mov    eax, dword ptr [ebp - 8]
        add    eax, esi
        cmp    byte ptr [eax - 1], dl
        jne    SHORT loc_1_12990
        cmp    byte ptr [eax], 3dh
        je     SHORT loc_1_12995
loc_1_12990:
        inc    esi
        cmp    esi, ecx
        jle    SHORT loc_1_12981
loc_1_12995:
        mov    eax, dword ptr [ebp - 8]
        add    eax, esi
        cmp    byte ptr [eax - 1], 43h
        jne    NEAR PTR loc_1_12A27
        cmp    byte ptr [eax], 3dh
        jne    SHORT loc_1_12A27
        lea    eax, [esi + 10h]
        cmp    eax, ecx
        jle    SHORT loc_1_129C0
        mov    dword ptr [ebp - 10h], 0
        jmp    SHORT loc_1_12A27
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_129C0:
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 87h
        sub    edi, edi
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        inc    esi
loc_1_129D8:
        test   edi, 1
        mov    ebx, edi
        je     SHORT loc_1_129F7
        sar    ebx, 1
        mov    eax, dword ptr [ebp - 8]
        add    eax, esi
        mov    al, byte ptr [edi + eax]
        push   eax
        call   sub_1_135A0
        add    esp, 4
        jmp    SHORT loc_1_12A0D
loc_1_129F7:
        sar    ebx, 1
        mov    eax, dword ptr [ebp - 8]
        add    eax, esi
        mov    al, byte ptr [edi + eax]
        push   eax
        call   sub_1_135A0
        add    esp, 4
        shl    al, 4
loc_1_12A0D:
        mov    ecx, dword ptr [ebp - 0ch]
        or     byte ptr [ebx + ecx + 87h], al
        inc    edi
        cmp    edi, 10h
        jl     SHORT loc_1_129D8
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 8fh], 8
loc_1_12A27:
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_12ADA
        mov    ecx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    byte ptr [ecx + 11h], 10h
        mov    ax, word ptr [ecx + 2bh]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 94h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [eax + 0d0h], 800h
        xor    eax, eax
        mov    al, byte ptr [ecx]
        cmp    eax, 80h
        je     SHORT loc_1_12A78
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [eax + 0d4h], 901h
        db  0C7h, 080h, 0E0h, 000h, 000h, 000h, 0F0h, 02Ch, 001h, 000h
        jmp    SHORT loc_1_12AB4
loc_1_12A78:
        cmp    dword ptr [ebp - 18h], 288h
        mov    eax, dword ptr [ebp - 14h]
        je     SHORT loc_1_12AA0
        mov    dword ptr [eax + 0d4h], 905h
        db  0C7h, 080h, 0E0h, 000h, 000h, 000h, 0F0h, 02Ch, 001h, 000h
        jmp    SHORT loc_1_12AB4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_12AA0:
        mov    dword ptr [eax + 0d4h], 907h
        db  0C7h, 080h, 0E0h, 000h, 000h, 000h, 030h, 02Bh, 001h, 000h
loc_1_12AB4:
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0d8h], 0
        mov    dword ptr [eax + 0dch], ecx
        push   eax
        call   PPPMAC_Service_09
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_12ADA:
        mov    ecx, dword ptr [ebp - 0ch]
        cmp    byte ptr [ecx], 80h
        jne    SHORT loc_1_12AF8
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [eax + 0d0h], 811h
        mov    dword ptr [eax + 0d4h], ecx
loc_1_12AF8:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    dword ptr [eax + 0d8h], 0
        mov    dword ptr [eax + 0dch], 0
        mov    dword ptr [eax + 0e0h], 0
        call   PPPMAC_Service_09
        add    esp, 4
loc_1_12B22:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_12B30:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        lea    edi, [esi + 4bh]
        mov    dword ptr [eax + 0d0h], 801h
        mov    dword ptr [eax + 0d4h], 0
        mov    dword ptr [eax + 0dch], 0
        mov    dword ptr [eax + 0e0h], 0
        mov    dword ptr [ebx + 10h], 801h
        mov    al, byte ptr [edi + 86h]
        inc    al
        mov    byte ptr [edi + 86h], al
        mov    byte ptr [esi + 15h], al
        xor    eax, eax
        mov    ax, word ptr [ebx + 16h]
        push   eax
        lea    eax, [ebx + 18h]
        push   eax
        lea    eax, [edi + 5]
        push   eax
        call   sub_1_083C
        mov    al, byte ptr [ebx + 16h]
        mov    byte ptr [edi + 85h], al
        push   esi
        call   sub_1_12BC0
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_12BC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [esi + 4bh]
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [eax + 0ch]
        mov    eax, dword ptr [ecx + 0feh]
        mov    dword ptr [ebp - 0ch], eax
        mov    al, byte ptr [edi + 85h]
        and    eax, 0ffff00ffh
        add    ax, 8
        mov    word ptr [ebp - 2], ax
        and    eax, 0ffffh
        push   eax
        push   ecx
        call   PPPMAC_Service_02
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_1_12CE4
        mov    eax, dword ptr [esi + 8]
        mov    ax, word ptr [eax + 0eh]
        mov    word ptr [ebx + 28h], ax
        mov    eax, dword ptr [ebx + 1ch]
        add    eax, dword ptr [ebx + 20h]
        mov    dword ptr [ebp - 8], eax
        mov    byte ptr [eax], 5
        inc    dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [edi + 86h]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 8]
        mov    al, byte ptr [ebp - 1]
        mov    ecx, dword ptr [ebp - 8]
        mov    ah, byte ptr [ebp - 2]
        mov    word ptr [ecx], ax
        xor    eax, eax
        add    dword ptr [ebp - 8], 2
        mov    al, byte ptr [edi + 85h]
        push   eax
        lea    eax, [edi + 5]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_083C
        xor    eax, eax
        mov    al, byte ptr [edi + 85h]
        add    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 28h]
        mov    cl, ah
        mov    ch, al
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], cx
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0
        add    dword ptr [ebp - 8], 2
        mov    dword ptr [esi + 3bh], ebx
        mov    eax, dword ptr [ebx + 1ch]
        add    eax, dword ptr [ebx + 20h]
        mov    dword ptr [esi + 35h], eax
        mov    eax, dword ptr [ebx + 24h]
        mov    word ptr [esi + 39h], ax
        mov    al, byte ptr [edi + 86h]
        mov    byte ptr [esi + 15h], al
        mov    byte ptr [esi + 11h], 6
        push   esi
        call   PPPMAC_Service_07
        add    esp, 4
        push   esi
        call   PPPMAC_Service_08
        add    esp, 4
        mov    dword ptr [esi + 3bh], 0
        mov    dword ptr [esi + 35h], 0
        mov    word ptr [esi + 39h], 0
        push   1388h
        lea    eax, [edi + 98h]
        push   eax
        call   sub_1_0732
        mov    dword ptr [edi + 0a4h], 1
loc_1_12CE4:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_12CF0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 4bh
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    esi, dword ptr [eax + 0ch]
        mov    eax, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [esi + 0feh]
        mov    dword ptr [esi + 0d0h], 801h
        mov    dword ptr [esi + 0d4h], 0
        mov    dword ptr [esi + 0dch], 0
        mov    dword ptr [esi + 0e0h], 0
        mov    dword ptr [eax + 8], 801h
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 8fh]
        and    eax, 0ffh
        add    eax, dword ptr [edi + 28h]
        mov    ecx, dword ptr [edi + 20h]
        lea    ebx, [eax + ecx + 1eh]
        db  0A1h, 050h, 082h, 001h, 000h
        push   eax
        push   0
        push   ebx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0756
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_12E6B
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ch], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 10h], 800h
        mov    eax, dword ptr [ebp - 8]
        mov    cl, byte ptr [eax + 86h]
        inc    cl
        mov    byte ptr [eax + 86h], cl
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 14h], cl
        mov    eax, dword ptr [ebp - 8]
        mov    cl, byte ptr [eax]
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 15h], cl
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [edi + 20h]
        mov    word ptr [ecx + 16h], ax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [edi + 28h]
        mov    word ptr [ecx + 18h], ax
        mov    eax, dword ptr [ebp - 8]
        mov    cl, byte ptr [eax + 8fh]
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1ah], cl
        mov    ebx, dword ptr [ebp - 4]
        add    ebx, 1ch
        mov    eax, dword ptr [edi + 20h]
        inc    eax
        push   eax
        mov    eax, dword ptr [edi + 30h]
        push   eax
        push   ebx
        call   sub_1_083C
        mov    eax, dword ptr [edi + 20h]
        inc    eax
        add    ebx, eax
        mov    eax, dword ptr [edi + 28h]
        inc    eax
        push   eax
        mov    eax, dword ptr [edi + 38h]
        push   eax
        push   ebx
        call   sub_1_083C
        mov    eax, dword ptr [edi + 28h]
        inc    eax
        add    ebx, eax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 8fh]
        or     al, al
        je     SHORT loc_1_12E28
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 87h
        push   eax
        push   ebx
        call   sub_1_083C
loc_1_12E28:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 8fh], 0
        mov    dword ptr [esi + 0d0h], 800h
        mov    dword ptr [esi + 0d4h], 904h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0d8h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0dch], eax
        db  0C7h, 086h, 0E0h, 000h, 000h, 000h, 090h, 025h, 001h, 000h
        push   esi
        call   PPPMAC_Service_09
        add    esp, 4
loc_1_12E6B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_12E80:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    ebx, [esi + 4bh]
        mov    eax, dword ptr [esi + 4]
        mov    di, word ptr [esi + 33h]
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 0ch], ecx
        mov    eax, dword ptr [esi + 2fh]
        mov    ecx, eax
        mov    dword ptr [ebp - 14h], eax
        mov    byte ptr [esi + 11h], 10h
        mov    al, byte ptr [esi + 15h]
        cmp    byte ptr [ecx + 1], al
        je     SHORT loc_1_12EC0
        mov    byte ptr [esi + 11h], 10h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_12EC0:
        cmp    dword ptr [ebx + 0b4h], 0
        je     SHORT loc_1_12EE3
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [ebx + 0a8h]
        push   eax
        call   sub_1_0738
        mov    dword ptr [ebx + 0b4h], 0
loc_1_12EE3:
        cmp    byte ptr [esi + 22h], 0
        je     SHORT loc_1_12EFA
        lea    eax, [ebp - 2]
        push   eax
        lea    eax, [esi + 16h]
        push   eax
        call   sub_1_0738
        mov    byte ptr [esi + 22h], 0
loc_1_12EFA:
        mov    eax, dword ptr [ebp - 14h]
        mov    dl, byte ptr [eax + 4]
        and    edx, 0ffff00ffh
        mov    ecx, edx
        and    ecx, 0ffffh
        mov    dword ptr [ebp - 10h], ecx
        xor    eax, eax
        mov    ax, word ptr [esi + 33h]
        lea    ebx, [ecx + 5]
        cmp    eax, ebx
        jl     NEAR PTR loc_1_13014
        sub    di, dx
        sub    di, 5
        mov    eax, dword ptr [ebp - 14h]
        lea    eax, [ecx + eax + 5]
        mov    dword ptr [ebp - 1ch], eax
        lea    eax, [edx + edi + 2ah]
        mov    ecx, eax
        and    ecx, 0ffffh
        mov    dword ptr [ebp - 18h], ecx
        db  0A1h, 050h, 082h, 001h, 000h
        push   eax
        push   0
        lea    eax, [ebp - 8]
        push   ecx
        push   eax
        call   sub_1_0756
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_13014
        mov    ebx, dword ptr [ebp - 8]
        add    ebx, 28h
        and    edi, 0ffffh
        push   edi
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        push   ebx
        add    ebx, edi
        call   sub_1_083C
        mov    byte ptr [ebx], 0
        inc    ebx
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ebx], 0
        push   ecx
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 5
        push   eax
        inc    ebx
        push   ebx
        call   sub_1_083C
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [eax + 0ch], ecx
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 10h], 800h
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [esi + 15h]
        mov    dword ptr [ecx + 14h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 18h], edi
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 1ch], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [eax + 24h], ecx
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        mov    dword ptr [ecx + 0d0h], 800h
        mov    dword ptr [ecx + 0d4h], 900h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0d8h], eax
        mov    dword ptr [ecx + 0dch], esi
        db  0C7h, 081h, 0E0h, 000h, 000h, 000h, 020h, 030h, 001h, 000h
        call   PPPMAC_Service_09
        add    esp, 4
loc_1_13014:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_13020:
        push   ebp
        mov    ebp, esp
        sub    esp, 38h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    ecx, [esi + 4bh]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ebp - 0ch], ecx
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 8], eax
        mov    edi, dword ptr [eax + 0feh]
        mov    eax, dword ptr [eax + 0d8h]
        mov    al, byte ptr [eax + 14h]
        mov    byte ptr [esi + 14h], al
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0d8h]
        or     eax, eax
        je     SHORT loc_1_13073
        push   0
        mov    ecx, dword ptr [eax + 0ch]
        push   ecx
        push   eax
        call   sub_1_075C
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0d8h], 0
loc_1_13073:
        mov    eax, dword ptr [ebp - 8]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 0d0h], 801h
        mov    dword ptr [eax + 0d4h], 0
        mov    dword ptr [eax + 0dch], 0
        mov    dword ptr [eax + 0e0h], 0
        mov    dword ptr [ebx + 10h], 801h
        cmp    dword ptr [ebx + 18h], 0
        je     SHORT loc_1_130E0
        mov    dword ptr [edi + 10h], 1
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [edi + 14h], eax
        mov    eax, dword ptr [ebx + 20h]
        mov    dword ptr [edi + 18h], eax
        mov    eax, dword ptr [ebx + 24h]
        mov    dword ptr [edi + 1ch], eax
        mov    byte ptr [esi + 11h], 6
        mov    byte ptr [ebp - 1], 3
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_131B0
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_130E0:
        lea    eax, [esi + 11h]
        mov    byte ptr [esi + 11h], 7
        mov    byte ptr [ebp - 1], 4
        push   2
        db  068h, 034h, 08Ch, 001h, 000h
        lea    eax, [ebp - 38h]
        push   eax
        call   sub_1_083C
        lea    eax, [ebp - 36h]
        push   eax
        mov    eax, dword ptr [ebx + 14h]
        push   eax
        call   sub_1_13530
        add    esp, 8
        add    ax, 2
        mov    ecx, dword ptr [ebp - 0ch]
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ecx + 90h]
        dec    eax
        mov    dword ptr [ecx + 90h], eax
        jne    SHORT loc_1_13145
        push   4
        db  068h, 038h, 08Ch, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        lea    eax, [ebp + eax - 38h]
        push   eax
        call   sub_1_083C
        add    word ptr [ebp - 4], 4
        jmp    SHORT loc_1_131B0
loc_1_13145:
        push   7
        db  068h, 040h, 08Ch, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        lea    eax, [ebp + eax - 38h]
        push   eax
        call   sub_1_083C
        inc    byte ptr [esi + 15h]
        add    word ptr [ebp - 4], 7
        sub    ebx, ebx
loc_1_13168:
        test   bl, 1
        mov    eax, dword ptr [edi + 3ch]
        mov    ecx, ebx
        je     SHORT loc_1_13180
        shr    ecx, 1
        mov    al, byte ptr [eax + ecx]
        and    eax, 0fh
        jmp    SHORT loc_1_1318D
        db  08Dh, 064h, 024h, 000h
loc_1_13180:
        shr    ecx, 1
        mov    al, byte ptr [eax + ecx]
        shr    al, 4
        and    eax, 0ffh
loc_1_1318D:
        push   eax
        call   sub_1_135F0
        add    esp, 4
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    ecx, ebx
        mov    byte ptr [ebp + ecx - 38h], al
        inc    ebx
        cmp    ebx, 10h
        jb     SHORT loc_1_13168
        add    word ptr [ebp - 4], 10h
loc_1_131B0:
        mov    edi, dword ptr [ebp - 4]
        and    edi, 0ffffh
        lea    eax, [edi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   PPPMAC_Service_02
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_131DA
        mov    byte ptr [esi + 11h], 10h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_131DA:
        mov    edx, dword ptr [eax + 1ch]
        add    edx, dword ptr [eax + 20h]
        mov    dword ptr [esi + 3bh], eax
        mov    dword ptr [esi + 35h], edx
        inc    edx
        mov    ecx, dword ptr [eax + 24h]
        mov    word ptr [esi + 39h], cx
        mov    cl, byte ptr [ebp - 1]
        mov    byte ptr [edx - 1], cl
        inc    edx
        mov    cl, byte ptr [esi + 14h]
        mov    byte ptr [edx - 1], cl
        mov    ecx, dword ptr [eax + 24h]
        mov    al, ch
        mov    ah, cl
        mov    word ptr [edx], ax
        push   edi
        lea    eax, [ebp - 38h]
        push   eax
        add    edx, 2
        push   edx
        call   sub_1_083C
        push   esi
        call   sub_1_13240
        add    esp, 4
        mov    dword ptr [esi + 3bh], 0
        mov    dword ptr [esi + 35h], 0
        mov    word ptr [esi + 39h], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_13240:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [esi + 4bh]
        mov    ecx, dword ptr [esi + 4]
        cmp    byte ptr [esi + 11h], 7
        mov    eax, dword ptr [ecx + 0ch]
        mov    eax, dword ptr [eax + 0feh]
        jne    SHORT loc_1_132B5
        mov    byte ptr [esi + 10h], 6
        push   esi
        call   PPPMAC_Service_07
        add    esp, 4
        push   esi
        call   PPPMAC_Service_08
        add    esp, 4
        cmp    dword ptr [edi + 90h], 0
        jne    SHORT loc_1_13296
        mov    byte ptr [esi + 11h], 5
        push   esi
        call   PPPMAC_Service_07
        add    esp, 4
        push   esi
        call   PPPMAC_Service_08
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_1_13296:
        push   15f90h
        lea    eax, [edi + 0a8h]
        push   eax
        call   sub_1_0732
        mov    dword ptr [edi + 0b4h], 1
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_1_132B5:
        cmp    byte ptr [esi + 10h], 9
        jne    SHORT loc_1_132C1
        mov    byte ptr [esi + 10h], 6
        jmp    SHORT loc_1_132D2
loc_1_132C1:
        cmp    dword ptr [ecx + 10h], 1
        sbb    ecx, ecx
        inc    ecx
        cmp    dword ptr [eax], 1
        sbb    eax, eax
        inc    eax
        cmp    ecx, eax
        je     SHORT loc_1_132E8
loc_1_132D2:
        push   esi
        call   PPPMAC_Service_07
        add    esp, 4
        push   esi
        call   PPPMAC_Service_08
        add    esp, 4
        mov    byte ptr [esi + 11h], 8
loc_1_132E8:
        push   esi
        call   PPPMAC_Service_07
        add    esp, 4
        push   esi
        call   PPPMAC_Service_08
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_1_13300:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 4]
        mov    eax, dword ptr [esi + 2fh]
        mov    ebx, dword ptr [edi + 0ch]
        mov    dl, byte ptr [esi + 10h]
        cmp    dl, 3
        mov    ecx, dword ptr [ebx + 0feh]
        mov    byte ptr [esi + 11h], 10h
        je     NEAR PTR loc_1_134C5
        cmp    dl, 9
        je     NEAR PTR loc_1_134C5
        cmp    dword ptr [ebx + 0dch], 0
        jne    NEAR PTR loc_1_134C5
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        cmp    eax, 1
        je     SHORT loc_1_1336A
        cmp    eax, 2
        je     SHORT loc_1_133B0
        cmp    eax, 3
        je     NEAR PTR loc_1_133F3
        cmp    eax, 4
        je     NEAR PTR loc_1_13464
        mov    byte ptr [esi + 11h], 0ch
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_1336A:
        cmp    dword ptr [edi + 10h], 0
        jne    SHORT loc_1_133A0
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_134C5
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 0E2h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 09Ch, 0E8h, 0FCh, 0FFh
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_133A0:
        push   esi
        call   sub_1_12430
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_1_133B0:
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_1_133E5
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_134C5
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 0E2h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 057h, 0E8h, 0FCh, 0FFh
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_133E5:
        push   esi
        call   sub_1_12E80
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_133F3:
        cmp    dword ptr [edi + 10h], 0
        jne    SHORT loc_1_13430
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_134C5
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 0E2h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 013h, 0E8h, 0FCh, 0FFh
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_13430:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_13456
        xor    eax, eax
        db  066h, 0A1h, 0E4h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0E1h, 0E7h, 0FCh, 0FFh
        add    esp, 8
loc_1_13456:
        push   esi
        call   sub_1_127C0
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_13464:
        cmp    dword ptr [edi + 10h], 0
        jne    SHORT loc_1_13496
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_134C5
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 0E2h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0A6h, 0E7h, 0FCh, 0FFh
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_13496:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_134BC
        xor    eax, eax
        db  066h, 0A1h, 0E6h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 07Bh, 0E7h, 0FCh, 0FFh
        add    esp, 8
loc_1_134BC:
        push   esi
        call   sub_1_12820
        add    esp, 4
loc_1_134C5:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
_LTEXT ends
        end
