; socket_lcode_01.asm -- SOCKET object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 18h], 0
        lea    eax, [ebp - 20h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_2550
        cmp    dword ptr [ebp - 20h], 0
        jne    SHORT loc_1_0036
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_0036:
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [ebp - 10h]
        mov    ebx, dword ptr [esi + 53h]
        shl    ebx, 8
        add    ebx, dword ptr [esi + 0f1h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_2574
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_1_0088
        sub    edi, edi
        mov    edi, edi
loc_1_0060:
        cmp    dword ptr [ebp - 10h], edi
        jne    SHORT loc_1_0088
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_2598
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_2574
        cmp    dword ptr [ebp - 0ch], edi
        jne    SHORT loc_1_0060
loc_1_0088:
        mov    eax, ebx
        and    eax, 1
        mov    dword ptr [ebp - 28h], eax
        lea    ecx, [eax + ebx]
        mov    di, word ptr [esi + 103h]
        mov    dx, di
        sub    al, al
        out    dx, al
        lea    edx, [edi + 8]
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 9]
        mov    al, ch
        out    dx, al
        lea    edx, [edi + 0ah]
        mov    al, 2
        out    dx, al
        lea    edx, [edi + 0bh]
        sub    al, al
        out    dx, al
        cmp    byte ptr [esi + 120h], 0
        mov    dx, di
        mov    al, 0ah
        out    dx, al
        lea    edx, [edi + 10h]
        je     SHORT loc_1_00E0
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [esi + 103h]
        add    dx, 10h
        in     al, dx
        mov    byte ptr [ebp - 2], al
        jmp    SHORT loc_1_00E6
        db  08Bh, 0FFh
loc_1_00E0:
        in     ax, dx
        mov    word ptr [ebp - 6], ax
loc_1_00E6:
        mov    word ptr [ebp - 6], 0ffh
        lea    esp, [esp]
loc_1_00F0:
        mov    edx, dword ptr [esi + 103h]
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    di, ax
        and    edi, 0ffff00ffh
        mov    edx, dword ptr [esi + 103h]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 2], al
        and    ax, 0ffh
        shl    ax, 8
        or     di, ax
        cmp    di, cx
        jne    SHORT loc_1_0151
        dec    word ptr [ebp - 6]
        jne    SHORT loc_1_00F0
        cmp    di, cx
        jne    SHORT loc_1_0151
        push   ebx
        push   2
        push   2
        push   0aa000003h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_25F8
        add    esp, 14h
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_0151:
        mov    cx, word ptr [esi + 103h]
        cmp    byte ptr [esi + 120h], 0
        mov    dx, cx
        je     SHORT loc_1_0177
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 8]
        mov    al, bl
        out    dx, al
        lea    edx, [ecx + 9]
        mov    al, bh
        jmp    NEAR PTR loc_1_0236
loc_1_0177:
        sub    al, al
        out    dx, al
        cmp    dword ptr [ebp - 28h], 0
        je     NEAR PTR loc_1_0224
        lea    edx, [ecx + 8]
        lea    eax, [ebx - 1]
        out    dx, al
        mov    byte ptr [ebp - 24h], ah
        lea    edx, [ecx + 9]
        mov    al, ah
        out    dx, al
        lea    edx, [ecx + 0ah]
        mov    al, 1
        out    dx, al
        lea    edx, [ecx + 0bh]
        sub    al, al
        out    dx, al
        mov    dx, cx
        mov    al, 0ah
        out    dx, al
        lea    edx, [ecx + 10h]
        in     al, dx
        mov    byte ptr [ebp - 3], al
        mov    cx, word ptr [esi + 103h]
        lea    eax, [ebx - 1]
        lea    edx, [ecx + 8]
        out    dx, al
        lea    edx, [ecx + 9]
        mov    al, byte ptr [ebp - 24h]
        out    dx, al
        lea    edx, [ecx + 0ah]
        mov    al, 2
        out    dx, al
        lea    edx, [ecx + 0bh]
        sub    al, al
        out    dx, al
        mov    dx, cx
        mov    al, 12h
        out    dx, al
        mov    eax, dword ptr [ebp - 14h]
        lea    edx, [ecx + 10h]
        mov    al, byte ptr [eax]
        and    eax, 0ffff00ffh
        shl    ax, 8
        mov    cl, byte ptr [ebp - 3]
        and    ecx, 0ffff00ffh
        or     ax, cx
        out    dx, ax
        mov    di, 0ffh
loc_1_01F7:
        mov    edx, dword ptr [esi + 103h]
        add    dx, 7
        in     al, dx
        mov    byte ptr [ebp - 3], al
        test   al, 40h
        jne    SHORT loc_1_021A
        dec    di
        push   3e8h
        call   sub_1_2520
        or     di, di
        jne    SHORT loc_1_01F7
loc_1_021A:
        inc    dword ptr [ebp - 14h]
        inc    ebx
        dec    dword ptr [ebp - 20h]
        dec    dword ptr [ebp - 10h]
loc_1_0224:
        mov    cx, word ptr [esi + 103h]
        mov    eax, ebx
        lea    edx, [ecx + 8]
        out    dx, al
        lea    edx, [ecx + 9]
        mov    al, ah
loc_1_0236:
        out    dx, al
        lea    edx, [ecx + 0ah]
        mov    al, byte ptr [ebp - 20h]
        out    dx, al
        lea    edx, [ecx + 0bh]
        mov    al, byte ptr [ebp - 1fh]
        out    dx, al
        mov    dx, cx
        mov    al, 12h
        out    dx, al
        sub    ebx, ebx
loc_1_024D:
        cmp    dword ptr [ebp - 18h], ebx
        je     SHORT loc_1_0283
        mov    edx, dword ptr [esi + 103h]
        mov    eax, dword ptr [ebp - 14h]
        add    dx, 10h
        mov    al, byte ptr [eax]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp - 1ch]
        shl    ax, 8
        mov    cl, byte ptr [ecx]
        and    ecx, 0ffff00ffh
        inc    dword ptr [ebp - 14h]
        or     ax, cx
        dec    dword ptr [ebp - 10h]
        out    dx, ax
        mov    dword ptr [ebp - 18h], ebx
loc_1_0283:
        cmp    byte ptr [esi + 120h], bl
        mov    eax, dword ptr [ebp - 10h]
        je     SHORT loc_1_02A4
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [esi + 103h]
        add    eax, 10h
        push   eax
        call   sub_1_23C0
        jmp    SHORT loc_1_02D4
loc_1_02A4:
        shr    eax, 1
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [esi + 103h]
        add    eax, 10h
        push   eax
        call   sub_1_23D4
        test   byte ptr [ebp - 10h], 1
        je     SHORT loc_1_02D4
        mov    ecx, dword ptr [ebp - 10h]
        mov    eax, dword ptr [ebp - 14h]
        lea    ecx, [ecx + eax - 1]
        mov    dword ptr [ebp - 1ch], ecx
        mov    dword ptr [ebp - 18h], 1
loc_1_02D4:
        mov    edi, 0ffh
        lea    esp, [esp]
loc_1_02E0:
        mov    edx, dword ptr [esi + 103h]
        add    dx, 7
        in     al, dx
        test   al, 40h
        jne    SHORT loc_1_02FE
        dec    edi
        push   3e8h
        call   sub_1_2520
        cmp    edi, ebx
        jne    SHORT loc_1_02E0
loc_1_02FE:
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_2598
        cmp    dword ptr [ebp - 0ch], ebx
        je     SHORT loc_1_0353
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_2574
        cmp    dword ptr [ebp - 0ch], ebx
        je     SHORT loc_1_035C
loc_1_0326:
        cmp    dword ptr [ebp - 10h], ebx
        jne    SHORT loc_1_0353
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_2598
        cmp    dword ptr [ebp - 0ch], ebx
        je     SHORT loc_1_0353
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_2574
        cmp    dword ptr [ebp - 0ch], ebx
        jne    SHORT loc_1_0326
loc_1_0353:
        cmp    dword ptr [ebp - 0ch], ebx
        jne    NEAR PTR loc_1_024D
loc_1_035C:
        cmp    dword ptr [ebp - 18h], 0
        je     SHORT loc_1_0372
        mov    edx, dword ptr [esi + 103h]
        mov    ecx, dword ptr [ebp - 1ch]
        add    dx, 10h
        mov    al, byte ptr [ecx]
        out    dx, al
loc_1_0372:
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 20h]
        mov    dword ptr [eax], ecx
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0390:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 14h], 0
        jne    SHORT loc_1_03AA
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_03AA:
        mov    ebx, dword ptr [ebp + 0ch]
        and    ebx, 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        lea    ecx, [ebx + eax]
        mov    di, word ptr [esi + 103h]
        mov    dx, di
        sub    al, al
        out    dx, al
        lea    edx, [edi + 8]
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 9]
        mov    al, ch
        out    dx, al
        lea    edx, [edi + 0ah]
        mov    al, 2
        out    dx, al
        lea    edx, [edi + 0bh]
        sub    al, al
        out    dx, al
        cmp    byte ptr [esi + 120h], 0
        mov    dx, di
        mov    al, 0ah
        out    dx, al
        lea    edx, [edi + 10h]
        je     SHORT loc_1_0404
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    edx, dword ptr [esi + 103h]
        add    dx, 10h
        in     al, dx
        mov    byte ptr [ebp - 1], al
        jmp    SHORT loc_1_040A
loc_1_0404:
        in     ax, dx
        mov    word ptr [ebp - 4], ax
loc_1_040A:
        mov    word ptr [ebp - 4], 0ffh
loc_1_0410:
        mov    edx, dword ptr [esi + 103h]
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    di, ax
        and    edi, 0ffff00ffh
        mov    edx, dword ptr [esi + 103h]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    ax, 0ffh
        shl    ax, 8
        or     di, ax
        cmp    cx, di
        jne    SHORT loc_1_0460
        dec    word ptr [ebp - 4]
        jne    SHORT loc_1_0410
        cmp    cx, di
        jne    SHORT loc_1_0460
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h
loc_1_0460:
        cmp    byte ptr [esi + 120h], 0
        je     SHORT loc_1_04B0
        mov    ecx, dword ptr [esi + 103h]
        mov    dx, cx
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 8]
        mov    al, byte ptr [ebp + 0ch]
        out    dx, al
        lea    edx, [ecx + 9]
        mov    al, byte ptr [ebp + 0dh]
        out    dx, al
        lea    edx, [ecx + 0ah]
        mov    al, byte ptr [ebp + 14h]
        out    dx, al
        lea    edx, [ecx + 0bh]
        mov    al, byte ptr [ebp + 15h]
        out    dx, al
        mov    dx, cx
        mov    al, 12h
        out    dx, al
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    edi, dword ptr [ebp + 10h]
        push   edi
        add    ecx, 10h
        push   ecx
        call   sub_1_23C0
        jmp    NEAR PTR loc_1_05C2
        db  08Dh, 049h, 000h
