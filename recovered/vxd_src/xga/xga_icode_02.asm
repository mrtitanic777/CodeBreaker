; xga_icode_02.asm -- XGA object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  089h, 01Dh, 0E8h, 000h, 000h, 000h
        push   0
        push   10000h
        push   0a0000h
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     NEAR PTR loc_2_0203
        db  0A3h, 0ECh, 000h, 000h, 000h
        call   sub_2_0205
        or     ax, dx
        je     NEAR PTR loc_2_0203
        movzx  edi, di
        cmp    edi, 8fdah
        je     SHORT loc_2_004F
        cmp    edi, 8fd9h
        je     SHORT loc_2_004F
        mov    edi, 8fdbh
loc_2_004F:
        db  089h, 03Dh, 010h, 001h, 000h, 000h
        push   0
        push   8
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        add    esp, 8
        db  0A3h, 0F0h, 000h, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_2_0203
        db  0A1h, 000h, 001h, 000h, 000h
        db  066h, 0A3h, 013h, 004h, 000h, 000h
        push   0
        push   15h
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        add    esp, 8
        mov    edi, eax
        mov    esi, eax
        and    eax, 0fh
        and    esi, 0fffffff0h
        shl    esi, 0ch
        or     eax, esi
        db  0A3h, 0F8h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_2_00AE
        db  0BEh, 010h, 004h, 000h, 000h
        mov    ecx, 15h
        rep movsb
loc_2_00AE:
        db  0CDh, 020h, 00Eh, 000h, 00Ah, 000h
        cmp    ecx, 31h
        jb     NEAR PTR loc_2_0203
        db  0C7h, 007h, 000h, 000h, 000h, 000h
        db  0C7h, 087h, 0A8h, 000h, 000h, 000h, 00Ah, 004h, 000h, 000h
        db  0C7h, 047h, 010h, 05Dh, 000h, 000h, 000h
        db  0C7h, 047h, 068h, 017h, 001h, 000h, 000h
        db  0C7h, 087h, 0C0h, 000h, 000h, 000h, 07Eh, 000h, 000h, 000h
        db  0C7h, 087h, 0A4h, 000h, 000h, 000h, 025h, 00Dh, 000h, 000h
        db  0C7h, 087h, 090h, 000h, 000h, 000h, 0F5h, 014h, 000h, 000h
        db  0C7h, 087h, 0ACh, 000h, 000h, 000h, 07Dh, 000h, 000h, 000h
        db  081h, 03Dh, 010h, 001h, 000h, 000h, 0D9h, 08Fh, 000h, 000h
        je     SHORT loc_2_0122
        db  0C7h, 047h, 078h, 0FDh, 014h, 000h, 000h
        db  0C7h, 047h, 07Ch, 0B0h, 015h, 000h, 000h
        jmp    NEAR PTR loc_2_0200
loc_2_0122:
        db  0C7h, 047h, 014h, 076h, 000h, 000h, 000h
        db  0BEh, 024h, 016h, 000h, 000h
        mov    edx, 2160h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     NEAR PTR loc_2_0203
        mov    edx, 2160h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        db  0BEh, 04Ch, 016h, 000h, 000h
        mov    edx, 216ah
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     NEAR PTR loc_2_0203
        mov    edx, 216ah
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        db  0BEh, 04Ch, 016h, 000h, 000h
        mov    edx, 216bh
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     NEAR PTR loc_2_0203
        mov    edx, 216bh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        db  0BEh, 04Ch, 016h, 000h, 000h
        mov    edx, 216ch
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_2_0203
        mov    edx, 216ch
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        db  0BEh, 04Ch, 016h, 000h, 000h
        mov    edx, 216dh
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_2_0203
        mov    edx, 216dh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        db  0BEh, 04Ch, 016h, 000h, 000h
        mov    edx, 216eh
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_2_0203
        mov    edx, 216eh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        db  0BEh, 04Ch, 016h, 000h, 000h
        mov    edx, 216fh
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_2_0203
        mov    edx, 216fh
        db  0CDh, 020h, 097h, 000h, 001h, 000h
loc_2_0200:
        clc
        jmp    SHORT loc_2_0204
loc_2_0203:
        stc
loc_2_0204:
        ret
sub_2_0205:
        cld
        db  0C7h, 005h, 0F4h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    ebx, 1
        mov    ecx, 8
loc_2_021A:
        push   ecx
        push   ebx
        mov    al, bl
        dec    al
        or     al, 8
        out    96h, al
        mov    edx, 100h
        in     al, dx
        mov    bl, al
        inc    edx
        in     al, dx
        mov    bh, al
        mov    di, bx
        inc    edx
        in     al, dx
        mov    cl, al
        inc    edx
        in     al, dx
        mov    ch, al
        mov    si, cx
        inc    edx
        in     al, dx
        mov    ah, al
        inc    edx
        in     al, dx
        pop    ebx
        pop    ecx
        push   eax
        xor    al, al
        out    96h, al
        pop    eax
        call   sub_2_03DF
        jb     NEAR PTR loc_2_02F2
        inc    ebx
        dec    ecx
        jne    SHORT loc_2_021A
        mov    al, 0dfh
        out    94h, al
        mov    edx, 100h
        in     al, dx
        mov    bl, al
        inc    edx
        in     al, dx
        mov    bh, al
        mov    di, bx
        inc    edx
        in     al, dx
        mov    cl, al
        inc    edx
        in     al, dx
        mov    ch, al
        mov    si, cx
        inc    edx
        in     al, dx
        mov    ah, al
        inc    edx
        in     al, dx
        push   eax
        mov    al, 0ffh
        out    94h, al
        pop    eax
        xor    ebx, ebx
        call   sub_2_03DF
        jb     SHORT loc_2_02F2
        mov    dx, 2160h
        mov    al, 0ah
        out    dx, al
        jmp    SHORT loc_2_0297
