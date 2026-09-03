; pageswap_pcode_03.asm -- PAGESWAP object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        db  083h, 03Dh, 0D0h, 000h, 000h, 000h, 000h
        je     SHORT loc_3_000F
        db  0CDh, 020h, 002h, 000h, 021h, 000h
loc_3_000F:
        clc
        ret
PageSwap_Invalid_Service1:
        stc
        ret
PageSwap_Grow_File:
        db  0CDh, 020h, 003h, 000h, 021h, 000h
        db  089h, 01Dh, 0D0h, 000h, 000h, 000h
        ret
_PTEXT ends
        end
