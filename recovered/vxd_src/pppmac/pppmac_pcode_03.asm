; pppmac_pcode_03.asm -- PPPMAC object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_3_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 44h
        mov    word ptr [ebp - 0ch], 7
        db  083h, 03Dh, 048h, 082h, 001h, 000h, 001h
        mov    word ptr [ebp - 0ah], 7
        db  0C7h, 045h, 0F8h, 074h, 083h, 001h, 000h
        je     SHORT loc_3_0030
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_0030:
        db  0C7h, 005h, 048h, 082h, 001h, 000h, 002h, 000h, 000h, 000h
        db  0C7h, 005h, 0D0h, 098h, 001h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 005h, 0D8h, 098h, 001h, 000h, 0D4h, 098h, 001h, 000h
        db  0C7h, 005h, 0D4h, 098h, 001h, 000h, 0D4h, 098h, 001h, 000h
        push   0
        push   0
        push   0
        db  068h, 0CCh, 098h, 001h, 000h
        db  0E8h, 002h, 006h, 0FCh, 0FFh
        mov    byte ptr [ebp - 44h], 3
        mov    byte ptr [ebp - 43h], 0
        db  0C7h, 045h, 0C2h, 000h, 009h, 000h, 000h
        db  0C7h, 045h, 0C6h, 0C0h, 00Ah, 000h, 000h
        db  0C7h, 045h, 0CAh, 070h, 03Ch, 000h, 000h
        db  0C7h, 045h, 0CEh, 0B0h, 034h, 000h, 000h
        db  0C7h, 045h, 0D2h, 0B0h, 015h, 000h, 000h
        db  0C7h, 045h, 0D6h, 060h, 00Ch, 000h, 000h
        db  0C7h, 045h, 0E2h, 0F0h, 001h, 000h, 000h
        db  0C7h, 045h, 0DEh, 050h, 008h, 000h, 000h
        db  0C7h, 045h, 0E6h, 000h, 008h, 000h, 000h
        db  0C7h, 045h, 0DAh, 010h, 015h, 000h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 16h], eax
        mov    dword ptr [ebp - 12h], ecx
        mov    eax, dword ptr [ebp + 8]
        db  0A3h, 0C8h, 098h, 001h, 000h
        db  0C7h, 005h, 0C0h, 098h, 001h, 000h, 0BCh, 098h, 001h, 000h
        db  0C7h, 005h, 0BCh, 098h, 001h, 000h, 0BCh, 098h, 001h, 000h
        db  066h, 0C7h, 005h, 04Ch, 082h, 001h, 000h, 000h, 000h
        db  0A1h, 050h, 082h, 001h, 000h
        push   eax
        push   0
        push   5eah
        db  068h, 0B4h, 098h, 001h, 000h
        db  0E8h, 058h, 006h, 0FCh, 0FFh
        db  083h, 03Dh, 0B4h, 098h, 001h, 000h, 000h
        jne    SHORT loc_3_0120
        db  0C7h, 005h, 048h, 082h, 001h, 000h, 001h, 000h, 000h, 000h
        mov    eax, 0c0000001h
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_3_0120:
        push   36h
        lea    eax, [ebp - 44h]
        push   eax
        db  068h, 0C4h, 098h, 001h, 000h
        db  0A1h, 0CCh, 098h, 001h, 000h
        push   eax
        lea    eax, [ebp - 4]
        db  068h, 0C4h, 098h, 001h, 000h
        push   eax
        db  0E8h, 037h, 005h, 0FCh, 0FFh
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_3_0180
        db  0E8h, 086h, 01Fh, 0FDh, 0FFh
        db  0E8h, 031h, 077h, 0FDh, 0FFh
        db  0E8h, 00Ch, 07Ch, 0FCh, 0FFh
        db  0E8h, 0C7h, 007h, 0FCh, 0FFh
        db  0E8h, 012h, 099h, 0FCh, 0FFh
        db  0E8h, 0CDh, 0B9h, 0FCh, 0FFh
        db  0E8h, 0E8h, 0F1h, 0FCh, 0FFh
        db  0E8h, 0A3h, 034h, 0FDh, 0FFh
        db  0C7h, 005h, 048h, 082h, 001h, 000h, 003h, 000h, 000h, 000h
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
loc_3_0180:
        push   0
        push   5eah
        db  0A1h, 0B4h, 098h, 001h, 000h
        push   eax
        db  0E8h, 0CAh, 005h, 0FCh, 0FFh
        push   0
        db  0A1h, 0CCh, 098h, 001h, 000h
        push   eax
        db  0E8h, 0D1h, 004h, 0FCh, 0FFh
        db  0C7h, 005h, 048h, 082h, 001h, 000h, 001h, 000h, 000h, 000h
        mov    eax, 0c0000001h
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_01C0:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_01D0:
        db  0C7h, 005h, 0D0h, 098h, 001h, 000h, 004h, 000h, 000h, 000h
        ret
        db  005h, 000h, 000h, 000h, 000h, 0B8h, 001h, 000h, 000h, 000h, 0C3h, 08Dh, 049h, 000h, 08Dh, 0A4h
        db  024h, 000h, 000h, 000h, 000h
loc_3_01F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 98h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 70h], 0dh
        mov    word ptr [ebp - 6eh], 0dh
        db  0C7h, 045h, 094h, 08Ch, 083h, 001h, 000h
        mov    word ptr [ebp - 68h], 0dh
        mov    word ptr [ebp - 66h], 0dh
        db  0C7h, 045h, 09Ch, 0ACh, 083h, 001h, 000h
        mov    word ptr [ebp - 60h], 0dh
        mov    word ptr [ebp - 5eh], 0dh
        db  0C7h, 045h, 0A4h, 0CCh, 083h, 001h, 000h
        mov    word ptr [ebp - 58h], 0eh
        mov    word ptr [ebp - 56h], 0eh
        db  0C7h, 045h, 0ACh, 0ECh, 083h, 001h, 000h
        mov    word ptr [ebp - 50h], 8
        mov    word ptr [ebp - 4eh], 8
        db  0C7h, 045h, 0B4h, 004h, 084h, 001h, 000h
        mov    word ptr [ebp - 48h], 0eh
        mov    word ptr [ebp - 46h], 0eh
        db  0C7h, 045h, 0BCh, 01Ch, 084h, 001h, 000h
        mov    word ptr [ebp - 40h], 7
        mov    word ptr [ebp - 3eh], 7
        db  0C7h, 045h, 0C4h, 034h, 084h, 001h, 000h
        mov    esi, 5eah
        mov    word ptr [ebp - 38h], 8
        mov    word ptr [ebp - 36h], 8
        db  0C7h, 045h, 0CCh, 044h, 084h, 001h, 000h
        mov    word ptr [ebp - 30h], 15h
        sub    bl, bl
        mov    word ptr [ebp - 2eh], 15h
        db  0C7h, 045h, 0D4h, 064h, 084h, 001h, 000h
        mov    dword ptr [ebp - 24h], 0
        mov    dword ptr [ebp - 20h], 0
        mov    word ptr [ebp - 6], 4
        mov    byte ptr [ebp - 4], bl
        mov    byte ptr [ebp - 3], bl
        mov    byte ptr [ebp - 2], bl
        mov    byte ptr [ebp - 1], 1
        db  0A1h, 050h, 082h, 001h, 000h
        push   eax
        lea    eax, [ebp - 0ch]
        push   0
        push   2b2h
        push   eax
        db  0E8h, 071h, 004h, 0FCh, 0FFh
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_3_02F9
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_3_02F9:
        mov    edi, dword ptr [ebp - 0ch]
        sub    eax, eax
        mov    ecx, 0ach
        rep stosd
        stosw
        lea    edi, [ebp - 98h]
        mov    ecx, 9
        rep stosd
        stosw
        mov    ecx, dword ptr [ebp - 0ch]
        db  0A1h, 0C4h, 098h, 001h, 000h
        mov    dword ptr [ecx + 26h], eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        db  0E8h, 0A2h, 004h, 0FCh, 0FFh
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_3_0346
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_3_0346:
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 8
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        db  0E8h, 07Eh, 003h, 0FCh, 0FFh
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_3_036A
        cmp    dword ptr [ebp - 28h], 6
        je     SHORT loc_3_0396
loc_3_036A:
        push   6
        db  068h, 07Ch, 084h, 001h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 8
        push   eax
        db  0E8h, 0BFh, 004h, 0FCh, 0FFh
        mov    eax, dword ptr [ebp - 0ch]
        db  066h, 08Bh, 00Dh, 04Ch, 082h, 001h, 000h
        mov    byte ptr [eax + 0ch], ch
        mov    eax, dword ptr [ebp - 0ch]
        db  08Ah, 00Dh, 04Ch, 082h, 001h, 000h
        mov    byte ptr [eax + 0dh], cl
