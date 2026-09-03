; vserver_lcode_01_04.asm -- VSERVER object 1 segment _LTEXT  (part 4/5)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_DA2C:
        push   esi
        sub    esi, esi
        nop
loc_1_DA30:
        db  08Bh, 004h, 0B5h, 0B0h, 01Bh, 001h, 000h
        cmp    eax, 0fffff000h
        je     SHORT loc_1_DA63
        db  0F6h, 086h, 0B8h, 01Ch, 001h, 000h, 040h
        je     SHORT loc_1_DA63
        push   eax
        call   sub_1_F664
        add    esp, 4
        push   esi
        call   sub_1_7115
        add    esp, 4
        cmp    esi, 10h
        jne    SHORT loc_1_DA63
        call   sub_1_7684
loc_1_DA63:
        inc    esi
        db  080h, 0A6h, 0B7h, 01Ch, 001h, 000h, 0BFh
        cmp    esi, 11h
        jl     SHORT loc_1_DA30
        db  083h, 025h, 038h, 034h, 001h, 000h, 0DFh
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_1_DA7C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    edi, edi
        cmp    dword ptr [ebp + 8], edi
        jne    SHORT loc_1_DA94
        db  0C6h, 005h, 040h, 034h, 001h, 000h, 000h
        jmp    SHORT loc_1_DAA0
        db  08Dh, 049h, 000h
loc_1_DA94:
        mov    edi, 200h
        db  0C6h, 005h, 040h, 034h, 001h, 000h, 001h
loc_1_DAA0:
        db  0A1h, 038h, 034h, 001h, 000h
        test   al, 10h
        je     SHORT loc_1_DAE6
        test   al, 4
        je     SHORT loc_1_DAE6
        sub    esi, esi
        nop
loc_1_DAB0:
        db  081h, 03Ch, 0B5h, 0B0h, 01Bh, 001h, 000h, 000h, 0F0h, 0FFh, 0FFh
        je     SHORT loc_1_DAE0
        db  0F6h, 086h, 0B8h, 01Ch, 001h, 000h, 040h
        jne    SHORT loc_1_DAE0
        push   esi
        push   edi
        push   200h
        call   sub_1_E8CF
        add    esp, 0ch
        push   0ah
        push   esi
        call   sub_1_E8ED
        add    esp, 8
loc_1_DAE0:
        inc    esi
        cmp    esi, 11h
        jl     SHORT loc_1_DAB0
loc_1_DAE6:
        pop    edi
        pop    esi
        leave
        ret
        db  08Bh, 0FFh
sub_1_DAEC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    eax, dword ptr [ebp + 0ch]
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_DB17
        mov    dl, byte ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 10h]
loc_1_DB01:
        mov    byte ptr [ebp - 1], dl
        ror    byte ptr [ebp - 1], 1
        mov    dl, byte ptr [ebp - 1]
        mov    bl, byte ptr [eax]
        xor    bl, dl
        mov    byte ptr [ecx], bl
        inc    eax
        inc    ecx
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_DB01
loc_1_DB17:
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
sub_1_DB1C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 14h]
        or     esi, esi
        je     SHORT loc_1_DB50
        mov    dl, byte ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
loc_1_DB34:
        mov    byte ptr [ebp - 1], dl
        ror    byte ptr [ebp - 1], 1
        mov    dl, byte ptr [ebp - 1]
        mov    bl, byte ptr [ecx]
        xor    bl, dl
        mov    byte ptr [eax], bl
        inc    ecx
        inc    eax
        dec    esi
        jne    SHORT loc_1_DB34
        mov    byte ptr [eax], 0
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_DB50:
        pop    esi
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax], 0
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
sub_1_DB5C:
        push   ebp
        mov    ebp, esp
        sub    esp, 104h
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        cmp    ecx, 10h
        jbe    SHORT loc_1_DB88
        mov    eax, 57h
        pop    edi
        pop    esi
        leave
        ret
        db  08Bh, 0FFh
loc_1_DB88:
        db  0BEh, 0A0h, 034h, 001h, 000h
        lea    edi, [ebp - 104h]
        mov    ecx, 0eh
        rep movsd
        movsb
        mov    byte ptr [ebp - 0cch], 5ch
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebp - 0cbh]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 0b0h], eax
        lea    eax, [ebp - 104h]
        mov    dword ptr [ebp - 0ach], eax
        mov    dword ptr [ebp - 0a8h], 80000002h
        push   dword ptr [ebp - 0b0h]
        push   dword ptr [ebp - 0ach]
        push   dword ptr [ebp - 0a8h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 04Ah, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_1_DEF3
        mov    dword ptr [ebp - 0a4h], 4
        lea    eax, [ebp + 20h]
        mov    dword ptr [ebp - 0a0h], eax
        mov    dword ptr [ebp - 9ch], 4
        mov    dword ptr [ebp - 98h], 0
        db  0C7h, 085h, 06Ch, 0FFh, 0FFh, 0FFh, 0DCh, 034h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 90h], eax
        push   dword ptr [ebp - 0a4h]
        push   dword ptr [ebp - 0a0h]
        push   dword ptr [ebp - 9ch]
        push   dword ptr [ebp - 98h]
        push   dword ptr [ebp - 94h]
        push   dword ptr [ebp - 90h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        add    esp, 18h
        mov    esi, eax
        or     esi, esi
        jne    NEAR PTR loc_1_DEB4
        mov    dword ptr [ebp - 8ch], 4
        lea    eax, [ebp + 1ch]
        mov    dword ptr [ebp - 88h], eax
        mov    dword ptr [ebp - 84h], 4
        mov    dword ptr [ebp - 80h], 0
        db  0C7h, 045h, 084h, 0E4h, 034h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 78h], eax
        push   dword ptr [ebp - 8ch]
        push   dword ptr [ebp - 88h]
        push   dword ptr [ebp - 84h]
        push   dword ptr [ebp - 80h]
        push   dword ptr [ebp - 7ch]
        push   dword ptr [ebp - 78h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        add    esp, 18h
        mov    esi, eax
        or     esi, esi
        jne    NEAR PTR loc_1_DEB4
        mov    edx, dword ptr [ebp + 0ch]
        or     edx, edx
        je     SHORT loc_1_DD4B
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    dword ptr [ebp - 74h], ecx
        mov    dword ptr [ebp - 70h], edx
        mov    dword ptr [ebp - 6ch], 1
        mov    dword ptr [ebp - 68h], 0
        db  0C7h, 045h, 09Ch, 0ECh, 034h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 60h], eax
        push   dword ptr [ebp - 74h]
        push   dword ptr [ebp - 70h]
        push   dword ptr [ebp - 6ch]
        push   dword ptr [ebp - 68h]
        push   dword ptr [ebp - 64h]
        push   dword ptr [ebp - 60h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        add    esp, 18h
        mov    esi, eax
        or     esi, esi
        jne    NEAR PTR loc_1_DEB4
loc_1_DD4B:
        mov    edx, dword ptr [ebp + 14h]
        or     edx, edx
        je     SHORT loc_1_DDC6
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    esi, [ecx - 1]
        lea    eax, [ebp - 0b8h]
        push   eax
        push   edx
        push   6ah
        call   sub_1_DAEC
        add    esp, 0ch
        mov    dword ptr [ebp - 5ch], esi
        lea    eax, [ebp - 0b8h]
        mov    dword ptr [ebp - 58h], eax
        mov    dword ptr [ebp - 54h], 3
        mov    dword ptr [ebp - 50h], 0
        db  0C7h, 045h, 0B4h, 0F4h, 034h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 48h], eax
        push   dword ptr [ebp - 5ch]
        push   dword ptr [ebp - 58h]
        push   dword ptr [ebp - 54h]
        push   dword ptr [ebp - 50h]
        push   dword ptr [ebp - 4ch]
        push   dword ptr [ebp - 48h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        add    esp, 18h
        mov    esi, eax
        or     esi, esi
        jne    NEAR PTR loc_1_DEB4
loc_1_DDC6:
        mov    edx, dword ptr [ebp + 18h]
        or     edx, edx
        je     SHORT loc_1_DE3D
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    esi, [ecx - 1]
        lea    eax, [ebp - 0b8h]
        push   eax
        push   edx
        push   6ah
        call   sub_1_DAEC
        add    esp, 0ch
        mov    dword ptr [ebp - 44h], esi
        lea    eax, [ebp - 0b8h]
        mov    dword ptr [ebp - 40h], eax
        mov    dword ptr [ebp - 3ch], 3
        mov    dword ptr [ebp - 38h], 0
        db  0C7h, 045h, 0CCh, 000h, 035h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 30h], eax
        push   dword ptr [ebp - 44h]
        push   dword ptr [ebp - 40h]
        push   dword ptr [ebp - 3ch]
        push   dword ptr [ebp - 38h]
        push   dword ptr [ebp - 34h]
        push   dword ptr [ebp - 30h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        add    esp, 18h
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_DEB4
loc_1_DE3D:
        mov    edx, dword ptr [ebp + 10h]
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    dword ptr [ebp - 2ch], ecx
        mov    dword ptr [ebp - 28h], edx
        mov    dword ptr [ebp - 24h], 1
        mov    dword ptr [ebp - 20h], 0
        db  0C7h, 045h, 0E4h, 00Ch, 035h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 18h], eax
        push   dword ptr [ebp - 2ch]
        push   dword ptr [ebp - 28h]
        push   dword ptr [ebp - 24h]
        push   dword ptr [ebp - 20h]
        push   dword ptr [ebp - 1ch]
        push   dword ptr [ebp - 18h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        add    esp, 18h
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_DEB4
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 14h], eax
        push   dword ptr [ebp - 14h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        sub    eax, eax
        pop    edi
        pop    esi
        leave
        ret
        db  090h
loc_1_DEB4:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 10h], eax
        push   dword ptr [ebp - 10h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        lea    eax, [ebp - 104h]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 8], 80000002h
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 04Bh, 001h, 001h, 000h
        add    esp, 8
        mov    eax, esi
loc_1_DEF3:
        pop    edi
        pop    esi
        leave
        ret
        db  090h
sub_1_DEF8:
        push   ebp
        mov    ebp, esp
        sub    esp, 54h
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        cmp    ecx, 10h
        jbe    SHORT loc_1_DF20
        mov    eax, 57h
        pop    edi
        pop    esi
        leave
        ret
        db  090h
loc_1_DF20:
        db  0BEh, 0C0h, 035h, 001h, 000h
        lea    edi, [ebp - 54h]
        mov    ecx, 0eh
        rep movsd
        movsb
        mov    byte ptr [ebp - 1ch], 5ch
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebp - 1bh]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        lea    eax, [ebp - 54h]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 4], 80000002h
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 04Bh, 001h, 001h, 000h
        add    esp, 8
        pop    edi
        pop    esi
        leave
        ret
sub_1_DF7C:
        push   ebp
        mov    ebp, esp
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 18h]
        lea    eax, [esi + 1]
        mov    dword ptr [ebp - 24h], eax
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 30h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 2ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 28h], eax
        push   dword ptr [ebp - 30h]
        push   dword ptr [ebp - 2ch]
        push   dword ptr [ebp - 28h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_E026
        lea    eax, [ebp - 24h]
        mov    dword ptr [ebp - 20h], eax
        mov    ebx, dword ptr [ebp + 14h]
        mov    dword ptr [ebp - 1ch], ebx
        lea    eax, [ebp - 34h]
        mov    dword ptr [ebp - 18h], eax
        mov    dword ptr [ebp - 14h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
        push   dword ptr [ebp - 20h]
        push   dword ptr [ebp - 1ch]
        push   dword ptr [ebp - 18h]
        push   dword ptr [ebp - 14h]
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_E00E
        mov    byte ptr [ebx + esi], 0
loc_1_E00E:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [ebp - 8]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
loc_1_E026:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
sub_1_E030:
        push   ebp
        mov    ebp, esp
        sub    esp, 54h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 10h], 4
        mov    edi, dword ptr [ebp + 0ch]
        mov    dword ptr [edi], 1
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi], 1
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 54h], eax
        db  0C7h, 045h, 0B0h, 038h, 036h, 001h, 000h
        mov    dword ptr [ebp - 4ch], 80000002h
        push   dword ptr [ebp - 54h]
        push   dword ptr [ebp - 50h]
        push   dword ptr [ebp - 4ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        mov    ebx, eax
        or     ebx, ebx
        jne    NEAR PTR loc_1_E152
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 48h], eax
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 44h], eax
        lea    eax, [ebp - 30h]
        mov    dword ptr [ebp - 40h], eax
        mov    dword ptr [ebp - 3ch], 0
        db  0C7h, 045h, 0C8h, 074h, 036h, 001h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 34h], eax
        push   dword ptr [ebp - 48h]
        push   dword ptr [ebp - 44h]
        push   dword ptr [ebp - 40h]
        push   dword ptr [ebp - 3ch]
        push   dword ptr [ebp - 38h]
        push   dword ptr [ebp - 34h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_E0DF
        cmp    dword ptr [ebp - 0ch], 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [esi], eax
loc_1_E0DF:
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 2ch], eax
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 28h], eax
        lea    eax, [ebp - 30h]
        mov    dword ptr [ebp - 24h], eax
        mov    dword ptr [ebp - 20h], 0
        db  0C7h, 045h, 0E4h, 084h, 036h, 001h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 18h], eax
        push   dword ptr [ebp - 2ch]
        push   dword ptr [ebp - 28h]
        push   dword ptr [ebp - 24h]
        push   dword ptr [ebp - 20h]
        push   dword ptr [ebp - 1ch]
        push   dword ptr [ebp - 18h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_1_E138
        cmp    dword ptr [ebp - 0ch], 1
        sbb    ecx, ecx
        neg    ecx
        mov    dword ptr [edi], ecx
        or     eax, eax
        je     SHORT loc_1_E13A
loc_1_E138:
        mov    ebx, eax
loc_1_E13A:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 14h], eax
        push   dword ptr [ebp - 14h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
loc_1_E152:
        push   3
        lea    eax, [ebp - 4]
        push   eax
        db  068h, 094h, 036h, 001h, 000h
        db  068h, 0A0h, 036h, 001h, 000h
        push   80000002h
        call   sub_1_DF7C
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_1_E17F
        cmp    byte ptr [ebp - 4], 4eh
        jne    SHORT loc_1_E17F
        mov    dword ptr [esi], 0
loc_1_E17F:
        push   3
        lea    eax, [ebp - 4]
        push   eax
        db  068h, 0D0h, 036h, 001h, 000h
        db  068h, 0E0h, 036h, 001h, 000h
        push   80000002h
        call   sub_1_DF7C
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_1_E1AC
        cmp    byte ptr [ebp - 4], 4eh
        jne    SHORT loc_1_E1AC
        mov    dword ptr [edi], 0
loc_1_E1AC:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
sub_1_E1B4:
        db  083h, 03Dh, 09Ch, 034h, 001h, 000h, 000h
        jne    SHORT loc_1_E1D9
        db  068h, 094h, 034h, 001h, 000h
        db  068h, 098h, 034h, 001h, 000h
        call   sub_1_E030
        add    esp, 8
        db  0C7h, 005h, 09Ch, 034h, 001h, 000h, 001h, 000h, 000h, 000h
loc_1_E1D9:
        ret
        db  08Bh, 0FFh
sub_1_E1DC:
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        clc
        test   al, al
        je     SHORT loc_1_E23E
        db  0A2h, 05Ch, 039h, 001h, 000h
        push   1
        push   1
        db  068h, 05Ch, 039h, 001h, 000h
        db  068h, 058h, 039h, 001h, 000h
        db  0CDh, 020h, 042h, 000h, 040h, 000h
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_1_E20F
        test   edx, 2
        je     SHORT loc_1_E216
        stc
loc_1_E20F:
        db  0A0h, 05Ch, 039h, 001h, 000h
        jmp    SHORT loc_1_E23E
loc_1_E216:
        push   2
        db  068h, 058h, 039h, 001h, 000h
        db  068h, 058h, 039h, 001h, 000h
        db  0CDh, 020h, 043h, 000h, 040h, 000h
        add    esp, 0ch
        db  0FFh, 035h, 058h, 039h, 001h, 000h
        db  0CDh, 020h, 044h, 000h, 040h, 000h
        add    esp, 4
        test   ah, ah
        jne    SHORT loc_1_E20F
loc_1_E23E:
        movzx  eax, al
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret
sub_1_E247:
        push   ebx
        push   esi
        push   edi
        cld
        db  0BFh, 058h, 037h, 001h, 000h
        db  0BBh, 058h, 038h, 001h, 000h
        sub    ebx, edi
        mov    ecx, 100h
loc_1_E25C:
        mov    al, byte ptr [edi]
        call   sub_1_E1DC
        jae    SHORT loc_1_E274
        db  0C7h, 005h, 04Ch, 037h, 001h, 000h, 001h, 000h, 000h, 000h
        mov    al, 2
        mov    byte ptr [ebx + edi], al
loc_1_E274:
        stosb
        loop   loc_1_E25C
        xor    eax, eax
        movzx  eax, al
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_E280:
        db  083h, 03Dh, 04Ch, 037h, 001h, 000h, 000h
        jne    SHORT loc_1_E28A
loc_1_E289:
        ret
loc_1_E28A:
        jecxz  loc_1_E289
        cmp    byte ptr [esi], 0
        je     SHORT loc_1_E289
        pushfd
        push   edi
        push   eax
        mov    edi, esi
        dec    esi
        std
        xor    eax, eax
loc_1_E29A:
        dec    ecx
        je     SHORT loc_1_E2AF
        lodsb
        test   eax, eax
        je     SHORT loc_1_E2BC
        db  080h, 0B8h, 058h, 037h, 001h, 000h, 002h
        je     SHORT loc_1_E29A
        lea    esi, [esi + 2]
        inc    ecx
loc_1_E2AF:
        sub    edi, esi
        and    edi, 0fffffffeh
        add    esi, edi
        add    ecx, edi
loc_1_E2B8:
        pop    eax
        pop    edi
        popfd
        ret
loc_1_E2BC:
        inc    esi
        jmp    SHORT loc_1_E2B8
        db  0E3h, 006h, 04Eh, 049h, 074h, 002h, 0EBh, 0B9h, 0C3h
sub_1_E2C8:
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        mov    ecx, esi
        sub    ecx, dword ptr [esp + 8]
        je     SHORT loc_1_E2DE
        dec    esi
        dec    ecx
        je     SHORT loc_1_E2DE
        call   sub_1_E280
loc_1_E2DE:
        mov    eax, esi
        pop    esi
        ret
        db  056h, 08Bh, 074h, 024h, 00Ch, 08Bh, 0CEh, 02Bh, 04Ch, 024h, 008h, 074h, 005h, 0E8h, 08Ch, 0FFh
        db  0FFh, 0FFh, 08Bh, 0C6h, 05Eh, 0C3h, 056h, 08Bh, 04Ch, 024h, 00Ch, 0E3h, 00Fh, 08Bh, 074h, 024h
        db  008h, 003h, 0F1h, 04Eh, 049h, 074h, 005h, 0E8h, 072h, 0FFh, 0FFh, 0FFh, 08Bh, 0C1h, 05Eh, 0C3h
        db  056h, 08Bh, 04Ch, 024h, 00Ch, 0E3h, 00Bh, 08Bh, 074h, 024h, 008h, 003h, 0F1h, 0E8h, 05Ch, 0FFh
        db  0FFh, 0FFh, 08Bh, 0C1h, 05Eh, 0C3h, 051h, 056h, 057h, 08Bh, 0FEh, 0E8h, 04Eh, 0FFh, 0FFh, 0FFh
        db  03Bh, 0FEh, 05Fh, 05Eh, 059h, 0C3h
sub_1_E338:
        push   ebx
        push   edi
        jecxz  loc_1_E35A
        mov    edi, esi
        std
loc_1_E33F:
        dec    edi
        repne scasb
        jne    SHORT loc_1_E35A
        inc    edi
        mov    esi, edi
        mov    ebx, ecx
        call   sub_1_E280
        cmp    esi, edi
        jne    SHORT loc_1_E356
loc_1_E352:
        cld
        pop    edi
        pop    ebx
        ret
loc_1_E356:
        mov    ecx, ebx
        jmp    SHORT loc_1_E33F
loc_1_E35A:
        stc
        jmp    SHORT loc_1_E352
        db  02Eh, 08Bh, 0C0h, 057h, 08Bh, 07Ch, 024h, 008h, 00Fh, 0B6h, 00Fh, 0E3h, 013h, 047h, 08Ah, 044h
        db  024h, 00Ch, 08Ah, 054h, 024h, 010h, 0FCh, 0F2h, 0AEh, 0E3h, 005h, 088h, 057h, 0FFh, 0EBh, 0F7h
        db  05Fh, 0C3h, 090h, 083h, 03Dh, 04Ch, 037h, 001h, 000h, 000h, 074h, 0D7h, 056h, 08Bh, 074h, 024h
        db  008h, 00Fh, 0B6h, 00Eh, 0E3h, 022h, 046h, 08Ah, 074h, 024h, 00Ch, 08Ah, 054h, 024h, 010h, 033h
        db  0C0h, 0ACh, 084h, 0C0h, 074h, 012h, 080h, 0B8h, 058h, 037h, 001h, 000h, 002h, 074h, 00Bh, 03Ah
        db  0C6h, 075h, 003h, 088h, 056h, 0FFh, 0E2h, 0E9h, 05Eh, 0C3h, 046h, 049h, 0E3h, 0FAh, 0E2h, 0E1h
        db  0EBh, 0F6h, 090h, 053h, 056h, 057h, 033h, 0C0h, 08Bh, 04Ch, 024h, 018h, 0E3h, 028h, 08Bh, 07Ch
        db  024h, 010h, 08Bh, 074h, 024h, 014h, 0BBh, 058h, 037h, 001h, 000h, 0FCh, 0ACh, 084h, 0C0h, 074h
        db  01Ch, 08Bh, 0D0h, 0D7h, 03Ch, 002h, 074h, 01Ah, 08Bh, 0D0h, 08Ah, 007h, 0D7h, 03Ah, 0C2h, 075h
        db  005h, 047h, 0E2h, 0E8h, 033h, 0C0h, 00Fh, 095h, 0C0h, 05Fh, 05Eh, 05Bh, 0C3h, 080h, 03Fh, 000h
        db  0EBh, 0F4h, 03Ah, 017h, 075h, 0F0h, 049h, 0E3h, 0EBh, 0ACh, 047h, 03Ah, 007h, 075h, 0E7h, 084h
        db  0C0h, 074h, 0E3h, 0EBh, 0DCh, 08Bh, 0C0h, 053h, 056h, 057h, 08Bh, 07Ch, 024h, 010h, 08Bh, 074h
        db  024h, 014h, 08Bh, 04Ch, 024h, 018h, 0BBh, 058h, 037h, 001h, 000h, 0FCh, 0E3h, 00Fh, 0ACh, 084h
        db  0C0h, 074h, 00Eh, 08Bh, 0D0h, 0D7h, 03Ch, 002h, 074h, 00Ah, 0AAh, 0E2h, 0F1h, 05Fh, 05Eh, 05Bh
        db  0C3h, 0AAh, 0EBh, 0F9h, 08Bh, 0C2h, 0AAh, 049h, 0E3h, 0F3h, 080h, 03Eh, 000h, 0A4h, 074h, 0EBh
sub_1_E44D:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 18h]
        db  0BBh, 058h, 037h, 001h, 000h
        cld
        jecxz  loc_1_E49A
