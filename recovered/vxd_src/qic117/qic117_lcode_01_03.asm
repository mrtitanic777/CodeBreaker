; qic117_lcode_01_03.asm -- QIC117 object 1 segment _LTEXT  (part 3/3)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_9069:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_1_653B
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_9210
        mov    si, word ptr [ebp + 0ch]
        and    si, 1
        jne    SHORT loc_1_90A0
        xor    bx, bx
        push   0bh
        push   edi
        call   sub_1_7237
        add    esp, 8
        jmp    SHORT loc_1_90AF
loc_1_90A0:
        mov    bx, 1
        push   0ch
        push   edi
        call   sub_1_7237
        add    esp, 8
loc_1_90AF:
        test   eax, eax
        jne    NEAR PTR loc_1_9210
        cmp    word ptr [edi + 16h], 40h
        jne    SHORT loc_1_90EC
        cmp    byte ptr [edi + 19h], 1
        jne    SHORT loc_1_90EC
        push   0
        push   0c8h
        push   dword ptr [edi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        push   48h
        push   edi
        call   sub_1_AAC3
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_9210
loc_1_90EC:
        push   0
        push   dword ptr [edi + 6ch]
        push   edi
        call   sub_1_7191
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_9210
        cmp    word ptr [edi + 16h], 40h
        jne    NEAR PTR loc_1_91C4
        cmp    byte ptr [edi + 19h], 1
        jne    NEAR PTR loc_1_91C4
        push   0
        push   109h
        push   dword ptr [edi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        lea    eax, [ebp - 1]
        push   0
        push   8
        push   eax
        push   2ch
        push   edi
        call   sub_1_740C
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_1_9210
        test   byte ptr [ebp - 1], 40h
        jne    SHORT loc_1_91C4
        test   bx, bx
        jne    SHORT loc_1_91A6
        push   5dh
        push   edi
        call   sub_1_AAC3
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_9210
        push   0
        lea    eax, [ebp - 1]
        push   8
        push   eax
        push   2ch
        push   edi
        call   sub_1_740C
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_1_9210
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_1_91C4
        push   68h
        push   edi
        call   sub_1_AAC3
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_9210
        push   23h
        push   edi
        call   sub_1_A9CF
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_91C4
        jmp    SHORT loc_1_9210
loc_1_91A6:
        push   68h
        push   edi
        call   sub_1_AAC3
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_9210
        push   29h
        push   edi
        call   sub_1_A9CF
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_9210
loc_1_91C4:
        test   si, si
        jne    SHORT loc_1_91E8
        cmp    byte ptr [edi + 0b5h], 0
        jne    SHORT loc_1_9207
        push   1
        push   1102bh
        push   1168h
        call   sub_1_1CB6
        add    esp, 0ch
        jmp    SHORT loc_1_9210
loc_1_91E8:
        cmp    byte ptr [edi + 0b6h], 0
        jne    SHORT loc_1_9207
        push   2
        push   1102bh
        push   1168h
        call   sub_1_1CB6
        add    esp, 0ch
        jmp    SHORT loc_1_9210
loc_1_9207:
        mov    dword ptr [edi + 34h], 0
        xor    eax, eax
loc_1_9210:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_9217:
        xor    al, al
        ret
sub_1_921A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 0
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_6E6A
        add    esp, 4
        push   esi
        call   sub_1_653B
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    NEAR PTR loc_1_936B
        push   esi
        call   sub_1_ADDD
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    NEAR PTR loc_1_936B
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp + 0ch]
        push   esi
        call   sub_1_AD34
        add    esp, 0ch
        mov    edi, eax
        test   edi, edi
        jne    NEAR PTR loc_1_936B
        movzx  eax, byte ptr [esi + 1ah]
        cmp    eax, 2
        jl     SHORT loc_1_92EC
        cmp    eax, 3
        jle    SHORT loc_1_928F
        cmp    eax, 4
        jl     SHORT loc_1_92EC
        cmp    eax, 5
        jle    SHORT loc_1_92C3
        jmp    SHORT loc_1_92EC
loc_1_928F:
        cmp    byte ptr [esi + 63h], 6
        jne    SHORT loc_1_9299
        push   1
        jmp    SHORT loc_1_92D8
loc_1_9299:
        push   dword ptr [ebp - 4]
        push   esi
        call   sub_1_9E52
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    NEAR PTR loc_1_936B
        cmp    byte ptr [esi + 63h], 1
        jne    SHORT loc_1_92EC
        push   dword ptr [esi + 7ch]
        push   esi
        call   sub_1_AB6D
        add    esp, 8
        jmp    SHORT loc_1_92EA
loc_1_92C3:
        movzx  eax, byte ptr [esi + 63h]
        cmp    eax, 1
        jl     SHORT loc_1_92EC
        cmp    eax, 2
        jle    SHORT loc_1_92D6
        cmp    eax, 4
        jne    SHORT loc_1_92EC
loc_1_92D6:
        push   2
loc_1_92D8:
        push   11015h
        push   1160h
        call   sub_1_1CB6
        add    esp, 0ch
loc_1_92EA:
        mov    edi, eax
loc_1_92EC:
        test   edi, edi
        jne    SHORT loc_1_936B
        push   esi
        call   sub_1_ACD6
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_936B
        push   0
        push   esi
        call   sub_1_6CF2
        add    esp, 8
        push   dword ptr [ebp - 4]
        push   esi
        call   sub_1_9E52
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_936B
        cmp    byte ptr [esi + 63h], 1
        jne    SHORT loc_1_9330
        push   dword ptr [esi + 7ch]
        push   esi
        call   sub_1_AB6D
        add    esp, 8
        mov    edi, eax
loc_1_9330:
        test   edi, edi
        jne    SHORT loc_1_936B
        push   esi
        call   sub_1_9DFA
        add    esp, 4
        mov    al, 8
loc_1_933F:
        test   byte ptr [esi + 7], al
        je     SHORT loc_1_9354
        push   eax
        push   esi
        call   sub_1_82BE
        add    esp, 8
        mov    edi, eax
        xor    al, al
        jmp    SHORT loc_1_9356
loc_1_9354:
        shr    al, 1
loc_1_9356:
        test   al, al
        jne    SHORT loc_1_933F
        test   edi, edi
        jne    SHORT loc_1_936B
        push   1
        push   esi
        call   sub_1_6CF2
        add    esp, 8
        mov    edi, eax
loc_1_936B:
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_9373:
        push   esi
        xor    eax, eax
        cmp    word ptr [esp + 0ch], 110bh
        push   edi
        jne    NEAR PTR loc_1_9405
        mov    edi, dword ptr [esp + 0ch]
        cmp    byte ptr [edi + 0b4h], 1
        jne    SHORT loc_1_93C0
        push   edi
        call   sub_1_66A9
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_1_93B3
        push   esi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 115ch
        je     SHORT loc_1_93B3
        mov    eax, esi
        jmp    SHORT loc_1_9405
loc_1_93B3:
        push   edi
        call   sub_1_6660
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_9405
loc_1_93C0:
        push   0dh
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_9405
        push   0
        push   3
        push   dword ptr [edi + 110h]
        call   sub_1_5BDA
        mov    ax, word ptr [edi + 38h]
        add    esp, 0ch
        add    al, 2
        push   eax
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_9405
        push   0
        push   2710h
        push   edi
        call   sub_1_7191
        add    esp, 0ch
loc_1_9405:
        pop    edi
        pop    esi
        ret
sub_1_9408:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        mov    ebx, dword ptr [esp + 10h]
        push   ebp
        mov    ebp, dword ptr [esp + 1ch]
        push   ebx
        mov    dword ptr [ebp], 0
        inc    word ptr [esi + 38h]
        call   sub_1_6E6A
        add    esp, 4
        push   ebx
        call   sub_1_653B
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_94B6
        mov    ax, word ptr [ebx + 0ach]
        push   eax
        push   ebx
        call   sub_1_A934
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_94B4
        cmp    byte ptr [ebx + 0b5h], 0
        jne    SHORT loc_1_9462
        cmp    byte ptr [ebx + 0b6h], 0
        je     SHORT loc_1_9468
loc_1_9462:
        mov    eax, dword ptr [ebx + 7ch]
        mov    dword ptr [ebx + 34h], eax
loc_1_9468:
        dec    word ptr [ebx + 0a0h]
        jne    SHORT loc_1_94B4
        mov    ax, word ptr [esi + 18h]
        push   eax
        push   ebx
        call   sub_1_9373
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_94B4
        push   1
        push   11035h
        push   1153h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    edx, 0ffffffffh
        mov    dword ptr [ebp], eax
        mov    al, byte ptr [ebx + 0afh]
        sub    al, byte ptr [ebx + 0b3h]
        mov    cl, al
        shl    edx, cl
        mov    dword ptr [esi + 2ch], edx
loc_1_94B4:
        mov    eax, edi
loc_1_94B6:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_94BB:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 1ch]
        mov    ebp, dword ptr [esp + 14h]
        push   7
        lea    ebx, [ebp + 0feh]
        push   ebx
        mov    dword ptr [edi], 0
        push   ebp
        call   sub_1_5FD8
        add    esp, 0ch
        mov    esi, eax
        push   esi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 115dh
        jne    SHORT loc_1_9500
        push   ebp
        call   sub_1_6660
        add    esp, 4
        mov    eax, esi
        jmp    NEAR PTR loc_1_967D
loc_1_9500:
        push   esi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 1163h
        jne    SHORT loc_1_9583
        push   ebp
        call   sub_1_6E6A
        add    esp, 4
        push   ebp
        call   sub_1_66A9
        add    esp, 4
        push   ebp
        call   sub_1_6660
        add    esp, 4
        cmp    word ptr [ebp + 0a0h], 0
        jne    SHORT loc_1_957C
        mov    esi, dword ptr [esp + 18h]
        mov    ax, word ptr [esi + 18h]
        push   eax
        push   ebp
        call   sub_1_9373
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_967D
        push   1
        push   11034h
        push   1151h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    edx, 0ffffffffh
        mov    dword ptr [edi], eax
        mov    al, byte ptr [ebp + 0afh]
        sub    al, byte ptr [ebp + 0b3h]
        mov    cl, al
        shl    edx, cl
        mov    dword ptr [esi + 2ch], edx
loc_1_957C:
        xor    eax, eax
        jmp    NEAR PTR loc_1_967D
loc_1_9583:
        mov    al, byte ptr [ebp + 0ffh]
        test   al, al
        jne    SHORT loc_1_95CF
        cmp    byte ptr [ebp + 100h], 0
        jne    SHORT loc_1_95CF
        mov    byte ptr [ebp + 0b2h], 1
        mov    eax, dword ptr [ebp + 0a8h]
        mov    dword ptr [ebp + 34h], eax
        mov    ax, word ptr [ebp + 9ch]
        mov    word ptr [ebp + 9ch], 0
        add    byte ptr [ebp + 0afh], al
        movzx  eax, ax
        shl    eax, 0ah
        add    dword ptr [ebp + 90h], eax
        xor    eax, eax
        jmp    NEAR PTR loc_1_967D
loc_1_95CF:
        mov    esi, dword ptr [esp + 18h]
        cmp    word ptr [esi + 18h], 110ah
        jne    SHORT loc_1_9605
        test   byte ptr [ebp + 100h], 40h
        je     SHORT loc_1_9605
        mov    dword ptr [esi + 2ch], 0ffffffffh
        push   1
        push   11034h
        push   1155h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    dword ptr [edi], eax
        xor    eax, eax
        jmp    SHORT loc_1_967D
loc_1_9605:
        test   byte ptr [ebp + 100h], 10h
        jne    SHORT loc_1_9612
        test   al, 15h
        je     SHORT loc_1_9641
loc_1_9612:
        inc    word ptr [esi + 36h]
        test   byte ptr [ebp + 0ffh], 10h
        je     SHORT loc_1_9623
        inc    word ptr [esi + 34h]
loc_1_9623:
        cmp    byte ptr [ebp + 0b2h], 1
        jne    SHORT loc_1_9641
        mov    byte ptr [ebp + 0b2h], 0
        mov    eax, dword ptr [ebp + 0a8h]
        inc    eax
        mov    dword ptr [ebp + 34h], eax
        xor    eax, eax
        jmp    SHORT loc_1_967D
loc_1_9641:
        push   dword ptr [ebp + 110h]
        mov    eax, dword ptr [ebp + 0a8h]
        mov    dword ptr [ebp + 34h], eax
        call   sub_1_384C
        add    esp, 4
        test   al, al
        je     SHORT loc_1_9671
        push   1bh
        call   sub_1_2C87
        add    esp, 4
        mov    dword ptr [edi], eax
        test   eax, eax
        mov    eax, 0
        jne    SHORT loc_1_967D
loc_1_9671:
        push   edi
        push   ebx
        push   esi
        push   ebp
        call   sub_1_AE46
        add    esp, 10h
loc_1_967D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_9682:
        push   esi
        xor    eax, eax
        mov    esi, dword ptr [esp + 8]
        cmp    byte ptr [esi + 0b4h], al
        jne    SHORT loc_1_96A7
        push   0ah
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_96A7
        mov    byte ptr [esi + 0b4h], 1
loc_1_96A7:
        pop    esi
        ret
sub_1_96A9:
        sub    esp, 4
        push   ebx
        push   esi
        mov    si, 0ah
        push   edi
        push   ebp
        xor    ebx, ebx
        mov    edi, dword ptr [esp + 18h]
        mov    byte ptr [esp + 13h], bl
        mov    byte ptr [esp + 12h], bl
