; ce2ndis3_lcode_01_02.asm -- CE2NDIS3 object 1 segment _LTEXT  (part 2/2)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_4910:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0ah
        xor    eax, eax
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_4930:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 14h
        mov    dword ptr [ecx + 128h], 3
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ecx + 0ch]
        mov    edx, esi
        lea    eax, [esi + 1]
        push   0ffffh
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ecx + 120h]
        mov    dword ptr [ebp - 0ch], eax
        mov    al, 2
        out    dx, al
        call   sub_1_2240
        mov    edx, esi
        xor    al, al
        out    dx, al
        push   0ffffh
        call   sub_1_2240
        mov    edx, dword ptr [ebp - 4]
        mov    al, 4
        out    dx, al
        lea    edx, [esi + 8]
        push   0ea60h
        out    dx, al
        call   sub_1_2240
        push   esi
        call   0
        mov    edx, dword ptr [ebp - 4]
        mov    al, 40h
        out    dx, al
        push   5
        call   sub_1_2240
        lea    edx, [esi + 0dh]
        mov    al, 80h
        out    dx, al
        lea    edx, [esi + 0eh]
        out    dx, al
        lea    edx, [esi + 0fh]
        mov    al, 20h
        out    dx, al
        lea    eax, [esi + 9]
        mov    edx, eax
        xor    al, al
        out    dx, al
        lea    edx, [esi + 0bh]
        out    dx, al
        lea    edx, [esi + 0ch]
        out    dx, al
        mov    edx, dword ptr [ebp - 0ch]
        cmp    dword ptr [edx], 0
        jne    SHORT loc_1_49DA
        mov    edx, dword ptr [ebp - 4]
        mov    al, 2
        out    dx, al
        lea    eax, [esi + 8]
        mov    edx, eax
        xor    al, al
        out    dx, al
        lea    edx, [esi + 9]
        mov    al, 20h
        out    dx, al
loc_1_49DA:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 6ch]
        mov    edi, eax
        mov    dword ptr [ebp - 8], eax
        and    edi, 2
        je     SHORT loc_1_49FB
        mov    edx, dword ptr [ebp - 4]
        mov    al, 3
        out    dx, al
        lea    eax, [esi + 8]
        mov    edx, eax
        mov    ax, word ptr [ecx + 7ch]
        out    dx, ax
loc_1_49FB:
        mov    ecx, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ecx + 48h]
        and    ebx, 2
        je     SHORT loc_1_4A15
        mov    edx, dword ptr [ebp - 4]
        mov    al, 3
        out    dx, al
        mov    ax, word ptr [ecx + 60h]
        lea    edx, [esi + 0ah]
        out    dx, ax
loc_1_4A15:
        mov    edx, dword ptr [ebp - 4]
        mov    al, 1
        out    dx, al
        lea    edx, [esi + 0dh]
        out    dx, al
        xor    al, al
        test   ebx, ebx
        je     SHORT loc_1_4A27
        mov    al, 1
loc_1_4A27:
        test   edi, edi
        je     SHORT loc_1_4A2D
        or     al, 2
loc_1_4A2D:
        mov    ecx, dword ptr [ebp + 8]
        test   byte ptr [ecx + 8ch], 8
        je     SHORT loc_1_4A3B
        or     al, 4
loc_1_4A3B:
        lea    edx, [esi + 0eh]
        out    dx, al
        mov    edx, dword ptr [ebp - 4]
        mov    al, 42h
        out    dx, al
        lea    eax, [esi + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, eax
        mov    al, 20h
        out    dx, al
        cmp    dword ptr [ecx + 0bch], 0
        mov    al, 10h
        jle    SHORT loc_1_4A63
        test   byte ptr [ebp - 8], 10h
        je     SHORT loc_1_4A63
        mov    al, 11h
loc_1_4A63:
        test   byte ptr [ebp - 8], 4
        je     SHORT loc_1_4A6B
        or     al, 4
loc_1_4A6B:
        test   byte ptr [ebp - 8], 8
        jne    SHORT loc_1_4A73
        or     al, 8
loc_1_4A73:
        lea    edx, [esi + 9]
        push   esi
        out    dx, al
        call   0
        mov    edx, dword ptr [ebp - 4]
        mov    al, 1
        out    dx, al
        lea    ecx, [esi + 0ch]
        push   ecx
        call   0
        or     al, 3
        lea    edx, [esi + 0ch]
        or     al, 70h
        push   0c8h
        xor    edi, edi
        mov    ebx, 8
        out    dx, al
        mov    edx, dword ptr [ebp - 4]
        mov    al, 50h
        out    dx, al
        call   sub_1_2240
loc_1_4AAB:
        lea    edx, [esi + ebx]
        mov    ecx, dword ptr [ebp + 8]
        inc    edi
        inc    ebx
        cmp    edi, 6
        mov    al, byte ptr [ecx + edi + 0b1h]
        out    dx, al
        jl     SHORT loc_1_4AAB
        push   esi
        xor    edi, edi
        call   0
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 10h], edi
        add    ecx, 29h
        mov    dword ptr [ebp - 14h], ecx
