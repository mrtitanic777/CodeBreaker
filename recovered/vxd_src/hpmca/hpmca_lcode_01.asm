; hpmca_lcode_01.asm -- HPMCA object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  052h, 00Fh, 0B7h, 054h, 024h, 008h, 033h, 0C0h, 0ECh, 05Ah, 0C3h, 090h
sub_1_000C:
        push   edx
        movzx  edx, word ptr [esp + 8]
        mov    al, byte ptr [esp + 0ch]
        out    dx, al
        in     al, 61h
        pop    edx
        ret
        db  090h
sub_1_001C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edx
        push   ecx
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        movzx  edx, word ptr [edi + 2]
        mov    word ptr [edi + 34h], 0
        mov    ax, cx
        and    al, 0feh
        mov    word ptr [edi + 36h], ax
        test   cx, 1
        je     SHORT loc_1_0053
        add    esi, ecx
        mov    ah, 0ffh
        mov    al, byte ptr [esi - 1]
        sub    esi, ecx
        mov    word ptr [edi + 34h], ax
loc_1_0053:
        test   byte ptr [edi + 8], 2
        je     SHORT loc_1_0074
        xor    dl, 8
        movzx  eax, byte ptr [edi + 3bh]
        xchg   al, ah
        out    dx, ax
        xor    dl, 4
        shr    cx, 1
        rep outsw
        xor    eax, eax
        jmp    NEAR PTR loc_1_010D
loc_1_0074:
        xor    dl, 1ah
        mov    al, 4
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 2
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 8
        push   0ah
        push   edx
        call   sub_1_000C
        add    esp, 8
        push   ecx
        mov    ecx, 40h
        xor    dl, 8
loc_1_00A7:
        in     al, dx
        cmp    al, 4
        loope  loc_1_00A7
        pop    ecx
        xor    eax, eax
        xor    dl, 0
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        movzx  eax, byte ptr [edi + 3bh]
        xor    dl, 1
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        mov    ax, cx
        and    al, 0feh
        xor    dl, 3
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xchg   al, ah
        xor    dl, 1
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 0bh
        push   12h
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 1ch
        cld
        shr    cx, 1
        rep outsw
        call   sub_1_07C3
        jne    SHORT loc_1_0113
loc_1_010D:
        pop    edi
        pop    ecx
        pop    edx
        pop    esi
        leave
        ret
loc_1_0113:
        mov    word ptr [edi + 34h], 0
        mov    word ptr [edi + 36h], 0
        jmp    SHORT loc_1_010D
        db  02Eh, 08Bh, 0C0h
sub_1_0124:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   edx
        push   ecx
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        movzx  edx, word ptr [edi + 2]
        xor    ecx, ecx
        mov    eax, dword ptr [ebp + 10h]
        or     ecx, eax
        je     NEAR PTR loc_1_0246
        movzx  ebx, word ptr [edi + 34h]
        or     ebx, ebx
        je     SHORT loc_1_014D
        dec    ecx
        inc    eax
loc_1_014D:
        and    ax, 0fffeh
        je     NEAR PTR loc_1_0250
        add    word ptr [edi + 36h], ax
        test   byte ptr [edi + 8], 2
        je     NEAR PTR loc_1_01E4
        test   byte ptr [edi + 8], 4
        je     SHORT loc_1_01B3
        push   edi
        mov    edi, dword ptr [edi + 20h]
        xor    dl, 4
        in     ax, dx
        and    ax, 0efffh
        out    dx, ax
        push   eax
        or     ebx, ebx
        je     SHORT loc_1_0187
        mov    bh, byte ptr [esi]
        inc    esi
        mov    word ptr [edi], bx
        xor    ebx, ebx
loc_1_0187:
        mov    eax, ecx
        shr    ecx, 2
        jecxz  loc_1_0192
        cld
        nop
        rep movsd
loc_1_0192:
        test   al, 2
        je     SHORT loc_1_0198
        movsw
loc_1_0198:
        test   al, 1
        je     SHORT loc_1_01A0
        mov    bh, 0ffh
        mov    bl, byte ptr [esi]
loc_1_01A0:
        pop    eax
        or     ax, 1000h
        out    dx, ax
        pop    edi
        mov    word ptr [edi + 34h], bx
        xor    eax, eax
        jmp    NEAR PTR loc_1_0246
loc_1_01B3:
        xor    dl, 0ch
        or     ebx, ebx
        je     SHORT loc_1_01C3
        mov    al, bl
        mov    ah, byte ptr [esi]
        inc    esi
        out    dx, ax
        xor    ebx, ebx
loc_1_01C3:
        mov    eax, ecx
        shr    ecx, 2
        jecxz  loc_1_01CE
        cld
        nop
        rep outsd
loc_1_01CE:
        test   al, 2
        je     SHORT loc_1_01D4
        outsw
