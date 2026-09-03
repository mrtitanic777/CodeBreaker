; lpt_pcode_03.asm -- LPT object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  083h, 0ECh, 004h, 053h, 056h, 057h, 055h, 08Bh, 044h, 024h, 018h, 066h, 02Bh, 0FFh, 00Fh, 0B7h
        db  098h, 094h, 000h, 000h, 000h, 08Dh, 043h, 002h, 089h, 044h, 024h, 010h, 08Dh, 0ABh, 002h, 004h
        db  000h, 000h, 08Bh, 044h, 024h, 018h, 081h, 0C3h, 000h, 004h, 000h, 000h, 0FFh, 074h, 024h, 010h
        db  066h, 0C7h, 080h, 022h, 001h, 000h, 000h, 001h, 000h, 066h, 089h, 0B8h, 01Ch, 001h, 000h, 000h
        db  066h, 0C7h, 080h, 01Eh, 001h, 000h, 000h, 001h, 000h, 0E8h, 042h, 03Eh, 0FDh, 0FFh, 083h, 0C4h
        db  004h, 00Ch, 020h, 050h, 0FFh, 074h, 024h, 014h, 0E8h, 0D1h, 03Eh, 0FDh, 0FFh, 083h, 0C4h, 008h
        db  068h, 0D0h, 000h, 000h, 000h, 055h, 0E8h, 0C3h, 03Eh, 0FDh, 0FFh, 08Bh, 044h, 024h, 020h, 083h
        db  0C4h, 008h, 066h, 08Bh, 0B0h, 020h, 001h, 000h, 000h, 066h, 083h, 0FEh, 001h, 075h, 00Dh, 068h
        db  0FFh, 000h, 000h, 000h, 053h, 0E8h, 0A4h, 03Eh, 0FDh, 0FFh, 0EBh, 01Bh, 066h, 083h, 0FEh, 002h
        db  075h, 00Dh, 068h, 0FFh, 0FFh, 000h, 000h, 053h, 0E8h, 0C0h, 03Eh, 0FDh, 0FFh, 0EBh, 008h, 06Ah
        db  0FFh, 053h, 0E8h, 0EEh, 03Eh, 0FDh, 0FFh, 083h, 0C4h, 008h, 066h, 047h, 055h, 0E8h, 0DEh, 03Dh
        db  0FDh, 0FFh, 083h, 0C4h, 004h, 08Ah, 0C8h, 080h, 0E1h, 003h, 080h, 0F9h, 001h, 074h, 029h, 0A8h
        db  004h, 074h, 0B6h, 066h, 08Bh, 0CFh, 08Bh, 044h, 024h, 018h, 066h, 00Fh, 0AFh, 0CEh, 066h, 089h
        db  088h, 01Eh, 001h, 000h, 000h, 066h, 083h, 0FEh, 001h, 075h, 014h, 068h, 0FFh, 000h, 000h, 000h
        db  053h, 0E8h, 048h, 03Eh, 0FDh, 0FFh, 0EBh, 022h, 02Bh, 0C0h, 0E9h, 0B1h, 000h, 000h, 000h, 066h
        db  083h, 0FEh, 002h, 075h, 00Dh, 068h, 0FFh, 0FFh, 000h, 000h, 053h, 0E8h, 05Dh, 03Eh, 0FDh, 0FFh
        db  0EBh, 008h, 06Ah, 0FFh, 053h, 0E8h, 08Bh, 03Eh, 0FDh, 0FFh, 083h, 0C4h, 008h, 066h, 047h, 055h
        db  0E8h, 07Bh, 03Dh, 0FDh, 0FFh, 083h, 0C4h, 004h, 024h, 003h, 03Ch, 002h, 075h, 0B7h, 08Bh, 044h
        db  024h, 018h, 0FFh, 074h, 024h, 010h, 066h, 089h, 0B8h, 022h, 001h, 000h, 000h, 0E8h, 05Eh, 03Dh
        db  0FDh, 0FFh, 083h, 0C4h, 004h, 024h, 0DFh, 050h, 0FFh, 074h, 024h, 014h, 0E8h, 0EDh, 03Dh, 0FDh
        db  0FFh, 083h, 0C4h, 008h, 055h, 0E8h, 046h, 03Dh, 0FDh, 0FFh, 083h, 0C4h, 004h, 024h, 0FBh, 050h
        db  055h, 0E8h, 0D8h, 03Dh, 0FDh, 0FFh, 083h, 0C4h, 008h, 066h, 083h, 0FEh, 001h, 075h, 00Bh, 053h
        db  0E8h, 02Bh, 03Dh, 0FDh, 0FFh, 083h, 0C4h, 004h, 0EBh, 00Eh, 066h, 083h, 0FEh, 002h, 066h, 08Bh
        db  0D3h, 075h, 004h, 066h, 0EDh, 0EBh, 001h, 0EDh, 066h, 04Fh, 055h, 0E8h, 010h, 03Dh, 0FDh, 0FFh
        db  083h, 0C4h, 004h, 0A8h, 004h, 075h, 005h, 066h, 00Bh, 0FFh, 075h, 0CDh, 08Bh, 044h, 024h, 018h
        db  066h, 00Fh, 0AFh, 0FEh, 066h, 089h, 0B8h, 01Ch, 001h, 000h, 000h, 0B8h, 001h, 000h, 000h, 000h
        db  05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 004h, 0C3h
