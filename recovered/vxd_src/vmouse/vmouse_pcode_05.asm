; vmouse_pcode_05.asm -- VMOUSE object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  066h, 033h, 0D2h, 033h, 0DBh, 033h, 0F6h, 0B8h, 000h, 000h, 0C3h, 023h
_PTEXT ends
        end
