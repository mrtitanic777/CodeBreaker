; vcache_pcode_03.asm -- VCACHE object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  0F7h, 0C3h, 003h, 000h, 075h, 010h, 0E8h, 01Eh, 000h, 072h, 00Bh, 066h, 033h, 0D2h, 033h, 0DBh
        db  033h, 0F6h, 0B8h, 000h, 000h, 0C3h, 033h, 0DBh, 033h, 0F6h, 0B8h, 001h, 080h, 0C3h, 049h, 046h
        db  053h, 024h, 048h, 04Ch, 050h, 024h, 000h, 066h, 060h, 01Eh, 083h, 0ECh, 008h, 08Bh, 0FCh, 0B8h
        db  040h, 03Dh, 0BAh, 01Eh, 000h, 0CDh, 021h, 072h, 03Bh, 08Bh, 0D8h, 08Bh, 0D7h, 066h, 0C7h, 005h
        db  070h, 0E9h, 035h, 037h, 0B8h, 002h, 044h, 0B9h, 008h, 000h, 0CDh, 021h, 08Bh, 0D0h, 09Ch, 0B4h
        db  03Eh, 0CDh, 021h, 09Dh, 072h, 01Dh, 083h, 0FAh, 008h, 075h, 018h, 066h, 081h, 03Dh, 035h, 037h
        db  070h, 0EFh, 075h, 00Fh, 06Ah, 008h, 0FFh, 05Dh, 004h, 083h, 0C4h, 002h, 00Bh, 0C0h, 075h, 003h
        db  0F8h, 0EBh, 001h, 0F9h, 066h, 058h, 066h, 058h, 01Fh, 066h, 061h, 0C3h
_PTEXT ends
        end
