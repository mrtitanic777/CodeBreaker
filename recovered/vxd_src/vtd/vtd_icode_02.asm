; vtd_icode_02.asm -- VTD object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  0A1h, 0F4h, 004h, 000h, 000h
        db  068h, 0D4h, 001h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0A1h, 0F8h, 004h, 000h, 000h
        db  068h, 0EBh, 001h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0A1h, 0FCh, 004h, 000h, 000h
        db  068h, 003h, 002h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0A1h, 000h, 005h, 000h, 000h
        db  068h, 019h, 002h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0A1h, 004h, 005h, 000h, 000h
        db  068h, 038h, 002h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0A1h, 008h, 005h, 000h, 000h
        db  068h, 053h, 002h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0A1h, 00Ch, 005h, 000h, 000h
        db  068h, 06Bh, 002h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0A1h, 010h, 005h, 000h, 000h
        db  068h, 08Ch, 002h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0A1h, 0ECh, 004h, 000h, 000h
        db  068h, 0AAh, 002h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0A1h, 0F0h, 004h, 000h, 000h
        db  068h, 0C4h, 002h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0A1h, 014h, 005h, 000h, 000h
        db  068h, 0DEh, 002h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0A1h, 018h, 005h, 000h, 000h
        db  068h, 0FCh, 002h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0A1h, 01Ch, 005h, 000h, 000h
        db  068h, 01Ch, 003h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        db  068h, 03Ah, 003h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0A1h, 030h, 005h, 000h, 000h
        db  068h, 05Ah, 003h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  00Fh, 0B7h, 005h, 02Ch, 005h, 000h, 000h
        db  068h, 072h, 003h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
loc_2_0108:
        db  068h, 092h, 003h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        push   ebx
        call   sub_2_013C
        pop    ebx
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        db  0CDh, 020h, 002h, 000h, 001h, 000h
        je     SHORT loc_2_013B
        db  068h, 096h, 003h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0CDh, 020h, 0C4h, 000h, 001h, 000h
        jne    SHORT loc_2_0108
loc_2_013B:
        ret
sub_2_013C:
        db  068h, 09Dh, 003h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  03Bh, 01Dh, 0FCh, 004h, 000h, 000h
        jne    SHORT loc_2_016D
        db  068h, 0B0h, 003h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  03Bh, 01Dh, 000h, 005h, 000h, 000h
        jne    SHORT loc_2_016D
        db  068h, 0BEh, 003h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
loc_2_016D:
        db  068h, 0D2h, 003h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  003h, 01Dh, 0F4h, 004h, 000h, 000h
        test   byte ptr [ebx], 4
        je     SHORT loc_2_018E
        db  068h, 0D6h, 003h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
loc_2_018E:
        mov    eax, dword ptr [ebx + 4]
        cmp    eax, 10000h
        jne    SHORT loc_2_01A5
        db  068h, 0E8h, 003h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        jmp    SHORT loc_2_01C5
loc_2_01A5:
        mov    eax, dword ptr [ebx + 4]
        mov    edx, dword ptr [ebx + 8]
        db  068h, 004h, 004h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        cmp    eax, edx
        je     SHORT loc_2_01C5
        db  068h, 018h, 004h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
