; enable_pcode_06.asm -- ENABLE object 6 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  0B8h, 001h, 080h, 0F7h, 0C3h, 001h, 000h, 075h, 003h, 0B8h, 000h, 000h, 033h, 0DBh, 033h, 0F6h
        db  066h, 033h, 0D2h, 0C3h
_PTEXT ends
        end
