; elnk3_lcode_01_01.asm -- ELNK3 object 1 segment _LTEXT  (part 1/2)
.386p
_LTEXT segment dword public use32 'LCODE'

loc_1_0000:
        db  0A0h, 068h, 055h, 000h, 000h
        cmp    al, 0
        jne    SHORT loc_1_0025
        db  0FEh, 005h, 068h, 055h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     SHORT loc_1_0027
        push   0
        push   0
        call   sub_1_1DB8
        cmp    eax, 0
        jne    SHORT loc_1_0027
loc_1_0025:
        clc
        ret
loc_1_0027:
        stc
        ret
ELNK3_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_0038
        db  0FFh, 024h, 085h, 03Ah, 000h, 000h, 000h
loc_1_0038:
        clc
        ret
        db  000h, 000h, 000h, 000h, 038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h
        db  038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h
        db  038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h
        db  038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h
        db  038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h
        db  038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h
        db  038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_00A8:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    edx, dword ptr [ecx + 160h]
        in     al, dx
        test   al, 1
        je     SHORT loc_1_00D4
        mov    edx, dword ptr [ecx + 15ch]
        mov    eax, 7000h
        out    dx, ax
        mov    eax, 6801h
        pop    ebp
        out    dx, ax
        mov    al, 1
        ret    4
loc_1_00D4:
        xor    al, al
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_00E8:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 0bbh], 1
        mov    edx, dword ptr [esi + 168h]
        in     al, dx
        mov    di, ax
        and    edi, 0ffff00ffh
loc_1_0107:
        mov    edx, dword ptr [esi + 160h]
        in     ax, dx
        and    eax, 0feh
        je     SHORT loc_1_0123
        push   esi
        and    eax, 0fffffffeh
        call   dword ptr [esi + eax*2 + 234h]
        jmp    SHORT loc_1_0107
loc_1_0123:
        mov    dx, word ptr [esi + 18ah]
        mov    ecx, edi
        add    dx, di
        and    ecx, 0ffffh
        imul   ecx, ecx
        mov    ax, word ptr [esi + 188h]
        add    ecx, dword ptr [esi + 18ch]
        inc    ax
        mov    dword ptr [esi + 18ch], ecx
        mov    word ptr [esi + 18ah], dx
        mov    word ptr [esi + 188h], ax
        cmp    ax, 100h
        jb     SHORT loc_1_017E
        mov    word ptr [esi + 188h], 80h
        shr    ecx, 1
        shr    dx, 1
        mov    dword ptr [esi + 18ch], ecx
        mov    word ptr [esi + 18ah], dx
loc_1_017E:
        mov    byte ptr [esi + 0bbh], 0
        cmp    byte ptr [esi + 0b1h], 0
        je     SHORT loc_1_01B1
        mov    edx, dword ptr [esi + 15ch]
        mov    eax, 70feh
        mov    byte ptr [esi + 0b1h], 0
        out    dx, ax
        mov    eax, dword ptr [esi + 5eh]
        push   eax
        call   sub_1_5160
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_01B1:
        mov    edx, dword ptr [esi + 15ch]
        mov    eax, 70feh
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_01C8:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0b0h], 0
        je     SHORT loc_1_01E5
        mov    byte ptr [esi + 0b0h], 0
        push   esi
        call   sub_1_1A78
loc_1_01E5:
        lea    ecx, [esi + 234h]
        xor    eax, eax
loc_1_01ED:
        test   al, 2
        je     SHORT loc_1_01F9
        db  0C7h, 001h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_0245
loc_1_01F9:
        test   al, 80h
        je     SHORT loc_1_0205
        db  0C7h, 001h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_0245
loc_1_0205:
        test   al, 20h
        je     SHORT loc_1_0211
        db  0C7h, 001h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_0245
loc_1_0211:
        test   al, 10h
        je     SHORT loc_1_021D
        db  0C7h, 001h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_0245
loc_1_021D:
        test   al, 4
        je     SHORT loc_1_0229
        db  0C7h, 001h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_0245
loc_1_0229:
        test   al, 8
        je     SHORT loc_1_0235
        db  0C7h, 001h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_0245
loc_1_0235:
        test   al, 40h
        db  0C7h, 001h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_0245
        db  0C7h, 001h, 000h, 000h, 000h, 000h
loc_1_0245:
        add    ecx, 4
        add    eax, 2
        cmp    eax, 100h
        jl     SHORT loc_1_01ED
        db  0C7h, 086h, 010h, 002h, 000h, 000h, 000h, 000h, 000h, 000h
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0268:
        push   ebp
        mov    eax, 806h
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ecx + 15ch]
        mov    edi, dword ptr [ecx + 158h]
        mov    edx, esi
        out    dx, ax
        mov    eax, 0b000h
        out    dx, ax
        lea    edx, [edi + 1]
        in     al, dx
        xor    edx, edx
        mov    dl, al
        add    dword ptr [ecx + 1d0h], edx
        lea    edx, [edi + 2]
        in     al, dx
        xor    edx, edx
        mov    dl, al
        add    dword ptr [ecx + 1d8h], edx
        lea    edx, [edi + 3]
        in     al, dx
        xor    edx, edx
        mov    dl, al
        add    dword ptr [ecx + 1d4h], edx
        lea    edx, [edi + 5]
        in     al, dx
        xor    edx, edx
        mov    dl, al
        add    dword ptr [ecx + 200h], edx
        lea    edx, [edi + 7]
        in     al, dx
        xor    edx, edx
        mov    dl, al
        add    dword ptr [ecx + 1bch], edx
        lea    edx, [edi + 0ch]
        in     ax, dx
        mov    edx, eax
        and    edx, 0ffffh
        add    dword ptr [ecx + 1c4h], edx
        lea    edx, [edi + 0ah]
        in     ax, dx
        mov    edx, eax
        and    edx, 0ffffh
        add    dword ptr [ecx + 1c8h], edx
        lea    edx, [edi + 6]
        in     al, dx
        and    eax, 0ffh
        add    dword ptr [ecx + 1b8h], eax
        cmp    byte ptr [ecx + 0b2h], 0
        je     SHORT loc_1_0316
        add    dword ptr [ecx + 1d0h], eax
loc_1_0316:
        mov    edx, esi
        mov    eax, 801h
        cmp    dword ptr [ecx + 0c0h], 1
        out    dx, ax
        jne    SHORT loc_1_0344
        mov    edx, dword ptr [ecx + 164h]
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0344
        mov    eax, dword ptr [ecx + 0c4h]
        mov    edx, esi
        and    eax, 0ff7fh
        out    dx, ax
        jmp    SHORT loc_1_0377
loc_1_0344:
        mov    edx, esi
        mov    eax, 806h
        out    dx, ax
        mov    edx, edi
        in     al, dx
        xor    edx, edx
        mov    dl, al
        add    dword ptr [ecx + 1e0h], edx
        lea    edx, [edi + 4]
        in     al, dx
        xor    edx, edx
        mov    dl, al
        add    dword ptr [ecx + 1e4h], edx
        lea    edx, [edi + 8]
        in     al, dx
        and    eax, 0ffh
        add    dword ptr [ecx + 1e8h], eax
loc_1_0377:
        mov    edx, esi
        mov    eax, 0a800h
        out    dx, ax
        mov    eax, 801h
        pop    edi
        out    dx, ax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0398:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        cmp    dword ptr [edi + 0c0h], 1
        ja     SHORT loc_1_03E9
        mov    esi, dword ptr [edi + 15ch]
        mov    eax, 806h
        mov    edx, esi
        mov    ecx, dword ptr [edi + 158h]
        out    dx, ax
        mov    eax, 0b000h
        out    dx, ax
        mov    edx, ecx
        in     al, dx
        lea    edx, [ecx + 4]
        in     al, dx
        lea    edx, [ecx + 8]
        in     al, dx
        mov    edx, esi
        mov    eax, 0a800h
        out    dx, ax
        mov    eax, 801h
        out    dx, ax
        mov    ax, word ptr [edi + 0c4h]
        out    dx, ax
loc_1_03E9:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_03F8:
        push   ebp
        mov    eax, 6840h
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 0b8h], 1
        mov    edx, dword ptr [esi + 15ch]
        cmp    byte ptr [esi + 0b9h], 0
        out    dx, ax
        je     SHORT loc_1_0445
        push   esi
        call   sub_1_1B58
        mov    byte ptr [esi + 0b0h], 0
        mov    byte ptr [esi + 0b9h], 0
        push   esi
        call   sub_1_3FC8
        mov    ax, word ptr [esi + 0c4h]
        mov    edx, dword ptr [esi + 15ch]
        out    dx, ax
loc_1_0445:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0458:
        push   ebp
        mov    eax, 804h
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 15ch]
        mov    edx, edi
        out    dx, ax
        mov    edx, dword ptr [esi + 158h]
        add    edx, 4
        in     ax, dx
        mov    cx, ax
        mov    edx, edi
        test   ch, 4
        mov    eax, 801h
        out    dx, ax
        je     SHORT loc_1_04C8
        push   esi
        call   sub_1_12B8
        mov    ax, word ptr [esi + 19ch]
        mov    edx, dword ptr [esi + 15ch]
        cmp    dword ptr [esi + 0f8h], 0
        out    dx, ax
        je     SHORT loc_1_051C
        mov    eax, dword ptr [esi + 0fch]
        pop    edi
        mov    byte ptr [esi + 110h], 0
        mov    dword ptr [esi + 100h], eax
        pop    esi
        sub    ax, 7000h
        pop    ebp
        out    dx, ax
        ret    4
loc_1_04C8:
        test   ch, 20h
        je     SHORT loc_1_04F2
        cmp    dword ptr [esi + 128h], 2
        je     SHORT loc_1_04F2
        cmp    dword ptr [esi + 0c0h], 1
        jne    SHORT loc_1_04F2
        push   esi
        call   sub_1_1B58
        mov    byte ptr [esi + 0b0h], 0
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_04F2:
        mov    edx, edi
        mov    eax, 2800h
        out    dx, ax
        mov    ecx, dword ptr [esi + 160h]
        mov    edx, ecx
loc_1_0503:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_0503
        mov    edx, edi
        mov    eax, 5800h
        out    dx, ax
        mov    edx, ecx
loc_1_0515:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_0515
loc_1_051C:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0528:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0538:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   0
        mov    ebx, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [esi]
        push   0
        push   0
        push   ebx
        call   sub_1_5400
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 28h], esi
        mov    word ptr [ebx + 2ch], cx
        cmp    ecx, 0eh
        mov    byte ptr [ebx + 2eh], 0
        jb     NEAR PTR loc_1_06B6
        cmp    dword ptr [edi + 13ch], ecx
        jb     NEAR PTR loc_1_06B6
        cmp    byte ptr [esi + 20h], 0
        je     SHORT loc_1_0590
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0590:
        cmp    byte ptr [edi + 83h], 0
        je     SHORT loc_1_05A7
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_05A7:
        inc    dword ptr [esi + 1ch]
        test   byte ptr [edi + 21h], 0ah
        mov    dword ptr [ebx + 24h], 0
        je     SHORT loc_1_05D9
        cmp    dword ptr [edi + 15h], 0
        jne    SHORT loc_1_05C2
        mov    dword ptr [edi + 15h], ebx
        jmp    SHORT loc_1_05C8
loc_1_05C2:
        mov    eax, dword ptr [edi + 11h]
        mov    dword ptr [eax + 24h], ebx
loc_1_05C8:
        mov    eax, 103h
        mov    dword ptr [edi + 11h], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_05D9:
        mov    dword ptr [edi + 15h], ebx
        mov    dword ptr [edi + 11h], ebx
        push   ebx
        or     byte ptr [edi + 21h], 8
        mov    eax, dword ptr [edi + 0c4h]
        push   edi
        and    eax, 0fffffffbh
        mov    dword ptr [edi + 0c4h], eax
        mov    edx, dword ptr [edi + 15ch]
        out    dx, ax
        call   sub_1_0728
        test   eax, eax
        je     SHORT loc_1_062A
        mov    eax, dword ptr [edi + 0c4h]
        or     eax, 4
        mov    dword ptr [edi + 0c4h], eax
        mov    edx, dword ptr [edi + 15ch]
        pop    edi
        out    dx, ax
        mov    eax, 103h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_062A:
        mov    eax, dword ptr [edi + 15h]
        mov    ebx, dword ptr [eax + 24h]
        mov    dword ptr [edi + 15h], ebx
        test   ebx, ebx
        je     SHORT loc_1_0650
loc_1_0637:
        push   ebx
        push   edi
        call   sub_1_0728
        test   eax, eax
        jne    SHORT loc_1_0685
        push   ebx
        push   edi
        call   sub_1_06C8
        mov    ebx, dword ptr [edi + 15h]
        test   ebx, ebx
        jne    SHORT loc_1_0637
loc_1_0650:
        and    byte ptr [edi + 21h], 0f7h
        mov    eax, dword ptr [esi + 1ch]
        dec    eax
        mov    dword ptr [esi + 1ch], eax
        jne    SHORT loc_1_0663
        push   esi
        call   sub_1_3F18
loc_1_0663:
        mov    eax, dword ptr [edi + 0c4h]
        or     eax, 4
        mov    dword ptr [edi + 0c4h], eax
        mov    edx, dword ptr [edi + 15ch]
        pop    edi
        out    dx, ax
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0685:
        mov    eax, dword ptr [esi + 1ch]
        dec    eax
        mov    dword ptr [esi + 1ch], eax
        jne    SHORT loc_1_0694
        push   esi
        call   sub_1_3F18
loc_1_0694:
        mov    eax, dword ptr [edi + 0c4h]
        or     eax, 4
        mov    dword ptr [edi + 0c4h], eax
        mov    edx, dword ptr [edi + 15ch]
        pop    edi
        out    dx, ax
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_06B6:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_06C8:
        push   ebp
        mov    edx, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   esi
        push   edi
        mov    al, byte ptr [edx + 2eh]
        test   al, 2
        jne    SHORT loc_1_06DC
        xor    edi, edi
        jmp    SHORT loc_1_06EE
loc_1_06DC:
        and    al, 8
        cmp    al, 1
        sbb    edi, edi
        and    edi, 0fffefff5h
        sub    edi, 3ffefff4h
loc_1_06EE:
        mov    ecx, dword ptr [ebp + 8]
        push   edi
        push   edx
        mov    eax, dword ptr [ecx + 15h]
        mov    esi, dword ptr [eax + 24h]
        mov    dword ptr [ecx + 15h], esi
        mov    esi, dword ptr [edx + 28h]
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_531A
        mov    eax, dword ptr [esi + 1ch]
        dec    eax
        mov    dword ptr [esi + 1ch], eax
        jne    SHORT loc_1_0717
        push   esi
        call   sub_1_3F18
loc_1_0717:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0728:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 18h
        mov    ecx, dword ptr [eax + 17ch]
        push   ebx
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        push   edi
        mov    dword ptr [ebp - 18h], ecx
        xor    edi, edi
        mov    eax, dword ptr [edx + 8]
        mov    di, word ptr [edx + 2ch]
        lea    ecx, [ebp - 8]
        push   ecx
        lea    edx, [ebp - 0ch]
        push   edx
        mov    dword ptr [ebp - 14h], eax
        push   eax
        call   sub_1_5424
        cmp    dword ptr [ebp - 8], 6
        jl     SHORT loc_1_07B3
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 82h], 0
        jne    SHORT loc_1_0787
        mov    eax, dword ptr [ebp - 0ch]
        test   byte ptr [eax], 1
        jne    SHORT loc_1_0787
        mov    ecx, eax
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 40h]
        mov    eax, 0
        cmp    dword ptr [ecx + 2], edx
        jne    SHORT loc_1_0799
loc_1_0787:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        call   sub_1_40F8
        and    eax, 0ffh
loc_1_0799:
        test   eax, eax
        je     SHORT loc_1_07B3
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        inc    dword ptr [eax + 1c0h]
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1888
loc_1_07B3:
        mov    eax, dword ptr [ebp + 8]
        and    dword ptr [eax + 0c4h], 0fffffffbh
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 15ch]
        mov    ax, word ptr [eax + 0c4h]
        and    eax, 0ffffh
        out    dx, ax
        mov    edx, dword ptr [ecx + 16ch]
        in     ax, dx
        and    eax, 0ffffh
        mov    dword ptr [ebp - 10h], eax
        cmp    eax, 8
        jge    SHORT loc_1_084B
        mov    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 0f8h], ecx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0fch], edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 100h], edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 110h], 0
        mov    ecx, dword ptr [ebp + 8]
        lea    eax, [edi - 7000h]
        mov    edx, dword ptr [ecx + 15ch]
        or     dword ptr [ecx + 0c4h], 4
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    edx, dword ptr [eax + 15ch]
        mov    ax, word ptr [eax + 0c4h]
        mov    esp, ebp
        out    dx, ax
        mov    eax, 1
        pop    ebp
        ret    8
loc_1_084B:
        cli
        mov    edx, dword ptr [ebp - 18h]
        mov    eax, edi
        out    dx, eax
        mov    ebx, edi
        mov    eax, dword ptr [ebp + 8]
        sub    dword ptr [ebp - 10h], 4
        mov    eax, dword ptr [eax + 0cch]
        mov    ecx, dword ptr [ebp - 10h]
        cmp    eax, ecx
        jl     SHORT loc_1_086A
loc_1_0868:
        mov    eax, ecx
loc_1_086A:
        mov    dword ptr [ebp - 4], eax
loc_1_086D:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 4], eax
        jl     SHORT loc_1_08B9
        sub    ebx, eax
        sub    dword ptr [ebp - 4], eax
        push   esi
        mov    esi, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp - 18h]
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd
        and    eax, 3
        mov    ecx, eax
        rep outsb
        pop    esi
        lea    eax, [ebp - 14h]
        mov    ecx, dword ptr [ebp - 14h]
        push   eax
        push   ecx
        call   sub_1_5448
        cmp    dword ptr [ebp - 14h], 0
        je     SHORT loc_1_0918
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 14h]
        push   ecx
        push   edx
        call   sub_1_5424
        jmp    SHORT loc_1_086D
loc_1_08B9:
        push   esi
        mov    esi, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp - 18h]
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd
        and    eax, 3
        mov    ecx, eax
        rep outsb
        pop    esi
        mov    eax, dword ptr [ebp - 4]
        sub    ebx, eax
        sub    dword ptr [ebp - 8], eax
        add    dword ptr [ebp - 0ch], eax
        sti
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 16ch]
        in     ax, dx
        and    eax, 0ffffh
        sub    eax, 4
        mov    dword ptr [ebp - 10h], eax
        cli
        cmp    dword ptr [ebp - 10h], 8
        jl     NEAR PTR loc_1_09EF
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 0cch]
        cmp    eax, ecx
        jl     NEAR PTR loc_1_086A
        jmp    NEAR PTR loc_1_0868
loc_1_0918:
        neg    edi
        and    edi, 3
        mov    dword ptr [ebp - 10h], edi
        je     SHORT loc_1_0931
        mov    eax, esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 10h]
        rep outsb
        mov    esi, eax
loc_1_0931:
        sti
        mov    esi, dword ptr [ebp + 8]
        mov    eax, 804h
        mov    edx, dword ptr [esi + 15ch]
        add    esi, 15ch
        mov    ecx, dword ptr [ebp + 8]
        out    dx, ax
        mov    edx, dword ptr [ecx + 178h]
        in     ax, dx
        and    eax, 0ffffh
        mov    dword ptr [ebp - 10h], eax
        mov    eax, 801h
        mov    edx, dword ptr [esi]
        test   byte ptr [ebp - 0fh], 20h
        out    dx, ax
        je     SHORT loc_1_09C8
        inc    dword ptr [ecx + 1cch]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 164h]
        mov    edx, ecx
loc_1_097B:
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_097B
        mov    edx, ecx
        xor    al, al
        out    dx, al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12B8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0398
        mov    eax, dword ptr [ebp + 8]
        add    eax, 19ch
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1198
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        push   edx
        call   sub_1_0A88
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + 19ch]
        mov    edx, dword ptr [ecx + 15ch]
        out    dx, ax
