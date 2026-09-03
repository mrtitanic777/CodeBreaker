; pci_pcode_02_02.asm -- PCI object 2 segment _PTEXT  (part 2/2)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_2_4592:
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_45BB
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_45B1
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_79E8
        add    esp, 4
loc_2_45B1:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_45BB:
        ret
sub_2_45BC:
        push   ebx
        xor    eax, eax
        push   esi
        xor    bl, bl
        cmp    byte ptr [ecx], al
        je     SHORT loc_2_45E9
loc_2_45C6:
        movzx  eax, bl
        imul   eax, eax, 2ch
        cmp    dword ptr [eax + ecx + 24h], edx
        lea    eax, [eax + ecx + 4]
        jne    SHORT loc_2_45E3
        mov    esi, dword ptr [eax + 1ch]
        and    esi, dword ptr [esp + 0ch]
        cmp    esi, dword ptr [esp + 0ch]
        je     SHORT loc_2_45E9
loc_2_45E3:
        inc    bl
        cmp    bl, byte ptr [ecx]
        jb     SHORT loc_2_45C6
loc_2_45E9:
        cmp    bl, byte ptr [ecx]
        jne    SHORT loc_2_460F
        xor    bl, bl
        cmp    bl, byte ptr [ecx]
        je     SHORT loc_2_460D
loc_2_45F3:
        movzx  eax, bl
        imul   eax, eax, 2ch
        cmp    dword ptr [eax + ecx + 24h], edx
        lea    eax, [eax + ecx + 4]
        je     SHORT loc_2_4609
        inc    bl
        cmp    bl, byte ptr [ecx]
        jb     SHORT loc_2_45F3
loc_2_4609:
        cmp    bl, byte ptr [ecx]
        jne    SHORT loc_2_460F
loc_2_460D:
        xor    eax, eax
loc_2_460F:
        pop    esi
        pop    ebx
        ret    4
sub_2_4614:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_4646
        push   0
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_7A80
        add    esp, 0ch
        push   0
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_7A80
        add    esp, 0ch
        cli
loc_2_4646:
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        ret
sub_2_464D:
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_467E
        sti
        push   0
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_7A88
        add    esp, 0ch
        push   0
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_7A88
        add    esp, 0ch
loc_2_467E:
        ret
sub_2_467F:
        mov    eax, edx
        mov    dx, cx
        movzx  ecx, byte ptr [esp + 4]
        test   ecx, ecx
        je     SHORT loc_2_4699
        cmp    ecx, 1
        je     SHORT loc_2_469C
        cmp    ecx, 2
        je     SHORT loc_2_46A0
        jmp    SHORT loc_2_46A1
loc_2_4699:
        out    dx, al
        jmp    SHORT loc_2_46A1
loc_2_469C:
        out    dx, ax
        jmp    SHORT loc_2_46A1
loc_2_46A0:
        out    dx, eax
loc_2_46A1:
        ret    4
sub_2_46A4:
        movzx  eax, dl
        test   eax, eax
        je     SHORT loc_2_46B7
        cmp    eax, 1
        je     SHORT loc_2_46C0
        cmp    eax, 2
        je     SHORT loc_2_46CA
        jmp    SHORT loc_2_46CE
loc_2_46B7:
        mov    dx, cx
        in     al, dx
        movzx  eax, al
        jmp    SHORT loc_2_46CE
loc_2_46C0:
        mov    dx, cx
        in     ax, dx
        movzx  eax, ax
        jmp    SHORT loc_2_46CE
loc_2_46CA:
        mov    dx, cx
        in     eax, dx
loc_2_46CE:
        ret
sub_2_46CF:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   ebx
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 4], 1
        push   esi
        push   edi
        je     SHORT loc_2_4734
        pushad
        mov    edi, dword ptr [ebp - 8]
        mov    eax, dword ptr [edi]
        mov    ebx, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi + 8]
        mov    edx, dword ptr [edi + 0ch]
        mov    esi, dword ptr [edi + 10h]
        mov    edi, dword ptr [edi + 14h]
        db  089h, 025h, 000h, 000h, 000h, 000h
        push   cs
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        db  08Bh, 025h, 000h, 000h, 000h, 000h
        push   edi
        mov    edi, dword ptr [ebp - 8]
        mov    dword ptr [edi], eax
        mov    dword ptr [edi + 4], ebx
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [edi + 0ch], edx
        mov    dword ptr [edi + 10h], esi
        pop    dword ptr [edi + 14h]
        popad
        jae    SHORT loc_2_4732
        mov    dword ptr [ebp - 4], 0
loc_2_4732:
        jmp    SHORT loc_2_473B
loc_2_4734:
        mov    dword ptr [ebp - 4], 0
loc_2_473B:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_4745:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        mov    eax, dword ptr [ecx]
        push   edi
        mov    esi, ecx
        xor    edi, edi
        mov    dword ptr [ebp - 8], eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 000h, 000h, 001h, 000h
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [ebp - 2], 40ah
        jb     SHORT loc_2_47AF
        push   edi
        lea    eax, [ebp - 8]
        push   4
        db  0BBh, 000h, 000h, 000h, 000h
        push   eax
        push   ebx
        push   edi
        call   sub_2_7A90
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_2_47C2
        call   sub_2_4614
        mov    ecx, esi
        call   sub_2_46CF
        mov    edi, eax
        call   sub_2_464D
        push   0
        lea    eax, [ebp - 8]
        push   4
        push   eax
        push   ebx
        push   3
        call   sub_2_7A90
        add    esp, 14h
        jmp    SHORT loc_2_47C2
loc_2_47AF:
        call   sub_2_4614
        mov    ecx, esi
        call   sub_2_46CF
        mov    edi, eax
        call   sub_2_464D
loc_2_47C2:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_47CB:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   ebx
        mov    byte ptr [ebp - 1], dl
        push   esi
        mov    bl, cl
        push   edi
        je     SHORT loc_2_47F0
        test   bl, bl
        je     SHORT loc_2_47F0
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_2_4A34
loc_2_47F0:
        call   sub_2_4614
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 001h
        je     SHORT loc_2_483E
        movzx  eax, byte ptr [ebp + 0ch]
        movzx  edx, byte ptr [ebp + 8]
        add    eax, 0b108h
        mov    dword ptr [ebp - 8], edx
        mov    dword ptr [ebp - 1ch], eax
        movzx  ecx, byte ptr [ebp - 1]
        movzx  eax, bl
        shl    eax, 8
        or     eax, ecx
        lea    ecx, [ebp - 1ch]
        mov    dword ptr [ebp - 18h], eax
        call   sub_2_46CF
        test   eax, eax
        je     SHORT loc_2_4834
        mov    edi, dword ptr [ebp - 14h]
        jmp    NEAR PTR loc_2_4A2D
loc_2_4834:
        mov    edi, 0ffffffffh
        jmp    NEAR PTR loc_2_4A2D
loc_2_483E:
        mov    byte ptr [ebp - 2], 0
        mov    byte ptr [ebp - 3], 0
        in     al, 21h
        xor    esi, esi
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_2_4946
        xor    edx, edx
        mov    edi, 0cf8h
        mov    ecx, edi
        call   sub_2_46A4
        push   esi
        mov    byte ptr [ebp - 2], al
        mov    al, byte ptr [ebp - 1]
        and    al, 7
        or     al, 78h
        movzx  ecx, al
        lea    edx, [ecx*2]
        mov    ecx, edi
        call   sub_2_467F
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_48A4
        xor    edx, edx
        mov    edi, 0cfah
        mov    ecx, edi
        call   sub_2_46A4
        push   esi
        mov    ecx, edi
        movzx  edx, bl
        mov    byte ptr [ebp - 3], al
        call   sub_2_467F
loc_2_48A4:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [ebp - 1]
        je     SHORT loc_2_48FA
        and    al, 78h
        shr    al, 3
        mov    edx, 2
        movzx  cx, al
        sub    cx, 40h
        movzx  ax, byte ptr [ebp + 8]
        shl    cx, 8
        and    ax, 0fffch
        add    cx, ax
        call   sub_2_46A4
        mov    edi, eax
        mov    al, byte ptr [ebp + 8]
        and    al, 3
        movzx  ebx, byte ptr [ebp + 0ch]
        lea    edx, [eax*8]
        db  08Bh, 004h, 09Dh, 000h, 000h, 000h, 000h
        mov    cl, dl
        shl    eax, cl
        and    edi, eax
        shr    edi, cl
        jmp    SHORT loc_2_491D
loc_2_48FA:
        and    al, 78h
        mov    edx, dword ptr [ebp + 0ch]
        shr    al, 3
        movzx  cx, al
        sub    cx, 40h
        movzx  ax, byte ptr [ebp + 8]
        shl    cx, 8
        add    cx, ax
        call   sub_2_46A4
        mov    edi, eax
loc_2_491D:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_4936
        push   0
        mov    ecx, 0cfah
        movzx  edx, byte ptr [ebp - 3]
        call   sub_2_467F
loc_2_4936:
        push   0
        mov    ecx, 0cf8h
        movzx  edx, byte ptr [ebp - 2]
        jmp    NEAR PTR loc_2_4A26
loc_2_4946:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_495E
        xor    edx, edx
        mov    ecx, 0cfbh
        call   sub_2_46A4
        mov    byte ptr [ebp - 2], al
loc_2_495E:
        mov    edx, 2
        mov    ecx, 0cf8h
        call   sub_2_46A4
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        mov    esi, eax
        je     SHORT loc_2_4989
        push   0
        mov    edx, 1
        mov    ecx, 0cfbh
        call   sub_2_467F
loc_2_4989:
        movzx  edx, bl
        movzx  eax, byte ptr [ebp - 1]
        shl    eax, 8
        push   2
        or     edx, 0ffff8000h
        mov    ecx, 0cf8h
        shl    edx, 10h
        or     edx, eax
        movzx  eax, byte ptr [ebp + 8]
        and    al, 0fch
        or     edx, eax
        call   sub_2_467F
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_49EB
        mov    edx, 2
        mov    ecx, 0cfch
        call   sub_2_46A4
        movzx  edx, byte ptr [ebp + 0ch]
        mov    edi, eax
        mov    al, byte ptr [ebp + 8]
        and    al, 3
        db  08Bh, 014h, 095h, 000h, 000h, 000h, 000h
        lea    ecx, [eax*8]
        shl    edx, cl
        and    edi, edx
        shr    edi, cl
        jmp    SHORT loc_2_4A04
loc_2_49EB:
        xor    cx, cx
        mov    edx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ebp + 8]
        and    cx, 3
        add    cx, 0cfch
        call   sub_2_46A4
        mov    edi, eax
loc_2_4A04:
        push   2
        mov    edx, esi
        mov    ecx, 0cf8h
        call   sub_2_467F
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_4A2B
        push   0
        mov    ecx, 0cfbh
        movzx  edx, byte ptr [ebp - 2]
loc_2_4A26:
        call   sub_2_467F
loc_2_4A2B:
        in     al, 21h
loc_2_4A2D:
        call   sub_2_464D
        mov    eax, edi
loc_2_4A34:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_2_4A3D:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   ebx
        mov    byte ptr [ebp - 1], dl
        push   esi
        mov    bl, cl
        push   edi
        je     SHORT loc_2_4A5C
        test   bl, bl
        jne    NEAR PTR loc_2_4CAF
loc_2_4A5C:
        call   sub_2_4614
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 001h
        je     SHORT loc_2_4A9F
        movzx  eax, byte ptr [ebp + 10h]
        add    eax, 0b10bh
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 1ch], eax
        mov    dword ptr [ebp - 14h], edx
        movzx  ecx, byte ptr [ebp - 1]
        movzx  eax, bl
        shl    eax, 8
        or     eax, ecx
        movzx  ecx, byte ptr [ebp + 8]
        mov    dword ptr [ebp - 18h], eax
        mov    dword ptr [ebp - 8], ecx
        lea    ecx, [ebp - 1ch]
        call   sub_2_46CF
        jmp    NEAR PTR loc_2_4CAA
loc_2_4A9F:
        mov    byte ptr [ebp - 2], 0
        mov    byte ptr [ebp - 3], 0
        in     al, 21h
        xor    esi, esi
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_2_4BB6
        xor    edx, edx
        mov    edi, 0cf8h
        mov    ecx, edi
        call   sub_2_46A4
        push   esi
        mov    byte ptr [ebp - 2], al
        mov    al, byte ptr [ebp - 1]
        and    al, 7
        or     al, 78h
        movzx  ecx, al
        lea    edx, [ecx*2]
        mov    ecx, edi
        call   sub_2_467F
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_4B05
        xor    edx, edx
        mov    edi, 0cfah
        mov    ecx, edi
        call   sub_2_46A4
        push   esi
        mov    ecx, edi
        movzx  edx, bl
        mov    byte ptr [ebp - 3], al
        call   sub_2_467F
loc_2_4B05:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_4B66
        mov    al, byte ptr [ebp - 1]
        and    al, 78h
        shr    al, 3
        mov    edx, 2
        movzx  si, al
        sub    si, 40h
        movzx  ax, byte ptr [ebp + 8]
        shl    si, 8
        and    ax, 0fffch
        add    si, ax
        mov    ecx, esi
        call   sub_2_46A4
        movzx  ebx, byte ptr [ebp + 10h]
        mov    dl, byte ptr [ebp + 8]
        db  08Bh, 03Ch, 09Dh, 000h, 000h, 000h, 000h
        and    dl, 3
        push   2
        lea    ecx, [edx*8]
        shl    edi, cl
        not    edi
        shl    dword ptr [ebp + 0ch], cl
        and    eax, edi
        mov    ecx, esi
        or     eax, dword ptr [ebp + 0ch]
        mov    edx, eax
        jmp    SHORT loc_2_4B88
loc_2_4B66:
        mov    al, byte ptr [ebp - 1]
        and    al, 78h
        mov    edx, dword ptr [ebp + 0ch]
        shr    al, 3
        push   dword ptr [ebp + 10h]
        movzx  cx, al
        sub    cx, 40h
        movzx  ax, byte ptr [ebp + 8]
        shl    cx, 8
        add    cx, ax
loc_2_4B88:
        call   sub_2_467F
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_4BA6
        push   0
        mov    ecx, 0cfah
        movzx  edx, byte ptr [ebp - 3]
        call   sub_2_467F
loc_2_4BA6:
        push   0
        mov    ecx, 0cf8h
        movzx  edx, byte ptr [ebp - 2]
        jmp    NEAR PTR loc_2_4CA3
loc_2_4BB6:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_4BCE
        xor    edx, edx
        mov    ecx, 0cfbh
        call   sub_2_46A4
        mov    byte ptr [ebp - 2], al
loc_2_4BCE:
        mov    edx, 2
        mov    ecx, 0cf8h
        call   sub_2_46A4
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        mov    esi, eax
        je     SHORT loc_2_4BF9
        push   0
        mov    edx, 1
        mov    ecx, 0cfbh
        call   sub_2_467F
loc_2_4BF9:
        movzx  edx, bl
        movzx  eax, byte ptr [ebp - 1]
        shl    eax, 8
        push   2
        or     edx, 0ffff8000h
        mov    ecx, 0cf8h
        shl    edx, 10h
        or     edx, eax
        movzx  eax, byte ptr [ebp + 8]
        and    al, 0fch
        or     edx, eax
        call   sub_2_467F
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_4C67
        mov    edx, 2
        mov    edi, 0cfch
        mov    ecx, edi
        call   sub_2_46A4
        movzx  ebx, byte ptr [ebp + 10h]
        mov    edx, eax
        mov    al, byte ptr [ebp + 8]
        and    al, 3
        db  08Bh, 01Ch, 09Dh, 000h, 000h, 000h, 000h
        push   2
        lea    ecx, [eax*8]
        shl    ebx, cl
        not    ebx
        shl    dword ptr [ebp + 0ch], cl
        and    edx, ebx
        mov    ecx, edi
        or     edx, dword ptr [ebp + 0ch]
        jmp    SHORT loc_2_4C7C
loc_2_4C67:
        xor    cx, cx
        mov    edx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ebp + 8]
        push   dword ptr [ebp + 10h]
        and    cx, 3
        add    cx, 0cfch
loc_2_4C7C:
        call   sub_2_467F
        push   2
        mov    edx, esi
        mov    ecx, 0cf8h
        call   sub_2_467F
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_4CA8
        push   0
        mov    ecx, 0cfbh
        movzx  edx, byte ptr [ebp - 2]
loc_2_4CA3:
        call   sub_2_467F
loc_2_4CA8:
        in     al, 21h
loc_2_4CAA:
        call   sub_2_464D
loc_2_4CAF:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_4CB8:
        mov    eax, 400h
        ret
