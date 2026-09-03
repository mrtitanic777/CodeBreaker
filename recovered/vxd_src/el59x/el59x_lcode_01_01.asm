; el59x_lcode_01_01.asm -- EL59X object 1 segment _LTEXT  (part 1/3)
.386p
_LTEXT segment dword public use32 'LCODE'

loc_1_0000:
        db  0A0h, 018h, 091h, 000h, 000h
        cmp    al, 0
        jne    SHORT loc_1_0025
        db  0FEh, 005h, 018h, 091h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     SHORT loc_1_0027
        push   0
        push   0
        call   sub_1_3C58
        cmp    eax, 0
        jne    SHORT loc_1_0027
loc_1_0025:
        clc
        ret
loc_1_0027:
        stc
        ret
EL59X_Control:
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
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        mov    ecx, dword ptr [ebp + 8]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ecx + 17ah]
        in     al, dx
        test   al, 1
        je     SHORT loc_1_00F2
        mov    edx, dword ptr [ecx + 176h]
        mov    eax, 7000h
        out    dx, ax
        mov    eax, 6801h
        pop    ebp
        out    dx, ax
        mov    al, 1
        ret    4
loc_1_00F2:
        xor    al, al
        pop    ebp
        ret    4
sub_1_00F8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 0c2h], 1
        mov    edx, dword ptr [esi + 182h]
        in     al, dx
        mov    cl, al
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        and    ecx, 0ffff00ffh
        db  0A1h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 2], cx
        mov    edi, ecx
        and    edi, 0ffffh
        db  089h, 004h, 095h, 000h, 000h, 000h, 000h
        lea    eax, [edx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 03Ch, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
loc_1_0159:
        mov    edx, dword ptr [esi + 17ah]
        in     ax, dx
        and    eax, 0feh
        je     SHORT loc_1_01C7
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  089h, 00Ch, 095h, 000h, 000h, 000h, 000h
        lea    ecx, [edx + 1]
        and    ecx, 7ffh
        mov    edx, ebx
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 01Ch, 08Dh, 000h, 000h, 000h, 000h
        inc    ecx
        inc    ebx
        and    ecx, 7ffh
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        inc    ecx
        and    ecx, 7ffh
        cmp    edx, 0ah
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        jae    SHORT loc_1_01C7
        push   esi
        and    eax, 0fffffffeh
        call   dword ptr [esi + eax*2 + 3a6h]
        jmp    SHORT loc_1_0159
loc_1_01C7:
        imul   edi, edi
        mov    cx, word ptr [ebp - 2]
        add    edi, dword ptr [esi + 20ah]
        mov    ax, word ptr [esi + 206h]
        add    cx, word ptr [esi + 208h]
        mov    dword ptr [esi + 20ah], edi
        inc    ax
        mov    word ptr [esi + 208h], cx
        mov    word ptr [esi + 206h], ax
        cmp    ax, 100h
        jb     SHORT loc_1_021B
        mov    word ptr [esi + 206h], 80h
        shr    edi, 1
        shr    cx, 1
        mov    dword ptr [esi + 20ah], edi
        mov    word ptr [esi + 208h], cx
loc_1_021B:
        mov    byte ptr [esi + 0c2h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        cmp    byte ptr [esi + 0b7h], 0
        je     SHORT loc_1_0270
        mov    edx, dword ptr [esi + 176h]
        mov    eax, 70feh
        mov    byte ptr [esi + 0b7h], 0
        out    dx, ax
        mov    eax, dword ptr [esi + 5eh]
        push   eax
        call   sub_1_8D10
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0270:
        mov    edx, dword ptr [esi + 176h]
        mov    eax, 70feh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        out    dx, ax
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_0288:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0b6h], 0
        je     SHORT loc_1_02A7
        mov    byte ptr [esi + 0b6h], 0
        push   0
        push   esi
        call   sub_1_3668
loc_1_02A7:
        lea    ecx, [esi + 3a6h]
        xor    eax, eax
loc_1_02AF:
        test   al, 2
        je     SHORT loc_1_02BB
        db  0C7h, 001h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_0307
loc_1_02BB:
        test   al, 80h
        je     SHORT loc_1_02C7
        db  0C7h, 001h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_0307
loc_1_02C7:
        test   al, 20h
        je     SHORT loc_1_02D3
        db  0C7h, 001h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_0307
loc_1_02D3:
        test   al, 10h
        je     SHORT loc_1_02DF
        db  0C7h, 001h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_0307
loc_1_02DF:
        test   al, 4
        je     SHORT loc_1_02EB
        db  0C7h, 001h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_0307
loc_1_02EB:
        test   al, 8
        je     SHORT loc_1_02F7
        db  0C7h, 001h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_0307
loc_1_02F7:
        test   al, 40h
        db  0C7h, 001h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_0307
        db  0C7h, 001h, 000h, 000h, 000h, 000h
loc_1_0307:
        add    ecx, 4
        add    eax, 2
        cmp    eax, 100h
        jl     SHORT loc_1_02AF
        db  0C7h, 086h, 077h, 003h, 000h, 000h, 000h, 000h, 000h, 000h
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_0328:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_1_0348
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0348:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    eax, 806h
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 176h]
        mov    edi, dword ptr [esi + 172h]
        mov    edx, ecx
        out    dx, ax
        mov    eax, 0b000h
        out    dx, ax
        lea    edx, [edi + 1]
        xor    ebx, ebx
        in     al, dx
        xor    edx, edx
        mov    dl, al
        add    dword ptr [esi + 31ah], edx
        lea    edx, [edi + 2]
        in     al, dx
        mov    bl, al
        lea    edx, [edi + 3]
        in     al, dx
        xor    edx, edx
        add    dword ptr [esi + 322h], ebx
        mov    dl, al
        xor    ebx, ebx
        add    dword ptr [esi + 31eh], edx
        lea    edx, [edi + 5]
        in     al, dx
        mov    bl, al
        lea    edx, [edi + 0ch]
        in     ax, dx
        lea    edx, [edi + 0ah]
        add    dword ptr [esi + 34ah], ebx
        mov    ebx, eax
        in     ax, dx
        and    ebx, 0ffffh
        mov    edx, eax
        and    edx, 0ffffh
        add    dword ptr [esi + 30eh], ebx
        xor    ebx, ebx
        add    dword ptr [esi + 312h], edx
        mov    edx, edi
        in     al, dx
        mov    bl, al
        lea    edx, [edi + 4]
        in     al, dx
        xor    edx, edx
        add    dword ptr [esi + 32ah], ebx
        mov    dl, al
        xor    ebx, ebx
        add    dword ptr [esi + 32eh], edx
        lea    edx, [edi + 8]
        in     al, dx
        mov    bl, al
        lea    edx, [edi + 7]
        in     al, dx
        add    dword ptr [esi + 332h], ebx
        xor    edx, edx
        mov    dl, al
        add    dword ptr [esi + 306h], edx
        lea    edx, [edi + 6]
        in     al, dx
        mov    bl, al
        lea    edx, [edi + 9]
        in     al, dx
        and    eax, 3
        shl    eax, 8
        add    dword ptr [esi + 306h], eax
        xor    eax, eax
        mov    al, bl
        add    dword ptr [esi + 302h], eax
        cmp    byte ptr [esi + 0b8h], 0
        je     SHORT loc_1_0431
        add    dword ptr [esi + 31ah], eax
loc_1_0431:
        mov    edx, ecx
        mov    eax, 804h
        out    dx, ax
        lea    edx, [edi + 0ch]
        in     al, dx
        and    eax, 0ffh
        mov    edx, ecx
        pop    edi
        add    dword ptr [esi + 34eh], eax
        mov    eax, 0a800h
        pop    esi
        out    dx, ax
        mov    eax, 807h
        pop    ebx
        out    dx, ax
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0468:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   esi
        mov    esi, dword ptr [ebp + 8]
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, 6840h
        mov    byte ptr [esi + 0bfh], 1
        mov    edx, dword ptr [esi + 176h]
        cmp    byte ptr [esi + 0c0h], 0
        out    dx, ax
        je     SHORT loc_1_04D4
        push   esi
        call   sub_1_3718
        mov    byte ptr [esi + 0b6h], 0
        mov    byte ptr [esi + 0c0h], 0
        push   esi
        call   sub_1_7A98
        mov    ax, word ptr [esi + 0ceh]
        mov    edx, dword ptr [esi + 176h]
        out    dx, ax
loc_1_04D4:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_04E8:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, 804h
        mov    edi, dword ptr [esi + 176h]
        mov    edx, edi
        out    dx, ax
        mov    edx, dword ptr [esi + 172h]
        add    edx, 4
        in     ax, dx
        mov    cx, ax
        mov    edx, edi
        mov    eax, 807h
        out    dx, ax
        mov    eax, ecx
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        and    eax, 0ffffh
        db  089h, 004h, 095h, 000h, 000h, 000h, 000h
        lea    eax, [edx + 1]
        and    eax, 7ffh
        test   ch, 4
        db  0A3h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_0595
        push   esi
        call   sub_1_1BC8
        cmp    dword ptr [esi + 10eh], 0
        je     SHORT loc_1_05C5
        mov    eax, dword ptr [esi + 112h]
        mov    edx, dword ptr [esi + 176h]
        pop    edi
        mov    dword ptr [esi + 116h], eax
        mov    byte ptr [esi + 126h], 0
        add    eax, 3
        pop    esi
        shr    eax, 2
        pop    ebp
        sub    ax, 7000h
        out    dx, ax
        ret    4
loc_1_0595:
        mov    edi, dword ptr [esi + 176h]
        mov    eax, 2800h
        mov    edx, edi
        mov    ecx, dword ptr [esi + 17ah]
        out    dx, ax
        mov    edx, ecx
loc_1_05AC:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_05AC
        mov    edx, edi
        mov    eax, 5800h
        out    dx, ax
        mov    edx, ecx
loc_1_05BE:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_05BE
loc_1_05C5:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_05D8:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        db  089h, 00Ch, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        ret    4
loc_1_05F8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi]
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        lea    ecx, [ebp - 4]
        push   ecx
        mov    ebx, dword ptr [ebp + 0ch]
        push   0
        db  0A3h, 000h, 000h, 000h, 000h
        push   0
        push   0
        push   ebx
        call   sub_1_8FB0
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 28h], edi
        mov    word ptr [ebx + 2ch], cx
        cmp    ecx, 0eh
        mov    byte ptr [ebx + 2eh], 0
        jb     NEAR PTR loc_1_0812
        cmp    dword ptr [esi + 156h], ecx
        jb     NEAR PTR loc_1_0812
        cmp    byte ptr [edi + 20h], 0
        je     SHORT loc_1_066F
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_066F:
        cmp    byte ptr [esi + 83h], 0
        je     SHORT loc_1_0686
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0686:
        inc    dword ptr [edi + 1ch]
        test   byte ptr [esi + 21h], 0ah
        mov    dword ptr [ebx + 24h], 0
        je     SHORT loc_1_06D7
        cmp    dword ptr [esi + 15h], 0
        jne    SHORT loc_1_06A1
        mov    dword ptr [esi + 15h], ebx
        jmp    SHORT loc_1_06A7
loc_1_06A1:
        mov    eax, dword ptr [esi + 11h]
        mov    dword ptr [eax + 24h], ebx
loc_1_06A7:
        mov    dword ptr [esi + 11h], ebx
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        mov    esp, ebp
        and    eax, 7ffh
        pop    ebp
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, 103h
        ret    8
loc_1_06D7:
        mov    dword ptr [esi + 15h], ebx
        mov    dword ptr [esi + 11h], ebx
        push   ebx
        or     byte ptr [esi + 21h], 8
        mov    eax, dword ptr [esi + 0ceh]
        push   esi
        and    eax, 0fffffffbh
        mov    dword ptr [esi + 0ceh], eax
        mov    edx, dword ptr [esi + 176h]
        out    dx, ax
        call   sub_1_08C8
        test   eax, eax
        je     SHORT loc_1_0747
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi + 0ceh]
        or     eax, 4
        pop    edi
        mov    dword ptr [esi + 0ceh], eax
        mov    edx, dword ptr [esi + 176h]
        pop    esi
        out    dx, ax
        mov    eax, 103h
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0747:
        mov    eax, dword ptr [esi + 15h]
        mov    ebx, dword ptr [eax + 24h]
        mov    dword ptr [esi + 15h], ebx
        test   ebx, ebx
        je     SHORT loc_1_078C
loc_1_0754:
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        call   sub_1_08C8
        test   eax, eax
        jne    SHORT loc_1_07E1
        push   ebx
        push   esi
        call   sub_1_0828
        mov    ebx, dword ptr [esi + 15h]
        test   ebx, ebx
        jne    SHORT loc_1_0754
loc_1_078C:
        and    byte ptr [esi + 21h], 0f7h
        mov    eax, dword ptr [edi + 1ch]
        dec    eax
        mov    dword ptr [edi + 1ch], eax
        jne    SHORT loc_1_079F
        push   edi
        call   sub_1_79E8
loc_1_079F:
        mov    eax, dword ptr [esi + 0ceh]
        or     eax, 4
        mov    dword ptr [esi + 0ceh], eax
        mov    edx, dword ptr [esi + 176h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        out    dx, ax
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        pop    edi
        pop    esi
        db  089h, 014h, 08Dh, 000h, 000h, 000h, 000h
        pop    ebx
        db  0A3h, 000h, 000h, 000h, 000h
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_07E1:
        mov    eax, dword ptr [edi + 1ch]
        dec    eax
        mov    dword ptr [edi + 1ch], eax
        jne    SHORT loc_1_07F0
        push   edi
        call   sub_1_79E8
loc_1_07F0:
        mov    eax, dword ptr [esi + 0ceh]
        pop    edi
        or     eax, 4
        mov    dword ptr [esi + 0ceh], eax
        mov    edx, dword ptr [esi + 176h]
        pop    esi
        out    dx, ax
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0812:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0828:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 0ch]
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [edx + 2eh]
        test   al, 2
        jne    SHORT loc_1_085A
        xor    edi, edi
        jmp    SHORT loc_1_086C
loc_1_085A:
        and    al, 8
        cmp    al, 1
        sbb    edi, edi
        and    edi, 0fffefff5h
        sub    edi, 3ffefff4h
loc_1_086C:
        mov    ecx, dword ptr [ebp + 8]
        push   edi
        push   edx
        mov    eax, dword ptr [ecx + 15h]
        mov    esi, dword ptr [eax + 24h]
        mov    dword ptr [ecx + 15h], esi
        mov    esi, dword ptr [edx + 28h]
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_8ECA
        mov    eax, dword ptr [esi + 1ch]
        dec    eax
        mov    dword ptr [esi + 1ch], eax
        jne    SHORT loc_1_0895
        push   esi
        call   sub_1_79E8
loc_1_0895:
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebp
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_08C8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 20h
        mov    ecx, dword ptr [eax + 192h]
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    esi, dword ptr [eax + 2eah]
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 20h], ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 1], bl
        xor    edx, edx
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        mov    ecx, dword ptr [ebp + 0ch]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        inc    eax
        mov    dx, word ptr [ecx + 2ch]
        push   ebx
        lea    ecx, [ebp - 18h]
        mov    dword ptr [ebp - 1ch], edx
        push   ecx
        db  089h, 014h, 085h, 0FCh, 0FFh, 0FFh, 0FFh
        and    eax, 7ffh
        push   ebx
        push   ebx
        mov    edx, dword ptr [ebp + 0ch]
        push   edx
        db  0A3h, 000h, 000h, 000h, 000h
        call   sub_1_8FB0
        lea    ecx, [ebp - 0ch]
        lea    edx, [ebp - 14h]
        push   ecx
        mov    eax, dword ptr [ebp - 18h]
        push   edx
        push   eax
        call   sub_1_8FD4
        cmp    dword ptr [ebp - 0ch], 6
        jl     SHORT loc_1_0998
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 82h], bl
        jne    SHORT loc_1_096D
        mov    eax, dword ptr [ebp - 14h]
        test   byte ptr [eax], 1
        jne    SHORT loc_1_096D
        mov    ecx, eax
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 40h]
        cmp    dword ptr [ecx + 2], edx
        jne    SHORT loc_1_097E
loc_1_096D:
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        xor    ebx, ebx
        push   ecx
        call   sub_1_7BC8
        mov    bl, al
loc_1_097E:
        test   ebx, ebx
        je     SHORT loc_1_0998
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        inc    dword ptr [eax + 30ah]
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3358
loc_1_0998:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 186h]
        in     ax, dx
        mov    ecx, eax
        db  0A1h, 000h, 000h, 000h, 000h
        and    ecx, 0ffffh
        inc    eax
        mov    dword ptr [ebp - 10h], ecx
        db  089h, 00Ch, 085h, 0FCh, 0FFh, 0FFh, 0FFh
        and    eax, 7ffh
        cmp    ecx, 8
        db  0A3h, 000h, 000h, 000h, 000h
        jge    NEAR PTR loc_1_0A5C
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp - 1ch]
        mov    dword ptr [eax + 10eh], ecx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 112h], edx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 116h], edx
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 126h], 0
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        db  089h, 01Ch, 08Dh, 000h, 000h, 000h, 000h
        and    eax, 7ffh
        mov    ecx, dword ptr [ebp + 8]
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 014h, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, 9fffh
        mov    edx, dword ptr [ecx + 176h]
        mov    ecx, dword ptr [ebp - 1ch]
        out    dx, ax
        add    ecx, 3
        pop    edi
        sar    ecx, 2
        pop    esi
        sub    cx, 7000h
        pop    ebx
        mov    ax, cx
        mov    esp, ebp
        out    dx, ax
        mov    eax, 1
        pop    ebp
        ret    8
loc_1_0A5C:
        cmp    esi, dword ptr [ebp - 1ch]
        jge    SHORT loc_1_0A83
        mov    eax, dword ptr [ebp - 1ch]
        add    eax, 4
        cmp    eax, dword ptr [ebp - 10h]
        jge    SHORT loc_1_0A83
        mov    eax, dword ptr [ebp + 8]
        add    eax, 29eh
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_0A83
        mov    byte ptr [ebp - 1], 1
        mov    dword ptr [eax], 1
loc_1_0A83:
        mov    ebx, dword ptr [ebp - 1ch]
        add    ebx, 100h
        and    ebx, 600h
        shr    ebx, 9
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_0ABC
        shl    ebx, 4
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [ebx + eax + 264h]
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 176h]
        mov    ax, word ptr [ebx + eax + 25eh]
        jmp    SHORT loc_1_0ADB
loc_1_0ABC:
        shl    ebx, 4
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [ebx + eax + 224h]
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 176h]
        mov    ax, word ptr [ebx + eax + 21eh]
loc_1_0ADB:
        cmp    byte ptr [ebp - 1], 0
        out    dx, ax
        mov    edx, dword ptr [ebp - 20h]
        mov    eax, dword ptr [ebp - 1ch]
        out    dx, eax
        je     NEAR PTR loc_1_0BDB
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        push   ecx
        push   0
        push   0
        push   eax
        call   sub_1_1E68
