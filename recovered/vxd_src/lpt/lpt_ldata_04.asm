; lpt_ldata_04.asm -- LPT object 4 segment _LDATA
.386p
_LDATA segment dword public use32 'LDATA'

        db  06Ch, 070h, 074h, 000h, 045h, 06Eh, 061h, 062h, 06Ch, 065h, 053h, 068h, 061h, 072h, 069h, 06Eh
        db  067h, 000h, 000h, 000h, 045h, 043h, 050h, 044h, 065h, 076h, 069h, 063h, 065h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
_LDATA ends
        end