sub_3_01A8:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        movzx  ebx, word ptr [esi + 94h]
        mov    word ptr [esi + 9ah], 1
        lea    edi, [ebx + 2]
        add    ebx, 402h
        push   edi
        db  0E8h, 0C2h, 03Ch, 0FDh, 0FFh
        add    esp, 4
        mov    cl, al
        and    cl, 3
        cmp    cl, 1
        jne    SHORT loc_3_01E7
        and    al, 0feh
        push   eax
        push   edi
        db  0E8h, 04Ah, 03Dh, 0FDh, 0FFh
        add    esp, 8
loc_3_01E7:
        push   ebx
        db  0E8h, 0A3h, 03Ch, 0FDh, 0FFh
        add    esp, 4
        and    al, 1fh
        or     al, 20h
        push   eax
        push   ebx
        db  0E8h, 033h, 03Dh, 0FDh, 0FFh
        add    esp, 8
        push   ebx
        db  0E8h, 08Ch, 03Ch, 0FDh, 0FFh
        add    esp, 4
        and    al, 3
        cmp    al, 1
        je     SHORT loc_3_0211
        sub    eax, eax
        jmp    SHORT loc_3_021F
loc_3_0211:
        mov    word ptr [esi + 9ah], 2
        mov    eax, 1
loc_3_021F:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_0223:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 14h]
        push   0f4h
        movzx  esi, word ptr [edi + 94h]
        lea    ebx, [esi + 402h]
        add    esi, 400h
        push   ebx
        db  0E8h, 0E5h, 03Ch, 0FDh, 0FFh
        add    esp, 8
        push   esi
        db  0E8h, 03Eh, 03Ch, 0FDh, 0FFh
        add    esp, 4
        mov    cl, al
        and    cl, 70h
        mov    word ptr [edi + 120h], 0
        cmp    cl, 10h
        jne    SHORT loc_3_0273
        mov    word ptr [edi + 120h], 1
        jmp    SHORT loc_3_0290
loc_3_0273:
        or     cl, cl
        jne    SHORT loc_3_0282
        mov    word ptr [edi + 120h], 2
        jmp    SHORT loc_3_0290
loc_3_0282:
        cmp    cl, 20h
        jne    SHORT loc_3_02F6
        mov    word ptr [edi + 120h], 4
loc_3_0290:
        test   al, 80h
        lea    esi, [edi + 16eh]
        je     SHORT loc_3_029F
        mov    byte ptr [esi], 80h
        jmp    SHORT loc_3_02A2
loc_3_029F:
        mov    byte ptr [esi], 0
loc_3_02A2:
        test   al, 4
        lea    ebp, [edi + 128h]
        je     SHORT loc_3_02B5
        mov    dword ptr [ebp], 0
        jmp    SHORT loc_3_02BC
loc_3_02B5:
        mov    dword ptr [ebp], 1
loc_3_02BC:
        call   sub_3_0D40
        or     eax, eax
        je     SHORT loc_3_02CC
        mov    dword ptr [ebp], 0
loc_3_02CC:
        push   edi
        call   0
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_3_02DB
        jmp    SHORT loc_3_02F6
loc_3_02DB:
        cmp    byte ptr [esi], 80h
        jne    SHORT loc_3_02E4
        push   14h
        jmp    SHORT loc_3_02E6
loc_3_02E4:
        push   34h
loc_3_02E6:
        push   ebx
        db  0E8h, 042h, 03Ch, 0FDh, 0FFh
        add    esp, 8
        mov    eax, 1
        jmp    SHORT loc_3_0303
