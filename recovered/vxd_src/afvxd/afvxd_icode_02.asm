; afvxd_icode_02.asm -- AFVXD object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

        db  0C7h, 005h, 000h, 042h, 000h, 000h, 0FCh, 041h, 000h, 000h, 0C7h, 005h, 0FCh, 041h, 000h, 000h
        db  0FCh, 041h, 000h, 000h, 0C7h, 005h, 0ECh, 041h, 000h, 000h, 0E8h, 041h, 000h, 000h, 0C7h, 005h
        db  0E8h, 041h, 000h, 000h, 0E8h, 041h, 000h, 000h, 0C7h, 005h, 040h, 042h, 000h, 000h, 03Ch, 042h
        db  000h, 000h, 0C7h, 005h, 03Ch, 042h, 000h, 000h, 03Ch, 042h, 000h, 000h, 0A1h, 048h, 042h, 000h
        db  000h, 083h, 0F8h, 001h, 074h, 015h, 083h, 0F8h, 002h, 074h, 06Fh, 083h, 0F8h, 003h, 00Fh, 084h
        db  0C2h, 000h, 000h, 000h, 02Bh, 0C0h, 0E9h, 01Ah, 001h, 000h, 000h, 0C7h, 005h, 058h, 042h, 000h
        db  000h, 050h, 000h, 000h, 000h, 0C7h, 005h, 04Ch, 042h, 000h, 000h, 028h, 000h, 000h, 000h, 0C7h
        db  005h, 00Ch, 042h, 000h, 000h, 014h, 000h, 000h, 000h, 0C7h, 005h, 054h, 042h, 000h, 000h, 018h
        db  000h, 000h, 000h, 0C7h, 005h, 038h, 042h, 000h, 000h, 010h, 000h, 000h, 000h, 0C7h, 005h, 0E0h
        db  041h, 000h, 000h, 008h, 000h, 000h, 000h, 0C7h, 005h, 05Ch, 042h, 000h, 000h, 008h, 000h, 000h
        db  000h, 0C7h, 005h, 044h, 042h, 000h, 000h, 004h, 000h, 000h, 000h, 0C7h, 005h, 0F4h, 041h, 000h
        db  000h, 002h, 000h, 000h, 000h, 0E9h, 0B6h, 000h, 000h, 000h, 0C7h, 005h, 058h, 042h, 000h, 000h
        db  040h, 001h, 000h, 000h, 0C7h, 005h, 04Ch, 042h, 000h, 000h, 0A0h, 000h, 000h, 000h, 0C7h, 005h
        db  00Ch, 042h, 000h, 000h, 050h, 000h, 000h, 000h, 0C7h, 005h, 054h, 042h, 000h, 000h, 060h, 000h
        db  000h, 000h, 0C7h, 005h, 038h, 042h, 000h, 000h, 040h, 000h, 000h, 000h, 0C7h, 005h, 0E0h, 041h
        db  000h, 000h, 020h, 000h, 000h, 000h, 0C7h, 005h, 05Ch, 042h, 000h, 000h, 020h, 000h, 000h, 000h
        db  0C7h, 005h, 044h, 042h, 000h, 000h, 010h, 000h, 000h, 000h, 0C7h, 005h, 0F4h, 041h, 000h, 000h
        db  008h, 000h, 000h, 000h, 0EBh, 05Ah, 0C7h, 005h, 058h, 042h, 000h, 000h, 080h, 002h, 000h, 000h
        db  0C7h, 005h, 04Ch, 042h, 000h, 000h, 040h, 001h, 000h, 000h, 0C7h, 005h, 00Ch, 042h, 000h, 000h
        db  0A0h, 000h, 000h, 000h, 0C7h, 005h, 054h, 042h, 000h, 000h, 0C0h, 000h, 000h, 000h, 0C7h, 005h
        db  038h, 042h, 000h, 000h, 080h, 000h, 000h, 000h, 0C7h, 005h, 0E0h, 041h, 000h, 000h, 040h, 000h
        db  000h, 000h, 0C7h, 005h, 05Ch, 042h, 000h, 000h, 040h, 000h, 000h, 000h, 0C7h, 005h, 044h, 042h
        db  000h, 000h, 020h, 000h, 000h, 000h, 0C7h, 005h, 0F4h, 041h, 000h, 000h, 010h, 000h, 000h, 000h
        db  0B8h, 001h, 000h, 000h, 000h, 0C3h, 000h, 000h