loc_1_96C2:
        cmp    byte ptr [edi + 11fh], 0
        je     SHORT loc_1_96DD
        lea    eax, [esp + 12h]
        push   eax
        push   edi
        call   sub_1_B438
        add    esp, 8
        mov    ebx, eax
        jmp    SHORT loc_1_9739
loc_1_96DD:
        cmp    dword ptr [edi + 0a8h], 0
        jne    SHORT loc_1_9701
        mov    ax, word ptr [edi + 0ach]
        push   eax
        push   edi
        call   sub_1_9069
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    NEAR PTR loc_1_9874
loc_1_9701:
        mov    ax, word ptr [edi + 0ach]
        cmp    word ptr [edi + 38h], ax
        je     SHORT loc_1_9739
        push   eax
        push   edi
        call   sub_1_A934
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    NEAR PTR loc_1_9878
        cmp    byte ptr [edi + 0b5h], 0
        jne    SHORT loc_1_9739
        cmp    byte ptr [edi + 0b6h], 0
        jne    SHORT loc_1_9739
        mov    byte ptr [esp + 12h], 1
loc_1_9739:
        cmp    dword ptr [edi + 0a8h], 0
        je     NEAR PTR loc_1_987C
        cmp    byte ptr [esp + 12h], 1
        jne    SHORT loc_1_9758
        push   edi
        call   sub_1_AFE1
        add    esp, 4
        mov    ebx, eax
loc_1_9758:
        test   ebx, ebx
        jne    SHORT loc_1_9776
        push   edi
        call   sub_1_B13E
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_1_9776
        push   edi
        call   sub_1_AFE1
        add    esp, 4
        mov    ebx, eax
loc_1_9776:
        dec    si
        push   ebx
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 1168h
        je     SHORT loc_1_9790
        test   si, si
        jne    NEAR PTR loc_1_981E
loc_1_9790:
        cmp    byte ptr [esp + 13h], 0
        jne    NEAR PTR loc_1_981E
        push   edi
        call   sub_1_653B
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_98B7
        cmp    byte ptr [edi + 19h], 1
        jne    SHORT loc_1_9800
        cmp    word ptr [edi + 16h], 50h
        jb     SHORT loc_1_9800
        push   2dh
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_98B7
        push   1
        push   0a3930h
        push   edi
        call   sub_1_7191
        add    esp, 0ch
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_1_9800
        push   esi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 1164h
        jne    NEAR PTR loc_1_9880
        mov    byte ptr [edi + 120h], 0
        mov    byte ptr [edi + 3ah], 0
loc_1_9800:
        push   0
        push   edi
        call   sub_1_7766
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_98B7
        mov    si, 0ah
        xor    ebx, ebx
        mov    byte ptr [esp + 13h], 1
loc_1_981E:
        test   ebx, ebx
        jne    SHORT loc_1_9884
        push   dword ptr [edi + 110h]
        mov    ebp, dword ptr [edi + 0a8h]
        sub    ebp, dword ptr [edi + 34h]
        call   sub_1_60D4
        dec    ebp
        add    esp, 4
        cmp    al, 0ffh
        jne    SHORT loc_1_9888
        test   ebp, ebp
        jl     SHORT loc_1_9847
        cmp    ebp, 0ah
        jle    SHORT loc_1_9850
loc_1_9847:
        test   si, si
        jg     NEAR PTR loc_1_96C2
loc_1_9850:
        test   si, si
        jne    SHORT loc_1_989E
        test   ebp, ebp
        jl     SHORT loc_1_985E
        cmp    ebp, 0ah
        jle    SHORT loc_1_989E
loc_1_985E:
        push   1
        push   1103fh
        push   1168h
        call   sub_1_1CB6
        add    esp, 0ch
        jmp    SHORT loc_1_98B7
loc_1_9874:
        mov    eax, ebx
        jmp    SHORT loc_1_98B7
loc_1_9878:
        mov    eax, ebx
        jmp    SHORT loc_1_98B7
loc_1_987C:
        xor    eax, eax
        jmp    SHORT loc_1_98B7
loc_1_9880:
        mov    eax, esi
        jmp    SHORT loc_1_98B7
loc_1_9884:
        mov    eax, ebx
        jmp    SHORT loc_1_98B7
loc_1_9888:
        push   1
        push   1103fh
        push   1150h
        call   sub_1_1CB6
        add    esp, 0ch
        jmp    SHORT loc_1_98B7
loc_1_989E:
        push   edi
        call   sub_1_AFE1
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_98B7
        mov    ecx, dword ptr [edi + 0a8h]
        dec    ecx
        cmp    ecx, dword ptr [edi + 34h]
        jg     SHORT loc_1_989E
loc_1_98B7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_98BF:
        mov    edx, dword ptr [esp + 4]
        mov    word ptr [edx + 9ch], 0
        test   byte ptr [edx + 98h], 1
        je     SHORT loc_1_98ED
loc_1_98D5:
        mov    eax, dword ptr [edx + 98h]
        inc    byte ptr [edx + 0afh]
        shr    eax, 1
        mov    dword ptr [edx + 98h], eax
        test   al, 1
        jne    SHORT loc_1_98D5
loc_1_98ED:
        mov    cx, word ptr [edx + 9eh]
        mov    eax, dword ptr [edx + 98h]
        shr    eax, 1
        dec    cx
        mov    dword ptr [edx + 98h], eax
        inc    word ptr [edx + 9ch]
        test   al, 1
        mov    word ptr [edx + 9eh], cx
        jne    SHORT loc_1_991B
        test   cx, cx
        jne    SHORT loc_1_98ED
loc_1_991B:
        ret
sub_1_991C:
        push   esi
        xor    eax, eax
        xor    esi, esi
        mov    edx, dword ptr [esp + 8]
        mov    cl, byte ptr [edx + 86h]
        cmp    cl, 3
        jne    SHORT loc_1_9935
        mov    esi, 2
loc_1_9935:
        cmp    cl, 4
        jne    SHORT loc_1_993F
        mov    esi, 4
loc_1_993F:
        test   esi, esi
        je     SHORT loc_1_9992
        movzx  ecx, word ptr [edx + 0ach]
        sub    ecx, 5
        cmp    ecx, 16h
        ja     SHORT loc_1_9992
        db  00Fh, 0B6h, 089h, 0C8h, 0A9h, 040h, 000h
        db  0FFh, 024h, 08Dh, 094h, 0A9h, 040h, 000h
loc_1_9960:
        mov    ecx, dword ptr [edx + 0a8h]
        cmp    esi, ecx
        ja     SHORT loc_1_9973
        mov    edx, dword ptr [edx + 7ch]
        sub    edx, esi
        cmp    edx, ecx
        ja     SHORT loc_1_9992
loc_1_9973:
        mov    eax, dword ptr [esp + 0ch]
        push   1
        push   11054h
        push   1153h
        mov    dword ptr [eax + 2ch], 0ffffffffh
        call   sub_1_1CB6
        add    esp, 0ch
loc_1_9992:
        pop    esi
        ret
        db  060h, 0A9h, 040h, 000h, 060h, 0A9h, 040h, 000h, 060h, 0A9h, 040h, 000h, 060h, 0A9h, 040h, 000h
        db  060h, 0A9h, 040h, 000h, 060h, 0A9h, 040h, 000h, 060h, 0A9h, 040h, 000h, 060h, 0A9h, 040h, 000h
        db  060h, 0A9h, 040h, 000h, 060h, 0A9h, 040h, 000h, 060h, 0A9h, 040h, 000h, 060h, 0A9h, 040h, 000h
        db  092h, 0A9h, 040h, 000h, 000h, 00Ch, 001h, 00Ch, 002h, 00Ch, 003h, 00Ch, 004h, 00Ch, 005h, 00Ch
        db  006h, 00Ch, 007h, 00Ch, 008h, 00Ch, 009h, 00Ch, 00Ah, 00Ch, 00Bh
sub_1_99DF:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        mov    ebx, dword ptr [esp + 14h]
        cmp    dword ptr [esi + 58h], ebx
        ja     SHORT loc_1_9A00
        push   1
        push   11002h
        push   1156h
        jmp    NEAR PTR loc_1_9AA3
loc_1_9A00:
        mov    byte ptr [esi + 0b0h], 0
        mov    ecx, dword ptr [esi + 54h]
        test   ecx, ecx
        je     NEAR PTR loc_1_9A97
        mov    eax, ebx
        sub    edx, edx
        div    ecx
        sub    edx, edx
        mov    byte ptr [esi + 0b0h], al
        mov    eax, ebx
        div    ecx
        mov    eax, edx
        and    dl, 7fh
        shr    eax, 7
        inc    dl
        mov    byte ptr [esi + 0aeh], al
        mov    cl, dl
        mov    edi, dword ptr [esi + 5ch]
        mov    byte ptr [esi + 0afh], cl
        mov    byte ptr [esi + 0b3h], cl
        test   edi, edi
        mov    word ptr [esi + 0ach], 0
        je     SHORT loc_1_9A93
        mov    eax, ebx
        sub    edx, edx
        div    edi
        movzx  ecx, cl
        mov    word ptr [esi + 0ach], ax
        sub    edx, edx
        mov    eax, ebx
        dec    ecx
        div    edi
        shr    edx, 5
        and    ecx, 1fh
        add    ecx, dword ptr [esp + 18h]
        mov    dword ptr [esi + 0a8h], edx
        movzx  edx, word ptr [esi + 48h]
        cmp    ecx, edx
        jbe    SHORT loc_1_9A8F
        push   2
        push   11002h
        push   1156h
        jmp    SHORT loc_1_9AA3
loc_1_9A8F:
        xor    eax, eax
        jmp    SHORT loc_1_9AAB
loc_1_9A93:
        push   2
        jmp    SHORT loc_1_9A99
loc_1_9A97:
        push   1
loc_1_9A99:
        push   11002h
        push   116ch
loc_1_9AA3:
        call   sub_1_1CB6
        add    esp, 0ch
loc_1_9AAB:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_9AAF:
        movzx  eax, word ptr [esp + 8]
        cmp    eax, 1109h
        jl     NEAR PTR loc_1_9B43
        cmp    eax, 110ah
        jle    SHORT loc_1_9AE4
        cmp    eax, 110bh
        je     SHORT loc_1_9AF3
        cmp    eax, 110ch
        je     SHORT loc_1_9B15
        cmp    eax, 110dh
        jl     SHORT loc_1_9B43
        cmp    eax, 110eh
        jle    SHORT loc_1_9B2D
        jmp    SHORT loc_1_9B43
loc_1_9AE4:
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 0a6h], 0
        jmp    SHORT loc_1_9B3A
loc_1_9AF3:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, 6
        mov    word ptr [eax + 114h], 0
        mov    word ptr [eax + 0a6h], cx
        mov    word ptr [eax + 0a0h], cx
        jmp    SHORT loc_1_9B43
loc_1_9B15:
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 0a6h], 0
        mov    word ptr [eax + 0a0h], 2
        jmp    SHORT loc_1_9B43
loc_1_9B2D:
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 0a6h], 0ah
loc_1_9B3A:
        mov    word ptr [eax + 0a0h], 3
loc_1_9B43:
        ret
sub_1_9B44:
        mov    ecx, dword ptr [esp + 4]
        movzx  eax, byte ptr [ecx + 1ah]
        cmp    eax, 2
        je     SHORT loc_1_9B69
        cmp    eax, 3
        je     SHORT loc_1_9B7C
        cmp    eax, 4
        je     SHORT loc_1_9BBF
        cmp    eax, 5
        je     NEAR PTR loc_1_9BF2
        jmp    NEAR PTR loc_1_9C2C
loc_1_9B69:
        mov    dword ptr [ecx + 78h], 14h
        movzx  eax, word ptr [ecx + 7ch]
        mov    dword ptr [ecx + 74h], eax
        jmp    NEAR PTR loc_1_9C2C
loc_1_9B7C:
        movzx  eax, byte ptr [ecx + 61h]
        dec    eax
        cmp    eax, 0ch
        ja     SHORT loc_1_9BB6
        db  00Fh, 0B6h, 090h, 04Dh, 0ACh, 040h, 000h
        db  0FFh, 024h, 095h, 02Dh, 0ACh, 040h, 000h
loc_1_9B94:
        mov    dword ptr [ecx + 74h], 64h
        jmp    SHORT loc_1_9BB6
loc_1_9B9D:
        mov    dword ptr [ecx + 74h], 96h
        jmp    SHORT loc_1_9BB6
loc_1_9BA6:
        mov    dword ptr [ecx + 74h], 219h
        jmp    SHORT loc_1_9BB6
loc_1_9BAF:
        mov    dword ptr [ecx + 74h], 0cfh
loc_1_9BB6:
        mov    dword ptr [ecx + 78h], 1ch
        jmp    SHORT loc_1_9C2C
loc_1_9BBF:
        movzx  eax, word ptr [ecx + 7ch]
        mov    dword ptr [ecx + 74h], eax
        movzx  eax, byte ptr [ecx + 62h]
        cmp    eax, 1
        je     SHORT loc_1_9BE0
        cmp    eax, 2
        je     SHORT loc_1_9BE9
        cmp    eax, 3
        je     SHORT loc_1_9C25
        cmp    eax, 4
        je     SHORT loc_1_9C25
        jmp    SHORT loc_1_9C2C
