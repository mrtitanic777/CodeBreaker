; mvpas_pcode_03.asm -- MVPAS object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        push   ebp
        mov    ebp, esp
        sub    esp, 10ch
        push   ebx
        lea    eax, [ebp - 10ch]
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        push   edi
        push   0
        push   100h
        push   eax
        push   0
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0025:
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_3_00EF
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    edx, ecx
        mov    esi, edi
        mov    ecx, 0ffffffffh
        lea    edi, [ebp - 10ch]
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 10ch]
        push   eax
        push   ecx
        push   80000002h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0079:
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_3_00ED
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, 2
loc_3_008A:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 2]
        push   eax
        mov    dword ptr [ebp - 8], edi
        push   ecx
        push   0
        push   0
        push   dword ptr [esi]
        push   dword ptr [ebp - 0ch]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_00A3:
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_3_00C3
        push   1
        lea    eax, [ebp - 2]
        push   edi
        push   eax
        push   3
        push   dword ptr [esi]
        db  068h, 000h, 000h, 000h, 000h
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_00C0:
        add    esp, 1ch
loc_3_00C3:
        add    esi, 4
        db  081h, 0FEh, 010h, 000h, 000h, 000h
        jb     SHORT loc_3_008A
        push   dword ptr [ebp - 0ch]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_00D6:
        add    esp, 4
        lea    eax, [ebp - 10ch]
        push   eax
        push   80000002h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_00EA:
        add    esp, 8
loc_3_00ED:
        xor    eax, eax
loc_3_00EF:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_00F6:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  0BBh, 000h, 000h, 000h, 000h
        push   ebp
        xor    edi, edi
        db  0BDh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [esp + 1ch]
loc_3_010D:
        lea    eax, [esp + 14h]
        push   1
        lea    ecx, [esp + 16h]
        push   eax
        mov    dword ptr [esp + 1ch], 2
        push   ecx
        push   3
        push   dword ptr [ebx]
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0130:
        add    esp, 1ch
        test   eax, eax
        je     SHORT loc_3_0140
        mov    ax, word ptr [ebp]
        mov    word ptr [esp + 12h], ax
loc_3_0140:
        cmp    word ptr [esp + 12h], 0
        je     SHORT loc_3_0158
        mov    edx, 1
        lea    ecx, [edi + 4]
        mov    eax, dword ptr [esp + 20h]
        shl    edx, cl
        or     dword ptr [eax], edx
loc_3_0158:
        db  081h, 0FBh, 00Ch, 000h, 000h, 000h
        jne    SHORT loc_3_016B
        mov    eax, dword ptr [esp + 24h]
        movzx  ecx, word ptr [esp + 12h]
        mov    dword ptr [eax], ecx
loc_3_016B:
        add    ebp, 2
        add    ebx, 4
        inc    edi
        db  081h, 0FBh, 010h, 000h, 000h, 000h
        jb     SHORT loc_3_010D
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_3_0184:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 100h
        test   eax, eax
        push   ebx
        push   esi
        push   edi
        je     SHORT loc_3_01C2
        cmp    eax, 1
        je     SHORT loc_3_01C9
        cmp    eax, 2
        je     NEAR PTR loc_3_0400
        cmp    eax, 3
        je     NEAR PTR loc_3_040B
        cmp    eax, 4
        je     NEAR PTR loc_3_0423
        mov    eax, 1
        jmp    NEAR PTR loc_3_042E
loc_3_01C2:
        xor    eax, eax
        jmp    NEAR PTR loc_3_042E
loc_3_01C9:
        cmp    dword ptr [ebp + 0ch], 1
        jbe    SHORT loc_3_01D9
        mov    eax, 1
        jmp    NEAR PTR loc_3_042E
