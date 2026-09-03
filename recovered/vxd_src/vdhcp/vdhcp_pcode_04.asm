; vdhcp_pcode_04.asm -- VDHCP object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  081h, 0ECh, 0C8h, 001h, 000h, 000h, 053h, 056h, 057h, 055h, 0C7h, 044h, 024h, 054h, 000h, 000h
        db  000h, 000h, 0C7h, 044h, 024h, 058h, 000h, 000h, 000h, 000h, 0C7h, 044h, 024h, 05Ch, 000h, 001h
        db  000h, 000h, 0C7h, 044h, 024h, 060h, 000h, 001h, 000h, 000h, 0C7h, 044h, 024h, 064h, 000h, 000h
        db  000h, 000h, 0C7h, 044h, 024h, 01Ch, 000h, 001h, 000h, 000h, 0C7h, 044h, 024h, 050h, 04Eh, 042h
        db  055h, 046h, 0C7h, 044h, 024h, 040h, 000h, 000h, 000h, 000h, 0C7h, 044h, 024h, 04Ch, 000h, 001h
        db  000h, 000h, 08Dh, 084h, 024h, 0D8h, 000h, 000h, 000h, 089h, 044h, 024h, 044h, 0C7h, 044h, 024h
        db  048h, 000h, 000h, 000h, 000h, 08Dh, 07Ch, 024h, 030h, 02Bh, 0C0h, 0ABh, 0ABh, 0ABh, 0ABh, 08Dh
        db  044h, 024h, 030h, 050h, 08Dh, 044h, 024h, 020h, 050h, 08Dh, 044h, 024h, 048h, 050h, 08Dh, 044h
        db  024h, 060h, 050h, 06Ah, 000h, 0A1h, 054h, 030h, 000h, 000h, 0FFh, 050h, 048h, 083h, 0C4h, 014h
        db  00Bh, 0C0h, 074h, 010h, 02Bh, 0C0h, 05Dh, 05Fh, 05Eh, 05Bh, 081h, 0C4h, 0C8h, 001h, 000h, 000h
        db  0C3h, 08Dh, 049h, 000h, 08Bh, 044h, 024h, 01Ch, 0C1h, 0E8h, 003h, 089h, 044h, 024h, 020h, 0C7h
        db  044h, 024h, 028h, 000h, 000h, 000h, 000h, 00Bh, 0C0h, 00Fh, 084h, 071h, 002h, 000h, 000h, 08Dh
        db  09Ch, 024h, 0D8h, 000h, 000h, 000h, 08Bh, 0B4h, 024h, 0DCh, 001h, 000h, 000h, 08Bh, 003h, 03Dh
        db  001h, 003h, 000h, 000h, 00Fh, 085h, 041h, 002h, 000h, 000h, 089h, 044h, 024h, 054h, 08Bh, 043h
        db  004h, 089h, 044h, 024h, 058h, 0C7h, 044h, 024h, 05Ch, 000h, 001h, 000h, 000h, 0C7h, 044h, 024h
        db  060h, 000h, 001h, 000h, 000h, 0C7h, 044h, 024h, 064h, 001h, 000h, 000h, 000h, 0C7h, 044h, 024h
        db  01Ch, 004h, 000h, 000h, 000h, 0C7h, 044h, 024h, 050h, 04Eh, 042h, 055h, 046h, 0C7h, 044h, 024h
        db  040h, 000h, 000h, 000h, 000h, 0C7h, 044h, 024h, 04Ch, 004h, 000h, 000h, 000h, 08Dh, 044h, 024h
        db  02Ch, 089h, 044h, 024h, 044h, 0C7h, 044h, 024h, 048h, 000h, 000h, 000h, 000h, 08Dh, 07Ch, 024h
        db  030h, 02Bh, 0C0h, 0ABh, 0ABh, 0ABh, 0ABh, 08Dh, 044h, 024h, 030h, 050h, 08Dh, 044h, 024h, 020h
        db  050h, 08Dh, 044h, 024h, 048h, 050h, 08Dh, 044h, 024h, 060h, 050h, 06Ah, 000h, 0A1h, 054h, 030h
        db  000h, 000h, 0FFh, 050h, 048h, 083h, 0C4h, 014h, 00Bh, 0C0h, 00Fh, 085h, 0BBh, 001h, 000h, 000h
        db  081h, 07Ch, 024h, 02Ch, 003h, 003h, 000h, 000h, 00Fh, 085h, 0ADh, 001h, 000h, 000h, 0C7h, 044h
        db  024h, 05Ch, 000h, 002h, 000h, 000h, 0C7h, 044h, 024h, 064h, 001h, 000h, 000h, 000h, 0C7h, 044h
        db  024h, 01Ch, 05Ch, 000h, 000h, 000h, 0C7h, 044h, 024h, 050h, 04Eh, 042h, 055h, 046h, 0C7h, 044h
        db  024h, 040h, 000h, 000h, 000h, 000h, 0C7h, 044h, 024h, 04Ch, 05Ch, 000h, 000h, 000h, 08Dh, 044h
        db  024h, 07Ch, 089h, 044h, 024h, 044h, 0C7h, 044h, 024h, 048h, 000h, 000h, 000h, 000h, 08Dh, 07Ch
        db  024h, 030h, 02Bh, 0C0h, 0ABh, 0ABh, 0ABh, 0ABh, 08Dh, 044h, 024h, 030h, 050h, 08Dh, 044h, 024h
        db  020h, 050h, 08Dh, 044h, 024h, 048h, 050h, 08Dh, 044h, 024h, 060h, 050h, 06Ah, 000h, 0A1h, 054h
        db  030h, 000h, 000h, 0FFh, 050h, 048h, 083h, 0C4h, 014h, 00Bh, 0C0h, 00Fh, 085h, 03Ah, 001h, 000h
        db  000h, 083h, 0BCh, 024h, 0D0h, 000h, 000h, 000h, 001h, 00Fh, 082h, 02Ch, 001h, 000h, 000h, 08Bh
        db  084h, 024h, 0D0h, 000h, 000h, 000h, 08Dh, 004h, 040h, 0C1h, 0E0h, 003h, 089h, 044h, 024h, 01Ch
        db  050h, 0E8h, 0CAh, 004h, 000h, 000h, 083h, 0C4h, 004h, 089h, 044h, 024h, 024h, 00Bh, 0C0h, 08Bh
        db  00Dh, 054h, 030h, 000h, 000h, 00Fh, 084h, 025h, 001h, 000h, 000h, 0C7h, 044h, 024h, 064h, 002h
        db  001h, 000h, 000h, 0C7h, 044h, 024h, 050h, 04Eh, 042h, 055h, 046h, 0C7h, 044h, 024h, 040h, 000h
        db  000h, 000h, 000h, 08Bh, 044h, 024h, 01Ch, 089h, 044h, 024h, 04Ch, 08Bh, 044h, 024h, 024h, 089h
        db  044h, 024h, 044h, 0C7h, 044h, 024h, 048h, 000h, 000h, 000h, 000h, 08Dh, 07Ch, 024h, 030h, 02Bh
        db  0C0h, 0ABh, 0ABh, 0ABh, 0ABh, 08Dh, 044h, 024h, 030h, 050h, 08Dh, 044h, 024h, 020h, 050h, 08Dh
        db  044h, 024h, 048h, 050h, 08Dh, 044h, 024h, 060h, 050h, 06Ah, 000h, 0FFh, 051h, 048h, 083h, 0C4h
        db  014h, 00Bh, 0C0h, 074h, 007h, 0E9h, 095h, 000h, 000h, 000h, 08Bh, 0FFh, 02Bh, 0FFh, 039h, 0BCh
        db  024h, 0D0h, 000h, 000h, 000h, 00Fh, 086h, 084h, 000h, 000h, 000h, 08Bh, 06Ch, 024h, 024h, 083h
        db  0C5h, 014h, 00Bh, 0F6h, 074h, 00Eh, 066h, 08Bh, 084h, 024h, 0E0h, 001h, 000h, 000h, 066h, 039h
        db  045h, 000h, 074h, 006h, 083h, 07Dh, 0ECh, 000h, 075h, 058h, 08Dh, 044h, 024h, 014h, 050h, 08Dh
        db  044h, 024h, 01Ch, 050h, 08Dh, 044h, 024h, 01Bh, 050h, 08Dh, 044h, 024h, 074h, 050h, 08Dh, 045h
        db  0ECh, 050h, 0FFh, 074h, 024h, 034h, 08Dh, 084h, 024h, 0F0h, 000h, 000h, 000h, 050h, 0E8h, 0ADh
        db  000h, 000h, 000h, 083h, 0C4h, 01Ch, 00Bh, 0C0h, 075h, 076h, 083h, 07Ch, 024h, 018h, 000h, 074h
        db  021h, 0FFh, 074h, 024h, 013h, 08Dh, 044h, 024h, 06Ch, 050h, 0FFh, 073h, 004h, 0FFh, 074h, 024h
        db  020h, 066h, 08Bh, 045h, 000h, 050h, 0E8h, 065h, 006h, 000h, 000h, 083h, 0C4h, 014h, 00Bh, 0F6h
        db  075h, 06Ah, 047h, 083h, 0C5h, 018h, 039h, 0BCh, 024h, 0D0h, 000h, 000h, 000h, 077h, 083h, 0FFh
        db  074h, 024h, 024h, 0E8h, 030h, 0FDh, 0FBh, 0FFh, 083h, 0C4h, 004h, 083h, 0C3h, 008h, 0FFh, 044h
        db  024h, 028h, 08Bh, 044h, 024h, 020h, 039h, 044h, 024h, 028h, 00Fh, 082h, 09Dh, 0FDh, 0FFh, 0FFh
        db  05Dh, 0B8h, 001h, 000h, 000h, 000h, 05Fh, 05Eh, 05Bh, 081h, 0C4h, 0C8h, 001h, 000h, 000h, 0C3h
        db  02Bh, 0C0h, 05Dh, 05Fh, 05Eh, 05Bh, 081h, 0C4h, 0C8h, 001h, 000h, 000h, 0C3h, 08Dh, 049h, 000h
        db  0FFh, 074h, 024h, 024h, 0E8h, 0EFh, 0FCh, 0FBh, 0FFh, 083h, 0C4h, 004h, 02Bh, 0C0h, 05Dh, 05Fh
        db  05Eh, 05Bh, 081h, 0C4h, 0C8h, 001h, 000h, 000h, 0C3h, 08Dh, 049h, 000h, 05Dh, 083h, 0F8h, 001h
        db  01Bh, 0C0h, 0F7h, 0D8h, 05Fh, 05Eh, 05Bh, 081h, 0C4h, 0C8h, 001h, 000h, 000h, 0C3h, 08Bh, 0FFh
        db  081h, 0ECh, 0A0h, 000h, 000h, 000h, 053h, 056h, 057h, 055h, 08Bh, 084h, 024h, 0C8h, 000h, 000h
        db  000h, 0C7h, 000h, 000h, 000h, 000h, 000h, 0C7h, 044h, 024h, 03Ch, 002h, 002h, 000h, 000h, 0C7h
        db  044h, 024h, 048h, 000h, 001h, 000h, 000h, 02Bh, 0EDh, 039h, 0ACh, 024h, 0B8h, 000h, 000h, 000h
        db  00Fh, 084h, 02Dh, 001h, 000h, 000h, 08Bh, 09Ch, 024h, 0B4h, 000h, 000h, 000h, 08Bh, 0B4h, 024h
        db  0BCh, 000h, 000h, 000h, 08Bh, 003h, 03Dh, 000h, 002h, 000h, 000h, 00Fh, 085h, 001h, 001h, 000h
        db  000h, 089h, 044h, 024h, 03Ch, 08Bh, 043h, 004h, 089h, 044h, 024h, 040h, 0C7h, 044h, 024h, 044h
        db  000h, 001h, 000h, 000h, 0C7h, 044h, 024h, 04Ch, 001h, 000h, 000h, 000h, 0C7h, 044h, 024h, 010h
        db  004h, 000h, 000h, 000h, 0C7h, 044h, 024h, 038h, 04Eh, 042h, 055h, 046h, 0C7h, 044h, 024h, 028h
        db  000h, 000h, 000h, 000h, 0C7h, 044h, 024h, 034h, 004h, 000h, 000h, 000h, 08Dh, 044h, 024h, 014h
        db  089h, 044h, 024h, 02Ch, 0C7h, 044h, 024h, 030h, 000h, 000h, 000h, 000h, 08Dh, 07Ch, 024h, 018h
        db  02Bh, 0C0h, 0ABh, 0ABh, 0ABh, 0ABh, 08Dh, 044h, 024h, 018h, 050h, 08Dh, 044h, 024h, 014h, 050h
        db  08Dh, 044h, 024h, 030h, 050h, 08Dh, 044h, 024h, 048h, 050h, 06Ah, 000h, 0A1h, 054h, 030h, 000h
        db  000h, 0FFh, 050h, 048h, 083h, 0C4h, 014h, 00Bh, 0C0h, 00Fh, 085h, 096h, 000h, 000h, 000h, 081h
        db  07Ch, 024h, 014h, 002h, 002h, 000h, 000h, 075h, 079h, 0C7h, 044h, 024h, 044h, 000h, 002h, 000h
        db  000h, 0C7h, 044h, 024h, 04Ch, 001h, 000h, 000h, 000h, 0C7h, 044h, 024h, 010h, 05Dh, 000h, 000h
        db  000h, 08Dh, 07Ch, 024h, 018h, 02Bh, 0C0h, 0ABh, 0ABh, 0ABh, 0ABh, 0C7h, 044h, 024h, 038h, 04Eh
        db  042h, 055h, 046h, 089h, 044h, 024h, 028h, 0C7h, 044h, 024h, 034h, 05Dh, 000h, 000h, 000h, 08Dh
        db  044h, 024h, 050h, 089h, 044h, 024h, 02Ch, 0C7h, 044h, 024h, 030h, 000h, 000h, 000h, 000h, 08Dh
        db  044h, 024h, 018h, 050h, 08Dh, 044h, 024h, 014h, 050h, 08Dh, 044h, 024h, 030h, 050h, 08Dh, 044h
        db  024h, 048h, 050h, 06Ah, 000h, 0A1h, 054h, 030h, 000h, 000h, 0FFh, 050h, 048h, 083h, 0C4h, 014h
        db  00Bh, 0C0h, 074h, 005h, 083h, 0F8h, 009h, 075h, 01Ch, 08Bh, 044h, 024h, 050h, 039h, 046h, 004h
        db  074h, 01Eh, 045h, 083h, 0C3h, 008h, 03Bh, 0ACh, 024h, 0B8h, 000h, 000h, 000h, 00Fh, 082h, 0E1h
        db  0FEh, 0FFh, 0FFh, 02Bh, 0C0h, 05Dh, 05Fh, 05Eh, 05Bh, 081h, 0C4h, 0A0h, 000h, 000h, 000h, 0C3h
        db  08Bh, 054h, 024h, 060h, 083h, 0EAh, 010h, 01Bh, 0C0h, 023h, 0D0h, 083h, 0C2h, 010h, 08Bh, 0BCh
        db  024h, 0C0h, 000h, 000h, 000h, 089h, 017h, 08Dh, 074h, 024h, 064h, 083h, 0C7h, 004h, 08Bh, 0CAh
        db  0C1h, 0E9h, 002h, 0F3h, 0A5h, 08Bh, 0CAh, 083h, 0E1h, 003h, 0F3h, 0A4h, 08Bh, 044h, 024h, 054h
        db  083h, 0F8h, 001h, 074h, 01Bh, 083h, 0F8h, 006h, 074h, 022h, 083h, 0F8h, 009h, 074h, 029h, 083h
        db  0F8h, 00Fh, 074h, 024h, 08Bh, 084h, 024h, 0C4h, 000h, 000h, 000h, 0C6h, 000h, 007h, 0EBh, 022h
        db  08Bh, 084h, 024h, 0C4h, 000h, 000h, 000h, 0C6h, 000h, 007h, 0EBh, 016h, 08Bh, 084h, 024h, 0C4h
        db  000h, 000h, 000h, 0C6h, 000h, 001h, 0EBh, 00Ah, 08Bh, 084h, 024h, 0C4h, 000h, 000h, 000h, 0C6h
        db  000h, 006h, 08Bh, 04Ch, 024h, 050h, 08Bh, 084h, 024h, 0CCh, 000h, 000h, 000h, 089h, 008h, 08Bh
        db  084h, 024h, 0C8h, 000h, 000h, 000h, 0C7h, 000h, 001h, 000h, 000h, 000h, 02Bh, 0C0h, 05Dh, 05Fh
        db  05Eh, 05Bh, 081h, 0C4h, 0A0h, 000h, 000h, 000h, 0C3h, 08Dh, 049h, 000h
