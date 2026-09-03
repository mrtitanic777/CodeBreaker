; am1500t_lcode_01.asm -- AM1500T object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_0000:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0050
        push   esi
        call   sub_1_03C0
        cmp    al, 1
        jne    SHORT loc_1_0024
        push   esi
        call   sub_1_0030
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
loc_1_0024:
        sub    al, al
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_0030:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    cx, word ptr [eax + 45h]
        sub    ax, ax
        lea    edx, [ecx + 12h]
        out    dx, ax
        lea    edx, [ecx + 10h]
        mov    ax, 42h
        out    dx, ax
        pop    ebp
        ret    4
        db  090h
sub_1_0050:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    cx, word ptr [eax + 45h]
        sub    ax, ax
        lea    edx, [ecx + 12h]
        out    dx, ax
        lea    edx, [ecx + 10h]
        mov    ax, 4
        out    dx, ax
        pop    ebp
        ret    4
        db  090h
loc_1_0070:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [esi + 49h]
        cmp    byte ptr [edi + 40h], 0
        jne    SHORT loc_1_00B0
        lea    eax, [edi + 18h]
        mov    dword ptr [edi + 28h], 4
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [eax], 0
        inc    dword ptr [edi + 14h]
        push   eax
        push   esi
        call   sub_1_16B0
        dec    dword ptr [edi + 14h]
        mov    edi, 103h
        jmp    SHORT loc_1_00B5
        db  005h, 000h, 000h, 000h, 000h
loc_1_00B0:
        mov    edi, 0c0010002h
loc_1_00B5:
        cmp    byte ptr [esi + 11h], 0
        jne    SHORT loc_1_0100
        cmp    dword ptr [esi + 49h], 2
        jne    SHORT loc_1_00CA
        cmp    byte ptr [esi + 0e8h], 0
        jne    SHORT loc_1_00E6
loc_1_00CA:
        cmp    dword ptr [esi + 0dch], 0
        jne    SHORT loc_1_00E6
        cmp    dword ptr [esi + 0d0h], 0
        jne    SHORT loc_1_00E6
        mov    eax, dword ptr [esi + 59h]
        sub    eax, esi
        cmp    eax, 59h
        je     SHORT loc_1_0100
loc_1_00E6:
        mov    byte ptr [esi + 11h], 1
        push   0
        add    esi, 12h
        push   esi
        call   sub_1_0B92
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_0100:
        dec    dword ptr [esi + 49h]
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_0110:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        db  0A1h, 030h, 029h, 000h, 000h
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0ADC
        push   0
        db  0A1h, 02Ch, 029h, 000h, 000h
        push   eax
        call   sub_1_0AD0
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0140:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    byte ptr [esi + 2], 1
        call   sub_1_0050
        mov    eax, dword ptr [esi + 71h]
        push   eax
        call   sub_1_0C94
        cmp    byte ptr [esi], 1
        jne    SHORT loc_1_0179
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 1eh]
        push   eax
        call   sub_1_0B98
        push   2625a0h
        call   sub_1_0BB0
loc_1_0179:
        lea    eax, [esi + 2bh]
        push   eax
        call   sub_1_0BA4
        mov    eax, dword ptr [esi + 75h]
        push   eax
        db  0E8h, 0F5h, 00Bh, 0FFh, 0FFh
        push   esi
        call   sub_1_0650
        mov    eax, dword ptr [esi + 71h]
        push   eax
        call   sub_1_0AE8
        push   0
        push   248h
        push   esi
        call   sub_1_0BBC
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_01B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 0
        mov    ebx, dword ptr [edi + 8]
        cmp    byte ptr [ebx + 2], 0
        je     SHORT loc_1_01E0
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_01E0:
        inc    dword ptr [ebx + 49h]
        cmp    byte ptr [ebx + 0e8h], 0
        jne    NEAR PTR loc_1_02E0
        cmp    byte ptr [ebx + 0e9h], 0
        jne    NEAR PTR loc_1_02E0
        cmp    byte ptr [edi + 40h], 0
        jne    NEAR PTR loc_1_02D0
        cmp    byte ptr [ebx + 0eah], 0
        jne    NEAR PTR loc_1_02E7
        mov    byte ptr [ebx + 0e9h], 1
        inc    dword ptr [edi + 14h]
        mov    esi, dword ptr [ebx + 4dh]
        mov    eax, ebx
        sub    eax, esi
        cmp    eax, -4dh
        je     SHORT loc_1_0256
loc_1_022A:
        inc    dword ptr [esi + 14h]
        push   0
        push   0
        push   40010004h
        mov    eax, dword ptr [esi + 10h]
        push   eax
        call   sub_1_0B1E
        mov    eax, dword ptr [esi + 10h]
        push   eax
        call   sub_1_0B24
        dec    dword ptr [esi + 14h]
        mov    esi, dword ptr [esi]
        mov    eax, ebx
        sub    eax, esi
        cmp    eax, -4dh
        jne    SHORT loc_1_022A
loc_1_0256:
        mov    byte ptr [ebx + 0e9h], 0
        push   edi
        push   ebx
        call   sub_1_05B0
        push   ebx
        call   sub_1_0300
        push   ebx
        call   sub_1_0030
        mov    byte ptr [ebx + 0e8h], 0
        mov    byte ptr [ebx + 0eah], 1
        cmp    dword ptr [ebx + 79h], 0
        je     SHORT loc_1_028A
        push   ebx
        call   sub_1_16F0
loc_1_028A:
        mov    esi, dword ptr [ebx + 4dh]
        mov    eax, ebx
        sub    eax, esi
        cmp    eax, -4dh
        je     SHORT loc_1_02C2
loc_1_0296:
        inc    dword ptr [esi + 14h]
        push   0
        push   0
        push   40010005h
        mov    eax, dword ptr [esi + 10h]
        push   eax
        call   sub_1_0B1E
        mov    eax, dword ptr [esi + 10h]
        push   eax
        call   sub_1_0B24
        dec    dword ptr [esi + 14h]
        mov    esi, dword ptr [esi]
        mov    eax, ebx
        sub    eax, esi
        cmp    eax, -4dh
        jne    SHORT loc_1_0296
loc_1_02C2:
        mov    byte ptr [ebx + 0eah], 0
        dec    dword ptr [edi + 14h]
        jmp    SHORT loc_1_02E7
        db  08Bh, 0FFh
loc_1_02D0:
        mov    dword ptr [ebp - 4], 0c0010002h
        jmp    SHORT loc_1_02E7
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_02E0:
        mov    dword ptr [ebp - 4], 0c001000dh
loc_1_02E7:
        dec    dword ptr [ebx + 49h]
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0300:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0050
        cmp    dword ptr [esi + 0d0h], 0
        mov    word ptr [esi + 43h], 0
        je     SHORT loc_1_0350
loc_1_0320:
        mov    edx, dword ptr [esi + 0d0h]
        push   0c001000ch
        mov    ecx, dword ptr [edx + 24h]
        lea    eax, [edx + 24h]
        mov    dword ptr [esi + 0d0h], ecx
        mov    edi, dword ptr [eax + 4]
        push   edx
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_0AFA
        dec    dword ptr [edi + 14h]
        cmp    dword ptr [esi + 0d0h], 0
        jne    SHORT loc_1_0320
loc_1_0350:
        mov    dword ptr [esi + 0d0h], 0
        mov    dword ptr [esi + 0d4h], 0
        cmp    dword ptr [esi + 0dch], 0
        je     SHORT loc_1_03A0
        lea    ecx, [ecx]
loc_1_0370:
        mov    edx, dword ptr [esi + 0dch]
        push   0c001000ch
        mov    ecx, dword ptr [edx + 24h]
        lea    eax, [edx + 24h]
        mov    dword ptr [esi + 0dch], ecx
        mov    edi, dword ptr [eax + 4]
        push   edx
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_0AFA
        dec    dword ptr [edi + 14h]
        cmp    dword ptr [esi + 0dch], 0
        jne    SHORT loc_1_0370
loc_1_03A0:
        mov    dword ptr [esi + 0dch], 0
        mov    dword ptr [esi + 0e0h], 0
        push   esi
        call   sub_1_03C0
        pop    edi
        pop    esi
        pop    ebp
        ret    4
sub_1_03C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    dl, dl
        mov    edi, dword ptr [esi + 99h]
        mov    eax, dword ptr [esi + 95h]
        cmp    dword ptr [esi + 8dh], 0
        jbe    SHORT loc_1_043F
loc_1_03E3:
        mov    cl, dl
        and    ecx, 0ffff00ffh
        mov    bx, cx
        imul   bx, bx, 5eah
        add    bx, ax
        mov    word ptr [edi], bx
        and    ecx, 0ffffh
        mov    ebx, ecx
        lea    ecx, [ecx + ecx*4]
        lea    ecx, [ebx + ecx*4]
        mov    word ptr [edi + 4], 0f000h
        lea    ecx, [ecx + ecx*8]
        lea    ecx, [ebx + ecx*4]
        lea    ecx, [eax + ecx*2]
        shr    ecx, 10h
        and    cx, 0ffh
        mov    word ptr [edi + 2], cx
        or     word ptr [edi + 2], 300h
        mov    word ptr [edi + 6], 0
        inc    dl
        add    edi, 8
        xor    ecx, ecx
        mov    cl, dl
        cmp    ecx, dword ptr [esi + 8dh]
        jb     SHORT loc_1_03E3
loc_1_043F:
        sub    cl, cl
        cmp    dword ptr [esi + 8dh], 0
        jbe    SHORT loc_1_0469
        mov    edx, 1
        nop
loc_1_0450:
        xor    eax, eax
        mov    al, cl
        mov    byte ptr [esi + eax + 0a2h], dl
        inc    cl
        xor    eax, eax
        mov    al, cl
        cmp    eax, dword ptr [esi + 8dh]
        jb     SHORT loc_1_0450
loc_1_0469:
        mov    byte ptr [esi + 0a1h], 0
        mov    eax, dword ptr [esi + 0beh]
        mov    edi, dword ptr [esi + 0bah]
        sub    dl, dl
        cmp    dword ptr [esi + 0b2h], 0
        jbe    SHORT loc_1_04E5
loc_1_0487:
        mov    cl, dl
        and    ecx, 0ffff00ffh
        mov    bx, cx
        imul   bx, bx, 5eeh
        add    bx, di
        mov    word ptr [eax], bx
        and    ecx, 0ffffh
        mov    ebx, ecx
        shl    ecx, 6
        sub    ecx, ebx
        lea    ecx, [ebx + ecx*4]
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [edi + ecx*2]
        shr    ecx, 10h
        and    cx, 0ffh
        mov    word ptr [eax + 2], cx
        or     cx, 8000h
        mov    word ptr [eax + 2], cx
        mov    word ptr [eax + 4], 0fa12h
        mov    word ptr [eax + 6], 0
        inc    dl
        add    eax, 8
        xor    ecx, ecx
        mov    cl, dl
        cmp    ecx, dword ptr [esi + 0b2h]
        jb     SHORT loc_1_0487