loc_1_4AD4:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    ebx, dword ptr [ebp - 14h]
        lea    edx, [ecx + eax + 20h]
        mov    dword ptr [ebp - 8], edx
        mov    edx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebx]
        xor    ebx, ebx
        mov    al, byte ptr [eax]
        out    dx, al
loc_1_4AF1:
        lea    edx, [esi + ecx]
        mov    eax, dword ptr [ebp + 8]
        add    eax, ebx
        inc    ecx
        cmp    ecx, 10h
        mov    al, byte ptr [eax + edi + 0c0h]
        out    dx, al
        jl     SHORT loc_1_4B21
        mov    eax, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        push   0c8h
        mov    al, byte ptr [eax]
        inc    al
        out    dx, al
        call   sub_1_2240
        mov    ecx, 8
loc_1_4B21:
        inc    ebx
        cmp    ebx, 6
        jl     SHORT loc_1_4AF1
        add    edi, 6
        inc    dword ptr [ebp - 10h]
        add    dword ptr [ebp - 14h], 4
        cmp    edi, 36h
        jl     SHORT loc_1_4AD4
        mov    edx, dword ptr [ebp - 4]
        xor    al, al
        out    dx, al
        lea    ecx, [esi + 0dh]
        push   ecx
        call   0
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 0ch]
        push   esi
        mov    dword ptr [ecx + 4], eax
        call   0
        push   esi
        call   0
        and    al, 20h
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 4]
        cmp    al, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 2
        mov    dword ptr [ecx + 3ch], eax
        cmp    eax, 2
        mov    al, 2
        out    dx, al
        lea    edx, [esi + 0ah]
        jne    SHORT loc_1_4B83
        mov    al, 3bh
        jmp    SHORT loc_1_4B85
loc_1_4B83:
        mov    al, 39h
loc_1_4B85:
        out    dx, al
        push   64h
        call   sub_1_2240
        mov    edx, dword ptr [ebp - 4]
        xor    al, al
        out    dx, al
        lea    edx, [esi + 0ch]
        mov    eax, 2000h
        out    dx, ax
        mov    edx, dword ptr [ebp - 4]
        mov    al, 40h
        out    dx, al
        lea    edx, [esi + 8]
        mov    al, 24h
        out    dx, al
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 4dh], eax
        mov    dword ptr [ecx + 51h], eax
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [ecx + 0ch], eax
        mov    dword ptr [ecx + 10h], eax
        pop    edi
        pop    esi
        mov    dword ptr [ecx + 14h], eax
        pop    ebx
        mov    dword ptr [ecx + 18h], eax
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 128h], 7
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_4BE0:
        db  0A0h, 078h, 052h, 000h, 000h
        cmp    al, 0
        jne    SHORT loc_1_4C05
        db  0FEh, 005h, 078h, 052h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     SHORT loc_1_4C07
        push   0
        push   0
        call   sub_1_0030
        cmp    eax, 0
        jne    SHORT loc_1_4C07
loc_1_4C05:
        clc
        ret
loc_1_4C07:
        stc
        ret
CE2NDIS3_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_4C18
        db  0FFh, 024h, 085h, 01Ah, 04Ch, 000h, 000h