loc_2_4CBE:
        mov    edx, dword ptr [esp + 8]
        push   2
        push   dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 0ch]
        call   sub_2_47CB
        ret
loc_2_4CD2:
        push   ebp
        mov    edx, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   2
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        call   sub_2_4A3D
        pop    ebp
        ret
sub_2_4CEB:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        lea    eax, [ebp - 1]
        push   esi
        mov    bl, cl
        mov    byte ptr [ebp - 2], dl
        push   dword ptr [ebp - 2]
        push   eax
        db  0FFh, 015h, 004h, 000h, 000h, 000h
        add    esp, 8
        test   eax, eax
        je     SHORT loc_2_4D13
        xor    eax, eax
        jmp    NEAR PTR loc_2_4DD1
loc_2_4D13:
        lea    eax, [ebp - 8]
        push   eax
        db  0FFh, 015h, 00Ch, 000h, 000h, 000h
        add    esp, 4
        test   eax, eax
        je     SHORT loc_2_4D2B
        xor    eax, eax
        jmp    NEAR PTR loc_2_4DD1
loc_2_4D2B:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_2_4D44
        mov    eax, 1
        mov    cl, byte ptr [ebp - 1]
        shl    eax, cl
        test   dword ptr [ebp - 8], eax
        jne    SHORT loc_2_4D44
        mov    byte ptr [ebp - 1], 0
loc_2_4D44:
        cmp    byte ptr [ebp - 1], bl
        mov    esi, 0
        test   bl, bl
        je     SHORT loc_2_4D59
        mov    esi, 1
        mov    cl, bl
        shl    esi, cl
loc_2_4D59:
        cmp    byte ptr [ebp - 1], 0
        mov    eax, 0
        je     SHORT loc_2_4D6E
        mov    eax, 1
        mov    cl, byte ptr [ebp - 1]
        shl    eax, cl
loc_2_4D6E:
        or     esi, eax
        call   sub_2_4614
        in     al, 0a1h
        mov    ah, al
        in     al, 21h
        mov    word ptr [ebp - 4], ax
        or     al, 0ffh
        out    21h, al
        out    0a1h, al
        mov    eax, esi
        not    eax
        and    dword ptr [ebp - 8], eax
        push   dword ptr [ebp - 8]
        db  0FFh, 015h, 008h, 000h, 000h, 000h
        add    esp, 4
        push   dword ptr [ebp - 2]
        push   ebx
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        add    esp, 8
        push   esi
        call   sub_2_7A98
        add    esp, 4
        or     dword ptr [ebp - 8], esi
        push   dword ptr [ebp - 8]
        db  0FFh, 015h, 008h, 000h, 000h, 000h
        add    esp, 4
        mov    ax, word ptr [ebp - 4]
        out    21h, al
        mov    al, ah
        out    0a1h, al
        call   sub_2_464D
        mov    eax, 1
loc_2_4DD1:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_4DD7:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    byte ptr [ebp - 2], dl
        push   esi
        mov    byte ptr [ebp - 1], cl
        call   sub_2_4614
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        mov    bl, byte ptr [esi + 26h]
        push   ebx
        mov    al, byte ptr [esi + 24h]
        push   eax
        call   sub_2_47CB
        mov    edx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [esi + 27h]
        push   ebx
        mov    dword ptr [edx], eax
        shr    eax, cl
        mov    cl, byte ptr [esi + 28h]
        mov    dword ptr [edx], eax
        shl    eax, cl
        mov    cl, byte ptr [esi + 25h]
        mov    dword ptr [edx], eax
        push   ecx
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_47CB
        mov    edx, dword ptr [ebp + 10h]
        mov    cl, byte ptr [esi + 27h]
        mov    dword ptr [edx], eax
        shr    eax, cl
        mov    cl, byte ptr [esi + 28h]
        mov    dword ptr [edx], eax
        shl    eax, cl
        mov    dword ptr [edx], eax
        call   sub_2_464D
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_2_4E43:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    byte ptr [ebp - 2], dl
        push   esi
        mov    byte ptr [ebp - 1], cl
        call   sub_2_4614
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        mov    bl, byte ptr [esi + 26h]
        mov    cl, byte ptr [esi + 28h]
        shr    eax, cl
        push   ebx
        mov    cl, byte ptr [esi + 27h]
        shl    eax, cl
        push   eax
        mov    cl, byte ptr [esi + 24h]
        push   ecx
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_4A3D
        push   ebx
        mov    eax, dword ptr [ebp + 10h]
        mov    cl, byte ptr [esi + 28h]
        mov    edx, dword ptr [ebp - 2]
        shr    eax, cl
        mov    cl, byte ptr [esi + 27h]
        shl    eax, cl
        push   eax
        mov    cl, byte ptr [esi + 25h]
        push   ecx
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_4A3D
        call   sub_2_464D
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_2_4EA5:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    byte ptr [ebp - 1], dl
        push   esi
        mov    bl, cl
        push   4
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        call   sub_2_58D3
        mov    si, ax
        mov    cl, byte ptr [ebp + 8]
        test   cl, cl
        je     SHORT loc_2_4ED5
        mov    ax, 1
        shl    ax, cl
        mov    word ptr [ebp - 4], ax
        jmp    SHORT loc_2_4EDB
loc_2_4ED5:
        mov    word ptr [ebp - 4], 0
loc_2_4EDB:
        call   sub_2_4614
        in     al, 0a1h
        mov    ah, al
        in     al, 21h
        push   eax
        or     ax, word ptr [ebp - 4]
        out    21h, al
        mov    al, ah
        out    0a1h, al
        or     si, word ptr [ebp + 0ch]
        push   2
        movzx  eax, si
        push   eax
        mov    edx, dword ptr [ebp - 1]
        push   4
        mov    ecx, ebx
        call   sub_2_4A3D
        pop    eax
        out    21h, al
        mov    al, ah
        out    0a1h, al
        call   sub_2_464D
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_2_4F1B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        mov    edi, edx
        push   2
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 10h]
        call   sub_2_47CB
        test   eax, 0fffffff0h
        mov    ebx, eax
        jne    SHORT loc_2_4F4B
        cmp    dword ptr [ebp + 14h], 0
        je     SHORT loc_2_4F4B
        xor    eax, eax
        jmp    SHORT loc_2_4FA5
loc_2_4F4B:
        call   sub_2_4614
        push   2
        mov    edx, dword ptr [ebp + 0ch]
        push   -1
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 10h]
        call   sub_2_4A3D
        push   2
        mov    edx, dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        call   sub_2_47CB
        push   2
        mov    edx, dword ptr [ebp + 0ch]
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 4], eax
        call   sub_2_4A3D
        call   sub_2_464D
        test   esi, esi
        je     SHORT loc_2_4F91
        mov    dword ptr [esi], ebx
loc_2_4F91:
        test   edi, edi
        je     SHORT loc_2_4F9A
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edi], eax
loc_2_4F9A:
        mov    eax, dword ptr [ebp - 4]
        and    al, 0f0h
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
loc_2_4FA5:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
sub_2_4FAE:
        call   sub_2_406B
        movzx  eax, al
        test   eax, eax
        je     SHORT loc_2_4FC3
        cmp    eax, 1
        je     SHORT loc_2_4FC7
        xor    al, al
        jmp    SHORT loc_2_4FC9
loc_2_4FC3:
        mov    al, 30h
        jmp    SHORT loc_2_4FC9
loc_2_4FC7:
        mov    al, 38h
loc_2_4FC9:
        ret
sub_2_4FCA:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    byte ptr [ebp - 2], dl
        push   esi
        mov    edx, dword ptr [ebp - 2]
        push   edi
        mov    byte ptr [ebp - 1], cl
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_4FAE
        mov    bl, al
        mov    eax, 0
        test   bl, bl
        je     SHORT loc_2_5047
        call   sub_2_4614
        push   2
        mov    edx, dword ptr [ebp - 2]
        push   ebx
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_47CB
        mov    esi, eax
        push   2
        or     eax, 0fffff800h
        mov    edx, dword ptr [ebp - 2]
        push   eax
        mov    ecx, dword ptr [ebp - 1]
        push   ebx
        call   sub_2_4A3D
        push   2
        mov    edx, dword ptr [ebp - 2]
        push   ebx
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_47CB
        mov    edi, eax
        push   2
        push   esi
        mov    edx, dword ptr [ebp - 2]
        push   ebx
        mov    ecx, dword ptr [ebp - 1]
        not    edi
        or     edi, 7ffh
        call   sub_2_4A3D
        call   sub_2_464D
        lea    eax, [edi + 1]
loc_2_5047:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_504E:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        mov    dword ptr [ebp - 14h], edx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    esi, ecx
        push   ebx
        lea    eax, [ebp - 2]
        push   ebx
        lea    edx, [ebp - 1]
        push   eax
        call   sub_2_4212
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_4FAE
        mov    byte ptr [ebp - 3], al
        cmp    al, bl
        jne    SHORT loc_2_5088
        mov    eax, 13h
        jmp    NEAR PTR loc_2_524D
loc_2_5088:
        push   2
        lea    eax, [ebp - 1ch]
        push   esi
        push   eax
        call   sub_2_7918
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_50A5
        mov    eax, 13h
        jmp    NEAR PTR loc_2_524D
loc_2_50A5:
        xor    esi, esi
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [ebp - 10h], eax
loc_2_50AD:
        push   0
        lea    ecx, [ebp - 10h]
        push   0
        push   1
        push   dword ptr [ebp - 10h]
        push   ecx
        call   sub_2_7908
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_2_50DC
        mov    ecx, dword ptr [ebp - 10h]
        cmp    dword ptr [ecx + 24h], 0fch
        jne    SHORT loc_2_50AD
        mov    esi, dword ptr [ecx + 4]
        mov    ebx, dword ptr [ecx + 14h]
        test   eax, eax
        je     SHORT loc_2_50E6
loc_2_50DC:
        mov    eax, 13h
        jmp    NEAR PTR loc_2_524D
loc_2_50E6:
        cmp    dword ptr [ebp - 14h], ebx
        jbe    SHORT loc_2_50F5
        mov    eax, 13h
        jmp    NEAR PTR loc_2_524D
loc_2_50F5:
        mov    eax, ebx
        sub    eax, dword ptr [ebp - 14h]
        cmp    eax, dword ptr [ebp + 0ch]
        jae    SHORT loc_2_5109
        mov    eax, 13h
        jmp    NEAR PTR loc_2_524D
loc_2_5109:
        cmp    dword ptr [ebp + 0ch], 0
        jne    SHORT loc_2_5116
        xor    eax, eax
        jmp    NEAR PTR loc_2_524D
loc_2_5116:
        add    ebx, 0fffh
        push   0
        shr    ebx, 0ch
        push   ebx
        push   80080000h
        call   sub_2_7AB8
        add    esp, 0ch
        mov    dword ptr [ebp - 0ch], eax
        cmp    eax, -1
        je     NEAR PTR loc_2_5235
        mov    edi, eax
        push   40000000h
        shr    edi, 0ch
        mov    eax, esi
        shr    eax, 0ch
        push   eax
        push   ebx
        push   edi
        call   sub_2_7AB0
        add    esp, 10h
        test   eax, eax
        je     NEAR PTR loc_2_5235
        push   0
        push   ebx
        push   edi
        call   sub_2_7AA8
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_2_5235
        call   sub_2_4614
        push   2
        mov    edx, dword ptr [ebp - 2]
        push   dword ptr [ebp - 3]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_47CB
        push   1
        mov    edx, dword ptr [ebp - 2]
        push   4
        mov    ecx, dword ptr [ebp - 1]
        or     esi, 1
        mov    dword ptr [ebp - 18h], eax
        call   sub_2_47CB
        mov    word ptr [ebp - 6], ax
        push   2
        push   esi
        mov    edx, dword ptr [ebp - 2]
        push   dword ptr [ebp - 3]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_4A3D
        mov    bx, word ptr [ebp - 6]
        and    bx, 2
        jne    SHORT loc_2_51D3
        mov    ax, word ptr [ebp - 6]
        push   2
        or     al, 2
        mov    edx, dword ptr [ebp - 2]
        movzx  ecx, ax
        push   ecx
        push   4
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_4A3D
loc_2_51D3:
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_2_51F3
        mov    eax, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp - 14h]
        add    esi, dword ptr [ebp - 0ch]
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_2_51F3:
        test   bx, bx
        jne    SHORT loc_2_520C
        push   2
        mov    edx, dword ptr [ebp - 2]
        movzx  eax, word ptr [ebp - 6]
        push   eax
        mov    ecx, dword ptr [ebp - 1]
        push   4
        call   sub_2_4A3D
loc_2_520C:
        push   2
        mov    edx, dword ptr [ebp - 2]
        push   dword ptr [ebp - 18h]
        push   dword ptr [ebp - 3]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_4A3D
        call   sub_2_464D
        push   0
        push   dword ptr [ebp - 0ch]
        call   sub_2_7AA0
        add    esp, 8
        xor    eax, eax
        jmp    SHORT loc_2_524D
loc_2_5235:
        cmp    dword ptr [ebp - 0ch], -1
        je     SHORT loc_2_5248
        push   0
        push   dword ptr [ebp - 0ch]
        call   sub_2_7AA0
        add    esp, 8
loc_2_5248:
        mov    eax, 2
loc_2_524D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_2_5256:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    byte ptr [ebp - 1], dl
        push   esi
        mov    bl, cl
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        call   sub_2_406B
        movzx  eax, al
        cmp    eax, 1
        je     SHORT loc_2_527E
        cmp    eax, 2
        je     SHORT loc_2_52F4
        xor    eax, eax
        jmp    SHORT loc_2_52F9
loc_2_527E:
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        call   sub_2_4075
        test   eax, 40000h
        je     SHORT loc_2_5293
        xor    eax, eax
        jmp    SHORT loc_2_52F9
loc_2_5293:
        call   sub_2_4614
        push   0
        mov    edx, dword ptr [ebp - 1]
        push   1ch
        mov    ecx, ebx
        call   sub_2_47CB
        test   al, al
        je     SHORT loc_2_52B0
        mov    byte ptr [ebp - 2], 0f0h
        jmp    SHORT loc_2_52E2
loc_2_52B0:
        xor    esi, esi
        mov    edx, dword ptr [ebp - 1]
        push   esi
        mov    ecx, ebx
        push   0f0h
        push   1ch
        call   sub_2_4A3D
        push   esi
        mov    edx, dword ptr [ebp - 1]
        push   1ch
        mov    ecx, ebx
        call   sub_2_47CB
        push   esi
        mov    edx, dword ptr [ebp - 1]
        push   esi
        mov    ecx, ebx
        push   1ch
        mov    byte ptr [ebp - 2], al
        call   sub_2_4A3D
loc_2_52E2:
        call   sub_2_464D
        mov    al, byte ptr [ebp - 2]
        sub    al, 0f0h
        cmp    al, 1
        sbb    eax, eax
        neg    eax
        jmp    SHORT loc_2_52F9
loc_2_52F4:
        mov    eax, 1
loc_2_52F9:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_52FF:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        mov    byte ptr [ebp - 1], dl
        push   esi
        mov    bl, byte ptr [ebp + 8]
        push   edi
        mov    edx, ebx
        push   0
        mov    byte ptr [ebp - 2], cl
        mov    ecx, dword ptr [ebp - 2]
        call   sub_2_58E1
        mov    edi, eax
        push   0
        mov    edx, ebx
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58E1
        cmp    edi, eax
        je     SHORT loc_2_5336
        xor    eax, eax
        jmp    NEAR PTR loc_2_5459
loc_2_5336:
        mov    edx, ebx
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_406B
        test   al, al
        je     SHORT loc_2_534C
        cmp    al, 1
        je     SHORT loc_2_534C
        cmp    al, 2
        jne    SHORT loc_2_53C8
loc_2_534C:
        push   3ch
        mov    edx, ebx
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58C5
        push   3ch
        mov    edx, ebx
        mov    ecx, dword ptr [ebp - 2]
        mov    byte ptr [ebp - 5], al
        call   sub_2_58C5
        cmp    al, byte ptr [ebp - 5]
        je     SHORT loc_2_5373
        xor    eax, eax
        jmp    NEAR PTR loc_2_5459
