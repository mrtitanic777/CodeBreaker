; vnbt_pcode_03.asm -- VNBT object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_3_0000:
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   96h
        db  0E8h, 0D2h, 06Bh, 0FCh, 0FFh
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_3_0021
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_0021:
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, 25h
        rep stosd
        stosw
        mov    dword ptr [ebx + 8], 54637281h
        mov    byte ptr [ebx + 52h], al
        mov    byte ptr [ebx + 92h], 4
        mov    ecx, dword ptr [esp + 20h]
        mov    dword ptr [ebx + 74h], ecx
        mov    edx, dword ptr [esp + 24h]
        mov    dword ptr [ebx + 78h], edx
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [ebx + 7ch], eax
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [ebx + 80h], eax
        mov    esi, dword ptr [esp + 30h]
        lea    edi, [ebx + 8ch]
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi], eax
        mov    ax, word ptr [esi + 4]
        mov    word ptr [edi + 4], ax
        db  066h, 0A1h, 064h, 033h, 001h, 000h
        mov    esi, eax
        and    esi, 0ffffh
        test   ah, 10h
        je     SHORT loc_3_00A9
        or     ecx, ecx
        jne    SHORT loc_3_008F
        or     edx, edx
        je     SHORT loc_3_00A9
loc_3_008F:
        db  066h, 0C7h, 005h, 064h, 033h, 001h, 000h, 008h, 000h
        test   esi, 10h
        je     SHORT loc_3_00A9
        db  066h, 0C7h, 005h, 064h, 033h, 001h, 000h, 018h, 000h
loc_3_00A9:
        db  0E8h, 072h, 098h, 0FCh, 0FFh
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_3_00D0
        cmp    esi, 0ffh
        je     SHORT loc_3_00D0
        push   ebx
        db  0E8h, 086h, 0FFh, 0FBh, 0FFh
        add    esp, 4
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h
loc_3_00D0:
        lea    eax, [ebx + 0ch]
        mov    dword ptr [ebx + 10h], eax
        mov    dword ptr [eax], eax
        lea    eax, [ebx + 14h]
        mov    dword ptr [ebx + 18h], eax
        mov    dword ptr [eax], eax
        lea    esi, [ebx + 1ch]
        mov    dword ptr [ebx + 20h], esi
        mov    dword ptr [esi], esi
        lea    eax, [ebx + 24h]
        mov    dword ptr [ebx + 28h], eax
        mov    dword ptr [eax], eax
        lea    eax, [ebx + 34h]
        mov    dword ptr [ebx + 38h], eax
        mov    dword ptr [eax], eax
        lea    eax, [ebx + 2ch]
        mov    dword ptr [ebx + 30h], eax
        mov    dword ptr [eax], eax
        lea    eax, [ebx + 3ch]
        mov    dword ptr [ebx + 40h], eax
        mov    dword ptr [eax], eax
        mov    edi, dword ptr [esp + 14h]
        mov    ecx, 1
        mov    eax, dword ptr [edi + 8]
        lea    ebp, [edi + 8]
        mov    edx, edi
        sub    edx, eax
        cmp    edx, -8
        je     SHORT loc_3_0135
loc_3_0120:
        test   dword ptr [eax + 88h], ecx
        je     SHORT loc_3_0135
        add    ecx, ecx
        mov    eax, dword ptr [eax]
        mov    edx, edi
        sub    edx, eax
        cmp    edx, -8
        jne    SHORT loc_3_0120
loc_3_0135:
        mov    dword ptr [ebx + 88h], ecx
        mov    al, byte ptr [esp + 34h]
        mov    byte ptr [ebx + 50h], al
        db  0FEh, 005h, 04Fh, 033h, 001h, 000h
        db  080h, 03Dh, 04Fh, 033h, 001h, 000h, 001h
        jbe    SHORT loc_3_0158
        db  0C6h, 005h, 050h, 033h, 001h, 000h, 001h
loc_3_0158:
        mov    eax, dword ptr [esp + 1ch]
        push   ebx
        push   eax
        mov    eax, dword ptr [esp + 20h]
        push   eax
        call   sub_3_0380
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_3_017A
        cmp    eax, 0ffh
        jne    NEAR PTR loc_3_025F
loc_3_017A:
        mov    ax, word ptr [edi + 117h]
        cmp    ax, 0feh
        jb     SHORT loc_3_018B
        mov    ax, 0feh
loc_3_018B:
        mov    byte ptr [ebx + 4eh], al
        mov    cx, word ptr [edi + 119h]
        cmp    cx, 0feh
        jb     SHORT loc_3_01A0
        mov    cx, 0feh
