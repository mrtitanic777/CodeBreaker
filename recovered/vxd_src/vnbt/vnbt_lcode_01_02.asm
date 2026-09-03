; vnbt_lcode_01_02.asm -- VNBT object 1 segment _LTEXT  (part 2/5)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_45B0:
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 3ch]
        lea    eax, [esp + 20h]
        mov    ecx, dword ptr [esp + 48h]
        push   eax
        sub    dword ptr [esi + 20h], ecx
        mov    eax, dword ptr [esp + 54h]
        push   eax
        mov    eax, dword ptr [esp + 54h]
        push   eax
        mov    eax, dword ptr [esp + 50h]
        push   ecx
        push   eax
        push   0
        push   esi
        push   0
        call   sub_1_3E90
        add    esp, 20h
        mov    ebx, eax
        cmp    ebx, 18h
        jne    SHORT loc_1_4652
        mov    edi, dword ptr [esp + 2ch]
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [esp + 0ch], eax
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [esp + 14h], eax
        mov    eax, edi
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [esp + 10h], eax
        mov    eax, edi
        add    eax, 20h
        push   eax
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 18h]
        db  068h, 050h, 02Fh, 001h, 000h
        push   eax
        db  0A1h, 094h, 034h, 001h, 000h
        call   dword ptr [eax + 28h]
        add    esp, 10h
        mov    ebx, eax
        cmp    ebx, 0ffh
        je     SHORT loc_1_4650
        push   0
        push   ebx
        mov    eax, dword ptr [edi + 1ch]
        push   eax
        call   sub_1_25A0
        add    esp, 0ch
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
        db  08Dh, 049h, 000h
loc_1_4650:
        sub    ebx, ebx
loc_1_4652:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_4660:
        sub    esp, 4
        lea    eax, [esp]
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        push   eax
        call   sub_1_7528
        add    esp, 18h
        or     eax, eax
        je     SHORT loc_1_46A0
        cmp    eax, 0ffh
        je     SHORT loc_1_46A0
        mov    eax, 17h
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_46A0:
        mov    eax, dword ptr [esp + 28h]
        db  0C7h, 000h, 0E0h, 046h, 000h, 000h
        mov    ecx, dword ptr [esp]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0ch], 0
        mov    eax, dword ptr [esp + 24h]
        mov    ecx, dword ptr [esp]
        mov    dword ptr [eax], ecx
        mov    eax, 18h
        add    esp, 4
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_46E0:
        push   esi
        mov    eax, dword ptr [esp + 0ch]
        or     eax, eax
        je     SHORT loc_1_4720
        cmp    eax, 0ffh
        je     SHORT loc_1_4720
        mov    edx, dword ptr [esp + 8]
        cmp    dword ptr [edx + 38h], 4
        jne    SHORT loc_1_4720
        mov    dword ptr [edx + 38h], 0
        mov    esi, dword ptr [edx + 18h]
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        add    esi, 1ch
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        dec    dword ptr [edx + 3ch]
        mov    eax, dword ptr [esi]
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], esi
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [esi], edx
loc_1_4720:
        pop    esi
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4730:
        mov    eax, dword ptr [esp + 1ch]
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        push   eax
        call   sub_1_7598
        add    esp, 1ch
        or     eax, eax
        je     SHORT loc_1_476B
        cmp    eax, 0ffh
        je     SHORT loc_1_476B
        mov    eax, 0eh
loc_1_476B:
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_4770:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 28h]
        lea    eax, [esp + 12h]
        mov    esi, dword ptr [edi + 80h]
        mov    ebx, dword ptr [esp + 2ch]
        push   eax
        push   ebx
        push   esi
        call   sub_1_2DC0
        mov    byte ptr [esp + 1fh], al
        add    esp, 0ch
        push   ebx
        push   esi
        call   sub_1_2FB0
        add    esp, 8
        mov    eax, dword ptr [ebx + 0ch]
        or     eax, eax
        je     SHORT loc_1_47CE
        cmp    byte ptr [eax + 48h], 1
        jne    SHORT loc_1_47CE
        cmp    word ptr [eax + 32h], 3
        jne    SHORT loc_1_47CE
        mov    edx, dword ptr [eax + 28h]
        lea    ebp, [eax + 28h]
        mov    ecx, dword ptr [eax + 2ch]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
        mov    byte ptr [eax + 48h], 0
        mov    dword ptr [eax + 2ch], ebp
        mov    dword ptr [ebp], ebp
loc_1_47CE:
        mov    ax, word ptr [ebx + 3ah]
        and    ax, 1
        cmp    ax, 1
        sbb    ebp, ebp
        inc    ebp
        mov    dword ptr [esp + 14h], ebx
        push   0
        push   esi
        lea    eax, [esp + 1ch]
        push   0
        push   eax
        call   sub_1_AD68
        lea    eax, [esp + 24h]
        add    esp, 10h
        push   eax
        call   sub_1_A9FC
        add    esp, 4
        cmp    byte ptr [esp + 13h], 0
        jne    SHORT loc_1_481B
        or     ebp, ebp
        je     SHORT loc_1_481B
        mov    al, byte ptr [esp + 12h]
        push   1
        push   eax
        push   esi
        call   sub_1_2F70
        add    esp, 0ch
loc_1_481B:
        cmp    dword ptr [edi + 44h], 0
        je     SHORT loc_1_4864
        push   edi
        call   sub_1_1770
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_4864
        lea    eax, [esp + 11h]
        push   eax
        mov    eax, dword ptr [edi + 10h]
        push   eax
        push   esi
        call   sub_1_2E00
        add    esp, 0ch
        or     al, al
        je     SHORT loc_1_4851
        mov    eax, 0c0000001h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
loc_1_4851:
        mov    al, byte ptr [esp + 11h]
        push   1
        push   eax
        push   edi
        push   0
        push   esi
        call   sub_1_17A0
        add    esp, 14h
loc_1_4864:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Bh, 0FFh
loc_1_4870:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [esp + 10h], 0
        lea    eax, [esp + 10h]
        mov    esi, dword ptr [esp + 40h]
        mov    dword ptr [esp + 0ch], 0
        push   eax
        mov    dword ptr [esi], 0
        lea    eax, [esp + 10h]
        push   eax
        mov    eax, dword ptr [esp + 44h]
        mov    ebx, dword ptr [esp + 40h]
        push   eax
        mov    edi, dword ptr [esp + 40h]
        push   ebx
        mov    eax, dword ptr [esp + 40h]
        push   edi
        push   eax
        mov    eax, dword ptr [esp + 40h]
        push   0
        push   eax
        mov    eax, dword ptr [esp + 44h]
        push   eax
        mov    eax, dword ptr [esp + 44h]
        push   eax
        mov    eax, dword ptr [esp + 44h]
        push   eax
        mov    eax, dword ptr [esp + 44h]
        push   eax
        call   sub_1_7AFC
        add    esp, 30h
        or     eax, eax
        je     SHORT loc_1_48DC
        cmp    eax, 0ffh
        jne    SHORT loc_1_4955
loc_1_48DC:
        push   30h
        call   sub_1_001E
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_48F6
        mov    eax, 17h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_48F6:
        db  0C7h, 040h, 008h, 090h, 049h, 000h, 000h
        sub    edi, dword ptr [ebx]
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [eax + 0ch], eax
        lea    edx, [eax + 1ch]
        mov    dword ptr [eax], edx
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [eax + 14h], ecx
        mov    dword ptr [eax + 10h], 0
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax + 18h], ecx
        mov    dword ptr [edx + 10h], 4655424eh
        mov    dword ptr [edx], 0
        mov    ecx, dword ptr [esp + 0ch]
        mov    cx, word ptr [ecx + 8]
        and    ecx, 0ffffh
        mov    dword ptr [edx + 0ch], ecx
        mov    ecx, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], ecx
        mov    dword ptr [edx + 8], 0
        mov    dword ptr [esi], eax
        mov    eax, 18h
loc_1_4955:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_4960:
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [esp + 1ch]
        push   eax
        mov    eax, dword ptr [esp + 2ch]
        push   eax
        mov    eax, dword ptr [esp + 14h]
        push   eax
        mov    eax, dword ptr [esp + 10h]
        push   eax
        call   sub_1_80D8
        add    esp, 10h
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4990:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 24h]
        cmp    dword ptr [esp + 28h], 0
        mov    ecx, dword ptr [eax + 14h]
        mov    dword ptr [esp + 14h], ecx
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [esp + 18h], eax
        jne    SHORT loc_1_49CB
        mov    cx, word ptr [ecx + 8]
        mov    eax, dword ptr [esp + 24h]
        and    ecx, 0ffffh
        cmp    ecx, dword ptr [eax + 4]
        jae    SHORT loc_1_49CB
        mov    dword ptr [esp + 28h], 9
loc_1_49CB:
        cmp    dword ptr [esp + 18h], 0
        je     NEAR PTR loc_1_4B2F
        mov    eax, dword ptr [esp + 18h]
        mov    ecx, dword ptr [eax]
        lea    eax, [ecx + 0ch]
        mov    dword ptr [esp + 1ch], eax
        mov    ebx, dword ptr [eax]
        mov    eax, dword ptr [esp + 18h]
        cmp    byte ptr [eax + 15h], 0
        je     SHORT loc_1_4A10
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [esp + 30h]
        push   ecx
        mov    ecx, dword ptr [esp + 1ch]
        push   eax
        mov    eax, dword ptr [ecx + 4]
        push   eax
        call   sub_1_8018
        add    esp, 10h
        jmp    NEAR PTR loc_1_4B0C
        db  08Bh, 0FFh
loc_1_4A10:
        cmp    ebx, dword ptr [esp + 1ch]
        je     NEAR PTR loc_1_4AFD
        lea    ebx, [ebx]
loc_1_4A20:
        lea    eax, [ebx + 84h]
        mov    dword ptr [esp + 10h], eax
        inc    dword ptr [eax]
        mov    eax, dword ptr [esp + 18h]
        cmp    dword ptr [eax + 10h], ebx
        je     NEAR PTR loc_1_4AEB
        mov    ebp, dword ptr [ebx + 24h]
        mov    eax, ebp
        sub    eax, ebx
        cmp    eax, 24h
        je     NEAR PTR loc_1_4AEB
        mov    eax, dword ptr [ebp + 4]
        mov    ecx, dword ptr [ebp]
        mov    dword ptr [eax], ecx
        sub    edx, edx
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [esp + 28h], edx
        je     SHORT loc_1_4A70
        cmp    dword ptr [esp + 28h], 9
        je     SHORT loc_1_4A70
        mov    ecx, dword ptr [esp + 28h]
        jmp    SHORT loc_1_4AD7
        db  08Dh, 064h, 024h, 000h
loc_1_4A70:
        xor    edx, edx
        mov    dx, word ptr [eax + 8]
        mov    ecx, dword ptr [esp + 2ch]
        cmp    edx, ecx
        jb     SHORT loc_1_4A80
        mov    edx, ecx
loc_1_4A80:
        mov    ecx, dword ptr [esp + 14h]
        mov    edi, dword ptr [eax + 4]
        mov    esi, dword ptr [ecx + 4]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [esp + 14h]
        lea    esi, [eax + 0ah]
        mov    edi, dword ptr [ecx + 0ah]
        mov    dword ptr [esi], edi
        mov    edi, dword ptr [ecx + 0eh]
        mov    dword ptr [esi + 4], edi
        mov    edi, dword ptr [ecx + 12h]
        add    ecx, 0ah
        mov    dword ptr [esi + 8], edi
        mov    ecx, dword ptr [ecx + 0ch]
        mov    dword ptr [esi + 0ch], ecx
        xor    ecx, ecx
        mov    cx, word ptr [eax + 8]
        cmp    dword ptr [esp + 2ch], ecx
        ja     SHORT loc_1_4AD2
        jne    SHORT loc_1_4ACE
        cmp    dword ptr [esp + 28h], 9
        je     SHORT loc_1_4AD2
loc_1_4ACE:
        sub    ecx, ecx
        jmp    SHORT loc_1_4AD7
loc_1_4AD2:
        mov    ecx, 9
loc_1_4AD7:
        push   edx
        push   ecx
        push   eax
        call   sub_1_25A0
        add    esp, 0ch
        push   ebp
        call   sub_1_0024
        add    esp, 4
loc_1_4AEB:
        mov    eax, dword ptr [esp + 10h]
        mov    ebx, dword ptr [ebx]
        dec    dword ptr [eax]
        cmp    ebx, dword ptr [esp + 1ch]
        jne    NEAR PTR loc_1_4A20
loc_1_4AFD:
        mov    eax, dword ptr [esp + 18h]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_DDE4
        add    esp, 4
loc_1_4B0C:
        mov    eax, dword ptr [esp + 18h]
        cmp    byte ptr [eax + 15h], 0
        jne    SHORT loc_1_4B22
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_4B22:
        mov    eax, dword ptr [esp + 24h]
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_4B2F:
        mov    eax, dword ptr [esp + 2ch]
        push   eax
        mov    eax, dword ptr [esp + 2ch]
        mov    ecx, dword ptr [esp + 18h]
        push   eax
        push   ecx
        call   sub_1_25A0
        mov    eax, dword ptr [esp + 30h]
        add    esp, 0ch
        push   eax
        call   sub_1_0024
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
loc_1_4B60:
        mov    eax, 0feh
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4B70:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0A1h, 074h, 032h, 001h, 000h
        db  03Dh, 074h, 032h, 001h, 000h
        mov    dword ptr [esp + 10h], eax
        je     NEAR PTR loc_1_4C1A
loc_1_4B8B:
        mov    eax, dword ptr [esp + 10h]
        mov    esi, dword ptr [eax + 0ch]
        sub    eax, esi
        cmp    eax, -0ch
        je     SHORT loc_1_4C05
        sub    ebx, ebx
        mov    ebp, 7
loc_1_4BA0:
        mov    edi, dword ptr [esi + 1ch]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -1ch
        je     SHORT loc_1_4BF8
        lea    esp, [esp]
loc_1_4BB0:
        cmp    byte ptr [edi + 38h], bl
        je     SHORT loc_1_4BED
        cmp    dword ptr [edi + 68h], ebp
        jne    SHORT loc_1_4BED
        mov    ecx, dword ptr [edi + 1ch]
        mov    eax, ecx
        sub    eax, edi
        cmp    eax, 1ch
        je     SHORT loc_1_4BED
        mov    al, byte ptr [ecx + 34h]
        cmp    al, 1
        jne    SHORT loc_1_4BE8
        mov    eax, dword ptr [ecx + 4]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [eax], edx
        mov    dword ptr [edx + 4], eax
        push   ebx
        push   15h
        mov    eax, dword ptr [ecx + 1ch]
        push   eax
        call   sub_1_25A0
        add    esp, 0ch
        jmp    SHORT loc_1_4BED
loc_1_4BE8:
        dec    al
        mov    byte ptr [ecx + 34h], al
loc_1_4BED:
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -1ch
        jne    SHORT loc_1_4BB0
loc_1_4BF8:
        mov    esi, dword ptr [esi]
        mov    eax, dword ptr [esp + 10h]
        sub    eax, esi
        cmp    eax, -0ch
        jne    SHORT loc_1_4BA0
loc_1_4C05:
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 10h], eax
        db  03Dh, 074h, 032h, 001h, 000h
        jne    NEAR PTR loc_1_4B8B
loc_1_4C1A:
        db  0A1h, 084h, 032h, 001h, 000h
        nop
loc_1_4C20:
        db  03Dh, 084h, 032h, 001h, 000h
        je     SHORT loc_1_4C48
        mov    ecx, eax
        mov    eax, dword ptr [eax]
        mov    dl, byte ptr [ecx + 0ch]
        dec    dl
        mov    byte ptr [ecx + 0ch], dl
        cmp    dl, 1
        jne    SHORT loc_1_4C20
        push   0
        push   15h
        sub    ecx, 32h
        push   ecx
        call   sub_1_25A0
        add    esp, 0ch
loc_1_4C48:
        db  0C7h, 005h, 098h, 033h, 001h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0A0h, 033h, 001h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 068h, 02Eh, 001h, 000h
        db  0A3h, 09Ch, 033h, 001h, 000h
        push   0
        db  068h, 070h, 04Bh, 000h, 000h
        push   1f4h
        db  068h, 08Ch, 033h, 001h, 000h
        call   0
        add    esp, 10h
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
sub_1_4C90:
        sub    esp, 4
        db  0F6h, 005h, 064h, 033h, 001h, 000h, 001h
        jne    SHORT loc_1_4CEE
        db  083h, 03Dh, 0F0h, 032h, 001h, 000h, 000h
        jne    SHORT loc_1_4CEE
        db  0C7h, 005h, 0E0h, 032h, 001h, 000h, 000h, 0A6h, 00Eh, 000h
        db  066h, 0C7h, 005h, 014h, 033h, 001h, 000h, 000h, 000h
        lea    eax, [esp]
        push   eax
        push   0
        push   0
        push   0
        db  068h, 0C0h, 0F5h, 000h, 000h
        push   0
        push   0
        db  0A1h, 0FCh, 032h, 001h, 000h
        push   eax
        call   sub_1_109B8
        add    esp, 20h
        or     eax, eax
        je     SHORT loc_1_4CE5
        cmp    eax, 0ffh
        jne    SHORT loc_1_4CF0
loc_1_4CE5:
        mov    eax, dword ptr [esp]
        db  0A3h, 0F0h, 032h, 001h, 000h
loc_1_4CEE:
        sub    eax, eax
loc_1_4CF0:
        add    esp, 4
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4D00:
        db  068h, 08Ch, 033h, 001h, 000h
        call   6
        add    esp, 4
        ret
        db  08Bh, 0FFh, 083h, 0ECh, 014h, 056h, 057h, 083h, 07Ch, 024h, 024h, 000h, 074h, 024h, 08Bh, 07Ch
        db  024h, 028h, 00Bh, 0FFh, 08Bh, 074h, 024h, 020h, 074h, 020h, 083h, 07Eh, 058h, 000h, 074h, 01Ah
        db  06Ah, 000h, 06Ah, 000h, 056h, 0E8h, 020h, 08Fh, 000h, 000h, 083h, 0C4h, 00Ch, 0EBh, 00Bh, 08Dh
        db  049h, 000h, 08Bh, 074h, 024h, 020h, 08Bh, 07Ch, 024h, 028h, 00Bh, 0FFh, 075h, 014h, 0C7h, 046h
        db  058h, 000h, 000h, 000h, 000h, 05Fh, 05Eh, 083h, 0C4h, 014h, 0C3h, 08Dh, 0A4h, 024h, 000h, 000h
        db  000h, 000h, 0C7h, 044h, 024h, 008h, 010h, 000h, 000h, 000h, 08Dh, 044h, 024h, 008h, 050h, 08Dh
        db  044h, 024h, 010h, 050h, 06Ah, 02Ch, 057h, 0E8h, 0F7h, 0B2h, 0FFh, 0FFh, 083h, 0C4h, 010h, 00Bh
        db  0C0h, 074h, 005h, 083h, 0F8h, 009h, 075h, 078h, 083h, 07Ch, 024h, 008h, 004h, 072h, 035h, 08Bh
        db  04Ch, 024h, 00Ch, 081h, 0E1h, 000h, 0FFh, 000h, 000h, 08Bh, 044h, 024h, 00Ch, 0C1h, 0E0h, 010h
        db  00Bh, 0C8h, 0C1h, 0E1h, 008h, 08Bh, 044h, 024h, 00Ch, 025h, 000h, 000h, 0FFh, 000h, 0C1h, 0E8h
        db  008h, 00Bh, 0C8h, 08Bh, 044h, 024h, 00Ch, 0C1h, 0E8h, 018h, 025h, 0FFh, 000h, 000h, 000h, 00Bh
        db  0C8h, 089h, 04Eh, 074h, 083h, 07Ch, 024h, 008h, 008h, 072h, 035h, 08Bh, 04Ch, 024h, 010h, 081h
        db  0E1h, 000h, 0FFh, 000h, 000h, 08Bh, 044h, 024h, 010h, 0C1h, 0E0h, 010h, 00Bh, 0C8h, 0C1h, 0E1h
        db  008h, 08Bh, 044h, 024h, 010h, 025h, 000h, 000h, 0FFh, 000h, 0C1h, 0E8h, 008h, 00Bh, 0C8h, 08Bh
        db  044h, 024h, 010h, 0C1h, 0E8h, 018h, 025h, 0FFh, 000h, 000h, 000h, 00Bh, 0C8h, 089h, 04Eh, 078h
        db  066h, 0A1h, 064h, 033h, 001h, 000h, 08Bh, 0C8h, 081h, 0E1h, 0FFh, 0FFh, 000h, 000h, 0F6h, 0C4h
        db  010h, 074h, 023h, 083h, 07Eh, 074h, 000h, 075h, 006h, 083h, 07Eh, 078h, 000h, 074h, 017h, 066h
        db  0C7h, 005h, 064h, 033h, 001h, 000h, 008h, 000h, 0F6h, 0C1h, 010h, 074h, 009h, 066h, 0C7h, 005h
        db  064h, 033h, 001h, 000h, 018h, 000h, 08Bh, 044h, 024h, 02Ch, 050h, 057h, 056h, 0E8h, 018h, 08Eh
        db  000h, 000h, 083h, 0C4h, 00Ch, 00Bh, 0C0h, 074h, 007h, 03Dh, 0FFh, 000h, 000h, 000h, 075h, 02Ch
        db  083h, 07Eh, 058h, 000h, 074h, 026h, 0F6h, 005h, 064h, 033h, 001h, 000h, 001h, 075h, 007h, 0E8h
        db  02Eh, 0B7h, 000h, 000h, 0EBh, 00Dh, 068h, 0F0h, 032h, 001h, 000h, 0E8h, 012h, 0B8h, 000h, 000h
        db  083h, 0C4h, 004h, 056h, 0E8h, 041h, 0ADh, 000h, 000h, 083h, 0C4h, 004h, 05Fh, 05Eh, 083h, 0C4h
        db  014h, 0C3h
