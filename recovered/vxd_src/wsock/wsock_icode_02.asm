; wsock_icode_02.asm -- WSOCK object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

        db  0C7h, 005h, 078h, 022h, 000h, 000h, 074h, 022h, 000h, 000h, 0C7h, 005h, 074h, 022h, 000h, 000h
        db  074h, 022h, 000h, 000h, 0C7h, 005h, 0ECh, 021h, 000h, 000h, 0E8h, 021h, 000h, 000h, 0C7h, 005h
        db  0E8h, 021h, 000h, 000h, 0E8h, 021h, 000h, 000h, 0C7h, 005h, 080h, 022h, 000h, 000h, 07Ch, 022h
        db  000h, 000h, 0C7h, 005h, 07Ch, 022h, 000h, 000h, 07Ch, 022h, 000h, 000h, 0C7h, 005h, 070h, 022h
        db  000h, 000h, 000h, 000h, 000h, 000h, 068h, 000h, 020h, 000h, 000h, 0E8h, 0D8h, 00Bh, 0FEh, 0FFh
        db  083h, 0C4h, 004h, 0A3h, 0E4h, 021h, 000h, 000h, 00Bh, 0C0h, 075h, 004h, 02Bh, 0C0h, 0EBh, 01Fh
        db  0A3h, 0E4h, 021h, 000h, 000h, 06Ah, 01Ch, 068h, 000h, 020h, 000h, 000h, 050h, 0E8h, 05Eh, 00Bh
        db  0FEh, 0FFh, 083h, 0C4h, 00Ch, 0E8h, 0C2h, 00Bh, 0FEh, 0FFh, 0B8h, 001h, 000h, 000h, 000h, 0C3h
sub_2_0080:
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [esp + 18h], 1
        je     SHORT loc_2_0092
        sub    eax, eax
        jmp    NEAR PTR loc_2_012C
loc_2_0092:
        sub    edx, edx
        db  0B9h, 0F0h, 021h, 000h, 000h
        db  08Bh, 02Dh, 084h, 022h, 000h, 000h
        cmp    ebp, edx
        mov    eax, dword ptr [esp + 14h]
        je     SHORT loc_2_00F5
loc_2_00A7:
        or     eax, eax
        je     SHORT loc_2_00B3
        cmp    dword ptr [ecx], eax
        je     SHORT loc_2_00ED
        or     eax, eax
        jne    SHORT loc_2_00E3
loc_2_00B3:
        mov    esi, dword ptr [ecx + 0ch]
        mov    edi, dword ptr [esp + 20h]
loc_2_00BA:
        mov    bl, byte ptr [esi]
        cmp    bl, byte ptr [edi]
        jne    SHORT loc_2_00DA
        or     bl, bl
        je     SHORT loc_2_00D6
        mov    bl, byte ptr [esi + 1]
        cmp    bl, byte ptr [edi + 1]
        jne    SHORT loc_2_00DA
        add    esi, 2
        add    edi, 2
        or     bl, bl
        jne    SHORT loc_2_00BA
loc_2_00D6:
        sub    esi, esi
        jmp    SHORT loc_2_00DF
loc_2_00DA:
        sbb    esi, esi
        sbb    esi, -1
loc_2_00DF:
        or     esi, esi
        je     SHORT loc_2_00F1
loc_2_00E3:
        inc    edx
        add    ecx, 10h
        cmp    ebp, edx
        ja     SHORT loc_2_00A7
        jmp    SHORT loc_2_00F5
loc_2_00ED:
        sub    eax, eax
        jmp    SHORT loc_2_012C
loc_2_00F1:
        sub    eax, eax
        jmp    SHORT loc_2_012C
loc_2_00F5:
        cmp    ebp, 8
        jb     SHORT loc_2_00FE
        sub    eax, eax
        jmp    SHORT loc_2_012C