loc_1_09C8:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        or     dword ptr [eax + 0c4h], 4
        mov    eax, dword ptr [ebp + 8]
        pop    esi
        pop    ebx
        mov    edx, dword ptr [eax + 15ch]
        mov    ax, word ptr [eax + 0c4h]
        mov    esp, ebp
        out    dx, ax
        xor    eax, eax
        pop    ebp
        ret    8
loc_1_09EF:
        sti
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 15ch]
        mov    ax, word ptr [eax + 1aah]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 0f8h], edx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0fch], edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 100h], ebx
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 14h]
        mov    dword ptr [eax + 104h], edx
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 108h], esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [eax + 10ch], edx
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 110h], 1
        mov    ecx, dword ptr [ebp + 8]
        lea    eax, [ebx - 7000h]
        mov    edx, dword ptr [ecx + 15ch]
        or     dword ptr [ecx + 0c4h], 4
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    edx, dword ptr [eax + 15ch]
        mov    ax, word ptr [eax + 0c4h]
        mov    esp, ebp
        out    dx, ax
        mov    eax, 1
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_1_0A88:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 18h
        mov    ecx, dword ptr [eax + 17ch]
        push   ebx
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        push   edi
        mov    dword ptr [ebp - 18h], ecx
        xor    edi, edi
        mov    di, word ptr [edx + 2ch]
        mov    edx, dword ptr [eax + 16ch]
        in     ax, dx
        mov    ecx, eax
        and    ecx, 0ffffh
        mov    dword ptr [ebp - 0ch], ecx
        cmp    ecx, 8
        jge    SHORT loc_1_0B06
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 0f8h], ecx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0fch], edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 100h], edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 110h], 0
        mov    ecx, dword ptr [ebp + 8]
        lea    eax, [edi - 7000h]
        pop    edi
        pop    esi
        mov    edx, dword ptr [ecx + 15ch]
        pop    ebx
        out    dx, ax
        mov    eax, 1
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0B06:
        cli
        mov    edx, dword ptr [ebp - 18h]
        mov    eax, edi
        out    dx, eax
        mov    ebx, edi
        mov    eax, dword ptr [ebp + 8]
        sub    dword ptr [ebp - 0ch], 4
        mov    eax, dword ptr [eax + 0cch]
        mov    ecx, dword ptr [ebp - 0ch]
        cmp    eax, ecx
        jl     SHORT loc_1_0B25
        mov    eax, ecx
loc_1_0B25:
        mov    ecx, dword ptr [ebp + 0ch]
        lea    edx, [ebp - 8]
        push   edx
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ecx + 8]
        lea    ecx, [ebp - 14h]
        push   ecx
        mov    dword ptr [ebp - 10h], eax
        push   eax
loc_1_0B3A:
        call   sub_1_5424
loc_1_0B3F:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 4], eax
        jl     SHORT loc_1_0B86
        sub    ebx, eax
        sub    dword ptr [ebp - 4], eax
        push   esi
        mov    esi, dword ptr [ebp - 14h]
        mov    edx, dword ptr [ebp - 18h]
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd
        and    eax, 3
        mov    ecx, eax
        rep outsb
        pop    esi
        lea    eax, [ebp - 10h]
        mov    ecx, dword ptr [ebp - 10h]
        push   eax
        push   ecx
        call   sub_1_5448
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_0BE6
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 14h]
        push   eax
        mov    edx, dword ptr [ebp - 10h]
        push   ecx
        push   edx
        jmp    SHORT loc_1_0B3A
loc_1_0B86:
        push   esi
        mov    esi, dword ptr [ebp - 14h]
        mov    edx, dword ptr [ebp - 18h]
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd
        and    eax, 3
        mov    ecx, eax
        rep outsb
        pop    esi
        mov    eax, dword ptr [ebp - 4]
        sub    ebx, eax
        sub    dword ptr [ebp - 8], eax
        add    dword ptr [ebp - 14h], eax
        sti
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 16ch]
        in     ax, dx
        and    eax, 0ffffh
        sub    eax, 4
        mov    dword ptr [ebp - 0ch], eax
        cli
        cmp    dword ptr [ebp - 0ch], 8
        mov    eax, dword ptr [ebp + 8]
        jl     NEAR PTR loc_1_0CA1
        mov    eax, dword ptr [eax + 0cch]
        mov    ecx, dword ptr [ebp - 0ch]
        cmp    eax, ecx
        jl     SHORT loc_1_0BDE
        mov    eax, ecx
loc_1_0BDE:
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_0B3F
loc_1_0BE6:
        neg    edi
        and    edi, 3
        mov    dword ptr [ebp - 0ch], edi
        je     SHORT loc_1_0BFF
        mov    eax, esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 0ch]
        rep outsb
        mov    esi, eax
loc_1_0BFF:
        sti
        mov    esi, dword ptr [ebp + 8]
        mov    eax, 804h
        mov    edx, dword ptr [esi + 15ch]
        add    esi, 15ch
        mov    ecx, dword ptr [ebp + 8]
        out    dx, ax
        mov    edx, dword ptr [ecx + 178h]
        in     ax, dx
        and    eax, 0ffffh
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, 801h
        mov    edx, dword ptr [esi]
        test   byte ptr [ebp - 0bh], 20h
        out    dx, ax
        je     SHORT loc_1_0C96
        inc    dword ptr [ecx + 1cch]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 164h]
        mov    edx, ecx
loc_1_0C49:
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0C49
        mov    edx, ecx
        xor    al, al
        out    dx, al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12B8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0398
        mov    eax, dword ptr [ebp + 8]
        add    eax, 19ch
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1198
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        push   edx
        call   sub_1_0A88
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + 19ch]
        mov    edx, dword ptr [ecx + 15ch]
        out    dx, ax
loc_1_0C96:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0CA1:
        mov    edx, dword ptr [eax + 15ch]
        mov    ax, word ptr [eax + 1aah]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 0f8h], edx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0fch], edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 100h], ebx
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 10h]
        mov    dword ptr [eax + 104h], edx
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp - 14h]
        mov    dword ptr [eax + 108h], esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [eax + 10ch], edx
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 110h], 1
        mov    ecx, dword ptr [ebp + 8]
        lea    eax, [ebx - 7000h]
        mov    edx, dword ptr [ecx + 15ch]
        out    dx, ax
        sti
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0D28:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 18h
        mov    ecx, dword ptr [eax + 17ch]
        push   esi
        push   edi
        mov    dword ptr [ebp - 18h], ecx
        mov    ecx, dword ptr [eax + 0f8h]
        cli
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 16ch]
        in     ax, dx
        mov    edx, eax
        mov    eax, dword ptr [ebp + 8]
        and    edx, 0ffffh
        mov    dword ptr [ebp - 10h], edx
        cmp    byte ptr [eax + 110h], 0
        jne    SHORT loc_1_0DB4
        cmp    edx, 8
        jge    SHORT loc_1_0D8D
        mov    edx, dword ptr [eax + 15ch]
        mov    ecx, eax
        mov    eax, dword ptr [eax + 0fch]
        sub    ax, 7000h
        out    dx, ax
        sti
        mov    eax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0D8D:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 0fch]
        out    dx, eax
        lea    edx, [ebp - 8]
        lea    eax, [ebp - 14h]
        sub    dword ptr [ebp - 10h], 4
        mov    ecx, dword ptr [ecx + 8]
        push   edx
        push   eax
        mov    dword ptr [ebp - 0ch], ecx
        push   ecx
        call   sub_1_5424
        jmp    SHORT loc_1_0DD2
loc_1_0DB4:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 104h]
        mov    dword ptr [ebp - 0ch], ecx
        mov    edx, dword ptr [eax + 108h]
        mov    dword ptr [ebp - 14h], edx
        mov    ecx, dword ptr [eax + 10ch]
        mov    dword ptr [ebp - 8], ecx
loc_1_0DD2:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 10h]
        mov    edi, dword ptr [eax + 100h]
        mov    eax, dword ptr [eax + 0cch]
        cmp    eax, ecx
        jl     SHORT loc_1_0DEA
loc_1_0DE8:
        mov    eax, ecx
loc_1_0DEA:
        mov    dword ptr [ebp - 4], eax
loc_1_0DED:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 4], eax
        jl     SHORT loc_1_0E3C
        push   esi
        mov    esi, dword ptr [ebp - 14h]
        mov    edx, dword ptr [ebp - 18h]
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd
        and    eax, 3
        mov    ecx, eax
        rep outsb
        pop    esi
        mov    eax, dword ptr [ebp - 8]
        lea    ecx, [ebp - 0ch]
        sub    edi, eax
        push   ecx
        sub    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_5448
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_1_0E9B
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 14h]
        push   eax
        mov    edx, dword ptr [ebp - 0ch]
        push   ecx
        push   edx
        call   sub_1_5424
        jmp    SHORT loc_1_0DED
loc_1_0E3C:
        push   esi
        mov    esi, dword ptr [ebp - 14h]
        mov    edx, dword ptr [ebp - 18h]
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd
        and    eax, 3
        mov    ecx, eax
        rep outsb
        pop    esi
        mov    eax, dword ptr [ebp - 4]
        sub    edi, eax
        sub    dword ptr [ebp - 8], eax
        add    dword ptr [ebp - 14h], eax
        sti
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 16ch]
        in     ax, dx
        and    eax, 0ffffh
        sub    eax, 4
        mov    dword ptr [ebp - 10h], eax
        cli
        cmp    dword ptr [ebp - 10h], 8
        mov    eax, dword ptr [ebp + 8]
        jl     NEAR PTR loc_1_0F61
        mov    eax, dword ptr [eax + 0cch]
        mov    ecx, dword ptr [ebp - 10h]
        cmp    eax, ecx
        jl     NEAR PTR loc_1_0DEA
        jmp    NEAR PTR loc_1_0DE8
loc_1_0E9B:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0fch]
        neg    eax
        and    eax, 3
        mov    dword ptr [ebp - 10h], eax
        je     SHORT loc_1_0EBD
        mov    eax, esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 10h]
        rep outsb
        mov    esi, eax
loc_1_0EBD:
        sti
        mov    esi, dword ptr [ebp + 8]
        mov    eax, 804h
        mov    edx, dword ptr [esi + 15ch]
        add    esi, 15ch
        mov    ecx, dword ptr [ebp + 8]
        out    dx, ax
        mov    edx, dword ptr [ecx + 178h]
        in     ax, dx
        and    eax, 0ffffh
        mov    dword ptr [ebp - 10h], eax
        mov    eax, 801h
        mov    edx, dword ptr [esi]
        test   byte ptr [ebp - 0fh], 20h
        out    dx, ax
        je     SHORT loc_1_0F57
        inc    dword ptr [ecx + 1cch]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 164h]
        mov    edx, ecx
loc_1_0F07:
        in     al, dx
        test   al, 80h
        je     SHORT loc_1_0F07
        mov    edx, ecx
        xor    al, al
        out    dx, al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12B8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0398
        mov    eax, dword ptr [ebp + 8]
        add    eax, 19ch
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1198
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0f8h]
        push   edx
        push   ecx
        call   sub_1_0A88
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + 19ch]
        mov    edx, dword ptr [ecx + 15ch]
        out    dx, ax
loc_1_0F57:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0F61:
        mov    edx, dword ptr [eax + 15ch]
        mov    ax, word ptr [eax + 1aah]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 100h], edi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 104h], edx
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp - 14h]
        mov    dword ptr [eax + 108h], esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [eax + 10ch], edx
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 110h], 1
        mov    ecx, dword ptr [ebp + 8]
        lea    eax, [edi - 7000h]
        mov    edx, dword ptr [ecx + 15ch]
        out    dx, ax
        sti
        mov    eax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_0FC8:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 164h]
        in     al, dx
        out    dx, al
        test   al, 10h
        je     SHORT loc_1_1048
        push   esi
        inc    dword ptr [esi + 1cch]
        call   sub_1_12B8
        push   esi
        call   sub_1_0398
        cmp    dword ptr [esi + 0f8h], 0
        je     SHORT loc_1_102A
        lea    eax, [esi + 1aah]
        push   eax
        push   esi
        call   sub_1_1198
        mov    eax, dword ptr [esi + 0f8h]
        push   eax
        push   esi
        call   sub_1_0A88
        mov    eax, dword ptr [esi + 0f8h]
        push   eax
        push   esi
        call   sub_1_06C8
        mov    dword ptr [esi + 0f8h], 0
        jmp    SHORT loc_1_1037
loc_1_102A:
        lea    eax, [esi + 19ch]
        push   eax
        push   esi
        call   sub_1_1198
loc_1_1037:
        mov    ax, word ptr [esi + 19ch]
        mov    edx, dword ptr [esi + 15ch]
        out    dx, ax
        jmp    SHORT loc_1_1099
loc_1_1048:
        test   al, 20h
        je     NEAR PTR loc_1_10D0
        push   esi
        inc    dword ptr [esi + 1ech]
        call   sub_1_12B8
        mov    ax, word ptr [esi + 19ch]
        mov    edx, dword ptr [esi + 15ch]
        out    dx, ax
        push   esi
        call   sub_1_0398
        mov    eax, dword ptr [esi + 0f8h]
        test   eax, eax
        je     SHORT loc_1_1099
        push   eax
        push   esi
        call   sub_1_0A88
        mov    eax, dword ptr [esi + 0f8h]
        push   eax
        push   esi
        call   sub_1_06C8
        mov    dword ptr [esi + 0f8h], 0
loc_1_1099:
        mov    edi, dword ptr [esi + 15h]
        test   edi, edi
        jne    SHORT loc_1_10A8
        and    byte ptr [esi + 21h], 0f7h
        test   edi, edi
        je     SHORT loc_1_110C
loc_1_10A8:
        push   edi
        push   esi
        call   sub_1_0A88
        cmp    eax, 1
        je     SHORT loc_1_110C
        push   edi
        push   esi
        call   sub_1_06C8
        mov    edi, dword ptr [esi + 15h]
        test   edi, edi
        jne    SHORT loc_1_10A8
        and    byte ptr [esi + 21h], 0f7h
        test   edi, edi
        jne    SHORT loc_1_10A8
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_10D0:
        test   al, 8
        je     SHORT loc_1_10F3
        push   esi
        inc    dword ptr [esi + 1dch]
        call   sub_1_0398
        mov    edx, dword ptr [esi + 15ch]
        mov    eax, 4800h
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_10F3:
        push   esi
        inc    dword ptr [esi + 1f0h]
        call   sub_1_0398
        mov    edx, dword ptr [esi + 15ch]
        mov    eax, 4800h
        out    dx, ax
loc_1_110C:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1118:
        push   ebp
        mov    eax, 6808h
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        mov    edx, dword ptr [edi + 15ch]
        out    dx, ax
        mov    ax, word ptr [edi + 19ch]
        out    dx, ax
        call   sub_1_0D28
        cmp    eax, 1
        je     SHORT loc_1_1189
        mov    eax, dword ptr [edi + 0f8h]
        push   eax
        push   edi
        call   sub_1_06C8
        mov    dword ptr [edi + 0f8h], 0
        mov    esi, dword ptr [edi + 15h]
        test   esi, esi
        jne    SHORT loc_1_1167
        and    byte ptr [edi + 21h], 0f7h
        test   esi, esi
        je     SHORT loc_1_1189
loc_1_1167:
        push   esi
        push   edi
        call   sub_1_0A88
        cmp    eax, 1
        je     SHORT loc_1_1189
        push   esi
        push   edi
        call   sub_1_06C8
        mov    esi, dword ptr [edi + 15h]
        test   esi, esi
        jne    SHORT loc_1_1167
        and    byte ptr [edi + 21h], 0f7h
        test   esi, esi
        jne    SHORT loc_1_1167
loc_1_1189:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1198:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_1268
        mov    dx, ax
        mov    ecx, dword ptr [ebp + 0ch]
        sub    dx, word ptr [ecx + 8]
        mov    word ptr [ecx + 8], ax
        mov    edi, edx
        mov    eax, dword ptr [esi + 198h]
        and    edi, 0ffffh
        cmp    edi, eax
        jbe    SHORT loc_1_11CA
        mov    dx, ax
loc_1_11CA:
        mov    di, word ptr [ecx + 0ah]
        inc    di
        mov    ax, word ptr [ecx + 0ch]
        add    ax, dx
        mov    word ptr [ecx + 0ah], di
        mov    word ptr [ecx + 0ch], ax
        cmp    di, 4
        jb     SHORT loc_1_11F5
        shr    di, 1
        shr    ax, 1
        mov    word ptr [ecx + 0ah], di
        mov    word ptr [ecx + 0ch], ax
loc_1_11F5:
        mov    ax, word ptr [ecx + 0ch]
        mov    edi, dword ptr [ecx]
        sub    dx, dx
        and    edi, 7ffh
        div    word ptr [ecx + 0ah]
        mov    esi, dword ptr [esi + 194h]
        and    eax, 0ffffh
        mov    ebx, esi
        sub    ebx, eax
        imul   ebx, edi
        mov    eax, ebx
        cdq
        idiv   esi
        mov    esi, eax
        test   esi, esi
        jl     SHORT loc_1_123C
        cmp    esi, edi
        jle    SHORT loc_1_122B
        mov    esi, edi
loc_1_122B:
        add    edi, esi
        cmp    edi, 5eah
        jle    SHORT loc_1_1257
        mov    edi, 5eah
        jmp    SHORT loc_1_1257
loc_1_123C:
        neg    esi
        mov    eax, edi
        cdq
        sub    eax, edx
        sar    eax, 1
        cmp    esi, eax
        jle    SHORT loc_1_124C
        mov    esi, eax
loc_1_124C:
        sub    edi, esi
        mov    eax, dword ptr [ecx + 4]
        cmp    edi, eax
        jge    SHORT loc_1_1257
        mov    edi, eax
loc_1_1257:
        add    edi, 9800h
        mov    dword ptr [ecx], edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_1_1268:
        push   ebp
        mov    eax, 806h
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ecx + 15ch]
        mov    edx, esi
        out    dx, ax
        mov    eax, 0b000h
        out    dx, ax
        mov    edx, dword ptr [ecx + 158h]
        add    edx, 6
        in     al, dx
        and    eax, 0ffh
        mov    edx, esi
        pop    esi
        add    dword ptr [ecx + 1b8h], eax
        mov    eax, 0a800h
        pop    ebp
        out    dx, ax
        mov    eax, 801h
        out    dx, ax
        mov    eax, dword ptr [ecx + 1b8h]
        ret    4
        db  08Dh, 049h, 000h
sub_1_12B8:
        push   ebp
        mov    eax, 5800h
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ecx + 15ch]
        mov    edx, esi
        out    dx, ax
        mov    edx, dword ptr [ecx + 160h]
loc_1_12D4:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_12D4
        mov    edx, esi
        mov    eax, 4800h
        out    dx, ax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_12F8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [eax]
        mov    esi, dword ptr [ebp + 18h]
        push   esi
        mov    dword ptr [esi + 28h], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    word ptr [esi + 2eh], ax
        mov    eax, dword ptr [ebp + 10h]
        add    ax, 0eh
        mov    word ptr [esi + 2ch], ax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   ecx
        call   dword ptr [eax + 0ch]
        test   eax, eax
        je     SHORT loc_1_1336
        cmp    eax, 1
        je     SHORT loc_1_1345
        mov    eax, 0c0000001h
        pop    esi
        pop    ebp
        ret    18h
loc_1_1336:
        mov    ecx, dword ptr [esi + 30h]
        mov    eax, dword ptr [ebp + 1ch]
        pop    esi
        pop    ebp
        mov    dword ptr [eax], ecx
        xor    eax, eax
        ret    18h
loc_1_1345:
        mov    eax, 103h
        pop    esi
        pop    ebp
        ret    18h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1358:
        push   ebp
        mov    ebp, esp
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 17ch]
        mov    ecx, dword ptr [esi + 15ch]
        mov    edx, ecx
        mov    dword ptr [ebp - 14h], eax
        mov    eax, 6820h
        mov    dword ptr [ebp - 2ch], ecx
        cmp    byte ptr [esi + 0b0h], 0
        out    dx, ax
        je     NEAR PTR loc_1_148B
        mov    edx, ecx
        xor    eax, eax
        mov    ax, word ptr [esi + 0d4h]
        mov    ecx, dword ptr [esi + 208h]
        mov    byte ptr [esi + 0b0h], 0
        out    dx, ax
        mov    edx, dword ptr [esi + 170h]
        mov    dword ptr [ebp - 8], ecx
        mov    ecx, dword ptr [esi + 22ch]
        mov    dword ptr [ebp - 24h], edx
        in     ax, dx
        add    ecx, dword ptr [ebp - 8]
        and    eax, 7fch
        mov    ebx, eax
        mov    dword ptr [ebp - 4], ecx
