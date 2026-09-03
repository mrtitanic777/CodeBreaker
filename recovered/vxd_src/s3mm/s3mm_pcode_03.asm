; s3mm_pcode_03.asm -- S3MM object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        mov    ax, 3c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        db  0A3h, 09Ch, 014h, 000h, 000h
        mov    ax, 3c04h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        db  0A3h, 0A0h, 014h, 000h, 000h
        mov    ax, 3c00h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        db  0A3h, 0A4h, 014h, 000h, 000h
        mov    ecx, dword ptr [ebp + 18h]
        mov    al, 5ah
        mov    ah, cl
        db  066h, 0A3h, 026h, 015h, 000h, 000h
        mov    al, 59h
        mov    ah, ch
        db  066h, 0A3h, 028h, 015h, 000h, 000h
        shr    ecx, 10h
        db  088h, 00Dh, 047h, 015h, 000h, 000h
        db  088h, 02Dh, 02Ah, 015h, 000h, 000h
        mov    eax, dword ptr [ebp + 4]
        shr    eax, 10h
        or     ax, ax
        je     SHORT loc_3_009D
        db  066h, 03Bh, 005h, 0AAh, 014h, 000h, 000h
        je     SHORT loc_3_009D
        db  066h, 0A3h, 0AAh, 014h, 000h, 000h
        db  0CDh, 020h, 012h, 000h, 00Ah, 000h
        db  0F7h, 005h, 0B0h, 014h, 000h, 000h, 002h, 000h, 000h, 000h
        je     SHORT loc_3_009D
        db  00Fh, 0B7h, 005h, 0AAh, 014h, 000h, 000h
        push   0
        push   eax
        db  0FFh, 035h, 084h, 014h, 000h, 000h
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        db  0A3h, 0ACh, 014h, 000h, 000h
loc_3_009D:
        mov    eax, dword ptr [ebp]
        shr    eax, 10h
        or     ax, ax
        je     SHORT loc_3_00BD
        db  066h, 03Bh, 005h, 0A8h, 014h, 000h, 000h
        je     SHORT loc_3_00BD
        db  066h, 0A3h, 0A8h, 014h, 000h, 000h
        db  0CDh, 020h, 012h, 000h, 00Ah, 000h
loc_3_00BD:
        mov    edx, 3cch
        in     al, dx
        db  0A2h, 048h, 015h, 000h, 000h
        ret
loc_3_00C9:
        db  066h, 0C7h, 005h, 039h, 015h, 000h, 000h, 000h, 000h
        db  0B8h, 0DCh, 014h, 000h, 000h
        mov    ecx, 4ah
        mov    word ptr [eax], cx
        push   eax
        db  0CDh, 020h, 014h, 000h, 00Ah, 000h
        pop    eax
        test   word ptr [eax + 2], 110h
        jne    NEAR PTR loc_3_047F
        movzx  edi, word ptr [eax + 20h]
        or     edi, edi
        je     NEAR PTR loc_3_047F
        movzx  esi, word ptr [eax + 18h]
        or     esi, esi
        je     NEAR PTR loc_3_047F
        mov    edx, 3d4h
        in     al, dx
        push   eax
        mov    eax, 0a539h
        out    dx, ax
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 080h, 000h, 000h, 000h
        je     NEAR PTR loc_3_0209
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 0A0h, 000h, 000h, 000h
        je     NEAR PTR loc_3_0262
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 0A0h, 000h, 000h, 000h
        je     NEAR PTR loc_3_0262
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 090h, 000h, 000h, 000h
        je     NEAR PTR loc_3_0262
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 0B0h, 000h, 000h, 000h
        je     NEAR PTR loc_3_0262
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 0C0h, 000h, 000h, 000h
        je     NEAR PTR loc_3_02C3
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 0D0h, 000h, 000h, 000h
        je     NEAR PTR loc_3_02C3
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 000h, 011h, 088h, 000h
        je     NEAR PTR loc_3_02C3
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 040h, 011h, 088h, 000h
        je     NEAR PTR loc_3_02C3
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 000h, 010h, 088h, 000h
        je     NEAR PTR loc_3_02C3
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 000h, 080h, 088h, 000h
        je     NEAR PTR loc_3_02C3
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 000h, 090h, 088h, 000h
        je     NEAR PTR loc_3_02C3
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 000h, 0B0h, 088h, 000h
        je     NEAR PTR loc_3_02C3
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 000h, 0F0h, 088h, 000h
        je     NEAR PTR loc_3_02C3
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 000h, 012h, 088h, 000h
        je     NEAR PTR loc_3_02C3
