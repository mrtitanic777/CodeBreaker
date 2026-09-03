; eisa_pcode_03.asm -- EISA object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  056h, 08Bh, 0F1h, 083h, 03Dh, 008h, 000h, 000h, 000h, 000h, 074h, 007h, 0B8h, 013h, 000h, 000h
        db  000h, 0EBh, 05Ah, 06Ah, 000h, 068h, 000h, 000h, 000h, 000h, 0E8h, 0C9h, 00Eh, 000h, 000h, 083h
        db  0C4h, 008h, 00Bh, 0C0h, 074h, 007h, 0B8h, 002h, 000h, 000h, 000h, 0EBh, 040h, 089h, 035h, 008h
        db  000h, 000h, 000h, 06Ah, 000h, 06Ah, 004h, 068h, 0D9h, 0FFh, 00Fh, 000h, 0E8h, 017h, 00Eh, 000h
        db  000h, 083h, 0C4h, 00Ch, 06Ah, 001h, 068h, 0F9h, 00Ah, 000h, 000h, 056h, 0E8h, 06Fh, 00Eh, 000h
        db  000h, 083h, 0C4h, 00Ch, 00Bh, 0C0h, 075h, 015h, 06Ah, 002h, 06Ah, 000h, 06Ah, 000h, 056h, 0E8h
        db  054h, 00Eh, 000h, 000h, 083h, 0C4h, 010h, 00Bh, 0C0h, 075h, 002h, 02Bh, 0C0h, 05Eh, 0C3h, 000h
sub_3_0070:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 18h], 1
        lea    eax, [esp + 18h]
        push   1
        push   eax
        db  068h, 08Dh, 000h, 000h, 000h
        push   3
        db  068h, 09Ch, 000h, 000h, 000h
        push   0
        db  0FFh, 035h, 008h, 000h, 000h, 000h
        call   sub_3_0F30
        add    esp, 1ch
        or     eax, eax
        je     SHORT loc_3_00D0
        db  068h, 0B0h, 000h, 000h, 000h
        push   0
        call   sub_3_0E78
        or     eax, eax
        jne    SHORT loc_3_00C6
        db  068h, 0BCh, 000h, 000h, 000h
        push   0
        call   sub_3_0E78
        or     eax, eax
        je     SHORT loc_3_00D0
loc_3_00C6:
        db  0C7h, 005h, 08Dh, 000h, 000h, 000h, 001h, 000h, 000h, 000h
loc_3_00D0:
        call   sub_3_0BFC
        or     eax, eax
        je     NEAR PTR loc_3_0227
        db  08Bh, 035h, 004h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_3_00FA
loc_3_00E7:
        mov    eax, esi
        mov    esi, dword ptr [esi]
        push   0
        push   eax
        call   sub_3_0E68
        add    esp, 8
        or     esi, esi
        jne    SHORT loc_3_00E7
loc_3_00FA:
        sub    ebp, ebp
        db  089h, 02Dh, 004h, 000h, 000h, 000h
        mov    byte ptr [esp + 12h], 1
