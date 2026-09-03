; ndis_pcode_04_03.asm -- NDIS object 4 segment _PTEXT  (part 3/3)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_4_A8A0:
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 14h]
        push   edi
        mov    dword ptr [esp + 0ch], 0
        mov    ecx, dword ptr [esi + 18h]
        mov    edi, dword ptr [esi + 20h]
        shr    edi, 2
        test   edi, edi
        je     SHORT loc_4_A8E2
        mov    ebx, ecx
loc_4_A8C1:
        mov    edx, dword ptr [ebx]
        mov    eax, edx
        and    eax, 0ff0000h
        cmp    eax, 20000h
        je     SHORT loc_4_A8DC
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [ecx + eax*4], edx
        inc    dword ptr [esp + 0ch]
loc_4_A8DC:
        add    ebx, 4
        dec    edi
        jne    SHORT loc_4_A8C1
loc_4_A8E2:
        mov    eax, dword ptr [esp + 14h]
        cmp    dword ptr [eax + 0fch], 7
        jne    SHORT loc_4_A903
        mov    eax, dword ptr [esi + 4]
        test   byte ptr [eax + 20h], 2
        je     SHORT loc_4_A903
        lea    eax, [esp + 0ch]
        push   eax
        push   ecx
        call   sub_4_0330
loc_4_A903:
        mov    eax, dword ptr [esp + 0ch]
        pop    edi
        shl    eax, 2
        mov    dword ptr [esi + 20h], eax
        xor    eax, eax
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_A920:
        mov    ecx, dword ptr [esp + 8]
        cmp    dword ptr [ecx + 14h], 10101h
        jne    SHORT loc_4_A96F
        cmp    dword ptr [ecx + 18h], 0
        je     SHORT loc_4_A94E
        cmp    dword ptr [ecx + 1ch], 0
        je     SHORT loc_4_A94E
        cmp    dword ptr [esp + 0ch], 0
        jne    SHORT loc_4_A94E
        mov    eax, dword ptr [esp + 4]
        push   ecx
        push   eax
        call   sub_4_A8A0
        ret    0ch
loc_4_A94E:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 0fch], 7
        jne    SHORT loc_4_A968
        mov    eax, dword ptr [ecx + 4]
        test   byte ptr [eax + 20h], 2
        je     SHORT loc_4_A968
        add    dword ptr [ecx + 24h], 8
loc_4_A968:
        mov    dword ptr [ecx + 20h], 0
loc_4_A96F:
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_A980:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        push   ebp
        mov    edi, dword ptr [esi + 1e0h]
        and    dword ptr [esi + 3ch], 0fffffdffh
        mov    dword ptr [esi + 1e0h], 0
        cmp    esi, edi
        jne    SHORT loc_4_A9BD
        mov    ebx, dword ptr [esp + 18h]
        add    esi, 1fch
        push   esi
        mov    dword ptr [esi - 18h], ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_A9B6:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_4_A9BD:
        cmp    dword ptr [edi + 10h], 2
        jne    NEAR PTR loc_4_AA62
        lea    esi, [edi - 4]
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [ecx]
        test   eax, eax
        je     SHORT loc_4_A9DE
        cmp    eax, 0eh
        je     SHORT loc_4_A9F5
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_4_A9DE:
        mov    ebx, dword ptr [esp + 18h]
        add    esi, 30h
        push   esi
        mov    dword ptr [esi - 4], ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_A9EE:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_4_A9F5:
        mov    eax, dword ptr [ecx + 8]
        test   eax, eax
        je     SHORT loc_4_AA08
        cmp    eax, 1
        je     SHORT loc_4_AA4B
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_4_AA08:
        mov    eax, dword ptr [edi + 20h]
        mov    ebx, dword ptr [esp + 18h]
        mov    dword ptr [ecx + 0ch], eax
        test   ebx, ebx
        jne    SHORT loc_4_AA1F
        mov    dword ptr [ecx + 10h], 0
        jmp    SHORT loc_4_AA35
loc_4_AA1F:
        mov    dword ptr [ecx + 10h], 80000005h
        cmp    ebx, 0c0010014h
        je     SHORT loc_4_AA35
        mov    dword ptr [ecx + 10h], 0c0000001h
loc_4_AA35:
        push   ecx
        call   dword ptr [ecx + 4]
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_AA41:
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_4_AA4B:
        mov    ebx, dword ptr [esp + 18h]
        add    esi, 30h
        push   esi
        mov    dword ptr [esi - 4], ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_AA5B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_4_AA62:
        mov    ebx, dword ptr [esp + 18h]
        push   ebx
        push   edi
        push   esi
        call   sub_4_A920
        mov    ebp, dword ptr [edi + 4]
        test   ebp, ebp
        je     SHORT loc_4_AA98
        push   ebx
        mov    eax, dword ptr [ebp + 10h]
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        push   eax
        call   dword ptr [ebx + 50h]
        mov    eax, dword ptr [ebp + 34h]
        dec    eax
        mov    dword ptr [ebp + 34h], eax
        jne    SHORT loc_4_AAA3
        push   ebp
        push   esi
        call   sub_4_7740
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_4_AA98:
        push   0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_AAA0:
        add    esp, 8
loc_4_AAA3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_AAB0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 1e0h], 0
        je     SHORT loc_4_AAE3
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        push   esi
        call   sub_4_A980
        cmp    dword ptr [esi + 1dch], 0
        je     SHORT loc_4_AAE3
        push   0
        mov    edx, 2
        push   0
        mov    ecx, esi
        call   dword ptr [esi + 11ch]
loc_4_AAE3:
        pop    esi
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_AAF0:
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 1010103h
        ja     SHORT loc_4_AB1B
        je     NEAR PTR loc_4_AC18
        cmp    eax, 1010eh
        je     SHORT loc_4_AB51
        cmp    eax, 1010fh
        je     NEAR PTR loc_4_ABF0
        pop    esi
        ret    0ch
loc_4_AB1B:
        cmp    eax, 3010103h
        ja     SHORT loc_4_AB42
        je     NEAR PTR loc_4_AC85
        cmp    eax, 2010103h
        je     NEAR PTR loc_4_AC3D
        cmp    eax, 2010104h
        je     NEAR PTR loc_4_AC63
        pop    esi
        ret    0ch
loc_4_AB42:
        cmp    eax, 3010107h
        je     NEAR PTR loc_4_ACAA
        pop    esi
        ret    0ch
loc_4_AB51:
        cmp    dword ptr [esp + 10h], 0
        je     NEAR PTR loc_4_ACCB
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [ecx + 0fch]
        cmp    eax, 7
        ja     NEAR PTR loc_4_ACCB
        xor    edx, edx
        db  08Ah, 090h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 095h, 000h, 000h, 000h, 000h
loc_4_AB7E:
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [ecx + 0d8h]
        mov    edx, dword ptr [eax + 28h]
        push   edx
        push   ecx
        call   sub_4_0C10
        pop    esi
        ret    0ch
loc_4_AB95:
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [ecx + 0dch]
        mov    edx, dword ptr [eax + 28h]
        push   edx
        push   ecx
        call   sub_4_14F0
        pop    esi
        ret    0ch
loc_4_ABAC:
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [ecx + 0e0h]
        mov    edx, dword ptr [eax + 28h]
        push   edx
        push   ecx
        call   sub_4_2280
        pop    esi
        ret    0ch
loc_4_ABC3:
        mov    eax, dword ptr [esi + 4]
        test   byte ptr [eax + 20h], 2
        mov    eax, dword ptr [eax + 28h]
        push   eax
        je     SHORT loc_4_ABE0
        mov    ecx, dword ptr [ecx + 0d8h]
        push   ecx
        call   sub_4_0C10
        pop    esi
        ret    0ch
loc_4_ABE0:
        mov    ecx, dword ptr [ecx + 0e4h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_ABEC:
        pop    esi
        ret    0ch
loc_4_ABF0:
        cmp    dword ptr [esp + 10h], 0
        jne    NEAR PTR loc_4_ACCB
        push   4
        mov    eax, dword ptr [esi + 18h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        add    eax, 38h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_AC0D:
        mov    dword ptr [esi + 20h], 4
        pop    esi
        ret    0ch
loc_4_AC18:
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_4_AC33
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [ecx + 0d8h]
        push   eax
        call   sub_4_07B0
        pop    esi
        ret    0ch
loc_4_AC33:
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [esi + 20h], eax
        pop    esi
        ret    0ch
loc_4_AC3D:
        cmp    dword ptr [esp + 10h], 0
        je     NEAR PTR loc_4_ACCB
        mov    eax, dword ptr [esi + 4]
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax + 28h]
        mov    eax, dword ptr [edx + 0dch]
        push   ecx
        push   eax
        call   sub_4_1110
        pop    esi
        ret    0ch
loc_4_AC63:
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_4_ACCB
        mov    eax, dword ptr [esi + 4]
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax + 28h]
        mov    eax, dword ptr [edx + 0dch]
        push   ecx
        push   eax
        call   sub_4_11D0
        pop    esi
        ret    0ch
loc_4_AC85:
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_4_ACA0
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [ecx + 0e0h]
        push   eax
        call   sub_4_1BD0
        pop    esi
        ret    0ch
loc_4_ACA0:
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [esi + 20h], eax
        pop    esi
        ret    0ch
loc_4_ACAA:
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_4_ACC5
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [ecx + 0e0h]
        push   eax
        call   sub_4_1E50
        pop    esi
        ret    0ch
loc_4_ACC5:
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [esi + 20h], eax
loc_4_ACCB:
        pop    esi
        ret    0ch
        db  090h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 001h, 002h, 004h, 004h, 004h, 004h, 003h, 08Dh, 064h, 024h
        db  000h
sub_4_ACF0:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        push   ebp
        mov    ebp, dword ptr [esi + 1e0h]
        and    dword ptr [esi + 3ch], 0fffffdffh
        mov    dword ptr [esi + 1e0h], 0
        mov    edi, dword ptr [ebp + 4]
        test   edi, edi
        je     SHORT loc_4_AD4B
        test   byte ptr [edi + 20h], 1
        jne    SHORT loc_4_AD34
        mov    ebx, dword ptr [esp + 18h]
        push   ebx
        push   ebp
        push   esi
        call   sub_4_AAF0
        push   ebx
        mov    eax, dword ptr [edi + 10h]
        push   ebp
        push   eax
        mov    ebp, dword ptr [edi + 8]
        call   dword ptr [ebp + 50h]
loc_4_AD34:
        mov    eax, dword ptr [edi + 34h]
        dec    eax
        mov    dword ptr [edi + 34h], eax
        jne    SHORT loc_4_AD65
        push   edi
        push   esi
        call   sub_4_7740
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_4_AD4B:
        cmp    dword ptr [esi + 1dch], 0
        jne    SHORT loc_4_AD5A
        push   esi
        call   sub_4_6FE0
loc_4_AD5A:
        push   0
        push   ebp
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_AD62:
        add    esp, 8
loc_4_AD65:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_4_AD70:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 1e0h]
        test   eax, eax
        je     SHORT loc_4_ADA8
        cmp    esi, eax
        je     SHORT loc_4_ADA8
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        push   esi
        call   sub_4_ACF0
        cmp    dword ptr [esi + 1dch], 0
        je     SHORT loc_4_ADA8
        push   0
        mov    edx, 2
        push   0
        mov    ecx, esi
        call   dword ptr [esi + 11ch]
loc_4_ADA8:
        pop    esi
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_4_ADB0:
        mov    ecx, dword ptr [esp + 4]
        push   esi
        lea    esi, [ecx - 4]
        mov    edx, dword ptr [esi]
        mov    eax, dword ptr [edx]
        test   eax, eax
        je     SHORT loc_4_ADC9
        cmp    eax, 0eh
        je     SHORT loc_4_ADDD
        pop    esi
        ret    8
loc_4_ADC9:
        mov    eax, dword ptr [esp + 0ch]
        add    esi, 30h
        push   esi
        mov    dword ptr [esi - 4], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_ADD9:
        pop    esi
        ret    8