loc_3_01D9:
        push   0
        mov    edi, dword ptr [ebp + 10h]
        push   edi
        lea    eax, [ebp - 100h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_01EB:
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_3_042E
        mov    dword ptr [ebp - 0ch], 0
        mov    esi, 0ffffh
        cmp    word ptr [ebp - 0a4h], 3
        mov    bx, si
        mov    word ptr [ebp - 6], si
        mov    word ptr [ebp - 2], si
        jae    SHORT loc_3_0221
        mov    eax, 13h
        jmp    NEAR PTR loc_3_042E
loc_3_0221:
        mov    ax, word ptr [ebp - 0a0h]
        cmp    word ptr [ebp - 52h], 1
        mov    cx, word ptr [ebp - 9eh]
        mov    word ptr [ebp - 8], ax
        mov    word ptr [ebp - 4], cx
        jb     NEAR PTR loc_3_03F9
        cmp    word ptr [ebp - 42h], 1
        jb     NEAR PTR loc_3_03F9
        push   edi
        call   0
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_3_042E
        lea    eax, [ebp - 10h]
        lea    ecx, [ebp - 0ch]
        push   eax
        push   ecx
        push   edi
        call   sub_3_00F6
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_3_042E
        lea    eax, [ebp - 28h]
        push   eax
        push   dword ptr [ebp - 4]
        call   sub_3_069E
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_0294
        mov    eax, 24h
        jmp    NEAR PTR loc_3_042E
loc_3_0294:
        test   byte ptr [ebp - 24h], 40h
        je     NEAR PTR loc_3_0364
        movzx  eax, word ptr [ebp - 0a4h]
        cmp    eax, 9
        jl     SHORT loc_3_02B3
        cmp    eax, 0ah
        jle    NEAR PTR loc_3_033C
loc_3_02B3:
        or     byte ptr [ebp - 0ch], 1
        movzx  eax, word ptr [ebp - 52h]
        cmp    eax, 1
        je     SHORT loc_3_0319
        cmp    eax, 2
        je     SHORT loc_3_02F3
        cmp    eax, 3
        je     SHORT loc_3_0320
loc_3_02CA:
        mov    cx, word ptr [ebp - 2]
loc_3_02CE:
        movzx  eax, word ptr [ebp - 42h]
        cmp    eax, 1
        je     SHORT loc_3_02E9
        cmp    eax, 2
        jne    NEAR PTR loc_3_03B5
        movzx  ax, byte ptr [ebp - 3fh]
        mov    word ptr [ebp - 6], ax
loc_3_02E9:
        movzx  ax, byte ptr [ebp - 40h]
        jmp    NEAR PTR loc_3_03B9
loc_3_02F3:
        cmp    word ptr [ebp - 0a4h], 0dh
        jne    SHORT loc_3_030D
        movzx  cx, byte ptr [ebp - 50h]
        movzx  ax, byte ptr [ebp - 4fh]
        mov    word ptr [ebp - 2], ax
        jmp    SHORT loc_3_02CE
loc_3_030D:
        movzx  bx, byte ptr [ebp - 4fh]
        mov    si, word ptr [ebp - 88h]
loc_3_0319:
        movzx  cx, byte ptr [ebp - 50h]
        jmp    SHORT loc_3_02CE
loc_3_0320:
        movzx  bx, byte ptr [ebp - 4eh]
        movzx  ax, byte ptr [ebp - 4fh]
        mov    si, word ptr [ebp - 88h]
        movzx  cx, byte ptr [ebp - 50h]
        mov    word ptr [ebp - 2], ax
        jmp    SHORT loc_3_02CE
loc_3_033C:
        movzx  eax, word ptr [ebp - 52h]
        cmp    eax, 1
        je     SHORT loc_3_035A
        cmp    eax, 2
        jne    NEAR PTR loc_3_02CA
        movzx  bx, byte ptr [ebp - 4fh]
        mov    si, word ptr [ebp - 90h]
loc_3_035A:
        movzx  cx, byte ptr [ebp - 50h]
        jmp    NEAR PTR loc_3_02CE
loc_3_0364:
        movzx  eax, word ptr [ebp - 0a4h]
        mov    si, word ptr [ebp - 90h]
        cmp    eax, 0ah
        je     SHORT loc_3_03AB
        or     byte ptr [ebp - 0ch], 1
        movzx  eax, word ptr [ebp - 52h]
        cmp    eax, 1
        je     SHORT loc_3_038E
        cmp    eax, 2
        je     SHORT loc_3_0398
        jmp    NEAR PTR loc_3_02CA
loc_3_038E:
        movzx  cx, byte ptr [ebp - 50h]
        jmp    NEAR PTR loc_3_02CE
loc_3_0398:
        movzx  cx, byte ptr [ebp - 50h]
        movzx  ax, byte ptr [ebp - 4fh]
        mov    word ptr [ebp - 2], ax
        jmp    NEAR PTR loc_3_02CE
loc_3_03AB:
        movzx  cx, byte ptr [ebp - 50h]
        jmp    NEAR PTR loc_3_02CE
loc_3_03B5:
        mov    ax, word ptr [ebp - 2]
loc_3_03B9:
        cmp    word ptr [ebp - 8], 0ffffh
        je     SHORT loc_3_03F2
        cmp    word ptr [ebp - 4], 0ffffh
        je     SHORT loc_3_03F2
        lea    edx, [ebp - 28h]
        push   edx
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 2]
        push   0
        push   0ffffh
        push   dword ptr [ebp - 6]
        push   ebx
        push   esi
        push   eax
        push   ecx
        push   dword ptr [ebp - 4]
        push   edi
        call   sub_3_0790
        add    esp, 34h
        jmp    SHORT loc_3_042E
