; chips_pcode_04.asm -- CHIPS object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  0E8h, 02Ch, 001h, 0FCh, 0FFh, 0F8h, 0C3h
_PTEXT ends
        end
