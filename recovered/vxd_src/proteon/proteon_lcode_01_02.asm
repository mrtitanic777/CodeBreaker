; proteon_lcode_01_02.asm -- PROTEON object 1 segment _LTEXT  (part 2/2)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_5156:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 29ah]
        and    eax, 0ffffh
        add    eax, 0c85h
        mov    dx, ax
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 8
        shr    eax, 3
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 2aah], ax
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 4
        je     NEAR PTR loc_1_51AB
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 2a8h], 4
        jmp    NEAR PTR loc_1_51B7
loc_1_51AB:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 2a8h], 10h
loc_1_51B7:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 294h], 1990h
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 298h], 16h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_45A0
        sub    ax, ax
        jmp    NEAR PTR loc_1_51E0
loc_1_51E0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_51E7:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        movsx  eax, word ptr [ebp + 0ch]
        or     eax, eax
        je     NEAR PTR loc_1_5225
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1eeh]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   al, 0c0h
        jne    NEAR PTR loc_1_5225
        mov    ax, 1
        jmp    NEAR PTR loc_1_5236
loc_1_5225:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4FB6
        sub    ax, ax
        jmp    NEAR PTR loc_1_5236
loc_1_5236:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_523D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 8], 3e8h
        jmp    NEAR PTR loc_1_5255
loc_1_5251:
        dec    word ptr [ebp - 8]
loc_1_5255:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        or     eax, eax
        jle    NEAR PTR loc_1_5290
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1eeh]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   ah, 2
        jne    NEAR PTR loc_1_528B
        jmp    NEAR PTR loc_1_5290
loc_1_528B:
        jmp    NEAR PTR loc_1_5251
loc_1_5290:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_5297:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        or     eax, 800h
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1eeh]
        mov    ax, word ptr [ebp - 0ch]
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 1f4h]
        in     ax, dx
        mov    word ptr [ebp - 10h], ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 1ech]
        in     ax, dx
        mov    word ptr [ebp - 6], ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 1e8h]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        test   ah, 4
        jne    NEAR PTR loc_1_534B
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        dec    eax
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1e4h]
        and    eax, 0ffffh
        push   eax
        call   sub_1_04D0
        mov    ecx, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp - 10h]
        and    ebx, 0ffffh
        mov    esi, dword ptr [ebp - 4]
        mov    dx, word ptr [ecx + 1e4h]
        mov    al, byte ptr [ebx + esi - 1]
        out    dx, al
        jmp    NEAR PTR loc_1_5389
loc_1_534B:
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        dec    eax
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1e4h]
        and    eax, 0ffffh
        push   eax
        call   sub_1_050D
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 1e4h]
        in     al, dx
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + edx - 1], al
loc_1_5389:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_523D
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_5399:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2a4h]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_1_53E8
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1eeh]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   ah, 2
        je     NEAR PTR loc_1_53E8
        mov    ax, word ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5297
loc_1_53E8:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_53EF:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
loc_1_53F8:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2a4h]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_1_5443
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1eeh]
        in     ax, dx
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        test   ah, 2
        je     NEAR PTR loc_1_5443
        mov    ax, word ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5297
        jmp    NEAR PTR loc_1_53F8
loc_1_5443:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1eah]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   al, 80h
        je     NEAR PTR loc_1_5476
        mov    ax, word ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6C5D
        jmp    NEAR PTR loc_1_547B
loc_1_5476:
        jmp    NEAR PTR loc_1_5480
loc_1_547B:
        jmp    NEAR PTR loc_1_53F8
loc_1_5480:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2dch]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_54D3
loc_1_54A4:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_495F
        jmp    NEAR PTR loc_1_54F6
loc_1_54B2:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4F87
        jmp    NEAR PTR loc_1_54F6
loc_1_54C0:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4DDE
        jmp    NEAR PTR loc_1_54F6
        db  0E9h, 023h, 000h, 000h, 000h
loc_1_54D3:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_54A4
        cmp    dword ptr [ebp - 4], 1
        je     NEAR PTR loc_1_54B2
        cmp    dword ptr [ebp - 4], 2
        je     NEAR PTR loc_1_54C0
        jmp    NEAR PTR loc_1_54F6
loc_1_54F6:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2dch]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_5549
loc_1_551A:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_496C
        jmp    NEAR PTR loc_1_556C
loc_1_5528:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4F94
        jmp    NEAR PTR loc_1_556C
loc_1_5536:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4DEB
        jmp    NEAR PTR loc_1_556C
        db  0E9h, 023h, 000h, 000h, 000h
loc_1_5549:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_551A
        cmp    dword ptr [ebp - 4], 1
        je     NEAR PTR loc_1_5528
        cmp    dword ptr [ebp - 4], 2
        je     NEAR PTR loc_1_5536
        jmp    NEAR PTR loc_1_556C
loc_1_556C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_5573:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2dch]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_55CB
loc_1_5590:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4BE2
        mov    word ptr [ebp - 4], ax
        jmp    NEAR PTR loc_1_55EE
loc_1_55A2:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5156
        mov    word ptr [ebp - 4], ax
        jmp    NEAR PTR loc_1_55EE
loc_1_55B4:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4EFB
        mov    word ptr [ebp - 4], ax
        jmp    NEAR PTR loc_1_55EE
        db  0E9h, 023h, 000h, 000h, 000h
loc_1_55CB:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_5590
        cmp    dword ptr [ebp - 8], 1
        je     NEAR PTR loc_1_55A2
        cmp    dword ptr [ebp - 8], 2
        je     NEAR PTR loc_1_55B4
        jmp    NEAR PTR loc_1_55EE
loc_1_55EE:
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_55F7
loc_1_55F7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_55FE:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2dch]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_5665
loc_1_561B:
        mov    ax, word ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4D57
        mov    word ptr [ebp - 4], ax
        jmp    NEAR PTR loc_1_5688
loc_1_5632:
        mov    ax, word ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_51E7
        mov    word ptr [ebp - 4], ax
        jmp    NEAR PTR loc_1_5688
loc_1_5649:
        mov    ax, word ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4F31
        mov    word ptr [ebp - 4], ax
        jmp    NEAR PTR loc_1_5688
        db  0E9h, 023h, 000h, 000h, 000h
loc_1_5665:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_561B
        cmp    dword ptr [ebp - 8], 1
        je     NEAR PTR loc_1_5632
        cmp    dword ptr [ebp - 8], 2
        je     NEAR PTR loc_1_5649
        jmp    NEAR PTR loc_1_5688