sub_1_4E80:
        sub    esp, 10h
        push   esi
        mov    esi, dword ptr [esp + 18h]
        mov    eax, dword ptr [esi + 6ch]
        mov    dword ptr [esp + 4], eax
        lea    eax, [esp + 4]
        push   eax
        db  0A1h, 094h, 034h, 001h, 000h
        call   dword ptr [eax + 4]
        add    esp, 4
        mov    eax, dword ptr [esi + 68h]
        mov    dword ptr [esp + 4], eax
        lea    eax, [esp + 4]
        push   eax
        db  0A1h, 094h, 034h, 001h, 000h
        call   dword ptr [eax + 4]
        add    esp, 4
        mov    eax, dword ptr [esi + 70h]
        mov    dword ptr [esp + 4], eax
        lea    eax, [esp + 4]
        push   eax
        db  0A1h, 094h, 034h, 001h, 000h
        call   dword ptr [eax + 4]
        add    esp, 4
        mov    eax, dword ptr [esi + 54h]
        mov    dword ptr [esp + 4], eax
        push   0
        push   esi
        lea    eax, [esp + 0ch]
        push   0
        push   eax
        call   sub_1_AB28
        add    esp, 10h
        sub    eax, eax
        pop    esi
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
loc_1_4EF0:
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 40h]
        or     esi, esi
        jne    SHORT loc_1_4F10
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 2ch
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4F10:
        cmp    dword ptr [esp + 50h], 0
        je     NEAR PTR loc_1_50C0
        db  083h, 03Dh, 060h, 02Fh, 001h, 000h, 000h
        jne    SHORT loc_1_4F63
        db  08Ah, 01Dh, 064h, 033h, 001h, 000h
        call   sub_1_5CD0
        push   0
        db  0A1h, 080h, 02Fh, 001h, 000h
        push   eax
        db  0E8h, 064h, 0BAh, 0FDh, 0FFh
        add    esp, 8
        call   sub_1_5DF0
        test   bl, 10h
        je     SHORT loc_1_4F59
        db  066h, 0A1h, 064h, 033h, 001h, 000h
        or     ax, 10h
        db  066h, 0A3h, 064h, 033h, 001h, 000h
loc_1_4F59:
        db  0C7h, 005h, 060h, 02Fh, 001h, 000h, 001h, 000h, 000h, 000h
loc_1_4F63:
        lea    eax, [esp + 1ch]
        push   eax
        push   esi
        call   sub_1_55E0
        lea    eax, [esp + 34h]
        add    esp, 8
        push   eax
        push   esi
        call   sub_1_5510
        add    esp, 8
        sub    edi, edi
        db  0B8h, 0C4h, 033h, 001h, 000h
loc_1_4F86:
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_4FA3
        add    eax, 4
        inc    edi
        db  03Dh, 0E4h, 033h, 001h, 000h
        jb     SHORT loc_1_4F86
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 2ch
        ret
loc_1_4FA3:
        lea    eax, [esp + 14h]
        push   eax
        push   esi
        call   sub_1_56B0
        mov    edx, dword ptr [esp + 4ch]
        add    esp, 8
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
        and    edx, 0ffh
        or     ecx, edx
        mov    ebp, esi
        and    ebp, 0ff00h
        mov    eax, esi
        shl    eax, 10h
        or     ebp, eax
        shl    ebp, 8
        mov    eax, esi
        and    eax, 0ff0000h
        shr    eax, 8
        or     ebp, eax
        shr    esi, 18h
        and    esi, 0ffh
        lea    eax, [esp + 14h]
        push   0
        push   eax
        or     ebp, esi
        mov    eax, dword ptr [esp + 38h]
        push   eax
        mov    eax, dword ptr [esp + 38h]
        push   eax
        mov    eax, dword ptr [esp + 30h]
        push   eax
        mov    eax, dword ptr [esp + 30h]
        push   eax
        push   ecx
        push   ebp
        db  0A1h, 080h, 02Fh, 001h, 000h
        push   eax
        db  0E8h, 0D1h, 0AFh, 0FDh, 0FFh
        mov    dword ptr [esp + 34h], eax
        add    esp, 24h
        or     eax, eax
        je     SHORT loc_1_5042
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 2ch
        ret
loc_1_5042:
        db  08Ah, 00Dh, 0F4h, 033h, 001h, 000h
        cmp    cl, 0ffh
        je     SHORT loc_1_5060
        db  0A0h, 064h, 02Fh, 001h, 000h
        lea    ebx, [ecx + eax]
        inc    al
        db  0A2h, 064h, 02Fh, 001h, 000h
        jmp    SHORT loc_1_5062
        db  08Bh, 0FFh
loc_1_5060:
        mov    bl, 0ffh
loc_1_5062:
        mov    esi, dword ptr [esp + 48h]
loc_1_5066:
        xor    eax, eax
        mov    al, bl
        push   eax
        push   esi
        call   sub_1_6420
        add    esp, 8
        cmp    eax, 0ffh
        jne    SHORT loc_1_50A1
        cmp    bl, 0ffh
        je     SHORT loc_1_5084
        mov    bl, 0ffh
        jmp    SHORT loc_1_5066
loc_1_5084:
        push   ebp
        db  08Bh, 00Dh, 080h, 02Fh, 001h, 000h
        push   ecx
        call   sub_1_5110
        add    esp, 8
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 2ch
        ret
loc_1_50A1:
        db  08Bh, 00Dh, 080h, 02Fh, 001h, 000h
        push   esi
        mov    ecx, dword ptr [ecx + 0ch]
        db  089h, 00Ch, 0BDh, 0C4h, 033h, 001h, 000h
        mov    byte ptr [ecx + 51h], al
        call   sub_1_015D
        add    esp, 4
        jmp    SHORT loc_1_50FC
        db  090h
loc_1_50C0:
        mov    eax, esi
        mov    ecx, esi
        and    ecx, 0ff00h
        mov    edx, eax
        shl    edx, 10h
        or     ecx, edx
        shl    ecx, 8
        and    esi, 0ff0000h
        shr    esi, 8
        or     ecx, esi
        shr    eax, 18h
        and    eax, 0ffh
        or     ecx, eax
        push   ecx
        db  0A1h, 080h, 02Fh, 001h, 000h
        push   eax
        call   sub_1_5110
        mov    dword ptr [esp + 18h], eax
        add    esp, 8
loc_1_50FC:
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 2ch
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_5110:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    esi, esi
        mov    edx, dword ptr [esp + 24h]
        mov    dword ptr [esp + 18h], esi
        mov    edi, dword ptr [edx + 8]
        mov    eax, edx
        sub    eax, edi
        cmp    eax, -8
        je     SHORT loc_1_5149
        mov    eax, dword ptr [esp + 28h]
loc_1_5131:
        cmp    dword ptr [edi + 58h], eax
        jne    SHORT loc_1_513A
        mov    esi, edi
        jmp    SHORT loc_1_513E
loc_1_513A:
        mov    dword ptr [esp + 18h], edi
loc_1_513E:
        mov    edi, dword ptr [edi]
        mov    ecx, edx
        sub    ecx, edi
        cmp    ecx, -8
        jne    SHORT loc_1_5131
loc_1_5149:
        or     esi, esi
        jne    SHORT loc_1_515A
        mov    eax, 0c000000dh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_515A:
        mov    byte ptr [esi + 52h], 0
        push   0
        push   0
        push   esi
        call   sub_1_DC58
        add    esp, 0ch
        db  0FEh, 00Dh, 04Fh, 033h, 001h, 000h
        db  080h, 03Dh, 04Fh, 033h, 001h, 000h, 001h
        jne    SHORT loc_1_5181
        db  0C6h, 005h, 050h, 033h, 001h, 000h, 000h
loc_1_5181:
        db  080h, 03Dh, 04Fh, 033h, 001h, 000h, 000h
        jne    SHORT loc_1_51FB
        mov    eax, dword ptr [esi + 24h]
        sub    eax, esi
        cmp    eax, 24h
        je     SHORT loc_1_51BE
loc_1_5194:
        mov    edx, dword ptr [esi + 24h]
        push   0
        mov    eax, dword ptr [edx + 4]
        mov    ecx, dword ptr [edx]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   0c00000cah
        mov    eax, dword ptr [edx + 1ch]
        push   eax
        call   sub_1_25A0
        add    esp, 0ch
        mov    eax, dword ptr [esi + 24h]
        sub    eax, esi
        cmp    eax, 24h
        jne    SHORT loc_1_5194
loc_1_51BE:
        mov    eax, dword ptr [esi + 34h]
        sub    eax, esi
        cmp    eax, 34h
        je     SHORT loc_1_51FB
loc_1_51C8:
        mov    edi, dword ptr [esi + 34h]
        push   0
        mov    eax, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   0c00000cah
        mov    eax, dword ptr [edi + 8]
        push   eax
        call   sub_1_25A0
        add    esp, 0ch
        push   edi
        call   sub_1_0024
        add    esp, 4
        mov    eax, dword ptr [esi + 34h]
        sub    eax, esi
        cmp    eax, 34h
        jne    SHORT loc_1_51C8
loc_1_51FB:
        push   esi
        call   sub_1_5E10
        add    esp, 4
        push   esi
        call   sub_1_3CF0
        mov    dword ptr [esp + 20h], 0
        add    esp, 4
        db  0A1h, 0B4h, 032h, 001h, 000h
        cmp    dword ptr [eax], 0
        jle    SHORT loc_1_5295
        sub    edi, edi
loc_1_5221:
        db  0A1h, 0B4h, 032h, 001h, 000h
        lea    ebp, [edi + eax + 8]
        mov    ebx, dword ptr [ebp]
        cmp    ebp, ebx
        je     SHORT loc_1_5280
loc_1_5231:
        mov    eax, ebx
        mov    ebx, dword ptr [ebx]
        test   byte ptr [eax + 20h], 80h
        je     SHORT loc_1_527C
        mov    ecx, dword ptr [eax + 14h]
        or     ecx, ecx
        je     SHORT loc_1_527C
        mov    edx, dword ptr [ecx + 20h]
        cmp    dword ptr [edx + 2ch], esi
        jne    SHORT loc_1_527C
        mov    dword ptr [eax + 14h], 0
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        push   ecx
        call   sub_1_10AEC
        add    esp, 0ch
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_527C
        mov    eax, dword ptr [esp + 14h]
        push   0c00000cah
        push   eax
        call   dword ptr [esp + 18h]
        add    esp, 8
loc_1_527C:
        cmp    ebp, ebx
        jne    SHORT loc_1_5231
loc_1_5280:
        add    edi, 8
        inc    dword ptr [esp + 1ch]
        db  08Bh, 00Dh, 0B4h, 032h, 001h, 000h
        mov    eax, dword ptr [esp + 1ch]
        cmp    dword ptr [ecx], eax
        jg     SHORT loc_1_5221
loc_1_5295:
        sub    edi, edi
        cmp    byte ptr [esi + 4eh], 0
        jb     SHORT loc_1_52C3
loc_1_52A0:
        mov    eax, dword ptr [esi + 44h]
        mov    eax, dword ptr [eax + edi*4]
        or     eax, eax
        je     SHORT loc_1_52B9
        push   1
        push   edi
        push   eax
        push   0
        push   esi
        call   sub_1_17A0
        add    esp, 14h
loc_1_52B9:
        inc    edi
        xor    eax, eax
        mov    al, byte ptr [esi + 4eh]
        cmp    eax, edi
        jge    SHORT loc_1_52A0
loc_1_52C3:
        push   esi
        call   sub_1_4E80
        add    esp, 4
        mov    eax, dword ptr [esi + 0ch]
        sub    eax, esi
        cmp    eax, 0ch
        je     SHORT loc_1_52F6
loc_1_52D6:
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        mov    ecx, dword ptr [eax + 4]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
        call   sub_1_0024
        add    esp, 4
        mov    eax, dword ptr [esi + 0ch]
        sub    eax, esi
        cmp    eax, 0ch
        jne    SHORT loc_1_52D6
loc_1_52F6:
        mov    eax, dword ptr [esi + 14h]
        sub    eax, esi
        cmp    eax, 14h
        je     SHORT loc_1_5320
loc_1_5300:
        mov    eax, dword ptr [esi + 14h]
        push   eax
        mov    ecx, dword ptr [eax + 4]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
        call   sub_1_0024
        add    esp, 4
        mov    eax, dword ptr [esi + 14h]
        sub    eax, esi
        cmp    eax, 14h
        jne    SHORT loc_1_5300
loc_1_5320:
        sub    eax, eax
        db  0B9h, 0C4h, 033h, 001h, 000h
loc_1_5327:
        cmp    dword ptr [ecx], esi
        je     SHORT loc_1_5340
        add    ecx, 4
        inc    eax
        db  081h, 0F9h, 0E4h, 033h, 001h, 000h
        jb     SHORT loc_1_5327
        jmp    SHORT loc_1_5360
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5340:
        db  08Dh, 03Ch, 085h, 0C4h, 033h, 001h, 000h
        mov    eax, dword ptr [edi]
        mov    al, byte ptr [eax + 51h]
        and    eax, 0ffh
        push   eax
        call   sub_1_6453
        add    esp, 4
        mov    dword ptr [edi], 0
loc_1_5360:
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        db  08Bh, 00Dh, 074h, 032h, 001h, 000h
        db  081h, 0F9h, 074h, 032h, 001h, 000h
        je     SHORT loc_1_539C
        mov    edx, dword ptr [esi + 88h]
        not    edx
loc_1_5380:
        cmp    dword ptr [ecx + 1ch], esi
        jne    SHORT loc_1_538C
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [ecx + 1ch], eax
loc_1_538C:
        mov    eax, dword ptr [ecx + 14h]
        and    dword ptr [eax + 28h], edx
        mov    ecx, dword ptr [ecx]
        db  081h, 0F9h, 074h, 032h, 001h, 000h
        jne    SHORT loc_1_5380
loc_1_539C:
        mov    eax, dword ptr [esi + 44h]
        or     eax, eax
        je     SHORT loc_1_53AC
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_53AC:
        mov    eax, dword ptr [esi + 48h]
        or     eax, eax
        je     SHORT loc_1_53BC
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_53BC:
        push   esi
        call   sub_1_0024
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h
sub_1_53D0:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, 1
        sub    bl, bl
        mov    edi, edi
loc_1_53E0:
        xor    eax, eax
        mov    al, bl
        lea    edi, [esp + eax*4 + 14h]
        lea    eax, [ebx + 31h]
        mov    dword ptr [edi], 7f000000h
        db  0A2h, 072h, 02Fh, 001h, 000h
        lea    eax, [esp + 10h]
        push   2
        push   eax
        db  068h, 068h, 02Fh, 001h, 000h
        call   sub_1_5D30
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_5433
        mov    eax, dword ptr [esp + 10h]
        push   edi
        push   eax
        call   sub_1_FD20
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_5426
        mov    dword ptr [edi], 7f000000h
loc_1_5426:
        mov    eax, dword ptr [esp + 10h]
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_5433:
        inc    bl
        cmp    bl, 4
        jb     SHORT loc_1_53E0
        mov    eax, dword ptr [esp + 14h]
        db  0A3h, 03Bh, 033h, 001h, 000h
        mov    eax, dword ptr [esp + 18h]
        db  0A3h, 03Fh, 033h, 001h, 000h
        sub    dl, dl
        mov    eax, 7f000000h
loc_1_5453:
        xor    ecx, ecx
        mov    cl, dl
        mov    dword ptr [esp + ecx*4 + 14h], eax
        inc    dl
        cmp    dl, 4
        jb     SHORT loc_1_5453
        lea    eax, [esp + 10h]
        push   2
        push   eax
        db  068h, 074h, 02Fh, 001h, 000h
        call   sub_1_5D30
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_54EE
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, ecx
        mov    edi, ecx
        mov    al, byte ptr [eax]
        cmp    al, 30h
        jb     SHORT loc_1_548C
        cmp    al, 39h
        jbe    SHORT loc_1_5490
loc_1_548C:
        cmp    al, 2eh
        jne    SHORT loc_1_54DA
loc_1_5490:
        sub    bl, bl
loc_1_5492:
        or     esi, esi
        je     SHORT loc_1_54DA
loc_1_5496:
        mov    al, byte ptr [edi]
        cmp    al, 30h
        jb     SHORT loc_1_54A0
        cmp    al, 39h
        jbe    SHORT loc_1_54A4
loc_1_54A0:
        cmp    al, 2eh
        jne    SHORT loc_1_54A7
loc_1_54A4:
        inc    edi
        jmp    SHORT loc_1_5496
loc_1_54A7:
        cmp    byte ptr [edi], 2ch
        jne    SHORT loc_1_54B2
        mov    byte ptr [edi], 0
        inc    edi
        jmp    SHORT loc_1_54B4
loc_1_54B2:
        sub    esi, esi
loc_1_54B4:
        xor    eax, eax
        mov    al, bl
        lea    ebp, [esp + eax*4 + 14h]
        push   ebp
        push   ecx
        call   sub_1_FD20
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_54D1
        mov    dword ptr [ebp], 7f000000h
loc_1_54D1:
        inc    bl
        mov    ecx, edi
        cmp    bl, 4
        jb     SHORT loc_1_5492
loc_1_54DA:
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_54EE
        mov    eax, dword ptr [esp + 10h]
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_54EE:
        mov    eax, dword ptr [esp + 14h]
        db  0A3h, 043h, 033h, 001h, 000h
        mov    eax, dword ptr [esp + 18h]
        db  0A3h, 047h, 033h, 001h, 000h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_5510:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        sub    cl, cl
        mov    esi, dword ptr [esp + 28h]
loc_1_551C:
        xor    eax, eax
        mov    al, cl
        mov    dword ptr [esi + eax*4], 7f000000h
        inc    cl
        cmp    cl, 4
        jb     SHORT loc_1_551C
        db  08Bh, 00Dh, 043h, 033h, 001h, 000h
        mov    dword ptr [esi], ecx
        db  0A1h, 047h, 033h, 001h, 000h
        mov    dword ptr [esi + 4], eax
        cmp    ecx, 7f000000h
        jne    NEAR PTR loc_1_55CC
        cmp    eax, 7f000000h
        jne    SHORT loc_1_55CC
        mov    dword ptr [esp + 0ch], 10h
        lea    eax, [esp + 0ch]
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        mov    eax, dword ptr [esp + 2ch]
        push   6
        push   eax
        call   sub_1_0071
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_557B
        cmp    eax, 9
        jne    SHORT loc_1_55CC
loc_1_557B:
        sub    cl, cl
        lea    ecx, [ecx]
loc_1_5580:
        mov    edi, ecx
        and    edi, 0ffh
        lea    eax, [edi*4 + 4]
        cmp    eax, dword ptr [esp + 0ch]
        ja     SHORT loc_1_55C5
        mov    edx, dword ptr [esp + edi*4 + 10h]
        mov    ebx, edx
        and    ebx, 0ff00h
        mov    eax, edx
        shl    eax, 10h
        or     ebx, eax
        shl    ebx, 8
        mov    eax, edx
        and    eax, 0ff0000h
        shr    eax, 8
        or     ebx, eax
        shr    edx, 18h
        and    edx, 0ffh
        or     ebx, edx
        mov    dword ptr [esi + edi*4], ebx
loc_1_55C5:
        inc    cl
        cmp    cl, 4
        jb     SHORT loc_1_5580
loc_1_55CC:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_55E0:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        sub    cl, cl
        mov    esi, dword ptr [esp + 28h]
loc_1_55EC:
        xor    eax, eax
        mov    al, cl
        mov    dword ptr [esi + eax*4], 7f000000h
        inc    cl
        cmp    cl, 4
        jb     SHORT loc_1_55EC
        db  08Bh, 00Dh, 03Bh, 033h, 001h, 000h
        mov    dword ptr [esi], ecx
        db  0A1h, 03Fh, 033h, 001h, 000h
        mov    dword ptr [esi + 4], eax
        cmp    ecx, 7f000000h
        jne    NEAR PTR loc_1_569C
        cmp    eax, 7f000000h
        jne    SHORT loc_1_569C
        mov    dword ptr [esp + 0ch], 10h
        lea    eax, [esp + 0ch]
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        mov    eax, dword ptr [esp + 2ch]
        push   2ch
        push   eax
        call   sub_1_0071
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_564B
        cmp    eax, 9
        jne    SHORT loc_1_569C
loc_1_564B:
        sub    cl, cl
        lea    ecx, [ecx]
loc_1_5650:
        mov    edi, ecx
        and    edi, 0ffh
        lea    eax, [edi*4 + 4]
        cmp    eax, dword ptr [esp + 0ch]
        ja     SHORT loc_1_5695
        mov    edx, dword ptr [esp + edi*4 + 10h]
        mov    ebx, edx
        and    ebx, 0ff00h
        mov    eax, edx
        shl    eax, 10h
        or     ebx, eax
        shl    ebx, 8
        mov    eax, edx
        and    eax, 0ff0000h
        shr    eax, 8
        or     ebx, eax
        shr    edx, 18h
        and    edx, 0ffh
        or     ebx, edx
        mov    dword ptr [esi + edi*4], ebx
loc_1_5695:
        inc    cl
        cmp    cl, 4
        jb     SHORT loc_1_5650
loc_1_569C:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_56B0:
        sub    esp, 230h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 248h]
        mov    dword ptr [eax], 0
        mov    word ptr [eax + 4], 0
        mov    dword ptr [esp + 50h], 0
        mov    dword ptr [esp + 54h], 0
        mov    dword ptr [esp + 58h], 100h
        lea    eax, [esp + 140h]
        mov    dword ptr [esp + 5ch], 100h
        mov    dword ptr [esp + 60h], 0
        mov    dword ptr [esp + 10h], 100h
        mov    dword ptr [esp + 4ch], 4655424eh
        mov    dword ptr [esp + 3ch], 0
        mov    dword ptr [esp + 48h], 100h
        mov    dword ptr [esp + 40h], eax
        lea    eax, [esp + 2ch]
        mov    dword ptr [esp + 44h], 0
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        push   eax
        mov    dword ptr [eax + 0ch], 0
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 44h]
        push   eax
        lea    eax, [esp + 5ch]
        push   eax
        db  0A1h, 094h, 034h, 001h, 000h
        push   0
        call   dword ptr [eax + 48h]
        add    esp, 14h
        or     eax, eax
        jne    NEAR PTR loc_1_5BE1
        mov    eax, dword ptr [esp + 10h]
        shr    eax, 3
        mov    dword ptr [esp + 20h], eax
        sub    esi, esi
        mov    dword ptr [esp + 18h], esi
        cmp    eax, esi
        je     NEAR PTR loc_1_58CE
        lea    edi, [esp + 0c0h]
        lea    ebx, [esp + 140h]
        sub    ebp, ebp