loc_1_0AFF:
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_1_0B5E
        mov    eax, dword ptr [ebp - 1ch]
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        neg    eax
        and    eax, 3
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  089h, 014h, 08Dh, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 10h]
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 014h, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        test   edx, edx
        db  0A3h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_0B58
        mov    eax, esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp - 10h]
        rep outsb
        mov    esi, eax
loc_1_0B58:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_0B6B
loc_1_0B5E:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 29eh], 0
loc_1_0B6B:
        sti
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 17eh]
        in     ax, dx
        mov    ecx, eax
        and    ecx, 0ffffh
        test   cl, 10h
        mov    dword ptr [ebp - 10h], ecx
        je     NEAR PTR loc_1_0D15
        push   5
        call   sub_1_8F80
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        mov    ecx, dword ptr [ebp + 8]
        db  0A3h, 000h, 000h, 000h, 000h
        inc    dword ptr [ecx + 316h]
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_1BC8
        cmp    byte ptr [ebp - 1], 0
        je     NEAR PTR loc_1_0CF3
        add    ebx, dword ptr [ebp + 8]
        add    ebx, 25eh
        jmp    NEAR PTR loc_1_0CFC
loc_1_0BDB:
        cli
        mov    edi, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [ebp + 8]
        sub    dword ptr [ebp - 10h], 4
        mov    eax, dword ptr [eax + 0d6h]
        mov    ecx, dword ptr [ebp - 10h]
        cmp    eax, ecx
        jl     SHORT loc_1_0BF5
loc_1_0BF3:
        mov    eax, ecx
loc_1_0BF5:
        mov    dword ptr [ebp - 8], eax
loc_1_0BF8:
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 8], eax
        jl     SHORT loc_1_0C65
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        mov    ecx, dword ptr [ebp - 0ch]
        sub    edi, ecx
        db  0A3h, 000h, 000h, 000h, 000h
        sub    dword ptr [ebp - 8], ecx
        push   esi
        mov    esi, dword ptr [ebp - 14h]
        mov    edx, dword ptr [ebp - 20h]
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd
        and    eax, 3
        mov    ecx, eax
        rep outsb
        pop    esi
        lea    eax, [ebp - 18h]
        mov    ecx, dword ptr [ebp - 18h]
        push   eax
        push   ecx
        call   sub_1_8FF8
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_1_0AFF
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 14h]
        push   eax
        mov    edx, dword ptr [ebp - 18h]
        push   ecx
        push   edx
        call   sub_1_8FD4
        jmp    SHORT loc_1_0BF8
loc_1_0C65:
        mov    eax, dword ptr [ebp - 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        push   esi
        mov    esi, dword ptr [ebp - 14h]
        mov    edx, dword ptr [ebp - 20h]
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd
        and    eax, 3
        mov    ecx, eax
        rep outsb
        pop    esi
        mov    eax, dword ptr [ebp - 8]
        sub    edi, eax
        sub    dword ptr [ebp - 0ch], eax
        add    dword ptr [ebp - 14h], eax
        sti
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 186h]
        in     ax, dx
        and    eax, 0ffffh
        sub    eax, 4
        mov    dword ptr [ebp - 10h], eax
        cli
        mov    ecx, dword ptr [ebp - 10h]
        db  0A1h, 000h, 000h, 000h, 000h
        db  089h, 00Ch, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        cmp    ecx, 8
        db  0A3h, 000h, 000h, 000h, 000h
        jl     SHORT loc_1_0D3F
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d6h]
        cmp    eax, ecx
        jl     NEAR PTR loc_1_0BF5
        jmp    NEAR PTR loc_1_0BF3
loc_1_0CF3:
        add    ebx, dword ptr [ebp + 8]
        add    ebx, 21eh
loc_1_0CFC:
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1AB8
        push   1
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0DF8
loc_1_0D15:
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        mov    esp, ebp
        and    eax, 7ffh
        pop    ebp
        db  0A3h, 000h, 000h, 000h, 000h
        xor    eax, eax
        ret    8
loc_1_0D3F:
        sti
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp - 1ch]
        mov    dword ptr [eax + 10eh], ecx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 112h], edx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 116h], edi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 18h]
        mov    dword ptr [eax + 11ah], edx
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp - 14h]
        mov    dword ptr [eax + 11eh], ebx
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 122h], edx
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 126h], 1
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        db  089h, 014h, 08Dh, 000h, 000h, 000h, 000h
        and    eax, 7ffh
        mov    ecx, dword ptr [ebp + 8]
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 03Ch, 085h, 000h, 000h, 000h, 000h
        inc    eax
        add    edi, 3
        sar    edi, 2
        and    eax, 7ffh
        sub    di, 7000h
        db  0A3h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ecx + 176h]
        mov    eax, 9fffh
        out    dx, ax
        mov    ax, di
        pop    edi
        out    dx, ax
        mov    eax, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0DF8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 28h
        mov    ecx, dword ptr [eax + 2eah]
        mov    edx, dword ptr [eax + 192h]
        push   ebx
        mov    dword ptr [ebp - 24h], ecx
        push   esi
        xor    ecx, ecx
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 1ch], edx
        mov    byte ptr [ebp - 1], cl
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        xor    ebx, ebx
        db  089h, 004h, 095h, 000h, 000h, 000h, 000h
        lea    eax, [edx + 1]
        and    eax, 7ffh
        mov    edx, dword ptr [ebp + 0ch]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    bx, word ptr [edx + 2ch]
        mov    edx, dword ptr [eax + 186h]
        in     ax, dx
        mov    edx, eax
        mov    dword ptr [ebp - 20h], ebx
        and    edx, 0ffffh
        db  0A1h, 000h, 000h, 000h, 000h
        db  089h, 014h, 085h, 000h, 000h, 000h, 000h
        inc    eax
        mov    dword ptr [ebp - 14h], edx
        and    eax, 7ffh
        cmp    edx, 8
        db  0A3h, 000h, 000h, 000h, 000h
        jge    NEAR PTR loc_1_0F0A
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 10eh], edx
        mov    edx, dword ptr [ebp - 20h]
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 112h], edx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 116h], edx
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 126h], cl
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  089h, 01Ch, 08Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 014h, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, 9fffh
        mov    edx, dword ptr [ecx + 176h]
        mov    ecx, dword ptr [ebp - 20h]
        out    dx, ax
        add    ecx, 3
        pop    edi
        sar    ecx, 2
        pop    esi
        sub    cx, 7000h
        pop    ebx
        mov    ax, cx
        mov    esp, ebp
        out    dx, ax
        mov    eax, 1
        pop    ebp
        ret    0ch
loc_1_0F0A:
        push   0
        lea    eax, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 0ch]
        push   0
        push   0
        push   edx
        call   sub_1_8FB0
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 18h]
        push   ecx
        mov    eax, dword ptr [ebp - 0ch]
        push   edx
        push   eax
        call   sub_1_8FD4
        mov    eax, dword ptr [ebp - 20h]
        sub    eax, dword ptr [ebp - 8]
        cmp    eax, dword ptr [ebp - 24h]
        jle    SHORT loc_1_0F5B
        mov    eax, dword ptr [ebp - 20h]
        add    eax, 4
        cmp    eax, dword ptr [ebp - 14h]
        jge    SHORT loc_1_0F5B
        mov    eax, dword ptr [ebp + 8]
        add    eax, 29eh
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_0F5B
        mov    byte ptr [ebp - 1], 1
        mov    dword ptr [eax], 1
loc_1_0F5B:
        cmp    byte ptr [ebp + 10h], 0
        jne    SHORT loc_1_0FC1
        mov    eax, dword ptr [ebp - 20h]
        add    eax, 100h
        and    eax, 600h
        shr    eax, 9
        cmp    byte ptr [ebp - 1], 0
        mov    dword ptr [ebp - 28h], eax
        je     SHORT loc_1_0F9D
        mov    esi, eax
        mov    eax, dword ptr [ebp + 8]
        shl    esi, 4
        inc    word ptr [eax + esi + 264h]
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 176h]
        mov    ax, word ptr [eax + esi + 25eh]
        jmp    SHORT loc_1_0FBF
loc_1_0F9D:
        mov    esi, dword ptr [ebp - 28h]
        mov    eax, dword ptr [ebp + 8]
        shl    esi, 4
        inc    word ptr [eax + esi + 224h]
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 176h]
        mov    ax, word ptr [eax + esi + 21eh]
loc_1_0FBF:
        out    dx, ax
loc_1_0FC1:
        cli
        mov    edx, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [ebp - 20h]
        out    dx, eax
        sub    dword ptr [ebp - 14h], 4
        mov    edi, eax
        mov    ecx, dword ptr [ebp - 14h]
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d6h]
        cmp    eax, ecx
        jl     SHORT loc_1_0FE1
        mov    eax, ecx
loc_1_0FE1:
        xor    ebx, ebx
        mov    dword ptr [ebp - 10h], eax
loc_1_0FE6:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_1050
        mov    eax, dword ptr [ebp - 24h]
        cmp    dword ptr [ebp - 8], eax
        jle    SHORT loc_1_1050
        mov    eax, dword ptr [ebp - 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   0
        mov    edx, dword ptr [ebp + 8]
        push   0
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        push   ebx
        and    eax, 7ffh
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        db  0A3h, 000h, 000h, 000h, 000h
        push   edx
        call   sub_1_1C18
        mov    ebx, dword ptr [ebp - 0ch]
        sub    edi, dword ptr [ebp - 8]
        lea    eax, [ebp - 0ch]
        push   eax
        push   ebx
        call   sub_1_8FF8
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_115A
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 18h]
        push   eax
        mov    edx, dword ptr [ebp - 0ch]
        push   ecx
        push   edx
        call   sub_1_8FD4
        jmp    SHORT loc_1_0FE6
loc_1_1050:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 10h], eax
        jl     SHORT loc_1_10C0
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        mov    ecx, dword ptr [ebp - 8]
        sub    edi, ecx
        db  0A3h, 000h, 000h, 000h, 000h
        sub    dword ptr [ebp - 10h], ecx
        push   esi
        mov    esi, dword ptr [ebp - 18h]
        mov    edx, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd
        and    eax, 3
        mov    ecx, eax
        rep outsb
        pop    esi
        lea    eax, [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        push   eax
        push   ecx
        call   sub_1_8FF8
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_115A
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 18h]
        push   eax
        mov    edx, dword ptr [ebp - 0ch]
        push   ecx
        push   edx
        call   sub_1_8FD4
        jmp    NEAR PTR loc_1_0FE6
loc_1_10C0:
        mov    eax, dword ptr [ebp - 10h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        push   esi
        mov    esi, dword ptr [ebp - 18h]
        mov    edx, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd
        and    eax, 3
        mov    ecx, eax
        rep outsb
        pop    esi
        mov    eax, dword ptr [ebp - 10h]
        sub    edi, eax
        sub    dword ptr [ebp - 8], eax
        add    dword ptr [ebp - 18h], eax
        sti
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 186h]
        in     ax, dx
        and    eax, 0ffffh
        sub    eax, 4
        mov    dword ptr [ebp - 14h], eax
        cli
        mov    eax, dword ptr [ebp - 14h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        cmp    dword ptr [ebp - 14h], 8
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        jl     NEAR PTR loc_1_128C
        mov    eax, dword ptr [eax + 0d6h]
        mov    ecx, dword ptr [ebp - 14h]
        cmp    eax, ecx
        jl     SHORT loc_1_1152
        mov    eax, ecx
loc_1_1152:
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_0FE6
loc_1_115A:
        mov    eax, dword ptr [ebp - 20h]
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        neg    eax
        and    eax, 3
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 14h], eax
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  089h, 014h, 08Dh, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 14h]
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 014h, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        test   edx, edx
        db  0A3h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_11AD
        mov    eax, esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp - 14h]
        rep outsb
        mov    esi, eax
loc_1_11AD:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_11CE
        mov    eax, dword ptr [ebp + 8]
        test   ebx, ebx
        mov    dword ptr [eax + 29eh], 0
        je     SHORT loc_1_11CE
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_89E8
loc_1_11CE:
        sti
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 17eh]
        in     ax, dx
        mov    ecx, eax
        and    ecx, 0ffffh
        test   cl, 10h
        mov    dword ptr [ebp - 14h], ecx
        je     SHORT loc_1_1262
        push   5
        call   sub_1_8F80
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        mov    ecx, dword ptr [ebp + 8]
        db  0A3h, 000h, 000h, 000h, 000h
        inc    dword ptr [ecx + 316h]
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_1BC8
        cmp    byte ptr [ebp + 10h], 0
        jne    SHORT loc_1_1253
        cmp    byte ptr [ebp - 1], 0
        mov    eax, dword ptr [ebp - 28h]
        je     SHORT loc_1_123E
        shl    eax, 4
        add    eax, dword ptr [ebp + 8]
        add    eax, 25eh
        jmp    SHORT loc_1_1249
loc_1_123E:
        shl    eax, 4
        add    eax, dword ptr [ebp + 8]
        add    eax, 21eh
loc_1_1249:
        push   eax
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_1AB8
loc_1_1253:
        push   1
        mov    edx, dword ptr [ebp + 0ch]
        push   edx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0DF8
loc_1_1262:
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        mov    esp, ebp
        and    eax, 7ffh
        pop    ebp
        db  0A3h, 000h, 000h, 000h, 000h
        xor    eax, eax
        ret    0ch
loc_1_128C:
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 20h]
        mov    dword ptr [eax + 10eh], edx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 112h], ecx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 116h], edi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 11ah], edx
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp - 18h]
        mov    dword ptr [eax + 11eh], ebx
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [eax + 122h], edx
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 126h], 1
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        db  089h, 014h, 08Dh, 000h, 000h, 000h, 000h
        and    eax, 7ffh
        mov    ecx, dword ptr [ebp + 8]
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 03Ch, 085h, 000h, 000h, 000h, 000h
        inc    eax
        add    edi, 3
        sar    edi, 2
        and    eax, 7ffh
        sub    di, 7000h
        db  0A3h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ecx + 176h]
        mov    eax, 9fffh
        out    dx, ax
        mov    ax, di
        out    dx, ax
        sti
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1348:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 30h
        mov    ecx, dword ptr [eax + 2eah]
        mov    edx, dword ptr [eax + 192h]
        push   ebx
        mov    dword ptr [ebp - 28h], ecx
        push   esi
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        push   edi
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        mov    dword ptr [ebp - 24h], edx
        and    eax, 7ffh
        db  089h, 01Ch, 08Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 1], 0
        mov    esi, dword ptr [ecx + 10eh]
        cli
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 186h]
        in     ax, dx
        mov    ecx, eax
        db  0A1h, 000h, 000h, 000h, 000h
        and    ecx, 0ffffh
        inc    eax
        mov    dword ptr [ebp - 14h], ecx
        db  089h, 00Ch, 085h, 0FCh, 0FFh, 0FFh, 0FFh
        and    eax, 7ffh
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 112h
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ecx]
        cmp    byte ptr [eax + 126h], 0
        jne    NEAR PTR loc_1_1474
        cmp    dword ptr [ebp - 14h], 8
        jge    SHORT loc_1_1447
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  089h, 004h, 095h, 000h, 000h, 000h, 000h
        lea    eax, [edx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        inc    eax
        mov    ebx, dword ptr [ecx]
        db  089h, 01Ch, 085h, 0FCh, 0FFh, 0FFh, 0FFh
        and    eax, 7ffh
        mov    ebx, dword ptr [ebp + 8]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, 9fffh
        mov    ecx, dword ptr [ecx]
        mov    edx, dword ptr [ebx + 176h]
        add    ecx, 3
        out    dx, ax
        shr    ecx, 2
        sub    cx, 7000h
        mov    ax, cx
        out    dx, ax
        sti
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1447:
        mov    edx, dword ptr [ebp - 24h]
        mov    eax, ebx
        out    dx, eax
        push   0
        lea    eax, [ebp - 0ch]
        sub    dword ptr [ebp - 14h], 4
        push   eax
        push   0
        push   0
        push   esi
        call   sub_1_8FB0
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 18h]
        push   eax
        mov    edx, dword ptr [ebp - 0ch]
        push   ecx
        push   edx
        call   sub_1_8FD4
        jmp    SHORT loc_1_1492
loc_1_1474:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 11ah]
        mov    dword ptr [ebp - 0ch], ecx
        mov    edx, dword ptr [eax + 11eh]
        mov    dword ptr [ebp - 18h], edx
        mov    ecx, dword ptr [eax + 122h]
        mov    dword ptr [ebp - 8], ecx
loc_1_1492:
        mov    eax, dword ptr [ebp + 8]
        mov    edi, dword ptr [eax + 116h]
        mov    ecx, dword ptr [eax + 0d6h]
        mov    eax, dword ptr [ebp - 14h]
        cmp    ecx, eax
        jl     SHORT loc_1_14AA
        mov    ecx, eax
loc_1_14AA:
        mov    dword ptr [ebp - 10h], ecx
        cmp    dword ptr [ebp - 28h], edi
        jge    SHORT loc_1_14EB
        lea    eax, [edi + 4]
        cmp    eax, dword ptr [ebp - 14h]
        jge    SHORT loc_1_14EB
        mov    eax, dword ptr [ebp + 8]
        add    eax, 29eh
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_14EB
        lea    ecx, [ebp - 30h]
        mov    dword ptr [eax], 1
        lea    eax, [ebp - 2ch]
        mov    edx, dword ptr [ebp - 0ch]
        mov    byte ptr [ebp - 1], 1
        push   eax
        push   ecx
        push   edx
        call   sub_1_8FD4
        mov    eax, dword ptr [ebp - 2ch]
        sub    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 1ch], eax
loc_1_14EB:
        add    ebx, 100h
        and    ebx, 600h
        shr    ebx, 9
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_1521
        shl    ebx, 4
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + ebx + 264h]
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 176h]
        mov    ax, word ptr [eax + ebx + 25eh]
        jmp    SHORT loc_1_1540
loc_1_1521:
        shl    ebx, 4
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + ebx + 224h]
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 176h]
        mov    ax, word ptr [eax + ebx + 21eh]
loc_1_1540:
        mov    dword ptr [ebp - 20h], 0
        out    dx, ax
loc_1_1549:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_15C2
        mov    eax, dword ptr [ebp - 28h]
        cmp    eax, dword ptr [ebp - 8]
        jge    SHORT loc_1_15C2
        mov    eax, dword ptr [ebp - 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   0
        mov    edx, dword ptr [ebp - 20h]
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        mov    ecx, dword ptr [ebp - 1ch]
        push   ecx
        db  0A3h, 000h, 000h, 000h, 000h
        push   edx
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_1C18
        mov    ecx, dword ptr [ebp - 0ch]
        sub    edi, dword ptr [ebp - 8]
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 20h], ecx
        push   eax
        push   ecx
        call   sub_1_8FF8
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_16D3
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 18h]
        push   eax
        mov    edx, dword ptr [ebp - 0ch]
        push   ecx
        push   edx
        call   sub_1_8FD4
        mov    dword ptr [ebp - 1ch], 0
        jmp    SHORT loc_1_1549
