; vnbt_lcode_01_04.asm -- VNBT object 1 segment _LTEXT  (part 4/5)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_DB70:
        sub    esp, 44h
        push   esi
        sub    esi, esi
        db  00Fh, 0B7h, 005h, 01Ch, 033h, 001h, 000h
        inc    eax
        cmp    eax, esi
        jle    SHORT loc_1_DBA0
loc_1_DB82:
        lea    eax, [esp + 4]
        push   eax
        push   0
        push   0
        call   sub_1_F7B8
        add    esp, 0ch
        inc    esi
        db  00Fh, 0B7h, 005h, 01Ch, 033h, 001h, 000h
        inc    eax
        cmp    eax, esi
        jg     SHORT loc_1_DB82
loc_1_DBA0:
        call   sub_1_125E4
        push   0
        push   1
        push   0
        db  0FFh, 035h, 027h, 033h, 001h, 000h
        call   sub_1_12638
        add    esp, 10h
        cmp    eax, -1
        jne    SHORT loc_1_DBC8
        mov    eax, 0c0000001h
        pop    esi
        add    esp, 44h
        ret
loc_1_DBC8:
        sub    eax, eax
        pop    esi
        add    esp, 44h
        ret
        db  090h
sub_1_DBD0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 1ch]
        cmp    dword ptr [ebx], 94h
        jae    SHORT loc_1_DBEC
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_DBEC:
        push   94h
        call   sub_1_001E
        add    esp, 4
        mov    edx, eax
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 25h
        rep stosd
        db  0BEh, 0F8h, 033h, 001h, 000h
        mov    edi, edx
        mov    ecx, 5
        rep movsd
        db  08Bh, 02Dh, 008h, 034h, 001h, 000h
        lea    ecx, [edx + 84h]
        mov    eax, 8
loc_1_DC25:
        mov    esi, ebp
        shl    esi, 4
        db  081h, 0C6h, 00Ch, 034h, 001h, 000h
        inc    ebp
        mov    edi, ecx
        movsd
        movsd
        movsd
        movsd
        sub    ecx, 10h
        cmp    ebp, 8
        jl     SHORT loc_1_DC41
        sub    ebp, ebp
loc_1_DC41:
        dec    eax
        jne    SHORT loc_1_DC25
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [eax], edx
        mov    dword ptr [ebx], 94h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
sub_1_DC58:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 20h]
        or     ebx, ebx
        mov    esi, dword ptr [esp + 1ch]
        jne    SHORT loc_1_DCAC
        cmp    dword ptr [esi + 58h], 0
        je     SHORT loc_1_DCA2
        push   esi
        call   sub_1_FCF4
        add    esp, 4
        lea    eax, [esp + 10h]
        push   eax
        push   esi
        call   sub_1_1030C
        add    esp, 8
        lea    eax, [esp + 10h]
        push   eax
        call   sub_1_10374
        add    esp, 4
        db  066h, 0A3h, 008h, 033h, 001h, 000h
        mov    dword ptr [esi + 58h], 0
loc_1_DCA2:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_DCAC:
        push   esi
        call   sub_1_4E80
        add    esp, 4
        mov    edi, ebx
        and    edi, 0ff00h
        mov    eax, ebx
        shl    eax, 10h
        or     edi, eax
        shl    edi, 8
        mov    eax, ebx
        and    eax, 0ff0000h
        shr    eax, 8
        or     edi, eax
        shr    ebx, 18h
        and    ebx, 0ffh
        or     edi, ebx
        mov    ecx, dword ptr [esp + 24h]
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
        mov    eax, dword ptr [esi + 88h]
        shr    eax, 1
        or     eax, eax
        je     SHORT loc_1_DD1E
        nop
loc_1_DD18:
        shr    eax, 1
        or     eax, eax
        jne    SHORT loc_1_DD18
loc_1_DD1E:
        push   esi
        call   sub_1_102C4
        add    esp, 4
        lea    ebp, [eax + 2]
        push   esi
        push   ebx
        push   edi
        db  0E8h, 0CEh, 029h, 0FDh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_DD40
        cmp    eax, 0ffh
        jne    SHORT loc_1_DD50
loc_1_DD40:
        push   esi
        push   ebp
        push   0
        add    esi, 1ch
        push   esi
        call   sub_1_104EC
        add    esp, 10h
loc_1_DD50:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_DD58:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    eax, dword ptr [esi + 84h]
        dec    eax
        mov    dword ptr [esi + 84h], eax
        je     SHORT loc_1_DD78
        mov    eax, 0ffh
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_DD78:
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [esi + 10h]
        mov    edi, dword ptr [esi + 0ch]
        mov    ebx, dword ptr [ecx + 14h]
        mov    eax, dword ptr [esi + 80h]
        mov    edx, dword ptr [ebx + 28h]
        mov    eax, dword ptr [eax + 88h]
        not    eax
        test   eax, edx
        je     SHORT loc_1_DDB1
        cmp    byte ptr [ecx + 23h], 0
        jne    SHORT loc_1_DDB1
        cmp    byte ptr [ebx + 43h], 1dh
        jne    SHORT loc_1_DDB1
        and    eax, edx
        mov    dword ptr [ebx + 28h], eax
loc_1_DDB1:
        or     ecx, ecx
        je     SHORT loc_1_DDBE
        push   ecx
        call   sub_1_DDE4
        add    esp, 4
loc_1_DDBE:
        push   esi
        call   sub_1_0024
        add    esp, 4
        or     edi, edi
        je     SHORT loc_1_DDD8
        push   0
        push   0
        push   edi
        call   sub_1_25A0
        add    esp, 0ch
loc_1_DDD8:
        pop    edi
        mov    eax, 0ffh
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_1_DDE4:
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    eax, dword ptr [esi + 18h]
        dec    eax
        mov    dword ptr [esi + 18h], eax
        je     SHORT loc_1_DE00
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_DE00:
        mov    ecx, dword ptr [esi + 14h]
        mov    edi, dword ptr [ecx + 20h]
        mov    eax, edi
        and    eax, 0ffffff0fh
        mov    dword ptr [ecx + 20h], eax
        or     eax, 40h
        mov    dword ptr [ecx + 20h], eax
        mov    eax, dword ptr [ecx + 14h]
        or     eax, eax
        je     SHORT loc_1_DE2D
        lea    ecx, [esp + 8]
        push   ecx
        push   0
        push   eax
        call   sub_1_10AEC
        add    esp, 0ch
loc_1_DE2D:
        mov    ecx, dword ptr [esi + 14h]
        test   edi, 41h
        jne    SHORT loc_1_DE74
        cmp    byte ptr [ecx + 34h], 2ah
        je     SHORT loc_1_DE74
        db  00Fh, 0B7h, 005h, 064h, 033h, 001h, 000h
        push   eax
        db  068h, 024h, 0F2h, 000h, 000h
        push   esi
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        push   ecx
        call   sub_1_EE34
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_1_DE66
        cmp    eax, 0ffh
        jne    SHORT loc_1_DE74
loc_1_DE66:
        pop    edi
        mov    eax, 0ffh
        pop    esi
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_DE74:
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [eax + 28h], 0
        push   0
        push   esi
        call   sub_1_F224
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
sub_1_DE94:
        push   dword ptr [esp + 4]
        call   sub_1_DEA4
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
sub_1_DEA4:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 1ch]
        dec    eax
        mov    dword ptr [esi + 1ch], eax
        jne    SHORT loc_1_DEDF
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        cmp    dword ptr [esi + 24h], 0facf0000h
        jne    SHORT loc_1_DED6
        test   byte ptr [esi + 20h], 8
        je     SHORT loc_1_DED6
        push   dword ptr [esi + 0ch]
        call   sub_1_0024
        add    esp, 4
loc_1_DED6:
        push   esi
        call   sub_1_0024
        add    esp, 4
loc_1_DEDF:
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_1_DEE4:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [ecx + 64h]
        dec    eax
        mov    dword ptr [ecx + 64h], eax
        or     eax, eax
        jle    SHORT loc_1_DEFC
        mov    eax, 0ffh
        pop    esi
        ret
        db  090h
loc_1_DEFC:
        push   ecx
        mov    esi, dword ptr [ecx + 58h]
        call   sub_1_FAF0
        add    esp, 4
        or     esi, esi
        je     SHORT loc_1_DF19
        push   0
        push   0
        push   esi
        call   sub_1_25A0
        add    esp, 0ch
loc_1_DF19:
        pop    esi
        mov    eax, 0ffh
        ret
sub_1_DF20:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 3ch]
        dec    eax
        mov    dword ptr [ecx + 3ch], eax
        jne    SHORT loc_1_DF49
        mov    dword ptr [ecx + 3ch], 3e8h
        push   dword ptr [ecx + 18h]
        db  068h, 068h, 0DFh, 000h, 000h
        push   0
        push   ecx
        push   0
        call   sub_1_3C00
        add    esp, 14h
loc_1_DF49:
        ret
        db  08Bh, 0FFh
sub_1_DF4C:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
        push   eax
        call   sub_1_DF20
        add    esp, 4
        sub    eax, eax
        ret
        db  08Bh, 0FFh
loc_1_DF68:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    edi, dword ptr [esi + 0ch]
        push   edi
        call   sub_1_37F0
        add    esp, 4
        cmp    dword ptr [edi + 14h], 0
        je     SHORT loc_1_DF89
        push   edi
        call   sub_1_3820
        add    esp, 4
loc_1_DF89:
        push   edi
        call   sub_1_0024
        add    esp, 4
        push   esi
        call   sub_1_0024
        add    esp, 4
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
sub_1_DFA0:
        sub    esp, 4
        cmp    dword ptr [esp + 8], 0
        db  0A1h, 080h, 02Fh, 001h, 000h
        jne    SHORT loc_1_DFB8
        mov    eax, dword ptr [eax + 80h]
        jmp    SHORT loc_1_DFBE
        db  090h
loc_1_DFB8:
        mov    eax, dword ptr [eax + 84h]
loc_1_DFBE:
        lea    ecx, [esp]
        push   ecx
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        push   eax
        call   sub_1_6954
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_DFE8
        cmp    eax, 0ffh
        je     SHORT loc_1_DFE8
        sub    eax, eax
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_DFE8:
        mov    eax, dword ptr [esp]
        mov    ecx, dword ptr [eax + 20h]
        mov    eax, dword ptr [esp + 14h]
        mov    word ptr [eax], cx
        mov    eax, dword ptr [esp]
        test   byte ptr [eax + 20h], 10h
        jne    SHORT loc_1_E008
        mov    dword ptr [esp], 0
loc_1_E008:
        mov    eax, dword ptr [esp]
        add    esp, 4
        ret
sub_1_E010:
        push   esi
        push   edi
        push   44h
        call   sub_1_001E
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        je     SHORT loc_1_E080
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 11h
        rep stosd
        lea    edi, [edx + 34h]
        mov    esi, dword ptr [esp + 0ch]
        movsd
        movsd
        movsd
        movsd
        mov    dword ptr [edx + 20h], 80h
        mov    dword ptr [edx + 1ch], 1
        mov    dword ptr [edx + 24h], 0facf0000h
        db  066h, 0A1h, 01Ch, 033h, 001h, 000h
        mov    word ptr [edx + 30h], ax
        db  0C7h, 002h, 07Ch, 032h, 001h, 000h
        db  0A1h, 080h, 032h, 001h, 000h
        mov    dword ptr [edx + 4], eax
        db  0A1h, 080h, 032h, 001h, 000h
        mov    dword ptr [eax], edx
        db  089h, 015h, 080h, 032h, 001h, 000h
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax], edx
        sub    eax, eax
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_E080:
        pop    edi
        mov    eax, 1
        pop    esi
        ret
sub_1_E088:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    eax, [esp + 10h]
        push   eax
        call   sub_1_101D0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_E0AB
        cmp    eax, 0ffh
        jne    NEAR PTR loc_1_E2FF
loc_1_E0AB:
        mov    esi, dword ptr [esp + 40h]
        or     esi, esi
        je     SHORT loc_1_E0B9
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esi], eax
loc_1_E0B9:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 1ch], 0
        mov    eax, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 3ch]
        mov    dword ptr [eax + 2ch], edi
        mov    dword ptr [esp + 14h], 0
        mov    eax, dword ptr [esp + 38h]
        or     eax, eax
        jne    SHORT loc_1_E130
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [esp + 20h]
        call   sub_1_E010
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_1_E118
        cmp    ebx, 0ffh
        je     SHORT loc_1_E118
        push   4
        push   dword ptr [esp + 14h]
        call   sub_1_D5C0
        add    esp, 8
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_E118:
        mov    ecx, dword ptr [esp + 14h]
        cmp    word ptr [esp + 28h], 1
        sbb    eax, eax
        and    eax, 0fffffffeh
        add    eax, 4
        mov    dword ptr [ecx + 20h], eax
        jmp    SHORT loc_1_E13B
        db  090h
loc_1_E130:
        mov    dword ptr [esp + 14h], eax
        mov    dword ptr [eax + 1ch], 1
loc_1_E13B:
        mov    eax, dword ptr [esp + 14h]
        and    dword ptr [eax + 20h], 0ffffff0fh
        mov    eax, dword ptr [esp + 14h]
        or     dword ptr [eax + 20h], 80h
        db  0A1h, 0E8h, 032h, 001h, 000h
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [ecx + 18h], eax
        mov    edx, dword ptr [esp + 34h]
        and    edx, 10h
        mov    eax, dword ptr [esp + 14h]
        je     SHORT loc_1_E170
        mov    byte ptr [eax + 32h], 1
        jmp    SHORT loc_1_E174
loc_1_E170:
        mov    byte ptr [eax + 32h], 0
loc_1_E174:
        or     edx, edx
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx + 38h], eax
        db  0A1h, 080h, 02Fh, 001h, 000h
        mov    bx, word ptr [eax + 9ch]
        mov    ecx, dword ptr [eax + 98h]
        je     SHORT loc_1_E1B4
        db  0BAh, 020h, 007h, 001h, 000h
        mov    eax, dword ptr [esp + 10h]
        mov    word ptr [eax + 4ch], 2
        mov    dword ptr [esp + 2ch], 0
        jmp    NEAR PTR loc_1_E24E
        db  08Dh, 049h, 000h
loc_1_E1B4:
        db  0BAh, 008h, 0E3h, 000h, 000h
        mov    ebp, dword ptr [esp + 10h]
        test   byte ptr [esp + 34h], 5
        mov    word ptr [ebp + 4ch], 2
        jne    SHORT loc_1_E20C
        test   byte ptr [esp + 34h], 8
        je     SHORT loc_1_E1E3
        cmp    dword ptr [edi + 74h], 7f000000h
        je     SHORT loc_1_E20C
        cmp    byte ptr [edi + 95h], 0
        jne    SHORT loc_1_E20C
loc_1_E1E3:
        cmp    dword ptr [edi + 74h], 7f000000h
        je     SHORT loc_1_E1F5
        cmp    byte ptr [edi + 95h], 0
        je     SHORT loc_1_E223
loc_1_E1F5:
        mov    ecx, 0ah
        mov    bx, 1
        mov    eax, dword ptr [esp + 10h]
        mov    word ptr [eax + 4ch], 4
        jmp    SHORT loc_1_E223
        db  08Bh, 0FFh
loc_1_E20C:
        mov    ecx, dword ptr [eax + 94h]
        mov    bx, word ptr [eax + 9eh]
        mov    eax, dword ptr [esp + 10h]
        mov    word ptr [eax + 4ch], 1
loc_1_E223:
        cmp    dword ptr [edi + 58h], 7f000000h
        jne    SHORT loc_1_E24E
        mov    bx, 1
        mov    ecx, 0ah
        mov    eax, dword ptr [esp + 10h]
        test   byte ptr [esp + 34h], 6
        mov    word ptr [eax + 4ch], bx
        je     SHORT loc_1_E24E
        mov    eax, dword ptr [esp + 10h]
        mov    word ptr [eax + 4ch], 4
loc_1_E24E:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 34h], 2
        mov    edi, dword ptr [esp + 14h]
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [edi + 10h], eax
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax + 14h], 0
        push   0
        push   ecx
        movzx  eax, bx
        push   eax
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 3ch]
        push   edx
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 34h]
        call   sub_1_10C0C
        add    esp, 24h
        mov    edi, eax
        push   dword ptr [esp + 10h]
        call   sub_1_F280
        add    esp, 4
        or     edi, edi
        je     SHORT loc_1_E2F8
        cmp    edi, 0ffh
        je     SHORT loc_1_E2F8
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax + 14h], 0
        cmp    edi, 1
        je     SHORT loc_1_E2CC
        mov    eax, dword ptr [esp + 10h]
        push   dword ptr [eax + 1ch]
        call   sub_1_0024
        add    esp, 4
loc_1_E2CC:
        or     esi, esi
        je     SHORT loc_1_E2D6
        mov    dword ptr [esi], 0
loc_1_E2D6:
        push   dword ptr [esp + 10h]
        call   sub_1_F29C
        add    esp, 4
        push   dword ptr [esp + 14h]
        call   sub_1_DEA4
        add    esp, 4
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_E2F8:
        mov    edi, 0ffh
        mov    eax, edi
loc_1_E2FF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  090h
loc_1_E308:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 20h]
        or     esi, esi
        je     NEAR PTR loc_1_E5D8
        mov    ax, word ptr [esi + 3ch]
        test   al, 10h
        je     SHORT loc_1_E354
        and    ax, 0ffefh
        mov    word ptr [esi + 3ch], ax
        or     ax, 1
        mov    word ptr [esi + 3ch], ax
        mov    edi, dword ptr [esp + 18h]
        mov    eax, dword ptr [edi + 2ch]
        cmp    dword ptr [eax + 68h], 0
        jne    NEAR PTR loc_1_E5E4
        mov    dword ptr [esi + 38h], 0ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
loc_1_E354:
        mov    ebp, dword ptr [esi + 2ch]
        test   byte ptr [ebp + 4ch], 40h
        mov    ebx, dword ptr [esi + 30h]
        je     SHORT loc_1_E3DC
        mov    edi, dword ptr [esp + 18h]
        mov    edx, dword ptr [edi + 38h]
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [edx + 4], edx
        mov    eax, dword ptr [edi + 38h]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esi + 34h]
        mov    dword ptr [eax + 14h], 0
        mov    dword ptr [esi + 30h], 0
        mov    eax, dword ptr [edi + 38h]
        and    dword ptr [eax + 20h], 0ffffff0fh
        mov    eax, dword ptr [edi + 38h]
        or     dword ptr [eax + 20h], 20h
        mov    eax, dword ptr [edi + 38h]
        mov    dword ptr [eax + 14h], 0
        push   dword ptr [edi + 38h]
        call   sub_1_DEA4
        add    esp, 4
        mov    dword ptr [edi + 38h], 0
        push   0c0000120h
        push   ebp
        push   ebx
        call   sub_1_E64C
        add    esp, 0ch
        push   edi
        call   sub_1_F280
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_E3DC:
        or     ebx, ebx
        je     NEAR PTR loc_1_E5E4
        mov    ax, word ptr [esi + 3eh]
        dec    ax
        mov    word ptr [esi + 3eh], ax
        jne    NEAR PTR loc_1_E59C
        db  066h, 0A1h, 0D0h, 032h, 001h, 000h
        mov    word ptr [esi + 3eh], ax
        mov    edi, dword ptr [esp + 18h]
        mov    cx, word ptr [edi + 4ch]
        mov    dx, cx
        and    dx, 0fff8h
        mov    word ptr [edi + 4ch], dx
        test   cl, 1
        je     SHORT loc_1_E44C
        db  0F6h, 005h, 064h, 033h, 001h, 000h, 004h
        je     SHORT loc_1_E44C
        mov    eax, dword ptr [edi + 2ch]
        cmp    dword ptr [eax + 74h], 7f000000h
        je     SHORT loc_1_E44C
        cmp    byte ptr [eax + 95h], 0
        jne    SHORT loc_1_E44C
        or     dx, 2
        mov    word ptr [edi + 4ch], dx
        db  0A1h, 0CCh, 032h, 001h, 000h
        mov    dword ptr [esi + 38h], eax
        jmp    NEAR PTR loc_1_E5A0
        db  08Bh, 0FFh
loc_1_E44C:
        test   cl, 2
        je     SHORT loc_1_E490
        db  0F6h, 005h, 064h, 033h, 001h, 000h, 001h
        jne    SHORT loc_1_E490
        or     dx, 4
        mov    word ptr [edi + 4ch], dx
        mov    eax, dword ptr [edi + 2ch]
        cmp    dword ptr [eax + 78h], 7f000000h
        je     SHORT loc_1_E47B
        cmp    byte ptr [eax + 95h], 0
        je     NEAR PTR loc_1_E5A0
loc_1_E47B:
        mov    dword ptr [esi + 38h], 0ah
        mov    word ptr [esi + 3eh], 1
        jmp    NEAR PTR loc_1_E5A0
        db  08Dh, 049h, 000h
loc_1_E490:
        test   cl, 4
        je     SHORT loc_1_E4D4
        db  0F6h, 005h, 064h, 033h, 001h, 000h, 008h
        je     SHORT loc_1_E4D4
        or     dx, 1
        mov    word ptr [edi + 4ch], dx
        db  0A1h, 0C8h, 032h, 001h, 000h
        mov    dword ptr [esi + 38h], eax
        db  066h, 0A1h, 0D2h, 032h, 001h, 000h
        mov    word ptr [esi + 3eh], ax
        test   ch, 2
        jne    NEAR PTR loc_1_E5A0
        push   dword ptr [edi + 2ch]
        call   sub_1_E5EC
        add    esp, 4
        jmp    NEAR PTR loc_1_E5A0
        db  08Dh, 049h, 000h
loc_1_E4D4:
        mov    eax, 0c0000001h
        mov    ecx, dword ptr [edi + 38h]
        mov    ebp, dword ptr [ecx]
        mov    edx, dword ptr [ecx + 4]
        mov    dword ptr [edx], ebp
        mov    dword ptr [ebp + 4], edx
        mov    dword ptr [ecx + 4], ecx
        mov    ecx, dword ptr [edi + 38h]
        mov    edx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], edx
        mov    ecx, dword ptr [esi + 34h]
        mov    dword ptr [ecx + 14h], 0
        mov    dword ptr [esi + 30h], 0
        db  080h, 03Dh, 054h, 033h, 001h, 000h, 000h
        jne    SHORT loc_1_E514
        db  080h, 03Dh, 053h, 033h, 001h, 000h, 000h
        je     SHORT loc_1_E53A
loc_1_E514:
        mov    ecx, dword ptr [edi + 38h]
        cmp    byte ptr [ecx + 32h], 0
        jne    SHORT loc_1_E53A
        or     ebx, ebx
        je     SHORT loc_1_E53A
        push   dword ptr [esp + 10h]
        push   dword ptr [edi + 2ch]
        db  068h, 054h, 02Bh, 001h, 000h
        push   ebx
        push   dword ptr [esi + 2ch]
        push   edi
        call   sub_1_12A74
        add    esp, 18h