loc_3_0396:
        push   0
        lea    eax, [ebp - 70h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        db  0E8h, 02Dh, 004h, 0FCh, 0FFh
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_3_03B9
        mov    eax, dword ptr [ebp - 14h]
        mov    esi, dword ptr [eax + 4]
loc_3_03B9:
        push   0
        lea    eax, [ebp - 68h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        db  0E8h, 00Ah, 004h, 0FCh, 0FFh
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_3_03DF
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 20h], ecx
loc_3_03DF:
        push   0
        lea    eax, [ebp - 60h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        db  0E8h, 0E4h, 003h, 0FCh, 0FFh
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_3_0405
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 24h], ecx
loc_3_0405:
        push   0
        lea    eax, [ebp - 58h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        db  0E8h, 0BEh, 003h, 0FCh, 0FFh
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_3_042D
        mov    eax, dword ptr [ebp - 14h]
        mov    cx, word ptr [eax + 4]
        mov    word ptr [ebp - 6], cx
loc_3_042D:
        push   0
        lea    eax, [ebp - 50h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        db  0E8h, 096h, 003h, 0FCh, 0FFh
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_3_0450
        mov    eax, dword ptr [ebp - 14h]
        mov    bl, byte ptr [eax + 4]
loc_3_0450:
        push   0
        lea    eax, [ebp - 48h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        db  0E8h, 073h, 003h, 0FCh, 0FFh
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_3_0476
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 4]
        mov    byte ptr [ebp - 4], al
loc_3_0476:
        push   0
        lea    eax, [ebp - 40h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        db  0E8h, 04Dh, 003h, 0FCh, 0FFh
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_3_049C
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 4]
        mov    byte ptr [ebp - 3], al
loc_3_049C:
        push   0
        lea    eax, [ebp - 38h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        db  0E8h, 027h, 003h, 0FCh, 0FFh
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_3_04C2
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 4]
        mov    byte ptr [ebp - 2], al
loc_3_04C2:
        push   0
        lea    eax, [ebp - 30h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        db  0E8h, 001h, 003h, 0FCh, 0FFh
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_3_04E8
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 4]
        mov    byte ptr [ebp - 1], al
loc_3_04E8:
        lea    eax, [ebp - 98h]
        push   eax
        mov    edi, dword ptr [ebp + 10h]
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [eax + 26h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 2ah
        push   eax
        db  0E8h, 077h, 001h, 0FCh, 0FFh
        mov    dword ptr [ebp - 10h], eax
        or     eax, eax
        je     SHORT loc_3_0530
        push   0
        push   2b2h
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0E8h, 03Ah, 002h, 0FCh, 0FFh
        mov    eax, dword ptr [ebp - 10h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
loc_3_0530:
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 0adh], 20h
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_3_08A0
        mov    dword ptr [ebp - 10h], eax
        or     eax, eax
        je     SHORT loc_3_0574
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 2ah]
        push   eax
        db  0E8h, 032h, 001h, 0FCh, 0FFh
        push   0
        push   2b2h
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0E8h, 0F6h, 001h, 0FCh, 0FFh
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_3_0574:
        db  0E8h, 0F7h, 011h, 0FCh, 0FFh
        mov    ecx, dword ptr [ebp - 0ch]
        lea    eax, [esi + 0c8h]
        mov    dword ptr [ecx + 0c7h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 0c3h], esi
        mov    eax, dword ptr [ebp - 0ch]
        mov    cx, word ptr [ebp - 6]
        mov    word ptr [eax + 0d3h], cx
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 24h]
        mov    dword ptr [eax + 0cfh], ecx
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 20h]
        mov    dword ptr [eax + 0cbh], ecx
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, bl
        or     bl, bl
        mov    dword ptr [ecx + 0ffh], eax
        je     SHORT loc_3_05DC
        mov    eax, dword ptr [ebp - 0ch]
        or     dword ptr [eax + 0cbh], 10h
        mov    eax, dword ptr [ebp - 0ch]
        or     dword ptr [eax + 0cfh], 10h
loc_3_05DC:
        cmp    byte ptr [ebp - 4], 0
        je     SHORT loc_3_05FC
        mov    eax, dword ptr [ebp - 0ch]
        or     dword ptr [eax + 0cbh], 80h
        mov    eax, dword ptr [ebp - 0ch]
        or     dword ptr [eax + 0cfh], 80h
loc_3_05FC:
        xor    eax, eax
        mov    ax, word ptr [edi]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 0d7h
        push   eax
        db  0E8h, 028h, 002h, 0FCh, 0FFh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    ax, word ptr [edi]
        mov    word ptr [ecx + 0d5h], ax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 0d5h
        cmp    word ptr [eax], 20h
        jbe    SHORT loc_3_0634
        mov    word ptr [eax], 20h
loc_3_0634:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax + 0d5h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 0d7h
        push   eax
        db  0E8h, 0E6h, 001h, 0FCh, 0FFh
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        add    eax, 0b3h
        mov    dword ptr [ecx + 0b7h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 0b7h]
        mov    dword ptr [ecx + 0b3h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        add    eax, 0bbh
        mov    dword ptr [ecx + 0bfh], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 0bfh]
        mov    dword ptr [ecx + 0bbh], eax
        mov    eax, dword ptr [ebp - 0ch]
        shl    dword ptr [eax + 0c7h], 1
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax + 0d3h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_3_17B0
        mov    esi, dword ptr [ebp - 0ch]
        mov    al, byte ptr [ebp - 3]
        mov    byte ptr [esi + 205h], al
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [esi + 213h], al
        add    esi, 103h
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 111h], al
        mov    dword ptr [esi], 2
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 20h], eax
        lea    eax, [ebp - 1ch]
        push   0
        push   eax
        push   esi
        db  0E8h, 006h, 0F9h, 0FBh, 0FFh
        mov    eax, dword ptr [ebp - 1ch]
        mov    word ptr [eax + 28h], 0deadh
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [esi + 61h], eax
        lea    eax, [esi + 24h]
        mov    dword ptr [esi + 8], 2580h
        mov    dword ptr [esi + 0e8h], 806h
        mov    dword ptr [esi + 28h], eax
        mov    dword ptr [eax], eax
        mov    dword ptr [esi + 2ch], 0
        lea    eax, [esi + 30h]
        mov    dword ptr [esi + 34h], eax
        mov    dword ptr [eax], eax
        mov    dword ptr [esi + 38h], 0
        lea    eax, [esi + 40h]
        mov    dword ptr [esi + 44h], eax
        mov    dword ptr [eax], eax
        mov    eax, dword ptr [ebp - 0ch]
        db  0C7h, 000h, 0BCh, 098h, 001h, 000h
        mov    ecx, dword ptr [ebp - 0ch]
        db  0A1h, 0C0h, 098h, 001h, 000h
        mov    dword ptr [ecx + 4], eax
        db  08Bh, 00Dh, 0C0h, 098h, 001h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0ch]
        db  0A3h, 0C0h, 098h, 001h, 000h
        mov    ecx, dword ptr [ebp - 0ch]
        db  066h, 0A1h, 04Ch, 082h, 001h, 000h
        mov    word ptr [ecx + 2b0h], ax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 2eh], 0
        mov    eax, dword ptr [ebp - 0ch]
        db  0C7h, 040h, 032h, 030h, 035h, 000h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, eax
        mov    dword ptr [ecx + 36h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 14bh], 0
        mov    eax, dword ptr [ebp - 0ch]
        db  0C7h, 080h, 04Fh, 001h, 000h, 000h, 0A0h, 036h, 000h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, eax
        mov    dword ptr [ecx + 153h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 207h], 0
        mov    eax, dword ptr [ebp - 0ch]
        db  0C7h, 080h, 00Bh, 002h, 000h, 000h, 0B0h, 010h, 000h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        add    eax, 103h
        mov    dword ptr [ecx + 20fh], eax
        db  066h, 0FFh, 005h, 04Ch, 082h, 001h, 000h
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_0800:
        push   ebp
        mov    ebp, esp
        push   esi
        db  066h, 083h, 03Dh, 04Ch, 082h, 001h, 000h, 000h
        je     SHORT loc_3_0846
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        db  066h, 0FFh, 00Dh, 04Ch, 082h, 001h, 000h
        mov    eax, dword ptr [esi + 3eh]
        push   eax
        call   sub_3_1D60
        mov    eax, dword ptr [esi + 2ah]
        push   eax
        db  0E8h, 04Fh, 0FEh, 0FBh, 0FFh
        push   0
        push   2b2h
        push   esi
        db  0E8h, 016h, 0FFh, 0FBh, 0FFh
loc_3_0846:
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_3_0850:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        db  0C7h, 005h, 048h, 082h, 001h, 000h, 001h, 000h, 000h, 000h
        db  083h, 03Dh, 0D0h, 098h, 001h, 000h, 004h
        je     SHORT loc_3_0873
        db  0E8h, 082h, 010h, 0FCh, 0FFh
        db  0E8h, 08Bh, 0F9h, 0FBh, 0FFh