loc_1_4C18:
        clc
        ret
        db  0E0h, 04Bh, 000h, 000h, 018h, 04Ch, 000h, 000h, 018h, 04Ch, 000h, 000h, 018h, 04Ch, 000h, 000h
        db  018h, 04Ch, 000h, 000h, 018h, 04Ch, 000h, 000h, 018h, 04Ch, 000h, 000h, 018h, 04Ch, 000h, 000h
        db  018h, 04Ch, 000h, 000h, 018h, 04Ch, 000h, 000h, 018h, 04Ch, 000h, 000h, 018h, 04Ch, 000h, 000h
        db  018h, 04Ch, 000h, 000h, 018h, 04Ch, 000h, 000h, 018h, 04Ch, 000h, 000h, 018h, 04Ch, 000h, 000h
        db  018h, 04Ch, 000h, 000h, 018h, 04Ch, 000h, 000h, 018h, 04Ch, 000h, 000h, 018h, 04Ch, 000h, 000h
        db  018h, 04Ch, 000h, 000h, 018h, 04Ch, 000h, 000h, 018h, 04Ch, 000h, 000h, 018h, 04Ch, 000h, 000h
        db  018h, 04Ch, 000h, 000h, 018h, 04Ch, 000h, 000h, 0E0h, 04Bh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_1_4C90:
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
        call   sub_1_4CC0
        pop    ebp
        ret    20h
        db  08Dh, 064h, 024h, 000h
sub_1_4CC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 18h], 0eh
        jb     NEAR PTR loc_1_4D60
        mov    edx, dword ptr [ebp + 10h]
        mov    al, byte ptr [edx]
        test   al, 1
        je     SHORT loc_1_4D00
        cmp    dword ptr [edx + 2], -1
        jne    SHORT loc_1_4CF5
        cmp    al, 0ffh
        jne    SHORT loc_1_4CF5
        cmp    byte ptr [edx + 1], 0ffh
        jne    SHORT loc_1_4CF5
        mov    dword ptr [ebp - 4], 8
        jmp    SHORT loc_1_4D67
loc_1_4CF5:
        mov    dword ptr [ebp - 4], 2
        jmp    SHORT loc_1_4D67
        db  08Bh, 0FFh
loc_1_4D00:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0ch], 20h
        je     SHORT loc_1_4D50
        mov    edx, dword ptr [ebp + 10h]
        lea    ecx, [esi + 2ch]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_4D26
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_4D2D
loc_1_4D26:
        mov    dword ptr [ebp - 10h], 1
loc_1_4D2D:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_4D40
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_4D6A
        db  08Dh, 064h, 024h, 000h
loc_1_4D40:
        mov    dword ptr [ebp - 4], 20h
        jmp    SHORT loc_1_4D6A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4D50:
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_4D6A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4D60:
        mov    dword ptr [ebp - 4], 20h
loc_1_4D67:
        mov    esi, dword ptr [ebp + 8]
loc_1_4D6A:
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     NEAR PTR loc_1_4E61
loc_1_4D75:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, dword ptr [ebp - 4]
        and    eax, ebx
        test   al, 9
        jne    SHORT loc_1_4DCD
        test   al, 2
        je     SHORT loc_1_4DB9
        lea    eax, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 026h, 0BAh, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_1_4DB9
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 19h]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0ch]
        test   dword ptr [ecx + eax*4], edx
        jne    SHORT loc_1_4DCD
loc_1_4DB9:
        test   byte ptr [ebp - 4], 2
        je     SHORT loc_1_4DC4
        test   bl, 4
        jne    SHORT loc_1_4DCD
loc_1_4DC4:
        test   bl, 20h
        je     NEAR PTR loc_1_4E56
loc_1_4DCD:
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
        call   sub_1_5042
        mov    byte ptr [edi + 18h], 1
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_4E56
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_4E0F
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_4E0F:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_4E20
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_4E26
        db  08Bh, 0FFh
loc_1_4E20:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_4E26:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_4E35
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_5048
loc_1_4E35:
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
        call   sub_1_50EC
        mov    edi, ebx
        jmp    SHORT loc_1_4E59
loc_1_4E56:
        mov    edi, dword ptr [edi + 10h]
loc_1_4E59:
        or     edi, edi
        jne    NEAR PTR loc_1_4D75
loc_1_4E61:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_4E70:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4E80
        pop    ebp
        ret    4
sub_1_4E80:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     SHORT loc_1_4EFE
loc_1_4E90:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_4EF7
        mov    byte ptr [edi + 18h], 0
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_5048
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_4EF7
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_4EBC
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_4EBC:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_4ED0
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_4ED6
        db  005h, 000h, 000h, 000h, 000h
