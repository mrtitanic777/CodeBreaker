; ndis2sup_pcode_03.asm -- NDIS2SUP object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  04Eh, 044h, 049h, 053h, 048h, 04Ch, 050h, 024h, 000h, 06Fh, 0E9h, 033h, 037h, 000h, 000h, 000h
        db  000h, 0F7h, 0C3h, 003h, 000h, 075h, 069h, 0B8h, 040h, 03Dh, 0BAh, 000h, 000h, 0CDh, 021h, 072h
        db  05Fh, 08Bh, 0D8h, 08Bh, 0FBh, 0BEh, 009h, 000h, 08Bh, 0D6h, 0B9h, 008h, 000h, 0B8h, 002h, 044h
        db  0CDh, 021h, 072h, 046h, 081h, 03Ch, 033h, 037h, 075h, 040h, 081h, 07Ch, 002h, 06Fh, 0EFh, 075h
        db  039h, 08Bh, 0DFh, 0B4h, 03Eh, 0CDh, 021h, 0B8h, 006h, 000h, 050h, 0FFh, 05Ch, 004h, 083h, 0C4h
        db  002h, 00Bh, 0C0h, 074h, 02Bh, 00Bh, 0D2h, 074h, 027h, 00Bh, 0DBh, 074h, 023h, 08Eh, 044h, 006h
        db  08Bh, 0F8h, 026h, 080h, 07Dh, 02Fh, 000h, 075h, 017h, 026h, 083h, 07Dh, 021h, 000h, 074h, 010h
        db  033h, 0C0h, 066h, 033h, 0D2h, 033h, 0F6h, 033h, 0DBh, 0C3h, 08Bh, 0DFh, 0B4h, 03Eh, 0CDh, 021h
        db  0B8h, 001h, 080h, 0EBh, 0EDh, 034h
_PTEXT ends
        end
