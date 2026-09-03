; vpasd_icode_08.asm -- VPASD object 8 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_8_0000:
        push   0
        push   4ah
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_8_0013
        stc
        ret
loc_8_0013:
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, 2fh
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        mov    eax, 15000eh
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        clc
        ret
_ITEXT ends
        end