loc_1_04E5:
        mov    cx, word ptr [esi + 45h]
        mov    word ptr [esi + 0c6h], 0
        lea    edi, [ecx + 12h]
        mov    dx, di
        mov    ax, 3
        out    dx, ax
        add    cx, 10h
        mov    dx, cx
        mov    ax, 200h
        out    dx, ax
        mov    dx, di
        mov    ax, 2
        out    dx, ax
        mov    ebx, dword ptr [esi + 0cch]
        mov    eax, ebx
        shr    eax, 10h
        mov    dx, cx
        and    ax, 0ffh
        out    dx, ax
        mov    dx, di
        mov    ax, 1
        out    dx, ax
        mov    dx, cx
        mov    ax, bx
        out    dx, ax
        mov    dx, di
        sub    ax, ax
        out    dx, ax
        mov    dx, cx
        mov    ax, 1
        out    dx, ax
        mov    edi, 0c350h
        mov    ebx, 100h
loc_1_0554:
        mov    cx, word ptr [esi + 45h]
        sub    ax, ax
        lea    edx, [ecx + 12h]
        out    dx, ax
        lea    edx, [ecx + 10h]
        in     ax, dx
        test   bx, ax
        jne    SHORT loc_1_0576
        push   0ah
        call   sub_1_0BB0
        dec    edi
        jne    SHORT loc_1_0554
        jmp    SHORT loc_1_058B
loc_1_0576:
        mov    cx, word ptr [esi + 45h]
        sub    ax, ax
        lea    edx, [ecx + 12h]
        out    dx, ax
        lea    edx, [ecx + 10h]
        mov    ax, 0ff00h
        out    dx, ax
loc_1_058B:
        or     edi, edi
        jne    SHORT loc_1_059A
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_059A:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_05B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0050
        mov    byte ptr [esi + 0e8h], 1
        mov    ecx, dword ptr [esi + 79h]
        lea    edi, [esi + 79h]
        or     ecx, ecx
        je     SHORT loc_1_061F
        cmp    dword ptr [ecx + 10h], 4
        jne    SHORT loc_1_05E2
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [ecx + 4]
        dec    dword ptr [eax + 14h]
        mov    ecx, dword ptr [edi]
loc_1_05E2:
        or     ecx, ecx
        je     SHORT loc_1_0618
loc_1_05E6:
        mov    eax, dword ptr [ecx + 10h]
        cmp    eax, 4
        je     SHORT loc_1_0610
        cmp    eax, 3
        je     SHORT loc_1_0610
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edi], eax
        mov    ebx, dword ptr [ecx + 4]
        push   0c001000ch
        push   ecx
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_0B0C
        dec    dword ptr [ebx + 14h]
        jmp    SHORT loc_1_0612
        db  08Bh, 0FFh
loc_1_0610:
        mov    edi, ecx
loc_1_0612:
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        jne    SHORT loc_1_05E6
loc_1_0618:
        mov    byte ptr [esi + 0ebh], 0
loc_1_061F:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0630:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    byte ptr [eax + 2], 1
        call   sub_1_0050
        push   3d090h
        call   sub_1_0BB0
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_0650:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [edx + 0b2h]
        mov    eax, esi
        shl    esi, 7
        sub    esi, eax
        lea    esi, [eax + esi*4]
        lea    esi, [esi + esi*2]
        sub    esi, eax
        mov    eax, dword ptr [edx + 8dh]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*4]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        lea    eax, [esi + eax + 1fh]
        mov    ecx, dword ptr [edx + 89h]
        or     ecx, ecx
        je     SHORT loc_1_06A4
        mov    esi, dword ptr [edx + 85h]
        push   esi
        push   ecx
        push   0
        push   eax
        mov    eax, dword ptr [edx + 71h]
        push   eax
        call   sub_1_0B48
loc_1_06A4:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_06B0:
        db  0A0h, 048h, 00Dh, 000h, 000h
        cmp    al, 0
        jne    SHORT loc_1_06D5
        db  0FEh, 005h, 048h, 00Dh, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     SHORT loc_1_06D7
        push   0
        push   0
        db  0E8h, 030h, 0F9h, 0FEh, 0FFh
        cmp    eax, 0
        jne    SHORT loc_1_06D7
loc_1_06D5:
        clc
        ret
loc_1_06D7:
        stc
        ret
AM1500T_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_06E8
        db  0FFh, 024h, 085h, 0EAh, 006h, 000h, 000h
loc_1_06E8:
        clc
        ret
        db  0B0h, 006h, 000h, 000h, 0E8h, 006h, 000h, 000h, 0E8h, 006h, 000h, 000h, 0E8h, 006h, 000h, 000h
        db  0E8h, 006h, 000h, 000h, 0E8h, 006h, 000h, 000h, 0E8h, 006h, 000h, 000h, 0E8h, 006h, 000h, 000h
        db  0E8h, 006h, 000h, 000h, 0E8h, 006h, 000h, 000h, 0E8h, 006h, 000h, 000h, 0E8h, 006h, 000h, 000h
        db  0E8h, 006h, 000h, 000h, 0E8h, 006h, 000h, 000h, 0E8h, 006h, 000h, 000h, 0E8h, 006h, 000h, 000h
        db  0E8h, 006h, 000h, 000h, 0E8h, 006h, 000h, 000h, 0E8h, 006h, 000h, 000h, 0E8h, 006h, 000h, 000h
        db  0E8h, 006h, 000h, 000h, 0E8h, 006h, 000h, 000h, 0E8h, 006h, 000h, 000h, 0E8h, 006h, 000h, 000h
        db  0E8h, 006h, 000h, 000h, 0E8h, 006h, 000h, 000h, 0B0h, 006h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_1_0760:
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
        call   sub_1_0790
        pop    ebp
        ret    20h
        db  08Dh, 064h, 024h, 000h
sub_1_0790:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 18h], 0eh
        jb     NEAR PTR loc_1_0830
        mov    edx, dword ptr [ebp + 10h]
        mov    al, byte ptr [edx]
        test   al, 1
        je     SHORT loc_1_07D0
        cmp    dword ptr [edx + 2], -1
        jne    SHORT loc_1_07C5
        cmp    al, 0ffh
        jne    SHORT loc_1_07C5
        cmp    byte ptr [edx + 1], 0ffh
        jne    SHORT loc_1_07C5
        mov    dword ptr [ebp - 4], 8
        jmp    SHORT loc_1_0837
loc_1_07C5:
        mov    dword ptr [ebp - 4], 2
        jmp    SHORT loc_1_0837
        db  08Bh, 0FFh
loc_1_07D0:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0ch], 20h
        je     SHORT loc_1_0820
        mov    edx, dword ptr [ebp + 10h]
        lea    ecx, [esi + 2ch]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_07F6
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_07FD
loc_1_07F6:
        mov    dword ptr [ebp - 10h], 1
loc_1_07FD:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_0810
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_083A
        db  08Dh, 064h, 024h, 000h
loc_1_0810:
        mov    dword ptr [ebp - 4], 20h
        jmp    SHORT loc_1_083A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0820:
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_083A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0830:
        mov    dword ptr [ebp - 4], 20h
loc_1_0837:
        mov    esi, dword ptr [ebp + 8]
loc_1_083A:
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     NEAR PTR loc_1_0931
loc_1_0845:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, ebx
        and    eax, dword ptr [ebp - 4]
        test   al, 9
        jne    SHORT loc_1_089D
        test   al, 2
        je     SHORT loc_1_0889
        lea    eax, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 0B6h, 00Bh, 0FFh, 0FFh
        or     al, al
        je     SHORT loc_1_0889
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 19h]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        test   dword ptr [eax + ecx*4], edx
        jne    SHORT loc_1_089D
loc_1_0889:
        test   byte ptr [ebp - 4], 2
        je     SHORT loc_1_0894
        test   bl, 4
        jne    SHORT loc_1_089D
loc_1_0894:
        test   bl, 20h
        je     NEAR PTR loc_1_0926
loc_1_089D:
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
        call   sub_1_0B12
        mov    byte ptr [edi + 18h], 1
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_0926
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_08DF
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_08DF:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_08F0
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_08F6
        db  08Bh, 0FFh
loc_1_08F0:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_08F6:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_0905
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_0B18
loc_1_0905:
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
        call   sub_1_0BBC
        mov    edi, ebx
        jmp    SHORT loc_1_0929
loc_1_0926:
        mov    edi, dword ptr [edi + 10h]
loc_1_0929:
        or     edi, edi
        jne    NEAR PTR loc_1_0845
loc_1_0931:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0940:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0950
        pop    ebp
        ret    4
sub_1_0950:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     SHORT loc_1_09CE
loc_1_0960:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_09C7
        mov    byte ptr [edi + 18h], 0
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_0B18
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_09C7
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_098C
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_098C:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_09A0
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_09A6
        db  005h, 000h, 000h, 000h, 000h
loc_1_09A0:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_09A6:
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
        call   sub_1_0BBC
        mov    edi, ebx
        jmp    SHORT loc_1_09CA
loc_1_09C7:
        mov    edi, dword ptr [edi + 10h]
loc_1_09CA:
        or     edi, edi
        jne    SHORT loc_1_0960
loc_1_09CE:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_09E0:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0ch]
        test   al, 20h
        je     SHORT loc_1_09F5
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_09F5:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     SHORT loc_1_0A31
        cmp    dword ptr [ecx + 2], -1
        jne    SHORT loc_1_0A22
        cmp    bl, 0ffh
        jne    SHORT loc_1_0A22
        cmp    byte ptr [ecx + 1], 0ffh
        jne    SHORT loc_1_0A22
        test   al, 8
        je     SHORT loc_1_0A1B
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0A1B:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_0A22:
        test   al, 4
        jne    SHORT loc_1_0A2A
        test   al, 2
        je     SHORT loc_1_0A49
loc_1_0A2A:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0A31:
        mov    eax, dword ptr [edx + 2eh]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_0A49
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 2ch], ax
        jne    SHORT loc_1_0A49
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0A49:
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
sub_1_0ACA:
        db  0CDh, 020h, 031h, 080h, 028h, 000h
sub_1_0AD0:
        db  0CDh, 020h, 032h, 080h, 028h, 000h
sub_1_0AD6:
        db  0CDh, 020h, 033h, 080h, 028h, 000h
sub_1_0ADC:
        db  0CDh, 020h, 034h, 080h, 028h, 000h
sub_1_0AE2:
        db  0CDh, 020h, 035h, 080h, 028h, 000h
sub_1_0AE8:
        db  0CDh, 020h, 036h, 080h, 028h, 000h
sub_1_0AEE:
        db  0CDh, 020h, 037h, 080h, 028h, 000h