loc_3_02F6:
        push   14h
        push   ebx
        db  0E8h, 030h, 03Ch, 0FDh, 0FFh
        add    esp, 8
        sub    eax, eax
loc_3_0303:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_0310:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        lea    esi, [edi + 0a0h]
        push   esi
        db  0E8h, 0E6h, 0FCh, 0FCh, 0FFh
        add    esp, 4
        mov    dword ptr [edi + 170h], eax
        or     eax, eax
        jne    SHORT loc_3_0333
        sub    eax, eax
        jmp    SHORT loc_3_036A
loc_3_0333:
        push   esi
        db  0E8h, 0D7h, 0FCh, 0FCh, 0FFh
        add    esp, 4
        mov    dword ptr [edi + 174h], eax
        or     eax, eax
        jne    SHORT loc_3_034A
        sub    eax, eax
        jmp    SHORT loc_3_036A
loc_3_034A:
        push   1
        push   edi
        db  068h, 0B0h, 021h, 000h, 000h
        push   eax
        push   1
        call   dword ptr [edi + 170h]
        add    esp, 14h
        mov    dword ptr [edi + 178h], eax
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
loc_3_036A:
        pop    edi
        pop    esi
        ret
sub_3_036D:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 170h]
        or     eax, eax
        je     SHORT loc_3_0392
        db  068h, 0B0h, 021h, 000h, 000h
        push   dword ptr [esi + 178h]
        push   3
        call   eax
        add    esp, 0ch
        or     byte ptr [esi + 3bh], 1
loc_3_0392:
        pop    esi
        ret
sub_3_0394:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 170h]
        or     eax, eax
        jne    SHORT loc_3_03A7
        sub    eax, eax
        jmp    SHORT loc_3_03C9
loc_3_03A7:
        xor    byte ptr [esi + 3bh], 1
        db  068h, 0B0h, 021h, 000h, 000h
        push   dword ptr [esi + 178h]
        push   2
        call   eax
        add    esp, 0ch
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        jne    SHORT loc_3_03C9
        or     byte ptr [esi + 3bh], 1
loc_3_03C9:
        pop    esi
        ret
sub_3_03CB:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    cx, word ptr [esi + 124h]
        or     cx, cx
        je     NEAR PTR loc_3_0465
        cmp    dword ptr [esi + 0f8h], 0
        jne    SHORT loc_3_0465
        mov    word ptr [esi + 0fch], cx
        lea    eax, [esi + 0fch]
        mov    word ptr [esi + 0feh], 4
        db  0C7h, 086h, 000h, 001h, 000h, 000h, 067h, 012h, 000h, 000h
        mov    dword ptr [esi + 104h], 0
        mov    dword ptr [esi + 108h], 0
        mov    dword ptr [esi + 10ch], 0
        mov    dword ptr [esi + 110h], 0
        mov    dword ptr [esi + 114h], 1f4h
        mov    dword ptr [esi + 118h], esi
        push   eax
        db  0E8h, 0BAh, 01Dh, 0FDh, 0FFh
        add    esp, 4
        mov    dword ptr [esi + 0f8h], eax
        or     eax, eax
        je     SHORT loc_3_048E
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 009h, 000h, 003h, 000h
loc_3_0465:
        mov    ax, word ptr [esi + 126h]
        or     ax, ax
        je     SHORT loc_3_048E
        cmp    dword ptr [esi + 130h], 0
        jne    SHORT loc_3_048E
        push   0
        movzx  eax, ax
        push   eax
        db  0E8h, 0BBh, 032h, 0FDh, 0FFh
        add    esp, 8
        mov    dword ptr [esi + 130h], eax
