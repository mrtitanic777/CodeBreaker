; serial_icode_02.asm -- SERIAL object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  083h, 03Dh, 05Ch, 011h, 000h, 000h, 000h
        jne    SHORT loc_2_0056
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  089h, 01Dh, 05Ch, 011h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 02Bh, 000h
        jb     SHORT loc_2_0057
        mov    eax, 4
        mov    ecx, 134h
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     SHORT loc_2_0057
        db  089h, 035h, 058h, 011h, 000h, 000h
        xor    eax, eax
        mov    ecx, 0ch
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     SHORT loc_2_0057
        db  089h, 035h, 05Ch, 000h, 000h, 000h
        db  068h, 030h, 007h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 02Bh, 000h
        pop    ecx
loc_2_0056:
        clc
loc_2_0057:
        ret
_ITEXT ends
        end
