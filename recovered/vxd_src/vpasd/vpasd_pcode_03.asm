; vpasd_pcode_03.asm -- VPASD object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        mov    eax, 2fh
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 018h, 001h, 001h, 000h
        mov    eax, 15000eh
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 01Ch, 001h, 001h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        clc
        ret
_PTEXT ends
        end
