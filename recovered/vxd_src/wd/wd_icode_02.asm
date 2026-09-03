; wd_icode_02.asm -- WD object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        push   ebx
        db  089h, 01Dh, 04Ch, 001h, 000h, 000h
        mov    cl, 5
        push   ecx
        call   sub_2_0103
        pop    ecx
        db  080h, 03Dh, 05Ah, 001h, 000h, 000h, 000h
        jne    SHORT loc_2_0022
        dec    cl
        jne    9
        jmp    NEAR PTR loc_2_0100
loc_2_0022:
        push   0
        push   4
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        add    esp, 8
        db  0A3h, 050h, 001h, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_2_0100
        db  0CDh, 020h, 00Eh, 000h, 00Ah, 000h
        cmp    ecx, 31h
        jb     NEAR PTR loc_2_0100
        db  0C7h, 007h, 000h, 000h, 000h, 000h
        db  0C7h, 047h, 010h, 017h, 000h, 000h, 000h
        db  0C7h, 047h, 018h, 021h, 000h, 000h, 000h
        db  0C7h, 047h, 040h, 060h, 000h, 000h, 000h
        db  0C7h, 047h, 048h, 0BCh, 000h, 000h, 000h
        db  0C7h, 047h, 034h, 0E9h, 000h, 000h, 000h
        db  0C7h, 047h, 038h, 02Ch, 001h, 000h, 000h
        db  0C7h, 087h, 0A8h, 000h, 000h, 000h, 09Ah, 001h, 000h, 000h
        db  0C7h, 087h, 0B0h, 000h, 000h, 000h, 069h, 001h, 000h, 000h
        db  0C7h, 047h, 024h, 000h, 000h, 000h, 000h
        db  0C7h, 047h, 014h, 07Dh, 000h, 000h, 000h
        db  0C7h, 047h, 044h, 07Dh, 000h, 000h, 000h
        db  0C7h, 047h, 04Ch, 0A6h, 000h, 000h, 000h
        mov    edx, 23c0h
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 23c2h
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 23c4h
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    ax, 7fh
        mov    bx, 22ffh
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        cmp    bl, 1
        ja     SHORT loc_2_00FD
        db  088h, 01Dh, 059h, 001h, 000h, 000h
        mov    ax, 7fh
        mov    bh, 23h
        mov    bl, 1
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
loc_2_00FD:
        clc
        jmp    SHORT loc_2_0101
loc_2_0100:
        stc
loc_2_0101:
        pop    ebx
        ret
sub_2_0103:
        db  0C6h, 005h, 05Ah, 001h, 000h, 000h, 000h
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    al, 0ch
        out    dx, al
        inc    edx
        in     al, dx
        mov    cl, al
        and    al, 0bfh
        out    dx, al
        dec    edx
        mov    al, 0fh
        out    dx, al
        inc    edx
        in     al, dx
        mov    ch, al
        xor    al, al
        out    dx, al
        dec    edx
        mov    al, 9
        out    dx, al
        inc    edx
        in     al, dx
        ror    ecx, 10h
        mov    ch, al
        inc    al
        out    dx, al
        mov    ah, al
        in     al, dx
        cmp    al, ah
        je     SHORT loc_2_017E
        dec    edx
        mov    al, 0fh
        out    dx, al
        inc    edx
        mov    al, 5
        out    dx, al
        dec    edx
        mov    al, 9
        out    dx, al
        inc    edx
        in     al, dx
        inc    al
        out    dx, al
        in     al, dx
        dec    al
        cmp    al, ch
        jne    SHORT loc_2_017E
        db  0C6h, 005h, 05Ah, 001h, 000h, 000h, 0FFh
        mov    dl, 0cch
        in     al, dx
        mov    dl, 0d4h
        test   al, 1
        jne    SHORT loc_2_0167
        mov    dl, 0b4h
loc_2_0167:
        in     al, dx
        ror    eax, 8
        mov    al, 37h
        out    dx, al
        inc    edx
        in     al, dx
        or     al, al
        je     SHORT loc_2_0179
        db  0A2h, 05Ah, 001h, 000h, 000h
loc_2_0179:
        dec    edx
        rol    eax, 8
        out    dx, al
loc_2_017E:
        mov    dl, 0ceh
        mov    al, 9
        mov    ah, ch
        out    dx, ax
        ror    ecx, 10h
        mov    al, 0fh
        mov    ah, ch
        out    dx, ax
        mov    al, 0ch
        mov    ah, cl
        out    dx, ax
        rol    eax, 8
        out    dx, al
        ret
loc_2_019A:
        mov    eax, 4457h
        ret
_ITEXT ends
        end