loc_1_E53A:
        or     eax, eax
        je     NEAR PTR loc_1_E5E4
        cmp    eax, 0ffh
        je     NEAR PTR loc_1_E5E4
        mov    esi, dword ptr [esi + 2ch]
        mov    eax, dword ptr [edi + 38h]
        and    dword ptr [eax + 20h], 0ffffff0fh
        mov    eax, dword ptr [edi + 38h]
        or     dword ptr [eax + 20h], 20h
        mov    eax, dword ptr [edi + 38h]
        mov    dword ptr [eax + 14h], 0
        push   dword ptr [edi + 38h]
        call   sub_1_DEA4
        add    esp, 4
        mov    dword ptr [edi + 38h], 0
        push   15h
        push   esi
        push   ebx
        call   sub_1_E64C
        add    esp, 0ch
        push   edi
        call   sub_1_F280
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_E59C:
        mov    edi, dword ptr [esp + 18h]
loc_1_E5A0:
        inc    dword ptr [edi + 34h]
        push   0
        push   0
        push   0
        push   0
        push   0
        push   0
        push   edi
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        push   dword ptr [edi + 38h]
        call   sub_1_10C0C
        add    esp, 24h
        push   edi
        call   sub_1_F280
        add    esp, 4
        or     word ptr [esi + 3ch], 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
loc_1_E5D8:
        push   dword ptr [esp + 18h]
        call   sub_1_F29C
        add    esp, 4
loc_1_E5E4:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_E5EC:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        cmp    byte ptr [esi + 95h], 0
        jne    SHORT loc_1_E639
        db  0F6h, 005h, 064h, 033h, 001h, 000h, 001h
        jne    SHORT loc_1_E639
        lea    eax, [esp + 4]
        push   eax
        push   1
        push   0
        push   0
        db  068h, 040h, 0E6h, 000h, 000h
        push   0
        push   esi
        db  0FFh, 035h, 0ECh, 032h, 001h, 000h
        call   sub_1_109B8
        add    esp, 20h
        or     eax, eax
        je     SHORT loc_1_E632
        cmp    eax, 0ffh
        jne    SHORT loc_1_E639
loc_1_E632:
        mov    byte ptr [esi + 95h], 1
loc_1_E639:
        pop    esi
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_E640:
        mov    eax, dword ptr [esp + 4]
        mov    byte ptr [eax + 95h], 0
        ret
sub_1_E64C:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    ebx, [esp + 18h]
        mov    ebp, ebx
        mov    ecx, dword ptr [esp + 28h]
        or     ecx, ecx
        je     SHORT loc_1_E694
        mov    edi, dword ptr [ecx + 2ch]
        mov    eax, dword ptr [ecx + 8]
        mov    edx, eax
        sub    edx, ecx
        cmp    edx, 8
        je     SHORT loc_1_E698
        mov    dword ptr [esp + 18h], eax
        mov    eax, ebx
        mov    edx, dword ptr [esp + 18h]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [esp + 1ch], eax
        mov    eax, ebx
        mov    edx, dword ptr [esp + 1ch]
        mov    dword ptr [edx], eax
        mov    ebp, dword ptr [esp + 18h]
        jmp    SHORT loc_1_E698
        db  08Bh, 0FFh
loc_1_E694:
        mov    edi, dword ptr [esp + 10h]
loc_1_E698:
        mov    esi, dword ptr [esp + 2ch]
        push   esi
        push   ecx
        call   dword ptr [esp + 2ch]
        add    esp, 8
        cmp    ebp, ebx
        je     SHORT loc_1_E711
loc_1_E6A9:
        lea    ecx, [ebp - 8]
        mov    ebp, dword ptr [ebp]
        cmp    dword ptr [ecx + 2ch], edi
        je     SHORT loc_1_E6B8
        or     esi, esi
        jne    SHORT loc_1_E6CC
loc_1_E6B8:
        push   esi
        push   ecx
        call   dword ptr [ecx + 48h]
        add    esp, 8
        cmp    ebp, ebx
        jne    SHORT loc_1_E6A9
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_E6CC:
        lea    edx, [ecx + 8]
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [ecx + 0ch], eax
        mov    dword ptr [eax], edx
        lea    eax, [esp + 18h]
        cmp    dword ptr [edx], eax
        jne    SHORT loc_1_E6E2
        mov    dword ptr [edx], edx
loc_1_E6E2:
        lea    eax, [esp + 14h]
        push   eax
        push   0
        push   dword ptr [ecx + 2ch]
        push   0
        db  066h, 0A1h, 064h, 033h, 001h, 000h
        and    eax, 0fh
        push   eax
        push   dword ptr [ecx + 48h]
        push   ecx
        push   0
        push   0
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        push   dword ptr [ecx + 38h]
        call   sub_1_E088
        add    esp, 2ch
loc_1_E711:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
sub_1_E71C:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 14h], 0
        mov    bx, word ptr [esp + 34h]
        or     bx, bx
        je     SHORT loc_1_E73F
        cmp    bx, 2
        mov    di, 1
        jne    SHORT loc_1_E742
loc_1_E73F:
        sub    di, di
loc_1_E742:
        mov    esi, dword ptr [esp + 20h]
        or     esi, esi
        lea    eax, [esp + 10h]
        push   eax
        je     SHORT loc_1_E780
        push   0
        movzx  eax, di
        push   eax
        push   esi
        push   dword ptr [esp + 38h]
        push   dword ptr [esp + 38h]
        db  0A1h, 080h, 02Fh, 001h, 000h
        push   dword ptr [eax + 80h]
        call   sub_1_6678
        add    esp, 1ch
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx + 2ch], 0
        jmp    SHORT loc_1_E7DA
        db  08Bh, 0FFh
loc_1_E780:
        db  0A1h, 080h, 02Fh, 001h, 000h
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 2ch]
        push   dword ptr [eax + 80h]
        call   sub_1_6954
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_E7AA
        cmp    eax, 0ffh
        jne    NEAR PTR loc_1_EA63
loc_1_E7AA:
        mov    ecx, dword ptr [esp + 10h]
        add    ecx, 20h
        mov    edx, dword ptr [ecx]
        mov    ebp, edx
        and    ebp, 0fffffff0h
        mov    dword ptr [ecx], ebp
        mov    ebp, dword ptr [esp + 10h]
        cmp    di, 1
        sbb    ecx, ecx
        and    ecx, 0fffffffeh
        add    ecx, 4
        or     dword ptr [ebp + 20h], ecx
        test   dl, 1
        je     SHORT loc_1_E7DA
        mov    ecx, dword ptr [esp + 10h]
        or     dword ptr [ecx + 20h], 1
loc_1_E7DA:
        or     bx, bx
        je     SHORT loc_1_E7F0
        cmp    bx, 2
        je     SHORT loc_1_E7F0
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx + 0ch], 0
loc_1_E7F0:
        or     eax, eax
        je     SHORT loc_1_E80C
        cmp    eax, 0ffh
        je     SHORT loc_1_E80C
        mov    esi, 0c0000001h
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_E80C:
        db  0A1h, 0E0h, 032h, 001h, 000h
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx + 18h], eax
        mov    edi, dword ptr [esp + 2ch]
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [ecx + 14h], eax
        mov    eax, dword ptr [edi + 10h]
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, dword ptr [esp + 38h]
        mov    ecx, dword ptr [ebp + 88h]
        mov    edx, ecx
        not    edx
        and    dword ptr [eax + 2ch], edx
        mov    eax, dword ptr [esp + 10h]
        or     dword ptr [eax + 28h], ecx
        mov    eax, dword ptr [esp + 10h]
        and    dword ptr [eax + 20h], 0ffffff0fh
        mov    eax, dword ptr [esp + 24h]
        cmp    byte ptr [eax], 2ah
        jne    SHORT loc_1_E874
        mov    eax, dword ptr [esp + 10h]
        or     dword ptr [eax + 20h], 10h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_E874:
        mov    ecx, dword ptr [esp + 10h]
        add    ecx, 20h
        mov    eax, dword ptr [ecx]
        test   al, 1
        jne    NEAR PTR loc_1_EA54
        cmp    bx, 2
        jae    NEAR PTR loc_1_EA54
        cmp    esi, 7f000000h
        jne    SHORT loc_1_E8A8
        or     eax, 10h
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_E8A8:
        or     eax, 80h
        mov    dword ptr [ecx], eax
        lea    eax, [esp + 14h]
        push   eax
        call   sub_1_101D0
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_E8E0
        cmp    esi, 0ffh
        je     SHORT loc_1_E8E0
        push   dword ptr [esp + 10h]
        call   sub_1_DEA4
        add    esp, 4
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_E8E0:
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, eax
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 14h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [ecx + 38h], eax
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax + 2ch], ebp
        db  08Bh, 015h, 080h, 02Fh, 001h, 000h
        mov    cx, word ptr [edx + 9eh]
        inc    cx
        mov    bx, cx
        mov    esi, dword ptr [edx + 94h]
        mov    eax, dword ptr [esp + 14h]
        mov    word ptr [eax + 4ch], 1
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax + 34h], 2
        db  0F6h, 005h, 064h, 033h, 001h, 000h, 00Ah
        je     SHORT loc_1_E9A7
        mov    bx, word ptr [edx + 9ch]
        inc    bx
        mov    esi, dword ptr [edx + 98h]
        mov    eax, dword ptr [esp + 14h]
        mov    word ptr [eax + 4ch], 2
        cmp    dword ptr [ebp + 74h], 7f000000h
        je     SHORT loc_1_E969
        cmp    byte ptr [ebp + 95h], 0
        je     SHORT loc_1_E9A7
loc_1_E969:
        db  0F6h, 005h, 064h, 033h, 001h, 000h, 008h
        je     SHORT loc_1_E994
        mov    eax, dword ptr [esp + 14h]
        mov    word ptr [eax + 4ch], 1
        mov    bx, cx
        mov    esi, dword ptr [edx + 94h]
        push   0
        push   1
        call   sub_1_FA40
        add    esp, 8
        jmp    SHORT loc_1_E9A7
        db  090h
loc_1_E994:
        mov    esi, 0ah
        mov    bx, 1
        mov    eax, dword ptr [esp + 14h]
        mov    word ptr [eax + 4ch], 4
loc_1_E9A7:
        push   3
        mov    eax, dword ptr [esp + 14h]
        inc    dword ptr [eax + 1ch]
        push   esi
        movzx  eax, bx
        push   eax
        push   dword ptr [esp + 3ch]
        push   edi
        db  068h, 06Ch, 0EAh, 000h, 000h
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 44h]
        push   dword ptr [esp + 30h]
        call   sub_1_10C0C
        add    esp, 24h
        mov    esi, eax
        push   dword ptr [esp + 14h]
        call   sub_1_F280
        add    esp, 4
        push   dword ptr [esp + 10h]
        call   sub_1_DEA4
        add    esp, 4
        or     esi, esi
        je     SHORT loc_1_EA44
        cmp    esi, 0ffh
        je     SHORT loc_1_EA44
        mov    eax, dword ptr [esp + 10h]
        and    dword ptr [eax + 20h], 0ffffff0fh
        mov    eax, dword ptr [esp + 10h]
        or     dword ptr [eax + 20h], 40h
        cmp    esi, 1
        je     SHORT loc_1_EA20
        mov    eax, dword ptr [esp + 14h]
        push   dword ptr [eax + 1ch]
        call   sub_1_0024
        add    esp, 4
loc_1_EA20:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 14h], 0
        push   dword ptr [esp + 14h]
        call   sub_1_F29C
        add    esp, 4
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_EA44:
        pop    ebp
        mov    eax, 0ffh
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_EA54:
        or     eax, 10h
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 10h]
        or     dword ptr [eax + 20h], 1
        sub    eax, eax
loc_1_EA63:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  090h
loc_1_EA6C:
        push   ebx
        push   esi
        push   edi
        mov    edi, 3
        mov    esi, dword ptr [esp + 18h]
        or     esi, esi
        je     NEAR PTR loc_1_EC98
        mov    cx, word ptr [esi + 3ch]
        test   cl, 10h
        je     SHORT loc_1_EAE4
        and    cx, 0ffefh
        mov    word ptr [esi + 3ch], cx
        or     cx, 1
        mov    word ptr [esi + 3ch], cx
        mov    ebx, dword ptr [esp + 10h]
        mov    ecx, dword ptr [ebx + 2ch]
        cmp    dword ptr [ecx + 68h], 0
        je     SHORT loc_1_EAD8
        mov    ax, word ptr [ebx + 4ch]
        test   al, 2
        je     SHORT loc_1_EAB8
        cmp    dword ptr [ecx + 74h], 7f000000h
        je     SHORT loc_1_EAD8
loc_1_EAB8:
        test   al, 4
        je     NEAR PTR loc_1_ECA4
        cmp    dword ptr [ecx + 78h], 7f000000h
        jne    NEAR PTR loc_1_ECA4
        mov    dword ptr [esi + 38h], 0ah
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_EAD8:
        pop    edi
        mov    dword ptr [esi + 38h], 0ah
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_EAE4:
        cmp    dword ptr [esi + 30h], 0
        je     NEAR PTR loc_1_ECA4
        mov    ax, word ptr [esi + 3eh]
        dec    ax
        mov    word ptr [esi + 3eh], ax
        je     SHORT loc_1_EB38
        cmp    ax, 1
        mov    ebx, dword ptr [esp + 10h]
        jne    NEAR PTR loc_1_EC2D
        test   byte ptr [ebx + 4ch], 1
        je     SHORT loc_1_EB18
        mov    edi, 4
        jmp    NEAR PTR loc_1_EC2D
loc_1_EB18:
        db  0F6h, 005h, 064h, 033h, 001h, 000h, 00Ah
        je     NEAR PTR loc_1_EC2D
        or     cx, 1
        mov    word ptr [esi + 3ch], cx
        mov    dword ptr [esi + 38h], 5
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_EB38:
        mov    ebx, dword ptr [esp + 10h]
        mov    dx, word ptr [ebx + 4ch]
        mov    cx, dx
        and    cx, 0fffch
        mov    word ptr [ebx + 4ch], cx
        db  0A1h, 0CCh, 032h, 001h, 000h
        mov    dword ptr [esi + 38h], eax
        db  066h, 0A1h, 0D0h, 032h, 001h, 000h
        inc    ax
        mov    word ptr [esi + 3eh], ax
        mov    ax, dx
        and    ax, 1
        je     SHORT loc_1_EBB0
        db  0F6h, 005h, 064h, 033h, 001h, 000h, 004h
        je     SHORT loc_1_EBB0
        push   0
        push   1
        call   sub_1_FA40
        add    esp, 8
        or     word ptr [ebx + 4ch], 2
        mov    eax, dword ptr [ebx + 2ch]
        cmp    dword ptr [eax + 74h], 7f000000h
        je     SHORT loc_1_EB9C
        cmp    byte ptr [eax + 95h], 0
        je     NEAR PTR loc_1_EC2D
loc_1_EB9C:
        mov    dword ptr [esi + 38h], 0ah
        mov    word ptr [esi + 3eh], 1
        jmp    NEAR PTR loc_1_EC2D
        db  08Bh, 0FFh
loc_1_EBB0:
        test   dl, 2
        je     SHORT loc_1_EBEC
        db  0F6h, 005h, 064h, 033h, 001h, 000h, 001h
        jne    SHORT loc_1_EBEC
        or     cx, 4
        mov    word ptr [ebx + 4ch], cx
        mov    eax, dword ptr [ebx + 2ch]
        cmp    dword ptr [eax + 78h], 7f000000h
        je     SHORT loc_1_EBDB
        cmp    byte ptr [eax + 95h], 0
        je     SHORT loc_1_EC2D
loc_1_EBDB:
        mov    dword ptr [esi + 38h], 0ah
        mov    word ptr [esi + 3eh], 1
        jmp    SHORT loc_1_EC2D
        db  08Bh, 0FFh
loc_1_EBEC:
        db  0F6h, 005h, 064h, 033h, 001h, 000h, 008h
        je     SHORT loc_1_EC60
        or     ax, ax
        jne    SHORT loc_1_EC60
        test   dl, 4
        je     SHORT loc_1_EC2D
        or     cx, 1
        mov    word ptr [ebx + 4ch], cx
        push   0
        push   1
        call   sub_1_FA40
        add    esp, 8
        db  0A1h, 0C8h, 032h, 001h, 000h
        mov    dword ptr [esi + 38h], eax
        db  0A1h, 080h, 02Fh, 001h, 000h
        mov    ax, word ptr [eax + 9eh]
        inc    ax
        mov    word ptr [esi + 3eh], ax
loc_1_EC2D:
        push   edi
        inc    dword ptr [ebx + 34h]
        push   0
        push   0
        push   0
        push   0
        push   0
        push   ebx
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        push   dword ptr [ebx + 38h]
        call   sub_1_10C0C
        add    esp, 24h
        push   ebx
        call   sub_1_F280
        add    esp, 4
        or     word ptr [esi + 3ch], 1
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_EC60:
        db  0F6h, 005h, 064h, 033h, 001h, 000h, 001h
        je     SHORT loc_1_EC75
        push   0
        push   1
        call   sub_1_FA40
        add    esp, 8
loc_1_EC75:
        push   dword ptr [ebx + 2ch]
        call   sub_1_E5EC
        add    esp, 4
        push   ebx
        call   sub_1_F280
        add    esp, 4
        push   0
        push   esi
        call   sub_1_108C8
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_EC98:
        push   dword ptr [esp + 10h]
        call   sub_1_F29C
        add    esp, 4
loc_1_ECA4:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_ECA8:
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 1ch]
        or     esi, esi
        je     NEAR PTR loc_1_ED86
        mov    ax, word ptr [esi + 3eh]
        dec    ax
        mov    word ptr [esi + 3eh], ax
        je     NEAR PTR loc_1_ED60
        mov    edi, dword ptr [esp + 14h]
        inc    dword ptr [edi + 34h]
        push   edi
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 10h]
        push   eax
        push   0
        push   0
        push   0
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        mov    eax, dword ptr [edi + 38h]
        add    eax, 34h
        push   eax
        call   sub_1_10E60
        add    esp, 20h
        or     eax, eax
        je     SHORT loc_1_ED4A
        mov    eax, dword ptr [esp + 8]
        and    word ptr [eax + 2], 0fffeh
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 8]
        mov    byte ptr [eax + ecx - 3], 21h
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [edi + 1ch], eax
        mov    eax, dword ptr [edi + 2ch]
        cmp    dword ptr [eax + 58h], 0
        je     SHORT loc_1_ED28
        mov    eax, dword ptr [eax + 68h]
        jmp    SHORT loc_1_ED2A
        db  08Bh, 0FFh
loc_1_ED28:
        sub    eax, eax
loc_1_ED2A:
        push   10h
        push   89h
        push   dword ptr [esp + 10h]
        db  068h, 0A8h, 010h, 001h, 000h
        push   eax
        mov    eax, dword ptr [edi + 38h]
        push   dword ptr [eax + 0ch]
        push   edi
        call   sub_1_115D4
        add    esp, 1ch
loc_1_ED4A:
        push   edi
        call   sub_1_F280
        add    esp, 4
        or     word ptr [esi + 3ch], 1
        pop    edi
        pop    esi
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_ED60:
        mov    edx, dword ptr [esi + 30h]
        mov    dword ptr [esi + 30h], 0
        or     edx, edx
        je     SHORT loc_1_ED86
        mov    edi, dword ptr [esp + 14h]
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   15h
        push   dword ptr [esi + 2ch]
        call   edx
        add    esp, 8
loc_1_ED86:
        pop    edi
        pop    esi
        add    esp, 8
        ret
loc_1_ED8C:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 14h]
        or     edi, edi
        je     SHORT loc_1_EDEB
        cmp    dword ptr [edi + 30h], 0
        je     SHORT loc_1_EDEB
        mov    ax, word ptr [edi + 3eh]
        dec    ax
        mov    word ptr [edi + 3eh], ax
        je     SHORT loc_1_EDE0
        mov    esi, dword ptr [esp + 0ch]
        inc    dword ptr [esi + 34h]
        push   7
        push   0
        push   0
        push   0
        push   0
        push   0
        push   esi
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        push   dword ptr [esi + 38h]
        call   sub_1_10C0C
        add    esp, 24h
        or     word ptr [edi + 3ch], 1
        push   esi
        call   sub_1_F280
        add    esp, 4
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_EDE0:
        push   15h
        push   edi
        call   sub_1_108C8
        add    esp, 8
loc_1_EDEB:
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
sub_1_EDF0:
        push   esi
        push   edi
        mov    esi, 1
        mov    edx, dword ptr [esp + 10h]
        mov    ecx, dword ptr [edx + 28h]
        test   esi, ecx
        jne    SHORT loc_1_EE08
loc_1_EE02:
        add    esi, esi
        test   esi, ecx
        je     SHORT loc_1_EE02
loc_1_EE08:
        db  08Bh, 03Dh, 03Ch, 032h, 001h, 000h
        mov    eax, 1
        cmp    esi, eax
        jbe    SHORT loc_1_EE20
        nop
loc_1_EE18:
        mov    edi, dword ptr [edi]
        add    eax, eax
        cmp    eax, esi
        jb     SHORT loc_1_EE18
loc_1_EE20:
        not    esi
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax + 2ch], edi
        and    esi, ecx
        mov    dword ptr [edx + 28h], esi
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_1_EE34:
        sub    esp, 0ch
        push   esi
        push   edi
        lea    eax, [esp + 8]
        push   eax
        call   sub_1_101D0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_EE55
        cmp    eax, 0ffh
        jne    NEAR PTR loc_1_EF64
loc_1_EE55:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 1ch], 0
        mov    esi, dword ptr [esp + 18h]
        push   esi
        push   dword ptr [esp + 0ch]
        call   sub_1_EDF0
        add    esp, 8
        mov    eax, dword ptr [esp + 28h]
        and    eax, 0fh
        cmp    eax, 2
        je     SHORT loc_1_EEA0
        cmp    eax, 4
        je     SHORT loc_1_EEA0
        cmp    eax, 8
        je     SHORT loc_1_EEA0
        mov    dx, 1
        db  0A1h, 080h, 02Fh, 001h, 000h
        mov    ecx, dword ptr [eax + 94h]
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax + 4ch], dx
        jmp    SHORT loc_1_EEBC