loc_1_04B0:
        mov    di, word ptr [esi + 103h]
        mov    dx, di
        sub    al, al
        out    dx, al
        or     ebx, ebx
        je     NEAR PTR loc_1_0570
        mov    eax, dword ptr [ebp + 0ch]
        lea    edx, [edi + 8]
        lea    ecx, [eax - 1]
        mov    al, cl
        out    dx, al
        mov    bl, ch
        lea    edx, [edi + 9]
        mov    al, bl
        out    dx, al
        lea    edx, [edi + 0ah]
        mov    al, 1
        out    dx, al
        lea    edx, [edi + 0bh]
        sub    al, al
        out    dx, al
        mov    dx, di
        mov    al, 0ah
        out    dx, al
        lea    edx, [edi + 10h]
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    di, word ptr [esi + 103h]
        mov    al, cl
        lea    edx, [edi + 8]
        out    dx, al
        lea    edx, [edi + 9]
        mov    al, bl
        out    dx, al
        lea    edx, [edi + 0ah]
        mov    al, 2
        out    dx, al
        lea    edx, [edi + 0bh]
        sub    al, al
        out    dx, al
        mov    dx, di
        mov    al, 12h
        out    dx, al
        lea    edx, [edi + 10h]
        mov    edi, dword ptr [ebp + 10h]
        mov    cl, byte ptr [ebp - 2]
        mov    al, byte ptr [edi]
        and    eax, 0ffff00ffh
        shl    ax, 8
        and    ecx, 0ffff00ffh
        or     ax, cx
        out    dx, ax
        mov    bx, 0ffh
        lea    ebx, [ebx]
loc_1_0540:
        mov    edx, dword ptr [esi + 103h]
        add    dx, 7
        in     al, dx
        mov    byte ptr [ebp - 2], al
        test   al, 40h
        jne    SHORT loc_1_0563
        dec    bx
        push   3e8h
        call   sub_1_2520
        or     bx, bx
        jne    SHORT loc_1_0540
loc_1_0563:
        inc    edi
        inc    dword ptr [ebp + 0ch]
        dec    dword ptr [ebp + 14h]
        jmp    SHORT loc_1_0573
        db  08Dh, 064h, 024h, 000h
loc_1_0570:
        mov    edi, dword ptr [ebp + 10h]
loc_1_0573:
        mov    ecx, dword ptr [esi + 103h]
        mov    eax, dword ptr [ebp + 0ch]
        lea    edx, [ecx + 8]
        out    dx, al
        lea    edx, [ecx + 9]
        mov    al, ah
        out    dx, al
        lea    edx, [ecx + 0ah]
        mov    al, byte ptr [ebp + 14h]
        out    dx, al
        lea    edx, [ecx + 0bh]
        mov    al, byte ptr [ebp + 15h]
        out    dx, al
        mov    dx, cx
        mov    al, 12h
        out    dx, al
        mov    eax, dword ptr [ebp + 14h]
        shr    eax, 1
        push   eax
        push   edi
        add    ecx, 10h
        push   ecx
        call   sub_1_23D4
        test   byte ptr [ebp + 14h], 1
        je     SHORT loc_1_05C2
        mov    edx, dword ptr [esi + 103h]
        add    dx, 10h
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [edi + eax - 1]
        out    dx, al
loc_1_05C2:
        mov    di, 0ffh
        mov    ebx, 40h
        add    eax, 0
loc_1_05D0:
        mov    edx, dword ptr [esi + 103h]
        add    dx, 7
        in     al, dx
        mov    byte ptr [ebp - 1], al
        test   bl, al
        jne    SHORT loc_1_05F3
        dec    di
        push   3e8h
        call   sub_1_2520
        or     di, di
        jne    SHORT loc_1_05D0
loc_1_05F3:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Bh, 0FFh
sub_1_0600:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 14h], 0
        jne    SHORT loc_1_0620
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0620:
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi + 103h]
        in     al, dx
        cmp    byte ptr [esi + 120h], 0
        je     SHORT loc_1_0677
        mov    ebx, dword ptr [esi + 103h]
        mov    dx, bx
        sub    al, al
        out    dx, al
        mov    eax, dword ptr [ebp + 10h]
        lea    edx, [ebx + 8]
        out    dx, al
        lea    edx, [ebx + 9]
        mov    al, ah
        out    dx, al
        lea    edx, [ebx + 0ah]
        mov    al, byte ptr [ebp + 14h]
        out    dx, al
        lea    edx, [ebx + 0bh]
        mov    al, byte ptr [ebp + 15h]
        out    dx, al
        mov    dx, bx
        mov    al, 0ah
        out    dx, al
        mov    ecx, dword ptr [ebp + 14h]
        push   ecx
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        add    ebx, 10h
        push   ebx
        call   sub_1_23FD
        jmp    NEAR PTR loc_1_0732
loc_1_0677:
        mov    cx, word ptr [esi + 103h]
        mov    dx, cx
        sub    al, al
        out    dx, al
        test   byte ptr [ebp + 10h], 1
        je     SHORT loc_1_06E0
        mov    eax, dword ptr [ebp + 10h]
        dec    eax
        lea    edx, [ecx + 8]
        out    dx, al
        lea    edx, [ecx + 9]
        mov    al, ah
        out    dx, al
        lea    edx, [ecx + 0ah]
        mov    al, 2
        out    dx, al
        lea    edx, [ecx + 0bh]
        sub    al, al
        out    dx, al
        mov    dx, cx
        mov    al, 0ah
        out    dx, al
        lea    edx, [ecx + 10h]
        mov    edi, dword ptr [ebp + 0ch]
        in     ax, dx
        mov    byte ptr [edi], ah
        mov    bl, 0ffh
loc_1_06B6:
        mov    edx, dword ptr [esi + 103h]
        add    dx, 7
        in     al, dx
        test   al, 40h
        jne    SHORT loc_1_06D5
        dec    bl
        push   3e8h
        call   sub_1_2520
        or     bl, bl
        jne    SHORT loc_1_06B6
loc_1_06D5:
        inc    dword ptr [ebp + 10h]
        inc    edi
        dec    dword ptr [ebp + 14h]
        jmp    SHORT loc_1_06E3
        db  08Bh, 0FFh
loc_1_06E0:
        mov    edi, dword ptr [ebp + 0ch]
loc_1_06E3:
        mov    ecx, dword ptr [esi + 103h]
        mov    eax, dword ptr [ebp + 10h]
        lea    edx, [ecx + 8]
        out    dx, al
        lea    edx, [ecx + 9]
        mov    al, ah
        out    dx, al
        lea    edx, [ecx + 0ah]
        mov    al, byte ptr [ebp + 14h]
        out    dx, al
        lea    edx, [ecx + 0bh]
        mov    al, byte ptr [ebp + 15h]
        out    dx, al
        mov    dx, cx
        mov    al, 0ah
        out    dx, al
        mov    eax, dword ptr [ebp + 14h]
        shr    eax, 1
        push   eax
        push   edi
        add    ecx, 10h
        push   ecx
        call   sub_1_2411
        test   byte ptr [ebp + 14h], 1
        je     SHORT loc_1_0732
        mov    edx, dword ptr [esi + 103h]
        add    dx, 10h
        in     al, dx
        mov    ecx, dword ptr [ebp + 14h]
        mov    byte ptr [edi + ecx - 1], al
loc_1_0732:
        mov    bl, 0ffh
loc_1_0734:
        mov    edx, dword ptr [esi + 103h]
        add    dx, 7
        in     al, dx
        test   al, 40h
        jne    SHORT loc_1_0753
        dec    bl
        push   3e8h
        call   sub_1_2520
        or     bl, bl
        jne    SHORT loc_1_0734
loc_1_0753:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Bh, 0FFh
sub_1_0760:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dx, word ptr [eax + 103h]
        mov    al, 21h
        out    dx, al
        sub    al, al
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0780:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edi + 5bh]
        mov    si, word ptr [edi + 103h]
        mov    ebx, dword ptr [edi + ecx*4 + 0c1h]
        lea    edx, [esi + 4]
        mov    al, byte ptr [edi + 0fdh]
        add    al, cl
        out    dx, al
        cmp    ebx, 3ch
        jae    SHORT loc_1_07B3
        mov    ebx, 3ch
loc_1_07B3:
        lea    edx, [esi + 6]
        mov    al, bh
        out    dx, al
        lea    edx, [esi + 5]
        mov    al, bl
        out    dx, al
        mov    dx, si
        in     al, dx
        mov    dx, word ptr [edi + 103h]
        mov    al, 26h
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_07E0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    al, 62h
        mov    dx, word ptr [ecx + 103h]
        out    dx, al
        add    dx, 7
        in     al, dx
        mov    byte ptr [ecx + 101h], al
        mov    dx, word ptr [ecx + 103h]
        mov    al, 22h
        out    dx, al
        sub    al, al
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_0810:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    edx, dword ptr [ecx + 103h]
        add    dx, 4
        in     al, dx
        mov    byte ptr [ecx + 102h], al
        sub    al, al
        ret    4
        db  08Dh, 049h, 000h
sub_1_0830:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx + 100h]
        cmp    byte ptr [ecx + 0feh], al
        mov    edx, dword ptr [ecx + 103h]
        jne    SHORT loc_1_0860
        add    dx, 3
        mov    al, byte ptr [ecx + 0ffh]
        dec    al
        out    dx, al
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_0860:
        add    dx, 3
        dec    al
        out    dx, al
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_0870:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    edx, dword ptr [eax + 103h]
        add    dx, 0ch
        mov    al, byte ptr [eax + 15fh]
        out    dx, al
        sub    al, al
        ret    4
        db  08Dh, 049h, 000h
loc_1_0890:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 107h], 4
        je     SHORT loc_1_08A5
        push   eax
        call   sub_1_08B0
loc_1_08A5:
        sub    al, al
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_08B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    edx, dword ptr [esi + 103h]
        add    dx, 0dh
        in     al, dx
        mov    cl, al
        mov    edx, dword ptr [esi + 103h]
        add    dword ptr [esi + 136h], ecx
        add    dx, 0eh
        in     al, dx
        xor    ecx, ecx
        mov    cl, al
        mov    edx, dword ptr [esi + 103h]
        add    dword ptr [esi + 13ah], ecx
        add    dx, 0fh
        in     al, dx
        and    eax, 0ffh
        add    dword ptr [esi + 13eh], eax
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0910:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0760
        push   7d0h
        call   sub_1_2520
        test   byte ptr [esi + 107h], 0ah
        jne    SHORT loc_1_094F
        mov    edx, dword ptr [esi + 103h]
        add    dx, 7
        in     al, dx
        test   al, 0ah
        jne    SHORT loc_1_094F
        mov    al, byte ptr [esi + 5fh]
        mov    byte ptr [esi + 60h], al
        mov    byte ptr [esi + 1b5h], 0
loc_1_094F:
        mov    byte ptr [esi + 5fh], 0
        mov    cx, word ptr [esi + 103h]
        mov    dword ptr [esi + 14bh], 0
        lea    edx, [ecx + 0bh]
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 0ah]
        out    dx, al
        lea    edx, [ecx + 0dh]
        mov    al, 2
        out    dx, al
        mov    dx, cx
        mov    al, 22h
        out    dx, al
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0990:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    edx, dword ptr [eax + 103h]
        add    dx, 0fh
        sub    al, al
        out    dx, al
        mov    al, 1
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_09B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 121h], 0
        jne    NEAR PTR loc_1_0CE0
        mov    byte ptr [esi + 121h], 1
        mov    edx, dword ptr [esi + 103h]
        inc    dword ptr [esi + 30h]
        add    dx, 7
        in     al, dx
        mov    byte ptr [esi + 107h], al
        cmp    al, 0ffh
        jne    SHORT loc_1_0A2D
        mov    edx, dword ptr [esi + 103h]
        add    dx, 7
        sub    al, al
        out    dx, al
        in     al, dx
        mov    byte ptr [ebp - 2], al
        cmp    al, 0ffh
        jne    SHORT loc_1_0A20
        mov    byte ptr [esi + 107h], 0
        lea    eax, [ebp - 3]
        push   eax
        lea    eax, [esi + 1a9h]
        push   eax
        call   sub_1_2508
        mov    dword ptr [esi + 2bfh], 1
        jmp    SHORT loc_1_0A2D
        db  090h