loc_2_5373:
        mov    al, byte ptr [ebp - 5]
        mov    edx, ebx
        not    al
        push   eax
        mov    ecx, dword ptr [ebp - 1]
        push   3ch
        call   sub_2_58EF
        push   3ch
        mov    edx, ebx
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58C5
        mov    byte ptr [ebp - 3], al
        cmp    al, byte ptr [ebp - 5]
        je     SHORT loc_2_53C8
        push   3ch
        mov    edx, ebx
        mov    ecx, dword ptr [ebp - 2]
        call   sub_2_58C5
        mov    edx, ebx
        mov    ecx, dword ptr [ebp - 1]
        push   dword ptr [ebp - 5]
        push   3ch
        mov    byte ptr [ebp - 2], al
        call   sub_2_58EF
        mov    al, byte ptr [ebp - 2]
        sub    al, byte ptr [ebp - 3]
        cmp    al, 1
        sbb    eax, eax
        neg    eax
        jmp    NEAR PTR loc_2_5459
loc_2_53C8:
        xor    esi, esi
        call   sub_2_4614
        push   1
        mov    edx, ebx
        push   4
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_47CB
        mov    di, ax
        push   1
        push   4
        mov    edx, ebx
        mov    ecx, dword ptr [ebp - 2]
        call   sub_2_47CB
        cmp    ax, di
        jne    SHORT loc_2_5452
        mov    ax, di
        push   2
        xor    ax, 30bh
        mov    edx, ebx
        movzx  ecx, ax
        push   ecx
        push   4
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_4A3D
        push   1
        mov    edx, ebx
        push   4
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_47CB
        mov    word ptr [ebp - 6], ax
        cmp    ax, di
        je     SHORT loc_2_5452
        push   1
        mov    edx, ebx
        push   4
        mov    ecx, dword ptr [ebp - 2]
        call   sub_2_47CB
        mov    si, ax
        push   2
        mov    edx, ebx
        mov    ecx, dword ptr [ebp - 1]
        movzx  eax, di
        push   eax
        push   4
        call   sub_2_4A3D
        sub    si, word ptr [ebp - 6]
        cmp    si, 1
        sbb    esi, esi
        neg    esi
loc_2_5452:
        call   sub_2_464D
        mov    eax, esi
loc_2_5459:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_5462:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        call   sub_2_4614
        xor    bl, bl
loc_2_5470:
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 00Dh, 000h, 010h, 000h
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_549B
        sti
        push   0
        push   2710h
        call   sub_2_7AC0
        add    esp, 8
        cli
        inc    bl
        cmp    bl, 64h
        jb     SHORT loc_2_5470
loc_2_549B:
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_54A0:
        push   ebx
        lea    eax, [ecx + 7]
        push   esi
        push   edi
        mov    word ptr [edx], ax
        push   ebp
        mov    esi, edx
        xor    edi, edi
        xor    ebp, ebp
loc_2_54B0:
        movzx  edx, cx
        lea    eax, [edx + 7]
        mov    edx, eax
        in     al, dx
        movzx  edx, cx
        mov    bl, al
        add    edx, 206h
        in     al, dx
        cmp    al, bl
        jne    SHORT loc_2_54D5
        cmp    edi, 1
        je     SHORT loc_2_54E2
        mov    edi, 1
        jmp    SHORT loc_2_54D7
loc_2_54D5:
        xor    edi, edi
loc_2_54D7:
        inc    ebp
        cmp    ebp, 1388h
        jb     SHORT loc_2_54B0
        jmp    SHORT loc_2_54EA
loc_2_54E2:
        add    cx, 206h
        mov    word ptr [esi], cx
loc_2_54EA:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_2_54EF:
        push   esi
        push   edi
        mov    si, cx
        xor    edi, edi
loc_2_54F6:
        mov    dx, si
        in     al, dx
        test   al, 80h
        je     SHORT loc_2_551A
        push   0
        add    edi, 64h
        push   64h
        call   sub_2_7AC0
        add    esp, 8
        cmp    edi, 1c9c380h
        jb     SHORT loc_2_54F6
        xor    eax, eax
loc_2_5517:
        pop    edi
        pop    esi
        ret
loc_2_551A:
        mov    eax, 1
        jmp    SHORT loc_2_5517
sub_2_5521:
        push   esi
        push   edi
        mov    si, cx
        xor    edi, edi
loc_2_5528:
        mov    dx, si
        in     al, dx
        test   al, 8
        jne    SHORT loc_2_554C
        push   0
        add    edi, 64h
        push   64h
        call   sub_2_7AC0
        add    esp, 8
        cmp    edi, 0f4240h
        jb     SHORT loc_2_5528
        xor    eax, eax
loc_2_5549:
        pop    edi
        pop    esi
        ret
loc_2_554C:
        mov    eax, 1
        jmp    SHORT loc_2_5549
sub_2_5553:
        push   ebp
        mov    ebp, esp
        sub    esp, 204h
        mov    word ptr [ebp - 4], cx
        push   esi
        push   edi
        lea    edx, [ebp - 2]
        xor    edi, edi
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_54A0
        mov    ecx, dword ptr [ebp - 2]
        call   sub_2_54EF
        test   eax, eax
        je     SHORT loc_2_55CA
        mov    al, 0ech
        mov    ecx, dword ptr [ebp - 2]
        movzx  esi, word ptr [ebp - 4]
        add    esi, 7
        mov    edx, esi
        out    dx, al
        call   sub_2_54EF
        test   eax, eax
        je     SHORT loc_2_55CA
        mov    dx, word ptr [ebp - 2]
        in     al, dx
        test   al, 8
        jne    SHORT loc_2_55AC
        test   al, 1
        jne    SHORT loc_2_55B1
        mov    ecx, dword ptr [ebp - 2]
        call   sub_2_5521
        test   eax, eax
        je     SHORT loc_2_55B1
loc_2_55AC:
        mov    edi, 1
loc_2_55B1:
        test   edi, edi
        je     SHORT loc_2_55CA
        mov    edx, esi
        in     al, dx
        mov    ecx, 100h
        movzx  edx, word ptr [ebp - 4]
        lea    edi, [ebp - 204h]
        rep insw
loc_2_55CA:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_55D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        mov    word ptr [ebp - 6], cx
        push   ebx
        push   esi
        mov    byte ptr [ebp - 1], dl
        push   edi
        xor    esi, esi
        mov    dword ptr [ebp - 14h], esi
        call   sub_2_5462
        mov    byte ptr [ebp - 2], 0
        mov    byte ptr [ebp - 10h], 0a0h
        movzx  ecx, word ptr [ebp - 6]
        add    ecx, 6
        mov    edx, ecx
        mov    dword ptr [ebp - 0ch], ecx
        in     al, dx
        movzx  ecx, word ptr [ebp - 6]
        mov    byte ptr [ebp - 3], al
        lea    edi, [ecx + 4]
loc_2_5609:
        mov    edx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [ebp - 10h]
        out    dx, al
        mov    edx, edi
        push   0
        in     al, dx
        push   64h
        mov    byte ptr [ebp - 4], al
        xor    al, al
        out    dx, al
        mov    al, 55h
        out    dx, al
        call   sub_2_7AC0
        add    esp, 8
        mov    edx, edi
        in     al, dx
        cmp    al, 55h
        jne    NEAR PTR loc_2_56B5
        mov    al, 0aah
        push   0
        out    dx, al
        push   64h
        call   sub_2_7AC0
        add    esp, 8
        mov    edx, edi
        in     al, dx
        cmp    al, 0aah
        jne    SHORT loc_2_56B5
        push   0
        mov    edx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_47CB
        push   0
        mov    edx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        movzx  ebx, byte ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_47CB
        test   ebx, eax
        jne    SHORT loc_2_56B5
        mov    dword ptr [ebp - 14h], 1
        mov    ecx, dword ptr [ebp - 6]
        call   sub_2_5553
        push   0
        mov    edx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_47CB
        and    eax, ebx
        cmp    eax, 1
        sbb    esi, esi
        inc    esi
        je     SHORT loc_2_56B5
        mov    eax, 1
        mov    cl, byte ptr [ebp + 14h]
        shl    eax, cl
        push   eax
        call   sub_2_7A98
        movzx  ecx, word ptr [ebp - 6]
        add    esp, 4
        lea    edx, [ecx + 7]
        in     al, dx
loc_2_56B5:
        mov    edx, edi
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        push   0
        push   64h
        call   sub_2_7AC0
        add    esp, 8
        cmp    dword ptr [ebp - 14h], 0
        jne    SHORT loc_2_56E3
        movzx  eax, byte ptr [ebp - 10h]
        add    al, 10h
        inc    byte ptr [ebp - 2]
        cmp    byte ptr [ebp - 2], 2
        mov    byte ptr [ebp - 10h], al
        jb     NEAR PTR loc_2_5609
loc_2_56E3:
        mov    edx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [ebp - 3]
        out    dx, al
        call   sub_2_464D
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
sub_2_56FA:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        mov    byte ptr [ebp - 2], cl
        push   esi
        xor    bl, bl
        mov    byte ptr [ebp - 1], 0
        push   edi
        call   sub_2_4614
        mov    al, byte ptr [ebp - 2]
        xor    esi, esi
        and    al, 78h
        mov    edi, 0cf8h
        shr    al, 3
        movzx  ax, al
        shl    ax, 8
        mov    word ptr [ebp - 10h], ax
loc_2_572C:
        push   2
        mov    edx, dword ptr [ebp - 2]
        push   ebx
        xor    ecx, ecx
        call   sub_2_47CB
        xor    edx, edx
        mov    ecx, edi
        mov    dword ptr [ebp - 0ch], eax
        call   sub_2_46A4
        push   esi
        xor    edx, edx
        mov    ecx, edi
        mov    byte ptr [ebp - 3], al
        call   sub_2_467F
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_5776
        xor    edx, edx
        mov    ecx, 0cfah
        call   sub_2_46A4
        push   esi
        xor    edx, edx
        mov    ecx, 0cfah
        mov    byte ptr [ebp - 1], al
        call   sub_2_467F
loc_2_5776:
        movzx  cx, bl
        add    cx, word ptr [ebp - 10h]
        mov    edx, 2
        sub    cx, 4000h
        call   sub_2_46A4
        mov    dword ptr [ebp - 8], eax
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_57A7
        push   esi
        mov    ecx, 0cfah
        movzx  edx, byte ptr [ebp - 1]
        call   sub_2_467F
loc_2_57A7:
        push   esi
        mov    ecx, edi
        movzx  edx, byte ptr [ebp - 3]
        call   sub_2_467F
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 8], eax
        jne    SHORT loc_2_57C7
        add    bl, 4
        cmp    bl, 10h
        jb     NEAR PTR loc_2_572C
loc_2_57C7:
        sub    bl, 10h
        call   sub_2_464D
        pop    edi
        cmp    bl, 1
        sbb    eax, eax
        pop    esi
        inc    eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_57DC:
        nop
        ret
sub_2_57DE:
        sub    esp, 4
        mov    byte ptr [esp + 3], dl
        push   ebx
        mov    bl, cl
        push   9
        mov    edx, dword ptr [esp + 0bh]
        mov    ecx, ebx
        call   sub_2_58C5
        test   al, 1
        mov    eax, 1
        jne    SHORT loc_2_5814
        push   0eh
        mov    edx, ebx
        push   4
        mov    ecx, 1f0h
        push   50h
        push   dword ptr [esp + 13h]
        call   sub_2_55D0
loc_2_5814:
        pop    ebx
        add    esp, 4
        ret
sub_2_5819:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    byte ptr [ebp - 1], dl
        mov    bl, cl
        push   51h
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        call   sub_2_58C5
        test   al, 8
        jne    SHORT loc_2_5839
        xor    eax, eax
        jmp    SHORT loc_2_5863
loc_2_5839:
        push   9
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        call   sub_2_58C5
        test   al, 4
        mov    eax, 1
        jne    SHORT loc_2_5863
        push   0fh
        mov    edx, ebx
        push   10h
        mov    ecx, 170h
        push   57h
        push   dword ptr [ebp - 1]
        call   sub_2_55D0
loc_2_5863:
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_5868:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    byte ptr [ebp - 1], dl
        mov    bl, cl
        push   0
        push   18h
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        call   sub_2_58EF
        push   0
        mov    edx, dword ptr [ebp - 1]
        push   19h
        mov    ecx, ebx
        call   sub_2_58EF
        push   0
        mov    edx, dword ptr [ebp - 1]
        push   1ah
        mov    ecx, ebx
        call   sub_2_58EF
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_58A3:
        sub    esp, 18h
        mov    dword ptr [esp], 0b101h
        lea    ecx, [esp]
        call   sub_2_4745
        test   eax, eax
        mov    al, 0
        je     SHORT loc_2_58C1
        mov    al, byte ptr [esp + 8]
loc_2_58C1:
        add    esp, 18h
        ret
sub_2_58C5:
        push   0
        push   dword ptr [esp + 8]
        call   sub_2_47CB
        ret    4
sub_2_58D3:
        push   1
        push   dword ptr [esp + 8]
        call   sub_2_47CB
        ret    4
sub_2_58E1:
        push   2
        push   dword ptr [esp + 8]
        call   sub_2_47CB
        ret    4
sub_2_58EF:
        movzx  eax, byte ptr [esp + 8]
        push   0
        push   eax
        push   dword ptr [esp + 0ch]
        call   sub_2_4A3D
        ret    8
sub_2_5903:
        movzx  eax, word ptr [esp + 8]
        push   1
        push   eax
        push   dword ptr [esp + 0ch]
        call   sub_2_4A3D
        ret    8
sub_2_5917:
        push   2
        push   dword ptr [esp + 0ch]
        push   dword ptr [esp + 0ch]
        call   sub_2_4A3D
        ret    8
sub_2_5929:
        push   ebp
        mov    ebp, esp
        sub    esp, 38h
        push   ebx
        mov    byte ptr [ebp - 1], dl
        push   esi
        mov    edx, dword ptr [ebp + 8]
        push   edi
        mov    esi, ecx
        mov    byte ptr [ebp - 3], 0
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_4075
        test   ah, 4
        mov    dword ptr [ebp - 1ch], eax
        je     SHORT loc_2_5958
        mov    eax, 13h
        jmp    NEAR PTR loc_2_5D71
loc_2_5958:
        push   2
        lea    eax, [ebp - 2ch]
        mov    dword ptr [ebp - 2ch], 0
        push   esi
        push   eax
        call   sub_2_7918
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 2ch]
        test   byte ptr [ebp - 1ah], 1
        mov    word ptr [ebp - 6], 4
        mov    dword ptr [ebp - 0ch], eax
        je     SHORT loc_2_5986
        mov    word ptr [ebp - 6], 7
loc_2_5986:
        push   0ah
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58D3
        cmp    ax, 605h
        je     SHORT loc_2_599F
        cmp    ax, 607h
        jne    SHORT loc_2_59A3
loc_2_599F:
        or     byte ptr [ebp - 6], 3
loc_2_59A3:
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_406B
        mov    byte ptr [ebp - 2], al
        movzx  eax, al
        test   eax, eax
        je     NEAR PTR loc_2_5C1B
        cmp    eax, 1
        jl     SHORT loc_2_59C6
        cmp    eax, 2
        jle    SHORT loc_2_59D0
loc_2_59C6:
        mov    eax, 13h
        jmp    NEAR PTR loc_2_5D71
loc_2_59D0:
        cmp    dword ptr [ebp - 2ch], 0
        jne    SHORT loc_2_59E0
        mov    eax, 13h
        jmp    NEAR PTR loc_2_5D71
loc_2_59E0:
        push   3eh
        mov    edx, dword ptr [ebp + 8]
        cmp    byte ptr [ebp - 2], 1
        mov    ecx, dword ptr [ebp - 1]
        jne    SHORT loc_2_59F7
        call   sub_2_58D3
        or     al, 4
        jmp    SHORT loc_2_5A00
loc_2_59F7:
        call   sub_2_58D3
        and    ax, 0fcffh
loc_2_5A00:
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   3eh
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_5903
        push   0
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_40BC
        test   eax, eax
        jne    SHORT loc_2_5A7E
        push   18h
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58C5
        push   19h
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 1]
        mov    byte ptr [ebp - 10h], al
        call   sub_2_58C5
        cmp    byte ptr [ebp - 10h], al
        jae    SHORT loc_2_5A7E
        push   19h
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58C5
        push   1ah
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 1]
        mov    byte ptr [ebp - 10h], al
        call   sub_2_58C5
        cmp    byte ptr [ebp - 10h], al
        ja     SHORT loc_2_5A7E
        db  068h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp + 8]
        push   19h
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58C5
        mov    edx, eax
        mov    ecx, esi
        call   sub_2_65F2
loc_2_5A7E:
        mov    ecx, esi
        xor    ebx, ebx
        call   sub_2_42A2
        mov    dword ptr [ebp - 18h], eax
        cmp    eax, ebx
        je     NEAR PTR loc_2_5C08
        mov    dword ptr [ebp - 30h], ebx
        cmp    byte ptr [eax], bl
        jbe    NEAR PTR loc_2_5C08
        lea    esi, [eax + 4]