loc_1_EEA0:
        db  0A1h, 080h, 02Fh, 001h, 000h
        mov    dx, word ptr [eax + 9ch]
        mov    ecx, dword ptr [eax + 98h]
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax + 4ch], 2
loc_1_EEBC:
        push   6
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax + 34h], 2
        push   ecx
        movzx  eax, dx
        push   eax
        push   dword ptr [esp + 30h]
        push   dword ptr [esp + 30h]
        db  068h, 06Ch, 0EFh, 000h, 000h
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 38h]
        push   esi
        call   sub_1_10C0C
        add    esp, 24h
        mov    edi, eax
        push   dword ptr [esp + 8]
        call   sub_1_F280
        add    esp, 4
        or     edi, edi
        je     SHORT loc_1_EF62
        cmp    edi, 0ffh
        je     SHORT loc_1_EF62
        mov    dword ptr [esp + 0ch], 0
        mov    ecx, dword ptr [esi + 14h]
        or     ecx, ecx
        je     SHORT loc_1_EF38
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 10h]
        push   eax
        push   ecx
        call   sub_1_10AEC
        add    esp, 0ch
        mov    dword ptr [esi + 14h], 0
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 0ch
        ret
        db  090h
loc_1_EF38:
        cmp    edi, 1
        je     SHORT loc_1_EF54
        push   5
        push   dword ptr [esp + 0ch]
        call   sub_1_D5C0
        add    esp, 8
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 0ch
        ret
        db  090h
loc_1_EF54:
        push   4
        push   dword ptr [esp + 0ch]
        call   sub_1_D5C0
        add    esp, 8
loc_1_EF62:
        mov    eax, edi
loc_1_EF64:
        pop    edi
        pop    esi
        add    esp, 0ch
        ret
        db  08Bh, 0FFh
loc_1_EF6C:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        or     esi, esi
        je     NEAR PTR loc_1_F01C
        mov    ax, word ptr [esi + 3eh]
        dec    ax
        mov    word ptr [esi + 3eh], ax
        jne    SHORT loc_1_EFE4
        db  0F6h, 005h, 064h, 033h, 001h, 000h, 004h
        mov    edi, dword ptr [esp + 0ch]
        je     SHORT loc_1_EFC0
        mov    ax, word ptr [edi + 4ch]
        test   al, 2
        je     SHORT loc_1_EFC0
        and    ax, 0fffdh
        mov    word ptr [edi + 4ch], ax
        or     ax, 1
        mov    word ptr [edi + 4ch], ax
        db  0A1h, 0C8h, 032h, 001h, 000h
        mov    dword ptr [esi + 38h], eax
        db  066h, 0A1h, 0D2h, 032h, 001h, 000h
        mov    word ptr [esi + 3eh], ax
        jmp    SHORT loc_1_EFE8
        db  090h
loc_1_EFC0:
        push   15h
        push   esi
        call   sub_1_108C8
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_EFD6
        cmp    eax, 0ffh
        jne    SHORT loc_1_F028
loc_1_EFD6:
        push   edi
        call   sub_1_F280
        add    esp, 4
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_EFE4:
        mov    edi, dword ptr [esp + 0ch]
loc_1_EFE8:
        inc    dword ptr [edi + 34h]
        push   6
        push   0
        push   0
        push   0
        push   0
        push   0
        push   edi
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        push   dword ptr [edi + 38h]
        call   sub_1_10C0C
        add    esp, 24h
        push   edi
        call   sub_1_F280
        add    esp, 4
        or     word ptr [esi + 3ch], 1
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_F01C:
        push   dword ptr [esp + 0ch]
        call   sub_1_F29C
        add    esp, 4
loc_1_F028:
        pop    edi
        pop    esi
        ret
        db  090h
loc_1_F02C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 14h]
        mov    edx, dword ptr [edi + 2ch]
        mov    ebx, dword ptr [esp + 1ch]
        or     ebx, ebx
        jne    SHORT loc_1_F050
        push   edi
        call   sub_1_F29C
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_F050:
        mov    ax, word ptr [ebx + 3ch]
        test   al, 10h
        je     SHORT loc_1_F080
        and    ax, 0ffefh
        mov    word ptr [ebx + 3ch], ax
        or     ax, 1
        mov    word ptr [ebx + 3ch], ax
        cmp    dword ptr [edx + 68h], 0
        jne    NEAR PTR loc_1_F21C
        mov    dword ptr [ebx + 38h], 0ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_F080:
        mov    ebp, dword ptr [ebx + 30h]
        or     ebp, ebp
        je     NEAR PTR loc_1_F21C
        mov    esi, dword ptr [ebx + 2ch]
        test   byte ptr [esi + 4ch], 40h
        je     SHORT loc_1_F0E8
        mov    eax, dword ptr [ebx + 34h]
        mov    dword ptr [eax + 14h], 0
        mov    dword ptr [ebx + 30h], 0
        mov    eax, dword ptr [edi + 38h]
        and    dword ptr [eax + 20h], 0ffffff0fh
        mov    eax, dword ptr [edi + 38h]
        or     dword ptr [eax + 20h], 20h
        mov    eax, dword ptr [edi + 38h]
        mov    dword ptr [eax + 14h], 0
        push   dword ptr [edi + 38h]
        call   sub_1_DEA4
        add    esp, 4
        push   0c0000120h
        push   esi
        push   ebp
        call   sub_1_E64C
        add    esp, 0ch
        push   edi
        call   sub_1_F280
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_F0E8:
        mov    ecx, 1
        mov    ax, word ptr [ebx + 3eh]
        dec    ax
        mov    word ptr [ebx + 3eh], ax
        jne    NEAR PTR loc_1_F196
        mov    eax, dword ptr [edx + 80h]
        or     eax, eax
        je     SHORT loc_1_F134
        cmp    eax, 7f000000h
        je     SHORT loc_1_F134
        mov    ax, word ptr [edi + 4ch]
        test   ah, 1
        jne    SHORT loc_1_F134
        db  066h, 08Bh, 015h, 0D0h, 032h, 001h, 000h
        mov    word ptr [ebx + 3eh], dx
        and    ax, 0ff7fh
        mov    word ptr [edi + 4ch], ax
        or     ax, 100h
        mov    word ptr [edi + 4ch], ax
        jmp    SHORT loc_1_F196
loc_1_F134:
        mov    edx, dword ptr [edi + 3ch]
        db  039h, 015h, 033h, 033h, 001h, 000h
        jne    SHORT loc_1_F154
        db  0A1h, 037h, 033h, 001h, 000h
        mov    dword ptr [edi + 3ch], eax
        or     eax, eax
        je     SHORT loc_1_F150
        jmp    SHORT loc_1_F178
        db  08Dh, 049h, 000h
loc_1_F150:
        sub    ecx, ecx
        jmp    SHORT loc_1_F196
loc_1_F154:
        cmp    byte ptr [edx], 2ch
        je     SHORT loc_1_F169
loc_1_F159:
        mov    al, byte ptr [edx]
        cmp    al, 20h
        je     SHORT loc_1_F169
        or     al, al
        je     SHORT loc_1_F169
        inc    edx
        cmp    byte ptr [edx], 2ch
        jne    SHORT loc_1_F159
loc_1_F169:
        cmp    byte ptr [edx], 0
        jne    SHORT loc_1_F174
        sub    ecx, ecx
        jmp    SHORT loc_1_F196
        db  08Bh, 0FFh
loc_1_F174:
        inc    edx
        mov    dword ptr [edi + 3ch], edx
loc_1_F178:
        mov    ax, word ptr [edi + 4ch]
        and    ax, 0feffh
        mov    word ptr [edi + 4ch], ax
        or     ax, 80h
        mov    word ptr [edi + 4ch], ax
        db  066h, 0A1h, 0D0h, 032h, 001h, 000h
        mov    word ptr [ebx + 3eh], ax
loc_1_F196:
        or     ecx, ecx
        je     SHORT loc_1_F1D0
        inc    dword ptr [edi + 34h]
        push   1
        push   0
        push   0
        push   0
        push   0
        push   0
        push   edi
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        push   dword ptr [edi + 38h]
        call   sub_1_10C0C
        add    esp, 24h
        push   edi
        call   sub_1_F280
        add    esp, 4
        or     word ptr [ebx + 3ch], 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_F1D0:
        mov    eax, dword ptr [ebx + 34h]
        mov    dword ptr [eax + 14h], 0
        mov    dword ptr [ebx + 30h], 0
        mov    eax, dword ptr [edi + 38h]
        and    dword ptr [eax + 20h], 0ffffff0fh
        mov    eax, dword ptr [edi + 38h]
        or     dword ptr [eax + 20h], 20h
        mov    eax, dword ptr [edi + 38h]
        mov    dword ptr [eax + 14h], 0
        push   dword ptr [edi + 38h]
        call   sub_1_DEA4
        add    esp, 4
        push   15h
        push   esi
        push   ebp
        call   sub_1_E64C
        add    esp, 0ch
        push   edi
        call   sub_1_F280
        add    esp, 4
loc_1_F21C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_1_F224:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    edx, dword ptr [esi + 14h]
        cmp    dword ptr [edx + 28h], 0
        je     SHORT loc_1_F254
        db  00Fh, 0B7h, 005h, 064h, 033h, 001h, 000h
        push   eax
        db  068h, 024h, 0F2h, 000h, 000h
        push   esi
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        push   edx
        call   sub_1_EE34
        add    esp, 14h
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_F254:
        cmp    dword ptr [esi + 18h], 0
        jne    SHORT loc_1_F27D
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [edx + 8], 0
        push   edx
        call   sub_1_DEA4
        add    esp, 4
        push   esi
        call   sub_1_FA98
        add    esp, 4
loc_1_F27D:
        pop    esi
        ret
        db  090h
sub_1_F280:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 34h]
        dec    eax
        mov    dword ptr [ecx + 34h], eax
        jne    SHORT loc_1_F298
        push   4
        push   ecx
        call   sub_1_D5C0
        add    esp, 8
loc_1_F298:
        ret
        db  08Dh, 049h, 000h
sub_1_F29C:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 34h]
        dec    ecx
        mov    dword ptr [eax + 34h], ecx
        jne    SHORT loc_1_F2B4
        push   4
        push   eax
        call   sub_1_D5C0
        add    esp, 8
loc_1_F2B4:
        ret
        db  08Dh, 049h, 000h, 08Bh, 044h, 024h, 004h, 08Bh, 048h, 034h, 049h, 089h, 048h, 034h, 075h, 00Bh
        db  06Ah, 005h, 050h, 0E8h, 0F3h, 0E2h, 0FFh, 0FFh, 083h, 0C4h, 008h, 0C3h, 08Dh, 049h, 000h
sub_1_F2D4:
        push   esi
        cmp    dword ptr [esp + 0ch], 0
        jne    SHORT loc_1_F314
        lea    eax, [esp + 0ch]
        push   eax
        call   sub_1_101D0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_F2FC
        cmp    eax, 0ffh
        je     SHORT loc_1_F2FC
        mov    eax, 1
        pop    esi
        ret
        db  090h
loc_1_F2FC:
        mov    eax, dword ptr [esp + 0ch]
        mov    word ptr [eax + 4ch], 2
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax + 34h], 2
        jmp    SHORT loc_1_F31B
        db  090h
loc_1_F314:
        mov    eax, dword ptr [esp + 0ch]
        inc    dword ptr [eax + 34h]
loc_1_F31B:
        mov    eax, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        mov    dword ptr [eax + 38h], edx
        cmp    byte ptr [esp + 10h], 0
        je     SHORT loc_1_F343
        db  0A1h, 03Ch, 032h, 001h, 000h
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [ecx + 2ch], eax
        mov    eax, dword ptr [esp + 0ch]
        mov    word ptr [eax + 40h], 0
loc_1_F343:
        push   7
        db  0FFh, 035h, 0CCh, 032h, 001h, 000h
        db  00Fh, 0B7h, 005h, 0D0h, 032h, 001h, 000h
        push   eax
        db  068h, 054h, 0F4h, 000h, 000h
        push   dword ptr [esp + 1ch]
        db  068h, 08Ch, 0EDh, 000h, 000h
        push   dword ptr [esp + 24h]
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        push   edx
        call   sub_1_10C0C
        add    esp, 24h
        mov    esi, eax
        push   dword ptr [esp + 0ch]
        call   sub_1_F280
        add    esp, 4
        or     esi, esi
        je     SHORT loc_1_F3A0
        cmp    esi, 0ffh
        je     SHORT loc_1_F3A0
        push   dword ptr [esp + 0ch]
        call   sub_1_F29C
        add    esp, 4
        mov    eax, esi
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_F3A0:
        mov    eax, esi
        pop    esi
        ret
sub_1_F3A4:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  00Fh, 0B7h, 015h, 00Ah, 033h, 001h, 000h
        db  0A1h, 0B4h, 032h, 001h, 000h
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 10h], eax
        cmp    edx, eax
        jge    SHORT loc_1_F424
        db  0A1h, 0B4h, 032h, 001h, 000h
        lea    eax, [eax + edx*8 + 8]
        mov    dword ptr [esp + 14h], eax
        mov    esi, eax
        mov    ebx, dword ptr [esp + 1ch]
loc_1_F3D4:
        or     ebx, ebx
        je     SHORT loc_1_F3E8
        test   byte ptr [ebx + 20h], 10h
        je     SHORT loc_1_F3E8
        mov    ebp, dword ptr [esp + 14h]
        mov    edi, dword ptr [ebx]
        sub    ebx, ebx
        jmp    SHORT loc_1_F3EC
loc_1_F3E8:
        mov    ebp, esi
        mov    edi, dword ptr [esi]
loc_1_F3EC:
        cmp    edi, ebp
        je     SHORT loc_1_F41A
loc_1_F3F0:
        mov    eax, dword ptr [edi + 20h]
        test   al, 10h
        je     SHORT loc_1_F414
        cmp    byte ptr [edi + 34h], 2ah
        je     SHORT loc_1_F414
        test   al, 1
        jne    SHORT loc_1_F414
        mov    eax, 1
        db  08Ah, 00Dh, 04Fh, 033h, 001h, 000h
        shl    eax, cl
        dec    eax
        cmp    dword ptr [edi + 2ch], eax
        jne    SHORT loc_1_F438
loc_1_F414:
        mov    edi, dword ptr [edi]
        cmp    edi, ebp
        jne    SHORT loc_1_F3F0
loc_1_F41A:
        inc    edx
        add    esi, 8
        cmp    edx, dword ptr [esp + 10h]
        jl     SHORT loc_1_F3D4
loc_1_F424:
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [eax], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_F438:
        mov    eax, dword ptr [edi + 8]
        inc    dword ptr [eax + 18h]
        db  066h, 089h, 015h, 00Ah, 033h, 001h, 000h
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [eax], edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  090h
loc_1_F454:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax + 2ch]
        db  068h, 070h, 0F4h, 000h, 000h
        push   0
        push   dword ptr [esp + 14h]
        push   eax
        call   sub_1_3C00
        add    esp, 14h
        ret
loc_1_F470:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    cl, cl
        mov    eax, dword ptr [esp + 18h]
        mov    esi, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 0ch]
        mov    edi, dword ptr [esi + 38h]
        or     eax, eax
        jne    SHORT loc_1_F498
        mov    eax, dword ptr [esi + 2ch]
        mov    eax, dword ptr [eax + 88h]
        or     dword ptr [edi + 2ch], eax
        jmp    SHORT loc_1_F4AE
loc_1_F498:
        cmp    eax, 15h
        je     SHORT loc_1_F4B0
        mov    eax, dword ptr [edi + 20h]
        and    eax, 0ffffff0fh
        mov    dword ptr [edi + 20h], eax
        or     eax, 40h
        mov    dword ptr [edi + 20h], eax
loc_1_F4AE:
        mov    cl, 1
loc_1_F4B0:
        db  080h, 03Dh, 050h, 033h, 001h, 000h, 000h
        je     SHORT loc_1_F4BB
        mov    cl, 1
loc_1_F4BB:
        mov    eax, dword ptr [esi + 2ch]
        mov    ebx, dword ptr [eax + 88h]
        add    ebx, ebx
        mov    edx, dword ptr [edi + 28h]
        cmp    ebx, edx
        ja     SHORT loc_1_F50D
loc_1_F4CD:
        test   edx, ebx
        jne    SHORT loc_1_F4D7
loc_1_F4D1:
        add    ebx, ebx
        test   edx, ebx
        je     SHORT loc_1_F4D1
loc_1_F4D7:
        db  0A1h, 03Ch, 032h, 001h, 000h
        mov    ebp, 1
        cmp    ebx, ebp
        jbe    SHORT loc_1_F4ED
loc_1_F4E5:
        mov    eax, dword ptr [eax]
        add    ebp, ebp
        cmp    ebp, ebx
        jb     SHORT loc_1_F4E5
loc_1_F4ED:
        cmp    dword ptr [eax + 58h], 7f000000h
        je     SHORT loc_1_F4FF
        cmp    dword ptr [eax + 74h], 7f000000h
        jne    SHORT loc_1_F524
loc_1_F4FF:
        add    ebx, ebx
        cmp    ebx, edx
        jbe    SHORT loc_1_F4CD
        cmp    ebx, edx
        jbe    NEAR PTR loc_1_F5AA
loc_1_F50D:
        or     cl, cl
        je     SHORT loc_1_F548
        lea    eax, [esp + 10h]
        push   eax
        push   edi
        call   sub_1_F3A4
        add    esp, 8
        jmp    SHORT loc_1_F550
        db  08Dh, 049h, 000h
loc_1_F524:
        mov    dword ptr [esi + 2ch], eax
        mov    dword ptr [edi + 14h], 0
        push   0
        push   esi
        push   edi
        call   sub_1_F2D4
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_F5AA
        cmp    eax, 0ffh
        je     SHORT loc_1_F5AA
        jmp    SHORT loc_1_F59A
        db  090h
loc_1_F548:
        mov    dword ptr [esp + 10h], 0
loc_1_F550:
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_F581
        push   1
        push   esi
        push   dword ptr [esp + 18h]
        call   sub_1_F2D4
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_F581
        cmp    eax, 0ffh
        je     SHORT loc_1_F581
        db  0C6h, 005h, 056h, 033h, 001h, 000h, 000h
        push   esi
        call   sub_1_F280
        add    esp, 4
loc_1_F581:
        push   dword ptr [edi + 8]
        mov    dword ptr [edi + 14h], 0
        call   sub_1_DDE4
        add    esp, 4
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_1_F5AA
loc_1_F59A:
        push   esi
        call   sub_1_F280
        add    esp, 4
        db  0C6h, 005h, 056h, 033h, 001h, 000h, 000h
loc_1_F5AA:
        push   dword ptr [esp + 18h]
        call   sub_1_0024
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_F5C0:
        push   esi
        mov    esi, dword ptr [esp + 10h]
        or     esi, esi
        je     SHORT loc_1_F61E
        db  0F6h, 005h, 064h, 033h, 001h, 000h, 001h
        je     SHORT loc_1_F5E4
        mov    word ptr [esi + 3ch], 0
        db  0C7h, 005h, 0F0h, 032h, 001h, 000h, 000h, 000h, 000h, 000h
        pop    esi
        ret
loc_1_F5E4:
        db  080h, 03Dh, 056h, 033h, 001h, 000h, 000h
        jne    SHORT loc_1_F609
        db  0C6h, 005h, 056h, 033h, 001h, 000h, 001h
        push   0
        db  068h, 020h, 0F6h, 000h, 000h
        push   0
        push   0
        push   0
        call   sub_1_3C00
        add    esp, 14h
loc_1_F609:
        sub    edx, edx
        db  0A1h, 0E0h, 032h, 001h, 000h
        db  0F7h, 035h, 0E4h, 032h, 001h, 000h
        mov    dword ptr [esi + 38h], eax
        or     word ptr [esi + 3ch], 1
loc_1_F61E:
        pop    esi
        ret
loc_1_F620:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   dword ptr [esp + 28h]
        call   sub_1_0024
        add    esp, 4
        db  00Fh, 0B7h, 00Dh, 014h, 033h, 001h, 000h
        db  066h, 0FFh, 005h, 014h, 033h, 001h, 000h
        db  00Fh, 0B7h, 005h, 014h, 033h, 001h, 000h
        sub    edx, edx
        db  0F7h, 035h, 0E4h, 032h, 001h, 000h
        db  066h, 089h, 015h, 014h, 033h, 001h, 000h
        db  083h, 03Dh, 0E0h, 032h, 001h, 000h, 0FFh
        jne    SHORT loc_1_F670
        db  0C6h, 005h, 056h, 033h, 001h, 000h, 000h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  090h
loc_1_F670:
        db  0A1h, 0B4h, 032h, 001h, 000h
        mov    dword ptr [esp + 20h], eax
        db  0A1h, 0E4h, 032h, 001h, 000h
        shr    eax, 1
        cmp    eax, ecx
        je     SHORT loc_1_F68C
        or     ecx, ecx
        jne    NEAR PTR loc_1_F750
loc_1_F68C:
        db  0A1h, 0B4h, 032h, 001h, 000h
        mov    dword ptr [esp + 14h], 0
        mov    dword ptr [esp + 1ch], 0
        cmp    dword ptr [eax], 0
        jle    NEAR PTR loc_1_F750
        add    eax, 8
        mov    dword ptr [esp + 18h], eax
loc_1_F6B1:
        mov    eax, dword ptr [esp + 18h]
        mov    esi, dword ptr [eax]
        cmp    esi, eax
        je     SHORT loc_1_F737
loc_1_F6BC:
        mov    dword ptr [esp + 10h], esi
        mov    eax, dword ptr [esi + 20h]
        test   al, 10h
        je     SHORT loc_1_F72F
        cmp    byte ptr [esi + 34h], 2ah
        je     SHORT loc_1_F72F
        test   al, 1
        jne    SHORT loc_1_F72F
        db  08Bh, 02Dh, 03Ch, 032h, 001h, 000h
        sub    ebx, ebx
        db  081h, 0FDh, 03Ch, 032h, 001h, 000h
        je     SHORT loc_1_F724
loc_1_F6E1:
        mov    cl, bl
        mov    edi, 1
        shl    edi, cl
        inc    ebx
        mov    ecx, ebp
        mov    ebp, dword ptr [ebp]
        mov    eax, dword ptr [esp + 10h]
        test   dword ptr [eax + 28h], edi
        je     SHORT loc_1_F71C
        test   dword ptr [esp + 14h], edi
        jne    SHORT loc_1_F71C
        mov    eax, dword ptr [eax + 2ch]
        and    eax, edi
        je     SHORT loc_1_F70F
        cmp    byte ptr [ecx + 93h], 0
        je     SHORT loc_1_F71C
loc_1_F70F:
        push   ecx
        call   sub_1_A08C
        add    esp, 4
        or     dword ptr [esp + 14h], edi
loc_1_F71C:
        db  081h, 0FDh, 03Ch, 032h, 001h, 000h
        jne    SHORT loc_1_F6E1
loc_1_F724:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 2ch], 0
loc_1_F72F:
        mov    esi, dword ptr [esi]
        cmp    esi, dword ptr [esp + 18h]
        jne    SHORT loc_1_F6BC