loc_1_E464:
        lodsb
        test   al, al
        je     SHORT loc_1_E49E
        mov    edx, eax
        xlatb
        cmp    al, 2
        je     SHORT loc_1_E4A1
        cmp    al, dl
        jne    SHORT loc_1_E495
        push   ecx
        push   edi
        mov    ecx, 100h
        db  0BFh, 058h, 037h, 001h, 000h
        add    edi, ecx
        dec    edi
        std
        repne scasb
        jecxz  loc_1_E492
        inc    edi
        db  0BAh, 058h, 037h, 001h, 000h
        xchg   edi, edx
        sub    edx, edi
loc_1_E492:
        pop    edi
        pop    ecx
        cld
loc_1_E495:
        mov    al, dl
        stosb
loc_1_E498:
        loop   loc_1_E464
loc_1_E49A:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_E49E:
        stosb
        jmp    SHORT loc_1_E49A
loc_1_E4A1:
        mov    eax, edx
        stosb
        dec    ecx
        jecxz  loc_1_E49A
        cmp    byte ptr [esi], 0
        movsb
        je     SHORT loc_1_E498
        mov    eax, eax
sub_1_E4B0:
        jecxz  loc_1_E4D5
        push   eax
        push   ebx
        push   edx
        db  0BBh, 058h, 037h, 001h, 000h
        cld
loc_1_E4BB:
        lodsb
        mov    edx, eax
        xlatb
        cmp    al, 2
        jne    SHORT loc_1_E4CF
        mov    eax, edx
        stosb
        dec    ecx
        jecxz  loc_1_E4D6
        cmp    byte ptr [esi], 0
        movsb
        jmp    SHORT loc_1_E4D0
loc_1_E4CF:
        stosb
loc_1_E4D0:
        loop   loc_1_E4BB
loc_1_E4D2:
        pop    edx
        pop    ebx
        pop    eax
loc_1_E4D5:
        ret
loc_1_E4D6:
        jmp    SHORT loc_1_E4D2
sub_1_E4D8:
        mov    ecx, dword ptr [esp + 0ch]
        jecxz  loc_1_E4F1
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 14h]
        call   sub_1_E4B0
        pop    edi
        pop    esi
        pop    ebx
loc_1_E4F1:
        ret
        db  08Bh, 0C0h
sub_1_E4F4:
        push   edi
        push   eax
        push   ecx
        push   edx
        push   ebp
        mov    ebp, esp
        sub    esp, 0dh
        cld
        mov    edi, esi
        xor    eax, eax
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        cmp    ecx, 0dh
        jle    SHORT loc_1_E513
        jmp    SHORT loc_1_E58C
loc_1_E513:
        mov    edi, ebp
        push   esi
        db  0BBh, 058h, 037h, 001h, 000h
        mov    ecx, 0dh
        rep movsb
        mov    esi, ebp
        pop    edi
        sub    ecx, ecx
loc_1_E527:
        lodsb
        cmp    al, 2ah
        je     SHORT loc_1_E542
        cmp    al, 2eh
        je     SHORT loc_1_E552
        cmp    al, 0
        je     SHORT loc_1_E58B
        mov    edx, eax
        xlatb
        cmp    al, 2
        jne    SHORT loc_1_E54E
        mov    eax, edx
        stosb
        inc    ecx
        lodsb
        jmp    SHORT loc_1_E54E
loc_1_E542:
        mov    al, 3fh
        sub    ecx, 9
        not    ecx
loc_1_E549:
        stosb
        loop   loc_1_E549
        jmp    SHORT loc_1_E552
loc_1_E54E:
        inc    ecx
        stosb
        jmp    SHORT loc_1_E527
loc_1_E552:
        cmp    al, 2eh
        je     SHORT loc_1_E561
loc_1_E556:
        lodsb
        cmp    al, 2eh
        je     SHORT loc_1_E561
        cmp    al, 0
        je     SHORT loc_1_E58B
        jmp    SHORT loc_1_E556
loc_1_E561:
        stosb
        sub    ecx, ecx
loc_1_E564:
        lodsb
        cmp    al, 2ah
        je     SHORT loc_1_E57B
        cmp    al, 0
        je     SHORT loc_1_E58B
        mov    edx, eax
        xlatb
        cmp    al, 2
        jne    SHORT loc_1_E587
        mov    eax, edx
        stosb
        inc    ecx
        lodsb
        jmp    SHORT loc_1_E587
loc_1_E57B:
        mov    al, 3fh
        sub    ecx, 4
        not    ecx
loc_1_E582:
        stosb
        loop   loc_1_E582
        jmp    SHORT loc_1_E552
loc_1_E587:
        inc    ecx
        stosb
        jmp    SHORT loc_1_E564
loc_1_E58B:
        stosb
loc_1_E58C:
        add    esp, 0dh
        pop    ebp
        pop    edx
        pop    ecx
        pop    eax
        pop    edi
        ret
        db  000h, 000h, 000h
sub_1_E598:
        push   ebx
        db  0CDh, 020h, 000h, 000h, 081h, 004h
        pop    ebx
        jb     SHORT loc_1_E5A7
        cmp    eax, 86h
loc_1_E5A7:
        cmc
        sbb    eax, eax
        ret
sub_1_E5AB:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [esp + 0ch]
        db  0CDh, 020h, 001h, 000h, 032h, 000h
        pop    esi
        sbb    eax, eax
        not    eax
        ret
sub_1_E5C0:
        db  068h, 0A0h, 015h, 001h, 000h
        db  068h, 0A4h, 0F7h, 000h, 000h
        db  068h, 0DCh, 0E9h, 000h, 000h
        db  0CDh, 020h, 005h, 000h, 081h, 004h
        add    esp, 0ch
        db  0A3h, 070h, 050h, 001h, 000h
        push   415h
        push   4
        db  0CDh, 020h, 001h, 000h, 081h, 004h
        add    esp, 8
        ret
sub_1_E5EE:
        push   ebx
        db  0B8h, 0D8h, 0E6h, 000h, 000h
        call   sub_1_89F1
        pop    ebx
        ret
sub_1_E5FB:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        call   sub_1_8D2A
        add    esp, 4
        mov    ecx, eax
        or     ecx, edx
        je     SHORT loc_1_E616
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    eax, edx
loc_1_E616:
        pop    ebp
        ret
sub_1_E618:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_1_8E42
        add    esp, 8
        pop    ebp
        ret
sub_1_E62B:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        test   eax, eax
        je     SHORT loc_1_E63E
        push   eax
        call   sub_1_8DD1
        add    esp, 4
loc_1_E63E:
        leave
        ret
sub_1_E640:
        mov    eax, dword ptr [esp + 4]
        push   ecx
        push   edx
        push   eax
        call   sub_1_8CB6
        pop    edx
        pop    edx
        pop    ecx
        cld
        ret
sub_1_E651:
        mov    eax, dword ptr [esp + 4]
        push   ecx
        push   edx
        push   eax
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        cld
        ret
sub_1_E662:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        call   sub_1_75B4
        cld
        test   al, al
        jne    SHORT loc_1_E67A
loc_1_E675:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_E67A:
        push   eax
        call   sub_1_781C
        pop    eax
        jmp    SHORT loc_1_E675
loc_1_E683:
        mov    al, 0
        db  086h, 005h, 064h, 039h, 001h, 000h
        test   al, al
        je     SHORT loc_1_E6BB
        mov    ecx, 0bh
        db  0BEh, 016h, 0E7h, 000h, 000h
        mov    eax, 1000h
        db  080h, 03Dh, 09Ch, 039h, 001h, 000h, 000h
        jne    SHORT loc_1_E6BD
        db  0CDh, 020h, 002h, 000h, 032h, 000h
        test   ebx, ebx
        je     SHORT loc_1_E6C7
loc_1_E6B1:
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        test   esi, esi
        je     SHORT loc_1_E6CF
loc_1_E6BB:
        sti
        ret
loc_1_E6BD:
        mov    ecx, 9
        mov    eax, 100000h
loc_1_E6C7:
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        jmp    SHORT loc_1_E6B1
loc_1_E6CF:
        db  0C6h, 005h, 064h, 039h, 001h, 000h, 001h
        sti
        ret
loc_1_E6D8:
        jmp    SHORT loc_1_E6FD
loc_1_E6DA:
        db  0BEh, 070h, 039h, 001h, 000h
        call   sub_1_E729
        sti
        db  0A1h, 020h, 01Ch, 001h, 000h
        cmp    byte ptr [eax], 2
        je     NEAR PTR loc_1_8780
        db  0B8h, 078h, 039h, 001h, 000h
        call   sub_1_875C
loc_1_E6FD:
        db  0A1h, 020h, 01Ch, 001h, 000h
        cmp    byte ptr [eax], 2
        jne    SHORT loc_1_E6DA
        jmp    NEAR PTR loc_1_8780
sub_1_E70C:
        db  0B8h, 078h, 039h, 001h, 000h
        jmp    NEAR PTR sub_1_8BC0
loc_1_E716:
        db  0BEh, 068h, 039h, 001h, 000h
        call   sub_1_E729
        db  0C6h, 005h, 064h, 039h, 001h, 000h, 001h
        sti
        ret
sub_1_E729:
        cli
loc_1_E72A:
        mov    ecx, dword ptr [esi]
        cmp    ecx, esi
        je     SHORT loc_1_E74E
        mov    eax, dword ptr [ecx]
        cmp    eax, esi
        jne    SHORT loc_1_E739
        mov    dword ptr [esi + 4], eax
loc_1_E739:
        mov    dword ptr [esi], eax
        sti
        mov    dword ptr [ecx + 4], 0
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ecx + 0ch]
        cld
        call   eax
        jmp    SHORT loc_1_E72A
loc_1_E74E:
        ret
loc_1_E74F:
        cli
        cmp    dword ptr [edx], 0
        jne    SHORT loc_1_E771
        db  0B9h, 068h, 039h, 001h, 000h
        db  068h, 083h, 0E6h, 000h, 000h
loc_1_E75F:
        mov    dword ptr [edx + 4], 0ffffffffh
        mov    edi, edx
        mov    dword ptr [edx], ecx
        xchg   dword ptr [ecx + 4], edi
        mov    dword ptr [edi], edx
        sti
        ret
loc_1_E771:
        db  0B8h, 078h, 039h, 001h, 000h
        db  068h, 0C0h, 08Bh, 000h, 000h
        db  0B9h, 070h, 039h, 001h, 000h
        jmp    SHORT loc_1_E75F
sub_1_E782:
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [esp + 8]
        push   esi
        db  0BEh, 04Fh, 0E7h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    dword ptr [edx + 4], esi
        mov    eax, esi
        pop    esi
        cld
        ret
sub_1_E7AB:
        mov    edx, dword ptr [esp + 4]
        push   esi
        xor    esi, esi
        cli
        xchg   dword ptr [edx + 4], esi
        cmp    esi, -1
        je     SHORT loc_1_E7C5
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
loc_1_E7C1:
        cld
        sti
        pop    esi
        ret
loc_1_E7C5:
        db  0B8h, 068h, 039h, 001h, 000h
        mov    esi, eax
loc_1_E7CC:
        cmp    dword ptr [eax], edx
        je     SHORT loc_1_E7E7
        mov    eax, dword ptr [eax]
        cmp    dword ptr [eax], esi
        jne    SHORT loc_1_E7CC
        db  081h, 0FEh, 068h, 039h, 001h, 000h
        jne    SHORT loc_1_E7C1
        db  0B8h, 070h, 039h, 001h, 000h
        mov    esi, eax
        jmp    SHORT loc_1_E7CC
loc_1_E7E7:
        cmp    edx, dword ptr [esi + 4]
        mov    edx, dword ptr [edx]
        mov    dword ptr [eax], edx
        jne    SHORT loc_1_E7C1
        mov    dword ptr [esi + 4], eax
        jmp    SHORT loc_1_E7C1
sub_1_E7F5:
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        ret
sub_1_E7FC:
        push   ebx
        db  08Bh, 01Dh, 028h, 028h, 001h, 000h
        db  08Bh, 00Dh, 07Ch, 039h, 001h, 000h
        db  089h, 01Dh, 07Ch, 039h, 001h, 000h
        sub    ebx, ecx
        db  0A1h, 0C4h, 031h, 001h, 000h
        db  08Bh, 00Dh, 080h, 039h, 001h, 000h
        db  0A3h, 080h, 039h, 001h, 000h
        sub    eax, ecx
        mov    ecx, 3e8h
        xor    edx, edx
        add    eax, 1f4h
        div    ecx
        sub    eax, ebx
        cdq
        sub    eax, edx
        and    al, 0feh
        pop    ebx
        ret
sub_1_E83A:
        db  0CDh, 020h, 019h, 080h, 040h, 000h
sub_1_E840:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        sub    esp, 28h
        mov    esi, esp
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        mov    ecx, 5fh
        mov    dword ptr [esi], 5f88h
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [esi + 20h], eax
        mov    byte ptr [esi + 24h], 0
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        db  0CDh, 020h, 00Ah, 000h, 040h, 000h
        jb     SHORT loc_1_E8AE
        sub    eax, eax
        test   word ptr [ebp + 2ch], 1
        je     SHORT loc_1_E897
        movzx  eax, word ptr [ebp + 1ch]
loc_1_E897:
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        add    esp, 28h
        pop    ebx
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_1_E8AE:
        int3
        mov    eax, 1
        jmp    SHORT loc_1_E897
sub_1_E8B6:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    al, 0
        mov    edx, dword ptr [ebp + 8]
        movzx  edx, byte ptr [edx]
        call   sub_1_7738
        cmc
        sbb    eax, eax
        and    eax, ecx
        pop    ebx
        pop    ebp
        ret
sub_1_E8CF:
        db  0CDh, 020h, 002h, 080h, 081h, 004h
        db  0CDh, 020h, 007h, 080h, 081h, 004h
sub_1_E8DB:
        db  0CDh, 020h, 00Eh, 080h, 081h, 004h
        db  0CDh, 020h, 008h, 080h, 081h, 004h
sub_1_E8E7:
        db  0CDh, 020h, 003h, 080h, 081h, 004h
sub_1_E8ED:
        db  0CDh, 020h, 004h, 080h, 081h, 004h
sub_1_E8F3:
        db  0CDh, 020h, 006h, 080h, 081h, 004h
sub_1_E8F9:
        db  0CDh, 020h, 01Bh, 080h, 040h, 000h
sub_1_E8FF:
        db  0CDh, 020h, 00Dh, 080h, 081h, 004h
sub_1_E905:
        push   ebp
        mov    ebp, esp
        push   1
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 040h, 000h, 040h, 000h
        add    esp, 14h
        leave
        ret
        db  000h, 000h, 000h
sub_1_E924:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        and    word ptr [esi + 0cch], 0ff7fh
        mov    eax, dword ptr [esi + 18h]
        test   eax, 40000h
        jne    NEAR PTR loc_1_E9D8
        or     eax, 40000h
        mov    dword ptr [esi + 18h], eax
        mov    al, byte ptr [esi]
        push   eax
        push   40000h
        push   40000h
        call   sub_1_E8CF
        add    esp, 0ch
        push   1
        mov    al, byte ptr [esi]
        push   eax
        call   sub_1_E8F3
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_E97C
        mov    ecx, esi
        call   sub_1_EAE4
        pop    esi
        leave
        ret
        db  08Bh, 0FFh
loc_1_E97C:
        db  0C7h, 086h, 080h, 000h, 000h, 000h, 080h, 0ECh, 000h, 000h
        db  0C7h, 086h, 084h, 000h, 000h, 000h, 014h, 0F0h, 000h, 000h
        mov    dword ptr [esi + 24h], 0
        add    esi, 9ch
        push   esi
        call   sub_1_E7AB
        add    esp, 4
        db  083h, 03Dh, 084h, 039h, 001h, 000h, 000h
        jne    SHORT loc_1_E9C6
        db  068h, 084h, 039h, 001h, 000h
        db  068h, 088h, 039h, 001h, 000h
        db  068h, 010h, 0EFh, 000h, 000h
        call   sub_1_E83A
        add    esp, 0ch
loc_1_E9C6:
        push   1
        db  068h, 09Ch, 011h, 001h, 000h
        push   5
        push   esi
        call   sub_1_E782
        add    esp, 10h
loc_1_E9D8:
        pop    esi
        leave
        ret
        db  090h
loc_1_E9DC:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    cl, byte ptr [ebp + 8]
        cmp    cl, 11h
        jae    NEAR PTR loc_1_EADF
        movzx  eax, cl
        db  08Bh, 034h, 085h, 0B0h, 01Bh, 001h, 000h
        cmp    esi, 0fffff000h
        je     NEAR PTR loc_1_EADF
        db  0F6h, 080h, 0B8h, 01Ch, 001h, 000h, 040h
        jne    NEAR PTR loc_1_EADF
        mov    eax, dword ptr [ebp + 0ch]
        or     eax, eax
        je     SHORT loc_1_EA24
        cmp    eax, 1
        je     SHORT loc_1_EA60
        cmp    eax, 2
        je     SHORT loc_1_EA90
        pop    esi
        leave
        ret
        db  090h
loc_1_EA24:
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_1_EA38
        push   ecx
        call   sub_1_F6F8
        add    esp, 4
        pop    esi
        leave
        ret
        db  08Bh, 0FFh
loc_1_EA38:
        mov    ax, word ptr [esi + 0cch]
        or     ax, 8000h
        mov    word ptr [esi + 0cch], ax
        test   ah, 40h
        je     NEAR PTR loc_1_EADF
        push   esi
        call   sub_1_FCDC
        add    esp, 4
        pop    esi
        leave
        ret
        db  090h
loc_1_EA60:
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_1_EA70
        mov    ecx, esi
        call   sub_1_EAE4
        pop    esi
        leave
        ret
loc_1_EA70:
        cmp    dword ptr [esi + 44h], 0
        je     SHORT loc_1_EA7C
        cmp    dword ptr [esi + 64h], 0
        jne    SHORT loc_1_EADF
loc_1_EA7C:
        mov    ecx, esi
        call   sub_1_103DC
        mov    ecx, esi
        call   sub_1_103DC
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_EA90:
        lea    eax, [esi + 9ch]
        push   eax
        call   sub_1_E7AB
        add    esp, 4
        push   0ah
        mov    al, byte ptr [esi]
        push   eax
        call   sub_1_E8ED
        add    esp, 8
        mov    byte ptr [esi + 88h], 4
        call   sub_1_E7F5
        mov    dword ptr [esi + 20h], eax
        add    esi, 8ch
        push   esi
        call   sub_1_E7AB
        add    esp, 4
        push   0
        db  068h, 0E8h, 000h, 001h, 000h
        push   0afc8h
        push   esi
        call   sub_1_E782
        add    esp, 10h
loc_1_EADF:
        pop    esi
        leave
        ret
        db  08Bh, 0FFh