loc_2_00FE:
        mov    ecx, ebp
        shl    ecx, 4
        db  081h, 0C1h, 0F0h, 021h, 000h, 000h
        inc    ebp
        db  089h, 02Dh, 084h, 022h, 000h, 000h
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, 1
loc_2_012C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_2_0131:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0BFh, 0F0h, 021h, 000h, 000h
        sub    ebp, ebp
        db  08Bh, 00Dh, 084h, 022h, 000h, 000h
        cmp    ecx, ebp
        je     SHORT loc_2_0190
        mov    eax, dword ptr [esp + 14h]
loc_2_014A:
        or     eax, eax
        je     SHORT loc_2_0156
        cmp    dword ptr [edi], eax
        je     SHORT loc_2_018E
        or     eax, eax
        jne    SHORT loc_2_0186
loc_2_0156:
        mov    edx, dword ptr [edi + 0ch]
        mov    esi, dword ptr [esp + 18h]
loc_2_015D:
        mov    bl, byte ptr [edx]
        cmp    bl, byte ptr [esi]
        jne    SHORT loc_2_017D
        or     bl, bl
        je     SHORT loc_2_0179
        mov    bl, byte ptr [edx + 1]
        cmp    bl, byte ptr [esi + 1]
        jne    SHORT loc_2_017D
        add    edx, 2
        add    esi, 2
        or     bl, bl
        jne    SHORT loc_2_015D
loc_2_0179:
        sub    edx, edx
        jmp    SHORT loc_2_0182
loc_2_017D:
        sbb    edx, edx
        sbb    edx, -1
loc_2_0182:
        or     edx, edx
        je     SHORT loc_2_018E
loc_2_0186:
        add    edi, 10h
        inc    ebp
        cmp    ecx, ebp
        ja     SHORT loc_2_014A
loc_2_018E:
        cmp    ecx, ebp
loc_2_0190:
        jbe    SHORT loc_2_01A6
        dec    ecx
        db  089h, 00Dh, 084h, 022h, 000h, 000h
        sub    ecx, ebp
        shl    ecx, 4
        lea    esi, [edi + 10h]
        shr    ecx, 2
        rep movsd
loc_2_01A6:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h
sub_2_01AC:
        db  0BFh, 090h, 022h, 000h, 000h
        db  0B9h, 090h, 022h, 000h, 000h
        sub    ecx, edi
        shr    ecx, 2
        xor    eax, eax
        cld
        rep stosd
        db  0B8h, 038h, 00Dh, 000h, 000h
        mov    dl, byte ptr [eax]
        or     dl, dl
        jne    SHORT loc_2_022A
        inc    byte ptr [eax]
        db  0CDh, 020h, 000h, 000h, 001h, 000h
        cmp    ax, 400h
        jb     SHORT loc_2_022A
        db  0CDh, 020h, 000h, 000h, 027h, 000h
        jb     SHORT loc_2_022A
        db  0CDh, 020h, 000h, 000h, 02Ah, 000h
        jb     SHORT loc_2_022A
        db  0CDh, 020h, 000h, 000h, 017h, 000h
        jb     SHORT loc_2_022A
        db  0CDh, 020h, 000h, 000h, 040h, 000h
        jb     SHORT loc_2_022A
        push   0
        push   0ch
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        add    esp, 8
        db  0A3h, 08Ch, 022h, 000h, 000h
        or     eax, eax
        je     SHORT loc_2_022A
        call   0
        or     eax, eax
        je     SHORT loc_2_0219
        ret
loc_2_0219:
        push   0
        db  0FFh, 035h, 08Ch, 022h, 000h, 000h
        db  0CDh, 020h, 015h, 001h, 001h, 000h
        add    esp, 8
loc_2_022A:
        stc
        ret
loc_2_022C:
        db  0FEh, 005h, 090h, 004h, 000h, 000h
        call   sub_2_01AC
        jb     SHORT loc_2_0244
        call   sub_2_02AB
        or     eax, eax
        je     SHORT loc_2_0244
        clc
        ret
loc_2_0244:
        stc
        ret
        db  000h, 000h
