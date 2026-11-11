; x.asm -- splitter.vxd object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  055h
        db  08Bh, 0ECh
        db  08Bh, 045h, 00Ch
        db  050h
        db  0CDh, 020h, 00Dh, 000h, 040h, 000h
        db  059h
        db  085h, 0C0h
        db  074h, 003h
loc_1_0012:
        db  0FCh
        db  0C9h
        db  0C3h
loc_1_0015:
        db  08Bh, 00Dh, 0ECh, 005h, 000h, 000h
        db  066h, 083h, 039h, 000h
        db  075h, 0F1h
        db  0CDh, 020h, 011h, 000h, 040h, 000h
        db  0FFh, 075h, 00Ch
        db  0CDh, 020h, 00Dh, 000h, 040h, 000h
        db  059h
        db  0FCh
        db  0C9h
        db  0C3h
sub_1_0034:
        db  055h
        db  08Bh, 0ECh
        db  08Bh, 045h, 008h
        db  050h
        db  0CDh, 020h, 00Eh, 000h, 040h, 000h
        db  058h
        db  0FCh
        db  033h, 0C0h
        db  0C9h
        db  0C3h
sub_1_0047:
        db  055h
        db  08Bh, 0ECh
        db  056h
        db  057h
        db  0FCh
        db  08Bh, 07Dh, 00Ch
        db  08Bh, 075h, 008h
        db  08Bh, 04Dh, 010h
        db  0F3h, 0A6h
        db  00Fh, 0B6h, 046h, 0FFh
        db  00Fh, 0B6h, 057h, 0FFh
        db  02Bh, 0C2h
        db  05Fh
        db  05Eh
        db  0C9h
        db  0C3h
loc_1_0066:
        db  0CDh, 020h, 004h, 000h, 015h, 000h
        db  0A3h, 0ECh, 005h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 014h, 000h
        db  072h, 01Fh
        db  03Dh, 00Ah, 003h, 000h, 000h
        db  072h, 018h
        db  0B8h, 005h, 000h, 000h, 000h
        db  0B9h, 02Ch, 000h, 000h, 000h
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        db  072h, 006h
        db  089h, 035h, 0F0h, 005h, 000h, 000h
loc_1_0098:
        db  0C3h
Splitter_Control:
        db  083h, 0F8h, 001h
        db  074h, 0C8h
        db  083h, 0F8h, 01Bh
        db  074h, 0C3h
        db  0F8h
        db  0C3h
sub_1_00A5:
        db  056h
        db  057h
        db  08Bh, 035h, 0F0h, 005h, 000h, 000h
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        db  074h, 019h
loc_1_00B5:
        db  08Ah, 008h
        db  038h, 04Bh, 030h
        db  075h, 00Ah
        db  08Ah, 048h, 001h
        db  038h, 04Bh, 003h
        db  075h, 002h
        db  0EBh, 008h
loc_1_00C6:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        db  075h, 0E7h
loc_1_00CE:
        db  05Fh
        db  05Eh
        db  0C3h
sub_1_00D1:
        db  056h
        db  057h
        db  08Bh, 035h, 0F0h, 005h, 000h, 000h
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        db  074h, 02Dh
loc_1_00E1:
        db  08Ah, 008h
        db  038h, 04Bh, 030h
        db  075h, 01Eh
        db  050h
        db  053h
        db  06Ah, 010h
        db  08Dh, 04Bh, 01Ah
        db  051h
        db  08Dh, 048h, 002h
        db  051h
        db  0E8h, 04Eh, 0FFh, 0FFh, 0FFh
        db  083h, 0C4h, 00Ch
        db  08Bh, 0C8h
        db  05Bh
        db  058h
        db  00Bh, 0C9h
        db  075h, 002h
        db  0EBh, 008h
loc_1_0106:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        db  075h, 0D3h
loc_1_010E:
        db  05Fh
        db  05Eh
        db  0C3h