loc_2_5AA0:
        push   ebx
        lea    eax, [ebp - 34h]
        push   eax
        lea    edx, [ebp - 0ch]
        push   ebx
        push   dword ptr [ebp - 0ch]
        push   edx
        call   sub_2_7908
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_2_5C74
        mov    ecx, dword ptr [ebp - 34h]
        and    ch, 7fh
        cmp    dword ptr [esi + 20h], ecx
        jne    NEAR PTR loc_2_5C7E
        mov    eax, dword ptr [ebp - 34h]
        mov    dword ptr [ebp - 10h], ebx
        and    eax, 8000h
        mov    dword ptr [ebp - 24h], ebx
        mov    dword ptr [ebp - 14h], 0ffffffffh
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [ebp - 28h], eax
        cmp    ecx, 1
        je     SHORT loc_2_5AFF
        cmp    ecx, 2
        je     SHORT loc_2_5B1F
        mov    eax, 13h
        jmp    NEAR PTR loc_2_5D71
loc_2_5AFF:
        cmp    dword ptr [ebp - 28h], ebx
        je     SHORT loc_2_5B45
        mov    ecx, dword ptr [ebp - 0ch]
        or     byte ptr [ebp - 6], 2
        mov    eax, dword ptr [ecx + 4]
        mov    edx, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ecx + 10h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 10h], edx
        mov    dword ptr [ebp - 24h], ecx
        jmp    SHORT loc_2_5B45
loc_2_5B1F:
        cmp    dword ptr [ebp - 28h], ebx
        je     SHORT loc_2_5B45
        mov    ecx, dword ptr [ebp - 0ch]
        or     byte ptr [ebp - 6], 1
        movzx  eax, word ptr [ecx + 4]
        movzx  edx, word ptr [ecx + 6]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 10h], edx
        movzx  eax, word ptr [ecx + 0ch]
        sub    eax, 10000h
        mov    dword ptr [ebp - 24h], eax
loc_2_5B45:
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 1]
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 14h]
        push   esi
        call   sub_2_4E43
        test   byte ptr [esi + 1ch], 4
        jne    NEAR PTR loc_2_5BF3
        push   ebx
        mov    dword ptr [esi], ebx
        push   dword ptr [esi + 10h]
        push   -1
        push   ebx
        mov    dword ptr [esi + 8], ebx
        call   sub_2_7910
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_2_5D71
        cmp    dword ptr [ebp - 28h], ebx
        jne    SHORT loc_2_5B9B
        push   1
        push   dword ptr [esi + 10h]
        push   -1
        push   ebx
        call   sub_2_7928
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_2_5BF3
        jmp    NEAR PTR loc_2_5D71
loc_2_5B9B:
        mov    eax, dword ptr [ebp - 10h]
        sub    eax, dword ptr [ebp - 14h]
        inc    eax
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ebp - 24h]
        not    eax
        inc    eax
        cmp    dword ptr [ebp - 14h], ebx
        mov    dword ptr [esi + 8], eax
        je     NEAR PTR loc_2_5C88
        push   1
        mov    eax, dword ptr [ebp - 14h]
        push   dword ptr [esi + 10h]
        dec    eax
        push   eax
        push   ebx
        call   sub_2_7928
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_2_5D71
        cmp    dword ptr [ebp - 10h], -1
        je     SHORT loc_2_5BF3
        push   1
        mov    eax, dword ptr [ebp - 10h]
        push   dword ptr [esi + 10h]
        push   -1
        inc    eax
        push   eax
        call   sub_2_7928
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_2_5D71
loc_2_5BF3:
        add    esi, 2ch
        mov    ecx, dword ptr [ebp - 18h]
        movzx  eax, byte ptr [ecx]
        inc    dword ptr [ebp - 30h]
        cmp    eax, dword ptr [ebp - 30h]
        ja     NEAR PTR loc_2_5AA0
loc_2_5C08:
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_5256
        test   eax, eax
        jne    SHORT loc_2_5C1B
        or     byte ptr [ebp - 6], 3
loc_2_5C1B:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_2_5D1F
        mov    esi, 0ffh
        xor    edi, edi
loc_2_5C2C:
        push   edi
        lea    eax, [ebp - 38h]
        push   eax
        lea    edx, [ebp - 0ch]
        push   edi
        push   dword ptr [ebp - 0ch]
        push   edx
        call   sub_2_7908
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_2_5D1F
        push   edi
        lea    ecx, [ebp - 20h]
        push   dword ptr [ebp - 0ch]
        push   ecx
        call   sub_2_7AC8
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 38h]
        cmp    eax, 1
        je     SHORT loc_2_5C92
        cmp    eax, 2
        je     SHORT loc_2_5CC6
        cmp    eax, 4
        je     NEAR PTR loc_2_5CF3
        jmp    NEAR PTR loc_2_5D16
loc_2_5C74:
        mov    eax, 13h
        jmp    NEAR PTR loc_2_5D71
loc_2_5C7E:
        mov    eax, 13h
        jmp    NEAR PTR loc_2_5D71
loc_2_5C88:
        mov    eax, 13h
        jmp    NEAR PTR loc_2_5D71
loc_2_5C92:
        cmp    dword ptr [ebp - 20h], 28h
        jne    NEAR PTR loc_2_5D7A
        mov    eax, dword ptr [ebp - 0ch]
        or     byte ptr [ebp - 6], 2
        mov    ecx, dword ptr [eax + 24h]
        cmp    ecx, esi
        je     SHORT loc_2_5D16
        cmp    ecx, 0fch
        je     SHORT loc_2_5D16
        mov    eax, dword ptr [eax + 4]
        mov    edx, dword ptr [ebp + 8]
        and    al, 0f0h
        push   eax
        push   ecx
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_5917
        jmp    SHORT loc_2_5D16
loc_2_5CC6:
        cmp    dword ptr [ebp - 20h], 1ch
        jne    NEAR PTR loc_2_5D81
        mov    eax, dword ptr [ebp - 0ch]
        or     byte ptr [ebp - 6], 1
        mov    ecx, dword ptr [eax + 18h]
        cmp    ecx, esi
        je     SHORT loc_2_5D16
        movzx  eax, word ptr [eax + 4]
        and    al, 0fch
        mov    edx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_5917
        jmp    SHORT loc_2_5D16
loc_2_5CF3:
        cmp    dword ptr [ebp - 20h], 8
        jne    NEAR PTR loc_2_5D88
        mov    eax, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 1]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [ebp - 3], al
        push   dword ptr [ebp - 3]
        push   3ch
        call   sub_2_58EF
loc_2_5D16:
        cmp    dword ptr [ebp - 0ch], edi
        jne    NEAR PTR loc_2_5C2C
loc_2_5D1F:
        test   byte ptr [ebp - 1bh], 40h
        jne    SHORT loc_2_5D36
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 1]
        push   dword ptr [ebp - 6]
        push   dword ptr [ebp - 3]
        call   sub_2_4EA5
loc_2_5D36:
        cmp    byte ptr [ebp - 2], 2
        jne    SHORT loc_2_5D6F
        push   3eh
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58D3
        mov    si, ax
        mov    edx, dword ptr [ebp + 8]
        and    si, 0ffbfh
        or     al, 40h
        push   eax
        mov    ecx, dword ptr [ebp - 1]
        push   3eh
        call   sub_2_5903
        push   esi
        mov    edx, dword ptr [ebp + 8]
        push   3eh
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_5903
loc_2_5D6F:
        xor    eax, eax
loc_2_5D71:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_5D7A:
        mov    eax, 13h
        jmp    SHORT loc_2_5D71
loc_2_5D81:
        mov    eax, 13h
        jmp    SHORT loc_2_5D71
loc_2_5D88:
        mov    eax, 13h
        jmp    SHORT loc_2_5D71
sub_2_5D8F:
        push   ebp
        mov    edx, dword ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    bl, byte ptr [ebp + 8]
        mov    ecx, ebx
        call   sub_2_406B
        movzx  esi, al
        test   esi, esi
        je     SHORT loc_2_5DBD
        cmp    esi, 1
        je     SHORT loc_2_5DC3
        cmp    esi, 2
        je     SHORT loc_2_5DC9
        mov    byte ptr [ebp - 1], 0
        jmp    SHORT loc_2_5DCD
loc_2_5DBD:
        mov    byte ptr [ebp - 1], 6
        jmp    SHORT loc_2_5DCD
loc_2_5DC3:
        mov    byte ptr [ebp - 1], 2
        jmp    SHORT loc_2_5DCD
loc_2_5DC9:
        mov    byte ptr [ebp - 1], 1
loc_2_5DCD:
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, ebx
        call   sub_2_4075
        test   eax, 14000h
        jne    SHORT loc_2_5DFC
        push   4
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, ebx
        call   sub_2_58D3
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, ebx
        movzx  eax, ax
        and    al, 0f8h
        push   eax
        push   4
        call   sub_2_5917
loc_2_5DFC:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_5E13
        push   0
        mov    edx, dword ptr [ebp + 0ch]
        push   3ch
        mov    ecx, ebx
        call   sub_2_58EF
loc_2_5E13:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_2_5E40
        mov    byte ptr [ebp - 8], 10h
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebp - 4], al
loc_2_5E23:
        push   0
        mov    edx, dword ptr [ebp + 0ch]
        push   dword ptr [ebp - 8]
        mov    ecx, ebx
        call   sub_2_5917
        movzx  eax, byte ptr [ebp - 8]
        add    al, 4
        dec    byte ptr [ebp - 4]
        mov    byte ptr [ebp - 8], al
        jne    SHORT loc_2_5E23
loc_2_5E40:
        cmp    esi, 1
        je     SHORT loc_2_5E53
        cmp    esi, 2
        je     NEAR PTR loc_2_5F02
        jmp    NEAR PTR loc_2_5FA0
loc_2_5E53:
        xor    esi, esi
        mov    edx, dword ptr [ebp + 0ch]
        push   esi
        mov    ecx, ebx
        push   2ch
        call   sub_2_5917
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        push   28h
        mov    ecx, ebx
        call   sub_2_5917
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        push   32h
        mov    ecx, ebx
        call   sub_2_5903
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        push   30h
        mov    ecx, ebx
        call   sub_2_5903
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        push   1dh
        mov    ecx, ebx
        call   sub_2_58EF
        push   0ffh
        mov    edx, dword ptr [ebp + 0ch]
        push   1ch
        mov    ecx, ebx
        mov    edi, 0ffffh
        call   sub_2_58EF
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        push   22h
        mov    ecx, ebx
        call   sub_2_5903
        push   edi
        mov    edx, dword ptr [ebp + 0ch]
        push   20h
        mov    ecx, ebx
        call   sub_2_5903
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        push   26h
        mov    ecx, ebx
        call   sub_2_5903
        push   edi
        mov    edx, dword ptr [ebp + 0ch]
        push   24h
        mov    ecx, ebx
        call   sub_2_5903
        push   3eh
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, ebx
        call   sub_2_58D3
        and    ax, 0fff3h
        mov    edx, dword ptr [ebp + 0ch]
        push   eax
        push   3eh
        mov    ecx, ebx
        call   sub_2_5903
        jmp    NEAR PTR loc_2_5FA0
loc_2_5F02:
        xor    esi, esi
        mov    edx, dword ptr [ebp + 0ch]
        push   esi
        mov    ecx, ebx
        push   32h
        call   sub_2_5903
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        push   2eh
        mov    ecx, ebx
        call   sub_2_5903
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        push   3ah
        mov    ecx, ebx
        call   sub_2_5903
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        push   36h
        mov    ecx, ebx
        call   sub_2_5903
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        push   30h
        mov    ecx, ebx
        call   sub_2_5903
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        push   2ch
        mov    ecx, ebx
        call   sub_2_5903
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        push   38h
        mov    ecx, ebx
        call   sub_2_5903
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        push   34h
        mov    ecx, ebx
        call   sub_2_5903
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        push   20h
        mov    ecx, ebx
        call   sub_2_5917
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        push   1ch
        mov    ecx, ebx
        call   sub_2_5917
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        push   28h
        mov    ecx, ebx
        call   sub_2_5917
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        push   24h
        mov    ecx, ebx
        call   sub_2_5917
loc_2_5FA0:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_5FA7:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    bl, dl
        mov    esi, ecx
        push   0
        push   100000h
        push   esi
        call   sub_2_7AD0
        mov    edx, dword ptr [esp + 20h]
        add    esp, 0ch
        mov    ecx, ebx
        push   0ah
        call   sub_2_58D3
        cmp    ax, 600h
        mov    di, ax
        je     SHORT loc_2_6024
        cmp    di, 601h
        je     SHORT loc_2_6024
        cmp    di, 602h
        je     SHORT loc_2_6024
        cmp    di, 603h
        je     SHORT loc_2_6024
        lea    eax, [esp + 0ch]
        push   2
        push   esi
        push   eax
        call   sub_2_7918
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_6007
        cmp    di, 604h
        jne    SHORT loc_2_6024
loc_2_6007:
        push   dword ptr [esp + 14h]
        push   ebx
        call   sub_2_5D8F
        add    esp, 8
        push   1
        push   100000h
        push   esi
        call   sub_2_7AD0
        add    esp, 0ch
loc_2_6024:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    4
sub_2_602D:
        sub    esp, 4
        xor    edx, edx
        xor    ecx, ecx
        push   ebx
        push   esi
        push   edi
        push   ebp
        xor    edi, edi
        push   0
        call   sub_2_58E1
        mov    ecx, 1010101h
        or     eax, ecx
        cmp    eax, ecx
        mov    esi, eax
        je     SHORT loc_2_6058
        cmp    esi, -1
        je     SHORT loc_2_6058
        mov    edi, 1
loc_2_6058:
        mov    bl, 8
        mov    ebp, 1010101h
        mov    byte ptr [esp + 13h], 1
loc_2_6064:
        push   0
        mov    edx, ebx
        xor    ecx, ecx
        call   sub_2_58E1
        or     eax, ebp
        cmp    eax, ebp
        je     SHORT loc_2_607F
        cmp    eax, -1
        je     SHORT loc_2_607F
        cmp    esi, eax
        jne    SHORT loc_2_6090
        inc    edi
loc_2_607F:
        movzx  ebx, bl
        add    bl, 8
        inc    byte ptr [esp + 13h]
        cmp    byte ptr [esp + 13h], 20h
        jb     SHORT loc_2_6064
loc_2_6090:
        cmp    byte ptr [esp + 13h], 20h
        jne    SHORT loc_2_60A4
        test   edi, edi
        je     SHORT loc_2_60A0
        cmp    edi, 20h
        jne    SHORT loc_2_60A4
loc_2_60A0:
        xor    eax, eax
        jmp    SHORT loc_2_60A9
loc_2_60A4:
        mov    eax, 1
loc_2_60A9:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_2_60B1:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, 1
        db  085h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_613B
        xor    edi, edi
        db  089h, 035h, 000h, 000h, 000h, 000h
        db  089h, 03Dh, 000h, 000h, 000h, 000h
        call   sub_2_602D
        test   eax, eax
        jne    SHORT loc_2_613B
        db  089h, 035h, 000h, 000h, 000h, 000h
        call   sub_2_602D
        test   eax, eax
        jne    SHORT loc_2_613B
        push   edi
        xor    edx, edx
        xor    ecx, ecx
        db  089h, 03Dh, 000h, 000h, 000h, 000h
        call   sub_2_58E1
        cmp    eax, 4838086h
        mov    esi, eax
        je     SHORT loc_2_610E
        cmp    esi, 4858086h
        jne    SHORT loc_2_613B
loc_2_610E:
        push   8
        xor    edx, edx
        xor    ecx, ecx
        call   sub_2_58C5
        cmp    esi, 4838086h
        jne    SHORT loc_2_6125
        cmp    al, 1
        jbe    SHORT loc_2_6131
loc_2_6125:
        cmp    esi, 4858086h
        jne    SHORT loc_2_613B
        test   al, al
        jne    SHORT loc_2_613B
loc_2_6131:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
loc_2_613B:
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 020h
        xor    bl, bl
        mov    byte ptr [ebp - 8], bl
loc_2_6147:
        push   0
        mov    edx, dword ptr [ebp - 8]
        xor    ecx, ecx
        call   sub_2_58E1
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_2_615E
        cmp    esi, -1
        jne    SHORT loc_2_616E
loc_2_615E:
        movzx  eax, byte ptr [ebp - 8]
        add    al, 8
        inc    bl
        mov    byte ptr [ebp - 8], al
        cmp    bl, 20h
        jb     SHORT loc_2_6147
