; serwave_pcode_02.asm -- SERWAVE object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_2_0000:
        mov    eax, 4000000h
        ret    10h
loc_2_0008:
        sub    esp, 40h
        push   esi
        push   edi
        push   1
        push   33dh
        call   sub_2_058C
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_2_0027
        jmp    NEAR PTR loc_2_00B6
loc_2_0027:
        mov    eax, dword ptr [esp + 4ch]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [esp + 50h]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [esp + 54h]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [esp + 58h]
        mov    dword ptr [esi + 1ch], eax
        lea    eax, [esi + 2ch]
        mov    edi, dword ptr [esi]
        push   0
        push   eax
        push   0c0000005h
        push   edi
        call   sub_2_0584
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_2_00AB
        cmp    dword ptr [esi + 5ch], 0
        je     SHORT loc_2_00AB
        cmp    dword ptr [esi + 74h], 0
        je     SHORT loc_2_00AB
        cmp    dword ptr [esi + 44h], 0
        je     SHORT loc_2_00AB
        lea    eax, [esi + 0b4h]
        push   eax
        push   edi
        call   sub_2_0574
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_2_0084
        jmp    SHORT loc_2_00AB
loc_2_0084:
        lea    eax, [esp + 8]
        push   eax
        push   dword ptr [esi]
        call   sub_2_057C
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_2_0099
        jmp    SHORT loc_2_00AB
loc_2_0099:
        mov    eax, dword ptr [esp + 34h]
        mov    dword ptr [esi + 0ch], eax
        mov    eax, dword ptr [esp + 38h]
        mov    dword ptr [esi + 10h], eax
        mov    eax, esi
        jmp    SHORT loc_2_00B8
loc_2_00AB:
        push   0
        push   esi
        call   sub_2_0594
        add    esp, 8
loc_2_00B6:
        sub    eax, eax
loc_2_00B8:
        pop    edi
        pop    esi
        add    esp, 40h
        ret    10h
loc_2_00C0:
        push   0
        push   dword ptr [esp + 8]
        call   sub_2_0594
        add    esp, 8
        mov    eax, 0ffffffffh
        ret    10h
sub_2_00D6:
        sub    esp, 30h
        push   esi
        lea    eax, [esp + 4]
        mov    esi, dword ptr [esp + 38h]
        push   eax
        push   dword ptr [esi]
        call   sub_2_0574
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_2_00F8
        mov    eax, 2
        jmp    SHORT loc_2_0147
loc_2_00F8:
        mov    eax, dword ptr [esp + 3ch]
        mov    dword ptr [esp + 8], eax
        mov    dword ptr [esp + 0ch], 4009h
        mov    eax, dword ptr [esi + 10h]
        sub    eax, 80h
        mov    dword ptr [esp + 10h], eax
        mov    dword ptr [esp + 14h], 40h
        mov    byte ptr [esp + 1ah], 8
        mov    byte ptr [esp + 1ch], 0
        mov    byte ptr [esp + 1bh], 0
        lea    eax, [esp + 4]
        push   5fh
        push   eax
        push   dword ptr [esi]
        call   sub_2_056C
        add    esp, 0ch
        or     eax, eax
        mov    eax, 2
        je     SHORT loc_2_0147
        sub    eax, eax
loc_2_0147:
        pop    esi
        add    esp, 30h
        ret    10h
        push   ebp
        mov    ebp, esp
        push   ecx
        pushfd
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 00Eh, 000h, 02Ah, 000h
        popfd
        pop    ecx
        pop    ebp
        ret
sub_2_0160:
        sub    esp, 1ach
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 1b8h]
        or     eax, eax
        je     SHORT loc_2_0198
        cmp    eax, 1
        je     SHORT loc_2_019F
        cmp    eax, 2
        je     NEAR PTR loc_2_03D5
        cmp    eax, 3
        je     NEAR PTR loc_2_03D9
        cmp    eax, 4
        je     NEAR PTR loc_2_03D5
        jmp    NEAR PTR loc_2_03D9