loc_1_5795:
        mov    eax, dword ptr [ebx]
        cmp    eax, 200h
        jne    NEAR PTR loc_1_58B9
        mov    dword ptr [esp + 50h], eax
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [esp + 54h], eax
        mov    dword ptr [esp + 58h], 100h
        lea    eax, [esp + 28h]
        mov    dword ptr [esp + 5ch], 100h
        mov    dword ptr [esp + 60h], 1
        mov    dword ptr [esp + 10h], 4
        mov    dword ptr [esp + 4ch], 4655424eh
        mov    dword ptr [esp + 3ch], ebp
        mov    dword ptr [esp + 48h], 4
        mov    dword ptr [esp + 40h], eax
        lea    eax, [esp + 2ch]
        mov    dword ptr [esp + 44h], ebp
        mov    dword ptr [eax], ebp
        mov    dword ptr [eax + 4], ebp
        mov    dword ptr [eax + 8], ebp
        push   eax
        mov    dword ptr [eax + 0ch], ebp
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 44h]
        push   eax
        lea    eax, [esp + 5ch]
        push   eax
        db  0A1h, 094h, 034h, 001h, 000h
        push   ebp
        call   dword ptr [eax + 48h]
        add    esp, 14h
        cmp    eax, ebp
        jne    NEAR PTR loc_1_5BE1
        cmp    dword ptr [esp + 28h], 202h
        jne    NEAR PTR loc_1_58B9
        mov    dword ptr [esp + 10h], 0deh
        push   0deh
        call   sub_1_35F0
        add    esp, 4
        mov    dword ptr [edi], eax
        cmp    eax, ebp
        db  08Bh, 00Dh, 094h, 034h, 001h, 000h
        je     NEAR PTR loc_1_5AD3
        mov    dword ptr [esp + 58h], 200h
        mov    dword ptr [esp + 60h], 1
        mov    dword ptr [esp + 10h], 0deh
        mov    dword ptr [esp + 4ch], 4655424eh
        mov    dword ptr [esp + 3ch], ebp
        mov    dword ptr [esp + 48h], 0deh
        mov    dword ptr [esp + 40h], eax
        lea    eax, [esp + 2ch]
        mov    dword ptr [esp + 44h], ebp
        mov    dword ptr [eax], ebp
        mov    dword ptr [eax + 4], ebp
        mov    dword ptr [eax + 8], ebp
        push   eax
        mov    dword ptr [eax + 0ch], ebp
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 44h]
        push   eax
        lea    eax, [esp + 5ch]
        push   eax
        push   ebp
        call   dword ptr [ecx + 48h]
        add    esp, 14h
        cmp    eax, ebp
        jne    NEAR PTR loc_1_5B00
        add    edi, 4
        inc    esi
loc_1_58B9:
        add    ebx, 8
        inc    dword ptr [esp + 18h]
        mov    eax, dword ptr [esp + 18h]
        cmp    dword ptr [esp + 20h], eax
        ja     NEAR PTR loc_1_5795
loc_1_58CE:
        mov    dword ptr [esp + 18h], 0
        cmp    dword ptr [esp + 20h], 0
        je     NEAR PTR loc_1_5BC5
        lea    eax, [esp + 140h]
        mov    dword ptr [esp + 1ch], eax
loc_1_58EC:
        mov    eax, dword ptr [esp + 1ch]
        mov    eax, dword ptr [eax]
        cmp    eax, 301h
        jne    NEAR PTR loc_1_5BAE
        mov    dword ptr [esp + 50h], eax
        mov    eax, dword ptr [esp + 1ch]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esp + 54h], eax
        mov    dword ptr [esp + 58h], 100h
        lea    eax, [esp + 24h]
        mov    dword ptr [esp + 5ch], 100h
        mov    dword ptr [esp + 60h], 1
        mov    dword ptr [esp + 10h], 4
        mov    dword ptr [esp + 4ch], 4655424eh
        mov    dword ptr [esp + 3ch], 0
        mov    dword ptr [esp + 48h], 4
        mov    dword ptr [esp + 40h], eax
        lea    eax, [esp + 2ch]
        mov    dword ptr [esp + 44h], 0
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        push   eax
        mov    dword ptr [eax + 0ch], 0
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 44h]
        push   eax
        lea    eax, [esp + 5ch]
        push   eax
        db  0A1h, 094h, 034h, 001h, 000h
        push   0
        call   dword ptr [eax + 48h]
        add    esp, 14h
        or     eax, eax
        jne    NEAR PTR loc_1_5BEC
        cmp    dword ptr [esp + 24h], 303h
        jne    NEAR PTR loc_1_5BAE
        mov    dword ptr [esp + 58h], 200h
        mov    dword ptr [esp + 60h], 1
        mov    dword ptr [esp + 10h], 5ch
        mov    dword ptr [esp + 4ch], 4655424eh
        lea    eax, [esp + 64h]
        mov    dword ptr [esp + 3ch], 0
        mov    dword ptr [esp + 48h], 5ch
        mov    dword ptr [esp + 40h], eax
        lea    eax, [esp + 2ch]
        mov    dword ptr [esp + 44h], 0
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        push   eax
        mov    dword ptr [eax + 0ch], 0
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 44h]
        push   eax
        lea    eax, [esp + 5ch]
        push   eax
        db  0A1h, 094h, 034h, 001h, 000h
        push   0
        call   dword ptr [eax + 48h]
        add    esp, 14h
        or     eax, eax
        jne    NEAR PTR loc_1_5BAE
        cmp    dword ptr [esp + 0b8h], 1
        jb     NEAR PTR loc_1_5BAE
        mov    eax, dword ptr [esp + 0b8h]
        lea    eax, [eax + eax*2]
        shl    eax, 3
        mov    dword ptr [esp + 10h], eax
        push   eax
        call   sub_1_35F0
        mov    dword ptr [esp + 18h], eax
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_5BAE
        mov    dword ptr [esp + 60h], 102h
        mov    dword ptr [esp + 4ch], 4655424eh
        mov    dword ptr [esp + 3ch], 0
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esp + 48h], eax
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esp + 40h], eax
        lea    eax, [esp + 2ch]
        mov    dword ptr [esp + 44h], 0
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        push   eax
        mov    dword ptr [eax + 0ch], 0
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 44h]
        push   eax
        lea    eax, [esp + 5ch]
        push   eax
        db  0A1h, 094h, 034h, 001h, 000h
        push   0
        call   dword ptr [eax + 48h]
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_1_5B30
        jmp    NEAR PTR loc_1_5BA1
loc_1_5AD3:
        or     esi, esi
        je     NEAR PTR loc_1_5BE1
        lea    edi, [esp + 0c0h]
loc_1_5AE2:
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0024
        add    esp, 4
        add    edi, 4
        dec    esi
        jne    SHORT loc_1_5AE2
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 230h
        ret
        db  08Bh, 0FFh
loc_1_5B00:
        or     esi, esi
        je     NEAR PTR loc_1_5BE1
        lea    edi, [esp + 0c0h]
        nop
loc_1_5B10:
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0024
        add    esp, 4
        add    edi, 4
        dec    esi
        jne    SHORT loc_1_5B10
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 230h
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_5B30:
        sub    edx, edx
        cmp    dword ptr [esp + 0b8h], edx
        jbe    SHORT loc_1_5B9A
        mov    ecx, dword ptr [esp + 14h]
        nop
loc_1_5B40:
        mov    eax, dword ptr [esp + 244h]
        cmp    dword ptr [ecx], eax
        jne    SHORT loc_1_5B8D
        sub    edi, edi
        cmp    esi, edi
        je     SHORT loc_1_5B8D
        lea    ebx, [esp + 0c0h]
        mov    eax, dword ptr [ecx + 4]
loc_1_5B5B:
        mov    ebp, dword ptr [ebx]
        cmp    dword ptr [ebp], eax
        je     SHORT loc_1_5B70
        add    ebx, 4
        inc    edi
        cmp    esi, edi
        ja     SHORT loc_1_5B5B
        jmp    SHORT loc_1_5B8D
        db  08Dh, 064h, 024h, 000h
loc_1_5B70:
        mov    eax, dword ptr [esp + edi*4 + 0c0h]
        add    eax, 14h
        mov    ebx, dword ptr [esp + 248h]
        mov    edi, dword ptr [eax]
        mov    dword ptr [ebx], edi
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebx + 4], ax
loc_1_5B8D:
        add    ecx, 18h
        inc    edx
        cmp    dword ptr [esp + 0b8h], edx
        ja     SHORT loc_1_5B40
loc_1_5B9A:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_1_5BAE
loc_1_5BA1:
        mov    eax, dword ptr [esp + 14h]
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_5BAE:
        add    dword ptr [esp + 1ch], 8
        inc    dword ptr [esp + 18h]
        mov    eax, dword ptr [esp + 18h]
        cmp    dword ptr [esp + 20h], eax
        ja     NEAR PTR loc_1_58EC
loc_1_5BC5:
        or     esi, esi
        je     SHORT loc_1_5BE1
        lea    edi, [esp + 0c0h]
loc_1_5BD0:
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0024
        add    esp, 4
        add    edi, 4
        dec    esi
        jne    SHORT loc_1_5BD0
loc_1_5BE1:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 230h
        ret
loc_1_5BEC:
        or     esi, esi
        je     SHORT loc_1_5BE1
        lea    edi, [esp + 0c0h]
loc_1_5BF7:
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0024
        add    esp, 4
        add    edi, 4
        dec    esi
        jne    SHORT loc_1_5BF7
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 230h
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_5C20:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        push   2
        mov    edi, dword ptr [esp + 10h]
        push   esi
        push   edi
        call   sub_1_5D30
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_5C40
        sub    eax, eax
        pop    edi
        pop    esi
        ret
        db  090h
loc_1_5C40:
        push   0
        push   edi
        db  0E8h, 098h, 0A8h, 0FDh, 0FFh
        add    esp, 8
        mov    dword ptr [esi], eax
        or     eax, eax
        je     SHORT loc_1_5C56
        sub    eax, eax
        pop    edi
        pop    esi
        ret
loc_1_5C56:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
sub_1_5C60:
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 18h]
        mov    dword ptr [esp + 8], edi
        lea    eax, [esp + 8]
        push   0
        mov    esi, dword ptr [esp + 18h]
        push   eax
        push   esi
        call   sub_1_5D30
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_5C93
        push   edi
        push   esi
        db  0E8h, 054h, 0A8h, 0FDh, 0FFh
        mov    dword ptr [esp + 10h], eax
        add    esp, 8
loc_1_5C93:
        mov    eax, dword ptr [esp + 1ch]
        cmp    eax, dword ptr [esp + 8]
        jbe    SHORT loc_1_5CA1
        mov    dword ptr [esp + 8], eax
loc_1_5CA1:
        mov    eax, dword ptr [esp + 8]
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_1_5CB0:
        mov    eax, dword ptr [esp + 10h]
        push   eax
        mov    eax, dword ptr [esp + 10h]
        push   eax
        mov    eax, dword ptr [esp + 10h]
        push   eax
        mov    eax, dword ptr [esp + 10h]
        push   eax
        call   sub_1_5C60
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
sub_1_5CD0:
        sub    esp, 0ch
        push   edi
        db  0BFh, 054h, 02Fh, 001h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    word ptr [esp + 8], cx
        mov    ax, cx
        mov    word ptr [esp + 0ah], ax
        db  0C7h, 044h, 024h, 00Ch, 054h, 02Fh, 001h, 000h
        lea    eax, [esp + 8]
        push   eax
        lea    eax, [esp + 8]
        db  068h, 05Ch, 02Fh, 001h, 000h
        push   eax
        call   sub_1_12D46
        cmp    dword ptr [esp + 4], 0
        je     SHORT loc_1_5D25
        db  0C7h, 005h, 05Ch, 02Fh, 001h, 000h, 000h, 000h, 000h, 000h
        sub    eax, eax
        pop    edi
        add    esp, 0ch
        ret
loc_1_5D25:
        mov    eax, 1
        pop    edi
        add    esp, 0ch
        ret
        db  090h
sub_1_5D30:
        sub    esp, 10h
        push   esi
        push   edi
        sub    esi, esi
        mov    edx, dword ptr [esp + 1ch]
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    word ptr [esp + 10h], cx
        mov    ax, cx
        mov    word ptr [esp + 12h], ax
        mov    dword ptr [esp + 14h], edx
        mov    edi, dword ptr [esp + 24h]
        lea    eax, [esp + 10h]
        push   edi
        push   eax
        db  0A1h, 05Ch, 02Fh, 001h, 000h
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_1_12E06
        cmp    dword ptr [esp + 0ch], esi
        jne    SHORT loc_1_5DE3
        cmp    edi, 2
        mov    eax, dword ptr [esp + 8]
        jne    SHORT loc_1_5DD5
        mov    ax, word ptr [eax + 4]
        inc    ax
        push   eax
        call   sub_1_35F0
        add    esp, 4
        mov    edx, eax
        cmp    edx, esi
        je     SHORT loc_1_5DE3
        mov    eax, dword ptr [esp + 8]
        mov    ecx, 0ffffffffh
        mov    edi, dword ptr [eax + 8]
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [esp + 20h]
        mov    esi, 1
        mov    dword ptr [ecx], edx
        mov    eax, esi
        pop    edi
        pop    esi
        add    esp, 10h
        ret
loc_1_5DD5:
        mov    ecx, dword ptr [esp + 20h]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], eax
        mov    esi, 1
loc_1_5DE3:
        mov    eax, esi
        pop    edi
        pop    esi
        add    esp, 10h
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_1_5DF0:
        db  083h, 03Dh, 05Ch, 02Fh, 001h, 000h, 000h
        je     SHORT loc_1_5E0E
        db  0A1h, 05Ch, 02Fh, 001h, 000h
        push   eax
        call   sub_1_12E0C
        db  0C7h, 005h, 05Ch, 02Fh, 001h, 000h, 000h, 000h, 000h, 000h
loc_1_5E0E:
        ret
        db  090h
sub_1_5E10:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  08Bh, 03Dh, 07Ch, 032h, 001h, 000h
        db  081h, 0FFh, 07Ch, 032h, 001h, 000h
        mov    esi, dword ptr [esp + 1ch]
        je     SHORT loc_1_5E93
        sub    ebx, ebx
loc_1_5E2B:
        mov    ebp, edi
        mov    edi, dword ptr [edi]
        mov    ecx, dword ptr [ebp + 14h]
        cmp    ecx, ebx
        je     SHORT loc_1_5E8B
        mov    eax, dword ptr [ecx + 20h]
        cmp    dword ptr [eax + 2ch], esi
        jne    SHORT loc_1_5E8B
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   ecx
        call   sub_1_10AEC
        add    esp, 0ch
        cmp    dword ptr [esp + 14h], ebx
        je     SHORT loc_1_5E8B
        mov    eax, dword ptr [ebp + 4]
        mov    ecx, dword ptr [ebp]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ebp + 4], ebp
        mov    dword ptr [ebp], ebp
        mov    dword ptr [ebp + 14h], ebx
        push   ebp
        call   sub_1_DEA4
        add    esp, 4
        mov    eax, dword ptr [esp + 10h]
        push   0c00000cah
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        push   eax
        call   sub_1_E64C
        add    esp, 0ch
loc_1_5E8B:
        db  081h, 0FFh, 07Ch, 032h, 001h, 000h
        jne    SHORT loc_1_5E2B
loc_1_5E93:
        mov    dword ptr [esp + 10h], 0
        db  080h, 03Dh, 078h, 033h, 001h, 000h, 000h
        je     SHORT loc_1_5EFE
        db  083h, 03Dh, 074h, 033h, 001h, 000h, 000h
        je     SHORT loc_1_5EFE
        db  0A1h, 074h, 033h, 001h, 000h
        mov    dword ptr [esp + 10h], eax
        mov    edi, dword ptr [eax + 8]
        cmp    dword ptr [edi + 2ch], esi
        jne    SHORT loc_1_5EFE
        db  0C7h, 005h, 074h, 033h, 001h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [esp + 10h]
        mov    ebx, dword ptr [eax + 0ch]
        mov    eax, dword ptr [edi + 38h]
        push   eax
        call   sub_1_12CC0
        add    esp, 4
        push   edi
        call   sub_1_F280
        add    esp, 4
        push   0c00000cah
        mov    eax, dword ptr [esp + 18h]
        push   ebx
        push   eax
        call   sub_1_E64C
        add    esp, 0ch
loc_1_5EFE:
        db  08Bh, 03Dh, 06Ch, 033h, 001h, 000h
        db  081h, 0FFh, 06Ch, 033h, 001h, 000h
        je     SHORT loc_1_5F6E
        lea    esp, [esp]
loc_1_5F10:
        mov    dword ptr [esp + 10h], edi
        mov    eax, dword ptr [esp + 10h]
        mov    edi, dword ptr [edi]
        mov    ebx, dword ptr [eax + 8]
        cmp    dword ptr [ebx + 2ch], esi
        jne    SHORT loc_1_5F66
        mov    eax, dword ptr [eax + 4]
        mov    ecx, edi
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, dword ptr [eax + 0ch]
        mov    eax, dword ptr [ebx + 38h]
        push   eax
        call   sub_1_12CC0
        add    esp, 4
        push   ebx
        call   sub_1_F280
        add    esp, 4
        push   0c00000cah
        mov    eax, dword ptr [esp + 18h]
        push   ebp
        push   eax
        call   sub_1_E64C
        add    esp, 0ch
loc_1_5F66:
        db  081h, 0FFh, 06Ch, 033h, 001h, 000h
        jne    SHORT loc_1_5F10
loc_1_5F6E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5F80:
        push   esi
        mov    eax, dword ptr [esp + 8]
        or     eax, eax
        je     NEAR PTR loc_1_601A
        db  0B9h, 054h, 02Fh, 001h, 000h
loc_1_5F92:
        mov    dl, byte ptr [eax]
        cmp    dl, byte ptr [ecx]
        jne    SHORT loc_1_5FB2
        or     dl, dl
        je     SHORT loc_1_5FAE
        mov    dl, byte ptr [eax + 1]
        cmp    dl, byte ptr [ecx + 1]
        jne    SHORT loc_1_5FB2
        add    eax, 2
        add    ecx, 2
        or     dl, dl
        jne    SHORT loc_1_5F92
loc_1_5FAE:
        sub    eax, eax
        jmp    SHORT loc_1_5FB7
loc_1_5FB2:
        sbb    eax, eax
        sbb    eax, -1
loc_1_5FB7:
        or     eax, eax
        jne    SHORT loc_1_601A
        db  08Bh, 00Dh, 080h, 02Fh, 001h, 000h
        mov    eax, ecx
        mov    esi, dword ptr [ecx + 8]
        sub    eax, esi
        cmp    eax, -8
        je     SHORT loc_1_5FF0
loc_1_5FD0:
        mov    eax, esi
        mov    esi, dword ptr [esi]
        mov    eax, dword ptr [eax + 58h]
        push   eax
        push   ecx
        call   sub_1_5110
        add    esp, 8
        db  08Bh, 00Dh, 080h, 02Fh, 001h, 000h
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, -8
        jne    SHORT loc_1_5FD0
loc_1_5FF0:
        push   0
        call   sub_1_3CF0
        add    esp, 4
        push   0
        db  068h, 0F0h, 04Eh, 000h, 000h
        call   sub_1_645E
        add    esp, 8
        call   sub_1_6020
        push   0
        call   sub_1_0042
        add    esp, 4
        pop    esi
        ret
loc_1_601A:
        sub    eax, eax
        pop    esi
        ret
        db  08Bh, 0FFh
sub_1_6020:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_4D00
        db  081h, 03Dh, 0E4h, 033h, 001h, 000h, 0E4h, 033h, 001h, 000h
        je     SHORT loc_1_605A
        db  08Bh, 035h, 0E4h, 033h, 001h, 000h
        mov    edi, edi
loc_1_6040:
        lea    eax, [esi - 18h]
        mov    esi, dword ptr [esi]
        push   0
        push   0
        push   eax
        call   sub_1_10AEC
        add    esp, 0ch
        db  081h, 0FEh, 0E4h, 033h, 001h, 000h
        jne    SHORT loc_1_6040
loc_1_605A:
        db  081h, 03Dh, 0ECh, 033h, 001h, 000h, 0ECh, 033h, 001h, 000h
        je     SHORT loc_1_6098
        db  0BEh, 0ECh, 033h, 001h, 000h
        add    eax, 0
loc_1_6070:
        db  08Bh, 015h, 0ECh, 033h, 001h, 000h
        mov    eax, edx
        mov    ecx, dword ptr [eax]
        mov    eax, edx
        sub    edx, 18h
        mov    eax, dword ptr [eax + 4]
        push   edx
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        call   sub_1_0024
        add    esp, 4
        db  039h, 035h, 0ECh, 033h, 001h, 000h
        jne    SHORT loc_1_6070
loc_1_6098:
        db  0A1h, 0C0h, 02Eh, 001h, 000h
        or     eax, eax
        je     SHORT loc_1_60AA
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_60AA:
        db  083h, 03Dh, 02Fh, 033h, 001h, 000h, 000h
        je     SHORT loc_1_60C1
        db  0A1h, 02Fh, 033h, 001h, 000h
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_60C1:
        db  083h, 03Dh, 0D8h, 032h, 001h, 000h, 000h
        je     SHORT loc_1_60D8
        db  0A1h, 0D8h, 032h, 001h, 000h
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_60D8:
        db  083h, 03Dh, 027h, 033h, 001h, 000h, 000h
        je     SHORT loc_1_60EF
        db  0A1h, 027h, 033h, 001h, 000h
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_60EF:
        db  083h, 03Dh, 033h, 033h, 001h, 000h, 000h
        je     SHORT loc_1_6106
        db  0A1h, 033h, 033h, 001h, 000h
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_6106:
        db  083h, 03Dh, 037h, 033h, 001h, 000h, 000h
        je     SHORT loc_1_611D
        db  0A1h, 037h, 033h, 001h, 000h
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_611D:
        db  081h, 03Dh, 074h, 032h, 001h, 000h, 074h, 032h, 001h, 000h
        je     NEAR PTR loc_1_61D0
        lea    ecx, [ecx]
loc_1_6130:
        db  08Bh, 015h, 074h, 032h, 001h, 000h
        mov    eax, edx
        mov    dword ptr [esp + 10h], edx
        mov    ecx, dword ptr [eax]
        mov    eax, edx
        lea    esi, [edx + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esi]
        sub    eax, edx
        cmp    eax, 0ch
        je     SHORT loc_1_61B3
loc_1_6154:
        mov    edi, dword ptr [esi]
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [edi + 4]
        lea    ebx, [edi + 1ch]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebx]
        sub    eax, edi
        cmp    eax, 1ch
        je     SHORT loc_1_619F
