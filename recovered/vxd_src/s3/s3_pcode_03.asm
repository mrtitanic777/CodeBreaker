; s3_pcode_03.asm -- S3 object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        mov    ax, 3c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        db  0A3h, 058h, 013h, 000h, 000h
        mov    ax, 3c04h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        db  0A3h, 05Ch, 013h, 000h, 000h
        mov    ax, 3c00h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        db  0A3h, 060h, 013h, 000h, 000h
        mov    ecx, dword ptr [ebp + 18h]
        mov    al, 5ah
        mov    ah, cl
        db  066h, 0A3h, 0D2h, 013h, 000h, 000h
        mov    al, 59h
        mov    ah, ch
        db  066h, 0A3h, 0D4h, 013h, 000h, 000h
        shr    ecx, 10h
        db  088h, 00Dh, 0E6h, 013h, 000h, 000h
        db  088h, 02Dh, 0D6h, 013h, 000h, 000h
        mov    edx, 3cch
        in     al, dx
        db  0A2h, 0E7h, 013h, 000h, 000h
        ret
loc_3_005F:
        db  0B8h, 088h, 013h, 000h, 000h
        mov    ecx, 4ah
        mov    word ptr [eax], cx
        push   eax
        db  0CDh, 020h, 014h, 000h, 00Ah, 000h
        pop    eax
        test   word ptr [eax + 2], 110h
        jne    NEAR PTR loc_3_02F1
        movzx  edi, word ptr [eax + 20h]
        or     edi, edi
        je     NEAR PTR loc_3_02F1
        movzx  esi, word ptr [eax + 18h]
        or     esi, esi
        je     NEAR PTR loc_3_02F1
        mov    edx, 3d4h
        in     al, dx
        push   eax
        mov    eax, 0a539h
        out    dx, ax
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 080h
        je     NEAR PTR loc_3_0142
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 0A0h
        je     NEAR PTR loc_3_019B
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 0A0h
        je     NEAR PTR loc_3_019B
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 090h
        je     NEAR PTR loc_3_019B
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 0B0h
        je     NEAR PTR loc_3_019B
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 0C0h
        je     NEAR PTR loc_3_0211
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 0D0h
        je     NEAR PTR loc_3_0211
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 0E0h
        je     NEAR PTR loc_3_0211
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 011h
        je     NEAR PTR loc_3_0211
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 081h
        je     NEAR PTR loc_3_0211
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 091h
        je     NEAR PTR loc_3_0211
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 0B1h
        je     NEAR PTR loc_3_0211
loc_3_0142:
        mov    al, 41h
        out    dx, al
        inc    dl
        in     al, dx
        and    al, 0f8h
        cmp    esi, 280h
        je     SHORT loc_3_018A
        cmp    esi, 320h
        je     SHORT loc_3_0178
        cmp    esi, 400h
        je     SHORT loc_3_0166
        mov    ah, 7
        jmp    SHORT loc_3_0193
loc_3_0166:
        mov    ah, 7
        cmp    edi, 46h
        jae    SHORT loc_3_0193
        mov    ah, 6
        cmp    edi, 3ch
        jae    SHORT loc_3_0193
        mov    ah, 5
        jmp    SHORT loc_3_0193
loc_3_0178:
        mov    ah, 3
        cmp    edi, 48h
        jae    SHORT loc_3_0193
        mov    ah, 2
        cmp    edi, 3ch
        jae    SHORT loc_3_0193
        mov    ah, 4
        jmp    SHORT loc_3_0193
loc_3_018A:
        mov    ah, 1
        cmp    edi, 48h
        jae    SHORT loc_3_0193
        xor    ah, ah
loc_3_0193:
        or     al, ah
        out    dx, al
        jmp    NEAR PTR loc_3_02EE
loc_3_019B:
        mov    ch, 3
        cmp    esi, 280h
        je     SHORT loc_3_01D1
        mov    ch, 0ch
        cmp    esi, 320h
        je     SHORT loc_3_01D1
        mov    ch, 30h
        cmp    esi, 400h
        je     SHORT loc_3_01D1
        mov    ch, 30h
        cmp    esi, 480h
        je     SHORT loc_3_01D1
        mov    ch, 0c0h
        cmp    esi, 500h
        jne    NEAR PTR loc_3_02EE
loc_3_01D1:
        mov    cl, ch
        not    cl
        mov    ah, 0ffh
        cmp    edi, 48h
        jae    SHORT loc_3_01EC
        mov    ah, 0aah
        cmp    edi, 3ch
        jae    SHORT loc_3_01EC
        mov    ah, 55h
        cmp    edi, 38h
        jae    SHORT loc_3_01EC
        xor    ah, ah
