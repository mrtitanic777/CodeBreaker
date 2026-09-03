; paralink_icode_02.asm -- PARALINK object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  083h, 03Dh, 00Ch, 025h, 000h, 000h, 000h
        jne    SHORT loc_2_0043
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  089h, 01Dh, 00Ch, 025h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 02Bh, 000h
        jb     SHORT loc_2_0044
        mov    eax, 4
        mov    ecx, 2a0h
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     SHORT loc_2_0044
        db  089h, 035h, 008h, 025h, 000h, 000h
        db  068h, 034h, 006h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 02Bh, 000h
        add    esp, 4
loc_2_0043:
        clc
loc_2_0044:
        ret
        db  000h, 000h, 000h
_ITEXT ends
        end