loc_3_048E:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_3_0493:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0f8h]
        or     eax, eax
        je     SHORT loc_3_04CF
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        mov    eax, dword ptr [esi + 0f8h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 011h, 000h, 003h, 000h
        mov    dword ptr [esi + 0f8h], 0
loc_3_04CF:
        mov    eax, dword ptr [esi + 130h]
        or     eax, eax
        je     SHORT loc_3_04FA
        push   eax
        db  0E8h, 0D7h, 033h, 0FDh, 0FFh
        add    esp, 4
        push   dword ptr [esi + 130h]
        db  0E8h, 069h, 032h, 0FDh, 0FFh
        add    esp, 4
        mov    dword ptr [esi + 130h], 0
loc_3_04FA:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_3_04FF:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        movzx  edi, word ptr [esi + 94h]
        inc    edi
        test   byte ptr [esi + 3bh], 1
        je     SHORT loc_3_0520
        push   esi
        call   sub_3_0394
        add    esp, 4
        or     eax, eax
        je     SHORT loc_3_0569
loc_3_0520:
        or     dword ptr [esi + 168h], 80h
        movzx  eax, byte ptr [esi + 0b2h]
        or     eax, eax
        je     SHORT loc_3_0543
        cmp    eax, 10h
        je     SHORT loc_3_056C
        cmp    eax, 80h
        je     SHORT loc_3_0555
        jmp    SHORT loc_3_055F
loc_3_0543:
        cmp    dword ptr [esi + 160h], 1
        je     SHORT loc_3_055F
        push   esi
        db  0E8h, 066h, 0FCh, 0FCh, 0FFh
        add    esp, 4
loc_3_0555:
        push   edi
        push   esi
        db  0E8h, 049h, 019h, 0FDh, 0FFh
        add    esp, 8
loc_3_055F:
        xor    dword ptr [esi + 168h], 80h
loc_3_0569:
        pop    edi
        pop    esi
        ret
loc_3_056C:
        push   esi
        db  0E8h, 034h, 006h, 0FDh, 0FFh
        add    esp, 4
        jmp    SHORT loc_3_055F
loc_3_0577:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 1ch], 1
        mov    ebp, dword ptr [esp + 38h]
        push   3
        push   ebp
        db  068h, 000h, 000h, 000h, 000h
        call   sub_3_0D10
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_3_08D0
        mov    al, byte ptr [ebp + 3]
        sub    al, 31h
        cmp    al, 3
        ja     NEAR PTR loc_3_08D0
        mov    ebx, dword ptr [esp + 30h]
        or     ebx, ebx
        je     NEAR PTR loc_3_08D0
        movzx  edi, al
        db  08Bh, 004h, 0BDh, 004h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_05D6
        cmp    dword ptr [eax + 64h], 534d5446h
        je     NEAR PTR loc_3_08D0
loc_3_05D6:
        push   1
        push   180h
        db  0E8h, 036h, 0FAh, 0FCh, 0FFh
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_3_08D0
        db  089h, 034h, 0BDh, 004h, 000h, 000h, 000h
        mov    word ptr [esi + 94h], bx
        lea    ebx, [esi + 0a0h]
        push   4
        push   ebp
        push   ebx
        db  0E8h, 0CEh, 033h, 0FDh, 0FFh
        add    esp, 0ch
        push   ebx
        db  068h, 0D8h, 008h, 000h, 000h
        mov    dword ptr [esi + 168h], 0
        push   dword ptr [esp + 34h]
        db  0E8h, 0D8h, 0F9h, 0FCh, 0FFh
        add    esp, 0ch
        push   esi
        call   sub_3_0310
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_3_064B
        mov    dword ptr [esi + 164h], 1
        or     byte ptr [esi + 3bh], 1
        jmp    NEAR PTR loc_3_08D0
loc_3_064B:
        mov    dword ptr [esi + 164h], 0
        lea    eax, [esp + 1ch]
        push   1
        push   eax
        mov    byte ptr [esi + 3bh], 0
        lea    eax, [esp + 1bh]
        push   eax
        push   3
        db  068h, 004h, 000h, 000h, 000h
        mov    ebx, dword ptr [esp + 3ch]
        push   0
        push   ebx
        db  0E8h, 00Ch, 0FAh, 0FCh, 0FFh
        add    esp, 1ch
        or     eax, eax
        jne    SHORT loc_3_068D
        cmp    byte ptr [esp + 13h], 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [esi + 168h], eax
loc_3_068D:
        or     dword ptr [esi + 168h], 80h
        mov    word ptr [esi + 98h], 1
        mov    byte ptr [esi + 0b2h], 80h
        mov    byte ptr [esi + 0b3h], 3
        lea    eax, [esp + 1ch]
        push   1
        push   eax
        mov    byte ptr [esi + 0b0h], 0ffh
        lea    eax, [esp + 1bh]
        push   eax
        push   3
        db  068h, 014h, 000h, 000h, 000h
        push   0
        push   ebx
        db  0E8h, 0B4h, 0F9h, 0FCh, 0FFh
        add    esp, 1ch
        or     eax, eax
        jne    SHORT loc_3_06E2
        cmp    byte ptr [esp + 13h], 0
        je     NEAR PTR loc_3_0816