loc_1_F737:
        inc    dword ptr [esp + 1ch]
        add    dword ptr [esp + 18h], 8
        mov    eax, dword ptr [esp + 20h]
        mov    ecx, dword ptr [esp + 1ch]
        cmp    dword ptr [eax], ecx
        jg     NEAR PTR loc_1_F6B1
loc_1_F750:
        lea    eax, [esp + 10h]
        db  066h, 0C7h, 005h, 00Ah, 033h, 001h, 000h, 000h, 000h
        push   eax
        push   0
        call   sub_1_F3A4
        add    esp, 8
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_F7A8
        push   1
        push   0
        push   dword ptr [esp + 18h]
        call   sub_1_F2D4
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_F7AF
        cmp    eax, 0ffh
        je     SHORT loc_1_F7AF
        mov    eax, dword ptr [esp + 10h]
        push   dword ptr [eax + 8]
        call   sub_1_DDE4
        add    esp, 4
        db  0C6h, 005h, 056h, 033h, 001h, 000h, 000h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
loc_1_F7A8:
        db  0C6h, 005h, 056h, 033h, 001h, 000h, 000h
loc_1_F7AF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  090h
sub_1_F7B8:
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_1_F832
        db  08Bh, 03Dh, 0B8h, 032h, 001h, 000h
        sub    ebx, ebx
        mov    eax, edi
        cmp    dword ptr [eax], ebx
        jle    SHORT loc_1_F829
        mov    esi, edi
        add    esi, 8
loc_1_F7D6:
        mov    ebp, dword ptr [esi]
        cmp    ebp, esi
        je     SHORT loc_1_F821
loc_1_F7DC:
        mov    eax, ebp
        mov    ebp, dword ptr [ebp]
        mov    dx, word ptr [eax + 30h]
        or     dx, dx
        jne    SHORT loc_1_F804
        test   byte ptr [eax + 20h], 30h
        je     SHORT loc_1_F804
        cmp    dword ptr [eax + 1ch], 1
        ja     SHORT loc_1_F804
        push   eax
        call   sub_1_DEA4
        add    esp, 4
        jmp    SHORT loc_1_F81D
        db  08Dh, 049h, 000h
loc_1_F804:
        mov    ecx, dword ptr [eax + 20h]
        test   ch, 10h
        jne    SHORT loc_1_F81D
        cmp    dword ptr [eax + 1ch], 1
        ja     SHORT loc_1_F81D
        test   cl, 30h
        je     SHORT loc_1_F81D
        dec    dx
        mov    word ptr [eax + 30h], dx
loc_1_F81D:
        cmp    ebp, esi
        jne    SHORT loc_1_F7DC
loc_1_F821:
        inc    ebx
        add    esi, 8
        cmp    dword ptr [edi], ebx
        jg     SHORT loc_1_F7D6
loc_1_F829:
        mov    eax, dword ptr [esp + 1ch]
        or     word ptr [eax + 3ch], 1
loc_1_F832:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_F838:
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        mov    eax, dword ptr [esi + 2ch]
        mov    dword ptr [esp + 0ch], eax
        mov    edi, dword ptr [esi + 14h]
        lea    eax, [esp + 8]
        push   eax
        push   edi
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [esp + 18h]
        call   sub_1_F8AC
        add    esp, 10h
        push   edi
        call   sub_1_DF20
        add    esp, 4
        cmp    dword ptr [esp + 8], 0
        je     SHORT loc_1_F898
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [esi + 2ch], eax
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [esi + 14h], eax
        db  068h, 038h, 0F8h, 000h, 000h
        push   dword ptr [esp + 0ch]
        push   esi
        call   sub_1_11858
        add    esp, 0ch
        pop    edi
        pop    esi
        add    esp, 8
        ret
loc_1_F898:
        push   5
        push   esi
        call   sub_1_D5C0
        add    esp, 8
        pop    edi
        pop    esi
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
sub_1_F8AC:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 10h]
        db  081h, 0FBh, 03Ch, 032h, 001h, 000h
        je     SHORT loc_1_F916
        inc    ecx
        mov    ecx, dword ptr [esp + 18h]
loc_1_F8C0:
        mov    esi, ebx
        mov    ebx, dword ptr [ebx]
        lea    edi, [esi + 14h]
        or     ecx, ecx
        je     SHORT loc_1_F8E0
        mov    eax, dword ptr [ecx + 38h]
        cmp    eax, 7
        je     SHORT loc_1_F8D8
        cmp    eax, 4
        jne    SHORT loc_1_F8E0
loc_1_F8D8:
        mov    edx, dword ptr [ecx]
        sub    ecx, ecx
        jmp    SHORT loc_1_F8E2
        db  08Bh, 0FFh
loc_1_F8E0:
        mov    edx, dword ptr [edi]
loc_1_F8E2:
        cmp    edx, edi
        je     SHORT loc_1_F90E
loc_1_F8E6:
        mov    eax, dword ptr [edx + 38h]
        cmp    eax, 7
        je     SHORT loc_1_F902
        cmp    eax, 4
        je     SHORT loc_1_F8F8
        jmp    SHORT loc_1_F908
        db  08Dh, 049h, 000h
loc_1_F8F8:
        cmp    eax, 7
        je     SHORT loc_1_F902
        cmp    eax, 4
        jne    SHORT loc_1_F90E
loc_1_F902:
        cmp    dword ptr [edx + 3ch], 3
        jl     SHORT loc_1_F924
loc_1_F908:
        mov    edx, dword ptr [edx]
        cmp    edx, edi
        jne    SHORT loc_1_F8E6
loc_1_F90E:
        db  081h, 0FBh, 03Ch, 032h, 001h, 000h
        jne    SHORT loc_1_F8C0
loc_1_F916:
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [eax], 0
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_F924:
        inc    dword ptr [edx + 3ch]
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax], esi
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_F938:
        push   esi
        mov    esi, dword ptr [esp + 10h]
        or     esi, esi
        je     SHORT loc_1_F95B
        push   0
        db  068h, 060h, 0F9h, 000h, 000h
        push   0
        push   0
        push   0
        call   sub_1_3C00
        add    esp, 14h
        or     word ptr [esi + 3ch], 1
loc_1_F95B:
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_F960:
        sub    esp, 0ch
        push   esi
        db  0A1h, 03Ch, 032h, 001h, 000h
        mov    dword ptr [esp + 0ch], eax
        lea    eax, [esp + 8]
        push   eax
        push   0
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [esp + 18h]
        call   sub_1_F8AC
        add    esp, 10h
        cmp    dword ptr [esp + 8], 0
        je     NEAR PTR loc_1_FA2C
        push   5
        call   sub_1_001E
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_1_FA38
        lea    eax, [esp + 4]
        push   eax
        call   sub_1_101D0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_F9CC
        cmp    eax, 0ffh
        je     SHORT loc_1_F9CC
        push   esi
        call   sub_1_0024
        add    esp, 4
        pop    esi
        add    esp, 0ch
        ret
        db  08Bh, 0FFh
loc_1_F9CC:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 1ch], esi
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 18h], 4
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 24h], 0
        mov    byte ptr [esi + 1], 0
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx + 2ch], eax
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx + 14h], eax
        mov    byte ptr [esi], 85h
        mov    word ptr [esi + 2], 0
        db  068h, 038h, 0F8h, 000h, 000h
        push   dword ptr [esp + 0ch]
        push   dword ptr [esp + 0ch]
        call   sub_1_11858
        add    esp, 0ch
loc_1_FA2C:
        push   dword ptr [esp + 14h]
        call   sub_1_0024
        add    esp, 4
loc_1_FA38:
        pop    esi
        add    esp, 0ch
        ret
        db  08Dh, 049h, 000h
sub_1_FA40:
        sub    esp, 4
        cmp    byte ptr [esp + 0ch], 0
        je     SHORT loc_1_FA4F
        add    dword ptr [esp + 8], 2
loc_1_FA4F:
        mov    eax, dword ptr [esp + 8]
        db  08Dh, 004h, 085h, 0F8h, 033h, 001h, 000h
        mov    dword ptr [esp], eax
        inc    dword ptr [eax]
        add    esp, 4
        ret
sub_1_FA64:
        push   esi
        push   edi
        db  0A1h, 008h, 034h, 001h, 000h
        shl    eax, 4
        db  08Dh, 0B8h, 00Ch, 034h, 001h, 000h
        mov    esi, dword ptr [esp + 0ch]
        movsd
        movsd
        movsd
        movsd
        db  0FFh, 005h, 008h, 034h, 001h, 000h
        db  083h, 03Dh, 008h, 034h, 001h, 000h, 008h
        jb     SHORT loc_1_FA95
        db  0C7h, 005h, 008h, 034h, 001h, 000h, 000h, 000h, 000h, 000h
loc_1_FA95:
        pop    edi
        pop    esi
        ret
sub_1_FA98:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax + 1ch]
        db  068h, 0B4h, 0FAh, 000h, 000h
        push   0
        push   eax
        push   0
        call   sub_1_3C00
        add    esp, 14h
        ret
        db  08Bh, 0FFh
loc_1_FAB4:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0ch]
        or     eax, eax
        je     SHORT loc_1_FACD
        add    dword ptr [eax + 8], 0ah
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_FACD:
        push   esi
        call   sub_1_0024
        add    esp, 4
        pop    esi
        ret
sub_1_FAD8:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        je     SHORT loc_1_FAED
        add    dword ptr [eax + 8], 0ah
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_FAED:
        ret
        db  08Bh, 0FFh
sub_1_FAF0:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        je     SHORT loc_1_FB05
        add    dword ptr [eax + 8], 0ah
        push   eax
        call   sub_1_0024
        add    esp, 4
loc_1_FB05:
        ret
        db  08Bh, 0FFh
sub_1_FB08:
        push   edi
        push   8ah
        call   sub_1_35F0
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        jne    SHORT loc_1_FB20
        sub    eax, eax
        pop    edi
        ret
loc_1_FB20:
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 22h
        rep stosd
        stosw
        mov    dword ptr [edx + 48h], eax
        mov    dword ptr [edx + 50h], eax
        mov    dword ptr [edx + 54h], eax
        mov    dword ptr [edx + 58h], eax
        mov    dword ptr [edx + 60h], eax
        mov    dword ptr [edx + 68h], eax
        mov    dword ptr [edx + 70h], eax
        mov    dword ptr [edx + 78h], eax
        mov    dword ptr [edx + 84h], 1
        mov    byte ptr [edx + 88h], 10h
        lea    eax, [edx + 24h]
        mov    dword ptr [edx + 28h], eax
        mov    dword ptr [eax], eax
        lea    eax, [edx + 2ch]
        mov    dword ptr [edx + 30h], eax
        mov    dword ptr [eax], eax
        lea    eax, [edx + 34h]
        mov    dword ptr [edx + 38h], eax
        mov    dword ptr [eax], eax
        lea    eax, [edx + 1ch]
        mov    dword ptr [edx + 20h], eax
        mov    dword ptr [eax], eax
        lea    eax, [edx + 14h]
        mov    dword ptr [edx + 18h], eax
        mov    dword ptr [eax], eax
        lea    eax, [edx + 3ch]
        mov    dword ptr [edx + 40h], eax
        mov    dword ptr [eax], eax
        mov    dword ptr [edx + 44h], 0
        mov    dword ptr [edx + 0ch], 0
        mov    dword ptr [edx + 8], 18273645h
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [edx + 10h], ecx
        add    ecx, 0ch
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx], ecx
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 4], edx
        mov    eax, edx
        pop    edi
        ret
        db  08Bh, 0FFh
sub_1_FBB8:
        sub    esp, 38h
        push   ebx
        push   esi
        push   edi
        lea    edi, [esp + 10h]
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        mov    ebx, dword ptr [esp + 48h]
        lea    esi, [ebx + 8ch]
        lea    edi, [esp + 1ah]
        movsd
        movsw
        mov    eax, dword ptr [ebx + 84h]
        or     eax, eax
        je     SHORT loc_1_FC15
        push   10h
        lea    ecx, [esp + 14h]
        push   ecx
        mov    eax, dword ptr [eax + 10h]
        mov    eax, dword ptr [eax + 14h]
        add    eax, 34h
        push   eax
        call   sub_1_0262
        add    esp, 0ch
        cmp    eax, 10h
        jne    SHORT loc_1_FC0C
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
        db  08Bh, 0FFh
loc_1_FC0C:
        push   ebx
        call   sub_1_FCF4
        add    esp, 4
loc_1_FC15:
        lea    eax, [esp + 0ch]
        push   eax
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        lea    eax, [esp + 18h]
        push   eax
        db  0A1h, 080h, 02Fh, 001h, 000h
        push   dword ptr [eax + 80h]
        call   sub_1_6954
        add    esp, 10h
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_FC84
        cmp    esi, 0ffh
        je     SHORT loc_1_FC84
        mov    dword ptr [esp + 20h], 0
        mov    word ptr [esp + 30h], 2
        lea    esi, [esp + 10h]
        lea    edi, [esp + 32h]
        movsd
        movsd
        movsd
        movsd
        push   0
        push   ebx
        push   0
        push   dword ptr [ebx + 58h]
        lea    eax, [esp + 40h]
        push   eax
        lea    eax, [esp + 34h]
        push   eax
        call   sub_1_A19C
        add    esp, 18h
        mov    esi, eax
        mov    eax, dword ptr [esp + 20h]
        jmp    SHORT loc_1_FCD3
        db  090h
loc_1_FC84:
        mov    eax, dword ptr [esp + 0ch]
        push   dword ptr [eax + 8]
        call   sub_1_FB08
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_FCA4
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
        db  090h
loc_1_FCA4:
        mov    ecx, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [ecx + 8]
        inc    dword ptr [ecx + 18h]
        mov    ecx, dword ptr [ebx + 58h]
        mov    edx, dword ptr [esp + 0ch]
        mov    dword ptr [edx + 0ch], ecx
        mov    dword ptr [eax + 80h], ebx
        mov    ecx, dword ptr [esp + 0ch]
        mov    edx, dword ptr [ebx + 88h]
        or     dword ptr [ecx + 28h], edx
        mov    ecx, dword ptr [esp + 0ch]
        or     dword ptr [ecx + 20h], 1
loc_1_FCD3:
        or     esi, esi
        je     SHORT loc_1_FCDF
        cmp    esi, 0ffh
        jne    SHORT loc_1_FCEA
loc_1_FCDF:
        mov    dword ptr [ebx + 84h], eax
        mov    ecx, dword ptr [ebx + 44h]
        mov    dword ptr [ecx], eax
loc_1_FCEA:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
        db  090h
sub_1_FCF4:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 84h]
        or     ecx, ecx
        je     SHORT loc_1_FD1E
        mov    dword ptr [eax + 84h], 0
        mov    eax, dword ptr [eax + 44h]
        mov    dword ptr [eax], 0
        push   ecx
        call   sub_1_DD58
        add    esp, 4
loc_1_FD1E:
        ret
        db  090h
sub_1_FD20:
        sub    esp, 4
        push   ebx
        push   esi
        sub    edx, edx
        sub    ecx, ecx
        mov    byte ptr [esp + 8], cl
        mov    eax, dword ptr [esp + 10h]
        mov    bl, byte ptr [eax]
        inc    eax
        cmp    bl, cl
        je     SHORT loc_1_FD78
loc_1_FD38:
        cmp    bl, 20h
        je     SHORT loc_1_FD78
        cmp    bl, 2eh
        jne    SHORT loc_1_FD60
        cmp    edx, 0ffh
        jg     SHORT loc_1_FDC0
        mov    byte ptr [esp + ecx + 8], dl
        inc    ecx
        cmp    ecx, 3
        ja     SHORT loc_1_FDB4
        mov    byte ptr [esp + ecx + 8], 0
        sub    edx, edx
        jmp    SHORT loc_1_FD71
        db  08Dh, 049h, 000h
loc_1_FD60:
        sub    bl, 30h
        cmp    bl, 9
        ja     SHORT loc_1_FDCC
        movzx  esi, bl
        lea    edx, [edx + edx*4]
        lea    edx, [esi + edx*2]
loc_1_FD71:
        mov    bl, byte ptr [eax]
        inc    eax
        or     bl, bl
        jne    SHORT loc_1_FD38
loc_1_FD78:
        cmp    edx, 0ffh
        jg     SHORT loc_1_FDD8
        cmp    ecx, 3
        jne    SHORT loc_1_FDD8
        mov    byte ptr [esp + ecx + 8], dl
        sub    edx, edx
        sub    cx, cx
loc_1_FD8E:
        movzx  eax, cx
        movzx  eax, byte ptr [esp + eax + 8]
        shl    edx, 8
        add    edx, eax
        inc    cx
        cmp    cx, 4
        jb     SHORT loc_1_FD8E
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax], edx
        sub    eax, eax
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_FDB4:
        pop    esi
        mov    eax, 0c0000001h
        pop    ebx
        add    esp, 4
        ret
        db  090h
loc_1_FDC0:
        pop    esi
        mov    eax, 0c0000001h
        pop    ebx
        add    esp, 4
        ret
        db  090h
loc_1_FDCC:
        pop    esi
        mov    eax, 0c0000001h
        pop    ebx
        add    esp, 4
        ret
        db  090h
loc_1_FDD8:
        pop    esi
        mov    eax, 0c0000001h
        pop    ebx
        add    esp, 4
        ret
        db  090h
sub_1_FDE4:
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebp, ebp
        mov    ebx, dword ptr [esp + 1ch]
        cmp    ebx, ebp
        jle    SHORT loc_1_FE1A
        mov    si, word ptr [esp + 18h]
        inc    edi
        mov    edi, dword ptr [esp + 14h]
loc_1_FDFC:
        push   esi
        call   sub_1_35F0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_FE24
        mov    ecx, dword ptr [edi]
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [edi], eax
        inc    ebp
        cmp    ebp, ebx
        jl     SHORT loc_1_FDFC
loc_1_FE1A:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_FE24:
        pop    ebp
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
sub_1_FE30:
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        mov    edi, dword ptr [esp + 14h]
        cmp    edi, ebx
        jle    SHORT loc_1_FE62
        mov    esi, dword ptr [esp + 10h]
loc_1_FE41:
        call   sub_1_FE74
        or     eax, eax
        je     SHORT loc_1_FE68
        db  066h, 0FFh, 005h, 044h, 032h, 001h, 000h
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], esi
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [esi], eax
        inc    ebx
        cmp    edi, ebx
        jg     SHORT loc_1_FE41
loc_1_FE62:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_FE68:
        pop    edi
        mov    eax, 1
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_1_FE74:
        push   esi
        push   edi
        db  00Fh, 0B7h, 035h, 0D6h, 032h, 001h, 000h
        add    esi, 76h
        push   esi
        call   sub_1_35F0
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        je     SHORT loc_1_FEEE
        mov    edi, edx
        sub    eax, eax
        mov    ecx, esi
        shr    ecx, 2
        rep stosd
        mov    ecx, esi
        and    ecx, 3
        rep stosb
        lea    ecx, [edx + 56h]
        mov    dword ptr [edx + 28h], ecx
        db  0A1h, 080h, 02Fh, 001h, 000h
        movzx  eax, word ptr [eax + 0a2h]
        add    eax, 8
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [edx + 28h]
        lea    ecx, [eax + 18h]
        mov    dword ptr [ecx], 1
        db  066h, 08Bh, 035h, 0D6h, 032h, 001h, 000h
        mov    word ptr [ecx + 4], si
        mov    word ptr [ecx + 6], 2
        mov    word ptr [ecx + 8], 89h
        mov    dword ptr [ecx + 0ah], 0
        mov    dword ptr [eax + 14h], ecx
        lea    eax, [edx + 8]
        mov    dword ptr [edx + 0ch], eax
        mov    dword ptr [eax], eax
loc_1_FEEE:
        mov    eax, edx
        pop    edi
        pop    esi
        ret
        db  090h
sub_1_FEF4:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esi]
        cmp    esi, eax
        jne    SHORT loc_1_FF58
        mov    edi, dword ptr [esp + 14h]
        db  0A1h, 080h, 02Fh, 001h, 000h
        db  066h, 08Bh, 00Ch, 07Dh, 044h, 032h, 001h, 000h
        cmp    word ptr [eax + edi*2 + 1ah], cx
        ja     SHORT loc_1_FF20
        mov    eax, 1
        pop    edi
        pop    esi
        ret
loc_1_FF20:
        push   1
        movzx  eax, word ptr [eax + edi*2 + 24h]
        push   eax
        push   esi
        call   sub_1_FDE4
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_FF3C
        cmp    eax, 0ffh
        jne    SHORT loc_1_FF6C
loc_1_FF3C:
        mov    ecx, dword ptr [esp + 10h]
        db  066h, 0FFh, 004h, 07Dh, 044h, 032h, 001h, 000h
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        jmp    SHORT loc_1_FF5E
loc_1_FF58:
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx], eax
loc_1_FF5E:
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        sub    eax, eax
loc_1_FF6C:
        pop    edi
        pop    esi
        ret
        db  090h
sub_1_FF70:
        mov    eax, dword ptr [esp + 4]
        cmp    word ptr [eax + 6], 11h
        je     SHORT loc_1_FF84
        mov    eax, 0c000000dh
        ret
        db  08Dh, 049h, 000h
loc_1_FF84:
        cmp    word ptr [eax + 4], 12h
        je     SHORT loc_1_FF94
        mov    eax, 0c000000dh
        ret
        db  08Dh, 049h, 000h
loc_1_FF94:
        lea    edx, [eax + 0ah]
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        movzx  ecx, word ptr [eax + 8]
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], ecx
        sub    eax, eax
        ret
        db  08Bh, 0FFh
sub_1_FFAC:
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 10h]
        cmp    byte ptr [edx], 20h
        jne    NEAR PTR loc_1_10058
        mov    esi, dword ptr [esp + 14h]
        cmp    esi, 20h
        jle    NEAR PTR loc_1_10058
        inc    edx
        inc    edi
        mov    edi, 8
loc_1_FFD0:
        mov    ecx, dword ptr [edx]
        sub    ecx, 41414141h
        add    edx, 4
        mov    ebx, ecx
        and    ebx, 0f0000h
        shr    ebx, 4
        mov    eax, ecx
        and    eax, 0f00h
        shr    eax, 8
        add    ebx, eax
        mov    eax, ecx
        and    eax, 0fh
        shl    eax, 4
        add    ebx, eax
        and    ecx, 0f000000h
        shr    ecx, 10h
        lea    eax, [ebx + ecx]
        mov    ecx, dword ptr [esp + 18h]
        mov    word ptr [ecx], ax
        add    dword ptr [esp + 18h], 2
        dec    edi
        jne    SHORT loc_1_FFD0
        sub    ecx, ecx
        mov    eax, edx
        sub    esi, 21h
        cmp    esi, ecx
        jle    SHORT loc_1_1002D
