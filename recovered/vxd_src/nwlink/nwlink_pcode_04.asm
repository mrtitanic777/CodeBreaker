; nwlink_pcode_04.asm -- NWLINK object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_4_0000:
        push   esi
        db  0C7h, 005h, 08Ch, 058h, 000h, 000h, 088h, 058h, 000h, 000h
        db  0C7h, 005h, 088h, 058h, 000h, 000h, 088h, 058h, 000h, 000h
        db  0C7h, 005h, 0A8h, 058h, 000h, 000h, 0A4h, 058h, 000h, 000h
        db  0C7h, 005h, 0A4h, 058h, 000h, 000h, 0A4h, 058h, 000h, 000h
        db  0C7h, 005h, 0B0h, 058h, 000h, 000h, 0ACh, 058h, 000h, 000h
        db  0C7h, 005h, 0ACh, 058h, 000h, 000h, 0ACh, 058h, 000h, 000h
        db  0C7h, 005h, 0B4h, 058h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0B8h, 058h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 04Ch, 058h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 048h, 058h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0E8h, 0C4h, 00Eh, 0FCh, 0FFh
        cmp    eax, 1
        jne    SHORT loc_4_0079
        mov    eax, 1
        jmp    NEAR PTR loc_4_0225
loc_4_0079:
        db  0E8h, 0D2h, 00Fh, 0FCh, 0FFh
        cmp    eax, 1
        jne    SHORT loc_4_0097
        push   0
        db  0E8h, 09Ah, 0FFh, 0FBh, 0FFh
        add    esp, 4
        mov    eax, 1
        jmp    NEAR PTR loc_4_0225
loc_4_0097:
        db  0A1h, 024h, 058h, 000h, 000h
        imul   eax, eax, 3fh
        db  0A3h, 01Ch, 058h, 000h, 000h
        db  0A1h, 028h, 058h, 000h, 000h
        imul   eax, eax, 76h
        db  001h, 005h, 01Ch, 058h, 000h, 000h
        db  0A1h, 034h, 058h, 000h, 000h
        shl    eax, 6
        db  001h, 005h, 01Ch, 058h, 000h, 000h
        db  0A1h, 038h, 058h, 000h, 000h
        imul   eax, eax, 50h
        db  001h, 005h, 01Ch, 058h, 000h, 000h
        push   -1
        push   0
        db  0FFh, 035h, 01Ch, 058h, 000h, 000h
        db  068h, 018h, 058h, 000h, 000h
        db  0E8h, 086h, 023h, 0FCh, 0FFh
        db  083h, 03Dh, 018h, 058h, 000h, 000h, 000h
        jne    SHORT loc_4_00FF
        push   0
        db  0E8h, 032h, 0FFh, 0FBh, 0FFh
        add    esp, 4
        mov    eax, 1
        jmp    NEAR PTR loc_4_0225
loc_4_00FF:
        db  08Bh, 00Dh, 018h, 058h, 000h, 000h
        sub    edx, edx
        db  039h, 015h, 038h, 058h, 000h, 000h
        jbe    SHORT loc_4_012A
        db  0B8h, 0DCh, 058h, 000h, 000h
        sub    esi, esi
loc_4_0116:
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 8], esi
        add    ecx, 50h
        add    eax, 4
        inc    edx
        db  039h, 015h, 038h, 058h, 000h, 000h
        ja     SHORT loc_4_0116
loc_4_012A:
        db  0A1h, 0DCh, 058h, 000h, 000h
        db  0A3h, 0ECh, 058h, 000h, 000h
        sub    edx, edx
        db  039h, 015h, 034h, 058h, 000h, 000h
        jbe    SHORT loc_4_0159
        db  0B8h, 0CCh, 058h, 000h, 000h
        sub    esi, esi
loc_4_0145:
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], esi
        add    ecx, 40h
        add    eax, 4
        inc    edx
        db  039h, 015h, 034h, 058h, 000h, 000h
        ja     SHORT loc_4_0145