loc_1_4ED0:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_4ED6:
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
        call   sub_1_50EC
        mov    edi, ebx
        jmp    SHORT loc_1_4EFA
loc_1_4EF7:
        mov    edi, dword ptr [edi + 10h]
loc_1_4EFA:
        or     edi, edi
        jne    SHORT loc_1_4E90
loc_1_4EFE:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4F10:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0ch]
        test   al, 20h
        je     SHORT loc_1_4F25
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_4F25:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     SHORT loc_1_4F61
        cmp    dword ptr [ecx + 2], -1
        jne    SHORT loc_1_4F52
        cmp    bl, 0ffh
        jne    SHORT loc_1_4F52
        cmp    byte ptr [ecx + 1], 0ffh
        jne    SHORT loc_1_4F52
        test   al, 8
        je     SHORT loc_1_4F4B
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_4F4B:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_4F52:
        test   al, 4
        jne    SHORT loc_1_4F5A
        test   al, 2
        je     SHORT loc_1_4F79
loc_1_4F5A:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_4F61:
        mov    eax, dword ptr [edx + 2eh]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_4F79
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 2ch], ax
        jne    SHORT loc_1_4F79
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_4F79:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
sub_1_4F80:
        push   esi
        mov    ecx, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        cld
        rep outsb
        pop    esi
        ret    0ch
sub_1_4F94:
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
sub_1_4FBD:
        push   edi
        mov    ecx, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        cld
        rep insb
        pop    edi
        ret    0ch
sub_1_4FD1:
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
sub_1_4FFA:
        db  0CDh, 020h, 031h, 080h, 028h, 000h
sub_1_5000:
        db  0CDh, 020h, 032h, 080h, 028h, 000h
sub_1_5006:
        db  0CDh, 020h, 033h, 080h, 028h, 000h
sub_1_500C:
        db  0CDh, 020h, 034h, 080h, 028h, 000h
sub_1_5012:
        db  0CDh, 020h, 035h, 080h, 028h, 000h
sub_1_5018:
        db  0CDh, 020h, 036h, 080h, 028h, 000h
        db  0CDh, 020h, 037h, 080h, 028h, 000h
sub_1_5024:
        db  0CDh, 020h, 038h, 080h, 028h, 000h
sub_1_502A:
        db  0CDh, 020h, 039h, 080h, 028h, 000h
        db  0CDh, 020h, 03Ah, 080h, 028h, 000h
        db  0CDh, 020h, 03Bh, 080h, 028h, 000h
        db  0CDh, 020h, 03Ch, 080h, 028h, 000h
sub_1_5042:
        db  0CDh, 020h, 03Dh, 080h, 028h, 000h
sub_1_5048:
        db  0CDh, 020h, 03Eh, 080h, 028h, 000h
sub_1_504E:
        db  0CDh, 020h, 03Fh, 080h, 028h, 000h
        db  0CDh, 020h, 040h, 080h, 028h, 000h
        db  0CDh, 020h, 041h, 080h, 028h, 000h
        db  0CDh, 020h, 009h, 080h, 028h, 000h
        db  0CDh, 020h, 008h, 080h, 028h, 000h
sub_1_506C:
        db  0CDh, 020h, 044h, 080h, 028h, 000h
        db  0CDh, 020h, 048h, 080h, 028h, 000h
        db  0CDh, 020h, 049h, 080h, 028h, 000h
sub_1_507E:
        db  0CDh, 020h, 04Fh, 080h, 028h, 000h
        db  0CDh, 020h, 057h, 080h, 028h, 000h
        db  0CDh, 020h, 058h, 080h, 028h, 000h
        db  0CDh, 020h, 059h, 080h, 028h, 000h
        db  0CDh, 020h, 05Ah, 080h, 028h, 000h
        db  0CDh, 020h, 05Bh, 080h, 028h, 000h
        db  0CDh, 020h, 05Ch, 080h, 028h, 000h
        db  0CDh, 020h, 05Dh, 080h, 028h, 000h
        db  0CDh, 020h, 05Eh, 080h, 028h, 000h
sub_1_50B4:
        db  0CDh, 020h, 056h, 080h, 028h, 000h
        add    byte ptr [eax], al
        db  0CDh, 020h, 000h, 080h, 028h, 000h
