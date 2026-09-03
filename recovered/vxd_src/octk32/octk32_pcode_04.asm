; octk32_pcode_04.asm -- OCTK32 object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_4_0000:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    dx, word ptr [ecx + 10h]
        add    dx, 7
        mov    al, byte ptr [ecx + 34h]
        out    dx, al
        mov    byte ptr [ecx + 34h], 0
        mov    al, 1
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_4_0020:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    si, word ptr [ecx + 10h]
        lea    edx, [esi + 4]
        in     al, dx
        mov    bl, al
        lea    edi, [esi + 5]
        mov    dx, di
        in     al, dx
        mov    dl, al
        and    edx, 0ffff00ffh
        shl    dx, 8
        mov    al, bl
        and    eax, 0ffff00ffh
        or     dx, ax
        mov    word ptr [ebp - 2], dx
        mov    bx, word ptr [ecx + 643h]
        lea    edx, [esi + 4]
        mov    al, bl
        out    dx, al
        mov    dx, di
        mov    al, bh
        out    dx, al
        lea    edi, [esi + 2]
        mov    dx, di
        in     al, dx
        add    si, 3
        mov    dx, si
        in     al, dx
        mov    dx, di
        in     al, dx
        mov    dx, si
        in     al, dx
        sub    esi, esi
loc_4_0083:
        mov    di, word ptr [ecx + 10h]
        lea    edx, [edi + 2]
        in     al, dx
        mov    bl, al
        lea    edx, [edi + 3]
        in     al, dx
        and    ax, 0ffh
        shl    ax, 8
        mov    dl, bl
        and    edx, 0ffff00ffh
        or     ax, dx
        lea    edx, [esi + ecx]
        add    esi, 2
        mov    byte ptr [edx + 63dh], ah
        mov    byte ptr [edx + 63eh], al
        cmp    esi, 6
        jl     SHORT loc_4_0083
        mov    cx, word ptr [ecx + 10h]
        mov    al, byte ptr [ebp - 2]
        lea    edx, [ecx + 4]
        out    dx, al
        lea    edx, [ecx + 5]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_00E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    di, word ptr [esi + 10h]
        mov    dx, di
        in     al, dx
        mov    cl, al
        lea    edx, [edi + 1]
        in     al, dx
        mov    dl, al
        and    edx, 0ffff00ffh
        shl    dx, 8
        and    cx, 0ffh
        or     dx, cx
        mov    word ptr [ebp - 4], dx
        lea    ecx, [edi + 4]
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 2], al
        add    di, 5
        mov    dx, di
        in     al, dx
        mov    bl, al
        and    ebx, 0ffff00ffh
        shl    bx, 8
        mov    al, byte ptr [ebp - 2]
        and    eax, 0ffff00ffh
        or     bx, ax
        mov    dx, cx
        mov    al, 6
        out    dx, al
        mov    dx, di
        mov    al, 0ah
        out    dx, al
        call   sub_4_1E20
        mov    cx, word ptr [esi + 10h]
        lea    edx, [ecx + 2]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        lea    edx, [ecx + 3]
        in     al, dx
        and    ax, 0ffh
        shl    ax, 8
        mov    cl, byte ptr [ebp - 1]
        and    ecx, 0ffff00ffh
        or     ax, cx
        mov    cx, word ptr [esi + 10h]
        mov    word ptr [esi + 643h], ax
        lea    edx, [ecx + 4]
        mov    al, bl
        out    dx, al
        lea    edx, [ecx + 5]
        mov    al, bh
        out    dx, al
        mov    dx, cx
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        lea    edx, [ecx + 1]
        mov    al, byte ptr [ebp - 3]
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_4_01A0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dx, word ptr [eax + 10h]
        add    dx, 7
        mov    al, 98h
        out    dx, al
        mov    al, 1
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_01C0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 65h]
        cmp    word ptr [eax], 0
        je     SHORT loc_4_01D9
        sub    al, al
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_4_01D9:
        mov    ax, word ptr [ebp + 0ch]
        mov    dl, ah
        mov    dh, al
        mov    eax, dword ptr [ecx + 65h]
        or     dword ptr [ecx + 0b7h], 800h
        mov    word ptr [eax], dx
        mov    eax, dword ptr [ebp + 10h]
        mov    edx, eax
        shr    edx, 8
        mov    ebx, eax
        shr    ebx, 18h
        mov    esi, dword ptr [ecx + 65h]
        mov    bh, dh
        mov    word ptr [esi + 2], bx
        mov    dh, al
        mov    eax, dword ptr [ecx + 65h]
        push   ecx
        mov    word ptr [eax + 4], dx
        db  068h, 000h, 000h, 000h, 000h
        add    ecx, 515h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0222:
        mov    al, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_0230:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        or     dword ptr [ecx + 0b7h], 80h
        cmp    byte ptr [ecx + 39h], 1
        sbb    eax, eax
        and    eax, 1000h
        mov    edx, dword ptr [ecx + 6dh]
        mov    dword ptr [ecx + 0a1h], 0
        mov    word ptr [edx], ax
        mov    eax, dword ptr [ecx + 6dh]
        or     word ptr [eax], 2
        mov    eax, dword ptr [ecx + 6dh]
        mov    edx, dword ptr [ecx + 6dh]
        mov    ax, word ptr [eax]
        mov    word ptr [ecx + 649h], ax
        mov    ax, word ptr [ecx + 54h]
        mov    word ptr [edx + 6], ax
        mov    edx, dword ptr [ecx + 6dh]
        mov    eax, dword ptr [ecx + 50h]
        mov    dword ptr [edx + 2], eax
        mov    eax, dword ptr [ecx + 61h]
        mov    ebx, eax
        shr    ebx, 10h
        mov    dx, ax
        and    edx, 0ffh
        shl    edx, 10h
        mov    dl, bl
        shl    edx, 8
        shr    ax, 8
        and    eax, 0ffffh
        shl    eax, 10h
        shr    bx, 8
        or     edx, eax
        mov    eax, ebx
        and    eax, 0ffffh
        or     edx, eax
        mov    eax, dword ptr [ecx + 6dh]
        mov    dword ptr [eax + 8], edx
        mov    ebx, dword ptr [ecx + 5dh]
        mov    ax, bx
        shr    ebx, 10h
        mov    dx, ax
        and    edx, 0ffh
        shl    edx, 10h
        mov    dl, bl
        shl    edx, 8
        shr    ax, 8
        and    eax, 0ffffh
        shl    eax, 10h
        shr    bx, 8
        or     edx, eax
        mov    eax, ebx
        and    eax, 0ffffh
        or     edx, eax
        mov    eax, dword ptr [ecx + 6dh]
        mov    dword ptr [eax + 0ch], edx
        mov    eax, dword ptr [ecx + 6dh]
        mov    word ptr [eax + 10h], 0e00h
        mov    eax, dword ptr [ecx + 6dh]
        mov    word ptr [eax + 12h], 1a00h
        mov    eax, dword ptr [ecx + 6dh]
        mov    word ptr [eax + 14h], 4
        mov    eax, dword ptr [ecx + 6dh]
        mov    word ptr [eax + 16h], 0
        mov    eax, dword ptr [ecx + 6dh]
        mov    word ptr [eax + 18h], 0
        mov    eax, dword ptr [ecx + 6dh]
        mov    byte ptr [eax + 1ah], 80h
        mov    eax, dword ptr [ecx + 6dh]
        mov    byte ptr [eax + 1bh], 80h
        mov    eax, dword ptr [ecx + 7dh]
        push   eax
        push   3
        push   ecx
        call   sub_4_01C0
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_0350:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0b7h]
        and    ecx, 0ffffff69h
        mov    dword ptr [eax + 0b7h], ecx
        or     ecx, 1000h
        mov    dword ptr [eax + 0b7h], ecx
        mov    cx, word ptr [eax + 10h]
        sub    al, al
        lea    edx, [ecx + 6]
        out    dx, al
        lea    edx, [ecx + 7]
        mov    al, 0ffh
        out    dx, al
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_0390:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    si, word ptr [ecx + 10h]
        lea    edx, [esi + 6]
        in     al, dx
        mov    bl, al
        lea    edx, [esi + 7]
        in     al, dx
        mov    dl, al
        and    edx, 0ffff00ffh
        shl    dx, 8
        mov    al, bl
        and    eax, 0ffff00ffh
        or     dx, ax
        mov    ax, dx
        and    ax, 0fh
        mov    word ptr [ecx + 0b1h], ax
        cmp    dl, 40h
        jne    SHORT loc_4_03FB
        mov    eax, dword ptr [ecx + 0b7h]
        and    eax, 0ffffefffh
        mov    dword ptr [ecx + 0b7h], eax
        or     eax, 2
        mov    dword ptr [ecx + 0b7h], eax
        mov    dword ptr [ecx + 61fh], 0
        mov    al, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_03FB:
        sub    al, al
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_0410:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [ebp - 24h], 9f00h
        cmp    dword ptr [esi + 3eh], 9
        jne    SHORT loc_4_0436
        mov    word ptr [ebp - 1ah], 0fffeh
        mov    word ptr [ebp - 1ch], 0fffeh
        jmp    SHORT loc_4_0467
loc_4_0436:
        cmp    dword ptr [esi + 46h], 1
        jne    SHORT loc_4_0450
        cmp    dword ptr [esi + 42h], 0
        jne    SHORT loc_4_0450
        mov    word ptr [ebp - 1ah], 18h
        jmp    SHORT loc_4_045F
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_0450:
        mov    eax, dword ptr [esi + 42h]
        db  066h, 08Bh, 004h, 045h, 024h, 000h, 000h, 000h
        mov    word ptr [ebp - 1ah], ax
loc_4_045F:
        mov    ax, word ptr [ebp - 1ah]
        mov    word ptr [ebp - 1ch], ax
loc_4_0467:
        mov    word ptr [ebp - 18h], 101h
        mov    eax, dword ptr [esi + 75h]
        mov    ecx, eax
        shl    ecx, 10h
        shr    eax, 10h
        or     ecx, eax
        mov    dword ptr [ebp - 16h], ecx
        mov    eax, dword ptr [esi + 79h]
        mov    ecx, eax
        shl    ecx, 10h
        shr    eax, 10h
        or     ecx, eax
        mov    dword ptr [ebp - 12h], ecx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0493:
        mov    dx, word ptr [esi + 10h]
        add    dx, 0ch
        mov    al, 1
        out    dx, al
        mov    dx, word ptr [esi + 10h]
        add    dx, 0dh
        sub    al, al
        out    dx, al
        mov    dx, word ptr [esi + 10h]
        add    dx, 4
        out    dx, al
        mov    dx, word ptr [esi + 10h]
        add    dx, 5
        mov    al, 0ah
        out    dx, al
        lea    edi, [ebp - 24h]
        mov    ax, word ptr [esi + 10h]
        add    ax, 2
        mov    word ptr [ebp - 0ch], ax
        mov    ax, word ptr [esi + 10h]
        add    ax, 3
        mov    word ptr [ebp - 8], ax
        mov    ebx, 0bh
loc_4_04E0:
        mov    cx, word ptr [edi]
        mov    dx, word ptr [ebp - 0ch]
        mov    al, cl
        out    dx, al
        mov    dx, word ptr [ebp - 8]
        mov    al, ch
        out    dx, al
        add    edi, 2
        dec    ebx
        jne    SHORT loc_4_04E0
        mov    di, word ptr [esi + 10h]
        add    di, 8
        mov    dx, di
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    bx, word ptr [esi + 10h]
        add    bx, 9
        mov    dx, bx
        in     al, dx
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, byte ptr [ebp - 1]
        and    eax, 0ffff00ffh
        or     cx, ax
        or     cx, 8
        cmp    dword ptr [esi + 3eh], 9
        je     SHORT loc_4_0538
        or     cx, 100h
loc_4_0538:
        mov    dx, di
        mov    al, cl
        out    dx, al
        or     dword ptr [esi + 0b7h], 4000h
        mov    dx, bx
        mov    al, ch
        mov    cx, word ptr [esi + 10h]
        out    dx, al
        lea    edx, [ecx + 6]
        mov    al, 80h
        out    dx, al
        lea    edx, [ecx + 7]
        mov    al, 90h
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_0570:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    di, word ptr [esi + 10h]
        lea    edx, [edi + 6]
        in     al, dx
        mov    cl, al
        lea    edx, [edi + 7]
        in     al, dx
        mov    dl, al
        and    edx, 0ffff00ffh
        shl    dx, 8
        mov    al, cl
        and    eax, 0ffff00ffh
        or     dx, ax
        test   dl, 10h
        je     SHORT loc_4_05BA
        mov    word ptr [esi + 0b3h], dx
        and    dword ptr [esi + 0b7h], 0ffffbfffh
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_05BA:
        test   dl, 60h
        jne    SHORT loc_4_0630
        mov    eax, dword ptr [esi + 65h]
        and    dword ptr [esi + 0b7h], 0ffffbfffh
        cmp    word ptr [eax], 0
        jne    SHORT loc_4_0620
        cmp    word ptr [eax + 2], 0e2c1h
        jne    SHORT loc_4_0620
        cmp    word ptr [eax + 4], 8bd4h
        jne    SHORT loc_4_0620
        mov    eax, dword ptr [esi + 69h]
        cmp    word ptr [eax], 0ffffh
        jne    SHORT loc_4_0620
        cmp    word ptr [eax + 2], 0d7d1h
        jne    SHORT loc_4_0620
        cmp    word ptr [eax + 4], 0d9c5h
        jne    SHORT loc_4_0620
        cmp    word ptr [eax + 6], 0d4c3h
        jne    SHORT loc_4_0620
        push   esi
        call   sub_4_00E0
        or     dword ptr [esi + 0b7h], 4
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_0620:
        mov    word ptr [esi + 0b3h], dx
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  090h
loc_4_0630:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_4_0640:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    al, al
        mov    cx, word ptr [esi + 10h]
        and    dword ptr [esi + 0b7h], 0ffffff69h
        lea    edx, [ecx + 6]
        out    dx, al
        lea    edx, [ecx + 7]
        mov    al, 0ffh
        out    dx, al
        sub    bx, bx
        sub    edi, edi
        add    eax, 0
loc_4_0670:
        push   2710h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_067A:
        mov    cx, bx
        mov    bx, word ptr [esi + 10h]
        lea    edx, [ebx + 6]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        lea    edx, [ebx + 7]
        in     al, dx
        mov    bl, al
        and    ebx, 0ffff00ffh
        shl    bx, 8
        mov    al, byte ptr [ebp - 1]
        and    eax, 0ffff00ffh
        or     bx, ax
        mov    word ptr [esi + 0b1h], bx
        cmp    cx, bx
        je     SHORT loc_4_06B4
        cmp    bl, 40h
        je     SHORT loc_4_0710
loc_4_06B4:
        add    edi, 2710h
        cmp    edi, 5b8d80h
        jl     SHORT loc_4_0670
        test   bl, 30h
        je     SHORT loc_4_06F4
        cmp    dword ptr [esi + 4ebh], 0ah
        jne    SHORT loc_4_06F4
        and    ebx, 0fh
        push   ebx
        push   0c0000017h
        push   0c1000001h
        push   3
        push   0c1000001h
        mov    eax, dword ptr [esi + 4ffh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_06F1:
        add    esp, 18h
loc_4_06F4:
        mov    dword ptr [esi + 61fh], 0c0000001h
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_0710:
        or     dword ptr [esi + 0b7h], 2
        mov    dword ptr [esi + 61fh], 0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_0730:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 1], 0
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [ebp - 28h], 9f00h
        cmp    dword ptr [esi + 3eh], 9
        jne    SHORT loc_4_075A
        mov    word ptr [ebp - 1eh], 0fffeh
        mov    word ptr [ebp - 20h], 0fffeh
        jmp    SHORT loc_4_0787
loc_4_075A:
        cmp    dword ptr [esi + 46h], 1
        jne    SHORT loc_4_0770
        cmp    dword ptr [esi + 42h], 0
        jne    SHORT loc_4_0770
        mov    word ptr [ebp - 1eh], 18h
        jmp    SHORT loc_4_077F
        db  08Bh, 0FFh
loc_4_0770:
        mov    eax, dword ptr [esi + 42h]
        db  066h, 08Bh, 004h, 045h, 02Ch, 000h, 000h, 000h
        mov    word ptr [ebp - 1eh], ax
loc_4_077F:
        mov    ax, word ptr [ebp - 1eh]
        mov    word ptr [ebp - 20h], ax
loc_4_0787:
        mov    word ptr [ebp - 1ch], 101h
        mov    eax, dword ptr [esi + 75h]
        mov    ecx, eax
        shl    ecx, 10h
        shr    eax, 10h
        or     ecx, eax
        mov    dword ptr [ebp - 1ah], ecx
        mov    eax, dword ptr [esi + 79h]
        mov    ecx, eax
        shl    ecx, 10h
        shr    eax, 10h
        or     ecx, eax
        mov    dword ptr [ebp - 16h], ecx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_07B3:
        mov    dword ptr [ebp - 10h], 0
loc_4_07BA:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_4_07D2
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 515h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_07D2:
        mov    byte ptr [ebp - 1], 1
        mov    dx, word ptr [esi + 10h]
        add    dx, 0ch
        mov    al, 1
        out    dx, al
        mov    dx, word ptr [esi + 10h]
        add    dx, 0dh
        sub    al, al
        out    dx, al
        mov    dx, word ptr [esi + 10h]
        add    dx, 4
        out    dx, al
        mov    dx, word ptr [esi + 10h]
        add    dx, 5
        mov    al, 0ah
        out    dx, al
        lea    edi, [ebp - 28h]
        mov    ax, word ptr [esi + 10h]
        add    ax, 2
        mov    word ptr [ebp - 0ch], ax
        mov    ax, word ptr [esi + 10h]
        add    ax, 3
        mov    word ptr [ebp - 8], ax
        mov    ebx, 0bh
loc_4_0820:
        mov    cx, word ptr [edi]
        mov    dx, word ptr [ebp - 0ch]
        mov    al, cl
        out    dx, al
        mov    dx, word ptr [ebp - 8]
        mov    al, ch
        out    dx, al
        add    edi, 2
        dec    ebx
        jne    SHORT loc_4_0820
        mov    di, word ptr [esi + 10h]
        add    di, 8
        mov    dx, di
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    cx, word ptr [esi + 10h]
        add    cx, 9
        mov    dx, cx
        in     al, dx
        mov    bl, al
        and    ebx, 0ffff00ffh
        shl    bx, 8
        mov    al, byte ptr [ebp - 2]
        and    eax, 0ffff00ffh
        or     bx, ax
        or     bx, 8
        cmp    dword ptr [esi + 3eh], 9
        je     SHORT loc_4_0878
        or     bx, 100h
loc_4_0878:
        mov    dx, di
        mov    al, bl
        out    dx, al
        mov    dx, cx
        mov    al, bh
        out    dx, al
        mov    dx, word ptr [esi + 10h]
        add    dx, 6
        mov    al, 80h
        out    dx, al
        mov    dx, word ptr [esi + 10h]
        add    dx, 7
        mov    al, 90h
        mov    byte ptr [esi + 4e6h], 0
        mov    dword ptr [esi + 4dah], 0
        db  0C7h, 086h, 0DEh, 004h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 4e2h], esi
        out    dx, al
        push   2710h
        lea    eax, [esi + 4dah]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_08CC:
        lea    esp, [esp]
