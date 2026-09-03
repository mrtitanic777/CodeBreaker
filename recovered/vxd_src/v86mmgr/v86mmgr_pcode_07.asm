; v86mmgr_pcode_07.asm -- V86MMGR object 7 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  00Dh, 00Ah, 045h, 052h, 052h, 04Fh, 052h, 03Ah, 020h, 043h, 061h, 06Eh, 06Eh, 06Fh, 074h, 020h
        db  064h, 069h, 073h, 061h, 062h, 06Ch, 065h, 020h, 074h, 068h, 065h, 020h, 069h, 06Eh, 073h, 074h
        db  061h, 06Ch, 06Ch, 065h, 064h, 020h, 033h, 038h, 036h, 020h, 065h, 078h, 070h, 061h, 06Eh, 064h
        db  065h, 064h, 02Dh, 06Dh, 065h, 06Dh, 06Fh, 072h, 079h, 020h, 064h, 072h, 069h, 076h, 065h, 072h
        db  02Eh, 00Dh, 00Ah, 024h, 00Dh, 00Ah, 045h, 052h, 052h, 04Fh, 052h, 03Ah, 020h, 041h, 070h, 070h
        db  06Ch, 069h, 063h, 061h, 074h, 069h, 06Fh, 06Eh, 073h, 020h, 061h, 072h, 065h, 020h, 061h, 06Ch
        db  072h, 065h, 061h, 064h, 079h, 020h, 075h, 073h, 069h, 06Eh, 067h, 020h, 065h, 078h, 070h, 061h
        db  06Eh, 064h, 065h, 064h, 020h, 06Dh, 065h, 06Dh, 06Fh, 072h, 079h, 02Eh, 00Dh, 00Ah, 024h, 00Dh
        db  00Ah, 045h, 052h, 052h, 04Fh, 052h, 03Ah, 020h, 043h, 061h, 06Eh, 06Eh, 06Fh, 074h, 020h, 069h
        db  06Eh, 073h, 074h, 061h, 06Ch, 06Ch, 020h, 074h, 068h, 065h, 020h, 033h, 038h, 036h, 020h, 065h
        db  06Eh, 068h, 061h, 06Eh, 063h, 065h, 064h, 020h, 06Dh, 06Fh, 064h, 065h, 020h, 065h, 078h, 070h
        db  061h, 06Eh, 064h, 065h, 064h, 02Dh, 06Dh, 065h, 06Dh, 06Fh, 072h, 079h, 020h, 064h, 072h, 069h
        db  076h, 065h, 072h, 02Eh, 000h, 000h, 045h, 04Dh, 04Dh, 058h, 058h, 058h, 058h, 030h, 000h, 04Dh
        db  049h, 043h, 052h, 04Fh, 053h, 04Fh, 046h, 054h, 020h, 045h, 058h, 050h, 041h, 04Eh, 044h, 045h
        db  044h, 020h, 04Dh, 045h, 04Dh, 04Fh, 052h, 059h, 020h, 04Dh, 041h, 04Eh, 041h, 047h, 045h, 052h
        db  020h, 033h, 038h, 036h, 043h, 04Fh, 04Dh, 050h, 041h, 051h, 020h, 045h, 058h, 050h, 041h, 04Eh
        db  044h, 045h, 044h, 020h, 04Dh, 045h, 04Dh, 04Fh, 052h, 059h, 020h, 04Dh, 041h, 04Eh, 041h, 047h
        db  045h, 052h, 020h, 033h, 038h, 036h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 000h
        db  000h, 033h, 0C0h, 08Eh, 0D8h, 0A1h, 09Eh, 001h, 08Eh, 0D8h, 0BEh, 014h, 000h, 0FCh, 0BFh, 0CFh
        db  000h, 00Eh, 007h, 0B9h, 025h, 000h, 0FCh, 0F3h, 0A6h, 074h, 00Eh, 0BEh, 014h, 000h, 0BFh, 0F4h
        db  000h, 0B9h, 022h, 000h, 0FCh, 0F3h, 0A6h, 075h, 011h, 033h, 0F6h, 02Eh, 08Ch, 01Eh, 018h, 001h
        db  08Bh, 04Ch, 012h, 02Eh, 089h, 00Eh, 016h, 001h, 0F8h, 0C3h, 0F9h, 0EBh, 0FCh, 033h, 0C0h, 02Eh
        db  0FFh, 01Eh, 016h, 001h, 0F6h, 0C4h, 001h, 075h, 00Ah, 0B8h, 001h, 001h, 02Eh, 0FFh, 01Eh, 016h
        db  001h, 072h, 002h, 0F8h, 0C3h, 0F9h, 0EBh, 0FCh, 033h, 0C0h, 02Eh, 0FFh, 01Eh, 016h, 001h, 0F6h
        db  0C4h, 001h, 075h, 026h, 0B8h, 000h, 046h, 0CDh, 067h, 080h, 0FCh, 000h, 075h, 01Ch, 08Bh, 0C8h
        db  0B8h, 000h, 04Bh, 0CDh, 067h, 080h, 0FCh, 000h, 075h, 010h, 080h, 0F9h, 040h, 072h, 005h, 00Bh
        db  0DBh, 074h, 001h, 04Bh, 083h, 0FBh, 000h, 0F9h, 075h, 001h, 0F8h, 0C3h, 033h, 0C0h, 08Eh, 0D8h
        db  0A1h, 09Eh, 001h, 08Eh, 0D8h, 0BEh, 00Ah, 000h, 0BFh, 0C6h, 000h, 00Eh, 007h, 0B9h, 008h, 000h
        db  0FCh, 0F3h, 0A6h, 075h, 002h, 0F8h, 0C3h, 0F9h, 0EBh, 0FCh, 0E8h, 0DFh, 0FFh, 00Fh, 082h, 09Ah
        db  000h, 0B2h, 001h, 0B8h, 007h, 033h, 0CDh, 021h, 00Eh, 01Fh, 0BAh, 0C6h, 000h, 0B8h, 000h, 03Dh
        db  0CDh, 021h, 09Ch, 050h, 0B2h, 000h, 0B8h, 007h, 033h, 0CDh, 021h, 058h, 09Dh, 073h, 00Ah, 083h
        db  0F8h, 004h, 075h, 04Bh, 0BAh, 07Fh, 000h, 0EBh, 064h, 08Bh, 0D8h, 0B8h, 000h, 044h, 0CDh, 021h
        db  072h, 038h, 0F7h, 0C2h, 080h, 000h, 074h, 032h, 0F7h, 0C2h, 000h, 040h, 074h, 02Ch, 0BAh, 01Ah
        db  001h, 0B9h, 006h, 000h, 0B8h, 002h, 044h, 0CDh, 021h, 072h, 01Fh, 03Bh, 0C1h, 075h, 01Bh, 08Bh
        db  0F2h, 08Bh, 044h, 004h, 086h, 0E0h, 03Dh, 000h, 001h, 072h, 00Fh, 03Dh, 063h, 001h, 077h, 00Ah
        db  066h, 08Bh, 014h, 0B8h, 000h, 03Eh, 0CDh, 021h, 0EBh, 034h, 0B8h, 000h, 03Eh, 0CDh, 021h, 066h
        db  0BAh, 002h, 000h, 000h, 000h, 0E8h, 0D9h, 0FEh, 072h, 024h, 0E8h, 02Bh, 0FFh, 072h, 017h, 066h
        db  0BAh, 001h, 000h, 000h, 000h, 0E8h, 005h, 0FFh, 073h, 014h, 0BAh, 000h, 000h, 00Eh, 01Fh, 0B8h
        db  000h, 009h, 0CDh, 021h, 0EBh, 011h, 0BAh, 044h, 000h, 0EBh, 0F2h, 066h, 033h, 0D2h, 033h, 0DBh
        db  033h, 0F6h, 0B8h, 000h, 000h, 0F8h, 0C3h, 0F9h, 0C3h, 000h, 0F7h, 0C3h, 001h, 000h, 075h, 011h
        db  0E8h, 047h, 0FFh, 072h, 001h, 0C3h, 0B8h, 002h, 080h, 033h, 0DBh, 066h, 033h, 0D2h, 033h, 0F6h
        db  0C3h, 0B8h, 001h, 080h, 0EBh, 0F3h
_PTEXT ends
        end