sub_1_0111:
        db  09Ch
        db  0FAh
        db  08Bh, 035h, 0F0h, 005h, 000h, 000h
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        db  09Dh
        db  056h
        db  08Bh, 070h, 024h
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
        db  08Bh, 070h, 01Ch
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
        db  05Eh
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        db  0C3h
Splitter_Service_00:
        db  0B8h, 001h, 000h, 000h, 000h
        db  0F8h
        db  0C3h
Splitter_Service_01:
        db  056h
        db  057h
        db  0E8h, 088h, 0FFh, 0FFh, 0FFh
        db  00Bh, 0C0h
        db  00Fh, 085h, 097h, 000h, 000h, 000h
        db  08Bh, 035h, 0F0h, 005h, 000h, 000h
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        db  072h, 06Dh
        db  08Ah, 04Bh, 003h
        db  088h, 048h, 001h
        db  08Ah, 04Bh, 030h
        db  088h, 008h
        db  02Bh, 0C9h
        db  089h, 048h, 028h
        db  089h, 048h, 020h
        db  089h, 048h, 018h
        db  0C7h, 040h, 014h, 001h, 000h, 000h, 000h
        db  080h, 07Bh, 001h, 00Dh
        db  075h, 004h
        db  083h, 048h, 014h, 002h
loc_1_0186:
        db  0FCh
        db  08Dh, 078h, 002h
        db  08Dh, 073h, 01Ah
        db  0B9h, 010h, 000h, 000h, 000h
        db  0F3h, 0A4h
        db  050h
        db  0B8h, 005h, 000h, 000h, 000h
        db  0B9h, 004h, 000h, 000h, 000h
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        db  058h
        db  072h, 030h
        db  089h, 070h, 024h
        db  050h
        db  0B8h, 005h, 000h, 000h, 000h
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        db  058h
        db  072h, 015h
        db  089h, 070h, 01Ch
        db  08Bh, 035h, 0F0h, 005h, 000h, 000h
        db  09Ch
        db  0FAh
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        db  09Dh
loc_1_01CC:
        db  05Fh
        db  05Eh
        db  0C3h
loc_1_01CF:
        db  08Bh, 070h, 024h
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_1_01D8:
        db  08Bh, 035h, 0F0h, 005h, 000h, 000h
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        db  033h, 0C0h
        db  0EBh, 0E4h
loc_1_01E8:
        db  083h, 048h, 014h, 001h
        db  0EBh, 0DEh
Splitter_Service_02:
        db  056h
        db  057h
        db  0E8h, 0DCh, 0FEh, 0FFh, 0FFh
        db  00Bh, 0C0h
        db  074h, 04Eh
        db  08Bh, 0F8h
        db  00Fh, 0BAh, 077h, 014h, 000h
        db  073h, 035h
        db  08Bh, 077h, 024h
loc_1_0205:
        db  09Ch
        db  0FAh
        db  0CDh, 020h, 0A5h, 000h, 001h, 000h
        db  09Dh
        db  00Bh, 0C0h
        db  074h, 025h
        db  0FFh, 04Fh, 028h
        db  060h
        db  08Bh, 018h
        db  0C6h, 043h, 001h, 017h
        db  0C6h, 043h, 031h, 017h
        db  083h, 07Bh, 02Ch, 000h
        db  074h, 008h
        db  0B8h, 017h, 000h, 000h, 000h
        db  0FFh, 053h, 02Ch
loc_1_022E:
        db  061h
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        db  0EBh, 0CEh
loc_1_0237:
        db  08Bh, 0C7h
        db  0F7h, 040h, 014h, 002h, 000h, 000h, 000h
        db  075h, 00Bh
        db  0E8h, 0CAh, 0FEh, 0FFh, 0FFh
loc_1_0247:
        db  0CDh, 020h, 002h, 000h, 014h, 000h
loc_1_024D:
        db  033h, 0C0h
        db  088h, 043h, 001h
        db  088h, 043h, 031h
        db  05Fh
        db  05Eh
        db  0C3h
