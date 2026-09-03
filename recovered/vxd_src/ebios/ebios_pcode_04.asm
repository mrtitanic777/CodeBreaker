; ebios_pcode_04.asm -- EBIOS object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  0F7h, 0C3h, 001h, 000h, 00Fh, 085h, 09Bh, 000h, 0F9h, 0B4h, 0C0h, 0CDh, 015h, 072h, 05Fh, 00Ah
        db  0E4h, 075h, 05Bh, 026h, 0F6h, 047h, 005h, 004h, 074h, 054h, 033h, 0C0h, 08Eh, 0C0h, 0B4h, 0C1h
        db  0CDh, 015h, 072h, 04Ah, 08Ch, 0C0h, 00Bh, 0C0h, 074h, 044h, 050h, 0CDh, 012h, 0C1h, 0E0h, 006h
        db  05Ah, 03Bh, 0D0h, 08Bh, 0C2h, 074h, 002h, 033h, 0D2h, 0C1h, 0E8h, 008h, 081h, 0E2h, 0FFh, 000h
        db  066h, 0C1h, 0E2h, 010h, 08Bh, 0D0h, 0BBh, 0AEh, 000h, 0B9h, 001h, 000h, 03Dh, 096h, 000h, 072h
        db  014h, 03Dh, 0A0h, 000h, 073h, 00Fh, 0B9h, 0A0h, 000h, 02Bh, 0C8h, 053h, 089h, 007h, 043h, 043h
        db  0E2h, 0FAh, 05Bh, 0EBh, 002h, 033h, 0DBh, 033h, 0F6h, 0B8h, 000h, 000h, 0EBh, 03Fh, 0B8h, 000h
        db  051h, 0CDh, 021h, 08Eh, 0C3h, 026h, 0A1h, 002h, 000h, 03Dh, 000h, 0A0h, 073h, 025h, 03Dh, 000h
        db  096h, 072h, 020h, 08Bh, 0D0h, 081h, 0E2h, 0FFh, 000h, 081h, 0FAh, 0FFh, 000h, 075h, 0AAh, 006h
        db  08Eh, 0C0h, 026h, 080h, 03Eh, 000h, 000h, 04Dh, 075h, 006h, 026h, 083h, 03Eh, 001h, 000h, 008h
        db  007h, 075h, 096h, 033h, 0DBh, 033h, 0F6h, 066h, 033h, 0D2h, 0B8h, 001h, 080h, 0C3h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
_PTEXT ends
        end
