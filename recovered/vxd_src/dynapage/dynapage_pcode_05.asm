; dynapage_pcode_05.asm -- DYNAPAGE object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

PageFile_Clean_Up:
        db  08Bh, 015h, 084h, 000h, 000h, 000h
        db  0E8h, 0F5h, 0FFh, 0FAh, 0FFh
        db  08Bh, 01Dh, 0F0h, 000h, 000h, 000h
        db  080h, 03Dh, 0F5h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_0026
        mov    eax, 0d700h
        db  0CDh, 020h, 032h, 000h, 040h, 000h
loc_5_0025:
        ret
loc_5_0026:
        mov    ah, 3eh
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jmp    SHORT loc_5_0025
        db  000h, 000h
_PTEXT ends
        end