loc_1_9BE0:
        mov    dword ptr [ecx + 78h], 14h
        jmp    SHORT loc_1_9C2C
loc_1_9BE9:
        mov    dword ptr [ecx + 78h], 1ch
        jmp    SHORT loc_1_9C2C
loc_1_9BF2:
        movzx  eax, word ptr [ecx + 7ch]
        mov    dword ptr [ecx + 74h], eax
        movzx  eax, byte ptr [ecx + 62h]
        cmp    eax, 1
        je     SHORT loc_1_9C13
        cmp    eax, 2
        je     SHORT loc_1_9C1C
        cmp    eax, 3
        je     SHORT loc_1_9C25
        cmp    eax, 4
        je     SHORT loc_1_9C25
        jmp    SHORT loc_1_9C2C
loc_1_9C13:
        mov    dword ptr [ecx + 78h], 14h
        jmp    SHORT loc_1_9C2C
loc_1_9C1C:
        mov    dword ptr [ecx + 78h], 1ch
        jmp    SHORT loc_1_9C2C
loc_1_9C25:
        mov    dword ptr [ecx + 78h], 28h
loc_1_9C2C:
        ret
        db  094h, 0ABh, 040h, 000h, 09Dh, 0ABh, 040h, 000h, 0A6h, 0ABh, 040h, 000h, 094h, 0ABh, 040h, 000h
        db  09Dh, 0ABh, 040h, 000h, 0A6h, 0ABh, 040h, 000h, 0AFh, 0ABh, 040h, 000h, 0B6h, 0ABh, 040h, 000h
        db  000h, 001h, 002h, 003h, 004h, 005h, 007h, 007h, 007h, 007h, 007h, 006h, 006h
sub_1_9C5A:
        mov    al, byte ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        mov    byte ptr [ecx + 3eh], al
        movzx  eax, al
        cmp    eax, 1
        je     SHORT loc_1_9C7E
        cmp    eax, 2
        je     SHORT loc_1_9C8C
        cmp    eax, 4
        je     SHORT loc_1_9C9A
        cmp    eax, 8
        je     SHORT loc_1_9CAB
        jmp    SHORT loc_1_9CBA
loc_1_9C7E:
        mov    byte ptr [ecx + 44h], 0
        mov    byte ptr [ecx + 45h], 2
        mov    byte ptr [ecx + 46h], 0ffh
        jmp    SHORT loc_1_9CBA
loc_1_9C8C:
        mov    byte ptr [ecx + 44h], 2
        mov    byte ptr [ecx + 45h], 0
        mov    byte ptr [ecx + 46h], 0efh
        jmp    SHORT loc_1_9CBA
loc_1_9C9A:
        mov    byte ptr [ecx + 46h], 0dfh
        mov    eax, 3
        mov    byte ptr [ecx + 44h], al
        mov    byte ptr [ecx + 45h], al
        jmp    SHORT loc_1_9CBA
loc_1_9CAB:
        mov    byte ptr [ecx + 46h], 0cfh
        mov    eax, 1
        mov    byte ptr [ecx + 44h], al
        mov    byte ptr [ecx + 45h], al
loc_1_9CBA:
        ret
sub_1_9CBB:
        xor    eax, eax
        mov    edx, dword ptr [esp + 4]
        movzx  ecx, byte ptr [edx + 1ah]
        cmp    ecx, 2
        je     SHORT loc_1_9CFA
        cmp    ecx, 3
        je     SHORT loc_1_9D23
        cmp    ecx, 4
        je     NEAR PTR loc_1_9D67
        cmp    ecx, 5
        je     NEAR PTR loc_1_9DAA
        push   5
        push   11055h
        push   116dh
        call   sub_1_1CB6
        add    esp, 0ch
        jmp    NEAR PTR loc_1_9DF9
loc_1_9CFA:
        movzx  ecx, byte ptr [edx + 86h]
        cmp    ecx, 1
        je     NEAR PTR loc_1_9DEB
        push   1
        push   11055h
        push   116dh
        call   sub_1_1CB6
        add    esp, 0ch
        jmp    NEAR PTR loc_1_9DF9
loc_1_9D23:
        movzx  ecx, byte ptr [edx + 86h]
        cmp    ecx, 1
        je     SHORT loc_1_9D51
        cmp    ecx, 2
        je     NEAR PTR loc_1_9DEB
        push   2
        push   11055h
        push   116dh
        call   sub_1_1CB6
        add    esp, 0ch
        jmp    NEAR PTR loc_1_9DF9
loc_1_9D51:
        mov    ecx, 1
        mov    byte ptr [edx + 84h], cl
        mov    byte ptr [edx + 83h], cl
        jmp    NEAR PTR loc_1_9DF9
loc_1_9D67:
        movzx  ecx, byte ptr [edx + 86h]
        cmp    ecx, 1
        jl     SHORT loc_1_9D82
        cmp    ecx, 2
        jle    SHORT loc_1_9DDB
        cmp    ecx, 3
        je     SHORT loc_1_9DEB
        cmp    ecx, 4
        je     SHORT loc_1_9D86
loc_1_9D82:
        push   3
        jmp    SHORT loc_1_9DC7
loc_1_9D86:
        mov    byte ptr [edx + 84h], 1
        mov    byte ptr [edx + 83h], 0
        push   3
        push   11055h
        push   116dh
        call   sub_1_1CB6
        add    esp, 0ch
        jmp    SHORT loc_1_9DF9
loc_1_9DAA:
        movzx  ecx, byte ptr [edx + 86h]
        cmp    ecx, 1
        jl     SHORT loc_1_9DC5
        cmp    ecx, 2
        jle    SHORT loc_1_9DDB
        cmp    ecx, 3
        jl     SHORT loc_1_9DC5
        cmp    ecx, 4
        jle    SHORT loc_1_9DEB
loc_1_9DC5:
        push   4
loc_1_9DC7:
        push   11055h
        push   116dh
        call   sub_1_1CB6
        add    esp, 0ch
        jmp    SHORT loc_1_9DF9
loc_1_9DDB:
        mov    byte ptr [edx + 84h], 0
        mov    byte ptr [edx + 83h], 1
        jmp    SHORT loc_1_9DF9
loc_1_9DEB:
        mov    byte ptr [edx + 84h], 1
        mov    byte ptr [edx + 83h], 0
loc_1_9DF9:
        ret
sub_1_9DFA:
        mov    eax, dword ptr [esp + 4]
        mov    cl, 8
        mov    dl, byte ptr [eax + 7]
        and    dl, byte ptr [eax + 60h]
loc_1_9E06:
        test   dl, cl
        je     SHORT loc_1_9E14
        mov    byte ptr [eax + 8bh], cl
        xor    cl, cl
        jmp    SHORT loc_1_9E16
loc_1_9E14:
        shr    cl, 1
loc_1_9E16:
        test   cl, cl
        jne    SHORT loc_1_9E06
        mov    cl, 1
loc_1_9E1C:
        test   dl, cl
        je     SHORT loc_1_9E2A
        mov    byte ptr [eax + 8ah], cl
        xor    cl, cl
        jmp    SHORT loc_1_9E2C
loc_1_9E2A:
        add    cl, cl
loc_1_9E2C:
        cmp    cl, 8
        ja     SHORT loc_1_9E35
        test   cl, cl
        jne    SHORT loc_1_9E1C
loc_1_9E35:
        mov    byte ptr [eax + 85h], 1
        mov    cl, byte ptr [eax + 8bh]
        cmp    byte ptr [eax + 8ah], cl
        jne    SHORT loc_1_9E51
        mov    byte ptr [eax + 85h], 0
loc_1_9E51:
        ret
sub_1_9E52:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        lea    eax, [ebp - 2]
        push   esi
        push   edi
        push   0
        mov    edi, dword ptr [ebp + 8]
        push   8
        push   eax
        push   21h
        push   edi
        call   sub_1_740C
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_1_9EE0
        push   edi
        call   sub_1_66A9
        add    esp, 4
        lea    eax, [ebp - 1]
        push   0
        push   8
        push   eax
        push   8
        push   edi
        call   sub_1_740C
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_1_A0FD
        test   byte ptr [ebp - 1], 40h
        je     SHORT loc_1_9EC1
        test   byte ptr [ebp - 1], 80h
        lea    esi, [edi + 62h]
        je     SHORT loc_1_9EB6
        lea    ebx, [edi + 63h]
        mov    byte ptr [esi], 2
        mov    byte ptr [ebx], 2
        jmp    NEAR PTR loc_1_9F3A
loc_1_9EB6:
        lea    ebx, [edi + 63h]
        mov    byte ptr [esi], 1
        mov    byte ptr [ebx], 2
        jmp    SHORT loc_1_9F3A
loc_1_9EC1:
        test   byte ptr [ebp - 1], 80h
        lea    esi, [edi + 62h]
        je     SHORT loc_1_9ED5
        lea    ebx, [edi + 63h]
        mov    byte ptr [esi], 2
        mov    byte ptr [ebx], 1
        jmp    SHORT loc_1_9F3A
loc_1_9ED5:
        lea    ebx, [edi + 63h]
        mov    byte ptr [esi], 1
        mov    byte ptr [ebx], 1
        jmp    SHORT loc_1_9F3A
loc_1_9EE0:
        lea    esi, [edi + 62h]
        mov    cl, byte ptr [ebp - 2]
        and    cl, 0fh
        lea    ebx, [edi + 63h]
        mov    dl, byte ptr [ebp - 2]
        mov    byte ptr [esi], cl
        shr    dl, 4
        test   cl, cl
        mov    byte ptr [ebx], dl
        jne    SHORT loc_1_9F3A
        movzx  ecx, byte ptr [edi + 1ah]
        cmp    ecx, 2
        je     SHORT loc_1_9F28
        cmp    ecx, 3
        je     SHORT loc_1_9F2D
        cmp    ecx, 4
        je     SHORT loc_1_9F32
        cmp    ecx, 5
        je     SHORT loc_1_9F37
        push   1
        push   11029h
        push   116bh
        call   sub_1_1CB6
        add    esp, 0ch
        jmp    SHORT loc_1_9F3A
loc_1_9F28:
        mov    byte ptr [esi], 1
        jmp    SHORT loc_1_9F3A
loc_1_9F2D:
        mov    byte ptr [esi], 2
        jmp    SHORT loc_1_9F3A
loc_1_9F32:
        mov    byte ptr [esi], 4
        jmp    SHORT loc_1_9F3A
loc_1_9F37:
        mov    byte ptr [esi], 3
loc_1_9F3A:
        mov    word ptr [edi + 48h], 20h
        movzx  ecx, byte ptr [esi]
        mov    word ptr [edi + 4ah], 4
        mov    word ptr [edi + 4ch], 80h
        mov    word ptr [edi + 4eh], 1
        cmp    ecx, 1
        je     SHORT loc_1_9F91
        cmp    ecx, 2
        je     NEAR PTR loc_1_9FF9
        cmp    ecx, 3
        je     NEAR PTR loc_1_A061
        cmp    ecx, 4
        je     NEAR PTR loc_1_A0A8
        push   2
        push   11029h
        push   116bh
        call   sub_1_1CB6
        add    esp, 0ch
        mov    esi, dword ptr [ebp + 0ch]
        jmp    NEAR PTR loc_1_A0ED
loc_1_9F91:
        mov    byte ptr [edi + 86h], 1
        mov    byte ptr [edi + 60h], 3
        mov    word ptr [edi + 80h], 14h
        mov    esi, dword ptr [ebp + 0ch]
        test   esi, esi
        jne    NEAR PTR loc_1_A0ED
        movzx  ecx, byte ptr [ebx]
        cmp    ecx, 1
        je     SHORT loc_1_9FDB
        cmp    ecx, 2
        je     SHORT loc_1_9FE5
        cmp    ecx, 4
        je     SHORT loc_1_9FEF
        push   1
        push   11029h
        push   116ch
        call   sub_1_1CB6
        add    esp, 0ch
        jmp    NEAR PTR loc_1_A0ED
loc_1_9FDB:
        mov    esi, 8dh
        jmp    NEAR PTR loc_1_A0ED
loc_1_9FE5:
        mov    esi, 66h
        jmp    NEAR PTR loc_1_A0ED
loc_1_9FEF:
        mov    esi, 16dh
        jmp    NEAR PTR loc_1_A0ED
loc_1_9FF9:
        mov    byte ptr [edi + 86h], 2
        mov    byte ptr [edi + 60h], 6
        mov    word ptr [edi + 80h], 1ch
        mov    esi, dword ptr [ebp + 0ch]
        test   esi, esi
        jne    NEAR PTR loc_1_A0ED
        movzx  ecx, byte ptr [ebx]
        cmp    ecx, 1
        je     SHORT loc_1_A043
        cmp    ecx, 2
        je     SHORT loc_1_A04D
        cmp    ecx, 4
        je     SHORT loc_1_A057
        push   2
        push   11029h
        push   116ch
        call   sub_1_1CB6
        add    esp, 0ch
        jmp    NEAR PTR loc_1_A0ED
loc_1_A043:
        mov    esi, 0cfh
        jmp    NEAR PTR loc_1_A0ED
loc_1_A04D:
        mov    esi, 96h
        jmp    NEAR PTR loc_1_A0ED
loc_1_A057:
        mov    esi, 219h
        jmp    NEAR PTR loc_1_A0ED