Splitter_Service_03:
        db  056h
        db  0A3h, 0F4h, 005h, 000h, 000h
        db  00Bh, 0C0h
        db  074h, 01Ah
        db  0B8h, 002h, 000h, 014h, 000h
        db  0BEh, 09Fh, 002h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        db  072h, 021h
        db  089h, 035h, 0FCh, 005h, 000h, 000h
        db  0EBh, 019h
loc_1_027C:
        db  083h, 03Dh, 0FCh, 005h, 000h, 000h, 000h
        db  074h, 010h
        db  0B8h, 002h, 000h, 014h, 000h
        db  0BEh, 09Fh, 002h, 000h, 000h
        db  0CDh, 020h, 01Ch, 001h, 001h, 000h
loc_1_0295:
        db  05Eh
        db  0C3h
        db  0EBh, 006h, 0FFh, 025h, 000h, 006h, 000h, 000h
Splitter_Service_04:
        db  051h
        db  052h
        db  056h
        db  057h
        db  050h
        db  0FCh
        db  02Bh, 0C0h
        db  08Ah, 003h
        db  025h, 07Fh, 0FFh, 0FFh, 0FFh
        db  083h, 0F8h, 036h
        db  074h, 03Eh
        db  083h, 0F8h, 021h
        db  00Fh, 084h, 0B6h, 000h, 000h, 000h
        db  083h, 0F8h, 035h
        db  00Fh, 084h, 0A1h, 001h, 000h, 000h
        db  083h, 0F8h, 031h
        db  074h, 046h
loc_1_02CA:
        db  0F8h
        db  058h
loc_1_02CC:
        db  05Fh
        db  05Eh
        db  05Ah
        db  059h
        db  0FCh
        db  072h, 00Fh
        db  083h, 03Dh, 0FCh, 005h, 000h, 000h, 000h
        db  074h, 006h
        db  0FFh, 025h, 0FCh, 005h, 000h, 000h
loc_1_02E2:
        db  0C3h
loc_1_02E3:
        db  02Bh, 0C0h
        db  088h, 043h, 001h
        db  088h, 043h, 031h
        db  083h, 0C4h, 004h
        db  0F9h
        db  0EBh, 0DBh
loc_1_02F1:
        db  0A1h, 0F4h, 005h, 000h, 000h
        db  00Bh, 0C0h
        db  074h, 0D0h
        db  039h, 043h, 02Ch
        db  074h, 0CBh
        db  053h
        db  0E8h, 0CCh, 0FDh, 0FFh, 0FFh
        db  05Bh
        db  00Bh, 0C0h
        db  074h, 0C0h
        db  083h, 048h, 014h, 002h
        db  0EBh, 0BAh
loc_1_0310:
        db  053h
        db  0E8h, 0BBh, 0FDh, 0FFh, 0FFh
        db  05Bh
        db  00Bh, 0C0h
        db  074h, 0AFh
        db  08Bh, 0F8h
        db  00Fh, 0BAh, 077h, 014h, 001h
        db  073h, 035h
        db  08Bh, 077h, 01Ch
loc_1_0327:
        db  09Ch
        db  0FAh
        db  0CDh, 020h, 0A5h, 000h, 001h, 000h
        db  09Dh
        db  00Bh, 0C0h
        db  074h, 025h
        db  060h
        db  08Bh, 018h
        db  0C6h, 043h, 001h, 017h
        db  0C6h, 043h, 031h, 017h
        db  083h, 07Bh, 02Ch, 000h
        db  074h, 008h
        db  0B8h, 017h, 000h, 000h, 000h
        db  0FFh, 053h, 02Ch
loc_1_034D:
        db  061h
        db  0FFh, 04Fh, 020h
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        db  0EBh, 0CEh
loc_1_0359:
        db  08Bh, 0C7h
        db  0F7h, 040h, 014h, 001h, 000h, 000h, 000h
        db  00Fh, 085h, 07Bh, 0FFh, 0FFh, 0FFh
        db  0E8h, 0A4h, 0FDh, 0FFh, 0FFh
        db  0E9h, 058h, 0FFh, 0FFh, 0FFh
