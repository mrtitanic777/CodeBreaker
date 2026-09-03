; declicl_pcode_03.asm -- DECLICL object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  03Dh, 00Ah, 003h, 072h, 011h, 0F7h, 0C3h, 003h, 000h, 075h, 00Bh, 033h, 0DBh, 033h, 0F6h, 066h
        db  033h, 0D2h, 0B8h, 000h, 000h, 0C3h, 033h, 0DBh, 033h, 0F6h, 066h, 033h, 0D2h, 0B8h, 001h, 080h
        db  0C3h, 000h
_PTEXT ends
        end