loc_4_ADDD:
        mov    eax, dword ptr [edx + 8]
        test   eax, eax
        je     SHORT loc_4_ADED
        cmp    eax, 1
        je     SHORT loc_4_AE2C
        pop    esi
        ret    8
loc_4_ADED:
        mov    eax, dword ptr [ecx + 20h]
        mov    dword ptr [edx + 0ch], eax
        mov    eax, dword ptr [esp + 0ch]
        test   eax, eax
        jne    SHORT loc_4_AE04
        mov    dword ptr [edx + 10h], 0
        jmp    SHORT loc_4_AE19
loc_4_AE04:
        mov    dword ptr [edx + 10h], 80000005h
        cmp    eax, 0c0010014h
        je     SHORT loc_4_AE19
        mov    dword ptr [edx + 10h], 0c0000001h
loc_4_AE19:
        push   edx
        call   dword ptr [edx + 4]
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_AE25:
        add    esp, 8
        pop    esi
        ret    8
loc_4_AE2C:
        mov    eax, dword ptr [esp + 0ch]
        add    esi, 30h
        push   esi
        mov    dword ptr [esi - 4], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_AE3C:
        pop    esi
        ret    8
sub_4_AE40:
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 14h]
        push   edi
        push   ebp
        mov    ebx, 0
        cmp    dword ptr [esi + 1ch], 4
        jae    SHORT loc_4_AE62
        mov    dword ptr [esi + 24h], 4
        mov    ebx, 0c0010014h
loc_4_AE62:
        test   ebx, ebx
        je     SHORT loc_4_AE72
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
loc_4_AE72:
        push   4
        mov    eax, dword ptr [esi + 18h]
        lea    ecx, [esp + 14h]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_AE82:
        mov    ecx, dword ptr [esi + 4]
        lea    edx, [ecx + 20h]
        mov    eax, dword ptr [edx]
        test   al, 1
        je     SHORT loc_4_AE9C
        mov    ebx, 103h
        mov    edi, dword ptr [esp + 18h]
        jmp    NEAR PTR loc_4_AFA7
loc_4_AE9C:
        mov    edi, dword ptr [esp + 18h]
        mov    ebp, dword ptr [edi + 0fch]
        cmp    ebp, 8
        ja     NEAR PTR loc_4_AFA7
        db  0FFh, 024h, 0ADh, 000h, 000h, 000h, 000h
loc_4_AEB6:
        mov    eax, dword ptr [esp + 10h]
        push   1
        push   eax
        mov    ecx, dword ptr [ecx + 28h]
        push   esi
        mov    edx, dword ptr [edi + 0d8h]
        push   ecx
        push   edx
        call   sub_4_0C30
        mov    ebx, eax
        mov    ecx, dword ptr [edi + 0d8h]
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [esp + 10h], eax
        jmp    NEAR PTR loc_4_AFA7
loc_4_AEE2:
        mov    eax, dword ptr [esp + 10h]
        push   1
        push   eax
        mov    ecx, dword ptr [ecx + 28h]
        push   esi
        mov    edx, dword ptr [edi + 0dch]
        push   ecx
        push   edx
        call   sub_4_1510
        mov    ebx, eax
        mov    ecx, dword ptr [edi + 0dch]
        mov    eax, dword ptr [ecx + 10h]
        mov    dword ptr [esp + 10h], eax
        jmp    NEAR PTR loc_4_AFA7
loc_4_AF0E:
        mov    eax, dword ptr [esp + 10h]
        push   1
        push   eax
        mov    ecx, dword ptr [ecx + 28h]
        push   esi
        mov    edx, dword ptr [edi + 0e0h]
        push   ecx
        push   edx
        call   sub_4_22A0
        mov    ebx, eax
        mov    ecx, dword ptr [edi + 0e0h]
        mov    eax, dword ptr [ecx + 14h]
        mov    dword ptr [esp + 10h], eax
        jmp    SHORT loc_4_AFA7
loc_4_AF37:
        push   1
        mov    ecx, dword ptr [ecx + 28h]
        test   al, 2
        mov    eax, dword ptr [esp + 14h]
        push   eax
        push   esi
        push   ecx
        je     SHORT loc_4_AF55
        mov    edx, dword ptr [edi + 0d8h]
        push   edx
        call   sub_4_0C30
        jmp    SHORT loc_4_AF61
loc_4_AF55:
        mov    edx, dword ptr [edi + 0e4h]
        push   edx
        call   sub_4_02B0
loc_4_AF61:
        mov    ebx, eax
        mov    edx, dword ptr [edi + 0d8h]
        mov    eax, dword ptr [edi + 0e4h]
        mov    ecx, dword ptr [eax + 8]
        mov    dword ptr [esp + 10h], ecx
        mov    eax, dword ptr [edx + 0ch]
        or     dword ptr [esp + 10h], eax
        test   byte ptr [edi + 3ch], 4
        jne    SHORT loc_4_AF8A
        test   byte ptr [esp + 10h], 2
        je     SHORT loc_4_AFA7
loc_4_AF8A:
        and    dword ptr [esp + 10h], 0fffffffdh
        or     dword ptr [esp + 10h], 8
        jmp    SHORT loc_4_AFA7
loc_4_AF96:
        test   byte ptr [esp + 10h], 0a0h
        je     SHORT loc_4_AFA5
        or     eax, 8
        mov    dword ptr [edx], eax
        inc    byte ptr [edi + 2fh]
loc_4_AFA5:
        xor    ebx, ebx
loc_4_AFA7:
        cmp    ebx, 103h
        jne    SHORT loc_4_AFFC
        lea    ecx, [edi + 20ch]
        mov    eax, dword ptr [esp + 10h]
        and    eax, 0ffffff7fh
        test   byte ptr [esp + 10h], 80h
        mov    dword ptr [ecx], eax
        je     SHORT loc_4_AFD9
        test   byte ptr [edi + 1f8h], 8
        jne    SHORT loc_4_AFD9
        or     dword ptr [edi + 1a4h], 8
        jmp    SHORT loc_4_AFE0
loc_4_AFD9:
        and    dword ptr [edi + 1a4h], 0fffffff7h
loc_4_AFE0:
        lea    eax, [esi + 24h]
        lea    edx, [esi + 20h]
        push   eax
        push   edx
        push   4
        mov    eax, dword ptr [edi + 8]
        push   ecx
        push   1010eh
        mov    ecx, dword ptr [edi + 0ch]
        push   ecx
        call   dword ptr [eax + 4ch]
        mov    ebx, eax
loc_4_AFFC:
        test   ebx, ebx
        jne    SHORT loc_4_B009
        mov    dword ptr [esi + 20h], 4
        jmp    SHORT loc_4_B019
loc_4_B009:
        cmp    ebx, 103h
        je     SHORT loc_4_B019
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
loc_4_B019:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
        db  08Dh, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 08Dh, 064h, 024h, 000h
sub_4_B050:
        sub    esp, 8
        mov    eax, 0
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 18h]
        push   edi
        cmp    dword ptr [esi + 1ch], 4
        jae    SHORT loc_4_B071
        mov    dword ptr [esi + 24h], 4
        mov    eax, 0c0010014h
loc_4_B071:
        test   eax, eax
        jne    NEAR PTR loc_4_B157
        push   4
        mov    eax, dword ptr [esi + 18h]
        lea    ecx, [esp + 14h]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_B089:
        mov    edi, dword ptr [esp + 18h]
        mov    edx, dword ptr [edi + 1f4h]
        cmp    dword ptr [esp + 10h], edx
        jbe    SHORT loc_4_B0AF
        xor    eax, eax
        pop    edi
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        mov    eax, 0c0010014h
        pop    esi
        pop    ebx
        add    esp, 8
        ret    8
loc_4_B0AF:
        mov    dword ptr [esp + 0ch], 0
        mov    eax, dword ptr [edi + 18h]
        test   eax, eax
        je     SHORT loc_4_B0D2
loc_4_B0BE:
        mov    ecx, dword ptr [eax + 38h]
        cmp    ecx, dword ptr [esp + 0ch]
        jbe    SHORT loc_4_B0CB
        mov    dword ptr [esp + 0ch], ecx
loc_4_B0CB:
        mov    eax, dword ptr [eax + 18h]
        test   eax, eax
        jne    SHORT loc_4_B0BE
loc_4_B0D2:
        mov    eax, dword ptr [esp + 0ch]
        cmp    dword ptr [esp + 10h], eax
        jbe    SHORT loc_4_B0E4
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esp + 0ch], eax
loc_4_B0E4:
        cmp    dword ptr [esp + 0ch], 0
        jne    SHORT loc_4_B0EF
        mov    dword ptr [esp + 0ch], edx
loc_4_B0EF:
        xor    eax, eax
        mov    ecx, dword ptr [esp + 0ch]
        cmp    dword ptr [edi + 1f0h], ecx
        je     SHORT loc_4_B12A
        lea    ebx, [edi + 20ch]
        push   4
        lea    eax, [esp + 10h]
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_B110:
        lea    eax, [esi + 24h]
        lea    ecx, [esi + 20h]
        push   eax
        mov    edx, dword ptr [edi + 0ch]
        push   ecx
        mov    eax, dword ptr [edi + 8]
        push   4
        push   ebx
        push   1010fh
        push   edx
        call   dword ptr [eax + 4ch]
loc_4_B12A:
        test   eax, eax
        jne    SHORT loc_4_B148
        mov    ecx, dword ptr [esp + 10h]
        mov    edx, dword ptr [esi + 4]
        pop    edi
        mov    dword ptr [edx + 38h], ecx
        mov    dword ptr [esi + 20h], 4
        pop    esi
        pop    ebx
        add    esp, 8
        ret    8
loc_4_B148:
        cmp    eax, 103h
        je     SHORT loc_4_B157
        xor    ecx, ecx
        mov    dword ptr [esi + 20h], ecx
        mov    dword ptr [esi + 24h], ecx
loc_4_B157:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    8
sub_4_B160:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 1ch]
        mov    eax, dword ptr [edi + 0fch]
        test   eax, eax
        je     SHORT loc_4_B19E
        mov    esi, dword ptr [esp + 20h]
        cmp    eax, 7
        jne    SHORT loc_4_B187
        mov    eax, dword ptr [esi + 4]
        test   byte ptr [eax + 20h], 2
        jne    SHORT loc_4_B1A2
loc_4_B187:
        xor    eax, eax
        pop    ebp
        pop    edi
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        mov    eax, 0c00000bbh
        pop    esi
        pop    ebx
        add    esp, 8
        ret    8
loc_4_B19E:
        mov    esi, dword ptr [esp + 20h]
loc_4_B1A2:
        mov    ebx, dword ptr [esi + 1ch]
        mov    ecx, 6
        mov    eax, ebx
        sub    edx, edx
        div    ecx
        test   edx, edx
        je     SHORT loc_4_B1CB
        xor    eax, eax
        pop    ebp
        pop    edi
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        mov    eax, 0c0010015h
        pop    esi
        pop    ebx
        add    esp, 8
        ret    8
loc_4_B1CB:
        mov    ecx, dword ptr [esi + 4]
        test   byte ptr [ecx + 20h], 1
        je     SHORT loc_4_B1DE
        mov    dword ptr [esp + 10h], 103h
        jmp    SHORT loc_4_B205
loc_4_B1DE:
        push   1
        mov    eax, dword ptr [esi + 18h]
        push   eax
        mov    ebp, 6
        mov    eax, ebx
        sub    edx, edx
        div    ebp
        push   eax
        mov    ecx, dword ptr [ecx + 28h]
        push   esi
        mov    eax, dword ptr [edi + 0d8h]
        push   ecx
        push   eax
        call   sub_4_07F0
        mov    dword ptr [esp + 10h], eax
loc_4_B205:
        cmp    dword ptr [esp + 10h], 103h
        jne    SHORT loc_4_B257
        lea    ebx, [edi + 20ch]
        lea    eax, [esp + 14h]
        push   ebx
        mov    ecx, dword ptr [edi + 0d8h]
        lea    edx, [esp + 14h]
        push   eax
        push   0c0h
        push   ecx
        push   edx
        call   sub_4_0DC0
        lea    ecx, [esi + 24h]
        lea    edx, [esi + 20h]
        mov    eax, dword ptr [esp + 14h]
        push   ecx
        push   edx
        mov    ecx, dword ptr [edi + 0ch]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        push   ebx
        push   1010103h
        mov    ebx, dword ptr [edi + 8]
        push   ecx
        call   dword ptr [ebx + 4ch]
        mov    dword ptr [esp + 10h], eax
