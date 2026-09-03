; enable_icode_03.asm -- ENABLE object 3 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_3_0000:
        call   sub_3_0116
        jb     SHORT loc_3_006F
        call   sub_3_009E
        jb     NEAR PTR loc_3_009C
        call   sub_3_0114
        call   sub_3_00E3
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 011h, 000h, 017h, 000h
        pop    ecx
        db  0CDh, 020h, 0B6h, 000h, 001h, 000h
        mov    esi, edx
        db  0BFh, 0E8h, 000h, 000h, 000h
        cld
        rep movsb
        db  0BEh, 028h, 001h, 000h, 000h
        mov    ecx, 0ch
        rep movsb
        db  0CDh, 020h, 0B6h, 000h, 001h, 000h
        mov    esi, edx
        db  0BFh, 03Ch, 001h, 000h, 000h
        cld
        rep movsb
        db  0BEh, 07Ch, 001h, 000h, 000h
        mov    ecx, 0ch
        rep movsb
        db  0BEh, 0A0h, 004h, 000h, 000h
        xor    edx, edx
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        clc
        ret
loc_3_006F:
        xchg   ebx, eax
        db  00Fh, 0B7h, 01Dh, 012h, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        xchg   ecx, ebx
        xchg   ebx, eax
        db  00Fh, 0B7h, 03Dh, 00Ch, 000h, 000h, 000h
        db  003h, 03Dh, 004h, 000h, 000h, 000h
        db  0BEh, 032h, 002h, 000h, 000h
        xor    edx, edx
        xor    eax, eax
        db  0CDh, 020h, 004h, 000h, 017h, 000h
loc_3_009C:
        stc
        ret
sub_3_009E:
        db  0CDh, 020h, 000h, 000h, 00Dh, 000h
        cmp    eax, 20ah
        jge    SHORT loc_3_00C9
loc_3_00AB:
        db  083h, 025h, 024h, 002h, 000h, 000h, 0FDh
        db  083h, 025h, 06Eh, 002h, 000h, 000h, 0FDh
        db  083h, 025h, 044h, 002h, 000h, 000h, 0FDh
        db  083h, 025h, 03Ch, 002h, 000h, 000h, 0FDh
        stc
        ret
loc_3_00C9:
        mov    eax, 0d0012h
        db  0BEh, 0C5h, 000h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        jb     SHORT loc_3_00AB
        db  089h, 035h, 0D0h, 003h, 000h, 000h
        clc
        ret
sub_3_00E3:
        db  0CDh, 020h, 000h, 000h, 00Ch, 000h
        cmp    eax, 400h
        jge    SHORT loc_3_00FA
        db  083h, 025h, 044h, 002h, 000h, 000h, 0F1h
        clc
        jmp    SHORT loc_3_0113
loc_3_00FA:
        mov    eax, 0c0003h
        db  0BEh, 098h, 000h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        jb     SHORT loc_3_0113
        db  089h, 035h, 0D4h, 003h, 000h, 000h
        clc
loc_3_0113:
        ret
sub_3_0114:
        clc
        ret
sub_3_0116:
        xor    ah, ah
        db  0A0h, 0E6h, 000h, 000h, 000h
        db  0BEh, 030h, 000h, 000h, 000h
        db  0BFh, 0D8h, 000h, 000h, 000h
        db  0CDh, 020h, 0AFh, 000h, 001h, 000h
        db  0A2h, 0E6h, 000h, 000h, 000h
        db  0C7h, 005h, 020h, 002h, 000h, 000h, 018h, 000h, 000h, 000h
        db  0C7h, 005h, 038h, 002h, 000h, 000h, 008h, 000h, 000h, 000h
        db  0C7h, 005h, 040h, 002h, 000h, 000h, 01Ch, 000h, 000h, 000h
        db  0C7h, 005h, 05Ch, 002h, 000h, 000h, 00Eh, 000h, 000h, 000h
        db  0C7h, 005h, 06Ah, 002h, 000h, 000h, 008h, 000h, 000h, 000h
        db  0C7h, 005h, 072h, 002h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  0C7h, 005h, 07Eh, 002h, 000h, 000h, 018h, 000h, 000h, 000h
        db  0C7h, 005h, 096h, 002h, 000h, 000h, 030h, 000h, 000h, 000h
        db  0C7h, 005h, 0C6h, 002h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  083h, 00Dh, 0CAh, 002h, 000h, 000h, 05Eh
        db  0A0h, 0E6h, 000h, 000h, 000h
        or     al, al
        je     SHORT loc_3_01C1
        db  083h, 00Dh, 0CAh, 002h, 000h, 000h, 008h
        db  083h, 00Dh, 044h, 002h, 000h, 000h, 008h
        db  083h, 00Dh, 03Ch, 002h, 000h, 000h, 008h
        db  083h, 00Dh, 024h, 002h, 000h, 000h, 008h
        db  083h, 00Dh, 06Eh, 002h, 000h, 000h, 008h
        jmp    SHORT loc_3_01E4
