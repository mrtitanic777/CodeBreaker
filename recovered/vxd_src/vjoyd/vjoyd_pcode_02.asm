; vjoyd_pcode_02.asm -- VJOYD object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  08Bh, 044h, 024h, 004h, 056h, 08Dh, 034h, 0C5h, 000h, 000h, 000h, 000h, 08Bh, 00Eh, 0FFh, 031h
        db  06Ah, 000h, 0E8h, 069h, 022h, 000h, 000h, 06Ah, 000h, 08Bh, 00Eh, 0FFh, 031h, 0E8h, 056h, 022h
        db  000h, 000h, 083h, 0C4h, 008h, 08Bh, 00Eh, 06Ah, 000h, 0FFh, 071h, 004h, 0E8h, 047h, 022h, 000h
        db  000h, 083h, 0C4h, 008h, 06Ah, 000h, 0FFh, 036h, 0E8h, 03Bh, 022h, 000h, 000h, 083h, 0C4h, 008h
        db  0C7h, 006h, 000h, 000h, 000h, 000h, 05Eh, 0C3h
sub_2_0048:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        mov    ecx, dword ptr [esp + 0ch]
        cmp    byte ptr [eax], 0
        je     SHORT loc_2_0083
loc_2_0056:
        mov    bl, byte ptr [ecx]
        test   bl, bl
        je     SHORT loc_2_0083
        mov    dl, byte ptr [eax]
        cmp    dl, 41h
        jl     SHORT loc_2_006B
        cmp    dl, 5ah
        jg     SHORT loc_2_006B
        add    dl, 20h
loc_2_006B:
        cmp    bl, 41h
        jl     SHORT loc_2_0078
        cmp    bl, 5ah
        jg     SHORT loc_2_0078
        add    bl, 20h
loc_2_0078:
        cmp    bl, dl
        jne    SHORT loc_2_0092
        inc    eax
        inc    ecx
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_0056
loc_2_0083:
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_0092
        mov    eax, 1
        cmp    byte ptr [ecx], 0
        je     SHORT loc_2_0094
loc_2_0092:
        xor    eax, eax
loc_2_0094:
        pop    ebx
        ret
sub_2_0096:
        push   ebp
        mov    ebp, esp
        sub    esp, 228h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        push   edi
        jle    SHORT loc_2_00C1
        db  0B9h, 004h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
loc_2_00B5:
        mov    dword ptr [ecx], 0
        add    ecx, 8
        dec    eax
        jne    SHORT loc_2_00B5
loc_2_00C1:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    esi, 10h
loc_2_00D0:
        push   0
        push   0
        push   0
        push   0
        call   sub_2_1D87
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        db  039h, 035h, 000h, 000h, 000h, 000h
        jb     SHORT loc_2_00D0
        lea    eax, [ebp - 18h]
        push   eax
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        push   80000002h
        call   sub_2_22D0
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_2_03CF
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 010h
loc_2_011B:
        jne    SHORT loc_2_012F
        db  0BEh, 000h, 000h, 000h, 000h
        lea    edi, [ebp - 228h]
        movsd
        movsd
        movsw
        movsb
        jmp    SHORT loc_2_0167
loc_2_012F:
        db  0A1h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 228h]
        inc    eax
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_2_22C8
        add    esp, 0ch
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  08Bh, 004h, 08Dh, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_2_0167
        mov    eax, dword ptr [eax + 14h]
        test   eax, eax
        je     SHORT loc_2_0167
        push   0
        push   ecx
        call   eax
loc_2_0167:
        db  0A1h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 14h]
        push   ecx
        lea    edx, [ebp - 124h]
        push   edx
        lea    ecx, [ebp - 228h]
        db  0C7h, 004h, 085h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 14h], 104h
        lea    eax, [ebp - 1ch]
        push   eax
        push   0
        push   ecx
        push   dword ptr [ebp - 18h]
        call   sub_2_22C0
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_2_039B
        xor    esi, esi
        xor    edi, edi
        db  039h, 035h, 000h, 000h, 000h, 000h
        jle    SHORT loc_2_0216
        db  0BBh, 000h, 000h, 000h, 000h
loc_2_01BA:
        mov    eax, dword ptr [ebx]
        lea    edx, [ebp - 124h]
        push   dword ptr [eax + 4]
        push   edx
        call   sub_2_0048
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_2_01E0
        add    ebx, 8
        inc    edi
        db  039h, 03Dh, 000h, 000h, 000h, 000h
        jg     SHORT loc_2_01BA
        jmp    SHORT loc_2_0216
loc_2_01E0:
        mov    esi, 1
        lea    eax, [edi*8]
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  001h, 0B0h, 004h, 000h, 000h, 000h
        db  08Bh, 088h, 000h, 000h, 000h, 000h
        db  089h, 00Ch, 095h, 000h, 000h, 000h, 000h
        push   dword ptr [ecx + 14h]
        push   dword ptr [ecx + 0ch]
        push   dword ptr [ecx + 10h]
        push   dword ptr [ecx + 8]
        call   sub_2_1D87
loc_2_0216:
        test   esi, esi
        jne    NEAR PTR loc_2_039B
        push   1
        push   18h
        call   sub_2_2294
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        je     NEAR PTR loc_2_039B
        db  0A1h, 000h, 000h, 000h, 000h
        push   1
        lea    ecx, [ebp - 124h]
        lea    edx, [ebp - 0ch]
        push   ecx
        db  089h, 01Ch, 085h, 000h, 000h, 000h, 000h
        push   edx
        lea    eax, [ebp - 20h]
        push   eax
        call   sub_2_229C
        test   eax, eax
        jne    NEAR PTR loc_2_038B
        push   1
        lea    edi, [ebp - 124h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        mov    dword ptr [ebp - 10h], 0
        mov    dword ptr [ebp - 4], 0
        repne scasb
        not    ecx
        push   ecx
        call   sub_2_2294
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     SHORT loc_2_0305
        lea    edi, [ebp - 124h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, dword ptr [ebp - 8]
        rep movsd
        mov    ecx, eax
        push   1
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [ebp - 0ch]
        sub    eax, eax
        mov    edi, dword ptr [ecx + 0bh]
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        push   ecx
        call   sub_2_2294
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     SHORT loc_2_0305
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, 0ffffffffh
        mov    edi, dword ptr [eax + 0bh]
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    edx, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, dword ptr [ebp - 4]
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    dword ptr [ebp - 10h], 1
loc_2_0305:
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_2_0336
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        shl    eax, 3
        mov    edx, dword ptr [ebp - 4]
        db  0C7h, 080h, 004h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  089h, 098h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 4], ecx
        mov    dword ptr [ebx], edx
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_039B
loc_2_0336:
        mov    eax, dword ptr [ebp - 0ch]
        push   dword ptr [eax + 0bh]
        push   0
        call   sub_2_2280
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        test   ebx, ebx
        db  0C7h, 004h, 08Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_0363
        push   0
        push   ebx
        call   sub_2_2278
        add    esp, 8
loc_2_0363:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_2_0376
        push   0
        push   dword ptr [ebp - 8]
        call   sub_2_2278
        add    esp, 8
loc_2_0376:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_039B
        push   0
        push   dword ptr [ebp - 4]
        call   sub_2_2278
        add    esp, 8
        jmp    SHORT loc_2_039B
loc_2_038B:
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 004h, 085h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_039B:
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 004h, 085h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_2_03BC
        mov    eax, dword ptr [eax + 14h]
        test   eax, eax
        je     SHORT loc_2_03BC
        push   1
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   eax
loc_2_03BC:
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 010h
        jbe    NEAR PTR loc_2_011B
loc_2_03CF:
        xor    esi, esi
        db  039h, 035h, 000h, 000h, 000h, 000h
        jle    SHORT loc_2_0433
        db  0C7h, 045h, 0FCh, 004h, 000h, 000h, 000h
loc_2_03E0:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax], 0
        jne    SHORT loc_2_0426
        push   esi
        call   0
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        dec    eax
        mov    dword ptr [ebp - 8], eax
        cmp    esi, eax
        jge    SHORT loc_2_041E
        mov    ecx, dword ptr [ebp - 4]
        db  0A1h, 000h, 000h, 000h, 000h
        sub    ecx, 4
        sub    eax, esi
        dec    eax
loc_2_040C:
        mov    ebx, dword ptr [ecx + 0ch]
        lea    edx, [ecx + 8]
        dec    eax
        mov    edi, dword ptr [edx]
        mov    dword ptr [ecx], edi
        mov    dword ptr [ecx + 4], ebx
        mov    ecx, edx
        jne    SHORT loc_2_040C
loc_2_041E:
        mov    eax, dword ptr [ebp - 8]
        db  0A3h, 000h, 000h, 000h, 000h
loc_2_0426:
        add    dword ptr [ebp - 4], 8
        inc    esi
        db  039h, 035h, 000h, 000h, 000h, 000h
        jg     SHORT loc_2_03E0
loc_2_0433:
        db  083h, 03Dh, 040h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_049D
        db  08Bh, 035h, 040h, 000h, 000h, 000h
        xor    edi, edi
        db  039h, 03Dh, 000h, 000h, 000h, 000h
        jle    SHORT loc_2_049D
        db  0B8h, 000h, 000h, 000h, 000h
loc_2_0451:
        db  08Bh, 00Dh, 040h, 000h, 000h, 000h
        cmp    dword ptr [eax], ecx
        je     SHORT loc_2_0469
        add    eax, 8
        inc    edi
        db  039h, 03Dh, 000h, 000h, 000h, 000h
        jg     SHORT loc_2_0451
        jmp    SHORT loc_2_049D
loc_2_0469:
        xor    ebx, ebx
loc_2_046B:
        db  08Dh, 004h, 09Dh, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax], 0
        jne    SHORT loc_2_048B
        mov    dword ptr [eax], esi
        db  0FFh, 004h, 0FDh, 004h, 000h, 000h, 000h
        mov    eax, dword ptr [esi + 14h]
        test   eax, eax
        je     SHORT loc_2_0497
        push   1
        jmp    SHORT loc_2_0494
loc_2_048B:
        mov    eax, dword ptr [esi + 14h]
        test   eax, eax
        je     SHORT loc_2_0497
        push   0
loc_2_0494:
        push   ebx
        call   eax
loc_2_0497:
        inc    ebx
        cmp    ebx, 10h
        jl     SHORT loc_2_046B