loc_3_03F2:
        mov    eax, 13h
        jmp    SHORT loc_3_042E
loc_3_03F9:
        mov    eax, 13h
        jmp    SHORT loc_3_042E
loc_3_0400:
        cmp    dword ptr [ebp + 0ch], 0
        mov    eax, 1
        jmp    SHORT loc_3_042E
loc_3_040B:
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, eax
        je     SHORT loc_3_041C
        cmp    eax, 1
        mov    eax, 1
        jmp    SHORT loc_3_042E
loc_3_041C:
        mov    eax, 1
        jmp    SHORT loc_3_042E
loc_3_0423:
        push   dword ptr [ebp + 10h]
        call   sub_3_0BC1
        add    esp, 4
loc_3_042E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_0435:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ax, word ptr [eax + 14h]
        mov    ebp, esp
        sub    esp, 4
        mov    word ptr [ebp - 4], ax
        mov    dx, word ptr [ebp - 4]
        xor    dx, word ptr [ebp + 0ch]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ebp - 1]
        mov    esp, ebp
        pop    ebp
        ret
sub_3_045A:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ax, word ptr [eax + 14h]
        mov    ebp, esp
        sub    esp, 4
        mov    word ptr [ebp - 2], ax
        mov    dx, word ptr [ebp - 2]
        xor    dx, word ptr [ebp + 0ch]
        mov    al, byte ptr [ebp + 10h]
        out    dx, al
        mov    esp, ebp
        pop    ebp
        ret
sub_3_047C:
        push   ebx
        xor    eax, eax
        push   esi
        mov    ecx, 6
        mov    esi, dword ptr [esp + 10h]
        push   edi
        mov    edi, esi
        rep stosd
        mov    di, word ptr [esp + 10h]
        push   0b8bh
        mov    ax, di
        push   esi
        xor    ax, 388h
        mov    word ptr [esi + 14h], ax
        call   sub_3_0435
        add    esp, 8
        mov    bl, al
        cmp    bl, 0ffh
        je     SHORT loc_3_0513
        mov    al, bl
        shr    al, 5
        movzx  ax, al
        mov    word ptr [esi], ax
        movzx  eax, ax
        test   eax, eax
        jl     SHORT loc_3_0513
        cmp    eax, 1
        jle    SHORT loc_3_04DA
        cmp    eax, 3
        jl     SHORT loc_3_0513
        cmp    eax, 5
        jle    SHORT loc_3_04DA
        cmp    eax, 7
        jne    SHORT loc_3_0513
loc_3_04DA:
        mov    al, bl
        xor    al, 0e0h
        push   eax
        push   0b8bh
        push   esi
        call   sub_3_045A
        add    esp, 0ch
        push   0b8bh
        push   esi
        call   sub_3_0435
        add    esp, 8
        mov    cl, bl
        xor    cl, al
        test   cl, 0e0h
        je     SHORT loc_3_0525
        push   ebx
        push   0b8bh
        push   esi
        call   sub_3_045A
        add    esp, 0ch
