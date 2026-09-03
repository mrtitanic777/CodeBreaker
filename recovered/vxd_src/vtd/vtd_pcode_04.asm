; vtd_pcode_04.asm -- VTD object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  0F7h, 0C3h, 001h, 000h, 075h, 027h, 0B8h, 012h, 016h, 0CDh, 02Fh, 066h, 033h, 0D2h, 03Bh, 0C2h
        db  075h, 016h, 066h, 026h, 08Bh, 047h, 002h, 08Bh, 0D0h, 066h, 0C1h, 0E8h, 010h, 066h, 0C1h, 0E0h
        db  004h, 066h, 003h, 0D0h, 033h, 0C0h, 0EBh, 008h, 0B8h, 002h, 000h, 0EBh, 003h, 0B8h, 001h, 080h
        db  033h, 0DBh, 033h, 0F6h, 0C3h
_PTEXT ends
        end