loc_3_06E2:
        push   esi
        call   sub_3_01A8
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_3_0816
        push   2
        lea    eax, [esp + 1ch]
        push   ebx
        push   eax
        db  0E8h, 074h, 0F9h, 0FCh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_3_0816
        mov    eax, dword ptr [esp + 34h]
        or     eax, eax
        je     SHORT loc_3_071B
        lea    ebp, [esi + 124h]
        jmp    SHORT loc_3_0748
loc_3_071B:
        push   0
        push   0
        push   4
        lea    eax, [esp + 20h]
        push   dword ptr [esp + 24h]
        push   eax
        db  0E8h, 04Dh, 0F9h, 0FCh, 0FFh
        add    esp, 14h
        or     eax, eax
        jne    NEAR PTR loc_3_0816
        mov    eax, dword ptr [esp + 14h]
        lea    ebp, [esi + 124h]
        mov    ax, word ptr [eax + 2]
loc_3_0748:
        mov    word ptr [ebp], ax
        push   4
        push   ebx
        push   0
        db  0E8h, 01Eh, 0F9h, 0FCh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_3_0772
        movzx  eax, word ptr [ebp]
        push   eax
        db  0E8h, 055h, 032h, 0FDh, 0FFh
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_3_0816
loc_3_0772:
        push   0
        push   0
        push   3
        lea    eax, [esp + 20h]
        push   dword ptr [esp + 24h]
        push   eax
        db  0E8h, 0F6h, 0F8h, 0FCh, 0FFh
        add    esp, 14h
        or     eax, eax
        jne    NEAR PTR loc_3_0816
        mov    eax, dword ptr [esp + 14h]
        push   esi
        movzx  ax, byte ptr [eax + 1]
        mov    word ptr [esi + 126h], ax
        call   sub_3_0223
        add    esp, 4
        or     eax, eax
        je     SHORT loc_3_0816
        push   esi
        call   sub_3_03CB
        add    esp, 4
        cmp    dword ptr [esi + 0f8h], 0
        je     SHORT loc_3_0816
        push   1
        push   10h
        db  0E8h, 04Fh, 0F8h, 0FCh, 0FFh
        add    esp, 8
        mov    dword ptr [esi + 134h], eax
        push   1
        push   98h
        db  0E8h, 03Ah, 0F8h, 0FCh, 0FFh
        add    esp, 8
        mov    dword ptr [esi + 138h], eax
        cmp    dword ptr [esi + 134h], 0
        je     SHORT loc_3_080D
        or     eax, eax
        je     SHORT loc_3_080D
        mov    word ptr [esi + 98h], 2
        mov    byte ptr [esi + 0b2h], 40h
        mov    byte ptr [esi + 0b0h], 10h
        jmp    SHORT loc_3_0816
loc_3_080D:
        push   esi
        call   sub_3_0493
        add    esp, 4
loc_3_0816:
        db  089h, 03Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 9ch], edi
        mov    word ptr [esi], 94h
        mov    word ptr [esi + 2], 100h
        db  0C7h, 046h, 004h, 0F4h, 03Fh, 000h, 000h
        mov    dword ptr [esi + 8], 17h
        mov    dword ptr [esi + 158h], 0
        mov    dword ptr [esi + 13ch], 0
        mov    dword ptr [esi + 140h], 0
        mov    dword ptr [esi + 160h], 0
        mov    dword ptr [esi + 150h], 0
        mov    dword ptr [esi + 0ech], 0ffffffffh
        mov    dword ptr [esi + 0f0h], 0ffffffffh
        db  0FEh, 005h, 020h, 000h, 000h, 000h
        push   0
        push   esi
        db  0E8h, 02Eh, 0F8h, 0FCh, 0FFh
        add    esp, 8
        cmp    byte ptr [esi + 0b2h], 0
        jne    SHORT loc_3_08A4
        push   esi
        db  0E8h, 017h, 0F9h, 0FCh, 0FFh
        add    esp, 4
loc_3_08A4:
        cmp    byte ptr [esi + 0b0h], 10h
        jne    SHORT loc_3_08C6
        movzx  eax, word ptr [esi + 122h]
        shl    eax, 2
        mov    dword ptr [esi + 154h], eax
        push   esi
        db  0E8h, 031h, 012h, 0FDh, 0FFh
        add    esp, 4
loc_3_08C6:
        xor    dword ptr [esi + 168h], 80h
