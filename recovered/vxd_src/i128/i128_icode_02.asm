; i128_icode_02.asm -- I128 object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  089h, 01Dh, 034h, 001h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 00Ah, 000h
        cmp    ecx, 31h
        jg     SHORT loc_2_0028
        db  0C7h, 007h, 000h, 000h, 000h, 000h
        db  0C7h, 047h, 010h, 0CCh, 000h, 000h, 000h
        db  0C7h, 047h, 068h, 048h, 000h, 000h, 000h
        clc
        jmp    SHORT loc_2_0029
loc_2_0028:
        stc
loc_2_0029:
        ret
        db  000h, 000h
_ITEXT ends
        end