loc_3_01C1:
        db  083h, 025h, 0CAh, 002h, 000h, 000h, 0F7h
        db  083h, 025h, 044h, 002h, 000h, 000h, 0F7h
        db  083h, 025h, 03Ch, 002h, 000h, 000h, 0F7h
        db  083h, 025h, 024h, 002h, 000h, 000h, 0F7h
        db  083h, 025h, 06Eh, 002h, 000h, 000h, 0F7h
loc_3_01E4:
        db  083h, 00Dh, 044h, 002h, 000h, 000h, 03Eh
        db  0C7h, 005h, 048h, 002h, 000h, 000h, 028h, 000h, 000h, 000h
        db  0C7h, 005h, 04Ch, 002h, 000h, 000h, 02Ch, 001h, 000h, 000h
        db  0C7h, 005h, 050h, 002h, 000h, 000h, 028h, 000h, 000h, 000h
        db  083h, 025h, 044h, 002h, 000h, 000h, 0BFh
        db  083h, 025h, 03Ch, 002h, 000h, 000h, 0FEh
        db  083h, 00Dh, 03Ch, 002h, 000h, 000h, 01Eh
        db  083h, 025h, 06Eh, 002h, 000h, 000h, 0FEh
        db  081h, 00Dh, 06Eh, 002h, 000h, 000h, 0FEh, 001h, 000h, 000h
        db  083h, 025h, 024h, 002h, 000h, 000h, 0FEh
        db  083h, 00Dh, 024h, 002h, 000h, 000h, 07Eh
        db  0C7h, 005h, 028h, 002h, 000h, 000h, 0E8h, 003h, 000h, 000h
        db  0C7h, 005h, 02Ch, 002h, 000h, 000h, 0F4h, 001h, 000h, 000h
        db  0C7h, 005h, 030h, 002h, 000h, 000h, 0E8h, 003h, 000h, 000h
        db  0C7h, 005h, 034h, 002h, 000h, 000h, 000h, 000h, 000h, 000h
        db  083h, 00Dh, 076h, 002h, 000h, 000h, 003h
        db  0C7h, 005h, 07Ah, 002h, 000h, 000h, 0E0h, 093h, 004h, 000h
        db  0C7h, 005h, 09Eh, 002h, 000h, 000h, 002h, 000h, 000h, 000h
        db  0C7h, 005h, 0A2h, 002h, 000h, 000h, 0F4h, 001h, 000h, 000h
        db  0C7h, 005h, 0A6h, 002h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0AAh, 002h, 000h, 000h, 003h, 000h, 000h, 000h
        db  0C7h, 005h, 0AEh, 002h, 000h, 000h, 0F4h, 001h, 000h, 000h
        db  0C7h, 005h, 0B2h, 002h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0B6h, 002h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 005h, 0BAh, 002h, 000h, 000h, 0F4h, 001h, 000h, 000h
        db  0C7h, 005h, 0BEh, 002h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0C2h, 002h, 000h, 000h, 000h, 000h, 000h, 000h
        db  083h, 00Dh, 09Ah, 002h, 000h, 000h, 002h
        db  0C7h, 005h, 08Ah, 002h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 082h, 002h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  0C7h, 005h, 08Eh, 002h, 000h, 000h, 02Ch, 001h, 000h, 000h
        db  0C7h, 005h, 092h, 002h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0A1h, 000h, 002h, 000h, 000h
        db  0BEh, 030h, 000h, 000h, 000h
        db  0BFh, 04Dh, 000h, 000h, 000h
        db  0CDh, 020h, 0AFh, 000h, 001h, 000h
        db  0A3h, 000h, 002h, 000h, 000h
        db  066h, 0A1h, 016h, 002h, 000h, 000h
        db  0BEh, 030h, 000h, 000h, 000h
        db  0BFh, 05Dh, 000h, 000h, 000h
        db  0CDh, 020h, 0AFh, 000h, 001h, 000h
        db  066h, 0A3h, 016h, 002h, 000h, 000h
        db  0A1h, 004h, 002h, 000h, 000h
        db  0BEh, 030h, 000h, 000h, 000h
        db  0BFh, 06Eh, 000h, 000h, 000h
        db  0CDh, 020h, 0AFh, 000h, 001h, 000h
        db  0A3h, 004h, 002h, 000h, 000h
        db  066h, 0A1h, 018h, 002h, 000h, 000h
        db  0BEh, 030h, 000h, 000h, 000h
        db  0BFh, 080h, 000h, 000h, 000h
        db  0CDh, 020h, 0AFh, 000h, 001h, 000h
        db  066h, 0A3h, 018h, 002h, 000h, 000h
        db  0A1h, 00Ch, 002h, 000h, 000h
        db  0BEh, 030h, 000h, 000h, 000h
        db  0BFh, 08Eh, 000h, 000h, 000h
        db  0CDh, 020h, 0AFh, 000h, 001h, 000h
        db  0A3h, 00Ch, 002h, 000h, 000h
        db  066h, 0A1h, 01Ch, 002h, 000h, 000h
        db  0BEh, 030h, 000h, 000h, 000h
        db  0BFh, 09Fh, 000h, 000h, 000h
        db  0CDh, 020h, 0AFh, 000h, 001h, 000h
        db  066h, 0A3h, 01Ch, 002h, 000h, 000h
        db  0A1h, 008h, 002h, 000h, 000h
        db  0BEh, 030h, 000h, 000h, 000h
        db  0BFh, 0A8h, 000h, 000h, 000h
        db  0CDh, 020h, 0AFh, 000h, 001h, 000h
        db  0A3h, 008h, 002h, 000h, 000h
        db  066h, 0A1h, 01Ah, 002h, 000h, 000h
        db  0BEh, 030h, 000h, 000h, 000h
        db  0BFh, 0B8h, 000h, 000h, 000h
        db  0CDh, 020h, 0AFh, 000h, 001h, 000h
        db  066h, 0A3h, 01Ah, 002h, 000h, 000h
        db  0A1h, 010h, 002h, 000h, 000h
        db  0BEh, 030h, 000h, 000h, 000h
        db  0BFh, 0CAh, 000h, 000h, 000h
        db  0CDh, 020h, 0AFh, 000h, 001h, 000h
        db  0A3h, 010h, 002h, 000h, 000h
        db  066h, 0A1h, 01Eh, 002h, 000h, 000h
        db  0BEh, 030h, 000h, 000h, 000h
        db  0BFh, 0C0h, 000h, 000h, 000h
        db  0CDh, 020h, 0AFh, 000h, 001h, 000h
        db  066h, 0A3h, 01Eh, 002h, 000h, 000h
        db  0A1h, 0FCh, 001h, 000h, 000h
        db  0BEh, 030h, 000h, 000h, 000h
        db  0BFh, 037h, 000h, 000h, 000h
        db  0CDh, 020h, 0AFh, 000h, 001h, 000h
        db  0A3h, 0FCh, 001h, 000h, 000h
        db  066h, 0A1h, 014h, 002h, 000h, 000h
        db  0BEh, 030h, 000h, 000h, 000h
        db  0BFh, 044h, 000h, 000h, 000h
        db  0CDh, 020h, 0AFh, 000h, 001h, 000h
        db  066h, 0A3h, 014h, 002h, 000h, 000h
        db  0BEh, 0B4h, 003h, 000h, 000h
        xor    eax, eax
        db  0C7h, 006h, 096h, 000h, 000h, 000h
        db  0C7h, 046h, 004h, 021h, 000h, 000h, 000h
        mov    dword ptr [esi + 8], eax
        mov    byte ptr [esi + 0ch], al
        mov    byte ptr [esi + 0dh], al
        mov    byte ptr [esi + 0eh], al
        mov    byte ptr [esi + 0fh], al
        mov    word ptr [esi + 10h], ax
        mov    byte ptr [esi + 12h], 0
        mov    byte ptr [esi + 13h], al
        mov    dword ptr [esi + 14h], eax
        mov    dword ptr [esi + 18h], eax
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 20h], eax
        mov    byte ptr [esi + 24h], al
        mov    byte ptr [esi + 25h], 0e0h
        mov    byte ptr [esi + 26h], 45h
        mov    byte ptr [esi + 27h], 0c5h
        mov    byte ptr [esi + 29h], al
        mov    byte ptr [esi + 2ah], al
        clc
        ret
        db  000h, 000h
loc_3_04A0:
        push   1
        push   0
        db  068h, 052h, 014h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 0ch
        db  0A3h, 0C8h, 008h, 000h, 000h
        ret
_ITEXT ends
        end