sub_2_0178:
        db  0CDh, 020h, 000h, 000h, 088h, 004h
        jb     SHORT loc_2_0194
        db  0CDh, 020h, 000h, 000h, 040h, 000h
        jb     SHORT loc_2_0194
        db  0CDh, 020h, 00Ah, 000h, 088h, 004h
        db  0A3h, 0D8h, 03Bh, 000h, 000h
        clc
loc_2_0194:
        cmc
        sbb    eax, eax
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_01A0:
        push   edi
        mov    edx, dword ptr [esp + 8]
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [edx + ecx - 1]
        cmp    eax, edx
        je     SHORT loc_2_01C7
        mov    cl, byte ptr [esp + 0ch]
loc_2_01BE:
        cmp    byte ptr [eax], cl
        je     SHORT loc_2_01C9
        dec    eax
        cmp    eax, edx
        jne    SHORT loc_2_01BE
loc_2_01C7:
        sub    eax, eax
loc_2_01C9:
        pop    edi
        ret
sub_2_01CB:
        push   ebp
        mov    ebp, esp
        sub    esp, 350h
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 34h]
        mov    dword ptr [ebp - 44h], eax
        db  0C7h, 045h, 0C0h, 028h, 03Ch, 000h, 000h
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
        je     SHORT loc_2_020E
        sub    eax, eax
        jmp    NEAR PTR loc_2_03A9
loc_2_020E:
        call   sub_2_0888
        or     eax, eax
        jne    SHORT loc_2_021E
        sub    eax, eax
        jmp    NEAR PTR loc_2_03A9
loc_2_021E:
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
        call   sub_2_01A0
        add    esp, 8
        mov    byte ptr [eax], 0
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 0ch], 104h
        mov    dword ptr [ebp - 8], 104h
loc_2_026C:
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
        jne    NEAR PTR loc_2_0364
        cmp    dword ptr [ebp - 10h], 1
        jne    NEAR PTR loc_2_036E
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
        db  0BEh, 054h, 03Ch, 000h, 000h
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
        call   sub_2_0899
        add    esp, 4
        jmp    SHORT loc_2_036E
loc_2_0364:
        cmp    ebx, 0eah
        jne    SHORT loc_2_036E
        sub    ebx, ebx