loc_4_B257:
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_4_B266
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [esi + 20h], eax
        jmp    SHORT loc_4_B278
loc_4_B266:
        cmp    dword ptr [esp + 10h], 103h
        je     SHORT loc_4_B278
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
loc_4_B278:
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_B290:
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        mov    edi, dword ptr [esp + 18h]
        cmp    dword ptr [esi + 0fch], 1
        je     SHORT loc_4_B2BD
        xor    eax, eax
        mov    dword ptr [edi + 20h], eax
        mov    dword ptr [edi + 24h], eax
        mov    eax, 0c00000bbh
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
loc_4_B2BD:
        cmp    dword ptr [edi + 1ch], 4
        mov    eax, 0
        jae    SHORT loc_4_B2D4
        mov    dword ptr [edi + 24h], 4
        mov    eax, 0c0010014h
loc_4_B2D4:
        test   eax, eax
        jne    NEAR PTR loc_4_B388
        mov    eax, dword ptr [edi + 4]
        test   byte ptr [eax + 20h], 1
        je     SHORT loc_4_B2EC
        mov    eax, 103h
        jmp    SHORT loc_4_B303
loc_4_B2EC:
        push   1
        mov    ecx, dword ptr [edi + 18h]
        push   ecx
        mov    eax, dword ptr [eax + 28h]
        push   edi
        mov    edx, dword ptr [esi + 0dch]
        push   eax
        push   edx
        call   sub_4_1130
loc_4_B303:
        cmp    eax, 103h
        jne    SHORT loc_4_B366
        mov    eax, dword ptr [esi + 0dch]
        mov    ecx, dword ptr [eax + 4]
        mov    dx, cx
        mov    eax, ecx
        shr    eax, 10h
        mov    dword ptr [esp + 0ch], ecx
        mov    ch, cl
        mov    cl, dh
        movzx  edx, cx
        mov    cx, ax
        lea    ebx, [esi + 20ch]
        shl    edx, 10h
        mov    ch, cl
        mov    cl, ah
        push   4
        movzx  eax, cx
        add    edx, eax
        lea    ecx, [esp + 10h]
        mov    dword ptr [esp + 10h], edx
        push   ecx
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_B34C:
        lea    eax, [edi + 24h]
        lea    ecx, [edi + 20h]
        push   eax
        mov    edx, dword ptr [esi + 0ch]
        push   ecx
        mov    eax, dword ptr [esi + 8]
        push   4
        push   ebx
        push   2010103h
        push   edx
        call   dword ptr [eax + 4ch]
loc_4_B366:
        test   eax, eax
        jne    SHORT loc_4_B379
        mov    ecx, dword ptr [edi + 1ch]
        mov    dword ptr [edi + 20h], ecx
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
loc_4_B379:
        cmp    eax, 103h
        je     SHORT loc_4_B388
        xor    ecx, ecx
        mov    dword ptr [edi + 20h], ecx
        mov    dword ptr [edi + 24h], ecx
loc_4_B388:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_B3A0:
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        mov    edi, dword ptr [esp + 18h]
        cmp    dword ptr [esi + 0fch], 1
        je     SHORT loc_4_B3CD
        xor    eax, eax
        mov    dword ptr [edi + 20h], eax
        mov    dword ptr [edi + 24h], eax
        mov    eax, 0c00000bbh
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
loc_4_B3CD:
        cmp    dword ptr [edi + 1ch], 4
        mov    eax, 0
        jae    SHORT loc_4_B3E4
        mov    dword ptr [edi + 24h], 4
        mov    eax, 0c0010014h
loc_4_B3E4:
        test   eax, eax
        jne    NEAR PTR loc_4_B498
        mov    eax, dword ptr [edi + 4]
        test   byte ptr [eax + 20h], 1
        je     SHORT loc_4_B3FC
        mov    eax, 103h
        jmp    SHORT loc_4_B413
loc_4_B3FC:
        push   1
        mov    ecx, dword ptr [edi + 18h]
        push   ecx
        mov    eax, dword ptr [eax + 28h]
        push   edi
        mov    edx, dword ptr [esi + 0dch]
        push   eax
        push   edx
        call   sub_4_11F0
loc_4_B413:
        cmp    eax, 103h
        jne    SHORT loc_4_B476
        mov    eax, dword ptr [esi + 0dch]
        mov    ecx, dword ptr [eax + 8]
        mov    dx, cx
        mov    eax, ecx
        shr    eax, 10h
        mov    dword ptr [esp + 0ch], ecx
        mov    ch, cl
        mov    cl, dh
        movzx  edx, cx
        mov    cx, ax
        lea    ebx, [esi + 20ch]
        shl    edx, 10h
        mov    ch, cl
        mov    cl, ah
        push   4
        movzx  eax, cx
        add    edx, eax
        lea    ecx, [esp + 10h]
        mov    dword ptr [esp + 10h], edx
        push   ecx
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_B45C:
        lea    eax, [edi + 24h]
        lea    ecx, [edi + 20h]
        push   eax
        mov    edx, dword ptr [esi + 0ch]
        push   ecx
        mov    eax, dword ptr [esi + 8]
        push   4
        push   ebx
        push   2010104h
        push   edx
        call   dword ptr [eax + 4ch]
loc_4_B476:
        test   eax, eax
        jne    SHORT loc_4_B489
        mov    ecx, dword ptr [edi + 1ch]
        mov    dword ptr [edi + 20h], ecx
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
loc_4_B489:
        cmp    eax, 103h
        je     SHORT loc_4_B498
        xor    ecx, ecx
        mov    dword ptr [edi + 20h], ecx
        mov    dword ptr [edi + 24h], ecx
loc_4_B498:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_B4B0:
        sub    esp, 0ch
        push   ebx
        push   esi
        mov    bl, byte ptr [esp + 20h]
        push   edi
        push   ebp
        test   bl, bl
        mov    ebp, 6
        je     SHORT loc_4_B4C9
        mov    ebp, 2
loc_4_B4C9:
        mov    edi, dword ptr [esp + 20h]
        mov    esi, dword ptr [esp + 24h]
        cmp    dword ptr [edi + 0fch], 2
        je     SHORT loc_4_B4F1
        xor    eax, eax
        pop    ebp
        pop    edi
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        mov    eax, 0c00000bbh
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret    0ch
loc_4_B4F1:
        mov    eax, dword ptr [esi + 1ch]
        sub    edx, edx
        mov    dword ptr [esp + 14h], eax
        div    ebp
        test   edx, edx
        je     SHORT loc_4_B517
        xor    eax, eax
        pop    ebp
        pop    edi
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        mov    eax, 0c0010015h
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret    0ch
loc_4_B517:
        mov    ecx, dword ptr [esi + 4]
        test   byte ptr [ecx + 20h], 1
        je     SHORT loc_4_B52A
        mov    dword ptr [esp + 10h], 103h
        jmp    SHORT loc_4_B56A
loc_4_B52A:
        push   1
        mov    eax, dword ptr [esi + 18h]
        push   eax
        test   bl, bl
        mov    eax, dword ptr [esp + 1ch]
        je     SHORT loc_4_B550
        sub    edx, edx
        mov    ecx, dword ptr [ecx + 28h]
        div    ebp
        push   eax
        mov    edx, dword ptr [edi + 0e0h]
        push   esi
        push   ecx
        push   edx
        call   sub_4_1E90
        jmp    SHORT loc_4_B566
loc_4_B550:
        sub    edx, edx
        mov    ecx, dword ptr [ecx + 28h]
        div    ebp
        push   eax
        mov    edx, dword ptr [edi + 0e0h]
        push   esi
        push   ecx
        push   edx
        call   sub_4_1C10
loc_4_B566:
        mov    dword ptr [esp + 10h], eax
loc_4_B56A:
        cmp    dword ptr [esp + 10h], 103h
        jne    SHORT loc_4_B5F1
        test   bl, bl
        je     SHORT loc_4_B59D
        lea    eax, [edi + 20ch]
        lea    ecx, [esp + 18h]
        push   eax
        push   ecx
        mov    eax, ebp
        mov    ecx, dword ptr [edi + 0e0h]
        shl    eax, 5
        lea    edx, [esp + 18h]
        push   eax
        push   ecx
        push   edx
        call   sub_4_25A0
        jmp    SHORT loc_4_B5C0
loc_4_B59D:
        lea    ecx, [edi + 20ch]
        lea    edx, [esp + 18h]
        push   ecx
        mov    eax, ebp
        shl    eax, 5
        push   edx
        push   eax
        mov    ecx, dword ptr [edi + 0e0h]
        lea    edx, [esp + 1ch]
        push   ecx
        push   edx
        call   sub_4_2540
loc_4_B5C0:
        lea    eax, [esi + 24h]
        lea    ecx, [esi + 20h]
        imul   ebp, dword ptr [esp + 18h]
        push   eax
        cmp    bl, 1
        push   ecx
        lea    eax, [edi + 20ch]
        push   ebp
        mov    ecx, dword ptr [edi + 0ch]
        push   eax
        mov    ebx, dword ptr [edi + 8]
        sbb    eax, eax
        and    eax, 0fffffffch
        add    eax, 3010107h
        push   eax
        push   ecx
        call   dword ptr [ebx + 4ch]
        mov    dword ptr [esp + 10h], eax
loc_4_B5F1:
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_4_B600
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [esi + 20h], eax
        jmp    SHORT loc_4_B612
loc_4_B600:
        cmp    dword ptr [esp + 10h], 103h
        je     SHORT loc_4_B612
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
loc_4_B612:
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret    0ch
sub_4_B620:
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esi + 4]
        test   ecx, ecx
        jne    SHORT loc_4_B652
        lea    eax, [esi + 24h]
        lea    ecx, [esi + 20h]
        push   eax
        mov    edx, dword ptr [esi + 1ch]
        push   ecx
        mov    eax, dword ptr [esi + 18h]
        push   edx
        mov    ecx, dword ptr [esi + 14h]
        push   eax
        push   ecx
        mov    ecx, dword ptr [esp + 1ch]
        mov    eax, dword ptr [ecx + 0ch]
        mov    esi, dword ptr [ecx + 8]
        push   eax
        call   dword ptr [esi + 4ch]
        pop    esi
        ret    8
loc_4_B652:
        mov    edx, dword ptr [esi + 14h]
        cmp    edx, 1010103h
        ja     SHORT loc_4_B67D
        je     NEAR PTR loc_4_B729
        cmp    edx, 1010eh
        je     SHORT loc_4_B6D4
        cmp    edx, 1010fh
        je     SHORT loc_4_B6E3
        cmp    edx, 10112h
        je     SHORT loc_4_B6F2
        jmp    SHORT loc_4_B6B1
loc_4_B67D:
        cmp    edx, 3010103h
        ja     SHORT loc_4_B6A5
        je     NEAR PTR loc_4_B756
        cmp    edx, 2010103h
        je     NEAR PTR loc_4_B738
        cmp    edx, 2010104h
        je     NEAR PTR loc_4_B747
        jmp    SHORT loc_4_B6B1
loc_4_B6A5:
        cmp    edx, 3010107h
        je     NEAR PTR loc_4_B767
loc_4_B6B1:
        lea    eax, [esi + 24h]
        lea    ecx, [esi + 20h]
        push   eax
        push   ecx
        mov    eax, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esp + 10h]
        push   eax
        mov    esi, dword ptr [esi + 18h]
        push   esi
        mov    eax, dword ptr [ecx + 0ch]
        push   edx
        mov    esi, dword ptr [ecx + 8]
        push   eax
        call   dword ptr [esi + 4ch]
        pop    esi
        ret    8
loc_4_B6D4:
        mov    ecx, dword ptr [esp + 8]
        push   esi
        push   ecx
        call   sub_4_AE40
        pop    esi
        ret    8
