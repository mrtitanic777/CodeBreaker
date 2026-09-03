; vmd_pcode_03.asm -- VMD object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  0F7h, 0C3h, 003h, 000h, 075h, 032h, 006h, 0B8h, 033h, 035h, 0CDh, 021h, 066h, 033h, 0D2h, 08Ch
        db  0C2h, 00Bh, 0D3h, 074h, 01Ah, 026h, 080h, 03Fh, 0CFh, 075h, 005h, 066h, 033h, 0D2h, 0EBh, 00Fh
        db  066h, 0BAh, 0FFh, 0FFh, 0FFh, 0FFh, 0B8h, 024h, 000h, 033h, 0C9h, 0CDh, 033h, 08Bh, 0D1h, 033h
        db  0DBh, 033h, 0F6h, 0B8h, 000h, 000h, 007h, 0C3h, 033h, 0DBh, 033h, 0F6h, 0B8h, 001h, 080h, 007h
        db  0C3h, 020h
_PTEXT ends
        end
