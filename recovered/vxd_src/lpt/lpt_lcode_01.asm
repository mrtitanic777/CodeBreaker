; lpt_lcode_01.asm -- LPT object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  083h, 0ECh, 008h, 056h, 057h, 08Bh, 074h, 024h, 014h, 066h, 0C7h, 044h, 024h, 00Ah, 000h, 000h
        db  066h, 08Bh, 0BEh, 094h, 000h, 000h, 000h, 06Ah, 023h, 00Fh, 0B7h, 0C7h, 040h, 06Ah, 000h, 06Ah
        db  008h, 050h, 0E8h, 011h, 03Eh, 000h, 000h, 083h, 0C4h, 010h, 00Bh, 0C0h, 00Fh, 084h, 085h, 000h
        db  000h, 000h, 08Dh, 044h, 024h, 00Ch, 050h, 08Dh, 044h, 024h, 00Eh, 06Ah, 002h, 050h, 057h, 0E8h
        db  06Fh, 03Ch, 000h, 000h, 08Ah, 064h, 024h, 01Ah, 08Ah, 044h, 024h, 01Bh, 066h, 089h, 044h, 024h
        db  01Ah, 083h, 0C4h, 010h, 066h, 03Dh, 002h, 000h, 072h, 05Dh, 00Fh, 0B7h, 044h, 024h, 00Ah, 06Ah
        db  001h, 083h, 0E8h, 002h, 050h, 0E8h, 0AEh, 0FFh, 0FEh, 0FFh, 083h, 0C4h, 008h, 089h, 086h, 0A8h
        db  000h, 000h, 000h, 08Dh, 044h, 024h, 00Ch, 050h, 00Fh, 0B7h, 044h, 024h, 00Eh, 083h, 0E8h, 002h
        db  050h, 0FFh, 0B6h, 0A8h, 000h, 000h, 000h, 066h, 08Bh, 086h, 094h, 000h, 000h, 000h, 050h, 0E8h
        db  01Fh, 03Ch, 000h, 000h, 08Bh, 044h, 024h, 01Ch, 083h, 0C4h, 010h, 089h, 086h, 0A4h, 000h, 000h
        db  000h, 08Bh, 046h, 034h, 089h, 046h, 00Ch, 0C7h, 046h, 034h, 000h, 000h, 000h, 000h, 056h, 0E8h
        db  004h, 001h, 000h, 000h, 083h, 0C4h, 004h, 05Fh, 05Eh, 083h, 0C4h, 008h, 0C3h
sub_1_00BD:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        movzx  eax, word ptr [esi + 98h]
        cmp    eax, 1
        je     SHORT loc_1_00D9
        cmp    eax, 2
        je     SHORT loc_1_0128
        jmp    NEAR PTR loc_1_01B0
loc_1_00D9:
        mov    byte ptr [esi + 0b3h], 3
        mov    byte ptr [esi + 0b0h], 0ffh
        db  0C7h, 086h, 0B4h, 000h, 000h, 000h, 06Eh, 01Dh, 000h, 000h
        mov    edi, dword ptr [esp + 10h]
        cmp    edi, 1
        cmc
        sbb    al, al
        and    al, 4
        push   eax
        push   esi
        call   sub_1_03B7
        add    esp, 8
        cmp    eax, 10000h
        jne    NEAR PTR loc_1_01A2
        or     edi, edi
        je     NEAR PTR loc_1_01B0
        push   esi
        call   0
        add    esp, 4
        jmp    NEAR PTR loc_1_01B0
loc_1_0128:
        mov    byte ptr [esi + 0b3h], 3
        mov    byte ptr [esi + 0b0h], 10h
        db  0C7h, 086h, 0B4h, 000h, 000h, 000h, 048h, 01Dh, 000h, 000h
        mov    edi, dword ptr [esp + 10h]
        cmp    edi, 1
        sbb    al, al
        and    al, 0fch
        add    al, 14h
        push   eax
        push   esi
        call   sub_1_0287
        add    esp, 8
        cmp    eax, 10000h
        jne    SHORT loc_1_0176
        or     edi, edi
        je     SHORT loc_1_01B0
        push   esi
        call   sub_1_0920
        add    esp, 4
        push   esi
        call   sub_1_01B8
        add    esp, 4
        jmp    SHORT loc_1_01B0
loc_1_0176:
        or     eax, eax
        jne    SHORT loc_1_01A2
        cmp    edi, 1
        cmc
        sbb    al, al
        and    al, 4
        push   eax
        push   esi
        call   sub_1_03B7
        add    esp, 8
        cmp    eax, 10000h
        jne    SHORT loc_1_01A2
        or     edi, edi
        je     SHORT loc_1_01B0
        push   esi
        call   0
        add    esp, 4
        jmp    SHORT loc_1_01B0
loc_1_01A2:
        mov    byte ptr [esi + 0b2h], 80h
        mov    byte ptr [esi + 0b1h], 0ffh
loc_1_01B0:
        mov    eax, 1
        pop    edi
        pop    esi
        ret
sub_1_01B8:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    al, byte ptr [esi + 0b2h]
        cmp    al, 80h
        je     NEAR PTR loc_1_0283
        movzx  ebx, word ptr [esi + 94h]
        lea    edi, [ebx + 402h]
        add    ebx, 2
        cmp    al, 40h
        jne    SHORT loc_1_01F1
        push   14h
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        jmp    NEAR PTR loc_1_0283
loc_1_01F1:
        test   al, 10h
        je     SHORT loc_1_0221
        test   byte ptr [esi + 0b3h], 4
        je     SHORT loc_1_0216
        push   esi
        call   sub_1_07BB
        add    esp, 4
        push   ebx
        call   sub_1_3E90
        add    esp, 4
        and    al, 0dfh
        push   eax
        push   ebx
        jmp    SHORT loc_1_0219
loc_1_0216:
        push   34h
        push   edi
loc_1_0219:
        call   sub_1_3F2E
        add    esp, 8
loc_1_0221:
        cmp    dword ptr [esi + 0ach], 4
        je     SHORT loc_1_0237
        cmp    byte ptr [esi + 0b2h], 0
        jne    SHORT loc_1_0237
        push   1
        jmp    SHORT loc_1_0239
loc_1_0237:
        push   0
loc_1_0239:
        mov    ax, word ptr [esi + 94h]
        push   eax
        call   sub_1_3A70
        add    esp, 8
        cmp    word ptr [esi + 9ah], 2
        jne    SHORT loc_1_025E
        push   14h
        push   edi
        call   sub_1_3F2E
        add    esp, 8
loc_1_025E:
        mov    byte ptr [esi + 0b2h], 80h
        mov    byte ptr [esi + 0b3h], 3
        mov    dword ptr [esi + 0ach], 0
        mov    eax, dword ptr [esi + 34h]
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [esi + 34h], 0
loc_1_0283:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_0287:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 18h]
        mov    bx, word ptr [esi + 94h]
        movzx  ebp, bx
        lea    eax, [ebp + 1]
        mov    dword ptr [esp + 10h], eax
        lea    edi, [ebp + 2]
        add    ebp, 402h
        test   byte ptr [esp + 1ch], 4
        je     SHORT loc_1_02BD
        mov    dword ptr [esi + 0ach], 4
loc_1_02BD:
        push   34h
        push   ebp
        call   sub_1_3F2E
        add    esp, 8
        push   dword ptr [esp + 1ch]
        push   ebx
        call   sub_1_3B3C
        add    esp, 8
        mov    ebx, eax
        cmp    ebx, 10000h
        je     SHORT loc_1_02FE
        push   14h
        push   ebp
        call   sub_1_3F2E
        add    esp, 8
        mov    eax, dword ptr [esi + 34h]
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [esi + 34h], 8000h
        mov    eax, ebx
        jmp    NEAR PTR loc_1_03AF
loc_1_02FE:
        push   edi
        call   sub_1_3E90
        add    esp, 4
        or     al, 2
        push   eax
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   23h
        push   0f0h
        push   0f0h
        push   dword ptr [esp + 1ch]
        call   sub_1_3E38
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_0349
        push   esi
        call   sub_1_01B8
        add    esp, 4
        mov    eax, dword ptr [esi + 34h]
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [esi + 34h], 8000h
        sub    eax, eax
        jmp    SHORT loc_1_03AF
loc_1_0349:
        push   edi
        call   sub_1_3E90
        add    esp, 4
        and    al, 0fch
        push   eax
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   34h
        push   ebp
        call   sub_1_3F2E
        add    esp, 8
        push   74h
        push   ebp
        call   sub_1_3F2E
        add    esp, 8
        mov    byte ptr [esi + 0b2h], 10h
        mov    byte ptr [esi + 0b3h], 23h
        mov    byte ptr [esi + 0b1h], 10h
        db  0C7h, 086h, 0B4h, 000h, 000h, 000h, 022h, 01Dh, 000h, 000h
        db  0C7h, 086h, 0B8h, 000h, 000h, 000h, 0ECh, 01Dh, 000h, 000h
        mov    eax, dword ptr [esi + 34h]
        mov    dword ptr [esi + 0ch], eax
        mov    eax, 10000h
        mov    dword ptr [esi + 34h], 0
loc_1_03AF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_03B7:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    bl, byte ptr [esp + 18h]
        test   bl, 4
        mov    esi, dword ptr [esp + 14h]
        je     SHORT loc_1_03D2
        mov    dword ptr [esi + 0ach], 4
loc_1_03D2:
        mov    bp, word ptr [esi + 9ah]
        cmp    bp, 2
        jne    SHORT loc_1_03F6
        movzx  eax, word ptr [esi + 94h]
        add    eax, 402h
        push   34h
        push   eax
        call   sub_1_3F2E
        add    esp, 8
loc_1_03F6:
        mov    di, word ptr [esi + 94h]
        push   ebx
        push   edi
        call   sub_1_3B3C
        add    esp, 8
        mov    ebx, eax
        cmp    ebx, 10000h
        je     SHORT loc_1_043E
        cmp    bp, 2
        jne    SHORT loc_1_042A
        movzx  eax, di
        add    eax, 402h
        push   14h
        push   eax
        call   sub_1_3F2E
        add    esp, 8
loc_1_042A:
        mov    eax, dword ptr [esi + 34h]
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [esi + 34h], 8000h
        mov    eax, ebx
        jmp    NEAR PTR loc_1_04BF
loc_1_043E:
        mov    byte ptr [esi + 0b2h], 0
        mov    byte ptr [esi + 0b3h], 16h
        mov    byte ptr [esi + 0b1h], 0
        movzx  edi, di
        db  0C7h, 086h, 0B8h, 000h, 000h, 000h, 036h, 01Eh, 000h, 000h
        lea    eax, [edi + 1]
        push   eax
        call   sub_1_3E20
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_0479
        mov    byte ptr [esi + 0b3h], 1ch
        jmp    SHORT loc_1_04AD
loc_1_0479:
        add    edi, 2
        push   edi
        call   sub_1_3E90
        add    esp, 4
        or     al, 2
        push   eax
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   edi
        call   sub_1_3E90
        add    esp, 4
        or     al, 2
        push   eax
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        mov    byte ptr [esi + 0b3h], 16h
loc_1_04AD:
        mov    eax, dword ptr [esi + 34h]
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [esi + 34h], 0
        mov    eax, 10000h
loc_1_04BF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_04D0:
        push   esi
        push   edi
        db  068h, 094h, 048h, 000h, 000h
        db  0E8h, 024h, 009h, 0FFh, 0FFh
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 14h]
        add    esp, 4
loc_1_04E7:
        push   esi
        call   sub_1_3E90
        add    esp, 4
        and    al, 3
        cmp    al, 1
        je     SHORT loc_1_0517
        db  068h, 098h, 048h, 000h, 000h
        db  0E8h, 000h, 009h, 0FFh, 0FFh
        add    esp, 4
        db  0A1h, 098h, 048h, 000h, 000h
        db  02Bh, 005h, 094h, 048h, 000h, 000h
        cmp    eax, edi
        jl     SHORT loc_1_04E7
        sub    eax, eax
loc_1_0514:
        pop    edi
        pop    esi
        ret
loc_1_0517:
        mov    eax, 1
        jmp    SHORT loc_1_0514
sub_1_051E:
        push   esi
        push   edi
        db  068h, 094h, 048h, 000h, 000h
        db  0E8h, 0D6h, 008h, 0FFh, 0FFh
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 14h]
        add    esp, 4
loc_1_0535:
        push   esi
        call   sub_1_3E90
        add    esp, 4
        and    al, 3
        cmp    al, 2
        je     SHORT loc_1_0565
        db  068h, 098h, 048h, 000h, 000h
        db  0E8h, 0B2h, 008h, 0FFh, 0FFh
        add    esp, 4
        db  0A1h, 098h, 048h, 000h, 000h
        db  02Bh, 005h, 094h, 048h, 000h, 000h
        cmp    eax, edi
        jl     SHORT loc_1_0535
        sub    eax, eax
loc_1_0562:
        pop    edi
        pop    esi
        ret
loc_1_0565:
        mov    eax, 1
        jmp    SHORT loc_1_0562
        db  056h, 08Bh, 074h, 024h, 008h, 080h, 0BEh, 0B0h, 000h, 000h, 000h, 010h, 075h, 042h, 068h, 094h
        db  048h, 000h, 000h, 0E8h, 07Ch, 008h, 0FFh, 0FFh, 083h, 0C4h, 004h, 083h, 0BEh, 060h, 001h, 000h
        db  000h, 000h, 074h, 02Eh, 068h, 098h, 048h, 000h, 000h, 0E8h, 066h, 008h, 0FFh, 0FFh, 083h, 0C4h
        db  004h, 0A1h, 098h, 048h, 000h, 000h, 02Bh, 005h, 094h, 048h, 000h, 000h, 083h, 0F8h, 064h, 072h
        db  0DAh, 08Bh, 046h, 034h, 089h, 046h, 00Ch, 0C7h, 046h, 034h, 000h, 002h, 000h, 000h, 02Bh, 0C0h
        db  05Eh, 0C3h, 0B8h, 001h, 000h, 000h, 000h, 0EBh, 0F7h
sub_1_05C5:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0ch]
        movzx  edi, word ptr [eax + 94h]
        movzx  ecx, byte ptr [eax + 0b3h]
        lea    esi, [edi + 402h]
        add    edi, 2
        cmp    ecx, 23h
        je     SHORT loc_1_0607
        cmp    ecx, 26h
        je     SHORT loc_1_0614
        cmp    ecx, 2ch
        je     SHORT loc_1_0614
        push   10h
        push   eax
        call   sub_1_0287
        add    esp, 8
        cmp    eax, 10000h
        je     SHORT loc_1_0645
        sub    eax, eax
        jmp    SHORT loc_1_064A
loc_1_0607:
        push   34h
        push   esi
        call   sub_1_3F2E
        add    esp, 8
        jmp    SHORT loc_1_0625
loc_1_0614:
        push   eax
        call   sub_1_07BB
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_0625
        sub    eax, eax
        jmp    SHORT loc_1_064A
loc_1_0625:
        push   edi
        call   sub_1_3E90
        add    esp, 4
        and    al, 0dfh
        push   eax
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   74h
        push   esi
        call   sub_1_3F2E
        add    esp, 8
loc_1_0645:
        mov    eax, 1
loc_1_064A:
        pop    edi
        pop    esi
        ret
sub_1_064D:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 18h]
loc_1_0658:
        movzx  eax, byte ptr [esi + 0b3h]
        cmp    eax, 3
        je     SHORT loc_1_067B
        cmp    eax, 23h
        je     SHORT loc_1_068D
        cmp    eax, 26h
        je     SHORT loc_1_06A2
loc_1_066E:
        mov    eax, 1
loc_1_0673:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_067B:
        push   10h
        push   esi
        call   sub_1_0287
        add    esp, 8
        cmp    eax, 10000h
        jne    SHORT loc_1_069E
loc_1_068D:
        push   esi
        call   sub_1_0711
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_0658
        sub    eax, eax
        jmp    SHORT loc_1_0673
loc_1_069E:
        sub    eax, eax
        jmp    SHORT loc_1_0673
loc_1_06A2:
        movzx  edi, word ptr [esi + 94h]
        lea    eax, [edi + 402h]
        mov    dword ptr [esp + 10h], eax
        lea    ebp, [edi + 1]
        add    edi, 2
        push   edi
        call   sub_1_3E90
        add    esp, 4
        mov    bl, al
        or     bl, 20h
        push   ebx
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   74h
        push   dword ptr [esp + 14h]
        call   sub_1_3F2E
        add    esp, 8
        push   ebp
        call   sub_1_3E90
        add    esp, 4
        test   al, 40h
        je     SHORT loc_1_06F8
        mov    byte ptr [esi + 0b3h], 26h
        jmp    NEAR PTR loc_1_066E
loc_1_06F8:
        and    bl, 0fdh
        push   ebx
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        mov    byte ptr [esi + 0b3h], 2ch
        jmp    NEAR PTR loc_1_066E
sub_1_0711:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        push   0ah
        movzx  edi, word ptr [esi + 94h]
        lea    ebx, [edi + 402h]
        lea    ebp, [edi + 1]
        add    edi, 2
        push   ebx
        call   sub_1_04D0
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_074C
        mov    eax, dword ptr [esi + 34h]
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [esi + 34h], 100h
        sub    eax, eax
        jmp    SHORT loc_1_07B6
loc_1_074C:
        push   34h
        push   ebx
        call   sub_1_3F2E
        add    esp, 8
        push   edi
        call   sub_1_3E90
        add    esp, 4
        mov    bl, al
        or     bl, 2
        push   ebx
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   ebx
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        and    bl, 0fbh
        push   ebx
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   23h
        push   0
        push   20h
        push   ebp
        call   sub_1_3E38
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_07AA
        mov    eax, dword ptr [esi + 34h]
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [esi + 34h], 2000h
        sub    eax, eax
        jmp    SHORT loc_1_07B6
loc_1_07AA:
        mov    byte ptr [esi + 0b3h], 26h
        mov    eax, 1
loc_1_07B6:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_07BB:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 14h]
        push   34h
        movzx  ebp, word ptr [edi + 94h]
        lea    eax, [ebp + 402h]
        lea    esi, [ebp + 2]
        inc    ebp
        push   eax
        call   sub_1_3F2E
        add    esp, 8
        push   esi
        call   sub_1_3E90
        add    esp, 4
        mov    bl, al
        or     bl, 4
        push   ebx
        push   esi
        call   sub_1_3F2E
        add    esp, 8
        push   23h
        push   0e0h
        push   0e0h
        push   ebp
        call   sub_1_3E38
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_0821
        mov    eax, dword ptr [edi + 34h]
        mov    dword ptr [edi + 0ch], eax
        mov    dword ptr [edi + 34h], 2000h
        sub    eax, eax
        jmp    SHORT loc_1_083A
loc_1_0821:
        and    bl, 0fdh
        push   ebx
        push   esi
        call   sub_1_3F2E
        add    esp, 8
        mov    byte ptr [edi + 0b3h], 23h
        mov    eax, 1
loc_1_083A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_083F:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 20h]
        mov    esi, dword ptr [esp + 28h]
        movzx  eax, word ptr [edx + 94h]
        mov    dword ptr [esp + 18h], esi
        lea    ecx, [eax + 400h]
        mov    dword ptr [esp + 10h], ecx
        add    eax, 402h
        mov    dword ptr [esp + 14h], eax
        movzx  edi, word ptr [edx + 122h]
        mov    eax, esi
        cmp    edi, eax
        jb     SHORT loc_1_087B
        mov    edi, eax
