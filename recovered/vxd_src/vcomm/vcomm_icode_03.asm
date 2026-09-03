; vcomm_icode_03.asm -- VCOMM object 3 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_3_0000:
        db  0CDh, 020h, 000h, 000h, 00Eh, 000h
        jb     SHORT loc_3_000D
        db  0A3h, 004h, 000h, 000h, 000h
loc_3_000D:
        call   sub_3_0048
        call   sub_3_005C
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_3_0038
        mov    ecx, 8
        mov    eax, 4
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     SHORT loc_3_0044
        db  089h, 035h, 000h, 000h, 000h, 000h
loc_3_0038:
        db  0BEh, 044h, 003h, 000h, 000h
        db  0CDh, 020h, 0EFh, 000h, 001h, 000h
        clc
loc_3_0044:
        ret
        db  000h, 000h, 000h
sub_3_0048:
        push   0
        push   0
        db  0CDh, 020h, 001h, 001h, 001h, 000h
        add    esp, 8
        db  0A3h, 090h, 000h, 000h, 000h
        ret
        db  000h
sub_3_005C:
        db  068h, 054h, 003h, 000h, 000h
        db  068h, 0F4h, 002h, 000h, 000h
        db  0CDh, 020h, 029h, 001h, 001h, 000h
        add    esp, 8
        ret
sub_3_0070:
        mov    ecx, 34h
        mov    eax, 4
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     SHORT loc_3_00A6
        db  089h, 035h, 0F8h, 002h, 000h, 000h
        db  0CDh, 020h, 0B6h, 000h, 001h, 000h
        cld
        mov    esi, edx
        db  0BFh, 0A4h, 002h, 000h, 000h
        rep movsb
        db  089h, 03Dh, 0F4h, 002h, 000h, 000h
        db  0C6h, 005h, 0FCh, 002h, 000h, 000h, 0FFh
        clc
loc_3_00A6:
        ret
loc_3_00A7:
        db  080h, 03Dh, 0FCh, 002h, 000h, 000h, 0FFh
        je     SHORT loc_3_00B5
        call   sub_3_0070
loc_3_00B5:
        mov    eax, dword ptr [400h]
        mov    edx, dword ptr [404h]
        db  0A3h, 041h, 003h, 000h, 000h
        db  089h, 015h, 045h, 003h, 000h, 000h
        db  08Bh, 035h, 0F8h, 002h, 000h, 000h
        test   esi, esi
        je     SHORT loc_3_00F6
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_3_00F6
loc_3_00DD:
        mov    ebx, eax
        test   byte ptr [ebx + 1eh], 2
        jne    SHORT loc_3_00EC
        push   ebx
        db  0E8h, 047h, 00Eh, 0FDh, 0FFh
        pop    ecx
loc_3_00EC:
        mov    eax, ebx
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_3_00DD
loc_3_00F6:
        db  0FEh, 005h, 00Dh, 003h, 000h, 000h
        clc
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 032h, 045h, 038h, 068h, 033h, 045h, 038h, 068h, 032h, 046h
        db  038h, 068h, 033h, 046h, 038h, 068h, 052h, 04Fh, 04Fh, 054h, 05Ch, 02Ah, 050h, 04Eh, 050h, 030h
        db  035h, 030h, 030h, 05Ch, 020h, 020h, 020h, 020h, 000h, 042h, 06Fh, 06Fh, 074h, 043h, 06Fh, 06Eh
        db  066h, 069h, 067h, 000h, 000h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 014h, 000h, 000h, 000h
        db  002h, 000h, 000h, 000h, 000h, 000h, 00Ch, 000h, 0F8h, 003h, 0FFh, 003h, 000h, 000h, 000h, 003h
        db  010h, 000h, 000h, 000h, 004h, 000h, 000h, 000h, 001h, 000h, 004h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 02Ah, 050h, 04Eh, 050h, 030h, 035h, 030h, 030h, 000h, 048h, 061h, 072h
        db  064h, 077h, 061h, 072h, 065h, 049h, 044h, 000h
_ITEXT ends
        end