loc_3_0107:
        lea    eax, [esp + 13h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   dword ptr [esp + 1ah]
        call   sub_3_0CDE
        mov    byte ptr [esp + 1dh], al
        add    esp, 0ch
        or     al, al
        je     NEAR PTR loc_3_0213
        movzx  eax, byte ptr [esp + 11h]
        push   200h
        imul   eax, eax, 140h
        add    eax, 0bh
        push   eax
        call   sub_3_0E60
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_3_0222
        mov    edx, eax
        db  089h, 015h, 004h, 000h, 000h, 000h
        mov    dword ptr [eax], ebp
        mov    cl, byte ptr [esp + 12h]
        mov    byte ptr [eax + 9], cl
        sub    bl, bl
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [eax + 4], ecx
        mov    cl, byte ptr [esp + 13h]
        mov    byte ptr [eax + 8], cl
        mov    byte ptr [esp + 10h], 0
        cmp    byte ptr [esp + 11h], bl
        je     SHORT loc_3_01B4
loc_3_0179:
        push   ebx
        push   dword ptr [esp + 16h]
        call   sub_3_0D78
        add    esp, 8
        or     eax, eax
        db  08Bh, 015h, 004h, 000h, 000h, 000h
        je     SHORT loc_3_01AC
        movzx  ecx, byte ptr [esp + 10h]
        imul   ecx, ecx, 140h
        lea    edi, [edx + ecx + 0bh]
        mov    esi, eax
        mov    ecx, 50h
        rep movsd
        inc    byte ptr [esp + 10h]
loc_3_01AC:
        inc    bl
        cmp    bl, byte ptr [esp + 11h]
        jb     SHORT loc_3_0179
loc_3_01B4:
        mov    al, byte ptr [esp + 10h]
        cmp    al, byte ptr [esp + 11h]
        je     SHORT loc_3_0204
        or     al, al
        db  089h, 015h, 004h, 000h, 000h, 000h
        jne    SHORT loc_3_01DB
        push   0
        push   edx
        call   sub_3_0E68
        add    esp, 8
        db  089h, 02Dh, 004h, 000h, 000h, 000h
        jmp    SHORT loc_3_0213
loc_3_01DB:
        movzx  eax, byte ptr [esp + 10h]
        push   201h
        imul   eax, eax, 140h
        add    eax, 0bh
        push   eax
        push   edx
        call   sub_3_0E70
        add    esp, 0ch
        or     eax, eax
        db  08Bh, 015h, 004h, 000h, 000h, 000h
        je     SHORT loc_3_0204
        mov    edx, eax
loc_3_0204:
        db  089h, 015h, 004h, 000h, 000h, 000h
        mov    ebp, edx
        mov    al, byte ptr [esp + 10h]
        mov    byte ptr [edx + 0ah], al
loc_3_0213:
        inc    byte ptr [esp + 12h]
        cmp    byte ptr [esp + 12h], 10h
        jb     NEAR PTR loc_3_0107
loc_3_0222:
        call   sub_3_0C77
loc_3_0227:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_3_022F:
        sub    esp, 10ch
        push   esi
        push   edi
        mov    edi, edx
        mov    dword ptr [esp + 8], 100h
        lea    eax, [esp + 8]
        push   0
        push   eax
        mov    esi, ecx
        lea    eax, [esp + 1ch]
        push   eax
        push   1
        db  068h, 0C8h, 000h, 000h, 000h
        push   0
        push   esi
        call   sub_3_0F30
        add    esp, 1ch
        or     eax, eax
        je     SHORT loc_3_0297
        movzx  eax, byte ptr [edi + 9]
        push   eax
        lea    eax, [esp + 18h]
        db  068h, 024h, 000h, 000h, 000h
        push   eax
        call   sub_3_0E90
        add    esp, 0ch
        push   0
        lea    eax, [esp + 18h]
        push   0
        push   eax
        push   1
        db  068h, 0C8h, 000h, 000h, 000h
        push   0
        push   esi
        call   sub_3_0F38
        add    esp, 1ch
loc_3_0297:
        cmp    byte ptr [edi + 0ah], 0
        je     SHORT loc_3_02DB
        mov    cx, word ptr [edi + 11h]
        or     cx, cx
        je     SHORT loc_3_02DB
        movzx  eax, cl
        push   eax
        movzx  eax, ch
        push   eax
        lea    eax, [esp + 14h]
        db  068h, 0D4h, 000h, 000h, 000h
        push   eax
        call   sub_3_0E90
        add    esp, 10h
        push   0
        lea    eax, [esp + 10h]
        push   0
        push   eax
        push   1
        db  068h, 0DCh, 000h, 000h, 000h
        push   0
        push   esi
        call   sub_3_0F38
        add    esp, 1ch
loc_3_02DB:
        pop    edi
        pop    esi
        add    esp, 10ch
        ret
loc_3_02E4:
        sub    esp, 10h
        push   edi
        lea    edi, [esp + 4]
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        mov    edx, dword ptr [esp + 18h]
        mov    word ptr [esp + 6], 14h
        movzx  eax, byte ptr [edx + 3]
        movzx  ecx, byte ptr [edx + 4]
        shl    ecx, 8
        add    ecx, eax
        shl    ecx, 8
        movzx  eax, byte ptr [edx + 2]
        add    ecx, eax
        shl    ecx, 8
        mov    dword ptr [esp + 8], ecx
        movzx  eax, word ptr [edx + 5]
        shl    eax, 0ah
        add    eax, ecx
        dec    eax
        mov    dword ptr [esp + 0ch], eax
        mov    al, byte ptr [edx]
        and    al, 1
        cmp    al, 1
        push   0
        sbb    eax, eax
        inc    eax
        mov    word ptr [esp + 14h], ax
        lea    eax, [esp + 8]
        push   10h
        push   eax
        db  0A1h, 020h, 000h, 000h, 000h
        or     eax, 1
        push   eax
        db  0FFh, 035h, 01Ch, 000h, 000h, 000h
        push   0
        call   sub_3_0EE0
        add    esp, 18h
        pop    edi
        add    esp, 10h
        ret
loc_3_035D:
        sub    esp, 8
        push   edi
        lea    edi, [esp + 4]
        sub    eax, eax
        stosd
        stosd
        mov    eax, dword ptr [esp + 10h]
        mov    word ptr [esp + 4], 1
        mov    al, byte ptr [eax]
        and    ax, 0fh
        mov    word ptr [esp + 6], ax
        push   0
        lea    eax, [esp + 8]
        push   8
        push   eax
        db  0A1h, 020h, 000h, 000h, 000h
        or     eax, 4
        push   eax
        db  0FFh, 035h, 01Ch, 000h, 000h, 000h
        push   0
        call   sub_3_0EE0
        add    esp, 18h
        pop    edi
        add    esp, 8
        ret
sub_3_03A6:
        db  0C7h, 005h, 040h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  083h, 03Dh, 03Ch, 000h, 000h, 000h, 000h
        jne    SHORT loc_3_03D6
        movzx  edx, word ptr [ecx + 1]
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        mov    al, byte ptr [ecx]
        and    eax, 1fh
        add    eax, edx
        push   eax
        push   edx
        call   sub_3_0EF0
        add    esp, 10h
loc_3_03D6:
        ret
loc_3_03D7:
        sub    esp, 18h
        push   esi
        push   edi
        lea    edi, [esp + 14h]
        sub    eax, eax
        stosd
        stosd
        stosd
        mov    eax, dword ptr [esp + 24h]
        mov    word ptr [esp + 16h], 0ch
        movzx  esi, word ptr [eax + 1]
        mov    al, byte ptr [eax]
        and    eax, 1fh
        lea    edi, [esi + eax]
        db  083h, 03Dh, 03Ch, 000h, 000h, 000h, 000h
        je     SHORT loc_3_040F
        mov    dword ptr [esp + 8], esi
        mov    dword ptr [esp + 0ch], edi
        jmp    SHORT loc_3_0479
loc_3_040F:
        lea    eax, [esp + 10h]
        push   0
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_3_0F08
        add    esp, 14h
        or     eax, eax
        jne    NEAR PTR loc_3_04C8
loc_3_0436:
        cmp    dword ptr [esp + 8], esi
        ja     SHORT loc_3_0442
        cmp    dword ptr [esp + 0ch], edi
        jae    SHORT loc_3_0461
loc_3_0442:
        lea    eax, [esp + 0ch]
        push   0
        push   eax
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_3_0F10
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_3_0436
        jmp    SHORT loc_3_04C8
loc_3_0461:
        push   0
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        call   sub_3_0EF8
        add    esp, 10h
loc_3_0479:
        mov    eax, dword ptr [esp + 8]
        cmp    dword ptr [esp + 8], 400h
        mov    word ptr [esp + 18h], ax
        mov    eax, dword ptr [esp + 0ch]
        mov    word ptr [esp + 1ah], ax
        jae    SHORT loc_3_04A1
        mov    byte ptr [esp + 1eh], 4
        mov    byte ptr [esp + 1fh], 3
        jmp    SHORT loc_3_04A6
loc_3_04A1:
        mov    byte ptr [esp + 1eh], 0ffh
loc_3_04A6:
        push   0
        lea    eax, [esp + 18h]
        push   0ch
        push   eax
        db  0A1h, 020h, 000h, 000h, 000h
        or     eax, 2
        push   eax
        db  0FFh, 035h, 01Ch, 000h, 000h, 000h
        push   0
        call   sub_3_0EE0
        add    esp, 18h
loc_3_04C8:
        pop    edi
        pop    esi
        add    esp, 18h
        ret
loc_3_04CE:
        sub    esp, 4
        push   edi
        lea    edi, [esp + 4]
        sub    eax, eax
        stosd
        mov    eax, dword ptr [esp + 0ch]
        push   0
        mov    al, byte ptr [eax]
        and    al, 7
        mov    byte ptr [esp + 9], al
        lea    eax, [esp + 8]
        push   4
        push   eax
        db  0A1h, 020h, 000h, 000h, 000h
        or     eax, 3
        push   eax
        db  0FFh, 035h, 01Ch, 000h, 000h, 000h
        push   0
        call   sub_3_0EE0
        add    esp, 18h
        pop    edi
        add    esp, 4
        ret
sub_3_050C:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    byte ptr [esp + 11h], dl
        mov    eax, ecx
        mov    dword ptr [esp + 14h], ecx
        mov    al, byte ptr [eax + 22h]
        mov    byte ptr [esp + 13h], al
        sub    bl, bl
        test   al, 40h
        jne    NEAR PTR loc_3_05C3
        and    al, 80h
        cmp    al, 1
        cmc
        sbb    eax, eax
        and    eax, 8000h
        db  0A3h, 020h, 000h, 000h, 000h
        mov    byte ptr [esp + 12h], bl
        mov    ebp, dword ptr [esp + 20h]
loc_3_0547:
        shr    byte ptr [esp + 13h], 1
        test   byte ptr [esp + 13h], 1
        je     SHORT loc_3_05B8
        movzx  eax, byte ptr [esp + 12h]
        lea    esi, [eax*4]
        mov    byte ptr [esp + 10h], 0
        db  08Bh, 08Eh, 044h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 14h]
        lea    edi, [ecx + eax + 73h]