loc_1_A061:
        mov    byte ptr [edi + 86h], 4
        mov    byte ptr [edi + 60h], 0ch
        movzx  ecx, byte ptr [ebx]
        mov    word ptr [edi + 80h], 28h
        cmp    ecx, 6
        je     SHORT loc_1_A096
        push   4
        push   11029h
        push   116ch
        call   sub_1_1CB6
        add    esp, 0ch
        mov    esi, dword ptr [ebp + 0ch]
        jmp    SHORT loc_1_A0ED
loc_1_A096:
        mov    byte ptr [edi + 61h], 0bh
        mov    esi, dword ptr [ebp + 0ch]
        test   esi, esi
        jne    SHORT loc_1_A0ED
        mov    esi, 1a6h
        jmp    SHORT loc_1_A0ED
loc_1_A0A8:
        mov    byte ptr [edi + 86h], 3
        mov    byte ptr [edi + 60h], 6
        movzx  ecx, byte ptr [ebx]
        mov    word ptr [edi + 80h], 28h
        cmp    ecx, 6
        je     SHORT loc_1_A0DD
        push   3
        push   11029h
        push   116ch
        call   sub_1_1CB6
        add    esp, 0ch
        mov    esi, dword ptr [ebp + 0ch]
        jmp    SHORT loc_1_A0ED
loc_1_A0DD:
        mov    byte ptr [edi + 61h], 9
        mov    esi, dword ptr [ebp + 0ch]
        test   esi, esi
        jne    SHORT loc_1_A0ED
        mov    esi, 12ch
loc_1_A0ED:
        test   eax, eax
        jne    SHORT loc_1_A0FD
        push   0
        push   esi
        push   edi
        call   sub_1_7E26
        add    esp, 0ch
loc_1_A0FD:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_A104:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        push   2dh
        push   edi
        call   sub_1_7237
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_1_A14D
        push   1
        push   0a3930h
        push   edi
        call   sub_1_7191
        add    esp, 0ch
        mov    esi, eax
        push   esi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 1108h
        je     SHORT loc_1_A14B
        push   esi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 1106h
        jne    SHORT loc_1_A14D
loc_1_A14B:
        xor    esi, esi
loc_1_A14D:
        mov    eax, esi
        pop    edi
        pop    esi
        ret
sub_1_A152:
        sub    esp, 4
        push   esi
        push   3
        mov    esi, dword ptr [esp + 10h]
        push   esi
        call   sub_1_6CF2
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_A19F
        lea    eax, [esp + 7]
        push   0
        push   8
        push   eax
        push   17h
        push   esi
        call   sub_1_740C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_1_A19F
        test   byte ptr [esp + 7], 20h
        mov    eax, dword ptr [esp + 10h]
        je     SHORT loc_1_A191
        mov    byte ptr [eax], 1
        jmp    SHORT loc_1_A194
loc_1_A191:
        mov    byte ptr [eax], 0
loc_1_A194:
        push   0
        push   esi
        call   sub_1_6CF2
        add    esp, 8
loc_1_A19F:
        pop    esi
        add    esp, 4
        ret
sub_1_A1A4:
        ret
sub_1_A1A5:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        lea    eax, [ebp - 2]
        push   esi
        push   0
        push   8
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   8
        push   esi
        call   sub_1_740C
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_1_A396
        and    byte ptr [ebp - 2], 18h
        movzx  eax, byte ptr [esi + 1ah]
        shr    byte ptr [ebp - 2], 3
        cmp    eax, 2
        je     SHORT loc_1_A20B
        cmp    eax, 3
        je     SHORT loc_1_A211
        cmp    eax, 4
        je     SHORT loc_1_A217
        cmp    eax, 5
        je     SHORT loc_1_A21D
        mov    bx, word ptr [ebp - 2]
loc_1_A1EF:
        movzx  eax, byte ptr [esi + 1ch]
        cmp    eax, 4
        jl     SHORT loc_1_A207
        cmp    eax, 6
        jle    SHORT loc_1_A223
        cmp    eax, 7
        je     SHORT loc_1_A227
        cmp    eax, 8
        je     SHORT loc_1_A223
loc_1_A207:
        mov    al, 3
        jmp    SHORT loc_1_A271
loc_1_A20B:
        mov    bx, 3
        jmp    SHORT loc_1_A1EF
loc_1_A211:
        mov    bx, 6
        jmp    SHORT loc_1_A1EF
loc_1_A217:
        mov    bx, 6
        jmp    SHORT loc_1_A1EF
loc_1_A21D:
        mov    bx, 0eh
        jmp    SHORT loc_1_A1EF
loc_1_A223:
        mov    al, 7
        jmp    SHORT loc_1_A271
loc_1_A227:
        push   0
        lea    eax, [ebp - 3]
        mov    byte ptr [ebp - 14h], 0
        mov    byte ptr [ebp - 3], 2eh
        push   1
        push   eax
        push   esi
        call   sub_1_5DDF
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_1_A257
        push   10h
        lea    eax, [ebp - 14h]
        push   eax
        push   esi
        call   sub_1_5FD8
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_A264
loc_1_A257:
        push   esi
        call   sub_1_6E6A
        mov    byte ptr [ebp - 14h], 0
        add    esp, 4
loc_1_A264:
        mov    al, byte ptr [ebp - 14h]
        and    al, 80h
        cmp    al, 1
        sbb    al, al
        and    al, 0f8h
        add    al, 0fh
loc_1_A271:
        and    bl, al
        movzx  eax, byte ptr [esi + 19h]
        mov    byte ptr [esi + 7], bl
        cmp    eax, 1
        jl     SHORT loc_1_A296
        cmp    eax, 3
        jle    NEAR PTR loc_1_A330
        cmp    eax, 5
        je     SHORT loc_1_A2D5
        cmp    eax, 7
        je     NEAR PTR loc_1_A330
loc_1_A296:
        movzx  eax, byte ptr [ebp - 2]
        test   eax, eax
        je     SHORT loc_1_A2C6
        cmp    eax, 1
        je     SHORT loc_1_A2CB
        cmp    eax, 2
        je     SHORT loc_1_A2D0
        cmp    eax, 3
        je     SHORT loc_1_A32A
        push   2
        push   11042h
        push   116eh
        call   sub_1_1CB6
        add    esp, 0ch
        jmp    NEAR PTR loc_1_A396
loc_1_A2C6:
        and    bl, 1
        jmp    SHORT loc_1_A32D
loc_1_A2CB:
        and    bl, 8
        jmp    SHORT loc_1_A32D
loc_1_A2D0:
        and    bl, 2
        jmp    SHORT loc_1_A32D
loc_1_A2D5:
        mov    ax, word ptr [esi + 42h]
        test   ax, ax
        je     SHORT loc_1_A2EE
        cmp    byte ptr [esi + 1ah], 3
        jne    SHORT loc_1_A330
        test   ah, 8
        jne    SHORT loc_1_A330
        and    bl, 2
        jmp    SHORT loc_1_A32D
loc_1_A2EE:
        movzx  eax, byte ptr [ebp - 2]
        test   eax, eax
        je     SHORT loc_1_A31B
        cmp    eax, 1
        je     SHORT loc_1_A320
        cmp    eax, 2
        je     SHORT loc_1_A325
        cmp    eax, 3
        je     SHORT loc_1_A32A
        push   1
        push   11042h
        push   116eh
        call   sub_1_1CB6
        add    esp, 0ch
        jmp    SHORT loc_1_A396
loc_1_A31B:
        and    bl, 1
        jmp    SHORT loc_1_A32D
loc_1_A320:
        and    bl, 8
        jmp    SHORT loc_1_A32D
loc_1_A325:
        and    bl, 2
        jmp    SHORT loc_1_A32D
loc_1_A32A:
        and    bl, 4
loc_1_A32D:
        mov    byte ptr [esi + 7], bl
loc_1_A330:
        push   dword ptr [esi + 110h]
        call   sub_1_B5CF
        add    esp, 4
        test   al, al
        je     SHORT loc_1_A34C
        cmp    byte ptr [esi + 1ah], 3
        jne    SHORT loc_1_A34C
        and    byte ptr [esi + 7], 0f3h
loc_1_A34C:
        cmp    byte ptr [ebp + 0ch], 3
        ja     SHORT loc_1_A356
        and    byte ptr [esi + 7], 0f7h
loc_1_A356:
        cmp    byte ptr [esi + 7], 0
        jne    SHORT loc_1_A372
        push   3
        push   11042h
        push   116eh
        call   sub_1_1CB6
        add    esp, 0ch
        jmp    SHORT loc_1_A396
loc_1_A372:
        mov    al, 8
loc_1_A374:
        test   byte ptr [esi + 7], al
        je     SHORT loc_1_A387
        push   eax
        push   esi
        call   sub_1_9C5A
        add    esp, 8
        xor    al, al
        jmp    SHORT loc_1_A389
loc_1_A387:
        shr    al, 1
loc_1_A389:
        test   al, al
        jne    SHORT loc_1_A374
        push   esi
        call   sub_1_8BC4
        add    esp, 4
loc_1_A396:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_A39C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    word ptr [ebp - 4], bx
        push   ebx
        push   8
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [esi + 16h]
        push   eax
        push   9
        push   esi
        call   sub_1_740C
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_1_A58E
        push   20h
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_A58E
        lea    eax, [ebp - 4]
        push   eax
        push   10h
        push   esi
        call   sub_1_8D17
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_A457
        push   esi
        call   sub_1_66A9
        add    esp, 4
        push   20h
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_A58E
        lea    eax, [ebp - 4]
        push   eax
        push   8
        push   esi
        call   sub_1_8D17
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_A457
        push   esi
        call   sub_1_66A9
        add    esp, 4
        push   18h
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_A58E
        lea    eax, [ebp - 4]
        push   eax
        push   8
        push   esi
        call   sub_1_8D17
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_A457
        push   esi
        mov    bl, 1
        call   sub_1_66A9
        add    esp, 4
loc_1_A457:
        cmp    word ptr [ebp - 4], 47h
        je     SHORT loc_1_A470
        mov    ax, word ptr [ebp - 4]
        and    ax, 0ffc0h
        cmp    ax, 11c0h
        je     SHORT loc_1_A470
        test   bl, bl
        je     SHORT loc_1_A4D9
loc_1_A470:
        push   3
        push   esi
        call   sub_1_6CF2
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_A58E
        push   9
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_A58E
        lea    eax, [ebp - 1]
        push   eax
        push   8
        push   esi
        call   sub_1_8D17
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_A4B2
        push   esi
        call   sub_1_66A9
        add    esp, 4
loc_1_A4B2:
        push   0
        push   esi
        call   sub_1_6CF2
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_1_A4D9
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 1167h
        je     SHORT loc_1_A4D9
        mov    eax, edi
        jmp    NEAR PTR loc_1_A58E
loc_1_A4D9:
        cmp    word ptr [ebp - 4], 47h
        je     NEAR PTR loc_1_A56F
        mov    ax, word ptr [ebp - 4]
        and    ax, 0ffc0h
        cmp    ax, 11c0h
        je     SHORT loc_1_A56F
        cmp    word ptr [ebp - 4], 5
        jne    SHORT loc_1_A4FF
        mov    byte ptr [esi + 19h], 5
        jmp    SHORT loc_1_A57D
loc_1_A4FF:
        movzx  eax, ax
        test   eax, eax
        je     SHORT loc_1_A54B
        cmp    eax, 140h
        je     SHORT loc_1_A551
        cmp    eax, 180h
        je     SHORT loc_1_A557
        cmp    eax, 1c0h
        je     SHORT loc_1_A55D
        cmp    eax, 380h
        je     SHORT loc_1_A563
        cmp    eax, 8880h
        je     SHORT loc_1_A569
        mov    byte ptr [esi + 19h], 9
        test   bl, bl
        je     SHORT loc_1_A57D
        cmp    byte ptr [ebp - 1], 0a5h
        jne    SHORT loc_1_A53B
        mov    byte ptr [esi + 19h], 6
loc_1_A53B:
        test   bl, bl
        je     SHORT loc_1_A57D
        cmp    byte ptr [ebp - 1], 0a5h
        je     SHORT loc_1_A57D
        mov    byte ptr [esi + 19h], 3
        jmp    SHORT loc_1_A57D
loc_1_A54B:
        mov    byte ptr [esi + 19h], 4
        jmp    SHORT loc_1_A57D
loc_1_A551:
        mov    byte ptr [esi + 19h], 5
        jmp    SHORT loc_1_A57D
loc_1_A557:
        mov    byte ptr [esi + 19h], 2
        jmp    SHORT loc_1_A57D
loc_1_A55D:
        mov    byte ptr [esi + 19h], 3
        jmp    SHORT loc_1_A57D
loc_1_A563:
        mov    byte ptr [esi + 19h], 8
        jmp    SHORT loc_1_A57D
loc_1_A569:
        mov    byte ptr [esi + 19h], 7
        jmp    SHORT loc_1_A57D
loc_1_A56F:
        cmp    byte ptr [ebp - 1], 0a5h
        mov    byte ptr [esi + 19h], 1
        je     SHORT loc_1_A57D
        mov    byte ptr [esi + 19h], 9
loc_1_A57D:
        push   dword ptr [ebp - 4]
        push   ebx
        push   esi
        call   sub_1_B5F5
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_A58E
loc_1_A58E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_A595:
        mov    ecx, dword ptr [esp + 4]
        movzx  eax, byte ptr [ecx + 1ch]
        cmp    eax, 4
        jl     SHORT loc_1_A5A7
        cmp    eax, 8
        jle    SHORT loc_1_A5AB