loc_3_0513:
        mov    word ptr [esi], 0
        xor    eax, eax
        mov    dword ptr [esi + 4], 0
loc_3_0521:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_0525:
        cmp    word ptr [esi], 0
        jne    SHORT loc_3_055E
        or     byte ptr [esi + 4], 3
        push   0
        push   7f89h
        push   esi
        call   sub_3_045A
        add    esp, 0ch
        push   7f89h
        push   esi
        call   sub_3_0435
        add    esp, 8
        test   al, 1
        jne    NEAR PTR loc_3_061B
        or     byte ptr [esi + 4], 4
        jmp    NEAR PTR loc_3_061B
loc_3_055E:
        push   8388h
        push   esi
        call   sub_3_0435
        add    esp, 8
        test   al, 80h
        jne    SHORT loc_3_0513
        push   0ef8bh
        push   esi
        call   sub_3_0435
        add    esp, 8
        test   al, 4
        je     SHORT loc_3_0586
        or     byte ptr [esi + 4], 10h
loc_3_0586:
        test   al, 8
        je     SHORT loc_3_059F
        mov    ecx, dword ptr [esi + 4]
        or     cl, 8
        mov    dword ptr [esi + 4], ecx
        or     cl, 40h
        mov    dword ptr [esi + 4], ecx
        or     cl, 20h
        mov    dword ptr [esi + 4], ecx
loc_3_059F:
        movzx  eax, al
        mov    ecx, dword ptr [esi + 4]
        xor    eax, ecx
        and    eax, 3
        xor    eax, ecx
        mov    ecx, eax
        mov    dword ptr [esi + 4], eax
        and    cl, 3
        cmp    cl, 3
        jne    SHORT loc_3_05C4
        mov    word ptr [esi], 1
        or     ah, 10h
        mov    dword ptr [esi + 4], eax
loc_3_05C4:
        mov    eax, dword ptr [esi + 4]
        push   0ff8bh
        or     ah, 1
        push   esi
        mov    dword ptr [esi + 4], eax
        or     ah, 8
        mov    dword ptr [esi + 4], eax
        call   sub_3_0435
        add    esp, 8
        test   al, 1
        jne    SHORT loc_3_05E9
        or     byte ptr [esi + 5], 2
loc_3_05E9:
        test   al, 4
        je     SHORT loc_3_05FB
        mov    eax, dword ptr [esi + 4]
        or     ah, 4
        mov    dword ptr [esi + 4], eax
        and    al, 0dfh
        mov    dword ptr [esi + 4], eax
loc_3_05FB:
        push   0ff88h
        push   esi
        call   sub_3_0435
        movzx  ax, al
        mov    word ptr [esi + 2], ax
        add    esp, 8
        cmp    ax, 4
        jb     SHORT loc_3_061B
        or     byte ptr [esi + 4], 80h
loc_3_061B:
        mov    word ptr [esi + 8], di
        mov    eax, 1
        jmp    NEAR PTR loc_3_0521
sub_3_0629:
        mov    ecx, dword ptr [esp + 4]
        test   ecx, ecx
        jle    SHORT loc_3_063A
loc_3_0631:
        mov    edx, 9a01h
        dec    ecx
        in     al, dx
        jne    SHORT loc_3_0631
loc_3_063A:
        ret
sub_3_063B:
        push   ebx
        mov    edx, 8388h
        push   esi
        mov    al, 0c0h
        out    dx, al
        push   edi
        push   ebp
        push   0ah
        xor    edi, edi
        call   sub_3_0629
        mov    bx, word ptr [esp + 18h]
        mov    esi, dword ptr [esp + 1ch]
        shr    bx, 2
        add    esp, 4
loc_3_0660:
        db  08Ah, 08Fh, 000h, 000h, 000h, 000h
        mov    ebp, 14h
