; cirruslg_pcode_03.asm -- CIRRUSLG object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        mov    edx, 3cch
        in     al, dx
        db  0A2h, 0E4h, 001h, 000h, 000h
        cmp    word ptr [ebp], 0
        je     SHORT loc_3_008C
        mov    ax, 3c00h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        db  0A3h, 0C8h, 001h, 000h, 000h
        db  0CDh, 020h, 016h, 000h, 00Ah, 000h
        mov    eax, dword ptr [ebp + 14h]
        db  0CDh, 020h, 012h, 000h, 00Ah, 000h
        mov    eax, dword ptr [ebp + 14h]
        push   0
        db  0FFh, 035h, 0ACh, 001h, 000h, 000h
        push   eax
        db  0CDh, 020h, 07Bh, 000h, 001h, 000h
        add    esp, 0ch
        mov    al, dh
        rol    eax, 10h
        and    eax, 0ffffffh
        and    edx, 0ff000000h
        add    eax, edx
        add    eax, 800000h
        push   0
        push   50000h
        push   eax
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        db  00Fh, 084h, 05Ah, 001h, 0FDh, 0FFh
        db  0A3h, 0CCh, 001h, 000h, 000h
        mov    eax, dword ptr [ebp + 4]
        db  0CDh, 020h, 012h, 000h, 00Ah, 000h
        mov    eax, dword ptr [ebp + 18h]
        db  0A3h, 0E9h, 001h, 000h, 000h
loc_3_008C:
        ret
loc_3_008D:
        mov    ax, 1200h
        mov    bl, 9ah
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        db  066h, 0A3h, 027h, 003h, 000h, 000h
        db  066h, 089h, 00Dh, 029h, 003h, 000h, 000h
        db  066h, 089h, 015h, 02Bh, 003h, 000h, 000h
        db  0C7h, 005h, 0D8h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    edx, 3c4h
        mov    al, 15h
        out    dx, al
        in     ax, dx
        or     ah, 80h
        out    dx, ax
        db  0B8h, 0F0h, 001h, 000h, 000h
        mov    ecx, 4ah
        mov    word ptr [eax], cx
        push   eax
        db  0CDh, 020h, 014h, 000h, 00Ah, 000h
        pop    edi
        test   word ptr [edi + 2], 110h
        jne    NEAR PTR loc_3_01EA
        push   edi
        mov    ah, 12h
        mov    bl, 9ah
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    edi
        mov    al, cl
        shr    al, 4
        and    al, 0fh
        mov    ah, dl
        shl    ah, 4
        or     al, ah
        mov    bh, ch
        shr    dl, 4
        and    dl, 0fh
        mov    ch, dl
        shl    cl, 3
        or     ch, cl
        movzx  esi, word ptr [edi + 20h]
        movzx  edx, word ptr [edi + 1ah]
        or     edx, edx
        je     NEAR PTR loc_3_01EA
        cmp    edx, 1e0h
        je     NEAR PTR loc_3_01C3
        cmp    edx, 258h
        je     SHORT loc_3_019E
        cmp    edx, 300h
        je     SHORT loc_3_017B
        cmp    edx, 400h
        je     SHORT loc_3_0156
        and    ch, 0f0h
        mov    ah, 1
        cmp    esi, 3ch
        jae    SHORT loc_3_0177
        xor    ah, ah
        jmp    SHORT loc_3_0177
loc_3_0156:
        and    ch, 0fh
        mov    ah, 40h
        cmp    esi, 55h
        jae    SHORT loc_3_0177
        mov    ah, 30h
        cmp    esi, 4bh
        jae    SHORT loc_3_0177
        mov    ah, 20h
        cmp    esi, 47h
        jae    SHORT loc_3_0177
        mov    ah, 10h
        cmp    esi, 3ch
        jae    SHORT loc_3_0177
        xor    ah, ah
loc_3_0177:
        or     ch, ah
        jmp    SHORT loc_3_01DE
loc_3_017B:
        and    bh, 0fh
        mov    ah, 50h
        cmp    esi, 55h
        jae    SHORT loc_3_01BF
        mov    ah, 40h
        cmp    esi, 4bh
        jae    SHORT loc_3_01BF
        mov    ah, 20h
        cmp    esi, 46h
        jae    SHORT loc_3_01BF
        mov    ah, 10h
        cmp    esi, 3ch
        jae    SHORT loc_3_01BF
        xor    ah, ah
        jmp    SHORT loc_3_01BF
loc_3_019E:
        and    bh, 0f0h
        mov    ah, 4
        cmp    esi, 55h
        jae    SHORT loc_3_01BF
        dec    ah
        cmp    esi, 4bh
        jae    SHORT loc_3_01BF
        dec    ah
        cmp    esi, 48h
        jae    SHORT loc_3_01BF
        dec    ah
        cmp    esi, 3ch
        jae    SHORT loc_3_01BF
        xor    ah, ah
loc_3_01BF:
        or     bh, ah
        jmp    SHORT loc_3_01DE
loc_3_01C3:
        and    al, 0fh
        mov    ah, 30h
        cmp    esi, 55h
        jae    SHORT loc_3_01DC
        mov    ah, 20h
        cmp    esi, 4bh
        jae    SHORT loc_3_01DC
        mov    ah, 10h
        cmp    esi, 48h
        jae    SHORT loc_3_01DC
        xor    ah, ah
loc_3_01DC:
        or     al, ah
loc_3_01DE:
        mov    ah, 12h
        mov    bl, 0a4h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
loc_3_01EA:
        ret
loc_3_01EB:
        db  066h, 0A1h, 029h, 003h, 000h, 000h
        mov    cx, ax
        shl    cx, 0bh
        and    cx, 70h
        mov    bx, ax
        and    bx, 0f000h
        and    ax, 0f00h
        or     bx, ax
        db  066h, 08Bh, 015h, 02Bh, 003h, 000h, 000h
        shl    dx, 4
        and    dx, 0f00h
        or     cx, dx
        db  066h, 0A1h, 027h, 003h, 000h, 000h
        shl    al, 4
        and    al, 0f0h
        or     al, 4
        mov    ah, 12h
        mov    bl, 0a4h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        ret
loc_3_0238:
        push   eax
        push   edx
        mov    ax, 3
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        db  08Bh, 015h, 0D0h, 001h, 000h, 000h
        and    word ptr [edx + 0e6h], 0fffeh
        mov    eax, dword ptr [edx + 3fch]
        and    eax, 0efffffffh
        mov    dword ptr [edx + 3f4h], eax
        mov    dword ptr [edx + 3fch], eax
        and    word ptr [edx + 0f0h], 0efffh
        mov    word ptr [edx + 0c0h], 0
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    byte ptr [ebx + 1dh], 12h
        mov    byte ptr [ebx + 10h], 0b5h
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    ax, word ptr [ebx + 1ch]
        and    ah, 3fh
        mov    al, byte ptr [edx + 0eah]
        and    al, 0c0h
        and    al, 7fh
        or     ah, al
        db  0E8h, 012h, 002h, 0FDh, 0FFh
        mov    byte ptr [edx + 0eah], ah
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    ebx
        mov    edx, 3c4h
        mov    al, 15h
        out    dx, al
        in     ax, dx
        and    ah, 7fh
        out    dx, ax
        pop    edx
        pop    eax
        db  0C6h, 005h, 0E0h, 001h, 000h, 000h, 000h
        ret
_PTEXT ends
        end
