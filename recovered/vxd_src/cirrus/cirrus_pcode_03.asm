; cirrus_pcode_03.asm -- CIRRUS object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        mov    edx, 3cch
        in     al, dx
        db  0A2h, 000h, 001h, 000h, 000h
        mov    eax, dword ptr [ebp + 14h]
        cmp    eax, 0b8000h
        jne    SHORT loc_3_0066
        db  08Bh, 00Dh, 0E8h, 000h, 000h, 000h
        or     ecx, ecx
        jne    SHORT loc_3_005D
        push   0
        push   8000h
        push   eax
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        push   0
        push   80h
        push   92h
        push   8
        push   eax
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        push   0
        push   1
        push   eax
        push   edx
        push   ebx
        db  0CDh, 020h, 078h, 000h, 001h, 000h
        add    esp, 14h
        db  0A3h, 0E8h, 000h, 000h, 000h
loc_3_005D:
        db  0A1h, 0E8h, 000h, 000h, 000h
        mov    word ptr [ebp + 14h], ax
loc_3_0066:
        db  0CDh, 020h, 012h, 000h, 00Ah, 000h
        cmp    word ptr [ebp], 0
        je     SHORT loc_3_0083
        mov    ax, 3c00h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        db  0CDh, 020h, 016h, 000h, 00Ah, 000h
loc_3_0083:
        db  0F7h, 005h, 0ECh, 000h, 000h, 000h, 0C0h, 003h, 01Fh, 000h
        je     SHORT loc_3_00A7
        push   edx
        mov    edx, 3c4h
        in     al, dx
        push   eax
        mov    al, 7
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0f0h
        db  0A2h, 0FFh, 000h, 000h, 000h
        dec    edx
        pop    eax
        out    dx, al
        pop    edx
loc_3_00A7:
        ret
loc_3_00A8:
        mov    ax, 1200h
        mov    bl, 9ah
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        db  066h, 0A3h, 053h, 001h, 000h, 000h
        db  066h, 089h, 01Dh, 055h, 001h, 000h, 000h
        db  066h, 089h, 00Dh, 057h, 001h, 000h, 000h
        db  0F7h, 005h, 0ECh, 000h, 000h, 000h, 000h, 000h, 016h, 000h
        je     SHORT loc_3_00EA
        mov    ax, 1200h
        mov    bl, 0aeh
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        db  066h, 0A3h, 059h, 001h, 000h, 000h
loc_3_00EA:
        db  0C7h, 005h, 04Fh, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0B8h, 005h, 001h, 000h, 000h
        mov    ecx, 4ah
        mov    word ptr [eax], cx
        push   eax
        db  0CDh, 020h, 014h, 000h, 00Ah, 000h
        pop    edi
        test   word ptr [edi + 2], 110h
        jne    NEAR PTR loc_3_039C
        cmp    word ptr [edi + 20h], 0
        je     NEAR PTR loc_3_039C
        push   edi
        mov    ax, 1200h
        mov    bl, 9ah
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    edi
        push   eax
        push   ebx
        push   ecx
        db  0F7h, 005h, 0ECh, 000h, 000h, 000h, 000h, 000h, 016h, 000h
        jne    SHORT loc_3_01AB
        test   word ptr [edi + 2], 200h
        jne    SHORT loc_3_0188
        movzx  ecx, word ptr [edi + 24h]
        movzx  edx, word ptr [edi + 26h]
        mov    esi, edx
        or     esi, ecx
        je     SHORT loc_3_0188
        mov    al, 7
        cmp    edx, 40h
        jae    SHORT loc_3_017A
        mov    al, 6
        cmp    edx, 38h
        jae    SHORT loc_3_017A
        mov    al, 5
        cmp    edx, 30h
        jae    SHORT loc_3_017A
        mov    al, 4
        cmp    edx, 25h
        jae    SHORT loc_3_017A
        mov    al, 3
        cmp    edx, 23h
        jae    SHORT loc_3_017A
        xor    al, al
loc_3_017A:
        push   edi
        mov    ah, 12h
        mov    bl, 0a2h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    edi
loc_3_0188:
        movzx  ecx, word ptr [edi + 20h]
        cmp    word ptr [edi + 18h], 280h
        jne    SHORT loc_3_01AB
        mov    al, 1
        cmp    ecx, 48h
        jae    SHORT loc_3_019D
        xor    al, al
loc_3_019D:
        push   edi
        mov    ah, 12h
        mov    bl, 0a3h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    edi
loc_3_01AB:
        pop    ecx
        pop    ebx
        pop    eax
        mov    al, 3
        and    ah, 40h
        shr    ah, 2
        or     al, ah
        mov    bh, ch
        mov    ch, cl
        shl    ch, 3
        and    ch, 70h
        shr    dh, 6
        and    dh, 3
        or     ch, dh
        movzx  esi, word ptr [edi + 20h]
        movzx  edx, word ptr [edi + 1ah]
        or     edx, edx
        je     NEAR PTR loc_3_039C
        cmp    edx, 1e0h
        je     NEAR PTR loc_3_031E
        cmp    edx, 258h
        je     NEAR PTR loc_3_02D5
        cmp    edx, 300h
        je     SHORT loc_3_0276
        cmp    edx, 360h
        je     SHORT loc_3_025F
        cmp    edx, 400h
        je     SHORT loc_3_020F
        jmp    NEAR PTR loc_3_039C
loc_3_020F:
        db  0F7h, 005h, 0ECh, 000h, 000h, 000h, 000h, 000h, 016h, 000h
        je     SHORT loc_3_022A
        and    ch, 0fh
        or     ch, 40h
        cmp    esi, 55h
        jae    NEAR PTR loc_3_038E