loc_3_066B:
        mov    edx, 9a01h
        mov    al, cl
        out    dx, al
        mov    al, bl
        dec    ebp
        out    dx, al
        jne    SHORT loc_3_066B
        push   esi
        push   dword ptr [esp + 18h]
        call   sub_3_047C
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_0697
        inc    edi
        cmp    edi, 4
        jb     SHORT loc_3_0660
        xor    eax, eax
loc_3_0692:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_0697:
        mov    eax, 1
        jmp    SHORT loc_3_0692
sub_3_069E:
        push   esi
        push   edi
        mov    si, word ptr [esp + 0ch]
        mov    edi, dword ptr [esp + 10h]
        push   edi
        push   esi
        call   sub_3_047C
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_06CA
        push   edi
        push   esi
        call   sub_3_063B
        add    esp, 8
        test   eax, eax
        mov    eax, 0
        je     SHORT loc_3_06CF
loc_3_06CA:
        mov    eax, 1
loc_3_06CF:
        pop    edi
        pop    esi
        ret
sub_3_06D2:
        mov    eax, dword ptr [esp + 4]
        movzx  eax, word ptr [eax]
        cmp    eax, 3
        jl     SHORT loc_3_06E8
        cmp    eax, 5
        jle    SHORT loc_3_06EC
        cmp    eax, 7
        je     SHORT loc_3_06EC
loc_3_06E8:
        xor    eax, eax
        jmp    SHORT loc_3_06F1
loc_3_06EC:
        mov    eax, 1
loc_3_06F1:
        ret
sub_3_06F2:
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   0
        push   0b8ah
        push   esi
        call   sub_3_045A
        add    esp, 0ch
        push   0b8bh
        push   esi
        call   sub_3_0435
        add    esp, 8
        and    al, 0f3h
        push   eax
        push   0b8bh
        push   esi
        call   sub_3_045A
        add    esp, 0ch
        push   esi
        call   sub_3_06D2
        add    esp, 4
        test   eax, eax
        je     SHORT loc_3_075A
        push   0b88h
        push   esi
        call   sub_3_0435
        add    esp, 8
        mov    bl, al
        or     al, 20h
        push   eax
        push   0b88h
        push   esi
        call   sub_3_045A
        add    esp, 0ch
        jmp    SHORT loc_3_075E
loc_3_075A:
        mov    bl, byte ptr [esp + 0bh]
loc_3_075E:
        push   19h
        push   0f8ah
        push   esi
        call   sub_3_045A
        add    esp, 0ch
        push   esi
        call   sub_3_06D2
        add    esp, 4
        test   eax, eax
        je     SHORT loc_3_078A
        push   ebx
        push   0b88h
        push   esi
        call   sub_3_045A
        add    esp, 0ch
loc_3_078A:
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_3_0790:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cli
        mov    esi, dword ptr [ebp + 38h]
        push   esi
        push   dword ptr [ebp + 0ch]
        call   sub_3_063B
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_07B7
        mov    eax, 24h
        jmp    NEAR PTR loc_3_0BBA
loc_3_07B7:
        test   byte ptr [esi + 5], 4
        jne    SHORT loc_3_07EB
        push   8388h
        push   esi
        call   sub_3_0435
        add    esp, 8
        and    al, 0fdh
        mov    edi, dword ptr [ebp + 34h]
        test   edi, 20h
        je     SHORT loc_3_07DA
        or     al, 4
loc_3_07DA:
        push   eax
        push   8388h
        push   esi
        call   sub_3_045A
        add    esp, 0ch
        jmp    SHORT loc_3_07EE
loc_3_07EB:
        mov    edi, dword ptr [ebp + 34h]
loc_3_07EE:
        push   0ah
        push   4388h
        push   esi
        call   sub_3_045A
        add    esp, 0ch
        xor    al, al
        test   edi, 80h
        je     SHORT loc_3_080D
        mov    eax, dword ptr [ebp + 30h]
        and    al, 0fh