loc_2_036E:
        mov    dword ptr [ebp - 0ch], 104h
        mov    dword ptr [ebp - 8], 104h
        inc    dword ptr [ebp - 4]
        or     ebx, ebx
        je     NEAR PTR loc_2_026C
        mov    eax, dword ptr [ebp - 34h]
        mov    dword ptr [ebp - 38h], eax
        push   dword ptr [ebp - 38h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        db  083h, 03Dh, 060h, 042h, 000h, 000h, 001h
        sbb    eax, eax
        inc    eax
loc_2_03A9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_03B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 150h
        push   ebx
        push   esi
        push   edi
        db  08Bh, 01Dh, 060h, 042h, 000h, 000h
        db  0BEh, 010h, 042h, 000h, 000h
        lea    eax, [ebx - 1]
        or     ebx, ebx
        je     SHORT loc_2_040C
loc_2_03CE:
        mov    ecx, dword ptr [esi]
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
loc_2_03D5:
        mov    dl, byte ptr [ecx]
        cmp    dl, byte ptr [edi]
        jne    SHORT loc_2_03F5
        or     dl, dl
        je     SHORT loc_2_03F1
        mov    dl, byte ptr [ecx + 1]
        cmp    dl, byte ptr [edi + 1]
        jne    SHORT loc_2_03F5
        add    ecx, 2
        add    edi, 2
        or     dl, dl
        jne    SHORT loc_2_03D5
loc_2_03F1:
        sub    ecx, ecx
        jmp    SHORT loc_2_03FA
loc_2_03F5:
        sbb    ecx, ecx
        sbb    ecx, -1
loc_2_03FA:
        or     ecx, ecx
        je     NEAR PTR loc_2_0718
        add    esi, 4
        mov    ecx, eax
        dec    eax
        or     ecx, ecx
        jne    SHORT loc_2_03CE
loc_2_040C:
        cmp    ebx, 8
        jb     SHORT loc_2_0416
        jmp    NEAR PTR loc_2_0718
loc_2_0416:
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    edi, dword ptr [ebp + 8]
        lea    edx, [ecx - 1]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    edx, [edx + ecx - 1]
        add    edx, 52h
        push   edx
        db  0E8h, 06Eh, 037h, 0FEh, 0FFh
        add    esp, 4
        mov    ebx, eax
        mov    dword ptr [esi], ebx
        or     ebx, ebx
        je     NEAR PTR loc_2_0718
        lea    edx, [ebx + 50h]
        mov    dword ptr [ebx], edx
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, 0ffffffffh
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
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    eax, dword ptr [ebx]
        lea    eax, [ecx + eax - 1]
        inc    eax
        mov    dword ptr [ebx + 4], eax
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [ebx + 4]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    esi, dword ptr [ebp + 10h]
        mov    dword ptr [ebx + 0ch], esi
        push   dword ptr [ebp + 0ch]
        db  0E8h, 0EEh, 035h, 0FEh, 0FFh
        add    esp, 4
        mov    dword ptr [ebx + 10h], eax
        push   0
        push   0
        call   dword ptr [esi + 0ch]
        add    esp, 8
        mov    edi, eax
        push   edi
        db  0E8h, 0D7h, 036h, 0FEh, 0FFh
        add    esp, 4
        mov    dword ptr [ebx + 8], eax
        or     eax, eax
        je     NEAR PTR loc_2_070F
        push   edi
        push   eax
        call   dword ptr [esi + 0ch]
        add    esp, 8
        cmp    eax, edi
        jne    NEAR PTR loc_2_06F9
        mov    eax, dword ptr [ebx + 8]
        cmp    dword ptr [eax + 4], 3
        jne    NEAR PTR loc_2_06F9
        db  0BEh, 058h, 03Ch, 000h, 000h
        lea    edi, [ebp - 150h]
        mov    ecx, 9
        rep movsd
        movsw
        db  0BEh, 080h, 03Ch, 000h, 000h
        lea    edi, [ebp - 150h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        movsw
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        lea    edi, [ebp - 150h]
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
        db  0BEh, 084h, 03Ch, 000h, 000h
        lea    edi, [ebp - 150h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, 5
        rep movsd
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 4ch], eax
        lea    eax, [ebp - 150h]
        mov    dword ptr [ebp - 48h], eax
        mov    dword ptr [ebp - 44h], 80000002h
        push   dword ptr [ebp - 4ch]
        push   dword ptr [ebp - 48h]
        push   dword ptr [ebp - 44h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_2_06F9
        mov    dword ptr [ebp - 4], 4
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 40h], eax
        lea    edi, [ebx + 18h]
        mov    dword ptr [ebp - 3ch], edi
        lea    eax, [ebp - 28h]
        mov    dword ptr [ebp - 38h], eax
        mov    dword ptr [ebp - 34h], 0
        db  0C7h, 045h, 0D0h, 098h, 03Ch, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 2ch], eax
        push   dword ptr [ebp - 40h]
        push   dword ptr [ebp - 3ch]
        push   dword ptr [ebp - 38h]
        push   dword ptr [ebp - 34h]
        push   dword ptr [ebp - 30h]
        push   dword ptr [ebp - 2ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        jne    NEAR PTR loc_2_06E1
        cmp    dword ptr [ebp - 4], 4
        jne    NEAR PTR loc_2_06E1
        mov    dword ptr [ebp - 4], 4
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 24h], eax
        lea    esi, [ebx + 14h]
        mov    dword ptr [ebp - 20h], esi
        lea    eax, [ebp - 28h]
        mov    dword ptr [ebp - 1ch], eax
        mov    dword ptr [ebp - 18h], 0
        db  0C7h, 045h, 0ECh, 0ACh, 03Ch, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 10h], eax
        push   dword ptr [ebp - 24h]
        push   dword ptr [ebp - 20h]
        push   dword ptr [ebp - 1ch]
        push   dword ptr [ebp - 18h]
        push   dword ptr [ebp - 14h]
        push   dword ptr [ebp - 10h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_2_066B
        cmp    dword ptr [ebp - 4], 4
        je     SHORT loc_2_066F
loc_2_066B:
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi], eax
loc_2_066F:
        mov    ecx, dword ptr [edi]
        db  039h, 00Dh, 020h, 03Ch, 000h, 000h
        jae    SHORT loc_2_0687
        db  089h, 00Dh, 020h, 03Ch, 000h, 000h
        lea    eax, [ecx + 6]
        db  0A3h, 024h, 03Ch, 000h, 000h
