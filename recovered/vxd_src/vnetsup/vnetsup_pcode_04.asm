; vnetsup_pcode_04.asm -- VNETSUP object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        test   ebx, 2a750003h
        cmp    eax, 2572030ah
        mov    eax, 21cd3000h
        cmp    al, 7
        db  00Fh, 087h, 01Ah, 000h, 03Ch, 003h
        db  072h, 016h
        ja     SHORT loc_4_0021
        cmp    ah, 1eh
        db  072h, 00Fh
loc_4_0021:
        db  0E8h, 01Dh, 000h, 00Fh, 082h
        or     byte ptr [eax], al
        xor    ebx, ebx
        xor    esi, esi
        mov    eax, 33c30000h
        db  0DBh, 033h, 0F6h, 0B8h, 001h, 080h, 0C3h, 049h, 046h, 053h, 024h, 048h, 04Ch, 050h, 024h, 000h
        db  066h, 060h, 01Eh, 083h, 0ECh, 008h, 08Bh, 0FCh, 0B8h, 040h, 03Dh, 0BAh, 038h, 000h, 0CDh, 021h
        db  00Fh, 082h, 044h, 000h, 08Bh, 0D8h, 08Bh, 0D7h, 066h, 0C7h, 005h, 070h, 0E9h, 035h, 037h, 0B8h
        db  002h, 044h, 0B9h, 008h, 000h, 0CDh, 021h, 08Bh, 0D0h, 09Ch, 0B4h, 03Eh, 0CDh, 021h, 09Dh, 00Fh
        db  082h, 024h, 000h, 083h, 0FAh, 008h, 00Fh, 085h, 01Dh, 000h, 066h, 081h, 03Dh, 035h, 037h, 070h
        db  0EFh, 00Fh, 085h, 012h, 000h, 06Ah, 008h, 0FFh, 05Dh, 004h, 083h, 0C4h, 002h, 00Bh, 0C0h, 00Fh
        db  085h, 004h, 000h, 0F8h, 0EBh, 002h, 090h, 0F9h, 066h, 058h, 066h, 058h, 01Fh, 066h, 061h, 0C3h
        db  01Ah
_PTEXT ends
        end