loc_3_0571:
        cmp    byte ptr [esp + 11h], bl
        ja     SHORT loc_3_057B
        sub    eax, eax
        jmp    SHORT loc_3_05A7
loc_3_057B:
        mov    eax, 1
        mov    cl, bl
        shl    eax, cl
        test   ebp, eax
        jne    SHORT loc_3_0592
        push   edi
        db  0FFh, 096h, 074h, 000h, 000h, 000h
        add    esp, 4
loc_3_0592:
        inc    bl
        mov    al, byte ptr [edi]
        and    al, 80h
        cmp    al, 1
        sbb    eax, eax
        inc    eax
        db  003h, 0BEh, 054h, 000h, 000h, 000h
        inc    byte ptr [esp + 10h]
loc_3_05A7:
        or     eax, eax
        je     SHORT loc_3_05B8
        movzx  eax, byte ptr [esp + 10h]
        db  03Bh, 086h, 064h, 000h, 000h, 000h
        jb     SHORT loc_3_0571
loc_3_05B8:
        inc    byte ptr [esp + 12h]
        cmp    byte ptr [esp + 12h], 4
        jb     SHORT loc_3_0547
loc_3_05C3:
        cmp    byte ptr [esp + 11h], 0ffh
        je     SHORT loc_3_05FB
        cmp    byte ptr [esp + 11h], bl
        jbe    SHORT loc_3_05FB
        mov    al, byte ptr [esp + 11h]
        sub    al, bl
        mov    byte ptr [esp + 18h], al