sub_1_EAE4:
        push   esi
        push   edi
        mov    esi, ecx
        test   byte ptr [esi + 1ah], 4
        je     SHORT loc_1_EB66
        and    word ptr [esi + 0cch], 0ff7fh
        lea    eax, [esi + 9ch]
        push   eax
        call   sub_1_E7AB
        add    esp, 4
        cmp    dword ptr [esi + 0c0h], 0
        je     SHORT loc_1_EB1E
        lea    eax, [esi + 0bch]
        push   eax
        call   sub_1_E7AB
        add    esp, 4
loc_1_EB1E:
        sub    eax, eax
        lea    edi, [esi + 0e0h]
        stosd
        stosd
        stosd
        stosd
        stosw
        db  0C7h, 086h, 080h, 000h, 000h, 000h, 06Ch, 0EBh, 000h, 000h
        db  0C7h, 086h, 084h, 000h, 000h, 000h, 010h, 0F0h, 000h, 000h
        and    dword ptr [esi + 18h], 0fffbffffh
        mov    al, byte ptr [esi]
        push   eax
        push   0
        push   40000h
        call   sub_1_E8CF
        add    esp, 0ch
        push   0
        mov    al, byte ptr [esi]
        push   eax
        call   sub_1_E8F3
        add    esp, 8
loc_1_EB66:
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_EB6C:
        mov    eax, 1
        ret
        db  08Bh, 0FFh
sub_1_EB74:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 22h]
        and    eax, 40000000h
        mov    dword ptr [ebp - 8], eax
        test   byte ptr [ebx + 9], 8
        je     SHORT loc_1_EBA2
        mov    eax, dword ptr [ebp + 0ch]
        and    dword ptr [eax + 12h], 0fffcffffh
loc_1_EBA2:
        mov    esi, dword ptr [ebp + 14h]
        or     esi, esi
        je     NEAR PTR loc_1_EC68
        call   sub_1_F0D0
        lea    eax, [esi + esi*2]
        db  003h, 005h, 028h, 028h, 001h, 000h
        mov    dword ptr [ebx + 0ch], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 16h]
        and    eax, 0ffff0000h
        cmp    eax, 0aa550000h
        je     SHORT loc_1_EBDA
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 16h], 0
loc_1_EBDA:
        mov    esi, dword ptr [ebp + 0ch]
        lea    esi, [esi + 10h]
        lea    edi, [ebx + 20h]
        mov    ecx, 2
        repe cmpsd
        jne    SHORT loc_1_EBF2
        mov    ax, word ptr [esi]
        cmp    ax, word ptr [edi]
loc_1_EBF2:
        je     SHORT loc_1_EC10
        lea    edi, [ebx + 10h]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, 6
        rep movsd
        movsw
        mov    eax, dword ptr [ebp - 8]
        or     dword ptr [ebx + 22h], eax
        mov    dword ptr [ebp - 4], 1
loc_1_EC10:
        sub    eax, eax
        mov    edi, dword ptr [ebp + 10h]
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        mov    al, cl
        movsx  edx, al
        mov    ecx, edx
        shr    ecx, 2
        lea    esi, [ebx + 2ah]
        mov    edi, dword ptr [ebp + 10h]
        repe cmpsd
        jne    SHORT loc_1_EC39
        mov    ecx, edx
        and    ecx, 3
        repe cmpsb
loc_1_EC39:
        je     SHORT loc_1_EC5A
        lea    edi, [ebx + 2ah]
        mov    esi, dword ptr [ebp + 10h]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    byte ptr [edx + ebx + 2ah], 0
        mov    byte ptr [ebx + 5bh], al
        inc    dword ptr [ebp - 4]
loc_1_EC5A:
        pop    edi
        and    byte ptr [ebx + 9], 0feh
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_EC68:
        push   ebx
        call   sub_1_F4B8
        add    esp, 4
        mov    dword ptr [ebp - 4], 1
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_EC80:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ecx, 3e8h
        mov    eax, dword ptr [ebp + 14h]
        sub    edx, edx
        div    ecx
        mov    dword ptr [ebp - 4], eax
        mov    edi, dword ptr [ebp + 10h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        cmp    ecx, 30h
        jbe    SHORT loc_1_ECB3
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax + 30h], 0
loc_1_ECB3:
        mov    esi, dword ptr [ebp + 8]
        add    esi, 3ch
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp + 0ch]
        push   esi
        call   sub_1_11288
        add    esp, 0ch
        or     al, al
        je     SHORT loc_1_ECE8
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp - 8]
        call   sub_1_EB74
        add    esp, 10h
        jmp    NEAR PTR loc_1_EDBF
        db  08Bh, 0FFh
loc_1_ECE8:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_ECF8
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_ECF8:
        push   5ch
        call   sub_1_E640
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_ED34
        push   esi
        call   sub_1_1134C
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_ED24
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_ED24:
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp + 0ch]
        push   esi
        call   sub_1_11288
        add    esp, 0ch
loc_1_ED34:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebx + 0bh], al
        mov    byte ptr [ebx + 9], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 0fh], 0
        mov    byte ptr [ebx + 5bh], 1
        mov    edi, eax
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    byte ptr [ebx + 0ah], cl
        lea    edi, [ebx + 10h]
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        movsx  edx, byte ptr [ebx + 0ah]
        lea    edi, [ebx + 10h]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    byte ptr [ebx + 8], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 12h]
        mov    dword ptr [ebx + 22h], eax
        push   dword ptr [ebp - 8]
        push   ebx
        call   sub_1_112CC
        add    esp, 8
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   ebx
        call   sub_1_EB74
        add    esp, 10h
        lea    eax, [ebx + 10h]
        push   eax
        call   sub_1_F09C
        add    esp, 4
        or     al, al
        je     SHORT loc_1_EDBA
        or     byte ptr [ebx + 9], 2
loc_1_EDBA:
        mov    eax, 1
loc_1_EDBF:
        or     eax, eax
        je     SHORT loc_1_EDD1
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        call   sub_1_E8E7
        add    esp, 4
loc_1_EDD1:
        pop    edi
        mov    eax, 1
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
sub_1_EDDC:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ecx, 3e8h
        mov    edi, dword ptr [ebp + 14h]
        mov    eax, edi
        sub    edx, edx
        div    ecx
        mov    dword ptr [ebp - 8], eax
        mov    esi, dword ptr [ebp + 8]
        add    esi, 5ch
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp + 0ch]
        push   esi
        call   sub_1_11288
        add    esp, 0ch
        or     al, al
        je     SHORT loc_1_EE40
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp - 4]
        call   sub_1_EB74
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_EE36
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        call   sub_1_E8E7
        add    esp, 4
loc_1_EE36:
        pop    edi
        mov    eax, 1
        pop    esi
        pop    ebx
        leave
        ret
loc_1_EE40:
        or     edi, edi
        jne    SHORT loc_1_EE50
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_EE50:
        push   5ch
        call   sub_1_E640
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_EE8C
        push   esi
        call   sub_1_1134C
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_EE7C
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_EE7C:
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp + 0ch]
        push   esi
        call   sub_1_11288
        add    esp, 0ch
loc_1_EE8C:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebx + 0bh], al
        mov    byte ptr [ebx + 8], 1
        mov    byte ptr [ebx + 9], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 0fh], 0
        mov    byte ptr [ebx + 5bh], 1
        mov    edi, eax
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    byte ptr [ebx + 0ah], cl
        movsx  eax, cl
        lea    edi, [ebx + 10h]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 12h]
        mov    dword ptr [ebx + 22h], eax
        push   dword ptr [ebp - 4]
        push   ebx
        call   sub_1_112CC
        add    esp, 8
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   ebx
        call   sub_1_EB74
        add    esp, 10h
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        push   eax
        call   sub_1_E8E7
        add    esp, 4
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_EF10:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        db  0A1h, 020h, 01Ch, 001h, 000h
        cmp    byte ptr [eax], 2
        jne    SHORT loc_1_EF2C
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_EF2C:
        mov    al, byte ptr [ebp + 10h]
        cmp    al, 8
        jae    NEAR PTR loc_1_F007
        mov    edx, dword ptr [ebp + 8]
        cmp    word ptr [edx], 1
        jne    NEAR PTR loc_1_F007
        test   dword ptr [edx + 2], 403000h
        jne    NEAR PTR loc_1_F007
        and    eax, 0ffh
        db  08Bh, 01Ch, 085h, 0B0h, 01Bh, 001h, 000h
        cmp    ebx, 0fffff000h
        je     NEAR PTR loc_1_F007
        test   byte ptr [ebx + 0cdh], 2
        je     SHORT loc_1_EF7C
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_EF7C:
        sub    eax, eax
        lea    edi, [ebp - 1ch]
        mov    ecx, 6
        rep stosd
        stosw
        mov    al, byte ptr [edx + 6]
        mov    byte ptr [ebp - 0ch], al
        mov    al, byte ptr [edx + 7]
        mov    byte ptr [ebp - 0bh], al
        lea    esi, [edx + 0ah]
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [ecx - 1]
        cmp    eax, 0fh
        jbe    SHORT loc_1_EFB8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_EFB8:
        mov    ecx, eax
        lea    edi, [ebp - 1ch]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [edx + 2]
        and    ecx, 7ff8ffffh
        mov    dword ptr [ebp - 0ah], ecx
        test   ecx, 0fff8fffeh
        mov    ecx, 0
        je     SHORT loc_1_EFF2
        movzx  ecx, word ptr [edx + 8]
        lea    ecx, [ecx + ecx*4]
        lea    ecx, [ecx + ecx*4]
        lea    ecx, [ecx + ecx*4]
        shl    ecx, 3
loc_1_EFF2:
        push   ecx
        add    eax, edx
        add    eax, 0bh
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        push   ebx
        call   dword ptr [ebx + 80h]
        add    esp, 10h
loc_1_F007:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_F010:
        ret
        db  08Dh, 049h, 000h
loc_1_F014:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 14h]
        cmp    ecx, 7
        jbe    SHORT loc_1_F07C
        mov    esi, dword ptr [ebp + 0ch]
        add    esi, 6
        lea    ebx, [ecx + esi - 7]
        cmp    ebx, esi
        jb     SHORT loc_1_F07C
        mov    eax, dword ptr [ebp + 8]
        add    eax, 3ch
        mov    dword ptr [ebp - 8], eax
loc_1_F03C:
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        lea    eax, [ebp - 4]
        push   eax
        push   esi
        push   dword ptr [ebp - 8]
        call   sub_1_11288
        add    esp, 0ch
        or     al, al
        je     SHORT loc_1_F069
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 8]
        call   sub_1_F45C
        add    esp, 8
loc_1_F069:
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    esi, ecx
        cmp    ebx, esi
        jae    SHORT loc_1_F03C
loc_1_F07C:
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 0ch]
        push   dword ptr [eax + 2]
        mov    al, byte ptr [eax + 1]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_101E0
        add    esp, 14h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_F09C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        db  0BFh, 092h, 024h, 001h, 000h
        db  0A1h, 0A2h, 024h, 001h, 000h
        mov    ecx, eax
        shr    ecx, 2
        mov    esi, dword ptr [ebp + 8]
        repe cmpsd
        jne    SHORT loc_1_F0C8
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb
        jne    SHORT loc_1_F0C8
        mov    al, 1
        pop    edi
        pop    esi
        leave
        ret
        db  08Bh, 0FFh
loc_1_F0C8:
        sub    al, al
        pop    edi
        pop    esi
        leave
        ret
        db  08Bh, 0FFh
sub_1_F0D0:
        push   esi
        call   sub_1_E7FC
        or     eax, eax
        je     SHORT loc_1_F135
        db  08Bh, 015h, 0A6h, 024h, 001h, 000h
        or     edx, edx
        je     SHORT loc_1_F135
loc_1_F0E4:
        mov    cx, word ptr [edx + 0cch]
        and    ch, 0c0h
        cmp    ch, 0c0h
        jne    SHORT loc_1_F12E
        sub    dword ptr [edx + 24h], eax
        sub    dword ptr [edx + 30h], eax
        mov    esi, dword ptr [edx + 3ch]
        mov    ecx, edx
        sub    ecx, esi
        cmp    ecx, -3ch
        je     SHORT loc_1_F113
loc_1_F105:
        mov    ecx, edx
        sub    dword ptr [esi + 0ch], eax
        mov    esi, dword ptr [esi]
        sub    ecx, esi
        cmp    ecx, -3ch
        jne    SHORT loc_1_F105
loc_1_F113:
        mov    ecx, edx
        mov    esi, dword ptr [edx + 5ch]
        sub    ecx, esi
        cmp    ecx, -5ch
        je     SHORT loc_1_F12E
        nop
loc_1_F120:
        mov    ecx, edx
        sub    dword ptr [esi + 0ch], eax
        mov    esi, dword ptr [esi]
        sub    ecx, esi
        cmp    ecx, -5ch
        jne    SHORT loc_1_F120
loc_1_F12E:
        mov    edx, dword ptr [edx + 0ch]
        or     edx, edx
        jne    SHORT loc_1_F0E4
loc_1_F135:
        pop    esi
        ret
        db  090h
loc_1_F138:
        push   ebp
        mov    ebp, esp
        sub    esp, 0cch
        push   ebx
        push   esi
        push   edi
        mov    ebx, ecx
        db  08Bh, 035h, 028h, 028h, 001h, 000h
        mov    dword ptr [ebp - 18h], 0
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 1ch], 15f90h
        test   byte ptr [ebx + 0cdh], 2
        jne    NEAR PTR loc_1_F455
        test   byte ptr [ebx + 1ah], 4
        je     NEAR PTR loc_1_F455
        call   sub_1_F0D0
        mov    dword ptr [ebp - 10h], 0
        mov    dword ptr [ebp - 8], 0
        cmp    dword ptr [ebx + 24h], esi
        jae    NEAR PTR loc_1_F268
        mov    ecx, dword ptr [ebx + 5ch]
        mov    eax, ebx
        sub    eax, ecx
        cmp    eax, -5ch
        je     SHORT loc_1_F1C4
loc_1_F1A0:
        mov    edi, dword ptr [ecx]
        test   byte ptr [ecx + 25h], 80h
        je     SHORT loc_1_F1AD
        cmp    dword ptr [ecx + 0ch], esi
        jae    SHORT loc_1_F1B9
loc_1_F1AD:
        push   ecx
        call   sub_1_F4B8
        add    esp, 4
        inc    dword ptr [ebp - 0ch]
loc_1_F1B9:
        mov    ecx, edi
        mov    eax, ebx
        sub    eax, edi
        cmp    eax, -5ch
        jne    SHORT loc_1_F1A0
loc_1_F1C4:
        mov    eax, dword ptr [ebx + 48h]
        or     eax, eax
        je     NEAR PTR loc_1_F262
        push   eax
        call   sub_1_1018C
        add    esp, 4
        mov    dword ptr [ebp - 14h], eax
        mov    ecx, dword ptr [ebx + 3ch]
        mov    eax, ebx
        sub    eax, ecx
        cmp    eax, -3ch
        je     SHORT loc_1_F262
        nop
loc_1_F1E8:
        mov    edi, dword ptr [ecx]
        mov    eax, dword ptr [ecx + 22h]
        test   eax, 0fff8fffeh
        je     SHORT loc_1_F248
        cmp    dword ptr [ecx + 0ch], esi
        jb     SHORT loc_1_F248
        test   byte ptr [ecx + 9], 2
        jne    SHORT loc_1_F254
        mov    edx, eax
        and    edx, 40020000h
        cmp    edx, 40020000h
        jne    SHORT loc_1_F214
        inc    dword ptr [ebp - 18h]
        jmp    SHORT loc_1_F254
loc_1_F214:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_F254
        test   byte ptr [ecx + 9], 1
        jne    SHORT loc_1_F254
        and    eax, 40030000h
        cmp    eax, 40010000h
        jne    SHORT loc_1_F254
        cmp    dword ptr [ebp - 14h], 0
        jle    SHORT loc_1_F240
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_F254
        mov    dword ptr [ebp - 10h], ecx
        jmp    SHORT loc_1_F254
        db  08Dh, 049h, 000h
loc_1_F240:
        mov    dword ptr [ebp - 8], ecx
        jmp    SHORT loc_1_F254
        db  08Dh, 049h, 000h
loc_1_F248:
        push   ecx
        call   sub_1_F4B8
        add    esp, 4
        inc    dword ptr [ebp - 0ch]
loc_1_F254:
        mov    ecx, edi
        dec    dword ptr [ebp - 14h]
        mov    eax, ebx
        sub    eax, edi
        cmp    eax, -3ch
        jne    SHORT loc_1_F1E8
loc_1_F262:
        lea    eax, [esi + 59h]
        mov    dword ptr [ebx + 24h], eax
loc_1_F268:
        mov    eax, dword ptr [ebx + 48h]
        cmp    eax, 4
        jb     SHORT loc_1_F2AD
        shr    eax, 5
        inc    eax
        cmp    eax, dword ptr [ebp - 18h]
        jbe    SHORT loc_1_F2AD
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_F28B
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_F2AD
loc_1_F28B:
        mov    eax, dword ptr [ebp - 8]
        add    eax, 10h
        push   eax
        push   ebx
        call   sub_1_1034C
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_F2A6
        mov    eax, dword ptr [ebp - 8]
        or     byte ptr [eax + 9], 1
loc_1_F2A6:
        mov    dword ptr [ebp - 1ch], 1f4h
loc_1_F2AD:
        cmp    dword ptr [ebx + 30h], esi
        jae    SHORT loc_1_F2D6
        mov    ecx, dword ptr [ebx + 3ch]
        mov    eax, ebx
        sub    eax, ecx
        cmp    eax, -3ch
        je     SHORT loc_1_F2CD
loc_1_F2BE:
        mov    eax, ebx
        and    byte ptr [ecx + 9], 0f7h
        mov    ecx, dword ptr [ecx]
        sub    eax, ecx
        cmp    eax, -3ch
        jne    SHORT loc_1_F2BE
loc_1_F2CD:
        add    esi, 1b7740h
        mov    dword ptr [ebx + 30h], esi
loc_1_F2D6:
        cmp    dword ptr [ebp - 1ch], 1f4h
        je     NEAR PTR loc_1_F413
        cmp    dword ptr [ebp - 18h], 0
        jle    NEAR PTR loc_1_F413
        mov    eax, dword ptr [ebx + 38h]
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 18h], 1
        jle    SHORT loc_1_F2FE
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
loc_1_F2FE:
        mov    eax, ebx
        sub    eax, dword ptr [ebp - 8]
        cmp    eax, -3ch
        jne    SHORT loc_1_F30E
        mov    eax, dword ptr [ebx + 3ch]
        mov    dword ptr [ebp - 8], eax
loc_1_F30E:
        mov    eax, ebx
        sub    eax, dword ptr [ebp - 8]
        cmp    eax, -3ch
        je     SHORT loc_1_F33C
loc_1_F318:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 22h]
        and    eax, 40060000h
        cmp    eax, 40020000h
        je     SHORT loc_1_F33C
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        mov    eax, ebx
        sub    eax, dword ptr [ebp - 8]
        cmp    eax, -3ch
        jne    SHORT loc_1_F318
loc_1_F33C:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebx + 38h], eax
        mov    eax, ebx
        sub    eax, dword ptr [ebp - 8]
        cmp    eax, -3ch
        je     NEAR PTR loc_1_F413
        mov    byte ptr [ebp - 30h], 5ch
        mov    byte ptr [ebp - 2fh], 5ch
        lea    edi, [ebp - 2eh]
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        mov    esi, dword ptr [ebp - 8]
        add    esi, 10h
        lea    edi, [ebp - 2eh]
        movsd
        movsd
        movsd
        movsd
        db  0C7h, 085h, 034h, 0FFh, 0FFh, 0FFh, 0F0h, 039h, 001h, 000h
        mov    dword ptr [ebp - 0c8h], 0ffffffffh
        mov    word ptr [ebp - 0b6h], 1
        lea    eax, [ebp - 30h]
        mov    dword ptr [ebp - 0b4h], eax
        lea    eax, [ebp - 2]
        mov    dword ptr [ebp - 0c4h], eax
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 0c0h], eax
        mov    word ptr [ebp - 0bch], 0
        mov    dword ptr [ebp - 0bah], 0
        mov    al, byte ptr [ebx]
        push   eax
        lea    eax, [ebp - 0cch]
        push   eax
        call   sub_1_E840
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_F3E4
        cmp    eax, 0eah
        je     SHORT loc_1_F3E4
        push   dword ptr [ebp - 8]
        push   ebx
        call   sub_1_F45C
        add    esp, 8
        jmp    SHORT loc_1_F413
loc_1_F3E4:
        mov    ecx, dword ptr [ebx + 44h]
        cmp    ecx, 40h
        jbe    SHORT loc_1_F40C
        movzx  eax, word ptr [ebp - 2]
        lea    edx, [ecx + 20h]
        cmp    edx, eax
        jb     SHORT loc_1_F3FE
        sub    ecx, 20h
        cmp    ecx, eax
        jbe    SHORT loc_1_F40C
loc_1_F3FE:
        push   dword ptr [ebp - 8]
        push   ebx
        call   sub_1_F45C
        add    esp, 8
        jmp    SHORT loc_1_F413