loc_3_01A0:
        mov    byte ptr [ebx + 4fh], cl
        and    ax, 0ffh
        lea    eax, [eax*4 + 4]
        push   eax
        db  0E8h, 02Ch, 06Ah, 0FCh, 0FFh
        add    esp, 4
        mov    dword ptr [ebx + 44h], eax
        or     eax, eax
        je     NEAR PTR loc_3_025A
        mov    al, byte ptr [ebx + 4fh]
        and    eax, 0ffff00ffh
        lea    eax, [eax*4 + 4]
        push   eax
        db  0E8h, 009h, 06Ah, 0FCh, 0FFh
        add    esp, 4
        mov    dword ptr [ebx + 48h], eax
        or     eax, eax
        je     SHORT loc_3_025A
        xor    eax, eax
        mov    al, byte ptr [ebx + 4eh]
        lea    ecx, [eax*4 + 4]
        mov    edi, dword ptr [ebx + 44h]
        sub    eax, eax
        shr    ecx, 2
        rep stosd
        xor    eax, eax
        mov    al, byte ptr [ebx + 4fh]
        lea    ecx, [eax*4 + 4]
        mov    edi, dword ptr [ebx + 48h]
        sub    eax, eax
        shr    ecx, 2
        rep stosd
        mov    byte ptr [ebx + 4ch], 1
        mov    byte ptr [ebx + 4dh], 1
        push   ebx
        push   2
        push   49h
        push   esi
        db  0E8h, 0B8h, 007h, 0FEh, 0FFh
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_3_022E
        cmp    eax, 0ffh
        jne    SHORT loc_3_025F
loc_3_022E:
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [ebx], ebp
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [eax], ebx
        mov    dword ptr [ebp + 4], ebx
        push   ebx
        db  0E8h, 02Fh, 0F5h, 0FDh, 0FFh
        add    esp, 4
        or     eax, eax
        je     SHORT loc_3_024F
        cmp    eax, 0ffh
        jne    SHORT loc_3_025F
loc_3_024F:
        mov    byte ptr [ebx + 52h], 1
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_025A:
        mov    eax, 1
loc_3_025F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_0270:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        db  0B9h, 0FCh, 02Eh, 001h, 000h
        mov    edx, dword ptr [esp + 18h]
loc_3_0281:
        mov    eax, dword ptr [ecx]
        mov    edi, edx
loc_3_0285:
        mov    bl, byte ptr [eax]
        cmp    bl, byte ptr [edi]
        jne    SHORT loc_3_02A5
        or     bl, bl
        je     SHORT loc_3_02A1
        mov    bl, byte ptr [eax + 1]
        cmp    bl, byte ptr [edi + 1]
        jne    SHORT loc_3_02A5
        add    eax, 2
        add    edi, 2
        or     bl, bl
        jne    SHORT loc_3_0285
loc_3_02A1:
        sub    eax, eax
        jmp    SHORT loc_3_02AA
loc_3_02A5:
        sbb    eax, eax
        sbb    eax, -1
loc_3_02AA:
        or     eax, eax
        je     SHORT loc_3_02C5
        add    ecx, 8
        inc    esi
        db  081h, 0F9h, 014h, 02Fh, 001h, 000h
        jb     SHORT loc_3_0281
        mov    eax, dword ptr [esp + 1ch]
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_3_02C5:
        db  08Bh, 034h, 0F5h, 000h, 02Fh, 001h, 000h
        cmp    esi, 2fh
        je     SHORT loc_3_0320
        mov    dword ptr [esp + 0ch], 4
        lea    eax, [esp + 0ch]
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        push   esi
        db  0A1h, 0D0h, 02Eh, 001h, 000h
        push   eax
        db  0E8h, 0F3h, 0FDh, 0FBh, 0FFh
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_3_030B
        cmp    eax, 9
        je     SHORT loc_3_030B
        cmp    eax, 1ah
        mov    eax, dword ptr [esp + 1ch]
        pop    edi
        pop    esi
        pop    ebx
        je     SHORT loc_3_0316
        add    esp, 8
        ret