loc_3_0209:
        mov    al, 41h
        out    dx, al
        inc    dl
        in     al, dx
        and    al, 0f8h
        cmp    esi, 280h
        je     SHORT loc_3_0251
        cmp    esi, 320h
        je     SHORT loc_3_023F
        cmp    esi, 400h
        je     SHORT loc_3_022D
        mov    ah, 7
        jmp    SHORT loc_3_025A
loc_3_022D:
        mov    ah, 7
        cmp    edi, 46h
        jae    SHORT loc_3_025A
        mov    ah, 6
        cmp    edi, 3ch
        jae    SHORT loc_3_025A
        mov    ah, 5
        jmp    SHORT loc_3_025A
loc_3_023F:
        mov    ah, 3
        cmp    edi, 48h
        jae    SHORT loc_3_025A
        mov    ah, 2
        cmp    edi, 3ch
        jae    SHORT loc_3_025A
        mov    ah, 4
        jmp    SHORT loc_3_025A
loc_3_0251:
        mov    ah, 1
        cmp    edi, 48h
        jae    SHORT loc_3_025A
        xor    ah, ah
loc_3_025A:
        or     al, ah
        out    dx, al
        jmp    NEAR PTR loc_3_047C
loc_3_0262:
        mov    ch, 3
        cmp    esi, 280h
        je     SHORT loc_3_0298
        mov    ch, 0ch
        cmp    esi, 320h
        je     SHORT loc_3_0298
        mov    ch, 30h
        cmp    esi, 400h
        je     SHORT loc_3_0298
        mov    ch, 30h
        cmp    esi, 480h
        je     SHORT loc_3_0298
        mov    ch, 0c0h
        cmp    esi, 500h
        jne    NEAR PTR loc_3_047C
loc_3_0298:
        mov    cl, ch
        not    cl
        mov    ah, 0ffh
        cmp    edi, 48h
        jae    SHORT loc_3_02B3
        mov    ah, 0aah
        cmp    edi, 3ch
        jae    SHORT loc_3_02B3
        mov    ah, 55h
        cmp    edi, 38h
        jae    SHORT loc_3_02B3
        xor    ah, ah
loc_3_02B3:
        mov    al, 52h
        out    dx, al
        inc    edx
        in     al, dx
        and    ax, cx
        or     al, ah
        out    dx, al
        jmp    NEAR PTR loc_3_047C
loc_3_02C3:
        mov    ah, 41h
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 000h, 011h, 088h, 000h
        je     SHORT loc_3_02F7
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 000h, 010h, 088h, 000h
        je     SHORT loc_3_02F7
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 040h, 011h, 088h, 000h
        je     SHORT loc_3_02F7
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 000h, 012h, 088h, 000h
        je     SHORT loc_3_02F7
        mov    ah, 5bh
loc_3_02F7:
        cmp    esi, 280h
        jne    SHORT loc_3_0351
        mov    cx, 8f30h
        db  066h, 0C7h, 005h, 039h, 015h, 000h, 000h, 055h, 000h
        cmp    edi, 55h
        jae    SHORT loc_3_0342
        mov    cx, 8f20h
        db  066h, 0C7h, 005h, 039h, 015h, 000h, 000h, 04Bh, 000h
        cmp    edi, 4bh
        jae    SHORT loc_3_0342
        mov    cx, 8f10h
        db  066h, 0C7h, 005h, 039h, 015h, 000h, 000h, 048h, 000h
        cmp    edi, 48h
        jae    SHORT loc_3_0342
        mov    cx, 8f00h
        db  066h, 0C7h, 005h, 039h, 015h, 000h, 000h, 03Ch, 000h
loc_3_0342:
        mov    al, 52h
        out    dx, al
        inc    edx
        in     al, dx
        and    al, ch
        or     al, cl
        out    dx, al
        jmp    NEAR PTR loc_3_047C