loc_1_5688:
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_5691
loc_1_5691:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_5698:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
loc_1_56A1:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_1_56D9
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2d4h]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_1_56D4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5399
loc_1_56D4:
        jmp    NEAR PTR loc_1_56DE
loc_1_56D9:
        jmp    NEAR PTR loc_1_56E3
loc_1_56DE:
        jmp    NEAR PTR loc_1_56A1
loc_1_56E3:
        mov    eax, dword ptr [ebp + 18h]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_1_56FF
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 2d0h], 0
loc_1_56FF:
        mov    ax, word ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx], ax
        mov    ax, word ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 2], ax
        mov    ax, word ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1eah]
        mov    ax, 9080h
        out    dx, ax
        mov    eax, dword ptr [ebp + 18h]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_1_57B6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3574
        mov    word ptr [ebp - 4], ax
loc_1_574C:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2d4h]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_1_576C
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5399
loc_1_576C:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3574
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    ecx, dword ptr [ebp + 18h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_579F
        mov    ax, 0ffffh
        jmp    NEAR PTR loc_1_57BE
loc_1_579F:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2d0h]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_1_574C
loc_1_57B6:
        sub    ax, ax
        jmp    NEAR PTR loc_1_57BE
loc_1_57BE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    14h
sub_1_57C5:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 220h]
        mov    ecx, dword ptr [ebp + 8]
        movsx  ecx, word ptr [ecx + 222h]
        sub    eax, ecx
        mov    word ptr [ebp - 0ch], ax
        movsx  eax, word ptr [ebp - 0ch]
        or     eax, eax
        jg     NEAR PTR loc_1_57FF
        movsx  eax, word ptr [ebp - 0ch]
        add    eax, 6
        mov    word ptr [ebp - 0ch], ax
loc_1_57FF:
        movsx  eax, word ptr [ebp - 0ch]
        cmp    eax, 1
        jne    NEAR PTR loc_1_5815
        mov    ax, 0ffffh
        jmp    NEAR PTR loc_1_5A22
loc_1_5815:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1fah]
        and    eax, 0ffffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_5836
        mov    ax, 0ffffh
        jmp    NEAR PTR loc_1_5A22
loc_1_5836:
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 1fah]
        mov    ax, word ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        movsx  ecx, word ptr [ecx + 222h]
        lea    ecx, [ecx + ecx*2]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + ecx*2 + 1fch], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        movsx  ecx, word ptr [ecx + 222h]
        lea    ecx, [ecx + ecx*2]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*2 + 1feh], eax
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 222h]
        cmp    eax, 5
        jne    NEAR PTR loc_1_589A
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 222h], 0
        jmp    NEAR PTR loc_1_58A4
loc_1_589A:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 222h]
loc_1_58A4:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3A52
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ah, al
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        mov    al, ch
        mov    ecx, dword ptr [ebp + 8]
        movsx  ecx, word ptr [ecx + 1f8h]
        add    ecx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 38h], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ah, al
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        mov    al, ch
        mov    ecx, dword ptr [ebp + 8]
        movsx  ecx, word ptr [ecx + 1f8h]
        add    ecx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 3ah], ax
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        mov    al, ah
        mov    ah, byte ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        movsx  ecx, word ptr [ecx + 1f8h]
        add    ecx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 36h], ax
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 224h]
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 226h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 224h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jge    NEAR PTR loc_1_5983
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 224h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 226h], ax
loc_1_5983:
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 1f8h]
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [eax + ecx + 32h], 88h
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 1f8h]
        cmp    eax, 5
        jne    NEAR PTR loc_1_59C3
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 1f8h], 0
        jmp    NEAR PTR loc_1_59CD
loc_1_59C3:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 1f8h]
loc_1_59CD:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2d4h]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_1_5A0A
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2d6h]
        and    eax, 0ffffh
        or     eax, 8200h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 2d6h], ax
        sub    ax, ax
        jmp    NEAR PTR loc_1_5A22
loc_1_5A0A:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1eah]
        mov    ax, 8280h
        out    dx, ax
        sub    ax, ax
        jmp    NEAR PTR loc_1_5A22
loc_1_5A22:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_5A29:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1deh], 0
        je     NEAR PTR loc_1_5A7A
        mov    eax, dword ptr [ebp + 14h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        call   dword ptr [eax + 1deh]
        mov    word ptr [ebp - 4], ax
        movsx  eax, word ptr [ebp - 4]
        or     eax, eax
        je     NEAR PTR loc_1_5A7A
        jmp    NEAR PTR loc_1_5A8C
loc_1_5A7A:
        mov    ax, word ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_57C5
loc_1_5A8C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
sub_1_5A93:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1deh], 0
        je     NEAR PTR loc_1_5AE3
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 226h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 224h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_5AE3
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        call   dword ptr [eax + 1deh]
loc_1_5AE3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_5AEA:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
loc_1_5AF3:
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 224h]
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 1fah]
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 1f6h]
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + ecx + 32h]
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 1f6h]
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [eax + ecx + 32h], 0
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 1f6h]
        cmp    eax, 5
        jne    NEAR PTR loc_1_5B65
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 1f6h], 0
        jmp    NEAR PTR loc_1_5B6F
loc_1_5B65:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 1f6h]
loc_1_5B6F:
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 220h]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*2 + 1feh]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 220h]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 1fch]
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 220h]
        cmp    eax, 5
        jne    NEAR PTR loc_1_5BC9
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 220h], 0
        jmp    NEAR PTR loc_1_5BD3
loc_1_5BC9:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 220h]
loc_1_5BD3:
        mov    ax, word ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_57C5
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        test   al, 10h
        je     NEAR PTR loc_1_5BFA
        jmp    NEAR PTR loc_1_5BFF
loc_1_5BFA:
        jmp    NEAR PTR loc_1_5AF3
loc_1_5BFF:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_5C06:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
loc_1_5C0F:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 224h]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_1_5C34
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5A93
        jmp    NEAR PTR loc_1_5D9E
loc_1_5C34:
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 1f6h]
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + ecx + 32h]
        and    eax, 0ffffh
        test   al, 40h
        jne    NEAR PTR loc_1_5C69
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5A93
        jmp    NEAR PTR loc_1_5D9E
