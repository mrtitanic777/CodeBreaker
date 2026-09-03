; ati_pcode_04.asm -- ATI object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

ATI_CREATE_VM:
        mov    edx, 1ceh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 1cfh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        db  0FFh, 005h, 08Ch, 001h, 000h, 000h
        clc
        ret
_PTEXT ends
        end
