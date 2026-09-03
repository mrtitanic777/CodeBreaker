; mgamil_pcode_04.asm -- MGAMIL object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

MGAMIL_CREATE_VM:
        db  0FFh, 005h, 030h, 008h, 000h, 000h
        clc
        ret
_PTEXT ends
        end