loc_1_0372:
        db  053h
        db  0E8h, 02Dh, 0FDh, 0FFh, 0FFh
        db  05Bh
        db  00Bh, 0C0h
        db  00Fh, 084h, 049h, 0FFh, 0FFh, 0FFh
        db  08Bh, 0F8h
        db  02Bh, 0D2h
        db  0A1h, 0F4h, 005h, 000h, 000h
        db  00Bh, 0C0h
        db  074h, 008h
        db  039h, 043h, 02Ch
        db  075h, 003h
        db  042h
        db  0EBh, 01Fh
loc_1_0396:
        db  0F7h, 047h, 014h, 001h, 000h, 000h, 000h
        db  075h, 016h
        db  083h, 07Fh, 018h, 000h
        db  00Fh, 085h, 021h, 0FFh, 0FFh, 0FFh
        db  08Bh, 0C7h
        db  0E8h, 061h, 0FDh, 0FFh, 0FFh
        db  0E9h, 015h, 0FFh, 0FFh, 0FFh
loc_1_03B5:
        db  08Bh, 074h, 0D7h, 01Ch
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        db  00Fh, 082h, 005h, 0FFh, 0FFh, 0FFh
        db  089h, 018h
        db  09Ch
        db  0FAh
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        db  0FFh, 044h, 0D7h, 020h
        db  08Bh, 04Fh, 018h
        db  03Bh, 04Ch, 0D7h, 020h
        db  072h, 014h
        db  09Dh
        db  0C6h, 043h, 001h, 0FFh
        db  0C6h, 043h, 031h, 0FFh
        db  02Bh, 0C0h
        db  083h, 0C4h, 004h
        db  0F9h
        db  0E9h, 0DCh, 0FEh, 0FFh, 0FFh
loc_1_03F0:
        db  0FFh, 047h, 018h
        db  09Dh
        db  08Bh, 0F0h
        db  052h
        db  053h
        db  0B9h, 040h, 000h, 000h, 000h
        db  081h, 0C1h, 0ADh, 005h, 000h, 000h
        db  051h
        db  06Ah, 000h
        db  0E8h, 0F5h, 0FBh, 0FFh, 0FFh
        db  083h, 0C4h, 008h
        db  05Bh
        db  05Ah
        db  00Bh, 0C0h
        db  075h, 01Ah
        db  08Bh, 0C6h
        db  08Bh, 074h, 0D7h, 01Ch
        db  09Ch
        db  0FAh
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        db  09Dh
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        db  0E9h, 09Ch, 0FEh, 0FFh, 0FFh
loc_1_042E:
        db  0BFh, 0E6h, 004h, 000h, 000h
        db  089h, 078h, 02Ch
        db  08Ah, 00Bh
        db  088h, 008h
        db  080h, 008h, 080h
        db  066h, 0C7h, 040h, 008h, 0ADh, 005h
        db  08Bh, 0F8h
        db  083h, 0C7h, 040h
        db  089h, 078h, 004h
        db  08Ah, 04Bh, 030h
        db  088h, 048h, 030h
        db  08Ah, 04Bh, 003h
        db  088h, 048h, 003h
        db  0C6h, 043h, 001h, 0FFh
        db  0C6h, 043h, 031h, 0FFh
        db  08Bh, 0D8h
        db  0E9h, 064h, 0FEh, 0FFh, 0FFh
loc_1_0466:
        db  02Bh, 0D2h
        db  0A1h, 0F4h, 005h, 000h, 000h
        db  00Bh, 0C0h
        db  074h, 006h
        db  039h, 043h, 02Ch
        db  075h, 001h
        db  042h