loc_3_0351:
        cmp    esi, 320h
        jne    SHORT loc_3_03C8
        mov    cx, 200h
        db  066h, 0C7h, 005h, 039h, 015h, 000h, 000h, 055h, 000h
        cmp    edi, 55h
        jae    SHORT loc_3_03AE
        mov    cx, 180h
        db  066h, 0C7h, 005h, 039h, 015h, 000h, 000h, 04Bh, 000h
        cmp    edi, 4bh
        jae    SHORT loc_3_03AE
        mov    cx, 100h
        db  066h, 0C7h, 005h, 039h, 015h, 000h, 000h, 048h, 000h
        cmp    edi, 48h
        jae    SHORT loc_3_03AE
        mov    cx, 80h
        db  066h, 0C7h, 005h, 039h, 015h, 000h, 000h, 03Ch, 000h
        cmp    edi, 3ch
        jae    SHORT loc_3_03AE
        mov    cx, 0
        db  066h, 0C7h, 005h, 039h, 015h, 000h, 000h, 038h, 000h
loc_3_03AE:
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
        or     al, ch
        out    dx, al
        jmp    NEAR PTR loc_3_047C
loc_3_03C8:
        cmp    esi, 400h
        je     SHORT loc_3_03D8
        cmp    esi, 480h
        jne    SHORT loc_3_042F
loc_3_03D8:
        mov    cl, 14h
        db  066h, 0C7h, 005h, 039h, 015h, 000h, 000h, 055h, 000h
        cmp    edi, 55h
        jae    SHORT loc_3_0423
        mov    cl, 10h
        db  066h, 0C7h, 005h, 039h, 015h, 000h, 000h, 04Bh, 000h
        cmp    edi, 4bh
        jae    SHORT loc_3_0423
        mov    cl, 0ch
        db  066h, 0C7h, 005h, 039h, 015h, 000h, 000h, 046h, 000h
        cmp    edi, 46h
        jae    SHORT loc_3_0423
        mov    cl, 8
        db  066h, 0C7h, 005h, 039h, 015h, 000h, 000h, 03Ch, 000h
        cmp    edi, 3ch
        jae    SHORT loc_3_0423
        mov    cl, 0
        db  066h, 0C7h, 005h, 039h, 015h, 000h, 000h, 02Bh, 000h
loc_3_0423:
        mov    al, ah
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0e3h
        or     al, cl
        out    dx, al
        jmp    SHORT loc_3_047C
loc_3_042F:
        cmp    esi, 500h
        jne    SHORT loc_3_047C
        mov    cl, 0c0h
        db  066h, 0C7h, 005h, 039h, 015h, 000h, 000h, 04Bh, 000h
        cmp    edi, 4bh
        jae    SHORT loc_3_0472
        mov    cl, 0a0h
        db  066h, 0C7h, 005h, 039h, 015h, 000h, 000h, 048h, 000h
        cmp    edi, 48h
        jae    SHORT loc_3_0472
        mov    cl, 80h
        db  066h, 0C7h, 005h, 039h, 015h, 000h, 000h, 03Ch, 000h
        cmp    edi, 3ch
        jae    SHORT loc_3_0472
        mov    cl, 0
        db  066h, 0C7h, 005h, 039h, 015h, 000h, 000h, 02Dh, 000h
loc_3_0472:
        mov    al, ah
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 1fh
        or     al, cl
        out    dx, al
loc_3_047C:
        pop    eax
        dec    edx
        out    dx, al
loc_3_047F:
        db  066h, 083h, 03Dh, 039h, 015h, 000h, 000h, 000h
        je     SHORT loc_3_049D
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        db  066h, 08Bh, 00Dh, 039h, 015h, 000h, 000h
        mov    word ptr [ebx + 18h], cx
loc_3_049D:
        ret
loc_3_049E:
        db  0F6h, 005h, 06Ch, 015h, 000h, 000h, 001h
        je     SHORT loc_3_04C3
        mov    edx, 3d4h
        in     al, dx
        push   eax
        mov    ax, 0a539h
        out    dx, ax
        mov    al, 66h
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0f7h
        out    dx, al
        pop    eax
        mov    edx, 3d4h
        out    dx, al
loc_3_04C3:
        ret