loc_1_01D4:
        test   al, 1
        je     SHORT loc_1_01DC
        mov    bh, 0ffh
        mov    bl, byte ptr [esi]
loc_1_01DC:
        mov    word ptr [edi + 34h], bx
        xor    eax, eax
        jmp    SHORT loc_1_0246
loc_1_01E4:
        xor    dl, 1ah
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xchg   al, ah
        xor    dl, 1
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 0bh
        push   12h
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 1ch
        or     ebx, ebx
        je     SHORT loc_1_021E
        mov    al, bl
        mov    ah, byte ptr [esi]
        inc    esi
        out    dx, ax
        xor    ebx, ebx
loc_1_021E:
        mov    eax, ecx
        shr    ecx, 2
        jecxz  loc_1_022D
        cld
        mov    eax, eax
        shl    ecx, 1
        rep outsw
loc_1_022D:
        test   al, 2
        je     SHORT loc_1_0233
        outsw
loc_1_0233:
        test   al, 1
        je     SHORT loc_1_023B
        mov    bh, 0ffh
        mov    bl, byte ptr [esi]
loc_1_023B:
        mov    word ptr [edi + 34h], bx
        call   sub_1_07C3
        jne    SHORT loc_1_025C
loc_1_0246:
        pop    ebx
        pop    ecx
        pop    edx
        pop    esi
        pop    edi
        leave
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_0250:
        mov    ah, 0ffh
        mov    al, byte ptr [esi]
        mov    word ptr [edi + 34h], ax
        xor    eax, eax
        jmp    SHORT loc_1_0246
loc_1_025C:
        mov    word ptr [edi + 34h], 0
        mov    word ptr [edi + 36h], 0
        jmp    SHORT loc_1_0246
        db  08Bh, 0C0h
sub_1_026C:
        push   ebp
        mov    ebp, esp
        push   edi
        push   edx
        push   ecx
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        xor    eax, eax
        xor    ebx, ebx
        movzx  ecx, word ptr [edi + 36h]
        or     ecx, ecx
        je     NEAR PTR loc_1_031D
        xchg   word ptr [edi + 34h], bx
        or     ebx, ebx
        je     SHORT loc_1_0290
        inc    ecx
        inc    eax
loc_1_0290:
        cmp    ecx, 3ch
        jge    SHORT loc_1_029F
        sub    ecx, 3ch
        sub    eax, ecx
        mov    ecx, 3ch
loc_1_029F:
        or     eax, eax
        je     SHORT loc_1_031D
        inc    eax
        and    al, 0feh
        mov    word ptr [edi + 36h], cx
        movzx  ecx, ax
        movzx  edx, word ptr [edi + 2]
        test   byte ptr [edi + 8], 2
        je     SHORT loc_1_02D1
        xor    dl, 0ch
        shr    ecx, 1
        or     ebx, ebx
        je     SHORT loc_1_02C9
        movzx  eax, bl
        out    dx, ax
        loop   loc_1_02C9
        jmp    SHORT loc_1_031D
loc_1_02C9:
        xor    eax, eax
loc_1_02CB:
        out    dx, ax
        loop   loc_1_02CB
        jmp    SHORT loc_1_031D
loc_1_02D1:
        xor    dl, 1ah
        movzx  eax, cx
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xchg   al, ah
        xor    dl, 1
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 0bh
        push   12h
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 1ch
        shr    ecx, 1
        or     ebx, ebx
        je     SHORT loc_1_0310
        movzx  eax, bl
        out    dx, ax
        loop   loc_1_0310
        jmp    SHORT loc_1_0316
loc_1_0310:
        xor    eax, eax
loc_1_0312:
        out    dx, ax
        loop   loc_1_0312
loc_1_0316:
        call   sub_1_07C3
        jne    SHORT loc_1_031F
loc_1_031D:
        xor    eax, eax
loc_1_031F:
        pop    ebx
        pop    ecx
        pop    edx
        pop    edi
        leave
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_0328:
        push   edx
        movzx  edx, word ptr [esp + 8]
        xor    dl, 10h
        push   62h
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 7
        in     al, dx
        xchg   ah, al
        xor    dl, 7
        push   22h
        push   edx
        call   sub_1_000C
        add    esp, 8
        movzx  eax, ah
        pop    edx
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_0358:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   edx
        push   ecx
        push   ebx
        mov    ecx, dword ptr [ebp + 10h]
        mov    edi, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        movzx  edx, word ptr [esi + 2]
        mov    eax, dword ptr [esi + 24h]
        xchg   al, ah
        mov    ebx, 0
        test   byte ptr [esi + 8], 2
        je     SHORT loc_1_0397
        xor    dl, 0ah
        out    dx, ax
        xor    dl, 6
        in     eax, dx
        mov    dword ptr [esi + 2ch], eax
        shr    cx, 1
        rep insw
        jae    SHORT loc_1_0407
        in     ax, dx
        mov    byte ptr [edi], al
        jmp    SHORT loc_1_0407
