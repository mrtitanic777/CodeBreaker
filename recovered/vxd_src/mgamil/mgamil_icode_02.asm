; mgamil_icode_02.asm -- MGAMIL object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  089h, 01Dh, 02Ch, 008h, 000h, 000h
        call   sub_2_00E9
        or     ax, ax
        je     NEAR PTR loc_2_00E7
        db  0C7h, 005h, 010h, 008h, 000h, 000h, 000h, 000h, 000h, 000h
        push   0
        push   1000h
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_0062
        mov    edi, eax
        mov    esi, eax
        and    eax, 0fh
        and    esi, 0fffffff0h
        shl    esi, 0ch
        or     eax, esi
        db  0A3h, 058h, 008h, 000h, 000h
        db  0BEh, 0F0h, 000h, 000h, 000h
        mov    ecx, 0f30h
        rep movsb
        db  0BEh, 0D5h, 001h, 000h, 000h
        mov    eax, 6dh
        db  0CDh, 020h, 041h, 000h, 001h, 000h
loc_2_0062:
        db  0BEh, 087h, 001h, 000h, 000h
        mov    eax, 10h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        db  0CDh, 020h, 00Eh, 000h, 00Ah, 000h
        cmp    ecx, 31h
        jb     SHORT loc_2_00E7
        db  0C7h, 007h, 000h, 000h, 000h, 000h
        db  0C7h, 047h, 068h, 081h, 001h, 000h, 000h
        db  0C7h, 047h, 010h, 007h, 000h, 000h, 000h
        db  0C7h, 047h, 018h, 00Dh, 000h, 000h, 000h
        db  0C7h, 087h, 0BCh, 000h, 000h, 000h, 051h, 002h, 000h, 000h
        db  0C7h, 087h, 090h, 000h, 000h, 000h, 011h, 001h, 000h, 000h
        db  0C7h, 087h, 0ACh, 000h, 000h, 000h, 077h, 002h, 000h, 000h
        db  0B9h, 00Eh, 000h, 000h, 000h
        mov    edx, 3deh
        db  0CDh, 020h, 013h, 000h, 00Ah, 000h
        jb     SHORT loc_2_00E7
        db  0B9h, 00Eh, 000h, 000h, 000h
        mov    edx, 3dfh
        db  0CDh, 020h, 013h, 000h, 00Ah, 000h
        jb     SHORT loc_2_00E7
        db  0C7h, 005h, 030h, 008h, 000h, 000h, 000h, 000h, 000h, 000h
        clc
        jmp    SHORT loc_2_00E8
loc_2_00E7:
        stc
loc_2_00E8:
        ret
