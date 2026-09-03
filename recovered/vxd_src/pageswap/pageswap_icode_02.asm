; pageswap_icode_02.asm -- PAGESWAP object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

PageSwap_Init_File:
        db  0CDh, 020h, 000h, 000h, 021h, 000h
        jb     SHORT loc_2_0027
        db  0CDh, 020h, 001h, 000h, 021h, 000h
        test   ebx, ebx
        je     SHORT loc_2_0027
        db  089h, 01Dh, 0D0h, 000h, 000h, 000h
        push   eax
        db  0CDh, 020h, 000h, 000h, 021h, 000h
        pop    eax
        db  088h, 01Dh, 0D4h, 000h, 000h, 000h
        clc
loc_2_0027:
        ret
_ITEXT ends
        end
