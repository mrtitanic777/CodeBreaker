; vsd_icode_02.asm -- VSD object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  0BEh, 053h, 000h, 000h, 000h
        db  0CDh, 020h, 02Ah, 000h, 001h, 000h
        push   0
        push   1
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        test   eax, eax
        jne    SHORT loc_2_0021
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
loc_2_0021:
        db  0A3h, 010h, 002h, 000h, 000h
        db  0BEh, 06Ch, 001h, 000h, 000h
        mov    edx, 61h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        mov    edx, 61h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        clc
        ret
        db  020h
_ITEXT ends
        end