loc_4_B6E3:
        mov    ecx, dword ptr [esp + 8]
        push   esi
        push   ecx
        call   sub_4_B050
        pop    esi
        ret    8
loc_4_B6F2:
        cmp    dword ptr [esi + 1ch], 4
        mov    eax, 0
        jae    SHORT loc_4_B709
        mov    dword ptr [esi + 24h], 4
        mov    eax, 0c0010014h
loc_4_B709:
        test   eax, eax
        jne    SHORT loc_4_B774
        push   4
        mov    eax, dword ptr [esi + 18h]
        push   eax
        add    ecx, 3ch
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_B71C:
        mov    dword ptr [esi + 20h], 4
        xor    eax, eax
        pop    esi
        ret    8
loc_4_B729:
        mov    ecx, dword ptr [esp + 8]
        push   esi
        push   ecx
        call   sub_4_B160
        pop    esi
        ret    8
loc_4_B738:
        mov    ecx, dword ptr [esp + 8]
        push   esi
        push   ecx
        call   sub_4_B290
        pop    esi
        ret    8
loc_4_B747:
        mov    ecx, dword ptr [esp + 8]
        push   esi
        push   ecx
        call   sub_4_B3A0
        pop    esi
        ret    8
loc_4_B756:
        mov    ecx, dword ptr [esp + 8]
        push   0
        push   esi
        push   ecx
        call   sub_4_B4B0
        pop    esi
        ret    8
loc_4_B767:
        mov    ecx, dword ptr [esp + 8]
        push   1
        push   esi
        push   ecx
        call   sub_4_B4B0
loc_4_B774:
        pop    esi
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_B780:
        sub    esp, 4
        mov    eax, 0
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        cmp    dword ptr [esi + 1ch], 4
        jae    SHORT loc_4_B7A0
        mov    dword ptr [esi + 24h], 4
        mov    eax, 0c0010014h
loc_4_B7A0:
        test   eax, eax
        jne    SHORT loc_4_B7EC
        mov    edi, dword ptr [esi + 4]
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [edi + 28h]
        mov    eax, dword ptr [eax + 0fch]
        cmp    eax, 7
        ja     SHORT loc_4_B7D3
        xor    edx, edx
        db  08Ah, 090h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 095h, 000h, 000h, 000h, 000h
loc_4_B7C8:
        test   byte ptr [edi + 20h], 2
loc_4_B7CC:
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [esp + 8], eax
loc_4_B7D3:
        lea    eax, [esp + 8]
        push   4
        push   eax
        mov    ecx, dword ptr [esi + 18h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_B7E3:
        mov    dword ptr [esi + 20h], 4
        xor    eax, eax
loc_4_B7EC:
        pop    edi
        pop    esi
        add    esp, 4
        ret    8
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 001h, 002h, 004h, 004h, 004h, 004h, 003h
sub_4_B810:
        sub    esp, 4
        mov    eax, 0
        push   esi
        mov    esi, dword ptr [esp + 10h]
        cmp    dword ptr [esi + 1ch], 4
        jae    SHORT loc_4_B82F
        mov    dword ptr [esi + 24h], 4
        mov    eax, 0c0010014h
loc_4_B82F:
        test   eax, eax
        jne    SHORT loc_4_B870
        mov    eax, dword ptr [esp + 0ch]
        mov    eax, dword ptr [eax + 0fch]
        mov    dword ptr [esp + 4], eax
        cmp    eax, 7
        jne    SHORT loc_4_B857
        mov    eax, dword ptr [esi + 4]
        test   byte ptr [eax + 20h], 2
        je     SHORT loc_4_B857
        mov    dword ptr [esp + 4], 0
loc_4_B857:
        lea    eax, [esp + 4]
        push   4
        push   eax
        mov    ecx, dword ptr [esi + 18h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_B867:
        mov    dword ptr [esi + 20h], 4
        xor    eax, eax
loc_4_B870:
        pop    esi
        add    esp, 4
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_B880:
        sub    esp, 8
        lea    ecx, [esp + 4]
        push   esi
        mov    esi, dword ptr [esp + 14h]
        push   edi
        mov    edi, dword ptr [esp + 14h]
        mov    eax, dword ptr [esi + 18h]
        push   eax
        mov    edx, dword ptr [esi + 1ch]
        push   ecx
        mov    eax, dword ptr [esi + 4]
        push   edx
        mov    ecx, dword ptr [eax + 28h]
        push   ecx
        mov    edx, dword ptr [edi + 0d8h]
        lea    eax, [esp + 18h]
        push   edx
        push   eax
        call   sub_4_0D10
        cmp    dword ptr [esp + 8], 0c0000001h
        jne    SHORT loc_4_B8E8
        mov    eax, dword ptr [esi + 4]
        mov    edx, dword ptr [edi + 0d8h]
        mov    ecx, dword ptr [eax + 28h]
        push   ecx
        push   edx
        call   sub_4_0CD0
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esp + 8], 0c0010014h
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [esi + 24h], eax
        jmp    SHORT loc_4_B8FB
loc_4_B8E8:
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [esi + 24h], 0
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [esi + 20h], eax
loc_4_B8FB:
        mov    eax, dword ptr [esp + 8]
        pop    edi
        pop    esi
        add    esp, 8
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_B910:
        sub    esp, 8
        lea    ecx, [esp + 4]
        push   esi
        mov    esi, dword ptr [esp + 14h]
        push   edi
        mov    edi, dword ptr [esp + 14h]
        mov    eax, dword ptr [esi + 18h]
        push   eax
        mov    edx, dword ptr [esi + 1ch]
        push   ecx
        mov    eax, dword ptr [esi + 4]
        push   edx
        mov    ecx, dword ptr [eax + 28h]
        push   ecx
        mov    edx, dword ptr [edi + 0e0h]
        lea    eax, [esp + 18h]
        push   edx
        push   eax
        call   sub_4_23C0
        cmp    dword ptr [esp + 8], 0c0000001h
        jne    SHORT loc_4_B978
        mov    eax, dword ptr [esi + 4]
        mov    edx, dword ptr [edi + 0e0h]
        mov    ecx, dword ptr [eax + 28h]
        push   ecx
        push   edx
        call   sub_4_2340
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esp + 8], 0c0010014h
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [esi + 24h], eax
        jmp    SHORT loc_4_B98B
loc_4_B978:
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [esi + 24h], 0
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [esi + 20h], eax
loc_4_B98B:
        mov    eax, dword ptr [esp + 8]
        pop    edi
        pop    esi
        add    esp, 8
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_B9A0:
        sub    esp, 8
        lea    ecx, [esp + 4]
        push   esi
        mov    esi, dword ptr [esp + 14h]
        push   edi
        mov    edi, dword ptr [esp + 14h]
        mov    eax, dword ptr [esi + 18h]
        push   eax
        mov    edx, dword ptr [esi + 1ch]
        push   ecx
        mov    eax, dword ptr [esi + 4]
        push   edx
        mov    ecx, dword ptr [eax + 28h]
        push   ecx
        mov    edx, dword ptr [edi + 0e0h]
        lea    eax, [esp + 18h]
        push   edx
        push   eax
        call   sub_4_2480
        cmp    dword ptr [esp + 8], 0c0000001h
        jne    SHORT loc_4_BA05
        mov    eax, dword ptr [esi + 4]
        mov    edx, dword ptr [edi + 0e0h]
        mov    ecx, dword ptr [eax + 28h]
        push   ecx
        push   edx
        call   sub_4_2380
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esp + 8], 0c0010014h
        add    eax, eax
        mov    dword ptr [esi + 24h], eax
        jmp    SHORT loc_4_BA15
loc_4_BA05:
        mov    dword ptr [esi + 24h], 0
        mov    eax, dword ptr [esp + 0ch]
        add    eax, eax
        mov    dword ptr [esi + 20h], eax
loc_4_BA15:
        mov    eax, dword ptr [esp + 8]
        pop    edi
        pop    esi
        add    esp, 8
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_BA30:
        push   esi
        mov    eax, 0
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    edx, dword ptr [edi + 18h]
        mov    ecx, dword ptr [edi + 1ch]
        cmp    ecx, 4
        jae    SHORT loc_4_BA52
        mov    dword ptr [edi + 24h], 4
        mov    eax, 0c0010014h
loc_4_BA52:
        test   eax, eax
        jne    SHORT loc_4_BA98
        mov    esi, dword ptr [esp + 0ch]
        cmp    dword ptr [esi + 0fch], 7
        jne    SHORT loc_4_BA80
        mov    eax, dword ptr [edi + 4]
        test   byte ptr [eax + 20h], 2
        je     SHORT loc_4_BA80
        xor    eax, eax
        mov    dword ptr [edx], 1eah
        mov    dword ptr [edi + 20h], 4
        pop    edi
        pop    esi
        ret    8
loc_4_BA80:
        lea    eax, [edi + 24h]
        push   eax
        lea    eax, [edi + 20h]
        push   eax
        push   ecx
        push   edx
        mov    ecx, dword ptr [edi + 14h]
        push   ecx
        mov    edx, dword ptr [esi + 0ch]
        push   edx
        mov    eax, dword ptr [esi + 8]
        call   dword ptr [eax + 3ch]
loc_4_BA98:
        pop    edi
        pop    esi
        ret    8
        db  08Dh, 049h, 000h
sub_4_BAA0:
        mov    ecx, dword ptr [esp + 8]
        push   esi
        push   edi
        mov    eax, 0
        mov    edi, dword ptr [ecx + 18h]
        mov    edx, dword ptr [ecx + 1ch]
        cmp    edx, 4
        jae    SHORT loc_4_BAC2
        mov    dword ptr [ecx + 24h], 4
        mov    eax, 0c0010014h
loc_4_BAC2:
        test   eax, eax
        jne    SHORT loc_4_BB08
        mov    esi, dword ptr [esp + 0ch]
        cmp    dword ptr [esi + 0fch], 7
        jne    SHORT loc_4_BAF0
        mov    eax, dword ptr [ecx + 4]
        test   byte ptr [eax + 20h], 2
        je     SHORT loc_4_BAF0
        xor    eax, eax
        mov    dword ptr [edi], 1f8h
        mov    dword ptr [ecx + 20h], 4
        pop    edi
        pop    esi
        ret    8
loc_4_BAF0:
        lea    eax, [ecx + 24h]
        push   eax
        lea    eax, [ecx + 20h]
        push   eax
        mov    ecx, dword ptr [ecx + 14h]
        push   edx
        mov    eax, dword ptr [esi + 8]
        push   edi
        mov    edx, dword ptr [esi + 0ch]
        push   ecx
        push   edx
        call   dword ptr [eax + 3ch]
loc_4_BB08:
        pop    edi
        pop    esi
        ret    8
        db  08Dh, 049h, 000h
sub_4_BB10:
        sub    esp, 8
        mov    eax, 0
        push   esi
        mov    esi, dword ptr [esp + 14h]
        mov    edx, dword ptr [esi + 1ch]
        cmp    edx, 6
        jae    SHORT loc_4_BB31
        mov    dword ptr [esi + 24h], 6
        mov    eax, 0c0010014h
loc_4_BB31:
        test   eax, eax
        jne    SHORT loc_4_BBA0
        mov    ecx, dword ptr [esp + 10h]
        cmp    dword ptr [ecx + 0fch], 7
        jne    SHORT loc_4_BB85
        mov    eax, dword ptr [esi + 4]
        test   byte ptr [eax + 20h], 2
        je     SHORT loc_4_BB85
        lea    eax, [esp + 4]
        mov    cl, byte ptr [ecx + 171h]
        push   6
        mov    edx, dword ptr [esi + 18h]
        mov    dword ptr [eax], 0
        mov    word ptr [eax + 4], 0
        lea    eax, [esp + 8]
        mov    byte ptr [esp + 0dh], cl
        push   eax
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_BB75:
        mov    dword ptr [esi + 20h], 6
        xor    eax, eax
        pop    esi
        add    esp, 8
        ret    8
