; vpasd_pcode_04.asm -- VPASD object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  060h, 0A1h, 000h, 000h, 000h, 000h, 0C1h, 0E8h, 00Ah, 083h, 0F8h, 040h, 07Eh, 005h, 0B8h, 040h
        db  000h, 000h, 000h, 083h, 0C0h, 003h, 0C1h, 0E8h, 002h, 08Bh, 0D8h, 048h, 066h, 00Fh, 0BDh, 0C8h
        db  0FEh, 0C1h, 0B8h, 001h, 000h, 000h, 000h, 0D3h, 0E0h, 048h, 08Bh, 0CBh, 051h, 0BFh, 000h, 000h
        db  000h, 000h, 06Ah, 00Eh, 057h, 068h, 0FFh, 00Fh, 000h, 000h, 06Ah, 000h, 050h, 06Ah, 000h, 06Ah
        db  001h, 051h, 0CDh, 020h, 053h, 000h, 001h, 000h, 083h, 0C4h, 020h, 059h, 00Bh, 0C0h, 075h, 00Eh
        db  08Bh, 0C1h, 0D1h, 0E8h, 075h, 0C3h, 0CDh, 020h, 0BFh, 000h, 001h, 000h, 0EBh, 034h, 0A3h, 000h
        db  000h, 000h, 000h, 089h, 015h, 000h, 000h, 000h, 000h, 0C1h, 0E1h, 00Ch, 06Ah, 000h, 06Ah, 000h
        db  068h, 092h, 000h, 000h, 000h, 051h, 052h, 0CDh, 020h, 07Ah, 000h, 001h, 000h, 083h, 0C4h, 014h
        db  06Ah, 000h, 050h, 052h, 0CDh, 020h, 076h, 000h, 001h, 000h, 083h, 0C4h, 00Ch, 0A3h, 000h, 000h
        db  000h, 000h, 061h, 0C3h
sub_4_0094:
        push   edi
        db  08Dh, 03Dh, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_00A0:
        pop    edi
        ret
sub_4_00A2:
        push   edi
        db  0BFh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 095h, 000h, 001h, 000h
        db  0F7h, 005h, 000h, 000h, 000h, 000h, 010h, 000h, 000h, 000h
        jne    SHORT loc_4_00CE
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 005h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 023h, 000h, 000h, 000h
loc_4_00CE:
        pop    edi
        ret
sub_4_00D0:
        push   esi
        sub    eax, eax
        mov    edx, 0b8ah
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        mov    edx, 0b8bh
        out    dx, al
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        mov    edx, 0f8ah
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        mov    edx, 1388h
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        mov    edx, 1389h
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        mov    edx, 138bh
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        mov    edx, 0f388h
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        mov    edx, 0f389h
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        mov    edx, 0f38ah
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        mov    edx, 8388h
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        mov    edx, 838bh
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        mov    edx, 0bf8ah
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        pop    esi
        ret
sub_4_0196:
        push   esi
        db  0F7h, 005h, 000h, 000h, 000h, 000h, 008h, 000h, 000h, 000h
        je     SHORT loc_4_01C7
        db  0A1h, 000h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_01C7
        push   eax
        mov    edx, eax
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        pop    edx
        inc    edx
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
loc_4_01C7:
        pop    esi
        ret
sub_4_01C9:
        push   esi
        db  0F7h, 005h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        je     NEAR PTR loc_4_02E2
        db  0F7h, 005h, 000h, 000h, 000h, 000h, 040h, 000h, 000h, 000h
        jne    NEAR PTR loc_4_026B
        db  0F7h, 005h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h
        je     SHORT loc_4_0257
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_0257
        mov    edx, 6
        db  003h, 015h, 000h, 000h, 000h, 000h
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        mov    edx, 0ah
        db  003h, 015h, 000h, 000h, 000h, 000h
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        mov    edx, 0ch
        db  003h, 015h, 000h, 000h, 000h, 000h
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        mov    edx, 0eh
        db  003h, 015h, 000h, 000h, 000h, 000h
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
loc_4_0257:
        db  0BFh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        jb     SHORT loc_4_02E2
        db  0A3h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_02D5
loc_4_026B:
        mov    edx, 6
        db  003h, 015h, 000h, 000h, 000h, 000h
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        mov    edx, 0ah
        db  003h, 015h, 000h, 000h, 000h, 000h
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        mov    edx, 0ch
        db  003h, 015h, 000h, 000h, 000h, 000h
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        mov    edx, 0eh
        db  003h, 015h, 000h, 000h, 000h, 000h
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        db  0BFh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        jb     SHORT loc_4_02E2
        db  0A3h, 000h, 000h, 000h, 000h
loc_4_02D5:
        db  0CDh, 020h, 009h, 000h, 003h, 000h
        db  083h, 00Dh, 000h, 000h, 000h, 000h, 004h