sub_1_50C2:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
sub_1_50C8:
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
sub_1_50CE:
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
sub_1_50D4:
        db  0CDh, 020h, 010h, 080h, 028h, 000h
        db  0CDh, 020h, 011h, 080h, 028h, 000h
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_50E6:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_50EC:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_5110:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_5134:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_5158:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
        db  0CDh, 020h, 028h, 080h, 028h, 000h
sub_1_5164:
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_516A:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_5170:
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
sub_1_51B2:
        db  0CDh, 020h, 046h, 080h, 028h, 000h
sub_1_51B8:
        db  0CDh, 020h, 045h, 080h, 028h, 000h
        db  0CDh, 020h, 043h, 080h, 028h, 000h
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        add    byte ptr [eax], al
sub_1_51CC:
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
sub_1_51EC:
        push   esi
        push   edi
        pushfd
        mov    ecx, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        cmp    esi, edi
        jae    SHORT loc_1_520C
        or     ecx, ecx
        je     SHORT loc_1_5222
        std
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    SHORT loc_1_5220
loc_1_520C:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     SHORT loc_1_5220
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd
        mov    cl, al
loc_1_5220:
        rep movsb
loc_1_5222:
        popfd
        pop    edi
        pop    esi
        ret    0ch
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 00Ah, 003h, 000h, 000h, 003h, 00Ah, 000h, 000h, 043h, 045h, 032h, 04Eh
        db  044h, 049h, 053h, 033h, 000h, 040h, 000h, 054h, 009h, 04Ch, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_5280:
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
loc_1_52FC:
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
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
loc_1_5358:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_535C:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_5360:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        inc    ebx
        inc    ebp
        xor    cl, byte ptr [esi + 44h]
        dec    ecx
        push   ebx
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ebx + 45h], al
        xor    cl, byte ptr [esi + 44h]
        dec    ecx
        push   ebx
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ecx + 64h], al
        popad
        db  070h, 074h
        jb     NEAR PTR loc_1_53D8
        jns    SHORT loc_1_53F6
        add    byte ptr gs:[ecx + 64h], al
        popad
        db  070h, 074h
        jb     NEAR PTR loc_1_53E4
        jns    SHORT loc_1_5402
        add    byte ptr gs:[ecx + 6fh], cl
        inc    edx
        popad
        jae    SHORT loc_1_53FF
        inc    ecx
        jb     NEAR PTR loc_1_5404
        db  073h, 073h
        add    byte ptr [eax], al
        add    byte ptr [ecx + 6fh], cl
        inc    edx
        popad
        jae    SHORT loc_1_540F
        inc    ecx
        db  064h, 064h, 072h, 065h
        jae    SHORT loc_1_5424
        add    byte ptr [eax], al
        add    byte ptr [ecx + 6eh], cl
        je     SHORT loc_1_541D
        db  072h, 072h
        db  075h, 070h
        db  074h, 04Eh
        db  075h, 06Dh
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [ecx + 6eh], cl
        db  074h, 065h
        db  072h, 072h
        db  075h, 070h
        db  074h, 04Eh
        jne    SHORT loc_1_543D
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [ecx + 52h], cl
        push   ecx
        dec    esp
loc_1_53D8:
        db  065h, 076h, 065h
        insb
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_53E0:
        dec    ecx
        push   edx
        push   ecx
        dec    esp
loc_1_53E4:
        db  065h, 076h, 065h
        insb
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ebp
        insd
        outsd
        db  072h, 079h
        inc    edx
        popad
        jae    SHORT loc_1_545B
loc_1_53F6:
        inc    ecx
        jb     NEAR PTR loc_1_5460
        jae    SHORT loc_1_5470
        add    byte ptr [eax], al
loc_1_53FF:
        add    byte ptr [ebp + 65h], cl
loc_1_5402:
        insd
        outsd
loc_1_5404:
        db  072h, 079h
        inc    edx
        popad
        db  073h, 065h
        inc    ecx
        jb     NEAR PTR loc_1_5474
loc_1_540F:
        jae    SHORT loc_1_5484
        add    byte ptr [eax], al
        add    byte ptr [eax + 43h], dl
        inc    ebx
        inc    ecx
        push   edx
        inc    esp
        inc    ecx
        db  074h, 074h
loc_1_541D:
        jb     SHORT loc_1_5488
        bound  esi, qword ptr [ebp + 74h]
        dec    ebp
