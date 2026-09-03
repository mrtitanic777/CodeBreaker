; vdd_pcode_08.asm -- VDD object 8 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_8_0000:
        mov    eax, dword ptr [ebp + 1ch]
        mov    ebx, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 18h]
        mov    edx, dword ptr [ebp + 14h]
        stc
        ret
        db  000h
sub_8_000F:
        pushad
        mov    ecx, 5
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        mov    edx, 21h
        mov    ecx, 0
        db  0CDh, 020h, 030h, 001h, 001h, 000h
        db  0A2h, 00Eh, 000h, 000h, 000h
        or     al, 0ffh
        mov    edx, 21h
        mov    ecx, 4
        db  0CDh, 020h, 030h, 001h, 001h, 000h
        db  0BEh, 000h, 000h, 000h, 000h
        mov    eax, 1ah
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        popad
        ret
sub_8_0054:
        pushad
        db  0BEh, 000h, 000h, 000h, 000h
        mov    eax, 1ah
        db  0CDh, 020h, 018h, 001h, 001h, 000h
        db  0A0h, 00Eh, 000h, 000h, 000h
        mov    edx, 21h
        mov    ecx, 4
        db  0CDh, 020h, 030h, 001h, 001h, 000h
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        popad
        ret
sub_8_0082:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        xor    edx, edx
        mov    eax, 0f0000h
loc_8_0095:
        cmp    byte ptr [edx + eax], 0cfh
        je     SHORT loc_8_009E
        inc    edx
        jmp    SHORT loc_8_0095
loc_8_009E:
        shl    eax, 0ch
        or     eax, edx
        mov    edx, 40h
        mov    dword ptr [edx], eax
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    edi, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp + 0ch]
        xchg   ah, al
        mov    dword ptr [edi + 1ch], eax
        mov    dword ptr [edi + 10h], 0
        mov    dword ptr [edi + 18h], 0
        mov    dword ptr [edi + 14h], 0
        mov    dword ptr [edi + 4], 0
        mov    dword ptr [edi], 0
        mov    dword ptr [edi + 8], 0
        mov    word ptr [edi + 40h], 0
        mov    word ptr [edi + 44h], 0
        mov    ecx, dword ptr [ebp + 8]
        shr    ecx, 4
        mov    edx, 3
        mov    word ptr [edi + 38h], cx
        mov    word ptr [edi + 3ch], cx
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_8_0138:
        push   ebx
        push   esi
        push   edi
        db  066h, 0F7h, 005h, 01Bh, 00Bh, 000h, 000h, 040h, 000h
        je     SHORT loc_8_0169
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 38h], 0
        je     SHORT loc_8_0163
        call   dword ptr [edi + 38h]
loc_8_0163:
        pop    edi
        db  0E8h, 08Bh, 004h, 0F8h, 0FFh
loc_8_0169:
        db  080h, 00Dh, 0A0h, 007h, 000h, 000h, 002h
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_8_0174:
        push   ebx
        push   esi
        push   edi
        db  066h, 0F7h, 005h, 01Bh, 00Bh, 000h, 000h, 040h, 000h
        je     SHORT loc_8_01A5
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 34h], 0
        je     SHORT loc_8_019F
        call   dword ptr [edi + 34h]
loc_8_019F:
        pop    edi
        db  0E8h, 085h, 004h, 0F8h, 0FFh
loc_8_01A5:
        db  080h, 025h, 0A0h, 007h, 000h, 000h, 0FDh
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_8_01B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        or     esi, esi
        jne    SHORT loc_8_01C4
        db  08Bh, 035h, 0B4h, 010h, 000h, 000h
loc_8_01C4:
        db  0A1h, 0ACh, 010h, 000h, 000h
        cmp    eax, esi
        jne    SHORT loc_8_01D2
        jmp    NEAR PTR loc_8_031D