loc_1_15C2:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 10h], eax
        jl     SHORT loc_1_1639
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        push   esi
        mov    esi, dword ptr [ebp - 18h]
        mov    edx, dword ptr [ebp - 24h]
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
        sub    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_8FF8
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_16D3
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 18h]
        push   eax
        mov    edx, dword ptr [ebp - 0ch]
        push   ecx
        push   edx
        call   sub_1_8FD4
        mov    dword ptr [ebp - 1ch], 0
        jmp    NEAR PTR loc_1_1549
loc_1_1639:
        mov    eax, dword ptr [ebp - 10h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        push   esi
        mov    esi, dword ptr [ebp - 18h]
        mov    edx, dword ptr [ebp - 24h]
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd
        and    eax, 3
        mov    ecx, eax
        rep outsb
        pop    esi
        mov    eax, dword ptr [ebp - 10h]
        sub    edi, eax
        sub    dword ptr [ebp - 8], eax
        add    dword ptr [ebp - 18h], eax
        sti
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 186h]
        in     ax, dx
        and    eax, 0ffffh
        sub    eax, 4
        mov    dword ptr [ebp - 14h], eax
        cli
        mov    eax, dword ptr [ebp - 14h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        cmp    dword ptr [ebp - 14h], 8
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        jl     NEAR PTR loc_1_1805
        mov    eax, dword ptr [eax + 0d6h]
        mov    ecx, dword ptr [ebp - 14h]
        cmp    eax, ecx
        jl     SHORT loc_1_16CB
        mov    eax, ecx
loc_1_16CB:
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_1549
loc_1_16D3:
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 112h]
        neg    eax
        and    eax, 3
        db  089h, 014h, 08Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 14h], eax
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        mov    edx, dword ptr [ebp - 14h]
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 014h, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        test   edx, edx
        db  0A3h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_172C
        mov    eax, esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 24h]
        mov    ecx, dword ptr [ebp - 14h]
        rep outsb
        mov    esi, eax
loc_1_172C:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_1752
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp - 20h], 0
        mov    dword ptr [eax + 29eh], 0
        je     SHORT loc_1_1752
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        call   sub_1_89E8
loc_1_1752:
        sti
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 17eh]
        in     ax, dx
        mov    ecx, eax
        and    ecx, 0ffffh
        test   cl, 10h
        mov    dword ptr [ebp - 14h], ecx
        je     SHORT loc_1_17DB
        push   5
        call   sub_1_8F80
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        mov    ecx, dword ptr [ebp + 8]
        db  0A3h, 000h, 000h, 000h, 000h
        inc    dword ptr [ecx + 316h]
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_1_1BC8
        cmp    byte ptr [ebp - 1], 0
        mov    eax, dword ptr [ebp + 8]
        je     SHORT loc_1_17B8
        add    eax, ebx
        add    eax, 25eh
        jmp    SHORT loc_1_17BF
loc_1_17B8:
        add    eax, ebx
        add    eax, 21eh
loc_1_17BF:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1AB8
        push   1
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 10eh]
        push   ecx
        push   eax
        call   sub_1_0DF8
loc_1_17DB:
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        mov    esp, ebp
        and    eax, 7ffh
        pop    ebp
        db  0A3h, 000h, 000h, 000h, 000h
        xor    eax, eax
        ret    4
loc_1_1805:
        mov    dword ptr [eax + 116h], edi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 11ah], edx
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp - 18h]
        mov    dword ptr [eax + 11eh], ebx
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [eax + 122h], edx
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 126h], 1
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        db  089h, 014h, 08Dh, 000h, 000h, 000h, 000h
        and    eax, 7ffh
        mov    ecx, dword ptr [ebp + 8]
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 03Ch, 085h, 000h, 000h, 000h, 000h
        inc    eax
        add    edi, 3
        sar    edi, 2
        and    eax, 7ffh
        sub    di, 7000h
        db  0A3h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ecx + 176h]
        mov    eax, 9fffh
        out    dx, ax
        mov    ax, di
        out    dx, ax
        sti
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_18A8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 17eh]
        in     al, dx
        out    dx, al
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        xor    ebx, ebx
        mov    bl, al
        db  089h, 00Ch, 095h, 000h, 000h, 000h, 000h
        lea    ecx, [edx + 1]
        and    ecx, 7ffh
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 01Ch, 08Dh, 000h, 000h, 000h, 000h
        inc    ecx
        and    ecx, 7ffh
        test   al, 10h
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_1_1938
        push   5
        call   sub_1_8F80
        push   esi
        inc    dword ptr [esi + 316h]
        call   sub_1_1BC8
        mov    eax, dword ptr [esi + 10eh]
        test   eax, eax
        jne    SHORT loc_1_1956
        lea    ebx, [esi + 25eh]
        mov    edi, 4
loc_1_191F:
        push   ebx
        push   esi
        add    ebx, 10h
        call   sub_1_1AB8
        lea    eax, [ebx - 50h]
        push   eax
        push   esi
        call   sub_1_1AB8
        dec    edi
        jne    SHORT loc_1_191F
        jmp    SHORT loc_1_1976
loc_1_1938:
        test   al, 20h
        je     NEAR PTR loc_1_19CF
        push   esi
        inc    dword ptr [esi + 336h]
        call   sub_1_1BC8
        mov    eax, dword ptr [esi + 10eh]
        test   eax, eax
        je     SHORT loc_1_1976
loc_1_1956:
        push   1
        push   eax
        push   esi
        call   sub_1_0DF8
        mov    eax, dword ptr [esi + 10eh]
        push   eax
        push   esi
        call   sub_1_0828
        mov    dword ptr [esi + 10eh], 0
loc_1_1976:
        mov    edi, dword ptr [esi + 15h]
        test   edi, edi
        jne    SHORT loc_1_1985
        and    byte ptr [esi + 21h], 0f7h
        test   edi, edi
        je     SHORT loc_1_19A9
loc_1_1985:
        push   0
        push   edi
        push   esi
        call   sub_1_0DF8
        cmp    eax, 1
        je     SHORT loc_1_19A9
        push   edi
        push   esi
        call   sub_1_0828
        mov    edi, dword ptr [esi + 15h]
        test   edi, edi
        jne    SHORT loc_1_1985
        and    byte ptr [esi + 21h], 0f7h
        test   edi, edi
        jne    SHORT loc_1_1985
loc_1_19A9:
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        pop    ebp
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        ret    4
loc_1_19CF:
        test   al, 8
        je     SHORT loc_1_19ED
        mov    edx, dword ptr [esi + 176h]
        mov    eax, 4800h
        pop    edi
        inc    dword ptr [esi + 326h]
        out    dx, ax
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_19ED:
        mov    edx, dword ptr [esi + 176h]
        mov    eax, 4800h
        pop    edi
        inc    dword ptr [esi + 33ah]
        out    dx, ax
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  090h
loc_1_1A08:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        push   edi
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, 6808h
        mov    edx, dword ptr [edi + 176h]
        out    dx, ax
        call   sub_1_1348
        cmp    eax, 1
        je     SHORT loc_1_1AB0
        mov    eax, dword ptr [edi + 10eh]
        push   eax
        push   edi
        call   sub_1_0828
        mov    dword ptr [edi + 10eh], 0
        mov    esi, dword ptr [edi + 15h]
        test   esi, esi
        jne    SHORT loc_1_1A6D
        and    byte ptr [edi + 21h], 0f7h
        test   esi, esi
        je     SHORT loc_1_1A91
loc_1_1A6D:
        push   0
        push   esi
        push   edi
        call   sub_1_0DF8
        cmp    eax, 1
        je     SHORT loc_1_1A91
        push   esi
        push   edi
        call   sub_1_0828
        mov    esi, dword ptr [edi + 15h]
        test   esi, esi
        jne    SHORT loc_1_1A6D
        and    byte ptr [edi + 21h], 0f7h
        test   esi, esi
        jne    SHORT loc_1_1A6D
loc_1_1A91:
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
loc_1_1AB0:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_1AB8:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        push   edi
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        mov    esi, dword ptr [ebp + 8]
        and    eax, 7ffh
        mov    ecx, dword ptr [ebp + 0ch]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    di, word ptr [ecx + 6]
        mov    word ptr [ecx + 6], 0
        mov    eax, dword ptr [esi + 21ah]
        mov    edx, edi
        and    edx, 0ffffh
        cmp    edx, eax
        jbe    SHORT loc_1_1B02
        mov    di, ax
loc_1_1B02:
        mov    dx, word ptr [ecx + 8]
        inc    dx
        mov    ax, word ptr [ecx + 0ah]
        add    ax, di
        mov    word ptr [ecx + 8], dx
        mov    word ptr [ecx + 0ah], ax
        cmp    dx, 4
        jb     SHORT loc_1_1B2D
        shr    dx, 1
        shr    ax, 1
        mov    word ptr [ecx + 8], dx
        mov    word ptr [ecx + 0ah], ax
loc_1_1B2D:
        xor    ebx, ebx
        xor    edi, edi
        mov    bx, word ptr [ecx + 4]
        mov    esi, dword ptr [ebp + 8]
        mov    di, word ptr [ecx + 2]
        mov    esi, dword ptr [esi + 216h]
        mov    ax, word ptr [ecx + 0ah]
        sub    edi, ebx
        sub    dx, dx
        div    word ptr [ecx + 8]
        and    eax, 0ffffh
        sub    esi, eax
        imul   esi, edi
        mov    eax, esi
        mov    esi, dword ptr [ebp + 8]
        cdq
        idiv   dword ptr [esi + 216h]
        mov    esi, eax
        test   esi, esi
        jl     SHORT loc_1_1B75
        cmp    esi, edi
        jle    SHORT loc_1_1B71
        mov    esi, edi
loc_1_1B71:
        add    edi, esi
        jmp    SHORT loc_1_1B87
loc_1_1B75:
        neg    esi
        mov    eax, edi
        cdq
        sub    eax, edx
        sar    eax, 1
        cmp    esi, eax
        jle    SHORT loc_1_1B85
        mov    esi, eax
loc_1_1B85:
        sub    edi, esi
loc_1_1B87:
        cmp    edi, 8
        jge    SHORT loc_1_1B91
        mov    edi, 8
loc_1_1B91:
        add    edi, ebx
        cmp    edi, ebx
        jge    SHORT loc_1_1B99
        mov    edi, ebx
loc_1_1B99:
        cmp    edi, 5eah
        jle    SHORT loc_1_1BA6
        mov    edi, 5eah
loc_1_1BA6:
        mov    word ptr [ecx + 2], di
        add    edi, 2
        sar    edi, 2
        sub    di, 6800h
        mov    word ptr [ecx], di
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1BC8:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   esi
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        mov    ecx, dword ptr [ebp + 8]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, 5840h
        mov    esi, dword ptr [ecx + 176h]
        mov    edx, esi
        out    dx, ax
        mov    edx, dword ptr [ecx + 17ah]
loc_1_1C03:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_1C03
        mov    edx, esi
        mov    eax, 4800h
        out    dx, ax
        pop    esi
        pop    ebp
        ret    4
sub_1_1C18:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 28h
        mov    ecx, dword ptr [eax + 192h]
        mov    edx, dword ptr [eax + 1feh]
        push   ebx
        mov    eax, dword ptr [ebp + 10h]
        push   esi
        mov    dword ptr [ebp - 24h], ecx
        push   edi
        mov    dword ptr [ebp - 20h], edx
        mov    dword ptr [ebp - 14h], 4
        test   eax, eax
        je     SHORT loc_1_1C4F
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_89E8
loc_1_1C4F:
        lea    eax, [ebp - 14h]
        mov    esi, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 2a2h
        push   eax
        push   1
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_8998
        lea    eax, [ebp - 28h]
        lea    ecx, [ebp - 1ch]
        push   eax
        push   ecx
        push   esi
        call   sub_1_8FD4
        mov    dword ptr [ebp - 10h], 0
        mov    edi, dword ptr [ebp + 14h]
        add    dword ptr [ebp - 1ch], edi
        cmp    dword ptr [ebp - 14h], 0
        jbe    NEAR PTR loc_1_1E54
        mov    eax, dword ptr [ebp + 8]
        add    eax, 2a6h
        mov    dword ptr [ebp - 18h], eax
loc_1_1C9B:
        mov    eax, dword ptr [ebp - 18h]
        mov    ebx, dword ptr [eax]
        cmp    edi, ebx
        jle    SHORT loc_1_1CEE
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        sub    edi, ebx
        db  0A1h, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 03Ch, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 01Ch, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_1E41
loc_1_1CEE:
        mov    eax, dword ptr [ebp - 18h]
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [eax - 4]
        db  0A1h, 000h, 000h, 000h, 000h
        db  089h, 014h, 085h, 000h, 000h, 000h, 000h
        inc    eax
        mov    dword ptr [ebp - 8], ecx
        and    eax, 7ffh
        add    dword ptr [ebp - 8], edi
        sub    ebx, edi
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 03Ch, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        xor    edi, edi
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 10h]
        sub    eax, dword ptr [ebp - 14h]
        cmp    eax, -1
        jne    SHORT loc_1_1D3B
        add    ebx, dword ptr [ebp + 18h]
loc_1_1D3B:
        mov    eax, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 14h]
        neg    eax
        and    eax, 0fh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  089h, 014h, 08Dh, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 8]
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 014h, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        mov    edx, dword ptr [ebp - 0ch]
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 01Ch, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 014h, 085h, 000h, 000h, 000h, 000h
        inc    eax
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 014h, 085h, 000h, 000h, 000h, 000h
        inc    eax
        mov    edx, dword ptr [ebp - 10h]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 014h, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        cmp    dword ptr [ebp - 0ch], 0
        db  0A3h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_1DF4
        push   esi
        mov    esi, dword ptr [ebp - 1ch]
        mov    edx, dword ptr [ebp - 24h]
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, eax
        shr    ecx, 2
        rep outsd
        and    eax, 3
        mov    ecx, eax
        rep outsb
        pop    esi
        sub    ebx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 0ch]
        add    dword ptr [ebp - 8], eax
loc_1_1DF4:
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 172h]
        mov    eax, dword ptr [ebp - 8]
        mov    edx, esi
        out    dx, eax
        mov    ax, bx
        lea    edx, [esi + 6]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp - 20h]
        mov    edx, dword ptr [eax + 176h]
        mov    eax, 0a001h
        out    dx, ax
        lea    edx, [esi + 0eh]
loc_1_1E21:
        mov    dword ptr [ebp - 4], ebx
        push   ecx
        mov    ecx, dword ptr [ebp - 4]
loc_1_1E28:
        xor    eax, eax
        dec    ecx
        jne    SHORT loc_1_1E28
        pop    ecx
        in     ax, dx
        test   ah, 8
        jne    SHORT loc_1_1E21
        lea    edx, [esi + 0ch]
        mov    eax, 1000h
        out    dx, ax
        in     ax, dx
loc_1_1E41:
        add    dword ptr [ebp - 18h], 8
        inc    dword ptr [ebp - 10h]
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp - 14h], eax
        ja     NEAR PTR loc_1_1C9B
loc_1_1E54:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1E68:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        mov    ecx, dword ptr [ebp + 14h]
        push   esi
        xor    ebx, ebx
        mov    bx, word ptr [ecx + 2ch]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, ebx
        sub    edi, dword ptr [ebp + 10h]
        mov    eax, dword ptr [esi + 1feh]
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 18h], eax
        mov    eax, ebx
        db  089h, 01Ch, 095h, 000h, 000h, 000h, 000h
        neg    eax
        and    eax, 3
        push   0
        mov    dword ptr [ebp - 14h], eax
        lea    eax, [edx + 1]
        and    eax, 7ffh
        lea    edx, [ebp - 8]
        push   edx
        db  0A3h, 000h, 000h, 000h, 000h
        push   0
        push   0
        push   ecx
        call   sub_1_8FB0
        lea    ecx, [ebp - 4]
        lea    edx, [ebp - 0ch]
        push   ecx
        mov    eax, dword ptr [ebp - 8]
        push   edx
        push   eax
        call   sub_1_8FD4
        mov    ecx, dword ptr [ebp + 0ch]
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi + ecx*4 + 2dah]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 00Ch, 095h, 000h, 000h, 000h, 000h
        lea    ecx, [edx + 1]
        and    ecx, 7ffh
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 10h]
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        add    eax, ecx
        db  089h, 00Ch, 095h, 000h, 000h, 000h, 000h
        inc    edx
        and    edx, 7ffh
        db  089h, 015h, 000h, 000h, 000h, 000h
        lea    ecx, [edx + 1]
        and    ecx, 7ffh
        db  089h, 004h, 095h, 000h, 000h, 000h, 000h
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 14h]
        add    ecx, edi
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  089h, 00Ch, 095h, 000h, 000h, 000h, 000h
        lea    ecx, [edx + 1]
        and    ecx, 7ffh
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [esi + 172h]
        out    dx, eax
        mov    ax, word ptr [ebp - 14h]
        add    edx, 6
        add    ax, di
        out    dx, ax
        cli
        mov    edi, dword ptr [esi + 39eh]
loc_1_1F6C:
        cmp    ebx, dword ptr [ebp - 4]
        jl     SHORT loc_1_1FAB
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0ch]
        push   eax
        push   ecx
        push   edi
        call   sub_1_906C
        sub    ebx, dword ptr [ebp - 4]
        add    edi, dword ptr [ebp - 4]
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_8FF8
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_1FB6
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_8FD4
        jmp    SHORT loc_1_1F6C
loc_1_1FAB:
        push   ebx
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        push   edi
        call   sub_1_906C
loc_1_1FB6:
        mov    edx, dword ptr [esi + 176h]
        mov    eax, 0a001h
        mov    edi, dword ptr [ebp - 18h]
        mov    esi, dword ptr [esi + 172h]
        out    dx, ax
        lea    edx, [esi + 0eh]
loc_1_1FCF:
        mov    dword ptr [ebp - 10h], edi
        push   ecx
        mov    ecx, dword ptr [ebp - 10h]
loc_1_1FD6:
        xor    eax, eax
        dec    ecx
        jne    SHORT loc_1_1FD6
        pop    ecx
        in     ax, dx
        test   ah, 8
        jne    SHORT loc_1_1FCF
        sti
        lea    edx, [esi + 0ch]
        mov    eax, 1000h
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        in     ax, dx
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2008:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [edi]
        db  089h, 004h, 095h, 000h, 000h, 000h, 000h
        lea    eax, [edx + 1]
        and    eax, 7ffh
        mov    edx, dword ptr [ebp + 14h]
        push   esi
        db  0A3h, 000h, 000h, 000h, 000h
        mov    word ptr [esi + 2eh], dx
        mov    eax, dword ptr [ebp + 10h]
        add    ax, 0eh
        mov    dword ptr [esi + 28h], edi
        mov    word ptr [esi + 2ch], ax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   ecx
        call   dword ptr [eax + 0ch]
        test   eax, eax
        je     SHORT loc_1_2066
        cmp    eax, 1
        je     SHORT loc_1_2076
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebp
        ret    18h