loc_4_0159:
        db  0A1h, 040h, 058h, 000h, 000h
        mov    dword ptr [eax], 0
        db  0A1h, 024h, 058h, 000h, 000h
        db  0A3h, 044h, 058h, 000h, 000h
        sub    eax, eax
        db  0C7h, 005h, 070h, 058h, 000h, 000h, 000h, 000h, 000h, 000h
        db  039h, 005h, 024h, 058h, 000h, 000h
        jbe    SHORT loc_4_019D
loc_4_0182:
        db  08Bh, 015h, 070h, 058h, 000h, 000h
        mov    dword ptr [ecx + 18h], edx
        db  089h, 00Dh, 070h, 058h, 000h, 000h
        inc    eax
        add    ecx, 38h
        db  03Bh, 005h, 024h, 058h, 000h, 000h
        jb     SHORT loc_4_0182
loc_4_019D:
        db  0C7h, 005h, 054h, 058h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 028h, 058h, 000h, 000h
        db  0A3h, 058h, 058h, 000h, 000h
        db  0C7h, 005h, 074h, 058h, 000h, 000h, 000h, 000h, 000h, 000h
        sub    eax, eax
        db  039h, 005h, 028h, 058h, 000h, 000h
        jbe    SHORT loc_4_01E0
loc_4_01C5:
        db  08Bh, 015h, 074h, 058h, 000h, 000h
        mov    dword ptr [ecx + 10h], edx
        db  089h, 00Dh, 074h, 058h, 000h, 000h
        inc    eax
        add    ecx, 70h
        db  03Bh, 005h, 028h, 058h, 000h, 000h
        jb     SHORT loc_4_01C5
loc_4_01E0:
        db  089h, 00Dh, 05Ch, 058h, 000h, 000h
        db  0A1h, 024h, 058h, 000h, 000h
        lea    eax, [ecx + eax*2]
        db  0A3h, 060h, 058h, 000h, 000h
        db  0A1h, 024h, 058h, 000h, 000h
        shl    eax, 2
        db  003h, 005h, 060h, 058h, 000h, 000h
        db  0A3h, 064h, 058h, 000h, 000h
        db  003h, 005h, 024h, 058h, 000h, 000h
        db  0A3h, 068h, 058h, 000h, 000h
        db  0A1h, 028h, 058h, 000h, 000h
        add    eax, eax
        db  003h, 005h, 068h, 058h, 000h, 000h
        db  0A3h, 06Ch, 058h, 000h, 000h
        sub    eax, eax
loc_4_0225:
        pop    esi
        ret
sub_4_0227:
        push   esi
        push   edi
        db  0A1h, 078h, 058h, 000h, 000h
        mov    esi, dword ptr [esp + 0ch]
        imul   eax, eax, 1ch
        lea    edi, [esi + 3ch]
        push   -1
        push   0
        push   eax
        push   edi
        db  0E8h, 025h, 022h, 0FCh, 0FFh
        db  0A1h, 078h, 058h, 000h, 000h
        shl    eax, 2
        add    eax, dword ptr [edi]
        mov    dword ptr [esi + 40h], eax
        db  08Bh, 00Dh, 078h, 058h, 000h, 000h
        lea    ecx, [eax + ecx*2]
        mov    dword ptr [esi + 44h], ecx
        mov    dword ptr [esi + 38h], 0
        or     dword ptr [esi + 8], 2
        pop    edi
        pop    esi
        ret
        db  053h, 056h, 057h, 055h, 02Bh, 0DBh, 08Bh, 07Ch, 024h, 018h, 03Bh, 0FBh, 074h, 024h, 08Bh, 06Ch
        db  024h, 014h, 0BEh, 0CCh, 058h, 000h, 000h, 053h, 0FFh, 036h, 055h, 0E8h, 01Dh, 000h, 000h, 000h
        db  083h, 0C4h, 00Ch, 00Bh, 0C0h, 075h, 012h, 083h, 0C5h, 008h, 083h, 0C6h, 004h, 043h, 03Bh, 0FBh
        db  077h, 0E5h, 08Bh, 0C3h, 05Dh, 05Fh, 05Eh, 05Bh, 0C3h, 02Bh, 0C0h, 0EBh, 0F7h
