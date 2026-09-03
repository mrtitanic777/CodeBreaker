; dosmgr_lcode_07.asm -- DOSMGR object 7 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

loc_7_0000:
        pushad
        movzx  eax, word ptr [edi + 2]
        movzx  edx, word ptr [edi + 4]
        shl    eax, 4
        shl    edx, 4
        movzx  ecx, word ptr [edi]
        add    edi, 8
loc_7_0015:
        movzx  ebx, word ptr [edi]
        mov    ebp, dword ptr [ebx + eax]
        movzx  esi, word ptr [edi + 2]
        xchg   dword ptr [esi + edx], ebp
        mov    dword ptr [ebx + eax], ebp
        add    edi, 4
        loop   loc_7_0015
        popad
        ret
        db  08Bh, 045h, 01Ch, 00Fh, 0B6h, 0CCh, 081h, 0E9h, 080h, 000h, 000h, 000h, 072h, 026h, 083h, 0F9h
        db  004h, 077h, 021h, 08Bh, 035h, 000h, 000h, 000h, 000h, 08Bh, 03Dh, 008h, 002h, 000h, 000h, 003h
        db  07Bh, 004h, 0FFh, 014h, 08Dh, 0E1h, 000h, 000h, 000h, 072h, 00Ah, 080h, 03Dh, 01Dh, 000h, 000h
        db  000h, 000h, 074h, 001h, 0F9h, 0C3h, 03Ch, 010h, 073h, 02Ah, 083h, 07Ch, 01Eh, 004h, 000h, 075h
        db  00Bh, 0B9h, 005h, 000h, 000h, 000h, 0CDh, 020h, 01Fh, 000h, 001h, 000h, 03Ch, 002h, 075h, 002h
        db  0FEh, 007h, 0FFh, 005h, 0FCh, 001h, 000h, 000h, 003h, 0DEh, 0FFh, 043h, 004h, 00Fh, 0B6h, 0C0h
        db  0FEh, 044h, 018h, 008h, 0F8h, 0C3h, 00Fh, 0B6h, 0C0h, 083h, 0F8h, 010h, 073h, 029h, 03Ch, 002h
        db  075h, 007h, 080h, 03Fh, 000h, 074h, 002h, 0FEh, 00Fh, 003h, 0DEh, 080h, 07Ch, 018h, 008h, 000h
        db  074h, 015h, 0FEh, 04Ch, 018h, 008h, 0FFh, 04Bh, 004h, 0FFh, 00Dh, 0FCh, 001h, 000h, 000h, 075h
        db  006h, 0CDh, 020h, 020h, 000h, 001h, 000h, 0F8h, 0C3h, 0B9h, 004h, 000h, 000h, 000h, 033h, 0C0h
        db  088h, 007h, 08Dh, 07Ch, 01Eh, 008h, 0FCh, 0F3h, 0ABh, 033h, 0C9h, 087h, 04Ch, 01Eh, 004h, 0E3h
        db  00Bh, 0A3h, 0FCh, 001h, 000h, 000h, 0CDh, 020h, 020h, 000h, 001h, 000h, 0F8h, 0C3h, 0CDh, 020h
        db  038h, 000h, 001h, 000h, 0F9h, 0C3h, 08Bh, 0C0h, 08Bh, 045h, 01Ch, 080h, 0FCh, 048h, 074h, 056h
        db  066h, 03Dh, 022h, 011h, 074h, 00Ch, 084h, 0C0h, 074h, 037h, 066h, 03Dh, 080h, 010h, 074h, 040h
        db  0F9h, 0C3h, 0CDh, 020h, 004h, 000h, 001h, 000h, 074h, 0F6h, 08Bh, 015h, 08Ch, 001h, 000h, 000h
        db  003h, 053h, 004h, 00Fh, 0B7h, 08Ah, 034h, 003h, 000h, 000h, 00Fh, 0B7h, 092h, 030h, 003h, 000h
        db  000h, 08Bh, 0F2h, 0C1h, 0E6h, 004h, 003h, 073h, 004h, 0CDh, 020h, 00Eh, 000h, 015h, 000h, 0F9h
        db  0C3h, 080h, 0FCh, 010h, 074h, 00Ah, 080h, 0FCh, 011h, 074h, 005h, 080h, 0FCh, 014h, 075h, 0C0h
        db  0C6h, 045h, 01Ch, 0FFh, 0F8h, 0C3h, 08Bh, 015h, 000h, 000h, 000h, 000h, 0F6h, 004h, 01Ah, 001h
        db  074h, 0AEh, 03Ch, 010h, 075h, 0AAh, 0A1h, 08Ch, 001h, 000h, 000h, 003h, 043h, 004h, 00Fh, 0B7h
        db  080h, 030h, 003h, 000h, 000h, 066h, 039h, 044h, 01Ah, 002h, 075h, 094h, 0BEh, 0F5h, 000h, 000h
        db  000h, 00Fh, 0BAh, 02Ch, 01Ah, 00Fh, 072h, 00Fh, 08Bh, 035h, 010h, 000h, 000h, 000h, 003h, 073h
        db  004h, 081h, 0C6h, 0AFh, 000h, 000h, 000h, 0FCh, 02Bh, 0C0h, 089h, 045h, 01Ch, 0ACh, 00Fh, 0B7h
        db  07Dh, 03Ch, 0C1h, 0E7h, 004h, 00Fh, 0B7h, 04Dh, 014h, 003h, 0F9h, 003h, 07Bh, 004h, 00Fh, 0B6h
        db  00Fh, 047h, 049h, 03Bh, 0C8h, 072h, 002h, 08Bh, 0C8h, 08Ah, 0C1h, 0AAh, 041h, 0F3h, 0A4h, 0F8h
        db  0C3h, 066h, 0A3h, 0D8h, 004h, 000h, 000h, 0C3h, 060h, 08Bh, 04Dh, 018h, 066h, 0F7h, 003h, 080h
        db  001h, 075h, 003h, 00Fh, 0B7h, 0C9h, 085h, 0C9h, 00Fh, 084h, 051h, 001h, 000h, 000h, 00Fh, 0A0h
        db  0FFh, 075h, 018h, 0FFh, 075h, 014h, 051h, 0CDh, 020h, 00Ah, 000h, 006h, 000h, 081h, 0F9h, 0FFh
        db  0FFh, 000h, 000h, 076h, 005h, 0B9h, 000h, 0F0h, 000h, 000h, 087h, 00Ch, 024h, 066h, 08Bh, 045h
        db  03Ch, 08Bh, 055h, 014h, 0E8h, 063h, 003h, 000h, 000h, 073h, 073h, 081h, 0F9h, 0FFh, 0FFh, 000h
        db  000h, 077h, 06Bh, 08Bh, 0F0h, 08Bh, 0F9h, 049h, 025h, 0FFh, 00Fh, 000h, 000h, 003h, 0C8h, 0C1h
        db  0E9h, 00Ch, 041h, 08Bh, 0C6h, 0C1h, 0E8h, 00Ch, 06Ah, 000h, 051h, 050h, 06Ah, 000h, 051h, 050h
        db  0CDh, 020h, 063h, 000h, 001h, 000h, 083h, 0C4h, 00Ch, 085h, 0C0h, 075h, 007h, 08Bh, 0CFh, 083h
        db  0C4h, 00Ch, 0EBh, 03Ah, 0C1h, 0E6h, 00Ch, 066h, 0C1h, 0EEh, 00Ch, 066h, 089h, 075h, 014h, 0C1h
        db  0EEh, 010h, 066h, 087h, 075h, 060h, 0E8h, 059h, 002h, 0FBh, 0FFh, 066h, 089h, 075h, 060h, 0CDh
        db  020h, 064h, 000h, 001h, 000h, 083h, 0C4h, 00Ch, 066h, 0F7h, 003h, 080h, 001h, 00Fh, 084h, 0AFh
        db  000h, 000h, 000h, 066h, 0C7h, 045h, 01Eh, 000h, 000h, 0E9h, 0A4h, 000h, 000h, 000h, 0B8h, 014h
        db  03Ch, 000h, 000h, 0CDh, 020h, 007h, 000h, 006h, 000h, 08Bh, 055h, 010h, 08Bh, 03Dh, 000h, 000h
        db  000h, 000h, 003h, 0FBh, 066h, 039h, 057h, 02Eh, 075h, 04Ah, 08Bh, 0C7h, 0CDh, 020h, 008h, 001h
        db  001h, 000h, 097h, 066h, 08Bh, 040h, 02Ah, 066h, 03Bh, 047h, 034h, 075h, 037h, 08Bh, 047h, 030h
        db  08Bh, 0D0h, 003h, 0C1h, 089h, 047h, 030h, 03Bh, 00Ch, 024h, 076h, 028h, 081h, 0E2h, 0FFh, 001h
        db  000h, 000h, 074h, 020h, 08Bh, 0F9h, 08Bh, 00Ch, 024h, 02Bh, 0CAh, 08Bh, 045h, 01Ch, 08Bh, 0D1h
        db  0E8h, 061h, 000h, 000h, 000h, 072h, 04Bh, 087h, 0D1h, 03Bh, 0D1h, 072h, 037h, 02Bh, 0FAh, 08Bh
        db  00Ch, 024h, 0EBh, 01Fh, 08Bh, 045h, 01Ch, 033h, 0D2h, 03Bh, 00Ch, 024h, 076h, 01Bh, 08Bh, 0F9h
        db  08Bh, 00Ch, 024h, 0E8h, 03Eh, 000h, 000h, 000h, 072h, 028h, 003h, 0D1h, 03Bh, 00Ch, 024h, 072h
        db  013h, 02Bh, 0F9h, 03Bh, 0F9h, 077h, 0ECh, 08Bh, 0CFh, 0E3h, 009h, 0E8h, 026h, 000h, 000h, 000h
        db  072h, 010h, 003h, 0D1h, 066h, 089h, 055h, 01Ch, 066h, 0F7h, 003h, 080h, 001h, 074h, 003h, 089h
        db  055h, 01Ch, 083h, 0C4h, 004h, 08Fh, 045h, 014h, 08Fh, 045h, 018h, 00Fh, 0A1h, 061h, 0C3h, 0E8h
        db  080h, 001h, 0FBh, 0FFh, 061h, 0C3h, 052h, 057h, 08Bh, 0D4h, 08Bh, 03Dh, 0F4h, 001h, 000h, 000h
        db  085h, 0FFh, 074h, 00Ah, 08Bh, 0D7h, 0CDh, 020h, 008h, 001h, 001h, 000h, 003h, 0D7h, 083h, 0ECh
        db  06Ch, 08Bh, 0FCh, 0CDh, 020h, 08Dh, 000h, 001h, 000h, 083h, 0ECh, 008h, 06Ah, 000h, 08Bh, 03Ah
        db  089h, 022h, 051h, 057h, 00Fh, 0BAh, 0E0h, 008h, 0F5h, 0CDh, 020h, 008h, 000h, 006h, 000h, 089h
        db  04Dh, 018h, 066h, 089h, 07Dh, 014h, 0C1h, 0EFh, 010h, 066h, 087h, 07Dh, 060h, 089h, 045h, 01Ch
        db  0E8h, 02Fh, 001h, 0FBh, 0FFh, 08Fh, 002h, 066h, 089h, 07Dh, 060h, 0F6h, 045h, 02Ch, 001h, 075h
        db  045h, 066h, 039h, 04Dh, 01Ch, 075h, 01Fh, 00Fh, 0BAh, 0E0h, 008h, 0CDh, 020h, 009h, 000h, 006h
        db  000h, 083h, 0C4h, 07Ch, 066h, 00Fh, 0BAh, 065h, 02Ch, 000h, 072h, 007h, 00Fh, 0B7h, 04Dh, 01Ch
        db  003h, 0F1h, 0F8h, 05Fh, 05Ah, 0C3h, 00Fh, 0BAh, 0E0h, 008h, 073h, 0DFh, 050h, 00Fh, 0B7h, 045h
        db  01Ch, 085h, 0C0h, 074h, 00Dh, 051h, 08Bh, 0C8h, 0F9h, 0CDh, 020h, 009h, 000h, 006h, 000h, 059h
        db  02Bh, 0C8h, 058h, 0F8h, 0EBh, 0C5h, 080h, 07Ch, 024h, 004h, 000h, 074h, 0BEh, 0F8h, 0CDh, 020h
        db  009h, 000h, 006h, 000h, 0CDh, 020h, 004h, 000h, 001h, 000h, 075h, 006h, 0CDh, 020h, 072h, 001h
        db  001h, 000h, 052h, 08Dh, 054h, 024h, 008h, 050h, 056h, 0CDh, 020h, 083h, 000h, 001h, 000h, 0E8h
        db  094h, 007h, 0FBh, 0FFh, 0CDh, 020h, 086h, 000h, 001h, 000h, 0CDh, 020h, 004h, 000h, 001h, 000h
        db  075h, 008h, 033h, 0C9h, 0CDh, 020h, 03Ch, 001h, 001h, 000h, 05Eh, 03Ch, 001h, 058h, 05Ah, 075h
        db  009h, 059h, 083h, 0C4h, 00Ch, 0E9h, 02Fh, 0FFh, 0FFh, 0FFh, 083h, 0C4h, 07Ch, 0F9h, 0EBh, 083h
        db  0A1h, 000h, 002h, 000h, 000h, 0C3h, 08Bh, 015h, 088h, 000h, 000h, 000h, 085h, 0D2h, 074h, 011h
        db  08Bh, 0C2h, 0C1h, 0E0h, 00Ch, 066h, 0C1h, 0E8h, 00Ch, 087h, 015h, 07Ch, 000h, 000h, 000h, 089h
        db  002h, 08Bh, 015h, 084h, 000h, 000h, 000h, 085h, 0D2h, 074h, 054h, 079h, 031h, 0F7h, 0DAh, 0CDh
        db  020h, 06Bh, 000h, 001h, 000h, 0C1h, 0E0h, 008h, 051h, 08Dh, 00Ch, 002h, 08Bh, 0F9h, 0C1h, 0E7h
        db  004h, 083h, 0C7h, 006h, 0A1h, 080h, 000h, 000h, 000h, 0BAh, 03Bh, 000h, 000h, 000h, 0F7h, 0E2h
        db  08Bh, 0D1h, 08Dh, 048h, 003h, 0C1h, 0E9h, 002h, 033h, 0C0h, 0FCh, 0F3h, 0ABh, 059h, 0C1h, 0E2h
        db  004h, 0A1h, 080h, 000h, 000h, 000h, 066h, 089h, 042h, 004h, 083h, 0C8h, 0FFh, 089h, 002h, 08Bh
        db  03Dh, 07Ch, 000h, 000h, 000h, 066h, 039h, 007h, 075h, 005h, 0C1h, 0E2h, 00Ch, 089h, 017h, 0C3h
        db  0CDh, 020h, 006h, 080h, 006h, 000h, 033h, 0C9h, 0CDh, 020h, 03Ch, 001h, 001h, 000h, 057h, 0CDh
        db  020h, 006h, 000h, 006h, 000h, 08Bh, 035h, 0D4h, 004h, 000h, 000h, 08Bh, 03Ch, 024h, 08Dh, 07Fh
        db  033h, 0FCh, 0A4h, 0A5h, 0A5h, 05Fh, 0CDh, 020h, 072h, 081h, 001h, 000h, 057h, 0CDh, 020h, 008h
        db  001h, 001h, 000h, 0F6h, 047h, 002h, 003h, 05Fh, 0C3h, 0E8h, 0EEh, 0FFh, 0FFh, 0FFh, 00Fh, 084h
        db  0D8h, 007h, 0FBh, 0FFh, 0CDh, 020h, 006h, 080h, 006h, 000h, 0E8h, 0DDh, 0FFh, 0FFh, 0FFh, 00Fh
        db  084h, 0F9h, 007h, 0FBh, 0FFh, 0CDh, 020h, 006h, 080h, 006h, 000h, 039h, 03Ah, 03Bh, 041h, 043h
        db  047h, 04Eh, 04Fh, 056h, 06Ch, 060h, 0A0h, 0A6h, 0A7h, 0A8h, 0FFh, 005h, 005h, 005h, 005h, 005h
        db  036h, 039h, 03Ah, 016h, 031h, 03Ch, 03Bh, 038h, 03Dh, 037h, 011h, 005h, 066h, 002h, 000h, 000h
        db  004h, 014h, 03Ch, 001h, 040h, 001h, 000h, 038h, 000h, 021h, 001h, 034h, 000h, 014h, 03Ch, 000h
        db  021h, 001h, 004h, 001h, 014h, 03Ch, 000h, 021h, 001h, 004h, 001h, 004h, 03Ch, 000h, 021h, 004h
        db  004h, 03Ch, 001h, 004h, 001h, 000h, 038h, 000h, 021h, 005h, 08Ah, 002h, 000h, 000h, 001h, 008h
        db  000h, 004h, 03Ch, 000h, 021h, 001h, 008h, 000h, 000h, 038h, 000h, 021h, 051h, 052h, 06Ah, 000h
        db  050h, 053h, 0CDh, 020h, 06Eh, 000h, 001h, 000h, 083h, 0C4h, 00Ch, 05Ah, 066h, 0F7h, 003h, 080h
        db  001h, 075h, 003h, 00Fh, 0B7h, 0D2h, 003h, 0C2h, 03Dh, 000h, 000h, 010h, 000h, 072h, 010h, 02Bh
        db  043h, 004h, 072h, 007h, 03Dh, 000h, 000h, 010h, 000h, 072h, 004h, 003h, 043h, 004h, 0F8h, 059h
        db  0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
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
        db  0FFh, 000h, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 001h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FFh, 0FFh, 0FFh, 0FFh, 043h, 04Fh, 04Dh, 053h, 050h, 045h, 043h, 000h, 01Ch, 000h, 000h, 000h
        db  02Bh, 000h, 000h, 000h, 035h, 000h, 000h, 000h, 04Fh, 000h, 000h, 000h, 065h, 000h, 000h, 000h
        db  06Ah, 000h, 000h, 000h, 02Fh, 043h, 020h, 02Fh, 04Bh, 020h, 001h, 000h, 000h, 000h, 00Ah, 000h
        db  000h, 000h, 064h, 000h, 000h, 000h, 0E8h, 003h, 000h, 000h, 010h, 027h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  000h, 000h, 000h, 000h, 000h, 062h, 000h, 000h, 000h, 092h, 000h, 000h, 000h, 0C5h, 000h, 000h
        db  000h, 060h, 000h, 000h, 000h, 0EAh, 000h, 000h, 000h, 004h, 065h, 078h, 069h, 074h, 00Dh, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 002h, 000h, 014h
        db  03Ch, 000h, 021h, 001h, 004h, 000h, 014h, 03Ch, 000h, 021h, 005h, 054h, 003h, 000h, 000h, 005h
        db  0A5h, 000h, 000h, 000h, 002h, 018h, 014h, 03Ch, 000h, 021h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 02Ch, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 002h, 003h, 000h, 004h, 000h, 000h, 013h, 013h, 009h, 009h, 00Ah, 013h, 013h
        db  013h, 00Bh, 000h, 000h, 00Ch, 00Eh, 00Eh, 000h, 000h, 00Eh, 000h, 013h, 013h, 013h, 013h, 014h
        db  028h, 013h, 013h, 010h, 000h, 000h, 000h, 000h, 000h, 00Dh, 000h, 02Fh, 007h, 000h, 00Fh, 015h
        db  000h, 000h, 008h, 006h, 006h, 006h, 006h, 006h, 000h, 019h, 019h, 006h, 030h, 006h, 01Ah, 000h
        db  000h, 018h, 01Dh, 01Eh, 020h, 02Bh, 02Dh, 000h, 01Bh, 01Ch, 024h, 025h, 00Fh, 027h, 000h, 029h
        db  017h, 000h, 000h, 026h, 02Ah, 006h, 000h, 01Fh, 021h, 022h, 023h, 000h, 025h, 033h, 011h, 02Eh
        db  000h, 000h, 000h, 000h, 000h, 000h, 032h, 000h, 000h, 000h, 034h, 035h, 010h, 003h, 000h, 000h
        db  010h, 003h, 000h, 000h, 01Bh, 003h, 000h, 000h, 012h, 003h, 000h, 000h, 020h, 003h, 000h, 000h
        db  053h, 003h, 000h, 000h, 049h, 003h, 000h, 000h, 05Dh, 003h, 000h, 000h, 062h, 003h, 000h, 000h
        db  02Eh, 003h, 000h, 000h, 025h, 003h, 000h, 000h, 067h, 003h, 000h, 000h, 06Eh, 003h, 000h, 000h
        db  073h, 003h, 000h, 000h, 078h, 003h, 000h, 000h, 033h, 003h, 000h, 000h, 03Dh, 003h, 000h, 000h
        db  010h, 003h, 000h, 000h, 010h, 003h, 000h, 000h, 010h, 003h, 000h, 000h, 097h, 003h, 000h, 000h
        db  09Ch, 003h, 000h, 000h, 08Fh, 003h, 000h, 000h, 07Dh, 003h, 000h, 000h, 0ABh, 003h, 000h, 000h
        db  0A1h, 003h, 000h, 000h, 0A6h, 003h, 000h, 000h, 0E9h, 003h, 000h, 000h, 0ECh, 003h, 000h, 000h
        db  0DAh, 003h, 000h, 000h, 0DFh, 003h, 000h, 000h, 0B7h, 003h, 000h, 000h, 0E4h, 003h, 000h, 000h
        db  0BCh, 003h, 000h, 000h, 0C1h, 003h, 000h, 000h, 0C6h, 003h, 000h, 000h, 0F1h, 003h, 000h, 000h
        db  0F6h, 003h, 000h, 000h