loc_3_0873:
        db  0E8h, 018h, 056h, 0FCh, 0FFh
        db  0A1h, 0C4h, 098h, 001h, 000h
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 0F5h, 0FDh, 0FBh, 0FFh
        push   0
        db  0A1h, 0CCh, 098h, 001h, 000h
        push   eax
        db  0E8h, 0DCh, 0FDh, 0FBh, 0FFh
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_3_08A0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        lea    eax, [ecx + 12h]
        mov    dword ptr [ecx + 16h], eax
        mov    dword ptr [eax], eax
        lea    eax, [ecx + 1ah]
        mov    dword ptr [ecx + 1eh], eax
        mov    dword ptr [eax], eax
        mov    byte ptr [ecx + 0aeh], 1
        lea    eax, [ecx + 3eh]
        push   eax
        lea    eax, [ecx + 22h]
        push   eax
        lea    eax, [ecx + 8]
        push   eax
        db  068h, 0C0h, 016h, 000h, 000h
        db  068h, 0B0h, 016h, 000h, 000h
        db  068h, 0A0h, 016h, 000h, 000h
        xor    eax, eax
        mov    al, byte ptr [ecx + 0adh]
        push   eax
        call   sub_3_1C40
        or     al, al
        jne    SHORT loc_3_08F3
        mov    eax, 0c000009ah
        pop    ebp
        ret    4
loc_3_08F3:
        sub    eax, eax
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_0900:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    edx, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [ebp + 14h]
        mov    esi, edx
        lea    eax, [ecx + edx*4]
        mov    ecx, eax
loc_3_0920:
        or     esi, esi
        je     SHORT loc_3_092D
        sub    ecx, 4
        dec    esi
        cmp    dword ptr [ecx], 3
        jne    SHORT loc_3_0920
loc_3_092D:
        mov    ecx, dword ptr [ebp + 14h]
        cmp    dword ptr [ecx + esi*4], 3
        je     SHORT loc_3_0945
        mov    esi, edx
loc_3_0938:
        or     esi, esi
        je     SHORT loc_3_0945
        sub    eax, 4
        dec    esi
        cmp    dword ptr [eax], 0
        jne    SHORT loc_3_0938
loc_3_0945:
        mov    ecx, dword ptr [ebp + 14h]
        mov    eax, dword ptr [ecx + esi*4]
        or     eax, eax
        je     SHORT loc_3_0962
        cmp    eax, 3
        je     SHORT loc_3_0962
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_3_0962:
        mov    eax, dword ptr [ebp + 10h]
        mov    ebx, dword ptr [ebp + 20h]
        mov    dword ptr [eax], esi
        inc    dword ptr [ebx + 0eh]
        db  0A1h, 050h, 082h, 001h, 000h
        push   eax
        lea    eax, [ebp - 4]
        push   0
        push   5ah
        push   eax
        db  0E8h, 0D6h, 0FDh, 0FBh, 0FFh
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_3_09B5
        push   0
        push   0
        push   0
        push   0
        push   0
        push   0
        push   0
        push   0c000009ah
        push   1
        push   0
        mov    eax, dword ptr [ebx + 2ah]
        push   eax
        db  0E8h, 082h, 0FEh, 0FBh, 0FFh
        add    esp, 2ch
        mov    dword ptr [ebp - 8], 0c000009ah
        jmp    NEAR PTR loc_3_0A9F
loc_3_09B5:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 16h
        rep stosd
        stosw
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp + 14h]
        mov    eax, dword ptr [ecx + esi*4]
        mov    ecx, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx + 35h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 39h], 3
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 3dh], 1
        mov    eax, dword ptr [esi + 1ch]
        add    eax, 70h
        push   eax
        db  0E8h, 093h, 072h, 0FCh, 0FFh
        add    esp, 4
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 47h], ax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 10h], esi
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 18h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1ch], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], 5eah
        mov    dword ptr [ebx + 0a9h], 5eah
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0ch
        push   eax
        push   esi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 3eh]
        push   eax
        call   sub_3_1DB0
        or     al, al
        jne    SHORT loc_3_0A7A
        push   0
        push   5ah
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 0EBh, 0FCh, 0FBh, 0FFh
        mov    dword ptr [ebp - 8], 0c0000001h
        jmp    SHORT loc_3_0A9F
loc_3_0A7A:
        mov    eax, dword ptr [ebp - 4]
        lea    ecx, [ebx + 12h]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebx + 16h]
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [ebx + 16h]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 16h], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 18h]
loc_3_0A9F:
        dec    dword ptr [ebx + 0eh]
        push   ebx
        db  0E8h, 0A8h, 02Dh, 0FCh, 0FFh
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_0AC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 3ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        lea    ebx, [edi + 103h]
        cmp    dword ptr [ebx], 2
        je     SHORT loc_3_0B1C
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], 1
        mov    word ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 30h], 300004h
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 2ch], eax
        mov    dword ptr [ebp - 28h], 0ch
        lea    eax, [ebp - 3ch]
        push   eax
        db  0E8h, 065h, 00Eh, 0FCh, 0FFh
        add    esp, 4
        push   802h
        push   ebx
        db  0E8h, 077h, 00Bh, 0FCh, 0FFh
        add    esp, 8
loc_3_0B1C:
        inc    dword ptr [edi + 0eh]
        cmp    byte ptr [esi + 1ch], 0
        jne    NEAR PTR loc_3_0C41
        inc    dword ptr [esi + 18h]
        push   0
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        mov    eax, dword ptr [edi + 3eh]
        push   eax
        call   sub_3_1E80
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_3_0BB0
        cmp    dword ptr [esi + 18h], 2
        jne    SHORT loc_3_0B70
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        push   0
        push   5ah
        push   esi
        db  0E8h, 0FCh, 0FBh, 0FBh, 0FFh
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_3_0B70:
        mov    byte ptr [esi + 1ch], 1
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        lea    eax, [edi + 1ah]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 1eh]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edi + 1eh]
        mov    ebx, 103h
        mov    dword ptr [eax], esi
        mov    dword ptr [edi + 1eh], esi
        sub    dword ptr [esi + 18h], 2
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_0BB0:
        cmp    ebx, 103h
        jne    SHORT loc_3_0BF2
        mov    byte ptr [esi + 1ch], 1
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        lea    eax, [edi + 1ah]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 1eh]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edi + 1eh]
        mov    ebx, 103h
        mov    dword ptr [eax], esi
        mov    dword ptr [edi + 1eh], esi
        sub    dword ptr [esi + 18h], 2
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_3_0BF2:
        cmp    ebx, 0c001000eh
        jne    SHORT loc_3_0C33
        mov    byte ptr [esi + 1ch], 1
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        lea    eax, [edi + 1ah]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 1eh]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edi + 1eh]
        mov    ebx, 103h
        mov    dword ptr [eax], esi
        mov    dword ptr [edi + 1eh], esi
        dec    dword ptr [esi + 18h]
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_3_0C33:
        dec    dword ptr [esi + 18h]
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_3_0C41:
        mov    ebx, 0c0010002h
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_0C60:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [eax + 8]
        inc    dword ptr [esi + 0eh]
        mov    edx, dword ptr [ecx + 10h]
        or     edx, edx
        jne    SHORT loc_3_0C82
        push   ecx
        push   eax
        push   esi
        db  0E8h, 000h, 004h, 000h, 000h
        jmp    SHORT loc_3_0C94
loc_3_0C82:
        cmp    edx, 1
        mov    edi, 10001h
        jne    SHORT loc_3_0C96
        push   ecx
        push   eax
        push   esi
        call   sub_3_10D0
loc_3_0C94:
        mov    edi, eax
loc_3_0C96:
        dec    dword ptr [esi + 0eh]
        push   esi
        db  0E8h, 0B1h, 02Bh, 0FCh, 0FFh
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_0CB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 18h], 3
        lea    edi, [ebp - 8]
        mov    dword ptr [ebp - 8], 0
        mov    word ptr [ebp - 2], 0
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 14h], 0
        mov    ebx, 4
        mov    eax, dword ptr [ebp + 10h]
        cmp    eax, 1010102h
        ja     SHORT loc_3_0D10
        cmp    eax, 1010101h
        jae    NEAR PTR loc_3_0ED0
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_3_0D73
        shl    eax, 2
        db  0FFh, 0A0h, 034h, 010h, 000h, 000h
        db  005h, 000h, 000h, 000h, 000h
loc_3_0D10:
        cmp    eax, 4010104h
        ja     SHORT loc_3_0D52
        je     NEAR PTR loc_3_0FF2
        cmp    eax, 1010103h
        je     NEAR PTR loc_3_0EF0
        cmp    eax, 1010104h
        je     NEAR PTR loc_3_0FB3
        cmp    eax, 4010101h
        jb     SHORT loc_3_0D73
        cmp    eax, 4010102h
        jbe    NEAR PTR loc_3_0ED0
        cmp    eax, 4010103h
        je     NEAR PTR loc_3_0FC0
        jmp    SHORT loc_3_0D73
loc_3_0D52:
        cmp    eax, 4010105h
        je     NEAR PTR loc_3_0FD0
        cmp    eax, 4010106h
        je     NEAR PTR loc_3_0FE0
        cmp    eax, 0ff010207h
        je     NEAR PTR loc_3_0FE9
loc_3_0D73:
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_3_0FF2
        db  090h
loc_3_0D80:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_3_0D95
        db  0BFh, 0E0h, 082h, 001h, 000h
        mov    ebx, 8ch
        jmp    NEAR PTR loc_3_0FF2
loc_3_0D95:
        db  0BFh, 054h, 082h, 001h, 000h
        mov    ebx, 8ch
        jmp    NEAR PTR loc_3_0FF2