sub_4_02A7:
        sub    esp, 38h
        mov    ecx, dword ptr [esp + 40h]
        mov    eax, dword ptr [esp + 44h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 40h]
        mov    byte ptr [eax + 2eh], 0
        mov    eax, dword ptr [esp + 40h]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [esp + 40h]
        mov    dword ptr [eax + 3ch], 0
        push   dword ptr [esp + 40h]
        push   dword ptr [esp + 40h]
        call   sub_4_0A60
        add    esp, 8
        or     eax, eax
        mov    eax, dword ptr [esp + 40h]
        je     SHORT loc_4_02F6
        mov    dword ptr [eax + 4], 80000000h
        jmp    NEAR PTR loc_4_0776
loc_4_02F6:
        or     dword ptr [eax + 4], 1
        mov    eax, dword ptr [esp + 40h]
        mov    eax, dword ptr [eax + 18h]
        or     eax, eax
        jne    SHORT loc_4_0312
        mov    dword ptr [esp + 24h], 1010102h
        jmp    NEAR PTR loc_4_0472
loc_4_0312:
        cmp    eax, 1
        jne    SHORT loc_4_0324
        mov    dword ptr [esp + 24h], 2010102h
        jmp    NEAR PTR loc_4_0472
loc_4_0324:
        cmp    eax, 2
        jne    SHORT loc_4_035D
        mov    dword ptr [esp + 24h], 6010102h
        mov    eax, dword ptr [esp + 40h]
        add    eax, 2dh
        mov    dword ptr [esp + 28h], eax
        push   6
        mov    eax, dword ptr [esp + 44h]
        push   0
        add    eax, 28h
        push   eax
        db  0E8h, 095h, 047h, 0FCh, 0FFh
        mov    dword ptr [esp + 38h], 1
        add    esp, 0ch
        jmp    NEAR PTR loc_4_0472
loc_4_035D:
        cmp    eax, 3
        jne    NEAR PTR loc_4_0776
        mov    word ptr [esp + 8], 80h
        mov    word ptr [esp + 0ah], 0
        mov    word ptr [esp + 0ch], 3781h
        mov    dword ptr [esp + 20h], 1
        lea    eax, [esp + 8]
        mov    dword ptr [esp + 24h], 4010104h
        mov    dword ptr [esp + 28h], eax
        mov    dword ptr [esp + 2ch], 6
        lea    eax, [esp + 10h]
        push   eax
        mov    eax, dword ptr [esp + 44h]
        push   dword ptr [eax + 0ch]
        lea    eax, [esp + 8]
        push   eax
        db  0E8h, 011h, 021h, 0FCh, 0FFh
        cmp    dword ptr [esp], 103h
        jne    SHORT loc_4_03D0
        lea    eax, [esp + 10h]
        push   dword ptr [esp + 40h]
        push   eax
        db  0E8h, 0E4h, 01Fh, 0FCh, 0FFh
        mov    dword ptr [esp + 8], eax
        add    esp, 8
loc_4_03D0:
        mov    dword ptr [esp + 4], 1
        mov    dword ptr [esp + 24h], 4010106h
        lea    eax, [esp + 4]
        mov    dword ptr [esp + 28h], eax
        mov    dword ptr [esp + 2ch], 4
        lea    eax, [esp + 10h]
        push   eax
        mov    eax, dword ptr [esp + 44h]
        push   dword ptr [eax + 0ch]
        lea    eax, [esp + 8]
        push   eax
        db  0E8h, 0BCh, 020h, 0FCh, 0FFh
        cmp    dword ptr [esp], 103h
        jne    SHORT loc_4_0425
        lea    eax, [esp + 10h]
        push   dword ptr [esp + 40h]
        push   eax
        db  0E8h, 08Fh, 01Fh, 0FCh, 0FFh
        mov    dword ptr [esp + 8], eax
        add    esp, 8
loc_4_0425:
        mov    dword ptr [esp + 24h], 0ff010207h
        lea    eax, [esp + 40h]
        mov    dword ptr [esp + 28h], eax
        mov    dword ptr [esp + 2ch], 4
        lea    eax, [esp + 10h]
        push   eax
        mov    eax, dword ptr [esp + 44h]
        push   dword ptr [eax + 0ch]
        lea    eax, [esp + 8]
        push   eax
        db  0E8h, 06Fh, 020h, 0FCh, 0FFh
        cmp    dword ptr [esp], 103h
        jne    SHORT loc_4_0472
        lea    eax, [esp + 10h]
        push   dword ptr [esp + 40h]
        push   eax
        db  0E8h, 042h, 01Fh, 0FCh, 0FFh
        mov    dword ptr [esp + 8], eax
        add    esp, 8