loc_1_5424:
        insd
        outsd
        db  072h, 079h
        inc    ecx
        jb     NEAR PTR loc_1_5493
        jae    SHORT loc_1_54A3
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   eax
        inc    ebx
        inc    ebx
        inc    ecx
        push   edx
        inc    esp
        inc    ecx
        db  074h, 074h
loc_1_543D:
        db  072h, 069h
        bound  esi, qword ptr [ebp + 74h]
        dec    ebp
        insd
        outsd
        jb     SHORT loc_1_54C2
        inc    ecx
        db  064h, 064h, 072h, 065h
        db  073h, 073h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ebp
        popad
        db  078h, 069h
        insd
        db  075h, 06Dh
loc_1_545B:
        dec    ebp
        db  075h, 06Ch
        db  074h, 069h
loc_1_5460:
        arpl   word ptr [ecx + 73h], sp
        db  074h, 04Ch
        imul   esi, dword ptr [ebx + 74h], 0
loc_1_546C:
        dec    ebp
        popad
        db  078h, 069h
loc_1_5470:
        insd
        jne    SHORT loc_1_54E0
        dec    ebp
loc_1_5474:
        jne    SHORT loc_1_54E2
        db  074h, 069h
        arpl   word ptr [ecx + 73h], sp
        db  074h, 04Ch
        imul   esi, dword ptr [ebx + 74h], 0
loc_1_5484:
        inc    esi
        db  075h, 06Ch
        insb
loc_1_5488:
        inc    esp
        jne    SHORT loc_1_54FB
        insb
        js     NEAR PTR loc_1_548F
loc_1_548F:
        add    byte ptr [esi + 75h], al
        insb
loc_1_5493:
        insb
        inc    esp
        jne    SHORT loc_1_5507
        insb
        js     NEAR PTR loc_1_549B
loc_1_549B:
        add    byte ptr [ecx + 4fh], cl
        sub    eax, 796c6e4fh
loc_1_54A3:
        add    byte ptr [ecx + 4fh], cl
        sub    eax, 796c6e4fh
        add    byte ptr [ebp + 61h], al
        jb     SHORT loc_1_551C
        jns    SHORT loc_1_5506
        db  072h, 061h
        outsb
        db  073h, 06Dh
        imul   esi, dword ptr [eax + eax], 72614500h
        insb
        db  079h, 054h
loc_1_54C2:
        db  072h, 061h
        outsb
        db  073h, 06Dh
        imul   esi, dword ptr [eax + eax], 73655200h
        outsd
        db  075h, 072h
        arpl   word ptr [ebp + 4dh], sp
        popad
        outsb
        popad
        jb     NEAR PTR loc_1_54DC
loc_1_54DC:
        push   edx
        jae    NEAR PTR loc_1_554F
loc_1_54E0:
        db  075h, 072h
loc_1_54E2:
        arpl   word ptr [ebp + 4dh], sp
        popad
        outsb
        popad
        jb     NEAR PTR loc_1_54EC
loc_1_54EC:
        pop    eax
        imul   esi, dword ptr [edx + 63h], 50206d6fh
        jb     NEAR PTR loc_1_555D
        outsd
        jb     SHORT loc_1_5567
        popad
loc_1_54FB:
        outsb
        arpl   word ptr [ebp + 20h], sp
        push   ebx
        db  065h, 072h, 069h
        db  065h, 073h, 020h
loc_1_5506:
        inc    ebp
loc_1_5507:
        je     SHORT loc_1_5571
        db  065h, 072h, 06Eh
        je     NEAR PTR loc_1_552F
        inc    ecx
        popad
        db  070h, 074h
        jb     NEAR PTR loc_1_5537
        inc    esp
        jb     SHORT loc_1_5583
        jbe    SHORT loc_1_5581
loc_1_551C:
        jb     SHORT loc_1_551E
loc_1_551E:
        add    byte ptr [eax], al
loc_1_5520:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_552F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5537:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_554F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_555D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5567:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5571:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5581:
        add    byte ptr [eax], al
loc_1_5583:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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
sub_1_57A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_57AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_57B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_57B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_57B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_57BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_57C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_57C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_57C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_57CC:
        add    byte ptr [eax], al
loc_1_57CE:
        add    byte ptr [eax], al
loc_1_57D0:
        add    byte ptr [eax], al
loc_1_57D2:
        add    byte ptr [eax], al
loc_1_57D4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_5860:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