loc_1_6170:
        mov    ebp, dword ptr [ebx]
        mov    ecx, dword ptr [ebp]
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        or     eax, eax
        je     SHORT loc_1_618D
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_618D:
        push   ebp
        call   sub_1_0024
        add    esp, 4
        mov    eax, dword ptr [ebx]
        sub    eax, edi
        cmp    eax, 1ch
        jne    SHORT loc_1_6170
loc_1_619F:
        push   edi
        call   sub_1_0024
        add    esp, 4
        mov    eax, dword ptr [esi]
        sub    eax, dword ptr [esp + 10h]
        cmp    eax, 0ch
        jne    SHORT loc_1_6154
loc_1_61B3:
        mov    eax, dword ptr [esp + 10h]
        push   eax
        call   sub_1_0024
        add    esp, 4
        db  081h, 03Dh, 074h, 032h, 001h, 000h, 074h, 032h, 001h, 000h
        jne    NEAR PTR loc_1_6130
loc_1_61D0:
        db  081h, 03Dh, 08Ch, 034h, 001h, 000h, 08Ch, 034h, 001h, 000h
        je     SHORT loc_1_6216
        db  0BFh, 08Ch, 034h, 001h, 000h
loc_1_61E1:
        db  08Bh, 035h, 08Ch, 034h, 001h, 000h
        mov    eax, esi
        mov    ecx, dword ptr [eax]
        mov    eax, esi
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esi + 0ch]
        or     eax, eax
        je     SHORT loc_1_6205
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_6205:
        push   esi
        call   sub_1_0024
        add    esp, 4
        db  039h, 03Dh, 08Ch, 034h, 001h, 000h
        jne    SHORT loc_1_61E1
loc_1_6216:
        db  083h, 03Dh, 0B4h, 032h, 001h, 000h, 000h
        je     SHORT loc_1_6282
        sub    edi, edi
        sub    esi, esi
loc_1_6223:
        db  0A1h, 0B4h, 032h, 001h, 000h
        cmp    dword ptr [eax], edi
        jle    SHORT loc_1_6274
        lea    ebx, [esi + 8]
        mov    eax, dword ptr [eax + esi + 8]
        db  02Bh, 005h, 0B4h, 032h, 001h, 000h
        cmp    eax, ebx
        je     SHORT loc_1_626F
loc_1_6240:
        db  0A1h, 0B4h, 032h, 001h, 000h
        mov    edx, dword ptr [esi + eax + 8]
        push   edx
        mov    eax, dword ptr [edx + 4]
        mov    ecx, dword ptr [edx]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        call   sub_1_0024
        add    esp, 4
        db  0A1h, 0B4h, 032h, 001h, 000h
        mov    eax, dword ptr [esi + eax + 8]
        db  02Bh, 005h, 0B4h, 032h, 001h, 000h
        cmp    eax, ebx
        jne    SHORT loc_1_6240
loc_1_626F:
        mov    esi, ebx
        inc    edi
        jmp    SHORT loc_1_6223
loc_1_6274:
        db  0A1h, 0B4h, 032h, 001h, 000h
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_6282:
        db  083h, 03Dh, 0B8h, 032h, 001h, 000h, 000h
        je     SHORT loc_1_62F2
        sub    edi, edi
        sub    esi, esi
        nop
loc_1_6290:
        db  0A1h, 0B8h, 032h, 001h, 000h
        cmp    dword ptr [eax], edi
        jle    SHORT loc_1_62E4
        lea    ebx, [esi + 8]
        mov    eax, dword ptr [eax + esi + 8]
        db  02Bh, 005h, 0B8h, 032h, 001h, 000h
        cmp    eax, ebx
        je     SHORT loc_1_62DF
        lea    ebx, [ebx]
loc_1_62B0:
        db  0A1h, 0B8h, 032h, 001h, 000h
        mov    ecx, dword ptr [esi + eax + 8]
        push   ecx
        mov    eax, dword ptr [ecx + 4]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [eax], edx
        mov    dword ptr [edx + 4], eax
        call   sub_1_0024
        add    esp, 4
        db  0A1h, 0B8h, 032h, 001h, 000h
        mov    eax, dword ptr [esi + eax + 8]
        db  02Bh, 005h, 0B8h, 032h, 001h, 000h
        cmp    eax, ebx
        jne    SHORT loc_1_62B0
loc_1_62DF:
        mov    esi, ebx
        inc    edi
        jmp    SHORT loc_1_6290
loc_1_62E4:
        db  0A1h, 0B8h, 032h, 001h, 000h
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_62F2:
        db  081h, 03Dh, 064h, 032h, 001h, 000h, 064h, 032h, 001h, 000h
        je     SHORT loc_1_6328
        db  0BEh, 064h, 032h, 001h, 000h
loc_1_6303:
        db  08Bh, 015h, 064h, 032h, 001h, 000h
        mov    eax, edx
        push   edx
        mov    ecx, dword ptr [eax]
        mov    eax, edx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        call   sub_1_0024
        add    esp, 4
        db  039h, 035h, 064h, 032h, 001h, 000h
        jne    SHORT loc_1_6303
loc_1_6328:
        db  081h, 03Dh, 07Ch, 032h, 001h, 000h, 07Ch, 032h, 001h, 000h
        je     SHORT loc_1_6365
        db  0BEh, 07Ch, 032h, 001h, 000h
        lea    esp, [esp]
loc_1_6340:
        db  08Bh, 015h, 07Ch, 032h, 001h, 000h
        mov    eax, edx
        push   edx
        mov    ecx, dword ptr [eax]
        mov    eax, edx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        call   sub_1_0024
        add    esp, 4
        db  039h, 035h, 07Ch, 032h, 001h, 000h
        jne    SHORT loc_1_6340
loc_1_6365:
        db  081h, 03Dh, 08Ch, 032h, 001h, 000h, 08Ch, 032h, 001h, 000h
        je     SHORT loc_1_639B
        db  0BEh, 08Ch, 032h, 001h, 000h
loc_1_6376:
        db  08Bh, 015h, 08Ch, 032h, 001h, 000h
        mov    eax, edx
        push   edx
        mov    ecx, dword ptr [eax]
        mov    eax, edx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        call   sub_1_0024
        add    esp, 4
        db  039h, 035h, 08Ch, 032h, 001h, 000h
        jne    SHORT loc_1_6376
loc_1_639B:
        db  081h, 03Dh, 094h, 032h, 001h, 000h, 094h, 032h, 001h, 000h
        je     SHORT loc_1_63D5
        db  0BEh, 094h, 032h, 001h, 000h
        lea    esp, [esp]
loc_1_63B0:
        db  08Bh, 015h, 094h, 032h, 001h, 000h
        mov    eax, edx
        push   edx
        mov    ecx, dword ptr [eax]
        mov    eax, edx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        call   sub_1_0024
        add    esp, 4
        db  039h, 035h, 094h, 032h, 001h, 000h
        jne    SHORT loc_1_63B0
loc_1_63D5:
        db  081h, 03Dh, 09Ch, 032h, 001h, 000h, 09Ch, 032h, 001h, 000h
        je     SHORT loc_1_640B
        db  0BEh, 09Ch, 032h, 001h, 000h
loc_1_63E6:
        db  08Bh, 015h, 09Ch, 032h, 001h, 000h
        mov    eax, edx
        push   edx
        mov    ecx, dword ptr [eax]
        mov    eax, edx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        call   sub_1_0024
        add    esp, 4
        db  039h, 035h, 09Ch, 032h, 001h, 000h
        jne    SHORT loc_1_63E6
loc_1_640B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_6420:
        db  0CDh, 020h, 000h, 000h, 014h, 000h
        jae    SHORT loc_1_642F
        mov    eax, 0ffh
        jmp    SHORT loc_1_6452
loc_1_642F:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 8]
        push   ebx
        push   edx
        mov    ebx, 1
        db  0BAh, 064h, 000h, 000h, 000h
        db  0CDh, 020h, 005h, 000h, 014h, 000h
        jae    SHORT loc_1_6450
        mov    eax, 0ffh
loc_1_6450:
        pop    edx
        pop    ebx
loc_1_6452:
        ret
sub_1_6453:
        mov    eax, dword ptr [esp + 4]
        db  0CDh, 020h, 004h, 000h, 014h, 000h
        ret
sub_1_645E:
        mov    eax, dword ptr [esp + 8]
        push   eax
        mov    eax, dword ptr [esp + 8]
        push   eax
        db  0CDh, 020h, 006h, 000h, 089h, 004h
        add    esp, 8
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_6480:
        push   edi
        push   esi
        push   ebx
        mov    ecx, 0
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        mov    edx, dword ptr [esp + 10h]
        mov    eax, 3d02h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_1_64A3
        jmp    SHORT loc_1_64A8
loc_1_64A3:
        mov    eax, 0
loc_1_64A8:
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        pop    ebx
        pop    esi
        pop    edi
        ret
sub_1_64B2:
        push   edi
        push   esi
        push   ebx
        mov    ecx, 0
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        mov    ebx, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 14h]
        mov    edx, dword ptr [esp + 18h]
        mov    eax, 3f00h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_1_64DD
        jmp    SHORT loc_1_64E2
loc_1_64DD:
        mov    eax, 0
loc_1_64E2:
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        pop    ebx
        pop    esi
        pop    edi
        ret
sub_1_64EC:
        push   edi
        push   esi
        push   ebx
        mov    ecx, 0
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        mov    ebx, dword ptr [esp + 10h]
        mov    eax, 3e00h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_1_650F
        jmp    SHORT loc_1_6514
loc_1_650F:
        mov    eax, 0
loc_1_6514:
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        pop    ebx
        pop    esi
        pop    edi
        ret
sub_1_651E:
        push   edi
        push   esi
        push   ebx
        mov    ecx, 0
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        db  0CDh, 020h, 0B7h, 000h, 001h, 000h
        mov    eax, edx
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        pop    ebx
        pop    esi
        pop    edi
        ret
        db  000h, 000h
sub_1_6540:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        lea    eax, [esi*8 + 8]
        push   eax
        call   sub_1_35F0
        add    esp, 4
        mov    edx, dword ptr [esp + 10h]
        mov    dword ptr [edx], eax
        or     eax, eax
        je     SHORT loc_1_6594
        or     esi, esi
        jle    SHORT loc_1_6583
        sub    edi, edi
        mov    ecx, esi
loc_1_6569:
        mov    eax, dword ptr [edx]
        add    eax, edi
        lea    ebx, [eax + 8]
        mov    dword ptr [eax + 0ch], ebx
        mov    eax, dword ptr [edx]
        add    eax, edi
        mov    ebx, dword ptr [eax + 0ch]
        mov    dword ptr [eax + 8], ebx
        add    edi, 8
        dec    ecx
        jne    SHORT loc_1_6569
loc_1_6583:
        mov    eax, dword ptr [edx]
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax], esi
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_6594:
        pop    edi
        mov    eax, 1
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_1_65A0:
        push   1
        push   dword ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 0ch]
        add    eax, 84h
        push   eax
        call   sub_1_6540
        add    esp, 0ch
        sub    eax, eax
        ret
        db  090h
sub_1_65BC:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 28h]
        push   esi
        mov    ebp, dword ptr [esp + 20h]
        push   ebp
        mov    ebx, dword ptr [esp + 20h]
        push   ebx
        mov    edi, dword ptr [esp + 20h]
        push   edi
        call   sub_1_6954
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_65EC
        mov    eax, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_65EC:
        push   esi
        push   0
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 2ch]
        push   ebp
        push   ebx
        push   edi
        call   sub_1_6678
        add    esp, 1ch
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
sub_1_6608:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        lea    edi, [esi + 34h]
        lea    eax, [esp + 0ch]
        push   eax
        mov    ebx, dword ptr [esp + 1ch]
        push   ebx
        push   edi
        db  0FFh, 035h, 0B8h, 032h, 001h, 000h
        call   sub_1_6954
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_6654
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esi + 0ch]
        cmp    dword ptr [eax + 0ch], ecx
        jne    SHORT loc_1_6646
        mov    ecx, eax
        mov    eax, dword ptr [esi + 28h]
        or     dword ptr [ecx + 28h], eax
loc_1_6646:
        pop    edi
        mov    eax, 0c0000001h
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_6654:
        lea    eax, [esp + 0ch]
        push   eax
        push   esi
        push   0
        push   0
        push   ebx
        push   edi
        db  0FFh, 035h, 0B8h, 032h, 001h, 000h
        call   sub_1_6678
        add    esp, 1ch
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
sub_1_6678:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 1ch]
        mov    al, byte ptr [esi + 1]
        and    eax, 0fh
        mov    cl, byte ptr [esi]
        and    ecx, 0fh
        shl    ecx, 4
        add    eax, ecx
        cdq
        mov    ecx, dword ptr [esp + 18h]
        idiv   dword ptr [ecx]
        mov    ebp, edx
        mov    ebx, dword ptr [esp + 2ch]
        or     ebx, ebx
        jne    SHORT loc_1_670C
        push   44h
        call   sub_1_001E
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_1_67B4
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, 11h
        rep stosd
        mov    dword ptr [ebx + 1ch], 1
        mov    ecx, dword ptr [esp + 18h]
        mov    eax, dword ptr [ecx + 4]
        or     eax, eax
        je     SHORT loc_1_66E1
        cmp    eax, 2
        mov    dword ptr [ebx + 24h], 0facf0000h
        jne    SHORT loc_1_66E8
loc_1_66E1:
        mov    dword ptr [ebx + 24h], 0dead0000h
loc_1_66E8:
        cmp    dword ptr [esp + 28h], 1
        sbb    eax, eax
        and    eax, 0fffffffeh
        add    eax, 4
        mov    dword ptr [ebx + 20h], eax
        or     eax, 40h
        mov    dword ptr [ebx + 20h], eax
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [ebx + 0ch], eax
        lea    edi, [ebx + 34h]
        movsd
        movsd
        movsd
        movsd
loc_1_670C:
        mov    dword ptr [ebx + 14h], 0
        db  066h, 0A1h, 01Ch, 033h, 001h, 000h
        mov    word ptr [ebx + 30h], ax
        mov    ecx, dword ptr [esp + 18h]
        lea    ecx, [ecx + ebp*8 + 8]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebx], eax
        mov    dword ptr [ebx + 4], ecx
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [ecx], ebx
        mov    ecx, dword ptr [esp + 18h]
        cmp    dword ptr [ecx + 4], 0
        je     NEAR PTR loc_1_6878
        mov    esi, dword ptr [esp + 20h]
        cmp    byte ptr [esi], 0
        je     NEAR PTR loc_1_6878
        lea    eax, [esp + 10h]
        push   eax
        push   0
        push   esi
        push   dword ptr [esp + 24h]
        call   sub_1_6954
        add    esp, 10h
        or     eax, eax
        je     NEAR PTR loc_1_686C
        cmp    eax, 0ffh
        je     NEAR PTR loc_1_686C
        sub    ebp, ebp
        mov    eax, esi
        cmp    byte ptr [esi], 0
        je     SHORT loc_1_678B
loc_1_677C:
        cmp    ebp, 0ffh
        jg     SHORT loc_1_678B
        inc    ebp
        inc    eax
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_677C
loc_1_678B:
        cmp    ebp, 0efh
        jle    SHORT loc_1_67C4
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   ebx
        call   sub_1_0024
        add    esp, 4
        mov    eax, 0c0000001h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
loc_1_67B4:
        pop    ebp
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_67C4:
        inc    ebp
        lea    eax, [ebp + 34h]
        and    eax, 0ffffh
        push   eax
        call   sub_1_001E
        add    esp, 4
        mov    dword ptr [esp + 10h], eax
        or     eax, eax
        jne    SHORT loc_1_67F4
        push   ebx
        call   sub_1_0024
        add    esp, 4
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_67F4:
        mov    ecx, ebp
        mov    edi, dword ptr [esp + 10h]
        add    edi, 34h
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 20h], 1010h
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 1ch], 2
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 14h], ebp
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ebx + 8], eax
        mov    eax, dword ptr [esp + 10h]
        mov    cl, byte ptr [eax + 34h]
        and    ecx, 0fh
        shl    ecx, 4
        mov    al, byte ptr [eax + 35h]
        and    eax, 0fh
        add    eax, ecx
        cdq
        mov    ecx, dword ptr [esp + 18h]
        idiv   dword ptr [ecx]
        lea    edx, [ecx + edx*8 + 8]
        mov    ecx, dword ptr [edx + 4]
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [edx + 4], eax
        jmp    SHORT loc_1_687F
loc_1_686C:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ebx + 8], eax
        jmp    SHORT loc_1_687F
        db  08Dh, 049h, 000h
loc_1_6878:
        mov    dword ptr [ebx + 8], 0
loc_1_687F:
        mov    eax, dword ptr [esp + 30h]
        mov    dword ptr [eax], ebx
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h, 083h, 0ECh, 004h, 08Dh, 044h, 024h, 000h, 050h, 06Ah, 000h, 0FFh, 074h, 024h, 014h, 0FFh
        db  074h, 024h, 014h, 0E8h, 0ADh, 000h, 000h, 000h, 083h, 0C4h, 010h, 00Bh, 0C0h, 074h, 012h, 03Dh
        db  0FFh, 000h, 000h, 000h, 074h, 00Bh, 0B8h, 001h, 000h, 000h, 0C0h, 083h, 0C4h, 004h, 0C3h, 08Bh
        db  0FFh, 08Bh, 044h, 024h, 000h, 081h, 060h, 020h, 00Fh, 0FFh, 0FFh, 0FFh, 08Bh, 044h, 024h, 000h
        db  083h, 048h, 020h, 040h, 0FFh, 074h, 024h, 000h, 0E8h, 0C8h, 075h, 000h, 000h, 083h, 0C4h, 004h
        db  02Bh, 0C0h, 083h, 0C4h, 004h, 0C3h, 08Dh, 049h, 000h, 083h, 0ECh, 004h, 056h, 08Dh, 044h, 024h
        db  004h, 050h, 0FFh, 074h, 024h, 018h, 0FFh, 074h, 024h, 018h, 0FFh, 074h, 024h, 018h, 0E8h, 052h
        db  000h, 000h, 000h, 083h, 0C4h, 010h, 00Bh, 0C0h, 074h, 013h, 03Dh, 0FFh, 000h, 000h, 000h, 074h
        db  00Ch, 0B8h, 001h, 000h, 000h, 0C0h, 05Eh, 083h, 0C4h, 004h, 0C3h, 08Bh, 0FFh, 08Bh, 044h, 024h
        db  004h, 081h, 060h, 020h, 00Fh, 0FFh, 0FFh, 0FFh, 08Bh, 044h, 024h, 004h, 08Bh, 074h, 024h, 018h
        db  009h, 070h, 020h, 083h, 0FEh, 020h, 075h, 013h, 0FFh, 074h, 024h, 004h, 0E8h, 064h, 075h, 000h
        db  000h, 083h, 0C4h, 004h, 08Bh, 044h, 024h, 004h, 009h, 070h, 020h, 02Bh, 0C0h, 05Eh, 083h, 0C4h
        db  004h, 0C3h, 08Dh, 049h, 000h
sub_1_6954:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 1ch]
        mov    cl, byte ptr [eax]
        and    ecx, 0fh
        shl    ecx, 4
        mov    al, byte ptr [eax + 1]
        and    eax, 0fh
        add    eax, ecx
        cdq
        mov    esi, dword ptr [esp + 18h]
        idiv   dword ptr [esi]
        lea    eax, [esi + edx*8 + 8]
        mov    dword ptr [esp + 10h], eax
        mov    edi, eax
        mov    ebx, dword ptr [esp + 20h]
loc_1_6984:
        mov    edi, dword ptr [edi]
        cmp    edi, dword ptr [esp + 10h]
        je     NEAR PTR loc_1_6A24
        test   byte ptr [edi + 21h], 10h
        mov    ebp, 10h
        je     SHORT loc_1_699E
        mov    ebp, dword ptr [edi + 14h]
loc_1_699E:
        push   ebp
        lea    eax, [edi + 34h]
        push   eax
        push   dword ptr [esp + 24h]
        call   sub_1_0262
        add    esp, 0ch
        cmp    eax, ebp
        jne    SHORT loc_1_6984
        or     ebx, ebx
        je     SHORT loc_1_6A14
        db  03Bh, 035h, 0B4h, 032h, 001h, 000h
        jne    SHORT loc_1_69D0
        db  08Bh, 00Dh, 0D8h, 032h, 001h, 000h
        db  00Fh, 0B7h, 02Dh, 0D4h, 032h, 001h, 000h
        jmp    SHORT loc_1_69F2
        db  08Bh, 0FFh
loc_1_69D0:
        mov    eax, dword ptr [edi + 8]
        or     eax, eax
        jne    SHORT loc_1_69EC
        cmp    byte ptr [ebx], 0
        jne    SHORT loc_1_6984
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [eax], edi
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_69EC:
        lea    ecx, [eax + 34h]
        mov    ebp, dword ptr [eax + 14h]
loc_1_69F2:
        push   ebp
        push   ecx
        push   ebx
        call   sub_1_0262
        add    esp, 0ch
        cmp    eax, ebp
        jne    SHORT loc_1_6984
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [eax], edi
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_6A14:
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [eax], edi
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_6A24:
        pop    ebp
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
sub_1_6A34:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 30h]
        mov    dword ptr [esi], 0
        mov    edi, dword ptr [esp + 20h]
        mov    ebx, dword ptr [edi + 0ch]
        cmp    dword ptr [ebx + 44h], 0
        jne    SHORT loc_1_6AC0
        mov    ecx, dword ptr [esp + 34h]
        mov    al, byte ptr [ecx]
        or     al, al
        jne    SHORT loc_1_6A9C
        mov    eax, dword ptr [ecx]
        mov    edx, eax
        and    edx, 0ff00h
        shl    edx, 8
        mov    ebp, eax
        and    ebp, 0ff0000h
        shr    ebp, 8
        or     edx, ebp
        shr    eax, 18h
        and    eax, 0ffh
        or     edx, eax
        mov    dword ptr [ebx + 3ch], edx
        add    ecx, 4
        sub    dword ptr [esp + 28h], 4
        mov    ebp, dword ptr [esp + 2ch]
        sub    ebp, 4
        mov    dword ptr [esi], 4
        jmp    SHORT loc_1_6AC8
        db  08Bh, 0FFh
loc_1_6A9C:
        cmp    al, 85h
        jne    SHORT loc_1_6AB0
        mov    dword ptr [esi], 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_6AB0:
        sub    eax, eax
        mov    ecx, dword ptr [esp + 28h]
        mov    dword ptr [esi], ecx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_6AC0:
        mov    ebp, dword ptr [esp + 2ch]
        mov    ecx, dword ptr [esp + 34h]
