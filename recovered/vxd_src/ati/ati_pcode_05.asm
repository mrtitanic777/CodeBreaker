; ati_pcode_05.asm -- ATI object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

ATI_DESTROY_VM:
        db  0FFh, 00Dh, 08Ch, 001h, 000h, 000h
        jge    SHORT loc_5_0012
        db  0C7h, 005h, 08Ch, 001h, 000h, 000h, 000h, 000h, 000h, 000h
loc_5_0012:
        clc
        ret
_PTEXT ends
        end