loc_1_0A20:
        mov    edx, dword ptr [esi + 103h]
        add    dx, 7
        mov    al, 0ffh
        out    dx, al
loc_1_0A2D:
        mov    al, byte ptr [esi + 107h]
        or     al, al
        je     SHORT loc_1_0A42
        mov    edx, dword ptr [esi + 103h]
        add    dx, 7
        out    dx, al
loc_1_0A42:
        test   al, 20h
        je     SHORT loc_1_0A50
        mov    eax, 8
        jmp    SHORT loc_1_0A86
        db  08Dh, 049h, 000h
loc_1_0A50:
        test   al, 10h
        je     SHORT loc_1_0A61
        push   esi
        call   sub_1_08B0
        mov    eax, 4
        jmp    SHORT loc_1_0A86
loc_1_0A61:
        test   al, 0ah
        je     SHORT loc_1_0A6C
        mov    eax, 2
        jmp    SHORT loc_1_0A86
loc_1_0A6C:
        test   al, 1
        je     SHORT loc_1_0A72
        jmp    SHORT loc_1_0A81
loc_1_0A72:
        test   al, 4
        mov    eax, 10h
        je     SHORT loc_1_0A86
        push   esi
        call   sub_1_08B0
loc_1_0A81:
        mov    eax, 1
loc_1_0A86:
        sub    ebx, ebx
loc_1_0A88:
        cmp    eax, 10h
        jne    SHORT loc_1_0AA5
        cmp    dword ptr [esi + 165h], ebx
        je     NEAR PTR loc_1_0BB0
        cmp    byte ptr [esi + 142h], bl
        jne    NEAR PTR loc_1_0BB0
loc_1_0AA5:
        cmp    eax, 1
        je     SHORT loc_1_0B12
        cmp    eax, 2
        je     SHORT loc_1_0AC0
        cmp    eax, 4
        je     SHORT loc_1_0B04
        cmp    eax, 8
        je     NEAR PTR loc_1_0B53
        jmp    SHORT loc_1_0B29
        db  090h
loc_1_0AC0:
        push   esi
        call   sub_1_0810
        mov    byte ptr [esi + 1b5h], bl
        cmp    byte ptr [esi + 11fh], bl
        jne    SHORT loc_1_0AFB
        mov    byte ptr [esi + 5fh], bl
        mov    dword ptr [esi + 14bh], ebx
        test   byte ptr [esi + 107h], 8
        je     SHORT loc_1_0AEC
        push   esi
        db  0E8h, 004h, 0FEh, 0FEh, 0FFh
loc_1_0AEC:
        test   byte ptr [esi + 107h], 2
        je     SHORT loc_1_0AFB
        push   esi
        call   sub_1_0F40
loc_1_0AFB:
        and    byte ptr [esi + 107h], 0f5h
        jmp    SHORT loc_1_0B29
loc_1_0B04:
        mov    byte ptr [esi + 11ch], 1
        and    byte ptr [esi + 107h], 0efh
loc_1_0B12:
        push   esi
        call   sub_1_0CF0
        cmp    al, bl
        je     SHORT loc_1_0B23
        and    byte ptr [esi + 107h], 0fah
loc_1_0B23:
        push   esi
        call   sub_1_1B40
loc_1_0B29:
        cmp    dword ptr [esi + 165h], ebx
        je     SHORT loc_1_0B3F
        cmp    byte ptr [esi + 142h], bl
        jne    SHORT loc_1_0B3F
        push   esi
        call   sub_1_0CF0
loc_1_0B3F:
        mov    al, byte ptr [esi + 107h]
        test   al, 20h
        je     SHORT loc_1_0B62
        mov    eax, 8
        jmp    NEAR PTR loc_1_0A88
loc_1_0B53:
        push   esi
        call   sub_1_08B0
        and    byte ptr [esi + 107h], 0dfh
        jmp    SHORT loc_1_0B29
loc_1_0B62:
        test   al, 10h
        je     SHORT loc_1_0B76
        push   esi
        call   sub_1_08B0
        mov    eax, 4
        jmp    NEAR PTR loc_1_0A88
loc_1_0B76:
        test   al, 0ah
        je     SHORT loc_1_0B84
        mov    eax, 2
        jmp    NEAR PTR loc_1_0A88
loc_1_0B84:
        test   al, 1
        je     SHORT loc_1_0B92
        mov    eax, 1
        jmp    NEAR PTR loc_1_0A88
loc_1_0B92:
        test   al, 4
        je     SHORT loc_1_0BA6
        push   esi
        call   sub_1_08B0
        mov    eax, 1
        jmp    NEAR PTR loc_1_0A88
loc_1_0BA6:
        mov    eax, 10h
        jmp    NEAR PTR loc_1_0A88
loc_1_0BB0:
        mov    edx, dword ptr [esi + 103h]
        add    dx, 7
        in     al, dx
        mov    byte ptr [esi + 107h], al
        cmp    al, 0ffh
        jne    SHORT loc_1_0C0D
        mov    edx, dword ptr [esi + 103h]
        add    dx, 7
        mov    al, bl
        out    dx, al
        in     al, dx
        mov    byte ptr [ebp - 1], al
        cmp    al, 0ffh
        jne    SHORT loc_1_0C00
        mov    byte ptr [esi + 107h], bl
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [esi + 1a9h]
        push   eax
        call   sub_1_2508
        mov    dword ptr [esi + 2bfh], 1
        jmp    SHORT loc_1_0C0D
        db  08Dh, 064h, 024h, 000h
loc_1_0C00:
        mov    edx, dword ptr [esi + 103h]
        add    dx, 7
        mov    al, 0ffh
        out    dx, al
loc_1_0C0D:
        mov    al, byte ptr [esi + 107h]
        cmp    al, bl
        je     SHORT loc_1_0C22
        mov    edx, dword ptr [esi + 103h]
        add    dx, 7
        out    dx, al
loc_1_0C22:
        test   al, 20h
        je     SHORT loc_1_0C30
        mov    eax, 8
        jmp    SHORT loc_1_0C66
        db  08Dh, 049h, 000h
loc_1_0C30:
        test   al, 10h
        je     SHORT loc_1_0C41
        push   esi
        call   sub_1_08B0
        mov    eax, 4
        jmp    SHORT loc_1_0C66
loc_1_0C41:
        test   al, 0ah
        je     SHORT loc_1_0C4C
        mov    eax, 2
        jmp    SHORT loc_1_0C66
loc_1_0C4C:
        test   al, 1
        je     SHORT loc_1_0C52
        jmp    SHORT loc_1_0C61
loc_1_0C52:
        test   al, 4
        mov    eax, 10h
        je     SHORT loc_1_0C66
        push   esi
        call   sub_1_08B0
loc_1_0C61:
        mov    eax, 1
loc_1_0C66:
        cmp    eax, 10h
        jne    NEAR PTR loc_1_0AA5
        mov    byte ptr [esi + 121h], 0
        mov    byte ptr [esi + 160h], 1bh
        mov    edx, dword ptr [esi + 103h]
        add    dx, 0fh
        mov    al, 1bh
        out    dx, al
        mov    eax, dword ptr [esi + 30h]
        dec    eax
        mov    dword ptr [esi + 30h], eax
        jne    SHORT loc_1_0CC1
        cmp    byte ptr [esi + 142h], 0
        jne    SHORT loc_1_0CAB
        cmp    dword ptr [esi + 171h], 0
        jne    SHORT loc_1_0CAB
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_1_0CC1
loc_1_0CAB:
        push   1
        add    esi, 17dh
        push   esi
        call   sub_1_2502
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_0CC1:
        cmp    dword ptr [esi + 4bh], 0
        je     SHORT loc_1_0CE0
        mov    eax, dword ptr [esi + 53h]
        cmp    dword ptr [esi + eax*4 + 61h], 0
        jne    SHORT loc_1_0CE0
        cmp    byte ptr [esi + 121h], 0
        jne    SHORT loc_1_0CE0
        push   esi
        call   sub_1_1B40
loc_1_0CE0:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0CF0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], 1
        mov    byte ptr [esi + 11eh], 0
        push   esi
        call   sub_1_07E0
        cmp    byte ptr [esi + 142h], 0
        jne    SHORT loc_1_0D2E
        cmp    byte ptr [esi + 11ch], 0
        je     SHORT loc_1_0D2E
        push   esi
        db  068h, 010h, 009h, 000h, 000h
        lea    eax, [esi + 8]
        push   eax
        call   sub_1_251A
loc_1_0D2E:
        mov    ebx, 4
loc_1_0D33:
        cmp    byte ptr [esi + 142h], 0
        jne    NEAR PTR loc_1_0E29
        test   byte ptr [esi + 107h], bl
        jne    NEAR PTR loc_1_0EE0
        mov    al, byte ptr [esi + 100h]
        cmp    byte ptr [esi + 101h], al
        jne    SHORT loc_1_0D72
        push   esi
        call   sub_1_07E0
        mov    al, byte ptr [esi + 100h]
        cmp    byte ptr [esi + 101h], al
        je     NEAR PTR loc_1_0E29
loc_1_0D72:
        inc    dword ptr [esi + 34h]
        mov    dword ptr [esi + 16dh], 0
        push   esi
        call   sub_1_1440
        or     al, al
        je     SHORT loc_1_0DE1
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 1bah]
        shl    ecx, 8
        xor    eax, eax
        mov    al, byte ptr [esi + 1b9h]
        lea    eax, [ecx + eax - 4]
        mov    ecx, dword ptr [esi + 161h]
        cmp    ecx, eax
        jb     SHORT loc_1_0DAC
        mov    ecx, eax
loc_1_0DAC:
        add    ecx, 0eh
        push   ecx
        mov    eax, dword ptr [esi + 1bbh]
        push   eax
        lea    eax, [esi + 1bfh]
        push   eax
        push   esi
        call   sub_1_0600
        or     al, al
        jne    SHORT loc_1_0DCA
        jmp    SHORT loc_1_0DE1
loc_1_0DCA:
        push   esi
        call   sub_1_1600
        cmp    eax, 3
        je     NEAR PTR loc_1_0F00
        inc    dword ptr [esi + 126h]
        jmp    SHORT loc_1_0DE6
loc_1_0DE1:
        mov    eax, 1
loc_1_0DE6:
        cmp    eax, 3
        je     NEAR PTR loc_1_0F00
        cmp    eax, 1
        jne    SHORT loc_1_0E02
        push   esi
        call   sub_1_07E0
        mov    al, byte ptr [esi + 101h]
        jmp    SHORT loc_1_0E08
loc_1_0E02:
        mov    al, byte ptr [esi + 1b8h]
loc_1_0E08:
        mov    byte ptr [esi + 100h], al
        push   esi
        call   sub_1_0830
        cmp    dword ptr [esi + 34h], 0ah
        jbe    NEAR PTR loc_1_0D33
        mov    byte ptr [ebp - 1], 0
        mov    dword ptr [esi + 34h], 0
