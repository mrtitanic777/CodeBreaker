; vdd_pcode_05.asm -- VDD object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_5_0000:
        sub    esp, 4
        mov    word ptr [esp + 2], 0
        db  0A1h, 0A4h, 010h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_5_0047
        db  068h, 050h, 001h, 000h, 000h
        db  0E8h, 043h, 018h, 0FBh, 0FFh
        add    esp, 4
        or     eax, eax
        je     SHORT loc_5_005E
        db  068h, 05Ch, 001h, 000h, 000h
        push   eax
        db  0E8h, 039h, 018h, 0FBh, 0FFh
        add    esp, 8
        db  0A3h, 0A4h, 010h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_5_0047
        db  0C7h, 005h, 0A4h, 010h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        jmp    SHORT loc_5_005E
loc_5_0047:
        db  0A3h, 0A4h, 010h, 000h, 000h
        lea    ecx, [esp + 2]
        push   ecx
        push   2
        push   eax
        push   0
        db  0E8h, 0F5h, 017h, 0FBh, 0FFh
        add    esp, 10h
loc_5_005E:
        db  0C7h, 005h, 0A0h, 010h, 000h, 000h, 000h, 000h, 000h, 000h
        add    esp, 4
        ret
loc_5_006C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   dword ptr [ebp + 8]
        db  0E8h, 063h, 000h, 0FBh, 0FFh
        add    esp, 4
        mov    dword ptr [ebp + 8], eax
        or     eax, eax
        jne    SHORT loc_5_0091
        mov    eax, 57h
        jmp    NEAR PTR loc_5_0127
loc_5_0091:
        cmp    dword ptr [ebp + 14h], 1ch
        je     SHORT loc_5_00A1
        mov    eax, 57h
        jmp    NEAR PTR loc_5_0127
loc_5_00A1:
        push   dword ptr [ebp + 8]
        db  0E8h, 060h, 000h, 0FBh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_5_00B7
        mov    eax, 57h
        jmp    SHORT loc_5_0127
loc_5_00B7:
        test   byte ptr [eax + 8], 1
        jne    SHORT loc_5_00C4
        mov    eax, 57h
        jmp    SHORT loc_5_0127
loc_5_00C4:
        push   0ffh
        push   dword ptr [ebp + 8]
        db  0E8h, 05Ah, 00Bh, 0FBh, 0FFh
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    eax, -1
        jne    SHORT loc_5_00E3
        mov    eax, 57h
        jmp    SHORT loc_5_0127
loc_5_00E3:
        pushad
        mov    ebp, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp]
        mov    ebx, dword ptr [ebp + 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 10h]
        mov    edi, dword ptr [ebp + 14h]
        push   ebp
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
        mov    dword ptr [ebp], eax
        mov    dword ptr [ebp + 4], ebx
        mov    dword ptr [ebp + 8], ecx
        mov    dword ptr [ebp + 0ch], edx
        mov    dword ptr [ebp + 10h], esi
        mov    dword ptr [ebp + 14h], edi
        popad
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 8]
        db  0E8h, 009h, 00Bh, 0FBh, 0FFh
        add    esp, 8
        sub    eax, eax