loc_3_030B:
        mov    eax, dword ptr [esp + 10h]
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_3_0316:
        add    esp, 8
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_0320:
        mov    dword ptr [esp + 0ch], 100h
        push   100h
        db  0E8h, 0AEh, 068h, 0FCh, 0FFh
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_3_0344
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_3_0344:
        lea    eax, [esp + 0ch]
        push   eax
        push   edi
        push   esi
        db  0A1h, 0D0h, 02Eh, 001h, 000h
        push   eax
        db  0E8h, 08Ch, 0FDh, 0FBh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_3_0366
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_3_0366:
        push   edi
        db  0E8h, 0DCh, 0FCh, 0FBh, 0FFh
        add    esp, 4
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_3_0380:
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 40h]
        mov    ebp, dword ptr [esp + 38h]
        lea    ebx, [esi + 60h]
        mov    dword ptr [esi + 58h], ebp
        mov    eax, dword ptr [esp + 3ch]
        mov    dword ptr [esi + 5ch], eax
        db  080h, 03Dh, 01Eh, 033h, 001h, 000h, 000h
        je     SHORT loc_3_03AC
        db  0A1h, 004h, 033h, 001h, 000h
        jmp    SHORT loc_3_03B0
loc_3_03AC:
        not    eax
        or     eax, ebp
loc_3_03B0:
        mov    dword ptr [ebx], eax
        push   0
        push   ebp
        push   8ah
        lea    eax, [esi + 6ch]
        push   esi
        push   eax
        push   0
        push   0
        db  0E8h, 0F8h, 076h, 0FCh, 0FFh
        add    esp, 1ch
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_3_03DD
        cmp    edi, 0ffh
        jne    NEAR PTR loc_3_04A6
loc_3_03DD:
        push   0
        push   ebp
        push   89h
        push   esi
        lea    eax, [esi + 68h]
        push   eax
        push   0
        push   0
        db  0E8h, 0CDh, 076h, 0FCh, 0FFh
        add    esp, 1ch
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_3_0408
        cmp    edi, 0ffh
        jne    NEAR PTR loc_3_049A
loc_3_0408:
        push   3
        push   ebp
        push   8bh
        push   esi
        lea    eax, [esi + 70h]
        push   eax
        push   0
        push   0
        db  0E8h, 0A2h, 076h, 0FCh, 0FFh
        add    esp, 1ch
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_3_042F
        cmp    edi, 0ffh
        jne    SHORT loc_3_048E
loc_3_042F:
        lea    eax, [esp + 22h]
        mov    word ptr [esp + 20h], 3
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0ch], 0
        mov    byte ptr [esp + 22h], 2ah
        push   0
        push   esi
        push   0
        mov    eax, dword ptr [ebx]
        push   eax
        lea    eax, [esp + 30h]
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        db  0E8h, 0C7h, 03Eh, 0FDh, 0FFh
        add    esp, 18h
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_3_04B0
        cmp    edi, 0ffh
        je     SHORT loc_3_04B0
        mov    eax, dword ptr [esi + 70h]
        push   eax
        db  0E8h, 0F5h, 06Bh, 0FCh, 0FFh
        add    esp, 4
loc_3_048E:
        mov    eax, dword ptr [esi + 68h]
        push   eax
        db  0E8h, 0E9h, 06Bh, 0FCh, 0FFh
        add    esp, 4
loc_3_049A:
        mov    eax, dword ptr [esi + 6ch]
        push   eax
        db  0E8h, 0DDh, 06Bh, 0FCh, 0FFh
        add    esp, 4
loc_3_04A6:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
loc_3_04B0:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esi + 54h], eax
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_04D0:
        push   esi
        push   edi
        push   0
        push   0
        db  068h, 0F8h, 030h, 001h, 000h
        mov    esi, dword ptr [esp + 1ch]
        push   esi
        db  0E8h, 0DBh, 0B3h, 0FCh, 0FFh
        add    esp, 10h
        cmp    eax, 1
        je     SHORT loc_3_0508
        cmp    eax, 2
        je     SHORT loc_3_0514
        cmp    eax, 4
        je     SHORT loc_3_0520
        cmp    eax, 8
        je     SHORT loc_3_052C
        db  066h, 0C7h, 005h, 064h, 033h, 001h, 000h, 001h, 010h
        jmp    SHORT loc_3_0535
        db  090h
loc_3_0508:
        db  066h, 0C7h, 005h, 064h, 033h, 001h, 000h, 001h, 000h
        jmp    SHORT loc_3_0535
        db  090h
loc_3_0514:
        db  066h, 0C7h, 005h, 064h, 033h, 001h, 000h, 002h, 000h
        jmp    SHORT loc_3_0535
        db  090h
loc_3_0520:
        db  066h, 0C7h, 005h, 064h, 033h, 001h, 000h, 004h, 000h
        jmp    SHORT loc_3_0535
        db  090h
loc_3_052C:
        db  066h, 0C7h, 005h, 064h, 033h, 001h, 000h, 008h, 000h