loc_1_2066:
        mov    ecx, dword ptr [esi + 30h]
        mov    eax, dword ptr [ebp + 1ch]
        pop    edi
        pop    esi
        pop    ebp
        mov    dword ptr [eax], ecx
        xor    eax, eax
        ret    18h
loc_1_2076:
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebp
        ret    18h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2088:
        push   ebp
        mov    ebp, esp
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], 0
        mov    esi, dword ptr [ebx + 1feh]
        mov    eax, dword ptr [ebx + 192h]
        mov    edx, dword ptr [ebx + 176h]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, 6820h
        cmp    byte ptr [ebx + 0b6h], 0
        out    dx, ax
        je     NEAR PTR loc_1_251F
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        xor    eax, eax
        mov    ax, word ptr [ebx + 0e2h]
        mov    edx, dword ptr [ebx + 176h]
        mov    byte ptr [ebx + 0b6h], 0
        out    dx, ax
        mov    ecx, dword ptr [ebx + 36fh]
        mov    edx, dword ptr [ebx + 18ah]
        in     ax, dx
        and    eax, 0ffffh
        mov    dword ptr [ebp - 0ch], ecx
        mov    ecx, dword ptr [ebx + 396h]
        mov    dword ptr [ebp - 8], eax
        add    ecx, dword ptr [ebp - 0ch]
        db  0A1h, 000h, 000h, 000h, 000h
        inc    eax
        mov    dword ptr [ebp - 10h], ecx
        mov    ecx, dword ptr [ebp - 8]
        and    dword ptr [ebp - 8], 1ffch
        db  089h, 00Ch, 085h, 0FCh, 0FFh, 0FFh, 0FFh
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebx + 29eh]
        test   eax, eax
        jne    SHORT loc_1_21B2
        mov    ecx, dword ptr [ebx + 2eeh]
        cmp    dword ptr [ebx + 0eah], ecx
        jbe    SHORT loc_1_21B2
        mov    edx, dword ptr [ebx + 172h]
        mov    eax, dword ptr [ebx + 2d2h]
        mov    cx, 5eah
        add    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebx + 29eh], 2
        mov    byte ptr [ebp - 1], 1
        out    dx, eax
        sub    cx, word ptr [ebp - 0ch]
        mov    ax, cx
        add    edx, 6
        out    dx, ax
        mov    edx, dword ptr [ebx + 176h]
        mov    eax, 0a000h
        out    dx, ax
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  089h, 004h, 095h, 000h, 000h, 000h, 000h
        lea    eax, [edx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_21E0
loc_1_21B2:
        cmp    eax, 2
        jne    SHORT loc_1_21DC
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 1], 1
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_21E0
loc_1_21DC:
        mov    byte ptr [ebp - 1], 0
loc_1_21E0:
        cmp    byte ptr [ebp - 1], 0
        je     NEAR PTR loc_1_2318
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebx + 172h]
        add    edx, 0eh
loc_1_2212:
        mov    dword ptr [ebp - 28h], esi
        push   ecx
        mov    ecx, dword ptr [ebp - 28h]
loc_1_2219:
        xor    eax, eax
        dec    ecx
        jne    SHORT loc_1_2219
        pop    ecx
        in     ax, dx
        test   ah, 8
        jne    SHORT loc_1_2212
        and    eax, 0ffffh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 014h, 085h, 000h, 000h, 000h, 000h
        inc    eax
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        and    eax, 7ffh
        mov    ecx, 5eah
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 014h, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebx + 172h]
        add    edx, 6
        in     ax, dx
        mov    edx, eax
        db  0A1h, 000h, 000h, 000h, 000h
        and    edx, 0ffffh
        inc    eax
        sub    ecx, edx
        mov    dword ptr [ebp - 0ch], ecx
        db  089h, 00Ch, 085h, 0FCh, 0FFh, 0FFh, 0FFh
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, 4000h
        mov    edx, dword ptr [ebx + 172h]
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        add    edx, 0ch
        out    dx, ax
        in     ax, dx
        mov    ecx, eax
        and    ecx, 0ffffh
        db  089h, 00Ch, 0B5h, 000h, 000h, 000h, 000h
        lea    ecx, [esi + 1]
        and    ecx, 7ffh
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 29eh], 0
        mov    edx, dword ptr [ebx + 18ah]
        in     ax, dx
        and    eax, 0ffffh
        mov    dword ptr [ebp - 8], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        db  089h, 00Ch, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_23F5
loc_1_2318:
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
loc_1_2337:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        inc    eax
        add    dword ptr [ebp - 0ch], ecx
        mov    dword ptr [ebp - 24h], ecx
        db  089h, 00Ch, 085h, 0FCh, 0FFh, 0FFh, 0FFh
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, edi
        mov    edi, dword ptr [ebp - 10h]
        mov    edx, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 24h]
        shr    ecx, 2
        rep insd
        mov    edi, eax
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebx + 18ah]
        in     ax, dx
        and    eax, 0ffffh
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        add    dword ptr [ebp - 10h], ecx
        mov    dword ptr [ebp - 8], eax
        db  089h, 004h, 095h, 000h, 000h, 000h, 000h
        lea    eax, [edx + 1]
        and    eax, 7ffh
        test   byte ptr [ebp - 7], 80h
        db  0A3h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_23A8
        and    dword ptr [ebp - 8], 1ffch
        jmp    SHORT loc_1_2337
loc_1_23A8:
        test   byte ptr [ebp - 7], 40h
        jne    SHORT loc_1_23FB
        and    dword ptr [ebp - 8], 1fffh
        mov    edx, dword ptr [ebp - 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        add    edx, 3
        mov    eax, edx
        mov    dword ptr [ebp - 34h], edx
        and    eax, 0fffffffch
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, edi
        mov    edi, dword ptr [ebp - 10h]
        mov    edx, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 34h]
        shr    ecx, 2
        rep insd
        mov    edi, eax
        mov    ecx, dword ptr [ebp - 8]
        add    dword ptr [ebp - 0ch], ecx
loc_1_23F5:
        test   byte ptr [ebp - 7], 40h
        je     SHORT loc_1_2464
loc_1_23FB:
        cmp    dword ptr [ebx + 29eh], 2
        jne    SHORT loc_1_2408
        mov    byte ptr [ebp - 1], 1
loc_1_2408:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_2451
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, 280fh
        mov    edx, dword ptr [ebx + 176h]
        out    dx, ax
        mov    edx, dword ptr [ebx + 17ah]
loc_1_2440:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_2440
        mov    dword ptr [ebx + 29eh], 0
loc_1_2451:
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        push   ebx
        call   sub_1_34D8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2464:
        mov    byte ptr [ebx + 0b7h], 1
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ebx + 373h], ecx
        mov    dword ptr [ebx + 36fh], ecx
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  089h, 004h, 095h, 000h, 000h, 000h, 000h
        lea    eax, [edx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 00Ch, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        cmp    byte ptr [ebx + 36ah], 0
        je     SHORT loc_1_24BF
        push   ecx
        push   ebx
        call   sub_1_3788
        test   al, al
        jne    SHORT loc_1_24ED
loc_1_24BF:
        lea    edx, [ebx + 36bh]
        mov    eax, dword ptr [ebx + 373h]
        sub    eax, 0eh
        mov    ecx, dword ptr [edx]
        push   eax
        mov    eax, dword ptr [ebx + 36fh]
        sub    eax, 0eh
        lea    esi, [ecx + 0eh]
        push   eax
        push   esi
        push   0eh
        push   ecx
        push   ecx
        push   edx
        mov    ecx, dword ptr [ebx + 5eh]
        push   ecx
        call   sub_1_8B30
loc_1_24ED:
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        push   ebx
        call   sub_1_3668
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        mov    esp, ebp
        and    eax, 7ffh
        pop    ebp
        db  0A3h, 000h, 000h, 000h, 000h
        ret    4
loc_1_251F:
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebx + 396h]
        mov    ecx, dword ptr [ebx + 0c6h]
        mov    edx, dword ptr [ebx + 18ah]
        mov    dword ptr [ebp - 0ch], ecx
        mov    dword ptr [ebp - 14h], ecx
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 10h], eax
        in     ax, dx
        mov    edx, eax
        lea    eax, [edi + 1]
        and    edx, 0ffffh
        and    eax, 7ffh
        mov    dword ptr [ebp - 8], edx
        db  089h, 014h, 0BDh, 000h, 000h, 000h, 000h
        and    dword ptr [ebp - 8], 1ffch
        db  0A3h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebx + 2eeh], ecx
        jae    SHORT loc_1_25ED
        cmp    dword ptr [ebx + 29eh], 0
        jne    SHORT loc_1_25E9
        mov    edx, dword ptr [ebx + 172h]
        mov    eax, dword ptr [ebx + 2d2h]
        mov    dword ptr [ebx + 29eh], 2
        mov    byte ptr [ebp - 1], 1
        out    dx, eax
        mov    ax, cx
        add    edx, 6
        out    dx, ax
        mov    edx, dword ptr [ebx + 176h]
        mov    eax, 0a000h
        out    dx, ax
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_25ED
loc_1_25E9:
        mov    byte ptr [ebp - 1], 0
loc_1_25ED:
        cmp    byte ptr [ebp - 1], 0
        je     NEAR PTR loc_1_2837
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebx + 172h]
        add    edx, 0eh
loc_1_261F:
        mov    dword ptr [ebp - 20h], esi
        push   ecx
        mov    ecx, dword ptr [ebp - 20h]
loc_1_2626:
        xor    eax, eax
        dec    ecx
        jne    SHORT loc_1_2626
        pop    ecx
        in     ax, dx
        test   ah, 8
        jne    SHORT loc_1_261F
        and    eax, 0ffffh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 00Ch, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 014h, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebx + 172h]
        add    edx, 6
        in     ax, dx
        mov    ecx, eax
        db  0A1h, 000h, 000h, 000h, 000h
        and    ecx, 0ffffh
        inc    eax
        sub    dword ptr [ebp - 0ch], ecx
        mov    edx, dword ptr [ebp - 0ch]
        db  089h, 014h, 085h, 0FCh, 0FFh, 0FFh, 0FFh
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, 4000h
        mov    edx, dword ptr [ebx + 172h]
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        add    edx, 0ch
        out    dx, ax
        in     ax, dx
        mov    ecx, eax
        mov    edx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        add    dword ptr [ebp - 10h], edx
        db  089h, 00Ch, 0B5h, 000h, 000h, 000h, 000h
        lea    ecx, [esi + 1]
        and    ecx, 7ffh
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 29eh], 0
        mov    edx, dword ptr [ebx + 18ah]
        in     ax, dx
        and    eax, 0ffffh
        test   ah, 80h
        mov    dword ptr [ebp - 8], eax
        jne    NEAR PTR loc_1_28ED
        test   ah, 40h
        je     SHORT loc_1_2729
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        push   ebx
        call   sub_1_34D8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2729:
        and    dword ptr [ebp - 8], 1fffh
        mov    edx, dword ptr [ebp - 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        add    edx, 3
        mov    eax, edx
        mov    dword ptr [ebp - 30h], edx
        and    eax, 0fffffffch
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, edi
        mov    edi, dword ptr [ebp - 10h]
        mov    edx, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 30h]
        shr    ecx, 2
        rep insd
        mov    edi, eax
        mov    byte ptr [ebx + 0b7h], 1
        mov    ecx, dword ptr [ebp - 8]
        add    dword ptr [ebp - 0ch], ecx
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebx + 36fh], eax
        mov    dword ptr [ebx + 373h], eax
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        db  089h, 014h, 08Dh, 000h, 000h, 000h, 000h
        and    eax, 7ffh
        mov    ecx, dword ptr [ebp - 0ch]
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 00Ch, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        cmp    byte ptr [ebx + 36ah], 0
        je     SHORT loc_1_27F6
        push   ecx
        push   ebx
        call   sub_1_3788
        test   al, al
        jne    SHORT loc_1_2824
        lea    ecx, [ebx + 36bh]
        mov    eax, dword ptr [ebx + 373h]
        sub    eax, 0eh
        mov    edx, dword ptr [ecx]
        push   eax
        mov    eax, dword ptr [ebx + 36fh]
        sub    eax, 0eh
        lea    esi, [edx + 0eh]
        push   eax
        push   esi
        push   0eh
        push   edx
        push   edx
        push   ecx
        jmp    SHORT loc_1_281B
loc_1_27F6:
        lea    edx, [ebx + 36bh]
        mov    eax, dword ptr [ebx + 373h]
        sub    eax, 0eh
        mov    ecx, dword ptr [edx]
        push   eax
        mov    eax, dword ptr [ebx + 36fh]
        sub    eax, 0eh
        lea    esi, [ecx + 0eh]
        push   eax
        push   esi
        push   0eh
        push   ecx
        push   ecx
        push   edx
loc_1_281B:
        mov    ecx, dword ptr [ebx + 5eh]
        push   ecx
        call   sub_1_8B30
loc_1_2824:
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        push   ebx
        call   sub_1_3668
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2837:
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        cmp    dword ptr [ebp - 14h], 0
        db  0A3h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_28ED
loc_1_2860:
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 8]
        cmp    eax, ecx
        jb     SHORT loc_1_286C
        mov    eax, ecx
loc_1_286C:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], eax
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        mov    ecx, dword ptr [ebp - 8]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 1ch], ecx
        mov    eax, edi
        mov    edi, dword ptr [ebp - 10h]
        mov    edx, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 1ch]
        shr    ecx, 2
        rep insd
        mov    edi, eax
        mov    eax, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebx + 18ah]
        add    dword ptr [ebp - 10h], eax
        sub    dword ptr [ebp - 14h], eax
        in     ax, dx
        mov    ecx, eax
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        and    ecx, 0ffffh
        lea    eax, [edx + 1]
        and    eax, 7ffh
        mov    dword ptr [ebp - 8], ecx
        db  089h, 00Ch, 095h, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        test   ch, 80h
        je     SHORT loc_1_2927
        and    dword ptr [ebp - 8], 1ffch
        cmp    dword ptr [ebp - 14h], 0
        jne    NEAR PTR loc_1_2860
loc_1_28ED:
        mov    ecx, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebx + 396h]
        xor    eax, eax
        mov    dword ptr [ebx + 36fh], ecx
        mov    cx, word ptr [edx + 0ch]
        mov    al, cl
        shl    eax, 8
        shr    cx, 8
        and    ecx, 0ffffh
        add    ecx, eax
        cmp    ecx, 600h
        jae    NEAR PTR loc_1_2A2C
        add    ecx, 0eh
        jmp    NEAR PTR loc_1_2A9A
loc_1_2927:
        test   byte ptr [ebp - 7], 40h
        je     SHORT loc_1_2943
        mov    eax, dword ptr [ebp - 0ch]
        sub    eax, dword ptr [ebp - 14h]
        push   eax
        push   ebx
        call   sub_1_34D8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2943:
        and    dword ptr [ebp - 8], 1fffh
        mov    edx, dword ptr [ebp - 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        add    edx, 3
        mov    eax, edx
        mov    dword ptr [ebp - 2ch], edx
        and    eax, 0fffffffch
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, edi
        mov    edi, dword ptr [ebp - 10h]
        mov    edx, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 2ch]
        shr    ecx, 2
        rep insd
        mov    edi, eax
        mov    ecx, dword ptr [ebp - 14h]
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [ebx + 0b7h], 1
        sub    dword ptr [ebp - 0ch], ecx
        add    dword ptr [ebp - 0ch], eax
        mov    edx, dword ptr [ebp - 0ch]
        mov    dword ptr [ebx + 36fh], edx
        mov    dword ptr [ebx + 373h], edx
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 014h, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        cmp    byte ptr [ebx + 36ah], 0
        je     SHORT loc_1_29EB
        push   edx
        push   ebx
        call   sub_1_3788
        test   al, al
        jne    SHORT loc_1_2A19
loc_1_29EB:
        lea    ecx, [ebx + 36bh]
        mov    eax, dword ptr [ebx + 373h]
        sub    eax, 0eh
        mov    edx, dword ptr [ecx]
        push   eax
        mov    eax, dword ptr [ebx + 36fh]
        sub    eax, 0eh
        lea    esi, [edx + 0eh]
        push   eax
        push   esi
        push   0eh
        push   edx
        push   edx
        push   ecx
        mov    ecx, dword ptr [ebx + 5eh]
        push   ecx
        call   sub_1_8B30
loc_1_2A19:
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        push   ebx
        call   sub_1_3668
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2A2C:
        jne    SHORT loc_1_2A4B
        mov    cx, word ptr [edx + 10h]
        xor    eax, eax
        mov    al, cl
        shr    cx, 8
        mov    edx, ecx
        and    edx, 0ffffh
        shl    eax, 8
        lea    ecx, [eax + edx + 0eh]
        jmp    SHORT loc_1_2A9A
loc_1_2A4B:
        cmp    ecx, 800h
        jne    SHORT loc_1_2A70
        mov    cx, word ptr [edx + 10h]
        xor    eax, eax
        mov    al, cl
        shr    cx, 8
        mov    edx, ecx
        and    edx, 0ffffh
        shl    eax, 8
        lea    ecx, [eax + edx + 0eh]
        jmp    SHORT loc_1_2A9A
loc_1_2A70:
        cmp    ecx, 8137h
        jne    SHORT loc_1_2A95
        mov    cx, word ptr [edx + 10h]
        xor    eax, eax
        mov    al, cl
        shr    cx, 8
        mov    edx, ecx
        and    edx, 0ffffh
        shl    eax, 8
        lea    ecx, [eax + edx + 0eh]
        jmp    SHORT loc_1_2A9A
loc_1_2A95:
        mov    ecx, 5eah
loc_1_2A9A:
        cmp    ecx, 5eah
        ja     SHORT loc_1_2AA7
        cmp    dword ptr [ebp - 0ch], ecx
        jbe    SHORT loc_1_2AAC
loc_1_2AA7:
        mov    ecx, 5eah