loc_4_08D0:
        cmp    byte ptr [esi + 4e6h], 0
        jne    NEAR PTR loc_4_09B0
        push   3e8h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_08E7:
        mov    di, word ptr [esi + 10h]
        lea    edx, [edi + 6]
        in     al, dx
        mov    cl, al
        lea    edx, [edi + 7]
        in     al, dx
        mov    bl, al
        and    ebx, 0ffff00ffh
        shl    bx, 8
        mov    al, cl
        and    eax, 0ffff00ffh
        or     bx, ax
        test   bl, 10h
        jne    NEAR PTR loc_4_0A00
        test   bl, 60h
        jne    SHORT loc_4_08D0
        mov    eax, dword ptr [esi + 65h]
        cmp    word ptr [eax], 0
        jne    NEAR PTR loc_4_0A58
        cmp    word ptr [eax + 2], 0e2c1h
        jne    NEAR PTR loc_4_0A58
        cmp    word ptr [eax + 4], 8bd4h
        jne    NEAR PTR loc_4_0A58
        mov    eax, dword ptr [esi + 69h]
        cmp    word ptr [eax], 0ffffh
        jne    NEAR PTR loc_4_0A58
        cmp    word ptr [eax + 2], 0d7d1h
        jne    NEAR PTR loc_4_0A58
        cmp    word ptr [eax + 4], 0d9c5h
        jne    NEAR PTR loc_4_0A58
        cmp    word ptr [eax + 6], 0d4c3h
        jne    NEAR PTR loc_4_0A58
        cmp    byte ptr [esi + 4e6h], 0
        jne    SHORT loc_4_0989
        lea    eax, [ebp - 3]
        push   eax
        lea    eax, [esi + 4dah]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0989:
        or     dword ptr [esi + 0b7h], 4
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        add    esi, 515h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_09A2:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_4_09B0:
        inc    dword ptr [ebp - 10h]
        cmp    dword ptr [ebp - 10h], 3
        jb     NEAR PTR loc_4_07BA
        and    dword ptr [esi + 0b7h], 0fffffffah
        cmp    dword ptr [esi + 4ebh], 0ah
        jne    SHORT loc_4_09F2
        mov    eax, ebx
        and    eax, 0ffffh
        push   eax
        push   0c0000009h
        push   3
        push   3
        push   0aa000008h
        mov    eax, dword ptr [esi + 4ffh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_09EF:
        add    esp, 18h
loc_4_09F2:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_0A00:
        cmp    dword ptr [esi + 4ebh], 0ah
        jne    SHORT loc_4_0A31
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        and    ebx, 0fh
        push   ebx
        push   0c0000008h
        push   0c1000002h
        push   4
        push   0c1000002h
        mov    eax, dword ptr [esi + 4ffh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0A2E:
        add    esp, 1ch
loc_4_0A31:
        cmp    byte ptr [esi + 4e6h], 0
        jne    SHORT loc_4_0A4A
        lea    eax, [ebp - 3]
        push   eax
        lea    eax, [esi + 4dah]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0A4A:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_0A58:
        cmp    dword ptr [esi + 4ebh], 0ah
        jne    SHORT loc_4_0A7E
        push   0c000000ah
        push   3
        push   2
        push   0aa00000bh
        mov    eax, dword ptr [esi + 4ffh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0A7B:
        add    esp, 14h
loc_4_0A7E:
        cmp    byte ptr [esi + 4e6h], 0
        jne    SHORT loc_4_0A97
        lea    eax, [ebp - 3]
        push   eax
        lea    eax, [esi + 4dah]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0A97:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0AB0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0a1h]
        or     eax, eax
        je     SHORT loc_4_0AE0
        cmp    eax, 1
        je     SHORT loc_4_0B12
        cmp    eax, 2
        je     SHORT loc_4_0B34
        cmp    eax, 3
        je     NEAR PTR loc_4_0BA8
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_4_0AE0:
        sub    eax, eax
        mov    dword ptr [esi + 0a1h], 1
        cmp    byte ptr [esi + 0e6h], al
        je     SHORT loc_4_0AF9
        mov    eax, 8000h
loc_4_0AF9:
        mov    ecx, eax
        shl    ecx, 10h
        shr    eax, 10h
        or     ecx, eax
        push   ecx
        push   4
        push   esi
        call   sub_4_01C0
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_0B12:
        mov    dword ptr [esi + 0a1h], 2
        push   1
        push   esi
        call   sub_4_4DE0
        push   0
        push   6
        push   esi
        call   sub_4_01C0
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_0B34:
        mov    eax, dword ptr [esi + 59h]
        test   eax, 8020h
        je     SHORT loc_4_0BA8
        xor    edi, edi
        mov    di, word ptr [esi + 649h]
        test   al, 20h
        je     SHORT loc_4_0B51
        or     edi, 200h
loc_4_0B51:
        test   ah, 80h
        je     SHORT loc_4_0B5C
        or     edi, 400h
loc_4_0B5C:
        mov    dword ptr [esi + 0a1h], 3
        mov    edx, edi
        shr    edx, 10h
        mov    cx, di
        and    ecx, 0ffh
        shl    ecx, 10h
        mov    cl, dl
        shr    di, 8
        shl    ecx, 8
        mov    eax, edi
        and    eax, 0ffffh
        shl    eax, 10h
        shr    dx, 8
        or     ecx, eax
        mov    eax, edx
        and    eax, 0ffffh
        or     ecx, eax
        push   ecx
        push   0dh
        push   esi
        call   sub_4_01C0
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_0BA8:
        mov    eax, dword ptr [esi + 0b7h]
        and    eax, 0ffffff77h
        mov    dword ptr [esi + 0b7h], eax
        or     eax, 10h
        mov    dword ptr [esi + 0b7h], eax
        mov    dword ptr [esi + 4efh], 4b0h
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 515h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0BDE:
        push   esi
        call   sub_4_3480
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_0BF0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 4ebh], 0ah
        je     SHORT loc_4_0C22
        test   byte ptr [esi + 0b7h], 10h
        je     SHORT loc_4_0C22
        mov    eax, dword ptr [esi + 4efh]
        dec    eax
        mov    dword ptr [esi + 4efh], eax
        jne    SHORT loc_4_0C22
        mov    dword ptr [esi + 4ebh], 0ah
loc_4_0C22:
        mov    eax, dword ptr [esi + 0b7h]
        test   al, 40h
        je     SHORT loc_4_0C36
        cmp    word ptr [esi + 623h], 4
        je     SHORT loc_4_0C44
loc_4_0C36:
        test   al, 58h
        je     SHORT loc_4_0C44
        and    al, 24h
        cmp    al, 24h
        jne    NEAR PTR loc_4_0F35
loc_4_0C44:
        push   esi
        call   sub_4_3440
        or     al, al
        je     NEAR PTR loc_4_0F35
        mov    eax, dword ptr [esi + 4e7h]
        dec    eax
        mov    dword ptr [esi + 4e7h], eax
        jne    NEAR PTR loc_4_0F35
        mov    eax, dword ptr [esi + 0b7h]
        cmp    eax, 1
        jne    SHORT loc_4_0CA0
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 515h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0C82:
        mov    dword ptr [esi + 4e7h], 6
        mov    dword ptr [esi + 4f3h], 1eh
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_4_0CA0:
        and    eax, 0d1bfh
        cmp    eax, 1001h
        jne    NEAR PTR loc_4_0D66
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 515h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0CC2:
        or     al, al
        mov    dword ptr [esi + 4e7h], 0ah
        je     SHORT loc_4_0CD5
        pop    esi
        pop    ebp
        ret    4
loc_4_0CD5:
        mov    eax, dword ptr [esi + 4f3h]
        dec    eax
        mov    dword ptr [esi + 4f3h], eax
        jne    NEAR PTR loc_4_0F35
        and    dword ptr [esi + 0b7h], 0ffffeffdh
        mov    ax, word ptr [esi + 0b1h]
        test   al, 30h
        je     SHORT loc_4_0D30
        cmp    dword ptr [esi + 4ebh], 0ah
        jne    SHORT loc_4_0D59
        and    eax, 0ffffh
        push   eax
        push   0c0000017h
        push   0c1000001h
        push   3
        push   0c1000001h
        mov    eax, dword ptr [esi + 4ffh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0D29:
        add    esp, 18h
        jmp    SHORT loc_4_0D59
        db  08Bh, 0FFh
loc_4_0D30:
        cmp    dword ptr [esi + 4ebh], 0ah
        jne    SHORT loc_4_0D59
        push   0c0000017h
        push   0c1000001h
        push   2
        push   0c1000001h
        mov    eax, dword ptr [esi + 4ffh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0D56:
        add    esp, 14h
loc_4_0D59:
        or     word ptr [esi + 619h], 8
        jmp    NEAR PTR loc_4_0EE8
loc_4_0D66:
        cmp    eax, 3
        jne    SHORT loc_4_0D96
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 515h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0D7D:
        mov    dword ptr [esi + 4e7h], 0ah
        mov    dword ptr [esi + 4f3h], 44ch
        pop    esi
        pop    ebp
        ret    4
loc_4_0D96:
        cmp    eax, 4003h
        jne    NEAR PTR loc_4_0E81
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 515h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0DB3:
        or     al, al
        je     NEAR PTR loc_4_0E50
        test   byte ptr [esi + 0b7h], 4
        je     SHORT loc_4_0DD3
        mov    dword ptr [esi + 4e7h], 0ah
        pop    esi
        pop    ebp
        ret    4
loc_4_0DD3:
        cmp    dword ptr [esi + 4ebh], 0ah
        jne    SHORT loc_4_0E30
        mov    ax, word ptr [esi + 0b3h]
        test   al, 10h
        je     SHORT loc_4_0E10
        and    eax, 0ffffh
        push   eax
        push   0c0000008h
        push   0c1000002h
        push   3
        push   0c1000002h
        mov    eax, dword ptr [esi + 4ffh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0E0A:
        add    esp, 18h
        jmp    SHORT loc_4_0E30
        db  090h
loc_4_0E10:
        push   0c000000ah
        push   0aa00000bh
        push   2
        push   0aa00000bh
        mov    eax, dword ptr [esi + 4ffh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0E2D:
        add    esp, 14h
loc_4_0E30:
        or     word ptr [esi + 619h], 8
        and    dword ptr [esi + 0b7h], 0ffffaff9h
        jmp    NEAR PTR loc_4_0EE8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_0E50:
        mov    dword ptr [esi + 4e7h], 0ah
        mov    eax, dword ptr [esi + 4f3h]
        dec    eax
        mov    dword ptr [esi + 4f3h], eax
        jne    NEAR PTR loc_4_0F35
        and    dword ptr [esi + 0b7h], 0ffffaff9h
        or     word ptr [esi + 619h], 8
        jmp    SHORT loc_4_0EE8
loc_4_0E81:
        cmp    eax, 7
        jne    SHORT loc_4_0EE8
        push   esi
        call   sub_4_3440
        or     al, al
        je     SHORT loc_4_0EB0
        mov    eax, dword ptr [esi + 0b7h]
        test   al, 20h
        jne    SHORT loc_4_0EB0
        test   al, 40h
        je     SHORT loc_4_0EA7
        mov    word ptr [esi + 623h], 5
loc_4_0EA7:
        push   esi
        call   sub_4_0230
        jmp    SHORT loc_4_0EE8
        db  090h
loc_4_0EB0:
        mov    eax, dword ptr [esi + 0b7h]
        and    eax, 0fffffff7h
        mov    dword ptr [esi + 0b7h], eax
        test   al, 4
        je     SHORT loc_4_0EE0
        test   eax, 2148h
        jne    SHORT loc_4_0EE0
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 515h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0EDC:
        jmp    SHORT loc_4_0EE8
        db  08Bh, 0FFh
loc_4_0EE0:
        or     word ptr [esi + 619h], 10h
loc_4_0EE8:
        mov    eax, dword ptr [esi + 4ebh]
        add    eax, eax
        mov    dword ptr [esi + 4ebh], eax
        cmp    eax, 280h
        jbe    SHORT loc_4_0F07
        mov    dword ptr [esi + 4ebh], 280h
loc_4_0F07:
        mov    eax, dword ptr [esi + 4ebh]
        mov    dword ptr [esi + 4e7h], eax
        test   byte ptr [esi + 619h], 8
        je     SHORT loc_4_0F35
        test   byte ptr [esi + 0b7h], 40h
        push   esi
        je     SHORT loc_4_0F30
        call   sub_4_2C80
        pop    esi
        pop    ebp
        ret    4
loc_4_0F30:
        call   sub_4_3380
loc_4_0F35:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_0F40:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 71h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        add    dword ptr [ecx + 48eh], eax
        mov    eax, dword ptr [ecx + 71h]
        mov    al, byte ptr [eax + 6]
        and    eax, 0ffh
        add    dword ptr [ecx + 492h], eax
        mov    eax, dword ptr [ecx + 71h]
        mov    al, byte ptr [eax + 7]
        and    eax, 0ffh
        add    dword ptr [ecx + 43ah], eax
        mov    eax, dword ptr [ecx + 71h]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        add    dword ptr [ecx + 496h], eax
        mov    eax, dword ptr [ecx + 71h]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        add    dword ptr [ecx + 49ah], eax
        mov    eax, dword ptr [ecx + 71h]
        mov    al, byte ptr [eax + 8]
        and    eax, 0ffh
        add    dword ptr [ecx + 4a2h], eax
        mov    eax, dword ptr [ecx + 71h]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        add    dword ptr [ecx + 4aah], eax
        mov    eax, dword ptr [ecx + 71h]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        add    dword ptr [ecx + 4aeh], eax
        mov    eax, dword ptr [ecx + 71h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        add    dword ptr [ecx + 49eh], eax
        mov    eax, dword ptr [ecx + 71h]
        mov    al, byte ptr [eax + 9]
        and    eax, 0ffh
        add    dword ptr [ecx + 4a6h], eax
        mov    eax, dword ptr [ecx + 71h]
        mov    al, byte ptr [eax + 0ch]
        and    eax, 0ffh
        add    dword ptr [ecx + 496h], eax
        mov    eax, dword ptr [ecx + 71h]
        pop    ebp
        mov    al, byte ptr [eax + 0dh]
        and    eax, 0ffh
        add    dword ptr [ecx + 49ah], eax
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1020:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    al, 1
        mov    cx, word ptr [esi + 10h]
        lea    edx, [ecx + 0ch]
        out    dx, al
        lea    edx, [ecx + 0dh]
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 4]
        mov    al, 0e0h
        out    dx, al
        lea    edx, [ecx + 5]
        mov    al, 5
        out    dx, al
        lea    ecx, [esi + 0bbh]
        mov    edi, 4
loc_4_1053:
        mov    bx, word ptr [esi + 10h]
        lea    edx, [ebx + 2]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        lea    edx, [ebx + 3]
        in     al, dx
        and    ax, 0ffh
        shl    ax, 8
        mov    dl, byte ptr [ebp - 1]
        and    edx, 0ffff00ffh
        or     ax, dx
        mov    word ptr [ecx], ax
        add    ecx, 2
        dec    edi
        jne    SHORT loc_4_1053
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_1090:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 515h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_10A9:
        xor    eax, eax
        mov    ax, word ptr [esi + 0c1h]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi + 0bfh]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi + 0bdh]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi + 0bbh]
        push   eax
        push   0c0000024h
        push   0c1000003h
        push   6
        push   0c1000003h
        mov    eax, dword ptr [esi + 4ffh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_10EE:
        add    esp, 24h
        push   5
        push   esi
        call   sub_4_1890
        mov    dword ptr [esi + 0b7h], 20h
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_1110:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    di, word ptr [esi + 0a7h]
        test   di, 8000h
        mov    word ptr [esi + 0abh], di
        mov    dword ptr [esi + 0adh], 0
        je     SHORT loc_4_1142
        mov    dword ptr [esi + 0adh], 8000h
loc_4_1142:
        test   di, 4000h
        je     SHORT loc_4_1153
        or     dword ptr [esi + 0adh], 4000h
loc_4_1153:
        test   di, 2000h
        je     SHORT loc_4_1164
        or     dword ptr [esi + 0adh], 2000h
loc_4_1164:
        test   di, 1000h
        je     SHORT loc_4_1175
        or     dword ptr [esi + 0adh], 1000h
loc_4_1175:
        test   di, 800h
        je     SHORT loc_4_1186
        or     dword ptr [esi + 0adh], 800h
loc_4_1186:
        test   di, 400h
        je     SHORT loc_4_1197
        or     dword ptr [esi + 0adh], 400h
loc_4_1197:
        test   di, 100h
        je     SHORT loc_4_11A8
        or     dword ptr [esi + 0adh], 200h
loc_4_11A8:
        test   di, 80h
        je     SHORT loc_4_11BF
        or     dword ptr [esi + 0adh], 100h
        push   esi
        call   sub_4_3410
loc_4_11BF:
        test   di, 40h
        je     SHORT loc_4_11D0
        or     dword ptr [esi + 0adh], 80h
loc_4_11D0:
        test   di, 20h
        je     SHORT loc_4_11DE
        or     dword ptr [esi + 0adh], 40h
loc_4_11DE:
        mov    bx, di
        and    bx, 0d00h
        je     NEAR PTR loc_4_1298
        test   di, 500h
        je     SHORT loc_4_11FA
        or     dword ptr [esi + 0b7h], 20h
loc_4_11FA:
        push   5
        push   esi
        call   sub_4_1890
        cmp    dword ptr [esi + 4ebh], 0ah
        jne    NEAR PTR loc_4_1298
        mov    eax, ebx
        and    eax, 0ffffh
        cmp    eax, 100h
        je     SHORT loc_4_1230
        cmp    eax, 400h
        je     SHORT loc_4_1250
        cmp    eax, 800h
        je     SHORT loc_4_1270
        jmp    SHORT loc_4_1298
        db  08Dh, 049h, 000h
loc_4_1230:
        mov    eax, edi
        and    eax, 0ffffh
        push   eax
        push   0c0000023h
        push   0c100000ch
        push   3
        push   0c100000ch
        jmp    SHORT loc_4_1289
        db  005h, 000h, 000h, 000h, 000h
loc_4_1250:
        mov    eax, edi
        and    eax, 0ffffh
        push   eax
        push   0c0000023h
        push   0c100000bh
        push   3
        push   0c100000bh
        jmp    SHORT loc_4_1289
        db  005h, 000h, 000h, 000h, 000h
loc_4_1270:
        mov    eax, edi
        and    eax, 0ffffh
        push   eax
        push   0c0000023h
        push   8100000ah
        push   3
        push   8100000ah
loc_4_1289:
        mov    eax, dword ptr [esi + 4ffh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1295:
        add    esp, 18h
loc_4_1298:
        push   4
        lea    eax, [esi + 0adh]
        push   eax
        push   40010006h
        push   esi
        call   sub_4_34D0
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        add    esi, 515h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_12BE:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_12D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [esi + 0a5h]
        cmp    eax, 3
        je     SHORT loc_4_1303
        cmp    eax, 7
        jl     NEAR PTR loc_4_1518
        cmp    eax, 0ah
        jle    NEAR PTR loc_4_1484
        cmp    eax, 0dh
        jne    NEAR PTR loc_4_1518
loc_4_1303:
        test   byte ptr [esi + 0b7h], 80h
        je     NEAR PTR loc_4_1484
        mov    ax, word ptr [esi + 0a7h]
        mov    word ptr [esi + 0b5h], ax
        cmp    ah, 80h
        jne    SHORT loc_4_1350
        mov    word ptr [esi + 0b5h], 0
        mov    word ptr [esi + 0abh], 0
        mov    dword ptr [esi + 0adh], 0
        push   esi
        call   sub_4_0AB0
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_4_1350:
        test   ah, 2
        je     NEAR PTR loc_4_1440
        and    ax, 0ffh
        mov    word ptr [esi + 0b5h], ax
        and    ax, 0fh
        cmp    ax, 7
        je     SHORT loc_4_1384
        cmp    ax, 8
        je     SHORT loc_4_1384
        cmp    ax, 9
        je     SHORT loc_4_1384
        cmp    ax, 0ah
        jne    NEAR PTR loc_4_146F
loc_4_1384:
        or     dword ptr [esi + 0b7h], 20h
        cmp    dword ptr [esi + 4ebh], 0ah
        jne    NEAR PTR loc_4_146F
        mov    cx, word ptr [esi + 0b5h]
        mov    ax, cx
        and    eax, 0fh
        cmp    eax, 7
        je     SHORT loc_4_13C0
        cmp    eax, 8
        je     SHORT loc_4_13E0
        cmp    eax, 9
        je     SHORT loc_4_1400
        cmp    eax, 0ah
        je     SHORT loc_4_1420
        jmp    NEAR PTR loc_4_146F
        db  08Bh, 0FFh
loc_4_13C0:
        mov    eax, ecx
        and    eax, 0ffffh
        push   eax
        push   0c000001eh
        push   0c1000004h
        push   3
        push   0c1000004h
        jmp    NEAR PTR loc_4_1460
        db  08Bh, 0FFh
loc_4_13E0:
        mov    eax, ecx
        and    eax, 0ffffh
        push   eax
        push   0c000001eh
        push   0c1000005h
        push   3
        push   0c1000005h
        jmp    SHORT loc_4_1460
        db  005h, 000h, 000h, 000h, 000h
loc_4_1400:
        mov    eax, ecx
        and    eax, 0ffffh
        push   eax
        push   0c000001eh
        push   0c1000006h
        push   3
        push   0c1000006h
        jmp    SHORT loc_4_1460
        db  005h, 000h, 000h, 000h, 000h
loc_4_1420:
        mov    eax, ecx
        and    eax, 0ffffh
        push   eax
        push   0c000001eh
        push   0c1000007h
        push   3
        push   0c1000007h
        jmp    SHORT loc_4_1460
        db  005h, 000h, 000h, 000h, 000h
loc_4_1440:
        cmp    dword ptr [esi + 4ebh], 0ah
        jne    SHORT loc_4_146F
        and    eax, 0ffffh
        push   eax
        push   0c000001eh
        push   81000008h
        push   3
        push   81000008h
loc_4_1460:
        mov    eax, dword ptr [esi + 4ffh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_146C:
        add    esp, 18h
loc_4_146F:
        push   5
        push   esi
        call   sub_4_1890
        push   esi
        call   sub_4_3480
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_1484:
        mov    eax, dword ptr [esi + 549h]
        mov    byte ptr [eax + 0eh], 3
        cmp    word ptr [esi + 0a7h], 8000h
        mov    dword ptr [esi + 61bh], 0
        je     SHORT loc_4_14AD
        mov    dword ptr [esi + 61bh], 0c0000001h
loc_4_14AD:
        cmp    word ptr [esi + 0a5h], 0ah
        jne    SHORT loc_4_14C8
        cmp    word ptr [esi + 0a7h], 8000h
        jne    SHORT loc_4_14C8
        push   esi
        call   sub_4_0F40
loc_4_14C8:
        cmp    word ptr [esi + 0a5h], 0dh
        jne    SHORT loc_4_1518
        test   byte ptr [esi + 0b8h], 2
        je     SHORT loc_4_1503
        and    dword ptr [esi + 59h], 0ffff7fdfh
        and    dword ptr [esi + 0b7h], 0fffffbffh
        lea    eax, [ebp - 1]
        push   eax
        add    esi, 64bh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_14FC:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_1503:
        test   dword ptr [esi + 59h], 8020h
        je     SHORT loc_4_1518
        push   0
        push   0
        push   esi
        push   0
        call   sub_4_1DB0
loc_4_1518:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_4_1520:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        or     dword ptr [esi + 0b7h], 2000h
        mov    eax, dword ptr [esi + 69h]
        mov    ax, word ptr [eax]
        mov    ch, al
        mov    cl, ah
        mov    eax, dword ptr [esi + 69h]
        mov    word ptr [esi + 0a5h], cx
        mov    ax, word ptr [eax + 2]
        mov    ch, al
        mov    cl, ah
        mov    eax, dword ptr [esi + 69h]
        mov    word ptr [esi + 0a7h], cx
        mov    ax, word ptr [eax + 4]
        mov    ch, al
        mov    cl, ah
        xor    eax, eax
        mov    al, byte ptr [esi + 33h]
        mov    word ptr [esi + 0a9h], cx
        or     eax, eax
        je     SHORT loc_4_1580
        cmp    eax, 4
        je     SHORT loc_4_1588
        cmp    eax, 6
        je     SHORT loc_4_1590
        cmp    eax, 8
        je     SHORT loc_4_15B6
        jmp    SHORT loc_4_15BC
loc_4_1580:
        push   esi
        call   sub_4_1090
        jmp    SHORT loc_4_15BC
loc_4_1588:
        push   esi
        call   sub_4_1110
        jmp    SHORT loc_4_15BC
loc_4_1590:
        mov    eax, dword ptr [esi + 0b7h]
        and    eax, 0fffff7ffh
        mov    dword ptr [esi + 0b7h], eax
        test   al, 80h
        je     SHORT loc_4_15BC
        cmp    dword ptr [esi + 0a1h], 0
        je     SHORT loc_4_15BC
        push   esi
        call   sub_4_0AB0
        jmp    SHORT loc_4_15BC
loc_4_15B6:
        push   esi
        call   sub_4_12D0
loc_4_15BC:
        cmp    word ptr [esi + 619h], 0
        je     SHORT loc_4_15CC
        push   esi
        call   sub_4_3380
loc_4_15CC:
        and    dword ptr [esi + 0b7h], 0ffffdfffh
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_4_15E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    si, word ptr [ebx + 10h]
        mov    ax, word ptr [ebx + 31h]
        lea    edx, [esi + 9]
        shr    ax, 8
        or     al, 8
        out    dx, al
        lea    edx, [esi + 0eh]
        in     al, dx
        mov    cl, al
        lea    edx, [esi + 0fh]
        in     al, dx
        mov    di, ax
        and    edi, 0ffff00ffh
        shl    di, 8
        mov    al, cl
        and    eax, 0ffff00ffh
        or     di, ax
        je     NEAR PTR loc_4_184E
        lea    edx, [esi + 6]
        in     al, dx
        mov    byte ptr [ebp - 2], al
        lea    edx, [esi + 7]
        in     al, dx
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    dl, byte ptr [ebp - 2]
        and    edx, 0ffff00ffh
        or     cx, dx
        lea    edx, [esi + 4]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        lea    edx, [esi + 5]
        in     al, dx
        and    ax, 0ffh
        shl    ax, 8
        mov    dl, byte ptr [ebp - 1]
        and    edx, 0ffff00ffh
        or     ax, dx
        and    ecx, 0ffffh
        shl    ecx, 10h
        and    eax, 0ffffh
        or     ecx, eax
        test   byte ptr [ebx + 32h], 4
        je     NEAR PTR loc_4_1755
        xor    eax, eax
        mov    al, byte ptr [ebx + 1eh]
        or     eax, eax
        je     SHORT loc_4_16A0
        cmp    eax, 1
        je     SHORT loc_4_16C0
        cmp    eax, 2
        je     SHORT loc_4_16F0
        cmp    eax, 4
        je     SHORT loc_4_16F0
        jmp    NEAR PTR loc_4_184E
        db  08Bh, 0FFh
loc_4_16A0:
        mov    eax, edi
        and    eax, 0ffffh
        push   eax
        push   ecx
        mov    eax, esi
        and    eax, 0ffffh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_16B6:
        jmp    NEAR PTR loc_4_184E
        db  005h, 000h, 000h, 000h, 000h
loc_4_16C0:
        mov    dword ptr [ebp - 2ch], ecx
        mov    word ptr [ebp - 16h], di
        mov    ax, word ptr [ebx + 10h]
        mov    word ptr [ebp - 14h], ax
        push   ecx
        push   edx
        push   edi
        movzx  ecx, word ptr [ebp - 16h]
        mov    edi, dword ptr [ebp - 2ch]
        mov    dx, word ptr [ebp - 14h]
        shr    cx, 1
        rep insw
        adc    cx, cx
        rep insb
        pop    edi
        pop    edx
        pop    ecx
        jmp    NEAR PTR loc_4_184E
loc_4_16F0:
        mov    dword ptr [ebp - 28h], ecx
        mov    word ptr [ebp - 10h], di
        mov    ax, word ptr [ebx + 10h]
        add    ax, 10h
        mov    word ptr [ebp - 12h], ax
        push   eax
        push   ecx
        push   edx
        push   edi
        mov    dx, word ptr [ebp - 12h]
        movzx  ecx, word ptr [ebp - 10h]
        mov    edi, dword ptr [ebp - 28h]
        mov    eax, ecx
        mov    cx, di
        neg    cl
        and    cx, 3
        cmp    ax, cx
        jb     SHORT loc_4_173F
        sub    ax, cx
        shr    cx, 1
        jae    SHORT loc_4_1731
        sub    dl, 10h
        insb
        add    dl, 10h
loc_4_1731:
        rep insw
        mov    cx, ax
        shr    cx, 2
        rep insd
        and    al, 3
loc_4_173F:
        mov    cl, al
        shr    cx, 1
        rep insw
        adc    cx, cx
        rep insb
        pop    edi
        pop    edx
        pop    ecx
        pop    eax
        jmp    NEAR PTR loc_4_184E
loc_4_1755:
        xor    eax, eax
        mov    al, byte ptr [ebx + 1eh]
        or     eax, eax
        je     SHORT loc_4_1776
        cmp    eax, 1
        je     SHORT loc_4_1791
        cmp    eax, 2
        je     SHORT loc_4_17C1
        cmp    eax, 4
        je     NEAR PTR loc_4_17F0
        jmp    NEAR PTR loc_4_184E
loc_4_1776:
        mov    eax, edi
        and    eax, 0ffffh
        push   eax
        push   ecx
        mov    eax, esi
        and    eax, 0ffffh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_178C:
        jmp    NEAR PTR loc_4_184E
loc_4_1791:
        mov    dword ptr [ebp - 24h], ecx
        mov    word ptr [ebp - 0eh], di
        mov    ax, word ptr [ebx + 10h]
        mov    word ptr [ebp - 0ch], ax
        push   ecx
        push   edx
        push   esi
        movzx  ecx, word ptr [ebp - 0eh]
        mov    esi, dword ptr [ebp - 24h]
        mov    dx, word ptr [ebp - 0ch]
        shr    cx, 1
        rep outsw
        adc    cx, cx
        rep outsb
        pop    esi
        pop    edx
        pop    ecx
        jmp    NEAR PTR loc_4_184E
loc_4_17C1:
        mov    dword ptr [ebp - 20h], ecx
        mov    word ptr [ebp - 0ah], di
        mov    ax, word ptr [ebx + 10h]
        mov    word ptr [ebp - 8], ax
        push   ecx
        push   edx
        push   esi
        movzx  ecx, word ptr [ebp - 0ah]
        mov    esi, dword ptr [ebp - 20h]
        mov    dx, word ptr [ebp - 8]
        shr    cx, 1
        rep outsw
        adc    cx, cx
        rep outsb
        pop    esi
        pop    edx
        pop    ecx
        jmp    SHORT loc_4_184E
        db  08Bh, 0FFh
loc_4_17F0:
        mov    dword ptr [ebp - 1ch], ecx
        mov    word ptr [ebp - 6], di
        mov    ax, word ptr [ebx + 10h]
        add    ax, 10h
        mov    word ptr [ebp - 4], ax
        push   ecx
        push   edx
        push   esi
        movzx  ecx, word ptr [ebp - 6]
        mov    esi, dword ptr [ebp - 1ch]
        mov    dx, word ptr [ebp - 4]
        mov    eax, ecx
        mov    cx, si
        neg    cl
        and    cx, 3
        cmp    ax, cx
        jb     SHORT loc_4_183E
        sub    ax, cx
        shr    cx, 1
        jae    SHORT loc_4_1830
        sub    dl, 10h
        outsb
        add    dl, 10h
loc_4_1830:
        rep outsw
        mov    cx, ax
        shr    cx, 2
        rep outsd
        and    al, 3
loc_4_183E:
        mov    cl, al
        shr    cx, 1
        rep outsw
        adc    cx, cx
        rep outsb
        pop    esi
        pop    edx
        pop    ecx
loc_4_184E:
        mov    cx, word ptr [ebx + 10h]
        lea    edx, [ecx + 9]
loc_4_1855:
        in     al, dx
        test   al, 8
        jne    SHORT loc_4_1855
        lea    edx, [ecx + 8]
        mov    ax, word ptr [ebx + 31h]
        or     al, 8
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_4_1870:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dx, word ptr [eax + 10h]
        add    dx, 7
        mov    al, 88h
        out    dx, al
        mov    al, 1
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1890:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        sub    eax, eax
        mov    edi, dword ptr [ebp + 0ch]
        cmp    edi, 5
        ja     NEAR PTR loc_4_1A5B
        db  0FFh, 024h, 0BDh, 064h, 01Ah, 000h, 000h
        db  08Dh, 049h, 000h
loc_4_18B0:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0b7h]
        test   al, 40h
        jne    SHORT loc_4_18CA
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_18CA:
        test   al, 8
        je     SHORT loc_4_18D8
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_18D8:
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        add    esi, 515h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_18EA:
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_18F4:
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_0730
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1905:
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0b7h]
        test   cl, 10h
        je     SHORT loc_4_1920
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_4_1920:
        test   cl, 8
        je     SHORT loc_4_1932
        mov    eax, 103h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1932:
        test   cl, 40h
        jne    SHORT loc_4_1940
        or     ecx, 8
        mov    dword ptr [esi + 0b7h], ecx
loc_4_1940:
        test   byte ptr [esi + 0b7h], 2
        jne    SHORT loc_4_1974
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 515h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_195B:
        mov    eax, dword ptr [esi + 61fh]
        or     eax, eax
        je     SHORT loc_4_1974
        and    dword ptr [esi + 0b7h], 0fffffff7h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1974:
        test   byte ptr [esi + 0b7h], 4
        jne    SHORT loc_4_1996
        push   esi
        call   sub_4_0730
        or     eax, eax
        je     SHORT loc_4_1996
        and    dword ptr [esi + 0b7h], 0fffffff7h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1996:
        cmp    edi, 2
        jne    NEAR PTR loc_4_1A5B
        mov    eax, dword ptr [esi + 0b7h]
        test   al, 20h
        je     SHORT loc_4_19D0
        and    eax, 0fffffff7h
        mov    dword ptr [esi + 0b7h], eax
        lea    eax, [ebp - 1]
        push   eax
        add    esi, 64bh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_19C2:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
loc_4_19D0:
        push   esi
        call   sub_4_0230
        mov    eax, 103h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_19E3:
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [ebp - 1]
        and    dword ptr [esi + 0b7h], 0ffffffe9h
        push   eax
        lea    eax, [esi + 64bh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_19FD:
        push   0
        push   7
        push   esi
        call   sub_4_01C0
        mov    eax, 103h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1A14:
        mov    esi, dword ptr [ebp + 8]
        and    dword ptr [esi + 0b7h], 0ffffff61h
        or     word ptr [esi + 619h], 8
        mov    eax, dword ptr [esi + 4ebh]
        mov    dword ptr [esi + 4e7h], eax
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 64bh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1A45:
        mov    eax, dword ptr [esi + 549h]
        or     eax, eax
        je     SHORT loc_4_1A59
        cmp    byte ptr [eax + 0eh], 2
        jne    SHORT loc_4_1A59
        mov    byte ptr [eax + 0eh], 3
loc_4_1A59:
        sub    eax, eax
loc_4_1A5B:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h, 0B0h, 018h, 000h, 000h, 0F4h, 018h, 000h, 000h, 005h, 019h, 000h, 000h, 005h, 019h, 000h
        db  000h, 0E3h, 019h, 000h, 000h, 014h, 01Ah, 000h, 000h, 08Dh, 064h, 024h, 000h
sub_4_1A80:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 61h], ecx
        test   byte ptr [eax + 0b7h], 10h
        jne    SHORT loc_4_1A9B
        sub    eax, eax
        pop    ebp
        ret    0ch
loc_4_1A9B:
        push   ecx
        push   8
        push   eax
        call   sub_4_01C0
        or     al, al
        je     SHORT loc_4_1AB5
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 0ch], 8
        mov    byte ptr [eax + 0eh], 2
loc_4_1AB5:
        mov    eax, 103h
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_4_1AC0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 5dh], ecx
        test   byte ptr [eax + 0b7h], 10h
        jne    SHORT loc_4_1ADB
        sub    eax, eax
        pop    ebp
        ret    0ch
loc_4_1ADB:
        push   ecx
        push   9
        push   eax
        call   sub_4_01C0
        or     al, al
        je     SHORT loc_4_1AF5
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 0ch], 9
        mov    byte ptr [eax + 0eh], 2