loc_1_13C7:
        add    dword ptr [ebp - 8], ebx
        mov    dword ptr [ebp - 1ch], ebx
        mov    eax, edi
        mov    edi, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 1ch]
        shr    ecx, 2
        rep insd
        mov    edi, eax
        mov    edx, dword ptr [ebp - 24h]
        add    dword ptr [ebp - 4], ebx
        in     ax, dx
        mov    ebx, eax
        and    ebx, 0ffffh
        test   bh, 80h
        je     SHORT loc_1_13FC
        and    ebx, 7fch
        jmp    SHORT loc_1_13C7
loc_1_13FC:
        test   bh, 40h
        je     SHORT loc_1_141D
        and    ebx, 7ffh
        mov    esi, dword ptr [ebp + 8]
        add    ebx, dword ptr [ebp - 8]
        push   ebx
        push   esi
        call   sub_1_19B8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_141D:
        and    ebx, 7ffh
        lea    eax, [ebx + 3]
        mov    dword ptr [ebp - 34h], eax
        mov    eax, edi
        mov    edi, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 34h]
        shr    ecx, 2
        rep insd
        mov    edi, eax
        mov    esi, dword ptr [ebp + 8]
        add    dword ptr [ebp - 8], ebx
        mov    edi, dword ptr [ebp - 8]
        lea    edx, [esi + 204h]
        mov    dword ptr [esi + 20ch], edi
        mov    dword ptr [esi + 208h], edi
        lea    eax, [edi - 0eh]
        mov    ecx, dword ptr [edx]
        push   eax
        sub    edi, 0eh
        push   edi
        lea    ebx, [ecx + 0eh]
        push   ebx
        mov    eax, dword ptr [esi + 5eh]
        mov    byte ptr [esi + 0b1h], 1
        push   0eh
        push   ecx
        push   ecx
        push   edx
        push   eax
        call   sub_1_4F80
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_1A78
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_148B:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0bch]
        mov    ecx, dword ptr [esi + 22ch]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [esi + 170h]
        mov    dword ptr [ebp - 28h], ecx
        mov    edx, eax
        mov    dword ptr [ebp - 4], ecx
        mov    dword ptr [ebp - 20h], eax
        in     ax, dx
        and    eax, 7fch
        cmp    dword ptr [ebp - 8], 0
        mov    ebx, eax
        je     SHORT loc_1_1508
loc_1_14C0:
        mov    eax, dword ptr [ebp - 10h]
        cmp    eax, ebx
        jl     SHORT loc_1_14C9
        mov    eax, ebx
loc_1_14C9:
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 18h], eax
        mov    eax, edi
        mov    edi, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 18h]
        shr    ecx, 2
        rep insd
        mov    edi, eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp - 20h]
        add    dword ptr [ebp - 4], eax
        sub    dword ptr [ebp - 10h], eax
        in     ax, dx
        mov    ebx, eax
        and    ebx, 0ffffh
        test   bh, 80h
        je     SHORT loc_1_1543
        and    ebx, 7fch
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_14C0
loc_1_1508:
        mov    eax, dword ptr [ebp - 8]
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 28h]
        mov    dword ptr [esi + 208h], eax
        mov    cx, word ptr [edx + 0ch]
        xor    eax, eax
        mov    al, cl
        shl    eax, 8
        shr    cx, 8
        mov    edx, ecx
        and    edx, 0ffffh
        add    eax, edx
        cmp    eax, 600h
        jge    NEAR PTR loc_1_15DE
        add    eax, 0eh
        jmp    NEAR PTR loc_1_1611
loc_1_1543:
        test   bh, 40h
        je     SHORT loc_1_1567
        and    ebx, 7ffh
        mov    esi, dword ptr [ebp + 8]
        sub    ebx, dword ptr [ebp - 10h]
        add    ebx, dword ptr [ebp - 8]
        push   ebx
        push   esi
        call   sub_1_19B8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1567:
        and    ebx, 7ffh
        lea    eax, [ebx + 3]
        mov    dword ptr [ebp - 30h], eax
        mov    eax, edi
        mov    edi, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 30h]
        shr    ecx, 2
        rep insd
        mov    edi, eax
        mov    eax, dword ptr [ebp - 10h]
        mov    esi, dword ptr [ebp + 8]
        sub    dword ptr [ebp - 8], eax
        lea    edx, [esi + 204h]
        mov    byte ptr [esi + 0b1h], 1
        add    dword ptr [ebp - 8], ebx
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 208h], ecx
        mov    dword ptr [esi + 20ch], ecx
        lea    eax, [ecx - 0eh]
        mov    esi, dword ptr [edx]
        push   eax
        sub    ecx, 0eh
        push   ecx
        lea    ebx, [esi + 0eh]
        push   ebx
        push   0eh
        push   esi
        push   esi
        push   edx
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 5eh]
        push   eax
        call   sub_1_4F80
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_1A78
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_15DE:
        je     SHORT loc_1_15F3
        cmp    eax, 800h
        je     SHORT loc_1_15F3
        cmp    eax, 8137h
        mov    eax, 5eah
        jne    SHORT loc_1_1611
loc_1_15F3:
        mov    eax, dword ptr [ebp - 28h]
        mov    cx, word ptr [eax + 10h]
        xor    eax, eax
        mov    al, cl
        shr    cx, 8
        mov    edx, ecx
        and    edx, 0ffffh
        shl    eax, 8
        lea    eax, [eax + edx + 0eh]
loc_1_1611:
        cmp    eax, 5eah
        jg     SHORT loc_1_161D
        cmp    dword ptr [ebp - 8], eax
        jle    SHORT loc_1_1622
loc_1_161D:
        mov    eax, 5eah
loc_1_1622:
        mov    ecx, eax
        mov    esi, dword ptr [ebp + 8]
        sub    ecx, dword ptr [ebp - 8]
        sar    ecx, 1
        imul   ecx, dword ptr [esi + 0d0h]
        sar    ecx, 5
        sub    eax, ecx
        sub    eax, dword ptr [esi + 190h]
        jns    SHORT loc_1_1643
        xor    eax, eax
loc_1_1643:
        sub    ax, 7800h
        mov    esi, dword ptr [ebp + 8]
        and    eax, 0ffffh
        mov    edx, dword ptr [ebp - 2ch]
        mov    byte ptr [esi + 0b0h], 1
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        out    dx, ax
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_1668:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 17ch]
        mov    edx, dword ptr [ebx + 170h]
        mov    dword ptr [ebp - 10h], eax
        in     ax, dx
        mov    ecx, eax
        and    ecx, 0ffffh
        test   ch, 40h
        mov    dword ptr [ebp - 4], ecx
        je     SHORT loc_1_16D7
        and    dword ptr [ebp - 4], 7ffh
        cmp    byte ptr [ebx + 0b0h], 0
        je     SHORT loc_1_16AE
        mov    eax, dword ptr [ebx + 208h]
        add    dword ptr [ebp - 4], eax
loc_1_16AE:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   ebx
        call   sub_1_19B8
        mov    ax, word ptr [ebx + 0d4h]
        mov    edx, dword ptr [ebx + 15ch]
        mov    byte ptr [ebx + 0b0h], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        out    dx, ax
        pop    ebp
        ret    4
loc_1_16D7:
        and    dword ptr [ebp - 4], 7ffh
        cmp    byte ptr [ebx + 0b0h], 0
        je     SHORT loc_1_175A
        mov    ax, word ptr [ebx + 0d4h]
        mov    edx, dword ptr [ebx + 15ch]
        mov    byte ptr [ebx + 0b0h], 0
        out    dx, ax
        mov    eax, dword ptr [ebx + 208h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebx + 22ch]
        mov    byte ptr [ebx + 0b1h], 1
        add    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0ch], eax
        add    dword ptr [ebp - 8], ecx
        mov    edx, dword ptr [ebp - 8]
        lea    eax, [ecx + 3]
        mov    dword ptr [ebx + 20ch], edx
        mov    dword ptr [ebx + 208h], edx
        mov    dword ptr [ebp - 18h], eax
        mov    eax, edi
        mov    edi, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 18h]
        shr    ecx, 2
        rep insd
        mov    edi, eax
        lea    edx, [ebx + 204h]
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 0eh
        mov    ecx, dword ptr [edx]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        jmp    SHORT loc_1_17A3
loc_1_175A:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebx + 22ch]
        mov    dword ptr [ebx + 20ch], eax
        mov    dword ptr [ebx + 208h], eax
        add    eax, 3
        mov    dword ptr [ebp - 0ch], ecx
        mov    byte ptr [ebx + 0b1h], 1
        mov    dword ptr [ebp - 14h], eax
        mov    eax, edi
        mov    edi, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 14h]
        shr    ecx, 2
        rep insd
        mov    edi, eax
        lea    edx, [ebx + 204h]
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0eh
        mov    ecx, dword ptr [edx]
        push   eax
        mov    eax, dword ptr [ebp - 4]
loc_1_17A3:
        sub    eax, 0eh
        push   eax
        lea    eax, [ecx + 0eh]
        push   eax
        push   0eh
        push   ecx
        push   ecx
        push   edx
        mov    ecx, dword ptr [ebx + 5eh]
        push   ecx
        call   sub_1_4F80
        push   ebx
        call   sub_1_1A78
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_17C8:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        mov    edi, dword ptr [eax + 22ch]
        mov    cx, word ptr [esi + 2ch]
        mov    eax, dword ptr [ebp + 0ch]
        add    edi, ecx
        mov    ebx, dword ptr [eax + 8]
        sub    ebx, ecx
        xor    eax, eax
        mov    ax, word ptr [esi + 2eh]
        cmp    ebx, eax
        jbe    SHORT loc_1_17F9
        mov    ebx, eax
loc_1_17F9:
        mov    eax, dword ptr [esi + 8]
        lea    ecx, [ebp - 4]
        push   ecx
        lea    edx, [ebp - 0ch]
        push   edx
        mov    dword ptr [esi + 30h], ebx
        push   eax
        mov    dword ptr [ebp - 8], eax
        call   sub_1_5424
        test   ebx, ebx
        je     SHORT loc_1_1879
loc_1_1814:
        cmp    ebx, dword ptr [ebp - 4]
        jbe    SHORT loc_1_1860
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0ch]
        push   eax
        push   edi
        push   ecx
        call   sub_1_54BC
        sub    ebx, dword ptr [ebp - 4]
        add    edi, dword ptr [ebp - 4]
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_5448
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_1876
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_5424
        test   ebx, ebx
        jne    SHORT loc_1_1814
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_1860:
        push   ebx
        mov    eax, dword ptr [ebp - 0ch]
        push   edi
        push   eax
        call   sub_1_54BC
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_1876:
        sub    dword ptr [esi + 30h], ebx
loc_1_1879:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h
sub_1_1888:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        mov    di, word ptr [eax + 2ch]
        mov    eax, dword ptr [esi]
        add    eax, 0fh
        mov    dword ptr [esi + 220h], edi
        and    eax, 0fffffff0h
        cmp    eax, edi
        jae    SHORT loc_1_18B2
        mov    edi, eax
loc_1_18B2:
        mov    eax, dword ptr [ebp + 0ch]
        lea    edx, [ebp - 4]
        push   edx
        mov    dword ptr [esi + 21ch], edi
        mov    ecx, dword ptr [eax + 8]
        lea    eax, [ebp - 0ch]
        push   eax
        mov    dword ptr [ebp - 8], ecx
        push   ecx
        call   sub_1_5424
        mov    ebx, dword ptr [esi + 230h]
loc_1_18D5:
        cmp    edi, dword ptr [ebp - 4]
        jb     SHORT loc_1_1914
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0ch]
        push   eax
        push   ecx
        push   ebx
        call   sub_1_54BC
        sub    edi, dword ptr [ebp - 4]
        add    ebx, dword ptr [ebp - 4]
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_5448
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_191F
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_5424
        jmp    SHORT loc_1_18D5
loc_1_1914:
        push   edi
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        push   ebx
        call   sub_1_54BC
loc_1_191F:
        mov    eax, dword ptr [ebp + 0ch]
        lea    edx, [esi + 218h]
        mov    dword ptr [esi + 228h], eax
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [esi + 220h]
        lea    ebx, [ecx + 0eh]
        sub    eax, 0eh
        push   eax
        mov    eax, dword ptr [esi + 21ch]
        sub    eax, 0eh
        push   eax
        push   ebx
        push   0eh
        push   ecx
        push   ecx
        push   edx
        mov    ecx, dword ptr [esi + 5eh]
        push   ecx
        call   sub_1_4F80
        cmp    byte ptr [esi + 0bbh], 0
        je     SHORT loc_1_196F
        mov    byte ptr [esi + 0b1h], 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_196F:
        mov    eax, dword ptr [esi + 5eh]
        push   eax
        call   sub_1_5160
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1988:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 10h]
        mov    dx, word ptr [ecx + 2ch]
        lea    eax, [ecx + 30h]
        push   eax
        push   edx
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 10h]
        xor    eax, eax
        mov    ax, word ptr [ecx + 2eh]
        push   edx
        push   eax
        push   0
        push   ecx
        call   sub_1_544E
        xor    eax, eax
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_1_19B8:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    edx, dword ptr [ecx + 170h]
        in     ax, dx
        and    eax, 3800h
        cmp    eax, 1800h
        ja     SHORT loc_1_19DE
        je     SHORT loc_1_19F6
        cmp    eax, 800h
        je     SHORT loc_1_19EE
        jmp    SHORT loc_1_1A0C
loc_1_19DE:
        cmp    eax, 2000h
        je     SHORT loc_1_19FE
        cmp    eax, 2800h
        je     SHORT loc_1_1A06
        jmp    SHORT loc_1_1A0C
loc_1_19EE:
        inc    dword ptr [ecx + 1fch]
        jmp    SHORT loc_1_1A0C
loc_1_19F6:
        inc    dword ptr [ecx + 0ech]
        jmp    SHORT loc_1_1A0C
loc_1_19FE:
        inc    dword ptr [ecx + 1f4h]
        jmp    SHORT loc_1_1A0C
loc_1_1A06:
        inc    dword ptr [ecx + 1f8h]
loc_1_1A0C:
        test   eax, eax
        je     SHORT loc_1_1A26
        cmp    eax, 1800h
        je     SHORT loc_1_1A26
        mov    eax, dword ptr [ebp + 0ch]
        dec    dword ptr [ecx + 1bch]
        sub    dword ptr [ecx + 1c8h], eax
loc_1_1A26:
        push   ecx
        call   sub_1_1A78
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_1A38:
        push   ebp
        mov    cl, 0f5h
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [edx + 0bch]
        sub    cl, byte ptr [edx + 190h]
        shr    esi, 1
        mov    eax, dword ptr [edx + 0d8h]
        imul   esi, dword ptr [edx + 0d0h]
        shr    esi, cl
        cmp    esi, eax
        jge    SHORT loc_1_1A65
        mov    esi, eax
loc_1_1A65:
        add    esi, 8800h
        mov    dword ptr [edx + 0d4h], esi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_1A78:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    eax, 4000h
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 15ch]
        mov    edx, esi
        out    dx, ax
        mov    edx, dword ptr [edi + 160h]
loc_1_1A96:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_1A96
        cmp    dword ptr [edi + 0c0h], 1
        ja     NEAR PTR loc_1_1B43
        mov    edx, dword ptr [edi + 170h]
        in     ax, dx
        mov    ecx, eax
        and    ecx, 0ffffh
        test   ch, 80h
        je     NEAR PTR loc_1_1B43
        mov    edx, esi
        mov    eax, 803h
        out    dx, ax
        mov    edx, dword ptr [edi + 174h]
        and    ecx, 7ffh
        in     ax, dx
        mov    ebx, eax
        mov    edx, esi
        and    ebx, 0ffffh
        mov    eax, 801h
        out    dx, ax
        add    ecx, ebx
        add    ecx, 64h
        cmp    ecx, dword ptr [edi + 0dch]
        jae    SHORT loc_1_1B43
        mov    edx, dword ptr [edi + 170h]
        in     ax, dx
        mov    ecx, eax
        mov    edx, esi
        and    ecx, 0ffffh
        mov    eax, 803h
        out    dx, ax
        mov    edx, dword ptr [edi + 174h]
        and    ecx, 7ffh
        in     ax, dx
        mov    ebx, eax
        mov    edx, esi
        and    ebx, 0ffffh
        mov    eax, 801h
        out    dx, ax
        add    ecx, ebx
        add    ecx, 64h
        cmp    ecx, dword ptr [edi + 0dch]
        jae    SHORT loc_1_1B43
        push   edi
        call   sub_1_1B58
loc_1_1B43:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1B58:
        push   ebp
        mov    eax, 2800h
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ecx + 15ch]
        mov    edx, esi
        out    dx, ax
        mov    edx, dword ptr [ecx + 160h]
loc_1_1B74:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_1B74
        mov    ax, word ptr [ecx + 0f0h]
        mov    edx, esi
        out    dx, ax
        mov    eax, 2000h
        pop    esi
        out    dx, ax
        mov    ax, word ptr [ecx + 0d4h]
        pop    ebp
        out    dx, ax
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1BA8:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        add    eax, dword ptr [esi + 0e4h]
        mov    dword ptr [esi + 0e4h], eax
        cmp    eax, 3e8h
        jbe    SHORT loc_1_1BDA
        mov    dword ptr [esi + 0e4h], 0
        push   esi
        call   sub_1_0268
loc_1_1BDA:
        cmp    byte ptr [esi + 0b2h], 0
        jne    SHORT loc_1_1C2C
        mov    eax, dword ptr [esi + 1d0h]
        cmp    eax, 3e8h
        jbe    SHORT loc_1_1C2C
        shl    eax, 2
        cmp    dword ptr [esi + 1b8h], eax
        jae    SHORT loc_1_1C2C
        mov    ecx, dword ptr [esi + 15ch]
        mov    eax, 804h
        mov    byte ptr [esi + 0b2h], 1
        mov    edx, ecx
        out    dx, ax
        mov    edx, dword ptr [esi + 158h]
        add    edx, 0ah
        in     ax, dx
        and    eax, 0fff7h
        out    dx, ax
        mov    edx, ecx
        mov    eax, 801h
        out    dx, ax
loc_1_1C2C:
        mov    cx, word ptr [esi + 188h]
        mov    ax, word ptr [esi + 18ah]
        mov    ebx, ecx
        sub    dx, dx
        and    ebx, 0ffffh
        div    cx
        mov    word ptr [ebp - 4], ax
        sub    edx, edx
        mov    cx, word ptr [ebp - 4]
        mov    eax, dword ptr [esi + 18ch]
        div    ebx
        imul   cx, cx
        sub    ax, cx
        mov    edx, dword ptr [esi + 4ah]
        mov    cx, ax
        shr    ax, 0ch
        mov    edi, eax
        and    edi, 0ffffh
        mov    bl, byte ptr [edx + edi + 144h]
        and    ebx, 0ffff00ffh
        test   bx, bx
        jne    SHORT loc_1_1C90
        mov    bx, cx
        shr    bx, 5
        add    bx, 2
loc_1_1C90:
        mov    ax, cx
        sub    dx, dx
        div    bx
        mov    edi, eax
        mov    eax, ebx
        sub    dx, dx
        and    edi, 0ffffh
        and    eax, 0ffffh
        add    edi, eax
        sar    edi, 1
        mov    ax, cx
        div    di
        mov    ecx, eax
        mov    eax, edi
        and    ecx, 0ffffh
        and    eax, 0ffffh
        add    ecx, eax
        mov    eax, dword ptr [ebp - 4]
        sar    ecx, 1
        and    eax, 0ffffh
        mov    edx, ecx
        and    edx, 0ffffh
        lea    ecx, [eax + edx*2]
        mov    eax, dword ptr [esi + 4]
        shl    ecx, 2
        add    eax, dword ptr [esi + 0e8h]
        mov    dword ptr [esi + 190h], ecx
        mov    dword ptr [esi + 0e8h], eax
        cmp    eax, 3e8h
        jbe    SHORT loc_1_1D20
        mov    dword ptr [esi + 0e8h], 0
        cmp    dword ptr [esi + 0ech], 64h
        jb     SHORT loc_1_1D16
        add    dword ptr [esi + 0d8h], 8