loc_1_087B:
        mov    ecx, dword ptr [esp + 2ch]
        mov    dword ptr [ecx], esi
        movzx  eax, word ptr [edx + 120h]
        cmp    eax, 1
        je     SHORT loc_1_08AB
        cmp    eax, 2
        je     SHORT loc_1_08E4
loc_1_0892:
        mov    eax, dword ptr [esp + 18h]
        mov    ecx, dword ptr [esp + 2ch]
        sub    eax, esi
        mov    dword ptr [ecx], eax
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_1_08AB:
        mov    ebp, dword ptr [esp + 24h]
loc_1_08AF:
        push   64h
        push   dword ptr [esp + 18h]
        call   sub_1_04D0
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_0892
        movzx  ebx, di
        push   ebx
        push   ebp
        sub    esi, ebx
        push   dword ptr [esp + 18h]
        call   sub_1_3E9E
        add    esp, 0ch
        add    ebp, ebx
        cmp    di, si
        jbe    SHORT loc_1_08DE
        mov    di, si
loc_1_08DE:
        or     esi, esi
        jne    SHORT loc_1_08AF
        jmp    SHORT loc_1_0892
loc_1_08E4:
        mov    ebp, dword ptr [esp + 24h]
loc_1_08E8:
        push   64h
        push   dword ptr [esp + 18h]
        call   sub_1_04D0
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_0892
        movzx  ebx, di
        push   ebx
        push   ebp
        sub    esi, ebx
        push   dword ptr [esp + 18h]
        call   sub_1_3ECB
        add    esp, 0ch
        add    ebp, ebx
        cmp    di, si
        jbe    SHORT loc_1_0917
        mov    di, si
loc_1_0917:
        or     esi, esi
        jne    SHORT loc_1_08E8
        jmp    NEAR PTR loc_1_0892
sub_1_0920:
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        push   34h
        movzx  edi, word ptr [esi + 94h]
        add    edi, 402h
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   esi
        call   sub_1_0711
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_0A06
        push   64h
        push   edi
        call   sub_1_04D0
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_0973
        mov    eax, dword ptr [esi + 34h]
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [esi + 34h], 0
        jmp    NEAR PTR loc_1_0A06
loc_1_0973:
        lea    eax, [esp + 0ch]
        push   eax
        lea    eax, [esp + 0eh]
        push   2
        push   eax
        push   esi
        call   sub_1_083F
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_0A06
        mov    ah, byte ptr [esp + 0ah]
        mov    al, byte ptr [esp + 0bh]
        mov    word ptr [esp + 0ah], ax
        cmp    ax, 2
        jbe    SHORT loc_1_0A06
        sub    word ptr [esp + 0ah], 2
        cmp    word ptr [esi + 120h], 2
        jne    SHORT loc_1_09C8
        mov    ax, word ptr [esp + 0ah]
        shr    ax, 1
        mov    cx, word ptr [esp + 0ah]
        and    cx, 1
        add    ax, cx
        mov    word ptr [esp + 0ah], ax
loc_1_09C8:
        movzx  eax, word ptr [esp + 0ah]
        push   1
        push   eax
        db  0E8h, 043h, 0F6h, 0FEh, 0FFh
        add    esp, 8
        mov    dword ptr [esi + 0a8h], eax
        lea    eax, [esp + 0ch]
        push   eax
        movzx  eax, word ptr [esp + 0eh]
        push   eax
        push   dword ptr [esi + 0a8h]
        push   esi
        call   sub_1_083F
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_0A06
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [esi + 0a4h], eax
loc_1_0A06:
        pop    edi
        pop    esi
        add    esp, 8
        ret
        db  056h, 057h, 08Bh, 074h, 024h, 00Ch, 06Ah, 014h, 00Fh, 0B7h, 086h, 094h, 000h, 000h, 000h, 08Dh
        db  078h, 002h, 005h, 002h, 004h, 000h, 000h, 050h, 0E8h, 005h, 035h, 000h, 000h, 083h, 0C4h, 008h
        db  08Bh, 086h, 034h, 001h, 000h, 000h, 066h, 081h, 078h, 00Ah, 000h, 008h, 075h, 00Eh, 0FFh, 0B6h
        db  038h, 001h, 000h, 000h, 0E8h, 0AEh, 02Eh, 000h, 000h, 083h, 0C4h, 004h, 08Bh, 086h, 034h, 001h
        db  000h, 000h, 0C7h, 086h, 058h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 066h, 0C7h, 040h, 00Ah
        db  000h, 000h, 0C7h, 046h, 058h, 000h, 000h, 000h, 000h, 0C7h, 046h, 05Ch, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 04Ch, 000h, 000h, 000h, 000h, 0C7h, 046h, 054h, 000h, 000h, 000h, 000h, 056h, 0E8h
        db  0ECh, 0FAh, 0FFh, 0FFh, 083h, 0C4h, 004h, 00Bh, 0C0h, 0B8h, 000h, 000h, 000h, 000h, 074h, 038h
        db  056h, 0E8h, 026h, 0F7h, 0FFh, 0FFh, 083h, 0C4h, 004h, 068h, 0CCh, 000h, 000h, 000h, 057h, 0E8h
        db  08Eh, 034h, 000h, 000h, 083h, 0C4h, 008h, 068h, 0C8h, 000h, 000h, 000h, 057h, 0E8h, 080h, 034h
        db  000h, 000h, 083h, 0C4h, 008h, 068h, 0CCh, 000h, 000h, 000h, 057h, 0E8h, 072h, 034h, 000h, 000h
        db  083h, 0C4h, 008h, 0B8h, 001h, 000h, 000h, 000h, 05Fh, 05Eh, 0C3h
sub_1_0AC7:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        movzx  edi, word ptr [esi + 94h]
        add    edi, 2
        push   edi
        call   sub_1_3E90
        add    esp, 4
        and    al, 40h
        mov    byte ptr [esp + 0fh], al
        mov    bl, al
        or     bl, 0ch
        push   ebx
        push   edi
        call   sub_1_3F2E
        mov    al, byte ptr [esp + 17h]
        add    esp, 8
        or     al, 8
        push   eax
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        db  068h, 094h, 048h, 000h, 000h
        db  0E8h, 0EDh, 002h, 0FFh, 0FFh
        add    esp, 4
loc_1_0B16:
        db  068h, 098h, 048h, 000h, 000h
        db  0E8h, 0E0h, 002h, 0FFh, 0FFh
        add    esp, 4
        db  0A1h, 098h, 048h, 000h, 000h
        db  02Bh, 005h, 094h, 048h, 000h, 000h
        cmp    eax, 12ch
        jl     SHORT loc_1_0B16
        push   ebx
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        movzx  eax, word ptr [esi + 94h]
        inc    eax
        push   eax
        push   esi
        call   sub_1_1EA5
        add    esp, 8
        mov    byte ptr [esi + 0b3h], 3
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  056h, 08Bh, 074h, 024h, 008h, 083h, 0BEh, 060h, 001h, 000h, 000h, 000h, 075h, 037h, 056h, 0E8h
        db  09Eh, 0FBh, 0FFh, 0FFh, 083h, 0C4h, 004h, 00Bh, 0C0h, 074h, 02Ah, 066h, 083h, 0BEh, 020h, 001h
        db  000h, 000h, 001h, 056h, 075h, 007h, 0E8h, 029h, 02Ah, 000h, 000h, 0EBh, 005h, 0E8h, 078h, 028h
        db  000h, 000h, 083h, 0C4h, 004h, 080h, 0B6h, 06Eh, 001h, 000h, 000h, 001h, 056h, 0E8h, 01Ah, 0FCh
        db  0FFh, 0FFh, 083h, 0C4h, 004h, 05Eh, 0C3h
sub_1_0BA6:
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0BB0:
        sub    esp, 8
        push   esi
        mov    dword ptr [esp + 4], edx
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 140h], 0
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax + 3bh], 1
        je     SHORT loc_1_0BE1
        push   eax
        db  0E8h, 0BEh, 0F7h, 0FEh, 0FFh
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_0DC1
loc_1_0BE1:
        mov    eax, dword ptr [esp + 4]
        or     dword ptr [eax + 168h], 80h
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 160h], 0
        je     SHORT loc_1_0C0A
        cmp    byte ptr [eax + 0b2h], 0
        je     SHORT loc_1_0C0A
        jmp    NEAR PTR loc_1_0DB7
loc_1_0C0A:
        mov    eax, dword ptr [esp + 4]
        mov    al, byte ptr [eax + 0b2h]
        or     al, al
        je     SHORT loc_1_0C6A
        cmp    al, 40h
        je     SHORT loc_1_0C20
        cmp    al, 10h
        jne    SHORT loc_1_0C44
loc_1_0C20:
        mov    eax, dword ptr [esp + 4]
        push   64h
        movzx  eax, word ptr [eax + 94h]
        add    eax, 402h
        push   eax
        call   sub_1_04D0
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_0C44
        jmp    NEAR PTR loc_1_0DB3
loc_1_0C44:
        push   dword ptr [esp + 4]
        call   sub_1_01B8
        add    esp, 4
        push   0
        push   dword ptr [esp + 8]
        call   sub_1_03B7
        add    esp, 8
        cmp    eax, 10000h
        je     SHORT loc_1_0C6A
        jmp    NEAR PTR loc_1_0DB3
loc_1_0C6A:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 160h], 1
        mov    eax, dword ptr [esp + 4]
        db  0C7h, 080h, 0BCh, 000h, 000h, 000h, 02Eh, 017h, 000h, 000h
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 14ch], 10h
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 4]
        add    ecx, 54h
        mov    esi, dword ptr [eax + 40h]
        sub    esi, dword ptr [ecx]
        mov    dword ptr [esp + 8], 0
        mov    eax, dword ptr [eax + 3ch]
        add    eax, dword ptr [ecx]
        lea    ecx, [esp + 8]
        push   ecx
        push   esi
        push   eax
        mov    eax, dword ptr [esp + 10h]
        mov    ax, word ptr [eax + 94h]
        push   eax
        call   sub_1_3CB3
        add    esp, 10h
        cmp    dword ptr [esp + 8], 0
        je     SHORT loc_1_0D20
        call   sub_1_39D1
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx + 88h], eax
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        add    dword ptr [eax + 4ch], ecx
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        add    dword ptr [eax + 54h], ecx
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax + 150h], 1
        je     SHORT loc_1_0D20
        mov    ecx, eax
        mov    eax, dword ptr [eax + 54h]
        cmp    dword ptr [ecx + 0f0h], eax
        jg     SHORT loc_1_0D20
        push   1
        push   dword ptr [esp + 8]
        call   sub_1_1B99
        add    esp, 8
loc_1_0D20:
        cmp    dword ptr [esp + 8], esi
        jne    SHORT loc_1_0D53
        push   dword ptr [esp + 4]
        call   sub_1_01B8
        mov    eax, dword ptr [esp + 8]
        add    esp, 4
        mov    dword ptr [eax + 160h], 0
        mov    eax, dword ptr [esp + 4]
        xor    dword ptr [eax + 168h], 80h
        jmp    NEAR PTR loc_1_0DDE
loc_1_0D53:
        mov    eax, dword ptr [esp + 4]
        movzx  esi, word ptr [eax + 94h]
        lea    eax, [esi + 1]
        push   eax
        call   sub_1_3E20
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_0D7B
        mov    eax, dword ptr [esp + 4]
        mov    byte ptr [eax + 0b3h], 1ch
        jmp    SHORT loc_1_0DB3
loc_1_0D7B:
        add    esi, 2
        push   esi
        call   sub_1_3E90
        add    esp, 4
        or     al, 2
        push   eax
        push   esi
        call   sub_1_3F2E
        add    esp, 8
        push   esi
        call   sub_1_3E90
        add    esp, 4
        or     al, 2
        push   eax
        push   esi
        call   sub_1_3F2E
        mov    eax, dword ptr [esp + 0ch]
        add    esp, 8
        mov    byte ptr [eax + 0b3h], 16h
loc_1_0DB3:
        mov    eax, dword ptr [esp + 4]
loc_1_0DB7:
        xor    dword ptr [eax + 168h], 80h
loc_1_0DC1:
        push   dword ptr [esp + 4]
        push   3e8h
        db  068h, 0B0h, 00Bh, 000h, 000h
        db  0E8h, 06Ch, 0F2h, 0FEh, 0FFh
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx + 140h], eax
loc_1_0DDE:
        pop    esi
        add    esp, 8
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0DF0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 18h]
        movzx  ebx, word ptr [edi + 94h]
        mov    esi, dword ptr [edi + 3ch]
        lea    eax, [ebx + 402h]
        mov    dword ptr [esp + 10h], eax
        add    ebx, 400h
        add    esi, dword ptr [edi + 54h]
        push   eax
        call   sub_1_3E90
        add    esp, 4
        and    al, 3
        cmp    al, 1
        je     SHORT loc_1_0E7F
loc_1_0E27:
        mov    ebp, dword ptr [edi + 4ch]
        cmp    ebp, dword ptr [edi + 40h]
        ja     SHORT loc_1_0E7F
        movzx  eax, word ptr [edi + 120h]
        cmp    eax, 1
        je     SHORT loc_1_0E42
        cmp    eax, 2
        je     SHORT loc_1_0E57
        jmp    SHORT loc_1_0E6D
loc_1_0E42:
        push   ebx
        call   sub_1_3E90
        add    esp, 4
        mov    byte ptr [esi], al
        inc    esi
        inc    ebp
        mov    dword ptr [edi + 4ch], ebp
        inc    dword ptr [edi + 54h]
        jmp    SHORT loc_1_0E6D
loc_1_0E57:
        mov    dx, bx
        in     ax, dx
        mov    byte ptr [esi], al
        mov    byte ptr [esi + 1], ah
        inc    esi
        inc    esi
        add    ebp, 2
        mov    dword ptr [edi + 4ch], ebp
        add    dword ptr [edi + 54h], 2
loc_1_0E6D:
        push   dword ptr [esp + 10h]
        call   sub_1_3E90
        add    esp, 4
        and    al, 3
        cmp    al, 1
        jne    SHORT loc_1_0E27
loc_1_0E7F:
        call   sub_1_39D1
        mov    dword ptr [edi + 88h], eax
        test   byte ptr [edi + 150h], 1
        je     SHORT loc_1_0EA9
        mov    eax, dword ptr [edi + 54h]
        cmp    dword ptr [edi + 0f0h], eax
        jg     SHORT loc_1_0EA9
        push   1
        push   edi
        call   sub_1_1B99
        add    esp, 8
loc_1_0EA9:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_0EB1:
        sub    esp, 4
        push   esi
        push   edi
        mov    dword ptr [esp + 8], edx
        mov    eax, dword ptr [esp + 8]
        movzx  esi, word ptr [eax + 94h]
        add    esi, 402h
        cmp    dword ptr [eax + 14ch], 8
        jne    SHORT loc_1_0F0C
        movzx  eax, word ptr [eax + 120h]
        cmp    eax, 1
        je     SHORT loc_1_0EEA
        cmp    eax, 2
        je     SHORT loc_1_0EFB
        jmp    NEAR PTR loc_1_0FE0
loc_1_0EEA:
        push   dword ptr [esp + 8]
        call   sub_1_35B3
        add    esp, 4
        jmp    NEAR PTR loc_1_0FE0
loc_1_0EFB:
        push   dword ptr [esp + 8]
        call   sub_1_3409
        add    esp, 4
        jmp    NEAR PTR loc_1_0FE0
loc_1_0F0C:
        mov    eax, dword ptr [esp + 8]
        mov    edi, dword ptr [eax + 58h]
        or     edi, edi
        jne    SHORT loc_1_0F7C
        push   0ah
        push   esi
        call   sub_1_04D0
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_0F45
        push   dword ptr [esp + 8]
        push   0ah
        db  068h, 0B1h, 00Eh, 000h, 000h
        db  0E8h, 00Ah, 0F1h, 0FEh, 0FFh
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx + 13ch], eax
        jmp    NEAR PTR loc_1_0FE0
loc_1_0F45:
        mov    eax, dword ptr [esp + 8]
        cmp    dword ptr [eax + 0ech], edi
        jg     SHORT loc_1_0F55
        or     edi, edi
        jne    SHORT loc_1_0F7C
loc_1_0F55:
        push   dword ptr [esp + 8]
        call   sub_1_196F
        mov    eax, dword ptr [esp + 0ch]
        add    esp, 4
        test   byte ptr [eax + 150h], 2
        je     SHORT loc_1_0F7C
        push   2
        push   dword ptr [esp + 0ch]
        call   sub_1_1B99
        add    esp, 8
loc_1_0F7C:
        mov    eax, dword ptr [esp + 8]
        cmp    dword ptr [eax + 58h], 0
        je     SHORT loc_1_0FC4
        push   esi
        call   sub_1_3E90
        mov    ecx, dword ptr [esp + 0ch]
        add    esp, 4
        movzx  ecx, word ptr [ecx + 120h]
        cmp    ecx, 1
        je     SHORT loc_1_0FA6
        cmp    ecx, 2
        je     SHORT loc_1_0FB5
        jmp    SHORT loc_1_0FE0
loc_1_0FA6:
        push   eax
        push   dword ptr [esp + 0ch]
        call   sub_1_2F45
        add    esp, 8
        jmp    SHORT loc_1_0FE0
loc_1_0FB5:
        push   eax
        push   dword ptr [esp + 0ch]
        call   sub_1_2DFC
        add    esp, 8
        jmp    SHORT loc_1_0FE0
loc_1_0FC4:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 14ch], 0
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 160h], 0
loc_1_0FE0:
        pop    edi
        pop    esi
        add    esp, 4
        ret
loc_1_0FE6:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], edx
        mov    eax, dword ptr [ebp - 4]
        movzx  ecx, word ptr [eax + 94h]
        add    ecx, 402h
        mov    eax, dword ptr [eax + 14ch]
        cmp    eax, 1
        je     SHORT loc_1_1018
        cmp    eax, 2
        je     NEAR PTR loc_1_113D
        jmp    NEAR PTR loc_1_11CC
loc_1_1018:
        push   0ah
        push   ecx
        call   sub_1_04D0
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_1044
        push   dword ptr [ebp - 4]
        push   0ah
        db  068h, 0E6h, 00Fh, 000h, 000h
        db  0E8h, 00Ah, 0F0h, 0FEh, 0FFh
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 13ch], eax
        jmp    NEAR PTR loc_1_11CC
loc_1_1044:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 158h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 160h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 134h]
        cmp    word ptr [eax + 0ah], 800h
        jne    SHORT loc_1_10D7
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 138h]
        mov    cx, word ptr [eax + 0eh]
        cmp    word ptr [eax + 0ch], cx
        jne    SHORT loc_1_109A
        push   eax
        call   sub_1_38F3
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 134h]
        mov    word ptr [eax + 0ah], 0
loc_1_109A:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 58h], 0
        je     SHORT loc_1_10D7
        mov    eax, dword ptr [eax + 138h]
        mov    cx, word ptr [eax + 0eh]
        cmp    word ptr [eax + 0ch], cx
        jae    SHORT loc_1_112E
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0b2h]
        sub    al, 10h
        cmp    al, 1
        sbb    al, al
        and    al, 20h
        add    al, 54h
        push   eax
        push   dword ptr [ebp - 4]
        call   sub_1_2950
        add    esp, 8
        jmp    NEAR PTR loc_1_11CC