loc_4_BB85:
        lea    eax, [esi + 24h]
        push   eax
        lea    eax, [esi + 20h]
        push   eax
        push   edx
        mov    edx, dword ptr [esi + 18h]
        mov    eax, dword ptr [ecx + 0ch]
        push   edx
        mov    esi, dword ptr [esi + 14h]
        push   esi
        push   eax
        mov    esi, dword ptr [ecx + 8]
        call   dword ptr [esi + 3ch]
loc_4_BBA0:
        pop    esi
        add    esp, 8
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_BBB0:
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 14h]
        push   edi
        mov    edi, dword ptr [esi + 18h]
        mov    edx, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi + 14h]
        cmp    ecx, 1010eh
        mov    eax, ecx
        ja     SHORT loc_4_BBE8
        je     NEAR PTR loc_4_BCFA
        sub    eax, 10103h
        cmp    eax, 3
        ja     NEAR PTR loc_4_BC72
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_BBE8:
        cmp    eax, 1010102h
        ja     SHORT loc_4_BC12
        cmp    eax, 1010101h
        jae    NEAR PTR loc_4_BD56
        cmp    eax, 1010fh
        je     NEAR PTR loc_4_BD0E
        cmp    eax, 10111h
        je     NEAR PTR loc_4_BD42
        jmp    SHORT loc_4_BC72
loc_4_BC12:
        cmp    eax, 2010103h
        ja     SHORT loc_4_BC37
        je     NEAR PTR loc_4_BDB9
        cmp    eax, 1010103h
        je     NEAR PTR loc_4_BD6A
        cmp    eax, 1010104h
        je     NEAR PTR loc_4_BD7E
        jmp    SHORT loc_4_BC72
loc_4_BC37:
        cmp    eax, 3010103h
        ja     SHORT loc_4_BC51
        je     NEAR PTR loc_4_BE79
        cmp    eax, 2010104h
        je     NEAR PTR loc_4_BE1B
        jmp    SHORT loc_4_BC72
loc_4_BC51:
        cmp    eax, 3010104h
        je     NEAR PTR loc_4_BE8D
        cmp    eax, 3010107h
        je     NEAR PTR loc_4_BEC4
        cmp    eax, 3010108h
        je     NEAR PTR loc_4_BED8
loc_4_BC72:
        lea    eax, [esi + 24h]
        lea    edx, [esi + 20h]
        push   eax
        mov    ebx, dword ptr [esi + 1ch]
        push   edx
        push   ebx
        push   edi
        push   ecx
        mov    ecx, dword ptr [esp + 28h]
        mov    eax, dword ptr [ecx + 0ch]
        mov    ebx, dword ptr [ecx + 8]
        push   eax
        call   dword ptr [ebx + 3ch]
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
loc_4_BC97:
        mov    ecx, dword ptr [esp + 14h]
        push   esi
        push   ecx
        call   sub_4_B810
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
loc_4_BCAB:
        cmp    dword ptr [esi + 1ch], 4
        mov    eax, 0
        jae    SHORT loc_4_BCC2
        mov    dword ptr [esi + 24h], 4
        mov    eax, 0c0010014h
loc_4_BCC2:
        test   eax, eax
        jne    NEAR PTR loc_4_BF06
        mov    ecx, dword ptr [esp + 14h]
        mov    edx, dword ptr [ecx + 1f4h]
        mov    dword ptr [edi], edx
        pop    edi
        mov    dword ptr [esi + 20h], 4
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
loc_4_BCE6:
        mov    ecx, dword ptr [esp + 14h]
        push   esi
        push   ecx
        call   sub_4_BA30
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
loc_4_BCFA:
        mov    ecx, dword ptr [esp + 14h]
        push   esi
        push   ecx
        call   sub_4_B780
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
loc_4_BD0E:
        cmp    dword ptr [esi + 1ch], 4
        mov    eax, 0
        jae    SHORT loc_4_BD25
        mov    dword ptr [esi + 24h], 4
        mov    eax, 0c0010014h
loc_4_BD25:
        test   eax, eax
        jne    NEAR PTR loc_4_BF06
        mov    ecx, dword ptr [edx + 38h]
        mov    dword ptr [edi], ecx
        pop    edi
        mov    dword ptr [esi + 20h], 4
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
loc_4_BD42:
        mov    ecx, dword ptr [esp + 14h]
        push   esi
        push   ecx
        call   sub_4_BAA0
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
loc_4_BD56:
        mov    ecx, dword ptr [esp + 14h]
        push   esi
        push   ecx
        call   sub_4_BB10
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
loc_4_BD6A:
        mov    ecx, dword ptr [esp + 14h]
        push   esi
        push   ecx
        call   sub_4_B880
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
loc_4_BD7E:
        cmp    dword ptr [esi + 1ch], 4
        mov    eax, 0
        jae    SHORT loc_4_BD95
        mov    dword ptr [esi + 24h], 4
        mov    eax, 0c0010014h
loc_4_BD95:
        test   eax, eax
        jne    NEAR PTR loc_4_BF06
        mov    ecx, dword ptr [esp + 14h]
        mov    edx, dword ptr [ecx + 1e8h]
        mov    dword ptr [edi], edx
        pop    edi
        mov    dword ptr [esi + 20h], 4
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
loc_4_BDB9:
        cmp    dword ptr [esi + 1ch], 4
        mov    eax, 0
        jae    SHORT loc_4_BDD0
        mov    dword ptr [esi + 24h], 4
        mov    eax, 0c0010014h
loc_4_BDD0:
        test   eax, eax
        jne    NEAR PTR loc_4_BF06
        mov    ecx, dword ptr [edx + 28h]
        mov    edx, dword ptr [ecx + 0ch]
        mov    word ptr [esp + 0ch], dx
        mov    ecx, edx
        shr    ecx, 10h
        mov    byte ptr [esp + 0dh], dl
        mov    byte ptr [esp + 0ch], dh
        mov    dx, cx
        mov    ebx, dword ptr [esp + 0ch]
        and    ebx, 0ffffh
        mov    dh, dl
        shl    ebx, 10h
        mov    dl, ch
        movzx  ecx, dx
        add    ebx, ecx
        mov    dword ptr [edi], ebx
        pop    edi
        mov    dword ptr [esi + 20h], 4
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
loc_4_BE1B:
        cmp    dword ptr [esi + 1ch], 4
        mov    eax, 0
        jae    SHORT loc_4_BE32
        mov    dword ptr [esi + 24h], 4
        mov    eax, 0c0010014h
loc_4_BE32:
        test   eax, eax
        jne    NEAR PTR loc_4_BF06
        mov    ecx, dword ptr [esp + 14h]
        mov    edx, dword ptr [ecx + 0dch]
        mov    ecx, dword ptr [edx + 8]
        mov    bx, cx
        mov    edx, ecx
        shr    edx, 10h
        mov    bh, bl
        mov    bl, ch
        mov    dword ptr [edi], ecx
        movzx  ebx, bx
        mov    cx, dx
        shl    ebx, 10h
        mov    ch, cl
        mov    cl, dh
        movzx  edx, cx
        add    ebx, edx
        mov    dword ptr [edi], ebx
        pop    edi
        mov    dword ptr [esi + 20h], 4
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
loc_4_BE79:
        mov    ecx, dword ptr [esp + 14h]
        push   esi
        push   ecx
        call   sub_4_B910
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
loc_4_BE8D:
        cmp    dword ptr [esi + 1ch], 4
        mov    eax, 0
        jae    SHORT loc_4_BEA4
        mov    dword ptr [esi + 24h], 4
        mov    eax, 0c0010014h
loc_4_BEA4:
        test   eax, eax
        jne    SHORT loc_4_BF06
        mov    ecx, dword ptr [esp + 14h]
        mov    edx, dword ptr [ecx + 1e8h]
        mov    dword ptr [edi], edx
        pop    edi
        mov    dword ptr [esi + 20h], 4
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
loc_4_BEC4:
        mov    ecx, dword ptr [esp + 14h]
        push   esi
        push   ecx
        call   sub_4_B9A0
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
loc_4_BED8:
        cmp    dword ptr [esi + 1ch], 4
        mov    eax, 0
        jae    SHORT loc_4_BEEF
        mov    dword ptr [esi + 24h], 4
        mov    eax, 0c0010014h
loc_4_BEEF:
        test   eax, eax
        jne    SHORT loc_4_BF06
        mov    ecx, dword ptr [esp + 14h]
        mov    edx, dword ptr [ecx + 1ech]
        mov    dword ptr [edi], edx
        mov    dword ptr [esi + 20h], 4
loc_4_BF06:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
        db  090h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h
sub_4_BF20:
        sub    esp, 14h
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 20h]
        push   edi
        push   ebp
        cmp    dword ptr [esi + 1e0h], 0
        jne    NEAR PTR loc_4_C2AB
loc_4_BF38:
        mov    edi, dword ptr [esi + 1dch]
        test   edi, edi
        je     NEAR PTR loc_4_C2AB
        mov    byte ptr [esp + 13h], 1
        mov    dword ptr [esp + 18h], 0
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 1dch], eax
        mov    dword ptr [esi + 1e0h], edi
        and    dword ptr [esi + 3ch], 0fffffdffh
        mov    eax, dword ptr [edi + 10h]
        test   eax, eax
        je     SHORT loc_4_BF7E
        cmp    eax, 1
        je     SHORT loc_4_BF8E
        cmp    eax, 2
        je     SHORT loc_4_BF9E
        jmp    NEAR PTR loc_4_C258
loc_4_BF7E:
        push   edi
        push   esi
        call   sub_4_BBB0
        mov    dword ptr [esp + 18h], eax
        jmp    NEAR PTR loc_4_C258
loc_4_BF8E:
        push   edi
        push   esi
        call   sub_4_B620
        mov    dword ptr [esp + 18h], eax
        jmp    NEAR PTR loc_4_C258
loc_4_BF9E:
        lea    ebp, [esp + 14h]
        mov    ebx, 4
        mov    ecx, dword ptr [edi + 14h]
        cmp    ecx, 1010eh
        ja     SHORT loc_4_BFDE
        je     NEAR PTR loc_4_C09A
        cmp    ecx, 10103h
        jb     NEAR PTR loc_4_C056
        cmp    ecx, 10104h
        jbe    NEAR PTR loc_4_C07F
        cmp    ecx, 10105h
        je     NEAR PTR loc_4_C08F
        jmp    SHORT loc_4_C056
loc_4_BFDE:
        cmp    ecx, 1010103h
        ja     SHORT loc_4_BFFA
        je     NEAR PTR loc_4_C10D
        cmp    ecx, 1010fh
        je     NEAR PTR loc_4_C102
        jmp    SHORT loc_4_C056
loc_4_BFFA:
        cmp    ecx, 2010103h
        ja     SHORT loc_4_C016
        je     NEAR PTR loc_4_C14C
        cmp    ecx, 1010104h
        je     NEAR PTR loc_4_C141
        jmp    SHORT loc_4_C056
loc_4_C016:
        cmp    ecx, 3010103h
        ja     SHORT loc_4_C032
        je     NEAR PTR loc_4_C1B1
        cmp    ecx, 2010104h
        je     NEAR PTR loc_4_C180
        jmp    SHORT loc_4_C056
loc_4_C032:
        cmp    ecx, 3010104h
        je     NEAR PTR loc_4_C1E2
        cmp    ecx, 3010107h
        je     NEAR PTR loc_4_C1EA
        cmp    ecx, 3010108h
        je     NEAR PTR loc_4_C213
loc_4_C056:
        lea    eax, [edi + 24h]
        lea    edx, [edi + 20h]
        mov    byte ptr [esp + 13h], 0
        push   eax
        push   edx
        mov    eax, dword ptr [edi + 1ch]
        mov    edx, dword ptr [edi + 18h]
        push   eax
        push   edx
        push   ecx
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [esi + 0ch]
        push   ecx
        call   dword ptr [eax + 3ch]
        mov    dword ptr [esp + 18h], eax
        jmp    NEAR PTR loc_4_C21D
loc_4_C07F:
        lea    ebp, [esi + 0fch]
        mov    ebx, 4
        jmp    NEAR PTR loc_4_C21D
loc_4_C08F:
        mov    eax, dword ptr [esi + 1f4h]
        jmp    NEAR PTR loc_4_C219