loc_1_0397:
        xor    dl, 18h
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xchg   ah, al
        xor    dl, 1
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 3
        mov    ax, cx
        add    ax, 4
        inc    ax
        and    al, 0feh
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xchg   al, ah
        xor    dl, 1
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 0bh
        push   0ah
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 1ch
        in     eax, dx
        mov    dword ptr [esi + 2ch], eax
        cld
        shr    cx, 1
        rep insw
        jae    SHORT loc_1_03FC
        in     ax, dx
        mov    byte ptr [edi], al
loc_1_03FC:
        call   sub_1_07C3
        jne    NEAR PTR loc_1_05BE
loc_1_0407:
        movzx  eax, word ptr [esi + 2ch]
        test   al, 48h
        jne    NEAR PTR loc_1_05F8
        test   al, 1
        je     NEAR PTR loc_1_04D2
loc_1_041B:
        shr    ax, 8
        mov    ecx, dword ptr [esi + 24h]
        sub    ax, cx
        dec    ax
        jns    SHORT loc_1_0436
        mov    al, byte ptr [esi + 0fh]
        sub    al, cl
        add    al, byte ptr [esi + 2dh]
        sub    al, byte ptr [esi + 0eh]
        dec    al
loc_1_0436:
        shl    ax, 8
        mov    al, byte ptr [esi + 2eh]
        cmp    al, 0fch
        jbe    SHORT loc_1_0443
        dec    ah
loc_1_0443:
        sub    ax, 4
        mov    word ptr [esi + 2eh], ax
        or     ebx, ebx
        jne    SHORT loc_1_047C
        cmp    ax, 5eah
        ja     NEAR PTR loc_1_0516
        cmp    ax, 3ch
        jb     NEAR PTR loc_1_0516
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [edi]
        movzx  edx, word ptr [edi + 4]
        mov    edi, dword ptr [esi + 10h]
        test   eax, 1
        jne    NEAR PTR loc_1_0535
        jmp    SHORT loc_1_048C
loc_1_047C:
        movzx  eax, word ptr [esi + 2eh]
loc_1_0480:
        mov    edi, dword ptr [ebp + 14h]
        mov    dword ptr [edi], ebx
        pop    ebx
        pop    ecx
        pop    edx
        pop    esi
        pop    edi
        leave
        ret
loc_1_048C:
        cmp    eax, dword ptr [edi]
        jne    SHORT loc_1_04BA
        cmp    dx, word ptr [edi + 4]
        jne    SHORT loc_1_04BA
        test   dword ptr [esi + 0c2h], 1
        jne    SHORT loc_1_047C
        mov    ebx, 100h
        test   dword ptr [esi + 0c2h], 10h
        jne    SHORT loc_1_047C
        mov    eax, 0
        jmp    SHORT loc_1_0480
loc_1_04BA:
        mov    ebx, 104h
        test   dword ptr [esi + 0c2h], 10h
        jne    SHORT loc_1_047C
        mov    eax, 0
        jmp    SHORT loc_1_0480
loc_1_04D2:
        mov    ebx, 206h
        and    eax, 6
        cmp    eax, 6
        je     SHORT loc_1_04FC
        mov    ebx, 202h
        test   eax, 2
        jne    SHORT loc_1_04FC
        mov    ebx, 204h
        test   eax, 4
        jne    SHORT loc_1_04FC
        mov    ebx, 201h
loc_1_04FC:
        test   dword ptr [esi + 0c2h], 20h
        jne    NEAR PTR loc_1_041B
        mov    eax, 0
        jmp    NEAR PTR loc_1_0480
loc_1_0516:
        mov    ebx, 208h
        test   dword ptr [esi + 0c2h], 20h
        jne    NEAR PTR loc_1_047C
        mov    eax, 0
        jmp    NEAR PTR loc_1_0480
loc_1_0535:
        cmp    eax, -1
        jne    SHORT loc_1_0574
        cmp    dx, -1
        jne    SHORT loc_1_0574
        mov    ebx, 1
        test   dword ptr [esi + 0c2h], 2
        jne    NEAR PTR loc_1_047C
        mov    ebx, 101h
        test   dword ptr [esi + 0c2h], 10h
        jne    NEAR PTR loc_1_047C
        mov    eax, 0
        jmp    NEAR PTR loc_1_0480
loc_1_0574:
        mov    ecx, dword ptr [esi + 14h]
        jecxz  loc_1_059B
loc_1_0579:
        add    edi, 6
        cmp    eax, dword ptr [edi]
        jne    SHORT loc_1_05BA
        cmp    dx, word ptr [edi + 4]
        jne    SHORT loc_1_05BA
        mov    ebx, 2
        test   dword ptr [esi + 0c2h], 4
        jne    NEAR PTR loc_1_047C