loc_2_616E:
        cmp    bl, 20h
        je     NEAR PTR loc_2_6217
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 001h
        jne    SHORT loc_2_6199
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_6199
        lea    ecx, [ebx*8]
        call   sub_2_56FA
        test   eax, eax
        je     SHORT loc_2_6217
loc_2_6199:
        lea    eax, [ebx + 1]
        mov    byte ptr [ebp - 5], al
        cmp    al, 20h
        jae    NEAR PTR loc_2_6227
        shl    al, 3
        mov    byte ptr [ebp - 0ch], al
loc_2_61AD:
        push   0
        mov    edx, dword ptr [ebp - 0ch]
        xor    ecx, ecx
        call   sub_2_58E1
        cmp    eax, esi
        jne    SHORT loc_2_61F7
        mov    byte ptr [ebp - 1], 4
loc_2_61C1:
        mov    edx, dword ptr [ebp - 0ch]
        xor    ecx, ecx
        push   dword ptr [ebp - 1]
        call   sub_2_58E1
        mov    edi, eax
        xor    ecx, ecx
        push   dword ptr [ebp - 1]
        lea    eax, [ebx*8]
        mov    edx, eax
        call   sub_2_58E1
        cmp    edi, eax
        jne    SHORT loc_2_61F1
        add    byte ptr [ebp - 1], 4
        cmp    byte ptr [ebp - 1], 40h
        jb     SHORT loc_2_61C1
loc_2_61F1:
        cmp    byte ptr [ebp - 1], 40h
        je     SHORT loc_2_620B
loc_2_61F7:
        movzx  eax, byte ptr [ebp - 0ch]
        add    al, 8
        inc    byte ptr [ebp - 5]
        cmp    byte ptr [ebp - 5], 20h
        mov    byte ptr [ebp - 0ch], al
        jb     SHORT loc_2_61AD
        jmp    SHORT loc_2_6227
loc_2_620B:
        mov    al, byte ptr [ebp - 5]
        sub    al, bl
        db  0A2h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_6227
loc_2_6217:
        push   2
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_79B8
        add    esp, 8
loc_2_6227:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_622E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0
        push   20000h
        push   0e0000h
        call   sub_2_7A38
        add    esp, 0ch
        mov    edx, eax
        mov    dword ptr [ebp - 4], eax
        add    dword ptr [ebp - 4], 1fff0h
        cmp    dword ptr [ebp - 4], edx
        jb     SHORT loc_2_628E
loc_2_625C:
        mov    eax, dword ptr [ebp - 4]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax], ecx
        jne    SHORT loc_2_6280
        xor    ecx, ecx
        xor    esi, esi
loc_2_626D:
        mov    eax, dword ptr [ebp - 4]
        movzx  eax, byte ptr [eax + esi]
        add    ecx, eax
        inc    esi
        cmp    esi, 10h
        jb     SHORT loc_2_626D
        test   cl, cl
        je     SHORT loc_2_6289
loc_2_6280:
        sub    dword ptr [ebp - 4], 10h
        cmp    dword ptr [ebp - 4], edx
        jae    SHORT loc_2_625C
loc_2_6289:
        cmp    dword ptr [ebp - 4], edx
        jae    SHORT loc_2_6292
loc_2_628E:
        xor    eax, eax
        jmp    SHORT loc_2_62FE
loc_2_6292:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   2000h
        push   dword ptr [eax + 4]
        call   sub_2_7A38
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        mov    al, byte ptr [eax]
        cmp    al, 0ffh
        jne    SHORT loc_2_62B4
        xor    eax, eax
        jmp    SHORT loc_2_62FE
loc_2_62B4:
        test   al, al
        jne    SHORT loc_2_62BC
        xor    eax, eax
        jmp    SHORT loc_2_62FE
loc_2_62BC:
        pushad
        mov    eax, 49435024h
        xor    ebx, ebx
        db  089h, 025h, 000h, 000h, 000h, 000h
        push   cs
        call   dword ptr [ebp - 4]
        db  08Bh, 025h, 000h, 000h, 000h, 000h
        sub    al, 1
        sbb    ecx, ecx
        add    ebx, edx
        and    ebx, ecx
        mov    dword ptr [ebp - 4], ebx
        popad
        mov    eax, dword ptr [ebp - 4]
        cmp    eax, 0a0000h
        jae    SHORT loc_2_62EE
        xor    eax, eax
        jmp    SHORT loc_2_62FE
loc_2_62EE:
        push   0
        push   2000h
        push   eax
        call   sub_2_7A38
        add    esp, 0ch
loc_2_62FE:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_6305:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   edi
        call   sub_2_44FB
        db  0B9h, 000h, 000h, 000h, 000h
        call   sub_2_4521
        mov    edi, eax
        db  0A3h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_636E
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_636E
        call   sub_2_622E
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_2_635D
        xor    eax, eax
        lea    ecx, [ebp - 18h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 18h], 0b10ah
        call   sub_2_4745
        test   eax, eax
        jne    SHORT loc_2_636E
loc_2_635D:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  080h, 025h, 000h, 000h, 000h, 000h, 0FEh
loc_2_636E:
        call   sub_2_60B1
        call   sub_2_3D9D
        db  039h, 03Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_2_6390
        db  0B9h, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        call   sub_2_4564
loc_2_6390:
        call   sub_2_4592
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_639A:
        push   ebp
        mov    ebp, esp
        sub    esp, 114h
        push   ebx
        push   esi
        push   edi
        mov    bl, dl
        mov    esi, ecx
        push   0
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, ebx
        call   sub_2_58E1
        push   8
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, ebx
        mov    dword ptr [ebp - 8], eax
        call   sub_2_58C5
        push   0ah
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, ebx
        mov    byte ptr [ebp - 1], al
        call   sub_2_58D3
        mov    word ptr [ebp - 4], ax
        push   9
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, ebx
        call   sub_2_58C5
        movzx  eax, al
        movzx  ecx, word ptr [ebp - 4]
        shl    ecx, 8
        mov    edx, dword ptr [ebp + 8]
        add    eax, ecx
        mov    ecx, ebx
        mov    dword ptr [ebp - 10h], eax
        call   sub_2_406B
        movzx  eax, al
        test   eax, eax
        je     SHORT loc_2_6412
        cmp    eax, 1
        je     SHORT loc_2_6416
        cmp    eax, 2
        je     SHORT loc_2_641A
        xor    edi, edi
        jmp    SHORT loc_2_6428
loc_2_6412:
        push   2ch
        jmp    SHORT loc_2_641C
loc_2_6416:
        xor    edi, edi
        jmp    SHORT loc_2_6428
loc_2_641A:
        push   40h
loc_2_641C:
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, ebx
        call   sub_2_58E1
        mov    edi, eax
loc_2_6428:
        xor    ebx, ebx
        lea    eax, [ebp - 0ch]
        push   ebx
        lea    ecx, [ebp - 114h]
        mov    dword ptr [ebp - 0ch], 100h
        push   eax
        push   ecx
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        call   sub_2_79D8
        add    esp, 1ch
        test   eax, eax
        je     SHORT loc_2_646E
        push   ebx
        mov    ecx, dword ptr [ebp - 4]
        push   ebx
        call   sub_2_42EB
        push   eax
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        call   sub_2_79E0
        add    esp, 1ch
loc_2_646E:
        movzx  eax, byte ptr [ebp - 1]
        push   eax
        lea    ecx, [ebp - 14h]
        db  068h, 000h, 000h, 000h, 000h
        xor    ebx, ebx
        push   ecx
        call   sub_2_79F8
        add    esp, 0ch
        lea    ecx, [ebp - 14h]
        push   ebx
        push   ebx
        push   ecx
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        call   sub_2_79E0
        add    esp, 1ch
        lea    eax, [ebp - 114h]
        push   dword ptr [ebp - 10h]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_79F8
        add    esp, 0ch
        lea    ecx, [ebp - 114h]
        push   1
        push   ecx
        push   esi
        call   sub_2_7AD8
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffff80h
        cmp    eax, 10180h
        jne    SHORT loc_2_6525
        push   ebx
        lea    eax, [ebp - 0ch]
        push   eax
        lea    ecx, [ebp - 114h]
        mov    dword ptr [ebp - 0ch], 100h
        push   ecx
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        call   sub_2_79D8
        add    esp, 1ch
        test   eax, eax
        je     SHORT loc_2_6515
        push   ebx
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        call   sub_2_79E0
        add    esp, 1ch
loc_2_6515:
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_2_7AD8
        add    esp, 0ch
loc_2_6525:
        movzx  eax, word ptr [ebp - 4]
        push   eax
        lea    ecx, [ebp - 114h]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_2_79F8
        add    esp, 0ch
        lea    ecx, [ebp - 114h]
        push   1
        push   ecx
        push   esi
        call   sub_2_7AD8
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 8]
        movzx  ecx, byte ptr [ebp - 1]
        shr    eax, 10h
        push   ecx
        push   edi
        lea    ecx, [ebp - 114h]
        movzx  ebx, ax
        movzx  eax, word ptr [ebp - 8]
        push   ebx
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_2_79F8
        add    esp, 18h
        lea    ecx, [ebp - 114h]
        push   0
        push   ecx
        push   esi
        call   sub_2_7AD8
        add    esp, 0ch
        lea    ecx, [ebp - 114h]
        movzx  eax, word ptr [ebp - 8]
        push   edi
        push   ebx
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_2_79F8
        add    esp, 14h
        lea    ecx, [ebp - 114h]
        push   0
        push   ecx
        push   esi
        call   sub_2_7AD8
        movzx  ecx, byte ptr [ebp - 1]
        add    esp, 0ch
        movzx  eax, word ptr [ebp - 8]
        push   ecx
        push   ebx
        push   eax
        lea    ecx, [ebp - 114h]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_2_79F8
        add    esp, 14h
        lea    ecx, [ebp - 114h]
        push   0
        push   ecx
        push   esi
        call   sub_2_7AD8
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_65F2:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        mov    byte ptr [ebp - 2], dl
        push   esi
        push   edi
        mov    esi, ecx
        call   sub_2_44FB
        db  0A0h, 000h, 000h, 000h, 000h
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 4], al
        je     SHORT loc_2_6656
        xor    edi, edi
        lea    eax, [ebp - 3]
        push   edi
        lea    edx, [ebp - 1]
        push   edi
        mov    ecx, esi
        push   eax
        call   sub_2_4212
        push   edi
        mov    edx, dword ptr [ebp - 3]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58E1
        push   eax
        lea    ecx, [ebp - 10h]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_2_79F8
        add    esp, 0ch
        lea    ecx, [ebp - 10h]
        call   sub_2_4521
        test   ah, 20h
        je     SHORT loc_2_6656
        mov    byte ptr [ebp - 4], 1
loc_2_6656:
        mov    byte ptr [ebp - 1], 0
        cmp    byte ptr [ebp - 4], 0
        je     NEAR PTR loc_2_6786
loc_2_6664:
        xor    bl, bl
loc_2_6666:
        cmp    bl, 1
        jne    SHORT loc_2_6683
        push   0eh
        mov    dl, byte ptr [ebp - 1]
        shl    dl, 3
        mov    ecx, dword ptr [ebp - 2]
        call   sub_2_58C5
        test   al, 80h
        je     NEAR PTR loc_2_6777
loc_2_6683:
        mov    al, byte ptr [ebp - 1]
        push   0
        mov    ecx, dword ptr [ebp - 2]
        lea    eax, [ebx + eax*8]
        mov    byte ptr [ebp - 3], al
        mov    edx, dword ptr [ebp - 3]
        call   sub_2_58E1
        mov    ecx, eax
        mov    edi, eax
        call   sub_2_41FE
        test   eax, eax
        jne    SHORT loc_2_66B3
        test   bl, bl
        je     NEAR PTR loc_2_6777
        jmp    NEAR PTR loc_2_676C
loc_2_66B3:
        push   edi
        lea    eax, [ebp - 10h]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_79F8
        add    esp, 0ch
        lea    ecx, [ebp - 10h]
        call   sub_2_4521
        test   eax, 20h
        mov    edi, eax
        je     SHORT loc_2_66E0
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
loc_2_66E0:
        test   edi, 8
        je     SHORT loc_2_66F2
        test   bl, bl
        je     NEAR PTR loc_2_6777
        jmp    SHORT loc_2_676C
loc_2_66F2:
        cmp    byte ptr [ebp - 2], 0
        je     SHORT loc_2_673C
        test   edi, 800h
        je     SHORT loc_2_6706
        test   bl, bl
        je     SHORT loc_2_6777
        jmp    SHORT loc_2_676C
loc_2_6706:
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_6714
        mov    byte ptr [ebp - 5], 0
        jmp    SHORT loc_2_6724
loc_2_6714:
        push   0
        lea    edx, [ebp - 5]
        push   0
        mov    ecx, esi
        push   0
        call   sub_2_4212
loc_2_6724:
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 5]
        push   dword ptr [ebp - 3]
        call   sub_2_52FF
        test   eax, eax
        je     SHORT loc_2_673C
        test   bl, bl
        je     SHORT loc_2_6777
        jmp    SHORT loc_2_676C
loc_2_673C:
        test   edi, 1000h
        je     SHORT loc_2_6750
        cmp    byte ptr [ebp - 1], 10h
        jb     SHORT loc_2_6750
        test   bl, bl
        je     SHORT loc_2_6777
        jmp    SHORT loc_2_676C
loc_2_6750:
        test   byte ptr [ebp - 3], 7
        je     SHORT loc_2_675E
        test   edi, 40h
        jne    SHORT loc_2_6777
loc_2_675E:
        push   edi
        push   esi
        push   dword ptr [ebp - 3]
        push   dword ptr [ebp - 2]
        call   dword ptr [ebp + 8]
        add    esp, 10h
loc_2_676C:
        inc    bl
        cmp    bl, 7
        jbe    NEAR PTR loc_2_6666
loc_2_6777:
        inc    byte ptr [ebp - 1]
        mov    al, byte ptr [ebp - 1]
        cmp    al, byte ptr [ebp - 4]
        jb     NEAR PTR loc_2_6664
loc_2_6786:
        call   sub_2_4592
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_6794:
        push   ebx
        push   esi
        db  038h, 015h, 000h, 000h, 000h, 000h
        push   edi
        mov    bl, dl
        mov    esi, ecx
        jae    SHORT loc_2_67A9
        db  088h, 01Dh, 000h, 000h, 000h, 000h
loc_2_67A9:
        mov    edi, dword ptr [esp + 10h]
        test   edi, edi
        jne    SHORT loc_2_67BC
        push   0
        push   esi
        call   sub_2_7AE8
        add    esp, 8
loc_2_67BC:
        db  068h, 000h, 000h, 000h, 000h
        mov    edx, ebx
        mov    ecx, esi
        call   sub_2_65F2
        test   edi, edi
        jne    SHORT loc_2_67D9
        push   0
        push   esi
        call   sub_2_7AE0
        add    esp, 8
loc_2_67D9:
        pop    edi
        pop    esi
        pop    ebx
        ret    4
sub_2_67DF:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        xor    ebx, ebx
        push   ebx
        push   eax
        call   sub_2_7AE8
        add    esp, 8
        call   sub_2_58A3
        push   ebx
        lea    ecx, [ebp - 8]
        db  0A2h, 000h, 000h, 000h, 000h
        db  088h, 01Dh, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   ecx
        call   sub_2_7968
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_685A
loc_2_681E:
        push   ebx
        lea    eax, [ebp - 1]
        push   eax
        xor    edx, edx
        push   ebx
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_4212
        test   byte ptr [ebp - 1], 10h
        je     SHORT loc_2_6846
        push   1
        push   200h
        push   dword ptr [ebp - 8]
        call   sub_2_7AD0
        add    esp, 0ch
loc_2_6846:
        push   ebx
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_7960
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_681E
loc_2_685A:
        push   1
        db  0A0h, 000h, 000h, 000h, 000h
        mov    edx, eax
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_2_6794
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 0FFh
        je     SHORT loc_2_688A
        db  0A0h, 000h, 000h, 000h, 000h
        db  0FEh, 005h, 000h, 000h, 000h, 000h
        db  038h, 005h, 000h, 000h, 000h, 000h
        jbe    SHORT loc_2_685A
loc_2_688A:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_7AE0
        add    esp, 8
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_689F:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        mov    dword ptr [ebp - 8], ecx
        push   esi
        mov    ebx, edx
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        push   0
        lea    eax, [ebp - 2]
        push   0
        lea    edx, [ebp - 1]
        push   eax
        mov    ecx, dword ptr [ebp + 8]
        call   sub_2_4212
        mov    edi, dword ptr [ebp + 10h]
        lea    eax, [ebx + edi]
        cmp    eax, 100h
        jbe    SHORT loc_2_68DA
        mov    eax, 13h
        jmp    NEAR PTR loc_2_69EA