sub_1_0AF4:
        db  0CDh, 020h, 038h, 080h, 028h, 000h
sub_1_0AFA:
        db  0CDh, 020h, 039h, 080h, 028h, 000h
        db  0CDh, 020h, 03Ah, 080h, 028h, 000h
        db  0CDh, 020h, 03Bh, 080h, 028h, 000h
sub_1_0B0C:
        db  0CDh, 020h, 03Ch, 080h, 028h, 000h
sub_1_0B12:
        db  0CDh, 020h, 03Dh, 080h, 028h, 000h
sub_1_0B18:
        db  0CDh, 020h, 03Eh, 080h, 028h, 000h
sub_1_0B1E:
        db  0CDh, 020h, 03Fh, 080h, 028h, 000h
sub_1_0B24:
        db  0CDh, 020h, 040h, 080h, 028h, 000h
sub_1_0B2A:
        db  0CDh, 020h, 041h, 080h, 028h, 000h
        db  0CDh, 020h, 009h, 080h, 028h, 000h
        db  0CDh, 020h, 008h, 080h, 028h, 000h
sub_1_0B3C:
        db  0CDh, 020h, 044h, 080h, 028h, 000h
sub_1_0B42:
        db  0CDh, 020h, 048h, 080h, 028h, 000h
sub_1_0B48:
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
sub_1_0B92:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
sub_1_0B98:
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
sub_1_0B9E:
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
sub_1_0BA4:
        db  0CDh, 020h, 010h, 080h, 028h, 000h
        db  0CDh, 020h, 011h, 080h, 028h, 000h
sub_1_0BB0:
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_0BB6:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_0BBC:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_0BE0:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_0C04:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_0C28:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
sub_1_0C2E:
        db  0CDh, 020h, 028h, 080h, 028h, 000h
sub_1_0C34:
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_0C3A:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_0C40:
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
sub_1_0C88:
        db  0CDh, 020h, 045h, 080h, 028h, 000h
sub_1_0C8E:
        db  0CDh, 020h, 043h, 080h, 028h, 000h
sub_1_0C94:
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        add    byte ptr [eax], al
sub_1_0C9C:
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
sub_1_0CBC:
        push   esi
        push   edi
        pushfd
        mov    ecx, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        cmp    esi, edi
        jae    SHORT loc_1_0CDC
        or     ecx, ecx
        je     SHORT loc_1_0CF2
        std
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    SHORT loc_1_0CF0
loc_1_0CDC:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     SHORT loc_1_0CF0
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd
        mov    cl, al
loc_1_0CF0:
        rep movsb
loc_1_0CF2:
        popfd
        pop    edi
        pop    esi
        ret    0ch
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 00Ah, 000h, 000h, 041h, 04Dh, 031h, 035h
        db  030h, 030h, 054h, 020h, 000h, 040h, 000h, 054h, 0D9h, 006h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0D50:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0d0h]
        mov    eax, dword ptr [edi + 28h]
        lea    ecx, [edi + 24h]
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_1_0D7A
        mov    dword ptr [esi + 0d4h], 0
loc_1_0D7A:
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 0d0h], eax
        mov    dword ptr [esi + 0d8h], edi
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        push   0
        push   0
        push   edi
        call   sub_1_0BE0
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_0C04
        cmp    dword ptr [ebp - 4], 0eh
        jae    SHORT loc_1_0DC3
        cmp    dword ptr [ebp - 4], 6
        jb     SHORT loc_1_0E18
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        jmp    SHORT loc_1_0E08
loc_1_0DC3:
        cmp    dword ptr [ebp - 4], 0deh
        jae    SHORT loc_1_0DF0
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp - 4], eax
        je     SHORT loc_1_0DF0
        lea    ebx, [esi + 178h]
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        push   0d0h
        push   0eh
        push   edi
        call   sub_1_1370
        jmp    SHORT loc_1_0DFA
        db  08Bh, 0FFh
loc_1_0DF0:
        mov    ebx, dword ptr [ebp - 8]
        add    ebx, 0eh
        sub    dword ptr [ebp - 4], 0eh
loc_1_0DFA:
        mov    eax, dword ptr [ebp - 10h]
        sub    eax, 0eh
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   ebx
        push   0eh
loc_1_0E08:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   eax
        push   0
        mov    eax, dword ptr [esi + 75h]
        push   eax
        call   sub_1_0760
loc_1_0E18:
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        push   edi
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_0AFA
        cmp    dword ptr [esi + 0d0h], 0
        jne    SHORT loc_1_0E39
        mov    eax, dword ptr [esi + 75h]
        push   eax
        call   sub_1_0940
loc_1_0E39:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        dec    dword ptr [eax + 14h]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0E50:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [edx + 28h]
        lea    esi, [edx + 24h]
        inc    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0d0h], 0
        jne    SHORT loc_1_0E74
        mov    dword ptr [eax + 0d0h], edx
        jmp    SHORT loc_1_0E7D
loc_1_0E74:
        mov    ecx, dword ptr [eax + 0d4h]
        mov    dword ptr [ecx + 24h], edx
loc_1_0E7D:
        mov    dword ptr [esi], 0
        mov    dword ptr [eax + 0d4h], edx
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_0E90:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        sub    ax, ax
        mov    cx, word ptr [edi + 45h]
        lea    esi, [ecx + 12h]
        mov    dx, si
        out    dx, ax
        add    cx, 10h
        mov    dx, cx
        out    dx, ax
        mov    dx, si
        out    dx, ax
        mov    dx, cx
        in     ax, dx
        mov    si, word ptr [edi + 45h]
        mov    word ptr [edi + 43h], ax
        lea    ecx, [esi + 12h]
        mov    dx, cx
        sub    ax, ax
        out    dx, ax
        add    si, 10h
        mov    ax, word ptr [edi + 43h]
        mov    dx, si
        out    dx, ax
        test   ah, 84h
        je     SHORT loc_1_0EE7
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_0EE7:
        mov    dx, cx
        sub    ax, ax
        out    dx, ax
        mov    dx, si
        mov    ax, 40h
        out    dx, ax
        sub    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_0F00:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 2], 0
        jne    NEAR PTR loc_1_100A
        cmp    byte ptr [esi + 0fh], 0
        jne    NEAR PTR loc_1_100A
        mov    byte ptr [esi + 0fh], 1
        mov    byte ptr [esi + 2ah], 0
        test   byte ptr [esi + 44h], 4
        je     SHORT loc_1_0F3E
        inc    dword ptr [esi + 49h]
        push   esi
        call   sub_1_1110
        mov    eax, dword ptr [esi + 75h]
        push   eax
        call   sub_1_0940
        dec    dword ptr [esi + 49h]
loc_1_0F3E:
        test   byte ptr [esi + 44h], 80h
        je     SHORT loc_1_0FAE
        inc    dword ptr [esi + 49h]
        test   byte ptr [esi + 44h], 20h
        je     SHORT loc_1_0F53
        inc    dword ptr [esi + 16ch]
loc_1_0F53:
        test   byte ptr [esi + 44h], 10h
        je     SHORT loc_1_0F80
        inc    dword ptr [esi + 0fch]
        push   esi
        call   sub_1_0050
        push   esi
        call   sub_1_03C0
        mov    cx, word ptr [esi + 45h]
        sub    ax, ax
        lea    edx, [ecx + 12h]
        out    dx, ax
        lea    edx, [ecx + 10h]
        mov    ax, 2
        out    dx, ax
loc_1_0F80:
        mov    ax, word ptr [esi + 43h]
        and    al, 33h
        cmp    al, 33h
        je     SHORT loc_1_0FAB
        push   esi
        call   sub_1_0050
        push   esi
        call   sub_1_03C0
        mov    cx, word ptr [esi + 45h]
        sub    ax, ax
        lea    edx, [ecx + 12h]
        out    dx, ax
        lea    edx, [ecx + 10h]
        mov    ax, 2
        out    dx, ax
loc_1_0FAB:
        dec    dword ptr [esi + 49h]
loc_1_0FAE:
        mov    byte ptr [esi + 0fh], 0
        cmp    byte ptr [esi + 11h], 0
        jne    SHORT loc_1_0FF5
        cmp    dword ptr [esi + 0dch], 0
        jne    SHORT loc_1_0FE3
        cmp    dword ptr [esi + 0d0h], 0
        jne    SHORT loc_1_0FE3
        cmp    byte ptr [esi + 0e8h], 0
        je     SHORT loc_1_0FD9
        cmp    dword ptr [esi + 49h], 1
        je     SHORT loc_1_0FE3
loc_1_0FD9:
        mov    eax, dword ptr [esi + 59h]
        sub    eax, esi
        cmp    eax, 59h
        je     SHORT loc_1_0FF5
loc_1_0FE3:
        mov    byte ptr [esi + 11h], 1
        inc    dword ptr [esi + 49h]
        push   0
        lea    eax, [esi + 12h]
        push   eax
        call   sub_1_0B92
loc_1_0FF5:
        mov    cx, word ptr [esi + 45h]
        sub    ax, ax
        lea    edx, [ecx + 12h]
        out    dx, ax
        lea    edx, [ecx + 10h]
        mov    ax, 40h
        out    dx, ax
loc_1_100A:
        pop    esi
        pop    ebp
        ret    10h
        db  090h
loc_1_1010:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        sub    ebx, ebx
        add    eax, 0
loc_1_1020:
        cmp    dword ptr [esi + 0d0h], ebx
        jne    SHORT loc_1_1048
        cmp    dword ptr [esi + 0dch], ebx
        jne    SHORT loc_1_1048
        cmp    byte ptr [esi + 0e8h], bl
        je     SHORT loc_1_103E
        cmp    dword ptr [esi + 49h], 2
        je     SHORT loc_1_1048
loc_1_103E:
        mov    eax, dword ptr [esi + 59h]
        sub    eax, esi
        cmp    eax, 59h
        je     SHORT loc_1_10C5
loc_1_1048:
        cmp    byte ptr [esi + 0e8h], bl
        je     SHORT loc_1_1056
        cmp    dword ptr [esi + 49h], 2
        je     SHORT loc_1_10B1
loc_1_1056:
        cmp    dword ptr [esi + 0dch], ebx
        je     SHORT loc_1_1068
        push   esi
        call   sub_1_2650
        cmp    al, bl
        je     SHORT loc_1_10C5
loc_1_1068:
        cmp    dword ptr [esi + 0d0h], ebx
        je     SHORT loc_1_1076
        push   esi
        call   sub_1_0D50
loc_1_1076:
        mov    ecx, dword ptr [esi + 59h]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, 59h
        je     SHORT loc_1_1020
        cmp    dword ptr [ecx + 14h], ebx
        jne    SHORT loc_1_1020
        mov    edi, dword ptr [ecx + 10h]
        mov    edx, dword ptr [ecx + 4]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    edx, dword ptr [ecx]
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], eax
        push   ebx
        push   45h
        push   ecx
        call   sub_1_0BBC
        dec    dword ptr [esi + 55h]
        push   ebx
        push   edi
        call   sub_1_0AF4
        jmp    NEAR PTR loc_1_1020