loc_3_0DA4:
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 92h], 0
        mov    dword ptr [ebp - 14h], 2
        jne    SHORT loc_3_0DBE
        mov    dword ptr [ebp - 14h], 0
loc_3_0DBE:
        lea    edi, [ebp - 14h]
        mov    ebx, 4
        jmp    NEAR PTR loc_3_0FF2
        db  005h, 000h, 000h, 000h, 000h
loc_3_0DD0:
        lea    edi, [ebp - 18h]
        mov    ebx, 4
        jmp    NEAR PTR loc_3_0FF2
        db  08Dh, 049h, 000h
loc_3_0DE0:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_3_0FF2
        db  08Dh, 064h, 024h, 000h
loc_3_0DF0:
        mov    dword ptr [ebp - 8], 5dch
        jmp    NEAR PTR loc_3_0FF2
        db  08Dh, 064h, 024h, 000h
loc_3_0E00:
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, 7dh
        mov    eax, dword ptr [esi + 10bh]
        sub    edx, edx
        div    ecx
        jmp    NEAR PTR loc_3_0FEF
loc_3_0E17:
        mov    dword ptr [ebp - 8], 17a8h
        jmp    NEAR PTR loc_3_0FF2
loc_3_0E23:
        mov    dword ptr [ebp - 8], 13ech
        jmp    NEAR PTR loc_3_0FF2
        db  090h
loc_3_0E30:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_3_0FF2
        db  08Dh, 064h, 024h, 000h
loc_3_0E40:
        mov    dword ptr [ebp - 8], 6a4h
        jmp    NEAR PTR loc_3_0FF2
        db  08Dh, 064h, 024h, 000h
loc_3_0E50:
        db  0BFh, 084h, 084h, 001h, 000h
        mov    ebx, 0fh
        jmp    NEAR PTR loc_3_0FF2
        db  090h
loc_3_0E60:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_3_0E74
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 3eh]
        mov    eax, dword ptr [eax + 0ch]
        jmp    NEAR PTR loc_3_0FEF
loc_3_0E74:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 8]
        jmp    NEAR PTR loc_3_0FEF
loc_3_0E82:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_3_0E96
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0a9h]
        jmp    NEAR PTR loc_3_0FEF
loc_3_0E96:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        jmp    NEAR PTR loc_3_0FEF
loc_3_0EA1:
        mov    word ptr [ebp - 2], 400h
        lea    edi, [ebp - 2]
        mov    ebx, 2
        jmp    NEAR PTR loc_3_0FF2
loc_3_0EB4:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_3_0FF2
loc_3_0EC0:
        mov    dword ptr [ebp - 8], 6
        jmp    NEAR PTR loc_3_0FF2
        db  08Dh, 064h, 024h, 000h
loc_3_0ED0:
        push   6
        mov    esi, dword ptr [ebp + 8]
        add    esi, 8
        push   esi
        lea    eax, [ebp - 20h]
        push   eax
        db  0E8h, 05Ah, 0F9h, 0FBh, 0FFh
        lea    edi, [ebp - 20h]
        mov    ebx, 6
        jmp    NEAR PTR loc_3_0FF2
        db  090h
loc_3_0EF0:
        cmp    byte ptr [ebp + 14h], 0
        mov    esi, dword ptr [ebp + 8]
        je     SHORT loc_3_0F50
        mov    eax, dword ptr [esi + 3eh]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 1ch], eax
        jae    SHORT loc_3_0F20
        mov    ecx, dword ptr [ebp + 20h]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ebp - 0ch], 0c0010016h
        jmp    NEAR PTR loc_3_0FF2
        db  08Dh, 049h, 000h
loc_3_0F20:
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [esi + 3eh]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_3_23A0
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        sub    edi, edi
        sub    ebx, ebx
        jmp    NEAR PTR loc_3_0FF2
        db  090h
loc_3_0F50:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [esi + 3eh]
        push   eax
        call   sub_3_22A0
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 1ch], eax
        jae    SHORT loc_3_0F80
        mov    ecx, dword ptr [ebp + 20h]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ebp - 0ch], 0c0010016h
        jmp    NEAR PTR loc_3_0FF2
        db  08Bh, 0FFh
loc_3_0F80:
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [esi + 3eh]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_3_22E0
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        sub    edi, edi
        sub    ebx, ebx
        jmp    SHORT loc_3_0FF2
loc_3_0FB3:
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    al, byte ptr [esi + 0adh]
        jmp    SHORT loc_3_0FEF
loc_3_0FC0:
        mov    dword ptr [ebp - 8], 0
        jmp    SHORT loc_3_0FF2
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_0FD0:
        mov    dword ptr [ebp - 8], 3
        jmp    SHORT loc_3_0FF2
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_0FE0:
        mov    dword ptr [ebp - 8], 1
        jmp    SHORT loc_3_0FF2
loc_3_0FE9:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4ah]
loc_3_0FEF:
        mov    dword ptr [ebp - 8], eax
loc_3_0FF2:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_3_1025
        cmp    dword ptr [ebp + 1ch], ebx
        jae    SHORT loc_3_1015
        mov    ecx, dword ptr [ebp + 20h]
        pop    edi
        mov    dword ptr [ecx], ebx
        mov    dword ptr [ebp - 0ch], 0c0010016h
        mov    eax, dword ptr [ebp - 0ch]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
loc_3_1015:
        push   ebx
        push   edi
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        db  0E8h, 01Ch, 0F8h, 0FBh, 0FFh
        mov    ecx, dword ptr [ebp + 24h]
        add    dword ptr [ecx], ebx
loc_3_1025:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 049h, 000h
loc_3_1034:
        db  080h, 00Dh, 000h, 000h, 0A4h, 00Dh, 000h
        db  000h, 0D0h
        db  00Dh, 000h, 000h, 0D0h, 00Dh
        add    byte ptr [eax], al
        loopne loc_3_1053
        add    byte ptr [eax], al
        db  0F0h, 00Dh, 000h, 000h, 000h, 00Eh, 000h, 000h, 017h, 00Eh, 000h
loc_3_1053:
        db  000h, 023h
        push   cs
        add    byte ptr [eax], al
        xor    byte ptr [esi], cl
        add    byte ptr [eax], al
        inc    eax
        push   cs
        add    byte ptr [eax], al
        jae    SHORT loc_3_106F
        add    byte ptr [eax], al
        push   eax
        push   cs
        add    byte ptr [eax], al
        pushad
        push   cs
        add    byte ptr [eax], al
        or     byte ptr [esi], 0
loc_3_106F:
        db  000h, 0A1h, 00Eh, 000h, 000h, 0B4h
        push   cs
        add    byte ptr [eax], al
        jae    SHORT loc_3_1087
        add    byte ptr [eax], al
        ror    byte ptr [esi], 0
        add    byte ptr [ebp - 75h], dl
        in     al, dx
        sub    esp, 8
        push   esi
loc_3_1087:
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], 0
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        mov    esi, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [esi + 1ch]
        push   eax
        mov    eax, dword ptr [esi + 18h]
        push   eax
        push   0
        mov    eax, dword ptr [esi + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_3_0CB0
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 20h], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 24h], ecx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_3_10D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 8], 0
        mov    edi, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esi + 18h]
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 4010104h
        ja     SHORT loc_3_1118
        je     NEAR PTR loc_3_1253
        cmp    eax, 1010eh
        je     SHORT loc_3_1153
        cmp    eax, 1010fh
        je     NEAR PTR loc_3_1198
        cmp    eax, 1010103h
        je     NEAR PTR loc_3_11E0
        jmp    SHORT loc_3_1139
loc_3_1118:
        cmp    eax, 4010105h
        je     NEAR PTR loc_3_12B0
        cmp    eax, 4010106h
        je     NEAR PTR loc_3_12BA
        cmp    eax, 0ff010207h
        je     NEAR PTR loc_3_12F0
loc_3_1139:
        mov    dword ptr [ebp - 8], 0c0010017h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_3_1318
loc_3_1153:
        cmp    edi, 4
        je     SHORT loc_3_1172
        mov    dword ptr [ebp - 8], 0c0010016h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_3_1318
loc_3_1172:
        push   4
        push   ecx
        lea    eax, [ebp - 10h]
        push   eax
        db  0E8h, 0BEh, 0F6h, 0FBh, 0FFh
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        push   esi
        mov    ebx, dword ptr [ebp + 0ch]
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_3_1340
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_3_1318
loc_3_1198:
        cmp    edi, 4
        je     SHORT loc_3_11B7
        mov    dword ptr [ebp - 8], 0c0010016h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_3_1318
loc_3_11B7:
        push   4
        push   ecx
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 079h, 0F6h, 0FBh, 0FFh
        cmp    dword ptr [ebp - 0ch], 5eah
        jbe    NEAR PTR loc_3_1318
        mov    dword ptr [ebp - 8], 0c0000001h
        jmp    NEAR PTR loc_3_1318
        db  08Dh, 064h, 024h, 000h