loc_1_A5A7:
        xor    al, al
        jmp    SHORT loc_1_A5AD
loc_1_A5AB:
        mov    al, 3
loc_1_A5AD:
        mov    byte ptr [ecx + 45h], al
        ret
sub_1_A5B1:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    edi, dword ptr [esp + 10h]
        push   28h
        push   ebx
        lea    esi, [edi + 0ch]
        push   esi
        call   sub_1_33CF
        mov    word ptr [edi + 14h], bx
        add    esp, 0ch
        mov    word ptr [edi + 16h], bx
        mov    dword ptr [esi], ebx
        mov    byte ptr [edi + 18h], 3
        mov    byte ptr [edi + 19h], 0ah
        mov    eax, 1
        mov    byte ptr [edi + 1dh], bl
        mov    byte ptr [edi + 1bh], al
        mov    byte ptr [edi + 1ah], al
        push   2
        mov    byte ptr [edi + 1ch], al
        mov    word ptr [edi + 10h], 400h
        mov    word ptr [edi + 12h], 20h
        push   edi
        call   sub_1_9C5A
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_A607:
        movzx  eax, byte ptr [esp + 8]
        push   esi
        cmp    eax, 1
        push   edi
        je     SHORT loc_1_A64E
        cmp    eax, 3
        je     SHORT loc_1_A660
        cmp    eax, 4
        je     SHORT loc_1_A672
        cmp    eax, 5
        je     SHORT loc_1_A684
        mov    edi, dword ptr [esp + 0ch]
loc_1_A626:
        push   edi
        call   sub_1_65A2
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_1_A649
        push   edi
        call   sub_1_66A9
        add    esp, 4
        mov    esi, eax
        push   edi
        call   sub_1_7106
        add    esp, 4
loc_1_A649:
        mov    eax, esi
        pop    edi
        pop    esi
        ret
loc_1_A64E:
        mov    edi, dword ptr [esp + 0ch]
        mov    byte ptr [edi + 3], 2dh
        mov    byte ptr [edi + 4], 0ch
        mov    byte ptr [edi + 5], 1
        jmp    SHORT loc_1_A626
loc_1_A660:
        mov    edi, dword ptr [esp + 0ch]
        mov    byte ptr [edi + 3], 8fh
        mov    byte ptr [edi + 4], 0eh
        mov    byte ptr [edi + 5], 3
        jmp    SHORT loc_1_A626
loc_1_A672:
        mov    edi, dword ptr [esp + 0ch]
        mov    byte ptr [edi + 3], 0dh
        mov    byte ptr [edi + 4], 0ch
        mov    byte ptr [edi + 5], 0
        jmp    SHORT loc_1_A626
loc_1_A684:
        mov    edi, dword ptr [esp + 0ch]
        mov    byte ptr [edi + 3], 2dh
        mov    byte ptr [edi + 4], 0ch
        mov    byte ptr [edi + 5], 0
        jmp    SHORT loc_1_A626
sub_1_A696:
        mov    eax, dword ptr [esp + 4]
        mov    al, byte ptr [eax + 4e6h]
        sub    al, 21h
        cmp    al, 1
        sbb    al, al
        neg    al
        ret
sub_1_A6A9:
        mov    al, byte ptr [esp + 8]
        and    al, 3
        push   eax
        mov    eax, dword ptr [esp + 8]
        push   dword ptr [eax + 0d8h]
        push   dword ptr [eax + 110h]
        call   sub_1_6100
        add    esp, 0ch
        ret
sub_1_A6C9:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    byte ptr [ebp - 2], 4
        push   esi
        lea    ecx, [ebp - 2]
        mov    esi, dword ptr [ebp + 8]
        push   0
        push   2
        push   ecx
        mov    al, byte ptr [esi + 5]
        push   esi
        mov    byte ptr [ebp - 1], al
        call   sub_1_5DDF
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_1_A700
        push   1
        push   dword ptr [ebp + 0ch]
        push   esi
        call   sub_1_5FD8
        add    esp, 0ch
loc_1_A700:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_A705:
        sub    esp, 4
        push   esi
        push   0
        mov    esi, dword ptr [esp + 10h]
        push   5
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        lea    eax, [esp + 13h]
        add    esp, 0ch
        push   eax
        push   esi
        call   sub_1_A6C9
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_A74C
        test   byte ptr [esp + 7], 10h
        jne    SHORT loc_1_A74C
        push   1
        push   11049h
        push   115bh
        call   sub_1_1CB6
        add    esp, 0ch
loc_1_A74C:
        pop    esi
        add    esp, 4
        ret
sub_1_A751:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_6E6A
        add    esp, 4
        mov    al, byte ptr [esi + 10bh]
        cmp    al, 80h
        jae    SHORT loc_1_A773
        add    al, 0ah
        jmp    SHORT loc_1_A775
loc_1_A773:
        sub    al, 0ah
loc_1_A775:
        push   dword ptr [esi + 110h]
        mov    byte ptr [ebp - 8], 0fh
        mov    byte ptr [ebp - 6], al
        mov    al, byte ptr [esi + 5]
        mov    byte ptr [ebp - 7], al
        call   sub_1_8CF8
        add    esp, 4
        lea    ecx, [ebp - 8]
        push   0
        push   3
        push   ecx
        push   esi
        call   sub_1_5DDF
        add    esp, 10h
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_1_A7C3
        push   dword ptr [esi + 110h]
        call   sub_1_8D0B
        add    esp, 4
        push   esi
        call   sub_1_6E6A
        add    esp, 4
        jmp    NEAR PTR loc_1_A92C
loc_1_A7C3:
        mov    al, byte ptr [ebp - 6]
        mov    cl, byte ptr [esi + 3eh]
        push   1
        mov    byte ptr [esi + 0bbh], al
        cmp    byte ptr [esi + 8ah], cl
        jne    SHORT loc_1_A7DE
        push   dword ptr [esi + 64h]
        jmp    SHORT loc_1_A7E1
loc_1_A7DE:
        push   dword ptr [esi + 68h]
loc_1_A7E1:
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        mov    byte ptr [esi + 10eh], 0
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_A8BA
        cmp    dword ptr [esi + 0d4h], 0
        jne    NEAR PTR loc_1_A8BA
        push   2
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_1_5FD8
        add    esp, 0ch
        mov    edi, eax
        test   edi, edi
        jne    NEAR PTR loc_1_A8A6
        test   byte ptr [ebp - 2], 0c0h
        jne    SHORT loc_1_A888
        push   dword ptr [esi + 110h]
        call   sub_1_60CE
        add    esp, 4
        cmp    al, 1
        je     SHORT loc_1_A85C
        mov    al, byte ptr [esi + 5]
        or     al, 20h
        cmp    al, byte ptr [ebp - 2]
        je     SHORT loc_1_A85C
        push   1
        push   11051h
        push   115dh
        call   sub_1_1CB6
        add    esp, 0ch
        mov    edi, eax
loc_1_A85C:
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [esi + 0bbh], al
        je     SHORT loc_1_A87D
        push   1
        push   11051h
        push   1157h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    edi, eax
loc_1_A87D:
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 10bh], al
        jmp    SHORT loc_1_A89E
loc_1_A888:
        push   1
        push   11051h
        push   115dh
        call   sub_1_1CB6
        add    esp, 0ch
        mov    edi, eax
loc_1_A89E:
        test   edi, edi
        je     NEAR PTR loc_1_A92C
loc_1_A8A6:
        push   esi
        call   sub_1_6E6A
        add    esp, 4
        push   esi
        call   sub_1_653B
        add    esp, 4
        jmp    SHORT loc_1_A92A
loc_1_A8BA:
        push   dword ptr [esi + 94h]
        push   dword ptr [esi + 90h]
        push   dword ptr [esi + 0cch]
        push   1
        push   dword ptr [esi + 110h]
        mov    byte ptr [esi + 10fh], 0
        call   sub_1_5F42
        add    esp, 14h
        push   esi
        call   sub_1_6E6A
        add    esp, 4
        push   esi
        call   sub_1_653B
        add    esp, 4
        mov    edi, eax
        mov    eax, dword ptr [esi + 0d4h]
        test   eax, eax
        je     SHORT loc_1_A903
        mov    edi, eax
loc_1_A903:
        test   edi, edi
        je     SHORT loc_1_A916
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 15a0h
        jne    SHORT loc_1_A92C
loc_1_A916:
        push   1
        push   11051h
        push   1154h
        call   sub_1_1CB6
        add    esp, 0ch
loc_1_A92A:
        mov    edi, eax
loc_1_A92C:
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_A934:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        push   edi
        call   sub_1_653B
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_A9CC
        push   0dh
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_A9CC
        push   0
        push   3
        push   dword ptr [edi + 110h]
        call   sub_1_5BDA
        mov    si, word ptr [esp + 1ch]
        add    esp, 0ch
        mov    ax, si
        add    al, 2
        push   eax
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_A9CC
        push   0
        push   2710h
        push   edi
        call   sub_1_7191
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_A9CC
        cmp    byte ptr [edi + 0b5h], 0
        mov    word ptr [edi + 38h], si
        jne    SHORT loc_1_A9AE
        cmp    byte ptr [edi + 0b6h], 0
        je     SHORT loc_1_A9CA
loc_1_A9AE:
        push   edi
        call   sub_1_B796
        add    esp, 4
        test   al, al
        je     SHORT loc_1_A9C4
        mov    dword ptr [edi + 34h], 0
        jmp    SHORT loc_1_A9CA
loc_1_A9C4:
        mov    eax, dword ptr [edi + 7ch]
        mov    dword ptr [edi + 34h], eax
loc_1_A9CA:
        xor    eax, eax
loc_1_A9CC:
        pop    edi
        pop    esi
        ret
sub_1_A9CF:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        push   29h
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_AABF
        push   0
        push   3
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        push   29h
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_AABF
        push   0
        push   3
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        mov    bl, byte ptr [esp + 20h]
        add    esp, 0ch
        mov    al, bl
        and    al, 0fh
        add    al, 2
        push   eax
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_AABF
        shr    bl, 4
        push   0
        push   3
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        push   28h
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_AABF
        push   0
        push   3
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        push   28h
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_AABF
        push   0
        and    bl, 0fh
        push   3
        add    bl, 2
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        push   ebx
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    edi, eax
        mov    eax, edi
        test   edi, edi
        jne    SHORT loc_1_AABF
        push   0
        push   3
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        mov    eax, edi
loc_1_AABF:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_AAC3:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        push   2bh
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_AB69
        push   0
        push   3
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        mov    bl, byte ptr [esp + 20h]
        add    esp, 0ch
        mov    al, bl
        and    al, 0fh
        add    al, 2
        push   eax
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_AB69
        shr    bl, 4
        push   0
        push   3
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        push   2ah
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_AB69
        push   0
        and    bl, 0fh
        push   3
        add    bl, 2
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        push   ebx
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    edi, eax
        mov    eax, edi
        test   edi, edi
        jne    SHORT loc_1_AB69
        push   0
        push   3
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        mov    eax, edi
loc_1_AB69:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_AB6D:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    edx, 0ah
        sub    esp, 20h
        mov    byte ptr [ebp - 20h], dl
        mov    byte ptr [ebp - 1eh], cl
        mov    byte ptr [ebp - 1ch], cl
        mov    byte ptr [ebp - 1fh], 21h
        mov    byte ptr [ebp - 1dh], 22h
        push   esi
        push   edi
        mov    eax, 1fh
        mov    byte ptr [ebp - 19h], cl
        mov    byte ptr [ebp - 1bh], al
        mov    byte ptr [ebp - 1ah], al
        mov    eax, 20h
        lea    edi, [ebp - 13h]
        mov    byte ptr [ebp - 18h], al
        mov    byte ptr [ebp - 17h], al
        xor    eax, eax
        mov    byte ptr [ebp - 16h], cl
        mov    byte ptr [ebp - 15h], 0fdh
        mov    byte ptr [ebp - 14h], cl
        stosw
        stosb
        mov    byte ptr [ebp - 10h], dl
        mov    byte ptr [ebp - 0eh], cl
        mov    edx, 28h
        mov    byte ptr [ebp - 0ch], cl
        mov    byte ptr [ebp - 0bh], dl
        mov    byte ptr [ebp - 0ah], dl
        mov    eax, 29h
        mov    byte ptr [ebp - 9], cl
        mov    byte ptr [ebp - 8], al
        mov    byte ptr [ebp - 7], al
        mov    byte ptr [ebp - 6], cl
        mov    byte ptr [ebp - 4], cl
        lea    edi, [ebp - 3]
        xor    eax, eax
        mov    byte ptr [ebp - 0fh], 2ah
        mov    byte ptr [ebp - 0dh], 2bh
        mov    byte ptr [ebp - 5], 0fdh
        stosw
        stosb
        mov    eax, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        inc    eax
        cmp    byte ptr [esi + 19h], 1
        jne    NEAR PTR loc_1_ACAD
        mov    di, word ptr [esi + 16h]
        cmp    di, 26h
        jb     SHORT loc_1_AC43
        cmp    di, dx
        ja     SHORT loc_1_AC43
        mov    ecx, eax
        shr    eax, 4
        and    cl, 0fh
        add    cl, 2
        and    al, 0fh
        add    al, 2
        mov    byte ptr [ebp - 16h], cl
        lea    ecx, [ebp - 20h]
        mov    byte ptr [ebp - 19h], al
        mov    byte ptr [ebp - 1ch], 5
        mov    byte ptr [ebp - 1eh], 6
        push   ecx
        push   esi
        call   sub_1_7665
        add    esp, 8
        jmp    NEAR PTR loc_1_ACCC
