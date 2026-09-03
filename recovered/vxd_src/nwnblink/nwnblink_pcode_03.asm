; nwnblink_pcode_03.asm -- NWNBLINK object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  0B8h, 000h, 07Ah, 0CDh, 02Fh, 066h, 033h, 0D2h, 03Ch, 0FFh, 075h, 003h, 0BAh, 001h, 000h, 033h
        db  0C0h, 033h, 0DBh, 033h, 0F6h, 0C3h
_PTEXT ends
        end
