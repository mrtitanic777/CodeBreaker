; ebios_ldata_03.asm -- EBIOS object 3 segment _LDATA
.386p
_LDATA segment dword public use32 'LDATA'

        db  0F7h, 0C3h, 001h, 000h, 00Fh, 085h, 09Bh, 000h, 0F9h, 0B4h, 0C0h, 0CDh, 015h
_LDATA ends
        end
