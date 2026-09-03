; vmm_pcode_09.asm -- VMM object 9 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  066h, 053h, 006h, 0B8h, 009h, 043h, 0CDh, 02Fh, 066h, 033h, 0D2h, 03Ch, 043h, 075h, 011h, 08Ch
        db  0C0h, 066h, 00Fh, 0B7h, 0D0h, 066h, 0C1h, 0E2h, 004h, 066h, 00Fh, 0B7h, 0DBh, 066h, 003h, 0D3h
        db  0B8h, 000h, 000h, 007h, 066h, 05Bh, 033h, 0DBh, 08Bh, 0F3h, 0C3h, 000h
_PTEXT ends
        end