loc_1_F40C:
        mov    eax, dword ptr [ebp - 8]
        and    byte ptr [eax + 9], 0f3h
loc_1_F413:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_1_F424
        mov    al, byte ptr [ebx]
        push   eax
        call   sub_1_E8E7
        add    esp, 4
loc_1_F424:
        test   byte ptr [ebx + 0cdh], 2
        jne    SHORT loc_1_F455
        test   byte ptr [ebx + 1ah], 4
        je     SHORT loc_1_F455
        cmp    dword ptr [ebx + 0a0h], 0
        jne    SHORT loc_1_F455
        push   1
        db  068h, 038h, 0F1h, 000h, 000h
        push   dword ptr [ebp - 1ch]
        add    ebx, 9ch
        push   ebx
        call   sub_1_E782
        add    esp, 10h
loc_1_F455:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
sub_1_F45C:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 22h]
        mov    edx, ecx
        and    edx, 40020000h
        cmp    edx, 40020000h
        jne    SHORT loc_1_F4B3
        mov    dl, byte ptr [eax + 9]
        test   dl, 8
        jne    SHORT loc_1_F4B3
        test   dl, 4
        jne    SHORT loc_1_F48C
        or     dl, 4
        mov    byte ptr [eax + 9], dl
        pop    esi
        leave
        ret
loc_1_F48C:
        push   eax
        and    ecx, 0fffcffffh
        mov    dword ptr [eax + 22h], ecx
        or     dl, 8
        mov    byte ptr [eax + 9], dl
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_10588
        add    esp, 8
        mov    al, byte ptr [esi]
        push   eax
        call   sub_1_E8E7
        add    esp, 4
loc_1_F4B3:
        pop    esi
        leave
        ret
        db  08Bh, 0FFh
sub_1_F4B8:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_11308
        add    esp, 4
        push   esi
        call   sub_1_E651
        add    esp, 4
        pop    esi
        leave
        ret
sub_1_F4D4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0a8h
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 4], 0
        mov    edi, 10h
loc_1_F4F2:
        dec    edi
        je     NEAR PTR loc_1_F61B
        lea    eax, [ebp - 8]
        push   eax
        push   edi
        call   sub_1_E5FB
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_F4F2
        shl    di, 0ch
        mov    word ptr [ebp - 98h], di
        mov    dword ptr [ebp - 96h], esi
        db  0A1h, 020h, 031h, 001h, 000h
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 0a8h], eax
        mov    dword ptr [ebp - 0a4h], 0ffffffffh
        mov    word ptr [ebp - 92h], 1
        mov    edi, dword ptr [ebp + 8]
        lea    eax, [edi + 0e0h]
        mov    dword ptr [ebp - 90h], eax
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 0a0h], eax
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 9ch], eax
        mov    al, byte ptr [edi]
        push   eax
        lea    eax, [ebp - 0a8h]
        push   eax
        call   sub_1_E840
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_F57C
        cmp    eax, 0eah
        jne    SHORT loc_1_F5AC
loc_1_F57C:
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [ebp - 4]
        or     eax, eax
        je     SHORT loc_1_F5AC
loc_1_F586:
        push   0dbba0h
        and    dword ptr [esi + 12h], 0bfffffffh
        push   dword ptr [esi + 16h]
        push   esi
        push   edi
        call   sub_1_EC80
        add    esp, 10h
        add    esi, 1ah
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [ebp - 4]
        or     eax, eax
        jne    SHORT loc_1_F586
loc_1_F5AC:
        mov    dword ptr [ebp - 0a4h], 80000000h
        mov    dword ptr [ebp - 4], 0
        mov    al, byte ptr [edi]
        push   eax
        lea    eax, [ebp - 0a8h]
        push   eax
        call   sub_1_E840
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_F5DA
        cmp    eax, 0eah
        jne    SHORT loc_1_F610
loc_1_F5DA:
        mov    esi, dword ptr [ebp - 96h]
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [ebp - 4]
        or     eax, eax
        je     SHORT loc_1_F610
loc_1_F5EA:
        push   0dbba0h
        and    dword ptr [esi + 12h], 0bfffffffh
        push   dword ptr [esi + 16h]
        push   esi
        push   edi
        call   sub_1_EDDC
        add    esp, 10h
        add    esi, 1ah
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [ebp - 4]
        or     eax, eax
        jne    SHORT loc_1_F5EA
loc_1_F610:
        push   dword ptr [ebp - 8]
        call   sub_1_E62B
        add    esp, 4
loc_1_F61B:
        pop    edi
        pop    esi
        leave
        ret
        db  090h
loc_1_F620:
        push   esi
        mov    esi, ecx
        cmp    byte ptr [esi + 0e2h], 0
        je     SHORT loc_1_F662
        push   esi
        call   sub_1_F4D4
        add    esp, 4
        push   esi
        call   sub_1_10480
        add    esp, 4
        cmp    dword ptr [esi + 0c0h], 0
        jne    SHORT loc_1_F662
        push   1
        db  068h, 020h, 0F6h, 000h, 000h
        push   0afc80h
        add    esi, 0bch
        push   esi
        call   sub_1_E782
        add    esp, 10h
loc_1_F662:
        pop    esi
        ret
sub_1_F664:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        and    word ptr [eax + 0cch], 0fffch
        mov    al, byte ptr [eax]
        push   eax
        call   sub_1_F6F8
        leave
        ret
        db  08Dh, 049h, 000h
sub_1_F680:
        push   esi
        db  08Bh, 035h, 0A6h, 024h, 001h, 000h
        mov    eax, esi
        db  080h, 03Dh, 09Ch, 039h, 001h, 000h, 000h
        jne    SHORT loc_1_F6C3
        or     esi, esi
        je     SHORT loc_1_F6A6
loc_1_F696:
        and    word ptr [eax + 0cch], 0fffch
        mov    eax, dword ptr [eax + 0ch]
        or     eax, eax
        jne    SHORT loc_1_F696
loc_1_F6A6:
        or     esi, esi
        db  0C6h, 005h, 09Ch, 039h, 001h, 000h, 001h
        je     SHORT loc_1_F6C3
loc_1_F6B1:
        mov    al, byte ptr [esi]
        push   eax
        call   sub_1_F6F8
        add    esp, 4
        mov    esi, dword ptr [esi + 0ch]
        or     esi, esi
        jne    SHORT loc_1_F6B1
loc_1_F6C3:
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_1_F6C8:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        lea    esi, [eax + 3ch]
        cmp    dword ptr [esi + 8], 0
        jbe    SHORT loc_1_F6E1
        push   eax
        call   sub_1_1138C
        add    esp, 4
loc_1_F6E1:
        cmp    dword ptr [esi + 10h], 0
        je     SHORT loc_1_F6F2
        push   dword ptr [esi + 1ch]
        call   sub_1_E62B
        add    esp, 4
loc_1_F6F2:
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
sub_1_F6F8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    bl, byte ptr [ebp + 8]
        movzx  edi, bl
        db  08Bh, 034h, 0BDh, 0B0h, 01Bh, 001h, 000h
        mov    ax, word ptr [esi + 0cch]
        test   ah, 2
        jne    NEAR PTR loc_1_F79C
        or     ax, 200h
        mov    word ptr [esi + 0cch], ax
        lea    eax, [esi + 8ch]
        push   eax
        call   sub_1_E7AB
        add    esp, 4
        lea    eax, [esi + 9ch]
        push   eax
        call   sub_1_E7AB
        add    esp, 4
        lea    eax, [esi + 0ach]
        push   eax
        call   sub_1_E7AB
        add    esp, 4
        db  0C7h, 046h, 07Ch, 074h, 0FDh, 000h, 000h
        db  0F6h, 087h, 0B8h, 01Ch, 001h, 000h, 040h
        jne    SHORT loc_1_F788
        push   esi
        call   sub_1_10F38
        add    esp, 4
        mov    ecx, esi
        call   sub_1_EAE4
        push   0ah
        push   ebx
        call   sub_1_E8ED
        add    esp, 8
        call   sub_1_E70C
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_F788:
        test   byte ptr [esi + 1ah], 6
        je     SHORT loc_1_F797
        push   esi
        call   sub_1_F6C8
        add    esp, 4
loc_1_F797:
        call   sub_1_E70C
loc_1_F79C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_F7A4:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 14h]
        or     esi, esi
        jne    SHORT loc_1_F7C0
        mov    dword ptr [ebp - 8], 0
        jmp    SHORT loc_1_F7D8
        db  08Dh, 049h, 000h
loc_1_F7C0:
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 8], eax
        push   0fh
        push   1eh
        push   esi
        push   eax
        call   sub_1_E905
        add    esp, 10h
        mov    byte ptr [ebp - 0dh], 0
loc_1_F7D8:
        mov    al, byte ptr [ebp + 10h]
        cmp    al, 11h
        jae    NEAR PTR loc_1_FC14
        movzx  edx, al
        db  08Bh, 01Ch, 095h, 0B0h, 01Bh, 001h, 000h
        cmp    ebx, 0fffff000h
        je     NEAR PTR loc_1_FC14
        mov    cx, word ptr [ebx + 0cch]
        and    ch, 0c2h
        cmp    ch, 0c0h
        jne    NEAR PTR loc_1_FC14
        db  0F6h, 082h, 0B8h, 01Ch, 001h, 000h, 040h
        jne    NEAR PTR loc_1_FC14
        mov    ecx, dword ptr [ebp + 8]
        movzx  ecx, byte ptr [ecx]
        dec    ecx
        cmp    ecx, 0eh
        ja     SHORT loc_1_F82C
        db  0FFh, 024h, 08Dh, 030h, 0FCh, 000h, 000h
loc_1_F82C:
        push   dword ptr [ebp + 18h]
        push   esi
        push   eax
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0FFh, 015h, 070h, 050h, 001h, 000h
        add    esp, 14h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_F848:
        test   byte ptr [ebx + 1ah], 4
        je     NEAR PTR loc_1_FC28
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + eax - 1], 0
        or     dword ptr [ecx + 18h], 40000000h
        mov    edx, ecx
        add    edx, 6
        push   dword ptr [ecx + 2]
        mov    eax, ecx
        add    eax, 20h
        push   eax
        push   edx
        push   ebx
        call   dword ptr [ebx + 80h]
        add    esp, 10h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_F884:
        movsx  eax, byte ptr [ebx]
        db  0F6h, 080h, 0B8h, 01Ch, 001h, 000h, 002h
        je     SHORT loc_1_F8C6
        cmp    dword ptr [ebp + 18h], 0
        je     SHORT loc_1_F8C6
        db  0BFh, 0E0h, 039h, 001h, 000h
        mov    ecx, 3
        mov    eax, dword ptr [ebp + 18h]
        mov    esi, dword ptr [eax + 8]
        repe cmpsd
        jne    NEAR PTR loc_1_FC28
        mov    ax, word ptr [esi]
        cmp    ax, word ptr [edi]
        jne    NEAR PTR loc_1_FC28
        mov    al, byte ptr [esi + 2]
        cmp    al, byte ptr [edi + 2]
        jne    NEAR PTR loc_1_FC28
loc_1_F8C6:
        push   dword ptr [ebp + 8]
        push   ebx
        call   dword ptr [ebx + 7ch]
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_F8D8:
        test   byte ptr [ebx + 1ah], 4
        je     NEAR PTR loc_1_FC28
        movsx  eax, byte ptr [ebx]
        db  0F6h, 080h, 0B8h, 01Ch, 001h, 000h, 002h
        je     SHORT loc_1_F924
        cmp    dword ptr [ebp + 18h], 0
        je     SHORT loc_1_F924
        db  0BFh, 0E0h, 039h, 001h, 000h
        mov    ecx, 3
        mov    eax, dword ptr [ebp + 18h]
        mov    esi, dword ptr [eax + 8]
        repe cmpsd
        jne    NEAR PTR loc_1_FC28
        mov    ax, word ptr [esi]
        cmp    ax, word ptr [edi]
        jne    NEAR PTR loc_1_FC28
        mov    al, byte ptr [esi + 2]
        cmp    al, byte ptr [edi + 2]
        jne    NEAR PTR loc_1_FC28
loc_1_F924:
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp + 8]
        push   ebx
        call   dword ptr [ebx + 84h]
        add    esp, 14h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_F940:
        test   byte ptr [ebx + 1ah], 4
        je     NEAR PTR loc_1_FC28
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 2], 2
        jne    NEAR PTR loc_1_FC28
        mov    byte ptr [ebx + 0e1h], 5ch
        mov    byte ptr [ebx + 0e0h], 5ch
        lea    edi, [ebx + 0e2h]
        mov    esi, dword ptr [ebp - 8]
        movsd
        movsd
        movsd
        movsd
        cmp    dword ptr [ebx + 0c0h], 0
        jne    NEAR PTR loc_1_FC28
        push   1
        db  068h, 020h, 0F6h, 000h, 000h
        push   0ea60h
        add    ebx, 0bch
        push   ebx
        call   sub_1_E782
        add    esp, 10h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_F9A0:
        mov    esi, dword ptr [ebp + 8]
        inc    esi
        db  0BFh, 092h, 024h, 001h, 000h
        db  0A1h, 0A2h, 024h, 001h, 000h
        mov    ecx, eax
        shr    ecx, 2
        repe cmpsd
        jne    NEAR PTR loc_1_FC28
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb
        jne    NEAR PTR loc_1_FC28
        push   ebx
        call   sub_1_10ED0
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_F9D8:
        test   byte ptr [ebx + 1ah], 4
        je     NEAR PTR loc_1_FC28
        mov    ecx, dword ptr [ebp + 8]
        or     dword ptr [ecx + 18h], 40000000h
        mov    eax, ecx
        add    eax, 6
        push   dword ptr [ecx + 2]
        add    ecx, 20h
        push   ecx
        push   eax
        push   ebx
        call   sub_1_EDDC
        add    esp, 10h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_FA08:
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx + 1]
        test   al, 4
        je     SHORT loc_1_FA20
        push   ebx
        call   sub_1_F664
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_FA20:
        test   byte ptr [ebx + 1ah], 4
        je     SHORT loc_1_FA31
        test   al, 1
        je     SHORT loc_1_FA31
        mov    ecx, ebx
        call   sub_1_EAE4
loc_1_FA31:
        mov    ecx, dword ptr [ebp + 8]
        test   byte ptr [ecx + 1], 2
        je     NEAR PTR loc_1_FC28
        push   ebx
        call   sub_1_10F38
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_FA4C:
        movsx  eax, byte ptr [ebx]
        db  0F6h, 080h, 0B8h, 01Ch, 001h, 000h, 002h
        je     SHORT loc_1_FAD0
        cmp    dword ptr [ebp + 18h], 0
        je     SHORT loc_1_FAD0
        db  0BFh, 0E0h, 039h, 001h, 000h
        mov    ecx, 3
        mov    eax, dword ptr [ebp + 18h]
        mov    esi, dword ptr [eax + 8]
        repe cmpsd
        jne    SHORT loc_1_FA80
        mov    ax, word ptr [esi]
        cmp    ax, word ptr [edi]
        jne    SHORT loc_1_FA80
        mov    al, byte ptr [esi + 2]
        cmp    al, byte ptr [edi + 2]
loc_1_FA80:
        je     SHORT loc_1_FAD0
        test   byte ptr [ebx + 1ah], 4
        je     NEAR PTR loc_1_FC28
        mov    eax, dword ptr [ebp + 8]
        add    eax, 6
        mov    dword ptr [ebp - 4], eax
        mov    edi, eax
        mov    eax, dword ptr [ebp + 18h]
        mov    esi, dword ptr [eax + 8]
        movsd
        movsd
        movsd
        movsd
        push   dword ptr [ebp - 4]
        call   sub_1_9D08
        add    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 18h], 0c0000000h
        push   dword ptr [ecx + 2]
        mov    eax, dword ptr [ebp + 18h]
        push   dword ptr [eax + 4]
        push   dword ptr [ebp - 4]
        push   ebx
        call   sub_1_EDDC
        add    esp, 10h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_FAD0:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 6
        mov    dword ptr [ebp - 4], eax
        lea    edi, [ebx + 0d0h]
        mov    esi, eax
        movsd
        movsd
        movsd
        movsw
        movsb
        mov    byte ptr [ebx + 0dfh], 0
        test   byte ptr [ebx + 1ah], 4
        je     NEAR PTR loc_1_FBB8
        cmp    dword ptr [ebp + 18h], 0
        je     SHORT loc_1_FB5B
        lea    edi, [ebp - 2ch]
        mov    esi, dword ptr [ebp - 4]
        movsd
        movsd
        movsd
        movsd
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 18h]
        mov    dword ptr [ebp - 0ch], eax
        mov    edi, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 18h]
        mov    esi, dword ptr [eax + 8]
        movsd
        movsd
        movsd
        movsd
        push   dword ptr [ebp - 4]
        call   sub_1_9D08
        add    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 18h], 0c0000000h
        push   dword ptr [ecx + 2]
        mov    eax, dword ptr [ebp + 18h]
        push   dword ptr [eax + 4]
        push   dword ptr [ebp - 4]
        push   ebx
        call   sub_1_EDDC
        add    esp, 10h
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 18h], eax
        lea    esi, [ebp - 2ch]
        mov    edi, dword ptr [ebp - 4]
        movsd
        movsd
        movsd
        movsd
loc_1_FB5B:
        db  0BFh, 092h, 024h, 001h, 000h
        db  0A1h, 0A2h, 024h, 001h, 000h
        mov    ecx, eax
        shr    ecx, 2
        mov    esi, dword ptr [ebp - 4]
        repe cmpsd
        jne    SHORT loc_1_FB78
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb
loc_1_FB78:
        je     SHORT loc_1_FB88
        mov    ecx, ebx
        call   sub_1_EAE4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_FB88:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + eax - 1], 0
        or     dword ptr [ecx + 18h], 40000000h
        push   dword ptr [ecx + 2]
        mov    eax, ecx
        add    eax, 20h
        push   eax
        push   dword ptr [ebp - 4]
        push   ebx
        call   dword ptr [ebx + 80h]
        add    esp, 10h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_FBB8:
        lea    eax, [ebx + 8ch]
        push   eax
        call   sub_1_E7AB
        add    esp, 4
        mov    byte ptr [ebx + 88h], 0
        and    word ptr [ebx + 0cch], 0ff7fh
        test   byte ptr [ebx + 1ah], 2
        jne    SHORT loc_1_FBE6
        push   ebx
        call   sub_1_1138C
        add    esp, 4
loc_1_FBE6:
        push   ebx
        call   sub_1_110A0
        add    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx + 1]
        cmp    byte ptr [ebx + 8bh], al
        je     SHORT loc_1_FC28
        mov    byte ptr [ebx + 8bh], al
        push   ebx
        call   sub_1_11148
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_FC14:
        push   dword ptr [ebp + 18h]
        push   esi
        push   eax
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0FFh, 015h, 070h, 050h, 001h, 000h
        add    esp, 14h
loc_1_FC28:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h, 048h, 0F8h, 000h, 000h, 02Ch, 0F8h, 000h, 000h, 02Ch, 0F8h, 000h, 000h, 02Ch
        db  0F8h, 000h, 000h, 02Ch, 0F8h, 000h, 000h, 02Ch, 0F8h, 000h, 000h, 02Ch, 0F8h, 000h, 000h, 084h
        db  0F8h, 000h, 000h, 0D8h, 0F8h, 000h, 000h, 040h, 0F9h, 000h, 000h, 0A0h, 0F9h, 000h, 000h, 0D8h
        db  0F9h, 000h, 000h, 02Ch, 0F8h, 000h, 000h, 008h, 0FAh, 000h, 000h, 04Ch, 0FAh, 000h, 000h
sub_1_FC6C:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        db  0C7h, 046h, 07Ch, 078h, 0FDh, 000h, 000h
        mov    dword ptr [esi + 98h], esi
        mov    dword ptr [esi + 0a8h], esi
        mov    dword ptr [esi + 0b8h], esi
        mov    dword ptr [esi + 0c8h], esi
        push   esi
        call   sub_1_11400
        add    esp, 4
        mov    dword ptr [esi + 18h], 402001h
        mov    al, byte ptr [esi]
        push   eax
        push   402001h
        push   402001h
        call   sub_1_E8CF
        add    esp, 0ch
        mov    ax, word ptr [esi + 0cch]
        or     ax, 4000h
        mov    word ptr [esi + 0cch], ax
        test   ah, 80h
        je     SHORT loc_1_FCD7
        push   esi
        call   sub_1_FCDC
        add    esp, 4
loc_1_FCD7:
        pop    esi
        leave
        ret
        db  08Bh, 0FFh
sub_1_FCDC:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        or     dword ptr [esi + 18h], 2
        mov    al, byte ptr [esi]
        push   eax
        push   2
        push   2
        call   sub_1_E8CF
        add    esp, 0ch
        push   3e8h
        mov    al, byte ptr [esi]
        push   eax
        call   sub_1_E8ED
        add    esp, 8
        db  0A1h, 020h, 031h, 001h, 000h
        mov    cl, byte ptr [eax + 45h]
        or     cl, cl
        je     SHORT loc_1_FD6F
        cmp    cl, 1
        jne    SHORT loc_1_FD30
        movsx  eax, byte ptr [esi]
        push   eax
        call   sub_1_E8FF
        add    esp, 4
        and    eax, 2
        cmp    eax, 2
        db  0A1h, 020h, 031h, 001h, 000h
        je     SHORT loc_1_FD6F