loc_3_022A:
        and    ch, 0fh
        or     ch, 30h
        cmp    esi, 4bh
        jae    NEAR PTR loc_3_038E
        and    ch, 0fh
        or     ch, 20h
        cmp    esi, 46h
        jae    NEAR PTR loc_3_038E
        and    ch, 0fh
        or     ch, 10h
        cmp    esi, 3ch
        jae    NEAR PTR loc_3_038E
        and    ch, 0fh
        jmp    NEAR PTR loc_3_038E
loc_3_025F:
        and    ch, 0f0h
        or     ch, 1
        cmp    esi, 4bh
        jae    NEAR PTR loc_3_038E
        and    ch, 0f0h
        jmp    NEAR PTR loc_3_038E
loc_3_0276:
        db  0F7h, 005h, 0ECh, 000h, 000h, 000h, 000h, 000h, 016h, 000h
        je     SHORT loc_3_0291
        and    bh, 0fh
        or     bh, 50h
        cmp    esi, 55h
        jae    NEAR PTR loc_3_038E
loc_3_0291:
        and    bh, 0fh
        or     bh, 40h
        cmp    esi, 4bh
        jae    NEAR PTR loc_3_038E
        and    bh, 0fh
        or     bh, 30h
        cmp    esi, 48h
        jae    NEAR PTR loc_3_038E
        and    bh, 0fh
        or     bh, 20h
        cmp    esi, 46h
        jae    NEAR PTR loc_3_038E
        and    bh, 0fh
        or     bh, 10h
        cmp    esi, 3ch
        jae    NEAR PTR loc_3_038E
        and    bh, 0fh
        jmp    NEAR PTR loc_3_038E
loc_3_02D5:
        db  0F7h, 005h, 0ECh, 000h, 000h, 000h, 000h, 000h, 016h, 000h
        je     SHORT loc_3_02F0
        and    bh, 0f0h
        or     bh, 4
        cmp    esi, 55h
        jae    NEAR PTR loc_3_038E
loc_3_02F0:
        and    bh, 0f0h
        or     bh, 3
        cmp    esi, 4bh
        jae    NEAR PTR loc_3_038E
        and    bh, 0f0h
        or     bh, 2
        cmp    esi, 48h
        jae    NEAR PTR loc_3_038E
        and    bh, 0f0h
        or     bh, 1
        cmp    esi, 3ch
        jae    SHORT loc_3_038E
        and    bh, 0f0h
        jmp    SHORT loc_3_038E
loc_3_031E:
        db  0F7h, 005h, 0ECh, 000h, 000h, 000h, 000h, 000h, 016h, 000h
        jne    SHORT loc_3_0357
        db  0F7h, 005h, 0ECh, 000h, 000h, 000h, 000h, 0E0h, 001h, 000h
        je     SHORT loc_3_034C
        and    al, 0cfh
        or     al, 20h
        cmp    esi, 4bh
        jae    SHORT loc_3_038E
        and    al, 0cfh
        or     al, 10h
        cmp    esi, 48h
        jae    SHORT loc_3_038E
        and    al, 0cfh
        jmp    SHORT loc_3_038E
loc_3_034C:
        or     al, 10h
        cmp    esi, 4bh
        jae    SHORT loc_3_038E
        and    al, 0efh
        jmp    SHORT loc_3_038E
loc_3_0357:
        or     al, 10h
        cmp    esi, 48h
        jae    SHORT loc_3_0360
        and    al, 0efh
loc_3_0360:
        push   edi
        push   esi
        mov    ah, 12h
        mov    bl, 0a4h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    esi
        mov    al, 2
        cmp    esi, 55h
        jae    SHORT loc_3_0376
loc_3_0376:
        mov    al, 1
        cmp    esi, 4bh
        jae    SHORT loc_3_037F
        mov    al, 0
loc_3_037F:
        mov    ah, 12h
        mov    bl, 0aeh
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    edi
        jmp    SHORT loc_3_039C
loc_3_038E:
        push   edi
        mov    ah, 12h
        mov    bl, 0a4h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    edi
loc_3_039C:
        ret
loc_3_039D:
        db  066h, 0A1h, 057h, 001h, 000h, 000h
        mov    cx, ax
        shl    cx, 0bh
        and    cx, 70h
        mov    bx, ax
        and    bx, 0f000h
        and    ax, 0f00h
        or     bx, ax
        mov    al, 3
        db  066h, 08Bh, 015h, 053h, 001h, 000h, 000h
        and    dx, 4000h
        shr    dx, 0ah
        or     ax, dx
        db  0F7h, 005h, 0ECh, 000h, 000h, 000h, 000h, 0E0h, 001h, 000h
        je     SHORT loc_3_03FF
        db  066h, 08Bh, 015h, 055h, 001h, 000h, 000h
        and    dx, 3000h
        shr    dx, 8
        or     ax, dx
        mov    ah, 12h
        mov    bl, 0a4h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jmp    SHORT loc_3_0437
loc_3_03FF:
        db  0F7h, 005h, 0ECh, 000h, 000h, 000h, 000h, 000h, 016h, 000h
        je     SHORT loc_3_042B
        mov    ah, 12h
        mov    bl, 0a4h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        db  066h, 0A1h, 059h, 001h, 000h, 000h
        mov    ah, 12h
        mov    bl, 0afh
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jmp    SHORT loc_3_0437
loc_3_042B:
        mov    ah, 12h
        mov    bl, 0a4h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
loc_3_0437:
        ret
loc_3_0438:
        db  0C6h, 005h, 0FEh, 000h, 000h, 000h, 000h
        ret
_PTEXT ends
        end