loc_7_12B0:
        sti
        add    eax, dword ptr [eax]
        db  000h, 000h
        add    al, 0
        db  000h, 00Eh
        add    al, 0
        db  000h, 013h
        add    al, 0
        db  000h, 018h
        add    al, 0
        db  000h, 08Bh, 004h, 000h, 000h, 021h
        add    al, 0
        db  000h, 026h
        add    al, 0
        db  000h, 02Bh
        add    al, 0
        db  000h, 044h, 004h, 000h
        db  000h, 03Fh
        add    al, 0
        db  000h, 05Ah, 004h
        add    byte ptr [eax], al
        pop    edi
        add    al, 0
        db  000h, 038h
        add    eax, dword ptr [eax]
loc_7_12E7:
        db  000h, 049h, 004h
        add    byte ptr [eax], al
        daa
        db  005h, 000h, 000h, 044h, 005h
        add    byte ptr [eax], al
        db  03Dh, 005h, 000h, 000h, 036h
        db  005h, 000h, 000h
loc_7_12FC:
        sub    al, 5
        add    byte ptr [eax], al
        das
        db  005h, 000h, 000h, 030h, 004h
        add    byte ptr [eax], al
        dec    ebx
        db  005h, 000h, 000h, 055h, 005h
        add    byte ptr [eax], al
        add    byte ptr [ecx], ah
        db  003h, 0C0h
        or     al, 0
        add    byte ptr [esp + edi], dl
        add    byte ptr [ecx], ah
        db  005h, 08Ah, 00Ch, 000h, 000h
        db  005h, 02Ch, 00Eh, 000h, 000h
        db  003h, 0C6h
        add    byte ptr [eax], al
        add    byte ptr [esp + edi], dl
        add    byte ptr [ecx], ah
        db  005h, 0D6h, 000h, 000h, 000h
        push   es
        adc    byte ptr [eax], bh
        add    byte ptr [ecx], ah
        push   es
        add    al, 3ch