loc_3_05DA:
        push   0
        push   0
        push   0
        push   8000h
        db  0FFh, 035h, 01Ch, 000h, 000h, 000h
        push   0
        call   sub_3_0EE0
        add    esp, 18h
        dec    byte ptr [esp + 18h]
        jne    SHORT loc_3_05DA
loc_3_05FB:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret    4
sub_3_0605:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    byte ptr [esp + 12h], dl
        mov    eax, ecx
        mov    dword ptr [esp + 14h], ecx
        mov    al, byte ptr [eax + 22h]
        mov    byte ptr [esp + 13h], al
        sub    bl, bl
        test   al, 40h
        jne    NEAR PTR loc_3_06B3
        mov    byte ptr [esp + 11h], bl
        mov    ebp, dword ptr [esp + 1ch]
loc_3_062F:
        shr    byte ptr [esp + 13h], 1
        test   byte ptr [esp + 13h], 1
        je     SHORT loc_3_06A4
        movzx  eax, byte ptr [esp + 11h]
        lea    esi, [eax*4]
        mov    byte ptr [esp + 10h], 0
        db  08Bh, 08Eh, 044h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 14h]
        lea    edi, [ecx + eax + 73h]
loc_3_0659:
        cmp    bl, byte ptr [esp + 12h]
        jb     SHORT loc_3_0663
        sub    eax, eax
        jmp    SHORT loc_3_0693
loc_3_0663:
        mov    eax, 1
        mov    cl, bl
        shl    eax, cl
        test   ebp, eax
        jne    SHORT loc_3_067E
        cmp    byte ptr [esp + 11h], 3
        jne    SHORT loc_3_067E
        mov    ecx, edi
        call   sub_3_03A6
loc_3_067E:
        inc    bl
        mov    al, byte ptr [edi]
        and    al, 80h
        cmp    al, 1
        sbb    eax, eax
        inc    eax
        db  003h, 0BEh, 054h, 000h, 000h, 000h
        inc    byte ptr [esp + 10h]
loc_3_0693:
        or     eax, eax
        je     SHORT loc_3_06A4
        movzx  eax, byte ptr [esp + 10h]
        db  039h, 086h, 064h, 000h, 000h, 000h
        ja     SHORT loc_3_0659
loc_3_06A4:
        inc    byte ptr [esp + 11h]
        cmp    byte ptr [esp + 11h], 4
        jb     NEAR PTR loc_3_062F
loc_3_06B3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    4
sub_3_06BD:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [esp + 0dh], dl
        db  0BEh, 012h, 000h, 000h, 000h
        mov    dword ptr [esp + 10h], ecx
        mov    al, ch
        mov    ah, cl
        mov    word ptr [esp + 0eh], ax
        mov    edx, 1
loc_3_06DE:
        mov    bx, word ptr [esp + 0eh]
        mov    al, 3
        sub    al, dl
        mov    cl, 5
        imul   cl
        mov    cl, al
        shr    bx, cl
        and    bl, 1fh
        add    bl, 40h
        mov    byte ptr [esi], bl
        inc    esi
        inc    edx
        cmp    edx, 3
        jbe    SHORT loc_3_06DE
        sub    edi, edi
