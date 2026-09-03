; qemmfix_pcode_03.asm -- QEMMFIX object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  03Dh, 00Ah, 003h, 072h, 00Bh, 0B8h, 000h, 000h, 033h, 0DBh, 033h, 0F6h, 066h, 033h, 0D2h, 0C3h
        db  0B8h, 001h, 080h, 0EBh, 0F3h
_PTEXT ends
        end