loc_3_08D0:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_3_08D8:
        mov    eax, dword ptr [esp + 4]
        movsx  eax, byte ptr [eax + 3]
        db  08Bh, 004h, 085h, 040h, 0FFh, 0FFh, 0FFh
        or     eax, eax
        jne    SHORT loc_3_08F9
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], 0fffffff6h
        sub    eax, eax
        jmp    SHORT loc_3_0921
loc_3_08F9:
        cmp    dword ptr [eax + 164h], 0
        je     SHORT loc_3_0910
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], 0fffffffeh
        sub    eax, eax
        jmp    SHORT loc_3_0921
loc_3_0910:
        mov    dword ptr [eax + 164h], 1
        mov    byte ptr [eax + 16fh], 0ffh
loc_3_0921:
        ret
loc_3_0922:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 10h]
        cmp    eax, 1
        sbb    al, al
        neg    al
        mov    byte ptr [ecx + 16ch], al
        mov    eax, dword ptr [esp + 8]
        cmp    eax, 1
        sbb    al, al
        neg    al
        mov    byte ptr [ecx + 16dh], al
        mov    dword ptr [ecx + 0ch], 0
        mov    eax, 1
        ret
loc_3_0955:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    al, byte ptr [esi + 0b2h]
        cmp    al, 40h
        je     SHORT loc_3_0968
        cmp    al, 10h
        jne    SHORT loc_3_097F
loc_3_0968:
        movzx  eax, word ptr [esi + 94h]
        add    eax, 402h
        push   64h
        push   eax
        db  0E8h, 054h, 0FBh, 0FCh, 0FFh
        add    esp, 8
loc_3_097F:
        push   dword ptr [esi + 13ch]
        db  0E8h, 0D2h, 0F6h, 0FCh, 0FFh
        push   dword ptr [esi + 140h]
        db  0E8h, 0C7h, 0F6h, 0FCh, 0FFh
        push   esi
        db  0E8h, 01Dh, 0F8h, 0FCh, 0FFh
        add    esp, 4
        cmp    byte ptr [esi + 16dh], 0
        jne    SHORT loc_3_09B9
        mov    eax, dword ptr [esi + 3ch]
        or     eax, eax
        je     SHORT loc_3_09B9
        push   0
        push   eax
        db  0E8h, 06Ah, 0F6h, 0FCh, 0FFh
        add    esp, 8
loc_3_09B9:
        push   esi
        db  0E8h, 021h, 010h, 0FDh, 0FFh
        add    esp, 4
        cmp    dword ptr [esi + 0f4h], 0
        jne    SHORT loc_3_09B9
        cmp    dword ptr [esi + 0a4h], 0
        je     SHORT loc_3_09EE
        push   0
        push   dword ptr [esi + 0a8h]
        db  0E8h, 03Fh, 0F6h, 0FCh, 0FFh
        add    esp, 8
        mov    dword ptr [esi + 0a4h], 0
loc_3_09EE:
        cmp    word ptr [esi + 98h], 2
        jne    SHORT loc_3_0A49
        cmp    dword ptr [esi + 148h], 0
        je     SHORT loc_3_0A29
        mov    ax, word ptr [esi + 15ch]
        cmp    ax, 8000h
        jne    SHORT loc_3_0A20
        push   0
        push   dword ptr [esi + 144h]
        db  0E8h, 051h, 0F6h, 0FCh, 0FFh
        add    esp, 8
        jmp    SHORT loc_3_0A29
loc_3_0A20:
        push   eax
        db  0E8h, 03Ch, 02Dh, 0FDh, 0FFh
        add    esp, 4
loc_3_0A29:
        push   0
        push   dword ptr [esi + 134h]
        db  0E8h, 0EAh, 0F5h, 0FCh, 0FFh
        add    esp, 8
        push   0
        push   dword ptr [esi + 138h]
        db  0E8h, 0DAh, 0F5h, 0FCh, 0FFh
        add    esp, 8
loc_3_0A49:
        push   esi
        call   sub_3_036D
        add    esp, 4
        push   esi
        call   sub_3_0493
        add    esp, 4
        push   0
        push   esi
        db  0E8h, 0BDh, 0F5h, 0FCh, 0FFh
        add    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 004h, 085h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 1
        pop    esi
        ret
loc_3_0A7D:
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        or     ecx, ecx
        je     SHORT loc_3_0A9B
        mov    eax, dword ptr [edx + 0f4h]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ecx + 2ch], eax
        mov    eax, dword ptr [edx + 40h]
        mov    dword ptr [ecx + 30h], eax
loc_3_0A9B:
        mov    dword ptr [edx + 0ch], 0
        mov    eax, 1
        ret