loc_3_11E0:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        or     edx, edx
        je     SHORT loc_3_1210
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        mov    eax, 0c0010016h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h
loc_3_1210:
        mov    ebx, dword ptr [ebp + 0ch]
        cmp    byte ptr [ebx + 1ch], 0
        jne    SHORT loc_3_1247
        inc    dword ptr [ebx + 18h]
        push   1
        push   ecx
        mov    ecx, 6
        mov    eax, edi
        sub    edx, edx
        div    ecx
        push   eax
        push   esi
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 3eh]
        push   eax
        call   sub_3_1F40
        mov    dword ptr [ebp - 8], eax
        dec    dword ptr [ebx + 18h]
        jmp    NEAR PTR loc_3_1318
loc_3_1247:
        mov    dword ptr [ebp - 8], 0c0010002h
        jmp    NEAR PTR loc_3_1318
loc_3_1253:
        cmp    edi, 6
        je     SHORT loc_3_1272
        mov    dword ptr [ebp - 8], 0c0010016h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_3_1318
loc_3_1272:
        push   6
        mov    ebx, dword ptr [ebp + 0ch]
        push   ecx
        lea    eax, [ebx + 41h]
        push   eax
        db  0E8h, 0BBh, 0F5h, 0FBh, 0FFh
        cmp    word ptr [ebx + 47h], 0
        jne    NEAR PTR loc_3_1318
        mov    ax, word ptr [ebx + 45h]
        mov    word ptr [ebp - 2], ax
        push   eax
        db  0E8h, 066h, 06Ah, 0FCh, 0FFh
        add    esp, 4
        mov    word ptr [ebx + 47h], ax
        or     ax, ax
        jne    SHORT loc_3_1318
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [ebx + 47h], ax
        jmp    SHORT loc_3_1318
loc_3_12B0:
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebx + 39h], eax
        jmp    SHORT loc_3_1318
loc_3_12BA:
        cmp    edi, 4
        je     SHORT loc_3_12D6
        mov    dword ptr [ebp - 8], 0c0010016h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    SHORT loc_3_1318
loc_3_12D6:
        mov    eax, dword ptr [ecx]
        cmp    eax, 1
        je     SHORT loc_3_12E4
        mov    dword ptr [ebp - 8], 0c0000001h
loc_3_12E4:
        mov    ebx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebx + 3dh], eax
        jmp    SHORT loc_3_1318
        db  08Dh, 064h, 024h, 000h
loc_3_12F0:
        cmp    edi, 4
        je     SHORT loc_3_1310
        mov    dword ptr [ebp - 8], 0c0010014h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    SHORT loc_3_1318
        db  08Dh, 064h, 024h, 000h
loc_3_1310:
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebx + 4ah], eax
loc_3_1318:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_3_1328
        mov    dword ptr [esi + 20h], edi
        mov    dword ptr [esi + 24h], 0
loc_3_1328:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_1340:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 14h]
        inc    dword ptr [esi + 0eh]
        test   al, 20h
        je     SHORT loc_3_1359
        mov    byte ptr [esi + 94h], 1
loc_3_1359:
        mov    edi, dword ptr [ebp + 0ch]
        cmp    byte ptr [edi + 1ch], 0
        jne    SHORT loc_3_1385
        inc    dword ptr [edi + 18h]
        push   1
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [edi + 0ch]
        push   eax
        mov    eax, dword ptr [esi + 3eh]
        push   eax
        call   sub_3_2220
        dec    dword ptr [edi + 18h]
        dec    dword ptr [esi + 0eh]
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
loc_3_1385:
        mov    eax, 0c0010002h
        dec    dword ptr [esi + 0eh]
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_13A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 4], 0
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        push   1
        mov    eax, dword ptr [esi + 14h]
        push   eax
        push   0
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_3_0CB0
        mov    edi, eax
        cmp    edi, 0c00000bbh
        jne    NEAR PTR loc_3_14EB
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 1020101h
        ja     SHORT loc_3_1439
        je     NEAR PTR loc_3_1498
        cmp    eax, 20101h
        je     SHORT loc_3_1460
        cmp    eax, 20102h
        je     SHORT loc_3_1470
        cmp    eax, 20103h
        je     SHORT loc_3_1480
        cmp    eax, 20104h
        je     SHORT loc_3_1488
        cmp    eax, 20105h
        je     SHORT loc_3_1490
        mov    edi, 0c0010017h
        jmp    NEAR PTR loc_3_14C7
loc_3_1439:
        cmp    eax, 1020102h
        je     SHORT loc_3_14A0
        cmp    eax, 1020103h
        je     SHORT loc_3_14A8
        cmp    eax, 4010103h
        je     SHORT loc_3_14B0
        cmp    eax, 4010105h
        je     SHORT loc_3_14C0
        mov    edi, 0c0010017h
        jmp    SHORT loc_3_14C7
        db  08Dh, 064h, 024h, 000h
loc_3_1460:
        mov    eax, dword ptr [ebx + 8ah]
        add    eax, dword ptr [ebx + 6ah]
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_3_14C7
        db  08Bh, 0FFh
loc_3_1470:
        mov    eax, dword ptr [ebx + 8eh]
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_3_14C7
        db  005h, 000h, 000h, 000h, 000h
loc_3_1480:
        mov    eax, dword ptr [ebx + 66h]
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_3_14C7
loc_3_1488:
        mov    eax, dword ptr [ebx + 5ah]
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_3_14C7
loc_3_1490:
        mov    eax, dword ptr [ebx + 56h]
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_3_14C7
loc_3_1498:
        mov    eax, dword ptr [ebx + 5eh]
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_3_14C7
loc_3_14A0:
        mov    eax, dword ptr [ebx + 76h]
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_3_14C7
loc_3_14A8:
        mov    eax, dword ptr [ebx + 7ah]
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_3_14C7
loc_3_14B0:
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_3_14C7
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_14C0:
        mov    dword ptr [ebp - 4], 3
loc_3_14C7:
        or     edi, edi
        jne    SHORT loc_3_14EB
        cmp    dword ptr [ebp - 14h], 4
        jb     SHORT loc_3_14E6
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 05Ch, 0F3h, 0FBh, 0FFh
        add    dword ptr [ebp - 8], 4
        jmp    SHORT loc_3_14EB
loc_3_14E6:
        mov    edi, 0c0010016h
loc_3_14EB:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 20h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 24h], eax
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_1510:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 0ch]
        sub    eax, eax
        mov    edx, dword ptr [ecx + 14h]
        cmp    edx, 1010104h
        ja     SHORT loc_3_1570
        cmp    edx, 1010101h
        jae    SHORT loc_3_159D
        cmp    edx, 10113h
        ja     SHORT loc_3_1557
        je     SHORT loc_3_159D
        cmp    edx, 10101h
        jb     SHORT loc_3_1598
        cmp    edx, 1010bh
        jbe    SHORT loc_3_159D
        cmp    edx, 1010dh
        jb     SHORT loc_3_1598
        cmp    edx, 10111h
        jbe    SHORT loc_3_159D
        jmp    SHORT loc_3_1598
loc_3_1557:
        cmp    edx, 20101h
        jb     SHORT loc_3_1598
        cmp    edx, 20105h
        jbe    SHORT loc_3_159D
        jmp    SHORT loc_3_1598
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_1570:
        cmp    edx, 1020101h
        jb     SHORT loc_3_1598
        cmp    edx, 1020103h
        jbe    SHORT loc_3_159D
        cmp    edx, 2010103h
        je     SHORT loc_3_159D
        cmp    edx, 4010103h
        je     SHORT loc_3_159D
        cmp    edx, 4010105h
        je     SHORT loc_3_159D
loc_3_1598:
        mov    eax, 0c0010017h
loc_3_159D:
        or     eax, eax
        jne    SHORT loc_3_15AB
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_3_13A0
loc_3_15AB:
        pop    ebp
        ret    8
        db  090h
loc_3_15B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 0
        mov    edi, dword ptr [esi + 8]
        inc    dword ptr [edi + 0eh]
        cmp    byte ptr [edi + 92h], 0
        jne    SHORT loc_3_1630
        cmp    byte ptr [esi + 1ch], 0
        jne    SHORT loc_3_1620
        inc    dword ptr [esi + 18h]
        mov    ebx, dword ptr [edi + 12h]
        mov    eax, edi
        sub    eax, ebx
        cmp    eax, -12h
        je     SHORT loc_3_160A
loc_3_15E7:
        inc    dword ptr [ebx + 18h]
        push   0
        push   0
        push   40010004h
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        db  0E8h, 0C2h, 0F0h, 0FBh, 0FFh
        dec    dword ptr [ebx + 18h]
        mov    ebx, dword ptr [ebx]
        mov    eax, edi
        sub    eax, ebx
        cmp    eax, -12h
        jne    SHORT loc_3_15E7
loc_3_160A:
        push   8
        push   0
        push   esi
        push   edi
        call   sub_3_16F0
        dec    dword ptr [esi + 18h]
        jmp    SHORT loc_3_1637
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_1620:
        mov    dword ptr [ebp - 4], 0c0010002h
        jmp    SHORT loc_3_1637
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_1630:
        mov    dword ptr [ebp - 4], 0
loc_3_1637:
        dec    dword ptr [edi + 0eh]
        push   edi
        db  0E8h, 010h, 022h, 0FCh, 0FFh
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, 0ffffffffh
        cmp    dword ptr [ebp + 8], 0
        je     SHORT loc_3_1692
        mov    esi, dword ptr [ebp + 0ch]
