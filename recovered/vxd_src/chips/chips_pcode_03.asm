; chips_pcode_03.asm -- CHIPS object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        movzx  eax, word ptr [ebp + 14h]
        or     eax, eax
        je     SHORT loc_3_000E
        db  0CDh, 020h, 012h, 000h, 00Ah, 000h
loc_3_000E:
        xor    eax, eax
        db  0A3h, 0D8h, 000h, 000h, 000h
        db  0A3h, 0DCh, 000h, 000h, 000h
        db  0F6h, 005h, 0F4h, 000h, 000h, 000h, 001h
        je     SHORT loc_3_006A
        mov    eax, dword ptr [ebp + 14h]
        shr    eax, 10h
        je     SHORT loc_3_0031
        db  0CDh, 020h, 012h, 000h, 00Ah, 000h
loc_3_0031:
        cmp    word ptr [ebp], 0
        je     SHORT loc_3_004D
        mov    ax, 3c00h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        db  0A3h, 0D8h, 000h, 000h, 000h
        db  0CDh, 020h, 016h, 000h, 00Ah, 000h
loc_3_004D:
        mov    eax, dword ptr [ebp + 14h]
        shr    eax, 10h
        je     SHORT loc_3_0065
        mov    ah, 16h
        mov    al, 0ffh
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        cmp    eax, -1
        jne    SHORT loc_3_0065
        inc    eax
loc_3_0065:
        db  0A3h, 0DCh, 000h, 000h, 000h
loc_3_006A:
        push   eax
        push   edx
        mov    edx, 3d6h
        in     al, dx
        ror    eax, 8
        mov    al, 0bh
        out    dx, al
        in     ax, dx
        db  066h, 0A3h, 0E8h, 000h, 000h, 000h
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        ret
loc_3_0087:
        db  0B8h, 0F5h, 000h, 000h, 000h
        mov    ecx, 4ah
        mov    word ptr [eax], cx
        push   eax
        db  0CDh, 020h, 014h, 000h, 00Ah, 000h
        pop    eax
        test   word ptr [eax + 2], 110h
        jne    NEAR PTR loc_3_01FE
        movzx  edi, word ptr [eax + 20h]
        or     edi, edi
        je     NEAR PTR loc_3_01FE
        movzx  esi, word ptr [eax + 18h]
        or     esi, esi
        je     NEAR PTR loc_3_01FE
        movzx  edx, byte ptr [eax + 1fh]
        xor    eax, eax
        cmp    esi, 280h
        je     SHORT loc_3_0104
        mov    al, 2
        cmp    esi, 320h
        je     SHORT loc_3_0104
        mov    al, 4
        cmp    esi, 400h
        je     SHORT loc_3_0104
        mov    al, 7
        cmp    esi, 480h
        je     SHORT loc_3_0104
        mov    al, 8
        cmp    esi, 500h
        je     SHORT loc_3_0104
        add    al, 0ch
        cmp    esi, 640h
        jne    NEAR PTR loc_3_01FE
loc_3_0104:
        mov    ah, 20h
        cmp    dl, 4
        je     SHORT loc_3_0124
        mov    ah, 30h
        cmp    dl, 8
        je     SHORT loc_3_0124
        mov    ah, 40h
        cmp    dl, 10h
        je     SHORT loc_3_0124
        mov    ah, 50h
        cmp    dl, 18h
        jne    NEAR PTR loc_3_01FE
loc_3_0124:
        or     al, ah
        mov    ah, 0
        push   ebx
        push   eax
        xchg   ebx, eax
        mov    eax, 5f04h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        movzx  eax, ax
        cmp    ax, 15fh
        je     SHORT loc_3_0189
        pop    eax
        pop    ebx
        db  080h, 03Dh, 0F3h, 000h, 000h, 000h, 080h
        jb     NEAR PTR loc_3_01FE
        db  080h, 03Dh, 0F3h, 000h, 000h, 000h, 0A0h
        je     NEAR PTR loc_3_01FE
        and    al, 0fh
        cmp    al, 4
        jne    NEAR PTR loc_3_01FE
        cmp    edi, 3ch
        jb     NEAR PTR loc_3_01FE
        mov    edx, 3d6h
        in     al, dx
        ror    eax, 8
        mov    al, 0fh
        out    dx, al
        in     ax, dx
        or     ah, 40h
        out    dx, ax
        rol    eax, 8
        out    dx, al
        jmp    SHORT loc_3_01FE
loc_3_0189:
        or     bl, bl
        je     SHORT loc_3_01E5
        mov    al, 80h
        test   al, bl
        je     SHORT loc_3_0198
        cmp    edi, 5ah
        jae    SHORT loc_3_01E9
loc_3_0198:
        mov    al, 40h
        test   al, bl
        je     SHORT loc_3_01A3
        cmp    edi, 54h
        jae    SHORT loc_3_01E9
loc_3_01A3:
        mov    al, 20h
        test   al, bl
        je     SHORT loc_3_01AE
        cmp    edi, 4bh
        jae    SHORT loc_3_01E9
loc_3_01AE:
        mov    al, 10h
        test   al, bl
        je     SHORT loc_3_01B9
        cmp    edi, 48h
        jae    SHORT loc_3_01E9
loc_3_01B9:
        mov    al, 8
        test   al, bl
        je     SHORT loc_3_01C4
        cmp    edi, 46h
        jae    SHORT loc_3_01E9
loc_3_01C4:
        mov    al, 4
        test   al, bl
        je     SHORT loc_3_01CF
        cmp    edi, 3ch
        jae    SHORT loc_3_01E9
loc_3_01CF:
        mov    al, 2
        test   al, bl
        je     SHORT loc_3_01DA
        cmp    edi, 38h
        jae    SHORT loc_3_01E9
loc_3_01DA:
        mov    al, 1
        test   al, bl
        je     SHORT loc_3_01E5
        cmp    edi, 19h
        jae    SHORT loc_3_01E9
loc_3_01E5:
        pop    eax
        pop    ebx
        jmp    SHORT loc_3_01FE
loc_3_01E9:
        cmp    al, bh
        pop    ebx
        je     SHORT loc_3_01FD
        mov    bh, al
        mov    eax, 5f05h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
loc_3_01FD:
        pop    ebx
loc_3_01FE:
        ret
loc_3_01FF:
        mov    eax, ecx
        shr    eax, 10h
        or     cx, cx
        je     SHORT loc_3_0223
        db  08Bh, 015h, 0E0h, 000h, 000h, 000h
        shr    edx, 10h
        dec    dl
        cmp    al, dl
        jae    SHORT loc_3_026A
        inc    al
        xor    cx, cx
        add    ecx, 10000h
loc_3_0223:
        db  08Bh, 015h, 0E0h, 000h, 000h, 000h
        sub    edx, ecx
        cmp    edx, 10000h
        mov    edx, 10000h
        ja     SHORT loc_3_023D
        mov    edx, 8000h
loc_3_023D:
        push   edx
        push   ebx
        db  0C6h, 005h, 0EDh, 000h, 000h, 000h, 0FFh
        db  0C6h, 005h, 0F1h, 000h, 000h, 000h, 000h
        mov    bl, al
        mov    bh, al
        and    bl, 0fh
        shl    bl, 4
        db  088h, 01Dh, 0EBh, 000h, 000h, 000h
        and    bh, 30h
        db  088h, 03Dh, 0ECh, 000h, 000h, 000h
        pop    ebx
        pop    edx
        jmp    SHORT loc_3_026C
loc_3_026A:
        mov    edx, ecx
loc_3_026C:
        ret
_PTEXT ends
        end
