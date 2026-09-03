; ndis_pcode_05.asm -- NDIS object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  0F7h, 0C3h, 003h, 000h, 075h, 049h, 03Dh, 000h, 004h, 072h, 044h, 066h, 089h, 00Eh, 0CBh, 000h
        db  0BFh, 0B5h, 000h, 066h, 033h, 0C9h, 033h, 0F6h, 0B8h, 003h, 000h, 0FFh, 01Eh, 0CBh, 000h, 00Bh
        db  0C9h, 075h, 005h, 0E8h, 03Ah, 000h, 072h, 027h, 066h, 02Bh, 0D2h, 0B4h, 058h, 02Ah, 0C0h, 0CDh
        db  02Fh, 00Ah, 0C0h, 074h, 012h, 0B4h, 058h, 0B0h, 0D4h, 0CDh, 02Fh, 081h, 0F9h, 0CFh, 03Fh, 075h
        db  006h, 066h, 0BAh, 001h, 000h, 000h, 000h, 033h, 0DBh, 033h, 0F6h, 0B8h, 000h, 000h, 0C3h, 033h
        db  0DBh, 033h, 0F6h, 0B8h, 001h, 080h, 0C3h, 049h, 046h, 053h, 024h, 048h, 04Ch, 050h, 024h, 000h
        db  066h, 060h, 01Eh, 083h, 0ECh, 008h, 08Bh, 0FCh, 0B8h, 040h, 03Dh, 0BAh, 057h, 000h, 0CDh, 021h
        db  072h, 03Bh, 08Bh, 0D8h, 08Bh, 0D7h, 066h, 0C7h, 005h, 070h, 0E9h, 035h, 037h, 0B8h, 002h, 044h
        db  0B9h, 008h, 000h, 0CDh, 021h, 08Bh, 0D0h, 09Ch, 0B4h, 03Eh, 0CDh, 021h, 09Dh, 072h, 01Dh, 083h
        db  0FAh, 008h, 075h, 018h, 066h, 081h, 03Dh, 035h, 037h, 070h, 0EFh, 075h, 00Fh, 06Ah, 008h, 0FFh
        db  05Dh, 004h, 083h, 0C4h, 002h, 00Bh, 0C0h, 075h, 003h, 0F8h, 0EBh, 001h, 0F9h, 066h, 058h, 066h
        db  058h, 01Fh, 066h, 061h, 0C3h, 041h, 04Ch, 057h, 041h, 059h, 053h, 04Ch, 04Fh, 041h, 044h, 04Eh
        db  044h, 049h, 053h, 057h, 052h, 041h, 050h, 050h, 045h, 052h, 000h, 000h, 000h, 000h, 000h
_PTEXT ends
        end
