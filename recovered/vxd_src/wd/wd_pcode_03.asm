; wd_pcode_03.asm -- WD object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

WD_SYSTEM_EXIT:
        db  0E8h, 03Ah, 000h, 0FDh, 0FFh
        clc
        ret
_PTEXT ends
        end