loc_4_1AF5:
        mov    eax, 103h
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_4_1B00:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 0b7h], 10h
        jne    SHORT loc_4_1B15
        sub    eax, eax
        pop    ebp
        ret    8
loc_4_1B15:
        mov    ecx, dword ptr [eax + 81h]
        push   ecx
        push   0ah
        push   eax
        call   sub_4_01C0
        or     al, al
        je     SHORT loc_4_1B35
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 0ch], 0ah
        mov    byte ptr [eax + 0eh], 2
loc_4_1B35:
        mov    eax, 103h
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_4_1B40:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 0b7h], 10h
        jne    SHORT loc_4_1B55
        sub    eax, eax
        pop    ebp
        ret    8
loc_4_1B55:
        mov    ecx, dword ptr [ebp + 0ch]
        push   4
        mov    word ptr [ecx + 0ch], 7
        mov    byte ptr [ecx + 0eh], 2
        push   eax
        call   sub_4_1890
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_4_1B70:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0b7h], 10h
        jne    SHORT loc_4_1B87
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    0ch
loc_4_1B87:
        mov    eax, dword ptr [ebp + 10h]
        mov    edx, eax
        shr    edx, 10h
        mov    cx, ax
        and    ecx, 0ffh
        shl    ecx, 10h
        mov    cl, dl
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0ffffh
        shl    eax, 10h
        shr    dx, 8
        or     ecx, eax
        mov    eax, edx
        and    eax, 0ffffh
        or     ecx, eax
        push   ecx
        push   0dh
        push   esi
        call   sub_4_01C0
        or     al, al
        je     SHORT loc_4_1BD5
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 0ch], 0dh
        mov    byte ptr [eax + 0eh], 2
loc_4_1BD5:
        mov    eax, 103h
        pop    esi
        pop    ebp
        ret    0ch
        db  090h
sub_4_1BE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   0
        mov    eax, dword ptr [esi + 85h]
        push   eax
        mov    eax, dword ptr [esi + 89h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1BFF:
        cmp    byte ptr [ebp + 0ch], 0
        je     SHORT loc_4_1C1C
        mov    eax, dword ptr [esi + 2a9h]
        or     eax, eax
        je     SHORT loc_4_1C1C
        push   0
        push   2000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1C1C:
        mov    eax, dword ptr [esi + 645h]
        cmp    eax, 6
        ja     SHORT loc_4_1C90
        db  0FFh, 024h, 085h, 098h, 01Ch, 000h, 000h
        db  08Bh, 0FFh
loc_4_1C30:
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [esi + 62dh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1C40:
        push   0
        push   20h
        mov    eax, dword ptr [esi + 67fh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1C50:
        mov    eax, dword ptr [esi + 635h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1C5C:
        mov    eax, dword ptr [esi + 631h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1C68:
        push   0
        push   4bh
        mov    eax, dword ptr [esi + 639h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1C78:
        mov    eax, dword ptr [esi + 62dh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1C84:
        mov    eax, dword ptr [esi + 629h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1C90:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  090h, 084h, 01Ch, 000h, 000h, 078h, 01Ch, 000h, 000h, 068h, 01Ch, 000h, 000h, 05Ch, 01Ch, 000h
        db  000h, 050h, 01Ch, 000h, 000h, 040h, 01Ch, 000h, 000h, 030h, 01Ch, 000h, 000h, 005h, 000h, 000h
        db  000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1CC0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        pop    ebp
        mov    byte ptr [eax + 4e6h], 1
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    dword ptr [esi + 531h], 0
        jle    SHORT loc_4_1D03
        push   64h
        add    esi, 4ceh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1CFE:
        pop    esi
        pop    ebp
        ret    10h
loc_4_1D03:
        push   esi
        call   sub_4_0BF0
        mov    eax, dword ptr [esi + 0b7h]
        test   al, 10h
        je     NEAR PTR loc_4_1D99
        test   dword ptr [esi + 59h], 8020h
        je     SHORT loc_4_1D7D
        test   ah, 2
        je     SHORT loc_4_1D7D
        test   ah, 4
        jne    SHORT loc_4_1D7D
        or     eax, 400h
        mov    dword ptr [esi + 0b7h], eax
        push   80000025h
        push   8100000fh
        push   2
        push   8100000fh
        mov    eax, dword ptr [esi + 4ffh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1D52:
        add    esp, 14h
        xor    eax, eax
        mov    ax, word ptr [esi + 649h]
        mov    dword ptr [esi + 667h], 0bh
        mov    dword ptr [esi + 65fh], eax
        push   0
        lea    eax, [esi + 657h]
        push   eax
        push   esi
        call   sub_4_3530
loc_4_1D7D:
        cmp    word ptr [esi + 619h], 0
        je     SHORT loc_4_1D99
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 515h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1D99:
        push   64h
        add    esi, 4ceh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1DA7:
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h
sub_4_1DB0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        test   dword ptr [esi + 59h], 8020h
        je     SHORT loc_4_1E0D
        test   byte ptr [esi + 0b7h], 10h
        je     SHORT loc_4_1DFC
        mov    ecx, dword ptr [esi + 639h]
        mov    ax, word ptr [esi + 641h]
        mov    word ptr [ecx + 1fh], ax
        mov    ecx, dword ptr [esi + 639h]
        mov    eax, dword ptr [esi + 63dh]
        mov    dword ptr [ecx + 1bh], eax
        mov    eax, dword ptr [esi + 62dh]
        push   eax
        mov    eax, dword ptr [esi + 67fh]
        push   eax
        call   sub_4_4500
loc_4_1DFC:
        push   0fa00h
        add    esi, 64bh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1E0D:
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1E20:
        in     al, dx
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1E30:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dx, word ptr [eax + 10h]
        add    dx, 9
        mov    al, 8
        out    dx, al
        mov    al, 1
        out    dx, al
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_4_1E50:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    di, word ptr [esi + 10h]
        lea    edx, [edi + 6]
        in     al, dx
        mov    cl, al
        test   cl, 80h
        je     SHORT loc_4_1EA6
        sub    al, al
        out    dx, al
        and    cl, 0fh
        mov    byte ptr [esi + 33h], cl
        cmp    cl, 0ch
        jne    SHORT loc_4_1E96
        test   byte ptr [esi + 0b7h], 48h
        je     SHORT loc_4_1E90
        mov    dx, di
        add    dx, 7
        mov    al, 0a1h
        out    dx, al
        sub    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_1E90:
        push   esi
        call   sub_4_4960
loc_4_1E96:
        mov    byte ptr [esi + 29h], 1
        mov    byte ptr [esi + 34h], 0a0h
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_1EA6:
        lea    edx, [edi + 8]
        in     al, dx
        mov    cl, al
        lea    edx, [edi + 9]
        in     al, dx
        mov    dl, al
        and    edx, 0ffff00ffh
        shl    dx, 8
        mov    al, cl
        and    eax, 0ffff00ffh
        or     dx, ax
        mov    word ptr [esi + 31h], dx
        and    dh, 0bh
        cmp    dh, 3
        jne    SHORT loc_4_1EE4
        push   esi
        call   sub_4_15E0
        in     al, dx
        in     al, dx
        in     al, dx
        in     al, dx
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_1EE4:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_4_1EF0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    di, word ptr [esi + 10h]
        lea    edx, [edi + 8]
        in     al, dx
        mov    cl, al
        lea    edx, [edi + 9]
        in     al, dx
        mov    dl, al
        and    edx, 0ffff00ffh
        shl    dx, 8
        mov    al, cl
        and    eax, 0ffff00ffh
        or     dx, ax
        and    dh, 0bh
        cmp    dh, 3
        jne    SHORT loc_4_1F36
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 515h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1F36:
        cmp    byte ptr [esi + 29h], 0
        je     SHORT loc_4_1F69
        mov    byte ptr [esi + 29h], 0
        cmp    byte ptr [esi + 33h], 0ch
        push   esi
        jne    SHORT loc_4_1F52
        call   sub_4_4B70
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
loc_4_1F52:
        call   sub_4_1520
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        add    esi, 515h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1F69:
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
        db  090h
sub_4_1F70:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 1ch], 1
        sub    edi, edi
        mov    dword ptr [ebp - 8], 0
        mov    word ptr [ebp - 2], 0
        mov    dword ptr [ebp - 0ch], edi
        lea    ecx, [ebp - 8]
        mov    esi, 4
        mov    edx, dword ptr [ebp + 10h]
        mov    ebx, edx
        and    ebx, 0ffff0000h
        cmp    ebx, 2020000h
        ja     SHORT loc_4_1FE0
        je     NEAR PTR loc_4_2470
        cmp    ebx, 10000h
        je     SHORT loc_4_2002
        cmp    ebx, 20000h
        je     NEAR PTR loc_4_2213
        cmp    ebx, 2010000h
        je     NEAR PTR loc_4_22C0
        mov    edi, 0c00000bbh
        jmp    NEAR PTR loc_4_256F
        db  08Dh, 049h, 000h
loc_4_1FE0:
        cmp    ebx, 0ff010000h
        je     NEAR PTR loc_4_24F7
        cmp    ebx, 0ff020000h
        je     NEAR PTR loc_4_2470
        mov    edi, 0c00000bbh
        jmp    NEAR PTR loc_4_256F
loc_4_2002:
        lea    eax, [edx - 10101h]
        cmp    eax, 12h
        ja     SHORT loc_4_2014
        db  0FFh, 024h, 085h, 038h, 006h, 000h, 000h
loc_4_2014:
        mov    edi, 0c00000bbh
        jmp    NEAR PTR loc_4_256F
        db  08Bh, 0FFh
loc_4_2020:
        cmp    byte ptr [ebp + 24h], 0
        jne    SHORT loc_4_2035
        db  0B9h, 010h, 001h, 000h, 000h
        mov    esi, 5ch
        jmp    NEAR PTR loc_4_256F
loc_4_2035:
        db  0B9h, 024h, 000h, 000h, 000h
        mov    esi, 0ech
        jmp    NEAR PTR loc_4_256F
loc_4_2044:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0b7h]
        test   al, 20h
        je     SHORT loc_4_205A
        mov    dword ptr [ebp - 0ch], 4
        jmp    SHORT loc_4_208A
loc_4_205A:
        test   al, 40h
        je     SHORT loc_4_2067
        mov    dword ptr [ebp - 0ch], 2
        jmp    SHORT loc_4_208A
loc_4_2067:
        test   al, 4
        jne    SHORT loc_4_2078
        test   al, 8
        je     SHORT loc_4_2078
        mov    dword ptr [ebp - 0ch], 1
        jmp    SHORT loc_4_208A
loc_4_2078:
        test   al, 10h
        mov    dword ptr [ebp - 0ch], 4
        je     SHORT loc_4_208A
        mov    dword ptr [ebp - 0ch], 0
loc_4_208A:
        lea    ecx, [ebp - 0ch]
        mov    esi, 4
        jmp    NEAR PTR loc_4_256F
loc_4_2097:
        lea    ecx, [ebp - 1ch]
        mov    esi, 4
        jmp    NEAR PTR loc_4_256F
loc_4_20A4:
        mov    dword ptr [ebp - 8], 0f0h
        jmp    NEAR PTR loc_4_256F
loc_4_20B0:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 2d5h]
        mov    dword ptr [ebp - 10h], eax
        xor    eax, eax
        mov    ax, word ptr [ebx + 2d7h]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 10h]
        imul   eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebx + 3ah]
        cmp    dword ptr [ebp - 14h], eax
        jae    SHORT loc_4_20E3
        mov    eax, dword ptr [ebp - 14h]
loc_4_20E3:
        mov    dword ptr [ebp - 8], eax
        cmp    edx, 10106h
        jne    NEAR PTR loc_4_256F
        sub    dword ptr [ebp - 8], 0eh
        jmp    NEAR PTR loc_4_256F
        db  005h, 000h, 000h, 000h, 000h
loc_4_2100:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 35h]
        jmp    NEAR PTR loc_4_256C
        db  005h, 000h, 000h, 000h, 000h
loc_4_2110:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0d2h]
        jmp    NEAR PTR loc_4_256C
        db  08Bh, 0FFh
loc_4_2120:
        mov    ebx, dword ptr [ebp + 8]
        xor    edx, edx
        xor    eax, eax
        mov    ax, word ptr [ebx + 2d7h]
        mov    dx, word ptr [ebx + 2d5h]
        imul   edx, eax
        mov    dword ptr [ebp - 8], edx
        jmp    NEAR PTR loc_4_256F
loc_4_2140:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0ceh]
        jmp    NEAR PTR loc_4_256C
        db  08Bh, 0FFh
loc_4_2150:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 2d5h]
        jmp    NEAR PTR loc_4_256C