loc_4_0472:
        mov    eax, dword ptr [esp + 40h]
        mov    dword ptr [esp + 20h], 0
        cmp    dword ptr [eax + 18h], 2
        jne    SHORT loc_4_0495
        add    eax, 2dh
        mov    dword ptr [esp + 28h], eax
        mov    dword ptr [esp + 2ch], 1
        jmp    SHORT loc_4_04A8
loc_4_0495:
        mov    eax, dword ptr [esp + 40h]
        add    eax, 28h
        mov    dword ptr [esp + 28h], eax
        mov    dword ptr [esp + 2ch], 6
loc_4_04A8:
        lea    eax, [esp + 10h]
        push   eax
        mov    eax, dword ptr [esp + 44h]
        push   dword ptr [eax + 0ch]
        lea    eax, [esp + 8]
        push   eax
        db  0E8h, 004h, 020h, 0FCh, 0FFh
        cmp    dword ptr [esp], 103h
        jne    SHORT loc_4_04DD
        lea    eax, [esp + 10h]
        push   dword ptr [esp + 40h]
        push   eax
        db  0E8h, 0D7h, 01Eh, 0FCh, 0FFh
        mov    dword ptr [esp + 8], eax
        add    esp, 8
loc_4_04DD:
        mov    dword ptr [esp + 24h], 10106h
        mov    eax, dword ptr [esp + 40h]
        add    eax, 1ch
        mov    dword ptr [esp + 28h], eax
        mov    dword ptr [esp + 2ch], 4
        lea    eax, [esp + 10h]
        push   eax
        mov    eax, dword ptr [esp + 44h]
        push   dword ptr [eax + 0ch]
        lea    eax, [esp + 8]
        push   eax
        db  0E8h, 0B4h, 01Fh, 0FCh, 0FFh
        cmp    dword ptr [esp], 103h
        jne    SHORT loc_4_052D
        lea    eax, [esp + 10h]
        push   dword ptr [esp + 40h]
        push   eax
        db  0E8h, 087h, 01Eh, 0FCh, 0FFh
        mov    dword ptr [esp + 8], eax
        add    esp, 8
loc_4_052D:
        mov    eax, dword ptr [esp + 40h]
        cmp    dword ptr [eax + 18h], 2
        jne    SHORT loc_4_054C
        mov    ecx, dword ptr [eax + 1ch]
        add    eax, 1ch
        cmp    ecx, 240h
        jb     SHORT loc_4_054A
        mov    ecx, 240h
loc_4_054A:
        mov    dword ptr [eax], ecx
loc_4_054C:
        mov    eax, dword ptr [esp + 40h]
        cmp    dword ptr [eax + 18h], 3
        jne    SHORT loc_4_0562
        mov    dword ptr [eax + 20h], 0
        jmp    NEAR PTR loc_4_05E4
loc_4_0562:
        mov    dword ptr [esp + 24h], 10107h
        mov    eax, dword ptr [esp + 40h]
        add    eax, 20h
        mov    dword ptr [esp + 28h], eax
        mov    dword ptr [esp + 2ch], 4
        lea    eax, [esp + 10h]
        push   eax
        mov    eax, dword ptr [esp + 44h]
        push   dword ptr [eax + 0ch]
        lea    eax, [esp + 8]
        push   eax
        db  0E8h, 02Fh, 01Fh, 0FCh, 0FFh
        cmp    dword ptr [esp], 103h
        jne    SHORT loc_4_05B2
        lea    eax, [esp + 10h]
        push   dword ptr [esp + 40h]
        push   eax
        db  0E8h, 002h, 01Eh, 0FCh, 0FFh
        mov    dword ptr [esp + 8], eax
        add    esp, 8