loc_1_1D16:
        mov    dword ptr [esi + 0ech], 0
loc_1_1D20:
        push   esi
        call   sub_1_1A38
        cmp    byte ptr [esi + 0b0h], 0
        jne    SHORT loc_1_1D3E
        mov    ax, word ptr [esi + 0d4h]
        mov    edx, dword ptr [esi + 15ch]
        out    dx, ax
loc_1_1D3E:
        push   esi
        xor    edi, edi
        call   sub_1_1268
        and    eax, 0ffffh
        mov    ebx, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
loc_1_1D51:
        test   edi, edi
        je     SHORT loc_1_1D5C
        cmp    edi, 1
        je     SHORT loc_1_1D64
        jmp    SHORT loc_1_1D6A
loc_1_1D5C:
        lea    ebx, [esi + 19ch]
        jmp    SHORT loc_1_1D6A
loc_1_1D64:
        lea    ebx, [esi + 1aah]
loc_1_1D6A:
        mov    eax, dword ptr [ebp - 4]
        xor    ecx, ecx
        mov    cx, word ptr [ebx + 8]
        sub    eax, ecx
        cmp    eax, dword ptr [esi + 198h]
        jl     SHORT loc_1_1D84
        push   ebx
        push   esi
        call   sub_1_1198
loc_1_1D84:
        inc    edi
        cmp    edi, 2
        jl     SHORT loc_1_1D51
        cmp    dword ptr [esi + 0f8h], 0
        jne    SHORT loc_1_1DA2
        mov    ax, word ptr [esi + 19ch]
        mov    edx, dword ptr [esi + 15ch]
        out    dx, ax
loc_1_1DA2:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1DB8:
        push   ebp
        mov    eax, 6
        mov    ebp, esp
        sub    esp, 0ch
        mov    word ptr [ebp - 0ch], ax
        push   ebx
        mov    word ptr [ebp - 0ah], ax
        push   edi
        db  0BFh, 000h, 000h, 000h, 000h
        xor    eax, eax
        mov    ecx, 55h
        xor    ebx, ebx
        db  0C7h, 045h, 0F8h, 000h, 000h, 000h, 000h
        rep stosd
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 008h, 000h
        db  089h, 01Dh, 004h, 000h, 000h, 000h
        db  089h, 01Dh, 008h, 000h, 000h, 000h
        db  089h, 01Dh, 00Ch, 000h, 000h, 000h
        db  089h, 01Dh, 010h, 000h, 000h, 000h
        db  089h, 01Dh, 014h, 000h, 000h, 000h
        db  089h, 01Dh, 018h, 000h, 000h, 000h
        push   ebx
        db  089h, 01Dh, 01Ch, 000h, 000h, 000h
        push   ebx
        db  089h, 01Dh, 020h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 002h, 000h, 000h, 000h, 004h, 000h
        db  066h, 0C7h, 005h, 024h, 000h, 000h, 000h, 0F7h, 062h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 004h, 000h, 000h, 000h
        call   sub_1_52EA
        mov    edx, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        db  089h, 015h, 03Eh, 000h, 000h, 000h
        db  089h, 00Dh, 042h, 000h, 000h, 000h
        db  089h, 01Dh, 046h, 000h, 000h, 000h
        db  0C6h, 005h, 008h, 000h, 000h, 000h, 003h
        db  088h, 01Dh, 009h, 000h, 000h, 000h
        db  089h, 01Dh, 00Ah, 000h, 000h, 000h
        db  0C7h, 005h, 00Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 012h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 016h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 01Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 01Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 022h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 026h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 02Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 02Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 032h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   36h
        db  068h, 008h, 000h, 000h, 000h
        db  089h, 015h, 036h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp + 0ch]
        db  068h, 000h, 000h, 000h, 000h
        db  089h, 00Dh, 03Ah, 000h, 000h, 000h
        db  0A3h, 052h, 000h, 000h, 000h
        db  089h, 015h, 056h, 000h, 000h, 000h
        db  08Bh, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_52F6
        cmp    dword ptr [ebp - 4], ebx
        je     SHORT loc_1_1F3C
        db  039h, 01Dh, 046h, 000h, 000h, 000h
        jbe    SHORT loc_1_1F24
        xor    eax, eax
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1F24:
        push   0
        db  0A1h, 004h, 000h, 000h, 000h
        push   eax
        call   sub_1_52F0
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1F3C:
        xor    eax, eax
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_1F48:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    ecx, [ebp - 4]
        push   edi
        push   eax
        push   0
        push   462h
        push   ecx
        call   sub_1_53D6
        test   eax, eax
        je     SHORT loc_1_1F77
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_1F77:
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        mov    ecx, 118h
        rep stosd
        stosw
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2438
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2648
        mov    edi, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 0ch]
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   esi
        push   eax
        call   sub_1_2048
        test   eax, eax
        je     SHORT loc_1_1FC9
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   462h
        push   eax
        call   sub_1_53DC
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_1FC9:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_3338
        test   eax, eax
        je     SHORT loc_1_2009
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0aa000004h
        mov    ecx, dword ptr [eax + 3ah]
        push   ecx
        call   sub_1_54A8
        add    esp, 0ch
        mov    ecx, dword ptr [ebp - 4]
        push   0
        push   462h
        push   ecx
        call   sub_1_53DC
        mov    eax, 0c0010006h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_2009:
        push   esi
        mov    eax, dword ptr [ebp - 4]
        push   edi
        push   eax
        call   sub_1_2188
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_1_2034
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   462h
        push   eax
        call   sub_1_53DC
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_2034:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2048:
        push   ebp
        mov    eax, 0fh
        mov    ebp, esp
        sub    esp, 24h
        mov    word ptr [ebp - 24h], ax
        push   esi
        mov    word ptr [ebp - 22h], ax
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, 8
        push   ecx
        mov    word ptr [ebp - 1ch], ax
        lea    edx, [ebp - 8]
        mov    word ptr [ebp - 1ah], ax
        push   edx
        db  0C7h, 045h, 0E0h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0E8h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_5454
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_209A
        mov    eax, 0c0000001h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_209A:
        mov    eax, dword ptr [ebp - 8]
        mov    esi, dword ptr [ebp + 8]
        push   eax
        lea    ecx, [ebp - 14h]
        push   ecx
        lea    edx, [ebp - 10h]
        push   edx
        mov    dword ptr [esi + 8], eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_535C
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_20D1
        lea    edx, [esi + 3eh]
        mov    ecx, dword ptr [ebp - 10h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [edx + 4], al
        mov    cl, byte ptr [ecx + 5]
        mov    byte ptr [edx + 5], cl
loc_1_20D1:
        push   1
        lea    eax, [ebp - 24h]
        push   eax
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        lea    edx, [ebp - 0ch]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_545A
        cmp    dword ptr [ebp - 4], 0
        mov    eax, 8
        jne    SHORT loc_1_20F9
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 4]
loc_1_20F9:
        cmp    eax, 0ffh
        ja     SHORT loc_1_210D
        and    ax, 0ffh
        mov    word ptr [esi + 460h], ax
        jmp    SHORT loc_1_2116
loc_1_210D:
        mov    word ptr [esi + 460h], 8
loc_1_2116:
        push   1
        lea    eax, [ebp - 1ch]
        push   eax
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        lea    edx, [ebp - 0ch]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_545A
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_215A
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4]
        cmp    al, 1
        jne    SHORT loc_1_2141
        xor    eax, eax
        jmp    SHORT loc_1_215F
loc_1_2141:
        cmp    al, 2
        jne    SHORT loc_1_214C
        mov    eax, 1
        jmp    SHORT loc_1_215F
loc_1_214C:
        sub    al, 3
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0fffffffeh
        add    eax, 4
        jmp    SHORT loc_1_215F
loc_1_215A:
        mov    eax, 4
loc_1_215F:
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        push   eax
        push   esi
        call   sub_1_31AE
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_5460
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2188:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        lea    ecx, [ebp - 4]
        push   esi
        push   edi
        push   eax
        push   0
        push   32h
        push   ecx
        call   sub_1_53D6
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     SHORT loc_1_21B9
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_21B9:
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        mov    ecx, 0ch
        rep stosd
        stosw
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 128h]
        test   eax, eax
        je     SHORT loc_1_21F6
        cmp    eax, 1
        jne    SHORT loc_1_21E5
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ah], 2
        jmp    SHORT loc_1_2200
loc_1_21E5:
        cmp    eax, 2
        jne    SHORT loc_1_21F6
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ah], 3
        jmp    SHORT loc_1_2200
loc_1_21F6:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ah], 1
loc_1_2200:
        mov    eax, dword ptr [ebp - 4]
        xor    ecx, ecx
        lea    edx, [esi + 158h]
        mov    edi, 10h
        mov    dword ptr [eax + 16h], 1
loc_1_2217:
        mov    eax, dword ptr [esi + 154h]
        mov    ebx, dword ptr [ebp - 4]
        add    ecx, 0ch
        cmp    ecx, 0ch
        mov    dword ptr [ebx + ecx + 0eh], eax
        mov    ebx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + ecx + 12h], edi
        mov    ebx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + ecx + 16h], edx
        jb     SHORT loc_1_2217
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 10h]
        push   ecx
        db  0A1h, 000h, 000h, 000h, 000h
        push   edx
        lea    ecx, [esi + 3ah]
        push   esi
        push   eax
        push   ecx
        call   sub_1_5302
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    SHORT loc_1_22AF
        push   esi
        call   sub_1_3AB8
        test   eax, eax
        je     SHORT loc_1_2286
        push   0
        mov    eax, dword ptr [esi + 3ah]
        push   0aa000002h
        push   eax
        call   sub_1_54A8
        mov    dword ptr [ebp - 8], 0c000009ah
        add    esp, 0ch
        jmp    NEAR PTR loc_1_23DB
loc_1_2286:
        push   esi
        call   sub_1_27A8
        test   eax, eax
        je     SHORT loc_1_230C
        cmp    eax, 2
        jne    SHORT loc_1_22C8
        push   0
        mov    eax, dword ptr [esi + 3ah]
        push   0aa000007h
        push   eax
        call   sub_1_54A8
        mov    dword ptr [ebp - 8], 0c0010004h
        add    esp, 0ch
loc_1_22AF:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   10h
        push   eax
        call   sub_1_53DC
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_22C8:
        cmp    eax, 3
        jne    SHORT loc_1_22F0
        mov    eax, dword ptr [esi + 154h]
        mov    ecx, dword ptr [esi + 3ah]
        push   eax
        push   1
        push   0aa000001h
        push   ecx
        call   sub_1_54A8
        mov    dword ptr [ebp - 8], 0c001001eh
        add    esp, 10h
        jmp    SHORT loc_1_22AF
loc_1_22F0:
        push   0
        mov    eax, dword ptr [esi + 3ah]
        push   0aa000006h
        push   eax
        call   sub_1_54A8
        mov    dword ptr [ebp - 8], 0c0000001h
        add    esp, 0ch
        jmp    SHORT loc_1_22AF
loc_1_230C:
        lea    ebx, [esi + 22h]
        xor    eax, eax
        mov    edi, ebx
        mov    ecx, 6
        rep stosd
        mov    eax, 2
        mov    ecx, dword ptr [esi + 130h]
        cmp    dword ptr [esi + 12ch], 1
        adc    eax, -1
        push   eax
        mov    al, byte ptr [esi + 124h]
        push   eax
        push   ecx
        push   ecx
        mov    eax, dword ptr [esi + 3ah]
        db  068h, 000h, 000h, 000h, 000h
        lea    edx, [ebp - 8]
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        push   ebx
        push   edx
        call   sub_1_53BE
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_2374
        mov    eax, dword ptr [esi + 130h]
        mov    ecx, dword ptr [esi + 3ah]
        push   eax
        push   1
        push   0aa000005h
        push   ecx
        call   sub_1_54A8
        add    esp, 10h
        jmp    SHORT loc_1_23DB
loc_1_2374:
        push   esi
        call   sub_1_3068
        test   eax, eax
        je     SHORT loc_1_23A8
        mov    eax, dword ptr [esi + 130h]
        mov    ecx, dword ptr [esi + 154h]
        push   eax
        mov    edx, dword ptr [esi + 3ah]
        push   ecx
        push   2
        push   0aa000001h
        push   edx
        call   sub_1_54A8
        mov    dword ptr [ebp - 8], 0c001001eh
        add    esp, 14h
        jmp    SHORT loc_1_23D5
loc_1_23A8:
        lea    eax, [esi + 5eh]
        lea    ecx, [esi + 0dh]
        push   eax
        lea    edx, [esi + 3eh]
        push   ecx
        push   edx
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   10h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_23CA:
        test   al, al
        jne    SHORT loc_1_23EF
        mov    dword ptr [ebp - 8], 0c0000001h
loc_1_23D5:
        push   ebx
        call   sub_1_53C4
loc_1_23DB:
        push   esi
        call   sub_1_3B58
        mov    eax, dword ptr [esi + 3ah]
        push   eax
        call   sub_1_5308
        jmp    NEAR PTR loc_1_22AF
loc_1_23EF:
        xor    ecx, ecx
        mov    dword ptr [esi + 56h], ecx
        mov    dword ptr [esi + 52h], ecx
        mov    dword ptr [esi + 5ah], ecx
        db  0A1h, 04Ah, 000h, 000h, 000h
        mov    dword ptr [esi + 4eh], eax
        push   ecx
        push   32h
        db  089h, 035h, 04Ah, 000h, 000h, 000h
        mov    edx, dword ptr [esi + 4ah]
        mov    dword ptr [esi + 62h], ecx
        db  0C7h, 046h, 066h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 6ah], esi
        inc    dword ptr [edx + 46h]
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        call   sub_1_53DC
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2438:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 19h], ecx
        mov    dword ptr [eax + 1dh], ecx
        mov    dword ptr [eax + 11h], ecx
        mov    dword ptr [eax + 15h], ecx
        mov    dword ptr [eax + 72h], ecx
        mov    dword ptr [eax + 76h], ecx
        mov    dword ptr [eax + 7ah], ecx
        mov    dword ptr [eax + 7eh], ecx
        pop    ebp
        mov    dword ptr [eax], 4ah
        db  0C7h, 040h, 04Ah, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 4], 64h
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h, 024h, 008h, 08Bh, 0ECh, 050h, 080h
        db  048h, 021h, 040h, 0E8h, 0DFh, 01Ch, 000h, 000h, 05Dh, 0C2h, 004h, 000h, 08Dh, 064h, 024h, 000h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2498:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_52FC
        push   0
        db  08Bh, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        call   sub_1_52F0
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_24C8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        db  039h, 035h, 04Ah, 000h, 000h, 000h
        jne    SHORT loc_1_24E6
        mov    eax, dword ptr [esi + 4eh]
        db  0A3h, 04Ah, 000h, 000h, 000h
        jmp    SHORT loc_1_2502
loc_1_24E6:
        db  08Bh, 00Dh, 04Ah, 000h, 000h, 000h
        cmp    dword ptr [ecx + 4eh], esi
        je     SHORT loc_1_24F9
loc_1_24F1:
        mov    ecx, dword ptr [ecx + 4eh]
        cmp    dword ptr [ecx + 4eh], esi
        jne    SHORT loc_1_24F1
loc_1_24F9:
        mov    eax, dword ptr [ecx + 4eh]
        mov    edx, dword ptr [eax + 4eh]
        mov    dword ptr [ecx + 4eh], edx
loc_1_2502:
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 62h]
        push   eax
        push   ecx
        mov    edi, 1
        call   sub_1_53B8
        lea    ecx, [esi + 22h]
        push   ecx
        call   sub_1_53C4
        mov    ebx, dword ptr [esi + 130h]
        mov    edx, ebx
        and    dl, 7
        mov    cl, dl
        shl    edi, cl
        mov    ecx, 7
        cmp    ecx, ebx
        sbb    edx, edx
        xor    ecx, ecx
        and    edx, 80h
        add    edx, 21h
        in     al, dx
        mov    cl, al
        jmp    SHORT loc_1_2545
loc_1_2545:
        jmp    SHORT loc_1_2547
loc_1_2547:
        mov    eax, edi
        or     al, cl
        mov    ecx, dword ptr [esi + 5eh]
        out    dx, al
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_2555:
        push   esi
        call   sub_1_3B58
        push   esi
        call   sub_1_3BA8
        mov    ecx, dword ptr [esi + 3ah]
        push   ecx
        call   sub_1_54B4
        mov    ecx, dword ptr [esi + 3ah]
        push   ecx
        call   sub_1_5308
        push   0
        push   462h
        push   esi
        call   sub_1_53DC
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h
        db  033h, 0D2h, 08Bh, 0ECh, 08Bh, 04Ch, 024h, 008h, 08Dh, 081h, 02Bh, 001h, 000h, 000h, 088h, 051h
        db  05Ah, 083h, 0C1h, 05Ah, 089h, 010h, 089h, 050h, 004h, 089h, 050h, 008h, 089h, 050h, 00Ch, 08Dh
        db  081h, 0E1h, 000h, 000h, 000h, 089h, 010h, 089h, 050h, 004h, 088h, 050h, 008h, 088h, 091h, 0EAh
        db  000h, 000h, 000h, 0C6h, 081h, 0EBh, 000h, 000h, 000h, 040h, 0C6h, 081h, 0ECh, 000h, 000h, 000h
        db  05Bh, 0C6h, 081h, 0EDh, 000h, 000h, 000h, 06Fh, 0C6h, 081h, 0EEh, 000h, 000h, 000h, 080h, 0C6h
        db  081h, 0EFh, 000h, 000h, 000h, 08Fh, 0C6h, 081h, 0F0h, 000h, 000h, 000h, 09Dh, 0C6h, 081h, 0F1h
        db  000h, 000h, 000h, 0A9h, 0C6h, 081h, 0F2h, 000h, 000h, 000h, 0B5h, 0C6h, 081h, 0F3h, 000h, 000h
        db  000h, 0C0h, 0C6h, 081h, 0F4h, 000h, 000h, 000h, 0CAh, 033h, 0C0h, 05Dh, 0C6h, 081h, 0F5h, 000h
        db  000h, 000h, 0D4h, 0C6h, 081h, 0F6h, 000h, 000h, 000h, 0DEh, 0C6h, 081h, 0F7h, 000h, 000h, 000h
        db  0E7h, 0C6h, 081h, 0F8h, 000h, 000h, 000h, 0EFh, 0C6h, 081h, 0F9h, 000h, 000h, 000h, 0F8h, 0C2h
        db  004h, 000h, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2648:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        lea    esi, [edi + 0b0h]
        mov    byte ptr [esi], dl
        mov    byte ptr [esi + 1], dl
        mov    byte ptr [esi + 2], dl
        mov    byte ptr [esi + 3], dl
        mov    byte ptr [esi + 4], dl
        mov    dword ptr [esi + 0ch], 3ch
        mov    dword ptr [esi + 14h], 78feh
        mov    dword ptr [esi + 28h], edx
        mov    dword ptr [esi + 24h], 8800h
        mov    dword ptr [esi + 34h], edx
        mov    dword ptr [esi + 38h], edx
        mov    dword ptr [esi + 3ch], edx
        mov    eax, 5eah
        mov    word ptr [esi + 40h], 8000h
        mov    dword ptr [esi + 68h], edx
        mov    dword ptr [esi + 6ch], edx
        mov    dword ptr [esi + 0a4h], edx
        mov    dword ptr [esi + 70h], edx
        mov    dword ptr [esi + 84h], edx
        mov    dword ptr [esi + 88h], eax
        mov    ecx, 20h
        mov    dword ptr [esi + 8ch], eax
        mov    eax, 4
        mov    dword ptr [esi + 90h], 989680h
        mov    dword ptr [esi + 9ch], eax
        mov    dword ptr [esi + 0a0h], eax
        mov    word ptr [esi + 0d8h], 1
        mov    word ptr [esi + 0dah], cx
        mov    dword ptr [esi + 0dch], 400h
        mov    dword ptr [esi + 0e0h], ecx
        mov    dword ptr [esi + 0e4h], 1388h
        mov    dword ptr [esi + 0e8h], 1d4ch
        mov    dword ptr [esi + 0ech], 9810h
        mov    dword ptr [esi + 0f0h], eax
        mov    word ptr [esi + 0f4h], dx
        mov    word ptr [esi + 0f6h], dx
        mov    word ptr [esi + 0f8h], dx
        mov    dword ptr [esi + 0fah], 9900h
        mov    dword ptr [esi + 0feh], 100h
        mov    word ptr [esi + 102h], dx
        mov    word ptr [esi + 104h], dx
        mov    word ptr [esi + 106h], dx
        db  0C7h, 086h, 060h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 164h], edx
        push   edi
        mov    dword ptr [esi + 158h], edx
        mov    dword ptr [esi + 15ch], edx
        db  0C7h, 086h, 074h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 178h], edx
        mov    dword ptr [esi + 16ch], edx
        mov    dword ptr [esi + 170h], edx
        call   sub_1_01C8
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_27A8:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        push   edi
        add    ebx, 0b0h
        mov    eax, dword ptr [ecx + 22ch]
        mov    esi, dword ptr [ecx + 184h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebx + 0a8h]
        add    eax, 0eh
        mov    dword ptr [ebx + 0ach], eax
        mov    eax, dword ptr [ebx + 0a8h]
        add    eax, 0eh
        mov    dword ptr [ebx + 0b0h], eax
        mov    eax, dword ptr [ebx + 0a8h]
        add    eax, 0bh
        mov    dword ptr [ebx + 0b4h], eax
        mov    eax, dword ptr [ebx + 0a8h]
        add    eax, 0ah
        mov    dword ptr [ebx + 0b8h], eax
        mov    eax, dword ptr [ebx + 0a8h]
        add    eax, 0ch
        mov    dword ptr [ebx + 0bch], eax
        mov    eax, dword ptr [ebx + 0a8h]
        add    eax, 8
        mov    dword ptr [ebx + 0c0h], eax
        mov    eax, dword ptr [ebx + 0a8h]
        add    eax, 0ah
        mov    dword ptr [ebx + 0c4h], eax
        mov    eax, dword ptr [ebx + 0a8h]
        add    eax, 8
        mov    dword ptr [ebx + 0c8h], eax
        mov    eax, dword ptr [ebx + 0a8h]
        mov    dword ptr [ebx + 0cch], eax
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebx + 78h]
        test   eax, eax
        je     SHORT loc_1_285F
        cmp    eax, 4
        jne    SHORT loc_1_288C
