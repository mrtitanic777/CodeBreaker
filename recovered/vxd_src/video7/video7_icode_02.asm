; video7_icode_02.asm -- VIDEO7 object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        push   ebx
        db  089h, 01Dh, 0D8h, 000h, 000h, 000h
        xor    ebx, ebx
        mov    eax, 6f00h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        cmp    bx, 5637h
        db  08Bh, 01Dh, 0D8h, 000h, 000h, 000h
        jne    NEAR PTR loc_2_00F5
        push   0
        push   82h
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        add    esp, 8
        db  0A3h, 0DCh, 000h, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_2_00F5
        db  0CDh, 020h, 00Eh, 000h, 00Ah, 000h
        cmp    ecx, 31h
        jb     NEAR PTR loc_2_00F5
        db  0C7h, 007h, 000h, 000h, 000h, 000h
        db  0C7h, 047h, 004h, 013h, 000h, 000h, 000h
        db  0C7h, 047h, 008h, 000h, 000h, 000h, 000h
        db  0C7h, 047h, 00Ch, 075h, 000h, 000h, 000h
        db  0C7h, 047h, 058h, 01Bh, 001h, 000h, 000h
        db  0C7h, 047h, 05Ch, 078h, 001h, 000h, 000h
        db  0C7h, 047h, 020h, 0B5h, 001h, 000h, 000h
        db  0C7h, 047h, 024h, 037h, 002h, 000h, 000h
        db  0C7h, 047h, 04Ch, 00Dh, 003h, 000h, 000h
        db  0C7h, 047h, 054h, 020h, 004h, 000h, 000h
        db  0C7h, 047h, 048h, 0DAh, 003h, 000h, 000h
        db  0C7h, 047h, 068h, 0A2h, 000h, 000h, 000h
        db  0C7h, 047h, 010h, 0FDh, 002h, 000h, 000h
        db  0C7h, 047h, 014h, 005h, 003h, 000h, 000h
        db  0C7h, 047h, 018h, 0FDh, 002h, 000h, 000h
        db  0C7h, 047h, 01Ch, 005h, 003h, 000h, 000h
        db  0C7h, 087h, 0A8h, 000h, 000h, 000h, 0F8h, 000h, 000h, 000h
        db  0C7h, 087h, 0B4h, 000h, 000h, 000h, 0AAh, 000h, 000h, 000h
        db  0C7h, 087h, 090h, 000h, 000h, 000h, 07Dh, 000h, 000h, 000h
        db  0CDh, 020h, 011h, 000h, 00Ah, 000h
        db  089h, 03Dh, 0E4h, 000h, 000h, 000h
        db  089h, 035h, 0E0h, 000h, 000h, 000h
        clc
        jmp    SHORT loc_2_00F6
loc_2_00F5:
        stc
loc_2_00F6:
        pop    ebx
        ret
loc_2_00F8:
        mov    eax, 5637h
        ret
        db  000h, 000h
_ITEXT ends
        end