loc_4_05B2:
        mov    eax, dword ptr [esp + 40h]
        mov    eax, dword ptr [eax + 20h]
        cmp    eax, 2710h
        jb     SHORT loc_4_05CD
        mov    eax, dword ptr [esp + 40h]
        mov    dword ptr [eax + 24h], 1
        jmp    SHORT loc_4_05EC
loc_4_05CD:
        mov    ecx, dword ptr [esp + 40h]
        push   dword ptr [ecx + 1ch]
        push   eax
        db  0E8h, 0D6h, 007h, 0FCh, 0FFh
        mov    ecx, dword ptr [esp + 48h]
        add    esp, 8
        mov    dword ptr [ecx + 24h], eax
loc_4_05E4:
        mov    eax, dword ptr [esp + 40h]
        or     dword ptr [eax + 4], 2
loc_4_05EC:
        mov    dword ptr [esp + 24h], 10105h
        lea    eax, [esp + 4]
        mov    dword ptr [esp + 28h], eax
        mov    dword ptr [esp + 2ch], 4
        lea    eax, [esp + 10h]
        push   eax
        mov    eax, dword ptr [esp + 44h]
        push   dword ptr [eax + 0ch]
        lea    eax, [esp + 8]
        push   eax
        db  0E8h, 0A8h, 01Eh, 0FCh, 0FFh
        cmp    dword ptr [esp], 103h
        jne    SHORT loc_4_0639
        lea    eax, [esp + 10h]
        push   dword ptr [esp + 40h]
        push   eax
        db  0E8h, 07Bh, 01Dh, 0FCh, 0FFh
        mov    dword ptr [esp + 8], eax
        add    esp, 8
loc_4_0639:
        mov    dword ptr [esp + 20h], 1
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 100h
        jb     SHORT loc_4_0651
        mov    eax, 100h
loc_4_0651:
        mov    dword ptr [esp + 4], eax
        mov    eax, dword ptr [esp + 40h]
        cmp    dword ptr [eax + 18h], 1
        jne    SHORT loc_4_0667
        mov    dword ptr [esp + 4], 0c0h
loc_4_0667:
        mov    dword ptr [esp + 24h], 1010fh
        lea    eax, [esp + 4]
        mov    dword ptr [esp + 28h], eax
        mov    dword ptr [esp + 2ch], 4
        lea    eax, [esp + 10h]
        push   eax
        mov    eax, dword ptr [esp + 44h]
        push   dword ptr [eax + 0ch]
        lea    eax, [esp + 8]
        push   eax
        db  0E8h, 02Dh, 01Eh, 0FCh, 0FFh
        cmp    dword ptr [esp], 103h
        jne    SHORT loc_4_06B4
        lea    eax, [esp + 10h]
        push   dword ptr [esp + 40h]
        push   eax
        db  0E8h, 000h, 01Dh, 0FCh, 0FFh
        mov    dword ptr [esp + 8], eax
        add    esp, 8
loc_4_06B4:
        mov    eax, dword ptr [esp + 40h]
        cmp    dword ptr [eax + 18h], 1
        jne    SHORT loc_4_071D
        mov    dword ptr [esp + 24h], 2010103h
        mov    dword ptr [esp + 4], 8000h
        lea    eax, [esp + 4]
        mov    dword ptr [esp + 28h], eax
        mov    dword ptr [esp + 2ch], 4
        lea    eax, [esp + 10h]
        push   eax
        mov    eax, dword ptr [esp + 44h]
        push   dword ptr [eax + 0ch]
        lea    eax, [esp + 8]
        push   eax
        db  0E8h, 0CEh, 01Dh, 0FCh, 0FFh
        cmp    dword ptr [esp], 103h
        jne    SHORT loc_4_0713
        lea    eax, [esp + 10h]
        push   dword ptr [esp + 40h]
        push   eax
        db  0E8h, 0A1h, 01Ch, 0FCh, 0FFh
        mov    dword ptr [esp + 8], eax
        add    esp, 8
loc_4_0713:
        mov    dword ptr [esp + 4], 4009h
        jmp    SHORT loc_4_0725
loc_4_071D:
        mov    dword ptr [esp + 4], 9
