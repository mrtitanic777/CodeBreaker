; vtd_pcode_07.asm -- VTD object 7 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_7_0000:
        db  03Bh, 01Dh, 0FCh, 004h, 000h, 000h
        jne    SHORT loc_7_0013
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0E9h, 0CAh, 001h, 0F9h, 0FFh
loc_7_0013:
        clc
        ret
_PTEXT ends
        end