loc_7_133B:
        add    byte ptr [ecx], ah
        db  003h, 0C6h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  038h, 005h, 0D7h, 001h, 000h, 000h
        add    al, 14h
        cmp    al, 5
        in     eax, 4
        add    byte ptr [eax], al
        add    byte ptr [ecx], ah
        add    al, 14h
        cmp    al, 0
        and    dword ptr [eax], ecx
        adc    al, 3ch
        add    byte ptr [ecx], ah
        push   es
        adc    byte ptr [eax + eax], bh
        db  021h, 005h, 036h, 00Dh, 000h, 000h
        add    dword ptr [3c1400h], esp
        db  021h, 005h, 06Ch, 000h, 000h, 000h
        db  005h, 094h, 000h, 000h, 000h
        push   es
        adc    byte ptr [eax + eax], bh
        and    dword ptr [esp + edx], eax
        cmp    al, 5
        in     eax, 4
        add    byte ptr [eax], al
        add    al, 0
loc_7_1387:
        db  038h, 005h, 0E5h, 004h, 000h, 000h
        add    byte ptr [ecx], ah
        add    al, 14h
        cmp    al, 4
        add    byte ptr [eax], bh
        add    byte ptr [ecx], ah
        db  005h, 07Ch, 001h, 000h, 000h
        db  005h, 05Bh, 001h, 000h, 000h
        db  005h, 0C4h, 001h, 000h, 000h
        db  005h, 048h, 000h, 000h, 000h