loc_2_049D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_04A4:
        push   ebp
        mov    ebp, esp
        sub    esp, 114h
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        push   edi
        xor    edi, edi
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   80000002h
        call   sub_2_22D0
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_2_0667
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, 4
loc_2_04D8:
        push   100h
        lea    eax, [ebp - 114h]
        push   eax
        push   edi
        push   dword ptr [ebp - 4]
        call   sub_2_22D8
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_2_0649
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 114h]
        push   eax
        push   ecx
        push   dword ptr [ebp - 4]
        call   sub_2_22D0
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_053C
        lea    eax, [ebp - 10h]
        lea    ecx, [ebp - 8]
        push   eax
        lea    edx, [ebp - 14h]
        push   ecx
        mov    dword ptr [ebp - 10h], ebx
        push   edx
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 0ch]
        call   sub_2_22C0
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_2_053C
        cmp    dword ptr [ebp - 8], esi
        je     SHORT loc_2_053F
loc_2_053C:
        inc    edi
        jmp    SHORT loc_2_04D8
loc_2_053F:
        lea    edi, [ebp - 114h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        db  0A1h, 000h, 000h, 000h, 000h
        lea    ebx, [ecx + 39h]
        test   eax, eax
        je     SHORT loc_2_0567
        push   0
        push   eax
        call   sub_2_2278
        add    esp, 8
loc_2_0567:
        push   1
        push   ebx
        call   sub_2_2294
        add    esp, 8
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     NEAR PTR loc_2_0649
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        lea    edi, [ebp - 114h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    edx, ecx
        shr    ecx, 2
        mov    esi, edi
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        add    edi, 39h
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        test   edx, edx
        je     SHORT loc_2_05E8
        push   0
        push   edx
        call   sub_2_2278
        add    esp, 8
loc_2_05E8:
        push   1
        lea    eax, [ebx + 18h]
        push   eax
        call   sub_2_2294
        add    esp, 8
        mov    edx, eax
        test   edx, edx
        je     SHORT loc_2_064F
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, edx
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        lea    edi, [ebx + edx - 1]
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        jmp    SHORT loc_2_064F
loc_2_0649:
        db  08Bh, 015h, 000h, 000h, 000h, 000h
loc_2_064F:
        db  089h, 015h, 000h, 000h, 000h, 000h
        test   edx, edx
        je     SHORT loc_2_0667
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_0667
        call   sub_2_0096
loc_2_0667:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0670:
        push   esi
        xor    esi, esi
        db  039h, 035h, 000h, 000h, 000h, 000h
        jle    SHORT loc_2_068D
loc_2_067B:
        push   esi
        call   0
        add    esp, 4
        inc    esi
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jl     SHORT loc_2_067B
loc_2_068D:
        xor    esi, esi
        db  0A1h, 000h, 000h, 000h, 000h
        db  089h, 035h, 000h, 000h, 000h, 000h
        cmp    eax, esi
        je     SHORT loc_2_06AE
        push   esi
        push   eax
        call   sub_2_2278
        add    esp, 8
        db  089h, 035h, 000h, 000h, 000h, 000h
loc_2_06AE:
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_2_06CC
        push   0
        push   eax
        call   sub_2_2278
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        add    esp, 8
loc_2_06CC:
        pop    esi
        ret
loc_2_06CE:
        jmp    NEAR PTR loc_2_0C28
sub_2_06D3:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 0e0h
        db  039h, 005h, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        mov    dword ptr [ebp - 0e0h], eax
        push   edi
        mov    dword ptr [ebp - 0dch], eax
        jle    SHORT loc_2_0750
        db  0BBh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
loc_2_0700:
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [eax + 10h]
        test   ecx, ecx
        je     SHORT loc_2_072F
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   edi
        mov    dword ptr [ebp - 0e0h], 1
        push   esi
        call   ecx
        add    esp, 14h
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     SHORT loc_2_072F
        cmp    eax, 1
        jne    SHORT loc_2_0748
loc_2_072F:
        add    ebx, 8
        inc    dword ptr [ebp - 0dch]
        mov    eax, dword ptr [ebp - 0dch]
        db  03Bh, 005h, 000h, 000h, 000h, 000h
        jl     SHORT loc_2_0700
        jmp    SHORT loc_2_0756
loc_2_0748:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_090F
loc_2_0750:
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
loc_2_0756:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_078B
        db  083h, 03Dh, 004h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_078B
        db  083h, 03Dh, 008h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_078B
        db  083h, 03Dh, 00Ch, 000h, 000h, 000h, 000h
        je     SHORT loc_2_078B
        cmp    dword ptr [ebp - 0e0h], 0
        je     SHORT loc_2_078B
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_090F
loc_2_078B:
        test   esi, esi
        je     NEAR PTR loc_2_090D
        cmp    esi, 1
        je     SHORT loc_2_07BD
        cmp    esi, 2
        je     NEAR PTR loc_2_08F2
        cmp    esi, 3
        je     NEAR PTR loc_2_08FD
        cmp    esi, 4
        je     NEAR PTR loc_2_090D
        mov    eax, 1
        jmp    NEAR PTR loc_2_090F
loc_2_07BD:
        cmp    edi, 1
        jbe    SHORT loc_2_07CC
        mov    eax, 1
        jmp    NEAR PTR loc_2_090F
loc_2_07CC:
        push   0
        lea    ecx, [ebp - 0dch]
        push   dword ptr [ebp + 10h]
        push   ecx
        call   sub_2_22E0
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_2_090F
        cmp    word ptr [ebp - 80h], 1
        jb     NEAR PTR loc_2_08DF
        cmp    word ptr [ebp - 7eh], 200h
        jb     NEAR PTR loc_2_08CC
        cmp    word ptr [ebp - 7eh], 20fh
        ja     NEAR PTR loc_2_08CC
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_084B
        db  083h, 03Dh, 004h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_084B
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 000h
        movzx  eax, word ptr [ebp - 7eh]
        mov    ecx, 2
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        db  0A3h, 004h, 000h, 000h, 000h
        db  089h, 00Dh, 004h, 000h, 000h, 000h
loc_2_084B:
        mov    edx, 2
        cmp    word ptr [ebp - 80h], dx
        jne    NEAR PTR loc_2_090D
        cmp    word ptr [ebp - 54h], 1
        jne    NEAR PTR loc_2_090D
        cmp    word ptr [ebp - 7ch], 200h
        jb     NEAR PTR loc_2_090D
        cmp    word ptr [ebp - 7ch], 20fh
        ja     NEAR PTR loc_2_090D
        db  083h, 03Dh, 008h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_2_090D
        db  083h, 03Dh, 00Ch, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_090D
        mov    ecx, 4
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 035h, 008h, 000h, 000h, 000h
        db  089h, 00Dh, 004h, 000h, 000h, 000h
        db  089h, 015h, 008h, 000h, 000h, 000h
        movzx  eax, word ptr [ebp - 7ch]
        db  0A3h, 004h, 000h, 000h, 000h
        db  0A3h, 00Ch, 000h, 000h, 000h
        db  089h, 015h, 00Ch, 000h, 000h, 000h
        jmp    SHORT loc_2_090D
loc_2_08CC:
        cmp    dword ptr [ebp - 0e0h], 0
        mov    eax, dword ptr [ebp - 4]
        jne    SHORT loc_2_090F
        mov    eax, 13h
        jmp    SHORT loc_2_090F
loc_2_08DF:
        cmp    dword ptr [ebp - 0e0h], 0
        mov    eax, dword ptr [ebp - 4]
        jne    SHORT loc_2_090F
        mov    eax, 13h
        jmp    SHORT loc_2_090F
loc_2_08F2:
        test   edi, edi
        je     SHORT loc_2_090D
        mov    eax, 1
        jmp    SHORT loc_2_090F
loc_2_08FD:
        test   edi, edi
        je     SHORT loc_2_090D
        cmp    edi, 1
        je     SHORT loc_2_090D
        mov    eax, 1
        jmp    SHORT loc_2_090F
loc_2_090D:
        xor    eax, eax
loc_2_090F:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0916:
        xor    eax, eax
        ret    8
sub_2_091B:
        xor    eax, eax
        ret    4
loc_2_0920:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   esi
        lea    eax, [ebp - 2ch]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        push   eax
        push   0
        db  08Bh, 004h, 0B5h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 2ch], esi
        mov    dword ptr [ebp - 28h], edi
        call   dword ptr [eax + 8]
        cmp    eax, 1
        jne    SHORT loc_2_096F
        mov    ecx, dword ptr [ebp - 24h]
        mov    edx, dword ptr [ebp - 8]
        mov    esi, dword ptr [ebp - 4]
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 0ch]
        db  089h, 015h, 000h, 000h, 000h, 000h
        db  089h, 035h, 000h, 000h, 000h, 000h
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_097B
loc_2_096F:
        cmp    eax, -1
        jne    SHORT loc_2_097B
        push   edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_097B:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_0983:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   esi
        lea    eax, [ebp - 2ch]
        mov    esi, dword ptr [ebp + 8]
        push   eax
        db  08Bh, 004h, 0B5h, 000h, 000h, 000h, 000h
        push   1
        mov    dword ptr [ebp - 2ch], esi
        call   dword ptr [eax + 8]
        cmp    eax, 1
        jne    SHORT loc_2_09D4
        mov    ecx, dword ptr [ebp - 24h]
        mov    edx, dword ptr [ebp - 20h]
        mov    esi, dword ptr [ebp - 8]
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        db  089h, 015h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 0ch]
        db  089h, 035h, 000h, 000h, 000h, 000h
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 015h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_09DF
loc_2_09D4:
        cmp    eax, -1
        jne    SHORT loc_2_09DF
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_09DF:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_09E6:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   esi
        lea    eax, [ebp - 2ch]
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        push   eax
        db  08Bh, 004h, 0BDh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 28h], esi
        push   2
        mov    dword ptr [ebp - 2ch], edi
        call   dword ptr [eax + 8]
        cmp    eax, 1
        jne    SHORT loc_2_0A56
        mov    ecx, dword ptr [ebp - 24h]
        mov    edx, dword ptr [ebp - 20h]
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 015h, 000h, 000h, 000h, 000h
        test   esi, esi
        jne    SHORT loc_2_0A30
        mov    ecx, dword ptr [ebp - 1ch]
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_0A39
loc_2_0A30:
        mov    ecx, dword ptr [ebp - 18h]
        db  089h, 00Dh, 000h, 000h, 000h, 000h
loc_2_0A39:
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp - 0ch]
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 015h, 000h, 000h, 000h, 000h
        db  089h, 035h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_0A62