loc_1_6AC8:
        mov    edx, dword ptr [ebx + 1ch]
        mov    eax, edx
        sub    eax, ebx
        cmp    eax, 1ch
        je     SHORT loc_1_6AF4
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    esi, dword ptr [esp + 38h]
        mov    dword ptr [esi], edx
        mov    eax, 18h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_6AF4:
        mov    edx, dword ptr [ebx + 10h]
        cmp    dword ptr [edx + 50h], 0
        je     NEAR PTR loc_1_6C20
        mov    eax, dword ptr [ebx + 3ch]
        sub    eax, dword ptr [ebx + 44h]
        cmp    ebp, eax
        jb     SHORT loc_1_6B20
        mov    ebp, eax
        cmp    dword ptr [esp + 28h], eax
        jbe    SHORT loc_1_6B17
        mov    dword ptr [esp + 28h], eax
loc_1_6B17:
        or     word ptr [esp + 24h], 0c00h
        jmp    SHORT loc_1_6B2E
loc_1_6B20:
        and    word ptr [esp + 24h], 0fbffh
        or     word ptr [esp + 24h], 800h
loc_1_6B2E:
        lea    eax, [esp + 14h]
        mov    dword ptr [esp + 10h], 0
        push   eax
        push   ecx
        lea    eax, [esp + 18h]
        push   eax
        push   ebp
        push   dword ptr [esp + 38h]
        movzx  eax, word ptr [esp + 38h]
        push   eax
        push   dword ptr [ebx + 24h]
        push   dword ptr [edx + 54h]
        call   dword ptr [edx + 50h]
        add    esp, 20h
        cmp    dword ptr [edi + 0ch], 0
        jne    SHORT loc_1_6B88
        cmp    eax, 18h
        jne    NEAR PTR loc_1_6C25
        push   0
        push   0c0000120h
        push   dword ptr [esp + 1ch]
        call   sub_1_25A0
        add    esp, 0ch
        mov    dword ptr [esi], ebp
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_6B88:
        cmp    eax, 18h
        jne    SHORT loc_1_6BAC
        mov    ecx, dword ptr [esp + 10h]
        add    dword ptr [ebx + 44h], ecx
        mov    ecx, dword ptr [esp + 10h]
        add    dword ptr [esi], ecx
        mov    ecx, dword ptr [esp + 14h]
        mov    esi, dword ptr [esp + 38h]
        mov    dword ptr [esi], ecx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_6BAC:
        or     eax, eax
        jne    SHORT loc_1_6BE8
        mov    edx, dword ptr [ebx + 44h]
        add    edx, dword ptr [esp + 10h]
        mov    dword ptr [ebx + 44h], edx
        mov    ecx, dword ptr [esp + 10h]
        add    dword ptr [esi], ecx
        mov    ecx, dword ptr [ebx + 3ch]
        cmp    ecx, edx
        jne    SHORT loc_1_6BD8
        mov    dword ptr [ebx + 44h], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_6BD8:
        jae    SHORT loc_1_6C25
        mov    ecx, dword ptr [esp + 28h]
        mov    dword ptr [esi], ecx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_6BE8:
        cmp    eax, 17h
        jne    SHORT loc_1_6C25
        cmp    dword ptr [ebx + 3ch], 0
        jne    SHORT loc_1_6C10
        cmp    dword ptr [ebx + 68h], 7
        jne    SHORT loc_1_6C10
        mov    word ptr [edi + 32h], 3
        mov    dword ptr [ebx + 48h], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_6C10:
        cmp    dword ptr [esi], 0
        je     SHORT loc_1_6C25
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  090h
loc_1_6C20:
        mov    eax, 17h
loc_1_6C25:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
sub_1_6C30:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 30h]
        mov    ecx, dword ptr [esp + 38h]
        mov    dword ptr [ecx], edx
        mov    esi, dword ptr [esp + 3ch]
        mov    cl, byte ptr [esi]
        cmp    cl, 85h
        jne    SHORT loc_1_6C58
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Bh, 0FFh
loc_1_6C58:
        mov    eax, dword ptr [esp + 28h]
        cmp    dword ptr [eax + 38h], 4
        jne    NEAR PTR loc_1_6E8C
        cmp    cl, 81h
        je     SHORT loc_1_6C90
        push   1
        push   8fh
        push   83h
        push   dword ptr [esp + 34h]
        call   sub_1_72A4
        add    esp, 10h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
loc_1_6C90:
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   edx
        push   dword ptr [esp + 38h]
        push   esi
        call   sub_1_7324
        add    esp, 18h
        or     eax, eax
        je     SHORT loc_1_6CD0
        push   1
        push   eax
        push   83h
        push   dword ptr [esp + 34h]
        call   sub_1_72A4
        add    esp, 10h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h
loc_1_6CD0:
        mov    eax, dword ptr [esp + 10h]
        mov    esi, dword ptr [eax + 2ch]
        mov    eax, esi
        sub    eax, dword ptr [esp + 10h]
        cmp    eax, 2ch
        je     NEAR PTR loc_1_6E48
        sub    ebx, ebx
        mov    eax, dword ptr [esp + 10h]
        sub    eax, esi
        cmp    eax, -2ch
        je     SHORT loc_1_6D2B
        nop
loc_1_6CF4:
        mov    eax, dword ptr [esi + 1ch]
        or     eax, eax
        je     NEAR PTR loc_1_6DD8
        mov    eax, dword ptr [eax + 14h]
        or     eax, eax
        je     NEAR PTR loc_1_6DD8
        push   10h
        mov    ecx, dword ptr [esp + 18h]
        add    ecx, 0ah
        push   ecx
        add    eax, 0ah
        push   eax
        call   sub_1_0262
        add    esp, 0ch
        cmp    eax, 10h
        jne    NEAR PTR loc_1_6DDE
        mov    ebx, esi
loc_1_6D2B:
        or     ebx, ebx
        je     NEAR PTR loc_1_6E48
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [ebx + 20h]
        or     eax, eax
        je     SHORT loc_1_6D61
        mov    edi, dword ptr [eax + 14h]
        or     edi, edi
        je     SHORT loc_1_6D61
        mov    eax, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 14h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_1_6D61:
        mov    ebp, dword ptr [ebx + 10h]
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [ebp + 0ch], eax
        mov    dword ptr [ebp + 68h], 5
        mov    dword ptr [ebp + 60h], 0
        mov    dword ptr [eax + 0ch], ebp
        mov    dword ptr [eax + 38h], 5
        mov    word ptr [eax + 32h], 0
        mov    ecx, dword ptr [ebp]
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [ebp + 10h]
        add    ecx, 1ch
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [ebp], ecx
        mov    dword ptr [ebp + 4], eax
        mov    dword ptr [eax], ebp
        mov    dword ptr [ecx + 4], ebp
        mov    esi, dword ptr [esp + 14h]
        lea    edi, [ebp + 28h]
        add    esi, 0ah
        movsd
        movsd
        movsd
        movsd
        test   byte ptr [ebx + 18h], 1
        jne    SHORT loc_1_6DF4
        push   dword ptr [esp + 1ch]
        push   dword ptr [ebx + 8]
        push   ebp
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 20h]
        call   sub_1_6F0C
        add    esp, 14h
        jmp    SHORT loc_1_6E12
        db  08Bh, 0FFh
loc_1_6DD8:
        or     ebx, ebx
        jne    SHORT loc_1_6DDE
        mov    ebx, esi
loc_1_6DDE:
        mov    esi, dword ptr [esi]
        mov    eax, dword ptr [esp + 10h]
        sub    eax, esi
        cmp    eax, -2ch
        jne    NEAR PTR loc_1_6CF4
        jmp    NEAR PTR loc_1_6D2B
loc_1_6DF4:
        push   ebp
        inc    dword ptr [ebp + 64h]
        push   dword ptr [esp + 2ch]
        call   sub_1_6E98
        add    esp, 8
        push   ebp
        push   0
        push   dword ptr [ebx + 8]
        call   sub_1_25A0
        add    esp, 0ch
loc_1_6E12:
        push   dword ptr [esp + 14h]
        call   sub_1_0024
        add    esp, 4
        push   ebx
        call   sub_1_0024
        add    esp, 4
        push   dword ptr [esp + 10h]
        call   sub_1_DD58
        add    esp, 4
        mov    eax, dword ptr [esp + 34h]
        mov    ecx, dword ptr [esp + 38h]
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h
loc_1_6E48:
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [eax + 4ch], 0
        jne    SHORT loc_1_6E80
        push   1
        push   80h
        push   83h
        push   dword ptr [esp + 34h]
        call   sub_1_72A4
        add    esp, 10h
        push   dword ptr [esp + 10h]
        call   sub_1_DD58
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_6E80:
        push   dword ptr [esp + 14h]
        call   sub_1_0024
        add    esp, 4
loc_1_6E8C:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Bh, 0FFh
sub_1_6E98:
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx + 48h], 0
        mov    dword ptr [ecx + 40h], 0
        mov    dword ptr [ecx + 44h], 0
        mov    dword ptr [ecx + 3ch], 0
        mov    dword ptr [ecx + 50h], 0
        mov    dword ptr [ecx + 60h], 0
        mov    dword ptr [ecx + 54h], 0
        mov    dword ptr [ecx + 5ch], 0
        mov    dword ptr [ecx + 58h], 0
        mov    byte ptr [ecx + 6fh], 0
        mov    byte ptr [ecx + 70h], 0
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [ecx + 0ch], eax
        lea    edx, [ecx + 1ch]
        mov    dword ptr [ecx + 20h], edx
        mov    dword ptr [edx], edx
        mov    dword ptr [eax + 0ch], ecx
        mov    word ptr [eax + 32h], 0
        mov    dword ptr [eax + 20h], 0
        mov    dword ptr [eax + 24h], 0
        ret
        db  08Bh, 0FFh
sub_1_6F0C:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   esi
        mov    edi, dword ptr [esp + 14h]
        push   edi
        call   sub_1_6E98
        add    esp, 8
        mov    dword ptr [esi + 68h], 7
        mov    dword ptr [edi + 38h], 7
        mov    ecx, dword ptr [esi + 10h]
        add    ecx, 1ch
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [esi], ecx
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [eax], esi
        mov    dword ptr [ecx + 4], esi
        inc    dword ptr [esi + 64h]
        push   0
        push   82h
        push   edi
        call   sub_1_11788
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_6FB4
        cmp    eax, 0ffh
        je     SHORT loc_1_6FB4
        push   1
        push   83h
        push   83h
        push   edi
        call   sub_1_72A4
        add    esp, 10h
        push   esi
        call   sub_1_BCD4
        add    esp, 4
        lea    eax, [esp + 18h]
        push   eax
        push   esi
        call   sub_1_727C
        add    esp, 8
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_1_6FC3
        push   0
        push   0c0000001h
        push   dword ptr [esp + 20h]
        call   sub_1_25A0
        add    esp, 0ch
        sub    eax, eax
        pop    edi
        pop    esi
        ret
loc_1_6FB4:
        push   esi
        push   0
        push   dword ptr [esp + 20h]
        call   sub_1_25A0
        add    esp, 0ch
loc_1_6FC3:
        sub    eax, eax
        pop    edi
        pop    esi
        ret
sub_1_6FC8:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 28h]
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [esp + 30h]
        mov    ecx, dword ptr [esp + 38h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 3ch]
        cmp    byte ptr [eax], 85h
        jne    SHORT loc_1_6FF8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h
loc_1_6FF8:
        mov    esi, dword ptr [ebp + 0ch]
        or     esi, esi
        je     NEAR PTR loc_1_7260
        cmp    dword ptr [esi + 68h], 6
        jne    NEAR PTR loc_1_7260
        mov    edi, dword ptr [esi + 60h]
        or     edi, edi
        je     NEAR PTR loc_1_726F
        mov    dword ptr [esi + 60h], 0
        mov    eax, dword ptr [edi + 30h]
        or     eax, eax
        je     SHORT loc_1_703A
        push   0
        push   0
        push   eax
        call   sub_1_10AEC
        add    esp, 0ch
        mov    dword ptr [edi + 30h], 0
loc_1_703A:
        mov    eax, dword ptr [esp + 3ch]
        mov    dl, byte ptr [eax]
        cmp    dl, 82h
        jne    SHORT loc_1_70B0
        mov    dword ptr [esi + 44h], 0
        mov    dword ptr [esi + 68h], 7
        mov    dword ptr [ebp + 38h], 7
        mov    eax, dword ptr [edi + 44h]
        or     eax, eax
        jne    SHORT loc_1_7070
        push   5
        push   edi
        call   sub_1_D5C0
        add    esp, 8
        jmp    SHORT loc_1_7074
        db  08Bh, 0FFh
loc_1_7070:
        dec    eax
        mov    dword ptr [edi + 44h], eax
loc_1_7074:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 60h], 0
        lea    eax, [esp + 10h]
        push   eax
        push   esi
        call   sub_1_727C
        add    esp, 8
        cmp    dword ptr [esp + 10h], 0
        je     NEAR PTR loc_1_726F
        push   esi
        push   0
        push   dword ptr [esp + 18h]
        call   sub_1_25A0
        add    esp, 0ch
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_70B0:
        mov    eax, dword ptr [esi + 68h]
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [esp + 3ch]
        movzx  eax, byte ptr [eax + 4]
        mov    dword ptr [esp + 14h], eax
        cmp    dl, 84h
        jne    NEAR PTR loc_1_71B4
        mov    al, byte ptr [esi + 6eh]
        lea    ecx, [eax - 1]
        mov    byte ptr [esi + 6eh], cl
        or     al, al
        je     NEAR PTR loc_1_71B4
        sub    ebx, ebx
        mov    dword ptr [esi + 68h], 1
        cmp    dl, 84h
        jne    SHORT loc_1_7140
        mov    eax, dword ptr [esp + 3ch]
        mov    ax, word ptr [eax + 8]
        mov    cx, ax
        and    ecx, 0ffh
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0ffffh
        or     ecx, eax
        mov    dword ptr [edi + 34h], ecx
        mov    eax, dword ptr [esp + 3ch]
        mov    ecx, dword ptr [eax + 4]
        mov    ebx, ecx
        and    ebx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     ebx, eax
        shl    ebx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    eax, 8
        or     ebx, eax
        shr    ecx, 18h
        and    ecx, 0ffh
        or     ebx, ecx
        jmp    SHORT loc_1_714C
        db  090h
loc_1_7140:
        cmp    dword ptr [esp + 14h], 82h
        jne    SHORT loc_1_714C
        sub    ebx, ebx
loc_1_714C:
        mov    dword ptr [ebp + 0ch], 0
        mov    dword ptr [esi + 0ch], 0
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_1_716A
        push   esi
        call   sub_1_BCD4
        add    esp, 4
loc_1_716A:
        mov    eax, dword ptr [esi + 54h]
        mov    dword ptr [esi + 68h], 2
        mov    byte ptr [esi + 6eh], 0
        mov    dword ptr [esp + 10h], eax
        push   esi
        call   sub_1_DEE4
        add    esp, 4
        push   0
        push   0
        push   0
        push   ebp
        call   sub_1_72A4
        add    esp, 10h
        push   dword ptr [esp + 1ch]
        db  068h, 078h, 0B1h, 000h, 000h
        push   0
        push   ebx
        push   edi
        call   sub_1_3C00
        add    esp, 14h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h
loc_1_71B4:
        mov    eax, dword ptr [edi + 38h]
        cmp    dword ptr [eax + 24h], 0facf0000h
        jne    SHORT loc_1_71D5
        cmp    dword ptr [eax + 1ch], 1
        jne    SHORT loc_1_71D5
        test   byte ptr [eax + 20h], 10h
        je     SHORT loc_1_71D5
        push   eax
        call   sub_1_DEA4
        add    esp, 4
loc_1_71D5:
        mov    dword ptr [esi + 68h], 1
        mov    dword ptr [esi + 0ch], 0
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_1_71F3
        push   esi
        call   sub_1_BCD4
        add    esp, 4
loc_1_71F3:
        mov    eax, dword ptr [edi + 44h]
        or     eax, eax
        jne    SHORT loc_1_7208
        push   5
        push   edi
        call   sub_1_D5C0
        add    esp, 8
        jmp    SHORT loc_1_720C
        db  090h
loc_1_7208:
        dec    eax
        mov    dword ptr [edi + 44h], eax
loc_1_720C:
        lea    eax, [esp + 10h]
        push   eax
        push   esi
        call   sub_1_727C
        add    esp, 8
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_7244
        mov    eax, 0c00000bch
        cmp    dword ptr [esp + 14h], 82h
        jne    SHORT loc_1_7235
        mov    eax, 0c00000beh
loc_1_7235:
        push   0
        push   eax
        push   dword ptr [esp + 18h]
        call   sub_1_25A0
        add    esp, 0ch
loc_1_7244:
        push   0
        push   0
        push   0
        push   ebp
        call   sub_1_72A4
        add    esp, 10h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
loc_1_7260:
        push   0
        push   0
        push   0
        push   ebp
        call   sub_1_72A4
        add    esp, 10h
loc_1_726F:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
sub_1_727C:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax + 54h]
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        mov    dword ptr [eax + 54h], 0
        ret
        db  08Dh, 049h, 000h, 08Bh, 044h, 024h, 014h, 08Bh, 04Ch, 024h, 018h, 089h, 001h, 02Bh, 0C0h, 0C3h
        db  08Dh, 049h, 000h
sub_1_72A4:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    bl, bl
        cmp    byte ptr [esp + 20h], bl
        je     SHORT loc_1_72CC
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 1ch]
        mov    edi, dword ptr [esp + 1ch]
        push   edi
        call   sub_1_11788
        add    esp, 0ch
        jmp    SHORT loc_1_72D0
        db  08Dh, 049h, 000h
loc_1_72CC:
        mov    edi, dword ptr [esp + 14h]
loc_1_72D0:
        mov    eax, dword ptr [edi + 38h]
        cmp    eax, 8
        jge    SHORT loc_1_7308
        cmp    eax, 3
        jle    SHORT loc_1_7308
        mov    dword ptr [edi + 38h], 8
        mov    esi, dword ptr [edi + 0ch]
        or     esi, esi
        je     SHORT loc_1_72FB
        mov    bl, 1
        mov    dword ptr [edi + 0ch], 0
        mov    dword ptr [esi + 0ch], 0
loc_1_72FB:
        push   edi
        call   sub_1_118A8
        add    esp, 4
        jmp    SHORT loc_1_730C
        db  08Bh, 0FFh
loc_1_7308:
        mov    esi, dword ptr [esp + 0ch]
loc_1_730C:
        or     bl, bl
        je     SHORT loc_1_731A
        push   edi
        push   esi
        call   sub_1_D61C
        add    esp, 8
loc_1_731A:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
sub_1_7324:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 34h]
        cmp    byte ptr [esi], 81h
        je     SHORT loc_1_7344
        mov    eax, 8fh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  08Dh, 049h, 000h
loc_1_7344:
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        lea    eax, [esp + 28h]
        push   eax
        mov    ebx, dword ptr [esp + 48h]
        lea    eax, [ebx - 4]
        push   eax
        lea    eax, [esi + 4]
        push   eax
        call   sub_1_FFAC
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_1_7380
        cmp    eax, 0ffh
        je     SHORT loc_1_7380
        mov    eax, 8fh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  090h
loc_1_7380:
        mov    edi, dword ptr [esp + 38h]
        cmp    dword ptr [edi + 38h], 4
        je     SHORT loc_1_7398
        mov    eax, 0c0000001h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  090h
loc_1_7398:
        lea    eax, [esp + 12h]
        push   eax
        push   dword ptr [esp + 1ch]
        lea    eax, [esp + 28h]
        push   eax
        push   0
        call   sub_1_DFA0
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_73C4
        mov    eax, 82h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  08Dh, 049h, 000h
loc_1_73C4:
        mov    eax, dword ptr [eax + 8]
        lea    ecx, [eax + 0ch]
        mov    edx, dword ptr [ecx]
        mov    ebp, edx
        sub    ebp, eax
        cmp    ebp, 0ch
        jne    SHORT loc_1_73E4
        mov    eax, 80h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  08Bh, 0FFh
loc_1_73E4:
        cmp    edx, ecx
        mov    byte ptr [esp + 11h], 0
        je     SHORT loc_1_741C
        mov    eax, dword ptr [edi + 18h]
loc_1_73F0:
        mov    edi, edx
        cmp    dword ptr [edx + 80h], eax
        jne    SHORT loc_1_740A
        mov    ebp, dword ptr [edx + 2ch]
        sub    ebp, edx
        cmp    ebp, 2ch
        jne    SHORT loc_1_7414
        cmp    dword ptr [edx + 4ch], 0
        jne    SHORT loc_1_7414
loc_1_740A:
        mov    edx, dword ptr [edx]
        cmp    edx, ecx
        jne    SHORT loc_1_73F0
        jmp    SHORT loc_1_7420
        db  08Bh, 0FFh
loc_1_7414:
        mov    byte ptr [esp + 11h], 1
        jmp    SHORT loc_1_7420
        db  090h
loc_1_741C:
        mov    edi, dword ptr [esp + 1ch]
loc_1_7420:
        cmp    byte ptr [esp + 11h], 0
        jne    SHORT loc_1_7434
        mov    eax, 80h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
loc_1_7434:
        inc    dword ptr [edi + 84h]
        mov    eax, dword ptr [esp + 14h]
        lea    eax, [eax + esi + 5]
        push   dword ptr [esp + 48h]
        push   dword ptr [esp + 48h]
        sub    ebx, dword ptr [esp + 1ch]
        push   ebx
        push   eax
        call   sub_1_74A4
        add    esp, 10h
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_7494
        cmp    esi, 0ffh
        je     SHORT loc_1_7494
        push   edi
        call   sub_1_DD58
        add    esp, 4
        cmp    esi, 1
        jne    SHORT loc_1_7484
        mov    eax, 83h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  08Dh, 049h, 000h
loc_1_7484:
        pop    ebp
        mov    eax, 8fh
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  08Dh, 049h, 000h
loc_1_7494:
        mov    eax, dword ptr [esp + 40h]
        mov    dword ptr [eax], edi
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
sub_1_74A4:
        sub    esp, 18h
        push   esi
        push   edi
        lea    eax, [esp + 8]
        push   eax
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 34h]
        call   sub_1_FFAC
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_1_74D3
        cmp    eax, 0ffh
        jne    SHORT loc_1_7521
