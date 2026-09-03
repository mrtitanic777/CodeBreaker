; pccard_lcode_01_03.asm -- PCCARD object 1 segment _LTEXT  (part 3/3)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_8C67:
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   edi
        test   eax, eax
        mov    edi, dword ptr [esp + 0ch]
        je     SHORT loc_1_8C80
loc_1_8C76:
        cmp    edi, eax
        je     SHORT loc_1_8C8B
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    SHORT loc_1_8C76
loc_1_8C80:
        cmp    edi, eax
        je     SHORT loc_1_8C8B
        mov    eax, 19h
        jmp    SHORT loc_1_8CBB
loc_1_8C8B:
        mov    esi, dword ptr [edi + 10h]
        test   esi, esi
        je     SHORT loc_1_8CAA
loc_1_8C92:
        mov    ecx, esi
        call   sub_1_2967
        test   al, 80h
        je     SHORT loc_1_8CA4
        mov    ecx, esi
        call   sub_1_5614
loc_1_8CA4:
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    SHORT loc_1_8C92
loc_1_8CAA:
        push   edi
        call   sub_1_8961
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_8CBB
        or     byte ptr [edi + 8], 10h
loc_1_8CBB:
        pop    edi
        pop    esi
        ret
sub_1_8CBE:
        push   ebx
        db  0A1h, 000h, 000h, 000h, 000h
        push   esi
        test   eax, eax
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        je     SHORT loc_1_8CD8
loc_1_8CCE:
        cmp    eax, esi
        je     SHORT loc_1_8CE3
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    SHORT loc_1_8CCE
loc_1_8CD8:
        cmp    eax, esi
        je     SHORT loc_1_8CE3
        mov    eax, 19h
        jmp    SHORT loc_1_8D61
loc_1_8CE3:
        lea    edi, [esi + 8]
        mov    eax, dword ptr [esi + 10h]
        and    byte ptr [edi], 0efh
        test   eax, eax
        je     SHORT loc_1_8CFA
loc_1_8CF0:
        or     byte ptr [eax + 14h], 20h
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    SHORT loc_1_8CF0
loc_1_8CFA:
        add    esi, 20h
        xor    ebx, ebx
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     SHORT loc_1_8D14
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_8D0B:
        push   dword ptr [esi]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_8D12:
        mov    dword ptr [esi], ebx
loc_1_8D14:
        mov    eax, dword ptr [edi]
        test   al, 1
        je     SHORT loc_1_8D5F
        and    al, 0feh
        mov    dword ptr [edi], eax
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_8D5F
        xor    esi, esi
        db  039h, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_8D41
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_8D3B:
        db  089h, 035h, 000h, 000h, 000h, 000h
loc_1_8D41:
        xor    esi, esi
        db  039h, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_8D5F
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_9880
        add    esp, 4
        db  089h, 035h, 000h, 000h, 000h, 000h
loc_1_8D5F:
        xor    eax, eax
loc_1_8D61:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_8D65:
        push   ebp
        mov    edx, dword ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 28h
        xor    eax, eax
        mov    ecx, 8
        push   ebx
        push   esi
        push   edi
        lea    esi, [edx + 0ch]
        lea    edi, [ebp - 28h]
        rep stosd
        stosw
        stosb
        mov    edi, dword ptr [ebp + 10h]
        and    edi, 40000000h
        je     SHORT loc_1_8DA8
        movzx  eax, word ptr [esi + 4]
        mov    dword ptr [ebp - 16h], eax
        movzx  ecx, word ptr [esi + 6]
        mov    dword ptr [ebp - 12h], ecx
        movzx  eax, word ptr [esi]
        not    eax
        inc    eax
        mov    dword ptr [ebp - 0ah], eax
        jmp    SHORT loc_1_8DAF
loc_1_8DA8:
        movzx  eax, word ptr [edx + 4]
        mov    dword ptr [ebp - 1eh], eax
loc_1_8DAF:
        mov    word ptr [ebp - 20h], 4
        mov    byte ptr [ebp - 6], 80h
        mov    ebx, dword ptr [ebp + 14h]
        test   byte ptr [ebp + 10h], 8
        movzx  eax, word ptr [ebx]
        mov    dword ptr [ebp - 1ah], eax
        je     SHORT loc_1_8DCE
        mov    word ptr [ebp - 20h], 0ch