loc_1_0E29:
        cmp    byte ptr [esi + 11ch], 0
        je     SHORT loc_1_0E76
        mov    byte ptr [esi + 11ch], 0
        push   esi
        db  068h, 090h, 008h, 000h, 000h
        lea    eax, [esi + 8]
        push   eax
        call   sub_1_251A
        mov    eax, dword ptr [esi + 103h]
        add    ax, 0dh
        mov    dx, ax
        sub    al, al
        out    dx, al
        cmp    byte ptr [esi + 60h], al
        je     SHORT loc_1_0E76
        cmp    dword ptr [esi + 5bh], -1
        je     SHORT loc_1_0E76
        mov    byte ptr [esi + 60h], al
        mov    byte ptr [esi + 1b5h], al
        mov    byte ptr [esi + 5fh], 1
        push   esi
        call   sub_1_0780
loc_1_0E76:
        cmp    dword ptr [esi + 165h], 0
        je     SHORT loc_1_0ECA
        nop
loc_1_0E80:
        cmp    byte ptr [esi + 142h], 0
        jne    SHORT loc_1_0ED3
        mov    ebx, dword ptr [esi + 165h]
        mov    eax, dword ptr [ebx + 24h]
        mov    dword ptr [esi + 165h], eax
        mov    dword ptr [esi + 16dh], ebx
        mov    eax, ebx
        inc    dword ptr [esi + 122h]
        mov    edi, dword ptr [ebx + 28h]
        push   eax
        push   esi
        call   sub_1_12B0
        push   0
        mov    eax, dword ptr [ebx + 28h]
        push   ebx
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_246A
        dec    dword ptr [edi + 1ch]
        cmp    dword ptr [esi + 165h], 0
        jne    SHORT loc_1_0E80
loc_1_0ECA:
        cmp    byte ptr [esi + 142h], 0
        je     SHORT loc_1_0F12
loc_1_0ED3:
        mov    al, byte ptr [ebp - 1]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_0EE0:
        push   esi
        call   sub_1_07E0
        mov    al, byte ptr [esi + 101h]
        mov    byte ptr [esi + 100h], al
        push   esi
        call   sub_1_0830
        jmp    NEAR PTR loc_1_0E29
        db  08Dh, 049h, 000h
loc_1_0F00:
        mov    byte ptr [esi + 160h], 1ah
        push   esi
        db  0E8h, 0E3h, 0F7h, 0FEh, 0FFh
        jmp    NEAR PTR loc_1_0E29
loc_1_0F12:
        cmp    byte ptr [esi + 11eh], 0
        je     SHORT loc_1_0F2E
        mov    eax, dword ptr [esi + 14fh]
        push   eax
        call   sub_1_22B0
        mov    byte ptr [esi + 11eh], 0
loc_1_0F2E:
        mov    al, byte ptr [ebp - 1]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0F40:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 11fh], 1
        cmp    dword ptr [esi + 5bh], -1
        jne    SHORT loc_1_0F80
        mov    byte ptr [esi + 11fh], 0
        push   4
        push   1
        push   0aa000006h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_25F8
        add    esp, 10h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_0F80:
        push   esi
        call   sub_1_0810
        test   byte ptr [esi + 102h], 1
        je     SHORT loc_1_0F97
        inc    dword ptr [esi + 122h]
        jmp    SHORT loc_1_0F9D
loc_1_0F97:
        inc    dword ptr [esi + 12ah]
loc_1_0F9D:
        mov    eax, dword ptr [esi + 5bh]
        mov    eax, dword ptr [esi + eax*4 + 0c1h]
        add    eax, 0ffh
        shr    eax, 8
        mov    dword ptr [ebp - 4], eax
        mov    edx, dword ptr [esi + 5bh]
        add    eax, edx
        cmp    eax, edx
        jbe    SHORT loc_1_0FD4
        lea    eax, [esi + edx*4 + 61h]
        nop
loc_1_0FC0:
        mov    dword ptr [eax], 0
        add    eax, 4
        inc    edx
        mov    ecx, dword ptr [esi + 5bh]
        add    ecx, dword ptr [ebp - 4]
        cmp    ecx, edx
        ja     SHORT loc_1_0FC0
loc_1_0FD4:
        mov    eax, dword ptr [esi + 57h]
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 57h], eax
        cmp    eax, 0ch
        jne    SHORT loc_1_0FE9
        mov    dword ptr [esi + 57h], 0
loc_1_0FE9:
        mov    eax, dword ptr [esi + 57h]
        cmp    dword ptr [esi + eax*4 + 61h], 0
        jne    SHORT loc_1_0FFF
        cmp    dword ptr [esi + 53h], eax
        je     SHORT loc_1_0FFF
        mov    dword ptr [esi + 57h], 0
loc_1_0FFF:
        mov    eax, dword ptr [esi + 5bh]
        mov    dword ptr [esi + eax*4 + 91h], 0
        mov    eax, dword ptr [esi + 57h]
        mov    ecx, dword ptr [esi + eax*4 + 61h]
        or     ecx, ecx
        je     SHORT loc_1_1040
        cmp    ecx, 1
        je     NEAR PTR loc_1_1230
        cmp    ecx, 2
        je     NEAR PTR loc_1_1250
        mov    byte ptr [esi + 11fh], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1040:
        cmp    byte ptr [esi + 142h], 0
        je     SHORT loc_1_1068
        mov    dword ptr [esi + 5bh], 0ffffffffh
        mov    byte ptr [esi + 11fh], 0
        push   2
        push   esi
        db  0E8h, 001h, 011h, 0FFh, 0FFh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1068:
        mov    ebx, dword ptr [esi + 4bh]
        or     ebx, ebx
        je     NEAR PTR loc_1_1215
        mov    eax, dword ptr [ebx + 24h]
        lea    edi, [ebx + 24h]
        mov    dword ptr [esi + 4bh], eax
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        push   0
        push   ebx
        call   sub_1_2550
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0ffh
        shr    eax, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [esi + 53h]
        add    eax, dword ptr [ebp - 4]
        cmp    eax, 0ch
        jbe    SHORT loc_1_10AC
        mov    dword ptr [esi + 53h], 0
loc_1_10AC:
        mov    eax, dword ptr [esi + 53h]
        mov    dword ptr [esi + 57h], eax
        mov    ecx, eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, ecx
        cmp    eax, ecx
        jbe    SHORT loc_1_10D5
        lea    eax, [esi + ecx*4 + 61h]
loc_1_10C1:
        mov    dword ptr [eax], 2
        add    eax, 4
        inc    ecx
        mov    edx, dword ptr [esi + 53h]
        add    edx, dword ptr [ebp - 4]
        cmp    edx, ecx
        ja     SHORT loc_1_10C1
loc_1_10D5:
        mov    eax, dword ptr [esi + 53h]
        mov    dword ptr [esi + eax*4 + 91h], ebx
        mov    eax, dword ptr [esi + 57h]
        mov    dword ptr [esi + 5bh], eax
        mov    eax, dword ptr [esi + 53h]
        lea    eax, [esi + eax*4]
        add    eax, 0c1h
        push   eax
        push   ebx
        push   esi
        call   0
        or     al, al
        jne    SHORT loc_1_1151
        push   0c0000001h
        mov    eax, dword ptr [ebx + 28h]
        push   ebx
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_246A
        mov    edx, dword ptr [esi + 53h]
        mov    eax, dword ptr [ebp - 4]
        add    eax, edx
        cmp    eax, edx
        jbe    SHORT loc_1_1134
        lea    eax, [esi + edx*4 + 61h]
loc_1_1120:
        mov    dword ptr [eax], 0
        add    eax, 4
        inc    edx
        mov    ecx, dword ptr [esi + 53h]
        add    ecx, dword ptr [ebp - 4]
        cmp    ecx, edx
        ja     SHORT loc_1_1120
loc_1_1134:
        mov    eax, dword ptr [esi + 53h]
        mov    dword ptr [esi + eax*4 + 91h], 0
        push   esi
        call   sub_1_1B40
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1151:
        mov    ecx, dword ptr [esi + 53h]
        mov    eax, dword ptr [esi + ecx*4 + 0c1h]
        cmp    eax, 3ch
        jae    SHORT loc_1_1181
        mov    edx, 3ch
        sub    edx, eax
        push   edx
        db  068h, 028h, 027h, 000h, 000h
        mov    edx, dword ptr [esi + 0f1h]
        shl    ecx, 8
        add    edx, ecx
        add    edx, eax
        push   edx
        push   esi
        call   sub_1_0390
loc_1_1181:
        mov    eax, dword ptr [esi + 53h]
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 53h], eax
        cmp    eax, 0ch
        jne    SHORT loc_1_1196
        mov    dword ptr [esi + 53h], 0
loc_1_1196:
        mov    byte ptr [esi + 11fh], 0
        cmp    byte ptr [esi + 11ch], 0
        je     SHORT loc_1_11B0
        mov    byte ptr [esi + 60h], 1
        jmp    SHORT loc_1_11BA
        db  08Dh, 064h, 024h, 000h
loc_1_11B0:
        mov    byte ptr [esi + 5fh], 1
        push   esi
        call   sub_1_0780
loc_1_11BA:
        cmp    byte ptr [edi + 8], 0
        jne    SHORT loc_1_11E0
        mov    edi, dword ptr [edi + 4]
        push   0
        push   ebx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_246A
        dec    dword ptr [edi + 1ch]
        push   esi
        call   sub_1_1B40
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_11E0:
        cmp    dword ptr [esi + 165h], 0
        jne    SHORT loc_1_11F1
        mov    dword ptr [esi + 165h], ebx
        jmp    SHORT loc_1_11FA
loc_1_11F1:
        mov    eax, dword ptr [esi + 169h]
        mov    dword ptr [eax + 24h], ebx
loc_1_11FA:
        mov    dword ptr [esi + 169h], ebx
        mov    dword ptr [edi], 0
        push   esi
        call   sub_1_1B40
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1215:
        mov    dword ptr [esi + 5bh], 0ffffffffh
        mov    byte ptr [esi + 11fh], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_1230:
        mov    dword ptr [esi + 5bh], 0ffffffffh
        mov    byte ptr [esi + 11fh], 0
        push   esi
        call   sub_1_1B40
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_1250:
        cmp    byte ptr [esi + 142h], 0
        je     SHORT loc_1_1283
        mov    dword ptr [esi + 5bh], 0ffffffffh
        mov    eax, dword ptr [esi + 57h]
        mov    dword ptr [esi + eax*4 + 61h], 0
        mov    byte ptr [esi + 11fh], 0
        push   2
        push   esi
        db  0E8h, 0E6h, 00Eh, 0FFh, 0FFh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1283:
        mov    dword ptr [esi + 5bh], eax
        mov    byte ptr [esi + 11fh], 0
        mov    byte ptr [esi + 5fh], 1
        push   esi
        call   sub_1_0780
        push   esi
        call   sub_1_1B40
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_12B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        push   0
        mov    ebx, dword ptr [ebp + 0ch]
        push   ebx
        call   sub_1_2550
        cmp    dword ptr [ebp - 4], 6
        jae    SHORT loc_1_12E0
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_12E0:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [esi + 161h]
        add    edi, 0eh
        cmp    edi, eax
        jb     SHORT loc_1_12F5
        mov    edi, eax