loc_1_10022:
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_1002D
        inc    eax
        inc    ecx
        cmp    ecx, esi
        jl     SHORT loc_1_10022
loc_1_1002D:
        inc    ecx
        cmp    ecx, 0efh
        jle    SHORT loc_1_10040
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_10040:
        add    ecx, 20h
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [eax], ecx
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_10058:
        pop    edi
        mov    eax, 0c0000001h
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_1_10064:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [eax], 20h
        inc    eax
        mov    esi, 10h
        mov    ecx, dword ptr [esp + 14h]
loc_1_10078:
        mov    dl, byte ptr [ecx]
        mov    bl, dl
        shr    bl, 4
        add    bl, 41h
        mov    byte ptr [eax], bl
        inc    eax
        and    dl, 0fh
        add    dl, 41h
        mov    byte ptr [eax], dl
        inc    ecx
        inc    eax
        dec    esi
        jne    SHORT loc_1_10078
        mov    edx, dword ptr [esp + 1ch]
        cmp    edx, 1
        jbe    SHORT loc_1_100B8
        mov    edi, eax
        mov    esi, dword ptr [esp + 18h]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        add    eax, edx
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_100B8:
        pop    edi
        mov    byte ptr [eax], 0
        inc    eax
        pop    esi
        pop    ebx
        ret
sub_1_100C0:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 10h]
        mov    esi, eax
        inc    eax
        sub    ebx, ebx
        mov    ecx, dword ptr [esp + 14h]
loc_1_100D0:
        mov    dl, byte ptr [ecx]
        cmp    dl, 20h
        je     SHORT loc_1_100E1
        mov    byte ptr [eax], dl
        inc    ecx
        inc    eax
        inc    ebx
        cmp    ebx, 10h
        jl     SHORT loc_1_100D0
loc_1_100E1:
        mov    byte ptr [esi], bl
        mov    esi, dword ptr [esp + 18h]
        or     esi, esi
        je     SHORT loc_1_10126
        mov    edi, 1
loc_1_100F0:
        sub    ebx, ebx
        mov    ecx, eax
        inc    eax
        cmp    byte ptr [esi], 2eh
        je     SHORT loc_1_1010F
loc_1_100FA:
        mov    dl, byte ptr [esi]
        or     dl, dl
        je     SHORT loc_1_1010F
        cmp    dl, 2ch
        je     SHORT loc_1_1010F
        mov    byte ptr [eax], dl
        inc    esi
        inc    eax
        inc    ebx
        cmp    byte ptr [esi], 2eh
        jne    SHORT loc_1_100FA
loc_1_1010F:
        mov    byte ptr [ecx], bl
        mov    cl, byte ptr [esi]
        or     cl, cl
        je     SHORT loc_1_10120
        cmp    cl, 2ch
        je     SHORT loc_1_10120
        inc    esi
        jmp    SHORT loc_1_10122
        db  090h
loc_1_10120:
        sub    edi, edi
loc_1_10122:
        or     edi, edi
        jne    SHORT loc_1_100F0
loc_1_10126:
        pop    edi
        mov    byte ptr [eax], 0
        inc    eax
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
sub_1_10130:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 14h]
        movsx  esi, byte ptr [ebx]
        inc    ebx
        mov    edi, dword ptr [esp + 1ch]
        mov    edx, edi
        sub    ecx, ecx
        cmp    esi, ecx
        je     SHORT loc_1_10158
loc_1_10148:
        mov    al, byte ptr [ebx]
        mov    byte ptr [edi], al
        inc    ebx
        cmp    ecx, 10h
        jge    SHORT loc_1_10153
        inc    edi
loc_1_10153:
        inc    ecx
        cmp    esi, ecx
        ja     SHORT loc_1_10148
loc_1_10158:
        cmp    esi, 10h
        jge    SHORT loc_1_10177
        mov    ebp, 10h
        sub    ebp, esi
        mov    eax, 20202020h
        mov    ecx, ebp
        shr    ecx, 2
        rep stosd
        mov    ecx, ebp
        and    ecx, 3
        rep stosb
loc_1_10177:
        mov    ecx, 10h
loc_1_1017C:
        mov    al, byte ptr [edx]
        cmp    al, 61h
        jl     SHORT loc_1_1018A
        cmp    al, 7ah
        jg     SHORT loc_1_1018A
        sub    al, 20h
        mov    byte ptr [edx], al
loc_1_1018A:
        inc    edx
        dec    ecx
        jne    SHORT loc_1_1017C
        cmp    byte ptr [ebx], 0
        je     SHORT loc_1_101A3
        inc    eax
        mov    eax, dword ptr [esp + 18h]
loc_1_10198:
        cmp    esi, eax
        jae    SHORT loc_1_101A3
        inc    ebx
        inc    esi
        cmp    byte ptr [ebx], 0
        jne    SHORT loc_1_10198
loc_1_101A3:
        inc    esi
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [eax], esi
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_1_101B4:
        sub    eax, eax
        mov    ecx, dword ptr [esp + 4]
        cmp    ecx, eax
        je     SHORT loc_1_101CE
        cmp    byte ptr [ecx], al
        je     SHORT loc_1_101CE
loc_1_101C2:
        cmp    byte ptr [ecx], 2ch
        je     SHORT loc_1_101CE
        inc    ecx
        inc    eax
        cmp    byte ptr [ecx], 0
        jne    SHORT loc_1_101C2
loc_1_101CE:
        ret
        db  090h
sub_1_101D0:
        push   esi
        db  081h, 03Dh, 064h, 032h, 001h, 000h, 064h, 032h, 001h, 000h
        je     SHORT loc_1_10218
        db  08Bh, 015h, 064h, 032h, 001h, 000h
        mov    eax, edx
        mov    ecx, dword ptr [eax]
        mov    eax, edx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        lea    eax, [edx + 8]
        mov    dword ptr [edx + 0ch], eax
        mov    dword ptr [eax], eax
        mov    dword ptr [edx + 30h], 0
        mov    dword ptr [edx + 10h], 0
        mov    word ptr [edx + 40h], 0
        mov    word ptr [edx + 4ch], 0
        jmp    SHORT loc_1_10252
        db  08Dh, 049h, 000h
loc_1_10218:
        db  066h, 0A1h, 044h, 032h, 001h, 000h
        db  066h, 039h, 005h, 04Eh, 032h, 001h, 000h
        ja     SHORT loc_1_10230
        mov    eax, 1
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_10230:
        call   sub_1_FE74
        mov    edx, eax
        or     edx, edx
        jne    SHORT loc_1_10244
        mov    eax, 1
        jmp    SHORT loc_1_10254
        db  08Bh, 0FFh
loc_1_10244:
        db  066h, 0FFh, 005h, 044h, 032h, 001h, 000h
        mov    dword ptr [edx + 30h], 0
loc_1_10252:
        sub    eax, eax
loc_1_10254:
        or     eax, eax
        je     SHORT loc_1_1025F
        cmp    eax, 0ffh
        jne    SHORT loc_1_1027F
loc_1_1025F:
        lea    esi, [edx + 4eh]
        db  0C7h, 006h, 080h, 033h, 001h, 000h
        db  08Bh, 00Dh, 084h, 033h, 001h, 000h
        mov    dword ptr [edx + 52h], ecx
        db  08Bh, 00Dh, 084h, 033h, 001h, 000h
        mov    dword ptr [ecx], esi
        db  089h, 035h, 084h, 033h, 001h, 000h
loc_1_1027F:
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        pop    esi
        ret
        db  090h
sub_1_10288:
        push   esi
        sub    eax, eax
        db  08Bh, 00Dh, 0B4h, 032h, 001h, 000h
        mov    ecx, dword ptr [ecx]
        cmp    ecx, eax
        jle    SHORT loc_1_102BF
        db  08Bh, 015h, 0B4h, 032h, 001h, 000h
        add    edx, 8
loc_1_102A0:
        mov    esi, dword ptr [edx]
        cmp    esi, edx
        je     SHORT loc_1_102B9
loc_1_102A6:
        test   byte ptr [esi + 20h], 80h
        jne    SHORT loc_1_102B3
        cmp    byte ptr [esi + 34h], 2ah
        je     SHORT loc_1_102B3
        inc    eax
loc_1_102B3:
        mov    esi, dword ptr [esi]
        cmp    esi, edx
        jne    SHORT loc_1_102A6
loc_1_102B9:
        add    edx, 8
        dec    ecx
        jne    SHORT loc_1_102A0
loc_1_102BF:
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_1_102C4:
        push   ebx
        push   esi
        push   edi
        sub    eax, eax
        db  081h, 03Dh, 074h, 032h, 001h, 000h, 074h, 032h, 001h, 000h
        je     SHORT loc_1_10305
        inc    ecx
        db  08Bh, 00Dh, 074h, 032h, 001h, 000h
loc_1_102DC:
        lea    edx, [ecx + 0ch]
        mov    edi, dword ptr [edx]
        cmp    edi, edx
        je     SHORT loc_1_102FB
loc_1_102E5:
        lea    esi, [edi + 14h]
        mov    ebx, dword ptr [esi]
        cmp    ebx, esi
        je     SHORT loc_1_102F5
loc_1_102EE:
        inc    eax
        mov    ebx, dword ptr [ebx]
        cmp    ebx, esi
        jne    SHORT loc_1_102EE
loc_1_102F5:
        mov    edi, dword ptr [edi]
        cmp    edi, edx
        jne    SHORT loc_1_102E5
loc_1_102FB:
        mov    ecx, dword ptr [ecx]
        db  081h, 0F9h, 074h, 032h, 001h, 000h
        jne    SHORT loc_1_102DC
loc_1_10305:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_1_1030C:
        sub    esp, 8
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 14h]
        lea    esi, [edi + 1ch]
        mov    edx, dword ptr [esi]
        mov    eax, edx
        sub    eax, edi
        cmp    eax, 1ch
        mov    ecx, dword ptr [esp + 18h]
        je     SHORT loc_1_1033C
        mov    dword ptr [ecx], edx
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [edx + 4], ecx
        mov    dword ptr [eax], ecx
        mov    dword ptr [edi + 20h], esi
        mov    dword ptr [esi], esi
        jmp    SHORT loc_1_10341
        db  090h
loc_1_1033C:
        mov    dword ptr [ecx], ecx
        mov    dword ptr [ecx + 4], ecx
loc_1_10341:
        push   dword ptr [esp + 8]
        push   dword ptr [esp + 10h]
        push   edi
        call   sub_1_103A8
        add    esp, 0ch
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 8
        ret
        db  08Bh, 0FFh, 08Bh, 044h, 024h, 008h, 08Bh, 008h, 08Bh, 054h, 024h, 004h, 089h, 04Ah, 01Ch, 08Bh
        db  040h, 004h, 089h, 042h, 020h, 02Bh, 0C0h, 0C3h, 08Bh, 0FFh
sub_1_10374:
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        mov    esi, dword ptr [esp + 10h]
        mov    ebx, dword ptr [esi]
        cmp    ebx, esi
        je     SHORT loc_1_103A0
loc_1_10384:
        mov    edx, ebx
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    ebx, eax
        inc    edi
        push   edx
        call   sub_1_DF20
        add    esp, 4
        cmp    ebx, esi
        jne    SHORT loc_1_10384
loc_1_103A0:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
sub_1_103A8:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ecx, ecx
        mov    edi, dword ptr [esp + 20h]
        add    edi, 14h
        mov    eax, dword ptr [edi]
        cmp    eax, edi
        je     SHORT loc_1_103C9
loc_1_103BE:
        inc    ecx
        mov    byte ptr [eax + 47h], 1
        mov    eax, dword ptr [eax]
        cmp    eax, edi
        jne    SHORT loc_1_103BE
loc_1_103C9:
        or     ecx, ecx
        je     NEAR PTR loc_1_104E4
        lea    eax, [ecx*4]
        push   eax
        call   sub_1_001E
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_1_104E4
        mov    eax, dword ptr [edi]
        mov    dword ptr [esp + 10h], 0
        cmp    eax, edi
        je     SHORT loc_1_1040B
loc_1_103F9:
        mov    dword ptr [esi], eax
        add    esi, 4
        inc    dword ptr [eax + 3ch]
        mov    eax, dword ptr [eax]
        inc    dword ptr [esp + 10h]
        cmp    eax, edi
        jne    SHORT loc_1_103F9
loc_1_1040B:
        mov    eax, dword ptr [esp + 10h]
        dec    dword ptr [esp + 10h]
        or     eax, eax
        je     NEAR PTR loc_1_104DB
        nop
loc_1_1041C:
        sub    esi, 4
        mov    edi, dword ptr [esi]
        mov    eax, dword ptr [edi + 38h]
        cmp    eax, 7
        je     SHORT loc_1_1045C
        cmp    eax, 3
        je     SHORT loc_1_10461
        or     eax, eax
        jne    NEAR PTR loc_1_104C2
        mov    ecx, dword ptr [edi + 0ch]
        or     ecx, ecx
        je     NEAR PTR loc_1_104C2
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        push   dword ptr [esp + 28h]
        push   ecx
        call   sub_1_AFAC
        add    esp, 10h
        jmp    SHORT loc_1_104C2
        db  08Bh, 0FFh
loc_1_1045C:
        cmp    eax, 3
        jne    SHORT loc_1_10464
loc_1_10461:
        inc    dword ptr [edi + 3ch]
loc_1_10464:
        mov    eax, dword ptr [edi + 0ch]
        mov    ebx, dword ptr [eax + 10h]
        mov    dword ptr [edi + 38h], 8
        mov    ebp, eax
        mov    dword ptr [ebp + 68h], 9
        mov    dword ptr [ebp + 0ch], 0
        mov    dword ptr [edi + 0ch], 0
        push   edi
        push   ebp
        call   sub_1_D61C
        add    esp, 8
        mov    eax, dword ptr [ebx + 58h]
        or     eax, eax
        je     SHORT loc_1_104AE
        push   2
        push   0
        push   0
        push   0
        push   0
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebx + 5ch]
        call   eax
        add    esp, 1ch
loc_1_104AE:
        push   0
        db  068h, 068h, 033h, 001h, 000h
        push   2
        push   7
        push   edi
        call   sub_1_C08C
        add    esp, 14h
loc_1_104C2:
        push   edi
        call   sub_1_DF20
        add    esp, 4
        mov    eax, dword ptr [esp + 10h]
        dec    dword ptr [esp + 10h]
        or     eax, eax
        jne    NEAR PTR loc_1_1041C
loc_1_104DB:
        push   esi
        call   sub_1_0024
        add    esp, 4
loc_1_104E4:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_1_104EC:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    word ptr [esp + 12h], 0
        cmp    dword ptr [esp + 20h], 0
        jle    SHORT loc_1_10573
        mov    esi, dword ptr [esp + 18h]
        mov    ebx, dword ptr [esp + 24h]
loc_1_10509:
        push   49h
        call   sub_1_001E
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_1_10580
        mov    edi, ebp
        sub    eax, eax
        mov    ecx, 12h
        rep stosd
        stosb
        push   ebx
        push   ebp
        call   sub_1_3630
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_1053A
        cmp    eax, 0ffh
        jne    SHORT loc_1_10575
loc_1_1053A:
        push   dword ptr [ebx + 70h]
        push   dword ptr [ebp + 10h]
        call   sub_1_3690
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_10553
        cmp    eax, 0ffh
        jne    SHORT loc_1_10575
loc_1_10553:
        mov    dword ptr [ebp + 18h], ebx
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp], eax
        mov    dword ptr [ebp + 4], esi
        mov    dword ptr [eax + 4], ebp
        mov    dword ptr [esi], ebp
        inc    word ptr [esp + 12h]
        movzx  eax, word ptr [esp + 12h]
        cmp    eax, dword ptr [esp + 20h]
        jl     SHORT loc_1_10509
loc_1_10573:
        sub    eax, eax
loc_1_10575:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_10580:
        pop    ebp
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h, 083h, 0ECh, 004h, 056h, 057h, 0A1h, 0F0h, 032h, 001h, 000h, 089h, 044h, 024h
        db  008h, 00Bh, 0C0h, 074h, 01Ah, 06Ah, 000h, 06Ah, 000h, 0FFh, 074h, 024h, 010h, 0E8h, 03Dh, 005h
        db  000h, 000h, 083h, 0C4h, 00Ch, 0C7h, 005h, 0F0h, 032h, 001h, 000h, 000h, 000h, 000h, 000h, 0A1h
        db  0FCh, 032h, 001h, 000h, 0A3h, 0E0h, 032h, 001h, 000h, 0C7h, 005h, 0E4h, 032h, 001h, 000h, 008h
        db  000h, 000h, 000h, 066h, 0C7h, 005h, 014h, 033h, 001h, 000h, 003h, 000h, 08Dh, 044h, 024h, 008h
        db  050h, 06Ah, 000h, 06Ah, 000h, 06Ah, 000h, 068h, 0C0h, 0F5h, 000h, 000h, 06Ah, 000h, 06Ah, 000h
        db  0A1h, 0FCh, 032h, 001h, 000h, 0C1h, 0E8h, 003h, 050h, 0E8h, 0BDh, 003h, 000h, 000h, 083h, 0C4h
        db  020h, 00Bh, 0C0h, 074h, 007h, 03Dh, 0FFh, 000h, 000h, 000h, 075h, 06Eh, 02Bh, 0D2h, 08Bh, 044h
        db  024h, 008h, 0A3h, 0F0h, 032h, 001h, 000h, 0A1h, 0B4h, 032h, 001h, 000h, 039h, 010h, 07Eh, 046h
        db  02Bh, 0F6h, 090h, 0A1h, 0B4h, 032h, 001h, 000h, 08Dh, 04Ch, 030h, 008h, 08Bh, 079h, 000h, 03Bh
        db  0F9h, 074h, 026h, 08Bh, 047h, 020h, 0A8h, 010h, 074h, 019h, 080h, 07Fh, 034h, 02Ah, 074h, 013h
        db  0A8h, 001h, 075h, 00Fh, 0C7h, 047h, 02Ch, 000h, 000h, 000h, 000h, 0A1h, 0FCh, 032h, 001h, 000h
        db  089h, 047h, 018h, 08Bh, 03Fh, 03Bh, 0F9h, 075h, 0DAh, 042h, 083h, 0C6h, 008h, 0A1h, 0B4h, 032h
        db  001h, 000h, 039h, 010h, 07Fh, 0BDh, 0FFh, 035h, 0F0h, 032h, 001h, 000h, 06Ah, 000h, 06Ah, 000h
        db  0E8h, 04Eh, 0EFh, 0FFh, 0FFh, 083h, 0C4h, 00Ch, 02Bh, 0C0h, 05Fh, 05Eh, 083h, 0C4h, 004h, 0C3h
        db  08Dh, 049h, 000h, 056h, 08Bh, 074h, 024h, 008h, 08Bh, 006h, 00Bh, 0C0h, 074h, 013h, 06Ah, 000h
        db  06Ah, 000h, 050h, 0E8h, 057h, 004h, 000h, 000h, 083h, 0C4h, 00Ch, 0C7h, 006h, 000h, 000h, 000h
        db  000h, 02Bh, 0C0h, 05Eh, 0C3h, 08Bh, 0FFh
sub_1_106A4:
        sub    esp, 4
        cmp    dword ptr [esp + 8], 0
        je     SHORT loc_1_106E8
        mov    ecx, dword ptr [esp + 14h]
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [eax + ecx + 1fh]
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
        jmp    SHORT loc_1_106EA
        db  08Dh, 049h, 000h
loc_1_106E8:
        sub    edx, edx
loc_1_106EA:
        sub    cl, cl
        lea    eax, [esp]
        push   eax
        push   0
        push   dword ptr [esp + 14h]
        push   0
        push   10h
        db  068h, 018h, 008h, 001h, 000h
        push   0
        movzx  ax, cl
        push   eax
        push   edx
        push   dword ptr [esp + 40h]
        push   dword ptr [esp + 40h]
        call   sub_1_E088
        add    esp, 2ch
        sub    eax, eax
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_10720:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 18h]
        or     ebx, ebx
        je     NEAR PTR loc_1_10808
        mov    ax, word ptr [ebx + 3eh]
        dec    ax
        mov    word ptr [ebx + 3eh], ax
        mov    edi, dword ptr [esp + 10h]
        jne    NEAR PTR loc_1_107D4
        mov    ax, word ptr [edi + 4ch]
        test   al, 2
        je     SHORT loc_1_10768
        and    ax, 0fffdh
        mov    word ptr [edi + 4ch], ax
        or     ax, 4
        mov    word ptr [edi + 4ch], ax
        db  066h, 0A1h, 0D0h, 032h, 001h, 000h
        mov    word ptr [ebx + 3eh], ax
        jmp    SHORT loc_1_107D4
        db  090h
loc_1_10768:
        mov    esi, dword ptr [edi + 38h]
        mov    dword ptr [ebx + 30h], 0
        mov    eax, dword ptr [esi + 20h]
        and    eax, 0ffffff0fh
        mov    dword ptr [esi + 20h], eax
        or     eax, 20h
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 14h], 0
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [esi + 4], esi
        mov    dword ptr [esi], esi
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        push   esi
        call   sub_1_6608
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_107C0
        cmp    eax, 0ffh
        je     SHORT loc_1_107C0
        push   esi
        call   sub_1_DEA4
        add    esp, 4
        jmp    SHORT loc_1_107C6
        db  08Bh, 0FFh
loc_1_107C0:
        mov    word ptr [esi + 30h], 1
loc_1_107C6:
        push   edi
        call   sub_1_F280
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_107D4:
        inc    dword ptr [edi + 34h]
        push   0
        push   0
        push   0
        push   0
        push   0
        push   0
        push   edi
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        push   dword ptr [edi + 38h]
        call   sub_1_10C0C
        add    esp, 24h
        push   edi
        call   sub_1_F280
        add    esp, 4
        or     word ptr [ebx + 3ch], 1
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_10808:
        push   dword ptr [esp + 10h]
        call   sub_1_F29C
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_10818:
        ret
        db  08Dh, 049h, 000h
sub_1_1081C:
        push   41h
        db  068h, 0E4h, 033h, 001h, 000h
        push   dword ptr [esp + 0ch]
        call   sub_1_10830
        add    esp, 0ch
        ret
sub_1_10830:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax + 4], eax
        mov    dword ptr [eax], eax
        lea    esi, [eax + 8]
        mov    dword ptr [eax + 0ch], esi
        mov    dword ptr [esi], esi
        mov    edi, dword ptr [esp + 10h]
        mov    eax, edi
        dec    edi
        or     eax, eax
        je     SHORT loc_1_10877
        mov    bx, word ptr [esp + 18h]
