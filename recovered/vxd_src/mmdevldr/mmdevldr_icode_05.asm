; mmdevldr_icode_05.asm -- MMDEVLDR object 5 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_5_0000:
        db  0E8h, 000h, 000h, 000h, 000h
        xor    eax, eax
        clc
        ret
_ITEXT ends
        end