loc_1_AC43:
        mov    edx, eax
        shr    eax, 4
        and    dl, 0fh
        add    dl, 2
        and    al, 0fh
        add    al, 2
        mov    byte ptr [ebp - 6], dl
        cmp    di, 3fh
        mov    byte ptr [ebp - 9], al
        jb     SHORT loc_1_AC7B
        cmp    di, 40h
        ja     SHORT loc_1_AC7B
        mov    byte ptr [ebp - 0ch], 4
        mov    byte ptr [ebp - 0eh], 8
        lea    eax, [ebp - 10h]
        push   eax
        push   esi
        call   sub_1_7665
        add    esp, 8
        jmp    SHORT loc_1_ACCC
loc_1_AC7B:
        cmp    di, 50h
        jb     SHORT loc_1_AC9E
        cmp    di, 6eh
        jae    SHORT loc_1_AC9E
        mov    byte ptr [ebp - 0ch], 5
        mov    byte ptr [ebp - 0eh], 8
        lea    eax, [ebp - 10h]
        push   eax
        push   esi
        call   sub_1_7665
        add    esp, 8
        jmp    SHORT loc_1_ACCC
loc_1_AC9E:
        mov    al, byte ptr [esi + 61h]
        cmp    al, 0ch
        je     SHORT loc_1_ACA9
        cmp    al, 0dh
        jne    SHORT loc_1_ACCE
loc_1_ACA9:
        push   1
        jmp    SHORT loc_1_ACBA
loc_1_ACAD:
        mov    al, byte ptr [esi + 61h]
        cmp    al, 0ch
        je     SHORT loc_1_ACB8
        cmp    al, 0dh
        jne    SHORT loc_1_ACCE
loc_1_ACB8:
        push   2
loc_1_ACBA:
        push   11056h
        push   1173h
        call   sub_1_1CB6
        add    esp, 0ch
loc_1_ACCC:
        mov    ecx, eax
loc_1_ACCE:
        mov    eax, ecx
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_ACD6:
        push   ebx
        push   esi
        xor    si, si
        push   edi
        xor    ebx, ebx
        mov    edi, dword ptr [esp + 10h]
loc_1_ACE2:
        push   10h
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_AD01
        push   1
        push   13d620h
        push   edi
        call   sub_1_7191
        add    esp, 0ch
loc_1_AD01:
        inc    si
        cmp    si, 2
        jae    SHORT loc_1_AD12
        cmp    byte ptr [edi + 3ch], bl
        jne    SHORT loc_1_AD12
        test   eax, eax
        je     SHORT loc_1_ACE2
loc_1_AD12:
        test   eax, eax
        jne    SHORT loc_1_AD30
        cmp    byte ptr [edi + 3ch], 0
        jne    SHORT loc_1_AD30
        push   1
        push   1104dh
        push   116fh
        call   sub_1_1CB6
        add    esp, 0ch
loc_1_AD30:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_AD34:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        push   0
        push   esi
        call   sub_1_6CF2
        add    esp, 8
        mov    al, byte ptr [esi + 61h]
        cmp    al, 9
        je     SHORT loc_1_AD5F
        cmp    al, 0bh
        je     SHORT loc_1_AD5F
        push   dword ptr [esp + 14h]
        push   esi
        call   sub_1_7766
        add    esp, 8
        jmp    SHORT loc_1_ADCB
loc_1_AD5F:
        push   24h
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_ADDA
        push   1
        push   13d620h
        push   esi
        call   sub_1_7191
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_ADDA
        mov    edi, dword ptr [esp + 14h]
        push   0
        push   10h
        push   edi
        push   25h
        push   esi
        call   sub_1_740C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_1_ADDA
        mov    ecx, dword ptr [esp + 10h]
        cmp    word ptr [ecx + 40h], 0
        je     SHORT loc_1_ADC6
        mov    ecx, dword ptr [esi + 7ch]
        cmp    dword ptr [edi], ecx
        jae    SHORT loc_1_ADC2
        push   1
        push   11018h
        push   1161h
        call   sub_1_1CB6
        add    esp, 0ch
        jmp    SHORT loc_1_ADCB
loc_1_ADC2:
        mov    dword ptr [edi], ecx
        jmp    SHORT loc_1_ADCB
loc_1_ADC6:
        mov    ecx, dword ptr [edi]
        mov    dword ptr [esi + 7ch], ecx
loc_1_ADCB:
        test   eax, eax
        jne    SHORT loc_1_ADDA
        push   1
        push   esi
        call   sub_1_6CF2
        add    esp, 8
loc_1_ADDA:
        pop    edi
        pop    esi
        ret
sub_1_ADDD:
        push   ebx
        xor    eax, eax
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        movzx  ecx, byte ptr [esi + 1ah]
        cmp    ecx, 4
        je     SHORT loc_1_ADF7
        cmp    ecx, 5
        je     SHORT loc_1_ADFB
        xor    bl, bl
        jmp    SHORT loc_1_ADFD
loc_1_ADF7:
        mov    bl, 11h
        jmp    SHORT loc_1_ADFD
loc_1_ADFB:
        mov    bl, 0dh
loc_1_ADFD:
        test   bl, bl
        je     SHORT loc_1_AE43
        push   1bh
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_AE43
        push   0
        add    bl, 2
        push   3
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        push   ebx
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_AE43
        push   0
        push   2710h
        push   esi
        call   sub_1_7191
        add    esp, 0ch
loc_1_AE43:
        pop    esi
        pop    ebx
        ret
sub_1_AE46:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        inc    word ptr [edi + 38h]
        cmp    word ptr [esi + 0a6h], ax
        je     SHORT loc_1_AEB1
        cmp    word ptr [edi + 18h], 110bh
        je     SHORT loc_1_AE9F
        push   esi
        call   sub_1_66A9
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_1_AE8E
        push   ebx
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 115ch
        je     SHORT loc_1_AE8E
        mov    eax, ebx
        jmp    NEAR PTR loc_1_AFDC
loc_1_AE8E:
        push   esi
        call   sub_1_6660
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_AFDC
loc_1_AE9F:
        push   esi
        call   sub_1_AFE1
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_AEB8
        jmp    NEAR PTR loc_1_AFDC
loc_1_AEB1:
        mov    byte ptr [esi + 0b2h], 1
loc_1_AEB8:
        mov    dx, word ptr [esi + 0a6h]
        mov    ebx, dword ptr [ebp + 10h]
        test   dx, dx
        je     SHORT loc_1_AF29
        mov    cl, byte ptr [ebx + 5]
        cmp    byte ptr [esi + 0b1h], cl
        je     SHORT loc_1_AF29
        mov    edx, 1
        sub    cl, byte ptr [esi + 0b3h]
        shl    edx, cl
        or     dword ptr [edi + 30h], edx
        mov    di, word ptr [esi + 0a6h]
        mov    cl, byte ptr [ebx + 5]
        mov    word ptr [esi + 0a4h], di
        mov    byte ptr [esi + 0b1h], cl
        mov    dl, byte ptr [ebx + 5]
        movzx  cx, dl
        movzx  bx, byte ptr [esi + 0afh]
        sub    cx, bx
        mov    byte ptr [esi + 0afh], dl
        sub    word ptr [esi + 9ch], cx
        movzx  ecx, cx
        shl    ecx, 0ah
        add    dword ptr [esi + 90h], ecx
        jmp    NEAR PTR loc_1_AFDC
loc_1_AF29:
        mov    byte ptr [esi + 3dh], 1
        mov    ax, word ptr [esi + 0a4h]
        dec    ax
        mov    word ptr [esi + 0a4h], ax
        test   dx, dx
        je     SHORT loc_1_AF57
        test   ax, ax
        je     SHORT loc_1_AF57
        mov    ax, word ptr [edi + 18h]
        push   eax
        push   esi
        call   sub_1_B7C3
        jmp    NEAR PTR loc_1_AFD9
loc_1_AF57:
        mov    eax, 1
        push   1
        mov    byte ptr [esi + 0b2h], 1
        mov    cl, byte ptr [ebx + 5]
        push   1103eh
        sub    cl, byte ptr [esi + 0b3h]
        push   1152h
        shl    eax, cl
        or     dword ptr [edi + 2ch], eax
        call   sub_1_1CB6
        add    esp, 0ch
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], eax
        mov    cl, byte ptr [ebx + 5]
        movzx  ax, cl
        movzx  dx, byte ptr [esi + 0afh]
        sub    ax, dx
        inc    cl
        inc    ax
        mov    byte ptr [esi + 0afh], cl
        mov    byte ptr [esi + 0b1h], 0
        mov    cx, word ptr [esi + 0a6h]
        sub    word ptr [esi + 9ch], ax
        movzx  eax, ax
        mov    word ptr [esi + 0a4h], cx
        shl    eax, 0ah
        add    dword ptr [esi + 90h], eax
        mov    dx, word ptr [edi + 18h]
        push   edx
        push   esi
        call   sub_1_9373
loc_1_AFD9:
        add    esp, 8
loc_1_AFDC:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_AFE1:
        sub    esp, 8
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 14h]
        push   edi
        push   ebp
        xor    ebx, ebx
        mov    bp, 0ah
loc_1_AFF2:
        push   esi
        call   sub_1_9682
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_B136
        lea    eax, [esp + 10h]
        push   eax
        push   1388h
        push   esi
        call   sub_1_B834
        add    esp, 0ch
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_1_B033
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 15a0h
        jne    NEAR PTR loc_1_B0AF
        test   edi, edi
        jne    SHORT loc_1_B053
loc_1_B033:
        test   byte ptr [esp + 10h], 0c0h
        jne    SHORT loc_1_B053
        push   dword ptr [esi + 110h]
        call   sub_1_60CE
        add    esp, 4
        cmp    al, 1
        jne    SHORT loc_1_B09C
        test   byte ptr [esp + 11h], 1
        je     SHORT loc_1_B09C
loc_1_B053:
        mov    ax, word ptr [esi + 0ach]
        push   eax
        push   esi
        call   sub_1_A934
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_1_B084
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 15a0h
        jne    SHORT loc_1_B0B6
        push   esi
        xor    edi, edi
        call   sub_1_6E6A
        add    esp, 4
loc_1_B084:
        cmp    byte ptr [esi + 0b5h], bl
        jne    SHORT loc_1_B0BA
        cmp    byte ptr [esi + 0b6h], bl
        jne    SHORT loc_1_B0BA
        dec    bp
        jne    NEAR PTR loc_1_AFF2
loc_1_B09C:
        test   bp, bp
        jne    SHORT loc_1_B0DC
        push   1
        push   11037h
        push   1168h
        jmp    SHORT loc_1_B12E
loc_1_B0AF:
        mov    eax, edi
        jmp    NEAR PTR loc_1_B136
loc_1_B0B6:
        mov    eax, edi
        jmp    SHORT loc_1_B136
loc_1_B0BA:
        push   esi
        call   sub_1_B796
        add    esp, 4
        test   al, al
        je     SHORT loc_1_B0D2
        mov    dword ptr [esi + 34h], 0
        xor    eax, eax
        jmp    SHORT loc_1_B136
loc_1_B0D2:
        mov    eax, dword ptr [esi + 7ch]
        mov    dword ptr [esi + 34h], eax
        xor    eax, eax
        jmp    SHORT loc_1_B136
loc_1_B0DC:
        movzx  ecx, word ptr [esi + 50h]
        movzx  eax, byte ptr [esp + 14h]
        imul   ecx, eax
        movzx  edx, byte ptr [esp + 13h]
        movzx  eax, byte ptr [esp + 15h]
        add    ecx, edx
        dec    eax
        cdq
        and    edx, 1fh
        add    eax, edx
        sar    eax, 5
        lea    ebx, [eax + ecx*4]
        mov    ecx, dword ptr [esi + 7ch]
        mov    dword ptr [esi + 34h], ebx
        test   ecx, ecx
        je     SHORT loc_1_B122
        mov    eax, ebx
        sub    edx, edx
        div    ecx
        mov    word ptr [esi + 38h], ax
        sub    edx, edx
        mov    eax, ebx
        div    ecx
        mov    eax, edi
        mov    dword ptr [esi + 34h], edx
        jmp    SHORT loc_1_B136
loc_1_B122:
        push   1
        push   11037h
        push   116ch
loc_1_B12E:
        call   sub_1_1CB6
        add    esp, 0ch
loc_1_B136:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_B13E:
        sub    esp, 4
        xor    eax, eax
        push   ebx
        push   esi
        push   edi
        mov    bl, 0
        mov    edi, dword ptr [esp + 14h]
        push   ebp
        mov    ebp, dword ptr [edi + 0a8h]
        sub    ebp, dword ptr [edi + 34h]
        dec    ebp
        jns    SHORT loc_1_B15D
        neg    ebp
        mov    bl, 1
loc_1_B15D:
        cmp    bl, 1
        je     SHORT loc_1_B16B
        cmp    ebp, 5
        jle    NEAR PTR loc_1_B430