loc_1_10D7:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, ecx
        mov    eax, dword ptr [eax + 0ech]
        cmp    dword ptr [ecx + 58h], eax
        jge    SHORT loc_1_1121
        push   ecx
        call   sub_1_196F
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        test   byte ptr [eax + 150h], 2
        je     SHORT loc_1_1107
        push   2
        push   eax
        call   sub_1_1B99
        add    esp, 8
loc_1_1107:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 160h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14ch], 0
loc_1_1121:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 58h], 0
        je     NEAR PTR loc_1_11CC
loc_1_112E:
        push   edx
        mov    edx, dword ptr [ebp - 4]
        call   sub_1_2A16
        pop    edx
        jmp    NEAR PTR loc_1_11CC
loc_1_113D:
        call   sub_1_39D1
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 88h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 160h], 0
        push   dword ptr [ebp - 4]
        call   sub_1_17B0
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 158h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 134h]
        mov    word ptr [eax + 0ah], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14ch], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, ecx
        mov    eax, dword ptr [eax + 0f0h]
        cmp    dword ptr [ecx + 54h], eax
        jl     SHORT loc_1_11B4
        mov    eax, ecx
        test   byte ptr [eax + 150h], 1
        je     SHORT loc_1_11CC
        push   1
        push   ecx
        call   sub_1_1B99
        add    esp, 8
        jmp    SHORT loc_1_11CC
loc_1_11B4:
        push   dword ptr [ebp - 4]
        push   0ah
        db  068h, 02Ah, 02Ch, 000h, 000h
        db  0E8h, 07Dh, 0EEh, 0FEh, 0FFh
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 140h], eax
loc_1_11CC:
        mov    esp, ebp
        pop    ebp
        ret
sub_1_11D0:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        test   byte ptr [esi + 16eh], 1
        jne    SHORT loc_1_1247
        mov    bl, byte ptr [esp + 14h]
        test   bl, 10h
        jne    SHORT loc_1_1247
        movzx  eax, word ptr [esi + 94h]
        lea    edi, [eax + 402h]
        inc    eax
        push   eax
        call   sub_1_3E20
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_1247
        mov    al, bl
        or     al, 14h
        push   eax
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        mov    al, byte ptr [esi + 16eh]
        or     al, 1
        mov    byte ptr [esi + 16eh], al
        push   200h
        push   esi
        call   sub_1_1C08
        add    esp, 8
        test   bl, 4
        jne    SHORT loc_1_1240
        and    bl, 0fbh
        push   ebx
        push   edi
        call   sub_1_3F2E
        add    esp, 8
loc_1_1240:
        mov    eax, 1
        jmp    SHORT loc_1_1249
loc_1_1247:
        sub    eax, eax
loc_1_1249:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_124D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        sti
        mov    esp, ebp
        pop    ebp
        ret
loc_1_1267:
        sub    esp, 0ch
        push   ebx
        push   esi
        mov    dword ptr [esp + 10h], eax
        mov    dword ptr [esp + 0ch], edx
        mov    eax, dword ptr [esp + 0ch]
        movzx  esi, word ptr [eax + 94h]
        add    esi, 402h
        push   esi
        call   sub_1_3E90
        add    esp, 4
        mov    byte ptr [esp + 0bh], al
        mov    bl, al
        and    bl, 0e0h
        cmp    bl, 60h
        je     SHORT loc_1_12B3
        cmp    bl, 40h
        je     SHORT loc_1_12B3
        push   dword ptr [esp + 10h]
        call   sub_1_124D
        add    esp, 4
        stc
        jmp    NEAR PTR loc_1_1341
loc_1_12B3:
        mov    eax, dword ptr [esp + 0ch]
        test   byte ptr [eax + 16eh], 80h
        je     SHORT loc_1_12D0
        push   dword ptr [esp + 0bh]
        push   dword ptr [esp + 10h]
        call   sub_1_11D0
        add    esp, 8
loc_1_12D0:
        test   byte ptr [esp + 0bh], 4
        jne    SHORT loc_1_12E6
        push   dword ptr [esp + 10h]
        call   sub_1_124D
        add    esp, 4
        stc
        jmp    SHORT loc_1_1341
loc_1_12E6:
        sub    bl, 60h
        cmp    bl, 1
        sbb    al, al
        and    al, 20h
        add    al, 54h
        push   eax
        push   esi
        call   sub_1_3F2E
        add    esp, 8
        push   dword ptr [esp + 10h]
        call   sub_1_124D
        mov    eax, dword ptr [esp + 10h]
        add    esp, 4
        cmp    dword ptr [eax + 164h], 0
        je     SHORT loc_1_1340
        mov    ecx, dword ptr [esp + 10h]
        cmp    dword ptr [eax + 0f8h], ecx
        jne    SHORT loc_1_1340
        cmp    dword ptr [eax + 158h], 0
        je     SHORT loc_1_1332
        push   eax
        db  068h, 0E6h, 00Fh, 000h, 000h
        jmp    SHORT loc_1_133B
loc_1_1332:
        push   dword ptr [esp + 0ch]
        db  068h, 0B1h, 00Eh, 000h, 000h
loc_1_133B:
        db  0E8h, 0E8h, 0ECh, 0FEh, 0FFh
loc_1_1340:
        clc
loc_1_1341:
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  083h, 0ECh, 004h, 053h, 056h, 057h, 055h, 08Bh, 074h, 024h, 018h, 00Fh, 0B7h, 0BEh, 094h, 000h
        db  000h, 000h, 08Dh, 06Fh, 002h, 047h, 057h, 0E8h, 02Dh, 02Bh, 000h, 000h, 083h, 0C4h, 004h, 0A8h
        db  080h, 075h, 005h, 0E9h, 0A2h, 000h, 000h, 000h, 055h, 0E8h, 01Bh, 02Bh, 000h, 000h, 083h, 0C4h
        db  004h, 08Ah, 0D8h, 00Ch, 020h, 050h, 055h, 0E8h, 0ABh, 02Bh, 000h, 000h, 083h, 0C4h, 008h, 080h
        db  0E3h, 0EBh, 053h, 055h, 0E8h, 09Eh, 02Bh, 000h, 000h, 083h, 0C4h, 008h, 06Ah, 023h, 06Ah, 000h
        db  06Ah, 040h, 057h, 0E8h, 099h, 02Ah, 000h, 000h, 083h, 0C4h, 010h, 00Bh, 0C0h, 074h, 05Ah, 08Ah
        db  0C3h, 024h, 0FEh, 088h, 044h, 024h, 010h, 0FFh, 074h, 024h, 010h, 055h, 0E8h, 076h, 02Bh, 000h
        db  000h, 083h, 0C4h, 008h, 080h, 0CBh, 0FBh, 0FFh, 074h, 024h, 010h, 055h, 0E8h, 066h, 02Bh, 000h
        db  000h, 083h, 0C4h, 008h, 053h, 055h, 0E8h, 05Ch, 02Bh, 000h, 000h, 083h, 0C4h, 008h, 053h, 055h
        db  0E8h, 052h, 02Bh, 000h, 000h, 083h, 0C4h, 008h, 06Ah, 023h, 06Ah, 040h, 06Ah, 040h, 057h, 0E8h
        db  04Dh, 02Ah, 000h, 000h, 083h, 0C4h, 010h, 00Bh, 0C0h, 074h, 00Eh, 0C6h, 086h, 0B3h, 000h, 000h
        db  000h, 023h, 0B8h, 001h, 000h, 000h, 000h, 0EBh, 013h, 0C7h, 046h, 034h, 000h, 004h, 000h, 000h
        db  0C7h, 086h, 060h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 02Bh, 0C0h, 05Dh, 05Fh, 05Eh, 05Bh
        db  083h, 0C4h, 004h, 0C3h
sub_1_141B:
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [esp + 10h], 1
        mov    dword ptr [esp + 14h], 3
        mov    dword ptr [esp + 18h], 5
        mov    dword ptr [esp + 1ch], 7
        mov    dword ptr [esp + 20h], 0c2h
        mov    dword ptr [esp + 24h], 0c6h
        mov    dword ptr [esp + 28h], 0cah
        mov    si, word ptr [esp + 34h]
        mov    dword ptr [esp + 2ch], 0ceh
        movzx  eax, si
        mov    edi, dword ptr [esp + eax*4 + 10h]
        push   edi
        call   sub_1_3E90
        add    esp, 4
        push   edi
        call   sub_1_3E90
        add    esp, 4
loc_1_147F:
        mov    bl, al
        push   edi
        call   sub_1_3E90
        mov    byte ptr [esp + 13h], al
        add    esp, 4
        push   edi
        call   sub_1_3E90
        add    esp, 4
        cmp    bl, al
        jne    SHORT loc_1_147F
        movzx  ax, al
        movzx  cx, byte ptr [esp + 0fh]
        shl    ax, 8
        or     ax, cx
        inc    ax
        cmp    si, 3
        jbe    SHORT loc_1_14B7
        add    ax, ax
loc_1_14B7:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
sub_1_14BE:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        movzx  edi, word ptr [esi + 94h]
        add    edi, 402h
        push   dword ptr [esi + 130h]
        call   sub_1_38B6
        add    esp, 4
        push   edi
        call   sub_1_3E90
        add    esp, 4
        and    al, 0f7h
        or     al, 14h
        push   eax
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        mov    ax, word ptr [esi + 126h]
        push   eax
        call   sub_1_141B
        add    esp, 4
        movzx  ecx, ax
        mov    eax, dword ptr [esi + 14ch]
        cmp    eax, 1
        je     SHORT loc_1_151C
        cmp    eax, 2
        je     SHORT loc_1_1536
        jmp    SHORT loc_1_1587
loc_1_151C:
        mov    eax, dword ptr [esi + 58h]
        add    eax, ecx
        mov    dword ptr [esi + 58h], eax
        sub    dword ptr [esi + 5ch], ecx
        or     eax, eax
        jne    SHORT loc_1_1587
        push   esi
        call   sub_1_19E0
        add    esp, 4
        jmp    SHORT loc_1_1587
loc_1_1536:
        movzx  ebx, word ptr [esi + 94h]
        mov    eax, dword ptr [esi + 134h]
        push   ecx
        push   dword ptr [eax + 4]
        add    ebx, 2
        push   1
        push   esi
        call   sub_1_1870
        add    esp, 10h
        push   esi
        call   sub_1_0DF0
        add    esp, 4
        push   esi
        call   sub_1_07BB
        add    esp, 4
        push   ebx
        call   sub_1_3E90
        add    esp, 4
        and    al, 0dfh
        push   eax
        push   ebx
        call   sub_1_3F2E
        add    esp, 8
        push   74h
        push   edi
        call   sub_1_3F2E
        add    esp, 8
loc_1_1587:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_158B:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_14BE
        add    esp, 4
        mov    dword ptr [esi + 160h], 0
        mov    dword ptr [esi + 158h], 0
        mov    eax, dword ptr [esi + 14ch]
        cmp    eax, 1
        je     SHORT loc_1_15C2
        cmp    eax, 2
        je     SHORT loc_1_161F
        jmp    NEAR PTR loc_1_1646
loc_1_15C2:
        mov    eax, dword ptr [esi + 134h]
        cmp    word ptr [eax + 0ah], 800h
        jne    SHORT loc_1_15EA
        push   dword ptr [esi + 138h]
        call   sub_1_38F3
        add    esp, 4
        mov    eax, dword ptr [esi + 134h]
        mov    word ptr [eax + 0ah], 0
loc_1_15EA:
        mov    eax, dword ptr [esi + 13ch]
        or     eax, eax
        je     SHORT loc_1_1604
        push   eax
        db  0E8h, 062h, 0EAh, 0FEh, 0FFh
        mov    dword ptr [esi + 13ch], 0
loc_1_1604:
        cmp    dword ptr [esi + 58h], 0
        je     SHORT loc_1_1646
        push   esi
        push   0ah
        db  068h, 016h, 02Ah, 000h, 000h
        db  0E8h, 029h, 0EAh, 0FEh, 0FFh
        mov    dword ptr [esi + 13ch], eax
        jmp    SHORT loc_1_1646
loc_1_161F:
        cmp    dword ptr [esi + 140h], 0
        jne    SHORT loc_1_1646
        mov    eax, dword ptr [esi + 54h]
        cmp    dword ptr [esi + 0f0h], eax
        jle    SHORT loc_1_1646
        push   esi
        push   0ah
        db  068h, 02Ah, 02Ch, 000h, 000h
        db  0E8h, 000h, 0EAh, 0FEh, 0FFh
        mov    dword ptr [esi + 140h], eax
loc_1_1646:
        pop    esi
        ret
loc_1_1648:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 58h], 0
        jne    SHORT loc_1_165C
        push   esi
        call   sub_1_19E0
        add    esp, 4
loc_1_165C:
        mov    dword ptr [esi + 160h], 0
        mov    eax, dword ptr [esi + 13ch]
        or     eax, eax
        je     SHORT loc_1_1680
        push   eax
        db  0E8h, 0E6h, 0E9h, 0FEh, 0FFh
        mov    dword ptr [esi + 13ch], 0
loc_1_1680:
        cmp    dword ptr [esi + 58h], 0
        je     SHORT loc_1_1699
        push   esi
        push   0ah
        db  068h, 0AFh, 02Fh, 000h, 000h
        db  0E8h, 0ADh, 0E9h, 0FEh, 0FFh
        mov    dword ptr [esi + 13ch], eax
loc_1_1699:
        pop    esi
        ret
loc_1_169B:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 10h]
        movzx  edi, word ptr [ebx + 94h]
        lea    esi, [edi + 2]
        add    edi, 402h
        push   edi
        call   sub_1_3E90
        add    esp, 4
        or     al, 14h
        push   eax
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   ebx
        call   sub_1_0DF0
        add    esp, 4
        push   ebx
        call   sub_1_07BB
        add    esp, 4
        push   esi
        call   sub_1_3E90
        add    esp, 4
        and    al, 0dfh
        push   eax
        push   esi
        call   sub_1_3F2E
        add    esp, 8
        push   74h
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        mov    dword ptr [ebx + 160h], 0
        cmp    dword ptr [ebx + 140h], 0
        jne    SHORT loc_1_172A
        mov    eax, dword ptr [ebx + 54h]
        cmp    dword ptr [ebx + 0f0h], eax
        jle    SHORT loc_1_172A
        push   ebx
        push   0ah
        db  068h, 05Ah, 036h, 000h, 000h
        db  0E8h, 01Ch, 0E9h, 0FEh, 0FFh
        mov    dword ptr [ebx + 140h], eax
loc_1_172A:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_172E:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, word ptr [eax + 94h]
        inc    eax
        cmp    byte ptr [ecx + 0b2h], 0
        jne    SHORT loc_1_176B
        push   eax
        call   sub_1_3E20
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_176B
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 140h], 0
        jne    SHORT loc_1_176B
        push   edx
        mov    edx, dword ptr [ebp + 8]
        call   sub_1_0BB0
        pop    edx
loc_1_176B:
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax + 160h], 0
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_1_1780:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    edi, dword ptr [esi + 134h]
        mov    eax, dword ptr [esi + 148h]
        mov    dword ptr [edi], eax
        push   eax
        push   dword ptr [edi + 4]
        push   2
        push   esi
        call   sub_1_17CB
        add    esp, 10h
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 134h]
        pop    edi
        mov    eax, dword ptr [eax]
        pop    esi
        ret
sub_1_17B0:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 134h]
        push   dword ptr [ecx]
        push   dword ptr [ecx + 4]
        push   1
        push   eax
        call   sub_1_1870
        add    esp, 10h
        ret
sub_1_17CB:
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        mov    eax, dword ptr [esp + 18h]
        cmp    eax, 1
        je     SHORT loc_1_17E4
        cmp    eax, 2
        je     SHORT loc_1_182F
        jmp    NEAR PTR loc_1_1869
loc_1_17E4:
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esi + 50h]
        mov    ecx, dword ptr [esi + 3ch]
        add    ecx, edi
        mov    ebp, dword ptr [esi + 4ch]
        mov    ebx, ebp
        or     ebx, ebx
        jne    SHORT loc_1_180A
        mov    eax, dword ptr [esi + 34h]
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [esi + 34h], 2
        sub    eax, eax
        jmp    SHORT loc_1_186B
loc_1_180A:
        mov    eax, dword ptr [esp + 20h]
        cmp    ebx, eax
        jbe    SHORT loc_1_1814
        mov    ebx, eax
loc_1_1814:
        push   ebx
        push   ecx
        sub    ebp, ebx
        push   dword ptr [esp + 24h]
        call   sub_1_39DA
        add    esp, 0ch
        lea    eax, [edi + ebx]
        mov    dword ptr [esi + 50h], eax
        mov    dword ptr [esi + 4ch], ebp
        jmp    SHORT loc_1_1869
loc_1_182F:
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esi + 5ch]
        mov    eax, dword ptr [esi + 0f4h]
        mov    ebp, dword ptr [esi + 58h]
        mov    ebx, ebp
        mov    ecx, dword ptr [eax + 4]
        add    ecx, edi
        mov    eax, dword ptr [esp + 20h]
        cmp    ebx, eax
        jbe    SHORT loc_1_1850
        mov    ebx, eax
loc_1_1850:
        push   ebx
        push   ecx
        sub    ebp, ebx
        push   dword ptr [esp + 24h]
        call   sub_1_39DA
        add    esp, 0ch
        lea    eax, [edi + ebx]
        mov    dword ptr [esi + 5ch], eax
        mov    dword ptr [esi + 58h], ebp
loc_1_1869:
        mov    eax, ebx
loc_1_186B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_1870:
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        mov    eax, dword ptr [esp + 18h]
        cmp    eax, 1
        je     SHORT loc_1_1889
        cmp    eax, 2
        je     SHORT loc_1_18D8
        jmp    NEAR PTR loc_1_1968
loc_1_1889:
        mov    esi, dword ptr [esp + 14h]
        mov    ebx, dword ptr [esi + 54h]
        mov    edi, dword ptr [esi + 40h]
        sub    edi, ebx
        or     edi, edi
        jg     SHORT loc_1_18AD
        mov    eax, dword ptr [esi + 34h]
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [esi + 34h], 1
        sub    eax, eax
        jmp    NEAR PTR loc_1_196A
loc_1_18AD:
        mov    ecx, dword ptr [esi + 3ch]
        add    ecx, ebx
        mov    ebp, dword ptr [esp + 20h]
        cmp    edi, ebp
        jb     SHORT loc_1_18BC
        mov    edi, ebp
loc_1_18BC:
        push   edi
        push   dword ptr [esp + 20h]
        push   ecx
        call   sub_1_39DA
        add    esp, 0ch
        lea    eax, [ebx + edi]
        mov    dword ptr [esi + 54h], eax
        add    dword ptr [esi + 4ch], edi
        jmp    NEAR PTR loc_1_1968
loc_1_18D8:
        mov    esi, dword ptr [esp + 14h]
        cmp    dword ptr [esi + 0f4h], 0
        jne    SHORT loc_1_18EE
        push   esi
        call   sub_1_199C
        add    esp, 4
loc_1_18EE:
        mov    ebx, dword ptr [esi + 0f4h]
        cmp    dword ptr [ebx + 4], 0
        jne    SHORT loc_1_191F
        mov    ebp, dword ptr [esp + 20h]
        push   1
        push   ebp
        db  0E8h, 012h, 0E7h, 0FEh, 0FFh
        add    esp, 8
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [ebx + 8], ebp
        mov    dword ptr [esi + 58h], 0
        mov    dword ptr [esi + 60h], 0
        jmp    SHORT loc_1_1923
loc_1_191F:
        mov    ebp, dword ptr [esp + 20h]