loc_4_2161:
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 4ah
        mov    esi, 4
        jmp    NEAR PTR loc_4_256F
loc_4_2171:
        db  0B9h, 06Ch, 001h, 000h, 000h
        mov    esi, 1fh
        jmp    NEAR PTR loc_4_256F
loc_4_2180:
        cmp    byte ptr [ebp + 24h], 0
        je     SHORT loc_4_2191
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 59h]
        jmp    NEAR PTR loc_4_256C
loc_4_2191:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    eax, dword ptr [eax + 8]
        jmp    NEAR PTR loc_4_256C
        db  090h
loc_4_21A0:
        cmp    byte ptr [ebp + 24h], 0
        jne    SHORT loc_4_21B8
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 8], eax
        sub    dword ptr [ebp - 8], 20h
        jmp    NEAR PTR loc_4_256F
loc_4_21B8:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, ebx
        mov    edx, dword ptr [ebx + 53dh]
        mov    dword ptr [ebp - 8], 20h
        sub    eax, edx
        cmp    eax, 0fffffac3h
        je     SHORT loc_4_21EB
loc_4_21D3:
        mov    eax, dword ptr [edx + 1ch]
        cmp    dword ptr [ebp - 8], eax
        jae    SHORT loc_4_21DE
        mov    dword ptr [ebp - 8], eax
loc_4_21DE:
        mov    edx, dword ptr [edx]
        mov    eax, ebx
        sub    eax, edx
        cmp    eax, 0fffffac3h
        jne    SHORT loc_4_21D3
loc_4_21EB:
        sub    dword ptr [ebp - 8], 20h
        jmp    NEAR PTR loc_4_256F
loc_4_21F4:
        mov    word ptr [ebp - 2], 300h
        lea    ecx, [ebp - 2]
        mov    esi, 2
        jmp    NEAR PTR loc_4_256F
loc_4_2207:
        mov    dword ptr [ebp - 8], 7
        jmp    NEAR PTR loc_4_256F
loc_4_2213:
        cmp    byte ptr [ebp + 24h], 0
        je     NEAR PTR loc_4_22B0
        mov    eax, edx
        and    eax, 0ffh
        dec    eax
        and    edx, 0ff00h
        cmp    edx, 100h
        je     SHORT loc_4_2245
        cmp    edx, 200h
        je     SHORT loc_4_2263
        mov    edi, 0c00000bbh
        jmp    NEAR PTR loc_4_256F
loc_4_2245:
        cmp    eax, 5
        jb     SHORT loc_4_2254
        mov    edi, 0c00000bbh
        jmp    NEAR PTR loc_4_256F
loc_4_2254:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + eax*4 + 42ah]
        jmp    NEAR PTR loc_4_256C
loc_4_2263:
        cmp    eax, 0eh
        jb     SHORT loc_4_2272
        mov    edi, 0c00000bbh
        jmp    NEAR PTR loc_4_256F
loc_4_2272:
        mov    edx, eax
        and    edx, 0fffffffeh
        cmp    edx, 0ch
        jae    SHORT loc_4_22A1
        test   al, 1
        mov    ebx, dword ptr [ebp + 8]
        je     SHORT loc_4_2290
        mov    eax, dword ptr [ebx + edx*2 + 46eh]
        jmp    NEAR PTR loc_4_256C
        db  090h
loc_4_2290:
        lea    ecx, [ebx + edx*4 + 43eh]
        mov    esi, 8
        jmp    NEAR PTR loc_4_256F
loc_4_22A1:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + eax*4 + 456h]
        jmp    NEAR PTR loc_4_256C
loc_4_22B0:
        mov    edi, 0c00000bbh
        jmp    NEAR PTR loc_4_256F
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_22C0:
        sub    edx, 2010101h
        cmp    edx, 6
        ja     SHORT loc_4_22D2
        db  0FFh, 024h, 095h, 084h, 006h, 000h, 000h
loc_4_22D2:
        mov    edi, 0c00000bbh
        jmp    NEAR PTR loc_4_256F
        db  08Dh, 064h, 024h, 000h
loc_4_22E0:
        mov    ebx, dword ptr [ebp + 8]
        mov    al, byte ptr [ebx + 4ah]
        add    ebx, 4ah
        mov    byte ptr [ebp - 24h], al
        mov    al, byte ptr [ebx + 1]
        mov    byte ptr [ebp - 23h], al
        mov    al, byte ptr [ebx + 2]
        mov    byte ptr [ebp - 22h], al
        lea    ecx, [ebp - 24h]
        mov    al, byte ptr [ebx + 3]
        mov    byte ptr [ebp - 21h], al
        mov    al, byte ptr [ebx + 4]
        mov    byte ptr [ebp - 20h], al
        mov    esi, 6
        mov    al, byte ptr [ebx + 5]
        mov    byte ptr [ebp - 1fh], al
        jmp    NEAR PTR loc_4_256F
loc_4_2317:
        mov    ebx, dword ptr [ebp + 8]
        mov    al, byte ptr [ebx + 50h]
        add    ebx, 50h
        mov    byte ptr [ebp - 24h], al
        mov    al, byte ptr [ebx + 1]
        mov    byte ptr [ebp - 23h], al
        mov    al, byte ptr [ebx + 2]
        mov    byte ptr [ebp - 22h], al
        lea    ecx, [ebp - 24h]
        mov    al, byte ptr [ebx + 3]
        mov    byte ptr [ebp - 21h], al
        mov    al, byte ptr [ebx + 4]
        mov    byte ptr [ebp - 20h], al
        mov    esi, 6
        mov    al, byte ptr [ebx + 5]
        mov    byte ptr [ebp - 1fh], al
        jmp    NEAR PTR loc_4_256F
        db  08Bh, 0FFh
loc_4_2350:
        cmp    byte ptr [ebp + 24h], 0
        jne    SHORT loc_4_2361
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    eax, dword ptr [eax + 0ch]
        jmp    SHORT loc_4_2367
loc_4_2361:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 5dh]
loc_4_2367:
        mov    dword ptr [ebp - 8], eax
        mov    edx, dword ptr [ebp - 8]
        and    edx, 0ff00h
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ff0000h
        mov    ebx, dword ptr [ebp - 8]
        shr    ebx, 10h
        or     eax, ebx
        shr    eax, 8
        or     edx, eax
        mov    dword ptr [ebp - 8], edx
        jmp    NEAR PTR loc_4_256F
        db  005h, 000h, 000h, 000h, 000h
loc_4_23A0:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 61h]
        mov    dword ptr [ebp - 8], eax
        mov    edx, eax
        and    edx, 0ff00h
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ff0000h
        mov    ebx, dword ptr [ebp - 8]
        shr    ebx, 10h
        or     eax, ebx
        shr    eax, 8
        or     edx, eax
        mov    dword ptr [ebp - 8], edx
        jmp    NEAR PTR loc_4_256F
loc_4_23D6:
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 0b5h
        mov    esi, 2
        jmp    NEAR PTR loc_4_256F
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_23F0:
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 0adh
        mov    esi, 2
        jmp    NEAR PTR loc_4_256F
loc_4_2403:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0b7h]
        test   al, 10h
        je     SHORT loc_4_241C
        mov    dword ptr [ebp - 8], 1
        jmp    NEAR PTR loc_4_256F
loc_4_241C:
        test   al, 8
        je     SHORT loc_4_2430
        mov    dword ptr [ebp - 8], 3
        jmp    NEAR PTR loc_4_256F
        db  08Dh, 064h, 024h, 000h
loc_4_2430:
        cmp    word ptr [ebx + 0b5h], 0
        je     SHORT loc_4_2446
        mov    dword ptr [ebp - 8], 5
        jmp    NEAR PTR loc_4_256F
loc_4_2446:
        test   byte ptr [ebx + 0ach], 0dh
        je     SHORT loc_4_2460
        mov    dword ptr [ebp - 8], 6
        jmp    NEAR PTR loc_4_256F
        db  005h, 000h, 000h, 000h, 000h
loc_4_2460:
        mov    dword ptr [ebp - 8], 2
        jmp    NEAR PTR loc_4_256F
        db  08Dh, 064h, 024h, 000h
loc_4_2470:
        cmp    byte ptr [ebp + 24h], 0
        je     SHORT loc_4_24F0
        mov    eax, edx
        and    eax, 0ffh
        dec    eax
        and    edx, 0ff00h
        cmp    edx, 100h
        je     SHORT loc_4_24A0
        cmp    edx, 200h
        je     SHORT loc_4_24C0
        mov    edi, 0c00000bbh
        jmp    NEAR PTR loc_4_256F
        db  08Bh, 0FFh
loc_4_24A0:
        cmp    eax, 2
        jb     SHORT loc_4_24B0
        mov    edi, 0c00000bbh
        jmp    NEAR PTR loc_4_256F
        db  090h
loc_4_24B0:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + eax*4 + 48eh]
        jmp    NEAR PTR loc_4_256C
        db  090h
loc_4_24C0:
        cmp    ebx, 0ff020000h
        jne    SHORT loc_4_24CB
        add    eax, 7
loc_4_24CB:
        cmp    eax, 9
        jb     SHORT loc_4_24E0
        mov    edi, 0c00000bbh
        jmp    NEAR PTR loc_4_256F
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_24E0:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + eax*4 + 496h]
        jmp    NEAR PTR loc_4_256C
        db  090h
loc_4_24F0:
        mov    edi, 0c00000bbh
        jmp    SHORT loc_4_256F
loc_4_24F7:
        cmp    byte ptr [ebp + 24h], 0
        jne    SHORT loc_4_2504
        mov    edi, 0c00000bbh
        jmp    SHORT loc_4_256F
loc_4_2504:
        cmp    edx, 0ff010101h
        je     SHORT loc_4_2533
        cmp    edx, 0ff010102h
        je     SHORT loc_4_2540
        cmp    edx, 0ff010103h
        je     SHORT loc_4_2548
        cmp    edx, 0ff010104h
        je     SHORT loc_4_2551
        cmp    edx, 0ff010105h
        je     SHORT loc_4_2560
        mov    edi, 0c00000bbh
        jmp    SHORT loc_4_256F
loc_4_2533:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 8]
        jmp    SHORT loc_4_256C
        db  08Bh, 0FFh
loc_4_2540:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 20h]
        jmp    SHORT loc_4_256C
loc_4_2548:
        mov    dword ptr [ebp - 8], 0ffffh
        jmp    SHORT loc_4_256F
loc_4_2551:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 539h]
        jmp    SHORT loc_4_256C
        db  08Dh, 064h, 024h, 000h
loc_4_2560:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0b1h]
loc_4_256C:
        mov    dword ptr [ebp - 8], eax
loc_4_256F:
        or     edi, edi
        jne    SHORT loc_4_259D
        cmp    esi, dword ptr [ebp + 18h]
        jbe    SHORT loc_4_258D
        mov    eax, dword ptr [ebp + 20h]
        mov    edi, 0c0010014h
        mov    dword ptr [eax], esi
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
loc_4_258D:
        push   esi
        push   ecx
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2598:
        mov    eax, dword ptr [ebp + 1ch]
        add    dword ptr [eax], esi
loc_4_259D:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  0B0h, 000h, 000h, 000h, 0D4h, 000h, 000h, 000h, 027h, 001h, 000h, 000h, 027h, 001h, 000h, 000h
        db  034h, 001h, 000h, 000h, 040h, 001h, 000h, 000h, 090h, 001h, 000h, 000h, 0A0h, 001h, 000h, 000h
        db  0B0h, 001h, 000h, 000h, 0D0h, 001h, 000h, 000h, 0E0h, 001h, 000h, 000h, 0F1h, 001h, 000h, 000h
        db  001h, 002h, 000h, 000h, 010h, 002h, 000h, 000h, 030h, 002h, 000h, 000h, 084h, 002h, 000h, 000h
        db  040h, 001h, 000h, 000h, 0A4h, 000h, 000h, 000h, 097h, 002h, 000h, 000h, 070h, 003h, 000h, 000h
        db  0A7h, 003h, 000h, 000h, 0E0h, 003h, 000h, 000h, 030h, 004h, 000h, 000h, 066h, 004h, 000h, 000h
        db  080h, 004h, 000h, 000h, 093h, 004h, 000h, 000h
sub_4_2610:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        sub    ebx, ebx
        mov    edi, dword ptr [esi + 1ch]
        mov    eax, dword ptr [esi + 18h]
        mov    ecx, dword ptr [esi + 14h]
        cmp    edi, 4
        je     SHORT loc_4_2644
        mov    ebx, 0c0010014h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_4_2748
loc_4_2644:
        cmp    ecx, 1010eh
        je     SHORT loc_4_2690
        cmp    ecx, 1010fh
        je     SHORT loc_4_26D0
        cmp    ecx, 10112h
        je     NEAR PTR loc_4_2748
        cmp    ecx, 2010103h
        je     NEAR PTR loc_4_2710
        cmp    ecx, 2010104h
        je     NEAR PTR loc_4_272C
        mov    ebx, 0c0010017h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_4_2748
loc_4_2690:
        push   4
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_269C:
        test   dword ptr [ebp - 8], 2056h
        je     SHORT loc_4_26B0
        mov    ebx, 0c00000bbh
        jmp    NEAR PTR loc_4_2748
        db  090h
loc_4_26B0:
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   esi
        mov    eax, dword ptr [eax + 10h]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ecx + 545h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_26CD:
        jmp    SHORT loc_4_2746
        db  090h
loc_4_26D0:
        push   4
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_26DC:
        cmp    dword ptr [ebp - 4], 0f0h
        ja     SHORT loc_4_2708
        add    dword ptr [ebp - 4], 20h
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 1ch], ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ecx + 535h], eax
        jae    SHORT loc_4_2748
        mov    dword ptr [ecx + 535h], eax
        jmp    SHORT loc_4_2748
loc_4_2708:
        mov    ebx, 0c0010014h
        jmp    SHORT loc_4_2748
        db  090h
loc_4_2710:
        push   1
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   esi
        mov    eax, dword ptr [eax + 10h]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ecx + 545h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_272A:
        jmp    SHORT loc_4_2746
loc_4_272C:
        push   1
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   esi
        mov    eax, dword ptr [eax + 10h]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ecx + 545h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2746:
        mov    ebx, eax
loc_4_2748:
        or     ebx, ebx
        jne    SHORT loc_4_2756
        mov    dword ptr [esi + 20h], edi
        mov    dword ptr [esi + 24h], 0
loc_4_2756:
        cmp    ebx, 103h
        je     SHORT loc_4_2768
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_4_28A0
loc_4_2768:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2780:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    esi, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esi + 18h]
        mov    edi, dword ptr [esi + 14h]
        mov    dl, byte ptr [ebp + 14h]
        or     dl, dl
        je     SHORT loc_4_27E0
        cmp    byte ptr [ebp + 18h], 0
        je     SHORT loc_4_27E0
        mov    ebx, edi
        and    ebx, 0ffff0000h
        cmp    ebx, 2020000h
        je     SHORT loc_4_27C8
        cmp    ebx, 0ff020000h
        jne    SHORT loc_4_27E0
loc_4_27C8:
        push   esi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_4_1B00
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_4_27E0:
        push   edx
        lea    edx, [ebp - 4]
        push   edx
        lea    edx, [ebp - 8]
        push   edx
        push   eax
        push   ecx
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_4_1F70
        mov    ebx, eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 20h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 24h], eax
        cmp    byte ptr [ebp + 18h], 0
        je     SHORT loc_4_2814
        push   ebx
        push   edi
        call   sub_4_28A0
