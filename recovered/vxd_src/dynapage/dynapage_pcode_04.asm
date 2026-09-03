; dynapage_pcode_04.asm -- DYNAPAGE object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  053h, 04Dh, 041h, 052h, 054h, 041h, 041h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0F7h, 0C3h, 003h, 000h, 075h, 067h, 0B8h, 007h, 016h, 0BBh, 021h
        db  000h, 033h, 0C9h, 0CDh, 02Fh, 00Bh, 0C0h, 075h, 00Ah, 08Ch, 0C2h, 066h, 0C1h, 0E2h, 010h, 08Bh
        db  0D7h, 0EBh, 047h, 0B8h, 000h, 03Dh, 0BAh, 000h, 000h, 0CDh, 021h, 072h, 03Ah, 08Bh, 0D8h, 0B8h
        db  000h, 044h, 0CDh, 021h, 072h, 02Dh, 0F7h, 0C2h, 080h, 000h, 074h, 027h, 0BAh, 009h, 000h, 0B9h
        db  02Ch, 000h, 0B8h, 002h, 044h, 0CDh, 021h, 072h, 01Ah, 083h, 0F8h, 02Ch, 075h, 015h, 0B4h, 03Eh
        db  0CDh, 021h, 066h, 033h, 0D2h, 080h, 03Eh, 018h, 000h, 003h, 072h, 00Eh, 066h, 08Bh, 016h, 031h
        db  000h, 0EBh, 007h, 0B4h, 03Eh, 0CDh, 021h, 066h, 033h, 0D2h, 033h, 0DBh, 033h, 0F6h, 0B8h, 000h
        db  000h, 0C3h, 033h, 0DBh, 033h, 0F6h, 0B8h, 001h, 080h, 0C3h
_PTEXT ends
        end
