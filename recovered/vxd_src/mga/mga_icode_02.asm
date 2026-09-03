; mga_icode_02.asm -- MGA object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  089h, 01Dh, 08Ch, 00Bh, 000h, 000h
        db  0C7h, 005h, 078h, 00Bh, 000h, 000h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 00Ah, 000h
        cmp    ecx, 30h
        jb     SHORT loc_2_0086
        db  0C7h, 007h, 048h, 005h, 000h, 000h
        db  0C7h, 047h, 010h, 05Bh, 006h, 000h, 000h
        db  0C7h, 047h, 068h, 061h, 006h, 000h, 000h
        db  0C7h, 087h, 0A4h, 000h, 000h, 000h, 0CCh, 006h, 000h, 000h
        db  0B9h, 000h, 000h, 000h, 000h
        mov    edx, 3deh
        db  0CDh, 020h, 013h, 000h, 00Ah, 000h
        jb     SHORT loc_2_0083
        db  0B9h, 000h, 000h, 000h, 000h
        mov    edx, 3dfh
        db  0CDh, 020h, 013h, 000h, 00Ah, 000h
        jb     SHORT loc_2_0083
        push   edx
        mov    dx, 3deh
        in     al, dx
        jmp    SHORT loc_2_0065
loc_2_0065:
        mov    ah, al
        mov    al, 7
        out    dx, al
        jmp    SHORT loc_2_006C
loc_2_006C:
        inc    dx
        in     al, dx
        jmp    SHORT loc_2_0071
loc_2_0071:
        and    eax, 0ffffh
        db  0A3h, 090h, 00Bh, 000h, 000h
        mov    al, ah
        dec    dx
        out    dx, al
        jmp    SHORT loc_2_0082
loc_2_0082:
        pop    edx
loc_2_0083:
        clc
        jmp    SHORT loc_2_0087
loc_2_0086:
        stc
loc_2_0087:
        ret
_ITEXT ends
        end