sub_4_058C:
        db  083h, 03Dh, 0CCh, 028h, 000h, 000h, 000h
        je     SHORT loc_4_05A8
        push   dword ptr [esp + 8]
        push   dword ptr [esp + 8]
        call   sub_4_05E8
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_4_05A8:
        db  083h, 03Dh, 06Ch, 02Bh, 000h, 000h, 000h
        jne    SHORT loc_4_05CC
        db  0C7h, 005h, 06Ch, 02Bh, 000h, 000h, 001h, 000h, 000h, 000h
        push   dword ptr [esp + 8]
        db  068h, 0D0h, 005h, 000h, 000h
        db  0E8h, 057h, 028h, 0FCh, 0FFh
        add    esp, 8
loc_4_05CC:
        ret
        db  08Dh, 049h, 000h
loc_4_05D0:
        db  083h, 03Dh, 054h, 030h, 000h, 000h, 000h
        je     SHORT loc_4_05E7
        push   dword ptr [esp + 4]
        push   0
        call   sub_4_05E8
        add    esp, 8
loc_4_05E7:
        ret
sub_4_05E8:
        sub    esp, 4
        push   esi
        mov    dword ptr [esp + 4], 0ffffffffh
        push   0
        call   sub_4_0BE4
        add    esp, 4
        mov    esi, eax
        db  08Bh, 00Dh, 060h, 02Bh, 000h, 000h
        db  081h, 0F9h, 060h, 02Bh, 000h, 000h
        je     SHORT loc_4_064E
