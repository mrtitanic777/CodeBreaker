; compaq_icode_02.asm -- COMPAQ object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  089h, 01Dh, 024h, 002h, 000h, 000h
        push   0
        push   10000h
        push   0a0000h
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     NEAR PTR loc_2_0159
        db  0A3h, 028h, 002h, 000h, 000h
        call   sub_2_015B
        jb     NEAR PTR loc_2_015A
        push   0
        push   8
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        db  0A3h, 044h, 002h, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_2_0159
        db  0CDh, 020h, 00Eh, 000h, 00Ah, 000h
        cmp    ecx, 31h
        jb     NEAR PTR loc_2_0159
        db  0C7h, 007h, 000h, 000h, 000h, 000h
        db  0C7h, 047h, 010h, 027h, 000h, 000h, 000h
        db  0C7h, 047h, 014h, 036h, 000h, 000h, 000h
        db  0C7h, 047h, 018h, 043h, 000h, 000h, 000h
        db  0C7h, 047h, 01Ch, 050h, 000h, 000h, 000h
        db  0C7h, 047h, 020h, 05Dh, 000h, 000h, 000h
        db  0C7h, 047h, 024h, 08Eh, 000h, 000h, 000h
        db  0C7h, 047h, 034h, 07Dh, 000h, 000h, 000h
        db  0C7h, 047h, 038h, 0AAh, 000h, 000h, 000h
        db  0C7h, 087h, 090h, 000h, 000h, 000h, 0B0h, 000h, 000h, 000h
        db  0C7h, 087h, 0A8h, 000h, 000h, 000h, 017h, 003h, 000h, 000h
        db  0C7h, 087h, 0C0h, 000h, 000h, 000h, 0B8h, 000h, 000h, 000h
        db  0C7h, 087h, 0A4h, 000h, 000h, 000h, 0FEh, 005h, 000h, 000h
        db  0C7h, 087h, 090h, 000h, 000h, 000h, 0B0h, 000h, 000h, 000h
        db  0CDh, 020h, 011h, 000h, 00Ah, 000h
        db  089h, 035h, 038h, 002h, 000h, 000h
        db  089h, 03Dh, 03Ch, 002h, 000h, 000h
        db  0BEh, 0D7h, 000h, 000h, 000h
        mov    edx, 63cah
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_2_0159
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        db  0BEh, 0D7h, 000h, 000h, 000h
        mov    edx, 13c8h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_2_0159
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        db  0BEh, 0D7h, 000h, 000h, 000h
        mov    edx, 13c9h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_2_0159
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        db  0BEh, 0D7h, 000h, 000h, 000h
        mov    edx, 83c6h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_2_0159
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        db  0BEh, 0A4h, 001h, 000h, 000h
        mov    edx, 13c6h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_2_0159
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        clc
        jmp    SHORT loc_2_015A
loc_2_0159:
        stc
loc_2_015A:
        ret
sub_2_015B:
        mov    eax, 0c0000h
        mov    ecx, 8000h
        push   0
        push   ecx
        push   eax
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     NEAR PTR loc_2_0315
        cmp    word ptr [eax], 0aa55h
        je     SHORT loc_2_01AD
        mov    eax, 0e0000h
        mov    ecx, 8000h
        push   0
        push   ecx
        push   eax
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     NEAR PTR loc_2_0315
        cmp    word ptr [eax], 0aa55h
        jne    NEAR PTR loc_2_0315