loc_1_5C69:
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 1f6h]
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + ecx + 32h]
        and    eax, 0ffffh
        test   al, 10h
        jne    NEAR PTR loc_1_5C9E
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5AEA
        jmp    NEAR PTR loc_1_5C0F
loc_1_5C9E:
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 224h]
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 1f6h]
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + ecx + 34h]
        and    eax, 0ffffh
        mov    al, ah
        mov    ecx, dword ptr [ebp + 8]
        movsx  ecx, word ptr [ecx + 1f6h]
        add    ecx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    ah, byte ptr [ecx + edx + 34h]
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 1f6h]
        cmp    eax, 5
        jne    NEAR PTR loc_1_5D0A
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 1f6h], 0
        jmp    NEAR PTR loc_1_5D14
loc_1_5D0A:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 1f6h]
loc_1_5D14:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 1fah]
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 220h]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*2 + 1feh]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 220h]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 1fch]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 220h]
        cmp    eax, 5
        jne    NEAR PTR loc_1_5D78
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 220h], 0
        jmp    NEAR PTR loc_1_5D82
loc_1_5D78:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 220h]
loc_1_5D82:
        mov    ax, word ptr [ebp - 4]
        push   eax
        mov    ax, word ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5A29
        jmp    NEAR PTR loc_1_5C0F
loc_1_5D9E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_5DA5:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 1f6h]
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        add    eax, dword ptr [ebp + 8]
        add    eax, 2eh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3A10
        push   0
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ah, al
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        mov    al, ch
        push   eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ah, al
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        mov    al, ch
        push   eax
        push   600h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5698
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_5E1B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1deh], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_5E34:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 1fah], 6
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 1f6h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 1f8h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 220h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 222h], 0
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_5E88
loc_1_5E84:
        inc    word ptr [ebp - 4]
loc_1_5E88:
        movsx  eax, word ptr [ebp - 4]
        cmp    eax, 5
        jge    NEAR PTR loc_1_5F4D
        movsx  eax, word ptr [ebp - 4]
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [eax + ecx + 32h], 0
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        movsx  eax, word ptr [ebp - 4]
        lea    eax, [eax*2 + 2]
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        add    eax, dword ptr [ebp + 8]
        add    eax, 2eh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3A10
        movsx  eax, word ptr [ebp - 4]
        lea    eax, [eax*2 + 2]
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        add    eax, dword ptr [ebp + 8]
        add    eax, 2eh
        movsx  ecx, word ptr [ebp - 4]
        add    ecx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + edx + 48h], eax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ah, al
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        mov    al, ch
        movsx  ecx, word ptr [ebp - 4]
        add    ecx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 2eh], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        mov    al, ah
        mov    ah, byte ptr [ebp - 0ch]
        movsx  ecx, word ptr [ebp - 4]
        add    ecx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 30h], ax
        jmp    NEAR PTR loc_1_5E84
loc_1_5F4D:
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 2eh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3A10
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 2eh
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 0deh], ecx
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        mov    ch, cl
        mov    edx, dword ptr [ebp - 8]
        and    edx, 0ffffh
        mov    cl, dh
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + 0c4h], cx
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        mov    cl, ch
        mov    ch, byte ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + 0c6h], cx
        sub    ax, ax
        jmp    NEAR PTR loc_1_5FB4
loc_1_5FB4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_5FBB:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 244h]
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        movsx  ecx, word ptr [ecx + 268h]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 246h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 242h]
        mov    word ptr [ebp - 14h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 242h]
        mov    word ptr [ebp - 10h], ax
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [eax + ecx + 0e6h], 20h
        mov    word ptr [ebp - 8], 0
        mov    word ptr [ebp - 4], 0
        mov    word ptr [ebp - 18h], 0
        jmp    NEAR PTR loc_1_6039
loc_1_6035:
        inc    word ptr [ebp - 8]
loc_1_6039:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_6231
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ax, word ptr [ecx + eax*2 + 0ch]
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        dec    eax
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_60B3
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        or     eax, 8000h
        mov    word ptr [ebp - 0ch], ax
loc_1_60B3:
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        mov    ah, al
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        mov    al, ch
        mov    ecx, dword ptr [ebp - 18h]
        and    ecx, 0ffffh
        lea    ecx, [ecx + ecx*2]
        mov    edx, dword ptr [ebp - 10h]
        and    edx, 0ffffh
        add    edx, edx
        lea    edx, [edx + edx*2]
        lea    edx, [edx + edx*4]
        lea    ecx, [edx + ecx*2]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 0eah], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ax, word ptr [ecx + eax*2 + 10h]
        and    eax, 0ffffh
        mov    ah, al
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        lea    ecx, [ecx + ecx*2]
        mov    edx, dword ptr [ebp + 0ch]
        mov    cx, word ptr [edx + ecx*2 + 10h]
        and    ecx, 0ffffh
        mov    al, ch
        mov    ecx, dword ptr [ebp - 18h]
        and    ecx, 0ffffh
        lea    ecx, [ecx + ecx*2]
        mov    edx, dword ptr [ebp - 10h]
        and    edx, 0ffffh
        add    edx, edx
        lea    edx, [edx + edx*2]
        lea    edx, [edx + edx*4]
        lea    ecx, [edx + ecx*2]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 0ech], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ax, word ptr [ecx + eax*2 + 0eh]
        and    eax, 0ffffh
        mov    al, ah
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        lea    ecx, [ecx + ecx*2]
        mov    edx, dword ptr [ebp + 0ch]
        mov    ah, byte ptr [edx + ecx*2 + 0eh]
        mov    ecx, dword ptr [ebp - 18h]
        and    ecx, 0ffffh
        lea    ecx, [ecx + ecx*2]
        mov    edx, dword ptr [ebp - 10h]
        and    edx, 0ffffh
        add    edx, edx
        lea    edx, [edx + edx*2]
        lea    edx, [edx + edx*4]
        lea    ecx, [edx + ecx*2]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 0eeh], ax
        inc    word ptr [ebp - 18h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        dec    eax
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_622C
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_622C
        mov    word ptr [ebp - 18h], 0
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 244h]
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        cmp    eax, 7
        jne    NEAR PTR loc_1_620B
        mov    word ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_1_620F
loc_1_620B:
        inc    word ptr [ebp - 10h]
loc_1_620F:
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [eax + ecx + 0e6h], 0
loc_1_622C:
        jmp    NEAR PTR loc_1_6035