loc_1_0477:
        db  08Bh, 035h, 0F0h, 005h, 000h, 000h
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        db  00Fh, 084h, 041h, 0FEh, 0FFh, 0FFh
loc_1_0489:
        db  050h
        db  08Bh, 074h, 0D0h, 01Ch
        db  09Ch
        db  0FAh
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        db  074h, 039h
loc_1_0498:
        db  039h, 018h
        db  075h, 02Dh
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        db  09Dh
        db  060h
        db  08Bh, 018h
        db  0C6h, 043h, 001h, 00Bh
        db  0C6h, 043h, 031h, 00Bh
        db  083h, 07Bh, 02Ch, 000h
        db  074h, 008h
        db  0B8h, 00Bh, 000h, 000h, 000h
        db  0FFh, 053h, 02Ch
loc_1_04BC:
        db  061h
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        db  058h
        db  0E9h, 01Ah, 0FEh, 0FFh, 0FFh
loc_1_04C9:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        db  075h, 0C7h
loc_1_04D1:
        db  09Dh
        db  058h
        db  08Bh, 035h, 0F0h, 005h, 000h, 000h
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        db  075h, 0A8h
        db  0E9h, 0E4h, 0FDh, 0FFh, 0FFh
loc_1_04E6:
        db  060h
        db  0FCh
        db  053h
        db  0E8h, 0B7h, 0FBh, 0FFh, 0FFh
        db  05Bh
        db  00Bh, 0C0h
        db  00Fh, 084h, 082h, 000h, 000h, 000h
        db  08Bh, 0F8h
        db  0BAh, 001h, 000h, 000h, 000h
loc_1_04FE:
        db  08Bh, 074h, 0D7h, 01Ch
        db  0CDh, 020h, 0A5h, 000h, 001h, 000h
        db  074h, 06Ch
        db  0FFh, 04Ch, 0D7h, 020h
        db  050h
        db  052h
        db  056h
        db  057h
        db  08Bh, 000h
        db  02Bh, 0C9h
        db  066h, 08Bh, 048h, 008h
        db  066h, 03Bh, 04Bh, 008h
        db  07Dh, 012h
        db  080h, 07Bh, 001h, 000h
        db  075h, 007h
        db  0BAh, 006h, 000h, 000h, 000h
        db  0EBh, 010h
loc_1_052D:
        db  08Ah, 053h, 001h
        db  0EBh, 00Bh
loc_1_0532:
        db  08Ah, 053h, 001h
        db  066h, 08Bh, 04Bh, 008h
        db  066h, 089h, 048h, 008h
loc_1_053D:
        db  08Bh, 078h, 004h
        db  08Bh, 073h, 004h
        db  0FCh
        db  0F3h, 0A4h
        db  0B9h, 010h, 000h, 000h, 000h
        db  08Dh, 078h, 00Ah
        db  08Dh, 073h, 00Ah
        db  0FCh
        db  0F3h, 0A4h
        db  088h, 050h, 001h
        db  088h, 050h, 031h
        db  060h
        db  08Bh, 0D8h
        db  02Bh, 0C0h
        db  039h, 043h, 02Ch
        db  074h, 007h
        db  08Ah, 043h, 001h
        db  0FFh, 053h, 02Ch
        db  0FCh
loc_1_056B:
        db  061h
        db  05Fh
        db  05Eh
        db  05Ah
        db  058h
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
loc_1_0576:
        db  04Ah
        db  074h, 085h
loc_1_0579:
        db  0FFh, 04Fh, 018h
        db  053h
        db  0E8h, 0B2h, 0FAh, 0FFh, 0FFh
        db  083h, 0C4h, 004h
        db  061h
        db  0C3h
        db  000h, 03Bh, 001h, 000h, 000h, 042h, 001h, 000h, 000h, 0EEh, 001h, 000h, 000h, 058h, 002h, 000h
        db  000h, 09Fh, 002h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 098h, 004h, 003h, 00Ah, 000h, 000h, 053h, 070h, 06Ch, 069h
        db  074h, 074h, 065h, 072h, 000h, 000h, 000h, 080h, 099h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  088h, 005h, 000h, 000h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_05FC:
        db  000h, 000h
        db  000h, 000h
loc_1_0600:
        db  000h, 000h
        db  000h, 000h
_LTEXT ends
        end