sub_2_0248:
        push   ebp
        push   esi
        push   edi
        push   ebx
        db  0CDh, 020h, 0B6h, 000h, 001h, 000h
        mov    eax, edx
        pop    ebx
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_0259:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 1
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        sbb    eax, eax
        pop    ebx
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_0280:
        push   edi
        mov    edx, dword ptr [esp + 8]
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [edx + ecx - 1]
        cmp    eax, edx
        je     SHORT loc_2_02A7
        mov    cl, byte ptr [esp + 0ch]
loc_2_029E:
        cmp    byte ptr [eax], cl
        je     SHORT loc_2_02A9
        dec    eax
        cmp    eax, edx
        jne    SHORT loc_2_029E
loc_2_02A7:
        sub    eax, eax
loc_2_02A9:
        pop    edi
        ret
sub_2_02AB:
        push   ebp
        mov    ebp, esp
        sub    esp, 350h
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 34h]
        mov    dword ptr [ebp - 44h], eax
        db  0C7h, 045h, 0C0h, 0B0h, 021h, 000h, 000h
        mov    dword ptr [ebp - 3ch], 80000002h
        push   dword ptr [ebp - 44h]
        push   dword ptr [ebp - 40h]
        push   dword ptr [ebp - 3ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_2_02EE
        sub    eax, eax
        jmp    NEAR PTR loc_2_0489
loc_2_02EE:
        call   sub_2_0248
        or     eax, eax
        jne    SHORT loc_2_02FE
        sub    eax, eax
        jmp    NEAR PTR loc_2_0489
loc_2_02FE:
        mov    edi, eax
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebp - 350h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        push   5ch
        lea    eax, [ebp - 350h]
        push   eax
        call   sub_2_0280
        add    esp, 8
        mov    byte ptr [eax], 0
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 0ch], 104h
        mov    dword ptr [ebp - 8], 104h
loc_2_034C:
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 30h], eax
        lea    eax, [ebp - 24ch]
        mov    dword ptr [ebp - 2ch], eax
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 28h], eax
        mov    dword ptr [ebp - 24h], 0
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 20h], eax
        lea    eax, [ebp - 148h]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 34h]
        mov    dword ptr [ebp - 14h], eax
        push   dword ptr [ebp - 30h]
        push   dword ptr [ebp - 2ch]
        push   dword ptr [ebp - 28h]
        push   dword ptr [ebp - 24h]
        push   dword ptr [ebp - 20h]
        push   dword ptr [ebp - 1ch]
        push   dword ptr [ebp - 18h]
        push   dword ptr [ebp - 14h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 050h, 001h, 001h, 000h
        add    esp, 20h
        mov    ebx, eax
        or     ebx, ebx
        jne    NEAR PTR loc_2_0444
        cmp    dword ptr [ebp - 10h], 1
        jne    NEAR PTR loc_2_044E
        lea    edi, [ebp - 350h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebp - 148h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  0BEh, 0E0h, 021h, 000h, 000h
        lea    edi, [ebp - 148h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        movsw
        lea    edi, [ebp - 24ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        lea    edi, [ebp - 148h]
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
        lea    eax, [ebp - 148h]
        push   eax
        call   sub_2_0259
        add    esp, 4
        jmp    SHORT loc_2_044E
loc_2_0444:
        cmp    ebx, 0eah
        jne    SHORT loc_2_044E
        sub    ebx, ebx
loc_2_044E:
        mov    dword ptr [ebp - 0ch], 104h
        mov    dword ptr [ebp - 8], 104h
        inc    dword ptr [ebp - 4]
        or     ebx, ebx
        je     NEAR PTR loc_2_034C
        mov    eax, dword ptr [ebp - 34h]
        mov    dword ptr [ebp - 38h], eax
        push   dword ptr [ebp - 38h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        db  083h, 03Dh, 084h, 022h, 000h, 000h, 001h
        sbb    eax, eax
        inc    eax
loc_2_0489:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_0490:
        and    edx, dword ptr [edi + 53h]
        dec    edi
_ITEXT ends
        end