loc_3_0701:
        mov    eax, dword ptr [esp + 10h]
        lea    ecx, [edi*8 + 10h]
        shr    eax, cl
        and    eax, 0ffh
        push   eax
        db  068h, 0E8h, 000h, 000h, 000h
        push   esi
        call   sub_3_0E90
        add    esp, 0ch
        add    esi, 2
        inc    edi
        cmp    edi, 2
        jb     SHORT loc_3_0701
        movzx  eax, byte ptr [esp + 0dh]
        push   eax
        db  068h, 0ECh, 000h, 000h, 000h
        push   esi
        call   sub_3_0E90
        add    esp, 0ch
        db  0B8h, 00Ch, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_3_074B:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, ecx
        mov    word ptr [esp + 20h], 0c00h
        mov    word ptr [esp + 22h], 0cffh
        lea    edi, [esp + 24h]
        sub    eax, eax
        stosd
        stosd
        stosd
        mov    dword ptr [esp + 10h], 10h
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 28h]
        push   eax
        push   3
        db  068h, 0F4h, 000h, 000h, 000h
        push   0
        push   edx
        call   sub_3_0F30
        add    esp, 1ch
        or     eax, eax
        jne    SHORT loc_3_079D
        mov    eax, dword ptr [esp + 10h]
        shr    eax, 2
        jmp    SHORT loc_3_07A9
loc_3_079D:
        db  0A1h, 040h, 000h, 000h, 000h
        cmp    eax, 1
        sbb    al, al
        neg    al
loc_3_07A9:
        or     al, al
        je     SHORT loc_3_080E
        lea    ebp, [esp + 20h]
        movzx  ebx, al
loc_3_07B4:
        lea    edi, [esp + 14h]
        sub    eax, eax
        stosd
        stosd
        stosd
        mov    word ptr [esp + 16h], 0ch
        movzx  ax, byte ptr [esi + 9]
        shl    ax, 0ch
        add    ax, word ptr [ebp]
        mov    word ptr [esp + 18h], ax
        movzx  ax, byte ptr [esi + 9]
        shl    ax, 0ch
        add    ax, word ptr [ebp + 2]
        mov    word ptr [esp + 1ah], ax
        mov    byte ptr [esp + 1eh], 0ffh
        push   0
        lea    eax, [esp + 18h]
        push   0ch
        add    ebp, 4
        push   eax
        push   2
        db  0FFh, 035h, 01Ch, 000h, 000h, 000h
        push   0
        call   sub_3_0EE0
        add    esp, 18h
        dec    ebx
        jne    SHORT loc_3_07B4
loc_3_080E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
sub_3_0816:
        sub    esp, 514h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 14h], edx
        mov    dword ptr [esp + 18h], ecx
        mov    dword ptr [esp + 20h], 0
        push   3
        push   1
        push   dword ptr [esp + 1ch]
        db  068h, 01Ch, 000h, 000h, 000h
        call   sub_3_0EC8
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_3_09DA
        mov    dword ptr [esp + 10h], 4
        lea    eax, [esp + 10h]
        push   0
        push   eax
        db  0C7h, 005h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        lea    eax, [esp + 28h]
        push   eax
        push   3
        db  068h, 000h, 001h, 000h, 000h
        push   0
        push   dword ptr [esp + 2ch]
        call   sub_3_0F30
        mov    eax, dword ptr [esp + 3ch]
        add    esp, 1ch
        and    eax, 1
        db  0A3h, 03Ch, 000h, 000h, 000h
        mov    ebp, dword ptr [esp + 20h]
        and    ebp, 2
        sub    edx, edx
        lea    edi, [esp + 24h]
        mov    eax, 0ffffffffh
        mov    ecx, 40h
        rep stosd
        sub    eax, eax
loc_3_08A7:
        mov    dword ptr [esp + edx*4 + 124h], eax
        inc    edx
        cmp    edx, 100h
        jb     SHORT loc_3_08A7
        mov    dword ptr [esp + 10h], 100h
        lea    eax, [esp + 10h]
        push   0
        push   eax
        lea    eax, [esp + 2ch]
        push   eax
        push   3
        db  068h, 00Ch, 001h, 000h, 000h
        push   0
        push   dword ptr [esp + 2ch]
        call   sub_3_0F30
        add    esp, 1ch
        or     eax, eax
        jne    SHORT loc_3_090F
        cmp    dword ptr [esp + 10h], 100h
        jae    SHORT loc_3_090F
        mov    edx, 100h
        sub    edx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 10h]
        lea    edi, [esp + eax + 24h]
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
loc_3_090F:
        mov    dword ptr [esp + 10h], 400h
        lea    eax, [esp + 10h]
        push   0
        push   eax
        lea    eax, [esp + 12ch]
        push   eax
        push   3
        db  068h, 01Ch, 001h, 000h, 000h
        push   0
        push   dword ptr [esp + 2ch]
        call   sub_3_0F30
        add    esp, 1ch
        push   0
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        push   -1
        push   0
        call   sub_3_0EF8
        mov    edi, dword ptr [esp + 28h]
        add    esp, 10h
        add    edi, 0bh
        mov    ebx, edi
        mov    eax, dword ptr [esp + 18h]
        sub    esi, esi
        mov    al, byte ptr [eax + 0ah]
        or     al, al
        je     SHORT loc_3_098B
        movzx  eax, al
        mov    dword ptr [esp + 1ch], eax
