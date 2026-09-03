; avvxp500_icode_07.asm -- AVVXP500 object 7 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_7_0000:
        push   0
        push   4
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_7_0013
        stc
        ret
loc_7_0013:
        db  0A3h, 000h, 000h, 000h, 000h
        clc
        ret
_ITEXT ends
        end
