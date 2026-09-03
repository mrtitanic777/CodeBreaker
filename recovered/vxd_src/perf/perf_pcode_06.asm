; perf_pcode_06.asm -- PERF object 6 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  033h, 0F6h, 0F7h, 0C3h, 003h, 000h, 075h, 009h, 033h, 0DBh, 066h, 033h, 0D2h, 0B8h, 000h, 000h
        db  0C3h, 033h, 0DBh, 0B8h, 001h, 080h, 0C3h, 021h
_PTEXT ends
        end
