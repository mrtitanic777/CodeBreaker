; spendis_ldata_03.asm -- SPENDIS object 3 segment _LDATA
.386p
_LDATA segment dword public use32 'LDATA'

        db  0F7h, 0C3h, 003h, 000h, 075h, 008h, 033h, 0DBh, 033h, 0F6h, 0B8h, 000h, 000h, 0C3h, 033h, 0DBh
        db  033h, 0F6h, 0B8h, 001h, 000h, 0C3h, 024h, 04Eh, 044h, 049h, 053h, 033h, 020h, 050h, 043h, 04Dh
        db  043h, 049h, 041h, 020h, 045h, 074h, 068h, 065h, 072h, 06Eh, 065h, 074h, 020h, 064h, 072h, 069h
        db  076h, 065h, 072h, 020h, 066h, 06Fh, 072h, 020h, 057h, 039h, 035h, 000h, 000h, 00Bh, 053h, 050h
        db  045h, 04Eh, 044h, 049h, 053h, 05Fh, 044h, 044h, 042h, 001h, 000h, 000h
_LDATA ends
        end