loc_3_080D:
        push   eax
        push   0bf88h
        push   esi
        call   sub_3_045A
        add    esp, 0ch
        push   0
        push   0b8ah
        push   esi
        call   sub_3_045A
        add    esp, 0ch
        push   36h
        push   138bh
        push   esi
        call   sub_3_045A
        add    esp, 0ch
        push   0dh
        push   1388h
        push   esi
        call   sub_3_045A
        add    esp, 0ch
        push   0
        push   1388h
        push   esi
        call   sub_3_045A
        add    esp, 0ch
        push   74h
        push   138bh
        push   esi
        call   sub_3_045A
        add    esp, 0ch
        push   0
        push   1389h
        push   esi
        call   sub_3_045A
        add    esp, 0ch
        push   0
        push   1389h
        push   esi
        call   sub_3_045A
        add    esp, 0ch
        test   byte ptr [esi + 4], 20h
        je     SHORT loc_3_08B2
        push   0
        push   0b88h
        push   esi
        call   sub_3_045A
        add    esp, 0ch
        push   8
        push   0b88h
        push   esi
        call   sub_3_045A
        add    esp, 0ch
loc_3_08B2:
        mov    eax, dword ptr [esi + 4]
        test   al, 8
        je     SHORT loc_3_08D2
        test   ah, 4
        je     SHORT loc_3_08C2
        push   0
        jmp    SHORT loc_3_08C4
loc_3_08C2:
        push   18h
loc_3_08C4:
        push   838ah
        push   esi
        call   sub_3_045A
        add    esp, 0ch
loc_3_08D2:
        xor    al, al
        test   edi, 10h
        je     SHORT loc_3_08DE
        mov    al, 40h
loc_3_08DE:
        test   edi, 40h
        je     SHORT loc_3_08E8
        or     al, 80h
loc_3_08E8:
        push   eax
        push   0f388h
        push   esi
        call   sub_3_045A
        movzx  eax, word ptr [ebp + 14h]
        add    esp, 0ch
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        push   eax
        push   0f389h
        push   esi
        call   sub_3_045A
        movzx  ecx, word ptr [ebp + 10h]
        add    esp, 0ch
        db  08Ah, 081h, 000h, 000h, 000h, 000h
        push   eax
        push   0f38ah
        push   esi
        call   sub_3_045A
        add    esp, 0ch
        push   8
        push   0bf8ah
        push   esi
        call   sub_3_045A
        add    esp, 0ch
        mov    eax, dword ptr [esi + 4]
        test   ah, 4
        je     SHORT loc_3_0951
        test   al, 4
        jne    SHORT loc_3_0951
        test   edi, 1
        jne    SHORT loc_3_0951
        push   68h
        jmp    SHORT loc_3_0953
loc_3_0951:
        push   49h
loc_3_0953:
        push   838bh
        push   esi
        call   sub_3_045A
        mov    di, word ptr [ebp + 2ch]
        add    esp, 0ch
        cmp    di, 0ffffh
        je     SHORT loc_3_09C2
        push   0f38ah
        push   esi
        call   sub_3_0435
        movzx  ecx, di
        add    esp, 8
        and    al, 0fh
        db  08Ah, 089h, 000h, 000h, 000h, 000h
        shl    cl, 4
        or     al, cl
        push   eax
        push   0f38ah
        push   esi
        call   sub_3_045A
        add    esp, 0ch
        push   838bh
        push   esi
        call   sub_3_0435
        add    esp, 8
        or     al, 24h
        push   eax
        push   838bh
        push   esi
        call   sub_3_045A
        add    esp, 0ch
        push   0ah
        call   sub_3_0629
        add    esp, 4
loc_3_09C2:
        push   esi
        call   sub_3_06F2
        add    esp, 4
        cmp    word ptr [esi], 1
        je     SHORT loc_3_09F0
        push   0b89h
        push   esi
        call   sub_3_0435
        add    esp, 8
        and    al, 0fdh
        push   eax
        push   0b89h
        push   esi
        call   sub_3_045A
        add    esp, 0ch