loc_1_2AAC:
        mov    esi, ecx
        sub    esi, dword ptr [ebp - 0ch]
        mov    dword ptr [ebx + 0eah], esi
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  089h, 004h, 095h, 000h, 000h, 000h, 000h
        lea    eax, [edx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 00Ch, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebx + 2eeh], ecx
        jae    NEAR PTR loc_1_2B7C
        cmp    dword ptr [ebx + 29eh], 0
        jne    SHORT loc_1_2B59
        mov    dword ptr [ebx + 29eh], 2
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 1], 1
        db  089h, 004h, 095h, 000h, 000h, 000h, 000h
        lea    eax, [edx + 1]
        and    eax, 7ffh
        mov    di, 5eah
        db  0A3h, 000h, 000h, 000h, 000h
        sub    di, word ptr [ebp - 0ch]
        mov    edx, dword ptr [ebx + 172h]
        mov    eax, dword ptr [ebx + 2d2h]
        add    eax, dword ptr [ebp - 0ch]
        out    dx, eax
        mov    ax, di
        add    edx, 6
        out    dx, ax
        mov    edx, dword ptr [ebx + 176h]
        mov    eax, 0a000h
        out    dx, ax
        jmp    SHORT loc_1_2B7C
loc_1_2B59:
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 1], 0
        db  089h, 004h, 095h, 000h, 000h, 000h, 000h
        lea    eax, [edx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
loc_1_2B7C:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_2B9A
        mov    eax, dword ptr [ebx + 1c2h]
        cmp    eax, 20h
        je     SHORT loc_1_2BB1
        cmp    eax, 30h
        je     SHORT loc_1_2BB1
        sub    ecx, dword ptr [ebx + 212h]
        jmp    SHORT loc_1_2BB1
loc_1_2B9A:
        shr    esi, 1
        mov    eax, dword ptr [ebx + 0dah]
        imul   eax, esi
        shr    eax, 5
        sub    ecx, eax
        sub    ecx, dword ptr [ebx + 20eh]
loc_1_2BB1:
        test   ecx, ecx
        jge    SHORT loc_1_2BB7
        xor    ecx, ecx
loc_1_2BB7:
        lea    eax, [ecx + 3]
        mov    edx, dword ptr [ebx + 176h]
        mov    byte ptr [ebx + 0b6h], 1
        shr    eax, 2
        and    ax, 7ffh
        sub    ax, 7800h
        out    dx, ax
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        lea    eax, [edx + 1]
        db  089h, 00Ch, 095h, 000h, 000h, 000h, 000h
        and    eax, 7ffh
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        pop    edi
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 014h, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        pop    esi
        pop    ebx
        db  0A3h, 000h, 000h, 000h, 000h
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2C18:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    esi, dword ptr [ebx + 1feh]
        mov    eax, dword ptr [ebx + 192h]
        mov    edx, dword ptr [ebx + 18ah]
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 18h], eax
        in     ax, dx
        mov    edx, eax
        db  0A1h, 000h, 000h, 000h, 000h
        and    edx, 0ffffh
        db  089h, 004h, 0BDh, 000h, 000h, 000h, 000h
        lea    eax, [edi + 1]
        mov    dword ptr [ebp - 4], edx
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 014h, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        test   dh, 40h
        db  0A3h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_2D0A
        xor    esi, esi
        cmp    byte ptr [ebx + 0b6h], cl
        je     SHORT loc_1_2C92
        mov    esi, dword ptr [ebx + 36fh]
loc_1_2C92:
        cmp    dword ptr [ebx + 29eh], 2
        jne    SHORT loc_1_2C9D
        mov    cl, 1
loc_1_2C9D:
        test   cl, cl
        je     SHORT loc_1_2CE4
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, 280fh
        mov    edx, dword ptr [ebx + 176h]
        out    dx, ax
        mov    edx, dword ptr [ebx + 17ah]
loc_1_2CD3:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_2CD3
        mov    dword ptr [ebx + 29eh], 0
loc_1_2CE4:
        push   esi
        push   ebx
        call   sub_1_34D8
        mov    ax, word ptr [ebx + 0e2h]
        mov    edx, dword ptr [ebx + 176h]
        mov    byte ptr [ebx + 0b6h], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        out    dx, ax
        pop    ebp
        ret    4
loc_1_2D0A:
        and    dword ptr [ebp - 4], 1fffh
        cmp    byte ptr [ebx + 0b6h], 0
        je     NEAR PTR loc_1_2FC0
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [ebx + 0e2h]
        mov    edx, dword ptr [ebx + 176h]
        mov    byte ptr [ebx + 0b6h], 0
        out    dx, ax
        mov    ecx, dword ptr [ebx + 36fh]
        mov    eax, dword ptr [ebx + 396h]
        add    eax, ecx
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebx + 29eh]
        test   eax, eax
        jne    SHORT loc_1_2DD6
        mov    edx, dword ptr [ebp - 4]
        cmp    dword ptr [ebx + 2eeh], edx
        jae    SHORT loc_1_2DD6
        mov    cl, 1
        mov    edx, dword ptr [ebx + 172h]
        mov    di, 5eah
        mov    eax, dword ptr [ebx + 2d2h]
        sub    di, word ptr [ebp - 8]
        add    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebx + 29eh], 2
        out    dx, eax
        mov    ax, di
        add    edx, 6
        out    dx, ax
        mov    edx, dword ptr [ebx + 176h]
        mov    eax, 0a000h
        out    dx, ax
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  089h, 004h, 095h, 000h, 000h, 000h, 000h
        lea    eax, [edx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_2DE0
loc_1_2DD6:
        sub    eax, 2
        cmp    eax, 1
        sbb    cl, cl
        neg    cl
loc_1_2DE0:
        test   cl, cl
        je     NEAR PTR loc_1_2EFC
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 00Ch, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebx + 172h]
        add    edx, 0eh
loc_1_2E28:
        mov    dword ptr [ebp - 10h], esi
        push   ecx
        mov    ecx, dword ptr [ebp - 10h]
loc_1_2E2F:
        xor    eax, eax
        dec    ecx
        jne    SHORT loc_1_2E2F
        pop    ecx
        in     ax, dx
        test   ah, 8
        jne    SHORT loc_1_2E28
        and    eax, 0ffffh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 014h, 085h, 000h, 000h, 000h, 000h
        inc    eax
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        and    eax, 7ffh
        mov    ecx, 5eah
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 014h, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebx + 172h]
        add    edx, 6
        in     ax, dx
        mov    edx, eax
        db  0A1h, 000h, 000h, 000h, 000h
        and    edx, 0ffffh
        inc    eax
        sub    ecx, edx
        mov    dword ptr [ebp - 8], ecx
        db  089h, 00Ch, 085h, 0FCh, 0FFh, 0FFh, 0FFh
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, 4000h
        mov    edx, dword ptr [ebx + 172h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        add    edx, 0ch
        out    dx, ax
        in     ax, dx
        and    eax, 0ffffh
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 29eh], 0
        jmp    SHORT loc_1_2F58
loc_1_2EFC:
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 4]
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        add    dword ptr [ebp - 8], edx
        and    eax, 7ffh
        add    edx, 3
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, edx
        and    eax, 0fffffffch
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 24h], edx
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, edi
        mov    edi, dword ptr [ebp - 14h]
        mov    edx, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 24h]
        shr    ecx, 2
        rep insd
        mov    edi, eax
loc_1_2F58:
        mov    byte ptr [ebx + 0b7h], 1
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ebx + 373h], ecx
        mov    dword ptr [ebx + 36fh], ecx
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  089h, 004h, 095h, 000h, 000h, 000h, 000h
        lea    eax, [edx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 00Ch, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        cmp    byte ptr [ebx + 36ah], 0
        je     NEAR PTR loc_1_3196
        push   ecx
        push   ebx
        call   sub_1_3788
        test   al, al
        jne    NEAR PTR loc_1_31C4
        jmp    NEAR PTR loc_1_3196
loc_1_2FC0:
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 4]
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        mov    ecx, 1
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebx + 29eh]
        mov    dword ptr [ebx + 373h], edx
        mov    dword ptr [ebx + 36fh], edx
        mov    byte ptr [ebx + 0b7h], cl
        test   eax, eax
        jne    SHORT loc_1_303A
        cmp    dword ptr [ebx + 2eeh], edx
        jae    SHORT loc_1_303A
        mov    edx, dword ptr [ebx + 172h]
        mov    eax, dword ptr [ebx + 2d2h]
        mov    dword ptr [ebx + 29eh], 2
        out    dx, eax
        mov    ax, word ptr [ebp - 4]
        add    edx, 6
        out    dx, ax
        mov    edx, dword ptr [ebx + 176h]
        mov    eax, 0a000h
        out    dx, ax
        jmp    SHORT loc_1_3057
loc_1_303A:
        cmp    eax, 2
        jne    SHORT loc_1_3055
        mov    dword ptr [ebp - 20h], esi
        push   ecx
        mov    ecx, dword ptr [ebp - 20h]
loc_1_3046:
        xor    eax, eax
        dec    ecx
        jne    SHORT loc_1_3046
        pop    ecx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3055:
        xor    cl, cl
loc_1_3057:
        test   cl, cl
        je     NEAR PTR loc_1_3108
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 00Ch, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebx + 172h]
        add    edx, 0eh
loc_1_309F:
        mov    dword ptr [ebp - 0ch], esi
        push   ecx
        mov    ecx, dword ptr [ebp - 0ch]
loc_1_30A6:
        xor    eax, eax
        dec    ecx
        jne    SHORT loc_1_30A6
        pop    ecx
        in     ax, dx
        test   ah, 8
        jne    SHORT loc_1_309F
        and    eax, 0ffffh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 014h, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, 4000h
        mov    dword ptr [ebx + 29eh], 0
        mov    edx, dword ptr [ebx + 172h]
        add    edx, 0ch
        out    dx, ax
        in     ax, dx
        jmp    SHORT loc_1_314B
loc_1_3108:
        mov    eax, dword ptr [ebx + 396h]
        mov    edx, dword ptr [ebp - 4]
        add    edx, 3
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 14h], eax
        mov    eax, edx
        and    eax, 0fffffffch
        mov    dword ptr [ebp - 1ch], edx
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, edi
        mov    edi, dword ptr [ebp - 14h]
        mov    edx, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 1ch]
        shr    ecx, 2
        rep insd
        mov    edi, eax
loc_1_314B:
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 4]
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        mov    dword ptr [ebp - 8], edx
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 014h, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        cmp    byte ptr [ebx + 36ah], 0
        je     SHORT loc_1_3196
        push   edx
        push   ebx
        call   sub_1_3788
        test   al, al
        jne    SHORT loc_1_31C4
loc_1_3196:
        lea    ecx, [ebx + 36bh]
        mov    eax, dword ptr [ebx + 373h]
        sub    eax, 0eh
        mov    edx, dword ptr [ecx]
        push   eax
        mov    eax, dword ptr [ebx + 36fh]
        sub    eax, 0eh
        lea    esi, [edx + 0eh]
        push   eax
        push   esi
        push   0eh
        push   edx
        push   edx
        push   ecx
        mov    ecx, dword ptr [ebx + 5eh]
        push   ecx
        call   sub_1_8B30
loc_1_31C4:
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        push   ebx
        call   sub_1_3668
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        mov    esp, ebp
        and    eax, 7ffh
        pop    ebp
        db  0A3h, 000h, 000h, 000h, 000h
        ret    4
        db  08Bh, 0FFh
loc_1_31F8:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        sub    esp, 0ch
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        push   ebx
        push   esi
        and    eax, 7ffh
        push   edi
        xor    edx, edx
        mov    esi, dword ptr [ebp + 10h]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    dx, word ptr [esi + 2ch]
        db  089h, 014h, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        xor    edx, edx
        xor    ecx, ecx
        db  0A3h, 000h, 000h, 000h, 000h
        mov    dx, word ptr [esi + 2eh]
        db  089h, 014h, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        mov    edx, dword ptr [ebp + 8]
        db  0A3h, 000h, 000h, 000h, 000h
        xor    eax, eax
        mov    cx, word ptr [esi + 2ch]
        mov    edi, dword ptr [edx + 396h]
        mov    edx, dword ptr [ebp + 0ch]
        mov    ax, word ptr [esi + 2eh]
        add    edi, ecx
        mov    ebx, dword ptr [edx + 8]
        sub    ebx, ecx
        cmp    ebx, eax
        jbe    SHORT loc_1_3278
        mov    ebx, eax
loc_1_3278:
        push   0
        lea    eax, [ebp - 8]
        push   eax
        mov    dword ptr [esi + 30h], ebx
        push   0
        push   0
        push   esi
        call   sub_1_8FB0
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_8FD4
        test   ebx, ebx
        je     NEAR PTR loc_1_332B
loc_1_32A4:
        cmp    ebx, dword ptr [ebp - 4]
        jbe    SHORT loc_1_3304
        mov    eax, dword ptr [ebp - 4]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 0ch]
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        mov    ecx, dword ptr [ebp - 4]
        and    eax, 7ffh
        push   ecx
        push   edi
        db  0A3h, 000h, 000h, 000h, 000h
        push   edx
        call   sub_1_906C
        sub    ebx, dword ptr [ebp - 4]
        add    edi, dword ptr [ebp - 4]
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_8FF8
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_3328
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_8FD4
        test   ebx, ebx
        jne    SHORT loc_1_32A4
        jmp    SHORT loc_1_332B
loc_1_3304:
        db  0A1h, 000h, 000h, 000h, 000h
        push   ebx
        inc    eax
        push   edi
        mov    ecx, dword ptr [ebp - 0ch]
        db  089h, 01Ch, 085h, 0FCh, 0FFh, 0FFh, 0FFh
        and    eax, 7ffh
        push   ecx
        db  0A3h, 000h, 000h, 000h, 000h
        call   sub_1_906C
        jmp    SHORT loc_1_332B
loc_1_3328:
        sub    dword ptr [esi + 30h], ebx
loc_1_332B:
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        mov    esp, ebp
        and    eax, 7ffh
        pop    ebp
        db  0A3h, 000h, 000h, 000h, 000h
        xor    eax, eax
        ret    0ch
        db  08Dh, 049h, 000h
sub_1_3358:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        sub    esp, 0ch
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        push   ebx
        push   esi
        and    eax, 7ffh
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        xor    edi, edi
        mov    esi, dword ptr [ebp + 8]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi]
        mov    di, word ptr [ecx + 2ch]
        add    eax, 0fh
        and    eax, 0fffffff0h
        mov    dword ptr [esi + 387h], edi
        cmp    eax, edi
        jae    SHORT loc_1_33A0
        mov    edi, eax
loc_1_33A0:
        push   0
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [ebp + 0ch]
        push   0
        mov    dword ptr [esi + 383h], edi
        push   0
        push   ecx
        call   sub_1_8FB0
        lea    ecx, [ebp - 4]
        lea    edx, [ebp - 0ch]
        push   ecx
        mov    eax, dword ptr [ebp - 8]
        push   edx
        push   eax
        call   sub_1_8FD4
        mov    ebx, dword ptr [esi + 39ah]
loc_1_33D0:
        cmp    edi, dword ptr [ebp - 4]
        jb     SHORT loc_1_340F
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0ch]
        push   eax
        push   ecx
        push   ebx
        call   sub_1_906C
        sub    edi, dword ptr [ebp - 4]
        add    ebx, dword ptr [ebp - 4]
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_8FF8
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_341A
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_8FD4
        jmp    SHORT loc_1_33D0
loc_1_340F:
        push   edi
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        push   ebx
        call   sub_1_906C
loc_1_341A:
        mov    eax, dword ptr [ebp + 0ch]
        lea    edx, [esi + 37fh]
        mov    dword ptr [esi + 38fh], eax
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [esi + 387h]
        lea    ebx, [ecx + 0eh]
        sub    eax, 0eh
        push   eax
        mov    eax, dword ptr [esi + 383h]
        sub    eax, 0eh
        push   eax
        push   ebx
        push   0eh
        push   ecx
        push   ecx
        push   edx
        mov    ecx, dword ptr [esi + 5eh]
        push   ecx
        call   sub_1_8B30
        cmp    byte ptr [esi + 0c2h], 0
        je     SHORT loc_1_346A
        mov    byte ptr [esi + 0b7h], 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_346A:
        mov    eax, dword ptr [esi + 5eh]
        push   eax
        call   sub_1_8D10
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3488:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        mov    ecx, dword ptr [ebp + 10h]
        db  0A3h, 000h, 000h, 000h, 000h
        lea    edx, [ecx + 30h]
        push   edx
        xor    eax, eax
        mov    ax, word ptr [ecx + 2ch]
        mov    edx, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [edx + 10h]
        push   eax
        xor    edx, edx
        mov    dx, word ptr [ecx + 2eh]
        push   edx
        push   0
        push   ecx
        call   sub_1_8FFE
        xor    eax, eax
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h
sub_1_34D8:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        push   edi
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        mov    ecx, dword ptr [ebp + 8]
        and    eax, 7ffh
        mov    esi, dword ptr [ebp + 0ch]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    ebx, dword ptr [ecx + 172h]
        lea    edx, [ebx + 18h]
        in     ax, dx
        mov    dx, ax
        and    dx, 1fffh
        lea    edi, [edx + esi]
        lea    edx, [ebx + 14h]
        in     al, dx
        xor    edx, edx
        test   al, 1
        mov    dl, al
        je     SHORT loc_1_3544
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 09Dh, 000h, 000h, 000h, 000h
        lea    eax, [ebx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
loc_1_3544:
        test   dl, 2
        je     SHORT loc_1_356E
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 09Dh, 000h, 000h, 000h, 000h
        lea    eax, [ebx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        inc    dword ptr [ecx + 0feh]
loc_1_356E:
        test   dl, 4
        je     SHORT loc_1_3598
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 09Dh, 000h, 000h, 000h, 000h
        lea    eax, [ebx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        inc    dword ptr [ecx + 33eh]
loc_1_3598:
        test   dl, 8
        je     SHORT loc_1_35C2
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 09Dh, 000h, 000h, 000h, 000h
        lea    eax, [ebx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        inc    dword ptr [ecx + 342h]
loc_1_35C2:
        test   dl, 10h
        je     SHORT loc_1_35EC
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        db  089h, 004h, 09Dh, 000h, 000h, 000h, 000h
        lea    eax, [ebx + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        inc    dword ptr [ecx + 346h]
loc_1_35EC:
        cmp    edx, 1
        je     SHORT loc_1_3609
        cmp    edx, 2
        je     SHORT loc_1_3609
        mov    eax, edi
        dec    dword ptr [ecx + 306h]
        and    eax, 0ffffh
        sub    dword ptr [ecx + 312h], eax
loc_1_3609:
        push   esi
        push   ecx
        call   sub_1_3668
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  090h
sub_1_3618:
        push   ebp
        mov    cl, 0f5h
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [edx + 0c6h]
        sub    cl, byte ptr [edx + 20eh]
        shr    esi, 1
        mov    eax, dword ptr [edx + 0e6h]
        imul   esi, dword ptr [edx + 0dah]
        shr    esi, cl
        shl    eax, 2
        cmp    eax, esi
        jle    SHORT loc_1_3648
        mov    esi, eax
loc_1_3648:
        add    esi, 3
        and    esi, 1ffch
        shr    esi, 2
        add    esi, 8800h
        mov    dword ptr [edx + 0e2h], esi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_3668:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    edi, 1fffh
        and    eax, 7ffh
        mov    ecx, dword ptr [ebp + 8]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ecx + 172h]
        add    edx, 18h
        in     ax, dx
        and    ax, 1fffh
        lea    esi, [eax + edi + 3]
        db  0A1h, 000h, 000h, 000h, 000h
        and    esi, 0fffch
        nop
        db  089h, 03Ch, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 034h, 085h, 000h, 000h, 000h, 000h
        inc    eax
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, 4000h
        mov    edx, dword ptr [ecx + 176h]
        out    dx, ax
        mov    edx, dword ptr [ecx + 17ah]
loc_1_36EE:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_36EE
        cmp    byte ptr [ecx + 36ah], 0
        je     SHORT loc_1_3705
        push   esi
        push   ecx
        call   sub_1_3898
loc_1_3705:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3718:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   esi
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        mov    ecx, dword ptr [ebp + 8]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, 2800h
        mov    esi, dword ptr [ecx + 176h]
        mov    edx, esi
        out    dx, ax
        mov    edx, dword ptr [ecx + 17ah]
loc_1_3753:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_3753
        mov    ax, word ptr [ecx + 102h]
        mov    edx, esi
        out    dx, ax
        mov    eax, 2000h
        pop    esi
        out    dx, ax
        mov    ax, word ptr [ecx + 0e2h]
        pop    ebp
        out    dx, ax
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3788:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   esi
        push   edi
        db  089h, 004h, 08Dh, 000h, 000h, 000h, 000h
        lea    eax, [ecx + 1]
        and    eax, 7ffh
        mov    ecx, dword ptr [ebp + 8]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ecx + 362h]
        cmp    dword ptr [ecx + 35eh], edx
        je     NEAR PTR loc_1_387C
        mov    edx, dword ptr [ebp + 0ch]
loc_1_37C4:
        mov    eax, dword ptr [ecx + 35ah]
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        add    eax, edx
        db  089h, 004h, 0B5h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ecx + 35eh]
        mov    edi, dword ptr [ecx + 366h]
        mov    eax, dword ptr [edi + esi*8]
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        db  089h, 004h, 0B5h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ecx + 35eh]
        mov    edi, dword ptr [ecx + 366h]
        mov    eax, dword ptr [edi + esi*8 + 4]
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        db  089h, 004h, 0B5h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 1]
        and    eax, 7ffh
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ecx + 35ah]
        mov    edi, dword ptr [ecx + 35eh]
        add    eax, edx
        lea    esi, [edi*8]
        add    esi, dword ptr [ecx + 366h]
        cmp    dword ptr [esi], eax
        ja     SHORT loc_1_3884
        cmp    dword ptr [esi + 4], eax
        jae    SHORT loc_1_388C
        inc    edi
        mov    dword ptr [ecx + 35eh], edi
        and    edi, 1ffh
        mov    dword ptr [ecx + 35eh], edi
        cmp    dword ptr [ecx + 362h], edi
        jne    NEAR PTR loc_1_37C4