loc_4_060E:
        lea    edx, [ecx - 39h]
        mov    eax, dword ptr [edx + 41h]
        cmp    esi, eax
        jl     SHORT loc_4_0638
        mov    ecx, dword ptr [edx + 39h]
        mov    eax, dword ptr [edx + 3dh]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        lea    eax, [esp + 4]
        push   eax
        push   edx
        call   dword ptr [edx + 49h]
        add    esp, 8
        db  08Bh, 00Dh, 060h, 02Bh, 000h, 000h
        jmp    SHORT loc_4_0644
        db  090h
loc_4_0638:
        sub    eax, esi
        cmp    dword ptr [esp + 4], eax
        jbe    SHORT loc_4_0644
        mov    dword ptr [esp + 4], eax
loc_4_0644:
        mov    ecx, dword ptr [ecx]
        db  081h, 0F9h, 060h, 02Bh, 000h, 000h
        jne    SHORT loc_4_060E
loc_4_064E:
        db  068h, 034h, 030h, 000h, 000h
        cmp    dword ptr [esp + 8], -1
        jne    SHORT loc_4_0664
        db  0E8h, 0ADh, 0F9h, 0FBh, 0FFh
        add    esp, 4
        jmp    SHORT loc_4_06C1
loc_4_0664:
        db  0E8h, 0A3h, 0F9h, 0FBh, 0FFh
        add    esp, 4
        db  0C7h, 005h, 040h, 030h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 048h, 030h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 0C8h, 028h, 000h, 000h
        db  0A3h, 044h, 030h, 000h, 000h
        cmp    dword ptr [esp + 4], 15180h
        jbe    SHORT loc_4_069C
        mov    dword ptr [esp + 4], 15180h