loc_5_0127:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  083h, 0ECh, 01Ch, 053h, 056h, 057h, 055h, 02Bh, 0DBh, 08Bh, 074h, 024h, 030h, 068h, 0FFh, 000h
        db  000h, 000h, 056h, 0E8h, 0E5h, 00Ah, 0FBh, 0FFh, 083h, 0C4h, 008h, 08Bh, 0F8h, 083h, 0FFh, 0FFh
        db  075h, 007h, 0B8h, 057h, 000h, 000h, 000h, 0EBh, 076h, 083h, 07Ch, 024h, 034h, 000h, 074h, 022h
        db  0C7h, 044h, 024h, 010h, 003h, 000h, 000h, 000h, 08Dh, 044h, 024h, 010h, 06Ah, 01Ch, 050h, 06Ah
        db  000h, 056h, 0E8h, 0F7h, 0FEh, 0FFh, 0FFh, 083h, 0C4h, 010h, 08Bh, 0D8h, 00Bh, 0DBh, 074h, 006h
        db  0EBh, 04Bh, 00Bh, 0DBh, 075h, 03Dh, 08Bh, 06Ch, 024h, 038h, 080h, 07Dh, 000h, 000h, 074h, 033h
        db  066h, 00Fh, 0BEh, 045h, 000h, 066h, 00Dh, 000h, 00Eh, 00Fh, 0BFh, 0C0h, 089h, 044h, 024h, 010h
        db  0C7h, 044h, 024h, 014h, 000h, 000h, 000h, 000h, 08Dh, 044h, 024h, 010h, 06Ah, 01Ch, 050h, 045h
        db  06Ah, 000h, 056h, 0E8h, 0B6h, 0FEh, 0FFh, 0FFh, 083h, 0C4h, 010h, 08Bh, 0D8h, 080h, 07Dh, 000h
        db  000h, 075h, 0CDh, 057h, 056h, 0E8h, 063h, 00Ah, 0FBh, 0FFh, 083h, 0C4h, 008h, 08Bh, 0C3h, 05Dh
        db  05Fh, 05Eh, 05Bh, 083h, 0C4h, 01Ch, 0C3h, 053h, 056h, 057h, 055h, 08Bh, 06Ch, 024h, 020h, 00Bh
        db  0EDh, 075h, 007h, 0B8h, 057h, 000h, 000h, 000h, 0EBh, 03Dh, 08Bh, 05Ch, 024h, 014h, 06Ah, 002h
        db  053h, 0E8h, 037h, 00Ah, 0FBh, 0FFh, 083h, 0C4h, 008h, 083h, 0F8h, 0FFh, 075h, 007h, 0B8h, 057h
        db  000h, 000h, 000h, 0EBh, 022h, 08Bh, 07Ch, 024h, 018h, 08Bh, 074h, 024h, 01Ch, 08Bh, 0CDh, 0C1h
        db  0E9h, 002h, 0F3h, 0A5h, 08Bh, 0CDh, 083h, 0E1h, 003h, 0F3h, 0A4h, 050h, 053h, 0E8h, 00Bh, 00Ah
        db  0FBh, 0FFh, 083h, 0C4h, 008h, 02Bh, 0C0h, 05Dh, 05Fh, 05Eh, 05Bh, 0C3h, 053h, 056h, 057h, 055h
        db  08Bh, 06Ch, 024h, 020h, 00Bh, 0EDh, 075h, 007h, 0B8h, 057h, 000h, 000h, 000h, 0EBh, 03Dh, 08Bh
        db  05Ch, 024h, 014h, 06Ah, 002h, 053h, 0E8h, 0E2h, 009h, 0FBh, 0FFh, 083h, 0C4h, 008h, 083h, 0F8h
        db  0FFh, 075h, 007h, 0B8h, 057h, 000h, 000h, 000h, 0EBh, 022h, 08Bh, 07Ch, 024h, 01Ch, 08Bh, 074h
        db  024h, 018h, 08Bh, 0CDh, 0C1h, 0E9h, 002h, 0F3h, 0A5h, 08Bh, 0CDh, 083h, 0E1h, 003h, 0F3h, 0A4h
        db  050h, 053h, 0E8h, 0B6h, 009h, 0FBh, 0FFh, 083h, 0C4h, 008h, 02Bh, 0C0h, 05Dh, 05Fh, 05Eh, 05Bh
        db  0C3h, 056h, 08Bh, 044h, 024h, 014h, 00Bh, 0C0h, 074h, 029h, 083h, 0F8h, 004h, 077h, 024h, 08Bh
        db  074h, 024h, 008h, 06Ah, 001h, 056h, 0E8h, 092h, 009h, 0FBh, 0FFh, 083h, 0C4h, 008h, 083h, 0F8h
        db  0FFh, 075h, 002h, 0EBh, 00Eh, 050h, 056h, 0E8h, 081h, 009h, 0FBh, 0FFh, 083h, 0C4h, 008h, 02Bh
        db  0C0h, 0EBh, 005h, 0B8h, 057h, 000h, 000h, 000h, 05Eh, 0C3h, 056h, 08Bh, 044h, 024h, 014h, 00Bh
        db  0C0h, 074h, 029h, 083h, 0F8h, 004h, 077h, 024h, 08Bh, 074h, 024h, 008h, 06Ah, 001h, 056h, 0E8h
        db  059h, 009h, 0FBh, 0FFh, 083h, 0C4h, 008h, 083h, 0F8h, 0FFh, 075h, 002h, 0EBh, 00Eh, 050h, 056h
        db  0E8h, 048h, 009h, 0FBh, 0FFh, 083h, 0C4h, 008h, 02Bh, 0C0h, 0EBh, 005h, 0B8h, 057h, 000h, 000h
        db  000h, 05Eh, 0C3h, 000h, 000h, 000h
sub_5_02F4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, 4011h
        mov    edx, 12h
        mov    eax, 6c00h
        mov    ecx, 6
        push   ebp
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
        jb     SHORT loc_5_035A
        mov    ebx, eax
        mov    ecx, dword ptr [ebp + 10h]
        mov    eax, 4000h
        mov    edx, dword ptr [ebp + 0ch]
        push   ebp
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
        jb     SHORT loc_5_033C
        cmp    eax, dword ptr [ebp + 10h]
        je     SHORT loc_5_033C
        stc
loc_5_033C:
        pushfd
        push   ebp
        mov    eax, 3e00h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        mov    eax, 0d00h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
        popfd
loc_5_035A:
        cmc
        pop    edi
        pop    esi
        pop    ebx
        sbb    eax, eax
        leave
        ret
sub_5_0362:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 4100h
        push   ebp
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
        mov    edx, 1
        jae    SHORT loc_5_0387
        cmp    al, 2
        je     SHORT loc_5_0387
        xor    edx, edx
loc_5_0387:
        push   edx
        push   ebp
        mov    eax, 0d00h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
        pop    eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_5_039D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, 10h
        mov    edx, 1
        mov    eax, 6c00h
        mov    ecx, 6
        push   ebp
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
        jb     SHORT loc_5_03F6
        mov    ebx, eax
        mov    ecx, dword ptr [ebp + 10h]
        mov    eax, 3f00h
        mov    edx, dword ptr [ebp + 0ch]
        push   ebp
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
        jb     SHORT loc_5_03E5
        cmp    eax, dword ptr [ebp + 10h]
        je     SHORT loc_5_03E5
        stc
loc_5_03E5:
        pushfd
        push   ebp
        mov    eax, 3e00h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebp
        popfd
loc_5_03F6:
        cmc
        pop    edi
        pop    esi
        pop    ebx
        sbb    eax, eax
        leave
        ret
_PTEXT ends
        end
