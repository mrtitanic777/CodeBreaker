; vpd_pcode_05.asm -- VPD object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  0F7h, 0C3h, 003h, 000h, 075h, 00Bh, 066h, 033h, 0D2h, 033h, 0DBh, 033h, 0F6h, 0B8h, 000h, 000h
        db  0C3h, 033h, 0DBh, 033h, 0F6h, 0B8h, 001h, 080h, 0C3h
_PTEXT ends
        end