loc_1_74D3:
        push   1ah
        call   sub_1_001E
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_74EC
        mov    eax, 1
        pop    edi
        pop    esi
        add    esp, 18h
        ret
loc_1_74EC:
        mov    dword ptr [eax], 1
        mov    word ptr [eax + 4], 12h
        mov    word ptr [eax + 6], 11h
        mov    word ptr [eax + 8], 0
        lea    esi, [esp + 10h]
        lea    edi, [eax + 0ah]
        movsd
        movsd
        movsd
        movsd
        mov    ecx, dword ptr [esp + 2ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 30h]
        mov    dword ptr [eax], 1ah
        sub    eax, eax
loc_1_7521:
        pop    edi
        pop    esi
        add    esp, 18h
        ret
        db  090h
sub_1_7528:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [edi + 1ch]
        mov    eax, edx
        sub    eax, edi
        cmp    eax, 1ch
        jne    SHORT loc_1_7544
        mov    eax, 17h
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_7544:
        mov    esi, dword ptr [esp + 14h]
        cmp    word ptr [esi + 6], 2
        je     SHORT loc_1_7558
        mov    eax, 17h
        pop    edi
        pop    esi
        ret
        db  090h
loc_1_7558:
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        add    edi, 14h
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [edx], edi
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [eax], edx
        mov    dword ptr [edi + 4], edx
        mov    dword ptr [edx + 38h], 4
        inc    dword ptr [edx + 3ch]
        mov    word ptr [edx + 32h], 0
        mov    byte ptr [edx + 45h], 0
        mov    eax, dword ptr [esi + 0ah]
        mov    dword ptr [edx + 34h], eax
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [eax], edx
        sub    eax, eax
        pop    edi
        pop    esi
        ret
        db  090h
sub_1_7598:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    byte ptr [esp + 11h], 0
        sub    ebp, ebp
        mov    dword ptr [esp + 14h], ebp
        mov    dword ptr [esp + 18h], ebp
        mov    byte ptr [esp + 13h], 0
        mov    byte ptr [esp + 12h], 0
        mov    ebx, dword ptr [esp + 30h]
        mov    esi, dword ptr [ebx + 0ch]
        cmp    esi, ebp
        je     NEAR PTR loc_1_76F8
        mov    eax, dword ptr [esi + 8]
        cmp    eax, 87654321h
        je     SHORT loc_1_75E8
        cmp    eax, 87654320h
        je     SHORT loc_1_75E8
        mov    eax, 0c0000001h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  08Dh, 049h, 000h
loc_1_75E8:
        mov    di, word ptr [esi + 68h]
        mov    cx, word ptr [ebx + 38h]
        cmp    di, 1
        jbe    NEAR PTR loc_1_76CD
        cmp    di, 8
        jae    NEAR PTR loc_1_76CD
        mov    eax, dword ptr [esi + 54h]
        or     eax, eax
        je     SHORT loc_1_761A
        cmp    di, 3
        jbe    SHORT loc_1_761A
        mov    ebp, eax
        mov    dword ptr [esi + 54h], 0
loc_1_761A:
        cmp    di, 7
        je     SHORT loc_1_7626
        cmp    di, 5
        jne    SHORT loc_1_7638
loc_1_7626:
        mov    eax, dword ptr [esi + 60h]
        or     eax, eax
        je     SHORT loc_1_7638
        mov    dword ptr [esp + 18h], eax
        mov    dword ptr [esi + 60h], 0
loc_1_7638:
        mov    eax, dword ptr [esi + 58h]
        mov    dword ptr [esp + 14h], eax
        or     eax, eax
        je     SHORT loc_1_764A
        mov    dword ptr [esi + 58h], 0
loc_1_764A:
        mov    dword ptr [esi + 68h], 1
        mov    dword ptr [esi + 0ch], 0
        mov    al, byte ptr [esp + 44h]
        mov    byte ptr [esi + 6fh], al
        cmp    word ptr [ebx + 32h], 3
        jne    SHORT loc_1_7683
        cmp    byte ptr [ebx + 48h], 1
        jne    SHORT loc_1_7683
        mov    eax, dword ptr [ebx + 28h]
        mov    edx, dword ptr [ebx + 2ch]
        mov    dword ptr [edx], eax
        mov    dword ptr [eax + 4], edx
        mov    byte ptr [ebx + 48h], 0
        lea    eax, [ebx + 28h]
        mov    dword ptr [ebx + 2ch], eax
        mov    dword ptr [eax], eax
loc_1_7683:
        mov    dword ptr [ebx + 38h], 8
        mov    dword ptr [ebx + 0ch], 0
        mov    byte ptr [esp + 13h], 1
        mov    byte ptr [esp + 12h], 1
        mov    edx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [eax], edx
        mov    dword ptr [edx + 4], eax
        mov    edx, dword ptr [esi + 10h]
        add    edx, 14h
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esi], edx
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [eax], esi
        mov    dword ptr [edx + 4], esi
        cmp    dword ptr [esp + 44h], 4
        mov    dword ptr [ebx + 38h], 9
        je     SHORT loc_1_76CD
        mov    dword ptr [ebx + 38h], 0
loc_1_76CD:
        cmp    cx, 8
        jne    SHORT loc_1_76DC
        mov    dword ptr [ebx + 38h], 9
        jmp    SHORT loc_1_76EE
loc_1_76DC:
        cmp    cx, 9
        jne    SHORT loc_1_76EE
        mov    byte ptr [esp + 11h], 1
        mov    dword ptr [ebx + 38h], 0
loc_1_76EE:
        cmp    cx, 3
        ja     SHORT loc_1_7750
        jmp    SHORT loc_1_774D
        db  08Bh, 0FFh
loc_1_76F8:
        sub    di, di
        mov    ax, word ptr [ebx + 38h]
        cmp    ax, di
        je     SHORT loc_1_7720
        cmp    ax, 8
        jae    SHORT loc_1_7724
        mov    byte ptr [esp + 12h], 1
        mov    dword ptr [ebx + 38h], 9
        cmp    dword ptr [esp + 44h], 4
        je     SHORT loc_1_7742
        jmp    SHORT loc_1_773B
        db  090h
loc_1_7720:
        cmp    ax, 8
loc_1_7724:
        jne    SHORT loc_1_7730
        mov    dword ptr [ebx + 38h], 9
        jmp    SHORT loc_1_7742
        db  090h
loc_1_7730:
        cmp    ax, 9
        jne    SHORT loc_1_7742
        mov    byte ptr [esp + 11h], 1
loc_1_773B:
        mov    dword ptr [ebx + 38h], 0
loc_1_7742:
        cmp    ax, 3
        ja     SHORT loc_1_7750
        or     ax, ax
        je     SHORT loc_1_7750
loc_1_774D:
        inc    dword ptr [ebx + 3ch]
loc_1_7750:
        movzx  eax, word ptr [ebx + 32h]
        mov    dword ptr [esp + 1ch], eax
        cmp    byte ptr [esp + 12h], 0
        je     SHORT loc_1_7779
        cmp    dword ptr [esp + 44h], 4
        jne    SHORT loc_1_7774
        push   ebx
        call   sub_1_118A8
        add    esp, 4
        jmp    SHORT loc_1_7779
        db  08Dh, 049h, 000h
loc_1_7774:
        mov    byte ptr [esp + 11h], 1
loc_1_7779:
        movzx  eax, di
        cmp    eax, 9
        ja     NEAR PTR loc_1_78B3
        db  0FFh, 024h, 085h, 008h, 079h, 000h, 000h
loc_1_778C:
        or     ebp, ebp
        je     NEAR PTR loc_1_78B3
        cmp    byte ptr [ebx + 45h], 0
        push   0
        je     SHORT loc_1_77A8
        push   0c00000beh
        jmp    NEAR PTR loc_1_78AA
        db  08Bh, 0FFh
loc_1_77A8:
        push   0c0000001h
        jmp    NEAR PTR loc_1_78AA
        db  08Bh, 0FFh
loc_1_77B4:
        or     ebp, ebp
        je     SHORT loc_1_77C8
        push   0
        push   0c0000120h
        push   ebp
        call   sub_1_25A0
        add    esp, 0ch
loc_1_77C8:
        cmp    dword ptr [esp + 1ch], 2
        jne    SHORT loc_1_77E9
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_1_77E9
        push   0
        push   0c0000120h
        push   dword ptr [esp + 20h]
        call   sub_1_25A0
        add    esp, 0ch
loc_1_77E9:
        mov    eax, dword ptr [esi + 10h]
        mov    ecx, dword ptr [eax + 58h]
        or     ecx, ecx
        je     SHORT loc_1_781C
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_1_7823
        push   2
        push   dword ptr [esp + 44h]
        push   dword ptr [esp + 44h]
        push   dword ptr [esp + 44h]
        push   dword ptr [esp + 44h]
        push   dword ptr [esi + 24h]
        push   dword ptr [eax + 5ch]
        call   ecx
        add    esp, 1ch
        jmp    SHORT loc_1_7843
        db  08Dh, 049h, 000h
loc_1_781C:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_1_7843
loc_1_7823:
        mov    eax, 16h
        cmp    dword ptr [esp + 44h], 4
        je     SHORT loc_1_7834
        mov    eax, 14h
loc_1_7834:
        push   0
        push   eax
        push   dword ptr [esp + 1ch]
        call   sub_1_25A0
        add    esp, 0ch
loc_1_7843:
        lea    eax, [esp + 20h]
        push   eax
        push   esi
        call   sub_1_AF30
        add    esp, 8
        jmp    SHORT loc_1_78B3
        db  090h
loc_1_7854:
        mov    edi, dword ptr [esi + 60h]
        or     edi, edi
        je     SHORT loc_1_788B
        mov    dword ptr [esi + 60h], 0
        mov    eax, dword ptr [edi + 30h]
        mov    dword ptr [edi + 30h], 0
        or     eax, eax
        je     SHORT loc_1_7880
        push   0
        lea    ecx, [esp + 28h]
        push   ecx
        push   eax
        call   sub_1_10AEC
        add    esp, 0ch
loc_1_7880:
        push   5
        push   edi
        call   sub_1_D5C0
        add    esp, 8
loc_1_788B:
        or     ebp, ebp
        je     SHORT loc_1_78B3
        push   0
        push   0c00000bch
        jmp    SHORT loc_1_78AA
loc_1_7898:
        mov    dword ptr [esi + 68h], 9
loc_1_789F:
        or     ebp, ebp
        je     SHORT loc_1_78B3
        push   0
        push   0c0000120h
loc_1_78AA:
        push   ebp
        call   sub_1_25A0
        add    esp, 0ch
loc_1_78B3:
        cmp    byte ptr [esp + 13h], 0
        je     SHORT loc_1_78C4
        push   ebx
        push   esi
        call   sub_1_D61C
        add    esp, 8
loc_1_78C4:
        cmp    byte ptr [esp + 11h], 0
        je     SHORT loc_1_78FB
        mov    eax, dword ptr [ebx + 40h]
        or     eax, eax
        je     SHORT loc_1_78E6
        mov    dword ptr [ebx + 40h], 0
        push   0
        push   0
        push   eax
        call   sub_1_25A0
        add    esp, 0ch
loc_1_78E6:
        push   dword ptr [ebx + 18h]
        db  068h, 030h, 079h, 000h, 000h
        push   0
        push   ebx
        push   0
        call   sub_1_3C00
        add    esp, 14h
loc_1_78FB:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  08Dh, 049h, 000h
loc_1_7908:
        lahf
        js     SHORT loc_1_790B
loc_1_790B:
        db  000h, 09Fh, 078h, 000h, 000h, 0B3h
        js     SHORT loc_1_7913
loc_1_7913:
        db  000h, 08Ch, 077h, 000h, 000h, 08Ch, 077h
        add    byte ptr [eax], al
        mov    ah, 77h
        add    byte ptr [eax], al
        push   esp
        js     SHORT loc_1_7923
loc_1_7923:
        db  000h, 0B4h, 077h, 000h, 000h, 098h, 078h
        add    byte ptr [eax], al
        lahf
        db  078h, 000h
        db  000h
loc_1_7930:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esi + 0ch]
        cmp    byte ptr [edi + 45h], 0
        jne    SHORT loc_1_7968
        mov    ebx, dword ptr [edi + 18h]
        cmp    byte ptr [edi + 47h], 0
        push   edi
        je     SHORT loc_1_794C
        jmp    SHORT loc_1_7972
loc_1_794C:
        call   sub_1_DF20
        add    esp, 4
        push   edi
        call   sub_1_DF4C
        add    esp, 4
        push   ebx
        call   sub_1_7988
        jmp    SHORT loc_1_7977
        db  08Dh, 049h, 000h
loc_1_7968:
        push   edi
        call   sub_1_DF20
        add    esp, 4
        push   edi
loc_1_7972:
        call   sub_1_DF4C
loc_1_7977:
        add    esp, 4
        push   esi
        call   sub_1_0024
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
sub_1_7988:
        push   esi
        push   49h
        call   sub_1_001E
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_79BC
        push   dword ptr [esp + 8]
        push   esi
        call   sub_1_A908
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_79BC
        cmp    eax, 0ffh
        je     SHORT loc_1_79BC
        push   esi
        call   sub_1_0024
        add    esp, 4
        sub    esi, esi
loc_1_79BC:
        or     esi, esi
        jne    SHORT loc_1_79D9
        push   dword ptr [esp + 8]
        db  068h, 0DCh, 079h, 000h, 000h
        push   0
        push   dword ptr [esp + 14h]
        push   0
        call   sub_1_3C00
        add    esp, 14h
loc_1_79D9:
        pop    esi
        ret
        db  090h
loc_1_79DC:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   dword ptr [esi + 0ch]
        call   sub_1_7988
        add    esp, 4
        push   esi
        call   sub_1_0024
        add    esp, 4
        pop    esi
        ret
        db  090h
sub_1_79F8:
        sub    esp, 1ch
        push   esi
        lea    eax, [esp + 0ch]
        push   eax
        lea    eax, [esp + 0ch]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        mov    eax, dword ptr [esp + 34h]
        sub    eax, 0eh
        push   eax
        mov    esi, dword ptr [esp + 34h]
        lea    eax, [esi + 0eh]
        push   eax
        call   sub_1_FFAC
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_1_7A32
        cmp    eax, 0ffh
        jne    NEAR PTR loc_1_7AF6
loc_1_7A32:
        mov    ecx, dword ptr [esi + 4]
        mov    esi, ecx
        and    esi, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     esi, eax
        shl    esi, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    eax, 8
        or     esi, eax
        shr    ecx, 18h
        and    ecx, 0ffh
        or     esi, ecx
        je     NEAR PTR loc_1_7AF6
        lea    eax, [esp + 4]
        push   eax
        push   0
        push   esi
        push   dword ptr [esp + 14h]
        lea    eax, [esp + 20h]
        push   eax
        db  0FFh, 035h, 0B8h, 032h, 001h, 000h
        call   sub_1_65BC
        add    esp, 18h
        or     eax, eax
        je     SHORT loc_1_7A8E
        cmp    eax, 0ffh
        jne    SHORT loc_1_7AF6
loc_1_7A8E:
        cmp    eax, 0ffh
        mov    eax, dword ptr [esp + 4]
        jne    SHORT loc_1_7ACC
        mov    eax, dword ptr [eax + 20h]
        test   al, 10h
        je     SHORT loc_1_7AF6
        test   al, 8
        jne    SHORT loc_1_7AF6
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0ch], esi
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 30h], 1
        mov    eax, dword ptr [esp + 2ch]
        mov    eax, dword ptr [eax + 88h]
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx + 28h], eax
        pop    esi
        add    esp, 1ch
        ret
        db  090h
loc_1_7ACC:
        mov    word ptr [eax + 30h], 1
        mov    eax, dword ptr [esp + 4]
        and    dword ptr [eax + 20h], 0ffffff0fh
        mov    eax, dword ptr [esp + 4]
        or     dword ptr [eax + 20h], 10h
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 2ch]
        mov    eax, dword ptr [eax + 88h]
        or     dword ptr [ecx + 28h], eax
loc_1_7AF6:
        pop    esi
        add    esp, 1ch
        ret
        db  090h
sub_1_7AFC:
        sub    esp, 40h
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [esp + 6ch], 0bh
        jae    SHORT loc_1_7B18
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 40h
        ret
        db  090h
loc_1_7B18:
        mov    esi, dword ptr [esp + 78h]
        mov    dword ptr [esp + 14h], 17h
        mov    al, byte ptr [esi]
        cmp    al, 10h
        jb     NEAR PTR loc_1_800A
        cmp    al, 12h
        ja     NEAR PTR loc_1_800A
        cmp    dword ptr [esp + 6ch], 52h
        jae    SHORT loc_1_7B4C
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 40h
        ret
        db  08Bh, 0FFh
loc_1_7B4C:
        sub    eax, eax
        lea    edi, [esi + 0fh]
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        lea    ebx, [ecx - 1]
        or     ebx, ebx
        jg     SHORT loc_1_7B70
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 40h
        ret
        db  08Bh, 0FFh
loc_1_7B70:
        lea    ecx, [ebx + 0fh]
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 28h]
        push   eax
        lea    eax, [esp + 48h]
        push   eax
        mov    eax, dword ptr [esp + 78h]
        sub    eax, ecx
        push   eax
        lea    eax, [ebx + esi]
        add    eax, 10h
        push   eax
        call   sub_1_FFAC
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_1_7BB0
        cmp    eax, 0ffh
        je     SHORT loc_1_7BB0
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 40h
        ret
loc_1_7BB0:
        db  00Fh, 0B7h, 005h, 0D4h, 032h, 001h, 000h
        add    eax, dword ptr [esp + 20h]
        add    eax, 30h
        cmp    eax, dword ptr [esp + 6ch]
        jbe    SHORT loc_1_7BD4
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 40h
        ret
        db  08Dh, 049h, 000h
loc_1_7BD4:
        cmp    byte ptr [esp + 40h], 2ah
        jne    SHORT loc_1_7BE7
        lea    edi, [esp + 41h]
        sub    eax, eax
        stosd
        stosd
        stosd
        stosw
        stosb
loc_1_7BE7:
        lea    eax, [esp + 12h]
        push   eax
        push   dword ptr [esp + 28h]
        lea    eax, [esp + 48h]
        push   eax
        push   0
        call   sub_1_DFA0
        add    esp, 10h
        mov    ecx, dword ptr [esp + 20h]
        lea    edi, [ecx + ebx + 11h]
        mov    ecx, dword ptr [esp + 70h]
        mov    dword ptr [esp + 2ch], ecx
        sub    dword ptr [esp + 70h], edi
        mov    ecx, dword ptr [esp + 54h]
        mov    edx, dword ptr [ecx + 88h]
        or     eax, eax
        je     NEAR PTR loc_1_7EC4
        mov    ebp, dword ptr [eax + 8]
        or     ebp, ebp
        je     NEAR PTR loc_1_7EC4
        test   dword ptr [eax + 28h], edx
        je     NEAR PTR loc_1_7EC4
        mov    ecx, dword ptr [ebp + 18h]
        inc    ecx
        mov    dword ptr [ebp + 18h], ecx
        mov    eax, dword ptr [ebp + 0ch]
        mov    ebx, eax
        sub    ebx, ebp
        cmp    ebx, 0ch
        je     NEAR PTR loc_1_7EC0
        mov    ecx, dword ptr [esp + 74h]
        mov    dword ptr [ecx], edi
        mov    ecx, dword ptr [esp + 7ch]
        mov    dword ptr [ecx], 0
        mov    byte ptr [esp + 10h], 0
        mov    ecx, dword ptr [esp + 80h]
        mov    dword ptr [ecx], 0
        lea    ecx, [ebp + 0ch]
        cmp    eax, ecx
        je     NEAR PTR loc_1_7DAC
loc_1_7C7D:
        mov    ebx, eax
        mov    ecx, dword ptr [eax + 80h]
        test   dword ptr [ecx + 88h], edx
        jne    SHORT loc_1_7C9C
        mov    eax, dword ptr [eax]
        lea    ecx, [ebp + 0ch]
        cmp    eax, ecx
        jne    SHORT loc_1_7C7D
        jmp    NEAR PTR loc_1_7DB0
        db  090h
loc_1_7C9C:
        cmp    byte ptr [esp + 40h], 2ah
        je     SHORT loc_1_7CDA
        mov    ecx, dword ptr [esp + 54h]
        mov    edx, dword ptr [ecx + 34h]
        mov    ecx, edx
        sub    ecx, dword ptr [esp + 54h]
        mov    dword ptr [esp + 1ch], ecx
        cmp    ecx, 34h
        je     SHORT loc_1_7CDA
        mov    edi, dword ptr [edx]
        mov    ecx, dword ptr [edx + 4]
        mov    dword ptr [ecx], edi
        mov    dword ptr [edi + 4], ecx
        lea    edi, [ebx + 24h]
        mov    dword ptr [edx], edi
loc_1_7CC8:
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [edx + 4], ecx
        mov    dword ptr [ecx], edx
        mov    dword ptr [edi + 4], edx
        cmp    dword ptr [esp + 1ch], 34h
        jne    SHORT loc_1_7CC8
loc_1_7CDA:
        mov    ecx, dword ptr [ebx + 24h]
        sub    ecx, ebx
        cmp    ecx, 24h
        je     NEAR PTR loc_1_7DA0
        sub    edx, edx
        mov    ebx, eax
        lea    ecx, [ebp + 0ch]
        cmp    eax, ecx
        je     SHORT loc_1_7D22
        nop
loc_1_7CF4:
        mov    ecx, dword ptr [eax + 24h]
        mov    edi, ecx
        sub    edi, eax
        cmp    edi, 24h
        je     SHORT loc_1_7D17
        mov    dword ptr [esp + 18h], ecx
        mov    ecx, dword ptr [ecx + 10h]
        cmp    dword ptr [esp + 70h], ecx
        jbe    SHORT loc_1_7D20
        cmp    edx, ecx
        jae    SHORT loc_1_7D15
        mov    ebx, eax
        mov    edx, ecx
loc_1_7D15:
        mov    eax, dword ptr [eax]
loc_1_7D17:
        lea    ecx, [ebp + 0ch]
        cmp    eax, ecx
        jne    SHORT loc_1_7CF4
        jmp    SHORT loc_1_7D22
loc_1_7D20:
        mov    ebx, eax