loc_2_01C5:
        db  068h, 031h, 004h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        ret
        db  000h, 000h, 000h, 023h, 045h, 041h, 058h, 020h, 020h, 020h, 056h, 054h, 044h, 05Fh, 043h, 042h
        db  05Fh, 04Fh, 066h, 066h, 073h, 065h, 074h, 00Dh, 00Ah, 000h, 023h, 045h, 041h, 058h, 020h, 020h
        db  020h, 056h, 054h, 044h, 05Fh, 049h, 052h, 051h, 05Fh, 048h, 061h, 06Eh, 064h, 06Ch, 065h, 00Dh
        db  00Ah, 000h, 023h, 045h, 041h, 058h, 020h, 020h, 020h, 056h, 054h, 044h, 05Fh, 046h, 06Fh, 063h
        db  075h, 073h, 05Fh, 056h, 04Dh, 00Dh, 00Ah, 000h, 023h, 045h, 041h, 058h, 020h, 020h, 020h, 056h
        db  054h, 044h, 05Fh, 054h, 069h, 06Dh, 065h, 072h, 05Fh, 041h, 074h, 05Fh, 056h, 04Dh, 05Fh, 053h
        db  070h, 065h, 065h, 064h, 00Dh, 00Ah, 000h, 023h, 045h, 041h, 058h, 020h, 020h, 020h, 056h, 054h
        db  044h, 05Fh, 043h, 075h, 072h, 072h, 065h, 06Eh, 074h, 05Fh, 043h, 06Fh, 075h, 06Eh, 074h, 00Dh
        db  00Ah, 000h, 023h, 045h, 041h, 058h, 020h, 020h, 020h, 056h, 054h, 044h, 05Fh, 04Eh, 065h, 078h
        db  074h, 05Fh, 043h, 06Fh, 075h, 06Eh, 074h, 00Dh, 00Ah, 000h, 023h, 045h, 041h, 058h, 020h, 020h
        db  020h, 056h, 054h, 044h, 05Fh, 052h, 069h, 06Eh, 067h, 030h, 05Fh, 044h, 065h, 073h, 069h, 072h
        db  065h, 064h, 05Fh, 043h, 06Fh, 075h, 06Eh, 074h, 00Dh, 00Ah, 000h, 023h, 045h, 041h, 058h, 020h
        db  020h, 020h, 056h, 054h, 044h, 05Fh, 04Ch, 06Fh, 077h, 065h, 073h, 074h, 05Fh, 04Ch, 061h, 074h
        db  063h, 068h, 05Fh, 056h, 061h, 06Ch, 00Dh, 00Ah, 000h, 023h, 045h, 041h, 058h, 020h, 020h, 020h
        db  056h, 054h, 044h, 05Fh, 04Ch, 06Fh, 05Fh, 04Eh, 075h, 06Dh, 05Fh, 054h, 069h, 063h, 06Bh, 073h
        db  00Dh, 00Ah, 000h, 023h, 045h, 041h, 058h, 020h, 020h, 020h, 056h, 054h, 044h, 05Fh, 048h, 069h
        db  05Fh, 04Eh, 075h
loc_2_02D4:
        insd
        pop    edi
        push   esp
        db  069h
loc_2_02D8:
        arpl   word ptr [ebx + 73h], bp
        or     eax, 4523000ah
        inc    ecx
        pop    eax
        and    byte ptr [eax], ah
        and    byte ptr [esi + 54h], dl
        inc    esp
        pop    edi
        dec    esp
        popad
        db  073h, 074h
        pop    edi
        push   esp
        db  069h, 06Dh, 065h, 05Fh, 055h
loc_2_02F4:
        db  070h, 064h
        popad
        db  074h, 065h
        or     eax, 4523000ah
        inc    ecx
        pop    eax
        and    byte ptr [eax], ah
        and    byte ptr [esi + 54h], dl
        inc    esp
        pop    edi
        dec    esp
        popad
        db  073h, 074h
        pop    edi
        push   esi
        imul   esi, dword ptr [edx + 74h], 746e495fh
        pop    edi
        push   esp
        imul   ebp, dword ptr [ebp + 65h], 23000a0dh
        inc    ebp
        inc    ecx
        pop    eax
        and    byte ptr [eax], ah
        and    byte ptr [esi + 54h], dl
        inc    esp
        pop    edi
        push   edx
        db  065h, 070h, 072h
        outsd
        pop    edi
loc_2_032E:
        dec    ebp
        db  069h, 06Eh, 05Fh
loc_2_0332:
        inc    ebx
        outsd
_ITEXT ends
        end