loc_3_01EC:
        mov    al, 52h
        out    dx, al
        inc    edx
        in     al, dx
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 0A0h
        jne    SHORT loc_3_0206
        db  0C6h, 005h, 0E8h, 013h, 000h, 000h, 052h
        db  0A2h, 0E9h, 013h, 000h, 000h
loc_3_0206:
        and    ax, cx
        or     al, ah
        out    dx, al
        jmp    NEAR PTR loc_3_02EE
loc_3_0211:
        mov    ah, 42h
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 0E0h
        je     SHORT loc_3_0227
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 011h
        je     SHORT loc_3_0227
        mov    ah, 5bh
loc_3_0227:
        cmp    esi, 280h
        jne    SHORT loc_3_0254
        mov    cx, 8f20h
        cmp    edi, 4bh
        jae    SHORT loc_3_0245
        mov    cx, 8f10h
        cmp    edi, 48h
        jae    SHORT loc_3_0245
        mov    cx, 8f00h
loc_3_0245:
        mov    al, 52h
        out    dx, al
        inc    edx
        in     al, dx
        and    al, ch
        or     al, cl
        out    dx, al
        jmp    NEAR PTR loc_3_02EE
loc_3_0254:
        cmp    esi, 320h
        jne    SHORT loc_3_0292
        mov    cx, 180h
        cmp    edi, 4bh
        jae    SHORT loc_3_027B
        mov    cx, 100h
        cmp    edi, 48h
        jae    SHORT loc_3_027B
        mov    cx, 80h
        cmp    edi, 3ch
        jae    SHORT loc_3_027B
        mov    cx, 0
loc_3_027B:
        mov    al, 52h
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 7fh
        or     al, cl
        out    dx, al
        dec    edx
        mov    al, ah
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0fch
        or     al, cl
        out    dx, al
        jmp    SHORT loc_3_02EE
loc_3_0292:
        cmp    esi, 400h
        je     SHORT loc_3_02A2
        cmp    esi, 480h
        jne    SHORT loc_3_02C5
loc_3_02A2:
        mov    cl, 10h
        cmp    edi, 4bh
        jae    SHORT loc_3_02B9
        mov    cl, 0ch
        cmp    edi, 46h
        jae    SHORT loc_3_02B9
        mov    cl, 8
        cmp    edi, 3ch
        jae    SHORT loc_3_02B9
        mov    cl, 0
loc_3_02B9:
        mov    al, ah
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0e3h
        or     al, cl
        out    dx, al
        jmp    SHORT loc_3_02EE
loc_3_02C5:
        cmp    esi, 500h
        jne    SHORT loc_3_02EE
        mov    cl, 0c0h
        cmp    edi, 4bh
        jae    SHORT loc_3_02E4
        mov    cl, 0a0h
        cmp    edi, 48h
        jae    SHORT loc_3_02E4
        mov    cl, 80h
        cmp    edi, 3ch
        jae    SHORT loc_3_02E4
        mov    cl, 0
loc_3_02E4:
        mov    al, ah
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 1fh
        or     al, cl
        out    dx, al
loc_3_02EE:
        pop    eax
        dec    edx
        out    dx, al
loc_3_02F1:
        ret
loc_3_02F2:
        db  066h, 083h, 03Dh, 0E8h, 013h, 000h, 000h, 000h
        je     SHORT loc_3_031D
        mov    edx, 3d4h
        in     al, dx
        push   eax
        mov    eax, 0a539h
        out    dx, ax
        db  066h, 0A1h, 0E8h, 013h, 000h, 000h
        out    dx, ax
        db  066h, 0C7h, 005h, 0E8h, 013h, 000h, 000h, 000h, 000h
        pop    eax
        out    dx, al
loc_3_031D:
        ret
loc_3_031E:
        db  0C6h, 005h, 0DAh, 013h, 000h, 000h, 000h
        db  0E8h, 076h, 003h, 0FDh, 0FFh
        mov    edx, 4ae8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 4ae9h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        ret
loc_3_0341:
        push   eax
        push   edi
        db  0BFh, 068h, 013h, 000h, 000h
        mov    al, 67h
        cmp    byte ptr [449h], 3
        jbe    SHORT loc_3_0355
        mov    al, 63h
loc_3_0355:
        mov    byte ptr [edi + 0ch], al
        mov    byte ptr [edi + 2], 0
        mov    byte ptr [edi + 5], 0
        mov    byte ptr [edi + 6], 0
        db  0C6h, 005h, 0DCh, 013h, 000h, 000h, 0FFh
        pop    edi
        pop    eax
        ret