loc_4_C09A:
        mov    eax, dword ptr [esi + 0fch]
        cmp    eax, 8
        ja     SHORT loc_4_C0F9
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_C0AC:
        mov    eax, dword ptr [esi + 0d8h]
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [esp + 20h], ecx
        jmp    SHORT loc_4_C0F9
loc_4_C0BB:
        mov    eax, dword ptr [esi + 0dch]
        mov    ecx, dword ptr [eax + 10h]
        mov    dword ptr [esp + 20h], ecx
        jmp    SHORT loc_4_C0F9
loc_4_C0CA:
        mov    eax, dword ptr [esi + 0e0h]
        mov    ecx, dword ptr [eax + 14h]
        mov    dword ptr [esp + 20h], ecx
        jmp    SHORT loc_4_C0F9
loc_4_C0D9:
        mov    eax, dword ptr [esi + 0e4h]
        mov    ecx, dword ptr [esi + 0d8h]
        mov    eax, dword ptr [eax + 8]
        or     eax, dword ptr [ecx + 0ch]
        mov    dword ptr [esp + 20h], eax
        jmp    SHORT loc_4_C0F9
loc_4_C0F1:
        mov    dword ptr [esp + 18h], 0c00000bbh
loc_4_C0F9:
        mov    eax, dword ptr [esp + 20h]
        jmp    NEAR PTR loc_4_C219
loc_4_C102:
        mov    eax, dword ptr [esi + 1f0h]
        jmp    NEAR PTR loc_4_C219
loc_4_C10D:
        mov    eax, dword ptr [edi + 18h]
        lea    ecx, [esp + 1ch]
        push   eax
        mov    edx, dword ptr [edi + 1ch]
        push   ecx
        mov    eax, dword ptr [esi + 0d8h]
        lea    ecx, [esp + 20h]
        push   edx
        push   eax
        push   ecx
        call   sub_4_0DC0
        mov    ebp, dword ptr [edi + 18h]
        mov    ecx, dword ptr [esp + 1ch]
        lea    eax, [ecx + ecx*2]
        lea    ebx, [eax*2]
        jmp    NEAR PTR loc_4_C21D
loc_4_C141:
        mov    eax, dword ptr [esi + 1e8h]
        jmp    NEAR PTR loc_4_C219
loc_4_C14C:
        mov    eax, dword ptr [esi + 0dch]
        mov    ecx, dword ptr [eax + 4]
        mov    dx, cx
        mov    eax, ecx
        shr    eax, 10h
        mov    dword ptr [esp + 14h], ecx
        mov    ch, cl
        mov    cl, dh
        movzx  edx, cx
        mov    cx, ax
        shl    edx, 10h
        mov    ch, cl
        mov    cl, ah
        movzx  eax, cx
        add    edx, eax
        mov    dword ptr [esp + 14h], edx
        jmp    NEAR PTR loc_4_C21D
loc_4_C180:
        mov    eax, dword ptr [esi + 0dch]
        mov    ecx, dword ptr [eax + 8]
        mov    dx, cx
        mov    eax, ecx
        shr    eax, 10h
        mov    dword ptr [esp + 14h], ecx
        mov    ch, cl
        mov    cl, dh
        movzx  edx, cx
        mov    cx, ax
        shl    edx, 10h
        mov    ch, cl
        mov    cl, ah
        movzx  eax, cx
        add    edx, eax
        mov    dword ptr [esp + 14h], edx
        jmp    SHORT loc_4_C21D
loc_4_C1B1:
        mov    eax, dword ptr [edi + 18h]
        lea    ecx, [esp + 1ch]
        push   eax
        mov    edx, dword ptr [edi + 1ch]
        push   ecx
        mov    eax, dword ptr [esi + 0e0h]
        lea    ecx, [esp + 20h]
        push   edx
        push   eax
        push   ecx
        call   sub_4_2540
        mov    ebp, dword ptr [edi + 18h]
        mov    ecx, dword ptr [esp + 1ch]
        lea    eax, [ecx + ecx*2]
        lea    ebx, [eax*2]
        jmp    SHORT loc_4_C21D
loc_4_C1E2:
        mov    eax, dword ptr [esi + 1e8h]
        jmp    SHORT loc_4_C219
loc_4_C1EA:
        mov    eax, dword ptr [edi + 18h]
        lea    ecx, [esp + 1ch]
        push   eax
        mov    edx, dword ptr [edi + 1ch]
        push   ecx
        mov    eax, dword ptr [esi + 0e0h]
        lea    ecx, [esp + 20h]
        push   edx
        push   eax
        push   ecx
        call   sub_4_25A0
        mov    ebp, dword ptr [edi + 18h]
        mov    ebx, dword ptr [esp + 1ch]
        add    ebx, ebx
        jmp    SHORT loc_4_C21D
loc_4_C213:
        mov    eax, dword ptr [esi + 1ech]
loc_4_C219:
        mov    dword ptr [esp + 14h], eax
loc_4_C21D:
        cmp    byte ptr [esp + 13h], 0
        je     SHORT loc_4_C258
        cmp    dword ptr [esp + 18h], 0
        jne    SHORT loc_4_C255
        cmp    dword ptr [edi + 1ch], ebx
        jae    SHORT loc_4_C23D
        mov    dword ptr [edi + 24h], ebx
        mov    dword ptr [esp + 18h], 0c0010014h
        jmp    SHORT loc_4_C258
loc_4_C23D:
        mov    dword ptr [edi + 20h], ebx
        test   ebx, ebx
        je     SHORT loc_4_C258
        mov    eax, dword ptr [edi + 18h]
        cmp    eax, ebp
        je     SHORT loc_4_C258
        push   ebx
        push   ebp
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_C253:
        jmp    SHORT loc_4_C258
loc_4_C255:
        mov    dword ptr [edi + 24h], ebx
loc_4_C258:
        cmp    dword ptr [esp + 18h], 103h
        jne    SHORT loc_4_C275
        cmp    dword ptr [esi + 1e0h], 0
        jne    SHORT loc_4_C2AB
        cmp    dword ptr [esp + 18h], 103h
        je     SHORT loc_4_C29E
loc_4_C275:
        mov    eax, dword ptr [edi + 10h]
        test   eax, eax
        je     SHORT loc_4_C286
        cmp    eax, 1
        je     SHORT loc_4_C293
        cmp    eax, 2
        jne    SHORT loc_4_C29E
loc_4_C286:
        mov    eax, dword ptr [esp + 18h]
        push   eax
        push   esi
        call   sub_4_A980
        jmp    SHORT loc_4_C29E
loc_4_C293:
        mov    eax, dword ptr [esp + 18h]
        push   eax
        push   esi
        call   sub_4_ACF0
loc_4_C29E:
        cmp    dword ptr [esi + 1e0h], 0
        je     NEAR PTR loc_4_BF38
loc_4_C2AB:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret    4
        db  08Dh, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 08Dh, 064h, 024h, 000h, 083h, 0ECh, 04Ch, 053h, 056h
        db  08Bh, 074h, 024h, 058h, 057h, 055h, 08Bh, 07Eh, 004h, 080h, 07Fh, 02Eh, 000h, 074h, 004h, 032h
        db  0DBh, 0EBh, 033h, 0BBh, 001h, 000h, 000h, 000h, 088h, 05Fh, 02Eh, 083h, 03Dh, 000h, 000h, 000h
        db  000h, 000h, 074h, 022h, 03Bh, 03Dh, 000h, 000h, 000h, 000h, 075h, 01Ah, 0C7h, 005h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 0C7h, 005h, 000h, 000h, 000h, 000h, 0BDh, 00Dh, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h, 08Dh, 044h, 024h, 04Ch, 050h, 0E8h, 000h, 000h, 000h, 000h
        db  0C7h, 044h, 024h, 030h, 002h, 000h, 000h, 000h, 0C7h, 044h, 024h, 034h, 001h, 001h, 001h, 000h
        db  0C7h, 044h, 024h, 038h, 000h, 000h, 000h, 000h, 08Bh, 044h, 024h, 064h, 06Ah, 000h, 08Dh, 04Ch
        db  024h, 024h, 089h, 044h, 024h, 020h, 0C7h, 044h, 024h, 040h, 000h, 000h, 000h, 000h, 0C7h, 044h
        db  024h, 044h, 000h, 000h, 000h, 000h, 0C7h, 044h, 024h, 048h, 000h, 000h, 000h, 000h, 051h, 057h
        db  0E8h, 0A6h, 0DDh, 0FFh, 0FFh, 06Ah, 000h, 0BAh, 002h, 000h, 000h, 000h, 06Ah, 000h, 08Bh, 0CFh
        db  0FFh, 097h, 01Ch, 001h, 000h, 000h, 084h, 0DBh, 074h, 055h, 08Bh, 0CFh, 0FFh, 097h, 018h, 001h
        db  000h, 000h, 084h, 0DBh, 074h, 049h, 0C6h, 047h, 02Eh, 000h, 083h, 03Dh, 000h, 000h, 000h, 000h
        db  000h, 074h, 026h, 03Bh, 03Dh, 000h, 000h, 000h, 000h, 075h, 01Eh, 0C7h, 005h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0C7h, 005h, 000h, 000h, 000h, 000h, 0E2h, 00Dh, 000h, 000h, 0C7h
        db  005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 080h, 07Fh, 02Ch, 001h, 075h, 010h, 0C6h
        db  047h, 02Ch, 000h, 08Dh, 047h, 02Eh, 050h, 0E8h, 000h, 000h, 000h, 000h, 083h, 0C4h, 004h, 08Dh
        db  044h, 024h, 04Ch, 06Ah, 000h, 050h, 0E8h, 000h, 000h, 000h, 000h, 080h, 07Fh, 02Eh, 000h, 074h
        db  007h, 0C6h, 044h, 024h, 013h, 000h, 0EBh, 036h, 0B8h, 001h, 000h, 000h, 000h, 088h, 047h, 02Eh
        db  088h, 044h, 024h, 013h, 083h, 03Dh, 000h, 000h, 000h, 000h, 000h, 074h, 021h, 03Bh, 03Dh, 000h
        db  000h, 000h, 000h, 075h, 019h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C7h
        db  005h, 000h, 000h, 000h, 000h, 0EBh, 00Dh, 000h, 000h, 0A3h, 000h, 000h, 000h, 000h, 08Bh, 05Ch
        db  024h, 048h, 081h, 0FBh, 014h, 000h, 001h, 0C0h, 074h, 00Ch, 081h, 0FBh, 016h, 000h, 001h, 0C0h
        db  00Fh, 085h, 0BFh, 001h, 000h, 000h, 08Bh, 05Ch, 024h, 044h, 06Ah, 000h, 053h, 0E8h, 000h, 000h
        db  000h, 000h, 089h, 044h, 024h, 01Ch, 083h, 0C4h, 008h, 085h, 0C0h, 075h, 00Ah, 0BBh, 09Ah, 000h
        db  000h, 0C0h, 0E9h, 09Eh, 001h, 000h, 000h, 08Dh, 044h, 024h, 04Ch, 050h, 0E8h, 000h, 000h, 000h
        db  000h, 08Bh, 044h, 024h, 014h, 089h, 05Ch, 024h, 03Ch, 0C7h, 044h, 024h, 030h, 002h, 000h, 000h
        db  000h, 089h, 044h, 024h, 038h, 06Ah, 000h, 0C7h, 044h, 024h, 038h, 001h, 001h, 001h, 000h, 0C7h
        db  044h, 024h, 044h, 000h, 000h, 000h, 000h, 0C7h, 044h, 024h, 048h, 000h, 000h, 000h, 000h, 08Dh
        db  04Ch, 024h, 024h, 051h, 057h, 0E8h, 071h, 0DCh, 0FFh, 0FFh, 06Ah, 000h, 0BAh, 002h, 000h, 000h
        db  000h, 06Ah, 000h, 08Bh, 0CFh, 0FFh, 097h, 01Ch, 001h, 000h, 000h, 080h, 07Ch, 024h, 013h, 000h
        db  074h, 058h, 08Bh, 0CFh, 0FFh, 097h, 018h, 001h, 000h, 000h, 080h, 07Ch, 024h, 013h, 000h, 074h
        db  049h, 0C6h, 047h, 02Eh, 000h, 083h, 03Dh, 000h, 000h, 000h, 000h, 000h, 074h, 026h, 03Bh, 03Dh
        db  000h, 000h, 000h, 000h, 075h, 01Eh, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 01Ch, 00Eh, 000h, 000h, 0C7h, 005h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 080h, 07Fh, 02Ch, 001h, 075h, 010h, 0C6h, 047h, 02Ch, 000h, 08Dh, 047h
        db  02Eh, 050h, 0E8h, 000h, 000h, 000h, 000h, 083h, 0C4h, 004h, 08Dh, 044h, 024h, 04Ch, 06Ah, 000h
        db  050h, 0E8h, 000h, 000h, 000h, 000h, 080h, 07Fh, 02Eh, 000h, 074h, 007h, 0C6h, 044h, 024h, 013h
        db  000h, 0EBh, 036h, 0B8h, 001h, 000h, 000h, 000h, 088h, 047h, 02Eh, 088h, 044h, 024h, 013h, 083h
        db  03Dh, 000h, 000h, 000h, 000h, 000h, 074h, 021h, 03Bh, 03Dh, 000h, 000h, 000h, 000h, 075h, 019h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C7h, 005h, 000h, 000h, 000h, 000h
        db  026h, 00Eh, 000h, 000h, 0A3h, 000h, 000h, 000h, 000h, 08Bh, 0C3h, 083h, 0E0h, 0FCh, 089h, 044h
        db  024h, 018h, 074h, 022h, 08Bh, 04Ch, 024h, 014h, 08Bh, 0C3h, 0C1h, 0E8h, 002h, 08Bh, 011h, 081h
        db  0E2h, 000h, 000h, 0FFh, 000h, 081h, 0FAh, 000h, 000h, 002h, 000h, 075h, 003h, 0FFh, 046h, 008h
        db  083h, 0C1h, 004h, 048h, 075h, 0E7h, 06Ah, 000h, 08Bh, 046h, 008h, 0C1h, 0E0h, 002h, 050h, 0E8h
        db  000h, 000h, 000h, 000h, 083h, 0C4h, 008h, 089h, 046h, 00Ch, 085h, 0C0h, 075h, 016h, 08Bh, 044h
        db  024h, 014h, 06Ah, 000h, 050h, 0BBh, 09Ah, 000h, 000h, 0C0h, 0E8h, 000h, 000h, 000h, 000h, 083h
        db  0C4h, 008h, 0EBh, 041h, 083h, 07Ch, 024h, 018h, 000h, 074h, 029h, 0C1h, 0EBh, 002h, 033h, 0D2h
        db  08Bh, 04Ch, 024h, 014h, 08Bh, 001h, 025h, 000h, 000h, 0FFh, 000h, 03Dh, 000h, 000h, 002h, 000h
        db  075h, 00Ch, 08Bh, 06Eh, 00Ch, 08Bh, 001h, 089h, 044h, 015h, 000h, 083h, 0C2h, 004h, 083h, 0C1h
        db  004h, 04Bh, 075h, 0E0h, 08Bh, 044h, 024h, 014h, 06Ah, 000h, 050h, 033h, 0DBh, 0E8h, 000h, 000h
        db  000h, 000h, 083h, 0C4h, 008h, 080h, 07Ch, 024h, 013h, 000h, 074h, 049h, 0C6h, 047h, 02Eh, 000h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h, 074h, 026h, 03Bh, 03Dh, 000h, 000h, 000h, 000h, 075h
        db  01Eh, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C7h, 005h, 000h, 000h, 000h
        db  000h, 05Ah, 00Eh, 000h, 000h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 080h
        db  07Fh, 02Ch, 001h, 075h, 010h, 0C6h, 047h, 02Ch, 000h, 083h, 0C7h, 02Eh, 057h, 0E8h, 000h, 000h
        db  000h, 000h, 083h, 0C4h, 004h, 08Bh, 0C3h, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 04Ch, 0C2h, 008h
        db  000h, 08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_C670:
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [ecx + 4]
        mov    eax, dword ptr [ecx + 34h]
        dec    eax
        mov    dword ptr [ecx + 34h], eax
        jne    SHORT loc_4_C687
        push   ecx
        push   edx
        call   sub_4_7740