loc_2_0A56:
        cmp    eax, -1
        jne    SHORT loc_2_0A62
        push   esi
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0A62:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_0A6A:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   esi
        lea    eax, [ebp - 2ch]
        mov    esi, dword ptr [ebp + 8]
        push   eax
        db  08Bh, 004h, 0B5h, 000h, 000h, 000h, 000h
        push   3
        mov    dword ptr [ebp - 2ch], esi
        call   dword ptr [eax + 8]
        cmp    eax, 1
        jne    SHORT loc_2_0ACD
        mov    ecx, dword ptr [ebp - 24h]
        mov    edx, dword ptr [ebp - 20h]
        mov    esi, dword ptr [ebp - 1ch]
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 18h]
        db  089h, 015h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 8]
        db  089h, 035h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ebp - 4]
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 0ch]
        db  089h, 015h, 000h, 000h, 000h, 000h
        db  089h, 035h, 000h, 000h, 000h, 000h
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_0AD8
loc_2_0ACD:
        cmp    eax, -1
        jne    SHORT loc_2_0AD8
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0AD8:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0ADF:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   esi
        lea    eax, [ebp - 2ch]
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        push   eax
        db  08Bh, 004h, 0BDh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 28h], esi
        push   4
        mov    dword ptr [ebp - 2ch], edi
        call   dword ptr [eax + 8]
        cmp    eax, 1
        jne    SHORT loc_2_0B61
        mov    ecx, dword ptr [ebp - 24h]
        mov    edx, dword ptr [ebp - 20h]
        mov    edi, dword ptr [ebp - 1ch]
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 18h]
        db  089h, 015h, 000h, 000h, 000h, 000h
        db  089h, 03Dh, 000h, 000h, 000h, 000h
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        test   esi, esi
        jne    SHORT loc_2_0B3B
        mov    ecx, dword ptr [ebp - 14h]
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_0B44
loc_2_0B3B:
        mov    ecx, dword ptr [ebp - 10h]
        db  089h, 00Dh, 000h, 000h, 000h, 000h
loc_2_0B44:
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp - 0ch]
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 015h, 000h, 000h, 000h, 000h
        db  089h, 035h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_0B6D
loc_2_0B61:
        cmp    eax, -1
        jne    SHORT loc_2_0B6D
        push   esi
        push   edi
        call   sub_2_0916
loc_2_0B6D:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_0B75:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   esi
        lea    eax, [ebp - 2ch]
        mov    esi, dword ptr [ebp + 8]
        push   eax
        db  08Bh, 004h, 0B5h, 000h, 000h, 000h, 000h
        push   5
        mov    dword ptr [ebp - 2ch], esi
        call   dword ptr [eax + 8]
        cmp    eax, 1
        jne    SHORT loc_2_0BEA
        mov    ecx, dword ptr [ebp - 24h]
        mov    edx, dword ptr [ebp - 20h]
        mov    esi, dword ptr [ebp - 1ch]
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 18h]
        db  089h, 015h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 14h]
        db  089h, 035h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ebp - 10h]
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        db  089h, 015h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 4]
        db  089h, 035h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ebp - 0ch]
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 015h, 000h, 000h, 000h, 000h
        db  089h, 035h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_0BF5
loc_2_0BEA:
        cmp    eax, -1
        jne    SHORT loc_2_0BF5
        push   esi
        call   sub_2_091B
loc_2_0BF5:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0BFC:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 2ch
        lea    ecx, [esp]
        mov    dword ptr [esp], eax
        push   ecx
        db  08Bh, 004h, 085h, 000h, 000h, 000h, 000h
        push   6
        call   dword ptr [eax + 8]
        mov    ecx, dword ptr [esp + 24h]
        add    esp, 2ch
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        ret    4
loc_2_0C28:
        push   esi
        db  0B8h, 004h, 000h, 000h, 000h
        push   edi
        db  0B9h, 000h, 000h, 000h, 000h
        db  0BAh, 000h, 000h, 000h, 000h
        db  0BEh, 000h, 000h, 000h, 000h
        db  0BFh, 000h, 000h, 000h, 000h
loc_2_0C43:
        db  0C7h, 000h, 000h, 000h, 000h, 000h
        add    eax, 20h
        db  03Dh, 004h, 002h, 000h, 000h
        mov    dword ptr [eax - 1ch], ecx
        mov    dword ptr [eax - 18h], edx
        mov    dword ptr [eax - 14h], esi
        mov    dword ptr [eax - 10h], edi
        db  0C7h, 040h, 0F4h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 0F8h, 000h, 000h, 000h, 000h
        jbe    SHORT loc_2_0C43
        pop    edi
        pop    esi
        ret
sub_2_0C70:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ebp - 4], eax
        cmp    ebx, 10h
        jl     SHORT loc_2_0C8E
        xor    eax, eax
        jmp    SHORT loc_2_0D07
loc_2_0C8E:
        mov    eax, ebx
        shl    eax, 5
        db  083h, 0B8h, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_0CD9
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 002h
        jne    SHORT loc_2_0CBD
        cmp    ebx, 1
        jne    SHORT loc_2_0CB7
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 002h
        jbe    SHORT loc_2_0CD9
        xor    eax, eax
        jmp    SHORT loc_2_0D07
loc_2_0CB7:
        jle    SHORT loc_2_0CD9
        xor    eax, eax
        jmp    SHORT loc_2_0D07
loc_2_0CBD:
        cmp    ebx, 3
        jle    SHORT loc_2_0CC6
        xor    eax, eax
        jmp    SHORT loc_2_0D07
loc_2_0CC6:
        cmp    ebx, 1
        jle    SHORT loc_2_0CD9
        db  083h, 03Ch, 09Dh, 000h, 000h, 000h, 000h, 002h
        jbe    SHORT loc_2_0CD9
        xor    eax, eax
        jmp    SHORT loc_2_0D07
loc_2_0CD9:
        test   byte ptr [ebp - 2], 1
        jne    SHORT loc_2_0D02
        mov    eax, ebx
        imul   eax, eax, 0c8h
        db  083h, 0B8h, 040h, 001h, 000h, 000h, 000h
        jne    SHORT loc_2_0CF4
        xor    eax, eax
        jmp    SHORT loc_2_0D07
loc_2_0CF4:
        db  083h, 0B8h, 0E4h, 000h, 000h, 000h, 000h
        mov    eax, 0
        je     SHORT loc_2_0D07
loc_2_0D02:
        mov    eax, 1
loc_2_0D07:
        test   eax, eax
        jne    SHORT loc_2_0D15
        mov    eax, 0a5h
        jmp    NEAR PTR loc_2_182E
loc_2_0D15:
        db  08Bh, 014h, 09Dh, 000h, 000h, 000h, 000h
        db  08Bh, 00Ch, 09Dh, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        mov    dword ptr [esi + 8], 0
        mov    dword ptr [esi + 0ch], 0
        mov    dword ptr [esi + 10h], 0
        mov    dword ptr [esi + 14h], 0
        mov    dword ptr [esi + 18h], 0
        mov    eax, ebx
        imul   eax, eax, 0c8h
        mov    dword ptr [esi + 1ch], 0
        db  08Dh, 0B8h, 07Ch, 000h, 000h, 000h
        db  08Bh, 004h, 09Dh, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        and    eax, 2ffh
        cmp    eax, 80h
        jne    SHORT loc_2_0DFA
        push   ebx
        shl    ebx, 5
        db  0FFh, 093h, 01Ch, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 20h], eax
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_0DEB
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 001h
        je     SHORT loc_2_0DBB
        mov    dword ptr [esi + 24h], 1
        jmp    SHORT loc_2_0DF3
loc_2_0DBB:
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 002h
        je     SHORT loc_2_0DCD
        mov    dword ptr [esi + 24h], 2
        jmp    SHORT loc_2_0DF3
loc_2_0DCD:
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 004h
        je     SHORT loc_2_0DDF
        mov    dword ptr [esi + 24h], 3
        jmp    SHORT loc_2_0DF3
loc_2_0DDF:
        db  0A1h, 000h, 000h, 000h, 000h
        and    eax, 8
        shr    eax, 1
        jmp    SHORT loc_2_0DF0
loc_2_0DEB:
        db  0A1h, 000h, 000h, 000h, 000h
loc_2_0DF0:
        mov    dword ptr [esi + 24h], eax
loc_2_0DF3:
        xor    eax, eax
        jmp    NEAR PTR loc_2_182E
loc_2_0DFA:
        test   dword ptr [ebp - 4], 0f320000h
        je     NEAR PTR loc_2_0F49
        test   byte ptr [ebp - 2], 2
        je     SHORT loc_2_0E49
        mov    eax, ebx
        push   0
        shl    eax, 5
        push   ebx
        db  08Dh, 0B8h, 004h, 000h, 000h, 000h
        call   dword ptr [edi]
        test   eax, eax
        je     NEAR PTR loc_2_1829
        db  0A1h, 000h, 000h, 000h, 000h
        push   1
        push   ebx
        mov    dword ptr [esi + 8], eax
        call   dword ptr [edi]
        test   eax, eax
        je     NEAR PTR loc_2_1829
loc_2_0E3A:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 0ch], eax
loc_2_0E42:
        xor    eax, eax
        jmp    NEAR PTR loc_2_182E
loc_2_0E49:
        test   byte ptr [ebp - 2], 10h
        je     SHORT loc_2_0E6D
        push   0
        push   ebx
        shl    ebx, 5
        db  0FFh, 093h, 004h, 000h, 000h, 000h
        test   eax, eax
        je     NEAR PTR loc_2_1829
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 8], eax
        jmp    SHORT loc_2_0E42
loc_2_0E6D:
        test   byte ptr [ebp - 2], 20h
        je     SHORT loc_2_0E88
        push   1
        push   ebx
        shl    ebx, 5
        db  0FFh, 093h, 004h, 000h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_2_0E3A
        jmp    NEAR PTR loc_2_1829
loc_2_0E88:
        test   byte ptr [ebp - 1], 1
        je     SHORT loc_2_0EB5
        cmp    ecx, 3
        jb     NEAR PTR loc_2_1829
        push   2
        push   ebx
        shl    ebx, 5
        db  0FFh, 093h, 004h, 000h, 000h, 000h
        test   eax, eax
        je     NEAR PTR loc_2_1829
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 10h], eax
        jmp    SHORT loc_2_0E42
loc_2_0EB5:
        test   byte ptr [ebp - 1], 2
        je     SHORT loc_2_0EE5
        cmp    ecx, 3
        jb     NEAR PTR loc_2_1829
        push   3
        push   ebx
        shl    ebx, 5
        db  0FFh, 093h, 004h, 000h, 000h, 000h
        test   eax, eax
        je     NEAR PTR loc_2_1829
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 14h], eax
        jmp    NEAR PTR loc_2_0E42
loc_2_0EE5:
        test   byte ptr [ebp - 1], 4
        je     SHORT loc_2_0F15
        cmp    ecx, 5
        jb     NEAR PTR loc_2_1829
        push   4
        push   ebx
        shl    ebx, 5
        db  0FFh, 093h, 004h, 000h, 000h, 000h
        test   eax, eax
        je     NEAR PTR loc_2_1829
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 18h], eax
        jmp    NEAR PTR loc_2_0E42
