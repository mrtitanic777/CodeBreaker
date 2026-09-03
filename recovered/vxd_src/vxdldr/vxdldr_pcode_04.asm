; vxdldr_pcode_04.asm -- VXDLDR object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  0CDh, 020h, 0B6h, 000h, 001h, 000h, 08Bh, 0F2h, 0BFh, 0F0h, 000h, 000h, 000h, 0F3h, 0A4h, 06Ah
        db  000h, 06Ah, 010h, 0CDh, 020h, 001h, 001h, 001h, 000h, 059h, 059h, 0A3h, 000h, 000h, 000h, 000h
        db  085h, 0C0h, 074h, 01Ah, 0CDh, 020h, 0C1h, 000h, 001h, 000h, 074h, 007h, 0C6h, 005h, 0C5h, 000h
        db  000h, 000h, 001h, 06Ah, 001h, 0CDh, 020h, 001h, 000h, 033h, 000h, 059h, 0F8h, 0C3h, 0CDh, 020h
        db  0BFh, 000h, 001h, 000h, 0C6h, 005h, 0E4h, 000h, 000h, 000h, 001h, 0F8h, 0C3h, 0CDh, 020h, 003h
        db  000h, 001h, 000h, 0CDh, 020h, 00Ah, 000h, 006h, 000h, 089h, 01Dh, 08Ch, 001h, 000h, 000h, 0A3h
        db  090h, 001h, 000h, 000h, 089h, 00Dh, 094h, 001h, 000h, 000h, 089h, 015h, 098h, 001h, 000h, 000h
        db  00Bh, 0D2h, 075h, 028h, 06Ah, 000h, 068h, 000h, 000h, 001h, 000h, 0CDh, 020h, 0A9h, 000h, 001h
        db  000h, 059h, 059h, 00Bh, 0C0h, 074h, 015h, 083h, 0C0h, 00Fh, 0C1h, 0E8h, 004h, 0B9h, 000h, 000h
        db  001h, 000h, 08Bh, 0D1h, 0CDh, 020h, 00Bh, 000h, 006h, 000h, 0F8h, 0C3h, 0F9h, 0C3h, 050h, 052h
        db  08Bh, 01Dh, 08Ch, 001h, 000h, 000h, 0A1h, 090h, 001h, 000h, 000h, 08Bh, 00Dh, 094h, 001h, 000h
        db  000h, 08Bh, 015h, 098h, 001h, 000h, 000h, 0CDh, 020h, 00Bh, 000h, 006h, 000h, 0CDh, 020h, 0AAh
        db  000h, 001h, 000h, 05Ah, 058h, 0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  083h, 0ECh, 00Ch, 056h, 057h, 08Bh, 074h, 024h, 01Ch, 08Bh, 07Ch, 024h, 018h, 056h, 057h, 0E8h
        db  0C2h, 019h, 0FDh, 0FFh, 083h, 0C4h, 008h, 083h, 03Eh, 000h, 076h, 00Bh, 0B8h, 001h, 000h, 000h
        db  000h, 05Fh, 05Eh, 083h, 0C4h, 00Ch, 0C3h, 06Ah, 000h, 068h, 000h, 000h, 001h, 000h, 0E8h, 07Dh
        db  000h, 000h, 000h, 083h, 0C4h, 008h, 00Bh, 0C0h, 074h, 02Eh, 0C1h, 0E8h, 004h, 066h, 089h, 044h
        db  024h, 010h, 0C7h, 044h, 024h, 00Ch, 000h, 000h, 001h, 000h, 0C7h, 044h, 024h, 008h, 000h, 000h
        db  001h, 000h, 08Dh, 044h, 024h, 008h, 050h, 057h, 0E8h, 099h, 019h, 0FDh, 0FFh, 083h, 0C4h, 008h
        db  02Bh, 0C0h, 05Fh, 05Eh, 083h, 0C4h, 00Ch, 0C3h, 0B8h, 002h, 000h, 000h, 000h, 05Fh, 05Eh, 083h
        db  0C4h, 00Ch, 0C3h, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        db  08Bh, 044h, 024h, 008h, 050h, 08Bh, 044h, 024h, 008h, 050h, 0E8h, 067h, 019h, 0FDh, 0FFh, 083h
        db  0C4h, 008h, 0E8h, 01Fh, 000h, 000h, 000h, 00Bh, 0C0h, 074h, 005h, 02Bh, 0C0h, 0C3h, 08Bh, 0FFh
        db  0B8h, 001h, 000h, 000h, 000h, 0C3h, 08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 0A9h, 080h, 001h, 000h, 0CDh, 020h, 0AAh, 080h, 001h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