loc_1_8DCE:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 10h]
        lea    edx, [ebp - 4]
        mov    dword ptr [ebp - 24h], eax
        push   edx
        mov    dword ptr [ebp - 28h], ecx
        lea    ecx, [ebp - 28h]
        push   ecx
        call   sub_1_3B7B
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_8E07
        cmp    eax, 0ah
        jne    SHORT loc_1_8E1B
        test   ebx, ebx
        je     SHORT loc_1_8E1B
        movzx  ecx, word ptr [ebx]
        cmp    ecx, dword ptr [ebp - 0eh]
        jbe    SHORT loc_1_8E1B
        mov    ecx, dword ptr [ebp - 0eh]
        mov    word ptr [ebx], cx
        jmp    SHORT loc_1_8E1B
loc_1_8E07:
        test   edi, edi
        je     SHORT loc_1_8E22
        mov    eax, dword ptr [ebp - 16h]
        mov    word ptr [esi + 4], ax
        mov    ecx, dword ptr [ebp - 12h]
        mov    word ptr [esi + 6], cx
loc_1_8E19:
        xor    eax, eax
loc_1_8E1B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_8E22:
        mov    eax, dword ptr [ebp + 8]
        db  081h, 078h, 010h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_8E19
        test   byte ptr [ebp + 13h], 80h
        jne    SHORT loc_1_8E19
        push   dword ptr [ebp - 4]
        call   sub_1_4053
        add    esp, 4
        jmp    SHORT loc_1_8E1B
sub_1_8E41:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        add    eax, 8
        sub    esp, 18h
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [eax]
        mov    ecx, 60000000h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 10h]
        mov    edx, eax
        and    edx, ecx
        cmp    edx, ecx
        jne    SHORT loc_1_8E70
        mov    eax, 1eh
        jmp    NEAR PTR loc_1_90CE
loc_1_8E70:
        test   eax, 1000000h
        jne    NEAR PTR loc_1_90C9
        test   eax, 200000h
        je     SHORT loc_1_8E8D
        test   eax, 20000000h
        je     NEAR PTR loc_1_90C9
loc_1_8E8D:
        test   eax, 400000h
        je     SHORT loc_1_8E9F
        test   eax, 40000000h
        je     NEAR PTR loc_1_90C9
loc_1_8E9F:
        push   1bh
        call   sub_1_2540
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_1_8EB9
        mov    eax, 20h
        jmp    NEAR PTR loc_1_90CE
loc_1_8EB9:
        mov    eax, dword ptr [ebp + 14h]
        and    eax, 40000000h
        mov    dword ptr [ebp - 18h], eax
        je     SHORT loc_1_8ED7
        lea    ecx, [esi + 0ch]
        movzx  eax, word ptr [edi + 0eh]
        mov    dword ptr [ecx], eax
        mov    dl, byte ptr [edi + 17h]
        mov    byte ptr [esi + 18h], dl
        jmp    SHORT loc_1_8F04
loc_1_8ED7:
        movzx  eax, word ptr [edi + 4]
        mov    dword ptr [esi + 8], eax
        test   eax, eax
        jne    SHORT loc_1_8EEE
        mov    dword ptr [ebp - 10h], 3
        jmp    NEAR PTR loc_1_902C
loc_1_8EEE:
        movzx  edx, word ptr [edi + 6]
        movzx  eax, word ptr [edi + 4]
        lea    ecx, [esi + 0ch]
        sub    edx, eax
        inc    edx
        mov    dword ptr [ecx], edx
        mov    al, byte ptr [edi + 0bh]
        mov    byte ptr [esi + 18h], al