loc_2_0F15:
        test   byte ptr [ebp - 1], 8
        je     NEAR PTR loc_2_0E42
        cmp    ecx, 5
        jb     NEAR PTR loc_2_1829
        push   5
        push   ebx
        shl    ebx, 5
        db  0FFh, 093h, 004h, 000h, 000h, 000h
        test   eax, eax
        je     NEAR PTR loc_2_1829
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 1ch], eax
        jmp    NEAR PTR loc_2_0E42
loc_2_0F49:
        test   byte ptr [ebp - 2], 0c0h
        jne    NEAR PTR loc_2_119A
        cmp    edx, 4
        ja     NEAR PTR loc_2_119A
        test   byte ptr [ebp - 2], 8
        jne    NEAR PTR loc_2_109C
        cmp    edx, 4
        je     NEAR PTR loc_2_109C
        mov    eax, dword ptr [ebp - 4]
        and    eax, 40000h
        mov    dword ptr [ebp - 8], eax
        jne    SHORT loc_2_0F9A
        cmp    edx, 3
        je     SHORT loc_2_0F9A
        push   ebx
        mov    eax, ebx
        shl    eax, 5
        db  0FFh, 090h, 008h, 000h, 000h, 000h
        test   eax, eax
        jne    NEAR PTR loc_2_14C0
        jmp    NEAR PTR loc_2_1829
loc_2_0F9A:
        cmp    ecx, 3
        jb     NEAR PTR loc_2_1829
        mov    eax, ebx
        shl    eax, 5
        push   dword ptr [edi + 0b0h]
        push   ebx
        db  0FFh, 090h, 00Ch, 000h, 000h, 000h
        test   eax, eax
        je     NEAR PTR loc_2_1829
        cmp    dword ptr [edi + 0b8h], 0
        je     SHORT loc_2_0FD0
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_2_14C0
loc_2_0FD0:
        cmp    dword ptr [edi + 0b0h], 0
        je     SHORT loc_2_103E
        test   byte ptr [ebp - 4], 8
        je     NEAR PTR loc_2_14C0
        test   byte ptr [ebp - 3], 1
        je     SHORT loc_2_0FF6
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 14h], eax
        jmp    NEAR PTR loc_2_14C0
loc_2_0FF6:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        sub    ecx, dword ptr [edi + 18h]
        jns    SHORT loc_2_100E
        db  0A1h, 010h, 000h, 000h, 000h
        mov    dword ptr [esi + 14h], eax
        jmp    NEAR PTR loc_2_14C0
loc_2_100E:
        db  0A1h, 070h, 000h, 000h, 000h
        sub    edx, edx
        imul   eax, ecx
        div    dword ptr [edi + 7ch]
        db  003h, 005h, 010h, 000h, 000h, 000h
        db  03Bh, 005h, 028h, 000h, 000h, 000h
        jbe    SHORT loc_2_1036
        db  0A1h, 028h, 000h, 000h, 000h
        mov    dword ptr [esi + 14h], eax
        jmp    NEAR PTR loc_2_14C0
loc_2_1036:
        mov    dword ptr [esi + 14h], eax
        jmp    NEAR PTR loc_2_14C0
loc_2_103E:
        test   byte ptr [ebp - 4], 4
        je     NEAR PTR loc_2_14C0
        test   byte ptr [ebp - 3], 1
        db  0A1h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_105B
        mov    dword ptr [esi + 10h], eax
        jmp    NEAR PTR loc_2_14C0
loc_2_105B:
        sub    eax, dword ptr [edi + 14h]
        jns    SHORT loc_2_106D
        db  0A1h, 00Ch, 000h, 000h, 000h
        mov    dword ptr [esi + 10h], eax
        jmp    NEAR PTR loc_2_14C0
loc_2_106D:
        db  00Fh, 0AFh, 005h, 06Ch, 000h, 000h, 000h
        sub    edx, edx
        div    dword ptr [edi + 78h]
        db  003h, 005h, 00Ch, 000h, 000h, 000h
        db  039h, 005h, 024h, 000h, 000h, 000h
        jae    SHORT loc_2_1094
        db  0A1h, 024h, 000h, 000h, 000h
        mov    dword ptr [esi + 10h], eax
        jmp    NEAR PTR loc_2_14C0
loc_2_1094:
        mov    dword ptr [esi + 10h], eax
        jmp    NEAR PTR loc_2_14C0
loc_2_109C:
        cmp    ecx, 4
        jb     NEAR PTR loc_2_1829
        push   ebx
        mov    eax, ebx
        shl    eax, 5
        db  0FFh, 090h, 010h, 000h, 000h, 000h
        test   eax, eax
        je     NEAR PTR loc_2_1829
        test   byte ptr [ebp - 2], 0c8h
        jne    SHORT loc_2_10E3
        cmp    dword ptr [edi + 0b8h], 0
        je     SHORT loc_2_10E3
        cmp    dword ptr [edi + 0ach], 0
        jne    SHORT loc_2_10DA
        mov    eax, 1
        xor    ecx, ecx
        jmp    SHORT loc_2_10EA
loc_2_10DA:
        xor    eax, eax
        mov    ecx, 1
        jmp    SHORT loc_2_10EA
loc_2_10E3:
        mov    eax, 1
        mov    ecx, eax
loc_2_10EA:
        test   eax, eax
        je     SHORT loc_2_112D
        test   byte ptr [ebp - 4], 8
        je     SHORT loc_2_112D
        test   byte ptr [ebp - 3], 1
        db  0A1h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_112A
        sub    eax, dword ptr [edi + 18h]
        jns    SHORT loc_2_110B
        db  0A1h, 010h, 000h, 000h, 000h
        jmp    SHORT loc_2_112A
loc_2_110B:
        db  00Fh, 0AFh, 005h, 070h, 000h, 000h, 000h
        sub    edx, edx
        div    dword ptr [edi + 7ch]
        db  003h, 005h, 010h, 000h, 000h, 000h
        db  03Bh, 005h, 028h, 000h, 000h, 000h
        jbe    SHORT loc_2_112A
        db  0A1h, 028h, 000h, 000h, 000h
loc_2_112A:
        mov    dword ptr [esi + 14h], eax
loc_2_112D:
        test   ecx, ecx
        je     NEAR PTR loc_2_14C0
        test   byte ptr [ebp - 4], 4
        je     NEAR PTR loc_2_14C0
        test   byte ptr [ebp - 3], 1
        je     SHORT loc_2_1152
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 10h], eax
        jmp    NEAR PTR loc_2_14C0
loc_2_1152:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        sub    ecx, dword ptr [edi + 14h]
        jns    SHORT loc_2_116A
        db  0A1h, 00Ch, 000h, 000h, 000h
        mov    dword ptr [esi + 10h], eax
        jmp    NEAR PTR loc_2_14C0
loc_2_116A:
        db  0A1h, 06Ch, 000h, 000h, 000h
        sub    edx, edx
        imul   eax, ecx
        div    dword ptr [edi + 78h]
        db  003h, 005h, 00Ch, 000h, 000h, 000h
        db  03Bh, 005h, 024h, 000h, 000h, 000h
        jbe    SHORT loc_2_1192
        db  0A1h, 024h, 000h, 000h, 000h
        mov    dword ptr [esi + 10h], eax
        jmp    NEAR PTR loc_2_14C0
loc_2_1192:
        mov    dword ptr [esi + 10h], eax
        jmp    NEAR PTR loc_2_14C0
loc_2_119A:
        test   byte ptr [ebp - 2], 40h
        jne    NEAR PTR loc_2_1334
        cmp    edx, 5
        je     NEAR PTR loc_2_1334
        cmp    ecx, 6
        jb     NEAR PTR loc_2_1829
        push   ebx
        mov    eax, ebx
        shl    eax, 5
        db  0FFh, 090h, 018h, 000h, 000h, 000h
        test   eax, eax
        je     NEAR PTR loc_2_1829
        test   byte ptr [ebp - 2], 0c8h
        jne    SHORT loc_2_11F4
        cmp    dword ptr [edi + 0b8h], 0
        je     SHORT loc_2_11F4
        cmp    dword ptr [edi + 0ach], 0
        jne    SHORT loc_2_11EB
        mov    eax, 1
        xor    ecx, ecx
        jmp    SHORT loc_2_11FB
loc_2_11EB:
        xor    eax, eax
        mov    ecx, 1
        jmp    SHORT loc_2_11FB
loc_2_11F4:
        mov    eax, 1
        mov    ecx, eax
loc_2_11FB:
        test   eax, eax
        je     SHORT loc_2_123E
        test   byte ptr [ebp - 4], 8
        je     SHORT loc_2_123E
        test   byte ptr [ebp - 3], 1
        db  0A1h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_123B
        sub    eax, dword ptr [edi + 18h]
        jns    SHORT loc_2_121C
        db  0A1h, 010h, 000h, 000h, 000h
        jmp    SHORT loc_2_123B
loc_2_121C:
        db  00Fh, 0AFh, 005h, 070h, 000h, 000h, 000h
        sub    edx, edx
        div    dword ptr [edi + 7ch]
        db  003h, 005h, 010h, 000h, 000h, 000h
        db  03Bh, 005h, 028h, 000h, 000h, 000h
        jbe    SHORT loc_2_123B
        db  0A1h, 028h, 000h, 000h, 000h
loc_2_123B:
        mov    dword ptr [esi + 14h], eax
loc_2_123E:
        test   ecx, ecx
        je     SHORT loc_2_128A
        test   byte ptr [ebp - 4], 4
        je     SHORT loc_2_128A
        test   byte ptr [ebp - 3], 1
        je     SHORT loc_2_1255
        db  0A1h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_1287
loc_2_1255:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        sub    ecx, dword ptr [edi + 14h]
        jns    SHORT loc_2_1267
        db  0A1h, 00Ch, 000h, 000h, 000h
        jmp    SHORT loc_2_1287
loc_2_1267:
        db  0A1h, 06Ch, 000h, 000h, 000h
        sub    edx, edx
        imul   eax, ecx
        div    dword ptr [edi + 78h]
        db  003h, 005h, 00Ch, 000h, 000h, 000h
        db  03Bh, 005h, 024h, 000h, 000h, 000h
        jbe    SHORT loc_2_1287
        db  0A1h, 024h, 000h, 000h, 000h
loc_2_1287:
        mov    dword ptr [esi + 10h], eax
loc_2_128A:
        test   byte ptr [ebp - 4], 10h
        je     SHORT loc_2_12CC
        test   byte ptr [ebp - 3], 1
        db  0A1h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_12C9
        sub    eax, dword ptr [edi + 1ch]
        jns    SHORT loc_2_12A7
        db  0A1h, 014h, 000h, 000h, 000h
        jmp    SHORT loc_2_12C9