loc_1_059B:
        mov    ebx, 102h
        test   dword ptr [esi + 0c2h], 18h
        jne    NEAR PTR loc_1_047C
        mov    eax, 0
        jmp    NEAR PTR loc_1_0480
loc_1_05BA:
        loop   loc_1_0579
        jmp    SHORT loc_1_059B
loc_1_05BE:
        inc    dword ptr [esi + 1e2h]
        and    dl, 0e0h
        push   edx
        call   sub_1_0328
        add    esp, 4
        movzx  eax, al
        mov    dword ptr [esi + 24h], eax
        dec    eax
        cmp    byte ptr [esi + 0eh], al
        jbe    SHORT loc_1_05E1
        movzx  eax, byte ptr [esi + 0fh]
        dec    eax
loc_1_05E1:
        xor    dl, 13h
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        mov    eax, 0fffffffeh
        jmp    NEAR PTR loc_1_0480
loc_1_05F8:
        inc    dword ptr [esi + 1deh]
        and    dl, 0e0h
        xor    dl, 1ah
        push   0ffh
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 1
        push   0ffh
        push   edx
        call   sub_1_000C
        add    esp, 8
        mov    al, byte ptr [esi + 0eh]
        xor    dl, 8
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 3
        push   61h
        push   edx
        call   sub_1_000C
        add    esp, 8
        movzx  eax, byte ptr [esi + 0eh]
        inc    al
        mov    dword ptr [esi + 24h], eax
        xor    dl, 7
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 7
        push   22h
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 7
        push   5
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 7
        in     al, dx
        and    al, 0c2h
        mov    eax, 0fffffffdh
        cmp    al, 2
        je     NEAR PTR loc_1_0480
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_0480
sub_1_0690:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   edx
        push   ecx
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 24h]
        xchg   al, ah
        add    eax, ebx
        add    eax, 4
        and    al, 0feh
        mov    ecx, eax
        shr    eax, 8
        movzx  edx, word ptr [esi + 0c0h]
        cmp    eax, edx
        jl     SHORT loc_1_06C8
        shl    edx, 8
        sub    ecx, edx
        movzx  eax, byte ptr [esi + 0eh]
        shl    eax, 8
        add    ecx, eax
loc_1_06C8:
        mov    eax, ecx
        movzx  edx, word ptr [esi + 2]
        mov    edi, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 14h]
        test   byte ptr [esi + 8], 2
        je     SHORT loc_1_06E1
        xor    dl, 0ah
        out    dx, ax
        jmp    SHORT loc_1_0740
loc_1_06E1:
        xor    dl, 18h
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xchg   ah, al
        xor    dl, 1
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        mov    eax, ecx
        test   bl, 1
        je     SHORT loc_1_070F
        inc    ax
        test   cl, 1
        jne    SHORT loc_1_0716
        inc    ax
        jmp    SHORT loc_1_0716
loc_1_070F:
        test   cl, 1
        je     SHORT loc_1_0716
        inc    ax
loc_1_0716:
        xor    dl, 3
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xchg   al, ah
        xor    dl, 1
        push   eax
        push   edx
        call   sub_1_000C
        add    esp, 8
        xor    dl, 0bh
        push   0ah
        push   edx
        call   sub_1_000C
        add    esp, 8
loc_1_0740:
        and    dl, 0e0h
        test   ebx, 1
        je     SHORT loc_1_0758
        dec    cx
        xor    dl, 0ch
        in     ax, dx
        mov    byte ptr [edi], ah
        inc    edi
        and    dl, 0e0h
loc_1_0758:
        push   ecx
        test   byte ptr [esi + 8], 2
        je     SHORT loc_1_0794
        test   byte ptr [esi + 8], 4
        je     SHORT loc_1_0786
        push   esi
        mov    esi, dword ptr [esi + 20h]
        xor    dl, 4
        in     ax, dx
        and    ax, 0efffh
        out    dx, ax
        cld
        shr    ecx, 2
        rep movsd
        or     ax, 1000h
        out    dx, ax
        pop    esi
        xor    dl, 8
        jmp    SHORT loc_1_079F
loc_1_0786:
        xor    dl, 0ch
        cld
        shr    ecx, 2
        mov    eax, eax
        rep insd
        jmp    SHORT loc_1_079F
loc_1_0794:
        xor    dl, 0ch
        cld
        shr    ecx, 1
        mov    eax, eax
        rep insw
loc_1_079F:
        pop    ecx
        test   cx, 1
        je     SHORT loc_1_07AB
        in     ax, dx
        mov    byte ptr [edi], al
loc_1_07AB:
        test   byte ptr [esi + 8], 2
        jne    SHORT loc_1_07BA
        push   edx
        call   sub_1_07C3
        pop    edx
        jne    SHORT loc_1_07BC
loc_1_07BA:
        xor    eax, eax
loc_1_07BC:
        pop    ebx
        pop    ecx
        pop    edx
        pop    esi
        pop    edi
        leave
        ret
