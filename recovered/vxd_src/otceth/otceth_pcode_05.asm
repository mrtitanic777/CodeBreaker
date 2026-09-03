; otceth_pcode_05.asm -- OTCETH object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_5_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 4ch
        push   edi
        mov    word ptr [ebp - 14h], 0
        mov    word ptr [ebp - 12h], 0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0026:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   0
        push   14h
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0039:
        or     eax, eax
        je     SHORT loc_5_0050
        mov    eax, 0c0000001h
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0050:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        lea    edi, [ebp - 4ch]
        mov    dword ptr [eax + 0ch], 0
        mov    dword ptr [eax + 10h], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        sub    eax, eax
        mov    ecx, 0dh
        rep stosd
        stosw
        db  0A1h, 004h, 000h, 000h, 000h
        db  08Bh, 00Dh, 008h, 000h, 000h, 000h
        mov    dword ptr [ebp - 1eh], eax
        mov    dword ptr [ebp - 1ah], ecx
        mov    byte ptr [ebp - 4ch], 3
        mov    byte ptr [ebp - 4bh], 0ah
        db  0C7h, 045h, 0BAh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0BEh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0C2h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0C6h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0CAh, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 4ch]
        db  0C7h, 045h, 0CEh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0D2h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0D6h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0DAh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0DEh, 000h, 000h, 000h, 000h
        push   36h
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 4
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0108:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_5_0132
        push   0
        push   14h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_011B:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0126:
        mov    eax, 0c0000001h
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_0132:
        call   sub_5_3330
        mov    dword ptr [ebp - 0ch], eax
        or     eax, eax
        je     SHORT loc_5_0162
        push   0
        push   14h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_014B:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0156:
        mov    eax, 0c0000001h
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_0162:
        sub    eax, eax
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_5_0170:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0188:
        mov    edi, dword ptr [esi]
        push   0
        push   14h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0194:
        push   0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_019C:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_01B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 94h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 64h], 7
        mov    word ptr [ebp - 62h], 7
        db  0C7h, 045h, 0A0h, 010h, 000h, 000h, 000h
        mov    word ptr [ebp - 5ch], 0ch
        mov    word ptr [ebp - 5ah], 0ch
        db  0C7h, 045h, 0A8h, 024h, 000h, 000h, 000h
        mov    word ptr [ebp - 54h], 0dh
        mov    word ptr [ebp - 52h], 0dh
        db  0C7h, 045h, 0B0h, 040h, 000h, 000h, 000h
        mov    word ptr [ebp - 4ch], 0ah
        mov    word ptr [ebp - 4ah], 0ah
        db  0C7h, 045h, 0B8h, 05Ch, 000h, 000h, 000h
        mov    word ptr [ebp - 44h], 0ch
        mov    word ptr [ebp - 42h], 0ch
        db  0C7h, 045h, 0C0h, 084h, 000h, 000h, 000h
        mov    word ptr [ebp - 24h], 0bh
        mov    word ptr [ebp - 22h], 0bh
        db  0C7h, 045h, 0E0h, 09Ch, 000h, 000h, 000h
        mov    word ptr [ebp - 2ch], 8
        mov    word ptr [ebp - 2ah], 8
        db  0C7h, 045h, 0D8h, 0B0h, 000h, 000h, 000h
        mov    word ptr [ebp - 3ch], 8
        mov    word ptr [ebp - 3ah], 8
        lea    eax, [ebp - 10h]
        db  0C7h, 045h, 0C8h, 0C0h, 000h, 000h, 000h
        mov    word ptr [ebp - 34h], 5
        mov    word ptr [ebp - 32h], 5
        db  0C7h, 045h, 0D0h, 0D0h, 000h, 000h, 000h
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0278:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_5_0290
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_0290:
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        push   ebx
        push   0
        push   118h
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_02A7:
        or     eax, eax
        je     SHORT loc_5_02C2
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_02B4:
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_5_02C2:
        mov    edi, dword ptr [ebp - 8]
        sub    eax, eax
        mov    ecx, 46h
        rep stosd
        push   ebx
        push   eax
        push   5f0h
        mov    eax, dword ptr [ebp - 8]
        add    eax, 9fh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_02E3:
        or     eax, eax
        je     SHORT loc_5_0310
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_02F0:
        push   0
        push   118h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0300:
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
loc_5_0310:
        mov    eax, dword ptr [ebp - 8]
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [eax + 6eh], ebx
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 76h], 9
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 78h], 300h
        mov    eax, dword ptr [ebp - 8]
        push   1
        mov    dword ptr [eax + 7dh], 5eah
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 81h], 0f8b7h
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 53h], 3
        lea    eax, [ebp - 64h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0360:
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_5_09C0
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        mov    dword ptr [ecx + 78h], eax
        push   0
        lea    eax, [ebp - 5ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0393:
        push   0
        lea    eax, [ebp - 54h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_03AA:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0a3h], 2
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_5_03C9
        mov    eax, dword ptr [ebp - 14h]
        mov    cl, byte ptr [eax + 4]
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0a3h], cl
loc_5_03C9:
        lea    edi, [ebp - 94h]
        sub    eax, eax
        mov    ecx, 9
        rep stosd
        stosw
        mov    dword ptr [ebp - 8ah], 8
        mov    dword ptr [ebp - 7eh], 1
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 78h]
        add    eax, 78h
        mov    dword ptr [ebp - 7ah], ecx
        mov    dword ptr [ebp - 76h], 20h
        mov    dword ptr [ebp - 72h], eax
        lea    eax, [ebp - 94h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebx + 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 8
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0421:
        mov    dword ptr [ebp - 0ch], eax
        or     eax, eax
        je     SHORT loc_5_0463
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0431:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   5f0h
        mov    eax, dword ptr [eax + 9fh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0447:
        push   0
        push   118h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0457:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_5_0463:
        push   0
        lea    eax, [ebp - 4ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_047A:
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_5_0960
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    word ptr [ecx + 76h], ax
        lea    eax, [ebp - 44h]
        push   2
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_04A8:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_5_04DB
        push   1
        lea    eax, [ebp - 3ch]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 4
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_04C0:
        or     al, al
        je     NEAR PTR loc_5_0550
        mov    eax, dword ptr [ebp - 8]
        or     word ptr [eax + 81h], 100h
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 7ch], 1
loc_5_04DB:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_5_2C70
        mov    dword ptr [ebp - 0ch], eax
        or     eax, eax
        je     NEAR PTR loc_5_0610
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0aa000004h
        mov    eax, dword ptr [eax + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0502:
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_050E:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_051A:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   5f0h
        mov    eax, dword ptr [eax + 9fh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0530:
        push   0
        push   118h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0540:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
loc_5_0550:
        push   1
        lea    eax, [ebp - 34h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 4
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0562:
        or     al, al
        je     SHORT loc_5_0580
        mov    eax, dword ptr [ebp - 8]
        and    word ptr [eax + 81h], 0feffh
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 7ch], 2
        jmp    NEAR PTR loc_5_04DB
        db  08Bh, 0FFh
loc_5_0580:
        push   1
        lea    eax, [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 4
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0592:
        or     al, al
        je     SHORT loc_5_05B0
        mov    eax, dword ptr [ebp - 8]
        and    word ptr [eax + 81h], 0feffh
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 7ch], 0
        jmp    NEAR PTR loc_5_04DB
        db  08Bh, 0FFh
loc_5_05B0:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0aa00000ah
        mov    eax, dword ptr [eax + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_05C3:
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_05CF:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_05DB:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   5f0h
        mov    eax, dword ptr [eax + 9fh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_05F1:
        push   0
        push   118h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0601:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  090h
loc_5_0610:
        mov    eax, dword ptr [ebp - 8]
        add    dword ptr [eax + 78h], 10h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_5_19B0
        mov    dword ptr [ebp - 0ch], eax
        or     eax, eax
        push   0
        je     SHORT loc_5_0684
        mov    eax, dword ptr [ebp - 8]
        push   0aa000004h
        mov    eax, dword ptr [eax + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_063A:
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0646:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0652:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   5f0h
        mov    eax, dword ptr [eax + 9fh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0668:
        push   0
        push   118h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0678:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_5_0684:
        lea    eax, [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0699:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_5_06F5
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [eax + 4], 1
        jne    SHORT loc_5_06F5
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_06BD:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_5_06F5
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 48h], 1
        cmp    dword ptr [ebp - 1ch], 6
        jne    NEAR PTR loc_5_0820
        mov    eax, dword ptr [ebp - 18h]
        test   byte ptr [eax], 1
        jne    NEAR PTR loc_5_0820
        mov    edx, dword ptr [ebp - 8]
        add    edx, 91h
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edx + 4], ax
loc_5_06F5:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_06FE:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        add    eax, 0ch
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        add    eax, 14h
        mov    dword ptr [ecx + 18h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 1ch], 0
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        add    eax, 1ch
        mov    dword ptr [ecx + 20h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 24h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        add    eax, 24h
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp - 8]
        push   1388h
        mov    dword ptr [eax + 34h], 0
        mov    eax, dword ptr [ebp - 8]
        db  0C7h, 040h, 038h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        mov    dword ptr [ecx + 3ch], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 34h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0785:
        mov    eax, dword ptr [ebp - 8]
        add    eax, 72h
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 30h
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 91h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   10h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_07B2:
        or     al, al
        jne    NEAR PTR loc_5_0880
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0aa000002h
        mov    eax, dword ptr [eax + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_07CD:
        add    esp, 0ch
        lea    eax, [ebp - 1]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 34h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_07E0:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_07EC:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   5f0h
        mov    eax, dword ptr [eax + 9fh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0802:
        push   0
        push   118h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0812:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_5_0820:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0aa000009h
        mov    eax, dword ptr [eax + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0833:
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_083F:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_084B:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   5f0h
        mov    eax, dword ptr [eax + 9fh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0861:
        push   0
        push   118h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0871:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  090h
loc_5_0880:
        mov    eax, dword ptr [ebp - 8]
        push   1
        mov    ax, word ptr [eax + 76h]
        and    eax, 0ffffh
        push   1
        push   eax
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 54h
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_08B4:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_5_0930
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0aa000005h
        mov    eax, dword ptr [eax + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_08CD:
        add    esp, 0ch
        lea    eax, [ebp - 1]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 34h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_08E0:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_08EC:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 72h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_08F8:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   5f0h
        mov    eax, dword ptr [eax + 9fh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_090E:
        push   0
        push   118h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_091E:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_0930:
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [eax + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0942:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_5_1B40
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        mov    word ptr [eax + 6ch], 1
        mov    eax, dword ptr [ebp - 0ch]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_5_0960:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0aa00000ch
        mov    eax, dword ptr [eax + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0973:
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_097F:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_098B:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   5f0h
        mov    eax, dword ptr [eax + 9fh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_09A1:
        push   0
        push   118h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_09B1:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  090h
loc_5_09C0:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0aa00000ch
        mov    eax, dword ptr [eax + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_09D3:
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_09DF:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_09EB:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   5f0h
        mov    eax, dword ptr [eax + 9fh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0A01:
        push   0
        push   118h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0A11:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  090h
loc_5_0A20:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, 2
        mov    cx, word ptr [esi + 78h]
        mov    word ptr [esi + 6ch], 9
        lea    edx, [ecx + 0eh]
        out    dx, ax
        lea    edx, [ecx + 0dh]
        sub    al, al
        out    dx, al
        mov    eax, dword ptr [esi + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0A4C:
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 34h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0A59:
        push   2625a0h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0A63:
        lea    eax, [esi + 54h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0A6C:
        mov    eax, dword ptr [esi + 72h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0A75:
        push   0
        push   5f0h
        mov    eax, dword ptr [esi + 9fh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0A88:
        push   esi
        call   sub_5_2DC0
        mov    eax, dword ptr [esi + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0A97:
        mov    eax, dword ptr [esi + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0AA0:
        push   0
        push   118h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0AAD:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0AC0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 6ch], 9
        mov    cx, word ptr [eax + 78h]
        mov    ax, 2
        lea    edx, [ecx + 0eh]
        out    dx, ax
        lea    edx, [ecx + 0dh]
        sub    al, al
        out    dx, al
        push   3d090h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0AE9:
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_5_0AF0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    word ptr [eax + 6ch], 2
        jne    SHORT loc_5_0B06
        mov    eax, 0c001000dh
        pop    ebp
        ret    8
loc_5_0B06:
        push   0
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_5_2130
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0B20:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 20h]
        cmp    word ptr [esi + 6ch], 9
        jne    SHORT loc_5_0B41
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_5_0B41:
        sub    ebx, ebx
        mov    edx, dword ptr [ebp + 18h]
        cmp    edx, ebx
        je     SHORT loc_5_0B63
        mov    ecx, dword ptr [ebp + 14h]
loc_5_0B50:
        mov    eax, dword ptr [ecx]
        or     eax, eax
        je     SHORT loc_5_0B63
        cmp    eax, 5
        je     SHORT loc_5_0B63
        add    ecx, 4
        inc    ebx
        cmp    edx, ebx
        ja     SHORT loc_5_0B50
loc_5_0B63:
        cmp    edx, ebx
        jne    SHORT loc_5_0B75
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_5_0B75:
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], ebx
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        lea    eax, [ebp - 4]
        push   0
        push   20h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0B8D:
        or     eax, eax
        je     SHORT loc_5_0BB2
        push   0
        push   0aa000002h
        mov    eax, dword ptr [esi + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0BA1:
        add    esp, 0ch
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_5_0BB2:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 8
        rep stosd
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [eax + 18h], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], esi
        mov    eax, dword ptr [ebp - 4]
        add    eax, 1ch
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [esi + 72h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0BE2:
        or     al, al
        jne    SHORT loc_5_0C14
        push   0
        push   0aa000002h
        mov    eax, dword ptr [esi + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0BF6:
        add    esp, 0ch
        push   0
        push   20h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0C06:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_5_0C14:
        inc    dword ptr [esi + 49h]
        cmp    word ptr [esi + 6ch], 2
        mov    eax, dword ptr [ebp - 4]
        jne    SHORT loc_5_0C30
        mov    word ptr [eax + 0ch], 8
        mov    eax, 103h
        jmp    SHORT loc_5_0C38
        db  08Bh, 0FFh
loc_5_0C30:
        mov    word ptr [eax + 0ch], 1
        sub    eax, eax
loc_5_0C38:
        mov    edx, dword ptr [ebp - 4]
        lea    ecx, [esi + 0ch]
        mov    dword ptr [edx], ecx
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 10h]
        mov    dword ptr [edx + 4], ecx
        mov    edx, dword ptr [esi + 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edx], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 10h], ecx
        mov    ecx, dword ptr [ebp + 14h]
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + ebx*4]
        mov    dword ptr [edx + 14h], ecx
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [ecx + 14h], 0
        jne    SHORT loc_5_0C71
        inc    byte ptr [esi + 52h]
        jmp    SHORT loc_5_0C74
loc_5_0C71:
        inc    byte ptr [esi + 51h]
loc_5_0C74:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0C90:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        cmp    word ptr [esi + 0ch], 1
        je     SHORT loc_5_0CB0
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_5_0CB0:
        cmp    word ptr [edi + 6ch], 2
        jne    SHORT loc_5_0CC8
        mov    word ptr [esi + 0ch], 5
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_5_0CC8:
        dec    dword ptr [edi + 49h]
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        cmp    dword ptr [edi + 49h], 0
        jne    SHORT loc_5_0CE6
        push   edi
        call   sub_5_1B40
loc_5_0CE6:
        cmp    dword ptr [esi + 14h], 0
        jne    SHORT loc_5_0CF1
        dec    byte ptr [edi + 52h]
        jmp    SHORT loc_5_0CF4
loc_5_0CF1:
        dec    byte ptr [edi + 51h]
loc_5_0CF4:
        push   0
        mov    eax, dword ptr [esi + 1ch]
        push   eax
        mov    eax, dword ptr [edi + 72h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0D03:
        or     eax, eax
        je     SHORT loc_5_0D12
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_5_0D12:
        push   0
        push   20h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0D1C:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0D30:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 8]
        mov    ax, word ptr [esi + 6ch]
        cmp    ax, 9
        jne    SHORT loc_5_0D57
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_0D57:
        cmp    ax, 2
        je     NEAR PTR loc_5_0E70
        cmp    ax, 3
        je     NEAR PTR loc_5_0E70
        mov    word ptr [esi + 6ch], 3
        mov    edi, dword ptr [esi + 0ch]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0ch
        je     SHORT loc_5_0D9D
loc_5_0D80:
        push   0
        push   0
        push   40010004h
        mov    eax, dword ptr [edi + 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0D92:
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -0ch
        jne    SHORT loc_5_0D80
loc_5_0D9D:
        mov    word ptr [esi + 6ch], 2
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 54h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0DB2:
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, esi
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, -0ch
        je     NEAR PTR loc_5_0E5D
loc_5_0DC6:
        mov    edi, dword ptr [ebp - 4]
        mov    ebx, edi
        push   0
        push   0
        push   40010005h
        mov    eax, dword ptr [ebx + 18h]
        add    ebx, 18h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0DE0:
        mov    eax, dword ptr [ebx]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0DE8:
        mov    eax, edi
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    ax, word ptr [edi + 0ch]
        cmp    ax, 5
        jne    SHORT loc_5_0E37
        dec    dword ptr [esi + 49h]
        mov    ecx, dword ptr [edi + 4]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ecx + 4], eax
        push   0
        mov    eax, dword ptr [edi + 1ch]
        push   eax
        mov    eax, dword ptr [esi + 72h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E1A:
        cmp    eax, 0c001000eh
        je     SHORT loc_5_0E4F
        push   0
        mov    eax, dword ptr [ebx]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E2B:
        push   0
        push   20h
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E35:
        jmp    SHORT loc_5_0E4F
loc_5_0E37:
        cmp    ax, 8
        jne    SHORT loc_5_0E4F
        mov    word ptr [edi + 0ch], 1
        push   0
        push   0
        mov    eax, dword ptr [ebx]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E4F:
        mov    eax, esi
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, -0ch
        jne    NEAR PTR loc_5_0DC6
loc_5_0E5D:
        mov    word ptr [esi + 6ch], 1
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_5_0E70:
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_5_0E80:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 8]
        cmp    word ptr [ecx + 6ch], 2
        jne    SHORT loc_5_0E99
        mov    eax, 0c001000dh
        pop    ebp
        ret    8
loc_5_0E99:
        cmp    word ptr [eax + 0ch], 1
        je     SHORT loc_5_0EA9
        mov    eax, 0c0000001h
        pop    ebp
        ret    8
loc_5_0EA9:
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_5_2130
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_5_0EC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [edx + 8]
        mov    ax, word ptr [esi + 6ch]
        cmp    ax, 9
        jne    SHORT loc_5_0EE3
        mov    eax, 0c0000001h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_0EE3:
        cmp    ax, 2
        jne    SHORT loc_5_0EF5
        mov    eax, 0c001000dh
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_0EF5:
        cmp    word ptr [edx + 0ch], 1
        je     SHORT loc_5_0F08
        mov    eax, 0c0000001h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_0F08:
        mov    ecx, dword ptr [ebp + 0ch]
        lea    eax, [ecx + 24h]
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [eax + 8], edx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 4], ecx
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        push   0
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0F2A:
        cmp    dword ptr [ebp - 8], 5eah
        ja     SHORT loc_5_0F7A
        cmp    dword ptr [ebp - 8], 0eh
        jb     SHORT loc_5_0F7A
        inc    dword ptr [esi + 0a8h]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 0
        mov    ecx, dword ptr [esi + 18h]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 18h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        lea    eax, [ebp - 4]
        push   eax
        push   esi
        call   sub_5_1D40
        or     al, al
        je     SHORT loc_5_0F6E
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   esi
        call   sub_5_1E70
loc_5_0F6E:
        mov    eax, 103h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_0F7A:
        mov    eax, 0c001000fh
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0F90:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [eax + 8]
        mov    esi, dword ptr [edx + 9fh]
        lea    esi, [ecx + esi + 0eh]
        cmp    word ptr [edx + 6ch], 2
        jne    SHORT loc_5_0FC1
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_5_0FC1:
        cmp    word ptr [eax + 0ch], 1
        je     SHORT loc_5_0FD6
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_5_0FD6:
        cmp    byte ptr [edx + 44h], 0
        jne    SHORT loc_5_0FEA
        mov    eax, 0c0010013h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_5_0FEA:
        mov    eax, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [eax + 4]
        sub    edi, dword ptr [ebp + 10h]
        sub    edi, 0eh
        jns    SHORT loc_5_1006
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_5_1006:
        mov    eax, dword ptr [ebp + 1ch]
        sub    ebx, ebx
        mov    dword ptr [eax], ebx
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   ebx
        push   ebx
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1020:
        mov    eax, dword ptr [ebp - 10h]
        cmp    eax, edi
        jge    SHORT loc_5_1029
        mov    edi, eax
loc_5_1029:
        mov    eax, dword ptr [ebp + 14h]
        cmp    eax, edi
        jge    SHORT loc_5_1032
        mov    edi, eax
loc_5_1032:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        je     SHORT loc_5_1060
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 0eh
        push   eax
        push   ecx
        push   edi
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1053:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Bh, 0FFh
loc_5_1060:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_5_10B3
loc_5_1066:
        or     edi, edi
        je     SHORT loc_5_10B3
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_107B:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_5_10A0
        cmp    dword ptr [ebp - 4], edi
        jbe    SHORT loc_5_1089
        mov    dword ptr [ebp - 4], edi
loc_5_1089:
        sub    edi, dword ptr [ebp - 4]
        add    ebx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_109D:
        add    esi, dword ptr [ebp - 4]
loc_5_10A0:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_10AD:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_5_1066
loc_5_10B3:
        mov    eax, dword ptr [ebp + 1ch]
        pop    edi
        mov    dword ptr [eax], ebx
        sub    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_10D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 18h]
        mov    edi, dword ptr [ebp + 10h]
        or     edi, edi
        mov    esi, dword ptr [eax + 8]
        mov    ebx, dword ptr [esi + 78h]
        jne    SHORT loc_5_1139
        lea    edi, [ebx + 0eh]
        mov    dx, di
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        mov    ax, 3
        out    dx, ax
        sub    cx, cx
        sub    eax, eax
loc_5_1100:
        mov    edx, ecx
        and    edx, 0ffffh
        lea    edx, [esi + edx*2]
        inc    cx
        mov    byte ptr [edx + 98h], al
        mov    byte ptr [edx + 97h], al
        lea    edx, [ebx + ecx*2 - 2]
        out    dx, ax
        cmp    cx, 4
        jb     SHORT loc_5_1100
        mov    dx, di
        mov    ax, word ptr [ebp - 2]
        out    dx, ax
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_5_1139:
        mov    eax, dword ptr [ebp + 14h]
        lea    ecx, [edi + edi*2]
        lea    ebx, [eax + ecx*2 - 6]
loc_5_1143:
        push   ebx
        call   sub_5_2040
        push   1
        push   eax
        push   esi
        call   sub_5_20B0
        sub    ebx, 6
        dec    edi
        jne    SHORT loc_5_1143
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_1170:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        sub    si, si
        mov    ecx, dword ptr [eax + 8]
        mov    eax, dword ptr [ebp + 0ch]
        test   al, 1
        je     SHORT loc_5_118C
        mov    si, 100h
loc_5_118C:
        test   al, 2
        je     SHORT loc_5_1194
        mov    si, 100h
loc_5_1194:
        test   al, 8
        je     SHORT loc_5_119C
        mov    si, 100h
loc_5_119C:
        test   al, 4
        je     SHORT loc_5_11A4
        mov    si, 104h
loc_5_11A4:
        test   al, 20h
        je     SHORT loc_5_11B2
        mov    si, 102h
        mov    byte ptr [ecx + 47h], 1
        jmp    SHORT loc_5_11B6
loc_5_11B2:
        mov    byte ptr [ecx + 47h], 0
loc_5_11B6:
        mov    edx, dword ptr [ecx + 78h]
        add    dx, 0eh
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        mov    di, word ptr [ecx + 78h]
        or     si, 200h
        lea    ecx, [edi + 0eh]
        mov    dx, cx
        sub    ax, ax
        out    dx, ax
        lea    edx, [edi + 4]
        mov    ax, si
        out    dx, ax
        mov    dx, cx
        mov    ax, word ptr [ebp - 2]
        out    dx, ax
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_1200:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   0
        mov    eax, dword ptr [esi + 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1212:
        push   0
        push   20h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_121C:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_1230:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        sub    bl, bl
        mov    ecx, dword ptr [edi + 78h]
        lea    esi, [ecx + 0eh]
        lea    edx, [ecx - 1]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        cmp    al, 0ffh
        jne    SHORT loc_5_1260
        mov    word ptr [edi + 6ch], 9
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_1260:
        test   byte ptr [ebp - 1], 2
        je     SHORT loc_5_1292
        mov    dx, si
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    ax, 2
        out    dx, ax
        lea    edx, [ecx + 0ch]
        in     al, dx
        test   al, 3bh
        je     SHORT loc_5_1289
        lea    edx, [ecx + 0dh]
        sub    al, al
        out    dx, al
        mov    byte ptr [edi + 46h], 1
        mov    bl, 1
loc_5_1289:
        mov    dx, si
        mov    ax, word ptr [ebp - 4]
        out    dx, ax
loc_5_1292:
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_5_12A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    byte ptr [esi + 46h], 1
        mov    eax, dword ptr [esi + 78h]
        mov    dword ptr [ebp - 10h], eax
        add    eax, 0ch
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [esi + 42h], 1
        lea    edi, [eax + 0eh]
        dec    ax
        mov    word ptr [ebp - 18h], ax
        mov    dx, ax
        sub    al, al
        out    dx, al
        mov    dx, word ptr [ebp - 18h]
        mov    al, byte ptr [esi + 53h]
        out    dx, al
        mov    dx, di
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        cmp    ah, 33h
        je     SHORT loc_5_12F7
        mov    word ptr [esi + 6ch], 9
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_5_12F7:
        mov    dx, di
        mov    ax, 2
        out    dx, ax
        mov    cx, word ptr [ebp - 10h]
        add    cx, 6
        mov    word ptr [ebp - 20h], cx
        mov    dx, cx
        in     ax, dx
        mov    word ptr [ebp - 0ah], ax
        mov    cx, word ptr [ebp - 10h]
        add    cx, 2
        mov    word ptr [ebp - 1ch], cx
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 2], al
loc_5_1328:
        mov    dx, di
        in     ax, dx
        cmp    ah, 33h
        jne    SHORT loc_5_13A1
        mov    ax, 2
        out    dx, ax
        mov    dx, word ptr [ebp - 14h]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        cmp    byte ptr [esi + 40h], 0
        je     SHORT loc_5_1353
        cmp    byte ptr [esi + 43h], 0
        jne    SHORT loc_5_1353
        mov    bl, al
        and    bl, 3bh
        jmp    SHORT loc_5_1359
loc_5_1353:
        mov    bl, byte ptr [ebp - 1]
        and    bl, 33h
loc_5_1359:
        or     bl, bl
        je     SHORT loc_5_13A7
        test   bl, 1
        je     SHORT loc_5_136A
        push   esi
        call   sub_5_1430
        jmp    SHORT loc_5_1328
loc_5_136A:
        test   bl, 2
        je     SHORT loc_5_1377
        push   esi
        call   sub_5_15D0
        jmp    SHORT loc_5_1328
loc_5_1377:
        test   bl, 8
        je     SHORT loc_5_1384
        push   esi
        call   sub_5_1870
        jmp    SHORT loc_5_1328
loc_5_1384:
        test   bl, 30h
        je     SHORT loc_5_1328
        test   bl, 20h
        je     SHORT loc_5_1394
        push   esi
        call   sub_5_1950
loc_5_1394:
        test   bl, 10h
        je     SHORT loc_5_1328
        push   esi
        call   sub_5_1900
        jmp    SHORT loc_5_1328
loc_5_13A1:
        mov    word ptr [esi + 6ch], 9
loc_5_13A7:
        mov    dx, di
        mov    ax, 2
        out    dx, ax
        mov    dx, word ptr [ebp - 20h]
        mov    ax, word ptr [ebp - 0ah]
        out    dx, ax
        mov    dx, word ptr [ebp - 1ch]
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        mov    byte ptr [ebp - 1], 33h
        cmp    byte ptr [esi + 40h], 0
        je     SHORT loc_5_13D6
        cmp    byte ptr [esi + 43h], 0
        jne    SHORT loc_5_13D6
        mov    byte ptr [ebp - 1], 3bh
loc_5_13D6:
        mov    dx, word ptr [ebp - 10h]
        add    dx, 0dh
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        cmp    byte ptr [esi + 45h], 0
        mov    dx, di
        mov    ax, word ptr [ebp - 8]
        out    dx, ax
        mov    byte ptr [esi + 42h], 0
        mov    dx, word ptr [ebp - 18h]
        in     al, dx
        je     SHORT loc_5_1407
        mov    byte ptr [esi + 45h], 0
        mov    eax, dword ptr [esi + 72h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1407:
        cmp    dword ptr [esi + 14h], 0
        je     SHORT loc_5_1425
        lea    eax, [ebp - 24h]
        push   eax
        push   esi
        call   sub_5_1D40
        or     al, al
        je     SHORT loc_5_1425
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        push   esi
        call   sub_5_1E70
loc_5_1425:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Bh, 0FFh
sub_5_1430:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 78h]
        mov    dword ptr [ebp - 0ch], eax
        add    eax, 8
        mov    dword ptr [ebp - 14h], eax
        mov    ax, word ptr [ebp - 0ch]
        add    ax, 5
        mov    word ptr [ebp - 18h], ax
        mov    dx, ax
        in     al, dx
        test   al, 80h
        jne    NEAR PTR loc_5_15C4
        mov    ax, word ptr [ebp - 0ch]
        add    ax, 6
        mov    word ptr [ebp - 1ch], ax
        lea    esp, [esp]
loc_5_1470:
        mov    ebx, dword ptr [esi + 9fh]
        mov    dx, word ptr [ebp - 1ch]
        mov    ax, 0e000h
        out    dx, ax
        push   2
        push   ebx
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_148C:
        mov    di, word ptr [ebx]
        mov    cx, word ptr [ebx + 2]
        test   di, 0ac00h
        jne    NEAR PTR loc_5_1586
        sub    cx, 6
        test   di, 1000h
        je     SHORT loc_5_14AB
        inc    cx
loc_5_14AB:
        lea    eax, [ecx - 0eh]
        mov    word ptr [ebp - 6], ax
        mov    eax, dword ptr [esi + 9fh]
        mov    ebx, ecx
        and    ebx, 0ffffh
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [ebx + 3]
        sar    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_14D7:
        mov    dx, word ptr [ebp - 0ch]
        mov    ax, 80h
        out    dx, ax
        mov    dword ptr [ebp - 20h], ebx
        mov    dword ptr [ebp - 24h], 0
        mov    byte ptr [esi + 44h], 1
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        push   eax
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        add    eax, 0eh
        push   eax
        push   0eh
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        push   eax
        lea    eax, [ebp - 24h]
        push   eax
        mov    eax, dword ptr [esi + 72h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1512:
        mov    byte ptr [esi + 44h], 0
        mov    byte ptr [esi + 45h], 1
        inc    dword ptr [esi + 0b0h]
        test   di, 4000h
        je     SHORT loc_5_1535
        add    dword ptr [esi + 0e8h], ebx
        inc    dword ptr [esi + 0ech]
        jmp    SHORT loc_5_155C
loc_5_1535:
        test   di, 1
        je     SHORT loc_5_1550
        add    dword ptr [esi + 0e0h], ebx
        inc    dword ptr [esi + 0e4h]
        jmp    SHORT loc_5_155C
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_1550:
        add    dword ptr [esi + 0d8h], ebx
        inc    dword ptr [esi + 0dch]
loc_5_155C:
        mov    ecx, 1
loc_5_1561:
        mov    dx, word ptr [ebp - 0ch]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        test   cx, ax
        jne    SHORT loc_5_1561
        mov    dx, word ptr [ebp - 18h]
        in     al, dx
        test   al, 80h
        je     NEAR PTR loc_5_1470
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_1586:
        inc    dword ptr [esi + 0b8h]
        test   di, 8000h
        je     SHORT loc_5_1599
        inc    dword ptr [esi + 0f4h]
loc_5_1599:
        test   di, 2000h
        je     SHORT loc_5_15A6
        inc    dword ptr [esi + 0f0h]
loc_5_15A6:
        mov    dx, word ptr [ebp - 0ch]
        mov    ax, 80h
        out    dx, ax
        mov    ecx, 1
loc_5_15B5:
        mov    dx, word ptr [ebp - 0ch]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        test   cx, ax
        jne    SHORT loc_5_15B5
loc_5_15C4:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_5_15D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 1ch], 0
        mov    ecx, dword ptr [esi + 78h]
        dec    dword ptr [esi + 0a4h]
        lea    edx, [ecx + 4]
        in     al, dx
        lea    edx, [ecx + 2]
        out    dx, al
        lea    edx, [ecx + 6]
        mov    ax, 6000h
        out    dx, ax
        lea    edx, [ecx + 8]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        in     ax, dx
        mov    word ptr [ebp - 6], ax
        mov    dx, cx
        sub    word ptr [ebp - 6], 6
        mov    ax, 0a0h
        test   word ptr [ebp - 4], 8e10h
        out    dx, ax
        lea    edx, [ecx + 0ch]
        mov    al, 2
        out    dx, al
        je     SHORT loc_5_1673
        mov    dword ptr [ebp - 1ch], 0c0000001h
        inc    dword ptr [esi + 0b4h]
        test   byte ptr [ebp - 3], 80h
        je     SHORT loc_5_1641
        inc    dword ptr [esi + 10ch]
loc_5_1641:
        test   byte ptr [ebp - 3], 4
        je     SHORT loc_5_164D
        inc    dword ptr [esi + 114h]
loc_5_164D:
        test   byte ptr [ebp - 3], 2
        je     SHORT loc_5_1659
        inc    dword ptr [esi + 104h]
loc_5_1659:
        test   byte ptr [ebp - 4], 10h
        je     SHORT loc_5_1665
        inc    dword ptr [esi + 100h]
loc_5_1665:
        test   byte ptr [ebp - 4], 20h
        je     SHORT loc_5_16E6
        inc    dword ptr [esi + 110h]
        jmp    SHORT loc_5_16E6
loc_5_1673:
        inc    dword ptr [esi + 0ach]
        test   byte ptr [ebp - 4], 40h
        je     SHORT loc_5_1693
        xor    eax, eax
        inc    dword ptr [esi + 0d4h]
        mov    ax, word ptr [ebp - 6]
        add    dword ptr [esi + 0d0h], eax
        jmp    SHORT loc_5_16C2
loc_5_1693:
        test   byte ptr [ebp - 4], 8
        je     SHORT loc_5_16B0
        xor    eax, eax
        inc    dword ptr [esi + 0cch]
        mov    ax, word ptr [ebp - 6]
        add    dword ptr [esi + 0c8h], eax
        jmp    SHORT loc_5_16C2
        db  08Dh, 049h, 000h
loc_5_16B0:
        xor    eax, eax
        inc    dword ptr [esi + 0c4h]
        mov    ax, word ptr [ebp - 6]
        add    dword ptr [esi + 0c0h], eax
loc_5_16C2:
        test   byte ptr [ebp - 4], 80h
        je     SHORT loc_5_16CE
        inc    dword ptr [esi + 108h]
loc_5_16CE:
        test   byte ptr [ebp - 4], 4
        je     SHORT loc_5_16DA
        inc    dword ptr [esi + 0fch]
loc_5_16DA:
        test   byte ptr [ebp - 4], 2
        je     SHORT loc_5_16E6
        inc    dword ptr [esi + 0f8h]
loc_5_16E6:
        lea    ecx, [esi + 24h]
        mov    dword ptr [ebp - 14h], 0
        cmp    dword ptr [ecx], 0
        je     SHORT loc_5_170F
        dec    dword ptr [esi + 0a8h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [eax], 0
        jne    SHORT loc_5_170F
        mov    dword ptr [esi + 28h], ecx
loc_5_170F:
        cmp    dword ptr [ebp - 14h], 0
        je     NEAR PTR loc_5_1820
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 24h], eax
        lea    eax, [ebp - 20h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1737:
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1748:
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [esi + 72h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1755:
        or     al, al
        je     NEAR PTR loc_5_1809
        cmp    byte ptr [esi + 47h], 0
        jne    NEAR PTR loc_5_1809
        mov    ebx, dword ptr [ebp - 0ch]
        mov    edi, dword ptr [ebp - 18h]
        mov    eax, dword ptr [ebp - 20h]
        cmp    ebx, eax
        jae    SHORT loc_5_17DD
        mov    eax, ebx
        sub    eax, 0eh
        cmp    eax, dword ptr [esi + 7dh]
        jae    SHORT loc_5_17DD
        sub    ebx, ebx
        mov    edi, dword ptr [esi + 9fh]
loc_5_1786:
        mov    eax, dword ptr [esi + 7dh]
        mov    ecx, dword ptr [ebp - 0ch]
        add    ecx, ebx
        cmp    ecx, eax
        jbe    SHORT loc_5_1797
        sub    eax, ebx
        mov    dword ptr [ebp - 0ch], eax
loc_5_1797:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        lea    eax, [edi + ebx]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_17A8:
        add    ebx, dword ptr [ebp - 0ch]
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_17B8:
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_5_17DD
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_17CF:
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_5_17DD
        lea    eax, [ebx - 0eh]
        cmp    eax, dword ptr [esi + 7dh]
        jb     SHORT loc_5_1786
loc_5_17DD:
        mov    byte ptr [esi + 44h], 1
        mov    eax, dword ptr [ebp - 20h]
        sub    eax, 0eh
        push   eax
        sub    ebx, 0eh
        push   ebx
        lea    eax, [edi + 0eh]
        push   eax
        push   0eh
        push   edi
        push   edi
        lea    eax, [ebp - 24h]
        push   eax
        mov    eax, dword ptr [esi + 72h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1801:
        mov    byte ptr [esi + 44h], 0
        mov    byte ptr [esi + 45h], 1
loc_5_1809:
        mov    ecx, dword ptr [ebp - 14h]
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ecx + 8]
        mov    eax, dword ptr [eax + 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1820:
        cmp    dword ptr [esi + 14h], 0
        je     SHORT loc_5_1844
loc_5_1826:
        lea    eax, [ebp - 14h]
        push   eax
        push   esi
        call   sub_5_1D40
        or     al, al
        je     SHORT loc_5_1844
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        push   esi
        call   sub_5_1E70
        cmp    dword ptr [esi + 14h], 0
        jne    SHORT loc_5_1826
loc_5_1844:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    edx, dword ptr [eax + 78h]
        add    dx, 0ch
        mov    al, 4
        out    dx, al
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1870:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        lea    esi, [edi + 14h]
        mov    ecx, dword ptr [edi + 78h]
        mov    byte ptr [edi + 40h], 0
        lea    edx, [ecx + 3]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     SHORT loc_5_18AB
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax], 0
        jne    SHORT loc_5_18AB
        mov    dword ptr [edi + 18h], esi
loc_5_18AB:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_5_18C6
        mov    eax, dword ptr [ebp - 8]
        mov    cl, byte ptr [ebp - 1]
        mov    byte ptr [eax + 0ch], cl
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_5_1E70
        jmp    SHORT loc_5_18D6
loc_5_18C6:
        lea    edx, [ecx + 2]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    dx, cx
        mov    ax, 0a0h
        out    dx, ax
loc_5_18D6:
        cmp    dword ptr [esi], 0
        je     SHORT loc_5_18F8
loc_5_18DB:
        lea    eax, [ebp - 8]
        push   eax
        push   edi
        call   sub_5_1D40
        or     al, al
        je     SHORT loc_5_18F8
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_5_1E70
        cmp    dword ptr [esi], 0
        jne    SHORT loc_5_18DB
loc_5_18F8:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_5_1900:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        sub    ax, ax
        mov    edi, dword ptr [ecx + 78h]
        lea    esi, [edi + 0eh]
        mov    dx, si
        out    dx, ax
        lea    edx, [edi + 4]
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        and    word ptr [ebp - 2], 0fffeh
        mov    ax, word ptr [ebp - 2]
        inc    dword ptr [ecx + 0bch]
        out    dx, ax
        mov    dx, si
        mov    ax, 2
        out    dx, ax
        lea    edx, [edi + 0ch]
        mov    al, 10h
        out    dx, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_5_1950:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        sub    ax, ax
        mov    esi, dword ptr [edi + 78h]
        lea    ecx, [esi + 0eh]
        mov    dx, cx
        out    dx, ax
        lea    edx, [esi + 2]
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        mov    dx, si
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        or     word ptr [ebp - 4], 1
        mov    ax, word ptr [ebp - 4]
        test   byte ptr [ebp - 2], 10h
        out    dx, ax
        mov    dx, cx
        mov    ax, 2
        out    dx, ax
        je     SHORT loc_5_199B
        inc    dword ptr [edi + 100h]
loc_5_199B:
        test   byte ptr [ebp - 2], 20h
        je     SHORT loc_5_19A7
        inc    dword ptr [edi + 110h]
loc_5_19A7:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_5_19B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, 1
        mov    edi, dword ptr [ecx + 78h]
        lea    esi, [edi + 0eh]
        mov    dx, si
        out    dx, ax
        lea    edx, [edi + 0ah]
        mov    ax, 0ffffh
        out    dx, ax
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        inc    word ptr [ebp - 2]
        je     SHORT loc_5_19F0
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_5_19F0:
        sub    ax, ax
        out    dx, ax
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        or     ax, ax
        je     SHORT loc_5_1A10
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_5_1A10:
        xor    edx, edx
        mov    dl, byte ptr [ecx + 7ch]
        mov    ax, 0f8b7h
        or     edx, edx
        je     SHORT loc_5_1A24
        cmp    edx, 1
        je     SHORT loc_5_1A30
        jmp    SHORT loc_5_1A34
loc_5_1A24:
        mov    ax, 0f8b7h
        jmp    SHORT loc_5_1A34
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_1A30:
        mov    ax, 0f9b7h
loc_5_1A34:
        or     ax, 1000h
        mov    dx, di
        out    dx, ax
        mov    word ptr [ecx + 81h], ax
        mov    dx, si
        mov    ax, 2
        out    dx, ax
        mov    dx, di
        mov    ax, 40h
        out    dx, ax
        mov    dx, si
        sub    ax, ax
        out    dx, ax
        lea    ebx, [edi + 8]
        mov    dx, bx
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        and    eax, 0ffh
        cmp    eax, 0ffh
        jne    SHORT loc_5_1A77
        inc    eax
loc_5_1A77:
        shl    eax, 8
        mov    dword ptr [ecx + 83h], eax
        mov    dx, si
        mov    ax, 2
        out    dx, ax
        mov    dx, di
        mov    ax, 20h
        out    dx, ax
        mov    dx, si
        sub    ax, ax
        out    dx, ax
        mov    dx, bx
        in     ax, dx
        mov    word ptr [ebp - 6], ax
        mov    ax, word ptr [ebp - 4]
        and    eax, 0ff00h
        mov    dx, word ptr [ebp - 6]
        and    edx, 0ff00h
        sub    eax, edx
        mov    dword ptr [ecx + 87h], eax
        mov    al, byte ptr [ecx + 0a3h]
        cmp    al, 9
        jbe    SHORT loc_5_1AD1
        mov    byte ptr [ecx + 0a3h], 8
        jmp    SHORT loc_5_1AE9
loc_5_1AD1:
        cmp    al, 1
        jbe    SHORT loc_5_1AE0
        dec    al
        mov    byte ptr [ecx + 0a3h], al
        jmp    SHORT loc_5_1AE9
        db  090h
loc_5_1AE0:
        jae    SHORT loc_5_1AE9
        mov    byte ptr [ecx + 0a3h], 1
loc_5_1AE9:
        db  0B8h, 000h, 000h, 000h, 000h
        lea    edx, [ecx + 8bh]
        mov    ebx, dword ptr [eax]
        mov    dword ptr [edx], ebx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edx + 4], ax
        cmp    byte ptr [ecx + 48h], 0
        jne    SHORT loc_5_1B18
        add    ecx, 91h
        mov    eax, ebx
        mov    dword ptr [ecx], eax
        mov    ax, word ptr [edx + 4]
        mov    word ptr [ecx + 4], ax
loc_5_1B18:
        mov    dx, si
        mov    ax, 2
        out    dx, ax
        lea    edx, [edi + 0dh]
        sub    al, al
        out    dx, al
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1B40:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, 2
        mov    edi, dword ptr [esi + 78h]
        lea    edx, [edi + 0eh]
        out    dx, ax
        lea    edx, [edi + 0dh]
        sub    ax, ax
        out    dx, ax
        lea    edx, [edi + 0eh]
        out    dx, ax
        lea    edx, [edi + 4]
        mov    ax, 8000h
        out    dx, ax
        mov    ebx, 10h
loc_5_1B70:
        push   2710h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1B7A:
        dec    ebx
        jne    SHORT loc_5_1B70
        lea    edx, [edi + 4]
        sub    ax, ax
        out    dx, ax
        push   2710h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1B8F:
        lea    ecx, [esi + 91h]
        lea    edx, [edi + 0eh]
        mov    ax, 1
        out    dx, ax
        lea    edx, [edi + 4]
        mov    ax, word ptr [ecx]
        out    dx, ax
        add    ecx, 2
        lea    edx, [edi + 6]
        mov    ax, word ptr [ecx]
        out    dx, ax
        lea    edx, [edi + 8]
        mov    ax, word ptr [ecx + 2]
        out    dx, ax
        mov    dx, di
        mov    ax, word ptr [esi + 81h]
        out    dx, ax
        lea    edx, [edi + 0eh]
        mov    ax, 3
        out    dx, ax
        sub    ecx, ecx
loc_5_1BD1:
        lea    edx, [edi + ecx]
        mov    al, byte ptr [esi + ecx + 97h]
        out    dx, al
        inc    ecx
        cmp    ecx, 6
        jl     SHORT loc_5_1BD1
        lea    edx, [edi + 0eh]
        mov    ax, 1
        out    dx, ax
        lea    edx, [edi + 0ch]
        mov    ax, 20h
        out    dx, ax
        lea    edx, [edi + 0eh]
        sub    ax, ax
        out    dx, ax
        mov    dx, di
        mov    ax, 1481h
        out    dx, ax
        lea    edx, [edi + 4]
        mov    ax, 300h
        out    dx, ax
        lea    edx, [edi + 0ah]
        sub    ax, ax
        out    dx, ax
        lea    edx, [edi + 0eh]
        mov    ax, 2
        out    dx, ax
        lea    edx, [edi + 0dh]
        mov    al, 33h
        out    dx, al
        push   esi
        call   sub_5_1C40
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1C40:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        lea    eax, [ebp - 8]
        mov    byte ptr [ecx + 41h], 0
        mov    byte ptr [ecx + 43h], 0
        mov    byte ptr [ecx + 42h], 0
        mov    byte ptr [ecx + 40h], 0
        lea    edx, [ecx + 24h]
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ecx + 0a8h], 0
        mov    dword ptr [ecx + 0a4h], 0
        cmp    dword ptr [edx], 0
        je     SHORT loc_5_1CA4
loc_5_1C83:
        mov    esi, dword ptr [edx]
        mov    eax, dword ptr [esi]
        mov    dword ptr [edx], eax
        cmp    dword ptr [esi], 0
        jne    SHORT loc_5_1C91
        mov    dword ptr [ecx + 28h], edx
loc_5_1C91:
        mov    dword ptr [esi], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [edx], 0
        jne    SHORT loc_5_1C83
loc_5_1CA4:
        lea    edx, [ecx + 1ch]
        cmp    dword ptr [edx], 0
        je     SHORT loc_5_1CCD
loc_5_1CAC:
        mov    esi, dword ptr [edx]
        mov    eax, dword ptr [esi]
        mov    dword ptr [edx], eax
        cmp    dword ptr [esi], 0
        jne    SHORT loc_5_1CBA
        mov    dword ptr [ecx + 20h], edx
loc_5_1CBA:
        mov    dword ptr [esi], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [edx], 0
        jne    SHORT loc_5_1CAC
loc_5_1CCD:
        lea    edx, [ecx + 14h]
        cmp    dword ptr [edx], 0
        je     SHORT loc_5_1CF6
loc_5_1CD5:
        mov    esi, dword ptr [edx]
        mov    eax, dword ptr [esi]
        mov    dword ptr [edx], eax
        cmp    dword ptr [esi], 0
        jne    SHORT loc_5_1CE3
        mov    dword ptr [ecx + 18h], edx
loc_5_1CE3:
        mov    dword ptr [esi], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [edx], 0
        jne    SHORT loc_5_1CD5
loc_5_1CF6:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_5_1D2E
        sub    esi, esi
        mov    edi, edi
loc_5_1D00:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, ecx
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ecx], esi
        jne    SHORT loc_5_1D14
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 4], eax
loc_5_1D14:
        push   0c001000ch
        mov    eax, dword ptr [ecx + 4]
        push   eax
        mov    eax, dword ptr [ecx + 8]
        mov    eax, dword ptr [eax + 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1D29:
        cmp    dword ptr [ebp - 8], esi
        jne    SHORT loc_5_1D00
loc_5_1D2E:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1D40:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 40h], 0
        jne    NEAR PTR loc_5_1E5A
        cmp    dword ptr [esi + 14h], 0
        je     NEAR PTR loc_5_1E5A
        xor    eax, eax
        mov    al, byte ptr [esi + 0a3h]
        cmp    eax, dword ptr [esi + 0a4h]
        jbe    NEAR PTR loc_5_1E5A
        mov    edi, dword ptr [esi + 78h]
        lea    eax, [edi + 0eh]
        mov    dx, ax
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        lea    edx, [edi + 0eh]
        sub    ax, ax
        out    dx, ax
        lea    edx, [edi + 8]
        in     ax, dx
        cmp    ah, 8
        lea    edx, [edi + 0eh]
        jae    SHORT loc_5_1DB0
        mov    ax, word ptr [ebp - 4]
        out    dx, ax
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_1DB0:
        mov    ax, 2
        inc    dword ptr [esi + 0a4h]
        out    dx, ax
        mov    byte ptr [esi + 43h], 1
        mov    byte ptr [esi + 40h], 1
        mov    ecx, dword ptr [esi + 14h]
        mov    dword ptr [ebp - 0ch], ecx
        lea    eax, [ebp - 10h]
        push   eax
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ecx + 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1DDD:
        mov    ecx, dword ptr [ebp - 10h]
        add    ecx, 6
        shr    ecx, 8
        or     cx, 20h
        mov    dx, di
        mov    ax, cx
        out    dx, ax
        mov    ebx, 0ah
        lea    edx, [edi + 0ch]
        mov    ecx, 8
        nop
loc_5_1E00:
        in     ax, dx
        test   cx, ax
        jne    SHORT loc_5_1E0A
        dec    ebx
        jne    SHORT loc_5_1E00
loc_5_1E0A:
        or     ebx, ebx
        je     SHORT loc_5_1E3A
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [eax], ecx
        lea    edx, [edi + 3]
        in     al, dx
        mov    byte ptr [ecx + 0ch], al
        mov    eax, dword ptr [esi + 14h]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 14h], ecx
        cmp    dword ptr [eax], 0
        jne    SHORT loc_5_1E30
        lea    eax, [esi + 14h]
        mov    dword ptr [esi + 18h], eax
loc_5_1E30:
        mov    byte ptr [esi + 43h], 0
        mov    byte ptr [esi + 40h], 0
        jmp    SHORT loc_5_1E46
loc_5_1E3A:
        cmp    byte ptr [esi + 42h], 0
        jne    SHORT loc_5_1E46
        push   esi
        call   sub_5_2B70
loc_5_1E46:
        lea    edx, [edi + 0eh]
        mov    ax, word ptr [ebp - 4]
        out    dx, ax
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_1E5A:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1E70:
        push   ebp
        mov    ebp, esp
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], 1
        cmp    byte ptr [ecx + 41h], 0
        jne    NEAR PTR loc_5_2020
        mov    esi, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 41h], 1
        mov    dword ptr [esi], 0
        mov    eax, dword ptr [ecx + 28h]
        mov    dword ptr [ecx + 28h], esi
        mov    dword ptr [eax], esi
        mov    eax, dword ptr [ecx + 78h]
        mov    dword ptr [ebp - 24h], eax
        lea    edi, [eax + 8]
        add    ax, 0eh
        mov    word ptr [ebp - 30h], ax
        mov    dx, ax
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    dx, word ptr [ebp - 30h]
        mov    ax, 2
        out    dx, ax
        mov    cx, word ptr [ebp - 24h]
        add    cx, 6
        mov    word ptr [ebp - 28h], cx
        mov    dx, cx
        in     ax, dx
        mov    word ptr [ebp - 6], ax
        mov    cx, word ptr [ebp - 24h]
        add    cx, 2
        mov    word ptr [ebp - 2ch], cx
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [ebp + 8]
        add    edx, 1ch
        mov    dword ptr [ebp - 1ch], edx
        sub    ebx, ebx
loc_5_1EF6:
        mov    dx, word ptr [ebp - 2ch]
        mov    al, byte ptr [esi + 0ch]
        out    dx, al
        mov    dx, word ptr [ebp - 28h]
        mov    ax, 4000h
        out    dx, ax
        lea    eax, [ebp - 20h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        push   ebx
        push   ebx
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1F1B:
        mov    dx, di
        mov    ax, bx
        out    dx, ax
        mov    eax, dword ptr [ebp - 20h]
        add    ax, 6
        cmp    dword ptr [ebp - 14h], ebx
        out    dx, ax
        je     SHORT loc_5_1F9C
loc_5_1F31:
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1F42:
        cmp    dword ptr [ebp - 0ch], 7
        jbe    SHORT loc_5_1F6D
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 7ffch
        shr    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1F5E:
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 7ffch
        add    dword ptr [ebp - 10h], eax
        and    dword ptr [ebp - 0ch], 3
loc_5_1F6D:
        cmp    dword ptr [ebp - 0ch], ebx
        je     SHORT loc_5_1F85
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 7fffh
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1F85:
        lea    eax, [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1F92:
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 14h], eax
        cmp    eax, ebx
        jne    SHORT loc_5_1F31
loc_5_1F9C:
        test   byte ptr [ebp - 20h], 1
        mov    dx, di
        je     SHORT loc_5_1FAA
        mov    al, 30h
        out    dx, al
        jmp    SHORT loc_5_1FB0
loc_5_1FAA:
        mov    ax, 10h
        out    dx, ax
loc_5_1FB0:
        mov    dx, word ptr [ebp - 24h]
        mov    ax, 0c0h
        out    dx, ax
        mov    edx, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [edx]
        cmp    eax, ebx
        jne    SHORT loc_5_1FD0
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], bl
        mov    byte ptr [ecx + 41h], bl
        jmp    SHORT loc_5_1FF0
        db  08Bh, 0FFh
loc_5_1FD0:
        mov    esi, eax
        mov    edx, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [esi]
        mov    dword ptr [edx], eax
        cmp    dword ptr [esi], ebx
        jne    SHORT loc_5_1FE3
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 20h], edx
loc_5_1FE3:
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [esi], ebx
        mov    eax, dword ptr [ecx + 28h]
        mov    dword ptr [ecx + 28h], esi
        mov    dword ptr [eax], esi
loc_5_1FF0:
        cmp    byte ptr [ebp - 1], bl
        jne    NEAR PTR loc_5_1EF6
        mov    dx, word ptr [ebp - 2ch]
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        mov    dx, word ptr [ebp - 28h]
        mov    ax, word ptr [ebp - 6]
        out    dx, ax
        mov    dx, word ptr [ebp - 30h]
        mov    ax, word ptr [ebp - 4]
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_5_2020:
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [esi], 0
        mov    eax, dword ptr [ecx + 20h]
        mov    dword ptr [ecx + 20h], esi
        mov    dword ptr [eax], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_5_2040:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, 0ffffffffh
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [ebp - 4], 6
loc_5_2057:
        mov    bl, byte ptr [esi]
        inc    esi
        mov    cx, 8
        mov    edi, edi
loc_5_2060:
        mov    edx, eax
        and    edx, 80000000h
        cmp    edx, 1
        sbb    edi, edi
        inc    edi
        mov    dl, bl
        and    dx, 1
        xor    di, dx
        add    eax, eax
        shr    bl, 1
        or     di, di
        je     SHORT loc_5_208F
        xor    eax, 4c11db6h
        mov    edx, edi
        and    edx, 0ffffh
        or     eax, edx
loc_5_208F:
        dec    cx
        jne    SHORT loc_5_2060
        dec    word ptr [ebp - 4]
        jne    SHORT loc_5_2057
        shr    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_20B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        sub    dl, dl
        mov    cx, word ptr [ebp + 0ch]
        mov    bx, cx
        shr    bx, 3
        mov    edi, dword ptr [eax + 78h]
        mov    esi, ebx
        and    esi, 0ffffh
        lea    esi, [eax + esi + 97h]
        mov    al, 1
        and    cl, 7
        shl    al, cl
        cmp    byte ptr [ebp + 10h], dl
        mov    cl, byte ptr [esi]
        je     SHORT loc_5_20F3
        test   cl, al
        jne    SHORT loc_5_20EF
        mov    dl, 1
loc_5_20EF:
        or     al, cl
        jmp    SHORT loc_5_20FD
loc_5_20F3:
        test   cl, al
        je     SHORT loc_5_20F9
        mov    dl, 1
loc_5_20F9:
        not    al
        and    al, cl
loc_5_20FD:
        mov    byte ptr [esi], al
        or     dl, dl
        je     SHORT loc_5_2124
        lea    ecx, [edi + 0eh]
        mov    dx, cx
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        mov    ax, 3
        out    dx, ax
        lea    edx, [edi + ebx]
        mov    al, byte ptr [esi]
        out    dx, al
        mov    dx, cx
        mov    ax, word ptr [ebp - 2]
        out    dx, ax
loc_5_2124:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h
sub_5_2130:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        mov    esi, dword ptr [ebp + 8]
        sub    edi, edi
        mov    eax, dword ptr [esi + 10h]
        cmp    eax, edi
        je     SHORT loc_5_2170
        cmp    eax, 1
        je     NEAR PTR loc_5_23C4
        cmp    eax, 2
        je     NEAR PTR loc_5_2510
        mov    dword ptr [ebp - 0ch], 0c0000001h
        jmp    NEAR PTR loc_5_2A5B
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_2170:
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 1010101h
        ja     SHORT loc_5_2193
        je     NEAR PTR loc_5_2340
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_5_21B4
        shl    eax, 2
        db  0FFh, 0A0h, 0E8h, 010h, 000h, 000h
loc_5_2193:
        cmp    eax, 1010102h
        je     NEAR PTR loc_5_2353
        cmp    eax, 1010103h
        je     NEAR PTR loc_5_2366
        cmp    eax, 1010104h
        je     NEAR PTR loc_5_23B0
loc_5_21B4:
        sub    edi, edi
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_5_2A5B
loc_5_21C2:
        db  0B9h, 0D0h, 000h, 000h, 000h
        mov    edi, 5ch
        jmp    NEAR PTR loc_5_2A5E
loc_5_21D1:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ax, word ptr [ecx + 6ch]
        sub    ax, 2
        cmp    ax, 1
        lea    ecx, [ebp - 8]
        sbb    eax, eax
        and    eax, 2
        mov    dword ptr [ebp - 8], eax
        mov    edi, 4
        jmp    NEAR PTR loc_5_2A5E
loc_5_21F5:
        db  0B9h, 02Ch, 001h, 000h, 000h
        mov    edi, 8
        jmp    NEAR PTR loc_5_2A5E
loc_5_2204:
        mov    eax, dword ptr [ebp + 10h]
        lea    ecx, [ebp - 8]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 8], eax
        mov    edi, 4
        jmp    NEAR PTR loc_5_2A5E
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_2220:
        mov    dword ptr [ebp - 8], 5dch
        mov    edi, 4
        lea    ecx, [ebp - 8]
        jmp    NEAR PTR loc_5_2A5E
loc_5_2234:
        mov    dword ptr [ebp - 8], 186a0h
        mov    edi, 4
        lea    ecx, [ebp - 8]
        jmp    NEAR PTR loc_5_2A5E
loc_5_2248:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, 4
        mov    eax, dword ptr [ecx + 83h]
        shr    eax, 1
        mov    dword ptr [ebp - 8], eax
        lea    ecx, [ebp - 8]
        jmp    NEAR PTR loc_5_2A5E
loc_5_2263:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, 4
        mov    eax, dword ptr [ecx + 87h]
        mov    dword ptr [ebp - 8], eax
        lea    ecx, [ebp - 8]
        jmp    NEAR PTR loc_5_2A5E
        db  08Dh, 064h, 024h, 000h
loc_5_2280:
        push   3
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 8bh
        push   ecx
        lea    eax, [ebp - 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2295:
        mov    byte ptr [ebp - 11h], 0
        lea    ecx, [ebp - 14h]
        mov    edi, 4
        jmp    NEAR PTR loc_5_2A5E
loc_5_22A6:
        db  0B9h, 034h, 001h, 000h, 000h
        mov    edi, 2ah
        jmp    NEAR PTR loc_5_2A5E
loc_5_22B5:
        mov    eax, dword ptr [ebp + 10h]
        lea    ecx, [ebp - 8]
        mov    eax, dword ptr [eax + 1ch]
        mov    edi, 4
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_5_2A5E
        db  08Bh, 0FFh
loc_5_22D0:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, 4
        mov    eax, dword ptr [ecx + 7dh]
        mov    dword ptr [ebp - 8], eax
        lea    ecx, [ebp - 8]
        jmp    NEAR PTR loc_5_2A5E
loc_5_22E6:
        mov    word ptr [ebp - 2], 30ah
        lea    ecx, [ebp - 2]
        mov    edi, 2
        jmp    NEAR PTR loc_5_2A5E
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_2300:
        mov    dword ptr [ebp - 8], 5eah
        mov    edi, 4
        lea    ecx, [ebp - 8]
        jmp    NEAR PTR loc_5_2A5E
loc_5_2314:
        mov    dword ptr [ebp - 8], 0
        mov    edi, 4
        lea    ecx, [ebp - 8]
        jmp    NEAR PTR loc_5_2A5E
loc_5_2328:
        mov    dword ptr [ebp - 8], 7
        mov    edi, 4
        lea    ecx, [ebp - 8]
        jmp    NEAR PTR loc_5_2A5E
        db  08Dh, 064h, 024h, 000h
loc_5_2340:
        mov    edi, 6
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 8bh
        jmp    NEAR PTR loc_5_2A5E
loc_5_2353:
        mov    edi, 6
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 91h
        jmp    NEAR PTR loc_5_2A5E
loc_5_2366:
        mov    eax, dword ptr [esi + 18h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [esi + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 1ch]
        push   eax
        mov    eax, dword ptr [ecx + 72h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2389:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_5_23A1
        mov    eax, dword ptr [ebp - 8]
        sub    edi, edi
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [esi + 20h], eax
        jmp    NEAR PTR loc_5_2A5B
loc_5_23A1:
        mov    dword ptr [esi + 20h], 0
        sub    edi, edi
        jmp    NEAR PTR loc_5_2A5B
        db  090h
loc_5_23B0:
        mov    dword ptr [ebp - 8], 10h
        lea    ecx, [ebp - 8]
        mov    edi, 4
        jmp    NEAR PTR loc_5_2A5E
loc_5_23C4:
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 1010eh
        je     SHORT loc_5_23F7
        cmp    eax, 1010fh
        je     SHORT loc_5_2449
        cmp    eax, 10112h
        je     NEAR PTR loc_5_2480
        cmp    eax, 1010103h
        je     NEAR PTR loc_5_24B0
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_5_2A5B
loc_5_23F7:
        cmp    dword ptr [esi + 1ch], 4
        jae    SHORT loc_5_2410
        mov    dword ptr [esi + 24h], 4
        mov    dword ptr [ebp - 0ch], 0c0010014h
        jmp    NEAR PTR loc_5_2A5B
loc_5_2410:
        push   4
        mov    eax, dword ptr [esi + 18h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_241F:
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   0
        mov    eax, dword ptr [eax + 1ch]
        mov    ecx, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ecx + 72h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_243A:
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [esi + 20h], 4
        jmp    NEAR PTR loc_5_2A5B
loc_5_2449:
        push   4
        mov    eax, dword ptr [esi + 18h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2458:
        cmp    dword ptr [ebp - 8], 5eah
        jbe    SHORT loc_5_2468
        mov    dword ptr [ebp - 0ch], 0c000009ah
loc_5_2468:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ecx + 7dh], eax
        jae    NEAR PTR loc_5_2A5B
        mov    dword ptr [ecx + 7dh], eax
        jmp    NEAR PTR loc_5_2A5B
        db  090h
loc_5_2480:
        cmp    dword ptr [esi + 1ch], 4
        jae    SHORT loc_5_2499
        mov    dword ptr [esi + 24h], 4
        mov    dword ptr [ebp - 0ch], 0c0010014h
        jmp    NEAR PTR loc_5_2A5B
loc_5_2499:
        push   4
        mov    eax, dword ptr [esi + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 10h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_24AB:
        jmp    NEAR PTR loc_5_2A5B
loc_5_24B0:
        mov    ecx, dword ptr [esi + 1ch]
        mov    ebx, 6
        mov    eax, ecx
        sub    edx, edx
        div    ebx
        or     edx, edx
        je     SHORT loc_5_24D4
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [esi + 20h], eax
        jmp    NEAR PTR loc_5_2A5B
loc_5_24D4:
        push   1
        mov    eax, dword ptr [esi + 18h]
        push   eax
        mov    ebx, 6
        mov    eax, ecx
        sub    edx, edx
        div    ebx
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   0
        mov    eax, dword ptr [eax + 1ch]
        mov    ecx, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ecx + 72h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_24FB:
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [esi + 20h], eax
        jmp    NEAR PTR loc_5_2A5B
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_2510:
        mov    edi, 4
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 20101h
        ja     SHORT loc_5_2538
        je     NEAR PTR loc_5_2810
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_5_258F
        shl    eax, 2
        db  0FFh, 0A0h, 034h, 011h, 000h, 000h
loc_5_2538:
        cmp    eax, 2020dh
        ja     NEAR PTR loc_5_25D6
        je     NEAR PTR loc_5_2920
        cmp    eax, 20205h
        ja     SHORT loc_5_25C3
        je     NEAR PTR loc_5_28A0
        cmp    eax, 20201h
        ja     SHORT loc_5_25A0
        je     NEAR PTR loc_5_2860
        cmp    eax, 20102h
        je     NEAR PTR loc_5_2820
        cmp    eax, 20103h
        je     NEAR PTR loc_5_2830
        cmp    eax, 20104h
        je     NEAR PTR loc_5_2840
        cmp    eax, 20105h
        je     NEAR PTR loc_5_2850
loc_5_258F:
        sub    edi, edi
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_5_2A5B
        db  08Dh, 049h, 000h
loc_5_25A0:
        cmp    eax, 20202h
        je     NEAR PTR loc_5_2870
        cmp    eax, 20203h
        je     NEAR PTR loc_5_2880
        cmp    eax, 20204h
        je     NEAR PTR loc_5_2890
        jmp    SHORT loc_5_258F
loc_5_25C3:
        sub    eax, 20206h
        cmp    eax, 6
        ja     SHORT loc_5_258F
        shl    eax, 2
        db  0FFh, 0A0h, 080h, 011h, 000h, 000h
loc_5_25D6:
        cmp    eax, 1020103h
        ja     SHORT loc_5_2640
        je     NEAR PTR loc_5_29E0
        cmp    eax, 1010103h
        ja     SHORT loc_5_2616
        je     NEAR PTR loc_5_2966
        cmp    eax, 2020eh
        je     NEAR PTR loc_5_2930
        cmp    eax, 1010101h
        je     NEAR PTR loc_5_2940
        cmp    eax, 1010102h
        je     NEAR PTR loc_5_2953
        jmp    NEAR PTR loc_5_258F
loc_5_2616:
        cmp    eax, 1010104h
        je     NEAR PTR loc_5_29B0
        cmp    eax, 1020101h
        je     NEAR PTR loc_5_29C4
        cmp    eax, 1020102h
        je     NEAR PTR loc_5_29D2
        jmp    NEAR PTR loc_5_258F
        db  08Dh, 064h, 024h, 000h
loc_5_2640:
        sub    eax, 1020201h
        cmp    eax, 6
        ja     NEAR PTR loc_5_258F
        db  0FFh, 024h, 085h, 09Ch, 011h, 000h, 000h
loc_5_2655:
        db  0B9h, 000h, 000h, 000h, 000h
        mov    edi, 0d0h
        jmp    NEAR PTR loc_5_2A5E
loc_5_2664:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ax, word ptr [ecx + 6ch]
        sub    ax, 2
        cmp    ax, 1
        lea    ecx, [ebp - 8]
        sbb    eax, eax
        and    eax, 2
        mov    dword ptr [ebp - 8], eax
        mov    edi, 4
        jmp    NEAR PTR loc_5_2A5E
loc_5_2688:
        db  0B9h, 02Ch, 001h, 000h, 000h
        mov    edi, 8
        jmp    NEAR PTR loc_5_2A5E
loc_5_2697:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    al, byte ptr [ecx + 51h]
        or     al, al
        je     SHORT loc_5_26D0
        cmp    byte ptr [ecx + 52h], 0
        je     SHORT loc_5_26B6
        db  0B9h, 02Ch, 001h, 000h, 000h
        mov    edi, 8
        jmp    NEAR PTR loc_5_2A5E
loc_5_26B6:
        or     al, al
        je     SHORT loc_5_26D0
        mov    dword ptr [ebp - 8], 5
        lea    ecx, [ebp - 8]
        mov    edi, 4
        jmp    NEAR PTR loc_5_2A5E
        db  08Bh, 0FFh
loc_5_26D0:
        cmp    byte ptr [ecx + 52h], 0
        je     NEAR PTR loc_5_2A5B
        mov    dword ptr [ebp - 8], 0
        lea    ecx, [ebp - 8]
        mov    edi, 4
        jmp    NEAR PTR loc_5_2A5E
        db  08Bh, 0FFh
loc_5_26F0:
        mov    dword ptr [ebp - 8], 5dch
        mov    edi, 4
        lea    ecx, [ebp - 8]
        jmp    NEAR PTR loc_5_2A5E
loc_5_2704:
        mov    dword ptr [ebp - 8], 186a0h
        mov    edi, 4
        lea    ecx, [ebp - 8]
        jmp    NEAR PTR loc_5_2A5E
loc_5_2718:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, 4
        mov    eax, dword ptr [ecx + 83h]
        shr    eax, 1
        mov    dword ptr [ebp - 8], eax
        lea    ecx, [ebp - 8]
        jmp    NEAR PTR loc_5_2A5E
loc_5_2733:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, 4
        mov    eax, dword ptr [ecx + 87h]
        mov    dword ptr [ebp - 8], eax
        lea    ecx, [ebp - 8]
        jmp    NEAR PTR loc_5_2A5E
        db  08Dh, 064h, 024h, 000h
loc_5_2750:
        push   3
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 8bh
        push   ecx
        lea    eax, [ebp - 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2765:
        mov    byte ptr [ebp - 11h], 0
        lea    ecx, [ebp - 14h]
        mov    edi, 4
        jmp    NEAR PTR loc_5_2A5E
loc_5_2776:
        db  0B9h, 034h, 001h, 000h, 000h
        mov    edi, 2ah
        jmp    NEAR PTR loc_5_2A5E
loc_5_2785:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, 4
        mov    eax, dword ptr [ecx + 72h]
        lea    ecx, [ebp - 8]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_5_2A5E
        db  08Bh, 0FFh
loc_5_27A0:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, 4
        mov    eax, dword ptr [ecx + 7dh]
        mov    dword ptr [ebp - 8], eax
        lea    ecx, [ebp - 8]
        jmp    NEAR PTR loc_5_2A5E
loc_5_27B6:
        mov    word ptr [ebp - 2], 300h
        lea    ecx, [ebp - 2]
        mov    edi, 2
        jmp    NEAR PTR loc_5_2A5E
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_27D0:
        mov    dword ptr [ebp - 8], 5eah
        mov    edi, 4
        lea    ecx, [ebp - 8]
        jmp    NEAR PTR loc_5_2A5E
loc_5_27E4:
        mov    dword ptr [ebp - 8], 0
        mov    edi, 4
        lea    ecx, [ebp - 8]
        jmp    NEAR PTR loc_5_2A5E
loc_5_27F8:
        mov    dword ptr [ebp - 8], 7
        mov    edi, 4
        lea    ecx, [ebp - 8]
        jmp    NEAR PTR loc_5_2A5E
        db  08Dh, 064h, 024h, 000h
loc_5_2810:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0ach
        jmp    NEAR PTR loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2820:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0b0h
        jmp    NEAR PTR loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2830:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0b4h
        jmp    NEAR PTR loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2840:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0b8h
        jmp    NEAR PTR loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2850:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0bch
        jmp    NEAR PTR loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2860:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0c0h
        jmp    NEAR PTR loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2870:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0c4h
        jmp    NEAR PTR loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2880:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0c8h
        jmp    NEAR PTR loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2890:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0cch
        jmp    NEAR PTR loc_5_2A5E
        db  08Bh, 0FFh
loc_5_28A0:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0d0h
        jmp    NEAR PTR loc_5_2A5E
        db  08Bh, 0FFh
loc_5_28B0:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0d4h
        jmp    NEAR PTR loc_5_2A5E
        db  08Bh, 0FFh
loc_5_28C0:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0d8h
        jmp    NEAR PTR loc_5_2A5E
        db  08Bh, 0FFh
loc_5_28D0:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0dch
        jmp    NEAR PTR loc_5_2A5E
        db  08Bh, 0FFh
loc_5_28E0:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0e0h
        jmp    NEAR PTR loc_5_2A5E
        db  08Bh, 0FFh
loc_5_28F0:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0e4h
        jmp    NEAR PTR loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2900:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0e8h
        jmp    NEAR PTR loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2910:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0ech
        jmp    NEAR PTR loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2920:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0f0h
        jmp    NEAR PTR loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2930:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0a8h
        jmp    NEAR PTR loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2940:
        mov    edi, 6
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 8bh
        jmp    NEAR PTR loc_5_2A5E
loc_5_2953:
        mov    edi, 6
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 91h
        jmp    NEAR PTR loc_5_2A5E
loc_5_2966:
        mov    eax, dword ptr [esi + 18h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ecx + 72h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2982:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_5_29A0
        mov    eax, dword ptr [ebp - 8]
        sub    edi, edi
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [esi + 20h], eax
        jmp    NEAR PTR loc_5_2A5B
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_29A0:
        mov    dword ptr [esi + 20h], 0
        sub    edi, edi
        jmp    NEAR PTR loc_5_2A5B
        db  08Bh, 0FFh
loc_5_29B0:
        mov    dword ptr [ebp - 8], 10h
        lea    ecx, [ebp - 8]
        mov    edi, 4
        jmp    NEAR PTR loc_5_2A5E
loc_5_29C4:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0f4h
        jmp    NEAR PTR loc_5_2A5E
loc_5_29D2:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0f8h
        jmp    NEAR PTR loc_5_2A5E
loc_5_29E0:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0fch
        jmp    NEAR PTR loc_5_2A5E
        db  08Bh, 0FFh
loc_5_29F0:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 108h
        jmp    NEAR PTR loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2A00:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 100h
        jmp    NEAR PTR loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2A10:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 0bch
        jmp    NEAR PTR loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2A20:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 10ch
        jmp    NEAR PTR loc_5_2A5E
        db  08Bh, 0FFh
loc_5_2A30:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 110h
        jmp    SHORT loc_5_2A5E
        db  005h, 000h, 000h, 000h, 000h
loc_5_2A40:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 114h
        jmp    SHORT loc_5_2A5E
        db  005h, 000h, 000h, 000h, 000h
loc_5_2A50:
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 104h
        jmp    SHORT loc_5_2A5E
loc_5_2A5B:
        mov    ecx, dword ptr [ebp - 10h]
loc_5_2A5E:
        or     edi, edi
        je     SHORT loc_5_2A89
        cmp    dword ptr [esi + 1ch], edi
        jae    SHORT loc_5_2A74
        mov    dword ptr [esi + 24h], edi
        mov    edi, dword ptr [esi + 1ch]
        mov    dword ptr [ebp - 0ch], 0c0010014h
loc_5_2A74:
        mov    dword ptr [esi + 20h], edi
        mov    eax, dword ptr [esi + 18h]
        or     edi, edi
        je     SHORT loc_5_2A89
        mov    edi, edi
loc_5_2A80:
        mov    dl, byte ptr [ecx]
        mov    byte ptr [eax], dl
        inc    ecx
        inc    eax
        dec    edi
        jne    SHORT loc_5_2A80
loc_5_2A89:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h
loc_5_2A98:
        adc    cl, byte ptr [eax]
        add    byte ptr [eax], al
        and    dword ptr [eax], ecx
        add    byte ptr [eax], al
        inc    ebp
        or     byte ptr [eax], al
        db  000h, 054h, 008h, 000h
        db  000h, 070h, 008h
        add    byte ptr [eax], al
        db  070h, 008h
        add    byte ptr [eax], al
        test   byte ptr [eax], cl
        add    byte ptr [eax], al
        cwde
        or     byte ptr [eax], al
        db  000h, 098h, 008h, 000h, 000h, 0B3h
        or     byte ptr [eax], al
        db  000h, 0B3h, 008h, 000h, 000h, 0D0h
        or     byte ptr [eax], al
        db  000h, 0F6h
        or     byte ptr [eax], al
        db  000h, 005h, 009h, 000h, 000h, 020h
        or     dword ptr [eax], eax
        db  000h, 036h
        or     dword ptr [eax], eax
        db  000h, 050h, 009h
        add    byte ptr [eax], al
        or     dword ptr fs:[eax], eax
        db  000h, 078h, 009h
        add    byte ptr [eax], al
loc_5_2AE4:
        movsd
        or     al, 0
        db  000h, 0B4h, 00Ch, 000h, 000h, 0D8h, 00Ch
        add    byte ptr [eax], al
        out    0ch, eax
        add    byte ptr [eax], al
        inc    eax
        db  00Dh, 000h, 000h, 040h, 00Dh
        add    byte ptr [eax], al
        push   esp
        db  00Dh, 000h, 000h, 068h, 00Dh
        add    byte ptr [eax], al
        db  068h, 00Dh, 000h, 000h, 083h
        db  00Dh, 000h, 000h, 083h, 00Dh
        add    byte ptr [eax], al
        db  0A0h, 00Dh, 000h, 000h, 0C6h
        db  00Dh, 000h, 000h, 0D5h, 00Dh
        add    byte ptr [eax], al
        db  0F0h, 00Dh, 000h, 000h, 006h, 00Eh, 000h, 000h, 020h, 00Eh, 000h, 000h, 034h, 00Eh, 000h, 000h
        db  048h, 00Eh, 000h, 000h
loc_5_2B30:
        add    byte ptr [edi], cl
        add    byte ptr [eax], al
        adc    byte ptr [edi], cl
        add    byte ptr [eax], al
        and    byte ptr [edi], cl
        add    byte ptr [eax], al
        xor    byte ptr [edi], cl
        add    byte ptr [eax], al
        inc    eax
        sldt   word ptr [eax]
        push   eax
        sldt   word ptr [eax]
        pushad
        sldt   word ptr [eax]
loc_5_2B4C:
        inc    eax
        adc    byte ptr [eax], al
        db  000h, 050h, 010h
        add    byte ptr [eax], al
        pushad
        adc    byte ptr [eax], al
        db  000h, 070h, 010h
        add    byte ptr [eax], al
        adc    byte ptr [eax], 0
        db  000h, 090h, 010h, 000h, 000h, 0A0h
        adc    byte ptr [eax], al
        add    byte ptr [ebp - 72ffdb9ch], cl
        and    al, 0
sub_5_2B70:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 78h]
        add    dx, 0eh
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    edx, dword ptr [esi + 78h]
        add    dx, 0dh
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    edx, dword ptr [esi + 78h]
        or     byte ptr [ebp - 1], 8
        add    dx, 0dh
        sub    al, al
        mov    cx, word ptr [esi + 78h]
        out    dx, al
        lea    edx, [ecx + 0dh]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        lea    edx, [ecx + 0eh]
        mov    ax, word ptr [ebp - 4]
        out    dx, ax
        mov    byte ptr [esi + 43h], 0
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_2BD0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 42h], 0
        jne    SHORT loc_5_2BF2
        cmp    byte ptr [esi + 46h], 0
        jne    SHORT loc_5_2BF2
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 54h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2BF2:
        mov    byte ptr [esi + 46h], 0
        push   1388h
        lea    eax, [esi + 34h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2C04:
        mov    byte ptr [esi + 46h], 0
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 049h, 000h
loc_5_2C10:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        sub    al, al
        mov    ebx, dword ptr [edi + 78h]
        lea    esi, [ebx - 1]
        mov    dx, si
        out    dx, al
        lea    ecx, [ebx + 0eh]
        mov    dx, cx
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    ax, 2
        out    dx, ax
        lea    edx, [ebx + 0dh]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        sub    al, al
        out    dx, al
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    dx, cx
        mov    ax, word ptr [ebp - 4]
        out    dx, ax
        mov    dx, si
        mov    al, byte ptr [edi + 53h]
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_2C70:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    al, 3
        mov    edx, dword ptr [esi + 78h]
        add    dx, 0dh
        out    dx, al
        mov    edi, 927ch
loc_5_2C87:
        push   28h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2C8E:
        dec    edi
        jne    SHORT loc_5_2C87
        push   esi
        call   sub_5_2CD0
        add    esp, 4
        cmp    eax, 1
        je     SHORT loc_5_2CAA
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_5_2CAA:
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_5_2E60
        or     eax, eax
        jne    SHORT loc_5_2CC1
        push   esi
        call   sub_5_33C0
        sub    eax, eax
loc_5_2CC1:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_2CD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [ebp - 6], 0f8b7h
        mov    esi, dword ptr [eax + 78h]
        mov    al, byte ptr [eax + 7ch]
        and    eax, 0ffh
        cmp    eax, 1
        jne    SHORT loc_5_2CF8
        mov    word ptr [ebp - 6], 0f9b7h
loc_5_2CF8:
        sub    edi, edi
        lea    eax, [esi + 0eh]
        mov    word ptr [ebp - 10h], ax
        lea    eax, [esi + 1eh]
        mov    word ptr [ebp - 0ch], ax
        lea    eax, [esi + 1fh]
        mov    word ptr [ebp - 18h], ax
        lea    eax, [esi + 10h]
        mov    word ptr [ebp - 14h], ax
        add    si, 11h
loc_5_2D1A:
        mov    dx, word ptr [ebp - 10h]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     byte ptr [ebp - 1], 2
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        push   0ah
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2D31:
        and    byte ptr [ebp - 1], 0fdh
        mov    dx, word ptr [ebp - 10h]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    ebx, 927ch
loc_5_2D42:
        push   28h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2D49:
        dec    ebx
        jne    SHORT loc_5_2D42
        mov    dx, word ptr [ebp - 0ch]
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    dx, word ptr [ebp - 18h]
        in     al, dx
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, byte ptr [ebp - 2]
        and    eax, 0ffff00ffh
        or     cx, ax
        mov    dx, word ptr [ebp - 0ch]
        mov    al, 1
        out    dx, al
        mov    dx, word ptr [ebp - 14h]
        mov    al, byte ptr [ebp - 6]
        out    dx, al
        mov    dx, si
        mov    al, byte ptr [ebp - 5]
        out    dx, al
        mov    dx, word ptr [ebp - 0ch]
        mov    ax, cx
        out    dx, ax
        cmp    ch, 33h
        je     SHORT loc_5_2DB0
        inc    edi
        cmp    edi, 0ah
        jl     NEAR PTR loc_5_2D1A
        cmp    ch, 33h
        je     SHORT loc_5_2DB0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
loc_5_2DB0:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
sub_5_2DC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 78h]
        lea    edx, [esi - 4]
        sub    esi, 10h
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    byte ptr [ebp - 1], 0bdh
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    al, byte ptr [edi + 53h]
        mov    byte ptr [ebp - 1], al
        and    byte ptr [ebp - 1], 0fdh
        lea    edx, [esi + 0fh]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        lea    ebx, [esi + 0eh]
        mov    dx, bx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     byte ptr [ebp - 1], 2
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        push   0dh
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2E0B:
        and    byte ptr [ebp - 1], 0fdh
        mov    dx, bx
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    ebx, 3c8ch
        add    eax, 0
loc_5_2E20:
        push   28h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2E27:
        dec    ebx
        jne    SHORT loc_5_2E20
        lea    edx, [esi + 1eh]
        mov    al, 1
        out    dx, al
        lea    edx, [esi + 10h]
        mov    al, 0b7h
        out    dx, al
        lea    edx, [esi + 11h]
        mov    al, 0f8h
        out    dx, al
        lea    edx, [esi + 0dh]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [edi + 53h]
        mov    byte ptr [ebp - 1], al
        and    byte ptr [ebp - 1], 0fdh
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_5_2E60:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    esi, dword ptr [edi + 78h]
        mov    cl, byte ptr [edi + 7ch]
        lea    eax, [esi + 1eh]
        mov    dx, ax
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        lea    edx, [esi + 0ch]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     ecx, ecx
        je     SHORT loc_5_2EA9
        cmp    ecx, 1
        je     SHORT loc_5_2EB6
        cmp    ecx, 2
        je     SHORT loc_5_2ED2
loc_5_2E95:
        lea    edx, [esi + 1eh]
        mov    ax, word ptr [ebp - 4]
        out    dx, ax
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_2EA9:
        and    byte ptr [ebp - 1], 0bfh
        lea    edx, [esi + 0ch]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        jmp    SHORT loc_5_2E95
loc_5_2EB6:
        or     byte ptr [ebp - 1], 40h
        lea    edx, [esi + 0ch]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    edi, 0fah
loc_5_2EC6:
        push   28h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2ECD:
        dec    edi
        jne    SHORT loc_5_2EC6
        jmp    SHORT loc_5_2E95
loc_5_2ED2:
        lea    edx, [esi + 1eh]
        mov    ax, 1
        out    dx, ax
        mov    eax, dword ptr [ebp + 0ch]
        lea    edx, [esi + 14h]
        mov    ax, word ptr [eax]
        out    dx, ax
        mov    eax, dword ptr [ebp + 0ch]
        lea    ecx, [eax + 2]
        lea    edx, [esi + 16h]
        mov    ax, word ptr [ecx]
        out    dx, ax
        lea    edx, [esi + 18h]
        mov    ax, word ptr [ecx + 2]
        and    byte ptr [ebp - 1], 0bfh
        out    dx, ax
        lea    edx, [esi + 0ch]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    byte ptr [edi + 7ch], 0
        lea    edx, [esi + 10h]
        mov    ax, 0f8b7h
        out    dx, ax
        mov    ebx, 0fah
        lea    ebx, [ebx]
loc_5_2F20:
        push   28h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2F27:
        dec    ebx
        jne    SHORT loc_5_2F20
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   edi
        call   sub_5_2FC0
        or     eax, eax
        je     NEAR PTR loc_5_2E95
        or     byte ptr [ebp - 1], 40h
        lea    edx, [esi + 0ch]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    byte ptr [edi + 7ch], 1
        lea    edx, [esi + 10h]
        mov    ax, 0f9b7h
        out    dx, ax
        mov    ebx, 0fah
loc_5_2F59:
        push   28h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2F60:
        dec    ebx
        jne    SHORT loc_5_2F59
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   edi
        call   sub_5_2FC0
        or     eax, eax
        je     NEAR PTR loc_5_2E95
        push   0
        push   0aa00000fh
        mov    eax, dword ptr [edi + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2F85:
        add    esp, 0ch
        mov    byte ptr [edi + 7ch], 0
        lea    edx, [esi + 10h]
        add    dword ptr [edi + 78h], 10h
        mov    ax, 0f8b7h
        out    dx, ax
        push   edi
        call   sub_5_2DC0
        sub    dword ptr [edi + 78h], 10h
        mov    eax, 0c001001fh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_2FC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 114h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 78h]
        add    esi, 10h
        mov    dword ptr [ebp - 10h], 0c0000001h
        lea    eax, [esi + 0eh]
        mov    dx, ax
        in     ax, dx
        mov    word ptr [ebp - 6], ax
        lea    edx, [esi + 0eh]
        sub    ax, ax
        out    dx, ax
        mov    dx, si
        mov    ax, 0c01h
        out    dx, ax
        lea    edx, [esi + 4]
        mov    ax, 300h
        out    dx, ax
        lea    edx, [esi + 0eh]
        mov    ax, 2
        out    dx, ax
        lea    edx, [esi + 0dh]
        sub    al, al
        out    dx, al
        mov    al, 3bh
        out    dx, al
        mov    dx, si
        mov    ax, 20h
        out    dx, ax
        sub    ebx, ebx
        mov    edi, 0ffffffffh
loc_5_3024:
        lea    edx, [esi + 0ch]
        in     al, dx
        test   al, 8
        jne    SHORT loc_5_3037
        inc    ebx
        cmp    ebx, 0c8h
        jl     SHORT loc_5_3024
        jmp    SHORT loc_5_3039
loc_5_3037:
        sub    edi, edi
loc_5_3039:
        or     edi, edi
        je     SHORT loc_5_3049
        mov    dword ptr [ebp - 10h], 0c0000001h
        jmp    NEAR PTR loc_5_31EA
loc_5_3049:
        lea    edx, [esi + 3]
        in     al, dx
        mov    byte ptr [ebp - 2], al
        test   al, 80h
        je     SHORT loc_5_3060
        mov    dword ptr [ebp - 10h], 0c0000001h
        jmp    NEAR PTR loc_5_31EA
loc_5_3060:
        lea    eax, [esi + 2]
        mov    dx, ax
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        lea    edx, [esi + 6]
        mov    ax, 4000h
        out    dx, ax
        lea    edi, [esi + 8]
        mov    dx, di
        sub    ax, ax
        out    dx, ax
        mov    ax, 116h
        out    dx, ax
        sub    ebx, ebx
        mov    ecx, dword ptr [ebp + 0ch]
loc_5_3089:
        mov    dx, di
        mov    al, byte ptr [ecx + ebx]
        out    dx, al
        inc    ebx
        cmp    ebx, 6
        jl     SHORT loc_5_3089
        sub    ebx, ebx
loc_5_3098:
        mov    dx, di
        mov    al, byte ptr [ecx + ebx]
        out    dx, al
        inc    ebx
        cmp    ebx, 6
        jl     SHORT loc_5_3098
        mov    ax, 0ff16h
        out    dx, ax
        sub    ax, ax
        out    dx, ax
        lea    ecx, [ebp - 114h]
loc_5_30B6:
        mov    dx, di
        mov    ax, word ptr [ecx]
        out    dx, ax
        add    ecx, 2
        lea    eax, [ebp - 14h]
        cmp    ecx, eax
        jb     SHORT loc_5_30B6
        sub    ax, ax
        out    dx, ax
        mov    dx, si
        mov    ax, 0c0h
        out    dx, ax
        sub    ebx, ebx
        mov    dword ptr [ebp - 14h], 0ffffffffh
        nop
loc_5_30E0:
        lea    edx, [esi + 0ch]
        in     al, dx
        test   al, 20h
        jne    SHORT loc_5_3100
        test   al, 2
        jne    SHORT loc_5_3110
        push   0ah
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_30F3:
        inc    ebx
        cmp    ebx, 3a98h
        jl     SHORT loc_5_30E0
        jmp    SHORT loc_5_3117
        db  08Bh, 0FFh
loc_5_3100:
        mov    dword ptr [ebp - 14h], 1
        jmp    SHORT loc_5_3117
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_3110:
        mov    dword ptr [ebp - 14h], 0
loc_5_3117:
        cmp    dword ptr [ebp - 14h], 0
        je     SHORT loc_5_312B
        mov    dword ptr [ebp - 10h], 0c0000001h
        mov    dword ptr [ebp - 14h], 0ffffffffh
loc_5_312B:
        lea    eax, [esi + 4]
        mov    dx, ax
        in     al, dx
        mov    byte ptr [ebp - 2], al
        lea    eax, [esi + 2]
        mov    dx, ax
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        lea    edx, [esi + 6]
        mov    ax, 6000h
        out    dx, ax
        mov    dx, di
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        in     ax, dx
        mov    dx, si
        mov    ax, 0a0h
        out    dx, ax
        lea    edx, [esi + 0ch]
        mov    al, 2
        out    dx, al
        mov    ecx, 1
loc_5_3167:
        mov    dx, si
        in     ax, dx
        test   cx, ax
        jne    SHORT loc_5_3167
        cmp    dword ptr [ebp - 14h], 0
        jne    SHORT loc_5_31EA
        test   word ptr [ebp - 8], 8e10h
        je     SHORT loc_5_3188
        mov    dword ptr [ebp - 10h], 0c0000001h
        jmp    SHORT loc_5_31EA
loc_5_3188:
        sub    edi, edi
        mov    ebx, 0ffffffffh
        nop
loc_5_3190:
        push   0ah
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_3197:
        lea    edx, [esi + 0ch]
        in     al, dx
        test   al, 1
        jne    SHORT loc_5_31AA
        inc    edi
        cmp    edi, 30d4h
        jl     SHORT loc_5_3190
        jmp    SHORT loc_5_31AC
loc_5_31AA:
        sub    ebx, ebx
loc_5_31AC:
        or     ebx, ebx
        je     SHORT loc_5_31C0
        mov    dword ptr [ebp - 10h], 0c0000001h
        jmp    SHORT loc_5_31EA
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_31C0:
        mov    dword ptr [ebp - 10h], 0
        lea    edx, [esi + 6]
        mov    ax, 0e000h
        out    dx, ax
        mov    dx, si
        mov    ax, 80h
        out    dx, ax
        mov    ecx, 1
        mov    edi, edi
loc_5_31E0:
        mov    dx, si
        in     ax, dx
        test   cx, ax
        jne    SHORT loc_5_31E0
loc_5_31EA:
        lea    edx, [esi + 0eh]
        sub    ax, ax
        out    dx, ax
        mov    dx, si
        out    dx, ax
        lea    edx, [esi + 4]
        out    dx, ax
        lea    edx, [esi + 0eh]
        mov    ax, 2
        out    dx, ax
        lea    edx, [esi + 0dh]
        sub    al, al
        out    dx, al
        lea    edx, [esi + 0eh]
        mov    ax, word ptr [ebp - 6]
        out    dx, ax
        mov    eax, dword ptr [ebp - 10h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_5_3220:
        push   ebp
        mov    ebp, esp
        mov    ax, word ptr [ebp + 8]
        db  066h, 0A3h, 050h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 052h, 000h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [ebp + 0ch]
        db  0A2h, 054h, 000h, 000h, 000h
        db  0C6h, 005h, 055h, 000h, 000h, 000h, 000h
        db  068h, 050h, 000h, 000h, 000h
        push   12h
        push   0
        push   0
        push   7
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_3257:
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_5_3260:
        push   ebp
        mov    ebp, esp
        mov    ax, word ptr [ebp + 8]
        db  066h, 0A3h, 050h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 052h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 055h, 000h, 000h, 000h, 000h
        db  068h, 050h, 000h, 000h, 000h
        push   12h
        push   0
        push   0
        push   0ah
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_328F:
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_32A0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi]
        db  066h, 0A3h, 050h, 000h, 000h, 000h
        mov    ax, word ptr [esi + 2]
        db  066h, 0A3h, 052h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 4]
        db  0A2h, 054h, 000h, 000h, 000h
        mov    al, byte ptr [ebp + 0ch]
        db  0A2h, 055h, 000h, 000h, 000h
        mov    ax, word ptr [esi + 6]
        db  066h, 0A3h, 056h, 000h, 000h, 000h
        mov    eax, dword ptr [esi + 8]
        db  0A3h, 058h, 000h, 000h, 000h
        mov    eax, dword ptr [esi + 0ch]
        db  0A3h, 05Ch, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 060h, 000h, 000h, 000h, 060h, 000h
        db  068h, 050h, 000h, 000h, 000h
        push   60h
        push   0
        push   0
        push   0dh
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_32FF:
        add    esp, 14h
        db  066h, 08Bh, 00Dh, 056h, 000h, 000h, 000h
        mov    word ptr [esi + 6], cx
        db  08Bh, 00Dh, 058h, 000h, 000h, 000h
        mov    dword ptr [esi + 8], ecx
        db  08Bh, 00Dh, 05Ch, 000h, 000h, 000h
        mov    dword ptr [esi + 0ch], ecx
        pop    esi
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_3330:
        push   esi
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_3337:
        or     eax, eax
        je     SHORT loc_5_3343
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        ret
loc_5_3343:
        db  066h, 081h, 03Dh, 000h, 000h, 000h, 000h, 000h, 002h
        mov    si, 0
        ja     SHORT loc_5_3356
        mov    si, 1
loc_5_3356:
        sub    edi, edi
        db  066h, 039h, 03Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_5_337E
loc_5_3361:
        push   esi
        call   sub_5_33E0
        add    esp, 4
        cmp    eax, 1
        je     SHORT loc_5_337E
        inc    edi
        inc    si
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        cmp    eax, edi
        jg     SHORT loc_5_3361
loc_5_337E:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        cmp    eax, edi
        jne    SHORT loc_5_3392
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        ret
loc_5_3392:
        db  066h, 089h, 035h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_5_3530
        cmp    eax, 1
        je     SHORT loc_5_33B1
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        ret
loc_5_33B1:
        sub    eax, eax
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_33C0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    edx, dword ptr [eax + 78h]
        add    dx, 0fh
        mov    al, byte ptr [eax + 53h]
        out    dx, al
        sub    eax, eax
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_33E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   15h
        mov    ax, word ptr [ebp + 8]
        push   eax
        call   sub_5_3220
        or     eax, eax
        jne    NEAR PTR loc_5_3520
        push   12h
        db  068h, 050h, 000h, 000h, 000h
        lea    eax, [ebp - 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_340D:
        push   0
        lea    eax, [ebp - 18h]
        push   eax
        call   sub_5_32A0
        mov    edx, 16h
        sub    edi, edi
        mov    esi, 1
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_345A
        sub    ecx, ecx
        nop
loc_5_3430:
        db  08Ah, 082h, 050h, 000h, 000h, 000h
        cmp    al, 39h
        jbe    SHORT loc_5_343C
        and    al, 0dfh
loc_5_343C:
        db  00Fh, 0BEh, 09Fh, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        cmp    ebx, eax
        jne    SHORT loc_5_3458
        inc    edi
        inc    edx
        db  038h, 08Fh, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_3430
        jmp    SHORT loc_5_345A
loc_5_3458:
        sub    esi, esi
loc_5_345A:
        or     esi, esi
        jne    SHORT loc_5_3467
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_3467:
        sub    edi, edi
        sub    ebx, ebx
        sub    eax, eax
loc_5_346D:
        cmp    edi, 2
        jge    SHORT loc_5_34A0
        db  038h, 082h, 050h, 000h, 000h, 000h
        je     SHORT loc_5_349A
loc_5_347A:
        cmp    edi, 1
        jne    SHORT loc_5_3492
        cmp    ebx, 4dh
        jae    SHORT loc_5_3492
        db  08Ah, 08Ah, 050h, 000h, 000h, 000h
        db  088h, 08Bh, 000h, 000h, 000h, 000h
        inc    edx
        inc    ebx
loc_5_3492:
        db  038h, 082h, 050h, 000h, 000h, 000h
        jne    SHORT loc_5_347A
loc_5_349A:
        inc    edi
        inc    edx
        jmp    SHORT loc_5_346D
        db  08Bh, 0FFh
loc_5_34A0:
        db  0C6h, 083h, 000h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 010h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_3506
        db  0C7h, 045h, 0FCh, 010h, 000h, 000h, 000h
loc_5_34B7:
        mov    edi, edx
        mov    ecx, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ebx, dword ptr [ecx]
        sub    esi, esi
        cmp    byte ptr [ebx], al
        je     SHORT loc_5_34F6
loc_5_34C6:
        db  08Ah, 08Fh, 050h, 000h, 000h, 000h
        cmp    cl, 39h
        jbe    SHORT loc_5_34D4
        and    cl, 0dfh
loc_5_34D4:
        movsx  esi, byte ptr [eax + ebx]
        and    ecx, 0ffh
        cmp    esi, ecx
        mov    esi, 0
        jne    SHORT loc_5_34F6
        mov    esi, 1
        add    eax, esi
        add    edi, esi
        cmp    byte ptr [eax + ebx], 0
        jne    SHORT loc_5_34C6
loc_5_34F6:
        or     esi, esi
        jne    SHORT loc_5_3513
        add    dword ptr [ebp - 4], 4
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_5_34B7
loc_5_3506:
        or     esi, esi
        jne    SHORT loc_5_3513
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_3513:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  090h
loc_5_3520:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_3530:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   90h
        mov    si, word ptr [ebp + 8]
        push   esi
        call   sub_5_3220
        or     eax, eax
        jne    SHORT loc_5_35B0
        sub    ebx, ebx
        mov    edi, dword ptr [ebp - 4]
loc_5_3551:
        or     ebx, ebx
        jne    SHORT loc_5_35C4
        db  0B8h, 050h, 000h, 000h, 000h
        lea    ecx, [ebp - 18h]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 4], edx
        mov    edx, dword ptr [eax + 8]
        mov    dword ptr [ecx + 8], edx
        mov    edx, dword ptr [eax + 0ch]
        mov    dword ptr [ecx + 0ch], edx
        mov    ax, word ptr [eax + 10h]
        mov    word ptr [ecx + 10h], ax
        mov    eax, ecx
        push   0
        push   eax
        call   sub_5_32A0
        or     eax, eax
        jne    SHORT loc_5_35B5
        mov    edi, 14h
        db  080h, 03Dh, 064h, 000h, 000h, 000h, 004h
        jne    SHORT loc_5_35A0
        mov    ebx, 1
        jmp    SHORT loc_5_3551
        db  08Bh, 0FFh
loc_5_35A0:
        push   esi
        call   sub_5_3260
        or     eax, eax
        je     SHORT loc_5_3551
        jmp    SHORT loc_5_35B5
        db  08Dh, 064h, 024h, 000h
loc_5_35B0:
        sub    ebx, ebx
        mov    edi, dword ptr [ebp - 4]
loc_5_35B5:
        or     ebx, ebx
        jne    SHORT loc_5_35C4
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_35C4:
        add    edi, 2
        sub    dl, dl
        sub    esi, esi
        mov    eax, dword ptr [ebp + 0ch]
        mov    edi, edi
loc_5_35D0:
        db  08Ah, 08Fh, 050h, 000h, 000h, 000h
        add    dl, cl
        mov    byte ptr [esi + eax], cl
        inc    esi
        inc    edi
        cmp    esi, 6
        jl     SHORT loc_5_35D0
        db  002h, 097h, 050h, 000h, 000h, 000h
        je     SHORT loc_5_35F5
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_5_35F5:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
_PTEXT ends
        end