loc_1_1923:
        mov    edi, dword ptr [ebx + 8]
        sub    edi, dword ptr [esi + 58h]
        or     edi, edi
        jg     SHORT loc_1_193E
        mov    eax, dword ptr [esi + 34h]
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [esi + 34h], 100h
        sub    eax, eax
        jmp    SHORT loc_1_196A
loc_1_193E:
        mov    ecx, dword ptr [ebx + 4]
        add    ecx, dword ptr [esi + 60h]
        cmp    edi, ebp
        jb     SHORT loc_1_194A
        mov    edi, ebp
loc_1_194A:
        push   edi
        push   dword ptr [esp + 20h]
        push   ecx
        call   sub_1_39DA
        add    esp, 0ch
        mov    eax, dword ptr [esi + 60h]
        add    eax, edi
        mov    dword ptr [esi + 60h], eax
        mov    eax, dword ptr [esi + 58h]
        add    eax, edi
        mov    dword ptr [esi + 58h], eax
loc_1_1968:
        mov    eax, edi
loc_1_196A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_196F:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 58h], 0
        jne    SHORT loc_1_199A
        push   esi
        call   sub_1_19E0
        add    esp, 4
        mov    eax, dword ptr [esi + 0f4h]
        or     eax, eax
        je     SHORT loc_1_199A
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [esi + 58h], eax
        mov    dword ptr [esi + 5ch], 0
loc_1_199A:
        pop    esi
        ret
sub_1_199C:
        push   1
        push   0ch
        db  0E8h, 073h, 0E6h, 0FEh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_19B0
        sub    eax, eax
        jmp    SHORT loc_1_19DF
loc_1_19B0:
        mov    edx, dword ptr [esp + 4]
        mov    dword ptr [eax], 0
        mov    ecx, dword ptr [edx + 0f4h]
        or     ecx, ecx
        jne    SHORT loc_1_19CC
        mov    dword ptr [edx + 0f4h], eax
        jmp    SHORT loc_1_19DA
loc_1_19CC:
        cmp    dword ptr [ecx], 0
        je     SHORT loc_1_19D8
loc_1_19D1:
        mov    ecx, dword ptr [ecx]
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_1_19D1
loc_1_19D8:
        mov    dword ptr [ecx], eax
loc_1_19DA:
        mov    eax, 1
loc_1_19DF:
        ret
sub_1_19E0:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    esi, dword ptr [edi + 0f4h]
        or     esi, esi
        je     SHORT loc_1_1A1E
        cmp    byte ptr [edi + 16ch], 0
        jne    SHORT loc_1_1A0B
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_1_1A0B
        push   0
        push   eax
        db  0E8h, 018h, 0E6h, 0FEh, 0FFh
        add    esp, 8
loc_1_1A0B:
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi + 0f4h], eax
        push   0
        push   esi
        db  0E8h, 005h, 0E6h, 0FEh, 0FFh
        add    esp, 8
loc_1_1A1E:
        pop    edi
        pop    esi
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_1A30:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esi + 134h]
        cmp    word ptr [eax + 0ah], 800h
        jne    SHORT loc_1_1A4E
        mov    eax, 1
        jmp    NEAR PTR loc_1_1AF1
loc_1_1A4E:
        mov    ecx, dword ptr [esi + 138h]
        mov    eax, dword ptr [esi + 58h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esi + 0f4h]
        push   ecx
        mov    eax, dword ptr [eax + 4]
        add    eax, dword ptr [esi + 5ch]
        mov    dword ptr [ecx + 4], eax
        mov    word ptr [ecx + 8], 0
        mov    word ptr [ecx + 0ch], 11h
        mov    word ptr [ecx + 0eh], 0
        call   sub_1_38C1
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_1AEF
        mov    eax, dword ptr [esi + 138h]
        sub    edi, edi
        cmp    word ptr [eax + 0eh], di
        jbe    SHORT loc_1_1AC2
loc_1_1A95:
        mov    ax, word ptr [esi + 120h]
        push   eax
        push   dword ptr [esi + 138h]
        call   sub_1_3916
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_1AE1
        mov    eax, dword ptr [esi + 138h]
        inc    edi
        inc    word ptr [eax + 0ch]
        movzx  eax, word ptr [eax + 0eh]
        cmp    eax, edi
        jg     SHORT loc_1_1A95
loc_1_1AC2:
        mov    eax, dword ptr [esi + 138h]
        mov    word ptr [eax + 0ch], 0
        mov    eax, dword ptr [esi + 134h]
        mov    word ptr [eax + 0ah], 800h
        mov    eax, 1
        jmp    SHORT loc_1_1AF1
loc_1_1AE1:
        push   dword ptr [esi + 138h]
        call   sub_1_38F3
        add    esp, 4
loc_1_1AEF:
        sub    eax, eax
loc_1_1AF1:
        pop    edi
        pop    esi
        ret
sub_1_1AF4:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esi + 134h]
        push   eax
        mov    dword ptr [eax], 1000h
        mov    word ptr [eax + 0ah], 0
        push   dword ptr [esi + 130h]
        call   sub_1_3774
        add    esp, 8
        mov    eax, dword ptr [esi + 134h]
        cmp    word ptr [eax + 0ah], 0
        je     SHORT loc_1_1B34
        lea    edi, [esi + 148h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [edi], eax
        jmp    SHORT loc_1_1B6D
loc_1_1B34:
        mov    dword ptr [eax], 1000h
        push   0fh
        push   1
        push   eax
        call   sub_1_37D2
        add    esp, 0ch
        mov    dword ptr [esi + 144h], eax
        mov    eax, dword ptr [esi + 134h]
        lea    edi, [esi + 148h]
        cmp    word ptr [eax + 0ah], 8000h
        jne    SHORT loc_1_1B67
        mov    eax, dword ptr [eax]
        mov    dword ptr [edi], eax
        jmp    SHORT loc_1_1B6D
loc_1_1B67:
        mov    dword ptr [edi], 0
loc_1_1B6D:
        mov    eax, dword ptr [esi + 134h]
        push   eax
        mov    cx, word ptr [eax + 0ah]
        mov    word ptr [esi + 15ch], cx
        call   sub_1_384A
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_1B91
        mov    dword ptr [edi], 0
loc_1_1B91:
        mov    eax, 1
        pop    edi
        pop    esi
        ret
sub_1_1B99:
        push   esi
        push   edi
        movzx  esi, byte ptr [esp + 10h]
        cmp    esi, 1
        je     SHORT loc_1_1BB1
        cmp    esi, 2
        je     SHORT loc_1_1BC3
        cmp    esi, 4
        je     SHORT loc_1_1BD5
        jmp    SHORT loc_1_1C05
loc_1_1BB1:
        mov    edi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [edi + 0dch]
        mov    ecx, dword ptr [edi + 0cch]
        jmp    SHORT loc_1_1BE5
loc_1_1BC3:
        mov    edi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [edi + 0d8h]
        mov    ecx, dword ptr [edi + 0c8h]
        jmp    SHORT loc_1_1BE5
loc_1_1BD5:
        mov    edi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [edi + 0d4h]
        mov    ecx, dword ptr [edi + 0c4h]
loc_1_1BE5:
        or     ecx, ecx
        je     SHORT loc_1_1BF3
        push   0
        push   esi
        push   eax
        push   edi
        call   ecx
        add    esp, 10h
loc_1_1BF3:
        not    esi
        and    dword ptr [edi + 150h], esi
        mov    dword ptr [edi + 160h], 0
loc_1_1C05:
        pop    edi
        pop    esi
        ret
sub_1_1C08:
        push   esi
        mov    esi, dword ptr [esp + 8]
        test   byte ptr [esi + 150h], 40h
        je     SHORT loc_1_1C3F
        mov    ecx, dword ptr [esi + 30h]
        mov    eax, dword ptr [esp + 0ch]
        or     dword ptr [ecx], eax
        test   dword ptr [esi + 0e0h], eax
        je     SHORT loc_1_1C32
        push   4
        push   esi
        call   sub_1_1B99
        add    esp, 8
loc_1_1C32:
        mov    eax, dword ptr [esi + 34h]
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [esi + 34h], 0
loc_1_1C3F:
        pop    esi
        ret
sub_1_1C41:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        push   esi
        call   sub_1_199C
        add    esp, 4
        mov    ebp, dword ptr [esi + 0f4h]
        mov    ebx, ebp
        cmp    dword ptr [ebx], 0
        je     SHORT loc_1_1C66
loc_1_1C5F:
        mov    ebx, dword ptr [ebx]
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_1_1C5F
loc_1_1C66:
        mov    edi, dword ptr [esp + 1ch]
        mov    dword ptr [ebx + 8], edi
        cmp    byte ptr [esi + 16ch], 0
        jne    SHORT loc_1_1C94
        push   1
        push   edi
        db  0E8h, 09Ah, 0E3h, 0FEh, 0FFh
        add    esp, 8
        push   edi
        mov    dword ptr [ebx + 4], eax
        push   dword ptr [esp + 1ch]
        push   eax
        call   sub_1_39DA
        add    esp, 0ch
        jmp    SHORT loc_1_1C9B
loc_1_1C94:
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [ebx + 4], eax
loc_1_1C9B:
        cmp    dword ptr [ebp], 0
        jne    SHORT loc_1_1CAB
        mov    dword ptr [esi + 5ch], 0
        mov    dword ptr [esi + 58h], edi
loc_1_1CAB:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_1CB2:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_1_1C41
        mov    ecx, dword ptr [ebp + 14h]
        add    esp, 0ch
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 13ch], 0
        jne    SHORT loc_1_1D20
        cmp    dword ptr [eax + 158h], 0
        jne    SHORT loc_1_1D16
        cmp    dword ptr [eax + 148h], 0
        je     SHORT loc_1_1D16
        mov    ecx, eax
        mov    eax, dword ptr [eax + 154h]
        cmp    dword ptr [ecx + 58h], eax
        jbe    SHORT loc_1_1D16
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 130h], 0
        je     SHORT loc_1_1D16
        cmp    dword ptr [eax + 0f8h], 0
        je     SHORT loc_1_1D16
        push   edx
        mov    edx, dword ptr [ebp + 8]
        call   sub_1_2A16
        jmp    SHORT loc_1_1D1F
loc_1_1D16:
        push   edx
        mov    edx, dword ptr [ebp + 8]
        call   sub_1_2FAF
loc_1_1D1F:
        pop    edx
loc_1_1D20:
        pop    ebp
        ret
loc_1_1D22:
        mov    ecx, dword ptr [esp + 4]
        push   dword ptr [esp + 10h]
        mov    byte ptr [ecx + 0b2h], 10h
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   ecx
        call   sub_1_1CB2
        add    esp, 10h
        mov    eax, 1
        ret
loc_1_1D48:
        mov    ecx, dword ptr [esp + 4]
        push   dword ptr [esp + 10h]
        mov    byte ptr [ecx + 0b2h], 40h
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   ecx
        call   sub_1_1CB2
        add    esp, 10h
        mov    eax, 1
        ret
loc_1_1D6E:
        push   esi
        push   dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 0ch]
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_1_1C41
        mov    ecx, dword ptr [esp + 20h]
        add    esp, 0ch
        mov    dword ptr [ecx], eax
        cmp    dword ptr [esi + 13ch], 0
        jne    SHORT loc_1_1DE5
        test   byte ptr [esi + 3bh], 1
        je     SHORT loc_1_1DA6
        push   esi
        db  0E8h, 0F5h, 0E5h, 0FEh, 0FFh
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_1DCC
loc_1_1DA6:
        or     dword ptr [esi + 168h], 80h
        push   esi
        call   sub_1_01B8
        add    esp, 4
        push   esi
        call   sub_1_209D
        add    esp, 4
        xor    dword ptr [esi + 168h], 80h
loc_1_1DCC:
        cmp    dword ptr [esi + 58h], 0
        jle    SHORT loc_1_1DE5
        push   esi
        push   0ah
        db  068h, 023h, 021h, 000h, 000h
        db  0E8h, 061h, 0E2h, 0FEh, 0FFh
        mov    dword ptr [esi + 13ch], eax
loc_1_1DE5:
        mov    eax, 1
        pop    esi
        ret
loc_1_1DEC:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 148h], 0
        je     SHORT loc_1_1E21
        mov    eax, dword ptr [esi + 154h]
        cmp    dword ptr [esi + 40h], eax
        jbe    SHORT loc_1_1E21
        cmp    dword ptr [esi + 130h], 0
        je     SHORT loc_1_1E21
        cmp    dword ptr [esi + 0f8h], 0
        je     SHORT loc_1_1E21
        push   esi
        push   0ah
        db  068h, 02Ah, 02Ch, 000h, 000h
        jmp    SHORT loc_1_1E29
loc_1_1E21:
        push   esi
        push   0ah
        db  068h, 05Ah, 036h, 000h, 000h
loc_1_1E29:
        db  0E8h, 012h, 0E2h, 0FEh, 0FFh
        mov    dword ptr [esi + 140h], eax
        pop    esi
        ret
loc_1_1E36:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 140h], 0
        jne    SHORT loc_1_1E57
        push   esi
        push   0ah
        db  068h, 0B0h, 00Bh, 000h, 000h
        db  0E8h, 0EFh, 0E1h, 0FEh, 0FFh
        mov    dword ptr [esi + 140h], eax
loc_1_1E57:
        pop    esi
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_1E60:
        mov    al, byte ptr [esp + 4]
        cmp    al, 30h
        jne    SHORT loc_1_1E6F
        mov    eax, 800h
        jmp    SHORT loc_1_1EA4
loc_1_1E6F:
        test   al, 20h
        je     SHORT loc_1_1E7A
        mov    eax, 1000h
        jmp    SHORT loc_1_1EA4
loc_1_1E7A:
        test   al, 8
        je     SHORT loc_1_1E85
        mov    eax, 400h
        jmp    SHORT loc_1_1EA4
loc_1_1E85:
        movzx  ecx, al
        not    ecx
        test   cl, 10h
        je     SHORT loc_1_1E96
        mov    eax, 800h
        jmp    SHORT loc_1_1EA4
loc_1_1E96:
        test   al, 80h
        mov    eax, 0
        jne    SHORT loc_1_1EA4
        mov    eax, 200h
loc_1_1EA4:
        ret
sub_1_1EA5:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   dword ptr [esp + 0ch]
        test   byte ptr [esi + 17ch], 1
        je     SHORT loc_1_1EBE
        call   sub_1_3A3C
        jmp    SHORT loc_1_1EC3
loc_1_1EBE:
        call   sub_1_3A12
loc_1_1EC3:
        add    esp, 4
        or     al, al
        mov    eax, 1
        je     SHORT loc_1_1EF8
        mov    eax, dword ptr [esi + 34h]
        mov    dword ptr [esi + 0ch], eax
        movzx  eax, word ptr [esi + 94h]
        inc    eax
        push   eax
        call   sub_1_3E90
        add    esp, 4
        and    al, 0f8h
        xor    al, 48h
        push   eax
        call   sub_1_1E60
        add    esp, 4
        mov    dword ptr [esi + 34h], eax
        sub    eax, eax
loc_1_1EF8:
        pop    esi
        ret
sub_1_1EFA:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        movzx  eax, word ptr [esi + 94h]
        mov    dword ptr [ebp - 8], eax
        mov    edi, eax
        lea    edi, [edi + 1]
        mov    byte ptr [esi + 16fh], 0
        pushfd
        cli
        mov    dword ptr [ebp - 4], 0c8h
loc_1_1F25:
        push   edi
        push   esi
        call   sub_1_1EA5
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_1_1FB7
        mov    eax, dword ptr [ebp + 0ch]
        inc    dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax]
        push   eax
        push   dword ptr [ebp - 8]
        call   sub_1_39EF
        add    esp, 8
        cmp    byte ptr [esi + 16fh], 0
        je     SHORT loc_1_1F67
        mov    bl, byte ptr [esi + 16fh]
loc_1_1F5A:
        push   edi
        call   sub_1_3E90
        add    esp, 4
        dec    bl
        jne    SHORT loc_1_1F5A
loc_1_1F67:
        sub    bl, bl
loc_1_1F69:
        push   edi
        call   sub_1_3E90
        add    esp, 4
        test   al, 80h
        je     SHORT loc_1_1F7D
        inc    bl
        cmp    bl, 14h
        jb     SHORT loc_1_1F69
loc_1_1F7D:
        movzx  eax, bl
        or     eax, eax
        je     SHORT loc_1_1FA9
        cmp    eax, 14h
        je     SHORT loc_1_1FA9
        mov    al, byte ptr [esi + 16fh]
        add    al, bl
        mov    byte ptr [esi + 16fh], al
        cmp    al, 14h
        jbe    SHORT loc_1_1FA9
        mov    byte ptr [esi + 16fh], 14h
        mov    dword ptr [ebp - 4], 0
loc_1_1FA9:
        mov    eax, dword ptr [esi + 58h]
        dec    eax
        mov    dword ptr [esi + 58h], eax
        inc    dword ptr [esi + 5ch]
        or     eax, eax
        je     SHORT loc_1_1FD1
loc_1_1FB7:
        dec    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 0
        jg     NEAR PTR loc_1_1F25
        popfd
        mov    eax, 1
loc_1_1FCA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_1FD1:
        popfd
        mov    eax, 1
        jmp    SHORT loc_1_1FCA
sub_1_1FD9:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 1ch]
        movzx  eax, word ptr [esi + 94h]
        mov    edi, eax
        mov    dword ptr [esp + 14h], eax
        inc    edi
        push   edi
        push   esi
        call   sub_1_1EA5
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_1_2090
        cmp    byte ptr [esi + 16fh], 0ffh
        jne    SHORT loc_1_201C
        push   dword ptr [esp + 20h]
        push   esi
        call   sub_1_1EFA
        add    esp, 8
        jmp    SHORT loc_1_2095
loc_1_201C:
        mov    dword ptr [esp + 10h], 800h
loc_1_2024:
        push   edi
        push   esi
        call   sub_1_1EA5
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_2085
        mov    eax, dword ptr [esp + 20h]
        inc    dword ptr [esp + 20h]
        mov    al, byte ptr [eax]
        push   eax
        push   dword ptr [esp + 18h]
        call   sub_1_39EF
        add    esp, 8
        mov    ebp, dword ptr [esi + 58h]
        dec    ebp
        mov    dword ptr [esi + 58h], ebp
        inc    dword ptr [esi + 5ch]
        cmp    byte ptr [esi + 16fh], 0
        je     SHORT loc_1_206F
        movzx  ebx, byte ptr [esi + 16fh]
loc_1_2063:
        push   edi
        call   sub_1_3E90
        add    esp, 4
        dec    ebx
        jne    SHORT loc_1_2063
loc_1_206F:
        or     ebp, ebp
        je     SHORT loc_1_2090
        movzx  eax, byte ptr [esi + 16fh]
        db  00Fh, 0B6h, 080h, 0E0h, 03Fh, 000h, 000h
        sub    dword ptr [esp + 10h], eax
loc_1_2085:
        dec    dword ptr [esp + 10h]
        cmp    dword ptr [esp + 10h], 0
        jg     SHORT loc_1_2024
loc_1_2090:
        mov    eax, 1
loc_1_2095:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_209D:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 58h], 0
        jne    SHORT loc_1_20B1
        mov    dword ptr [esi + 34h], 0
        jmp    SHORT loc_1_2121
