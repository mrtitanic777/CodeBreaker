; vpicd_pcode_07.asm -- VPICD object 7 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_7_0000:
        db  0C7h, 005h, 0DCh, 025h, 000h, 000h, 000h, 000h, 000h, 000h
        ret
        db  000h
_PTEXT ends
        end