loc_2_0687:
        mov    eax, dword ptr [esi]
        add    eax, 6
        mov    dword ptr [ebx + 1ch], eax
        add    ecx, 6
        mov    dword ptr [ebx + 20h], ecx
        db  068h, 0E4h, 03Bh, 000h, 000h
        push   dword ptr [ebx]
        db  0E8h, 0EFh, 033h, 0FEh, 0FFh
        add    esp, 8
        cmp    eax, 1
        jne    SHORT loc_2_06F9
        db  0A1h, 060h, 042h, 000h, 000h
        inc    eax
        db  0A3h, 060h, 042h, 000h, 000h
        lea    eax, [ebx + 24h]
        mov    dword ptr [ebx + 28h], eax
        mov    dword ptr [eax], eax
        lea    eax, [ebx + 2ch]
        mov    dword ptr [ebx + 30h], eax
        mov    dword ptr [eax], eax
        lea    eax, [ebx + 34h]
        mov    dword ptr [ebx + 38h], eax
        mov    dword ptr [eax], eax
        lea    eax, [ebx + 3ch]
        mov    dword ptr [ebx + 40h], eax
        mov    dword ptr [eax], eax
        lea    eax, [ebx + 48h]
        mov    dword ptr [ebx + 4ch], eax
        mov    dword ptr [eax], eax
        lea    eax, [ebx + 10h]
        jmp    SHORT loc_2_071A