loc_1_10854:
        push   ebx
        call   sub_1_35F0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_10880
        mov    edx, dword ptr [esi]
        lea    ecx, [eax + 18h]
        mov    dword ptr [ecx], edx
        mov    dword ptr [eax + 1ch], esi
        mov    dword ptr [edx + 4], ecx
        mov    dword ptr [esi], ecx
        mov    eax, edi
        dec    edi
        or     eax, eax
        jne    SHORT loc_1_10854
loc_1_10877:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_10880:
        pop    edi
        mov    eax, 1
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_1_1088C:
        sub    esp, 8
        mov    ecx, dword ptr [esp + 0ch]
        or     ecx, ecx
        je     SHORT loc_1_108C3
        lea    eax, [esp + 4]
        push   eax
        lea    eax, [esp + 4]
        push   eax
        push   ecx
        call   sub_1_10AEC
        add    esp, 0ch
        cmp    dword ptr [esp], 0
        je     SHORT loc_1_108C3
        push   15h
        push   dword ptr [esp + 8]
        push   dword ptr [esp + 8]
        call   sub_1_E64C
        add    esp, 0ch
loc_1_108C3:
        add    esp, 8
        ret
        db  090h
sub_1_108C8:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 30h]
        mov    dword ptr [eax + 30h], 0
        or     ecx, ecx
        je     SHORT loc_1_108F4
        mov    edx, dword ptr [eax + 34h]
        mov    dword ptr [edx + 14h], 0
        push   dword ptr [esp + 8]
        push   dword ptr [eax + 2ch]
        call   ecx
        add    esp, 8
        sub    eax, eax
        ret
        db  090h
loc_1_108F4:
        mov    eax, 0c0000001h
        ret
        db  08Bh, 0FFh
sub_1_108FC:
        push   esi
        sub    esi, esi
        mov    eax, dword ptr [esp + 8]
        mov    edx, dword ptr [eax]
        cmp    eax, edx
        je     SHORT loc_1_10920
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx], edx
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    eax, esi
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_10920:
        push   dword ptr [esp + 0ch]
        call   sub_1_35F0
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_1093C
        mov    esi, 1
        mov    eax, esi
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_1093C:
        add    eax, 18h
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx], eax
        mov    eax, esi
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_1_1094C:
        sub    esp, 4
        push   esi
        lea    eax, [esp + 4]
        push   eax
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 24h]
        push   0
        mov    esi, dword ptr [esp + 28h]
        push   esi
        push   dword ptr [esp + 28h]
        call   sub_1_109B8
        add    esp, 20h
        or     eax, eax
        je     SHORT loc_1_10983
        cmp    eax, 0ffh
        jne    SHORT loc_1_109B2
loc_1_10983:
        cmp    byte ptr [esp + 28h], 0
        je     SHORT loc_1_109A4
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 24h]
        mov    dword ptr [ecx + 34h], edx
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [edx + 14h], ecx
        pop    esi
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_109A4:
        mov    edx, dword ptr [esp + 24h]
        mov    dword ptr [esi + 38h], edx
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [esi + 30h], ecx
loc_1_109B2:
        pop    esi
        add    esp, 4
        ret
        db  090h
sub_1_109B8:
        sub    esp, 4
        push   esi
        push   edi
        lea    eax, [esp + 8]
        push   eax
        push   41h
        db  068h, 0ECh, 033h, 001h, 000h
        call   sub_1_108FC
        add    esp, 0ch
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_109E3
        cmp    esi, 0ffh
        jne    NEAR PTR loc_1_10A83
loc_1_109E3:
        mov    edi, dword ptr [esp + 8]
        sub    edi, 18h
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [edi + 38h], ecx
        mov    byte ptr [edi + 40h], 1
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [edi + 20h], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [edi + 24h], eax
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [edi + 28h], eax
        mov    word ptr [edi + 3ch], 0
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [edi + 2ch], eax
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [edi + 30h], eax
        mov    ax, word ptr [esp + 28h]
        mov    word ptr [edi + 3eh], ax
        mov    dword ptr [edi + 0ch], 0
        mov    dword ptr [edi + 14h], 0
        db  0A1h, 068h, 02Eh, 001h, 000h
        mov    dword ptr [edi + 10h], eax
        push   edi
        db  068h, 080h, 00Bh, 001h, 000h
        push   ecx
        push   edi
        call   0
        add    esp, 10h
        mov    eax, dword ptr [esp + 2ch]
        or     eax, eax
        je     SHORT loc_1_10A57
        mov    dword ptr [eax], edi
loc_1_10A57:
        db  0A1h, 0E4h, 033h, 001h, 000h
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 8]
        db  0C7h, 040h, 004h, 0E4h, 033h, 001h, 000h
        mov    eax, dword ptr [esp + 8]
        db  08Bh, 00Dh, 0E4h, 033h, 001h, 000h
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 8]
        db  0A3h, 0E4h, 033h, 001h, 000h
loc_1_10A83:
        mov    eax, esi
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  090h
sub_1_10A8C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 14h]
        cmp    byte ptr [ebx + 40h], 0
        jne    SHORT loc_1_10AE0
        mov    ecx, dword ptr [ebx + 28h]
        mov    esi, dword ptr [ebx + 20h]
        mov    edi, dword ptr [ebx + 24h]
        lea    edx, [ebx + 18h]
        mov    ebp, dword ptr [edx]
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [eax], ebp
        mov    dword ptr [ebp + 4], eax
        db  0C7h, 002h, 0ECh, 033h, 001h, 000h
        db  0A1h, 0F0h, 033h, 001h, 000h
        mov    dword ptr [ebx + 1ch], eax
        db  0A1h, 0F0h, 033h, 001h, 000h
        mov    dword ptr [eax], edx
        db  089h, 015h, 0F0h, 033h, 001h, 000h
        or     ecx, ecx
        je     SHORT loc_1_10AD8
        push   0
        push   edi
        push   esi
        call   ecx
        add    esp, 0ch
loc_1_10AD8:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_10AE0:
        pop    ebp
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
sub_1_10AEC:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        or     edi, edi
        je     SHORT loc_1_10AFC
        mov    dword ptr [edi], 0
loc_1_10AFC:
        mov    esi, dword ptr [esp + 0ch]
        mov    cl, byte ptr [esi + 40h]
        cmp    cl, 1
        jne    SHORT loc_1_10B44
        test   byte ptr [esi + 3ch], 8
        jne    SHORT loc_1_10B17
        push   esi
        call   6
        add    esp, 4
loc_1_10B17:
        or     edi, edi
        je     SHORT loc_1_10B20
        mov    eax, dword ptr [esi + 30h]
        mov    dword ptr [edi], eax
loc_1_10B20:
        mov    edx, dword ptr [esp + 14h]
        or     edx, edx
        je     SHORT loc_1_10B2D
        mov    eax, dword ptr [esi + 2ch]
        mov    dword ptr [edx], eax
loc_1_10B2D:
        push   esi
        mov    dword ptr [esi + 30h], 0
        mov    byte ptr [esi + 40h], 0
        call   sub_1_10A8C
        add    esp, 4
        pop    edi
        pop    esi
        ret
loc_1_10B44:
        cmp    cl, 2
        jne    SHORT loc_1_10B78
        mov    eax, dword ptr [esi + 30h]
        or     eax, eax
        je     SHORT loc_1_10B6A
        or     edi, edi
        je     SHORT loc_1_10B56
        mov    dword ptr [edi], eax
loc_1_10B56:
        mov    edx, dword ptr [esp + 14h]
        or     edx, edx
        je     SHORT loc_1_10B63
        mov    eax, dword ptr [esi + 2ch]
        mov    dword ptr [edx], eax
loc_1_10B63:
        mov    dword ptr [esi + 30h], 0
loc_1_10B6A:
        inc    cl
        mov    byte ptr [esi + 40h], cl
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        ret
        db  090h
loc_1_10B78:
        pop    edi
        mov    eax, 0c0000001h
        pop    esi
        ret
loc_1_10B80:
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        mov    al, byte ptr [esi + 40h]
        or     al, al
        je     SHORT loc_1_10C08
        inc    al
        mov    byte ptr [esi + 40h], al
        and    word ptr [esi + 3ch], 0fffeh
        push   esi
        push   dword ptr [esi + 24h]
        push   dword ptr [esi + 20h]
        call   dword ptr [esi + 28h]
        add    esp, 0ch
        mov    al, byte ptr [esi + 40h]
        dec    al
        mov    byte ptr [esi + 40h], al
        test   byte ptr [esi + 3ch], 1
        je     SHORT loc_1_10BDC
        cmp    al, 2
        jne    SHORT loc_1_10BC8
        mov    byte ptr [esi + 40h], 0
        push   esi
        call   sub_1_10A8C
        add    esp, 4
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_10BC8:
        push   esi
        db  068h, 080h, 00Bh, 001h, 000h
        push   dword ptr [esi + 38h]
        push   esi
        call   0
        add    esp, 10h
        pop    esi
        ret
loc_1_10BDC:
        lea    edx, [esi + 18h]
        mov    byte ptr [esi + 40h], 0
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        db  0C7h, 002h, 0ECh, 033h, 001h, 000h
        db  0A1h, 0F0h, 033h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        db  0A1h, 0F0h, 033h, 001h, 000h
        mov    dword ptr [eax], edx
        db  089h, 015h, 0F0h, 033h, 001h, 000h
loc_1_10C08:
        pop    esi
        ret
        db  08Bh, 0FFh
sub_1_10C0C:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 24h]
        test   byte ptr [ebx + 20h], 0ch
        mov    di, 1
        jne    SHORT loc_1_10C24
        sub    di, di
loc_1_10C24:
        lea    eax, [esp + 10h]
        push   eax
        call   sub_1_101D0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_10C4C
        cmp    eax, 0ffh
        je     SHORT loc_1_10C4C
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
loc_1_10C4C:
        mov    ebp, dword ptr [esp + 2ch]
        push   ebp
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        push   dword ptr [esp + 50h]
        push   edi
        push   0
        push   dword ptr [esp + 40h]
        lea    eax, [ebx + 34h]
        push   eax
        call   sub_1_10E60
        add    esp, 20h
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_1_10E50
        cmp    dword ptr [esp + 44h], 7
        jne    SHORT loc_1_10C94
        db  066h, 0A1h, 058h, 033h, 001h, 000h
        mov    ecx, dword ptr [esp + 14h]
        mov    word ptr [ecx + 2], ax
        jmp    SHORT loc_1_10CA4
        db  090h
loc_1_10C94:
        cmp    dword ptr [esp + 44h], 0
        je     SHORT loc_1_10CA2
        cmp    dword ptr [esp + 44h], 1
        jne    SHORT loc_1_10CA4
loc_1_10CA2:
        sub    esi, esi
loc_1_10CA4:
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx + 18h], eax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 24h], 0
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 38h], ebx
        mov    eax, dword ptr [ebp + 2ch]
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx + 2ch], eax
        mov    dword ptr [ebp + 38h], ebx
        mov    eax, dword ptr [esp + 14h]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp + 40h], ax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 30h], esi
        mov    eax, dword ptr [esp + 40h]
        or     eax, eax
        je     SHORT loc_1_10D62
        lea    ecx, [esp + 18h]
        push   ecx
        push   dword ptr [esp + 40h]
        push   dword ptr [esp + 40h]
        push   dword ptr [esp + 40h]
        push   dword ptr [esp + 40h]
        push   0
        push   ebp
        push   eax
        call   sub_1_109B8
        add    esp, 20h
        or     eax, eax
        je     SHORT loc_1_10D54
        cmp    eax, 0ffh
        je     SHORT loc_1_10D54
        push   dword ptr [esp + 14h]
        call   sub_1_0024
        add    esp, 4
        push   4
        push   dword ptr [esp + 14h]
        call   sub_1_D5C0
        add    esp, 8
        mov    eax, 0c00000f4h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
loc_1_10D54:
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [eax + 34h], ebx
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [ebx + 14h], eax
loc_1_10D62:
        mov    ecx, dword ptr [ebp + 2ch]
        mov    dx, word ptr [ebp + 4ch]
        test   dl, 1
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 1ch]
        je     SHORT loc_1_10D84
        or     byte ptr [eax + 3], 10h
        mov    di, 89h
        mov    edx, dword ptr [ecx + 60h]
        jmp    SHORT loc_1_10DEE
        db  08Bh, 0FFh
loc_1_10D84:
        and    byte ptr [eax + 3], 0efh
        db  080h, 03Dh, 050h, 033h, 001h, 000h, 000h
        je     SHORT loc_1_10DA8
        cmp    dword ptr [esp + 44h], 3
        jne    SHORT loc_1_10DA8
        or     di, di
        jne    SHORT loc_1_10DA8
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 1ch]
        or     byte ptr [eax + 2], 78h
loc_1_10DA8:
        test   dl, 2
        db  066h, 08Bh, 03Dh, 010h, 033h, 001h, 000h
        je     SHORT loc_1_10DBC
        mov    edx, dword ptr [ecx + 74h]
        jmp    SHORT loc_1_10DE1
        db  08Dh, 049h, 000h
loc_1_10DBC:
        test   dl, 4
        je     SHORT loc_1_10DC8
        mov    edx, dword ptr [ecx + 78h]
        jmp    SHORT loc_1_10DE1
        db  08Bh, 0FFh
loc_1_10DC8:
        test   dl, 80h
        je     SHORT loc_1_10DD4
        mov    edx, dword ptr [ecx + 7ch]
        jmp    SHORT loc_1_10DDA
        db  08Bh, 0FFh
loc_1_10DD4:
        mov    edx, dword ptr [ecx + 80h]
loc_1_10DDA:
        db  066h, 08Bh, 03Dh, 012h, 033h, 001h, 000h
loc_1_10DE1:
        mov    eax, dword ptr [esp + 14h]
        cmp    byte ptr [eax + 7], 0ffh
        jne    SHORT loc_1_10DEE
        mov    edx, dword ptr [ecx + 58h]
loc_1_10DEE:
        or     esi, esi
        je     SHORT loc_1_10E20
        mov    eax, dword ptr [ecx + 58h]
        mov    ebx, eax
        and    ebx, 0ff00h
        mov    ebp, eax
        shl    ebp, 10h
        or     ebx, ebp
        shl    ebx, 8
        mov    ebp, eax
        and    ebp, 0ff0000h
        shr    ebp, 8
        or     ebx, ebp
        shr    eax, 18h
        and    eax, 0ffh
        or     ebx, eax
        mov    dword ptr [esi], ebx
loc_1_10E20:
        mov    eax, 0
        cmp    dword ptr [ecx + 58h], 0
        je     SHORT loc_1_10E2E
        mov    eax, dword ptr [ecx + 68h]
loc_1_10E2E:
        push   10h
        push   edi
        push   dword ptr [esp + 18h]
        db  068h, 0B8h, 010h, 001h, 000h
        push   eax
        push   edx
        push   dword ptr [esp + 28h]
        call   sub_1_115D4
        add    esp, 1ch
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_10E50:
        pop    ebp
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
sub_1_10E60:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 2ch]
        cmp    esi, 1
        jne    SHORT loc_1_10E90
        mov    eax, dword ptr [esp + 38h]
        push   dword ptr [eax + 3ch]
        call   sub_1_101B4
        add    esp, 4
        inc    eax
        mov    dword ptr [esp + 10h], eax
        cmp    eax, 1
        jbe    SHORT loc_1_10EA3
        inc    dword ptr [esp + 10h]
        jmp    SHORT loc_1_10EA3
        db  090h
loc_1_10E90:
        sub    eax, eax
        mov    edi, dword ptr [esp + 20h]
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        mov    dword ptr [esp + 10h], ecx
loc_1_10EA3:
        mov    ebp, dword ptr [esp + 10h]
        add    ebp, 43h
        or     esi, esi
        jne    SHORT loc_1_10EB4
        sub    ebp, 12h
        jmp    SHORT loc_1_10EBC
        db  090h
loc_1_10EB4:
        cmp    esi, 1
        jne    SHORT loc_1_10EBC
        sub    ebp, 22h
loc_1_10EBC:
        movzx  eax, bp
        push   eax
        call   sub_1_001E
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_10ED8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_10ED8:
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, ebp
        shr    ecx, 2
        rep stosd
        mov    ecx, ebp
        and    ecx, 3
        rep stosb
        mov    eax, dword ptr [esp + 38h]
        mov    ax, word ptr [eax + 40h]
        or     ax, ax
        je     SHORT loc_1_10EFC
        mov    word ptr [ebx], ax
        jmp    SHORT loc_1_10F25
loc_1_10EFC:
        db  08Ah, 025h, 00Eh, 033h, 001h, 000h
        db  0A0h, 00Fh, 033h, 001h, 000h
        mov    word ptr [ebx], ax
        db  066h, 0FFh, 005h, 00Eh, 033h, 001h, 000h
        db  066h, 081h, 03Dh, 00Eh, 033h, 001h, 000h, 0FFh, 0FFh
        jne    SHORT loc_1_10F25
        db  066h, 0C7h, 005h, 00Eh, 033h, 001h, 000h, 000h, 000h
loc_1_10F25:
        mov    byte ptr [ebx + 5], 1
        mov    byte ptr [ebx + 7], 0
        mov    byte ptr [ebx + 9], 0
        cmp    esi, 1
        je     SHORT loc_1_10F58
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 24h]
        lea    eax, [ebx + 0ch]
        push   eax
        call   sub_1_10064
        add    esp, 10h
        mov    dword ptr [eax], 1002000h
        jmp    SHORT loc_1_10F5C
        db  08Bh, 0FFh
loc_1_10F58:
        mov    eax, dword ptr [esp + 14h]
loc_1_10F5C:
        cmp    esi, 7
        mov    ecx, dword ptr [esp + 30h]
        mov    dword ptr [ecx], ebx
        mov    ecx, dword ptr [esp + 34h]
        mov    dword ptr [ecx], ebp
        ja     NEAR PTR loc_1_11066
        db  0FFh, 024h, 0B5h, 074h, 010h, 001h, 000h
loc_1_10F78:
        db  0F6h, 005h, 064h, 033h, 001h, 000h, 001h
        je     SHORT loc_1_10F98
        mov    word ptr [ebx + 2], 1001h
        mov    byte ptr [ebx + 0bh], 0
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_10F98:
        mov    eax, ebx
        mov    word ptr [ebx + 2], 1
        mov    byte ptr [ebx + 0bh], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_10FAC:
        mov    eax, dword ptr [esp + 38h]
        push   dword ptr [eax + 3ch]
        push   dword ptr [esp + 20h]
        lea    eax, [ebx + 0ch]
        push   eax
        call   sub_1_100C0
        add    esp, 0ch
        mov    dword ptr [eax], 1000100h
        mov    word ptr [ebx + 2], 1
        mov    byte ptr [ebx + 0bh], 0
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_10FE0:
        cmp    esi, 4
        mov    word ptr [ebx + 2], 28h
        je     SHORT loc_1_10FF1
        mov    word ptr [ebx + 2], 29h
loc_1_10FF1:
        mov    dword ptr [eax + 0ah], 0e0930400h
loc_1_10FF8:
        cmp    esi, 6
        jne    SHORT loc_1_1100A
        mov    word ptr [ebx + 2], 30h
        mov    dword ptr [eax + 0ah], 0
loc_1_1100A:
        mov    byte ptr [ebx + 0bh], 1
        mov    byte ptr [eax + 4], 0c0h
        mov    byte ptr [eax + 5], 0ch
        mov    dword ptr [eax + 6], 1002000h
        mov    word ptr [eax + 0eh], 600h
        mov    cx, word ptr [esp + 28h]
        shl    cx, 0fh
        db  066h, 00Bh, 00Dh, 00Ch, 033h, 001h, 000h
        mov    dh, cl
        mov    dl, ch
        mov    word ptr [eax + 10h], dx
        mov    ecx, dword ptr [esp + 24h]
        mov    ebx, ecx
        and    ebx, 0ff00h
        mov    edx, ecx
        shl    edx, 10h
        or     ebx, edx
        shl    ebx, 8
        mov    edx, ecx
        sar    edx, 18h
        mov    bl, dl
        and    ecx, 0ff0000h
        shr    ecx, 8
        or     ebx, ecx
        mov    dword ptr [eax + 12h], ebx
loc_1_11066:
        pop    ebp
        add    eax, 12h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h, 078h, 00Fh, 001h, 000h, 0ACh, 00Fh, 001h, 000h, 066h, 010h, 001h, 000h, 0E0h
        db  00Fh, 001h, 000h, 0E0h, 00Fh, 001h, 000h, 066h, 010h, 001h, 000h, 0F8h, 00Fh, 001h, 000h, 0E0h
        db  00Fh, 001h, 000h, 08Bh, 044h, 024h, 004h, 066h, 081h, 060h, 04Ch, 07Fh, 0FFh, 050h, 0E8h, 0DCh
        db  0E1h, 0FFh, 0FFh, 083h, 0C4h, 004h, 0C3h
loc_1_110A8:
        push   dword ptr [esp + 4]
        call   sub_1_0024
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_110B8:
        push   5
        push   dword ptr [esp + 8]
        call   sub_1_D5C0
        add    esp, 8
        ret
        db  090h
sub_1_110C8:
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    byte ptr [esp + 13h], 1
        sub    esi, esi
        mov    byte ptr [esp + 12h], 1
        mov    ecx, dword ptr [esp + 48h]
        mov    edx, dword ptr [ecx + 2]
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
        and    edx, 0ffh
        or     edi, edx
        mov    dword ptr [esp + 30h], edi
        db  080h, 03Dh, 050h, 033h, 001h, 000h, 000h
        mov    ax, word ptr [ecx]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [esp + 14h], cx
        je     SHORT loc_1_1115E
        db  080h, 03Dh, 051h, 033h, 001h, 000h, 000h
        jne    SHORT loc_1_1115E
        cmp    dword ptr [esp + 54h], 2
        jne    SHORT loc_1_1115E
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 34h]
        call   sub_1_9D64
        add    esp, 8
        or     al, al
        je     SHORT loc_1_1115E
        mov    byte ptr [esp + 13h], 0
        db  00Fh, 0B6h, 005h, 04Fh, 033h, 001h, 000h
        lea    eax, [eax + eax*2]
        lea    esi, [eax*2 - 6]
loc_1_1115E:
        mov    eax, dword ptr [esp + 3ch]
        lea    eax, [esi + eax + 1dh]
        mov    dword ptr [esp + 2ch], eax
        and    eax, 0ffffh
        push   eax
        call   sub_1_001E
        add    esp, 4
        mov    dword ptr [esp + 18h], eax
        or     eax, eax
        jne    SHORT loc_1_11190
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
        db  08Dh, 049h, 000h
