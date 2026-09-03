; vmcpd_pcode_05.asm -- VMCPD object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  033h, 0DBh, 066h, 033h, 0D2h, 00Fh, 001h, 0E0h, 08Ah, 0D0h, 080h, 0E2h, 004h, 0A8h, 004h, 074h
        db  005h, 024h, 0FBh, 00Fh, 001h, 0F0h, 0E8h, 01Ah, 000h, 00Ah, 0D0h, 0B8h, 000h, 000h, 08Bh, 0F3h
        db  0C3h, 000h, 000h, 000h, 000h, 000h, 0C0h, 07Eh, 001h, 050h, 041h, 000h, 000h, 000h, 080h, 0FFh
        db  0FFh, 047h, 041h, 0DBh, 0E3h, 0BEh, 021h, 000h, 0C7h, 004h, 05Ah, 05Ah, 0DDh, 03Ch, 080h, 03Ch
        db  000h, 075h, 02Ch, 0D9h, 03Ch, 08Bh, 004h, 025h, 03Fh, 010h, 083h, 0F8h, 03Fh, 075h, 020h, 0DDh
        db  006h, 023h, 000h, 0DCh, 036h, 02Bh, 000h, 0DCh, 00Eh, 02Bh, 000h, 0DCh, 02Eh, 023h, 000h, 0D9h
        db  0E8h, 0DEh, 0D9h, 0DFh, 0E0h, 0DBh, 0E3h, 09Eh, 0B0h, 003h, 073h, 002h, 00Ch, 008h, 0C3h, 02Bh
        db  0C0h, 0EBh, 0FBh
_PTEXT ends
        end