loc_4_C687:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_C690:
        mov    eax, 103h
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_4_C6A0:
        mov    eax, 103h
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_4_C6B0:
        mov    eax, 103h
        ret    2ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_4_C6C0:
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [eax + 4]
        cmp    dword ptr [ecx + 0fch], 7
        jne    SHORT loc_4_C6EF
        test   byte ptr [eax + 20h], 2
        je     SHORT loc_4_C6EF
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_4_C6E6
        or     dword ptr [ecx + 3ch], 4
        xor    eax, eax
        ret    1ch
loc_4_C6E6:
        and    dword ptr [ecx + 3ch], 0fffffffbh
        xor    eax, eax
        ret    1ch
loc_4_C6EF:
        mov    eax, 103h
        ret    1ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_C700:
        mov    eax, 103h
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_C710:
        push   ebp
        mov    edx, dword ptr [esp + 20h]
        mov    ebp, esp
        sub    esp, 28h
        cmp    edx, 0fh
        push   ebx
        push   esi
        push   edi
        jbe    SHORT loc_4_C734
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 0c0000001h
        mov    esp, ebp
        pop    ebp
        ret    28h
loc_4_C734:
        cmp    edx, 2
        jne    SHORT loc_4_C73E
        mov    edx, 9
loc_4_C73E:
        lea    edi, [ebp - 28h]
        xor    eax, eax
        mov    ecx, 8
        rep stosd
        mov    word ptr [ebp - 28h], dx
        mov    al, byte ptr [ebp + 28h]
        mov    dword ptr [ebp - 10h], 1f4h
        test   al, al
        je     SHORT loc_4_C760
        or     byte ptr [ebp - 26h], 2
loc_4_C760:
        mov    esi, dword ptr [ebp + 0ch]
        xor    edx, edx
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 0ch], esi
        or     byte ptr [ebp - 26h], 4
        db  0C7h, 045h, 0DCh, 000h, 000h, 000h, 000h
        mov    dword ptr [esi], edx
        mov    dword ptr [ecx + 38h], esi
        mov    byte ptr [esi + 38h], dl
        mov    dword ptr [esi + 8], edx
        mov    dword ptr [esi + 34h], ecx
        mov    edi, dword ptr [ecx + 8]
        mov    edi, dword ptr [edi + 38h]
        mov    dword ptr [esi + 0ch], edi
        mov    edi, dword ptr [ecx + 150h]
        mov    dword ptr [esi + 10h], edi
        mov    byte ptr [esi + 4ch], al
        mov    al, byte ptr [ebp + 1ch]
        mov    byte ptr [esi + 4dh], al
        mov    eax, dword ptr [ecx + 3ch]
        test   al, 42h
        jne    SHORT loc_4_C7BC
        mov    edi, dword ptr [ecx + 38h]
        test   edi, edi
        je     SHORT loc_4_C7BC
        cmp    byte ptr [edi + 4dh], dl
        jne    SHORT loc_4_C7BC
        cmp    byte ptr [edi + 4ch], dl
        jne    SHORT loc_4_C7BC
        or     eax, 1
        jmp    SHORT loc_4_C7BF
loc_4_C7BC:
        and    eax, 0fffffffeh
loc_4_C7BF:
        lea    edi, [esi + 14h]
        mov    dword ptr [ecx + 3ch], eax
        lea    eax, [esi + 3ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_C7CE:
        xor    eax, eax
        mov    ecx, 8
        rep stosd
        db  0C7h, 046h, 020h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 24h], esi
        cli
        lea    eax, [ebp - 28h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_C7EB:
        mov    dword ptr [esi], eax
        test   eax, eax
        jne    SHORT loc_4_C803
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 0c0000001h
        mov    esp, ebp
        pop    ebp
        ret    28h
loc_4_C803:
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_C809:
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_C80E:
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ebx, dword ptr [ebp - 4]
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        mov    eax, dword ptr [esi]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_C82A:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        sti
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    28h
        db  08Dh, 049h, 000h
sub_4_C840:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 8], 0
        jne    SHORT loc_4_C859
        mov    eax, dword ptr [esi + 34h]
        or     dword ptr [eax + 3ch], 20h
        jmp    SHORT loc_4_C85D
loc_4_C859:
        mov    byte ptr [esi + 35h], 1
loc_4_C85D:
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 011h, 000h, 003h, 000h
        cmp    byte ptr [esi + 38h], 0
        jbe    SHORT loc_4_C882
        add    esi, 3ch
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_C87C:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_C882:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_C890:
        mov    edx, dword ptr [esp + 4]
        push   edi
        mov    edi, edx
        xor    eax, eax
        mov    ecx, 0ah
        rep stosd
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 10h]
        lea    edi, [edx + 28h]
        mov    dword ptr [edx + 50h], eax
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [edx + 48h], ecx
        mov    ecx, 8
        mov    dword ptr [edx + 4ch], eax
        xor    eax, eax
        rep stosd
        pop    edi
        mov    dword ptr [edx + 38h], edx
        db  0C7h, 042h, 034h, 000h, 000h, 000h, 000h
        ret    10h
        db  090h
loc_4_C8D0:
        mov    eax, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [esp + 18h]
        mov    edx, dword ptr [esp + 10h]
        sub    esp, 4
        push   eax
        push   ecx
        mov    eax, dword ptr [esp + 18h]
        push   edx
        mov    ecx, dword ptr [esp + 18h]
        push   eax
        xor    eax, eax
        mov    edx, dword ptr [esp + 18h]
        mov    al, byte ptr [esp + 28h]
        push   eax
        push   0
        lea    eax, [esp + 18h]
        push   0
        push   ecx
        push   edx
        push   eax
        call   sub_4_C710
        mov    eax, dword ptr [esp]
        add    esp, 4
        ret    1ch
loc_4_C910:
        mov    eax, dword ptr [esp + 4]
        push   eax
        call   sub_4_C840
        ret    4
        db  08Dh, 049h, 000h
loc_4_C920:
        mov    eax, dword ptr [esp + 8]
        test   eax, eax
        jne    SHORT loc_4_C935
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], 0c0000001h
        ret    8
loc_4_C935:
        mov    ecx, dword ptr [eax + 10h]
        mov    eax, dword ptr [eax + 0ch]
        push   ecx
        call   dword ptr [eax + 4ch]
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_4_C950:
        mov    eax, dword ptr [esp + 8]
        push   eax
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax + 24h]
        mov    eax, dword ptr [eax + 1ch]
        push   ecx
        call   dword ptr [eax + 4ch]
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_C970:
        sub    esp, 54h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 6ch]
        test   ebp, ebp
        jne    SHORT loc_4_C993
        mov    edi, dword ptr [esp + 68h]
        pop    ebp
        mov    dword ptr [edi], 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 54h
        ret    0ch
loc_4_C993:
        mov    esi, dword ptr [esp + 70h]
        cmp    dword ptr [esi + 10h], 8
        je     SHORT loc_4_C9B8
        push   esi
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    ebx, dword ptr [ebp + 0ch]
        call   dword ptr [ebx + 50h]
        mov    edi, dword ptr [esp + 68h]
        pop    ebp
        mov    dword ptr [edi], eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 54h
        ret    0ch