loc_3_0AA8:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 8]
        or     dword ptr [ecx + 0e0h], eax
        mov    eax, dword ptr [esp + 0ch]
        or     eax, eax
        je     SHORT loc_3_0AC1
        mov    dword ptr [ecx + 30h], eax
loc_3_0AC1:
        mov    dword ptr [ecx + 0ch], 0
        mov    eax, 1
        ret
loc_3_0ACE:
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 0ch]
        or     eax, eax
        je     SHORT loc_3_0AE2
        mov    ecx, dword ptr [edx + 0e0h]
        mov    dword ptr [eax], ecx
loc_3_0AE2:
        mov    eax, dword ptr [esp + 8]
        xor    dword ptr [edx + 0e0h], eax
        mov    dword ptr [edx + 0ch], 0
        mov    eax, 1
        ret
loc_3_0AF9:
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [edx + 0c4h], eax
        mov    ecx, dword ptr [esp + 0ch]
        or     eax, eax
        mov    dword ptr [edx + 0d4h], ecx
        jne    SHORT loc_3_0B1E
        and    dword ptr [edx + 150h], 0ffffffbfh
        jmp    SHORT loc_3_0B25
loc_3_0B1E:
        or     dword ptr [edx + 150h], 40h
loc_3_0B25:
        mov    dword ptr [edx + 0ch], 0
        mov    eax, 1
        ret
loc_3_0B32:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0ch]
        cmp    eax, -1
        je     SHORT loc_3_0B6E
        mov    ecx, dword ptr [esp + 10h]
        or     ecx, ecx
        je     SHORT loc_3_0B6E
        or     dword ptr [esi + 150h], 2
        mov    dword ptr [esi + 0ech], eax
        mov    dword ptr [esi + 0c8h], ecx
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esi + 0d8h], eax
        mov    dword ptr [esi + 0ch], 0
        jmp    SHORT loc_3_0B8A
loc_3_0B6E:
        and    dword ptr [esi + 150h], 0fffffffdh
        push   dword ptr [esi + 13ch]
        db  0E8h, 0DCh, 0F4h, 0FCh, 0FFh
        mov    dword ptr [esi + 13ch], 0
loc_3_0B8A:
        mov    eax, 1
        pop    esi
        ret
loc_3_0B91:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        cmp    dword ptr [esi + 0b8h], 0
        jne    SHORT loc_3_0BA8
        sub    eax, eax
        jmp    NEAR PTR loc_3_0C94
loc_3_0BA8:
        mov    eax, dword ptr [esi + 140h]
        or     eax, eax
        je     SHORT loc_3_0BC2
        push   eax
        db  0E8h, 0A4h, 0F4h, 0FCh, 0FFh
        mov    dword ptr [esi + 140h], 0
loc_3_0BC2:
        mov    edi, dword ptr [esp + 14h]
        cmp    edi, -1
        je     NEAR PTR loc_3_0C88
        or     edi, edi
        je     NEAR PTR loc_3_0C88
        mov    ebx, dword ptr [esp + 18h]
        or     ebx, ebx
        je     NEAR PTR loc_3_0C88
        or     dword ptr [esi + 150h], 1
        mov    dword ptr [esi + 0f0h], edi
        cmp    dword ptr [esi + 4ch], 0
        jne    SHORT loc_3_0C00
        mov    dword ptr [esi + 88h], 0
loc_3_0C00:
        cmp    byte ptr [esi + 16dh], 0
        jne    SHORT loc_3_0C29
        mov    eax, dword ptr [esi + 3ch]
        or     eax, eax
        je     SHORT loc_3_0C29
        push   0
        push   eax
        db  0E8h, 008h, 0F4h, 0FCh, 0FFh
        add    esp, 8
        mov    dword ptr [esi + 3ch], 0
        mov    byte ptr [esi + 16dh], 1
loc_3_0C29:
        push   1
        push   edi
        db  0E8h, 0E7h, 0F3h, 0FCh, 0FFh
        add    esp, 8
        mov    dword ptr [esi + 3ch], eax
        or     eax, eax
        jne    SHORT loc_3_0C46
        and    dword ptr [esi + 150h], 0fffffffeh
        sub    eax, eax
        jmp    SHORT loc_3_0C94
