; parity_icode_03.asm -- PARITY object 3 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_3_0000:
        db  0CDh, 020h, 0BBh, 000h, 001h, 000h
        db  089h, 01Dh, 014h, 001h, 000h, 000h
        db  0CDh, 020h, 016h, 000h, 001h, 000h
        db  089h, 035h, 010h, 001h, 000h, 000h
        db  0BEh, 00Bh, 000h, 000h, 000h
        db  0CDh, 020h, 017h, 000h, 001h, 000h
        clc
        ret
        db  023h, 050h, 041h
_ITEXT ends
        end