loc_3_04C4:
        db  0C6h, 005h, 03Bh, 015h, 000h, 000h, 000h
        db  0E8h, 0C6h, 004h, 0FDh, 0FFh
        mov    edx, 4ae8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 4ae9h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        db  0F6h, 005h, 033h, 015h, 000h, 000h, 001h
        je     SHORT loc_3_055A
        mov    edx, 3d4h
        in     al, dx
        push   eax
        mov    al, 67h
        out    dx, al
        inc    dx
        in     al, dx
        and    al, 0ch
        cmp    al, 0ch
        jne    SHORT loc_3_0557
        db  080h, 00Dh, 033h, 015h, 000h, 000h, 002h
        db  0A1h, 0ACh, 014h, 000h, 000h
        mov    edx, dword ptr [eax + 81ech]
        db  089h, 015h, 034h, 015h, 000h, 000h
        and    edx, 0fff78000h
        or     edx, 3000h
        db  080h, 03Dh, 031h, 015h, 000h, 000h, 001h
        jne    SHORT loc_3_0535
        or     edx, 80000h
loc_3_0535:
        mov    dword ptr [eax + 81ech], edx
        mov    edx, 3dah
loc_3_0540:
        in     al, dx
        test   al, 8
        jne    SHORT loc_3_0540
loc_3_0545:
        in     al, dx
        test   al, 8
        je     SHORT loc_3_0545
        mov    dx, 3d4h
        mov    al, 67h
        out    dx, al
        inc    dx
        in     al, dx
        and    al, 0f3h
        out    dx, al
loc_3_0557:
        dec    edx
        pop    eax
        out    dx, al
loc_3_055A:
        ret
loc_3_055B:
        push   eax
        push   edi
        db  0BFh, 0BCh, 014h, 000h, 000h
        mov    al, 67h
        cmp    byte ptr [449h], 3
        jbe    SHORT loc_3_056F
        mov    al, 63h
loc_3_056F:
        mov    byte ptr [edi + 0dh], al
        mov    byte ptr [edi + 2], 0
        mov    byte ptr [edi + 5], 0
        mov    byte ptr [edi + 6], 0
        db  0F6h, 005h, 033h, 015h, 000h, 000h, 001h
        je     SHORT loc_3_058B
        mov    byte ptr [edi + 9], 0
loc_3_058B:
        db  0C6h, 005h, 03Dh, 015h, 000h, 000h, 0FFh
        pop    edi
        pop    eax
        ret
loc_3_0595:
        cld
        push   edi
        db  0BFh, 0CCh, 014h, 000h, 000h
        mov    byte ptr [edi + 2], 0
        mov    byte ptr [edi + 5], 0
        mov    byte ptr [edi + 6], 0
        db  0F6h, 005h, 033h, 015h, 000h, 000h, 001h
        je     SHORT loc_3_05B5
        mov    byte ptr [edi + 9], 0
loc_3_05B5:
        pop    edi
        ret
        db  000h
loc_3_05B8:
        push   ebx
        db  0B8h, 074h, 015h, 000h, 000h
        mov    ecx, 4ah
        mov    word ptr [eax], cx
        push   eax
        db  0CDh, 020h, 014h, 000h, 00Ah, 000h
        pop    eax
        test   word ptr [eax + 2], 110h
        jne    NEAR PTR loc_3_078C
        mov    dh, byte ptr [eax + 1fh]
        or     dh, dh
        je     NEAR PTR loc_3_078C
        movzx  edi, word ptr [eax + 20h]
        or     edi, edi
        je     NEAR PTR loc_3_078C
        movzx  edi, word ptr [eax + 18h]
        or     edi, edi
        je     NEAR PTR loc_3_078C
        test   word ptr [eax + 2], 280h
        jne    SHORT loc_3_0638
        xor    dl, dl
        cmp    byte ptr [eax + 30h], 2bh
        je     SHORT loc_3_0615
        cmp    byte ptr [eax + 30h], 2dh
        jne    SHORT loc_3_0638
        mov    dl, 40h
loc_3_0615:
        cmp    byte ptr [eax + 31h], 2bh
        je     SHORT loc_3_0624
        cmp    byte ptr [eax + 31h], 2dh
        jne    SHORT loc_3_0638
        or     dl, 80h
loc_3_0624:
        push   ax
        mov    ah, dl
        mov    dx, 3cch
        in     al, dx
        and    al, 3fh
        or     al, ah
        mov    dx, 3c2h
        out    dx, al
        pop    ax
