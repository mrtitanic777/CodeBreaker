; nwredir_pcode_06.asm -- NWREDIR object 6 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  0F7h, 0C3h, 003h, 000h, 075h, 042h, 066h, 02Bh, 0D2h, 0B4h, 058h, 02Ah, 0C0h, 0CDh, 02Fh, 00Ah
        db  0C0h, 074h, 00Ch, 0B4h, 058h, 0B0h, 0D4h, 0CDh, 02Fh, 081h, 0F9h, 0CFh, 03Fh, 074h, 00Bh, 0B8h
        db  000h, 0EAh, 033h, 0DBh, 0CDh, 021h, 00Ah, 0FBh, 075h, 01Eh, 0B4h, 052h, 0CDh, 021h, 066h, 08Ch
        db  0C2h, 066h, 00Fh, 0B7h, 0D2h, 066h, 0C1h, 0E2h, 004h, 066h, 00Fh, 0B7h, 0DBh, 066h, 003h, 0D3h
        db  0B8h, 000h, 000h, 033h, 0DBh, 033h, 0F6h, 0C3h, 0B8h, 001h, 000h, 0C3h
_PTEXT ends
        end