loc_2_68DA:
        test   edi, edi
        je     SHORT loc_2_690C
        test   bl, 1
        je     SHORT loc_2_690C
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_68F9
        push   ebx
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58C5
        mov    byte ptr [esi], al
        jmp    SHORT loc_2_6908
loc_2_68F9:
        mov    al, byte ptr [esi]
        mov    edx, dword ptr [ebp - 2]
        push   eax
        mov    ecx, dword ptr [ebp - 1]
        push   ebx
        call   sub_2_58EF
loc_2_6908:
        inc    esi
        inc    bl
        dec    edi
loc_2_690C:
        cmp    edi, 2
        jb     SHORT loc_2_6949
        mov    al, bl
        and    al, 3
        cmp    al, 2
        jne    SHORT loc_2_6949
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_6930
        push   ebx
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58D3
        mov    word ptr [esi], ax
        jmp    SHORT loc_2_6940
loc_2_6930:
        mov    ax, word ptr [esi]
        mov    edx, dword ptr [ebp - 2]
        push   eax
        mov    ecx, dword ptr [ebp - 1]
        push   ebx
        call   sub_2_5903
loc_2_6940:
        add    esi, 2
        add    bl, 2
        sub    edi, 2
loc_2_6949:
        cmp    edi, 4
        jb     SHORT loc_2_698A
        mov    eax, edi
        shr    eax, 2
        mov    dword ptr [ebp - 0ch], eax
        shl    eax, 2
        sub    edi, eax
loc_2_695B:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_6971
        push   ebx
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58E1
        mov    dword ptr [esi], eax
        jmp    SHORT loc_2_697F
loc_2_6971:
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        push   dword ptr [esi]
        push   ebx
        call   sub_2_5917
loc_2_697F:
        add    esi, 4
        add    bl, 4
        dec    dword ptr [ebp - 0ch]
        jne    SHORT loc_2_695B
loc_2_698A:
        cmp    edi, 2
        jb     SHORT loc_2_69BF
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_69A6
        push   ebx
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58D3
        mov    word ptr [esi], ax
        jmp    SHORT loc_2_69B6
loc_2_69A6:
        mov    ax, word ptr [esi]
        mov    edx, dword ptr [ebp - 2]
        push   eax
        mov    ecx, dword ptr [ebp - 1]
        push   ebx
        call   sub_2_5903
loc_2_69B6:
        add    esi, 2
        add    bl, 2
        sub    edi, 2
loc_2_69BF:
        test   edi, edi
        je     SHORT loc_2_69E8
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_69D9
        push   ebx
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58C5
        mov    byte ptr [esi], al
        jmp    SHORT loc_2_69E8
loc_2_69D9:
        mov    al, byte ptr [esi]
        mov    edx, dword ptr [ebp - 2]
        push   eax
        mov    ecx, dword ptr [ebp - 1]
        push   ebx
        call   sub_2_58EF
loc_2_69E8:
        xor    eax, eax
loc_2_69EA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_69F3:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        cmp    eax, 1
        jbe    SHORT loc_2_6A18
        cmp    eax, 2
        je     SHORT loc_2_6A2E
        cmp    eax, 3
        je     SHORT loc_2_6A4F
        cmp    eax, 4
        je     SHORT loc_2_6A83
        mov    eax, 20h
        jmp    SHORT loc_2_6A94
loc_2_6A18:
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        call   sub_2_689F
        jmp    SHORT loc_2_6A94
loc_2_6A2E:
        cmp    dword ptr [ebp + 18h], 8
        je     SHORT loc_2_6A3B
        mov    eax, 13h
        jmp    SHORT loc_2_6A94
loc_2_6A3B:
        mov    ecx, dword ptr [ebp + 14h]
        xor    eax, eax
        db  0C7h, 041h, 004h, 000h, 000h, 000h, 000h
        db  0C7h, 001h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_6A94
loc_2_6A4F:
        cmp    dword ptr [ebp + 18h], 4
        je     SHORT loc_2_6A5C
        mov    eax, 13h
        jmp    SHORT loc_2_6A94
loc_2_6A5C:
        push   0
        lea    eax, [ebp - 2]
        push   0
        lea    edx, [ebp - 1]
        push   eax
        mov    ecx, dword ptr [ebp + 10h]
        call   sub_2_4212
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_4FCA
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], eax
        xor    eax, eax
        jmp    SHORT loc_2_6A94
loc_2_6A83:
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        call   sub_2_504E
loc_2_6A94:
        mov    esp, ebp
        pop    ebp
        ret
sub_2_6A98:
        push   esi
        cmp    edx, 1
        mov    esi, ecx
        je     SHORT loc_2_6AAB
        cmp    edx, 2
        je     SHORT loc_2_6ADF
loc_2_6AA5:
        xor    eax, eax
loc_2_6AA7:
        pop    esi
        ret    4
loc_2_6AAB:
        push   0
        mov    edx, 1
        mov    ecx, dword ptr [esp + 0ch]
        call   sub_2_45BC
        test   eax, eax
        je     SHORT loc_2_6AA5
        movzx  ecx, word ptr [esi]
        add    esi, 10h
        test   ecx, ecx
        je     SHORT loc_2_6AA5
        mov    edx, dword ptr [esi + 0ch]
        mov    eax, dword ptr [eax + 18h]
loc_2_6ACF:
        cmp    eax, edx
        jae    SHORT loc_2_6AD8
        dec    ecx
        jne    SHORT loc_2_6ACF
        jmp    SHORT loc_2_6AA5
loc_2_6AD8:
        mov    eax, 1
        jmp    SHORT loc_2_6AA7
loc_2_6ADF:
        push   0
        mov    edx, 2
        mov    ecx, dword ptr [esp + 0ch]
        call   sub_2_45BC
        test   eax, eax
        je     SHORT loc_2_6AA5
        movzx  edx, word ptr [esi]
        add    esi, 0ch
        test   edx, edx
        je     SHORT loc_2_6AA5
        mov    ecx, dword ptr [eax + 18h]
        movzx  eax, word ptr [esi + 6]
loc_2_6B04:
        cmp    eax, ecx
        jbe    SHORT loc_2_6B0D
        dec    edx
        jne    SHORT loc_2_6B04
        jmp    SHORT loc_2_6AA5
loc_2_6B0D:
        mov    eax, 1
        jmp    SHORT loc_2_6AA7
sub_2_6B14:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        lea    eax, [ebp - 4]
        push   edi
        mov    esi, edx
        push   2
        push   ecx
        push   eax
        call   sub_2_7918
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_6B38
        xor    eax, eax
        jmp    NEAR PTR loc_2_6BFA
loc_2_6B38:
        push   0
        lea    ecx, [ebp - 4]
        push   0
        push   4
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_7908
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_2_6B58
        xor    eax, eax
        jmp    NEAR PTR loc_2_6BFA
loc_2_6B58:
        xor    eax, eax
        push   eax
        push   eax
        push   4
        push   dword ptr [ebp - 4]
        push   eax
        call   sub_2_7908
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_2_6B75
        xor    eax, eax
        jmp    NEAR PTR loc_2_6BFA
loc_2_6B75:
        mov    di, 1
        mov    eax, dword ptr [ebp - 4]
        push   1
        push   esi
        mov    cl, byte ptr [eax + 2]
        lea    eax, [ebp - 8]
        shl    di, cl
        push   eax
        call   sub_2_7918
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_6BF5
        xor    esi, esi
loc_2_6B97:
        mov    eax, dword ptr [ebp - 8]
        push   esi
        push   esi
        lea    ecx, [ebp - 4]
        push   4
        mov    dword ptr [ebp - 4], eax
        push   eax
        push   ecx
        call   sub_2_7908
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_2_6BE1
loc_2_6BB2:
        mov    eax, dword ptr [ebp - 4]
        test   byte ptr [eax], 2
        je     SHORT loc_2_6BC0
        and    word ptr [eax + 4], di
        jmp    SHORT loc_2_6BCA
loc_2_6BC0:
        mov    cx, di
        not    cx
        and    word ptr [eax + 4], cx
loc_2_6BCA:
        push   esi
        lea    ecx, [ebp - 4]
        push   esi
        push   4
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_7908
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_2_6BB2
loc_2_6BE1:
        push   esi
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_79B0
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_6B97
loc_2_6BF5:
        mov    eax, 1
loc_2_6BFA:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_6C00:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        lea    edx, [ebp - 1]
        push   edi
        mov    esi, ecx
        push   eax
        lea    ebx, [ebp - 3]
        push   edx
        push   ebx
        lea    edx, [ebp - 2]
        call   sub_2_4212
        test   byte ptr [ebp - 1], 20h
        jne    NEAR PTR loc_2_6CD0
        push   0
        lea    eax, [ebp - 18h]
        push   esi
        push   eax
        call   sub_2_7958
        add    esp, 0ch
        mov    edx, dword ptr [ebp - 18h]
        mov    ecx, esi
        call   sub_2_32C0
        test   eax, eax
        je     SHORT loc_2_6C5F
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, esi
        push   dword ptr [ebp - 4]
        or     byte ptr [ebp - 1], 20h
        push   dword ptr [ebp - 1]
        push   dword ptr [ebp - 3]
        call   sub_2_426A
        jmp    SHORT loc_2_6CD0
loc_2_6C5F:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_6C76
        mov    edx, esi
        mov    ecx, dword ptr [ebp - 18h]
        call   sub_2_6B14
        test   eax, eax
        jne    SHORT loc_2_6CD0
loc_2_6C76:
        push   1
        lea    eax, [ebp - 0ch]
        push   esi
        push   eax
        call   sub_2_7918
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_6CD0
loc_2_6C89:
        mov    edi, dword ptr [ebp - 0ch]
        push   0
        push   edi
        lea    ecx, [ebp - 0ch]
        push   ecx
        call   sub_2_79B0
        add    esp, 0ch
        mov    ebx, eax
        mov    dword ptr [ebp - 8], edi
loc_2_6CA0:
        push   0
        lea    ecx, [ebp - 8]
        push   0
        push   4
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_7908
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_2_6CCC
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax], 2
        je     SHORT loc_2_6CA0
        push   0
        push   edi
        call   sub_2_7AF0
        add    esp, 8
loc_2_6CCC:
        test   ebx, ebx
        je     SHORT loc_2_6C89
loc_2_6CD0:
        push   0
        lea    eax, [ebp - 10h]
        push   esi
        push   eax
        call   sub_2_7958
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_2_6D83
loc_2_6CE7:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    eax, dword ptr [ebp - 10h]
        je     NEAR PTR loc_2_6D83
        mov    ecx, dword ptr [ebp - 10h]
        call   sub_2_42A2
        mov    dword ptr [ebp - 14h], eax
        test   eax, eax
        je     SHORT loc_2_6D6A
        push   1
        lea    eax, [ebp - 0ch]
        push   esi
        push   eax
        call   sub_2_7918
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_6D6A
loc_2_6D17:
        mov    edi, dword ptr [ebp - 0ch]
        push   0
        push   edi
        lea    ecx, [ebp - 0ch]
        push   ecx
        call   sub_2_79B0
        add    esp, 0ch
        mov    ebx, eax
        mov    dword ptr [ebp - 8], edi
loc_2_6D2E:
        push   0
        lea    eax, [ebp - 1ch]
        push   eax
        lea    edx, [ebp - 8]
        push   0
        push   dword ptr [ebp - 8]
        push   edx
        call   sub_2_7908
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_2_6D66
        mov    edx, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp - 8]
        push   dword ptr [ebp - 14h]
        call   sub_2_6A98
        test   eax, eax
        je     SHORT loc_2_6D2E
        push   0
        push   edi
        call   sub_2_7AF0
        add    esp, 8
loc_2_6D66:
        test   ebx, ebx
        je     SHORT loc_2_6D17
loc_2_6D6A:
        push   0
        lea    ecx, [ebp - 10h]
        push   dword ptr [ebp - 10h]
        push   ecx
        call   sub_2_7958
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_2_6CE7
loc_2_6D83:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_6D8F:
        push   ebp
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 14h]
        cmp    ecx, esi
        jne    NEAR PTR loc_2_6E53
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 1
        je     SHORT loc_2_6DD7
        cmp    eax, 3
        je     SHORT loc_2_6DF1
        cmp    eax, 4
        je     SHORT loc_2_6E08
        cmp    eax, 5
        je     SHORT loc_2_6E35
        cmp    eax, 10h
        je     SHORT loc_2_6E3F
        cmp    eax, 13h
        je     NEAR PTR loc_2_6FFD
        mov    eax, 1
        jmp    NEAR PTR loc_2_6FFF
loc_2_6DD7:
        push   0
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_79D0
        add    esp, 0ch
        call   sub_2_6305
        jmp    NEAR PTR loc_2_6FFD
loc_2_6DF1:
        mov    eax, dword ptr [ebp + 0ch]
        dec    eax
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 13h
        jmp    NEAR PTR loc_2_6FFF
loc_2_6E08:
        xor    esi, esi
        cmp    dword ptr [ebp + 0ch], esi
        jne    NEAR PTR loc_2_6FFD
        call   sub_2_3D54
        db  039h, 035h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_2_6FFD
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_7B08
        jmp    NEAR PTR loc_2_6FFA
loc_2_6E35:
        call   sub_2_67DF
        jmp    NEAR PTR loc_2_6FFD
loc_2_6E3F:
        call   sub_2_6305
        push   1
        push   dword ptr [ebp + 10h]
        call   sub_2_7B00
        jmp    NEAR PTR loc_2_6FFA
loc_2_6E53:
        push   0
        lea    eax, [ebp - 1]
        push   eax
        lea    ecx, [ebp - 3]
        push   ecx
        lea    edx, [ebp - 2]
        mov    ecx, esi
        call   sub_2_4212
        mov    edi, dword ptr [ebp + 10h]
        cmp    esi, edi
        jne    NEAR PTR loc_2_6F17
        test   byte ptr [ebp - 1], 1
        je     NEAR PTR loc_2_6F17
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 3
        je     SHORT loc_2_6E98
        cmp    eax, 5
        je     SHORT loc_2_6EC9
        cmp    eax, 9
        je     SHORT loc_2_6EE6
        mov    eax, 1
        jmp    NEAR PTR loc_2_6FFF
loc_2_6E98:
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_2_6EA8
        mov    eax, 1
        jmp    NEAR PTR loc_2_6FFF
loc_2_6EA8:
        mov    ecx, edi
        call   sub_2_42A2
        test   eax, eax
        je     SHORT loc_2_6EBF
        mov    ecx, edi
        call   sub_2_77D9
        jmp    NEAR PTR loc_2_6FFF
loc_2_6EBF:
        mov    eax, 1
        jmp    NEAR PTR loc_2_6FFF
loc_2_6EC9:
        push   0
        mov    edx, dword ptr [ebp - 3]
        push   19h
        mov    ecx, dword ptr [ebp - 2]
        call   sub_2_58C5
        mov    edx, eax
        mov    ecx, edi
        call   sub_2_6794
        jmp    NEAR PTR loc_2_6FFD
loc_2_6EE6:
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_2_6EF6
        mov    eax, 1
        jmp    NEAR PTR loc_2_6FFF
loc_2_6EF6:
        mov    ecx, edi
        call   sub_2_42A2
        test   eax, eax
        je     SHORT loc_2_6F0D
        mov    ecx, edi
        call   sub_2_77FE
        jmp    NEAR PTR loc_2_6FFF
loc_2_6F0D:
        mov    eax, 1
        jmp    NEAR PTR loc_2_6FFF
loc_2_6F17:
        push   0
        lea    eax, [ebp - 0ch]
        push   esi
        push   eax
        call   sub_2_7958
        add    esp, 0ch
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 6]
        lea    edx, [ebp - 5]
        push   0
        push   eax
        push   ecx
        mov    ecx, dword ptr [ebp - 0ch]
        call   sub_2_4212
        db  039h, 03Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_6F7E
        cmp    dword ptr [ebp + 8], 0
        jne    SHORT loc_2_6F7E
        test   byte ptr [ebp - 1], 11h
        jne    SHORT loc_2_6F7E
        test   byte ptr [ebp - 4], 1
        je     SHORT loc_2_6F7E
        test   byte ptr [ebp - 4], 4
        je     SHORT loc_2_6F7E
        mov    edx, dword ptr [ebp - 6]
        mov    ecx, dword ptr [ebp - 5]
        call   sub_2_406B
        cmp    al, 2
        jne    SHORT loc_2_6F7E
        mov    ecx, esi
        call   sub_2_6C00
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 13h
        jmp    NEAR PTR loc_2_6FFF
