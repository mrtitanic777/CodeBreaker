; vcond_pcode_05.asm -- VCOND object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  0F7h, 0C3h, 003h, 000h, 075h, 015h, 0B4h, 062h, 0CDh, 021h, 08Bh, 0D3h, 066h, 0C1h, 0E2h, 010h
        db  0BAh, 050h, 000h, 033h, 0DBh, 033h, 0F6h, 0B8h, 000h, 000h, 0C3h, 0B8h, 001h, 080h, 033h, 0DBh
        db  033h, 0F6h, 0C3h, 000h
_PTEXT ends
        end