loc_2_0297:
        in     al, dx
        and    al, 0fh
        cmp    al, 0ah
        jne    NEAR PTR loc_2_03D9
        mov    al, 1
        out    dx, al
        mov    dx, 2168h
        mov    al, 5
        out    dx, al
        jmp    SHORT loc_2_02AE
loc_2_02AE:
        in     al, dx
        cmp    al, 5
        jne    NEAR PTR loc_2_03D9
        mov    di, 8fd9h
        db  0C7h, 005h, 0FCh, 000h, 000h, 000h, 060h, 021h, 000h, 000h
        db  0C7h, 005h, 000h, 001h, 000h, 000h, 068h, 021h, 000h, 000h
        db  0C7h, 005h, 00Ch, 001h, 000h, 000h, 000h, 000h, 080h, 0FDh
        mov    edx, 216ah
        mov    al, 6dh
        mov    ah, 3
        out    dx, ax
        mov    ebx, 0b1f00h
        db  0C6h, 005h, 065h, 001h, 000h, 000h, 000h
        jmp    SHORT loc_2_0354
loc_2_02F2:
        db  088h, 01Dh, 065h, 001h, 000h, 000h
        movzx  ecx, si
        mov    ch, cl
        and    cl, 0eh
        shr    cl, 1
        xor    ebx, ebx
        mov    bl, cl
        mov    al, cl
        shl    bl, 4
        or     ebx, 2100h
        db  089h, 01Dh, 0FCh, 000h, 000h, 000h
        add    bl, 8
        db  089h, 01Dh, 000h, 001h, 000h, 000h
        mov    bl, ah
        and    bl, 0feh
        shl    ebx, 18h
        movzx  edx, al
        shl    edx, 16h
        or     edx, ebx
        db  089h, 015h, 00Ch, 001h, 000h, 000h
        movzx  ebx, cl
        shl    ebx, 7
        add    ebx, 1c00h
        shr    ecx, 0ch
        mov    eax, 2000h
        mul    ecx
        add    ebx, eax
        add    ebx, 0c0000h
loc_2_0354:
        push   0
        push   100h
        push   ebx
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        db  0A3h, 008h, 001h, 000h, 000h
        db  080h, 03Dh, 065h, 001h, 000h, 000h, 000h
        je     SHORT loc_2_03D1
        push   edi
        mov    al, 0dfh
        out    94h, al
        mov    edx, 100h
        in     al, dx
        mov    bl, al
        inc    edx
        in     al, dx
        mov    bh, al
        mov    di, bx
        call   sub_2_03DF
        jae    SHORT loc_2_0393
        call   sub_2_0400
loc_2_0393:
        mov    al, 0ffh
        out    94h, al
        mov    cl, 1
        mov    ch, 8
loc_2_039B:
        db  03Ah, 00Dh, 065h, 001h, 000h, 000h
        je     SHORT loc_2_03CA
        mov    al, cl
        dec    al
        or     al, 8
        out    96h, al
        mov    edx, 100h
        in     al, dx
        mov    bl, al
        inc    edx
        in     al, dx
        mov    bh, al
        mov    di, bx
        call   sub_2_03DF
        jae    SHORT loc_2_03C6
        call   sub_2_0400
loc_2_03C6:
        xor    al, al
        out    96h, al
loc_2_03CA:
        inc    cl
        dec    ch
        jne    SHORT loc_2_039B
        pop    edi
loc_2_03D1:
        mov    ax, 1
        cwd
        jmp    SHORT loc_2_03DE
loc_2_03D9:
        xor    ax, ax
        cwd
loc_2_03DE:
        ret
sub_2_03DF:
        cmp    di, 8fd8h
        je     SHORT loc_2_03FE
        cmp    di, 8fd9h
        je     SHORT loc_2_03FE
        cmp    di, 8fdah
        je     SHORT loc_2_03FE
        cmp    di, 8fdbh
        je     SHORT loc_2_03FE
        clc
        jmp    SHORT loc_2_03FF
loc_2_03FE:
        stc
loc_2_03FF:
        ret
sub_2_0400:
        mov    edx, 102h
        in     al, dx
        and    al, 0feh
        out    dx, al
        ret
loc_2_040A:
        db  0A1h, 010h, 001h, 000h, 000h
        ret
loc_2_0410:
        mov    al, dl
        mov    edx, 0ff0a0000h
        je     SHORT loc_2_0420
        xor    ah, ah
        in     al, dx
        mov    edx, eax
        jmp    SHORT loc_2_0421
loc_2_0420:
        out    dx, al
loc_2_0421:
        mov    eax, 0cb004fh
        add    byte ptr [eax], al
_ITEXT ends
        end