_PELDR_LoadModule:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_PELDR_LoadModuleEx:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_PELDR_GetModuleHandle:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_PELDR_GetModuleUsage:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_PELDR_GetEntryPoint:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_PELDR_GetProcAddress:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_PELDR_AddExportTable:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_PELDR_RemoveExportTable:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_PELDR_FreeModule:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_PELDR_InitCompleted:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   esi
        db  0A1h, 0E8h, 000h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_4_1010
        sub    eax, eax
        pop    esi
        ret
        db  08Bh, 0FFh
loc_4_1010:
        push   eax
        db  0E8h, 024h, 01Ah, 000h, 000h
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_103F
        db  08Bh, 00Dh, 0E8h, 000h, 000h, 000h
        mov    esi, dword ptr [esp + 8]
loc_4_1027:
        cmp    esi, eax
        je     SHORT loc_4_1043
        push   eax
        push   ecx
        db  0E8h, 018h, 01Ah, 000h, 000h
        add    esp, 8
        or     eax, eax
        db  08Bh, 00Dh, 0E8h, 000h, 000h, 000h
        jne    SHORT loc_4_1027
loc_4_103F:
        sub    eax, eax
        pop    esi
        ret
loc_4_1043:
        mov    eax, 1
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 056h, 057h, 08Bh, 07Ch, 024h, 010h, 08Bh, 007h, 00Bh, 0C0h
        db  075h, 005h, 02Bh, 0C0h, 05Fh, 05Eh, 0C3h, 050h
loc_4_1062:
        db  0E8h, 0D3h, 019h, 000h, 000h
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_1086
        db  08Bh, 074h
loc_4_1070:
        and    al, 0ch
loc_4_1072:
        cmp    eax, esi
loc_4_1074:
        je     SHORT loc_4_1090
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 0CBh, 019h, 000h, 000h
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_1072
loc_4_1086:
        sub    eax, eax
        pop    edi
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_4_1090:
        mov    eax, 1
        pop    edi
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 053h, 056h, 057h, 055h, 08Bh, 05Ch, 024h, 014h
        db  08Bh, 083h, 008h, 002h, 000h, 000h, 033h, 0F6h, 050h, 0E8h, 0EAh, 015h, 000h, 000h, 083h, 0C4h
        db  004h, 033h, 0C9h, 066h, 08Bh, 048h, 014h, 066h
loc_4_10C0:
        mov    esi, dword ptr [eax + 6]
        lea    edx, [eax + ecx + 18h]
        lea    ecx, [esi + esi*4]
        lea    ecx, [edx + ecx*8]
        sub    ecx, 28h
loc_4_10D0:
        db  0BFh, 0F4h, 001h, 000h, 000h
        mov    eax, dword ptr [ecx]
        cmp    eax, dword ptr [edi]
        jne    SHORT loc_4_10EE
        mov    ax, word ptr [ecx + 4]
        db  066h
loc_4_10E0:
        cmp    eax, dword ptr [edi + 4]
        jne    SHORT loc_4_10EE
        mov    al, byte ptr [ecx + 6]
        cmp    al, byte ptr [edi + 6]
        jne    SHORT loc_4_10EE
        dec    esi
loc_4_10EE:
        sub    edi, edi
        cmp    esi, edi
        je     SHORT loc_4_1131
        mov    eax, dword ptr [esp + 18h]
loc_4_10F8:
        mov    ecx, dword ptr [edx + 8]
        mov    ebx, dword ptr [edx + 10h]
        test   cl, 1
        je     SHORT loc_4_1104
        inc    ecx
loc_4_1104:
        test   bl, 1
        je     SHORT loc_4_110A
        inc    ebx
loc_4_110A:
        or     ecx, ecx
        jne    SHORT loc_4_1110
        mov    ecx, ebx
loc_4_1110:
        mov    ebx, dword ptr [esp + 14h]
        mov    ebp, dword ptr [edx + 0ch]
        mov    ebx, dword ptr [ebx + 208h]
        add    ebx, ebp
        cmp    ebx, eax
        ja     SHORT loc_4_1129
        add    ecx, ebx
        cmp    ecx, eax
        ja     SHORT loc_4_1138
loc_4_1129:
        add    edx, 28h
        inc    edi
        cmp    esi, edi
        ja     SHORT loc_4_10F8
loc_4_1131:
        sub    eax, eax