loc_1_FD30:
        movsx  ax, byte ptr [eax + 45h]
        or     word ptr [esi + 0cch], ax
        mov    byte ptr [esi + 88h], 4
        call   sub_1_E7F5
        mov    dword ptr [esi + 20h], eax
        push   1
        db  068h, 0E8h, 000h, 001h, 000h
        push   0afc8h
        lea    eax, [esi + 8ch]
        push   eax
        call   sub_1_E782
        add    esp, 10h
        push   esi
        call   sub_1_11260
        add    esp, 4
loc_1_FD6F:
        pop    esi
        leave
        ret
        db  08Bh, 0FFh
loc_1_FD74:
        ret
        db  08Dh, 049h, 000h
loc_1_FD78:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  080h, 03Dh, 09Ch, 039h, 001h, 000h, 000h
        jne    NEAR PTR loc_1_FFB5
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0cch], 3
        je     NEAR PTR loc_1_FFB5
        push   2
        mov    al, byte ptr [esi]
        push   eax
        call   sub_1_E8F3
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_1_FFB5
        test   byte ptr [esi + 0cch], 80h
        je     SHORT loc_1_FE38
        mov    edi, dword ptr [ebp + 0ch]
        lea    ebx, [edi + 0eh]
        db  0B8h, 092h, 024h, 001h, 000h
        mov    edx, ebx
loc_1_FDC9:
        mov    cl, byte ptr [eax]
        cmp    cl, byte ptr [edx]
        jne    SHORT loc_1_FDEC
        or     cl, cl
        je     SHORT loc_1_FDE5
        mov    cl, byte ptr [eax + 1]
        cmp    cl, byte ptr [edx + 1]
        jne    SHORT loc_1_FDEC
        add    eax, 2
        add    edx, 2
        or     cl, cl
        jne    SHORT loc_1_FDC9
loc_1_FDE5:
        sub    eax, eax
        jmp    SHORT loc_1_FDF1
        db  08Dh, 049h, 000h
loc_1_FDEC:
        sbb    eax, eax
        sbb    eax, -1
loc_1_FDF1:
        or     eax, eax
        je     NEAR PTR loc_1_FFB5
        cmp    dword ptr [edi + 2], 0
        jne    SHORT loc_1_FE08
        cmp    byte ptr [ebx], 0
        je     NEAR PTR loc_1_FFB5
loc_1_FE08:
        test   byte ptr [esi + 1ah], 4
        je     SHORT loc_1_FE20
        push   64h
        call   sub_1_1018C
        add    esp, 4
        lea    ebx, [eax + 1]
        jmp    SHORT loc_1_FE46
        db  08Dh, 049h, 000h
loc_1_FE20:
        push   2bch
        call   sub_1_1018C
        add    esp, 4
        lea    ebx, [eax + 0c8h]
        jmp    SHORT loc_1_FE46
        db  08Dh, 049h, 000h
loc_1_FE38:
        push   esi
        call   sub_1_FFBC
        add    esp, 4
        mov    ebx, eax
        mov    edi, dword ptr [ebp + 0ch]
loc_1_FE46:
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [ebp - 4], eax
        mov    al, byte ptr [edi + 1]
        cmp    al, 1
        je     SHORT loc_1_FE58
        setb   cl
        jmp    SHORT loc_1_FEAA
loc_1_FE58:
        mov    edx, dword ptr [esi + 1ch]
        mov    eax, dword ptr [edi + 2]
        cmp    edx, eax
        je     SHORT loc_1_FE68
        seta   cl
        jmp    SHORT loc_1_FEAA
        db  090h
loc_1_FE68:
        mov    eax, dword ptr [edi + 6]
        cmp    eax, dword ptr [ebp - 4]
        je     SHORT loc_1_FE78
        setb   cl
        jmp    SHORT loc_1_FEAA
        db  08Dh, 049h, 000h
loc_1_FE78:
        db  0B9h, 092h, 024h, 001h, 000h
        add    edi, 0eh
loc_1_FE80:
        mov    al, byte ptr [ecx]
        cmp    al, byte ptr [edi]
        jne    SHORT loc_1_FEA0
        or     al, al
        je     SHORT loc_1_FE9C
        mov    al, byte ptr [ecx + 1]
        cmp    al, byte ptr [edi + 1]
        jne    SHORT loc_1_FEA0
        add    ecx, 2
        add    edi, 2
        or     al, al
        jne    SHORT loc_1_FE80
loc_1_FE9C:
        sub    eax, eax
        jmp    SHORT loc_1_FEA5
loc_1_FEA0:
        sbb    eax, eax
        sbb    eax, -1
loc_1_FEA5:
        or     eax, eax
        setl   cl
loc_1_FEAA:
        or     cl, cl
        je     NEAR PTR loc_1_FF54
        cmp    byte ptr [esi + 89h], 1eh
        ja     NEAR PTR loc_1_FF54
        test   byte ptr [esi + 0cch], 80h
        jne    SHORT loc_1_FF00
        mov    edi, dword ptr [esi + 28h]
        or     edi, edi
        je     SHORT loc_1_FEE1
        call   sub_1_E7F5
        sub    eax, edi
        cmp    eax, 36b0h
        jb     NEAR PTR loc_1_FFB5
loc_1_FEE1:
        push   esi
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 20h], eax
        or     word ptr [esi + 0cch], 80h
        call   sub_1_1106C
        add    esp, 4
        mov    dword ptr [esi + 2ch], 0
loc_1_FF00:
        mov    byte ptr [esi + 88h], 4
        call   sub_1_E7F5
        mov    edi, eax
        mov    eax, dword ptr [esi + 2ch]
        mov    ecx, eax
        sub    ecx, edi
        or     eax, eax
        je     SHORT loc_1_FF21
        cmp    ecx, ebx
        jle    NEAR PTR loc_1_FFB5
loc_1_FF21:
        lea    eax, [esi + 8ch]
        push   eax
        call   sub_1_E7AB
        add    esp, 4
        lea    eax, [edi + ebx]
        mov    dword ptr [esi + 2ch], eax
        push   0
        db  068h, 044h, 000h, 001h, 000h
        push   ebx
        lea    eax, [esi + 8ch]
        push   eax
        call   sub_1_E782
        add    esp, 10h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_FF54:
        lea    edi, [esi + 8ch]
        push   edi
        call   sub_1_E7AB
        add    esp, 4
        and    word ptr [esi + 0cch], 0ff5fh
        call   sub_1_E7F5
        mov    dword ptr [esi + 28h], eax
        mov    eax, dword ptr [esi + 18h]
        test   eax, 40000h
        je     SHORT loc_1_FF8C
        mov    ecx, esi
        call   sub_1_EAE4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_FF8C:
        test   eax, 20000h
        je     SHORT loc_1_FFB5
        mov    byte ptr [esi + 88h], 0ah
        mov    eax, dword ptr [esi + 28h]
        mov    dword ptr [esi + 20h], eax
        push   0
        db  068h, 0E8h, 000h, 001h, 000h
        push   0afc8h
        push   edi
        call   sub_1_E782
        add    esp, 10h
loc_1_FFB5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
sub_1_FFBC:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 89h], 0
        call   sub_1_E7F5
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 1ch], 1041500h
        mov    ax, word ptr [esi + 0cch]
        and    al, 3
        cmp    al, 2
        jne    SHORT loc_1_FFF3
        cmp    dword ptr [esi + 44h], 0
        je     SHORT loc_1_FFF3
        mov    dword ptr [esi + 1ch], 1041504h
loc_1_FFF3:
        mov    eax, dword ptr [esi + 18h]
        test   eax, 40000h
        je     SHORT loc_1_1000C
        or     dword ptr [esi + 1ch], 2
        mov    eax, 64h
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_1000C:
        test   eax, 20000h
        je     SHORT loc_1_1002C
        or     dword ptr [esi + 1ch], 1
        push   190h
        call   sub_1_1018C
        add    esp, 4
        add    eax, 0c8h
        pop    esi
        leave
        ret
loc_1_1002C:
        push   898h
        call   sub_1_1018C
        add    esp, 4
        add    eax, 320h
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
sub_1_10044:
        push   esi
        push   edi
        mov    edi, ecx
        test   byte ptr [edi + 0cdh], 2
        jne    NEAR PTR loc_1_100E4
        cmp    byte ptr [edi + 88h], 0
        push   edi
        je     SHORT loc_1_100DC
        call   sub_1_104E8
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_100C0
        test   byte ptr [edi + 0cch], 80h
        jne    SHORT loc_1_1007D
        push   edi
        call   sub_1_FFBC
        add    esp, 4
loc_1_1007D:
        dec    byte ptr [edi + 88h]
        inc    byte ptr [edi + 89h]
        lea    esi, [edi + 8ch]
        push   esi
        call   sub_1_E7AB
        add    esp, 4
        call   sub_1_E7F5
        add    eax, 3e8h
        mov    dword ptr [edi + 2ch], eax
        push   0
        db  068h, 044h, 000h, 001h, 000h
        push   3e8h
        push   esi
        call   sub_1_E782
        add    esp, 10h
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_100C0:
        mov    ecx, edi
        and    word ptr [edi + 0cch], 0ff7fh
        mov    byte ptr [edi + 88h], 4
        call   sub_1_EAE4
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_100DC:
        call   sub_1_E924
        add    esp, 4
loc_1_100E4:
        pop    edi
        pop    esi
        ret
        db  090h
loc_1_100E8:
        push   esi
        mov    esi, ecx
        or     esi, esi
        je     NEAR PTR loc_1_10188
        test   byte ptr [esi + 0cdh], 2
        jne    NEAR PTR loc_1_10188
        cmp    byte ptr [esi + 88h], 0
        je     SHORT loc_1_1016C
        push   8
        push   esi
        call   sub_1_103E8
        add    esp, 8
        or     al, al
        jne    SHORT loc_1_10144
        dec    byte ptr [esi + 88h]
        add    esi, 8ch
        push   esi
        call   sub_1_E7AB
        add    esp, 4
        push   0
        db  068h, 0E8h, 000h, 001h, 000h
        push   2710h
        push   esi
        call   sub_1_E782
        add    esp, 10h
        pop    esi
        ret
loc_1_10144:
        add    esi, 8ch
        push   esi
        call   sub_1_E7AB
        add    esp, 4
        push   1
        db  068h, 0E8h, 000h, 001h, 000h
        push   2710h
        push   esi
        call   sub_1_E782
        add    esp, 10h
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_1016C:
        mov    byte ptr [esi + 88h], 4
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 1ch], 0
        mov    ecx, esi
        call   sub_1_10044
loc_1_10188:
        pop    esi
        ret
        db  08Bh, 0FFh
sub_1_1018C:
        push   ebp
        mov    ebp, esp
        db  0A1h, 018h, 03Ah, 001h, 000h
        imul   eax, eax, 41c64e6dh
        add    eax, 3039h
        db  0A3h, 018h, 03Ah, 001h, 000h
        jne    SHORT loc_1_101AB
        mov    eax, 1
loc_1_101AB:
        sub    edx, edx
        db  0A3h, 018h, 03Ah, 001h, 000h
        div    dword ptr [ebp + 8]
        mov    eax, edx
        leave
        ret
        db  08Dh, 049h, 000h
sub_1_101BC:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, esi
        shr    eax, 2
        push   eax
        call   sub_1_1018C
        add    esp, 4
        mov    ecx, esi
        shr    ecx, 3
        sub    eax, ecx
        add    eax, esi
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
sub_1_101E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 1
        lea    eax, [ebp - 18h]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_10610
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        jne    SHORT loc_1_10210
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_10210:
        mov    ecx, dword ptr [ebp - 8]
        add    ecx, 36h
        mov    dword ptr [ebp - 0ch], ecx
        db  08Bh, 01Dh, 074h, 050h, 001h, 000h
        mov    edx, dword ptr [ebp - 8]
        add    edx, ebx
        mov    eax, dword ptr [ebp - 8]
        add    eax, 80h
        mov    dword ptr [ebp - 14h], eax
        db  0BEh, 0CCh, 050h, 001h, 000h
        mov    edi, dword ptr [ebp - 8]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 2], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 44h], 0
        jbe    NEAR PTR loc_1_102E8
        cmp    byte ptr [ebp + 0ch], 1
        jle    NEAR PTR loc_1_102E8
        mov    ebx, dword ptr [eax + 34h]
        sub    eax, ebx
        cmp    eax, -3ch
        jne    SHORT loc_1_10275
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [eax + 3ch]
loc_1_10275:
        mov    dword ptr [ebp - 10h], ebx
loc_1_10278:
        mov    eax, dword ptr [ebx + 22h]
        and    eax, 60000h
        cmp    eax, 20000h
        jne    SHORT loc_1_102CB
        test   byte ptr [ebx + 9], 0ch
        jne    SHORT loc_1_102CB
        lea    esi, [ebx + 10h]
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    eax, ecx
        lea    ecx, [eax + edx]
        cmp    dword ptr [ebp - 14h], ecx
        jb     SHORT loc_1_102E2
        mov    edi, edx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [ebp - 0ch]
        inc    byte ptr [ecx + 1]
        add    edx, eax
        inc    dword ptr [ebp - 4]
        movsx  eax, byte ptr [ebp + 0ch]
        cmp    eax, dword ptr [ebp - 4]
        jle    SHORT loc_1_102E2
loc_1_102CB:
        mov    ebx, dword ptr [ebx]
        mov    eax, dword ptr [ebp + 8]
        sub    eax, ebx
        cmp    eax, -3ch
        jne    SHORT loc_1_102DD
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [eax + 3ch]
loc_1_102DD:
        cmp    dword ptr [ebp - 10h], ebx
        jne    SHORT loc_1_10278
loc_1_102E2:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 34h], ebx
loc_1_102E8:
        sub    edx, dword ptr [ebp - 8]
        lea    ecx, [edx - 36h]
        mov    esi, dword ptr [ebp - 8]
        mov    word ptr [esi + 3], cx
        mov    word ptr [esi + 17h], cx
        lea    eax, [ecx + 11h]
        mov    word ptr [esi + 23h], ax
        mov    eax, dword ptr [ebp + 14h]
        or     eax, eax
        je     SHORT loc_1_10324
        push   dword ptr [ebp + 18h]
        push   edx
        push   0
        push   eax
        push   dword ptr [ebp - 18h]
        push   dword ptr [ebp + 8]
        call   sub_1_10678
        add    esp, 18h
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_10324:
        push   0
        push   0
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax]
        push   eax
        movzx  eax, cx
        push   eax
        push   dword ptr [ebp - 0ch]
        db  068h, 0ACh, 039h, 001h, 000h
        call   sub_1_E8F9
        add    esp, 18h
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
sub_1_1034C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    ebx, ecx
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_10610
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_10384
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_10384:
        mov    edi, eax
        db  0BEh, 07Ch, 050h, 001h, 000h
        mov    ecx, 0dh
        rep movsd
        movsw
        movsb
        lea    edi, [eax + 37h]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        lea    ecx, [ebx + 1]
        mov    word ptr [eax + 3], cx
        mov    word ptr [eax + 17h], cx
        add    cx, 11h
        mov    word ptr [eax + 23h], cx
        push   0
        add    ebx, 37h
        push   ebx
        push   1eh
        db  068h, 0E0h, 039h, 001h, 000h
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 8]
        call   sub_1_10678
        add    esp, 18h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_103DC:
        push   4
        push   ecx
        call   sub_1_103E8
        add    esp, 8
        ret
sub_1_103E8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_1_10610
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_1040C
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_1040C:
        mov    edi, eax
        db  08Bh, 015h, 020h, 051h, 001h, 000h
        db  0BEh, 0D4h, 04Eh, 001h, 000h
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebp + 0ch]
        cmp    eax, 8
        je     SHORT loc_1_10450
        mov    cx, word ptr [ebx + 0cch]
        test   ch, 1
        je     SHORT loc_1_10450
        mov    al, 1
        db  0BEh, 004h, 03Ah, 001h, 000h
        and    cx, 0feffh
        mov    word ptr [ebx + 0cch], cx
        jmp    SHORT loc_1_10467
loc_1_10450:
        db  0BEh, 0E0h, 039h, 001h, 000h
        cmp    eax, 8
        mov    al, 1dh
        je     SHORT loc_1_10467
        mov    al, 1eh
        or     word ptr [ebx + 0cch], 100h
loc_1_10467:
        push   0
        push   edx
        push   eax
        push   esi
        push   dword ptr [ebp - 4]
        push   ebx
        call   sub_1_10678
        add    esp, 18h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
sub_1_10480:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 0e2h], 0
        je     SHORT loc_1_104E0
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        call   sub_1_10610
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_104E0
        db  08Bh, 015h, 01Ch, 04Fh, 001h, 000h
        db  0BEh, 024h, 04Fh, 001h, 000h
        mov    edi, eax
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        push   0
        push   edx
        push   0
        lea    eax, [ebx + 0e2h]
        push   eax
        push   dword ptr [ebp - 4]
        push   ebx
        call   sub_1_10678
        add    esp, 18h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_104E0:
        pop    edi
        mov    al, 1
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
sub_1_104E8:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 8]
        push   eax
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_1_10610
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_10510
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_10510:
        lea    edx, [eax + 36h]
        db  08Bh, 00Dh, 020h, 04Fh, 001h, 000h
        mov    dword ptr [ebp - 4], ecx
        db  0BEh, 06Ch, 04Fh, 001h, 000h
        mov    edi, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 3
        rep movsb
        db  080h, 03Dh, 09Ch, 039h, 001h, 000h, 000h
        jne    SHORT loc_1_10550
        test   byte ptr [ebx + 0cdh], 2
        jne    SHORT loc_1_10550
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [edx + 2], eax
        mov    eax, dword ptr [ebx + 20h]
        mov    dword ptr [edx + 6], eax
        jmp    SHORT loc_1_10566
loc_1_10550:
        mov    byte ptr [edx + 1], 0
        mov    dword ptr [edx + 2], 0
        mov    dword ptr [edx + 6], 0
        mov    byte ptr [edx + 0eh], 0
loc_1_10566:
        push   0
        db  0FFh, 035h, 020h, 04Fh, 001h, 000h
        push   1eh
        db  068h, 0E0h, 039h, 001h, 000h
        push   dword ptr [ebp - 8]
        push   ebx
        call   sub_1_10678
        add    esp, 18h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
sub_1_10588:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_1_10610
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_105B0
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_105B0:
        mov    edi, eax
        db  0BEh, 028h, 050h, 001h, 000h
        mov    ecx, 0eh
        rep movsd
        mov    byte ptr [eax + 37h], 2
        lea    edi, [ebp - 14h]
        mov    eax, 20202020h
        stosd
        stosd
        stosd
        stosd
        mov    esi, dword ptr [ebp + 0ch]
        add    esi, 10h
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [ecx - 1]
        lea    edi, [ebp - 14h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        push   0
        push   38h
        push   0
        lea    eax, [ebp - 14h]
        push   eax
        push   dword ptr [ebp - 4]
        push   ebx
        call   sub_1_10678
        add    esp, 18h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_10610:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_1_E8B6
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        jne    SHORT loc_1_10630
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_10630:
        movsx  eax, byte ptr [edi]
        db  0F6h, 080h, 0B8h, 01Ch, 001h, 000h, 002h
        je     SHORT loc_1_10644
        mov    esi, dword ptr [edx + 34h]
        add    esi, 62h
        jmp    SHORT loc_1_10647
loc_1_10644:
        mov    esi, dword ptr [edx + 0ch]
loc_1_10647:
        lea    ebx, [esi + 4]
        mov    byte ptr [edx + 8], 0a0h
        mov    al, byte ptr [edi + 9]
        mov    byte ptr [edx + 0bh], al
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, 7
        rep stosd
        mov    dword ptr [esi], 424d53ffh
        mov    byte ptr [ebx], 25h
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], edx
        lea    eax, [esi + 20h]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
sub_1_10678:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax]
        db  0F6h, 080h, 0B8h, 01Ch, 001h, 000h, 002h
        mov    edx, dword ptr [ebp + 0ch]
        je     NEAR PTR loc_1_1072C
        mov    ebx, dword ptr [edx + 34h]
        mov    eax, dword ptr [ebp + 1ch]
        or     eax, eax
        jne    SHORT loc_1_106C4
        mov    dword ptr [ebx + 6], 0
        lea    edi, [ebx + 0ah]
        mov    eax, 0ffffffffh
        stosd
        stosw
        mov    word ptr [ebx + 10h], 5305h
        mov    byte ptr [ebx + 5], 14h
        lea    edi, [edx + 2ch]
        stosd
        stosw
        jmp    SHORT loc_1_106E1
        db  08Bh, 0FFh
loc_1_106C4:
        mov    esi, dword ptr [eax]
        lea    edi, [edx + 2ch]
        movsd
        movsw
        mov    esi, dword ptr [eax]
        lea    edi, [ebx + 6]
        add    esi, 6
        mov    ecx, 7
        rep movsd
        movsw
        mov    byte ptr [ebx + 5], 4