sub_1_07C3:
        push   ecx
        xor    dl, 1bh
        mov    ecx, 1ah
loc_1_07CC:
        in     al, dx
        and    al, 40h
        loope  loc_1_07CC
        je     SHORT loc_1_07E2
        push   40h
        push   edx
        call   sub_1_000C
        add    esp, 8
        pop    ecx
        xor    eax, eax
        ret
loc_1_07E2:
        push   edx
        xor    dl, 7
        push   22h
        push   edx
        call   sub_1_000C
        add    esp, 8
        pop    edx
        push   40h
        push   edx
        call   sub_1_000C
        add    esp, 8
        pop    ecx
        mov    eax, 0fffffffeh
        or     eax, eax
        ret
        db  000h, 000h
loc_1_0808:
        db  0A0h, 068h, 00Eh, 000h, 000h
        cmp    al, 0
        jne    SHORT loc_1_082D
        db  0FEh, 005h, 068h, 00Eh, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     SHORT loc_1_082F
        push   0
        push   0
        db  0E8h, 078h, 013h, 0FFh, 0FFh
        cmp    eax, 0
        jne    SHORT loc_1_082F
loc_1_082D:
        clc
        ret
loc_1_082F:
        stc
        ret
HPMCA_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_0840
        db  0FFh, 024h, 085h, 042h, 008h, 000h, 000h
loc_1_0840:
        clc
        ret
        db  008h, 008h, 000h, 000h, 040h, 008h, 000h, 000h, 040h, 008h, 000h, 000h, 040h, 008h, 000h, 000h
        db  040h, 008h, 000h, 000h, 040h, 008h, 000h, 000h, 040h, 008h, 000h, 000h, 040h, 008h, 000h, 000h
        db  040h, 008h, 000h, 000h, 040h, 008h, 000h, 000h, 040h, 008h, 000h, 000h, 040h, 008h, 000h, 000h
        db  040h, 008h, 000h, 000h, 040h, 008h, 000h, 000h, 040h, 008h, 000h, 000h, 040h, 008h, 000h, 000h
        db  040h, 008h, 000h, 000h, 040h, 008h, 000h, 000h, 040h, 008h, 000h, 000h, 040h, 008h, 000h, 000h
        db  040h, 008h, 000h, 000h, 040h, 008h, 000h, 000h, 040h, 008h, 000h, 000h, 040h, 008h, 000h, 000h
        db  040h, 008h, 000h, 000h, 040h, 008h, 000h, 000h, 008h, 008h, 000h, 000h, 000h, 000h
sub_1_08B0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 24h]
        push   eax
        mov    eax, dword ptr [ebp + 20h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_08E0
        pop    ebp
        ret    20h
        db  08Dh, 064h, 024h, 000h
sub_1_08E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 18h], 0eh
        jb     NEAR PTR loc_1_0980
        mov    edx, dword ptr [ebp + 10h]
        mov    al, byte ptr [edx]
        test   al, 1
        je     SHORT loc_1_0920
        cmp    dword ptr [edx + 2], -1
        jne    SHORT loc_1_0915
        cmp    al, 0ffh
        jne    SHORT loc_1_0915
        cmp    byte ptr [edx + 1], 0ffh
        jne    SHORT loc_1_0915
        mov    dword ptr [ebp - 4], 8
        jmp    SHORT loc_1_0987
loc_1_0915:
        mov    dword ptr [ebp - 4], 2
        jmp    SHORT loc_1_0987
        db  08Bh, 0FFh
loc_1_0920:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0ch], 20h
        je     SHORT loc_1_0970
        mov    edx, dword ptr [ebp + 10h]
        lea    ecx, [esi + 2ch]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_0946
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_094D
loc_1_0946:
        mov    dword ptr [ebp - 10h], 1
loc_1_094D:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_0960
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_098A
        db  08Dh, 064h, 024h, 000h
loc_1_0960:
        mov    dword ptr [ebp - 4], 20h
        jmp    SHORT loc_1_098A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0970:
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_098A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0980:
        mov    dword ptr [ebp - 4], 20h
loc_1_0987:
        mov    esi, dword ptr [ebp + 8]
loc_1_098A:
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     NEAR PTR loc_1_0A81
loc_1_0995:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, ebx
        and    eax, dword ptr [ebp - 4]
        test   al, 9
        jne    SHORT loc_1_09ED
        test   al, 2
        je     SHORT loc_1_09D9
        lea    eax, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 006h, 0FEh, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_1_09D9
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 19h]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        test   dword ptr [eax + ecx*4], edx
        jne    SHORT loc_1_09ED
loc_1_09D9:
        test   byte ptr [ebp - 4], 2
        je     SHORT loc_1_09E4
        test   bl, 4
        jne    SHORT loc_1_09ED