loc_4_2814:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  090h
sub_4_2820:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 10h]
        or     eax, eax
        jne    SHORT loc_4_2845
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [ecx + 10h], 2
        jne    SHORT loc_4_2845
        push   0
        push   1
        push   ecx
        mov    eax, dword ptr [ecx + 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_4_2780
loc_4_2845:
        pop    ebp
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2850:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ecx + 10h]
        or     eax, eax
        jl     SHORT loc_4_2894
        cmp    eax, 1
        jle    SHORT loc_4_286B
        cmp    eax, 2
        je     SHORT loc_4_2880
        pop    ebp
        ret    10h
loc_4_286B:
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   ecx
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_287C:
        pop    ebp
        ret    10h
loc_4_2880:
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   ecx
        mov    eax, dword ptr [eax + 4ffh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2894:
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_28A0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 549h]
        mov    byte ptr [ecx + 0eh], 0
        mov    eax, dword ptr [ecx]
        or     eax, eax
        mov    dword ptr [esi + 549h], eax
        jne    SHORT loc_4_28D1
        mov    dword ptr [esi + 54dh], 0
        and    word ptr [esi + 619h], 0fffdh
loc_4_28D1:
        mov    edi, dword ptr [ecx + 4]
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   ecx
        push   edi
        push   esi
        call   sub_4_2850
        or     edi, edi
        je     SHORT loc_4_28E7
        dec    dword ptr [edi + 14h]
loc_4_28E7:
        or     word ptr [esi + 619h], 10h
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2900:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 549h]
        or     eax, eax
        je     SHORT loc_4_2943
        mov    edi, dword ptr [ebp + 0ch]
        mov    ebx, 2
loc_4_291B:
        cmp    dword ptr [eax + 10h], ebx
        jne    SHORT loc_4_2930
        push   0
        push   1
        push   eax
        push   0
        push   esi
        call   sub_4_2780
        jmp    SHORT loc_4_2932
        db  090h
loc_4_2930:
        mov    eax, edi
loc_4_2932:
        push   eax
        push   esi
        call   sub_4_28A0
        mov    eax, dword ptr [esi + 549h]
        or     eax, eax
        jne    SHORT loc_4_291B
loc_4_2943:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_2950:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 549h]
        mov    al, byte ptr [ecx + 0eh]
        cmp    al, 3
        jne    SHORT loc_4_2984
        mov    eax, dword ptr [esi + 61bh]
        push   eax
        push   ecx
        push   esi
        call   sub_4_2820
        mov    eax, dword ptr [esi + 61bh]
        push   eax
        push   esi
        call   sub_4_28A0
        pop    esi
        pop    ebp
        ret    4
loc_4_2984:
        cmp    al, 1
        jne    NEAR PTR loc_4_2A61
        test   byte ptr [esi + 0b7h], 8
        jne    NEAR PTR loc_4_2A61
        mov    eax, dword ptr [ecx + 10h]
        cmp    eax, 0bh
        ja     NEAR PTR loc_4_2A61
        db  0FFh, 024h, 085h, 0F8h, 00Ah, 000h, 000h
loc_4_29AC:
        push   1
        push   0
        push   ecx
        mov    eax, dword ptr [ecx + 4]
        push   eax
        push   esi
        call   sub_4_2780
        pop    esi
        pop    ebp
        ret    4
loc_4_29C0:
        push   ecx
        mov    eax, dword ptr [ecx + 4]
        push   eax
        push   esi
        call   sub_4_2610
        pop    esi
        pop    ebp
        ret    4
loc_4_29D0:
        push   1
        push   1
        push   ecx
        push   0
        push   esi
        call   sub_4_2780
        pop    esi
        pop    ebp
        ret    4
loc_4_29E2:
        push   ecx
        push   esi
        call   sub_4_1B40
        or     eax, eax
        jne    SHORT loc_4_2A61
        push   0
        push   esi
        call   sub_4_28A0
        pop    esi
        pop    ebp
        ret    4
loc_4_29FA:
        mov    eax, dword ptr [ecx + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_1AC0
        or     eax, eax
        jne    SHORT loc_4_2A61
        push   0
        push   esi
        call   sub_4_28A0
        pop    esi
        pop    ebp
        ret    4
loc_4_2A16:
        mov    eax, dword ptr [ecx + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_1A80
        or     eax, eax
        jne    SHORT loc_4_2A61
        push   0
        push   esi
        call   sub_4_28A0
        pop    esi
        pop    ebp
        ret    4
loc_4_2A32:
        push   ecx
        push   esi
        call   sub_4_1B00
        or     eax, eax
        jne    SHORT loc_4_2A61
        push   0
        push   esi
        call   sub_4_28A0
        pop    esi
        pop    ebp
        ret    4
loc_4_2A4A:
        mov    eax, dword ptr [ecx + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_1B70
        or     eax, eax
        jne    SHORT loc_4_2A61
        push   0
        push   esi
        call   sub_4_28A0
loc_4_2A61:
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 03Ch, 00Ah, 000h, 000h, 050h, 00Ah, 000h, 000h, 060h, 00Ah, 000h, 000h, 0F1h, 00Ah
        db  000h, 000h, 072h, 00Ah, 000h, 000h, 0F1h, 00Ah, 000h, 000h, 0F1h, 00Ah, 000h, 000h, 0F1h, 00Ah
        db  000h, 000h, 08Ah, 00Ah, 000h, 000h, 0A6h, 00Ah, 000h, 000h, 0C2h, 00Ah, 000h, 000h, 0DAh, 00Ah
        db  000h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_2AA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
loc_4_2AAC:
        mov    byte ptr [ebp - 1], 1
        and    word ptr [esi + 619h], 0fffbh
        mov    edi, dword ptr [esi + 53dh]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0fffffac3h
        je     NEAR PTR loc_4_2B7E
        sub    ebx, ebx
loc_4_2AD0:
        test   byte ptr [edi + 18h], 4
        je     SHORT loc_4_2B3C
        or     word ptr [esi + 619h], 4
        mov    eax, dword ptr [edi + 18h]
        test   al, 8
        je     SHORT loc_4_2B17
        cmp    byte ptr [esi + 55fh], bl
        jne    SHORT loc_4_2B17
        cmp    byte ptr [esi + 587h], bl
        jne    SHORT loc_4_2B17
        cmp    byte ptr [esi + 5afh], bl
        jne    SHORT loc_4_2B17
        and    eax, 0fffffff7h
        mov    dword ptr [edi + 18h], eax
        push   ebx
        mov    eax, dword ptr [edi + 10h]
        push   eax
        mov    eax, dword ptr [esi + 545h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2B14:
        dec    dword ptr [edi + 14h]
loc_4_2B17:
        cmp    dword ptr [edi + 14h], ebx
        jne    SHORT loc_4_2B3C
        cmp    dword ptr [esi + 539h], 1
        ja     SHORT loc_4_2B50
        test   byte ptr [esi + 0b7h], 10h
        je     SHORT loc_4_2B50
        push   edi
        lea    eax, [esi + 5f1h]
        push   eax
        push   esi
        call   sub_4_3530
loc_4_2B3C:
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0fffffac3h
        jne    SHORT loc_4_2AD0
        jmp    SHORT loc_4_2B7E
        db  005h, 000h, 000h, 000h, 000h
loc_4_2B50:
        dec    dword ptr [esi + 539h]
        mov    ecx, dword ptr [edi + 4]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ecx + 4], eax
        push   0
        mov    eax, dword ptr [edi + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2B70:
        push   0
        push   20h
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2B7A:
        mov    byte ptr [ebp - 1], 0
loc_4_2B7E:
        cmp    byte ptr [ebp - 1], 0
        je     NEAR PTR loc_4_2AAC
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2BA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    esi, dword ptr [ebp + 8]
        sub    ebx, ebx
        test   byte ptr [esi + 0b7h], 20h
        je     SHORT loc_4_2BD4
        mov    dword ptr [ebp - 4], 0c0010007h
        xor    ebx, ebx
        mov    bx, word ptr [esi + 0b5h]
        or     ebx, 0c0011000h
loc_4_2BD4:
        mov    edi, dword ptr [esi + 53dh]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0fffffac3h
        je     SHORT loc_4_2C44
loc_4_2BE5:
        mov    eax, dword ptr [edi + 18h]
        test   al, 1
        je     SHORT loc_4_2C37
        and    eax, 0fffffffeh
        mov    dword ptr [edi + 18h], eax
        or     eax, 2
        mov    dword ptr [edi + 18h], eax
        push   ebx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [edi + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2C06:
        test   byte ptr [esi + 0b7h], 20h
        je     SHORT loc_4_2C37
        mov    ecx, dword ptr [edi + 4]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ecx + 4], eax
        push   0
        push   20h
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2C28:
        mov    eax, dword ptr [esi + 539h]
        dec    eax
        mov    dword ptr [esi + 539h], eax
        je     SHORT loc_4_2C44
loc_4_2C37:
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0fffffac3h
        jne    SHORT loc_4_2BE5
loc_4_2C44:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_4_2C50:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0c0000001h
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_36B0
        push   esi
        call   sub_4_2BA0
        and    word ptr [esi + 619h], 0fff7h
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2C80:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   80010004h
        and    dword ptr [esi + 0b7h], 0ffffffbfh
        mov    eax, dword ptr [esi + 625h]
        and    word ptr [esi + 619h], 0fffeh
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2CAB:
        or     word ptr [esi + 619h], 18h
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_2CC0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [esi + 623h]
        cmp    eax, 7
        ja     NEAR PTR loc_4_2D6F
        db  0FFh, 024h, 085h, 04Ch, 00Eh, 000h, 000h
loc_4_2CE0:
        mov    word ptr [esi + 623h], 1
        mov    dword ptr [esi + 4ebh], 0ah
        mov    dword ptr [esi + 4e7h], 0ah
loc_4_2CFD:
        push   0
        push   esi
        call   sub_4_1890
        or     eax, eax
        je     SHORT loc_4_2D74
        push   esi
        call   sub_4_2C80
        pop    esi
        pop    ebp
        ret    4
loc_4_2D14:
        test   byte ptr [esi + 0b7h], 10h
        jne    SHORT loc_4_2D30
        push   esi
        call   sub_4_2C80
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_4_2D30:
        mov    word ptr [esi + 623h], 7
loc_4_2D39:
        push   0
        push   0
        push   40010005h
        push   esi
        call   sub_4_34D0
        and    dword ptr [esi + 0b7h], 0ffffffbfh
        and    word ptr [esi + 619h], 0fffeh
        mov    eax, dword ptr [esi + 625h]
        push   0
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2D69:
        push   esi
        call   sub_4_2BA0
loc_4_2D6F:
        pop    esi
        pop    ebp
        ret    4
loc_4_2D74:
        mov    word ptr [esi + 623h], 2
loc_4_2D7D:
        push   0
        push   0
        push   40010004h
        push   esi
        call   sub_4_34D0
        mov    word ptr [esi + 623h], 3
loc_4_2D95:
        push   0c001000ch
        push   esi
        call   sub_4_36B0
        push   0c001000ch
        push   esi
        call   sub_4_2900
        mov    word ptr [esi + 623h], 4
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 070h, 00Dh, 000h, 000h, 08Dh, 00Dh, 000h, 000h, 00Dh, 00Eh, 000h, 000h, 025h
        db  00Eh, 000h, 000h, 0FFh, 00Dh, 000h, 000h, 0FFh, 00Dh, 000h, 000h, 0A4h, 00Dh, 000h, 000h, 0C9h
        db  00Dh, 000h, 000h, 08Dh, 064h, 024h, 000h
loc_4_2DE0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 10h]
        cmp    byte ptr [ebp + 18h], 0
        mov    esi, dword ptr [ecx + 8]
        je     SHORT loc_4_2E03
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        push   esi
        call   sub_4_1AC0
        pop    esi
        pop    ebp
        ret    14h
loc_4_2E03:
        lea    edx, [esi + 551h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 8], eax
        push   ecx
        push   edx
        push   esi
        call   sub_4_3530
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    14h
        db  08Bh, 0FFh
loc_4_2E20:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 10h]
        cmp    byte ptr [ebp + 18h], 0
        mov    esi, dword ptr [ecx + 8]
        je     SHORT loc_4_2E43
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        push   esi
        call   sub_4_1A80
        pop    esi
        pop    ebp
        ret    14h
loc_4_2E43:
        lea    edx, [esi + 579h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 8], eax
        push   ecx
        push   edx
        push   esi
        call   sub_4_3530
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    14h
        db  08Bh, 0FFh
loc_4_2E60:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [esi + 8]
        test   eax, 8020h
        jne    SHORT loc_4_2E7E
        and    dword ptr [edx + 0b7h], 0fffffdffh
loc_4_2E7E:
        test   byte ptr [edx + 0b8h], 2
        je     SHORT loc_4_2E8C
        and    eax, 0ffff7fdfh
loc_4_2E8C:
        mov    ecx, dword ptr [edx + 59h]
        xor    ecx, eax
        test   ecx, 8020h
        mov    dword ptr [edx + 59h], eax
        je     SHORT loc_4_2EEB
        xor    ecx, ecx
        mov    cx, word ptr [edx + 649h]
        test   al, 20h
        je     SHORT loc_4_2EAF
        or     ecx, 200h
loc_4_2EAF:
        test   ah, 80h
        je     SHORT loc_4_2EBA
        or     ecx, 400h
loc_4_2EBA:
        cmp    byte ptr [ebp + 18h], 0
        je     SHORT loc_4_2ED0
        push   ecx
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        push   edx
        call   sub_4_1B70
        pop    esi
        pop    ebp
        ret    14h
loc_4_2ED0:
        mov    dword ptr [edx + 5a9h], ecx
        lea    eax, [edx + 5a1h]
        push   esi
        push   eax
        push   edx
        call   sub_4_3530
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    14h
loc_4_2EEB:
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_2F00:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_2F10:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        push   0
        mov    edi, dword ptr [ebx + 8]
        push   0
        push   ebx
        call   sub_4_32F0
        mov    esi, eax
        cmp    esi, 103h
        jne    SHORT loc_4_2F55
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 10h]
        or     ecx, ecx
        je     SHORT loc_4_2F4D
        cmp    ecx, 1
        je     SHORT loc_4_2F4D
        mov    esi, 0c00000bbh
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_4_2F4D:
        push   ebx
        push   eax
        push   edi
        call   sub_4_3530
loc_4_2F55:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_4_2F60:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 0b7h], 10h
        je     SHORT loc_4_2F93
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [ecx + 10h], 2
        jne    SHORT loc_4_2F8A
        push   0
        push   ecx
        push   eax
        call   sub_4_3530
        mov    eax, 103h
        pop    ebp
        ret    8
loc_4_2F8A:
        mov    eax, 0c00000bbh
        pop    ebp
        ret    8
loc_4_2F93:
        push   0
        push   1
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        push   0
        push   eax
        call   sub_4_2780
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_2FB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    edx, edx
        mov    dword ptr [esi], 0
        mov    ecx, dword ptr [ebp + 18h]
        cmp    ecx, edx
        je     SHORT loc_4_2FE1
        mov    eax, dword ptr [ebp + 14h]
loc_4_2FD0:
        cmp    dword ptr [eax], 1
        je     SHORT loc_4_2FDD
        add    eax, 4
        inc    edx
        cmp    ecx, edx
        ja     SHORT loc_4_2FD0
loc_4_2FDD:
        cmp    ecx, edx
        jne    SHORT loc_4_2FF0
loc_4_2FE1:
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Bh, 0FFh
loc_4_2FF0:
        mov    eax, dword ptr [ebp + 10h]
        mov    edi, dword ptr [ebp + 20h]
        mov    dword ptr [eax], edx
        test   byte ptr [edi + 0b7h], 20h
        je     SHORT loc_4_3048
        xor    eax, eax
        mov    ax, word ptr [edi + 0b5h]
        push   eax
        push   0c000001eh
        push   81000008h
        push   3
        push   81000008h
        mov    eax, dword ptr [edi + 4ffh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3028:
        add    esp, 18h
        xor    eax, eax
        mov    ax, word ptr [edi + 0b5h]
        or     eax, 0c0011000h
        mov    dword ptr [esi], eax
        mov    eax, 0c0010007h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_4_3048:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   0
        push   20h
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_305B:
        or     eax, eax
        je     SHORT loc_4_3089
        push   0c000000fh
        push   6
        push   2
        push   0aa000002h
        mov    eax, dword ptr [edi + 4ffh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3079:
        add    esp, 14h
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_4_3089:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [eax + 0ch], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], edi
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 18h], 1
        mov    eax, dword ptr [ebp - 4]
        add    eax, 10h
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 545h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_30E2:
        or     al, al
        jne    SHORT loc_4_3120
        push   0c000001dh
        push   6
        push   2
        push   0aa000002h
        mov    eax, dword ptr [edi + 4ffh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3100:
        add    esp, 14h
        push   0
        push   20h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3110:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Dh, 049h, 000h
loc_4_3120:
        mov    eax, dword ptr [ebp - 4]
        or     dword ptr [eax + 18h], 8
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 1ch], 0f0h
        mov    ecx, dword ptr [ebp - 4]
        lea    eax, [edi + 53dh]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [edi + 541h]
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [edi + 541h]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edi + 541h], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 14h]
        mov    eax, 103h
        inc    dword ptr [edi + 539h]
        test   byte ptr [edi + 0b7h], 40h
        jne    SHORT loc_4_317E
        push   2
        push   edi
        call   sub_4_1890
loc_4_317E:
        cmp    eax, 103h
        je     SHORT loc_4_3195
        mov    eax, dword ptr [ebp - 4]
        and    dword ptr [eax + 18h], 0fffffffeh
        mov    eax, dword ptr [ebp - 4]
        or     dword ptr [eax + 18h], 2
        sub    eax, eax
loc_4_3195:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Dh, 049h, 000h
loc_4_31A0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 8]
        mov    eax, dword ptr [edx + 18h]
        test   al, 4
        je     SHORT loc_4_31C0
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_4_31C0:
        and    eax, 0fffffffdh
        mov    dword ptr [edx + 18h], eax
        or     eax, 4
        mov    dword ptr [edx + 18h], eax
        mov    edx, dword ptr [ecx + 53dh]
        mov    dword ptr [ecx + 535h], 0f0h
        mov    eax, ecx
        sub    eax, edx
        cmp    eax, 0fffffac3h
        je     SHORT loc_4_3211
        mov    esi, 3
loc_4_31EC:
        mov    eax, edx
        mov    edx, dword ptr [edx]
        test   dword ptr [eax + 18h], esi
        je     SHORT loc_4_3206
        mov    eax, dword ptr [eax + 1ch]
        cmp    dword ptr [ecx + 535h], eax
        jae    SHORT loc_4_3206
        mov    dword ptr [ecx + 535h], eax
loc_4_3206:
        mov    eax, ecx
        sub    eax, edx
        cmp    eax, 0fffffac3h
        jne    SHORT loc_4_31EC
loc_4_3211:
        mov    dx, word ptr [ecx + 619h]
        or     dx, 4
        mov    word ptr [ecx + 619h], dx
        mov    eax, dword ptr [ecx + 0b7h]
        test   al, 4
        je     SHORT loc_4_3250
        test   eax, 2148h
        jne    SHORT loc_4_3250
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        add    ecx, 515h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3246:
        mov    eax, 103h
        pop    esi
        pop    ebp
        ret    4
loc_4_3250:
        or     dx, 10h
        mov    word ptr [ecx + 619h], dx
        mov    eax, 103h
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_3270:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        push   0
        mov    edi, dword ptr [ebx + 8]
        push   1
        push   ebx
        call   sub_4_32F0
        mov    esi, eax
        cmp    esi, 103h
        jne    SHORT loc_4_32DF
        mov    dword ptr [edi + 625h], ebx
        or     word ptr [edi + 619h], 1
        mov    word ptr [edi + 623h], 0
        mov    eax, dword ptr [edi + 0b7h]
        test   al, 4
        je     SHORT loc_4_32D0
        test   eax, 2148h
        jne    SHORT loc_4_32D0
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [edi + 515h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_32CA:
        jmp    SHORT loc_4_32D8
        db  08Dh, 064h, 024h, 000h
loc_4_32D0:
        or     word ptr [edi + 619h], 10h
loc_4_32D8:
        or     dword ptr [edi + 0b7h], 40h
loc_4_32DF:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_32F0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 18h]
        test   al, 2
        jne    SHORT loc_4_3320
        test   al, 4
        je     SHORT loc_4_3310
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    0ch
        db  090h
loc_4_3310:
        mov    eax, 0c0010011h
        pop    esi
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_3320:
        mov    eax, 103h
        mov    ecx, dword ptr [edx + 0b7h]
        test   cl, 40h
        je     SHORT loc_4_3353
        cmp    byte ptr [ebp + 0ch], 0
        jne    SHORT loc_4_3349
        mov    cx, word ptr [edx + 623h]
        cmp    cx, 2
        jbe    SHORT loc_4_3379
        cmp    cx, 7
        jae    SHORT loc_4_3379
loc_4_3349:
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    0ch
loc_4_3353:
        and    ecx, 10h
        cmp    ecx, 1
        sbb    ecx, ecx
        neg    ecx
        mov    esi, dword ptr [ebp + 10h]
        and    esi, 0ffh
        test   esi, ecx
        je     SHORT loc_4_3379
        cmp    byte ptr [edx + 0e6h], 1
        cmc
        sbb    eax, eax
        and    eax, 0c0000001h
