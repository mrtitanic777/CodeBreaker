; dosmgr_pcode_16.asm -- DOSMGR object 16 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

DOSMGR_End_V86_App:
        ret
        db  000h, 000h, 000h
_PTEXT ends
        end