loc_1_09E4:
        test   bl, 20h
        je     NEAR PTR loc_1_0A76
loc_1_09ED:
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [ebp + 24h]
        push   eax
        mov    eax, dword ptr [ebp + 20h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_1_0C62
        mov    byte ptr [edi + 18h], 1
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_0A76
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_0A2F
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_0A2F:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_0A40
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_0A46
        db  08Bh, 0FFh
loc_1_0A40:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_0A46:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_0A55
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_0C68
loc_1_0A55:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 1ch]
        mov    eax, 1
        mov    cl, byte ptr [edi + 19h]
        shl    eax, cl
        or     dword ptr [esi + 28h], eax
        push   0
        push   1ah
        push   edi
        call   sub_1_0CDC
        mov    edi, ebx
        jmp    SHORT loc_1_0A79
loc_1_0A76:
        mov    edi, dword ptr [edi + 10h]
loc_1_0A79:
        or     edi, edi
        jne    NEAR PTR loc_1_0995
loc_1_0A81:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0A90:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0AA0
        pop    ebp
        ret    4
sub_1_0AA0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     SHORT loc_1_0B1E
loc_1_0AB0:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_0B17
        mov    byte ptr [edi + 18h], 0
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_0C68
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_0B17
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_0ADC
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_0ADC:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_0AF0
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_0AF6
        db  005h, 000h, 000h, 000h, 000h
loc_1_0AF0:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_0AF6:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 1ch]
        mov    eax, 1
        mov    cl, byte ptr [edi + 19h]
        shl    eax, cl
        or     dword ptr [esi + 28h], eax
        push   0
        push   1ah
        push   edi
        call   sub_1_0CDC
        mov    edi, ebx
        jmp    SHORT loc_1_0B1A
loc_1_0B17:
        mov    edi, dword ptr [edi + 10h]
loc_1_0B1A:
        or     edi, edi
        jne    SHORT loc_1_0AB0
loc_1_0B1E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0B30:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0ch]
        test   al, 20h
        je     SHORT loc_1_0B45
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0B45:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     SHORT loc_1_0B81
        cmp    dword ptr [ecx + 2], -1
        jne    SHORT loc_1_0B72
        cmp    bl, 0ffh
        jne    SHORT loc_1_0B72
        cmp    byte ptr [ecx + 1], 0ffh
        jne    SHORT loc_1_0B72
        test   al, 8
        je     SHORT loc_1_0B6B
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0B6B:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_0B72:
        test   al, 4
        jne    SHORT loc_1_0B7A
        test   al, 2
        je     SHORT loc_1_0B99
loc_1_0B7A:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0B81:
        mov    eax, dword ptr [edx + 2eh]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_0B99
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 2ch], ax
        jne    SHORT loc_1_0B99
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0B99:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
        db  056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Eh
        db  05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h
        db  008h, 0FCh, 0F3h, 066h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h
        db  024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 057h, 08Bh, 04Ch
        db  024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Ch, 05Fh, 0C2h, 00Ch
        db  000h, 057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h
        db  066h, 06Dh, 05Fh, 0C2h, 00Ch, 000h, 057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh
        db  04Ch, 024h, 010h, 0FCh, 0F3h, 06Dh, 05Fh, 0C2h, 00Ch, 000h
sub_1_0C1A:
        db  0CDh, 020h, 031h, 080h, 028h, 000h
sub_1_0C20:
        db  0CDh, 020h, 032h, 080h, 028h, 000h
sub_1_0C26:
        db  0CDh, 020h, 033h, 080h, 028h, 000h
sub_1_0C2C:
        db  0CDh, 020h, 034h, 080h, 028h, 000h
sub_1_0C32:
        db  0CDh, 020h, 035h, 080h, 028h, 000h
sub_1_0C38:
        db  0CDh, 020h, 036h, 080h, 028h, 000h
sub_1_0C3E:
        db  0CDh, 020h, 037h, 080h, 028h, 000h
sub_1_0C44:
        db  0CDh, 020h, 038h, 080h, 028h, 000h
sub_1_0C4A:
        db  0CDh, 020h, 039h, 080h, 028h, 000h
        db  0CDh, 020h, 03Ah, 080h, 028h, 000h
sub_1_0C56:
        db  0CDh, 020h, 03Bh, 080h, 028h, 000h
        db  0CDh, 020h, 03Ch, 080h, 028h, 000h
sub_1_0C62:
        db  0CDh, 020h, 03Dh, 080h, 028h, 000h
sub_1_0C68:
        db  0CDh, 020h, 03Eh, 080h, 028h, 000h
sub_1_0C6E:
        db  0CDh, 020h, 03Fh, 080h, 028h, 000h
sub_1_0C74:
        db  0CDh, 020h, 040h, 080h, 028h, 000h
        db  0CDh, 020h, 041h, 080h, 028h, 000h