loc_1_11190:
        sub    eax, eax
        mov    edx, dword ptr [esp + 2ch]
        mov    edi, dword ptr [esp + 18h]
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        mov    ecx, dword ptr [esp + 18h]
        mov    byte ptr [ecx + 5], al
        mov    byte ptr [ecx + 7], 1
        mov    eax, dword ptr [esp + 3ch]
        lea    esi, [ecx + eax + 0dh]
        mov    dword ptr [esp + 20h], esi
        mov    dword ptr [esi], 1002000h
        mov    dword ptr [esi + 4], 0
        mov    word ptr [esi + 8], 600h
        mov    eax, esi
        add    eax, 0ah
        db  08Ah, 02Dh, 00Ch, 033h, 001h, 000h
        db  08Ah, 00Dh, 00Dh, 033h, 001h, 000h
        mov    word ptr [eax], cx
        mov    edx, dword ptr [esp + 44h]
        or     edx, edx
        je     SHORT loc_1_11200
        mov    ecx, dword ptr [edx + 20h]
        and    ecx, 0ch
        or     ecx, ecx
        mov    cx, 0
        setne  cl
        jmp    SHORT loc_1_11205
        db  090h
loc_1_11200:
        mov    cx, word ptr [esp + 16h]
loc_1_11205:
        shl    cx, 0fh
        db  066h, 00Bh, 00Dh, 00Ch, 033h, 001h, 000h
        mov    bh, cl
        mov    bl, ch
        mov    word ptr [esp + 28h], bx
        mov    si, bx
        mov    word ptr [eax], si
        sub    ebx, ebx
        mov    ecx, dword ptr [esp + 50h]
        mov    bh, cl
        mov    bl, ch
        mov    ecx, dword ptr [esp + 54h]
        cmp    ecx, 2
        je     SHORT loc_1_112AC
        cmp    ecx, 5
        jl     SHORT loc_1_11240
        cmp    ecx, 6
        jle    NEAR PTR loc_1_114DC
loc_1_11240:
        mov    ecx, dword ptr [esp + 34h]
loc_1_11244:
        or     ecx, ecx
        je     SHORT loc_1_11272
loc_1_11248:
        mov    edx, ecx
        shl    edx, 10h
        mov    eax, ecx
        and    eax, 0ff00h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        sar    eax, 18h
        mov    dl, al
        and    ecx, 0ff0000h
        shr    ecx, 8
        or     edx, ecx
        mov    esi, dword ptr [esp + 20h]
        mov    dword ptr [esi + 0ch], edx
loc_1_11272:
        lea    eax, [esp + 24h]
        push   eax
        call   sub_1_101D0
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_11568
        cmp    eax, 0ffh
        je     NEAR PTR loc_1_11568
        push   dword ptr [esp + 18h]
        call   sub_1_0024
        add    esp, 4
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
        db  090h
loc_1_112AC:
        mov    ecx, dword ptr [esp + 18h]
        mov    word ptr [ecx + 2], 85h
        mov    esi, dword ptr [esp + 40h]
        mov    di, word ptr [esi]
        mov    word ptr [ecx], di
        mov    ebx, dword ptr [esp + 3ch]
        inc    ebx
        add    esi, 0ch
        mov    edi, ecx
        add    edi, 0ch
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        or     edx, edx
        jne    SHORT loc_1_112FC
        mov    ecx, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 20h]
        or     word ptr [ecx + 2], 300h
        mov    word ptr [esi + 8], 0
        sub    ecx, ecx
        jmp    NEAR PTR loc_1_11244
        db  08Bh, 0FFh
loc_1_112FC:
        db  066h, 08Bh, 03Dh, 064h, 033h, 001h, 000h
        and    di, 10h
        jne    SHORT loc_1_11358
        mov    ecx, dword ptr [edx + 28h]
        mov    esi, dword ptr [esp + 4ch]
        test   dword ptr [esi + 88h], ecx
        jne    SHORT loc_1_11358
        mov    cx, word ptr [esp + 14h]
        db  066h, 03Bh, 00Dh, 010h, 033h, 001h, 000h
        jne    SHORT loc_1_1132F
        mov    ecx, dword ptr [esp + 30h]
        cmp    dword ptr [esi + 74h], ecx
        je     SHORT loc_1_11358
loc_1_1132F:
        mov    esi, dword ptr [esp + 4ch]
        mov    ecx, dword ptr [esp + 30h]
        cmp    dword ptr [esi + 78h], ecx
        je     SHORT loc_1_11358
        push   dword ptr [esp + 18h]
        call   sub_1_0024
        add    esp, 4
        mov    eax, 0c0000001h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
        db  08Dh, 049h, 000h
loc_1_11358:
        or     di, di
        mov    esi, dword ptr [esp + 20h]
        mov    dword ptr [esi + 4], 0e0930400h
        je     SHORT loc_1_113B4
        mov    byte ptr [esp + 12h], 0
        test   byte ptr [edx + 20h], 8
        je     SHORT loc_1_11384
        mov    esi, dword ptr [edx + 0ch]
        mov    ecx, dword ptr [esi + 4]
        cmp    ecx, -1
        jne    SHORT loc_1_113A7
        mov    ecx, dword ptr [esi]
        jmp    SHORT loc_1_113A7
        db  08Bh, 0FFh
loc_1_11384:
        cmp    dword ptr [edx + 24h], 0dead0000h
        jne    SHORT loc_1_113A4
        db  080h, 03Dh, 050h, 033h, 001h, 000h, 000h
        je     SHORT loc_1_113A4
        mov    byte ptr [esp + 12h], 1
        mov    ecx, dword ptr [esp + 34h]
        jmp    SHORT loc_1_113A7
        db  08Dh, 049h, 000h
loc_1_113A4:
        mov    ecx, dword ptr [edx + 0ch]
loc_1_113A7:
        or     ecx, ecx
        jne    SHORT loc_1_113B8
        mov    esi, dword ptr [esp + 4ch]
        mov    ecx, dword ptr [esi + 60h]
        jmp    SHORT loc_1_113B8
loc_1_113B4:
        mov    ecx, dword ptr [esp + 34h]
loc_1_113B8:
        cmp    byte ptr [esp + 12h], 0
        je     NEAR PTR loc_1_11244
        cmp    byte ptr [esp + 13h], 0
        je     SHORT loc_1_11428
        db  080h, 03Dh, 052h, 033h, 001h, 000h, 000h
        je     SHORT loc_1_1141C
        sub    esi, esi
        sub    ecx, ecx
        nop
loc_1_113D8:
        cmp    esi, 5
        jae    SHORT loc_1_11406
        inc    esi
        call   sub_1_000C
        shr    eax, 4
        db  00Fh, 0B6h, 00Dh, 04Fh, 033h, 001h, 000h
        sub    edx, edx
        div    ecx
        db  0A1h, 03Ch, 032h, 001h, 000h
        or     edx, edx
        jle    SHORT loc_1_113FF
loc_1_113FA:
        mov    eax, dword ptr [eax]
        dec    edx
        jne    SHORT loc_1_113FA
loc_1_113FF:
        mov    ecx, dword ptr [eax + 58h]
        or     ecx, ecx
        je     SHORT loc_1_113D8
loc_1_11406:
        or     ecx, ecx
        jne    NEAR PTR loc_1_11248
        mov    esi, dword ptr [esp + 4ch]
        mov    ecx, dword ptr [esi + 58h]
        jmp    NEAR PTR loc_1_11244
        db  08Bh, 0FFh
loc_1_1141C:
        mov    esi, dword ptr [esp + 4ch]
        mov    ecx, dword ptr [esi + 58h]
        jmp    NEAR PTR loc_1_11244
loc_1_11428:
        sub    ecx, ecx
        mov    dword ptr [esp + 1ch], 0
        db  08Bh, 03Dh, 03Ch, 032h, 001h, 000h
        db  081h, 0FFh, 03Ch, 032h, 001h, 000h
        je     SHORT loc_1_1149A
loc_1_11440:
        mov    esi, dword ptr [edx + 28h]
        test   dword ptr [edi + 88h], esi
        je     SHORT loc_1_11490
        cmp    dword ptr [edi + 58h], 0
        je     SHORT loc_1_11490
        mov    si, word ptr [esp + 28h]
        mov    word ptr [eax], si
        mov    esi, dword ptr [edi + 58h]
        mov    ebp, esi
        and    ebp, 0ff00h
        mov    ebx, esi
        shl    ebx, 10h
        or     ebp, ebx
        shl    ebp, 8
        mov    ebx, esi
        and    ebx, 0ff0000h
        shr    ebx, 8
        or     ebp, ebx
        shr    esi, 18h
        and    esi, 0ffh
        or     ebp, esi
        mov    dword ptr [eax + 2], ebp
        inc    dword ptr [esp + 1ch]
        add    eax, 6
loc_1_11490:
        mov    edi, dword ptr [edi]
        db  081h, 0FFh, 03Ch, 032h, 001h, 000h
        jne    SHORT loc_1_11440
loc_1_1149A:
        db  00Fh, 0B6h, 005h, 04Fh, 033h, 001h, 000h
        cmp    eax, dword ptr [esp + 1ch]
        je     SHORT loc_1_114BB
        mov    edx, dword ptr [esp + 1ch]
        sub    edx, eax
        lea    edx, [edx + edx*2]
        mov    eax, dword ptr [esp + 2ch]
        lea    eax, [eax + edx*2]
        mov    dword ptr [esp + 2ch], eax
loc_1_114BB:
        mov    eax, dword ptr [esp + 1ch]
        lea    edx, [eax + eax*2]
        add    edx, edx
        mov    dl, dh
        imul   ax, ax, 600h
        mov    dh, ah
        mov    esi, dword ptr [esp + 20h]
        mov    word ptr [esi + 8], dx
        jmp    NEAR PTR loc_1_11244
        db  08Bh, 0FFh
loc_1_114DC:
        mov    eax, dword ptr [esp + 3ch]
        add    eax, 0dh
        mov    edi, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 40h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        cmp    dword ptr [esp + 54h], 6
        jne    SHORT loc_1_11508
        or     bx, 0b4h
        jmp    SHORT loc_1_1150D
        db  090h
loc_1_11508:
        or     bx, 80adh
loc_1_1150D:
        mov    ecx, dword ptr [esp + 18h]
        mov    word ptr [ecx + 2], bx
        mov    ecx, dword ptr [esp + 18h]
        mov    byte ptr [ecx + 5], 0
        mov    byte ptr [ecx + 7], 1
        mov    byte ptr [ecx + 0bh], 0
        mov    byte ptr [ecx + 9], 0
        or     edx, edx
        je     SHORT loc_1_11560
        mov    eax, dword ptr [edx + 20h]
        test   al, 8
        je     SHORT loc_1_11540
        mov    eax, dword ptr [edx + 0ch]
        mov    ecx, dword ptr [eax]
        jmp    NEAR PTR loc_1_11244
        db  08Bh, 0FFh
loc_1_11540:
        mov    ecx, dword ptr [edx + 0ch]
        or     ecx, ecx
        jne    NEAR PTR loc_1_11248
        test   al, 4
        je     NEAR PTR loc_1_11244
        mov    esi, dword ptr [esp + 4ch]
        mov    ecx, dword ptr [esi + 58h]
        jmp    NEAR PTR loc_1_11244
        db  090h
loc_1_11560:
        sub    ecx, ecx
        jmp    NEAR PTR loc_1_11244
        db  090h
loc_1_11568:
        mov    ecx, dword ptr [esp + 24h]
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [ecx + 18h], eax
        mov    eax, dword ptr [esp + 24h]
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [eax + 1ch], ecx
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [eax + 24h], 0
        push   10h
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 2ch]
        db  068h, 0C4h, 015h, 001h, 000h
        mov    esi, dword ptr [esp + 5ch]
        push   dword ptr [esi + 68h]
        push   dword ptr [esp + 44h]
        push   dword ptr [esp + 3ch]
        call   sub_1_115D4
        add    esp, 1ch
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
        db  08Bh, 0FFh
loc_1_115C4:
        push   5
        push   dword ptr [esp + 8]
        call   sub_1_D5C0
        add    esp, 8
        ret
        db  090h
sub_1_115D4:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 2ch]
        cmp    eax, 7f000000h
        je     NEAR PTR loc_1_116A0
        mov    edx, dword ptr [esp + 30h]
        or     edx, edx
        je     NEAR PTR loc_1_116A0
        mov    esi, dword ptr [esp + 28h]
        mov    ecx, dword ptr [esi + 28h]
        or     eax, eax
        jne    SHORT loc_1_11607
        mov    eax, dword ptr [esi + 2ch]
        mov    eax, dword ptr [eax + 60h]
loc_1_11607:
        lea    ebp, [esi + 18h]
        mov    dword ptr [esp + 14h], edx
        mov    edx, dword ptr [esp + 34h]
        mov    dword ptr [esp + 18h], edx
        mov    edx, dword ptr [esp + 38h]
        mov    dword ptr [esp + 1ch], edx
        mov    edi, dword ptr [esi + 20h]
        db  08Bh, 015h, 080h, 02Fh, 001h, 000h
        add    edi, dword ptr [ebp]
        mov    bx, word ptr [edx + 0a2h]
        movzx  edx, bx
        add    edx, 8
        mov    dword ptr [ecx + 10h], edx
        mov    esi, dword ptr [ecx + 14h]
        mov    dword ptr [esi], 1
        mov    word ptr [esi + 4], bx
        mov    word ptr [esi + 6], 2
        mov    bx, word ptr [esp + 3ch]
        mov    dh, bl
        mov    dl, bh
        mov    word ptr [esi + 8], dx
        mov    edx, eax
        and    edx, 0ff00h
        mov    ebx, eax
        shl    ebx, 10h
        or     edx, ebx
        shl    edx, 8
        mov    ebx, eax
        sar    ebx, 18h
        mov    dl, bl
        and    eax, 0ff0000h
        shr    eax, 8
        or     edx, eax
        mov    dword ptr [esi + 0ah], edx
        push   10h
        push   ebp
        lea    eax, [esp + 18h]
        push   eax
        push   edi
        push   ecx
        lea    eax, [esp + 28h]
        push   eax
        call   sub_1_3880
        add    esp, 18h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  090h
loc_1_116A0:
        mov    edx, dword ptr [esp + 34h]
        or     edx, edx
        je     SHORT loc_1_116B5
        push   0
        push   0
        push   dword ptr [esp + 40h]
        call   edx
        add    esp, 0ch
loc_1_116B5:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  090h
sub_1_116C0:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 24h]
        mov    eax, dword ptr [edi + 28h]
        mov    esi, dword ptr [edi + 14h]
        mov    ecx, dword ptr [esi + 0ch]
        or     ecx, ecx
        je     NEAR PTR loc_1_1176C
        mov    ecx, dword ptr [ecx + 10h]
        mov    dword ptr [esp + 10h], ecx
        mov    ecx, dword ptr [esp + 30h]
        mov    dword ptr [esp + 14h], ecx
        mov    dword ptr [esp + 18h], edi
        db  08Bh, 00Dh, 080h, 02Fh, 001h, 000h
        mov    dx, word ptr [ecx + 0a2h]
        movzx  ecx, dx
        add    ecx, 8
        mov    dword ptr [eax + 10h], ecx
        mov    ecx, dword ptr [eax + 14h]
        mov    dword ptr [ecx], 1
        mov    word ptr [ecx + 4], dx
        mov    word ptr [ecx + 6], 2
        mov    bx, word ptr [esp + 34h]
        mov    dh, bl
        mov    dl, bh
        mov    word ptr [ecx + 8], dx
        mov    ebp, dword ptr [esp + 28h]
        mov    edx, ebp
        shl    edx, 10h
        mov    ebx, ebp
        and    ebx, 0ff00h
        or     edx, ebx
        shl    edx, 8
        mov    ebx, ebp
        sar    ebx, 18h
        mov    dl, bl
        and    ebp, 0ff0000h
        shr    ebp, 8
        or     edx, ebp
        mov    dword ptr [ecx + 0ah], edx
        push   dword ptr [esi + 54h]
        push   eax
        push   dword ptr [edi + 3ch]
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_1_39F0
        add    esp, 10h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_1176C:
        push   0
        push   0c0000120h
        push   edi
        call   dword ptr [esp + 3ch]
        add    esp, 0ch
        mov    eax, 0c0000120h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
sub_1_11788:
        sub    esp, 4
        push   esi
        push   edi
        push   5
        call   sub_1_001E
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_117A8
        mov    eax, 1
        pop    edi
        pop    esi
        add    esp, 4
        ret
loc_1_117A8:
        lea    eax, [esp + 8]
        push   eax
        call   sub_1_101D0
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_117D4
        cmp    edi, 0ffh
        je     SHORT loc_1_117D4
        push   esi
        call   sub_1_0024
        add    esp, 4
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 4
        ret
loc_1_117D4:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 1ch], esi
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 24h], 0
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 20h], 0
        mov    byte ptr [esi + 1], 0
        mov    eax, dword ptr [esp + 14h]
        mov    byte ptr [esi], al
        cmp    eax, 82h
        je     SHORT loc_1_1180C
        cmp    eax, 83h
        je     SHORT loc_1_11820
        jmp    SHORT loc_1_11838
        db  090h
loc_1_1180C:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 18h], 4
        mov    word ptr [esi + 2], 0
        jmp    SHORT loc_1_11838
        db  090h
loc_1_11820:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 18h], 5
        mov    word ptr [esi + 2], 100h
        mov    al, byte ptr [esp + 18h]
        mov    byte ptr [esi + 4], al
loc_1_11838:
        db  068h, 098h, 018h, 001h, 000h
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 10h]
        call   sub_1_11858
        add    esp, 0ch
        mov    edi, eax
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  090h
sub_1_11858:
        sub    esp, 14h
        mov    eax, dword ptr [esp + 1ch]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [esp + 4], eax
        mov    edx, dword ptr [esp + 18h]
        mov    dword ptr [esp + 0ch], edx
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [esp + 8], eax
        lea    ecx, [edx + 18h]
        push   0
        push   ecx
        lea    eax, [esp + 8]
        push   eax
        mov    eax, dword ptr [edx + 20h]
        add    eax, dword ptr [ecx]
        push   eax
        push   0
        lea    eax, [esp + 18h]
        push   eax
        call   sub_1_3A60
        add    esp, 2ch
        ret
        db  090h
loc_1_11898:
        push   5
        push   dword ptr [esp + 8]
        call   sub_1_D5C0
        add    esp, 8
        ret
        db  090h
sub_1_118A8:
        sub    esp, 4
        lea    eax, [esp]
        push   eax
        call   sub_1_101D0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_118C3
        cmp    eax, 0ffh
        jne    SHORT loc_1_118E0
loc_1_118C3:
        push   0
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [ecx + 14h], eax
        push   4
        push   0
        push   dword ptr [esp + 0ch]
        call   sub_1_118E4
        add    esp, 10h
loc_1_118E0:
        add    esp, 4
        ret
sub_1_118E4:
        sub    esp, 10h
        mov    edx, dword ptr [esp + 14h]
        mov    eax, dword ptr [edx + 14h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [esp], eax
        mov    dword ptr [esp + 8], edx
        db  0C7h, 044h, 024h, 004h, 02Ch, 019h, 001h, 000h
        mov    eax, dword ptr [esp + 1ch]
        push   dword ptr [esp + 20h]
        mov    word ptr [edx + 4ch], ax
        mov    ecx, dword ptr [edx + 14h]
        push   dword ptr [ecx + 40h]
        push   dword ptr [edx + 28h]
        push   eax
        push   dword ptr [esp + 28h]
        lea    eax, [esp + 14h]
        push   eax
        call   sub_1_3A30
        add    esp, 28h
        ret
        db  08Dh, 049h, 000h
sub_1_1192C:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 1ch]
        mov    edi, dword ptr [esi + 14h]
        db  080h, 03Dh, 030h, 032h, 001h, 000h, 000h
        jne    SHORT loc_1_11964
        mov    eax, dword ptr [edi + 38h]
        cmp    eax, 8
        jne    SHORT loc_1_11960
        cmp    word ptr [esi + 4ch], 4
        jne    SHORT loc_1_11960
        mov    dword ptr [edi + 38h], 9
        sub    bl, bl
        jmp    SHORT loc_1_11974
        db  08Dh, 049h, 000h
loc_1_11960:
        or     eax, eax
        je     SHORT loc_1_11970
loc_1_11964:
        mov    bl, 1
        mov    dword ptr [edi + 38h], 0
        jmp    SHORT loc_1_11974
        db  090h
loc_1_11970:
        mov    bl, byte ptr [esp + 13h]
loc_1_11974:
        mov    eax, dword ptr [edi + 0ch]
        or     eax, eax
        je     SHORT loc_1_119A0
        mov    ecx, dword ptr [eax + 58h]
        or     ecx, ecx
        je     SHORT loc_1_119A0
        mov    dword ptr [eax + 58h], 0
        cmp    byte ptr [eax + 6fh], 2
        jne    SHORT loc_1_11998
        mov    eax, 14h
        jmp    SHORT loc_1_119A6
        db  08Bh, 0FFh
loc_1_11998:
        mov    eax, 16h
        jmp    SHORT loc_1_119A6
        db  090h
loc_1_119A0:
        sub    ecx, ecx
        mov    eax, dword ptr [esp + 14h]
loc_1_119A6:
        mov    ebp, dword ptr [edi + 40h]
        or     ebp, ebp
        je     SHORT loc_1_119B8
        mov    dword ptr [edi + 40h], 0
        jmp    SHORT loc_1_119BA
        db  08Bh, 0FFh
loc_1_119B8:
        sub    ebp, ebp
loc_1_119BA:
        or     ecx, ecx
        je     SHORT loc_1_119CA
        push   0
        push   eax
        push   ecx
        call   sub_1_25A0
        add    esp, 0ch
loc_1_119CA:
        or     ebp, ebp
        je     SHORT loc_1_119DD
        push   0
        push   dword ptr [esp + 24h]
        push   ebp
        call   sub_1_25A0
        add    esp, 0ch
loc_1_119DD:
        or     bl, bl
        je     SHORT loc_1_119F6
        push   dword ptr [edi + 18h]
        db  068h, 030h, 079h, 000h, 000h
        push   0
        push   edi
        push   0
        call   sub_1_3C00
        add    esp, 14h
loc_1_119F6:
        push   4
        push   esi
        call   sub_1_D5C0
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
sub_1_11A0C:
        sub    esp, 3ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 54h]
        push   esi
        mov    edi, dword ptr [esp + 54h]
        push   edi
        call   sub_1_12000
        add    esp, 8
        cmp    al, 1
        jne    SHORT loc_1_11A34
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 3ch
        ret
        db  090h
loc_1_11A34:
        push   edi
        call   sub_1_0620
        add    esp, 4
        mov    dword ptr [esp + 18h], eax
        or     eax, eax
        jne    SHORT loc_1_11A50
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 3ch
        ret
        db  090h
loc_1_11A50:
        mov    ebx, dword ptr [esp + 5ch]
        mov    byte ptr [ebx], 0
        mov    dword ptr [esp + 1ch], 0
loc_1_11A5F:
        sub    ebp, ebp