loc_4_0725:
        mov    dword ptr [esp + 24h], 1010eh
        lea    eax, [esp + 4]
        mov    dword ptr [esp + 28h], eax
        mov    dword ptr [esp + 2ch], 4
        lea    eax, [esp + 10h]
        push   eax
        mov    eax, dword ptr [esp + 44h]
        push   dword ptr [eax + 0ch]
        lea    eax, [esp + 8]
        push   eax
        db  0E8h, 06Fh, 01Dh, 0FCh, 0FFh
        cmp    dword ptr [esp], 103h
        jne    SHORT loc_4_0772
        lea    eax, [esp + 10h]
        push   dword ptr [esp + 40h]
        push   eax
        db  0E8h, 042h, 01Ch, 0FCh, 0FFh
        mov    dword ptr [esp + 8], eax
        add    esp, 8
loc_4_0772:
        sub    eax, eax
        jmp    SHORT loc_4_077B
loc_4_0776:
        mov    eax, 1
loc_4_077B:
        add    esp, 38h
        ret
sub_4_077F:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 18h]
        mov    ebx, dword ptr [esp + 1ch]
        mov    dword ptr [esi + 8], 1
        mov    eax, dword ptr [ebx + 18h]
        or     eax, eax
        je     SHORT loc_4_07BE
        cmp    eax, 1
        je     NEAR PTR loc_4_0867
        cmp    eax, 2
        je     NEAR PTR loc_4_08E6
        cmp    eax, 3
        je     NEAR PTR loc_4_0922
        mov    eax, 1
        jmp    NEAR PTR loc_4_0A04
loc_4_07BE:
        mov    ebp, dword ptr [esp + 14h]
        movzx  eax, byte ptr [ebx + 2eh]
        mov    ecx, dword ptr [ebp + 4]
        mov    edx, 1
        shl    edx, cl
        test   edx, eax
        je     SHORT loc_4_07DE
        mov    eax, 1
        jmp    NEAR PTR loc_4_0A04
loc_4_07DE:
        or     ecx, ecx
        jne    SHORT loc_4_0808
        test   byte ptr [ebp + 0ch], 4
        db  0C7h, 046h, 028h, 01Eh, 047h, 000h, 000h
        jne    SHORT loc_4_07F6
        db  0C7h, 046h, 028h, 063h, 047h, 000h, 000h
loc_4_07F6:
        lea    edi, [esi + 14h]
        db  0C7h, 046h, 02Ch, 087h, 01Fh, 000h, 000h
        mov    dword ptr [edi], 0
        jmp    SHORT loc_4_084B
loc_4_0808:
        cmp    ecx, 1
        jne    SHORT loc_4_080F
        jmp    SHORT loc_4_0834
loc_4_080F:
        cmp    ecx, 2
        jne    SHORT loc_4_082D
        db  0C7h, 046h, 028h, 012h, 047h, 000h, 000h
        lea    edi, [esi + 14h]
        db  0C7h, 046h, 02Ch, 031h, 01Fh, 000h, 000h
        mov    dword ptr [edi], 0
        jmp    SHORT loc_4_084B
loc_4_082D:
        mov    dword ptr [ebp + 4], 4
loc_4_0834:
        db  0C7h, 046h, 028h, 0C2h, 047h, 000h, 000h
        lea    edi, [esi + 14h]
        db  0C7h, 046h, 02Ch, 060h, 01Eh, 000h, 000h
        mov    dword ptr [edi], 3
loc_4_084B:
        lea    edx, [esi + 10h]
        mov    al, 1
        mov    dword ptr [edx], 0eh
        mov    cl, byte ptr [ebp + 4]
        shl    al, cl
        or     byte ptr [ebx + 2eh], al
        mov    byte ptr [esi + 22h], 0ffh
        jmp    NEAR PTR loc_4_095A
loc_4_0867:
        cmp    dword ptr [ebx + 8], 0
        je     SHORT loc_4_0877
        mov    eax, 1
        jmp    NEAR PTR loc_4_0A04
