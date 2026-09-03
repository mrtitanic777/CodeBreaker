; enable4_icode_02.asm -- ENABLE4 object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  0CDh, 020h, 000h, 000h, 037h, 000h
        jb     SHORT loc_2_0055
        db  0CDh, 020h, 000h, 000h, 02Bh, 000h
        jb     SHORT loc_2_0055
        mov    eax, 370001h
        db  0BEh, 008h, 000h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        db  089h, 035h, 01Ch, 029h, 000h, 000h
        jb     SHORT loc_2_0055
        db  0BFh, 05Ah, 029h, 000h, 000h
        mov    dword ptr [edi], 14h
        db  0CDh, 020h, 009h, 000h, 037h, 000h
        jb     SHORT loc_2_0055
        xor    edx, edx
        mov    eax, 64h
        db  0BEh, 0DBh, 000h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 004h, 029h, 000h, 000h
        clc
        ret
loc_2_0055:
        stc
        ret
        db  024h
_ITEXT ends
        end