loc_2_12A7:
        db  00Fh, 0AFh, 005h, 074h, 000h, 000h, 000h
        sub    edx, edx
        div    dword ptr [edi + 80h]
        db  003h, 005h, 014h, 000h, 000h, 000h
        db  039h, 005h, 02Ch, 000h, 000h, 000h
        jae    SHORT loc_2_12C9
        db  0A1h, 02Ch, 000h, 000h, 000h
loc_2_12C9:
        mov    dword ptr [esi + 18h], eax
loc_2_12CC:
        test   byte ptr [ebp - 4], 20h
        je     NEAR PTR loc_2_14C0
        test   byte ptr [ebp - 3], 1
        je     SHORT loc_2_12E9
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 1ch], eax
        jmp    NEAR PTR loc_2_14C0
loc_2_12E9:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        sub    ecx, dword ptr [edi + 20h]
        jns    SHORT loc_2_1301
        db  0A1h, 018h, 000h, 000h, 000h
        mov    dword ptr [esi + 1ch], eax
        jmp    NEAR PTR loc_2_14C0
loc_2_1301:
        db  0A1h, 078h, 000h, 000h, 000h
        sub    edx, edx
        imul   eax, ecx
        div    dword ptr [edi + 84h]
        db  003h, 005h, 018h, 000h, 000h, 000h
        db  03Bh, 005h, 030h, 000h, 000h, 000h
        jbe    SHORT loc_2_132C
        db  0A1h, 030h, 000h, 000h, 000h
        mov    dword ptr [esi + 1ch], eax
        jmp    NEAR PTR loc_2_14C0
loc_2_132C:
        mov    dword ptr [esi + 1ch], eax
        jmp    NEAR PTR loc_2_14C0
loc_2_1334:
        cmp    ecx, 5
        jb     NEAR PTR loc_2_1829
        mov    eax, ebx
        shl    eax, 5
        push   dword ptr [edi + 0c0h]
        push   ebx
        db  0FFh, 090h, 014h, 000h, 000h, 000h
        test   eax, eax
        je     NEAR PTR loc_2_1829
        test   byte ptr [ebp - 2], 0c8h
        je     SHORT loc_2_1366
        mov    eax, 1
        mov    ecx, eax
        jmp    SHORT loc_2_1391
loc_2_1366:
        cmp    dword ptr [edi + 0b8h], 0
        je     SHORT loc_2_138A
        cmp    dword ptr [edi + 0ach], 0
        jne    SHORT loc_2_1381
        mov    eax, 1
        xor    ecx, ecx
        jmp    SHORT loc_2_1391
loc_2_1381:
        xor    eax, eax
        mov    ecx, 1
        jmp    SHORT loc_2_1391
loc_2_138A:
        mov    ecx, 1
        mov    eax, ecx
loc_2_1391:
        test   eax, eax
        je     SHORT loc_2_13D4
        test   byte ptr [ebp - 4], 8
        je     SHORT loc_2_13D4
        test   byte ptr [ebp - 3], 1
        db  0A1h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_13D1
        sub    eax, dword ptr [edi + 18h]
        jns    SHORT loc_2_13B2
        db  0A1h, 010h, 000h, 000h, 000h
        jmp    SHORT loc_2_13D1
loc_2_13B2:
        db  00Fh, 0AFh, 005h, 070h, 000h, 000h, 000h
        sub    edx, edx
        div    dword ptr [edi + 7ch]
        db  003h, 005h, 010h, 000h, 000h, 000h
        db  03Bh, 005h, 028h, 000h, 000h, 000h
        jbe    SHORT loc_2_13D1
        db  0A1h, 028h, 000h, 000h, 000h
loc_2_13D1:
        mov    dword ptr [esi + 14h], eax
loc_2_13D4:
        test   ecx, ecx
        je     SHORT loc_2_1420
        test   byte ptr [ebp - 4], 4
        je     SHORT loc_2_1420
        test   byte ptr [ebp - 3], 1
        je     SHORT loc_2_13EB
        db  0A1h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_141D
loc_2_13EB:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        sub    ecx, dword ptr [edi + 14h]
        jns    SHORT loc_2_13FD
        db  0A1h, 00Ch, 000h, 000h, 000h
        jmp    SHORT loc_2_141D
loc_2_13FD:
        db  0A1h, 06Ch, 000h, 000h, 000h
        sub    edx, edx
        imul   eax, ecx
        div    dword ptr [edi + 78h]
        db  003h, 005h, 00Ch, 000h, 000h, 000h
        db  03Bh, 005h, 024h, 000h, 000h, 000h
        jbe    SHORT loc_2_141D
        db  0A1h, 024h, 000h, 000h, 000h
loc_2_141D:
        mov    dword ptr [esi + 10h], eax
loc_2_1420:
        cmp    dword ptr [edi + 0c0h], 0
        je     SHORT loc_2_147E
        test   byte ptr [ebp - 4], 20h
        je     NEAR PTR loc_2_14C0
        test   byte ptr [ebp - 3], 1
        db  0A1h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_1443
        mov    dword ptr [esi + 1ch], eax
        jmp    SHORT loc_2_14C0
loc_2_1443:
        sub    eax, dword ptr [edi + 20h]
        jns    SHORT loc_2_1452
        db  0A1h, 018h, 000h, 000h, 000h
        mov    dword ptr [esi + 1ch], eax
        jmp    SHORT loc_2_14C0
loc_2_1452:
        db  00Fh, 0AFh, 005h, 078h, 000h, 000h, 000h
        sub    edx, edx
        div    dword ptr [edi + 84h]
        db  003h, 005h, 018h, 000h, 000h, 000h
        db  039h, 005h, 030h, 000h, 000h, 000h
        jae    SHORT loc_2_1479
        db  0A1h, 030h, 000h, 000h, 000h
        mov    dword ptr [esi + 1ch], eax
        jmp    SHORT loc_2_14C0
loc_2_1479:
        mov    dword ptr [esi + 1ch], eax
        jmp    SHORT loc_2_14C0
loc_2_147E:
        test   byte ptr [ebp - 4], 10h
        je     SHORT loc_2_14C0
        test   byte ptr [ebp - 3], 1
        db  0A1h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_14BD
        sub    eax, dword ptr [edi + 1ch]
        jns    SHORT loc_2_149B
        db  0A1h, 014h, 000h, 000h, 000h
        jmp    SHORT loc_2_14BD
loc_2_149B:
        db  00Fh, 0AFh, 005h, 074h, 000h, 000h, 000h
        sub    edx, edx
        div    dword ptr [edi + 80h]
        db  003h, 005h, 014h, 000h, 000h, 000h
        db  03Bh, 005h, 02Ch, 000h, 000h, 000h
        jbe    SHORT loc_2_14BD
        db  0A1h, 02Ch, 000h, 000h, 000h
loc_2_14BD:
        mov    dword ptr [esi + 18h], eax
loc_2_14C0:
        test   byte ptr [ebp - 3], 1
        je     SHORT loc_2_14EB
        test   byte ptr [ebp - 4], 1
        je     SHORT loc_2_14D4
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 8], eax
loc_2_14D4:
        test   byte ptr [ebp - 4], 2
        je     NEAR PTR loc_2_16C9
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 0ch], eax
        jmp    NEAR PTR loc_2_16C9
loc_2_14EB:
        test   byte ptr [ebp - 3], 8
        je     SHORT loc_2_1538
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_1538
        imul   ebx, ebx, 18h
        db  0A1h, 000h, 000h, 000h, 000h
        db  039h, 083h, 000h, 000h, 000h, 000h
        ja     SHORT loc_2_1538
        db  039h, 083h, 000h, 000h, 000h, 000h
        jb     SHORT loc_2_1538
        db  0A1h, 000h, 000h, 000h, 000h
        db  039h, 083h, 004h, 000h, 000h, 000h
        ja     SHORT loc_2_1538
        db  039h, 083h, 004h, 000h, 000h, 000h
        jb     SHORT loc_2_1538
        mov    eax, dword ptr [edi + 3ch]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [edi + 40h]
        db  089h, 00Dh, 000h, 000h, 000h, 000h
loc_2_1538:
        test   byte ptr [ebp - 3], 4
        jne    NEAR PTR loc_2_15CD
        test   byte ptr [ebp - 4], 1
        je     SHORT loc_2_157B
        db  0A1h, 000h, 000h, 000h, 000h
        sub    eax, dword ptr [edi + 0ch]
        jns    SHORT loc_2_1559
        db  0A1h, 004h, 000h, 000h, 000h
        jmp    SHORT loc_2_1578
loc_2_1559:
        db  00Fh, 0AFh, 005h, 064h, 000h, 000h, 000h
        sub    edx, edx
        div    dword ptr [edi + 70h]
        db  003h, 005h, 004h, 000h, 000h, 000h
        db  03Bh, 005h, 01Ch, 000h, 000h, 000h
        jbe    SHORT loc_2_1578
        db  0A1h, 01Ch, 000h, 000h, 000h
loc_2_1578:
        mov    dword ptr [esi + 8], eax
loc_2_157B:
        test   byte ptr [ebp - 4], 2
        je     NEAR PTR loc_2_16C9
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        sub    ecx, dword ptr [edi + 10h]
        jns    SHORT loc_2_159D
        db  0A1h, 008h, 000h, 000h, 000h
        mov    dword ptr [esi + 0ch], eax
        jmp    NEAR PTR loc_2_16C9
loc_2_159D:
        db  0A1h, 068h, 000h, 000h, 000h
        sub    edx, edx
        imul   eax, ecx
        div    dword ptr [edi + 74h]
        db  003h, 005h, 008h, 000h, 000h, 000h
        db  039h, 005h, 020h, 000h, 000h, 000h
        jae    SHORT loc_2_15C5
        db  0A1h, 020h, 000h, 000h, 000h
        mov    dword ptr [esi + 0ch], eax
        jmp    NEAR PTR loc_2_16C9
loc_2_15C5:
        mov    dword ptr [esi + 0ch], eax
        jmp    NEAR PTR loc_2_16C9
loc_2_15CD:
        test   byte ptr [ebp - 4], 2
        je     SHORT loc_2_164B
        mov    ecx, dword ptr [edi + 40h]
        db  08Bh, 01Dh, 068h, 000h, 000h, 000h
        shr    ebx, 1
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    ecx, eax
        jle    SHORT loc_2_1609
        mov    edx, dword ptr [edi + 10h]
        sub    eax, edx
        jns    SHORT loc_2_15F6
        db  08Bh, 01Dh, 008h, 000h, 000h, 000h
        jmp    SHORT loc_2_1636
loc_2_15F6:
        imul   eax, ebx
        sub    ecx, edx
        je     SHORT loc_2_1631
        sub    edx, edx
        db  08Bh, 01Dh, 008h, 000h, 000h, 000h
        div    ecx
        jmp    SHORT loc_2_162D