loc_1_B16B:
        push   edi
        call   sub_1_653B
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_B430
        test   bl, bl
        jne    SHORT loc_1_B18E
        cmp    byte ptr [edi + 40h], 1
        jne    SHORT loc_1_B18B
        sub    ebp, 3
        jmp    SHORT loc_1_B1E5
loc_1_B18B:
        dec    ebp
        jmp    SHORT loc_1_B1E5
loc_1_B18E:
        cmp    byte ptr [edi + 0b5h], 0
        jne    SHORT loc_1_B1C4
        cmp    byte ptr [edi + 0b6h], 0
        jne    SHORT loc_1_B1C4
        cmp    byte ptr [edi + 40h], 1
        je     SHORT loc_1_B1BF
        cmp    byte ptr [edi + 19h], 1
        jne    SHORT loc_1_B1BC
        mov    ax, word ptr [edi + 16h]
        cmp    ax, 3ch
        jb     SHORT loc_1_B1BC
        cmp    ax, 50h
        jb     SHORT loc_1_B1BF
loc_1_B1BC:
        inc    ebp
        jmp    SHORT loc_1_B1E5
loc_1_B1BF:
        add    ebp, 3
        jmp    SHORT loc_1_B1E5
loc_1_B1C4:
        cmp    byte ptr [edi + 19h], 1
        jne    SHORT loc_1_B1D4
        cmp    word ptr [edi + 16h], 58h
        jb     SHORT loc_1_B1D4
        inc    ebp
        jmp    SHORT loc_1_B1E5
loc_1_B1D4:
        mov    eax, dword ptr [edi + 7ch]
        mov    ecx, 3e8h
        imul   eax, eax, 2dh
        sub    edx, edx
        div    ecx
        add    ebp, eax
loc_1_B1E5:
        movzx  eax, byte ptr [edi + 40h]
        cmp    eax, 2
        je     SHORT loc_1_B255
        cmp    eax, 3
        je     NEAR PTR loc_1_B3B8
        test   bl, bl
        jne    SHORT loc_1_B201
        test   byte ptr [edi + 38h], 1
        je     SHORT loc_1_B20C
loc_1_B201:
        cmp    bl, 1
        jne    SHORT loc_1_B220
        test   byte ptr [edi + 38h], 1
        je     SHORT loc_1_B220
loc_1_B20C:
        push   0ch
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_B233
        jmp    NEAR PTR loc_1_B430
loc_1_B220:
        push   0bh
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_B430
loc_1_B233:
        push   ebp
        push   edi
        call   sub_1_B8CE
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_B430
        push   edi
        call   sub_1_653B
        add    esp, 4
        test   eax, eax
        jmp    NEAR PTR loc_1_B430