loc_3_036E:
        cld
        push   edi
        db  0BFh, 078h, 013h, 000h, 000h
        mov    byte ptr [edi + 2], 0
        mov    byte ptr [edi + 5], 0
        mov    byte ptr [edi + 6], 0
        pop    edi
        ret
        db  000h
loc_3_0384:
        push   ebx
        db  0B8h, 0F0h, 013h, 000h, 000h
        mov    ecx, 4ah
        mov    word ptr [eax], cx
        push   eax
        db  0CDh, 020h, 014h, 000h, 00Ah, 000h
        pop    eax
        test   word ptr [eax + 2], 110h
        jne    NEAR PTR loc_3_0558
        mov    dh, byte ptr [eax + 1fh]
        or     dh, dh
        je     NEAR PTR loc_3_0558
        movzx  edi, word ptr [eax + 20h]
        or     edi, edi
        je     NEAR PTR loc_3_0558
        movzx  edi, word ptr [eax + 18h]
        or     edi, edi
        je     NEAR PTR loc_3_0558
        test   word ptr [eax + 2], 280h
        jne    SHORT loc_3_0404
        xor    dl, dl
        cmp    byte ptr [eax + 30h], 2bh
        je     SHORT loc_3_03E1
        cmp    byte ptr [eax + 30h], 2dh
        jne    SHORT loc_3_0404
        mov    dl, 40h
loc_3_03E1:
        cmp    byte ptr [eax + 31h], 2bh
        je     SHORT loc_3_03F0
        cmp    byte ptr [eax + 31h], 2dh
        jne    SHORT loc_3_0404
        or     dl, 80h
loc_3_03F0:
        push   ax
        mov    ah, dl
        mov    dx, 3cch
        in     al, dx
        and    al, 3fh
        or     al, ah
        mov    dx, 3c2h
        out    dx, al
        pop    ax
loc_3_0404:
        push   ax
        mov    dx, 3d4h
        mov    ax, 4838h
        out    dx, ax
        mov    ax, 0a039h
        out    dx, ax
        pop    ax
        mov    dh, byte ptr [eax + 1fh]
        or     dh, dh
        je     NEAR PTR loc_3_04E4
        cmp    dh, 10h
        jne    NEAR PTR loc_3_04E4
        movzx  edi, word ptr [eax + 18h]
        or     edi, edi
        je     NEAR PTR loc_3_04E4
        cmp    edi, 640h
        jne    NEAR PTR loc_3_04E4
        db  066h, 08Bh, 015h, 046h, 014h, 000h, 000h
        and    dx, 4200h
        je     SHORT loc_3_048C
        mov    dx, 3d4h
        in     al, dx
        push   ax
        mov    al, 55h
        out    dx, al
        in     ax, dx
        push   ax
        mov    al, 55h
        out    dx, ax
        mov    dl, 0c8h
        in     al, dx
        push   ax
        mov    al, 18h
        out    dx, al
        mov    al, 2
        mov    dl, 0d5h
        out    dx, al
        mov    dl, 0c6h
        in     al, dx
        or     al, 1
        out    dx, al
        xor    al, al
        mov    dl, 0d5h
        out    dx, al
        pop    ax
        mov    dl, 0c8h
        out    dx, al
        pop    ax
        mov    dl, 0d4h
        out    dx, ax
        pop    ax
        out    dx, al
        jmp    SHORT loc_3_04E4
loc_3_048C:
        db  066h, 08Bh, 015h, 046h, 014h, 000h, 000h
        and    dx, 1080h
        je     SHORT loc_3_04E4
        mov    dx, 3d4h
        in     al, dx
        push   ax
        mov    al, 55h
        out    dx, al
        in     ax, dx
        push   ax
        mov    ax, 355h
        out    dx, ax
        mov    dl, 0c7h
        in     al, dx
        push   ax
        sub    al, al
        out    dx, al
        mov    dl, 0c9h
        in     al, dx
        push   ax
        sub    al, al
        out    dx, al
        dec    dx
        in     al, dx
        push   ax
        mov    al, 0ch
        out    dx, al
        mov    dl, 0c6h
        in     al, dx
        or     al, 2
        out    dx, al
        mov    dl, 0c8h
        pop    ax
        out    dx, al
        mov    dl, 0c9h
        pop    ax
        out    dx, al
        mov    dl, 0c7h
        pop    ax
        out    dx, al
        mov    dl, 0d4h
        pop    ax
        out    dx, ax
        pop    ax
        out    dx, al