loc_1_106E1:
        lea    edi, [ebx + 42h]
        mov    esi, dword ptr [ebp + 10h]
        movsd
        movsd
        movsd
        movsd
        mov    byte ptr [ebx + 51h], 0
        db  0BEh, 048h, 023h, 001h, 000h
        lea    edi, [ebx + 52h]
        movsd
        movsd
        movsd
        movsd
        mov    byte ptr [ebx + 3eh], 0fch
        mov    al, byte ptr [ebp + 14h]
        or     al, al
        je     SHORT loc_1_1071C
        cmp    al, 1dh
        je     SHORT loc_1_1071C
        cmp    al, 1eh
        jne    SHORT loc_1_10714
        mov    byte ptr [ebx + 3fh], 2
        jmp    SHORT loc_1_10720
loc_1_10714:
        mov    byte ptr [ebx + 3fh], 3
        jmp    SHORT loc_1_10720
        db  08Bh, 0FFh
loc_1_1071C:
        mov    byte ptr [ebx + 3fh], 1
loc_1_10720:
        mov    eax, dword ptr [ebp + 18h]
        add    ax, 64h
        jmp    SHORT loc_1_10743
        db  08Dh, 049h, 000h
loc_1_1072C:
        lea    edi, [edx + 12h]
        mov    esi, dword ptr [ebp + 10h]
        movsd
        movsd
        movsd
        movsd
        mov    al, byte ptr [ebp + 14h]
        mov    byte ptr [edx + 21h], al
        mov    eax, dword ptr [ebp + 18h]
        add    ax, 20h
loc_1_10743:
        push   edx
        mov    word ptr [edx + 10h], ax
        call   sub_1_E662
        add    esp, 4
        and    eax, 0ffh
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
sub_1_1075C:
        push   ebx
        push   esi
        push   edi
        db  0BFh, 0CCh, 050h, 001h, 000h
        sub    eax, eax
        mov    ecx, 13h
        rep stosd
        db  0C7h, 005h, 068h, 050h, 001h, 000h, 0CCh, 050h, 001h, 000h
        db  0C6h, 005h, 0CCh, 050h, 001h, 000h, 011h
        db  0B8h, 002h, 051h, 001h, 000h
        db  0B9h, 0CCh, 050h, 001h, 000h
        sub    ax, cx
        add    ax, 20h
        db  066h, 0A3h, 0E5h, 050h, 001h, 000h
        db  0C6h, 005h, 0E7h, 050h, 001h, 000h, 003h
        db  066h, 0C7h, 005h, 0E9h, 050h, 001h, 000h, 001h, 000h
        db  066h, 0C7h, 005h, 0EBh, 050h, 001h, 000h, 001h, 000h
        db  066h, 0C7h, 005h, 0EDh, 050h, 001h, 000h, 002h, 000h
        db  0BEh, 0CCh, 039h, 001h, 000h
        db  0BFh, 0F1h, 050h, 001h, 000h
        movsd
        movsd
        movsd
        movsd
        movsb
        db  0C7h, 005h, 024h, 050h, 001h, 000h, 002h, 051h, 001h, 000h
        db  0C6h, 005h, 002h, 051h, 001h, 000h, 00Ah
        db  0C6h, 005h, 003h, 051h, 001h, 000h, 001h
        db  08Bh, 015h, 0A2h, 024h, 001h, 000h
        db  0BEh, 092h, 024h, 001h, 000h
        db  0BFh, 008h, 051h, 001h, 000h
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        lea    eax, [edx + 3ch]
        db  0A3h, 074h, 050h, 001h, 000h
        db  0BFh, 07Ch, 050h, 001h, 000h
        sub    eax, eax
        mov    ecx, 0dh
        rep stosd
        stosw
        stosb
        db  0C7h, 005h, 060h, 050h, 001h, 000h, 07Ch, 050h, 001h, 000h
        db  0C6h, 005h, 07Ch, 050h, 001h, 000h, 011h
        db  0B8h, 0B2h, 050h, 001h, 000h
        db  0B9h, 07Ch, 050h, 001h, 000h
        sub    ax, cx
        add    ax, 20h
        db  066h, 0A3h, 095h, 050h, 001h, 000h
        db  0C6h, 005h, 097h, 050h, 001h, 000h, 003h
        db  066h, 0C7h, 005h, 099h, 050h, 001h, 000h, 001h, 000h
        db  066h, 0C7h, 005h, 09Bh, 050h, 001h, 000h, 001h, 000h
        db  066h, 0C7h, 005h, 09Dh, 050h, 001h, 000h, 002h, 000h
        db  0BEh, 0CCh, 039h, 001h, 000h
        db  0BFh, 0A1h, 050h, 001h, 000h
        movsd
        movsd
        movsd
        movsd
        movsb
        db  0C7h, 005h, 018h, 050h, 001h, 000h, 0B2h, 050h, 001h, 000h
        db  0C6h, 005h, 0B2h, 050h, 001h, 000h, 00Bh
        db  0BFh, 0D4h, 04Eh, 001h, 000h
        sub    eax, eax
        mov    ecx, 12h
        rep stosd
        db  0C7h, 005h, 078h, 050h, 001h, 000h, 0D4h, 04Eh, 001h, 000h
        db  0C6h, 005h, 0D4h, 04Eh, 001h, 000h, 011h
        db  0B8h, 00Ah, 04Fh, 001h, 000h
        db  0B9h, 0D4h, 04Eh, 001h, 000h
        sub    ax, cx
        add    ax, 20h
        db  066h, 0A3h, 0EDh, 04Eh, 001h, 000h
        db  0C6h, 005h, 0EFh, 04Eh, 001h, 000h, 003h
        db  066h, 0C7h, 005h, 0F1h, 04Eh, 001h, 000h, 001h, 000h
        db  066h, 0C7h, 005h, 0F3h, 04Eh, 001h, 000h, 001h, 000h
        db  066h, 0C7h, 005h, 0F5h, 04Eh, 001h, 000h, 002h, 000h
        db  0BEh, 0CCh, 039h, 001h, 000h
        db  0BFh, 0F9h, 04Eh, 001h, 000h
        movsd
        movsd
        movsd
        movsd
        movsb
        db  0C7h, 005h, 064h, 050h, 001h, 000h, 00Ah, 04Fh, 001h, 000h
        db  0C6h, 005h, 00Ah, 04Fh, 001h, 000h, 002h
        db  0BEh, 092h, 024h, 001h, 000h
        db  0BFh, 00Ch, 04Fh, 001h, 000h
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        lea    eax, [edx + 38h]
        db  0A3h, 020h, 051h, 001h, 000h
        mov    bx, dx
        lea    eax, [ebx + 2]
        db  066h, 0A3h, 0D7h, 04Eh, 001h, 000h
        db  066h, 0A3h, 0EBh, 04Eh, 001h, 000h
        add    ax, 11h
        db  066h, 0A3h, 0F7h, 04Eh, 001h, 000h
        db  0BFh, 06Ch, 04Fh, 001h, 000h
        sub    eax, eax
        mov    ecx, 15h
        rep stosd
        db  0C7h, 005h, 01Ch, 051h, 001h, 000h, 06Ch, 04Fh, 001h, 000h
        db  0C6h, 005h, 06Ch, 04Fh, 001h, 000h, 011h
        db  0B8h, 0A2h, 04Fh, 001h, 000h
        db  0B9h, 06Ch, 04Fh, 001h, 000h
        sub    ax, cx
        add    ax, 20h
        db  066h, 0A3h, 085h, 04Fh, 001h, 000h
        db  0C6h, 005h, 087h, 04Fh, 001h, 000h, 003h
        db  066h, 0C7h, 005h, 089h, 04Fh, 001h, 000h, 001h, 000h
        db  066h, 0C7h, 005h, 08Bh, 04Fh, 001h, 000h, 001h, 000h
        db  066h, 0C7h, 005h, 08Dh, 04Fh, 001h, 000h, 002h, 000h
        db  0BEh, 0CCh, 039h, 001h, 000h
        db  0BFh, 091h, 04Fh, 001h, 000h
        movsd
        movsd
        movsd
        movsd
        movsb
        db  0C7h, 005h, 0C8h, 050h, 001h, 000h, 0A2h, 04Fh, 001h, 000h
        db  0C6h, 005h, 0A2h, 04Fh, 001h, 000h, 008h
        db  0C6h, 005h, 0A3h, 04Fh, 001h, 000h, 001h
        db  0BEh, 092h, 024h, 001h, 000h
        db  0BFh, 0B0h, 04Fh, 001h, 000h
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        lea    eax, [edx + 44h]
        db  0A3h, 020h, 04Fh, 001h, 000h
        lea    eax, [ebx + 0eh]
        db  066h, 0A3h, 06Fh, 04Fh, 001h, 000h
        db  066h, 0A3h, 083h, 04Fh, 001h, 000h
        add    ax, 11h
        db  066h, 0A3h, 08Fh, 04Fh, 001h, 000h
        db  0BFh, 028h, 050h, 001h, 000h
        sub    eax, eax
        mov    ecx, 0eh
        rep stosd
        db  0C7h, 005h, 01Ch, 050h, 001h, 000h, 028h, 050h, 001h, 000h
        db  0C6h, 005h, 028h, 050h, 001h, 000h, 011h
        db  0B8h, 05Eh, 050h, 001h, 000h
        db  0B9h, 028h, 050h, 001h, 000h
        sub    ax, cx
        add    ax, 20h
        db  066h, 0A3h, 041h, 050h, 001h, 000h
        db  0C6h, 005h, 043h, 050h, 001h, 000h, 003h
        db  066h, 0C7h, 005h, 045h, 050h, 001h, 000h, 001h, 000h
        db  066h, 0C7h, 005h, 047h, 050h, 001h, 000h, 001h, 000h
        db  066h, 0C7h, 005h, 049h, 050h, 001h, 000h, 002h, 000h
        db  0BEh, 0CCh, 039h, 001h, 000h
        db  0BFh, 04Dh, 050h, 001h, 000h
        movsd
        movsd
        movsd
        movsd
        movsb
        db  0C7h, 005h, 020h, 050h, 001h, 000h, 05Eh, 050h, 001h, 000h
        db  0C6h, 005h, 05Eh, 050h, 001h, 000h, 00Eh
        db  066h, 0C7h, 005h, 02Bh, 050h, 001h, 000h, 002h, 000h
        db  066h, 0C7h, 005h, 03Fh, 050h, 001h, 000h, 002h, 000h
        db  066h, 0C7h, 005h, 04Bh, 050h, 001h, 000h, 013h, 000h
        db  0BFh, 024h, 04Fh, 001h, 000h
        sub    eax, eax
        mov    ecx, 12h
        rep stosd
        db  0C7h, 005h, 018h, 051h, 001h, 000h, 024h, 04Fh, 001h, 000h
        db  0C6h, 005h, 024h, 04Fh, 001h, 000h, 011h
        db  0B8h, 05Ah, 04Fh, 001h, 000h
        db  0B9h, 024h, 04Fh, 001h, 000h
        sub    ax, cx
        add    ax, 20h
        db  066h, 0A3h, 03Dh, 04Fh, 001h, 000h
        db  0C6h, 005h, 03Fh, 04Fh, 001h, 000h, 003h
        db  066h, 0C7h, 005h, 041h, 04Fh, 001h, 000h, 001h, 000h
        db  066h, 0C7h, 005h, 043h, 04Fh, 001h, 000h, 001h, 000h
        db  066h, 0C7h, 005h, 045h, 04Fh, 001h, 000h, 002h, 000h
        db  0BEh, 0CCh, 039h, 001h, 000h
        db  0BFh, 049h, 04Fh, 001h, 000h
        movsd
        movsd
        movsd
        movsd
        movsb
        db  0C7h, 005h, 06Ch, 050h, 001h, 000h, 05Ah, 04Fh, 001h, 000h
        db  0C6h, 005h, 05Ah, 04Fh, 001h, 000h, 00Dh
        db  0BEh, 092h, 024h, 001h, 000h
        db  0BFh, 05Bh, 04Fh, 001h, 000h
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        inc    bx
        db  066h, 089h, 01Dh, 027h, 04Fh, 001h, 000h
        mov    ax, bx
        db  066h, 0A3h, 03Bh, 04Fh, 001h, 000h
        mov    ax, bx
        add    ax, 11h
        db  066h, 0A3h, 047h, 04Fh, 001h, 000h
        add    edx, 37h
        db  089h, 015h, 01Ch, 04Fh, 001h, 000h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_10B3C:
        push   ebp
        mov    ebp, esp
        sub    esp, 9ch
        push   esi
        mov    esi, ecx
        mov    ax, word ptr [esi + 0cch]
        test   ax, 1620h
        jne    NEAR PTR loc_1_10CD9
        test   ah, 8
        jne    SHORT loc_1_10B68
        test   byte ptr [esi + 1ah], 2
        je     NEAR PTR loc_1_10CD9
loc_1_10B68:
        test   byte ptr [esi + 1ah], 4
        jne    NEAR PTR loc_1_10CD9
        cmp    byte ptr [esi + 0d0h], 0
        jne    SHORT loc_1_10B8C
        or     ax, 400h
        mov    word ptr [esi + 0cch], ax
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_10B8C:
        or     ax, 20h
        mov    word ptr [esi + 0cch], ax
        and    ax, 0dfffh
        mov    word ptr [esi + 0cch], ax
        db  0C7h, 085h, 064h, 0FFh, 0FFh, 0FFh, 0F0h, 039h, 001h, 000h
        mov    dword ptr [ebp - 98h], 0ffffffffh
        mov    word ptr [ebp - 86h], 1
        lea    eax, [esi + 0ceh]
        mov    dword ptr [ebp - 84h], eax
        lea    eax, [ebp - 9ch]
        push   eax
        lea    edx, [esi + 3ch]
        mov    ecx, esi
        call   sub_1_10CDC
        or     eax, eax
        jne    SHORT loc_1_10BFB
        mov    dword ptr [ebp - 98h], 80000000h
        lea    eax, [ebp - 9ch]
        push   eax
        lea    edx, [esi + 5ch]
        mov    ecx, esi
        call   sub_1_10CDC
loc_1_10BFB:
        mov    cx, word ptr [esi + 0cch]
        and    cx, 0ffdfh
        mov    word ptr [esi + 0cch], cx
        or     eax, eax
        mov    al, byte ptr [esi + 8ah]
        je     SHORT loc_1_10C6C
        add    al, 2
        mov    byte ptr [esi + 8ah], al
        test   ch, 10h
        jne    SHORT loc_1_10C60
        cmp    al, 6
        ja     SHORT loc_1_10C60
        test   ch, 4
        jne    NEAR PTR loc_1_10CD9
        cmp    dword ptr [esi + 0b0h], 0
        jne    NEAR PTR loc_1_10CD9
        push   1
        db  068h, 03Ch, 00Bh, 001h, 000h
        push   7530h
        add    esi, 0ach
        push   esi
        call   sub_1_E782
        add    esp, 10h
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_10C60:
        push   esi
        call   sub_1_10F38
        add    esp, 4
        pop    esi
        leave
        ret
loc_1_10C6C:
        or     al, al
        je     SHORT loc_1_10C78
        dec    al
        mov    byte ptr [esi + 8ah], al
loc_1_10C78:
        test   ch, 8
        je     SHORT loc_1_10CB5
        and    cx, 0f7ffh
        or     dword ptr [esi + 18h], 20000h
        mov    word ptr [esi + 0cch], cx
        mov    al, byte ptr [esi]
        push   eax
        push   20000h
        push   20000h
        call   sub_1_E8CF
        add    esp, 0ch
        push   3e8h
        mov    al, byte ptr [esi]
        push   eax
        call   sub_1_E8ED
        add    esp, 8
loc_1_10CB5:
        cmp    dword ptr [esi + 0b0h], 0
        jne    SHORT loc_1_10CD9
        push   1
        db  068h, 03Ch, 00Bh, 001h, 000h
        push   0afc80h
        add    esi, 0ach
        push   esi
        call   sub_1_E782
        add    esp, 10h
loc_1_10CD9:
        pop    esi
        leave
        ret
sub_1_10CDC:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    esi, edx
        mov    edi, ecx
        mov    dword ptr [ebp - 14h], 32h
        test   byte ptr [edi + 0cdh], 16h
        je     SHORT loc_1_10D08
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        db  08Dh, 049h, 000h
loc_1_10D08:
        lea    eax, [ebp - 2]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 8], eax
        lea    eax, [ebp - 4]
        mov    dword ptr [ecx + 0ch], eax
        mov    ecx, dword ptr [esi + 10h]
        or     ecx, ecx
        je     SHORT loc_1_10D50
        mov    eax, dword ptr [esi + 14h]
        or     eax, eax
        je     SHORT loc_1_10D50
        lea    edx, [eax + eax*4]
        lea    edx, [edx + edx*4]
        add    edx, eax
        mov    eax, dword ptr [edx + ecx - 4]
        mov    edx, dword ptr [ecx + 16h]
        cmp    eax, edx
        jbe    SHORT loc_1_10D3A
        mov    edx, eax
loc_1_10D3A:
        sub    eax, eax
loc_1_10D3C:
        cmp    byte ptr [edx + eax], 0
        je     SHORT loc_1_10D48
        inc    eax
        cmp    eax, 31h
        jbe    SHORT loc_1_10D3C
loc_1_10D48:
        sub    eax, ecx
        lea    eax, [eax + edx + 4ch]
        jmp    SHORT loc_1_10D6D
loc_1_10D50:
        mov    eax, 400h
        cmp    dword ptr [esi + 8], 0
        jbe    SHORT loc_1_10D6D
        mov    eax, dword ptr [esi + 14h]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        add    eax, eax
        mov    dword ptr [ebp - 14h], 4bh
loc_1_10D6D:
        lea    ebx, [eax + 0fffh]
        and    ebx, 0fffff000h
        mov    eax, ebx
        sar    eax, 0ch
        mov    dword ptr [ebp - 18h], eax
        lea    eax, [ebp - 10h]
        push   eax
        push   dword ptr [ebp - 18h]
        call   sub_1_E5FB
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        or     eax, eax
        je     SHORT loc_1_10E14
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 10h], bx
        mov    dword ptr [ecx + 12h], eax
        cmp    dword ptr [esi + 10h], 0
        jne    SHORT loc_1_10DB8
        cmp    dword ptr [esi + 8], 0
        jbe    SHORT loc_1_10DB8
        mov    al, byte ptr [edi]
        push   eax
        push   ecx
        call   sub_1_115A0
        jmp    SHORT loc_1_10DC3
loc_1_10DB8:
        mov    al, byte ptr [edi]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_E840
loc_1_10DC3:
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_10E20
        push   dword ptr [ebp - 10h]
        call   sub_1_E62B
        add    esp, 4
        cmp    dword ptr [ebp - 8], 0eah
        jne    NEAR PTR loc_1_10E70
        movzx  eax, word ptr [ebp - 2]
        imul   eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 14h], 4bh
        cmp    eax, 0ffffh
        jg     SHORT loc_1_10E05
        cmp    eax, ebx
        jg     NEAR PTR loc_1_10D6D
loc_1_10E05:
        pop    edi
        mov    eax, 1
        pop    esi
        pop    ebx
        leave
        ret    4
        db  08Dh, 049h, 000h
loc_1_10E14:
        pop    edi
        mov    eax, 1
        pop    esi
        pop    ebx
        leave
        ret    4
loc_1_10E20:
        cmp    dword ptr [esi + 8], 0
        jbe    SHORT loc_1_10E2F
        push   edi
        call   sub_1_1138C
        add    esp, 4
loc_1_10E2F:
        cmp    dword ptr [esi + 10h], 0
        je     SHORT loc_1_10E40
        push   dword ptr [esi + 1ch]
        call   sub_1_E62B
        add    esp, 4
loc_1_10E40:
        push   dword ptr [ebp - 18h]
        push   dword ptr [ebp - 10h]
        call   sub_1_E618
        add    esp, 8
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 10h], eax
        mov    dword ptr [esi + 18h], ebx
        movzx  eax, word ptr [ebp - 4]
        mov    dword ptr [esi + 14h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [esi + 1ch], eax
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        db  08Dh, 049h, 000h
loc_1_10E70:
        cmp    dword ptr [ebp - 8], 35h
        je     SHORT loc_1_10E9C
        cmp    dword ptr [ebp - 8], 910h
        je     SHORT loc_1_10E9C
        cmp    dword ptr [ebp - 8], 32h
        je     SHORT loc_1_10E9C
        cmp    dword ptr [ebp - 8], 8
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        jne    SHORT loc_1_10E98
        ret    4
        db  090h
loc_1_10E98:
        ret    4
        db  090h
loc_1_10E9C:
        push   edi
        call   sub_1_1106C
        add    esp, 4
        mov    byte ptr [edi + 88h], 4
        mov    dword ptr [edi + 20h], 0
        mov    dword ptr [edi + 1ch], 0
        mov    ecx, edi
        call   sub_1_10044
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        db  08Dh, 049h, 000h
sub_1_10ED0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ax, word ptr [edi + 0cch]
        test   ah, 1eh
        jne    SHORT loc_1_10F31
        test   al, 3
        je     SHORT loc_1_10F31
        db  080h, 03Dh, 09Ch, 039h, 001h, 000h, 000h
        jne    SHORT loc_1_10F31
        test   byte ptr [edi + 1ah], 6
        jne    SHORT loc_1_10F31
        lea    esi, [edi + 0ach]
        push   esi
        call   sub_1_E7AB
        add    esp, 4
        or     word ptr [edi + 0cch], 2800h
        mov    byte ptr [edi + 8ah], 0
        push   1
        db  068h, 03Ch, 00Bh, 001h, 000h
        push   64h
        call   sub_1_101BC
        add    esp, 4
        push   eax
        push   esi
        call   sub_1_E782
        add    esp, 10h
loc_1_10F31:
        pop    edi
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
sub_1_10F38:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 0cch]
        test   ah, 18h
        jne    SHORT loc_1_10F56
        test   byte ptr [esi + 1ah], 2
        je     NEAR PTR loc_1_10FFA