loc_4_069C:
        push   0
        db  068h, 08Ch, 005h, 000h, 000h
        mov    eax, dword ptr [esp + 0ch]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        shl    eax, 3
        push   eax
        db  068h, 034h, 030h, 000h, 000h
        db  0E8h, 042h, 0F9h, 0FBh, 0FFh
        add    esp, 10h
loc_4_06C1:
        pop    esi
        db  0C7h, 005h, 06Ch, 02Bh, 000h, 000h, 000h, 000h, 000h, 000h
        add    esp, 4
        ret
sub_4_06D0:
        push   esi
        movzx  esi, word ptr [esp + 8]
        push   esi
        db  0E8h, 060h, 0F9h, 0FBh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_06FA
        db  083h, 03Dh, 0CCh, 028h, 000h, 000h, 000h
        je     SHORT loc_4_06FA
        db  0E8h, 063h, 0F9h, 0FBh, 0FFh
        push   esi
        db  0E8h, 045h, 0F9h, 0FBh, 0FFh
        add    esp, 4
loc_4_06FA:
        pop    esi
        ret
loc_4_06FC:
        push   dword ptr [esp + 4]
        push   1
        call   0
        add    esp, 8
        ret
        db  090h
loc_4_070C:
        push   esi
        mov    eax, dword ptr [esp + 8]
        or     eax, eax
        je     NEAR PTR loc_4_07B0
        db  0BAh, 0D0h, 028h, 000h, 000h
        mov    ecx, dword ptr [eax]
        cmp    ecx, dword ptr [edx]
        jne    NEAR PTR loc_4_07B0
        mov    ax, word ptr [eax + 4]
        cmp    ax, word ptr [edx + 4]
        jne    SHORT loc_4_07B0
        db  068h, 034h, 030h, 000h, 000h
        db  0E8h, 0D0h, 0F8h, 0FBh, 0FFh
        add    esp, 4
        db  0E8h, 06Ch, 010h, 0FCh, 0FFh
        db  0C7h, 005h, 054h, 030h, 000h, 000h, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 084h, 02Bh, 000h, 000h
        db  081h, 0FAh, 084h, 02Bh, 000h, 000h
        je     SHORT loc_4_078E
loc_4_075C:
        mov    esi, dword ptr [edx]
        mov    ecx, esi
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        db  0A1h, 04Ch, 030h, 000h, 000h
        mov    dword ptr [edx], eax
        db  0C7h, 042h, 004h, 04Ch, 030h, 000h, 000h
        db  0A1h, 04Ch, 030h, 000h, 000h
        mov    dword ptr [eax + 4], edx
        db  089h, 015h, 04Ch, 030h, 000h, 000h
        mov    edx, esi
        db  081h, 0FEh, 084h, 02Bh, 000h, 000h
        jne    SHORT loc_4_075C
loc_4_078E:
        db  0A1h, 060h, 02Bh, 000h, 000h
        db  03Dh, 060h, 02Bh, 000h, 000h
        je     SHORT loc_4_07B0
loc_4_079A:
        mov    edx, dword ptr [eax]
        mov    ecx, edx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], edx
        mov    dword ptr [edx + 4], eax
        mov    eax, ecx
        db  081h, 0F9h, 060h, 02Bh, 000h, 000h
        jne    SHORT loc_4_079A
loc_4_07B0:
        pop    esi
        ret
        db  08Bh, 0FFh
