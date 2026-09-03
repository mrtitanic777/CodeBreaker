; snapshot_pcode_04.asm -- SNAPSHOT object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  0B4h, 058h, 0B0h, 0D4h, 0CDh, 02Fh, 072h, 008h, 0B8h, 000h, 000h, 033h, 0DBh, 033h, 0F6h, 0C3h
        db  0B8h, 001h, 080h, 0EBh, 0F6h
_PTEXT ends
        end
