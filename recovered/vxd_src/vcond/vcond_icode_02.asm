; vcond_icode_02.asm -- VCOND object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        mov    ecx, 1
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        jb     SHORT loc_2_0060
        db  0A3h, 0DEh, 001h, 000h, 000h
        db  0BEh, 0DFh, 002h, 000h, 000h
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h
        jb     SHORT loc_2_0065
        db  0A3h, 0B0h, 001h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 044h, 000h, 000h, 000h
        db  0CDh, 020h, 029h, 001h, 001h, 000h
        add    esp, 8
        db  0E8h, 0D0h, 015h, 0FEh, 0FFh
        or     eax, eax
        je     SHORT loc_2_006A
        db  0C7h, 005h, 024h, 003h, 000h, 000h, 000h, 000h, 000h, 000h
        push   0
        push   4
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        add    esp, 8
        db  0A3h, 0E6h, 001h, 000h, 000h
        mov    al, 1
        clc
loc_2_005F:
        ret
loc_2_0060:
        xor    eax, eax
        stc
        jmp    SHORT loc_2_005F
loc_2_0065:
        xor    eax, eax
        stc
        jmp    SHORT loc_2_005F
loc_2_006A:
        stc
        jmp    SHORT loc_2_005F
        db  000h, 000h, 000h
_ITEXT ends
        end