loc_1_6231:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 26eh]
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 26ah]
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 26ah]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 26ch]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_6280
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 26ah]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 26ch], ax
loc_1_6280:
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 268h]
        cmp    eax, 7
        jne    NEAR PTR loc_1_62A4
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 268h], 0
        jmp    NEAR PTR loc_1_62AE
loc_1_62A4:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 268h]
loc_1_62AE:
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + ecx + 0e6h]
        and    eax, 0ffffh
        or     eax, 10h
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        add    ecx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 0e6h], ax
        mov    ax, word ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 242h], ax
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 242h]
        cmp    eax, 7
        jne    NEAR PTR loc_1_631F
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 242h], 0
        jmp    NEAR PTR loc_1_6329
loc_1_631F:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 242h]
loc_1_6329:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ah, al
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        mov    al, ch
        mov    ecx, dword ptr [ebp - 14h]
        and    ecx, 0ffffh
        add    ecx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 0e8h], ax
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + ecx + 0e6h]
        and    eax, 0ffffh
        or     eax, 88h
        mov    ecx, dword ptr [ebp - 14h]
        and    ecx, 0ffffh
        add    ecx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 0e6h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2d4h]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_1_63D8
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2d6h]
        and    eax, 0ffffh
        or     eax, 8100h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 2d6h], ax
        sub    ax, ax
        jmp    NEAR PTR loc_1_63F0
loc_1_63D8:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1eah]
        mov    ax, 8180h
        out    dx, ax
        sub    ax, ax
        jmp    NEAR PTR loc_1_63F0
loc_1_63F0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_63F7:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 26ah]
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1dah], 0
        je     NEAR PTR loc_1_6467
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 266h]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 246h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        call   dword ptr [eax + 1dah]
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 266h]
        cmp    eax, 7
        jne    NEAR PTR loc_1_645D
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 266h], 0
        jmp    NEAR PTR loc_1_6467
loc_1_645D:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 266h]
loc_1_6467:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_646E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2dch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        call   dword ptr [eax + 1d6h]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_648E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
loc_1_6494:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 26ah]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_1_64B5
        jmp    NEAR PTR loc_1_65C4
        db  0E9h, 00Ah, 001h, 000h, 000h
loc_1_64B5:
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 240h]
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + ecx + 0e6h]
        and    eax, 0ffffh
        test   al, 80h
        jne    NEAR PTR loc_1_65BA
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 240h]
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + ecx + 0e6h]
        and    eax, 0ffffh
        test   al, 10h
        jne    NEAR PTR loc_1_656B
loc_1_6509:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 244h]
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 240h]
        cmp    eax, 7
        jne    NEAR PTR loc_1_6537
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 240h], 0
        jmp    NEAR PTR loc_1_6541
loc_1_6537:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 240h]
loc_1_6541:
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 240h]
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + ecx + 0e6h]
        and    eax, 0ffffh
        test   al, 10h
        je     NEAR PTR loc_1_6509
loc_1_656B:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 244h]
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 240h]
        cmp    eax, 7
        jne    NEAR PTR loc_1_6599
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 240h], 0
        jmp    NEAR PTR loc_1_65A3
loc_1_6599:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 240h]
loc_1_65A3:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_63F7
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_646E
        jmp    NEAR PTR loc_1_65BF
loc_1_65BA:
        jmp    NEAR PTR loc_1_65C4
loc_1_65BF:
        jmp    NEAR PTR loc_1_6494
loc_1_65C4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_65CB:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
loc_1_65D1:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 26ah]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_1_6677
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 240h]
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + ecx + 0e6h]
        and    eax, 0ff7fh
        mov    ecx, dword ptr [ebp + 8]
        movsx  ecx, word ptr [ecx + 240h]
        add    ecx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 0e6h], ax
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 240h]
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + ecx + 0e6h]
        and    eax, 0ffffh
        or     eax, 40h
        mov    ecx, dword ptr [ebp + 8]
        movsx  ecx, word ptr [ecx + 240h]
        add    ecx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 0e6h], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_648E
        jmp    NEAR PTR loc_1_65D1
loc_1_6677:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_667E:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 240h]
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        add    eax, dword ptr [ebp + 8]
        add    eax, 0e2h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3A10
        push   0
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ah, al
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        mov    al, ch
        push   eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ah, al
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        mov    al, ch
        push   eax
        push   400h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5698
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_66F6:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 244h], 8
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 240h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 242h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 266h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 268h], 0
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_674A
loc_1_6746:
        inc    word ptr [ebp - 4]
loc_1_674A:
        movsx  eax, word ptr [ebp - 4]
        cmp    eax, 7
        jge    NEAR PTR loc_1_6827
        movsx  eax, word ptr [ebp - 4]
        add    eax, eax
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [eax + ecx + 0e6h], 0
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        movsx  eax, word ptr [ebp - 4]
        lea    eax, [eax*2 + 2]
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        add    eax, dword ptr [ebp + 8]
        add    eax, 0e2h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3A10
        movsx  eax, word ptr [ebp - 4]
        lea    eax, [eax*2 + 2]
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        add    eax, dword ptr [ebp + 8]
        add    eax, 0e2h
        movsx  ecx, word ptr [ebp - 4]
        add    ecx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + edx + 0fch], eax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ah, al
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        mov    al, ch
        movsx  ecx, word ptr [ebp - 4]
        add    ecx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 0e2h], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        mov    ah, al
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        mov    al, ch
        movsx  ecx, word ptr [ebp - 4]
        add    ecx, ecx
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx + edx + 0e4h], ax
        jmp    NEAR PTR loc_1_6746
loc_1_6827:
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0e2h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3A10
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 0e2h
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 1ceh], ecx
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        mov    ch, cl
        mov    edx, dword ptr [ebp - 8]
        and    edx, 0ffffh
        mov    cl, dh
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + 1b4h], cx
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        mov    ch, cl
        mov    edx, dword ptr [ebp - 0ch]
        and    edx, 0ffffh
        mov    cl, dh
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + 1b6h], cx
        sub    ax, ax
        jmp    NEAR PTR loc_1_689B
