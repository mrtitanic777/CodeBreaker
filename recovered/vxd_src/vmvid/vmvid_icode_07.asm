; vmvid_icode_07.asm -- VMVID object 7 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

        db  06Ah, 061h, 07Ah, 07Ah, 02Eh, 064h, 072h, 076h, 000h, 06Ah, 061h, 07Ah, 07Ah, 034h, 030h, 031h
        db  02Eh, 064h, 072h, 076h, 000h, 064h, 069h, 073h, 061h, 062h, 06Ch, 065h, 077h, 061h, 072h, 06Eh
        db  069h, 06Eh, 067h, 000h, 070h, 06Fh, 072h, 074h, 000h, 069h, 06Eh, 074h, 000h, 064h, 06Dh, 061h
        db  063h, 068h, 061h, 06Eh, 06Eh, 065h, 06Ch, 000h, 064h, 06Dh, 061h, 063h, 068h, 061h, 06Eh, 06Eh
        db  065h, 06Ch, 032h, 000h, 064h, 06Dh, 061h, 070h, 061h, 067h, 065h, 073h, 000h, 0CCh, 0CCh, 0CCh
loc_7_0050:
        push   0
        push   4
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_7_0063
        stc
        ret
loc_7_0063:
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, 15000eh
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        clc
        ret
_ITEXT ends
        end