loc_1_10F56:
        or     ax, 1000h
        mov    word ptr [esi + 0cch], ax
        and    ax, 0f3ffh
        mov    word ptr [esi + 0cch], ax
        mov    eax, dword ptr [esi + 18h]
        test   eax, 30000h
        je     SHORT loc_1_10F9D
        and    eax, 0fffcffffh
        mov    dword ptr [esi + 18h], eax
        mov    al, byte ptr [esi]
        push   eax
        push   0
        push   30000h
        call   sub_1_E8CF
        add    esp, 0ch
        push   0ah
        mov    al, byte ptr [esi]
        push   eax
        call   sub_1_E8ED
        add    esp, 8
loc_1_10F9D:
        lea    edi, [esi + 0ach]
        push   edi
        call   sub_1_E7AB
        add    esp, 4
        test   byte ptr [esi + 0cch], 20h
        jne    SHORT loc_1_10FFA
        cmp    dword ptr [esi + 4ch], 0
        je     SHORT loc_1_10FCD
        push   dword ptr [esi + 58h]
        call   sub_1_E62B
        add    esp, 4
        mov    dword ptr [esi + 4ch], 0
loc_1_10FCD:
        cmp    dword ptr [esi + 6ch], 0
        je     SHORT loc_1_10FE5
        push   dword ptr [esi + 78h]
        call   sub_1_E62B
        add    esp, 4
        mov    dword ptr [esi + 6ch], 0
loc_1_10FE5:
        push   1
        db  068h, 000h, 010h, 001h, 000h
        push   1b7740h
        push   edi
        call   sub_1_E782
        add    esp, 10h
loc_1_10FFA:
        pop    edi
        pop    esi
        leave
        ret
        db  08Bh, 0FFh
loc_1_11000:
        push   esi
        mov    esi, ecx
        mov    ax, word ptr [esi + 0cch]
        and    ax, 0efffh
        mov    word ptr [esi + 0cch], ax
        and    al, 3
        cmp    al, 1
        jb     SHORT loc_1_11052
        mov    eax, dword ptr [esi + 18h]
        test   eax, 10000h
        jne    SHORT loc_1_11052
        or     eax, 10000h
        mov    dword ptr [esi + 18h], eax
        mov    al, byte ptr [esi]
        push   eax
        push   10000h
        push   10000h
        call   sub_1_E8CF
        add    esp, 0ch
        push   3e8h
        mov    al, byte ptr [esi]
        push   eax
        call   sub_1_E8ED
        add    esp, 8
loc_1_11052:
        mov    ax, word ptr [esi + 0cch]
        and    al, 3
        cmp    al, 2
        jne    SHORT loc_1_11068
        push   esi
        call   sub_1_10ED0
        add    esp, 4
loc_1_11068:
        pop    esi
        ret
        db  08Bh, 0FFh
sub_1_1106C:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    cx, word ptr [eax + 0cch]
        test   ch, 18h
        jne    SHORT loc_1_11084
        test   byte ptr [eax + 1ah], 2
        je     SHORT loc_1_1109B
loc_1_11084:
        add    eax, 0ach
        or     cx, 400h
        mov    word ptr [eax + 20h], cx
        push   eax
        call   sub_1_E7AB
        add    esp, 4
loc_1_1109B:
        leave
        ret
        db  08Dh, 049h, 000h
sub_1_110A0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 0cch]
        test   ah, 4
        je     NEAR PTR loc_1_11142
        test   ah, 10h
        je     SHORT loc_1_110E4
        cmp    dword ptr [esi + 0b0h], 0
        jne    SHORT loc_1_11142
        push   1
        db  068h, 000h, 010h, 001h, 000h
        push   1b7740h
        add    esi, 0ach
        push   esi
        call   sub_1_E782
        add    esp, 10h
        pop    esi
        leave
        ret
        db  090h
loc_1_110E4:
        and    ax, 0fbffh
        mov    word ptr [esi + 0cch], ax
        test   ah, 8
        jne    SHORT loc_1_110FA
        test   byte ptr [esi + 1ah], 2
        je     SHORT loc_1_11142
loc_1_110FA:
        cmp    byte ptr [esi + 8ah], 6
        jbe    SHORT loc_1_1110A
        mov    byte ptr [esi + 8ah], 5
loc_1_1110A:
        or     ax, 2000h
        mov    word ptr [esi + 0cch], ax
        cmp    dword ptr [esi + 0b0h], 0
        jne    SHORT loc_1_11142
        push   1
        db  068h, 03Ch, 00Bh, 001h, 000h
        push   1388h
        call   sub_1_101BC
        add    esp, 4
        push   eax
        add    esi, 0ach
        push   esi
        call   sub_1_E782
        add    esp, 10h
loc_1_11142:
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
sub_1_11148:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        test   byte ptr [edi + 0cdh], 3ch
        jne    SHORT loc_1_11195
        test   byte ptr [edi + 1ah], 2
        je     SHORT loc_1_11195
        lea    esi, [edi + 0ach]
        push   esi
        call   sub_1_E7AB
        add    esp, 4
        or     word ptr [edi + 0cch], 2000h
        push   1
        db  068h, 03Ch, 00Bh, 001h, 000h
        push   2710h
        call   sub_1_101BC
        add    esp, 4
        push   eax
        push   esi
        call   sub_1_E782
        add    esp, 10h
loc_1_11195:
        pop    edi
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_1119C:
        push   ebx
        push   esi
        push   edi
        mov    edi, ecx
        test   byte ptr [edi + 0cch], 20h
        je     SHORT loc_1_111C8
        push   1
        db  068h, 09Ch, 011h, 001h, 000h
        push   5
        add    edi, 9ch
        push   edi
        call   sub_1_E782
        add    esp, 10h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_111C8:
        test   byte ptr [edi + 1ah], 2
        je     SHORT loc_1_11242
        mov    esi, dword ptr [edi + 4ch]
        or     esi, esi
        je     SHORT loc_1_11208
        mov    ebx, dword ptr [edi + 50h]
        or     ebx, ebx
        jle    SHORT loc_1_111F6
loc_1_111DC:
        push   0dbba0h
        push   dword ptr [esi + 16h]
        push   esi
        push   edi
        call   sub_1_EC80
        add    esp, 10h
        dec    ebx
        add    esi, 1ah
        or     ebx, ebx
        jg     SHORT loc_1_111DC
loc_1_111F6:
        push   dword ptr [edi + 58h]
        call   sub_1_E62B
        add    esp, 4
        mov    dword ptr [edi + 4ch], 0
loc_1_11208:
        mov    esi, dword ptr [edi + 6ch]
        or     esi, esi
        je     SHORT loc_1_11242
        mov    ebx, dword ptr [edi + 70h]
        or     ebx, ebx
        jle    SHORT loc_1_11230
loc_1_11216:
        push   0dbba0h
        push   dword ptr [esi + 16h]
        push   esi
        push   edi
        call   sub_1_EDDC
        add    esp, 10h
        dec    ebx
        add    esi, 1ah
        or     ebx, ebx
        jg     SHORT loc_1_11216
loc_1_11230:
        push   dword ptr [edi + 78h]
        call   sub_1_E62B
        add    esp, 4
        mov    dword ptr [edi + 6ch], 0
loc_1_11242:
        push   1
        db  068h, 038h, 0F1h, 000h, 000h
        push   5ah
        add    edi, 9ch
        push   edi
        call   sub_1_E782
        add    esp, 10h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
sub_1_11260:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        or     word ptr [eax + 0cch], 1000h
        push   1
        db  068h, 000h, 010h, 001h, 000h
        push   0ea60h
        add    eax, 0ach
        push   eax
        call   sub_1_E782
        leave
        ret
sub_1_11288:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edx, 0ffffffffh
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [eax]
        cmp    ebx, eax
        je     SHORT loc_1_112BB
loc_1_1129C:
        lea    edi, [ebx + 10h]
        mov    esi, dword ptr [ebp + 0ch]
        sub    edx, edx
        movsx  ecx, byte ptr [ebx + 0ah]
        repe cmpsb
        je     SHORT loc_1_112B1
        sbb    edx, edx
        sbb    edx, -1
loc_1_112B1:
        or     edx, edx
        jle    SHORT loc_1_112BB
        mov    ebx, dword ptr [ebx]
        cmp    ebx, eax
        jne    SHORT loc_1_1129C
loc_1_112BB:
        or     edx, edx
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], ebx
        sete   al
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
sub_1_112CC:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        movzx  eax, byte ptr [esi + 0bh]
        db  08Bh, 014h, 085h, 0B0h, 01Bh, 001h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [esi + 4], ecx
        mov    dword ptr [esi], eax
        mov    dword ptr [ecx], esi
        mov    dword ptr [eax + 4], esi
        movzx  eax, byte ptr [esi + 8]
        shl    eax, 5
        inc    dword ptr [eax + edx + 44h]
        test   byte ptr [esi + 25h], 40h
        je     SHORT loc_1_11303
        inc    dword ptr [eax + edx + 48h]
loc_1_11303:
        pop    esi
        leave
        ret
        db  08Bh, 0FFh
sub_1_11308:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [edx + 0bh]
        db  08Bh, 00Ch, 085h, 0B0h, 01Bh, 001h, 000h
        mov    esi, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [eax], esi
        movzx  eax, byte ptr [edx + 8]
        shl    eax, 5
        add    eax, ecx
        dec    dword ptr [eax + 44h]
        test   byte ptr [edx + 25h], 40h
        je     SHORT loc_1_11339
        dec    dword ptr [eax + 48h]
loc_1_11339:
        mov    eax, dword ptr [ecx + 34h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx + 34h], eax
        mov    eax, dword ptr [ecx + 38h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx + 38h], eax
        pop    esi
        leave
        ret
sub_1_1134C:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 8]
        or     ecx, ecx
        jne    SHORT loc_1_11360
        sub    eax, eax
        pop    esi
        leave
        ret
        db  090h
loc_1_11360:
        mov    esi, dword ptr [eax]
        push   ecx
        call   sub_1_1018C
        add    esp, 4
        mov    ecx, eax
        dec    eax
        or     ecx, ecx
        je     SHORT loc_1_1137B
loc_1_11372:
        mov    esi, dword ptr [esi]
        mov    ecx, eax
        dec    eax
        or     ecx, ecx
        jne    SHORT loc_1_11372
loc_1_1137B:
        push   esi
        call   sub_1_11308
        add    esp, 4
        mov    eax, esi
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
sub_1_1138C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [esi + 3ch]
        mov    dword ptr [esi + 34h], edi
        mov    dword ptr [esi + 38h], edi
        mov    al, byte ptr [esi]
        push   eax
        call   sub_1_E8E7
        add    esp, 4
        add    esi, 5ch
        push   esi
        call   sub_1_113C8
        add    esp, 4
        mov    esi, eax
        push   edi
        call   sub_1_113C8
        add    esp, 4
        add    eax, esi
        pop    edi
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
sub_1_113C8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi]
        mov    eax, edi
        mov    ebx, dword ptr [esi + 8]
        mov    dword ptr [esi + 4], esi
        mov    dword ptr [esi], esi
        mov    dword ptr [esi + 8], 0
        cmp    edi, esi
        je     SHORT loc_1_113F9
loc_1_113E8:
        mov    edi, dword ptr [edi]
        push   eax
        call   sub_1_E651
        add    esp, 4
        mov    eax, edi
        cmp    edi, esi
        jne    SHORT loc_1_113E8
loc_1_113F9:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_11400:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        lea    edx, [esi + 40h]
        inc    ecx
        mov    ecx, 2
loc_1_11410:
        lea    eax, [edx - 4]
        mov    dword ptr [edx], eax
        mov    dword ptr [eax], eax
        add    edx, 20h
        dec    ecx
        jne    SHORT loc_1_11410
        lea    eax, [esi + 3ch]
        mov    dword ptr [esi + 34h], eax
        mov    dword ptr [esi + 38h], eax
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_1142C:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 12h]
        push   eax
        push   dword ptr [ebp + 0ch]
        call   sub_1_115A0
        leave
        ret
sub_1_11440:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        movsx  eax, byte ptr [ebp + 0ch]
        db  08Bh, 004h, 085h, 0B0h, 01Bh, 001h, 000h
        mov    dword ptr [ebp - 14h], eax
        add    eax, 5ch
        mov    dword ptr [ebp - 8], eax
        mov    edx, dword ptr [eax]
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0ch], eax
        mov    edi, eax
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    ebx, ecx
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 12h]
        mov    dword ptr [ebp - 10h], ecx
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp - 14h]
        test   byte ptr [eax + 1ah], 4
        je     NEAR PTR loc_1_11594
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 4], 40000h
        jne    NEAR PTR loc_1_11594
        or     ecx, ecx
        je     NEAR PTR loc_1_11594
        cmp    word ptr [eax + 10h], 1bh
        jb     NEAR PTR loc_1_11594
        cmp    word ptr [eax + 16h], 1
        jne    NEAR PTR loc_1_11594
        mov    eax, dword ptr [eax + 8]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        mov    word ptr [eax], 0
        mov    edi, ecx
        sub    eax, eax
        mov    ecx, 6
        rep stosd
        stosw
        stosb
        cmp    edx, dword ptr [ebp - 8]
        je     NEAR PTR loc_1_1158D
        nop
loc_1_114E8:
        movsx  eax, byte ptr [edx + 0ah]
        cmp    eax, ebx
        jne    SHORT loc_1_11508
        lea    esi, [edx + 10h]
        mov    ecx, ebx
        shr    ecx, 2
        mov    edi, dword ptr [ebp - 0ch]
        repe cmpsd
        jne    SHORT loc_1_11506
        mov    ecx, ebx
        and    ecx, 3
        repe cmpsb
loc_1_11506:
        je     SHORT loc_1_11518
loc_1_11508:
        mov    edx, dword ptr [edx]
        cmp    edx, dword ptr [ebp - 8]
        jne    SHORT loc_1_114E8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_11518:
        mov    al, byte ptr [edx + 5bh]
        cmp    al, 1
        jle    SHORT loc_1_1158D
        movsx  eax, al
        lea    esi, [edx + 2ah]
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [edx + 22h]
        and    eax, 7fffffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 12h], eax
        or     eax, 40003h
        mov    dword ptr [ecx + 12h], eax
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, word ptr [eax + 10h]
        mov    ecx, dword ptr [ebp - 10h]
        lea    eax, [eax + ecx - 1]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 16h], eax
        mov    byte ptr [eax], 0
        cmp    byte ptr [ecx + 10h], 0
        jne    SHORT loc_1_11577
        cmp    byte ptr [ecx + 11h], 0
        jne    SHORT loc_1_11577
        mov    byte ptr [ecx + 10h], 3
        mov    byte ptr [ecx + 11h], 0bh
loc_1_11577:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        mov    word ptr [eax], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    word ptr [eax], 1
loc_1_1158D:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_11594:
        pop    edi
        mov    eax, 910h
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
sub_1_115A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    bl, byte ptr [ebp + 0ch]
        cmp    bl, 11h
        jge    NEAR PTR loc_1_118B8
        movsx  eax, bl
        db  08Bh, 014h, 085h, 0B0h, 01Bh, 001h, 000h
        cmp    edx, 0fffff000h
        je     NEAR PTR loc_1_118B8
        test   byte ptr [edx + 0cdh], 2
        je     SHORT loc_1_115E0
        mov    eax, 32h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_115E0:
        db  0F6h, 080h, 0B8h, 01Ch, 001h, 000h, 040h
        je     SHORT loc_1_115F4
        mov    eax, 32h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_115F4:
        test   byte ptr [edx + 1ah], 6
        jne    SHORT loc_1_11604
        mov    eax, 32h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_11604:
        mov    ecx, dword ptr [ebp + 8]
        cmp    word ptr [ecx + 16h], 1
        jle    SHORT loc_1_11618
        mov    eax, 7ch
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_11618:
        mov    ecx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ecx]
        or     edi, edi
        je     SHORT loc_1_11674
        cmp    byte ptr [edi], 0
        je     SHORT loc_1_11674
        db  0BEh, 0E0h, 039h, 001h, 000h
        db  0A1h, 000h, 03Ah, 001h, 000h
        mov    ecx, eax
        shr    ecx, 2
        repe cmpsd
        jne    SHORT loc_1_1164D
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb
        jne    SHORT loc_1_1164D
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        cmp    byte ptr [ecx + eax], 0
        je     SHORT loc_1_11674
loc_1_1164D:
        cmp    byte ptr [edx + 0e2h], 0
        je     SHORT loc_1_11668
        push   ebx
        push   dword ptr [ebp + 8]
        call   sub_1_11440
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_11668:
        pop    edi
        mov    eax, 910h
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_11674:
        lea    ecx, [edx + 3ch]
        mov    dword ptr [ebp - 1ch], ecx
        mov    dword ptr [ebp - 20h], 0
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ebp - 10h], ecx
        test   ecx, 40000000h
        je     SHORT loc_1_116CC
        cmp    ecx, 0c0000000h
        jne    SHORT loc_1_116A7
        mov    dword ptr [ebp - 10h], 40000000h
        lea    ecx, [edx + 5ch]
        mov    dword ptr [ebp - 1ch], ecx
loc_1_116A7:
        cmp    dword ptr [ebp - 10h], 40000000h
        jne    SHORT loc_1_116BC
        mov    dword ptr [ebp - 20h], 1
        jmp    SHORT loc_1_116EE
        db  08Dh, 049h, 000h
loc_1_116BC:
        cmp    dword ptr [ebp - 10h], -1
        je     SHORT loc_1_116EE
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_116CC:
        test   byte ptr [ebp - 0dh], 80h
        je     SHORT loc_1_116EE
        cmp    dword ptr [ebp - 10h], 80000000h
        je     SHORT loc_1_116E8
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_116E8:
        lea    ecx, [edx + 5ch]
        mov    dword ptr [ebp - 1ch], ecx
loc_1_116EE:
        cmp    dword ptr [ebp - 10h], 40000000h
        jne    SHORT loc_1_11708
        test   byte ptr [edx + 1ah], 4
        jne    SHORT loc_1_11708
        mov    eax, 32h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_11708:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 12h]
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        jne    SHORT loc_1_11720
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_1172D
        db  08Bh, 0FFh
loc_1_11720:
        mov    ecx, dword ptr [ebp + 8]
        movzx  ecx, word ptr [ecx + 10h]
        lea    esi, [ecx + eax]
        mov    dword ptr [ebp - 4], esi
loc_1_1172D:
        mov    ecx, dword ptr [ebp + 8]
        cmp    word ptr [ecx + 16h], 0
        mov    ebx, 10h
        je     SHORT loc_1_11741
        mov    ebx, 1ah
loc_1_11741:
        sub    eax, eax
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 18h], eax
        mov    ecx, dword ptr [ebp - 1ch]
        mov    edx, dword ptr [ecx + 10h]
        cmp    edx, eax
        je     NEAR PTR loc_1_117FC
        mov    ecx, dword ptr [ecx + 14h]
        mov    dword ptr [ebp - 24h], ecx
        cmp    ecx, eax
        jle    NEAR PTR loc_1_11883
loc_1_11765:
        mov    ecx, dword ptr [ebp - 10h]
        test   dword ptr [edx + 12h], ecx
        je     SHORT loc_1_117E6
        inc    dword ptr [ebp - 18h]
        mov    esi, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 8], esi
        jae    SHORT loc_1_117E6
        cmp    ebx, 1ah
        jne    SHORT loc_1_11790
        mov    edi, dword ptr [edx + 16h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    eax, ecx
        sub    dword ptr [ebp - 4], eax
loc_1_11790:
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0ch], ecx
        add    dword ptr [ebp - 8], ebx
        mov    esi, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 8], esi
        ja     SHORT loc_1_117E6
        mov    edi, ecx
        mov    esi, edx
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        cmp    dword ptr [ebp - 20h], 0
        je     SHORT loc_1_117C3
        mov    ecx, dword ptr [ebp - 0ch]
        and    dword ptr [ecx + 12h], 0bfffffffh