loc_1_285F:
        cmp    byte ptr [ebx + 0ah], 0
        jne    SHORT loc_1_288C
        cli
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_3A38
        mov    ecx, dword ptr [ebx + 68h]
        mov    edx, esi
        sub    cl, 28h
        mov    al, cl
        out    dx, al
        mov    eax, dword ptr [ebx + 0a4h]
        sub    eax, 200h
        shr    eax, 4
        sub    al, 20h
        out    dx, al
        sti
loc_1_288C:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, 800h
        mov    edx, dword ptr [ecx + 15ch]
        mov    ecx, dword ptr [ecx + 158h]
        out    dx, ax
        mov    edx, ecx
        in     ax, dx
        cmp    ax, 6d50h
        je     SHORT loc_1_28BA
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_28BA:
        lea    edx, [ecx + 0ch]
        mov    eax, 5555h
        out    dx, ax
        in     ax, dx
        cmp    ax, 5555h
        je     SHORT loc_1_28DA
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_28DA:
        mov    eax, 0aaaah
        out    dx, ax
        in     ax, dx
        cmp    ax, 0aaaah
        je     SHORT loc_1_28F7
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_28F7:
        cmp    dword ptr [ebx + 80h], 0
        jne    SHORT loc_1_2917
        lea    edx, [ecx + 8]
        in     ax, dx
        shr    ax, 0ch
        mov    ecx, eax
        and    ecx, 0ffffh
        mov    dword ptr [ebx + 80h], ecx
loc_1_2917:
        cmp    dword ptr [ebx + 78h], 2
        jne    SHORT loc_1_292A
        mov    dword ptr [ebx + 7ch], 0
        mov    byte ptr [ebx + 74h], 1
        jmp    SHORT loc_1_2935
loc_1_292A:
        mov    dword ptr [ebx + 7ch], 1
        mov    byte ptr [ebx + 74h], 0
loc_1_2935:
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 158h]
        add    edx, 4
        in     ax, dx
        test   ah, 2
        je     SHORT loc_1_294C
        mov    byte ptr [ebx + 5], 1
loc_1_294C:
        test   ah, 10h
        je     SHORT loc_1_2955
        mov    byte ptr [ebx + 6], 1
loc_1_2955:
        test   ah, 20h
        je     SHORT loc_1_295E
        mov    byte ptr [ebx + 7], 1
loc_1_295E:
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 158h]
        add    edx, 6
        cmp    dword ptr [ebx + 44h], 0
        in     ax, dx
        jne    SHORT loc_1_29B0
        mov    cx, ax
        and    ecx, 0c000h
        je     SHORT loc_1_298F
        cmp    ecx, 4000h
        je     SHORT loc_1_2998
        cmp    ecx, 0c000h
        je     SHORT loc_1_29A1
        jmp    SHORT loc_1_29A8
loc_1_298F:
        mov    dword ptr [ebx + 44h], 1
        jmp    SHORT loc_1_29A8
loc_1_2998:
        mov    dword ptr [ebx + 44h], 3
        jmp    SHORT loc_1_29A8
loc_1_29A1:
        mov    dword ptr [ebx + 44h], 2
loc_1_29A8:
        test   al, 80h
        je     SHORT loc_1_29B0
        mov    byte ptr [ebx + 4], 1
loc_1_29B0:
        push   0dh
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_3BF8
        cmp    dword ptr [ebx + 18h], 0
        jne    SHORT loc_1_29DF
        mov    cx, ax
        and    cx, 3f00h
        shr    cx, 8
        mov    edx, ecx
        and    edx, 0ffffh
        lea    esi, [edx + edx*4 + 5]
        lea    ecx, [esi + esi*4]
        mov    dword ptr [ebx + 18h], ecx
loc_1_29DF:
        test   ah, 40h
        je     SHORT loc_1_29E8
        mov    byte ptr [ebx + 3], 1
loc_1_29E8:
        push   0eh
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_3BF8
        and    eax, 0ffffh
        test   eax, 0ffffff00h
        jle    SHORT loc_1_2A0D
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2A0D:
        push   14h
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_3BF8
        and    eax, 0fh
        push   0ah
        inc    eax
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        mov    dword ptr [ebx + 10h], eax
        call   sub_1_3BF8
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    esi, 44h
        push   0bh
        push   ecx
        mov    byte ptr [ecx + 45h], al
        mov    byte ptr [esi], ah
        call   sub_1_3BF8
        mov    ecx, dword ptr [ebp + 8]
        push   0ch
        push   ecx
        mov    byte ptr [ecx + 46h], ah
        mov    byte ptr [ecx + 47h], al
        call   sub_1_3BF8
        mov    ecx, dword ptr [ebp + 8]
        lea    edx, [ecx + 3eh]
        mov    byte ptr [ecx + 48h], ah
        mov    byte ptr [ecx + 49h], al
        mov    al, byte ptr [ecx + 3fh]
        or     al, byte ptr [ecx + 40h]
        or     al, byte ptr [ecx + 41h]
        or     al, byte ptr [ecx + 43h]
        or     al, byte ptr [ecx + 42h]
        or     al, byte ptr [edx]
        jne    SHORT loc_1_2A7C
        mov    eax, dword ptr [esi]
        mov    dword ptr [edx], eax
        mov    cx, word ptr [esi + 4]
        mov    word ptr [edx + 4], cx
loc_1_2A7C:
        mov    ecx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ecx + 158h]
        mov    esi, dword ptr [ecx + 130h]
        shl    si, 0ch
        lea    eax, [edi + 8]
        mov    edx, eax
        in     ax, dx
        and    ax, 0fffh
        lea    edx, [edi + 8]
        or     si, ax
        mov    ax, si
        mov    esi, dword ptr [ecx + 15ch]
        out    dx, ax
        lea    edx, [edi + 4]
        mov    eax, 1
        out    dx, ax
        mov    edx, esi
        mov    eax, 5800h
        out    dx, ax
        mov    eax, dword ptr [ecx + 160h]
        mov    edx, eax
        mov    dword ptr [ebp - 14h], eax
loc_1_2AC9:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_2AC9
        mov    edx, esi
        mov    eax, 2800h
        out    dx, ax
        mov    edx, dword ptr [ebp - 14h]
loc_1_2ADC:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_2ADC
        mov    edx, esi
        mov    eax, 68ffh
        out    dx, ax
        mov    eax, 806h
        out    dx, ax
        mov    eax, 0b000h
        out    dx, ax
        mov    edx, edi
        in     al, dx
        lea    edx, [edi + 4]
        in     al, dx
        lea    edx, [edi + 8]
        in     al, dx
        lea    edx, [edi + 1]
        in     al, dx
        lea    edx, [edi + 2]
        in     al, dx
        lea    edx, [edi + 3]
        in     al, dx
        lea    edx, [edi + 5]
        in     al, dx
        lea    edx, [edi + 6]
        in     al, dx
        lea    edx, [edi + 7]
        in     al, dx
        lea    edx, [edi + 0ch]
        in     ax, dx
        lea    edx, [edi + 0ah]
        in     ax, dx
        mov    edx, esi
        mov    eax, 801h
        mov    esi, dword ptr [ebp + 8]
        out    dx, ax
        mov    edx, dword ptr [esi + 16ch]
        in     ax, dx
        mov    ecx, eax
        and    ecx, 0ffffh
        cmp    ax, 0bd4h
        mov    dword ptr [ebx + 30h], ecx
        ja     SHORT loc_1_2B50
        mov    ax, 0bd4h
loc_1_2B50:
        and    eax, 0ffffh
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebx + 94h], eax
        mov    eax, 803h
        mov    edx, dword ptr [esi + 15ch]
        out    dx, ax
        mov    edx, dword ptr [esi + 174h]
        in     ax, dx
        mov    ecx, eax
        and    ecx, 0ffffh
        cmp    ax, 0bd4h
        mov    dword ptr [ebx + 2ch], ecx
        ja     SHORT loc_1_2B88
        mov    ax, 0bd4h
loc_1_2B88:
        and    eax, 0ffffh
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebx + 98h], eax
        mov    ax, word ptr [ebx + 14h]
        mov    edx, dword ptr [esi + 15ch]
        out    dx, ax
        mov    eax, 70feh
        mov    edi, dword ptr [ebx + 18h]
        out    dx, ax
        lea    edi, [edi + edi*4]
        add    edi, edi
        shr    edi, 5
        cmp    edi, 0ffh
        jb     SHORT loc_1_2BC1
        mov    edi, 0ffh
loc_1_2BC1:
        mov    eax, 801h
        out    dx, ax
        mov    eax, 6000h
        out    dx, ax
        push   esi
        mov    esi, dword ptr [ebp - 10h]
        mov    edx, dword ptr [ebp - 0ch]
        mov    eax, 200h
        mov    ecx, eax
        shr    ecx, 2
        rep outsd
        and    eax, 3
        mov    ecx, eax
        rep outsb
        pop    esi
        shl    edi, 9
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 168h]
        xor    ecx, ecx
        in     al, dx
        mov    cl, al
        sub    edx, edx
        mov    eax, edi
        div    ecx
        and    eax, 0fffffffch
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebx + 1ch], eax
        mov    eax, 5800h
        mov    esi, dword ptr [ecx + 15ch]
        mov    edx, esi
        out    dx, ax
        mov    edx, dword ptr [ecx + 160h]
loc_1_2C1F:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_2C1F
        mov    edx, esi
        mov    eax, 68ffh
        mov    dword ptr [ebp - 18h], 200h
        out    dx, ax
        mov    eax, 6000h
        out    dx, ax
        mov    eax, edi
        mov    edi, dword ptr [ebp - 10h]
        mov    edx, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 18h]
        shr    ecx, 2
        rep insd
        mov    edi, eax
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 168h]
        in     al, dx
        and    eax, 0ffh
        shr    eax, 1
        mov    dword ptr [ebx + 20h], eax
        mov    eax, 2800h
        mov    edx, dword ptr [ecx + 15ch]
        out    dx, ax
        mov    dword ptr [ebp - 8], edx
        mov    edx, dword ptr [ecx + 160h]
loc_1_2C7A:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_2C7A
        mov    edx, dword ptr [ebp - 8]
        mov    eax, 68ffh
        xor    edi, edi
        xor    esi, esi
        out    dx, ax
        mov    eax, 802h
        mov    ecx, dword ptr [ebp + 8]
        out    dx, ax
        mov    eax, dword ptr [ecx + 158h]
        mov    dword ptr [ebp - 4], eax
loc_1_2CA2:
        mov    edx, dword ptr [ebp - 4]
        mov    eax, edi
        inc    di
        add    edx, esi
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        inc    esi
        mov    al, byte ptr [eax + ecx + 3eh]
        out    dx, al
        cmp    di, 6
        jb     SHORT loc_1_2CA2
        cmp    byte ptr [ebx + 4], 0
        je     SHORT loc_1_2D24
        mov    edx, dword ptr [ebp - 8]
        mov    eax, 0b000h
        cmp    byte ptr [ebx + 5], 0
        out    dx, ax
        je     SHORT loc_1_2CE8
        push   ecx
        call   sub_1_2D98
        test   al, al
        je     SHORT loc_1_2CE8
        mov    dword ptr [ebx + 44h], 1
        jmp    SHORT loc_1_2D31
loc_1_2CE8:
        cmp    byte ptr [ebx + 6], 0
        je     SHORT loc_1_2D06
        push   2
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_2E28
        test   al, al
        je     SHORT loc_1_2D06
        mov    dword ptr [ebx + 44h], 2
        jmp    SHORT loc_1_2D31
loc_1_2D06:
        cmp    byte ptr [ebx + 7], 0
        je     SHORT loc_1_2D24
        push   3
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_2E28
        test   al, al
        je     SHORT loc_1_2D24
        mov    dword ptr [ebx + 44h], 3
        jmp    SHORT loc_1_2D31
loc_1_2D24:
        mov    eax, dword ptr [ebx + 44h]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        call   sub_1_2FC8
loc_1_2D31:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, 0a800h
        mov    esi, dword ptr [ecx + 15ch]
        mov    ecx, dword ptr [ecx + 160h]
        mov    edx, esi
        out    dx, ax
        mov    eax, 5800h
        out    dx, ax
        mov    edx, ecx
loc_1_2D52:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_2D52
        mov    edx, esi
        mov    eax, 2800h
        out    dx, ax
        mov    edx, ecx
loc_1_2D64:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_2D64
        mov    ax, word ptr [ebx + 24h]
        mov    edx, esi
        out    dx, ax
        mov    ax, word ptr [ebx + 0ech]
        pop    edi
        out    dx, ax
        mov    eax, 801h
        pop    esi
        out    dx, ax
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2D98:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   1
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_1_2FC8
        mov    edx, dword ptr [edi + 15ch]
        mov    eax, 4800h
        out    dx, ax
        mov    eax, 2000h
        push   30d40h
        out    dx, ax
        call   sub_1_53D0
        mov    esi, dword ptr [edi + 15ch]
        mov    eax, 804h
        mov    edx, esi
        out    dx, ax
        mov    edx, dword ptr [edi + 158h]
        add    edx, 0ah
        in     ax, dx
        and    ax, 800h
        mov    edx, esi
        cmp    ax, 1
        mov    ecx, dword ptr [edi + 160h]
        sbb    ebx, ebx
        mov    eax, 5800h
        out    dx, ax
        inc    ebx
        mov    edx, ecx
loc_1_2DFD:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_2DFD
        mov    edx, esi
        mov    eax, 2800h
        out    dx, ax
        mov    edx, ecx
loc_1_2E0F:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_2E0F
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2E28:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   esi
        push   edi
        push   eax
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        push   esi
        call   sub_1_2FC8
loc_1_2E3D:
        push   esi
        call   sub_1_2E68
        test   al, al
        jne    SHORT loc_1_2E55
        inc    edi
        cmp    edi, 3
        jb     SHORT loc_1_2E3D
        xor    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_2E55:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2E68:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        xor    bl, bl
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 17ch]
        mov    ecx, dword ptr [edi + 15ch]
        mov    edx, ecx
        mov    dword ptr [ebp - 4], eax
        mov    eax, 804h
        out    dx, ax
        mov    edx, dword ptr [edi + 158h]
        add    edx, 6
        in     ax, dx
        and    ax, 0fffh
        or     ah, 80h
        out    dx, ax
        mov    edx, ecx
        mov    eax, 4800h
        out    dx, ax
        mov    eax, 2000h
        out    dx, ax
        mov    eax, 8001h
        lea    ecx, [edi + 3eh]
        out    dx, ax
        mov    eax, 801h
        mov    dword ptr [ebp - 8], ecx
        out    dx, ax
        mov    edx, dword ptr [ebp - 4]
        mov    eax, 800eh
        out    dx, eax
        push   esi
        mov    esi, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    eax, 6
        mov    ecx, eax
        shr    ecx, 2
        rep outsd
        and    eax, 3
        mov    ecx, eax
        rep outsb
        pop    esi
        push   esi
        mov    esi, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    eax, 6
        mov    ecx, eax
        shr    ecx, 2
        rep outsd
        and    eax, 3
        mov    ecx, eax
        rep outsb
        pop    esi
        mov    edx, dword ptr [ebp - 4]
        xor    eax, eax
        out    dx, eax
        mov    esi, 3e8h
loc_1_2F10:
        push   esi
        call   sub_1_53D0
        mov    edx, dword ptr [edi + 164h]
        in     al, dx
        test   al, 80h
        jne    SHORT loc_1_2F2F
        add    esi, 3e8h
        cmp    esi, 59d8h
        jb     SHORT loc_1_2F10
loc_1_2F2F:
        mov    edx, dword ptr [edi + 15ch]
        mov    eax, 5800h
        out    dx, ax
        mov    edx, dword ptr [edi + 160h]
loc_1_2F42:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_2F42
        xor    esi, esi
loc_1_2F4B:
        mov    edx, dword ptr [edi + 170h]
        in     ax, dx
        test   ah, 80h
        je     SHORT loc_1_2F65
        push   64h
        inc    esi
        call   sub_1_53D0
        cmp    esi, 0fh
        jb     SHORT loc_1_2F4B
loc_1_2F65:
        mov    edx, dword ptr [edi + 170h]
        in     ax, dx
        test   ah, 0c0h
        jne    SHORT loc_1_2F74
        mov    bl, 1
loc_1_2F74:
        mov    ecx, dword ptr [edi + 15ch]
        mov    eax, 2800h
        mov    edx, ecx
        out    dx, ax
        mov    edx, dword ptr [edi + 160h]
loc_1_2F89:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_2F89
        mov    edx, ecx
        mov    eax, 68ffh
        out    dx, ax
        mov    eax, 804h
        out    dx, ax
        mov    edx, dword ptr [edi + 158h]
        pop    edi
        add    edx, 6
        pop    esi
        in     ax, dx
        and    eax, 0fffh
        out    dx, ax
        mov    al, bl
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2FC8:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, 800h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ecx + 15ch]
        mov    edx, ebx
        mov    edi, dword ptr [ecx + 158h]
        out    dx, ax
        lea    edx, [edi + 6]
        mov    esi, dword ptr [ebp + 0ch]
        in     ax, dx
        and    ax, 3fffh
        cmp    esi, 1
        je     SHORT loc_1_300A
        cmp    esi, 2
        jne    SHORT loc_1_3002
        or     ah, 0c0h
        jmp    SHORT loc_1_300A
loc_1_3002:
        cmp    esi, 3
        jne    SHORT loc_1_300A
        or     ah, 40h
loc_1_300A:
        out    dx, ax
        mov    edx, ebx
        mov    eax, 804h
        out    dx, ax
        lea    edx, [edi + 0ah]
        in     ax, dx
        and    ax, 0ff37h
        cmp    esi, 3
        jne    SHORT loc_1_3025
        or     al, 8