loc_3_0638:
        push   ax
        mov    dx, 3d4h
        mov    ax, 4838h
        out    dx, ax
        mov    ax, 0a039h
        out    dx, ax
        pop    ax
        mov    dh, byte ptr [eax + 1fh]
        or     dh, dh
        je     NEAR PTR loc_3_0718
        cmp    dh, 10h
        jne    NEAR PTR loc_3_0718
        movzx  edi, word ptr [eax + 18h]
        or     edi, edi
        je     NEAR PTR loc_3_0718
        cmp    edi, 640h
        jne    NEAR PTR loc_3_0718
        db  066h, 08Bh, 015h, 0CAh, 015h, 000h, 000h
        and    dx, 4200h
        je     SHORT loc_3_06C0
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
        jmp    SHORT loc_3_0718
loc_3_06C0:
        db  066h, 08Bh, 015h, 0CAh, 015h, 000h, 000h
        and    dx, 1080h
        je     SHORT loc_3_0718
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
loc_3_0718:
        db  066h, 08Bh, 015h, 0CAh, 015h, 000h, 000h
        and    dx, 1080h
        je     SHORT loc_3_077A
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
loc_3_077A:
        mov    dx, 3d4h
        db  066h, 0A1h, 0C7h, 015h, 000h, 000h
        out    dx, ax
        db  0A0h, 0C9h, 015h, 000h, 000h
        out    dx, al
loc_3_078C:
        pop    ebx
        ret
loc_3_078E:
        push   ebx
        db  0C6h, 005h, 0CEh, 015h, 000h, 000h, 000h
        db  0B8h, 074h, 015h, 000h, 000h
        mov    ecx, 4ah
        mov    word ptr [eax], cx
        push   eax
        db  0CDh, 020h, 014h, 000h, 00Ah, 000h
        pop    eax
        test   word ptr [eax + 2], 110h
        jne    NEAR PTR loc_3_089D
        mov    dh, byte ptr [eax + 1fh]
        or     dh, dh
        je     NEAR PTR loc_3_089D
        movzx  edi, word ptr [eax + 20h]
        or     edi, edi
        je     NEAR PTR loc_3_089D
        movzx  edi, word ptr [eax + 18h]
        or     edi, edi
        je     NEAR PTR loc_3_089D
        mov    dl, 1
        mov    bx, di
        db  0BFh, 0B1h, 002h, 000h, 000h
loc_3_07E4:
        cmp    word ptr [edi], 0
        je     NEAR PTR loc_3_089D
        cmp    word ptr [edi], bx
        je     SHORT loc_3_07FA
        add    edi, 4
        inc    dl
        jmp    SHORT loc_3_07E4
loc_3_07FA:
        movzx  edi, dl
        shl    edi, 10h
        xor    dl, dl
        movzx  esi, word ptr [eax + 20h]
        or     esi, esi
        je     NEAR PTR loc_3_089D
        or     si, dx
        db  0E8h, 052h, 0FAh, 0FCh, 0FFh
        mov    di, si
        xchg   si, dx
        db  088h, 015h, 0CEh, 015h, 000h, 000h
        mov    edx, 3d4h
        in     al, dx
        db  0A2h, 0C9h, 015h, 000h, 000h
        push   eax
        mov    eax, 0a539h
        out    dx, ax
        db  08Bh, 035h, 0C3h, 015h, 000h, 000h
        mov    al, byte ptr [esi + 1]
        mov    ah, al
        mov    al, byte ptr [esi]
        out    dx, al
        mov    bl, al
        inc    dl
        in     al, dx
        mov    bh, al
        db  066h, 089h, 01Dh, 0C7h, 015h, 000h, 000h
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
loc_3_086A:
        mov    al, byte ptr [esi]
        or     al, al
        je     SHORT loc_3_0897
        cmp    bl, al
        jb     SHORT loc_3_0890
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
        jne    SHORT loc_3_0897
loc_3_0890:
        add    esi, 7
        or     al, al
        jne    SHORT loc_3_086A
loc_3_0897:
        or     al, ah
        out    dx, al
        pop    eax
        dec    edx
        out    dx, al
loc_3_089D:
        db  080h, 03Dh, 0CEh, 015h, 000h, 000h, 000h
        je     SHORT loc_3_08BB
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        db  066h, 00Fh, 0B6h, 00Dh, 0CEh, 015h, 000h, 000h
        mov    word ptr [ebx + 18h], cx
loc_3_08BB:
        pop    ebx
        ret
_PTEXT ends
        end