loc_8_01D2:
        and    dword ptr [eax + 0ch], 0ffffff7fh
        add    eax, 2ch
        push   eax
        db  0E8h, 01Eh, 0FEh, 0F7h, 0FFh
        add    esp, 4
        db  089h, 035h, 0ACh, 010h, 000h, 000h
        or     dword ptr [esi + 0ch], 80h
        lea    eax, [esi + 2ch]
        push   eax
        db  0E8h, 06Eh, 0FEh, 0F7h, 0FFh
        add    esp, 4
        db  0A1h, 0ACh, 010h, 000h, 000h
        db  03Bh, 005h, 0B4h, 010h, 000h, 000h
        jne    NEAR PTR loc_8_0290
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 000h, 000h, 001h, 000h
        mov    word ptr [ebp - 4], ax
        cmp    word ptr [ebp - 4], 40ah
        push   0
        jb     SHORT loc_8_0274
        db  0FFh, 035h, 02Ch, 000h, 000h, 000h
        push   0a0h
        db  0E8h, 071h, 016h, 0F8h, 0FFh
        push   eax
        db  0A1h, 028h, 000h, 000h, 000h
        shr    eax, 0ch
        push   eax
        db  0E8h, 0DAh, 016h, 0F8h, 0FFh
        add    esp, 14h
        push   40000000h
        db  0FFh, 035h, 02Ch, 000h, 000h, 000h
        db  0A1h, 028h, 000h, 000h, 000h
        shr    eax, 0ch
        push   eax
        db  0E8h, 07Eh, 016h, 0F8h, 0FFh
        add    esp, 0ch
        db  0C7h, 005h, 028h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_8_031D
loc_8_0274:
        push   30h
        push   0a0h
        db  0E8h, 028h, 016h, 0F8h, 0FFh
        push   eax
        push   0a0h
        db  0E8h, 08Dh, 016h, 0F8h, 0FFh
        jmp    NEAR PTR loc_8_031A
loc_8_0290:
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 000h, 000h, 001h, 000h
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [ebp - 2], 40ah
        jb     SHORT loc_8_02D9
        mov    eax, dword ptr [esi + 24h]
        add    eax, 0fffh
        shr    eax, 0ch
        add    eax, 20h
        db  0A3h, 02Ch, 000h, 000h, 000h
        push   40000000h
        push   eax
        push   0a0h
        db  0E8h, 00Fh, 016h, 0F8h, 0FFh
        add    esp, 0ch
        db  0A3h, 028h, 000h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_8_02D9
        sub    eax, eax
        jmp    SHORT loc_8_0322
loc_8_02D9:
        push   0
        push   20h
        push   0a0h
        db  0E8h, 0C1h, 015h, 0F8h, 0FFh
        push   eax
        push   0a0h
        db  0E8h, 026h, 016h, 0F8h, 0FFh
        add    esp, 14h
        push   0
        mov    eax, dword ptr [esi + 24h]
        add    eax, 0fffh
        shr    eax, 0ch
        push   eax
        push   0c0h
        db  0E8h, 09Bh, 015h, 0F8h, 0FFh
        push   eax
        mov    eax, dword ptr [esi + 28h]
        shr    eax, 0ch
        push   eax
        db  0E8h, 006h, 016h, 0F8h, 0FFh
loc_8_031A:
        add    esp, 14h
loc_8_031D:
        mov    eax, 1
loc_8_0322:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_8_0327:
        push   ebp
        mov    ebp, esp
        sub    esp, 5ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 50h
        lea    eax, [ebp - 8]
        mov    esi, dword ptr [ebp + 8]
        push   1
        push   eax
        lea    eax, [ebp - 5ch]
        push   eax
        push   1
        db  068h, 0F0h, 001h, 000h, 000h
        db  068h, 0F8h, 001h, 000h, 000h
        push   dword ptr [esi + 4]
        db  0E8h, 038h, 016h, 0F8h, 0FFh
        add    esp, 1ch
        or     eax, eax
        jne    SHORT loc_8_036A
        cmp    byte ptr [ebp - 5ch], 30h
        jne    SHORT loc_8_036A
        jmp    NEAR PTR loc_8_0590
loc_8_036A:
        mov    dword ptr [ebp - 8], 50h
        mov    byte ptr [ebp - 5ch], 0
        push   1
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 5ch]
        push   eax
        push   1
        db  068h, 000h, 002h, 000h, 000h
        db  068h, 008h, 002h, 000h, 000h
        push   dword ptr [esi + 4]
        db  0E8h, 0FDh, 015h, 0F8h, 0FFh
        add    esp, 1ch
        or     eax, eax
        jne    NEAR PTR loc_8_0590
        cmp    byte ptr [ebp - 5ch], 0
        je     NEAR PTR loc_8_0590
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 000h, 000h, 001h, 000h
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [ebp - 2], 40ah
        jae    SHORT loc_8_03E6
        push   10000h
        push   0c0000h
        db  0E8h, 0ACh, 009h, 0F8h, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_8_03E6
        db  068h, 010h, 002h, 000h, 000h
        db  0E8h, 008h, 003h, 0F8h, 0FFh
        add    esp, 4
        jmp    NEAR PTR loc_8_0590