loc_1_387C:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_3884:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_388C:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_3898:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    eax, 803h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 176h]
        mov    edx, ecx
        out    dx, ax
        mov    edx, dword ptr [esi + 172h]
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        add    edx, 0ah
        in     ax, dx
        mov    di, ax
        mov    edx, ecx
        mov    eax, 807h
        and    edi, 0ffffh
        out    dx, ax
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        lea    ecx, [edx + 1]
        db  089h, 01Ch, 095h, 000h, 000h, 000h, 000h
        and    ecx, 7ffh
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 03Ch, 08Dh, 000h, 000h, 000h, 000h
        inc    ecx
        and    ecx, 7ffh
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [esi + 172h]
        add    edx, 18h
        in     ax, dx
        test   ah, 80h
        je     SHORT loc_1_3935
        mov    eax, dword ptr [esi + 35eh]
        pop    edi
        mov    dword ptr [esi + 35ah], 0
        mov    dword ptr [esi + 362h], eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_3935:
        mov    ecx, dword ptr [ebp + 0ch]
        and    edi, 0fffffffch
        lea    eax, [ecx + 3]
        and    eax, 0fffffffch
        add    eax, 4
        mov    dword ptr [ebp - 4], eax
        cmp    eax, edi
        jb     SHORT loc_1_39AE
        mov    edx, dword ptr [esi + 362h]
        lea    eax, [edx + 1]
        and    eax, 1ffh
        mov    dword ptr [esi + 362h], eax
        cmp    dword ptr [esi + 35eh], eax
        jne    SHORT loc_1_3980
        push   esi
        call   sub_1_3718
        mov    dword ptr [esi + 35ah], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_3980:
        mov    eax, dword ptr [esi + 352h]
        mov    ebx, dword ptr [esi + 366h]
        add    eax, dword ptr [esi + 35ah]
        sub    eax, 14h
        mov    dword ptr [ebx + edx*8], eax
        mov    eax, dword ptr [esi + 366h]
        lea    eax, [eax + edx*8]
        mov    edx, dword ptr [eax]
        sub    edx, edi
        add    edx, dword ptr [ebp - 4]
        add    edx, 14h
        mov    dword ptr [eax + 4], edx
loc_1_39AE:
        mov    eax, dword ptr [esi + 35ah]
        lea    edx, [eax + ecx + 4]
        mov    eax, dword ptr [esi + 352h]
        add    ecx, eax
        mov    dword ptr [esi + 35ah], edx
        add    ecx, 4
        add    eax, edx
        cmp    ecx, eax
        jbe    SHORT loc_1_3A01
        sub    edx, 80000000h
        xor    ecx, ecx
        mov    dword ptr [esi + 35ah], edx
        mov    edx, 80000000h
loc_1_39E2:
        mov    eax, dword ptr [esi + 366h]
        add    ecx, 8
        add    dword ptr [eax + ecx - 8], edx
        mov    eax, dword ptr [esi + 366h]
        add    dword ptr [eax + ecx - 4], edx
        cmp    ecx, 1000h
        jb     SHORT loc_1_39E2
loc_1_3A01:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3A18:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        add    eax, dword ptr [esi + 0f6h]
        mov    dword ptr [esi + 0f6h], eax
        cmp    eax, 3e8h
        jbe    SHORT loc_1_3A4A
        mov    dword ptr [esi + 0f6h], 0
        push   esi
        call   sub_1_0348
loc_1_3A4A:
        cmp    byte ptr [esi + 0b8h], 0
        jne    SHORT loc_1_3A9C
        mov    eax, dword ptr [esi + 31ah]
        cmp    eax, 3e8h
        jbe    SHORT loc_1_3A9C
        shl    eax, 2
        cmp    eax, dword ptr [esi + 302h]
        jbe    SHORT loc_1_3A9C
        mov    ecx, dword ptr [esi + 176h]
        mov    eax, 804h
        mov    byte ptr [esi + 0b8h], 1
        mov    edx, ecx
        out    dx, ax
        mov    edx, dword ptr [esi + 172h]
        add    edx, 0ah
        in     ax, dx
        and    eax, 0fff7h
        out    dx, ax
        mov    edx, ecx
        mov    eax, 807h
        out    dx, ax
loc_1_3A9C:
        mov    cx, word ptr [esi + 206h]
        mov    ax, word ptr [esi + 208h]
        mov    ebx, ecx
        sub    dx, dx
        and    ebx, 0ffffh
        div    cx
        mov    word ptr [ebp - 2], ax
        sub    edx, edx
        mov    cx, word ptr [ebp - 2]
        mov    eax, dword ptr [esi + 20ah]
        div    ebx
        imul   cx, cx
        sub    ax, cx
        mov    edx, dword ptr [esi + 4ah]
        mov    cx, ax
        shr    ax, 0ch
        mov    edi, eax
        and    edi, 0ffffh
        mov    bl, byte ptr [edi + edx + 2e4h]
        and    ebx, 0ffff00ffh
        test   bx, bx
        jne    SHORT loc_1_3B00
        mov    bx, cx
        shr    bx, 5
        add    bx, 2
loc_1_3B00:
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
        xor    edx, edx
        sar    ecx, 1
        mov    dx, word ptr [ebp - 2]
        mov    eax, ecx
        and    eax, 0ffffh
        mov    ecx, dword ptr [esi + 0deh]
        lea    eax, [edx + eax*2]
        shl    eax, 2
        cmp    ecx, 40h
        mov    dword ptr [esi + 20eh], eax
        mov    dword ptr [esi + 212h], eax
        jge    SHORT loc_1_3B72
        mov    edx, eax
        imul   edx, ecx
        shl    edx, 6
        mov    dword ptr [esi + 212h], edx
loc_1_3B72:
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [esi + 212h]
        mov    dword ptr [esi + 15ah], 5f5e100h
        add    eax, eax
        mov    dword ptr [esi + 20eh], eax
        lea    eax, [ecx + ecx*4]
        add    eax, eax
        cmp    dword ptr [esi + 1c2h], 30h
        mov    dword ptr [esi + 212h], eax
        jne    SHORT loc_1_3BAB
        mov    dword ptr [esi + 212h], 0
loc_1_3BAB:
        mov    eax, dword ptr [esi + 4]
        add    eax, dword ptr [esi + 0fah]
        mov    dword ptr [esi + 0fah], eax
        cmp    eax, 3e8h
        jbe    SHORT loc_1_3BE5
        mov    dword ptr [esi + 0fah], 0
        cmp    dword ptr [esi + 0feh], 64h
        jb     SHORT loc_1_3BDB
        add    dword ptr [esi + 0e6h], 2
loc_1_3BDB:
        mov    dword ptr [esi + 0feh], 0
loc_1_3BE5:
        push   esi
        call   sub_1_3618
        cmp    byte ptr [esi + 0b6h], 0
        jne    SHORT loc_1_3C03
        mov    ax, word ptr [esi + 0e2h]
        mov    edx, dword ptr [esi + 176h]
        out    dx, ax
loc_1_3C03:
        lea    ebx, [esi + 21eh]
        mov    edi, 4
loc_1_3C0E:
        xor    eax, eax
        mov    ax, word ptr [ebx + 6]
        cmp    eax, dword ptr [esi + 21ah]
        jl     SHORT loc_1_3C23
        push   ebx
        push   esi
        call   sub_1_1AB8
loc_1_3C23:
        lea    eax, [ebx + 40h]
        xor    ecx, ecx
        mov    cx, word ptr [eax + 6]
        cmp    ecx, dword ptr [esi + 21ah]
        jl     SHORT loc_1_3C3B
        push   eax
        push   esi
        call   sub_1_1AB8
loc_1_3C3B:
        add    ebx, 10h
        dec    edi
        jne    SHORT loc_1_3C0E
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3C58:
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
        mov    ecx, 0bdh
        xor    ebx, ebx
        db  0C7h, 045h, 0F8h, 000h, 000h, 000h, 000h
        rep stosd
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 008h, 000h
        push   ebx
        db  089h, 01Dh, 004h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 002h, 000h, 000h, 000h, 004h, 000h
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 004h, 000h, 000h, 000h
        call   sub_1_8E9A
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
        lea    eax, [ebp - 4]
        db  089h, 015h, 056h, 000h, 000h, 000h
        db  08Bh, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_8EA6
        cmp    dword ptr [ebp - 4], ebx
        je     SHORT loc_1_3DA9
        db  039h, 01Dh, 046h, 000h, 000h, 000h
        jbe    SHORT loc_1_3D91
        xor    eax, eax
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_3D91:
        push   0
        db  0A1h, 004h, 000h, 000h, 000h
        push   eax
        call   sub_1_8EA0
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_3DA9:
        xor    eax, eax
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_3DB8:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    ecx, [ebp - 4]
        push   edi
        push   eax
        push   3
        push   5d4h
        push   ecx
        call   sub_1_8F86
        test   eax, eax
        je     SHORT loc_1_3DE7
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_3DE7:
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        mov    ecx, 175h
        rep stosd
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_42F8
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_44E8
        mov    edi, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 0ch]
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   esi
        push   eax
        call   sub_1_3EB8
        test   eax, eax
        je     SHORT loc_1_3E37
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   5d4h
        push   eax
        call   sub_1_8F8C
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_3E37:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_7098
        test   eax, eax
        je     SHORT loc_1_3E77
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0aa000004h
        mov    ecx, dword ptr [eax + 3ah]
        push   ecx
        call   sub_1_9058
        add    esp, 0ch
        mov    ecx, dword ptr [ebp - 4]
        push   3
        push   5d4h
        push   ecx
        call   sub_1_8F8C
        mov    eax, 0c0010006h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_3E77:
        push   esi
        mov    eax, dword ptr [ebp - 4]
        push   edi
        push   eax
        call   sub_1_3FE8
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_1_3EA2
        push   3
        mov    eax, dword ptr [ebp - 4]
        push   5d4h
        push   eax
        call   sub_1_8F8C
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_3EA2:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3EB8:
        push   ebp
        mov    eax, 0fh
        mov    ebp, esp
        sub    esp, 24h
        mov    word ptr [ebp - 24h], ax
        push   esi
        mov    word ptr [ebp - 22h], ax
        push   edi
        mov    eax, 8
        mov    edi, dword ptr [ebp + 0ch]
        mov    word ptr [ebp - 1ch], ax
        push   edi
        mov    word ptr [ebp - 1ah], ax
        lea    ecx, [ebp - 8]
        push   ecx
        lea    eax, [ebp - 4]
        db  0C7h, 045h, 0E0h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0E8h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_9004
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_3F0C
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_3F0C:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 8]
        push   eax
        lea    ecx, [ebp - 14h]
        push   ecx
        lea    edx, [ebp - 10h]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        mov    dword ptr [esi + 8], edi
        call   sub_1_8F0C
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_3F45
        lea    edx, [esi + 3eh]
        mov    ecx, dword ptr [ebp - 10h]
        mov    eax, ecx
        mov    edi, dword ptr [ecx]
        mov    dword ptr [edx], edi
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [edx + 4], al
        mov    cl, byte ptr [ecx + 5]
        mov    byte ptr [edx + 5], cl
loc_1_3F45:
        push   1
        lea    eax, [ebp - 24h]
        push   eax
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        lea    edx, [ebp - 0ch]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_900A
        cmp    dword ptr [ebp - 4], 0
        mov    eax, 8
        jne    SHORT loc_1_3F6D
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 4]
loc_1_3F6D:
        cmp    eax, 0ffh
        ja     SHORT loc_1_3F81
        and    ax, 0ffh
        mov    word ptr [esi + 5d2h], ax
        jmp    SHORT loc_1_3F8A
loc_1_3F81:
        mov    word ptr [esi + 5d2h], 8
loc_1_3F8A:
        push   1
        lea    eax, [ebp - 1ch]
        push   eax
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        lea    edx, [ebp - 0ch]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_900A
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_3FC3
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4]
        cmp    al, 5
        jne    SHORT loc_1_3FB5
        xor    eax, eax
        jmp    SHORT loc_1_3FC8
loc_1_3FB5:
        sub    al, 2
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0fffffffdh
        add    eax, 4
        jmp    SHORT loc_1_3FC8
loc_1_3FC3:
        mov    eax, 4
loc_1_3FC8:
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        push   eax
        push   esi
        call   sub_1_6C78
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_9010
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_1_3FE8:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        lea    ecx, [ebp - 4]
        push   esi
        push   edi
        push   eax
        push   0
        push   32h
        push   ecx
        call   sub_1_8F86
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     SHORT loc_1_4019
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_4019:
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        mov    ecx, 0ch
        rep stosd
        stosw
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 142h]
        test   eax, eax
        jne    SHORT loc_1_4040
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ah], 5
        jmp    SHORT loc_1_4058
loc_1_4040:
        cmp    eax, 1
        mov    eax, dword ptr [ebp - 4]
        jne    SHORT loc_1_4051
        mov    dword ptr [eax + 0ah], 2
        jmp    SHORT loc_1_4058
loc_1_4051:
        mov    dword ptr [eax + 0ah], 0ffh
loc_1_4058:
        mov    eax, dword ptr [ebp - 4]
        xor    ecx, ecx
        lea    edi, [esi + 172h]
        mov    edx, 20h
        mov    dword ptr [eax + 16h], 1
loc_1_406F:
        mov    eax, dword ptr [esi + 16eh]
        mov    ebx, dword ptr [ebp - 4]
        add    ecx, 0ch
        cmp    ecx, 0ch
        mov    dword ptr [ebx + ecx + 0eh], eax
        mov    ebx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + ecx + 12h], edx
        mov    ebx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + ecx + 16h], edi
        jb     SHORT loc_1_406F
        mov    eax, 1
        mov    byte ptr [ebp - 18h], al
        mov    byte ptr [ebp - 17h], al
        mov    byte ptr [ebp - 16h], al
        mov    dword ptr [ebp - 15h], 0
        mov    dword ptr [ebp - 11h], 4
        mov    ecx, dword ptr [esi + 156h]
        mov    dword ptr [ebp - 0dh], ecx
        cmp    byte ptr [ebp - 18h], 0
        je     SHORT loc_1_40E9
        mov    cl, byte ptr [ebp - 17h]
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 4], cl
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 15h]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [ebp - 16h]
        mov    byte ptr [eax + 5], cl
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 11h]
        mov    dword ptr [eax + 0eh], edx
        mov    eax, dword ptr [ebp - 4]
        mov    ebx, dword ptr [ebp - 0dh]
        mov    dword ptr [eax + 12h], ebx
loc_1_40E9:
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
        call   sub_1_8EB2
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        mov    edx, dword ptr [ecx + 22h]
        cmp    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [edx]
        mov    byte ptr [esi + 0adh], 1
        mov    dword ptr [edi], eax
        jne    SHORT loc_1_4171
        push   esi
        call   sub_1_74C8
        test   eax, eax
        je     SHORT loc_1_4148
        push   0
        mov    eax, dword ptr [esi + 3ah]
        push   0aa000002h
        push   eax
        call   sub_1_9058
        mov    dword ptr [ebp - 8], 0c000009ah
        add    esp, 0ch
        jmp    NEAR PTR loc_1_429D
loc_1_4148:
        push   esi
        call   sub_1_4738
        test   eax, eax
        je     SHORT loc_1_41CE
        cmp    eax, 2
        jne    SHORT loc_1_418A
        push   0
        mov    eax, dword ptr [esi + 3ah]
        push   0aa000007h
        push   eax
        call   sub_1_9058
        mov    dword ptr [ebp - 8], 0c0010004h
        add    esp, 0ch
loc_1_4171:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   10h
        push   eax
        call   sub_1_8F8C
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_418A:
        cmp    eax, 3
        jne    SHORT loc_1_41B2
        mov    eax, dword ptr [esi + 16eh]
        mov    ecx, dword ptr [esi + 3ah]
        push   eax
        push   1
        push   0aa000001h
        push   ecx
        call   sub_1_9058
        mov    dword ptr [ebp - 8], 0c001001eh
        add    esp, 10h
        jmp    SHORT loc_1_4171