loc_3_096C:
        push   dword ptr [esp + esi*4 + 124h]
        mov    edx, dword ptr [esp + esi + 28h]
        mov    ecx, ebx
        call   sub_3_0605
        add    ebx, 140h
        inc    esi
        cmp    esi, dword ptr [esp + 1ch]
        jb     SHORT loc_3_096C
loc_3_098B:
        or     ebp, ebp
        je     SHORT loc_3_099C
        mov    edx, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 18h]
        call   sub_3_074B
loc_3_099C:
        mov    eax, dword ptr [esp + 18h]
        sub    esi, esi
        mov    al, byte ptr [eax + 0ah]
        or     al, al
        je     SHORT loc_3_09C9
        movzx  ebx, al
loc_3_09AC:
        push   dword ptr [esp + esi*4 + 124h]
        mov    edx, dword ptr [esp + esi + 28h]
        mov    ecx, edi
        call   sub_3_050C
        add    edi, 140h
        inc    esi
        cmp    esi, ebx
        jb     SHORT loc_3_09AC
loc_3_09C9:
        or     ebp, ebp
        jne    SHORT loc_3_09DA
        mov    edx, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 18h]
        call   sub_3_074B
loc_3_09DA:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 514h
        ret
sub_3_09E5:
        sub    esp, 8
        push   ebx
        push   esi
        call   sub_3_0070
        push   0
        db  0FFh, 035h, 008h, 000h, 000h, 000h
        db  068h, 030h, 001h, 000h, 000h
        push   0
        call   sub_3_0EB0
        add    esp, 10h
        db  08Bh, 035h, 004h, 000h, 000h, 000h
        or     esi, esi
        je     NEAR PTR loc_3_0AF3
        mov    ebx, 40h
loc_3_0A19:
        test   byte ptr [esi + 8], bl
        jne    NEAR PTR loc_3_0AE9
        mov    ecx, dword ptr [esi + 4]
        cmp    cx, 6126h
        je     NEAR PTR loc_3_0AE9
        cmp    ecx, 40026356h
        je     NEAR PTR loc_3_0AE9
        cmp    cx, 8906h
        je     NEAR PTR loc_3_0AE9
        cmp    ecx, 120835ch
        je     NEAR PTR loc_3_0AE9
        cmp    cx, 9004h
        jne    SHORT loc_3_0A7A
        mov    eax, ecx
        shr    eax, 10h
        or     ax, ax
        je     NEAR PTR loc_3_0AE9
        cmp    ax, 100h
        je     SHORT loc_3_0AE9
        cmp    ax, 200h
        je     SHORT loc_3_0AE9
        cmp    ax, 4
        je     SHORT loc_3_0AE9
loc_3_0A7A:
        mov    dl, byte ptr [esi + 9]
        call   sub_3_06BD
        push   0
        db  0FFh, 035h, 008h, 000h, 000h, 000h
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        call   sub_3_0EB0
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_3_0AE9
        mov    al, byte ptr [esi + 9]
        mov    byte ptr [esp + 0ch], al
        mov    byte ptr [esp + 0dh], 0
        movzx  ax, byte ptr [esi + 2dh]
        mov    word ptr [esp + 0eh], ax
        lea    eax, [esp + 0ch]
        push   0
        push   eax
        push   4
        push   2
        push   dword ptr [esp + 18h]
        call   sub_3_0F40
        add    esp, 14h
        push   0
        push   esi
        db  0FFh, 035h, 008h, 000h, 000h, 000h
        push   dword ptr [esp + 14h]
        call   sub_3_0EA8
        mov    edx, dword ptr [esp + 18h]
        add    esp, 10h
        mov    ecx, esi
        call   sub_3_0816
loc_3_0AE9:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    NEAR PTR loc_3_0A19
loc_3_0AF3:
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_3_0AF9:
        sub    esp, 0ch
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 24h]
        mov    edi, dword ptr [esp + 20h]
        cmp    esi, edi
        jne    SHORT loc_3_0B61
        mov    eax, dword ptr [esp + 18h]
        cmp    eax, 4
        je     SHORT loc_3_0B22
        cmp    eax, 5
        je     SHORT loc_3_0B37
        mov    eax, 1
        jmp    NEAR PTR loc_3_0BF3