loc_3_09F0:
        mov    di, word ptr [ebp + 24h]
        xor    ebx, ebx
        mov    word ptr [esi + 10h], di
        mov    al, byte ptr [ebp + 28h]
        push   ebx
        mov    byte ptr [ebp - 1], bl
        push   0f788h
        mov    byte ptr [ebp - 2], bl
        push   esi
        mov    byte ptr [esi + 12h], al
        call   sub_3_045A
        add    esp, 0ch
        push   ebx
        push   0f789h
        push   esi
        call   sub_3_045A
        add    esp, 0ch
        push   ebx
        push   0fb8ah
        push   esi
        call   sub_3_045A
        add    esp, 0ch
        test   byte ptr [esi + 4], 80h
        je     SHORT loc_3_0A8B
        cmp    di, 0ffffh
        je     SHORT loc_3_0A4C
        mov    byte ptr [ebp - 2], 1
        mov    ax, di
        and    al, 0f0h
        mov    byte ptr [ebp - 1], al
loc_3_0A4C:
        push   dword ptr [ebp - 2]
        push   0f788h
        push   esi
        call   sub_3_045A
        add    esp, 0ch
        push   dword ptr [ebp - 1]
        push   0f789h
        push   esi
        call   sub_3_045A
        add    esp, 0ch
        cmp    di, 0ffffh
        je     SHORT loc_3_0A7C
        mov    bx, word ptr [ebp + 28h]
        and    bl, 3
loc_3_0A7C:
        push   ebx
        push   0fb8ah
        push   esi
        call   sub_3_045A
        add    esp, 0ch
loc_3_0A8B:
        mov    ax, word ptr [ebp + 18h]
        cmp    ax, 0ffffh
        je     NEAR PTR loc_3_0B6E
        cmp    word ptr [esi + 2], 4
        mov    word ptr [esi + 0ch], ax
        jb     SHORT loc_3_0AE7
        or     byte ptr [ebp - 2], 2
        shr    ax, 4
        and    al, 0fh
        or     byte ptr [ebp - 1], al
        test   byte ptr [esi + 4], 40h
        je     SHORT loc_3_0AE7
        mov    ax, word ptr [ebp + 1ch]
        mov    cx, word ptr [ebp + 20h]
        mov    byte ptr [esi + 0fh], al
        movzx  edx, ax
        mov    byte ptr [esi + 0eh], cl
        shl    cl, 3
        db  08Ah, 082h, 000h, 000h, 000h, 000h
        or     al, cl
        shl    al, 3
        or     bl, al
        push   ebx
        push   0fb8ah
        push   esi
        call   sub_3_045A
        add    esp, 0ch
loc_3_0AE7:
        push   dword ptr [ebp - 2]
        push   0f788h
        push   esi
        call   sub_3_045A
        add    esp, 0ch
        push   dword ptr [ebp - 1]
        push   0f789h
        push   esi
        call   sub_3_045A
        add    esp, 0ch
        push   0b88h
        push   esi
        call   sub_3_0435
        add    esp, 8
        or     al, 10h
        push   eax
        push   0b88h
        push   esi
        call   sub_3_045A
        add    esp, 0ch
        push   0b8ah
        push   esi
        call   sub_3_0435
        add    esp, 8
        and    al, 0f9h
        or     al, 39h
        push   eax
        push   0b8ah
        push   esi
        call   sub_3_045A
        add    esp, 0ch
        test   byte ptr [esi + 4], 40h
        je     SHORT loc_3_0B6E
        push   0b89h
        push   esi
        call   sub_3_0435
        add    esp, 8
        or     al, 2
        push   eax
        push   0b89h
        push   esi
        call   sub_3_045A
        add    esp, 0ch
loc_3_0B6E:
        push   0b8ah
        push   esi
        call   sub_3_0435
        add    esp, 8
        and    al, 0f1h
        or     al, 21h
        push   eax
        push   0b8ah
        push   esi
        call   sub_3_045A
        add    esp, 0ch
        push   0
        push   0b8bh
        push   esi
        call   sub_3_045A
        add    esp, 0ch
        push   1
        call   sub_3_0629
        add    esp, 4
        push   0b8bh
        push   esi
        call   sub_3_0435
        add    esp, 8
        sti
        xor    eax, eax
loc_3_0BBA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_0BC1:
        xor    eax, eax
        ret
_PTEXT ends
        end