loc_3_1664:
        mov    dl, byte ptr [esi]
        inc    esi
        mov    ecx, 8
loc_3_1670:
        mov    edi, eax
        shr    edi, 1fh
        add    eax, eax
        mov    bl, dl
        and    ebx, 1
        cmp    ebx, edi
        je     SHORT loc_3_1688
        xor    eax, 4c11db6h
        or     eax, 1
loc_3_1688:
        shr    dl, 1
        dec    ecx
        jne    SHORT loc_3_1670
        dec    dword ptr [ebp + 8]
        jne    SHORT loc_3_1664
loc_3_1692:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_16A0:
        sub    eax, eax
        ret    1ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_16B0:
        sub    eax, eax
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_16C0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 18h]
        dec    eax
        mov    dword ptr [ecx + 18h], eax
        jne    SHORT loc_3_16DE
        mov    eax, dword ptr [ecx + 8]
        dec    dword ptr [eax + 0eh]
        mov    eax, dword ptr [ecx + 8]
        push   eax
        db  0E8h, 072h, 021h, 0FCh, 0FFh
loc_3_16DE:
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_16F0:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [edx + 92h], 1
        mov    byte ptr [edx + 93h], 0
        mov    dword ptr [edx + 99h], eax
        mov    eax, dword ptr [ebp + 0ch]
        or     eax, eax
        mov    dword ptr [edx + 95h], eax
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [edx + 9dh], ecx
        je     SHORT loc_3_1726
        inc    dword ptr [eax + 18h]
loc_3_1726:
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_3_1730:
        push   ebp
        mov    ebp, esp
        push   esi
        cmp    byte ptr [ebp + 0ch], 0
        mov    esi, dword ptr [ebp + 8]
        je     SHORT loc_3_1770
        push   0
        mov    eax, dword ptr [esi + 99h]
        push   eax
        mov    eax, dword ptr [esi + 95h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 057h, 0EFh, 0FBh, 0FFh
        mov    eax, dword ptr [esi + 95h]
        mov    dword ptr [esi + 99h], 0
        dec    dword ptr [eax + 18h]
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_3_1770:
        push   0c0000001h
        mov    eax, dword ptr [esi + 99h]
        push   eax
        mov    eax, dword ptr [esi + 95h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 021h, 0EFh, 0FBh, 0FFh
        mov    eax, dword ptr [esi + 95h]
        mov    dword ptr [esi + 99h], 0
        dec    dword ptr [eax + 18h]
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_17B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [esi + 0c7h]
        add    ebx, 43h
        and    ebx, 0fffffffch
        mov    eax, ebx
        imul   eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 0ch]
        lea    eax, [eax + ecx*4 + 10h]
        mov    dword ptr [ebp - 0ch], eax
        and    dword ptr [ebp - 0ch], 0fffffffch
        db  0A1h, 050h, 082h, 001h, 000h
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 065h, 0EFh, 0FBh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_3_1805
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_3_1805:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 0afh], eax
        mov    edx, eax
        mov    edi, dword ptr [ebp - 8]
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        mov    ecx, dword ptr [ebp - 8]
        lea    eax, [esi + 0bbh]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [esi + 0bfh]
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [esi + 0bfh]
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0bfh], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 8], ecx
        sub    edx, edx
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ecx*4]
        add    eax, dword ptr [ebp - 8]
        add    eax, 0ch
        mov    dword ptr [ebp - 4], eax
        cmp    ecx, edx
        je     SHORT loc_3_18D5
        lea    ecx, [esi + 0b3h]
loc_3_1879:
        mov    edi, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edi + edx*4 + 0ch], eax
        mov    eax, dword ptr [esi + 0c7h]
        mov    edi, dword ptr [ebp - 4]
        add    eax, 10h
        mov    dword ptr [edi + 18h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    edi, eax
        mov    dword ptr [edi + 1ch], eax
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [eax + 1ch], 30h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], edx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    edi, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 0b7h]
        mov    dword ptr [edi + 4], eax
        inc    edx
        mov    edi, dword ptr [esi + 0b7h]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0b7h], eax
        add    dword ptr [ebp - 4], ebx
        cmp    dword ptr [ebp + 0ch], edx
        ja     SHORT loc_3_1879
loc_3_18D5:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh
PPPMAC_PM_API:
        xchg   esi, eax
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        mov    eax, dword ptr [esi + 4]
        mov    word ptr [ebp + 14h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 18h], ax
        xchg   edi, eax
        mov    ax, 1814h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        mov    eax, dword ptr [esi]
        cmp    eax, 300018h
        ja     SHORT loc_3_1937
        cmp    eax, 300000h
        jb     SHORT loc_3_1937
        mov    ebx, dword ptr [esi + 8]
        push   ebx
        push   edi
        push   eax
        db  0E8h, 04Fh, 000h, 0FCh, 0FFh
        add    esp, 0ch
        cmp    eax, 0
        jne    SHORT loc_3_192B
        xor    eax, eax
loc_3_192B:
        mov    word ptr [ebp + 1ch], ax
        shr    eax, 10h
        mov    word ptr [ebp + 14h], ax
        ret
loc_3_1937:
        mov    word ptr [ebp + 1ch], 0ffffh
        mov    word ptr [ebp + 14h], 0ffffh
        ret
PPPMAC_W32_DEVICEIOCONTROL:
        mov    ecx, dword ptr [esi + 0ch]
        cmp    ecx, 0
        jne    SHORT loc_3_194E
        jmp    SHORT loc_3_1961
loc_3_194E:
        cmp    ecx, -1
        je     SHORT loc_3_1961
        push   esi
        db  0E8h, 017h, 000h, 0FCh, 0FFh
        add    esp, 4
        cmp    eax, 0
        jne    SHORT loc_3_1963
loc_3_1961:
        xor    eax, eax
loc_3_1963:
        clc
        ret
        db  0C3h, 066h, 0C7h, 045h, 01Ch, 000h, 000h, 0C3h, 000h, 000h, 000h
sub_3_1970:
        push   esi
        push   edi
        db  0CDh, 020h, 000h, 000h, 087h, 004h
        jb     SHORT loc_3_1988
        cmp    ax, 30ah
        jb     SHORT loc_3_1988
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        ret
loc_3_1988:
        xor    eax, eax
        pop    edi
        pop    esi
        ret
sub_3_198D:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        push   edx
        mov    eax, dword ptr [ebp + 0ch]
        or     eax, eax
        je     SHORT loc_3_19A2
        mov    ebx, 1
        jmp    SHORT loc_3_19A7
loc_3_19A2:
        mov    ebx, 2
loc_3_19A7:
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 003h, 000h, 087h, 004h
        pop    edx
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_19B6:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        push   edx
        mov    ebx, dword ptr [ebp + 10h]
        or     ebx, ebx
        je     SHORT loc_3_19CB
        mov    ebx, 5
        jmp    SHORT loc_3_19D0
loc_3_19CB:
        mov    ebx, 3
loc_3_19D0:
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 003h, 000h, 087h, 004h
        mov    ebx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebx], edx
        pop    edx
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_19E4:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        push   edx
        mov    ebx, 4
        mov    edx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 003h, 000h, 087h, 004h
        pop    edx
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
        db  000h, 000h
sub_3_1A04:
        push   ebx
        push   esi
        push   edi
        db  0CDh, 020h, 000h, 000h, 088h, 004h
        jae    SHORT loc_3_1A18
loc_3_1A0F:
        mov    eax, 0
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_1A18:
        db  0CDh, 020h, 000h, 000h, 089h, 004h
        jae    SHORT loc_3_1A22
        jmp    SHORT loc_3_1A0F