loc_1_7D22:
        mov    eax, dword ptr [ebx + 24h]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 18h]
        mov    edx, dword ptr [eax + 8]
        mov    ecx, dword ptr [esp + 7ch]
        mov    dword ptr [ecx], edx
        lea    ecx, [esi + 0eh]
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 28h]
        push   eax
        add    edx, 0ah
        push   edx
        movzx  eax, byte ptr [ecx]
        inc    eax
        push   eax
        push   ecx
        call   sub_1_FFAC
        add    esp, 14h
        push   dword ptr [esp + 18h]
        call   sub_1_0024
        add    esp, 4
        cmp    byte ptr [ebp + 23h], 0
        je     SHORT loc_1_7D8C
        db  080h, 03Dh, 050h, 033h, 001h, 000h, 000h
        jne    SHORT loc_1_7D8C
        mov    byte ptr [esp + 10h], 1
        mov    dword ptr [esp + 14h], 0
        mov    byte ptr [esp + 11h], 1
        jmp    SHORT loc_1_7DB0
        db  08Bh, 0FFh
loc_1_7D8C:
        push   ebp
        call   sub_1_DDE4
        add    esp, 4
        mov    dword ptr [esp + 14h], 0
        jmp    SHORT loc_1_7DB0
        db  090h
loc_1_7DA0:
        push   ebp
        call   sub_1_DDE4
        add    esp, 4
        jmp    SHORT loc_1_7DB0
        db  090h
loc_1_7DAC:
        mov    ebx, dword ptr [esp + 38h]
loc_1_7DB0:
        cmp    byte ptr [esi], 12h
        je     SHORT loc_1_7E2A
        mov    eax, dword ptr [esp + 5ch]
        mov    ecx, dword ptr [eax + 0ah]
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
        and    ecx, 0ffh
        or     edx, ecx
        mov    ecx, dword ptr [esp + 54h]
        mov    edi, dword ptr [ecx + 5ch]
        mov    eax, dword ptr [ecx + 58h]
        and    eax, edi
        and    edi, edx
        cmp    eax, edi
        jne    SHORT loc_1_7E19
        cmp    byte ptr [esp + 4fh], 1eh
        je     SHORT loc_1_7E2A
        cmp    byte ptr [esp + 4fh], 1dh
        je     SHORT loc_1_7E2A
        cmp    byte ptr [esp + 4fh], 1
        je     SHORT loc_1_7E2A
        push   edx
        call   sub_1_A064
        add    esp, 4
        or     al, al
        jne    SHORT loc_1_7E2A
loc_1_7E19:
        push   dword ptr [esp + 54h]
        push   dword ptr [esp + 70h]
        push   esi
        call   sub_1_79F8
        add    esp, 0ch
loc_1_7E2A:
        cmp    byte ptr [esp + 10h], 0
        je     NEAR PTR loc_1_7ECC
        push   16h
        call   sub_1_001E
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_7EC4
        mov    byte ptr [edi + 15h], 0
        mov    ecx, dword ptr [esp + 80h]
        mov    dword ptr [ecx], edi
        mov    dword ptr [edi], ebp
        mov    dword ptr [edi + 10h], ebx
        mov    al, byte ptr [esp + 11h]
        mov    byte ptr [edi + 14h], al
        mov    eax, dword ptr [esp + 68h]
        mov    dword ptr [edi + 4], eax
        lea    eax, [esp + 30h]
        push   eax
        lea    eax, [esp + 38h]
        push   eax
        mov    eax, dword ptr [esp + 74h]
        sub    eax, 0eh
        push   eax
        lea    eax, [esi + 0eh]
        push   eax
        call   sub_1_74A4
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_7EA8
        cmp    eax, 0ffh
        je     SHORT loc_1_7EA8
        mov    ecx, dword ptr [esp + 80h]
        mov    dword ptr [ecx], 0
        push   edi
        call   sub_1_0024
        add    esp, 4
        jmp    SHORT loc_1_7EC4
        db  08Bh, 0FFh
loc_1_7EA8:
        mov    eax, dword ptr [esp + 34h]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esp + 30h]
        mov    dword ptr [edi + 0ch], eax
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 40h
        ret
loc_1_7EC0:
        dec    ecx
        mov    dword ptr [ebp + 18h], ecx
loc_1_7EC4:
        mov    dword ptr [esp + 14h], 17h
loc_1_7ECC:
        db  0F6h, 005h, 064h, 033h, 001h, 000h, 010h
        je     NEAR PTR loc_1_800A
        mov    eax, dword ptr [esp + 5ch]
        add    eax, 0ah
        mov    dword ptr [esp + 28h], eax
        mov    edx, dword ptr [eax]
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
        and    edx, 0ffh
        or     ecx, edx
        test   byte ptr [esi + 1], 0fch
        jne    NEAR PTR loc_1_800A
        cmp    byte ptr [esp + 40h], 2ah
        je     NEAR PTR loc_1_800A
        push   ecx
        call   sub_1_A064
        add    esp, 4
        or     al, al
        jne    NEAR PTR loc_1_800A
        lea    eax, [esp + 12h]
        push   eax
        push   dword ptr [esp + 28h]
        lea    eax, [esp + 48h]
        push   eax
        push   1
        call   sub_1_DFA0
        add    esp, 10h
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_7FC4
        test   byte ptr [edi + 20h], 8
        je     NEAR PTR loc_1_800A
        mov    eax, dword ptr [esp + 28h]
        mov    eax, dword ptr [eax]
        cmp    dword ptr [esi + 4], eax
        jne    NEAR PTR loc_1_800A
        mov    eax, dword ptr [esp + 6ch]
        cmp    eax, dword ptr [esp + 2ch]
        je     SHORT loc_1_7FAC
        push   16h
        call   sub_1_001E
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_7FA0
        mov    ecx, dword ptr [esp + 80h]
        mov    dword ptr [ecx], eax
        mov    byte ptr [eax + 15h], 1
        mov    dword ptr [eax], edi
        mov    ecx, dword ptr [esp + 54h]
        mov    dword ptr [eax + 8], ecx
        inc    dword ptr [edi + 1ch]
        jmp    SHORT loc_1_8002
        db  090h
loc_1_7FA0:
        mov    dword ptr [esp + 14h], 0c0000001h
        jmp    SHORT loc_1_800A
        db  08Bh, 0FFh
loc_1_7FAC:
        inc    dword ptr [edi + 1ch]
        push   dword ptr [esp + 54h]
        push   edi
        push   dword ptr [esp + 74h]
        push   esi
        call   sub_1_8018
        add    esp, 10h
        jmp    SHORT loc_1_8002
        db  090h
loc_1_7FC4:
        lea    eax, [esp + 3ch]
        push   eax
        push   1
        push   0
        lea    eax, [esp + 4ch]
        push   eax
        call   sub_1_A0EC
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_8002
        cmp    eax, 0ffh
        je     SHORT loc_1_8002
        push   dword ptr [esp + 24h]
        lea    eax, [esp + 44h]
        push   eax
        push   dword ptr [esp + 28h]
        push   0
        push   dword ptr [esp + 64h]
        push   0
        call   sub_1_106A4
        add    esp, 18h
loc_1_8002:
        mov    dword ptr [esp + 14h], 17h
loc_1_800A:
        mov    eax, dword ptr [esp + 14h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 40h
        ret
        db  08Bh, 0FFh
sub_1_8018:
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_FE74
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_8040
        push   dword ptr [esp + 1ch]
        call   sub_1_DEA4
        add    esp, 4
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_8040:
        mov    ebp, dword ptr [esp + 18h]
        push   ebp
        call   sub_1_001E
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_8070
        push   dword ptr [esp + 1ch]
        call   sub_1_DEA4
        add    esp, 4
        push   ebx
        call   sub_1_0024
        add    esp, 4
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_8070:
        mov    edi, eax
        mov    esi, dword ptr [esp + 14h]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        mov    dword ptr [ebx + 1ch], eax
        mov    dword ptr [ebx + 18h], ebp
        mov    dword ptr [ebx + 24h], 0
        mov    dword ptr [ebx + 20h], 0
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [ebx + 38h], eax
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [ebx + 2ch], eax
        mov    dword ptr [ebx + 14h], 0
        mov    dword ptr [ebx + 44h], 0
        mov    dword ptr [ebx + 10h], 0
        mov    dword ptr [ebx + 14h], 0
        push   dword ptr [esp + 1ch]
        push   ebx
        call   sub_1_CC18
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
sub_1_80D8:
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 1ch]
        cmp    edx, 4
        jae    SHORT loc_1_80F0
        mov    eax, 17h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_80F0:
        mov    ecx, dword ptr [esp + 18h]
        mov    di, word ptr [ecx + 2]
        db  066h, 08Bh, 01Dh, 064h, 033h, 001h, 000h
        mov    si, bx
        and    si, 2
        je     SHORT loc_1_8120
        test   bl, 10h
        jne    SHORT loc_1_8120
        test   di, 1000h
        je     SHORT loc_1_8120
        mov    eax, 17h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_8120:
        mov    ax, di
        and    eax, 78h
        je     SHORT loc_1_8144
        cmp    eax, 28h
        je     SHORT loc_1_8160
        cmp    eax, 30h
        je     SHORT loc_1_81A4
        cmp    eax, 38h
        je     NEAR PTR loc_1_81C0
        mov    eax, 17h
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_8144:
        push   edi
        push   edx
        push   ecx
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 20h]
        call   sub_1_81E4
        add    esp, 14h
        mov    eax, 17h
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_8160:
        test   di, 80h
        je     SHORT loc_1_8184
        push   edi
        push   edx
        push   ecx
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 20h]
        call   sub_1_880C
        add    esp, 14h
        mov    eax, 17h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_8184:
        or     si, si
        jne    SHORT loc_1_81D7
        push   edx
        push   ecx
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 1ch]
        call   sub_1_8A74
        add    esp, 10h
        mov    eax, 17h
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_81A4:
        push   edx
        push   ecx
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 1ch]
        call   sub_1_8D7C
        add    esp, 10h
        mov    eax, 17h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_81C0:
        test   bl, 1
        jne    SHORT loc_1_81D7
        push   edx
        push   ecx
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 1ch]
        call   sub_1_9154
        add    esp, 10h
loc_1_81D7:
        pop    edi
        mov    eax, 17h
        pop    esi
        pop    ebx
        ret
        db  0C3h, 08Dh, 049h, 000h
sub_1_81E4:
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 48h]
        mov    ecx, dword ptr [eax + 0ah]
        mov    esi, ecx
        and    esi, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     esi, eax
        shl    esi, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    eax, 8
        or     esi, eax
        shr    ecx, 18h
        and    ecx, 0ffh
        or     esi, ecx
        mov    eax, dword ptr [esp + 48h]
        lea    ebp, [eax + 8]
        mov    ax, word ptr [ebp]
        mov    bh, al
        mov    bl, ah
        cmp    bx, 35h
        jne    SHORT loc_1_825C
        push   dword ptr [esp + 54h]
        push   dword ptr [esp + 54h]
        push   dword ptr [esp + 54h]
        push   dword ptr [esp + 54h]
        push   dword ptr [esp + 54h]
        call   sub_1_9DA0
        add    esp, 14h
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  08Dh, 049h, 000h
loc_1_825C:
        db  00Fh, 0B7h, 005h, 0D4h, 032h, 001h, 000h
        add    eax, 31h
        cmp    eax, dword ptr [esp + 50h]
        jle    SHORT loc_1_827C
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  08Dh, 049h, 000h
loc_1_827C:
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 38h]
        push   eax
        push   dword ptr [esp + 5ch]
        mov    edi, dword ptr [esp + 5ch]
        lea    eax, [edi + 0ch]
        push   eax
        call   sub_1_FFAC
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_1_82B8
        cmp    eax, 0ffh
        je     SHORT loc_1_82B8
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  090h
loc_1_82B8:
        test   byte ptr [esp + 54h], 80h
        je     NEAR PTR loc_1_85F8
        db  00Fh, 0B7h, 005h, 0D4h, 032h, 001h, 000h
        add    eax, 37h
        cmp    eax, dword ptr [esp + 50h]
        jle    SHORT loc_1_82E0
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
loc_1_82E0:
        test   byte ptr [esp + 54h], 4
        jne    SHORT loc_1_82F4
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
loc_1_82F4:
        mov    eax, dword ptr [esp + 18h]
        cmp    byte ptr [eax + edi + 0eh], 21h
        jne    SHORT loc_1_8324
        push   esi
        push   dword ptr [esp + 1ch]
        lea    eax, [esp + 38h]
        push   eax
        push   dword ptr [esp + 5ch]
        push   edi
        call   sub_1_93A4
        add    esp, 14h
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  090h
loc_1_8324:
        lea    eax, [esp + 14h]
        push   eax
        push   0
        push   edi
        lea    eax, [esp + 3ch]
        push   eax
        call   sub_1_A0EC
        add    esp, 10h
        or     eax, eax
        je     NEAR PTR loc_1_83FC
        cmp    eax, 0ffh
        je     NEAR PTR loc_1_83FC
        db  080h, 03Dh, 050h, 033h, 001h, 000h, 000h
        jne    NEAR PTR loc_1_83EC
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [esp + 20h]
        lea    eax, [esp + 38h]
        push   eax
        db  0FFh, 035h, 0B8h, 032h, 001h, 000h
        call   sub_1_6954
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_8380
        cmp    eax, 0ffh
        jne    SHORT loc_1_83EC
loc_1_8380:
        mov    eax, dword ptr [esp + 14h]
        mov    eax, dword ptr [eax + 20h]
        test   ah, 8
        jne    SHORT loc_1_83EC
        mov    ecx, dword ptr [esp + 14h]
        cmp    dword ptr [ecx + 0ch], esi
        je     SHORT loc_1_83EC
        test   al, 2
        je     SHORT loc_1_83EC
        test   al, 10h
        je     SHORT loc_1_83EC
        mov    ebx, dword ptr [esp + 44h]
        cmp    dword ptr [ebx + 74h], esi
        je     SHORT loc_1_83EC
        cmp    dword ptr [ebx + 78h], esi
        je     SHORT loc_1_83EC
        push   dword ptr [esp + 20h]
        push   5
        push   7
        push   ebx
        push   ebp
        push   dword ptr [esp + 28h]
        push   edi
        push   dword ptr [esp + 34h]
        call   sub_1_110C8
        add    esp, 20h
        mov    eax, dword ptr [esp + 14h]
        and    dword ptr [eax + 20h], 0ffffff0fh
        mov    eax, dword ptr [esp + 14h]
        or     dword ptr [eax + 20h], 20h
        mov    eax, dword ptr [esp + 14h]
        cmp    dword ptr [eax + 1ch], 1
        jne    SHORT loc_1_83EC
        push   eax
        call   sub_1_DEA4
        add    esp, 4
loc_1_83EC:
        pop    ebp
        mov    eax, 17h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  08Dh, 049h, 000h
loc_1_83FC:
        mov    eax, dword ptr [esp + 18h]
        lea    ebp, [eax + edi + 0dh]
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [eax + 14h]
        or     ecx, ecx
        je     SHORT loc_1_83EC
        mov    edx, dword ptr [ecx + 20h]
        cmp    byte ptr [eax + 32h], 0
        jne    SHORT loc_1_844C
        test   byte ptr [esp + 55h], 0fh
        je     SHORT loc_1_844C
        mov    ax, word ptr [edx + 4ch]
        db  0F6h, 005h, 064h, 033h, 001h, 000h, 00Eh
        je     SHORT loc_1_843E
        test   al, 6
        je     SHORT loc_1_843E
        mov    word ptr [ecx + 3eh], 1
        or     ax, 200h
        mov    word ptr [edx + 4ch], ax
loc_1_843E:
        pop    ebp
        mov    eax, 17h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  090h
loc_1_844C:
        mov    eax, dword ptr [edx + 2ch]
        mov    dword ptr [esp + 2ch], eax
        lea    eax, [esp + 28h]
        push   eax
        lea    eax, [esp + 28h]
        push   eax
        push   ecx
        call   sub_1_10AEC
        add    esp, 0ch
        cmp    dword ptr [esp + 24h], 0
        je     NEAR PTR loc_1_83EC
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, eax
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 14h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax + 14h], 0
        test   byte ptr [esp + 55h], 0fh
        jne    NEAR PTR loc_1_8544
        push   ebx
        push   esi
        call   sub_1_9D64
        add    esp, 8
        mov    byte ptr [esp + 13h], al
        or     al, al
        jne    SHORT loc_1_84CA
        lea    eax, [esp + 30h]
        push   eax
        call   sub_1_FA64
        add    esp, 4
loc_1_84CA:
        push   dword ptr [esp + 13h]
        push   0
        call   sub_1_FA40
        add    esp, 8
        db  0F6h, 005h, 064h, 033h, 001h, 000h, 010h
        je     SHORT loc_1_84F7
        mov    eax, dword ptr [esp + 18h]
        mov    al, byte ptr [eax + edi + 17h]
        and    al, 60h
        cmp    al, 20h
        sete   al
        mov    ecx, dword ptr [esp + 14h]
        mov    byte ptr [ecx + 33h], al
loc_1_84F7:
        push   ebx
        mov    ecx, dword ptr [esp + 18h]
        mov    eax, dword ptr [esp + 30h]
        mov    eax, dword ptr [eax + 88h]
        or     dword ptr [ecx + 28h], eax
        push   esi
        call   sub_1_9D64
        add    esp, 8
        push   eax
        mov    ebx, dword ptr [esp + 48h]
        push   ebx
        mov    eax, 0fffffff6h
        sub    eax, ebp
        add    eax, dword ptr [esp + 58h]
        add    eax, edi
        push   eax
        push   dword ptr [esp + 20h]
        add    ebp, 0ah
        push   ebp
        call   sub_1_9940
        add    esp, 14h
        mov    dword ptr [esp + 14h], 0
        sub    esi, esi
        jmp    SHORT loc_1_8596
        db  08Bh, 0FFh
loc_1_8544:
        mov    eax, dword ptr [esp + 14h]
        and    dword ptr [eax + 20h], 0ffffff0fh
        mov    eax, dword ptr [esp + 14h]
        or     dword ptr [eax + 20h], 20h
        db  0F6h, 005h, 064h, 033h, 001h, 000h, 010h
        jne    SHORT loc_1_8564
        jmp    SHORT loc_1_8581
        db  08Bh, 0FFh
loc_1_8564:
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        push   dword ptr [esp + 18h]
        call   sub_1_6608
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_858D
        cmp    eax, 0ffh
        je     SHORT loc_1_858D
loc_1_8581:
        push   dword ptr [esp + 14h]
        call   sub_1_DEA4
        add    esp, 4
loc_1_858D:
        mov    esi, 0c00000beh
        mov    ebx, dword ptr [esp + 44h]
loc_1_8596:
        mov    ecx, dword ptr [ebx + 78h]
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
        and    ecx, 0ffh
        or     edx, ecx
        mov    eax, dword ptr [esp + 48h]
        cmp    edx, dword ptr [eax + 0ah]
        jne    SHORT loc_1_85D4
        push   ebx
        call   sub_1_A08C
        add    esp, 4
loc_1_85D4:
        push   ebx
        db  068h, 000h, 032h, 000h, 000h
        push   dword ptr [esp + 2ch]
        push   esi
        push   dword ptr [esp + 38h]
        call   sub_1_3C00
        add    esp, 14h
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
loc_1_85F8:
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [esp + 20h]
        lea    eax, [esp + 38h]
        push   eax
        db  0FFh, 035h, 0B4h, 032h, 001h, 000h
        call   sub_1_6954
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_861F
        cmp    eax, 0ffh
        jne    SHORT loc_1_862E
loc_1_861F:
        mov    eax, dword ptr [esp + 14h]
        mov    eax, dword ptr [eax + 20h]
        test   al, 90h
        jne    NEAR PTR loc_1_87A4
loc_1_862E:
        test   byte ptr [esp + 55h], 10h
        jne    SHORT loc_1_866C
        mov    eax, dword ptr [esp + 18h]
        cmp    byte ptr [eax + edi + 0eh], 20h
        jne    SHORT loc_1_865C
        push   dword ptr [esp + 20h]
        push   2
        push   0
        push   dword ptr [esp + 50h]
        push   ebp
        push   0
        push   edi
        push   dword ptr [esp + 34h]
        call   sub_1_110C8
        add    esp, 20h
loc_1_865C:
        pop    ebp
        mov    eax, 17h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  08Dh, 049h, 000h
loc_1_866C:
        push   esi
        call   sub_1_A064
        add    esp, 4
        or     al, al
        je     SHORT loc_1_8688
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  08Bh, 0FFh
loc_1_8688:
        mov    eax, dword ptr [esp + 18h]
        lea    eax, [eax + edi + 0dh]
        db  0F6h, 005h, 064h, 033h, 001h, 000h, 010h
        je     NEAR PTR loc_1_8794
        cmp    byte ptr [eax + 1], 20h
        jne    NEAR PTR loc_1_8794
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [esp + 20h]
        lea    eax, [esp + 38h]
        push   eax
        db  0FFh, 035h, 0B8h, 032h, 001h, 000h
        call   sub_1_6954
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_8728
        cmp    eax, 0ffh
        je     SHORT loc_1_8728
        lea    eax, [esp + 14h]
        push   eax
        push   1
        push   edi
        lea    eax, [esp + 3ch]
        push   eax
        call   sub_1_A0EC
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_8718
        cmp    eax, 0ffh
        je     SHORT loc_1_8718
        push   dword ptr [esp + 1ch]
        lea    eax, [esp + 34h]
        push   eax
        push   dword ptr [esp + 20h]
        push   edi
        push   dword ptr [esp + 54h]
        push   0
        call   sub_1_106A4
        add    esp, 18h
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  090h
loc_1_8718:
        pop    ebp
        mov    eax, 17h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  08Dh, 049h, 000h
loc_1_8728:
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [eax + 20h]
        test   cl, 10h
        je     SHORT loc_1_8784
        mov    ebx, dword ptr [esp + 44h]
        mov    eax, dword ptr [ebx + 5ch]
        mov    edx, dword ptr [esp + 14h]
        mov    edx, dword ptr [edx + 0ch]
        and    edx, eax
        and    esi, eax
        cmp    edx, esi
        jne    SHORT loc_1_8759
        mov    eax, dword ptr [esp + 14h]
        cmp    byte ptr [eax + 33h], 0
        jne    SHORT loc_1_8759
        test   cl, 2
        jne    SHORT loc_1_8784
loc_1_8759:
        push   dword ptr [esp + 20h]
        push   2
        push   0
        push   ebx
        push   ebp
        push   dword ptr [esp + 28h]
        push   edi
        push   dword ptr [esp + 34h]
        call   sub_1_110C8
        add    esp, 20h
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  08Dh, 049h, 000h
loc_1_8784:
        pop    ebp
        mov    eax, 17h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  08Dh, 049h, 000h