loc_1_41B2:
        push   0
        mov    eax, dword ptr [esi + 3ah]
        push   0aa000006h
        push   eax
        call   sub_1_9058
        mov    dword ptr [ebp - 8], 0c0000001h
        add    esp, 0ch
        jmp    SHORT loc_1_4171
loc_1_41CE:
        lea    ebx, [esi + 22h]
        xor    eax, eax
        mov    edi, ebx
        mov    ecx, 6
        rep stosd
        mov    eax, 2
        mov    ecx, dword ptr [esi + 14ah]
        cmp    dword ptr [esi + 146h], 1
        adc    eax, -1
        push   eax
        mov    al, byte ptr [esi + 13eh]
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
        call   sub_1_8F6E
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_4236
        mov    eax, dword ptr [esi + 14ah]
        mov    ecx, dword ptr [esi + 3ah]
        push   eax
        push   1
        push   0aa000005h
        push   ecx
        call   sub_1_9058
        add    esp, 10h
        jmp    SHORT loc_1_429D
loc_1_4236:
        push   esi
        call   sub_1_68D8
        test   eax, eax
        je     SHORT loc_1_426A
        mov    eax, dword ptr [esi + 14ah]
        mov    ecx, dword ptr [esi + 16eh]
        push   eax
        mov    edx, dword ptr [esi + 3ah]
        push   ecx
        push   2
        push   0aa000001h
        push   edx
        call   sub_1_9058
        mov    dword ptr [ebp - 8], 0c001001eh
        add    esp, 14h
        jmp    SHORT loc_1_4297
loc_1_426A:
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
loc_1_428C:
        test   al, al
        jne    SHORT loc_1_42B1
        mov    dword ptr [ebp - 8], 0c0000001h
loc_1_4297:
        push   ebx
        call   sub_1_8F74
loc_1_429D:
        push   esi
        call   sub_1_7638
        mov    eax, dword ptr [esi + 3ah]
        push   eax
        call   sub_1_8EB8
        jmp    NEAR PTR loc_1_4171
loc_1_42B1:
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
        call   sub_1_8F8C
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h
sub_1_42F8:
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
        db  048h, 021h, 040h, 0E8h, 0EFh, 038h, 000h, 000h, 05Dh, 0C2h, 004h, 000h, 08Dh, 064h, 024h, 000h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4358:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_8EAC
        push   0
        db  08Bh, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        call   sub_1_8EA0
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4388:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        db  039h, 035h, 04Ah, 000h, 000h, 000h
        jne    SHORT loc_1_43A6
        mov    eax, dword ptr [esi + 4eh]
        db  0A3h, 04Ah, 000h, 000h, 000h
        jmp    SHORT loc_1_43C2
loc_1_43A6:
        db  08Bh, 00Dh, 04Ah, 000h, 000h, 000h
        cmp    dword ptr [ecx + 4eh], esi
        je     SHORT loc_1_43B9
loc_1_43B1:
        mov    ecx, dword ptr [ecx + 4eh]
        cmp    dword ptr [ecx + 4eh], esi
        jne    SHORT loc_1_43B1
loc_1_43B9:
        mov    eax, dword ptr [ecx + 4eh]
        mov    edx, dword ptr [eax + 4eh]
        mov    dword ptr [ecx + 4eh], edx
loc_1_43C2:
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 62h]
        push   eax
        push   ecx
        mov    edi, 1
        call   sub_1_8F68
        lea    ecx, [esi + 22h]
        push   ecx
        call   sub_1_8F74
        mov    ebx, dword ptr [esi + 14ah]
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
        jmp    SHORT loc_1_4405
loc_1_4405:
        jmp    SHORT loc_1_4407
loc_1_4407:
        mov    eax, edi
        or     al, cl
        mov    ecx, dword ptr [esi + 5eh]
        out    dx, al
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4415:
        push   esi
        call   sub_1_7638
        mov    ecx, dword ptr [esi + 3ah]
        push   ecx
        call   sub_1_9064
        mov    ecx, dword ptr [esi + 3ah]
        push   ecx
        call   sub_1_8EB8
        push   0
        push   5d4h
        push   esi
        call   sub_1_8F8C
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 055h, 033h, 0D2h, 08Bh, 0ECh, 08Bh, 044h, 024h, 008h, 08Dh, 088h
        db  0CBh, 002h, 000h, 000h, 088h, 050h, 05Ah, 083h, 0C0h, 05Ah, 089h, 011h, 089h, 051h, 004h, 089h
        db  051h, 008h, 089h, 051h, 00Ch, 088h, 090h, 08Ah, 002h, 000h, 000h, 0C6h, 080h, 08Bh, 002h, 000h
        db  000h, 040h, 0C6h, 080h, 08Ch, 002h, 000h, 000h, 05Bh, 0C6h, 080h, 08Dh, 002h, 000h, 000h, 06Fh
        db  0C6h, 080h, 08Eh, 002h, 000h, 000h, 080h, 0C6h, 080h, 08Fh, 002h, 000h, 000h, 08Fh, 0C6h, 080h
        db  090h, 002h, 000h, 000h, 09Dh, 0C6h, 080h, 091h, 002h, 000h, 000h, 0A9h, 0C6h, 080h, 092h, 002h
        db  000h, 000h, 0B5h, 0C6h, 080h, 093h, 002h, 000h, 000h, 0C0h, 0C6h, 080h, 094h, 002h, 000h, 000h
        db  0CAh, 0C6h, 080h, 095h, 002h, 000h, 000h, 0D4h, 0C6h, 080h, 096h, 002h, 000h, 000h, 0DEh, 0C6h
        db  080h, 097h, 002h, 000h, 000h, 0E7h, 0C6h, 080h, 098h, 002h, 000h, 000h, 0EFh, 0C6h, 080h, 099h
        db  002h, 000h, 000h, 0F8h, 033h, 0C0h, 05Dh, 0C2h, 004h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h
        db  024h, 000h, 000h, 000h, 000h
sub_1_44E8:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 0b6h], cl
        mov    byte ptr [esi + 0b7h], cl
        mov    byte ptr [esi + 0b8h], cl
        mov    byte ptr [esi + 0b9h], cl
        mov    byte ptr [esi + 0bbh], cl
        mov    dword ptr [esi + 0c6h], 3ch
        mov    dword ptr [esi + 0ceh], 79feh
        mov    dword ptr [esi + 0e6h], ecx
        mov    dword ptr [esi + 0e2h], 8800h
        mov    dword ptr [esi + 0f6h], ecx
        mov    dword ptr [esi + 0fah], ecx
        mov    dword ptr [esi + 0feh], ecx
        add    esi, 0b6h
        mov    word ptr [esi + 4ch], 8000h
        mov    edi, 4
        mov    dword ptr [esi + 80h], ecx
        mov    dword ptr [esi + 84h], ecx
        mov    eax, 1
        mov    dword ptr [esi + 0b8h], ecx
        mov    edx, 20h
        mov    dword ptr [esi + 98h], ecx
        mov    dword ptr [esi + 9ch], 5eah
        mov    dword ptr [esi + 0a0h], 5eah
        mov    dword ptr [esi + 0b0h], edi
        mov    dword ptr [esi + 0b4h], edi
        mov    word ptr [esi + 150h], ax
        mov    word ptr [esi + 152h], dx
        mov    dword ptr [esi + 154h], 400h
        mov    dword ptr [esi + 158h], edx
        mov    dword ptr [esi + 15ch], edx
        mov    dword ptr [esi + 160h], 1388h
        mov    dword ptr [esi + 164h], 1d4ch
        mov    byte ptr [esi + 88h], al
        mov    eax, 0ffh
        mov    edx, 10h
        mov    ebx, 0faceh
        mov    dword ptr [esi + 0a4h], 5f5e100h
        mov    dword ptr [esi + 110h], 1ah
        mov    dword ptr [esi + 50h], eax
        mov    dword ptr [esi + 54h], eax
        lea    eax, [esi + 1a8h]
loc_1_4607:
        mov    word ptr [eax], 9804h
        mov    word ptr [eax + 4], dx
        mov    word ptr [eax + 6], cx
        mov    word ptr [eax + 8], cx
        mov    word ptr [eax + 0ah], cx
        mov    word ptr [eax + 0ch], bx
        mov    word ptr [eax + 0eh], bx
        mov    word ptr [eax - 40h], 9804h
        mov    word ptr [eax - 3ch], dx
        mov    word ptr [eax - 3ah], cx
        mov    word ptr [eax - 38h], cx
        mov    word ptr [eax - 36h], cx
        mov    word ptr [eax - 34h], bx
        mov    word ptr [eax - 32h], bx
        add    eax, edx
        dec    edi
        jne    SHORT loc_1_4607
        db  0C7h, 086h, 0C1h, 002h, 000h, 000h, 000h, 000h, 000h, 000h
        xor    ebx, ebx
        mov    dword ptr [esi + 2c5h], ebx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    dword ptr [esi + 2b9h], ebx
        mov    dword ptr [esi + 2bdh], ebx
        db  0C7h, 086h, 0D5h, 002h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 2d9h], ebx
        mov    dword ptr [esi + 2cdh], ebx
        mov    dword ptr [esi + 2d1h], ebx
        call   sub_1_0288
        mov    eax, 7fffh
        mov    dword ptr [esi + 114h], eax
        mov    dword ptr [esi + 118h], eax
        mov    dword ptr [esi + 11ch], eax
        mov    dword ptr [esi + 120h], eax
        mov    dword ptr [esi + 124h], eax
        mov    dword ptr [esi + 128h], eax
        mov    dword ptr [esi + 12ch], eax
        mov    dword ptr [esi + 130h], eax
        mov    dword ptr [esi + 134h], eax
        mov    dword ptr [esi + 138h], eax
        mov    dword ptr [esi + 13ch], eax
        mov    dword ptr [esi + 140h], eax
        mov    dword ptr [esi + 144h], eax
        mov    dword ptr [esi + 148h], eax
        mov    ecx, dword ptr [esi + 0a0h]
        mov    dword ptr [esi + 238h], ecx
        mov    ecx, dword ptr [esi + 0a0h]
        mov    dword ptr [esi + 234h], ecx
        mov    byte ptr [esi + 512h], bl
        xor    eax, eax
        mov    word ptr [esi + 513h], bx
        pop    edi
        mov    word ptr [esi + 515h], bx
        mov    word ptr [esi + 517h], bx
        mov    byte ptr [esi + 519h], bl
        mov    word ptr [esi + 51ah], bx
        mov    byte ptr [esi + 2b4h], 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_4738:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 172h]
        lea    edi, [esi + 0b6h]
        add    eax, 0eh
        mov    dword ptr [edi + 0c0h], eax
        mov    eax, dword ptr [edi + 0bch]
        add    eax, 0eh
        mov    dword ptr [edi + 0c4h], eax
        mov    eax, dword ptr [edi + 0bch]
        add    eax, 1bh
        mov    dword ptr [edi + 0c8h], eax
        mov    eax, dword ptr [edi + 0bch]
        add    eax, 1ah
        mov    dword ptr [edi + 0cch], eax
        mov    eax, dword ptr [edi + 0bch]
        add    eax, 1ch
        mov    dword ptr [edi + 0d0h], eax
        mov    eax, dword ptr [edi + 0bch]
        add    eax, 18h
        mov    dword ptr [edi + 0d4h], eax
        mov    eax, dword ptr [edi + 0bch]
        add    eax, 0ah
        mov    dword ptr [edi + 0d8h], eax
        mov    eax, dword ptr [edi + 0bch]
        add    eax, 10h
        mov    dword ptr [edi + 0dch], eax
        mov    eax, 807h
        mov    ebx, dword ptr [esi + 176h]
        mov    ecx, dword ptr [esi + 172h]
        mov    edx, ebx
        out    dx, ax
        lea    edx, [ecx + 0eh]
        in     ax, dx
        and    ah, 0e0h
        cmp    ah, 0e0h
        je     SHORT loc_1_47ED
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_47ED:
        lea    edx, [ecx + 6]
        mov    eax, 5555h
        out    dx, ax
        in     ax, dx
        cmp    ax, 1555h
        je     SHORT loc_1_480B
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_480B:
        mov    eax, 0aaaah
        out    dx, ax
        in     ax, dx
        cmp    ax, 0aaah
        je     SHORT loc_1_4826
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_4826:
        xor    eax, eax
        out    dx, ax
        mov    edx, ebx
        mov    eax, 7bfh
        out    dx, ax
        mov    edx, dword ptr [esi + 17ah]
loc_1_4839:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_4839
        mov    edx, ebx
        mov    eax, 800h
        out    dx, ax
        mov    eax, dword ptr [esi + 142h]
        cmp    eax, 1
        je     SHORT loc_1_4859
        cmp    eax, 3
        jne    SHORT loc_1_4881
loc_1_4859:
        lea    edx, [ecx + 8]
        in     ax, dx
        shr    ax, 0ch
        mov    ecx, eax
        and    ecx, 0ffffh
        mov    dword ptr [edi + 94h], ecx
        mov    dword ptr [edi + 90h], 0
        mov    byte ptr [edi + 88h], 1
loc_1_4881:
        push   0dh
        push   esi
        call   sub_1_7698
        cmp    dword ptr [edi + 1ch], 0
        jne    SHORT loc_1_48AD
        mov    cx, ax
        and    cx, 3f00h
        shr    cx, 8
        mov    edx, ecx
        and    edx, 0ffffh
        lea    ebx, [edx + edx*4 + 5]
        lea    ecx, [ebx + ebx*4]
        mov    dword ptr [edi + 1ch], ecx
loc_1_48AD:
        test   ah, 40h
        je     SHORT loc_1_48B6
        mov    byte ptr [edi + 3], 1
loc_1_48B6:
        test   ah, 80h
        je     SHORT loc_1_48BF
        mov    byte ptr [edi + 4], 1
loc_1_48BF:
        and    eax, 30h
        push   0fh
        push   esi
        mov    dword ptr [edi + 10ch], eax
        call   sub_1_7698
        mov    byte ptr [edi + 2b4h], 0
        test   al, 20h
        jne    SHORT loc_1_48E2
        mov    byte ptr [edi + 2b4h], 1
loc_1_48E2:
        push   0ah
        lea    ebx, [esi + 44h]
        push   esi
        call   sub_1_7698
        push   0bh
        mov    byte ptr [ebx], ah
        push   esi
        mov    byte ptr [esi + 45h], al
        call   sub_1_7698
        push   0ch
        mov    byte ptr [esi + 46h], ah
        push   esi
        mov    byte ptr [esi + 47h], al
        call   sub_1_7698
        lea    ecx, [esi + 3eh]
        mov    byte ptr [esi + 48h], ah
        mov    byte ptr [esi + 49h], al
        mov    al, byte ptr [esi + 3fh]
        or     al, byte ptr [esi + 40h]
        or     al, byte ptr [esi + 41h]
        or     al, byte ptr [esi + 42h]
        or     al, byte ptr [esi + 43h]
        or     al, byte ptr [ecx]
        jne    SHORT loc_1_4930
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx], eax
        mov    dx, word ptr [ebx + 4]
        mov    word ptr [ecx + 4], dx
loc_1_4930:
        mov    ebx, dword ptr [esi + 176h]
        mov    eax, 5800h
        mov    edx, ebx
        mov    ecx, dword ptr [esi + 17ah]
        out    dx, ax
        mov    edx, ecx
loc_1_4947:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_4947
        mov    edx, ebx
        mov    eax, 2800h
        out    dx, ax
        mov    edx, ecx
loc_1_4959:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_4959
        mov    edx, ebx
        mov    eax, 69ffh
        out    dx, ax
        mov    eax, 806h
        mov    ecx, dword ptr [esi + 172h]
        out    dx, ax
        mov    eax, 0b000h
        out    dx, ax
        mov    edx, ecx
        in     al, dx
        lea    edx, [ecx + 4]
        in     al, dx
        lea    edx, [ecx + 8]
        in     al, dx
        lea    edx, [ecx + 1]
        in     al, dx
        lea    edx, [ecx + 2]
        in     al, dx
        lea    edx, [ecx + 3]
        in     al, dx
        lea    edx, [ecx + 5]
        in     al, dx
        lea    edx, [ecx + 6]
        in     al, dx
        lea    edx, [ecx + 7]
        in     al, dx
        lea    edx, [ecx + 0ch]
        in     ax, dx
        lea    edx, [ecx + 0ah]
        in     ax, dx
        mov    edx, dword ptr [esi + 186h]
        in     ax, dx
        mov    edx, eax
        and    edx, 0ffffh
        cmp    ax, 0bd4h
        mov    dword ptr [edi + 3ch], edx
        ja     SHORT loc_1_49C7
        mov    ax, 0bd4h
loc_1_49C7:
        and    eax, 0ffffh
        mov    dword ptr [edi + 0a8h], eax
        mov    eax, 803h
        mov    edx, dword ptr [esi + 176h]
        out    dx, ax
        mov    edx, dword ptr [esi + 18eh]
        in     ax, dx
        and    eax, 0ffffh
        mov    ecx, eax
        mov    dword ptr [edi + 38h], eax
        and    ecx, 0fffffffch
        cmp    ecx, 0bd4h
        jg     SHORT loc_1_4A01
        mov    ecx, 0bd4h
loc_1_4A01:
        add    eax, 3
        xor    ebx, ebx
        and    eax, 0fffffffch
        mov    dword ptr [edi + 0ach], ecx
        add    eax, 6
        mov    dword ptr [edi + 29ch], eax
        mov    dword ptr [edi + 2a8h], ebx
        mov    dword ptr [edi + 2ach], ebx
        mov    ax, word ptr [edi + 18h]
        mov    edx, dword ptr [esi + 176h]
        out    dx, ax
        mov    eax, 71feh
        xor    ecx, ecx
        out    dx, ax
        mov    eax, 802h
        out    dx, ax
loc_1_4A40:
        mov    edx, dword ptr [esi + 172h]
        mov    eax, ebx
        inc    bx
        add    edx, ecx
        and    eax, 0ffffh
        inc    ecx
        cmp    bx, 6
        mov    al, byte ptr [eax + esi + 3eh]
        out    dx, al
        jb     SHORT loc_1_4A40
        mov    edx, dword ptr [esi + 176h]
        mov    eax, 0a800h
        out    dx, ax
        mov    eax, 5800h
        mov    ecx, dword ptr [esi + 17ah]
        out    dx, ax
        mov    edx, ecx
loc_1_4A79:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_4A79
        mov    edx, dword ptr [esi + 176h]
        mov    eax, 2800h
        out    dx, ax
        mov    edx, ecx
loc_1_4A8F:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_4A8F
        mov    ax, word ptr [edi + 2ch]
        mov    edx, dword ptr [esi + 176h]
        out    dx, ax
        mov    eax, 807h
        pop    edi
        out    dx, ax
        xor    eax, eax
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_4AB8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        push   edi
        push   eax
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_5478
        mov    edx, dword ptr [esi + 176h]
        mov    eax, 4800h
        out    dx, ax
        mov    eax, 2000h
        test   edi, edi
        out    dx, ax
        jne    SHORT loc_1_4AF0
        push   esi
        call   sub_1_4C38