loc_1_20B1:
        mov    eax, dword ptr [esi + 0f4h]
        mov    eax, dword ptr [eax + 4]
        add    eax, dword ptr [esi + 5ch]
        mov    dword ptr [esi + 160h], 1
        db  0C7h, 086h, 0BCh, 000h, 000h, 000h, 02Eh, 017h, 000h, 000h
        mov    dword ptr [esi + 14ch], 20h
        push   eax
        push   esi
        call   sub_1_1FD9
        add    esp, 8
        mov    eax, dword ptr [esi + 58h]
        cmp    dword ptr [esi + 0ech], eax
        jg     SHORT loc_1_20F4
        or     eax, eax
        jne    SHORT loc_1_2111
loc_1_20F4:
        push   esi
        call   sub_1_196F
        add    esp, 4
        test   byte ptr [esi + 150h], 2
        je     SHORT loc_1_2111
        push   2
        push   esi
        call   sub_1_1B99
        add    esp, 8
loc_1_2111:
        cmp    dword ptr [esi + 58h], 0
        jne    SHORT loc_1_2121
        mov    dword ptr [esi + 160h], 0
loc_1_2121:
        pop    esi
        ret
loc_1_2123:
        sub    esp, 4
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        test   byte ptr [eax + 3bh], 1
        je     SHORT loc_1_2141
        push   eax
        db  0E8h, 05Ah, 0E2h, 0FEh, 0FFh
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_2175
loc_1_2141:
        mov    eax, dword ptr [esp]
        or     dword ptr [eax + 168h], 80h
        push   dword ptr [esp]
        call   sub_1_01B8
        add    esp, 4
        push   dword ptr [esp]
        call   sub_1_209D
        mov    eax, dword ptr [esp + 4]
        add    esp, 4
        xor    dword ptr [eax + 168h], 80h
loc_1_2175:
        mov    eax, dword ptr [esp]
        cmp    dword ptr [eax + 58h], 0
        jle    SHORT loc_1_2198
        push   eax
        push   0ah
        db  068h, 023h, 021h, 000h, 000h
        db  0E8h, 0B4h, 0DEh, 0FEh, 0FFh
        mov    ecx, dword ptr [esp]
        mov    dword ptr [ecx + 13ch], eax
        jmp    SHORT loc_1_21A6
loc_1_2198:
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 13ch], 0
loc_1_21A6:
        add    esp, 4
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h
loc_1_21B0:
        push   esi
        mov    eax, dword ptr [esp + 0ch]
        cmp    eax, 1
        jne    SHORT loc_1_21C6
        mov    esi, dword ptr [esp + 8]
        and    byte ptr [esi + 3bh], 0feh
loc_1_21C2:
        sub    eax, eax
loc_1_21C4:
        pop    esi
        ret
loc_1_21C6:
        or     eax, eax
        jne    SHORT loc_1_21C2
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 168h], 1
        jne    SHORT loc_1_21C2
        cmp    dword ptr [esi + 160h], 0
        jne    SHORT loc_1_21C2
        cmp    byte ptr [esi + 0b0h], 0ffh
        jne    SHORT loc_1_21C2
        cmp    byte ptr [esi + 0b2h], 0
        jne    SHORT loc_1_21FB
        push   esi
        call   sub_1_01B8
        add    esp, 4
loc_1_21FB:
        or     byte ptr [esi + 3bh], 1
        mov    eax, 1
        jmp    SHORT loc_1_21C4
sub_1_2206:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        jae    SHORT loc_1_2217
        xor    eax, eax
loc_1_2217:
        pop    edi
        pop    ebp
        ret
loc_1_221A:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0ch]
        or     eax, eax
        je     SHORT loc_1_223A
        cmp    eax, 1
        je     SHORT loc_1_2274
        mov    dword ptr [esi + 0ch], 0ffffffebh
        sub    eax, eax
        jmp    NEAR PTR loc_1_22C1
loc_1_223A:
        mov    dword ptr [esi + 58h], 0
        mov    dword ptr [esi + 5ch], 0
        mov    dword ptr [esi + 60h], 0
        mov    eax, dword ptr [esi + 13ch]
        or     eax, eax
        je     SHORT loc_1_2269
        push   eax
        db  0E8h, 0FDh, 0DDh, 0FEh, 0FFh
        mov    dword ptr [esi + 13ch], 0
loc_1_2269:
        push   esi
        call   sub_1_19E0
        add    esp, 4
        jmp    SHORT loc_1_22A3
loc_1_2274:
        mov    dword ptr [esi + 4ch], 0
        mov    dword ptr [esi + 50h], 0
        mov    dword ptr [esi + 54h], 0
        mov    eax, dword ptr [esi + 140h]
        or     eax, eax
        je     SHORT loc_1_22A3
        push   eax
        db  0E8h, 0C3h, 0DDh, 0FEh, 0FFh
        mov    dword ptr [esi + 140h], 0
loc_1_22A3:
        cmp    dword ptr [esi + 160h], 1
        cmc
        sbb    eax, eax
        and    eax, 200h
        mov    dword ptr [esi + 34h], eax
        mov    dword ptr [esi + 0ch], 0
        mov    eax, 1
loc_1_22C1:
        pop    esi
        ret
sub_1_22C3:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 10h]
        or     esi, esi
        je     SHORT loc_1_22F7
        mov    dword ptr [esi], 0
        cmp    dword ptr [edi + 160h], 0
        je     SHORT loc_1_22EB
        sti
        push   edi
        call   dword ptr [edi + 0bch]
        add    esp, 4
loc_1_22EB:
        mov    eax, dword ptr [edi + 4ch]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edi + 58h]
        mov    dword ptr [esi + 8], eax
loc_1_22F7:
        mov    dword ptr [edi + 0ch], 0
        mov    eax, 1
        pop    edi
        pop    esi
        ret
sub_1_2306:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    word ptr [esp + 0eh], 0
        mov    esi, dword ptr [esp + 18h]
        mov    dword ptr [esp + 10h], 0
        test   byte ptr [esi + 3bh], 1
        je     SHORT loc_1_2341
        push   esi
        db  0E8h, 069h, 0E0h, 0FEh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_2341
        mov    dword ptr [esi + 0e8h], 0
        jmp    NEAR PTR loc_1_2485
loc_1_2341:
        or     dword ptr [esi + 168h], 80h
        mov    bl, byte ptr [esi + 0b2h]
        push   esi
        call   sub_1_01B8
        add    esp, 4
        push   4
        push   esi
        call   sub_1_03B7
        add    esp, 8
        cmp    eax, 10000h
        je     SHORT loc_1_237B
        mov    dword ptr [esi + 0e8h], 0
        jmp    NEAR PTR loc_1_247B
loc_1_237B:
        mov    di, word ptr [esi + 94h]
        push   23h
        push   0
        push   8
        movzx  eax, di
        inc    eax
        push   eax
        call   sub_1_3E38
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_23B1
        push   esi
        call   sub_1_01B8
        add    esp, 4
        mov    dword ptr [esi + 0e8h], 0
        jmp    NEAR PTR loc_1_247B
loc_1_23B1:
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 12h]
        push   2
        push   eax
        push   edi
        call   sub_1_3CB3
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_23E2
        push   esi
        call   sub_1_01B8
        add    esp, 4
        mov    dword ptr [esi + 0e8h], 0
        jmp    NEAR PTR loc_1_247B
loc_1_23E2:
        mov    ah, byte ptr [esp + 0eh]
        mov    al, byte ptr [esp + 0fh]
        mov    word ptr [esp + 0eh], ax
        cmp    ax, 2
        ja     SHORT loc_1_240A
        push   esi
        call   sub_1_01B8
        add    esp, 4
        mov    dword ptr [esi + 0e8h], 0
        jmp    SHORT loc_1_247B
loc_1_240A:
        sub    word ptr [esp + 0eh], 2
        mov    eax, dword ptr [esi + 0e8h]
        movzx  ecx, word ptr [esp + 0eh]
        cmp    ecx, eax
        jbe    SHORT loc_1_2424
        mov    word ptr [esp + 0eh], ax
loc_1_2424:
        lea    eax, [esp + 10h]
        push   eax
        movzx  eax, word ptr [esp + 12h]
        push   eax
        push   dword ptr [esi + 0e4h]
        mov    ax, word ptr [esi + 94h]
        push   eax
        call   sub_1_3CB3
        mov    eax, dword ptr [esp + 20h]
        add    esp, 10h
        mov    dword ptr [esi + 0e8h], eax
        push   esi
        call   sub_1_01B8
        add    esp, 4
        movzx  eax, bl
        or     eax, eax
        je     SHORT loc_1_2466
        cmp    eax, 10h
        je     SHORT loc_1_2470
        jmp    SHORT loc_1_247B
loc_1_2466:
        push   0
        push   esi
        call   sub_1_03B7
        jmp    SHORT loc_1_2478
loc_1_2470:
        push   10h
        push   esi
        call   sub_1_0287
loc_1_2478:
        add    esp, 8
loc_1_247B:
        xor    dword ptr [esi + 168h], 80h
loc_1_2485:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_248C:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 14h]
        cmp    eax, 7
        je     SHORT loc_1_24BB
        cmp    eax, 11h
        je     SHORT loc_1_24F4
        cmp    eax, 12h
        je     NEAR PTR loc_1_2535
        mov    dword ptr [esi + 0ch], 0ffffffech
        mov    eax, 1
        jmp    NEAR PTR loc_1_262D
loc_1_24BB:
        test   byte ptr [esi + 3bh], 1
        je     SHORT loc_1_24D3
        push   esi
        db  0E8h, 0CDh, 0DEh, 0FEh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_24D3
        jmp    NEAR PTR loc_1_262B
loc_1_24D3:
        or     dword ptr [esi + 168h], 80h
        push   esi
        call   sub_1_01B8
        add    esp, 4
        push   esi
        call   sub_1_0AC7
        add    esp, 4
        jmp    NEAR PTR loc_1_2601
loc_1_24F4:
        cmp    byte ptr [esi + 0b1h], 0ffh
        mov    edi, dword ptr [esp + 1ch]
        jne    SHORT loc_1_250C
        mov    dword ptr [edi], 0
        jmp    NEAR PTR loc_1_260B
loc_1_250C:
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esi + 0e4h], eax
        mov    ebx, dword ptr [esp + 18h]
        mov    dword ptr [esi + 0e8h], ebx
        push   esi
        call   sub_1_2306
        add    esp, 4
        mov    eax, dword ptr [esi + 0e8h]
        mov    dword ptr [edi], eax
        jmp    NEAR PTR loc_1_260B
loc_1_2535:
        test   byte ptr [esi + 3bh], 1
        je     SHORT loc_1_254D
        push   esi
        db  0E8h, 053h, 0DEh, 0FEh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_254D
        jmp    NEAR PTR loc_1_262B
loc_1_254D:
        mov    eax, dword ptr [esi + 168h]
        or     eax, 80h
        mov    dword ptr [esi + 168h], eax
        mov    ebx, dword ptr [esp + 18h]
        cmp    bl, 7fh
        jbe    SHORT loc_1_2573
        mov    dword ptr [esi + 0ch], 0ffffffebh
        jmp    NEAR PTR loc_1_2620
loc_1_2573:
        cmp    word ptr [esi + 98h], 2
        jne    NEAR PTR loc_1_2619
        mov    cl, byte ptr [esi + 0b3h]
        test   cl, 4
        je     SHORT loc_1_2599
        cmp    dword ptr [esi + 160h], 1
        je     NEAR PTR loc_1_2619
loc_1_2599:
        cmp    cl, 23h
        je     SHORT loc_1_25BC
        push   10h
        push   esi
        call   sub_1_0287
        add    esp, 8
        cmp    eax, 10000h
        je     SHORT loc_1_25BC
        xor    dword ptr [esi + 168h], 80h
        jmp    SHORT loc_1_262B
loc_1_25BC:
        movzx  edi, word ptr [esi + 94h]
        push   64h
        lea    eax, [edi + 402h]
        push   eax
        call   sub_1_051E
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_25F1
        mov    eax, dword ptr [esi + 34h]
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [esi + 34h], 100h
        xor    dword ptr [esi + 168h], 80h
        jmp    SHORT loc_1_262B
loc_1_25F1:
        or     ebx, 80h
        push   ebx
        push   edi
        call   sub_1_3F2E
        add    esp, 8
loc_1_2601:
        xor    dword ptr [esi + 168h], 80h
loc_1_260B:
        mov    dword ptr [esi + 0ch], 0
        mov    eax, 1
        jmp    SHORT loc_1_262D
loc_1_2619:
        mov    dword ptr [esi + 0ch], 0fffffff6h
loc_1_2620:
        xor    eax, 80h
        mov    dword ptr [esi + 168h], eax
loc_1_262B:
        sub    eax, eax
loc_1_262D:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_2631:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        cmp    dword ptr [esp + 24h], 0
        je     NEAR PTR loc_1_2726
        mov    edi, dword ptr [esp + 28h]
        or     edi, edi
        je     NEAR PTR loc_1_2726
        mov    eax, dword ptr [esp + 10h]
        cmp    eax, 7
        je     SHORT loc_1_2673
        cmp    eax, 11h
        je     SHORT loc_1_26B3
        cmp    eax, 12h
        je     NEAR PTR loc_1_26FB
        cmp    eax, 16h
        je     NEAR PTR loc_1_2718
        jmp    NEAR PTR loc_1_2726
loc_1_2673:
        test   byte ptr [esi + 3bh], 1
        je     SHORT loc_1_268B
        push   esi
        db  0E8h, 015h, 0DDh, 0FEh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_268B
        jmp    NEAR PTR loc_1_2726
loc_1_268B:
        or     dword ptr [esi + 168h], 80h
        push   esi
        call   sub_1_01B8
        add    esp, 4
        push   esi
        call   sub_1_0AC7
        add    esp, 4
        xor    dword ptr [esi + 168h], 80h
        jmp    SHORT loc_1_271F
loc_1_26B3:
        cmp    byte ptr [esi + 0b1h], 0ffh
        jne    SHORT loc_1_26CE
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [edi + 10h], 0
        mov    dword ptr [eax], 0
        jmp    SHORT loc_1_271F
loc_1_26CE:
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [esi + 0e4h], eax
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [esi + 0e8h], eax
        push   esi
        call   sub_1_2306
        add    esp, 4
        mov    eax, dword ptr [esi + 0e8h]
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [edi + 10h], eax
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_271F
loc_1_26FB:
        mov    eax, dword ptr [esp + 14h]
        push   0
        movsx  eax, byte ptr [eax]
        push   eax
        push   12h
        push   esi
        call   sub_1_248C
        add    esp, 10h
        cmp    eax, 1
        cmc
        sbb    eax, eax
        jmp    SHORT loc_1_2728
loc_1_2718:
        or     dword ptr [esi + 17ch], 1
loc_1_271F:
        mov    eax, 0ffffffffh
        jmp    SHORT loc_1_2728
loc_1_2726:
        sub    eax, eax
loc_1_2728:
        pop    edi
        pop    esi
        ret
loc_1_272B:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   dword ptr [esp + 0ch]
        push   esi
        call   sub_1_22C3
        add    esp, 8
        or     eax, eax
        mov    eax, 0
        je     SHORT loc_1_276B
        push   esi
        db  0E8h, 0B3h, 0DDh, 0FEh, 0FFh
        add    esp, 4
        mov    ecx, dword ptr [esi + 34h]
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [esi + 34h], 0
        mov    eax, 1
        mov    dword ptr [esi + 0ch], 0
loc_1_276B:
        pop    esi
        ret
loc_1_276D:
        push   esi
        mov    esi, dword ptr [esp + 8]
        movzx  eax, word ptr [esi + 98h]
        cmp    eax, 1
        je     SHORT loc_1_2791
        cmp    eax, 2
        je     SHORT loc_1_27F4
        mov    dword ptr [esi + 0ch], 0ffffffebh
        sub    eax, eax
        jmp    NEAR PTR loc_1_281E
loc_1_2791:
        test   byte ptr [esi + 3bh], 1
        je     SHORT loc_1_27A8
        push   esi
        db  0E8h, 0F7h, 0DBh, 0FEh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_27A8
        sub    eax, eax
        jmp    SHORT loc_1_281E
loc_1_27A8:
        or     dword ptr [esi + 168h], 80h
        cmp    byte ptr [esi + 0b2h], 0
        jne    SHORT loc_1_27C4
        push   esi
        call   sub_1_01B8
        add    esp, 4
loc_1_27C4:
        mov    dword ptr [esi + 58h], 1
        lea    eax, [esp + 0ch]
        push   eax
        push   esi
        call   sub_1_1FD9
        add    esp, 8
        xor    dword ptr [esi + 168h], 80h
        cmp    dword ptr [esi + 58h], 0
        je     SHORT loc_1_2810
        mov    dword ptr [esi + 58h], 0
        sub    eax, eax
        jmp    SHORT loc_1_2810
loc_1_27F4:
        cmp    byte ptr [esi + 0b1h], 10h
        push   dword ptr [esp + 0ch]
        jne    SHORT loc_1_2805
        push   74h
        jmp    SHORT loc_1_2807
loc_1_2805:
        push   54h
loc_1_2807:
        push   esi
        call   sub_1_30ED
        add    esp, 0ch
loc_1_2810:
        cmp    eax, 1
        sbb    ecx, ecx
        and    ecx, 4000h
        mov    dword ptr [esi + 0ch], ecx
loc_1_281E:
        pop    esi
        ret
loc_1_2820:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 10h]
        or     eax, eax
        mov    dword ptr [esi + 0ch], 0
        je     SHORT loc_1_2859
        mov    ecx, dword ptr [esp + 14h]
        or     ecx, ecx
        je     SHORT loc_1_2859
        mov    edi, dword ptr [esi + 0b4h]
        or     edi, edi
        je     SHORT loc_1_2855
        push   dword ptr [esp + 18h]
        push   ecx
        push   eax
        push   esi
        call   edi
        add    esp, 10h
        jmp    SHORT loc_1_285E
loc_1_2855:
        sub    eax, eax
        jmp    SHORT loc_1_285E
loc_1_2859:
        mov    eax, 1
loc_1_285E:
        pop    edi
        pop    esi
        ret
loc_1_2861:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 160h], 0
        je     SHORT loc_1_2882
        test   byte ptr [esi + 14ch], 1ah
        je     SHORT loc_1_2882
        push   esi
        call   dword ptr [esi + 0bch]
        add    esp, 4
loc_1_2882:
        mov    eax, dword ptr [esi + 140h]
        or     eax, eax
        je     SHORT loc_1_289C
        push   eax
        db  0E8h, 0CAh, 0D7h, 0FEh, 0FFh
        mov    dword ptr [esi + 140h], 0
loc_1_289C:
        cmp    byte ptr [esi + 16dh], 0
        je     SHORT loc_1_28AA
        mov    eax, dword ptr [esi + 4ch]
        jmp    SHORT loc_1_28BD
loc_1_28AA:
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   1
        push   esi
        call   sub_1_17CB
        add    esp, 10h
loc_1_28BD:
        mov    ecx, dword ptr [esp + 14h]
        pop    esi
        mov    dword ptr [ecx], eax
        mov    eax, 1
        ret
sub_1_28CA:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0ch], 0
        mov    eax, 1
        ret
loc_1_28DB:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        db  0E8h, 019h, 0DCh, 0FEh, 0FFh
        add    esp, 4
        mov    eax, dword ptr [esi + 34h]
        cmp    eax, 200h
        je     SHORT loc_1_2914
        cmp    eax, 400h
        je     SHORT loc_1_2920
        cmp    eax, 800h
        je     SHORT loc_1_292C
        cmp    eax, 1000h
        mov    eax, dword ptr [esp + 0ch]
        je     SHORT loc_1_2938
        mov    dword ptr [eax], 0
        jmp    SHORT loc_1_293E
