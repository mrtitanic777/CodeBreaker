; oak_icode_02.asm -- OAK object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        push   ebx
        db  089h, 01Dh, 0D0h, 000h, 000h, 000h
        db  0C6h, 005h, 01Dh, 001h, 000h, 000h, 000h
        mov    edx, 3deh
        in     al, dx
        ror    eax, 8
        mov    al, 9
        out    dx, al
        inc    edx
        in     al, dx
        ror    eax, 8
        xor    al, al
        out    dx, al
        jmp    SHORT loc_2_0024
loc_2_0024:
        in     al, dx
        or     al, al
        jne    SHORT loc_2_004C
        mov    al, 55h
        out    dx, al
        jmp    SHORT loc_2_002E
loc_2_002E:
        in     al, dx
        cmp    al, 55h
        jne    SHORT loc_2_004C
        dec    edx
        mov    al, 0
        out    dx, al
        inc    dx
        db  0C6h, 005h, 01Dh, 001h, 000h, 000h, 067h
        in     al, dx
        cmp    al, 1
        jne    SHORT loc_2_004C
        db  0C6h, 005h, 01Dh, 001h, 000h, 000h, 087h
loc_2_004C:
        mov    edx, 3deh
        mov    al, 9
        out    dx, al
        rol    eax, 8
        inc    edx
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        db  080h, 03Dh, 01Dh, 001h, 000h, 000h, 000h
        je     NEAR PTR loc_2_015C
        push   0
        push   39h
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        add    esp, 8
        db  0A3h, 0D4h, 000h, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_2_015C
        db  0CDh, 020h, 00Eh, 000h, 00Ah, 000h
        cmp    ecx, 31h
        jb     NEAR PTR loc_2_015C
        db  0C7h, 007h, 000h, 000h, 000h, 000h
        db  0C7h, 047h, 020h, 000h, 000h, 000h, 000h
        db  0C7h, 087h, 0A8h, 000h, 000h, 000h, 05Fh, 001h, 000h, 000h
        db  0C7h, 087h, 080h, 000h, 000h, 000h, 0E0h, 001h, 000h, 000h
        db  0C7h, 087h, 084h, 000h, 000h, 000h, 0E0h, 001h, 000h, 000h
        db  0C7h, 087h, 088h, 000h, 000h, 000h, 019h, 002h, 000h, 000h
        db  0C7h, 087h, 090h, 000h, 000h, 000h, 06Eh, 002h, 000h, 000h
        db  080h, 03Dh, 01Dh, 001h, 000h, 000h, 087h
        je     SHORT loc_2_0117
        db  0C7h, 047h, 024h, 04Bh, 000h, 000h, 000h
        db  0C7h, 047h, 04Ch, 098h, 001h, 000h, 000h
        db  0C7h, 047h, 048h, 0BCh, 001h, 000h, 000h
        db  0C7h, 047h, 018h, 0D4h, 000h, 000h, 000h
        db  0C7h, 047h, 01Ch, 0E9h, 000h, 000h, 000h
        db  0C7h, 047h, 010h, 0D4h, 000h, 000h, 000h
        db  0C7h, 047h, 014h, 0E9h, 000h, 000h, 000h
        db  0C7h, 087h, 0C0h, 000h, 000h, 000h, 0C6h, 000h, 000h, 000h
loc_2_0117:
        mov    edx, 3deh
        db  0B9h, 0FEh, 000h, 000h, 000h
        db  0CDh, 020h, 013h, 000h, 00Ah, 000h
        mov    edx, 3dfh
        db  0B9h, 0FEh, 000h, 000h, 000h
        db  0CDh, 020h, 013h, 000h, 00Ah, 000h
        db  0CDh, 020h, 011h, 000h, 00Ah, 000h
        db  089h, 035h, 0D8h, 000h, 000h, 000h
        db  089h, 03Dh, 0DCh, 000h, 000h, 000h
        db  0BBh, 0E0h, 000h, 000h, 000h
        db  02Bh, 01Dh, 0D4h, 000h, 000h, 000h
        db  0E8h, 0A7h, 0FEh, 0FDh, 0FFh
        clc
        jmp    SHORT loc_2_015D
loc_2_015C:
        stc
loc_2_015D:
        pop    ebx
        ret
loc_2_015F:
        db  00Fh, 0B6h, 005h, 01Dh, 001h, 000h, 000h
        ret
        db  000h
_ITEXT ends
        end