loc_4_0877:
        mov    ebp, dword ptr [esp + 14h]
        cmp    dword ptr [ebp + 4], 6
        jne    SHORT loc_4_08A4
        db  0C7h, 046h, 028h, 03Ch, 048h, 000h, 000h
        lea    edi, [esi + 14h]
        db  0C7h, 046h, 02Ch, 0A2h, 01Eh, 000h, 000h
        mov    dword ptr [edi], 8
        db  0C7h, 005h, 0D8h, 055h, 000h, 000h, 000h, 0AAh, 000h, 000h
        jmp    SHORT loc_4_08C5
loc_4_08A4:
        db  0C7h, 046h, 028h, 01Bh, 048h, 000h, 000h
        lea    edi, [esi + 14h]
        db  0C7h, 046h, 02Ch, 0DEh, 01Eh, 000h, 000h
        mov    dword ptr [edi], 3
        db  0C7h, 005h, 0D8h, 055h, 000h, 000h, 000h, 0E0h, 000h, 000h
loc_4_08C5:
        db  083h, 03Dh, 078h, 058h, 000h, 000h, 000h
        je     SHORT loc_4_08D7
        push   esi
        call   sub_4_0227
        add    esp, 4
loc_4_08D7:
        lea    edx, [esi + 10h]
        mov    dword ptr [edx], 0eh
        mov    byte ptr [esi + 22h], 7fh
        jmp    SHORT loc_4_095A
loc_4_08E6:
        cmp    dword ptr [ebx + 8], 0
        je     SHORT loc_4_08F6
        mov    eax, 1
        jmp    NEAR PTR loc_4_0A04
loc_4_08F6:
        db  0C7h, 046h, 028h, 0E6h, 047h, 000h, 000h
        lea    edi, [esi + 14h]
        db  0C7h, 046h, 02Ch, 054h, 01Fh, 000h, 000h
        mov    dword ptr [edi], 0
        lea    edx, [esi + 10h]
        db  0C7h, 005h, 0D8h, 055h, 000h, 000h, 0FAh, 000h, 000h, 000h
        mov    dword ptr [edx], 3
        jmp    SHORT loc_4_0956
loc_4_0922:
        cmp    dword ptr [ebx + 8], 0
        je     SHORT loc_4_0932
        mov    eax, 1
        jmp    NEAR PTR loc_4_0A04
loc_4_0932:
        db  0C7h, 046h, 028h, 063h, 047h, 000h, 000h
        lea    edi, [esi + 14h]
        db  0C7h, 046h, 02Ch, 087h, 01Fh, 000h, 000h
        mov    dword ptr [edi], 0
        lea    edx, [esi + 10h]
        or     dword ptr [esi + 8], 10h
        mov    dword ptr [edx], 0eh
loc_4_0956:
        mov    ebp, dword ptr [esp + 14h]
loc_4_095A:
        inc    dword ptr [ebx + 8]
        mov    eax, dword ptr [ebx + 3ch]
        or     eax, eax
        jne    SHORT loc_4_0969
        mov    dword ptr [ebx + 3ch], esi
        jmp    SHORT loc_4_097C
loc_4_0969:
        cmp    dword ptr [eax + 4ch], 0
        je     SHORT loc_4_0979
        sub    ecx, ecx
loc_4_0971:
        mov    eax, dword ptr [eax + 4ch]
        cmp    dword ptr [eax + 4ch], ecx
        jne    SHORT loc_4_0971
loc_4_0979:
        mov    dword ptr [eax + 4ch], esi
loc_4_097C:
        mov    dword ptr [esi + 4ch], 0
        mov    dword ptr [esi + 48h], ebx
        mov    ecx, dword ptr [esp + 20h]
        mov    dword ptr [esi], ecx
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [esi + 4], eax
        test   byte ptr [ebx + 4], 2
        je     SHORT loc_4_09A0
        or     dword ptr [esi + 8], 8
loc_4_09A0:
        mov    eax, dword ptr [ebx + 24h]
        mov    word ptr [esi + 24h], ax
        mov    eax, dword ptr [ebx + 1ch]
        sub    eax, dword ptr [edi]
        mov    dword ptr [esi + 0ch], eax
        mov    eax, dword ptr [ebx + 1ch]
        add    dword ptr [edx], eax
        test   byte ptr [esi + 8], 2
        je     SHORT loc_4_09BE
        sub    dword ptr [esi + 0ch], 12h