loc_4_C9B8:
        lea    eax, [esp + 10h]
        xor    ebx, ebx
        lea    edi, [esp + 24h]
        mov    ecx, 10h
        mov    dword ptr [eax], ebx
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax + 8], ebx
        mov    dword ptr [eax + 0ch], ebx
        mov    dword ptr [eax + 10h], ebx
        xor    eax, eax
        rep stosd
        lea    eax, [esp + 10h]
        push   28h
        mov    dword ptr [esp + 28h], eax
        push   esi
        mov    dword ptr [esp + 18h], 0eh
        mov    dword ptr [esp + 20h], 1
        mov    dword ptr [esi + 10h], 2
        lea    ecx, [esp + 30h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_CA05:
        lea    ecx, [esp + 54h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_CA0F:
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 14h]
        cmp    byte ptr [edx + 34h], bl
        je     NEAR PTR loc_4_CADC
        mov    ebp, dword ptr [eax + 0ch]
        cmp    byte ptr [ebp + 2eh], bl
        jne    SHORT loc_4_CA59
        mov    ebx, 1
        mov    byte ptr [ebp + 2eh], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_CA59
        db  03Bh, 02Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_CA59
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 019h, 003h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_4_CA59:
        lea    eax, [esp + 28h]
        push   0
        push   eax
        push   ebp
        call   sub_4_A120
        push   0
        mov    edx, 2
        push   0
        mov    ecx, ebp
        call   dword ptr [ebp + 11ch]
        test   bl, bl
        je     SHORT loc_4_CA83
        mov    ecx, ebp
        call   dword ptr [ebp + 118h]
loc_4_CA83:
        mov    edi, dword ptr [esp + 68h]
        test   bl, bl
        mov    dword ptr [edi], 103h
        je     SHORT loc_4_CAEE
        mov    byte ptr [ebp + 2eh], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_CAC4
        db  03Bh, 02Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_CAC4
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 02Fh, 003h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_CAC4:
        cmp    byte ptr [ebp + 2ch], 1
        jne    SHORT loc_4_CAEE
        mov    byte ptr [ebp + 2ch], 0
        add    ebp, 2eh
        push   ebp
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_CAD7:
        add    esp, 4
        jmp    SHORT loc_4_CAEE
loc_4_CADC:
        lea    ecx, [esp + 28h]
        mov    ebx, dword ptr [eax + 0ch]
        push   ecx
        push   ebx
        call   dword ptr [edx + 54h]
        mov    edi, dword ptr [esp + 68h]
        mov    dword ptr [edi], eax
loc_4_CAEE:
        cmp    dword ptr [edi], 103h
        jne    SHORT loc_4_CB08
        lea    eax, [esp + 54h]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_CB02:
        mov    eax, dword ptr [esp + 50h]
        mov    dword ptr [edi], eax
loc_4_CB08:
        lea    eax, [esp + 28h]
        push   28h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_CB15:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 54h
        ret    0ch
        db  090h
loc_4_CB20:
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        push   eax
        mov    eax, dword ptr [esp + 8]
        push   ecx
        push   edx
        mov    ecx, dword ptr [eax + 24h]
        push   ecx
        mov    eax, dword ptr [eax + 1ch]
        call   dword ptr [eax + 5ch]
        ret    10h
loc_4_CB40:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 24h]
        mov    eax, dword ptr [eax + 1ch]
        push   ecx
        call   dword ptr [eax + 60h]
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_CB60:
        mov    ecx, dword ptr [esp + 8]
        sub    esp, 4
        test   ecx, ecx
        push   esi
        push   edi
        je     NEAR PTR loc_4_CC0F
        mov    edx, dword ptr [esp + 18h]
        test   edx, edx
        je     NEAR PTR loc_4_CC0F
        mov    esi, dword ptr [esp + 1ch]
        test   esi, esi
        je     SHORT loc_4_CBCD
        xor    edi, edi
        db  0B8h, 004h, 000h, 000h, 000h
loc_4_CB8C:
        cmp    dword ptr [eax], 0
        je     SHORT loc_4_CB9C
        add    eax, 0ch
        inc    edi
        db  03Dh, 07Ch, 000h, 000h, 000h
        jb     SHORT loc_4_CB8C
loc_4_CB9C:
        cmp    edi, 0ah
        jb     SHORT loc_4_CBB3
        mov    ecx, dword ptr [esp + 10h]
        pop    edi
        pop    esi
        mov    dword ptr [ecx], 0c000009ah
        add    esp, 4
        ret    10h
loc_4_CBB3:
        lea    eax, [edi + edi*2]
        shl    eax, 2
        db  089h, 0B0h, 004h, 000h, 000h, 000h
        db  089h, 090h, 000h, 000h, 000h, 000h
        db  089h, 088h, 008h, 000h, 000h, 000h
        jmp    SHORT loc_4_CBD1
loc_4_CBCD:
        mov    edi, dword ptr [esp + 8]
loc_4_CBD1:
        push   edx
        mov    eax, dword ptr [ecx + 0ch]
        push   eax
        mov    eax, dword ptr [ecx + 8]
        call   dword ptr [eax + 54h]
        mov    ecx, dword ptr [esp + 10h]
        xor    edx, edx
        test   esi, esi
        mov    dword ptr [ecx], eax
        je     SHORT loc_4_CC19
        cmp    eax, 103h
        je     SHORT loc_4_CC19
        lea    eax, [edi + edi*2]
        pop    edi
        shl    eax, 2
        pop    esi
        add    esp, 4
        db  089h, 090h, 004h, 000h, 000h, 000h
        db  089h, 090h, 000h, 000h, 000h, 000h
        db  089h, 090h, 008h, 000h, 000h, 000h
        ret    10h
loc_4_CC0F:
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx], 0c0000001h
loc_4_CC19:
        pop    edi
        pop    esi
        add    esp, 4
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_CC30:
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0ch]
        sub    ecx, 4
        add    ecx, 30h
        push   ecx
        mov    dword ptr [ecx - 4], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_CC47:
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_CC50:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_CC70:
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_CC80:
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 0ch]
        add    ecx, 8
        push   ecx
        mov    dword ptr [ecx - 8], eax
        mov    dword ptr [ecx - 4], edx
        mov    dword ptr [ecx], 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_CCA1:
        add    esp, 4
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_CCB0:
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 4]
        add    eax, 8
        push   eax
        mov    dword ptr [eax - 8], ecx
        mov    dword ptr [eax], 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_CCCA:
        add    esp, 4
        ret    8
sub_4_CCD0:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_CCE0:
        push   esi
        db  0A1h, 004h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_4_CCFB
loc_4_CCEA:
        mov    esi, dword ptr [eax + 4]
        push   1
        push   eax
        call   sub_4_6E90
        mov    eax, esi
        test   esi, esi
        jne    SHORT loc_4_CCEA
loc_4_CCFB:
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_4_CD00:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_CD10:
        push   ebx
        push   esi
        xor    si, si
        push   edi
        mov    edi, dword ptr [esp + 14h]
        push   ebp
loc_4_CD1B:
        mov    ax, si
        movzx  ecx, ax
        inc    si
        cmp    byte ptr [ecx + edi], 0
        jne    SHORT loc_4_CD1B
        mov    ebp, dword ptr [esp + 14h]
        push   0
        movzx  ebx, si
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_CD38:
        add    esp, 8
        mov    dword ptr [ebp + 4], eax
        test   eax, eax
        je     SHORT loc_4_CD5B
        mov    word ptr [ebp + 2], si
        push   ebx
        mov    word ptr [ebp], si
        push   edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_CD52:
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_4_CD5B:
        mov    eax, 0c000009ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_CD70:
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        mov    edi, dword ptr [esp + 18h]
        push   ebp
        mov    ax, word ptr [edi]
        xor    ebx, ebx
        mov    word ptr [esp + 12h], ax
        push   ebx
        mov    eax, dword ptr [esp + 16h]
        and    eax, 0ffffh
        add    eax, eax
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_CD9B:
        add    esp, 8
        mov    dword ptr [esi + 4], eax
        cmp    eax, ebx
        je     SHORT loc_4_CDF7
        mov    ax, word ptr [esp + 12h]
        add    ax, ax
        xor    cx, cx
        mov    word ptr [esi + 2], ax
        sub    ax, 2
        mov    word ptr [esi], ax
        cmp    word ptr [esp + 12h], bx
        je     SHORT loc_4_CDEB
loc_4_CDC2:
        mov    eax, dword ptr [edi + 4]
        mov    ebp, dword ptr [esi + 4]
        movzx  edx, bx
        inc    bx
        mov    al, byte ptr [edx + eax]
        movzx  edx, cx
        inc    cx
        mov    byte ptr [edx + ebp], al
        movzx  eax, cx
        inc    cx
        mov    ebp, dword ptr [esi + 4]
        cmp    bx, word ptr [esp + 12h]
        mov    byte ptr [eax + ebp], 0
        jb     SHORT loc_4_CDC2
loc_4_CDEB:
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
loc_4_CDF7:
        mov    eax, 0c000009ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_CE10:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        mov    ecx, dword ptr [esp + 0ch]
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 4]
        cmp    byte ptr [esp + 18h], 0
        mov    edi, dword ptr [ecx + 4]
        je     SHORT loc_4_CEA3
loc_4_CE28:
        mov    cl, byte ptr [esi]
        cmp    cl, 61h
        jb     SHORT loc_4_CE3D
        cmp    cl, 7ah
        ja     SHORT loc_4_CE3D
        xor    ebx, ebx
        mov    bl, cl
        sub    ebx, 20h
        jmp    SHORT loc_4_CE41
loc_4_CE3D:
        xor    ebx, ebx
        mov    bl, cl
loc_4_CE41:
        mov    al, byte ptr [edi]
        cmp    al, 61h
        jb     SHORT loc_4_CE54
        cmp    al, 7ah
        ja     SHORT loc_4_CE54
        xor    edx, edx
        mov    dl, al
        sub    edx, 20h
        jmp    SHORT loc_4_CE58
loc_4_CE54:
        xor    edx, edx
        mov    dl, al
loc_4_CE58:
        cmp    ebx, edx
        jne    SHORT loc_4_CE64
        test   cl, cl
        je     SHORT loc_4_CE64
        inc    esi
        inc    edi
        jmp    SHORT loc_4_CE28
loc_4_CE64:
        mov    al, byte ptr [esi]
        cmp    al, 61h
        jb     SHORT loc_4_CE77
        cmp    al, 7ah
        ja     SHORT loc_4_CE77
        xor    edx, edx
        mov    dl, al
        sub    edx, 20h
        jmp    SHORT loc_4_CE7B
loc_4_CE77:
        xor    edx, edx
        mov    dl, al
loc_4_CE7B:
        mov    cl, byte ptr [edi]
        cmp    cl, 61h
        jb     SHORT loc_4_CE90
        cmp    cl, 7ah
        ja     SHORT loc_4_CE90
        xor    eax, eax
        mov    al, cl
        sub    eax, 20h
        jmp    SHORT loc_4_CE94
loc_4_CE90:
        xor    eax, eax
        mov    al, cl
loc_4_CE94:
        sub    edx, eax
        pop    edi
        pop    esi
        cmp    edx, 1
        sbb    al, al
        pop    ebx
        neg    al
        ret    0ch
loc_4_CEA3:
        mov    al, byte ptr [esi]
        cmp    byte ptr [edi], al
        jne    SHORT loc_4_CEB7
loc_4_CEA9:
        cmp    byte ptr [esi], 0
        je     SHORT loc_4_CEB7
        inc    edi
        mov    al, byte ptr [esi + 1]
        inc    esi
        cmp    byte ptr [edi], al
        je     SHORT loc_4_CEA9
loc_4_CEB7:
        mov    al, byte ptr [edi]
        pop    edi
        sub    al, byte ptr [esi]
        pop    esi
        pop    ebx
        cmp    al, 1
        sbb    al, al
        neg    al
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_CED0:
        mov    eax, dword ptr [esp + 8]
        test   eax, eax
        je     SHORT loc_4_CEF2
        cmp    eax, 1
        je     SHORT loc_4_CEFF
        cmp    eax, 2
        mov    eax, dword ptr [esp + 4]
        je     SHORT loc_4_CF0C
        mov    dword ptr [eax], 0
        mov    eax, 0c0000001h
        ret
loc_4_CEF2:
        mov    eax, dword ptr [esp + 4]
        db  0C7h, 000h, 000h, 000h, 000h, 000h
        xor    eax, eax
        ret
loc_4_CEFF:
        mov    eax, dword ptr [esp + 4]
        db  0C7h, 000h, 000h, 000h, 000h, 000h
        xor    eax, eax
        ret
loc_4_CF0C:
        db  0C7h, 000h, 000h, 000h, 000h, 000h
        xor    eax, eax
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
_PTEXT ends
        end