loc_3_0B22:
        push   0
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_3_0F00
        add    esp, 8
        jmp    NEAR PTR loc_3_0BF1
loc_3_0B37:
        push   0
        db  0FFh, 035h, 008h, 000h, 000h, 000h
        call   sub_3_0F20
        add    esp, 8
        call   sub_3_09E5
        push   0
        db  0FFh, 035h, 008h, 000h, 000h, 000h
        call   sub_3_0F28
        add    esp, 8
        jmp    NEAR PTR loc_3_0BF1
loc_3_0B61:
        push   0
        lea    eax, [esp + 14h]
        push   esi
        push   eax
        call   sub_3_0E98
        add    esp, 0ch
        cmp    dword ptr [esp + 10h], edi
        je     SHORT loc_3_0B7E
        mov    eax, 1
        jmp    SHORT loc_3_0BF3
loc_3_0B7E:
        push   0
        db  0FFh, 035h, 008h, 000h, 000h, 000h
        lea    eax, [esp + 10h]
        push   esi
        push   eax
        call   sub_3_0EA0
        add    esp, 10h
        cmp    dword ptr [esp + 8], 0
        jne    SHORT loc_3_0BA2
        mov    eax, 1
        jmp    SHORT loc_3_0BF3
loc_3_0BA2:
        mov    eax, dword ptr [esp + 18h]
        cmp    eax, 6
        je     SHORT loc_3_0BB7
        cmp    eax, 0fh
        je     SHORT loc_3_0BC4
        mov    eax, 1
        jmp    SHORT loc_3_0BF3
loc_3_0BB7:
        mov    edx, dword ptr [esp + 8]
        mov    ecx, esi
        call   sub_3_022F
        jmp    SHORT loc_3_0BF1
loc_3_0BC4:
        push   3
        lea    eax, [esp + 10h]
        push   esi
        push   eax
        call   sub_3_0ED8
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_3_0BE6
        push   0
        push   dword ptr [esp + 10h]
        call   sub_3_0ED0
        add    esp, 8
loc_3_0BE6:
        mov    edx, esi
        mov    ecx, dword ptr [esp + 8]
        call   sub_3_0816
loc_3_0BF1:
        sub    eax, eax
loc_3_0BF3:
        pop    edi
        pop    esi
        add    esp, 0ch
        ret
        db  000h, 000h, 000h
sub_3_0BFC:
        db  0CDh, 020h, 011h, 001h, 001h, 000h
        cmp    eax, 20000000h
        jae    SHORT loc_3_0C40
        push   0
        push   740h
        db  0CDh, 020h, 0A9h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     SHORT loc_3_0C3F
        db  0C6h, 005h, 08Ch, 000h, 000h, 000h, 001h
        shr    eax, 4
loc_3_0C26:
        db  066h, 0A3h, 088h, 000h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    eax, dword ptr [ebx + 8]
        db  0A3h, 084h, 000h, 000h, 000h
        mov    eax, 1
loc_3_0C3F:
        ret
loc_3_0C40:
        push   740h
        mov    eax, esp
        push   eax
        push   4
        push   0b8h
        db  068h, 091h, 000h, 000h, 000h
        db  0CDh, 020h, 018h, 000h, 017h, 000h
        add    esp, 10h
        add    esp, 4
        or     ax, ax
        je     SHORT loc_3_0C3F
        db  066h, 0A3h, 08Ah, 000h, 000h, 000h
        shr    eax, 10h
        db  0C6h, 005h, 08Ch, 000h, 000h, 000h, 000h
        jmp    SHORT loc_3_0C26
sub_3_0C77:
        db  080h, 03Dh, 08Ch, 000h, 000h, 000h, 001h
        je     SHORT loc_3_0CA3
        push   ax
        db  066h, 0FFh, 035h, 08Ah, 000h, 000h, 000h
        mov    eax, esp
        push   eax
        push   2
        push   0b9h
        db  068h, 091h, 000h, 000h, 000h
        db  0CDh, 020h, 018h, 000h, 017h, 000h
        add    esp, 10h
        pop    eax
        ret
loc_3_0CA3:
        db  0CDh, 020h, 0AAh, 000h, 001h, 000h
        ret