loc_1_8F04:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        mov    byte ptr [esi + 1ah], 1
        mov    bl, byte ptr [ebp + 14h]
        mov    dword ptr [esi + 10h], eax
        mov    dword ptr [esi + 14h], edx
        mov    byte ptr [esi + 19h], bl
        mov    ax, word ptr [ecx]
        mov    word ptr [ebp - 4], ax
        mov    word ptr [ebp - 2], ax
        lea    eax, [ebp - 2]
        push   eax
        push   dword ptr [ebp + 14h]
        push   edi
        push   esi
        call   sub_1_8D65
        add    esp, 10h
        mov    dword ptr [ebp - 10h], eax
        test   eax, eax
        je     NEAR PTR loc_1_903D
        mov    ax, word ptr [ebp - 4]
        cmp    word ptr [ebp - 2], ax
        jae    NEAR PTR loc_1_9018
        cmp    word ptr [ebp - 2], 0
        je     NEAR PTR loc_1_9018
        mov    ax, word ptr [edi + 0ch]
        mov    cx, word ptr [edi + 4]
        mov    dx, word ptr [edi + 6]
        mov    word ptr [ebp - 0ch], ax
        mov    ax, 8000h
        mov    word ptr [ebp - 0ah], cx
        test   word ptr [ebp - 2], ax
        mov    word ptr [ebp - 8], dx
        jne    SHORT loc_1_8F85
loc_1_8F7C:
        shr    ax, 1
        test   word ptr [ebp - 2], ax
        je     SHORT loc_1_8F7C
loc_1_8F85:
        xor    bx, bx
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [ebp - 4], bx
        je     SHORT loc_1_8FFA
loc_1_8F92:
        mov    ax, word ptr [ebp - 2]
        movzx  ecx, bx
        movzx  edx, ax
        mov    word ptr [ebp - 6], ax
        add    ecx, edx
        movzx  eax, word ptr [ebp - 4]
        cmp    ecx, eax
        jle    SHORT loc_1_8FB5
        mov    ax, word ptr [ebp - 4]
        sub    ax, bx
        mov    word ptr [ebp - 6], ax
loc_1_8FB5:
        mov    ax, word ptr [edi + 4]
        lea    ecx, [ebp - 6]
        add    ax, word ptr [ebp - 6]
        push   ecx
        dec    ax
        push   dword ptr [ebp + 14h]
        push   edi
        mov    word ptr [edi + 6], ax
        push   esi
        call   sub_1_8D65
        add    esp, 10h
        mov    dword ptr [ebp - 10h], eax
        test   eax, eax
        jne    SHORT loc_1_8FFA
        mov    ax, word ptr [ebp - 6]
        add    word ptr [edi + 4], ax
        mov    ax, word ptr [ebp - 2]
        add    bx, word ptr [ebp - 6]
        dec    ax
        not    ax
        mov    word ptr [edi + 0ch], ax
        cmp    bx, word ptr [ebp - 4]
        jb     SHORT loc_1_8F92
loc_1_8FFA:
        mov    ax, word ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 10h], 0
        mov    cx, word ptr [ebp - 0ah]
        mov    dx, word ptr [ebp - 8]
        mov    word ptr [edi + 0ch], ax
        mov    word ptr [edi + 4], cx
        mov    word ptr [edi + 6], dx
        je     SHORT loc_1_903D
loc_1_9018:
        push   esi
        call   sub_1_4245
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_902C
        mov    dword ptr [ebp - 10h], 19h
loc_1_902C:
        push   esi
        call   sub_1_254F
        add    esp, 4
        mov    eax, dword ptr [ebp - 10h]
        jmp    NEAR PTR loc_1_90CE
loc_1_903D:
        cmp    dword ptr [ebp - 18h], 0
        je     SHORT loc_1_904E
        push   esi
        call   sub_1_254F
        add    esp, 4
        jmp    SHORT loc_1_90C5
loc_1_904E:
        db  081h, 07Dh, 008h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_90A1
        test   byte ptr [ebp + 17h], 80h
        jne    SHORT loc_1_907F
        mov    ecx, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ecx]
        mov    eax, ecx
        and    eax, 200000h
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0ffe00000h
        add    eax, 400000h
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ecx], eax
loc_1_907F:
        mov    ecx, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ecx]
        mov    eax, ecx
        and    eax, 20000000h
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0e0000000h
        add    eax, 40000000h
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ecx], eax
loc_1_90A1:
        push   4
        mov    eax, dword ptr [ebp + 0ch]
        push   esi
        add    eax, 24h
        push   eax
        call   sub_1_2D57
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        add    eax, 1ch
        push   0
        push   esi
        push   eax
        call   sub_1_2D57
        add    esp, 0ch
loc_1_90C5:
        xor    eax, eax
        jmp    SHORT loc_1_90CE
loc_1_90C9:
        mov    eax, 19h