loc_1_2914:
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], 461h
        jmp    SHORT loc_1_293E
loc_1_2920:
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], 45dh
        jmp    SHORT loc_1_293E
loc_1_292C:
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], 15h
        jmp    SHORT loc_1_293E
loc_1_2938:
        mov    dword ptr [eax], 1ch
loc_1_293E:
        mov    eax, 1
        pop    esi
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_2950:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        movzx  edi, word ptr [esi + 94h]
        mov    eax, dword ptr [esi + 134h]
        add    edi, 402h
        cmp    word ptr [eax + 0ah], 800h
        jne    SHORT loc_1_299E
        lea    ebp, [esi + 130h]
        push   dword ptr [esi + 138h]
        push   dword ptr [ebp]
        call   sub_1_3968
        add    esp, 8
        mov    ebx, eax
        mov    eax, dword ptr [esi + 138h]
        inc    word ptr [eax + 0ch]
        sub    dword ptr [esi + 58h], ebx
        add    dword ptr [esi + 5ch], ebx
        jmp    SHORT loc_1_29C2
loc_1_299E:
        push   esi
        call   sub_1_1780
        add    esp, 4
        mov    ebx, eax
        lea    ebp, [esi + 130h]
        push   8
        push   dword ptr [esi + 134h]
        push   dword ptr [ebp]
        call   sub_1_3865
        add    esp, 0ch
loc_1_29C2:
        mov    dword ptr [esi + 158h], ebx
        mov    al, byte ptr [esi + 16eh]
        test   al, 80h
        mov    bl, byte ptr [esp + 18h]
        je     SHORT loc_1_29E2
        cmp    bl, 74h
        jne    SHORT loc_1_29E2
        test   al, 1
        jne    SHORT loc_1_29E2
        xor    bl, 10h
loc_1_29E2:
        or     bl, 8
        push   ebx
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        and    bl, 0fbh
        push   ebx
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        mov    dword ptr [esi + 160h], 1
        push   dword ptr [ebp]
        call   sub_1_38A3
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_2A16:
        sub    esp, 4
        push   ebx
        mov    dword ptr [esp + 4], edx
        cmp    dword ptr [esp + 4], 0
        je     NEAR PTR loc_1_2B9F
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 13ch], 0
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 160h], 0
        je     SHORT loc_1_2A63
        cmp    byte ptr [eax + 0b2h], 0
        je     SHORT loc_1_2A7E
        test   byte ptr [eax + 0b3h], 4
        jne    SHORT loc_1_2A7E
        test   byte ptr [eax + 14ch], 5
        jne    NEAR PTR loc_1_2B9F
loc_1_2A63:
        mov    eax, dword ptr [esp + 4]
        mov    cl, byte ptr [eax + 0b2h]
        or     cl, cl
        jne    SHORT loc_1_2A9D
        push   eax
        call   sub_1_01B8
        add    esp, 4
        mov    bl, 54h
        jmp    SHORT loc_1_2AEE
loc_1_2A7E:
        push   dword ptr [esp + 4]
        push   0ah
        db  068h, 016h, 02Ah, 000h, 000h
        db  0E8h, 0B2h, 0D5h, 0FEh, 0FFh
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx + 13ch], eax
        jmp    NEAR PTR loc_1_2B9F
loc_1_2A9D:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax + 0b3h], 4
        je     SHORT loc_1_2AE0
        push   eax
        call   sub_1_05C5
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_2ADC
        push   dword ptr [esp + 4]
        call   sub_1_01B8
        add    esp, 4
        push   10h
        push   dword ptr [esp + 8]
        call   sub_1_0287
        add    esp, 8
        cmp    eax, 10000h
        jne    NEAR PTR loc_1_2B9F
loc_1_2ADC:
        mov    bl, 74h
        jmp    SHORT loc_1_2AEE
loc_1_2AE0:
        sub    cl, 10h
        cmp    cl, 1
        sbb    bl, bl
        and    bl, 20h
        add    bl, 54h
loc_1_2AEE:
        mov    eax, dword ptr [esp + 4]
        cmp    word ptr [eax + 120h], 2
        jne    SHORT loc_1_2B51
        test   byte ptr [eax + 58h], 1
        je     SHORT loc_1_2B51
        mov    eax, dword ptr [eax + 0f4h]
        mov    ecx, dword ptr [eax + 4]
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 5ch]
        mov    al, byte ptr [ecx + eax]
        push   eax
        push   ebx
        push   dword ptr [esp + 0ch]
        call   sub_1_30ED
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_2B43
        push   dword ptr [esp + 4]
        push   0ah
        db  068h, 016h, 02Ah, 000h, 000h
        db  0E8h, 009h, 0D5h, 0FEh, 0FFh
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx + 13ch], eax
        jmp    SHORT loc_1_2B9F
loc_1_2B43:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 5ch]
        mov    eax, dword ptr [esp + 4]
        dec    dword ptr [eax + 58h]
loc_1_2B51:
        push   dword ptr [esp + 4]
        call   sub_1_1A30
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_2B76
        mov    eax, dword ptr [esp + 4]
        mov    cx, word ptr [eax + 15ch]
        mov    eax, dword ptr [eax + 134h]
        mov    word ptr [eax + 0ah], cx
loc_1_2B76:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        mov    dword ptr [eax + 14ch], 1
        mov    eax, dword ptr [esp + 8]
        db  0C7h, 080h, 0BCh, 000h, 000h, 000h, 08Bh, 015h, 000h, 000h
        push   dword ptr [esp + 8]
        call   sub_1_2950
        add    esp, 8
loc_1_2B9F:
        pop    ebx
        add    esp, 4
        ret
sub_1_2BA4:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        movzx  edi, word ptr [esi + 94h]
        mov    ecx, dword ptr [esi + 40h]
        sub    ecx, dword ptr [esi + 54h]
        mov    dword ptr [esi + 158h], ecx
        mov    eax, dword ptr [esi + 134h]
        add    edi, 402h
        mov    edx, dword ptr [esi + 148h]
        cmp    edx, ecx
        jb     SHORT loc_1_2BD5
        mov    edx, ecx
loc_1_2BD5:
        mov    dword ptr [eax], edx
        push   4
        push   eax
        push   dword ptr [esi + 130h]
        call   sub_1_3865
        add    esp, 0ch
        push   7ch
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   78h
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        mov    dword ptr [esi + 160h], 1
        mov    ecx, dword ptr [esi + 134h]
        mov    ax, word ptr [esi + 15ch]
        mov    word ptr [ecx + 0ah], ax
        push   dword ptr [esi + 130h]
        call   sub_1_38A3
        add    esp, 4
        pop    edi
        pop    esi
        ret
loc_1_2C2A:
        sub    esp, 4
        mov    dword ptr [esp], edx
        cmp    dword ptr [esp], 0
        je     NEAR PTR loc_1_2CCE
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 140h], 0
        mov    eax, dword ptr [esp]
        cmp    dword ptr [eax + 160h], 0
        je     SHORT loc_1_2C70
        push   eax
        push   0ah
        db  068h, 02Ah, 02Ch, 000h, 000h
        db  0E8h, 0DCh, 0D3h, 0FEh, 0FFh
        mov    ecx, dword ptr [esp]
        mov    dword ptr [ecx + 140h], eax
        jmp    SHORT loc_1_2CCE
loc_1_2C70:
        push   dword ptr [esp]
        call   sub_1_064D
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_2CCE
        mov    eax, dword ptr [esp]
        cmp    byte ptr [eax + 0b3h], 26h
        jne    SHORT loc_1_2CA6
        push   eax
        push   0ah
        db  068h, 02Ah, 02Ch, 000h, 000h
        db  0E8h, 0A6h, 0D3h, 0FEh, 0FFh
        mov    ecx, dword ptr [esp]
        mov    dword ptr [ecx + 140h], eax
        jmp    SHORT loc_1_2CCE
loc_1_2CA6:
        push   dword ptr [esp]
        call   sub_1_2BA4
        mov    eax, dword ptr [esp + 4]
        add    esp, 4
        mov    dword ptr [eax + 14ch], 2
        mov    eax, dword ptr [esp]
        db  0C7h, 080h, 0BCh, 000h, 000h, 000h, 08Bh, 015h, 000h, 000h
loc_1_2CCE:
        add    esp, 4
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_2CE0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 1ch]
        sub    esi, esi
        mov    eax, dword ptr [eax + 0f4h]
        mov    edi, dword ptr [eax + 4]
        mov    eax, dword ptr [esp + 1ch]
        add    edi, dword ptr [eax + 5ch]
        test   byte ptr [esp + 20h], 1
        je     SHORT loc_1_2D33
        mov    eax, edi
        inc    edi
        mov    al, byte ptr [eax]
        push   eax
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 24h]
        call   sub_1_30ED
        add    esp, 0ch
        cmp    eax, esi
        jne    SHORT loc_1_2D25
        sub    eax, eax
        jmp    NEAR PTR loc_1_2DF4
loc_1_2D25:
        mov    eax, dword ptr [esp + 1ch]
        dec    dword ptr [esp + 20h]
        inc    dword ptr [eax + 5ch]
        dec    dword ptr [eax + 58h]
loc_1_2D33:
        mov    eax, dword ptr [esp + 1ch]
        movzx  eax, word ptr [eax + 94h]
        lea    ecx, [eax + 402h]
        add    eax, 400h
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [esp + 14h], ecx
        movzx  ebx, word ptr [eax + 122h]
        mov    eax, dword ptr [esp + 20h]
        cmp    ebx, eax
        jb     SHORT loc_1_2D66
        mov    ebx, eax
loc_1_2D66:
        mov    eax, dword ptr [esp + 1ch]
        sub    ebp, ebp
        mov    dword ptr [eax + 160h], 1
loc_1_2D76:
        push   dword ptr [esp + 14h]
        call   sub_1_3E90
        add    esp, 4
        and    eax, 3
        cmp    eax, 1
        je     SHORT loc_1_2DB5
        cmp    eax, 2
        je     SHORT loc_1_2DCB
        movzx  ax, byte ptr [edi]
        movsx  cx, byte ptr [edi + 1]
        inc    edi
        shl    cx, 8
        or     ax, cx
        push   eax
        add    ebp, 2
        push   dword ptr [esp + 14h]
        call   sub_1_3F5D
        add    esp, 8
        inc    edi
        sub    esi, esi
        jmp    SHORT loc_1_2DD4
loc_1_2DB5:
        push   ebx
        push   edi
        add    ebp, ebx
        push   dword ptr [esp + 18h]
        call   sub_1_3F70
        add    esp, 0ch
        add    edi, ebx
        sub    esi, esi
        jmp    SHORT loc_1_2DD4
loc_1_2DCB:
        inc    esi
        cmp    esi, 800h
        ja     SHORT loc_1_2DE8
loc_1_2DD4:
        mov    eax, dword ptr [esp + 20h]
        sub    eax, ebp
        cmp    eax, ebx
        jb     SHORT loc_1_2DE0
        mov    eax, ebx
loc_1_2DE0:
        mov    ebx, eax
        cmp    ebp, dword ptr [esp + 20h]
        jb     SHORT loc_1_2D76
loc_1_2DE8:
        mov    eax, dword ptr [esp + 1ch]
        add    dword ptr [eax + 5ch], ebp
        sub    dword ptr [eax + 58h], ebp
        mov    eax, ebp
loc_1_2DF4:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_2DFC:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esi + 58h]
        or     ecx, ecx
        jne    SHORT loc_1_2E16
        mov    dword ptr [esi + 160h], 0
        jmp    SHORT loc_1_2E65
loc_1_2E16:
        movzx  edi, word ptr [esi + 94h]
        add    edi, 402h
        movzx  eax, word ptr [esi + 122h]
        shl    eax, 3
        cmp    eax, ecx
        jbe    SHORT loc_1_2E33
        mov    eax, ecx
loc_1_2E33:
        mov    bl, byte ptr [esp + 14h]
        push   ebx
        push   eax
        push   esi
        call   sub_1_2CE0
        add    esp, 0ch
        cmp    bl, 74h
        jne    SHORT loc_1_2E58
        mov    al, byte ptr [esi + 16eh]
        test   al, 80h
        je     SHORT loc_1_2E58
        test   al, 1
        jne    SHORT loc_1_2E58
        xor    bl, 10h
loc_1_2E58:
        xor    bl, 4
        push   ebx
        push   edi
        call   sub_1_3F2E
        add    esp, 8
loc_1_2E65:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_2E69:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 20h]
        sub    esi, esi
        movzx  eax, word ptr [ecx + 94h]
        lea    ecx, [eax + 402h]
        mov    dword ptr [esp + 14h], ecx
        add    eax, 400h
        mov    ecx, dword ptr [esp + 20h]
        mov    dword ptr [esp + 10h], eax
        movzx  ebx, word ptr [ecx + 122h]
        mov    eax, dword ptr [esp + 24h]
        cmp    ebx, eax
        jb     SHORT loc_1_2EA5
        mov    ebx, eax
loc_1_2EA5:
        mov    ecx, dword ptr [esp + 20h]
        sub    edi, edi
        mov    dword ptr [ecx + 160h], 1
        mov    eax, dword ptr [ecx + 5ch]
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [ecx + 0f4h]
        mov    ebp, dword ptr [esp + 18h]
        add    ebp, dword ptr [eax + 4]
loc_1_2EC9:
        push   dword ptr [esp + 14h]
        call   sub_1_3E90
        add    esp, 4
        and    eax, 3
        cmp    eax, 1
        je     SHORT loc_1_2EF8
        cmp    eax, 2
        je     SHORT loc_1_2F0E
        mov    al, byte ptr [ebp]
        push   eax
        inc    edi
        push   dword ptr [esp + 14h]
        call   sub_1_3F2E
        add    esp, 8
        inc    ebp
        sub    esi, esi
        jmp    SHORT loc_1_2F17
loc_1_2EF8:
        push   ebx
        push   ebp
        add    edi, ebx
        push   dword ptr [esp + 18h]
        call   sub_1_3F3F
        add    esp, 0ch
        add    ebp, ebx
        sub    esi, esi
        jmp    SHORT loc_1_2F17
loc_1_2F0E:
        inc    esi
        cmp    esi, 800h
        ja     SHORT loc_1_2F2B
loc_1_2F17:
        mov    eax, dword ptr [esp + 24h]
        sub    eax, edi
        cmp    eax, ebx
        jb     SHORT loc_1_2F23
        mov    eax, ebx
loc_1_2F23:
        mov    ebx, eax
        cmp    edi, dword ptr [esp + 24h]
        jb     SHORT loc_1_2EC9
loc_1_2F2B:
        mov    eax, dword ptr [esp + 18h]
        mov    ecx, dword ptr [esp + 20h]
        add    eax, edi
        mov    dword ptr [ecx + 5ch], eax
        sub    dword ptr [ecx + 58h], edi
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_1_2F45:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [edi + 58h]
        or     ecx, ecx
        jne    SHORT loc_1_2F5E
        mov    dword ptr [edi + 160h], 0
        jmp    SHORT loc_1_2FAC
loc_1_2F5E:
        movzx  esi, word ptr [edi + 94h]
        add    esi, 402h
        movzx  eax, word ptr [edi + 122h]
        shl    eax, 2
        cmp    eax, ecx
        jbe    SHORT loc_1_2F7B
        mov    eax, ecx
loc_1_2F7B:
        push   eax
        push   edi
        call   sub_1_2E69
        mov    cl, byte ptr [esp + 18h]
        add    esp, 8
        cmp    cl, 74h
        jne    SHORT loc_1_2F9F
        mov    al, byte ptr [edi + 16eh]
        test   al, 80h
        je     SHORT loc_1_2F9F
        test   al, 1
        jne    SHORT loc_1_2F9F
        xor    cl, 10h
loc_1_2F9F:
        xor    cl, 4
        push   ecx
        push   esi
        call   sub_1_3F2E
        add    esp, 8
loc_1_2FAC:
        pop    edi
        pop    esi
        ret
sub_1_2FAF:
        sub    esp, 4
        push   ebx
        push   esi
        mov    dword ptr [esp + 8], edx
        cmp    dword ptr [esp + 8], 0
        je     NEAR PTR loc_1_30E7
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 13ch], 0
        mov    eax, dword ptr [esp + 8]
        movzx  esi, word ptr [eax + 94h]
        add    esi, 402h
        cmp    dword ptr [eax + 160h], 0
        je     SHORT loc_1_300A
        cmp    byte ptr [eax + 0b2h], 0
        je     SHORT loc_1_3025
        test   byte ptr [eax + 0b3h], 4
        jne    SHORT loc_1_3025
        test   byte ptr [eax + 14ch], 5
        jne    NEAR PTR loc_1_30E7
loc_1_300A:
        mov    eax, dword ptr [esp + 8]
        mov    cl, byte ptr [eax + 0b2h]
        or     cl, cl
        jne    SHORT loc_1_3044
        push   eax
        call   sub_1_01B8
        add    esp, 4
        mov    bl, 54h
        jmp    SHORT loc_1_3091
loc_1_3025:
        push   dword ptr [esp + 8]
        push   0ah
        db  068h, 0AFh, 02Fh, 000h, 000h
        db  0E8h, 00Bh, 0D0h, 0FEh, 0FFh
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx + 13ch], eax
        jmp    NEAR PTR loc_1_30E7
loc_1_3044:
        mov    eax, dword ptr [esp + 8]
        test   byte ptr [eax + 0b3h], 4
        je     SHORT loc_1_3083
        push   eax
        call   sub_1_05C5
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_307F
        push   dword ptr [esp + 8]
        call   sub_1_01B8
        add    esp, 4
        push   10h
        push   dword ptr [esp + 0ch]
        call   sub_1_0287
        add    esp, 8
        cmp    eax, 10000h
        jne    SHORT loc_1_30E7
loc_1_307F:
        mov    bl, 74h
        jmp    SHORT loc_1_3091
loc_1_3083:
        sub    cl, 10h
        cmp    cl, 1
        sbb    bl, bl
        and    bl, 20h
        add    bl, 54h
loc_1_3091:
        push   ebx
        push   esi
        call   sub_1_3F2E
        mov    eax, dword ptr [esp + 10h]
        add    esp, 8
        movzx  eax, word ptr [eax + 120h]
        cmp    eax, 1
        je     SHORT loc_1_30B2
        cmp    eax, 2
        je     SHORT loc_1_30BE
        jmp    SHORT loc_1_30CB
loc_1_30B2:
        push   ebx
        push   dword ptr [esp + 0ch]
        call   sub_1_2F45
        jmp    SHORT loc_1_30C8
loc_1_30BE:
        push   ebx
        push   dword ptr [esp + 0ch]
        call   sub_1_2DFC
loc_1_30C8:
        add    esp, 8
loc_1_30CB:
        mov    eax, dword ptr [esp + 8]
        db  0C7h, 080h, 0BCh, 000h, 000h, 000h, 048h, 016h, 000h, 000h
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 14ch], 4
loc_1_30E7:
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_30ED:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 20h]
        movzx  edi, word ptr [esi + 94h]
        lea    ebp, [edi + 402h]
        add    edi, 400h
        cmp    dword ptr [esi + 160h], 0
        je     SHORT loc_1_3134
        cmp    byte ptr [esi + 0b2h], 0
        je     SHORT loc_1_3126
        test   byte ptr [esi + 0b3h], 4
        je     SHORT loc_1_3134