loc_1_3025:
        cmp    esi, 1
        jne    SHORT loc_1_3037
        or     al, 40h
        cmp    byte ptr [ecx + 0b3h], 0
        jne    SHORT loc_1_3037
        or     al, 80h
loc_1_3037:
        out    dx, ax
        mov    edx, ebx
        cmp    esi, 2
        jne    SHORT loc_1_3047
        mov    eax, 1000h
        jmp    SHORT loc_1_304C
loc_1_3047:
        mov    eax, 0b800h
loc_1_304C:
        out    dx, ax
        push   0bb8h
        call   sub_1_53D0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3068:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0b8h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 15ch]
        mov    eax, 6000h
        out    dx, ax
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_309A
loc_1_3097:
        inc    dword ptr [ebp - 8]
loc_1_309A:
        cmp    dword ptr [ebp - 8], 1388h
        jae    NEAR PTR loc_1_30ED
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0b8h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_30C2
        jmp    NEAR PTR loc_1_30ED
loc_1_30C2:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 168h]
        in     al, dx
        and    eax, 0ffh
        cmp    eax, 0ffh
        jne    NEAR PTR loc_1_30E1
        jmp    NEAR PTR loc_1_30ED
loc_1_30E1:
        push   1
        call   sub_1_53D0
        jmp    NEAR PTR loc_1_3097
loc_1_30ED:
        cmp    dword ptr [ebp - 8], 320h
        jae    NEAR PTR loc_1_3101
        mov    dword ptr [ebp - 8], 320h
loc_1_3101:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_3110
loc_1_310D:
        inc    dword ptr [ebp - 4]
loc_1_3110:
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 2
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        cmp    eax, dword ptr [ebp - 4]
        jbe    NEAR PTR loc_1_3180
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0b8h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_3143
        jmp    NEAR PTR loc_1_3180
loc_1_3143:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 160h]
        in     ax, dx
        and    eax, 0ffffh
        test   al, 1
        jne    NEAR PTR loc_1_316A
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0b8h], 1
        jmp    NEAR PTR loc_1_3180
loc_1_316A:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 168h]
        in     al, dx
        push   1
        call   sub_1_53D0
        jmp    NEAR PTR loc_1_310D
loc_1_3180:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0b8h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_31A0
        mov    eax, 1
        jmp    NEAR PTR loc_1_31A7
loc_1_31A0:
        xor    eax, eax
        jmp    NEAR PTR loc_1_31A7
loc_1_31A7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_31AE:
        push   ebp
        mov    ecx, 0ah
        mov    ebp, esp
        sub    esp, 28h
        mov    word ptr [ebp - 28h], cx
        mov    eax, 5
        mov    word ptr [ebp - 26h], cx
        push   esi
        mov    word ptr [ebp - 18h], ax
        push   edi
        db  0C7h, 045h, 0DCh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0ECh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0F4h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0E4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 16h], ax
        mov    eax, 7
        mov    word ptr [ebp - 10h], ax
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [ebp - 0eh], ax
        push   1
        mov    word ptr [ebp - 20h], cx
        lea    eax, [ebp - 28h]
        mov    word ptr [ebp - 1eh], cx
        push   eax
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 10h]
        push   edi
        lea    eax, [ebp - 8]
        mov    dword ptr [esi + 128h], ecx
        lea    ecx, [ebp - 4]
        push   ecx
        push   eax
        call   sub_1_545A
        mov    dword ptr [esi + 154h], 0
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_3252
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        cmp    eax, 200h
        jb     SHORT loc_1_3252
        cmp    eax, 3e0h
        ja     SHORT loc_1_3252
        test   al, 0fh
        jne    SHORT loc_1_3252
        mov    dword ptr [esi + 154h], eax
loc_1_3252:
        push   1
        lea    eax, [ebp - 20h]
        push   eax
        lea    ecx, [ebp - 4]
        push   edi
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_545A
        mov    dword ptr [esi + 130h], 0
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_32AA
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        cmp    eax, 3
        je     SHORT loc_1_32A4
        cmp    eax, 5
        je     SHORT loc_1_32A4
        cmp    eax, 7
        je     SHORT loc_1_32A4
        cmp    eax, 9
        je     SHORT loc_1_32A4
        cmp    eax, 0ah
        je     SHORT loc_1_32A4
        cmp    eax, 0bh
        je     SHORT loc_1_32A4
        cmp    eax, 0ch
        je     SHORT loc_1_32A4
        cmp    eax, 0fh
        jne    SHORT loc_1_32AA
loc_1_32A4:
        mov    dword ptr [esi + 130h], eax
loc_1_32AA:
        push   1
        lea    eax, [ebp - 18h]
        push   eax
        lea    ecx, [ebp - 4]
        push   edi
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_545A
        mov    dword ptr [esi + 120h], 0
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_32E4
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        cmp    eax, 1
        jb     SHORT loc_1_32E4
        cmp    eax, 0fh
        ja     SHORT loc_1_32E4
        mov    dword ptr [esi + 120h], eax
loc_1_32E4:
        push   1
        lea    eax, [ebp - 10h]
        push   eax
        lea    ecx, [ebp - 4]
        push   edi
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_545A
        mov    dword ptr [esi + 180h], 110h
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_3326
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        cmp    eax, 100h
        jb     SHORT loc_1_3326
        cmp    eax, 1f0h
        ja     SHORT loc_1_3326
        test   al, 0fh
        jne    SHORT loc_1_3326
        mov    dword ptr [esi + 180h], eax
loc_1_3326:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3338:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 128h]
        cmp    eax, 1
        jne    SHORT loc_1_3388
        mov    eax, dword ptr [esi + 4ah]
        push   eax
        push   esi
        call   sub_1_3558
        test   eax, eax
        jne    SHORT loc_1_335F
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
loc_1_335F:
        mov    eax, dword ptr [esi + 4ah]
        push   eax
        push   esi
        call   sub_1_3628
        test   eax, eax
        jne    SHORT loc_1_337E
        mov    dword ptr [esi + 128h], 0
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
loc_1_337E:
        mov    eax, 1
        pop    esi
        pop    ebp
        ret    4
loc_1_3388:
        cmp    eax, 2
        jne    SHORT loc_1_339C
        mov    eax, dword ptr [esi + 4ah]
        push   eax
        push   esi
        call   sub_1_3408
        pop    esi
        pop    ebp
        ret    4
loc_1_339C:
        test   eax, eax
        mov    eax, dword ptr [esi + 4ah]
        push   eax
        push   esi
        jne    SHORT loc_1_33CA
        call   sub_1_3628
        test   eax, eax
        jne    SHORT loc_1_33B5
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
loc_1_33B5:
        mov    eax, dword ptr [esi + 4ah]
        push   eax
        push   esi
        call   sub_1_3558
        pop    esi
        cmp    eax, 1
        sbb    eax, eax
        pop    ebp
        inc    eax
        ret    4
loc_1_33CA:
        call   sub_1_3628
        test   eax, eax
        jne    SHORT loc_1_33DA
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
loc_1_33DA:
        mov    eax, dword ptr [esi + 4ah]
        push   eax
        push   esi
        call   sub_1_3558
        test   eax, eax
        jne    SHORT loc_1_33EF
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
loc_1_33EF:
        mov    eax, dword ptr [esi + 4ah]
        push   eax
        push   esi
        call   sub_1_3408
        pop    esi
        cmp    eax, 1
        sbb    eax, eax
        pop    ebp
        inc    eax
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_3408:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [esi + 120h]
        test   ebx, ebx
        je     SHORT loc_1_344A
        cmp    byte ptr [edi + ebx + 13bh], 0
        jne    SHORT loc_1_344A
        push   ebx
        push   esi
        call   sub_1_3498
        test   al, al
        je     SHORT loc_1_344A
        xor    eax, eax
        mov    dword ptr [esi + 11ch], ebx
        mov    byte ptr [edi + ebx + 13bh], 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_344A:
        mov    ebx, 1
loc_1_344F:
        cmp    byte ptr [edi + ebx + 13bh], 0
        jne    SHORT loc_1_3464
        push   ebx
        push   esi
        call   sub_1_3498
        test   al, al
        jne    SHORT loc_1_3476
loc_1_3464:
        inc    ebx
        cmp    ebx, 8
        jbe    SHORT loc_1_344F
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_3476:
        xor    eax, eax
        mov    dword ptr [esi + 11ch], ebx
        mov    byte ptr [edi + ebx + 13bh], 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3498:
        push   ebp
        mov    al, 0ffh
        mov    ebp, esp
        xor    edx, edx
        out    94h, al
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    ecx, edx
        je     SHORT loc_1_34B3
loc_1_34A8:
        mov    eax, edx
        inc    edx
        or     al, 8
        cmp    edx, ecx
        out    96h, al
        jb     SHORT loc_1_34A8
loc_1_34B3:
        mov    edx, 100h
        in     al, dx
        mov    cl, al
        mov    edx, 101h
        in     al, dx
        and    ax, 0ffh
        and    ecx, 0ffff00ffh
        shl    cx, 8
        add    cx, ax
        mov    al, cl
        shr    cx, 8
        and    eax, 0ffff00ffh
        shl    ax, 8
        add    ax, cx
        cmp    ax, 627ch
        je     SHORT loc_1_350A
        cmp    ax, 627dh
        je     SHORT loc_1_350A
        cmp    ax, 61dbh
        je     SHORT loc_1_350A
        cmp    ax, 62f6h
        je     SHORT loc_1_350A
        cmp    ax, 62f7h
        je     SHORT loc_1_350A
        xor    al, al
        pop    ebp
        out    96h, al
        ret    8
loc_1_350A:
        mov    edx, 102h
        in     al, dx
        test   al, 1
        jne    SHORT loc_1_351C
        xor    al, al
        pop    ebp
        out    96h, al
        ret    8
loc_1_351C:
        mov    edx, 104h
        in     al, dx
        and    ax, 0fch
        mov    edx, dword ptr [ebp + 8]
        add    ax, 2
        shl    ax, 8
        pop    ebp
        mov    ecx, eax
        xor    al, al
        out    96h, al
        and    ecx, 0ffffh
        mov    al, 1
        mov    dword ptr [edx + 154h], ecx
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3558:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [esi + 120h]
        test   ebx, ebx
        je     SHORT loc_1_35A5
        cmp    byte ptr [edi + ebx + 12bh], 0
        jne    SHORT loc_1_35A5
        push   ebx
        push   esi
        call   sub_1_35F8
        test   al, al
        je     SHORT loc_1_35A5
        mov    eax, ebx
        mov    dword ptr [esi + 11ch], ebx
        shl    eax, 0ch
        mov    dword ptr [esi + 154h], eax
        xor    eax, eax
        mov    byte ptr [edi + ebx + 12bh], 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_35A5:
        mov    ebx, 1
loc_1_35AA:
        cmp    byte ptr [edi + ebx + 12bh], 0
        jne    SHORT loc_1_35BF
        push   ebx
        push   esi
        call   sub_1_35F8
        test   al, al
        jne    SHORT loc_1_35D1
loc_1_35BF:
        inc    ebx
        cmp    ebx, 0fh
        jbe    SHORT loc_1_35AA
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_35D1:
        mov    eax, ebx
        mov    dword ptr [esi + 11ch], ebx
        shl    eax, 0ch
        mov    dword ptr [esi + 154h], eax
        xor    eax, eax
        mov    byte ptr [edi + ebx + 12bh], 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_35F8:
        push   ebp
        mov    edx, dword ptr [esp + 0ch]
        shl    edx, 0ch
        mov    ebp, esp
        add    edx, 0c80h
        in     ax, dx
        cmp    ax, 6d50h
        je     SHORT loc_1_3616
        xor    al, al
        pop    ebp
        ret    8
loc_1_3616:
        add    edx, 2
        pop    ebp
        in     ax, dx
        mov    cl, al
        not    cl
        and    cl, 1
        mov    al, cl
        ret    8
sub_1_3628:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [edi + 180h]
        mov    dword ptr [edi + 184h], eax
        cmp    byte ptr [esi + 5ah], 0
        jne    SHORT loc_1_3651
        mov    byte ptr [esi + 5ah], 1
        push   esi
        push   edi
        call   sub_1_3718
loc_1_3651:
        mov    ebx, dword ptr [edi + 154h]
        test   ebx, ebx
        je     NEAR PTR loc_1_36E6
        push   ebx
        push   esi
        push   edi
        call   sub_1_3838
        cmp    eax, 2
        jne    NEAR PTR loc_1_370F
        push   0
        push   ebx
        push   esi
        push   edi
        call   sub_1_39B8
        test   al, al
        je     SHORT loc_1_3687
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_3687:
        push   1
        push   ebx
        push   esi
        push   edi
        call   sub_1_39B8
        test   al, al
        je     SHORT loc_1_369E
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_369E:
        push   0
        push   0
        push   esi
        push   edi
        call   sub_1_39B8
        test   al, al
        je     SHORT loc_1_36BC
        xor    eax, eax
        mov    dword ptr [edi + 154h], ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_36BC:
        push   1
        push   0
        push   esi
        push   edi
        call   sub_1_39B8
        test   al, al
        je     SHORT loc_1_36DA
        xor    eax, eax
        mov    dword ptr [edi + 154h], ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_36DA:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_36E6:
        push   0
        push   0
        push   esi
        push   edi
        call   sub_1_39B8
        test   al, al
        je     SHORT loc_1_36FE
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_36FE:
        push   1
        push   0
        push   esi
        push   edi
        call   sub_1_39B8
        cmp    al, 1
        sbb    eax, eax
        neg    eax
loc_1_370F:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_1_3718:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    byte ptr [ebp - 1], 0
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 184h]
        mov    dword ptr [ebp - 8], eax
        cli
        push   edi
        mov    ebx, 1
        call   sub_1_3A38
        mov    edx, dword ptr [ebp - 8]
        mov    al, 0d0h
        out    dx, al
        mov    esi, dword ptr [ebp + 0ch]
        add    esi, 69h
loc_1_3749:
        push   7
        push   edi
        call   sub_1_3A78
        cmp    ax, 6d50h
        jne    NEAR PTR loc_1_3814
        push   0ah
        push   edi
        call   sub_1_3A78
        push   0bh
        mov    byte ptr [esi + 6], ah
        push   edi
        mov    byte ptr [esi + 7], al
        call   sub_1_3A78
        push   0ch
        mov    byte ptr [esi + 8], ah
        push   edi
        mov    byte ptr [esi + 9], al
        call   sub_1_3A78
        push   3
        mov    byte ptr [esi + 0ah], ah
        push   edi
        mov    byte ptr [esi + 0bh], al
        mov    byte ptr [esi - 1], 1
        mov    byte ptr [esi], 0
        call   sub_1_3A78
        test   al, 1
        je     SHORT loc_1_379D
        mov    byte ptr [esi], 1
        jmp    SHORT loc_1_37F7
loc_1_379D:
        push   8
        push   edi
        call   sub_1_3A78
        and    ax, 1fh
        mov    ecx, eax
        and    ecx, 0ffffh
        add    ecx, 20h
        shl    ecx, 4
        cmp    ax, 1fh
        mov    dword ptr [esi + 2], ecx
        jne    SHORT loc_1_37C5
        mov    byte ptr [esi], 1
        jmp    SHORT loc_1_37F7
loc_1_37C5:
        push   10h
        push   edi
        call   sub_1_3A78
        test   al, 1
        je     SHORT loc_1_37F7
        push   13h
        push   edi
        call   sub_1_3A78
        test   al, 8
        jne    SHORT loc_1_37E1
        test   al, 0ch
        jne    SHORT loc_1_37F7
loc_1_37E1:
        mov    byte ptr [esi + 1], 1
        push   14h
        push   edi
        call   sub_1_3A78
        and    al, 0fh
        cmp    al, 2
        jae    SHORT loc_1_37F7
        mov    byte ptr [ebp - 1], 1
loc_1_37F7:
        lea    eax, [ebx - 30h]
        mov    edx, dword ptr [ebp - 8]
        push   edi
        add    esi, 0dh
        out    dx, al
        xor    al, al
        inc    ebx
        out    dx, al
        call   sub_1_3A38
        cmp    ebx, 8
        jb     NEAR PTR loc_1_3749
loc_1_3814:
        mov    edx, dword ptr [ebp - 8]
        xor    al, al
        out    dx, al
        sti
        cmp    byte ptr [ebp - 1], al
        je     SHORT loc_1_382E
        mov    edx, 279h
        mov    al, 2
        out    dx, al
        mov    edx, 0a79h
        out    dx, al
loc_1_382E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
sub_1_3838:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        mov    edi, dword ptr [ebp + 8]
        mov    eax, 800h
        lea    edx, [esi + 0eh]
        mov    dword ptr [edi + 154h], esi
        out    dx, ax
        mov    edx, esi
        mov    dword ptr [edi + 158h], esi
        in     ax, dx
        cmp    ax, 6d50h
        je     SHORT loc_1_3875
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_3875:
        lea    edx, [esi + 2]
        in     ax, dx
        test   al, 1
        je     SHORT loc_1_388C
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_388C:
        lea    edx, [esi + 0ch]
        mov    eax, 5555h
        out    dx, ax
        in     ax, dx
        cmp    ax, 5555h
        je     SHORT loc_1_38AC
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_38AC:
        mov    eax, 0aaaah
        out    dx, ax
        in     ax, dx
        cmp    ax, 0aaaah
        je     SHORT loc_1_38C9
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_38C9:
        mov    ebx, 1
        push   0ah
        push   edi
        mov    byte ptr [edi + 0bah], bl
        call   sub_1_3BF8
        push   0bh
        mov    byte ptr [ebp - 8], ah
        push   edi
        mov    byte ptr [ebp - 7], al
        call   sub_1_3BF8
        push   0ch
        mov    byte ptr [ebp - 6], ah
        push   edi
        mov    byte ptr [ebp - 5], al
        call   sub_1_3BF8
        mov    edi, dword ptr [ebp + 0ch]
        mov    byte ptr [ebp - 4], ah
        lea    edx, [edi + 68h]
loc_1_3901:
        cmp    byte ptr [edx], 0
        je     SHORT loc_1_393C
        mov    cl, byte ptr [ebp - 8]
        cmp    byte ptr [edx + 7], cl
        jne    SHORT loc_1_3933
        mov    cl, byte ptr [ebp - 7]
        cmp    byte ptr [edx + 8], cl
        jne    SHORT loc_1_3933
        mov    cl, byte ptr [ebp - 6]
        cmp    byte ptr [edx + 9], cl
        jne    SHORT loc_1_3933
        mov    cl, byte ptr [ebp - 5]
        cmp    byte ptr [edx + 0ah], cl
        jne    SHORT loc_1_3933
        mov    cl, byte ptr [ebp - 4]
        cmp    byte ptr [edx + 0bh], cl
        jne    SHORT loc_1_3933
        cmp    byte ptr [edx + 0ch], al
        je     SHORT loc_1_3980
loc_1_3933:
        add    edx, 0dh
        inc    ebx
        cmp    ebx, 10h
        jb     SHORT loc_1_3901
loc_1_393C:
        lea    ecx, [ebx + ebx*2]
        lea    ebx, [ebx + ecx*4]
        mov    ecx, 1
        mov    byte ptr [ebx + edi + 5bh], cl
        lea    edx, [ebx + edi]
        mov    bl, byte ptr [ebp - 8]
        mov    byte ptr [edx + 5ch], cl
        mov    cl, byte ptr [ebp - 7]
        mov    dword ptr [edx + 5eh], esi
        mov    byte ptr [edx + 62h], bl
        mov    bl, byte ptr [ebp - 6]
        mov    byte ptr [edx + 63h], cl
        mov    cl, byte ptr [ebp - 5]
        pop    edi
        mov    byte ptr [edx + 64h], bl
        mov    bl, byte ptr [ebp - 4]
        pop    esi
        mov    byte ptr [edx + 65h], cl
        mov    byte ptr [edx + 66h], bl
        pop    ebx
        mov    byte ptr [edx + 67h], al
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_3980:
        lea    eax, [ebx + ebx*2]
        lea    eax, [ebx + eax*4]
        add    eax, edi
        cmp    byte ptr [eax + 5ch], 0
        je     SHORT loc_1_399C
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_399C:
        pop    edi
        mov    dword ptr [eax + 5eh], esi
        mov    byte ptr [eax + 5ch], 1
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_39B8:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 10h]
        mov    cl, byte ptr [ebp + 14h]
        lea    edi, [esi + 68h]