loc_1_90CE:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_90D5:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 1ch
        mov    ecx, dword ptr [eax + 18h]
        push   ebx
        push   esi
        mov    dword ptr [ebp - 0ch], ecx
        push   edi
        db  08Bh, 035h, 00Ah, 000h, 000h, 000h
        add    ecx, 8
        mov    dword ptr [ebp - 8], ecx
        mov    edx, dword ptr [ecx]
        test   edx, 11000000h
        je     SHORT loc_1_9108
        mov    eax, 1dh
        jmp    NEAR PTR loc_1_93C3
loc_1_9108:
        test   edx, 600000h
        je     SHORT loc_1_911A
        mov    eax, 1eh
        jmp    NEAR PTR loc_1_93C3
loc_1_911A:
        db  0A1h, 00Ah, 000h, 000h, 000h
        cmp    byte ptr [eax + 4], 0
        je     NEAR PTR loc_1_93BE
        mov    cl, byte ptr [eax + 8]
        test   cl, cl
        je     SHORT loc_1_9153
        mov    di, word ptr [eax + 6]
        mov    ax, word ptr [eax + 2]
        cmp    di, ax
        je     NEAR PTR loc_1_93BE
        test   ax, ax
        je     NEAR PTR loc_1_93BE
        test   di, di
        je     NEAR PTR loc_1_93BE
loc_1_9153:
        xor    di, di
        xor    eax, eax
loc_1_9158:
        movzx  ebx, di
        inc    di
        mov    dword ptr [ebp + ebx*4 - 14h], eax
        cmp    di, 2
        jb     SHORT loc_1_9158
        test   edx, 8000000h
        je     NEAR PTR loc_1_9302
        test   edx, 20000000h
        je     NEAR PTR loc_1_92F8
        test   cl, cl
        je     SHORT loc_1_918F
        test   edx, 40000000h
        je     NEAR PTR loc_1_92F8
loc_1_918F:
        xor    bx, bx
loc_1_9192:
        movzx  eax, bx
        lea    edi, [eax*4]
        mov    al, byte ptr [esi + eax*4 + 4]
        test   al, al
        je     NEAR PTR loc_1_925F
        movzx  eax, al
        push   eax
        call   sub_1_27A5
        mov    word ptr [ebp - 2], ax
        add    esp, 4
        lea    ecx, [ebp + edi - 14h]
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 18h], 1
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [ecx], eax
        test   eax, eax
        je     SHORT loc_1_9233
loc_1_91CF:
        mov    eax, dword ptr [ecx]
        cmp    byte ptr [eax + 1ah], 1
        jne    SHORT loc_1_9228
        db  081h, 078h, 010h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_9228
        mov    dword ptr [ebp - 18h], 0
        cmp    word ptr [edi + esi + 2], 0
        je     SHORT loc_1_91FB
        movzx  edx, word ptr [edi + esi + 2]
        cmp    dword ptr [eax + 8], edx
        je     SHORT loc_1_9210
        jmp    SHORT loc_1_9228
loc_1_91FB:
        movzx  eax, word ptr [ebp - 2]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [edx + 8]
        sub    edx, edx
        div    dword ptr [ebp - 1ch]
        test   edx, edx
        jne    SHORT loc_1_9228
loc_1_9210:
        mov    eax, dword ptr [ecx]
        movzx  edx, byte ptr [edi + esi + 4]
        cmp    edx, dword ptr [eax + 0ch]
        je     SHORT loc_1_9233
        cmp    word ptr [edi + esi + 2], 0
        jne    NEAR PTR loc_1_92B3
loc_1_9228:
        mov    eax, dword ptr [ecx]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], eax
        test   eax, eax
        jne    SHORT loc_1_91CF
loc_1_9233:
        cmp    dword ptr [ebp - 18h], 0
        jne    NEAR PTR loc_1_92EA
        mov    eax, dword ptr [ecx]
        test   eax, eax
        je     NEAR PTR loc_1_92F1
        inc    bx
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_6288
        add    esp, 8
        cmp    bx, 2
        jb     NEAR PTR loc_1_9192
loc_1_925F:
        xor    cx, cx