loc_2_6F7E:
        cmp    dword ptr [ebp - 0ch], edi
        je     SHORT loc_2_6F8A
        mov    eax, 1
        jmp    SHORT loc_2_6FFF
loc_2_6F8A:
        test   byte ptr [ebp - 1], 1
        jne    SHORT loc_2_6F97
        mov    eax, 1
        jmp    SHORT loc_2_6FFF
loc_2_6F97:
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 1
        je     SHORT loc_2_6FBA
        cmp    eax, 2
        je     SHORT loc_2_6FC9
        cmp    eax, 4
        je     SHORT loc_2_6FC9
        cmp    eax, 6
        je     SHORT loc_2_6FD8
        cmp    eax, 0fh
        je     SHORT loc_2_6FE7
        mov    eax, 1
        jmp    SHORT loc_2_6FFF
loc_2_6FBA:
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, esi
        push   dword ptr [ebp - 3]
        call   sub_2_5929
        jmp    SHORT loc_2_6FFF
loc_2_6FC9:
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, esi
        push   dword ptr [ebp - 3]
        call   sub_2_5FA7
        jmp    SHORT loc_2_6FFD
loc_2_6FD8:
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, esi
        push   dword ptr [ebp - 3]
        call   sub_2_639A
        jmp    SHORT loc_2_6FFD
loc_2_6FE7:
        test   byte ptr [ebp - 1], 4
        mov    eax, 0
        jne    SHORT loc_2_6FFF
        push   0
        push   esi
        call   sub_2_7AF8
loc_2_6FFA:
        add    esp, 8
loc_2_6FFD:
        xor    eax, eax
loc_2_6FFF:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_7005:
        push   esi
        push   edi
        mov    esi, edx
        mov    edi, ecx
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        call   sub_2_7B10
        add    esp, 0ch
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        call   sub_2_7A10
        add    esp, 0ch
        cmp    esi, 1
        sbb    eax, eax
        and    eax, 4
        or     al, 3
        push   eax
        push   0
        push   0
        push   edi
        call   sub_2_7A08
        add    esp, 10h
        pop    edi
        pop    esi
        ret
loc_2_7046:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_7056
        mov    eax, 13h
        jmp    SHORT loc_2_70C1
loc_2_7056:
        cmp    dword ptr [esp + 8], 1
        mov    eax, 13h
        jne    SHORT loc_2_70C1
        mov    eax, dword ptr [esp + 4]
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   1
        push   3
        push   3
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_7B18
        add    esp, 24h
        xor    eax, eax
        push   eax
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   43h
        push   eax
        call   sub_2_79C0
        mov    ecx, dword ptr [esp + 1ch]
        add    esp, 18h
        mov    edx, 1
        call   sub_2_7005
        xor    eax, eax
loc_2_70C1:
        ret    8
sub_2_70C4:
        push   ebp
        mov    ebp, esp
        push   dword ptr [esp + 8]
        call   sub_2_45BC
        test   eax, eax
        je     SHORT loc_2_70F6
        test   byte ptr [eax + 1ch], 2
        mov    ecx, dword ptr [ebp + 0ch]
        je     SHORT loc_2_70E8
        cmp    ecx, 100h
        ja     SHORT loc_2_70E8
        shl    ecx, 2
loc_2_70E8:
        add    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [ebp + 10h]
        cmp    dword ptr [eax + 0ch], ecx
        jae    SHORT loc_2_70F6
        mov    dword ptr [eax + 0ch], ecx
loc_2_70F6:
        pop    ebp
        ret    0ch
loc_2_70FA:
        push   ebp
        mov    ecx, dword ptr [esp + 10h]
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        call   sub_2_42A2
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0ch], eax
        call   sub_2_406B
        movzx  eax, al
        test   eax, eax
        je     SHORT loc_2_7130
        cmp    eax, 1
        je     SHORT loc_2_7134
        cmp    eax, 2
        je     SHORT loc_2_7138
        jmp    NEAR PTR loc_2_72DD
loc_2_7130:
        mov    bl, 6
        jmp    SHORT loc_2_713A
loc_2_7134:
        mov    bl, 2
        jmp    SHORT loc_2_713A
loc_2_7138:
        mov    bl, 1
loc_2_713A:
        push   1
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        call   sub_2_40BC
        mov    esi, eax
        movzx  eax, bl
        mov    byte ptr [ebp - 1], 10h
        lea    edi, [eax*4 + 10h]
        cmp    edi, 10h
        jle    NEAR PTR loc_2_7228
        mov    ebx, 1
loc_2_7165:
        push   esi
        lea    edx, [ebp - 8]
        push   dword ptr [ebp - 1]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        lea    ecx, [ebp - 14h]
        call   sub_2_4F1B
        test   eax, eax
        je     NEAR PTR loc_2_720B
        test   dword ptr [ebp - 8], ebx
        je     SHORT loc_2_71B3
        mov    eax, dword ptr [ebp - 8]
        not    eax
        or     al, 3
        mov    dword ptr [ebp - 8], eax
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        dec    eax
        test   dword ptr [ebp - 8], eax
        je     SHORT loc_2_71A4
        mov    dword ptr [ebp - 8], 4
loc_2_71A4:
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 8]
        push   0
        mov    edx, 2
        jmp    SHORT loc_2_7203
loc_2_71B3:
        test   byte ptr [ebp - 8], 8
        je     SHORT loc_2_71C4
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 040h
        mov    ecx, ebx
        je     SHORT loc_2_71C6
loc_2_71C4:
        xor    ecx, ecx
loc_2_71C6:
        mov    eax, dword ptr [ebp - 8]
        and    al, 7
        cmp    al, 2
        jne    SHORT loc_2_71D6
        or     dword ptr [ebp - 8], 0fff00000h
loc_2_71D6:
        and    byte ptr [ebp - 8], 0f0h
        mov    eax, dword ptr [ebp - 8]
        not    eax
        mov    dword ptr [ebp - 8], eax
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        dec    eax
        test   dword ptr [ebp - 8], eax
        je     SHORT loc_2_71F5
        mov    dword ptr [ebp - 8], 10h
loc_2_71F5:
        mov    edx, ebx
        cmp    ecx, ebx
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 8]
        sbb    eax, eax
        inc    eax
        push   eax
loc_2_7203:
        mov    ecx, dword ptr [ebp - 0ch]
        call   sub_2_70C4
loc_2_720B:
        mov    eax, dword ptr [ebp - 14h]
        and    al, 7
        cmp    al, 4
        jne    SHORT loc_2_7218
        add    byte ptr [ebp - 1], 4
loc_2_7218:
        add    byte ptr [ebp - 1], 4
        movzx  eax, byte ptr [ebp - 1]
        cmp    eax, edi
        jl     NEAR PTR loc_2_7165
loc_2_7228:
        test   esi, esi
        jne    SHORT loc_2_725F
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        call   sub_2_4FCA
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_2_725F
        push   0
        mov    edx, 1
        mov    ecx, dword ptr [ebp - 0ch]
        call   sub_2_45BC
        test   eax, eax
        je     NEAR PTR loc_2_72DD
        add    dword ptr [eax + 4], esi
        cmp    dword ptr [eax + 0ch], esi
        jae    SHORT loc_2_725F
        mov    dword ptr [eax + 0ch], esi
loc_2_725F:
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        call   sub_2_406B
        movzx  eax, al
        cmp    eax, 1
        jl     SHORT loc_2_72DD
        cmp    eax, 2
        jg     SHORT loc_2_72DD
        push   0
        mov    edx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        call   sub_2_433A
        push   eax
        lea    ecx, [ebp - 10h]
        push   ecx
        call   sub_2_7B20
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_72DD
        mov    ecx, dword ptr [ebp - 10h]
        call   sub_2_42A2
        test   eax, eax
        je     SHORT loc_2_72DD
        mov    cl, byte ptr [eax]
        test   cl, cl
        je     SHORT loc_2_72DD
        movzx  edi, cl
        lea    esi, [eax + 4]
loc_2_72B0:
        mov    eax, dword ptr [esi + 1ch]
        mov    edx, dword ptr [esi + 20h]
        and    eax, 1
        mov    ecx, dword ptr [ebp - 0ch]
        push   eax
        call   sub_2_45BC
        test   eax, eax
        je     SHORT loc_2_72D7
        mov    ecx, dword ptr [esi + 4]
        add    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [esi + 0ch]
        cmp    dword ptr [eax + 0ch], ecx
        jae    SHORT loc_2_72D7
        mov    dword ptr [eax + 0ch], ecx
loc_2_72D7:
        add    esi, 2ch
        dec    edi
        jne    SHORT loc_2_72B0
loc_2_72DD:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_72E4:
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [esp + 14h]
        cmp    edx, 1
        mov    dword ptr [edi], 0ffffffffh
        je     SHORT loc_2_72FF
        cmp    edx, 2
        je     SHORT loc_2_731F
        jmp    SHORT loc_2_7345
loc_2_72FF:
        movzx  ebx, word ptr [ecx]
        add    ecx, 10h
        test   ebx, ebx
        je     SHORT loc_2_7345
        mov    edx, dword ptr [ecx]
        mov    eax, dword ptr [ecx + 4]
loc_2_730E:
        cmp    dword ptr [edi], edx
        jbe    SHORT loc_2_7314
        mov    dword ptr [edi], edx
loc_2_7314:
        cmp    esi, eax
        jae    SHORT loc_2_731A
        mov    esi, eax
loc_2_731A:
        dec    ebx
        jne    SHORT loc_2_730E
        jmp    SHORT loc_2_7345
loc_2_731F:
        movzx  ebx, word ptr [ecx]
        add    ecx, 0ch
        test   ebx, ebx
        je     SHORT loc_2_7345
        movzx  edx, word ptr [ecx]
        movzx  eax, word ptr [ecx + 2]
        or     edx, 0ffff0000h
loc_2_7336:
        cmp    dword ptr [edi], edx
        jbe    SHORT loc_2_733C
        mov    dword ptr [edi], edx
loc_2_733C:
        cmp    esi, eax
        jae    SHORT loc_2_7342
        mov    esi, eax
loc_2_7342:
        dec    ebx
        jne    SHORT loc_2_7336
loc_2_7345:
        mov    eax, dword ptr [esp + 10h]
        pop    edi
        add    dword ptr [eax], esi
        pop    esi
        pop    ebx
        ret    8
sub_2_7351:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        mov    dword ptr [ebp - 14h], ecx
        push   esi
        lea    eax, [ebp - 0ch]
        push   edi
        xor    esi, esi
        xor    edi, edi
        push   1
        push   edx
        push   eax
        call   sub_2_7918
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_73EB
        xor    ebx, ebx
loc_2_7376:
        mov    eax, dword ptr [ebp - 0ch]
        push   ebx
        push   ebx
        lea    edx, [ebp - 4]
        mov    dword ptr [ebp - 10h], ebx
        mov    dword ptr [ebp - 4], eax
        push   dword ptr [ebp + 8]
        push   eax
        push   edx
        call   sub_2_7908
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_2_73CF
loc_2_7395:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 10h]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_72E4
        mov    eax, dword ptr [ebp - 8]
        not    eax
        inc    eax
        mov    dword ptr [ebp - 8], eax
        cmp    edi, eax
        jae    SHORT loc_2_73B7
        mov    edi, eax
loc_2_73B7:
        push   ebx
        lea    edx, [ebp - 4]
        push   ebx
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp - 4]
        push   edx
        call   sub_2_7908
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_2_7395
loc_2_73CF:
        cmp    esi, dword ptr [ebp - 10h]
        jae    SHORT loc_2_73D7
        mov    esi, dword ptr [ebp - 10h]
loc_2_73D7:
        push   ebx
        lea    ecx, [ebp - 0ch]
        push   dword ptr [ebp - 0ch]
        push   ecx
        call   sub_2_79B0
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_7376
loc_2_73EB:
        push   edi
        mov    edx, dword ptr [ebp + 8]
        push   esi
        mov    ecx, dword ptr [ebp - 14h]
        push   0
        call   sub_2_70C4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_7403:
        push   ebp
        mov    ebp, esp
        sub    esp, 90h
        push   ebx
        mov    dword ptr [ebp - 0ch], ecx
        push   esi
        xor    ebx, ebx
        push   edi
        call   sub_2_42A2
        mov    esi, eax
        push   ebx
        push   ebx
        lea    eax, [ebp - 2]
        push   eax
        lea    edx, [ebp - 1]
        mov    ecx, dword ptr [ebp - 0ch]
        call   sub_2_4212
        cmp    byte ptr [esi], bl
        jbe    SHORT loc_2_7445
        lea    ecx, [esi + 8]
        xor    eax, eax
loc_2_7435:
        inc    ebx
        mov    dword ptr [ecx], eax
        mov    dword ptr [ecx + 8], eax
        add    ecx, 2ch
        movzx  edx, byte ptr [esi]
        cmp    edx, ebx
        ja     SHORT loc_2_7435
loc_2_7445:
        push   19h
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58C5
        test   al, al
        je     SHORT loc_2_746A
        cmp    byte ptr [esi], 0
        je     SHORT loc_2_746A
        db  068h, 000h, 000h, 000h, 000h
        mov    edx, eax
        mov    ecx, dword ptr [ebp - 0ch]
        call   sub_2_65F2
loc_2_746A:
        push   0
        lea    ecx, [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        push   ecx
        call   sub_2_7968
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_74C5
        xor    edi, edi
loc_2_7481:
        push   edi
        lea    eax, [ebp - 44h]
        push   edi
        push   edi
        push   eax
        push   dword ptr [ebp - 10h]
        call   sub_2_7940
        add    esp, 14h
        cmp    dword ptr [ebp - 44h], 4
        je     SHORT loc_2_74B1
        push   2
        mov    edx, dword ptr [ebp - 10h]
        mov    ecx, esi
        call   sub_2_7351
        push   1
        mov    edx, dword ptr [ebp - 10h]
        mov    ecx, esi
        call   sub_2_7351
loc_2_74B1:
        push   edi
        lea    ecx, [ebp - 10h]
        push   dword ptr [ebp - 10h]
        push   ecx
        call   sub_2_7960
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_7481
loc_2_74C5:
        xor    ecx, ecx
        mov    al, byte ptr [esi]
        test   al, al
        je     SHORT loc_2_7517
        movzx  eax, al
        lea    edx, [esi + 4]
        mov    dword ptr [ebp - 14h], eax
loc_2_74D6:
        mov    edi, dword ptr [edx + 18h]
        mov    eax, dword ptr [edx + 4]
        add    eax, edi
        mov    ebx, edi
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [edx + 4], eax
        not    ebx
        mov    eax, ebx
        and    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [edx + 0ch]
        add    eax, edi
        and    ebx, eax
        mov    dword ptr [edx + 0ch], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [edx + 0ch], ebx
        cmp    dword ptr [edx], eax
        jne    SHORT loc_2_750C
        cmp    dword ptr [edx + 8], ebx
        je     SHORT loc_2_750F
loc_2_750C:
        or     cl, 1
loc_2_750F:
        add    edx, 2ch
        dec    dword ptr [ebp - 14h]
        jne    SHORT loc_2_74D6
loc_2_7517:
        test   ecx, ecx
        jne    SHORT loc_2_7522
        xor    eax, eax
        jmp    NEAR PTR loc_2_7780
loc_2_7522:
        push   0
        lea    ecx, [ebp - 20h]
        push   dword ptr [ebp - 0ch]
        push   ecx
        call   sub_2_7918
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_753E
        xor    eax, eax
        jmp    NEAR PTR loc_2_7780
loc_2_753E:
        push   0
        lea    ecx, [ebp - 1ch]
        push   dword ptr [ebp - 20h]
        push   ecx
        call   sub_2_79B0
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_755A
        xor    eax, eax
        jmp    NEAR PTR loc_2_7780
loc_2_755A:
        lea    eax, [ebp - 90h]
        lea    edi, [ebp - 20h]
        mov    dword ptr [ebp - 8], eax
loc_2_7566:
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebp - 18h], eax
        cmp    byte ptr [esi], 0
        jbe    SHORT loc_2_75C4
        mov    eax, dword ptr [ebp - 8]
        lea    ebx, [ebp - 78h]
        mov    dword ptr [ebp - 14h], eax
loc_2_7580:
        mov    eax, edi
        lea    ecx, [ebp - 20h]
        sub    eax, ecx
        push   0
        lea    edx, [ebp - 18h]
        cmp    eax, 1
        sbb    eax, eax
        and    eax, ebx
        push   eax
        push   0
        push   dword ptr [ebp - 18h]
        push   edx
        call   sub_2_7908
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_2_7656
        mov    ecx, dword ptr [ebp - 18h]
        mov    eax, dword ptr [ebp - 14h]
        add    ebx, 4
        inc    dword ptr [ebp - 0ch]
        mov    dword ptr [eax], ecx
        add    dword ptr [ebp - 14h], 8
        movzx  ecx, byte ptr [esi]
        cmp    ecx, dword ptr [ebp - 0ch]
        ja     SHORT loc_2_7580