loc_3_1A22:
        db  0B8h, 000h, 000h, 000h, 000h
        push   eax
        db  0CDh, 020h, 008h, 000h, 088h, 004h
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_1A35:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0CDh, 020h, 004h, 000h, 089h, 004h
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h
sub_3_1A5C:
        push   ebx
        push   esi
        push   edi
        db  0CDh, 020h, 000h, 000h, 014h, 000h
        jb     SHORT loc_3_1A77
        cmp    eax, 30ah
        jb     SHORT loc_3_1A77
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_1A77:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_1A7D:
        push   ebx
        push   esi
        push   edi
        db  0CDh, 020h, 003h, 000h, 014h, 000h
        and    eax, 0ffffh
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_1A8F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        cld
        db  0CDh, 020h, 002h, 000h, 014h, 000h
        cld
        and    eax, 0ffh
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_3_1AAA:
        push   ebp
        mov    ebp, esp
        add    esp, -50h
        push   ebx
        push   esi
        push   edi
        db  0CDh, 020h, 0B6h, 000h, 001h, 000h
        cld
        mov    esi, edx
        lea    edi, [ebp - 50h]
        rep movsb
        mov    esi, edi
        db  0BFh, 008h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    edi, esi
        db  0BEh, 008h, 000h, 000h, 000h
        rep movsb
        lea    edx, [ebp - 50h]
        mov    eax, 1
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        jb     SHORT loc_3_1AFB
        db  0A3h, 01Eh, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_3_1AFB:
        xor    eax, eax
        db  0A3h, 01Eh, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_3_1B07:
        push   ebx
        push   esi
        push   edi
        mov    ebx, 0
        db  0BAh, 015h, 000h, 000h, 000h
        db  0CDh, 020h, 002h, 000h, 027h, 000h
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_1B1E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, 23h
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 0ch], edx
        db  08Bh, 00Dh, 01Eh, 000h, 000h, 000h
        db  0CDh, 020h, 047h, 001h, 001h, 000h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_3_1B43:
        push   ebp
        mov    ebp, esp
        add    esp, 0ffffff5ch
        push   ebx
        push   esi
        push   edi
        db  0CDh, 020h, 0B6h, 000h, 001h, 000h
        cld
        mov    eax, ecx
        add    eax, dword ptr [ebp + 0ch]
        cmp    eax, 0a1h
        jg     SHORT loc_3_1B8C
        mov    esi, edx
        lea    edi, [ebp - 0a1h]
        rep movsb
        mov    ecx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        rep movsb
        lea    edx, [ebp - 0a1h]
        mov    eax, 1
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        jb     SHORT loc_3_1B8C
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_3_1B8C:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_3_1B93:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, 0
        mov    edx, dword ptr [ebp + 8]
        db  0CDh, 020h, 002h, 000h, 027h, 000h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_3_1BAC:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 015h, 000h, 001h, 000h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_3_1BC0:
        push   ebp
        mov    ebp, esp
        add    esp, -50h
        push   ebx
        push   esi
        push   edi
        db  0CDh, 020h, 0B6h, 000h, 001h, 000h
        cld
        mov    esi, edx
        lea    edi, [ebp - 50h]
        rep movsb
        mov    esi, edi
        db  0BFh, 024h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    edi, esi
        db  0BEh, 024h, 000h, 000h, 000h
        rep movsb
        lea    edx, [ebp - 50h]
        mov    eax, 1
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        jb     SHORT loc_3_1C16
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 003h, 000h, 09Dh, 004h
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_3_1C16:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_3_1C1D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 002h, 000h, 09Dh, 004h
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_3_1C34:
        db  0CDh, 020h, 001h, 080h, 09Dh, 004h
sub_3_1C3A:
        db  0CDh, 020h, 004h, 080h, 09Dh, 004h
sub_3_1C40:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  08Bh, 01Dh, 0B0h, 098h, 001h, 000h
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   32h
        push   eax
        db  0E8h, 0F9h, 0EAh, 0FBh, 0FFh
        mov    ecx, dword ptr [ebp + 20h]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     SHORT loc_3_1C74
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_3_1C74:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0ch
        rep stosd
        stosw
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, eax
        jne    SHORT loc_3_1C8E
        mov    esi, 2
loc_3_1C8E:
        push   ebx
        push   0
        lea    eax, [esi + esi*2]
        shl    eax, 2
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 0B5h, 0EAh, 0FBh, 0FFh
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 4], ecx
        or     eax, eax
        je     SHORT loc_3_1CC2
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_3_1D60
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_3_1CC2:
        push   ebx
        push   0
        lea    eax, [esi*8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        db  0E8h, 07Dh, 0EAh, 0FBh, 0FFh
        or     eax, eax
        mov    eax, dword ptr [ebp - 4]
        je     SHORT loc_3_1CF1
        push   eax
        call   sub_3_1D60
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_3_1CF1:
        mov    dword ptr [eax + 28h], 0ffffffffh
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [eax + 2ch], edx
        mov    dl, byte ptr [ecx + 4]
        mov    byte ptr [eax + 30h], dl
        add    eax, 2ch
        mov    cl, byte ptr [ecx + 5]
        mov    byte ptr [eax + 5], cl
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 14h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 18h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        mov    dword ptr [eax + 24h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 20h], esi
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_3_1D60:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_3_1D80
        mov    ecx, dword ptr [esi + 20h]
        push   0
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 2
        push   ecx
        push   eax
        db  0E8h, 0DCh, 0E9h, 0FBh, 0FFh
loc_3_1D80:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     SHORT loc_3_1D96
        push   0
        mov    ecx, dword ptr [esi + 20h]
        shl    ecx, 3
        push   ecx
        push   eax
        db  0E8h, 0C6h, 0E9h, 0FBh, 0FFh
loc_3_1D96:
        push   0
        push   32h
        push   esi
        db  0E8h, 0BCh, 0E9h, 0FBh, 0FFh
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_1DB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 28h], 0
        jne    SHORT loc_3_1DC9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_3_1DC9:
        db  0A1h, 0B0h, 098h, 001h, 000h
        push   eax
        push   0
        push   1ah
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 07Ah, 0E9h, 0FBh, 0FFh
        or     eax, eax
        je     SHORT loc_3_1DE9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_3_1DE9:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 28h]
        mov    edi, edi
loc_3_1DF0:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    SHORT loc_3_1E03
        inc    ecx
        cmp    ecx, 20h
        jb     SHORT loc_3_1DF0
        jmp    SHORT loc_3_1E12
loc_3_1E03:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 28h], eax
loc_3_1E12:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 10h]
        mov    dword ptr [eax + 10h], ecx
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        je     SHORT loc_3_1E28
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], ecx
loc_3_1E28:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 10h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [ebp - 8]
        mov    byte ptr [eax + 19h], cl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ch], 1
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        mov    byte ptr [eax + 18h], 0
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    al, 1
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Bh, 0FFh
sub_3_1E80:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_3_2220
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_3_1EA9
        cmp    ebx, 103h
        jne    SHORT loc_3_1EC0
loc_3_1EA9:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   edi
        push   esi
        call   sub_3_1F40
        or     eax, eax
        je     SHORT loc_3_1EC0
        mov    ebx, eax
loc_3_1EC0:
        or     ebx, ebx
        je     SHORT loc_3_1ECC
        cmp    ebx, 103h
        jne    SHORT loc_3_1F2B
loc_3_1ECC:
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_3_1F26
        mov    eax, dword ptr [edi + 10h]
        or     eax, eax
        je     SHORT loc_3_1EE2
        mov    ecx, dword ptr [edi + 14h]
        mov    dword ptr [eax + 14h], ecx
loc_3_1EE2:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_3_1EF1
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_3_1EF7
loc_3_1EF1:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_3_1EF7:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_3_1F06
        mov    eax, dword ptr [edi + 4]
        push   eax
        db  0E8h, 0B2h, 0E7h, 0FBh, 0FFh
loc_3_1F06:
        mov    eax, 1
        mov    cl, byte ptr [edi + 19h]
        shl    eax, cl
        or     dword ptr [esi + 28h], eax
        push   0
        push   1ah
        push   edi
        db  0E8h, 03Fh, 0E8h, 0FBh, 0FFh
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_3_1F26:
        mov    ebx, 0c001000eh
loc_3_1F2B:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_1F40:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [esi + 20h]
        lea    ecx, [eax + eax*2]
        add    ecx, ecx
        add    ecx, dword ptr [esi + 4]
        mov    dword ptr [ebp - 20h], ecx
        mov    ecx, dword ptr [esi + 8]
        lea    eax, [ecx + eax*4]
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [esi + 24h]
        shl    eax, 2
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        db  0E8h, 0E1h, 0E8h, 0FBh, 0FFh
        mov    eax, dword ptr [esi + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        sub    edi, edi
        push   eax
        db  0E8h, 0C9h, 0E8h, 0FBh, 0FFh
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [ebp - 0ch], eax
        cmp    eax, edi
        je     SHORT loc_3_1FC4
        sub    eax, eax
        nop
loc_3_1FA0:
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 1ch], ecx
        mov    edx, dword ptr [ebp + 0ch]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 19h]
        shl    ebx, cl
        not    ebx
        mov    ecx, dword ptr [ebp - 1ch]
        and    dword ptr [eax + ecx], ebx
        add    eax, 4
        inc    edi
        cmp    dword ptr [esi + 24h], edi
        ja     SHORT loc_3_1FA0
loc_3_1FC4:
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [esi + 24h], 0
        jbe    SHORT loc_3_202F
loc_3_1FD1:
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx*4], 0
        jne    SHORT loc_3_2024
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        mov    eax, dword ptr [esi + 24h]
        sub    eax, dword ptr [ebp - 4]
        add    ecx, dword ptr [esi + 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        sub    eax, 6
        push   eax
        lea    eax, [ecx + 6]
        push   eax
        push   ecx
        db  0E8h, 05Eh, 0E8h, 0FBh, 0FFh
        mov    ecx, dword ptr [ebp - 4]
        shl    ecx, 2
        add    ecx, dword ptr [esi + 8]
        mov    eax, dword ptr [esi + 24h]
        sub    eax, dword ptr [ebp - 4]
        lea    eax, [eax*4 - 4]
        push   eax
        lea    eax, [ecx + 4]
        push   eax
        push   ecx
        db  0E8h, 03Dh, 0E8h, 0FBh, 0FFh
        dec    dword ptr [esi + 24h]
        jmp    SHORT loc_3_2027
loc_3_2024:
        inc    dword ptr [ebp - 4]
loc_3_2027:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 24h], eax
        ja     SHORT loc_3_1FD1