loc_1_8794:
        pop    ebp
        mov    eax, 17h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  08Dh, 049h, 000h
loc_1_87A4:
        mov    eax, dword ptr [esp + 18h]
        cmp    byte ptr [eax + edi + 0eh], 21h
        jne    SHORT loc_1_87CC
        push   dword ptr [esp + 44h]
        push   esi
        push   dword ptr [esp + 20h]
        lea    eax, [esp + 3ch]
        push   eax
        push   dword ptr [esp + 60h]
        push   edi
        call   sub_1_96B8
        add    esp, 18h
        jmp    SHORT loc_1_87FE
loc_1_87CC:
        push   esi
        call   sub_1_A064
        add    esp, 4
        or     al, al
        je     SHORT loc_1_87E0
        test   byte ptr [esp + 55h], 10h
        jne    SHORT loc_1_87FE
loc_1_87E0:
        push   dword ptr [esp + 20h]
        push   2
        push   0
        push   dword ptr [esp + 50h]
        push   ebp
        push   dword ptr [esp + 28h]
        push   edi
        push   dword ptr [esp + 34h]
        call   sub_1_110C8
        add    esp, 20h
loc_1_87FE:
        pop    ebp
        mov    eax, 17h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  090h
sub_1_880C:
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 40h]
        mov    edx, dword ptr [ecx + 0ah]
        mov    esi, edx
        and    esi, 0ff00h
        mov    eax, edx
        shl    eax, 10h
        or     esi, eax
        shl    esi, 8
        mov    eax, edx
        and    eax, 0ff0000h
        shr    eax, 8
        or     esi, eax
        shr    edx, 18h
        and    edx, 0ffh
        or     esi, edx
        mov    ax, word ptr [ecx + 8]
        mov    bh, al
        mov    bl, ah
        db  00Fh, 0B7h, 005h, 0D4h, 032h, 001h, 000h
        add    eax, 3dh
        mov    edi, dword ptr [esp + 48h]
        cmp    eax, edi
        jle    SHORT loc_1_886C
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
        db  08Bh, 0FFh
loc_1_886C:
        push   esi
        call   sub_1_A064
        add    esp, 4
        or     al, al
        je     SHORT loc_1_8888
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
        db  08Bh, 0FFh
loc_1_8888:
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esp + 28h]
        push   eax
        lea    eax, [esp + 30h]
        push   eax
        push   edi
        mov    eax, dword ptr [esp + 54h]
        add    eax, 0ch
        push   eax
        call   sub_1_FFAC
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_1_88C0
        cmp    eax, 0ffh
        je     SHORT loc_1_88C0
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
loc_1_88C0:
        lea    eax, [esp + 10h]
        push   eax
        push   dword ptr [esp + 28h]
        lea    eax, [esp + 30h]
        push   eax
        db  0FFh, 035h, 0B4h, 032h, 001h, 000h
        call   sub_1_6954
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_88EB
        cmp    eax, 0ffh
        jne    NEAR PTR loc_1_8A64
loc_1_88EB:
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 28h]
        mov    ecx, dword ptr [esp + 3ch]
        test   dword ptr [ecx + 88h], eax
        je     NEAR PTR loc_1_8A64
        mov    edx, dword ptr [esp + 10h]
        add    edx, 20h
        mov    ecx, dword ptr [edx]
        mov    eax, ecx
        and    eax, 0f0h
        cmp    eax, 10h
        je     SHORT loc_1_8922
        cmp    eax, 80h
        jne    NEAR PTR loc_1_8A64
loc_1_8922:
        mov    ax, word ptr [esp + 4ch]
        mov    bp, ax
        and    bp, 0f00h
        jne    SHORT loc_1_8954
        test   ah, 80h
        je     SHORT loc_1_898C
        test   cl, 80h
        je     SHORT loc_1_898C
        push   ebx
        push   esi
        call   sub_1_9D64
        add    esp, 8
        push   eax
        push   1
        call   sub_1_FA40
        add    esp, 8
        jmp    SHORT loc_1_898C
        db  08Bh, 0FFh
loc_1_8954:
        cmp    bp, 600h
        jb     SHORT loc_1_898C
        db  080h, 03Dh, 050h, 033h, 001h, 000h, 000h
        je     SHORT loc_1_896F
        cmp    bp, 700h
        je     NEAR PTR loc_1_8A64
loc_1_896F:
        mov    edi, 0c00000bdh
        test   cl, 10h
        je     SHORT loc_1_898E
        and    ecx, 0ffffff0fh
        mov    dword ptr [edx], ecx
        mov    eax, dword ptr [esp + 10h]
        or     dword ptr [eax + 20h], 40h
        jmp    SHORT loc_1_898E
        db  090h
loc_1_898C:
        sub    edi, edi
loc_1_898E:
        mov    ebx, dword ptr [esp + 10h]
        add    ebx, 14h
        mov    ecx, dword ptr [ebx]
        or     ecx, ecx
        je     NEAR PTR loc_1_8A64
        mov    eax, dword ptr [ecx + 20h]
        mov    edx, dword ptr [eax + 2ch]
        mov    dword ptr [esp + 20h], edx
        mov    dx, word ptr [eax + 40h]
        mov    eax, dword ptr [esp + 44h]
        cmp    word ptr [eax], dx
        je     SHORT loc_1_89C4
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
        db  090h
loc_1_89C4:
        mov    dword ptr [ebx], 0
        lea    eax, [esp + 1ch]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   ecx
        call   sub_1_10AEC
        add    esp, 0ch
        mov    ecx, dword ptr [esp + 3ch]
        cmp    dword ptr [ecx + 74h], esi
        jne    SHORT loc_1_89FC
        db  0F6h, 005h, 064h, 033h, 001h, 000h, 001h
        jne    SHORT loc_1_8A34
        or     edi, edi
        jne    SHORT loc_1_8A34
        or     bp, bp
        jne    SHORT loc_1_8A34
        jmp    SHORT loc_1_8A20
        db  08Bh, 0FFh
loc_1_89FC:
        mov    ecx, dword ptr [esp + 3ch]
        cmp    dword ptr [ecx + 78h], esi
        jne    SHORT loc_1_8A34
        push   ecx
        call   sub_1_A08C
        add    esp, 4
        db  0F6h, 005h, 064h, 033h, 001h, 000h, 001h
        jne    SHORT loc_1_8A34
        or     edi, edi
        jne    SHORT loc_1_8A34
        or     bp, bp
        jne    SHORT loc_1_8A34
loc_1_8A20:
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 4ch]
        call   sub_1_9234
        add    esp, 0ch
loc_1_8A34:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_1_8A64
        or     bp, bp
        jne    SHORT loc_1_8A51
        mov    eax, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 20h]
        mov    ecx, dword ptr [edx + 88h]
        or     dword ptr [eax + 2ch], ecx
loc_1_8A51:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_1_8A64
        push   edi
        push   dword ptr [esp + 20h]
        call   dword ptr [esp + 1ch]
        add    esp, 8
loc_1_8A64:
        pop    ebp
        mov    eax, 17h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
        db  08Dh, 049h, 000h
sub_1_8A74:
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 40h]
        mov    ecx, dword ptr [esi + 0ah]
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
        and    ecx, 0ffh
        or     edx, ecx
        db  00Fh, 0B7h, 005h, 0D4h, 032h, 001h, 000h
        add    eax, 43h
        mov    edi, dword ptr [esp + 48h]
        cmp    eax, edi
        jle    SHORT loc_1_8ACC
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
        db  08Bh, 0FFh
loc_1_8ACC:
        push   edx
        call   sub_1_A064
        add    esp, 4
        or     al, al
        je     SHORT loc_1_8AE8
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
        db  08Bh, 0FFh
loc_1_8AE8:
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        lea    eax, [esp + 30h]
        push   eax
        push   edi
        mov    edi, dword ptr [esp + 54h]
        lea    eax, [edi + 0ch]
        push   eax
        call   sub_1_FFAC
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_1_8B20
        cmp    eax, 0ffh
        je     SHORT loc_1_8B20
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
loc_1_8B20:
        lea    eax, [esp + 10h]
        push   eax
        push   dword ptr [esp + 1ch]
        lea    eax, [esp + 30h]
        push   eax
        db  0FFh, 035h, 0B4h, 032h, 001h, 000h
        call   sub_1_6954
        add    esp, 10h
        or     eax, eax
        je     NEAR PTR loc_1_8C9C
        cmp    eax, 0ffh
        je     NEAR PTR loc_1_8C9C
        db  0F6h, 005h, 064h, 033h, 001h, 000h, 010h
        je     NEAR PTR loc_1_8D6C
        db  080h, 03Dh, 055h, 033h, 001h, 000h, 000h
        je     NEAR PTR loc_1_8D6C
        sub    bl, bl
        lea    eax, [esp + 10h]
        push   eax
        push   dword ptr [esp + 1ch]
        lea    eax, [esp + 30h]
        push   eax
        db  0FFh, 035h, 0B8h, 032h, 001h, 000h
        call   sub_1_6954
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_8BBC
        cmp    eax, 0ffh
        je     SHORT loc_1_8BBC
        push   dword ptr [esp + 18h]
        lea    eax, [esp + 2ch]
        push   eax
        push   dword ptr [esp + 1ch]
        push   edi
        push   dword ptr [esp + 4ch]
        push   1
        call   sub_1_106A4
        add    esp, 18h
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
        db  090h
loc_1_8BBC:
        mov    eax, dword ptr [esp + 10h]
        test   byte ptr [eax + 20h], 10h
        je     NEAR PTR loc_1_8D6C
        push   dword ptr [esp + 14h]
        push   edi
        call   sub_1_A0AC
        add    esp, 8
        and    ax, 8000h
        jne    SHORT loc_1_8BEC
        mov    ecx, dword ptr [esp + 10h]
        test   byte ptr [ecx + 20h], 2
        jne    SHORT loc_1_8BEC
        jmp    SHORT loc_1_8C61
        db  08Dh, 049h, 000h
loc_1_8BEC:
        mov    ecx, dword ptr [esp + 14h]
        mov    edx, dword ptr [ecx + edi + 1fh]
        mov    ecx, edx
        and    ecx, 0ff00h
        mov    ebp, edx
        shl    ebp, 10h
        or     ecx, ebp
        shl    ecx, 8
        mov    ebp, edx
        and    ebp, 0ff0000h
        shr    ebp, 8
        or     ecx, ebp
        shr    edx, 18h
        and    edx, 0ffh
        or     ecx, edx
        or     ax, ax
        je     SHORT loc_1_8C32
        mov    edx, dword ptr [esp + 10h]
        test   byte ptr [edx + 20h], 2
        jne    SHORT loc_1_8C41
        or     ax, ax
        jne    SHORT loc_1_8C63
loc_1_8C32:
        mov    eax, dword ptr [esp + 10h]
        test   byte ptr [eax + 20h], 2
        je     SHORT loc_1_8C63
        cmp    dword ptr [eax + 0ch], ecx
        je     SHORT loc_1_8C63
loc_1_8C41:
        mov    edx, dword ptr [esp + 3ch]
        mov    edx, dword ptr [edx + 5ch]
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 0ch]
        and    eax, edx
        and    ecx, edx
        cmp    eax, ecx
        jne    SHORT loc_1_8C61
        mov    eax, dword ptr [esp + 10h]
        cmp    byte ptr [eax + 33h], 0
        je     SHORT loc_1_8C63
loc_1_8C61:
        mov    bl, 1
loc_1_8C63:
        or     bl, bl
        je     NEAR PTR loc_1_8D6C
        push   dword ptr [esp + 24h]
        push   5
        push   6
        push   dword ptr [esp + 48h]
        add    esi, 8
        push   esi
        push   dword ptr [esp + 24h]
        push   edi
        push   dword ptr [esp + 30h]
        call   sub_1_110C8
        add    esp, 20h
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
        db  08Dh, 049h, 000h
loc_1_8C9C:
        push   dword ptr [esp + 14h]
        push   edi
        call   sub_1_A0AC
        add    esp, 8
        cmp    byte ptr [esp + 28h], 2ah
        jne    SHORT loc_1_8CC0
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
        db  08Dh, 049h, 000h
loc_1_8CC0:
        mov    ecx, dword ptr [esp + 10h]
        mov    ecx, dword ptr [ecx + 20h]
        test   cl, 2
        jne    SHORT loc_1_8CDE
        test   cl, 4
        je     NEAR PTR loc_1_8D6C
        test   ah, 80h
        jne    NEAR PTR loc_1_8D6C
loc_1_8CDE:
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 28h]
        mov    edx, dword ptr [esp + 3ch]
        test   dword ptr [edx + 88h], eax
        je     SHORT loc_1_8D6C
        and    ecx, 0f0h
        cmp    ecx, 10h
        je     SHORT loc_1_8D08
        cmp    ecx, 80h
        je     SHORT loc_1_8D2C
        jmp    SHORT loc_1_8D6C
        db  08Bh, 0FFh
loc_1_8D08:
        add    esi, 8
        push   dword ptr [esp + 24h]
        push   5
        push   6
        push   dword ptr [esp + 48h]
        push   esi
        push   dword ptr [esp + 24h]
        push   edi
        push   dword ptr [esp + 30h]
        call   sub_1_110C8
        add    esp, 20h
        jmp    SHORT loc_1_8D6C
        db  090h
loc_1_8D2C:
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [eax + 14h]
        or     ecx, ecx
        je     SHORT loc_1_8D6C
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        push   ecx
        call   sub_1_10AEC
        add    esp, 0ch
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 14h], 0
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_1_8D6C
        push   0c00000bdh
        push   dword ptr [esp + 24h]
        call   dword ptr [esp + 24h]
        add    esp, 8
loc_1_8D6C:
        pop    ebp
        mov    eax, 17h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
        db  08Dh, 049h, 000h
sub_1_8D7C:
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 48h]
        mov    ecx, dword ptr [esi + 0ah]
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
        and    ecx, 0ffh
        or     edx, ecx
        mov    ax, word ptr [esi + 8]
        mov    bh, al
        mov    bl, ah
        db  00Fh, 0B7h, 005h, 0D4h, 032h, 001h, 000h
        add    eax, 3dh
        cmp    eax, dword ptr [esp + 50h]
        jle    SHORT loc_1_8DD8
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
loc_1_8DD8:
        push   edx
        call   sub_1_A064
        add    esp, 4
        or     al, al
        je     SHORT loc_1_8DF4
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  08Bh, 0FFh
loc_1_8DF4:
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esp + 38h]
        push   eax
        push   dword ptr [esp + 5ch]
        mov    eax, dword ptr [esp + 5ch]
        add    eax, 0ch
        push   eax
        call   sub_1_FFAC
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_1_8E30
        cmp    eax, 0ffh
        je     SHORT loc_1_8E30
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  090h
loc_1_8E30:
        mov    ecx, dword ptr [esp + 4ch]
        mov    bp, word ptr [ecx + 2]
        mov    ecx, dword ptr [esi + 0ah]
        mov    edi, ecx
        and    edi, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edi, eax
        shl    edi, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    eax, 8
        or     edi, eax
        shr    ecx, 18h
        and    ecx, 0ffh
        or     edi, ecx
        db  00Fh, 0B7h, 005h, 0D4h, 032h, 001h, 000h
        test   bp, 80h
        je     NEAR PTR loc_1_8F8C
        add    eax, 3dh
        cmp    eax, dword ptr [esp + 50h]
        jle    SHORT loc_1_8E8C
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
loc_1_8E8C:
        lea    eax, [esp + 10h]
        push   eax
        push   dword ptr [esp + 18h]
        lea    eax, [esp + 38h]
        push   eax
        db  0FFh, 035h, 0B4h, 032h, 001h, 000h
        call   sub_1_6954
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_8EC0
        cmp    eax, 0ffh
        je     SHORT loc_1_8EC0
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
loc_1_8EC0:
        mov    eax, dword ptr [esp + 10h]
        mov    esi, dword ptr [eax + 14h]
        or     esi, esi
        jne    SHORT loc_1_8ED8
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
loc_1_8ED8:
        mov    ebp, dword ptr [esi + 20h]
        mov    ax, word ptr [ebp + 40h]
        mov    ecx, dword ptr [esp + 4ch]
        cmp    word ptr [ecx], ax
        je     SHORT loc_1_8EF8
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  08Dh, 049h, 000h
loc_1_8EF8:
        db  066h, 0A1h, 064h, 033h, 001h, 000h
        and    eax, 0fh
        cmp    eax, 2
        je     SHORT loc_1_8F20
        cmp    eax, 4
        je     SHORT loc_1_8F64
        cmp    eax, 8
        je     SHORT loc_1_8F64
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  08Dh, 049h, 000h
loc_1_8F20:
        or     esi, esi
        je     SHORT loc_1_8F56
        lea    eax, [esp + 2ch]
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        push   esi
        call   sub_1_10AEC
        add    esp, 0ch
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 14h], 0
        cmp    dword ptr [esp + 20h], 0
        je     SHORT loc_1_8F56
        push   0
        push   dword ptr [esp + 30h]
        call   dword ptr [esp + 28h]
        add    esp, 8
loc_1_8F56:
        pop    ebp
        mov    eax, 17h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  090h
loc_1_8F64:
        push   ebx
        push   edi
        call   sub_1_9D64
        add    esp, 8
        or     al, al
        je     SHORT loc_1_8F7E
        test   byte ptr [ebp + 4ch], 2
        je     SHORT loc_1_8F7E
        mov    word ptr [esi + 3eh], 1
loc_1_8F7E:
        pop    ebp
        mov    eax, 17h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  090h
loc_1_8F8C:
        add    eax, 43h
        cmp    eax, dword ptr [esp + 50h]
        jle    SHORT loc_1_8FA4
        mov    eax, 17h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  08Bh, 0FFh
loc_1_8FA4:
        lea    eax, [esp + 10h]
        push   eax
        push   dword ptr [esp + 18h]
        lea    eax, [esp + 38h]
        push   eax
        db  0FFh, 035h, 0B8h, 032h, 001h, 000h
        call   sub_1_6954
        add    esp, 10h
        sub    dl, dl
        or     eax, eax
        je     SHORT loc_1_8FF6
        cmp    eax, 0ffh
        je     SHORT loc_1_8FEB
        lea    eax, [esp + 10h]
        push   eax
        push   dword ptr [esp + 18h]
        lea    eax, [esp + 38h]
        push   eax
        db  0FFh, 035h, 0B4h, 032h, 001h, 000h
        call   sub_1_6954
        add    esp, 10h
        mov    dl, 1
loc_1_8FEB:
        or     eax, eax
        je     SHORT loc_1_8FF6
        cmp    eax, 0ffh
        jne    SHORT loc_1_903A
loc_1_8FF6:
        mov    edi, dword ptr [esp + 18h]
        mov    ecx, dword ptr [esp + 4ch]
        mov    ecx, dword ptr [edi + ecx + 1fh]
        mov    edi, ecx
        and    edi, 0ff00h
        mov    ebx, ecx
        shl    ebx, 10h
        or     edi, ebx
        shl    edi, 8
        mov    ebx, ecx
        and    ebx, 0ff0000h
        shr    ebx, 8
        or     edi, ebx
        shr    ecx, 18h
        and    ecx, 0ffh
        or     edi, ecx
        mov    ecx, dword ptr [esp + 10h]
        cmp    edi, dword ptr [ecx + 0ch]
        je     SHORT loc_1_903A
        mov    eax, 0c0000001h
loc_1_903A:
        or     eax, eax
        je     SHORT loc_1_9050
        cmp    eax, 0ffh
        je     SHORT loc_1_9050
        mov    eax, 3
        jmp    NEAR PTR loc_1_9124
        db  090h
loc_1_9050:
        mov    edi, dword ptr [esp + 10h]
        add    edi, 20h
        mov    ecx, dword ptr [edi]
        test   cl, 2
        je     NEAR PTR loc_1_9122
        mov    eax, ecx
        and    eax, 0f0h
        cmp    eax, 10h
        je     SHORT loc_1_907C
        cmp    eax, 80h
        je     SHORT loc_1_90CC
        jmp    NEAR PTR loc_1_9122
        db  08Bh, 0FFh
loc_1_907C:
        or     dl, dl
        jne    SHORT loc_1_90B8
        test   ch, 8
        jne    NEAR PTR loc_1_9122
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [eax + 1ch], 1
        jbe    SHORT loc_1_90A8
        and    ecx, 0ffffff0fh
        mov    dword ptr [edi], ecx
        mov    eax, dword ptr [esp + 10h]
        or     dword ptr [eax + 20h], 40h
        jmp    SHORT loc_1_9122
        db  08Dh, 049h, 000h
loc_1_90A8:
        push   dword ptr [esp + 10h]
        call   sub_1_DEA4
        add    esp, 4
        jmp    SHORT loc_1_9122
        db  08Bh, 0FFh
loc_1_90B8:
        and    ecx, 0ffffff0fh
        mov    dword ptr [edi], ecx
        mov    eax, dword ptr [esp + 10h]
        or     dword ptr [eax + 20h], 40h
        jmp    SHORT loc_1_9122
        db  08Bh, 0FFh
loc_1_90CC:
        mov    eax, dword ptr [esp + 10h]
        add    eax, 14h
        mov    ecx, dword ptr [eax]
        mov    dword ptr [eax], 0
        or     dl, dl
        jne    SHORT loc_1_90F4
        push   dword ptr [esp + 24h]
        push   0c00000bdh
        push   ecx
        call   sub_1_1088C
        add    esp, 0ch
        jmp    SHORT loc_1_9122
        db  090h
loc_1_90F4:
        or     ecx, ecx
        je     SHORT loc_1_9122
        lea    eax, [esp + 28h]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        push   ecx
        call   sub_1_10AEC
        add    esp, 0ch
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_1_9122
        push   0c00000bdh
        push   dword ptr [esp + 2ch]
        call   dword ptr [esp + 24h]
        add    esp, 8
loc_1_9122:
        sub    eax, eax
loc_1_9124:
        test   bp, 1000h
        jne    SHORT loc_1_914C
        push   dword ptr [esp + 24h]
        push   6
        push   eax
        push   dword ptr [esp + 50h]
        lea    eax, [esi + 8]
        push   eax
        push   0
        push   dword ptr [esp + 64h]
        push   dword ptr [esp + 34h]
        call   sub_1_110C8
        add    esp, 20h
loc_1_914C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
_LTEXT ends
        end
