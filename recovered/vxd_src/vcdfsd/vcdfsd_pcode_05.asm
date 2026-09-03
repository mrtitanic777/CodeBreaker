; vcdfsd_pcode_05.asm -- VCDFSD object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  0B8h, 001h, 000h, 0F7h, 0C3h, 003h, 000h, 075h, 026h, 0B8h, 000h, 015h, 033h, 0DBh, 0CDh, 02Fh
        db  00Bh, 0DBh, 074h, 015h, 0B8h, 00Ch, 015h, 033h, 0DBh, 0CDh, 02Fh, 083h, 0FBh, 000h, 075h, 003h
        db  0BBh, 001h, 001h, 066h, 00Fh, 0B7h, 0D3h, 0EBh, 003h, 066h, 033h, 0D2h, 0B8h, 000h, 000h, 033h
        db  0DBh, 033h, 0F6h, 0C3h
_PTEXT ends
        end