loc_1_689B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_68A2:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1d6h], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_68BB:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1dah], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_68D4:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2c0h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 16h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2c2h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 18h], ax
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0eh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3A10
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ah, al
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        mov    al, ch
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ah, al
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        mov    al, ch
        mov    word ptr [ebp - 8], ax
        mov    ax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 2ah], ax
        mov    ax, word ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 2ch], ax
        mov    ax, word ptr [ebp + 0ch]
        push   eax
        mov    ax, word ptr [ebp - 8]
        push   eax
        mov    ax, word ptr [ebp - 4]
        push   eax
        push   300h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5698
        jmp    NEAR PTR loc_1_6981
loc_1_6981:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_6988:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2beh]
        push   eax
        push   0f00h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5698
loc_1_69AE:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5399
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 1eah]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        cmp    byte ptr [ebp - 4], 84h
        jne    NEAR PTR loc_1_69FF
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        mov    al, ah
        mov    ecx, dword ptr [ebp + 8]
        mov    ah, byte ptr [ecx + 8]
        and    eax, 0ffffh
        test   ah, 0dh
        je     NEAR PTR loc_1_69FF
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 2d2h], 211h
loc_1_69FF:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   al, 80h
        je     NEAR PTR loc_1_6A1F
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1eah]
        mov    ax, 0a000h
        out    dx, ax
loc_1_6A1F:
        cmp    byte ptr [ebp - 4], 88h
        jne    NEAR PTR loc_1_69AE
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_6A30:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        mov    ah, al
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 8]
        and    ecx, 0ffffh
        mov    al, ch
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 2d2h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 6]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 2d0h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1eah]
        mov    ax, 0a000h
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2d0h]
        and    eax, 0ffffh
        cmp    eax, 300h
        jne    NEAR PTR loc_1_6B7F
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2d2h]
        and    eax, 0ffffh
        cmp    eax, 8000h
        jne    NEAR PTR loc_1_6B59
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2c0h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 2c2h]
        and    ecx, 0ffffh
        or     eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 2beh]
        and    ecx, 0ffffh
        or     eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_1_6B30
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6988
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2d2h]
        and    eax, 0ffffh
        cmp    eax, 8000h
        je     NEAR PTR loc_1_6B30
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 2d4h], 0
        jmp    NEAR PTR loc_1_6B8B
loc_1_6B30:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_648E
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5C06
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5DA5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_667E
        jmp    NEAR PTR loc_1_6B7F
loc_1_6B59:
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 2ceh]
        or     eax, eax
        je     NEAR PTR loc_1_6B7F
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_65CB
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68D4
loc_1_6B7F:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 2d4h], 0
loc_1_6B8B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_6B92:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 8], 1
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        mov    ah, al
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 8]
        and    ecx, 0ffffh
        mov    al, ch
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1eah]
        mov    ax, 0a000h
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1d2h], 0
        je     NEAR PTR loc_1_6BF3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        call   dword ptr [eax + 1d2h]
        mov    word ptr [ebp - 8], ax
loc_1_6BF3:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   ah, 0dh
        je     NEAR PTR loc_1_6C31
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_1_6C31
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 2ceh]
        or     eax, eax
        je     NEAR PTR loc_1_6C31
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68D4
loc_1_6C31:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 2d4h], 0
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1d2h], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_6C5D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 2c6h]
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 2d4h], 1
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffh
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_6CFB
loc_1_6C8B:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6A30
        jmp    NEAR PTR loc_1_6D1A
loc_1_6C99:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6B92
        jmp    NEAR PTR loc_1_6D1A
loc_1_6CA7:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 2d6h], 0a080h
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1eah]
        sub    ax, ax
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5C06
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_648E
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 2d4h], 0
        mov    ecx, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 1eah]
        mov    ax, word ptr [ebx + 2d6h]
        out    dx, ax
        jmp    NEAR PTR loc_1_6D1A
loc_1_6CFB:
        cmp    dword ptr [ebp - 4], 84h
        je     NEAR PTR loc_1_6C99
        cmp    dword ptr [ebp - 4], 88h
        je     NEAR PTR loc_1_6C8B
        jmp    NEAR PTR loc_1_6CA7
loc_1_6D1A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_6D21:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2dch]
        mov    al, byte ptr [eax + 0b5h]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_6D63
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5573
        movsx  eax, ax
        or     eax, eax
        je     NEAR PTR loc_1_6D63
        mov    ax, 0ffffh
        jmp    NEAR PTR loc_1_6E06
loc_1_6D63:
        db  066h, 0C7h, 005h, 0A8h, 07Ah, 000h, 000h, 000h, 09Fh
        db  066h, 0C7h, 005h, 0B4h, 07Ah, 000h, 000h, 005h, 005h
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3A10
        mov    ax, word ptr [ebp - 4]
        db  066h, 0A3h, 0B6h, 07Ah, 000h, 000h
        mov    ax, word ptr [ebp - 8]
        db  066h, 0A3h, 0B8h, 07Ah, 000h, 000h
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 6
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3A10
        mov    cx, word ptr [ebp - 4]
        db  066h, 089h, 00Dh, 0BAh, 07Ah, 000h, 000h
        mov    cx, word ptr [ebp - 8]
        db  066h, 089h, 00Dh, 0BCh, 07Ah, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0eh], 60h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1eh], 1a00h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 20h], 1a00h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 22h], 4
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 28h], 0ah
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 29h], 19h
        sub    ax, ax
        jmp    NEAR PTR loc_1_6E06
loc_1_6E06:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_6E0D:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 0ch], eax
        push   4
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_474D
        mov    byte ptr [ebp - 10h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        or     eax, eax
        je     NEAR PTR loc_1_6E72
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        cmp    eax, 3bh
        jne    NEAR PTR loc_1_6E72
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp + 0ch], eax
        push   3
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_474D
        movsx  eax, ax
        or     eax, eax
        je     NEAR PTR loc_1_6E72
        mov    ax, 0ffffh
        jmp    NEAR PTR loc_1_7014
loc_1_6E72:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1eeh]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ff3fh
        mov    dx, word ptr [eax + 1eeh]
        mov    ax, cx
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3574
        mov    word ptr [ebp - 14h], ax
loc_1_6EA7:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1eah]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        mov    al, byte ptr [ebp - 8]
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 3bh
        jne    NEAR PTR loc_1_6ED0
        jmp    NEAR PTR loc_1_6F28
loc_1_6ED0:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 40h
        jne    NEAR PTR loc_1_6EE6
        sub    ax, ax
        jmp    NEAR PTR loc_1_7014