loc_4_07B4:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        je     SHORT loc_4_07D6
        db  0BAh, 0D8h, 028h, 000h, 000h
        mov    ecx, dword ptr [eax]
        cmp    ecx, dword ptr [edx]
        jne    SHORT loc_4_07D6
        mov    ax, word ptr [eax + 4]
        cmp    ax, word ptr [edx + 4]
        jne    SHORT loc_4_07D6
        db  0E8h, 08Ah, 01Ah, 0FCh, 0FFh
loc_4_07D6:
        ret
        db  090h
loc_4_07D8:
        push   esi
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_4_07E4
        sub    eax, eax
        pop    esi
        ret
loc_4_07E4:
        db  0A1h, 060h, 02Bh, 000h, 000h
        db  03Dh, 060h, 02Bh, 000h, 000h
        mov    si, word ptr [esp + 8]
        je     SHORT loc_4_0817
loc_4_07F5:
        lea    edx, [eax - 39h]
        mov    ecx, dword ptr [edx + 51h]
        cmp    word ptr [ecx + 4], si
        je     SHORT loc_4_080C
        mov    eax, dword ptr [eax]
        db  03Dh, 060h, 02Bh, 000h, 000h
        jne    SHORT loc_4_07F5
        jmp    SHORT loc_4_0817
loc_4_080C:
        mov    ecx, dword ptr [edx + 39h]
        mov    eax, dword ptr [edx + 3dh]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
loc_4_0817:
        db  08Bh, 015h, 084h, 02Bh, 000h, 000h
        db  081h, 0FAh, 084h, 02Bh, 000h, 000h
        je     SHORT loc_4_0861
loc_4_0825:
        mov    eax, dword ptr [edx + 51h]
        cmp    word ptr [eax + 4], si
        je     SHORT loc_4_083C
        mov    edx, dword ptr [edx]
        db  081h, 0FAh, 084h, 02Bh, 000h, 000h
        jne    SHORT loc_4_0825
        sub    eax, eax
        pop    esi
        ret
loc_4_083C:
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        db  0C7h, 002h, 04Ch, 030h, 000h, 000h
        db  0A1h, 050h, 030h, 000h, 000h
        mov    dword ptr [edx + 4], eax
        db  0A1h, 050h, 030h, 000h, 000h
        mov    dword ptr [eax], edx
        db  089h, 015h, 050h, 030h, 000h, 000h
loc_4_0861:
        sub    eax, eax
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_4_0868:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 2ch]
        lea    esi, [ebx + 2c7h]
        push   esi
        call   sub_4_0CD0
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        jne    SHORT loc_4_0890
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_4_0890:
        mov    edi, edx
        sub    eax, eax
        mov    ecx, esi
        shr    ecx, 2
        rep stosd
        mov    ecx, esi
        and    ecx, 3
        rep stosb
        lea    edi, [edx + 55h]
        mov    dword ptr [edx + 51h], edi
        add    edi, 32h
        mov    dword ptr [edx + 4dh], edi
        add    edi, 240h
        mov    dword ptr [edx + 9], edi
        mov    al, byte ptr [esp + 24h]
        mov    byte ptr [edx + 8], al
        mov    dword ptr [edx + 0dh], ebx
        mov    esi, dword ptr [esp + 28h]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [edx + 11h], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [edx + 15h], eax
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [edx + 19h], eax
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [edx + 1dh], eax
        mov    eax, dword ptr [esp + 30h]
        mov    dword ptr [edx + 21h], eax
        mov    eax, dword ptr [esp + 34h]
        mov    dword ptr [edx + 25h], eax
        mov    eax, dword ptr [esp + 38h]
        mov    dword ptr [edx + 29h], eax
        mov    eax, dword ptr [esp + 3ch]
        mov    dword ptr [edx + 2dh], eax
        mov    eax, dword ptr [esp + 40h]
        mov    dword ptr [edx + 31h], eax
        mov    dword ptr [edx + 35h], 0
        mov    ecx, dword ptr [edx + 51h]
        mov    dword ptr [ecx], 0ffffffffh
        mov    ax, word ptr [esp + 44h]
        mov    word ptr [ecx + 4], ax
        mov    eax, dword ptr [esp + 48h]
        mov    dword ptr [ecx + 6], eax
        mov    eax, dword ptr [esp + 4ch]
        mov    dword ptr [ecx + 0ah], eax
        mov    dword ptr [ecx + 12h], 0
        lea    eax, [ecx + 2ah]
        mov    dword ptr [ecx + 2eh], eax
        mov    dword ptr [eax], eax
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx], ecx
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 4], edx
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h, 083h, 0ECh, 00Ch, 053h, 056h, 057h, 055h, 08Bh, 07Ch, 024h, 02Ch, 057h, 068h, 04Ch, 030h
        db  000h, 000h, 0E8h, 052h, 0FDh, 0FBh, 0FFh, 083h, 0C4h, 008h, 08Bh, 0F0h, 00Bh, 0F6h, 074h, 059h
        db  08Bh, 00Eh, 08Bh, 046h, 004h, 089h, 008h, 089h, 041h, 004h, 0C7h, 006h, 084h, 02Bh, 000h, 000h
        db  0A1h, 088h, 02Bh, 000h, 000h, 089h, 046h, 004h, 0A1h, 088h, 02Bh, 000h, 000h, 089h, 030h, 089h
        db  035h, 088h, 02Bh, 000h, 000h, 0C7h, 046h, 035h, 000h, 000h, 000h, 000h, 08Bh, 04Eh, 051h, 0C7h
        db  001h, 0FFh, 0FFh, 0FFh, 0FFh, 066h, 08Bh, 044h, 024h, 020h, 066h, 089h, 041h, 004h, 08Bh, 044h
        db  024h, 024h, 089h, 041h, 006h, 08Bh, 044h, 024h, 028h, 089h, 041h, 00Ah, 0C7h, 041h, 012h, 000h
        db  000h, 000h, 000h, 08Bh, 05Ch, 024h, 010h, 0EBh, 05Bh, 0FFh, 074h, 024h, 028h, 0FFh, 074h, 024h
        db  028h, 0FFh, 074h, 024h, 028h, 06Ah, 000h, 06Ah, 000h, 06Ah, 000h, 06Ah, 000h, 06Ah, 000h, 0FFh
        db  037h, 08Dh, 047h, 004h, 050h, 0FFh, 074h, 024h, 058h, 06Ah, 000h, 06Ah, 0FFh, 06Ah, 000h, 06Ah
        db  000h, 068h, 084h, 02Bh, 000h, 000h, 0E8h, 05Eh, 0FEh, 0FFh, 0FFh, 083h, 0C4h, 040h, 08Bh, 0D8h
        db  00Bh, 0DBh, 074h, 00Dh, 08Bh, 0C3h, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 00Ch, 0C3h, 08Dh, 049h
        db  000h, 08Bh, 035h, 088h, 02Bh, 000h, 000h, 08Bh, 0C6h, 08Bh, 068h, 051h, 0E8h, 018h, 0FDh, 0FBh
        db  0FFh, 089h, 045h, 00Eh, 083h, 03Fh, 006h, 075h, 024h, 08Bh, 047h, 004h, 03Bh, 005h, 038h, 029h
        db  000h, 000h, 075h, 019h, 0C7h, 046h, 011h, 0FFh, 0FFh, 0FFh, 0FFh, 0C7h, 046h, 021h, 0FFh, 0FFh
        db  0FFh, 0FFh, 08Bh, 0C3h, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 00Ch, 0C3h, 090h, 083h, 07Eh, 011h
        db  000h, 075h, 01Ah, 08Dh, 044h, 024h, 014h, 050h, 056h, 0E8h, 003h, 039h, 0FCh, 0FFh, 083h, 0C4h
        db  008h, 08Bh, 0D8h, 08Bh, 0C3h, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 00Ch, 0C3h, 08Dh, 044h, 024h
        db  018h, 050h, 056h, 0E8h, 0C1h, 03Ah, 0FCh, 0FFh, 083h, 0C4h, 008h, 08Bh, 0D8h, 08Bh, 0C3h, 05Dh
        db  05Fh, 05Eh, 05Bh, 083h, 0C4h, 00Ch, 0C3h, 08Bh, 0FFh