loc_2_06E1:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0ch], eax
        push   dword ptr [ebp - 0ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
loc_2_06F9:
        push   dword ptr [ebx + 8]
        db  0E8h, 0B5h, 034h, 0FEh, 0FFh
        add    esp, 4
        push   ebx
        db  0E8h, 0ACh, 034h, 0FEh, 0FFh
        add    esp, 4
        jmp    SHORT loc_2_0718
loc_2_070F:
        push   ebx
        db  0E8h, 0A1h, 034h, 0FEh, 0FFh
        add    esp, 4
loc_2_0718:
        sub    eax, eax
loc_2_071A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0721:
        push   ebx
        push   esi
        push   edi
        db  0BFh, 010h, 042h, 000h, 000h
        db  08Bh, 00Dh, 060h, 042h, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_2_0771
        mov    edx, dword ptr [esp + 10h]
loc_2_0737:
        mov    eax, dword ptr [edi]
        mov    esi, edx
        mov    eax, dword ptr [eax]
loc_2_073D:
        mov    bl, byte ptr [eax]
        cmp    bl, byte ptr [esi]
        jne    SHORT loc_2_075D
        or     bl, bl
        je     SHORT loc_2_0759
        mov    bl, byte ptr [eax + 1]
        cmp    bl, byte ptr [esi + 1]
        jne    SHORT loc_2_075D
        add    eax, 2
        add    esi, 2
        or     bl, bl
        jne    SHORT loc_2_073D
loc_2_0759:
        sub    eax, eax
        jmp    SHORT loc_2_0762
loc_2_075D:
        sbb    eax, eax
        sbb    eax, -1
loc_2_0762:
        or     eax, eax
        je     SHORT loc_2_076D
        add    edi, 4
        or     ecx, ecx
        jne    SHORT loc_2_0737
loc_2_076D:
        or     ecx, ecx
        jne    SHORT loc_2_0775
loc_2_0771:
        sub    eax, eax
        jmp    SHORT loc_2_079E
loc_2_0775:
        push   dword ptr [edi]
        db  0E8h, 03Ah, 034h, 0FEh, 0FFh
        add    esp, 4
        db  0A1h, 060h, 042h, 000h, 000h
        dec    eax
        db  0A3h, 060h, 042h, 000h, 000h
        lea    ecx, [eax*4]
        lea    esi, [edi + 4]
        shr    ecx, 2
        rep movsd
        mov    eax, 1
loc_2_079E:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_07B0:
        sub    esp, 40h
        lea    eax, [esp]
        push   eax
        db  0E8h, 0C3h, 032h, 0FEh, 0FFh
        add    esp, 4
        cmp    dword ptr [esp + 4], 400h
        ja     SHORT loc_2_07D6
        db  0C7h, 005h, 048h, 042h, 000h, 000h, 001h, 000h, 000h, 000h
        jmp    SHORT loc_2_07F4
loc_2_07D6:
        cmp    dword ptr [esp + 4], 1000h
        db  0C7h, 005h, 048h, 042h, 000h, 000h, 002h, 000h, 000h, 000h
        jbe    SHORT loc_2_07F4
        db  0C7h, 005h, 048h, 042h, 000h, 000h, 003h, 000h, 000h, 000h
loc_2_07F4:
        call   sub_2_0178
        or     eax, eax
        jne    SHORT loc_2_0804
        sub    eax, eax
        jmp    NEAR PTR loc_2_0884
loc_2_0804:
        db  068h, 0BDh, 01Eh, 000h, 000h
        db  0E8h, 068h, 006h, 0FEh, 0FFh
        add    esp, 4
        or     eax, eax
        je     SHORT loc_2_0819
        sub    eax, eax
        jmp    SHORT loc_2_0884
loc_2_0819:
        db  068h, 02Eh, 01Fh, 000h, 000h
        db  0E8h, 048h, 006h, 0FEh, 0FFh
        add    esp, 4
        or     eax, eax
        je     SHORT loc_2_0838
        push   0
        db  0E8h, 045h, 006h, 0FEh, 0FFh
        add    esp, 4
        sub    eax, eax
        jmp    SHORT loc_2_0884
loc_2_0838:
        call   0
        or     eax, eax
        jne    SHORT loc_2_0859
        push   0
        db  0E8h, 023h, 006h, 0FEh, 0FFh
        add    esp, 4
        push   0
        db  0E8h, 024h, 006h, 0FEh, 0FFh
        add    esp, 4
        sub    eax, eax
        jmp    SHORT loc_2_0884
loc_2_0859:
        call   sub_2_01CB
        or     eax, eax
        jne    SHORT loc_2_087A
        push   0
        db  0E8h, 002h, 006h, 0FEh, 0FFh
        add    esp, 4
        push   0
        db  0E8h, 003h, 006h, 0FEh, 0FFh
        add    esp, 4
        sub    eax, eax
        jmp    SHORT loc_2_0884
loc_2_087A:
        db  0E8h, 045h, 033h, 0FEh, 0FFh
        mov    eax, 1
loc_2_0884:
        add    esp, 40h
        ret
sub_2_0888:
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
sub_2_0899:
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
sub_2_08B4:
        mov    eax, 488h
        db  0CDh, 020h, 046h, 001h, 001h, 000h
        jecxz  loc_2_08C3
        clc
        ret
loc_2_08C3:
        db  0BAh, 01Ch, 009h, 000h, 000h
        mov    eax, 1
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        ret
loc_2_08D4:
        db  080h, 03Dh, 028h, 03Dh, 000h, 000h, 000h
        jne    SHORT loc_2_091A
        db  0C6h, 005h, 028h, 03Dh, 000h, 000h, 001h
loc_2_08E4:
        db  0BFh, 064h, 042h, 000h, 000h
        db  0B9h, 064h, 042h, 000h, 000h
        sub    ecx, edi
        shr    ecx, 2
        xor    eax, eax
        cld
        rep stosd
        call   sub_2_08B4
        jb     SHORT loc_2_0919
        db  0CDh, 020h, 000h, 000h, 088h, 004h
        jb     SHORT loc_2_0919
        db  0CDh, 020h, 000h, 000h, 03Eh, 000h
        jb     SHORT loc_2_0919
        call   sub_2_07B0
        cmp    eax, 1
        jmp    SHORT loc_2_091A
loc_2_0919:
        stc
loc_2_091A:
        ret
        db  000h
loc_2_091C:
        push   esi
        push   esp
        inc    esp
        dec    ecx
        xor    edi, dword ptr cs:[eax]
        and    al, byte ptr ss:[ecx + 46h]
        push   esi
_ITEXT ends
        end