loc_3_04E4:
        db  066h, 08Bh, 015h, 046h, 014h, 000h, 000h
        and    dx, 1080h
        je     SHORT loc_3_0546
        mov    dx, 3d4h
        in     al, dx
        push   ax
        mov    al, 42h
        out    dx, al
        in     ax, dx
        and    ah, 20h
        mov    cl, ah
        mov    al, 55h
        out    dx, al
        in     ax, dx
        push   ax
        mov    ax, 355h
        out    dx, ax
        mov    dl, 0c7h
        in     al, dx
        push   ax
        sub    al, al
        out    dx, al
        mov    dl, 0c9h
        in     al, dx
        push   ax
        sub    al, al
        out    dx, al
        dec    dx
        in     al, dx
        push   ax
        mov    al, 71h
        out    dx, al
        mov    dl, 0c6h
        in     al, dx
        or     al, cl
        out    dx, al
        mov    dl, 0c8h
        pop    ax
        out    dx, al
        mov    dl, 0c9h
        pop    ax
        out    dx, al
        mov    dl, 0c7h
        pop    ax
        out    dx, al
        mov    dl, 0d4h
        pop    ax
        out    dx, ax
        pop    ax
        out    dx, al
loc_3_0546:
        mov    dx, 3d4h
        db  066h, 0A1h, 043h, 014h, 000h, 000h
        out    dx, ax
        db  0A0h, 045h, 014h, 000h, 000h
        out    dx, al
loc_3_0558:
        pop    ebx
        ret
loc_3_055A:
        push   ebx
        db  0B8h, 0F0h, 013h, 000h, 000h
        mov    ecx, 4ah
        mov    word ptr [eax], cx
        push   eax
        db  0CDh, 020h, 014h, 000h, 00Ah, 000h
        pop    eax
        test   word ptr [eax + 2], 110h
        jne    NEAR PTR loc_3_0662
        mov    dh, byte ptr [eax + 1fh]
        or     dh, dh
        je     NEAR PTR loc_3_0662
        movzx  edi, word ptr [eax + 20h]
        or     edi, edi
        je     NEAR PTR loc_3_0662
        movzx  edi, word ptr [eax + 18h]
        or     edi, edi
        je     NEAR PTR loc_3_0662
        mov    dl, 1
        mov    bx, di
        db  0BFh, 06Dh, 001h, 000h, 000h
loc_3_05A9:
        cmp    word ptr [edi], 0
        je     NEAR PTR loc_3_0662
        cmp    word ptr [edi], bx
        je     SHORT loc_3_05BF
        add    edi, 4
        inc    dl
        jmp    SHORT loc_3_05A9
loc_3_05BF:
        movzx  edi, dl
        shl    edi, 10h
        xor    dl, dl
        movzx  esi, word ptr [eax + 20h]
        or     esi, esi
        je     NEAR PTR loc_3_0662
        or     si, dx
        db  0E8h, 049h, 0FBh, 0FCh, 0FFh
        mov    di, si
        xchg   si, dx
        db  088h, 015h, 04Ah, 014h, 000h, 000h
        mov    edx, 3d4h
        in     al, dx
        db  0A2h, 045h, 014h, 000h, 000h
        push   eax
        mov    eax, 0a539h
        out    dx, ax
        db  08Bh, 035h, 03Fh, 014h, 000h, 000h
        mov    al, byte ptr [esi + 1]
        mov    ah, al
        mov    al, byte ptr [esi]
        out    dx, al
        mov    bl, al
        inc    dl
        in     al, dx
        mov    bh, al
        db  066h, 089h, 01Dh, 043h, 014h, 000h, 000h
        and    ah, al
        add    esi, 2
        xor    ebx, ebx
        mov    bx, di
        shr    ebx, 0bh
        shl    ebx, 2
        add    esi, ebx
        mov    esi, dword ptr [esi]
        mov    bx, di
        xor    bh, bh
loc_3_062F:
        mov    al, byte ptr [esi]
        or     al, al
        je     SHORT loc_3_065C
        cmp    bl, al
        jb     SHORT loc_3_0655
        push   ebx
        push   esi
        mov    ebx, esi
        push   ax
        mov    eax, edi
        shr    eax, 10h
        and    eax, 0fh
        add    ebx, eax
        mov    al, byte ptr [ebx]
        pop    bx
        mov    ah, bh
        cmp    al, 0ffh
        pop    esi
        pop    ebx
        jne    SHORT loc_3_065C
loc_3_0655:
        add    esi, 7
        or     al, al
        jne    SHORT loc_3_062F
loc_3_065C:
        or     al, ah
        out    dx, al
        pop    eax
        dec    edx
        out    dx, al
loc_3_0662:
        pop    ebx
        ret
_PTEXT ends
        end