loc_1_11A61:
        lea    eax, [esp + 20h]
        push   eax
        push   dword ptr [esp + 1ch]
        call   sub_1_0510
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_1_11BC8
        mov    dword ptr [esp + 10h], 5
        lea    ecx, [esp + 10h]
        push   ecx
        lea    ecx, [esp + 3ch]
        push   ecx
        push   eax
        call   sub_1_11C8C
        add    esp, 0ch
        mov    dword ptr [esp + 14h], eax
        shl    eax, 1ch
        sar    eax, 1ch
        cmp    eax, 1
        jb     SHORT loc_1_11A61
        cmp    eax, 2
        jbe    SHORT loc_1_11AC4
        cmp    eax, 3
        je     NEAR PTR loc_1_11B60
        cmp    eax, 4
        je     NEAR PTR loc_1_11BB8
        cmp    eax, 5
        je     SHORT loc_1_11A5F
        jmp    SHORT loc_1_11A61
        db  08Bh, 0FFh
loc_1_11AC4:
        test   byte ptr [esp + 14h], 10h
        jne    SHORT loc_1_11A61
        mov    eax, dword ptr [esp + 10h]
        dec    eax
        cmp    eax, 1
        jl     SHORT loc_1_11A61
        mov    edi, dword ptr [esp + 3ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        cmp    ecx, 10h
        jne    SHORT loc_1_11B04
        push   10h
        push   esi
        push   dword ptr [esp + 44h]
        call   sub_1_0262
        add    esp, 0ch
        cmp    eax, 10h
        je     SHORT loc_1_11B30
        jmp    NEAR PTR loc_1_11A61
        db  090h
loc_1_11B04:
        push   0
        push   dword ptr [esp + 40h]
        lea    eax, [esp + 2ch]
        push   eax
        call   sub_1_122EC
        add    esp, 0ch
        push   0fh
        push   esi
        lea    eax, [esp + 2ch]
        push   eax
        call   sub_1_0262
        add    esp, 0ch
        cmp    eax, 0fh
        jne    NEAR PTR loc_1_11A61
loc_1_11B30:
        test   byte ptr [esp + 14h], 20h
        je     SHORT loc_1_11B3A
        mov    byte ptr [ebx], 1
loc_1_11B3A:
        lea    eax, [esp + 1ch]
        push   eax
        push   dword ptr [esp + 3ch]
        call   sub_1_FD20
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_11BC8
        cmp    eax, 0ffh
        je     SHORT loc_1_11BC8
        mov    dword ptr [esp + 1ch], 0
        jmp    SHORT loc_1_11BC8
loc_1_11B60:
        cmp    byte ptr [esp + 58h], 0
        je     NEAR PTR loc_1_11A61
        cmp    ebp, 2
        je     NEAR PTR loc_1_11A61
        cmp    dword ptr [esp + 10h], 2
        jl     NEAR PTR loc_1_11A61
        push   ebx
        push   esi
        db  068h, 00Ch, 01Ah, 001h, 000h
        push   dword ptr [esp + 48h]
        call   sub_1_1239C
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_11BAC
        cmp    ebp, 1
        jne    NEAR PTR loc_1_11A61
        mov    ebp, 2
        jmp    NEAR PTR loc_1_11A61
        db  08Dh, 049h, 000h
loc_1_11BAC:
        cmp    eax, -1
        jne    SHORT loc_1_11BC4
        jmp    NEAR PTR loc_1_11A61
        db  08Bh, 0FFh
loc_1_11BB8:
        mov    ebp, 1
        jmp    NEAR PTR loc_1_11A61
        db  08Bh, 0FFh
loc_1_11BC4:
        mov    dword ptr [esp + 1ch], eax
loc_1_11BC8:
        push   dword ptr [esp + 18h]
        call   sub_1_04E0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_11BE2
        cmp    eax, 0ffh
        je     SHORT loc_1_11BE2
        mov    byte ptr [ebx], 0
loc_1_11BE2:
        mov    eax, dword ptr [esp + 1ch]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 3ch
        ret
        db  08Bh, 0FFh
sub_1_11BF0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esi]
        lea    edx, [edi + 1]
        movsx  eax, byte ptr [edx]
        cmp    eax, 5ch
        je     SHORT loc_1_11C80
        sub    eax, eax
        sub    ecx, ecx
        inc    edx
        cmp    byte ptr [edx], 78h
        je     SHORT loc_1_11C1C
        mov    dword ptr [esi], edx
        mov    eax, 0ffffffffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_11C1C:
        inc    edx
        sub    ebp, ebp
        nop
loc_1_11C20:
        mov    bl, byte ptr [edx]
        or     bl, bl
        je     SHORT loc_1_11C63
        cmp    bl, 20h
        je     SHORT loc_1_11C63
        cmp    bl, 61h
        jl     SHORT loc_1_11C3C
        movsx  ebx, bl
        shl    eax, 4
        lea    eax, [ebx + eax - 57h]
        jmp    SHORT loc_1_11C5B
loc_1_11C3C:
        cmp    bl, 41h
        jl     SHORT loc_1_11C50
        movsx  ebx, bl
        shl    eax, 4
        lea    eax, [ebx + eax - 37h]
        jmp    SHORT loc_1_11C5B
        db  08Dh, 049h, 000h
loc_1_11C50:
        sub    eax, 3
        shl    eax, 4
        movsx  ebx, bl
        add    eax, ebx
loc_1_11C5B:
        inc    edx
        inc    ecx
        inc    ebp
        cmp    ebp, 2
        jl     SHORT loc_1_11C20
loc_1_11C63:
        cmp    ecx, 1
        jge    SHORT loc_1_11C74
        mov    dword ptr [esi], edx
        mov    eax, 0ffffffffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_11C74:
        add    ecx, edi
        add    ecx, 2
        mov    dword ptr [esi], ecx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_11C80:
        mov    dword ptr [esi], edx
        mov    eax, 5ch
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_11C8C:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 2ch]
        mov    eax, dword ptr [eax]
        lea    ecx, [eax*4]
        mov    edi, dword ptr [esp + 28h]
        sub    eax, eax
        shr    ecx, 2
        rep stosd
        sub    ebp, ebp
        mov    eax, dword ptr [esp + 14h]
        and    eax, 0fffffff1h
        or     eax, 1
        mov    dword ptr [esp + 14h], eax
        and    dword ptr [esp + 14h], 0ffffffefh
        and    dword ptr [esp + 14h], 0ffffffdfh
        mov    eax, dword ptr [esp + 2ch]
        mov    edi, dword ptr [eax]
        sub    esi, esi
        mov    dword ptr [esp + 18h], ebp
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [esp + 10h], eax
        cmp    byte ptr [eax], 0
        je     NEAR PTR loc_1_11F42
        mov    ebx, dword ptr [esp + 1ch]
loc_1_11CE6:
        mov    eax, dword ptr [esp + 10h]
        mov    al, byte ptr [eax]
        movzx  ecx, al
        cmp    ecx, 20h
        jg     SHORT loc_1_11D20
        je     SHORT loc_1_11D3C
        cmp    ecx, 9
        je     SHORT loc_1_11D3C
        cmp    ecx, 0ah
        je     NEAR PTR loc_1_11F31
        cmp    ecx, 0dh
        je     NEAR PTR loc_1_11F31
loc_1_11D0D:
        cmp    dword ptr [esp + 18h], 0
        je     NEAR PTR loc_1_11E88
        mov    byte ptr [ebx], al
        inc    ebx
        jmp    NEAR PTR loc_1_11EB2
loc_1_11D20:
        cmp    ecx, 22h
        je     SHORT loc_1_11D70
        cmp    ecx, 23h
        je     NEAR PTR loc_1_11DD8
        cmp    ecx, 5ch
        je     NEAR PTR loc_1_11E64
        jmp    SHORT loc_1_11D0D
        db  08Dh, 049h, 000h
loc_1_11D3C:
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_1_11D4C
        mov    byte ptr [ebx], al
        inc    ebx
        jmp    NEAR PTR loc_1_11EB2
        db  090h
loc_1_11D4C:
        cmp    ebp, 2
        jne    NEAR PTR loc_1_11EB2
        mov    ebp, 1
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [eax], 0
        cmp    edi, esi
        jne    NEAR PTR loc_1_11EB2
        jmp    NEAR PTR loc_1_11F42
        db  08Bh, 0FFh
loc_1_11D70:
        cmp    ebp, 2
        jne    SHORT loc_1_11DA0
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_1_11DA0
        mov    ebp, 1
        mov    dword ptr [esp + 18h], 0
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [eax], 0
        mov    byte ptr [ebx], 0
        cmp    edi, esi
        jne    NEAR PTR loc_1_11EB2
        jmp    NEAR PTR loc_1_11F42
loc_1_11DA0:
        mov    ebp, 2
        mov    dword ptr [esp + 18h], 1
        mov    eax, dword ptr [esp + 2ch]
        cmp    dword ptr [eax], edi
        jl     NEAR PTR loc_1_11EC8
        cmp    edi, esi
        jle    NEAR PTR loc_1_11EE0
        mov    eax, dword ptr [esp + 10h]
        inc    eax
        mov    ecx, dword ptr [esp + 28h]
        mov    dword ptr [ecx + esi*4], eax
        inc    esi
        mov    ebx, dword ptr [esp + 10h]
        inc    ebx
        jmp    NEAR PTR loc_1_11EB2
loc_1_11DD8:
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_1_11DE8
        mov    byte ptr [ebx], al
        inc    ebx
        jmp    NEAR PTR loc_1_11EB2
        db  090h
loc_1_11DE8:
        or     ebp, ebp
        db  0B8h, 0C8h, 031h, 001h, 000h
        je     SHORT loc_1_11DF6
        db  0B8h, 058h, 031h, 001h, 000h
loc_1_11DF6:
        push   eax
        push   dword ptr [esp + 14h]
        call   sub_1_11FB0
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_1_11F28
        mov    ebp, 2
        mov    edi, dword ptr [eax + 0ch]
        mov    edx, dword ptr [eax + 8]
        cmp    edx, 4000h
        je     SHORT loc_1_11E54
        cmp    edx, 8000h
        je     SHORT loc_1_11E5C
        mov    eax, dword ptr [esp + 2ch]
        cmp    dword ptr [eax], edi
        jl     NEAR PTR loc_1_11EF8
        cmp    edi, esi
        jle    NEAR PTR loc_1_11F10
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 28h]
        mov    dword ptr [ecx + esi*4], eax
        inc    esi
        xor    edx, dword ptr [esp + 14h]
        and    edx, 0fh
        xor    dword ptr [esp + 14h], edx
        jmp    SHORT loc_1_11EB2
        db  090h
loc_1_11E54:
        or     dword ptr [esp + 14h], 20h
        jmp    SHORT loc_1_11EB2
        db  090h
loc_1_11E5C:
        or     dword ptr [esp + 14h], 10h
        jmp    SHORT loc_1_11EB2
        db  090h
loc_1_11E64:
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_1_11E88
        lea    eax, [esp + 10h]
        push   eax
        call   sub_1_11BF0
        add    esp, 4
        cmp    eax, -1
        je     NEAR PTR loc_1_11F68
        mov    byte ptr [ebx], al
        inc    ebx
        jmp    SHORT loc_1_11EB2
        db  08Bh, 0FFh
loc_1_11E88:
        cmp    ebp, 2
        je     SHORT loc_1_11EB2
        mov    ebp, 2
        mov    eax, dword ptr [esp + 2ch]
        cmp    dword ptr [eax], edi
        jl     NEAR PTR loc_1_11F80
        cmp    edi, esi
        jle    NEAR PTR loc_1_11F98
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 28h]
        mov    dword ptr [ecx + esi*4], eax
        inc    esi
loc_1_11EB2:
        inc    dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 10h]
        cmp    byte ptr [eax], 0
        jne    NEAR PTR loc_1_11CE6
        jmp    NEAR PTR loc_1_11F42
loc_1_11EC8:
        mov    eax, dword ptr [esp + 14h]
        and    eax, 0fffffff6h
        or     eax, 6
        mov    dword ptr [esp + 14h], eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Bh, 0FFh
loc_1_11EE0:
        mov    eax, dword ptr [esp + 14h]
        and    eax, 0fffffff6h
        or     eax, 6
        mov    dword ptr [esp + 14h], eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Bh, 0FFh
loc_1_11EF8:
        mov    eax, dword ptr [esp + 14h]
        and    eax, 0fffffff6h
        or     eax, 6
        mov    dword ptr [esp + 14h], eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Bh, 0FFh
loc_1_11F10:
        mov    eax, dword ptr [esp + 14h]
        and    eax, 0fffffff6h
        or     eax, 6
        mov    dword ptr [esp + 14h], eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Bh, 0FFh
loc_1_11F28:
        or     ebp, ebp
        jne    SHORT loc_1_11F31
        and    dword ptr [esp + 14h], 0fffffff0h
loc_1_11F31:
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [eax], 0
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_1_11F42
        mov    byte ptr [ebx], 0
loc_1_11F42:
        cmp    esi, 1
        jg     SHORT loc_1_11F55
        mov    eax, dword ptr [esp + 14h]
        and    eax, 0fffffff6h
        or     eax, 6
        mov    dword ptr [esp + 14h], eax
loc_1_11F55:
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [eax], esi
        mov    eax, dword ptr [esp + 14h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h
loc_1_11F68:
        mov    eax, dword ptr [esp + 14h]
        and    eax, 0fffffff6h
        or     eax, 6
        mov    dword ptr [esp + 14h], eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Bh, 0FFh
loc_1_11F80:
        mov    eax, dword ptr [esp + 14h]
        and    eax, 0fffffff6h
        or     eax, 6
        mov    dword ptr [esp + 14h], eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Bh, 0FFh
loc_1_11F98:
        mov    eax, dword ptr [esp + 14h]
        and    eax, 0fffffff6h
        or     eax, 6
        mov    dword ptr [esp + 14h], eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Bh, 0FFh
sub_1_11FB0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 14h]
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    esi, [ecx - 1]
        mov    edi, dword ptr [esp + 18h]
        cmp    dword ptr [edi], 0
        je     SHORT loc_1_11FF0
loc_1_11FD1:
        mov    ebp, dword ptr [edi + 4]
        cmp    esi, ebp
        jb     SHORT loc_1_11FE8
        push   ebp
        push   dword ptr [edi]
        push   ebx
        call   sub_1_0262
        add    esp, 0ch
        cmp    eax, ebp
        je     SHORT loc_1_11FF8
loc_1_11FE8:
        add    edi, 10h
        cmp    dword ptr [edi], 0
        jne    SHORT loc_1_11FD1
loc_1_11FF0:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_11FF8:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
sub_1_12000:
        sub    esp, 48h
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    bl, bl
        mov    esi, dword ptr [esp + 5ch]
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    edx, [ecx - 1]
        db  0BFh, 00Ch, 032h, 001h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        cmp    edx, ecx
        jbe    NEAR PTR loc_1_120FD
        db  0BFh, 00Ch, 032h, 001h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        push   ecx
        db  068h, 00Ch, 032h, 001h, 000h
        push   esi
        call   sub_1_0262
        add    esp, 0ch
        mov    edx, eax
        db  0BFh, 00Ch, 032h, 001h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        cmp    edx, ecx
        jne    NEAR PTR loc_1_120FD
        db  0BFh, 00Ch, 032h, 001h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    ebp, [ecx + esi - 1]
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    edi, [ecx + esi - 2]
        push   5ch
        push   ebp
        call   sub_1_12CE8
        add    esp, 8
        mov    esi, eax
        cmp    edi, esi
        je     SHORT loc_1_120FD
        mov    byte ptr [esi], bl
        push   0
        push   ebp
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_1_122EC
        add    esp, 0ch
        push   0fh
        push   dword ptr [esp + 64h]
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_1_0262
        add    esp, 0ch
        cmp    eax, 0fh
        jne    SHORT loc_1_120D4
        jmp    SHORT loc_1_120F8
        db  090h
loc_1_120D4:
        lea    eax, [esp + 12h]
        push   eax
        db  0FFh, 035h, 0D8h, 032h, 001h, 000h
        lea    eax, [esp + 1ch]
        push   eax
        push   1
        call   sub_1_DFA0
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_120F8
        test   byte ptr [eax + 21h], 8
        jne    SHORT loc_1_120FA
loc_1_120F8:
        mov    bl, 1
loc_1_120FA:
        mov    byte ptr [esi], 5ch
loc_1_120FD:
        mov    al, bl
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 48h
        ret
        db  090h
sub_1_12108:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 14h]
        mov    ebx, edi
        mov    esi, dword ptr [esp + 10h]
loc_1_12115:
        mov    ebx, dword ptr [ebx]
        cmp    ebx, edi
        je     SHORT loc_1_12138
        push   10h
        push   esi
        lea    eax, [ebx + 34h]
        push   eax
        call   sub_1_0262
        add    esp, 0ch
        cmp    eax, 10h
        jne    SHORT loc_1_12115
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_12138:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
sub_1_12140:
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        mov    ecx, dword ptr [esp + 1ch]
        cmp    dword ptr [ecx], ecx
        je     NEAR PTR loc_1_12206
        push   ecx
        push   dword ptr [esp + 18h]
        call   sub_1_12108
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_1_12206
        mov    ax, word ptr [ebx + 8]
        mov    dx, word ptr [ebx + 0ah]
        cmp    ax, dx
        jbe    SHORT loc_1_121A0
        mov    eax, dword ptr [ebx + 0ch]
        cmp    dword ptr [eax], -1
        je     SHORT loc_1_12186
loc_1_1217E:
        add    eax, 4
        cmp    dword ptr [eax], -1
        jne    SHORT loc_1_1217E
loc_1_12186:
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], 0ffffffffh
        add    dx, 4
        mov    word ptr [ebx + 0ah], dx
        jmp    SHORT loc_1_12206
        db  08Dh, 049h, 000h
loc_1_121A0:
        add    ax, 18h
        push   eax
        call   sub_1_35F0
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_1_12206
        movzx  eax, word ptr [ebx + 8]
        mov    edi, ebp
        mov    esi, dword ptr [ebx + 0ch]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        push   dword ptr [ebx + 0ch]
        call   sub_1_0024
        add    esp, 4
        mov    dword ptr [ebx + 0ch], ebp
        mov    ax, word ptr [ebx + 8]
        movzx  ecx, ax
        add    ebp, ecx
        sub    ebp, 4
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [ebp], ecx
        mov    dword ptr [ebp + 4], 0ffffffffh
        add    word ptr [ebx + 0ah], 4
        add    ax, 18h
        mov    word ptr [ebx + 8], ax
        mov    dword ptr [ebx + 24h], 0facf0000h
loc_1_12206:
        or     ebx, ebx
        jne    SHORT loc_1_12285
        push   44h
        call   sub_1_35F0
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_1_12285
        push   18h
        call   sub_1_35F0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_1227C
        lea    edi, [ebx + 34h]
        mov    esi, dword ptr [esp + 14h]
        movsd
        movsd
        movsd
        movsd
        mov    dword ptr [ebx + 0ch], eax
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], 0ffffffffh
        mov    dword ptr [ebx + 1ch], 1
        mov    dword ptr [ebx + 20h], 8
        mov    word ptr [ebx + 8], 18h
        mov    word ptr [ebx + 0ah], 8
        mov    dword ptr [ebx + 24h], 0facf0000h
        mov    ecx, dword ptr [esp + 1ch]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebx], eax
        mov    dword ptr [ebx + 4], ecx
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [ecx], ebx
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_1227C:
        push   ebx
        call   sub_1_0024
        add    esp, 4
loc_1_12285:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_1228C:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        cmp    dword ptr [esi], esi
        je     SHORT loc_1_122E9
        db  08Bh, 03Dh, 08Ch, 034h, 001h, 000h
        db  081h, 0FFh, 08Ch, 034h, 001h, 000h
        je     SHORT loc_1_122C8
loc_1_122A4:
        mov    eax, edi
        mov    edi, dword ptr [edi]
        mov    edx, edi
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
        mov    dword ptr [eax + 4], eax
        mov    dword ptr [eax], eax
        push   eax
        call   sub_1_DEA4
        add    esp, 4
        db  081h, 0FFh, 08Ch, 034h, 001h, 000h
        jne    SHORT loc_1_122A4
loc_1_122C8:
        mov    eax, dword ptr [esi]
        db  0A3h, 08Ch, 034h, 001h, 000h
        mov    eax, dword ptr [esi + 4]
        db  0A3h, 090h, 034h, 001h, 000h
        mov    eax, dword ptr [esi]
        db  0C7h, 040h, 004h, 08Ch, 034h, 001h, 000h
        mov    eax, dword ptr [esi + 4]
        db  0C7h, 000h, 08Ch, 034h, 001h, 000h
loc_1_122E9:
        pop    edi
        pop    esi
        ret
sub_1_122EC:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 18h]
        mov    dword ptr [esp + 10h], edx
        mov    ecx, dword ptr [esp + 1ch]
        mov    ebx, ecx
        lea    esi, [edx + 0fh]
        cmp    byte ptr [ecx], 0
        je     SHORT loc_1_12318
loc_1_12309:
        cmp    edx, esi
        jae    SHORT loc_1_12337
        mov    al, byte ptr [ecx]
        mov    byte ptr [edx], al
        inc    ecx
        inc    edx
        cmp    byte ptr [ecx], 0
        jne    SHORT loc_1_12309
loc_1_12318:
        cmp    edx, esi
        jae    SHORT loc_1_12337
        mov    ebp, esi
        sub    ebp, edx
        mov    edi, edx
        mov    eax, 20202020h
        mov    ecx, ebp
        shr    ecx, 2
        rep stosd
        mov    ecx, ebp
        and    ecx, 3
        rep stosb
        add    edx, ebp
loc_1_12337:
        mov    byte ptr [edx], 0
        mov    byte ptr [edx + 1], 0
        mov    ebp, dword ptr [esp + 10h]
        cmp    ebp, esi
        jae    SHORT loc_1_12383
loc_1_12346:
        mov    al, byte ptr [ebx]
        inc    ebx
        or     al, al
        je     SHORT loc_1_1235F
        cmp    al, 61h
        jl     SHORT loc_1_12357
        cmp    al, 7ah
        jg     SHORT loc_1_12357
        sub    al, 20h
loc_1_12357:
        mov    byte ptr [ebp], al
        inc    ebp
        cmp    ebp, esi
        jb     SHORT loc_1_12346
loc_1_1235F:
        cmp    ebp, esi
        jae    SHORT loc_1_12383
        mov    edx, dword ptr [esp + 10h]
        sub    edx, ebp
        add    edx, 0fh
        mov    edi, ebp
        mov    eax, 20202020h
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        add    ebp, edx
loc_1_12383:
        mov    al, byte ptr [esp + 20h]
        mov    byte ptr [ebp], al
        mov    byte ptr [ebp + 1], 0
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
_LTEXT ends
        end
