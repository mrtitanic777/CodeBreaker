; vshare_pcode_03.asm -- VSHARE object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  0B9h, 001h, 080h, 0F7h, 0C3h, 001h, 000h, 075h, 01Ah, 0B9h, 001h, 000h, 0B4h, 052h, 0CDh, 021h
        db  026h, 00Fh, 0B6h, 016h, 004h, 000h, 083h, 0FAh, 001h, 077h, 008h, 066h, 0C1h, 0E2h, 010h, 08Ch
        db  0C2h, 033h, 0C9h, 033h, 0DBh, 033h, 0F6h, 08Bh, 0C1h, 0C3h
_PTEXT ends
        end