loc_1_9262:
        movzx  eax, cx
        lea    edx, [eax*4]
        cmp    byte ptr [edx + esi + 4], 0
        je     SHORT loc_1_92AC
        mov    eax, dword ptr [ebp - 8]
        mov    ebx, dword ptr [ebp + edx - 14h]
        mov    eax, dword ptr [eax]
        mov    edi, eax
        and    edi, 200000h
        cmp    edi, 1
        sbb    edi, edi
        inc    cx
        and    edi, 0ffe00000h
        add    edi, 400000h
        or     edi, eax
        mov    eax, dword ptr [ebp - 8]
        cmp    cx, 2
        mov    dword ptr [eax], edi
        mov    edi, dword ptr [ebx + 8]
        mov    word ptr [edx + esi + 2], di
        jb     SHORT loc_1_9262
loc_1_92AC:
        xor    eax, eax
        jmp    NEAR PTR loc_1_93C3
loc_1_92B3:
        mov    edi, 0ah
loc_1_92B8:
        xor    si, si
        test   bx, bx
        je     NEAR PTR loc_1_93BA
loc_1_92C4:
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax + 3], 8
        movzx  eax, si
        push   dword ptr [ebp + eax*4 - 14h]
        je     NEAR PTR loc_1_939E
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_6288
        add    esp, 8
        jmp    NEAR PTR loc_1_93AF
loc_1_92EA:
        mov    edi, 19h
        jmp    SHORT loc_1_92B8
loc_1_92F1:
        mov    edi, 3
        jmp    SHORT loc_1_92B8
loc_1_92F8:
        mov    eax, 20h
        jmp    NEAR PTR loc_1_93C3
loc_1_9302:
        xor    bx, bx
loc_1_9305:
        movzx  eax, bx
        lea    edx, [esi + eax*4]
        cmp    byte ptr [edx + 4], 0
        je     NEAR PTR loc_1_925F
        db  0BFh, 000h, 000h, 000h, 000h
        xor    eax, eax
        mov    ecx, 9
        rep stosd
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  066h, 0C7h, 005h, 002h, 000h, 000h, 000h, 00Ch, 000h
        mov    ax, word ptr [edx + 2]
        db  066h, 0A3h, 004h, 000h, 000h, 000h
        movzx  ax, byte ptr [edx + 4]
        add    ax, word ptr [edx + 2]
        dec    ax
        db  066h, 0A3h, 006h, 000h, 000h, 000h
        mov    cl, byte ptr [esi + 0ah]
        db  088h, 00Dh, 00Bh, 000h, 000h, 000h
        movzx  eax, byte ptr [edx + 5]
        or     eax, 10000000h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp + 8]
        call   sub_1_8E41
        add    esp, 10h
        mov    edi, eax
        test   edi, edi
        jne    NEAR PTR loc_1_92B8
        movzx  edx, bx
        inc    bx
        mov    eax, dword ptr [ebp + 8]
        cmp    bx, 2
        mov    ecx, dword ptr [eax + 1ch]
        mov    dword ptr [ebp + edx*4 - 14h], ecx
        jb     NEAR PTR loc_1_9305
        jmp    NEAR PTR loc_1_925F
loc_1_939E:
        call   sub_1_62B7
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_93AF
        mov    edi, 19h
loc_1_93AF:
        inc    si
        cmp    si, bx
        jb     NEAR PTR loc_1_92C4
loc_1_93BA:
        mov    eax, edi
        jmp    SHORT loc_1_93C3
loc_1_93BE:
        mov    eax, 1ch
loc_1_93C3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_93CA:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        db  08Bh, 00Dh, 00Ah, 000h, 000h, 000h
        sub    esp, 8
        cmp    byte ptr [ecx + 4], 0
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 18h]
        jne    SHORT loc_1_93F0
        mov    eax, 1ch
        jmp    NEAR PTR loc_1_9547
loc_1_93F0:
        test   byte ptr [esi + 0bh], 11h
        je     SHORT loc_1_9400
        mov    eax, 1dh
        jmp    NEAR PTR loc_1_9547
loc_1_9400:
        xor    ax, ax
        xor    ecx, ecx
loc_1_9405:
        movzx  edx, ax
        inc    ax
        mov    dword ptr [ebp + edx*4 - 8], ecx
        cmp    ax, 2
        jb     SHORT loc_1_9405
        xor    dx, dx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 1ch]