loc_1_39D1:
        cmp    byte ptr [edi], 0
        je     SHORT loc_1_39F7
        cmp    byte ptr [edi + 1], 0
        jne    SHORT loc_1_39EA
        cmp    byte ptr [edi + 2], cl
        jne    SHORT loc_1_39EA
        test   eax, eax
        je     SHORT loc_1_39F5
        cmp    dword ptr [edi + 3], eax
        je     SHORT loc_1_39F5
loc_1_39EA:
        add    edi, 0dh
        inc    edx
        cmp    edx, 10h
        jl     SHORT loc_1_39D1
        jmp    SHORT loc_1_39F7
loc_1_39F5:
        mov    bl, 1
loc_1_39F7:
        cmp    bl, 1
        jne    SHORT loc_1_3A23
        lea    eax, [edx + edx*2]
        lea    eax, [edx + eax*4]
        add    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    byte ptr [eax + 5ch], 1
        mov    ecx, dword ptr [eax + 5eh]
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax + 154h], ecx
        mov    dword ptr [eax + 118h], edx
        mov    al, 1
        ret    10h
loc_1_3A23:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3A38:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [eax + 184h]
        mov    esi, 0feh
        xor    al, al
        out    dx, al
        out    dx, al
        mov    ax, 0ffh
loc_1_3A53:
        out    dx, al
        add    ax, ax
        test   ah, 1
        je     SHORT loc_1_3A5E
        xor    al, 0cfh
loc_1_3A5E:
        mov    ecx, esi
        dec    esi
        test   ecx, ecx
        jne    SHORT loc_1_3A53
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3A78:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        push   190h
        xor    si, si
        mov    edi, dword ptr [eax + 184h]
        mov    al, byte ptr [ebp + 0ch]
        mov    edx, edi
        add    al, 80h
        out    dx, al
        call   sub_1_53D0
        mov    ecx, 10h
loc_1_3AA1:
        mov    edx, edi
        in     al, dx
        and    ax, 1
        dec    ecx
        lea    esi, [eax + esi*2]
        jne    SHORT loc_1_3AA1
        mov    ax, si
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  090h
sub_1_3AB8:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   eax
        add    esi, 0b0h
        push   0
        push   800h
        lea    ecx, [esi + 17ch]
        push   ecx
        call   sub_1_53D6
        test   eax, eax
        je     SHORT loc_1_3AF0
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_3AF0:
        mov    edi, dword ptr [esi + 17ch]
        xor    eax, eax
        mov    ecx, 200h
        rep stosd
        db  0A1h, 000h, 000h, 000h, 000h
        lea    ebx, [esi + 180h]
        push   eax
        push   0
        push   5ech
        push   ebx
        call   sub_1_53D6
        test   eax, eax
        je     SHORT loc_1_3B28
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_3B28:
        mov    edi, dword ptr [ebx]
        xor    eax, eax
        mov    ecx, 17bh
        rep stosd
        pop    edi
        mov    eax, dword ptr [esi + 17ch]
        mov    dword ptr [esi + 154h], eax
        xor    eax, eax
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [esi + 168h], ecx
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3B58:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 22ch]
        add    esi, 0b0h
        test   eax, eax
        je     SHORT loc_1_3B7C
        push   0
        push   800h
        push   eax
        call   sub_1_53DC
loc_1_3B7C:
        mov    eax, dword ptr [esi + 180h]
        test   eax, eax
        je     SHORT loc_1_3B93
        push   0
        push   5ech
        push   eax
        call   sub_1_53DC
loc_1_3B93:
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3BA8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    edx, 1
        push   ebx
        push   esi
        push   edi
        xor    bl, bl
        mov    esi, dword ptr [eax + 4ah]
        lea    ecx, [esi + 69h]
loc_1_3BBF:
        cmp    byte ptr [ecx], 0
        je     SHORT loc_1_3BCF
        mov    edi, dword ptr [eax + 154h]
        cmp    dword ptr [ecx + 2], edi
        je     SHORT loc_1_3BDA
loc_1_3BCF:
        add    ecx, 0dh
        inc    edx
        cmp    edx, 10h
        jl     SHORT loc_1_3BBF
        jmp    SHORT loc_1_3BDC
loc_1_3BDA:
        mov    bl, 1
loc_1_3BDC:
        cmp    bl, 1
        jne    SHORT loc_1_3BEC
        lea    eax, [edx + edx*2]
        lea    ecx, [edx + eax*4]
        mov    byte ptr [ecx + esi + 5ch], 0
loc_1_3BEC:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_3BF8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    si, word ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 158h]
        mov    eax, 800h
        and    esi, 0ffff00ffh
        add    si, 80h
        lea    edx, [ecx + 0eh]
        out    dx, ax
        mov    ax, si
        lea    edx, [ecx + 0ah]
        out    dx, ax
loc_1_3C27:
        in     ax, dx
        test   ah, 80h
        jne    SHORT loc_1_3C27
        lea    edx, [ecx + 0ch]
        pop    esi
        in     ax, dx
        pop    ebp
        ret    8
loc_1_3C38:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        call   sub_1_1BA8
        mov    eax, dword ptr [esi + 4]
        add    esi, 62h
        push   eax
        push   esi
        call   sub_1_53B2
        pop    esi
        pop    ebp
        ret    10h
        db  090h
loc_1_3C58:
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        mov    ebp, esp
        mov    ecx, dword ptr [eax]
        mov    eax, 0
        pop    ebp
        cmp    byte ptr [ecx + 83h], 1
        adc    eax, -1
        and    eax, 0c001000dh
        ret    1ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3C88:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        mov    edx, dword ptr [eax]
        xor    eax, eax
        cmp    byte ptr [edx + 83h], al
        je     SHORT loc_1_3CA4
        mov    eax, 0c001000dh
        pop    ebp
        ret    14h
loc_1_3CA4:
        mov    ecx, dword ptr [ebp + 0ch]
        test   ecx, 0f010h
        je     SHORT loc_1_3CB8
        mov    eax, 0c0000001h
        pop    ebp
        ret    14h
loc_1_3CB8:
        test   cl, 1
        je     SHORT loc_1_3CC2
        mov    eax, 1
loc_1_3CC2:
        test   cl, 2
        je     SHORT loc_1_3CCA
        or     eax, 2
loc_1_3CCA:
        test   cl, 4
        je     SHORT loc_1_3CD2
        or     eax, 2
loc_1_3CD2:
        test   cl, 8
        je     SHORT loc_1_3CDA
        or     eax, 4
loc_1_3CDA:
        mov    byte ptr [edx + 82h], 0
        test   cl, 20h
        je     SHORT loc_1_3CF0
        mov    byte ptr [edx + 82h], 1
        or     eax, 8
loc_1_3CF0:
        add    ax, 8000h
        pop    ebp
        mov    word ptr [edx + 0f0h], ax
        mov    edx, dword ptr [edx + 15ch]
        out    dx, ax
        xor    eax, eax
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3D18:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [eax], 0
        push   ebx
        push   esi
        mov    ecx, dword ptr [ebp + 10h]
        push   edi
        mov    edx, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], 0ffffffffh
        lea    eax, [eax + edx*4]
loc_1_3D3D:
        test   edx, edx
        je     SHORT loc_1_3D4C
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_3D3D
        mov    dword ptr [ecx], edx
loc_1_3D4C:
        cmp    dword ptr [ecx], -1
        jne    SHORT loc_1_3D5F
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_3D5F:
        db  0A1h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 4]
        push   eax
        push   0
        push   49h
        push   ecx
        call   sub_1_53D6
        test   eax, eax
        je     SHORT loc_1_3D84
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_3D84:
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 1ch]
        add    eax, 14h
        mov    ebx, dword ptr [ebp + 20h]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebx + 5eh]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_3D9F:
        test   al, al
        jne    SHORT loc_1_3DBE
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   49h
        push   eax
        call   sub_1_53DC
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_3DBE:
        cmp    byte ptr [ebx + 83h], 0
        je     SHORT loc_1_3DDD
        mov    eax, dword ptr [ebx + 5ah]
        mov    ecx, dword ptr [ebp - 4]
        mov    esi, 103h
        mov    dword ptr [ecx + 0ch], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 5ah], ecx
        jmp    SHORT loc_1_3DEE
loc_1_3DDD:
        mov    eax, dword ptr [ebx + 52h]
        mov    ecx, dword ptr [ebp - 4]
        xor    esi, esi
        mov    dword ptr [ecx + 0ch], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 52h], ecx
loc_1_3DEE:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebx + 4ah]
        mov    dword ptr [eax + 10h], edx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 28h]
        mov    dword ptr [eax + 4], edi
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 8], edx
        xor    ecx, ecx
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 2ah], cl
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 20h], cl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 1ch], 1
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edi], edx
        mov    eax, dword ptr [ebx]
        mov    dword ptr [edx + 18h], eax
        cmp    dword ptr [ebx + 56h], ecx
        jne    SHORT loc_1_3E42
        mov    eax, dword ptr [ebx + 4]
        lea    ecx, [ebx + 62h]
        push   eax
        push   ecx
        call   sub_1_53B2
loc_1_3E42:
        push   ebx
        inc    dword ptr [ebx + 56h]
        call   sub_1_4118
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Bh, 0FFh
loc_1_3E58:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 20h], 0
        mov    ecx, dword ptr [esi]
        je     SHORT loc_1_3E71
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    4
loc_1_3E71:
        mov    byte ptr [esi + 20h], 1
        inc    dword ptr [esi + 1ch]
        mov    eax, dword ptr [ecx + 52h]
        cmp    esi, eax
        jne    SHORT loc_1_3E87
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [ecx + 52h], eax
        jmp    SHORT loc_1_3E9A
loc_1_3E87:
        cmp    dword ptr [eax + 0ch], esi
        je     SHORT loc_1_3E94
loc_1_3E8C:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], esi
        jne    SHORT loc_1_3E8C
loc_1_3E94:
        mov    edx, dword ptr [esi + 0ch]
        mov    dword ptr [eax + 0ch], edx
loc_1_3E9A:
        lea    eax, [esi + 21h]
        mov    edx, dword ptr [esi + 14h]
        push   eax
        mov    ecx, dword ptr [ecx + 5eh]
        push   edx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_3EAB:
        test   eax, eax
        je     SHORT loc_1_3EB6
        cmp    eax, 103h
        jne    SHORT loc_1_3EBC
loc_1_3EB6:
        push   esi
        call   sub_1_3EF8
loc_1_3EBC:
        push   esi
        call   sub_1_3EF8
        mov    eax, 103h
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3ED8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_1_3EF8
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3EF8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 1ch]
        dec    ecx
        mov    dword ptr [eax + 1ch], ecx
        jne    SHORT loc_1_3F0E
        push   eax
        call   sub_1_3F18
loc_1_3F0E:
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_3F18:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi]
        push   edi
        call   sub_1_4E38
        mov    eax, dword ptr [edi + 56h]
        mov    ecx, 1
        dec    eax
        push   edi
        mov    dword ptr [edi + 56h], eax
        cmp    ecx, eax
        mov    al, cl
        adc    al, 0ffh
        mov    byte ptr [edi + 0ch], al
        call   sub_1_4148
        push   0
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        call   sub_1_5314
        push   0
        push   49h
        push   esi
        call   sub_1_53DC
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3F68:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        cmp    byte ptr [eax + 20h], 0
        mov    esi, dword ptr [eax]
        je     SHORT loc_1_3F82
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    4
loc_1_3F82:
        mov    cl, byte ptr [esi + 21h]
        test   cl, 2
        je     SHORT loc_1_3F94
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    4
loc_1_3F94:
        or     cl, 2
        push   40010004h
        push   esi
        mov    byte ptr [esi + 21h], cl
        inc    dword ptr [eax + 1ch]
        mov    dword ptr [esi + 6eh], eax
        call   sub_1_4098
        mov    byte ptr [esi + 83h], 1
        push   esi
        call   sub_1_41E8
        mov    eax, 103h
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_3FC8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [esi + 11h]
loc_1_3FD6:
        push   edi
        call   sub_1_4078
        test   eax, eax
        je     SHORT loc_1_3FFF
        or     byte ptr [eax + 2eh], 0ah
        mov    dword ptr [eax + 24h], ebx
        cmp    dword ptr [esi + 1dh], ebx
        jne    SHORT loc_1_3FF4
        mov    dword ptr [esi + 1dh], eax
        mov    dword ptr [esi + 19h], eax
        jmp    SHORT loc_1_3FD6
loc_1_3FF4:
        mov    ecx, dword ptr [esi + 19h]
        mov    dword ptr [ecx + 24h], eax
        mov    dword ptr [esi + 19h], eax
        jmp    SHORT loc_1_3FD6
loc_1_3FFF:
        lea    edi, [esi + 19h]
loc_1_4002:
        push   edi
        call   sub_1_4078
        test   eax, eax
        je     SHORT loc_1_4015
        push   eax
        push   esi
        call   sub_1_06C8
        jmp    SHORT loc_1_4002
loc_1_4015:
        mov    edi, dword ptr [esi + 6eh]
        push   40010005h
        and    byte ptr [esi + 21h], 0fdh
        mov    byte ptr [esi + 83h], 0
        push   esi
        call   sub_1_4098
        push   0
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_5326
        push   edi
        call   sub_1_3EF8
        mov    ecx, dword ptr [esi + 5ah]
        test   ecx, ecx
        je     SHORT loc_1_406C
loc_1_4046:
        mov    eax, dword ptr [esi + 5ah]
        mov    ebx, dword ptr [esi + 52h]
        push   0
        push   0
        mov    edx, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 5ah], edx
        mov    dword ptr [ecx + 0ch], ebx
        mov    dword ptr [esi + 52h], ecx
        mov    eax, dword ptr [ecx + 4]
        push   eax
        call   sub_1_530E
        mov    ecx, dword ptr [esi + 5ah]
        test   ecx, ecx
        jne    SHORT loc_1_4046
loc_1_406C:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_4078:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [ecx + 4]
        test   eax, eax
        je     SHORT loc_1_408C
        mov    edx, dword ptr [eax + 24h]
        mov    dword ptr [ecx + 4], edx
loc_1_408C:
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_4098:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [eax + 52h]
        test   edi, edi
        je     SHORT loc_1_40E1
        mov    esi, dword ptr [ebp + 0ch]
loc_1_40AF:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        inc    dword ptr [edi + 1ch]
        mov    ebx, dword ptr [edi + 0ch]
        push   esi
        mov    dword ptr [ebp - 4], 0
        mov    ecx, dword ptr [edi + 4]
        push   ecx
        call   sub_1_533E
        mov    ecx, dword ptr [edi + 4]
        push   ecx
        call   sub_1_5344
        push   edi
        call   sub_1_3EF8
        mov    edi, ebx
        test   ebx, ebx
        jne    SHORT loc_1_40AF
loc_1_40E1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_40F8:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   eax
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 5eh]
        push   edx
        call   sub_1_5200
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_4118:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    dword ptr [eax + 56h], 1
        jne    SHORT loc_1_4139
        mov    edx, dword ptr [eax + 15ch]
        mov    eax, 2000h
        out    dx, ax
        mov    eax, 4800h
        out    dx, ax
loc_1_4139:
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4148:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    dword ptr [eax + 56h], 0
        jne    SHORT loc_1_4162
        mov    edx, dword ptr [eax + 15ch]
        mov    eax, 1800h
        out    dx, ax
loc_1_4162:
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 055h, 0B8h, 000h, 058h, 000h, 000h, 08Bh, 0ECh, 056h, 08Bh, 04Dh, 008h, 08Bh, 0B1h
        db  05Ch, 001h, 000h, 000h, 08Bh, 089h, 060h, 001h, 000h, 000h, 08Bh, 0D6h, 066h, 0EFh, 08Bh, 0D1h
        db  066h, 0EDh, 0F6h, 0C4h, 010h, 075h, 0F9h, 08Bh, 0D6h, 0B8h, 000h, 028h, 000h, 000h, 066h, 0EFh
        db  08Bh, 0D1h, 066h, 0EDh, 0F6h, 0C4h, 010h, 075h, 0F9h, 08Bh, 0D6h, 0B8h, 000h, 078h, 000h, 000h
        db  066h, 0EFh, 0B8h, 0FFh, 068h, 000h, 000h, 05Eh, 066h, 0EFh, 05Dh, 0C2h, 004h, 000h, 08Dh, 064h
        db  024h, 000h
sub_1_41B8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_1_0268
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_41D8:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_41E8:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    byte ptr [esi + 0b9h], 1
        call   sub_1_12B8
        mov    ax, word ptr [esi + 19ch]
        mov    edx, dword ptr [esi + 15ch]
        mov    dword ptr [esi + 0f8h], 0
        out    dx, ax
        mov    eax, 1800h
        pop    esi
        out    dx, ax
        mov    eax, 7840h
        pop    ebp
        out    dx, ax
        mov    eax, 6000h
        out    dx, ax
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4238:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi]
        inc    dword ptr [esi + 1ch]
        cmp    byte ptr [esi + 20h], 0
        je     SHORT loc_1_4252
        mov    edi, 0c0010002h
        jmp    SHORT loc_1_428E
loc_1_4252:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 10h]
        test   ecx, ecx
        jne    SHORT loc_1_4268
        push   eax
        push   esi
        push   edx
        call   sub_1_47F8
        mov    edi, eax
        jmp    SHORT loc_1_428E
loc_1_4268:
        cmp    ecx, 1
        jne    SHORT loc_1_4289
        cmp    byte ptr [edx + 83h], 0
        je     SHORT loc_1_427D
        mov    edi, 0c001000dh
        jmp    SHORT loc_1_428E
loc_1_427D:
        push   eax
        push   esi
        push   edx
        call   sub_1_4848
        mov    edi, eax
        jmp    SHORT loc_1_428E
loc_1_4289:
        mov    edi, 10001h
loc_1_428E:
        push   esi
        call   sub_1_3EF8
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_42A8:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    ebx, ebx
        mov    ecx, dword ptr [esi + 134h]
        mov    word ptr [ebp - 6], bx
        mov    dword ptr [ebp - 20h], ecx
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 0ch], ebx
        cmp    edx, 1010101h
        mov    eax, edx
        mov    byte ptr [ebp - 4], bl
        mov    dword ptr [ebp - 10h], ebx
        mov    dword ptr [ebp - 1ch], ebx
        ja     SHORT loc_1_42F3
        je     NEAR PTR loc_1_4639
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_1_4314
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_42F3:
        cmp    eax, 1010102h
        je     NEAR PTR loc_1_4655
        cmp    eax, 1010103h
        je     NEAR PTR loc_1_4671
        cmp    eax, 1010104h
        je     NEAR PTR loc_1_471F
loc_1_4314:
        mov    eax, edx
        and    eax, 0ff000000h
        cmp    eax, 0ff000000h
        je     SHORT loc_1_432E
        mov    dword ptr [ebp - 10h], 0c00000bbh
        jmp    NEAR PTR loc_1_4730
loc_1_432E:
        add    edx, 0fefeffh
        cmp    edx, 8
        ja     SHORT loc_1_4340
        db  0FFh, 024h, 095h, 000h, 000h, 000h, 000h
loc_1_4340:
        mov    dword ptr [ebp - 10h], 0c00000bbh
        jmp    NEAR PTR loc_1_4730
loc_1_434C:
        mov    al, byte ptr [esi + 456h]
        mov    esi, 1
        mov    byte ptr [ebp - 4], al
        lea    eax, [ebp - 4]
        jmp    NEAR PTR loc_1_4736
loc_1_4362:
        mov    ax, word ptr [esi + 457h]
        mov    esi, 2
        mov    word ptr [ebp - 6], ax
        lea    eax, [ebp - 6]
        jmp    NEAR PTR loc_1_4736
loc_1_437A:
        mov    ax, word ptr [esi + 459h]
        mov    esi, 2
        mov    word ptr [ebp - 6], ax
        lea    eax, [ebp - 6]
        jmp    NEAR PTR loc_1_4736
loc_1_4392:
        mov    ax, word ptr [esi + 45bh]
        mov    esi, 2
        mov    word ptr [ebp - 6], ax
        lea    eax, [ebp - 6]
        jmp    NEAR PTR loc_1_4736
