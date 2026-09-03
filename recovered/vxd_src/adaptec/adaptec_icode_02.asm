; adaptec_icode_02.asm -- ADAPTEC object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    eax, 0
        jne    SHORT loc_2_0024
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 0A2h, 004h
        jb     SHORT loc_2_0026
        push   0
        push   0
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0021:
        add    esp, 8
loc_2_0024:
        clc
        ret
loc_2_0026:
        stc
        ret
_ITEXT ends
        end