loc_1_6EE6:
        mov    al, byte ptr [ebp - 4]
        and    eax, 30h
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 30h
        jne    NEAR PTR loc_1_6F06
        mov    ax, 0fffeh
        jmp    NEAR PTR loc_1_7014
loc_1_6F06:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3574
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 14h]
        and    ecx, 0ffffh
        sub    eax, ecx
        cmp    eax, 5bh
        jle    NEAR PTR loc_1_6EA7
loc_1_6F28:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 3bh
        jne    NEAR PTR loc_1_700B
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7777
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp + 0ch], eax
        push   3
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_474D
        movsx  eax, ax
        or     eax, eax
        je     NEAR PTR loc_1_6F68
        mov    ax, 0ffffh
        jmp    NEAR PTR loc_1_7014
loc_1_6F68:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1eeh]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ff3fh
        mov    dx, word ptr [eax + 1eeh]
        mov    ax, cx
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3574
        mov    word ptr [ebp - 14h], ax
loc_1_6F9D:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1eah]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        mov    al, byte ptr [ebp - 8]
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 40h
        jne    NEAR PTR loc_1_6FC9
        sub    ax, ax
        jmp    NEAR PTR loc_1_7014
loc_1_6FC9:
        mov    al, byte ptr [ebp - 4]
        and    eax, 30h
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 30h
        jne    NEAR PTR loc_1_6FE9
        mov    ax, 0fffeh
        jmp    NEAR PTR loc_1_7014
loc_1_6FE9:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3574
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 14h]
        and    ecx, 0ffffh
        sub    eax, ecx
        cmp    eax, 5bh
        jle    NEAR PTR loc_1_6F9D
loc_1_700B:
        mov    ax, 0fffeh
        jmp    NEAR PTR loc_1_7014
loc_1_7014:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_701B:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1f2h]
        mov    ax, 1
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1f0h]
        mov    ax, 0a00h
        out    dx, ax
        jmp    NEAR PTR loc_1_7049
loc_1_7049:
        jmp    NEAR PTR loc_1_704E
loc_1_704E:
        push   0bh
        db  068h, 0A8h, 07Ah, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1e6h]
        and    eax, 0ffffh
        push   eax
        call   sub_1_04E4
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 1eah]
        mov    ax, 9080h
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3574
        mov    word ptr [ebp - 0ch], ax
loc_1_7087:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1eah]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        mov    al, byte ptr [ebp - 8]
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 70h
        je     NEAR PTR loc_1_70C5
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 10h
        je     NEAR PTR loc_1_70C0
        mov    ax, 0fffdh
        jmp    NEAR PTR loc_1_70FA
loc_1_70C0:
        jmp    NEAR PTR loc_1_70CD
loc_1_70C5:
        sub    ax, ax
        jmp    NEAR PTR loc_1_70FA
loc_1_70CD:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3574
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        sub    eax, ecx
        cmp    eax, 0b4h
        jle    NEAR PTR loc_1_7087
        mov    ax, 0fffdh
        jmp    NEAR PTR loc_1_70FA
loc_1_70FA:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_7101:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_55FE
        mov    word ptr [ebp - 8], ax
        movsx  eax, word ptr [ebp - 8]
        or     eax, eax
        je     NEAR PTR loc_1_712E
        mov    ax, word ptr [ebp - 8]
        jmp    NEAR PTR loc_1_735C
loc_1_712E:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2dch]
        mov    al, byte ptr [eax + 0b5h]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_7164
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3460
        or     eax, eax
        je     NEAR PTR loc_1_7164
        mov    ax, 10h
        jmp    NEAR PTR loc_1_735C
loc_1_7164:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1eeh]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        or     ecx, 8
        mov    dx, word ptr [eax + 1eeh]
        mov    ax, cx
        out    dx, ax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6E0D
        mov    word ptr [ebp - 8], ax
        movsx  eax, word ptr [ebp - 8]
        or     eax, eax
        je     NEAR PTR loc_1_71B5
        mov    ax, word ptr [ebp - 8]
        jmp    NEAR PTR loc_1_735C
loc_1_71B5:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_701B
        mov    word ptr [ebp - 8], ax
        movsx  eax, word ptr [ebp - 8]
        or     eax, eax
        je     NEAR PTR loc_1_71D7
        mov    ax, word ptr [ebp - 8]
        jmp    NEAR PTR loc_1_735C
loc_1_71D7:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_66F6
        mov    word ptr [ebp - 8], ax
        movsx  eax, word ptr [ebp - 8]
        or     eax, eax
        je     NEAR PTR loc_1_71F9
        mov    ax, word ptr [ebp - 8]
        jmp    NEAR PTR loc_1_735C
loc_1_71F9:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5E34
        mov    word ptr [ebp - 8], ax
        movsx  eax, word ptr [ebp - 8]
        or     eax, eax
        je     NEAR PTR loc_1_721B
        mov    ax, word ptr [ebp - 8]
        jmp    NEAR PTR loc_1_735C
loc_1_721B:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1f2h]
        mov    ax, 1
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1f0h]
        mov    ax, 0a00h
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 1e2h]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1f0h]
        mov    ax, word ptr [ebp - 4]
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 1e6h]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        mov    ch, cl
        mov    edx, dword ptr [ebp - 4]
        and    edx, 0ffffh
        mov    cl, dh
        mov    word ptr [ebp - 4], cx
        mov    cx, word ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + 2b2h], cx
        mov    cx, word ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + 2b8h], cx
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 1e6h]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        mov    ch, cl
        mov    edx, dword ptr [ebp - 4]
        and    edx, 0ffffh
        mov    cl, dh
        mov    word ptr [ebp - 4], cx
        mov    cx, word ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + 2b4h], cx
        mov    cx, word ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + 2bah], cx
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 1e6h]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        mov    ch, cl
        mov    edx, dword ptr [ebp - 4]
        and    edx, 0ffffh
        mov    cl, dh
        mov    word ptr [ebp - 4], cx
        mov    cx, word ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + 2b6h], cx
        mov    cx, word ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + 2bch], cx
        mov    ecx, dword ptr [ebp + 8]
        dec    dword ptr [ecx + 2c6h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 1eah]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    ax, word ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6C5D
        sub    ax, ax
        jmp    NEAR PTR loc_1_735C
loc_1_735C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_7363:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 10h], ax
        add    dword ptr [ebp - 4], 2
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 12h], ax
        add    dword ptr [ebp - 4], 2
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 14h], ax
        sub    ax, ax
        jmp    NEAR PTR loc_1_73A9