loc_1_12F5:
        push   edi
        push   0
        push   ebx
        lea    eax, [esi + 1bfh]
        push   eax
        call   sub_1_1360
        cmp    edi, 0eh
        jae    SHORT loc_1_1318
        cmp    edi, 6
        jb     SHORT loc_1_1341
        push   0
        push   0
        push   0
        push   edi
        jmp    SHORT loc_1_132C
loc_1_1318:
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0eh
        push   eax
        sub    edi, 0eh
        push   edi
        lea    eax, [esi + 1cdh]
        push   eax
        push   0eh
loc_1_132C:
        lea    eax, [esi + 1bfh]
        push   eax
        push   eax
        push   esi
        mov    eax, dword ptr [esi + 14fh]
        push   eax
        call   sub_1_20D0
loc_1_1341:
        mov    byte ptr [esi + 11eh], 1
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1360:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        push   esi
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        sub    edi, edi
        push   eax
        call   sub_1_2550
        cmp    dword ptr [ebp - 4], esi
        mov    ebx, dword ptr [ebp + 10h]
        je     SHORT loc_1_13B4
loc_1_1385:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2574
        mov    eax, dword ptr [ebp - 8]
        add    eax, edi
        cmp    ebx, eax
        jb     SHORT loc_1_13B4
        mov    edi, eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2598
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_1385
loc_1_13B4:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_13C5
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_13C5:
        mov    eax, ebx
        sub    eax, edi
        add    dword ptr [ebp - 0ch], eax
        sub    edi, ebx
        add    dword ptr [ebp - 8], edi
        mov    edi, dword ptr [ebp + 14h]
loc_1_13D4:
        mov    eax, dword ptr [ebp - 8]
        add    eax, esi
        cmp    eax, edi
        jbe    SHORT loc_1_13E3
        mov    ebx, edi
        sub    ebx, esi
        jmp    SHORT loc_1_13E6
loc_1_13E3:
        mov    ebx, dword ptr [ebp - 8]
loc_1_13E6:
        push   ebx
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, esi
        push   eax
        call   sub_1_260C
        add    esi, ebx
        cmp    esi, edi
        je     SHORT loc_1_1422
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2598
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_142D
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2574
        jmp    SHORT loc_1_13D4
loc_1_1422:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_142D:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_1440:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 0feh]
        mov    al, byte ptr [esi + 100h]
        sub    eax, ecx
        shl    eax, 8
        add    eax, dword ptr [esi + 0f5h]
        mov    dword ptr [ebp - 0ch], eax
        push   6
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        push   esi
        call   sub_1_0600
        or     al, al
        jne    SHORT loc_1_1486
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1486:
        add    dword ptr [ebp - 0ch], 4
        sub    ecx, ecx
        lea    edx, [ebp - 1ch]
        nop
loc_1_1490:
        lea    edi, [edx + 1]
        mov    dword ptr [ebp - 8], edi
        lea    edi, [edx + 2]
        mov    dword ptr [ebp - 10h], edi
        mov    bl, byte ptr [edx + 3]
        lea    eax, [edx + 3]
        mov    byte ptr [ebp - 2], bl
        lea    edi, [edx + 4]
        mov    byte ptr [ebp - 1], 0
        test   byte ptr [edx], 5eh
        je     SHORT loc_1_14B3
        jmp    SHORT loc_1_1527
loc_1_14B3:
        test   byte ptr [edx], 20h
        je     SHORT loc_1_14C3
        or     ecx, ecx
        jne    SHORT loc_1_14C3
        test   byte ptr [edi], 1
        jne    SHORT loc_1_14C3
        jmp    SHORT loc_1_1527
loc_1_14C3:
        mov    edi, dword ptr [ebp - 10h]
        cmp    byte ptr [edi], bl
        jne    SHORT loc_1_14E5
        mov    edi, dword ptr [ebp - 8]
        mov    byte ptr [ebp - 2], bl
        mov    bl, byte ptr [edi]
        sub    bl, byte ptr [esi + 100h]
        dec    bl
        mov    edi, dword ptr [ebp - 10h]
        mov    byte ptr [eax], bl
        cmp    byte ptr [edi], 0fch
        jbe    SHORT loc_1_14E5
        inc    eax
loc_1_14E5:
        xor    ebx, ebx
        mov    bl, byte ptr [eax]
        mov    edi, dword ptr [ebp - 10h]
        shl    ebx, 8
        movzx  edi, byte ptr [edi]
        lea    edi, [ebx + edi - 4]
        cmp    edi, 5eah
        ja     SHORT loc_1_1503
        cmp    edi, 3ch
        jae    SHORT loc_1_150C
loc_1_1503:
        mov    bl, byte ptr [ebp - 2]
        mov    byte ptr [eax], bl
        mov    byte ptr [ebp - 1], 1
loc_1_150C:
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_1_1531
        mov    edi, dword ptr [ebp - 8]
        mov    al, byte ptr [edi]
        cmp    byte ptr [esi + 0feh], al
        ja     SHORT loc_1_1527
        cmp    byte ptr [esi + 0ffh], al
        jae    SHORT loc_1_152B
loc_1_1527:
        mov    byte ptr [ebp - 1], 1
loc_1_152B:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_1567
loc_1_1531:
        or     ecx, ecx
        je     SHORT loc_1_1540
        cmp    ecx, 1
        je     SHORT loc_1_1555
        jmp    SHORT loc_1_1566
        db  08Dh, 064h, 024h, 000h
loc_1_1540:
        mov    edx, dword ptr [ebp - 8]
        inc    dword ptr [ebp - 0ch]
        cmp    byte ptr [esi + 120h], 0
        jne    SHORT loc_1_1566
        inc    edx
        inc    dword ptr [ebp - 0ch]
        jmp    SHORT loc_1_1566
loc_1_1555:
        dec    edx
        dec    dword ptr [ebp - 0ch]
        cmp    byte ptr [esi + 120h], 0
        jne    SHORT loc_1_1566
        dec    edx
        dec    dword ptr [ebp - 0ch]
loc_1_1566:
        inc    ecx
loc_1_1567:
        cmp    ecx, 2
        jae    SHORT loc_1_1576
        cmp    byte ptr [ebp - 1], 0
        jne    NEAR PTR loc_1_1490
loc_1_1576:
        mov    al, byte ptr [edx]
        mov    byte ptr [esi + 1b7h], al
        mov    al, byte ptr [edx + 1]
        inc    edx
        mov    byte ptr [esi + 1b8h], al
        mov    al, byte ptr [edx + 1]
        inc    edx
        mov    byte ptr [esi + 1b9h], al
        mov    al, byte ptr [edx + 1]
        mov    byte ptr [esi + 1bah], al
        mov    eax, dword ptr [ebp - 0ch]
        xor    ecx, ecx
        mov    dword ptr [esi + 1bbh], eax
        xor    eax, eax
        mov    al, byte ptr [esi + 1bah]
        shl    eax, 8
        mov    cl, byte ptr [esi + 1b9h]
        lea    eax, [eax + ecx - 4]
        cmp    eax, 5eah
        ja     SHORT loc_1_15D2
        cmp    eax, 3ch
        jb     SHORT loc_1_15D2
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_15D2:
        mov    al, byte ptr [esi + 1b8h]
        cmp    byte ptr [esi + 0feh], al
        ja     SHORT loc_1_15F3
        cmp    byte ptr [esi + 0ffh], al
        jb     SHORT loc_1_15F3
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_15F3:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_1600:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    bl, byte ptr [esi + 1bah]
        mov    al, byte ptr [esi + 100h]
        lea    ecx, [ebx + eax + 1]
        mov    dl, byte ptr [esi + 0ffh]
        cmp    cl, dl
        jb     SHORT loc_1_162A
        sub    cl, dl
        add    cl, byte ptr [esi + 0feh]
loc_1_162A:
        mov    al, byte ptr [esi + 1b8h]
        cmp    cl, al
        je     SHORT loc_1_1650
        inc    cl
        cmp    cl, dl
        jne    SHORT loc_1_1640
        mov    cl, byte ptr [esi + 0feh]
loc_1_1640:
        cmp    cl, al
        je     SHORT loc_1_1650
        mov    eax, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  090h
loc_1_1650:
        cmp    byte ptr [esi + 0feh], al
        ja     NEAR PTR loc_1_16E8
        cmp    al, dl
        ja     NEAR PTR loc_1_16E8
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 1b9h]
        xor    eax, eax
        mov    al, bl
        shl    eax, 8
        lea    eax, [ecx + eax - 4]
        cmp    eax, 5eah
        jbe    SHORT loc_1_1690
        mov    eax, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1690:
        mov    ecx, dword ptr [esi + 161h]
        add    ecx, 0eh
        cmp    ecx, eax
        jb     SHORT loc_1_169F
        mov    ecx, eax
loc_1_169F:
        mov    dword ptr [esi + 2bbh], eax
        cmp    ecx, 0eh
        jae    SHORT loc_1_16B3
        push   0
        push   0
        push   0
        push   ecx
        jmp    SHORT loc_1_16C4
loc_1_16B3:
        sub    eax, 0eh
        push   eax
        sub    ecx, 0eh
        push   ecx
        lea    eax, [esi + 1cdh]
        push   eax
        push   0eh
loc_1_16C4:
        lea    eax, [esi + 1bfh]
        push   eax
        push   eax
        push   esi
        mov    eax, dword ptr [esi + 14fh]
        push   eax
        call   sub_1_20D0
        mov    byte ptr [esi + 11eh], 1
        sub    eax, eax
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_16E8:
        mov    eax, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1700:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 0ch]
        add    edi, 0eh
        cmp    dword ptr [esi + 16dh], 0
        je     NEAR PTR loc_1_17A7
        push   0
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 18h]
        mov    ebx, edi
        push   eax
        call   sub_1_2550
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_177A
loc_1_173A:
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_2574
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        push   ebx
        mov    eax, dword ptr [esi + 16dh]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_1_1360
        add    ebx, eax
        cmp    eax, dword ptr [ebp - 0ch]
        jb     SHORT loc_1_177A
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_2598
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_173A
loc_1_177A:
        cmp    edi, ebx
        jbe    SHORT loc_1_1795
        mov    ecx, dword ptr [ebp + 1ch]
        mov    eax, 0c0000001h
        mov    dword ptr [ecx], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_1795:
        mov    ecx, dword ptr [ebp + 1ch]
        sub    ebx, edi
        mov    dword ptr [ecx], ebx
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_17A7:
        mov    eax, dword ptr [esi + 2bbh]
        mov    ecx, dword ptr [ebp + 14h]
        lea    edx, [edi + ecx]
        cmp    edx, eax
        jbe    SHORT loc_1_17D9
        cmp    edi, eax
        jbe    SHORT loc_1_17D2
        mov    ecx, dword ptr [ebp + 1ch]
        mov    eax, 0c0000001h
        mov    dword ptr [ecx], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_17D2:
        sub    eax, edi
        mov    dword ptr [ebp - 20h], eax
        jmp    SHORT loc_1_17DC
loc_1_17D9:
        mov    dword ptr [ebp - 20h], ecx
loc_1_17DC:
        mov    eax, dword ptr [ebp - 20h]
        mov    dword ptr [ebp - 14h], eax
        add    edi, dword ptr [esi + 1bbh]
        mov    eax, dword ptr [esi + 0f9h]
        cmp    edi, eax
        jbe    SHORT loc_1_17FA
        sub    edi, eax
        add    edi, dword ptr [esi + 0f5h]
loc_1_17FA:
        push   0
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        call   sub_1_2550
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_2574
        mov    dword ptr [ebp - 10h], 0
        cmp    dword ptr [ebp - 20h], 0
        je     NEAR PTR loc_1_18BB
        nop