loc_2_1609:
        sub    eax, ecx
        jns    SHORT loc_2_1615
        db  003h, 01Dh, 008h, 000h, 000h, 000h
        jmp    SHORT loc_2_1636
loc_2_1615:
        imul   eax, ebx
        mov    edx, dword ptr [edi + 28h]
        sub    edx, ecx
        mov    dword ptr [ebp - 8], edx
        je     SHORT loc_2_1631
        sub    edx, edx
        db  003h, 01Dh, 008h, 000h, 000h, 000h
        div    dword ptr [ebp - 8]
loc_2_162D:
        add    ebx, eax
        jmp    SHORT loc_2_1636
loc_2_1631:
        mov    ebx, 0ffffffffh
loc_2_1636:
        db  039h, 01Dh, 020h, 000h, 000h, 000h
        jae    SHORT loc_2_1648
        db  0A1h, 020h, 000h, 000h, 000h
        mov    dword ptr [esi + 0ch], eax
        jmp    SHORT loc_2_164B
loc_2_1648:
        mov    dword ptr [esi + 0ch], ebx
loc_2_164B:
        test   byte ptr [ebp - 4], 1
        je     SHORT loc_2_16C9
        mov    ecx, dword ptr [edi + 3ch]
        db  08Bh, 01Dh, 064h, 000h, 000h, 000h
        shr    ebx, 1
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    ecx, eax
        jle    SHORT loc_2_1687
        mov    edx, dword ptr [edi + 0ch]
        sub    eax, edx
        jns    SHORT loc_2_1674
        db  08Bh, 01Dh, 004h, 000h, 000h, 000h
        jmp    SHORT loc_2_16B4
loc_2_1674:
        imul   eax, ebx
        sub    ecx, edx
        je     SHORT loc_2_16AF
        sub    edx, edx
        db  08Bh, 01Dh, 004h, 000h, 000h, 000h
        div    ecx
        jmp    SHORT loc_2_16AB
loc_2_1687:
        sub    eax, ecx
        jns    SHORT loc_2_1693
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        jmp    SHORT loc_2_16B4
loc_2_1693:
        imul   eax, ebx
        mov    edx, dword ptr [edi + 24h]
        sub    edx, ecx
        mov    dword ptr [ebp - 8], edx
        je     SHORT loc_2_16AF
        sub    edx, edx
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        div    dword ptr [ebp - 8]
loc_2_16AB:
        add    ebx, eax
        jmp    SHORT loc_2_16B4
loc_2_16AF:
        mov    ebx, 0ffffffffh
loc_2_16B4:
        db  039h, 01Dh, 01Ch, 000h, 000h, 000h
        jae    SHORT loc_2_16C6
        db  0A1h, 01Ch, 000h, 000h, 000h
        mov    dword ptr [esi + 8], eax
        jmp    SHORT loc_2_16C9
loc_2_16C6:
        mov    dword ptr [esi + 8], ebx
loc_2_16C9:
        cmp    dword ptr [edi + 0b4h], 0
        je     NEAR PTR loc_2_17C4
        test   dword ptr [ebp - 4], 240h
        je     NEAR PTR loc_2_17C4
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 0FFh
        je     SHORT loc_2_16F7
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_2_17C1
loc_2_16F7:
        cmp    dword ptr [edi + 0b8h], 0
        mov    ecx, 0ffffh
        je     SHORT loc_2_1776
        cmp    dword ptr [edi + 0ach], 0
        jne    SHORT loc_2_1715
        db  0A1h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_171A
loc_2_1715:
        db  0A1h, 000h, 000h, 000h, 000h
loc_2_171A:
        cmp    dword ptr [edi + 88h], eax
        jae    SHORT loc_2_1731
        cmp    dword ptr [edi + 98h], eax
        jbe    SHORT loc_2_1731
        xor    ecx, ecx
        jmp    NEAR PTR loc_2_17C1
loc_2_1731:
        cmp    dword ptr [edi + 8ch], eax
        jae    SHORT loc_2_1748
        cmp    dword ptr [edi + 9ch], eax
        jbe    SHORT loc_2_1748
        mov    ecx, 4650h
        jmp    SHORT loc_2_17C1
loc_2_1748:
        cmp    dword ptr [edi + 90h], eax
        jae    SHORT loc_2_175F
        cmp    dword ptr [edi + 0a0h], eax
        jbe    SHORT loc_2_175F
        mov    ecx, 6978h
        jmp    SHORT loc_2_17C1
loc_2_175F:
        cmp    dword ptr [edi + 94h], eax
        jae    SHORT loc_2_17C1
        cmp    dword ptr [edi + 0a4h], eax
        jbe    SHORT loc_2_17C1
        mov    ecx, 2328h
        jmp    SHORT loc_2_17C1
loc_2_1776:
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_2_1784
        mov    eax, 0ffffffffh
loc_2_1784:
        cmp    dword ptr [edi + 54h], eax
        jne    SHORT loc_2_178D
        xor    ecx, ecx
        jmp    SHORT loc_2_17AF
loc_2_178D:
        cmp    dword ptr [edi + 58h], eax
        jne    SHORT loc_2_1799
        mov    ecx, 4650h
        jmp    SHORT loc_2_17AF
loc_2_1799:
        cmp    dword ptr [edi + 5ch], eax
        jne    SHORT loc_2_17A5
        mov    ecx, 6978h
        jmp    SHORT loc_2_17AF
loc_2_17A5:
        cmp    dword ptr [edi + 60h], eax
        jne    SHORT loc_2_17AF
        mov    ecx, 2328h
loc_2_17AF:
        cmp    ecx, 0ffffh
        je     SHORT loc_2_17C1
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_17C1:
        mov    dword ptr [esi + 28h], ecx
loc_2_17C4:
        test   byte ptr [ebp - 4], 80h
        je     SHORT loc_2_1825
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 20h], eax
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_181D
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 001h
        je     SHORT loc_2_17ED
        mov    dword ptr [esi + 24h], 1
        jmp    SHORT loc_2_1825
loc_2_17ED:
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 002h
        je     SHORT loc_2_17FF
        mov    dword ptr [esi + 24h], 2
        jmp    SHORT loc_2_1825
loc_2_17FF:
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 004h
        je     SHORT loc_2_1811
        mov    dword ptr [esi + 24h], 3
        jmp    SHORT loc_2_1825
loc_2_1811:
        db  0A1h, 000h, 000h, 000h, 000h
        and    eax, 8
        shr    eax, 1
        jmp    SHORT loc_2_1822
loc_2_181D:
        db  0A1h, 000h, 000h, 000h, 000h
loc_2_1822:
        mov    dword ptr [esi + 24h], eax
loc_2_1825:
        xor    eax, eax
        jmp    SHORT loc_2_182E
loc_2_1829:
        mov    eax, 0a7h
loc_2_182E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_2_1837:
        mov    ecx, dword ptr [esp + 4]
        push   esi
        cmp    ecx, 10h
        jl     SHORT loc_2_1845
        xor    eax, eax
        jmp    SHORT loc_2_18AF
loc_2_1845:
        mov    eax, ecx
        shl    eax, 5
        db  083h, 0B8h, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_1890
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 002h
        jne    SHORT loc_2_1874
        cmp    ecx, 1
        jne    SHORT loc_2_186E
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 002h
        jbe    SHORT loc_2_1890
        xor    eax, eax
        jmp    SHORT loc_2_18AF
loc_2_186E:
        jle    SHORT loc_2_1890
        xor    eax, eax
        jmp    SHORT loc_2_18AF
loc_2_1874:
        cmp    ecx, 3
        jle    SHORT loc_2_187D
        xor    eax, eax
        jmp    SHORT loc_2_18AF
loc_2_187D:
        cmp    ecx, 1
        jle    SHORT loc_2_1890
        db  083h, 03Ch, 08Dh, 000h, 000h, 000h, 000h, 002h
        jbe    SHORT loc_2_1890
        xor    eax, eax
        jmp    SHORT loc_2_18AF
loc_2_1890:
        mov    eax, ecx
        imul   eax, eax, 0c8h
        db  083h, 0B8h, 040h, 001h, 000h, 000h, 000h
        jne    SHORT loc_2_18A5
        xor    eax, eax
        jmp    SHORT loc_2_18AF
loc_2_18A5:
        db  083h, 0B8h, 0E4h, 000h, 000h, 000h, 001h
        sbb    eax, eax
        inc    eax
loc_2_18AF:
        test   eax, eax
        jne    SHORT loc_2_18BD
        mov    eax, 0a5h
        jmp    NEAR PTR loc_2_19EC
loc_2_18BD:
        mov    eax, ecx
        imul   eax, eax, 0c8h
        db  08Dh, 0B0h, 07Ch, 000h, 000h, 000h
        db  08Bh, 014h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx*4]
        db  089h, 015h, 000h, 000h, 000h, 000h
        cmp    dword ptr [esi + 0ach], 0
        je     SHORT loc_2_1954
        db  083h, 0B8h, 000h, 000h, 000h, 000h, 003h
        jb     SHORT loc_2_194A
        push   0
        push   ecx
        shl    ecx, 5
        db  0FFh, 091h, 00Ch, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_2_194A
        db  0A1h, 000h, 000h, 000h, 000h
        sub    eax, dword ptr [esi + 14h]
        jns    SHORT loc_2_1919
        db  0A1h, 00Ch, 000h, 000h, 000h
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [ecx + 8], eax
        jmp    SHORT loc_2_1977
loc_2_1919:
        db  00Fh, 0AFh, 005h, 06Ch, 000h, 000h, 000h
        sub    edx, edx
        div    dword ptr [esi + 78h]
        db  003h, 005h, 00Ch, 000h, 000h, 000h
        db  03Bh, 005h, 024h, 000h, 000h, 000h
        jbe    SHORT loc_2_1941
        db  0A1h, 024h, 000h, 000h, 000h
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [ecx + 8], eax
        jmp    SHORT loc_2_1977
loc_2_1941:
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [ecx + 8], eax
        jmp    SHORT loc_2_1977
loc_2_194A:
        mov    eax, 0a7h
        jmp    NEAR PTR loc_2_19EC
loc_2_1954:
        push   ecx
        shl    ecx, 5
        db  0FFh, 091h, 008h, 000h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_2_196C
        mov    eax, 0a7h
        jmp    NEAR PTR loc_2_19EC
loc_2_196C:
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [ecx + 8], 0
loc_2_1977:
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        sub    edx, dword ptr [esi + 0ch]
        jns    SHORT loc_2_1989
        db  0A1h, 004h, 000h, 000h, 000h
        jmp    SHORT loc_2_19A9
loc_2_1989:
        db  0A1h, 064h, 000h, 000h, 000h
        imul   eax, edx
        sub    edx, edx
        div    dword ptr [esi + 70h]
        db  003h, 005h, 004h, 000h, 000h, 000h
        db  039h, 005h, 01Ch, 000h, 000h, 000h
        jae    SHORT loc_2_19A9
        db  0A1h, 01Ch, 000h, 000h, 000h
