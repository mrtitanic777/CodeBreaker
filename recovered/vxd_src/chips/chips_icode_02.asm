; chips_icode_02.asm -- CHIPS object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  089h, 01Dh, 0D0h, 000h, 000h, 000h
        push   0
        push   10000h
        push   0a0000h
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     NEAR PTR loc_2_019E
        db  0A3h, 0D4h, 000h, 000h, 000h
        mov    edx, 3d6h
        in     al, dx
        cmp    al, 0ffh
        je     NEAR PTR loc_2_019E
        push   ebx
        mov    eax, 5f00h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        cmp    ax, 15fh
        mov    ah, bl
        pop    ebx
        jne    NEAR PTR loc_2_017A
        or     ah, ah
        je     NEAR PTR loc_2_019E
        cmp    ah, 0ffh
        je     NEAR PTR loc_2_019E
loc_2_0063:
        mov    al, ah
        and    ax, 0fc03h
        db  0A2h, 0F2h, 000h, 000h, 000h
        db  088h, 025h, 0F3h, 000h, 000h, 000h
        mov    al, 0
        cmp    ah, 0b0h
        jb     SHORT loc_2_0087
        cmp    ah, 0c0h
        je     SHORT loc_2_0087
        cmp    ah, 0d0h
        je     SHORT loc_2_0087
        mov    al, 2
loc_2_0087:
        db  0A2h, 0F4h, 000h, 000h, 000h
        mov    edx, 80000h
        db  080h, 03Dh, 0F3h, 000h, 000h, 000h, 080h
        jb     SHORT loc_2_00BE
        push   edx
        mov    eax, 5f10h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    edx
        cmp    ax, 15fh
        jne    SHORT loc_2_00BE
        db  080h, 00Dh, 0F4h, 000h, 000h, 000h, 001h
        ror    esi, 10h
        mov    si, di
        mov    edx, esi
loc_2_00BE:
        db  089h, 015h, 0E0h, 000h, 000h, 000h
        push   0
        push   103h
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        add    esp, 8
        db  0A3h, 0E4h, 000h, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_2_019E
        db  0CDh, 020h, 00Eh, 000h, 00Ah, 000h
        cmp    ecx, 31h
        jb     NEAR PTR loc_2_019E
        db  0C7h, 007h, 000h, 000h, 000h, 000h
        db  0C7h, 047h, 004h, 0FFh, 001h, 000h, 000h
        db  0C7h, 047h, 008h, 000h, 000h, 000h, 000h
        db  0C7h, 047h, 00Ch, 04Dh, 000h, 000h, 000h
        db  0C7h, 047h, 058h, 0B2h, 000h, 000h, 000h
        db  0C7h, 047h, 05Ch, 0EBh, 000h, 000h, 000h
        db  0C7h, 047h, 014h, 011h, 001h, 000h, 000h
        db  0C7h, 047h, 03Ch, 031h, 002h, 000h, 000h
        db  0C7h, 047h, 02Ch, 05Bh, 001h, 000h, 000h
        db  0C7h, 047h, 030h, 06Fh, 001h, 000h, 000h
        db  0C7h, 047h, 020h, 031h, 001h, 000h, 000h
        db  0C7h, 087h, 0A8h, 000h, 000h, 000h, 0A0h, 001h, 000h, 000h
        db  0C7h, 087h, 090h, 000h, 000h, 000h, 070h, 002h, 000h, 000h
        db  0C7h, 047h, 070h, 087h, 000h, 000h, 000h
        mov    edx, 3d6h
        db  0B9h, 095h, 001h, 000h, 000h
        db  0CDh, 020h, 013h, 000h, 00Ah, 000h
        mov    edx, 3d7h
        db  0B9h, 095h, 001h, 000h, 000h
        db  0CDh, 020h, 013h, 000h, 00Ah, 000h
        clc
        jmp    SHORT loc_2_019F
loc_2_017A:
        mov    dx, 3d6h
        in     al, dx
        ror    eax, 8
        xor    al, al
        out    dx, al
        jmp    SHORT loc_2_0187
loc_2_0187:
        in     ax, dx
        rol    eax, 8
        out    dx, al
        ror    eax, 8
        test   al, al
        jne    SHORT loc_2_019E
        cmp    ah, 0ffh
        je     SHORT loc_2_019E
        jmp    NEAR PTR loc_2_0063
loc_2_019E:
        stc
loc_2_019F:
        ret
loc_2_01A0:
        db  00Fh, 0B6h, 005h, 0F2h, 000h, 000h, 000h
        db  00Ah, 005h, 0F3h, 000h, 000h, 000h
        ret
        db  000h, 000h
_ITEXT ends
        end