loc_1_10B1:
        mov    byte ptr [esi + 11h], 0
        dec    dword ptr [esi + 49h]
        push   esi
        call   sub_1_0300
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_1_10C5:
        cmp    dword ptr [esi + 0d0h], 0
        jne    SHORT loc_1_10F0
        cmp    dword ptr [esi + 0dch], 0
        jne    SHORT loc_1_10F0
        mov    eax, dword ptr [esi + 59h]
        sub    eax, esi
        cmp    eax, 59h
        jne    SHORT loc_1_10F0
        mov    byte ptr [esi + 11h], 0
        dec    dword ptr [esi + 49h]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  090h
loc_1_10F0:
        push   0
        lea    eax, [esi + 12h]
        push   eax
        call   sub_1_0B92
        dec    dword ptr [esi + 49h]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1110:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    cx, word ptr [esi + 0c6h]
        mov    eax, ecx
        and    eax, 0ffffh
        shl    eax, 3
        add    eax, dword ptr [esi + 0beh]
        mov    edx, eax
        mov    dword ptr [ebp - 8], eax
        mov    bx, word ptr [edx + 2]
        and    bx, 0ff00h
        test   bh, 80h
        jne    NEAR PTR loc_1_1329
        mov    edi, dword ptr [ebp - 14h]
loc_1_1150:
        mov    ax, word ptr [esi + 0c6h]
        inc    ax
        mov    word ptr [esi + 0c6h], ax
        and    eax, 0ffffh
        cmp    eax, dword ptr [esi + 0b2h]
        jb     SHORT loc_1_1176
        mov    word ptr [esi + 0c6h], 0
loc_1_1176:
        mov    ax, bx
        and    ax, 4000h
        mov    word ptr [ebp - 0ch], ax
        or     ax, ax
        jne    NEAR PTR loc_1_123E
        test   bh, 1
        je     NEAR PTR loc_1_123E
        test   bh, 2
        je     NEAR PTR loc_1_123E
        mov    edx, dword ptr [ebp - 8]
        xor    edi, edi
        mov    di, word ptr [edx + 6]
        sub    edi, 4
        cmp    edi, 5eah
        mov    edx, edi
        jb     SHORT loc_1_11B7
        mov    edx, 5eah
loc_1_11B7:
        and    ecx, 0ffffh
        mov    eax, ecx
        shl    ecx, 6
        sub    ecx, eax
        lea    ecx, [eax + ecx*4]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        add    ecx, dword ptr [esi + 0b6h]
        cmp    byte ptr [esi + 0e8h], 0
        jne    NEAR PTR loc_1_1329
        mov    al, byte ptr [ecx]
        test   al, 1
        je     SHORT loc_1_1208
        cmp    dword ptr [ecx + 2], -1
        jne    SHORT loc_1_1200
        cmp    al, 0ffh
        jne    SHORT loc_1_1200
        cmp    byte ptr [ecx + 1], 0ffh
        jne    SHORT loc_1_1200
        mov    word ptr [ebp - 2], 4
        jmp    SHORT loc_1_120E
        db  08Dh, 049h, 000h
loc_1_1200:
        mov    word ptr [ebp - 2], 2
        jmp    SHORT loc_1_120E
loc_1_1208:
        mov    word ptr [ebp - 2], 1
loc_1_120E:
        cmp    edi, 0eh
        jae    SHORT loc_1_1224
        cmp    edi, 6
        jb     SHORT loc_1_123E
        lea    eax, [ecx + 0eh]
        push   0
        push   0
        push   0
        push   edi
        jmp    SHORT loc_1_1232
loc_1_1224:
        lea    eax, [edi - 0eh]
        push   eax
        sub    edx, 0eh
        push   edx
        lea    eax, [ecx + 0eh]
        push   eax
        push   0eh
loc_1_1232:
        push   ecx
        push   ecx
        push   eax
        mov    eax, dword ptr [esi + 75h]
        push   eax
        call   sub_1_0760
loc_1_123E:
        mov    edx, dword ptr [ebp - 8]
        mov    ax, word ptr [edx + 2]
        and    ax, 0ffh
        mov    word ptr [edx + 2], ax
        or     ax, 8000h
        cmp    word ptr [ebp - 0ch], 0
        mov    word ptr [edx + 2], ax
        jne    SHORT loc_1_12D0
        inc    dword ptr [esi + 0f0h]
        cmp    word ptr [ebp - 2], 4
        jne    SHORT loc_1_1288
        inc    dword ptr [esi + 144h]
        mov    eax, dword ptr [esi + 128h]
        lea    ecx, [esi + 128h]
        add    eax, edi
        mov    dword ptr [ecx], eax
        cmp    edi, eax
        jbe    SHORT loc_1_12FB
        inc    dword ptr [ecx + 4]
        jmp    SHORT loc_1_12FB
loc_1_1288:
        cmp    word ptr [ebp - 2], 2
        jne    SHORT loc_1_12B0
        inc    dword ptr [esi + 140h]
        mov    eax, dword ptr [esi + 120h]
        lea    ecx, [esi + 120h]
        add    eax, edi
        mov    dword ptr [ecx], eax
        cmp    edi, eax
        jbe    SHORT loc_1_12FB
        inc    dword ptr [ecx + 4]
        jmp    SHORT loc_1_12FB
        db  08Bh, 0FFh
loc_1_12B0:
        inc    dword ptr [esi + 13ch]
        mov    eax, dword ptr [esi + 118h]
        lea    ecx, [esi + 118h]
        add    eax, edi
        mov    dword ptr [ecx], eax
        cmp    edi, eax
        jbe    SHORT loc_1_12FB
        inc    dword ptr [ecx + 4]
        jmp    SHORT loc_1_12FB
        db  090h
loc_1_12D0:
        inc    dword ptr [esi + 0f8h]
        test   bh, 8
        je     SHORT loc_1_12E3
        inc    dword ptr [esi + 178h]
        jmp    SHORT loc_1_12FB
loc_1_12E3:
        test   bh, 20h
        je     SHORT loc_1_12F0
        inc    dword ptr [esi + 150h]
        jmp    SHORT loc_1_12FB
loc_1_12F0:
        test   bh, 4
        je     SHORT loc_1_12FB
        inc    dword ptr [esi + 164h]
loc_1_12FB:
        mov    cx, word ptr [esi + 0c6h]
        mov    eax, ecx
        and    eax, 0ffffh
        shl    eax, 3
        add    eax, dword ptr [esi + 0beh]
        mov    edx, eax
        mov    dword ptr [ebp - 8], eax
        mov    bx, word ptr [edx + 2]
        and    bx, 0ff00h
        test   bh, 80h
        je     NEAR PTR loc_1_1150
loc_1_1329:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1340:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 2ah], 0
        je     SHORT loc_1_1353
        push   esi
        call   0
loc_1_1353:
        mov    byte ptr [esi + 2ah], 1
        push   1388h
        add    esi, 1eh
        push   esi
        call   sub_1_0B92
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1370:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 18h]
        sub    esi, esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [ebp + 10h], esi
        je     NEAR PTR loc_1_143A
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0BE0
        cmp    dword ptr [ebp - 10h], esi
        je     NEAR PTR loc_1_143A
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0C04
        cmp    dword ptr [ebp + 10h], esi
        je     SHORT loc_1_1435
        mov    edi, dword ptr [ebp + 0ch]
        mov    edi, edi
loc_1_13C0:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_13EC
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0C28
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_1435
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0C04
        jmp    SHORT loc_1_1430
loc_1_13EC:
        or     edi, edi
        je     SHORT loc_1_1409
        cmp    dword ptr [ebp - 4], edi
        jae    SHORT loc_1_1401
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_1430
loc_1_1401:
        add    dword ptr [ebp - 0ch], edi
        sub    dword ptr [ebp - 4], edi
        sub    edi, edi
loc_1_1409:
        mov    ebx, dword ptr [ebp + 10h]
        sub    ebx, esi
        mov    eax, dword ptr [ebp - 4]
        cmp    ebx, eax
        jb     SHORT loc_1_1417
        mov    ebx, eax
loc_1_1417:
        push   ebx
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        add    esi, ebx
        push   eax
        call   sub_1_0C9C
        add    dword ptr [ebp + 14h], ebx
        add    dword ptr [ebp - 0ch], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_1430:
        cmp    esi, dword ptr [ebp + 10h]
        jb     SHORT loc_1_13C0
loc_1_1435:
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], esi
loc_1_143A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1450:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 18h]
        sub    esi, esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [ebp + 0ch], esi
        je     NEAR PTR loc_1_151B
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_0BE0
        cmp    dword ptr [ebp - 14h], esi
        je     NEAR PTR loc_1_151B
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0C04
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp + 0ch], esi
        je     SHORT loc_1_1516
        mov    edi, dword ptr [ebp + 14h]
loc_1_14A4:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_14D0
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0C28
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_1516
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0C04
        jmp    SHORT loc_1_1511
loc_1_14D0:
        or     edi, edi
        je     SHORT loc_1_14ED
        cmp    edi, dword ptr [ebp - 4]
        jbe    SHORT loc_1_14E5
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_1511
loc_1_14E5:
        add    dword ptr [ebp - 0ch], edi
        sub    dword ptr [ebp - 4], edi
        sub    edi, edi
loc_1_14ED:
        mov    ebx, dword ptr [ebp + 0ch]
        sub    ebx, esi
        mov    eax, dword ptr [ebp - 4]
        cmp    ebx, eax
        jb     SHORT loc_1_14FB
        mov    ebx, eax
loc_1_14FB:
        push   ebx
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        add    esi, ebx
        push   eax
        call   sub_1_0C9C
        add    dword ptr [ebp - 10h], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_1511:
        cmp    dword ptr [ebp + 0ch], esi
        ja     SHORT loc_1_14A4
