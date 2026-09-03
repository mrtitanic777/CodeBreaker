; vserver_pcode_04.asm -- VSERVER object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  0B4h, 058h, 0B0h, 0D4h, 0B9h, 000h, 000h, 0CDh, 02Fh, 081h, 0F9h, 0CFh, 03Fh, 075h, 011h, 00Bh
        db  0C0h, 074h, 00Dh, 083h, 0FAh, 002h, 075h, 008h, 0B8h, 000h, 000h, 033h, 0DBh, 033h, 0F6h, 0C3h
        db  0B8h, 003h, 080h, 033h, 0DBh, 033h, 0F6h, 0C3h
_PTEXT ends
        end
