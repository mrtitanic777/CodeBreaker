; cirruslg_icode_02.asm -- CIRRUSLG object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        push   ebx
        db  089h, 01Dh, 0ACh, 001h, 000h, 000h
        db  0C6h, 005h, 0DCh, 001h, 000h, 000h, 000h
        push   0
        push   10000h
        push   0a0000h
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     NEAR PTR loc_2_01D0
        db  0A3h, 0C0h, 001h, 000h, 000h
        call   sub_2_01D4
        or     ebx, ebx
        je     NEAR PTR loc_2_01D0
        db  089h, 01Dh, 0D4h, 001h, 000h, 000h
        mov    ah, 12h
        mov    bx, 0a7h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        shl    edx, 10h
        mov    dx, cx
        push   0
        push   4000h
        push   edx
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     NEAR PTR loc_2_01D0
        db  0A3h, 0D0h, 001h, 000h, 000h
        mov    ah, 12h
        mov    bl, 85h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        movzx  eax, al
        shl    eax, 10h
        db  0A3h, 0BCh, 001h, 000h, 000h
        db  0CDh, 020h, 011h, 000h, 00Ah, 000h
        db  089h, 035h, 0B4h, 001h, 000h, 000h
        db  089h, 03Dh, 0B8h, 001h, 000h, 000h
        push   0
        push   2ah
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        add    esp, 8
        db  0A3h, 0C4h, 001h, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_2_01D0
        push   ebx
        db  003h, 01Dh, 0C4h, 001h, 000h, 000h
        mov    byte ptr [ebx], 0
        mov    byte ptr [ebx + 1], 0
        mov    byte ptr [ebx + 2], 0
        pop    ebx
        db  0CDh, 020h, 00Eh, 000h, 00Ah, 000h
        cmp    ecx, 31h
        jb     NEAR PTR loc_2_01D0
        db  0C7h, 007h, 000h, 000h, 000h, 000h
        db  0C7h, 047h, 004h, 000h, 000h, 000h, 000h
        db  0C7h, 047h, 008h, 030h, 000h, 000h, 000h
        db  0C7h, 047h, 00Ch, 05Ch, 000h, 000h, 000h
        db  0C7h, 047h, 058h, 096h, 000h, 000h, 000h
        db  0C7h, 047h, 05Ch, 0B9h, 000h, 000h, 000h
        db  0C7h, 047h, 010h, 0D5h, 000h, 000h, 000h
        db  0C7h, 047h, 014h, 021h, 001h, 000h, 000h
        db  0C7h, 047h, 018h, 0BEh, 001h, 000h, 000h
        db  0C7h, 047h, 01Ch, 0CDh, 001h, 000h, 000h
        db  0C7h, 047h, 020h, 0D5h, 001h, 000h, 000h
        db  0C7h, 047h, 024h, 014h, 002h, 000h, 000h
        db  0C7h, 047h, 068h, 038h, 002h, 000h, 000h
        db  0C7h, 087h, 090h, 000h, 000h, 000h, 087h, 002h, 000h, 000h
        db  0C7h, 087h, 080h, 000h, 000h, 000h, 08Eh, 002h, 000h, 000h
        db  0C7h, 087h, 084h, 000h, 000h, 000h, 0D3h, 002h, 000h, 000h
        db  0C7h, 087h, 094h, 000h, 000h, 000h, 0FEh, 002h, 000h, 000h
        db  0C7h, 087h, 088h, 000h, 000h, 000h, 056h, 003h, 000h, 000h
        db  0C7h, 087h, 09Ch, 000h, 000h, 000h, 00Bh, 003h, 000h, 000h
        db  0C7h, 087h, 0A0h, 000h, 000h, 000h, 034h, 003h, 000h, 000h
        db  0C7h, 087h, 0A8h, 000h, 000h, 000h, 028h, 002h, 000h, 000h
        db  0C7h, 047h, 070h, 08Dh, 000h, 000h, 000h
        db  0C7h, 047h, 074h, 0EBh, 001h, 000h, 000h
        db  0C7h, 087h, 0ACh, 000h, 000h, 000h, 058h, 001h, 000h, 000h
        db  0C7h, 087h, 0A4h, 000h, 000h, 000h, 087h, 003h, 000h, 000h
        db  0C7h, 087h, 0BCh, 000h, 000h, 000h, 0CCh, 003h, 000h, 000h
        db  0C7h, 047h, 048h, 0E8h, 000h, 000h, 000h
        db  0C7h, 047h, 04Ch, 026h, 001h, 000h, 000h
        db  0C7h, 087h, 0C0h, 000h, 000h, 000h, 080h, 000h, 000h, 000h
        clc
        jmp    SHORT loc_2_01D1
loc_2_01D0:
        stc
loc_2_01D1:
        pop    ebx
        ret
        db  090h
sub_2_01D4:
        mov    edx, 3c4h
        in     al, dx
        ror    eax, 8
        mov    al, 6
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, al
        ror    eax, 8
        cmp    al, 12h
        jne    SHORT loc_2_0212
        mov    al, 0fah
        out    dx, al
        in     al, dx
        cmp    al, 12h
        jne    SHORT loc_2_0212
        mov    ah, 12h
        mov    bl, 80h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        cmp    ax, 60h
        je     SHORT loc_2_020A
        jmp    SHORT loc_2_0212
loc_2_020A:
        shl    ebx, 10h
        mov    bx, ax
        jmp    SHORT loc_2_0214
loc_2_0212:
        xor    ebx, ebx
loc_2_0214:
        mov    edx, 3c4h
        mov    al, 6
        out    dx, al
        inc    edx
        rol    eax, 8
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        ret
        db  090h
loc_2_0228:
        db  0A1h, 0D4h, 001h, 000h, 000h
        ret
        db  000h, 000h
_ITEXT ends
        end