loc_1_1516:
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], esi
loc_1_151B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1530:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [esi + 49h]
        cmp    byte ptr [esi + 0e8h], 0
        jne    SHORT loc_1_15A7
        cmp    byte ptr [edi + 40h], 0
        jne    SHORT loc_1_15A0
        inc    dword ptr [edi + 14h]
        mov    ecx, dword ptr [ebp + 0ch]
        or     ecx, ecx
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        jne    SHORT loc_1_1580
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 0eh
        push   eax
        mov    eax, dword ptr [esi + 0d8h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        call   sub_1_0C2E
        dec    dword ptr [edi + 14h]
        sub    edi, edi
        jmp    SHORT loc_1_15AC
        db  090h
loc_1_1580:
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        add    eax, ecx
        push   eax
        call   sub_1_1450
        dec    dword ptr [edi + 14h]
        sub    edi, edi
        jmp    SHORT loc_1_15AC
        db  08Dh, 064h, 024h, 000h
loc_1_15A0:
        mov    edi, 0c001000ch
        jmp    SHORT loc_1_15AC
loc_1_15A7:
        mov    edi, 0c001000dh
loc_1_15AC:
        cmp    byte ptr [esi + 11h], 0
        jne    SHORT loc_1_15F4
        cmp    dword ptr [esi + 49h], 2
        jne    SHORT loc_1_15C1
        cmp    byte ptr [esi + 0e8h], 0
        jne    SHORT loc_1_15DD
loc_1_15C1:
        cmp    dword ptr [esi + 0dch], 0
        jne    SHORT loc_1_15DD
        cmp    dword ptr [esi + 0d0h], 0
        jne    SHORT loc_1_15DD
        mov    eax, dword ptr [esi + 59h]
        sub    eax, esi
        cmp    eax, 59h
        je     SHORT loc_1_15F4
loc_1_15DD:
        mov    byte ptr [esi + 11h], 1
        push   0
        add    esi, 12h
        push   esi
        call   sub_1_0B92
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    18h
loc_1_15F4:
        dec    dword ptr [esi + 49h]
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    18h
        db  090h
loc_1_1600:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 8]
        inc    dword ptr [esi + 49h]
        cmp    byte ptr [esi + 0e8h], 0
        jne    SHORT loc_1_1657
        cmp    byte ptr [eax + 40h], 0
        jne    SHORT loc_1_1650
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [edx + 10h]
        or     ecx, ecx
        jl     SHORT loc_1_162C
        cmp    ecx, 1
        jle    SHORT loc_1_1633
loc_1_162C:
        mov    edi, 0c00000bbh
        jmp    SHORT loc_1_165C
loc_1_1633:
        inc    dword ptr [eax + 14h]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [edx], 0
        push   edx
        mov    edi, 103h
        push   esi
        call   sub_1_16B0
        jmp    SHORT loc_1_165C
        db  08Dh, 049h, 000h
loc_1_1650:
        mov    edi, 0c0010002h
        jmp    SHORT loc_1_165C
loc_1_1657:
        mov    edi, 0c001000dh
loc_1_165C:
        cmp    byte ptr [esi + 11h], 0
        jne    SHORT loc_1_16A4
        cmp    dword ptr [esi + 49h], 2
        jne    SHORT loc_1_1671
        cmp    byte ptr [esi + 0e8h], 0
        jne    SHORT loc_1_168D
loc_1_1671:
        cmp    dword ptr [esi + 0dch], 0
        jne    SHORT loc_1_168D
        cmp    dword ptr [esi + 0d0h], 0
        jne    SHORT loc_1_168D
        mov    eax, dword ptr [esi + 59h]
        sub    eax, esi
        cmp    eax, 59h
        je     SHORT loc_1_16A4
loc_1_168D:
        mov    byte ptr [esi + 11h], 1
        push   0
        add    esi, 12h
        push   esi
        call   sub_1_0B92
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_16A4:
        dec    dword ptr [esi + 49h]
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  090h
sub_1_16B0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 79h], 0
        je     SHORT loc_1_16D0
        mov    edx, dword ptr [eax + 7dh]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx], ecx
        mov    dword ptr [eax + 7dh], ecx
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_16D0:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 79h], ecx
        mov    dword ptr [eax + 7dh], ecx
        cmp    byte ptr [eax + 0ebh], 0
        jne    SHORT loc_1_16E8
        push   eax
        call   sub_1_16F0
loc_1_16E8:
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_16F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
loc_1_16FC:
        mov    edi, dword ptr [esi + 79h]
        or     edi, edi
        je     NEAR PTR loc_1_18F0
        cmp    byte ptr [esi + 0e8h], 0
        jne    NEAR PTR loc_1_1900
        mov    byte ptr [esi + 0ebh], 1
        mov    eax, dword ptr [edi + 10h]
        or     eax, eax
        je     SHORT loc_1_1743
        cmp    eax, 1
        je     SHORT loc_1_176B
        cmp    eax, 2
        je     SHORT loc_1_1791
        cmp    eax, 3
        je     NEAR PTR loc_1_17B9
        cmp    eax, 4
        je     NEAR PTR loc_1_1837
        jmp    NEAR PTR loc_1_1897
loc_1_1743:
        push   0
        lea    eax, [edi + 24h]
        push   eax
        lea    eax, [edi + 20h]
        push   eax
        mov    eax, dword ptr [edi + 1ch]
        push   eax
        mov    eax, dword ptr [edi + 18h]
        push   eax
        mov    eax, dword ptr [edi + 14h]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        push   esi
        call   sub_1_19B0
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_1897
loc_1_176B:
        lea    eax, [edi + 24h]
        push   eax
        lea    eax, [edi + 20h]
        push   eax
        mov    eax, dword ptr [edi + 1ch]
        push   eax
        mov    eax, dword ptr [edi + 18h]
        push   eax
        mov    eax, dword ptr [edi + 14h]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        push   esi
        call   sub_1_1DD0
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_1897
loc_1_1791:
        push   1
        lea    eax, [edi + 24h]
        push   eax
        lea    eax, [edi + 20h]
        push   eax
        mov    eax, dword ptr [edi + 1ch]
        push   eax
        mov    eax, dword ptr [edi + 18h]
        push   eax
        mov    eax, dword ptr [edi + 14h]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        push   esi
        call   sub_1_19B0
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_1897
loc_1_17B9:
        mov    ebx, dword ptr [edi + 4]
        lea    eax, [ebx + 0ch]
        push   eax
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        mov    eax, dword ptr [ebx + 8]
        push   ebx
        mov    eax, dword ptr [eax + 75h]
        push   eax
        db  0E8h, 0FFh, 0F5h, 0FEh, 0FFh
        or     al, al
        jne    SHORT loc_1_1808
        push   0
        push   0c0000001h
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_0AEE
        push   8
        push   1
        push   2
        push   0aa000002h
        mov    eax, dword ptr [esi + 71h]
        push   eax
        call   sub_1_0C88
        add    esp, 14h
        push   0
        push   45h
        push   ebx
        call   sub_1_0BBC
        jmp    SHORT loc_1_182E
loc_1_1808:
        lea    eax, [esi + 4dh]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [esi + 51h]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [esi + 51h]
        push   0
        mov    dword ptr [eax], ebx
        mov    dword ptr [esi + 51h], ebx
        inc    dword ptr [esi + 55h]
        inc    dword ptr [ebx + 14h]
        push   0
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_0AEE
loc_1_182E:
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_1897
loc_1_1837:
        mov    ebx, dword ptr [edi + 4]
        push   0
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        mov    eax, dword ptr [esi + 75h]
        push   eax
        db  0E8h, 057h, 0F6h, 0FEh, 0FFh
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        jne    SHORT loc_1_1855
        dec    dword ptr [ebx + 14h]
        jmp    SHORT loc_1_186E
loc_1_1855:
        cmp    dword ptr [ebp - 4], 103h
        je     SHORT loc_1_186E
        cmp    dword ptr [ebp - 4], 0c001000eh
        jne    SHORT loc_1_186E
        mov    dword ptr [ebp - 4], 103h
loc_1_186E:
        mov    byte ptr [ebx + 40h], 1
        dec    dword ptr [ebx + 14h]
        mov    ecx, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 4], eax
        lea    eax, [esi + 59h]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [esi + 5dh]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [esi + 5dh]
        mov    dword ptr [eax], ebx
        mov    dword ptr [esi + 5dh], ebx
loc_1_1897:
        cmp    dword ptr [ebp - 4], 103h
        je     SHORT loc_1_1907
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 79h], eax
        mov    eax, dword ptr [edi + 10h]
        or     eax, eax
        je     SHORT loc_1_18D0
        cmp    eax, 1
        je     SHORT loc_1_18D0
        cmp    eax, 2
        jne    NEAR PTR loc_1_16FC
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 71h]
        push   eax
        call   sub_1_0B2A
        dec    dword ptr [esi + 49h]
        jmp    NEAR PTR loc_1_16FC
loc_1_18D0:
        mov    ebx, dword ptr [edi + 4]
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_0B0C
        dec    dword ptr [ebx + 14h]
        jmp    NEAR PTR loc_1_16FC
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_18F0:
        mov    byte ptr [esi + 0ebh], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1900:
        mov    byte ptr [esi + 0ebh], 0
loc_1_1907:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1910:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 49h]
        cmp    byte ptr [esi + 0e8h], 0
        jne    SHORT loc_1_1952
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 10h], 2
        je     SHORT loc_1_1934
        mov    edi, 0c00000bbh
        jmp    SHORT loc_1_1957
loc_1_1934:
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax], 0
        inc    dword ptr [esi + 49h]
        push   eax
        mov    edi, 103h
        push   esi
        call   sub_1_16B0
        jmp    SHORT loc_1_1957
loc_1_1952:
        mov    edi, 0c001000dh
loc_1_1957:
        cmp    byte ptr [esi + 11h], 0
        jne    SHORT loc_1_19A0
        cmp    dword ptr [esi + 49h], 2
        jne    SHORT loc_1_196C
        cmp    byte ptr [esi + 0e8h], 0
        jne    SHORT loc_1_1988
loc_1_196C:
        cmp    dword ptr [esi + 0dch], 0
        jne    SHORT loc_1_1988
        cmp    dword ptr [esi + 0d0h], 0
        jne    SHORT loc_1_1988
        mov    eax, dword ptr [esi + 59h]
        sub    eax, esi
        cmp    eax, 59h
        je     SHORT loc_1_19A0
loc_1_1988:
        mov    byte ptr [esi + 11h], 1
        push   0
        add    esi, 12h
        push   esi
        call   sub_1_0B92
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  090h
loc_1_19A0:
        dec    dword ptr [esi + 49h]
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_19B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    cl, byte ptr [ebp + 24h]
        or     cl, cl
        je     SHORT loc_1_19D0
        db  0C7h, 045h, 0F0h, 0D8h, 027h, 000h, 000h
        mov    edi, 34h
        jmp    SHORT loc_1_19DC
        db  08Bh, 0FFh
loc_1_19D0:
        db  0C7h, 045h, 0F0h, 0A8h, 028h, 000h, 000h
        mov    edi, 18h
loc_1_19DC:
        sub    esi, esi
        cmp    edi, esi
        jle    SHORT loc_1_19F6
        mov    eax, dword ptr [ebp - 10h]
        mov    edx, dword ptr [ebp + 10h]
loc_1_19E8:
        cmp    dword ptr [eax], edx
        je     SHORT loc_1_19F9
        add    eax, 4
        inc    esi
        cmp    edi, esi
        jg     SHORT loc_1_19E8
        jmp    SHORT loc_1_19F9
loc_1_19F6:
        mov    edx, dword ptr [ebp + 10h]
