; vdef_pcode_03.asm -- VDEF object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  033h, 0DBh, 033h, 0F6h, 0B8h, 000h, 000h, 0C3h
_PTEXT ends
        end