loc_1_43AA:
        mov    al, byte ptr [esi + 45dh]
        mov    esi, 1
        mov    byte ptr [ebp - 4], al
        lea    eax, [ebp - 4]
        jmp    NEAR PTR loc_1_4736
loc_1_43C0:
        mov    ax, word ptr [esi + 460h]
        mov    esi, 2
        mov    word ptr [ebp - 6], ax
        lea    eax, [ebp - 6]
        jmp    NEAR PTR loc_1_4736
loc_1_43D8:
        mov    ax, word ptr [esi + 45eh]
        mov    esi, 2
        mov    word ptr [ebp - 6], ax
        lea    eax, [ebp - 6]
        jmp    NEAR PTR loc_1_4736
loc_1_43F0:
        xor    eax, eax
        mov    ax, word ptr [esi + 460h]
        lea    ecx, [eax + eax*4]
        lea    edi, [ecx*2]
        cmp    dword ptr [ebp + 1ch], edi
        jae    SHORT loc_1_4419
        mov    ecx, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 10h], 0c0010014h
        mov    dword ptr [ecx], edi
        jmp    NEAR PTR loc_1_4730
loc_1_4419:
        push   edi
        mov    eax, dword ptr [esi + 452h]
        push   eax
        mov    ecx, dword ptr [ebp + 18h]
        push   ecx
        mov    bl, 1
        call   sub_1_54BC
        mov    ecx, dword ptr [ebp + 24h]
        mov    dword ptr [ecx], edi
        jmp    NEAR PTR loc_1_4730
loc_1_4436:
        cmp    dword ptr [ebp + 1ch], 1eh
        jae    SHORT loc_1_4451
        mov    ecx, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 10h], 0c0010014h
        mov    dword ptr [ecx], 1eh
        jmp    NEAR PTR loc_1_4730
loc_1_4451:
        push   1eh
        mov    eax, dword ptr [esi + 452h]
        push   eax
        mov    ecx, dword ptr [ebp + 18h]
        push   ecx
        mov    bl, 1
        call   sub_1_54BC
        mov    ecx, dword ptr [ebp + 24h]
        mov    dword ptr [ecx], 1eh
        jmp    NEAR PTR loc_1_4730
loc_1_4473:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_1_4488
        db  0B8h, 000h, 000h, 000h, 000h
        mov    esi, 5ch
        jmp    NEAR PTR loc_1_4736
loc_1_4488:
        db  0B8h, 000h, 000h, 000h, 000h
        mov    esi, 9ch
        jmp    NEAR PTR loc_1_4736
loc_1_4497:
        cmp    byte ptr [esi + 83h], 0
        mov    dword ptr [ebp - 1ch], 2
        jne    SHORT loc_1_44AE
        mov    dword ptr [ebp - 1ch], 0
loc_1_44AE:
        lea    eax, [ebp - 1ch]
        mov    esi, 4
        jmp    NEAR PTR loc_1_4736
loc_1_44BB:
        lea    eax, [ebp - 20h]
        mov    esi, 4
        jmp    NEAR PTR loc_1_4736
loc_1_44C8:
        mov    eax, dword ptr [esi + 138h]
        mov    esi, 4
        sub    eax, 0eh
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 0ch]
        jmp    NEAR PTR loc_1_4736
loc_1_44E1:
        mov    eax, dword ptr [esi + 13ch]
        mov    esi, 4
        sub    eax, 0eh
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 0ch]
        jmp    NEAR PTR loc_1_4736
loc_1_44FA:
        mov    ecx, 64h
        mov    eax, dword ptr [esi + 140h]
        sub    edx, edx
        mov    esi, 4
        div    ecx
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 0ch]
        jmp    NEAR PTR loc_1_4736
loc_1_4519:
        mov    eax, dword ptr [esi + 144h]
        mov    esi, 4
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 0ch]
        jmp    NEAR PTR loc_1_4736
loc_1_452F:
        mov    eax, dword ptr [esi + 148h]
        mov    esi, 4
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 0ch]
        jmp    NEAR PTR loc_1_4736
loc_1_4545:
        mov    eax, dword ptr [esi + 14ch]
        mov    esi, 4
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 0ch]
        jmp    NEAR PTR loc_1_4736
loc_1_455B:
        mov    eax, dword ptr [esi + 150h]
        mov    esi, 4
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 0ch]
        jmp    NEAR PTR loc_1_4736
loc_1_4571:
        push   3
        add    esi, 44h
        push   esi
        lea    eax, [ebp - 0ch]
        push   eax
        mov    esi, 4
        call   sub_1_54BC
        and    dword ptr [ebp - 0ch], 0ffffff00h
        lea    eax, [ebp - 0ch]
        jmp    NEAR PTR loc_1_4736
loc_1_4594:
        db  0B8h, 000h, 000h, 000h, 000h
        mov    esi, 18h
        jmp    NEAR PTR loc_1_4736
loc_1_45A3:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_45BA
        test   ecx, ecx
        jne    SHORT loc_1_45B5
        mov    eax, dword ptr [esi + 5eh]
        mov    eax, dword ptr [eax + 0ch]
        jmp    SHORT loc_1_45C9
loc_1_45B5:
        mov    eax, dword ptr [ebp - 4]
        jmp    SHORT loc_1_45C9
loc_1_45BA:
        xor    eax, eax
        test   ecx, ecx
        jne    SHORT loc_1_45C9
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [edi + 14h]
        mov    eax, dword ptr [eax + 8]
loc_1_45C9:
        mov    esi, 4
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 0ch]
        jmp    NEAR PTR loc_1_4736
loc_1_45D9:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_45E6
        mov    eax, dword ptr [esi]
        sub    eax, 0eh
        jmp    SHORT loc_1_45EC
loc_1_45E6:
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [edi + 18h]
loc_1_45EC:
        mov    esi, 4
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 0ch]
        jmp    NEAR PTR loc_1_4736
loc_1_45FC:
        lea    eax, [ebp - 6]
        mov    esi, 2
        mov    word ptr [ebp - 6], 300h
        jmp    NEAR PTR loc_1_4736
loc_1_460F:
        mov    eax, dword ptr [esi + 13ch]
        mov    esi, 4
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 0ch]
        jmp    NEAR PTR loc_1_4736
loc_1_4625:
        lea    eax, [ebp - 0ch]
        mov    esi, 4
        mov    dword ptr [ebp - 0ch], 7
        jmp    NEAR PTR loc_1_4736
loc_1_4639:
        push   6
        add    esi, 44h
        push   esi
        lea    eax, [ebp - 28h]
        push   eax
        mov    esi, 6
        call   sub_1_54BC
        lea    eax, [ebp - 28h]
        jmp    NEAR PTR loc_1_4736
loc_1_4655:
        push   6
        add    esi, 3eh
        push   esi
        lea    eax, [ebp - 28h]
        push   eax
        mov    esi, 6
        call   sub_1_54BC
        lea    eax, [ebp - 28h]
        jmp    NEAR PTR loc_1_4736
loc_1_4671:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_46C6
        mov    ecx, dword ptr [esi + 5eh]
        mov    eax, dword ptr [ecx + 24h]
        mov    dword ptr [ebp - 18h], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 1ch], eax
        jae    SHORT loc_1_469B
        mov    ecx, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 10h], 0c0010014h
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_1_4730
loc_1_469B:
        mov    eax, dword ptr [ebp + 18h]
        lea    edx, [ebp - 18h]
        push   eax
        mov    ebx, dword ptr [ebp + 1ch]
        push   edx
        lea    eax, [ebp - 10h]
        push   ebx
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_46B1:
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp + 24h]
        cmp    dword ptr [ebp - 10h], 0
        lea    eax, [eax + eax*2]
        mov    bl, 1
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_4730
loc_1_46C6:
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 5eh]
        mov    eax, dword ptr [edi + 14h]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_46D6:
        mov    dword ptr [ebp - 18h], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 1ch], eax
        jae    SHORT loc_1_46F1
        mov    ecx, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 10h], 0c0010014h
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_4730
loc_1_46F1:
        mov    eax, dword ptr [ebp + 18h]
        lea    ecx, [ebp - 18h]
        push   eax
        mov    edx, dword ptr [ebp + 1ch]
        push   ecx
        mov    eax, dword ptr [edi + 14h]
        push   edx
        mov    ecx, dword ptr [esi + 5eh]
        push   eax
        lea    edx, [ebp - 10h]
        push   ecx
        push   edx
        mov    bl, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4710:
        mov    ecx, dword ptr [ebp - 18h]
        lea    eax, [ecx + ecx*2]
        mov    ecx, dword ptr [ebp + 24h]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_4730
loc_1_471F:
        lea    eax, [ebp - 0ch]
        mov    esi, 4
        mov    dword ptr [ebp - 0ch], 10h
        jmp    SHORT loc_1_4736
loc_1_4730:
        mov    eax, dword ptr [ebp - 14h]
        mov    esi, dword ptr [ebp - 14h]
loc_1_4736:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_476D
        test   bl, bl
        jne    SHORT loc_1_476D
        cmp    dword ptr [ebp + 1ch], esi
        jae    SHORT loc_1_475D
        mov    ecx, dword ptr [ebp + 20h]
        pop    edi
        mov    dword ptr [ebp - 10h], 0c0010014h
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ecx], esi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
loc_1_475D:
        push   esi
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        call   sub_1_54BC
        mov    ecx, dword ptr [ebp + 24h]
        add    dword ptr [ecx], esi
loc_1_476D:
        mov    eax, dword ptr [ebp - 10h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_47F8:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 8
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    edx, [ebp - 4]
        push   edx
        mov    esi, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [esi + 1ch]
        mov    edx, dword ptr [esi + 18h]
        push   eax
        push   edx
        push   ecx
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 14h]
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        push   eax
        push   edx
        call   sub_1_42A8
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 20h], ecx
        mov    dword ptr [esi + 24h], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4848:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 4], 0
        mov    ebx, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esi + 18h]
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 1010103h
        ja     SHORT loc_1_4892
        je     NEAR PTR loc_1_4A67
        cmp    eax, 1010eh
        je     NEAR PTR loc_1_49D5
        cmp    eax, 1010fh
        je     NEAR PTR loc_1_4A11
        cmp    eax, 10112h
        je     NEAR PTR loc_1_4A5E
loc_1_4892:
        mov    edx, eax
        and    edx, 0ff000000h
        cmp    edx, 0ff000000h
        je     SHORT loc_1_48B6
        mov    dword ptr [ebp - 4], 0c0010017h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_1_4AA5
loc_1_48B6:
        add    eax, 0fefefeh
        cmp    eax, 3
        ja     SHORT loc_1_48C7
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_48C7:
        mov    dword ptr [ebp - 4], 0c0010017h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_1_4AA5
loc_1_48DB:
        cmp    ebx, 2
        je     SHORT loc_1_48F4
        mov    dword ptr [ebp - 4], 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_1_4AA5
loc_1_48F4:
        push   2
        mov    edi, dword ptr [ebp + 8]
        push   ecx
        add    edi, 457h
        push   edi
        call   sub_1_54BC
        jmp    NEAR PTR loc_1_4AA5
loc_1_490B:
        cmp    ebx, 2
        je     SHORT loc_1_4924
        mov    dword ptr [ebp - 4], 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_1_4AA5
loc_1_4924:
        push   2
        mov    edi, dword ptr [ebp + 8]
        push   ecx
        add    edi, 457h
        push   edi
        call   sub_1_54BC
        jmp    NEAR PTR loc_1_4AA5
loc_1_493B:
        cmp    ebx, 2
        je     SHORT loc_1_4954
        mov    dword ptr [ebp - 4], 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_1_4AA5
loc_1_4954:
        push   2
        mov    edi, dword ptr [ebp + 8]
        push   ecx
        add    edi, 457h
        push   edi
        call   sub_1_54BC
        jmp    NEAR PTR loc_1_4AA5
loc_1_496B:
        cmp    ebx, 1
        je     SHORT loc_1_4984
        mov    dword ptr [ebp - 4], 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_1_4AA5
loc_1_4984:
        mov    edi, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx]
        cmp    byte ptr [edi + 45dh], al
        je     NEAR PTR loc_1_4AA5
        mov    byte ptr [edi + 45dh], al
        test   al, al
        jne    NEAR PTR loc_1_4AA5
        xor    edx, edx
        xor    eax, eax
        mov    word ptr [edi + 45eh], 0
        mov    dx, word ptr [edi + 460h]
        mov    edi, dword ptr [edi + 452h]
        lea    edx, [edx + edx*4]
        add    edx, edx
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        jmp    NEAR PTR loc_1_4AA5
loc_1_49D5:
        cmp    ebx, 4
        je     SHORT loc_1_49EE
        mov    dword ptr [ebp - 4], 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_1_4AA5
loc_1_49EE:
        push   4
        lea    eax, [ebp - 0ch]
        push   ecx
        push   eax
        call   sub_1_54BC
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 0ch]
        push   eax
        mov    edi, dword ptr [ebp + 8]
        push   esi
        push   ecx
        push   edi
        call   sub_1_4DC8
        jmp    NEAR PTR loc_1_4AA2
loc_1_4A11:
        cmp    ebx, 4
        je     SHORT loc_1_4A27
        mov    dword ptr [ebp - 4], 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    SHORT loc_1_4AA5
loc_1_4A27:
        push   4
        lea    eax, [ebp - 8]
        push   ecx
        push   eax
        call   sub_1_54BC
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 138h]
        sub    eax, 0eh
        cmp    eax, dword ptr [ebp - 8]
        jb     SHORT loc_1_4A55
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 0ch]
        push   edi
        mov    dword ptr [eax + 18h], ecx
        call   sub_1_4E38
        jmp    SHORT loc_1_4AA5
loc_1_4A55:
        mov    dword ptr [ebp - 4], 0c0010014h
        jmp    SHORT loc_1_4AA5
loc_1_4A5E:
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_4AA5
loc_1_4A67:
        mov    edi, 6
        mov    eax, ebx
        sub    edx, edx
        div    edi
        test   edx, edx
        je     SHORT loc_1_4A87
        mov    dword ptr [ebp - 4], 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    SHORT loc_1_4AA5
loc_1_4A87:
        push   ecx
        mov    eax, ebx
        mov    ecx, 6
        sub    edx, edx
        div    ecx
        push   eax
        mov    edi, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   edi
        call   sub_1_4D58
loc_1_4AA2:
        mov    dword ptr [ebp - 4], eax
loc_1_4AA5:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_4AB5
        mov    dword ptr [esi + 24h], 0
        mov    dword ptr [esi + 20h], ebx
loc_1_4AB5:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 08Dh, 064h, 024h, 000h
sub_1_4AD8:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 8], 0
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        lea    edx, [ebp - 0ch]
        mov    eax, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esi + 18h]
        push   edx
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 10h], ecx
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 14h]
        push   eax
        mov    edx, dword ptr [ebp - 10h]
        push   ecx
        mov    eax, dword ptr [esi + 14h]
        push   edx
        mov    ebx, dword ptr [ebp + 8]
        push   1
        push   eax
        push   0
        push   ebx
        call   sub_1_42A8
        cmp    eax, 0c00000bbh
        mov    edi, eax
        jne    NEAR PTR loc_1_4C7E
        xor    edi, edi
        push   ebx
        call   sub_1_41B8
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 2020dh
        ja     SHORT loc_1_4B5C
        je     NEAR PTR loc_1_4BF7
        sub    eax, 20101h
        cmp    eax, 4
        ja     SHORT loc_1_4B8A
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_4B5C:
        cmp    eax, 1020201h
        ja     SHORT loc_1_4B94
        je     NEAR PTR loc_1_4C17
        cmp    eax, 1020101h
        je     NEAR PTR loc_1_4BFF
        cmp    eax, 1020102h
        je     NEAR PTR loc_1_4C07
        cmp    eax, 1020103h
        je     NEAR PTR loc_1_4C0F
loc_1_4B8A:
        mov    edi, 0c0010017h
        jmp    NEAR PTR loc_1_4C50
loc_1_4B94:
        sub    eax, 1020202h
        cmp    eax, 5
        ja     SHORT loc_1_4B8A
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_4BA5:
        mov    eax, dword ptr [ebx + 1b8h]
        jmp    NEAR PTR loc_1_4C4D
loc_1_4BB0:
        mov    eax, dword ptr [ebx + 1bch]
        jmp    NEAR PTR loc_1_4C4D
loc_1_4BBB:
        mov    eax, dword ptr [ebx + 1ech]
        add    eax, dword ptr [ebx + 1dch]
        add    eax, dword ptr [ebx + 1cch]
        add    eax, dword ptr [ebx + 1f0h]
        jmp    SHORT loc_1_4C4D
loc_1_4BD5:
        mov    eax, dword ptr [ebx + 1f4h]
        add    eax, dword ptr [ebx + 1f8h]
        add    eax, dword ptr [ebx + 1fch]
        add    eax, dword ptr [ebx + 200h]
        jmp    SHORT loc_1_4C4D
loc_1_4BEF:
        mov    eax, dword ptr [ebx + 200h]
        jmp    SHORT loc_1_4C4D
loc_1_4BF7:
        mov    eax, dword ptr [ebx + 1f8h]
        jmp    SHORT loc_1_4C4D
loc_1_4BFF:
        mov    eax, dword ptr [ebx + 1f4h]
        jmp    SHORT loc_1_4C4D
loc_1_4C07:
        mov    eax, dword ptr [ebx + 1d4h]
        jmp    SHORT loc_1_4C4D
loc_1_4C0F:
        mov    eax, dword ptr [ebx + 1d8h]
        jmp    SHORT loc_1_4C4D
loc_1_4C17:
        mov    eax, dword ptr [ebx + 1e8h]
        jmp    SHORT loc_1_4C4D
loc_1_4C1F:
        mov    eax, dword ptr [ebx + 1dch]
        jmp    SHORT loc_1_4C4D
loc_1_4C27:
        mov    eax, dword ptr [ebx + 200h]
        jmp    SHORT loc_1_4C4D
loc_1_4C2F:
        mov    eax, dword ptr [ebx + 1cch]
        jmp    SHORT loc_1_4C4D
loc_1_4C37:
        mov    eax, dword ptr [ebx + 1d0h]
        jmp    SHORT loc_1_4C4D
loc_1_4C3F:
        mov    eax, dword ptr [ebx + 1e0h]
        jmp    SHORT loc_1_4C4D
loc_1_4C47:
        mov    eax, dword ptr [ebx + 1e4h]
loc_1_4C4D:
        mov    dword ptr [ebp - 4], eax
loc_1_4C50:
        push   ebx
        call   sub_1_41D8
        test   edi, edi
        jne    SHORT loc_1_4C7E
        cmp    dword ptr [ebp - 14h], 4
        jb     SHORT loc_1_4C75
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        call   sub_1_54BC
        add    dword ptr [ebp - 0ch], 4
        jmp    SHORT loc_1_4C7E
loc_1_4C75:
        add    dword ptr [ebp - 8], 4
        mov    edi, 0c0010014h
loc_1_4C7E:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], ecx
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 08Dh
        db  064h, 024h, 000h
loc_1_4CC8:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 14h]
        cmp    ecx, 20105h
        ja     SHORT loc_1_4CFB
        cmp    ecx, 20101h
        jae    SHORT loc_1_4D45
        cmp    ecx, 10101h
        jb     SHORT loc_1_4D3C
        cmp    ecx, 10111h
        jbe    SHORT loc_1_4D45
        mov    eax, 0c0010017h
        pop    ebp
        ret    8
loc_1_4CFB:
        cmp    ecx, 1010104h
        ja     SHORT loc_1_4D1C
        cmp    ecx, 1010101h
        jae    SHORT loc_1_4D45
        cmp    ecx, 2020dh
        je     SHORT loc_1_4D45
        mov    eax, 0c0010017h
        pop    ebp
        ret    8
loc_1_4D1C:
        cmp    ecx, 1020207h
        ja     SHORT loc_1_4D3C
        cmp    ecx, 1020201h
        jae    SHORT loc_1_4D45
        cmp    ecx, 1020101h
        jb     SHORT loc_1_4D3C
        cmp    ecx, 1020103h
        jbe    SHORT loc_1_4D45
loc_1_4D3C:
        mov    eax, 0c0010017h
        pop    ebp
        ret    8
loc_1_4D45:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4AD8
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