loc_1_941D:
        movzx  eax, dx
        shl    eax, 2
        db  08Bh, 03Dh, 00Ah, 000h, 000h, 000h
        add    edi, eax
        cmp    byte ptr [edi + 4], 0
        jne    SHORT loc_1_9448
        test   ecx, ecx
        je     SHORT loc_1_947A
loc_1_9435:
        cmp    dword ptr [ecx + 14h], esi
        jne    SHORT loc_1_9440
        cmp    byte ptr [ecx + 1ah], 1
        je     SHORT loc_1_94AD
loc_1_9440:
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    SHORT loc_1_9435
        jmp    SHORT loc_1_947A
loc_1_9448:
        test   ecx, ecx
        je     SHORT loc_1_947A
        lea    eax, [ebp + eax - 8]
loc_1_9450:
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_947A
        cmp    dword ptr [ecx + 14h], esi
        jne    SHORT loc_1_9474
        cmp    byte ptr [ecx + 1ah], 1
        jne    SHORT loc_1_9474
        movzx  ebx, word ptr [edi + 2]
        cmp    ebx, dword ptr [ecx + 8]
        jne    SHORT loc_1_9474
        movzx  ebx, byte ptr [edi + 4]
        cmp    ebx, dword ptr [ecx + 0ch]
        jne    SHORT loc_1_9474
        mov    dword ptr [eax], ecx
loc_1_9474:
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    SHORT loc_1_9450
loc_1_947A:
        inc    dx
        cmp    dx, 2
        jb     SHORT loc_1_941D
        xor    dx, dx
loc_1_9485:
        movzx  eax, dx
        lea    edi, [eax*4]
        mov    eax, dword ptr [ebp + eax*4 - 8]
        test   eax, eax
        jne    SHORT loc_1_94B7
        db  0A1h, 00Ah, 000h, 000h, 000h
        cmp    byte ptr [edi + eax + 4], 0
        je     SHORT loc_1_94CE
        mov    eax, 1ch
        jmp    NEAR PTR loc_1_9547
loc_1_94AD:
        mov    eax, 1ch
        jmp    NEAR PTR loc_1_9547
loc_1_94B7:
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 10h], ecx
        jne    SHORT loc_1_94FC
        db  08Bh, 00Dh, 00Ah, 000h, 000h, 000h
        mov    al, byte ptr [eax + 19h]
        cmp    byte ptr [edi + ecx + 5], al
        jne    SHORT loc_1_9503
loc_1_94CE:
        inc    dx
        cmp    dx, 2
        jb     SHORT loc_1_9485
        xor    di, di
        xor    ebx, ebx
loc_1_94DB:
        movzx  eax, di
        mov    eax, dword ptr [ebp + eax*4 - 8]
        test   eax, eax
        je     SHORT loc_1_9545
        test   byte ptr [esi + 0bh], 8
        push   eax
        je     SHORT loc_1_950A
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_6288
        add    esp, 8
        jmp    SHORT loc_1_951B
loc_1_94FC:
        mov    eax, 19h
        jmp    SHORT loc_1_9547
loc_1_9503:
        mov    eax, 2
        jmp    SHORT loc_1_9547
loc_1_950A:
        call   sub_1_62B7
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_951B
        mov    ebx, 19h
loc_1_951B:
        test   ebx, ebx
        jne    SHORT loc_1_953D
        mov    ecx, dword ptr [esi + 8]
        mov    eax, ecx
        and    eax, 400000h
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 200000h
        sub    eax, 400001h
        and    eax, ecx
        mov    dword ptr [esi + 8], eax
loc_1_953D:
        inc    di
        cmp    di, 2
        jb     SHORT loc_1_94DB
loc_1_9545:
        mov    eax, ebx
loc_1_9547:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_954E:
        mov    ecx, dword ptr [esp + 4]
        mov    al, byte ptr [ecx + 13h]
        db  00Ah, 005h, 000h, 000h, 000h, 000h
        test   byte ptr [ecx + 17h], 2
        je     SHORT loc_1_9569
        test   al, 71h
        je     SHORT loc_1_9567
        or     al, 10h
loc_1_9567:
        and    al, 9eh
loc_1_9569:
        db  022h, 005h, 000h, 000h, 000h, 000h
        or     al, 80h
        ret
