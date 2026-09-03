; ebios_pcode_06.asm -- EBIOS object 6 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_6_0000:
        db  080h, 03Dh, 00Ch, 000h, 000h, 000h, 000h
        jne    SHORT loc_6_0025
        db  0A1h, 054h, 000h, 000h, 000h
        movzx  eax, ax
        push   0
        db  0FFh, 035h, 008h, 000h, 000h, 000h
        push   eax
        push   ebx
        push   eax
        db  0CDh, 020h, 05Eh, 000h, 001h, 000h
        add    esp, 14h
loc_6_0025:
        clc
        ret
_PTEXT ends
        end