sub_2_00E9:
        mov    ax, 1
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 07Eh, 081h, 0A5h, 081h, 081h, 0BDh, 099h, 081h, 07Eh, 000h, 000h, 000h, 000h, 000h
        db  07Eh, 0FFh, 0DBh, 0FFh, 0FFh, 0C3h, 0E7h, 0FFh, 07Eh, 000h, 000h, 000h, 000h, 000h, 000h, 06Ch
        db  0FEh, 0FEh, 0FEh, 0FEh, 07Ch, 038h, 010h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 038h, 07Ch
        db  0FEh, 07Ch, 038h, 010h, 000h, 000h, 000h, 000h, 000h, 000h, 018h, 03Ch, 03Ch, 0E7h, 0E7h, 0E7h
        db  018h, 018h, 03Ch, 000h, 000h, 000h, 000h, 000h, 018h, 03Ch, 07Eh, 0FFh, 0FFh, 07Eh, 018h, 018h
        db  03Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 018h, 03Ch, 03Ch, 018h, 000h, 000h, 000h
        db  000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0E7h, 0C3h, 0C3h, 0E7h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  000h, 000h, 000h, 000h, 03Ch, 066h, 042h, 042h, 066h, 03Ch, 000h, 000h, 000h, 000h, 0FFh, 0FFh
        db  0FFh, 0FFh, 0C3h, 099h, 0BDh, 0BDh, 099h, 0C3h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 01Eh, 00Eh
        db  01Ah, 032h, 078h, 0CCh, 0CCh, 0CCh, 078h, 000h, 000h, 000h, 000h, 000h, 03Ch, 066h, 066h, 066h
        db  03Ch, 018h, 07Eh, 018h, 018h, 000h, 000h, 000h, 000h, 000h, 03Fh, 033h, 03Fh, 030h, 030h, 030h
        db  070h, 0F0h, 0E0h, 000h, 000h, 000h, 000h, 000h, 07Fh, 063h, 07Fh, 063h, 063h, 063h, 067h, 0E7h
        db  0E6h, 0C0h, 000h, 000h, 000h, 000h, 018h, 018h, 0DBh, 03Ch, 0E7h, 03Ch, 0DBh, 018h, 018h, 000h
        db  000h, 000h, 000h, 000h, 080h, 0C0h, 0E0h, 0F8h, 0FEh, 0F8h, 0E0h, 0C0h, 080h, 000h, 000h, 000h
        db  000h, 000h, 002h, 006h, 00Eh, 03Eh, 0FEh, 03Eh, 00Eh, 006h, 002h, 000h, 000h, 000h, 000h, 000h
        db  018h, 03Ch, 07Eh, 018h, 018h, 018h, 07Eh, 03Ch, 018h, 000h, 000h, 000h, 000h, 000h, 066h, 066h
        db  066h, 066h, 066h, 066h, 000h, 066h, 066h, 000h, 000h, 000h, 000h, 000h, 07Fh, 0DBh, 0DBh, 0DBh
        db  07Bh, 01Bh, 01Bh, 01Bh, 01Bh, 000h, 000h, 000h, 000h, 07Ch, 0C6h, 060h, 038h, 06Ch, 0C6h, 0C6h
        db  06Ch, 038h, 00Ch, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FEh, 0FEh
        db  0FEh, 000h, 000h, 000h, 000h, 000h, 018h, 03Ch, 07Eh, 018h, 018h, 018h, 07Eh, 03Ch, 018h, 07Eh
        db  000h, 000h, 000h, 000h, 018h, 03Ch, 07Eh, 018h, 018h, 018h, 018h, 018h, 018h, 000h, 000h, 000h
        db  000h, 000h, 018h, 018h, 018h, 018h, 018h, 018h, 07Eh, 03Ch, 018h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 018h, 00Ch, 0FEh, 00Ch, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  030h, 060h, 0FEh, 060h, 030h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h
        db  0C0h, 0C0h, 0FEh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 028h, 06Ch, 0FEh, 06Ch
        db  028h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 038h, 038h, 07Ch, 07Ch, 0FEh, 0FEh
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FEh, 0FEh, 07Ch, 07Ch, 038h, 038h, 010h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 018h, 03Ch, 03Ch, 03Ch, 018h, 018h, 000h, 018h, 018h, 000h, 000h, 000h, 000h, 066h
        db  066h, 066h, 024h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 06Ch, 06Ch
        db  0FEh, 06Ch, 06Ch, 06Ch, 0FEh, 06Ch, 06Ch, 000h, 000h, 000h, 018h, 018h, 07Ch, 0C6h, 0C2h, 0C0h
        db  07Ch, 006h, 086h, 0C6h, 07Ch, 018h, 018h, 000h, 000h, 000h, 000h, 000h, 0C2h, 0C6h, 00Ch, 018h
        db  030h, 066h, 0C6h, 000h, 000h, 000h, 000h, 000h, 038h, 06Ch, 06Ch, 038h, 076h, 0DCh, 0CCh, 0CCh
        db  076h, 000h, 000h, 000h, 000h, 030h, 030h, 030h, 060h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 00Ch, 018h, 030h, 030h, 030h, 030h, 030h, 018h, 00Ch, 000h, 000h, 000h
        db  000h, 000h, 030h, 018h, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 018h, 030h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 066h, 03Ch, 0FFh, 03Ch, 066h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  018h, 018h, 07Eh, 018h, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 018h, 018h, 018h, 030h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FEh, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 018h
        db  018h, 000h, 000h, 000h, 000h, 000h, 002h, 006h, 00Ch, 018h, 030h, 060h, 0C0h, 080h, 000h, 000h
        db  000h, 000h, 000h, 000h, 07Ch, 0C6h, 0CEh, 0DEh, 0F6h, 0E6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h
        db  000h, 000h, 018h, 038h, 078h, 018h, 018h, 018h, 018h, 018h, 07Eh, 000h, 000h, 000h, 000h, 000h
        db  07Ch, 0C6h, 006h, 00Ch, 018h, 030h, 060h, 0C6h, 0FEh, 000h, 000h, 000h, 000h, 000h, 07Ch, 0C6h
        db  006h, 006h, 03Ch, 006h, 006h, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 000h, 00Ch, 01Ch, 03Ch, 06Ch
        db  0CCh, 0FEh, 00Ch, 00Ch, 01Eh, 000h, 000h, 000h, 000h, 000h, 0FEh, 0C0h, 0C0h, 0C0h, 0FCh, 006h
        db  006h, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 000h, 038h, 060h, 0C0h, 0C0h, 0FCh, 0C6h, 0C6h, 0C6h
        db  07Ch, 000h, 000h, 000h, 000h, 000h, 0FEh, 0C6h, 006h, 00Ch, 018h, 030h, 030h, 030h, 030h, 000h
        db  000h, 000h, 000h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 07Ch, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h
        db  000h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 07Eh, 006h, 006h, 00Ch, 078h, 000h, 000h, 000h, 000h, 000h
        db  000h, 018h, 018h, 000h, 000h, 000h, 018h, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 018h
        db  018h, 000h, 000h, 000h, 018h, 018h, 030h, 000h, 000h, 000h, 000h, 000h, 006h, 00Ch, 018h, 030h
        db  060h, 030h, 018h, 00Ch, 006h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 07Eh, 000h, 000h
        db  07Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 060h, 030h, 018h, 00Ch, 006h, 00Ch, 018h, 030h
        db  060h, 000h, 000h, 000h, 000h, 000h, 07Ch, 0C6h, 0C6h, 00Ch, 018h, 018h, 000h, 018h, 018h, 000h
        db  000h, 000h, 000h, 000h, 07Ch, 0C6h, 0C6h, 0DEh, 0DEh, 0DEh, 0DCh, 0C0h, 07Ch, 000h, 000h, 000h
        db  000h, 000h, 010h, 038h, 06Ch, 0C6h, 0C6h, 0FEh, 0C6h, 0C6h, 0C6h, 000h, 000h, 000h, 000h, 000h
        db  0FCh, 066h, 066h, 066h, 07Ch, 066h, 066h, 066h, 0FCh, 000h, 000h, 000h, 000h, 000h, 03Ch, 066h
        db  0C2h, 0C0h, 0C0h, 0C0h, 0C2h, 066h, 03Ch, 000h, 000h, 000h, 000h, 000h, 0F8h, 06Ch, 066h, 066h
        db  066h, 066h, 066h, 06Ch, 0F8h, 000h, 000h, 000h, 000h, 000h, 0FEh, 066h, 062h, 068h, 078h, 068h
        db  062h, 066h, 0FEh, 000h, 000h, 000h, 000h, 000h, 0FEh, 066h, 062h, 068h, 078h, 068h, 060h, 060h
        db  0F0h, 000h, 000h, 000h, 000h, 000h, 03Ch, 066h, 0C2h, 0C0h, 0C0h, 0DEh, 0C6h, 066h, 03Ah, 000h
        db  000h, 000h, 000h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 0FEh, 0C6h, 0C6h, 0C6h, 0C6h, 000h, 000h, 000h
        db  000h, 000h, 03Ch, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h, 000h
        db  01Eh, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 0CCh, 0CCh, 078h, 000h, 000h, 000h, 000h, 000h, 0E6h, 066h
        db  06Ch, 06Ch, 078h, 06Ch, 06Ch, 066h, 0E6h, 000h, 000h, 000h, 000h, 000h, 0F0h, 060h, 060h, 060h
        db  060h, 060h, 062h, 066h, 0FEh, 000h, 000h, 000h, 000h, 000h, 0C6h, 0EEh, 0FEh, 0FEh, 0D6h, 0C6h
        db  0C6h, 0C6h, 0C6h, 000h, 000h, 000h, 000h, 000h, 0C6h, 0E6h, 0F6h, 0FEh, 0DEh, 0CEh, 0C6h, 0C6h
        db  0C6h, 000h, 000h, 000h, 000h, 000h, 038h, 06Ch, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 06Ch, 038h, 000h
        db  000h, 000h, 000h, 000h, 0FCh, 066h, 066h, 066h, 07Ch, 060h, 060h, 060h, 0F0h, 000h, 000h, 000h
        db  000h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 0D6h, 0DEh, 07Ch, 00Ch, 00Eh, 000h, 000h, 000h, 000h
        db  0FCh, 066h, 066h, 066h, 07Ch, 06Ch, 066h, 066h, 0E6h, 000h, 000h, 000h, 000h, 000h, 07Ch, 0C6h
        db  0C6h, 060h, 038h, 00Ch, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 000h, 07Eh, 07Eh, 05Ah, 018h
        db  018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h
        db  0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 06Ch, 038h
        db  010h, 000h, 000h, 000h, 000h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 0D6h, 0D6h, 0FEh, 07Ch, 06Ch, 000h
        db  000h, 000h, 000h, 000h, 0C6h, 0C6h, 06Ch, 038h, 038h, 038h, 06Ch, 0C6h, 0C6h, 000h, 000h, 000h
        db  000h, 000h, 066h, 066h, 066h, 066h, 03Ch, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h, 000h
        db  0FEh, 0C6h, 08Ch, 018h, 030h, 060h, 0C2h, 0C6h, 0FEh, 000h, 000h, 000h, 000h, 000h, 03Ch, 030h
        db  030h, 030h, 030h, 030h, 030h, 030h, 03Ch, 000h, 000h, 000h, 000h, 000h, 080h, 0C0h, 0E0h, 070h
        db  038h, 01Ch, 00Eh, 006h, 002h, 000h, 000h, 000h, 000h, 000h, 03Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch
        db  00Ch, 00Ch, 03Ch, 000h, 000h, 000h, 010h, 038h, 06Ch, 0C6h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FFh, 000h, 030h, 030h, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 078h, 00Ch, 07Ch, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h, 000h
        db  0E0h, 060h, 060h, 078h, 06Ch, 066h, 066h, 066h, 07Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 07Ch, 0C6h, 0C0h, 0C0h, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 000h, 01Ch, 00Ch, 00Ch, 03Ch
        db  06Ch, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 07Ch, 0C6h, 0FEh
        db  0C0h, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 000h, 038h, 06Ch, 064h, 060h, 0F0h, 060h, 060h, 060h
        db  0F0h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 0CCh, 0CCh, 0CCh, 07Ch, 00Ch, 0CCh
        db  078h, 000h, 000h, 000h, 0E0h, 060h, 060h, 06Ch, 076h, 066h, 066h, 066h, 0E6h, 000h, 000h, 000h
        db  000h, 000h, 018h, 018h, 000h, 038h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h, 000h
        db  006h, 006h, 000h, 00Eh, 006h, 006h, 006h, 006h, 066h, 066h, 03Ch, 000h, 000h, 000h, 0E0h, 060h
        db  060h, 066h, 06Ch, 078h, 06Ch, 066h, 0E6h, 000h, 000h, 000h, 000h, 000h, 038h, 018h, 018h, 018h
        db  018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0ECh, 0FEh, 0D6h
        db  0D6h, 0D6h, 0C6h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0DCh, 066h, 066h, 066h, 066h
        db  066h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 0DCh, 066h, 066h, 066h, 07Ch, 060h, 060h, 0F0h, 000h
        db  000h, 000h, 000h, 000h, 000h, 076h, 0CCh, 0CCh, 0CCh, 07Ch, 00Ch, 00Ch, 01Eh, 000h, 000h, 000h
        db  000h, 000h, 000h, 0DCh, 076h, 066h, 060h, 060h, 0F0h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 07Ch, 0C6h, 070h, 01Ch, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 000h, 010h, 030h, 030h, 0FCh
        db  030h, 030h, 030h, 036h, 01Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0CCh, 0CCh, 0CCh
        db  0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 066h, 066h, 066h, 066h, 03Ch
        db  018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C6h, 0C6h, 0D6h, 0D6h, 0FEh, 06Ch, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C6h, 06Ch, 038h, 038h, 06Ch, 0C6h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 07Eh, 006h, 00Ch, 0F8h, 000h, 000h, 000h
        db  000h, 000h, 000h, 0FEh, 0CCh, 018h, 030h, 066h, 0FEh, 000h, 000h, 000h, 000h, 000h, 00Eh, 018h
        db  018h, 018h, 070h, 018h, 018h, 018h, 00Eh, 000h, 000h, 000h, 000h, 000h, 018h, 018h, 018h, 018h
        db  000h, 018h, 018h, 018h, 018h, 000h, 000h, 000h, 000h, 000h, 070h, 018h, 018h, 018h, 00Eh, 018h
        db  018h, 018h, 070h, 000h, 000h, 000h, 000h, 000h, 076h, 0DCh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 038h, 06Ch, 0C6h, 0C6h, 0FEh, 000h, 000h
        db  000h, 000h, 000h, 000h, 03Ch, 066h, 0C2h, 0C0h, 0C0h, 0C2h, 066h, 03Ch, 00Ch, 006h, 07Ch, 000h
        db  000h, 000h, 0CCh, 0CCh, 000h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h, 00Ch
        db  018h, 030h, 000h, 07Ch, 0C6h, 0FEh, 0C0h, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 010h, 038h, 06Ch
        db  000h, 078h, 00Ch, 07Ch, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h, 000h, 0CCh, 0CCh, 000h, 078h
        db  00Ch, 07Ch, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h, 060h, 030h, 018h, 000h, 078h, 00Ch, 07Ch
        db  0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h, 038h, 06Ch, 038h, 000h, 078h, 00Ch, 07Ch, 0CCh, 0CCh
        db  076h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 03Ch, 066h, 060h, 066h, 03Ch, 00Ch, 006h, 03Ch
        db  000h, 000h, 000h, 010h, 038h, 06Ch, 000h, 07Ch, 0C6h, 0FEh, 0C0h, 0C6h, 07Ch, 000h, 000h, 000h
        db  000h, 000h, 0CCh, 0CCh, 000h, 07Ch, 0C6h, 0FEh, 0C0h, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 060h
        db  030h, 018h, 000h, 07Ch, 0C6h, 0FEh, 0C0h, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 000h, 066h, 066h
        db  000h, 038h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h, 018h, 03Ch, 066h, 000h, 038h
        db  018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h, 060h, 030h, 018h, 000h, 038h, 018h, 018h
        db  018h, 018h, 03Ch, 000h, 000h, 000h, 000h, 0C6h, 0C6h, 010h, 038h, 06Ch, 0C6h, 0C6h, 0FEh, 0C6h
        db  0C6h, 000h, 000h, 000h, 038h, 06Ch, 038h, 000h, 038h, 06Ch, 0C6h, 0C6h, 0FEh, 0C6h, 0C6h, 000h
        db  000h, 000h, 018h, 030h, 060h, 000h, 0FEh, 066h, 060h, 07Ch, 060h, 066h, 0FEh, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 0CCh, 076h, 036h, 07Eh, 0D8h, 0D8h, 06Eh, 000h, 000h, 000h, 000h, 000h
        db  03Eh, 06Ch, 0CCh, 0CCh, 0FEh, 0CCh, 0CCh, 0CCh, 0CEh, 000h, 000h, 000h, 000h, 010h, 038h, 06Ch
        db  000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 000h, 0C6h, 0C6h, 000h, 07Ch
        db  0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 060h, 030h, 018h, 000h, 07Ch, 0C6h, 0C6h
        db  0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 030h, 078h, 0CCh, 000h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
        db  076h, 000h, 000h, 000h, 000h, 060h, 030h, 018h, 000h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 076h, 000h
        db  000h, 000h, 000h, 000h, 0C6h, 0C6h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 07Eh, 006h, 00Ch, 078h, 000h
        db  000h, 0C6h, 0C6h, 038h, 06Ch, 0C6h, 0C6h, 0C6h, 0C6h, 06Ch, 038h, 000h, 000h, 000h, 000h, 0C6h
        db  0C6h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 018h, 018h, 03Ch
        db  066h, 060h, 060h, 066h, 03Ch, 018h, 018h, 000h, 000h, 000h, 000h, 038h, 06Ch, 064h, 060h, 0F0h
        db  060h, 060h, 060h, 0E6h, 0FCh, 000h, 000h, 000h, 000h, 000h, 066h, 066h, 03Ch, 018h, 07Eh, 018h
        db  07Eh, 018h, 018h, 000h, 000h, 000h, 000h, 0F8h, 0CCh, 0CCh, 0F8h, 0C4h, 0CCh, 0DEh, 0CCh, 0CCh
        db  0C6h, 000h, 000h, 000h, 000h, 00Eh, 01Bh, 018h, 018h, 018h, 07Eh, 018h, 018h, 018h, 018h, 0D8h
        db  070h, 000h, 000h, 018h, 030h, 060h, 000h, 078h, 00Ch, 07Ch, 0CCh, 0CCh, 076h, 000h, 000h, 000h
        db  000h, 00Ch, 018h, 030h, 000h, 038h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h, 018h
        db  030h, 060h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 018h, 030h, 060h
        db  000h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h, 000h, 076h, 0DCh, 000h, 0DCh
        db  066h, 066h, 066h, 066h, 066h, 000h, 000h, 000h, 076h, 0DCh, 000h, 0C6h, 0E6h, 0F6h, 0FEh, 0DEh
        db  0CEh, 0C6h, 0C6h, 000h, 000h, 000h, 000h, 03Ch, 06Ch, 06Ch, 03Eh, 000h, 07Eh, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 038h, 06Ch, 06Ch, 038h, 000h, 07Ch, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 030h, 030h, 000h, 030h, 030h, 060h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 0FEh, 0C0h, 0C0h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 0FEh, 006h, 006h, 006h, 000h, 000h, 000h, 000h, 000h, 0C0h, 0C0h, 0C6h
        db  0CCh, 0D8h, 030h, 060h, 0DCh, 086h, 00Ch, 018h, 03Eh, 000h, 000h, 0C0h, 0C0h, 0C6h, 0CCh, 0D8h
        db  030h, 066h, 0CEh, 09Eh, 03Eh, 006h, 006h, 000h, 000h, 000h, 018h, 018h, 000h, 018h, 018h, 03Ch
        db  03Ch, 03Ch, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 036h, 06Ch, 0D8h, 06Ch, 036h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0D8h, 06Ch, 036h, 06Ch, 0D8h, 000h, 000h, 000h
        db  000h, 000h, 011h, 044h, 011h, 044h, 011h, 044h, 011h, 044h, 011h, 044h, 011h, 044h, 011h, 044h
        db  055h, 0AAh, 055h, 0AAh, 055h, 0AAh, 055h, 0AAh, 055h, 0AAh, 055h, 0AAh, 055h, 0AAh, 0DDh, 077h
        db  0DDh, 077h, 0DDh, 077h, 0DDh, 077h, 0DDh, 077h, 0DDh, 077h, 0DDh, 077h, 018h, 018h, 018h, 018h
        db  018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h
        db  018h, 0F8h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 0F8h, 018h, 0F8h
        db  018h, 018h, 018h, 018h, 018h, 018h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 0F6h, 036h, 036h
        db  036h, 036h, 036h, 036h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FEh, 036h, 036h, 036h, 036h
        db  036h, 036h, 000h, 000h, 000h, 000h, 000h, 0F8h, 018h, 0F8h, 018h, 018h, 018h, 018h, 018h, 018h
        db  036h, 036h, 036h, 036h, 036h, 0F6h, 006h, 0F6h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h
        db  036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 000h, 000h, 000h, 000h
        db  000h, 0FEh, 006h, 0F6h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 0F6h
        db  006h, 0FEh, 000h, 000h, 000h, 000h, 000h, 000h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 0FEh
        db  000h, 000h, 000h, 000h, 000h, 000h, 018h, 018h, 018h, 018h, 018h, 0F8h, 018h, 0F8h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0F8h, 018h, 018h, 018h, 018h
        db  018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 01Fh, 000h, 000h, 000h, 000h, 000h, 000h
        db  018h, 018h, 018h, 018h, 018h, 018h, 018h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0FFh, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h
        db  018h, 018h, 018h, 01Fh, 018h, 018h, 018h, 018h, 018h, 018h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 0FFh
        db  018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 01Fh, 018h, 01Fh, 018h, 018h
        db  018h, 018h, 018h, 018h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 037h, 036h, 036h, 036h, 036h
        db  036h, 036h, 036h, 036h, 036h, 036h, 036h, 037h, 030h, 03Fh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 03Fh, 030h, 037h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h
        db  036h, 036h, 036h, 0F7h, 000h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 0FFh, 000h, 0F7h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 037h
        db  030h, 037h, 036h, 036h, 036h, 036h, 036h, 036h, 000h, 000h, 000h, 000h, 000h, 0FFh, 000h, 0FFh
        db  000h, 000h, 000h, 000h, 000h, 000h, 036h, 036h, 036h, 036h, 036h, 0F7h, 000h, 0F7h, 036h, 036h
        db  036h, 036h, 036h, 036h, 018h, 018h, 018h, 018h, 018h, 0FFh, 000h, 0FFh, 000h, 000h, 000h, 000h
        db  000h, 000h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0FFh, 000h, 0FFh, 018h, 018h, 018h, 018h, 018h, 018h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0FFh, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h
        db  036h, 036h, 036h, 03Fh, 000h, 000h, 000h, 000h, 000h, 000h, 018h, 018h, 018h, 018h, 018h, 01Fh
        db  018h, 01Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 01Fh, 018h, 01Fh
        db  018h, 018h, 018h, 018h, 018h, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 03Fh, 036h, 036h
        db  036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 0FFh, 036h, 036h, 036h, 036h
        db  036h, 036h, 018h, 018h, 018h, 018h, 018h, 0FFh, 018h, 0FFh, 018h, 018h, 018h, 018h, 018h, 018h
        db  018h, 018h, 018h, 018h, 018h, 018h, 018h, 0F8h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 01Fh, 018h, 018h, 018h, 018h, 018h, 018h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h
        db  0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh
        db  00Fh, 00Fh, 00Fh, 00Fh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 0DCh, 0D8h, 0D8h, 0DCh, 076h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 07Ch, 0C6h, 0FCh, 0C6h, 0C6h, 0FCh, 0C0h, 0C0h, 040h, 000h, 000h, 000h
        db  0FEh, 0C6h, 0C6h, 0C0h, 0C0h, 0C0h, 0C0h, 0C0h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FEh, 06Ch, 06Ch, 06Ch, 06Ch, 06Ch, 06Ch, 000h, 000h, 000h, 000h, 000h, 0FEh, 0C6h, 060h, 030h
        db  018h, 030h, 060h, 0C6h, 0FEh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 07Eh, 0D8h, 0D8h
        db  0D8h, 0D8h, 070h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 066h, 066h, 066h, 066h, 07Ch, 060h
        db  060h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 0DCh, 018h, 018h, 018h, 018h, 018h, 000h
        db  000h, 000h, 000h, 000h, 07Eh, 018h, 03Ch, 066h, 066h, 066h, 03Ch, 018h, 07Eh, 000h, 000h, 000h
        db  000h, 000h, 038h, 06Ch, 0C6h, 0C6h, 0FEh, 0C6h, 0C6h, 06Ch, 038h, 000h, 000h, 000h, 000h, 000h
        db  038h, 06Ch, 0C6h, 0C6h, 0C6h, 06Ch, 06Ch, 06Ch, 0EEh, 000h, 000h, 000h, 000h, 000h, 01Eh, 030h
        db  018h, 00Ch, 03Eh, 066h, 066h, 066h, 03Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 07Eh
        db  0DBh, 0DBh, 07Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 003h, 006h, 07Eh, 0DBh, 0DBh, 0F3h
        db  07Eh, 060h, 0C0h, 000h, 000h, 000h, 000h, 000h, 01Ch, 030h, 060h, 060h, 07Ch, 060h, 060h, 030h
        db  01Ch, 000h, 000h, 000h, 000h, 000h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0FEh, 000h, 000h, 0FEh, 000h, 000h, 0FEh, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 018h, 018h, 07Eh, 018h, 018h, 000h, 000h, 0FFh, 000h, 000h, 000h, 000h, 000h
        db  030h, 018h, 00Ch, 006h, 00Ch, 018h, 030h, 000h, 07Eh, 000h, 000h, 000h, 000h, 000h, 00Ch, 018h
        db  030h, 060h, 030h, 018h, 00Ch, 000h, 07Eh, 000h, 000h, 000h, 000h, 000h, 00Eh, 01Bh, 01Bh, 018h
        db  018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h
        db  0D8h, 0D8h, 070h, 000h, 000h, 000h, 000h, 000h, 000h, 018h, 018h, 000h, 07Eh, 000h, 018h, 018h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 0DCh, 000h, 076h, 0DCh, 000h, 000h, 000h
        db  000h, 000h, 000h, 038h, 06Ch, 06Ch, 038h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 018h, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Fh, 00Ch, 00Ch
        db  00Ch, 00Ch, 00Ch, 0ECh, 06Ch, 03Ch, 01Ch, 000h, 000h, 000h, 000h, 0D8h, 06Ch, 06Ch, 06Ch, 06Ch
        db  06Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 070h, 0D8h, 030h, 060h, 0C8h, 0F8h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 07Ch, 07Ch, 07Ch, 07Ch, 07Ch, 07Ch
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 01Dh, 000h, 000h, 000h, 000h, 024h, 066h, 0FFh, 066h, 024h, 000h, 000h, 000h, 000h
        db  000h, 022h, 000h, 063h, 063h, 063h, 022h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  02Bh, 000h, 000h, 000h, 018h, 018h, 018h, 0FFh, 018h, 018h, 018h, 000h, 000h, 000h, 000h, 02Dh
        db  000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 04Dh, 000h
        db  000h, 0C3h, 0E7h, 0FFh, 0DBh, 0C3h, 0C3h, 0C3h, 0C3h, 0C3h, 000h, 000h, 000h, 054h, 000h, 000h
        db  0FFh, 0DBh, 099h, 018h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 056h, 000h, 000h, 0C3h
        db  0C3h, 0C3h, 0C3h, 0C3h, 0C3h, 066h, 03Ch, 018h, 000h, 000h, 000h, 057h, 000h, 000h, 0C3h, 0C3h
        db  0C3h, 0C3h, 0DBh, 0DBh, 0FFh, 066h, 066h, 000h, 000h, 000h, 058h, 000h, 000h, 0C3h, 0C3h, 066h
        db  03Ch, 018h, 03Ch, 066h, 0C3h, 0C3h, 000h, 000h, 000h, 059h, 000h, 000h, 0C3h, 0C3h, 0C3h, 066h
        db  03Ch, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 05Ah, 000h, 000h, 0FFh, 0C3h, 086h, 00Ch, 018h
        db  030h, 061h, 0C3h, 0FFh, 000h, 000h, 000h, 06Dh, 000h, 000h, 000h, 000h, 000h, 0E6h, 0FFh, 0DBh
        db  0DBh, 0DBh, 0DBh, 000h, 000h, 000h, 076h, 000h, 000h, 000h, 000h, 000h, 0C3h, 0C3h, 0C3h, 066h
        db  03Ch, 018h, 000h, 000h, 000h, 077h, 000h, 000h, 000h, 000h, 000h, 0C3h, 0C3h, 0DBh, 0DBh, 0FFh
        db  066h, 000h, 000h, 000h, 091h, 000h, 000h, 000h, 000h, 06Eh, 03Bh, 01Bh, 07Eh, 0D8h, 0DCh, 077h
        db  000h, 000h, 000h, 09Bh, 000h, 018h, 018h, 07Eh, 0C3h, 0C0h, 0C0h, 0C3h, 07Eh, 018h, 018h, 000h
        db  000h, 000h, 09Dh, 000h, 000h, 0C3h, 066h, 03Ch, 018h, 0FFh, 018h, 0FFh, 018h, 018h, 000h, 000h
        db  000h, 09Eh, 000h, 0FCh, 066h, 066h, 07Ch, 062h, 066h, 06Fh, 066h, 066h, 0F3h, 000h, 000h, 000h
        db  0F1h, 000h, 000h, 018h, 018h, 018h, 0FFh, 018h, 018h, 018h, 000h, 0FFh, 000h, 000h, 000h, 0F6h
        db  000h, 000h, 018h, 018h, 000h, 000h, 0FFh, 000h, 000h, 018h, 018h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h
_ITEXT ends
        end