sub_4_0A98:
        sub    ecx, ecx
        db  039h, 00Dh, 040h, 029h, 000h, 000h
        je     SHORT loc_4_0AB9
        db  0BAh, 040h, 029h, 000h, 000h
        inc    eax
        mov    eax, dword ptr [esp + 4]
loc_4_0AAC:
        cmp    dword ptr [edx], eax
        je     SHORT loc_4_0AB9
        add    edx, 8
        inc    ecx
        cmp    dword ptr [edx], 0
        jne    SHORT loc_4_0AAC
loc_4_0AB9:
        db  08Bh, 004h, 0CDh, 044h, 029h, 000h, 000h
        ret
        db  08Dh, 049h, 000h
sub_4_0AC4:
        push   esi
        push   edi
        db  083h, 03Dh, 078h, 02Bh, 000h, 000h, 000h
        jne    SHORT loc_4_0AD4
        sub    eax, eax
        pop    edi
        pop    esi
        ret
loc_4_0AD4:
        push   dword ptr [esp + 0ch]
        call   sub_4_0A98
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_0AEC
        sub    eax, eax
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_4_0AEC:
        mov    edi, eax
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        db  08Bh, 03Dh, 078h, 029h, 000h, 000h
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        push   0
        call   sub_4_0BE4
        add    esp, 4
        mov    esi, eax
        db  0FFh, 035h, 078h, 029h, 000h, 000h
        db  0E8h, 012h, 013h, 0FCh, 0FFh
        add    esp, 4
        push   0
        call   sub_4_0BE4
        add    esp, 4
        sub    eax, esi
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
sub_4_0B3C:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 20h]
        or     eax, eax
        mov    ebx, dword ptr [esp + 1ch]
        je     SHORT loc_4_0B4D
        mov    dword ptr [eax], ebx
loc_4_0B4D:
        or     ebx, ebx
        mov    edx, dword ptr [esp + 14h]
        jne    SHORT loc_4_0B70
        mov    edi, dword ptr [esp + 10h]
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_4_0B70:
        or     edx, edx
        jne    SHORT loc_4_0B80
        mov    eax, 21h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_4_0B80:
        cmp    ebx, edx
        mov    edi, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 18h]
        jg     SHORT loc_4_0BBC
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        cmp    ebx, edx
        jge    SHORT loc_4_0BB6
        sub    edx, ebx
        mov    edi, dword ptr [esp + 10h]
        add    edi, ebx
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
loc_4_0BB6:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_4_0BBC:
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    eax, 9
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
sub_4_0BD4:
        db  0E8h, 03Fh, 0F4h, 0FBh, 0FFh
        shr    eax, 3
        ret
        db  08Dh, 049h, 000h, 0C3h, 08Dh, 049h, 000h
sub_4_0BE4:
        db  083h, 03Dh, 084h, 029h, 000h, 000h, 000h
        jne    SHORT loc_4_0C01
        db  0E8h, 0F6h, 0FAh, 0FBh, 0FFh
        db  0A3h, 084h, 029h, 000h, 000h
        db  0E8h, 01Ch, 0F4h, 0FBh, 0FFh
        db  0A3h, 088h, 029h, 000h, 000h
loc_4_0C01:
        db  0E8h, 012h, 0F4h, 0FBh, 0FFh
        db  02Bh, 005h, 088h, 029h, 000h, 000h
        mov    ecx, 3e8h
        sub    edx, edx
        div    ecx
        db  003h, 005h, 084h, 029h, 000h, 000h
        mov    ecx, dword ptr [esp + 4]
        or     ecx, ecx
        je     SHORT loc_4_0C25
        mov    dword ptr [ecx], eax