loc_4_09BE:
        push   6
        lea    eax, [ebx + 28h]
        push   eax
        lea    eax, [esi + 1ch]
        push   eax
        db  0E8h, 026h, 041h, 0FCh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [esi + 18h], eax
        mov    byte ptr [esi + 23h], 0
        cmp    dword ptr [ebx + 18h], 0
        jne    SHORT loc_4_09F1
        cmp    dword ptr [ebp + 4], 4
        jne    SHORT loc_4_09F1
        push   esi
        db  0E8h, 050h, 00Ch, 0FCh, 0FFh
        add    esp, 4
        jmp    SHORT loc_4_09F8
loc_4_09F1:
        db  083h, 00Dh, 0F8h, 058h, 000h, 000h, 001h
loc_4_09F8:
        db  081h, 025h, 0F8h, 058h, 000h, 000h, 0FFh, 0FEh, 0FFh, 0FFh
        sub    eax, eax
loc_4_0A04:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  053h, 056h, 057h, 055h, 02Bh, 0DBh, 08Bh, 074h, 024h, 018h, 03Bh, 0F3h, 074h, 034h, 0BDh, 0DCh
        db  058h, 000h, 000h, 08Bh, 07Ch, 024h, 01Ch, 08Bh, 007h, 039h, 044h, 024h, 014h, 076h, 02Ah, 053h
        db  0FFh, 034h, 085h, 0CCh, 058h, 000h, 000h, 0FFh, 075h, 000h, 057h, 0E8h, 046h, 0FDh, 0FFh, 0FFh
        db  083h, 0C4h, 010h, 00Bh, 0C0h, 075h, 019h, 083h, 0C5h, 004h, 083h, 0C7h, 010h, 043h, 03Bh, 0DEh
        db  072h, 0D5h, 02Bh, 0C0h, 05Dh, 05Fh, 05Eh, 05Bh, 0C3h, 0B8h, 001h, 000h, 000h, 000h, 0EBh, 0F4h
        db  0B8h, 001h, 000h, 000h, 000h, 0EBh, 0EDh
sub_4_0A60:
        sub    esp, 18h
        push   esi
        mov    dword ptr [esp + 0ch], 0
        mov    dword ptr [esp + 10h], 1
        mov    dword ptr [esp + 14h], 7
        mov    dword ptr [esp + 18h], 3
        push   0
        push   0
        mov    esi, dword ptr [esp + 2ch]
        push   dword ptr [esp + 28h]
        push   esi
        db  0FFh, 035h, 014h, 058h, 000h, 000h
        lea    eax, [esp + 20h]
        push   4
        push   eax
        lea    eax, [esi + 18h]
        push   eax
        lea    eax, [esi + 0ch]
        push   eax
        lea    eax, [esp + 2ch]
        push   eax
        lea    eax, [esp + 2ch]
        push   eax
        db  0E8h, 001h, 01Ah, 0FCh, 0FFh
        cmp    dword ptr [esp + 4], 103h
        je     SHORT loc_4_0AC6
        cmp    dword ptr [esp + 4], 1
        jmp    SHORT loc_4_0AEA
loc_4_0AC6:
        test   byte ptr [esi + 7], 20h
        jne    SHORT loc_4_0AD7
loc_4_0ACC:
        db  0E8h, 0C7h, 03Fh, 0FCh, 0FFh
        test   byte ptr [esi + 7], 20h
        je     SHORT loc_4_0ACC
loc_4_0AD7:
        mov    eax, dword ptr [esi + 4]
        and    eax, 0dfffffffh
        mov    dword ptr [esi + 4], eax
        and    eax, 40000000h
        cmp    eax, 1
loc_4_0AEA:
        sbb    eax, eax
        inc    eax
        pop    esi
        add    esp, 18h
        ret
loc_4_0AF2:
        cmp    dword ptr [esp + 8], 0
        mov    eax, dword ptr [esp + 4]
        je     SHORT loc_4_0B04
        or     dword ptr [eax + 4], 40000000h
loc_4_0B04:
        or     dword ptr [eax + 4], 20000000h
        ret    240ch
        db  04Eh, 057h
_PTEXT ends
        end