sub_1_9572:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushfd
        pop    dword ptr [ebp - 4]
        cli
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 13h], 0
        mov    ecx, dword ptr [eax + 20h]
        test   ecx, ecx
        je     SHORT loc_1_9598
loc_1_958B:
        mov    dl, byte ptr [ecx + 10h]
        or     byte ptr [eax + 13h], dl
        mov    ecx, dword ptr [ecx + 4]
        test   ecx, ecx
        jne    SHORT loc_1_958B
loc_1_9598:
        push   dword ptr [ebp - 4]
        popfd
        mov    esp, ebp
        pop    ebp
        ret
sub_1_95A0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    ecx, esi
        call   sub_1_2967
        mov    di, ax
        lea    ecx, [esi + 0ch]
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [eax + 18h]
        db  089h, 015h, 014h, 000h, 000h, 000h
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [eax + 1ch]
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 08Dh
        db  089h, 015h, 00Ah, 000h, 000h, 000h
        mov    eax, dword ptr [ecx]
        mov    al, byte ptr [eax + 0ch]
        db  0A2h, 000h, 000h, 000h, 000h
        mov    cl, byte ptr [esi + 12h]
        db  088h, 00Dh, 002h, 000h, 000h, 000h
        call   sub_1_2438
        test   eax, eax
        jne    SHORT loc_1_9616
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 08Eh
        test   di, 80h
        je     SHORT loc_1_960A
        push   esi
        call   sub_1_954E
        add    esp, 4
        db  0A2h, 003h, 000h, 000h, 000h
        jmp    SHORT loc_1_9611
loc_1_960A:
        db  0C6h, 005h, 003h, 000h, 000h, 000h, 080h
loc_1_9611:
        call   sub_1_2438
loc_1_9616:
        pop    edi
        pop    esi
        ret
sub_1_9619:
        push   esi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        test   esi, esi
        je     SHORT loc_1_9641
loc_1_9624:
        push   esi
        call   sub_1_9572
        add    esp, 4
        push   esi
        call   sub_1_95A0
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_9643
        mov    esi, dword ptr [esi + 4]
        test   esi, esi
        jne    SHORT loc_1_9624
loc_1_9641:
        xor    eax, eax
loc_1_9643:
        pop    esi
        ret
sub_1_9645:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        test   ecx, ecx
        je     SHORT loc_1_9665
        mov    ax, word ptr [esp + 4]
loc_1_9654:
        cmp    word ptr [ecx + 10h], ax
        je     SHORT loc_1_9661
        mov    ecx, dword ptr [ecx + 4]
        test   ecx, ecx
        jne    SHORT loc_1_9654
loc_1_9661:
        test   ecx, ecx
        jne    SHORT loc_1_966C
loc_1_9665:
        mov    eax, 0bh
        jmp    SHORT loc_1_9674
loc_1_966C:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax], ecx
        xor    eax, eax
loc_1_9674:
        ret
sub_1_9675:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_269E
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_96B8
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 10h]
        test   ecx, ecx
        je     SHORT loc_1_96AA
        mov    al, byte ptr [ebp + 0ch]
loc_1_969B:
        cmp    byte ptr [ecx + 12h], al
        je     SHORT loc_1_96A6
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    SHORT loc_1_969B
loc_1_96A6:
        test   ecx, ecx
        jne    SHORT loc_1_96B1
loc_1_96AA:
        mov    eax, 0bh
        jmp    SHORT loc_1_96B8
loc_1_96B1:
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], ecx
        xor    eax, eax
loc_1_96B8:
        mov    esp, ebp
        pop    ebp
        ret
loc_1_96BC:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        mov    edi, dword ptr [esp + 14h]
        push   ebp
        push   edi
        push   esi
        call   sub_1_2635
        add    esp, 8
        mov    ebx, eax
        db  066h, 083h, 03Dh, 000h, 000h, 000h, 000h, 022h
        jne    NEAR PTR loc_1_976D
        test   ebx, ebx
        je     SHORT loc_1_96F0
        mov    eax, 0bh
        jmp    NEAR PTR loc_1_97B3
loc_1_96F0:
        push   12h
        call   sub_1_2540
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_1_970A
        mov    eax, 20h
        jmp    NEAR PTR loc_1_97B3