loc_1_4AF0:
        push   30d40h
        call   sub_1_8F80
        cmp    edi, 4
        je     SHORT loc_1_4B04
        cmp    edi, 5
        jne    SHORT loc_1_4B11
loc_1_4B04:
        mov    edx, dword ptr [esi + 176h]
        mov    eax, 0a800h
        out    dx, ax
loc_1_4B11:
        mov    edx, dword ptr [esi + 176h]
        mov    eax, 804h
        out    dx, ax
        cmp    edi, 4
        je     SHORT loc_1_4B28
        cmp    edi, 5
        jne    SHORT loc_1_4B32
loc_1_4B28:
        mov    edx, dword ptr [esi + 172h]
        add    edx, 0ch
        in     al, dx
loc_1_4B32:
        mov    edx, dword ptr [esi + 172h]
        xor    ecx, ecx
        add    edx, 0ah
        in     ax, dx
        test   ah, 8
        je     SHORT loc_1_4B97
        cmp    edi, 4
        je     SHORT loc_1_4B55
        cmp    edi, 5
        je     SHORT loc_1_4B55
        mov    ecx, 1
        jmp    SHORT loc_1_4B97
loc_1_4B55:
        push   0c8h
        xor    ebx, ebx
        call   sub_1_8F80
        mov    ecx, 1
        mov    eax, dword ptr [esi + 172h]
        add    eax, 0ch
        mov    dword ptr [ebp - 4], eax
loc_1_4B72:
        mov    edx, dword ptr [ebp - 4]
        in     al, dx
        test   al, al
        jne    SHORT loc_1_4B95
        mov    edx, dword ptr [esi + 172h]
        add    edx, 0ah
        in     ax, dx
        test   ah, 8
        je     SHORT loc_1_4B95
        inc    ebx
        cmp    ebx, 3e8h
        jb     SHORT loc_1_4B72
        jmp    SHORT loc_1_4B97
loc_1_4B95:
        xor    ecx, ecx
loc_1_4B97:
        mov    ebx, dword ptr [esi + 176h]
        mov    eax, 5800h
        mov    edx, ebx
        mov    esi, dword ptr [esi + 17ah]
        out    dx, ax
        mov    edx, esi
loc_1_4BAE:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_4BAE
        mov    edx, ebx
        mov    eax, 2800h
        out    dx, ax
        mov    edx, esi
loc_1_4BC0:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_4BC0
        mov    edx, ebx
        mov    eax, 807h
        out    dx, ax
        cmp    edi, 4
        je     SHORT loc_1_4BDA
        cmp    edi, 5
        jne    SHORT loc_1_4BE3
loc_1_4BDA:
        mov    edx, ebx
        mov    eax, 0b000h
        out    dx, ax
loc_1_4BE3:
        mov    al, cl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4BF8:
        push   ebp
        mov    ecx, dword ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 4]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   eax
        xor    edi, edi
        push   ecx
        push   esi
        call   sub_1_5478
loc_1_4C14:
        push   esi
        call   sub_1_4C38
        test   al, al
        jne    SHORT loc_1_4C2E
        inc    edi
        cmp    edi, 3
        jb     SHORT loc_1_4C14
        xor    al, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_4C2E:
        mov    al, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
sub_1_4C38:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        xor    bl, bl
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 192h]
        mov    ecx, dword ptr [edi + 176h]
        mov    edx, ecx
        mov    dword ptr [ebp - 4], eax
        mov    eax, 803h
        out    dx, ax
        mov    edx, dword ptr [edi + 172h]
        add    edx, 6
        in     ax, dx
        or     al, 20h
        out    dx, ax
        mov    edx, ecx
        mov    eax, 4800h
        out    dx, ax
        mov    eax, 2000h
        lea    ecx, [edi + 3eh]
        out    dx, ax
        mov    eax, 8001h
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
loc_1_4CD4:
        push   esi
        call   sub_1_8F80
        mov    edx, dword ptr [edi + 17eh]
        in     al, dx
        test   al, 80h
        jne    SHORT loc_1_4CF3
        add    esi, 3e8h
        cmp    esi, 59d8h
        jb     SHORT loc_1_4CD4
loc_1_4CF3:
        mov    edx, dword ptr [edi + 176h]
        mov    eax, 5800h
        out    dx, ax
        mov    edx, dword ptr [edi + 17ah]
loc_1_4D06:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_4D06
        xor    esi, esi
loc_1_4D0F:
        mov    edx, dword ptr [edi + 18ah]
        in     ax, dx
        test   ah, 80h
        je     SHORT loc_1_4D29
        push   64h
        inc    esi
        call   sub_1_8F80
        cmp    esi, 0fh
        jb     SHORT loc_1_4D0F
loc_1_4D29:
        mov    edx, dword ptr [edi + 18ah]
        in     ax, dx
        test   ah, 0c0h
        jne    SHORT loc_1_4D38
        mov    bl, 1
loc_1_4D38:
        mov    ecx, dword ptr [edi + 176h]
        mov    eax, 2800h
        mov    edx, ecx
        out    dx, ax
        mov    edx, dword ptr [edi + 17ah]
loc_1_4D4D:
        in     ax, dx
        test   ah, 10h
        jne    SHORT loc_1_4D4D
        mov    edx, ecx
        mov    eax, 69ffh
        out    dx, ax
        mov    edx, dword ptr [edi + 172h]
        add    edx, 6
        in     ax, dx
        and    ax, 0ffdfh
        pop    edi
        out    dx, ax
        mov    edx, ecx
        mov    eax, 807h
        out    dx, ax
        mov    al, bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_4D88:
        push   ebp
        mov    eax, 804h
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, 20h
        mov    edx, dword ptr [esi + 176h]
        out    dx, ax
        mov    edx, dword ptr [esi + 172h]
        xor    eax, eax
        add    edx, 8
        out    dx, ax
loc_1_4DAF:
        push   1
        call   sub_1_8F80
        mov    edx, dword ptr [esi + 172h]
        mov    eax, 1
        add    edx, 8
        push   eax
        out    dx, ax
        call   sub_1_8F80
        mov    edx, dword ptr [esi + 172h]
        xor    eax, eax
        add    edx, 8
        dec    edi
        out    dx, ax
        jne    SHORT loc_1_4DAF
        mov    edx, dword ptr [esi + 172h]
        add    edx, 8
        in     ax, dx
        and    al, 2
        mov    edx, dword ptr [esi + 176h]
        shr    al, 1
        pop    edi
        mov    cl, al
        pop    esi
        mov    eax, 801h
        pop    ebp
        out    dx, ax
        mov    al, cl
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_4E08:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    bx, 4
        push   esi
        push   edi
        mov    eax, 4
        mov    edi, dword ptr [ebp + 8]
        mov    esi, 20h
        mov    edx, dword ptr [edi + 172h]
        add    edx, 8
        out    dx, ax
loc_1_4E2A:
        or     bl, 2
        mov    edx, dword ptr [edi + 172h]
        mov    ax, bx
        add    edx, 8
        or     bl, 1
        push   1
        out    dx, ax
        mov    ax, bx
        out    dx, ax
        mov    bx, 4
        call   sub_1_8F80
        mov    edx, dword ptr [edi + 172h]
        mov    eax, 4
        add    edx, 8
        push   1
        out    dx, ax
        call   sub_1_8F80
        dec    esi
        jne    SHORT loc_1_4E2A
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4E78:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    eax, 804h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    edx, dword ptr [esi + 176h]
        out    dx, ax
        call   sub_1_4E08
        mov    ax, word ptr [ebp + 0ch]
        and    ax, 1fh
        lea    edi, [ebp - 8]
        shl    ax, 2
        or     ax, 5002h
        mov    cx, word ptr [ebp + 10h]
        mov    bx, 4
        mov    word ptr [ebp - 8], ax
        mov    word ptr [ebp - 6], cx
loc_1_4EBA:
        mov    dword ptr [ebp - 4], 10h
loc_1_4EC1:
        mov    ax, word ptr [edi]
        test   ah, 80h
        je     SHORT loc_1_4ECE
        or     bl, 2
        jmp    SHORT loc_1_4ED3
loc_1_4ECE:
        and    bx, 0fffdh
loc_1_4ED3:
        add    ax, ax
        mov    edx, dword ptr [esi + 172h]
        mov    word ptr [edi], ax
        add    edx, 8
        mov    ax, bx
        push   1
        out    dx, ax
        or     bl, 1
        mov    ax, bx
        out    dx, ax
        mov    bx, 4
        call   sub_1_8F80
        mov    edx, dword ptr [esi + 172h]
        mov    eax, 4
        add    edx, 8
        push   1
        out    dx, ax
        call   sub_1_8F80
        dec    dword ptr [ebp - 4]
        jne    SHORT loc_1_4EC1
        add    edi, 2
        lea    eax, [ebp - 4]
        cmp    edi, eax
        jb     SHORT loc_1_4EBA
        mov    edx, dword ptr [esi + 172h]
        xor    eax, eax
        add    edx, 8
        mov    edi, 2
        out    dx, ax
loc_1_4F32:
        mov    edx, dword ptr [esi + 172h]
        mov    eax, 1
        add    edx, 8
        push   eax
        out    dx, ax
        call   sub_1_8F80
        mov    edx, dword ptr [esi + 172h]
        xor    eax, eax
        add    edx, 8
        push   1
        out    dx, ax
        call   sub_1_8F80
        dec    edi
        jne    SHORT loc_1_4F32
        mov    edx, dword ptr [esi + 176h]
        mov    eax, 801h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        out    dx, ax
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h
sub_1_4F78:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, 804h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        mov    edx, dword ptr [edi + 176h]
        out    dx, ax
        mov    si, 4
        call   sub_1_4E08
        mov    ax, word ptr [ebp + 0ch]
        and    ax, 1fh
        mov    ah, 18h
        mov    dword ptr [ebp - 4], 0eh
        lea    ebx, [eax*4]
loc_1_4FB3:
        test   bh, 80h
        je     SHORT loc_1_4FBE
        or     si, 2
        jmp    SHORT loc_1_4FC3
loc_1_4FBE:
        and    si, 0fffdh
loc_1_4FC3:
        add    bx, bx
        mov    edx, dword ptr [edi + 172h]
        mov    ax, si
        add    edx, 8
        or     si, 1
        push   1
        out    dx, ax
        mov    ax, si
        out    dx, ax
        mov    si, 4
        call   sub_1_8F80
        mov    edx, dword ptr [edi + 172h]
        mov    eax, 4
        add    edx, 8
        push   1
        out    dx, ax
        call   sub_1_8F80
        dec    dword ptr [ebp - 4]
        jne    SHORT loc_1_4FB3
        mov    edx, dword ptr [edi + 172h]
        xor    eax, eax
        add    edx, 8
        out    dx, ax
        mov    eax, 1
        out    dx, ax
        push   eax
        call   sub_1_8F80
        mov    edx, dword ptr [edi + 172h]
        xor    eax, eax
        add    edx, 8
        push   1
        out    dx, ax
        call   sub_1_8F80
        mov    edx, dword ptr [edi + 172h]
        mov    eax, 1
        add    edx, 8
        push   eax
        out    dx, ax
        call   sub_1_8F80
        mov    edx, dword ptr [edi + 172h]
        add    edx, 8
        in     ax, dx
        test   al, 2
        je     SHORT loc_1_506F
        mov    edx, dword ptr [edi + 176h]
        mov    eax, 807h
        out    dx, ax
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_506F:
        xor    eax, eax
        push   1
        out    dx, ax
        mov    esi, 10h
        call   sub_1_8F80
        mov    ebx, dword ptr [ebp + 10h]
loc_1_5082:
        shl    word ptr [ebx], 1
        mov    eax, 1
        mov    edx, dword ptr [edi + 172h]
        push   eax
        add    edx, 8
        out    dx, ax
        call   sub_1_8F80
        mov    edx, dword ptr [edi + 172h]
        add    edx, 8
        in     ax, dx
        test   al, 2
        je     SHORT loc_1_50B0
        or     byte ptr [ebx], 1
        jmp    SHORT loc_1_50B5
loc_1_50B0:
        and    word ptr [ebx], 0fffeh
loc_1_50B5:
        mov    edx, dword ptr [edi + 172h]
        xor    eax, eax
        add    edx, 8
        push   1
        out    dx, ax
        call   sub_1_8F80
        dec    esi
        jne    SHORT loc_1_5082
        mov    edx, dword ptr [edi + 172h]
        xor    eax, eax
        add    edx, 8
        mov    esi, 2
        out    dx, ax
loc_1_50DE:
        mov    edx, dword ptr [edi + 172h]
        mov    eax, 1
        add    edx, 8
        push   eax
        out    dx, ax
        call   sub_1_8F80
        mov    edx, dword ptr [edi + 172h]
        xor    eax, eax
        add    edx, 8
        push   1
        out    dx, ax
        call   sub_1_8F80
        dec    esi
        jne    SHORT loc_1_50DE
        mov    edx, dword ptr [edi + 176h]
        mov    eax, 801h
        out    dx, ax
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h
sub_1_5128:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 2]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   1
        push   esi
        call   sub_1_4F78
        test   al, al
        jne    SHORT loc_1_514B
        xor    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_514B:
        lea    eax, [ebp - 2]
        push   eax
        push   1
        push   esi
        call   sub_1_4F78
        test   al, al
        je     SHORT loc_1_516A
        test   byte ptr [ebp - 2], 4
        je     SHORT loc_1_516A
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_516A:
        xor    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_5178:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], bl
        push   esi
        call   sub_1_4D88
        test   al, al
        jne    SHORT loc_1_519E
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_519E:
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   esi
        call   sub_1_4F78
        test   al, al
        jne    SHORT loc_1_51B9
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_51B9:
        or     byte ptr [ebp - 3], 80h
        mov    eax, dword ptr [ebp - 4]
        xor    edi, edi
        push   eax
        push   0
        push   esi
        call   sub_1_4E78
loc_1_51CB:
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   esi
        call   sub_1_4F78
        test   al, al
        je     SHORT loc_1_520C
        test   byte ptr [ebp - 3], 80h
        je     SHORT loc_1_51F1
        push   30d40h
        inc    edi
        call   sub_1_8F80
        cmp    edi, 0ah
        jb     SHORT loc_1_51CB
loc_1_51F1:
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   esi
        call   sub_1_4F78
        test   al, al
        jne    SHORT loc_1_5217
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_520C:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_5217:
        lea    eax, [ebp - 6]
        push   eax
        push   1
        push   esi
        call   sub_1_4F78
        test   al, al
        jne    SHORT loc_1_5232
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_5232:
        test   byte ptr [ebp - 6], 1
        je     SHORT loc_1_5274
        lea    eax, [esi + 19ah]
        push   eax
        push   2
        push   esi
        call   sub_1_4F78
        test   al, al
        jne    SHORT loc_1_5256
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_5256:
        lea    eax, [esi + 19ch]
        push   eax
        push   3
        push   esi
        call   sub_1_4F78
        test   al, al
        jne    SHORT loc_1_5274
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_5274:
        test   byte ptr [ebp - 5], 0e0h
        je     SHORT loc_1_527C
        mov    bl, 1
loc_1_527C:
        test   byte ptr [ebp - 5], 18h
        je     SHORT loc_1_5286
        mov    byte ptr [ebp - 1], 1
loc_1_5286:
        test   bl, bl
        je     NEAR PTR loc_1_541B
        cmp    byte ptr [ebp - 1], 0
        je     NEAR PTR loc_1_53F8
        test   byte ptr [ebp - 6], 8
        je     NEAR PTR loc_1_5355
        test   byte ptr [ebp - 3], 10h
        je     SHORT loc_1_52AE
        test   byte ptr [ebp - 6], 20h
        jne    SHORT loc_1_5311
loc_1_52AE:
        or     byte ptr [ebp - 3], 12h
        mov    eax, dword ptr [ebp - 4]
        xor    edi, edi
        push   eax
        mov    ebx, 20h
        push   0
        push   esi
        call   sub_1_4E78
loc_1_52C5:
        lea    eax, [ebp - 6]
        push   eax
        push   1
        push   esi
        call   sub_1_4F78
        test   al, al
        je     SHORT loc_1_5306
        test   word ptr [ebp - 6], bx
        jne    SHORT loc_1_52EB
        push   30d40h
        inc    edi
        call   sub_1_8F80
        cmp    edi, 0ah
        jb     SHORT loc_1_52C5
loc_1_52EB:
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   esi
        call   sub_1_4F78
        test   al, al
        jne    SHORT loc_1_5311
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_5306:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_5311:
        test   byte ptr [ebp - 3], 20h
        je     SHORT loc_1_5336
        mov    al, 1
        pop    edi
        mov    dword ptr [esi + 15ah], 5f5e100h
        mov    dword ptr [esi + 1c6h], 1ah
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_5336:
        mov    al, 1
        pop    edi
        mov    dword ptr [esi + 15ah], 989680h
        mov    dword ptr [esi + 1c6h], 100h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_5355:
        or     byte ptr [ebp - 3], 20h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   0
        push   esi
        call   sub_1_4E78
        push   esi
        call   sub_1_5128
        test   al, al
        je     SHORT loc_1_538E
        mov    al, 1
        pop    edi
        mov    dword ptr [esi + 15ah], 5f5e100h
        mov    dword ptr [esi + 1c6h], 1ah
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_538E:
        and    word ptr [ebp - 4], 0dfffh
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   0
        push   esi
        call   sub_1_4E78
        push   esi
        call   sub_1_5128
        test   al, al
        je     SHORT loc_1_53C9
        mov    al, 1
        pop    edi
        mov    dword ptr [esi + 15ah], 989680h
        mov    dword ptr [esi + 1c6h], 100h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_53C9:
        or     byte ptr [ebp - 3], 20h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   0
        push   esi
        call   sub_1_4E78
        xor    al, al
        pop    edi
        mov    dword ptr [esi + 15ah], 5f5e100h
        mov    dword ptr [esi + 1c6h], 1ah
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_53F8:
        test   bl, bl
        je     SHORT loc_1_541B
        mov    al, 1
        pop    edi
        mov    dword ptr [esi + 15ah], 5f5e100h
        mov    dword ptr [esi + 1c6h], 1ah
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_541B:
        mov    al, 1
        pop    edi
        mov    dword ptr [esi + 15ah], 989680h
        mov    dword ptr [esi + 1c6h], 100h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 2]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   0
        push   esi
        call   sub_1_4F78
        and    word ptr [ebp - 2], 0fbffh
        mov    ecx, dword ptr [ebp - 2]
        push   ecx
        push   0
        push   esi
        call   sub_1_4E78
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
_LTEXT ends
        end