loc_2_75C4:
        add    edi, 4
        lea    eax, [ebp - 18h]
        add    dword ptr [ebp - 8], 4
        cmp    edi, eax
        jb     SHORT loc_2_7566
        mov    dword ptr [ebp - 0ch], 0
        cmp    byte ptr [esi], 0
        jbe    SHORT loc_2_7627
        lea    ebx, [ebp - 90h]
        lea    edi, [ebp - 78h]
loc_2_75E7:
        push   0
        lea    ecx, [ebp - 40h]
        push   dword ptr [ebx]
        push   ecx
        call   sub_2_7AC8
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_765D
        lea    ecx, [ebx + 4]
        push   0
        push   dword ptr [ebp - 40h]
        push   dword ptr [ebx]
        push   dword ptr [edi]
        push   dword ptr [ecx]
        push   ecx
        call   sub_2_7B28
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_2_7664
        add    ebx, 8
        add    edi, 4
        movzx  eax, byte ptr [esi]
        inc    dword ptr [ebp - 0ch]
        cmp    eax, dword ptr [ebp - 0ch]
        ja     SHORT loc_2_75E7
loc_2_7627:
        xor    ebx, ebx
        mov    dword ptr [ebp - 0ch], ebx
        cmp    byte ptr [esi], bl
        jbe    NEAR PTR loc_2_777B
        lea    eax, [ebp - 90h]
        lea    edi, [esi + 4]
        mov    dword ptr [ebp - 8], eax
loc_2_7640:
        mov    eax, dword ptr [edi + 20h]
        cmp    eax, 1
        je     SHORT loc_2_766B
        cmp    eax, 2
        je     NEAR PTR loc_2_76F0
        jmp    NEAR PTR loc_2_7765
loc_2_7656:
        xor    eax, eax
        jmp    NEAR PTR loc_2_7780
loc_2_765D:
        xor    eax, eax
        jmp    NEAR PTR loc_2_7780
loc_2_7664:
        xor    eax, eax
        jmp    NEAR PTR loc_2_7780
loc_2_766B:
        push   ebx
        lea    edx, [ebp - 6ch]
        push   28h
        mov    dword ptr [ebp - 68h], ebx
        push   edx
        mov    dword ptr [ebp - 64h], ebx
        mov    word ptr [ebp - 6ch], 1
        mov    word ptr [ebp - 6ah], 14h
        mov    eax, dword ptr [edi + 1ch]
        mov    word ptr [ebp - 5eh], bx
        and    eax, 1
        shl    eax, 2
        or     al, 1
        mov    word ptr [ebp - 60h], ax
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        not    eax
        mov    dword ptr [ebp - 5ch], eax
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [ebp - 58h], ecx
        mov    dword ptr [ebp - 54h], ebx
        mov    dword ptr [ebp - 50h], 0ffffffffh
        mov    eax, dword ptr [edi + 1ch]
        mov    word ptr [ebp - 4ah], bx
        and    eax, 1
        shl    eax, 2
        or     al, 1
        mov    word ptr [ebp - 4ch], ax
        movzx  ecx, byte ptr [esi + 1]
        mov    dword ptr [ebp - 48h], ecx
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [edi + 4], 1
        sbb    eax, eax
        and    eax, 8000h
        inc    eax
        push   eax
        push   dword ptr [ecx]
        push   ecx
        call   sub_2_7B28
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_2_7765
        xor    eax, eax
        jmp    NEAR PTR loc_2_7780
loc_2_76F0:
        mov    word ptr [ebp - 38h], bx
        push   ebx
        mov    word ptr [ebp - 36h], bx
        push   1ch
        mov    word ptr [ebp - 34h], bx
        mov    byte ptr [ebp - 32h], bl
        mov    word ptr [ebp - 3ch], 1
        mov    word ptr [ebp - 3ah], 0ch
        mov    byte ptr [ebp - 31h], bl
        mov    eax, dword ptr [edi + 0ch]
        dec    ax
        not    ax
        mov    word ptr [ebp - 30h], ax
        lea    eax, [ebp - 3ch]
        mov    ecx, dword ptr [edi + 4]
        push   eax
        mov    word ptr [ebp - 2eh], cx
        mov    byte ptr [ebp - 25h], bl
        mov    word ptr [ebp - 2ch], bx
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ebp - 26h], 0ffh
        mov    word ptr [ebp - 2ah], 0ffffh
        mov    word ptr [ebp - 28h], bx
        movzx  edx, byte ptr [esi + 1]
        mov    dword ptr [ebp - 24h], edx
        cmp    dword ptr [edi + 4], 1
        sbb    eax, eax
        and    eax, 8000h
        add    eax, 2
        push   eax
        push   dword ptr [ecx]
        push   ecx
        call   sub_2_7B28
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_2_7787
loc_2_7765:
        add    dword ptr [ebp - 8], 8
        movzx  eax, byte ptr [esi]
        add    edi, 2ch
        inc    dword ptr [ebp - 0ch]
        cmp    eax, dword ptr [ebp - 0ch]
        ja     NEAR PTR loc_2_7640
loc_2_777B:
        mov    eax, 1
loc_2_7780:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_7787:
        xor    eax, eax
        jmp    SHORT loc_2_7780
sub_2_778B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   1
        mov    esi, ecx
        push   200000h
        push   esi
        call   sub_2_7AD0
        add    esp, 0ch
        lea    eax, [ebp - 4]
        push   0
        push   esi
        push   eax
        call   sub_2_7968
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_77D4
loc_2_77B7:
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_778B
        push   0
        lea    ecx, [ebp - 4]
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_7960
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_77B7
loc_2_77D4:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_77D9:
        push   esi
        mov    esi, ecx
        call   sub_2_7403
        test   eax, eax
        mov    eax, 0
        je     SHORT loc_2_77FC
        mov    ecx, esi
        call   sub_2_778B
        push   0
        push   esi
        call   sub_2_7B30
        add    esp, 8
loc_2_77FC:
        pop    esi
        ret
sub_2_77FE:
        push   ebp
        mov    ebp, esp
        sub    esp, 40h
        push   ebx
        push   esi
        push   edi
        mov    edi, ecx
        call   sub_2_42A2
        mov    esi, eax
        push   0
        push   edi
        lea    eax, [ebp - 18h]
        push   eax
        call   sub_2_7918
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_2_78F5
        push   0
        lea    ecx, [ebp - 14h]
        push   dword ptr [ebp - 18h]
        push   ecx
        call   sub_2_79B0
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_2_78F5
        lea    eax, [ebp - 40h]
        lea    edi, [ebp - 18h]
        mov    dword ptr [ebp - 0ch], eax
loc_2_7849:
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebp - 8], eax
        cmp    byte ptr [esi], 0
        jbe    SHORT loc_2_7895
        mov    eax, dword ptr [ebp - 0ch]
        lea    ebx, [ebp - 28h]
        mov    dword ptr [ebp - 10h], eax
loc_2_7863:
        push   0
        lea    ecx, [ebp - 8]
        push   ebx
        push   0
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_7908
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_2_78F5
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 10h]
        add    ebx, 4
        inc    dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        add    dword ptr [ebp - 10h], 8
        movzx  ecx, byte ptr [esi]
        cmp    ecx, dword ptr [ebp - 4]
        ja     SHORT loc_2_7863
loc_2_7895:
        add    edi, 4
        lea    eax, [ebp - 10h]
        add    dword ptr [ebp - 0ch], 4
        cmp    edi, eax
        jb     SHORT loc_2_7849
        mov    dword ptr [ebp - 4], 0
        cmp    byte ptr [esi], 0
        jbe    SHORT loc_2_78F5
        lea    ebx, [ebp - 3ch]
        lea    edi, [ebp - 28h]
loc_2_78B5:
        push   0
        lea    ecx, [ebp - 1ch]
        push   dword ptr [ebx]
        push   ecx
        call   sub_2_7AC8
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_2_78F5
        lea    ecx, [ebx - 4]
        push   0
        push   dword ptr [ebp - 1ch]
        push   dword ptr [ebx]
        push   dword ptr [edi]
        push   dword ptr [ecx]
        push   ecx
        call   sub_2_7B28
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_2_78F5
        add    ebx, 8
        add    edi, 4
        movzx  eax, byte ptr [esi]
        inc    dword ptr [ebp - 4]
        cmp    eax, dword ptr [ebp - 4]
        ja     SHORT loc_2_78B5
loc_2_78F5:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  0CCh, 0CCh
sub_2_7900:
        db  0CDh, 020h, 029h, 080h, 033h, 000h
        int3
        int3
sub_2_7908:
        db  0CDh, 020h, 01Fh, 080h, 033h, 000h
        int3
        int3
sub_2_7910:
        db  0CDh, 020h, 026h, 080h, 033h, 000h
        int3
        int3
sub_2_7918:
        db  0CDh, 020h, 01Ah, 080h, 033h, 000h
        int3
        int3
sub_2_7920:
        db  0CDh, 020h, 028h, 080h, 033h, 000h
        int3
        int3
sub_2_7928:
        db  0CDh, 020h, 025h, 080h, 033h, 000h
        int3
        int3
sub_2_7930:
        db  0CDh, 020h, 007h, 080h, 033h, 000h
        int3
        int3
sub_2_7938:
        db  0CDh, 020h, 056h, 080h, 033h, 000h
        int3
        int3
sub_2_7940:
        db  0CDh, 020h, 044h, 080h, 033h, 000h
        int3
        int3
sub_2_7948:
        db  0CDh, 020h, 06Ch, 080h, 033h, 000h
        int3
        int3
sub_2_7950:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        int3
        int3
sub_2_7958:
        db  0CDh, 020h, 003h, 080h, 033h, 000h
        int3
        int3
sub_2_7960:
        db  0CDh, 020h, 005h, 080h, 033h, 000h
        int3
        int3
sub_2_7968:
        db  0CDh, 020h, 004h, 080h, 033h, 000h
        int3
        int3
sub_2_7970:
        db  0CDh, 020h, 036h, 080h, 033h, 000h
        int3
        int3
sub_2_7978:
        db  0CDh, 020h, 043h, 080h, 033h, 000h
        int3
        int3
sub_2_7980:
        db  0CDh, 020h, 02Dh, 080h, 033h, 000h
        int3
        int3
sub_2_7988:
        db  0CDh, 020h, 02Ch, 080h, 033h, 000h
        int3
        int3
sub_2_7990:
        db  0CDh, 020h, 02Ah, 080h, 033h, 000h
        int3
        int3
sub_2_7998:
        db  0CDh, 020h, 024h, 080h, 033h, 000h
        int3
        int3
sub_2_79A0:
        db  0CDh, 020h, 01Ch, 080h, 033h, 000h
        int3
        int3
sub_2_79A8:
        db  0CDh, 020h, 018h, 080h, 033h, 000h
        int3
        int3
sub_2_79B0:
        db  0CDh, 020h, 01Bh, 080h, 033h, 000h
        int3
        int3
sub_2_79B8:
        db  0CDh, 020h, 040h, 080h, 033h, 000h
        int3
        int3
sub_2_79C0:
        db  0CDh, 020h, 010h, 080h, 033h, 000h
        int3
        int3
sub_2_79C8:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        int3
        int3
sub_2_79D0:
        db  0CDh, 020h, 008h, 080h, 033h, 000h
        int3
        int3
sub_2_79D8:
        db  0CDh, 020h, 03Eh, 080h, 033h, 000h
        int3
        int3
sub_2_79E0:
        db  0CDh, 020h, 03Fh, 080h, 033h, 000h
        int3
        int3
sub_2_79E8:
        db  0CDh, 020h, 049h, 081h, 001h, 000h
        int3
        int3
sub_2_79F0:
        db  0CDh, 020h, 048h, 081h, 001h, 000h
        int3
        int3
sub_2_79F8:
        db  0CDh, 020h, 074h, 081h, 001h, 000h
        int3
        int3
sub_2_7A00:
        db  0CDh, 020h, 05Fh, 080h, 033h, 000h
        int3
        int3
sub_2_7A08:
        db  0CDh, 020h, 00Eh, 080h, 033h, 000h
        int3
        int3
sub_2_7A10:
        db  0CDh, 020h, 00Fh, 080h, 033h, 000h
        int3
        int3
sub_2_7A18:
        db  0CDh, 020h, 00Bh, 080h, 033h, 000h
        int3
        int3
sub_2_7A20:
        db  0CDh, 020h, 03Bh, 080h, 033h, 000h
        int3
        int3
        db  0CDh, 020h, 05Eh, 080h, 033h, 000h
        int3
        int3
sub_2_7A30:
        db  0CDh, 020h, 00Dh, 080h, 033h, 000h
        int3
        int3
sub_2_7A38:
        db  0CDh, 020h, 06Ch, 080h, 001h, 000h
        int3
        int3
sub_2_7A40:
        db  0CDh, 020h, 051h, 081h, 001h, 000h
        int3
        int3
sub_2_7A48:
        db  0CDh, 020h, 00Eh, 080h, 027h, 000h
        int3
        int3
sub_2_7A50:
        db  0CDh, 020h, 00Bh, 080h, 027h, 000h
        int3
        int3
sub_2_7A58:
        db  0CDh, 020h, 007h, 080h, 027h, 000h
        int3
        int3
sub_2_7A60:
        db  0CDh, 020h, 009h, 080h, 033h, 000h
        int3
        int3
sub_2_7A68:
        db  0CDh, 020h, 00Ah, 080h, 033h, 000h
        int3
        int3
sub_2_7A70:
        db  0CDh, 020h, 06Fh, 081h, 001h, 000h
        int3
        int3
sub_2_7A78:
        db  0CDh, 020h, 052h, 081h, 001h, 000h
        int3
        int3
sub_2_7A80:
        db  0CDh, 020h, 094h, 081h, 001h, 000h
        int3
        int3
sub_2_7A88:
        db  0CDh, 020h, 095h, 081h, 001h, 000h
        int3
        int3
sub_2_7A90:
        db  0CDh, 020h, 096h, 081h, 001h, 000h
        int3
        int3
sub_2_7A98:
        db  0CDh, 020h, 019h, 080h, 003h, 000h
        int3
        int3
sub_2_7AA0:
        db  0CDh, 020h, 055h, 080h, 001h, 000h
        int3
        int3
sub_2_7AA8:
        db  0CDh, 020h, 063h, 080h, 001h, 000h
        int3
        int3
sub_2_7AB0:
        db  0CDh, 020h, 028h, 081h, 001h, 000h
        int3
        int3
sub_2_7AB8:
        db  0CDh, 020h, 01Dh, 081h, 001h, 000h
        int3
        int3
sub_2_7AC0:
        db  0CDh, 020h, 015h, 080h, 033h, 000h
        int3
        int3
sub_2_7AC8:
        db  0CDh, 020h, 021h, 080h, 033h, 000h
        int3
        int3
sub_2_7AD0:
        db  0CDh, 020h, 05Bh, 080h, 033h, 000h
        int3
        int3
sub_2_7AD8:
        db  0CDh, 020h, 055h, 080h, 033h, 000h
        int3
        int3
sub_2_7AE0:
        db  0CDh, 020h, 037h, 080h, 033h, 000h
        int3
        int3
sub_2_7AE8:
        db  0CDh, 020h, 035h, 080h, 033h, 000h
        int3
        int3
sub_2_7AF0:
        db  0CDh, 020h, 019h, 080h, 033h, 000h
        int3
        int3
sub_2_7AF8:
        db  0CDh, 020h, 05Ch, 080h, 033h, 000h
        int3
        int3
sub_2_7B00:
        db  0CDh, 020h, 033h, 080h, 033h, 000h
        int3
        int3
sub_2_7B08:
        db  0CDh, 020h, 00Dh, 080h, 027h, 000h
        int3
        int3
sub_2_7B10:
        db  0CDh, 020h, 053h, 080h, 033h, 000h
        int3
        int3
sub_2_7B18:
        db  0CDh, 020h, 00Ch, 080h, 027h, 000h
        int3
        int3
sub_2_7B20:
        db  0CDh, 020h, 002h, 080h, 033h, 000h
        int3
        int3
sub_2_7B28:
        db  0CDh, 020h, 01Dh, 080h, 033h, 000h
        int3
        int3
sub_2_7B30:
        db  0CDh, 020h, 061h, 080h, 033h, 000h
_PTEXT ends
        end
