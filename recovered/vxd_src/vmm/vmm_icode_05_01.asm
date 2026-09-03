; vmm_icode_05_01.asm -- VMM object 5 segment _ITEXT  (part 1/2)
.386p
_ITEXT segment dword public use32 'ICODE'

sub_5_0000:
        add    dword ptr [eax], eax
        lea    ebx, [ebx + 0b8h]
        db  0BEh, 0A4h, 0E5h, 000h, 000h
        db  0BFh, 0B0h, 060h, 000h, 000h
        xor    edx, edx
        mov    ecx, 11h
loc_5_0019:
        cld
        lodsd
        db  068h, 00Ah, 064h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        pushad
        mov    esi, dword ptr [edi + edx*4]
        db  0CDh, 020h, 0C2h, 000h, 001h, 000h
        popad
        db  068h, 012h, 064h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        push   edi
        mov    edi, edx
        shl    edi, 2
        add    edi, edx
        add    edi, edx
        mov    eax, dword ptr [edi + ebx]
        movzx  edi, word ptr [edi + ebx + 4]
        test   edi, edi
        je     SHORT loc_5_005D
        db  068h, 01Ah, 064h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
loc_5_005D:
        pop    edi
        db  068h, 02Eh, 064h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        inc    edx
        loop   loc_5_0019
        db  068h, 032h, 064h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  068h, 036h, 064h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0CDh, 020h, 0C4h, 000h, 001h, 000h
        db  068h, 067h, 064h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        je     SHORT loc_5_0109
        db  068h, 06Bh, 064h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  068h, 06Fh, 064h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  068h, 073h, 064h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  068h, 097h, 064h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  068h, 0C2h, 064h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  068h, 0E5h, 064h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0BEh, 02Ch, 0E7h, 000h, 000h
        xor    edx, edx
        mov    ecx, 11h
loc_5_00E3:
        cld
        lodsd
        db  068h, 010h, 065h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        pushad
        mov    esi, dword ptr [edi + edx*4]
        db  0CDh, 020h, 0C2h, 000h, 001h, 000h
        popad
        db  068h, 018h, 065h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        inc    edx
        loop   loc_5_00E3
loc_5_0109:
        popad
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FAh, 036h, 08Fh, 005h, 00Bh, 011h, 000h, 000h
        db  036h, 08Fh, 005h, 00Fh, 011h, 000h, 000h, 06Ah, 000h, 060h, 0FCh, 08Bh, 0ECh, 036h, 08Bh, 01Dh
        db  098h, 0F6h, 000h, 000h, 066h, 0B9h, 030h, 000h, 0B8h, 0A1h, 011h, 000h, 000h, 0F6h, 045h, 02Eh
        db  002h, 075h, 01Bh, 0F6h, 045h, 028h, 003h, 074h, 01Eh, 08Ch, 05Dh, 03Ch, 08Ch, 045h, 038h, 08Ch
        db  065h, 040h, 08Ch, 06Dh, 044h, 08Eh, 0E1h, 08Eh, 0E9h, 0B8h, 08Bh, 011h, 000h, 000h, 08Eh, 0D9h
        db  08Eh, 0C1h, 087h, 063h, 04Ch, 0EBh, 00Fh, 01Eh, 006h, 00Fh, 0A0h, 00Fh, 0A8h, 08Eh, 0D9h, 08Eh
        db  0C1h, 0B8h, 0AEh, 011h, 000h, 000h, 050h, 036h, 0FFh, 02Dh, 00Bh, 011h, 000h, 000h, 0FAh, 036h
        db  08Fh, 005h, 00Bh, 011h, 000h, 000h, 036h, 08Fh, 005h, 00Fh, 011h, 000h, 000h, 058h, 0FFh, 0E0h
        db  036h, 08Bh, 02Dh, 00Ch, 0F7h, 000h, 000h, 08Bh, 06Dh, 008h, 08Eh, 05Dh, 03Ch, 08Eh, 045h, 038h
        db  08Eh, 065h, 040h, 08Eh, 06Dh, 044h, 036h, 08Bh, 03Dh, 098h, 0F6h, 000h, 000h, 036h, 087h, 067h
        db  04Ch, 0EBh, 006h, 00Fh, 0A9h, 00Fh, 0A1h, 007h, 01Fh, 061h, 083h, 0C4h, 004h, 036h, 0FFh, 02Dh
        db  00Bh, 011h, 000h, 000h, 000h, 0CDh, 020h, 000h, 000h, 033h, 000h, 00Fh, 082h, 0E4h, 000h, 000h
        db  000h, 068h, 024h, 065h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 068h, 028h, 065h, 000h
        db  000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0CDh, 020h, 0C4h, 000h, 001h, 000h, 068h, 058h, 065h
        db  000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 074h, 010h, 06Ah, 000h, 0FFh, 035h, 0D8h, 002h
        db  000h, 000h, 0CDh, 020h, 02Eh, 000h, 033h, 000h, 059h, 059h, 068h, 05Ch, 065h, 000h, 000h, 0CDh
        db  020h, 0F3h, 000h, 001h, 000h, 068h, 060h, 065h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0CDh, 020h, 0C4h, 000h, 001h, 000h, 068h, 090h, 065h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h
        db  000h, 074h, 010h, 06Ah, 000h, 0FFh, 035h, 0E0h, 002h, 000h, 000h, 0CDh, 020h, 02Eh, 000h, 033h
        db  000h, 059h, 059h, 068h, 094h, 065h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 068h, 098h
        db  065h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0CDh, 020h, 0C4h, 000h, 001h, 000h, 068h
        db  0CEh, 065h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 074h, 010h, 06Ah, 000h, 0FFh, 035h
        db  0D0h, 002h, 000h, 000h, 0CDh, 020h, 02Eh, 000h, 033h, 000h, 059h, 059h, 068h, 0D2h, 065h, 000h
        db  000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 068h, 0D6h, 065h, 000h, 000h, 0CDh, 020h, 0F3h, 000h
        db  001h, 000h, 0CDh, 020h, 0C4h, 000h, 001h, 000h, 068h, 00Bh, 066h, 000h, 000h, 0CDh, 020h, 0F3h
        db  000h, 001h, 000h, 074h, 010h, 06Ah, 000h, 0FFh, 035h, 0CCh, 002h, 000h, 000h, 0CDh, 020h, 02Eh
        db  000h, 033h, 000h, 059h, 059h, 0C3h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        pushad
        db  0CDh, 020h, 0C2h, 000h, 001h, 000h
        popad
        db  0BEh, 068h, 0EFh, 000h, 000h
        pushad
        db  0CDh, 020h, 0C2h, 000h, 001h, 000h
        popad
        pop    esi
        leave
        ret    4
sub_5_02D5:
        db  080h, 03Dh, 005h, 0BFh, 000h, 000h, 000h
        je     SHORT loc_5_0305
        push   esi
        push   edi
        mov    ecx, 10h
        lea    esi, [esp + 10h]
        db  0BFh, 010h, 066h, 000h, 000h
        rep movsd
        mov    ax, 73h
        mov    esi, dword ptr [esp + 0ch]
        db  0BFh, 010h, 066h, 000h, 000h
        int    41h
        movzx  eax, ax
        pop    edi
        pop    esi
loc_5_0305:
        ret
sub_5_0306:
        push   esi
        mov    ax, 79h
        mov    esi, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 0ch]
        int    41h
        movzx  eax, ax
        pop    esi
        test   ax, ax
        ret    8
        db  033h, 0FFh, 0BEh, 0EBh, 0FDh, 0FFh, 0FFh, 047h, 0B2h, 020h, 08Ah, 0CAh, 0E8h, 058h, 000h, 000h
        db  000h, 00Ah, 0E4h, 074h, 045h, 08Ah, 0C8h, 03Ch, 020h, 074h, 00Bh, 0E8h, 049h, 000h, 000h, 000h
        db  00Ah, 0E4h, 074h, 036h, 08Ah, 0D0h, 066h, 0B8h, 077h, 000h, 0B3h, 002h, 0CDh, 041h, 00Ah, 0E4h
        db  074h, 028h, 03Ch, 04Ch, 074h, 00Ah, 066h, 0B8h, 078h, 000h, 051h, 0CDh, 041h, 059h, 08Bh, 0F3h
        db  066h, 0B8h, 077h, 000h, 0B3h, 001h, 0CDh, 041h, 00Ah, 0E4h, 074h, 00Eh, 03Ch, 04Ch, 075h, 016h
        db  066h, 0B8h, 078h, 000h, 051h, 0CDh, 041h, 059h, 08Bh, 0FBh, 057h, 056h, 052h, 051h, 0E8h, 052h
        db  022h, 000h, 000h, 033h, 0C0h, 0CBh, 0B0h, 001h, 0CBh, 066h, 0B8h, 077h, 000h, 032h, 0DBh, 0CDh
        db  041h, 00Ah, 0E4h, 074h, 00Ch, 03Ch, 020h, 074h, 008h, 066h, 0B8h, 077h, 000h, 0B3h, 001h, 0CDh
        db  041h, 0C3h, 081h, 0FAh, 0FFh, 0FFh, 03Fh, 000h, 077h, 00Fh, 0E8h, 09Ah, 000h, 000h, 000h, 072h
        db  04Bh, 08Bh, 05Fh, 014h, 003h, 053h, 004h, 0EBh, 038h, 081h, 0FAh, 000h, 000h, 040h, 000h, 072h
        db  030h, 081h, 0FAh, 0FFh, 0FFh, 0FFh, 07Fh, 077h, 028h, 066h, 083h, 0FBh, 001h, 074h, 02Eh, 0E8h
        db  075h, 000h, 000h, 000h, 072h, 026h, 0CDh, 020h, 00Ch, 000h, 02Ah, 000h, 00Bh, 0DBh, 074h, 01Dh
        db  0C1h, 0EAh, 016h, 033h, 0C9h, 03Bh, 053h, 004h, 073h, 011h, 08Bh, 00Bh, 08Bh, 00Ch, 091h, 0EBh
        db  00Ah, 0C1h, 0EAh, 016h, 08Bh, 00Ch, 095h, 000h, 0E0h, 0BFh, 0FFh, 0F8h, 0C3h, 033h, 0C0h, 040h
        db  0F9h, 0C3h, 0F7h, 0C1h, 000h, 002h, 000h, 000h, 074h, 03Dh, 08Bh, 0D1h, 081h, 0E2h, 000h, 0F0h
        db  0FFh, 0FFh, 0F7h, 0C1h, 000h, 008h, 000h, 000h, 075h, 02Bh, 08Bh, 01Dh, 0ACh, 0F8h, 000h, 000h
        db  0C1h, 0EAh, 00Ch, 06Bh, 0D2h, 00Ah, 003h, 0DAh, 0F6h, 043h, 003h, 010h, 074h, 019h, 08Bh, 043h
        db  006h, 02Bh, 005h, 0E4h, 0F7h, 000h, 000h, 0B9h, 00Dh, 000h, 000h
Locate_Byte_In_ROM:
        add    byte ptr [ebx], dh
        sal    bh, cl
        int1
        shl    eax, 0ch
        mov    edx, eax
        clc
        ret
        db  0F9h, 0C3h, 08Bh, 03Dh, 098h, 0F6h, 000h, 000h, 066h, 00Bh, 0DBh, 074h, 031h, 08Bh, 035h, 010h
        db  0F7h, 000h, 000h, 08Bh, 07Eh, 0E4h, 066h, 03Bh, 05Fh, 018h, 074h, 022h, 08Bh, 07Fh, 040h, 00Bh
        db  0FFh, 075h, 0F3h, 08Bh, 076h, 02Ch, 00Bh, 0F6h, 075h, 006h, 08Bh, 035h, 014h, 0F7h, 000h, 000h
        db  03Bh, 035h, 010h, 0F7h, 000h, 000h, 075h, 0DBh, 033h, 0C0h, 040h, 040h, 0F9h, 0C3h, 0F8h, 0C3h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 056h, 08Bh, 074h, 024h, 008h, 081h
        db  0FEh, 0EBh, 0FDh, 0FFh, 0FFh, 075h, 005h, 0BEh, 000h, 000h, 080h, 0FFh, 056h, 068h, 07Ch, 071h
        db  000h, 000h, 0E8h, 028h, 0FEh, 0FFh, 0FFh, 083h, 0C4h, 008h, 081h, 0FEh, 000h, 000h, 080h, 0FFh
        db  00Fh, 082h, 082h, 000h, 000h, 000h, 081h, 0FEh, 0FFh, 0FFh, 0BFh, 0FFh, 073h, 07Ah, 0F7h, 0C6h
        db  003h, 000h, 000h, 000h, 075h, 072h, 08Bh, 0C6h, 083h, 0E0h, 0FCh, 0C1h, 0E0h, 00Ah, 050h, 068h
        db  09Ch, 071h, 000h, 000h, 0E8h, 0F6h, 0FDh, 0FFh, 0FFh, 08Bh, 0C6h, 025h, 0FFh, 0F3h, 0FFh, 0FFh
        db  0C1h, 0E8h, 00Ah, 083h, 0C4h, 008h, 08Bh, 080h, 000h, 000h, 080h, 0FFh, 0F6h, 0C4h, 002h, 075h
        db  007h, 068h, 0ACh, 071h, 000h, 000h, 0EBh, 045h, 0A8h, 001h, 075h, 007h, 068h, 0C8h, 071h, 000h
        db  000h, 0EBh, 03Ah, 0FFh, 036h, 068h, 0E0h, 071h, 000h, 000h, 0E8h, 0C0h, 0FDh, 0FFh, 0FFh, 083h
        db  0C4h, 008h, 056h, 0E8h, 0E7h, 000h, 000h, 000h, 08Ah, 044h, 024h, 00Ch, 03Ch, 020h, 074h, 013h
        db  00Fh, 0B6h, 0C0h, 050h, 068h, 0ECh, 071h, 000h, 000h, 0E8h, 0A1h, 0FDh, 0FFh, 0FFh, 083h, 0C4h
        db  008h, 0EBh, 012h, 083h, 0C6h, 004h, 0EBh, 00Fh, 068h, 088h, 071h, 000h, 000h, 0E8h, 08Dh, 0FDh
        db  0FFh, 0FFh, 083h, 0C4h, 004h, 02Bh, 0F6h, 068h, 000h, 072h, 000h, 000h, 0E8h, 07Eh, 0FDh, 0FFh
        db  0FFh, 083h, 0C4h, 004h, 08Bh, 0C6h, 05Eh, 0C2h, 008h, 000h, 056h, 08Bh, 074h, 024h, 008h, 081h
        db  0FEh, 0EBh, 0FDh, 0FFh, 0FFh, 075h, 005h, 0BEh, 000h, 0E0h, 0BFh, 0FFh, 056h, 068h, 004h, 072h
        db  000h, 000h, 0E8h, 058h, 0FDh, 0FFh, 0FFh, 083h, 0C4h, 008h, 081h, 0FEh, 000h, 0E0h, 0BFh, 0FFh
        db  072h, 05Bh, 081h, 0FEh, 000h, 0F0h, 0BFh, 0FFh, 073h, 053h, 0F7h, 0C6h, 003h, 000h, 000h, 000h
        db  075h, 04Bh, 08Bh, 0C6h, 083h, 0E0h, 0FCh, 0C1h, 0E0h, 014h, 050h, 068h, 024h, 072h, 000h, 000h
        db  0E8h, 02Ah, 0FDh, 0FFh, 0FFh, 083h, 0C4h, 008h, 0FFh, 036h, 068h, 034h, 072h, 000h, 000h, 0E8h
        db  01Bh, 0FDh, 0FFh, 0FFh, 083h, 0C4h, 008h, 056h, 0E8h, 042h, 000h, 000h, 000h, 08Ah, 044h, 024h
        db  00Ch, 03Ch, 020h, 074h, 013h, 00Fh, 0B6h, 0C0h, 050h, 068h, 040h, 072h, 000h, 000h, 0E8h, 0FCh
        db  0FCh, 0FFh, 0FFh, 083h, 0C4h, 008h, 0EBh, 012h, 083h, 0C6h, 004h, 0EBh, 00Fh, 068h, 010h, 072h
        db  000h, 000h, 0E8h, 0E8h, 0FCh, 0FFh, 0FFh, 083h, 0C4h, 004h, 02Bh, 0F6h, 068h, 054h, 072h, 000h
        db  000h, 0E8h, 0D9h, 0FCh, 0FFh, 0FFh, 083h, 0C4h, 004h, 08Bh, 0C6h, 05Eh, 0C2h, 008h, 000h, 056h
        db  08Bh, 074h, 024h, 008h, 08Bh, 006h, 0F6h, 0C4h, 002h, 00Fh, 084h, 0A3h, 000h, 000h, 000h, 0F6h
        db  0C4h, 008h, 074h, 007h, 068h, 058h, 072h, 000h, 000h, 0EBh, 005h, 068h, 060h, 072h, 000h, 000h
        db  0E8h, 0AAh, 0FCh, 0FFh, 0FFh, 083h, 0C4h, 004h, 0F6h, 006h, 001h, 074h, 007h, 068h, 068h, 072h
        db  000h, 000h, 0EBh, 005h, 068h, 078h, 072h, 000h, 000h, 0E8h, 091h, 0FCh, 0FFh, 0FFh, 083h, 0C4h
        db  004h, 08Bh, 006h, 0C1h, 0E8h, 00Ch, 050h, 068h, 088h, 072h, 000h, 000h, 0E8h, 07Eh, 0FCh, 0FFh
        db  0FFh, 083h, 0C4h, 008h, 0F6h, 006h, 002h, 074h, 007h, 068h, 090h, 072h, 000h, 000h, 0EBh, 005h
        db  068h, 098h, 072h, 000h, 000h, 0E8h, 065h, 0FCh, 0FFh, 0FFh, 083h, 0C4h, 004h, 0F6h, 006h, 004h
        db  074h, 007h, 068h, 0A0h, 072h, 000h, 000h, 0EBh, 005h, 068h, 0A8h, 072h, 000h, 000h, 0E8h, 04Ch
        db  0FCh, 0FFh, 0FFh, 083h, 0C4h, 004h, 0F6h, 006h, 020h, 074h, 007h, 068h, 0B0h, 072h, 000h, 000h
        db  0EBh, 005h, 068h, 0B4h, 072h, 000h, 000h, 0E8h, 033h, 0FCh, 0FFh, 0FFh, 083h, 0C4h, 004h, 0F6h
        db  006h, 040h, 074h, 007h, 068h, 0B8h, 072h, 000h, 000h, 0EBh, 023h, 068h, 0BCh, 072h, 000h, 000h
        db  0EBh, 01Ch, 00Bh, 0C0h, 074h, 013h, 0C1h, 0E8h, 00Ch, 050h, 068h, 0C0h, 072h, 000h, 000h, 0E8h
        db  00Bh, 0FCh, 0FFh, 0FFh, 083h, 0C4h, 008h, 0EBh, 00Dh, 068h, 0DCh, 072h, 000h, 000h, 0E8h, 0FCh
        db  0FBh, 0FFh, 0FFh, 083h, 0C4h, 004h, 05Eh, 0C2h, 004h, 000h
sub_5_06E0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, esi
        db  02Bh, 005h, 0ECh, 0FAh, 000h, 000h
        mov    ecx, 0ch
        sub    edx, edx
        div    ecx
        db  08Bh, 00Dh, 0D0h, 0F7h, 000h, 000h
        shr    ecx, 0ch
        cmp    eax, ecx
        jne    SHORT loc_5_0712
        db  068h, 0E4h, 072h, 000h, 000h
        call   sub_5_02D5
        add    esp, 4
        jmp    SHORT loc_5_0728
loc_5_0712:
        mov    eax, dword ptr [esi + 4]
        and    eax, 0fffff000h
        push   eax
        db  068h, 0F4h, 072h, 000h, 000h
        call   sub_5_02D5
        add    esp, 8
loc_5_0728:
        pop    esi
        ret    4
        db  083h, 0ECh, 004h, 053h, 056h, 057h, 055h, 08Bh, 074h, 024h, 018h, 081h, 0FEh, 0EBh, 0FDh, 0FFh
        db  0FFh, 075h, 002h, 02Bh, 0F6h, 02Bh, 0FFh, 081h, 0FEh, 000h, 000h, 000h, 0C0h, 073h, 00Dh, 06Bh
        db  0F6h, 00Ch, 08Bh, 01Dh, 0ECh, 0FAh, 000h, 000h, 003h, 0F3h, 0EBh, 006h, 08Bh, 01Dh, 0ECh, 0FAh
        db  000h, 000h, 056h, 068h, 0FCh, 072h, 000h, 000h, 0E8h, 06Ch, 0FBh, 0FFh, 0FFh, 083h, 0C4h, 008h
        db  08Bh, 0C6h, 02Bh, 0C3h, 0B9h, 00Ch, 000h, 000h, 000h, 02Bh, 0D2h, 0F7h, 0F1h, 08Bh, 0E8h, 00Fh
        db  0AFh, 0C1h, 003h, 0C3h, 03Bh, 0C6h, 074h, 00Ah, 068h, 008h, 073h, 000h, 000h, 0E9h, 0E1h, 000h
        db  000h, 000h, 08Bh, 043h, 0E8h, 0B9h, 00Ch, 000h, 000h, 000h, 083h, 0E8h, 01Ch, 02Bh, 0D2h, 0F7h
        db  0F1h, 03Bh, 0C5h, 073h, 00Ah, 068h, 01Ch, 073h, 000h, 000h, 0E9h, 0C4h, 000h, 000h, 000h, 06Ah
        db  00Ch, 056h, 0E8h, 053h, 0FBh, 0FFh, 0FFh, 00Bh, 0C0h, 074h, 00Ah, 068h, 040h, 073h, 000h, 000h
        db  0E9h, 0AEh, 000h, 000h, 000h, 055h, 068h, 050h, 073h, 000h, 000h, 0E8h, 009h, 0FBh, 0FFh, 0FFh
        db  083h, 0C4h, 008h, 08Bh, 02Eh, 03Bh, 0EBh, 072h, 044h, 08Bh, 043h, 0E8h, 0B9h, 00Ch, 000h, 000h
        db  000h, 083h, 0E8h, 01Ch, 02Bh, 0D2h, 0F7h, 0F1h, 00Fh, 0AFh, 0C1h, 003h, 0C3h, 03Bh, 0C5h, 076h
        db  02Ch, 055h, 068h, 05Ch, 073h, 000h, 000h, 0E8h, 0DDh, 0FAh, 0FFh, 0FFh, 08Ah, 05Ch, 024h, 024h
        db  083h, 0C4h, 008h, 080h, 0FBh, 042h, 075h, 007h, 068h, 074h, 073h, 000h, 000h, 0EBh, 064h, 080h
        db  0FBh, 046h, 00Fh, 085h
loc_5_0810:
        push   1
        add    byte ptr [eax], al
        jmp    NEAR PTR loc_5_0982
        db  08Bh, 04Eh, 008h, 03Bh, 0D9h, 077h, 068h, 08Bh
loc_5_0821:
        inc    ebx
        db  0E8h, 0BDh, 00Ch, 000h, 000h
        add    byte ptr [ebx - 2dd4e318h], al
        div    ebp
        imul   eax, ebp
        add    eax, ebx
        cmp    eax, ecx
        jbe    SHORT loc_5_0888
        cmp    dword ptr [esi], 0
        jne    SHORT loc_5_0844
        db  068h, 088h, 073h, 000h, 000h
        jmp    SHORT loc_5_0849
loc_5_0844:
        db  068h, 094h, 073h, 000h, 000h
loc_5_0849:
        call   sub_5_02D5
        add    esp, 4
        push   dword ptr [esi + 8]
        db  068h, 0A0h, 073h, 000h, 000h
        call   sub_5_02D5
        mov    bl, byte ptr [esp + 24h]
        add    esp, 8
        cmp    bl, 42h
        jne    SHORT loc_5_087E
        db  068h, 0B4h, 073h, 000h, 000h
        call   sub_5_02D5
        add    esp, 4
        sub    ebp, ebp
        jmp    NEAR PTR loc_5_098A
loc_5_087E:
        cmp    bl, 46h
        jne    SHORT loc_5_08DF
        mov    ebp, dword ptr [esi + 8]
        jmp    SHORT loc_5_08E3
loc_5_0888:
        cmp    dword ptr [esi], 0
        jne    SHORT loc_5_089C
        db  068h, 0C8h, 073h, 000h, 000h
        call   sub_5_02D5
        add    esp, 4
        jmp    SHORT loc_5_08AE
loc_5_089C:
        db  068h, 0D4h, 073h, 000h, 000h
        call   sub_5_02D5
        add    esp, 4
        mov    edi, 1
loc_5_08AE:
        movzx  eax, word ptr [esi + 0ah]
        push   eax
        movzx  eax, word ptr [esi + 8]
        push   eax
        db  068h, 0E0h, 073h, 000h, 000h
        call   sub_5_02D5
        mov    bl, byte ptr [esp + 28h]
        add    esp, 0ch
        cmp    bl, 42h
        jne    SHORT loc_5_08D4
        movzx  ebp, word ptr [esi + 0ah]
        jmp    SHORT loc_5_08E3
loc_5_08D4:
        cmp    bl, 46h
        jne    SHORT loc_5_08DF
        movzx  ebp, word ptr [esi + 8]
        jmp    SHORT loc_5_08E3
loc_5_08DF:
        mov    ebp, dword ptr [esp + 10h]
loc_5_08E3:
        push   dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        shr    eax, 0ch
        push   eax
        db  068h, 0FCh, 073h, 000h, 000h
        call   sub_5_02D5
        add    esp, 0ch
        or     edi, edi
        je     NEAR PTR loc_5_0982
        test   byte ptr [esi + 4], 10h
        je     SHORT loc_5_090E
        db  068h, 014h, 074h, 000h, 000h
        jmp    SHORT loc_5_0913
loc_5_090E:
        db  068h, 018h, 074h, 000h, 000h
loc_5_0913:
        call   sub_5_02D5
        add    esp, 4
        test   byte ptr [esi + 4], 8
        je     SHORT loc_5_0928
        db  068h, 01Ch, 074h, 000h, 000h
        jmp    SHORT loc_5_092D
loc_5_0928:
        db  068h, 020h, 074h, 000h, 000h
loc_5_092D:
        call   sub_5_02D5
        add    esp, 4
        test   byte ptr [esi + 4], 2
        je     SHORT loc_5_0942
        db  068h, 024h, 074h, 000h, 000h
        jmp    SHORT loc_5_0947
loc_5_0942:
        db  068h, 028h, 074h, 000h, 000h
loc_5_0947:
        call   sub_5_02D5
        add    esp, 4
        test   byte ptr [esi + 4], 1
        je     SHORT loc_5_095C
        db  068h, 02Ch, 074h, 000h, 000h
        jmp    SHORT loc_5_0961
loc_5_095C:
        db  068h, 030h, 074h, 000h, 000h
loc_5_0961:
        call   sub_5_02D5
        add    esp, 4
        db  068h, 034h, 074h, 000h, 000h
        call   sub_5_02D5
        add    esp, 4
        push   esi
        call   sub_5_06E0
        jmp    SHORT loc_5_0982
        db  08Bh, 06Ch, 024h, 010h
loc_5_0982:
        cmp    bl, 20h
        jne    SHORT loc_5_098A
        lea    ebp, [esi + 0ch]
loc_5_098A:
        db  068h, 038h, 074h, 000h, 000h
        call   sub_5_02D5
        add    esp, 4
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
        db  083h, 0ECh, 00Ch, 053h, 056h, 057h, 055h, 0C7h, 044h, 024h, 010h, 0F8h, 0FCh, 000h, 000h, 08Bh
        db  074h, 024h, 020h, 081h, 0FEh, 000h, 000h, 00Ch, 000h, 073h, 01Ch, 0A1h, 03Ch, 0F8h, 000h, 000h
        db  081h, 0FEh, 0FFh, 0FFh, 007h, 000h, 08Bh, 040h, 008h, 089h, 044h, 024h, 010h, 076h, 008h, 0C7h
        db  044h, 024h, 010h, 08Ch, 0F8h, 000h, 000h, 081h, 07Ch, 024h, 010h, 0ADh, 0FBh, 0FFh, 0FFh, 075h
        db  012h, 068h, 03Ch, 074h, 000h, 000h, 0E8h, 0E7h, 0F8h, 0FFh, 0FFh, 083h, 0C4h, 004h, 0E9h, 04Fh
        db  001h, 000h, 000h, 08Bh, 03Dh, 0ECh, 0FAh, 000h, 000h, 0B9h, 00Ch, 000h, 000h, 000h, 08Bh, 047h
        db  0E8h, 083h, 0E8h, 01Ch, 02Bh, 0D2h, 0F7h, 0F1h, 00Fh, 0AFh, 0C1h, 050h, 057h, 0E8h, 0F1h, 0F8h
        db  0FFh, 0FFh, 00Bh, 0C0h, 074h, 012h, 068h, 07Ch, 074h, 000h, 000h, 0E8h, 0B2h, 0F8h, 0FFh, 0FFh
        db  083h, 0C4h, 004h, 0E9h, 01Ah, 001h, 000h, 000h, 08Bh, 044h, 024h, 010h, 0B9h, 0E8h, 003h, 000h
        db  000h, 08Bh, 040h, 008h, 089h, 044h, 024h, 018h, 08Bh, 0D8h, 08Bh, 047h, 0E8h, 0BDh, 00Ch, 000h
        db  000h, 000h, 083h, 0E8h, 01Ch, 02Bh, 0D2h, 0F7h, 0F5h, 089h, 044h, 024h, 014h, 00Fh, 0AFh, 0C5h
        db  08Dh, 014h, 007h, 03Bh, 0DFh, 00Fh, 082h, 0D9h, 000h, 000h, 000h, 03Bh, 0D3h, 00Fh, 086h, 0D1h
        db  000h, 000h, 000h, 08Bh, 0C3h, 08Bh, 05Bh, 008h, 03Bh, 0DFh, 00Fh, 082h, 0B0h, 000h, 000h, 000h
        db  03Bh, 0D3h, 00Fh, 086h, 0A8h, 000h, 000h, 000h, 08Bh, 0E9h, 049h, 00Bh, 0EDh, 074h, 068h, 08Bh
        db  06Bh, 004h, 0C1h, 0EDh, 00Ch, 03Bh, 0EEh, 076h, 0DAh, 03Bh, 044h, 024h, 018h, 074h, 010h, 08Bh
        db  048h, 004h, 0C1h, 0E9h, 00Ch, 003h, 008h, 03Bh, 0CEh, 00Fh, 087h, 0A5h, 000h, 000h, 000h, 08Bh
        db  044h, 024h, 010h, 0B9h, 0E8h, 003h, 000h, 000h, 08Bh, 058h, 00Ch, 08Bh, 0C3h, 03Bh, 0C7h, 072h
        db  060h, 03Bh, 0D0h, 076h, 05Ch, 066h, 08Bh, 050h, 008h, 066h, 039h, 054h, 024h, 014h, 076h, 032h
        db  00Fh, 0B7h, 0C2h, 06Bh, 0C0h, 00Ch, 003h, 0C7h, 08Bh, 050h, 004h, 0C1h, 0EAh, 00Ch, 03Bh, 0D6h
        db  077h, 008h, 08Bh, 028h, 003h, 0EAh, 03Bh, 0EEh, 077h, 06Ah, 08Bh, 0D1h, 049h, 00Bh, 0D2h, 074h
        db  025h, 03Bh, 0C3h, 075h, 0D0h, 0EBh, 05Bh, 0FFh, 074h, 024h, 010h, 068h, 0F0h, 074h, 000h, 000h
        db  0EBh, 048h, 0FFh, 074h, 024h, 010h, 050h, 068h, 040h, 075h, 000h, 000h, 0E8h, 0D1h, 0F7h, 0FFh
        db  0FFh, 083h, 0C4h, 00Ch, 0EBh, 03Ch, 0FFh, 074h, 024h, 010h, 068h, 06Ch, 075h, 000h, 000h, 0EBh
        db  029h, 068h, 01Ch, 075h, 000h, 000h, 0E8h, 0B7h, 0F7h, 0FFh, 0FFh, 083h, 0C4h, 004h, 0EBh, 022h
        db  0FFh, 074h, 024h, 010h, 050h, 068h, 0C4h, 074h, 000h, 000h, 0E8h, 0A3h, 0F7h, 0FFh, 0FFh, 083h
        db  0C4h, 00Ch, 0EBh, 00Eh, 053h, 068h, 0A0h, 074h, 000h, 000h, 0E8h, 093h, 0F7h, 0FFh, 0FFh, 083h
        db  0C4h, 008h, 02Bh, 0C0h, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 00Ch, 0C2h, 004h, 000h, 083h, 0ECh
        db  004h, 053h, 056h, 057h, 08Bh, 074h, 024h, 014h, 081h, 0FEh, 0EBh, 0FDh, 0FFh, 0FFh, 075h, 002h
        db  02Bh, 0F6h, 081h, 0FEh, 000h, 000h, 000h, 0C0h, 073h, 00Dh, 06Bh, 0F6h, 00Ah, 08Bh, 03Dh, 0ACh
        db  0F8h, 000h, 000h, 003h, 0F7h, 0EBh, 006h, 08Bh, 03Dh, 0ACh, 0F8h, 000h, 000h, 056h, 068h, 098h
        db  075h, 000h, 000h, 0E8h, 04Ah, 0F7h, 0FFh, 0FFh, 083h, 0C4h, 008h, 08Bh, 0C6h, 02Bh, 0C7h, 0B9h
        db  00Ah, 000h, 000h, 000h, 02Bh, 0D2h, 0F7h, 0F1h, 08Bh, 0D8h, 00Fh, 0AFh, 0C1h, 003h, 0C7h, 03Bh
        db  0C6h, 074h, 012h, 068h, 0A4h, 075h, 000h, 000h, 0E8h, 025h, 0F7h, 0FFh, 0FFh, 083h, 0C4h, 004h
        db  0E9h, 025h, 001h, 000h, 000h, 08Bh, 047h, 0E8h, 0B9h, 00Ah, 000h, 000h, 000h, 083h, 0E8h, 01Ch
        db  02Bh, 0D2h, 0F7h, 0F1h, 03Bh, 0C3h, 073h, 012h, 068h, 0B8h, 075h, 000h, 000h, 0E8h, 000h, 0F7h
        db  0FFh, 0FFh, 083h, 0C4h, 004h, 0E9h, 000h, 001h, 000h, 000h, 06Ah, 00Ah, 056h, 0E8h, 021h, 0F7h
        db  0FFh, 0FFh, 00Bh, 0C0h, 074h, 012h, 068h, 0DCh, 075h, 000h, 000h, 0E8h, 0E2h, 0F6h, 0FFh, 0FFh
        db  083h, 0C4h, 004h, 0E9h, 0E2h, 000h, 000h, 000h, 053h, 068h, 0ECh, 075h, 000h, 000h, 0E8h, 0CFh
        db  0F6h, 0FFh, 0FFh, 083h, 0C4h, 008h, 08Bh, 01Eh, 00Bh, 0DBh, 00Fh, 084h, 08Ch, 000h, 000h, 000h
        db  03Bh, 0DFh, 072h, 018h, 08Bh, 047h, 0E8h, 0B9h, 00Ah, 000h, 000h, 000h, 083h, 0E8h, 01Ch, 02Bh
        db  0D2h, 0F7h, 0F1h, 00Fh, 0AFh, 0C1h, 003h, 0C7h, 03Bh, 0C3h, 077h, 070h, 0F6h, 046h, 003h, 010h
        db  074h, 007h, 0B8h, 00Ch, 076h, 000h, 000h, 0EBh, 010h, 0B8h, 010h, 076h, 000h, 000h, 0F6h, 046h
        db  003h, 020h, 075h, 005h, 0B8h, 014h, 076h, 000h, 000h, 0FFh, 076h, 006h, 050h, 00Fh, 0B7h, 046h
        db  004h, 050h, 00Fh, 0B7h, 006h, 02Bh, 0FFh, 050h, 00Fh, 0B6h, 046h, 002h, 050h, 068h, 018h, 076h
        db  000h, 000h, 0E8h, 06Bh, 0F6h, 0FFh, 0FFh, 083h, 0C4h, 018h, 0B3h, 001h, 084h, 05Eh, 003h, 0B8h
        db  02Eh, 000h, 000h, 000h, 074h, 007h, 00Fh, 0BEh, 087h, 070h, 071h, 000h, 000h, 050h, 068h, 044h
        db  076h, 000h, 000h, 0E8h, 04Ah, 0F6h, 0FFh, 0FFh, 083h, 0C4h, 008h, 002h, 0DBh, 047h, 00Ah, 0DBh
        db  075h, 0DAh, 08Ah, 044h, 024h, 018h, 03Ch, 046h, 074h, 02Fh, 0EBh, 01Ch, 053h, 068h, 0F8h, 075h
        db  000h, 000h, 0E8h, 02Bh, 0F6h, 0FFh, 0FFh, 08Ah, 044h, 024h, 020h, 083h, 0C4h, 008h, 03Ch, 046h
        db  075h, 006h, 00Bh, 0DBh, 074h, 024h, 0EBh, 004h, 08Bh, 05Ch, 024h, 00Ch, 03Ch, 020h, 075h, 005h
        db  08Dh, 05Eh, 00Ah, 0EBh, 017h, 03Ch, 042h, 075h, 013h, 00Fh, 0B6h, 0C0h, 050h, 068h, 048h, 076h
        db  000h, 000h, 0E8h, 0FBh, 0F5h, 0FFh, 0FFh, 083h, 0C4h, 008h, 02Bh, 0DBh, 068h, 05Ch, 076h, 000h
        db  000h, 0E8h, 0ECh, 0F5h, 0FFh, 0FFh, 083h, 0C4h, 004h, 08Bh, 0C3h, 05Fh, 05Eh, 05Bh, 083h, 0C4h
        db  004h, 0C2h, 008h, 000h, 056h, 057h, 08Bh, 074h, 024h, 00Ch, 08Bh, 0FEh, 081h, 0FFh, 000h, 000h
        db  000h, 0C0h, 073h, 02Fh, 00Bh, 0FFh, 074h, 015h, 083h, 0FFh, 020h, 073h, 010h, 08Bh, 034h, 0BDh
        db  008h, 0FCh, 000h, 000h, 00Bh, 0F6h, 0B8h, 001h, 000h, 000h, 000h, 075h, 002h, 02Bh, 0C0h, 00Bh
        db  0C0h, 075h, 029h, 057h, 068h, 09Ch, 077h, 000h, 000h, 0E8h, 0A4h, 0F5h, 0FFh, 0FFh, 083h, 0C4h
        db  008h, 0EBh, 05Ah, 0BFh, 001h, 000h, 000h, 000h, 0B8h, 00Ch, 0FCh, 000h, 000h, 039h, 030h, 074h
        db  00Bh, 083h, 0C0h, 004h, 047h, 03Dh, 088h, 0FCh, 000h, 000h, 072h, 0F1h, 056h, 068h, 0B8h, 077h
        db  000h, 000h, 0E8h, 07Bh, 0F5h, 0FFh, 0FFh, 083h, 0C4h, 008h, 083h, 0FFh, 020h, 075h, 007h, 068h
        db  0C8h, 077h, 000h, 000h, 0EBh, 01Fh, 057h, 068h, 0D4h, 077h, 000h, 000h, 0E8h, 061h, 0F5h, 0FFh
        db  0FFh, 083h, 0C4h, 008h, 06Ah, 020h, 056h, 0E8h, 087h, 0F5h, 0FFh, 0FFh, 00Bh, 0C0h, 074h, 011h
        db  068h, 0DCh, 077h, 000h, 000h, 0E8h, 048h, 0F5h, 0FFh, 0FFh, 083h, 0C4h, 004h, 02Bh, 0F6h, 0EBh
        db  02Ch, 083h, 0FFh, 00Ch, 077h, 01Ah, 00Bh, 0FFh, 074h, 016h, 0FFh, 034h, 0BDh, 068h, 077h, 000h
        db  000h, 068h, 0F0h, 077h, 000h, 000h, 0E8h, 027h, 0F5h, 0FFh, 0FFh, 083h, 0C4h, 008h, 0EBh, 00Dh
        db  068h, 0F4h, 077h, 000h, 000h, 0E8h, 018h, 0F5h, 0FFh, 0FFh, 083h, 0C4h, 004h, 068h, 00Ch, 078h
        db  000h, 000h, 0E8h, 00Bh, 0F5h, 0FFh, 0FFh, 083h, 0C4h, 004h, 08Bh, 0C6h, 05Fh, 05Eh, 0C2h, 004h
        db  000h, 083h, 0ECh, 004h, 056h, 057h, 08Bh, 044h, 024h, 010h, 03Dh, 0EBh, 0FDh, 0FFh, 0FFh, 075h
        db  005h, 0B8h, 001h, 000h, 000h, 000h, 08Ah, 04Ch, 024h, 014h, 080h, 0F9h, 020h, 074h, 018h, 00Fh
        db  0B6h, 0C1h, 050h, 068h, 010h, 078h, 000h, 000h, 0E8h, 0D5h, 0F4h, 0FFh, 0FFh, 083h, 0C4h, 008h
        db  02Bh, 0FFh, 0E9h, 001h, 001h, 000h, 000h, 050h, 0E8h, 0E7h, 0FEh, 0FFh, 0FFh, 08Bh, 0F0h, 00Bh
        db  0F6h, 00Fh, 084h, 0EDh, 000h, 000h, 000h, 0FFh, 036h, 0FFh, 036h, 068h, 024h, 078h, 000h, 000h
        db  0E8h, 0ADh, 0F4h, 0FFh, 0FFh, 083h, 0C4h, 00Ch, 0FFh, 076h, 004h, 0FFh, 076h, 004h, 068h, 040h
        db  078h, 000h, 000h, 0E8h, 09Ah, 0F4h, 0FFh, 0FFh, 083h, 0C4h, 00Ch, 0FFh, 076h, 008h, 0FFh, 076h
        db  008h, 068h, 05Ch, 078h, 000h, 000h, 0E8h, 087h, 0F4h, 0FFh, 0FFh, 083h, 0C4h, 00Ch, 0FFh, 076h
        db  00Ch, 0FFh, 076h, 00Ch, 068h, 078h, 078h, 000h, 000h, 0E8h, 074h, 0F4h, 0FFh, 0FFh, 083h, 0C4h
        db  00Ch, 0FFh, 076h, 010h, 0FFh, 076h, 010h, 068h, 094h, 078h, 000h, 000h, 0E8h, 061h, 0F4h, 0FFh
        db  0FFh, 083h, 0C4h, 00Ch, 0FFh, 076h, 014h, 0FFh, 076h, 014h, 068h, 0B0h, 078h, 000h, 000h, 0E8h
        db  04Eh, 0F4h, 0FFh, 0FFh, 083h, 0C4h, 00Ch, 0FFh, 076h, 018h, 0FFh, 076h, 018h, 068h, 0CCh, 078h
        db  000h, 000h, 0E8h, 03Bh, 0F4h, 0FFh, 0FFh, 083h, 0C4h, 00Ch, 068h, 0E8h, 078h, 000h, 000h, 0E8h
        db  02Eh, 0F4h, 0FFh, 0FFh, 083h, 0C4h, 004h, 0F6h, 046h, 01Ch, 001h, 074h, 007h, 068h, 0F4h, 078h
        db  000h, 000h, 0EBh, 005h, 068h, 004h, 079h, 000h, 000h, 0E8h, 014h, 0F4h, 0FFh, 0FFh, 083h, 0C4h
        db  004h, 0F6h, 046h, 01Ch, 002h, 074h, 00Dh, 068h, 010h, 079h, 000h, 000h, 0E8h, 001h, 0F4h, 0FFh
        db  0FFh, 083h, 0C4h, 004h, 0F7h, 046h, 01Ch, 0FCh, 0FFh, 0FFh, 0FFh, 074h, 007h, 068h, 01Ch, 079h
        db  000h, 000h, 0EBh, 005h, 068h, 034h, 079h, 000h, 000h, 0E8h, 0E4h, 0F3h, 0FFh, 0FFh, 083h, 0C4h
        db  004h, 0BFh, 001h, 000h, 000h, 000h, 08Bh, 0C7h, 047h, 039h, 034h, 085h, 008h, 0FCh, 000h, 000h
        db  075h, 0F4h, 0EBh
loc_5_0F06:
        add    al, 8bh
        db  07Ch, 024h
        db  008h, 068h, 038h
        jns    SHORT loc_5_0F0F
loc_5_0F0F:
        add    al, ch
        sal    bl, 0ffh
        inc    dword ptr [ebx - 3874fb3ch]
        pop    edi
        pop    esi
        add    esp, 4
        ret    8
        db  083h, 0ECh, 004h, 053h, 056h, 057h, 055h, 08Bh, 07Ch, 024h, 018h, 081h, 0FFh, 0EBh, 0FDh, 0FFh
        db  0FFh, 075h, 006h, 08Bh, 03Dh, 06Ch, 0FAh, 000h, 000h, 081h, 0FFh, 000h, 000h, 000h, 0C0h, 073h
        db  00Dh, 06Bh, 0FFh, 00Dh, 08Bh, 01Dh, 0E4h, 0F7h, 000h, 000h, 003h, 0FBh, 0EBh, 006h, 08Bh, 01Dh
        db  0E4h, 0F7h, 000h, 000h, 057h, 068h, 03Ch, 079h, 000h, 000h, 0E8h, 074h, 0F3h, 0FFh, 0FFh, 083h
        db  0C4h, 008h, 08Bh, 0C7h, 02Bh, 0C3h, 0B9h, 00Dh, 000h, 000h, 000h, 02Bh, 0D2h, 0F7h, 0F1h, 08Bh
        db  0F0h, 00Fh, 0AFh, 0C1h, 003h, 0C3h, 03Bh, 0C7h, 074h, 012h, 068h, 048h, 079h, 000h, 000h, 0E8h
        db  04Fh, 0F3h, 0FFh, 0FFh, 083h, 0C4h, 004h, 0E9h, 0AEh, 001h, 000h, 000h, 039h, 035h, 0DCh, 0F7h
        db  000h, 000h, 074h, 037h, 039h, 035h, 0A0h, 0FDh, 000h, 000h, 074h, 02Fh, 0B9h, 06Ch, 0FAh, 000h
        db  000h, 0A1h, 020h, 0EFh, 000h, 000h, 039h, 031h, 077h, 005h, 039h, 071h, 004h, 073h, 01Ch, 039h
        db  041h, 004h, 074h, 005h, 083h, 0C1h, 008h, 0EBh, 0EDh, 068h, 05Ch, 079h, 000h, 000h, 0E8h, 010h
        db  0F3h, 0FFh, 0FFh, 083h, 0C4h, 004h, 0E9h, 06Fh, 001h, 000h, 000h, 06Ah, 00Dh, 057h, 0E8h, 031h
        db  0F3h, 0FFh, 0FFh, 00Bh, 0C0h, 074h, 012h, 068h, 084h, 079h, 000h, 000h, 0E8h, 0F2h, 0F2h, 0FFh
        db  0FFh, 083h, 0C4h, 004h, 0E9h, 051h, 001h, 000h, 000h, 056h, 068h, 0B4h, 079h, 000h, 000h, 0E8h
        db  0DFh, 0F2h, 0FFh, 0FFh, 083h, 0C4h, 008h, 083h, 03Fh, 000h, 075h, 062h, 039h, 035h, 0A0h, 0FDh
        db  000h, 000h, 075h, 044h, 068h, 0C4h, 079h, 000h, 000h
loc_5_100B:
        call   sub_5_02D5
        add    esp, 4
loc_5_1013:
        mov    eax, dword ptr [edi + 8]
        and    eax, 0fffffh
        push   eax
        mov    eax, dword ptr [edi + 9]
        shr    eax, 0ch
        push   eax
        db  068h, 094h, 07Ah, 000h, 000h
        call   sub_5_02D5
        mov    al, byte ptr [esp + 28h]
        add    esp, 0ch
        cmp    al, 42h
        jne    NEAR PTR loc_5_1115
        mov    edi, dword ptr [edi + 8]
        and    edi, 0fffffh
        jmp    NEAR PTR loc_5_1125
        db  039h, 035h, 0DCh, 0F7h, 000h, 000h, 075h, 007h, 068h, 0E8h, 079h, 000h, 000h, 0EBh, 0B2h, 068h
        db  00Ch, 07Ah, 000h, 000h
Get_Profile_Decimal_Int:
        jmp    SHORT loc_5_100B
        db  08Bh, 01Fh, 081h, 0FBh, 055h, 0F1h, 0FFh, 0FFh, 072h, 00Fh, 068h, 030h, 07Ah, 000h, 000h, 0E8h
        db  061h, 0F2h, 0FFh, 0FFh, 083h, 0C4h
Convert_Decimal_String:
        add    al, 0ebh
        db  072h, 06Ah
        or     dl, byte ptr [ebx - 18h]
        test   edx, esi
        db  0FFh, 0FFh, 00Bh, 0C0h, 00Fh, 085h, 0A5h, 000h, 000h, 000h, 08Bh, 00Dh, 0ACh, 0F8h, 000h, 000h
        db  03Bh, 0D9h, 00Fh, 082h, 097h, 000h, 000h, 000h, 08Bh, 041h, 0E8h, 0BDh, 00Ah, 000h, 000h, 000h
        db  083h, 0E8h, 01Ch, 02Bh, 0D2h, 0F7h, 0F5h, 00Fh, 0AFh
Get_Profile_Fixed_Point:
        lds    eax, ptr [ebx]
        sar    dword ptr [ebx], 0c3h
        jbe    SHORT loc_5_112E
        push   dword ptr [edi + 4]
        mov    al, byte ptr [ebx + 3]
        and    al, 40h
        cmp    al, 1
        push   ebx
        sbb    eax, eax
        and    eax, 0ffffffdch
        db  083h
Convert_Fixed_Point_String:
        db  0C0h, 044h, 050h, 0B8h, 068h
        jp     SHORT loc_5_10C7
loc_5_10C7:
        add    dh, dh
        inc    ebx
        add    ecx, dword ptr [eax]
        jne    SHORT loc_5_10D3
        db  0B8h, 070h, 07Ah, 000h, 000h
loc_5_10D3:
        push   eax
        db  068h, 078h, 07Ah, 000h, 000h
        call   sub_5_02D5
        add    esp, 14h
        test   byte ptr [ebx + 3], 8
        jne    NEAR PTR loc_5_1013
        movzx  eax, word ptr [edi + 0ah]
        push   eax
        movzx  eax, word ptr [edi + 8]
        push   eax
        db  068h, 0A8h, 07Ah, 000h, 000h
        call   sub_5_02D5
        mov    al, byte ptr [esp + 28h]
        add    esp, 0ch
        cmp    al, 20h
        je     SHORT loc_5_1121
        movzx  eax, al
        push   eax
        db  068h, 0C0h
Get_Profile_Hex_Int:
        jp     SHORT loc_5_1112
loc_5_1112:
        add    bl, ch
        pop    ds
loc_5_1115:
        cmp    al, 46h
        jne    SHORT loc_5_1121
        mov    edi, dword ptr [edi + 9]
        shr    edi, 0ch
        jmp    SHORT loc_5_1125
loc_5_1121:
        mov    edi, dword ptr [esp + 10h]
loc_5_1125:
        cmp    al, 20h
        jne    SHORT loc_5_113E
        db  08Dh
Convert_Hex_String:
        db  07Eh, 001h
        jmp    SHORT loc_5_113E
loc_5_112E:
        push   ebx
        db  068h, 054h, 07Ah, 000h, 000h
        call   sub_5_02D5
        add    esp, 8
        sub    edi, edi
loc_5_113E:
        db  068h, 0D4h, 07Ah, 000h, 000h
        call   sub_5_02D5
        add    esp, 4
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
        db  056h, 08Bh, 074h, 024h, 008h, 081h
Get_Profile_Boolean:
        db  0FEh, 0EBh, 0FDh, 0FFh, 0FFh, 075h, 006h, 08Bh, 035h, 03Ch, 0F8h, 000h, 000h, 080h, 07Ch, 024h
        db  00Ch, 042h, 075h, 007h, 068h, 0D8h, 07Ah, 000h
Convert_Boolean_String:
        add    bl, ch
        dec    ebp
        push   esi
        db  068h, 0ECh, 07Ah, 000h, 000h
        call   sub_5_02D5
        add    esp, 8
        push   14h
        push   esi
        call   sub_5_0306
        or     eax, eax
        db  075h, 02Eh
        test   esi, 3
        db  075h, 026h
        cmp    dword ptr [esi + 10h], 44434443h
        db  075h, 01Dh
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   dword ptr [esi]
        db  068h, 00Ch, 07Bh, 000h, 000h
        call   sub_5_02D5
        add    esp, 14h
        mov    esi, dword ptr [esi + 0ch]
        jmp    SHORT loc_5_11CF
        db  068h, 0F8h, 07Ah
Get_Profile_String:
        add    byte ptr [eax], al
        call   sub_5_02D5
        add    esp, 4
        sub    esi, esi
loc_5_11CF:
        db  068h, 038h, 07Bh, 000h, 000h
        call   sub_5_02D5
        add    esp, 4
        mov    eax, esi
        pop    esi
        ret    8
        db  056h, 08Bh, 074h, 024h, 008h, 081h, 0FEh, 0EBh, 0FDh, 0FFh, 0FFh, 075h, 005h, 0BEh, 0E0h, 0FCh
        db  000h, 000h, 080h, 07Ch, 024h, 00Ch, 042h, 075h, 007h, 068h, 03Ch, 07Bh, 000h, 000h, 0EBh, 04Ch
        db  056h, 068h, 050h, 07Bh, 000h, 000h, 0E8h, 0C8h, 0F0h, 0FFh, 0FFh, 083h, 0C4h, 008h, 06Ah, 010h
Get_Next_Profile_String:
        push   esi
        call   sub_5_0306
        or     eax, eax
        jne    SHORT loc_5_1249
        cmp    esi, 0c0000000h
        jb     SHORT loc_5_1249
        test   esi, 3
        jne    SHORT loc_5_1249
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   dword ptr [esi]
        db  068h, 070h, 07Bh, 000h, 000h
        call   sub_5_02D5
        add    esp, 14h
        mov    esi, dword ptr [esi + 0ch]
        jmp    SHORT loc_5_1258
loc_5_1249:
        db  068h, 05Ch, 07Bh, 000h, 000h
        call   sub_5_02D5
        add    esp, 4
        sub    esi, esi
loc_5_1258:
        db  068h, 0A8h, 07Bh, 000h, 000h
        call   sub_5_02D5
        add    esp, 4
        mov    eax, esi
        pop    esi
Get_Name_Of_Ugly_TSR:
        ret    8
        db  056h, 08Bh, 074h, 024h, 008h, 081h, 0FEh, 0EBh, 0FDh, 0FFh, 0FFh, 075h, 005h, 0BEh, 06Ch
Get_Next_Arena:
        cli
        add    byte ptr [eax], al
loc_5_127D:
        cmp    byte ptr [esp + 0ch], 42h
        jne    SHORT loc_5_128B
        db  068h, 0ACh, 07Bh, 000h, 000h
        jmp    SHORT loc_5_12E9
loc_5_128B:
        push   esi
        db  068h, 0C0h, 07Bh, 000h, 000h
        call   sub_5_02D5
        add    esp, 8
        push   8
        push   esi
        call   sub_5_0306
        or     eax, eax
        jne    SHORT loc_5_12E4
        db  081h, 0FEh, 06Ch, 0FAh, 000h, 000h
        jb     SHORT loc_5_12E4
        db  0A1h, 06Ch, 0F8h, 000h, 000h
        db  08Dh, 004h, 0C5h, 06Ch, 0FAh, 000h, 000h
        cmp    eax, esi
        jbe    SHORT loc_5_12E4
        push   dword ptr [esi + 4]
        push   dword ptr [esi]
        db  068h, 0E0h, 07Bh, 000h, 000h
        call   sub_5_02D5
        add    esp, 0ch
        add    esi, 8
        db  0A1h, 06Ch, 0F8h, 000h, 000h
        db  08Dh, 004h, 0C5h, 06Ch, 0FAh, 000h, 000h
        cmp    eax, esi
        je     SHORT loc_5_12F1
        jmp    SHORT loc_5_12F3
loc_5_12E4:
        db  068h, 0CCh, 07Bh, 000h, 000h
loc_5_12E9:
        call   sub_5_02D5
        add    esp, 4
loc_5_12F1:
        sub    esi, esi
loc_5_12F3:
        db  068h, 008h, 07Ch, 000h, 000h
        call   sub_5_02D5
        add    esp, 4
        mov    eax, esi
        pop    esi
        ret    8
        db  056h, 057h, 08Bh, 074h, 024h, 00Ch, 081h, 0FEh, 0EBh, 0FDh
GetDOSVectors:
        db  0FFh, 0FFh, 075h, 008h, 0A1h, 03Ch, 0F8h, 000h, 000h, 08Bh, 070h, 008h, 08Ah, 044h, 024h, 010h
        db  03Ch, 020h, 074h, 016h, 00Fh, 0B6h, 0C0h, 050h, 068h, 00Ch, 07Ch, 000h, 000h, 0E8h, 0A3h, 0EFh
        db  0FFh, 0FFh, 083h, 0C4h, 008h, 0E9h, 09Fh, 000h, 000h, 000h, 056h, 068h, 020h, 07Ch, 000h, 000h
        db  0E8h, 090h, 0EFh, 0FFh, 0FFh, 083h, 0C4h, 008h, 06Ah, 016h, 056h, 0E8h, 0B6h, 0EFh, 0FFh, 0FFh
        db  00Bh, 0C0h, 074h, 007h, 068h, 02Ch, 07Ch, 000h, 000h, 0EBh, 076h, 08Bh, 006h, 03Dh, 000h, 004h
        db  000h, 000h, 075h, 012h, 08Dh, 07Eh, 004h, 081h, 03Fh, 0FFh, 0FFh, 007h, 000h, 075h, 007h, 068h
        db  040h, 07Ch, 000h, 000h, 0EBh, 030h, 03Dh, 000h, 000h, 008h, 000h, 075h, 012h, 08Dh, 07Eh, 004h
        db  081h, 03Fh, 0FFh, 0FFh, 00Bh, 000h, 075h, 007h, 068h, 044h, 07Ch, 000h, 000h, 0EBh, 017h, 03Dh
        db  000h, 000h, 00Ch, 000h, 075h, 036h, 08Dh, 07Eh, 004h, 081h, 03Fh, 0FFh, 0FBh, 00Fh, 000h, 075h
        db  02Bh, 068h, 048h, 07Ch, 000h, 000h, 0E8h, 02Ah, 0EFh, 0FFh, 0FFh, 083h, 0C4h, 004h, 00Fh, 0B7h
        db  046h, 014h, 050h, 0FFh, 076h, 00Ch, 0FFh, 076h, 008h, 0FFh, 037h, 0FFh, 036h, 068h, 060h, 07Ch
        db  000h, 000h, 0E8h, 00Eh, 0EFh, 0FFh, 0FFh, 083h
sub_5_13C8:
        les    ebx, ptr [eax]
        jmp    SHORT loc_5_13DB
        db  068h, 04Ch, 07Ch, 000h, 000h, 0E8h, 0FFh, 0EEh, 0FFh, 0FFh, 083h, 0C4h, 004h, 02Bh, 0F6h
loc_5_13DB:
        db  068h, 09Ch, 07Ch, 000h, 000h
        call   sub_5_02D5
        add    esp, 4
        mov    eax, esi
        pop    edi
        pop    esi
        ret    8
        db  083h, 0ECh, 014h, 053h, 056h, 057h, 055h, 08Bh, 074h, 024h, 028h, 08Dh, 07Eh, 072h, 066h, 081h
        db  03Fh, 048h, 049h, 074h, 018h, 068h, 0A0h, 07Ch, 000h, 000h, 0E8h, 0C7h, 0EEh, 0FFh, 0FFh, 083h
        db  0C4h, 004h, 057h, 068h, 0C4h, 07Ch, 000h, 000h, 0E9h, 088h, 001h, 000h, 000h, 08Dh, 046h, 074h
        db  089h, 044h, 024h, 010h, 08Bh, 0CEh
loc_5_1425:
        mov    ebp, dword ptr [esp + 10h]
        mov    ebx, ebp
        mov    eax, dword ptr [ecx]
        add    eax, ecx
        mov    dword ptr [esp + 14h], eax
        mov    eax, ebp
        cmp    ebp, eax
        ja     NEAR PTR loc_5_1591
loc_5_143D:
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [esp + 14h], eax
        jbe    NEAR PTR loc_5_1591
        mov    edx, dword ptr [eax]
        mov    eax, edx
        db  025h
Allocate_PM_App_CB_Area:
        add    eax, dword ptr [eax]
        add    al, dh
        cmp    eax, 0a0000001h
        mov    eax, edx
        jne    NEAR PTR loc_5_1504
        and    eax, 0ffffffch
        mov    edi, dword ptr [esp + 10h]
        add    edi, eax
        mov    eax, edi
        and    eax, 0fffh
        cmp    eax, 4
        je     SHORT loc_5_149B
        test   dl, 1
        je     NEAR PTR loc_5_15F2
        mov    eax, dword ptr [edi]
        test   al, 2
        je     NEAR PTR loc_5_15F2
        and    eax, 0f0000001h
        cmp    eax, 0a0000000h
        jne    NEAR PTR loc_5_15F2
loc_5_149B:
        mov    eax, dword ptr [esp + 10h]
        mov    edi, dword ptr [eax + 4]
        mov    eax, dword ptr [edi]
        and    eax, 0f0000003h
        cmp    eax, 0a0000001h
        jne    SHORT loc_5_14D7
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [edi + 8], eax
        jne    SHORT loc_5_14D7
        mov    edi, dword ptr [eax + 8]
        mov    eax, dword ptr [edi]
        and    eax, 0f0000003h
        cmp    eax, 0a0000001h
        jne    SHORT loc_5_14D7
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [edi + 4], eax
        je     NEAR PTR loc_5_1577
loc_5_14D7:
        db  068h, 04Ch, 07Dh, 000h, 000h
        call   sub_5_02D5
        mov    eax, dword ptr [esp + 14h]
        add    esp, 4
        push   dword ptr [eax + 4]
        push   dword ptr [eax + 8]
        push   dword ptr [esp + 18h]
        db  068h, 080h, 07Dh, 000h, 000h
        call   sub_5_02D5
        add    esp, 10h
        jmp    NEAR PTR loc_5_15AC
loc_5_1504:
        and    eax, 0f0000001h
        cmp    eax, 0a0000000h
        jne    NEAR PTR loc_5_186A
        test   dl, 1
        jne    NEAR PTR loc_5_1614
        test   dl, 2
        mov    eax, dword ptr [ebx]
        je     SHORT loc_5_1559
        and    eax, 0f0000003h
        cmp    eax, 0a0000001h
        jne    NEAR PTR loc_5_162F
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [eax - 4], ebx
        je     SHORT loc_5_1577
        db  068h, 0E8h, 07Dh, 000h, 000h
        call   sub_5_02D5
        mov    eax, dword ptr [esp + 14h]
        add    esp, 4
        sub    eax, 4
        push   eax
        db  068h, 01Ch, 07Eh, 000h, 000h
        jmp    SHORT loc_5_15A4
loc_5_1559:
        and    eax, 0f0000003h
        cmp    eax, 0a0000001h
        jne    SHORT loc_5_1577
        mov    eax, dword ptr [esp + 10h]
        and    eax, 0fffh
        cmp    eax, 4
        jne    NEAR PTR loc_5_164A
loc_5_1577:
        and    edx, 0ffffffch
        je     SHORT loc_5_15B8
        mov    ebx, dword ptr [esp + 10h]
        add    dword ptr [esp + 10h], edx
        cmp    ebp, dword ptr [esp + 10h]
        jbe    NEAR PTR loc_5_143D
loc_5_1591:
        db  068h, 0D4h, 07Ch, 000h, 000h
        call   sub_5_02D5
        add    esp, 4
        push   ebx
        db  068h, 0F4h, 07Ch, 000h, 000h
loc_5_15A4:
        call   sub_5_02D5
        add    esp, 8
loc_5_15AC:
        sub    eax, eax
loc_5_15AE:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret    4
loc_5_15B8:
        mov    eax, dword ptr [esp + 14h]
        sub    eax, dword ptr [esp + 10h]
        cmp    eax, 4
        jne    NEAR PTR loc_5_1843
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax]
        and    eax, 0f0000001h
        cmp    eax, 0a0000000h
        jne    NEAR PTR loc_5_1843
        mov    ecx, dword ptr [ecx + 4]
        or     ecx, ecx
        je     SHORT loc_5_1665
        lea    eax, [ecx + 8]
        mov    dword ptr [esp + 10h], eax
        jmp    NEAR PTR loc_5_1425
loc_5_15F2:
        db  068h, 008h, 07Dh, 000h, 000h
        call   sub_5_02D5
        add    esp, 4
        push   edi
        push   dword ptr [esp + 14h]
        push   ebx
        db  068h, 028h, 07Dh, 000h, 000h
        call   sub_5_02D5
        add    esp, 10h
        jmp    SHORT loc_5_15AC
loc_5_1614:
        db  068h, 0ACh, 07Dh, 000h, 000h
        call   sub_5_02D5
        add    esp, 4
        push   dword ptr [esp + 10h]
        db  068h, 0D4h, 07Dh, 000h, 000h
        jmp    NEAR PTR loc_5_15A4
loc_5_162F:
        db  068h, 02Ch, 07Eh, 000h, 000h
        call   sub_5_02D5
        add    esp, 4
        push   dword ptr [esp + 10h]
        db  068h, 04Ch, 07Eh, 000h, 000h
        jmp    NEAR PTR loc_5_15A4
loc_5_164A:
        db  068h, 05Ch, 07Eh, 000h, 000h
        call   sub_5_02D5
        add    esp, 4
        push   dword ptr [esp + 10h]
        db  068h, 080h, 07Eh, 000h, 000h
        jmp    NEAR PTR loc_5_15A4
loc_5_1665:
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [esp + 10h], eax
        mov    ebp, eax
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [esp + 18h], eax
        mov    dword ptr [esp + 1ch], 1
        lea    eax, [esi + 0ch]
        mov    dword ptr [esp + 20h], eax
        cmp    eax, dword ptr [esp + 10h]
        je     NEAR PTR loc_5_1792
        lea    ebx, [esi + 18h]
        mov    edi, 10h
loc_5_1696:
        mov    eax, esi
        or     esi, esi
        je     SHORT loc_5_16B3
        mov    ecx, dword ptr [esp + 10h]
loc_5_16A0:
        cmp    ecx, eax
        jbe    SHORT loc_5_16AC
        mov    edx, dword ptr [eax]
        add    edx, eax
        cmp    edx, ecx
        ja     SHORT loc_5_16CB
loc_5_16AC:
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        jne    SHORT loc_5_16A0
loc_5_16B3:
        db  068h, 00Ch, 07Fh, 000h, 000h
        call   sub_5_02D5
        add    esp, 4
        push   ebp
        db  068h, 048h, 07Fh, 000h, 000h
        jmp    NEAR PTR loc_5_15A4
loc_5_16CB:
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [eax]
        mov    eax, ecx
        and    eax, 0f0000003h
        cmp    eax, 0a0000001h
        jne    NEAR PTR loc_5_17C1
        cmp    dword ptr [esp + 10h], esi
        jb     SHORT loc_5_173C
        lea    eax, [esi + 74h]
        cmp    eax, dword ptr [esp + 10h]
        jbe    SHORT loc_5_173C
        test   ecx, 0ffffffch
        je     SHORT loc_5_1719
        db  068h, 0B4h, 07Fh, 000h, 000h
        call   sub_5_02D5
        add    esp, 4
        push   ebp
        push   dword ptr [esp + 14h]
        db  068h, 0E0h, 07Fh, 000h, 000h
        call   sub_5_02D5
        add    esp, 0ch
loc_5_1719:
        mov    eax, esi
        sub    eax, dword ptr [esp + 10h]
        add    eax, edi
        cmp    eax, -0ch
        jne    NEAR PTR loc_5_17E5
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esp + 18h], eax
        add    ebx, 10h
        add    edi, 10h
        inc    dword ptr [esp + 1ch]
        jmp    SHORT loc_5_1777
loc_5_173C:
        and    ecx, 0ffffffch
        cmp    dword ptr [esp + 18h], ecx
        jb     NEAR PTR loc_5_1815
        mov    eax, dword ptr [ebp]
        and    eax, 0ffffffch
        cmp    eax, ecx
        jbe    SHORT loc_5_1777
        db  068h, 09Ch, 080h, 000h, 000h
        call   sub_5_02D5
        add    esp, 4
        push   ebp
        push   dword ptr [esp + 14h]
        db  068h, 0C8h, 080h, 000h, 000h
        call   sub_5_02D5
        add    esp, 0ch
loc_5_1777:
        mov    ebp, dword ptr [esp + 10h]
        mov    eax, ebp
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [esp + 20h]
        cmp    eax, dword ptr [esp + 10h]
        jne    NEAR PTR loc_5_1696
loc_5_1792:
        cmp    dword ptr [esp + 1ch], 4
        je     NEAR PTR loc_5_1839
        db  068h, 0E8h, 080h, 000h, 000h
        call   sub_5_02D5
        add    esp, 4
        push   dword ptr [esp + 14h]
        push   esi
        db  068h, 010h, 081h, 000h, 000h
        call   sub_5_02D5
        add    esp, 0ch
        jmp    NEAR PTR loc_5_15AC
loc_5_17C1:
        db  068h, 064h, 07Fh, 000h, 000h
        call   sub_5_02D5
        add    esp, 4
        push   ebp
        push   dword ptr [esp + 14h]
        db  068h, 094h, 07Fh, 000h, 000h
        call   sub_5_02D5
        add    esp, 0ch
        jmp    NEAR PTR loc_5_15AC
loc_5_17E5:
        db  068h, 0FCh, 07Fh, 000h, 000h
        call   sub_5_02D5
        mov    eax, dword ptr [esp + 20h]
        add    esp, 4
        shl    eax, 4
        add    eax, esi
        add    eax, 0ch
        push   eax
        push   dword ptr [esp + 14h]
        db  068h, 028h, 080h, 000h, 000h
        call   sub_5_02D5
        add    esp, 0ch
        jmp    NEAR PTR loc_5_15AC
loc_5_1815:
        db  068h, 050h, 080h, 000h, 000h
        call   sub_5_02D5
        add    esp, 4
        push   ebp
        push   dword ptr [esp + 14h]
        db  068h, 07Ch, 080h, 000h, 000h
        call   sub_5_02D5
        add    esp, 0ch
        jmp    NEAR PTR loc_5_15AC
loc_5_1839:
        mov    eax, 1
        jmp    NEAR PTR loc_5_15AE
loc_5_1843:
        db  068h, 0CCh, 07Eh, 000h, 000h
        call   sub_5_02D5
        add    esp, 4
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        db  068h, 0ECh, 07Eh, 000h, 000h
        call   sub_5_02D5
        add    esp, 0ch
        jmp    NEAR PTR loc_5_15AC
loc_5_186A:
        db  068h, 094h, 07Eh, 000h, 000h
        call   sub_5_02D5
        add    esp, 4
        push   dword ptr [esp + 10h]
        db  068h, 0B8h, 07Eh, 000h, 000h
        jmp    NEAR PTR loc_5_15A4
        db  083h, 0ECh, 010h, 053h, 056h, 057h, 055h, 081h, 07Ch, 024h, 024h, 0EBh, 0FDh, 0FFh, 0FFh, 075h
        db  009h, 0A1h, 0E8h, 0F7h, 000h, 000h, 089h, 044h, 024h, 024h, 0F6h, 044h, 024h, 024h, 003h, 074h
        db  00Eh, 0FFh, 074h, 024h, 024h, 068h, 038h, 081h, 000h, 000h, 0E9h, 04Ch, 004h, 000h, 000h, 08Ah
        db  05Ch, 024h, 028h, 080h, 0FBh, 053h, 00Fh, 085h, 01Bh, 001h, 000h, 000h, 06Ah, 074h, 0FFh, 074h
        db  024h, 028h, 0E8h, 03Ah, 0EAh, 0FFh, 0FFh, 00Bh, 0C0h, 00Fh, 085h, 0D1h, 002h, 000h, 000h, 08Bh
        db  044h, 024h, 024h, 089h, 044h, 024h, 010h, 089h, 044h, 024h, 01Ch, 066h, 081h, 078h, 072h, 048h
        db  049h, 074h, 012h, 068h, 050h, 081h, 000h, 000h, 0E8h, 0E3h, 0E9h, 0FFh, 0FFh, 083h, 0C4h, 004h
        db  0E9h, 00Eh, 004h, 000h, 000h, 0C7h, 044h, 024h, 014h, 000h, 000h, 000h, 000h, 02Bh, 0F6h, 02Bh
        db  0FFh, 02Bh, 0C9h, 02Bh, 0D2h, 083h, 044h, 024h, 024h, 074h, 08Bh, 044h, 024h, 024h, 08Bh, 018h
        db  081h, 0E3h, 0FCh, 0FFh, 0FFh, 00Fh, 075h, 016h, 08Bh, 044h, 024h, 010h, 08Bh, 040h, 004h, 00Bh
        db  0C0h, 074h, 078h, 089h, 044h, 024h, 010h, 083h, 0C0h, 008h, 089h, 044h, 024h, 024h, 08Bh, 044h
        db  024h, 024h, 08Bh, 000h, 08Bh, 0E8h, 081h, 0E5h, 001h, 000h, 000h, 0F0h, 081h, 0FDh, 000h, 000h
        db  000h, 0A0h, 075h, 005h, 046h, 003h, 0D3h, 0EBh, 049h, 025h, 003h, 000h, 000h, 0F0h, 03Dh, 001h
        db  000h, 000h, 0A0h, 075h, 070h, 0FFh, 044h, 024h, 014h, 08Bh, 044h, 024h, 024h, 005h, 00Bh, 010h
        db  000h, 000h, 025h, 000h, 0F0h, 0FFh, 0FFh, 08Bh, 06Ch, 024h, 024h, 003h, 0EBh, 083h, 0EDh, 004h
        db  081h, 0E5h, 000h, 0F0h, 0FFh, 0FFh, 03Bh, 0E8h, 077h, 004h, 003h, 0CBh, 0EBh, 014h, 02Bh, 04Ch
        db  024h, 024h, 003h, 0C8h, 02Bh, 0F8h, 003h, 0FDh, 08Bh, 0C3h, 02Bh, 0C5h, 003h, 044h, 024h, 024h
        db  003h, 0C8h, 001h, 05Ch, 024h, 024h, 0E9h, 06Fh, 0FFh, 0FFh, 0FFh, 057h, 051h, 08Dh, 004h, 0B5h
        db  000h, 000h, 000h, 000h, 0FFh, 074h, 024h, 01Ch, 050h, 052h, 056h, 0FFh, 074h, 024h, 034h, 068h
        db  0A0h, 081h, 000h, 000h, 0E8h, 017h, 0E9h, 0FFh, 0FFh, 08Bh, 074h, 024h, 038h, 083h, 0C4h, 020h
        db  0E9h, 040h, 003h, 000h, 000h, 068h, 078h, 081h, 000h, 000h, 0E8h, 001h, 0E9h, 0FFh, 0FFh, 083h
        db  0C4h, 004h, 0E9h, 02Ch, 003h, 000h, 000h, 080h, 0FBh, 058h, 075h, 050h, 06Ah, 074h, 0FFh, 074h
        db  024h, 028h, 0E8h, 01Ah, 0E9h, 0FFh, 0FFh, 00Bh, 0C0h, 00Fh, 085h, 0B1h, 001h, 000h, 000h, 08Bh
        db  044h, 024h, 024h, 066h, 081h, 078h, 072h, 048h, 049h, 074h, 012h, 068h, 040h, 082h, 000h, 000h
        db  0E8h, 0CBh, 0E8h, 0FFh, 0FFh, 083h, 0C4h, 004h, 0E9h, 0F6h, 002h, 000h, 000h, 0FFh, 074h, 024h
        db  024h, 0E8h, 0D4h, 0F9h, 0FFh, 0FFh, 00Bh, 0C0h, 00Fh, 084h, 0E5h, 002h, 000h, 000h, 0FFh, 074h
        db  024h, 024h, 068h, 068h, 082h, 000h, 000h, 0E9h, 0CFh, 002h, 000h, 000h, 080h, 0FBh, 057h, 075h
        db  012h, 068h, 078h, 082h, 000h, 000h, 0E8h, 095h, 0E8h, 0FFh, 0FFh, 083h, 0C4h, 004h, 0E9h, 0C0h
        db  002h, 000h, 000h, 06Ah, 004h, 0FFh, 074h, 024h, 028h, 0E8h, 0B3h, 0E8h, 0FFh, 0FFh, 00Bh, 0C0h
        db  00Fh, 085h, 09Ch, 002h, 000h, 000h, 08Bh, 044h, 024h, 024h, 08Bh, 000h, 025h, 000h, 000h, 000h
        db  0F0h, 03Dh, 000h, 000h, 000h, 0A0h, 00Fh, 084h, 0CDh, 000h, 000h, 000h, 06Ah, 074h, 0FFh, 074h
        db  024h, 028h, 0E8h, 08Ah, 0E8h, 0FFh, 0FFh, 00Bh, 0C0h, 00Fh, 085h, 087h, 000h, 000h, 000h, 08Bh
        db  044h, 024h, 024h, 066h, 081h, 078h, 072h, 048h, 049h, 075h, 07Bh, 083h, 0C0h, 00Ch, 050h, 08Bh
        db  044h, 024h, 028h, 0FFh, 030h, 0FFh, 074h, 024h, 02Ch, 068h, 060h, 083h, 000h, 000h, 0E8h, 02Dh
        db  0E8h, 0FFh, 0FFh, 08Bh, 074h, 024h, 034h, 083h, 0C4h, 010h, 083h, 0C6h, 070h, 0F6h, 006h, 008h
        db  075h, 00Dh, 068h, 08Ch, 083h, 000h, 000h, 0E8h, 014h, 0E8h, 0FFh, 0FFh, 083h, 0C4h, 004h, 0F6h
        db  006h, 080h, 074h, 00Dh, 068h, 094h, 083h, 000h, 000h, 0E8h, 002h, 0E8h, 0FFh, 0FFh, 083h, 0C4h
        db  004h, 080h, 0FBh, 020h, 075h, 00Ch, 08Bh, 074h, 024h, 024h, 083h, 0C6h, 074h, 0E9h, 023h, 002h
        db  000h, 000h, 080h, 0FBh, 046h, 074h, 013h, 080h, 0FBh, 042h, 074h, 00Eh, 00Fh, 0B6h, 0C3h, 050h
        db  068h, 09Ch, 083h, 000h, 000h, 0E9h, 001h, 002h, 000h, 000h, 08Bh, 074h, 024h, 024h, 083h, 0C6h
        db  00Ch, 0E9h, 0FFh, 001h, 000h, 000h, 08Bh, 044h, 024h, 024h, 06Ah, 00Ch, 083h, 0E8h, 00Ch, 050h
        db  0E8h, 0ECh, 0E7h, 0FFh, 0FFh, 00Bh, 0C0h, 00Fh, 085h, 083h, 000h, 000h, 000h, 08Bh, 044h, 024h
        db  024h, 08Bh, 048h, 0F4h, 083h, 0E8h, 00Ch, 081h, 0E1h, 003h, 000h, 000h, 0F0h, 081h, 0F9h, 001h
        db  000h, 000h, 0A0h, 075h, 051h, 089h, 044h, 024h, 024h, 08Bh, 044h, 024h, 024h, 08Bh, 000h, 025h
        db  0FCh, 0FFh, 0FFh, 00Fh, 050h, 0FFh, 074h, 024h, 028h, 068h, 098h, 082h, 000h, 000h, 0E8h, 07Dh
        db  0E7h, 0FFh, 0FFh, 08Bh, 044h, 024h, 030h, 083h, 0C4h, 00Ch, 08Bh, 000h, 08Bh, 0C8h, 081h, 0E1h
        db  001h, 000h, 000h, 0F0h, 081h, 0F9h, 000h, 000h, 000h, 0A0h, 00Fh, 085h, 0C9h, 000h, 000h, 000h
        db  0A9h, 0FCh, 0FFh, 0FFh, 00Fh, 075h, 037h, 068h, 0B0h, 082h, 000h, 000h, 0E8h, 04Fh, 0E7h, 0FFh
        db  0FFh, 083h, 0C4h, 004h, 0EBh, 03Dh, 08Bh, 044h, 024h, 024h, 08Bh, 048h, 0FCh, 083h, 0E8h, 004h
        db  081h, 0E1h, 001h, 000h, 000h, 0F0h, 081h, 0F9h, 000h, 000h, 000h, 0A0h, 075h, 002h, 0EBh, 095h
        db  0FFh, 074h, 024h, 024h, 068h, 0B0h, 083h, 000h, 000h, 0E9h, 04Dh, 001h, 000h, 000h, 08Bh, 044h
        db  024h, 024h, 083h, 0C0h, 004h, 050h, 068h, 0C0h, 082h, 000h, 000h, 0E8h, 010h, 0E7h, 0FFh, 0FFh
        db  083h, 0C4h, 008h, 08Bh, 044h, 024h, 024h, 0F6h, 000h, 002h, 074h, 00Dh, 068h, 0D4h, 082h, 000h
        db  000h, 0E8h, 0FAh, 0E6h, 0FFh, 0FFh, 083h, 0C4h, 004h, 080h, 0FBh, 042h, 08Bh, 044h, 024h, 024h
        db  075h, 040h, 0F6h, 000h, 002h, 074h, 02Dh, 06Ah, 004h, 083h, 0E8h, 004h, 050h, 0E8h, 00Fh, 0E7h
        db  0FFh, 0FFh, 00Bh, 0C0h, 075h, 00Ch, 08Bh, 044h, 024h, 024h, 08Bh, 070h, 0FCh, 0E9h, 003h, 001h
        db  000h, 000h, 068h, 0E0h, 082h, 000h, 000h, 0E8h, 0C4h, 0E6h, 0FFh, 0FFh, 083h, 0C4h, 004h, 0E9h
        db  0EFh, 000h, 000h, 000h, 00Fh, 0B6h, 0C3h, 050h, 068h, 0FCh, 082h, 000h, 000h, 0E9h, 0D9h, 000h
        db  000h, 000h, 08Bh, 030h, 081h, 0E6h, 0FCh, 0FFh, 0FFh, 00Fh, 003h, 0F0h, 03Bh, 0F0h, 00Fh, 085h
        db  0D1h, 000h, 000h, 000h, 0E9h, 0CAh, 000h, 000h, 000h, 06Ah, 00Ch, 0FFh, 074h, 024h, 028h, 0E8h
        db  0BDh, 0E6h, 0FFh, 0FFh, 00Bh, 0C0h, 074h, 00Eh, 0FFh, 074h, 024h, 024h, 068h, 010h, 083h, 000h
        db  000h, 0E9h, 0A5h, 000h, 000h, 000h, 08Bh, 044h, 024h, 024h, 0FFh, 070h, 008h, 0FFh, 070h, 004h
        db  068h, 02Ch, 083h, 000h, 000h, 0E8h, 066h, 0E6h, 0FFh, 0FFh, 08Bh, 044h, 024h, 030h, 083h, 0C4h
        db  00Ch, 0F7h, 000h, 0FCh, 0FFh, 0FFh, 00Fh, 075h, 034h, 068h, 04Ch, 083h, 000h, 000h, 0E8h, 04Dh
        db  0E6h, 0FFh, 0FFh, 083h, 0C4h, 004h, 08Bh, 044h, 024h, 024h, 06Ah, 004h, 083h, 0E8h, 004h, 050h
        db  0E8h, 06Ch, 0E6h, 0FFh, 0FFh, 00Bh, 0C0h, 075h, 014h, 08Bh, 044h, 024h, 024h, 0FFh, 070h, 0FCh
        db  068h, 054h, 083h, 000h, 000h, 0E8h, 026h, 0E6h, 0FFh, 0FFh, 083h, 0C4h, 008h, 080h, 0FBh, 020h
        db  075h, 010h, 08Bh, 044h, 024h, 024h, 08Bh, 030h, 081h, 0E6h, 0FCh, 0FFh, 0FFh, 00Fh, 003h, 0F0h
        db  0EBh, 020h, 080h, 0FBh, 046h, 075h, 009h, 08Bh, 044h, 024h, 024h, 08Bh, 070h, 004h, 0EBh, 012h
        db  080h, 0FBh, 042h, 075h, 009h, 08Bh, 044h, 024h, 024h, 08Bh, 070h, 008h, 0EBh, 004h, 08Bh, 074h
        db  024h, 018h, 0F7h, 006h, 0FCh, 0FFh, 0FFh, 00Fh, 075h, 01Bh, 083h, 07Eh, 0FCh, 020h, 075h, 015h
        db  0EBh, 011h, 0FFh, 074h, 024h, 024h, 068h, 0E4h, 083h, 000h, 000h, 0E8h, 0D0h, 0E5h, 0FFh, 0FFh
        db  083h, 0C4h, 008h, 02Bh, 0F6h, 068h, 000h, 084h, 000h, 000h, 0E8h, 0C1h, 0E5h, 0FFh, 0FFh, 083h
        db  0C4h, 004h, 08Bh, 0C6h, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 010h, 0C2h, 008h, 000h, 056h, 08Bh
        db  074h, 024h, 008h, 06Ah, 00Ah, 056h, 0E8h, 0D6h, 0E5h, 0FFh, 0FFh, 00Bh, 0C0h, 074h, 00Fh, 068h
        db  094h, 084h, 000h, 000h, 0E8h, 097h, 0E5h, 0FFh, 0FFh, 083h, 0C4h, 004h, 0EBh, 029h, 00Fh, 0B6h
        db  046h, 002h, 083h, 0F8h, 00Ch, 077h, 012h, 00Bh, 0C0h, 074h, 00Eh, 0FFh, 034h, 085h, 060h, 084h
        db  000h, 000h, 068h, 09Ch, 084h, 000h, 000h, 0EBh, 006h, 050h, 068h, 0A4h, 084h, 000h, 000h, 0E8h
        db  06Ch, 0E5h, 0FFh, 0FFh, 083h, 0C4h, 008h, 05Eh, 0C2h, 004h, 000h, 053h, 056h, 057h, 055h, 081h
        db  07Ch, 024h, 014h, 0EBh, 0FDh, 0FFh, 0FFh, 075h, 007h, 00Fh, 020h, 0D0h, 089h, 044h, 024h, 014h
        db  02Bh, 0F6h, 08Bh, 07Ch, 024h, 014h, 0C1h, 0EFh, 00Ch, 08Dh, 02Ch, 0BDh, 000h, 000h, 080h, 0FFh
        db  08Bh, 0C7h, 0C1h, 0E0h, 00Ch, 050h, 068h, 0B0h, 084h, 000h, 000h, 0E8h, 030h, 0E5h, 0FFh, 0FFh
        db  08Bh, 0DDh, 081h, 0E3h, 0FFh, 0F3h, 0FFh, 0FFh, 0C1h, 0EBh, 00Ah, 081h, 0EBh, 000h, 000h, 080h
        db  000h, 083h, 0C4h, 008h, 0F6h, 003h, 001h, 075h, 031h, 068h, 0C0h, 084h, 000h, 000h, 0E8h, 00Dh
        db  0E5h, 0FFh, 0FFh, 083h, 0C4h, 004h, 0F6h, 043h, 001h, 002h, 075h, 007h, 068h, 0D0h, 084h, 000h
        db  000h, 0EBh, 005h, 068h, 0DCh, 084h, 000h, 000h, 0E8h, 0F3h, 0E4h, 0FFh, 0FFh, 083h, 0C4h, 004h
        db  068h, 0E4h, 084h, 000h, 000h, 0E9h, 03Dh, 001h, 000h, 000h, 08Bh, 045h, 000h, 0F6h, 0C4h, 002h
        db  00Fh, 084h, 014h, 001h, 000h, 000h, 068h, 008h, 085h, 000h, 000h, 0E8h, 0D0h, 0E4h, 0FFh, 0FFh
        db  083h, 0C4h, 004h, 0F6h, 045h, 000h, 002h, 074h, 007h, 068h, 014h, 085h, 000h, 000h, 0EBh, 005h
        db  068h, 01Ch, 085h, 000h, 000h, 0E8h, 0B6h, 0E4h, 0FFh, 0FFh, 083h, 0C4h, 004h, 0F6h, 045h, 000h
        db  004h, 074h, 007h, 068h, 024h, 085h, 000h, 000h, 0EBh, 005h, 068h, 02Ch, 085h, 000h, 000h, 0E8h
        db  09Ch, 0E4h, 0FFh, 0FFh, 083h, 0C4h, 004h, 08Bh, 075h, 000h, 0F7h, 0C6h, 001h, 000h, 000h, 000h
        db  00Fh, 084h, 087h, 000h, 000h, 000h, 0F7h, 0C6h, 000h, 008h, 000h, 000h, 074h, 020h, 08Bh, 0C6h
        db  033h, 044h, 024h, 014h, 025h, 0FFh, 00Fh, 000h, 000h, 033h, 0C6h, 050h, 068h, 034h, 085h, 000h
        db  000h, 0E8h, 06Ah, 0E4h, 0FFh, 0FFh, 083h, 0C4h, 008h, 0E9h, 095h, 000h, 000h, 000h, 08Bh, 0CEh
        db  0C1h, 0E9h, 00Ch, 06Bh, 0C9h, 00Dh, 08Bh, 02Dh, 0E4h, 0F7h, 000h, 000h, 003h, 0E9h, 06Ah, 00Dh
        db  055h, 0E8h, 07Bh, 0E4h, 0FFh, 0FFh, 00Bh, 0C0h, 074h, 012h, 068h, 054h, 085h, 000h, 000h, 0E8h
        db  03Ch, 0E4h, 0FFh, 0FFh, 083h, 0C4h, 004h, 0E9h, 00Eh, 001h, 000h, 000h, 0FFh, 075h, 000h, 0E8h
        db  07Ah, 0FEh, 0FFh, 0FFh, 08Bh, 044h, 024h, 014h, 033h, 0C6h, 025h, 0FFh, 00Fh, 000h, 000h, 033h
        db  0C6h, 050h, 068h, 060h, 085h, 000h, 000h, 0E8h, 014h, 0E4h, 0FFh, 0FFh, 083h, 0C4h, 008h, 066h
        db  083h, 07Dh, 008h, 000h, 074h, 03Dh, 068h, 078h, 085h, 000h, 000h, 0EBh, 02Eh, 0F7h, 0C6h, 000h
        db  008h, 000h, 000h, 074h, 00Fh, 068h, 080h, 085h, 000h, 000h, 0E8h, 0F1h, 0E3h, 0FFh, 0FFh, 083h
        db  0C4h, 004h, 0EBh, 012h, 0C1h, 0EEh, 00Ch, 06Bh, 0F6h, 00Ah, 003h, 035h, 0ACh, 0F8h, 000h, 000h
        db  056h, 0E8h, 028h, 0FEh, 0FFh, 0FFh, 068h, 08Ch, 085h, 000h, 000h, 0E8h, 0D0h, 0E3h, 0FFh, 0FFh
        db  083h, 0C4h, 004h, 0BEh, 001h, 000h, 000h, 000h, 0EBh, 025h, 00Bh, 0C0h, 074h, 014h, 068h, 098h
        db  085h, 000h, 000h, 0E8h, 0B8h, 0E3h, 0FFh, 0FFh, 083h, 0C4h, 004h, 0BEh, 001h, 000h, 000h, 000h
        db  0EBh, 00Dh, 068h, 0A4h, 085h, 000h, 000h, 0E8h, 0A4h, 0E3h, 0FFh, 0FFh, 083h, 0C4h, 004h, 057h
        db  0E8h, 069h, 0EAh, 0FFh, 0FFh, 08Bh, 0E8h, 00Bh, 0EDh, 074h, 032h, 0FFh, 075h, 000h, 08Bh, 045h
        db  004h, 025h, 000h, 0F0h, 0FFh, 0FFh, 050h, 068h, 0ACh, 085h, 000h, 000h, 0E8h, 07Fh, 0E3h, 0FFh
        db  0FFh, 083h, 0C4h, 00Ch, 00Bh, 0F6h, 074h, 008h, 055h, 0E8h, 07Dh, 0E7h, 0FFh, 0FFh, 0EBh, 00Dh
        db  068h, 0C8h, 085h, 000h, 000h, 0E8h, 066h, 0E3h, 0FFh, 0FFh, 083h, 0C4h, 004h, 08Ah, 044h, 024h
        db  018h, 03Ch, 020h, 075h, 024h, 0F6h, 003h, 001h, 074h, 00Ah, 047h, 0C1h, 0E7h, 00Ch, 089h, 07Ch
        db  024h, 014h, 0EBh, 02Eh, 081h, 0C7h, 000h, 004h, 000h, 000h, 081h, 0E7h, 000h, 0FCh, 0FFh, 0FFh
        db  0C1h, 0E7h, 00Ch, 089h, 07Ch, 024h, 014h, 0EBh, 019h, 00Fh, 0B6h, 0C0h, 050h, 068h, 0D0h, 085h
        db  000h, 000h, 0E8h, 029h, 0E3h, 0FFh, 0FFh, 083h, 0C4h, 008h, 0C7h, 044h, 024h, 014h, 000h, 000h
        db  000h, 000h, 068h, 0E4h, 085h, 000h, 000h, 0E8h, 014h, 0E3h, 0FFh, 0FFh, 08Bh, 044h, 024h, 018h
        db  083h, 0C4h, 004h, 05Dh, 05Fh, 05Eh, 05Bh, 0C2h, 008h, 000h, 083h, 0ECh, 008h, 053h, 056h, 057h
        db  055h, 02Bh, 0DBh, 02Bh, 0EDh, 02Bh, 0F6h, 081h, 07Ch, 024h, 01Ch, 0EBh, 0FDh, 0FFh, 0FFh, 075h
        db  007h, 00Fh, 020h, 0D0h, 089h, 044h, 024h, 01Ch, 08Bh, 07Ch, 024h, 01Ch, 0C1h, 0EFh, 00Ch, 08Dh
        db  004h, 0BDh, 000h, 000h, 080h, 0FFh, 025h, 0FFh, 0F3h, 0FFh, 0FFh, 0C1h, 0E8h, 00Ah, 02Dh, 000h
        db  000h, 080h, 000h, 089h, 044h, 024h, 014h, 06Ah, 020h, 08Dh, 004h, 0BDh, 000h, 000h, 080h, 0FFh
        db  050h, 0E8h, 075h, 0E4h, 0FFh, 0FFh, 089h, 044h, 024h, 010h, 06Ah, 020h, 0FFh, 074h, 024h, 018h
        db  0E8h, 036h, 0E5h, 0FFh, 0FFh, 083h, 07Ch, 024h, 010h, 000h, 074h, 07Bh, 08Bh, 004h, 0BDh, 000h
        db  000h, 080h, 0FFh, 0F6h, 0C4h, 008h, 075h, 06Fh, 08Bh, 0C8h, 0C1h, 0E9h, 00Ch, 0F6h, 0C4h, 002h
        db  074h, 05Fh, 0A8h, 001h, 074h, 01Ch, 06Bh, 0C9h, 00Dh, 08Bh, 02Dh, 0E4h, 0F7h, 000h, 000h, 003h
        db  0E9h, 06Ah, 020h, 055h, 0E8h, 0C4h, 0EEh, 0FFh, 0FFh, 00Bh, 0C0h, 074h, 010h, 08Bh, 05Dh, 000h
        db  0EBh, 00Bh, 06Bh, 0C9h, 00Ah, 08Bh, 01Dh, 0ACh, 0F8h, 000h, 000h, 003h, 0D9h, 00Bh, 0DBh, 074h
        db  036h, 06Ah, 020h, 053h, 0E8h, 0D3h, 0EAh, 0FFh, 0FFh, 00Bh, 0C0h, 074h, 02Ah, 00Fh, 0B7h, 073h
        db  004h, 00Bh, 0EDh, 075h, 010h, 0F6h, 043h, 003h, 010h, 074h, 00Ah, 06Ah, 020h, 0FFh, 073h, 006h
        db  0E8h, 088h, 0EEh, 0FFh, 0FFh, 00Fh, 0B6h, 043h, 002h, 050h, 0E8h, 053h, 0ECh, 0FFh, 0FFh, 0EBh
        db  006h, 00Bh, 0C0h, 074h, 002h, 08Bh, 0F1h, 00Bh, 0F6h, 075h, 008h, 057h, 0E8h, 0EDh, 0E8h, 0FFh
        db  0FFh, 0EBh, 00Ah, 06Bh, 0F6h, 00Ch, 0A1h, 0ECh, 0FAh, 000h, 000h, 003h, 0C6h, 00Bh, 0C0h, 074h
        db  008h, 06Ah, 020h, 050h, 0E8h, 05Eh, 0E6h, 0FFh, 0FFh, 08Ah, 044h, 024h, 020h, 03Ch, 020h, 074h
        db  02Dh, 03Ch, 046h, 074h, 029h, 03Ch, 042h, 075h, 00Ah, 081h, 06Ch, 024h, 01Ch, 000h, 010h, 000h
        db  000h, 0EBh, 023h, 00Fh, 0B6h, 0C0h, 050h, 068h, 0E8h, 085h, 000h, 000h, 0E8h, 0DFh, 0E1h, 0FFh
        db  0FFh, 0C7h, 044h, 024h, 024h, 000h, 000h, 000h, 000h, 083h, 0C4h, 008h, 0EBh, 008h, 081h, 044h
        db  024h, 01Ch, 000h, 010h, 000h, 000h, 08Bh, 044h, 024h, 01Ch, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h
        db  008h, 0C2h, 008h, 000h, 053h, 056h, 057h, 055h, 08Bh, 074h, 024h, 014h, 081h, 0FEh, 0EBh, 0FDh
        db  0FFh, 0FFh, 075h, 002h, 02Bh, 0F6h, 081h, 0FEh, 000h, 000h, 000h, 0C0h, 073h, 00Dh, 06Bh, 0F6h
        db  00Ah, 08Bh, 03Dh, 0ACh, 0F8h, 000h, 000h, 003h, 0F7h, 0EBh, 006h, 08Bh, 03Dh, 0ACh, 0F8h, 000h
        db  000h, 08Bh, 0C6h, 02Bh, 0C7h, 0B9h, 00Ah, 000h, 000h, 000h, 02Bh, 0D2h, 0F7h, 0F1h, 00Fh, 0AFh
        db  0C1h, 003h, 0C7h, 03Bh, 0C6h, 074h, 00Ah, 068h, 0FCh, 085h, 000h, 000h, 0E9h, 04Eh, 001h, 000h
        db  000h, 08Bh, 047h, 0E8h, 0B9h, 00Ah, 000h, 000h, 000h, 083h, 0E8h, 01Ch, 02Bh, 0D2h, 0F7h, 0F1h
        db  00Fh, 0AFh, 0C1h, 08Dh, 01Ch, 007h, 03Bh, 0FEh, 00Fh, 087h, 02Ch, 001h, 000h, 000h, 03Bh, 0DEh
        db  00Fh, 086h, 024h, 001h, 000h, 000h, 08Dh, 06Fh, 0E4h, 06Ah, 004h, 055h, 0E8h, 070h, 0E1h, 0FFh
        db  0FFh, 00Bh, 0C0h, 00Fh, 085h, 00Ah, 001h, 000h, 000h, 0FFh, 075h, 004h, 055h, 0E8h, 05Fh, 0E1h
        db  0FFh, 0FFh, 00Bh, 0C0h, 00Fh, 085h, 0F9h, 000h, 000h, 000h, 03Bh, 0DEh, 00Fh, 086h, 005h, 001h
        db  000h, 000h, 08Bh, 006h, 00Bh, 0C0h, 00Fh, 084h, 0B8h, 000h, 000h, 000h, 03Bh, 0C7h, 072h, 008h
        db  03Bh, 0C3h, 00Fh, 082h, 0ACh, 000h, 000h, 000h, 0F6h, 046h, 003h, 010h, 074h, 02Eh, 08Bh, 06Eh
        db  006h, 06Ah, 00Dh, 055h, 0E8h, 028h, 0E1h, 0FFh, 0FFh, 00Bh, 0C0h, 00Fh, 085h, 0A0h, 000h, 000h
        db  000h, 0F6h, 046h, 003h, 008h, 075h, 00Eh, 066h, 083h, 07Dh, 008h, 000h, 074h, 007h, 0B8h, 070h
        db  086h, 000h, 000h, 0EBh, 00Ch, 0B8h, 060h, 086h, 000h, 000h, 0EBh, 005h, 0B8h, 080h, 086h, 000h
        db  000h, 080h, 07Ch, 024h, 018h, 046h, 075h, 005h, 080h, 038h, 06Ch, 075h, 067h, 050h, 08Bh, 0C6h
        db  02Bh, 0C7h, 0B9h, 00Ah, 000h, 000h, 000h, 02Bh, 0D2h, 0F7h, 0F1h, 050h, 068h, 090h, 086h, 000h
        db  000h, 0E8h, 0AAh, 0E0h, 0FFh, 0FFh, 083h, 0C4h, 00Ch, 056h, 0E8h, 0EFh, 0FAh, 0FFh, 0FFh, 066h
        db  08Bh, 046h, 004h, 066h, 00Bh, 0C0h, 074h, 022h, 00Fh, 0B7h, 0C8h, 06Bh, 0C9h, 00Ch, 08Bh, 02Dh
        db  0ECh, 0FAh, 000h, 000h, 003h, 0E9h, 06Ah, 004h, 055h, 0E8h, 0B3h, 0E0h, 0FFh, 0FFh, 00Bh, 0C0h
        db  075h, 040h, 055h, 0E8h, 083h, 0E4h, 0FFh, 0FFh, 0EBh, 00Dh, 068h, 0C8h, 086h, 000h, 000h, 0E8h
        db  06Ch, 0E0h, 0FFh, 0FFh, 083h, 0C4h, 004h, 068h, 0E4h, 086h, 000h, 000h, 0E8h, 05Fh, 0E0h, 0FFh
        db  0FFh, 083h, 0C4h, 004h, 083h, 0C6h, 00Ah, 03Bh, 0DEh, 00Fh, 087h, 033h, 0FFh, 0FFh, 0FFh, 0EBh
        db  036h, 056h, 055h, 068h, 034h, 086h, 000h, 000h, 0E8h, 043h, 0E0h, 0FFh, 0FFh, 083h, 0C4h, 00Ch
        db  0EBh, 025h, 056h, 055h, 068h, 09Ch, 086h, 000h, 000h, 0E8h, 032h, 0E0h, 0FFh, 0FFh, 083h, 0C4h
        db  00Ch, 0EBh, 014h, 068h, 020h, 086h, 000h, 000h, 0EBh, 005h, 068h, 010h, 086h, 000h, 000h, 0E8h
        db  01Ch, 0E0h, 0FFh, 0FFh, 083h, 0C4h, 004h, 02Bh, 0C0h, 05Dh, 05Fh, 05Eh, 05Bh, 0C2h, 008h, 000h
        db  056h, 057h, 0FFh, 035h, 0F4h, 0F8h, 000h, 000h, 0FFh, 035h, 024h, 0EFh, 000h, 000h, 0FFh, 035h
        db  01Ch, 0EFh, 000h, 000h, 0FFh, 035h, 088h, 0EEh, 000h, 000h, 0FFh, 035h, 084h, 0EEh, 000h, 000h
        db  0A1h, 088h, 0EEh, 000h, 000h, 003h, 005h, 084h, 0EEh, 000h, 000h, 050h, 08Bh, 00Dh, 050h, 0F9h
        db  000h, 000h, 0A1h, 040h, 0F9h, 000h, 000h, 02Bh, 0C8h, 051h, 050h, 0FFh, 035h, 018h, 0EFh, 000h
        db  000h, 068h, 0E8h, 086h, 000h, 000h, 0E8h, 0C5h, 0DFh, 0FFh, 0FFh, 083h, 0C4h, 028h, 0FFh, 035h
        db  07Ch, 0F8h, 000h, 000h, 0FFh, 035h, 0FCh, 0F8h, 000h, 000h, 0FFh, 035h, 0F4h, 0FAh, 000h, 000h
        db  068h, 0C4h, 087h, 000h, 000h, 0E8h, 0A6h, 0DFh, 0FFh, 0FFh, 083h, 0C4h, 010h, 0F6h, 005h
loc_5_2334:
        xor    cl, bh
        add    byte ptr [eax], al
        add    dh, byte ptr [esp + 68h]
        adc    byte ptr [eax - 6f180000h], cl
        db  0DFh, 0FFh, 0FFh, 083h, 0C4h, 004h
loc_5_2348:
        db  0F6h, 005h, 030h, 0F9h, 000h, 000h, 008h
        je     SHORT loc_5_2358
        db  068h, 020h, 088h, 000h, 000h
        jmp    SHORT loc_5_2364
loc_5_2358:
        db  068h, 034h, 088h, 000h, 000h
        jmp    SHORT loc_5_2364
        db  068h, 04Ch, 088h, 000h, 000h
loc_5_2364:
        call   sub_5_02D5
loc_5_2369:
        add    esp, 4
        db  08Bh, 03Dh, 064h, 0EFh, 000h, 000h
        db  00Bh
loc_5_2373:
        push   dword ptr [ebp + 13h]
        db  068h, 05Ch, 088h, 000h, 000h
        call   sub_5_02D5
        add    esp, 4
        db  08Bh, 03Dh, 064h, 0EFh, 000h, 000h
        db  08Bh, 035h, 018h, 0EFh, 000h, 000h
        mov    eax, esi
        db  02Bh, 005h, 088h, 0EEh, 000h, 000h
        mov    eax, 0
        js     SHORT loc_5_23A6
loc_5_239E:
        mov    eax, esi
        db  02Bh, 005h, 088h, 0EEh, 000h, 000h
loc_5_23A6:
        db  08Bh, 00Dh, 084h, 0EEh, 000h, 000h
        mov    edx, edi
        and    edx, eax
        sub    ecx, edx
        jns    SHORT loc_5_23B8
        sub    ecx, ecx
        jmp    SHORT loc_5_23D6
loc_5_23B8:
        mov    eax, esi
        db  02Bh, 005h
loc_5_23BC:
        mov    dh, ch
        add    byte ptr [eax], al
        jns    SHORT loc_5_23C6
        sub    esi, esi
        jmp    SHORT loc_5_23CC
loc_5_23C6:
        db  02Bh, 035h, 088h, 0EEh, 000h, 000h
loc_5_23CC:
        db  08Bh, 00Dh, 084h, 0EEh, 000h, 000h
        and    edi, esi
        sub    ecx, edi
loc_5_23D6:
        db  0FFh, 035h, 058h, 0EFh, 000h, 000h
        db  0FFh, 035h, 05Ch, 0EFh, 000h, 000h
        db  0FFh, 035h, 030h, 0EFh, 000h, 000h
        db  0FFh, 035h, 02Ch, 0EFh, 000h, 000h
        push   ecx
        db  0FFh, 035h, 060h, 0EFh, 000h, 000h
        db  0FFh, 035h, 028h, 0EFh, 000h, 000h
        db  068h, 07Ch, 088h, 000h, 000h
        call   sub_5_02D5
        add    esp, 20h
        db  0FFh, 035h, 000h, 0F9h, 000h, 000h
        db  0FFh, 035h, 01Ch, 0FDh, 000h, 000h
        db  0FFh, 035h, 010h, 0FDh, 000h, 000h
        db  068h, 00Ch, 089h, 000h, 000h
        call   sub_5_02D5
        add    esp, 10h
        db  083h, 03Dh, 080h, 0F8h, 000h, 000h, 000h
        je     SHORT loc_5_246B
        db  068h, 058h, 089h, 000h, 000h
        call   sub_5_02D5
        add    esp, 4
        db  068h
loc_5_243E:
        add    byte ptr [eax], 0
        add    bh, bh
        db  035h, 080h, 0F8h, 000h, 000h
loc_5_2448:
        call   sub_5_0306
        or     eax, eax
        db  0FFh, 035h, 080h, 0F8h, 000h, 000h
        je     SHORT loc_5_245E
        db  068h, 06Ch, 089h, 000h, 000h
        jmp    SHORT loc_5_2463
loc_5_245E:
        db  068h, 078h, 089h, 000h, 000h
loc_5_2463:
        call   sub_5_02D5
        add    esp, 8
loc_5_246B:
        db  0A1h, 0A8h, 0FDh, 000h, 000h
        cmp    eax, -1
loc_5_2473:
        je     SHORT loc_5_2489
        push   eax
        db  0FFh, 035h, 02Ch, 0FDh, 000h, 000h
        db  068h, 07Ch, 089h, 000h, 000h
loc_5_2481:
        call   sub_5_02D5
        add    esp, 0ch
loc_5_2489:
        db  0FFh, 035h, 040h, 0EFh, 000h, 000h
        db  0FFh, 035h, 03Ch, 0EFh, 000h, 000h
        db  0FFh, 035h, 038h, 0EFh, 000h, 000h
        db  0FFh, 035h, 034h, 0EFh, 000h, 000h
loc_5_24A1:
        db  068h, 0B4h, 089h, 000h, 000h
        call   sub_5_02D5
loc_5_24AB:
        add    esp, 14h
loc_5_24AE:
        db  0FFh, 035h, 044h, 0EFh, 000h, 000h
        db  0FFh, 035h, 054h, 0EFh, 000h, 000h
        db  0FFh, 035h, 050h, 0EFh, 000h, 000h
        db  0FFh, 035h, 04Ch, 0EFh, 000h, 000h
        db  0FFh, 035h, 048h, 0EFh, 000h, 000h
        db  068h, 0F8h, 089h, 000h, 000h
        call   sub_5_02D5
        add    esp, 18h
        sub    eax, eax
        pop    edi
        pop    esi
        ret    8
        db  081h, 07Ch, 024h, 004h, 0EBh, 0FDh, 0FFh, 0FFh
loc_5_24E8:
        db  075h, 007h
        mov    eax, cr2
        mov    dword ptr [esp + 4], eax
        db  0FFh, 074h
loc_5_24F3:
        and    al, 4
        db  0E8h, 06Bh, 011h, 0F8h, 0FFh
        add    esp, 4
        or     eax, eax
        db  074h, 037h
        mov    eax, dword ptr [eax]
        test   al, 1
        je     SHORT loc_5_2512
        push   dword ptr [esp + 4]
        db  068h, 050h, 08Ah, 000h, 000h
        jmp    SHORT loc_5_2520
loc_5_2512:
        test   ah, 2
        jne    SHORT loc_5_2532
        push   dword ptr [esp + 4]
        db  068h, 06Ch, 08Ah, 000h, 000h
loc_5_2520:
        call   sub_5_02D5
        mov    dword ptr [esp + 0ch], 0
        add    esp, 8
        jmp    SHORT loc_5_2551
loc_5_2532:
        mov    eax, dword ptr [esp + 4]
        mov    al, byte ptr [eax]
        db  0FFh
loc_5_2539:
        je     SHORT loc_5_255F
        add    al, 68h
        test   byte ptr [edx - 70180000h], cl
        db  0DDh, 0FFh, 0FFh, 081h, 044h, 024h, 00Ch, 000h, 010h, 000h, 000h, 083h, 0C4h, 008h
loc_5_2551:
        mov    eax, dword ptr [esp + 4]
        ret    8
loc_5_2558:
        push   esi
        db  0A1h, 03Ch, 0F9h, 000h, 000h
        db  083h
loc_5_255F:
        js     SHORT loc_5_2571
loc_5_2561:
        add    byte ptr [edi + ecx + 68h], dh
        mov    al, byte ptr [0e800008ah]
        db  067h, 0DDh, 0FFh, 0FFh, 083h, 0C4h, 004h
loc_5_2571:
        jmp    SHORT loc_5_2598
loc_5_2573:
        db  08Bh, 035h, 050h, 0F9h, 000h, 000h
        db  068h, 0FFh, 0FFh
loc_5_257C:
        dec    dword ptr [edi]
        db  0E8h, 04Fh, 019h, 0F8h, 0FFh
        db  0A1h, 050h, 0F9h, 000h, 000h
        sub    eax, esi
        push   eax
        db  068h, 0ECh, 08Ah, 000h, 000h
        call   sub_5_02D5
        add    esp, 8
loc_5_2598:
        sub    eax, eax
        pop    esi
        ret    8
        db  068h, 008h, 08Bh, 000h, 000h, 0E8h, 02Dh, 0DDh, 0FFh, 0FFh, 083h, 0C4h, 004h, 02Bh, 0C0h, 0C2h
        db  008h, 000h
sub_5_25B0:
        db  068h, 030h, 08Bh, 000h, 000h
        call   sub_5_02D5
        add    esp, 4
        sub    eax, eax
        ret    0ch
        db  068h, 058h, 08Bh, 000h, 000h, 0E8h, 009h, 0DDh, 0FFh, 0FFh, 083h, 0C4h, 004h, 02Bh, 0C0h, 0C2h
        db  008h, 000h, 053h, 056h, 057h, 08Ah, 05Ch, 024h, 014h, 080h, 0FBh, 046h, 074h, 036h, 080h, 0FBh
        db  042h, 074h, 031h, 080h, 0FBh, 020h, 074h, 02Ch, 080h, 07Ch, 024h, 010h, 048h, 075h, 00Fh, 080h
        db  0FBh, 058h
loc_5_25F4:
        je     SHORT loc_5_2616
        db  080h, 0FBh
loc_5_25F8:
        push   edi
        je     SHORT loc_5_2616
        cmp    bl, 53h
        je     SHORT loc_5_2616
        movzx  eax, bl
        push   eax
        db  068h, 080h, 08Bh, 000h, 000h
        call   sub_5_02D5
        add    esp, 8
        jmp    NEAR PTR loc_5_2710
loc_5_2616:
        movzx  ecx, byte ptr [esp + 10h]
loc_5_261B:
        lea    eax, [ecx - 20h]
        cmp    eax, 39h
        ja     SHORT loc_5_262A
        db  0FFh, 024h, 085h, 016h, 037h, 000h, 000h
loc_5_262A:
        push   ecx
        db  068h, 094h, 08Bh, 000h, 000h
        call   sub_5_02D5
        add    esp, 8
        jmp    NEAR PTR loc_5_2710
        db  0BFh, 070h, 02Dh, 000h, 000h, 0E9h, 0B2h, 000h, 000h, 000h, 068h, 098h, 066h, 000h, 000h, 0E8h
        db  063h, 0DCh, 0FFh, 0FFh, 068h, 0F0h, 06Bh, 000h, 000h, 0E8h, 059h, 0DCh, 0FFh, 0FFh, 0E9h, 0B0h
        db  000h, 000h, 000h, 0BFh, 02Ch, 017h, 000h, 000h, 0E9h
loc_5_2666:
        pop    dword ptr [eax]
        add    byte ptr [eax], al
        db  0BFh, 057h, 021h, 000h, 000h
        jmp    NEAR PTR loc_5_26F9
        db  0BFh, 060h, 015h, 000h, 000h, 0EBh, 07Eh, 0BFh, 09Eh, 035h, 000h, 000h, 0EBh, 077h, 0FFh
loc_5_2683:
        je     SHORT loc_5_26A9
        sbb    al, 0ffh
        db  074h, 024h
        sbb    al, 53h
        call   sub_5_25B0
        jmp    SHORT loc_5_2710
        db  0BFh, 0D4h, 01Dh, 000h, 000h, 0EBh, 060h, 0BFh
loc_5_269A:
        test   dword ptr [eax], ebp
        add    byte ptr [eax], al
        jmp    SHORT loc_5_26F9
        db  0E8h, 05Bh, 001h, 000h, 000h, 0EBh, 069h, 0BFh, 0E2h
loc_5_26A9:
        and    dword ptr [eax], eax
        db  000h
loc_5_26AC:
        jmp    SHORT loc_5_26F9
        db  0BFh, 0E0h, 034h, 000h, 000h, 0EBh, 044h, 0BFh, 058h, 035h, 000h, 000h, 0EBh, 03Dh, 0BFh, 022h
loc_5_26BE:
        pop    ds
        add    byte ptr [eax], al
        jmp    SHORT loc_5_26F9
        db  0BFh, 006h, 023h, 000h, 000h, 0EBh, 02Fh, 0BFh, 0C5h, 032h, 000h, 000h, 0EBh, 028h, 0BFh, 090h
        db  014h, 000h, 000h, 0EBh, 021h, 0BFh, 0C2h, 035h, 000h, 000h, 0EBh, 01Ah, 0BFh, 051h, 01Bh, 000h
        db  000h, 0EBh, 013h, 0BFh, 019h, 031h, 000h, 000h, 0EBh, 00Ch, 0BFh, 0CFh, 02Fh, 000h, 000h, 0EBh
        db  005h, 0BFh, 06Bh, 022h, 000h, 000h
loc_5_26F9:
        mov    esi, dword ptr [esp + 1ch]
        or     esi, esi
        je     SHORT loc_5_2710
        mov    eax, dword ptr [esp + 18h]
loc_5_2705:
        push   ebx
        push   eax
        call   edi
        or     eax, eax
        je     SHORT loc_5_2710
        dec    esi
        jne    SHORT loc_5_2705
loc_5_2710:
        pop    edi
        pop    esi
        pop    ebx
        ret    10h
        db  03Dh, 036h, 000h, 000h, 02Ah, 036h, 000h, 000h, 02Ah, 036h, 000h, 000h, 02Ah, 036h, 000h, 000h
        db  02Ah, 036h, 000h, 000h, 02Ah, 036h, 000h, 000h, 02Ah, 036h, 000h, 000h, 02Ah, 036h, 000h, 000h
        db  02Ah, 036h, 000h, 000h, 02Ah, 036h, 000h, 000h, 02Ah, 036h, 000h, 000h, 02Ah, 036h, 000h, 000h
        db  02Ah, 036h, 000h, 000h, 02Ah, 036h, 000h, 000h, 02Ah, 036h, 000h, 000h, 02Ah, 036h, 000h, 000h
        db  02Ah, 036h, 000h, 000h, 02Ah, 036h, 000h, 000h, 02Ah, 036h, 000h, 000h, 02Ah, 036h, 000h, 000h
        db  02Ah, 036h, 000h, 000h, 02Ah, 036h, 000h, 000h, 02Ah, 036h, 000h, 000h, 02Ah, 036h, 000h, 000h
        db  02Ah, 036h, 000h, 000h, 02Ah, 036h, 000h, 000h, 02Ah, 036h, 000h, 000h, 02Ah, 036h, 000h, 000h
        db  02Ah, 036h, 000h, 000h, 02Ah, 036h, 000h, 000h, 02Ah, 036h, 000h, 000h, 047h, 036h, 000h, 000h
        db  02Ah, 036h, 000h, 000h, 060h, 036h, 000h, 000h, 02Ah, 036h, 000h, 000h, 06Ah, 036h, 000h, 000h
        db  074h, 036h, 000h, 000h, 07Bh, 036h, 000h, 000h, 082h, 036h, 000h, 000h, 092h, 036h, 000h, 000h
        db  099h, 036h, 000h, 000h, 0A0h, 036h, 000h, 000h, 02Ah, 036h, 000h, 000h, 02Ah, 036h, 000h, 000h
        db  0A7h, 036h, 000h, 000h, 0AEh, 036h, 000h, 000h, 0B5h, 036h, 000h, 000h, 02Ah, 036h, 000h, 000h
        db  0BCh, 036h, 000h, 000h, 02Ah, 036h, 000h, 000h, 0C3h, 036h, 000h, 000h, 0CAh, 036h, 000h, 000h
        db  0D1h, 036h, 000h, 000h, 0D8h, 036h, 000h, 000h, 0DFh, 036h, 000h, 000h, 0E6h, 036h, 000h, 000h
        db  0EDh, 036h, 000h, 000h
loc_5_27FA:
        hlt
        add    byte ptr ss:[eax], al
        add    byte ptr [eax], al
        pushad
        sub    esp, 4
        mov    ebp, esp
        xor    eax, eax
        mov    dword ptr [ebp], eax
        db  038h, 005h, 0F8h, 0F1h, 000h, 000h
        jne    SHORT loc_5_2823
        db  068h, 0C0h, 08Bh, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        jmp    NEAR PTR loc_5_28D4
loc_5_2823:
        db  068h, 0D3h, 08Bh, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  068h, 0F1h, 08Bh, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
loc_5_2839:
        db  00Fh, 0B6h, 0B8h, 0E8h, 0F0h, 000h, 000h
        test   edi, edi
        je     SHORT loc_5_2886
        lea    edi, [edi + edi*2 - 3]
        shl    edi, 3
        db  003h, 03Dh, 0E4h, 0F0h, 000h, 000h
        mov    edx, dword ptr [edi + 10h]
        mov    edx, dword ptr [edx + 0ch]
        db  068h, 00Bh, 08Ch, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        cmp    dword ptr [edi + 8], 0
        jne    SHORT loc_5_2875
        db  068h, 014h, 08Ch, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        jmp    SHORT loc_5_2886
loc_5_2875:
        mov    ebx, dword ptr [edi + 8]
        mov    ecx, dword ptr [edi + 0ch]
        db  068h, 031h, 08Ch, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
loc_5_2886:
        inc    eax
        cmp    eax, 110h
        jb     SHORT loc_5_2839
        db  068h, 03Eh, 08Ch, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0CDh, 020h, 0C4h, 000h, 001h, 000h
        pushfd
        db  068h, 073h, 08Ch, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  068h, 077h, 08Ch, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        popfd
loc_5_28B7:
        je     SHORT loc_5_28D4
        db  068h, 07Bh, 08Ch, 000h, 000h
        db  0CDh
loc_5_28BF:
        and    bl, dh
loc_5_28C1:
        add    byte ptr [ecx], al
        db  000h, 08Bh, 035h, 0F9h, 0F1h, 000h
        add    al, ch
        cmp    byte ptr [eax], al
        add    byte ptr [eax], al
        call   sub_5_28E6
loc_5_28D4:
        mov    esp, ebp
        add    esp, 4
        db  068h, 09Dh, 08Ch, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        popad
        ret
sub_5_28E6:
        db  068h, 0A1h, 08Ch, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        push   eax
        db  0CDh, 020h, 0C4h, 000h, 001h, 000h
        je     SHORT loc_5_28D4
        pop    eax
        db  068h, 0A5h, 08Ch, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        ret
        db  060h, 085h, 0F6h, 074h, 033h, 033h, 0C9h, 085h, 0F6h, 074h, 01Ch, 08Bh, 046h, 004h, 08Bh, 05Eh
        db  008h, 08Bh, 0D3h, 02Bh, 0D0h, 042h, 003h, 0CAh, 068h, 0A9h, 08Ch, 000h, 000h, 0CDh, 020h, 0F3h
        db  000h, 001h, 000h, 08Bh, 036h, 0EBh, 0E0h, 08Bh, 074h, 024h, 004h, 068h, 0CEh, 08Ch, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 061h, 0C3h, 068h, 008h, 08Dh, 000h, 000h, 0CDh, 020h, 0F3h
        db  000h, 001h, 000h, 0EBh, 0F1h, 0F9h, 0C3h, 080h, 03Dh, 005h, 0BFh, 000h, 000h, 000h, 074h, 00Ah
        db  066h, 0B8h, 079h, 000h, 0CDh, 041h, 066h, 083h, 0F8h, 001h, 0F5h, 0C3h
loc_5_2963:
        ret
        db  0A1h, 0F8h, 0E3h, 000h, 000h, 048h, 068h, 020h, 08Dh, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h
        db  000h, 068h, 03Bh, 08Dh, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 068h, 03Fh, 08Dh, 000h
        db  000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0B9h, 0C4h, 0F6h, 000h, 000h, 0E8h, 016h, 000h, 000h
        db  000h, 068h, 05Ah, 08Dh, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0B9h, 0E4h, 0F6h, 000h
        db  000h, 0E8h, 001h, 000h, 000h, 000h, 0C3h, 08Bh, 041h, 00Ch, 085h, 0C0h, 074h, 01Eh, 068h, 07Ch
        db  08Dh, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 08Bh, 041h, 010h, 066h, 08Bh, 050h, 018h
        db  068h, 093h, 08Dh, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0C3h, 068h, 0B1h, 08Dh, 000h
        db  000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0C3h
sub_5_29DC:
        pushad
        call   sub_5_39C1
        cmp    esi, 1
        jb     SHORT loc_5_29F2
        mov    edi, dword ptr [esp + 24h]
        add    esi, 0ch
        cmpsd
        jne    SHORT loc_5_29F2
        cmpsd
loc_5_29F2:
        popad
        ret    4
sub_5_29F6:
        pushad
        db  0FFh, 005h, 0C0h, 08Dh, 000h, 000h
        db  08Bh, 00Dh, 0C0h, 08Dh, 000h, 000h
        cmp    ecx, 10h
        ja     SHORT loc_5_2A3B
        db  089h, 03Ch, 08Dh, 000h, 08Eh, 000h, 000h
        mov    eax, dword ptr [esp + 24h]
        db  089h, 004h, 08Dh, 0C0h, 08Dh, 000h, 000h
        cmp    byte ptr [eax], 40h
        jne    SHORT loc_5_2A3B
        pushad
        call   sub_5_39C1
        mov    dword ptr [esp + 4], esi
        popad
        jne    SHORT loc_5_2A31
        db  0BEh, 0D5h, 08Eh, 000h, 000h
loc_5_2A31:
        add    esi, 0ch
        db  089h, 034h, 08Dh, 040h, 08Eh, 000h, 000h
loc_5_2A3B:
        popad
        ret    4
        db  083h, 025h, 0C0h, 08Dh, 000h, 000h, 000h, 068h, 05Fh, 08Fh, 000h, 000h, 0CDh, 020h, 0F3h, 000h
        db  001h, 000h, 0FCh, 08Bh, 01Dh, 014h, 0F7h, 000h, 000h
loc_5_2A58:
        mov    edi, dword ptr [ebx - 1ch]
loc_5_2A5B:
        db  03Bh, 03Dh, 098h, 0F6h, 000h, 000h
        db  075h, 011h
        mov    esi, esp
        mov    ax, ds
        cmp    ax, 30h
        db  074h, 009h
        cmp    ax, 70h
        db  074h
loc_5_2A73:
        add    ecx, dword ptr [ebx - 12cca389h]
loc_5_2A79:
        push   80000000h
        push   0
        push   0
        push   1
        push   esi
        db  0CDh, 020h, 073h, 001h, 001h, 000h
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_5_2C8E
        mov    ecx, esi
        or     ecx, 0fffh
        inc    ecx
        sub    ecx, esi
        shr    ecx, 2
        sub    ecx, 20h
        jb     NEAR PTR loc_5_2C8E
        add    esi, 40h
loc_5_2AB0:
        mov    eax, dword ptr [esi]
        db  03Dh, 098h, 080h, 000h, 000h
        je     SHORT loc_5_2AEF
        db  03Dh, 0F1h, 07Bh, 000h, 000h
        je     NEAR PTR loc_5_2B6A
        db  03Dh, 070h, 07Ah, 000h, 000h
        je     NEAR PTR loc_5_2BAF
        db  03Dh, 02Dh, 023h, 000h, 000h
        je     NEAR PTR loc_5_2C38
        db  03Dh, 002h, 02Bh, 000h, 000h
        je     NEAR PTR loc_5_2C0D
        add    esi, 4
        loop   loc_5_2AB0
        jmp    NEAR PTR loc_5_2C99
loc_5_2AEF:
        db  081h, 07Eh, 0F4h, 053h, 080h, 000h, 000h
        jne    SHORT loc_5_2A79
        mov    edx, dword ptr [esi + 38h]
        cmp    edx, dword ptr [esi + 28h]
        jne    NEAR PTR loc_5_2A79
        cmp    dword ptr [esi + 2ch], edi
        jne    NEAR PTR loc_5_2A79
        mov    ebp, dword ptr [esi + 34h]
        db  068h, 084h, 08Eh, 000h, 000h
        call   sub_5_29DC
        jne    SHORT loc_5_2B4A
        cmp    dword ptr [ebp], 0e9c3c9fch
        jne    SHORT loc_5_2B4A
        cmp    dword ptr [ebp - 0ah], 875ff50h
        jne    SHORT loc_5_2B4A
        mov    ebp, dword ptr [esi + 44h]
        db  068h, 08Ch, 08Eh, 000h, 000h
        call   sub_5_29DC
        jne    SHORT loc_5_2B40
        mov    ebp, dword ptr [esi + 54h]
loc_5_2B40:
        db  068h, 003h, 08Fh, 000h, 000h
        call   sub_5_29F6
loc_5_2B4A:
        db  068h, 094h, 08Eh, 000h, 000h
        call   sub_5_29DC
        jne    SHORT loc_5_2B60
        db  068h, 0E9h, 08Eh, 000h, 000h
        call   sub_5_29F6
loc_5_2B60:
        mov    ecx, 20204449h
        jmp    NEAR PTR loc_5_2C79
loc_5_2B6A:
        db  081h, 07Eh, 0F4h, 0F8h, 07Bh, 000h, 000h
        jne    NEAR PTR loc_5_2A79
        mov    ebp, dword ptr [esi + 24h]
        db  068h, 094h, 08Eh, 000h, 000h
        call   sub_5_29DC
        jne    SHORT loc_5_2BA2
        cmp    dword ptr [ebp], 0fe2c6280h
        je     SHORT loc_5_2BA2
        cmp    dword ptr [ebp], 1b8h
        je     SHORT loc_5_2BA2
        db  068h, 0E9h, 08Eh, 000h, 000h
        call   sub_5_29F6
loc_5_2BA2:
        mov    edx, dword ptr [esi + 20h]
        mov    ecx, 206d6573h
        jmp    NEAR PTR loc_5_2C79
loc_5_2BAF:
        db  081h, 07Eh, 0F4h, 0A7h, 07Ah, 000h, 000h
        jne    NEAR PTR loc_5_2A79
        mov    ebp, dword ptr [esi + 10h]
        mov    edx, dword ptr [esi + 14h]
        push   edi
        mov    edi, dword ptr [edx + 10h]
        db  068h, 02Ah, 08Fh, 000h, 000h
        call   sub_5_29F6
        pop    edi
        mov    ecx, 20205343h
        db  081h, 0FAh, 0C4h, 0F6h, 000h, 000h
        je     SHORT loc_5_2C08
        mov    ecx, 20363856h
        db  081h, 0FAh, 0E4h, 0F6h, 000h, 000h
        je     SHORT loc_5_2C08
        db  068h, 094h, 08Eh, 000h, 000h
        call   sub_5_29DC
        jne    SHORT loc_5_2C01
        db  068h, 0E9h, 08Eh, 000h, 000h
        call   sub_5_29F6
loc_5_2C01:
        mov    ecx, 2078746dh
        jmp    SHORT loc_5_2C79
loc_5_2C08:
        mov    ebp, dword ptr [esi + 24h]
        jmp    SHORT loc_5_2C79
loc_5_2C0D:
        mov    ebp, dword ptr [esi + 4]
        push   80000000h
        push   0
        push   0
        push   14h
        push   ebp
        db  0CDh, 020h, 073h, 001h, 001h, 000h
        add    esp, 14h
        jb     NEAR PTR loc_5_2A79
        mov    edx, dword ptr [ebp + 10h]
        mov    ebp, dword ptr [ebp + 0ch]
        mov    ecx, 20204f54h
        jmp    SHORT loc_5_2C79
loc_5_2C38:
        mov    ebp, dword ptr [esi + 8]
        push   80000000h
        push   0
        push   0
        push   18h
        push   ebp
        db  0CDh, 020h, 073h, 001h, 001h, 000h
        add    esp, 14h
        jb     NEAR PTR loc_5_2A79
        mov    edx, dword ptr [ebp + 4]
        mov    ebp, dword ptr [ebp]
        mov    ecx, 20747665h
        db  081h, 0FDh, 0CEh, 02Ah, 000h, 000h
        je     NEAR PTR loc_5_2A79
        db  081h, 0FDh, 0CEh, 02Ah, 000h, 000h
        je     NEAR PTR loc_5_2A79
loc_5_2C79:
        db  089h, 00Dh, 0A0h, 08Eh, 000h, 000h
        db  068h, 0A0h, 08Eh, 000h, 000h
        call   sub_5_2D27
        jmp    NEAR PTR loc_5_2A79
loc_5_2C8E:
        db  068h, 0BAh, 08Eh, 000h, 000h
        call   sub_5_2D27
        inc    ebp
loc_5_2C99:
        db  0E8h, 0CEh, 0CDh, 0FFh, 0FFh
        db  0E8h, 00Ah, 0CEh, 0FFh, 0FFh
        or     ebp, ebp
        jne    SHORT loc_5_2CB1
        db  068h, 0DCh, 08Eh, 000h, 000h
        call   sub_5_2D27
loc_5_2CB1:
        mov    edi, dword ptr [edi + 40h]
        test   edi, edi
        jne    NEAR PTR loc_5_2A5B
        mov    ebx, dword ptr [ebx + 2ch]
        test   ebx, ebx
        jne    NEAR PTR loc_5_2A58
        db  08Bh, 00Dh, 0C0h, 08Dh, 000h, 000h
        jecxz  loc_5_2D26
        cmp    ecx, 10h
        jbe    SHORT loc_5_2CD9
        mov    ecx, 10h
loc_5_2CD9:
        db  08Bh, 03Ch, 08Dh, 000h, 08Eh, 000h, 000h
        mov    ax, word ptr [edi + 18h]
        db  068h, 098h, 08Fh, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  08Bh, 014h, 08Dh, 0C0h, 08Dh, 000h, 000h
        cmp    byte ptr [edx], 40h
        jne    SHORT loc_5_2D12
        inc    edx
        db  08Bh, 034h, 08Dh, 040h, 08Eh, 000h, 000h
        xor    eax, eax
        xchg   byte ptr [esi + 8], al
        push   esi
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        xchg   byte ptr [esi + 8], al
loc_5_2D12:
        push   edx
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  068h, 0DEh, 08Eh, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        loop   loc_5_2CD9
loc_5_2D26:
        ret
sub_5_2D27:
        db  081h, 0FDh, 0CCh, 07Eh, 000h, 000h
        je     SHORT loc_5_2D37
        db  081h, 0FDh, 03Dh, 07Dh, 000h, 000h
        jne    SHORT loc_5_2D3A
loc_5_2D37:
        ret    4
loc_5_2D3A:
        mov    eax, dword ptr [edi + 10h]
        test   byte ptr [eax + 2dh], 2
        mov    al, 20h
        jne    SHORT loc_5_2D47
        mov    al, 21h
loc_5_2D47:
        db  03Bh, 03Dh, 098h, 0F6h, 000h, 000h
        jne    SHORT loc_5_2D51
        mov    al, 2ah
loc_5_2D51:
        db  0CDh, 020h, 0C3h, 000h, 001h, 000h
        mov    ax, word ptr [edi + 18h]
        db  068h, 09Ch, 08Eh, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        mov    al, 20h
        db  03Bh, 03Dh, 0D4h, 0F6h, 000h, 000h
        jne    SHORT loc_5_2D72
        mov    al, 3eh
loc_5_2D72:
        db  0CDh, 020h, 0C3h, 000h, 001h, 000h
        db  0CDh, 020h, 0F3h, 080h, 001h, 000h
        add    byte ptr [eax], al
        pushad
loc_5_2D81:
        lodsb
        cmp    al, 20h
        je     SHORT loc_5_2D81
        or     al, al
        je     SHORT loc_5_2D93
        cmp    al, 3bh
        jne    NEAR PTR loc_5_2E1C
loc_5_2D93:
        db  068h, 0B8h, 091h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  068h, 0BCh, 091h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  068h, 0C0h, 091h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        xor    ecx, ecx
loc_5_2DB6:
        db  08Bh, 034h, 0CDh, 0A8h, 08Fh, 000h, 000h
        test   esi, esi
        je     SHORT loc_5_2E01
        db  083h, 03Ch, 0CDh, 0ACh, 08Fh, 000h, 000h, 000h
        je     SHORT loc_5_2DFE
        mov    al, 5bh
        db  0CDh, 020h, 0C3h, 000h, 001h, 000h
        mov    al, cl
        add    al, 41h
        db  0CDh, 020h, 0C3h, 000h, 001h, 000h
        db  068h, 00Ah, 092h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0CDh, 020h, 0C2h, 000h, 001h, 000h
        mov    al, 0dh
        db  0CDh, 020h, 0C3h, 000h, 001h, 000h
        mov    al, 0ah
        db  0CDh, 020h, 0C3h, 000h, 001h, 000h
loc_5_2DFE:
        inc    ecx
        jmp    SHORT loc_5_2DB6
loc_5_2E01:
        db  068h, 00Eh, 092h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0CDh, 020h, 0C4h, 000h, 001h, 000h
        je     SHORT loc_5_2E7F
        and    al, 0dfh
        db  0CDh, 020h, 0C3h, 000h, 001h, 000h
loc_5_2E1C:
        sub    al, 41h
        jl     NEAR PTR loc_5_2D93
        cmp    al, 18h
        jge    NEAR PTR loc_5_2D93
        movzx  eax, al
        db  083h, 03Ch, 0C5h, 0ACh, 08Fh, 000h, 000h, 000h
        je     NEAR PTR loc_5_2D93
        db  068h, 031h, 092h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  068h, 035h, 092h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0FFh, 034h, 0C5h, 0A8h, 08Fh, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  068h, 039h, 092h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  068h, 03Dh, 092h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        pushad
        db  0FFh, 014h, 0C5h, 0ACh, 08Fh, 000h, 000h
        popad
loc_5_2E7F:
        db  068h, 041h, 092h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        popad
        ret
        db  068h, 045h, 092h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0A1h, 020h, 0DAh, 000h, 000h
        db  053h, 051h, 052h, 056h, 033h, 0D2h, 0B9h, 0E8h, 003h, 000h, 000h, 0F7h, 0F1h, 08Bh, 0DAh, 033h
        db  0D2h, 0B9h, 03Ch, 000h, 000h, 000h, 0F7h, 0F1h, 08Bh, 0F2h, 033h, 0D2h, 0F7h, 0F1h, 053h, 056h
        db  052h, 050h, 054h, 068h, 054h, 092h, 000h, 000h, 0CDh, 020h, 02Dh, 001h, 001h, 000h, 083h, 0C4h
        db  018h, 05Eh, 05Ah, 059h, 05Bh, 0C3h, 060h, 08Bh, 07Dh, 000h, 066h, 064h, 08Bh, 057h, 022h, 066h
        db  083h, 0FAh, 030h, 074h, 00Dh, 068h, 068h, 092h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0EBh, 065h, 064h, 08Bh, 077h, 010h, 052h, 06Ah, 004h, 056h, 0E8h, 073h, 001h, 000h, 000h, 059h
        db  059h, 05Ah, 00Bh, 0C0h, 074h, 00Dh, 068h, 09Fh, 092h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h
        db  000h, 0EBh, 044h, 0B9h, 014h, 000h, 000h, 000h, 0FCh, 08Bh, 0FEh, 051h, 052h, 06Ah, 004h, 056h
        db  0E8h, 04Dh, 001h, 000h, 000h, 059h, 059h, 05Ah, 059h, 00Bh, 0C0h, 074h, 00Dh, 068h, 0DBh, 092h
        db  000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0EBh, 01Dh, 0ADh, 068h, 0F9h, 092h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 0E2h, 0D1h, 083h, 07Ch, 024h, 014h, 000h, 075h, 008h, 0CDh
        db  020h, 0C4h, 000h, 001h, 000h, 075h, 0BCh, 061h, 0C3h, 060h, 09Ch, 0B8h, 020h, 020h, 020h, 020h
        db  050h, 050h, 033h, 0EDh, 08Bh, 03Dh, 040h, 0C3h, 000h, 000h, 085h, 0F6h, 00Fh, 084h, 0E0h, 000h
        db  000h, 000h, 064h, 080h, 03Eh, 03Dh, 00Fh, 085h, 0D6h, 000h, 000h, 000h, 046h, 064h, 08Ah, 006h
        db  03Ch, 03Bh, 074h, 00Dh, 084h, 0C0h, 074h, 009h, 088h, 004h, 02Ch, 045h, 083h, 0FDh, 008h, 072h
        db  0EBh, 0E9h, 0BCh, 000h, 000h, 000h, 08Dh, 077h, 00Ch, 085h, 0EDh, 074h, 020h, 08Bh, 00Eh, 033h
        db  00Ch, 024h, 081h, 0E1h, 0DFh, 0DFh, 0DFh, 0DFh, 00Fh, 085h, 08Ah, 000h, 000h, 000h, 08Bh, 04Eh
        db  004h, 033h, 04Ch, 024h, 004h, 081h, 0E1h, 0DFh, 0DFh, 0DFh, 0DFh, 075h, 07Bh, 0B9h, 008h, 000h
        db  000h, 000h, 0FCh, 0ACh, 0CDh, 020h, 0C3h, 000h, 001h, 000h, 0E2h, 0F7h, 066h, 08Bh, 047h, 006h
        db  08Ah, 05Fh, 008h, 08Ah, 04Fh, 009h, 08Bh, 077h, 014h, 068h, 011h, 093h, 000h, 000h, 0CDh, 020h
        db  0F3h, 000h, 001h, 000h, 08Bh, 047h, 030h, 08Bh, 05Fh, 034h, 08Bh, 04Fh, 02Ch, 068h, 023h, 093h
        db  000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0FEh, 0C2h, 08Bh, 047h, 018h, 068h, 03Ah, 093h
        db  000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0FEh, 0C2h, 08Bh, 047h, 01Ch, 00Bh, 0C0h, 074h
        db  010h, 08Bh, 05Fh, 024h, 068h, 054h, 093h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0FEh
        db  0C2h, 08Bh, 047h, 020h, 00Bh, 0C0h, 074h, 010h, 08Bh, 05Fh, 028h, 068h, 075h, 093h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 0FEh, 0C2h, 08Bh, 03Fh, 080h, 0FAh, 014h, 072h, 020h, 0CDh
        db  020h, 0C4h, 000h, 001h, 000h, 074h, 026h, 068h, 096h, 093h, 000h, 000h, 0CDh, 020h, 0F3h, 000h
        db  001h, 000h, 068h, 09Ah, 093h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0B2h, 001h, 00Bh
        db  0FFh, 00Fh, 085h, 02Fh, 0FFh, 0FFh, 0FFh, 0CDh, 020h, 0C4h, 000h, 001h, 000h, 058h, 058h, 09Dh
        db  061h, 0C3h
sub_5_306E:
        enter  0, 0
        push   ecx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, 79h
        int    41h
        pop    esi
        pop    ecx
        leave
        ret
sub_5_3085:
        enter  4, 0
        pushad
        push   es
        push   ss
        pop    es
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [ebp + 0ch]
        mov    ax, 73h
        int    41h
        pop    es
        mov    dword ptr [ebp - 4], eax
        popad
        mov    eax, dword ptr [ebp - 4]
        leave
        ret
        db  000h, 066h, 0C1h, 0C0h, 004h, 0E8h, 01Ch, 000h, 000h, 000h, 066h, 0C1h, 0C0h, 004h, 0E8h, 013h
        db  000h, 000h, 000h, 066h, 0C1h, 0C0h, 004h, 0E8h, 00Ah, 000h, 000h, 000h, 066h, 0C1h, 0C0h, 004h
        db  0E8h, 001h, 000h, 000h, 000h, 0C3h, 050h, 053h, 08Bh, 0D8h, 083h, 0E3h, 00Fh, 00Fh, 0B6h, 083h
        db  0DCh, 093h, 000h, 000h, 0CDh, 020h, 0C3h, 000h, 001h, 000h, 05Bh, 058h, 0C3h, 066h, 083h, 03Dh
        db  0F8h, 0E3h, 000h, 000h, 000h, 077h, 00Dh, 068h, 0ECh, 093h, 000h, 000h, 0CDh, 020h, 0F3h, 000h
        db  001h, 000h, 032h, 0C0h, 0C3h, 060h, 09Ch, 0FAh, 053h, 066h, 0A1h, 0F8h, 0E3h, 000h, 000h, 068h
        db  009h, 094h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0CDh, 020h, 0F1h, 000h, 001h, 000h
        db  0E3h, 01Dh, 068h, 032h, 094h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0A1h, 0D4h, 0F6h
        db  000h, 000h, 068h, 058h, 094h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0EBh, 00Bh, 068h
        db  080h, 094h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 068h, 0ACh, 094h, 000h, 000h, 0CDh
        db  020h, 0F3h, 000h, 001h, 000h, 05Bh, 08Bh, 073h, 008h, 068h, 0B0h, 094h, 000h, 000h, 0CDh, 020h
        db  0F3h, 000h, 001h, 000h, 068h, 0C8h, 094h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 068h
        db  0E0h, 094h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0E8h, 038h, 000h, 000h, 000h, 068h
        db  0F2h, 094h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 068h, 0F6h, 094h, 000h, 000h, 0CDh
        db  020h, 0F3h, 000h, 001h, 000h, 0E8h, 053h, 0FFh, 0FFh, 0FFh, 074h, 018h, 08Bh, 05Bh, 0E0h, 0E8h
        db  0C4h, 000h, 000h, 000h, 068h, 0FAh, 094h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0E8h
        db  0FDh, 001h, 000h, 000h, 09Dh, 061h, 0C3h, 060h, 08Bh, 003h, 068h, 099h, 095h, 000h, 000h, 0CDh
        db  020h, 0F3h, 000h, 001h, 000h, 068h, 0A1h, 095h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  08Bh, 003h, 0B9h, 010h, 000h, 000h, 000h, 0BEh, 000h, 095h, 000h, 000h, 0D1h, 0E8h, 073h, 008h
        db  060h, 0CDh, 020h, 0C2h, 000h, 001h, 000h, 061h, 083h, 0C6h, 009h, 0E2h, 0EFh, 061h, 0C3h, 0C8h
        db  080h, 000h, 000h, 09Ch, 060h, 0FAh, 08Bh, 0FBh, 08Bh, 05Fh, 014h, 08Bh, 057h, 010h, 08Dh, 075h
        db  080h, 0E8h, 05Dh, 001h, 000h, 000h, 083h, 07Fh, 024h, 000h, 074h, 03Bh, 066h, 08Bh, 047h, 01Ah
        db  066h, 089h, 046h, 022h, 08Bh, 047h, 01Ch, 089h, 046h, 010h, 066h, 08Bh, 047h, 028h, 066h, 089h
        db  046h, 02Eh, 08Bh, 047h, 020h, 089h, 046h, 02Ah, 0F7h, 003h, 040h, 000h, 000h, 000h, 074h, 010h
        db  0F7h, 042h, 02Ch, 000h, 000h, 002h, 000h, 074h, 007h, 0E8h, 0EEh, 000h, 000h, 000h, 0EBh, 024h
        db  0E8h, 094h, 000h, 000h, 000h, 0EBh, 01Dh, 0F7h, 003h, 040h, 000h, 000h, 000h, 074h, 010h, 0F7h
        db  042h, 02Ch, 000h, 000h, 002h, 000h, 074h, 007h, 0E8h, 0B3h, 000h, 000h, 000h, 0EBh, 005h, 0E8h
        db  059h, 000h, 000h, 000h, 061h, 09Dh, 0C9h, 0C3h, 0C8h, 080h, 000h, 000h, 09Ch, 060h, 0FAh, 0E8h
        db  079h, 0FEh, 0FFh, 0FFh, 074h, 043h, 08Bh, 0FBh, 08Bh, 05Fh, 014h, 08Bh, 057h, 010h, 08Dh, 075h
        db  080h, 0E8h, 0DDh, 000h, 000h, 000h, 0F7h, 003h, 040h, 000h, 000h, 000h, 074h, 01Bh, 068h, 0AAh
        db  095h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0E8h, 072h, 000h, 000h, 000h, 068h, 0C3h
        db  095h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 068h, 0C7h, 095h, 000h, 000h, 0CDh, 020h
        db  0F3h, 000h, 001h, 000h, 0E8h, 004h, 000h, 000h, 000h, 061h, 09Dh, 0C9h, 0C3h, 066h, 08Bh, 042h
        db  034h, 066h, 089h, 046h, 022h, 08Bh, 042h, 030h, 089h, 046h, 010h, 066h, 08Bh, 042h, 028h, 066h
        db  089h, 046h, 02Eh, 08Bh, 042h, 024h, 089h, 046h, 02Ah, 08Bh, 042h, 02Ch, 089h, 046h, 034h, 066h
        db  08Bh, 042h, 03Ch, 066h, 089h, 046h, 024h, 066h, 08Bh, 042h, 038h, 066h, 089h, 046h, 020h, 066h
        db  08Bh, 042h, 040h, 066h, 089h, 046h, 026h, 066h, 08Bh, 042h, 044h, 066h, 089h, 046h, 028h, 066h
        db  08Bh, 04Fh, 018h, 066h, 0B8h, 076h, 000h, 0CDh, 041h, 066h, 0B8h, 07Ah, 000h, 0CDh, 041h, 0C3h
        db  066h, 08Bh, 042h, 058h, 066h, 089h, 046h, 022h, 08Bh, 042h, 054h, 089h, 046h, 010h, 066h, 08Bh
        db  042h, 04Ch, 066h, 089h, 046h, 02Eh, 08Bh, 042h, 048h, 089h, 046h, 02Ah, 08Bh, 042h, 050h, 089h
        db  046h, 034h, 066h, 08Bh, 042h, 060h, 066h, 089h, 046h, 024h, 066h, 08Bh, 042h, 05Ch, 066h, 089h
        db  046h, 020h, 066h, 08Bh, 042h, 064h, 066h, 089h, 046h, 026h, 066h, 08Bh, 042h, 068h, 066h, 089h
        db  046h, 028h, 066h, 08Bh, 04Fh, 018h, 066h, 0B8h, 076h, 000h, 0CDh, 041h, 066h, 0B8h, 07Ah, 000h
        db  0CDh, 041h, 0C3h, 066h, 0B8h, 075h, 000h, 0CDh, 041h, 08Bh, 042h, 01Ch, 089h, 006h, 08Bh, 042h
        db  010h, 089h, 046h, 004h, 08Bh, 042h, 018h, 089h, 046h, 008h, 08Bh, 042h, 014h, 089h, 046h, 00Ch
        db  08Bh, 042h, 008h, 089h, 046h, 014h, 08Bh, 042h, 004h, 089h, 046h, 018h, 08Bh, 002h, 089h, 046h
        db  01Ch, 066h, 0C7h, 046h, 07Ch, 0FFh, 0FFh, 066h, 0C7h, 046h, 07Eh, 000h, 000h, 066h, 08Bh, 043h
        db  01Ah, 066h, 089h, 046h, 04Ch, 0C3h, 068h, 0DCh, 095h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h
        db  000h, 09Ch, 060h, 0FAh, 0E8h, 034h, 0FDh, 0FFh, 0FFh, 00Fh, 084h, 081h, 000h, 000h, 000h, 08Bh
        db  073h, 010h, 00Fh, 0B7h, 046h, 034h, 00Fh, 0B7h, 07Eh, 030h, 0E8h, 082h, 000h, 000h, 000h, 072h
        db  06Fh, 02Bh, 0C7h, 08Bh, 0D8h, 0B9h, 006h, 000h, 000h, 000h, 00Fh, 0B7h, 046h, 034h, 0E8h, 0CEh
        db  0FCh, 0FFh, 0FFh, 0B0h, 03Ah, 0CDh, 020h, 0C3h, 000h, 001h, 000h, 08Bh, 0C7h, 0E8h, 0BFh, 0FCh
        db  0FFh, 0FFh, 0B0h, 020h, 0CDh, 020h, 0C3h, 000h, 001h, 000h, 051h, 0B9h, 008h, 000h, 000h, 000h
        db  0B0h, 020h, 066h, 03Bh, 07Eh, 008h, 075h, 002h, 0B0h, 03Eh, 0CDh, 020h, 0C3h, 000h, 001h, 000h
        db  051h, 056h, 08Bh, 0F3h, 003h, 0F7h, 0B9h, 002h, 000h, 000h, 000h, 0E8h, 03Bh, 0F5h, 0FFh, 0FFh
        db  05Eh, 059h, 072h, 01Fh, 066h, 08Bh, 004h, 01Fh, 0E8h, 084h, 0FCh, 0FFh, 0FFh, 083h, 0C7h, 002h
        db  0E2h, 0CEh, 068h, 0F0h, 095h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 059h, 0E2h, 09Ah
        db  061h, 09Dh, 0C3h, 068h, 0F4h, 095h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 059h, 0EBh
        db  0EFh, 0F7h, 046h, 02Ch, 000h, 000h, 002h, 000h, 075h, 03Ch, 050h, 0E8h, 039h, 02Ch, 0F8h, 0FFh
        db  08Bh, 00Ch, 024h, 080h, 0E1h, 0F8h, 083h, 0E8h, 008h, 03Bh, 0C8h, 077h, 033h, 003h, 0CAh, 08Ah
        db  049h, 005h, 080h, 0E1h, 090h, 080h, 0F9h, 090h, 058h, 074h, 00Dh, 068h, 007h, 096h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 0EBh, 019h, 06Ah, 000h, 050h, 053h, 0E8h, 0E4h, 022h, 0F8h
        db  0FFh, 083h, 0C4h, 00Ch, 0EBh, 006h, 0C1h, 0E0h, 004h, 003h, 043h, 004h, 003h, 0C7h, 0F8h, 0C3h
        db  058h, 0F9h, 0C3h, 060h, 068h, 021h, 096h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0BAh
        db  000h, 000h, 000h, 000h, 08Bh, 004h, 01Ah, 068h, 04Fh, 096h, 000h, 000h, 0CDh, 020h, 0F3h, 000h
        db  001h, 000h, 0BAh, 004h, 000h, 000h, 000h, 08Bh, 004h, 01Ah, 068h, 071h, 096h, 000h, 000h, 0CDh
        db  020h, 0F3h, 000h, 001h, 000h, 0BAh, 008h, 000h, 000h, 000h, 08Bh, 004h, 01Ah, 068h, 093h, 096h
        db  000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0BAh, 00Ch, 000h, 000h, 000h, 08Bh, 004h, 01Ah
        db  068h, 0B5h, 096h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0BAh, 0F0h, 0FFh, 0FFh, 0FFh
        db  08Bh, 004h, 01Ah, 068h, 0D7h, 096h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0BAh, 090h
        db  0FFh, 0FFh, 0FFh, 08Bh, 004h, 01Ah, 068h, 0F9h, 096h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h
        db  000h, 0BAh, 02Ch, 000h, 000h, 000h, 08Bh, 004h, 01Ah, 068h, 01Bh, 097h, 000h, 000h, 0CDh, 020h
        db  0F3h, 000h, 001h, 000h, 0BAh, 0ECh, 0FFh, 0FFh, 0FFh, 08Bh, 004h, 01Ah, 068h, 03Dh, 097h, 000h
        db  000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0BAh, 0F4h, 0FFh, 0FFh, 0FFh, 08Bh, 004h, 01Ah, 068h
        db  05Fh, 097h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0BAh, 030h, 000h, 000h, 000h, 08Bh
        db  004h, 01Ah, 068h, 081h, 097h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0BAh, 024h, 000h
        db  000h, 000h, 08Bh, 004h, 01Ah, 068h, 0A3h, 097h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0BAh, 01Ah, 000h, 000h, 000h, 066h, 08Bh, 004h, 01Ah, 068h, 0C5h, 097h, 000h, 000h, 0CDh, 020h
        db  0F3h, 000h, 001h, 000h, 0BAh, 0CCh, 0FFh, 0FFh, 0FFh, 066h, 08Bh, 004h, 01Ah, 068h, 0E6h, 097h
        db  000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0BAh, 028h, 000h, 000h, 000h, 08Bh, 004h, 01Ah
        db  068h, 000h, 098h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0BAh, 0FCh, 0FFh, 0FFh, 0FFh
        db  08Bh, 004h, 01Ah, 068h, 01Bh, 098h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0BAh, 014h
        db  000h, 000h, 000h, 08Bh, 004h, 01Ah, 068h, 03Dh, 098h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h
        db  000h, 0BAh, 0E4h, 0FFh, 0FFh, 0FFh, 08Bh, 004h, 01Ah, 068h, 05Fh, 098h, 000h, 000h, 0CDh, 020h
        db  0F3h, 000h, 001h, 000h, 0BAh, 01Ch, 000h, 000h, 000h, 08Bh, 004h, 01Ah, 068h, 081h, 098h, 000h
        db  000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0BAh, 0E8h, 0FFh, 0FFh, 0FFh, 08Bh, 004h, 01Ah, 068h
        db  0A3h, 098h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0BAh, 0E0h, 0FFh, 0FFh, 0FFh, 08Bh
        db  004h, 01Ah, 068h, 0C5h, 098h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0BAh, 0E4h, 0FFh
        db  0FFh, 0FFh, 08Bh, 004h, 01Ah, 068h, 0E7h, 098h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  061h, 0C3h, 060h, 0CDh, 020h, 003h, 000h, 001h, 000h, 00Bh, 0DBh, 074h, 04Ch, 08Bh, 04Bh, 00Ch
        db  03Bh, 01Dh, 00Ch, 0F7h, 000h, 000h, 075h, 00Dh, 068h, 009h, 099h, 000h, 000h, 0CDh, 020h, 0F3h
        db  000h, 001h, 000h, 0EBh, 00Bh, 068h, 00Bh, 099h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  068h, 00Dh, 099h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0E8h, 037h, 0FBh, 0FFh, 0FFh
        db  068h, 023h, 099h, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0CDh, 020h, 03Bh, 000h, 001h
        db  000h, 0CDh, 020h, 004h, 000h, 001h, 000h, 075h, 0B0h, 061h, 0C3h, 000h, 000h, 053h, 051h, 052h
        db  08Bh, 0DEh, 033h, 0C9h, 033h, 0D2h, 064h, 080h, 03Eh, 020h, 075h, 003h, 046h, 0EBh, 0F7h, 064h
        db  08Ah, 006h, 03Ch, 020h, 07Eh, 02Ah, 03Ch, 02Ch, 074h, 026h, 03Ch, 03Bh, 074h, 022h, 03Ch, 030h
        db  07Ch, 029h, 03Ch, 039h, 07Eh, 00Ch, 03Ch, 041h, 07Ch, 021h, 03Ch, 046h, 07Fh, 01Dh, 02Ch, 037h
        db  0EBh, 002h, 02Ch, 030h, 042h, 0C1h, 0E1h, 004h, 083h, 0E0h, 00Fh, 003h, 0C8h, 046h, 0EBh, 0CFh
        db  00Bh, 0D2h, 074h, 007h, 08Bh, 0C1h, 05Ah, 059h, 05Bh, 0F8h, 0C3h, 08Bh, 0F3h, 05Ah, 059h, 05Bh
        db  0F9h, 0C3h, 0E8h, 0A6h, 0FFh, 0FFh, 0FFh, 072h, 02Eh, 08Bh, 0D8h, 073h, 030h, 0CDh, 020h, 003h
        db  000h, 001h, 000h, 03Bh, 043h, 00Ch, 074h, 025h, 0CDh, 020h, 03Bh, 000h, 001h, 000h, 0CDh, 020h
        db  004h, 000h, 001h, 000h, 075h, 0EDh, 08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h, 068h, 044h, 09Bh, 000h
        db  000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h, 0C3h, 0E8h, 06Ah
        db  0FFh, 0FFh, 0FFh, 072h, 01Dh, 052h, 051h, 050h, 0E8h, 050h, 005h, 000h, 000h, 059h, 059h, 05Ah
        db  08Bh, 0D8h, 083h, 0FBh, 000h, 075h, 011h, 068h, 074h, 09Bh, 000h, 000h, 0CDh, 020h, 0F3h, 000h
        db  001h, 000h, 08Bh, 01Dh, 098h, 0F6h, 000h, 000h, 0C3h, 055h, 083h, 0ECh, 00Ch, 08Bh, 0F4h, 0C7h
        db  006h, 067h, 048h, 000h, 000h, 066h, 08Ch, 04Eh, 004h, 089h, 066h, 006h, 066h, 08Ch, 056h, 00Ah
        db  066h, 0B8h, 080h, 000h, 0CDh, 041h, 066h, 08Eh, 065h, 0FCh, 08Bh, 075h, 004h, 064h, 08Ah, 006h
        db  0E8h, 007h, 001h, 000h, 000h, 00Fh, 083h, 0F3h, 000h, 000h, 000h, 03Ch, 03Fh, 074h, 030h, 03Ch
        db  056h, 074h, 069h, 03Ch, 044h, 074h, 038h, 03Ch, 052h, 074h, 051h, 064h, 08Ah, 006h, 046h, 00Ah
        db  0C0h, 074h, 00Ch, 03Ch, 03Bh, 074h, 008h, 0CDh, 020h, 0C3h, 000h, 001h, 000h, 0EBh, 0ECh, 068h
        db  09Eh, 09Bh, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0E9h, 0BFh, 000h, 000h, 000h, 0BEh
        db  028h, 099h, 000h, 000h, 0CDh, 020h, 0C2h, 000h, 001h, 000h, 0E9h, 0AFh, 000h, 000h, 000h, 064h
        db  080h, 07Eh, 001h, 053h, 075h, 0C5h, 083h, 0C6h, 002h, 033h, 0D2h, 064h, 080h, 03Eh, 058h, 075h
        db  001h, 042h, 0E8h, 0F8h, 0F6h, 0FFh, 0FFh, 0E9h, 092h, 000h, 000h, 000h, 046h, 0E8h, 03Ch, 0FFh
        db  0FFh, 0FFh, 0E8h, 0F8h, 0F9h, 0FFh, 0FFh, 0E9h, 082h, 000h, 000h, 000h, 064h, 08Ah, 046h, 001h
        db  03Ch, 04Dh, 075h, 00Fh, 083h, 0C6h, 002h, 0E8h, 0E6h, 0FEh, 0FFh, 0FFh, 0E8h, 0F4h, 0F8h, 0FFh
        db  0FFh, 0EBh, 06Bh, 03Ch, 048h, 075h, 00Ah, 083h, 0C6h, 002h, 0E8h, 03Ah, 0BCh, 0FFh, 0FFh, 0EBh
        db  05Dh, 03Ch, 052h, 075h, 00Fh, 083h, 0C6h, 002h, 0E8h, 001h, 0FFh, 0FFh, 0FFh, 0E8h, 036h, 0FAh
        db  0FFh, 0FFh, 0EBh, 04Ah, 03Ch, 053h, 075h, 00Fh, 083h, 0C6h, 002h, 0E8h, 0EEh, 0FEh, 0FFh, 0FFh
        db  0E8h, 061h, 0FBh, 0FFh, 0FFh, 0EBh, 037h, 03Ch, 043h, 075h, 00Fh, 083h, 0C6h, 002h, 0E8h, 09Fh
        db  0FEh, 0FFh, 0FFh, 0E8h, 04Bh, 0FCh, 0FFh, 0FFh, 0EBh, 024h, 03Ch, 04Ch, 00Fh, 085h, 039h, 0FFh
        db  0FFh, 0FFh, 0E8h, 0DBh, 0FDh, 0FFh, 0FFh, 0EBh, 015h, 068h, 0C8h, 09Bh, 000h, 000h, 0CDh, 020h
        db  0F3h, 000h, 001h, 000h, 066h, 0B8h, 030h, 000h, 066h, 08Eh, 0D8h, 066h, 08Eh, 0C0h, 08Bh, 0F4h
        db  0B8h, 080h, 000h, 000h, 000h, 0CDh, 041h, 083h, 0C4h, 00Ch, 05Dh, 0C3h, 060h, 08Dh, 01Dh, 040h
        db  0C3h, 000h, 000h, 08Bh, 01Bh, 085h, 0DBh, 074h, 042h, 033h, 0C9h, 064h, 08Ah, 004h, 031h, 084h
        db  0C0h, 074h, 018h, 03Ch, 03Bh, 074h, 014h, 03Ch, 020h, 074h, 010h, 08Ah, 054h, 019h, 00Ch, 024h
        db  0DFh, 080h, 0E2h, 0DFh, 038h, 0D0h, 075h, 0DBh, 041h, 0EBh, 0E0h, 083h, 0F9h, 008h, 074h, 007h
        db  080h, 07Ch, 019h, 00Ch, 020h, 075h, 0CCh, 003h, 0F1h, 08Bh, 07Dh, 000h, 0B8h, 014h, 000h, 000h
        db  000h, 00Fh, 0A0h, 0FFh, 053h, 018h, 00Fh, 0A1h, 0F8h, 061h, 0C3h, 0F9h, 061h, 0C3h, 000h, 000h
        db  000h, 08Bh, 045h, 01Ch, 066h, 083h, 0F8h, 00Dh, 00Fh, 087h, 0CAh, 000h, 000h, 000h, 0C1h, 0E0h
        db  002h, 0FFh, 0A0h, 0EAh, 048h, 000h, 000h, 0B8h, 049h, 000h, 000h, 04Ch, 047h, 000h, 000h, 038h
        db  049h, 000h, 000h, 022h, 049h, 000h, 000h, 0C0h, 049h, 000h, 000h, 0C0h, 049h, 000h, 000h, 000h
        db  000h, 000h, 000h, 056h, 04Ah, 000h, 000h, 04Fh, 049h, 000h, 000h, 063h, 049h, 000h, 000h, 074h
        db  049h, 000h, 000h, 09Ah, 049h, 000h, 000h, 0C2h, 002h, 000h, 000h, 0F4h, 002h, 000h, 000h, 08Bh
        db  075h, 004h, 00Fh, 0B7h, 04Dh, 018h, 0E8h, 020h, 0F0h, 0FFh, 0FFh, 073h, 019h, 066h, 033h, 0C0h
        db  0E9h, 084h, 000h, 000h, 000h, 08Bh, 075h, 004h, 00Fh, 0B7h, 04Dh, 018h, 0E8h, 008h, 0F0h, 0FFh
        db  0FFh, 072h, 0EAh, 089h, 045h, 004h, 066h, 0B8h, 001h, 000h, 0EBh, 06Dh, 08Bh, 05Dh, 010h, 08Bh
        db  055h, 014h, 0E8h, 047h, 0CAh, 0FFh, 0FFh, 072h, 060h, 089h, 04Dh, 018h, 033h, 0C0h, 0EBh, 059h
        db  08Bh, 04Dh, 018h, 0E8h, 096h, 0CAh, 0FFh, 0FFh, 072h, 039h, 089h, 055h, 014h, 033h, 0C0h, 0EBh
        db  048h, 08Bh, 05Dh, 010h, 0E8h, 0CCh, 0CAh, 0FFh, 0FFh, 072h, 03Eh, 08Bh, 05Fh, 014h, 08Bh, 07Bh
        db  028h, 00Bh, 0FFh, 074h, 01Eh, 00Fh, 0B7h, 04Bh, 0CCh, 0C1h, 0E1h, 00Ch, 049h, 089h, 07Dh, 000h
        db  089h, 04Dh, 018h, 033h, 0C0h, 0EBh, 022h, 08Bh, 03Dh, 098h, 0F6h, 000h, 000h, 066h, 08Bh, 047h
        db  018h, 0EBh, 016h, 033h, 0C0h, 040h, 0EBh, 011h, 068h, 002h, 09Ch, 000h, 000h, 0CDh, 020h, 0F4h
        db  000h, 001h, 000h, 0EBh, 008h, 066h, 0B8h, 086h, 0F3h, 066h, 089h, 045h, 01Ch, 0C3h
sub_5_39C1:
        db  0A1h, 000h, 0C7h, 000h, 000h
        test   eax, eax
        je     SHORT loc_5_3A25
        db  08Bh, 015h, 004h, 0C7h, 000h, 000h
loc_5_39D0:
        xor    ecx, ecx
        lea    edi, [eax + 5]
        jmp    SHORT loc_5_39E6
loc_5_39D7:
        inc    ecx
        mov    esi, ebp
        mov    ebx, dword ptr [edi]
        sub    esi, ebx
        cmp    esi, dword ptr [edi + 4]
        jb     SHORT loc_5_3A28
        add    edi, 9
loc_5_39E6:
        cmp    cl, byte ptr [eax + 4]
        jb     SHORT loc_5_39D7
        mov    eax, edi
        dec    edx
        jne    SHORT loc_5_39D0
        db  0CDh, 020h, 000h, 000h, 027h, 000h
        jb     SHORT loc_5_3A25
        db  0CDh, 020h, 005h, 000h, 027h, 000h
        jmp    SHORT loc_5_3A21
loc_5_3A00:
        mov    edi, dword ptr [eax + 17h]
        xor    ecx, ecx
        test   edi, edi
        je     SHORT loc_5_3A1F
        jmp    SHORT loc_5_3A1A
loc_5_3A0B:
        inc    ecx
        mov    esi, ebp
        mov    ebx, dword ptr [edi]
        sub    esi, ebx
        cmp    esi, dword ptr [edi + 4]
        jb     SHORT loc_5_3A2B
        add    edi, 10h
loc_5_3A1A:
        cmp    ecx, dword ptr [eax + 13h]
        jb     SHORT loc_5_3A0B
loc_5_3A1F:
        mov    eax, dword ptr [eax]
loc_5_3A21:
        or     eax, eax
        jne    SHORT loc_5_3A00
loc_5_3A25:
        xor    esi, esi
        ret
loc_5_3A28:
        mov    esi, dword ptr [eax]
        ret
loc_5_3A2B:
        mov    esi, dword ptr [eax + 5]
        test   esi, esi
        je     SHORT loc_5_3A25
        js     SHORT loc_5_3A55
        db  0BAh, 0F9h, 09Bh, 000h, 000h
        mov    esi, dword ptr [eax + 0bh]
        push   8
        pop    ecx
loc_5_3A3F:
        inc    edx
        lodsb
        mov    byte ptr [edx], al
        test   al, al
        loopne loc_5_3A3F
        jecxz  loc_5_3A50
        mov    al, 20h
loc_5_3A4B:
        mov    byte ptr [edx], al
        inc    edx
        loop   loc_5_3A4B
loc_5_3A50:
        db  0BEh, 0EEh, 09Bh, 000h, 000h
loc_5_3A55:
        ret
        db  08Bh, 0C4h, 083h, 0E4h, 0FCh, 050h, 006h, 055h, 08Bh, 06Dh, 014h, 0E8h, 05Bh, 0FFh, 0FFh, 0FFh
        db  05Dh, 075h, 008h, 0E8h, 0E3h, 000h, 000h, 000h, 007h, 05Ch, 0C3h, 083h, 065h, 01Ch, 000h, 066h
        db  08Eh, 045h, 0FCh, 08Bh, 07Dh, 004h, 066h, 083h, 07Eh, 006h, 002h, 075h, 022h, 083h, 0F9h, 001h
        db  075h, 01Dh, 08Bh, 045h, 014h, 02Bh, 0C3h, 03Dh, 049h, 001h, 000h, 000h, 075h, 011h, 026h, 0C7h
        db  007h, 043h, 074h, 072h, 06Ch, 026h, 0C7h, 047h, 004h, 02Bh, 043h, 000h, 000h, 0EBh, 0C9h, 089h
        db  05Dh, 014h, 051h, 06Ah, 008h, 059h, 093h, 0FCh, 0E8h, 0C4h, 0BEh, 0F7h, 0FFh, 0B0h, 03Ah, 0AAh
        db  08Bh, 04Eh, 00Ch, 08Bh, 076h, 010h, 026h, 089h, 00Fh, 026h, 089h, 077h, 004h, 06Ah, 008h, 059h
        db  026h, 080h, 07Ch, 039h, 0FFh, 020h, 0E1h, 0F8h, 041h, 003h, 0F9h, 0B0h, 028h, 0AAh, 0B1h, 002h
        db  058h, 0E8h, 09Bh, 0BEh, 0F7h, 0FFh, 066h, 026h, 0C7h, 007h, 029h, 000h, 0EBh, 08Ah, 08Bh, 0C8h
        db  0C1h, 0E8h, 016h, 048h, 03Dh, 0FEh, 002h, 000h, 000h, 077h, 021h, 08Bh, 004h, 085h, 004h, 0E0h
        db  0BFh, 0FFh, 0A8h, 001h, 074h, 016h, 0F6h, 0C4h, 002h, 074h, 011h, 0F6h, 0C4h, 008h, 075h, 00Ch
        db  0C1h, 0E9h, 00Ch, 08Bh, 004h, 08Dh, 000h, 000h, 080h, 0FFh, 0F8h, 0C3h, 0F9h, 0C3h, 08Bh, 044h
        db  024h, 004h, 025h, 0FFh, 00Fh, 000h, 000h, 001h, 044h, 024h, 008h, 08Bh, 044h, 024h, 004h, 0E8h
        db  0BAh, 0FFh, 0FFh, 0FFh, 072h, 021h, 0A8h, 001h, 074h, 01Dh, 0F6h, 0C4h, 002h, 074h, 018h, 0F6h
        db  0C4h, 008h, 075h, 013h, 081h, 044h, 024h, 004h, 000h, 010h, 000h, 000h, 081h, 06Ch, 024h, 008h
        db  000h, 010h, 000h, 000h, 077h, 0D5h, 0A8h, 0F9h, 0C2h, 008h, 000h, 08Bh, 045h, 014h, 0E8h, 08Bh
        db  0FFh, 0FFh, 0FFh, 00Fh, 082h, 09Ah, 000h, 000h, 000h, 0F6h, 0C4h, 008h, 00Fh, 085h, 091h, 000h
        db  000h, 000h, 0F6h, 0C4h, 002h, 00Fh, 084h, 088h, 000h, 000h, 000h, 0A8h, 001h, 075h, 017h, 0C1h
        db  0E8h, 00Ch, 06Bh, 0C0h, 00Ah, 003h, 005h, 0ACh, 0F8h, 000h, 000h, 08Bh, 070h, 006h, 0F6h, 040h
        db  003h, 010h, 074h, 01Eh, 0EBh, 012h, 0C1h, 0E8h, 00Ch, 06Bh, 0F0h, 00Dh, 003h, 035h, 0E4h, 0F7h
        db  000h, 000h, 08Bh, 006h, 085h, 0C0h, 074h, 05Bh, 08Bh, 076h, 004h, 03Dh, 055h, 0F1h, 0FFh, 0FFh
        db  073h, 051h, 080h, 078h, 002h, 009h, 075h, 04Bh, 0C1h, 0EEh, 013h, 081h, 0E6h, 0FFh, 003h, 000h
        db  000h, 083h, 065h, 01Ch, 000h, 006h, 066h, 08Eh, 045h, 0FCh, 08Bh, 07Dh, 004h, 083h, 0C7h, 008h
        db  0FCh, 0B0h, 03Ah, 0AAh, 0E8h, 02Bh, 000h, 000h, 000h, 072h, 005h, 089h, 045h, 014h, 0EBh, 017h
        db  0B8h, 049h, 04Dh, 054h, 045h, 0ABh, 096h, 0B9h, 004h, 000h, 000h, 000h, 0E8h, 090h, 0BDh, 0F7h
        db  0FFh, 026h, 088h, 00Fh, 08Bh, 045h, 014h, 08Bh, 07Dh, 004h, 06Ah, 008h, 059h, 0E8h, 07Fh, 0BDh
        db  0F7h, 0FFh, 007h, 0C3h, 08Bh, 01Dh, 0E8h, 09Bh, 000h, 000h, 085h, 0DBh, 074h, 049h, 06Ah, 004h
        db  053h, 0E8h, 008h, 0FFh, 0FFh, 0FFh, 072h, 03Fh, 08Bh, 01Bh, 08Dh, 01Ch, 0B3h, 06Ah, 004h, 053h
        db  0E8h, 0F9h, 0FEh, 0FFh, 0FFh, 072h, 030h, 08Bh, 01Bh, 06Ah, 030h, 053h, 0E8h, 0EDh, 0FEh, 0FFh
        db  0FFh, 072h, 024h, 0FFh, 073h, 024h, 08Bh, 053h, 010h, 00Fh, 0B7h, 05Bh, 016h, 043h, 053h, 052h
        db  0E8h, 0D9h, 0FEh, 0FFh, 0FFh, 058h, 072h, 00Fh, 080h, 07Ch, 013h, 0FFh, 000h, 075h, 008h, 08Bh
        db  0F2h, 08Bh, 0CBh, 0F3h, 0A4h, 0F8h, 0C3h, 0F9h, 0C3h, 060h, 0CDh, 020h, 0C1h, 000h, 001h, 000h
        db  074h, 016h, 0BEh, 0ECh, 09Bh, 000h, 000h, 066h, 0B8h, 08Dh, 000h, 0CDh, 041h, 066h, 085h, 0C0h
        db  075h, 006h, 089h, 00Dh, 0E8h, 09Bh, 000h, 000h, 061h, 0C3h, 083h, 025h, 0E8h, 09Bh, 000h, 000h
        db  000h, 0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  0A1h, 014h, 0F7h, 000h, 000h
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_5_3C9F
loc_5_3C96:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 2ch]
        mov    dword ptr [ebp - 8], eax
loc_5_3C9F:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_5_3D5F
        push   1c0h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_5_306E
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_5_3CDA
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  068h, 034h, 09Ch, 000h, 000h
        call   sub_5_3085
        add    esp, 8
        sub    eax, eax
        jmp    NEAR PTR loc_5_3D5F
loc_5_3CDA:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax - 1ch]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_5_3CF1
loc_5_3CE8:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 40h]
        mov    dword ptr [ebp - 4], eax
loc_5_3CF1:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_5_3D5A
        push   80h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_306E
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_5_3D30
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  068h, 058h, 09Ch, 000h, 000h
        call   sub_5_3085
        add    esp, 0ch
        sub    eax, eax
        jmp    NEAR PTR loc_5_3D5F
loc_5_3D30:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 18h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jne    NEAR PTR loc_5_3D55
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_5_3D5F
loc_5_3D55:
        jmp    NEAR PTR loc_5_3CE8
loc_5_3D5A:
        jmp    NEAR PTR loc_5_3C96
loc_5_3D5F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  023h, 045h, 042h, 058h, 020h, 056h, 04Dh, 020h, 023h, 041h, 04Ch, 020h, 069h, 073h, 020h, 06Eh
        db  06Fh, 074h, 020h, 075h, 073h, 069h, 06Eh, 067h, 020h, 044h, 050h, 04Dh, 049h, 00Dh, 00Ah, 000h
        db  023h, 045h, 042h, 058h, 020h, 056h, 04Dh, 020h, 023h, 041h, 04Ch, 020h, 02Dh, 020h, 04Eh, 075h
        db  06Dh, 020h, 061h, 070h, 070h, 073h, 020h, 03Dh, 020h, 023h, 043h, 04Ch, 020h, 02Dh, 020h, 071h
        db  075h, 06Fh, 074h, 061h, 020h, 06Ch, 065h, 066h, 074h, 020h, 03Dh, 020h, 023h, 045h, 044h, 058h
        db  00Dh, 00Ah, 000h, 020h, 020h, 023h, 045h, 044h, 049h, 020h, 02Dh, 020h, 050h, 053h, 050h, 020h
        db  03Dh, 020h, 023h, 041h, 058h, 00Dh, 00Ah, 000h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h
        db  020h, 020h, 020h, 040h, 020h, 023h, 045h, 043h, 058h, 020h, 04Ch, 020h, 023h, 045h, 044h, 058h
        db  00Dh, 00Ah, 000h, 020h, 020h, 023h, 045h, 044h, 049h, 020h, 02Dh, 020h, 043h, 06Fh, 072h, 072h
        db  075h, 070h, 074h, 065h, 064h, 021h, 00Dh, 00Ah, 000h, 020h, 00Dh, 00Ah, 000h, 043h, 061h, 06Eh
        db  06Eh, 06Fh, 074h, 020h, 064h, 075h, 06Dh, 070h, 020h, 044h, 050h, 04Dh, 049h, 020h, 069h, 06Eh
        db  066h, 06Fh, 020h, 061h, 074h, 020h, 068h, 061h, 072h, 064h, 077h, 061h, 072h, 065h, 020h, 069h
        db  06Eh, 074h, 065h, 072h, 072h, 075h, 070h, 074h, 020h, 074h, 069h, 06Dh, 065h, 00Dh, 00Ah, 000h
        db  020h, 056h, 038h, 036h, 020h, 04Eh, 075h, 06Dh, 020h, 046h, 072h, 065h, 065h, 020h, 053h, 06Ch
        db  06Fh, 074h, 073h, 03Ah, 023h, 045h, 042h, 058h, 00Dh, 00Ah, 000h, 020h, 00Dh, 00Ah, 000h, 020h
        db  00Dh, 00Ah, 000h, 020h, 020h, 043h, 053h, 03Ah, 049h, 050h, 020h, 020h, 020h, 020h, 020h, 052h
        db  065h, 066h, 020h, 044h, 061h, 074h, 061h, 020h, 020h, 020h, 050h, 072h, 06Fh, 063h, 065h, 064h
        db  075h, 072h, 065h, 00Dh, 00Ah, 000h, 020h, 00Dh, 00Ah, 000h, 023h, 041h, 058h, 03Ah, 023h, 042h
        db  058h, 020h, 020h, 020h, 023h, 045h, 044h, 058h, 020h, 020h, 020h, 000h, 03Fh, 045h, 053h, 049h
        db  020h, 02Dh, 03Eh, 020h, 000h, 03Fh, 045h, 053h, 049h, 00Dh, 00Ah, 000h, 020h, 00Dh, 00Ah, 000h
        db  020h, 00Dh, 00Ah, 000h, 020h, 00Dh, 00Ah, 000h, 020h, 00Dh, 00Ah, 000h, 020h, 020h, 043h, 053h
        db  03Ah, 049h, 050h, 020h, 020h, 020h, 020h, 020h, 052h, 065h, 066h, 020h, 044h, 061h, 074h, 061h
        db  020h, 020h, 020h, 050h, 072h, 06Fh, 063h, 065h, 064h, 075h, 072h, 065h, 00Dh, 00Ah, 000h, 020h
        db  00Dh, 00Ah, 000h, 023h, 041h, 058h, 03Ah, 023h, 042h, 058h, 020h, 020h, 020h, 023h, 045h, 044h
        db  058h, 020h, 020h, 020h, 03Fh, 045h, 053h, 049h, 00Dh, 00Ah, 000h, 020h, 00Dh, 00Ah, 000h, 020h
        db  00Dh, 00Ah, 000h, 020h, 020h, 04Eh, 075h, 06Dh, 020h, 050h, 04Dh, 020h, 042h, 050h, 020h, 046h
        db  072h, 065h, 065h, 020h, 053h, 06Ch, 06Fh, 074h, 073h, 00Dh, 00Ah, 000h, 020h, 00Dh, 00Ah, 000h
        db  020h, 020h, 023h, 045h, 042h, 058h, 00Dh, 00Ah, 000h, 020h, 00Dh, 00Ah, 000h, 020h, 00Dh, 00Ah
        db  000h, 020h, 020h, 043h, 053h, 03Ah, 049h, 050h, 020h, 020h, 020h, 020h, 020h, 052h, 065h, 066h
        db  020h, 044h, 061h, 074h, 061h, 020h, 020h, 020h, 050h, 072h, 06Fh, 063h, 065h, 064h, 075h, 072h
        db  065h, 00Dh, 00Ah, 000h, 020h, 00Dh, 00Ah, 000h, 023h, 041h, 058h, 03Ah, 023h, 042h, 058h, 020h
        db  020h, 020h, 023h, 045h, 044h, 058h, 020h, 020h, 020h, 000h, 03Fh, 045h, 053h, 049h, 020h, 02Dh
        db  03Eh, 020h, 000h, 03Fh, 045h, 053h, 049h, 00Dh, 00Ah, 000h, 020h, 00Dh, 00Ah, 000h, 020h, 00Dh
        db  00Ah, 000h, 000h, 000h, 0CEh, 0FAh, 0CDh, 0FAh, 0CEh, 0FAh, 0CDh, 0FAh, 0CEh, 0FAh, 0CDh, 0FAh
        db  0CEh, 0FAh, 0CDh, 0FAh, 0CEh, 0FAh, 0CDh, 0FAh, 0CEh, 0FAh, 0CDh, 0FAh, 0CEh, 0FAh, 0CDh, 0FAh
        db  0CEh, 0FAh, 0CDh, 0FAh, 0CEh, 0FAh, 0CDh, 0FAh, 0CEh, 0FAh, 0CDh, 0FAh, 0CEh, 0FAh, 0CDh, 0FAh
        db  0CEh, 0FAh, 0CDh, 0FAh, 0CEh, 0FAh, 0CDh, 0FAh, 0CEh, 0FAh, 0CDh, 0FAh
_AddFreePhysPage:
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        into
        cli
        int    0fah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   ebp
        db  073h, 061h
        cmp    ah, byte ptr gs:[bx + si]
        and    byte ptr [esi], ch
        push   esi
        dec    eax
        pop    ebx
        inc    edx
        jl     SHORT loc_5_4146
        db  07Ch, 044h
        pop    ebp
        and    byte ptr [eax + 6ch], ch
        imul   esi, dword ptr [ebx + 74h], 20200a0dh
        inc    edx
        db  07Ch, 057h
        db  07Ch, 044h
        and    byte ptr [6d754420h], bh
        db  070h, 020h
        popad
        jae    SHORT loc_5_412C
        bound  edi, qword ptr [ecx + 74h]
        db  065h, 073h, 02Ch
        and    byte ptr [edi + 6fh], dh
        db  072h, 064h
        jae    SHORT loc_5_4139
        outsd
        db  072h, 020h
        ja     NEAR PTR loc_5_418E
        jb     SHORT loc_5_4185
        db  073h, 020h
        sub    byte ptr [ebp + 66h], ah
        popad
        db  075h, 06Ch
        db  074h, 03Dh
loc_5_412C:
        db  064h, 077h, 06Fh
        jb     SHORT loc_5_4195
        jae    SHORT loc_5_415C
        or     eax, 6820200ah
        insb
loc_5_4139:
        imul   esi, dword ptr [ebx + 74h], 56203d20h
        dec    ebp
        dec    ebp
        and    byte ptr [ecx + ebp*2 + 73h], ch
loc_5_4146:
        db  074h, 020h
        push   6c646e61h
        or     eax, 1000ah
        add    byte ptr [eax], al
        and    eax, dword ptr [ecx + 4ch]
        and    byte ptr [eax], al
        add    al, byte ptr [eax]
loc_5_415C:
        add    byte ptr [eax], al
        and    eax, dword ptr [ecx + 58h]
        and    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        and    eax, dword ptr [ebp + 41h]
        pop    eax
        and    byte ptr [eax], al
        dec    esp
        imul   ebp, dword ptr [esi + 6bh], 6c206465h
        imul   esi, dword ptr [ebx + 74h], 6d616420h
        popad
        and    byte ptr fs:[bx + di + 74h], ah
        and    byte ptr [ebx], ah
loc_5_4185:
        inc    ebp
        push   ebx
        dec    ecx
        and    byte ptr [edi + 72h], ch
        and    byte ptr [eax + 61h], dh
loc_5_418E:
        and    byte ptr gs:[bp + 6fh], ch
        db  074h, 020h
loc_5_4195:
        jo     SHORT loc_5_4209
        jae    NEAR PTR loc_5_41FF
        outsb
        db  074h, 00Dh
        or     al, byte ptr [eax]
        dec    esp
        imul   esi, dword ptr [ebx + 74h], 6e616820h
        insb
        and    byte ptr gs:[ebx], ah
        inc    ebp
        push   ebx
        dec    ecx
        cmp    ah, byte ptr [eax]
        dec    esi
        outsd
        and    byte ptr gs:[ebx + 69h], dh
        jp     SHORT loc_5_421F
        cmp    eax, 2c584323h
        and    byte ptr [esi + 6ch], al
        popad
        jae    NEAR PTR loc_5_4203
        and    eax, dword ptr [ebp + 44h]
        pop    eax
        or     eax, 4523000ah
        inc    ecx
        pop    eax
        and    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_41FF:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_4203:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_4209:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_421F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
_SetLastV86Page:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_5_45E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_Allocate_Global_V86_Data_Area:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], ah
        inc    ecx
        dec    esp
        and    byte ptr [eax], ah
        and    eax, dword ptr [ebx + 58h]
        cmp    ah, byte ptr [ebx]
        inc    ebp
        inc    edx
        pop    eax
        and    byte ptr [eax], ah
        add    byte ptr [edi], bh
        inc    ebp
        push   ebx
        dec    ecx
        add    byte ptr [edi], bh
        inc    ebx
        pop    eax
        cmp    al, byte ptr [ebp + 42h]
        pop    eax
        add    byte ptr [ebx], ah
        inc    ecx
        dec    esp
        and    byte ptr [eax], ah
        and    eax, dword ptr [ebx + 58h]
        cmp    ah, byte ptr [ebx]
        inc    edx
        pop    eax
        and    byte ptr [eax], ah
        add    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        add    byte ptr [edi], bh
        inc    ebp
        inc    esp
        pop    eax
        add    byte ptr [edi + ecx*2 + 4eh], al
        inc    ebp
        and    byte ptr cs:[eax], ah
        push   eax
        jb     SHORT loc_5_49D3
        jae    SHORT loc_5_49E3
        and    byte ptr [ecx + 6eh], ah
        db  079h, 020h
        imul   esp, dword ptr [ebp + 79h], 20h
        db  074h, 06Fh
        and    byte ptr [ebx + 6fh], ah
        outsb
        db  074h, 069h
        outsb
        jne    SHORT loc_5_49E9
        add    byte ptr cs:[eax], ah
        or     eax, 415b000ah
        pop    ebp
        and    byte ptr [ebp + 6eh], al
        popad
        bound  ebp, qword ptr [ebp + 20h]
        inc    ebx
        dec    esp
        dec    ecx
        das
        push   ebx
        push   esp
        dec    ecx
        and    byte ptr [edx + esi*2 + 61h], dh
        arpl   word ptr [ecx + 6eh], bp
        or     eax, 415b000ah
        pop    ebp
        and    byte ptr [ecx + ebp*2 + 73h], al
        popad
        bound  ebp, qword ptr [ebp + 20h]
        inc    ebx
        dec    esp
        dec    ecx
        das
        push   ebx
        push   esp
        dec    ecx
        and    byte ptr [edx + esi*2 + 61h], dh
        arpl   word ptr [ecx + 6eh], bp
        or     eax, 425b000ah
        pop    ebp
        and    byte ptr [ecx + ebp*2 + 73h], al
        jo     SHORT loc_5_4A3C
        popad
        jns    SHORT loc_5_49F3
loc_5_49D3:
        inc    ebx
        dec    esp
        dec    ecx
        das
        push   ebx
        push   esp
        dec    ecx
        and    byte ptr [edx + esi*2 + 61h], dh
        arpl   word ptr [ebp + 0dh], sp
        or     al, byte ptr [eax]
loc_5_49E3:
        pop    ebx
        inc    ebx
        pop    ebp
        and    byte ptr [edx + 65h], dl
loc_5_49E9:
        db  073h, 065h
        db  074h, 020h
        inc    ebx
        dec    esp
        dec    ecx
        das
        push   ebx
        push   esp
loc_5_49F3:
        dec    ecx
        and    byte ptr [edx + esi*2 + 61h], dh
        arpl   word ptr [ebp + 0dh], sp
        or     al, byte ptr [eax]
        inc    ebp
        outsb
        db  074h, 065h
        db  072h, 020h
        jae    SHORT loc_5_4A6A
        insb
        arpl   word ptr gs:[ecx + ebp*2 + 6fh], si
        outsb
        and    byte ptr [edi + 72h], ch
        and    byte ptr [ebx + 45h], bl
        push   ebx
        inc    ebx
        pop    ebp
        and    byte ptr [edi + ebp*2 + 20h], dh
        js     NEAR PTR loc_5_4A85
        db  074h, 03Ah
        and    byte ptr [eax], al
        and    byte ptr [7250000ah], cl
        outsd
        imul   bp, word ptr [ebp + 20h], 666fh
        and    byte ptr [ebx + 4ch], al
        dec    ecx
        db  073h, 020h
        db  066h, 072h, 06Fh
        insd
        and    byte ptr [eax + 72h], dh
        outsd
loc_5_4A3C:
        db  074h, 065h
        arpl   word ptr [ebp + 64h], si
        and    byte ptr [ebp + 6fh], ch
        cmp    al, byte ptr gs:[eax]
        push   eax
        jb     SHORT loc_5_4ABB
        imul   bp, word ptr [ebp + 20h], 666fh
        and    byte ptr [ebx + 54h], dl
        dec    ecx
        jae    SHORT loc_5_4A79
        db  066h, 072h, 06Fh
        insd
        and    byte ptr [eax + 72h], dh
        outsd
        je     SHORT loc_5_4AC8
        arpl   word ptr [ebp + 64h], si
        and    byte ptr [ebp + 6fh], ch
loc_5_4A6A:
        cmp    al, byte ptr gs:[eax]
        dec    esp
        outsd
        and    byte ptr [bx + 66h], ch
        and    byte ptr [ebx + 4ch], al
        dec    ecx
        das
loc_5_4A79:
        push   ebx
        push   esp
        dec    ecx
        db  073h, 020h
        jb     NEAR PTR loc_5_4AF0
        insd
        and    byte ptr [eax + 72h], dh
loc_5_4A85:
        outsd
        je     SHORT loc_5_4AED
        arpl   word ptr [ebp + 64h], si
        and    byte ptr [ebp + 6fh], ch
        and    byte ptr gs:[eax], ch
        insd
        outsd
        db  073h, 074h
        and    byte ptr [edx + 65h], dh
        arpl   word ptr [ebp + 6eh], sp
        db  074h, 020h
        imul   si, word ptr [edx + 73h], 2974h
        cmp    al, byte ptr [eax]
        and    eax, dword ptr [eax + ebx*2 + 3ah]
        and    eax, dword ptr [edx + 58h]
        and    byte ptr [eax], ah
        and    byte ptr [ebx + 4ch], al
        dec    ecx
        add    byte ptr [ebx], ah
        inc    esp
        pop    eax
        cmp    ah, byte ptr [ebx]
        inc    edx
loc_5_4ABB:
        pop    eax
        and    byte ptr [eax], ah
        and    byte ptr [ebx + 54h], dl
        dec    ecx
        add    byte ptr [ebx], ah
        inc    ebx
        pop    eax
        cmp    ah, byte ptr [ebx]
loc_5_4AC8:
        inc    edx
        pop    eax
        and    byte ptr [eax], ah
        and    byte ptr [ebx], ah
        inc    ebp
        inc    ecx
        pop    eax
        add    byte ptr [eax], ah
        or     eax, 0d20000ah
        or     al, byte ptr [eax]
        push   eax
        db  072h, 065h
        db  073h, 073h
        and    byte ptr [ecx + 6eh], ah
        jns    SHORT _Add_Global_V86_Data_Area
        imul   esp, dword ptr [ebp + 79h], 20h
        je     SHORT loc_5_4B59
        and    byte ptr [ebx + 6fh], ah
loc_5_4AED:
        outsb
        je     SHORT loc_5_4B59
loc_5_4AF0:
        outsb
        db  075h, 065h
        sub    al, 20h
        pop    ebx
        inc    ebp
        push   ebx
        inc    ebx
        pop    ebp
        and    byte ptr [edi + ebp*2 + 20h], dh
        db  065h, 078h, 069h
        db  074h, 02Eh
        db  02Eh
_Add_Global_V86_Data_Area:
        add    byte ptr cs:[eax], ah
        or     eax, 0d20000ah
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        inc    edi
        dec    esp
        dec    edi
        inc    edx
        inc    ecx
        dec    esp
        and    byte ptr [ebp + 56h], al
        inc    ebp
        dec    esi
        push   esp
        and    byte ptr [ecx + ecx*2 + 53h], cl
        push   esp
        and    byte ptr [eax], ch
        and    eax, dword ptr [ecx + 58h]
        and    byte ptr [eax + 65h], dh
        outsb
        imul   ebp, dword ptr fs:[esi + 67h], 0a0d29h
        inc    ebp
        push   esi
        inc    ebp
        dec    esi
        push   esp
        and    byte ptr [ecx + ecx*2 + 53h], cl
        push   esp
        and    byte ptr [esi + 4fh], al
        push   edx
        and    byte ptr [esi + 4dh], dl
        and    byte ptr [ebx], ah
        inc    ebp
        inc    edx
        pop    eax
        and    byte ptr [eax], ch
        and    eax, dword ptr [ecx + 58h]
        and    byte ptr [eax + 65h], dh
        outsb
        imul   ebp, dword ptr fs:[esi + 67h], 0a0d29h
loc_5_4B59:
        inc    ebp
        push   esi
        inc    ebp
        dec    esi
        push   esp
        and    byte ptr [ecx + ecx*2 + 53h], cl
        push   esp
        and    byte ptr [esi + 4fh], al
        push   edx
        and    byte ptr [eax + ecx*2 + 52h], dl
        inc    ebp
        inc    ecx
        inc    esp
        and    byte ptr [ebx], ah
        inc    ebp
        inc    esp
        dec    ecx
        and    byte ptr [eax], ch
        and    eax, dword ptr [ecx + 58h]
        and    byte ptr [eax + 65h], dh
        outsb
        imul   ebp, dword ptr fs:[esi + 67h], 0a0d29h
        inc    esi
        jb     SHORT loc_5_4BEC
        and    byte ptr gs:[ecx + ebp*2 + 73h], ch
        je     SHORT loc_5_4BAE
        imul   esi, dword ptr [ebx + 20h], 63726963h
        db  075h, 06Ch
        popad
        jb     SHORT loc_5_4BBA
        popad
        je     SHORT loc_5_4BBD
        and    eax, dword ptr [ebp + 41h]
        pop    eax
        or     eax, 6523000ah
        popad
        js     SHORT loc_5_4BE3
        and    byte ptr [ebx], ah
        inc    ebp
        inc    esp
        pop    eax
loc_5_4BAE:
        sub    al, 20h
        and    esp, dword ptr [ebp + 64h]
        imul   ebp, dword ptr [eax], 636f7270h
loc_5_4BBA:
        and    byte ptr [edi], bh
        inc    ebp
loc_5_4BBD:
        inc    edx
        pop    eax
        or     eax, 5220000ah
        dec    ecx
        dec    esi
        inc    edi
        xor    byte ptr [eax], al
        and    byte ptr [ecx + 73h], ch
        je     SHORT loc_5_4BCF
loc_5_4BCF:
        and    byte ptr [edi + 41h], dl
        dec    ecx
        push   esp
        and    byte ptr [esi + 4fh], al
        push   edx
        cmp    ah, byte ptr [eax]
        add    byte ptr [ebx + 74h], dh
        imul   esp, dword ptr [eax], 746f6e00h
loc_5_4BE3:
        pop    edi
        arpl   word ptr [edx + 69h], si
        je     SHORT loc_5_4C09
        add    byte ptr [esi + 6fh], ch
loc_5_4BEC:
        db  074h, 05Fh
        push   6e695f77h
        db  074h, 020h
        add    byte ptr [esi + 6fh], ch
        je     SHORT loc_5_4C59
        outsb
        jae    NEAR PTR loc_5_4C72
        pop    edi
        js     NEAR PTR loc_5_4C69
        arpl   word ptr [eax], sp
        add    byte ptr [ecx + 6eh], ch
loc_5_4C09:
        pop    edi
        db  070h, 06Dh
        and    byte ptr [eax], al
        je     SHORT loc_5_4C78
        pop    edi
        db  073h, 074h
        imul   esp, dword ptr [eax], 69726300h
        je     SHORT loc_5_4C3B
        add    byte ptr [ebx + 72h], ah
        imul   esi, dword ptr [esi + esi*2 + 6dh], 0d200020h
        or     al, byte ptr [eax]
        and    byte ptr [454e000ah], cl
        and    byte ptr [eax], ah
        and    eax, dword ptr [ebp + 53h]
        dec    ecx
        and    byte ptr [ebx], ah
        inc    ebp
        inc    ebx
        pop    eax
        and    byte ptr [ebx], ah
loc_5_4C3B:
        inc    ebp
        inc    edx
        push   eax
        and    byte ptr [edi], bh
        inc    ebp
        inc    edx
        push   eax
        or     eax, 0ah
        inc    ebx
        push   edx
        xor    esp, dword ptr [eax]
        and    eax, dword ptr [ebp + 43h]
        pop    eax
        and    byte ptr [ebx], ah
        inc    ebp
        push   ebx
        dec    ecx
        and    byte ptr [ebx], ah
        inc    ebp
        inc    edx
loc_5_4C59:
        push   eax
        and    byte ptr [edi], bh
        inc    ebp
        inc    edx
        push   eax
        or     eax, 2041000ah
        db  077h, 069h
        outsb
        outsd
loc_5_4C69:
        db  077h, 020h
        imul   esi, dword ptr [ebx + 20h], 20746f6eh
loc_5_4C72:
        jb     SHORT loc_5_4CD9
        jae    SHORT loc_5_4CE6
        outsd
        outsb
loc_5_4C78:
        imul   ebp, dword ptr fs:[esi + 67h], 206f7420h
        inc    edx
        db  072h, 06Fh
        popad
        arpl   word ptr fs:[ecx + 73h], sp
        db  074h, 053h
        db  079h, 073h
        je     SHORT loc_5_4CF3
        insd
        dec    ebp
        db  065h, 073h, 073h
        popad
        cmp    esp, dword ptr gs:[bx + si]
        db  074h, 079h
        jo     SHORT loc_5_4D01
        and    byte ptr [eax + 2eh], ah
        push   276d7362h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    ecx
        db  073h, 079h
        outsb
        arpl   word ptr [eax], sp
        push   esp
        imul   ebp, dword ptr [ebp + 65h], 74754f20h
        and    byte ptr [ecx + 75h], dl
        db  065h, 075h, 065h
        and    byte ptr [eax], ch
        and    eax, dword ptr [ebp + 41h]
        pop    eax
        sub    dword ptr [edx], edi
        or     eax, 0d20000ah
        or     al, byte ptr [eax]
        inc    edi
        insb
        outsd
        bound  esp, qword ptr [ecx + 6ch]
        and    byte ptr [ecx + ebp*2 + 6dh], dl
        and    byte ptr gs:[edi + 75h], cl
loc_5_4CD9:
        db  074h, 020h
        push   ecx
        jne    SHORT loc_5_4D43
        jne    SHORT loc_5_4D45
        and    byte ptr [eax], ch
        and    eax, dword ptr [ebp + 41h]
        pop    eax
loc_5_4CE6:
        sub    dword ptr [edx], edi
        or     eax, 0d20000ah
        or     al, byte ptr [eax]
        push   esi
        dec    ebp
        and    byte ptr [ebx], ah
loc_5_4CF3:
        inc    ebp
        inc    edx
        pop    eax
        and    byte ptr [ecx + ebp*2 + 6dh], dl
        and    byte ptr gs:[edi + 75h], cl
        db  074h, 020h
        push   ecx
loc_5_4D01:
        db  075h, 065h
        jne    SHORT loc_5_4D6A
        and    byte ptr [eax], ch
        and    eax, dword ptr [ebp + 41h]
        pop    eax
        sub    dword ptr [edx], edi
        or     eax, 0d20000ah
        or     al, byte ptr [eax]
        push   esp
        push   64616572h
        and    byte ptr [ebx], ah
        inc    ebp
        inc    esp
        dec    ecx
        and    byte ptr [ecx + ebp*2 + 6dh], dl
        and    byte ptr gs:[edi + 75h], cl
        db  074h, 020h
        push   ecx
        db  075h, 065h
        jne    SHORT loc_5_4D93
        and    byte ptr [eax], ch
        and    eax, dword ptr [ebp + 41h]
        pop    eax
        sub    dword ptr [edx], edi
        or     eax, 0d20000ah
        or     al, byte ptr [eax]
        dec    esi
        outsd
        and    byte ptr gs:[eax], ah
loc_5_4D43:
        and    byte ptr [eax], ah
loc_5_4D45:
        and    byte ptr [eax + 72h], dl
        jbe    NEAR PTR loc_5_4D6B
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        inc    esp
        insb
        je     SHORT loc_5_4DB5
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        push   edx
        and    byte ptr gs:[eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        push   eax
        db  072h, 06Fh
        arpl   word ptr [4523000ah], cx
loc_5_4D6A:
        push   ebx
loc_5_4D6B:
        dec    ecx
        and    byte ptr [ebx], ah
        inc    ebp
        inc    ecx
        pop    eax
        and    byte ptr [ebx], ah
        inc    ebp
        inc    edx
        pop    eax
        and    byte ptr [ebx], ah
        inc    ebp
        inc    ebx
        pop    eax
        and    byte ptr [edi], bh
        inc    ebp
        inc    esp
        pop    eax
        or     eax, 4900000ah
        das
        dec    edi
        and    byte ptr [eax + 65h], dl
        jb     SHORT loc_5_4DF9
        imul   esi, dword ptr [ebx + 73h], 206e6f69h
loc_5_4D93:
        dec    ebp
        popad
        jo     SHORT loc_5_4DB7
        popad
        db  064h, 064h, 072h, 020h
        and    byte ptr [eax], ah
        cmp    eax, 41452320h
        pop    eax
        and    byte ptr [eax], ch
        xor    al, 4bh
        and    byte ptr [edx + 69h], ah
        db  074h, 073h
        and    byte ptr [edi + ebp*2 + 6eh], ch
        sub    dword ptr [di], ecx
loc_5_4DB5:
        or     al, byte ptr [eax]
loc_5_4DB7:
        dec    ebp
        popad
        db  078h, 020h
        and    esp, dword ptr [eax]
        jo     SHORT loc_5_4E2E
        db  072h, 074h
        and    byte ptr [edx + esi*2 + 61h], dh
        db  070h, 073h
        and    byte ptr [ecx + 6ch], ah
        insb
        outsd
        db  077h, 065h
        and    byte ptr fs:[eax], ah
        cmp    eax, 43452320h
        pop    eax
        or     eax, 664f000ah
        db  066h, 073h, 065h
        db  074h, 020h
        outsd
        and    byte ptr [ecx + 4fh], cl
        pop    edi
        push   eax
        outsd
        db  072h, 074h
        pop    edi
        push   esp
        popad
        bound  ebp, qword ptr [ebp + 20h]
        and    byte ptr [eax], ah
        cmp    eax, 53452320h
loc_5_4DF9:
        dec    ecx
        or     eax, 664f000ah
        db  066h, 073h, 065h
        je     SHORT loc_5_4E24
        outsd
        and    byte ptr [ecx + 4fh], cl
        pop    edi
        push   eax
        db  072h, 06Fh
        arpl   word ptr [edi + 54h], bx
        popad
        bound  ebp, qword ptr [ebp + 20h]
        and    byte ptr [eax], ah
        cmp    eax, 44452320h
        dec    ecx
        or     eax, 0d20000ah
        or     al, byte ptr [eax]
loc_5_4E24:
        push   eax
        db  072h, 065h
        jae    SHORT loc_5_4E9C
        and    byte ptr [ecx + 6eh], ah
        jns    SHORT loc_5_4E4E
loc_5_4E2E:
        imul   esp, dword ptr [ebp + 79h], 20h
        je     SHORT loc_5_4EA3
        and    byte ptr [ebx + 65h], dh
        and    byte ptr gs:[ecx + 2fh], cl
        dec    edi
        and    byte ptr [edx + esi*2 + 61h], dh
        db  070h, 020h
        imul   ebp, dword ptr [esi + 66h], 726f206fh
        and    byte ptr [ebx + 45h], bl
        push   ebx
        inc    ebx
loc_5_4E4E:
        pop    ebp
        and    byte ptr [edi + ebp*2 + 20h], dh
        jae    SHORT loc_5_4EC0
        imul   esi, dword ptr [eax + 2eh], 20002e2eh
        or     eax, 6f50000ah
        db  072h, 074h
        and    byte ptr [eax], ah
        push   ebx
        db  074h, 061h
        db  074h, 065h
        and    byte ptr [eax + 61h], cl
        db  073h, 068h
        and    byte ptr [edx + 75h], al
        arpl   word ptr [ebx + 65h], bp
        je     SHORT loc_5_4E97
        dec    edi
        and    byte ptr [eax + 72h], dl
        outsd
        arpl   word ptr [4223000ah], cx
        pop    eax
        and    byte ptr [eax], ah
        add    byte ptr [eax], ah
        dec    edi
        dec    esi
        add    byte ptr [edi + 66h], ch
        add    byte ptr [eax], ah
        and    byte ptr [ebx], ah
        inc    esp
        dec    ecx
        and    byte ptr [eax], ah
loc_5_4E97:
        and    eax, dword ptr [ebx + 58h]
        and    byte ptr [eax], ah
loc_5_4E9C:
        and    eax, dword ptr [esp + ecx*2 + 20h]
        and    byte ptr [edi], bh
        inc    ebp
loc_5_4EA3:
        push   ebx
        dec    ecx
        or     eax, 2020000ah
        and    byte ptr [edx + esi*2 + 61h], dl
        db  070h, 070h
        imul   ebp, dword ptr [esi + 67h], 616e6520h
        bound  ebp, qword ptr [ebp + 64h]
        and    byte ptr [esi + 6fh], ah
        db  072h, 020h
loc_5_4EC0:
        push   esi
        dec    ebp
        and    byte ptr [ebx], ah
        inc    ebp
        inc    edx
        pop    eax
        or     eax, 2020000ah
        and    byte ptr [edx + esi*2 + 61h], dl
        db  070h, 070h
        imul   ebp, dword ptr [esi + 67h], 73696420h
        popad
        bound  ebp, qword ptr [ebp + 64h]
        and    byte ptr [esi + 6fh], ah
        db  072h, 020h
        push   esi
        dec    ebp
        and    byte ptr [ebx], ah
        inc    ebp
        inc    edx
        pop    eax
        or     eax, 0d20000ah
        or     al, byte ptr [eax]
        push   eax
        db  072h, 065h
        db  073h, 073h
        and    byte ptr [ecx + 6eh], ah
        jns    SHORT loc_5_4F1B
        imul   esp, dword ptr [ebp + 79h], 20h
        db  074h, 06Fh
        and    byte ptr [ebx + 65h], dh
        and    byte ptr gs:[esi + 4dh], dl
        and    byte ptr [ecx + 2fh], cl
        dec    edi
        and    byte ptr [edx + esi*2 + 61h], dh
        arpl   word ptr [ebp + 20h], sp
        outsd
        db  072h, 020h
        pop    ebx
        inc    ebp
        push   ebx
        inc    ebx
        pop    ebp
loc_5_4F1B:
        and    byte ptr [edi + ebp*2 + 20h], dh
        db  073h, 06Bh
        imul   esi, dword ptr [eax + 2eh], 20002e2eh
        or     eax, 6f50000ah
        db  072h, 074h
        and    byte ptr [eax], ah
        and    byte ptr [ebx + 53h], al
        cmp    cl, byte ptr [ecx + 50h]
        or     eax, 4123000ah
        pop    eax
        and    byte ptr [ebx], ah
        inc    esp
        pop    eax
        cmp    al, byte ptr [eax]
        and    eax, dword ptr [ecx + 58h]
        or     eax, 0d20000ah
        or     al, byte ptr [eax]
        push   eax
        jb     SHORT loc_5_4FB5
        jae    SHORT loc_5_4FC5
        and    byte ptr [ecx + 6eh], ah
        db  079h, 020h
        imul   esp, dword ptr [ebp + 79h], 20h
        db  074h, 06Fh
        and    byte ptr [ebx + 65h], dh
        and    byte ptr gs:[ecx + 2fh], cl
        dec    edi
        and    byte ptr [eax + 65h], dh
        db  072h, 06Dh
        imul   esi, dword ptr [ebx + 73h], 206e6f69h
        insd
        popad
        db  070h, 020h
        outsd
        jb     SHORT loc_5_4F98
        pop    ebx
        inc    ebp
        push   ebx
        inc    ebx
        pop    ebp
        and    byte ptr [edi + ebp*2 + 20h], dh
        db  065h, 078h, 069h
        je     SHORT loc_5_4FB4
        add    byte ptr cs:[eax], ah
        or     eax, 0d20000ah
        or     al, byte ptr [eax]
        and    byte ptr [4f43000ah], cl
        dec    ebp
loc_5_4F98:
        push   eax
        dec    esp
        inc    ebp
        push   esp
        inc    ebp
        and    byte ptr [ebp + edx*2 + 4dh], al
sub_5_4FA1:
        push   eax
        and    byte ptr [edi + 46h], cl
        and    byte ptr [ebx + 55h], al
        push   edx
        push   edx
        inc    ebp
        dec    esi
        push   esp
        and    byte ptr [ecx + 2fh], cl
        dec    edi
        and    byte ptr [eax + 45h], dl
loc_5_4FB4:
        push   edx
loc_5_4FB5:
        dec    ebp
        dec    ecx
        push   ebx
        push   ebx
        dec    ecx
        dec    edi
        dec    esi
        and    byte ptr [ebp + 41h], cl
        push   eax
        or     eax, 6c41000ah
loc_5_4FC5:
        insb
        and    byte ptr [edx + 69h], ah
        je     SHORT loc_5_503E
        and    byte ptr [ecx + 72h], ah
        and    byte ptr gs:[ebp + esi*2 + 6dh], ah
        jo     SHORT loc_5_503A
        and    byte ptr fs:[edi + ebp*2 + 77h], ch
        db  065h, 073h, 074h
        and    byte ptr [edi + ebp*2 + 20h], dh
        push   65686769h
        jae    SHORT loc_5_505C
        and    byte ptr [eax], ch
        xor    byte ptr [eax], ah
        sub    eax, 29343620h
        or     eax, 0d20000ah
        or     al, byte ptr [eax]
        and    byte ptr [2020000ah], cl
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        xor    byte ptr [eax], dh
        xor    byte ptr [eax], dh
        xor    byte ptr [eax], dh
        xor    byte ptr [eax], dh
        and    byte ptr [eax], dh
        xor    byte ptr [eax], dh
        xor    byte ptr [eax], dh
        xor    byte ptr [eax], dh
        xor    byte ptr [eax], ah
        xor    dword ptr [ecx], esi
        xor    dword ptr [ecx], esi
        xor    dword ptr [ecx], esi
        xor    dword ptr [ecx], esi
        and    byte ptr [ecx], dh
        xor    dword ptr [ecx], esi
        xor    dword ptr [ecx], esi
        xor    dword ptr [ecx], esi
        xor    dword ptr [eax], esp
        xor    dh, byte ptr [edx]
        xor    dh, byte ptr [edx]
        xor    dh, byte ptr [edx]
        xor    dh, byte ptr [edx]
        xor    ah, byte ptr [eax]
        xor    dh, byte ptr [edx]
        xor    dh, byte ptr [edx]
        xor    dh, byte ptr [edx]
        xor    ah, byte ptr [eax]
        xor    esi, dword ptr [ebx]
loc_5_503A:
        xor    esi, dword ptr [ebx]
        xor    esi, dword ptr [ebx]
loc_5_503E:
        xor    esi, dword ptr [ebx]
        and    byte ptr [ebx], dh
        xor    esi, dword ptr [ebx]
        xor    esi, dword ptr [ebx]
        xor    esi, dword ptr [ebx]
        xor    ecx, dword ptr [2020000ah]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        xor    byte ptr [ecx], dh
        xor    dh, byte ptr [ebx]
        xor    al, 35h
        aaa
        and    byte ptr [eax], bh
loc_5_505C:
        cmp    dword ptr [ecx + 42h], eax
        inc    ebx
        inc    esp
        inc    ebp
        inc    esi
        and    byte ptr [eax], dh
        xor    dword ptr [edx], esi
        xor    esi, dword ptr [esi + 38203736h]
        cmp    dword ptr [ecx + 42h], eax
        inc    ebx
        inc    esp
        inc    ebp
        inc    esi
        and    byte ptr [eax], dh
        xor    dword ptr [edx], esi
        xor    esi, dword ptr [esi + 20383736h]
        cmp    dword ptr [ecx + 42h], eax
        inc    ebx
        inc    esp
        inc    ebp
        inc    esi
        and    byte ptr [eax], dh
        xor    dword ptr [edx], esi
        xor    esi, dword ptr [esi + 38203736h]
        cmp    dword ptr [ecx + 42h], eax
        inc    ebx
        inc    esp
        inc    ebp
        inc    esi
        or     eax, 0d20000ah
        or     al, byte ptr [eax]
        and    eax, dword ptr [ecx + 58h]
        cmp    ah, byte ptr [eax]
        add    byte ptr [esi], ch
        add    byte ptr [edx], ch
        add    byte ptr [eax], ah
        add    byte ptr [eax], ah
        db  00Dh, 00Ah, 000h, 0F4h, 060h
        add    byte ptr [eax], al
        db  005h, 061h, 000h, 000h, 016h
        popad
        add    byte ptr [eax], al
        daa
        popad
        add    byte ptr [eax], al
        cmp    byte ptr [ecx], ah
        db  000h, 049h, 061h
        add    byte ptr [eax], al
        pop    edx
        popad
        add    byte ptr [eax], al
        imul   esp, dword ptr [ecx], 0
        db  07Ch, 061h
        add    byte ptr [eax], al
        lea    esp, [ecx]
        db  000h, 09Eh, 061h, 000h, 000h, 0AFh
        popad
        add    byte ptr [eax], al
        shl    byte ptr [ecx], 0
        shl    dword ptr [ecx], 1
        db  000h, 0E2h
        popad
        add    byte ptr [eax], al
        popad
        add    byte ptr [eax], al
        add    al, 62h
        add    byte ptr [eax], al
        inc    esp
        imul   esi, dword ptr [esi + 69h], 65206564h
        jb     SHORT loc_5_5170
        outsd
        jb     SHORT loc_5_5121
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        inc    esp
        bound  esi, qword ptr gs:[ebp + 67h]
        and    byte ptr [ebp + 78h], ah
        arpl   word ptr [ebp + 70h], sp
        je     SHORT loc_5_517B
        outsd
        outsb
        and    byte ptr [eax], al
        dec    esi
        outsd
        outsb
        and    byte ptr [ebp + 61h], cl
        jae    SHORT loc_5_5189
        and    byte ptr [ecx + 6eh], cl
loc_5_5121:
        je     SHORT loc_5_5143
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        inc    esp
        bound  esi, qword ptr gs:[ebp + 67h]
        and    byte ptr [edx + 72h], al
        popad
        imul   edx, dword ptr [eax + 6eh], 74h
        and    byte ptr [eax], ah
        add    byte ptr [edi + 76h], cl
        jb     NEAR PTR loc_5_51A3
        insb
        outsd
        db  077h, 020h
        and    byte ptr [eax], ah
loc_5_5143:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        inc    edx
        outsd
        jne    SHORT loc_5_51BB
        jae    NEAR PTR loc_5_5170
        inc    ebx
        push   206b6365h
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        dec    ecx
        outsb
        db  076h, 061h
        insb
        imul   esp, dword ptr [eax + 4fh], 646f6370h
        and    byte ptr gs:[eax], ah
        add    byte ptr [ebx + 6fh], al
        jo     SHORT loc_5_51E1
        outsd
loc_5_5170:
        arpl   word ptr [ebp + 73h], sp
        jae    SHORT loc_5_51E4
        jb     SHORT loc_5_5197
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_5_517B:
        add    byte ptr [edi + ebp*2 + 75h], al
        bound  ebp, qword ptr [ebp + 20h]
        inc    esi
        popad
        jne    SHORT loc_5_51F3
        db  074h, 020h
loc_5_5189:
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        inc    ebx
        outsd
        jo     SHORT loc_5_5203
        outsd
        arpl   word ptr [ebp + 73h], sp
        jae    SHORT loc_5_5206
loc_5_5197:
        jb     SHORT loc_5_51B9
        push   ebx
        inc    ebp
        inc    edi
        and    byte ptr [eax], al
        dec    ecx
        outsb
        jbe    SHORT loc_5_5203
        insb
loc_5_51A3:
        imul   esp, dword ptr [eax + 54h], 20205353h
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        push   ebx
sub_5_51B0:
        insd
        outsb
        je     SHORT loc_5_51D7
        dec    esi
        outsd
loc_5_51B9:
        db  074h, 020h
loc_5_51BB:
        push   eax
        db  072h, 073h
        and    byte ptr [eax], al
        push   ebx
        je     SHORT loc_5_5224
        arpl   word ptr [ebx + 20h], bp
        js     NEAR PTR loc_5_522C
        db  065h, 070h, 074h
        imul   ebp, dword ptr [edi + 6eh], 65470020h
        outsb
        and    byte ptr [eax + 72h], dl
loc_5_51D7:
        outsd
        je     SHORT loc_5_523F
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        outsb
        and    byte ptr [eax], ah
loc_5_51E1:
        add    byte ptr [eax + 61h], dl
loc_5_51E4:
        and    byte ptr gs:[bp + 61h], al
        db  075h, 06Ch
        db  074h, 020h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
loc_5_51F3:
        push   edx
        db  065h, 073h, 065h
        jb     SHORT loc_5_526F
        and    byte ptr fs:[eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_5_5203:
        add    byte ptr [ebx + 6fh], al
loc_5_5206:
        jo     SHORT loc_5_527A
        outsd
        arpl   word ptr [ebp + 73h], sp
        jae    SHORT loc_5_527D
        jb     SHORT loc_5_5230
        inc    ebp
        db  072h, 072h
        and    byte ptr [eax], al
        and    byte ptr [0d20000ah], cl
        or     al, byte ptr [eax]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        push   esi
        and    byte ptr [eax], bh
loc_5_5224:
        and    byte ptr [esi], dh
        and    byte ptr [eax], ah
        and    byte ptr [esi + 20h], al
        inc    ecx
loc_5_522C:
        and    byte ptr [ebp + 20h], dl
        dec    esp
loc_5_5230:
        and    byte ptr [eax + 20h], dl
        and    byte ptr [eax + 20h], cl
        dec    edi
        and    byte ptr [edi + 20h], cl
        dec    ebx
        and    byte ptr [ebx + 0dh], dl
loc_5_523F:
        or     al, byte ptr [eax]
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        or     eax, 4146000ah
        push   ebp
loc_5_526F:
        dec    esp
        push   esp
        and    byte ptr [ebp + eax*2 + 53h], al
        inc    ebx
        push   edx
        dec    ecx
        push   eax
        push   esp
loc_5_527A:
        dec    ecx
        dec    edi
        dec    esi
loc_5_527D:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [esi + 78h], dl
        inc    esp
        and    byte ptr [eax + 4fh], cl
        dec    edi
        dec    ebx
        or     eax, 2d2d000ah
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 0a0d2d2dh
        add    byte ptr [eax], ah
        and    eax, dword ptr [esp + ecx*2 + 20h]
        and    byte ptr [eax], ah
        add    byte ptr [eax], ah
        and    byte ptr [edi], bh
        inc    ebp
        inc    ecx
        pop    eax
        or     eax, 0d20000ah
        or     al, byte ptr [eax]
        pop    ebx
        inc    ebp
        push   ebx
        inc    ebx
        pop    ebp
        and    byte ptr [edi + ebp*2 + 20h], dh
        db  065h, 078h, 069h
        db  074h, 02Ch
        and    byte ptr [ecx + 6eh], ah
        db  079h, 074h
        push   20676e69h
        insb
        db  073h, 065h
        and    byte ptr [esi + 6fh], ah
        jb     SHORT loc_5_5310
        push   eax
        dec    ebp
        and    byte ptr [esi + 61h], ah
        jne    SHORT loc_5_5363
        je     SHORT loc_5_5319
        push   736b6f6fh
        add    byte ptr [eax], ah
        or     eax, 0d20000ah
        or     al, byte ptr [eax]
        and    byte ptr [2020000ah], cl
        and    byte ptr [eax], ah
        push   eax
loc_5_5310:
        and    byte ptr [ebp + 20h], cl
        and    byte ptr [eax], ah
        inc    esi
        and    byte ptr [ecx + 20h], al
loc_5_5319:
        push   ebp
        and    byte ptr [eax + 54h], cl
        and    byte ptr [eax], ah
        and    byte ptr [eax + 20h], cl
        dec    edi
        and    byte ptr [edi + 20h], cl
        dec    ebx
        and    byte ptr [ebx + 20h], dl
        and    byte ptr [eax], ah
        inc    ebx
        and    byte ptr [ebp + 20h], dl
        push   edx
        and    byte ptr [edx + 20h], dl
        inc    ebp
        and    byte ptr [esi + 20h], cl
        push   esp
        and    byte ptr [eax], ah
        and    byte ptr [esi + 20h], dl
        dec    ebp
        or     eax, 4d56000ah
        and    byte ptr [ebx], ah
        inc    ebp
        inc    edx
        pop    eax
        or     eax, 2d2d000ah
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
loc_5_5363:
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 0a0d2d2dh
        add    byte ptr [esi + 41h], al
        push   ebp
        dec    esp
        push   esp
        and    byte ptr [ebp + eax*2 + 53h], al
        inc    ebx
        push   edx
        dec    ecx
        push   eax
        push   esp
        dec    ecx
        dec    edi
        dec    esi
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [esi + 78h], dl
        inc    esp
        and    byte ptr [eax + 4fh], cl
        dec    edi
        dec    ebx
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax + 4dh], dl
        and    byte ptr [ecx + 50h], al
        push   eax
        and    byte ptr [eax + 4fh], cl
        dec    edi
        dec    ebx
        or     eax, 2d2d000ah
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 0a0d2d2dh
        add    byte ptr [eax], ah
        and    eax, dword ptr [esp + ecx*2 + 20h]
        and    byte ptr [eax], ah
        add    byte ptr [eax], ah
        and    byte ptr [edi], bh
        inc    ebp
        inc    ecx
        pop    eax
        or     dword ptr [eax], eax
        and    byte ptr [eax], ch
        and    eax, dword ptr [ecx + ecx*2 + 3ah]
        and    eax, dword ptr [ebp + 41h]
        pop    eax
        sub    dword ptr [eax], esp
        aas
        inc    esp
        dec    ecx
        cmp    al, byte ptr [ebp + 41h]
        pop    eax
        add    byte ptr [eax], ah
        or     eax, 0d20000ah
        or     al, byte ptr [eax]
        pop    ebx
        inc    ebp
        push   ebx
        inc    ebx
        pop    ebp
        and    byte ptr [edi + ebp*2 + 20h], dh
        js     NEAR PTR loc_5_54AB
        db  074h, 02Ch
        and    byte ptr [ecx + 6eh], ah
        db  079h, 074h
        push   20676e69h
        insb
        db  073h, 065h
        and    byte ptr [esi + 6fh], ah
        jb     SHORT loc_5_5477
        push   esi
        dec    ebp
        dec    ebp
        and    byte ptr [esi + 61h], ah
        jne    SHORT loc_5_54CB
        je     SHORT loc_5_5481
        push   736b6f6fh
        add    byte ptr [eax], ah
        or     eax, 0d20000ah
        or     al, byte ptr [eax]
        and    byte ptr [2020000ah], cl
        and    byte ptr [eax], ah
loc_5_5477:
        push   esi
        and    byte ptr [ebp + 20h], cl
        dec    ebp
        and    byte ptr [eax], ah
        and    byte ptr [esi + 20h], al
loc_5_5481:
        inc    ecx
        and    byte ptr [ebp + 20h], dl
        dec    esp
        and    byte ptr [eax + 20h], dl
        and    byte ptr [eax + 20h], cl
        dec    edi
        and    byte ptr [edi + 20h], cl
        dec    ebx
        and    byte ptr [ebx + 0dh], dl
        or     al, byte ptr [eax]
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
loc_5_54AB:
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        or     eax, 4146000ah
        push   ebp
        dec    esp
        push   esp
        and    byte ptr [ebp + eax*2 + 53h], al
loc_5_54CB:
        inc    ebx
        push   edx
        dec    ecx
        push   eax
        push   esp
        dec    ecx
        dec    edi
        dec    esi
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [esi + 78h], dl
        inc    esp
        and    byte ptr [eax + 4fh], cl
        dec    edi
        dec    ebx
        or     eax, 2d2d000ah
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 2d2d2d2dh
        sub    eax, 0a0d2d2dh
        add    byte ptr [eax], ah
        and    eax, dword ptr [esp + ecx*2 + 20h]
        and    byte ptr [eax], ah
        add    byte ptr [eax], ah
        and    byte ptr [edi], bh
        inc    ebp
        inc    ecx
        pop    eax
        or     eax, 0ah
        add    byte ptr [eax], ah
        or     eax, 7250000ah
        jae    NEAR PTR loc_5_55A0
        and    byte ptr [ecx + 6eh], ah
        jns    SHORT loc_5_5552
        imul   esp, dword ptr [ebp + 79h], 20h
        db  074h, 06Fh
        and    byte ptr [ebx + 65h], dh
        and    byte ptr gs:[ecx + 2fh], cl
        dec    edi
        and    byte ptr [esi + 41h], cl
        and    byte ptr [edi + 72h], ch
        and    byte ptr [ebx + 45h], bl
        push   ebx
        inc    ebx
        pop    ebp
        and    byte ptr [edi + ebp*2 + 20h], dh
        jae    SHORT loc_5_55BD
loc_5_5552:
        imul   esi, dword ptr [eax + 2eh], 20002e2eh
        or     eax, 0d20000ah
        or     al, byte ptr [eax]
        push   eax
        jb     SHORT loc_5_55C8
        db  073h, 073h
        and    byte ptr [ecx + 6eh], ah
        jns    SHORT loc_5_558A
        imul   esp, dword ptr [ebp + 79h], 20h
        db  074h, 06Fh
        and    byte ptr [ebx + 65h], dh
        and    byte ptr gs:[ecx + 2fh], cl
        dec    edi
        and    byte ptr [esi + 4ch], cl
        and    byte ptr [edi + 72h], ch
        and    byte ptr [ebx + 45h], bl
        push   ebx
        inc    ebx
        pop    ebp
        and    byte ptr [edi + ebp*2 + 20h], dh
        jae    SHORT loc_5_55F5
loc_5_558A:
        imul   esi, dword ptr [eax + 2eh], 20002e2eh
        or     eax, 0d20000ah
        or     al, byte ptr [eax]
        push   eax
        db  072h, 065h
        db  073h, 073h
        and    byte ptr [ecx + 6eh], ah
loc_5_55A0:
        jns    SHORT loc_5_55C2
        imul   esp, dword ptr [ebp + 79h], 20h
        je     SHORT loc_5_5617
        and    byte ptr [ebx + 65h], dh
        and    byte ptr gs:[ecx + 2fh], cl
        dec    edi
        and    byte ptr [eax + 72h], dh
        popad
        insb
        insb
        outsd
        arpl   word ptr [eax], sp
        outsd
        db  072h, 020h
loc_5_55BD:
        pop    ebx
        inc    ebp
        push   ebx
        inc    ebx
        pop    ebp
loc_5_55C2:
        and    byte ptr [edi + ebp*2 + 20h], dh
        jae    SHORT loc_5_5633
loc_5_55C8:
        imul   esi, dword ptr [eax + 2eh], 20002e2eh
        or     eax, 0d20000ah
        or     al, byte ptr [eax]
        push   eax
        db  072h, 065h
        db  073h, 073h
        and    byte ptr [ecx + 6eh], ah
        db  079h, 020h
        imul   esp, dword ptr [ebp + 79h], 20h
        db  074h, 06Fh
        and    byte ptr [ebx + 65h], dh
        and    byte ptr gs:[ecx + 2fh], cl
        dec    edi
        and    byte ptr [edx + 65h], dh
        jae    SHORT loc_5_5658
        jb     SHORT loc_5_566B
loc_5_55F5:
        and    byte ptr gs:[edi + 72h], ch
        and    byte ptr [ebx + 45h], bl
        push   ebx
        inc    ebx
        pop    ebp
        and    byte ptr [edi + ebp*2 + 20h], dh
        db  073h, 06Bh
        imul   esi, dword ptr [eax + 2eh], 20002e2eh
        or     eax, 0ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_5617:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_5633:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi], ch
        dec    ebp
        and    byte ptr [6d756420h], ch
loc_5_5658:
        db  070h, 020h
        insd
        insd
        outsd
        jb     SHORT loc_5_56D9
        and    byte ptr [ebp + 61h], ch
        outsb
        popad
        jb     NEAR PTR loc_5_5689
        jae    SHORT loc_5_56DF
loc_5_566B:
        jb     SHORT loc_5_56E2
        arpl   word ptr [ebp + esi*2 + 72h], si
        db  065h, 073h, 02Eh
_SetFreePhysRegCalBk:
        and    byte ptr [eax], ah
        push   esp
        jns    SHORT loc_5_56E9
        and    byte ptr gs:[edi], ah
        dec    ebp
        aas
        daa
        and    byte ptr [esi + 6fh], ah
        jb     SHORT loc_5_56A5
        insd
        outsd
        db  072h, 065h
loc_5_5689:
        and    byte ptr [ecx + 6eh], ch
        outsw
        jb     SHORT loc_5_56FD
        popad
        db  074h, 069h
        outsd
        outsb
        or     al, byte ptr cs:[eax]
        dec    ebp
        pop    ebx
        cmp    al, 73h
        je     SHORT loc_5_5711
        db  075h, 063h
        je     SHORT loc_5_56E1
        pop    ebp
        pop    ebx
loc_5_56A5:
        cmp    al, 6ch
        imul   ebp, dword ptr [esi + 6bh], 5b205d3eh
        cmp    al, 65h
        db  078h, 070h
        db  072h, 065h
        jae    SHORT loc_5_5729
        imul   ebp, dword ptr [edi + 6eh], 4c5b203eh
        cmp    al, 63h
        outsd
        db  075h, 06Eh
        db  074h, 03Eh
        pop    ebp
        pop    ebp
        or     eax, 200a0d0ah
        db  077h, 068h
        db  065h, 072h, 065h
        and    byte ptr [ebx + esi*2], bh
        db  074h, 072h
        jne    SHORT loc_5_573A
        je     SHORT loc_5_5717
loc_5_56D9:
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [ecx + 20h], ah
loc_5_56DF:
        db  073h, 069h
loc_5_56E1:
        outsb
loc_5_56E2:
        insb
        and    byte ptr gs:[ebx + 68h], ah
        popad
loc_5_56E9:
        db  072h, 061h
        arpl   word ptr [ebp + 72h], si
        and    byte ptr [edi + ebp*2 + 20h], dh
        db  073h, 070h
        arpl   word ptr gs:[ecx + 66h], bp
        db  079h, 020h
        db  074h, 068h
loc_5_56FD:
        and    byte ptr gs:[ebx + 74h], dh
        jb     SHORT loc_5_5778
        arpl   word ptr [ebp + esi*2 + 72h], si
        and    byte ptr gs:[edi + ebp*2 + 20h], dh
        db  064h, 075h, 06Dh
        db  070h, 00Dh
loc_5_5711:
        or     ah, byte ptr [eax]
        sub    byte ptr [ebp + 66h], ah
loc_5_5717:
        popad
        db  075h, 06Ch
        db  074h, 020h
        imul   esi, dword ptr [ebx + 20h], 64206f74h
        db  075h, 06Dh
        db  070h, 020h
        popad
        insb
loc_5_5729:
        insb
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 74h], dh
        db  072h, 075h
        arpl   word ptr [ebp + esi*2 + 72h], si
        db  065h, 073h, 020h
loc_5_573A:
        popad
        jae    SHORT loc_5_57B0
        imul   esp, dword ptr [ebx + 61h], 20646574h
        db  077h, 069h
        je     SHORT loc_5_57B0
        and    byte ptr [ecx + 20h], ah
        imul   esi, dword ptr [bp + 65h], 696c206eh
        outsb
        popad
        jb     SHORT loc_5_5778
        popad
        jb     NEAR PTR loc_5_57C2
        db  073h, 073h
        sub    dword ptr [edx], edi
        or     eax, 2720200ah
        inc    ecx
        daa
        and    byte ptr [20524120h], ch
        sub    eax, 65724120h
        outsb
        popad
        and    byte ptr [edx + 65h], dl
loc_5_5778:
        arpl   word ptr [edi + 72h], bp
        or     dword ptr fs:[ecx], ecx
        daa
        push   eax
        daa
        and    byte ptr [20465020h], ch
        sub    eax, 67615020h
        and    byte ptr gs:[eax], ch
        push   eax
        push   63697379h
        popad
        insb
        sub    dword ptr [eax], esp
        inc    esi
        jb     SHORT loc_5_57FD
        insd
        or     eax, 2720200ah
        inc    ebx
        daa
        and    byte ptr [20444320h], ch
        sub    eax, 6e6f4320h
loc_5_57B0:
        db  074h, 065h
        db  078h, 074h
        and    byte ptr [ebp + 73h], al
        arpl   word ptr [edx + 69h], si
        db  070h, 074h
        outsd
        db  072h, 020h
        or     dword ptr [edi], esp
loc_5_57C2:
        push   edx
        daa
        and    byte ptr [20484120h], ch
        sub    eax, 65724120h
        outsb
        popad
        and    byte ptr [eax + 65h], cl
        popad
        db  064h, 065h, 072h, 00Dh
        or     ah, byte ptr [eax]
        and    byte ptr [edi], ah
        inc    esp
        daa
        and    byte ptr [45445020h], ch
        and    byte ptr [67615020h], ch
        and    byte ptr gs:[ecx + ebp*2 + 72h], al
        arpl   word ptr gs:[edi + ebp*2 + 72h], si
        db  079h, 020h
        inc    ebp
        outsb
        db  074h, 072h
        jns    SHORT loc_5_5806
loc_5_57FD:
        daa
        push   esp
        daa
        and    byte ptr [45545020h], ch
loc_5_5806:
        and    byte ptr [67615020h], ch
        and    byte ptr gs:[ecx + 62h], dl
        insb
        and    byte ptr gs:[ebp + 6eh], al
        je     SHORT loc_5_588A
        db  079h, 00Dh
        or     ah, byte ptr [eax]
        and    byte ptr [edi], ah
        inc    edi
        daa
        and    byte ptr [20445020h], ch
        sub    eax, 67615020h
        db  065h, 072h, 020h
        inc    esp
        jae    NEAR PTR loc_5_5895
        db  072h, 069h
        db  070h, 074h
        outsd
        db  072h, 020h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        daa
        push   esi
        daa
        and    byte ptr [20505620h], ch
        sub    eax, 72695620h
        je     SHORT loc_5_58C8
        popad
        insb
        and    byte ptr [eax + 61h], dl
        or     eax, 2720200ah
        dec    eax
        daa
        and    byte ptr [61654820h], ch
        db  070h, 020h
        inc    edx
        insb
        outsd
        arpl   word ptr [ebx + 20h], bp
        outsd
        db  072h, 020h
        dec    eax
        popad
        jo     SHORT loc_5_5897
        dec    eax
        popad
        outsb
        insb
        or     dword ptr gs:[edi], esp
        pop    eax
        daa
        and    byte ptr [6c6c4120h], ch
        and    byte ptr [ebx + 74h], dh
loc_5_588A:
        db  072h, 075h
        arpl   word ptr [ebp + esi*2 + 72h], si
        db  065h, 073h, 020h
        outsw
loc_5_5895:
        db  072h, 020h
loc_5_5897:
        popad
        outsb
        and    byte ptr [ecx + 64h], ah
        db  064h, 072h, 065h
        jae    SHORT loc_5_5914
        or     eax, 2720200ah
        dec    esp
        daa
        and    byte ptr [79685020h], ch
        jae    SHORT loc_5_58DD
        dec    esp
        imul   ebp, dword ptr [esi + 65h], 6d207261h
        popad
        db  070h, 070h
        imul   ebp, dword ptr [esi + 67h], 65726120h
        popad
        jae    SHORT loc_5_58CE
        daa
        pop    ecx
        daa
loc_5_58C8:
        and    byte ptr [6c615620h], ch
loc_5_58CE:
        imul   esp, dword ptr [eax + 70h], 69737968h
        arpl   word ptr [ecx + 6ch], sp
        and    byte ptr [ebp + 65h], ch
        insd
loc_5_58DD:
        outsd
        db  072h, 079h
        and    byte ptr [edx + 61h], dh
        outsb
        db  067h, 065h, 073h, 00Dh
        or     cl, byte ptr [4920200ah]
        and    byte ptr [ebx + esi*2], bh
        db  074h, 072h
        db  075h, 063h
        db  074h, 03Eh
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [esi + 6fh], ch
        db  074h, 020h
        db  073h, 070h
        arpl   word ptr gs:[ecx + 66h], bp
        imul   esp, dword ptr [ebp + 64h], 2061202ch
        db  073h, 075h
        insd
        insd
        popad
        db  072h, 079h
loc_5_5914:
        and    byte ptr [ecx + 62h], ah
        outsd
        jne    SHORT loc_5_598E
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 70h], dh
        arpl   word ptr gs:[ecx + 66h], bp
        imul   esp, dword ptr [ebp + 64h], 67617020h
        and    byte ptr gs:[ecx + 73h], ch
        and    byte ptr [eax + 72h], dh
        imul   ebp, dword ptr [esi + 74h], 0d2e6465h
        or     cl, byte ptr [6c3c200ah]
        imul   ebp, dword ptr [esi + 6bh], 7369203eh
        and    byte ptr [ebx + 69h], dh
        outsb
        insb
        and    byte ptr gs:[ebx + 68h], ah
        popad
        jb     SHORT loc_5_59B5
        arpl   word ptr [ebp + 72h], si
        and    byte ptr [edi + ebp*2 + 20h], dh
        db  073h, 070h
        arpl   word ptr gs:[ecx + 66h], bp
        db  079h, 020h
        push   7420776fh
        outsd
        and    byte ptr [edi + 61h], dh
        insb
        imul   esp, dword ptr [eax], 74h
        push   74732065h
        db  072h, 075h
        arpl   word ptr [ebp + esi*2 + 72h], si
        jae    NEAR PTR loc_5_599F
        imul   esp, dword ptr [esi + 0dh], 7564200ah
        insd
        db  070h, 069h
        outsb
        and    byte ptr [di + 6fh], ch
loc_5_598E:
        db  072h, 065h
        and    byte ptr [eax + ebp*2 + 61h], dh
        outsb
        and    byte ptr [edi + 6eh], ch
        and    byte ptr gs:[eax], ch
        popaw
loc_5_599F:
        jne    SHORT loc_5_5A0D
        db  074h, 020h
        imul   esi, dword ptr [ebx + 20h], 77206f74h
        popad
        insb
        imul   esp, dword ptr [eax], 61h
        push   61h
        arpl   word ptr [ebp + 6eh], sp
loc_5_59B5:
        db  074h, 020h
        outsb
        je     SHORT loc_5_5A2D
        imul   esp, dword ptr [ebp + 73h], 0a0d3a29h
        and    byte ptr [eax], ah
        daa
        inc    edx
        daa
        and    byte ptr [63616220h], ch
        imul   esi, dword ptr [edi + 61h], 72h
        and    byte ptr fs:[ecx + ebp*2 + 6eh], ch
        imul   ecx, dword ptr [2720200ah], 46h
        daa
        and    byte ptr [726f6620h], ch
        db  077h, 061h
        db  072h, 064h
        and    byte ptr [ecx + ebp*2 + 6eh], ch
        imul   ecx, dword ptr [200a0d0ah], 3ch
        js     NEAR PTR loc_5_5A66
        jb     SHORT loc_5_5A5D
        db  073h, 073h
        imul   ebp, dword ptr [edi + 6eh], 7369203eh
        and    byte ptr [ecx + 20h], ah
        bound  esi, qword ptr gs:[ebp + 67h]
        jb     NEAR PTR loc_5_5A2D
loc_5_5A0D:
        db  065h, 078h, 070h
        jb     SHORT loc_5_5A77
        db  073h, 073h
        imul   ebp, dword ptr [edi + 6eh], 61687420h
        db  074h, 020h
        imul   esp, dword ptr [ebp + 6eh], 69666974h
        db  065h, 073h, 020h
        ja     SHORT loc_5_5A92
        popad
        je     SHORT loc_5_5A4D
loc_5_5A2D:
        jae    SHORT loc_5_5AA3
        jb     SHORT loc_5_5AA6
        arpl   word ptr [ebp + esi*2 + 72h], si
        db  065h, 073h, 020h
        db  074h, 06Fh
        or     eax, 7564200ah
        insd
        db  070h, 020h
        sub    byte ptr [ecx + 20h], ah
        insb
        imul   ebp, dword ptr [esi + 65h], 61207261h
loc_5_5A4D:
        db  064h, 064h, 072h, 065h
        db  073h, 073h
        and    byte ptr [edi + 66h], ch
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 74h], dh
loc_5_5A5D:
        jb     SHORT loc_5_5AD4
        arpl   word ptr [ebp + esi*2 + 72h], si
        sub    al, 20h
loc_5_5A66:
        outsd
        jb     SHORT loc_5_5A89
        push   6c646e61h
        sub    al, 20h
        outsd
        db  072h, 020h
        popad
        db  072h, 072h
loc_5_5A77:
        popad
        db  079h, 020h
        imul   ebp, dword ptr [esi + 64h], 202c7865h
        db  065h, 074h, 063h
        sub    dword ptr cs:[esi], ebp
loc_5_5A89:
        or     eax, 6f46200ah
        db  072h, 020h
        insd
loc_5_5A92:
        and    byte ptr [ecx + 6eh], ah
        and    byte ptr fs:[esi], ch
        insd
        db  078h, 02Ch
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebp + 66h], ah
loc_5_5AA3:
        popad
        db  075h, 06Ch
loc_5_5AA6:
        db  074h, 020h
        imul   esi, dword ptr [ebx + 20h], 20656874h
        arpl   word ptr [edi + 6eh], bp
        db  074h, 065h
        outsb
        je     SHORT loc_5_5B2A
        and    byte ptr [edi + 66h], ch
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 72h], ah
        xor    ah, byte ptr [eax]
        jb     SHORT loc_5_5B2A
        imul   esi, dword ptr [bp + di + 74h], 28207265h
        je     SHORT loc_5_5B37
        and    byte ptr gs:[ecx + 73h], ch
loc_5_5AD4:
        je     SHORT loc_5_5AE3
        or     ah, byte ptr [eax]
        db  070h, 061h
        and    byte ptr gs:[bp + 61h], ah
        db  075h, 06Ch
        db  074h, 029h
loc_5_5AE3:
        and    byte ptr [ecx + 6eh], ah
        and    byte ptr fs:[esi + 6fh], ah
        db  072h, 020h
        db  074h, 068h
        and    byte ptr gs:[edi + 74h], ch
        push   63207265h
        outsd
        insd
        insd
        popad
        outsb
        jae    NEAR PTR loc_5_5B1F
        imul   esi, dword ptr [eax + 69h], 68742073h
        and    byte ptr gs:[esi + 69h], ah
        db  072h, 073h
        db  074h, 020h
        jae    SHORT loc_5_5B85
        db  072h, 075h
        arpl   word ptr [ebp + esi*2 + 72h], si
        and    byte ptr gs:[ecx + 6eh], ch
        and    byte ptr [eax + ebp*2 + 65h], dh
loc_5_5B1F:
        and    byte ptr [ecx + ebp*2 + 73h], ch
        je     SHORT loc_5_5B53
        or     eax, 460a0d0ah
loc_5_5B2A:
        outsd
        db  072h, 020h
        db  065h, 078h, 061h
        insd
        db  070h, 06Ch
        sub    al, 20h
        daa
loc_5_5B37:
        dec    ebp
        daa
        and    byte ptr [edi + 69h], dh
        insb
        insb
        and    byte ptr [edi + 69h], ah
        jbe    SHORT loc_5_5BA9
        and    byte ptr [ecx + 6fh], bh
        db  075h, 020h
        imul   ebp, dword ptr [esi + 66h], 616d726fh
        db  074h, 069h
        outsd
loc_5_5B53:
        outsb
        and    byte ptr [ecx + 62h], ah
        outsd
        db  075h, 074h
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [eax + 61h], dh
        and    byte ptr gs:[di + 6fh], ch
        db  073h, 074h
        and    byte ptr [edx + 65h], dh
        arpl   word ptr [ebp + 6eh], sp
        db  074h, 06Ch
        jns    SHORT loc_5_5B7F
        or     ah, byte ptr [esi + 61h]
        jne    SHORT loc_5_5BE3
        db  074h, 065h
        and    byte ptr fs:[ebp + 70h], dh
        outsd
        outsb
loc_5_5B7F:
        and    byte ptr cs:[eax], ah
        daa
        dec    ebp
loc_5_5B85:
        push   eax
        inc    esi
        and    byte ptr [ebp + 41h], al
        pop    eax
        and    byte ptr [ecx + esi + 30h], cl
        daa
        and    byte ptr [edi + 69h], dh
        insb
        insb
        and    byte ptr [ebp + esi*2 + 6dh], ah
        db  070h, 020h
        je     SHORT loc_5_5C05
        and    byte ptr gs:[ebp + 6eh], dh
        and    byte ptr [eax + 46h], dl
        db  073h, 020h
        jae    SHORT loc_5_5C1D
loc_5_5BA9:
        popad
        jb     SHORT loc_5_5C20
        imul   ebp, dword ptr [esi + 67h], 20746120h
        je     SHORT loc_5_5C1D
        and    byte ptr gs:[ecx + ebp*2 + 6eh], ch
        popad
        db  072h, 00Dh
        or     ah, byte ptr [ecx + 64h]
        jb     NEAR PTR loc_5_5C29
        jae    SHORT loc_5_5C39
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [ebp + 41h], al
        pop    eax
        and    byte ptr [ecx + 6eh], ah
        and    byte ptr fs:[esi + 6fh], ah
        insb
        insb
        outsd
        db  077h, 020h
        db  074h, 068h
        imul   esi, dword ptr gs:[edx + 20h], 77726f66h
loc_5_5BE3:
        popad
        db  072h, 064h
        and    byte ptr [ecx + ebp*2 + 6eh], ch
        imul   esi, dword ptr [ebx + 0dh], 0ah
        add    byte ptr [eax], al
        dec    ecx
        outsb
        and    byte ptr [ecx + 64h], ah
        imul   esi, dword ptr fs:[ecx + ebp*2 + 6fh], 6f74206eh
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ecx + 62h], ah
loc_5_5C05:
        outsd
        db  076h, 065h
        sub    al, 20h
        db  074h, 068h
        db  065h, 072h, 065h
        and    byte ptr [ecx + 72h], ah
        and    byte ptr gs:[edi + 74h], ch
        push   6d207265h
        insd
loc_5_5C1D:
        outsd
        db  072h, 079h
loc_5_5C20:
        and    byte ptr [ebp + 61h], ch
        outsb
        popad
        db  067h, 065h, 072h, 020h
loc_5_5C29:
        arpl   word ptr [edi + 6dh], bp
        insd
        popad
        outsb
        db  064h, 073h, 020h
        je     SHORT loc_5_5C9C
        popad
        je     SHORT loc_5_5C57
        outsd
loc_5_5C39:
        and    byte ptr [esi + 6fh], ch
        db  074h, 00Dh
        or     ah, byte ptr [ebp + esi*2 + 6dh]
        db  070h, 020h
        jae    SHORT loc_5_5CBA
        db  072h, 075h
        arpl   word ptr [ebp + esi*2 + 72h], si
        jae    NEAR PTR loc_5_5C89
        or     eax, 200a0d0ah
        and    byte ptr [edi], ah
        inc    ebp
loc_5_5C57:
        daa
        and    byte ptr [eax], ah
        sub    eax, 676f5420h
        insb
        and    byte ptr gs:[ebx + 74h], dh
        outsd
        db  070h, 070h
        imul   ebp, dword ptr [esi + 67h], 206e6920h
        je     SHORT loc_5_5CD9
        and    byte ptr gs:[ebp + 62h], ah
        jne    SHORT loc_5_5CDF
        jb     NEAR PTR loc_5_5C9C
        outsw
        db  072h, 020h
        insd
        insd
        outsd
        db  072h, 079h
        and    byte ptr [ebp + 61h], ch
loc_5_5C89:
        outsb
        popad
        jb     NEAR PTR loc_5_5CAF
        db  065h, 072h, 072h
        outsd
        jb     SHORT loc_5_5D08
        or     eax, 2720200ah
        inc    esi
        daa
loc_5_5C9C:
        and    byte ptr [eax], ah
        sub    eax, 74695720h
        push   206f6e20h
        popad
        db  072h, 067h
        db  075h, 06Dh
        outsb
loc_5_5CAF:
        je     SHORT loc_5_5D24
        sub    al, 20h
        db  064h, 075h, 06Dh
        db  070h, 020h
        db  070h, 061h
loc_5_5CBA:
        and    byte ptr gs:[bp + 61h], ah
        db  075h, 06Ch
        je     SHORT loc_5_5CE3
        insb
        outsd
        and    byte ptr cs:[bx + si], ah
        push   edi
        imul   esi, dword ptr [eax + ebp*2 + 20h], 75677261h
        insd
        outsb
        je     SHORT loc_5_5D4A
        sub    al, 20h
loc_5_5CD9:
        jae    SHORT loc_5_5D40
        db  074h, 020h
loc_5_5CDD:
        db  074h, 06Fh
loc_5_5CDF:
        and    byte ptr [edi + ebp*2 + 67h], ch
loc_5_5CE3:
        or     eax, 6620090ah
        popad
        db  075h, 06Ch
        je     SHORT loc_5_5D60
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [ecx + 20h], ah
        db  073h, 070h
        arpl   word ptr gs:[ecx + 66h], bp
        imul   esp, dword ptr [ebx + 20h], 676e6172h
        and    byte ptr gs:[eax], ch
        db  073h, 079h
        outsb
        db  074h, 061h
loc_5_5D08:
        db  078h, 03Ah
        and    byte ptr [esi], ch
        dec    ebp
        inc    esi
        and    byte ptr [ecx], bh
loc_5_5D11:
        db  064h, 064h, 072h, 065h
loc_5_5D15:
        db  073h, 073h
        and    byte ptr ds:[esp + edi + 6ch], cl
        outsb
        push   206e6920h
loc_5_5D24:
        bound  edi, qword ptr [ecx + 74h]
        db  065h, 073h, 03Eh
        sub    dword ptr [esi], ebp
        or     eax, 2720200ah
        inc    esi
        inc    edx
        daa
        and    byte ptr [676f5420h], ch
        insb
        and    byte ptr gs:[ebx + 74h], dh
loc_5_5D40:
        outsd
        db  070h, 070h
        imul   ebp, dword ptr [esi + 67h], 726f6620h
loc_5_5D4A:
        and    byte ptr [ebp + 61h], ah
        arpl   word ptr [eax + 65h], bp
        and    byte ptr fs:[edi + ebp*2 + 67h], ch
        and    byte ptr fs:[bx + si + 61h], dh
        and    byte ptr gs:[bp + 61h], ah
loc_5_5D60:
        db  075h, 06Ch
        je     SHORT loc_5_5D84
        sub    byte ptr [ebx + 65h], dh
        and    byte ptr gs:[ecx + 62h], ah
        outsd
        db  076h, 065h
        sub    dword ptr [2720200ah], ecx
        inc    esi
        inc    esi
        daa
        and    byte ptr [676f5420h], ch
        insb
        and    byte ptr gs:[edi + ebp*2 + 67h], ch
loc_5_5D84:
        imul   ebp, dword ptr [bp + 67h], 67617020h
        and    byte ptr gs:[esi + 61h], ah
        db  075h, 06Ch
        db  074h, 073h
        and    byte ptr [edi + ebp*2 + 20h], dh
        bound  esi, qword ptr gs:[ebp + 67h]
        and    byte ptr [ebp + 72h], dh
        insd
        imul   ebp, dword ptr [esi + 61h], 200a0d6ch
        and    byte ptr [edi], ah
        dec    eax
        push   ebx
        daa
        and    byte ptr [6d754420h], ch
        db  070h, 020h
        db  073h, 074h
        popad
        db  074h, 069h
        jae    SHORT loc_5_5E31
        imul   esp, dword ptr [ebx + 73h], 6f626120h
        jne    SHORT loc_5_5E3A
        and    byte ptr [ecx + 20h], ah
        imul   esi, dword ptr [bp + 65h], 6568206eh
        popad
        db  070h, 020h
        sub    byte ptr [ebx + 79h], dh
        outsb
        db  074h, 061h
        js     SHORT loc_5_5E16
        and    byte ptr [esi], ch
        dec    ebp
        dec    eax
        push   ebx
        and    byte ptr [ebx + 3ch], bl
        push   20706165h
        push   6c646e61h
        pop    ebp
        sub    dword ptr [2720200ah], ecx
        dec    eax
        push   edi
        daa
        and    byte ptr [676f5420h], ch
        insb
        and    byte ptr gs:[eax + 61h], dh
        db  072h, 061h
        outsb
        outsd
        imul   esp, dword ptr [eax + 56h], 32334d4dh
        and    byte ptr [eax + 65h], ch
        popad
loc_5_5E16:
        jo     SHORT loc_5_5E38
        arpl   word ptr [eax + 65h], bp
        arpl   word ptr [ebx + 69h], bp
        outsb
        or     eax, 2720200ah
        dec    eax
        pop    eax
        daa
        and    byte ptr [65684320h], ch
        arpl   word ptr [ebx + 20h], bp
loc_5_5E31:
        outsw
        db  072h, 020h
        arpl   word ptr [edi + 72h], bp
loc_5_5E38:
        jb     SHORT loc_5_5EAF
loc_5_5E3A:
        db  070h, 074h
        imul   ebp, dword ptr [edi + 6eh], 206e6f20h
        imul   esi, dword ptr [bp + 65h], 6568206eh
        popad
        db  070h, 020h
        sub    byte ptr [ebx + 79h], dh
        outsb
        db  074h, 061h
        db  078h, 03Ah
        and    byte ptr [esi], ch
        dec    ebp
        dec    eax
        pop    eax
        and    byte ptr [ebx + 3ch], bl
        push   20706165h
        push   6c646e61h
        pop    ebp
        sub    dword ptr [6420090ah], ecx
        popaw
        jne    SHORT loc_5_5EE2
        db  074h, 073h
        and    byte ptr [edi + ebp*2 + 20h], dh
        push   esi
        dec    ebp
        dec    ebp
        xor    esi, dword ptr [edx]
        and    byte ptr [esi + 69h], ah
        db  078h, 065h
        and    byte ptr fs:[eax + 65h], ch
        popad
        jo     SHORT loc_5_5EBB
        or     eax, 2720200ah
        dec    ecx
        daa
        and    byte ptr [eax], ah
        sub    eax, 6d754420h
        db  070h, 073h
        and    byte ptr [ecx + 6eh], ch
        outsw
        and    byte ptr [ecx + 62h], ah
        outsd
        jne    SHORT loc_5_5F1C
        and    byte ptr [ecx + 6eh], ch
        jae    SHORT loc_5_5F21
        popad
        outsb
loc_5_5EAF:
        arpl   word ptr [ebp + 20h], sp
        popad
        db  074h, 061h
        or     eax, 2720200ah
loc_5_5EBB:
        dec    ebp
        daa
        and    byte ptr [eax], ah
        sub    eax, 726f4620h
        arpl   word ptr [ebp + 20h], sp
        insd
        insd
        outsd
        db  072h, 079h
        and    byte ptr [eax + 72h], dh
        jae    NEAR PTR loc_5_5F38
        outsb
        db  074h, 020h
        sub    byte ptr [ebx + 79h], dh
        outsb
        je     SHORT loc_5_5F3D
        db  078h, 03Ah
        and    byte ptr [esi], ch
        dec    ebp
        dec    ebp
loc_5_5EE2:
        and    byte ptr [ebx + 3ch], bl
        popad
        db  064h, 064h, 072h, 065h
        jae    SHORT loc_5_5F5F
        and    byte ptr ds:[ebx + 6ch], bl
        cmp    al, 70h
        popad
        and    byte ptr gs:[bp + di + 6fh], ah
        db  075h, 06Eh
        je     SHORT loc_5_5F3A
        pop    ebp
        pop    ebp
        and    byte ptr [ecx], ch
        or     eax, 3c20090ah
        popad
        jb     NEAR PTR loc_5_5F6F
        db  073h, 073h
        and    byte ptr ds:[ebp + 66h], ah
        popad
        jne    SHORT loc_5_5F80
        je     SHORT loc_5_5F89
        and    byte ptr [edi + ebp*2 + 20h], dh
        insb
        popad
loc_5_5F1C:
        db  073h, 074h
        and    byte ptr [eax + 61h], dh
loc_5_5F21:
        and    byte ptr gs:[bp + 61h], ah
        jne    SHORT loc_5_5F94
        je     SHORT loc_5_5F8F
        and    byte ptr fs:[ebp + 70h], dh
        outsd
        outsb
        and    byte ptr [eax], ch
        arpl   word ptr [edi + 6eh], bp
        je     SHORT loc_5_5F9C
        outsb
loc_5_5F38:
        je     SHORT loc_5_5FAD
loc_5_5F3A:
        and    byte ptr [edi + 66h], ch
loc_5_5F3D:
        and    byte ptr [ebx + 72h], ah
        xor    ah, byte ptr [eax]
        jb     SHORT loc_5_5FA9
        imul   esi, dword ptr [bp + di + 74h], 0d297265h
        or     ah, byte ptr [eax]
        and    byte ptr [edi], ah
        dec    esi
        daa
        and    byte ptr [eax], ah
        sub    eax, 726f4620h
        arpl   word ptr [ebp + 20h], sp
        popad
        insb
        insb
loc_5_5F5F:
        and    byte ptr [ebp + 6eh], dh
        insb
        outsd
        arpl   word ptr [ebx + 65h], bp
        and    byte ptr fs:[ebp + 65h], ch
        insd
        outsd
        db  072h, 079h
loc_5_5F6F:
        and    byte ptr [esi + 6fh], ch
        je     SHORT loc_5_5F94
        db  070h, 072h
        jae    NEAR PTR loc_5_5FDE
        outsb
        je     SHORT loc_5_5F89
        or     ah, byte ptr [eax]
        and    byte ptr [edi], ah
loc_5_5F80:
        push   ebx
        daa
        and    byte ptr [eax], ah
        sub    eax, 6d754420h
loc_5_5F89:
        jo     SHORT loc_5_5FAB
        insd
        insd
        outsd
loc_5_5F8F:
        jb     SHORT loc_5_600A
        and    byte ptr [ebp + 61h], ch
loc_5_5F94:
        outsb
        popad
        db  067h, 065h, 072h, 020h
        db  073h, 074h
loc_5_5F9C:
        popad
        je     SHORT loc_5_6008
        db  073h, 074h
        imul   esp, dword ptr [ebx + 73h], 20200a0dh
        daa
loc_5_5FA9:
        push   ebp
        daa
loc_5_5FAB:
        and    byte ptr [eax], ah
loc_5_5FAD:
        sub    eax, 676f5420h
        insb
        and    byte ptr gs:[ebx + 6fh], ah
        insd
        jo     SHORT loc_5_6030
        db  074h, 069h
        outsb
        and    byte ptr [bp + di + 68h], ah
        arpl   word ptr gs:[ebx + 73h], bp
        db  075h, 06Dh
        jae    SHORT loc_5_5FEA
        outsw
        db  072h, 020h
        db  070h, 061h
        sub    eax, 7374756fh
        and    byte ptr [ecx + 6eh], ah
        and    byte ptr fs:[eax + 61h], dh
loc_5_5FDE:
        sub    eax, 0d736e69h
        or     ah, byte ptr [eax]
        and    byte ptr [edi], ah
        push   edi
loc_5_5FEA:
        daa
        and    byte ptr [eax], ah
        sub    eax, 6d754420h
        db  070h, 020h
        popad
        insb
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        outsd
        db  072h, 073h
        and    byte ptr [edi + 66h], ch
        and    byte ptr [ecx + 6ch], ah
        insb
        and    byte ptr [ebx + 6fh], ah
loc_5_6008:
        insd
        insd
loc_5_600A:
        imul   esi, dword ptr [esp + esi*2 + 65h], 61702064h
        db  067h, 065h, 073h, 00Dh
        or     cl, byte ptr [7720410ah]
        outsd
        db  072h, 064h
        and    byte ptr [edi + 66h], ch
        and    byte ptr [ebx + 61h], ah
        db  075h, 074h
        imul   ebp, dword ptr [edi + 6eh], 6f626120h
        db  075h, 074h
loc_5_6030:
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [esi], ch
        dec    ebp
        dec    ebp
        and    byte ptr [ebx + 6fh], ah
        insd
        insd
        popad
        outsb
        and    byte ptr fs:[6669202dh], ch
        and    byte ptr [ecx + 6fh], bh
        jne    SHORT loc_5_606B
        imul   ebp, dword ptr [esi + 76h], 20656b6fh
        imul   esi, dword ptr [eax + 61h], 68742074h
        and    byte ptr gs:[edi + 72h], dh
        outsd
        outsb
        and    byte ptr [si + 69h], dh
        insd
        sub    al, 0dh
        or     bh, byte ptr [ecx + 6fh]
loc_5_606B:
        db  075h, 020h
        arpl   word ptr [ecx + 6eh], sp
        and    byte ptr [ebx + 72h], ah
        popad
        jae    SHORT loc_5_60DE
        and    byte ptr [ecx + 6fh], bh
        jne    SHORT loc_5_60ED
        and    byte ptr [ebx + 79h], dh
        jae    SHORT loc_5_60F4
        insd
        and    byte ptr cs:[eax], ah
        push   esp
        push   6e6f2065h
        insb
        db  079h, 020h
        db  074h, 069h
        insd
        and    byte ptr gs:[ecx + 74h], ch
        and    byte ptr [ebx + 61h], ah
        outsb
        and    byte ptr [edx + 65h], ah
        and    byte ptr [edi + 75h], ah
        popad
        db  072h, 061h
        outsb
        db  074h, 065h
        and    byte ptr fs:[ebx + 61h], dh
        and    byte ptr gs:[ecx + 73h], ch
        and    byte ptr [ecx + 66h], ch
        and    byte ptr [ecx + 6fh], bh
        jne    SHORT loc_5_60C4
        or     ah, byte ptr [ecx + 72h]
        and    byte ptr gs:[ebx + 75h], ah
        jb     SHORT loc_5_6132
        outsb
        je     SHORT loc_5_6130
loc_5_60C4:
        db  079h, 020h
        db  065h, 078h, 065h
        arpl   word ptr [ebp + 74h], si
        imul   ebp, dword ptr [esi + 67h], 6e697220h
        and    byte ptr [bp + di], dh
        and    byte ptr [ebx + 6fh], ah
        and    byte ptr cs:[eax], ah
loc_5_60DE:
        inc    ecx
        db  074h, 020h
        outsd
        je     SHORT loc_5_614C
        jb     NEAR PTR loc_5_6107
        db  074h, 069h
        insd
        db  065h, 073h, 020h
loc_5_60ED:
        db  077h, 065h
        and    byte ptr [ebp + 61h], ch
        db  079h, 020h
loc_5_60F4:
        bound  esp, qword ptr [ebp + 20h]
        imul   ebp, dword ptr [esi + 20h], 20656874h
        insd
        imul   esp, dword ptr [esp + 6ch], 6f0a0d65h
loc_5_6107:
        and    byte ptr [ecx + 6eh], ah
        outsd
        je     SHORT loc_5_6176
        db  065h, 072h, 020h
        imul   bp, word ptr [ebp + 20h], 7973h
        db  073h, 074h
        insd
        and    byte ptr [edi + 72h], ch
        and    byte ptr [ebp + 65h], ch
        insd
        outsd
        db  072h, 079h
        and    byte ptr [ebp + 61h], ch
        outsb
        popad
        jb     NEAR PTR loc_5_614F
        outsd
loc_5_6130:
        db  070h, 065h
loc_5_6132:
        jb     SHORT loc_5_6195
        db  074h, 069h
        outsd
        outsb
        and    byte ptr cs:[eax], ah
        push   esp
        push   77207369h
        popad
        db  072h, 06Eh
        imul   ebp, dword ptr [esi + 67h], 736c6120h
        outsd
loc_5_614C:
        and    byte ptr [ecx + 70h], ah
loc_5_614F:
        db  070h, 06Ch
        imul   esp, dword ptr [ebp + 73h], 6f740a0dh
        and    byte ptr [esi], ch
        dec    ebp
        dec    eax
        pop    eax
        and    byte ptr [ecx + 6eh], ah
        and    byte ptr fs:[esi], ch
        dec    ebp
        dec    eax
        push   ebx
        add    byte ptr [eax], al
        dec    edi
        dec    esi
        add    byte ptr [eax], al
        dec    edi
        inc    esi
        inc    esi
        add    byte ptr [eax + 42h], bl
        push   edi
        dec    ecx
        push   eax
        push   ebx
loc_5_6176:
        inc    esp
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  070h, 050h
        push   esp
        inc    ebp
        cmp    eax, 382e3825h
        js     SHORT loc_5_61A7
        add    byte ptr [ecx + 6eh], ch
        db  076h, 061h
        insb
        imul   esp, dword ptr [eax + 50h], 61204554h
loc_5_6195:
        db  064h, 064h, 072h, 065h
        db  073h, 073h
        add    byte ptr [edx + 61h], al
        db  073h, 065h
        inc    ecx
        cmp    eax, 382e3825h
loc_5_61A7:
        db  078h, 020h
        add    byte ptr [eax], al
        add    byte ptr [eax + 61h], dh
        and    byte ptr gs:[si + 61h], dh
        bound  ebp, qword ptr [ebp + 20h]
        outsb
        outsd
        db  074h, 020h
        arpl   word ptr [edi + 6dh], bp
        insd
        imul   esi, dword ptr [esp + esi*2 + 65h], 64h
        add    byte ptr [eax + 61h], dh
        and    byte ptr gs:[si + 61h], dh
        bound  ebp, qword ptr [ebp + 20h]
        outsb
        outsd
        db  074h, 020h
        db  070h, 072h
        db  065h, 073h, 065h
        outsb
        je     SHORT loc_5_61DF
loc_5_61DF:
        add    byte ptr [eax + 54h], dl
        inc    ebp
        cmp    eax, 382e3825h
        db  078h, 020h
        add    byte ptr [eax], al
        or     ch, byte ptr [ecx + 6eh]
        db  076h, 061h
        insb
        imul   esp, dword ptr [eax + 6fh], 6f697470h
        outsb
        and    byte ptr [0a000063h], ah
        add    byte ptr [eax], al
        add    byte ptr [eax + 50h], dh
        inc    esp
        inc    ebp
        cmp    eax, 382e3825h
        js     SHORT loc_5_622F
        add    byte ptr [ecx + 6eh], ch
        db  076h, 061h
        insb
        imul   esp, dword ptr [eax + 50h], 61204544h
        db  064h, 064h, 072h, 065h
        jae    SHORT loc_5_6296
        add    byte ptr [edx + 61h], al
        jae    SHORT loc_5_628D
        inc    ecx
        cmp    eax, 382e3825h
loc_5_622F:
        db  078h, 020h
        add    byte ptr [eax], al
        add    byte ptr [eax + 44h], dl
        inc    ebp
        cmp    eax, 382e3825h
        js     SHORT loc_5_625E
        add    byte ptr [eax], al
        or     ch, byte ptr [ecx + 6eh]
        jbe    SHORT loc_5_62A6
        insb
        imul   esp, dword ptr [eax + 6fh], 6f697470h
        outsb
        and    byte ptr [0a000063h], ah
        add    byte ptr [eax], al
        add    byte ptr [eax + 68h], dh
        db  079h, 073h
        and    byte ptr [eax], ah
loc_5_625E:
        and    byte ptr [eax], al
        arpl   word ptr [edi + 6dh], bp
        insd
        imul   esi, dword ptr [eax], 73657270h
        outsb
        je     SHORT loc_5_628F
        db  066h, 072h, 061h
        insd
        cmp    eax, 6e000000h
        outsd
        db  074h, 02Dh
        db  070h, 072h
        db  065h, 073h, 020h
        imul   edx, dword ptr [esi + 50h], 3dh
        and    eax, 78352e35h
loc_5_628D:
        and    byte ptr [eax], al
loc_5_628F:
        add    byte ptr [edx + 2fh], dh
        db  077h, 020h
        add    byte ptr [eax], al
loc_5_6296:
        add    byte ptr [eax], al
        db  072h, 02Fh
        outsd
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp + 73h], dh
        jb     SHORT loc_5_62C4
        add    byte ptr [eax], al
loc_5_62A6:
        add    byte ptr [eax], al
        db  073h, 079h
        jae    SHORT loc_5_62CC
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    ecx
        and    byte ptr [eax], al
        add    byte ptr [ebp + 20h], dh
        add    byte ptr [eax], al
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [ebx], ah
        add    byte ptr [eax], al
        db  072h, 065h
        db  073h, 065h
loc_5_62C4:
        jb     SHORT loc_5_633C
        and    byte ptr fs:[eax], ah
        and    byte ptr [eax], ah
loc_5_62CC:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        imul   eax, dword ptr [ecx + 52h], 2e35253dh
        xor    eax, 78h
        jb     NEAR PTR loc_5_6344
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
        jo     SHORT loc_5_6347
        and    byte ptr gs:[si + 61h], dh
        bound  ebp, qword ptr [ebp + 3ah]
        and    byte ptr [edx], bh
        add    byte ptr [eax], al
        add    byte ptr [4d5a70h], ah
        add    byte ptr [eax], al
        add    byte ptr [eax + 41h], dh
        push   edx
        cmp    eax, 382e3825h
        db  078h, 020h
        add    byte ptr [eax], al
        insd
        imul   esi, dword ptr [ebx + 2dh], 67696c61h
        outsb
        and    byte ptr fs:[eax + 6fh], dh
        imul   ebp, dword ptr [esi + 74h], 6e007265h
        outsd
        je     SHORT loc_5_6340
        ja     SHORT loc_5_638B
        db  074h, 068h
        imul   ebp, dword ptr [esi + 20h], 6d6d6f63h
        imul   esi, dword ptr [esp + esi*2 + 65h], 61722064h
        outsb
        and    byte ptr gs:[bx + 66h], ch
        and    byte ptr [ecx + 52h], al
loc_5_633C:
        jae    SHORT loc_5_633E
loc_5_633E:
        add    byte ptr [eax], al
loc_5_6340:
        outsb
        outsd
        db  074h, 020h
loc_5_6344:
        arpl   word ptr [edi + 6dh], bp
loc_5_6347:
        insd
        imul   esi, dword ptr [esp + esi*2 + 65h], 64h
        imul   eax, dword ptr [ecx + 52h], 2e34253dh
        xor    al, 78h
        and    byte ptr [eax], al
        add    byte ptr [ecx + 52h], al
        sub    eax, 65657266h
        and    byte ptr [eax], ah
        jo     SHORT loc_5_63B5
        js     NEAR PTR loc_5_63DE
        cmp    eax, 382e3825h
        js     SHORT loc_5_6371
loc_5_6371:
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        imul   ebp, dword ptr [esi + 76h], 64696c61h
        and    byte ptr [edi + 70h], ch
        db  074h, 069h
        outsd
        outsb
        and    byte ptr [edx], al
        add    byte ptr [eax], al
        inc    esi
        db  072h, 065h
loc_5_638B:
        dec    eax
        popad
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], al
        jb     NEAR PTR loc_5_63FC
        and    byte ptr gs:[eax], ah
        and    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        and    byte ptr [eax + 4eh], dh
        db  065h, 078h, 074h
        cmp    eax, 382e3825h
        js     SHORT loc_5_63CD
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        add    byte ptr [edx], cl
loc_5_63B5:
        imul   ebp, dword ptr [esi + 76h], 64696c61h
        and    byte ptr [edi + 70h], ch
        db  074h, 069h
        outsd
        outsb
        and    byte ptr [edx], al
        add    byte ptr [eax], al
        inc    edx
        jne    SHORT loc_5_643E
        jns    SHORT loc_5_6415
loc_5_63CD:
        popad
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], al
        jb     SHORT loc_5_643B
        db  073h, 065h
        jb     SHORT loc_5_6450
        add    byte ptr fs:[eax], al
loc_5_63DE:
        add    byte ptr [eax], al
        and    byte ptr [ecx + 4eh], ch
        db  065h, 078h, 074h
        cmp    eax, 342e3425h
        db  078h, 020h
        imul   edx, dword ptr [eax + 72h], 253d7665h
        xor    al, 2eh
        xor    al, 78h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_63FC:
        and    byte ptr [eax + 61h], dh
        cmp    eax, 352e3525h
        js     SHORT loc_5_6428
        arpl   word ptr [eax + 67h], si
        cmp    eax, 352e3525h
        db  078h, 020h
        add    byte ptr [eax], al
        push   ebx
loc_5_6415:
        add    byte ptr [eax], al
        add    byte ptr [esi], ch
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr cs:[eax], al
        add    byte ptr [ebp], dl
        add    byte ptr [eax], al
loc_5_6428:
        add    byte ptr cs:[eax], al
        add    byte ptr [edi], dl
        add    byte ptr [eax], al
        add    byte ptr cs:[eax], al
        add    byte ptr [edx], cl
        and    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
loc_5_643B:
        add    byte ptr [ecx + 72h], ah
loc_5_643E:
        outsb
        popad
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [esi + 72h], ah
        sub    al, 20h
        arpl   word ptr [edi + 6eh], bp
        db  074h, 065h
loc_5_6450:
        db  078h, 074h
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [eax + 72h], dh
        outsd
        bound  esp, qword ptr [ecx + 62h]
        insb
        db  079h, 020h
        imul   ebp, dword ptr [esi + 20h], 636f7270h
        jae    NEAR PTR loc_5_64DC
        and    byte ptr [edi + 66h], ch
        and    byte ptr [edi + 6fh], ah
        imul   ebp, dword ptr [esi + 67h], 61776120h
        db  079h, 00Ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    ecx
        push   edx
        and    byte ptr [ecx + 72h], ah
        jb     SHORT loc_5_64E4
        jns    SHORT loc_5_64A5
        imul   esi, dword ptr [ebx + 20h], 20746f6eh
        jne    NEAR PTR loc_5_64FB
        insb
        jns    SHORT loc_5_64B2
        arpl   word ptr [edi + 6dh], bp
        insd
        imul   esi, dword ptr [esp + esi*2 + 65h], 0a64h
        add    byte ptr [eax], al
        arpl   word ptr [edi + 72h], bp
        jb     SHORT loc_5_651A
loc_5_64A5:
        db  070h, 074h
        and    byte ptr [ecx + 52h], al
        and    byte ptr [esi + 72h], ah
        and    byte ptr gs:[eax + 65h], ch
loc_5_64B2:
        popad
        sub    al, 20h
        db  070h, 041h
        push   edx
        cmp    eax, 382e3825h
        db  078h, 00Ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        arpl   word ptr [edi + 72h], bp
        jb     SHORT loc_5_653E
        db  070h, 074h
        and    byte ptr [ecx + 52h], al
        and    byte ptr [esi + 72h], ah
        and    byte ptr gs:[ecx + ebp*2 + 73h], ch
        db  074h, 02Ch
        and    byte ptr [eax + 41h], dh
loc_5_64DC:
        push   edx
        cmp    eax, 382e3825h
        db  078h, 02Ch
loc_5_64E4:
        and    byte ptr [eax + 41h], dh
        dec    eax
        cmp    eax, 382e3825h
        db  078h, 00Ah
        add    byte ptr [ecx + 52h], al
        and    byte ptr [esi + 72h], ah
        and    byte ptr gs:[ecx + ebp*2 + 73h], ch
loc_5_64FB:
        db  074h, 020h
        jo     SHORT loc_5_6571
        outsd
        bound  esp, qword ptr [ecx + 62h]
        insb
        jns    SHORT loc_5_6526
        arpl   word ptr [ecx + 72h], bp
        arpl   word ptr [ebp + 6ch], si
        popad
        db  072h, 02Ch
        and    byte ptr [eax + 41h], dh
        dec    eax
        cmp    eax, 382e3825h
        db  078h, 00Ah
loc_5_651A:
        add    byte ptr [eax], al
        arpl   word ptr [edi + 72h], bp
        jb     SHORT loc_5_6596
        db  070h, 074h
        and    byte ptr [ecx + 52h], al
loc_5_6526:
        and    byte ptr [esi + 72h], ah
        and    byte ptr gs:[eax + 65h], ch
        popad
        sub    al, 20h
        db  070h, 041h
        push   edx
        cmp    eax, 382e3825h
        db  078h, 00Ah
        add    byte ptr [eax], al
loc_5_653E:
        add    byte ptr [eax], al
        inc    ecx
        push   edx
        and    byte ptr [edx + 75h], ah
        jae    SHORT loc_5_65C0
        and    byte ptr [ecx + ebp*2 + 73h], ch
        db  074h, 020h
        arpl   word ptr [edi + 72h], bp
        jb     SHORT loc_5_65C7
        db  070h, 074h
        sub    al, 20h
        db  070h, 041h
        push   edx
        cmp    eax, 382e3825h
        db  078h, 02Ch
        and    byte ptr [eax + 41h], dh
        dec    eax
        cmp    eax, 382e3825h
        db  078h, 00Ah
        add    byte ptr [ecx + 52h], al
        and    byte ptr [edx + 75h], ah
loc_5_6571:
        jae    SHORT loc_5_65EC
        and    byte ptr [ecx + ebp*2 + 73h], ch
        db  074h, 020h
        db  070h, 072h
        outsd
loc_5_657C:
        bound  esp, qword ptr [ecx + 62h]
        insb
        jns    SHORT loc_5_65A2
        arpl   word ptr [ecx + 72h], bp
        arpl   word ptr [ebp + 6ch], si
        popad
        db  072h, 02Ch
        and    byte ptr [eax + 41h], dh
        dec    eax
        cmp    eax, 382e3825h
        js     SHORT loc_5_65A0
loc_5_6596:
        add    byte ptr [eax], al
        db  070h, 056h
        push   eax
        cmp    eax, 382e3825h
loc_5_65A0:
        db  078h, 020h
loc_5_65A2:
        add    byte ptr [eax], al
        insd
        imul   esi, dword ptr [ebx + 2dh], 67696c61h
        outsb
        and    byte ptr fs:[eax + 6fh], dh
        imul   ebp, dword ptr [esi + 74h], 6e007265h
        outsd
        je     SHORT loc_5_65DC
        db  077h, 069h
        db  074h, 068h
loc_5_65C0:
        imul   ebp, dword ptr [esi + 20h], 6d6d6f63h
loc_5_65C7:
        imul   esi, dword ptr [esp + esi*2 + 65h], 61722064h
        outsb
        and    byte ptr gs:[bx + 66h], ch
        and    byte ptr [esi + 50h], dl
        jae    SHORT loc_5_65DA
loc_5_65DA:
        add    byte ptr [eax], al
loc_5_65DC:
        outsb
        outsd
        db  074h, 020h
        arpl   word ptr [edi + 6dh], bp
        insd
        imul   esi, dword ptr [esp + esi*2 + 65h], 64h
loc_5_65EC:
        imul   edx, dword ptr [esi + 50h], 2e35253dh
        xor    eax, 2078h
        db  066h, 072h, 065h
        and    byte ptr gs:[eax + 4eh], dh
        js     NEAR PTR loc_5_6676
        cmp    eax, 382e3825h
        js     SHORT loc_5_6609
loc_5_6609:
        add    byte ptr [eax], al
        add    byte ptr [eax + 50h], dh
        inc    esi
        add    byte ptr [eax], ah
        push   ebx
        inc    esi
        add    byte ptr [ecx + 74h], ah
        add    byte ptr [eax + 50h], ch
        inc    esp
        cmp    eax, 322e3225h
        js     SHORT loc_5_6642
        arpl   word ptr [edx + 65h], dx
        cmp    ax, 3425h
        xor    al, 78h
        and    byte ptr [ecx + 41h], ch
        push   edx
        cmp    eax, 342e3425h
        js     SHORT loc_5_6657
        and    eax, 38253d73h
        cmp    byte ptr cs:[eax + 20h], bh
        add    byte ptr [eax], al
loc_5_6642:
        add    byte ptr [eax], al
        and    eax, 0a000063h
        imul   ebp, dword ptr [esi + 76h], 64696c61h
        and    byte ptr [edi + 70h], ch
        db  074h, 069h
        outsd
        outsb
loc_5_6657:
        and    byte ptr [0a000063h], ah
        add    byte ptr [eax], al
        add    byte ptr [ebx + 77h], dl
        popad
        db  070h, 070h
        popad
        bound  ebp, qword ptr [ebp + 20h]
        pop    edx
        db  065h, 072h, 06Fh
        sub    eax, 74696e49h
        add    byte ptr [ebx + 77h], dl
loc_5_6676:
        popad
        db  070h, 070h
        popad
        bound  ebp, qword ptr [ebp + 20h]
        push   ebp
        outsb
        sub    eax, 74696e49h
        add    byte ptr [eax], al
        add    byte ptr [esi + 69h], al
        js     SHORT loc_5_66F1
        and    byte ptr fs:[edx + 65h], bl
        jb     SHORT loc_5_6701
        sub    eax, 74696e49h
        add    byte ptr [esi + 69h], al
        js     SHORT loc_5_6701
        and    byte ptr fs:[ebp + 6eh], dl
        sub    eax, 74696e49h
        add    byte ptr [eax], al
        add    byte ptr [edi + 69h], dl
        outsb
        xor    esi, dword ptr [edx]
        and    byte ptr [ebx + 79h], dl
        db  073h, 020h
        inc    esp
        dec    esp
        dec    esp
        and    byte ptr [ecx + 74h], al
        popad
        add    byte ptr [eax], al
        push   edi
        imul   ebp, dword ptr [esi + 33h], 79532032h
        jae    SHORT loc_5_66E6
        inc    esp
        dec    esp
        dec    esp
        and    byte ptr [ebx + 6fh], al
        add    byte ptr gs:[eax], al
        push   edi
        imul   ebp, dword ptr [esi + 33h], 655a2032h
        db  072h, 06Fh
        sub    eax, 74696e49h
        and    byte ptr [ebx + 79h], dl
        jae    SHORT loc_5_6704
        inc    esp
        dec    esp
loc_5_66E6:
        dec    esp
        and    byte ptr [ecx + 74h], al
        popad
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   edi
loc_5_66F1:
        imul   ebp, dword ptr [esi + 33h], 58452032h
        inc    ebp
        das
        inc    esp
        dec    esp
        dec    esp
        and    byte ptr [ecx + 74h], al
loc_5_6701:
        popad
        add    byte ptr [eax], al
loc_5_6704:
        push   edi
        imul   ebp, dword ptr [esi + 33h], 58452032h
        inc    ebp
        das
        inc    esp
        dec    esp
        dec    esp
        and    byte ptr [ebx + 6fh], al
        add    byte ptr gs:[eax], al
        push   edi
        imul   ebp, dword ptr [esi + 33h], 61532032h
        and    byte ptr gs:[ebp + 61h], cl
        jo     SHORT loc_5_6797
        and    byte ptr fs:[esi + 69h], al
        insb
        add    byte ptr gs:[eax], al
        push   edi
        imul   ebp, dword ptr [esi + 33h], 6e552032h
        jae    SHORT loc_5_679B
        and    byte ptr gs:[ebp + 61h], cl
        jo     SHORT loc_5_67B1
        and    byte ptr fs:[esi + 69h], al
        insb
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
        push   edi
        imul   ebp, dword ptr [esi + 33h], 6f432032h
        jo     SHORT loc_5_67CF
        sub    eax, 572d6e4fh
        db  072h, 069h
        je     SHORT loc_5_67C4
        and    byte ptr [ebp + 61h], cl
        jo     SHORT loc_5_67D4
        and    byte ptr fs:[esi + 69h], al
        insb
        db  065h, 000h, 060h, 076h
        add    byte ptr [eax], al
        je     SHORT loc_5_67E8
        add    byte ptr [eax], al
        mov    byte ptr [esi], dh
        db  000h, 098h, 076h, 000h, 000h, 0A8h
        jbe    SHORT loc_5_677F
loc_5_677F:
        db  000h, 0BCh, 076h, 000h, 000h, 0D0h, 076h
        add    byte ptr [eax], al
        db  0F0h, 076h, 000h, 000h, 004h, 077h, 000h, 000h, 018h, 077h, 000h, 000h, 030h, 077h, 000h
loc_5_6797:
        db  000h, 04Ch, 077h, 000h
loc_5_679B:
        add    byte ptr [ecx + 6eh], ch
        db  076h, 061h
        insb
        imul   esp, dword ptr [eax + 50h], 64692044h
        outsb
        db  074h, 069h
        db  066h, 069h, 065h, 072h
loc_5_67B1:
        and    byte ptr [78h], ah
        add    byte ptr [eax + 50h], dh
        inc    esp
        cmp    eax, 382e3825h
        db  078h, 020h
        db  068h, 050h
loc_5_67C4:
        inc    esp
        cmp    eax, 693c0000h
        outsb
        db  076h, 061h
        insb
        db  069h
loc_5_67CF:
        add    byte ptr ds:[eax], al
        db  000h
loc_5_67D4:
        and    eax, 78322e32h
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        imul   ebp, dword ptr [esi + 76h], 64696c61h
        and    byte ptr [eax + 6fh], dh
        db  069h
loc_5_67E8:
        outsb
        je     SHORT loc_5_6850
        jb     SHORT loc_5_67ED
loc_5_67ED:
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        and    eax, 3c200073h
        jne    SHORT loc_5_6866
        imul   ebp, dword ptr [esi + 6fh], 77h
        outsb
        and    byte ptr [eax + 61h], dh
        jb     NEAR PTR loc_5_6824
        outsb
        popad
        insd
        add    byte ptr ds:[eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [ecx + 6eh], cl
        db  076h, 061h
        insb
        imul   esp, dword ptr [eax + 6fh], 6f697470h
        outsb
        and    byte ptr [63h], ah
loc_5_6824:
        and    byte ptr [esi + 69h], dh
        db  072h, 067h
        imul   ebp, dword ptr [esi + 20h], 203a6e69h
        and    byte ptr [eax], ah
        and    byte ptr [78382e38h], ah
        sub    al, 20h
        and    eax, 4dh
        add    byte ptr [edx], cl
        and    byte ptr [ecx + 69h], dh
        outsb
        db  074h, 065h
        and    byte ptr fs:[ecx + 6eh], ch
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
loc_5_6850:
        and    eax, 78382e38h
        sub    al, 20h
        and    eax, 4dh
        or     ah, byte ptr [eax]
        arpl   word ptr [ebp + 61h], bp
        outsb
        and    byte ptr [edi + 75h], ch
loc_5_6866:
        db  074h, 03Ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    eax, 78382e38h
        sub    al, 20h
        and    eax, 4dh
        or     ah, byte ptr [eax]
        imul   esi, dword ptr fs:[edx + 74h], 756f2079h
        db  074h, 03Ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    eax, 78382e38h
        sub    al, 20h
        and    eax, 4dh
        or     ah, byte ptr [eax]
        db  076h, 069h
        db  072h, 067h
        imul   ebp, dword ptr [esi + 20h], 65657266h
        cmp    ah, byte ptr [eax]
        and    byte ptr [78382e38h], ah
        sub    al, 20h
        and    eax, 4dh
        or     ah, byte ptr [eax]
        db  074h, 061h
        imul   ebp, dword ptr [esi + 74h], 66206465h
        jb     SHORT loc_5_6922
        cmp    ah, byte ptr gs:[eax]
        and    eax, 78382e38h
        sub    al, 20h
        and    eax, 4dh
        or     ah, byte ptr [eax]
        imul   esi, dword ptr fs:[edx + 74h], 20203a79h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    eax, 78382e38h
        sub    al, 20h
        and    eax, 4dh
        or     ah, byte ptr [eax]
        je     SHORT loc_5_6965
        db  070h, 065h
        cmp    ah, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   eax
        inc    esp
        pop    edi
        push   eax
        inc    ecx
        inc    edi
        inc    ebp
        push   edx
        dec    edi
        dec    esi
        dec    esp
        pop    ecx
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        push   eax
        inc    esp
        pop    edi
        push   ebx
        push   edi
        inc    ecx
        push   eax
        push   eax
        inc    ebp
        push   edx
        and    byte ptr [eax], al
        inc    ebx
        popad
        insb
        insb
        db  073h, 020h
        inc    esp
        dec    edi
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        dec    ecx
        outsb
        db  076h, 061h
        insb
loc_5_6922:
        imul   esp, dword ptr [eax + 65h], 61727478h
        and    byte ptr [esi + 6ch], ah
        popad
        jae    NEAR PTR loc_5_693B
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
loc_5_693B:
        add    byte ptr [eax + 50h], dh
        inc    esi
        cmp    eax, 382e3825h
        db  078h, 020h
        add    byte ptr [eax], al
        insd
        imul   esi, dword ptr [ebx + 2dh], 67696c61h
        outsb
        and    byte ptr fs:[eax + 6fh], dh
        imul   ebp, dword ptr [esi + 74h], 69007265h
        db  073h, 06Eh
        daa
        db  074h, 020h
        arpl   word ptr [edi + 6eh], bp
loc_5_6965:
        je     SHORT loc_5_69D9
        outsd
        insb
        insb
        and    byte ptr fs:[edx + 79h], ah
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebp + 65h], ch
        insd
        outsd
        jb     SHORT loc_5_69F3
        and    byte ptr [ebp + 61h], ch
        outsb
        popad
        jb     NEAR PTR loc_5_6983
loc_5_6983:
        add    byte ptr [esi + 6fh], ch
        db  074h, 020h
        arpl   word ptr [edi + 6dh], bp
        insd
        imul   esi, dword ptr [esp + esi*2 + 65h], 69282064h
        outsb
        db  074h, 065h
        jb     SHORT loc_5_6A07
        popad
        insb
        and    byte ptr [ebp + 65h], ch
        insd
        outsd
        db  072h, 079h
        and    byte ptr [ebp + 61h], ch
        outsb
        popad
        jb     NEAR PTR loc_5_69CB
        jb     NEAR PTR loc_5_6A20
        outsd
        db  072h, 029h
        add    byte ptr [eax], al
        add    byte ptr [esi + 72h], ah
        popad
        insd
        cmp    eax, 352e3525h
        db  078h, 020h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        jb     NEAR PTR loc_5_6A2C
        and    byte ptr gs:[eax + 65h], ch
loc_5_69CB:
        popad
        and    byte ptr fs:[eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_5_69D9:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        add    byte ptr [ecx + 64h], ch
        insb
        and    byte ptr gs:[eax + 65h], ch
        popad
        and    byte ptr fs:[eax], ah
loc_5_69F3:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_5_6A07:
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        add    byte ptr [esi + 72h], ah
        and    byte ptr gs:[eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_5_6A20:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_5_6A2C:
        and    byte ptr [eax], ah
        add    byte ptr [eax], al
        bound  esi, qword ptr [ebp + 73h]
        db  079h, 020h
        popad
        insb
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        and    byte ptr fs:[eax + 68h], dh
        db  079h, 073h
        and    byte ptr [edi + 2fh], dh
        outsd
        and    byte ptr [ecx + ebp*2 + 6eh], ch
        and    byte ptr [ebp + 61h], ch
        jo     SHORT loc_5_6A70
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        imul   ebp, dword ptr [esi + 76h], 64696c61h
        and    byte ptr [eax + 56h], dh
        push   eax
        cmp    eax, 382e3825h
        js     SHORT loc_5_6A66
loc_5_6A66:
        add    byte ptr [eax], al
        imul   esp, dword ptr [esp + ebp*2 + 65h], 0
loc_5_6A70:
        bound  esi, qword ptr [ebp + 73h]
        jns    SHORT loc_5_6A75
loc_5_6A75:
        add    byte ptr [eax], al
        add    byte ptr [63252073h], ah
        and    byte ptr [eax + 56h], dh
        push   eax
        cmp    eax, 382e3825h
        js     SHORT loc_5_6AA8
        popad
        db  074h, 061h
        cmp    eax, 382e3825h
        js     SHORT loc_5_6AB3
        add    byte ptr [esi + 6ch], ah
        cmp    eax, 352e3525h
        db  078h, 020h
        bound  ebp, qword ptr [ebp + edi + 25h]
        xor    eax, 78352eh
        add    byte ptr [eax], al
loc_5_6AA8:
        arpl   word ptr [edi + ebp*2 + 63h], cx
        imul   edi, dword ptr [342e3425h], 78h
loc_5_6AB3:
        and    byte ptr [eax], ah
        arpl   word ptr [edx + 65h], dx
        cmp    ax, 3425h
        xor    al, 78h
        add    byte ptr [edx], cl
        imul   ebp, dword ptr [esi + 76h], 64696c61h
        and    byte ptr [edi + 70h], ch
        je     SHORT loc_5_6B36
        outsd
        outsb
        and    byte ptr [0a000063h], ah
        add    byte ptr [eax], al
        add    byte ptr [ecx + 6eh], ch
        db  076h, 061h
        insb
        imul   esp, dword ptr [eax + 6fh], 6f697470h
        outsb
        and    byte ptr [edi], ah
        inc    edx
        daa
        add    byte ptr [eax], al
        db  070h, 043h
        inc    esp
        cmp    eax, 382e3825h
        db  078h, 020h
        add    byte ptr [eax], al
        and    byte ptr [ecx + 6eh], ch
        db  076h, 061h
        insb
        imul   esp, dword ptr [eax + 70h], 746e696fh
        jb     NEAR PTR loc_5_6B09
loc_5_6B09:
        add    byte ptr [eax], al
        add    byte ptr [eax + 72h], dl
        db  076h, 050h
        inc    esp
        inc    ebp
        cmp    eax, 382e3825h
        db  078h, 020h
        and    eax, 78332e33h
        and    byte ptr [eax], ah
        db  070h, 041h
        dec    eax
        cmp    eax, 382e3825h
        js     SHORT loc_5_6B4A
        db  070h, 04Eh
        js     NEAR PTR loc_5_6BA3
        cmp    eax, 382e3825h
        js     SHORT loc_5_6B36
loc_5_6B36:
        add    byte ptr [eax], al
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        imul   ebp, dword ptr [esi + 76h], 64696c61h
        and    byte ptr [edi + 70h], ch
        je     SHORT loc_5_6BB1
        outsd
        outsb
loc_5_6B4A:
        and    byte ptr [edi], ah
        inc    edx
        daa
        add    byte ptr [eax], al
        jo     SHORT loc_5_6BC2
        insb
        cmp    eax, 382e3825h
        db  078h, 020h
        add    byte ptr [eax], al
        and    byte ptr [ecx + 6eh], ch
        jbe    SHORT loc_5_6BC2
        insb
        imul   esp, dword ptr [eax + 70h], 746e696fh
        jb     NEAR PTR loc_5_6B6D
loc_5_6B6D:
        add    byte ptr [eax], al
        add    byte ptr [eax + 68h], dl
        db  079h, 073h
        push   ebx
        db  074h, 061h
        jb     SHORT loc_5_6BED
        cmp    eax, 382e3825h
        db  078h, 020h
        inc    ebp
        outsb
        cmp    eax, 382e3825h
        db  078h, 020h
        dec    esp
        imul   ebp, dword ptr [esi + 53h], 74726174h
        cmp    eax, 382e3825h
        js     SHORT loc_5_6BB9
        jo     SHORT loc_5_6BE9
        db  065h, 078h, 074h
        cmp    eax, 382e3825h
loc_5_6BA3:
        js     SHORT loc_5_6BA5
loc_5_6BA5:
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [ecx + 6eh], ch
        db  076h, 061h
        insb
loc_5_6BB1:
        imul   esp, dword ptr [eax + 6fh], 6f697470h
loc_5_6BB9:
        outsb
        and    byte ptr [edi], ah
        inc    edx
        daa
        add    byte ptr [eax], al
        jo     SHORT loc_5_6C32
loc_5_6BC2:
        db  072h, 03Dh
        and    eax, 78382e38h
        and    byte ptr [eax], al
        add    byte ptr [eax], ah
        imul   ebp, dword ptr [esi + 76h], 64696c61h
        and    byte ptr [eax + 6fh], dh
        imul   ebp, dword ptr [esi + 74h], 7265h
        add    byte ptr [eax], al
        push   eax
        push   61507379h
        push   ebx
loc_5_6BE9:
        je     SHORT loc_5_6C4C
        db  072h, 074h
loc_5_6BED:
        cmp    eax, 352e3525h
        db  078h, 020h
        push   eax
        push   61507379h
        inc    ebp
        outsb
        cmp    eax, 352e3525h
        js     SHORT loc_5_6C06
loc_5_6C06:
        add    byte ptr [eax], al
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        imul   ebp, dword ptr [esi + 76h], 64696c61h
        and    byte ptr [edi + 70h], ch
        db  074h, 069h
        outsd
        outsb
        and    byte ptr [63h], ah
        jo     SHORT loc_5_6C63
        dec    eax
        cmp    eax, 382e3825h
        js     SHORT loc_5_6C4A
        add    byte ptr [eax], al
        and    byte ptr [ecx + 6eh], ch
        jbe    SHORT loc_5_6C92
        insb
loc_5_6C32:
        imul   esp, dword ptr [eax + 70h], 746e696fh
        jb     NEAR PTR loc_5_6C3D
loc_5_6C3D:
        add    byte ptr [eax], al
        add    byte ptr [eax + 72h], dl
        jbe    SHORT loc_5_6C44
loc_5_6C44:
        push   ebx
        push   79530072h
loc_5_6C4A:
        jae    SHORT loc_5_6C4C
loc_5_6C4C:
        dec    ecx
        outsb
        db  076h, 061h
        insb
        imul   esp, dword ptr [eax + 63h], 65746e6fh
        outsb
        db  074h, 073h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        and    byte ptr [eax + 61h], dh
loc_5_6C63:
        db  067h, 065h, 073h, 03Dh
        and    eax, 78352e35h
        and    byte ptr [edi + ebp*2 + 20h], dh
        and    eax, 78352e35h
        and    byte ptr [eax + 46h], dh
        db  072h, 065h
        cmp    eax, 382e3825h
        js     SHORT loc_5_6CA2
        db  070h, 042h
        jne    SHORT loc_5_6CF9
        db  079h, 03Dh
        and    eax, 78382e38h
        and    byte ptr [ecx + 42h], ch
        db  075h, 073h
loc_5_6C92:
        db  079h, 03Dh
        and    eax, 78342e34h
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [ebp + 6dh], ch
loc_5_6CA2:
        inc    ebx
        push   486b6365h
        popad
        db  070h, 03Ah
        and    byte ptr [edx + 61h], ah
        and    byte ptr fs:[eax + 65h], ch
        popad
        db  064h, 065h, 072h, 020h
        db  073h, 069h
        outsb
        popad
        db  074h, 075h
        jb     SHORT loc_5_6D26
        or     al, byte ptr [eax]
        add    byte ptr [edx + esi*2 + 61h], dh
        db  073h, 068h
        and    byte ptr fs:[ecx + 74h], ah
        and    byte ptr [0a78h], ah
        insd
        insd
        inc    ebx
        push   486b6365h
        popad
        db  070h, 03Ah
        and    byte ptr [edx + 61h], ah
        and    byte ptr fs:[edx + 6ch], ah
        outsd
        arpl   word ptr [ebx + 20h], bp
        popad
        db  064h, 064h, 072h, 065h
        jae    SHORT loc_5_6D65
        or     al, byte ptr [eax]
        je     SHORT loc_5_6D68
        popad
        jae    SHORT loc_5_6D61
loc_5_6CF9:
        and    byte ptr fs:[ecx + 64h], ah
        jb     NEAR PTR loc_5_6D21
        and    eax, 0a78h
        add    byte ptr [eax], al
        insd
        insd
        inc    ebx
        push   486b6365h
        popad
        jo     SHORT loc_5_6D4E
        and    byte ptr [edx + 61h], ah
        and    byte ptr fs:[esi + 72h], ah
        and    byte ptr gs:[edx + 6ch], ah
        outsd
loc_5_6D21:
        arpl   word ptr [ebx + 0ah], bp
        add    byte ptr [eax], al
loc_5_6D26:
        add    byte ptr [eax], al
        db  074h, 072h
        popad
        db  073h, 068h
        and    byte ptr fs:[ecx + 64h], ah
        db  064h, 072h, 020h
        outsb
        popad
        db  072h, 020h
        and    eax, 726f2078h
        and    byte ptr [726f2078h], ah
        and    byte ptr [0a78h], ah
        add    byte ptr [ebp + 6dh], ch
loc_5_6D4E:
        inc    ebx
        push   486b6365h
        popad
        db  070h, 03Ah
        and    byte ptr [esi + 72h], ah
        and    byte ptr gs:[edx + 6ch], ah
        outsd
loc_5_6D61:
        arpl   word ptr [ebx + 20h], bp
        outsb
loc_5_6D65:
        outsd
        db  074h, 020h
loc_5_6D68:
        imul   ebp, dword ptr [esi + 20h], 65657266h
        and    byte ptr [ecx + ebp*2 + 73h], ch
        db  074h, 020h
        jo     SHORT loc_5_6DE9
        outsd
        db  070h, 065h
        jb     SHORT loc_5_6DE8
        db  079h, 00Ah
        add    byte ptr [eax], al
        je     SHORT loc_5_6DF4
        popad
        db  073h, 068h
        and    byte ptr fs:[ecx + 64h], ah
        jb     NEAR PTR loc_5_6DAD
        db  070h, 072h
        outsd
        bound  esp, qword ptr [ecx + 62h]
        insb
        jns    SHORT loc_5_6DB6
        outsb
        popad
        jb     SHORT loc_5_6DBB
        and    eax, 726f2078h
        and    byte ptr [726f2078h], ah
        and    byte ptr [0a78h], ah
        insd
loc_5_6DAD:
        insd
        inc    ebx
        push   486b6365h
        popad
loc_5_6DB6:
        jo     SHORT loc_5_6DF2
        and    byte ptr [edx + 75h], ah
loc_5_6DBB:
        jae    SHORT loc_5_6E36
        and    byte ptr [edx + 6ch], ah
        outsd
        arpl   word ptr [ebx + 20h], bp
        insd
        popad
        jb     SHORT loc_5_6E33
        and    byte ptr fs:[esi + 72h], ah
        or     al, byte ptr gs:[eax]
        add    byte ptr [eax], al
        add    byte ptr [edx + esi*2 + 61h], dh
        jae    SHORT loc_5_6E41
        and    byte ptr fs:[ecx + 64h], ah
        db  064h, 072h, 020h
        and    eax, 0a78h
        add    byte ptr [eax], al
loc_5_6DE8:
        insd
loc_5_6DE9:
        insd
        inc    ebx
        push   486b6365h
        popad
loc_5_6DF2:
        jo     SHORT loc_5_6E2E
loc_5_6DF4:
        and    byte ptr [esi + 72h], ah
        and    byte ptr gs:[edx + 6ch], ah
        outsd
        arpl   word ptr [ebx + 20h], bp
        db  074h, 061h
        imul   ebp, dword ptr [eax + 64h], 6e73656fh
        daa
        je     SHORT loc_5_6E2D
        db  070h, 06Fh
        imul   ebp, dword ptr [esi + 74h], 206f7420h
        push   0a646165h
        add    byte ptr [edx + esi*2 + 61h], dh
        db  073h, 068h
        and    byte ptr fs:[ecx + 74h], ah
        and    byte ptr [0a78h], ah
        dec    eax
loc_5_6E2D:
        push   eax
loc_5_6E2E:
        pop    edi
        push   eax
        push   edx
        inc    ebp
        push   esi
loc_5_6E33:
        inc    esi
        push   edx
        inc    ebp
loc_5_6E36:
        inc    ebp
        and    byte ptr [ebp + 72h], ah
        jb     SHORT loc_5_6EAB
        outsb
        outsd
        jne    SHORT loc_5_6EB4
loc_5_6E41:
        insb
        jns    SHORT loc_5_6E64
        jae    SHORT loc_5_6EAB
        db  074h, 00Ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  074h, 072h
        popad
        db  073h, 068h
        and    byte ptr fs:[ecx + 74h], ah
        and    byte ptr [0a78h], ah
        insd
        insd
        inc    ebx
        push   486b6365h
loc_5_6E64:
        popad
        db  070h, 03Ah
        and    byte ptr [eax + 50h], cl
        pop    edi
        push   eax
        push   edx
        inc    ebp
        push   esi
        inc    esi
        push   edx
        inc    ebp
        inc    ebp
        and    byte ptr [esi + 6fh], ch
        db  074h, 020h
        db  073h, 065h
        db  074h, 00Ah
        add    byte ptr [eax], al
        add    byte ptr [edx + esi*2 + 61h], dh
        db  073h, 068h
        and    byte ptr fs:[ecx + 64h], ah
        jb     NEAR PTR loc_5_6EAD
        and    eax, 0a78h
        add    byte ptr [eax], al
        insd
        insd
        inc    ebx
        push   486b6365h
        popad
        db  070h, 03Ah
        and    byte ptr [edx + 61h], ah
        and    byte ptr fs:[edx + 6ch], ah
        outsd
        arpl   word ptr [ebx + 20h], bp
loc_5_6EAB:
        jae    SHORT loc_5_6F16
loc_5_6EAD:
        outsb
        popad
        db  074h, 075h
        db  072h, 065h
loc_5_6EB4:
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        je     SHORT loc_5_6F2C
        popad
        jae    SHORT loc_5_6F25
        and    byte ptr fs:[ecx + 64h], ah
        db  064h, 072h, 020h
        and    eax, 0a78h
        add    byte ptr [eax], al
        insd
        insd
        inc    ebx
        push   486b6365h
        popad
        db  070h, 03Ah
        and    byte ptr [edx + 61h], ah
        and    byte ptr fs:[ebp + 6eh], ah
        and    byte ptr fs:[ebx + 65h], dh
        outsb
        db  074h, 069h
        outsb
        insb
        or     al, byte ptr [eax]
        add    byte ptr [edx + esi*2 + 61h], dh
        db  073h, 068h
        and    byte ptr fs:[ecx + 64h], ah
        db  064h, 072h, 020h
        bound  esp, qword ptr [ebp + 74h]
        ja     SHORT loc_5_6F63
        outsb
        and    byte ptr [6e612078h], ah
        and    byte ptr fs:[6d000a78h], ah
        insd
        inc    ebx
        push   486b6365h
        popad
loc_5_6F16:
        jo     SHORT loc_5_6F52
        and    byte ptr [esi + 72h], ah
        and    byte ptr gs:[ecx + ebp*2 + 73h], ch
        je     SHORT loc_5_6F43
        jo     SHORT loc_5_6F94
loc_5_6F25:
        imul   ebp, dword ptr [esi + 74h], 70207265h
loc_5_6F2C:
        outsd
        imul   ebp, dword ptr [esi + 74h], 756f2073h
        db  074h, 073h
        imul   esp, dword ptr [ebp + 20h], 70616568h
        and    byte ptr [edx + 6fh], ah
        db  075h, 06Eh
loc_5_6F43:
        db  064h, 073h, 00Ah
        add    byte ptr [eax], al
        je     SHORT loc_5_6FBC
        popad
        db  073h, 068h
        and    byte ptr fs:[ecx + 64h], ah
loc_5_6F52:
        db  064h, 072h, 020h
        jo     SHORT loc_5_6FC9
        outsd
        bound  esp, qword ptr [ecx + 62h]
        insb
        db  079h, 020h
        and    eax, 0a78h
loc_5_6F63:
        add    byte ptr [ebp + 6dh], ch
        inc    ebx
        push   486b6365h
        popad
        jo     SHORT loc_5_6FAA
        and    byte ptr [edx + 61h], ah
        and    byte ptr fs:[esi + 72h], ah
        and    byte ptr gs:[edx + 6ch], ah
        outsd
        arpl   word ptr [ebx + 20h], bp
        db  073h, 069h
        outsb
        popad
        db  074h, 075h
        jb     SHORT loc_5_6FEE
        and    byte ptr [edi + 6eh], ch
        and    byte ptr [ecx + ebp*2 + 73h], ch
        db  074h, 00Ah
        add    byte ptr [eax], al
loc_5_6F94:
        je     SHORT loc_5_7008
        popad
        db  073h, 068h
        and    byte ptr fs:[ecx + 64h], ah
        db  064h, 072h, 020h
        imul   esi, dword ptr gs:[eax + ebp*2 + 65h], 78252072h
loc_5_6FAA:
        and    byte ptr [edi + 72h], ch
        and    byte ptr [0a78h], ah
        add    byte ptr [ebp + 6dh], ch
        inc    ebx
        push   486b6365h
loc_5_6FBC:
        popad
        db  070h, 03Ah
        and    byte ptr [edx + 61h], ah
        and    byte ptr fs:[ebx + 69h], dh
        db  07Ah, 065h
loc_5_6FC9:
        and    byte ptr [edi + 66h], ch
        and    byte ptr [esi + 72h], ah
        and    byte ptr gs:[ecx + ebp*2 + 73h], ch
        je     SHORT loc_5_6FF7
        push   0a646165h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  074h, 072h
        popad
        db  073h, 068h
        and    byte ptr fs:[ecx + 64h], ah
        db  064h, 072h, 020h
        outsb
loc_5_6FEE:
        popad
        db  072h, 020h
        and    eax, 726f2078h
loc_5_6FF7:
        and    byte ptr [6d000a78h], ah
        insd
        inc    ebx
        push   486b6365h
        popad
        db  070h, 03Ah
loc_5_7008:
        and    byte ptr [esi + 72h], ah
        and    byte ptr gs:[ecx + ebp*2 + 73h], ch
        db  074h, 020h
        push   20646165h
        outsd
        jne    SHORT loc_5_708F
        and    byte ptr [edi + 66h], ch
        and    byte ptr [edi + 72h], ch
        db  064h, 065h, 072h, 00Ah
        add    byte ptr [eax], al
        add    byte ptr [edx + esi*2 + 61h], dh
        db  073h, 068h
        and    byte ptr fs:[ecx + 64h], ah
        db  064h, 072h, 020h
        db  070h, 072h
        outsd
        bound  esp, qword ptr [ecx + 62h]
        insb
        db  079h, 020h
        outsb
        popad
        jb     SHORT loc_5_7063
        and    eax, 726f2078h
        and    byte ptr [0a78h], ah
        add    byte ptr [eax], al
        insd
        insd
        inc    ebx
        push   486b6365h
        popad
        db  070h, 03Ah
        and    byte ptr [edx + 61h], ah
        and    byte ptr fs:[esi + 72h], ah
loc_5_7063:
        and    byte ptr gs:[edx + 6ch], ah
        outsd
        arpl   word ptr [ebx + 20h], bp
        outsd
        outsb
        and    byte ptr [esi + 72h], ah
        and    byte ptr gs:[ecx + ebp*2 + 73h], ch
        db  074h, 00Ah
        add    byte ptr [eax], al
        add    byte ptr [edx + esi*2 + 61h], dh
        jae    SHORT loc_5_70E9
        and    byte ptr fs:[ecx + 64h], ah
        db  064h, 072h, 020h
        db  070h, 072h
        outsd
        bound  esp, qword ptr [ecx + 62h]
loc_5_708F:
        insb
        db  079h, 020h
        and    eax, 726f2078h
        and    byte ptr [6d000a78h], ah
        insd
        inc    ebx
        push   486b6365h
        popad
        db  070h, 03Ah
        and    byte ptr [esi + 72h], ah
        and    byte ptr gs:[ecx + ebp*2 + 73h], ch
        db  074h, 020h
        outsb
        outsd
        je     SHORT loc_5_70D7
        db  073h, 06Fh
        jb     SHORT loc_5_712F
        and    byte ptr fs:[edx + 69h], dh
        push   0a74h
        add    byte ptr [eax], al
        db  074h, 072h
        popad
        db  073h, 068h
        and    byte ptr fs:[ecx + 64h], ah
        db  064h, 072h, 020h
        db  070h, 072h
loc_5_70D7:
        outsd
        bound  esp, qword ptr [ecx + 62h]
        insb
        db  079h, 020h
        and    eax, 726f2078h
        and    byte ptr [6d000a78h], ah
loc_5_70E9:
        insd
        inc    ebx
        push   486b6365h
        popad
        db  070h, 03Ah
        and    byte ptr [edx + 61h], ah
        and    byte ptr fs:[esi + 72h], ah
        and    byte ptr gs:[ecx + ebp*2 + 73h], ch
        db  074h, 020h
        push   20646165h
        arpl   word ptr [edi + 75h], bp
        outsb
        db  074h, 00Ah
        add    byte ptr [eax], al
        je     SHORT loc_5_7184
        popad
        jae    SHORT loc_5_717D
        and    byte ptr fs:[ebx + 6fh], dh
        insd
        ja     NEAR PTR loc_5_7186
        jb     NEAR PTR loc_5_7186
        and    byte ptr [edx + 65h], ah
        je     SHORT loc_5_719D
        outsb
        and    byte ptr [6e612078h], ah
loc_5_712F:
        and    byte ptr fs:[0a78h], ah
        add    byte ptr [eax], al
        dec    ebp
        imul   esi, dword ptr [ebx + 2dh], 67696c61h
        outsb
        and    byte ptr fs:[eax + 6fh], dh
        imul   ebp, dword ptr [esi + 74h], 25207265h
        js     SHORT loc_5_714F
loc_5_714F:
        add    byte ptr [ecx + 72h], ah
        db  067h, 075h, 06Dh
        outsb
        db  074h, 020h
        je     SHORT loc_5_71CA
        and    byte ptr [esi], ch
        insd
        push   756d2073h
        db  073h, 074h
        and    byte ptr [edx + 65h], ah
        and    byte ptr [ecx + 20h], ah
        push   20706165h
        push   65646165h
        jb     SHORT loc_5_7177
loc_5_7177:
        add    byte ptr [eax + 65h], cl
        popad
        jo     SHORT loc_5_719D
loc_5_717D:
        imul   esi, dword ptr [ebx + 20h], 72726f63h
loc_5_7184:
        db  075h, 070h
loc_5_7186:
        db  074h, 02Ch
        and    byte ptr [ebp + 73h], dh
        and    byte ptr gs:[esi], ch
        insd
        push   6f742078h
        and    byte ptr [ecx + ebp*2 + 61h], ah
        outsb
        outsd
        jae    SHORT loc_5_7202
loc_5_719D:
        add    byte ptr [eax], al
        add    byte ptr [ebx + 75h], dl
        insd
        insd
        popad
        db  072h, 079h
        and    byte ptr [edi + 66h], ch
        and    byte ptr [eax + 65h], ch
        popad
        jo     SHORT loc_5_71D0
        and    eax, 78382e38h
        or     al, byte ptr [edx + 75h]
        jae    SHORT loc_5_7233
        and    byte ptr [edx + 6ch], ah
        outsd
        arpl   word ptr [ebx + 73h], bp
        cmp    ah, byte ptr [eax]
        and    eax, 78342e34h
        and    byte ptr [eax], ah
loc_5_71CA:
        inc    edx
        jns    SHORT loc_5_7241
        jae    NEAR PTR loc_5_71F0
loc_5_71D0:
        arpl   word ptr [edi + 6dh], bp
        insd
        imul   esi, dword ptr [esp + esi*2 + 65h], 25203a64h
        cmp    byte ptr [esi], ch
        cmp    byte ptr [eax + 20h], bh
        and    byte ptr [edi + 66h], cl
        and    byte ptr [eax + ebp*2 + 61h], dh
        db  074h, 02Ch
        and    byte ptr [edi + 76h], ch
        jb     NEAR PTR loc_5_7258
loc_5_71F0:
        popad
        cmp    ah, byte ptr fs:[eax]
loc_5_71F5:
        and    eax, 78382e38h
        or     al, byte ptr [esi + 72h]
        and    byte ptr gs:[edx + 6ch], ah
loc_5_7202:
        outsd
        arpl   word ptr [ebx + 73h], bp
        cmp    ah, byte ptr [eax]
        and    eax, 78342e34h
        and    byte ptr [eax], ah
        inc    edx
        db  079h, 074h
        db  065h, 073h, 020h
        arpl   word ptr [edi + 6dh], bp
        insd
        imul   esi, dword ptr [esp + esi*2 + 65h], 25203a64h
        cmp    byte ptr [esi], ch
        cmp    byte ptr [eax + 20h], bh
        and    byte ptr [edx + 79h], al
        db  074h, 065h
        jae    SHORT loc_5_724D
loc_5_722D:
        db  075h, 06Eh
        arpl   word ptr [edi + 6dh], bp
        insd
loc_5_7233:
        imul   esi, dword ptr [esp + esi*2 + 65h], 25203a64h
        cmp    byte ptr [esi], ch
        cmp    byte ptr [eax], bh
        popad
loc_5_7241:
        db  072h, 067h
        jne    SHORT loc_5_72B2
        outsb
        db  074h, 020h
        je     SHORT loc_5_72BA
        and    byte ptr [esi], ch
loc_5_724D:
        insd
        push   756d2077h
        db  073h, 074h
        and    byte ptr [edx + 65h], ah
loc_5_7258:
        and    byte ptr [ecx + 20h], ah
        push   20706165h
        push   65646165h
        jb     SHORT loc_5_7267
loc_5_7267:
        add    byte ptr [eax + 65h], cl
        popad
        db  070h, 020h
        and    eax, 73692078h
        and    byte ptr [edi + 4bh], cl
        add    byte ptr [eax], al
        add    byte ptr [esi], ch
        insd
        push   6f642077h
        jae    NEAR PTR loc_5_72F0
loc_5_7282:
        daa
        db  074h, 020h
        db  077h, 06Fh
        db  072h, 06Bh
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [edx + 65h], dh
        je     SHORT loc_5_72F2
        imul   ebp, dword ptr [edx + ecx], 48000000h
        popad
        jb     NEAR PTR loc_5_72DC
        and    eax, 78382e38h
        and    byte ptr [ebx + 62h], ah
        cmp    eax, 382e3825h
        db  078h, 020h
loc_5_72AE:
        add    byte ptr [eax], al
        inc    ebp
        outsb
loc_5_72B2:
        and    byte ptr fs:[ebx + 65h], dl
        outsb
        db  074h, 069h
        outsb
loc_5_72BA:
        insb
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    edx
        jne    SHORT loc_5_7336
        db  079h, 020h
        jo     SHORT loc_5_730B
        popad
        je     SHORT loc_5_732B
        cmp    eax, 382e3825h
loc_5_72CF:
        js     SHORT loc_5_72D1
loc_5_72D1:
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        push   eax
        jb     SHORT loc_5_733D
        db  076h, 046h
        jb     SHORT loc_5_7341
loc_5_72DC:
        add    byte ptr gs:[eax], al
        add    byte ptr [edx], cl
        bound  esp, qword ptr [ecx + 63h]
        imul   esi, dword ptr [edi + 61h], 72h
        and    byte ptr fs:[ecx + ebp*2 + 6eh], ch
        imul   esp, dword ptr [eax], 69h
loc_5_72F0:
        outsb
        popad
loc_5_72F2:
        arpl   word ptr [ebx + 65h], sp
        jae    SHORT loc_5_736A
        imul   esp, dword ptr [edx + 6ch], 690a0065h
        outsb
        db  076h, 061h
        insb
        imul   esp, dword ptr [eax + 6fh], 6f697470h
        outsb
loc_5_730B:
        and    byte ptr [25000063h], ah
loc_5_7311:
        cmp    byte ptr [esi], ch
        cmp    byte ptr [eax + 20h], bh
        insd
        insd
        outsd
        jb     SHORT loc_5_7395
        and    byte ptr [esi + 6fh], ch
        je     SHORT loc_5_7341
        jo     SHORT loc_5_7395
        jae    NEAR PTR loc_5_738B
        outsb
        je     SHORT loc_5_7329
loc_5_7329:
        add    byte ptr [eax], al
loc_5_732B:
        add    byte ptr [esi + 72h], al
        and    byte ptr gs:[eax + 4eh], dh
        js     NEAR PTR loc_5_73AA
loc_5_7336:
        cmp    eax, 382e3825h
        db  078h, 020h
loc_5_733D:
        db  070h, 050h
        db  072h, 065h
loc_5_7341:
        db  076h, 03Dh
        and    eax, 78382e38h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        and    byte ptr [eax + 65h], cl
        popad
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], ah
        dec    ebp
        popad
        js     SHORT loc_5_7396
        and    eax, 78382e38h
        add    byte ptr [eax], al
        push   70616548h
        cmp    eax, 382e3825h
loc_5_736A:
        db  078h, 020h
        arpl   word ptr [edx + 52h], sp
        jae    NEAR PTR loc_5_73D7
        db  072h, 076h
        cmp    eax, 382e3825h
        db  078h, 020h
        db  070h, 046h
        db  072h, 065h
        cmp    eax, 382e3825h
        js     SHORT loc_5_7389
loc_5_7389:
        add    byte ptr [eax], al
loc_5_738B:
        add    byte ptr [eax], ah
        push   ebx
        jb     NEAR PTR loc_5_73FA
        popad
        insb
        add    byte ptr [eax], ah
loc_5_7395:
        dec    esp
loc_5_7396:
        outsd
        arpl   word ptr [ebx], bp
        add    byte ptr [eax], al
        or     ch, byte ptr [ecx + 6eh]
        db  076h, 061h
        insb
        imul   esp, dword ptr [eax + 6fh], 6f697470h
loc_5_73AA:
        outsb
        and    byte ptr [25000063h], ah
        cmp    byte ptr [esi], ch
        cmp    byte ptr [eax + 20h], bh
        insd
        insd
        outsd
        db  072h, 079h
        and    byte ptr [esi + 6fh], ch
        db  074h, 020h
        db  070h, 072h
        jae    NEAR PTR loc_5_742B
        outsb
        je     SHORT loc_5_73E9
        outsd
        jb     SHORT loc_5_73EC
        imul   ebp, dword ptr [esi + 76h], 64696c61h
        and    byte ptr [eax + 65h], ch
        popad
loc_5_73D7:
        db  070h, 020h
        jo     SHORT loc_5_744A
        imul   ebp, dword ptr [esi + 74h], 7265h
        add    byte ptr [eax], al
        and    eax, 78382e38h
loc_5_73E9:
        and    byte ptr [ebp + 65h], ch
loc_5_73EC:
        insd
        outsd
        db  072h, 079h
        and    byte ptr [esi + 6fh], ch
        db  074h, 020h
        db  070h, 072h
        db  065h, 073h, 065h
loc_5_73FA:
        outsb
        je     SHORT loc_5_73FD
loc_5_73FD:
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [ebx + 77h], dl
        popad
        jo     SHORT loc_5_7409
loc_5_7409:
        add    byte ptr [eax], al
        add    byte ptr [ebx + 77h], dl
        popad
        jo     SHORT loc_5_7411
loc_5_7411:
        add    byte ptr [eax], al
        add    byte ptr [esi + 69h], al
        db  078h, 065h
        add    byte ptr fs:[eax], al
        add    byte ptr [esi + 69h], al
        js     SHORT loc_5_7485
        add    byte ptr fs:[eax], al
        add    byte ptr [ebp + 58h], al
        inc    ebp
        das
        inc    esp
        dec    esp
        dec    esp
loc_5_742B:
        add    byte ptr [ebp + 58h], al
        inc    ebp
        das
        inc    esp
        dec    esp
        dec    esp
        add    byte ptr [ebp + 58h], al
        inc    ebp
        das
        inc    esp
        dec    esp
        dec    esp
        add    byte ptr [ebp + 58h], al
        inc    ebp
        das
        inc    esp
        dec    esp
        dec    esp
        add    byte ptr [ebp + 58h], al
        inc    ebp
        das
        inc    esp
        dec    esp
loc_5_744A:
        dec    esp
        add    byte ptr [ebp + 61h], cl
        jo     SHORT loc_5_7496
        imul   ebp, dword ptr [ebp], 4670614dh
        imul   ebp, dword ptr [ebp], 46574f43h
        db  069h, 06Ch, 065h, 000h, 004h, 084h, 000h, 000h
        or     al, 84h
        add    byte ptr [eax], al
        adc    al, 84h
        add    byte ptr [eax], al
        sbb    al, 84h
        add    byte ptr [eax], al
        and    al, 84h
        add    byte ptr [eax], al
        sub    al, 84h
        add    byte ptr [eax], al
        xor    al, 84h
        add    byte ptr [eax], al
        cmp    al, 84h
        add    byte ptr [eax], al
        inc    esp
loc_5_7485:
        test   byte ptr [eax], al
        db  000h, 04Ch, 084h, 000h
        db  000h, 054h, 084h, 000h
        db  000h, 05Ch, 084h, 000h
        add    byte ptr [ecx + 6eh], cl
loc_5_7496:
        jbe    SHORT loc_5_74F9
        insb
        imul   esp, dword ptr [eax + eax + 25h], 73382dh
        add    byte ptr [eax], al
        add    byte ptr [eax + 61h], dl
        jb     NEAR PTR loc_5_74CF
        xor    ch, byte ptr [esi]
        xor    ah, byte ptr [eax]
        push   eax
        popad
        inc    edx
        popad
        db  073h, 065h
        cmp    eax, 382e3825h
        db  078h, 020h
        add    byte ptr [eax + 61h], dh
        and    byte ptr gs:[si + 61h], dh
        bound  ebp, qword ptr [ebp + 20h]
        outsb
        outsd
        je     SHORT loc_5_74EF
loc_5_74CF:
        add    byte ptr [ebx + 6fh], ah
        insd
        insd
        imul   esi, dword ptr [esp + esi*2 + 65h], 64h
        db  070h, 072h
        db  065h, 073h, 065h
        outsb
        je     SHORT loc_5_74E4
loc_5_74E4:
        sub    al, 20h
        outsb
        outsd
        and    byte ptr [eax + 65h], dh
        jb     SHORT loc_5_751A
        db  070h, 061h
loc_5_74EF:
        and    byte ptr gs:[bx + di + 6eh], ch
        outsw
        jb     SHORT loc_5_7565
        popad
loc_5_74F9:
        db  074h, 069h
        outsd
        outsb
        and    byte ptr [ecx + 76h], ah
        popad
        imul   ebp, dword ptr [ecx + 62h], 6300656ch
        outsd
        insd
        insd
        imul   esi, dword ptr [esp + esi*2 + 65h], 2064h
        jb     SHORT loc_5_7545
        ja     SHORT loc_5_7538
        add    byte ptr [eax], al
loc_5_751A:
        add    byte ptr [eax], al
        jb     SHORT loc_5_754D
        outsd
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp + 73h], dh
        db  065h, 072h, 020h
        and    byte ptr [eax], ah
        add    byte ptr [ebx + 79h], dh
        jae    SHORT loc_5_75A4
        insd
        and    byte ptr [eax], al
        jo     SHORT loc_5_759E
        db  079h, 073h
loc_5_7538:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        db  070h, 072h
        db  065h, 073h, 065h
        outsb
        db  074h, 020h
        push   eax
loc_5_7545:
        push   64417379h
        jb     NEAR PTR loc_5_758A
loc_5_754D:
        and    eax, 78382e38h
        add    byte ptr [eax], al
        imul   ebp, dword ptr [esi + 76h], 64696c61h
        and    byte ptr [eax + 46h], dl
        add    byte ptr [eax], al
        db  070h, 072h
        db  065h, 073h, 065h
loc_5_7565:
        outsb
        je     SHORT loc_5_7588
        and    byte ptr [eax + 68h], dl
        db  079h, 073h
        inc    ecx
        jb     NEAR PTR loc_5_75AF
        and    eax, 78382e38h
        add    byte ptr [eax], ah
        insb
        outsd
        arpl   word ptr [ebx + 65h], bp
        add    byte ptr fs:[eax + 68h], dh
        db  079h, 073h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_5_7588:
        add    byte ptr [eax], al
loc_5_758A:
        add    byte ptr [eax], al
        outsb
        outsd
        db  074h, 02Dh
        jo     SHORT loc_5_7604
        db  065h, 073h, 065h
        outsb
        je     SHORT loc_5_7598
loc_5_7598:
        db  072h, 065h
        db  073h, 065h
        jb     SHORT loc_5_7614
loc_5_759E:
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], al
loc_5_75A4:
        db  066h, 072h, 065h
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
        or     ah, byte ptr [eax]
        dec    edi
loc_5_75AF:
        bound  ebp, qword ptr [edx + 42h]
        popad
        db  073h, 065h
        cmp    eax, 382e3825h
        js     SHORT loc_5_75DC
        arpl   word ptr [eax + 67h], si
        cmp    eax, 352e3525h
        js     SHORT loc_5_75E6
        add    byte ptr [eax], al
        inc    esi
        db  072h, 065h
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
        imul   ebp, dword ptr [esi + 76h], 64696c61h
        and    byte ptr [edi + 70h], ch
        db  074h, 069h
loc_5_75DC:
        outsd
        outsb
        and    byte ptr [0a63h], ah
        or     al, byte ptr [eax]
loc_5_75E6:
        add    byte ptr [eax], al
        imul   ebp, dword ptr [esi + 76h], 64696c61h
        and    byte ptr [edi + 70h], ch
        db  074h, 069h
        outsd
        outsb
        and    byte ptr [0a63h], ah
        insd
        imul   esi, dword ptr [ebx + 2dh], 67696c61h
loc_5_7604:
        outsb
        and    byte ptr fs:[eax + 6fh], dh
        imul   ebp, dword ptr [esi + 74h], 76007265h
        jo     SHORT loc_5_7633
        outsd
loc_5_7614:
        jne    SHORT loc_5_768A
        and    byte ptr [edi + 66h], ch
        and    byte ptr [edx + 61h], dh
        outsb
        add    byte ptr gs:[bx + di + 6eh], cl
        db  076h, 061h
        insb
        imul   esp, dword ptr [eax + 56h], 72612050h
        db  072h, 061h
        db  079h, 00Ah
        add    byte ptr [eax], al
loc_5_7633:
        add    byte ptr [eax + 70h], dh
        cmp    ax, 3825h
        cmp    byte ptr cs:[eax + 20h], bh
        outsb
        outsd
        je     SHORT loc_5_7662
        popad
        arpl   word ptr [ebx + 65h], sp
        jae    SHORT loc_5_76BB
        imul   esp, dword ptr [edx + 6ch], 66282065h
        jb     SHORT loc_5_76C0
        insd
        and    byte ptr [eax + 76h], dh
        db  070h, 03Dh
        and    eax, 78382e38h
        sub    dword ptr [edx], ecx
        add    byte ptr [eax], al
        db  070h, 061h
loc_5_7662:
        and    byte ptr fs:[bx + si], ah
        jo     SHORT loc_5_76DB
        db  065h, 073h, 020h
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        insb
        outsd
        arpl   word ptr [ebx + 65h], bp
        and    byte ptr fs:[eax + 72h], dh
        db  065h, 073h, 020h
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        db  070h, 061h
        and    byte ptr fs:[bx + si], ah
        and    dword ptr [eax + 72h], esi
loc_5_768A:
        db  065h, 073h, 020h
        add    byte ptr [eax], al
        add    byte ptr [78352e35h], ah
        and    byte ptr [73h], ah
        add    byte ptr [eax], ah
        db  070h, 061h
        jb     SHORT loc_5_76DE
        and    eax, 78382e38h
        and    byte ptr [esi + 6fh], ch
        je     SHORT loc_5_76CB
        popad
        arpl   word ptr [ebx + 65h], sp
        db  073h, 073h
        imul   esp, dword ptr [edx + 6ch], 66282065h
        jb     SHORT loc_5_7729
        insd
loc_5_76BB:
        and    byte ptr [eax + 76h], dh
        db  070h, 03Dh
loc_5_76C0:
        and    eax, 78382e38h
        sub    dword ptr [edx], ecx
        add    byte ptr [ebx + 68h], dh
        popad
loc_5_76CB:
        db  072h, 065h
        and    byte ptr fs:[edi + 69h], dh
        outsb
        xor    esi, dword ptr [edx]
        and    byte ptr [ebp + 78h], ah
        das
        insb
loc_5_76DB:
        insb
        cmp    ah, byte ptr [eax]
loc_5_76DE:
        cmp    ah, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        push   eax
        push   63697379h
        popad
        insb
        and    byte ptr [eax + 61h], dh
        db  067h, 065h, 073h, 03Ah
        and    byte ptr [eax], ah
        and    byte ptr [78352e35h], ah
        and    byte ptr [eax], ah
        inc    esi
        db  072h, 065h
        and    byte ptr gs:[eax + 68h], dh
        db  079h, 073h
        and    byte ptr [eax + 61h], dh
        jae    NEAR PTR loc_5_774B
        and    byte ptr [78352e35h], ah
        and    byte ptr [eax], ah
        dec    ecx
        insb
        and    byte ptr gs:[eax + 68h], dh
        jns    SHORT loc_5_7795
        and    byte ptr [eax + 61h], dh
        jae    NEAR PTR loc_5_7763
loc_5_7729:
        and    byte ptr [eax], ah
        and    eax, 78352e35h
        or     al, byte ptr [ebx + 6fh]
        insd
        insd
        imul   esi, dword ptr [esp + esi*2 + 65h], 61702064h
        db  067h, 065h, 073h, 03Ah
        and    byte ptr [eax], ah
        and    eax, 78352e35h
        and    byte ptr [eax], ah
        push   ebx
loc_5_774B:
        db  077h, 061h
        db  070h, 070h
        popad
        bound  ebp, qword ptr [ebp + 20h]
        db  070h, 061h
        jae    NEAR PTR loc_5_7794
        and    byte ptr [78352e35h], ah
        and    byte ptr [eax], ah
        dec    edi
loc_5_7763:
        db  074h, 068h
        db  065h, 072h, 020h
        db  074h, 079h
        jo     SHORT loc_5_77D1
        and    byte ptr [eax + 61h], dh
        jae    NEAR PTR loc_5_77AD
        and    byte ptr [78352e35h], ah
        or     cl, byte ptr [edi + 66h]
        and    byte ptr [eax + ebp*2 + 6fh], dh
        db  073h, 065h
        sub    al, 20h
        dec    esp
        outsd
        arpl   word ptr [ebx + 65h], bp
        cmp    ah, byte ptr fs:[eax]
        and    eax, 78352e35h
        and    byte ptr [eax], ah
        dec    ebp
loc_5_7794:
        popad
loc_5_7795:
        db  078h, 069h
        insd
        db  075h, 06Dh
        and    byte ptr [edi + ebp*2 + 63h], ch
        imul   esp, dword ptr [ebp + 64h], 3ah
        and    byte ptr [eax], ah
        and    eax, 78352e35h
        and    byte ptr [eax], ah
        push   ebp
        outsb
loc_5_77AD:
        jae    SHORT loc_5_7810
        and    byte ptr gs:[eax + 61h], dh
        db  067h, 065h, 073h, 03Ah
        or     dword ptr [eax], esp
        and    byte ptr [eax], ah
        and    eax, 78352e35h
        or     al, byte ptr [eax]
        add    byte ptr [ecx + ebp*2 + 73h], al
        imul   esp, dword ptr [eax], 63h
        popad
        arpl   word ptr [eax + 65h], bp
        and    byte ptr [eax + 61h], dh
loc_5_77D1:
        db  067h, 065h, 073h, 03Ah
        and    byte ptr [78352e35h], ah
        and    byte ptr [eax], ah
        dec    ebp
        imul   ebp, dword ptr [esi + 69h], 206d756dh
        arpl   word ptr [ecx + 63h], sp
        push   20203a65h
        and    byte ptr [78352e35h], ah
        and    byte ptr [eax], ah
        dec    ebp
        popad
        db  078h, 069h
        insd
        db  075h, 06Dh
        and    byte ptr [ebx + 61h], ah
        arpl   word ptr [eax + 65h], bp
        cmp    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        and    byte ptr [78352e35h], ah
        or     cl, byte ptr [edx]
        add    byte ptr [eax], al
loc_5_7810:
        push   ebx
        ja     SHORT loc_5_7874
        db  070h, 070h
        imul   ebp, dword ptr [esi + 67h], 3a4e4f20h
        and    byte ptr [eax], ah
        add    byte ptr [eax], al
        push   eax
        popad
        imul   ebp, dword ptr [bp + 67h], 72687420h
        outsd
        jne    SHORT loc_5_7894
        push   736f4420h
        add    byte ptr [eax], al
        push   eax
        popad
        imul   ebp, dword ptr [bp + 67h], 72687420h
        outsd
        db  075h, 067h
        push   61724420h
        outsd
        outsb
        add    byte ptr [eax], al
        add    byte ptr [ebx + 77h], dl
        popad
        jo     SHORT loc_5_78C1
        imul   ebp, dword ptr [esi + 67h], 46464f20h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        and    byte ptr [eax], ah
        dec    ecx
        insb
        sub    eax, 656d6974h
        and    byte ptr [eax + 61h], dh
        imul   ebp, dword ptr [bp + 67h], 20736920h
        outsb
loc_5_7874:
        popad
        bound  ebp, qword ptr [ebp + 64h]
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        push   ebx
        ja     SHORT loc_5_78E1
        db  070h, 020h
        imul   bp, word ptr [ebp + 20h], 6973h
        jp     SHORT loc_5_78F0
        and    byte ptr [eax], ch
        imul   ebp, dword ptr [esi + 20h], 65676170h
loc_5_7894:
        db  073h, 029h
        cmp    ah, byte ptr [eax]
        and    eax, 78352e35h
        and    byte ptr [ecx + 6eh], cl
        and    byte ptr [ebp + 73h], dh
        cmp    ah, byte ptr gs:[eax]
        and    eax, 78352e35h
        and    byte ptr [esi + 65h], cl
        cmp    ah, byte ptr fs:[eax]
        and    eax, 78352e35h
        and    byte ptr [edx + 72h], al
        outsd
        imul   esp, dword ptr [ebp + 6eh], 3ah
loc_5_78C1:
        and    byte ptr [78352e35h], ah
        or     cl, byte ptr [eax + 69h]
        push   20747365h
        imul   ebp, dword ptr [esi + 20h], 3a657375h
        and    byte ptr [78352e35h], ah
        and    byte ptr [edx + 75h], dl
        outsb
loc_5_78E1:
        outsb
        imul   ebp, dword ptr [esi + 67h], 746f7420h
        popad
        insb
        and    byte ptr [ecx + 6ch], ah
        insb
        outsd
loc_5_78F0:
        arpl   word ptr [ecx + 74h], sp
        cmp    ah, byte ptr fs:[eax]
        and    eax, 78382e38h
        and    byte ptr [ebx + 74h], dl
        outsd
        insb
        outsb
        cmp    ah, byte ptr [eax]
        and    eax, 78382e38h
        or     al, byte ptr [eax]
        dec    ebp
        imul   ebp, dword ptr [esi + 69h], 206d756dh
        jae    SHORT loc_5_797F
        jp     SHORT loc_5_797D
        cmp    cl, byte ptr [ecx]
        and    eax, 78352e35h
        and    byte ptr [ebp + 61h], cl
        js     SHORT loc_5_7944
        imul   ebp, dword ptr [esi + 20h], 74737973h
        insd
        imul   ebp, dword ptr cs:[esi + 69h], 3525203ah
        xor    eax, 76412078h
        popad
        imul   ebp, dword ptr [ecx + 62h], 6d20656ch
loc_5_7944:
        popad
        db  078h, 020h
        popad
        je     SHORT loc_5_796A
        bound  ebp, qword ptr [edi + 6fh]
        db  074h, 03Ah
        and    byte ptr [78352e35h], ah
        or     al, byte ptr [eax]
        add    byte ptr [ebx + 77h], dl
        popad
        jo     SHORT loc_5_797D
        imul   bp, word ptr [ebp + 20h], 616eh
        insd
        cmp    ah, byte ptr gs:[eax]
        add    byte ptr [eax], al
loc_5_796A:
        add    byte ptr [eax], al
        pop    ebx
        and    eax, 78382e38h
        pop    ebp
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [53000a73h], ah
loc_5_797D:
        ja     SHORT loc_5_79E0
loc_5_797F:
        db  070h, 020h
        imul   bp, word ptr [ebp + 20h], 6873h
        db  072h, 069h
        outsb
        imul   esp, dword ptr [eax], 70h
        outsb
        imul   ebp, dword ptr fs:[esi + 67h], 206e6920h
        and    eax, 65732064h
        arpl   word ptr [edi + 6eh], bp
        db  064h, 073h, 02Ch
        and    byte ptr [edi + ebp*2 + 20h], dh
        and    eax, 78352e35h
        and    byte ptr [eax + 61h], dh
        db  067h, 065h, 073h, 00Ah
        add    byte ptr [edx], cl
        push   eax
        popad
        and    byte ptr gs:[bp + 61h], ah
        db  075h, 06Ch
        db  074h, 073h
        cmp    ah, byte ptr [eax]
        and    eax, 78382e38h
        and    byte ptr [eax + 61h], dl
        sub    eax, 203a6e49h
        and    eax, 78382e38h
        and    byte ptr [eax + 61h], dl
        sub    eax, 3a74754fh
loc_5_79E0:
        and    byte ptr [78382e38h], ah
        and    byte ptr [ecx + ebp*2 + 73h], al
        arpl   word ptr [ecx + 72h], sp
        cmp    ah, byte ptr fs:[eax]
        and    eax, 78382e38h
        or     al, byte ptr [eax]
        add    byte ptr [ecx + 6eh], cl
        jae    SHORT loc_5_7A70
        popad
        outsb
        arpl   word ptr [ebp + 3ah], sp
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    eax, 78382e38h
        and    byte ptr [eax + 6fh], cl
        outsd
        imul   esp, dword ptr [ebp + 64h], 3ah
        and    byte ptr [eax], ah
        and    eax, 78382e38h
        and    byte ptr [esi + 75h], cl
        insb
        and    byte ptr [eax + 61h], dh
        cmp    ah, byte ptr gs:[bx + si]
        and    eax, 78382e38h
        and    byte ptr [ecx + 6eh], cl
        db  076h, 061h
        insb
        imul   esp, dword ptr [edx + edi + 20h], 382e3825h
        db  078h, 00Ah
        push   edx
        arpl   word ptr gs:[ecx + 69h], bp
        insd
        db  073h, 03Ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    eax, 78382e38h
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax + 61h], dh
        and    byte ptr gs:[bx + di + 6ch], ah
        db  072h, 065h
        popad
        jns    NEAR PTR loc_5_7A7D
        db  070h, 072h
        db  065h, 073h, 065h
        outsb
        db  074h, 03Ah
        and    byte ptr [0a78h], ah
        add    byte ptr [ecx + 6eh], ch
        db  076h, 061h
loc_5_7A70:
        insb
        imul   esp, dword ptr [eax + 61h], 65726464h
        db  073h, 073h
        cmp    ah, byte ptr [eax]
loc_5_7A7D:
        and    eax, 0a78h
        add    byte ptr [eax], al
        db  070h, 061h
        and    byte ptr gs:[bx + di + 74h], ah
        and    byte ptr [6f662078h], ah
        jb     SHORT loc_5_7AF6
        and    byte ptr fs:[eax + 72h], dh
        jae    NEAR PTR loc_5_7B00
        outsb
        je     SHORT loc_5_7AA8
        add    byte ptr [eax], al
        push   esp
        push   656d2065h
        insd
        outsd
loc_5_7AA8:
        db  072h, 079h
        and    byte ptr [ebp + 61h], ch
        outsb
        popad
        db  067h, 065h, 072h, 020h
        arpl   word ptr [ecx + 6eh], sp
        outsb
        outsd
        je     SHORT loc_5_7ADA
        arpl   word ptr [ebp + 72h], si
        jb     SHORT loc_5_7B24
        outsb
        je     SHORT loc_5_7B2E
        db  079h, 020h
        db  070h, 065h
        jb     SHORT loc_5_7B2E
        outsd
        db  072h, 06Dh
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [edx + 65h], dh
        db  071h, 075h
        db  065h, 073h, 074h
        and    byte ptr cs:[eax], ah
loc_5_7ADA:
        push   esp
        jb     SHORT loc_5_7B56
        and    byte ptr [ecx + 67h], ah
        popad
        imul   ebp, dword ptr [esi + 20h], 6574616ch
        jb     SHORT loc_5_7B18
        or     al, byte ptr [eax]
        and    eax, 61702078h
        db  067h, 065h, 073h, 020h
        insd
loc_5_7AF6:
        popad
        and    byte ptr gs:[esi + 6fh], ch
        db  074h, 020h
        jo     SHORT loc_5_7B72
loc_5_7B00:
        jae    NEAR PTR loc_5_7B68
        outsb
        db  074h, 00Ah
        add    byte ptr [eax], al
        insd
        and    byte ptr gs:[edi + ebp*2 + 65h], ah
        db  073h, 06Eh
        daa
        db  074h, 020h
        db  077h, 06Fh
        db  072h, 06Bh
loc_5_7B18:
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [edx + 65h], dh
        db  074h, 061h
loc_5_7B24:
        imul   ebp, dword ptr [eax + 62h], 646c6975h
        or     al, byte ptr [eax]
loc_5_7B2E:
        add    byte ptr [eax], al
        insd
        and    byte ptr [edi + ebp*2 + 65h], ah
        db  073h, 06Eh
        daa
        je     SHORT loc_5_7B5C
        ja     SHORT loc_5_7BAD
        db  072h, 06Bh
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [edx + 65h], dh
        je     SHORT loc_5_7BAD
        imul   ebp, dword ptr [eax + 62h], 646c6975h
        or     al, byte ptr [eax]
loc_5_7B56:
        add    byte ptr [eax], al
        insd
        jne    SHORT loc_5_7B7C
loc_5_7B5C:
        outsd
        db  065h, 073h, 06Eh
        daa
        db  074h, 020h
        ja     SHORT loc_5_7BD5
        db  072h, 06Bh
loc_5_7B68:
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [edx + 65h], dh
loc_5_7B72:
        je     SHORT loc_5_7BD5
        imul   ebp, dword ptr [eax + 62h], 646c6975h
loc_5_7B7C:
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        imul   ebp, dword ptr [esi + 76h], 64696c61h
        and    byte ptr [edi + 70h], ch
        db  074h, 069h
        outsd
        outsb
        and    byte ptr [0a63h], ah
        imul   ebp, dword ptr [esi + 76h], 64696c61h
        and    byte ptr [edi + 70h], ch
        db  074h, 069h
        outsd
        outsb
        and    byte ptr [54202e63h], ah
        db  079h, 070h
        and    byte ptr gs:[edi], ah
loc_5_7BAD:
        dec    ebp
        aas
        daa
        and    byte ptr [esi + 6fh], ah
        db  072h, 020h
        push   2e706c65h
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [esi + 6fh], cl
        and    byte ptr [ecx + 6eh], ch
        jae    SHORT loc_5_7C3B
        popad
        outsb
        arpl   word ptr [ebp + 20h], sp
        popad
        db  074h, 061h
        or     eax, 6150000ah
loc_5_7BD5:
        and    byte ptr gs:[bx + 77h], cl
        and    byte ptr [eax], ah
        push   eax
        sub    eax, 6675423eh
        and    byte ptr [eax], ah
        inc    edx
        jne    SHORT loc_5_7C4F
        sub    ax, 503eh
        or     eax, 754e000ah
        insd
        and    byte ptr [eax], ah
        dec    esi
        db  072h, 020h
        and    byte ptr [eax + 72h], dl
        outsd
        arpl   word ptr [eax], sp
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax + 72h], dl
        outsd
        arpl   word ptr [4123000ah], cx
        pop    eax
        and    byte ptr [ebx], ah
        inc    esp
        dec    esp
        and    byte ptr [eax], al
        sub    byte ptr [eax + 61h], dl
        and    byte ptr gs:[bp + 6fh], ch
        db  074h, 020h
        imul   ebp, dword ptr [esi + 69h], 6c616974h
        imul   edi, dword ptr [edx + 65h], 65792064h
        db  074h, 029h
        or     eax, 2320000ah
        inc    ebp
        inc    edx
        pop    eax
        and    byte ptr [ebx], ah
        inc    ebp
        inc    ebx
        pop    eax
loc_5_7C3B:
        or     eax, 7250000ah
        db  065h, 073h, 073h
        and    byte ptr [ebx + 45h], bl
        jae    SHORT loc_5_7CAB
        pop    ebp
        and    byte ptr [edi + ebp*2 + 20h], dh
        jae    SHORT loc_5_7CC3
loc_5_7C4F:
        outsd
        jo     SHORT loc_5_7C7E
        and    byte ptr [edi + 72h], ch
        and    byte ptr [ecx + 6eh], ah
        db  079h, 020h
        outsd
        db  074h, 068h
        db  065h, 072h, 020h
        imul   esp, dword ptr [ebp + 79h], 20h
        db  074h, 06Fh
        and    byte ptr [ebx + 6fh], ah
        outsb
        db  074h, 069h
        outsb
        jne    SHORT loc_5_7CD4
        add    byte ptr cs:[eax], ah
        or     eax, 0d20000ah
        or     al, byte ptr [eax]
        inc    edi
        insb
        outsd
loc_5_7C7E:
        bound  esp, qword ptr [ecx + 6ch]
        and    byte ptr [ecx + ebp*2 + 73h], ch
        db  074h, 020h
        outsd
        and    byte ptr [ecx + 6eh], ch
        db  073h, 074h
        popad
        outsb
        arpl   word ptr [ebp + 20h], sp
        jb     SHORT loc_5_7CFA
        imul   ebp, dword ptr [bx + 6eh], 0a0d73h
        and    byte ptr [0d20000ah], cl
        or     al, byte ptr [eax]
        and    byte ptr [2020000ah], cl
loc_5_7CAB:
        and    byte ptr [eax], ah
        and    eax, dword ptr [ebp + 41h]
        pop    eax
        and    byte ptr [42452320h], ch
        pop    eax
        and    byte ptr [eax], ah
        sub    byte ptr [ebx], ah
        inc    esp
        pop    eax
        sub    dword ptr [esi], ebp
        and    byte ptr [eax], ah
        dec    esi
loc_5_7CC3:
        outsd
        and    byte ptr gs:[ebx], ah
        inc    ebp
        push   ebx
        dec    ecx
        or     eax, 2020000ah
        and    byte ptr [eax], ah
        push   esp
        outsd
loc_5_7CD4:
        db  074h, 061h
        insb
        and    byte ptr [ebx + 69h], dh
        jp     SHORT loc_5_7D41
        and    byte ptr [edi + 66h], ch
        and    byte ptr [ecx + 6eh], ch
        db  073h, 074h
        popad
        outsb
        arpl   word ptr [ebp + 20h], sp
        db  072h, 065h
        imul   ebp, dword ptr [bx + 6eh], 203d2073h
        and    eax, dword ptr [ebp + 43h]
        pop    eax
        and    byte ptr [esi + 6fh], ah
loc_5_7CFA:
        jb     SHORT loc_5_7D1C
        insb
        imul   esi, dword ptr [ebx + 74h], 53452320h
        dec    ecx
        or     eax, 4c28000ah
        imul   esi, dword ptr [ebx + 74h], 53452320h
        dec    ecx
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [ebp + 6dh], ah
        db  070h, 074h
        db  079h, 029h
loc_5_7D1C:
        or     eax, 5600000ah
        dec    ebp
        dec    ebp
        and    byte ptr [edx + 65h], dh
        outsb
        je     SHORT loc_5_7D8F
        jb     SHORT loc_5_7D91
        and    byte ptr fs:[ebx], ah
        inc    ebp
        inc    ecx
        pop    eax
        and    byte ptr [ecx + ebp*2 + 6dh], dh
        db  065h, 073h, 00Dh
        or     al, byte ptr [eax]
        and    byte ptr [7243000ah], cl
loc_5_7D41:
        imul   esi, dword ptr [ecx + ebp*2 + 63h], 53206c61h
        arpl   word ptr gs:[ecx + ebp*2 + 6fh], si
        outsb
        and    byte ptr [ebx + 74h], dh
        popad
        db  074h, 075h
        jae    SHORT loc_5_7D91
        or     eax, 3856000ah
        and    byte ptr ss:[ebx + 65h], dh
        jb     SHORT loc_5_7DCB
        popad
        insb
        imul   edi, dword ptr [edx + 61h], 6e6f6974h
        and    byte ptr [ebp + 75h], ch
        je     SHORT loc_5_7DD5
        db  078h, 020h
        db  073h, 074h
        popad
        db  074h, 075h
        jae    SHORT loc_5_7DB3
        or     eax, 2020000ah
        inc    ebp
        outsb
        je     SHORT loc_5_7DE7
        db  072h, 020h
        arpl   word ptr [edi + 75h], bp
        outsb
        db  074h, 020h
        cmp    eax, 61652320h
loc_5_7D8F:
        db  078h, 00Dh
loc_5_7D91:
        or     al, byte ptr [eax]
        and    byte ptr [eax], ah
        dec    edi
        db  077h, 06Eh
        db  065h, 072h, 020h
        je     SHORT loc_5_7E05
        db  072h, 065h
        popad
        and    byte ptr fs:[61652320h], bh
        js     SHORT loc_5_7DC9
        sub    byte ptr [ebx], ah
        inc    esp
        pop    eax
        sub    dword ptr [2020000ah], ecx
loc_5_7DB3:
        jne    SHORT loc_5_7E23
        outsd
        db  077h, 06Eh
        or     eax, 0ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_7DC9:
        add    byte ptr [eax], al
loc_5_7DCB:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_7DD5:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_7DE7:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_7E05:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_7E23:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx + 46h], cl
        push   ebx
        dec    ebp
        inc    edi
        push   edx
        and    byte ptr [eax], ah
        push   esi
        inc    esi
        inc    ecx
        push   esp
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        push   esi
        push   edi
        dec    ecx
        dec    esi
        xor    esi, dword ptr [edx]
        and    byte ptr [eax], ah
        and    eax, dword ptr [ecx + 58h]
        add    byte ptr [eax + 78h], bh
        db  078h, 078h
        and    eax, dword ptr [ebp + 44h]
        pop    eax
        and    byte ptr [ebx], ah
        inc    ebp
        push   ebx
        dec    ecx
        and    byte ptr [ebx], ah
        inc    ebp
        inc    edx
        push   eax
        and    byte ptr [edi], bh
        inc    ebp
        inc    edx
        push   eax
        or     eax, 3f3f000ah
        aas
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    eax, dword ptr [ebp + 53h]
        dec    ecx
        and    byte ptr [ecx + 6dh], ah
        popad
        and    byte ptr fs:[bp + di + 74h], dh
        popad
        arpl   word ptr [ebx + 0dh], bp
        or     al, byte ptr [eax]
        dec    edi
        dec    ebx
        or     eax, 6e75000ah
        imul   ebp, dword ptr [esi + 6fh], 77h
        outsb
        and    byte ptr [edi + 69h], dl
        outsb
        xor    esi, dword ptr [edx]
        and    byte ptr [ecx + 70h], ah
        db  070h, 065h
        popad
        db  072h, 073h
        and    byte ptr [edi + ebp*2 + 20h], dh
        bound  esp, qword ptr [ebp + 20h]
        db  073h, 074h
        db  075h, 063h
        imul   eax, dword ptr [eax], 40h
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebp + 69h], ch
        insb
        and    byte ptr gs:[edi + 66h], ch
        and    byte ptr [ecx + 6eh], ah
        and    byte ptr [ecx + 2fh], cl
        dec    edi
        and    byte ptr [edi + 70h], ch
        db  065h, 072h, 061h
        db  074h, 069h
        outsd
        outsb
        add    byte ptr [ebp + 75h], cl
        je     SHORT loc_5_7F93
        js     SHORT loc_5_7F50
        outsd
        ja     SHORT loc_5_7FA1
        jb     NEAR PTR loc_5_7F56
        imul   esi, dword ptr [ebx + 20h], 76657270h
        outsb
        je     SHORT loc_5_7FAA
        outsb
        and    byte ptr [bx + 74h], ch
        push   74207265h
        push   64616572h
loc_5_7F50:
        db  073h, 020h
        jb     NEAR PTR loc_5_7FC4
        insd
loc_5_7F56:
        and    byte ptr [edx + 75h], dh
        outsb
        outsb
        imul   ebp, dword ptr [esi + 67h], 68742000h
        imul   esp, dword ptr [eax + 6fh], 48206a62h
        popad
        outsb
        insb
        and    byte ptr gs:[eax], ah
        and    byte ptr [ebx + 74h], dl
        popad
        arpl   word ptr [ebx + 20h], bp
        and    byte ptr [eax], ah
        and    byte ptr [ebx + 61h], al
        insb
        insb
        jb     NEAR PTR loc_5_7FA2
        and    byte ptr [eax], ah
        push   ebx
        jns    SHORT loc_5_7FF4
        bound  ebp, qword ptr [edi + 6ch]
        and    byte ptr [eax], ch
        imul   esp, dword ptr [esi + 20h], 776f6e6bh
loc_5_7F93:
        outsb
        sub    dword ptr [6854000ah], ecx
        jb     SHORT loc_5_8001
        popad
        and    byte ptr fs:[ebx], ah
        inc    ecx
loc_5_7FA1:
        pop    eax
loc_5_7FA2:
        cmp    ah, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        insb
        nop
loc_5_7FAA:
        add    byte ptr [eax], al
        db  08Ch, 03Eh, 000h, 000h, 078h, 090h, 000h, 000h, 064h, 039h, 000h, 000h, 095h, 091h, 000h, 000h
        db  03Fh, 03Ah, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
loc_5_7FC4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        xchg   esi, eax
        nop
        add    byte ptr [eax], al
        fmul   dword ptr [ebx]
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
        db  0B0h, 090h, 000h, 000h, 0E0h, 004h, 000h, 000h, 0CFh, 090h, 000h, 000h, 0E6h, 004h, 000h, 000h
        db  0FFh, 0FFh, 0FFh, 0FFh
loc_5_7FF4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  00Dh, 091h, 000h, 000h, 084h
        or     byte ptr [eax], al
        db  000h, 021h
loc_5_8001:
        xchg   ecx, eax
        add    byte ptr [eax], al
        push   ebp
        aas
        add    byte ptr [eax], al
        db  035h, 091h, 000h, 000h, 0FCh
        add    byte ptr [eax], al
        db  000h, 04Eh, 091h
        add    byte ptr [eax], al
        cmp    dword ptr [ebx], eax
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
        db  066h, 091h, 000h, 000h, 008h, 00Fh, 000h, 000h, 07Dh, 091h, 000h, 000h, 0F0h, 00Ah, 000h, 000h
        db  0EDh, 090h, 000h, 000h, 09Dh, 006h, 000h, 000h, 003h, 091h, 000h, 000h, 0C1h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 053h, 079h, 073h, 074h, 065h, 06Dh, 020h, 074h, 069h, 06Dh, 065h, 000h
        db  044h, 069h, 073h, 070h, 06Ch, 061h, 079h, 020h, 043h, 072h, 069h, 074h, 069h, 063h, 061h, 06Ch
        db  020h, 053h, 065h, 063h, 074h, 069h, 06Fh, 06Eh, 020h, 069h, 06Eh, 066h, 06Fh, 000h, 049h, 02Fh
        db  04Fh, 020h, 070h, 06Fh, 072h, 074h, 020h, 074h, 072h, 061h, 070h, 020h, 069h, 06Eh, 066h, 06Fh
        db  072h, 06Dh, 061h, 074h, 069h, 06Fh, 06Eh, 000h, 056h, 038h, 036h, 020h, 069h, 06Eh, 074h, 065h
        db  072h, 072h, 075h, 070h, 074h, 020h, 068h, 06Fh, 06Fh, 06Bh, 020h, 069h, 06Eh, 066h, 06Fh, 072h
        db  06Dh, 061h, 074h, 069h, 06Fh, 06Eh, 000h, 050h, 04Dh, 020h, 069h, 06Eh, 074h, 065h, 072h, 072h
        db  075h, 070h, 074h, 020h, 068h, 06Fh, 06Fh, 06Bh, 020h, 069h, 06Eh, 066h, 06Fh, 072h, 06Dh, 061h
        db  074h, 069h, 06Fh, 06Eh, 000h, 050h, 04Dh, 020h, 043h, 04Ch, 049h, 02Fh, 053h, 054h, 049h, 020h
        db  074h, 072h, 061h, 063h, 065h, 020h, 069h, 06Eh, 066h, 06Fh, 000h, 044h, 050h, 04Dh, 049h, 020h
        db  069h, 06Eh, 066h, 06Fh, 000h, 044h, 069h, 073h, 070h, 06Ch, 061h, 079h, 020h, 065h, 076h, 065h
        db  06Eh, 074h, 020h, 06Ch, 069h, 073h, 074h, 073h, 000h, 044h, 069h, 073h, 070h, 06Ch, 061h, 079h
        db  020h, 064h, 065h, 076h, 069h, 063h, 065h, 020h, 06Ch, 069h, 073h, 074h, 000h, 044h, 069h, 073h
        db  070h, 06Ch, 061h, 079h, 020h, 056h, 038h, 036h, 020h, 062h, 072h, 065h, 061h, 06Bh, 020h, 070h
        db  06Fh, 069h, 06Eh, 074h, 073h, 000h, 044h, 069h, 073h, 070h, 06Ch, 061h, 079h, 020h, 050h, 04Dh
        db  020h, 062h, 072h, 065h, 061h, 06Bh, 020h, 070h, 06Fh, 069h, 06Eh, 074h, 073h, 000h, 046h, 061h
        db  075h, 06Ch, 074h, 020h, 048h, 06Fh, 06Fh, 06Bh, 020h, 069h, 06Eh, 066h, 06Fh, 072h, 06Dh, 061h
        db  074h, 069h, 06Fh, 06Eh, 000h, 044h, 069h, 073h, 070h, 06Ch, 061h, 079h, 020h, 074h, 069h, 06Dh
        db  065h, 020h, 06Fh, 075h, 074h, 020h, 071h, 075h, 065h, 075h, 065h, 073h, 000h, 044h, 069h, 073h
        db  070h, 06Ch, 061h, 079h, 020h, 062h, 06Ch, 06Fh, 063h, 06Bh, 065h, 064h, 020h, 074h, 068h, 072h
        db  065h, 061h, 064h, 020h, 069h, 06Eh, 066h, 06Fh, 072h, 06Dh, 061h, 074h, 069h, 06Fh, 06Eh, 000h
        db  020h, 00Dh, 00Ah, 000h, 020h, 00Dh, 00Ah, 000h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h
        db  056h, 020h, 04Dh, 020h, 04Dh, 020h, 020h, 020h, 044h, 020h, 045h, 020h, 042h, 020h, 055h, 020h
        db  047h, 020h, 020h, 020h, 049h, 020h, 04Eh, 020h, 046h, 020h, 04Fh, 020h, 052h, 020h, 04Dh, 020h
        db  041h, 020h, 054h, 020h, 049h, 020h, 04Fh, 020h, 04Eh, 020h, 041h, 020h, 04Ch, 020h, 020h, 020h
        db  053h, 020h, 045h, 020h, 052h, 020h, 056h, 020h, 049h, 020h, 043h, 020h, 045h, 020h, 053h, 00Dh
        db  00Ah, 000h, 05Dh, 020h, 020h, 000h, 045h, 06Eh, 074h, 065h, 072h, 020h, 073h, 065h, 06Ch, 065h
        db  063h, 074h, 069h, 06Fh, 06Eh, 020h, 06Fh, 072h, 020h, 05Bh, 045h, 053h, 043h, 05Dh, 020h, 074h
        db  06Fh, 020h, 065h, 078h, 069h, 074h, 03Ah, 020h, 000h, 020h, 00Dh, 00Ah, 000h, 020h, 00Dh, 00Ah
        db  000h, 03Ah, 00Dh, 00Ah, 000h, 020h, 00Dh, 00Ah, 000h, 020h, 00Dh, 00Ah, 000h, 068h, 068h, 03Ah
        db  06Dh, 06Dh, 03Ah, 073h, 073h, 02Eh, 06Dh, 073h, 020h, 000h, 000h, 000h, 025h, 030h, 032h, 064h
        db  03Ah, 025h, 030h, 032h, 064h, 03Ah, 025h, 030h, 032h, 064h, 02Eh, 025h, 064h, 05Ch, 06Eh, 000h
        db  043h, 061h, 06Ch, 06Ch, 065h, 072h, 027h, 073h, 020h, 073h, 074h, 061h, 063h, 06Bh, 020h, 069h
        db  073h, 020h, 06Eh, 06Fh, 074h, 020h, 066h, 06Ch, 061h, 074h, 020h, 06Dh, 06Fh, 064h, 065h, 06Ch
        db  02Eh, 020h, 020h, 049h, 06Eh, 074h, 020h, 06Eh, 06Fh, 074h, 020h, 066h, 072h, 06Fh, 06Dh, 020h
        db  056h, 04Dh, 04Dh, 02Eh, 00Dh, 00Ah, 000h, 053h, 074h, 061h, 063h, 06Bh, 020h, 06Dh, 065h, 06Dh
        db  06Fh, 072h, 079h, 020h, 061h, 074h, 020h, 023h, 045h, 053h, 049h, 020h, 069h, 073h, 020h, 06Eh
        db  06Fh, 074h, 020h, 076h, 061h, 06Ch, 069h, 064h, 020h, 028h, 069h, 074h, 020h, 06Dh, 061h, 079h
        db  020h, 062h, 065h, 020h, 073h, 077h, 061h, 070h, 070h, 065h, 064h, 020h, 06Fh, 075h, 074h, 029h
        db  00Dh, 00Ah, 000h, 023h, 044h, 058h, 03Ah, 023h, 045h, 044h, 049h, 020h, 03Dh, 020h, 028h, 049h
        db  06Eh, 076h, 061h, 06Ch, 069h, 064h, 020h, 04Dh, 065h, 06Dh, 06Fh, 072h, 079h, 029h, 00Dh, 00Ah
        db  000h, 023h, 044h, 058h, 03Ah, 023h, 045h, 044h, 049h, 020h, 03Dh, 020h, 023h, 045h, 041h, 058h
        db  02Ch, 020h, 03Fh, 045h, 041h, 058h, 00Dh, 00Ah, 000h, 020h, 023h, 062h
loc_5_8314:
        insb
        and    esp, dword ptr cs:[ebx + 6ch]
        and    byte ptr [ebx], ah
        popad
        js     SHORT loc_5_833E
        and    esp, dword ptr [ebp + 73h]
        imul   eax, dword ptr [eax], 63652320h
        js     SHORT loc_5_8349
        and    esp, dword ptr [ebp + 62h]
        db  078h, 020h
        and    esp, dword ptr [ebp + 61h]
        db  078h, 020h
        and    esp, dword ptr [ebp + 64h]
        db  069h, 00Dh
loc_5_8338:
        or     al, byte ptr [eax]
        and    byte ptr [eax], ah
loc_5_833C:
        and    byte ptr [eax], ah
loc_5_833E:
        and    byte ptr [eax], ah
loc_5_8340:
        and    byte ptr [eax], ah
loc_5_8342:
        and    byte ptr [ebx + 6fh], al
        outsb
        db  074h, 072h
loc_5_8348:
        outsd
loc_5_8349:
        insb
        and    byte ptr [61653f20h], bh
loc_5_8350:
        js     SHORT loc_5_835F
        or     al, byte ptr [eax]
loc_5_8354:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_5_8358:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_5_835C:
        and    byte ptr [esi + 38h], dl
loc_5_835F:
        pop    edi
        inc    ecx
        push   eax
        dec    ecx
        and    byte ptr [61653f20h], bh
        db  078h, 020h
        and    byte ptr [eax + 23h], al
        bound  edi, qword ptr gs:[eax + 0dh]
        or     al, byte ptr [eax]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax + 4dh], dl
        pop    edi
        inc    ecx
        push   eax
        dec    ecx
        and    byte ptr [eax], ah
        cmp    eax, 61653f20h
        db  078h, 020h
        and    byte ptr [eax + 23h], al
        bound  edi, qword ptr gs:[eax + 0dh]
        or     al, byte ptr [eax]
        and    byte ptr [614e000ah], cl
        insd
        and    byte ptr gs:[eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        push   esi
        db  065h, 072h, 020h
        and    byte ptr [eax], ah
        dec    ecx
        inc    esp
        and    byte ptr [eax], ah
        dec    ecx
        outsb
        imul   esi, dword ptr [edi + ecx*2 + 72h], 52202064h
        and    byte ptr gs:[ecx + 74h], al
        popad
        and    byte ptr [ebx + 65h], dl
        jb     SHORT loc_5_843A
        imul   esp, dword ptr [ebx + 65h], 65532073h
        db  072h, 076h
        push   esp
        popad
        bound  ebp, qword ptr [eax + 20h]
        and    byte ptr [esp + eax*2 + 42h], al
        or     eax, 0ah
loc_5_83DC:
        xor    byte ptr [ecx], dh
        xor    dh, byte ptr [ebx]
loc_5_83E0:
        xor    al, 35h
        aaa
loc_5_83E4:
        cmp    byte ptr [ecx], bh
        inc    ecx
        inc    edx
loc_5_83E8:
        inc    ebx
        inc    esp
        inc    ebp
        inc    esi
loc_5_83EC:
        push   ebx
        db  074h, 06Fh
        jo     SHORT loc_5_8461
        and    byte ptr fs:[edi + 68h], dh
        db  069h, 06Ch
loc_5_83F8:
        and    byte ptr gs:[esi + 4dh], dl
loc_5_83FC:
        and    byte ptr [ebp + 78h], ah
        arpl   word ptr gs:[ebp + 74h], si
        imul   ebp, dword ptr [esi + 67h], 57000a0dh
        imul   ebp, dword ptr [esi + 64h], 2f73776fh
        xor    edi, dword ptr [eax]
        and    byte ptr ss:[ebx + 65h], ch
        db  072h, 06Eh
        insb
        and    byte ptr [edx + 65h], dh
        outsb
        db  074h, 065h
        db  072h, 065h
        and    byte ptr fs:[ebx], ah
        inc    ecx
        pop    eax
        and    byte ptr [ecx + ebp*2 + 6dh], dh
        jae    NEAR PTR loc_5_843D
        or     al, byte ptr [eax]
        inc    ebx
        jb     SHORT loc_5_849E
        je     SHORT loc_5_84A0
        arpl   word ptr [ecx + 6ch], sp
loc_5_843A:
        and    byte ptr [ebx + 65h], dh
loc_5_843D:
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        outsb
        and    byte ptr [ebx + 6ch], ah
        popad
        imul   ebp, dword ptr [ebp + 20h], 6e756f63h
        db  074h, 020h
        cmp    eax, 43452320h
        pop    eax
        or     eax, 6854000ah
        db  072h, 065h
        popad
        and    byte ptr fs:[edi + 77h], ch
loc_5_8461:
        outsb
        imul   ebp, dword ptr [esi + 67h], 69726320h
        db  074h, 069h
        arpl   word ptr [ecx + 6ch], sp
        and    byte ptr [ebx + 65h], dh
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        outsb
        and    byte ptr [41452320h], bh
        pop    eax
        or     eax, 6854000ah
        and    byte ptr gs:[ebx + 72h], ah
        imul   esi, dword ptr [ecx + ebp*2 + 63h], 73206c61h
        arpl   word ptr gs:[ecx + ebp*2 + 6fh], si
        outsb
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [ebx + 75h], ah
        jb     SHORT loc_5_850E
        outsb
loc_5_849E:
        db  074h, 06Ch
loc_5_84A0:
        db  079h, 020h
        jne    SHORT loc_5_8512
        outsd
        ja     SHORT loc_5_8515
        or     eax, 0d20000ah
        or     al, byte ptr [eax]
loc_5_84B0:
        push   esi
        dec    ebp
        and    byte ptr [eax + 61h], ch
        outsb
        insb
        and    byte ptr gs:[eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        cmp    eax, 42452320h
        pop    eax
        or     eax, 6c43000ah
        imul   esp, dword ptr [ebp + 6eh], 6f702074h
        imul   ebp, dword ptr [esi + 74h], 3d207265h
        and    byte ptr [ebx], ah
        inc    ebp
        push   ebx
        dec    ecx
        or     eax, 4d56000ah
        and    byte ptr [ebx + 74h], dl
        popad
        je     SHORT loc_5_855D
        db  073h, 020h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_5_84EE:
        and    byte ptr [0d200020h], bh
        or     al, byte ptr [eax]
        and    byte ptr [0d20000ah], cl
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        inc    ebp
        js     SHORT loc_5_8566
        insb
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        add    byte ptr [edx + 61h], al
        arpl   word ptr [ebx + 20h], bp
loc_5_850E:
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
loc_5_8512:
        inc    ebx
        jb     SHORT loc_5_857A
loc_5_8515:
        popad
        je     SHORT loc_5_8538
        and    byte ptr [eax], ah
        add    byte ptr [ebx + 75h], dl
        jae    SHORT loc_5_858F
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_5_8523:
        add    byte ptr [esi + 6fh], cl
        db  074h, 05Fh
        inc    ebp
        js     SHORT loc_5_854B
        and    byte ptr [eax], al
        push   eax
        dec    ebp
        pop    edi
        inc    ebp
        db  078h, 065h
loc_5_8533:
        and    byte ptr [eax], ah
        add    byte ptr [eax + 4dh], dl
loc_5_8538:
        pop    edi
        inc    ecx
        jo     SHORT loc_5_85AC
        and    byte ptr [eax], ah
        add    byte ptr [eax + 4dh], dl
        pop    edi
        push   ebp
        xor    esi, dword ptr [edx]
        and    byte ptr [eax], ah
        add    byte ptr [esi + 78h], dl
        inc    esp
loc_5_854B:
        pop    edi
        inc    ebp
        js     SHORT loc_5_856F
        and    byte ptr [eax], al
        dec    eax
        push   eax
        jb     SHORT loc_5_85BE
        inc    edx
loc_5_8556:
        imul   esp, dword ptr [eax], 20h
        add    byte ptr [edx + 6ch], al
        outsd
loc_5_855D:
        arpl   word ptr [ebx + 20h], bp
        and    byte ptr [eax], ah
loc_5_8562:
        add    byte ptr [ecx + 77h], al
        popad
loc_5_8566:
        imul   esp, dword ptr [ebp + 20h], 20h
        and    byte ptr [eax], al
        push   eax
        push   esi
loc_5_856F:
        cmp    byte ptr [esi], dh
        and    byte ptr [eax], ah
loc_5_8573:
        and    byte ptr [eax], al
        push   esi
        cmp    byte ptr [esi], dh
        dec    ecx
        dec    esp
loc_5_857A:
        imul   esp, dword ptr [eax], 20h
        add    byte ptr [ecx + 64h], cl
        insb
        push   esp
        dec    edi
        db  075h, 074h
        add    byte ptr [ecx + 64h], cl
loc_5_8589:
        insb
        and    byte ptr gs:[eax], ah
        and    byte ptr [eax], ah
loc_5_858F:
        add    byte ptr [ebx + 6ch], al
        outsd
        db  073h, 069h
        outsb
        and    byte ptr [bx + si], al
        and    eax, dword ptr [ebp + 41h]
loc_5_859C:
        pop    eax
        and    byte ptr [2020000ah], cl
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_5_85A9:
        add    byte ptr [ecx + 6ch], al
loc_5_85AC:
        je     SHORT loc_5_8613
        db  072h, 06Eh
loc_5_85B0:
        popad
        db  074h, 065h
        and    byte ptr [edx + 65h], dl
        imul   esi, dword ptr [bp + di + 74h], 53207265h
loc_5_85BE:
        db  065h, 074h, 00Dh
        or     al, byte ptr [eax]
        and    byte ptr [6552000ah], cl
        imul   esi, dword ptr [bp + di + 74h], 20737265h
        outsw
        db  072h, 020h
        and    esp, dword ptr [ebp + 62h]
loc_5_85D8:
        db  078h, 00Dh
        or     al, byte ptr [eax]
loc_5_85DC:
        push   esp
        push   64616572h
        and    byte ptr [ebx + 74h], dl
        popad
        arpl   word ptr [ebx + 20h], bp
        and    esp, dword ptr [ebp + 62h]
        js     SHORT loc_5_85FB
        or     al, byte ptr [eax]
loc_5_85F0:
        and    byte ptr [6150000ah], cl
        and    byte ptr gs:[bp + 6fh], ch
loc_5_85FB:
        db  074h, 020h
        db  070h, 072h
        jae    NEAR PTR loc_5_8667
        outsb
        je     SHORT loc_5_8612
        or     al, byte ptr [eax]
        push   ebx
        insb
        arpl   word ptr gs:[edi + ebp*2 + 72h], si
        and    byte ptr [ebx], ah
        inc    ecx
loc_5_8612:
        pop    eax
loc_5_8613:
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [ecx + 6eh], ch
        db  076h, 061h
        insb
        imul   esp, dword ptr [ebp + ecx + 0ah], 6d754400h
        jo     SHORT loc_5_8646
        outsd
        and    byte ptr [esi + 4dh], dl
        and    byte ptr [ebx + 6fh], ah
        outsb
        db  074h, 072h
        outsd
        insb
        and    byte ptr [edx + 6ch], ah
        outsd
        arpl   word ptr [ebx + 20h], bp
        outsw
        db  072h, 020h
        push   esi
        dec    ebp
        and    byte ptr [eax + 61h], ch
        outsb
        insb
loc_5_8646:
        and    byte ptr gs:[ebx], ah
        inc    ebp
        inc    edx
        pop    eax
        or     eax, 4523000ah
        inc    esp
        pop    eax
        and    byte ptr [eax], ah
        inc    esi
        insb
        popad
        db  067h, 073h, 020h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_5_8667:
        and    byte ptr [eax], ah
        and    byte ptr [ebx], ah
        inc    ebp
        inc    ecx
        pop    eax
        or     eax, 4523000ah
        inc    esp
        pop    eax
        and    byte ptr [eax], ah
        dec    eax
        imul   esp, dword ptr [edi + 68h], 6e694c20h
        popad
        db  072h, 020h
        inc    ecx
        db  064h, 064h, 072h, 020h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    eax, dword ptr [ebp + 41h]
        pop    eax
        or     eax, 4523000ah
        inc    esp
        pop    eax
        and    byte ptr [eax], ah
        inc    ebx
        insb
        imul   esp, dword ptr [ebp + 6eh], 6f505f74h
        imul   ebp, dword ptr [esi + 74h], 20207265h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx], ah
        inc    ebp
        inc    ecx
        pop    eax
        or     eax, 4523000ah
        inc    esp
        pop    eax
        and    byte ptr [eax], ah
        push   esi
        dec    ebp
        dec    ecx
        inc    esp
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx], ah
        inc    ebp
        inc    ecx
        pop    eax
        or     eax, 4523000ah
        inc    esp
        pop    eax
        and    byte ptr [eax], ah
        jo     SHORT loc_5_874F
        je     SHORT loc_5_8746
        push   esi
        dec    ebp
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx], ah
        inc    ebp
loc_5_86F4:
        inc    ecx
        pop    eax
        or     eax, 4523000ah
        inc    esp
        pop    eax
        and    byte ptr [eax], ah
        push   70616568h
        push   esi
        dec    ebp
        inc    ebx
        inc    edx
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    eax, dword ptr [ebp + 41h]
        pop    eax
        or     eax, 4523000ah
        inc    esp
        pop    eax
        and    byte ptr [eax], ah
        push   esi
        dec    ebp
        pop    edi
        dec    esp
        imul   esi, dword ptr [ebx + 74h], 6e694c5fh
loc_5_872C:
        imul   esp, dword ptr [eax], 20h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx], ah
        inc    ebp
loc_5_8738:
        inc    ecx
        pop    eax
        or     eax, 4523000ah
        inc    esp
        pop    eax
        and    byte ptr [eax], ah
        push   ebx
        db  075h, 073h
loc_5_8746:
        jo     SHORT loc_5_87AD
loc_5_8748:
        outsb
        pop    edi
        inc    ebx
        outsd
        db  075h, 06Eh
loc_5_874F:
        db  074h, 020h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx], ah
        inc    ebp
        inc    ecx
        pop    eax
        or     eax, 4523000ah
        inc    esp
        pop    eax
        and    byte ptr [eax], ah
        jo     SHORT loc_5_87CB
        push   esi
        dec    ebp
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    eax, dword ptr [ebp + 41h]
        pop    eax
        or     eax, 4523000ah
        inc    esp
loc_5_8784:
        pop    eax
        and    byte ptr [eax], ah
        dec    ecx
loc_5_8788:
        outsb
        db  073h, 074h
        pop    edi
        inc    edx
        db  075h, 066h
        pop    edi
        jo     SHORT loc_5_8806
        jb     SHORT loc_5_87B4
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    eax, dword ptr [ebp + 41h]
        pop    eax
        or     eax, 4523000ah
        inc    esp
        pop    eax
        and    byte ptr [eax], ah
        dec    ebp
        dec    ebp
        inc    edi
        push   edx
loc_5_87AD:
        pop    edi
        inc    esi
        insb
        popad
        db  067h, 073h, 020h
loc_5_87B4:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    eax, dword ptr [ebp + 41h]
        pop    eax
        or     eax, 4523000ah
        inc    esp
        pop    eax
        and    byte ptr [eax], ah
loc_5_87CB:
        dec    esp
        inc    esp
        push   esp
        push   ebx
        insb
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx], ah
        inc    ecx
        pop    eax
        or     eax, 4523000ah
loc_5_87E8:
        inc    esp
        pop    eax
        and    byte ptr [eax], ah
        dec    esp
        inc    esp
        push   esp
        arpl   word ptr [eax + 67h], si
        and    byte ptr [ecx], cl
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    eax, dword ptr [ecx + 58h]
        or     eax, 4523000ah
        inc    esp
        pop    eax
        and    byte ptr [eax], ah
loc_5_8806:
        dec    esp
        inc    esp
        push   esp
        inc    ecx
        jb     NEAR PTR loc_5_8817
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    eax, dword ptr [ebp + 41h]
loc_5_8817:
        pop    eax
        or     eax, 4523000ah
        inc    esp
        pop    eax
        and    byte ptr [eax], ah
        dec    ecx
        outsb
        je     SHORT loc_5_8884
        inc    ebp
        outsb
        popad
loc_5_8828:
        bound  ebp, qword ptr [ebp + 5fh]
        inc    ebx
        outsd
        jne    SHORT loc_5_889E
        db  074h, 020h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    eax, dword ptr [ebp + 41h]
        pop    eax
        or     eax, 4523000ah
        inc    esp
        pop    eax
        and    byte ptr [eax], ah
        push   esi
        dec    ebp
        pop    edi
        inc    ebp
        db  078h, 065h
        arpl   word ptr [edi + 54h], bx
        imul   ebp, dword ptr [ebp + 65h], 20202020h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx], ah
        inc    ebp
        inc    ecx
        pop    eax
        or     eax, 4523000ah
        inc    esp
        pop    eax
        and    byte ptr [eax], ah
        push   esp
        push   64616572h
        dec    esp
        imul   esi, dword ptr [ebx + 74h], 64616548h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx], ah
        inc    ebp
        inc    ecx
        pop    eax
        or     eax, 4523000ah
        inc    esp
loc_5_8884:
        pop    eax
        and    byte ptr [eax], ah
        push   esi
        dec    ebp
        dec    ebp
        pop    edi
        inc    esi
        insb
        popad
        db  067h, 073h, 020h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx], ah
        inc    ebp
loc_5_889E:
        inc    ecx
        pop    eax
        or     eax, 4523000ah
        inc    esp
        pop    eax
        and    byte ptr [eax], ah
        push   esp
        push   ebx
        pop    edi
        push   ebx
        arpl   word ptr [eax + 65h], bp
        pop    edi
        inc    ebx
        outsd
        jne    SHORT loc_5_8924
        je     SHORT loc_5_88D8
loc_5_88B8:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    eax, dword ptr [ebp + 41h]
        pop    eax
        or     eax, 4523000ah
        inc    esp
        pop    eax
        and    byte ptr [eax], ah
        db  070h, 074h
        push   68546263h
        jb     SHORT loc_5_8939
        popad
        and    byte ptr fs:[eax], ah
loc_5_88D8:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    eax, dword ptr [ebp + 41h]
        pop    eax
        or     eax, 4523000ah
        inc    esp
        pop    eax
        and    byte ptr [eax], ah
        push   esp
        push   64616572h
        dec    esp
        imul   esi, dword ptr [ebx + 74h], 64616548h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx], ah
        inc    ebp
        inc    ecx
        pop    eax
        or     eax, 2a000ah
        and    byte ptr [eax], al
        and    eax, dword ptr [ebp + 42h]
        pop    eax
        sub    al, 20h
        imul   esp, dword ptr [eax + 23h], 202c7863h
        db  073h, 074h
        popad
        db  074h, 075h
        db  073h, 020h
        add    byte ptr [eax], ah
loc_5_8924:
        or     eax, 2e00000ah
        push   edx
        and    byte ptr [ebx + 23h], bl
        pop    ebp
        and    byte ptr [2d2d2d2dh], ch
loc_5_8934:
        sub    eax, 6944202dh
loc_5_8939:
        db  073h, 070h
        insb
loc_5_893C:
        popad
        db  079h, 073h
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [edx + 65h], dh
        imul   esi, dword ptr [bp + di + 74h], 20737265h
        outsd
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 75h], ah
        jb     SHORT loc_5_89CB
        outsb
        db  074h, 020h
        je     SHORT loc_5_89C7
        db  072h, 065h
        popad
        or     eax, 4d562e0ah
        and    byte ptr [ebx + 23h], bl
        pop    ebp
        and    byte ptr [2d2d2d2dh], ch
        sub    eax, 73694420h
        jo     SHORT loc_5_89E5
        popad
        db  079h, 073h
        and    byte ptr [ebx + 6fh], ah
        insd
        jo     SHORT loc_5_89EE
        db  065h, 074h, 065h
        and    byte ptr [esi + 4dh], dl
        and    byte ptr [ebx + 74h], dh
        popad
        je     SHORT loc_5_8A03
        db  073h, 00Dh
        or     ch, byte ptr [esi]
        push   esi
        inc    ebx
        and    byte ptr [ebx + 23h], bl
        pop    ebp
        and    byte ptr [2d2d2d2dh], ch
        sub    eax, 73694420h
        db  070h, 06Ch
        popad
        jns    SHORT loc_5_8A1B
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 75h], ah
        db  072h, 072h
        outsb
        je     SHORT loc_5_89D5
        push   esi
        dec    ebp
        db  073h, 020h
        arpl   word ptr [edi + 6eh], bp
        db  074h, 072h
        outsd
        insb
        and    byte ptr [edx + 6ch], ah
        outsd
        arpl   word ptr [ebx + 0dh], bp
loc_5_89C7:
        or     ch, byte ptr [esi]
        push   esi
        dec    eax
loc_5_89CB:
        and    byte ptr [ebx + 23h], bl
        pop    ebp
        and    byte ptr [2d2d2d2dh], ch
loc_5_89D5:
        sub    eax, 73694420h
        db  070h, 06Ch
        popad
        db  079h, 073h
        and    byte ptr [ecx + 20h], ah
        push   esi
        dec    ebp
        dec    ebp
loc_5_89E5:
        and    byte ptr [ecx + ebp*2 + 6eh], ch
        imul   esp, dword ptr [ebp + 64h], 20h
        insb
loc_5_89EE:
        imul   esi, dword ptr [ebx + 74h], 6967202ch
        db  076h, 065h
        outsb
        and    byte ptr [ecx + ebp*2 + 73h], ch
        db  074h, 020h
        push   6c646e61h
loc_5_8A03:
        or     eax, 52562e0ah
        and    byte ptr [ebx + 23h], bl
        pop    ebp
        and    byte ptr [2d2d2d2dh], ch
        sub    eax, 73694420h
        db  070h, 06Ch
        popad
loc_5_8A1B:
        jns    SHORT loc_5_8A90
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [edx + 65h], dh
        imul   esi, dword ptr [bp + di + 74h], 20737265h
        outsd
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 75h], ah
        db  072h, 072h
        outsb
        db  074h, 020h
        push   esi
        dec    ebp
        or     eax, 53562e0ah
        and    byte ptr [ebx + 23h], bl
        pop    ebp
        and    byte ptr [2d2d2d2dh], ch
loc_5_8A4C:
        sub    eax, 73694420h
        db  070h, 06Ch
        popad
loc_5_8A54:
        db  079h, 073h
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 75h], ah
        db  072h, 072h
        outsb
        db  074h, 020h
        push   esi
        dec    ebp
        daa
        db  073h, 020h
        db  076h, 069h
        jb     SHORT loc_5_8AE0
        db  075h, 061h
        insb
        and    byte ptr [ebp + 6fh], ch
        and    byte ptr gs:[ebx + 74h], dh
        popad
        arpl   word ptr [ebx + 0dh], bp
        or     ch, byte ptr [esi]
        push   esi
        dec    esp
        and    byte ptr [2d2d2d2dh], ch
        sub    eax, 2d2d2d2dh
        and    byte ptr [ecx + ebp*2 + 73h], al
        db  070h, 06Ch
loc_5_8A90:
        popad
        jns    SHORT loc_5_8B06
        and    byte ptr [ecx + 20h], ah
        insb
        imul   esi, dword ptr [ebx + 74h], 20666f20h
        popad
        insb
        insb
        and    byte ptr [esi + 61h], dh
        insb
        imul   esp, dword ptr [eax + 56h], 6168204dh
        outsb
        insb
        db  065h, 073h, 00Dh
        or     ch, byte ptr [esi]
        inc    esp
        push   ebx
        and    byte ptr [2d2d2d2dh], ch
        sub    eax, 2d2d2d2dh
        and    byte ptr [ebp + esi*2 + 6dh], al
        db  070h, 073h
        and    byte ptr [eax + 72h], dh
        outsd
        db  074h, 065h
        arpl   word ptr [ebp + 64h], si
        and    byte ptr [ebp + 6fh], ch
        and    byte ptr gs:[ebx + 74h], dh
        popad
        arpl   word ptr [ebx + 20h], bp
        ja     SHORT loc_5_8B49
loc_5_8AE0:
        db  074h, 068h
        and    byte ptr [ecx + 62h], ch
        insb
        db  073h, 00Dh
        or     ch, byte ptr [esi]
        push   esi
        dec    ebp
        dec    ebp
        and    byte ptr [2d2d2d2dh], ch
        sub    eax, 202d2d2dh
        dec    ebp
        outsb
        jne    SHORT loc_5_8B1F
        push   esi
        dec    ebp
        dec    ebp
        and    byte ptr [ebx + 74h], dh
        popad
loc_5_8B06:
        db  074h, 065h
        and    byte ptr [ecx + 6eh], ch
        outsw
        db  072h, 06Dh
        popad
        db  074h, 069h
        outsd
        outsb
        or     eax, 643c2e0ah
        db  065h, 076h, 05Fh
        outsb
        popad
        insd
loc_5_8B1F:
        and    byte ptr ds:[6944202dh], ch
        db  073h, 070h
        insb
        popad
        db  079h, 020h
        db  064h, 065h, 076h, 069h
        arpl   word ptr [ebp + 20h], sp
        db  073h, 070h
        arpl   word ptr gs:[ecx + 66h], bp
        imul   esp, dword ptr [ebx + 20h], 6f666e69h
        or     eax, 6e49000ah
        jbe    SHORT loc_5_8BA9
        insb
loc_5_8B49:
        imul   esp, dword ptr [eax + 56h], 6168204dh
        outsb
        insb
        and    byte ptr gs:[ebx], ah
        popad
        js     SHORT loc_5_8B87
        and    byte ptr [ebp + 73h], dh
        imul   ebp, dword ptr [esi + 67h], 72756320h
        db  072h, 065h
        outsb
        db  074h, 020h
        push   esi
        dec    ebp
        and    byte ptr [ebx], ah
        bound  edi, qword ptr gs:[eax + 0dh]
        or     al, byte ptr [eax]
        dec    ecx
        outsb
        db  076h, 061h
        insb
        imul   esp, dword ptr [eax + 54h], 61657268h
        and    byte ptr fs:[ecx + 64h], cl
        sub    al, 20h
loc_5_8B87:
        jne    SHORT loc_5_8BFC
        imul   ebp, dword ptr [esi + 67h], 72756320h
        db  072h, 065h
        outsb
        db  074h, 020h
        push   esp
        push   64616572h
        or     eax, 203a000ah
        and    byte ptr [ebp + 6eh], dh
        jb     SHORT loc_5_8C0A
        arpl   word ptr [edi + 67h], bp
        outsb
loc_5_8BA9:
        imul   edi, dword ptr [edx + 65h], 6f632064h
        insd
        insd
        popad
        outsb
        and    byte ptr fs:[eax], ah
        sub    byte ptr [edi], ah
        aas
        daa
        and    byte ptr [esi + 6fh], ah
        jb     SHORT loc_5_8BE0
        push   29706c65h
        or     eax, 6f4e000ah
        and    byte ptr [edi + ebp*2 + 67h], ch
        imul   ebp, dword ptr [bp + 67h], 72657020h
        outsw
        db  072h, 06Dh
        and    byte ptr fs:[eax + ebp*2 + 75h], dh
loc_5_8BE0:
        jae    SHORT loc_5_8C02
        popaw
        jb     SHORT loc_5_8BF3
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        pop    edi
        jo     SHORT loc_5_8C5C
        db  074h, 065h
        insd
        outsd
loc_5_8BF3:
        je     NEAR PTR loc_5_8C57
        bound  ebp, qword ptr [ebp]
        add    byte ptr [eax], al
loc_5_8BFC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_8C02:
        inc    edx
        popad
        and    byte ptr fs:[ebx + 65h], dh
        db  072h, 076h
loc_5_8C0A:
        imul   esp, dword ptr [ebx + 65h], 71657220h
        jne    SHORT loc_5_8C78
        db  073h, 074h
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [ebp + 62h], al
        pop    edi
        push   ecx
        db  075h, 065h
        db  072h, 079h
        and    byte ptr [ebx + 65h], bl
        popad
        db  078h, 020h
        cmp    eax, 61652320h
        js     SHORT loc_5_8C8C
        or     eax, 0ah
        dec    ecx
        outsb
        db  076h, 061h
        insb
        imul   esp, dword ptr [eax + 56h], 2042434dh
        push   6c646e61h
        and    byte ptr gs:[69207838h], ah
        outsb
        and    byte ptr [esi + 4dh], dl
        and    byte ptr [ecx + ebp*2 + 73h], ch
        db  074h, 00Ah
loc_5_8C57:
        add    byte ptr [ecx + 6eh], cl
        db  076h, 061h
loc_5_8C5C:
        insb
        imul   esp, dword ptr [eax + 54h], 20424348h
        push   6c646e61h
        and    byte ptr gs:[69207838h], ah
        outsb
        and    byte ptr [eax + ebp*2 + 72h], dh
        popad
loc_5_8C78:
        insb
        imul   esi, dword ptr [ebx + 74h], 726f6620h
        and    byte ptr [esi + 6dh], dh
        and    byte ptr [0a7838h], ah
        add    byte ptr [eax], al
loc_5_8C8C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        test   dword ptr [eax], eax
        push   esp
        push   32332065h
        sub    eax, 20746962h
        db  064h, 065h, 076h, 069h
        arpl   word ptr [ebp + 20h], sp
        jb     NEAR PTR loc_5_908E
        jbe    SHORT loc_5_908C
        db  072h, 020h
        and    eax, 68207331h
        popad
        db  073h, 020h
        popad
        je     SHORT loc_5_90A8
        insd
        jo     SHORT loc_5_90AC
        and    byte ptr fs:[edi + ebp*2 + 20h], dh
        db  075h, 073h
        and    byte ptr gs:[ecx + 20h], ah
        db  073h, 079h
        jae    SHORT loc_5_90BC
        insd
        and    byte ptr [ebx + 65h], dh
        db  072h, 076h
        imul   esp, dword ptr [ebx + 65h], 69687720h
        arpl   word ptr [eax + 20h], bp
        imul   esi, dword ptr [ebx + 20h], 20746f6eh
        insb
        outsd
        popad
        and    byte ptr cs:[eax], ah
        dec    ecx
        je     SHORT loc_5_908C
        insd
        popad
        db  079h, 020h
        bound  esp, qword ptr [ebp + 20h]
        jo     SHORT loc_5_90E4
        jae    SHORT loc_5_90EA
        imul   esp, dword ptr [edx + 6ch], 6f742065h
        and    byte ptr [ebx + 6fh], ah
        outsb
        db  074h, 069h
        outsb
        jne    SHORT loc_5_90EC
        and    byte ptr [esi + 6fh], ch
        db  072h, 06Dh
loc_5_908C:
        popad
        insb
loc_5_908E:
        insb
        db  079h, 02Eh
        and    eax, 7332h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_90A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_90AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_90BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_90E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_90EA:
        add    byte ptr [eax], al
loc_5_90EC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0A1h, 050h, 083h, 000h, 000h
        test   eax, eax
        je     SHORT loc_5_A04B
        push   0
        push   dword ptr [eax + 10h]
        push   dword ptr [eax + 0ch]
        mov    eax, esp
        mov    edx, 8
loc_5_A018:
        dec    edx
        je     SHORT loc_5_A021
        cmp    byte ptr [edx + eax], 20h
        je     SHORT loc_5_A018
loc_5_A021:
        mov    byte ptr [edx + eax + 1], 0
        push   eax
        push   0c0002h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        push   ecx
        push   eax
        push   edi
        db  0CDh, 020h, 07Bh, 001h, 001h, 000h
        add    esp, 0ch
        add    esp, dword ptr [esp]
        add    esp, 0ch
        mov    edi, edx
loc_5_A04B:
        ret
        db  0C6h, 005h, 04Bh, 008h, 000h, 000h, 03Dh, 0C3h, 0B9h, 024h, 000h, 000h, 000h, 0CDh, 020h, 087h
        db  000h, 001h, 000h, 085h, 0C0h, 075h, 006h, 0CDh, 020h, 0BFh, 000h, 001h, 000h, 0A3h, 084h, 000h
        db  000h, 000h, 0BEh, 000h, 000h, 000h, 000h, 0CDh, 020h, 00Ch, 000h, 001h, 000h, 08Bh, 0C8h, 00Fh
        db  0B7h, 0D1h, 0C1h, 0E9h, 010h, 0B8h, 031h, 000h, 000h, 000h, 0CDh, 020h, 045h, 000h, 001h, 000h
        db  0B8h, 004h, 000h, 000h, 000h, 0B9h, 012h, 000h, 000h, 000h, 0CDh, 020h, 09Bh, 000h, 001h, 000h
        db  089h, 035h, 080h, 000h, 000h, 000h, 0B9h, 010h, 000h, 000h, 000h, 033h, 0C0h, 083h, 0CAh, 0FFh
        db  0CDh, 020h, 0CEh, 000h, 001h, 000h, 040h, 0E2h, 0F7h, 033h, 0D2h, 0BEh, 0C0h, 00Ah, 000h, 000h
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h, 0A3h, 088h, 000h, 000h, 000h, 0BEh, 0F4h, 00Ah, 000h, 000h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h, 0A3h, 08Ch, 000h, 000h, 000h, 0BEh, 09Ch, 00Ah, 000h, 000h
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h, 0A3h, 090h, 000h, 000h, 000h, 0CDh, 020h, 00Ch, 000h, 001h
        db  000h, 0A3h, 094h, 000h, 000h, 000h, 00Fh, 021h, 0F0h, 0A9h, 000h, 020h, 000h, 000h, 075h, 005h
        db  0E8h, 044h, 006h, 0FBh, 0FFh, 033h, 0C0h, 0B0h, 00Ah, 033h, 0D2h, 0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 00Ah, 000h, 017h, 000h, 0C3h, 000h, 00Bh, 0C0h, 075h, 03Eh, 08Bh, 01Dh, 0E0h, 071h
        db  000h, 000h, 08Bh, 051h, 00Ch, 089h, 053h, 001h, 08Bh, 051h, 010h, 089h, 053h, 005h, 0C6h, 003h
        db  000h, 0EBh, 027h, 01Bh, 0D2h, 00Bh, 0C0h, 074h, 007h, 0E8h, 050h, 000h, 000h, 000h, 0EBh, 017h
        db  08Bh, 01Dh, 0E0h, 071h, 000h, 000h, 0C6h, 003h, 001h, 028h, 013h, 083h, 005h, 0E0h, 071h, 000h
        db  000h, 009h, 0E8h, 037h, 000h, 000h, 000h, 0D1h, 0EAh, 0C3h, 080h, 03Dh, 028h, 001h, 000h, 000h
        db  000h, 075h, 01Ah, 083h, 0F8h, 001h, 075h, 007h, 066h, 083h, 079h, 006h, 040h, 074h, 00Fh, 08Bh
        db  014h, 085h, 0F5h, 071h, 000h, 000h, 050h, 0E8h, 031h, 000h, 000h, 000h, 058h, 0C3h, 050h, 0CDh
        db  020h, 08Ah, 001h, 001h, 000h, 0C6h, 005h, 028h, 001h, 000h, 000h, 001h, 058h, 0C3h, 080h, 03Dh
        db  028h, 001h, 000h, 000h, 000h, 075h, 015h, 052h, 050h, 0F7h, 0DAh, 0C1h, 0E2h, 004h, 08Bh, 094h
        db  082h, 001h, 072h, 000h, 000h, 0E8h, 003h, 000h, 000h, 000h, 058h, 05Ah, 0C3h, 00Fh, 0B6h, 05Ah
        db  0FFh, 003h, 0D3h, 08Bh, 079h, 00Ch, 089h, 07Ah, 0F6h, 08Bh, 079h, 010h, 089h, 07Ah, 0FAh, 02Bh
        db  0D3h, 051h, 08Bh, 0CBh, 00Bh, 0C0h, 074h, 008h, 0CDh, 020h, 089h, 001h, 001h, 000h, 0EBh, 005h
        db  0E8h, 07Ch, 000h, 000h, 000h, 059h, 0C3h, 050h, 0CDh, 020h, 088h, 001h, 001h, 000h, 066h, 00Bh
        db  0C0h, 058h, 075h, 06Ch, 0BEh, 010h, 06Fh, 000h, 000h, 00Fh, 0B6h, 006h, 03Ch, 0FFh, 074h, 05Ah
        db  050h, 08Bh, 046h, 001h, 0A3h, 02Dh, 072h, 000h, 000h, 08Bh, 046h, 005h, 0A3h, 031h, 072h, 000h
        db  000h, 0BAh, 01Eh, 072h, 000h, 000h, 0B9h, 019h, 000h, 000h, 000h, 0CDh, 020h, 089h, 001h, 001h
        db  000h, 058h, 0BAh, 0CFh, 072h, 000h, 000h, 0B9h, 020h, 000h, 000h, 000h, 0BFh, 0E5h, 072h, 000h
        db  000h, 03Ch, 001h, 075h, 00Fh, 0BAh, 06Ch, 072h, 000h, 000h, 0B9h, 020h, 000h, 000h, 000h, 0BFh
        db  082h, 072h, 000h, 000h, 08Bh, 046h, 001h, 089h, 007h, 08Bh, 046h, 005h, 089h, 047h, 004h, 0CDh
        db  020h, 089h, 001h, 001h, 000h, 083h, 0C6h, 009h, 0EBh, 09Fh, 0CDh, 020h, 08Ah, 001h, 001h, 000h
        db  0C3h, 09Ch, 056h, 057h, 050h, 051h, 08Bh, 03Dh, 031h, 073h, 000h, 000h, 083h, 0FFh, 0FFh, 074h
        db  01Ch, 0FCh, 066h, 0B8h, 020h, 007h, 051h, 057h, 0B9h, 050h, 000h, 000h, 000h, 0F3h, 066h, 0ABh
        db  05Fh, 059h, 08Bh, 0F2h, 0B4h, 007h, 049h, 049h, 0ACh, 066h, 0ABh, 0E2h, 0FBh, 059h, 058h, 05Fh
        db  05Eh, 09Dh, 0C3h, 000h, 060h, 0B8h, 090h, 001h, 000h, 000h, 0BFh, 073h, 085h, 000h, 000h, 033h
        db  0F6h, 0CDh, 020h, 0ABh, 000h, 001h, 000h, 03Dh, 0FFh, 00Fh, 000h, 000h, 076h, 005h, 0B8h, 0FFh
        db  00Fh, 000h, 000h, 0BBh, 00Ah, 000h, 000h, 000h, 0F7h, 0E3h, 005h, 0FFh, 011h, 000h, 000h, 025h
        db  000h, 0F0h, 0FFh, 0FFh, 050h, 033h, 0D2h, 02Dh, 000h, 002h, 000h, 000h, 0F7h, 0F3h, 03Dh, 0FFh
        db  00Fh, 000h, 000h, 076h, 005h, 0B8h, 0FFh, 00Fh, 000h, 000h, 083h, 0E0h, 0FCh, 096h, 0B0h, 063h
        db  0CDh, 020h, 0E8h, 000h, 001h, 000h, 073h, 019h, 06Ah, 000h, 06Ah, 001h, 0CDh, 020h, 0A8h, 000h
        db  001h, 000h, 059h, 059h, 085h, 0C0h, 075h, 006h, 0CDh, 020h, 0BFh, 000h, 001h, 000h, 0C6h, 000h
        db  063h, 0A3h, 010h, 00Ah, 000h, 000h, 096h, 0C1h, 0EEh, 004h, 03Bh, 0C6h, 076h, 001h, 096h, 0A3h
        db  068h, 09Bh, 000h, 000h, 048h, 0A3h, 07Ch, 09Bh, 000h, 000h, 058h, 0C1h, 0E8h, 00Ch, 06Ah, 008h
        db  06Ah, 000h, 06Ah, 0FFh, 06Ah, 000h, 06Ah, 000h, 06Ah, 000h, 06Ah, 001h, 050h, 0CDh, 020h, 053h
        db  000h, 001h, 000h, 083h, 0C4h, 020h, 085h, 0C0h, 075h, 006h, 0CDh, 020h, 0BFh, 000h, 001h, 000h
        db  089h, 015h, 06Ch, 09Bh, 000h, 000h, 08Bh, 0FAh, 081h, 0EAh, 06Ch, 09Bh, 000h, 000h, 001h, 015h
        db  052h, 00Bh, 000h, 000h, 001h, 015h, 059h, 00Bh, 000h, 000h, 001h, 015h, 01Fh, 00Ah, 000h, 000h
        db  001h, 015h, 026h, 00Ah, 000h, 000h, 001h, 015h, 04Eh, 00Ah, 000h, 000h, 001h, 015h, 055h, 00Ah
        db  000h, 000h, 06Bh, 005h, 068h, 09Bh, 000h, 000h, 008h, 083h, 0E8h, 004h, 003h, 005h, 06Ch, 09Bh
        db  000h, 000h, 0A3h, 070h, 09Bh, 000h, 000h, 06Bh, 005h, 068h, 09Bh, 000h, 000h, 008h, 003h, 0F8h
        db  089h, 03Dh, 080h, 09Bh, 000h, 000h, 08Bh, 00Dh, 068h, 09Bh, 000h, 000h, 081h, 0C1h, 000h, 001h
        db  000h, 000h, 0D1h, 0E9h, 0B8h, 0CDh, 030h, 0CDh, 030h, 0FCh, 0F3h, 0ABh, 0A1h, 010h, 00Ah, 000h
        db  000h, 0C1h, 0E0h, 00Ch, 066h, 0C1h, 0E8h, 00Ch, 08Bh, 01Dh, 068h, 09Bh, 000h, 000h, 0C1h, 0C3h
        db  010h, 02Bh, 0C3h, 0C1h, 0C3h, 010h, 0C1h, 0E3h, 004h, 066h, 003h, 0C3h, 0A3h, 060h, 09Bh, 000h
        db  000h, 0C1h, 0E8h, 010h, 0A3h, 018h, 00Ah, 000h, 000h, 0A1h, 080h, 09Bh, 000h, 000h, 08Bh, 01Dh
        db  068h, 09Bh, 000h, 000h, 081h, 0C3h, 000h, 001h, 000h, 000h, 0D1h, 0E3h, 04Bh, 06Ah, 000h, 06Ah
        db  000h, 068h, 0FAh, 000h, 000h, 000h, 053h, 050h, 0CDh, 020h, 07Ah, 000h, 001h, 000h, 083h, 0C4h
        db  014h, 06Ah, 000h, 050h, 052h, 06Ah, 000h, 06Ah, 03Bh, 0CDh, 020h, 07Ch, 000h, 001h, 000h, 083h
        db  0C4h, 014h, 0B8h, 004h, 000h, 000h, 000h, 0B9h, 020h, 000h, 000h, 000h, 0CDh, 020h, 09Bh, 000h
        db  001h, 000h, 072h, 024h, 089h, 035h, 084h, 09Bh, 000h, 000h, 0CDh, 020h, 09Bh, 000h, 001h, 000h
        db  072h, 016h, 089h, 035h, 088h, 09Bh, 000h, 000h, 0CDh, 020h, 09Bh, 000h, 001h, 000h, 072h, 008h
        db  089h, 035h, 08Ch, 09Bh, 000h, 000h, 061h, 0C3h, 0CDh, 020h, 0BFh, 000h, 001h, 000h, 060h, 0B8h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0F6h, 005h, 020h, 0EBh, 000h, 000h, 008h, 074h, 005h, 0B8h, 000h, 000h
        db  000h, 000h, 033h, 0F6h, 0BFh, 033h, 085h, 000h, 000h, 0CDh, 020h, 0B1h, 000h, 001h, 000h, 085h
        db  0C0h, 074h, 025h, 0B0h, 073h, 0CDh, 020h, 0DCh, 000h, 001h, 000h, 074h, 01Bh, 0BFh, 0EFh, 0FFh
        db  00Fh, 000h, 0B9h, 0F0h, 03Fh, 000h, 000h, 08Bh, 044h, 024h, 01Ch, 0FDh, 0F2h, 0AEh, 075h, 008h
        db  047h, 089h, 07Ch, 024h, 01Ch, 0F8h, 0EBh, 001h, 0F9h, 0FCh, 061h, 0C3h, 053h, 08Bh, 01Dh, 010h
        db  0F7h, 000h, 000h, 0E8h, 094h, 0FBh, 0FAh, 0FFh, 05Bh, 072h, 001h, 0C3h, 0CDh, 020h, 0BFh, 000h
        db  001h, 000h, 000h, 000h, 056h, 0BEh, 09Ch, 09Bh, 000h, 000h, 0B9h, 060h, 000h, 000h, 000h, 066h
        db  08Bh, 046h, 002h, 066h, 087h, 046h, 006h, 066h, 089h, 046h, 002h, 083h, 0C6h, 008h, 0E2h, 0EFh
        db  08Bh, 035h, 03Ch, 083h, 000h, 000h, 057h, 0BFh, 09Ch, 09Bh, 000h, 000h, 00Fh, 001h, 01Dh, 090h
        db  09Bh, 000h, 000h, 08Bh, 046h, 03Eh, 00Bh, 0C0h, 074h, 015h, 032h, 0C0h, 0FFh, 05Eh, 03Eh, 066h
        db  0B8h, 04Fh, 000h, 0CDh, 041h, 066h, 03Dh, 086h, 0F3h, 0B0h, 000h, 075h, 002h, 0B0h, 001h, 0A2h
        db  005h, 0BFh, 000h, 000h, 00Fh, 001h, 01Dh, 090h, 09Bh, 000h, 000h, 05Fh, 05Eh, 0C3h, 066h, 0B8h
        db  030h, 000h, 066h, 08Eh, 0D0h, 0BCh, 014h, 083h, 000h, 000h, 066h, 08Eh, 0D8h, 066h, 08Eh, 0C0h
        db  066h, 08Eh, 0E0h, 066h, 08Eh, 0E8h, 068h, 002h, 030h, 000h, 000h, 09Dh, 089h, 035h, 03Ch, 083h
        db  000h, 000h, 089h, 03Dh, 098h, 09Bh, 000h, 000h, 0E8h, 077h, 0FFh, 0FFh, 0FFh, 08Bh, 046h, 058h
        db  00Bh, 0C0h, 074h, 022h, 056h, 08Dh, 076h, 05Ch, 0BFh, 062h, 001h, 000h, 000h, 0FCh, 066h, 0ADh
        db  066h, 0ABh, 00Fh, 0B7h, 0C8h, 083h, 0F9h, 040h, 076h, 005h, 0B9h, 040h, 000h, 000h, 000h, 0FCh
        db  0F3h, 0A4h, 05Eh, 08Bh, 046h, 058h, 0A3h, 048h, 001h, 000h, 000h, 08Bh, 0C8h, 08Bh, 046h, 054h
        db  0A3h, 04Ch, 001h, 000h, 000h, 00Fh, 0B7h, 086h, 0AAh, 000h, 000h, 000h, 066h, 0A3h, 060h, 001h
        db  000h, 000h, 00Fh, 0B6h, 005h, 0FEh, 0FFh, 00Fh, 000h, 08Ah, 0A6h, 0ACh, 000h, 000h, 000h, 0F6h
        db  086h, 0A6h, 000h, 000h, 000h, 001h, 074h, 005h, 00Dh, 000h, 000h, 001h, 000h, 0F6h, 086h, 0A6h
        db  000h, 000h, 000h, 010h, 074h, 005h, 00Dh, 000h, 000h, 010h, 000h, 03Ch, 0FEh, 074h, 008h, 03Ch
        db  0FFh, 074h, 004h, 03Ch, 0FBh, 075h, 005h, 00Dh, 000h, 000h, 002h, 000h, 0E3h, 00Eh, 0F6h, 005h
        db  067h, 001h, 000h, 000h, 002h, 074h, 005h, 00Dh, 000h, 000h, 004h, 000h, 081h, 03Dh, 0D9h, 0FFh
        db  00Fh, 000h, 045h, 049h, 053h, 041h, 075h, 005h, 00Dh, 000h, 000h, 008h, 000h, 0A3h, 044h, 001h
        db  000h, 000h, 08Ah, 086h, 0A7h, 000h, 000h, 000h, 024h, 00Fh, 008h, 005h, 040h, 001h, 000h, 000h
        db  08Dh, 044h, 024h, 0DCh, 0A3h, 0A0h, 09Eh, 000h, 000h, 066h, 08Ch, 015h, 0A4h, 09Eh, 000h, 000h
        db  00Fh, 020h, 0D8h, 0A3h, 0B8h, 09Eh, 000h, 000h, 066h, 0C7h, 005h, 002h, 09Fh, 000h, 000h, 068h
        db  000h, 00Fh, 001h, 005h, 040h, 083h, 000h, 000h, 08Bh, 035h, 042h, 083h, 000h, 000h, 083h, 0C6h
        db  018h, 0B8h, 09Ch, 09Eh, 000h, 000h, 066h, 0BBh, 069h, 020h, 066h, 089h, 01Eh, 066h, 089h, 046h
        db  002h, 0C1h, 0E8h, 010h, 088h, 046h, 004h, 088h, 066h, 007h, 066h, 0B8h, 089h, 000h, 088h, 046h
        db  005h, 088h, 066h, 006h, 066h, 0B8h, 018h, 000h, 00Fh, 000h, 0D8h, 0F7h, 005h, 03Dh, 006h, 000h
        db  000h, 000h, 000h, 040h, 000h, 074h, 005h, 0E8h, 031h, 000h, 000h, 000h, 033h, 0C0h, 00Fh, 000h
        db  0D0h, 050h, 050h, 050h, 050h, 050h, 050h, 050h, 050h, 050h, 050h, 050h, 050h, 050h, 050h, 050h
        db  068h, 000h, 030h, 002h, 000h, 050h, 050h, 050h, 050h, 050h, 050h, 050h, 050h, 050h, 050h, 050h
        db  08Bh, 0ECh, 081h, 0ECh, 0D8h, 000h, 000h, 000h, 0E9h, 01Bh, 001h, 000h, 000h, 0C7h, 005h, 07Ch
        db  050h, 000h, 000h, 078h, 050h, 000h, 000h, 066h, 08Ch, 01Dh, 080h, 050h, 000h, 000h, 0C7h, 005h
        db  0B0h, 050h, 000h, 000h, 078h, 050h, 000h, 000h, 0C7h, 005h, 098h, 050h, 000h, 000h, 024h, 004h
        db  000h, 000h, 066h, 08Ch, 00Dh, 0C4h, 050h, 000h, 000h, 066h, 08Ch, 01Dh, 0CCh, 050h, 000h, 000h
        db  066h, 08Ch, 01Dh, 0C8h, 050h, 000h, 000h, 066h, 08Ch, 01Dh, 0C0h, 050h, 000h, 000h, 00Fh, 020h
        db  0D8h, 0A3h, 094h, 050h, 000h, 000h, 066h, 0C7h, 005h, 0DEh, 050h, 000h, 000h, 0FFh, 0FFh, 08Bh
        db  035h, 042h, 083h, 000h, 000h, 083h, 0C6h, 068h, 0B8h, 078h, 050h, 000h, 000h, 066h, 0BBh, 068h
        db  000h, 066h, 089h, 01Eh, 066h, 089h, 046h, 002h, 0C1h, 0E8h, 010h, 088h, 046h, 004h, 088h, 066h
        db  007h, 066h, 0B8h, 089h, 000h, 088h, 046h, 005h, 088h, 066h, 006h, 0BEh, 0DCh, 09Bh, 000h, 000h
        db  033h, 0C0h, 066h, 089h, 006h, 066h, 0C7h, 046h, 002h, 068h, 000h, 088h, 046h, 004h, 0C6h, 046h
        db  005h, 085h, 066h, 089h, 046h, 006h, 0C3h, 000h, 060h, 0B9h, 008h, 000h, 000h, 000h, 0E8h, 075h
        db  00Ah, 0FBh, 0FFh, 072h, 011h, 051h, 06Ah, 001h, 06Ah, 010h, 0CDh, 020h, 04Fh, 000h, 001h, 000h
        db  059h, 059h, 085h, 0C0h, 075h, 006h, 0CDh, 020h, 0BFh, 080h, 001h, 000h, 059h, 089h, 048h, 004h
        db  089h, 008h, 0C7h, 040h, 00Ch, 00Ch, 000h, 000h, 000h, 0A3h, 034h, 0C3h, 000h, 000h, 061h, 0C3h
        db  060h, 083h, 0ECh, 06Ch, 057h, 08Dh, 07Ch, 024h, 004h, 0CDh, 020h, 08Dh, 000h, 001h, 000h, 05Fh
        db  0CDh, 020h, 083h, 000h, 001h, 000h, 0CDh, 020h, 001h, 000h, 001h, 000h, 08Bh, 06Bh, 008h, 066h
        db  0C7h, 045h, 01Ch, 008h, 016h, 0B8h, 02Fh, 000h, 000h, 000h, 0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h, 056h, 08Dh, 074h, 024h, 004h, 0CDh, 020h, 08Eh, 000h, 001h
        db  000h, 05Eh, 083h, 0C4h, 06Ch, 061h, 0F8h, 0C3h, 08Bh, 035h, 03Ch, 083h, 000h, 000h, 08Bh, 076h
        db  02Eh, 08Dh, 096h, 013h, 0FFh, 0FFh, 0FFh, 0FFh, 072h, 004h, 0E8h, 071h, 007h, 000h, 000h, 08Bh
        db  035h, 03Ch, 083h, 000h, 000h, 08Bh, 076h, 02Eh, 033h, 0C9h, 083h, 0F9h, 003h, 00Fh, 084h, 08Eh
        db  000h, 000h, 000h, 08Bh, 01Eh, 085h, 0DBh, 00Fh, 084h, 084h, 000h, 000h, 000h, 066h, 083h, 07Eh
        db  008h, 002h, 074h, 00Fh, 066h, 083h, 07Eh, 008h, 02Eh, 074h, 061h, 081h, 0C6h, 0EDh, 000h, 000h
        db  000h, 0EBh, 0D7h, 083h, 07Bh, 02Ch, 000h, 074h, 0F2h, 083h, 0C9h, 001h, 060h, 0BEh, 05Ch, 083h
        db  000h, 000h, 08Bh, 03Dh, 03Ch, 083h, 000h, 000h, 08Dh, 057h, 03Eh, 089h, 056h, 018h, 08Bh, 047h
        db  03Eh, 089h, 046h, 002h, 066h, 08Bh, 047h, 042h, 066h, 089h, 046h, 006h, 032h, 0C0h, 0F7h, 005h
        db  010h, 008h, 000h, 000h, 000h, 000h, 040h, 000h, 074h, 002h, 0FEh, 0C8h, 0FFh, 053h, 02Ch, 0F7h
        db  005h, 021h, 008h, 000h, 000h, 000h, 000h, 040h, 000h, 074h, 00Eh, 08Bh, 03Dh, 03Ch, 083h, 000h
        db  000h, 0B8h, 002h, 000h, 000h, 000h, 0FFh, 05Fh, 03Eh, 061h, 0EBh, 09Fh, 083h, 07Bh, 02Ch, 000h
        db  074h, 099h, 083h, 0C9h, 002h, 060h, 0BEh, 0E0h, 0ECh, 000h, 000h, 0FFh, 053h, 02Ch, 061h, 0EBh
        db  08Ah, 080h, 03Dh, 005h, 0BFh, 000h, 000h, 000h, 074h, 00Ch, 0B8h, 001h, 0F0h, 000h, 000h, 0BEh
        db  07Ch, 083h, 000h, 000h, 0CDh, 041h, 08Bh, 035h, 03Ch, 083h, 000h, 000h, 08Bh, 046h, 032h, 0A3h
        db  054h, 083h, 000h, 000h, 056h, 0E8h, 0CEh, 051h, 000h, 000h, 059h, 0E8h, 098h, 0FEh, 0FFh, 0FFh
        db  0B8h, 004h, 000h, 000h, 000h, 0B9h, 004h, 000h, 000h, 000h, 0CDh, 020h, 09Bh, 000h, 001h, 000h
        db  00Fh, 082h, 0B4h, 001h, 000h, 000h, 089h, 035h, 00Ch, 0F2h, 000h, 000h, 08Bh, 035h, 03Ch, 083h
        db  000h, 000h, 0F6h, 086h, 0A7h, 000h, 000h, 000h, 080h, 074h, 018h, 00Fh, 0B6h, 086h, 0ADh, 000h
        db  000h, 000h, 08Bh, 035h, 054h, 083h, 000h, 000h, 0E8h, 05Dh, 00Eh, 000h, 000h, 089h, 035h, 054h
        db  083h, 000h, 000h, 0A1h, 098h, 09Bh, 000h, 000h, 00Fh, 0B7h, 040h, 004h, 040h, 0A3h, 050h, 001h
        db  000h, 000h, 0E8h, 036h, 004h, 000h, 000h, 08Bh, 035h, 03Ch, 083h, 000h, 000h, 08Bh, 0B6h, 03Eh
        db  001h, 000h, 000h, 0E8h, 007h, 005h, 000h, 000h, 00Fh, 0B7h, 005h, 003h, 007h, 000h, 000h, 0C1h
        db  0E0h, 004h, 08Dh, 080h, 030h, 003h, 000h, 000h, 0A3h, 000h, 0F2h, 000h, 000h, 08Bh, 015h, 050h
        db  001h, 000h, 000h, 0C1h, 0E2h, 004h, 08Dh, 0B2h, 080h, 000h, 000h, 000h, 052h, 06Ah, 000h, 068h
        db  080h, 000h, 000h, 000h, 0CDh, 020h, 04Fh, 000h, 001h, 000h, 059h, 059h, 05Ah, 00Bh, 0C0h, 00Fh
        db  084h, 025h, 001h, 000h, 000h, 0A3h, 058h, 083h, 000h, 000h, 08Bh, 0F8h, 0B9h, 020h, 000h, 000h
        db  000h, 0FCh, 0F3h, 0A5h, 033h, 0FFh, 066h, 087h, 07Ah, 02Ch, 0C1h, 0E7h, 004h, 074h, 075h, 08Bh
        db  0D7h, 083h, 0C9h, 0FFh, 033h, 0C0h, 0FCh, 0F2h, 0AEh, 038h, 007h, 075h, 0FAh, 083h, 0C7h, 003h
        db  0F2h, 0AEh, 02Bh, 0FAh, 08Dh, 08Fh, 000h, 004h, 000h, 000h, 083h, 0C1h, 01Fh, 080h, 0E1h, 0F0h
        db  051h, 052h, 068h, 000h, 002h, 000h, 000h, 051h, 0CDh, 020h, 04Fh, 000h, 001h, 000h, 059h, 059h
        db  05Eh, 059h, 085h, 0C0h, 00Fh, 084h, 0D0h, 000h, 000h, 000h, 0C6h, 000h, 04Dh, 08Bh, 015h, 050h
        db  001h, 000h, 000h, 066h, 089h, 050h, 001h, 0C1h, 0E9h, 004h, 049h, 066h, 089h, 048h, 003h, 083h
        db  0C0h, 010h, 0A3h, 054h, 001h, 000h, 000h, 0C6h, 005h, 0A4h, 001h, 000h, 000h, 0FFh, 08Dh, 04Fh
        db  003h, 0C1h, 0E9h, 002h, 08Bh, 0F8h, 0FCh, 0F3h, 0A5h, 0E8h, 0A1h, 009h, 000h, 000h, 00Fh, 082h
        db  096h, 000h, 000h, 000h, 0E8h, 082h, 005h, 000h, 000h, 08Bh, 035h, 03Ch, 083h, 000h, 000h, 08Bh
        db  04Eh, 050h, 051h, 08Bh, 076h, 04Ch, 089h, 035h, 048h, 083h, 000h, 000h, 089h, 00Dh, 04Ch, 083h
        db  000h, 000h, 06Ah, 000h, 051h, 056h, 0CDh, 020h, 06Ch, 000h, 001h, 000h, 083h, 0C4h, 00Ch, 059h
        db  00Bh, 0C0h, 074h, 066h, 08Bh, 0F0h, 0E8h, 0D1h, 062h, 000h, 000h, 08Bh, 035h, 03Ch, 083h, 000h
        db  000h, 08Bh, 076h, 02Eh, 0FCh, 08Bh, 006h, 085h, 0C0h, 074h, 057h, 056h, 08Bh, 01Dh, 040h, 0C3h
        db  000h, 000h, 066h, 08Bh, 050h, 006h, 08Bh, 078h, 014h, 066h, 03Bh, 053h, 006h, 075h, 00Fh, 066h
        db  083h, 0FAh, 000h, 074h, 009h, 060h, 033h, 0F6h, 0CDh, 020h, 0BEh, 000h, 001h, 000h, 03Bh, 07Bh
        db  014h, 072h, 002h, 08Bh, 0F3h, 08Bh, 01Bh, 00Bh, 0DBh, 075h, 0DEh, 08Bh, 00Eh, 089h, 008h, 089h
        db  006h, 08Bh, 0D8h, 05Eh, 08Bh, 046h, 004h, 089h, 043h, 02Ch, 066h, 08Bh, 046h, 00Ah, 066h, 089h
        db  043h, 00Ah, 081h, 0C6h, 0EDh, 000h, 000h, 000h, 0EBh, 0ABh, 0CDh, 020h, 0BFh, 000h, 001h, 000h
        db  0EBh, 0F8h, 0FFh, 035h, 03Ch, 083h, 000h, 000h, 0E8h, 0E7h, 023h, 000h, 000h, 059h, 00Bh, 0C0h
        db  074h, 0E8h, 0FFh, 035h, 03Ch, 083h, 000h, 000h, 0E8h, 067h, 04Eh, 000h, 000h, 0E8h, 0E7h, 003h
        db  000h, 000h, 0E8h, 00Dh, 0F8h, 0FFh, 0FFh, 0E8h, 03Ah, 060h, 000h, 000h, 0E8h, 033h, 012h, 000h
        db  000h, 0CDh, 020h, 000h, 000h, 033h, 000h, 072h, 016h, 06Ah, 000h, 0CDh, 020h, 001h, 000h, 033h
        db  000h, 059h, 0E8h, 0E1h, 00Dh, 000h, 000h, 08Bh, 014h, 024h, 0E8h, 01Eh, 05Bh, 000h, 000h, 05Ah
        db  0E8h, 04Fh, 009h, 000h, 000h, 0E8h, 0C2h, 009h, 000h, 000h, 0E8h, 00Dh, 004h, 000h, 000h, 0E8h
        db  024h, 061h, 000h, 000h, 0E8h, 0CBh, 00Bh, 000h, 000h, 0E8h, 09Ah, 0F5h, 0FFh, 0FFh, 0E8h, 0B9h
        db  003h, 000h, 000h, 0E8h, 0C4h, 0F9h, 0FFh, 0FFh, 0A1h, 098h, 09Bh, 000h, 000h, 066h, 0F7h, 040h
        db  01Ah, 004h, 000h, 074h, 008h, 060h, 0CDh, 020h, 014h, 000h, 006h, 000h, 061h, 0E8h, 0EEh, 01Eh
        db  000h, 000h, 060h, 033h, 0C0h, 033h, 0F6h, 0BFh, 0C0h, 083h, 000h, 000h, 0CDh, 020h, 0B1h, 000h
        db  001h, 000h, 00Bh, 0C0h, 074h, 01Ah, 06Ah, 000h, 068h, 000h, 010h, 000h, 000h, 068h, 000h, 080h
        db  00Bh, 000h, 0CDh, 020h, 06Ch, 000h, 001h, 000h, 083h, 0C4h, 00Ch, 0A3h, 031h, 073h, 000h, 000h
        db  061h, 033h, 0C0h, 08Bh, 01Dh, 010h, 0F7h, 000h, 000h, 08Bh, 035h, 058h, 083h, 000h, 000h, 0E8h
        db  028h, 001h, 000h, 000h, 0CDh, 020h, 017h, 000h, 003h, 000h, 0C7h, 005h, 0DCh, 0C5h, 000h, 000h
        db  000h, 000h, 000h, 010h, 0E8h, 050h, 059h, 000h, 000h, 08Bh, 01Dh, 010h, 0F7h, 000h, 000h, 080h
        db  04Bh, 01Ch, 008h, 0FBh, 0E8h, 07Bh, 00Ah, 000h, 000h, 0E8h, 0BDh, 009h, 000h, 000h, 0E8h, 0C4h
        db  060h, 000h, 000h, 033h, 0C0h, 0BFh, 0D0h, 083h, 000h, 000h, 0CDh, 020h, 046h, 001h, 001h, 000h
        db  0E3h, 02Dh, 039h, 041h, 014h, 075h, 028h, 066h, 081h, 079h, 004h, 000h, 004h, 073h, 020h, 051h
        db  080h, 049h, 00Ah, 002h, 040h, 08Bh, 035h, 058h, 083h, 000h, 000h, 0CDh, 020h, 047h, 001h, 001h
        db  000h, 059h, 073h, 00Bh, 080h, 061h, 00Ah, 0FDh, 0C7h, 041h, 018h, 0B6h, 014h, 000h, 000h, 0E8h
        db  043h, 0F6h, 0FFh, 0FFh, 08Bh, 01Dh, 010h, 0F7h, 000h, 000h, 0CDh, 020h, 017h, 000h, 006h, 000h
        db  0B8h, 001h, 000h, 000h, 000h, 08Bh, 035h, 058h, 083h, 000h, 000h, 0E8h, 09Ch, 000h, 000h, 000h
        db  0C7h, 005h, 0DCh, 0C5h, 000h, 000h, 000h, 000h, 000h, 020h, 08Bh, 00Dh, 04Ch, 083h, 000h, 000h
        db  08Bh, 035h, 048h, 083h, 000h, 000h, 0E8h, 009h, 061h, 000h, 000h, 0E8h, 031h, 061h, 000h, 000h
        db  08Bh, 035h, 054h, 083h, 000h, 000h, 033h, 0C0h, 048h, 0E8h, 03Ch, 00Bh, 000h, 000h, 0E8h, 03Eh
        db  051h, 000h, 000h, 0E8h, 05Ah, 00Fh, 000h, 000h, 0E8h, 02Ch, 05Ch, 000h, 000h, 0E8h, 0B8h, 008h
        db  000h, 000h, 0E8h, 011h, 059h, 000h, 000h, 0B8h, 002h, 000h, 000h, 000h, 08Bh, 01Dh, 010h, 0F7h
        db  000h, 000h, 08Bh, 035h, 058h, 083h, 000h, 000h, 0E8h, 03Fh, 000h, 000h, 000h, 0E8h, 037h, 041h
        db  000h, 000h, 0E8h, 039h, 0FBh, 0FFh, 0FFh, 0C7h, 005h, 0DCh, 0C5h, 000h, 000h, 000h, 000h, 000h
        db  040h, 06Ah, 000h, 0FFh, 035h, 058h, 083h, 000h, 000h, 0CDh, 020h, 051h, 000h, 001h, 000h, 059h
        db  059h, 0E8h, 019h, 004h, 000h, 000h, 0E8h, 015h, 0F4h, 0FFh, 0FFh, 0E9h, 08Ah, 0FEh, 0FAh, 0FFh
        db  00Bh, 0C0h, 074h, 006h, 0CDh, 020h, 08Ah, 001h, 001h, 000h, 061h, 0C3h, 060h, 00Bh, 0C0h, 074h
        db  008h, 050h, 0CDh, 020h, 088h, 001h, 001h, 000h, 058h, 0B9h, 040h, 0C3h, 000h, 000h, 089h, 04Ch
        db  024h, 00Ch, 08Bh, 009h, 0E3h, 0DAh, 089h, 04Ch, 024h, 018h, 083h, 0F8h, 001h, 077h, 006h, 00Fh
        db  0A3h, 041h, 00Ah, 072h, 0E9h, 08Bh, 015h, 098h, 09Bh, 000h, 000h, 066h, 0F7h, 042h, 01Ah, 060h
        db  000h, 075h, 05Fh, 0E8h, 090h, 0F4h, 0FFh, 0FFh, 08Bh, 051h, 02Ch, 08Bh, 05Ch, 024h, 010h, 08Bh
        db  074h, 024h, 004h, 08Bh, 03Ch, 024h, 08Bh, 06Ch, 024h, 008h, 089h, 00Dh, 050h, 083h, 000h, 000h
        db  0FFh, 051h, 018h, 0C7h, 005h, 050h, 083h, 000h, 000h, 000h, 000h, 000h, 000h, 08Bh, 044h, 024h
        db  01Ch, 08Bh, 04Ch, 024h, 018h, 0E8h, 079h, 0F4h, 0FFh, 0FFh, 072h, 013h, 083h, 0F8h, 001h, 072h
        db  006h, 0CDh, 020h, 009h, 000h, 017h, 000h, 077h, 095h, 00Fh, 0ABh, 041h, 00Ah, 0EBh, 08Fh, 08Bh
        db  07Ch, 024h, 018h, 0CDh, 020h, 0F8h, 000h, 001h, 000h, 08Bh, 04Ch, 024h, 00Ch, 0E9h, 07Ch, 0FFh
        db  0FFh, 0FFh, 083h, 0F8h, 000h, 075h, 09Ch, 066h, 083h, 079h, 006h, 01Ch, 075h, 095h, 066h, 0F7h
        db  042h, 01Ah, 040h, 000h, 074h, 005h, 0E8h, 0D1h, 00Fh, 000h, 000h, 066h, 0F7h, 042h, 01Ah, 020h
        db  000h, 074h, 080h, 0E8h, 055h, 015h, 000h, 000h, 0E9h, 076h, 0FFh, 0FFh, 0FFh, 060h, 08Bh, 035h
        db  03Ch, 083h, 000h, 000h, 08Bh, 076h, 02Eh, 081h, 0EEh, 0EDh, 000h, 000h, 000h, 0C7h, 006h, 080h
        db  0D9h, 000h, 000h, 033h, 0C0h, 083h, 03Eh, 000h, 074h, 023h, 0FFh, 005h, 004h, 0C7h, 000h, 000h
        db  056h, 0B9h, 019h, 000h, 000h, 000h, 08Dh, 076h, 00Ch, 083h, 03Eh, 000h, 074h, 006h, 040h, 083h
        db  0C6h, 009h, 0E2h, 0F5h, 05Eh, 081h, 0C6h, 0EDh, 000h, 000h, 000h, 0EBh, 0D8h, 0BBh, 009h, 000h
        db  000h, 000h, 0F7h, 0E3h, 08Bh, 0C8h, 0B8h, 005h, 000h, 000h, 000h, 0F7h, 025h, 004h, 0C7h, 000h
        db  000h, 003h, 0C1h, 0A3h, 008h, 0C7h, 000h, 000h, 033h, 0C9h, 080h, 03Dh, 005h, 0BFh, 000h, 000h
        db  000h, 075h, 002h, 0B5h, 002h, 051h, 050h, 0CDh, 020h, 04Fh, 000h, 001h, 000h, 059h, 059h, 00Bh
        db  0C0h, 074h, 06Ah, 0A3h, 000h, 0C7h, 000h, 000h, 08Bh, 00Dh, 004h, 0C7h, 000h, 000h, 08Bh, 035h
        db  03Ch, 083h, 000h, 000h, 08Bh, 076h, 02Eh, 081h, 0EEh, 0EDh, 000h, 000h, 000h, 08Bh, 0F8h, 0C6h
        db  047h, 004h, 000h, 08Bh, 006h, 089h, 007h, 033h, 0DBh, 051h, 0B9h, 019h, 000h, 000h, 000h, 08Bh
        db  044h, 033h, 00Ch, 00Bh, 0C0h, 074h, 01Ch, 089h, 044h, 03Bh, 005h, 0FEh, 047h, 004h, 08Bh, 044h
        db  033h, 010h, 089h, 044h, 03Bh, 009h, 08Ah, 044h, 033h, 014h, 088h, 044h, 03Bh, 00Dh, 083h, 0C3h
        db  009h, 0E2h, 0DCh, 081h, 0C6h, 0EDh, 000h, 000h, 000h, 00Fh, 0B6h, 047h, 004h, 048h, 0BBh, 009h
        db  000h, 000h, 000h, 0F7h, 0E3h, 083h, 0C0h, 00Eh, 003h, 0F8h, 059h, 0E2h, 0B2h, 061h, 0C3h, 0FCh
        db  066h, 0ADh, 00Fh, 0B7h, 0C8h, 051h, 0C1h, 0E1h, 002h, 041h, 06Ah, 000h, 051h, 0CDh, 020h, 04Fh
        db  000h, 001h, 000h, 059h, 059h, 08Bh, 0D8h, 0A3h, 009h, 09Bh, 000h, 000h, 059h, 089h, 00Dh, 00Dh
        db  09Bh, 000h, 000h, 0E3h, 015h, 051h, 0FCh, 0ACh, 00Fh, 0B6h, 0C8h, 0E8h, 00Bh, 000h, 000h, 000h
        db  089h, 003h, 083h, 0C3h, 004h, 003h, 0F1h, 059h, 0E2h, 0EBh, 0C3h, 053h, 051h, 056h, 08Bh, 01Dh
        db  03Ch, 083h, 000h, 000h, 08Bh, 05Bh, 02Eh, 08Bh, 03Bh, 00Bh, 0FFh, 074h, 013h, 0E8h, 0E9h, 0FCh
        db  0FAh, 0FFh, 074h, 008h, 081h, 0C3h, 0EDh, 000h, 000h, 000h, 0EBh, 0EBh, 08Bh, 0C7h, 0EBh, 005h
        db  0E8h, 0F0h, 0FCh, 0FAh, 0FFh, 05Eh, 059h, 05Bh, 0C3h, 033h, 0C0h, 08Bh, 00Dh, 040h, 0C3h, 000h
        db  000h, 08Bh, 009h, 0E3h, 00Fh, 066h, 081h, 079h, 004h, 000h, 004h, 072h, 0F4h, 089h, 041h, 03Ch
        db  08Bh, 0C1h, 0EBh, 0EDh, 0A3h, 0BCh, 0D9h, 000h, 000h, 0C3h, 000h, 000h, 060h, 0BEh, 058h, 018h
        db  000h, 000h, 0CDh, 020h, 00Ch, 000h, 001h, 000h, 00Fh, 0B7h, 0D0h, 0C1h, 0E8h, 010h, 08Bh, 0C8h
        db  0B8h, 041h, 000h, 000h, 000h, 0CDh, 020h, 045h, 000h, 001h, 000h, 0B8h, 046h, 000h, 000h, 000h
        db  0CDh, 020h, 045h, 000h, 001h, 000h, 0B8h, 002h, 000h, 000h, 000h, 0BEh, 06Ah, 00Eh, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h, 061h, 0E9h, 040h, 0F2h, 0FAh, 0FFh, 060h, 08Bh, 035h, 034h
        db  0C3h, 000h, 000h, 0B8h, 0D8h, 0D9h, 000h, 000h, 0CDh, 020h, 09Eh, 000h, 001h, 000h, 0B8h, 001h
        db  000h, 000h, 000h, 0B9h, 018h, 000h, 000h, 000h, 0CDh, 020h, 09Bh, 000h, 001h, 000h, 072h, 022h
        db  089h, 035h, 0D0h, 0D9h, 000h, 000h, 033h, 0C0h, 0B9h, 004h, 000h, 000h, 000h, 0CDh, 020h, 09Bh
        db  000h, 001h, 000h, 072h, 00Dh, 089h, 035h, 0E8h, 0D9h, 000h, 000h, 0E8h, 014h, 0F2h, 0FAh, 0FFh
        db  061h, 0C3h, 0CDh, 020h, 0BFh, 000h, 001h, 000h, 0EBh, 0F8h, 00Bh, 0C9h, 075h, 00Bh, 083h, 0F8h
        db  005h, 075h, 006h, 0B8h, 01Eh, 000h, 000h, 000h, 0C3h, 033h, 0C0h, 0F9h, 0C3h, 000h, 000h, 000h
        db  060h, 0A1h, 03Ch, 083h, 000h, 000h, 00Fh, 0B7h, 048h, 044h, 089h, 00Dh, 0ECh, 083h, 000h, 000h
        db  08Bh, 070h, 046h, 089h, 035h, 0E8h, 083h, 000h, 000h, 061h, 0C3h, 060h, 08Bh, 035h, 0E8h, 083h
        db  000h, 000h, 06Ah, 000h, 0FFh, 035h, 0ECh, 083h, 000h, 000h, 0CDh, 020h, 04Fh, 000h, 001h, 000h
        db  059h, 059h, 085h, 0C0h, 075h, 006h, 0CDh, 020h, 0BFh, 000h, 001h, 000h, 0A3h, 0E8h, 083h, 000h
        db  000h, 08Bh, 0F8h, 08Bh, 00Dh, 0ECh, 083h, 000h, 000h, 0FCh, 0C1h, 0E9h, 002h, 0F3h, 0A5h, 08Bh
        db  00Dh, 0ECh, 083h, 000h, 000h, 083h, 0E1h, 003h, 0F3h, 0A4h, 08Bh, 035h, 03Ch, 083h, 000h, 000h
        db  081h, 0C6h, 0AEh, 000h, 000h, 000h, 0BFh, 0A8h, 001h, 000h, 000h, 0B9h, 020h, 000h, 000h, 000h
        db  0F3h, 0A5h, 08Bh, 035h, 03Ch, 083h, 000h, 000h, 08Bh, 086h, 09Eh, 000h, 000h, 000h, 0A3h, 0F0h
        db  083h, 000h, 000h, 08Bh, 086h, 0A2h, 000h, 000h, 000h, 0A3h, 0F4h, 083h, 000h, 000h, 00Fh, 0B7h
        db  086h, 0A8h, 000h, 000h, 000h, 0A3h, 0E0h, 083h, 000h, 000h, 00Fh, 0B7h, 086h, 0A6h, 000h, 000h
        db  000h, 0A3h, 0E4h, 083h, 000h, 000h, 08Bh, 046h, 036h, 0A3h, 0D8h, 083h, 000h, 000h, 08Bh, 046h
        db  03Ah, 0A3h, 0DCh, 083h, 000h, 000h, 0BFh, 028h, 002h, 000h, 000h, 08Bh, 035h, 058h, 083h, 000h
        db  000h, 00Fh, 0B6h, 00Eh, 0E3h, 067h, 046h, 0FCh, 0ACh, 03Ch, 02Fh, 075h, 056h, 049h, 074h, 05Dh
        db  0ACh, 00Ch, 020h, 03Ch, 064h, 075h, 038h, 049h, 074h, 053h, 0ACh, 03Ch, 03Ah, 075h, 030h, 049h
        db  074h, 04Bh, 0ACh, 03Ch, 041h, 072h, 028h, 03Ch, 05Ah, 076h, 008h, 03Ch, 061h, 072h, 020h, 03Ch
        db  07Ah, 077h, 01Ch, 00Ch, 020h, 0AAh, 0C7h, 046h, 0FCh, 020h, 020h, 020h, 020h, 049h, 074h, 02Dh
        db  0ACh, 03Ch, 020h, 074h, 026h, 03Ch, 009h, 074h, 022h, 03Ch, 02Fh, 074h, 0C0h, 0EBh, 0D4h, 049h
        db  074h, 01Bh, 0ACh, 03Ch, 020h, 074h, 014h, 03Ch, 009h, 074h, 010h, 03Ch, 02Fh, 074h, 0AEh, 0E2h
        db  0F1h, 0EBh, 00Ah, 03Ch, 020h, 074h, 004h, 03Ch, 009h, 075h, 002h, 0E2h, 09Bh, 061h, 0C3h, 060h
        db  06Ah, 000h, 0FFh, 035h, 0E8h, 083h, 000h, 000h, 0CDh, 020h, 051h, 000h, 001h, 000h, 059h, 059h
        db  061h, 0C3h, 052h, 0CDh, 020h, 0B3h, 000h, 001h, 000h, 072h, 00Dh, 080h, 03Ah, 000h, 074h, 008h
        db  0CDh, 020h, 0ACh, 000h, 001h, 000h, 00Bh, 0D2h, 05Ah, 0C3h, 08Ah, 002h, 03Ch, 02Dh, 075h, 009h
        db  042h, 0E8h, 00Ah, 000h, 000h, 000h, 0F7h, 0D8h, 0C3h, 03Ch, 02Bh, 075h, 003h, 042h, 0EBh, 000h
        db  051h, 033h, 0C0h, 00Fh, 0B6h, 00Ah, 083h, 0E9h, 030h, 072h, 00Fh, 083h, 0F9h, 009h, 077h, 00Ah
        db  0D1h, 0E0h, 08Dh, 004h, 080h, 003h, 0C1h, 042h, 0EBh, 0E9h, 059h, 0C3h, 052h, 0CDh, 020h, 0B3h
        db  000h, 001h, 000h, 072h, 00Dh, 080h, 03Ah, 000h, 074h, 008h, 0CDh, 020h, 0AEh, 000h, 001h, 000h
        db  00Bh, 0D2h, 05Ah, 0C3h, 053h, 051h, 08Ah, 002h, 03Ch, 02Dh, 09Ch, 074h, 004h, 03Ch, 02Bh, 075h
        db  001h, 042h, 0E8h, 0B9h, 0FFh, 0FFh, 0FFh, 0E3h, 033h, 080h, 03Ah, 02Eh, 075h, 025h, 042h, 00Fh
        db  0B6h, 01Ah, 083h, 0EBh, 030h, 072h, 01Ch, 083h, 0FBh, 009h, 077h, 017h, 0D1h, 0E0h, 08Dh, 004h
        db  080h, 003h, 0C3h, 042h, 0E2h, 0E9h, 080h, 03Ah, 030h, 072h, 011h, 080h, 03Ah, 039h, 077h, 00Ch
        db  042h, 0EBh, 0F3h, 0E3h, 007h, 0D1h, 0E0h, 08Dh, 004h, 080h, 0E2h, 0F7h, 09Dh, 075h, 002h, 0F7h
        db  0D8h, 059h, 05Bh, 0C3h, 052h, 051h, 0CDh, 020h, 0B3h, 000h, 001h, 000h, 072h, 00Dh, 080h, 03Ah
        db  000h, 074h, 008h, 0CDh, 020h, 0B0h, 000h, 001h, 000h, 00Bh, 0D2h, 059h, 05Ah, 0C3h, 051h, 033h
        db  0C9h, 00Fh, 0B6h, 002h, 0E8h, 09Bh, 0F0h, 0FAh, 0FFh, 083h, 0E8h, 030h, 07Ch, 01Ah, 083h, 0F8h
        db  009h, 076h, 00Dh, 083h, 0E8h, 011h, 07Ch, 010h, 083h, 0F8h, 005h, 077h, 00Bh, 083h, 0C0h, 00Ah
        db  0C1h, 0E1h, 004h, 003h, 0C8h, 042h, 0EBh, 0D9h, 03Ch, 007h, 075h, 001h, 042h, 08Bh, 0C1h, 059h
        db  0C3h, 052h, 051h, 0CDh, 020h, 0B3h, 000h, 001h, 000h, 072h, 00Bh, 080h, 03Ah, 000h, 074h, 006h
        db  0CDh, 020h, 0B2h, 000h, 001h, 000h, 059h, 05Ah, 0C3h, 056h, 0BEh, 0CFh, 084h, 000h, 000h, 0E8h
        db  01Dh, 000h, 000h, 000h, 072h, 009h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh, 05Eh, 00Bh, 0D2h, 0C3h, 0BEh
        db  0DEh, 084h, 000h, 000h, 0E8h, 008h, 000h, 000h, 000h, 072h, 004h, 033h, 0C0h, 00Bh, 0D2h, 05Eh
        db  0C3h, 051h, 057h, 08Bh, 0FAh, 080h, 03Eh, 000h, 074h, 016h, 0E8h, 0FAh, 0EFh, 0FAh, 0FFh, 072h
        db  006h, 080h, 03Ch, 039h, 000h, 074h, 00Ch, 046h, 080h, 03Eh, 000h, 075h, 0FAh, 046h, 0EBh, 0E5h
        db  0F9h, 0EBh, 001h, 0F8h, 05Fh, 059h, 0C3h, 060h, 085h, 0F6h, 075h, 005h, 0BEh, 0C8h, 084h, 000h
        db  000h, 08Bh, 015h, 0E8h, 083h, 000h, 000h, 0A1h, 0ECh, 083h, 000h, 000h, 0E8h, 051h, 000h, 000h
        db  000h, 072h, 030h, 08Bh, 0F7h, 0E8h, 074h, 000h, 000h, 000h, 072h, 027h, 0E8h, 06Dh, 000h, 000h
        db  000h, 072h, 020h, 080h, 03Ah, 05Bh, 074h, 01Bh, 08Bh, 0FAh, 0E8h, 0AAh, 0EFh, 0FAh, 0FFh, 072h
        db  0E4h, 080h, 03Ch, 011h, 000h, 075h, 0DEh, 0E8h, 052h, 000h, 000h, 000h, 089h, 054h, 024h, 014h
        db  0F8h, 061h, 0C3h, 0F9h, 061h, 0C3h, 060h, 08Bh, 0C2h, 02Bh, 005h, 0E8h, 083h, 000h, 000h, 0F7h
        db  0D8h, 003h, 005h, 0ECh, 083h, 000h, 000h, 0E8h, 032h, 000h, 000h, 000h, 072h, 0E5h, 08Bh, 0F7h
        db  0EBh, 0C1h, 051h, 057h, 080h, 03Ah, 05Bh, 074h, 011h, 0E8h, 020h, 000h, 000h, 000h, 072h, 007h
        db  0E8h, 019h, 000h, 000h, 000h, 073h, 0EDh, 0F9h, 0EBh, 011h, 08Dh, 07Ah, 001h, 0E8h, 057h, 0EFh
        db  0FAh, 0FFh, 072h, 0E5h, 080h, 03Ch, 039h, 05Dh, 075h, 0DFh, 0F8h, 05Fh, 059h, 0C3h, 042h, 048h
        db  074h, 008h, 080h, 07Ah, 0FFh, 000h, 075h, 0F6h, 0F8h, 0C3h, 0F9h, 0C3h, 051h, 0A1h, 0D8h, 083h
        db  000h, 000h, 08Bh, 01Dh, 0DCh, 083h, 000h, 000h, 08Bh, 0C8h, 00Bh, 0CBh, 059h, 0C3h, 049h, 049h
        db  074h, 012h, 0A1h, 0E0h, 083h, 000h, 000h, 08Bh, 00Dh, 0E4h, 083h, 000h, 000h, 08Bh, 015h, 0F8h
        db  083h, 000h, 000h, 0C3h, 088h, 00Dh, 07Dh, 012h, 000h, 000h, 060h, 050h, 08Bh, 0ECh, 08Bh, 0C7h
        db  0C1h, 0E8h, 00Ch, 06Ah, 000h, 055h, 06Ah, 001h, 050h, 0CDh, 020h, 061h, 000h, 001h, 000h, 083h
        db  0C4h, 010h, 058h, 025h, 000h, 0F0h, 0FFh, 0FFh, 081h, 0E7h, 0FFh, 00Fh, 000h, 000h, 00Bh, 0C7h
        db  06Ah, 000h, 06Ah, 003h, 050h, 0CDh, 020h, 06Ch, 000h, 001h, 000h, 083h, 0C4h, 00Ch, 089h, 004h
        db  024h, 0A3h, 0BCh, 0ECh, 000h, 000h, 0B8h, 000h, 000h, 000h, 000h, 0B9h, 05Ch, 000h, 000h, 000h
        db  0C1h, 0E8h, 00Ch, 0C1h, 0E9h, 00Ch, 02Bh, 0C8h, 041h, 06Ah, 000h, 051h, 050h, 0CDh, 020h, 063h
        db  000h, 001h, 000h, 083h, 0C4h, 00Ch, 085h, 0C0h, 074h, 014h, 0BEh, 0B0h, 084h, 000h, 000h, 0ADh
        db  0C6h, 000h, 0E8h, 0ADh, 085h, 0C0h, 075h, 0F8h, 061h, 0E9h, 074h, 0FFh, 0FFh, 0FFh, 0CDh, 020h
        db  0BFh, 000h, 001h, 000h, 0A1h, 0F0h, 083h, 000h, 000h, 08Bh, 015h, 0F4h, 083h, 000h, 000h, 0C3h
        db  056h, 057h, 08Bh, 044h, 024h, 00Ch, 08Bh, 074h, 024h, 010h, 08Bh, 07Ch, 024h, 014h, 0E8h, 02Fh
        db  0FDh, 0FFh, 0FFh, 08Bh, 07Ch, 024h, 018h, 089h, 007h, 0B8h, 001h, 000h, 000h, 000h, 072h, 00Ch
        db  0B8h, 002h, 000h, 000h, 000h, 074h, 005h, 0B8h, 000h, 000h, 000h, 000h, 05Fh, 05Eh, 0C3h, 08Bh
        db  0D0h, 066h, 083h, 03Ah, 000h, 074h, 003h, 042h, 0EBh, 0F7h, 083h, 0C2h, 004h, 033h, 0C9h, 033h
        db  0C0h, 080h, 03Ch, 010h, 000h, 074h, 00Ch, 040h, 080h, 07Ch, 010h, 0FFh, 05Ch, 075h, 0F2h, 08Bh
        db  0C8h, 0EBh, 0EEh, 0E3h, 029h, 089h, 00Dh, 05Ch, 001h, 000h, 000h, 040h, 050h, 08Bh, 0F2h, 068h
        db  000h, 002h, 000h, 000h, 050h, 0CDh, 020h, 04Fh, 000h, 001h, 000h, 059h, 059h, 059h, 085h, 0C0h
        db  0F9h, 074h, 00Bh, 0A3h, 058h, 001h, 000h, 000h, 08Bh, 0F8h, 0FCh, 0F3h, 0A4h, 0F8h, 0C3h, 051h
        db  052h, 0CDh, 020h, 00Ah, 000h, 006h, 000h, 081h, 0F9h, 004h, 001h, 000h, 000h, 072h, 007h, 0E8h
        db  019h, 0F0h, 0FAh, 0FFh, 073h, 013h, 080h, 04Dh, 0F0h, 001h, 06Ah, 000h, 068h, 004h, 001h, 000h
        db  000h, 0CDh, 020h, 0A9h, 000h, 001h, 000h, 059h, 059h, 05Ah, 059h, 0C3h, 060h, 0BEh, 0FCh, 083h
        db  000h, 000h, 0FCh, 033h, 0C0h, 08Bh, 01Dh, 0B0h, 0D9h, 000h, 000h, 066h, 0ADh, 085h, 0C0h, 074h
        db  00Fh, 08Dh, 00Ch, 083h, 066h, 0ADh, 0C7h, 001h, 0D8h, 011h, 000h, 000h, 001h, 001h, 0EBh, 0EBh
        db  061h, 0C3h, 000h, 000h, 060h, 0BEh, 060h, 025h, 000h, 000h, 0CDh, 020h, 00Bh, 000h, 001h, 000h
        db  0A3h, 0ECh, 0D9h, 000h, 000h, 0BEh, 084h, 025h, 000h, 000h, 0CDh, 020h, 00Ch, 000h, 001h, 000h
        db  0A3h, 0F0h, 0D9h, 000h, 000h, 08Bh, 01Dh, 010h, 0F7h, 000h, 000h, 0E8h, 0C0h, 0EDh, 0FAh, 0FFh
        db  072h, 02Ch, 0E8h, 0F9h, 0EBh, 0FAh, 0FFh, 072h, 025h, 0BEh, 004h, 001h, 000h, 000h, 0CDh, 020h
        db  00Ch, 000h, 001h, 000h, 072h, 018h, 0A3h, 0F4h, 0D9h, 000h, 000h, 0BEh, 010h, 000h, 000h, 000h
        db  0B8h, 00Eh, 000h, 015h, 000h, 0CDh, 020h, 090h, 000h, 001h, 000h, 0F8h, 061h, 0C3h, 0CDh, 020h
        db  0BFh, 080h, 001h, 000h, 0A1h, 0F8h, 0D9h, 000h, 000h, 051h, 083h, 0C1h, 003h, 083h, 0E1h, 0FCh
        db  001h, 00Dh, 0F8h, 0D9h, 000h, 000h, 059h, 0F8h, 0C3h, 000h, 000h, 000h, 08Bh, 01Dh, 010h, 0F7h
        db  000h, 000h, 0E9h, 075h, 0EDh, 0FAh, 0FFh, 000h, 0FAh, 00Fh, 0B6h, 0D2h, 0ECh, 08Ah, 0E0h, 083h
        db  0C2h, 010h, 0EBh, 000h, 0ECh, 038h, 0E0h, 075h, 01Eh, 083h, 0EAh, 010h, 0B0h, 0FFh, 0EBh, 000h
        db  0EEh, 083h, 0C2h, 010h, 0EBh, 000h, 0ECh, 083h, 0EAh, 010h, 086h, 0E0h, 0EBh, 000h, 0EEh, 080h
        db  0FCh, 0FFh, 075h, 003h, 0FBh, 0F8h, 0C3h, 0FBh, 0F9h, 0C3h, 060h, 033h, 0C0h, 033h, 0F6h, 0BFh
        db  077h, 084h, 000h, 000h, 0CDh, 020h, 0B1h, 000h, 001h, 000h, 085h, 0C0h, 074h, 04Bh, 0FCh, 0BEh
        db  070h, 084h, 000h, 000h, 0ACh, 033h, 0D2h, 00Ah, 0D0h, 074h, 013h, 0E8h, 074h, 018h, 0FBh, 0FFh
        db  074h, 037h, 083h, 0EAh, 010h, 0E8h, 06Ah, 018h, 0FBh, 0FFh, 075h, 02Dh, 0EBh, 0E6h, 0B2h, 021h
        db  0E8h, 093h, 0FFh, 0FFh, 0FFh, 072h, 022h, 0B2h, 0A1h, 0E8h, 08Ah, 0FFh, 0FFh, 0FFh, 072h, 019h
        db  0B8h, 070h, 084h, 000h, 000h, 0BEh, 051h, 018h, 000h, 000h, 033h, 0D2h, 00Ah, 010h, 074h, 009h
        db  0CDh, 020h, 096h, 000h, 001h, 000h, 040h, 0EBh, 0F1h, 061h, 0C3h, 083h, 0C8h, 0FFh, 033h, 0F6h
        db  0BFh, 089h, 084h, 000h, 000h, 0CDh, 020h, 0B1h, 000h, 001h, 000h, 085h, 0C0h, 00Fh, 084h, 0A0h
        db  000h, 000h, 000h, 00Fh, 0B6h, 005h, 005h, 0BFh, 000h, 000h, 033h, 0F6h, 0BFh, 09Fh, 084h, 000h
        db  000h, 0CDh, 020h, 0B1h, 000h, 001h, 000h, 085h, 0C0h, 074h, 021h, 068h, 081h, 000h, 000h, 000h
        db  06Ah, 000h, 06Ah, 000h, 06Ah, 000h, 06Ah, 000h, 06Ah, 000h, 06Ah, 001h, 06Ah, 001h, 0CDh, 020h
        db  053h, 000h, 001h, 000h, 083h, 0C4h, 020h, 0A3h, 0D8h, 0E3h, 000h, 000h, 053h, 0CDh, 020h, 0BBh
        db  000h, 001h, 000h, 087h, 01Ch, 024h, 0BAh, 000h, 001h, 000h, 000h, 0F7h, 004h, 024h, 000h, 000h
        db  008h, 000h, 074h, 008h, 0F7h, 0C2h, 000h, 003h, 000h, 000h, 074h, 038h, 0E8h, 0C3h, 017h, 0FBh
        db  0FFh, 074h, 031h, 00Fh, 0ABh, 015h, 004h, 09Fh, 000h, 000h, 081h, 0FAh, 0E0h, 000h, 000h, 000h
        db  072h, 010h, 081h, 0FAh, 0E0h, 002h, 000h, 000h, 072h, 01Ah, 081h, 0FAh, 000h, 004h, 000h, 000h
        db  073h, 012h, 08Bh, 0C2h, 025h, 0FFh, 001h, 000h, 000h, 00Fh, 0ABh, 005h, 094h, 0E3h, 000h, 000h
        db  00Fh, 0ABh, 043h, 078h, 066h, 042h, 075h, 0B3h, 058h, 0C7h, 005h, 0D4h, 0E3h, 000h, 000h, 058h
        db  02Fh, 000h, 000h, 0C3h, 060h, 0B8h, 000h, 00Ah, 000h, 000h, 087h, 005h, 034h, 0E4h, 000h, 000h
        db  0A3h, 0FCh, 0E3h, 000h, 000h, 0B8h, 0B4h, 010h, 000h, 000h, 087h, 005h, 050h, 0E4h, 000h, 000h
        db  0A3h, 000h, 0E4h, 000h, 000h, 0B8h, 0ECh, 070h, 000h, 000h, 087h, 005h, 054h, 0E4h, 000h, 000h
        db  0A3h, 004h, 0E4h, 000h, 000h, 0B8h, 0D1h, 010h, 000h, 000h, 087h, 005h, 0D8h, 0E5h, 000h, 000h
        db  0A3h, 008h, 0E4h, 000h, 000h, 0B8h, 0ECh, 070h, 000h, 000h, 087h, 005h, 0DCh, 0E5h, 000h, 000h
        db  0A3h, 00Ch, 0E4h, 000h, 000h, 0B8h, 0ECh, 070h, 000h, 000h, 087h, 005h, 064h, 0E7h, 000h, 000h
        db  0A3h, 010h, 0E4h, 000h, 000h, 0BEh, 014h, 0C3h, 000h, 000h, 0CDh, 020h, 0EFh, 000h, 001h, 000h
        db  0BEh, 024h, 0C3h, 000h, 000h, 0CDh, 020h, 0EFh, 000h, 001h, 000h, 06Ah, 001h, 06Ah, 040h, 068h
        db  092h, 000h, 000h, 000h, 068h, 0FFh, 0FFh, 000h, 000h, 06Ah, 000h, 0CDh, 020h, 07Ah, 000h, 001h
        db  000h, 083h, 0C4h, 014h, 06Ah, 000h, 050h, 052h, 0CDh, 020h, 076h, 000h, 001h, 000h, 083h, 0C4h
        db  00Ch, 085h, 0C0h, 074h, 01Ch, 066h, 0A3h, 0EEh, 003h, 000h, 000h, 083h, 0ECh, 008h, 00Fh, 001h
        db  044h, 024h, 002h, 058h, 058h, 0A3h, 0ACh, 0EFh, 000h, 000h, 0C6h, 005h, 0CCh, 003h, 000h, 000h
        db  005h, 061h, 0C3h, 000h, 060h, 0B8h, 02Fh, 000h, 000h, 000h, 0BEh, 0A4h, 034h, 000h, 000h, 0CDh
        db  020h, 041h, 000h, 001h, 000h, 0B8h, 02Fh, 000h, 000h, 000h, 0CDh, 020h, 044h, 000h, 001h, 000h
        db  089h, 00Dh, 0D4h, 0ECh, 000h, 000h, 089h, 015h, 0D8h, 0ECh, 000h, 000h, 0BEh, 084h, 034h, 000h
        db  000h, 0CDh, 020h, 00Ch, 000h, 001h, 000h, 00Fh, 0B7h, 0D0h, 08Bh, 0C8h, 0C1h, 0E9h, 010h, 0B8h
        db  02Fh, 000h, 000h, 000h, 0CDh, 020h, 045h, 000h, 001h, 000h, 0BEh, 068h, 01Bh, 000h, 000h, 0CDh
        db  020h, 00Bh, 000h, 001h, 000h, 0A3h, 0C8h, 0ECh, 000h, 000h, 0BEh, 0F9h, 004h, 000h, 000h, 0CDh
        db  020h, 00Ch, 000h, 001h, 000h, 0A3h, 0CCh, 0ECh, 000h, 000h, 0BEh, 051h, 012h, 000h, 000h, 0CDh
        db  020h, 00Bh, 000h, 001h, 000h, 0A3h, 0D0h, 0ECh, 000h, 000h, 033h, 0C0h, 040h, 033h, 0D2h, 0BEh
        db  01Ch, 002h, 000h, 000h, 0CDh, 020h, 00Ah, 000h, 017h, 000h, 061h, 0C3h, 053h, 00Fh, 0B7h, 0D8h
        db  066h, 033h, 0C0h, 0C1h, 0E8h, 00Ch, 003h, 0C3h, 05Bh, 0C3h, 050h, 083h, 03Dh, 0C0h, 0ECh, 000h
        db  000h, 000h, 075h, 018h, 0B8h, 002h, 000h, 000h, 000h, 0B9h, 0C8h, 000h, 000h, 000h, 056h, 0CDh
        db  020h, 09Bh, 000h, 001h, 000h, 089h, 035h, 0C0h, 0ECh, 000h, 000h, 05Eh, 033h, 0C9h, 0BAh, 000h
        db  002h, 000h, 000h, 085h, 0F6h, 00Fh, 084h, 02Bh, 001h, 000h, 000h, 083h, 03Ch, 024h, 000h, 00Fh
        db  084h, 021h, 001h, 000h, 000h, 0FFh, 00Ch, 024h, 08Bh, 046h, 00Eh, 085h, 0C0h, 074h, 078h, 0E8h
        db  0A8h, 0FFh, 0FFh, 0FFh, 08Bh, 0D8h, 08Bh, 003h, 085h, 0C0h, 074h, 06Bh, 0E8h, 09Bh, 0FFh, 0FFh
        db  0FFh, 0E3h, 002h, 0EBh, 01Dh, 056h, 050h, 08Bh, 035h, 0C0h, 0ECh, 000h, 000h, 0CDh, 020h, 09Dh
        db  000h, 001h, 000h, 0CDh, 020h, 09Eh, 000h, 001h, 000h, 08Bh, 0F8h, 058h, 05Eh, 0B9h, 00Ah, 000h
        db  000h, 000h, 089h, 047h, 008h, 00Fh, 0B7h, 043h, 004h, 089h, 047h, 00Ch, 089h, 057h, 010h, 033h
        db  0C0h, 089h, 007h, 089h, 047h, 004h, 051h, 052h, 06Ah, 000h, 057h, 0CDh, 020h, 0A6h, 000h, 001h
        db  000h, 059h, 059h, 05Ah, 059h, 085h, 0C0h, 075h, 015h, 00Fh, 0B7h, 035h, 00Ch, 000h, 000h, 000h
        db  003h, 035h, 004h, 000h, 000h, 000h, 033h, 0C0h, 0CDh, 020h, 0BEh, 080h, 001h, 000h, 083h, 0C7h
        db  014h, 049h, 083h, 0C3h, 006h, 0EBh, 08Fh, 0F7h, 0C2h, 000h, 004h, 000h, 000h, 075h, 013h, 080h
        db  03Eh, 004h, 072h, 014h, 081h, 0CAh, 000h, 004h, 000h, 000h, 08Bh, 046h, 012h, 0E9h, 069h, 0FFh
        db  0FFh, 0FFh, 081h, 0E2h, 0FFh, 0FBh, 0FFh, 0FFh, 056h, 057h, 066h, 08Bh, 006h, 086h, 0C4h, 066h
        db  03Dh, 001h, 004h, 072h, 060h, 08Bh, 046h, 016h, 085h, 0C0h, 074h, 059h, 0E8h, 0FBh, 0FEh, 0FFh
        db  0FFh, 00Fh, 0B7h, 030h, 00Fh, 0B7h, 078h, 002h, 085h, 0F6h, 074h, 049h, 0C1h, 0E6h, 004h, 0C1h
        db  0E7h, 004h, 050h, 051h, 052h, 00Fh, 0B7h, 050h, 008h, 08Bh, 040h, 004h, 0E8h, 0DBh, 0FEh, 0FFh
        db  0FFh, 083h, 0ECh, 018h, 08Bh, 0DCh, 089h, 033h, 089h, 07Bh, 004h, 0C7h, 043h, 008h, 000h, 000h
        db  000h, 000h, 0C7h, 043h, 00Ch, 000h, 000h, 000h, 000h, 089h, 043h, 010h, 089h, 053h, 014h, 06Ah
        db  000h, 053h, 0CDh, 020h, 081h, 001h, 001h, 000h, 059h, 059h, 083h, 0C4h, 018h, 05Ah, 059h, 058h
        db  083h, 0C0h, 00Ah, 0EBh, 0ACh, 05Fh, 05Eh, 08Bh, 046h, 002h, 0E8h, 09Dh, 0FEh, 0FFh, 0FFh, 08Bh
        db  0F0h, 0E9h, 0CDh, 0FEh, 0FFh, 0FFh, 058h, 0C3h, 060h, 06Ah, 000h, 068h, 0C4h, 002h, 000h, 000h
        db  0CDh, 020h, 024h, 000h, 033h, 000h, 059h, 059h, 06Ah, 000h, 068h, 0C8h, 002h, 000h, 000h, 0CDh
        db  020h, 024h, 000h, 033h, 000h, 059h, 059h, 06Ah, 000h, 068h, 0CCh, 002h, 000h, 000h, 0CDh, 020h
        db  024h, 000h, 033h, 000h, 059h, 059h, 06Ah, 000h, 068h, 0D0h, 002h, 000h, 000h, 0CDh, 020h, 024h
        db  000h, 033h, 000h, 059h, 059h, 06Ah, 000h, 068h, 0D4h, 002h, 000h, 000h, 0CDh, 020h, 024h, 000h
        db  033h, 000h, 059h, 059h, 06Ah, 000h, 068h, 0D8h, 002h, 000h, 000h, 0CDh, 020h, 024h, 000h, 033h
        db  000h, 059h, 059h, 06Ah, 000h, 068h, 0DCh, 002h, 000h, 000h, 0CDh, 020h, 024h, 000h, 033h, 000h
        db  059h, 059h, 06Ah, 000h, 068h, 0E0h, 002h, 000h, 000h, 0CDh, 020h, 024h, 000h, 033h, 000h, 059h
        db  059h, 06Ah, 000h, 068h, 0E4h, 002h, 000h, 000h, 0CDh, 020h, 024h, 000h, 033h, 000h, 059h, 059h
        db  06Ah, 000h, 068h, 0E8h, 002h, 000h, 000h, 0CDh, 020h, 024h, 000h, 033h, 000h, 059h, 059h, 06Ah
        db  000h, 068h, 0ECh, 002h, 000h, 000h, 0CDh, 020h, 024h, 000h, 033h, 000h, 059h, 059h, 06Ah, 000h
        db  0FFh, 035h, 0D8h, 002h, 000h, 000h, 06Ah, 0FFh, 068h, 000h, 000h, 001h, 000h, 0CDh, 020h, 025h
        db  000h, 033h, 000h, 083h, 0C4h, 010h, 06Ah, 000h, 0FFh, 035h, 0E0h, 002h, 000h, 000h, 06Ah, 0FFh
        db  068h, 000h, 000h, 001h, 000h, 0CDh, 020h, 025h, 000h, 033h, 000h, 083h, 0C4h, 010h, 06Ah, 000h
        db  0FFh, 035h, 0CCh, 002h, 000h, 000h, 068h, 002h, 001h, 000h, 000h, 068h, 002h, 001h, 000h, 000h
        db  0CDh, 020h, 025h, 000h, 033h, 000h, 083h, 0C4h, 010h, 068h, 0D4h, 085h, 000h, 000h, 068h, 0F8h
        db  086h, 000h, 000h, 068h, 002h, 000h, 000h, 080h, 0CDh, 020h, 048h, 001h, 001h, 000h, 083h, 0C4h
        db  00Ch, 083h, 0F8h, 000h, 00Fh, 085h, 0B8h, 000h, 000h, 000h, 0C7h, 005h, 0D8h, 085h, 000h, 000h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 005h, 0D8h, 085h, 000h, 000h, 0C7h, 005h, 0F0h, 085h, 000h, 000h
        db  010h, 000h, 000h, 000h, 0C7h, 005h, 0F4h, 086h, 000h, 000h, 000h, 001h, 000h, 000h, 068h, 0F4h
        db  086h, 000h, 000h, 068h, 0F4h, 085h, 000h, 000h, 068h, 0DCh, 085h, 000h, 000h, 06Ah, 000h, 068h
        db  0F0h, 085h, 000h, 000h, 068h, 0E0h, 085h, 000h, 000h, 0FFh, 035h, 0D8h, 085h, 000h, 000h, 0FFh
        db  035h, 0D4h, 085h, 000h, 000h, 0CDh, 020h, 050h, 001h, 001h, 000h, 083h, 0C4h, 020h, 083h, 0F8h
        db  000h, 075h, 052h, 083h, 03Dh, 0DCh, 085h, 000h, 000h, 001h, 075h, 0A8h, 0BAh, 0F3h, 085h, 000h
        db  000h, 042h, 0F6h, 002h, 0FFh, 074h, 09Dh, 080h, 03Ah, 02Ch, 074h, 0F5h, 080h, 03Ah, 020h, 074h
        db  0F0h, 080h, 03Ah, 008h, 074h, 0EBh, 0CDh, 020h, 0B0h, 000h, 001h, 000h, 08Bh, 0C8h, 080h, 03Ah
        db  03Ah, 075h, 00Bh, 050h, 042h, 0CDh, 020h, 0B0h, 000h, 001h, 000h, 08Bh, 0C8h, 058h, 052h, 06Ah
        db  000h, 0FFh, 035h, 0CCh, 002h, 000h, 000h, 051h, 050h, 0CDh, 020h, 025h, 000h, 033h, 000h, 083h
        db  0C4h, 010h, 05Ah, 0EBh, 0BDh, 0FFh, 035h, 0D4h, 085h, 000h, 000h, 0CDh, 020h, 049h, 001h, 001h
        db  000h, 059h, 0B9h, 007h, 000h, 000h, 000h, 0BBh, 007h, 000h, 000h, 000h, 0BAh, 000h, 004h, 000h
        db  000h, 066h, 08Bh, 002h, 066h, 00Bh, 0C0h, 074h, 01Eh, 053h, 051h, 052h, 066h, 003h, 0D8h, 06Ah
        db  000h, 0FFh, 035h, 0CCh, 002h, 000h, 000h, 053h, 050h, 0CDh, 020h, 025h, 000h, 033h, 000h, 083h
        db  0C4h, 010h, 05Ah, 059h, 05Bh, 00Bh, 0C9h, 075h, 005h, 083h, 0F9h, 003h, 076h, 005h, 042h, 042h
        db  049h, 075h, 0CEh, 06Ah, 000h, 0FFh, 035h, 0CCh, 002h, 000h, 000h, 068h, 0DFh, 003h, 000h, 000h
        db  068h, 0C0h, 003h, 000h, 000h, 0CDh, 020h, 025h, 000h, 033h, 000h, 083h, 0C4h, 010h, 033h, 0FFh
        db  0BEh, 0FFh, 000h, 000h, 000h, 0BBh, 001h, 000h, 000h, 000h, 06Ah, 000h, 0FFh, 035h, 0D0h, 002h
        db  000h, 000h, 056h, 057h, 0CDh, 020h, 025h, 000h, 033h, 000h, 083h, 0C4h, 010h, 0E4h, 021h, 08Ah
        db  0E0h, 0E4h, 0A1h, 08Bh, 0C8h, 0B0h, 0FFh, 0E6h, 021h, 0E6h, 0A1h, 066h, 0BAh, 021h, 004h, 0ECh
        db  03Ch, 0FFh, 075h, 028h, 066h, 0BAh, 0A1h, 004h, 0ECh, 03Ch, 0FFh, 075h, 01Fh, 0B0h, 05Ah, 0E6h
        db  021h, 0F6h, 0D0h, 0E6h, 0A1h, 066h, 0BAh, 021h, 004h, 0ECh, 03Ch, 05Ah, 075h, 00Eh, 066h, 0BAh
        db  0A1h, 004h, 0ECh, 03Ch, 0A5h, 075h, 005h, 0BBh, 040h, 000h, 000h, 000h, 08Ah, 0C5h, 0E6h, 021h
        db  08Ah, 0C1h, 0E6h, 0A1h, 081h, 0C7h, 000h, 004h, 000h, 000h, 081h, 0C6h, 000h, 004h, 000h, 000h
        db  04Bh, 074h, 015h, 06Ah, 000h, 0FFh, 035h, 0D0h, 002h, 000h, 000h, 056h, 057h, 0CDh, 020h, 025h
        db  000h, 033h, 000h, 083h, 0C4h, 010h, 0EBh, 0DCh, 06Ah, 001h, 06Ah, 000h, 06Ah, 000h, 068h, 0ECh
        db  004h, 000h, 000h, 06Ah, 002h, 06Ah, 000h, 0CDh, 020h, 010h, 000h, 033h, 000h, 083h, 0C4h, 018h
        db  061h, 0C3h, 060h, 068h, 0F8h, 086h, 000h, 000h, 068h, 002h, 000h, 000h, 080h, 0CDh, 020h, 04Bh
        db  001h, 001h, 000h, 059h, 059h, 068h, 0D4h, 085h, 000h, 000h, 068h, 0F8h, 086h, 000h, 000h, 068h
        db  002h, 000h, 000h, 080h, 0CDh, 020h, 04Ah, 001h, 001h, 000h, 083h, 0C4h, 00Ch, 083h, 0F8h, 000h
        db  00Fh, 085h, 002h, 001h, 000h, 000h, 06Ah, 000h, 068h, 034h, 087h, 000h, 000h, 068h, 030h, 087h
        db  000h, 000h, 068h, 02Ch, 087h, 000h, 000h, 0FFh, 035h, 0CCh, 002h, 000h, 000h, 0CDh, 020h, 02Ch
        db  000h, 033h, 000h, 083h, 0C4h, 014h, 083h, 0F8h, 000h, 00Fh, 085h, 0CCh, 000h, 000h, 000h, 0C7h
        db  005h, 0D8h, 085h, 000h, 000h, 000h, 000h, 000h, 000h, 0BFh, 0F4h, 085h, 000h, 000h, 0A1h, 02Ch
        db  087h, 000h, 000h, 0E8h, 0CEh, 000h, 000h, 000h, 0A1h, 030h, 087h, 000h, 000h, 03Bh, 005h, 02Ch
        db  087h, 000h, 000h, 074h, 009h, 0C6h, 007h, 03Ah, 047h, 0E8h, 0B8h, 000h, 000h, 000h, 081h, 0FFh
        db  030h, 086h, 000h, 000h, 073h, 024h, 0C6h, 007h, 02Ch, 047h, 06Ah, 000h, 068h, 030h, 087h, 000h
        db  000h, 068h, 02Ch, 087h, 000h, 000h, 068h, 034h, 087h, 000h, 000h, 0CDh, 020h, 02Dh, 000h, 033h
        db  000h, 083h, 0C4h, 010h, 083h, 0F8h, 000h, 074h, 0B5h, 04Fh, 0C6h, 007h, 000h, 081h, 0EFh, 0F4h
        db  085h, 000h, 000h, 074h, 066h, 047h, 08Bh, 0D7h, 0BFh, 0E0h, 085h, 000h, 000h, 0A1h, 0D8h, 085h
        db  000h, 000h, 0E8h, 06Fh, 000h, 000h, 000h, 0C6h, 007h, 000h, 0FFh, 005h, 0D8h, 085h, 000h, 000h
        db  052h, 068h, 0F4h, 085h, 000h, 000h, 06Ah, 001h, 06Ah, 000h, 068h, 0E0h, 085h, 000h, 000h, 0FFh
        db  035h, 0D4h, 085h, 000h, 000h, 0CDh, 020h, 052h, 001h, 001h, 000h, 083h, 0C4h, 018h, 083h, 0F8h
        db  000h, 075h, 028h, 06Ah, 000h, 068h, 030h, 087h, 000h, 000h, 068h, 02Ch, 087h, 000h, 000h, 068h
        db  034h, 087h, 000h, 000h, 0CDh, 020h, 02Dh, 000h, 033h, 000h, 083h, 0C4h, 010h, 0BFh, 0F4h, 085h
        db  000h, 000h, 083h, 0F8h, 000h, 00Fh, 084h, 043h, 0FFh, 0FFh, 0FFh, 0FFh, 035h, 0D4h, 085h, 000h
        db  000h, 0CDh, 020h, 049h, 001h, 001h, 000h, 059h, 061h, 0C3h, 050h, 066h, 08Bh, 044h, 024h, 002h
        db  0E8h, 001h, 000h, 000h, 000h, 058h, 066h, 0C1h, 0C0h, 004h, 0E8h, 016h, 000h, 000h, 000h, 066h
        db  0C1h, 0C0h, 004h, 0E8h, 00Dh, 000h, 000h, 000h, 066h, 0C1h, 0C0h, 004h, 0E8h, 004h, 000h, 000h
        db  000h, 066h, 0C1h, 0C0h, 004h, 050h, 083h, 0E0h, 00Fh, 08Ah, 080h, 038h, 087h, 000h, 000h, 088h
        db  007h, 047h, 058h, 0C3h, 056h, 0BEh, 020h, 0EDh, 000h, 000h, 0CDh, 020h, 0EFh, 000h, 001h, 000h
        db  05Eh, 072h, 001h, 0C3h, 0CDh, 020h, 0BFh, 000h, 001h, 000h, 000h, 000h, 060h, 08Bh, 079h, 018h
        db  08Bh, 0F7h, 08Dh, 076h, 012h, 0FCh, 0ADh, 03Dh, 083h, 0F8h, 000h, 075h, 00Fh, 085h, 0F1h, 001h
        db  000h, 000h, 066h, 0ADh, 066h, 03Dh, 005h, 0E9h, 00Fh, 085h, 0E3h, 001h, 000h, 000h, 0ADh, 003h
        db  0F0h, 08Bh, 0DEh, 083h, 0C6h, 010h, 066h, 081h, 03Eh, 0C7h, 005h, 00Fh, 085h, 0D0h, 001h, 000h
        db  000h, 083h, 07Eh, 006h, 000h, 00Fh, 085h, 0C6h, 001h, 000h, 000h, 0C7h, 006h, 090h, 090h, 090h
        db  090h, 0C7h, 046h, 004h, 090h, 090h, 090h, 090h, 066h, 0C7h, 046h, 008h, 090h, 090h, 08Dh, 0B3h
        db  052h, 003h, 000h, 000h, 081h, 03Eh, 0B8h, 002h, 000h, 000h, 00Fh, 085h, 0A1h, 001h, 000h, 000h
        db  081h, 07Eh, 004h, 000h, 0B9h, 008h, 000h, 00Fh, 085h, 094h, 001h, 000h, 000h, 08Dh, 046h, 005h
        db  08Dh, 093h, 0F0h, 003h, 000h, 000h, 02Bh, 0D0h, 0C6h, 006h, 0E9h, 089h, 056h, 001h, 08Dh, 0B3h
        db  0F0h, 003h, 000h, 000h, 080h, 03Eh, 0A1h, 00Fh, 085h, 074h, 001h, 000h, 000h, 066h, 081h, 07Eh
        db  005h, 08Bh, 00Dh, 00Fh, 085h, 068h, 001h, 000h, 000h, 08Dh, 0B3h, 023h, 004h, 000h, 000h, 0BAh
        db  090h, 090h, 090h, 090h, 08Bh, 0C2h, 087h, 006h, 08Dh, 076h, 004h, 066h, 03Dh, 050h, 0BEh, 00Fh
        db  085h, 04Ch, 001h, 000h, 000h, 08Bh, 0C2h, 087h, 006h, 08Dh, 076h, 004h, 025h, 000h, 000h, 0FFh
        db  0FFh, 03Dh, 000h, 000h, 0CDh, 020h, 00Fh, 085h, 035h, 001h, 000h, 000h, 08Bh, 0C2h, 087h, 006h
        db  08Dh, 076h, 004h, 03Dh, 071h, 000h, 001h, 000h, 00Fh, 085h, 023h, 001h, 000h, 000h, 0B8h, 090h
        db  059h, 090h, 090h, 087h, 006h, 08Dh, 076h, 004h, 03Dh, 058h, 059h, 072h, 010h, 00Fh, 085h, 00Eh
        db  001h, 000h, 000h, 0FCh, 08Bh, 0F7h, 083h, 0C6h, 01Ch, 0ADh, 03Dh, 083h, 0F8h, 001h, 075h, 00Fh
        db  085h, 0FCh, 000h, 000h, 000h, 0ACh, 03Ch, 005h, 00Fh, 085h, 0F3h, 000h, 000h, 000h, 080h, 03Eh
        db  0E9h, 00Fh, 085h, 0EAh, 000h, 000h, 000h, 0C7h, 006h, 0F8h, 0C3h, 090h, 090h, 0C6h, 046h, 004h
        db  090h, 08Bh, 0F7h, 083h, 0C6h, 030h, 0ADh, 03Dh, 083h, 0F8h, 00Ch, 075h, 00Fh, 085h, 0CFh, 000h
        db  000h, 000h, 0ACh, 03Ch, 005h, 00Fh, 085h, 0C6h, 000h, 000h, 000h, 080h, 03Eh, 0E9h, 00Fh, 085h
        db  0BDh, 000h, 000h, 000h, 0C7h, 006h, 0F8h, 0C3h, 090h, 090h, 0C6h, 046h, 004h, 090h, 08Bh, 0F7h
        db  083h, 0C6h, 03Ah, 0ADh, 03Dh, 083h, 0F8h, 002h, 075h, 00Fh, 085h, 0A2h, 000h, 000h, 000h, 0ACh
        db  03Ch, 005h, 00Fh, 085h, 099h, 000h, 000h, 000h, 080h, 03Eh, 0E9h, 00Fh, 085h, 090h, 000h, 000h
        db  000h, 0C7h, 006h, 0F8h, 0C3h, 090h, 090h, 0C6h, 046h, 004h, 090h, 08Bh, 0F7h, 083h, 0C6h, 044h
        db  0ADh, 03Dh, 083h, 0F8h, 00Dh, 075h, 075h, 079h, 0ACh, 03Ch, 005h, 075h, 074h, 080h, 03Eh, 0E9h
        db  075h, 06Fh, 0C7h, 006h, 0F8h, 0C3h, 090h, 090h, 0C6h, 046h, 004h, 090h, 08Bh, 0F7h, 083h, 0C6h
        db  04Eh, 0ADh, 03Dh, 083h, 0F8h, 00Eh, 075h, 075h, 058h, 0ACh, 03Ch, 005h, 075h, 053h, 080h, 03Eh
        db  0E9h, 075h, 04Eh, 0C7h, 006h, 0F8h, 0C3h, 090h, 090h, 0C6h, 046h, 004h, 090h, 08Bh, 0F7h, 083h
        db  0C6h, 058h, 0ADh, 03Dh, 083h, 0F8h, 003h, 075h, 075h, 037h, 0ACh, 03Ch, 005h, 075h, 032h, 080h
        db  03Eh, 0E9h, 075h, 02Dh
loc_5_BEA0:
        mov    dword ptr [esi], 9090c3f8h
        mov    byte ptr [esi + 4], 90h
        mov    esi, edi
        add    esi, 62h
        lodsd
        cmp    eax, 7505f883h
        jne    SHORT loc_5_BECD
        lodsb
        cmp    al, 5
        jne    SHORT loc_5_BECD
        cmp    byte ptr [esi], 0e9h
        jne    SHORT loc_5_BECD
        mov    dword ptr [esi], 9090c3f8h
        mov    byte ptr [esi + 4], 90h
loc_5_BECB:
        popad
        ret
loc_5_BECD:
        jmp    SHORT loc_5_BECB
        db  08Bh, 0F7h, 08Dh, 076h, 009h, 0FCh, 0ADh, 03Dh, 083h, 0F8h, 000h, 075h, 00Fh, 085h, 0D2h, 001h
        db  000h, 000h, 066h, 0ADh, 066h, 03Dh, 005h, 0E9h, 075h, 0E4h, 0ADh, 003h, 0F0h, 08Bh, 0DEh, 08Dh
        db  073h, 01Ch, 081h, 03Eh, 0B8h, 002h, 000h, 000h, 075h, 0D4h, 081h, 07Eh, 004h, 000h, 0B9h, 008h
        db  000h, 075h, 0CBh, 08Dh, 046h, 005h, 08Dh, 093h, 0B6h, 000h, 000h, 000h, 02Bh, 0D0h, 0C6h, 006h
        db  0E9h, 089h, 056h, 001h, 08Dh, 0B3h, 0B6h, 000h, 000h, 000h, 080h, 03Eh, 0A1h, 075h, 0AFh, 066h
        db  081h, 07Eh, 005h, 08Bh, 00Dh, 075h, 0A7h, 08Dh, 0B3h, 0E9h, 000h, 000h, 000h, 0BAh, 090h, 090h
        db  090h, 090h, 08Bh, 0C2h, 087h, 006h, 08Dh, 076h, 004h, 066h, 03Dh, 050h, 0BEh, 075h, 08Fh, 08Bh
        db  0C2h, 087h, 006h, 08Dh, 076h, 004h, 025h, 000h, 000h, 0FFh, 0FFh, 03Dh, 000h, 000h, 0CDh, 020h
        db  00Fh, 085h, 078h, 0FFh, 0FFh, 0FFh, 08Bh, 0C2h, 087h, 006h, 08Dh, 076h, 004h, 03Dh, 071h, 000h
        db  001h, 000h, 00Fh, 085h, 066h, 0FFh, 0FFh, 0FFh, 0B8h, 090h, 059h, 090h, 090h, 087h, 006h, 08Dh
        db  076h, 004h, 03Dh, 058h, 059h, 072h, 010h, 00Fh, 085h, 051h, 0FFh, 0FFh, 0FFh, 0FCh, 08Bh, 0F7h
        db  083h, 0C6h, 013h, 0ADh, 03Dh, 083h, 0F8h, 001h, 075h, 00Fh, 085h, 03Fh, 0FFh, 0FFh, 0FFh, 0ACh
        db  03Ch, 005h, 00Fh, 085h, 036h, 0FFh, 0FFh, 0FFh, 080h, 03Eh, 0E9h, 00Fh, 085h, 02Dh, 0FFh, 0FFh
        db  0FFh, 0C7h, 006h, 0F8h, 0C3h, 090h, 090h, 0C6h, 046h, 004h, 090h, 08Bh, 0F7h, 083h, 0C6h, 027h
        db  0ADh, 03Dh, 083h, 0F8h, 00Ch, 075h, 00Fh, 085h, 012h, 0FFh, 0FFh, 0FFh, 0ACh, 03Ch, 005h, 00Fh
        db  085h, 009h, 0FFh, 0FFh, 0FFh, 080h, 03Eh, 0E9h, 00Fh, 085h, 000h, 0FFh, 0FFh, 0FFh, 0C7h, 006h
        db  0F8h, 0C3h, 090h, 090h, 0C6h, 046h, 004h, 090h, 08Bh, 0F7h, 083h, 0C6h, 031h, 0ADh, 03Dh, 083h
        db  0F8h, 002h, 075h, 00Fh, 085h, 0E5h, 0FEh, 0FFh, 0FFh, 0ACh, 03Ch, 005h, 00Fh, 085h, 0DCh, 0FEh
        db  0FFh, 0FFh, 080h, 03Eh, 0E9h, 00Fh, 085h, 0D3h, 0FEh, 0FFh, 0FFh, 0C7h, 006h, 0F8h, 0C3h, 090h
        db  090h, 0C6h, 046h, 004h, 090h, 08Bh, 0F7h, 083h, 0C6h, 03Bh, 0ADh, 03Dh, 083h, 0F8h, 00Dh, 075h
        db  00Fh, 085h, 0B8h, 0FEh, 0FFh, 0FFh, 0ACh, 03Ch, 005h, 00Fh, 085h, 0AFh, 0FEh, 0FFh, 0FFh, 080h
        db  03Eh, 0E9h, 00Fh, 085h, 0A6h, 0FEh, 0FFh, 0FFh, 0C7h, 006h, 0F8h, 0C3h, 090h, 090h, 0C6h, 046h
        db  004h, 090h, 08Bh, 0F7h, 083h, 0C6h, 045h, 0ADh, 03Dh, 083h, 0F8h, 00Eh, 075h, 00Fh, 085h, 08Bh
        db  0FEh, 0FFh, 0FFh, 0ACh, 03Ch, 005h, 00Fh, 085h, 082h, 0FEh, 0FFh, 0FFh, 080h, 03Eh, 0E9h, 00Fh
        db  085h, 079h, 0FEh, 0FFh, 0FFh, 0C7h, 006h, 0F8h, 0C3h, 090h, 090h, 0C6h, 046h, 004h, 090h, 08Bh
        db  0F7h, 083h, 0C6h, 04Fh, 0ADh, 03Dh, 083h, 0F8h, 003h, 075h, 00Fh, 085h, 05Eh, 0FEh, 0FFh, 0FFh
        db  0ACh, 03Ch, 002h, 00Fh, 085h, 055h, 0FEh, 0FFh, 0FFh, 080h, 03Eh, 0EBh, 00Fh, 085h, 04Ch, 0FEh
        db  0FFh, 0FFh, 066h, 0C7h, 006h, 0F8h, 0C3h, 08Bh, 0F7h, 083h, 0C6h, 056h, 0ADh, 03Dh, 083h, 0F8h
        db  005h, 075h, 00Fh, 085h, 036h, 0FEh, 0FFh, 0FFh, 0ACh, 03Ch, 002h, 00Fh, 085h, 02Dh, 0FEh, 0FFh
        db  0FFh, 080h, 03Eh, 0EBh, 00Fh, 085h, 024h, 0FEh, 0FFh, 0FFh, 066h, 0C7h, 006h, 0F8h, 0C3h, 0E9h
        db  018h, 0FEh, 0FFh, 0FFh, 08Bh, 0F7h, 08Dh, 036h, 0FCh, 0ADh, 03Dh, 083h, 0F8h, 000h, 075h, 00Fh
        db  085h, 009h, 0FEh, 0FFh, 0FFh, 066h, 0ADh, 066h, 03Dh, 005h, 0E9h, 00Fh, 085h, 0FDh, 0FDh, 0FFh
        db  0FFh, 0ADh, 003h, 0F0h, 08Bh, 0DEh, 08Dh, 073h, 01Ch, 081h, 03Eh, 0B8h, 002h, 000h, 000h, 00Fh
        db  085h, 0E9h, 0FDh, 0FFh, 0FFh, 081h, 07Eh, 004h, 000h, 0B9h, 008h, 000h, 00Fh, 085h, 0DCh, 0FDh
        db  0FFh, 0FFh, 08Dh, 046h, 005h, 08Dh, 093h, 0B6h, 000h, 000h, 000h, 02Bh, 0D0h, 0C6h, 006h, 0E9h
        db  089h, 056h, 001h, 08Dh, 0B3h, 0B6h, 000h, 000h, 000h, 080h, 03Eh, 0A1h, 00Fh, 085h, 0BCh, 0FDh
        db  0FFh, 0FFh, 066h, 081h, 07Eh, 005h, 08Bh, 00Dh, 00Fh, 085h, 0B0h, 0FDh, 0FFh, 0FFh, 08Dh, 0B3h
        db  0E9h, 000h, 000h, 000h, 0BAh, 090h, 090h, 090h, 090h, 08Bh, 0C2h, 087h, 006h, 08Dh, 076h, 004h
        db  066h, 03Dh, 050h, 0BEh, 00Fh, 085h, 094h, 0FDh, 0FFh, 0FFh, 08Bh, 0C2h, 087h, 006h, 08Dh, 076h
        db  004h, 025h, 000h, 000h, 0FFh, 0FFh, 03Dh, 000h, 000h, 0CDh, 020h, 00Fh, 085h, 07Dh, 0FDh, 0FFh
        db  0FFh, 08Bh, 0C2h, 087h, 006h, 08Dh, 076h, 004h, 03Dh, 071h, 000h, 001h, 000h, 00Fh, 085h, 06Bh
        db  0FDh, 0FFh, 0FFh, 0B8h, 090h, 059h, 090h, 090h, 087h, 006h, 08Dh, 076h, 004h, 03Dh, 058h, 059h
        db  072h, 010h, 00Fh, 085h, 056h, 0FDh, 0FFh, 0FFh, 0FCh, 08Bh, 0F7h, 083h, 0C6h, 00Ah, 0ADh, 03Dh
        db  083h, 0F8h, 001h, 075h, 00Fh, 085h, 044h, 0FDh, 0FFh, 0FFh, 0ACh, 03Ch, 005h, 00Fh, 085h, 03Bh
        db  0FDh, 0FFh, 0FFh, 080h, 03Eh, 0E9h, 00Fh, 085h, 032h, 0FDh, 0FFh, 0FFh, 0C7h, 006h, 0F8h, 0C3h
        db  090h, 090h, 0C6h, 046h, 004h, 090h, 08Bh, 0F7h, 083h, 0C6h, 01Eh, 0ADh, 03Dh, 083h, 0F8h, 00Ch
        db  075h, 00Fh, 085h, 017h, 0FDh, 0FFh, 0FFh, 0ACh, 03Ch, 005h, 00Fh, 085h, 00Eh, 0FDh, 0FFh, 0FFh
        db  080h, 03Eh, 0E9h, 00Fh, 085h, 005h, 0FDh, 0FFh, 0FFh, 0C7h, 006h, 0F8h, 0C3h, 090h, 090h, 0C6h
        db  046h, 004h, 090h, 08Bh, 0F7h, 083h, 0C6h, 028h, 0ADh, 03Dh, 083h, 0F8h, 002h, 075h, 00Fh, 085h
        db  0EAh, 0FCh, 0FFh, 0FFh, 0ACh, 03Ch, 005h, 00Fh, 085h, 0E1h, 0FCh, 0FFh, 0FFh, 080h, 03Eh, 0E9h
        db  00Fh, 085h, 0D8h, 0FCh, 0FFh, 0FFh, 0C7h, 006h, 0F8h, 0C3h, 090h, 090h, 0C6h, 046h, 004h, 090h
        db  08Bh, 0F7h, 083h, 0C6h, 032h, 0ADh, 03Dh, 083h, 0F8h, 00Dh, 075h, 00Fh, 085h, 0BDh, 0FCh, 0FFh
        db  0FFh, 0ACh, 03Ch, 005h, 00Fh, 085h, 0B4h, 0FCh, 0FFh, 0FFh, 080h, 03Eh, 0E9h, 00Fh, 085h, 0ABh
        db  0FCh, 0FFh, 0FFh, 0C7h, 006h, 0F8h, 0C3h, 090h, 090h, 0C6h, 046h, 004h, 090h, 08Bh, 0F7h, 083h
        db  0C6h, 03Ch, 0ADh, 03Dh, 083h, 0F8h, 003h, 075h, 00Fh, 085h, 090h, 0FCh, 0FFh, 0FFh, 0ACh, 03Ch
        db  002h, 00Fh, 085h, 087h, 0FCh, 0FFh, 0FFh, 080h, 03Eh, 0EBh, 00Fh, 085h, 07Eh, 0FCh, 0FFh, 0FFh
        db  066h, 0C7h, 006h, 0F8h, 0C3h, 0E9h, 072h, 0FCh, 0FFh, 0FFh, 060h, 08Bh, 071h, 018h, 08Dh, 076h
        db  012h, 0FCh, 0ADh, 03Dh, 083h, 0F8h, 000h, 075h, 075h, 032h, 066h, 0ADh, 066h, 03Dh, 005h, 0E9h
        db  075h, 02Ah, 0ADh, 003h, 0F0h, 081h, 0C6h, 012h, 003h, 000h, 000h, 0ACh, 03Ch, 0E8h, 075h, 01Ch
        db  0ADh, 003h, 0F0h, 083h, 0C6h, 019h, 081h, 03Eh, 0B8h, 001h, 000h, 000h, 075h, 00Eh, 066h, 081h
        db  07Eh, 004h, 000h, 0C3h, 075h, 006h, 080h, 04Eh, 001h, 002h, 061h, 0C3h, 0EBh, 0FCh, 057h, 033h
        db  0C0h, 0CDh, 020h, 046h, 001h, 001h, 000h, 0E3h, 061h, 066h, 081h, 079h, 004h, 000h, 004h, 073h
        db  059h, 08Bh, 0E9h, 08Bh, 077h, 008h, 0FCh, 0ADh, 085h, 0C0h, 074h, 004h, 001h, 028h, 0EBh, 0F7h
        db  08Dh, 07Fh, 00Ch, 08Bh, 00Fh, 0E3h, 02Ah, 08Bh, 077h, 004h, 003h, 0F5h, 06Ah, 000h, 06Ah, 000h
        db  06Ah, 000h, 051h, 056h, 0CDh, 020h, 073h, 001h, 001h, 000h, 083h, 0C4h, 014h, 085h, 0C0h, 074h
        db  029h, 057h, 08Bh, 00Fh, 08Bh, 07Fh, 008h, 0F3h, 0A6h, 05Fh, 075h, 01Eh, 083h, 0C7h, 00Ch, 0EBh
        db  0D2h, 083h, 0C7h, 00Ch, 08Bh, 00Fh, 0E3h, 011h, 08Bh, 077h, 008h, 057h, 08Bh, 07Fh, 004h, 003h
        db  0FDh, 0F3h, 0A4h, 05Fh, 083h, 0C7h, 00Ch, 0EBh, 0EBh, 00Ch, 0F9h, 05Fh, 0C3h, 066h, 083h, 00Ah
        db  002h, 052h, 08Bh, 072h, 00Ch, 066h, 08Bh, 042h, 004h, 066h, 083h, 00Ah, 002h, 052h, 08Bh, 072h
        db  00Ch, 00Fh, 0B7h, 042h, 004h, 066h, 08Eh, 045h, 03Ch, 00Fh, 0B7h, 07Dh, 004h, 0BEh, 05Ch, 000h
        db  000h, 000h, 00Fh, 0B7h, 00Dh, 06Fh, 000h, 000h, 000h, 0F3h, 0A6h, 0E3h, 019h, 00Fh, 0B7h, 07Dh
        db  004h, 0BEh, 071h, 000h, 000h, 000h, 00Fh, 0B7h, 00Dh, 086h, 000h, 000h, 000h, 0F3h, 0A6h, 0E3h
        db  014h, 0E9h, 0D5h, 0FEh, 0FFh, 0FFh, 066h, 0B8h, 004h, 03Ch, 0CDh, 020h, 01Ch, 000h, 001h, 000h
        db  08Bh, 0F8h, 0BEh, 05Ch, 000h, 000h, 000h, 00Fh, 0B7h, 00Dh, 06Fh, 000h, 000h, 000h, 0F3h, 0A6h
        db  074h, 015h, 097h, 0BEh, 071h, 000h, 000h, 000h, 08Ah, 00Dh, 086h, 000h, 000h, 000h, 0F3h, 0A6h
        db  074h, 014h, 0E9h, 0D5h, 0FEh, 0FFh, 0FFh, 033h, 0C9h, 066h, 087h, 04Ah, 046h, 00Fh, 0AFh, 0C1h
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h, 08Bh, 072h, 038h, 0CDh, 020h, 013h, 000h, 001h, 000h, 0CDh
        db  020h, 015h, 000h, 001h, 000h, 0C3h, 0BEh, 0ACh, 0F7h, 0FFh, 0FFh, 0CDh, 020h, 02Ah, 000h, 001h
        db  000h, 0F8h, 0C3h, 083h, 0F8h, 001h, 075h, 002h, 0EBh, 0ECh, 0F8h, 0C3h, 0BEh, 0CBh, 0FEh, 0FFh
        db  0FFh, 0CDh, 020h, 03Ch, 000h, 001h, 000h, 089h, 035h, 0CCh, 000h, 000h, 000h, 0BEh, 0CBh, 0FEh
        db  0FFh, 0FFh, 0CDh, 020h, 03Ch, 000h, 001h, 000h, 089h, 035h, 0CCh, 000h, 000h, 000h, 056h, 08Bh
        db  035h, 0CCh, 000h, 000h, 000h, 0CDh, 020h, 012h, 000h, 001h, 000h, 0C6h, 005h, 0E6h, 000h, 000h
        db  000h, 0FFh, 05Eh, 0C3h, 053h, 051h, 052h, 057h, 056h, 083h, 025h, 0CCh, 000h, 000h, 000h, 000h
        db  0E9h, 013h, 007h, 000h, 000h, 0CCh, 0CCh, 083h, 0F8h, 001h, 0EBh, 002h, 0BEh, 0ACh, 0F7h, 0FFh
        db  0FFh, 0BEh, 0ACh, 0F7h, 0FFh, 0FFh, 033h, 0F6h, 087h, 035h, 0CCh, 000h, 000h, 000h, 0CDh, 020h
        db  03Eh, 000h, 001h, 000h, 0C6h, 005h, 0E6h, 000h, 000h, 000h, 0FFh, 00Ah, 003h, 0C6h, 022h, 003h
        db  00Ah, 000h, 000h, 042h, 049h, 047h, 057h, 049h, 04Eh, 020h, 020h, 001h, 000h, 000h, 081h, 0FEh
        db  0FFh, 0FFh, 0FFh, 0F9h, 0C3h, 09Ch, 066h, 0F7h, 005h, 0DCh, 0FFh, 0FFh, 0FFh, 000h, 080h, 075h
        db  00Bh, 0BEh, 0F4h, 0FCh, 0FFh, 0FFh, 0CDh, 020h, 0BEh, 000h, 001h, 000h, 066h, 083h, 03Dh, 0DEh
        db  0FFh, 0FFh, 0FFh, 000h, 074h, 00Bh, 0BEh, 077h, 0FDh, 0FFh, 0FFh, 0CDh, 020h, 0BEh, 000h, 001h
        db  000h, 066h, 0FFh, 005h, 0DEh, 0FFh, 0FFh, 0FFh, 050h, 051h, 052h, 056h, 0B8h, 05Eh, 000h, 001h
        db  000h, 0BEh, 04Ah, 0EEh, 0FFh, 0FFh, 0CDh, 020h, 090h, 000h, 001h, 000h, 073h, 00Bh, 0BEh, 0F4h
        db  0FCh, 0FFh, 0FFh, 0CDh, 020h, 0BEh, 000h, 001h, 000h, 089h, 035h, 0F0h, 0FFh, 0FFh, 0FFh, 066h
        db  081h, 00Dh, 0DCh, 0FFh, 0FFh, 0FFh, 000h, 008h, 05Eh, 05Ah, 059h, 058h, 09Dh, 0FFh, 015h, 0E4h
        db  0FFh, 0FFh, 0FFh, 09Ch, 09Ch, 083h, 0F8h, 01Ah, 076h, 009h, 09Dh, 0FFh, 025h, 0E4h, 0FFh, 0FFh
        db  0FFh, 090h, 090h, 066h, 0F7h, 005h, 0DCh, 0FFh, 0FFh, 0FFh, 000h, 080h, 075h, 007h, 0BEh, 0F4h
        db  0FCh, 0FFh, 0FFh, 0EBh, 030h, 066h, 083h, 03Dh, 0DEh, 0FFh, 0FFh, 0FFh, 000h, 074h, 007h, 0BEh
        db  077h, 0FDh, 0FFh, 0FFh, 0EBh, 01Fh, 066h, 0FFh, 005h, 0DEh, 0FFh, 0FFh, 0FFh, 060h, 0B8h, 05Eh
        db  000h, 001h, 000h, 0BEh, 04Ah, 0EEh, 0FFh, 0FFh, 0CDh, 020h, 090h, 000h, 001h, 000h, 073h, 00Bh
        db  0BEh, 0F4h, 0FCh, 0FFh, 0FFh, 0CDh, 020h, 0BEh, 000h, 001h, 000h, 089h, 035h, 0F0h, 0FFh, 0FFh
        db  0FFh, 066h, 081h, 00Dh, 0DCh, 0FFh, 0FFh, 0FFh, 000h, 008h, 061h, 09Ch, 066h, 0F7h, 005h, 0DCh
        db  0FFh, 0FFh, 0FFh, 000h, 080h, 075h, 010h, 0BEh, 04Ah, 0FFh, 0FFh, 0FFh, 0E8h, 0DDh, 0FAh, 0FFh
        db  0FFh, 0CDh, 020h, 0BEh, 000h, 001h, 000h, 066h, 083h, 03Dh, 0DEh, 0FFh, 0FFh, 0FFh, 000h, 074h
        db  010h, 0BEh, 092h, 0FFh, 0FFh, 0FFh, 0E8h, 0C3h, 0FAh, 0FFh, 0FFh, 0CDh, 020h, 0BEh, 000h, 001h
        db  000h, 09Dh, 0FFh, 015h, 0E4h, 0FFh, 0FFh, 0FFh, 09Ch, 09Ch, 083h, 0F8h, 01Ah, 076h, 009h, 09Dh
        db  0FFh, 025h, 0E4h, 0FFh, 0FFh, 0FFh, 090h, 090h, 066h, 0F7h, 005h, 0DCh, 0FFh, 0FFh, 0FFh, 000h
        db  080h, 075h, 007h, 0BEh, 04Ah, 0FFh, 0FFh, 0FFh, 0EBh, 00Fh, 066h, 083h, 03Dh, 0DEh, 0FFh, 0FFh
        db  0FFh, 000h, 074h, 00Bh, 0BEh, 092h, 0FFh, 0FFh, 0FFh, 0CDh, 020h, 0BEh, 000h, 001h, 000h, 0CDh
        db  020h, 000h, 000h, 004h, 000h, 066h, 03Dh, 00Ah, 003h, 074h, 01Ch, 0B8h, 0FCh, 030h, 000h, 000h
        db  0CDh, 020h, 046h, 001h, 001h, 000h, 0E3h, 031h, 066h, 081h, 079h, 004h, 000h, 004h, 073h, 029h
        db  08Bh, 041h, 018h, 081h, 038h, 083h, 0F8h, 001h, 075h, 075h, 01Eh, 066h, 081h, 078h, 004h, 005h
        db  0E9h, 075h, 016h, 08Bh, 078h, 006h, 08Dh, 07Ch, 007h, 030h, 0BEh, 08Eh, 025h, 000h, 000h, 06Ah
        db  00Ch, 059h, 0F3h, 0A6h, 075h, 003h, 0FEh, 04Fh, 0FEh, 0C3h, 033h, 0C0h, 0B0h, 01Bh, 0CDh, 020h
        db  046h, 001h, 001h, 000h, 0E3h, 00Eh, 08Bh, 041h, 018h, 081h, 038h, 083h, 0F8h, 000h, 075h, 075h
        db  003h, 0FEh, 040h, 002h, 0C3h, 000h, 000h, 000h, 000h, 00Fh, 0B7h, 045h, 014h, 083h, 0F8h, 00Bh
        db  077h, 007h, 0FFh, 024h, 085h, 0F4h, 025h, 000h, 000h, 0EBh, 011h, 0FEh, 005h, 0F8h, 025h, 000h
        db  000h, 075h, 000h, 033h, 0C9h, 0CDh, 020h, 01Fh, 000h, 001h, 000h, 0C3h, 0E8h, 0EAh, 0FFh, 0FFh
        db  0FFh, 057h, 0CDh, 020h, 008h, 001h, 001h, 000h, 08Bh, 07Fh, 010h, 08Bh, 07Fh, 02Ch, 0F7h, 0C7h
        db  000h, 002h, 000h, 000h, 05Fh, 075h, 023h, 060h, 0CDh, 020h, 045h, 001h, 001h, 000h, 0CDh, 020h
        db  009h, 001h, 001h, 000h, 061h, 074h, 013h, 0CDh, 020h, 01Ah, 000h, 001h, 000h, 0CDh, 020h, 01Fh
        db  000h, 001h, 000h, 0CDh, 020h, 01Bh, 000h, 001h, 000h, 0C3h, 0CDh, 020h, 01Fh, 080h, 001h, 000h
        db  0E8h, 01Ch, 000h, 000h, 000h, 0D0h, 02Dh, 09Ah, 026h, 000h, 000h, 073h, 013h, 0CDh, 020h, 029h
        db  000h, 001h, 000h, 00Fh, 0B7h, 0C9h, 02Bh, 0CAh, 07Eh, 006h, 0CDh, 020h, 023h, 000h, 001h, 000h
        db  0C3h, 05Eh, 0D0h, 025h, 09Ah, 026h, 000h, 000h, 0CDh, 020h, 029h, 000h, 001h, 000h, 00Fh, 0B7h
        db  0D1h, 033h, 0C0h, 0CDh, 020h, 00Dh, 000h, 001h, 000h, 0F9h, 0C3h, 000h, 08Bh, 045h, 01Ch, 066h
        db  03Dh, 006h, 012h, 075h, 018h, 080h, 00Dh, 09Ah, 026h, 000h, 000h, 001h, 0B9h, 0BEh, 026h, 000h
        db  000h, 00Fh, 0B7h, 0D1h, 0C1h, 0E9h, 010h, 0CDh, 020h, 048h, 000h, 001h, 000h, 0F9h, 0C3h, 080h
        db  025h, 09Ah, 026h, 000h, 000h, 0FEh, 0CDh, 020h, 04Ah, 000h, 001h, 000h, 0C3h, 0B8h, 01Ah, 000h
        db  000h, 000h, 0CDh, 020h, 046h, 001h, 001h, 000h, 085h, 0C9h, 00Fh, 084h, 011h, 001h, 000h, 000h
        db  083h, 079h, 034h, 000h, 00Fh, 085h, 007h, 001h, 000h, 000h, 08Bh, 071h, 01Ch, 085h, 0F6h, 00Fh
        db  084h, 0FCh, 000h, 000h, 000h, 0BFh, 0F8h, 025h, 000h, 000h, 08Bh, 006h, 03Bh, 007h, 00Fh, 085h
        db  0EFh, 000h, 000h, 000h, 083h, 0C6h, 009h, 083h, 0C7h, 009h, 08Bh, 006h, 08Bh, 00Fh, 025h, 0FFh
        db  0FFh, 0FFh, 000h, 081h, 0E1h, 0FFh, 0FFh, 0FFh, 000h, 03Bh, 0C1h, 00Fh, 085h, 0D2h, 000h, 000h
        db  000h, 08Bh, 076h, 003h, 08Bh, 076h, 01Ch, 08Ah, 006h, 03Ah, 005h, 008h, 026h, 000h, 000h, 00Fh
        db  085h, 0BEh, 000h, 000h, 000h, 00Fh, 0B6h, 046h, 001h, 08Dh, 074h, 030h, 002h, 08Ah, 006h, 03Ah
        db  005h, 01Bh, 026h, 000h, 000h, 00Fh, 085h, 0A8h, 000h, 000h, 000h, 08Bh, 046h, 001h, 08Dh, 074h
        db  030h, 005h, 0BFh, 00Ah, 026h, 000h, 000h, 08Bh, 046h, 008h, 03Bh, 047h, 008h, 00Fh, 085h, 090h
        db  000h, 000h, 000h, 083h, 0C6h, 00Ah, 066h, 0C7h, 006h, 090h, 0E8h, 06Ah, 000h, 068h, 0ACh, 000h
        db  000h, 000h, 0E8h, 0B6h, 03Eh, 0FBh, 0FFh, 059h, 059h, 085h, 0C0h, 074h, 078h, 08Dh, 088h, 07Ah
        db  000h, 000h, 000h, 089h, 00Dh, 066h, 026h, 000h, 000h, 089h, 00Dh, 083h, 026h, 000h, 000h, 089h
        db  00Dh, 0A6h, 026h, 000h, 000h, 089h, 00Dh, 0C0h, 026h, 000h, 000h, 056h, 050h, 08Dh, 0B0h, 09Eh
        db  000h, 000h, 000h, 0CDh, 020h, 00Bh, 000h, 001h, 000h, 0A3h, 0ACh, 026h, 000h, 000h, 0BEh, 020h
        db  026h, 000h, 000h, 058h, 08Bh, 0F8h, 0B9h, 0ACh, 000h, 000h, 000h, 0FCh, 0F3h, 0A4h, 05Eh, 08Bh
        db  0F8h, 08Dh, 04Eh, 006h, 02Bh, 0C1h, 089h, 046h, 002h, 083h, 03Dh, 0ACh, 026h, 000h, 000h, 000h
        db  074h, 01Fh, 08Dh, 077h, 03Fh, 0B8h, 021h, 000h, 000h, 000h, 0CDh, 020h, 041h, 000h, 001h, 000h
        db  08Dh, 0B7h, 07Bh, 000h, 000h, 000h, 0B8h, 02Fh, 000h, 000h, 000h, 0CDh, 020h, 041h, 000h, 001h
        db  000h, 0F8h, 0C3h, 0EBh, 0FCh, 0CDh, 020h, 0BFh, 080h, 001h, 000h, 066h, 0C7h, 045h, 0ECh, 0FFh
        db  000h, 051h, 068h, 000h, 002h, 000h, 000h, 0CDh, 020h, 0A9h, 000h, 001h, 000h, 083h, 0C4h, 004h
        db  059h, 051h, 033h, 0C9h, 051h, 0FEh, 0C9h, 089h, 04Dh, 0ECh, 041h, 0FEh, 0C5h, 051h, 0CDh, 020h
        db  0A9h, 000h, 001h, 000h, 059h, 059h, 059h, 0B8h, 0B6h, 031h, 000h, 000h, 0CDh, 020h, 046h, 001h
        db  001h, 000h, 085h, 0C9h, 074h, 03Fh, 066h, 083h, 079h, 008h, 001h, 075h, 038h, 08Bh, 049h, 018h
        db  081h, 039h, 083h, 0F8h, 001h, 075h, 075h, 02Dh, 083h, 0C1h, 006h, 066h, 081h, 079h, 0FEh, 005h
        db  0E9h, 075h, 022h, 08Bh, 001h, 08Dh, 0BCh, 001h, 00Eh, 003h, 000h, 000h, 057h, 0BEh, 0FAh, 027h
        db  000h, 000h, 033h, 0C9h, 0B1h, 016h, 0FCh, 0F3h, 0A6h, 05Fh, 075h, 009h, 0BEh, 010h, 028h, 000h
        db  000h, 0B1h, 016h, 0F3h, 0A4h, 0C3h, 033h, 0C0h, 0B0h, 00Eh, 0CDh, 020h, 046h, 001h, 001h, 000h
        db  0E3h, 035h, 066h, 081h, 079h, 004h, 000h, 004h, 073h, 02Dh, 033h, 0C0h, 0BFh, 04Ch, 08Ah, 000h
        db  000h, 0CDh, 020h, 046h, 001h, 001h, 000h, 0E3h, 01Eh, 081h, 079h, 014h, 0FFh, 0FFh, 0FFh, 02Fh
        db  075h, 015h, 08Bh, 0F9h, 0CDh, 020h, 0F8h, 000h, 001h, 000h, 0C7h, 047h, 014h, 000h, 000h, 000h
        db  080h, 0CDh, 020h, 0F7h, 000h, 001h, 000h, 0C3h, 056h, 049h, 04Eh, 054h, 031h, 034h, 020h, 020h
        db  000h, 000h, 000h, 000h, 000h, 000h, 0E8h, 00Fh, 000h, 000h, 000h, 0E9h, 0F6h, 0FFh, 0FFh, 0FFh
        db  0E8h, 005h, 000h, 000h, 000h, 0E9h, 0F6h, 0FFh, 0FFh, 0FFh, 0C3h, 08Ah, 04Bh, 005h, 080h, 0E1h
        db  01Fh, 080h, 0F9h, 00Eh, 075h, 01Bh, 066h, 083h, 07Bh, 002h, 028h, 075h, 014h, 08Bh, 0CFh, 066h
        db  087h, 00Bh, 0C1h, 0C9h, 010h, 066h, 087h, 04Bh, 006h, 0C1h, 0C9h, 010h, 02Bh, 0CFh, 001h, 04Fh
        db  006h, 0C3h, 033h, 0C0h, 0BFh, 0B7h, 028h, 000h, 000h, 0CDh, 020h, 046h, 001h, 001h, 000h, 0E3h
        db  052h, 066h, 081h, 079h, 004h, 000h, 004h, 073h, 04Ah, 06Ah, 000h, 06Ah, 015h, 0E8h, 00Bh, 03Dh
        db  0FBh, 0FFh, 059h, 059h, 085h, 0C0h, 074h, 03Bh, 08Bh, 0F8h, 0BEh, 0C5h, 028h, 000h, 000h, 0B9h
        db  015h, 000h, 000h, 000h, 0FCh, 0F3h, 0A4h, 08Bh, 0F8h, 00Fh, 001h, 00Dh, 0BFh, 028h, 000h, 000h
        db  08Bh, 01Dh, 0C1h, 028h, 000h, 000h, 081h, 0C3h, 098h, 002h, 000h, 000h, 0E8h, 08Ah, 0FFh, 0FFh
        db  0FFh, 083h, 0C7h, 00Ah, 083h, 0C3h, 008h, 0E8h, 07Fh, 0FFh, 0FFh, 0FFh, 00Fh, 001h, 01Dh, 0BFh
        db  028h, 000h, 000h, 0C3h, 08Bh, 045h, 01Ch, 083h, 0F8h, 004h, 073h, 00Ah, 0C1h, 0E0h, 002h, 02Eh
        db  0FFh, 0A0h, 00Fh, 0B7h, 045h, 01Ch, 083h, 0F8h, 004h, 073h, 009h, 0C1h, 0E0h, 002h, 033h, 0C0h
        db  0B0h, 00Dh, 0CDh, 020h, 046h, 001h, 001h, 000h, 0E3h, 042h, 066h, 081h, 079h, 004h, 000h, 004h
        db  073h, 03Ah, 08Bh, 079h, 020h, 085h, 0FFh, 074h, 033h, 06Ah, 000h, 06Ah, 000h, 06Ah, 000h, 06Ah
        db  00Eh, 057h, 0CDh, 020h, 073h, 001h, 001h, 000h, 083h, 0C4h, 014h, 085h, 0C0h, 074h, 01Dh, 0BEh
        db  063h, 029h, 000h, 000h, 08Ah, 047h, 005h, 088h, 046h, 005h, 057h, 06Ah, 00Eh, 059h, 0FCh, 0F3h
        db  0A6h, 05Fh, 075h, 008h, 088h, 046h, 006h, 06Ah, 00Ch, 059h, 0F3h, 0A4h, 0C3h, 060h, 0BFh, 048h
        db  087h, 000h, 000h, 0E8h, 0C6h, 0F8h, 0FFh, 0FFh, 0BFh, 088h, 087h, 000h, 000h, 0E8h, 0BCh, 0F8h
        db  0FFh, 0FFh, 0BFh, 0E8h, 087h, 000h, 000h, 0E8h, 0B2h, 0F8h, 0FFh, 0FFh, 0BFh, 028h, 088h, 000h
        db  000h, 0E8h, 0A8h, 0F8h, 0FFh, 0FFh, 0BFh, 0ECh, 088h, 000h, 000h, 0E8h, 09Eh, 0F8h, 0FFh, 0FFh
        db  0BFh, 03Ch, 089h, 000h, 000h, 0E8h, 094h, 0F8h, 0FFh, 0FFh, 0BFh, 0D8h, 089h, 000h, 000h, 0E8h
        db  08Ah, 0F8h, 0FFh, 0FFh, 0E8h, 082h, 0FBh, 0FFh, 0FFh, 0E8h, 0BCh, 0FBh, 0FFh, 0FFh, 0E8h, 0AAh
        db  0FCh, 0FFh, 0FFh, 0E8h, 0FFh, 0FDh, 0FFh, 0FFh, 0E8h, 049h, 0FEh, 0FFh, 0FFh, 0E8h, 0D0h, 0FEh
        db  0FFh, 0FFh, 0E8h, 047h, 0FFh, 0FFh, 0FFh, 061h, 0C3h, 00Fh, 020h, 0D8h, 0EBh, 000h, 0A3h, 0CCh
        db  0FCh, 000h, 000h, 00Dh, 003h, 002h, 000h, 000h, 08Bh, 054h, 024h, 004h, 089h, 002h, 0C2h, 004h
        db  000h, 068h, 030h, 0EDh, 000h, 000h, 068h, 080h, 0D9h, 000h, 000h, 0CDh, 020h, 029h, 001h, 001h
        db  000h, 083h, 0C4h, 008h, 0C3h, 080h, 03Dh, 005h, 0BFh, 000h, 000h, 000h, 074h, 024h, 0F7h, 005h
        db  073h, 02Ah, 000h, 000h, 000h, 000h, 040h, 000h, 074h, 018h, 053h, 056h, 057h, 066h, 0B8h, 070h
        db  000h, 0B3h, 04Dh, 0BEh, 01Fh, 013h, 000h, 000h, 0BFh, 050h, 066h, 000h, 000h, 0CDh, 041h, 05Fh
        db  05Eh, 05Bh, 0C3h, 0B9h, 020h, 000h, 000h, 000h, 0CDh, 020h, 025h, 000h, 001h, 000h, 072h, 006h
        db  0A3h, 0D8h, 0FCh, 000h, 000h, 0C3h, 033h, 0C0h, 0EBh, 0FBh, 08Bh, 044h, 024h, 004h, 0CDh, 020h
        db  046h, 001h, 001h, 000h, 033h, 0C0h, 0E3h, 009h, 066h, 081h, 079h, 004h, 000h, 004h, 073h, 001h
        db  091h, 0C2h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 056h, 057h, 0E8h, 08Dh, 0FFh, 0FFh, 0FFh, 08Bh, 074h, 024h, 00Ch, 056h, 08Bh, 046h, 01Ch
        db  0C1h, 0E8h, 00Ch, 0A3h, 074h, 0F8h, 000h, 000h, 00Fh, 0B7h, 046h, 020h, 0A3h, 058h, 0F9h, 000h
        db  000h, 0E8h, 007h, 008h, 000h, 000h, 08Bh, 0F8h, 00Bh, 0FFh, 00Fh, 084h, 03Ch, 003h, 000h, 000h
        db  08Bh, 046h, 022h, 005h, 0F8h, 00Fh, 000h, 000h, 050h, 0E8h, 02Bh, 0FFh, 0FFh, 0FFh, 08Bh, 046h
        db  022h, 025h, 0FFh, 0F3h, 0FFh, 0FFh, 0C1h, 0E8h, 00Ah, 0C7h, 080h, 000h, 000h, 080h, 0FFh, 000h
        db  0F0h, 0FFh, 0FFh, 08Bh, 046h, 026h, 025h, 0FFh, 0F3h, 0FFh, 0FFh, 0C1h, 0E8h, 00Ah, 06Ah, 001h
        db  0C7h, 080h, 000h, 000h, 080h, 0FFh, 000h, 0F0h, 0FFh, 0FFh, 08Bh, 046h, 02Ah, 025h, 0FFh, 0F3h
        db  0FFh, 0FFh, 0C1h, 0E8h, 00Ah, 06Ah, 000h, 0C7h, 080h, 000h, 000h, 080h, 0FFh, 000h, 0F0h, 0FFh
        db  0FFh, 068h, 008h, 0F9h, 000h, 000h, 0E8h, 0E4h, 0E0h, 0FAh, 0FFh, 06Ah, 001h, 06Ah, 000h, 068h
        db  040h, 0F8h, 000h, 000h, 0E8h, 0D6h, 0E0h, 0FAh, 0FFh, 06Ah, 001h, 06Ah, 000h, 068h, 0CCh, 0F8h
        db  000h, 000h, 0E8h, 0C8h, 0E0h, 0FAh, 0FFh, 0C7h, 005h, 0C8h, 0F7h, 000h, 000h, 008h, 0F9h, 000h
        db  000h, 0C7h, 005h, 09Ch, 0FDh, 000h, 000h, 040h, 0F8h, 000h, 000h, 0C7h, 005h, 03Ch, 0F9h, 000h
        db  000h, 0CCh, 0F8h, 000h, 000h, 0C7h, 005h, 028h, 0FDh, 000h, 000h, 04Ch, 07Bh, 000h, 000h, 0C7h
        db  005h, 070h, 0F8h, 000h, 000h, 058h, 07Bh, 000h, 000h, 0C7h, 005h, 0F0h, 0F8h, 000h, 000h, 0F4h
        db  079h, 000h, 000h, 0C7h, 005h, 0ECh, 0F7h, 000h, 000h, 0B4h, 07Ah, 000h, 000h, 056h, 0E8h, 0E7h
        db  008h, 000h, 000h, 00Bh, 0C0h, 00Fh, 084h, 071h, 002h, 000h, 000h, 0C7h, 005h, 008h, 0FCh, 000h
        db  000h, 0ADh, 0FBh, 0FFh, 0FFh, 0C7h, 005h, 00Ch, 0FCh, 000h, 000h, 098h, 0EEh, 000h, 000h, 0C7h
        db  005h, 010h, 0FCh, 000h, 000h, 0B8h, 0EEh, 000h, 000h, 0C7h, 005h, 018h, 0FCh, 000h, 000h, 0D8h
        db  0EEh, 000h, 000h, 0C7h, 005h, 014h, 0FCh, 000h, 000h, 0F8h, 0EEh, 000h, 000h, 056h, 0E8h, 0C4h
        db  00Ah, 000h, 000h, 00Fh, 0B7h, 046h, 014h, 083h, 0E8h, 002h, 0C1h, 0E0h, 00Ch, 003h, 046h, 010h
        db  0A3h, 0F0h, 0F7h, 000h, 000h, 025h, 0FFh, 0F3h, 0FFh, 0FFh, 0C1h, 0E8h, 00Ah, 02Dh, 000h, 000h
        db  080h, 000h, 0A3h, 0F0h, 0FCh, 000h, 000h, 056h, 057h, 0E8h, 0A0h, 00Ch, 000h, 000h, 00Bh, 0C0h
        db  00Fh, 084h, 006h, 002h, 000h, 000h, 08Bh, 046h, 016h, 0C1h, 0E8h, 00Ch, 08Dh, 00Ch, 085h, 000h
        db  000h, 080h, 0FFh, 0A3h, 018h, 0EBh, 000h, 000h, 0F6h, 041h, 001h, 002h, 074h, 014h, 0A1h, 01Ch
        db  0EBh, 000h, 000h, 083h, 0C1h, 004h, 040h, 0A3h, 01Ch, 0EBh, 000h, 000h, 0F6h, 041h, 001h, 002h
        db  075h, 0F1h, 08Bh, 046h, 010h, 0C1h, 0E8h, 00Ch, 040h, 0A3h, 014h, 0FDh, 000h, 000h, 08Dh, 00Ch
        db  085h, 000h, 000h, 080h, 0FFh, 0A3h, 004h, 0F9h, 000h, 000h, 0F6h, 041h, 001h, 002h, 074h, 00Fh
        db  083h, 0C1h, 004h, 040h, 0A3h, 004h, 0F9h, 000h, 000h, 0F6h, 041h, 001h, 002h, 075h, 0F1h, 0E8h
        db  071h, 00Bh, 000h, 000h, 00Bh, 0C0h, 00Fh, 084h, 0A0h, 001h, 000h, 000h, 068h, 000h, 000h, 004h
        db  000h, 068h, 000h, 000h, 008h, 000h, 068h, 08Ch, 0F8h, 000h, 000h, 0E8h, 09Fh, 0D9h, 0FAh, 0FFh
        db  00Bh, 0C0h, 00Fh, 084h, 084h, 001h, 000h, 000h, 06Ah, 010h, 06Ah, 001h, 068h, 000h, 000h, 008h
        db  000h, 0E8h, 01Bh, 0D5h, 0FAh, 0FFh, 083h, 0C4h, 00Ch, 083h, 0F8h, 0FFh, 00Fh, 084h, 06Ah, 001h
        db  000h, 000h, 0FFh, 015h, 0B4h, 0D7h, 000h, 000h, 0A3h, 03Ch, 0F8h, 000h, 000h, 00Bh, 0C0h, 00Fh
        db  084h, 057h, 001h, 000h, 000h, 089h, 040h, 00Ch, 0A3h, 054h, 0F9h, 000h, 000h, 06Ah, 018h, 06Ah
        db  001h, 068h, 000h, 000h, 008h, 080h, 0FFh, 015h, 09Ch, 0D7h, 000h, 000h, 083h, 0C4h, 00Ch, 0A3h
        db  04Ch, 0F9h, 000h, 000h, 083h, 0F8h, 0FFh, 00Fh, 084h, 02Fh, 001h, 000h, 000h, 068h, 008h, 000h
        db  006h, 020h, 06Ah, 000h, 06Ah, 004h, 06Ah, 001h, 0C1h, 0E8h, 00Ch, 050h, 0FFh, 015h, 0A0h, 0D7h
        db  000h, 000h, 083h, 0C4h, 014h, 00Bh, 0C0h, 00Fh, 084h, 00Fh, 001h, 000h, 000h, 0A1h, 04Ch, 0F9h
        db  000h, 000h, 025h, 0FFh, 0F3h, 0FFh, 0FFh, 0C1h, 0E8h, 00Ah, 08Bh, 080h, 000h, 000h, 080h, 0FFh
        db  0C1h, 0E8h, 00Ch, 0A3h, 0D8h, 0F7h, 000h, 000h, 0E8h, 0D2h, 010h, 000h, 000h, 00Bh, 0C0h, 00Fh
        db  084h, 0E7h, 000h, 000h, 000h, 06Ah, 018h, 06Ah, 02Dh, 068h, 000h, 000h, 008h, 080h, 0FFh, 015h
        db  09Ch, 0D7h, 000h, 000h, 083h, 0C4h, 00Ch, 0A3h, 0F0h, 0F7h, 000h, 000h, 083h, 0F8h, 0FFh, 00Fh
        db  084h, 0C7h, 000h, 000h, 000h, 08Bh, 0D0h, 081h, 0E2h, 0FFh, 0F3h, 0FFh, 0FFh, 0C1h, 0EAh, 00Ah
        db  081h, 0EAh, 000h, 000h, 080h, 000h, 089h, 015h, 0F0h, 0FCh, 000h, 000h, 005h, 000h, 000h, 002h
        db  000h, 0A3h, 0F4h, 0FCh, 000h, 000h, 08Dh, 08Ah, 080h, 000h, 000h, 000h, 089h, 00Dh, 0FCh, 0FAh
        db  000h, 000h, 083h, 0C1h, 004h, 089h, 00Dh, 02Ch, 0F9h, 000h, 000h, 005h, 000h, 010h, 000h, 000h
        db  0A3h, 034h, 0F9h, 000h, 000h, 083h, 0C1h, 008h, 089h, 00Dh, 0F4h, 0F7h, 000h, 000h, 005h, 000h
        db  020h, 000h, 000h, 0A3h, 0CCh, 0F7h, 000h, 000h, 083h, 0C1h, 008h, 089h, 00Dh, 0D4h, 0FCh, 000h
        db  000h, 005h, 000h, 020h, 000h, 000h, 0A3h, 068h, 0F8h, 000h, 000h, 081h, 009h, 000h, 00Ah, 000h
        db  000h, 06Ah, 000h, 08Bh, 0C2h, 025h, 000h, 0C0h, 03Fh, 000h, 0C1h, 0E8h, 00Ch, 005h, 02Ch, 004h
        db  000h, 000h, 0C1h, 0E8h, 00Ah, 050h, 0C1h, 0EAh, 00Ch, 052h, 0FFh, 015h, 0B4h, 0D4h, 000h, 000h
        db  083h, 0C4h, 00Ch, 00Bh, 0C0h, 074h, 035h, 0C7h, 005h, 08Ch, 0EEh, 000h, 000h, 001h, 000h, 000h
        db  000h, 0A1h, 03Ch, 0F8h, 000h, 000h, 0A3h, 060h, 0F8h, 000h, 000h, 06Ah, 008h, 06Ah, 010h, 068h
        db  000h, 000h, 008h, 080h, 0FFh, 015h, 09Ch, 0D7h, 000h, 000h, 083h, 0C4h, 00Ch, 0A3h, 0A8h, 0EFh
        db  000h, 000h, 083h, 0F8h, 0FFh, 0B8h, 001h, 000h, 000h, 000h, 075h, 002h, 02Bh, 0C0h, 05Fh, 05Eh
        db  0C3h, 083h, 0ECh, 014h, 056h, 0A1h, 0C4h, 0F8h, 000h, 000h, 0A3h, 0E0h, 0F7h, 000h, 000h, 08Dh
        db  044h, 024h, 008h, 050h, 068h, 078h, 0E6h, 000h, 000h, 068h, 002h, 000h, 000h, 080h, 0FFh, 015h
        db  050h, 0D8h, 000h, 000h, 083h, 0C4h, 00Ch, 00Bh, 0C0h, 075h, 071h, 0C7h, 044h, 024h, 00Ch, 001h
        db  000h, 000h, 000h, 08Dh, 044h, 024h, 00Ch, 050h, 08Dh, 044h, 024h, 00Bh, 050h, 06Ah, 000h, 06Ah
        db  000h, 068h, 0A4h, 0E6h, 000h, 000h, 0FFh, 074h, 024h, 01Ch, 0FFh, 015h, 06Ch, 0D8h, 000h, 000h
        db  083h, 0C4h, 018h, 00Bh, 0C0h, 075h, 011h, 080h, 07Ch, 024h, 007h, 000h, 074h, 00Ah, 0A1h, 0E8h
        db  0F7h, 000h, 000h, 0A3h, 0E0h, 0F7h, 000h, 000h, 08Bh, 044h, 024h, 01Ch, 068h, 080h, 001h, 000h
        db  000h, 005h, 054h, 002h, 000h, 000h, 050h, 06Ah, 003h, 06Ah, 000h, 068h, 0B0h, 0E6h, 000h, 000h
        db  0FFh, 074h, 024h, 01Ch, 0FFh, 015h, 070h, 0D8h, 000h, 000h, 083h, 0C4h, 018h, 0FFh, 074h, 024h
        db  008h, 0FFh, 015h, 04Ch, 0D8h, 000h, 000h, 083h, 0C4h, 004h, 0EBh, 010h, 066h, 0A1h, 030h, 0F9h
        db  000h, 000h, 066h, 00Dh, 020h, 000h, 066h, 0A3h, 030h, 0F9h, 000h, 000h, 0BEh, 000h, 010h, 000h
        db  000h, 081h, 03Dh, 020h, 0EFh, 000h, 000h, 0C0h, 00Fh, 000h, 000h, 072h, 064h, 08Dh, 044h, 024h
        db  008h, 050h, 068h, 0BCh, 0E6h, 000h, 000h, 068h, 002h, 000h, 000h, 080h, 0FFh, 015h, 048h, 0D8h
        db  000h, 000h, 083h, 0C4h, 00Ch, 00Bh, 0C0h, 075h, 048h, 0C7h, 044h, 024h, 00Ch, 007h, 000h, 000h
        db  000h, 08Dh, 044h, 024h, 00Ch, 050h, 08Dh, 044h, 024h, 014h, 050h, 06Ah, 000h, 06Ah, 000h, 068h
        db  0D4h, 0E6h, 000h, 000h, 0FFh, 074h, 024h, 01Ch, 0FFh, 015h, 06Ch, 0D8h, 000h, 000h, 083h, 0C4h
        db  018h, 00Bh, 0C0h, 075h, 00Fh, 081h, 07Ch, 024h, 010h, 044h, 065h, 06Ch, 06Ch, 075h, 005h, 0BEh
        db  0C0h, 00Fh, 000h, 000h, 0FFh, 074h, 024h, 008h, 0FFh, 015h, 04Ch, 0D8h, 000h, 000h, 083h, 0C4h
        db  004h, 056h, 068h, 062h, 085h, 000h, 000h, 06Ah, 000h, 0E8h, 0F2h, 035h, 000h, 000h, 083h, 0C4h
        db  00Ch, 03Bh, 005h, 020h, 0EFh, 000h, 000h, 077h, 02Ah, 03Dh, 000h, 010h, 000h, 000h, 073h, 023h
        db  08Dh, 070h, 001h, 056h, 050h, 06Ah, 000h, 068h, 000h, 000h, 000h, 010h, 06Ah, 001h, 06Ah, 000h
        db  0FFh, 015h, 080h, 0D8h, 000h, 000h, 083h, 0C4h, 018h, 08Bh, 0C6h, 03Bh, 035h, 020h, 0EFh, 000h
        db  000h, 076h, 0D6h, 068h, 007h, 002h, 000h, 000h, 0E8h, 0FDh, 0FAh, 0FFh, 0FFh, 00Bh, 0C0h, 074h
        db  030h, 06Ah, 001h, 068h, 000h, 004h, 000h, 000h, 068h, 000h, 000h, 008h, 080h, 0FFh, 015h, 09Ch
        db  0D7h, 000h, 000h, 083h, 0C4h, 00Ch, 0A3h, 0A8h, 0F8h, 000h, 000h, 083h, 0F8h, 0FFh, 074h, 00Dh
        db  0C1h, 0E8h, 00Ch, 050h, 0E8h, 0D8h, 0D4h, 0FAh, 0FFh, 00Bh, 0C0h, 075h, 004h, 02Bh, 0C0h, 0EBh
        db  005h, 0B8h, 001h, 000h, 000h, 000h, 05Eh, 083h, 0C4h, 014h, 0C3h, 083h, 0ECh, 00Ch, 053h, 056h
        db  057h, 055h, 0E8h, 05Ah, 0FAh, 0FFh, 0FFh, 0F6h, 044h, 024h, 020h, 002h, 00Fh, 084h, 091h, 001h
        db  000h, 000h, 06Ah, 008h, 06Ah, 010h, 068h, 000h, 000h, 008h, 080h, 0FFh, 015h, 09Ch, 0D7h, 000h
        db  000h, 083h, 0C4h, 00Ch, 0A3h, 004h, 0FCh, 000h, 000h, 083h, 0F8h, 0FFh, 00Fh, 084h, 0D1h, 002h
        db  000h, 000h, 08Bh, 035h, 028h, 0EFh, 000h, 000h, 00Bh, 0F6h, 074h, 025h, 0C1h, 0EEh, 00Fh, 046h
        db  068h, 009h, 000h, 000h, 010h, 056h, 050h, 0FFh, 015h, 078h, 0D4h, 000h, 000h, 083h, 0C4h, 00Ch
        db  00Bh, 0C0h, 00Fh, 084h, 0ABh, 002h, 000h, 000h, 0C1h, 0E6h, 00Fh, 089h, 035h, 000h, 0FBh, 000h
        db  000h, 0F6h, 044h, 024h, 020h, 008h, 00Fh, 084h, 028h, 001h, 000h, 000h, 0C7h, 005h, 008h, 0FDh
        db  000h, 000h, 0C4h, 0F6h, 000h, 000h, 0A1h, 03Ch, 0F8h, 000h, 000h, 0C7h, 005h, 09Ch, 0F8h, 000h
        db  000h, 0C4h, 0F6h, 000h, 000h, 08Bh, 040h, 008h, 0C7h, 040h, 010h, 0C4h, 0F6h, 000h, 000h, 0C7h
        db  005h, 0C8h, 0F7h, 000h, 000h, 0C4h, 0F6h, 000h, 000h, 0C7h, 005h, 09Ch, 0FDh, 000h, 000h, 0C4h
        db  0F6h, 000h, 000h, 0A1h, 0F8h, 0F7h, 000h, 000h, 0C7h, 005h, 03Ch, 0F9h, 000h, 000h, 0C4h, 0F6h
        db  000h, 000h, 0C7h, 040h, 04Ch, 0C4h, 0F6h, 000h, 000h, 0A1h, 0E8h, 0F7h, 000h, 000h, 0C7h, 040h
        db  04Ch, 0C4h, 0F6h, 000h, 000h, 0A1h, 0C4h, 0F8h, 000h, 000h, 0C7h, 040h, 04Ch, 0C4h, 0F6h, 000h
        db  000h, 0C7h, 005h, 028h, 0FDh, 000h, 000h, 0FDh, 036h, 000h, 000h, 0A1h, 028h, 0FDh, 000h, 000h
        db  0A3h, 0F0h, 0F8h, 000h, 000h, 0C7h, 005h, 070h, 0F8h, 000h, 000h, 008h, 037h, 000h, 000h, 0A1h
        db  070h, 0F8h, 000h, 000h, 0A3h, 0ECh, 0F7h, 000h, 000h, 08Bh, 035h, 058h, 0F9h, 000h, 000h, 00Bh
        db  0F6h, 08Bh, 03Dh, 074h, 0F8h, 000h, 000h, 08Dh, 01Ch, 0BDh, 000h, 000h, 080h, 0FFh, 074h, 025h
        db  0F6h, 043h, 001h, 002h, 074h, 016h, 06Ah, 000h, 06Ah, 001h, 057h, 0FFh, 015h, 0B4h, 0D4h, 000h
        db  000h, 083h, 0C4h, 00Ch, 00Bh, 0C0h, 00Fh, 084h, 0D7h, 001h, 000h, 000h, 047h, 04Eh, 083h, 0C3h
        db  004h, 00Bh, 0F6h, 075h, 0DBh, 0A1h, 0E0h, 0F7h, 000h, 000h, 03Bh, 005h, 0C4h, 0F8h, 000h, 000h
        db  00Fh, 085h, 038h, 001h, 000h, 000h, 0A1h, 0C4h, 0F8h, 000h, 000h, 08Bh, 0F8h, 08Bh, 030h, 0C1h
        db  0EEh, 00Ch, 0C1h, 0EFh, 00Ch, 08Dh, 01Ch, 0BDh, 000h, 000h, 080h, 0FFh, 00Bh, 0F6h, 074h, 025h
        db  0F6h, 043h, 001h, 002h, 074h, 016h, 06Ah, 000h, 06Ah, 001h, 057h, 0FFh, 015h, 0B4h, 0D4h, 000h
        db  000h, 083h, 0C4h, 00Ch, 00Bh, 0C0h, 00Fh, 084h, 087h, 001h, 000h, 000h, 047h, 04Eh, 083h, 0C3h
        db  004h, 00Bh, 0F6h, 075h, 0DBh, 0A1h, 0E8h, 0F7h, 000h, 000h, 0A3h, 0E0h, 0F7h, 000h, 000h, 0E9h
        db  0EAh, 000h, 000h, 000h, 0A1h, 0C4h, 0F8h, 000h, 000h, 0A3h, 0E0h, 0F7h, 000h, 000h, 0E9h, 0DBh
        db  000h, 000h, 000h, 02Bh, 0F6h, 089h, 035h, 028h, 0EFh, 000h, 000h, 0A1h, 0E8h, 0F7h, 000h, 000h
        db  0A3h, 0E0h, 0F7h, 000h, 000h, 0BFh, 000h, 000h, 080h, 0FFh, 0C7h, 044h, 024h, 014h, 000h, 0E0h
        db  0BFh, 0FFh, 08Bh, 044h, 024h, 014h, 0F6h, 040h, 001h, 002h, 074h, 073h, 08Dh, 09Fh, 000h, 010h
        db  000h, 000h, 03Bh, 0DFh, 076h, 06Fh, 08Bh, 007h, 08Bh, 0C8h, 080h, 0E5h, 00Ah, 080h, 0FDh, 002h
        db  075h, 054h, 0A8h, 001h, 074h, 010h, 0C1h, 0E8h, 00Ch, 06Bh, 0C0h, 00Dh, 003h, 005h, 0E4h, 0F7h
        db  000h, 000h, 08Bh, 028h, 0EBh, 021h, 0C1h, 0E8h, 00Ch, 06Bh, 0C0h, 00Ah, 003h, 005h, 0ACh, 0F8h
        db  000h, 000h, 08Bh, 0E8h, 0F6h, 045h, 003h, 010h, 074h, 02Ch, 055h, 057h, 06Ah, 000h, 0E8h, 000h
        db  016h, 0FBh, 0FFh, 00Bh, 0C0h, 074h, 01Fh, 00Fh, 0B6h, 04Dh, 002h, 08Bh, 00Ch, 08Dh, 008h, 0FCh
        db  000h, 000h, 0F6h, 041h, 01Ch, 001h, 075h, 00Eh, 066h, 083h, 078h, 008h, 000h, 074h, 001h, 046h
        db  066h, 0C7h, 040h, 008h, 088h, 0F1h, 083h, 0C7h, 004h, 03Bh, 0DFh, 077h, 099h, 0EBh, 006h, 081h
        db  0C7h, 000h, 010h, 000h, 000h, 083h, 044h, 024h, 014h, 004h, 081h, 07Ch, 024h, 014h, 000h, 0F0h
        db  0BFh, 0FFh, 00Fh, 082h, 06Ah, 0FFh, 0FFh, 0FFh, 0A1h, 084h, 0EEh, 000h, 000h, 02Bh, 0C6h, 001h
        db  005h, 01Ch, 0EFh, 000h, 000h, 0A1h, 018h, 0EFh, 000h, 000h, 083h, 0E8h, 024h, 0A3h, 024h, 0EFh
        db  000h, 000h, 039h, 005h, 01Ch, 0EFh, 000h, 000h, 00Fh, 08Fh, 085h, 000h, 000h, 000h, 0F6h, 005h
        db  030h, 0F9h, 000h, 000h, 020h, 075h, 05Bh, 08Dh, 044h, 024h, 018h, 050h, 068h, 0E4h, 0E6h, 000h
        db  000h, 068h, 002h, 000h, 000h, 080h, 0FFh, 015h, 050h, 0D8h, 000h, 000h, 083h, 0C4h, 00Ch, 00Bh
        db  0C0h, 075h, 03Fh, 066h, 08Bh, 044h, 024h, 020h, 066h, 025h, 008h, 000h, 066h, 03Dh, 001h, 000h
        db  01Ah, 0C0h, 0FEh, 0C0h, 088h, 044h, 024h, 013h, 08Dh, 044h, 024h, 013h, 06Ah, 001h, 050h, 06Ah
        db  003h, 06Ah, 000h, 068h, 010h, 0E7h, 000h, 000h, 0FFh, 074h, 024h, 02Ch, 0FFh, 015h, 070h, 0D8h
        db  000h, 000h, 083h, 0C4h, 018h, 0FFh, 074h, 024h, 018h, 0FFh, 015h, 04Ch, 0D8h, 000h, 000h, 083h
        db  0C4h, 004h, 0E8h, 0BCh, 0F7h, 0FFh, 0FFh, 00Bh, 0C0h, 074h, 018h, 066h, 0A1h, 030h, 0F9h, 000h
        db  000h, 066h, 00Bh, 044h, 024h, 020h, 066h, 0A3h, 030h, 0F9h, 000h, 000h, 0B8h, 001h, 000h, 000h
        db  000h, 0EBh, 002h, 02Bh, 0C0h, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 00Ch, 0C3h, 081h, 0ECh, 044h
        db  006h, 000h, 000h, 053h, 056h, 057h, 055h, 08Bh, 0B4h, 024h, 058h, 006h, 000h, 000h, 02Bh, 0FFh
        db  08Bh, 04Eh, 02Ah, 00Fh, 0B7h, 056h, 014h, 00Fh, 0B7h, 046h, 01Ah, 003h, 0D0h, 00Fh, 0B7h, 046h
        db  020h, 003h, 0D0h, 08Dh, 004h, 091h, 089h, 044h, 024h, 010h, 0F6h, 001h, 001h, 075h, 00Eh, 081h
        db  009h, 000h, 0F0h, 0FFh, 0FFh, 083h, 0C1h, 004h, 0F6h, 001h, 001h, 074h, 0F2h, 08Bh, 001h, 0C1h
        db  0E8h, 00Ch, 089h, 044h, 024h, 014h, 08Bh, 0C1h, 083h, 0C1h, 004h, 039h, 04Ch, 024h, 010h, 076h
        db  056h, 08Dh, 054h, 024h, 018h, 08Bh, 019h, 0F6h, 0C3h, 001h, 074h, 03Ah, 081h, 0CBh, 000h, 002h
        db  000h, 000h, 089h, 019h, 08Bh, 000h, 0C1h, 0E8h, 00Ch, 08Bh, 0E8h, 0C1h, 0EBh, 00Ch, 02Bh, 0EBh
        db  083h, 0FDh, 0FFh, 074h, 01Dh, 089h, 002h, 08Dh, 084h, 024h, 050h, 006h, 000h, 000h, 03Bh, 0D0h
        db  00Fh, 083h, 084h, 000h, 000h, 000h, 083h, 0C2h, 008h, 047h, 08Bh, 001h, 0C1h, 0E8h, 00Ch, 089h
        db  042h, 0FCh, 08Bh, 0C1h, 0EBh, 008h, 081h, 0CBh, 000h, 0F0h, 0FFh, 0FFh, 089h, 019h, 083h, 0C1h
        db  004h, 039h, 04Ch, 024h, 010h, 077h, 0AEh, 08Bh, 000h, 0C1h, 0E8h, 00Ch, 089h, 044h, 0FCh, 018h
        db  06Ah, 0FFh, 068h, 056h, 085h, 000h, 000h, 06Ah, 000h, 0E8h, 0A2h, 031h, 000h, 000h, 083h, 0C4h
        db  00Ch, 03Dh, 080h, 003h, 000h, 000h, 073h, 005h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh, 048h, 08Bh, 056h
        db  00Ch, 08Dh, 074h, 0FCh, 014h, 08Bh, 04Ah, 004h, 00Bh, 0C9h, 075h, 005h, 083h, 03Ah, 000h, 074h
        db  030h, 03Bh, 0C8h, 076h, 007h, 039h, 002h, 077h, 01Ch, 089h, 042h, 004h, 083h, 0C6h, 008h, 047h
        db  08Dh, 08Ch, 024h, 054h, 006h, 000h, 000h, 03Bh, 0F1h, 073h, 016h, 08Bh, 00Ah, 089h, 00Eh, 08Bh
        db  04Ah, 004h, 089h, 04Eh, 004h, 083h, 0C2h, 008h, 0EBh, 0CBh, 02Bh, 0C0h, 0E9h, 08Ch, 000h, 000h
        db  000h, 08Dh, 074h, 0FCh, 014h, 02Bh, 0D2h, 08Dh, 044h, 024h, 014h, 03Bh, 0F0h, 072h, 06Fh, 0B9h
        db  06Ch, 0FAh, 000h, 000h, 02Bh, 0FFh, 08Dh, 05Ch, 024h, 014h, 08Dh, 044h, 024h, 01Ch, 08Bh, 0E8h
        db  03Bh, 0F5h, 072h, 00Fh, 08Bh, 02Bh, 039h, 028h, 073h, 002h, 08Bh, 0D8h, 083h, 0C0h, 008h, 03Bh
        db  0F0h, 073h, 0F1h, 039h, 03Dh, 06Ch, 0FAh, 000h, 000h, 075h, 002h, 0EBh, 01Eh, 08Bh, 041h, 004h
        db  02Bh, 003h, 083h, 0F8h, 0FFh, 075h, 008h, 08Bh, 043h, 004h, 089h, 041h, 004h, 0EBh, 016h, 083h
        db  0C1h, 008h, 042h, 081h, 0F9h, 0ECh, 0FAh, 000h, 000h, 073h, 023h, 08Bh, 003h, 08Bh, 06Bh, 004h
        db  089h, 001h, 089h, 069h, 004h, 03Bh, 0F3h, 074h, 00Ah, 08Bh, 006h, 08Bh, 06Eh, 004h, 089h, 003h
        db  089h, 06Bh, 004h, 083h, 0EEh, 008h, 08Dh, 044h, 024h, 014h, 03Bh, 0F0h, 073h, 098h, 08Dh, 042h
        db  001h, 0A3h, 06Ch, 0F8h, 000h, 000h, 08Dh, 004h, 0D5h, 06Ch, 0FAh, 000h, 000h, 05Dh, 05Fh, 05Eh
        db  05Bh, 081h, 0C4h, 044h, 006h, 000h, 000h, 0C2h, 004h, 000h, 083h, 0ECh, 024h, 056h, 08Bh, 044h
        db  024h, 02Ch, 08Bh, 050h, 00Ch, 083h, 07Ah, 00Ch, 000h, 074h, 017h, 02Bh, 0C0h, 08Bh, 04Ah, 004h
        db  02Bh, 00Ah, 081h, 0F9h, 080h, 000h, 000h, 000h, 077h, 008h, 083h, 0C2h, 008h, 039h, 042h, 00Ch
        db  075h, 0EBh, 0B8h, 0BCh, 08Bh, 000h, 000h, 08Bh, 00Ah, 06Bh, 0C9h, 00Dh, 02Bh, 0C1h, 0A3h, 0E4h
        db  0F7h, 000h, 000h, 005h, 000h, 00Dh, 000h, 000h, 0A3h, 0A4h, 0F8h, 000h, 000h, 08Bh, 00Ah, 08Bh
        db  042h, 004h, 02Bh, 0C1h, 048h, 03Dh, 0E8h, 003h, 000h, 000h, 072h, 005h, 0B8h, 0E8h, 003h, 000h
        db  000h, 0A3h, 018h, 0EFh, 000h, 000h, 083h, 0F8h, 030h, 07Dh, 007h, 02Bh, 0C0h, 0E9h, 0AEh, 001h
        db  000h, 000h, 0A1h, 018h, 0EFh, 000h, 000h, 08Dh, 044h, 001h, 0FFh, 0A3h, 020h, 0EFh, 000h, 000h
        db  08Bh, 0F0h, 06Bh, 0F6h, 00Dh, 003h, 035h, 0E4h, 0F7h, 000h, 000h, 089h, 035h, 0DCh, 0FCh, 000h
        db  000h, 0A1h, 018h, 0EFh, 000h, 000h, 06Bh, 0C0h, 00Dh, 005h, 0BCh, 08Bh, 000h, 000h, 0A3h, 060h
        db  0FAh, 000h, 000h, 0A1h, 020h, 0EFh, 000h, 000h, 040h, 0A3h, 0A0h, 0FDh, 000h, 000h, 0A1h, 060h
        db  0FAh, 000h, 000h, 083h, 0C0h, 008h, 089h, 044h, 024h, 008h, 08Bh, 000h, 08Bh, 015h, 0A0h, 0FDh
        db  000h, 000h, 033h, 0D0h, 081h, 0E2h, 0FFh, 0FFh, 00Fh, 000h, 033h, 0D0h, 08Bh, 044h, 024h, 008h
        db  089h, 010h, 0A1h, 060h, 0FAh, 000h, 000h, 083h, 0C0h, 009h, 0C1h, 0E2h, 00Ch, 08Bh, 008h, 033h
        db  0CAh, 081h, 0E1h, 0FFh, 00Fh, 000h, 000h, 033h, 0CAh, 089h, 008h, 081h, 0FEh, 0BCh, 08Bh, 000h
        db  000h, 072h, 029h, 0C7h, 006h, 000h, 000h, 000h, 000h, 0FFh, 005h, 040h, 0F9h, 000h, 000h, 0FFh
        db  035h, 0A0h, 0FDh, 000h, 000h, 0FFh, 035h, 060h, 0FAh, 000h, 000h, 056h, 0E8h, 0E0h, 017h, 0FBh
        db  0FFh, 083h, 0EEh, 00Dh, 081h, 0FEh, 0BCh, 08Bh, 000h, 000h, 073h, 0D7h, 0A1h, 018h, 0EFh, 000h
        db  000h, 06Bh, 0C0h, 00Dh, 08Bh, 00Dh, 020h, 0EFh, 000h, 000h, 08Dh, 090h, 0C9h, 08Bh, 000h, 000h
        db  089h, 015h, 068h, 0FAh, 000h, 000h, 083h, 0C1h, 002h, 089h, 00Dh, 0DCh, 0F7h, 000h, 000h, 08Bh
        db  042h, 008h, 033h, 0C8h, 081h, 0E1h, 0FFh, 0FFh, 00Fh, 000h, 033h, 0C8h, 089h, 04Ah, 008h, 0C1h
        db  0E1h, 00Ch, 08Bh, 042h, 009h, 033h, 0C1h, 025h, 0FFh, 00Fh, 000h, 000h, 033h, 0C1h, 089h, 042h
        db  009h, 0C7h, 005h, 088h, 0EEh, 000h, 000h, 000h, 000h, 000h, 000h, 0A1h, 018h, 0EFh, 000h, 000h
        db  083h, 0E8h, 030h, 0A3h, 024h, 0EFh, 000h, 000h, 0C7h, 005h, 0F8h, 0F8h, 000h, 000h, 088h, 001h
        db  000h, 000h, 0C7h, 005h, 088h, 0F8h, 000h, 000h, 088h, 001h, 000h, 080h, 0C7h, 044h, 024h, 00Ch
        db  00Ah, 000h, 000h, 000h, 08Dh, 044h, 024h, 00Ch, 0C7h, 044h, 024h, 010h, 0D8h, 027h, 000h, 000h
        db  0C7h, 044h, 024h, 014h, 0D8h, 027h, 000h, 000h, 0C7h, 044h, 024h, 018h, 004h, 000h, 000h, 000h
        db  0C7h, 044h, 024h, 01Ch, 000h, 000h, 000h, 000h, 0C7h, 044h, 024h, 020h, 008h, 000h, 002h, 020h
        db  050h, 068h, 0A0h, 0BEh, 000h, 000h, 0E8h, 066h, 00Eh, 000h, 000h, 0A3h, 0ACh, 0F8h, 000h, 000h
        db  0A1h, 068h, 0FAh, 000h, 000h, 0C7h, 000h, 000h, 000h, 000h, 000h, 0C7h, 044h, 024h, 00Ch, 00Ch
        db  000h, 000h, 000h, 08Dh, 044h, 024h, 00Ch, 0C7h, 044h, 024h, 010h, 068h, 001h, 000h, 000h, 0C7h
        db  044h, 024h, 014h, 068h, 001h, 000h, 000h, 050h, 068h, 054h, 08Ah, 000h, 000h, 0E8h, 02Fh, 00Eh
        db  000h, 000h, 0A3h, 0ECh, 0FAh, 000h, 000h, 068h, 000h, 0FCh, 003h, 000h, 068h, 000h, 000h, 00Ch
        db  000h, 068h, 0F8h, 0FCh, 000h, 000h, 0E8h, 094h, 0CFh, 0FAh, 0FFh, 0B8h, 001h, 000h, 000h, 000h
        db  05Eh, 083h, 0C4h, 024h, 0C2h, 004h, 000h, 056h, 068h, 000h, 000h, 00Ch, 000h, 0E8h, 0DFh, 001h
        db  0FBh, 0FFh, 08Bh, 044h, 024h, 008h, 00Fh, 0B7h, 050h, 014h, 00Fh, 0B7h, 048h, 01Ah, 003h, 0D1h
        db  00Fh, 0B7h, 040h, 020h, 003h, 0D0h, 0A1h, 000h, 0FDh, 000h, 000h, 052h, 0FFh, 070h, 008h, 0E8h
        db  040h, 0CEh, 0FAh, 0FFh, 08Bh, 0F0h, 0FFh, 035h, 008h, 0FDh, 000h, 000h, 0FFh, 015h, 0ECh, 0F7h
        db  000h, 000h, 083h, 0C4h, 004h, 083h, 04Eh, 004h, 010h, 08Bh, 0C6h, 02Bh, 005h, 0ECh, 0FAh, 000h
        db  000h, 0B9h, 00Ch, 000h, 000h, 000h, 02Bh, 0D2h, 0F7h, 0F1h, 066h, 08Bh, 00Dh, 00Ch, 0FDh, 000h
        db  000h, 066h, 089h, 04Eh, 00Ah, 08Bh, 00Dh, 004h, 0FDh, 000h, 000h, 066h, 08Bh, 049h, 008h, 066h
        db  089h, 04Eh, 008h, 08Bh, 00Dh, 004h, 0FDh, 000h, 000h, 00Fh, 0B7h, 051h, 008h, 06Bh, 0D2h, 00Ch
        db  08Bh, 00Dh, 0ECh, 0FAh, 000h, 000h, 066h, 089h, 044h, 00Ah, 00Ah, 08Bh, 00Dh, 004h, 0FDh, 000h
        db  000h, 066h, 089h, 041h, 008h, 0A1h, 000h, 0FDh, 000h, 000h, 08Bh, 040h, 008h, 081h, 028h, 000h
        db  004h, 000h, 000h, 0FFh, 035h, 0ECh, 0FAh, 000h, 000h, 0E8h, 0B3h, 002h, 0FBh, 0FFh, 08Bh, 0F0h
        db  08Bh, 046h, 004h, 025h, 010h, 0F0h, 0FFh, 0FFh, 083h, 0C8h, 010h, 089h, 046h, 004h, 025h, 0FFh
        db  00Fh, 000h, 000h, 00Dh, 000h, 000h, 080h, 0FFh, 089h, 046h, 004h, 0C7h, 006h, 000h, 004h, 000h
        db  000h, 08Bh, 0C6h, 02Bh, 005h, 0ECh, 0FAh, 000h, 000h, 0B9h, 00Ch, 000h, 000h, 000h, 02Bh, 0D2h
        db  0F7h, 0F1h, 066h, 08Bh, 00Dh, 00Ch, 0FDh, 000h, 000h, 066h, 089h, 04Eh, 00Ah, 08Bh, 00Dh, 004h
        db  0FDh, 000h, 000h, 066h, 08Bh, 049h, 008h, 066h, 089h, 04Eh, 008h, 08Bh, 00Dh, 004h, 0FDh, 000h
        db  000h, 00Fh, 0B7h, 051h, 008h, 06Bh, 0D2h, 00Ch, 08Bh, 00Dh, 0ECh, 0FAh, 000h, 000h, 066h, 089h
        db  044h, 00Ah, 00Ah, 08Bh, 00Dh, 004h, 0FDh, 000h, 000h, 066h, 089h, 041h, 008h, 0B9h, 000h, 0E0h
        db  0BFh, 0FFh, 0C1h, 0E0h, 00Ch, 0A3h, 0D0h, 0F7h, 000h, 000h, 0F6h, 001h, 001h, 075h, 007h, 0A1h
        db  0D0h, 0F7h, 000h, 000h, 089h, 001h, 083h, 0C1h, 004h, 081h, 0F9h, 000h, 0F0h, 0BFh, 0FFh, 072h
        db  0E9h, 05Eh, 0C2h, 004h, 000h, 06Ah, 018h, 068h, 000h, 001h, 000h, 000h, 068h, 000h, 000h, 008h
        db  080h, 0FFh, 015h, 09Ch, 0D7h, 000h, 000h, 083h, 0C4h, 00Ch, 083h, 0F8h, 0FFh, 00Fh, 084h, 0A5h
        db  000h, 000h, 000h, 068h, 088h, 000h, 000h, 000h, 068h, 000h, 000h, 010h, 000h, 050h, 050h, 0E8h
        db  0FDh, 0C9h, 0FAh, 0FFh, 0A3h, 0F8h, 0F7h, 000h, 000h, 00Bh, 0C0h, 00Fh, 084h, 087h, 000h, 000h
        db  000h, 005h, 000h, 000h, 010h, 000h, 0A3h, 018h, 0FDh, 000h, 000h, 06Ah, 018h, 068h, 000h, 001h
        db  000h, 000h, 068h, 000h, 000h, 008h, 080h, 0FFh, 015h, 09Ch, 0D7h, 000h, 000h, 083h, 0C4h, 00Ch
        db  083h, 0F8h, 0FFh, 074h, 063h, 068h, 0C0h, 000h, 000h, 000h, 068h, 000h, 000h, 010h, 000h, 050h
        db  050h, 0E8h, 0BBh, 0C9h, 0FAh, 0FFh, 0A3h, 0E8h, 0F7h, 000h, 000h, 00Bh, 0C0h, 074h, 049h, 0C7h
        db  040h, 04Ch, 008h, 0F9h, 000h, 000h, 06Ah, 010h, 068h, 000h, 001h, 000h, 000h, 068h, 000h, 000h
        db  008h, 080h, 0FFh, 015h, 09Ch, 0D7h, 000h, 000h, 083h, 0C4h, 00Ch, 083h, 0F8h, 0FFh, 074h, 028h
        db  06Ah, 040h, 068h, 000h, 000h, 010h, 000h, 050h, 050h, 0E8h, 083h, 0C9h, 0FAh, 0FFh, 0A3h, 0C4h
        db  0F8h, 000h, 000h, 00Bh, 0C0h, 074h, 011h, 0C7h, 040h, 04Ch, 008h, 0F9h, 000h, 000h, 0A1h, 0E8h
        db  0F7h, 000h, 000h, 0A3h, 0E0h, 0F7h, 000h, 000h, 0A1h, 0E0h, 0F7h, 000h, 000h, 0C3h, 083h, 0ECh
        db  034h, 053h, 056h, 057h, 055h, 08Bh, 074h, 024h, 048h, 08Bh, 046h, 004h, 03Dh, 000h, 010h, 000h
        db  000h, 077h, 005h, 0B8h, 000h, 010h, 000h, 000h, 0A3h, 0D0h, 0FCh, 000h, 000h, 005h, 03Ch, 001h
        db  000h, 000h, 06Bh, 0C0h, 00Dh, 0C1h, 0E8h, 00Ch, 0A3h, 0D0h, 0FCh, 000h, 000h, 06Ah, 018h, 050h
        db  068h, 000h, 000h, 008h, 080h, 0FFh, 015h, 09Ch, 0D7h, 000h, 000h, 083h, 0C4h, 00Ch, 08Bh, 0F8h
        db  083h, 0FFh, 0FFh, 00Fh, 084h, 0FAh, 004h, 000h, 000h, 02Bh, 0EDh, 0BBh, 06Ch, 0FAh, 000h, 000h
        db  03Bh, 0F3h, 072h, 04Ah, 08Bh, 003h, 06Bh, 0C0h, 00Dh, 003h, 0C7h, 0C1h, 0E8h, 00Ch, 03Bh, 0E8h
        db  075h, 001h, 040h, 08Bh, 06Bh, 004h, 06Bh, 0EDh, 00Dh, 003h, 0EFh, 083h, 0C5h, 00Dh, 04Dh, 0C1h
        db  0EDh, 00Ch, 03Bh, 0E8h, 072h, 021h, 068h, 008h, 000h, 002h, 020h, 06Ah, 000h, 06Ah, 004h, 08Bh
        db  0CDh, 02Bh, 0C8h, 041h, 051h, 050h, 0FFh, 015h, 0A0h, 0D7h, 000h, 000h, 083h, 0C4h, 014h, 00Bh
        db  0C0h, 00Fh, 084h, 0ACh, 004h, 000h, 000h, 083h, 0C3h, 008h, 03Bh, 0DEh, 076h, 0B6h, 0A1h, 06Ch
        db  0FAh, 000h, 000h, 06Bh, 0C0h, 00Dh, 03Dh, 000h, 010h, 000h, 000h, 072h, 022h, 068h, 008h, 000h
        db  002h, 020h, 06Ah, 000h, 06Ah, 004h, 06Ah, 001h, 08Bh, 0C7h, 0C1h, 0E8h, 00Ch, 050h, 0FFh, 015h
        db  0A0h, 0D7h, 000h, 000h, 083h, 0C4h, 014h, 00Bh, 0C0h, 00Fh, 084h, 074h, 004h, 000h, 000h, 06Ah
        db  018h, 068h, 000h, 00Ah, 000h, 000h, 068h, 000h, 000h, 008h, 080h, 0FFh, 015h, 09Ch, 0D7h, 000h
        db  000h, 083h, 0C4h, 00Ch, 08Bh, 0D8h, 083h, 0FBh, 0FFh, 00Fh, 084h, 054h, 004h, 000h, 000h, 068h
        db  008h, 000h, 002h, 020h, 06Ah, 000h, 06Ah, 004h, 06Ah, 003h, 08Bh, 0C3h, 0C1h, 0E8h, 00Ch, 050h
        db  0FFh, 015h, 0A0h, 0D7h, 000h, 000h, 083h, 0C4h, 014h, 00Bh, 0C0h, 00Fh, 084h, 032h, 004h, 000h
        db  000h, 0C7h, 044h, 024h, 028h, 00Ah, 000h, 000h, 000h, 0C7h, 044h, 024h, 02Ch, 000h, 030h, 000h
        db  000h, 0C7h, 044h, 024h, 030h, 000h, 000h, 0A0h, 000h, 0C7h, 044h, 024h, 034h, 004h, 000h, 000h
        db  000h, 08Dh, 044h, 024h, 028h, 0C7h, 044h, 024h, 038h, 000h, 000h, 000h, 000h, 0C7h, 044h, 024h
        db  03Ch, 008h, 000h, 002h, 020h, 050h, 053h, 0E8h, 0B5h, 00Ah, 000h, 000h, 08Bh, 0D8h, 06Ah, 018h
        db  068h, 0C0h, 000h, 000h, 000h, 068h, 000h, 000h, 008h, 080h, 0FFh, 015h, 09Ch, 0D7h, 000h, 000h
        db  083h, 0C4h, 00Ch, 08Bh, 0E8h, 083h, 0FDh, 0FFh, 00Fh, 084h, 0D5h, 003h, 000h, 000h, 068h, 008h
        db  000h, 002h, 020h, 06Ah, 000h, 06Ah, 004h, 06Ah, 001h, 08Bh, 0C5h, 0C1h, 0E8h, 00Ch, 050h, 0FFh
        db  015h, 0A0h, 0D7h, 000h, 000h, 083h, 0C4h, 014h, 00Bh, 0C0h, 00Fh, 084h, 0B3h, 003h, 000h, 000h
        db  08Bh, 00Dh, 0ECh, 0FAh, 000h, 000h, 089h, 04Ch, 024h, 020h, 0C7h, 044h, 024h, 028h, 00Ch, 000h
        db  000h, 000h, 0C7h, 044h, 024h, 02Ch, 000h, 010h, 000h, 000h, 08Dh, 044h, 024h, 028h, 0C7h, 044h
        db  024h, 030h, 000h, 000h, 00Ch, 000h, 050h, 055h, 0E8h, 044h, 00Ah, 000h, 000h, 0A3h, 0ECh, 0FAh
        db  000h, 000h, 050h, 0E8h, 089h, 0FFh, 0FAh, 0FFh, 089h, 01Dh, 0ACh, 0F8h, 000h, 000h, 089h, 03Dh
        db  060h, 0FAh, 000h, 000h, 0C7h, 005h, 0A0h, 0FDh, 000h, 000h, 000h, 000h, 000h, 000h, 08Bh, 047h
        db  008h, 025h, 000h, 000h, 0F0h, 0FFh, 089h, 047h, 008h, 0C1h, 0E0h, 00Ch, 08Bh, 04Fh, 009h, 033h
        db  0C8h, 081h, 0E1h, 0FFh, 00Fh, 000h, 000h, 033h, 0C8h, 089h, 04Fh, 009h, 0A1h, 068h, 0FAh, 000h
        db  000h, 08Dh, 057h, 00Dh, 0C7h, 000h, 000h, 000h, 000h, 000h, 0C7h, 007h, 000h, 000h, 000h, 000h
        db  089h, 015h, 068h, 0FAh, 000h, 000h, 0C7h, 005h, 0DCh, 0F7h, 000h, 000h, 001h, 000h, 000h, 000h
        db  08Bh, 04Ah, 008h, 081h, 0E1h, 000h, 000h, 0F0h, 0FFh, 0B1h, 001h, 089h, 04Ah, 008h, 0C1h, 0E1h
        db  00Ch, 08Bh, 042h, 009h, 033h, 0C1h, 025h, 0FFh, 00Fh, 000h, 000h, 033h, 0C1h, 089h, 042h, 009h
        db  089h, 03Dh, 0E4h, 0F7h, 000h, 000h, 081h, 0C7h, 000h, 00Dh, 000h, 000h, 089h, 03Dh, 0A4h, 0F8h
        db  000h, 000h, 0C7h, 002h, 000h, 000h, 000h, 000h, 08Bh, 046h, 004h, 0A3h, 020h, 0EFh, 000h, 000h
        db  06Bh, 0C0h, 00Dh, 003h, 005h, 0E4h, 0F7h, 000h, 000h, 0A3h, 0DCh, 0FCh, 000h, 000h, 0C7h, 005h
        db  040h, 0F9h, 000h, 000h, 000h, 000h, 000h, 000h, 0C7h, 005h, 050h, 0F9h, 000h, 000h, 000h, 000h
        db  000h, 000h, 02Bh, 0C0h, 0BDh, 06Ch, 0FAh, 000h, 000h, 03Bh, 0F5h, 072h, 053h, 08Bh, 05Dh, 000h
        db  06Bh, 0DBh, 00Dh, 003h, 01Dh, 0E4h, 0F7h, 000h, 000h, 08Bh, 07Dh, 004h, 06Bh, 0FFh, 00Dh, 003h
        db  03Dh, 0E4h, 0F7h, 000h, 000h, 03Bh, 0FBh, 072h, 030h, 040h, 0A3h, 018h, 0EFh, 000h, 000h, 0C7h
        db  007h, 000h, 000h, 000h, 000h, 0FFh, 005h, 040h, 0F9h, 000h, 000h, 0FFh, 035h, 0A0h, 0FDh, 000h
        db  000h, 0FFh, 035h, 060h, 0FAh, 000h, 000h, 057h, 0E8h, 0F4h, 011h, 0FBh, 0FFh, 083h, 0EFh, 00Dh
        db  03Bh, 0FBh, 0A1h, 018h, 0EFh, 000h, 000h, 073h, 0D0h, 083h, 0C5h, 008h, 03Bh, 0EEh, 076h, 0ADh
        db  0A3h, 018h, 0EFh, 000h, 000h, 03Dh, 0A0h, 001h, 000h, 000h, 00Fh, 08Ch, 053h, 002h, 000h, 000h
        db  0C7h, 005h, 01Ch, 0EFh, 000h, 000h, 000h, 000h, 000h, 000h, 0C7h, 005h, 088h, 0EEh, 000h, 000h
        db  000h, 000h, 000h, 000h, 083h, 0E8h, 030h, 0A3h, 024h, 0EFh, 000h, 000h, 0A1h, 004h, 0FDh, 000h
        db  000h, 089h, 044h, 024h, 018h, 089h, 044h, 024h, 01Ch, 0A1h, 000h, 0FDh, 000h, 000h, 0C7h, 005h
        db  084h, 0EEh, 000h, 000h, 000h, 000h, 000h, 000h, 08Bh, 040h, 008h, 089h, 044h, 024h, 024h, 068h
        db  000h, 0FCh, 003h, 000h, 068h, 000h, 000h, 00Ch, 000h, 068h, 0F8h, 0FCh, 000h, 000h, 0E8h, 03Ch
        db  0CAh, 0FAh, 0FFh, 08Bh, 044h, 024h, 04Ch, 08Bh, 048h, 01Ch, 081h, 0E1h, 0FFh, 0F3h, 0FFh, 0FFh
        db  0C1h, 0E9h, 00Ah, 081h, 0E9h, 000h, 000h, 080h, 000h, 089h, 04Ch, 024h, 010h, 00Fh, 0B7h, 040h
        db  020h, 08Dh, 004h, 081h, 089h, 044h, 024h, 014h, 08Bh, 044h, 024h, 018h, 00Fh, 0B7h, 040h, 00Ah
        db  06Bh, 0C0h, 00Ch, 003h, 044h, 024h, 020h, 089h, 044h, 024h, 018h, 03Bh, 044h, 024h, 01Ch, 00Fh
        db  084h, 05Bh, 001h, 000h, 000h, 0FFh, 035h, 0ECh, 0FAh, 000h, 000h, 0E8h, 0C1h, 0FDh, 0FAh, 0FFh
        db  08Bh, 0C8h, 08Bh, 0F9h, 08Bh, 074h, 024h, 018h, 0A5h, 0A5h, 0A5h, 08Bh, 0C1h, 02Bh, 005h, 0ECh
        db  0FAh, 000h, 000h, 0BEh, 00Ch, 000h, 000h, 000h, 02Bh, 0D2h, 0F7h, 0F6h, 08Bh, 0F0h, 066h, 0A1h
        db  00Ch, 0FDh, 000h, 000h, 066h, 089h, 041h, 00Ah, 0A1h, 004h, 0FDh, 000h, 000h, 066h, 08Bh, 040h
        db  008h, 066h, 089h, 041h, 008h, 0A1h, 004h, 0FDh, 000h, 000h, 00Fh, 0B7h, 040h, 008h, 06Bh, 0C0h
        db  00Ch, 08Bh, 015h, 0ECh, 0FAh, 000h, 000h, 066h, 089h, 074h, 010h, 00Ah, 0A1h, 004h, 0FDh, 000h
        db  000h, 066h, 089h, 070h, 008h, 08Bh, 079h, 004h, 081h, 0E7h, 0FFh, 0F3h, 0FFh, 0FFh, 0C1h, 0EFh
        db  00Ah, 081h, 0EFh, 000h, 000h, 080h, 000h, 08Bh, 001h, 08Dh, 02Ch, 087h, 03Bh, 0EFh, 00Fh, 086h
        db  064h, 0FFh, 0FFh, 0FFh, 08Bh, 01Fh, 0F6h, 0C7h, 002h, 00Fh, 084h, 0AEh, 000h, 000h, 000h, 0C1h
        db  0EBh, 00Ch, 06Bh, 0DBh, 00Dh, 003h, 01Dh, 0E4h, 0F7h, 000h, 000h, 0FFh, 00Dh, 040h, 0F9h, 000h
        db  000h, 053h, 0E8h, 029h, 011h, 0FBh, 0FFh, 0FFh, 035h, 0ACh, 0F8h, 000h, 000h, 0E8h, 01Fh, 0FDh
        db  0FAh, 0FFh, 089h, 003h, 0C7h, 043h, 004h, 000h, 000h, 000h, 000h, 066h, 0C7h, 043h, 00Ah, 001h
        db  000h, 066h, 0C7h, 000h, 001h, 000h, 081h, 0FFh, 000h, 0E0h, 0BFh, 0FFh, 0C6h, 040h, 003h, 010h
        db  066h, 089h, 070h, 004h, 089h, 058h, 006h, 072h, 02Ah, 08Dh, 08Fh, 000h, 020h, 000h, 000h, 083h
        db  0E1h, 0FCh, 0C1h, 0E1h, 00Ah, 08Dh, 091h, 000h, 010h, 080h, 0FFh, 081h, 0E9h, 000h, 000h, 080h
        db  000h, 03Bh, 0D1h, 076h, 01Ah, 0F6h, 001h, 001h, 075h, 02Bh, 083h, 0C1h, 004h, 03Bh, 0D1h, 077h
        db  0F4h, 0EBh, 00Ch, 039h, 07Ch, 024h, 010h, 077h, 01Ch, 039h, 07Ch, 024h, 014h, 076h, 016h, 066h
        db  0C7h, 043h, 008h, 000h, 000h, 0C6h, 040h, 002h, 001h, 080h, 048h, 003h, 0C0h, 0FFh, 005h, 084h
        db  0EEh, 000h, 000h, 0EBh, 02Bh, 066h, 0C7h, 043h, 008h, 088h, 0F1h, 0C6h, 040h, 002h, 004h, 0FFh
        db  005h, 088h, 0EEh, 000h, 000h, 0FFh, 005h, 01Ch, 0EFh, 000h, 000h, 0EBh, 013h, 08Bh, 0C6h, 0C1h
        db  0E0h, 00Ch, 08Bh, 0C8h, 033h, 0CBh, 081h, 0E1h, 0FFh, 00Fh, 000h, 000h, 033h, 0C8h, 089h, 00Fh
        db  083h, 0C7h, 004h, 03Bh, 0EFh, 00Fh, 087h, 029h, 0FFh, 0FFh, 0FFh, 0E9h, 088h, 0FEh, 0FFh, 0FFh
        db  0A1h, 000h, 0FDh, 000h, 000h, 08Bh, 050h, 008h, 08Bh, 044h, 024h, 024h, 083h, 0C2h, 004h, 08Bh
        db  040h, 004h, 08Bh, 00Ah, 033h, 0C8h, 081h, 0E1h, 0FFh, 00Fh, 000h, 000h, 033h, 0C8h, 08Bh, 044h
        db  024h, 024h, 089h, 00Ah, 08Bh, 008h, 0A1h, 000h, 0FDh, 000h, 000h, 08Bh, 040h, 008h, 089h, 008h
        db  08Bh, 00Dh, 0CCh, 0FCh, 000h, 000h, 0C1h, 0E9h, 00Ch, 06Bh, 0C9h, 00Dh, 0A1h, 0E4h, 0F7h, 000h
        db  000h, 066h, 0C7h, 044h, 001h, 008h, 000h, 0F0h, 0A1h, 0E4h, 0F7h, 000h, 000h, 08Bh, 004h, 001h
        db  00Fh, 0B7h, 040h, 004h, 0C1h, 0E0h, 00Ch, 0A3h, 0D0h, 0F7h, 000h, 000h, 0B8h, 001h, 000h, 000h
        db  000h, 0EBh, 002h, 02Bh, 0C0h, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 034h, 0C2h, 008h, 000h, 06Ah
        db  000h, 068h, 090h, 001h, 000h, 000h, 068h, 000h, 000h, 008h, 080h, 0FFh, 015h, 09Ch, 0D7h, 000h
        db  000h, 083h, 0C4h, 00Ch, 0A3h, 080h, 0EEh, 000h, 000h, 083h, 0F8h, 0FFh, 074h, 059h, 06Ah, 000h
        db  06Ah, 001h, 025h, 0FFh, 0F3h, 0FFh, 0FFh, 0C1h, 0E8h, 00Ah, 02Dh, 000h, 000h, 080h, 000h, 0C1h
        db  0E8h, 00Ch, 050h, 0FFh, 015h, 0B4h, 0D4h, 000h, 000h, 083h, 0C4h, 00Ch, 00Bh, 0C0h, 074h, 037h
        db  068h, 001h, 010h, 006h, 000h, 06Ah, 000h, 068h, 090h, 001h, 000h, 000h, 0A1h, 080h, 0EEh, 000h
        db  000h, 0C1h, 0E8h, 00Ch, 050h, 0E8h, 09Bh, 020h, 0FBh, 0FFh, 083h, 0C4h, 010h, 00Bh, 0C0h, 074h
        db  016h, 0C7h, 005h, 0E4h, 0FCh, 000h, 000h, 0FFh, 0FFh, 018h, 000h, 0A1h, 080h, 0EEh, 000h, 000h
        db  0A3h, 0E8h, 0FCh, 000h, 000h, 0EBh, 002h, 02Bh, 0C0h, 0A3h, 080h, 0EEh, 000h, 000h, 0C3h, 083h
        db  0ECh, 004h, 053h, 056h, 057h, 055h, 08Bh, 044h, 024h, 018h, 0C1h, 0E8h, 00Ch, 089h, 044h, 024h
        db  010h, 068h, 001h, 000h, 006h, 000h, 0A1h, 04Ch, 0F9h, 000h, 000h, 0FFh, 074h, 024h, 014h, 0C1h
        db  0E8h, 00Ch, 06Ah, 001h, 050h, 0E8h, 04Bh, 020h, 0FBh, 0FFh, 083h, 0C4h, 010h, 0B9h, 000h, 000h
        db  080h, 0FFh, 0BBh, 000h, 0E0h, 0BFh, 0FFh, 08Bh, 015h, 0D8h, 0F7h, 000h, 000h, 0F6h, 043h, 001h
        db  002h, 074h, 037h, 08Dh, 0B1h, 000h, 010h, 000h, 000h, 03Bh, 0F1h, 076h, 027h, 08Bh, 039h, 08Bh
        db  0C7h, 0C1h, 0E8h, 00Ch, 03Bh, 0C2h, 075h, 015h, 08Bh, 044h, 024h, 010h, 0C1h, 0E0h, 00Ch, 08Bh
        db  0E8h, 033h, 0EFh, 081h, 0E5h, 0FFh, 00Fh, 000h, 000h, 033h, 0E8h, 089h, 029h, 083h, 0C1h, 004h
        db  03Bh, 0F1h, 077h, 0D9h, 081h, 0E9h, 000h, 010h, 000h, 000h, 083h, 0C3h, 004h, 081h, 0C1h, 000h
        db  010h, 000h, 000h, 081h, 0FBh, 000h, 0F0h, 0BFh, 0FFh, 072h, 0B2h, 0C1h, 0E2h, 00Ch, 0BEh, 0E0h
        db  0FCh, 000h, 000h, 08Bh, 0C6h, 00Bh, 0C0h, 074h, 039h, 08Bh, 006h, 03Bh, 0C2h, 077h, 02Ch, 039h
        db  056h, 004h, 072h, 027h, 08Bh, 04Eh, 008h, 02Bh, 0C8h, 003h, 0CAh, 081h, 0E1h, 0FFh, 0F3h, 0FFh
        db  0FFh, 0C1h, 0E9h, 00Ah, 08Bh, 081h, 000h, 000h, 080h, 0FFh, 081h, 0E9h, 000h, 000h, 080h, 000h
        db  033h, 0C2h, 025h, 0FFh, 00Fh, 000h, 000h, 033h, 0C2h, 089h, 001h, 08Bh, 076h, 00Ch, 00Bh, 0F6h
        db  075h, 0C7h, 08Bh, 044h, 024h, 010h, 0A3h, 0D8h, 0F7h, 000h, 000h, 05Dh, 05Fh, 05Eh, 05Bh, 083h
        db  0C4h, 004h, 0C2h, 004h, 000h, 083h, 0ECh, 004h, 08Dh, 044h, 024h, 000h, 050h, 08Bh, 044h, 024h
        db  00Ch, 06Ah, 001h, 0C1h, 0E8h, 00Ch, 050h, 0E8h, 055h, 0F9h, 0FAh, 0FFh, 02Bh, 005h, 0ECh, 0FAh
        db  000h, 000h, 0B9h, 00Ch, 000h, 000h, 000h, 02Bh, 0D2h, 0F7h, 0F1h, 083h, 0C4h, 004h, 0C2h, 004h
        db  000h, 083h, 0ECh, 02Ch, 053h, 056h, 057h, 055h, 0C7h, 044h, 024h, 014h, 001h, 000h, 000h, 000h
        db  0C7h, 044h, 024h, 010h, 000h, 000h, 000h, 000h, 0C7h, 044h, 024h, 020h, 002h, 000h, 000h, 000h
        db  08Bh, 00Dh, 06Ch, 0F8h, 000h, 000h, 083h, 0F9h, 010h, 00Fh, 083h, 0A7h, 004h, 000h, 000h, 0A1h
        db  0D0h, 0FCh, 000h, 000h, 0C1h, 0E0h, 00Ch, 0BEh, 00Dh, 000h, 000h, 000h, 02Bh, 0D2h, 0F7h, 0F6h
        db  048h, 08Bh, 05Ch, 024h, 040h, 03Bh, 0D8h, 00Fh, 087h, 089h, 004h, 000h, 000h, 08Bh, 06Ch, 024h
        db  044h, 08Dh, 054h, 02Bh, 0FFh, 089h, 054h, 024h, 018h, 03Bh, 0D0h, 076h, 00Ch, 089h, 044h, 024h
        db  018h, 0C7h, 044h, 024h, 020h, 001h, 000h, 000h, 000h, 083h, 0FBh, 002h, 073h, 017h, 0BBh, 002h
        db  000h, 000h, 000h, 039h, 05Ch, 024h, 018h, 00Fh, 082h, 059h, 004h, 000h, 000h, 0C7h, 044h, 024h
        db  020h, 001h, 000h, 000h, 000h, 0F6h, 044h, 024h, 048h, 001h, 00Fh, 084h, 0C4h, 000h, 000h, 000h
        db  08Bh, 044h, 024h, 018h, 02Bh, 0C3h, 08Dh, 068h, 001h, 089h, 06Ch, 024h, 044h, 083h, 0FDh, 020h
        db  076h, 010h, 0C7h, 044h, 024h, 044h, 020h, 000h, 000h, 000h, 0C7h, 044h, 024h, 020h, 001h, 000h
        db  000h, 000h, 0A1h, 014h, 0FDh, 000h, 000h, 08Bh, 054h, 024h, 044h, 003h, 0D0h, 08Bh, 00Dh, 004h
        db  0F9h, 000h, 000h, 03Bh, 0D1h, 072h, 016h, 02Bh, 0C8h, 089h, 04Ch, 024h, 044h, 00Bh, 0C9h, 00Fh
        db  084h, 001h, 004h, 000h, 000h, 0C7h, 044h, 024h, 020h, 001h, 000h, 000h, 000h, 08Bh, 06Ch, 024h
        db  044h, 08Dh, 054h, 02Bh, 0FFh, 089h, 054h, 024h, 018h, 06Ah, 001h, 06Ah, 01Ch, 0FFh, 035h, 0C4h
        db  0F8h, 000h, 000h, 0E8h, 039h, 0D4h, 0FAh, 0FFh, 089h, 044h, 024h, 010h, 00Bh, 0C0h, 00Fh, 084h
        db  0D2h, 003h, 000h, 000h, 08Bh, 0D0h, 089h, 05Ah, 004h, 08Bh, 0D0h, 089h, 06Ah, 008h, 0A1h, 014h
        db  0FDh, 000h, 000h, 08Bh, 054h, 024h, 010h, 0C1h, 0E0h, 00Ch, 089h, 042h, 010h, 08Bh, 044h, 024h
        db  04Ch, 089h, 042h, 018h, 06Ah, 000h, 06Ah, 000h, 06Ah, 000h, 06Ah, 000h, 06Ah, 000h, 06Ah, 000h
        db  06Ah, 001h, 0FFh, 074h, 024h, 060h, 0FFh, 015h, 074h, 0D4h, 000h, 000h, 08Bh, 054h, 024h, 030h
        db  083h, 0C4h, 020h, 089h, 042h, 014h, 00Bh, 0C0h, 08Bh, 00Dh, 06Ch, 0F8h, 000h, 000h, 00Fh, 084h
        db  082h, 003h, 000h, 000h, 0BEh, 06Ch, 0FAh, 000h, 000h, 08Dh, 07Ch, 024h, 034h, 0C7h, 044h, 024h
        db  038h, 0FEh, 0FFh, 0FFh, 0FFh, 0C7h, 044h, 024h, 034h, 0FEh, 0FFh, 0FFh, 0FFh, 039h, 01Eh, 073h
        db  01Dh, 08Bh, 0FEh, 083h, 0C6h, 008h, 08Dh, 004h, 0CDh, 06Ch, 0FAh, 000h, 000h, 03Bh, 0C6h, 075h
        db  0ECh, 0C7h, 046h, 004h, 0FEh, 0FFh, 0FFh, 0FFh, 0C7h, 006h, 0FEh, 0FFh, 0FFh, 0FFh, 08Bh, 0C3h
        db  06Bh, 0C0h, 00Dh, 089h, 044h, 024h, 02Ch, 08Bh, 015h, 0E4h, 0F7h, 000h, 000h, 003h, 0D0h, 0C1h
        db  0EAh, 00Ch, 08Bh, 044h, 024h, 018h, 06Bh, 0C0h, 00Dh, 089h, 044h, 024h, 028h, 0A1h, 0E4h, 0F7h
        db  000h, 000h, 003h, 044h, 024h, 028h, 083h, 0C0h, 00Dh, 048h, 0C1h, 0E8h, 00Ch, 08Bh, 02Eh, 06Bh
        db  0EDh, 00Dh, 003h, 02Dh, 0E4h, 0F7h, 000h, 000h, 0C1h, 0EDh, 00Ch, 03Bh, 0E8h, 075h, 001h, 048h
        db  08Bh, 06Fh, 004h, 06Bh, 0EDh, 00Dh, 003h, 02Dh, 0E4h, 0F7h, 000h, 000h, 083h, 0C5h, 00Dh, 04Dh
        db  0C1h, 0EDh, 00Ch, 03Bh, 0EAh, 075h, 001h, 042h, 08Bh, 02Dh, 0E4h, 0F7h, 000h, 000h, 0C1h, 0EDh
        db  00Ch, 03Bh, 0EAh, 075h, 001h, 042h, 03Bh, 0D0h, 077h, 025h, 068h, 008h, 000h, 002h, 020h, 06Ah
        db  000h, 06Ah, 004h, 02Bh, 0C2h, 040h, 050h, 052h, 0FFh, 015h, 0A0h, 0D7h, 000h, 000h, 083h, 0C4h
        db  014h, 00Bh, 0C0h, 08Bh, 00Dh, 06Ch, 0F8h, 000h, 000h, 00Fh, 084h, 0B7h, 002h, 000h, 000h, 08Bh
        db  047h, 004h, 02Bh, 0C3h, 083h, 0F8h, 0FFh, 075h, 009h, 08Bh, 054h, 024h, 018h, 089h, 057h, 004h
        db  0EBh, 03Eh, 08Bh, 006h, 02Bh, 044h, 024h, 018h, 083h, 0F8h, 001h, 075h, 004h, 089h, 01Eh, 0EBh
        db  02Fh, 08Dh, 004h, 0CDh, 06Ch, 0FAh, 000h, 000h, 03Bh, 0C6h, 074h, 014h, 08Bh, 078h, 0F8h, 08Bh
        db  068h, 0FCh, 08Dh, 050h, 0F8h, 089h, 038h, 089h, 068h, 004h, 08Bh, 0C2h, 03Bh, 0F2h, 075h, 0ECh
        db  041h, 089h, 00Dh, 06Ch, 0F8h, 000h, 000h, 089h, 01Eh, 08Bh, 054h, 024h, 018h, 089h, 056h, 004h
        db  08Bh, 044h, 024h, 018h, 08Bh, 00Dh, 020h, 0EFh, 000h, 000h, 03Bh, 0C1h, 077h, 002h, 08Bh, 0C1h
        db  0A3h, 020h, 0EFh, 000h, 000h, 06Bh, 0C0h, 00Dh, 003h, 005h, 0E4h, 0F7h, 000h, 000h, 0A3h, 0DCh
        db  0FCh, 000h, 000h, 08Bh, 035h, 0E4h, 0F7h, 000h, 000h, 003h, 074h, 024h, 02Ch, 08Bh, 03Dh, 0E4h
        db  0F7h, 000h, 000h, 003h, 07Ch, 024h, 028h, 03Bh, 0F7h, 077h, 065h, 0A1h, 060h, 0FAh, 000h, 000h
        db  0FFh, 005h, 018h, 0EFh, 000h, 000h, 0FFh, 005h, 024h, 0EFh, 000h, 000h, 03Bh, 03Dh, 0A4h, 0F8h
        db  000h, 000h, 072h, 01Bh, 083h, 07Ch, 024h, 010h, 000h, 075h, 014h, 0C7h, 007h, 000h, 000h, 000h
        db  000h, 0FFh, 005h, 040h, 0F9h, 000h, 000h, 0FFh, 035h, 0A0h, 0FDh, 000h, 000h, 0EBh, 01Eh, 0C7h
        db  007h, 000h, 000h, 000h, 000h, 0FFh, 005h, 040h, 0F9h, 000h, 000h, 08Bh, 040h, 008h, 025h, 0FFh
        db  0FFh, 00Fh, 000h, 050h, 06Bh, 0C0h, 00Dh, 003h, 005h, 0E4h, 0F7h, 000h, 000h, 050h, 057h, 0E8h
        db  0DDh, 00Ah, 0FBh, 0FFh, 0A1h, 060h, 0FAh, 000h, 000h, 083h, 0EFh, 00Dh, 03Bh, 0F7h, 076h, 0A0h
        db  083h, 07Ch, 024h, 010h, 000h, 00Fh, 084h, 0F0h, 001h, 000h, 000h, 08Dh, 044h, 024h, 030h, 06Ah
        db  00Eh, 050h, 08Bh, 044h, 024h, 020h, 040h, 050h, 053h, 06Ah, 000h, 06Ah, 000h, 06Ah, 001h, 0FFh
        db  074h, 024h, 060h, 0FFh, 015h, 074h, 0D4h, 000h, 000h, 089h, 044h, 024h, 044h, 083h, 0C4h, 020h
        db  00Bh, 0C0h, 00Fh, 084h, 08Eh, 001h, 000h, 000h, 08Bh, 0C3h, 0C1h, 0E0h, 00Ch, 03Bh, 044h, 024h
        db  030h, 00Fh, 085h, 07Fh, 001h, 000h, 000h, 08Bh, 04Ch, 024h, 044h, 08Bh, 054h, 024h, 010h, 0C1h
        db  0E1h, 00Ch, 08Bh, 07Ah, 014h, 08Bh, 074h, 024h, 024h, 0C1h, 0E9h, 002h, 0F3h, 0A5h, 08Bh, 074h
        db  024h, 024h, 081h, 0E6h, 0FFh, 0F3h, 0FFh, 0FFh, 0C1h, 0EEh, 00Ah, 0A1h, 014h, 0FDh, 000h, 000h
        db  08Dh, 03Ch, 085h, 000h, 000h, 080h, 0FFh, 0FFh, 074h, 024h, 044h, 0C1h, 0E0h, 00Ch, 050h, 081h
        db  0EEh, 000h, 000h, 080h, 000h, 0FFh, 074h, 024h, 02Ch, 0E8h, 0B3h, 0BCh, 0FAh, 0FFh, 08Bh, 06Ch
        db  024h, 044h, 00Bh, 0EDh, 08Dh, 045h, 0FFh, 089h, 044h, 024h, 01Ch, 074h, 04Dh, 08Bh, 00Eh, 0C1h
        db  0E9h, 00Ch, 06Bh, 0C9h, 00Dh, 0A1h, 0E4h, 0F7h, 000h, 000h, 08Bh, 004h, 001h, 066h, 08Bh, 048h
        db  004h, 083h, 0C0h, 004h, 08Bh, 017h, 0C1h, 0EAh, 00Ch, 06Bh, 0D2h, 00Dh, 08Bh, 02Dh, 0E4h, 0F7h
        db  000h, 000h, 08Bh, 02Ch, 02Ah, 083h, 0C6h, 004h, 066h, 08Bh, 06Dh, 004h, 066h, 089h, 028h, 0A1h
        db  0E4h, 0F7h, 000h, 000h, 08Bh, 004h, 002h, 083h, 0C7h, 004h, 066h, 089h, 048h, 004h, 08Bh, 044h
        db  024h, 01Ch, 0FFh, 04Ch, 024h, 01Ch, 00Bh, 0C0h, 075h, 0B3h, 06Ah, 000h, 0FFh, 074h, 024h, 028h
        db  0FFh, 015h, 07Ch, 0D4h, 000h, 000h, 083h, 0C4h, 008h, 0A1h, 014h, 0FDh, 000h, 000h, 003h, 044h
        db  024h, 044h, 0A3h, 014h, 0FDh, 000h, 000h, 081h, 0FBh, 010h, 001h, 000h, 000h, 08Bh, 054h, 024h
        db  010h, 0A1h, 064h, 0FAh, 000h, 000h, 089h, 002h, 089h, 015h, 064h, 0FAh, 000h, 000h, 00Fh, 083h
        db  0D7h, 000h, 000h, 000h, 0A1h, 010h, 0F7h, 000h, 000h, 08Bh, 070h, 004h, 00Fh, 0B7h, 086h, 003h
        db  007h, 000h, 000h, 0C1h, 0E0h, 004h, 066h, 083h, 04Ch, 006h, 002h, 001h, 081h, 0FBh, 010h, 001h
        db  000h, 000h, 00Fh, 083h, 0B3h, 000h, 000h, 000h, 03Bh, 05Ch, 024h, 018h, 00Fh, 087h, 0A9h, 000h
        db  000h, 000h, 08Bh, 0C6h, 0C1h, 0E8h, 00Ch, 08Dh, 03Ch, 003h, 08Bh, 004h, 0BDh, 000h, 000h, 080h
        db  0FFh, 0C1h, 0E8h, 00Ch, 03Bh, 0C3h, 075h, 018h, 06Ah, 000h, 06Ah, 001h, 057h, 0E8h, 0ADh, 01Bh
        db  0FBh, 0FFh, 08Bh, 044h, 024h, 020h, 08Bh, 054h, 024h, 01Ch, 083h, 0C4h, 00Ch, 009h, 042h, 00Ch
        db  081h, 0FBh, 000h, 001h, 000h, 000h, 072h, 029h, 083h, 0C7h, 010h, 08Bh, 004h, 0BDh, 000h, 000h
        db  080h, 0FFh, 0C1h, 0E8h, 00Ch, 03Bh, 0C3h, 075h, 018h, 06Ah, 000h, 06Ah, 001h, 057h, 0E8h, 07Ch
        db  01Bh, 0FBh, 0FFh, 08Bh, 044h, 024h, 020h, 08Bh, 054h, 024h, 01Ch, 083h, 0C4h, 00Ch, 009h, 042h
        db  00Ch, 08Bh, 044h, 024h, 014h, 003h, 0C0h, 089h, 044h, 024h, 014h, 043h, 081h, 0FBh, 010h, 001h
        db  000h, 000h, 072h, 084h, 0EBh, 035h, 083h, 07Ch, 024h, 010h, 000h, 074h, 026h, 08Bh, 054h, 024h
        db  010h, 08Bh, 042h, 014h, 00Bh, 0C0h, 074h, 00Ch, 06Ah, 000h, 050h, 0FFh, 015h, 07Ch, 0D4h, 000h
        db  000h, 083h, 0C4h, 008h, 0FFh, 074h, 024h, 010h, 0FFh, 035h, 0C4h, 0F8h, 000h, 000h, 0E8h, 09Eh
        db  0CFh, 0FAh, 0FFh, 0C7h, 044h, 024h, 020h, 000h, 000h, 000h, 000h, 08Bh, 044h, 024h, 020h, 05Dh
        db  05Fh, 05Eh, 05Bh, 083h, 0C4h, 02Ch, 0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 083h, 0ECh, 004h, 053h, 056h, 057h, 08Bh, 05Ch, 024h, 014h, 08Bh, 0FBh, 08Bh, 074h, 024h
        db  018h, 0B9h, 007h, 000h, 000h, 000h, 0F3h, 0A5h, 08Dh, 073h, 01Ch, 089h, 073h, 018h, 089h, 074h
        db  024h, 00Ch, 08Bh, 043h, 004h, 083h, 0E8h, 01Ch, 02Bh, 0D2h, 0F7h, 033h, 048h, 074h, 015h, 08Bh
        db  00Bh, 08Bh, 054h, 024h, 00Ch, 08Bh, 07Ch, 024h, 00Ch, 003h, 0D1h, 089h, 017h, 001h, 04Ch, 024h
        db  00Ch, 048h, 075h, 0EBh, 08Bh, 044h, 024h, 00Ch, 05Fh, 0C7h, 000h, 000h, 000h, 000h, 000h, 08Bh
        db  0C6h, 05Eh, 05Bh, 083h, 0C4h, 004h, 0C2h, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   esi
        lea    eax, [ebp - 18h]
        mov    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp - 0ch], 1
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        db  0CDh, 020h, 007h, 000h, 026h, 000h
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_5_E55D
        cmp    byte ptr [ebp - 18h], 0
        je     SHORT loc_5_E585
loc_5_E55D:
        cmp    byte ptr [ebp - 18h], 2
        je     SHORT loc_5_E585
        db  080h, 03Dh, 064h, 003h, 000h, 000h, 000h
        jne    SHORT loc_5_E5A5
        db  0C7h, 045h, 0F8h, 043h, 055h, 000h, 000h
        mov    esi, dword ptr [ebp - 8]
        db  0CDh, 020h, 03Ah, 000h, 001h, 000h
        db  0C6h, 005h, 064h, 003h, 000h, 000h, 001h
        jmp    SHORT loc_5_E5A5
loc_5_E585:
        db  080h, 03Dh, 064h, 003h, 000h, 000h, 000h
        je     SHORT loc_5_E5A5
        db  0C7h, 045h, 0FCh, 043h, 055h, 000h, 000h
        mov    esi, dword ptr [ebp - 4]
        db  0CDh, 020h, 02Bh, 001h, 001h, 000h
        db  0C6h, 005h, 064h, 003h, 000h, 000h, 000h
loc_5_E5A5:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h, 06Ah, 000h, 068h, 000h, 020h, 000h, 000h, 0CDh, 020h, 04Fh, 000h, 001h, 000h, 059h
        db  059h, 00Bh, 0C0h, 074h, 019h, 09Ch, 0FAh, 0BAh, 070h, 0EFh, 000h, 000h, 089h, 002h, 0C7h, 042h
        db  004h, 000h, 020h, 000h, 000h, 0FEh, 005h, 0A0h, 0EFh, 000h, 000h, 09Dh, 0F8h, 0C3h, 0F9h, 0C3h
        db  000h, 000h, 08Bh, 044h, 024h, 004h, 0A3h, 070h, 0F6h, 000h, 000h, 0B0h, 001h, 0C3h, 0C8h, 000h
        db  000h, 000h, 057h, 056h, 053h, 0BFh, 0C0h, 0F0h, 000h, 000h, 0B9h, 009h, 000h, 000h, 000h, 033h
        db  0C0h, 048h, 0FCh, 0F3h, 0ABh, 0BEh, 0B0h, 0EFh, 000h, 000h, 0B9h, 010h, 001h, 000h, 000h, 033h
        db  0D2h, 033h, 0DBh, 0F6h, 006h, 050h, 075h, 008h, 00Fh, 0B3h, 01Ch, 095h, 0C0h, 0F0h, 000h, 000h
        db  046h, 043h, 083h, 0FBh, 020h, 072h, 003h, 033h, 0DBh, 042h, 0E2h, 0E7h, 0BEh, 0C0h, 0F0h, 000h
        db  000h, 08Bh, 03Dh, 010h, 0F7h, 000h, 000h, 08Dh, 0BFh, 078h, 001h, 000h, 000h, 0B9h, 009h, 000h
        db  000h, 000h, 0FCh, 0F3h, 0A5h, 05Bh, 05Eh, 05Fh, 0C9h, 0C3h, 060h, 083h, 03Dh, 074h, 0E9h, 000h
        db  000h, 000h, 07Eh, 042h, 08Bh, 00Dh, 078h, 0E9h, 000h, 000h, 02Bh, 00Dh, 074h, 0E9h, 000h, 000h
        db  074h, 034h, 083h, 0C1h, 003h, 080h, 0E1h, 0FCh, 051h, 068h, 000h, 002h, 000h, 000h, 051h, 0CDh
        db  020h
loc_5_E66B:
        dec    edi
        add    byte ptr [ecx], al
        add    byte ptr [ecx + 59h], bl
        pop    ecx
        db  0A3h, 068h, 003h, 000h, 000h
        db  085h
loc_5_E678:
        sar    cl, 74h
        adc    eax, 358bf88bh
        je     SHORT loc_5_E66B
        add    byte ptr [eax], al
        shr    ecx, 2
        db  089h, 00Dh, 06Ch, 003h, 000h, 000h
        cld
        rep movsd
        clc
        popad
        ret
_ITEXT ends
        end