loc_1_1830:
        mov    ebx, dword ptr [ebp - 0ch]
        sub    ebx, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp - 14h], ebx
        jae    SHORT loc_1_183E
        mov    ebx, dword ptr [ebp - 14h]
loc_1_183E:
        mov    eax, dword ptr [esi + 0f9h]
        lea    ecx, [ebx + edi]
        cmp    ecx, eax
        jbe    SHORT loc_1_184F
        mov    ebx, eax
        sub    ebx, edi
loc_1_184F:
        push   ebx
        push   edi
        mov    eax, dword ptr [ebp - 18h]
        add    eax, dword ptr [ebp - 10h]
        push   eax
        push   esi
        call   sub_1_0600
        or     al, al
        je     NEAR PTR loc_1_1931
        add    edi, ebx
        sub    dword ptr [ebp - 14h], ebx
        je     SHORT loc_1_18BB
        cmp    dword ptr [esi + 0f9h], edi
        jne    SHORT loc_1_187B
        mov    edi, dword ptr [esi + 0f5h]
loc_1_187B:
        add    dword ptr [ebp - 10h], ebx
        mov    eax, dword ptr [ebp - 10h]
        cmp    eax, dword ptr [ebp - 0ch]
        jne    SHORT loc_1_18B1
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_2598
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_18BB
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_2574
        mov    dword ptr [ebp - 10h], 0
loc_1_18B1:
        cmp    dword ptr [ebp - 14h], 0
        jne    NEAR PTR loc_1_1830
loc_1_18BB:
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp + 1ch]
        sub    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ecx], eax
        cmp    byte ptr [esi + 11ch], 0
        jne    NEAR PTR loc_1_1A07
        cmp    dword ptr [esi + 5bh], -1
        je     NEAR PTR loc_1_1A07
        mov    edx, dword ptr [esi + 103h]
        add    dx, 7
        in     al, dx
        mov    byte ptr [ebp - 1], al
        test   al, 8
        je     SHORT loc_1_18F5
        push   esi
        db  0E8h, 0FBh, 0EFh, 0FEh, 0FFh
loc_1_18F5:
        test   byte ptr [ebp - 1], 2
        je     NEAR PTR loc_1_1A07
        mov    edx, dword ptr [esi + 103h]
        add    dx, 7
        mov    al, 2
        out    dx, al
        push   esi
        call   sub_1_0810
        mov    byte ptr [esi + 5fh], 0
        mov    dword ptr [esi + 14bh], 0
        test   byte ptr [esi + 102h], 1
        je     SHORT loc_1_1960
        inc    dword ptr [esi + 122h]
        jmp    SHORT loc_1_1966
loc_1_1931:
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp + 1ch]
        sub    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ecx], eax
        push   2
        push   1
        push   0aa000003h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_25F8
        add    esp, 10h
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  090h
loc_1_1960:
        inc    dword ptr [esi + 12ah]
loc_1_1966:
        mov    edi, dword ptr [esi + 5bh]
        lea    ecx, [esi + edi*4]
        mov    edx, dword ptr [ecx + 0c1h]
        add    edx, 0ffh
        shr    edx, 8
        lea    eax, [edi + edx]
        cmp    eax, edi
        jbe    SHORT loc_1_1998
        add    ecx, 61h
loc_1_1985:
        mov    dword ptr [ecx], 0
        add    ecx, 4
        inc    edi
        mov    eax, dword ptr [esi + 5bh]
        add    eax, edx
        cmp    eax, edi
        ja     SHORT loc_1_1985
loc_1_1998:
        mov    eax, dword ptr [esi + 57h]
        add    eax, edx
        mov    dword ptr [esi + 57h], eax
        cmp    eax, 0ch
        jne    SHORT loc_1_19AC
        mov    dword ptr [esi + 57h], 0
loc_1_19AC:
        mov    eax, dword ptr [esi + 57h]
        cmp    dword ptr [esi + eax*4 + 61h], 0
        jne    SHORT loc_1_19C2
        cmp    dword ptr [esi + 53h], eax
        je     SHORT loc_1_19C2
        mov    dword ptr [esi + 57h], 0
loc_1_19C2:
        mov    eax, dword ptr [esi + 5bh]
        mov    dword ptr [esi + eax*4 + 91h], 0
        mov    byte ptr [esi + 1b5h], 0
        mov    eax, dword ptr [esi + 57h]
        cmp    dword ptr [esi + eax*4 + 61h], 2
        jne    SHORT loc_1_1A00
        mov    dword ptr [esi + 5bh], eax
        mov    byte ptr [esi + 5fh], 1
        push   esi
        call   sub_1_0780
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1A00:
        mov    dword ptr [esi + 5bh], 0ffffffffh
loc_1_1A07:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1A20:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        add    edi, 24h
        mov    esi, dword ptr [ebx + 8]
        cmp    byte ptr [ebx + 20h], 0
        je     SHORT loc_1_1A50
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1A50:
        cmp    byte ptr [esi + 142h], 0
        je     SHORT loc_1_1A67
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1A67:
        inc    dword ptr [ebx + 1ch]
        inc    dword ptr [esi + 30h]
        mov    dword ptr [edi + 4], ebx
        push   0
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        call   sub_1_2550
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_2574
        test   byte ptr [ebx + 71h], 2
        je     SHORT loc_1_1AA0
        mov    byte ptr [edi + 8], 0
        jmp    SHORT loc_1_1AB3
loc_1_1AA0:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [esi + 14fh]
        push   eax
        call   sub_1_2350
        mov    byte ptr [edi + 8], al
loc_1_1AB3:
        cmp    dword ptr [esi + 4bh], 0
        je     SHORT loc_1_1AC4
        mov    eax, dword ptr [esi + 4fh]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 24h], ecx
        jmp    SHORT loc_1_1ACA
loc_1_1AC4:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 4bh], ecx
loc_1_1ACA:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 4fh], ecx
        mov    dword ptr [edi], 0
        mov    eax, dword ptr [esi + 30h]
        dec    eax
        mov    dword ptr [esi + 30h], eax
        jne    SHORT loc_1_1B07
        cmp    byte ptr [esi + 142h], 0
        jne    SHORT loc_1_1AF7
        cmp    dword ptr [esi + 171h], 0
        jne    SHORT loc_1_1AF7
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_1_1B07
loc_1_1AF7:
        push   1
        add    esi, 17dh
        push   esi
        call   sub_1_2502
        jmp    SHORT loc_1_1B26
loc_1_1B07:
        cmp    dword ptr [esi + 4bh], 0
        je     SHORT loc_1_1B26
        mov    eax, dword ptr [esi + 53h]
        cmp    dword ptr [esi + eax*4 + 61h], 0
        jne    SHORT loc_1_1B26
        cmp    byte ptr [esi + 121h], 0
        jne    SHORT loc_1_1B26
        push   esi
        call   sub_1_1B40
loc_1_1B26:
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1B40:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 4bh], 0
        je     NEAR PTR loc_1_1DFF
loc_1_1B56:
        mov    eax, dword ptr [esi + 53h]
        cmp    dword ptr [esi + eax*4 + 61h], 0
        jne    NEAR PTR loc_1_1DFF
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        push   0
        mov    eax, dword ptr [esi + 4bh]
        push   eax
        call   sub_1_2550
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0ffh
        shr    eax, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [esi + 5bh]
        cmp    eax, -1
        jne    SHORT loc_1_1BE0
        mov    ecx, dword ptr [esi + 57h]
        cmp    dword ptr [esi + ecx*4 + 61h], 0
        mov    eax, dword ptr [esi + 53h]
        jne    SHORT loc_1_1BAB
        add    eax, dword ptr [ebp - 4]
        cmp    eax, 0ch
        jbe    SHORT loc_1_1C12
        mov    dword ptr [esi + 53h], 0
        jmp    SHORT loc_1_1C12
loc_1_1BAB:
        cmp    ecx, eax
        jbe    SHORT loc_1_1BC0
        add    eax, dword ptr [ebp - 4]
        cmp    eax, ecx
        jbe    SHORT loc_1_1C12
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_1BC0:
        add    eax, dword ptr [ebp - 4]
        cmp    eax, 0ch
        jbe    SHORT loc_1_1C12
        mov    dword ptr [esi + 53h], 0
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 57h], eax
        jae    SHORT loc_1_1C12
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1BE0:
        mov    ecx, dword ptr [esi + 53h]
        cmp    eax, ecx
        jbe    SHORT loc_1_1BF7
        add    ecx, dword ptr [ebp - 4]
        cmp    ecx, eax
        jbe    SHORT loc_1_1C12
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1BF7:
        add    ecx, dword ptr [ebp - 4]
        cmp    ecx, 0ch
        jbe    SHORT loc_1_1C12
        mov    dword ptr [esi + 53h], 0
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 5bh], eax
        jb     NEAR PTR loc_1_1DFF
loc_1_1C12:
        mov    edi, dword ptr [esi + 4bh]
        mov    ebx, dword ptr [esi + 53h]
        mov    eax, dword ptr [edi + 24h]
        mov    ecx, ebx
        mov    dword ptr [esi + 4bh], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, ebx
        cmp    eax, ebx
        jbe    SHORT loc_1_1C43
        lea    eax, [esi + ebx*4 + 61h]
loc_1_1C30:
        mov    dword ptr [eax], 1
        add    eax, 4
        inc    ecx
        mov    edx, dword ptr [ebp - 4]
        add    edx, ebx
        cmp    edx, ecx
        ja     SHORT loc_1_1C30
loc_1_1C43:
        mov    dword ptr [esi + ebx*4 + 91h], edi
        lea    eax, [esi + ebx*4 + 0c1h]
        push   eax
        push   edi
        push   esi
        call   0
        or     al, al
        jne    SHORT loc_1_1CB0
        push   0c0000001h
        mov    eax, dword ptr [edi + 28h]
        push   edi
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_246A
        mov    ecx, ebx
        mov    eax, dword ptr [ebp - 4]
        add    eax, ebx
        cmp    eax, ebx
        jbe    SHORT loc_1_1C93
        lea    eax, [esi + ebx*4 + 61h]
loc_1_1C80:
        mov    dword ptr [eax], 0
        add    eax, 4
        inc    ecx
        mov    edx, dword ptr [ebp - 4]
        add    edx, ebx
        cmp    edx, ecx
        ja     SHORT loc_1_1C80
loc_1_1C93:
        mov    eax, dword ptr [edi + 28h]
        dec    dword ptr [eax + 1ch]
        mov    dword ptr [esi + ebx*4 + 91h], 0
        jmp    NEAR PTR loc_1_1DA7
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1CB0:
        mov    ecx, dword ptr [esi + ebx*4 + 0c1h]
        cmp    ecx, 3ch
        jae    SHORT loc_1_1CDF
        mov    eax, 3ch
        sub    eax, ecx
        push   eax
        db  068h, 028h, 027h, 000h, 000h
        mov    eax, dword ptr [esi + 0f1h]
        mov    edx, ebx
        shl    edx, 8
        add    eax, edx
        add    eax, ecx
        push   eax
        push   esi
        call   sub_1_0390
loc_1_1CDF:
        cmp    byte ptr [esi + 142h], 0
        jne    NEAR PTR loc_1_1DBA
        mov    ecx, ebx
        mov    eax, dword ptr [ebp - 4]
        add    eax, ebx
        cmp    eax, ebx
        jbe    SHORT loc_1_1D13
        lea    eax, [esi + ebx*4 + 61h]
        add    eax, 0