loc_3_0C46:
        mov    dword ptr [esi + 40h], edi
        mov    byte ptr [esi + 16dh], 0
        mov    dword ptr [esi + 4ch], 0
        mov    dword ptr [esi + 50h], 0
        mov    dword ptr [esi + 54h], 0
        mov    dword ptr [esi + 0cch], ebx
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [esi + 0dch], eax
        push   esi
        call   dword ptr [esi + 0b8h]
        add    esp, 4
        mov    dword ptr [esi + 0ch], 0
        jmp    SHORT loc_3_0C8F
loc_3_0C88:
        and    dword ptr [esi + 150h], 0fffffffeh
loc_3_0C8F:
        mov    eax, 1
loc_3_0C94:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_0C98:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0ch], 0
        mov    eax, 1
        ret
loc_3_0CA9:
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [eax], 30h
        mov    eax, dword ptr [esp + 8]
        or     eax, eax
        je     SHORT loc_3_0CCB
        push   eax
        push   ecx
        call   sub_3_0C98
        add    esp, 8
        jmp    SHORT loc_3_0CD4
loc_3_0CCB:
        mov    dword ptr [ecx + 0ch], 0ffffffebh
        sub    eax, eax
loc_3_0CD4:
        ret
loc_3_0CD5:
        mov    ecx, dword ptr [esp + 4]
        cmp    dword ptr [esp + 0ch], 30h
        je     SHORT loc_3_0CEB
        mov    dword ptr [ecx + 0ch], 0ffffffebh
        sub    eax, eax
        jmp    SHORT loc_3_0CFA
loc_3_0CEB:
        push   -1
        push   dword ptr [esp + 0ch]
        push   ecx
        db  0E8h, 0D3h, 01Bh, 0FDh, 0FFh
        add    esp, 0ch
loc_3_0CFA:
        ret
        db  000h, 000h, 000h, 000h, 000h
loc_3_0D00:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        je     NEAR PTR loc_3_0577
        ret
        db  000h, 000h, 000h
sub_3_0D10:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
loc_3_0D1E:
        xor    eax, eax
        jecxz  loc_3_0D37
        mov    al, byte ptr [esi]
        mov    ah, byte ptr [edi]
        inc    edi
        inc    esi
        dec    ecx
        or     al, 20h
        or     ah, 20h
        cmp    al, ah
        je     SHORT loc_3_0D1E
        mov    eax, 1
loc_3_0D37:
        pop    edi
        pop    esi
        leave
        ret
        db  000h, 000h, 000h, 000h, 000h
sub_3_0D40:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        mov    dword ptr [ebp - 14h], 398h
        mov    dword ptr [ebp - 10h], 26eh
        mov    dword ptr [ebp - 0ch], 15ch
        mov    dword ptr [ebp - 8], 2eh
        pushf
        cli
        sub    bl, bl
loc_3_0D69:
        movzx  eax, bl
        mov    esi, dword ptr [ebp + eax*4 - 14h]
        push   esi
        db  0E8h, 01Ah, 031h, 0FDh, 0FFh
        add    esp, 4
        mov    byte ptr [ebp - 1], al
        push   0ffh
        push   esi
        db  0E8h, 0A7h, 031h, 0FDh, 0FFh
        add    esp, 8
        push   esi
        db  0E8h, 000h, 031h, 0FDh, 0FFh
        add    esp, 4
        cmp    al, 7
        je     SHORT loc_3_0D9F
        cmp    al, 0fh
        je     SHORT loc_3_0D9F
        cmp    al, 88h
        jne    SHORT loc_3_0DBA
loc_3_0D9F:
        push   0f0h
        push   esi
        db  0E8h, 084h, 031h, 0FDh, 0FFh
        add    esp, 8
        push   esi
        db  0E8h, 0DDh, 030h, 0FDh, 0FFh
        add    esp, 4
        or     al, al
        je     SHORT loc_3_0DD7
loc_3_0DBA:
        push   dword ptr [ebp - 1]
        push   esi
        db  0E8h, 06Bh, 031h, 0FDh, 0FFh
        add    esp, 8
        inc    bl
        cmp    bl, 2
        jb     SHORT loc_3_0D69
        popf
        sub    eax, eax
loc_3_0DD1:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_0DD7:
        push   dword ptr [ebp - 1]
        movzx  eax, bl
        push   dword ptr [ebp + eax*4 - 14h]
        db  0E8h, 048h, 031h, 0FDh, 0FFh
        add    esp, 8
        popf
        mov    eax, 1
        jmp    SHORT loc_3_0DD1
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_0E00:
        push   ebp
        mov    ebp, esp
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        pop    ebp
        ret
_PTEXT ends
        end