loc_2_19A9:
        mov    dword ptr [ecx], eax
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        sub    edx, dword ptr [esi + 10h]
        jns    SHORT loc_2_19BD
        db  0A1h, 008h, 000h, 000h, 000h
        jmp    SHORT loc_2_19DD
loc_2_19BD:
        db  0A1h, 068h, 000h, 000h, 000h
        imul   eax, edx
        sub    edx, edx
        div    dword ptr [esi + 74h]
        db  003h, 005h, 008h, 000h, 000h, 000h
        db  03Bh, 005h, 020h, 000h, 000h, 000h
        jbe    SHORT loc_2_19DD
        db  0A1h, 020h, 000h, 000h, 000h
loc_2_19DD:
        mov    dword ptr [ecx + 4], eax
        db  00Fh, 0B7h, 005h, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 0ch], eax
        xor    eax, eax
loc_2_19EC:
        pop    esi
        ret    8
sub_2_19F0:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 004h
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        jne    SHORT loc_2_1A4A
        test   esi, esi
        je     SHORT loc_2_1A1A
        cmp    esi, 1
        je     SHORT loc_2_1A26
        cmp    esi, 2
        je     SHORT loc_2_1A32
        cmp    esi, 3
        je     SHORT loc_2_1A3E
        jmp    NEAR PTR loc_2_1D3C
loc_2_1A1A:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 000h
        jmp    SHORT loc_2_1A6E
loc_2_1A26:
        db  0C7h, 005h, 004h, 000h, 000h, 000h, 004h, 000h, 000h, 000h
        jmp    SHORT loc_2_1A6E
loc_2_1A32:
        db  0C7h, 005h, 008h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        jmp    SHORT loc_2_1A6E
loc_2_1A3E:
        db  0C7h, 005h, 00Ch, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        jmp    SHORT loc_2_1A6E
loc_2_1A4A:
        test   esi, esi
        je     SHORT loc_2_1A58
        cmp    esi, 1
        je     SHORT loc_2_1A64
        jmp    NEAR PTR loc_2_1D3C
loc_2_1A58:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 000h
        jmp    SHORT loc_2_1A6E
loc_2_1A64:
        db  0C7h, 005h, 004h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
loc_2_1A6E:
        db  08Dh, 014h, 0B5h, 000h, 000h, 000h, 000h
        mov    eax, esi
        imul   eax, eax, 0c8h
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 000h
        db  08Dh, 0B8h, 07Ch, 000h, 000h, 000h
        mov    dword ptr [edx], 2
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 000h
        mov    ebx, dword ptr [edi]
        xor    eax, eax
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 001h
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 002h
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 001h
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 004h
        test   esi, esi
        jne    SHORT loc_2_1ADC
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 004h
        jne    SHORT loc_2_1AD3
        db  039h, 005h, 0D0h, 002h, 000h, 000h
        jmp    SHORT loc_2_1AE8
loc_2_1AD3:
        db  083h, 03Dh, 008h, 002h, 000h, 000h, 000h
        jmp    SHORT loc_2_1AE8
loc_2_1ADC:
        cmp    esi, 1
        jne    SHORT loc_2_1AEF
        db  083h, 03Dh, 098h, 003h, 000h, 000h, 000h
loc_2_1AE8:
        jne    SHORT loc_2_1AEF
        mov    eax, 1
loc_2_1AEF:
        test   eax, eax
        je     SHORT loc_2_1B6D
        test   bh, 1
        je     SHORT loc_2_1B08
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 004h
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 002h
        jmp    SHORT loc_2_1B30
loc_2_1B08:
        test   bh, 2
        je     SHORT loc_2_1B1D
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 008h
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 003h
        jmp    SHORT loc_2_1B30
loc_2_1B1D:
        test   bl, 80h
        je     SHORT loc_2_1B30
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 002h
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 001h
loc_2_1B30:
        test   bh, 8
        je     SHORT loc_2_1B45
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 004h
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 002h
        jmp    SHORT loc_2_1B6D
loc_2_1B45:
        test   bh, 10h
        je     SHORT loc_2_1B5A
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 008h
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 003h
        jmp    SHORT loc_2_1B6D
loc_2_1B5A:
        test   bh, 4
        je     SHORT loc_2_1B6D
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 001h
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 000h
loc_2_1B6D:
        cmp    dword ptr [edi + 0b8h], 0
        je     SHORT loc_2_1BCD
        test   ebx, 40000h
        mov    dword ptr [edx], 3
        je     SHORT loc_2_1B8A
        mov    al, 4
        mov    cl, 2
        jmp    SHORT loc_2_1BAA
loc_2_1B8A:
        test   ebx, 10000h
        je     SHORT loc_2_1B98
        mov    al, 1
        xor    cl, cl
        jmp    SHORT loc_2_1BAA
loc_2_1B98:
        test   ebx, 20000h
        je     SHORT loc_2_1BA6
        mov    al, 2
        mov    cl, 1
        jmp    SHORT loc_2_1BAA
loc_2_1BA6:
        mov    al, 8
        mov    cl, 3
loc_2_1BAA:
        cmp    dword ptr [edi + 0ach], 0
        jne    SHORT loc_2_1BC1
        db  088h, 086h, 000h, 000h, 000h, 000h
        db  088h, 08Eh, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_1BCD
loc_2_1BC1:
        db  088h, 086h, 000h, 000h, 000h, 000h
        db  088h, 08Eh, 000h, 000h, 000h, 000h
loc_2_1BCD:
        cmp    dword ptr [edi + 0ach], 0
        je     SHORT loc_2_1C2E
        db  080h, 0BEh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_1C2E
        inc    dword ptr [edx]
        test   bh, 80h
        je     SHORT loc_2_1BF6
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 004h
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 002h
        jmp    SHORT loc_2_1C2E
loc_2_1BF6:
        test   bh, 20h
        je     SHORT loc_2_1C0B
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 001h
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_1C2E
loc_2_1C0B:
        test   bh, 40h
        je     SHORT loc_2_1C20
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 002h
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 001h
        jmp    SHORT loc_2_1C2E
loc_2_1C20:
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 008h
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 003h
loc_2_1C2E:
        cmp    dword ptr [edi + 0b0h], 0
        je     SHORT loc_2_1C98
        db  080h, 0BEh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_1C98
        inc    dword ptr [edx]
        test   ebx, 400000h
        je     SHORT loc_2_1C5A
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 008h
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 003h
        jmp    SHORT loc_2_1C98
loc_2_1C5A:
        test   ebx, 100000h
        je     SHORT loc_2_1C72
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 001h
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_1C98
loc_2_1C72:
        test   ebx, 200000h
        je     SHORT loc_2_1C8A
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 002h
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 001h
        jmp    SHORT loc_2_1C98
loc_2_1C8A:
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 004h
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 002h
loc_2_1C98:
        cmp    dword ptr [edi + 0bch], 0
        je     SHORT loc_2_1CA3
        inc    dword ptr [edx]
loc_2_1CA3:
        cmp    dword ptr [edi + 0c0h], 0
        je     SHORT loc_2_1CAE
        inc    dword ptr [edx]
loc_2_1CAE:
        test   esi, esi
        je     SHORT loc_2_1CF1
        cmp    esi, 1
        jne    SHORT loc_2_1CC0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 004h
        je     SHORT loc_2_1CF1
loc_2_1CC0:
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 004h
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 008h
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 002h
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 003h
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 086h, 000h, 000h, 000h, 000h, 006h
loc_2_1CF1:
        db  08Ah, 08Eh, 000h, 000h, 000h, 000h
        db  08Ah, 086h, 000h, 000h, 000h, 000h
        db  08Ah, 096h, 000h, 000h, 000h, 000h
        mov    bl, cl
        or     bl, al
        or     bl, dl
        db  00Ah, 096h, 000h, 000h, 000h, 000h
        db  00Ah, 09Eh, 000h, 000h, 000h, 000h
        db  088h, 096h, 000h, 000h, 000h, 000h
        db  088h, 09Eh, 000h, 000h, 000h, 000h
        cmp    dword ptr [edi + 0b0h], 0
        je     SHORT loc_2_1D34
        or     al, dl
        db  088h, 086h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_1D3C
loc_2_1D34:
        or     cl, dl
        db  088h, 08Eh, 000h, 000h, 000h, 000h
loc_2_1D3C:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_1D41:
        mov    edx, dword ptr [esp + 4]
        sub    esp, 0ch
        push   esi
        db  08Bh, 004h, 095h, 000h, 000h, 000h, 000h
        lea    esi, [edx*4]
        test   eax, eax
        je     SHORT loc_2_1D82
        mov    eax, dword ptr [eax + 0ch]
        test   eax, eax
        je     SHORT loc_2_1D82
        lea    ecx, [esp + 4]
        push   ecx
        push   edx
        call   eax
        test   eax, eax
        je     SHORT loc_2_1D82
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 8]
        db  089h, 086h, 000h, 000h, 000h, 000h
        db  089h, 08Eh, 000h, 000h, 000h, 000h
loc_2_1D82:
        pop    esi
        add    esp, 0ch
        ret
sub_2_1D87:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [esp + 4]
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  08Bh, 00Ch, 085h, 000h, 000h, 000h, 000h
        test   ecx, ecx
        je     SHORT loc_2_1DC1
        test   edx, edx
        je     SHORT loc_2_1DC5
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [ecx + 8], edx
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ecx + 14h], eax
loc_2_1DC1:
        test   edx, edx
        jne    SHORT loc_2_1E2A
loc_2_1DC5:
        db  0A1h, 000h, 000h, 000h, 000h
        shl    eax, 5
        db  0C7h, 080h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 080h, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 080h, 00Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 080h, 010h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 080h, 014h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 080h, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 080h, 01Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_19F0
        jmp    SHORT loc_2_1E8D
loc_2_1E2A:
        db  0A1h, 000h, 000h, 000h, 000h
        shl    eax, 5
        db  0C7h, 080h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 080h, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 080h, 00Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 080h, 010h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 080h, 014h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 080h, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 080h, 01Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 080h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_1D41
loc_2_1E8D:
        add    esp, 4
        ret    10h
sub_2_1E93:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0ch
        cmp    ecx, 10h
        push   esi
        push   edi
        jl     SHORT loc_2_1EA8
        xor    eax, eax
        jmp    SHORT loc_2_1F12
loc_2_1EA8:
        mov    eax, ecx
        shl    eax, 5
        db  083h, 0B8h, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_1EF3
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 002h
        jne    SHORT loc_2_1ED7
        cmp    ecx, 1
        jne    SHORT loc_2_1ED1
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 002h
        jbe    SHORT loc_2_1EF3
        xor    eax, eax
        jmp    SHORT loc_2_1F12