loc_4_02E2:
        pop    esi
        ret
sub_4_02E4:
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        pop    esi
        ret
sub_4_02FE:
        db  0A1h, 000h, 000h, 000h, 000h
        db  00Bh, 005h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_0312
        mov    eax, 0
        jmp    SHORT loc_4_0317
loc_4_0312:
        mov    eax, 0ffffffffh
loc_4_0317:
        ret
loc_4_0318:
        push   esi
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     NEAR PTR loc_4_03AD
        mov    esi, ebx
        db  003h, 035h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     SHORT loc_4_033F
        clc
        mov    ecx, 23h
        db  0CDh, 020h, 009h, 000h, 006h, 000h
loc_4_033F:
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_4_0350
        clc
        mov    ecx, dword ptr [esi + 8]
        db  0CDh, 020h, 009h, 000h, 006h, 000h
loc_4_0350:
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_0361
        push   ebx
        xor    ebx, ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0360:
        pop    ebx
loc_4_0361:
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_0389
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_036E:
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0373:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_03AD
loc_4_0389:
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_039B
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_039B:
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_03AD
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_03AD:
        pop    esi
        clc
        ret
        db  0EBh, 006h, 0FFh, 025h, 000h, 000h, 000h, 000h
loc_4_03B8:
        pushad
        pushfd
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_4_03EE
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_03D3
        push   ebx
        xor    ebx, ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_03D2:
        pop    ebx
loc_4_03D3:
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_03EE
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_03E0:
        xor    ebx, ebx
        db  087h, 01Dh, 000h, 000h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_4_03EE:
        popfd
        popad
        db  0FFh, 025h, 000h, 000h, 000h, 000h
sub_4_03F6:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    esi, ebx
        add    esi, eax
        call   sub_4_04A9
        call   sub_4_04D2
        call   sub_4_0414
        call   sub_4_0422
        ret
sub_4_0414:
        push   edi
        db  0BFh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 017h, 001h, 001h, 000h
        pop    edi
        ret
sub_4_0422:
        push   esi
        mov    edx, 0b8ah
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        mov    edx, 0b8bh
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        mov    edx, 0f8ah
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        mov    edx, 1388h
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        mov    edx, 1389h
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        mov    edx, 138bh
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        mov    edx, 8388h
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        mov    edx, 838bh
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        mov    edx, 0f388h
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        mov    edx, 0f389h
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        mov    edx, 0f38ah
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        mov    edx, 0bf8ah
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        pop    esi
        ret
sub_4_04A9:
        push   esi
        db  0F7h, 005h, 000h, 000h, 000h, 000h, 008h, 000h, 000h, 000h
        je     SHORT loc_4_04D0
        db  0A1h, 000h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_04D0
        push   eax
        mov    edx, eax
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        pop    edx
        inc    edx
        db  0CDh, 020h, 016h, 001h, 001h, 000h
loc_4_04D0:
        pop    esi
        ret
sub_4_04D2:
        push   esi
        db  0F7h, 005h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        je     NEAR PTR loc_4_0597
        db  0F7h, 005h, 000h, 000h, 000h, 000h, 040h, 000h, 000h, 000h
        jne    SHORT loc_4_054A
        db  0F7h, 005h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h
        je     SHORT loc_4_0548
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_0548
        mov    edx, 6
        db  003h, 015h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        mov    edx, 0ah
        db  003h, 015h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        mov    edx, 0ch
        db  003h, 015h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        mov    edx, 0eh
        db  003h, 015h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 016h, 001h, 001h, 000h
loc_4_0548:
        jmp    SHORT loc_4_0597
loc_4_054A:
        mov    edx, 6
        db  003h, 015h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        mov    edx, 0ah
        db  003h, 015h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        mov    edx, 0ch
        db  003h, 015h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        mov    edx, 0eh
        db  003h, 015h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        db  083h, 00Dh, 000h, 000h, 000h, 000h, 0FBh
        jmp    SHORT loc_4_0597
loc_4_0597:
        pop    esi
        ret
        db  02Eh, 08Bh, 0C0h
loc_4_059C:
        db  0CDh, 020h, 012h, 080h, 004h, 000h
sub_4_05A2:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushad
        mov    eax, dword ptr [ebp + 8]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 20h]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 10h]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 14h]
        db  0A3h, 000h, 000h, 000h, 000h
        movsx  eax, word ptr [ebp + 18h]
        cmp    eax, -1
        jne    SHORT loc_4_05DC
        xor    eax, eax
loc_4_05DC:
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 1ch]
        shl    eax, 0ah
        db  0A3h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        call   sub_4_0790
        jb     SHORT loc_4_063F
        movzx  eax, word ptr [ebp + 14h]
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 004h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        jb     SHORT loc_4_063F
        mov    dword ptr [esp + 1ch], 0ffffffffh
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0626:
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        je     SHORT loc_4_0638
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0636:
        jmp    SHORT loc_4_0626