loc_1_1D00:
        mov    dword ptr [eax], 2
        add    eax, 4
        inc    ecx
        mov    edx, dword ptr [ebp - 4]
        add    edx, ebx
        cmp    edx, ecx
        ja     SHORT loc_1_1D00
loc_1_1D13:
        mov    eax, dword ptr [esi + 53h]
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 53h], eax
        cmp    eax, 0ch
        jne    SHORT loc_1_1D28
        mov    dword ptr [esi + 53h], 0
loc_1_1D28:
        cmp    dword ptr [esi + 5bh], -1
        jne    SHORT loc_1_1D63
        mov    eax, dword ptr [esi + 57h]
        cmp    dword ptr [esi + eax*4 + 61h], 0
        jne    SHORT loc_1_1D44
        cmp    dword ptr [esi + 53h], eax
        je     SHORT loc_1_1D44
        mov    dword ptr [esi + 57h], 0
loc_1_1D44:
        mov    eax, dword ptr [esi + 57h]
        mov    dword ptr [esi + 5bh], eax
        cmp    byte ptr [esi + 11ch], 0
        je     SHORT loc_1_1D59
        mov    byte ptr [esi + 60h], 1
        jmp    SHORT loc_1_1D63
loc_1_1D59:
        mov    byte ptr [esi + 5fh], 1
        push   esi
        call   sub_1_0780
loc_1_1D63:
        cmp    byte ptr [edi + 2ch], 0
        jne    SHORT loc_1_1D80
        mov    ebx, dword ptr [edi + 28h]
        push   0
        push   edi
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_246A
        dec    dword ptr [ebx + 1ch]
        jmp    SHORT loc_1_1DA7
        db  08Dh, 064h, 024h, 000h
loc_1_1D80:
        cmp    dword ptr [esi + 165h], 0
        jne    SHORT loc_1_1D91
        mov    dword ptr [esi + 165h], edi
        jmp    SHORT loc_1_1D9A
loc_1_1D91:
        mov    eax, dword ptr [esi + 169h]
        mov    dword ptr [eax + 24h], edi
loc_1_1D9A:
        mov    dword ptr [esi + 169h], edi
        mov    dword ptr [edi + 24h], 0
loc_1_1DA7:
        cmp    dword ptr [esi + 4bh], 0
        jne    NEAR PTR loc_1_1B56
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1DBA:
        mov    eax, dword ptr [edi + 28h]
        mov    dword ptr [ebp - 8], eax
        mov    ecx, ebx
        mov    eax, dword ptr [ebp - 4]
        add    eax, ebx
        cmp    eax, ebx
        jbe    SHORT loc_1_1DE3
        lea    eax, [esi + ebx*4 + 61h]
        nop
loc_1_1DD0:
        mov    dword ptr [eax], 0
        add    eax, 4
        inc    ecx
        mov    edx, dword ptr [ebp - 4]
        add    edx, ebx
        cmp    edx, ecx
        ja     SHORT loc_1_1DD0
loc_1_1DE3:
        push   0
        mov    eax, dword ptr [edi + 28h]
        push   edi
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_246A
        push   3
        push   esi
        db  0E8h, 067h, 003h, 0FFh, 0FFh
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 1ch]
loc_1_1DFF:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_1E10:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 1b5h], 0
        je     SHORT loc_1_1E70
        cmp    dword ptr [esi + 5bh], -1
        je     SHORT loc_1_1E7D
        mov    byte ptr [esi + 1b5h], 0
        push   3
        push   1
        push   0aa000003h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_25F8
        add    esp, 10h
        push   esi
        db  0E8h, 038h, 0E8h, 0FEh, 0FFh
        cmp    dword ptr [esi + 14bh], 4
        jbe    SHORT loc_1_1E61
        push   esi
        db  0E8h, 099h, 0E8h, 0FEh, 0FFh
        mov    dword ptr [esi + 14bh], 0
loc_1_1E61:
        push   esi
        db  0E8h, 089h, 0EAh, 0FEh, 0FFh
        inc    dword ptr [esi + 14bh]
        jmp    SHORT loc_1_1E7D
        db  090h
loc_1_1E70:
        cmp    dword ptr [esi + 5bh], -1
        je     SHORT loc_1_1E7D
        mov    byte ptr [esi + 1b5h], 1
loc_1_1E7D:
        push   7d0h
        add    esi, 1a9h
        push   esi
        call   sub_1_2502
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1EA0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        inc    dword ptr [esi + 30h]
        cmp    byte ptr [esi + 142h], 0
        jne    NEAR PTR loc_1_1F56
        cmp    dword ptr [esi + 179h], 0
        jne    NEAR PTR loc_1_1F56
loc_1_1EC6:
        mov    edi, dword ptr [esi + 171h]
        or     edi, edi
        je     SHORT loc_1_1F31
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 171h], eax
        cmp    dword ptr [esi + 175h], edi
        jne    SHORT loc_1_1EEA
        mov    dword ptr [esi + 175h], 0
loc_1_1EEA:
        mov    dword ptr [esi + 179h], edi
        mov    eax, dword ptr [edi + 8]
        cmp    eax, 4
        je     SHORT loc_1_1F05
        cmp    eax, 0ah
        je     SHORT loc_1_1F05
        push   esi
        db  0E8h, 06Dh, 004h, 0FFh, 0FFh
        jmp    SHORT loc_1_1F0B
loc_1_1F05:
        push   esi
        db  0E8h, 0F5h, 004h, 0FFh, 0FFh
loc_1_1F0B:
        mov    ebx, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi + 8]
        cmp    ecx, 8
        je     SHORT loc_1_1F25
        cmp    ecx, 4
        je     SHORT loc_1_1F25
        push   eax
        push   edi
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_247C
loc_1_1F25:
        dec    dword ptr [ebx + 1ch]
        cmp    byte ptr [esi + 142h], 0
        je     SHORT loc_1_1EC6
loc_1_1F31:
        mov    dword ptr [esi + 179h], 0
        cmp    byte ptr [esi + 142h], 0
        je     SHORT loc_1_1F56
        dec    dword ptr [esi + 30h]
        push   1
        push   esi
        db  0E8h, 011h, 002h, 0FFh, 0FFh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_1_1F56:
        mov    edi, dword ptr [esi + 2ch]
        or     edi, edi
        je     SHORT loc_1_1FC1
        sub    ebx, ebx
        nop
loc_1_1F60:
        cmp    dword ptr [edi + 1ch], 0
        jbe    SHORT loc_1_1F70
        mov    edi, dword ptr [edi + 0ch]
        mov    ebx, edi
        jmp    SHORT loc_1_1FAB
        db  08Dh, 049h, 000h
loc_1_1F70:
        mov    eax, dword ptr [edi + 18h]
        cmp    dword ptr [esi + 161h], eax
        jne    SHORT loc_1_1F81
        push   esi
        db  0E8h, 0AFh, 0F2h, 0FEh, 0FFh
loc_1_1F81:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_2464
        or     ebx, ebx
        mov    eax, dword ptr [edi + 0ch]
        lea    ecx, [edi + 0ch]
        je     SHORT loc_1_1F9A
        mov    dword ptr [ebx + 0ch], eax
        jmp    SHORT loc_1_1F9D
loc_1_1F9A:
        mov    dword ptr [esi + 2ch], eax
loc_1_1F9D:
        mov    eax, edi
        mov    edi, dword ptr [ecx]
        push   0
        push   75h
        push   eax
        call   sub_1_252C
loc_1_1FAB:
        or     edi, edi
        jne    SHORT loc_1_1F60
        cmp    dword ptr [esi + 2ch], 0
        jne    SHORT loc_1_1FC1
        cmp    dword ptr [esi + 28h], 0
        jne    SHORT loc_1_1FC1
        push   esi
        db  0E8h, 0BFh, 0E6h, 0FEh, 0FFh
loc_1_1FC1:
        mov    eax, dword ptr [esi + 30h]
        dec    eax
        mov    dword ptr [esi + 30h], eax
        jne    SHORT loc_1_1FF7
        cmp    byte ptr [esi + 142h], 0
        jne    SHORT loc_1_1FE2
        cmp    dword ptr [esi + 171h], 0
        jne    SHORT loc_1_1FE2
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_1_1FF7
loc_1_1FE2:
        push   1
        add    esi, 17dh
        push   esi
        call   sub_1_2502
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_1_1FF7:
        cmp    dword ptr [esi + 4bh], 0
        je     SHORT loc_1_2016
        mov    eax, dword ptr [esi + 53h]
        cmp    dword ptr [esi + eax*4 + 61h], 0
        jne    SHORT loc_1_2016
        cmp    byte ptr [esi + 121h], 0
        jne    SHORT loc_1_2016
        push   esi
        call   sub_1_1B40
loc_1_2016:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Dh, 049h, 000h
loc_1_2020:
        db  0A0h, 0B8h, 026h, 000h, 000h
        cmp    al, 0
        jne    SHORT loc_1_2045
        db  0FEh, 005h, 0B8h, 026h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     SHORT loc_1_2047
        push   0
        push   0
        db  0E8h, 010h, 0E9h, 0FEh, 0FFh
        cmp    eax, 0
        jne    SHORT loc_1_2047
loc_1_2045:
        clc
        ret
loc_1_2047:
        stc
        ret
SOCKET_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_2058
        db  0FFh, 024h, 085h, 05Ah, 020h, 000h, 000h
loc_1_2058:
        clc
        ret
        db  020h, 020h, 000h, 000h, 058h, 020h, 000h, 000h, 058h, 020h, 000h, 000h, 058h, 020h, 000h, 000h
        db  058h, 020h, 000h, 000h, 058h, 020h, 000h, 000h, 058h, 020h, 000h, 000h, 058h, 020h, 000h, 000h
        db  058h, 020h, 000h, 000h, 058h, 020h, 000h, 000h, 058h, 020h, 000h, 000h, 058h, 020h, 000h, 000h
        db  058h, 020h, 000h, 000h, 058h, 020h, 000h, 000h, 058h, 020h, 000h, 000h, 058h, 020h, 000h, 000h
        db  058h, 020h, 000h, 000h, 058h, 020h, 000h, 000h, 058h, 020h, 000h, 000h, 058h, 020h, 000h, 000h
        db  058h, 020h, 000h, 000h, 058h, 020h, 000h, 000h, 058h, 020h, 000h, 000h, 058h, 020h, 000h, 000h
        db  058h, 020h, 000h, 000h, 058h, 020h, 000h, 000h, 020h, 020h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_1_20D0:
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
        call   sub_1_2100
        pop    ebp
        ret    20h
        db  08Dh, 064h, 024h, 000h
sub_1_2100:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 18h], 0eh
        jb     NEAR PTR loc_1_21A0
        mov    edx, dword ptr [ebp + 10h]
        mov    al, byte ptr [edx]
        test   al, 1
        je     SHORT loc_1_2140
        cmp    dword ptr [edx + 2], -1
        jne    SHORT loc_1_2135
        cmp    al, 0ffh
        jne    SHORT loc_1_2135
        cmp    byte ptr [edx + 1], 0ffh
        jne    SHORT loc_1_2135
        mov    dword ptr [ebp - 4], 8
        jmp    SHORT loc_1_21A7