loc_3_0535:
        push   0
        push   -1
        db  068h, 004h, 031h, 001h, 000h
        push   esi
        db  0E8h, 01Ch, 0B4h, 0FCh, 0FFh
        add    esp, 10h
        mov    edi, eax
        push   0
        push   0
        db  068h, 018h, 031h, 001h, 000h
        push   esi
        db  0E8h, 008h, 0B4h, 0FCh, 0FFh
        add    esp, 10h
        cmp    eax, edi
        je     SHORT loc_3_0568
        db  0C6h, 005h, 01Eh, 033h, 001h, 000h, 000h
        jmp    SHORT loc_3_0574
loc_3_0568:
        db  0A3h, 004h, 033h, 001h, 000h
        db  0C6h, 005h, 01Eh, 033h, 001h, 000h, 001h
loc_3_0574:
        push   esi
        push   dword ptr [esp + 10h]
        call   sub_3_0584
        add    esp, 8
        pop    edi
        pop    esi
        ret
sub_3_0584:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    eax, [esp + 10h]
        push   eax
        db  068h, 02Ch, 031h, 001h, 000h
        db  0E8h, 0A6h, 0B2h, 0FCh, 0FFh
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_3_05AF
        cmp    ebp, 0ffh
        jne    NEAR PTR loc_3_0672
loc_3_05AF:
        sub    eax, eax
        mov    edi, dword ptr [esp + 10h]
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        dec    ecx
        je     NEAR PTR loc_3_0672
        mov    eax, dword ptr [esp + 10h]
        cmp    byte ptr [eax], 2ah
        jne    SHORT loc_3_05D1
        mov    byte ptr [eax], 0
loc_3_05D1:
        sub    eax, eax
        mov    edi, dword ptr [esp + 10h]
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        lea    ebx, [ecx - 1]
        cmp    ebx, 0ffh
        ja     NEAR PTR loc_3_0672
        lea    eax, [ebx + 2]
        push   eax
        db  0E8h, 0E8h, 065h, 0FCh, 0FFh
        add    esp, 4
        or     eax, eax
        je     SHORT loc_3_0666
        lea    edx, [eax + 1]
        mov    edi, edx
        mov    esi, dword ptr [esp + 10h]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        mov    byte ptr [ebx + eax + 1], 0
        mov    edi, 2
        mov    esi, eax
        mov    cl, byte ptr [edx]
        or     cl, cl
        je     SHORT loc_3_0651
loc_3_0628:
        inc    edi
        cmp    cl, 2eh
        jne    SHORT loc_3_0641
        mov    ecx, edx
        sub    ecx, esi
        dec    cl
        mov    byte ptr [esi], cl
        cmp    cl, 3fh
        ja     SHORT loc_3_064C
        or     cl, cl
        je     SHORT loc_3_064C
        mov    esi, edx
loc_3_0641:
        inc    edx
        mov    cl, byte ptr [edx]
        or     cl, cl
        jne    SHORT loc_3_0628
        jmp    SHORT loc_3_0651
        db  08Bh, 0FFh
loc_3_064C:
        mov    ebp, 0c0000001h
loc_3_0651:
        or     ebp, ebp
        je     SHORT loc_3_06B0
        cmp    ebp, 0ffh
        je     SHORT loc_3_06B0
        push   eax
        db  0E8h, 0E5h, 0F9h, 0FBh, 0FFh
        add    esp, 4
loc_3_0666:
        push   dword ptr [esp + 10h]
        db  0E8h, 0D9h, 0F9h, 0FBh, 0FFh
        add    esp, 4
loc_3_0672:
        mov    esi, dword ptr [esp + 18h]
        mov    word ptr [esi + 0a0h], 1
        mov    eax, dword ptr [esi + 0a4h]
        or     eax, eax
        je     SHORT loc_3_0692
        push   eax
        db  0E8h, 0B9h, 0F9h, 0FBh, 0FFh
        add    esp, 4
loc_3_0692:
        push   1
        db  0E8h, 047h, 065h, 0FCh, 0FFh
        add    esp, 4
        mov    dword ptr [esi + 0a4h], eax
        mov    byte ptr [eax], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_3_06B0:
        sub    edx, esi
        dec    dl
        mov    byte ptr [esi], dl
        mov    esi, dword ptr [esp + 18h]
        mov    word ptr [esi + 0a0h], di
        mov    dword ptr [esi + 0a4h], eax
        push   dword ptr [esp + 10h]
        db  0E8h, 078h, 0F9h, 0FBh, 0FFh
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
_PTEXT ends
        end