loc_4_3379:
        pop    esi
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_4_3380:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        or     dword ptr [esi + 0b7h], 100h
        mov    ax, word ptr [esi + 619h]
        or     ax, 10h
        mov    word ptr [esi + 619h], ax
        test   al, 10h
        je     SHORT loc_4_33F6
loc_4_33A7:
        mov    ax, word ptr [esi + 619h]
        and    ax, 0ffefh
        mov    word ptr [esi + 619h], ax
        test   al, 1
        je     SHORT loc_4_33C5
        push   esi
        call   sub_4_2CC0
        jmp    SHORT loc_4_33ED
loc_4_33C5:
        test   al, 8
        je     SHORT loc_4_33CF
        push   esi
        call   sub_4_2C50
loc_4_33CF:
        test   byte ptr [esi + 619h], 2
        je     SHORT loc_4_33DE
        push   esi
        call   sub_4_2950
loc_4_33DE:
        test   byte ptr [esi + 619h], 4
        je     SHORT loc_4_33ED
        push   esi
        call   sub_4_2AA0
loc_4_33ED:
        test   byte ptr [esi + 619h], 10h
        jne    SHORT loc_4_33A7
loc_4_33F6:
        and    dword ptr [esi + 0b7h], 0fffffeffh
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3410:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 5d7h], 0
        jne    SHORT loc_4_342E
        push   0
        lea    ecx, [eax + 5c9h]
        push   ecx
        push   eax
        call   sub_4_3530
loc_4_342E:
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3440:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        mov    esi, 3
        mov    ecx, dword ptr [edx + 53dh]
loc_4_3452:
        mov    eax, edx
        sub    eax, ecx
        cmp    eax, 0fffffac3h
        je     SHORT loc_4_346D
        mov    eax, ecx
        mov    ecx, dword ptr [ecx]
        test   dword ptr [eax + 18h], esi
        je     SHORT loc_4_3452
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
loc_4_346D:
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3480:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0b7h], 40h
        je     SHORT loc_4_34A8
        cmp    word ptr [esi + 623h], 5
        jne    SHORT loc_4_34A8
        mov    word ptr [esi + 623h], 6
        pop    esi
        pop    ebp
        ret    4
loc_4_34A8:
        push   esi
        call   sub_4_2BA0
        test   byte ptr [esi + 0b7h], 10h
        jne    SHORT loc_4_34BF
        or     word ptr [esi + 619h], 8
loc_4_34BF:
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_34D0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    edi, dword ptr [eax + 53dh]
        sub    eax, edi
        cmp    eax, 0fffffac3h
        je     SHORT loc_4_351C
        mov    esi, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [ebp + 10h]
        mov    edi, edi
loc_4_34F0:
        test   byte ptr [edi + 18h], 2
        je     SHORT loc_4_350E
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        push   ebx
        push   esi
        mov    eax, dword ptr [edi + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3505:
        mov    eax, dword ptr [edi + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_350E:
        mov    edi, dword ptr [edi]
        mov    eax, dword ptr [ebp + 8]
        sub    eax, edi
        cmp    eax, 0fffffac3h
        jne    SHORT loc_4_34F0
loc_4_351C:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3530:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 4], esi
        mov    byte ptr [eax + 0eh], 1
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        cmp    dword ptr [edx + 549h], 0
        jne    SHORT loc_4_3560
        mov    dword ptr [edx + 549h], eax
        jmp    SHORT loc_4_3568
        db  005h, 000h, 000h, 000h, 000h
loc_4_3560:
        mov    ecx, dword ptr [edx + 54dh]
        mov    dword ptr [ecx], eax
loc_4_3568:
        mov    dword ptr [edx + 54dh], eax
        mov    cx, word ptr [edx + 619h]
        or     cx, 12h
        mov    word ptr [edx + 619h], cx
        mov    eax, dword ptr [edx + 0b7h]
        test   al, 4
        je     SHORT loc_4_35A5
        test   eax, 2148h
        jne    SHORT loc_4_35A5
        push   edx
        db  068h, 000h, 000h, 000h, 000h
        add    edx, 515h
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_35A3:
        jmp    SHORT loc_4_35B0
loc_4_35A5:
        or     cx, 10h
        mov    word ptr [edx + 619h], cx
loc_4_35B0:
        or     esi, esi
        je     SHORT loc_4_35B7
        inc    dword ptr [esi + 14h]
loc_4_35B7:
        pop    esi
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h
sub_4_35C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        cmp    dword ptr [ebp + 0ch], 8
        jb     SHORT loc_4_3623
        mov    eax, dword ptr [ebp + 8]
        mov    cl, byte ptr [eax + 2]
        test   cl, 80h
        je     SHORT loc_4_3623
        cmp    cl, 0ffh
        jne    SHORT loc_4_35E2
        cmp    byte ptr [eax + 3], 0ffh
        je     SHORT loc_4_35ED
loc_4_35E2:
        cmp    cl, 0c0h
        jne    SHORT loc_4_3609
        cmp    byte ptr [eax + 3], 0
        jne    SHORT loc_4_3609
loc_4_35ED:
        cmp    byte ptr [eax + 4], 0ffh
        jne    SHORT loc_4_3609
        cmp    byte ptr [eax + 5], 0ffh
        jne    SHORT loc_4_3609
        cmp    byte ptr [eax + 6], 0ffh
        jne    SHORT loc_4_3609
        cmp    byte ptr [eax + 7], 0ffh
        mov    byte ptr [ebp - 1], 1
        je     SHORT loc_4_360D
loc_4_3609:
        mov    byte ptr [ebp - 1], 0
loc_4_360D:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_4_361B
        mov    al, 1
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_361B:
        mov    al, 2
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_3623:
        sub    al, al
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_4_3630:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 10h]
        cmp    ecx, 8
        mov    eax, dword ptr [ebp + 0ch]
        jb     SHORT loc_4_3648
        test   byte ptr [eax + 2], 80h
        jne    SHORT loc_4_3648
        sub    eax, eax
        jmp    SHORT loc_4_3654
loc_4_3648:
        push   ecx
        push   eax
        call   sub_4_35C0
        and    eax, 0ffh
loc_4_3654:
        mov    edx, dword ptr [ebp + 8]
        inc    dword ptr [edx + 42ah]
        or     eax, eax
        jne    SHORT loc_4_3670
        lea    eax, [edx + 46eh]
        add    edx, 43eh
        jmp    SHORT loc_4_368F
        db  090h
loc_4_3670:
        cmp    eax, 1
        jne    SHORT loc_4_3683
        lea    eax, [edx + 476h]
        add    edx, 44eh
        jmp    SHORT loc_4_368F
loc_4_3683:
        lea    eax, [edx + 472h]
        add    edx, 446h
loc_4_368F:
        inc    dword ptr [eax]
        mov    eax, dword ptr [edx]
        mov    ecx, dword ptr [ebp + 14h]
        add    eax, ecx
        mov    dword ptr [edx], eax
        cmp    ecx, eax
        jbe    SHORT loc_4_36A1
        inc    dword ptr [edx + 4]
loc_4_36A1:
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_36B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, 0
        cmp    byte ptr [esi + 0e6h], 0
        mov    word ptr [esi + 0e4h], 0
        mov    dword ptr [esi + 0deh], 0
        je     SHORT loc_4_36DD
        mov    edi, dword ptr [ebp + 0ch]
loc_4_36DD:
        cmp    word ptr [esi + 0e2h], 0
        je     SHORT loc_4_3738
loc_4_36E7:
        mov    ecx, dword ptr [esi + 0d6h]
        dec    word ptr [esi + 0e2h]
        mov    ebx, dword ptr [ecx + 28h]
        lea    eax, [ecx + 24h]
        inc    dword ptr [esi + 432h]
        cmp    dword ptr [esi + 0dah], ecx
        jne    SHORT loc_4_3712
        mov    dword ptr [esi + 0dah], 0
loc_4_3712:
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 0d6h], eax
        test   byte ptr [ebx + 19h], 80h
        jne    SHORT loc_4_372E
        push   edi
        push   ecx
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_372B:
        dec    dword ptr [ebx + 14h]
loc_4_372E:
        cmp    word ptr [esi + 0e2h], 0
        jne    SHORT loc_4_36E7
loc_4_3738:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  090h
sub_4_3740:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 0ebh]
        cmp    word ptr [esi + 0e9h], ax
        je     NEAR PTR loc_4_3846
loc_4_3760:
        mov    edi, dword ptr [esi + 0d6h]
        cmp    byte ptr [esi + 0e6h], 0
        lea    eax, [edi + 24h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 4], eax
        je     SHORT loc_4_379B
        mov    ax, word ptr [esi + 0ebh]
        and    eax, 7fh
        mov    al, byte ptr [esi + eax + 1f3h]
        sub    al, 0cch
        cmp    al, 1
        cmc
        sbb    ebx, ebx
        and    ebx, 10001h
        jmp    SHORT loc_4_379D
loc_4_379B:
        sub    ebx, ebx
loc_4_379D:
        push   0
        lea    eax, [ebp - 14h]
        push   eax
        push   0
        push   0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_37AD:
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_37BE:
        or     ebx, ebx
        jne    SHORT loc_4_37E0
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        push   esi
        call   sub_4_3630
        jmp    SHORT loc_4_37E6
        db  090h
loc_4_37E0:
        inc    dword ptr [esi + 432h]
loc_4_37E6:
        cmp    dword ptr [esi + 0dah], edi
        jne    SHORT loc_4_37F8
        mov    dword ptr [esi + 0dah], 0
loc_4_37F8:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 0d6h], eax
        dec    word ptr [esi + 0e2h]
        dec    word ptr [esi + 0e4h]
        mov    eax, dword ptr [ebp - 4]
        inc    word ptr [esi + 0ebh]
        test   byte ptr [eax + 19h], 80h
        jne    SHORT loc_4_3832
        push   ebx
        push   edi
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_382C:
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [eax + 14h]
loc_4_3832:
        mov    ax, word ptr [esi + 0ebh]
        cmp    word ptr [esi + 0e9h], ax
        jne    NEAR PTR loc_4_3760
loc_4_3846:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_4_3850:
        push   ebp
        mov    ebp, esp
        push   esi
        sub    ecx, ecx
        mov    ax, word ptr [ebp + 10h]
        cmp    ax, 15h
        jb     SHORT loc_4_38C6
        cmp    ax, 25h
        ja     SHORT loc_4_38C6
        mov    edx, dword ptr [ebp + 0ch]
        cmp    byte ptr [edx + 1], 40h
        jne    SHORT loc_4_38C6
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 54h]
        cmp    word ptr [edx + 6], ax
        jne    SHORT loc_4_38C6
        mov    eax, dword ptr [esi + 50h]
        cmp    dword ptr [edx + 2], eax
        jne    SHORT loc_4_38C6
        test   byte ptr [edx + 8], 80h
        je     SHORT loc_4_3890
        mov    cl, byte ptr [edx + 0eh]
        and    ecx, 1fh
loc_4_3890:
        lea    eax, [ecx + edx]
        mov    cl, byte ptr [eax + 0eh]
        and    cl, 0efh
        cmp    cl, 0e4h
        jne    SHORT loc_4_38C6
        cmp    byte ptr [eax + 10h], 3
        jne    SHORT loc_4_38C6
        cmp    byte ptr [eax + 11h], 0
        jne    SHORT loc_4_38C6
        cmp    byte ptr [eax + 12h], 4
        jne    SHORT loc_4_38C6
        cmp    byte ptr [eax + 13h], 90h
        jne    SHORT loc_4_38C6
        cmp    byte ptr [eax + 14h], 7
        jne    SHORT loc_4_38C6
        or     dword ptr [esi + 0b7h], 200h
loc_4_38C6:
        pop    esi
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h
sub_4_38D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    esp, [esp]
loc_4_38E0:
        cmp    word ptr [esi + 2ddh], 0
        je     NEAR PTR loc_4_3A42
        mov    bx, word ptr [esi + 426h]
        lea    edi, [esi + 2e3h]
        inc    dword ptr [esi + 42eh]
        mov    al, byte ptr [edi + 2]
        test   al, 80h
        je     SHORT loc_4_3970
        cmp    al, 0ffh
        jne    SHORT loc_4_3912
        cmp    byte ptr [edi + 3], 0ffh
        je     SHORT loc_4_391C
loc_4_3912:
        cmp    al, 0c0h
        jne    SHORT loc_4_3938
        cmp    byte ptr [edi + 3], 0
        jne    SHORT loc_4_3938
loc_4_391C:
        cmp    byte ptr [edi + 4], 0ffh
        jne    SHORT loc_4_3938
        cmp    byte ptr [edi + 5], 0ffh
        jne    SHORT loc_4_3938
        cmp    byte ptr [edi + 6], 0ffh
        jne    SHORT loc_4_3938
        cmp    byte ptr [edi + 7], 0ffh
        mov    byte ptr [ebp - 1], 1
        je     SHORT loc_4_393C
loc_4_3938:
        mov    byte ptr [ebp - 1], 0
loc_4_393C:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_4_3956
        lea    eax, [esi + 482h]
        mov    dword ptr [ebp - 0ch], eax
        lea    edx, [esi + 466h]
        mov    dword ptr [ebp - 8], edx
        jmp    SHORT loc_4_3993
loc_4_3956:
        lea    eax, [esi + 47eh]
        mov    dword ptr [ebp - 0ch], eax
        lea    edx, [esi + 45eh]
        mov    dword ptr [ebp - 8], edx
        jmp    SHORT loc_4_3993
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_3970:
        lea    eax, [esi + 47ah]
        mov    dword ptr [ebp - 0ch], eax
        lea    edx, [esi + 456h]
        mov    dword ptr [ebp - 8], edx
        test   dword ptr [esi + 59h], 8020h
        je     SHORT loc_4_3993
        push   ebx
        push   edi
        push   esi
        call   sub_4_3850
loc_4_3993:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, ebx
        inc    dword ptr [eax]
        mov    edx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        mov    eax, dword ptr [edx]
        add    eax, ecx
        mov    dword ptr [edx], eax
        cmp    ecx, eax
        jbe    SHORT loc_4_39B0
        inc    dword ptr [edx + 4]
loc_4_39B0:
        mov    ax, 0eh
        test   byte ptr [edi + 8], 80h
        je     SHORT loc_4_39C5
        mov    al, byte ptr [edi + 0eh]
        and    ax, 1fh
        add    ax, 0eh
loc_4_39C5:
        mov    word ptr [esi + 428h], ax
        sub    bx, ax
        mov    word ptr [esi + 426h], bx
        mov    edx, dword ptr [esi + 535h]
        cmp    edx, ecx
        jae    SHORT loc_4_39F0
        mov    ecx, eax
        and    ecx, 0ffffh
        sub    edx, ecx
        jmp    SHORT loc_4_39F8
        db  08Dh, 064h, 024h, 000h
loc_4_39F0:
        mov    edx, ebx
        and    edx, 0ffffh
loc_4_39F8:
        mov    ecx, eax
        and    ecx, 0ffffh
        mov    eax, ebx
        and    eax, 0ffffh
        push   eax
        push   edx
        lea    eax, [ecx + edi]
        push   eax
        push   ecx
        push   edi
        push   esi
        mov    eax, dword ptr [esi + 545h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3A1C:
        mov    eax, dword ptr [esi + 545h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3A28:
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 515h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3A3A:
        or     al, al
        je     NEAR PTR loc_4_38E0
loc_4_3A42:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_4_3A50:
        push   ebp
        mov    ebp, esp
        sub    esp, 38h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], 1
        lea    eax, [edi + 279h]
        mov    si, word ptr [edi + 273h]
        mov    word ptr [ebp - 0ch], si
        mov    dword ptr [ebp - 24h], eax
        lea    edx, [edi + 2adh]
        mov    dword ptr [ebp - 28h], edx
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebp - 20h], ecx
        add    dword ptr [ebp - 24h], 8
        mov    ax, word ptr [eax + 4]
        dec    word ptr [edi + 277h]
        mov    word ptr [ebp - 8], ax
        mov    dword ptr [ebp - 30h], ecx
        mov    word ptr [ebp - 18h], ax
        mov    cx, word ptr [edi + 0edh]
        mov    ax, cx
        and    eax, 7fh
        mov    bx, word ptr [edi + eax*2 + 0f1h]
        inc    cx
        mov    word ptr [edi + 0edh], cx
        mov    word ptr [ebp - 0eh], bx
        mov    ax, word ptr [edi + 10h]
        add    ax, 0ch
        mov    word ptr [ebp - 16h], ax
        mov    dx, word ptr [edi + 16h]
        sub    al, al
        out    dx, al
        mov    dx, word ptr [ebp - 16h]
        mov    ax, 10h
        out    dx, ax
        sub    dx, 2
        sub    ax, ax
        out    dx, ax
        mov    dx, word ptr [edi + 10h]
        in     al, dx
        mov    edx, dword ptr [ebp - 28h]
        and    ecx, 7fh
        mov    ax, word ptr [edx]
        mov    word ptr [ebp - 4], ax
        mov    ax, si
        sub    ax, word ptr [ebp - 4]
        mov    word ptr [ebp - 6], ax
        mov    cx, word ptr [edi + ecx*2 + 0f1h]
        or     cx, 8000h
        mov    ah, cl
        mov    al, ch
        mov    dx, word ptr [edi + 14h]
        out    dx, ax
        mov    ax, 80h
        out    dx, ax
        mov    ah, byte ptr [ebp - 4]
        mov    al, byte ptr [ebp - 3]
        out    dx, ax
        or     si, si
        je     NEAR PTR loc_4_3DED
loc_4_3B30:
        cmp    word ptr [ebp - 8], 0
        jne    SHORT loc_4_3B4B
        mov    eax, dword ptr [ebp - 24h]
        add    dword ptr [ebp - 24h], 8
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebp - 20h], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebp - 8], ax
loc_4_3B4B:
        mov    ax, word ptr [ebp - 4]
        mov    cx, word ptr [ebp - 8]
        cmp    ax, cx
        jb     SHORT loc_4_3B5B
        mov    ax, cx
loc_4_3B5B:
        mov    ecx, dword ptr [ebp - 20h]
        mov    dword ptr [ebp - 2ch], ecx
        mov    word ptr [ebp - 0ah], ax
        mov    cx, word ptr [edi + 14h]
        mov    word ptr [ebp - 10h], cx
        push   ecx
        push   edx
        push   esi
        mov    dx, word ptr [ebp - 10h]
        movzx  ecx, word ptr [ebp - 0ah]
        mov    esi, dword ptr [ebp - 2ch]
        shr    cx, 2
        rep outsd
        mov    cx, word ptr [ebp - 0ah]
        and    cx, 3
        rep outsb
        pop    esi
        pop    edx
        pop    ecx
        mov    ecx, eax
        and    ecx, 0ffffh
        add    dword ptr [ebp - 20h], ecx
        sub    word ptr [ebp - 8], ax
        sub    word ptr [ebp - 0ch], ax
        sub    word ptr [ebp - 4], ax
        jne    NEAR PTR loc_4_3D0A
        cmp    word ptr [ebp - 6], 2
        jbe    NEAR PTR loc_4_3D1A
        add    dword ptr [ebp - 28h], 4
        mov    edx, dword ptr [ebp - 28h]
        mov    ax, word ptr [ebp - 6]
        cmp    word ptr [edx + 2], ax
        jae    SHORT loc_4_3BE0
        mov    ax, word ptr [edx]
        mov    word ptr [ebp - 4], ax
        mov    ax, word ptr [ebp - 6]
        sub    ax, word ptr [ebp - 4]
        mov    word ptr [ebp - 6], ax
        mov    cx, word ptr [ebp - 4]
        jmp    SHORT loc_4_3C15
loc_4_3BE0:
        cmp    word ptr [ebp - 6], 3f8h
        jbe    SHORT loc_4_3BF8
        mov    cx, 396h
        sub    word ptr [ebp - 6], 396h
        mov    word ptr [ebp - 4], cx
        jmp    SHORT loc_4_3C15
loc_4_3BF8:
        mov    ax, word ptr [ebp - 6]
        dec    ax
        and    ax, 0fffeh
        mov    word ptr [ebp - 4], ax
        mov    cx, ax
        mov    ax, word ptr [ebp - 6]
        sub    ax, word ptr [ebp - 4]
        mov    word ptr [ebp - 6], ax
loc_4_3C15:
        mov    ah, cl
        mov    al, ch
        mov    dx, word ptr [edi + 14h]
        out    dx, ax
        mov    dx, word ptr [edi + 16h]
        sub    al, al
        out    dx, al
        mov    cx, word ptr [edi + 10h]
        or     bx, 400h
        mov    ax, bx
        shr    ax, 6
        lea    edx, [ecx + 0ch]
        out    dx, ax
        lea    esi, [ecx + 0ah]
        shl    bx, 0ah
        cmp    byte ptr [ebp - 1], 0
        mov    dx, si
        mov    ax, bx
        out    dx, ax
        mov    dx, cx
        in     al, dx
        je     SHORT loc_4_3CBD
        mov    byte ptr [ebp - 1], 0
        cmp    byte ptr [edi + 0e6h], 0
        jne    SHORT loc_4_3C6B
        cmp    word ptr [edi + 277h], 40h
        jae    SHORT loc_4_3C71
