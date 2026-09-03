; dosnet_pcode_04.asm -- DOSNET object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  0B8h, 000h, 030h, 0CDh, 021h, 086h, 0E0h, 03Dh, 00Ah, 003h, 072h, 023h, 0B8h, 002h, 05Fh, 033h
        db  0DBh, 00Eh, 01Fh, 00Eh, 007h, 0BEh, 034h, 000h, 0BFh, 034h, 000h, 0CDh, 021h, 073h, 005h, 083h
        db  0F8h, 001h, 074h, 00Bh, 0B8h, 000h, 000h, 033h, 0DBh, 066h, 033h, 0D2h, 033h, 0F6h, 0C3h, 0B8h
        db  001h, 080h, 0EBh, 0F3h, 023h, 044h, 04Fh, 053h, 04Eh, 045h, 054h, 020h, 056h, 069h, 072h, 074h
        db  075h, 061h, 06Ch, 020h, 044h, 065h, 076h, 069h, 063h, 065h, 009h, 028h, 056h, 065h, 072h, 073h
        db  069h, 06Fh, 06Eh, 020h, 034h, 02Eh, 030h, 029h, 000h, 000h, 00Ah, 044h, 04Fh, 053h, 04Eh, 045h
        db  054h, 05Fh, 044h, 044h, 042h, 001h, 000h, 000h, 0FFh, 010h, 000h, 0FFh, 001h, 000h, 030h, 000h
        db  0E4h, 001h, 000h, 000h, 0E4h, 001h, 034h, 000h, 056h, 053h, 05Fh, 056h, 045h, 052h, 053h, 049h
        db  04Fh, 04Eh, 05Fh, 049h, 04Eh, 046h, 04Fh, 000h, 0BDh, 004h, 0EFh, 0FEh, 000h, 000h, 001h, 000h
        db  000h, 000h, 004h, 000h, 0B6h, 003h, 000h, 000h, 000h, 000h, 004h, 000h, 0B6h, 003h, 000h, 000h
        db  03Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 001h, 000h, 005h, 000h, 000h, 000h
        db  01Ah, 000h, 000h, 000h
_PTEXT ends
        end