loc_1_73A9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_73B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1ah], ax
        add    dword ptr [ebp - 4], 2
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1ch], ax
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 2ceh]
        or     eax, eax
        je     NEAR PTR loc_1_740F
        push   24h
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1ah]
        push   eax
        push   900h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5698
loc_1_740F:
        sub    ax, ax
        jmp    NEAR PTR loc_1_7417
loc_1_7417:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_741E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 2ceh]
        or     eax, eax
        je     NEAR PTR loc_1_7450
        push   24h
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0eh]
        push   eax
        push   0d00h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5698
loc_1_7450:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ax, word ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 2b0h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0f9ffh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0eh], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        and    ecx, 0ffffh
        or     eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0eh], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_741E
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_74AC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 2beh], ax
        add    dword ptr [ebp - 4], 2
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 2c0h], ax
        add    dword ptr [ebp - 4], 2
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 2c2h], ax
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 2ceh]
        or     eax, eax
        je     NEAR PTR loc_1_7558
        push   24h
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2c2h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2c0h]
        push   eax
        push   800h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5698
        movsx  eax, ax
        or     eax, eax
        je     NEAR PTR loc_1_7558
        push   24h
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2beh]
        push   eax
        push   0f00h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5698
        jmp    NEAR PTR loc_1_7560
loc_1_7558:
        sub    ax, ax
        jmp    NEAR PTR loc_1_7560
loc_1_7560:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1eah]
        mov    ax, 8880h
        out    dx, ax
        sub    ax, ax
        jmp    NEAR PTR loc_1_7585
loc_1_7585:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_758C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 14h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 12h]
        and    ecx, 0ffffh
        or     eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 10h]
        and    ecx, 0ffffh
        or     eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_1_75E3
        mov    eax, dword ptr [ebp + 8]
        add    eax, 2b2h
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_75EC
loc_1_75E3:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 10h
        mov    dword ptr [ebp - 8], eax
loc_1_75EC:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 2b8h], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 2bah], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 2bch], ax
        push   276h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68D4
        mov    word ptr [ebp - 0ch], ax
        movsx  eax, word ptr [ebp - 0ch]
        or     eax, eax
        je     NEAR PTR loc_1_764B
        mov    ax, word ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_76C2
loc_1_764B:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2d2h]
        and    eax, 0ffffh
        test   ah, 80h
        jne    NEAR PTR loc_1_76AE
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2d2h]
        and    eax, 0ffffh
        test   ah, 40h
        je     NEAR PTR loc_1_7684
        mov    ax, 0fff3h
        jmp    NEAR PTR loc_1_76C2
loc_1_7684:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2d2h]
        and    eax, 0ffffh
        test   ah, 2
        je     NEAR PTR loc_1_76A5
        mov    ax, 0fff5h
        jmp    NEAR PTR loc_1_76C2
loc_1_76A5:
        mov    ax, 0fff4h
        jmp    NEAR PTR loc_1_76C2
loc_1_76AE:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 2ceh], 1
        sub    ax, ax
        jmp    NEAR PTR loc_1_76C2
loc_1_76C2:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 2ceh], 0
        push   24h
        push   0
        push   0
        push   700h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5698
        sub    ax, ax
        jmp    NEAR PTR loc_1_76F7
loc_1_76F7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_76FE:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1eeh]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0fff7h
        mov    dx, word ptr [eax + 1eeh]
        mov    ax, cx
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2dch]
        mov    al, byte ptr [eax + 0b5h]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_7754
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_34FB
loc_1_7754:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 2ceh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1eah]
        mov    ax, 0ff00h
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_7777:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1eeh]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        or     eax, 0c0h
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1eeh]
        mov    ax, word ptr [ebp - 4]
        out    dx, ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ff7fh
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1eeh]
        mov    ax, word ptr [ebp - 4]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2a4h]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_1_7802
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        or     ecx, 100h
        mov    dx, word ptr [eax + 1eeh]
        mov    ax, cx
        out    dx, ax
loc_1_7802:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_7810:
        inc    ebx
        outsd
        db  070h, 079h
        jb     SHORT loc_1_787F
        push   43282074h
        sub    dword ptr [eax], esp
        xor    dword ptr [ecx], edi
        cmp    dword ptr [ebx], esi
        and    byte ptr [eax + 72h], dl
        outsd
        db  074h, 065h
        outsd
        outsb
        sub    al, 20h
        dec    ecx
        outsb
        arpl   word ptr [esi], bp
        and    byte ptr [ecx + 6ch], al
        insb
        and    byte ptr [edx + 69h], dl
        push   52207374h
        jae    NEAR PTR loc_1_78A5
        db  072h, 076h
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], al
        adc    byte ptr [eax], bh
        add    byte ptr [eax], al
loc_1_784D:
        add    cl, cl
loc_1_784F:
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    byte ptr [edx], al
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], al
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], al
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
loc_1_787F:
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
        add    dword ptr [edx], eax
        add    byte ptr [edx], al
        add    dword ptr [edx], eax
        add    byte ptr [ebx], al
loc_1_78A5:
        add    dword ptr [edx], eax
        add    byte ptr [ecx + eax], al
        add    al, byte ptr [eax]
        add    eax, 1000201h
        add    dword ptr [ecx], eax
        add    al, byte ptr [edx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ebx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx + eax]
        add    dword ptr [edx], eax
        add    eax, 1020101h
        add    dword ptr [edx], eax
        add    al, byte ptr [edx]
        add    dword ptr [edx], eax
        add    al, byte ptr [ecx]
        add    dword ptr [ecx], eax
        add    byte ptr [edx], al
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], al
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], al
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
        add    al, byte ptr [edx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ebx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx + eax]
        add    dword ptr [edx], eax
        push   eax
        push   edx
        dec    edi
        push   esp
        inc    ebp
        dec    edi
        dec    esi
        add    byte ptr [eax + 52h], dl
        dec    edi
        push   esp
        inc    ebp
        dec    edi
        dec    esi
        add    byte ptr [eax + 52h], dl
        dec    edi
        push   edx
        inc    ecx
        push   eax
        dec    ebp
        inc    esp
        push   edi
        dec    esi
        add    byte ptr [eax + 52h], dl
        dec    edi
        push   edx
        inc    ecx
        push   eax
        dec    ebp
        inc    esp
        push   edi
        dec    esi
        add    byte ptr [eax + 72h], dl
        outsd
        db  074h, 065h
        outsd
        outsb
        and    byte ptr [eax + 31h], dl
        xor    edi, dword ptr [ecx]
        js     SHORT loc_1_797E
        push   esp
        outsd
        imul   esp, dword ptr [ebp + 6eh], 20h
        push   edx
        imul   ebp, dword ptr [esi + 67h], 74654e20h
        db  077h, 06Fh
        jb     SHORT loc_1_79DB
        and    byte ptr [ebx + 61h], al
        jb     SHORT loc_1_79D9
        add    byte ptr cs:[eax], al