loc_1_2135:
        mov    dword ptr [ebp - 4], 2
        jmp    SHORT loc_1_21A7
        db  08Bh, 0FFh
loc_1_2140:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0ch], 20h
        je     SHORT loc_1_2190
        mov    edx, dword ptr [ebp + 10h]
        lea    ecx, [esi + 2ch]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_2166
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_216D
loc_1_2166:
        mov    dword ptr [ebp - 10h], 1
loc_1_216D:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_2180
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_21AA
        db  08Dh, 064h, 024h, 000h
loc_1_2180:
        mov    dword ptr [ebp - 4], 20h
        jmp    SHORT loc_1_21AA
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2190:
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_21AA
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_21A0:
        mov    dword ptr [ebp - 4], 20h
loc_1_21A7:
        mov    esi, dword ptr [ebp + 8]
loc_1_21AA:
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     NEAR PTR loc_1_22A1
loc_1_21B5:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, ebx
        and    eax, dword ptr [ebp - 4]
        test   al, 9
        jne    SHORT loc_1_220D
        test   al, 2
        je     SHORT loc_1_21F9
        lea    eax, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 016h, 00Ah, 0FFh, 0FFh
        or     al, al
        je     SHORT loc_1_21F9
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 19h]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        test   dword ptr [eax + ecx*4], edx
        jne    SHORT loc_1_220D
loc_1_21F9:
        test   byte ptr [ebp - 4], 2
        je     SHORT loc_1_2204
        test   bl, 4
        jne    SHORT loc_1_220D
loc_1_2204:
        test   bl, 20h
        je     NEAR PTR loc_1_2296
loc_1_220D:
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
        call   sub_1_2482
        mov    byte ptr [edi + 18h], 1
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_2296
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_224F
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_224F:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_2260
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_2266
        db  08Bh, 0FFh
loc_1_2260:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_2266:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_2275
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_2488
loc_1_2275:
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
        call   sub_1_252C
        mov    edi, ebx
        jmp    SHORT loc_1_2299
loc_1_2296:
        mov    edi, dword ptr [edi + 10h]
loc_1_2299:
        or     edi, edi
        jne    NEAR PTR loc_1_21B5
loc_1_22A1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_22B0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_22C0
        pop    ebp
        ret    4
sub_1_22C0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     SHORT loc_1_233E
loc_1_22D0:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_2337
        mov    byte ptr [edi + 18h], 0
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_2488
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_2337
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_22FC
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_22FC:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_2310
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_2316
        db  005h, 000h, 000h, 000h, 000h
loc_1_2310:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_2316:
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
        call   sub_1_252C
        mov    edi, ebx
        jmp    SHORT loc_1_233A
loc_1_2337:
        mov    edi, dword ptr [edi + 10h]
loc_1_233A:
        or     edi, edi
        jne    SHORT loc_1_22D0
loc_1_233E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2350:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0ch]
        test   al, 20h
        je     SHORT loc_1_2365
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_2365:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     SHORT loc_1_23A1
        cmp    dword ptr [ecx + 2], -1
        jne    SHORT loc_1_2392
        cmp    bl, 0ffh
        jne    SHORT loc_1_2392
        cmp    byte ptr [ecx + 1], 0ffh
        jne    SHORT loc_1_2392
        test   al, 8
        je     SHORT loc_1_238B
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_238B:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_2392:
        test   al, 4
        jne    SHORT loc_1_239A
        test   al, 2
        je     SHORT loc_1_23B9
loc_1_239A:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_23A1:
        mov    eax, dword ptr [edx + 2eh]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_23B9
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 2ch], ax
        jne    SHORT loc_1_23B9
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_23B9:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
sub_1_23C0:
        push   esi
        mov    ecx, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        cld
        rep outsb
        pop    esi
        ret    0ch
sub_1_23D4:
        push   esi
        mov    ecx, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        cld
        rep outsw
        pop    esi
        ret    0ch
        db  056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh
        db  05Eh, 0C2h, 00Ch, 000h
sub_1_23FD:
        push   edi
        mov    ecx, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        cld
        rep insb
        pop    edi
        ret    0ch
sub_1_2411:
        push   edi
        mov    ecx, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        cld
        rep insw
        pop    edi
        ret    0ch
        db  057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Dh
        db  05Fh, 0C2h, 00Ch, 000h
sub_1_243A:
        db  0CDh, 020h, 031h, 080h, 028h, 000h
sub_1_2440:
        db  0CDh, 020h, 032h, 080h, 028h, 000h
sub_1_2446:
        db  0CDh, 020h, 033h, 080h, 028h, 000h
sub_1_244C:
        db  0CDh, 020h, 034h, 080h, 028h, 000h
sub_1_2452:
        db  0CDh, 020h, 035h, 080h, 028h, 000h
sub_1_2458:
        db  0CDh, 020h, 036h, 080h, 028h, 000h
        db  0CDh, 020h, 037h, 080h, 028h, 000h
sub_1_2464:
        db  0CDh, 020h, 038h, 080h, 028h, 000h
sub_1_246A:
        db  0CDh, 020h, 039h, 080h, 028h, 000h
        db  0CDh, 020h, 03Ah, 080h, 028h, 000h
sub_1_2476:
        db  0CDh, 020h, 03Bh, 080h, 028h, 000h
sub_1_247C:
        db  0CDh, 020h, 03Ch, 080h, 028h, 000h
sub_1_2482:
        db  0CDh, 020h, 03Dh, 080h, 028h, 000h
sub_1_2488:
        db  0CDh, 020h, 03Eh, 080h, 028h, 000h
sub_1_248E:
        db  0CDh, 020h, 03Fh, 080h, 028h, 000h
sub_1_2494:
        db  0CDh, 020h, 040h, 080h, 028h, 000h
        db  0CDh, 020h, 041h, 080h, 028h, 000h
        db  0CDh, 020h, 009h, 080h, 028h, 000h
        db  0CDh, 020h, 008h, 080h, 028h, 000h
sub_1_24AC:
        db  0CDh, 020h, 044h, 080h, 028h, 000h
        db  0CDh, 020h, 048h, 080h, 028h, 000h
        db  0CDh, 020h, 049h, 080h, 028h, 000h
        db  0CDh, 020h, 04Fh, 080h, 028h, 000h
        db  0CDh, 020h, 057h, 080h, 028h, 000h
        db  0CDh, 020h, 058h, 080h, 028h, 000h
        db  0CDh, 020h, 059h, 080h, 028h, 000h
        db  0CDh, 020h, 05Ah, 080h, 028h, 000h
        db  0CDh, 020h, 05Bh, 080h, 028h, 000h
        db  0CDh, 020h, 05Ch, 080h, 028h, 000h
        db  0CDh, 020h, 05Dh, 080h, 028h, 000h
        db  0CDh, 020h, 05Eh, 080h, 028h, 000h
        db  0CDh, 020h, 056h, 080h, 028h, 000h
        add    byte ptr [eax], al
        db  0CDh, 020h, 000h, 080h, 028h, 000h
sub_1_2502:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
sub_1_2508:
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
sub_1_250E:
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
sub_1_2514:
        db  0CDh, 020h, 010h, 080h, 028h, 000h
sub_1_251A:
        db  0CDh, 020h, 011h, 080h, 028h, 000h
sub_1_2520:
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_2526:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_252C:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_2550:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_2574:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_2598:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
        db  0CDh, 020h, 028h, 080h, 028h, 000h
sub_1_25A4:
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_25AA:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_25B0:
        db  0CDh, 020h, 007h, 080h, 028h, 000h
        db  0CDh, 020h, 042h, 080h, 028h, 000h
        db  0CDh, 020h, 012h, 080h, 028h, 000h
        db  0CDh, 020h, 013h, 080h, 028h, 000h
        db  0CDh, 020h, 014h, 080h, 028h, 000h
        db  0CDh, 020h, 015h, 080h, 028h, 000h
        db  0CDh, 020h, 04Ah, 080h, 028h, 000h
        db  0CDh, 020h, 04Bh, 080h, 028h, 000h
        db  0CDh, 020h, 04Ch, 080h, 028h, 000h
        db  0CDh, 020h, 04Dh, 080h, 028h, 000h
        db  0CDh, 020h, 04Eh, 080h, 028h, 000h
        db  0CDh, 020h, 046h, 080h, 028h, 000h
sub_1_25F8:
        db  0CDh, 020h, 045h, 080h, 028h, 000h
        db  0CDh, 020h, 043h, 080h, 028h, 000h
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        add    byte ptr [eax], al
sub_1_260C:
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
sub_1_262C:
        push   esi
        push   edi
        pushfd
        mov    ecx, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        cmp    esi, edi
        jae    SHORT loc_1_264C
        or     ecx, ecx
        je     SHORT loc_1_2662
        std
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    SHORT loc_1_2660
loc_1_264C:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     SHORT loc_1_2660
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd
        mov    cl, al
loc_1_2660:
        rep movsb
loc_1_2662:
        popfd
        pop    edi
        pop    esi
        ret    0ch
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 00Ah, 000h, 000h, 053h, 04Fh, 043h, 04Bh
        db  045h, 054h, 020h, 020h, 000h, 040h, 000h, 054h, 049h, 020h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_2728:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_27B4:
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
        add    byte ptr [10000101h], cl
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
loc_1_282C:
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
        add    byte ptr [10000101h], cl
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   ebx
        dec    edi
        inc    ebx
        dec    ebx
        inc    ebp
        push   esp
        add    byte ptr [eax], al
loc_1_2890:
        push   ebx
        dec    edi
        inc    ebx
        dec    ebx
        inc    ebp
        push   esp
        add    byte ptr [eax], al
        dec    ecx
        dec    edi
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
loc_1_28A0:
        dec    ecx
        dec    edi
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
        dec    ecx
        dec    esi
        push   esp
        inc    ebp
        push   edx
        push   edx
        push   ebp
        push   eax
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [ecx + 4eh], cl
        push   esp
        inc    ebp
        push   edx
        push   edx
        push   ebp
        push   eax
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [ebp + 41h], cl
        pop    eax
        dec    ebp
        push   ebp
        dec    esp
        push   esp
        dec    ecx
        inc    ebx
        inc    ecx
        push   ebx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_28D0:
        dec    ebp
        inc    ecx
        pop    eax
        dec    ebp
        push   ebp
        dec    esp
        push   esp
        dec    ecx
        inc    ebx
        inc    ecx
        push   ebx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    esi
        inc    ebp
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        dec    esi
        inc    ebp
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        inc    edx
        db  075h, 073h
        push   esp
        db  079h, 070h
        add    byte ptr gs:[edx + 75h], al
        db  073h, 054h
        db  079h, 070h
        add    byte ptr gs:[ebx + 6fh], al
        outsb
        outsb
        arpl   word ptr gs:[edi + ebp*2 + 72h], si
        push   esp
        db  079h, 070h
        add    byte ptr gs:[eax], al
        add    byte ptr [ebx + 6fh], al
        outsb
        outsb
        arpl   word ptr gs:[edi + ebp*2 + 72h], si
        push   esp
        db  079h, 070h
        add    byte ptr gs:[eax], al
        add    byte ptr [esi + 6fh], cl
        db  076h, 065h
        insb
        insb
        and    byte ptr [edx], dh
        xor    byte ptr [eax], dh
        xor    byte ptr [eax], ah
        inc    ecx
        popad
        db  070h, 074h
        db  065h, 072h, 02Eh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
_LTEXT ends
        end