loc_4_3C6B:
        or     word ptr [ebp - 0eh], 8000h
loc_4_3C71:
        lea    edx, [ecx + 0ch]
        mov    ax, 1
        out    dx, ax
        mov    bx, word ptr [edi + 1f1h]
        mov    ax, bx
        and    ax, 7fh
        add    ax, ax
        add    ax, 148eh
        mov    dx, si
        out    dx, ax
        mov    dx, cx
        mov    ax, word ptr [ebp - 0eh]
        out    dx, ax
        inc    bx
        mov    word ptr [edi + 1f1h], bx
        mov    dx, word ptr [edi + 10h]
        add    dx, 4
        mov    ax, 1086h
        out    dx, ax
        sub    dx, 4
        mov    ax, bx
        out    dx, ax
loc_4_3CBD:
        mov    cx, word ptr [edi + 0edh]
        dec    word ptr [edi + 277h]
        mov    ax, cx
        and    eax, 7fh
        mov    bx, word ptr [edi + eax*2 + 0f1h]
        inc    cx
        mov    word ptr [edi + 0edh], cx
        and    ecx, 7fh
        mov    cx, word ptr [edi + ecx*2 + 0f1h]
        or     cx, 8000h
        mov    ah, cl
        mov    al, ch
        mov    dx, word ptr [edi + 14h]
        out    dx, ax
        mov    ax, 80h
        out    dx, ax
        mov    ah, byte ptr [ebp - 4]
        mov    al, byte ptr [ebp - 3]
        out    dx, ax
loc_4_3D0A:
        cmp    word ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_4_3B30
        jmp    NEAR PTR loc_4_3DED
loc_4_3D1A:
        mov    ah, byte ptr [ebp - 6]
        mov    al, byte ptr [ebp - 5]
        mov    dx, word ptr [edi + 14h]
        out    dx, ax
        mov    ax, word ptr [ebp - 6]
        cmp    ax, word ptr [ebp - 8]
        jbe    SHORT loc_4_3D7B
        mov    ecx, dword ptr [ebp - 20h]
        mov    dword ptr [ebp - 38h], ecx
        mov    ax, word ptr [ebp - 8]
        mov    word ptr [ebp - 14h], ax
        mov    ax, word ptr [edi + 14h]
        mov    word ptr [ebp - 1ch], ax
        push   ecx
        push   edx
        push   esi
        mov    dx, word ptr [ebp - 1ch]
        movzx  ecx, word ptr [ebp - 14h]
        mov    esi, dword ptr [ebp - 38h]
        shr    cx, 2
        rep outsd
        mov    cx, word ptr [ebp - 14h]
        and    cx, 3
        rep outsb
        pop    esi
        pop    edx
        pop    ecx
        mov    ax, word ptr [ebp - 6]
        sub    ax, word ptr [ebp - 8]
        mov    word ptr [ebp - 6], ax
        mov    eax, dword ptr [ebp - 24h]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebp - 20h], ecx
loc_4_3D7B:
        mov    ecx, dword ptr [ebp - 20h]
        mov    dword ptr [ebp - 34h], ecx
        mov    ax, word ptr [ebp - 6]
        mov    word ptr [ebp - 12h], ax
        mov    ax, word ptr [edi + 14h]
        mov    word ptr [ebp - 1ah], ax
        push   ecx
        push   edx
        push   esi
        mov    dx, word ptr [ebp - 1ah]
        movzx  ecx, word ptr [ebp - 12h]
        mov    esi, dword ptr [ebp - 34h]
        shr    cx, 2
        rep outsd
        mov    cx, word ptr [ebp - 12h]
        and    cx, 3
        rep outsb
        pop    esi
        pop    edx
        pop    ecx
        mov    dx, word ptr [edi + 16h]
        sub    al, al
        out    dx, al
        mov    cx, word ptr [edi + 10h]
        lea    edx, [ecx + 0ch]
        or     bx, 400h
        mov    ax, bx
        shr    ax, 6
        out    dx, ax
        sub    dx, 2
        mov    ax, bx
        shl    ax, 0ah
        out    dx, ax
        mov    dx, cx
        in     al, dx
        inc    word ptr [edi + 0edh]
        dec    word ptr [edi + 277h]
loc_4_3DED:
        mov    dx, word ptr [edi + 16h]
        mov    al, 1
        out    dx, al
        mov    dx, word ptr [edi + 10h]
        add    dx, 0ch
        cmp    byte ptr [edi + 0e6h], 0
        mov    ax, 1
        out    dx, ax
        jne    SHORT loc_4_3E25
        mov    eax, dword ptr [edi + 273h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        push   eax
        push   edi
        call   sub_4_3630
loc_4_3E25:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_3E30:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    bx, word ptr [edi + 273h]
        mov    cx, word ptr [edi + 0edh]
        mov    ax, cx
        and    eax, 7fh
        mov    ax, word ptr [edi + eax*2 + 0f1h]
        dec    word ptr [edi + 277h]
        inc    cx
        mov    word ptr [edi + 0edh], cx
        mov    cx, word ptr [edi + 10h]
        mov    word ptr [ebp - 6], ax
        lea    esi, [ecx + 0ch]
        mov    dx, word ptr [edi + 16h]
        sub    al, al
        out    dx, al
        mov    dx, si
        mov    ax, 10h
        out    dx, ax
        lea    edx, [esi - 2]
        sub    ax, ax
        out    dx, ax
        mov    dx, cx
        in     al, dx
        mov    dx, word ptr [edi + 14h]
        mov    ax, 40h
        out    dx, ax
        mov    ax, 81h
        out    dx, ax
        mov    cx, bx
        mov    ah, cl
        mov    al, ch
        out    dx, ax
        mov    ecx, dword ptr [edi + 279h]
        lea    eax, [edi + 279h]
        mov    dword ptr [ebp - 10h], ecx
        mov    cx, word ptr [eax + 4]
        mov    word ptr [ebp - 8], cx
        or     bx, bx
        je     SHORT loc_4_3F05
loc_4_3EC4:
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebp - 0ch], ecx
        mov    ecx, dword ptr [eax + 4]
        mov    word ptr [ebp - 2], cx
        mov    cx, word ptr [edi + 14h]
        mov    word ptr [ebp - 4], cx
        push   ecx
        push   edx
        push   esi
        mov    dx, word ptr [ebp - 4]
        movzx  ecx, word ptr [ebp - 2]
        mov    esi, dword ptr [ebp - 0ch]
        shr    cx, 2
        rep outsd
        mov    cx, word ptr [ebp - 2]
        and    cx, 3
        rep outsb
        pop    esi
        pop    edx
        pop    ecx
        sub    bx, word ptr [eax + 4]
        add    eax, 8
        or     bx, bx
        jne    SHORT loc_4_3EC4
loc_4_3F05:
        mov    dx, word ptr [edi + 16h]
        sub    al, al
        mov    bx, word ptr [edi + 10h]
        out    dx, al
        lea    esi, [ebx + 0ch]
        mov    cx, word ptr [ebp - 6]
        or     cx, 400h
        mov    ax, cx
        shr    ax, 6
        mov    dx, si
        out    dx, ax
        mov    ax, cx
        shl    ax, 0ah
        lea    edx, [esi - 2]
        out    dx, ax
        mov    dx, bx
        in     al, dx
        cmp    byte ptr [edi + 0e6h], 0
        mov    dx, word ptr [edi + 16h]
        mov    al, 1
        out    dx, al
        mov    dx, si
        mov    ax, 1
        out    dx, ax
        jne    SHORT loc_4_3F5B
        cmp    word ptr [edi + 277h], 40h
        jae    SHORT loc_4_3F61
loc_4_3F5B:
        or     word ptr [ebp - 6], 8000h
loc_4_3F61:
        mov    dx, si
        mov    ax, 1
        out    dx, ax
        mov    bx, word ptr [edi + 1f1h]
        mov    ax, bx
        and    ax, 7fh
        add    ax, ax
        add    ax, 148eh
        lea    edx, [esi - 2]
        out    dx, ax
        lea    edx, [esi - 0ch]
        mov    ax, word ptr [ebp - 6]
        out    dx, ax
        inc    bx
        mov    word ptr [edi + 1f1h], bx
        mov    dx, word ptr [edi + 10h]
        add    dx, 4
        mov    ax, 1086h
        out    dx, ax
        sub    dx, 4
        cmp    byte ptr [edi + 0e6h], 0
        mov    ax, bx
        out    dx, ax
        jne    SHORT loc_4_3FD0
        mov    eax, dword ptr [edi + 273h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        push   edi
        call   sub_4_3630
loc_4_3FD0:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_4_3FE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 3f5h], 0
        je     NEAR PTR loc_4_4080
        mov    dword ptr [ebx + 3ddh], 0
        mov    dword ptr [ebx + 3e1h], 0
        mov    ax, word ptr [ebx + 2ddh]
        mov    word ptr [ebx + 3d3h], ax
        mov    eax, dword ptr [ebx + 3e9h]
        mov    dword ptr [ebp - 0ch], eax
        xor    eax, eax
        mov    ax, word ptr [ebx + 2d5h]
        cmp    eax, dword ptr [ebp - 0ch]
        ja     NEAR PTR loc_4_40F9
loc_4_4036:
        mov    si, word ptr [ebx + 10h]
        mov    cx, word ptr [ebx + 3d3h]
        lea    edx, [esi + 0ch]
        mov    ax, cx
        shr    ax, 6
        out    dx, ax
        sub    dx, 2
        shl    cx, 0ah
        mov    ax, cx
        out    dx, ax
        mov    dx, si
        in     ax, dx
        mov    word ptr [ebx + 3d3h], ax
        xor    eax, eax
        mov    ax, word ptr [ebx + 2d5h]
        sub    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], eax
        jae    SHORT loc_4_4036
        jmp    NEAR PTR loc_4_40F9
        db  08Dh, 064h, 024h, 000h
loc_4_4080:
        lea    eax, [ebx + 2d5h]
        mov    cx, word ptr [ebx + 2d5h]
        sub    cx, word ptr [ebx + 3d9h]
        add    cx, 6
        mov    dx, word ptr [ebx + 16h]
        mov    al, 1
        mov    si, word ptr [ebx + 10h]
        out    dx, al
        lea    edx, [esi + 0ch]
        lea    eax, [ebx + 3d3h]
        mov    di, word ptr [ebx + 3d3h]
        or     di, 400h
        mov    ax, di
        shr    ax, 6
        out    dx, ax
        sub    dx, 2
        shl    di, 0ah
        add    di, cx
        mov    ax, di
        out    dx, ax
        mov    dx, si
        in     al, dx
        and    cx, 3
        mov    ax, cx
        dec    cx
        or     ax, ax
        je     SHORT loc_4_40F2
        mov    dx, word ptr [ebx + 14h]
loc_4_40E7:
        in     al, dx
        mov    ax, cx
        dec    cx
        or     ax, ax
        jne    SHORT loc_4_40E7
loc_4_40F2:
        mov    dword ptr [ebp - 0ch], 0
loc_4_40F9:
        lea    eax, [ebx + 3f6h]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebx + 3d5h]
        mov    ecx, eax
        cmp    ecx, 6
        jb     SHORT loc_4_4114
        mov    ecx, 6
loc_4_4114:
        mov    dword ptr [ebp - 1ch], ecx
        sub    eax, ecx
        mov    dword ptr [ebx + 3d5h], eax
        cmp    dword ptr [ebx + 3d9h], 0
        je     NEAR PTR loc_4_42CA
loc_4_412C:
        cmp    dword ptr [ebp - 1ch], 0
        jne    SHORT loc_4_413F
        cmp    dword ptr [ebx + 3e1h], 0
        je     NEAR PTR loc_4_42CA
loc_4_413F:
        cmp    dword ptr [ebx + 3ddh], 0
        jne    NEAR PTR loc_4_41FE
        mov    eax, dword ptr [ebp - 0ch]
        mov    dx, word ptr [ebx + 16h]
        lea    ecx, [eax + 6]
        mov    al, 1
        out    dx, al
        mov    ax, word ptr [ebx + 10h]
        add    ax, 0ch
        mov    word ptr [ebp - 10h], ax
        mov    si, word ptr [ebx + 3d3h]
        mov    di, si
        or     di, 400h
        mov    ax, di
        shr    ax, 6
        mov    dx, word ptr [ebp - 10h]
        out    dx, ax
        mov    ax, dx
        sub    ax, 2
        mov    word ptr [ebp - 18h], ax
        mov    ax, di
        shl    ax, 0ah
        add    ax, cx
        mov    dx, word ptr [ebp - 18h]
        out    dx, ax
        mov    dx, word ptr [ebx + 10h]
        in     al, dx
        and    cx, 3
        mov    ax, cx
        dec    cx
        or     ax, ax
        je     SHORT loc_4_41BE
        mov    dx, word ptr [ebx + 14h]
loc_4_41B3:
        in     al, dx
        mov    ax, cx
        dec    cx
        or     ax, ax
        jne    SHORT loc_4_41B3
loc_4_41BE:
        mov    ax, si
        shr    ax, 6
        mov    dx, word ptr [ebp - 10h]
        out    dx, ax
        mov    ax, si
        shl    ax, 0ah
        mov    dx, word ptr [ebp - 18h]
        out    dx, ax
        mov    dx, word ptr [ebx + 10h]
        in     ax, dx
        mov    word ptr [ebx + 3d3h], ax
        xor    eax, eax
        mov    ax, word ptr [ebx + 2d5h]
        sub    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebx + 3ddh], eax
        mov    dword ptr [ebp - 0ch], 0
loc_4_41FE:
        cmp    dword ptr [ebx + 3e1h], 0
        jne    SHORT loc_4_4225
        mov    eax, dword ptr [ebp - 14h]
        dec    dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebx + 3e5h], eax
        mov    eax, dword ptr [ebp - 14h]
        add    dword ptr [ebp - 14h], 8
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebx + 3e1h], eax
loc_4_4225:
        mov    eax, dword ptr [ebx + 3ddh]
        mov    ecx, dword ptr [ebx + 3e1h]
        cmp    eax, ecx
        jb     SHORT loc_4_4237
        mov    eax, ecx
loc_4_4237:
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebx + 3d9h]
        cmp    dword ptr [ebp - 8], eax
        jbe    SHORT loc_4_4248
        mov    dword ptr [ebp - 8], eax
loc_4_4248:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_4_42BD
        mov    eax, dword ptr [ebx + 3e5h]
        mov    dword ptr [ebp - 20h], eax
        mov    ax, word ptr [ebp - 8]
        mov    word ptr [ebp - 2], ax
        mov    ax, word ptr [ebx + 14h]
        mov    word ptr [ebp - 4], ax
        push   eax
        push   ecx
        push   edx
        push   edi
        mov    dx, word ptr [ebp - 4]
        movzx  ecx, word ptr [ebp - 2]
        mov    edi, dword ptr [ebp - 20h]
        cmp    cx, 20h
        jb     SHORT loc_4_429C
        mov    eax, ecx
        mov    cx, di
        neg    cl
        and    cx, 3
        sub    ax, cx
        rep insb
        mov    cx, ax
        shr    cx, 2
        rep insd
        mov    cx, ax
        and    cx, 3
loc_4_429C:
        rep insb
        pop    edi
        pop    edx
        pop    ecx
        pop    eax
        mov    eax, dword ptr [ebp - 8]
        add    dword ptr [ebx + 3e5h], eax
        sub    dword ptr [ebx + 3d9h], eax
        sub    dword ptr [ebx + 3ddh], eax
        sub    dword ptr [ebx + 3e1h], eax
loc_4_42BD:
        cmp    dword ptr [ebx + 3d9h], 0
        jne    NEAR PTR loc_4_412C
loc_4_42CA:
        mov    esi, dword ptr [ebx + 3d9h]
        or     esi, esi
        je     SHORT loc_4_42DD
        cmp    dword ptr [ebx + 3d5h], 0
        jne    SHORT loc_4_4302
loc_4_42DD:
        mov    dx, word ptr [ebx + 16h]
        mov    al, 1
        out    dx, al
        mov    dx, word ptr [ebx + 10h]
        add    dx, 0ch
        mov    ax, 1
        out    dx, ax
        mov    ecx, dword ptr [ebx + 3edh]
        mov    eax, dword ptr [ebx + 3f1h]
        sub    ecx, esi
        mov    dword ptr [eax], ecx
loc_4_4302:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_4_4310:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 0e2h]
        cmp    word ptr [esi + 0e4h], ax
        je     NEAR PTR loc_4_44EC
        sub    ebx, ebx
loc_4_4332:
        mov    eax, dword ptr [esi + 0b7h]
        and    al, 50h
        cmp    al, 10h
        jne    NEAR PTR loc_4_44EC
        cmp    word ptr [esi + 277h], 0fh
        jb     NEAR PTR loc_4_44E8
        mov    ecx, dword ptr [esi + 0deh]
        mov    dword ptr [ebp - 18h], ecx
        mov    eax, ecx
        add    eax, 24h
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 0deh], eax
        cmp    byte ptr [esi + 0e6h], bl
        je     SHORT loc_4_4380
        inc    word ptr [esi + 0e4h]
        jmp    SHORT loc_4_43A3
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_4380:
        mov    ecx, dword ptr [ebp - 18h]
        cmp    dword ptr [esi + 0dah], ecx
        jne    SHORT loc_4_4391
        mov    dword ptr [esi + 0dah], ebx
loc_4_4391:
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 0d6h], eax
        dec    word ptr [esi + 0e2h]
loc_4_43A3:
        mov    eax, dword ptr [ebp - 14h]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        mov    dword ptr [esi + 273h], eax
        mov    eax, dword ptr [ebp - 14h]
        lea    edi, [esi + 279h]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        cmp    eax, 6
        jae    SHORT loc_4_4410
        cmp    eax, ebx
        je     SHORT loc_4_4410
        cmp    dword ptr [ebp - 4], ebx
        je     NEAR PTR loc_4_4473
loc_4_43E2:
        lea    eax, [edi + 4]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_43F0:
        cmp    dword ptr [edi + 4], ebx
        je     SHORT loc_4_43F8
        add    edi, 8
loc_4_43F8:
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4405:
        cmp    dword ptr [ebp - 4], ebx
        jne    SHORT loc_4_43E2
        jmp    SHORT loc_4_4473
        db  08Dh, 064h, 024h, 000h
loc_4_4410:
        mov    eax, dword ptr [esi + 2a9h]
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 4], ebx
        je     SHORT loc_4_4462
        mov    edi, edi
loc_4_4420:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4431:
        cmp    dword ptr [ebp - 8], ebx
        je     SHORT loc_4_4450
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4447:
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0ch], eax
loc_4_4450:
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_445D:
        cmp    dword ptr [ebp - 4], ebx
        jne    SHORT loc_4_4420
loc_4_4462:
        mov    eax, dword ptr [esi + 2a9h]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 273h]
        mov    dword ptr [edi + 4], eax
loc_4_4473:
        cmp    dword ptr [esi + 273h], 161h
        push   esi
        jae    SHORT loc_4_4487
        db  068h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_448C
loc_4_4487:
        db  068h, 000h, 000h, 000h, 000h
loc_4_448C:
        lea    eax, [esi + 515h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4498:
        cmp    byte ptr [esi + 0e6h], bl
        jne    SHORT loc_4_44CB
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 4]
        test   byte ptr [eax + 19h], 80h
        jne    SHORT loc_4_44BA
        push   ebx
        mov    ecx, dword ptr [ebp - 18h]
        push   ecx
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_44BA:
        cmp    byte ptr [esi + 0e6h], bl
        jne    SHORT loc_4_44CB
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 4]
        dec    dword ptr [eax + 14h]
loc_4_44CB:
        mov    ax, word ptr [esi + 0e2h]
        cmp    word ptr [esi + 0e4h], ax
        jne    NEAR PTR loc_4_4332
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_44E8:
        mov    byte ptr [esi + 2ch], 1
loc_4_44EC:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_4500:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 14h], 0
        lea    eax, [ebp - 18h]
        mov    byte ptr [ebp - 1], 1
        mov    edi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 2], 0
        mov    esi, dword ptr [edi + 8]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        push   0
        mov    ebx, dword ptr [ebp + 0ch]
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4535:
        mov    eax, dword ptr [ebp - 18h]
        cmp    dword ptr [esi + 3ah], eax
        jae    SHORT loc_4_4550
        mov    eax, 0c001000fh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_4_4550:
        test   byte ptr [edi + 18h], 2
        je     SHORT loc_4_45A0
        mov    eax, dword ptr [esi + 0b7h]
        and    al, 50h
        cmp    al, 10h
        jne    SHORT loc_4_45A0
        cmp    byte ptr [esi + 2bh], 0
        jne    SHORT loc_4_45B8
        mov    ax, word ptr [esi + 0e4h]
        cmp    word ptr [esi + 0e2h], ax
        jne    SHORT loc_4_45B8
        cmp    word ptr [esi + 277h], 0fh
        jae    SHORT loc_4_458E
        mov    byte ptr [ebp - 2], 1
        mov    byte ptr [ebp - 1], 0
        mov    byte ptr [esi + 2ch], 1