loc_1_3126:
        mov    dword ptr [esi + 34h], 0
        sub    eax, eax
        jmp    NEAR PTR loc_1_32D7
loc_1_3134:
        cmp    byte ptr [esp + 24h], 74h
        jne    SHORT loc_1_316A
        push   esi
        call   sub_1_05C5
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_316A
        push   esi
        call   sub_1_01B8
        add    esp, 4
        push   10h
        push   esi
        call   sub_1_0287
        add    esp, 8
        cmp    eax, 10000h
        je     SHORT loc_1_316A
        sub    eax, eax
        jmp    NEAR PTR loc_1_32D7
loc_1_316A:
        movzx  eax, word ptr [esi + 120h]
        cmp    eax, 1
        je     SHORT loc_1_3189
        cmp    eax, 2
        je     SHORT loc_1_31C4
        mov    dword ptr [esi + 34h], 10000h
        sub    eax, eax
        jmp    NEAR PTR loc_1_32D7
loc_1_3189:
        push   0
        push   ebp
        call   sub_1_051E
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_31AC
        mov    eax, dword ptr [esi + 34h]
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [esi + 34h], 100h
        sub    eax, eax
        jmp    NEAR PTR loc_1_32D7
loc_1_31AC:
        push   dword ptr [esp + 24h]
        push   ebp
        call   sub_1_3F2E
        mov    al, byte ptr [esp + 30h]
        add    esp, 8
        push   eax
        push   edi
        jmp    NEAR PTR loc_1_32AC
loc_1_31C4:
        movzx  ebx, word ptr [esi + 94h]
        push   64h
        lea    eax, [ebx + 1]
        mov    dword ptr [esp + 18h], eax
        add    ebx, 2
        push   ebp
        call   sub_1_04D0
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_31F8
        mov    eax, dword ptr [esi + 34h]
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [esi + 34h], 100h
        sub    eax, eax
        jmp    NEAR PTR loc_1_32D7
loc_1_31F8:
        push   34h
        push   ebp
        call   sub_1_3F2E
        add    esp, 8
        push   ebx
        call   sub_1_3E90
        add    esp, 4
        and    al, 0efh
        mov    byte ptr [esp + 13h], al
        and    al, 0feh
        mov    byte ptr [esp + 18h], al
        push   dword ptr [esp + 18h]
        push   ebx
        call   sub_1_3F2E
        mov    al, byte ptr [esp + 30h]
        add    esp, 8
        push   eax
        push   edi
        call   sub_1_3F2E
        mov    al, byte ptr [esp + 1bh]
        add    esp, 8
        or     al, 1
        push   eax
        push   ebx
        call   sub_1_3F2E
        add    esp, 8
        push   64h
        push   0
        push   80h
        push   dword ptr [esp + 20h]
        call   sub_1_3E38
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_326D
        mov    eax, dword ptr [esi + 34h]
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [esi + 34h], 200h
        sub    eax, eax
        jmp    SHORT loc_1_32D7
loc_1_326D:
        push   dword ptr [esp + 18h]
        push   ebx
        call   sub_1_3F2E
        add    esp, 8
        push   23h
        push   80h
        push   80h
        push   dword ptr [esp + 20h]
        call   sub_1_3E38
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_32A7
        mov    eax, dword ptr [esi + 34h]
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [esi + 34h], 200h
        sub    eax, eax
        jmp    SHORT loc_1_32D7
loc_1_32A7:
        push   dword ptr [esp + 24h]
        push   ebp
loc_1_32AC:
        call   sub_1_3F2E
        add    esp, 8
        cmp    byte ptr [esp + 24h], 74h
        jne    SHORT loc_1_32CB
        mov    byte ptr [esi + 0b2h], 10h
        mov    byte ptr [esi + 0b3h], 23h
        jmp    SHORT loc_1_32D2
loc_1_32CB:
        mov    byte ptr [esi + 0b2h], 40h
loc_1_32D2:
        mov    eax, 1
loc_1_32D7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_1_32DF:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 24h]
        sub    esi, esi
        movzx  ecx, word ptr [ecx + 94h]
        lea    eax, [ecx + 402h]
        mov    dword ptr [esp + 14h], eax
        lea    eax, [ecx + 400h]
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [esp + 28h]
        and    eax, 1
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [esp + 28h]
        sub    eax, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [esp + 24h]
        mov    dword ptr [esp + 28h], eax
        movzx  ebx, word ptr [ecx + 122h]
        cmp    ebx, eax
        jb     SHORT loc_1_332F
        mov    ebx, eax
loc_1_332F:
        mov    ecx, dword ptr [esp + 24h]
        sub    edi, edi
        mov    dword ptr [ecx + 160h], 1
        mov    eax, dword ptr [ecx + 54h]
        mov    dword ptr [esp + 18h], eax
        mov    ebp, dword ptr [ecx + 3ch]
        add    ebp, eax
loc_1_334B:
        push   dword ptr [esp + 14h]
        call   sub_1_3E90
        add    esp, 4
        and    eax, 3
        cmp    eax, 1
        je     SHORT loc_1_3378
        cmp    eax, 2
        je     SHORT loc_1_33A6
        mov    dx, word ptr [esp + 10h]
        in     ax, dx
        mov    byte ptr [ebp], al
        mov    byte ptr [ebp + 1], ah
        inc    ebp
        inc    ebp
        add    edi, 2
        jmp    SHORT loc_1_33B8
loc_1_3378:
        inc    esi
        cmp    esi, 400h
        jbe    SHORT loc_1_33BA
        mov    ecx, dword ptr [esp + 24h]
        mov    eax, dword ptr [esp + 18h]
        add    dword ptr [ecx + 4ch], edi
        add    eax, edi
        mov    dword ptr [ecx + 54h], eax
        or     edi, edi
        je     SHORT loc_1_33FF
        call   sub_1_39D1
        mov    ecx, dword ptr [esp + 24h]
        mov    dword ptr [ecx + 88h], eax
        jmp    SHORT loc_1_33FF
loc_1_33A6:
        push   ebx
        push   ebp
        add    ebp, ebx
        push   dword ptr [esp + 18h]
        call   sub_1_3ECB
        add    esp, 0ch
        add    edi, ebx
loc_1_33B8:
        sub    esi, esi
loc_1_33BA:
        mov    eax, dword ptr [esp + 28h]
        sub    eax, edi
        cmp    eax, ebx
        jb     SHORT loc_1_33C6
        mov    eax, ebx
loc_1_33C6:
        mov    ebx, eax
        cmp    edi, dword ptr [esp + 28h]
        jb     NEAR PTR loc_1_334B
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_1_33E4
        mov    dx, word ptr [esp + 10h]
        in     ax, dx
        mov    byte ptr [ebp], al
        inc    edi
loc_1_33E4:
        call   sub_1_39D1
        mov    ecx, dword ptr [esp + 24h]
        mov    dword ptr [ecx + 88h], eax
        add    dword ptr [ecx + 4ch], edi
        mov    eax, dword ptr [esp + 18h]
        add    eax, edi
        mov    dword ptr [ecx + 54h], eax
loc_1_33FF:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
sub_1_3409:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 0f0h], 0
        je     NEAR PTR loc_1_34AE
        mov    ecx, dword ptr [esi + 40h]
        sub    ecx, dword ptr [esi + 54h]
        je     NEAR PTR loc_1_34AE
        movzx  eax, word ptr [esi + 122h]
        shl    eax, 3
        cmp    ecx, eax
        jae    SHORT loc_1_3437
        mov    eax, ecx
loc_1_3437:
        push   eax
        push   esi
        call   sub_1_32DF
        add    esp, 8
        test   byte ptr [esi + 150h], 1
        je     SHORT loc_1_3460
        mov    eax, dword ptr [esi + 54h]
        cmp    dword ptr [esi + 0f0h], eax
        jg     SHORT loc_1_3460
        push   1
        push   esi
        call   sub_1_1B99
        add    esp, 8
loc_1_3460:
        mov    eax, dword ptr [esi + 54h]
        cmp    dword ptr [esi + 0f0h], eax
        jle    SHORT loc_1_349A
        movzx  eax, word ptr [esi + 94h]
        add    eax, 402h
        mov    cl, byte ptr [esi + 16eh]
        test   cl, 80h
        je     SHORT loc_1_3489
        test   cl, 1
        mov    cl, 64h
        je     SHORT loc_1_348B
loc_1_3489:
        mov    cl, 74h
loc_1_348B:
        xor    cl, 4
        push   ecx
        push   eax
        call   sub_1_3F2E
        add    esp, 8
        jmp    SHORT loc_1_34AE
loc_1_349A:
        mov    dword ptr [esi + 160h], 0
        mov    dword ptr [esi + 14ch], 0
loc_1_34AE:
        pop    esi
        ret
sub_1_34B0:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 20h]
        sub    esi, esi
        movzx  eax, word ptr [ecx + 94h]
        lea    ecx, [eax + 402h]
        mov    dword ptr [esp + 14h], ecx
        add    eax, 400h
        mov    ecx, dword ptr [esp + 20h]
        mov    dword ptr [esp + 10h], eax
        movzx  ebx, word ptr [ecx + 122h]
        mov    eax, dword ptr [esp + 24h]
        cmp    ebx, eax
        jb     SHORT loc_1_34EC
        mov    ebx, eax
loc_1_34EC:
        mov    ecx, dword ptr [esp + 20h]
        sub    edi, edi
        mov    dword ptr [ecx + 160h], 1
        mov    eax, dword ptr [ecx + 54h]
        mov    dword ptr [esp + 18h], eax
        mov    ebp, dword ptr [ecx + 3ch]
        add    ebp, eax
loc_1_3508:
        push   dword ptr [esp + 14h]
        call   sub_1_3E90
        add    esp, 4
        and    eax, 3
        cmp    eax, 1
        je     SHORT loc_1_3534
        cmp    eax, 2
        je     SHORT loc_1_3562
        push   dword ptr [esp + 10h]
        call   sub_1_3E90
        add    esp, 4
        mov    byte ptr [ebp], al
        inc    ebp
        inc    edi
        jmp    SHORT loc_1_3574
loc_1_3534:
        inc    esi
        cmp    esi, 400h
        jbe    SHORT loc_1_3576
        mov    ecx, dword ptr [esp + 20h]
        mov    eax, dword ptr [esp + 18h]
        add    dword ptr [ecx + 4ch], edi
        add    eax, edi
        mov    dword ptr [ecx + 54h], eax
        or     edi, edi
        je     SHORT loc_1_35A9
        call   sub_1_39D1
        mov    ecx, dword ptr [esp + 20h]
        mov    dword ptr [ecx + 88h], eax
        jmp    SHORT loc_1_35A9
loc_1_3562:
        push   ebx
        push   ebp
        add    ebp, ebx
        push   dword ptr [esp + 18h]
        call   sub_1_3E9E
        add    esp, 0ch
        add    edi, ebx
loc_1_3574:
        sub    esi, esi
loc_1_3576:
        mov    eax, dword ptr [esp + 24h]
        sub    eax, edi
        cmp    eax, ebx
        jb     SHORT loc_1_3582
        mov    eax, ebx
loc_1_3582:
        mov    ebx, eax
        cmp    dword ptr [esp + 24h], edi
        ja     NEAR PTR loc_1_3508
        call   sub_1_39D1
        mov    ecx, dword ptr [esp + 20h]
        mov    dword ptr [ecx + 88h], eax
        add    dword ptr [ecx + 4ch], edi
        mov    eax, dword ptr [esp + 18h]
        add    eax, edi
        mov    dword ptr [ecx + 54h], eax
loc_1_35A9:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_1_35B3:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 0f0h], 0
        je     NEAR PTR loc_1_3658
        mov    ecx, dword ptr [esi + 40h]
        sub    ecx, dword ptr [esi + 54h]
        je     NEAR PTR loc_1_3658
        movzx  eax, word ptr [esi + 122h]
        shl    eax, 2
        cmp    ecx, eax
        jae    SHORT loc_1_35E1
        mov    eax, ecx
loc_1_35E1:
        push   eax
        push   esi
        call   sub_1_34B0
        add    esp, 8
        test   byte ptr [esi + 150h], 1
        je     SHORT loc_1_360A
        mov    eax, dword ptr [esi + 54h]
        cmp    dword ptr [esi + 0f0h], eax
        jg     SHORT loc_1_360A
        push   1
        push   esi
        call   sub_1_1B99
        add    esp, 8
loc_1_360A:
        mov    eax, dword ptr [esi + 54h]
        cmp    dword ptr [esi + 0f0h], eax
        jle    SHORT loc_1_3644
        movzx  eax, word ptr [esi + 94h]
        add    eax, 402h
        mov    cl, byte ptr [esi + 16eh]
        test   cl, 80h
        je     SHORT loc_1_3633
        test   cl, 1
        mov    cl, 64h
        je     SHORT loc_1_3635
loc_1_3633:
        mov    cl, 74h
loc_1_3635:
        xor    cl, 4
        push   ecx
        push   eax
        call   sub_1_3F2E
        add    esp, 8
        jmp    SHORT loc_1_3658
loc_1_3644:
        mov    dword ptr [esi + 160h], 0
        mov    dword ptr [esi + 14ch], 0
loc_1_3658:
        pop    esi
        ret
loc_1_365A:
        sub    esp, 4
        mov    dword ptr [esp], edx
        cmp    dword ptr [esp], 0
        je     NEAR PTR loc_1_3728
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 140h], 0
        mov    eax, dword ptr [esp]
        cmp    dword ptr [eax + 160h], 0
        je     SHORT loc_1_36A3
        push   eax
        push   0ah
        db  068h, 05Ah, 036h, 000h, 000h
        db  0E8h, 0ACh, 0C9h, 0FEh, 0FFh
        mov    ecx, dword ptr [esp]
        mov    dword ptr [ecx + 140h], eax
        jmp    NEAR PTR loc_1_3728
loc_1_36A3:
        push   dword ptr [esp]
        call   sub_1_064D
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_36CF
        push   dword ptr [esp]
        push   0ah
        db  068h, 05Ah, 036h, 000h, 000h
        db  0E8h, 07Dh, 0C9h, 0FEh, 0FFh
        mov    ecx, dword ptr [esp]
        mov    dword ptr [ecx + 140h], eax
        jmp    SHORT loc_1_3728
loc_1_36CF:
        mov    ecx, dword ptr [esp]
        mov    eax, ecx
        mov    eax, dword ptr [eax + 54h]
        cmp    dword ptr [ecx + 0f0h], eax
        jle    SHORT loc_1_3728
        mov    eax, ecx
        db  0C7h, 080h, 0BCh, 000h, 000h, 000h, 09Bh, 016h, 000h, 000h
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 14ch], 8
        mov    eax, dword ptr [esp]
        movzx  eax, word ptr [eax + 120h]
        cmp    eax, 1
        je     SHORT loc_1_3711
        cmp    eax, 2
        je     SHORT loc_1_371C
        jmp    SHORT loc_1_3728
loc_1_3711:
        push   dword ptr [esp]
        call   sub_1_35B3
        jmp    SHORT loc_1_3725
loc_1_371C:
        push   dword ptr [esp]
        call   sub_1_3409
loc_1_3725:
        add    esp, 4
loc_1_3728:
        add    esp, 4
        ret
LPT_Control:
        cmp    eax, 1bh
        db  00Fh, 084h, 0CBh, 0C8h, 0FEh, 0FFh
        cmp    eax, 1ch
        db  00Fh, 084h, 0C2h, 0D5h, 0FEh, 0FFh
        clc
        ret
sub_1_3740:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [esp + 0ch]
        db  0CDh, 020h, 001h, 000h, 004h, 000h
        jae    SHORT loc_1_3753
        xor    eax, eax
loc_1_3753:
        clc
        pop    esi
        ret
sub_1_3756:
        mov    eax, dword ptr [esp + 4]
        db  0CDh, 020h, 01Ah, 000h, 004h, 000h
        clc
        ret
sub_1_3762:
        push   ebx
        xor    ebx, ebx
        movzx  ebx, word ptr [esp + 8]
        db  0CDh, 020h, 00Fh, 000h, 004h, 000h
        jae    SHORT loc_1_3772
loc_1_3772:
        pop    ebx
        ret
sub_1_3774:
        push   edi
        push   esi
        push   ebx
        mov    eax, dword ptr [esp + 10h]
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0CDh, 020h, 004h, 000h, 004h, 000h
        mov    edi, dword ptr [esp + 14h]
        mov    dword ptr [edi + 4], esi
        mov    ecx, dword ptr [edi]
        db  0CDh, 020h, 00Eh, 000h, 004h, 000h
        jb     SHORT loc_1_37C2
        mov    dword ptr [edi + 0ch], edx
        mov    word ptr [edi + 0ah], bx
        push   0
        push   ecx
        push   edx
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_1_37BE
        mov    dword ptr [edi + 4], eax
        mov    eax, 1
loc_1_37B9:
        clc
        pop    ebx
        pop    esi
        pop    edi
        ret
loc_1_37BE:
        xor    eax, eax
        jmp    SHORT loc_1_37B9
loc_1_37C2:
        cmp    al, 5
        jne    SHORT loc_1_37CA
        xor    eax, eax
        jmp    SHORT loc_1_37B9
loc_1_37CA:
        cmp    al, 6
        jne    SHORT loc_1_37B9
        xor    eax, eax
        jmp    SHORT loc_1_37B9
sub_1_37D2:
        push   edi
        push   esi
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    esi, dword ptr [esp + 10h]
        db  0C7h, 005h, 08Ch, 048h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 18h]
        push   0eh
        db  068h, 08Ch, 048h, 000h, 000h
        push   1000h
        push   0
        push   ecx
        push   0
        push   1
        push   eax
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        db  0A3h, 090h, 048h, 000h, 000h
        or     eax, edx
        je     SHORT loc_1_3846
        db  0A1h, 08Ch, 048h, 000h, 000h
        mov    dword ptr [esi + 0ch], eax
        mov    ecx, dword ptr [esi]
        push   0
        push   ecx
        push   eax
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_1_3848
        mov    word ptr [esi + 0ah], 8000h
        mov    dword ptr [esi + 4], eax
loc_1_383C:
        db  0A1h, 090h, 048h, 000h, 000h
        clc
        pop    ebx
        pop    esi
        pop    edi
        ret
loc_1_3846:
        jmp    SHORT loc_1_383C
loc_1_3848:
        jmp    SHORT loc_1_383C
sub_1_384A:
        push   edi
        push   esi
        mov    edi, dword ptr [esp + 0ch]
        mov    esi, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi]
        mov    dl, 1
        db  0CDh, 020h, 009h, 000h, 004h, 000h
        xor    eax, eax
        jb     SHORT loc_1_3862
        inc    eax
loc_1_3862:
        pop    esi
        pop    edi
        ret
sub_1_3865:
        pushad
        mov    edi, dword ptr [esp + 28h]
        mov    esi, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi]
        mov    dl, 1
        db  0CDh, 020h, 009h, 000h, 004h, 000h
        jae    SHORT loc_1_387B
        jmp    SHORT loc_1_38A1
loc_1_387B:
        mov    eax, dword ptr [esp + 24h]
        mov    bx, word ptr [edi + 0ah]
        or     bh, 0ffh
        db  0CDh, 020h, 003h, 000h, 004h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    dl, byte ptr [esp + 2ch]
        or     dl, 0
        mov    dh, 0
        db  0CDh, 020h, 006h, 000h, 004h, 000h
loc_1_38A1:
        popad
        ret
sub_1_38A3:
        push   ebx
        mov    eax, dword ptr [esp + 8]
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0CDh, 020h, 008h, 000h, 004h, 000h
        pop    ebx
        ret