loc_1_7978:
        push   eax
        db  072h, 06Fh
        je     SHORT loc_1_79E2
        outsd
loc_1_797E:
        outsb
        and    byte ptr [eax + 31h], dl
        cmp    byte ptr [ecx], bh
        js     SHORT loc_1_79A6
        push   esp
        outsd
        imul   esp, dword ptr [ebp + 6eh], 20h
        push   edx
        imul   ebp, dword ptr [esi + 67h], 74654e20h
        ja     SHORT loc_1_7A05
        db  072h, 06Bh
        and    byte ptr [ebx + 61h], al
        db  072h, 064h
        add    byte ptr cs:[eax], al
loc_1_79A0:
        push   eax
        db  072h, 06Fh
        je     SHORT loc_1_7A0A
        outsd
loc_1_79A6:
        outsb
        and    byte ptr [eax + 31h], dl
        cmp    dword ptr [ecx], edi
        js     SHORT loc_1_79CE
        push   esp
        outsd
        imul   esp, dword ptr [ebp + 6eh], 20h
        push   edx
        imul   ebp, dword ptr [esi + 67h], 74654e20h
        db  077h, 06Fh
        jb     SHORT loc_1_7A2B
        and    byte ptr [ebx + 61h], al
        db  072h, 064h
        add    byte ptr cs:[eax], al
        dec    ecx
        outsd
        inc    edx
        popad
        db  073h, 065h
loc_1_79CE:
        add    byte ptr [eax], al
loc_1_79D0:
        dec    ecx
        outsd
        inc    edx
        popad
        jae    SHORT loc_1_7A3B
        add    byte ptr [eax], al
        dec    ecx
loc_1_79D9:
        dec    esi
        push   esp
loc_1_79DB:
        dec    esp
        jbe    NEAR PTR loc_1_7A44
        insb
        add    byte ptr [eax], al
loc_1_79E2:
        add    byte ptr [eax], al
loc_1_79E4:
        dec    ecx
        dec    esi
        push   esp
        dec    esp
        jbe    NEAR PTR loc_1_7A50
        insb
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esp
        dec    ebp
        inc    ecx
        inc    ebx
        push   656e6e61h
        insb
        add    byte ptr [eax], al
loc_1_79FC:
        inc    esp
        dec    ebp
        inc    ecx
        inc    ebx
        push   656e6e61h
loc_1_7A05:
        insb
        add    byte ptr [eax], al
        inc    ebx
        popad
loc_1_7A0A:
        db  072h, 064h
        push   ebx
        jo     SHORT loc_1_7A74
        add    byte ptr fs:[eax], al
        add    byte ptr [ebx + 61h], al
        db  072h, 064h
        push   ebx
        db  070h, 065h
        add    byte ptr fs:[eax], al
        add    byte ptr [ebx + 61h], al
        bound  ebp, qword ptr [ebp + 54h]
        jns    SHORT loc_1_7A98
        add    byte ptr gs:[eax], al
loc_1_7A2B:
        add    byte ptr [ebx + 61h], al
        bound  ebp, qword ptr [ebp + 54h]
        db  079h, 070h
        add    byte ptr gs:[eax], al
        add    byte ptr [ebx + 6ch], dl
        outsd
loc_1_7A3B:
        je     SHORT loc_1_7A8B
loc_1_7A3D:
        jne    SHORT loc_1_7AAC
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [eax], al
loc_1_7A44:
        push   ebx
loc_1_7A45:
        insb
        outsd
        db  074h, 04Eh
        jne    SHORT loc_1_7AB8
loc_1_7A4B:
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [eax], al
loc_1_7A50:
        push   ebx
        push   esp
        push   eax
loc_1_7A53:
        add    byte ptr [ebx + 54h], dl
        push   eax
loc_1_7A57:
        add    byte ptr [eax + 61h], dl
        arpl   word ptr [ebx + 65h], bp
        je     SHORT loc_1_7AB2
        imul   edi, dword ptr [edx + 65h], 61500000h
        arpl   word ptr [ebx + 65h], bp
        je     SHORT loc_1_7ABE
        imul   edi, dword ptr [edx + 65h], 4050000h
        add    ecx, dword ptr [edi]
loc_1_7A74:
        push   cs
        or     al, 0bh
        or     al, al
        jno    SHORT loc_1_7A4B
        jno    SHORT loc_1_7A3D
        db  075h, 0D0h
        db  075h, 0C0h
        jns    SHORT loc_1_7A53
        jns    SHORT loc_1_7A45
loc_1_7A85:
        jge    SHORT loc_1_7A57
        jge    SHORT loc_1_7A8D
        add    eax, dword ptr [edx]
loc_1_7A8B:
        add    dword ptr [eax], eax
loc_1_7A8D:
        add    byte ptr [eax], cl
        pop    es
        push   es
        add    eax, 20300000h
        adc    byte ptr [eax], al
loc_1_7A98:
        rol    byte ptr [eax + 400000h], 0
        add    byte ptr [eax], al
        add    eax, 0f0b0a09h
        add    byte ptr [eax], al
loc_1_7AA8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7AAC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7AB2:
        add    byte ptr [eax], al
loc_1_7AB4:
        add    byte ptr [eax], al
loc_1_7AB6:
        add    byte ptr [eax], al
loc_1_7AB8:
        add    byte ptr [eax], al
loc_1_7ABA:
        add    byte ptr [eax], al
loc_1_7ABC:
        add    byte ptr [eax], al
loc_1_7ABE:
        add    byte ptr [eax], al
loc_1_7AC0:
        add    byte ptr [eax], al
loc_1_7AC2:
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 000h, 0FFh, 0FFh, 0FFh, 0FFh
_LTEXT ends
        end