loc_8_03E6:
        push   dword ptr [esi + 4]
        db  0E8h, 092h, 001h, 0F8h, 0FFh
        add    esp, 4
        mov    dword ptr [esi + 24h], eax
        or     eax, eax
        je     SHORT loc_8_042C
        cmp    eax, 100000h
        jbe    SHORT loc_8_0406
        mov    dword ptr [esi + 24h], 10000h
loc_8_0406:
        cmp    dword ptr [esi + 24h], 10000h
        jbe    SHORT loc_8_0414
        jmp    NEAR PTR loc_8_0590
loc_8_0414:
        mov    dword ptr [ebp - 8], 0
        push   4
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [esi + 4]
        db  0E8h, 083h, 001h, 0F8h, 0FFh
        add    esp, 0ch
loc_8_042C:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 0aa55h
        jne    NEAR PTR loc_8_0583
        push   1
        push   0
        push   0
        push   0
        push   0
        push   0
        push   1
        mov    eax, dword ptr [esi + 24h]
        shr    eax, 0ch
        push   eax
        db  0E8h, 063h, 014h, 0F8h, 0FFh
        add    esp, 20h
        mov    dword ptr [esi + 28h], eax
        or     eax, eax
        jne    SHORT loc_8_0468
        jmp    NEAR PTR loc_8_0590
loc_8_0468:
        push   dword ptr [esi + 24h]
        push   eax
        push   dword ptr [esi + 4]
        db  0E8h, 038h, 001h, 0F8h, 0FFh
        add    esp, 0ch
        push   1
        push   dword ptr [esi + 4]
        db  0E8h, 05Ah, 0FCh, 0F7h, 0FFh
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_8_0568
        db  0BBh, 000h, 000h, 000h, 000h
        shr    ebx, 0ch
        db  0BFh, 09Bh, 005h, 000h, 000h
        shr    edi, 0ch
        sub    edi, ebx
        inc    edi
        push   0
        push   edi
        push   ebx
        db  0E8h, 030h, 014h, 0F8h, 0FFh
        add    esp, 0ch
        call   sub_8_000F
        push   1
        db  0E8h, 053h, 0FFh, 0F7h, 0FFh
        add    esp, 4
        push   esi
        db  0E8h, 041h, 003h, 0F8h, 0FFh
        add    esp, 4
        push   esi
        call   sub_8_01B0
        add    esp, 4
        mov    dword ptr [ebp - 0ch], eax
        or     eax, eax
        je     SHORT loc_8_04ED
        push   dword ptr [esi + 18h]
        push   0c0000h
        call   sub_8_0082
        add    esp, 8
        push   0
        call   sub_8_01B0
        add    esp, 4
loc_8_04ED:
        push   0
        db  0E8h, 00Dh, 003h, 0F8h, 0FFh
        add    esp, 4
        push   0
        db  0E8h, 00Ch, 0FFh, 0F7h, 0FFh
        add    esp, 4
        call   sub_8_0054
        push   0
        push   edi
        push   ebx
        db  0E8h, 0D1h, 013h, 0F8h, 0FFh
        add    esp, 0ch
        push   1
        push   dword ptr [esi + 4]
        db  0E8h, 0E2h, 0FCh, 0F7h, 0FFh
        add    esp, 8
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_8_0568
        mov    eax, dword ptr [esi + 28h]
        movzx  ecx, byte ptr [eax + 2]
        shl    ecx, 9
        cmp    dword ptr [esi + 24h], ecx
        je     SHORT loc_8_0558
        mov    dword ptr [esi + 24h], ecx
        add    ecx, 0fffh
        and    ecx, 0fffff000h
        mov    dword ptr [esi + 24h], ecx
        push   0
        shr    ecx, 0ch
        push   ecx
        push   eax
        db  0E8h, 0D6h, 013h, 0F8h, 0FFh
        add    esp, 0ch
        mov    dword ptr [esi + 28h], eax
loc_8_0558:
        cmp    dword ptr [esi + 24h], 8000h
        ja     SHORT loc_8_0568
        mov    eax, 1
        jmp    SHORT loc_8_0592
loc_8_0568:
        mov    eax, dword ptr [esi + 28h]
        or     eax, eax
        je     SHORT loc_8_057A
        push   0
        push   eax
        db  0E8h, 04Dh, 013h, 0F8h, 0FFh
        add    esp, 8
loc_8_057A:
        mov    dword ptr [esi + 28h], 0
        jmp    SHORT loc_8_0590
loc_8_0583:
        db  068h, 044h, 002h, 000h, 000h
        db  0E8h, 059h, 001h, 0F8h, 0FFh
        add    esp, 4
loc_8_0590:
        sub    eax, eax
loc_8_0592:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h, 000h
_PTEXT ends
        end