sub_1_38B6:
        mov    eax, dword ptr [esp + 4]
        db  0CDh, 020h, 007h, 000h, 004h, 000h
        ret
sub_1_38C1:
        push   edi
        push   esi
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        xor    eax, eax
        mov    edi, dword ptr [esp + 10h]
        db  0CDh, 020h, 00Bh, 000h, 004h, 000h
        mov    eax, 0
        mov    word ptr [edi + 0ch], 0
        jb     SHORT loc_1_38EF
        je     SHORT loc_1_38ED
        db  0CDh, 020h, 00Ch, 000h, 004h, 000h
        jmp    SHORT loc_1_38EF
loc_1_38ED:
        mov    eax, edx
loc_1_38EF:
        pop    ebx
        pop    esi
        pop    edi
        ret
sub_1_38F3:
        push   edi
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        xor    eax, eax
        mov    edi, dword ptr [esp + 0ch]
        db  0CDh, 020h, 00Ch, 000h, 004h, 000h
        mov    word ptr [edi + 0eh], 0
        mov    word ptr [edi + 0ch], 0
        pop    ebx
        pop    edi
        ret
sub_1_3916:
        push   edi
        push   ebx
        mov    edi, dword ptr [esp + 0ch]
        xor    ebx, ebx
        mov    bx, word ptr [edi + 0ch]
        shl    bx, 3
        add    edi, ebx
        add    edi, 10h
        mov    edx, dword ptr [edi]
        mov    ecx, dword ptr [edi + 4]
        cmp    edx, 1000000h
        jae    SHORT loc_1_3963
        movzx  ebx, word ptr [esp + 10h]
        dec    ebx
        je     SHORT loc_1_3948
        and    ebx, edx
        jne    SHORT loc_1_3963
        and    ebx, ecx
        jne    SHORT loc_1_3963
loc_1_3948:
        push   edx
        not    dx
        and    edx, 0ffffh
        inc    edx
        cmp    ecx, edx
        jbe    SHORT loc_1_395B
        xor    eax, eax
        jmp    SHORT loc_1_3965
loc_1_395B:
        pop    edx
        mov    eax, 1
        jmp    SHORT loc_1_3965
loc_1_3963:
        xor    eax, eax
loc_1_3965:
        pop    ebx
        pop    edi
        ret
sub_1_3968:
        push   esi
        push   edi
        push   ebx
        mov    edi, dword ptr [esp + 14h]
        movzx  ebx, word ptr [edi + 0ch]
        mov    cx, word ptr [edi + 0eh]
        cmp    bx, cx
        jne    SHORT loc_1_3980
        xor    eax, eax
        jmp    SHORT loc_1_39B6
loc_1_3980:
        shl    bx, 3
        add    edi, ebx
        add    edi, 10h
        mov    edx, dword ptr [edi]
        mov    ecx, dword ptr [edi + 4]
        mov    eax, dword ptr [esp + 10h]
        mov    ebx, 100h
        xor    esi, esi
        db  0CDh, 020h, 003h, 000h, 004h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    edx, 8
        db  0CDh, 020h, 006h, 000h, 004h, 000h
        mov    edi, dword ptr [esp + 14h]
        mov    eax, ecx
loc_1_39B6:
        pop    ebx
        pop    edi
        pop    esi
        ret
        db  000h, 000h
sub_1_39BC:
        push   ebp
        mov    ebp, esp
        xor    ecx, ecx
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 00Bh, 000h, 003h, 000h
        sbb    edx, edx
        inc    edx
        mov    eax, edx
        leave
        ret
sub_1_39D1:
        xor    eax, eax
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        ret
sub_1_39DA:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        cld
        rep movsb
        pop    edi
        pop    esi
        leave
        ret
sub_1_39EF:
        push   ebp
        mov    ebp, esp
        mov    dx, word ptr [ebp + 8]
        mov    al, byte ptr [ebp + 0ch]
        out    dx, al
        out    dx, al
        add    dx, 2
        in     al, dx
        and    al, 40h
        mov    cl, al
        or     al, 0dh
        out    dx, al
        out    dx, al
        out    dx, al
        out    dx, al
        or     cl, 0ch
        mov    al, cl
        out    dx, al
        leave
        ret
sub_1_3A12:
        push   ebp
        mov    ebp, esp
        mov    dx, word ptr [ebp + 8]
loc_1_3A19:
        in     al, dx
        mov    ah, al
        jmp    SHORT loc_1_3A1E
loc_1_3A1E:
        in     al, dx
        cmp    al, ah
        jne    SHORT loc_1_3A19
        and    ah, 0f8h
        xor    ah, 48h
        test   ah, 28h
        jne    SHORT loc_1_3A3A
        test   ah, 10h
        je     SHORT loc_1_3A3A
        and    ah, 80h
        je     SHORT loc_1_3A3A
        xor    eax, eax
loc_1_3A3A:
        leave
        ret
sub_1_3A3C:
        push   ebp
        mov    ebp, esp
        mov    dx, word ptr [ebp + 8]
        in     al, dx
        and    al, 0f8h
        xor    al, 48h
        mov    ah, al
        and    al, 30h
        cmp    al, 30h
        mov    al, ah
        jne    SHORT loc_1_3A56
        and    al, 0dfh
        or     al, 8
loc_1_3A56:
        test   al, 28h
        jne    SHORT loc_1_3A63
        and    ah, 80h
        mov    al, 1
        je     SHORT loc_1_3A63
        xor    eax, eax
loc_1_3A63:
        leave
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_3A70:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        movzx  edi, word ptr [esp + 14h]
        lea    esi, [edi + 1]
        add    edi, 2
        push   edi
        call   sub_1_3E90
        add    esp, 4
        and    al, 40h
        mov    bl, al
        or     al, 0ch
        mov    byte ptr [esp + 0ch], al
        push   dword ptr [esp + 0ch]
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        cmp    dword ptr [esp + 18h], 0
        push   23h
        je     SHORT loc_1_3AE6
        push   10h
        push   50h
        push   esi
        call   sub_1_3E38
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_3AF7
loc_1_3ABB:
        or     bl, 8
        push   ebx
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   ebx
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   dword ptr [esp + 0ch]
        push   edi
        call   sub_1_3F2E
        add    esp, 8
loc_1_3ADF:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_3AE6:
        push   0
        push   50h
        push   esi
        call   sub_1_3E38
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_3ABB
loc_1_3AF7:
        mov    al, bl
        or     al, 0eh
        push   eax
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   23h
        push   40h
        push   40h
        push   esi
        call   sub_1_3E38
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_3ABB
        push   dword ptr [esp + 0ch]
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   23h
        push   80h
        push   80h
        push   esi
        call   sub_1_3E38
        add    esp, 10h
        jmp    SHORT loc_1_3ADF
sub_1_3B3C:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        movzx  esi, word ptr [esp + 1ch]
        lea    ebp, [esi + 1]
        lea    edi, [esi + 2]
        push   edi
        call   sub_1_3E90
        add    esp, 4
        and    al, 40h
        mov    bl, al
        or     al, 0ch
        mov    byte ptr [esp + 14h], al
        push   dword ptr [esp + 14h]
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   dword ptr [esp + 20h]
        push   esi
        call   sub_1_3F2E
        add    esp, 8
        mov    al, bl
        or     al, 6
        push   eax
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   23h
        push   38h
        push   78h
        push   ebp
        call   sub_1_3E38
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_3BB3
        push   dword ptr [esp + 14h]
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        mov    eax, 100000h
        jmp    NEAR PTR loc_1_3C70
loc_1_3BB3:
        mov    al, bl
        or     al, 7
        mov    byte ptr [esp + 10h], al
        push   dword ptr [esp + 10h]
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        or     bl, 4
        push   dword ptr [esp + 10h]
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   ebx
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        push   ebx
        push   edi
        call   sub_1_3F2E
        add    esp, 8
        cmp    byte ptr [esp + 20h], 0
        jne    SHORT loc_1_3C0D
        push   23h
        push   40h
        push   50h
        push   ebp
        call   sub_1_3E38
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_3C55
        mov    eax, 10000h
        jmp    SHORT loc_1_3C70
loc_1_3C0D:
        cmp    byte ptr [esp + 20h], 10h
        je     SHORT loc_1_3C35
        cmp    byte ptr [esp + 20h], 14h
        je     SHORT loc_1_3C35
        push   23h
        push   50h
        push   50h
        push   ebp
        call   sub_1_3E38
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_3C55
        mov    eax, 10000h
        jmp    SHORT loc_1_3C70
loc_1_3C35:
        push   23h
        push   0d8h
        push   0f8h
        push   ebp
        call   sub_1_3E38
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_3C55
        mov    eax, 10000h
        jmp    SHORT loc_1_3C70
loc_1_3C55:
        cmp    byte ptr [esp + 20h], 0
        jne    SHORT loc_1_3C60
        push   0
        jmp    SHORT loc_1_3C62
loc_1_3C60:
        push   1
loc_1_3C62:
        push   dword ptr [esp + 20h]
        call   sub_1_3A70
        add    esp, 8
        sub    eax, eax
loc_1_3C70:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_3C78:
        push   ebp
        mov    ebp, esp
        push   eax
        mov    al, byte ptr [ebp + 8]
        rcl    al, 1
        jb     SHORT loc_1_3C8A
        bts    ax, 7
        jmp    SHORT loc_1_3C8F
loc_1_3C8A:
        btr    ax, 7
loc_1_3C8F:
        shr    al, 4
        mov    ah, byte ptr [ebp + 0ch]
        rcl    ah, 1
        jb     SHORT loc_1_3CA0
        bts    ax, 0fh
        jmp    SHORT loc_1_3CA5
loc_1_3CA0:
        btr    ax, 0fh
loc_1_3CA5:
        and    ah, 0f0h
        or     al, ah
        mov    byte ptr [ebp + 0ch], al
        pop    eax
        mov    al, byte ptr [ebp + 0ch]
        pop    ebp
        ret
sub_1_3CB3:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        movzx  ebp, word ptr [esp + 1ch]
        sub    edi, edi
        lea    esi, [ebp + 1]
        add    ebp, 2
        push   ebp
        call   sub_1_3E90
        mov    byte ptr [esp + 15h], al
        add    esp, 4
        test   al, 2
        je     SHORT loc_1_3D2B
        push   esi
        call   sub_1_3E20
        add    esp, 4
        cmp    eax, edi
        je     SHORT loc_1_3CF2
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [eax], edi
        sub    eax, eax
        jmp    NEAR PTR loc_1_3E16
loc_1_3CF2:
        and    byte ptr [esp + 11h], 0f0h
        mov    al, byte ptr [esp + 11h]
        or     al, 4
        push   eax
        push   ebp
        call   sub_1_3F2E
        add    esp, 8
        push   23h
        push   40h
        push   60h
        push   esi
        call   sub_1_3E38
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_3D30
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [eax], 0
        sub    eax, eax
        jmp    NEAR PTR loc_1_3E16
loc_1_3D2B:
        and    byte ptr [esp + 11h], 0f0h
loc_1_3D30:
        mov    al, byte ptr [esp + 11h]
        or     al, 6
        mov    byte ptr [esp + 14h], al
loc_1_3D3A:
        push   dword ptr [esp + 14h]
        push   ebp
        call   sub_1_3F2E
        add    esp, 8
        push   23h
        push   0
        push   40h
        push   esi
        call   sub_1_3E38
        add    esp, 10h
        or     eax, eax
        je     NEAR PTR loc_1_3E0B
        push   esi
        call   sub_1_3E90
        mov    byte ptr [esp + 16h], al
        mov    bl, byte ptr [esp + 15h]
        add    esp, 4
        or     bl, 4
        push   ebx
        push   ebp
        call   sub_1_3F2E
        add    esp, 8
        push   23h
        push   40h
        push   40h
        push   esi
        call   sub_1_3E38
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_3E0B
        push   dword ptr [esp + 14h]
        push   ebp
        call   sub_1_3F2E
        add    esp, 8
        push   23h
        push   0
        push   40h
        push   esi
        call   sub_1_3E38
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_3E0B
        push   esi
        call   sub_1_3E90
        mov    byte ptr [esp + 17h], al
        add    esp, 4
        push   ebx
        push   ebp
        call   sub_1_3F2E
        add    esp, 8
        push   dword ptr [esp + 13h]
        inc    edi
        push   dword ptr [esp + 16h]
        call   sub_1_3C78
        mov    ecx, dword ptr [esp + 28h]
        add    esp, 8
        mov    byte ptr [ecx], al
        inc    dword ptr [esp + 20h]
        push   23h
        push   40h
        push   40h
        push   esi
        call   sub_1_3E38
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_3E0B
        push   esi
        call   sub_1_3E20
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_3E0B
        cmp    edi, dword ptr [esp + 24h]
        jb     NEAR PTR loc_1_3D3A
loc_1_3E0B:
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [eax], edi
        mov    eax, 1
loc_1_3E16:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  000h, 000h
sub_1_3E20:
        push   dword ptr [esp + 4]
        call   sub_1_3E90
        add    esp, 4
        test   al, 8
        mov    eax, 1
        jne    SHORT loc_1_3E37
        sub    eax, eax
loc_1_3E37:
        ret
sub_1_3E38:
        push   ebx
        push   esi
        push   edi
        db  068h, 094h, 048h, 000h, 000h
        db  0E8h, 0BBh, 0CFh, 0FEh, 0FFh
        mov    esi, dword ptr [esp + 14h]
        mov    bl, byte ptr [esp + 18h]
        mov    edi, dword ptr [esp + 20h]
        add    esp, 4
loc_1_3E54:
        db  068h, 098h, 048h, 000h, 000h
        db  0E8h, 0A2h, 0CFh, 0FEh, 0FFh
        add    esp, 4
        push   esi
        call   sub_1_3E90
        add    esp, 4
        and    al, bl
        cmp    al, byte ptr [esp + 18h]
        je     SHORT loc_1_3E87
        db  0A1h, 098h, 048h, 000h, 000h
        db  02Bh, 005h, 094h, 048h, 000h, 000h
        cmp    eax, edi
        jl     SHORT loc_1_3E54
        sub    eax, eax
loc_1_3E83:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_3E87:
        mov    eax, 1
        jmp    SHORT loc_1_3E83
        db  000h, 000h
sub_1_3E90:
        push   ebp
        mov    ebp, esp
        xor    eax, eax
        push   edx
        mov    dx, word ptr [ebp + 8]
        in     al, dx
        pop    edx
        leave
        ret
sub_1_3E9E:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        xor    edx, edx
        xor    ebx, ebx
        mov    dx, word ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
loc_1_3EB1:
        in     al, dx
        mov    byte ptr [ebx + esi], al
        inc    ebx
        loop   loc_1_3EB1
        pop    ebx
        pop    esi
        leave
        ret
        push   ebp
        mov    ebp, esp
        xor    eax, eax
        push   edx
        mov    dx, word ptr [ebp + 8]
        in     ax, dx
        pop    edx
        leave
        ret
sub_1_3ECB:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        xor    edx, edx
        xor    ebx, ebx
        mov    dx, word ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        shr    ecx, 1
loc_1_3EE0:
        in     ax, dx
        mov    byte ptr [ebx + esi], al
        inc    ebx
        mov    byte ptr [ebx + esi], ah
        inc    ebx
        loop   loc_1_3EE0
        pop    ebx
        pop    esi
        leave
        ret
        push   ebp
        mov    ebp, esp
        xor    eax, eax
        push   edx
        mov    dx, word ptr [ebp + 8]
        in     eax, dx
        pop    edx
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        xor    edx, edx
        xor    ebx, ebx
        mov    dx, word ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        shr    ecx, 2
loc_1_3F14:
        in     eax, dx
        mov    byte ptr [ebx + esi], al
        inc    ebx
        mov    byte ptr [ebx + esi], ah
        inc    ebx
        shr    eax, 10h
        mov    byte ptr [ebx + esi], al
        inc    ebx
        mov    byte ptr [ebx + esi], ah
        inc    ebx
        loop   loc_1_3F14
        pop    ebx
        pop    esi
        leave
        ret
sub_1_3F2E:
        push   ebp
        mov    ebp, esp
        push   edx
        xor    edx, edx
        mov    dx, word ptr [ebp + 8]
        mov    al, byte ptr [ebp + 0ch]
        out    dx, al
        pop    edx
        leave
        ret
sub_1_3F3F:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        xor    edx, edx
        xor    ebx, ebx
        mov    dx, word ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
loc_1_3F52:
        mov    al, byte ptr [ebx + esi]
        out    dx, al
        inc    ebx
        loop   loc_1_3F52
        pop    ebx
        pop    esi
        leave
        ret
sub_1_3F5D:
        push   ebp
        mov    ebp, esp
        push   edx
        xor    edx, edx
        mov    dx, word ptr [ebp + 8]
        mov    ax, word ptr [ebp + 0ch]
        out    dx, ax
        pop    edx
        leave
        ret
sub_1_3F70:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        xor    edx, edx
        xor    ebx, ebx
        mov    dx, word ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        shr    ecx, 1
loc_1_3F85:
        mov    al, byte ptr [ebx + esi]
        inc    ebx
        mov    ah, byte ptr [ebx + esi]
        inc    ebx
        out    dx, ax
        loop   loc_1_3F85
        pop    ebx
        pop    esi
        leave
        ret
loc_1_3F95:
        push   ebp
        mov    ebp, esp
        push   edx
        xor    edx, edx
        mov    dx, word ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        out    dx, eax
        pop    edx
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        xor    edx, edx
        xor    ebx, ebx
        mov    dx, word ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        shr    ecx, 2
loc_1_3FBC:
        mov    al, byte ptr [ebx + esi]
        inc    ebx
        mov    ah, byte ptr [ebx + esi]
        inc    ebx
        shl    eax, 10h
        mov    al, byte ptr [ebx + esi]
        inc    ebx
        mov    ah, byte ptr [ebx + esi]
        inc    ebx
        ror    eax, 10h
        out    dx, eax
        loop   loc_1_3FBC
        pop    ebx
        pop    esi
        leave
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 001h, 001h, 001h, 001h, 002h, 002h
        db  002h, 002h, 002h, 002h, 002h, 003h, 003h, 003h, 003h, 003h, 003h
loc_1_3FF4:
        retf   28h
        db  000h, 098h, 00Ch, 000h, 000h, 022h, 009h, 000h, 000h, 06Dh, 027h, 000h, 000h, 055h, 009h, 000h
        db  000h, 0C3h, 022h, 000h, 000h, 02Bh, 027h, 000h, 000h, 000h, 000h, 000h, 000h, 07Dh, 00Ah, 000h
        db  000h, 08Ch, 024h, 000h, 000h, 01Ah, 022h, 000h, 000h, 0A8h, 00Ah, 000h, 000h, 0CEh, 00Ah, 000h
        db  000h, 020h, 028h, 000h, 000h, 061h, 028h, 000h, 000h, 0F9h, 00Ah, 000h, 000h, 091h, 00Bh, 000h
        db  000h, 032h, 00Bh, 000h, 000h, 000h, 000h, 000h, 000h, 0A9h, 00Ch, 000h, 000h, 0D5h, 00Ch, 000h
        db  000h, 0DBh, 028h, 000h, 000h, 031h, 026h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
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
        db  000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 001h, 000h, 000h, 000h, 04Ch, 050h, 054h, 020h
        db  020h, 020h, 020h, 020h, 000h, 000h, 000h, 080h, 02Ch, 037h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
_LTEXT ends
        end