loc_1_19F9:
        cmp    edi, esi
        jne    SHORT loc_1_1A14
        mov    eax, dword ptr [ebp + 1ch]
        pop    edi
        mov    dword ptr [eax], 0
        mov    eax, 0c00000bbh
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
loc_1_1A14:
        lea    ebx, [ebp - 8]
        mov    esi, 4
        mov    eax, edx
        and    eax, 0ffff0000h
        cmp    eax, 10000h
        je     SHORT loc_1_1A50
        cmp    eax, 20000h
        je     NEAR PTR loc_1_1BA0
        cmp    eax, 1010000h
        je     NEAR PTR loc_1_1C26
        cmp    eax, 1020000h
        je     NEAR PTR loc_1_1D0B
        jmp    NEAR PTR loc_1_1D4D
loc_1_1A50:
        sub    edx, 10101h
        cmp    edx, 12h
        ja     NEAR PTR loc_1_1D4D
        db  0FFh, 024h, 095h, 080h, 01Dh, 000h, 000h
loc_1_1A66:
        mov    ebx, dword ptr [ebp - 10h]
        lea    esi, [edi*4]
        jmp    NEAR PTR loc_1_1D4D
loc_1_1A75:
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_1D4D
loc_1_1A81:
        mov    dword ptr [ebp - 8], 0
        or     cl, cl
        je     NEAR PTR loc_1_1D4D
        mov    edi, dword ptr [ebp + 8]
        cmp    dword ptr [edi + 55h], 0
        jne    NEAR PTR loc_1_1D4D
        sub    esi, esi
        jmp    NEAR PTR loc_1_1D4D
loc_1_1AA4:
        mov    dword ptr [ebp - 8], 0d0h
        jmp    NEAR PTR loc_1_1D4D
loc_1_1AB0:
        mov    dword ptr [ebp - 8], 5dch
        jmp    NEAR PTR loc_1_1D4D
        db  08Dh, 064h, 024h, 000h
loc_1_1AC0:
        mov    dword ptr [ebp - 8], 186a0h
        jmp    NEAR PTR loc_1_1D4D
        db  08Dh, 064h, 024h, 000h
loc_1_1AD0:
        mov    dword ptr [ebp - 8], 5ea0h
        jmp    NEAR PTR loc_1_1D4D
        db  08Dh, 064h, 024h, 000h
loc_1_1AE0:
        mov    dword ptr [ebp - 8], 0bdc0h
        jmp    NEAR PTR loc_1_1D4D
        db  08Dh, 064h, 024h, 000h
loc_1_1AF0:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_1_1D4D
        db  08Dh, 064h, 024h, 000h
loc_1_1B00:
        mov    dword ptr [ebp - 8], 5eeh
        jmp    NEAR PTR loc_1_1D4D
        db  08Dh, 064h, 024h, 000h
loc_1_1B10:
        push   3
        mov    edi, dword ptr [ebp + 8]
        add    edi, 3
        push   edi
        lea    eax, [ebp - 18h]
        push   eax
        call   sub_1_0C9C
        mov    byte ptr [ebp - 15h], 0
        lea    ebx, [ebp - 18h]
        mov    esi, 4
        jmp    NEAR PTR loc_1_1D4D
loc_1_1B33:
        db  0BBh, 008h, 029h, 000h, 000h
        mov    esi, 1dh
        jmp    NEAR PTR loc_1_1D4D
loc_1_1B42:
        or     cl, cl
        je     SHORT loc_1_1B57
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 81h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_1D4D
loc_1_1B57:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_1D4D
loc_1_1B68:
        mov    dword ptr [ebp - 8], 0d0h
        jmp    NEAR PTR loc_1_1D4D
loc_1_1B74:
        mov    word ptr [ebp - 2], 300h
        lea    ebx, [ebp - 2]
        mov    esi, 2
        jmp    NEAR PTR loc_1_1D4D
loc_1_1B87:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_1_1D4D
loc_1_1B93:
        mov    dword ptr [ebp - 8], 0fh
        jmp    NEAR PTR loc_1_1D4D
        db  090h
loc_1_1BA0:
        or     cl, cl
        je     NEAR PTR loc_1_1D4D
        mov    ecx, edx
        and    ecx, 0ffh
        dec    ecx
        and    edx, 0ff00h
        cmp    edx, 100h
        je     SHORT loc_1_1BD0
        cmp    edx, 200h
        je     SHORT loc_1_1BE2
        jmp    NEAR PTR loc_1_1D4D
        db  08Dh, 064h, 024h, 000h
loc_1_1BD0:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + ecx*4 + 0ech]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_1D4D
loc_1_1BE2:
        mov    eax, ecx
        and    eax, 0fffffffeh
        cmp    eax, 0ch
        jae    SHORT loc_1_1C14
        test   cl, 1
        mov    edi, dword ptr [ebp + 8]
        je     SHORT loc_1_1C03
        mov    eax, dword ptr [edi + eax*2 + 130h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_1D4D
loc_1_1C03:
        lea    ebx, [edi + eax*4 + 100h]
        mov    esi, 8
        jmp    NEAR PTR loc_1_1D4D
loc_1_1C14:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + ecx*4 + 118h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_1D4D
loc_1_1C26:
        cmp    edx, 1010101h
        je     SHORT loc_1_1C50
        cmp    edx, 1010102h
        je     SHORT loc_1_1C60
        cmp    edx, 1010103h
        je     SHORT loc_1_1C70
        cmp    edx, 1010104h
        je     NEAR PTR loc_1_1D02
        jmp    NEAR PTR loc_1_1D4D
        db  090h
loc_1_1C50:
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 3
        mov    esi, 6
        jmp    NEAR PTR loc_1_1D4D
loc_1_1C60:
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 9
        mov    esi, 6
        jmp    NEAR PTR loc_1_1D4D
loc_1_1C70:
        or     cl, cl
        je     SHORT loc_1_1CA5
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        mov    edi, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [edi + 75h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        db  0E8h, 030h, 0F7h, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 0ch]
        mov    ebx, dword ptr [ebp + 14h]
        lea    eax, [eax + eax*2]
        lea    esi, [eax*2]
        jmp    NEAR PTR loc_1_1D4D
loc_1_1CA5:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [edi + 75h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        db  0E8h, 038h, 0F6h, 0FEh, 0FFh
        cmp    dword ptr [ebp - 14h], 0
        mov    ebx, dword ptr [ebp + 14h]
        jne    SHORT loc_1_1CE3
        mov    eax, dword ptr [ebp - 0ch]
        lea    eax, [eax + eax*2]
        lea    esi, [eax*2]
        jmp    NEAR PTR loc_1_1D4D
loc_1_1CE3:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [edi + 75h]
        push   eax
        db  0E8h, 0CDh, 0F5h, 0FEh, 0FFh
        lea    eax, [eax + eax*2]
        lea    esi, [eax*2]
        jmp    NEAR PTR loc_1_1D4D
loc_1_1D02:
        mov    dword ptr [ebp - 8], 10h
        jmp    SHORT loc_1_1D4D
loc_1_1D0B:
        or     cl, cl
        je     SHORT loc_1_1D4D
        mov    eax, edx
        and    eax, 0ffh
        dec    eax
        and    edx, 0ff00h
        cmp    edx, 100h
        je     SHORT loc_1_1D30
        cmp    edx, 200h
        je     SHORT loc_1_1D40
        jmp    SHORT loc_1_1D4D
        db  090h
loc_1_1D30:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + eax*4 + 150h]
        mov    dword ptr [ebp - 8], eax
        jmp    SHORT loc_1_1D4D
        db  090h
loc_1_1D40:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + eax*4 + 15ch]
        mov    dword ptr [ebp - 8], eax
loc_1_1D4D:
        cmp    dword ptr [ebp + 18h], esi
        jae    SHORT loc_1_1D65
        mov    eax, dword ptr [ebp + 20h]
        pop    edi
        mov    dword ptr [eax], esi
        mov    eax, 0c0010016h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
loc_1_1D65:
        push   esi
        push   ebx
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        call   sub_1_0C9C
        mov    eax, dword ptr [ebp + 1ch]
        pop    edi
        mov    dword ptr [eax], esi
        sub    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  066h, 01Ah, 000h, 000h, 075h, 01Ah, 000h, 000h, 081h, 01Ah, 000h, 000h, 081h, 01Ah, 000h, 000h
        db  0A4h, 01Ah, 000h, 000h, 0B0h, 01Ah, 000h, 000h, 0C0h, 01Ah, 000h, 000h, 0D0h, 01Ah, 000h, 000h
        db  0E0h, 01Ah, 000h, 000h, 0F0h, 01Ah, 000h, 000h, 000h, 01Bh, 000h, 000h, 010h, 01Bh, 000h, 000h
        db  033h, 01Bh, 000h, 000h, 042h, 01Bh, 000h, 000h, 068h, 01Bh, 000h, 000h, 074h, 01Bh, 000h, 000h
        db  087h, 01Bh, 000h, 000h, 04Dh, 01Dh, 000h, 000h, 093h, 01Bh, 000h, 000h, 08Dh, 064h, 024h, 000h
sub_1_1DD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [ebp + 10h]
        cmp    eax, 1010eh
        je     SHORT loc_1_1E10
        cmp    eax, 1010fh
        je     NEAR PTR loc_1_1E81
        cmp    eax, 10112h
        je     NEAR PTR loc_1_1E93
        cmp    eax, 1010103h
        je     NEAR PTR loc_1_1ED4
        mov    eax, 0c0010017h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Bh, 0FFh
loc_1_1E10:
        mov    esi, dword ptr [ebp + 18h]
        cmp    esi, 4
        je     SHORT loc_1_1E24
        mov    eax, 0c0010015h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_1E24:
        push   4
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0C9C
        test   dword ptr [ebp - 4], 0f050h
        je     SHORT loc_1_1E5A
        mov    ecx, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ecx], 4
        mov    dword ptr [eax], 0
        mov    eax, 0c00000bbh
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_1E5A:
        push   1
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   0
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 75h]
        push   eax
        db  0E8h, 0CBh, 0F3h, 0FEh, 0FFh
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_1E81:
        mov    ecx, dword ptr [ebp + 1ch]
        sub    eax, eax
        mov    dword ptr [ecx], 4
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_1E93:
        cmp    dword ptr [ebp + 18h], 4
        je     SHORT loc_1_1EB0
        mov    eax, dword ptr [ebp + 20h]
        pop    esi
        mov    dword ptr [eax], 4
        mov    eax, 0c0010014h
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Bh, 0FFh
loc_1_1EB0:
        push   4
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 41h
        push   eax
        call   sub_1_0C9C
        mov    ecx, dword ptr [ebp + 1ch]
        sub    eax, eax
        mov    dword ptr [ecx], 4
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_1ED4:
        mov    ecx, 6
        mov    esi, dword ptr [ebp + 18h]
        mov    eax, esi
        cdq
        idiv   ecx
        or     edx, edx
        je     SHORT loc_1_1EF1
        mov    eax, 0c0010015h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_1EF1:
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    ecx, 6
        mov    eax, esi
        cdq
        idiv   ecx
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   0
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 75h]
        push   eax
        db  0E8h, 049h, 0F0h, 0FEh, 0FFh
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1F30:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [eax + 8]
        cmp    byte ptr [ecx + 0e8h], 0
        je     SHORT loc_1_1F4B
        mov    eax, 0c001000dh
        pop    ebp
        ret    14h