loc_1_B255:
        mov    al, 1ah
        cmp    bl, 1
        adc    al, 0ffh
        mov    byte ptr [esp + 13h], al
        push   dword ptr [esp + 13h]
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_B430
        push   0
        push   3
        push   dword ptr [edi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        mov    eax, ebp
        and    al, 0fh
        add    al, 2
        push   eax
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_B430
        sar    ebp, 4
        push   0
        push   3
        push   dword ptr [edi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        mov    eax, ebp
        and    al, 0fh
        add    al, 2
        push   eax
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_B430
        push   0
        push   3
        push   dword ptr [edi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        push   1
        push   dword ptr [edi + 6ch]
        push   edi
        call   sub_1_7191
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_B430
        sar    ebp, 4
        test   ebp, ebp
        je     NEAR PTR loc_1_B430
loc_1_B2FF:
        xor    si, si
loc_1_B302:
        push   dword ptr [esp + 13h]
        push   edi
        cmp    si, 1
        sbb    ebx, ebx
        call   sub_1_7237
        and    ebx, 0feh
        add    esp, 8
        inc    bx
        test   eax, eax
        jne    NEAR PTR loc_1_B430
        push   0
        push   3
        push   dword ptr [edi + 110h]
        call   sub_1_5BDA
        mov    ax, bx
        add    esp, 0ch
        and    al, 0fh
        add    al, 2
        push   eax
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_B430
        shr    bx, 4
        push   0
        push   3
        add    bl, 2
        push   dword ptr [edi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        push   ebx
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_B430
        push   0
        push   3
        push   dword ptr [edi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        push   1
        push   dword ptr [edi + 6ch]
        push   edi
        call   sub_1_7191
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_B430
        inc    si
        cmp    si, 2
        jb     NEAR PTR loc_1_B302
        dec    ebp
        jne    NEAR PTR loc_1_B2FF
        jmp    SHORT loc_1_B430
loc_1_B3B8:
        test   bl, bl
        jne    SHORT loc_1_B3CD
        push   23h
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_B3DC
        jmp    SHORT loc_1_B430
loc_1_B3CD:
        push   22h
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_B430
loc_1_B3DC:
        xor    si, si
        push   0
        push   3
        push   dword ptr [edi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
loc_1_B3F1:
        mov    eax, ebp
        and    al, 0fh
        add    al, 2
        push   eax
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_B430
        inc    si
        push   0
        sar    ebp, 4
        push   3
        push   dword ptr [edi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        cmp    si, 3
        jb     SHORT loc_1_B3F1
        push   1
        push   dword ptr [edi + 6ch]
        push   edi
        call   sub_1_7191
        add    esp, 0ch
loc_1_B430:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_B438:
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        xor    ebx, ebx
        mov    edi, dword ptr [esp + 10h]
        mov    ax, word ptr [edi + 38h]
        cmp    word ptr [edi + 0ach], ax
        jne    SHORT loc_1_B458
        cmp    dword ptr [edi + 0a8h], esi
        jne    SHORT loc_1_B46B
loc_1_B458:
        push   edi
        call   sub_1_653B
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        jne    NEAR PTR loc_1_B5C9
loc_1_B46B:
        cmp    dword ptr [edi + 0a8h], 0
        jne    SHORT loc_1_B4A8
        test   byte ptr [edi + 38h], 1
        jne    SHORT loc_1_B47E
        push   0bh
        jmp    SHORT loc_1_B480
loc_1_B47E:
        push   0ch
loc_1_B480:
        push   edi
        call   sub_1_7237
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    NEAR PTR loc_1_B5C9
        mov    ebx, dword ptr [edi + 6ch]
        push   0
        push   19h
        push   dword ptr [edi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
loc_1_B4A8:
        mov    ax, word ptr [edi + 38h]
        cmp    word ptr [edi + 0ach], ax
        je     SHORT loc_1_B500
        push   0dh
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_B5CB
        push   0
        push   3
        push   dword ptr [edi + 110h]
        call   sub_1_5BDA
        mov    ax, word ptr [edi + 0ach]
        add    esp, 0ch
        add    al, 2
        push   eax
        push   edi
        call   sub_1_7237
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    NEAR PTR loc_1_B5C9
        test   ebx, ebx
        jne    SHORT loc_1_B504
        mov    ebx, 2710h
loc_1_B500:
        test   ebx, ebx
        je     SHORT loc_1_B51A
loc_1_B504:
        push   0
        push   ebx
        push   edi
        call   sub_1_7191
        add    esp, 0ch
        mov    esi, eax
        test   esi, esi
        jne    NEAR PTR loc_1_B5C9
loc_1_B51A:
        cmp    dword ptr [edi + 0a8h], 0
        jne    SHORT loc_1_B571
        test   byte ptr [edi + 38h], 1
        jne    SHORT loc_1_B54B
        cmp    byte ptr [edi + 0b5h], 0
        jne    SHORT loc_1_B56A
        push   1
        push   11048h
        push   1168h
        call   sub_1_1CB6
        add    esp, 0ch
        jmp    NEAR PTR loc_1_B5CB
loc_1_B54B:
        cmp    byte ptr [edi + 0b6h], 0
        jne    SHORT loc_1_B56A
        push   2
        push   11048h
        push   1168h
        call   sub_1_1CB6
        add    esp, 0ch
        jmp    SHORT loc_1_B5CB
loc_1_B56A:
        mov    dword ptr [edi + 34h], 0
loc_1_B571:
        mov    ax, word ptr [edi + 0ach]
        cmp    word ptr [edi + 38h], ax
        je     SHORT loc_1_B5C9
        cmp    byte ptr [edi + 0b5h], 0
        mov    word ptr [edi + 38h], ax
        jne    SHORT loc_1_B594
        cmp    byte ptr [edi + 0b6h], 0
        je     SHORT loc_1_B5B0
loc_1_B594:
        push   edi
        call   sub_1_B796
        add    esp, 4
        test   al, al
        je     SHORT loc_1_B5AA
        mov    dword ptr [edi + 34h], 0
        jmp    SHORT loc_1_B5B0
loc_1_B5AA:
        mov    eax, dword ptr [edi + 7ch]
        mov    dword ptr [edi + 34h], eax
loc_1_B5B0:
        cmp    byte ptr [edi + 0b5h], 0
        jne    SHORT loc_1_B5C9
        cmp    byte ptr [edi + 0b6h], 0
        jne    SHORT loc_1_B5C9
        mov    eax, dword ptr [esp + 14h]
        mov    byte ptr [eax], 1
loc_1_B5C9:
        mov    eax, esi
loc_1_B5CB:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_B5CF:
        xor    al, al
        mov    ecx, dword ptr [esp + 4]
        cmp    byte ptr [ecx + 4e6h], 22h
        jne    SHORT loc_1_B5F4
        mov    cl, byte ptr [ecx + 4d1h]
        test   cl, cl
        je     SHORT loc_1_B5F2
        cmp    cl, 1
        je     SHORT loc_1_B5F2
        cmp    cl, 4
        jne    SHORT loc_1_B5F4
loc_1_B5F2:
        mov    al, 1
loc_1_B5F4:
        ret
sub_1_B5F5:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        movzx  ecx, byte ptr [esi + 19h]
        mov    word ptr [esi + 42h], ax
        cmp    ecx, 1
        mov    byte ptr [esi + 40h], 1
        je     NEAR PTR loc_1_B6BF
        cmp    ecx, 2
        je     NEAR PTR loc_1_B6D0
        cmp    ecx, 3
        je     NEAR PTR loc_1_B6E1
        cmp    ecx, 4
        je     NEAR PTR loc_1_B729
        cmp    ecx, 5
        je     NEAR PTR loc_1_B749
        cmp    ecx, 7
        je     NEAR PTR loc_1_B754
        cmp    ecx, 8
        je     NEAR PTR loc_1_B782
        push   1
        mov    byte ptr [esi], al
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_B791
        push   0
        push   3e8h
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        push   esi
        call   sub_1_65A2
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_B791
        push   0
        lea    eax, [ebp - 1]
        push   8
        push   eax
        push   8
        push   esi
        call   sub_1_740C
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_1_B791
        test   byte ptr [ebp - 1], 80h
        je     SHORT loc_1_B6B5
        mov    byte ptr [esi + 1bh], 3
        mov    byte ptr [esi + 1ah], 3
        jmp    NEAR PTR loc_1_B791
loc_1_B6B5:
        mov    ecx, 2
        jmp    NEAR PTR loc_1_B78B
loc_1_B6BF:
        push   dword ptr [ebp + 10h]
        push   esi
        call   sub_1_BB52
        add    esp, 8
        jmp    NEAR PTR loc_1_B791
loc_1_B6D0:
        push   dword ptr [ebp + 10h]
        push   esi
        call   sub_1_BA66
        add    esp, 8
        jmp    NEAR PTR loc_1_B791
loc_1_B6E1:
        mov    byte ptr [esi + 40h], 2
        mov    dl, byte ptr [ebp + 0ch]
        test   dl, dl
        jne    SHORT loc_1_B71F
        movzx  ecx, word ptr [ebp + 10h]
        and    ecx, 0ffff003fh
        cmp    ecx, 0ah
        jne    SHORT loc_1_B705
        mov    ecx, 3
        jmp    NEAR PTR loc_1_B78B
loc_1_B705:
        test   dl, dl
        jne    SHORT loc_1_B71F
        movzx  ecx, word ptr [ebp + 10h]
        and    ecx, 0ffff003fh
        cmp    ecx, 0ch
        jne    SHORT loc_1_B71F
        mov    ecx, 4
        jmp    SHORT loc_1_B78B
loc_1_B71F:
        mov    ecx, 2
        mov    byte ptr [esi], 0
        jmp    SHORT loc_1_B78B
loc_1_B729:
        movzx  ecx, word ptr [ebp + 10h]
        and    ecx, 0ffff003fh
        mov    byte ptr [esi], 0
        cmp    ecx, 21h
        jne    SHORT loc_1_B742
        mov    ecx, 3
        jmp    SHORT loc_1_B78B
loc_1_B742:
        mov    ecx, 2
        jmp    SHORT loc_1_B78B
loc_1_B749:
        push   esi
        call   sub_1_B945
        add    esp, 4
        jmp    SHORT loc_1_B791
loc_1_B754:
        movzx  ecx, word ptr [ebp + 10h]
        and    ecx, 0ffff003fh
        je     SHORT loc_1_B76C
        cmp    ecx, 1
        je     SHORT loc_1_B777
        cmp    ecx, 2
        je     SHORT loc_1_B782
        jmp    SHORT loc_1_B791
loc_1_B76C:
        mov    byte ptr [esi + 40h], 2
        mov    ecx, 3
        jmp    SHORT loc_1_B78B
loc_1_B777:
        mov    byte ptr [esi + 40h], 3
        mov    ecx, 4
        jmp    SHORT loc_1_B78B
loc_1_B782:
        mov    byte ptr [esi + 40h], 3
        mov    ecx, 5
loc_1_B78B:
        mov    byte ptr [esi + 1bh], cl
        mov    byte ptr [esi + 1ah], cl
loc_1_B791:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_B796:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 38h]
        and    ax, 1
        jne    SHORT loc_1_B7AD
        cmp    byte ptr [ecx + 0b5h], 0
        jne    SHORT loc_1_B7BC
loc_1_B7AD:
        cmp    ax, 1
        jne    SHORT loc_1_B7C0
        cmp    byte ptr [ecx + 0b6h], 0
        je     SHORT loc_1_B7C0
loc_1_B7BC:
        mov    al, 1
        jmp    SHORT loc_1_B7C2
loc_1_B7C0:
        xor    al, al
loc_1_B7C2:
        ret
sub_1_B7C3:
        push   esi
        xor    eax, eax
        cmp    word ptr [esp + 0ch], 110bh
        push   edi
        jne    SHORT loc_1_B831
        mov    edi, dword ptr [esp + 0ch]
        push   edi
        call   sub_1_66A9
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_1_B7F6
        push   esi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 115ch
        je     SHORT loc_1_B7F6
        mov    eax, esi
        jmp    SHORT loc_1_B831
loc_1_B7F6:
        inc    word ptr [edi + 114h]
        cmp    word ptr [edi + 0a4h], 3
        jbe    SHORT loc_1_B80B
        push   3
        jmp    SHORT loc_1_B80D
loc_1_B80B:
        push   4
loc_1_B80D:
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_B831
        push   1
        push   3e80h
        push   edi
        call   sub_1_7191
        mov    byte ptr [edi + 0b4h], 0
        add    esp, 0ch
loc_1_B831:
        pop    edi
        pop    esi
        ret
sub_1_B834:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    byte ptr [ebp - 2], 4ah
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [esi + 110h]
        mov    al, byte ptr [esi + 5]
        mov    byte ptr [ebp - 1], al
        call   sub_1_8CF8
        add    esp, 4
        lea    ecx, [ebp - 2]
        push   1
        push   2
        push   ecx
        push   esi
        call   sub_1_5DDF
        add    esp, 10h
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_1_B889
        push   dword ptr [esi + 110h]
        call   sub_1_8D0B
        add    esp, 4
        push   esi
        call   sub_1_6E6A
        add    esp, 4
        mov    eax, edi
        jmp    SHORT loc_1_B8C8
loc_1_B889:
        push   1
        push   dword ptr [ebp + 0ch]
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        mov    edi, eax
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 15a0h
        jne    SHORT loc_1_B8BA
        push   esi
        call   sub_1_6E6A
        add    esp, 4
        mov    eax, edi
        jmp    SHORT loc_1_B8C8
loc_1_B8BA:
        push   7
        push   dword ptr [ebp + 10h]
        push   esi
        call   sub_1_5FD8
        add    esp, 0ch
loc_1_B8C8:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_B8CE:
        mov    eax, dword ptr [esp + 8]
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        cmp    byte ptr [edi + 86h], 2
        jne    SHORT loc_1_B8E9
        imul   eax, eax, 109h
        jmp    SHORT loc_1_B8EF
loc_1_B8E9:
        imul   eax, eax, 186h
loc_1_B8EF:
        push   0
        push   eax
        push   edi
        call   sub_1_7191
        add    esp, 0ch
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_1_B918
        push   esi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 15a0h
        je     SHORT loc_1_B914
        mov    eax, esi
        jmp    SHORT loc_1_B942
loc_1_B914:
        test   esi, esi
        jne    SHORT loc_1_B940
loc_1_B918:
        cmp    byte ptr [edi + 0b5h], 0
        jne    SHORT loc_1_B940
        cmp    byte ptr [edi + 0b6h], 0
        jne    SHORT loc_1_B940
        push   1
        push   1104ch
        push   115bh
        call   sub_1_1CB6
        add    esp, 0ch
        jmp    SHORT loc_1_B942
loc_1_B940:
        xor    eax, eax
loc_1_B942:
        pop    edi
        pop    esi
        ret
sub_1_B945:
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        push   3
        push   esi
        mov    byte ptr [esi + 40h], 2
        call   sub_1_6CF2
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    NEAR PTR loc_1_BA5D
        push   28h
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    NEAR PTR loc_1_BA5D
        lea    edi, [esi + 42h]
        push   edi
        push   10h
        push   esi
        call   sub_1_8D17
        add    esp, 0ch
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_1_B9CA
        mov    ax, word ptr [edi]
        test   al, 8
        je     SHORT loc_1_B99D
        mov    byte ptr [esi + 40h], 3
loc_1_B99D:
        test   al, 1
        je     SHORT loc_1_B9AC
        mov    ecx, 2
        mov    byte ptr [esi + 1bh], cl
        mov    byte ptr [esi + 1ah], cl
loc_1_B9AC:
        test   al, 0eh
        je     SHORT loc_1_B9BB
        mov    ecx, 3
        mov    byte ptr [esi + 1bh], cl
        mov    byte ptr [esi + 1ah], cl
loc_1_B9BB:
        test   al, 20h
        je     NEAR PTR loc_1_BA4C
        mov    eax, 4
        jmp    SHORT loc_1_BA46
loc_1_B9CA:
        push   esi
        call   sub_1_66A9
        add    esp, 4
        mov    ebx, eax
        push   ebx
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 1162h
        jne    SHORT loc_1_BA4C
        push   1
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_1_BA5D
        push   0
        push   3e8h
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        push   esi
        call   sub_1_65A2
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_1_BA5D
        lea    eax, [esp + 0fh]
        push   0
        push   8
        push   eax
        push   8
        push   esi
        call   sub_1_740C
        add    esp, 14h
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_1_BA5D
        test   byte ptr [esp + 0fh], 80h
        je     SHORT loc_1_BA41
        mov    eax, 3
        jmp    SHORT loc_1_BA46
loc_1_BA41:
        mov    eax, 2
loc_1_BA46:
        mov    byte ptr [esi + 1bh], al
        mov    byte ptr [esi + 1ah], al
loc_1_BA4C:
        test   ebx, ebx
        jne    SHORT loc_1_BA5D
        push   0
        push   esi
        call   sub_1_6CF2
        add    esp, 8
        mov    ebx, eax
loc_1_BA5D:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_BA66:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        movzx  eax, word ptr [ebp + 0ch]
        push   ebx
        and    eax, 0ffff003fh
        push   esi
        push   edi
        xor    edi, edi
        mov    esi, dword ptr [ebp + 8]
        cmp    eax, 15h
        mov    byte ptr [esi + 40h], 2
        jne    SHORT loc_1_BA9A
        mov    byte ptr [esi + 40h], 3
        mov    eax, 4
        mov    byte ptr [esi + 1bh], al
        mov    byte ptr [esi + 1ah], al
        jmp    NEAR PTR loc_1_BB49
loc_1_BA9A:
        push   1bh
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    NEAR PTR loc_1_BB49
        push   0
        mov    ebx, 3
        push   ebx
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        push   5
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_BB49
        push   0
        push   2710h
        push   esi
        call   sub_1_7191
        add    esp, 0ch
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_BB2A
        push   0
        lea    eax, [ebp - 1]
        push   8
        push   eax
        push   8
        push   esi
        call   sub_1_740C
        add    esp, 14h
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_BB49
        and    byte ptr [ebp - 1], 18h
        shr    byte ptr [ebp - 1], 3
        cmp    byte ptr [ebp - 1], bl
        jne    SHORT loc_1_BB1A
        mov    byte ptr [esi + 1bh], bl
        mov    byte ptr [esi + 1ah], bl
        jmp    SHORT loc_1_BB49
loc_1_BB1A:
        mov    eax, 2
        mov    byte ptr [esi], 0
        mov    byte ptr [esi + 1bh], al
        mov    byte ptr [esi + 1ah], al
        jmp    SHORT loc_1_BB49
loc_1_BB2A:
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 116eh
        jne    SHORT loc_1_BB49
        mov    eax, 2
        xor    edi, edi
        mov    byte ptr [esi], 0
        mov    byte ptr [esi + 1bh], al
        mov    byte ptr [esi + 1ah], al
loc_1_BB49:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_BB52:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 16h]
        mov    byte ptr [esi + 11fh], bl
        cmp    ax, 3ch
        mov    byte ptr [esi + 121h], bl
        jb     SHORT loc_1_BB83
        mov    byte ptr [esi + 40h], 2
        cmp    ax, 57h
        jb     SHORT loc_1_BB83
        mov    byte ptr [esi + 40h], 3
loc_1_BB83:
        cmp    ax, 6eh
        jae    SHORT loc_1_BC03
        push   1bh
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    NEAR PTR loc_1_BC5D
        push   0
        push   3
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        push   2
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    NEAR PTR loc_1_BC5D
        push   0
        push   2710h
        push   esi
        call   sub_1_7191
        add    esp, 0ch
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_1_BBE5
        mov    byte ptr [esi + 1bh], 2
        mov    byte ptr [esi + 1ah], 2
        jmp    SHORT loc_1_BC5D
loc_1_BBE5:
        push   ebx
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 111fh
        jne    SHORT loc_1_BC5D
        mov    eax, 3
        xor    ebx, ebx
        mov    byte ptr [esi + 1bh], al
        mov    byte ptr [esi + 1ah], al
        jmp    SHORT loc_1_BC5D
loc_1_BC03:
        movzx  eax, word ptr [ebp + 0ch]
        and    eax, 0ffff003fh
        je     SHORT loc_1_BC24
        cmp    eax, 1
        je     SHORT loc_1_BC31
        cmp    eax, 2
        je     SHORT loc_1_BC3E
        cmp    eax, 3
        je     SHORT loc_1_BC4B
        cmp    eax, 4
        je     SHORT loc_1_BC31
        jmp    SHORT loc_1_BC5D
loc_1_BC24:
        mov    eax, 2
        mov    byte ptr [esi + 1bh], al
        mov    byte ptr [esi + 1ah], al
        jmp    SHORT loc_1_BC5D
loc_1_BC31:
        mov    eax, 3
        mov    byte ptr [esi + 1bh], al
        mov    byte ptr [esi + 1ah], al
        jmp    SHORT loc_1_BC5D
loc_1_BC3E:
        mov    eax, 4
        mov    byte ptr [esi + 1bh], al
        mov    byte ptr [esi + 1ah], al
        jmp    SHORT loc_1_BC5D
loc_1_BC4B:
        mov    byte ptr [esi + 1bh], 5
        cmp    byte ptr [esi + 45h], 0
        mov    byte ptr [esi + 1ah], 4
        je     SHORT loc_1_BC5D
        mov    byte ptr [esi + 1ah], 5
loc_1_BC5D:
        cmp    word ptr [esi + 16h], 3ch
        jb     SHORT loc_1_BCD7
        push   3
        push   esi
        call   sub_1_6CF2
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_BCD9
        push   0
        lea    eax, [ebp - 1]
        push   8
        push   eax
        push   25h
        push   esi
        call   sub_1_740C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_1_BCD9
        test   byte ptr [ebp - 1], 2
        je     SHORT loc_1_BCA6
        cmp    word ptr [esi + 16h], 6eh
        jae    SHORT loc_1_BCA6
        push   20h
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_BCD9
loc_1_BCA6:
        push   0
        push   esi
        call   sub_1_6CF2
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_1_BCD7
        test   byte ptr [ebp - 1], 2
        je     SHORT loc_1_BCD7
        cmp    word ptr [esi + 16h], 6eh
        jb     SHORT loc_1_BCD7
        push   8
        push   esi
        call   sub_1_BCDF
        add    esp, 8
        mov    ebx, eax
        mov    eax, ebx
        test   ebx, ebx
        jne    SHORT loc_1_BCD9
loc_1_BCD7:
        mov    eax, ebx
loc_1_BCD9:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_BCDF:
        push   esi
        push   3
        mov    esi, dword ptr [esp + 0ch]
        push   esi
        call   sub_1_6CF2
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_BD45
        push   11h
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_BD45
        push   0
        push   3
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        mov    al, byte ptr [esp + 18h]
        add    esp, 0ch
        add    al, 2
        push   eax
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_BD45
        push   0
        push   3
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        push   0
        push   esi
        call   sub_1_6CF2
        add    esp, 8
loc_1_BD45:
        pop    esi
        ret
        db  0CCh
loc_1_BD48:
        dec    esp
        push   eax
        push   esp
loc_1_BD4B:
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BD6F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BD9D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BE65:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BF81:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BF95:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_C030:
        inc    ebx
        insb
        popad
        db  073h, 073h
        add    byte ptr [eax], al
        add    byte ptr [eax + 6fh], dh
        db  072h, 074h
        jae    SHORT loc_1_C03E
loc_1_C03E:
        add    byte ptr [eax], al
loc_1_C040:
        push   eax
        outsd
        db  072h, 074h
        push   ebx
        db  075h, 062h
        inc    ebx
        insb
        popad
        db  073h, 073h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    ebx
        dec    edi
        dec    ebp
        push   eax
        inc    ecx
        push   ecx
        add    byte ptr [eax], al
loc_1_C058:
        inc    ebx
        dec    edi
        dec    ebp
        push   eax
        inc    ecx
        push   ecx
        add    byte ptr [eax], al
loc_1_C060:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
