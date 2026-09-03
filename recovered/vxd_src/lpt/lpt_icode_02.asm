; lpt_icode_02.asm -- LPT object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  0CDh, 020h, 000h, 000h, 02Bh, 000h
        jae    9
        ret
        db  068h, 000h, 00Dh, 000h, 000h, 0CDh, 020h, 001h, 000h, 02Bh, 000h, 059h, 080h, 03Dh, 020h, 000h
        db  000h, 000h, 000h, 074h, 002h, 0F8h, 0C3h, 0F8h, 0C3h, 000h, 000h
_ITEXT ends
        end