loc_1_117C3:
        or     eax, eax
        je     SHORT loc_1_117E3
        mov    ecx, dword ptr [ebp - 0ch]
        mov    esi, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 16h], esi
        mov    edi, esi
        mov    esi, dword ptr [edx + 16h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_1_117E3:
        inc    dword ptr [ebp - 14h]
loc_1_117E6:
        dec    dword ptr [ebp - 24h]
        add    edx, 1ah
        cmp    dword ptr [ebp - 24h], 0
        jg     NEAR PTR loc_1_11765
        jmp    NEAR PTR loc_1_11883
        db  090h
loc_1_117FC:
        mov    ecx, dword ptr [ebp - 1ch]
        mov    edx, dword ptr [ecx]
        cmp    ecx, edx
        je     SHORT loc_1_11883
loc_1_11805:
        mov    ecx, dword ptr [ebp - 10h]
        test   dword ptr [edx + 22h], ecx
        je     SHORT loc_1_1187C
        inc    dword ptr [ebp - 18h]
        mov    esi, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 8], esi
        jae    SHORT loc_1_1187C
        cmp    ebx, 1ah
        jne    SHORT loc_1_11824
        movsx  eax, byte ptr [edx + 5bh]
        sub    dword ptr [ebp - 4], eax
loc_1_11824:
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0ch], ecx
        add    dword ptr [ebp - 8], ebx
        mov    esi, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 8], esi
        ja     SHORT loc_1_1187E
        lea    esi, [edx + 10h]
        mov    edi, ecx
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        cmp    dword ptr [ebp - 20h], 0
        je     SHORT loc_1_11858
        mov    ecx, dword ptr [ebp - 0ch]
        and    dword ptr [ecx + 12h], 0bfffffffh
loc_1_11858:
        or     eax, eax
        je     SHORT loc_1_11879
        mov    ecx, dword ptr [ebp - 0ch]
        mov    esi, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 16h], esi
        lea    esi, [edx + 2ah]
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_1_11879:
        inc    dword ptr [ebp - 14h]
loc_1_1187C:
        mov    edx, dword ptr [edx]
loc_1_1187E:
        cmp    dword ptr [ebp - 1ch], edx
        jne    SHORT loc_1_11805
loc_1_11883:
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 8]
        mov    ax, word ptr [ebp - 18h]
        mov    word ptr [ecx], ax
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0ch]
        mov    ax, word ptr [ebp - 14h]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp - 18h]
        cmp    dword ptr [ebp - 14h], eax
        je     SHORT loc_1_118B0
        mov    eax, 0eah
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_118B0:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_118B8:
        pop    edi
        mov    eax, 32h
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        test   edx, 80070000h
        je     SHORT loc_1_118DC
        mov    eax, 3
        pop    esi
        leave
        ret
        db  090h
loc_1_118DC:
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [ebp + 10h], 0
        jne    SHORT loc_1_118FC
        or     dword ptr [esi + 18h], edx
        mov    al, byte ptr [esi]
        push   eax
        push   edx
        push   edx
        call   sub_1_E8CF
        add    esp, 0ch
        sub    eax, eax
        pop    esi
        leave
        ret
        db  08Bh, 0FFh
loc_1_118FC:
        mov    eax, edx
        mov    ecx, dword ptr [esi + 18h]
        and    eax, ecx
        cmp    eax, edx
        je     SHORT loc_1_11910
        mov    eax, 3
        pop    esi
        leave
        ret
        db  090h
loc_1_11910:
        mov    eax, edx
        not    eax
        and    eax, ecx
        mov    dword ptr [esi + 18h], eax
        mov    al, byte ptr [esi]
        push   eax
        push   0
        push   edx
        call   sub_1_E8CF
        add    esp, 0ch
        sub    eax, eax
        pop    esi
        leave
        ret
sub_1_1192C:
        push   edi
        db  0A1h, 028h, 028h, 001h, 000h
        db  0A3h, 07Ch, 039h, 001h, 000h
        db  0A1h, 0C4h, 031h, 001h, 000h
        db  0A3h, 080h, 039h, 001h, 000h
        call   sub_1_E598
        or     al, al
        jne    SHORT loc_1_11950
        sub    eax, eax
        pop    edi
        ret
        db  08Bh, 0FFh
loc_1_11950:
        db  08Bh, 015h, 020h, 031h, 001h, 000h
        cmp    byte ptr [edx + 45h], 0
        je     SHORT loc_1_11971
        db  068h, 0BCh, 039h, 001h, 000h
        push   68h
        call   sub_1_E5AB
        add    esp, 8
        db  08Bh, 015h, 020h, 031h, 001h, 000h
loc_1_11971:
        db  0BFh, 0E0h, 039h, 001h, 000h
        mov    eax, 20202020h
        stosd
        stosd
        stosd
        stosd
        mov    edi, dword ptr [edx + 8]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [ecx - 1]
        db  0A3h, 000h, 03Ah, 001h, 000h
        push   eax
        push   dword ptr [edx + 8]
        db  068h, 0E0h, 039h, 001h, 000h
        call   sub_1_E4D8
        add    esp, 0ch
        db  0A1h, 000h, 03Ah, 001h, 000h
        inc    eax
        push   eax
        db  0A1h, 020h, 031h, 001h, 000h
        push   dword ptr [eax + 8]
        db  068h, 0F0h, 039h, 001h, 000h
        call   sub_1_E4D8
        add    esp, 0ch
        sub    eax, eax
        db  0B9h, 048h, 023h, 001h, 000h
loc_1_119C9:
        add    eax, eax
        add    eax, dword ptr [ecx]
        add    ecx, 4
        db  081h, 0F9h, 054h, 023h, 001h, 000h
        jb     SHORT loc_1_119C9
        db  0A3h, 018h, 03Ah, 001h, 000h
        or     eax, eax
        jne    SHORT loc_1_119E7
        inc    eax
        db  0A3h, 018h, 03Ah, 001h, 000h
loc_1_119E7:
        call   sub_1_1075C
        call   sub_1_E5C0
        mov    eax, 1
        pop    edi
        ret
sub_1_119F8:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 040h, 07Ch, 074h, 0FDh, 000h, 000h
        db  0C7h, 080h, 080h, 000h, 000h, 000h, 06Ch, 0EBh, 000h, 000h
        db  0C7h, 080h, 084h, 000h, 000h, 000h, 010h, 0F0h, 000h, 000h
        mov    byte ptr [eax + 0cfh], 5ch
        mov    byte ptr [eax + 0ceh], 5ch
        leave
        ret
        db  08Dh, 049h, 000h
sub_1_11A2C:
        call   sub_1_E5EE
        mov    eax, 1
        ret
        db  090h, 018h, 002h, 000h, 000h, 085h, 002h, 000h, 000h, 0BCh, 024h, 000h, 000h
loc_1_11A44:
        add    dword ptr [edx], eax
        add    ecx, dword ptr [eax]
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 0F0h, 0FFh, 0FFh, 000h, 0F0h, 0FFh, 0FFh, 000h, 0F0h, 0FFh, 0FFh
        db  000h, 0F0h, 0FFh, 0FFh, 000h, 0F0h, 0FFh, 0FFh, 000h, 0F0h, 0FFh, 0FFh, 000h, 0F0h, 0FFh, 0FFh
        db  000h, 0F0h, 0FFh, 0FFh, 000h, 0F0h, 0FFh, 0FFh, 000h, 0F0h, 0FFh, 0FFh, 000h, 0F0h, 0FFh, 0FFh
        db  000h, 0F0h, 0FFh, 0FFh, 000h, 0F0h, 0FFh, 0FFh, 000h, 0F0h, 0FFh, 0FFh, 000h, 0F0h, 0FFh, 0FFh
        db  000h, 0F0h, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0F0h, 0FFh, 0FFh
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
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
loc_1_11BA8:
        movsb
        sbb    eax, dword ptr [ecx]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0F0h, 0FFh, 0FFh, 000h, 0F0h, 0FFh, 0FFh, 000h, 0F0h, 0FFh, 0FFh, 000h, 0F0h, 0FFh, 0FFh, 000h
        db  0F0h, 0FFh, 0FFh, 000h, 0F0h, 0FFh, 0FFh, 000h, 0F0h, 0FFh, 0FFh, 000h, 0F0h, 0FFh, 0FFh, 000h
        db  0F0h, 0FFh, 0FFh, 000h, 0F0h, 0FFh, 0FFh, 000h, 0F0h, 0FFh, 0FFh, 000h, 0F0h, 0FFh, 0FFh, 000h
        db  0F0h, 0FFh, 0FFh, 000h, 0F0h, 0FFh, 0FFh, 000h, 0F0h, 0FFh, 0FFh, 000h, 0F0h, 0FFh, 0FFh, 000h
        db  0F0h, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 0F0h, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_11C0C:
        xor    byte ptr [ebp], dh
        db  000h
loc_1_11C10:
        push   ds
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0C8h
loc_1_11C19:
        sbb    eax, dword ptr [ecx]
        add    byte ptr [eax], al
        db  0F0h, 0FFh, 0FFh, 000h, 0F0h, 0FFh, 0FFh, 000h, 0F0h, 0FFh, 0FFh, 000h, 0F0h, 0FFh, 0FFh, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_11C3C:
        jl     SHORT loc_1_11C7E
        add    byte ptr [eax], al
        sub    al, 0c6h
        add    byte ptr [eax], al
        sub    cl, cl
        add    byte ptr [eax], al
        loopne loc_1_11C19
        add    byte ptr [eax], al
        in     al, 0d3h
        add    byte ptr [eax], al
        je     SHORT loc_1_11C92
        add    byte ptr [eax], al
        cwde
        rol    dword ptr [eax], 1
        db  000h, 074h, 040h, 000h
        db  000h, 0B0h, 0D6h, 000h, 000h, 0ECh
        fadd   dword ptr [eax]
        db  000h, 074h, 040h, 000h
        db  000h, 074h, 040h, 000h
        db  000h, 074h, 040h, 000h
        db  000h, 074h, 040h, 000h
        db  000h, 074h, 040h, 000h
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_11C7E:
        add    byte ptr [eax], al
        pop    eax
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [0], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 2], bl
loc_1_11C92:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        pop    eax
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 2], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_11CB8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_11CCA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_11CDC:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        db  0C8h, 01Fh, 000h, 000h, 084h, 020h, 000h, 000h, 024h, 005h, 000h, 000h, 0C8h, 006h, 000h, 000h
        db  0CCh, 00Ah, 000h, 000h, 0BCh, 025h, 000h, 000h, 058h, 027h, 000h, 000h, 07Ch, 028h, 000h, 000h
        db  0B0h, 029h, 000h, 000h, 07Ch, 02Ah, 000h, 000h, 078h, 00Bh, 000h, 000h, 098h, 010h, 000h, 000h
        db  0FCh, 021h, 000h, 000h, 09Ch, 022h, 000h, 000h, 00Ah, 000h, 000h, 000h, 0C8h, 006h, 000h, 000h
        db  040h, 021h, 000h, 000h, 04Dh, 02Bh, 000h, 000h, 044h, 026h, 000h, 000h, 014h, 00Ch, 000h, 000h
        db  000h, 011h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 0C8h, 00Eh, 000h, 000h, 068h, 00Dh, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 0F4h, 012h, 000h, 000h, 0E8h, 015h, 000h, 000h, 0DCh, 017h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 010h, 027h, 000h, 000h, 0B0h, 026h, 000h, 000h
        db  0D0h, 022h, 000h, 000h, 088h, 03Ah, 000h, 000h, 028h, 03Ch, 000h, 000h, 068h, 02Ch, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 02Ch, 02Ch, 000h, 000h
        db  008h, 012h, 000h, 000h, 0A4h, 008h, 000h, 000h, 0ACh, 00Ch, 000h, 000h, 08Ch, 011h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 070h, 03Ah, 000h, 000h, 028h, 03Ch, 000h, 000h
        db  0F8h, 0D0h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  094h, 031h, 000h, 000h, 02Ch, 033h, 000h, 000h, 030h, 080h, 000h, 000h, 0ECh, 030h, 000h, 000h
        db  060h, 003h, 000h, 000h, 078h, 032h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  0C4h, 02Bh, 000h, 000h, 008h, 02Dh, 000h, 000h, 008h, 02Dh, 000h, 000h, 008h, 02Dh, 000h, 000h
        db  090h, 02Eh, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  014h, 038h, 000h, 000h, 058h, 039h, 000h, 000h, 0E4h, 039h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
loc_1_120E0:
        db  0E8h, 003h, 000h, 000h, 0DCh
        db  005h, 000h, 000h
loc_1_120E8:
        add    byte ptr fs:[eax], al
        db  000h
loc_1_120EC:
        add    al, 0
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
loc_1_120F4:
        inc    ecx
        cmp    al, byte ptr [eax]
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        db  000h
loc_1_120FC:
        dec    esp
        push   eax
        push   esp
        xor    dword ptr [edx], edi
        add    byte ptr [eax], al
        add    byte ptr [43000000h], al
        dec    edi
        dec    ebp
        dec    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
loc_1_12114:
        dec    ecx
        push   eax
        inc    ebx
        db  000h, 0F4h
        and    byte ptr [ecx], al
        db  000h, 0FCh
        and    byte ptr [ecx], al
        db  000h, 008h
        and    dword ptr [ecx], eax
        db  000h, 014h, 021h
        add    dword ptr [eax], eax
        add    al, 0
        add    byte ptr [eax], al
        inc    esi
        inc    ecx
        push   esp
        add    byte ptr [eax + edx*2 + 52h], bl
        dec    ecx
        dec    esi
        push   esp
        pop    esp
loc_1_12137:
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
loc_1_12157:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1215B:
        add    byte ptr [ebx + 65h], dl
        db  072h, 076h
        jb     NEAR PTR loc_1_1219D
        and    byte ptr [ebp + 6eh], dl
        db  073h, 075h
        jo     SHORT loc_1_121DA
        outsd
        jb     SHORT loc_1_121E1
        and    byte ptr fs:[edx + esi + 20h], dl
        arpl   word ptr [edi + 6dh], bp
        insd
        popad
        outsb
        and    byte ptr fs:[6e5c78h], ah
loc_1_12180:
        push   edi
        jb     SHORT loc_1_121CF
        db  065h, 068h, 000h, 000h, 000h, 088h
        dec    esp
        add    byte ptr [eax], al
        and    byte ptr [ecx], 1
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh
loc_1_12198:
        push   edi
        jb     SHORT loc_1_121E7
        db  068h, 000h
loc_1_1219D:
        add    byte ptr [eax], al
        db  000h, 0E0h
        dec    edi
        add    byte ptr [eax], al
        cwde
        and    dword ptr [ecx], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh
loc_1_121B0:
        db  07Ah, 057h
        db  072h, 04Ch
        db  068h, 000h, 000h, 000h
loc_1_121B8:
        in     al, dx
        add    byte ptr gs:[eax], al
        mov    al, 21h
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
loc_1_121C8:
        push   edi
        db  072h, 04Ch
        db  065h, 068h, 000h, 000h
loc_1_121CF:
        db  000h, 0DCh
        add    byte ptr [eax], al
        enter  121h, 0
        add    byte ptr [eax], al
loc_1_121DA:
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
loc_1_121E0:
        push   edi
loc_1_121E1:
        push   edi
        jae    SHORT loc_1_12238
        push   eax
        add    byte ptr [eax], al
loc_1_121E7:
        db  000h, 088h, 067h, 000h, 000h, 0E0h
        and    dword ptr [ecx], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh
loc_1_121F8:
        push   edi
        add    byte ptr [eax], al
        db  000h
loc_1_121FC:
        db  0B8h, 06Ch, 000h, 000h, 0F8h
        and    dword ptr [ecx], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh
loc_1_1220C:
        push   edi
        add    byte ptr [eax], al
        db  000h
loc_1_12210:
        shr    byte ptr [eax + eax], 1
        or     al, 22h
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
sub_1_12220:
        push   edi
        add    byte ptr [eax], al
        db  000h
loc_1_12224:
        db  0E8h, 06Ch, 000h, 000h, 020h
        and    al, byte ptr [ecx]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh
loc_1_12234:
        jb     SHORT loc_1_12282
        add    byte ptr [eax], al
loc_1_12238:
        add    byte ptr [ebp], ch
        db  000h, 034h, 022h
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
loc_1_12248:
        jp     SHORT loc_1_1224A
loc_1_1224A:
        add    byte ptr [eax], al
loc_1_1224C:
        dec    ebx
        or     byte ptr [eax], al
        add    byte ptr [ebx], bh
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [edx + 8], bl
        add    byte ptr [eax], al
        or     byte ptr ss:[eax], al
        add    byte ptr [edi], dh
        add    byte ptr [eax], al
        add    byte ptr [ecx + 8], dh
        add    byte ptr [eax], al
        push   4
        add    byte ptr [eax], al
        jne    SHORT loc_1_12276
        add    byte ptr [eax], al
        or     al, 7
        add    byte ptr [eax], al
        db  077h, 008h
loc_1_12276:
        add    byte ptr [eax], al
        mov    al, 4
        add    byte ptr [eax], al
        and    eax, 0b000009h
        pop    es
loc_1_12282:
        add    byte ptr [eax], al
        db  073h, 008h
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        or     byte ptr [bx + si], al
        add    byte ptr [76000007h], al
        or     byte ptr [eax], al
        add    byte ptr [esi], al
        pop    es
        add    byte ptr [eax], al
        js     SHORT loc_1_122A6
        add    byte ptr [eax], al
loc_1_122A0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_122A6:
        add    byte ptr [eax], al
loc_1_122A8:
        pop    ds
        add    byte ptr [eax], al
        add    byte ptr [ebx], bh
        add    byte ptr [eax], al
        add    byte ptr [edx], bl
        add    byte ptr [eax], al
        js     SHORT loc_1_122B6
loc_1_122B6:
        add    byte ptr [eax], al
        xchg   edi, eax
        add    byte ptr [eax], al
        add    byte ptr [ebp - 2c000000h], dh
        add    byte ptr [eax], al
        add    bl, dh
        add    byte ptr [eax], al
        add    byte ptr [ecx], dl
        add    dword ptr [eax], eax
        add    byte ptr [eax], dh
        add    dword ptr [eax], eax
        add    byte ptr [esi + 1], cl
        add    byte ptr [eax], al
loc_1_122D4:
        pop    esp
        push   eax
        push   edx
        dec    ecx
        dec    esi
        push   esp
        inc    ebp
        push   edx
        and    al, 0
        add    byte ptr [eax], al
loc_1_122E0:
        pop    esp
        push   eax
        push   edx
        dec    ecx
        dec    esi
        push   esp
        inc    ebp
        push   edx
        and    al, 0
        add    byte ptr [eax], al
loc_1_122EC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_122F4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_122F8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_122FC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12300:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12304:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12308:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1230C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12310:
        add    byte ptr [eax], al
        add    byte ptr [eax], 0
loc_1_12315:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12319:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1231D:
        add    al, dh
        db  0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 021h, 06Eh, 000h, 000h, 060h, 0EAh
        db  000h, 000h, 000h, 000h, 000h
loc_1_12334:
        mov    ah, 5
        add    byte ptr [eax], al
        push   0b400000bh
        add    eax, 800000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12348:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_1_12354:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_1_12358:
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
loc_1_1237C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_12384:
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
loc_1_1239C:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_123A0:
        add    byte ptr [eax], al
loc_1_123A2:
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
loc_1_1243A:
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
loc_1_12492:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1249A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_124A2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_124A6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_124AA:
        and    byte ptr [eax], al
loc_1_124AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_124B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [ebx]
        add    bh, bh
        db  0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0AAh, 024h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_124F1:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_124F5:
        add    byte ptr [eax], al
loc_1_124F7:
        or     eax, 45004600h
        add    byte ptr [ebx + 52005100h], dl
        add    byte ptr [ebx], dl
        pop    ebx
        add    byte ptr [ebx], al
        add    byte ptr [eax + eax], al
        add    dword ptr [eax], eax
        add    al, byte ptr [eax]
        push   es
        add    byte ptr [eax], cl
        add    byte ptr [ecx], cl
        add    byte ptr [edx], cl
        add    byte ptr [ebp], bh
        aad    0
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
        db  000h, 088h, 021h, 001h, 000h, 0A0h
        and    dword ptr [ecx], eax
        db  000h, 0B8h, 021h, 001h, 000h, 0D0h
        and    dword ptr [ecx], eax
        db  000h, 0E8h
        and    dword ptr [ecx], eax
        db  000h, 0FCh
        and    dword ptr [ecx], eax
        db  000h, 010h
        and    al, byte ptr [ecx]
        db  000h, 024h, 022h
        add    dword ptr [eax], eax
        cmp    byte ptr [edx], ah
        add    dword ptr [eax], eax
        hlt
        xor    al, byte ptr [ecx]
        db  000h, 008h
        xor    eax, dword ptr [ecx]
        db  000h, 020h
        xor    eax, dword ptr [ecx]
        db  000h, 038h
        xor    eax, dword ptr [ecx]
        db  000h, 050h, 033h
        add    dword ptr [eax], eax
        xor    eax, dword ptr fs:[ecx]
        db  000h, 07Ch, 033h, 001h
        db  000h, 094h, 033h, 001h, 000h, 0A8h, 033h
        add    dword ptr [eax], eax
        sal    byte ptr [ebx], 1
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
        add    byte ptr [ebx], dl
        add    byte ptr [eax], al
        db  000h
_LTEXT ends
        end