sub_1_0C80:
        db  0CDh, 020h, 009h, 080h, 028h, 000h
        db  0CDh, 020h, 008h, 080h, 028h, 000h
        db  0CDh, 020h, 044h, 080h, 028h, 000h
        db  0CDh, 020h, 048h, 080h, 028h, 000h
        db  0CDh, 020h, 049h, 080h, 028h, 000h
        db  0CDh, 020h, 04Fh, 080h, 028h, 000h
        db  0CDh, 020h, 056h, 080h, 028h, 000h
        add    byte ptr [eax], al
        db  0CDh, 020h, 000h, 080h, 028h, 000h
sub_1_0CB2:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
sub_1_0CBE:
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
sub_1_0CC4:
        db  0CDh, 020h, 010h, 080h, 028h, 000h
sub_1_0CCA:
        db  0CDh, 020h, 011h, 080h, 028h, 000h
sub_1_0CD0:
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_0CD6:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_0CDC:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_0D00:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
sub_1_0D06:
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
sub_1_0D0C:
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
sub_1_0D12:
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
sub_1_0D1E:
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_0D24:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_0D48:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
sub_1_0D4E:
        db  0CDh, 020h, 028h, 080h, 028h, 000h
sub_1_0D54:
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_0D5A:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_0D60:
        db  0CDh, 020h, 007h, 080h, 028h, 000h
        db  0CDh, 020h, 042h, 080h, 028h, 000h
        db  0CDh, 020h, 012h, 080h, 028h, 000h
        db  0CDh, 020h, 013h, 080h, 028h, 000h
        db  0CDh, 020h, 014h, 080h, 028h, 000h
        db  0CDh, 020h, 015h, 080h, 028h, 000h
sub_1_0D84:
        db  0CDh, 020h, 04Ah, 080h, 028h, 000h
sub_1_0D8A:
        db  0CDh, 020h, 04Bh, 080h, 028h, 000h
sub_1_0D90:
        db  0CDh, 020h, 04Ch, 080h, 028h, 000h
loc_1_0D96:
        db  0CDh, 020h, 04Dh, 080h, 028h, 000h
sub_1_0D9C:
        db  0CDh, 020h, 04Eh, 080h, 028h, 000h
loc_1_0DA2:
        db  0CDh, 020h, 046h, 080h, 028h, 000h
sub_1_0DA8:
        db  0CDh, 020h, 045h, 080h, 028h, 000h
        db  0CDh, 020h, 043h, 080h, 028h, 000h
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        add    byte ptr [eax], al
sub_1_0DBC:
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 14h]
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 0ch]
        rep movsd
        mov    cl, al
        rep movsb
        pop    edi
        pop    esi
        ret    0ch
sub_1_0DDC:
        push   esi
        push   edi
        pushfd
        mov    ecx, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        cmp    esi, edi
        jae    SHORT loc_1_0DFC
        or     ecx, ecx
        je     SHORT loc_1_0E12
        std
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    SHORT loc_1_0E10
loc_1_0DFC:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     SHORT loc_1_0E10
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd
        mov    cl, al
loc_1_0E10:
        rep movsb
loc_1_0E12:
        popfd
        pop    edi
        pop    esi
        ret    0ch
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 00Ah, 003h, 000h, 000h, 003h, 00Ah, 000h, 000h, 048h, 050h, 04Dh, 043h
        db  041h, 020h, 020h, 020h, 000h, 040h, 000h, 054h, 031h, 008h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 04Dh, 061h, 078h, 069h, 06Dh, 075h, 06Dh, 04Dh
        db  075h, 06Ch, 074h, 069h, 063h, 061h, 073h, 074h, 04Ch, 069h, 073h, 074h, 000h, 000h, 000h, 000h
loc_1_0E88:
        dec    ebp
        popad
        db  078h, 069h
        insd
        jne    SHORT loc_1_0EFC
        dec    ebp
        jne    SHORT loc_1_0EFE
        db  074h, 069h
        arpl   word ptr [ecx + 73h], sp
        je     SHORT loc_1_0EE5
        imul   esi, dword ptr [ebx + 74h], 0
        dec    esi
        je     NEAR PTR loc_1_0F1B
        outsd
        db  072h, 06Bh
        inc    ecx
        jb     NEAR PTR loc_1_0F11
        jae    SHORT loc_1_0F21
        add    byte ptr [eax], al
        dec    esi
        db  065h, 074h, 077h
        outsd
        db  072h, 06Bh
        inc    ecx
        jb     NEAR PTR loc_1_0F21
        jae    SHORT loc_1_0F31
        add    byte ptr [eax], al
        push   ebx
        insb
        outsd
        je     SHORT loc_1_0F13
        db  075h, 06Dh
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [eax], al
        push   ebx
        insb
        outsd
        je     SHORT loc_1_0F1F
        db  075h, 06Dh
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [eax], al
        inc    edx
        db  075h, 073h
        dec    esi
        db  075h, 06Dh
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [eax], al
        db  000h