loc_4_1133:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_4_1138:
        db  0B9h, 0FCh, 001h, 000h, 000h
        mov    esi, dword ptr [edx]
        cmp    esi, dword ptr [ecx]
        jne    SHORT loc_4_1133
        mov    dl, byte ptr [edx + 4]
        cmp    dl, byte ptr [ecx + 4]
        jne    SHORT loc_4_1133
        mov    ebx, dword ptr [esp + 14h]
        cmp    dword ptr [ebx + 210h], 0
        je     SHORT loc_4_1133
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h, 053h, 056h, 057h, 055h, 08Bh, 04Ch, 024h, 014h, 08Bh, 081h, 008h, 002h, 000h, 000h, 050h
        db  0E8h, 02Ch, 015h, 000h, 000h, 083h, 0C4h, 004h, 08Bh, 0E8h, 033h, 0C0h, 066h, 08Bh, 045h, 014h
        db  033h, 0FFh, 066h, 08Bh, 07Dh, 006h, 08Dh, 054h, 005h, 018h, 08Dh, 034h, 0BFh, 08Dh, 034h, 0F2h
        db  083h, 0EEh, 028h, 0B9h, 004h, 002h, 000h, 000h, 08Bh, 006h, 03Bh, 001h, 075h, 013h, 066h, 08Bh
        db  046h, 004h, 066h, 03Bh, 041h, 004h, 075h, 009h, 08Ah, 046h, 006h, 03Ah, 041h, 006h, 075h, 001h
        db  04Fh, 02Bh, 0F6h, 03Bh, 0FEh, 00Fh, 084h, 0C2h, 000h, 000h, 000h, 08Bh, 0CAh, 08Bh, 05Ch, 024h
        db  018h, 08Ah, 001h, 03Ah, 003h, 075h, 01Ah, 00Ah, 0C0h, 074h, 012h, 08Ah, 041h, 001h, 03Ah, 043h
        db  001h, 075h, 00Eh, 083h, 0C1h, 002h, 083h, 0C3h, 002h, 00Ah, 0C0h, 075h, 0E4h, 02Bh, 0C0h, 0EBh
        db  005h, 01Bh, 0C0h, 083h, 0D8h, 0FFh, 00Bh, 0C0h, 074h, 00Dh, 083h, 0C2h, 028h, 046h, 03Bh, 0FEh
        db  077h, 0C9h, 05Dh, 05Fh, 05Eh, 05Bh, 0C3h, 08Bh, 042h, 008h, 08Bh, 04Ah, 010h, 0A8h, 001h, 074h
        db  001h, 040h, 0F6h, 0C1h, 001h, 074h, 001h, 041h, 00Bh, 0C0h, 075h, 002h, 08Bh, 0C1h, 08Bh, 04Ch
        db  024h, 014h, 08Bh, 052h, 00Ch, 003h, 091h, 008h, 002h, 000h, 000h, 08Dh, 08Ah, 0FFh, 00Fh, 000h
        db  000h, 0C1h, 0E9h, 00Ch, 003h, 0D0h, 0C1h, 0EAh, 00Ch, 03Bh, 0D1h, 073h, 004h, 02Bh, 0D2h, 0EBh
        db  01Bh, 02Bh, 0D1h, 0C1h, 0EAh, 00Ch, 00Bh, 0D2h, 075h, 016h, 08Bh, 0DFh, 02Bh, 0DEh, 083h, 0FBh
        db  001h, 075h, 009h, 00Bh, 0C0h, 074h, 005h, 0BAh, 001h, 000h, 000h, 000h, 00Bh, 0D2h, 074h, 022h
        db  06Ah, 000h, 052h, 051h, 0E8h, 028h, 018h, 000h, 000h, 083h, 0C4h, 00Ch, 00Bh, 0C0h, 074h, 012h
        db  08Bh, 044h, 024h, 018h, 08Bh, 04Ch, 024h, 014h, 050h, 051h, 0E8h, 032h, 013h, 000h, 000h, 083h
        db  0C4h, 008h, 02Bh, 0FEh, 083h, 0FFh, 001h, 075h, 004h, 066h, 0FFh, 04Dh, 006h, 05Dh, 05Fh, 05Eh
        db  05Bh, 0C3h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h
        db  000h, 08Bh, 044h, 024h, 00Ch, 06Ah, 000h, 050h, 08Bh, 044h, 024h, 010h, 050h, 08Bh, 044h, 024h
        db  010h, 050h, 0E8h, 00Ah, 000h, 000h, 000h, 083h, 0C4h, 010h, 0C3h, 08Dh, 09Bh, 000h, 000h, 000h
        db  000h, 083h, 0ECh, 03Ch, 053h, 056h, 057h, 055h, 08Bh, 04Ch, 024h, 050h, 0C7h, 044h, 024h, 034h
        db  001h, 000h, 000h, 000h, 0C7h, 001h, 000h, 000h, 000h, 000h, 0C7h, 044h, 024h, 014h, 000h, 000h
        db  000h, 000h, 0C7h, 044h, 024h, 02Ch, 000h, 000h, 000h, 000h, 0C7h, 044h, 024h, 028h, 000h, 000h
        db  000h, 000h, 0C7h, 044h, 024h, 020h, 000h, 000h, 000h, 000h, 0C7h, 044h, 024h, 024h, 000h, 000h
        db  000h, 000h, 06Ah, 001h, 068h, 000h, 001h, 000h, 000h, 0E8h, 0DEh, 016h, 000h, 000h, 089h, 044h
        db  024h, 038h, 083h, 0C4h, 008h, 00Bh, 0C0h, 075h, 00Eh, 0BEh, 0FEh, 0FFh, 0FFh, 0FFh, 08Bh, 06Ch
        db  024h, 03Ch, 0E9h, 0B6h, 006h, 000h, 000h, 06Ah, 001h, 068h, 000h, 001h, 000h, 000h, 0E8h, 0B9h
        db  016h, 000h, 000h, 089h, 044h, 024h, 030h, 083h, 0C4h, 008h, 00Bh, 0C0h, 075h, 013h, 0BEh, 0FEh
        db  0FFh, 0FFh, 0FFh, 08Bh, 06Ch, 024h, 03Ch, 0E9h, 091h, 006h, 000h, 000h, 005h, 000h, 000h, 000h
        db  000h, 06Ah, 001h, 068h, 000h, 002h, 000h, 000h, 0E8h, 08Fh, 016h, 000h, 000h, 089h, 044h, 024h
        db  02Ch, 083h, 0C4h, 008h, 00Bh, 0C0h, 075h, 00Eh, 0BEh, 0FEh, 0FFh, 0FFh, 0FFh, 08Bh, 06Ch, 024h
        db  03Ch, 0E9h, 067h, 006h, 000h, 000h, 06Ah, 001h, 068h, 000h, 002h, 000h, 000h, 0E8h, 06Ah, 016h
        db  000h, 000h, 089h, 044h, 024h, 028h, 083h, 0C4h, 008h, 00Bh, 0C0h, 075h, 014h, 0BEh, 0FEh, 0FFh
        db  0FFh, 0FFh, 08Bh, 06Ch, 024h, 03Ch, 0E9h, 042h, 006h, 000h, 000h, 08Dh, 09Bh, 000h, 000h, 000h
        db  000h, 083h, 07Ch, 024h, 058h, 000h, 075h, 008h, 0C7h, 044h, 024h, 058h, 0ECh, 000h, 000h, 000h
        db  08Dh, 044h, 024h, 040h, 08Bh, 05Ch, 024h, 054h, 0C7h, 000h, 000h, 000h, 000h, 000h, 0C7h, 040h
        db  004h, 000h, 000h, 000h, 000h, 066h, 0C7h, 040h, 008h, 000h, 000h, 00Bh, 0DBh, 075h, 00Dh, 0B8h
        db  0FDh, 0FFh, 0FFh, 0FFh, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 03Ch, 0C3h, 08Bh, 044h, 024h, 028h
        db  050h, 08Bh, 044h, 024h, 034h, 050h, 08Bh, 044h, 024h, 028h, 050h, 06Ah, 000h, 053h, 0E8h, 08Eh
        db  014h, 000h, 000h, 08Bh, 07Ch, 024h, 044h, 083h, 0C4h, 014h, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh
        db  0C0h, 0F2h, 0AEh, 0F7h, 0D1h, 02Bh, 0F9h, 08Bh, 0C1h, 08Bh, 0F7h, 08Bh, 07Ch, 024h, 024h, 0C1h
        db  0E9h, 002h, 0F3h, 0A5h, 08Bh, 0C8h, 083h, 0E1h, 003h, 0F3h, 0A4h, 08Bh, 044h, 024h, 028h, 080h
        db  038h, 000h, 074h, 02Fh, 08Bh, 0F8h, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0C0h, 0F2h, 0AEh, 0F7h
        db  0D1h, 02Bh, 0F9h, 08Bh, 0F7h, 08Bh, 0D1h, 08Bh, 07Ch, 024h, 024h, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0C0h, 0F2h, 0AEh, 04Fh, 08Bh, 0CAh, 0C1h, 0E9h, 002h, 0F3h, 0A5h, 08Bh, 0CAh, 083h, 0E1h
        db  003h, 0F3h, 0A4h, 08Bh, 044h, 024h, 024h, 050h, 0E8h, 064h, 006h, 000h, 000h, 083h, 0C4h, 004h
        db  00Bh, 0C0h, 074h, 01Dh, 08Bh, 04Ch, 024h, 050h, 0FFh, 080h, 000h, 002h, 000h, 000h, 089h, 001h
        db  02Bh, 0C0h, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 03Ch, 0C3h, 08Dh, 0A4h, 024h, 000h, 000h, 000h
        db  000h, 083h, 03Dh, 0E8h, 000h, 000h, 000h, 000h, 075h, 027h, 068h, 047h, 002h, 000h, 000h, 06Ah
        db  000h, 0E8h, 089h, 015h, 000h, 000h, 083h, 0C4h, 008h, 0A3h, 0E8h, 000h, 000h, 000h, 00Bh, 0C0h
        db  075h, 00Fh, 0B8h, 0FEh, 0FFh, 0FFh, 0FFh, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 03Ch, 0C3h, 08Bh
        db  0FFh, 0A1h, 0E8h, 000h, 000h, 000h, 050h, 0E8h, 03Ch, 015h, 000h, 000h, 083h, 0C4h, 004h, 08Bh
        db  0E8h, 00Bh, 0EDh, 075h, 00Dh, 0B8h, 0FEh, 0FFh, 0FFh, 0FFh, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h
        db  03Ch, 0C3h, 0C7h, 085h, 010h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 08Bh, 044h, 024h, 020h
        db  0C7h, 085h, 008h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 080h, 038h, 000h, 075h, 05Bh, 0BFh
        db  0F0h, 000h, 000h, 000h, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0C0h, 0F2h, 0AEh, 0F7h, 0D1h, 02Bh
        db  0F9h, 08Bh, 0C1h, 08Bh, 0F7h, 08Bh, 07Ch, 024h, 020h, 0C1h, 0E9h, 002h, 0F3h, 0A5h, 08Bh, 0C8h
        db  083h, 0E1h, 003h, 0F3h, 0A4h, 08Bh, 07Ch, 024h, 024h, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0C0h
        db  0F2h, 0AEh, 0F7h, 0D1h, 02Bh, 0F9h, 08Bh, 0F7h, 08Bh, 0D1h, 08Bh, 07Ch, 024h, 020h, 0B9h, 0FFh
        db  0FFh, 0FFh, 0FFh, 02Bh, 0C0h, 0F2h, 0AEh, 04Fh, 08Bh, 0CAh, 0C1h, 0E9h, 002h, 0F3h, 0A5h, 08Bh
        db  0CAh, 083h, 0E1h, 003h, 0F3h, 0A4h, 08Bh, 05Ch, 024h, 020h, 080h, 03Dh, 0E4h, 000h, 000h, 000h
        db  000h, 075h, 026h, 08Dh, 044h, 024h, 040h, 050h, 0E8h, 084h, 014h, 000h, 000h, 050h, 0E8h, 07Eh
        db  0FBh, 0FBh, 0FFh, 089h, 044h, 024h, 03Ch, 083h, 0C4h, 008h, 083h, 0F8h, 002h, 075h, 00Ah, 0BEh
        db  0FEh, 0FFh, 0FFh, 0FFh, 0E9h, 064h, 004h, 000h, 000h, 06Ah, 020h, 053h, 0E8h, 028h, 012h, 000h
        db  000h, 089h, 044h, 024h, 01Ch, 083h, 0C4h, 008h, 00Bh, 0C0h, 00Fh, 084h, 048h, 004h, 000h, 000h
        db  06Ah, 001h, 068h, 000h, 004h, 000h, 000h, 0E8h, 050h, 014h, 000h, 000h, 089h, 044h, 024h, 034h
        db  083h, 0C4h, 008h, 00Bh, 0C0h, 075h, 00Ah, 0BEh, 0FEh, 0FFh, 0FFh, 0FFh, 0E9h, 02Ch, 004h, 000h
        db  000h, 08Bh, 044h, 024h, 02Ch, 050h, 08Bh, 044h, 024h, 018h, 068h, 000h, 004h, 000h, 000h, 050h
        db  0E8h, 049h, 012h, 000h, 000h, 083h, 0C4h, 00Ch, 03Dh, 000h, 004h, 000h, 000h, 074h, 00Ah, 0BEh
        db  0FBh, 0FFh, 0FFh, 0FFh, 0E9h, 004h, 004h, 000h, 000h, 08Bh, 044h, 024h, 02Ch, 050h, 0E8h, 0CEh
        db  010h, 000h, 000h, 083h, 0C4h, 004h, 08Bh, 0F0h, 00Bh, 0F6h, 00Fh, 084h, 0E1h, 003h, 000h, 000h
        db  066h, 081h, 07Eh, 004h, 04Ch, 001h, 00Fh, 085h, 0D5h, 003h, 000h, 000h, 033h, 0C0h, 066h, 08Bh
        db  046h, 006h, 089h, 044h, 024h, 01Ch, 08Bh, 046h, 050h, 005h, 0FFh, 00Fh, 000h, 000h, 0C1h, 0E8h
        db  00Ch, 089h, 044h, 024h, 038h, 033h, 0C0h, 066h, 08Bh, 046h, 014h, 08Dh, 04Ch, 006h, 018h, 08Bh
        db  044h, 024h, 01Ch, 08Dh, 03Ch, 080h, 08Dh, 03Ch, 0F9h, 083h, 0EFh, 028h, 0BAh, 00Ch, 002h, 000h
        db  000h, 08Bh, 007h, 03Bh, 002h, 075h, 031h, 066h, 08Bh, 047h, 004h, 066h, 03Bh, 042h, 004h, 075h
        db  027h, 08Ah, 047h, 006h, 03Ah, 042h, 006h, 075h, 01Fh, 0FFh, 04Ch, 024h, 01Ch, 08Bh, 044h, 024h
        db  01Ch, 08Bh, 056h, 050h, 08Dh, 004h, 080h, 02Bh, 054h, 0C1h, 010h, 081h, 0C2h, 0FFh, 00Fh, 000h
        db  000h, 0C1h, 0EAh, 00Ch, 089h, 054h, 024h, 038h, 08Bh, 04Ch, 024h, 038h, 06Ah, 000h, 051h, 068h
        db  000h, 000h, 008h, 080h, 0E8h, 024h, 014h, 000h, 000h, 083h, 0C4h, 00Ch, 089h, 085h, 008h, 002h
        db  000h, 000h, 083h, 0F8h, 0FFh, 075h, 00Ah, 0BEh, 0FEh, 0FFh, 0FFh, 0FFh, 0E9h, 04Ch, 003h, 000h
        db  000h, 06Ah, 008h, 06Ah, 000h, 08Bh, 04Ch, 024h, 040h, 06Ah, 003h, 0C1h, 0E8h, 00Ch, 051h, 050h
        db  0E8h, 0E6h, 013h, 000h, 000h, 083h, 0C4h, 014h, 00Bh, 0C0h, 075h, 00Ah, 0BEh, 0FEh, 0FFh, 0FFh
        db  0FFh, 0E9h, 027h, 003h, 000h, 000h, 06Ah, 000h, 08Bh, 044h, 024h, 018h, 06Ah, 000h, 050h, 0E8h
        db  017h, 011h, 000h, 000h, 083h, 0C4h, 00Ch, 08Bh, 076h, 054h, 08Bh, 085h, 008h, 002h, 000h, 000h
        db  050h, 08Bh, 044h, 024h, 018h, 056h, 050h, 0E8h, 032h, 011h, 000h, 000h, 083h, 0C4h, 00Ch, 03Bh
        db  0C6h, 074h, 00Eh, 0BEh, 0FCh, 0FFh, 0FFh, 0FFh, 0E9h, 0F0h, 002h, 000h, 000h, 08Dh, 064h, 024h
        db  000h, 08Bh, 085h, 008h, 002h, 000h, 000h, 050h, 0E8h, 0B4h, 00Fh, 000h, 000h, 083h, 0C4h, 004h
        db  033h, 0C9h, 066h, 08Bh, 048h, 014h, 08Dh, 05Ch, 008h, 018h, 08Bh, 07Ch, 024h, 024h, 0B9h, 0FFh
        db  0FFh, 0FFh, 0FFh, 02Bh, 0C0h, 0F2h, 0AEh, 0F7h, 0D1h, 02Bh, 0F9h, 08Bh, 0C1h, 08Bh, 0F7h, 08Bh
        db  0FDh, 0C1h, 0E9h, 002h, 0F3h, 0A5h, 08Bh, 0C8h, 083h, 0E1h, 003h, 0F3h, 0A4h, 0C7h, 044h, 024h
        db  018h, 000h, 000h, 000h, 000h, 083h, 07Ch, 024h, 01Ch, 000h, 00Fh, 084h, 019h, 001h, 000h, 000h
        db  090h, 08Bh, 043h, 008h, 08Bh, 04Bh, 010h, 089h, 04Ch, 024h, 010h, 0A8h, 001h, 074h, 001h, 040h
        db  0F6h, 044h, 024h, 010h, 001h, 074h, 004h, 0FFh, 044h, 024h, 010h, 00Bh, 0C0h, 075h, 004h, 08Bh
        db  044h, 024h, 010h, 083h, 07Bh, 014h, 000h, 075h, 00Ah, 0C7h, 044h, 024h, 010h, 000h, 000h, 000h
        db  000h, 0EBh, 00Ah, 039h, 044h, 024h, 010h, 076h, 004h, 089h, 044h, 024h, 010h, 00Bh, 0C0h, 074h
        db  079h, 08Bh, 00Bh, 081h, 0F9h, 050h, 041h, 047h, 045h, 074h, 008h, 081h, 0F9h, 02Eh, 065h, 064h
        db  061h, 075h, 067h, 08Bh, 04Bh, 00Ch, 003h, 08Dh, 008h, 002h, 000h, 000h, 08Dh, 0B1h, 0FFh, 00Fh
        db  000h, 000h, 0C1h, 0EEh, 00Ch, 08Dh, 03Ch, 001h, 0C1h, 0EFh, 00Ch, 03Bh, 0FEh, 073h, 004h, 02Bh
        db  0FFh, 0EBh, 01Fh, 02Bh, 0FEh, 0C1h, 0EFh, 00Ch, 00Bh, 0FFh, 075h, 016h, 08Bh, 04Ch, 024h, 01Ch
        db  02Bh, 04Ch, 024h, 018h, 083h, 0F9h, 001h, 075h, 009h, 00Bh, 0C0h, 074h, 005h, 0BFh, 001h, 000h
        db  000h, 000h, 06Ah, 000h, 057h, 056h, 0E8h, 0B6h, 012h, 000h, 000h, 083h, 0C4h, 00Ch, 085h, 0C7h
        db  074h, 018h, 06Ah, 000h, 06Ah, 000h, 06Ah, 001h, 057h, 056h, 0E8h, 09Ch, 012h, 000h, 000h, 083h
        db  0C4h, 014h, 00Bh, 0C0h, 00Fh, 084h, 0C7h, 001h, 000h, 000h, 083h, 07Ch, 024h, 010h, 000h, 074h
        db  039h, 06Ah, 000h, 08Bh, 043h, 014h, 050h, 08Bh, 044h, 024h, 01Ch, 050h, 0E8h, 0CAh, 00Fh, 000h
        db  000h, 083h, 0C4h, 00Ch, 08Bh, 043h, 00Ch, 003h, 085h, 008h, 002h, 000h, 000h, 08Bh, 04Ch, 024h
        db  010h, 050h, 08Bh, 044h, 024h, 018h, 051h, 050h, 0E8h, 0E1h, 00Fh, 000h, 000h, 083h, 0C4h, 00Ch
        db  03Bh, 044h, 024h, 010h, 00Fh, 085h, 08Eh, 001h, 000h, 000h, 053h, 055h, 0E8h, 000h, 00Ch, 000h
        db  000h, 083h, 0C4h, 008h, 083h, 0C3h, 028h, 0FFh, 044h, 024h, 018h, 08Bh, 044h, 024h, 01Ch, 03Bh
        db  044h, 024h, 018h, 00Fh, 087h, 0E8h, 0FEh, 0FFh, 0FFh, 08Bh, 044h, 024h, 058h, 050h, 055h, 0E8h
        db  06Dh, 00Eh, 000h, 000h, 083h, 0C4h, 008h, 08Bh, 0F0h, 00Bh, 0F6h, 00Fh, 085h, 06Ch, 001h, 000h
        db  000h, 068h, 014h, 002h, 000h, 000h, 055h, 0E8h, 0F5h, 0F8h, 0FFh, 0FFh, 083h, 0C4h, 008h, 0F6h
        db  044h, 024h, 05Ch, 001h, 074h, 00Eh, 068h, 01Ch, 002h, 000h, 000h, 055h, 0E8h, 0E0h, 0F8h, 0FFh
        db  0FFh, 083h, 0C4h, 008h, 0C7h, 085h, 000h, 002h, 000h, 000h, 001h, 000h, 000h, 000h, 055h, 0A1h
        db  0E8h, 000h, 000h, 000h, 050h, 0E8h, 062h, 011h, 000h, 000h, 083h, 0C4h, 008h, 0C6h, 085h, 018h
        db  002h, 000h, 000h, 001h, 0C7h, 085h, 019h, 002h, 000h, 000h, 001h, 000h, 000h, 000h, 066h, 0C7h
        db  085h, 01Dh, 002h, 000h, 000h, 000h, 000h, 089h, 0ADh, 01Fh, 002h, 000h, 000h, 08Dh, 085h, 037h
        db  002h, 000h, 000h, 0C7h, 085h, 023h, 002h, 000h, 000h, 058h, 056h, 04Ch, 044h, 0C7h, 085h, 027h
        db  002h, 000h, 000h, 001h, 000h, 000h, 000h, 089h, 085h, 02Bh, 002h, 000h, 000h, 0C7h, 085h, 02Fh
        db  002h, 000h, 000h, 000h, 000h, 000h, 000h, 0C7h, 085h, 033h, 002h, 000h, 000h, 000h, 000h, 000h
        db  000h, 08Bh, 08Dh, 008h, 002h, 000h, 000h, 089h, 008h, 08Bh, 044h, 024h, 038h, 0C1h, 0E0h, 00Ch
        db  089h, 085h, 03Bh, 002h, 000h, 000h, 0C7h, 085h, 03Fh, 002h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 085h, 043h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 08Dh, 085h, 014h, 002h, 000h, 000h
        db  050h, 0E8h, 0A8h, 0F8h, 0FBh, 0FFh, 08Bh, 04Ch, 024h, 050h, 06Ah, 000h, 089h, 029h, 08Bh, 044h
        db  024h, 034h, 050h, 0E8h, 0AAh, 010h, 000h, 000h, 083h, 0C4h, 008h, 08Bh, 044h, 024h, 028h, 06Ah
        db  000h, 050h, 0E8h, 09Bh, 010h, 000h, 000h, 083h, 0C4h, 008h, 08Bh, 044h, 024h, 020h, 06Ah, 000h
        db  050h, 0E8h, 08Ch, 010h, 000h, 000h, 083h, 0C4h, 008h, 08Bh, 044h, 024h, 024h, 06Ah, 000h, 050h
        db  0E8h, 07Dh, 010h, 000h, 000h, 083h, 0C4h, 008h, 08Bh, 044h, 024h, 02Ch, 06Ah, 000h, 050h, 0E8h
        db  06Eh, 010h, 000h, 000h, 08Bh, 044h, 024h, 01Ch, 083h, 0C4h, 008h, 050h, 0E8h, 030h, 00Eh, 000h
        db  000h, 083h, 0C4h, 004h, 080h, 03Dh, 0E4h, 000h, 000h, 000h, 000h, 075h, 01Ah, 083h, 07Ch, 024h
        db  034h, 000h, 075h, 013h, 08Dh, 044h, 024h, 040h, 050h, 0E8h, 033h, 010h, 000h, 000h, 050h, 0E8h
        db  0ADh, 0F7h, 0FBh, 0FFh, 083h, 0C4h, 008h, 02Bh, 0C0h, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 03Ch
        db  0C3h, 0BEh, 0FEh, 0FFh, 0FFh, 0FFh, 0EBh, 015h, 0BEh, 0FCh, 0FFh, 0FFh, 0FFh, 0EBh, 00Eh, 08Bh
        db  0FFh, 0BEh, 0FAh, 0FFh, 0FFh, 0FFh, 0EBh, 005h, 0BEh, 0FDh, 0FFh, 0FFh, 0FFh, 083h, 07Ch, 024h
        db  024h, 000h, 074h, 00Fh, 08Bh, 044h, 024h, 024h, 06Ah, 000h, 050h, 0E8h, 002h, 010h, 000h, 000h
        db  083h, 0C4h, 008h, 083h, 07Ch, 024h, 020h, 000h, 074h, 00Fh, 08Bh, 044h, 024h, 020h, 06Ah, 000h
        db  050h, 0E8h, 0ECh, 00Fh, 000h, 000h, 083h, 0C4h, 008h, 083h, 07Ch, 024h, 028h, 000h, 074h, 00Fh
        db  08Bh, 044h, 024h, 028h, 06Ah, 000h, 050h, 0E8h, 0D6h, 00Fh, 000h, 000h, 083h, 0C4h, 008h, 083h
        db  07Ch, 024h, 030h, 000h, 074h, 00Fh, 08Bh, 044h, 024h, 030h, 06Ah, 000h, 050h, 0E8h, 0C0h, 00Fh
        db  000h, 000h, 083h, 0C4h, 008h, 083h, 07Ch, 024h, 014h, 000h, 074h, 00Dh, 08Bh, 044h, 024h, 014h
        db  050h, 0E8h, 07Bh, 00Dh, 000h, 000h, 083h, 0C4h, 004h, 080h, 03Dh, 0E4h, 000h, 000h, 000h, 000h
        db  075h, 01Ah, 083h, 07Ch, 024h, 034h, 000h, 075h, 013h, 08Dh, 044h, 024h, 040h, 050h, 0E8h, 07Eh
        db  00Fh, 000h, 000h, 050h, 0E8h, 0F8h, 0F6h, 0FBh, 0FFh, 083h, 0C4h, 008h, 083h, 07Ch, 024h, 02Ch
        db  000h, 074h, 00Fh, 08Bh, 044h, 024h, 02Ch, 06Ah, 000h, 050h, 0E8h, 073h, 00Fh, 000h, 000h, 083h
        db  0C4h, 008h, 00Bh, 0EDh, 074h, 024h, 08Bh, 085h, 008h, 002h, 000h, 000h, 00Bh, 0C0h, 074h, 00Bh
        db  06Ah, 000h, 050h, 0E8h, 0FFh, 00Fh, 000h, 000h, 083h, 0C4h, 008h, 055h, 0A1h, 0E8h, 000h, 000h
        db  000h, 050h, 0E8h, 093h, 00Fh, 000h, 000h, 083h, 0C4h, 008h, 08Bh, 0C6h, 05Dh, 05Fh, 05Eh, 05Bh
        db  083h, 0C4h, 03Ch, 0C3h, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h
        db  000h, 056h, 057h, 0A1h, 0E8h, 000h, 000h, 000h, 00Bh, 0C0h, 075h, 005h, 02Bh, 0C0h, 05Fh, 05Eh
        db  0C3h, 050h, 0E8h, 074h, 00Fh, 000h, 000h
loc_4_1AC6:
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        db  074h, 027h
        mov    esi, dword ptr [esp + 0ch]
        push   esi
        push   edi
        db  0E8h, 076h, 00Ch, 000h, 000h
        add    esp, 8
        or     eax, eax
        db  0A1h, 0E8h, 000h, 000h, 000h
        db  074h, 015h
        push   edi
        push   eax
_PTEXT ends
        end