loc_1_1F4B:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   ecx
        call   sub_1_1F60
        pop    ebp
        ret    14h
        db  08Dh, 049h, 000h
sub_1_1F60:
        push   ebp
        mov    ebp, esp
        sub    esp, 68h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 8]
        test   edi, 20h
        mov    eax, dword ptr [esi + 0c8h]
        je     SHORT loc_1_1F84
        mov    word ptr [eax], 8000h
        jmp    SHORT loc_1_1FED
loc_1_1F84:
        mov    word ptr [eax], 0
        test   edi, 4
        je     SHORT loc_1_1FC0
        sub    ax, ax
        mov    ecx, 0ffh
        lea    esp, [esp]
loc_1_1FA0:
        mov    edx, eax
        and    edx, 0ffffh
        mov    ebx, dword ptr [esi + 0c8h]
        mov    byte ptr [edx + ebx + 8], cl
        inc    ax
        cmp    ax, 8
        jb     SHORT loc_1_1FA0
        jmp    SHORT loc_1_1FED
        db  08Dh, 064h, 024h, 000h
loc_1_1FC0:
        test   edi, 2
        je     SHORT loc_1_1FED
        lea    eax, [ebp - 68h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        push   60h
        mov    eax, dword ptr [esi + 75h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 0E1h, 0F3h, 0FEh, 0FFh
        lea    eax, [ebp - 68h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   esi
        call   sub_1_2060
loc_1_1FED:
        push   esi
        call   0
        mov    dword ptr [esi + 81h], edi
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2010:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 18h]
        mov    esi, dword ptr [eax + 8]
        cmp    byte ptr [esi + 0e8h], 0
        je     SHORT loc_1_2030
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    1ch
        db  08Dh, 049h, 000h
loc_1_2030:
        mov    eax, dword ptr [ebp + 10h]
        cmp    eax, 10h
        jbe    SHORT loc_1_2042
        mov    eax, 0c0010009h
        pop    esi
        pop    ebp
        ret    1ch
loc_1_2042:
        mov    ecx, dword ptr [ebp + 14h]
        push   ecx
        push   eax
        push   esi
        call   sub_1_2060
        push   esi
        call   0
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    1ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2060:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    eax, eax
        sub    edx, edx
        lea    ecx, [ecx]
loc_1_2070:
        mov    ecx, dword ptr [ebp + 8]
        inc    eax
        mov    ecx, dword ptr [ecx + 0c8h]
        mov    byte ptr [eax + ecx + 7], dl
        cmp    eax, 8
        jb     SHORT loc_1_2070
        mov    eax, dword ptr [ebp + 0ch]
        or     eax, eax
        je     SHORT loc_1_20D3
        mov    esi, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 4], eax
loc_1_2090:
        sub    bl, bl
        push   esi
        push   6
        call   sub_1_20E0
        sub    ecx, ecx
loc_1_209C:
        mov    dl, al
        shr    dl, cl
        and    dl, 1
        inc    ecx
        lea    ebx, [edx + ebx*2]
        cmp    ecx, 6
        jb     SHORT loc_1_209C
        mov    al, bl
        shr    al, 3
        xor    edx, edx
        mov    ecx, dword ptr [ebp + 8]
        mov    dl, al
        mov    edi, dword ptr [ecx + 0c8h]
        mov    al, 1
        mov    cl, bl
        and    cl, 7
        shl    al, cl
        or     byte ptr [edx + edi + 8], al
        add    esi, 6
        dec    dword ptr [ebp - 4]
        jne    SHORT loc_1_2090
loc_1_20D3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h
sub_1_20E0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, 0ffffffffh
        cmp    dword ptr [ebp + 8], 0
        je     SHORT loc_1_2122
        mov    ecx, dword ptr [ebp + 0ch]
loc_1_20F4:
        mov    dl, byte ptr [ecx]
        inc    ecx
        mov    esi, 8
loc_1_2100:
        mov    edi, eax
        shr    edi, 1fh
        add    eax, eax
        mov    bl, dl
        and    ebx, 1
        cmp    ebx, edi
        je     SHORT loc_1_2118
        xor    eax, 4c11db6h
        or     eax, 1
loc_1_2118:
        shr    dl, 1
        dec    esi
        jne    SHORT loc_1_2100
        dec    dword ptr [ebp + 8]
        jne    SHORT loc_1_20F4
loc_1_2122:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2130:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        dec    dword ptr [eax + 14h]
        ret    4
        db  08Dh, 049h, 000h
loc_1_2140:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 103h
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [esi + 49h]
        cmp    byte ptr [esi + 0e8h], 0
        jne    NEAR PTR loc_1_2220
        cmp    byte ptr [edi + 40h], 0
        jne    NEAR PTR loc_1_2210
        inc    dword ptr [edi + 14h]
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_0BE0
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_2200
        cmp    dword ptr [ebp - 8], 5eah
        ja     SHORT loc_1_2200
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [ebx + 2ch], ax
        mov    dword ptr [ebx + 28h], edi
        mov    eax, dword ptr [ebp + 0ch]
        add    ebx, 24h
        push   eax
        push   edi
        push   edi
        call   sub_1_2290
        cmp    byte ptr [ebx + 0ah], 8
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   esi
        jne    SHORT loc_1_21F0
        call   sub_1_0E50
        inc    dword ptr [esi + 0ech]
        lea    edx, [esi + 100h]
        xor    ecx, ecx
        inc    dword ptr [esi + 130h]
        mov    cx, word ptr [ebx + 8]
        add    ecx, dword ptr [edx]
        mov    dword ptr [edx], ecx
        xor    eax, eax
        mov    ax, word ptr [ebx + 8]
        cmp    eax, ecx
        jbe    SHORT loc_1_220A
        inc    dword ptr [edx + 4]
        dec    dword ptr [edi + 14h]
        jmp    SHORT loc_1_2227
        db  08Bh, 0FFh
loc_1_21F0:
        call   sub_1_2700
        dec    dword ptr [edi + 14h]
        jmp    SHORT loc_1_2227
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2200:
        mov    dword ptr [ebp - 4], 0c001000fh
        dec    dword ptr [edi + 14h]
loc_1_220A:
        dec    dword ptr [edi + 14h]
        jmp    SHORT loc_1_2227
        db  090h
loc_1_2210:
        mov    dword ptr [ebp - 4], 0c0010002h
        jmp    SHORT loc_1_2227
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2220:
        mov    dword ptr [ebp - 4], 0c001000dh
loc_1_2227:
        cmp    byte ptr [esi + 11h], 0
        jne    SHORT loc_1_2273
        cmp    dword ptr [esi + 49h], 2
        jne    SHORT loc_1_223C
        cmp    byte ptr [esi + 0e8h], 0
        jne    SHORT loc_1_2258
loc_1_223C:
        cmp    dword ptr [esi + 0dch], 0
        jne    SHORT loc_1_2258
        cmp    dword ptr [esi + 0d0h], 0
        jne    SHORT loc_1_2258
        mov    eax, dword ptr [esi + 59h]
        sub    eax, esi
        cmp    eax, 59h
        je     SHORT loc_1_2273
loc_1_2258:
        mov    byte ptr [esi + 11h], 1
        push   0
        add    esi, 12h
        push   esi
        call   sub_1_0B92
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_2273:
        dec    dword ptr [esi + 49h]
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2290:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 8]
        lea    esi, [edi + 24h]
        mov    ebx, dword ptr [eax + 8]
        mov    byte ptr [esi + 0ah], 0
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        push   6
        push   0
        push   edi
        call   sub_1_1370
        mov    eax, dword ptr [ebp + 0ch]
        test   byte ptr [eax + 41h], 2
        jne    SHORT loc_1_22D9
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebx + 75h]
        push   eax
        call   sub_1_09E0
        or     al, al
        je     SHORT loc_1_22D9
        or     byte ptr [esi + 0ah], 8
loc_1_22D9:
        test   byte ptr [ebp - 10h], 1
        je     SHORT loc_1_2310
        cmp    dword ptr [ebp - 0eh], -1
        jne    SHORT loc_1_2300
        cmp    byte ptr [ebp - 10h], 0ffh
        jne    SHORT loc_1_2300
        cmp    byte ptr [ebp - 0fh], 0ffh
        jne    SHORT loc_1_2300
        or     byte ptr [esi + 0ah], 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
loc_1_2300:
        or     byte ptr [esi + 0ah], 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h
loc_1_2310:
        or     byte ptr [esi + 0ah], 1
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        push   6
        push   6
        push   edi
        call   sub_1_1370
        mov    eax, dword ptr [ebp - 16h]
        cmp    dword ptr [ebp - 0eh], eax
        jae    SHORT loc_1_2337
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_2378
loc_1_2337:
        mov    eax, dword ptr [ebp - 16h]
        cmp    dword ptr [ebp - 0eh], eax
        jbe    SHORT loc_1_2348
        mov    dword ptr [ebp - 4], 0ffffffffh
        jmp    SHORT loc_1_2378
loc_1_2348:
        mov    ax, word ptr [ebp - 18h]
        cmp    word ptr [ebp - 10h], ax
        jae    SHORT loc_1_2360
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_2378
        db  005h, 000h, 000h, 000h, 000h
loc_1_2360:
        mov    ax, word ptr [ebp - 18h]
        cmp    word ptr [ebp - 10h], ax
        mov    dword ptr [ebp - 4], 0ffffffffh
        ja     SHORT loc_1_2378
        mov    dword ptr [ebp - 4], 0
loc_1_2378:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_2382
        mov    byte ptr [esi + 0ah], 8
loc_1_2382:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h
sub_1_2390:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 24h
        mov    esi, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 0a1h]
        sub    ebx, ebx
        mov    eax, dword ptr [esi + 99h]
        mov    dword ptr [ebp - 14h], ecx
        lea    ecx, [eax + ecx*8]
        mov    dword ptr [ebp - 18h], ecx
        mov    ecx, dword ptr [ebp - 14h]
        mov    eax, dword ptr [esi + 91h]
        lea    edi, [ecx + ecx*4]
        lea    edi, [ecx + edi*4]
        lea    edi, [edi + edi*8]
        lea    edi, [ecx + edi*4]
        mov    ecx, dword ptr [ebp - 18h]
        lea    edi, [eax + edi*2]
        mov    ax, word ptr [ecx + 2]
        and    ax, 0ff00h
        mov    cx, word ptr [ecx + 6]
        mov    word ptr [ebp - 2], cx
        mov    cl, byte ptr [esi + 0a1h]
        inc    cl
        mov    byte ptr [esi + 0a1h], cl
        and    ecx, 0ffh
        cmp    dword ptr [esi + 8dh], ecx
        ja     SHORT loc_1_2408
        mov    byte ptr [esi + 0a1h], bl