loc_2_01AD:
        movzx  edx, byte ptr [eax + 2]
        shl    edx, 9
        sub    edx, 16h
        cmp    dword ptr [edx + eax], 504d4f43h
        jne    NEAR PTR loc_2_0315
        cmp    word ptr [edx + eax + 4], 5141h
        jne    NEAR PTR loc_2_0315
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        xor    al, al
        out    dx, al
        inc    edx
        in     al, dx
        mov    cl, al
        dec    edx
        mov    al, 3
        out    dx, al
        inc    edx
        in     al, dx
        mov    ch, al
        ror    ecx, 10h
        dec    edx
        mov    al, 0fh
        out    dx, al
        inc    edx
        in     al, dx
        mov    ch, al
        mov    al, 5
        out    dx, al
        mov    edx, 63cah
        mov    al, 0
        out    dx, al
        mov    edx, 3ceh
        mov    al, 10h
        out    dx, al
        inc    edx
        in     al, dx
        mov    cl, al
        dec    edx
        ror    ecx, 10h
        and    al, 0fh
        cmp    cl, al
        jne    SHORT loc_2_022C
        xor    al, al
        mov    ah, cl
        not    ah
        out    dx, ax
        mov    al, 10h
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0fh
        cmp    ah, al
        je     NEAR PTR loc_2_0315
loc_2_022C:
        ror    ecx, 10h
        mov    ah, cl
        or     ah, 8
        mov    al, 10h
        mov    edx, 3ceh
        out    dx, ax
        mov    esi, 33c8h
        test   cl, 4
        je     SHORT loc_2_024B
        mov    si, 53c8h
loc_2_024B:
        ror    ecx, 10h
        mov    ah, ch
        and    ah, 7
        mov    edx, esi
        in     al, dx
        cmp    ah, al
        jne    SHORT loc_2_029A
        not    ah
        and    ah, 7
        mov    al, 3
        mov    edx, 3ceh
        out    dx, ax
        mov    edx, esi
        in     al, dx
        cmp    ah, al
        jne    NEAR PTR loc_2_0315
        xor    ah, ah
        mov    edx, 3ceh
        mov    al, 0ch
        out    dx, al
        inc    dx
        in     al, dx
        test   al, 80h
        jns    SHORT loc_2_02C1
        shr    al, 2
        and    al, 7
        cmp    al, 2
        jg     NEAR PTR loc_2_0315
        db  0C6h, 005h, 04Ah, 002h, 000h, 000h, 041h
        jmp    SHORT loc_2_02E0
loc_2_029A:
        mov    edx, 63c0h
        in     al, dx
        xor    ah, ah
        ror    eax, 10h
        mov    al, 0abh
        out    dx, al
        mov    ah, al
        in     al, dx
        cmp    ah, al
        jne    SHORT loc_2_02BB
        mov    al, 54h
        out    dx, al
        mov    ah, al
        in     al, dx
        cmp    ah, al
        jne    SHORT loc_2_02BB
        jmp    SHORT loc_2_02C1
loc_2_02BB:
        ror    eax, 10h
        out    dx, al
        jmp    SHORT loc_2_0315
loc_2_02C1:
        db  0C6h, 005h, 04Ah, 002h, 000h, 000h, 051h
        shr    al, 3
        and    al, 7
        cmp    al, 7
        je     SHORT loc_2_02E0
        cmp    al, 6
        je     SHORT loc_2_02E0
        db  0C6h, 005h, 04Ah, 002h, 000h, 000h, 041h
        cmp    al, 5
        jl     SHORT loc_2_0315
loc_2_02E0:
        mov    edx, 3dah
        in     al, dx
        mov    edx, 3c0h
        mov    al, 0
        out    dx, al
        mov    edx, 3ceh
        mov    al, 3
        mov    ah, ch
        out    dx, ax
        xor    al, al
        mov    ah, cl
        out    dx, ax
        ror    ecx, 10h
        mov    al, 10h
        mov    ah, cl
        out    dx, ax
        mov    al, 0fh
        mov    ah, ch
        out    dx, ax
        rol    eax, 8
        out    dx, al
        clc
        jmp    SHORT loc_2_0316
loc_2_0315:
        stc
loc_2_0316:
        ret
loc_2_0317:
        db  00Fh, 0B6h, 005h, 04Ah, 002h, 000h, 000h
        ret
        db  000h
_ITEXT ends
        end