loc_2_1ED1:
        jle    SHORT loc_2_1EF3
        xor    eax, eax
        jmp    SHORT loc_2_1F12
loc_2_1ED7:
        cmp    ecx, 3
        jle    SHORT loc_2_1EE0
        xor    eax, eax
        jmp    SHORT loc_2_1F12
loc_2_1EE0:
        cmp    ecx, 1
        jle    SHORT loc_2_1EF3
        db  083h, 03Ch, 08Dh, 000h, 000h, 000h, 000h, 002h
        jbe    SHORT loc_2_1EF3
        xor    eax, eax
        jmp    SHORT loc_2_1F12
loc_2_1EF3:
        mov    eax, ecx
        imul   eax, eax, 0c8h
        db  083h, 0B8h, 040h, 001h, 000h, 000h, 000h
        jne    SHORT loc_2_1F08
        xor    eax, eax
        jmp    SHORT loc_2_1F12
loc_2_1F08:
        db  083h, 0B8h, 0E4h, 000h, 000h, 000h, 001h
        sbb    eax, eax
        inc    eax
loc_2_1F12:
        test   eax, eax
        je     NEAR PTR loc_2_200A
        db  08Dh, 034h, 08Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     SHORT loc_2_1F7B
        mov    eax, dword ptr [eax + 0ch]
        test   eax, eax
        je     SHORT loc_2_1F7B
        lea    edx, [ebp - 0ch]
        push   edx
        push   ecx
        call   eax
        test   eax, eax
        je     NEAR PTR loc_2_200A
        mov    eax, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + 4], ecx
        mov    ecx, 0ffffffffh
        mov    dword ptr [edx + 8], eax
        mov    esi, dword ptr [esi]
        sub    eax, eax
        mov    edi, dword ptr [esi + 4]
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        lea    edi, [edx + 0ch]
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        jmp    NEAR PTR loc_2_2003
loc_2_1F7B:
        mov    edx, dword ptr [ebp + 0ch]
        test   ecx, ecx
        mov    byte ptr [edx + 0ch], 0
        jne    SHORT loc_2_1F9A
        mov    dword ptr [edx + 4], 4
        mov    dword ptr [edx], 4
        db  0A1h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_2000
loc_2_1F9A:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 004h
        jne    SHORT loc_2_1FE9
        cmp    ecx, 1
        jne    SHORT loc_2_1FB7
        mov    dword ptr [edx + 4], 4
        mov    dword ptr [edx], 4
        jmp    SHORT loc_2_1FFB
loc_2_1FB7:
        cmp    ecx, 2
        jne    SHORT loc_2_1FD0
        mov    dword ptr [edx + 4], 2
        mov    dword ptr [edx], 2
        db  0A1h, 008h, 000h, 000h, 000h
        jmp    SHORT loc_2_2000
loc_2_1FD0:
        cmp    ecx, 3
        jne    SHORT loc_2_200A
        mov    dword ptr [edx + 4], 2
        mov    dword ptr [edx], 2
        db  0A1h, 00Ch, 000h, 000h, 000h
        jmp    SHORT loc_2_2000
loc_2_1FE9:
        cmp    ecx, 1
        jne    SHORT loc_2_200A
        mov    dword ptr [edx + 4], 2
        mov    dword ptr [edx], 2
loc_2_1FFB:
        db  0A1h, 004h, 000h, 000h, 000h
loc_2_2000:
        mov    dword ptr [edx + 8], eax
loc_2_2003:
        mov    eax, 1
        jmp    SHORT loc_2_200C
loc_2_200A:
        xor    eax, eax
loc_2_200C:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
sub_2_2014:
        push   esi
        push   edi
        db  0BFh, 000h, 000h, 000h, 000h
        call   sub_2_0096
        mov    esi, dword ptr [esp + 0ch]
        mov    ecx, 33fh
        rep movsd
        db  0A1h, 000h, 000h, 000h, 000h
        xor    esi, esi
        db  0BFh, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
loc_2_203C:
        push   esi
        cmp    dword ptr [edi], 0
        jne    SHORT loc_2_2049
        call   sub_2_19F0
        jmp    SHORT loc_2_204E
loc_2_2049:
        call   sub_2_1D41
loc_2_204E:
        add    esp, 4
        add    edi, 20h
        inc    esi
        db  081h, 0FFh, 000h, 002h, 000h, 000h
        jb     SHORT loc_2_203C
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 04Ch, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_207D
        db  083h, 03Dh, 050h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_2_20FD
loc_2_207D:
        xor    edi, edi
        db  0BEh, 0B8h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
loc_2_208E:
        mov    eax, dword ptr [esi - 18h]
        mov    ecx, 64h
        sub    eax, dword ptr [esi - 30h]
        sub    edx, edx
        db  00Fh, 0AFh, 005h, 04Ch, 000h, 000h, 000h
        div    ecx
        mov    ecx, dword ptr [esi]
        add    edi, 18h
        mov    edx, ecx
        add    esi, 0c8h
        sub    edx, eax
        add    eax, ecx
        mov    ecx, 64h
        db  089h, 097h, 0E8h, 0FFh, 0FFh, 0FFh
        sub    edx, edx
        db  089h, 087h, 0E8h, 0FFh, 0FFh, 0FFh
        mov    eax, dword ptr [esi - 0dch]
        sub    eax, dword ptr [esi - 0f4h]
        db  00Fh, 0AFh, 005h, 050h, 000h, 000h, 000h
        div    ecx
        mov    ecx, dword ptr [esi - 0c4h]
        mov    edx, ecx
        sub    edx, eax
        add    eax, ecx
        db  089h, 097h, 0ECh, 0FFh, 0FFh, 0FFh
        db  089h, 087h, 0ECh, 0FFh, 0FFh, 0FFh
        db  081h, 0FEh, 038h, 00Dh, 000h, 000h
        jb     SHORT loc_2_208E
loc_2_20FD:
        pop    edi
        pop    esi
        ret    4
        db  0CCh, 0CCh
loc_2_2104:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_2_06D3
        add    esp, 14h
        clc
        leave
        ret
loc_2_2121:
        push   ebx
        mov    eax, ebx
        db  0BBh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 04Ah, 004h
        call   sub_2_04A4
        mov    eax, 0
        clc
        ret
vjoyd_Service_00:
        push   edx
        push   ecx
        push   ebx
        push   eax
        call   sub_2_1D87
        clc
        ret
vjoyd_Service_01:
        push   eax
        push   ebx
        call   sub_2_0C70
        clc
        ret
loc_2_214F:
        cmp    ecx, 3
        jb     SHORT loc_2_215D
        mov    eax, 0ffffffffh
        ret
loc_2_215D:
        push   edi
        push   esi
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_2_2166:
        mov    edi, dword ptr [esi + 20h]
        mov    esi, dword ptr [esi + 18h]
        mov    dword ptr [esi], 100h
        or     edi, edi
        je     SHORT loc_2_217C
        mov    dword ptr [edi], 4
loc_2_217C:
        pop    esi
        pop    edi
        xor    eax, eax
        ret
loc_2_2181:
        mov    edi, dword ptr [esi + 18h]
        push   edi
        push   dword ptr [edi + 2ch]
        call   sub_2_0C70
        mov    dword ptr [edi + 2ch], eax
        jmp    SHORT loc_2_217C
loc_2_2192:
        mov    eax, dword ptr [esi + 10h]
        mov    ecx, dword ptr [esi + 18h]
        push   ecx
        push   dword ptr [eax]
        call   sub_2_1837
        mov    edi, dword ptr [esi + 20h]
        mov    dword ptr [edi], eax
        jmp    SHORT loc_2_217C
vjoyd_PM_API:
        movzx  eax, word ptr [ebp + 1ch]
        cmp    eax, 5
        ja     SHORT loc_2_21BA
        db  0FFh, 014h, 085h, 000h, 000h, 000h, 000h
        ret
loc_2_21BA:
        xor    eax, eax
        mov    dword ptr [ebp + 1ch], eax
        ret
loc_2_21C0:
        mov    ah, 1
        mov    al, 0
        mov    word ptr [ebp + 1ch], ax
        ret
loc_2_21C9:
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        movzx  edx, word ptr [ebp + 14h]
        push   edi
        push   edx
        call   sub_2_0C70
        mov    dword ptr [edi + 2ch], eax
        mov    word ptr [ebp + 1ch], ax
        shr    eax, 10h
        mov    word ptr [ebp + 14h], ax
        ret
loc_2_21EE:
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        movzx  edx, word ptr [ebp + 14h]
        push   edi
        push   edx
        call   sub_2_1E93
        mov    word ptr [ebp + 1ch], ax
        shr    eax, 10h
        mov    word ptr [ebp + 14h], ax
        ret
loc_2_2210:
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        movzx  edx, word ptr [ebp + 14h]
        push   edi
        push   edx
        call   sub_2_1837
        mov    word ptr [ebp + 1ch], ax
        shr    eax, 10h
        mov    word ptr [ebp + 14h], ax
        ret
loc_2_2232:
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        push   edi
        call   sub_2_2014
        mov    word ptr [ebp + 1ch], 1
        ret
        db  0CCh, 0CCh
loc_2_224C:
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        in     al, dx
        shr    al, 4
        mov    edx, 0fh
        cmp    dword ptr [esp + 4], 0
        je     SHORT loc_2_226A
        shr    al, 2
        mov    edx, 3
loc_2_226A:
        and    eax, edx
        xor    eax, edx
        db  0A3h, 000h, 000h, 000h, 000h
        ret    4
        db  0CCh, 0CCh
sub_2_2278:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        int3
        int3
sub_2_2280:
        pop    ecx
        pop    eax
        pop    edx
        push   ebx
        mov    ebx, eax
        db  0CDh, 020h, 002h, 000h, 027h, 000h
        sbb    edx, edx
        and    eax, edx
        pop    ebx
        jmp    ecx
        db  0CCh
sub_2_2294:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        int3
        int3
sub_2_229C:
        mov    eax, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 0ch]
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        jb     SHORT loc_2_22BA
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        xor    eax, eax
loc_2_22BA:
        ret    10h
        db  0CCh, 0CCh, 0CCh
sub_2_22C0:
        db  0CDh, 020h, 051h, 081h, 001h, 000h
        int3
        int3
sub_2_22C8:
        db  0CDh, 020h, 074h, 081h, 001h, 000h
        int3
        int3
sub_2_22D0:
        db  0CDh, 020h, 048h, 081h, 001h, 000h
        int3
        int3
sub_2_22D8:
        db  0CDh, 020h, 04Ch, 081h, 001h, 000h
        int3
        int3
sub_2_22E0:
        db  0CDh, 020h, 03Bh, 080h, 033h, 000h
_PTEXT ends
        end
