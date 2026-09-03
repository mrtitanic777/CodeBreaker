; nwnblink_icode_02.asm -- NWNBLINK object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  088h, 015h, 038h, 079h, 000h, 000h
        clc
        ret
loc_2_0008:
        db  0C7h, 005h, 054h, 077h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 087h, 004h
        jb     SHORT loc_2_002B
        db  083h, 03Dh, 058h, 077h, 000h, 000h, 000h
        je     SHORT loc_2_002B
        db  0E8h, 048h, 066h, 0FEh, 0FFh
        cmp    eax, 1
loc_2_002B:
        ret
loc_2_002C:
        clc
        ret
        db  000h, 000h
_ITEXT ends
        end