loc_2_0198:
        sub    eax, eax
        jmp    NEAR PTR loc_2_03DE
loc_2_019F:
        db  0FFh, 035h, 048h, 000h, 000h, 000h
        lea    eax, [esp + 18h]
        db  068h, 04Ch, 000h, 000h, 000h
        push   eax
        call   sub_2_059C
        add    esp, 0ch
        db  0A1h, 048h, 000h, 000h, 000h
        inc    eax
        db  0A3h, 048h, 000h, 000h, 000h
        lea    edi, [esp + 14h]
        push   1
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        lea    eax, [esp + 18h]
        push   ecx
        push   eax
        push   1
        db  068h, 064h, 000h, 000h, 000h
        mov    esi, dword ptr [esp + 1d4h]
        push   0
        push   esi
        call   sub_2_05BC
        add    esp, 1ch
        push   1
        lea    eax, [esp + 0b8h]
        push   100h
        push   eax
        push   0
        push   esi
        call   sub_2_05AC
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_2_0218
        sub    eax, eax
        jmp    NEAR PTR loc_2_03DE
loc_2_0218:
        lea    edi, [esp + 0b4h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        cmp    ecx, 4
        jae    SHORT loc_2_0237
        sub    eax, eax
        jmp    NEAR PTR loc_2_03DE
loc_2_0237:
        lea    edi, [esp + ecx + 0b0h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [esp + 0ch]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  0BEh, 070h, 000h, 000h, 000h
        lea    edi, [esp + 0b4h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        movsd
        movsd
        movsd
        lea    eax, [esp + 8]
        push   eax
        movsw
        lea    eax, [esp + 0b8h]
        push   eax
        push   80000002h
        call   sub_2_054C
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_2_029F
        sub    eax, eax
        jmp    NEAR PTR loc_2_03DE
loc_2_029F:
        lea    eax, [esp + 34h]
        push   80h
        push   eax
        push   0
        push   dword ptr [esp + 14h]
        call   sub_2_055C
        add    esp, 10h
        mov    esi, eax
        push   dword ptr [esp + 8]
        call   sub_2_0554
        add    esp, 4
        or     esi, esi
        je     SHORT loc_2_02D0
        sub    eax, eax
        jmp    NEAR PTR loc_2_03DE
loc_2_02D0:
        db  0BEh, 080h, 000h, 000h, 000h
        lea    edi, [esp + 0b4h]
        mov    ecx, 0dh
        rep movsd
        movsw
        lea    edi, [esp + 34h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        lea    edi, [esp + 0b4h]
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
        db  0BEh, 0B8h, 000h, 000h, 000h
        lea    edi, [esp + 0b4h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        movsw
        lea    edi, [esp + 0ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        lea    edi, [esp + 0b4h]
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
        db  0BEh, 0BCh, 000h, 000h, 000h
        lea    edi, [esp + 0b4h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        lea    eax, [esp + 8]
        push   eax
        movsw
        lea    eax, [esp + 0b8h]
        push   eax
        push   80000002h
        call   sub_2_054C
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_2_039F
        sub    eax, eax
        jmp    SHORT loc_2_03DE
loc_2_039F:
        push   2
        db  068h, 0C0h, 000h, 000h, 000h
        push   1
        push   0
        db  068h, 0C4h, 000h, 000h, 000h
        push   dword ptr [esp + 1ch]
        call   sub_2_0564
        add    esp, 18h
        mov    esi, eax
        push   dword ptr [esp + 8]
        call   sub_2_0554
        add    esp, 4
        or     esi, esi
        je     SHORT loc_2_03D1
        sub    eax, eax
        jmp    SHORT loc_2_03DE
loc_2_03D1:
        sub    eax, eax
        jmp    SHORT loc_2_03DE
loc_2_03D5:
        sub    eax, eax
        jmp    SHORT loc_2_03DE
loc_2_03D9:
        mov    eax, 1
loc_2_03DE:
        pop    edi
        pop    esi
        add    esp, 1ach
        ret
loc_2_03E7:
        cmp    dword ptr [esp + 4], 5
        mov    eax, 1
        jne    SHORT loc_2_03FE
        mov    ecx, dword ptr [esp + 0ch]
        call   sub_2_03FF
        sub    eax, eax
loc_2_03FE:
        ret
sub_2_03FF:
        sub    esp, 188h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [esp + 0ch], 80h
        lea    eax, [esp + 0ch]
        push   0
        push   eax
        mov    ebx, ecx
        lea    eax, [esp + 1ch]
        push   eax
        push   1
        db  068h, 0D0h, 000h, 000h, 000h
        push   0
        push   ebx
        call   sub_2_05B4
        add    esp, 1ch
        or     eax, eax
        je     SHORT loc_2_043B
        sub    eax, eax
        jmp    NEAR PTR loc_2_0540
loc_2_043B:
        sub    ecx, ecx
        cmp    dword ptr [esp + 0ch], ecx
        jbe    SHORT loc_2_0456
loc_2_0443:
        lea    eax, [esp + ecx + 14h]
        cmp    byte ptr [eax], 20h
        jne    SHORT loc_2_044F
        mov    byte ptr [eax], 5fh
loc_2_044F:
        inc    ecx
        cmp    ecx, dword ptr [esp + 0ch]
        jb     SHORT loc_2_0443
loc_2_0456:
        db  0BEh, 0DCh, 000h, 000h, 000h
        lea    edi, [esp + 94h]
        movsd
        movsd
        movsw
        movsb
        lea    edi, [esp + 14h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        lea    edi, [esp + 94h]
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
        push   0
        lea    eax, [esp + 98h]
        push   ebx
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_2_05A4
        add    esp, 10h
        cmp    eax, 2
        jne    SHORT loc_2_04BF
        sub    eax, eax
        jmp    NEAR PTR loc_2_0540
loc_2_04BF:
        cmp    eax, 10h
        jne    SHORT loc_2_04C6
        jmp    SHORT loc_2_053B
loc_2_04C6:
        or     eax, eax
        mov    eax, 0
        jne    SHORT loc_2_0540
        push   0
        push   1ch
        db  068h, 0E8h, 000h, 000h, 000h
        push   1
        db  068h, 004h, 001h, 000h, 000h
        push   0
        push   dword ptr [esp + 28h]
        call   sub_2_05BC
        add    esp, 1ch
        push   0
        push   6
        db  068h, 010h, 001h, 000h, 000h
        push   1
        db  068h, 018h, 001h, 000h, 000h
        push   0
        push   dword ptr [esp + 28h]
        call   sub_2_05BC
        add    esp, 1ch
        lea    edi, [esp + 14h]
        push   0
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        lea    eax, [esp + 98h]
        push   ecx
        push   eax
        push   1
        db  068h, 020h, 001h, 000h, 000h
        push   0
        push   dword ptr [esp + 28h]
        call   sub_2_05BC
        add    esp, 1ch
loc_2_053B:
        mov    eax, 1
loc_2_0540:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 188h
        ret
        db  000h, 000h
sub_2_054C:
        db  0CDh, 020h, 048h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_2_0554:
        db  0CDh, 020h, 049h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_2_055C:
        db  0CDh, 020h, 04Ch, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_2_0564:
        db  0CDh, 020h, 052h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_2_056C:
        db  0CDh, 020h, 005h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_2_0574:
        db  0CDh, 020h, 006h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_2_057C:
        db  0CDh, 020h, 00Dh, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_2_0584:
        db  0CDh, 020h, 00Eh, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_2_058C:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_2_0594:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_2_059C:
        db  0CDh, 020h, 074h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_2_05A4:
        db  0CDh, 020h, 00Bh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_2_05AC:
        db  0CDh, 020h, 03Dh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_2_05B4:
        db  0CDh, 020h, 03Eh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_2_05BC:
        db  0CDh, 020h, 03Fh, 080h, 033h, 000h
_PTEXT ends
        end