loc_4_0638:
        pop    ebx
loc_4_0639:
        popad
        add    esp, 4
        pop    ebp
        ret
loc_4_063F:
        call   sub_4_064E
        mov    dword ptr [esp + 1ch], 0
        jmp    SHORT loc_4_0639
sub_4_064E:
        pushad
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        call   sub_4_03F6
        db  0A1h, 000h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_0669
        db  0CDh, 020h, 01Ah, 000h, 004h, 000h
loc_4_0669:
        db  0A1h, 000h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_068F
        push   0
        push   eax
        db  0CDh, 020h, 077h, 000h, 001h, 000h
        add    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
loc_4_068F:
        db  0A1h, 000h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_069E
        db  0CDh, 020h, 011h, 000h, 003h, 000h
loc_4_069E:
        popad
        ret
sub_4_06A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushad
        mov    dword ptr [esp + 1ch], 13h
        push   dword ptr [ebp + 20h]
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_4_05A2
        add    esp, 1ch
        or     eax, eax
        je     SHORT loc_4_0700
        mov    dword ptr [esp + 1ch], 0
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_0700:
        popad
        add    esp, 4
        pop    ebp
        ret
        db  0C7h, 044h, 024h, 01Ch, 024h, 000h, 000h, 000h, 0EBh, 0F0h
sub_4_0710:
        push   ebp
        mov    ebp, esp
        pushfd
        pushad
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_0727
        push   ebx
        xor    ebx, ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0726:
        pop    ebx
loc_4_0727:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_074E
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0735:
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0740:
        xor    ebx, ebx
        db  087h, 01Dh, 000h, 000h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_4_074E:
        call   sub_4_064E
        mov    dword ptr [esp + 1ch], 0ffffffffh
        popad
        popfd
        pop    ebp
        ret
loc_4_075F:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0776:
        add    esp, 14h
        leave
        clc
        ret
loc_4_077C:
        mov    eax, ebx
        db  0BBh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 04Ah, 004h
        mov    eax, 0
        stc
        ret
sub_4_0790:
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        xor    dx, 388h
        xor    dx, 0b8bh
        in     al, dx
        cmp    al, 0ffh
        je     NEAR PTR loc_4_08AC
        shr    al, 1
        shr    al, 1
        shr    al, 1
        shr    al, 1
        shr    al, 1
        db  0A2h, 000h, 000h, 000h, 000h
        cmp    al, 7
        je     SHORT loc_4_07C4
        cmp    al, 5
        ja     NEAR PTR loc_4_08AC
loc_4_07C4:
        in     al, dx
        mov    ah, al
        xor    al, 0e0h
        out    dx, al
        in     al, dx
        and    ax, 0e0e0h
        cmp    al, ah
        jne    NEAR PTR loc_4_08AC
        db  0A0h, 000h, 000h, 000h, 000h
        cmp    al, 0
        je     SHORT loc_4_07F9
        cmp    al, 1
        je     SHORT loc_4_0803
        cmp    al, 3
        je     SHORT loc_4_082E
        cmp    al, 4
        je     SHORT loc_4_0844
        cmp    al, 5
        je     SHORT loc_4_0854
        cmp    al, 7
        je     SHORT loc_4_0864
        jmp    NEAR PTR loc_4_08AC
loc_4_07F9:
        mov    ebx, 1
        or     ebx, 2
        jmp    SHORT loc_4_0872
loc_4_0803:
        mov    ebx, 1
        or     ebx, 2
        or     ebx, 10h
        or     ebx, 20h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        xor    dx, 388h
        xor    dx, 0ef8bh
        in     al, dx
        test   al, 8
        jne    SHORT loc_4_0872
        or     ebx, 200h
        jmp    SHORT loc_4_0872
loc_4_082E:
        mov    ebx, 1
        or     ebx, 2
        or     ebx, 10h
        or     ebx, 20h
        or     ebx, 100h
        jmp    SHORT loc_4_0872
loc_4_0844:
        mov    ebx, 1
        or     ebx, 2
        or     ebx, 10h
        or     ebx, 20h
        jmp    SHORT loc_4_0872
loc_4_0854:
        mov    ebx, 1
        or     ebx, 2
        or     ebx, 10h
        or     ebx, 20h
        jmp    SHORT loc_4_0872
loc_4_0864:
        mov    ebx, 1
        or     ebx, 2
        or     ebx, 10h
        or     ebx, 20h
loc_4_0872:
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    esi, ebx
        add    esi, eax
        call   sub_4_0094
        call   sub_4_02E4
        call   0
        call   sub_4_0196
        call   sub_4_01C9
        call   sub_4_00A2
        call   sub_4_00D0
        clc
        ret
loc_4_08AC:
        stc
        ret
_PTEXT ends
        end
