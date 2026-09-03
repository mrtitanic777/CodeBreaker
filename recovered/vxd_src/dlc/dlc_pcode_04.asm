; dlc_pcode_04.asm -- DLC object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  044h, 04Ch, 043h, 03Ah, 020h, 052h, 065h, 061h, 06Ch, 020h, 04Dh, 06Fh, 064h, 065h, 020h, 049h
        db  06Eh, 069h, 074h, 020h, 063h, 061h, 06Ch, 06Ch, 065h, 064h, 00Dh, 00Ah, 0B4h, 040h, 0BBh, 001h
        db  000h, 0B9h, 01Ch, 000h, 0BAh, 000h, 000h, 0CDh, 021h, 033h, 0DBh, 033h, 0F6h, 066h, 033h, 0D2h
        db  0B8h, 000h, 000h, 0C3h
_PTEXT ends
        end