loc_4_458E:
        cmp    byte ptr [esi + 0e6h], 0
        je     SHORT loc_4_45C0
        mov    byte ptr [ebp - 2], 1
        jmp    SHORT loc_4_45C0
        db  08Dh, 049h, 000h
loc_4_45A0:
        push   1
        push   0
        push   edi
        call   sub_4_32F0
        mov    dword ptr [ebp - 14h], eax
        cmp    eax, 103h
        jne    NEAR PTR loc_4_4767
loc_4_45B8:
        mov    byte ptr [ebp - 2], 1
        mov    byte ptr [ebp - 1], 0
loc_4_45C0:
        cmp    byte ptr [ebp - 2], 0
        je     NEAR PTR loc_4_4665
        mov    eax, dword ptr [ebp - 18h]
        mov    word ptr [ebx + 30h], ax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebx + 2ch], eax
        cmp    dword ptr [ebp - 1ch], 0ffffh
        jbe    SHORT loc_4_45E7
        mov    dword ptr [ebp - 1ch], 0
loc_4_45E7:
        mov    eax, dword ptr [ebp - 1ch]
        mov    word ptr [ebx + 32h], ax
        mov    dword ptr [ebx + 24h], 0
        mov    dword ptr [ebx + 28h], edi
        cmp    dword ptr [esi + 0d6h], 0
        jne    SHORT loc_4_4620
        mov    dword ptr [esi + 0deh], ebx
        lea    ecx, [esi + 0deh]
        mov    dword ptr [esi + 0dah], ebx
        mov    dword ptr [esi + 0d6h], ebx
        jmp    SHORT loc_4_463C
        db  005h, 000h, 000h, 000h, 000h
loc_4_4620:
        mov    eax, dword ptr [esi + 0dah]
        lea    ecx, [esi + 0deh]
        mov    dword ptr [eax + 24h], ebx
        mov    dword ptr [esi + 0dah], ebx
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_4_463C
        mov    dword ptr [ecx], ebx
loc_4_463C:
        inc    word ptr [esi + 0e2h]
        inc    dword ptr [edi + 14h]
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_4_4658
        inc    word ptr [esi + 0e4h]
        mov    eax, dword ptr [ebx + 24h]
        mov    dword ptr [ecx], eax
loc_4_4658:
        cmp    dword ptr [ebp - 14h], 0
        jne    SHORT loc_4_4665
        mov    dword ptr [ebp - 14h], 103h
loc_4_4665:
        cmp    byte ptr [ebp - 1], 0
        je     NEAR PTR loc_4_475B
        mov    byte ptr [esi + 2bh], 1
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [esi + 273h], eax
        lea    edi, [esi + 279h]
        cmp    dword ptr [ebp - 1ch], 6
        jae    SHORT loc_4_46C4
        cmp    dword ptr [ebp - 1ch], 0
        jbe    SHORT loc_4_46C4
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_4_4719
        sub    ebx, ebx
loc_4_469A:
        lea    eax, [edi + 4]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_46A8:
        cmp    dword ptr [edi + 4], ebx
        je     SHORT loc_4_46B0
        add    edi, 8
loc_4_46B0:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_46BD:
        cmp    dword ptr [ebp - 8], ebx
        jne    SHORT loc_4_469A
        jmp    SHORT loc_4_4719
loc_4_46C4:
        mov    ebx, dword ptr [esi + 2a9h]
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_4_470B
loc_4_46D0:
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_46E1:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_4_46F8
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_46F5:
        add    ebx, dword ptr [ebp - 0ch]
loc_4_46F8:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4705:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_4_46D0
loc_4_470B:
        mov    eax, dword ptr [esi + 2a9h]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [edi + 4], eax
loc_4_4719:
        cmp    dword ptr [ebp - 18h], 161h
        push   esi
        jae    SHORT loc_4_4730
        db  068h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_4735
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_4730:
        db  068h, 000h, 000h, 000h, 000h
loc_4_4735:
        lea    eax, [esi + 515h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4741:
        mov    ax, word ptr [esi + 0e4h]
        cmp    word ptr [esi + 0e2h], ax
        je     SHORT loc_4_4757
        push   esi
        call   sub_4_4310
loc_4_4757:
        mov    byte ptr [esi + 2bh], 0
loc_4_475B:
        mov    eax, dword ptr [ebp - 14h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_4767:
        mov    eax, dword ptr [ebp - 14h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_4780:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 10h]
        mov    esi, dword ptr [eax + 8]
        xor    ebx, ebx
        mov    bx, word ptr [esi + 426h]
        cmp    edi, ebx
        jb     SHORT loc_4_47B3
        mov    eax, dword ptr [ebp + 1ch]
        pop    edi
        mov    dword ptr [eax], 0
        sub    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_4_47B3:
        sub    ebx, edi
        cmp    dword ptr [ebp + 14h], ebx
        jae    SHORT loc_4_47C0
        mov    ebx, dword ptr [ebp + 14h]
        jmp    SHORT loc_4_47C3
        db  090h
loc_4_47C0:
        mov    dword ptr [ebp + 14h], ebx
loc_4_47C3:
        xor    eax, eax
        mov    ax, word ptr [esi + 428h]
        push   0
        add    edi, eax
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [esi + 3d5h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_47E6:
        lea    eax, [ebx + edi]
        cmp    eax, dword ptr [esi + 535h]
        ja     NEAR PTR loc_4_48C0
        mov    dword ptr [ebp - 0ch], 0
        lea    eax, [edi + esi + 2e3h]
        mov    dword ptr [ebp - 14h], eax
        or     ebx, ebx
        je     NEAR PTR loc_4_48AA
        mov    edi, edi
loc_4_4810:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_4_483E
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_4_48AA
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4831:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_483E:
        mov    edx, dword ptr [ebp - 0ch]
        cmp    edx, ebx
        jb     SHORT loc_4_4847
        mov    edx, ebx
loc_4_4847:
        or     edx, edx
        je     SHORT loc_4_48A2
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 18h], eax
        mov    word ptr [ebp - 2], dx
        push   eax
        push   ecx
        push   esi
        push   edi
        movzx  ecx, word ptr [ebp - 2]
        mov    esi, dword ptr [ebp - 1ch]
        mov    edi, dword ptr [ebp - 18h]
        cmp    cx, 20h
        jb     SHORT loc_4_488F
        mov    eax, ecx
        mov    cx, di
        neg    cl
        and    cx, 3
        sub    ax, cx
        rep movsb
        mov    cx, ax
        shr    cx, 2
        rep movsd
        mov    cx, ax
        and    cx, 3
loc_4_488F:
        rep movsb
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        sub    ebx, edx
        je     SHORT loc_4_48A2
        add    dword ptr [ebp - 14h], edx
        add    dword ptr [ebp - 10h], edx
        sub    dword ptr [ebp - 0ch], edx
loc_4_48A2:
        or     ebx, ebx
        jne    NEAR PTR loc_4_4810
loc_4_48AA:
        mov    ecx, dword ptr [ebp + 14h]
        mov    eax, dword ptr [ebp + 1ch]
        sub    ecx, ebx
        mov    dword ptr [eax], ecx
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  090h
loc_4_48C0:
        mov    dword ptr [esi + 3d9h], ebx
        mov    dword ptr [esi + 3e9h], edi
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [esi + 3edh], eax
        or     ebx, ebx
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [esi + 3f1h], eax
        mov    byte ptr [esi + 3f5h], 1
        je     SHORT loc_4_494A
loc_4_48E9:
        mov    edi, dword ptr [esi + 3d5h]
        or     edi, edi
        je     SHORT loc_4_494A
        cmp    edi, 6
        jb     SHORT loc_4_48FD
        mov    edi, 6
loc_4_48FD:
        lea    ebx, [esi + 3f6h]
        or     edi, edi
        je     SHORT loc_4_4928
loc_4_4907:
        lea    eax, [ebx + 4]
        push   eax
        push   ebx
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4915:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4922:
        add    ebx, 8
        dec    edi
        jne    SHORT loc_4_4907
loc_4_4928:
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 515h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_493A:
        mov    byte ptr [esi + 3f5h], 0
        cmp    dword ptr [esi + 3d9h], 0
        jne    SHORT loc_4_48E9
loc_4_494A:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_4960:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        sub    bl, bl
        mov    si, word ptr [ecx + 10h]
        add    si, 0ch
        mov    dx, si
        mov    ax, 1
        out    dx, ax
        lea    edx, [esi - 2]
        mov    ax, 1080h
        out    dx, ax
        sub    si, 0ah
        mov    dx, si
        in     ax, dx
        mov    word ptr [ecx + 2d9h], ax
        in     ax, dx
        mov    word ptr [ecx + 0efh], ax
        sub    ax, word ptr [ecx + 0edh]
        add    ax, 80h
        cmp    byte ptr [ecx + 0e6h], bl
        mov    word ptr [ecx + 277h], ax
        je     SHORT loc_4_4A36
        in     ax, dx
        mov    word ptr [ecx + 0e7h], ax
        mov    si, ax
        sub    si, word ptr [ecx + 0e9h]
        je     NEAR PTR loc_4_4A5A
        mov    bl, 1
        jbe    SHORT loc_4_4A26
loc_4_49D8:
        mov    di, word ptr [ecx + 0e9h]
        and    di, 7fh
        mov    dx, word ptr [ecx + 10h]
        add    dx, 4
        lea    eax, [edi + 158eh]
        and    ax, 0fffeh
        out    dx, ax
        sub    dx, 4
        test   di, 1
        in     ax, dx
        mov    edx, edi
        je     SHORT loc_4_4A15
        and    edx, 0ffffh
        mov    byte ptr [ecx + edx + 1f3h], al
        jmp    SHORT loc_4_4A22
loc_4_4A15:
        and    edx, 0ffffh
        mov    byte ptr [ecx + edx + 1f3h], ah
loc_4_4A22:
        dec    si
        jne    SHORT loc_4_49D8
loc_4_4A26:
        mov    ax, word ptr [ecx + 0e7h]
        mov    word ptr [ecx + 0e9h], ax
        jmp    SHORT loc_4_4A5A
loc_4_4A36:
        cmp    byte ptr [ecx + 2ch], 0
        je     SHORT loc_4_4A5A
        mov    ax, word ptr [ecx + 0efh]
        sub    ax, word ptr [ecx + 0edh]
        add    ax, 80h
        cmp    ax, 0fh
        jbe    SHORT loc_4_4A5A
        mov    bl, 1
        mov    byte ptr [ecx + 2ch], 0
loc_4_4A5A:
        mov    ax, word ptr [ecx + 2dbh]
        cmp    word ptr [ecx + 2d9h], ax
        je     NEAR PTR loc_4_4B5A
        mov    bl, 1
        mov    dx, word ptr [ecx + 10h]
        add    dx, 4
        and    ax, 1ffh
        add    ax, ax
        add    ax, 108eh
        out    dx, ax
        sub    dx, 4
        in     ax, dx
        mov    word ptr [ecx + 2ddh], ax
        mov    dx, word ptr [ecx + 16h]
        mov    al, bl
        mov    si, word ptr [ecx + 10h]
        out    dx, al
        lea    edx, [esi + 0ch]
        mov    di, word ptr [ecx + 2ddh]
        or     di, 400h
        mov    ax, di
        shr    ax, 6
        out    dx, ax
        sub    dx, 2
        mov    ax, di
        shl    ax, 0ah
        out    dx, ax
        mov    dx, si
        in     al, dx
        mov    dx, word ptr [ecx + 14h]
        in     ax, dx
        mov    dh, al
        mov    dl, ah
        mov    word ptr [ecx + 2dfh], dx
        mov    dx, word ptr [ecx + 14h]
        in     ax, dx
        mov    word ptr [ecx + 2e1h], ax
        in     ax, dx
        mov    dh, al
        mov    dl, ah
        sub    dx, 4
        mov    word ptr [ecx + 426h], dx
        lea    eax, [ecx + 2e3h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ecx + 535h]
        mov    dx, word ptr [ecx + 426h]
        cmp    ax, dx
        jb     SHORT loc_4_4B13
        mov    ax, dx
loc_4_4B13:
        mov    word ptr [ebp - 2], ax
        mov    ax, word ptr [ecx + 14h]
        mov    word ptr [ebp - 4], ax
        push   eax
        push   ecx
        push   edx
        push   edi
        mov    dx, word ptr [ebp - 4]
        movzx  ecx, word ptr [ebp - 2]
        mov    edi, dword ptr [ebp - 8]
        cmp    cx, 20h
        jb     SHORT loc_4_4B54
        mov    eax, ecx
        mov    cx, di
        neg    cl
        and    cx, 3
        sub    ax, cx
        rep insb
        mov    cx, ax
        shr    cx, 2
        rep insd
        mov    cx, ax
        and    cx, 3
loc_4_4B54:
        rep insb
        pop    edi
        pop    edx
        pop    ecx
        pop    eax
loc_4_4B5A:
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_4B70:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0e6h], 0
        je     SHORT loc_4_4B86
        push   esi
        call   sub_4_3740
loc_4_4B86:
        mov    ax, word ptr [esi + 0e4h]
        cmp    word ptr [esi + 0e2h], ax
        je     SHORT loc_4_4BAA
        cmp    byte ptr [esi + 2bh], 0
        jne    SHORT loc_4_4BAA
        mov    byte ptr [esi + 2bh], 1
        push   esi
        call   sub_4_4310
        mov    byte ptr [esi + 2bh], 0
loc_4_4BAA:
        mov    ax, word ptr [esi + 2dbh]
        cmp    word ptr [esi + 2d9h], ax
        push   esi
        je     SHORT loc_4_4BC5
        call   sub_4_38D0
        pop    esi
        pop    ebp
        ret    4
loc_4_4BC5:
        db  068h, 000h, 000h, 000h, 000h
        add    esi, 515h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4BD6:
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_4_4BE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        lea    esp, [esp]
loc_4_4BF0:
        test   byte ptr [ecx + 2e1h], 1
        jne    SHORT loc_4_4C38
        mov    di, word ptr [ecx + 10h]
        mov    si, word ptr [ecx + 2dfh]
        lea    edx, [edi + 0ch]
        mov    ax, si
        shr    ax, 6
        out    dx, ax
        sub    dx, 2
        shl    si, 0ah
        mov    ax, si
        out    dx, ax
        lea    edx, [edi + 2]
        in     ax, dx
        mov    word ptr [ecx + 2dfh], ax
        in     ax, dx
        mov    dh, al
        mov    dl, ah
        mov    word ptr [ecx + 2e1h], dx
        jmp    SHORT loc_4_4BF0
loc_4_4C38:
        mov    ax, word ptr [ecx + 2dfh]
        mov    word ptr [ecx + 2ddh], ax
        or     ax, ax
        jne    NEAR PTR loc_4_4D07
        mov    ax, word ptr [ecx + 2dbh]
        inc    ax
        cmp    word ptr [ecx + 2d9h], ax
        mov    word ptr [ecx + 2dbh], ax
        jne    SHORT loc_4_4C8E
        mov    si, word ptr [ecx + 10h]
        add    si, 0ch
        mov    dx, si
        mov    ax, 1
        out    dx, ax
        lea    edx, [esi - 2]
        mov    ax, 1080h
        out    dx, ax
        lea    edx, [esi - 0ah]
        in     ax, dx
        mov    word ptr [ecx + 2d9h], ax
loc_4_4C8E:
        mov    di, word ptr [ecx + 2dbh]
        cmp    word ptr [ecx + 2d9h], di
        jne    SHORT loc_4_4CD5
        mov    bx, word ptr [ecx + 10h]
        mov    ax, 1
        lea    esi, [ebx + 0ch]
        mov    dx, si
        out    dx, ax
        lea    edx, [esi - 2]
        mov    ax, 1088h
        out    dx, ax
        lea    edx, [esi - 0ch]
        mov    ax, di
        out    dx, ax
        lea    edx, [ebx + 7]
        mov    al, byte ptr [ecx + 34h]
        out    dx, al
        mov    byte ptr [ecx + 34h], 0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_4CD5:
        mov    si, word ptr [ecx + 10h]
        add    si, 0ch
        mov    dx, si
        mov    ax, 1
        out    dx, ax
        lea    edx, [esi - 2]
        and    di, 1ffh
        add    di, di
        add    di, 108eh
        mov    ax, di
        out    dx, ax
        lea    edx, [esi - 0ch]
        in     ax, dx
        mov    word ptr [ecx + 2ddh], ax
loc_4_4D07:
        mov    dx, word ptr [ecx + 16h]
        mov    al, 1
        mov    si, word ptr [ecx + 10h]
        out    dx, al
        lea    edx, [esi + 0ch]
        mov    di, word ptr [ecx + 2ddh]
        or     di, 400h
        mov    ax, di
        shr    ax, 6
        out    dx, ax
        sub    dx, 2
        mov    ax, di
        shl    ax, 0ah
        out    dx, ax
        mov    dx, si
        in     al, dx
        mov    dx, word ptr [ecx + 14h]
        in     ax, dx
        mov    dh, al
        mov    dl, ah
        mov    word ptr [ecx + 2dfh], dx
        mov    dx, word ptr [ecx + 14h]
        in     ax, dx
        mov    word ptr [ecx + 2e1h], ax
        in     ax, dx
        mov    dh, al
        mov    dl, ah
        sub    dx, 4
        mov    word ptr [ecx + 426h], dx
        lea    eax, [ecx + 2e3h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ecx + 535h]
        mov    dx, word ptr [ecx + 426h]
        cmp    ax, dx
        jb     SHORT loc_4_4D88
        mov    ax, dx
loc_4_4D88:
        mov    word ptr [ebp - 2], ax
        mov    ax, word ptr [ecx + 14h]
        mov    word ptr [ebp - 4], ax
        push   eax
        push   ecx
        push   edx
        push   edi
        mov    dx, word ptr [ebp - 4]
        movzx  ecx, word ptr [ebp - 2]
        mov    edi, dword ptr [ebp - 8]
        cmp    cx, 20h
        jb     SHORT loc_4_4DC9
        mov    eax, ecx
        mov    cx, di
        neg    cl
        and    cx, 3
        sub    ax, cx
        rep insb
        mov    cx, ax
        shr    cx, 2
        rep insd
        mov    cx, ax
        and    cx, 3
loc_4_4DC9:
        rep insb
        pop    edi
        pop    edx
        pop    ecx
        pop    eax
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_4DE0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi + 2dbh], 0
        mov    word ptr [esi + 2d9h], 0
        mov    word ptr [esi + 2e1h], 0
        mov    word ptr [esi + 2dfh], 0
        mov    word ptr [esi + 2ddh], 0
        mov    word ptr [esi + 0efh], 0
        mov    word ptr [esi + 0edh], 0
        mov    word ptr [esi + 1f1h], 0
        mov    word ptr [esi + 0ebh], 0
        mov    word ptr [esi + 0e9h], 0
        mov    word ptr [esi + 0e7h], 0
        mov    dword ptr [esi + 0deh], 0
        mov    dword ptr [esi + 0dah], 0
        mov    dword ptr [esi + 0d6h], 0
        mov    word ptr [esi + 0e4h], 0
        mov    word ptr [esi + 0e2h], 0
        mov    word ptr [esi + 277h], 80h
        mov    word ptr [esi + 2adh], 100h
        mov    word ptr [esi + 2afh], 161h
        mov    word ptr [esi + 2b1h], 100h
        mov    word ptr [esi + 2b3h], 161h
        mov    word ptr [esi + 2b5h], 200h
        mov    word ptr [esi + 2b7h], 261h
        mov    word ptr [esi + 2b9h], 3f6h
        mov    word ptr [esi + 2bbh], 457h
        mov    word ptr [esi + 2bdh], 3f6h
        mov    word ptr [esi + 2bfh], 457h
        mov    word ptr [esi + 2c1h], 3f6h
        mov    word ptr [esi + 2c3h], 457h
        mov    word ptr [esi + 2c5h], 3f6h
        mov    word ptr [esi + 2c7h], 457h
        cmp    byte ptr [ebp + 0ch], 0
        mov    word ptr [esi + 2c9h], 3f6h
        mov    word ptr [esi + 2cbh], 457h
        mov    word ptr [esi + 2cdh], 3f6h
        mov    word ptr [esi + 2cfh], 457h
        mov    word ptr [esi + 2d1h], 3f6h
        mov    word ptr [esi + 2d3h], 457h
        je     SHORT loc_4_4F7C
        sub    di, di
loc_4_4F41:
        mov    cx, word ptr [esi + 10h]
        add    cx, 0ch
        mov    dx, cx
        mov    ax, 1
        out    dx, ax
        lea    edx, [ecx - 2]
        lea    eax, [edi*2 + 148eh]
        out    dx, ax
        lea    edx, [ecx - 0ch]
        in     ax, dx
        mov    ecx, edi
        and    ecx, 0ffffh
        mov    word ptr [esi + ecx*2 + 0f1h], ax
        inc    di
        cmp    di, 80h
        jb     SHORT loc_4_4F41
loc_4_4F7C:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 032h, 045h, 06Eh, 068h
_PTEXT ends
        end
