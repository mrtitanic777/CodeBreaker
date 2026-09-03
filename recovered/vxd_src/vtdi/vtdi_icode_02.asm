; vtdi_icode_02.asm -- VTDI object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  0FEh, 005h, 074h, 004h, 000h, 000h
        jne    SHORT loc_2_001A
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  089h, 01Dh, 070h, 004h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 040h, 000h
loc_2_001A:
        ret
        db  029h
_ITEXT ends
        end