sub_3_0CAA:
        db  083h, 03Dh, 08Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_3_0CD2
        mov    ax, word ptr [ebp + 2ch]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    cx, 0f000h
        mov    dx, 0f859h
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        ret
loc_3_0CD2:
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        ret
sub_3_0CDE:
        push   ebp
        push   ebx
        push   esi
        push   edi
        db  08Bh, 02Dh, 084h, 000h, 000h, 000h
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 0d800h
        mov    al, byte ptr [esp + 80h]
        mov    byte ptr [ebp + 18h], al
        db  066h, 0A1h, 088h, 000h, 000h, 000h
        mov    word ptr [ebp + 34h], ax
        mov    word ptr [ebp + 30h], 600h
        call   sub_3_0CAA
        xor    al, al
        test   dword ptr [ebp + 2ch], 1
        jne    SHORT loc_3_0D5E
        mov    bx, word ptr [ebp + 4]
        shl    ebx, 10h
        mov    bx, word ptr [ebp]
        cmp    ebx, 1044110eh
        je     SHORT loc_3_0D46
        test   byte ptr [ebp + 1ch], 30h
        jne    SHORT loc_3_0D5E
loc_3_0D46:
        mov    ecx, dword ptr [esp + 84h]
        mov    dword ptr [ecx], ebx
        mov    ecx, dword ptr [esp + 88h]
        mov    al, byte ptr [ebp + 1ch]
        mov    byte ptr [ecx], al
        mov    al, byte ptr [ebp + 15h]
loc_3_0D5E:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_3_0D78:
        push   ebp
        push   ebx
        push   esi
        push   edi
        db  08Bh, 02Dh, 084h, 000h, 000h, 000h
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 0d801h
        mov    al, byte ptr [esp + 80h]
        mov    ah, byte ptr [esp + 84h]
        mov    word ptr [ebp + 18h], ax
        db  066h, 0A1h, 088h, 000h, 000h, 000h
        mov    word ptr [ebp + 34h], ax
        mov    word ptr [ebp + 30h], 600h
        mov    word ptr [ebp + 3ch], ax
        mov    word ptr [ebp + 4], 600h
        call   sub_3_0CAA
        xor    eax, eax
        test   dword ptr [ebp + 2ch], 1
        jne    SHORT loc_3_0DE9
        xor    eax, eax
        db  066h, 0A1h, 088h, 000h, 000h, 000h
        shl    eax, 4
        add    eax, 600h
loc_3_0DE9:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_0E10:
        push   0
        db  0C7h, 005h, 098h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        push   dword ptr [esp + 8]
        call   sub_3_0F18
        add    esp, 8
        mov    eax, 1
        ret    4
sub_3_0E30:
        cmp    dword ptr [esp + 8], 1
        je     SHORT loc_3_0E3E
        mov    eax, 1
        jmp    SHORT loc_3_0E55
loc_3_0E3E:
        db  083h, 03Dh, 098h, 000h, 000h, 000h, 000h
        mov    eax, 21h
        je     SHORT loc_3_0E55
        mov    ecx, dword ptr [esp + 4]
        call   0
loc_3_0E55:
        ret    8
        db  0CDh, 020h, 06Ch, 080h, 001h, 000h, 000h, 000h
sub_3_0E60:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_3_0E68:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_3_0E70:
        db  0CDh, 020h, 050h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_3_0E78:
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        movzx  eax, word ptr [esp + 8]
        db  0CDh, 020h, 046h, 001h, 001h, 000h
        mov    eax, ecx
        pop    edi
        ret    8
        db  000h, 000h
sub_3_0E90:
        db  0CDh, 020h, 074h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_3_0E98:
        db  0CDh, 020h, 003h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_3_0EA0:
        db  0CDh, 020h, 009h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_3_0EA8:
        db  0CDh, 020h, 00Ah, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_3_0EB0:
        db  0CDh, 020h, 00Bh, 080h, 033h, 000h
        add    byte ptr [eax], al
        db  0CDh, 020h, 00Eh, 080h, 033h, 000h
        add    byte ptr [eax], al
        db  0CDh, 020h, 00Fh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_3_0EC8:
        db  0CDh, 020h, 018h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_3_0ED0:
        db  0CDh, 020h, 019h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_3_0ED8:
        db  0CDh, 020h, 01Ah, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_3_0EE0:
        db  0CDh, 020h, 01Ch, 080h, 033h, 000h
        add    byte ptr [eax], al
        db  0CDh, 020h, 024h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_3_0EF0:
        db  0CDh, 020h, 025h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_3_0EF8:
        db  0CDh, 020h, 026h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_3_0F00:
        db  0CDh, 020h, 029h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_3_0F08:
        db  0CDh, 020h, 02Ch, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_3_0F10:
        db  0CDh, 020h, 02Dh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_3_0F18:
        db  0CDh, 020h, 032h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_3_0F20:
        db  0CDh, 020h, 035h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_3_0F28:
        db  0CDh, 020h, 037h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_3_0F30:
        db  0CDh, 020h, 03Eh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_3_0F38:
        db  0CDh, 020h, 03Fh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_3_0F40:
        db  0CDh, 020h, 043h, 080h, 033h, 021h
_PTEXT ends
        end
