; tseng_icode_02.asm -- TSENG object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  089h, 01Dh, 044h, 001h, 000h, 000h
        db  0C6h, 005h, 061h, 001h, 000h, 000h, 000h
        mov    dx, 3cbh
        in     al, dx
        ror    eax, 8
        mov    al, 11h
        out    dx, al
        in     al, dx
        and    al, 33h
        cmp    al, 11h
        jne    SHORT loc_2_0039
        mov    al, 22h
        out    dx, al
        in     al, dx
        and    al, 33h
        cmp    al, 22h
        jne    SHORT loc_2_0039
        rol    eax, 8
        out    dx, al
        db  0C6h, 005h, 060h, 001h, 000h, 000h, 020h
        jmp    NEAR PTR loc_2_00BF
loc_2_0039:
        rol    eax, 8
        out    dx, al
        mov    dx, 3cdh
        in     al, dx
        ror    eax, 8
        cmp    al, 0ffh
        je     SHORT loc_2_005B
        mov    dx, 3bfh
        mov    al, 3
        out    dx, al
        mov    dx, 3d8h
        mov    al, 0a0h
        out    dx, al
        mov    dx, 3cdh
loc_2_005B:
        mov    al, 11h
        out    dx, al
        in     al, dx
        cmp    al, 11h
        jne    SHORT loc_2_00B6
        mov    al, 22h
        out    dx, al
        in     al, dx
        cmp    al, 22h
        jne    SHORT loc_2_00B6
        push   ebx
        mov    dx, 3d4h
        in     al, dx
        mov    bl, al
        mov    al, 33h
        out    dx, al
        inc    dx
        in     al, dx
        mov    bh, al
        mov    al, 5
        out    dx, al
        dec    dx
        mov    al, 33h
        out    dx, al
        inc    dx
        in     al, dx
        mov    cl, al
        mov    al, 0ah
        out    dx, al
        dec    dx
        mov    al, 33h
        out    dx, al
        inc    dx
        in     al, dx
        mov    ch, al
        mov    al, bh
        out    dx, al
        dec    dx
        mov    al, bl
        out    dx, al
        pop    ebx
        cmp    cx, 0a05h
        jne    SHORT loc_2_00B6
        mov    dx, 3cdh
        rol    eax, 8
        out    dx, al
        db  0C6h, 005h, 060h, 001h, 000h, 000h, 010h
        jmp    SHORT loc_2_00BF
loc_2_00B6:
        rol    eax, 8
        out    dx, al
        jmp    NEAR PTR loc_2_01E8
loc_2_00BF:
        db  0CDh, 020h, 011h, 000h, 00Ah, 000h
        db  089h, 035h, 04Ch, 001h, 000h, 000h
        db  089h, 03Dh, 050h, 001h, 000h, 000h
        push   0
        push   4
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        add    esp, 8
        db  0A3h, 058h, 001h, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_2_01E8
        db  0CDh, 020h, 00Eh, 000h, 00Ah, 000h
        cmp    ecx, 31h
        jb     NEAR PTR loc_2_01E8
        db  0C7h, 007h, 000h, 000h, 000h, 000h
        db  0C7h, 047h, 004h, 085h, 000h, 000h, 000h
        db  0C7h, 047h, 008h, 000h, 000h, 000h, 000h
        db  0C7h, 047h, 00Ch, 054h, 000h, 000h, 000h
        db  0C7h, 047h, 058h, 09Ah, 000h, 000h, 000h
        db  0C7h, 047h, 05Ch, 0D0h, 000h, 000h, 000h
        db  0C7h, 047h, 010h, 0FAh, 000h, 000h, 000h
        db  0C7h, 047h, 014h, 016h, 001h, 000h, 000h
        db  0C7h, 047h, 018h, 01Eh, 001h, 000h, 000h
        db  0C7h, 047h, 01Ch, 026h, 001h, 000h, 000h
        db  0C7h, 047h, 024h, 042h, 001h, 000h, 000h
        db  0C7h, 087h, 084h, 000h, 000h, 000h, 0A3h, 001h, 000h, 000h
        db  0C7h, 087h, 080h, 000h, 000h, 000h, 0C9h, 001h, 000h, 000h
        db  0C7h, 087h, 088h, 000h, 000h, 000h, 0EFh, 001h, 000h, 000h
        db  0C7h, 087h, 0A8h, 000h, 000h, 000h, 010h, 002h, 000h, 000h
        db  080h, 03Dh, 060h, 001h, 000h, 000h, 010h
        je     SHORT loc_2_018C
        db  0C7h, 047h, 044h, 044h, 002h, 000h, 000h
        db  0C7h, 047h, 034h, 075h, 001h, 000h, 000h
        db  0C7h, 047h, 038h, 08Ch, 001h, 000h, 000h
loc_2_018C:
        mov    edx, 3cdh
        db  0B9h, 07Dh, 000h, 000h, 000h
        db  0CDh, 020h, 013h, 000h, 00Ah, 000h
        db  080h, 03Dh, 060h, 001h, 000h, 000h, 010h
        je     SHORT loc_2_01E5
        mov    edx, 3cbh
        db  0B9h, 07Dh, 000h, 000h, 000h
        db  0CDh, 020h, 013h, 000h, 00Ah, 000h
        db  0BEh, 061h, 002h, 000h, 000h
        mov    edx, 217ah
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_2_01E8
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        db  0BEh, 061h, 002h, 000h, 000h
        mov    edx, 217bh
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_2_01E8
        db  0CDh, 020h, 099h, 000h, 001h, 000h
loc_2_01E5:
        clc
        jmp    SHORT loc_2_01E9
loc_2_01E8:
        stc
loc_2_01E9:
        ret
TSENG_INIT_COMPLETE:
        db  080h, 03Dh, 060h, 001h, 000h, 000h, 010h
        jne    SHORT loc_2_020E
        mov    edx, 3ddh
        db  0B9h, 022h, 002h, 000h, 000h
        db  0CDh, 020h, 013h, 000h, 00Ah, 000h
        mov    edx, 3ddh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
loc_2_020E:
        clc
        ret
loc_2_0210:
        db  00Fh, 0B6h, 005h, 060h, 001h, 000h, 000h
        ret
_ITEXT ends
        end