loc_3_202F:
        mov    dword ptr [ebp - 10h], 0
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_3_211A
        mov    edi, dword ptr [ebp + 18h]
loc_3_2043:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        call   sub_3_2400
        or     al, al
        je     SHORT loc_3_207A
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 18h], ecx
        mov    edx, dword ptr [ebp + 0ch]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 19h]
        shl    ebx, cl
        mov    ecx, dword ptr [ebp - 18h]
        or     dword ptr [ecx + eax*4], ebx
        jmp    NEAR PTR loc_3_2108
loc_3_207A:
        mov    ecx, dword ptr [esi + 24h]
        cmp    dword ptr [esi + 20h], ecx
        jbe    NEAR PTR loc_3_2154
        mov    eax, dword ptr [ebp - 4]
        sub    ecx, dword ptr [ebp - 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        add    eax, dword ptr [esi + 4]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        push   ecx
        push   eax
        add    eax, 6
        push   eax
        db  0E8h, 0B8h, 0E7h, 0FBh, 0FFh
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        add    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [eax], ecx
        mov    cl, byte ptr [edi + 4]
        mov    byte ptr [eax + 4], cl
        mov    cl, byte ptr [edi + 5]
        mov    byte ptr [eax + 5], cl
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 2
        add    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [esi + 24h]
        sub    ecx, dword ptr [ebp - 4]
        shl    ecx, 2
        push   ecx
        push   eax
        add    eax, 4
        push   eax
        db  0E8h, 080h, 0E7h, 0FBh, 0FFh
        mov    edx, dword ptr [esi + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + eax*4], 0
        mov    ebx, 1
        mov    edx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 14h], edx
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [edx + 19h]
        mov    edx, dword ptr [ebp - 14h]
        shl    ebx, cl
        or     dword ptr [edx + eax*4], ebx
        inc    dword ptr [esi + 24h]
loc_3_2108:
        add    edi, 6
        inc    dword ptr [ebp - 10h]
        mov    eax, dword ptr [ebp - 10h]
        cmp    eax, dword ptr [ebp + 14h]
        jb     NEAR PTR loc_3_2043
loc_3_211A:
        mov    eax, dword ptr [esi + 24h]
        sub    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], eax
        sub    edi, edi
        cmp    dword ptr [ebp - 0ch], edi
        je     SHORT loc_3_21A5
        mov    edx, dword ptr [ebp - 20h]
        sub    ecx, ecx
        nop
loc_3_2130:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_3_21AB
        mov    eax, dword ptr [esi + 4]
        add    eax, ecx
        mov    ebx, dword ptr [eax + 2]
        cmp    dword ptr [edx + 2], ebx
        jne    SHORT loc_3_2192
        mov    ax, word ptr [eax]
        cmp    word ptr [edx], ax
        jne    SHORT loc_3_2192
        mov    dword ptr [ebp - 8], 0
        jmp    SHORT loc_3_2199
loc_3_2154:
        mov    eax, dword ptr [ebp - 0ch]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 0F2h, 0E6h, 0FBh, 0FFh
        mov    eax, dword ptr [ebp - 0ch]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        db  0E8h, 0DEh, 0E6h, 0FBh, 0FFh
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 24h], eax
        mov    eax, 0c0010009h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_3_2192:
        mov    dword ptr [ebp - 8], 1
loc_3_2199:
        add    edx, 6
        add    ecx, 6
        inc    edi
        cmp    edi, dword ptr [ebp - 0ch]
        jb     SHORT loc_3_2130
loc_3_21A5:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_3_2210
loc_3_21AB:
        mov    al, byte ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [edx]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   dword ptr [esi + 14h]
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_3_2212
        cmp    edi, 103h
        je     SHORT loc_3_2212
        mov    eax, dword ptr [ebp - 0ch]
        lea    eax, [eax + eax*2]
        lea    ebx, [eax*2]
        push   ebx
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 067h, 0E6h, 0FBh, 0FFh
        push   ebx
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 059h, 0E6h, 0FBh, 0FFh
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Bh, 0FFh
loc_3_2210:
        sub    edi, edi
loc_3_2212:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 049h, 000h
sub_3_2220:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [esi + 0ch]
        mov    ecx, dword ptr [edi + 8]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        mov    dword ptr [esi + 0ch], 0
        je     SHORT loc_3_225D
        lea    esp, [esp]
loc_3_2250:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 0ch], ecx
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_3_2250
loc_3_225D:
        mov    ecx, dword ptr [esi + 0ch]
        cmp    ebx, ecx
        je     SHORT loc_3_2291
        mov    al, byte ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        push   ecx
        push   ebx
        call   dword ptr [esi + 18h]
        or     eax, eax
        je     SHORT loc_3_2293
        cmp    eax, 103h
        je     SHORT loc_3_2293
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 0ch], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_3_2291:
        sub    eax, eax
loc_3_2293:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h
sub_3_22A0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        sub    eax, eax
        mov    cl, byte ptr [ecx + 19h]
        mov    edx, dword ptr [ebp + 8]
        and    ecx, 0ffh
        mov    edi, dword ptr [edx + 24h]
        cmp    edi, eax
        je     SHORT loc_3_22D2
        mov    esi, dword ptr [edx + 8]
        mov    edx, 1
        shl    edx, cl
loc_3_22C7:
        test   dword ptr [esi], edx
        je     SHORT loc_3_22CC
        inc    eax
loc_3_22CC:
        add    esi, 4
        dec    edi
        jne    SHORT loc_3_22C7
loc_3_22D2:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_3_22E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ebp + 10h]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 19h]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebx + 24h], 0
        jbe    SHORT loc_3_235C
        mov    edi, dword ptr [ebp + 1ch]
        mov    dword ptr [ebp - 8], 0
        sub    esi, esi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 10h], eax
        mov    edi, edi
loc_3_2320:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 10h]
        test   dword ptr [ecx + esi], eax
        je     SHORT loc_3_234A
        cmp    dword ptr [ebp + 14h], 6
        jb     SHORT loc_3_2376
        sub    dword ptr [ebp + 14h], 6
        push   6
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        db  0E8h, 018h, 0E5h, 0FBh, 0FFh
        add    edi, 6
        inc    dword ptr [ebp - 0ch]
loc_3_234A:
        add    dword ptr [ebp - 8], 6
        add    esi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebx + 24h], eax
        ja     SHORT loc_3_2320
loc_3_235C:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [eax], ecx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_3_2376:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    dword ptr [eax], 0c0000001h
        mov    eax, dword ptr [ebp + 18h]
        pop    esi
        mov    dword ptr [eax], 0
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_23A0:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 24h]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        cmp    ecx, dword ptr [ebp + 10h]
        jbe    SHORT loc_3_23D0
        mov    dword ptr [eax], 0c0000001h
        mov    eax, dword ptr [ebp + 14h]
        pop    ebp
        mov    dword ptr [eax], 0
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_23D0:
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [edx + 24h]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [edx + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [edx + 4]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        db  0E8h, 068h, 0E4h, 0FBh, 0FFh
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_3_2400:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    ecx, ecx
        mov    esi, dword ptr [ebp + 8]
        mov    edx, esi
        shr    edx, 1
        cmp    esi, ecx
        je     NEAR PTR loc_3_2499
        dec    esi
        cmp    esi, edx
        jb     SHORT loc_3_2499
        nop
loc_3_2420:
        cmp    edx, ecx
        jb     SHORT loc_3_2499
        mov    eax, dword ptr [ebp + 0ch]
        lea    edi, [edx + edx*2]
        lea    edi, [eax + edi*2]
        mov    eax, dword ptr [ebp + 10h]
        mov    ebx, dword ptr [eax + 2]
        mov    eax, dword ptr [edi + 2]
        cmp    ebx, eax
        jae    SHORT loc_3_2443
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_3_2477
loc_3_2443:
        jbe    SHORT loc_3_2450
        mov    dword ptr [ebp - 4], 0ffffffffh
        jmp    SHORT loc_3_2477
        db  08Bh, 0FFh
loc_3_2450:
        mov    eax, dword ptr [ebp + 10h]
        mov    bx, word ptr [eax]
        mov    ax, word ptr [edi]
        cmp    bx, ax
        jae    SHORT loc_3_2467
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_3_2477
loc_3_2467:
        mov    dword ptr [ebp - 4], 0ffffffffh
        ja     SHORT loc_3_2477
        mov    dword ptr [ebp - 4], 0
loc_3_2477:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_3_24A9
        jle    SHORT loc_3_2488
        or     edx, edx
        je     SHORT loc_3_2499
        lea    esi, [edx - 1]
        jmp    SHORT loc_3_248B
loc_3_2488:
        lea    ecx, [edx + 1]
loc_3_248B:
        mov    eax, esi
        sub    eax, ecx
        inc    eax
        shr    eax, 1
        lea    edx, [ecx + eax]
        cmp    esi, edx
        jae    SHORT loc_3_2420
loc_3_2499:
        mov    eax, dword ptr [ebp + 14h]
        pop    edi
        mov    dword ptr [eax], edx
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_3_24A9:
        mov    eax, dword ptr [ebp + 14h]
        pop    edi
        mov    dword ptr [eax], edx
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
_PTEXT ends
        end