loc_1_970A:
        mov    dword ptr [ebx + 8], esi
        mov    dword ptr [ebx + 0ch], edi
        db  0A1h, 00Ah, 000h, 000h, 000h
        mov    ax, word ptr [eax + 2]
        mov    word ptr [ebx + 10h], ax
        mov    cl, byte ptr [edi + 13h]
        movzx  ebp, al
        movzx  edx, cl
        not    edx
        test   ebp, edx
        je     SHORT loc_1_974D
        or     cl, al
        push   edi
        mov    byte ptr [edi + 13h], cl
        call   sub_1_95A0
        add    esp, 4
        mov    ebp, eax
        test   ebp, ebp
        je     SHORT loc_1_974D
        push   ebx
        call   sub_1_254F
        add    esp, 4
        mov    eax, ebp
        jmp    SHORT loc_1_97B3
loc_1_974D:
        push   0
        add    esi, 18h
        push   ebx
        add    edi, 20h
        push   esi
        call   sub_1_2D7C
        add    esp, 0ch
        push   4
        push   ebx
        push   edi
        call   sub_1_2D7C
        add    esp, 0ch
        jmp    SHORT loc_1_97B1
loc_1_976D:
        test   ebx, ebx
        jne    SHORT loc_1_9778
        mov    eax, 0bh
        jmp    SHORT loc_1_97B3
loc_1_9778:
        push   0
        add    esi, 18h
        push   ebx
        push   esi
        call   sub_1_2E0B
        add    esp, 0ch
        lea    eax, [edi + 20h]
        push   4
        push   ebx
        push   eax
        call   sub_1_2E0B
        add    esp, 0ch
        push   ebx
        call   sub_1_254F
        add    esp, 4
        push   edi
        call   sub_1_9572
        add    esp, 4
        push   edi
        call   sub_1_95A0
        add    esp, 4
loc_1_97B1:
        xor    eax, eax
loc_1_97B3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_97B8:
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [ecx + 0ch]
        mov    dl, byte ptr [eax + 0fh]
        db  0A1h, 00Ah, 000h, 000h, 000h
        mov    byte ptr [eax + 2], dl
        db  0A1h, 00Ah, 000h, 000h, 000h
        mov    cl, byte ptr [ecx + 12h]
        mov    byte ptr [eax + 3], cl
        xor    eax, eax
        ret
loc_1_97D8:
        sub    esp, 4
        db  08Bh, 00Dh, 00Ah, 000h, 000h, 000h
        lea    eax, [esp]
        mov    dl, byte ptr [ecx + 3]
        push   eax
        push   edx
        mov    al, byte ptr [ecx + 2]
        push   eax
        call   sub_1_9675
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_980B
        mov    ecx, dword ptr [esp]
        db  08Bh, 015h, 00Ah, 000h, 000h, 000h
        mov    cx, word ptr [ecx + 10h]
        mov    word ptr [edx], cx
loc_1_980B:
        add    esp, 4
        ret
loc_1_980F:
        push   esi
        db  0A1h, 00Ah, 000h, 000h, 000h
        cmp    word ptr [eax + 2], 0
        je     SHORT loc_1_9823
        mov    eax, 2
        jmp    SHORT loc_1_9874
loc_1_9823:
        push   0ah
        call   sub_1_2540
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_1_983A
        mov    eax, 20h
        jmp    SHORT loc_1_9874
loc_1_983A:
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 8]
        mov    word ptr [esi + 8], 10h
        push   esi
        mov    dword ptr [esi], eax
        push   32h
        mov    dword ptr [esi + 4], ecx
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_2566
        add    esp, 0ch
        test   eax, eax
        mov    eax, 0
        jne    SHORT loc_1_9874
        push   esi
        call   sub_1_254F
        add    esp, 4
        mov    eax, 19h
loc_1_9874:
        pop    esi
        ret
loc_1_9876:
        mov    eax, 15h
        ret
loc_1_987C:
        ret
        db  0CCh, 0CCh, 0CCh
sub_1_9880:
        db  0CDh, 020h, 00Fh, 080h, 017h, 000h
        int3
        int3
sub_1_9888:
        db  0CDh, 020h, 00Eh, 080h, 017h, 000h
        int3
        int3
_LTEXT ends
        end