loc_4_0C25:
        ret
        db  08Bh, 0FFh
sub_4_0C28:
        sub    esp, 24h
        push   edi
        db  083h, 03Dh, 0CCh, 028h, 000h, 000h, 000h
        jne    SHORT loc_4_0C47
        cmp    dword ptr [esp + 2ch], 3e8h
        jbe    SHORT loc_4_0C47
        mov    dword ptr [esp + 2ch], 3e8h
loc_4_0C47:
        sub    eax, eax
        lea    edi, [esp + 4]
        stosd
        stosd
        stosd
        mov    dword ptr [esp + 4], 1
        mov    dword ptr [esp + 8], eax
        mov    dword ptr [esp + 0ch], eax
        lea    edi, [esp + 10h]
        mov    ecx, 6
        rep stosd
        mov    dword ptr [esp + 1ch], eax
        mov    dword ptr [esp + 24h], eax
        db  0A1h, 0C8h, 028h, 000h, 000h
        mov    dword ptr [esp + 20h], eax
        lea    eax, [esp + 4]
        push   eax
        db  068h, 0B0h, 00Ch, 000h, 000h
        push   dword ptr [esp + 34h]
        lea    eax, [esp + 1ch]
        push   eax
        db  0E8h, 06Ch, 0F3h, 0FBh, 0FFh
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_4_0CA8
        lea    eax, [esp + 4]
        push   eax
        db  0E8h, 0C7h, 0F3h, 0FBh, 0FFh
        add    esp, 4
loc_4_0CA8:
        pop    edi
        add    esp, 24h
        ret
        db  08Dh, 049h, 000h
loc_4_0CB0:
        push   0
        push   dword ptr [esp + 0ch]
        db  0E8h, 0BDh, 0F3h, 0FBh, 0FFh
        add    esp, 8
        ret
        db  090h
sub_4_0CC0:
        push   dword ptr [esp + 4]
        db  0E8h, 07Fh, 0F3h, 0FBh, 0FFh
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
sub_4_0CD0:
        movzx  eax, word ptr [esp + 4]
        push   eax
        db  0E8h, 061h, 0F3h, 0FBh, 0FFh
        add    esp, 4
        ret
        db  090h
sub_4_0CE0:
        sub    eax, eax
        ret
        db  090h
sub_4_0CE4:
        push   ebp
        mov    ebp, esp
        sub    esp, 51ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0ffffffffh
        mov    dword ptr [ebp - 0ch], 0ffffffffh
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 51h]
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 9ch], eax
        db  0C7h, 085h, 068h, 0FFh, 0FFh, 0FFh, 088h, 02Ah, 000h, 000h
        mov    dword ptr [ebp - 94h], 80000002h
        push   dword ptr [ebp - 9ch]
        push   dword ptr [ebp - 98h]
        push   dword ptr [ebp - 94h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_4_0DFC
        cmp    eax, 2
        jne    NEAR PTR loc_4_1090
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 90h], eax
        db  0C7h, 085h, 074h, 0FFh, 0FFh, 0FFh, 0B4h, 02Ah, 000h, 000h
        mov    dword ptr [ebp - 88h], 80000002h
        push   dword ptr [ebp - 90h]
        push   dword ptr [ebp - 8ch]
        push   dword ptr [ebp - 88h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 04Ah, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_4_1090
        mov    dword ptr [ebp - 84h], 1
        mov    dword ptr [ebp - 80h], 4
        lea    eax, [ebp - 84h]
        mov    dword ptr [ebp - 7ch], eax
        mov    dword ptr [ebp - 78h], 3
        mov    dword ptr [ebp - 74h], 0
        db  0C7h, 045h, 090h, 0E0h, 02Ah, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 6ch], eax
        push   dword ptr [ebp - 80h]
        push   dword ptr [ebp - 7ch]
        push   dword ptr [ebp - 78h]
        push   dword ptr [ebp - 74h]
        push   dword ptr [ebp - 70h]
        push   dword ptr [ebp - 6ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        add    esp, 18h
        mov    esi, eax
        or     esi, esi
        jne    NEAR PTR loc_4_1052
loc_4_0DFC:
        mov    dword ptr [ebp - 68h], 4
        db  0C7h, 045h, 09Ch, 078h, 02Bh, 000h, 000h
        mov    dword ptr [ebp - 60h], 3
        mov    dword ptr [ebp - 5ch], 0
        db  0C7h, 045h, 0A8h, 0E8h, 02Ah, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 54h], eax
        push   dword ptr [ebp - 68h]
        push   dword ptr [ebp - 64h]
        push   dword ptr [ebp - 60h]
        push   dword ptr [ebp - 5ch]
        push   dword ptr [ebp - 58h]
        push   dword ptr [ebp - 54h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        add    esp, 18h
        mov    esi, eax
        or     esi, esi
        jne    NEAR PTR loc_4_1052
        db  0BEh, 0F4h, 02Ah, 000h, 000h
        lea    edi, [ebp - 11ch]
        movsd
        movsd
        movsb
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0eh]
        cmp    eax, 63h
        jle    SHORT loc_4_0E74
        mov    esi, 57h
        jmp    NEAR PTR loc_4_1052
        db  090h