loc_1_2408:
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [esi + ecx + 0a2h], 0
        mov    byte ptr [esi + 10h], 1
        test   ah, 44h
        je     SHORT loc_1_2490
        inc    dword ptr [esi + 0f4h]
        test   ah, 4
        je     SHORT loc_1_242D
        inc    dword ptr [esi + 15ch]
loc_1_242D:
        test   ah, 40h
        je     NEAR PTR loc_1_255F
        mov    ax, word ptr [ebp - 2]
        and    ax, 4000h
        je     SHORT loc_1_2446
        inc    dword ptr [esi + 168h]
loc_1_2446:
        test   byte ptr [ebp - 1], 8
        je     SHORT loc_1_2452
        inc    dword ptr [esi + 170h]
loc_1_2452:
        test   byte ptr [ebp - 1], 10h
        je     SHORT loc_1_245E
        inc    dword ptr [esi + 174h]
loc_1_245E:
        test   byte ptr [ebp - 1], 4
        je     SHORT loc_1_246A
        inc    dword ptr [esi + 160h]
loc_1_246A:
        or     ax, ax
        jne    SHORT loc_1_2479
        test   byte ptr [ebp - 1], 80h
        je     NEAR PTR loc_1_255F
loc_1_2479:
        push   esi
        call   0
        mov    byte ptr [esi + 10h], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_1_2490:
        inc    dword ptr [esi + 0ech]
        test   ah, 8
        je     SHORT loc_1_24A1
        inc    dword ptr [esi + 154h]
loc_1_24A1:
        test   ah, 10h
        je     SHORT loc_1_24AC
        inc    dword ptr [esi + 158h]
loc_1_24AC:
        mov    al, byte ptr [edx + 0ah]
        and    eax, 0f7h
        cmp    eax, 1
        je     SHORT loc_1_24C8
        cmp    eax, 2
        je     SHORT loc_1_2500
        cmp    eax, 4
        je     SHORT loc_1_2530
        jmp    NEAR PTR loc_1_255F
loc_1_24C8:
        xor    eax, eax
        inc    dword ptr [esi + 130h]
        mov    ax, word ptr [edx + 8]
        add    eax, dword ptr [esi + 100h]
        mov    dword ptr [esi + 100h], eax
        lea    ecx, [esi + 100h]
        mov    dx, word ptr [edx + 8]
        and    edx, 0ffffh
        cmp    edx, eax
        jbe    SHORT loc_1_255F
        inc    dword ptr [ecx + 4]
        jmp    SHORT loc_1_255F
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2500:
        xor    ecx, ecx
        inc    dword ptr [esi + 134h]
        mov    cx, word ptr [edx + 8]
        add    ecx, dword ptr [esi + 108h]
        mov    dword ptr [esi + 108h], ecx
        lea    eax, [esi + 108h]
        mov    dx, word ptr [edx + 8]
        and    edx, 0ffffh
        cmp    edx, ecx
        jbe    SHORT loc_1_255F
        jmp    SHORT loc_1_255C
        db  08Bh, 0FFh
loc_1_2530:
        xor    ecx, ecx
        inc    dword ptr [esi + 138h]
        mov    cx, word ptr [edx + 8]
        add    ecx, dword ptr [esi + 110h]
        mov    dword ptr [esi + 110h], ecx
        lea    eax, [esi + 110h]
        mov    dx, word ptr [edx + 8]
        and    edx, 0ffffh
        cmp    edx, ecx
        jbe    SHORT loc_1_255F
loc_1_255C:
        inc    dword ptr [eax + 4]
loc_1_255F:
        push   0
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_0BE0
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_25B0
loc_1_2578:
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0C04
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        push   edi
        call   sub_1_0C9C
        add    edi, dword ptr [ebp - 0ch]
        add    ebx, dword ptr [ebp - 0ch]
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0C28
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_2578
loc_1_25B0:
        cmp    ebx, 3ch
        jge    SHORT loc_1_25D7
        mov    edx, 3ch
        sub    edx, ebx
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        mov    ecx, dword ptr [ebp - 18h]
        mov    word ptr [ecx + 4], 0ffc4h
        jmp    SHORT loc_1_25E1
loc_1_25D7:
        mov    ecx, dword ptr [ebp - 18h]
        neg    bx
        mov    word ptr [ecx + 4], bx
loc_1_25E1:
        mov    ecx, dword ptr [ebp - 14h]
        cmp    byte ptr [esi + ecx + 0a2h], 0
        je     SHORT loc_1_2600
        mov    byte ptr [esi + 10h], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_2600:
        mov    ecx, dword ptr [ebp - 18h]
        and    word ptr [ecx + 2], 0a3ffh
        mov    word ptr [ecx + 6], 0
        or     word ptr [ecx + 2], 8000h
        sub    ax, ax
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [esi + ecx + 0a2h], 1
        mov    cx, word ptr [esi + 45h]
        mov    byte ptr [esi + 10h], 0
        lea    edx, [ecx + 12h]
        out    dx, ax
        lea    edx, [ecx + 10h]
        mov    ax, 48h
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2650:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 10h], 0
        je     SHORT loc_1_2670
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_2670:
        xor    eax, eax
        mov    ecx, dword ptr [esi + 99h]
        mov    al, byte ptr [esi + 0a1h]
        test   byte ptr [ecx + eax*8 + 3], 80h
        jne    SHORT loc_1_26EB
        cmp    byte ptr [eax + esi + 0a2h], 0
        je     SHORT loc_1_26EB
        mov    edi, dword ptr [esi + 0dch]
        lea    ebx, [edi + 24h]
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_1_26A7
        mov    dword ptr [esi + 0e0h], 0
loc_1_26A7:
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi + 0dch], eax
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ebp - 4], eax
        push   edi
        push   esi
        call   sub_1_2390
        test   byte ptr [ebx + 0ah], 8
        je     SHORT loc_1_26CB
        push   edi
        push   esi
        call   sub_1_0E50
        jmp    SHORT loc_1_26DA
loc_1_26CB:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   edi
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_0AFA
loc_1_26DA:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        dec    dword ptr [eax + 14h]
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_26EB:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2700:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [edx + 28h]
        lea    esi, [edx + 24h]
        inc    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0dch], 0
        jne    SHORT loc_1_2724
        mov    dword ptr [eax + 0dch], edx
        jmp    SHORT loc_1_272D
loc_1_2724:
        mov    ecx, dword ptr [eax + 0e0h]
        mov    dword ptr [ecx + 24h], edx
loc_1_272D:
        mov    dword ptr [esi], 0
        mov    dword ptr [eax + 0e0h], edx
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_2740:
        inc    ecx
        insd
        xor    dword ptr [543030h], esi
loc_1_2748:
        or     byte ptr [eax], al
        or     byte ptr [eax], al
loc_1_274C:
        inc    eax
        daa
        add    byte ptr [eax], al
        pop    esp
        inc    esp
        jbe    NEAR PTR loc_1_27BE
        arpl   word ptr [ebp + 5ch], sp
        inc    ecx
        insd
        xor    dword ptr [30543030h], esi
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_2764:
        pop    esp
        inc    esp
        db  065h, 076h, 069h
        arpl   word ptr [ebp + 5ch], sp
        inc    ecx
        insd
        xor    dword ptr [30543030h], esi
        xor    dword ptr [eax], eax
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
        dec    ecx
        dec    edi
        inc    ecx
        db  064h, 064h, 072h, 065h
        db  073h, 073h
        add    byte ptr [eax], al
        add    byte ptr [ecx + 4fh], cl
        inc    ecx
        db  064h, 064h, 072h, 065h
        db  073h, 073h
        add    byte ptr [eax], al
        add    byte ptr [ecx + 6eh], cl
        db  074h, 065h
        db  072h, 072h
        db  075h, 070h
        je     SHORT loc_1_27B2
loc_1_27B2:
        add    byte ptr [eax], al
loc_1_27B4:
        dec    ecx
        outsb
        je     SHORT loc_1_281D
        db  072h, 072h
        db  075h, 070h
        je     SHORT loc_1_27BE
loc_1_27BE:
        add    byte ptr [eax], al
        inc    esp
        insd
        popad
        inc    ebx
        push   656e6e61h
        insb
        add    byte ptr [eax], al
loc_1_27CC:
        inc    esp
        insd
        popad
        inc    ebx
        push   656e6e61h
        insb
        add    byte ptr [eax], al
loc_1_27D8:
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
        or     eax, 0e000101h
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [eax], dl
loc_1_281D:
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
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
        add    al, byte ptr [edx]
        add    byte ptr [edx], al
        add    al, byte ptr [edx]
        add    byte ptr [ebx], al
        add    al, byte ptr [edx]
        add    byte ptr [edx + eax], al
        add    al, byte ptr [eax]
        add    eax, 6000202h
        add    al, byte ptr [edx]
        add    byte ptr [edi], al
        add    al, byte ptr [edx]
        add    byte ptr [eax], cl
        add    al, byte ptr [edx]
        add    byte ptr [ecx], cl
        add    al, byte ptr [edx]
        add    byte ptr [edx], cl
        add    al, byte ptr [edx]
        add    byte ptr [ebx], cl
        add    al, byte ptr [edx]
        add    byte ptr [edx + eax], cl
        add    al, byte ptr [eax]
        or     eax, 0e000202h
        add    al, byte ptr [edx]
        add    byte ptr [ecx], al
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
        add    dword ptr [edx], eax
        add    al, byte ptr [ecx]
        add    al, byte ptr [edx]
        add    al, byte ptr [ecx]
        add    eax, dword ptr [edx]
        add    al, byte ptr [ecx]
        add    al, 2
        add    al, byte ptr [ecx]
        add    eax, 6010202h
        add    al, byte ptr [edx]
        add    dword ptr [edi], eax
        add    al, byte ptr [edx]
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
        or     eax, 0e000101h
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [eax], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    al, byte ptr [edx]
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
loc_1_2908:
        inc    ecx
        dec    ebp
        inc    esp
        and    byte ptr [ecx + 6dh], al
        xor    dh, byte ptr [ecx]
        xor    byte ptr [eax], dh
        das
        push   eax
        inc    ebx
        dec    esi
        inc    ebp
        push   esp
        sub    eax, 20415349h
        inc    ecx
        popad
        db  070h, 074h
        jb     NEAR PTR loc_1_2925
loc_1_2925:
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