loc_1_0EE4:
        inc    edx
loc_1_0EE5:
        db  075h, 073h
        dec    esi
        jne    SHORT loc_1_0F57
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [eax], al
        db  000h
loc_1_0EF0:
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
loc_1_0EFC:
        add    al, 1
loc_1_0EFE:
        add    dword ptr [eax], eax
        add    eax, 6000101h
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
loc_1_0F11:
        add    dword ptr [ecx], eax
loc_1_0F13:
        add    byte ptr [edi], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    dword ptr [ecx], eax
loc_1_0F1B:
        add    byte ptr [ecx], cl
        add    dword ptr [ecx], eax
loc_1_0F1F:
        add    byte ptr [edx], cl
loc_1_0F21:
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 10000101h
loc_1_0F31:
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
        add    byte ptr [edx], al
        add    dword ptr [edx], eax
        add    byte ptr [ebx], al
        add    dword ptr [edx], eax
        add    byte ptr [ecx + eax], al
        add    al, byte ptr [eax]
        add    eax, 1000201h
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
loc_1_0F57:
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx]
        add    al, byte ptr [ecx]
        add    al, byte ptr [ecx]
        add    eax, dword ptr [ecx]
        add    al, byte ptr [ecx]
loc_1_0F6C:
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 6000101h
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 10000101h
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
loc_1_0FC8:
        dec    eax
        push   eax
        xor    dh, byte ptr [edi]
        xor    dh, byte ptr [esi + esi]
        inc    ecx
        and    byte ptr [ebp + 43h], cl
        and    byte ptr [ecx + eax*2 + 4eh], cl
        and    byte ptr [ecx + 64h], al
        popad
        db  070h, 074h
        db  065h, 072h, 02Fh
        xor    dword ptr [esi], esi
        and    byte ptr [eax + edx*2], dl
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 048h, 050h, 04Dh, 043h, 041h, 000h, 000h, 000h
loc_1_0FF4:
        dec    eax
        push   eax
        dec    ebp
        inc    ebx
        inc    ecx
        add    byte ptr [eax], al
        db  000h
loc_1_0FFC:
        dec    esi
        dec    edi
        push   esp
        dec    ecx
        inc    ebx
        inc    ebp
        cmp    ah, byte ptr [eax]
        inc    ecx
        popad
        jo     SHORT loc_1_107D
        jb     NEAR PTR loc_1_102C
        outsb
        outsd
        je     SHORT loc_1_1030
        db  070h, 072h
        jae    NEAR PTR loc_1_107A
        outsb
        je     SHORT loc_1_1018
loc_1_1018:
        inc    ebp
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        dec    ecx
        outsb
        db  076h, 061h
        insb
        imul   esp, dword ptr [eax + 49h], 61204f2fh
loc_1_102C:
        jb     NEAR PTR loc_1_1095
loc_1_1030:
        db  073h, 073h
        sub    byte ptr [ebp + 73h], ah
        sub    dword ptr [eax], eax
        add    byte ptr [ebp + 52h], al
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        dec    ecx
        outsb
        jbe    SHORT loc_1_10A4
        insb
        imul   esp, dword ptr [eax + 49h], 7265746eh
        db  072h, 075h
        jo     SHORT loc_1_10C4
        and    byte ptr [esi + 65h], dl
        arpl   word ptr [edi + ebp*2 + 72h], si
        add    byte ptr [ebp + 52h], al
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        push   edi
        db  072h, 06Fh
        outsb
        and    byte ptr [bx + di + 6eh], ch
        je     SHORT loc_1_10CE
        db  072h, 072h
        db  075h, 070h
        db  074h, 020h
        jbe    SHORT loc_1_10D6
        arpl   word ptr [edi + ebp*2 + 72h], si
        add    byte ptr [eax], al
        add    byte ptr [eax + 50h], cl
loc_1_107A:
        das
        xor    dword ptr [esi], esi
loc_1_107D:
        and    byte ptr [ebp + 43h], cl
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [esi + 61h], al
        imul   ebp, dword ptr [ebp + 64h], 206f7420h
        jb     SHORT loc_1_10F5
        jae    SHORT loc_1_1106
        popad
        db  072h, 074h
loc_1_1095:
        add    byte ptr cs:[eax], al
loc_1_1098:
        inc    esi
        popad
        imul   ebp, dword ptr [ebp + 64h], 206f7420h
        db  072h, 065h
loc_1_10A4:
        db  073h, 074h
        popad
        db  072h, 074h
        add    byte ptr cs:[eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_10C4:
        add    byte ptr [eax], al
loc_1_10C6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_10CA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_10CE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_10D2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_10D6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_10DA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_10DE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_10E2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_10E6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_10EA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_10EE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_10F2:
        add    byte ptr [eax], al
        db  000h
loc_1_10F5:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_1106:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