loc_4_0E74:
        mov    ecx, 0ah
        cdq
        idiv   ecx
        add    al, 30h
        mov    byte ptr [ebp - 114h], al
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0eh]
        cdq
        idiv   ecx
        add    dl, 30h
        mov    byte ptr [ebp - 113h], dl
        mov    byte ptr [ebp - 112h], 0
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 50h], eax
        lea    eax, [ebp - 11ch]
        mov    dword ptr [ebp - 4ch], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 48h], eax
        push   dword ptr [ebp - 50h]
        push   dword ptr [ebp - 4ch]
        push   dword ptr [ebp - 48h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 04Ah, 001h, 001h, 000h
        add    esp, 0ch
        mov    esi, eax
        or     esi, esi
        jne    NEAR PTR loc_4_1052
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0eh]
        mov    dword ptr [ebp - 0dch], eax
        mov    eax, dword ptr [ebx + 11h]
        mov    dword ptr [ebp - 0d8h], eax
        mov    eax, dword ptr [ebx + 15h]
        mov    dword ptr [ebp - 0d4h], eax
        mov    eax, dword ptr [ebx + 19h]
        mov    dword ptr [ebp - 0d0h], eax
        mov    eax, dword ptr [ebx + 1dh]
        mov    dword ptr [ebp - 0cch], eax
        mov    eax, dword ptr [ebx + 21h]
        mov    dword ptr [ebp - 0c8h], eax
        mov    eax, dword ptr [ebx + 25h]
        mov    dword ptr [ebp - 0c4h], eax
        mov    eax, dword ptr [ebx + 29h]
        mov    dword ptr [ebp - 0c0h], eax
        mov    eax, dword ptr [ebx + 2dh]
        mov    dword ptr [ebp - 0bch], eax
        mov    eax, dword ptr [ebx + 31h]
        mov    dword ptr [ebp - 0b8h], eax
        mov    al, byte ptr [ebx + 8]
        mov    byte ptr [ebp - 0b0h], al
        mov    eax, dword ptr [ebx + 0dh]
        mov    dword ptr [ebp - 0b4h], eax
        mov    eax, dword ptr [ebx + 0dh]
        lea    edi, [ebp - 0afh]
        mov    esi, dword ptr [ebx + 9]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    dword ptr [ebp - 44h], 3dh
        lea    eax, [ebp - 0dch]
        mov    dword ptr [ebp - 40h], eax
        mov    dword ptr [ebp - 3ch], 3
        mov    dword ptr [ebp - 38h], 0
        db  0C7h, 045h, 0CCh, 000h, 02Bh, 000h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 30h], eax
        push   dword ptr [ebp - 44h]
        push   dword ptr [ebp - 40h]
        push   dword ptr [ebp - 3ch]
        push   dword ptr [ebp - 38h]
        push   dword ptr [ebp - 34h]
        push   dword ptr [ebp - 30h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        add    esp, 18h
        mov    esi, eax
        or     esi, esi
        jne    NEAR PTR loc_4_1052
        lea    edx, [ebp - 51ch]
        mov    eax, dword ptr [ebp - 4]
        mov    ebx, dword ptr [eax + 2ah]
        sub    eax, ebx
        cmp    eax, -2ah
        je     SHORT loc_4_0FEE
loc_4_0FC6:
        mov    edi, edx
        movzx  eax, byte ptr [ebx + 9]
        add    eax, 2
        lea    esi, [ebx + 8]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        add    edx, eax
        mov    ebx, dword ptr [ebx]
        mov    eax, dword ptr [ebp - 4]
        sub    eax, ebx
        cmp    eax, -2ah
        jne    SHORT loc_4_0FC6
loc_4_0FEE:
        mov    byte ptr [edx], 0ffh
        mov    byte ptr [edx + 1], 0
        add    edx, 2
        lea    eax, [ebp - 51ch]
        sub    edx, eax
        mov    dword ptr [ebp - 2ch], edx
        mov    dword ptr [ebp - 28h], eax
        mov    dword ptr [ebp - 24h], 3
        mov    dword ptr [ebp - 20h], 0
        db  0C7h, 045h, 0E4h, 00Ch, 02Bh, 000h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 18h], eax
        push   dword ptr [ebp - 2ch]
        push   dword ptr [ebp - 28h]
        push   dword ptr [ebp - 24h]
        push   dword ptr [ebp - 20h]
        push   dword ptr [ebp - 1ch]
        push   dword ptr [ebp - 18h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        add    esp, 18h
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_4_1052
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 12h], 0
loc_4_1052:
        cmp    dword ptr [ebp - 8], -1
        je     SHORT loc_4_1070
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 14h], eax
        push   dword ptr [ebp - 14h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
loc_4_1070:
        cmp    dword ptr [ebp - 0ch], -1
        je     SHORT loc_4_108E
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 10h], eax
        push   dword ptr [ebp - 10h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
loc_4_108E:
        mov    eax, esi
loc_4_1090:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_4_1098:
        push   esi
        db  08Bh, 035h, 084h, 02Bh, 000h, 000h
        db  081h, 0FEh, 084h, 02Bh, 000h, 000h
        je     SHORT loc_4_10C8
        nop
loc_4_10A8:
        mov    eax, esi
        mov    ecx, dword ptr [esi + 51h]
        mov    esi, dword ptr [esi]
        cmp    dword ptr [ecx + 12h], 0
        je     SHORT loc_4_10C0
        push   0
        push   eax
        call   sub_4_0CE4
        add    esp, 8
loc_4_10C0:
        db  081h, 0FEh, 084h, 02Bh, 000h, 000h
        jne    SHORT loc_4_10A8
loc_4_10C8:
        pop    esi
        db  0C7h, 005h, 0FCh, 029h, 000h, 000h, 000h, 000h, 000h, 000h
        ret
sub_4_10D4:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 51h]
        mov    dword ptr [eax + 12h], 1
        db  083h, 03Dh, 0FCh, 029h, 000h, 000h, 000h
        jne    SHORT loc_4_10FD
        db  068h, 098h, 010h, 000h, 000h
        db  0E8h, 0DBh, 00Eh, 0FCh, 0FFh
        add    esp, 4
        db  0A3h, 000h, 02Ah, 000h, 000h
loc_4_10FD:
        sub    eax, eax
        ret
sub_4_1100:
        push   0
        db  0FFh, 035h, 084h, 02Bh, 000h, 000h
        call   sub_4_10D4
        add    esp, 8
        ret
        db  08Dh, 049h, 032h
_PTEXT ends
        end
