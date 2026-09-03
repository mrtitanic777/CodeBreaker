; vmpoll_icode_02.asm -- VMPOLL object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        pushad
        db  08Bh, 03Dh, 080h, 001h, 000h, 000h
        db  068h, 08Ch, 000h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  068h, 0A5h, 000h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
loc_2_0023:
        call   sub_2_0057
        db  068h, 0A9h, 000h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  0CDh, 020h, 0C4h, 000h, 001h, 000h
        db  068h, 0D8h, 000h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        je     SHORT loc_2_0054
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        db  0CDh, 020h, 002h, 000h, 001h, 000h
        jne    SHORT loc_2_0023
loc_2_0054:
        popad
        clc
        ret
sub_2_0057:
        lea    edx, [edi + ebx]
        mov    eax, dword ptr [edx]
        db  068h, 0DCh, 000h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        mov    eax, dword ptr [edx + 4]
        mov    ecx, dword ptr [edx + 8]
        mov    esi, dword ptr [edx + 0ch]
        mov    edx, dword ptr [edx + 10h]
        db  068h, 008h, 001h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        db  068h, 04Dh, 001h, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h
        ret
        db  000h, 000h, 043h, 042h, 020h, 06Fh, 066h, 066h, 073h, 065h, 074h, 020h, 020h, 020h, 020h, 020h
        db  020h, 020h, 03Dh, 020h, 023h, 045h, 044h, 049h, 00Dh, 00Ah, 000h, 020h, 00Dh, 00Ah, 000h, 05Bh
        db  045h, 053h, 043h, 05Dh, 020h, 074h, 06Fh, 020h, 065h, 078h, 069h, 074h, 02Ch, 020h, 061h, 06Eh
        db  079h, 020h, 06Fh, 074h, 068h, 065h, 072h, 020h, 06Bh, 065h, 079h, 020h, 066h, 06Fh, 072h, 020h
        db  06Eh, 065h, 078h, 074h, 020h, 073h, 063h, 072h, 065h, 065h, 06Eh, 03Ah, 020h, 000h, 020h, 00Dh
        db  00Ah, 000h, 056h, 04Dh, 020h, 023h, 045h, 042h, 058h, 02Ch, 020h, 042h, 061h, 073h, 065h, 020h
        db  061h, 064h, 064h, 072h, 065h, 073h, 073h, 020h, 03Dh, 020h, 023h, 045h, 044h, 058h, 02Ch, 020h
        db  050h, 072h, 06Fh, 063h, 020h, 03Dh, 020h, 03Fh, 045h, 041h, 058h, 00Dh, 00Ah, 000h, 020h, 020h
        db  020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 04Ch, 061h, 073h, 074h, 020h, 074h
        db  069h, 06Dh, 065h, 020h, 023h, 045h, 041h, 058h, 02Ch, 020h, 04Ch, 061h, 073h, 074h, 020h, 076h
        db  069h, 064h, 020h, 023h, 045h, 043h, 058h, 02Ch, 020h, 054h, 068h, 072h, 065h, 073h, 068h, 020h
        db  023h, 045h, 053h, 049h, 02Ch, 020h, 046h, 06Ch, 061h, 067h, 073h, 020h, 023h, 045h, 044h, 058h
        db  00Dh, 00Ah, 000h, 020h, 00Dh, 00Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_01C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_ITEXT ends
        end