loc_7_13AB:
        add    dword ptr [eax], eax
        add    al, 3ch
        db  005h, 0F6h, 004h, 000h, 000h
        add    byte ptr [ecx], ah
        db  005h, 01Dh, 002h, 000h, 000h
        db  005h, 039h, 002h, 000h, 000h
        db  005h, 055h, 002h, 000h, 000h
loc_7_13C6:
        add    al, 4
        cmp    al, 5
loc_7_13CA:
        in     eax, 4
        add    byte ptr [eax], al
        add    dword ptr [esi + 5380000h], eax
        test   byte ptr [eax + eax], 0
        add    byte ptr [ecx], ah
        db  005h, 08Eh, 00Dh, 000h, 000h
        db  005h, 0BFh, 00Eh, 000h, 000h
        db  005h, 031h, 00Fh, 000h, 000h
        add    al, 14h
        cmp    al, 5
        jmp    SHORT loc_7_13EF
loc_7_13EF:
        add    byte ptr [eax], al
        db  005h, 077h, 002h, 000h, 000h
        db  005h, 0AAh, 002h, 000h, 000h
        db  005h, 052h, 003h, 000h, 000h
        db  003h, 000h
        adc    dword ptr [eax], eax
        add    byte ptr [esp + edi], al
        add    dword ptr [ecx], esp
        add    byte ptr [eax], cl
        cmp    byte ptr [eax], al
        db  021h, 005h, 004h, 003h, 000h, 000h
        db  005h, 023h, 003h, 000h, 000h
        db  003h, 07Ch, 003h, 000h
        add    byte ptr [esp + edi], dl
        add    byte ptr [ecx], ah
        db  005h, 0E0h, 003h, 000h, 000h
        db  005h, 011h, 003h, 000h, 000h
        db  005h, 063h, 00Dh, 000h, 000h
        add    al, 14h
        cmp    al, 2
        sbb    byte ptr [eax], al
        cmp    byte ptr [eax], al
        and    dword ptr [edx], eax
        sbb    byte ptr [esp + edi], dl
        add    byte ptr [ecx], ah
        db  005h, 0A2h, 003h, 000h, 000h
        db  005h, 0EDh, 002h, 000h, 000h
        db  005h, 04Dh, 00Dh, 000h, 000h
        add    dword ptr [eax], edx
        add    byte ptr [esp + edi], al
        add    dword ptr [eax + 380000h], eax
        and    dword ptr [esp + eax], eax
        cmp    al, 0
        and    dword ptr [esp + eax], eax
        cmp    al, 5
        in     eax, 4
        add    byte ptr [eax], al
        add    byte ptr [ecx], ah
        add    dword ptr [eax], edx
        add    byte ptr [esp + edi], dl
        add    byte ptr [ecx], ah
        add    bl, byte ptr [eax]
        add    al, 3ch
        add    byte ptr [ecx], ah
        add    dword ptr [eax], eax
        add    byte ptr [eax], bh
        add    byte ptr [ecx], ah
        add    dword ptr [edx], esp
        add    byte ptr [esp + edi], dl
        add    byte ptr [ecx], ah
        add    dword ptr [esi], edx
        add    byte ptr [esp + edi], dl
        add    byte ptr [ecx], ah
        db  005h, 012h, 011h, 000h, 000h
        adc    byte ptr [ebx], al
        add    byte ptr [eax], al
        test   byte ptr [eax + eax], al
        db  000h, 010h
        add    eax, dword ptr [eax]
        db  000h, 084h, 004h, 000h, 000h, 084h, 004h
        add    byte ptr [eax], al
        adc    byte ptr [ebx], al
        add    byte ptr [eax], al
        adc    byte ptr [ebx], al
        add    byte ptr [eax], al
        adc    byte ptr [ebx], al
        add    byte ptr [eax], al
loc_7_14B0:
        adc    byte ptr [ebx], al
        add    byte ptr [eax], al
        adc    byte ptr [ebx], al
        add    byte ptr [eax], al
        test   byte ptr [eax + eax], al
        db  000h, 010h
        add    eax, dword ptr [eax]
        db  000h, 069h, 004h
        add    byte ptr [eax], al
        push   ebx
        add    eax, dword ptr [eax]
        db  000h, 070h, 004h
        add    byte ptr [eax], al
        jbe    SHORT loc_7_14D2
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_14D2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_150E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_155C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_156A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_158E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_1598:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_159E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_15A2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_15B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_15B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_15E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_1670:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_16A2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_16AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
