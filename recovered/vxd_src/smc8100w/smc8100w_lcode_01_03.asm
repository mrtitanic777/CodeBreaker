; smc8100w_lcode_01_03.asm -- SMC8100W object 1 segment _LTEXT  (part 3/3)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_A564:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_67FC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9300
        lea    eax, [ebp - 8]
        push   eax
        push   0b301h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7BE5
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_A5C4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_A6C2
loc_1_A5C4:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_A5EA
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_A6C2
loc_1_A5EA:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 99h
        push   eax
        push   0b402h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7BE5
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_A627
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_A6C2
loc_1_A627:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_A64D
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_A6C2
loc_1_A64D:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 97h
        push   eax
        push   0b301h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7BE5
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_A68A
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_A6C2
loc_1_A68A:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_A6B0
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_A6C2
loc_1_A6B0:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_A6C2
loc_1_A6C2:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_A6C9:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        push   4
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6B39
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_A6F4
loc_1_A6F0:
        inc    word ptr [ebp - 8]
loc_1_A6F4:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 5dch
        jge    NEAR PTR loc_1_A765
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_BA5F
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_A760
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_A73B
        mov    ax, 92h
        jmp    NEAR PTR loc_1_A7DD
        db  0E9h, 025h, 000h, 000h, 000h
loc_1_A73B:
        mov    word ptr [ebp - 4], 1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_BA5F
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_A760
        mov    ax, 92h
        jmp    NEAR PTR loc_1_A7DD
loc_1_A760:
        jmp    NEAR PTR loc_1_A6F0
loc_1_A765:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_B8A2
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_A79A
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_B8A2
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_A79A
        mov    ax, 92h
        jmp    NEAR PTR loc_1_A7DD
loc_1_A79A:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 114h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_A7CC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 118h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_A7D5
loc_1_A7CC:
        mov    ax, 92h
        jmp    NEAR PTR loc_1_A7DD
loc_1_A7D5:
        xor    ax, ax
        jmp    NEAR PTR loc_1_A7DD
loc_1_A7DD:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_A7E4:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    cl, byte ptr [eax]
        cmp    cl, 2
        jb     SHORT loc_1_A830
        cmp    cl, 0feh
        ja     SHORT loc_1_A830
        mov    dl, byte ptr [ebp + 10h]
        and    dl, 0f0h
        cmp    dl, 40h
        je     SHORT loc_1_A828
        cmp    cl, 2
        jb     SHORT loc_1_A814
        cmp    cl, 14h
        jae    SHORT loc_1_A814
        mov    ax, 8006h
        pop    ebp
        ret    0ch
loc_1_A814:
        cmp    byte ptr [eax + 2], 0
        jne    SHORT loc_1_A820
        cmp    byte ptr [eax + 3], 0
        je     SHORT loc_1_A828
loc_1_A820:
        mov    ax, 8006h
        pop    ebp
        ret    0ch
loc_1_A828:
        mov    ax, 1
        pop    ebp
        ret    0ch
loc_1_A830:
        mov    ax, 8005h
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_A844:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   esi
        cmp    byte ptr [eax], 4
        je     SHORT loc_1_A85A
        mov    ax, 8005h
        pop    esi
        pop    ebp
        ret    8
loc_1_A85A:
        add    eax, 2
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   0bb01h
        push   esi
        call   sub_1_7BE5
        push   esi
        call   sub_1_685C
        test   ax, ax
        jne    SHORT loc_1_A87B
        mov    ax, 1
loc_1_A87B:
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_A884:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   esi
        cmp    byte ptr [eax], 6
        je     SHORT loc_1_A89A
        mov    ax, 8005h
        pop    esi
        pop    ebp
        ret    8
loc_1_A89A:
        add    eax, 2
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   3b02h
        push   esi
        call   sub_1_7BE5
        push   esi
        call   sub_1_685C
        test   ax, ax
        jne    SHORT loc_1_A8BB
        mov    ax, 1
loc_1_A8BB:
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_A8C4:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 4
        cmp    byte ptr [eax], 4
        push   esi
        je     SHORT loc_1_A8DF
        mov    ax, 8005h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_A8DF:
        mov    cl, byte ptr [eax + 2]
        mov    al, byte ptr [eax + 3]
        and    ecx, 0ffff00ffh
        and    eax, 0ffff00ffh
        shl    cx, 8
        mov    esi, dword ptr [ebp + 8]
        or     cx, ax
        imul   cx, cx, 0ah
        mov    word ptr [ebp - 2], cx
        lea    ecx, [ebp - 2]
        push   ecx
        push   0aa01h
        push   esi
        call   sub_1_7B23
        push   esi
        call   sub_1_685C
        test   ax, ax
        jne    SHORT loc_1_A920
        mov    ax, 1
loc_1_A920:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_A934:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        cmp    byte ptr [eax], 4
        je     SHORT loc_1_A948
        mov    ax, 8005h
        pop    ebp
        ret    0ch
loc_1_A948:
        mov    cl, byte ptr [eax + 2]
        mov    edx, dword ptr [ebp + 10h]
        and    ecx, 0ffff00ffh
        mov    al, byte ptr [eax + 3]
        shl    cx, 8
        and    eax, 0ffff00ffh
        or     cx, ax
        pop    ebp
        mov    ax, 1
        mov    word ptr [edx], cx
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_A974:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        cmp    byte ptr [eax], 4
        je     SHORT loc_1_A988
        mov    ax, 8005h
        pop    ebp
        ret    8
loc_1_A988:
        mov    cl, byte ptr [eax + 2]
        mov    edx, dword ptr [ebp + 8]
        and    ecx, 0ffff00ffh
        mov    al, byte ptr [eax + 3]
        shl    cx, 8
        and    eax, 0ffff00ffh
        or     cx, ax
        pop    ebp
        mov    ax, 1
        mov    word ptr [edx + 91h], cx
        ret    8
        db  08Bh, 0FFh
sub_1_A9B4:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        cmp    byte ptr [eax], 4
        je     SHORT loc_1_A9C8
        mov    ax, 8005h
        pop    ebp
        ret    8
loc_1_A9C8:
        mov    cl, byte ptr [eax + 2]
        mov    edx, dword ptr [ebp + 8]
        and    ecx, 0ffff00ffh
        mov    al, byte ptr [eax + 3]
        shl    cx, 8
        and    eax, 0ffff00ffh
        or     cx, ax
        pop    ebp
        mov    ax, 1
        mov    word ptr [edx + 93h], cx
        ret    8
        db  08Bh, 0FFh
sub_1_A9F4:
        push   ebp
        mov    ebp, esp
        push   esi
        push   3e03h
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_7CDB
        push   esi
        call   sub_1_685C
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_AA14:
        push   ebp
        mov    ebp, esp
        push   esi
        push   3b02h
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_7CDB
        push   esi
        call   sub_1_685C
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_AA34:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0b702h
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_7CDB
        push   esi
        call   sub_1_685C
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_AA54:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0b402h
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_7CDB
        push   esi
        call   sub_1_685C
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_AA74:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0b003h
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_7CDB
        push   esi
        call   sub_1_685C
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_AA94:
        push   ebp
        mov    ecx, dword ptr [esp + 10h]
        mov    ax, word ptr [esp + 14h]
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx], ah
        mov    byte ptr [ecx + 1], al
        mov    al, byte ptr [edx + 24h]
        mov    byte ptr [ecx + 8], al
        mov    al, byte ptr [edx + 25h]
        mov    byte ptr [ecx + 9], al
        mov    al, byte ptr [edx + 26h]
        mov    byte ptr [ecx + 0ah], al
        mov    al, byte ptr [edx + 27h]
        mov    byte ptr [ecx + 0bh], al
        mov    al, byte ptr [edx + 28h]
        mov    byte ptr [ecx + 0ch], al
        xor    eax, eax
        mov    dl, byte ptr [edx + 29h]
        mov    al, byte ptr [ecx + 11h]
        mov    byte ptr [ecx + 0dh], dl
        cmp    eax, 20h
        je     SHORT loc_1_AB09
        cmp    eax, 2ah
        je     SHORT loc_1_AB26
        mov    edx, dword ptr [ebp + 0ch]
        mov    al, byte ptr [edx + 8]
        mov    byte ptr [ecx + 2], al
        mov    al, byte ptr [edx + 9]
        mov    byte ptr [ecx + 3], al
        mov    al, byte ptr [edx + 0ah]
        mov    byte ptr [ecx + 4], al
        mov    al, byte ptr [edx + 0bh]
        pop    ebp
        mov    byte ptr [ecx + 5], al
        mov    al, byte ptr [edx + 0ch]
        mov    byte ptr [ecx + 6], al
        xor    ax, ax
        mov    dl, byte ptr [edx + 0dh]
        mov    byte ptr [ecx + 7], dl
        ret    10h
loc_1_AB09:
        mov    byte ptr [ecx + 2], 0c0h
        mov    byte ptr [ecx + 7], 2
        xor    eax, eax
        pop    ebp
        mov    byte ptr [ecx + 3], al
        mov    byte ptr [ecx + 4], al
        mov    byte ptr [ecx + 5], al
        mov    byte ptr [ecx + 6], al
        xor    ax, ax
        ret    10h
loc_1_AB26:
        mov    byte ptr [ecx + 2], 0c0h
        mov    byte ptr [ecx + 7], 10h
        xor    eax, eax
        pop    ebp
        mov    byte ptr [ecx + 3], al
        mov    byte ptr [ecx + 4], al
        mov    byte ptr [ecx + 5], al
        mov    byte ptr [ecx + 6], al
        xor    ax, ax
        ret    10h
        db  090h
sub_1_AB44:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    byte ptr [eax], 0fah
        mov    byte ptr [eax + 1], 26h
        xor    ax, ax
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_AB64:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    cx, word ptr [esp + 10h]
        mov    ebp, esp
        pop    ebp
        mov    byte ptr [eax], 4
        mov    byte ptr [eax + 2], ch
        mov    byte ptr [eax + 3], cl
        mov    byte ptr [eax + 1], 9
        xor    ax, ax
        ret    0ch
sub_1_AB84:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    byte ptr [eax], 4
        mov    byte ptr [eax + 1], 2ah
        mov    byte ptr [eax + 2], 1
        mov    cl, byte ptr [ebp + 10h]
        pop    ebp
        mov    byte ptr [eax + 3], cl
        xor    ax, ax
        ret    0ch
        db  090h
sub_1_ABA4:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 0ch]
        mov    byte ptr [ecx], 0ch
        mov    byte ptr [ecx + 2], al
        mov    byte ptr [ecx + 3], al
        mov    byte ptr [ecx + 4], al
        mov    byte ptr [ecx + 1], 23h
        mov    byte ptr [ecx + 5], 0e2h
        mov    byte ptr [ecx + 6], 0d4h
        mov    byte ptr [ecx + 7], 0c3h
        mov    byte ptr [ecx + 8], 40h
        mov    byte ptr [ecx + 9], 0e5h
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 95h]
        shr    dx, 4
        sub    dl, 10h
        pop    ebp
        mov    byte ptr [ecx + 0ah], dl
        mov    ax, word ptr [eax + 95h]
        and    al, 0fh
        sub    al, 10h
        mov    byte ptr [ecx + 0bh], al
        xor    ax, ax
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_AC04:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 0ch]
        mov    byte ptr [eax], 8
        mov    byte ptr [eax + 2], cl
        mov    byte ptr [eax + 3], cl
        mov    byte ptr [eax + 4], cl
        pop    ebp
        mov    byte ptr [eax + 5], cl
        mov    byte ptr [eax + 6], cl
        mov    byte ptr [eax + 7], cl
        mov    byte ptr [eax + 1], 29h
        xor    ax, ax
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_AC34:
        push   ebp
        mov    ecx, 0f0h
        mov    ebp, esp
        mov    eax, dword ptr [esp + 0ch]
        mov    byte ptr [eax], 14h
        mov    byte ptr [eax + 2], cl
        mov    byte ptr [eax + 3], cl
        mov    byte ptr [eax + 4], cl
        mov    byte ptr [eax + 5], cl
        mov    byte ptr [eax + 6], cl
        mov    byte ptr [eax + 7], cl
        mov    byte ptr [eax + 8], cl
        mov    byte ptr [eax + 9], cl
        mov    byte ptr [eax + 0ah], cl
        mov    byte ptr [eax + 0bh], cl
        mov    byte ptr [eax + 0ch], cl
        mov    byte ptr [eax + 0dh], cl
        mov    byte ptr [eax + 0eh], cl
        mov    byte ptr [eax + 0fh], cl
        mov    byte ptr [eax + 10h], cl
        pop    ebp
        mov    byte ptr [eax + 11h], cl
        mov    byte ptr [eax + 12h], cl
        mov    byte ptr [eax + 13h], cl
        mov    byte ptr [eax + 1], 22h
        xor    ax, ax
        ret    8
sub_1_AC84:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_A9F4
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 1], 2
        mov    byte ptr [ecx], 8
        mov    eax, dword ptr [esi + 0e4h]
        mov    ax, word ptr [eax]
        mov    byte ptr [ecx + 2], ah
        mov    edx, dword ptr [esi + 0e4h]
        mov    al, byte ptr [edx]
        mov    byte ptr [ecx + 3], al
        mov    edx, dword ptr [esi + 0e4h]
        mov    ax, word ptr [edx + 2]
        mov    byte ptr [ecx + 4], ah
        mov    edx, dword ptr [esi + 0e4h]
        mov    al, byte ptr [edx + 2]
        mov    byte ptr [ecx + 5], al
        mov    edx, dword ptr [esi + 0e4h]
        mov    ax, word ptr [edx + 4]
        mov    byte ptr [ecx + 6], ah
        mov    edx, dword ptr [esi + 0e4h]
        pop    esi
        pop    ebp
        mov    al, byte ptr [edx + 4]
        mov    byte ptr [ecx + 7], al
        xor    ax, ax
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_ACF4:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_AA14
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 1], 0bh
        mov    byte ptr [ecx], 6
        mov    eax, dword ptr [esi + 0e4h]
        mov    ax, word ptr [eax]
        mov    byte ptr [ecx + 2], ah
        mov    edx, dword ptr [esi + 0e4h]
        mov    al, byte ptr [edx]
        mov    byte ptr [ecx + 3], al
        mov    edx, dword ptr [esi + 0e4h]
        mov    ax, word ptr [edx + 2]
        mov    byte ptr [ecx + 4], ah
        mov    edx, dword ptr [esi + 0e4h]
        pop    esi
        pop    ebp
        mov    al, byte ptr [edx + 2]
        mov    byte ptr [ecx + 5], al
        xor    ax, ax
        ret    8
        db  090h
sub_1_AD44:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_AA34
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 1], 2ch
        mov    byte ptr [ecx], 6
        mov    eax, dword ptr [esi + 0e4h]
        mov    ax, word ptr [eax]
        mov    byte ptr [ecx + 2], ah
        mov    edx, dword ptr [esi + 0e4h]
        mov    al, byte ptr [edx]
        mov    byte ptr [ecx + 3], al
        mov    edx, dword ptr [esi + 0e4h]
        mov    ax, word ptr [edx + 2]
        mov    byte ptr [ecx + 4], ah
        mov    edx, dword ptr [esi + 0e4h]
        pop    esi
        pop    ebp
        mov    al, byte ptr [edx + 2]
        mov    byte ptr [ecx + 5], al
        xor    ax, ax
        ret    8
        db  090h
sub_1_AD94:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_AA54
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 1], 2bh
        mov    byte ptr [ecx], 6
        mov    eax, dword ptr [esi + 0e4h]
        mov    ax, word ptr [eax]
        mov    byte ptr [ecx + 2], ah
        mov    edx, dword ptr [esi + 0e4h]
        mov    al, byte ptr [edx]
        mov    byte ptr [ecx + 3], al
        mov    edx, dword ptr [esi + 0e4h]
        mov    ax, word ptr [edx + 2]
        mov    byte ptr [ecx + 4], ah
        mov    edx, dword ptr [esi + 0e4h]
        pop    esi
        pop    ebp
        mov    al, byte ptr [edx + 2]
        mov    byte ptr [ecx + 5], al
        xor    ax, ax
        ret    8
        db  090h
sub_1_ADE4:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_AA74
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 1], 28h
        mov    byte ptr [ecx], 8
        mov    eax, dword ptr [esi + 0e4h]
        mov    ax, word ptr [eax]
        mov    byte ptr [ecx + 2], ah
        mov    edx, dword ptr [esi + 0e4h]
        mov    al, byte ptr [edx]
        mov    byte ptr [ecx + 3], al
        mov    edx, dword ptr [esi + 0e4h]
        mov    ax, word ptr [edx + 2]
        mov    byte ptr [ecx + 4], ah
        mov    edx, dword ptr [esi + 0e4h]
        mov    al, byte ptr [edx + 2]
        mov    byte ptr [ecx + 5], al
        mov    edx, dword ptr [esi + 0e4h]
        mov    ax, word ptr [edx + 4]
        mov    byte ptr [ecx + 6], ah
        mov    edx, dword ptr [esi + 0e4h]
        pop    esi
        pop    ebp
        mov    al, byte ptr [edx + 4]
        mov    byte ptr [ecx + 7], al
        xor    ax, ax
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_AE54:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    byte ptr [eax], 4
        mov    byte ptr [eax + 1], 6
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    dx, word ptr [ecx + 91h]
        mov    byte ptr [eax + 2], dh
        mov    cl, byte ptr [ecx + 91h]
        mov    byte ptr [eax + 3], cl
        xor    ax, ax
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_AE84:
        push   ebp
        mov    ecx, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    byte ptr [ecx], 4
        mov    byte ptr [ecx + 1], 7
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dx, word ptr [eax + 93h]
        mov    byte ptr [ecx + 2], dh
        mov    al, byte ptr [eax + 93h]
        mov    byte ptr [ecx + 3], al
        xor    ax, ax
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_AEB4:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 0ch]
        pop    ebp
        mov    byte ptr [eax], 4
        mov    byte ptr [eax + 2], cl
        mov    byte ptr [eax + 3], cl
        mov    byte ptr [eax + 1], 21h
        xor    ax, ax
        ret    8
        db  08Dh, 049h, 000h
sub_1_AED4:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    cx, word ptr [esp + 10h]
        mov    ebp, esp
        mov    byte ptr [eax], 6
        mov    byte ptr [eax + 2], ch
        mov    byte ptr [eax + 1], 20h
        mov    byte ptr [eax + 3], cl
        mov    cx, word ptr [ebp + 14h]
        pop    ebp
        mov    byte ptr [eax + 4], ch
        mov    byte ptr [eax + 5], cl
        xor    ax, ax
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_AF04:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   1ch
        push   0
        push   esi
        call   sub_1_6C63
        mov    dword ptr [ebp - 4], eax
        cmp    eax, -1
        jne    SHORT loc_1_AF2E
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_AF2E:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0eh]
        mov    eax, dword ptr [ebp + 0ch]
        push   0
        mov    edi, dword ptr [ecx + 0ch]
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [edi + 11h], 0
        mov    al, byte ptr [eax + 10h]
        push   ecx
        shl    al, 4
        push   esi
        lea    ebx, [edi + 12h]
        mov    byte ptr [edi + 10h], al
        mov    word ptr [edi + 0eh], 4
        call   sub_1_AA94
        mov    cx, word ptr [ebp + 14h]
        push   ecx
        push   ebx
        push   esi
        call   sub_1_AB64
        mov    al, byte ptr [ebx]
        xor    ecx, ecx
        and    eax, 0ffff00ffh
        add    word ptr [edi + 0eh], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ebx]
        add    ebx, ecx
        mov    cl, byte ptr [eax + 11h]
        mov    dx, word ptr [ebp + 10h]
        and    ecx, 0ffff00ffh
        mov    al, byte ptr [eax + 10h]
        and    eax, 0ffff00ffh
        shl    ax, 8
        or     cx, ax
        push   ecx
        push   edx
        push   ebx
        push   esi
        call   sub_1_AED4
        mov    al, byte ptr [ebx]
        push   0
        and    eax, 0ffff00ffh
        add    ax, word ptr [edi + 0eh]
        mov    word ptr [edi + 0eh], ax
        mov    ch, al
        mov    cl, ah
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [edi + 0eh], cx
        push   eax
        push   esi
        call   sub_1_6F4B
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_AFD4:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   34h
        push   0
        push   esi
        call   sub_1_6C63
        mov    dword ptr [ebp - 4], eax
        cmp    eax, -1
        jne    SHORT loc_1_AFFE
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_AFFE:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0eh]
        mov    eax, dword ptr [ebp + 0ch]
        push   0
        mov    edi, dword ptr [ecx + 0ch]
        push   edi
        push   eax
        mov    byte ptr [edi + 11h], 22h
        mov    cl, byte ptr [eax + 10h]
        push   esi
        shl    cl, 4
        lea    ebx, [edi + 12h]
        mov    byte ptr [edi + 10h], cl
        mov    word ptr [edi + 0eh], 4
        call   sub_1_AA94
        mov    cx, word ptr [ebp + 10h]
        push   ecx
        push   ebx
        push   esi
        call   sub_1_AB64
        mov    al, byte ptr [ebx]
        xor    ecx, ecx
        and    eax, 0ffff00ffh
        add    word ptr [edi + 0eh], ax
        mov    cl, byte ptr [ebx]
        add    ebx, ecx
        push   ebx
        push   esi
        call   sub_1_ACF4
        mov    cl, byte ptr [ebx]
        xor    eax, eax
        and    ecx, 0ffff00ffh
        add    word ptr [edi + 0eh], cx
        mov    al, byte ptr [ebx]
        add    ebx, eax
        push   ebx
        push   esi
        call   sub_1_AC84
        mov    cl, byte ptr [ebx]
        xor    eax, eax
        and    ecx, 0ffff00ffh
        add    word ptr [edi + 0eh], cx
        mov    al, byte ptr [ebx]
        add    ebx, eax
        push   ebx
        push   esi
        call   sub_1_AEB4
        mov    cl, byte ptr [ebx]
        xor    eax, eax
        and    ecx, 0ffff00ffh
        add    word ptr [edi + 0eh], cx
        mov    al, byte ptr [ebx]
        add    ebx, eax
        push   ebx
        push   esi
        call   sub_1_AD94
        mov    cl, byte ptr [ebx]
        xor    eax, eax
        and    ecx, 0ffff00ffh
        add    word ptr [edi + 0eh], cx
        mov    al, byte ptr [ebx]
        add    ebx, eax
        push   ebx
        push   esi
        call   sub_1_AD44
        mov    al, byte ptr [ebx]
        push   0
        and    eax, 0ffff00ffh
        add    ax, word ptr [edi + 0eh]
        mov    word ptr [edi + 0eh], ax
        mov    ch, al
        mov    cl, ah
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [edi + 0eh], cx
        push   eax
        push   esi
        call   sub_1_6F4B
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_B0E4:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   32h
        push   0
        push   esi
        call   sub_1_6C63
        mov    dword ptr [ebp - 4], eax
        cmp    eax, -1
        jne    SHORT loc_1_B10E
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_B10E:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0eh]
        mov    eax, dword ptr [ebp + 0ch]
        push   0
        mov    edi, dword ptr [ecx + 0ch]
        push   edi
        push   eax
        mov    byte ptr [edi + 11h], 23h
        mov    cl, byte ptr [eax + 10h]
        push   esi
        shl    cl, 4
        lea    ebx, [edi + 12h]
        mov    byte ptr [edi + 10h], cl
        mov    word ptr [edi + 0eh], 4
        call   sub_1_AA94
        mov    cx, word ptr [ebp + 10h]
        push   ecx
        push   ebx
        push   esi
        call   sub_1_AB64
        mov    al, byte ptr [ebx]
        xor    ecx, ecx
        and    eax, 0ffff00ffh
        add    word ptr [edi + 0eh], ax
        mov    cl, byte ptr [ebx]
        add    ebx, ecx
        push   ebx
        push   esi
        call   sub_1_ABA4
        mov    cl, byte ptr [ebx]
        xor    eax, eax
        and    ecx, 0ffff00ffh
        add    word ptr [edi + 0eh], cx
        mov    al, byte ptr [ebx]
        add    ebx, eax
        push   ebx
        push   esi
        call   sub_1_AC04
        mov    cl, byte ptr [ebx]
        xor    eax, eax
        and    ecx, 0ffff00ffh
        add    word ptr [edi + 0eh], cx
        mov    al, byte ptr [ebx]
        add    ebx, eax
        push   ebx
        push   esi
        call   sub_1_ADE4
        mov    al, byte ptr [ebx]
        push   0
        and    eax, 0ffff00ffh
        add    ax, word ptr [edi + 0eh]
        mov    word ptr [edi + 0eh], ax
        mov    ch, al
        mov    cl, ah
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [edi + 0eh], cx
        push   eax
        push   esi
        call   sub_1_6F4B
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_B1C4:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   38h
        push   0
        push   esi
        call   sub_1_6C63
        mov    dword ptr [ebp - 4], eax
        cmp    eax, -1
        jne    SHORT loc_1_B1EE
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_B1EE:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0eh]
        mov    eax, dword ptr [ebp + 0ch]
        push   0
        mov    edi, dword ptr [ecx + 0ch]
        push   edi
        push   eax
        mov    byte ptr [edi + 11h], 24h
        mov    cl, byte ptr [eax + 10h]
        push   esi
        shl    cl, 4
        lea    ebx, [edi + 12h]
        mov    byte ptr [edi + 10h], cl
        mov    word ptr [edi + 0eh], 4
        call   sub_1_AA94
        mov    cx, word ptr [ebp + 10h]
        push   ecx
        push   ebx
        push   esi
        call   sub_1_AB64
        mov    al, byte ptr [ebx]
        xor    ecx, ecx
        and    eax, 0ffff00ffh
        add    word ptr [edi + 0eh], ax
        mov    cl, byte ptr [ebx]
        add    ebx, ecx
        push   ebx
        push   esi
        call   sub_1_AC34
        mov    cl, byte ptr [ebx]
        xor    eax, eax
        and    ecx, 0ffff00ffh
        add    word ptr [edi + 0eh], cx
        mov    al, byte ptr [ebx]
        add    ebx, eax
        push   ebx
        push   esi
        call   sub_1_AD44
        mov    cl, byte ptr [ebx]
        xor    eax, eax
        and    ecx, 0ffff00ffh
        add    word ptr [edi + 0eh], cx
        mov    al, byte ptr [ebx]
        add    ebx, eax
        push   ebx
        push   esi
        call   sub_1_AE54
        mov    cl, byte ptr [ebx]
        xor    eax, eax
        and    ecx, 0ffff00ffh
        add    word ptr [edi + 0eh], cx
        mov    al, byte ptr [ebx]
        add    ebx, eax
        push   ebx
        push   esi
        call   sub_1_AE84
        mov    al, byte ptr [ebx]
        and    eax, 0ffff00ffh
        add    ax, word ptr [edi + 0eh]
        mov    word ptr [edi + 0eh], ax
        mov    ch, al
        mov    cl, ah
        push   0
        mov    word ptr [edi + 0eh], cx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   esi
        call   sub_1_6F4B
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_B2C4:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 17ah]
        add    eax, 2
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 16eh]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_B303
        mov    word ptr [ebp - 8], 1
        jmp    NEAR PTR loc_1_B59A
loc_1_B303:
        push   12h
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6C63
        mov    dword ptr [ebp - 18h], eax
        push   12h
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6C63
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 0eh]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 60h
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 1], 1
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 24h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 8], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 25h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 9], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 26h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 0ah], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 27h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 0bh], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 28h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 0ch], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 29h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 0dh], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 24h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 25h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 26h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 27h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 5], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 28h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 6], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 29h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 7], al
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 11h], 7
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [eax + 0eh], 4
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffh
        shl    eax, 8
        mov    ecx, dword ptr [ebp - 10h]
        mov    cx, word ptr [ecx + 0eh]
        and    ecx, 0ffffh
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 0eh], ax
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 0eh]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 60h
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 1], 1
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 24h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 8], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 25h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 9], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 26h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 0ah], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 27h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 0bh], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 28h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 0ch], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 29h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 0dh], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 24h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 25h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 26h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 27h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 5], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 28h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 6], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 29h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 7], al
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 11h], 7
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [eax + 0eh], 4
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffh
        shl    eax, 8
        mov    ecx, dword ptr [ebp - 10h]
        mov    cx, word ptr [ecx + 0eh]
        and    ecx, 0ffffh
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 0eh], ax
        mov    eax, dword ptr [ebp - 14h]
        mov    word ptr [eax + 2], 8000h
        mov    eax, dword ptr [ebp - 18h]
        mov    word ptr [eax + 2], 2000h
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 16h]
        mov    word ptr [eax + 2], 2000h
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 186h], 1
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7034
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_B545
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_B89B
loc_1_B545:
        mov    word ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_B554
loc_1_B550:
        inc    word ptr [ebp - 0ch]
loc_1_B554:
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        cmp    eax, 0ffffh
        jge    NEAR PTR loc_1_B580
        mov    eax, dword ptr [ebp - 14h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   ah, 80h
        jne    NEAR PTR loc_1_B580
        jmp    NEAR PTR loc_1_B550
loc_1_B580:
        mov    eax, dword ptr [ebp - 14h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   ah, 80h
        jne    NEAR PTR loc_1_B59A
        mov    word ptr [ebp - 8], 1
loc_1_B59A:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_B893
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68FD
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_B5D1
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_B89B
loc_1_B5D1:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_B5F7
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_B89B
loc_1_B5F7:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d2h]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d2h]
        mov    word ptr [eax + 8], 85h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d2h]
        mov    word ptr [eax + 0ah], 0c503h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0d2h]
        mov    word ptr [ecx + 0ch], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9093
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_B665
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_B89B
loc_1_B665:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_B68B
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_B89B
loc_1_B68B:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        cmp    eax, 4
        jne    NEAR PTR loc_1_B6DC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        test   ah, ah
        jne    NEAR PTR loc_1_B6DC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        cmp    eax, 0fd00h
        je     NEAR PTR loc_1_B7F1
loc_1_B6DC:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_B7E8
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        test   ah, ah
        jne    NEAR PTR loc_1_B7E8
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        cmp    eax, 0fd00h
        jne    NEAR PTR loc_1_B7E8
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d2h]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d2h]
        mov    word ptr [eax + 8], 85h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d2h]
        mov    word ptr [eax + 0ah], 0f101h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0d2h]
        mov    word ptr [ecx + 0ch], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9093
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_B79A
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_B89B
loc_1_B79A:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_B7C0
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_B89B
loc_1_B7C0:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   ah, 40h
        jne    NEAR PTR loc_1_B7E3
        mov    ax, 8
        jmp    NEAR PTR loc_1_B89B
loc_1_B7E3:
        jmp    NEAR PTR loc_1_B7F1
loc_1_B7E8:
        mov    ax, 8
        jmp    NEAR PTR loc_1_B89B
loc_1_B7F1:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    word ptr [eax], 100h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d2h]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d2h]
        mov    word ptr [eax + 8], 84h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d2h]
        mov    word ptr [eax + 0ah], 0c601h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0d2h]
        mov    word ptr [ecx + 0ch], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9093
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_B86D
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_B89B
loc_1_B86D:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_B893
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_B89B
loc_1_B893:
        xor    ax, ax
        jmp    NEAR PTR loc_1_B89B
loc_1_B89B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_B8A2:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   12h
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6C63
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], -1
        jne    NEAR PTR loc_1_B8CE
        mov    ax, 6
        jmp    NEAR PTR loc_1_BA58
loc_1_B8CE:
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 0eh]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax], 60h
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 1], 1
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 24h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 8], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 25h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 9], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 26h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 0ah], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 27h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 0bh], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 28h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 0ch], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 29h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 0dh], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 24h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 25h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 26h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 27h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 5], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 28h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 6], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 29h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 7], al
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 11h], 7
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp - 0ch]
        mov    word ptr [eax + 0eh], 4
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffh
        shl    eax, 8
        mov    ecx, dword ptr [ebp - 0ch]
        mov    cx, word ptr [ecx + 0eh]
        and    ecx, 0ffffh
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0ch]
        mov    word ptr [ecx + 0eh], ax
        push   0
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6F4B
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_B9E2
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_BA58
loc_1_B9E2:
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_B9F1
loc_1_B9ED:
        inc    word ptr [ebp - 8]
loc_1_B9F1:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 0ffffh
        jge    NEAR PTR loc_1_BA1D
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   ah, 80h
        jne    NEAR PTR loc_1_BA1D
        jmp    NEAR PTR loc_1_B9ED
loc_1_BA1D:
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   ah, 80h
        je     NEAR PTR loc_1_BA47
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   eax, 1cch
        je     NEAR PTR loc_1_BA50
loc_1_BA47:
        mov    ax, 9
        jmp    NEAR PTR loc_1_BA58
loc_1_BA50:
        xor    ax, ax
        jmp    NEAR PTR loc_1_BA58
loc_1_BA58:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_BA5F:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   206h
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6C63
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 14h], -1
        jne    NEAR PTR loc_1_BA8E
        mov    ax, 6
        jmp    NEAR PTR loc_1_BC88
loc_1_BA8E:
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 0eh]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax], 60h
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 1], 0
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 2], 0
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 3], 0
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 4], 0
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 5], 0
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 6], 0
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 7], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 24h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 8], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 25h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 9], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 26h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 0ah], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 27h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 0bh], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 28h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 0ch], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 29h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 0dh], al
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 11h], 8
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [eax + 0eh], 4
        mov    eax, dword ptr [ebp - 10h]
        add    eax, 12h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_AB44
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 10h]
        mov    cx, word ptr [ecx + 0eh]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 0eh], ax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        add    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_AB44
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 10h]
        mov    cx, word ptr [ecx + 0eh]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 0eh], ax
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffh
        shl    eax, 8
        mov    ecx, dword ptr [ebp - 10h]
        mov    cx, word ptr [ecx + 0eh]
        and    ecx, 0ffffh
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 0eh], ax
        push   0
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6F4B
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_BBF7
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_BC88
loc_1_BBF7:
        mov    word ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_BC06
loc_1_BC02:
        inc    word ptr [ebp - 0ch]
loc_1_BC06:
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        cmp    eax, 0ffffh
        jge    NEAR PTR loc_1_BC32
        mov    eax, dword ptr [ebp - 14h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   ah, 80h
        jne    NEAR PTR loc_1_BC32
        jmp    NEAR PTR loc_1_BC02
loc_1_BC32:
        mov    eax, dword ptr [ebp - 14h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   ah, 80h
        je     NEAR PTR loc_1_BC5C
        mov    eax, dword ptr [ebp - 14h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   eax, 1cch
        je     NEAR PTR loc_1_BC65
loc_1_BC5C:
        mov    ax, 92h
        jmp    NEAR PTR loc_1_BC88
loc_1_BC65:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 182h], 0
        push   0
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9F62
        xor    ax, ax
        jmp    NEAR PTR loc_1_BC88
loc_1_BC88:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_BC8F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   3eh
        push   0
        push   esi
        call   sub_1_6C63
        mov    dword ptr [ebp - 4], eax
        cmp    eax, -1
        jne    SHORT loc_1_BCB9
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_BCB9:
        mov    eax, dword ptr [ebp - 4]
        push   6000h
        mov    ecx, dword ptr [eax + 0eh]
        mov    edi, dword ptr [ecx + 0ch]
        push   edi
        push   0
        push   esi
        lea    ebx, [edi + 12h]
        mov    byte ptr [edi + 11h], 20h
        mov    byte ptr [edi + 10h], 50h
        mov    word ptr [edi + 0eh], 4
        call   sub_1_AA94
        push   ebx
        push   esi
        call   sub_1_AC34
        mov    al, byte ptr [ebx]
        xor    ecx, ecx
        and    eax, 0ffff00ffh
        add    word ptr [edi + 0eh], ax
        mov    cl, byte ptr [ebx]
        add    ebx, ecx
        push   ebx
        push   esi
        call   sub_1_AC84
        mov    cl, byte ptr [ebx]
        xor    eax, eax
        and    ecx, 0ffff00ffh
        add    word ptr [edi + 0eh], cx
        mov    al, byte ptr [ebx]
        add    ebx, eax
        push   ebx
        push   esi
        call   sub_1_ABA4
        mov    cl, byte ptr [ebx]
        xor    eax, eax
        and    ecx, 0ffff00ffh
        add    word ptr [edi + 0eh], cx
        mov    al, byte ptr [ebx]
        add    ebx, eax
        push   ebx
        push   esi
        call   sub_1_AEB4
        mov    al, byte ptr [ebx]
        push   0
        and    eax, 0ffff00ffh
        add    ax, word ptr [edi + 0eh]
        mov    word ptr [edi + 0eh], ax
        mov    ch, al
        mov    cl, ah
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [edi + 0eh], cx
        push   eax
        push   esi
        call   sub_1_6F4B
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_BD64:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 14h], 1
        mov    word ptr [ebp - 8], 0
        mov    dword ptr [ebp - 10h], 0
        mov    eax, dword ptr [ebp + 0ch]
        movsx  eax, word ptr [eax + 0eh]
        sub    eax, 4
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 12h
        mov    dword ptr [ebp - 4], eax
loc_1_BD97:
        movsx  eax, word ptr [ebp - 0ch]
        test   eax, eax
        jle    NEAR PTR loc_1_BE40
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_BE40
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_1_BDF5
loc_1_BDC7:
        or     dword ptr [ebp - 10h], 40h
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A934
        mov    word ptr [ebp - 14h], ax
        jmp    NEAR PTR loc_1_BE04
loc_1_BDE5:
        mov    word ptr [ebp - 14h], 8008h
        jmp    NEAR PTR loc_1_BE04
        db  0E9h, 00Fh, 000h, 000h, 000h
loc_1_BDF5:
        cmp    dword ptr [ebp - 18h], 9
        je     NEAR PTR loc_1_BDC7
        jmp    NEAR PTR loc_1_BDE5
loc_1_BE04:
        movsx  eax, word ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    cl, byte ptr [ecx]
        and    ecx, 0ffh
        sub    eax, ecx
        mov    word ptr [ebp - 0ch], ax
        movsx  eax, word ptr [ebp - 0ch]
        test   eax, eax
        jge    NEAR PTR loc_1_BE2B
        mov    word ptr [ebp - 14h], 8002h
loc_1_BE2B:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_BD97
loc_1_BE40:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_BE73
        movsx  eax, word ptr [ebp - 0ch]
        test   eax, eax
        je     NEAR PTR loc_1_BE68
        mov    word ptr [ebp - 14h], 8002h
        jmp    NEAR PTR loc_1_BE73
loc_1_BE68:
        jmp    NEAR PTR loc_1_BE73
        db  066h, 0C7h, 045h, 0ECh, 007h, 080h
loc_1_BE73:
        mov    ax, word ptr [ebp - 14h]
        jmp    NEAR PTR loc_1_BE7C
loc_1_BE7C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_BE83:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 10h], 1
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 10h]
        and    eax, 0fh
        cmp    al, 4
        je     NEAR PTR loc_1_BEB3
        mov    ax, 8004h
        jmp    NEAR PTR loc_1_C056
loc_1_BEB3:
        mov    eax, dword ptr [ebp + 0ch]
        movsx  eax, word ptr [eax + 0eh]
        sub    eax, 4
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 12h
        mov    dword ptr [ebp - 4], eax
loc_1_BECA:
        movsx  eax, word ptr [ebp - 8]
        test   eax, eax
        jle    NEAR PTR loc_1_C01A
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_C01A
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_1_BFAA
loc_1_BEFA:
        or     dword ptr [ebp - 0ch], 40h
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A934
        mov    word ptr [ebp - 10h], ax
        jmp    NEAR PTR loc_1_BFDE
loc_1_BF18:
        or     dword ptr [ebp - 0ch], 2
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A844
        mov    word ptr [ebp - 10h], ax
        jmp    NEAR PTR loc_1_BFDE
loc_1_BF32:
        or     dword ptr [ebp - 0ch], 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A884
        mov    word ptr [ebp - 10h], ax
        jmp    NEAR PTR loc_1_BFDE
loc_1_BF4C:
        or     dword ptr [ebp - 0ch], 8
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A8C4
        mov    word ptr [ebp - 10h], ax
        jmp    NEAR PTR loc_1_BFDE
loc_1_BF66:
        or     dword ptr [ebp - 0ch], 10h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A974
        mov    word ptr [ebp - 10h], ax
        jmp    NEAR PTR loc_1_BFDE
loc_1_BF80:
        or     dword ptr [ebp - 0ch], 20h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A9B4
        mov    word ptr [ebp - 10h], ax
        jmp    NEAR PTR loc_1_BFDE
loc_1_BF9A:
        mov    word ptr [ebp - 10h], 8008h
        jmp    NEAR PTR loc_1_BFDE
        db  0E9h, 034h, 000h, 000h, 000h
loc_1_BFAA:
        sub    dword ptr [ebp - 14h], 3
        cmp    dword ptr [ebp - 14h], 6
        ja     NEAR PTR loc_1_BF9A
        mov    eax, dword ptr [ebp - 14h]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_BFDE:
        movsx  eax, word ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    cl, byte ptr [ecx]
        and    ecx, 0ffh
        sub    eax, ecx
        mov    word ptr [ebp - 8], ax
        movsx  eax, word ptr [ebp - 8]
        test   eax, eax
        jge    NEAR PTR loc_1_C005
        mov    word ptr [ebp - 10h], 8002h
loc_1_C005:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_BECA
loc_1_C01A:
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_C04D
        movsx  eax, word ptr [ebp - 8]
        test   eax, eax
        je     NEAR PTR loc_1_C042
        mov    word ptr [ebp - 10h], 8002h
        jmp    NEAR PTR loc_1_C04D
loc_1_C042:
        jmp    NEAR PTR loc_1_C04D
        db  066h, 0C7h, 045h, 0F0h, 007h, 080h
loc_1_C04D:
        mov    ax, word ptr [ebp - 10h]
        jmp    NEAR PTR loc_1_C056
loc_1_C056:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_C05D:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 10h], 1
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 10h]
        and    eax, 0fh
        cmp    al, 5
        je     NEAR PTR loc_1_C08D
        mov    ax, 8004h
        jmp    NEAR PTR loc_1_C1FC
loc_1_C08D:
        mov    eax, dword ptr [ebp + 0ch]
        movsx  eax, word ptr [eax + 0eh]
        sub    eax, 4
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 12h
        mov    dword ptr [ebp - 4], eax
loc_1_C0A4:
        movsx  eax, word ptr [ebp - 8]
        test   eax, eax
        jle    NEAR PTR loc_1_C1C0
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_C1C0
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_1_C150
loc_1_C0D4:
        or     dword ptr [ebp - 0ch], 40h
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A934
        mov    word ptr [ebp - 10h], ax
        jmp    NEAR PTR loc_1_C184
loc_1_C0F2:
        or     dword ptr [ebp - 0ch], 2
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A844
        mov    word ptr [ebp - 10h], ax
        jmp    NEAR PTR loc_1_C184
loc_1_C10C:
        or     dword ptr [ebp - 0ch], 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A884
        mov    word ptr [ebp - 10h], ax
        jmp    NEAR PTR loc_1_C184
loc_1_C126:
        or     dword ptr [ebp - 0ch], 8
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A8C4
        mov    word ptr [ebp - 10h], ax
        jmp    NEAR PTR loc_1_C184
loc_1_C140:
        mov    word ptr [ebp - 10h], 8008h
        jmp    NEAR PTR loc_1_C184
        db  0E9h, 034h, 000h, 000h, 000h
loc_1_C150:
        sub    dword ptr [ebp - 14h], 3
        cmp    dword ptr [ebp - 14h], 6
        ja     NEAR PTR loc_1_C140
        mov    eax, dword ptr [ebp - 14h]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_C184:
        movsx  eax, word ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    cl, byte ptr [ecx]
        and    ecx, 0ffh
        sub    eax, ecx
        mov    word ptr [ebp - 8], ax
        movsx  eax, word ptr [ebp - 8]
        test   eax, eax
        jge    NEAR PTR loc_1_C1AB
        mov    word ptr [ebp - 10h], 8002h
loc_1_C1AB:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_C0A4
loc_1_C1C0:
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_C1F3
        movsx  eax, word ptr [ebp - 8]
        test   eax, eax
        je     NEAR PTR loc_1_C1E8
        mov    word ptr [ebp - 10h], 8002h
        jmp    NEAR PTR loc_1_C1F3
loc_1_C1E8:
        jmp    NEAR PTR loc_1_C1F3
        db  066h, 0C7h, 045h, 0F0h, 007h, 080h
loc_1_C1F3:
        mov    ax, word ptr [ebp - 10h]
        jmp    NEAR PTR loc_1_C1FC
loc_1_C1FC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_C203:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 10h], 1
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 10h]
        and    eax, 0fh
        cmp    al, 4
        je     NEAR PTR loc_1_C233
        mov    ax, 8004h
        jmp    NEAR PTR loc_1_C33A
loc_1_C233:
        mov    eax, dword ptr [ebp + 0ch]
        movsx  eax, word ptr [eax + 0eh]
        sub    eax, 4
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 12h
        mov    dword ptr [ebp - 4], eax
loc_1_C24A:
        movsx  eax, word ptr [ebp - 8]
        test   eax, eax
        jle    NEAR PTR loc_1_C2F9
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_C2F9
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_1_C2AE
loc_1_C27A:
        or     dword ptr [ebp - 0ch], 8000h
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A7E4
        mov    word ptr [ebp - 10h], ax
        jmp    NEAR PTR loc_1_C2BD
loc_1_C29E:
        mov    word ptr [ebp - 10h], 8008h
        jmp    NEAR PTR loc_1_C2BD
        db  0E9h, 00Fh, 000h, 000h, 000h
loc_1_C2AE:
        cmp    dword ptr [ebp - 14h], 27h
        je     NEAR PTR loc_1_C27A
        jmp    NEAR PTR loc_1_C29E
loc_1_C2BD:
        movsx  eax, word ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    cl, byte ptr [ecx]
        and    ecx, 0ffh
        sub    eax, ecx
        mov    word ptr [ebp - 8], ax
        movsx  eax, word ptr [ebp - 8]
        test   eax, eax
        jge    NEAR PTR loc_1_C2E4
        mov    word ptr [ebp - 10h], 8002h
loc_1_C2E4:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_C24A
loc_1_C2F9:
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_C331
        movsx  eax, word ptr [ebp - 8]
        test   eax, eax
        je     NEAR PTR loc_1_C321
        mov    word ptr [ebp - 10h], 8002h
        jmp    NEAR PTR loc_1_C331
loc_1_C321:
        test   byte ptr [ebp - 0bh], 80h
        jne    NEAR PTR loc_1_C331
        mov    word ptr [ebp - 10h], 8007h
loc_1_C331:
        mov    ax, word ptr [ebp - 10h]
        jmp    NEAR PTR loc_1_C33A
loc_1_C33A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_C341:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        cmp    eax, 12h
        jg     NEAR PTR loc_1_C367
        xor    ax, ax
        jmp    NEAR PTR loc_1_C4EC
loc_1_C367:
        push   0
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6C63
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], -1
        jne    NEAR PTR loc_1_C389
        xor    ax, ax
        jmp    NEAR PTR loc_1_C4EC
loc_1_C389:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ecx + 0eh]
        mov    dword ptr [ecx + 6], eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 14h
        mov    ecx, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ecx + 0eh]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        sub    eax, 6
        mov    ecx, dword ptr [ebp - 0ch]
        mov    word ptr [ecx + 0ch], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        sub    eax, 6
        mov    ecx, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ecx + 0eh]
        mov    word ptr [ecx + 0ah], ax
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6F4B
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_C408
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_C4EC
loc_1_C408:
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_C417
loc_1_C413:
        inc    word ptr [ebp - 8]
loc_1_C417:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 0ffffh
        jge    NEAR PTR loc_1_C443
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   ah, 80h
        jne    NEAR PTR loc_1_C443
        jmp    NEAR PTR loc_1_C413
loc_1_C443:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   ah, 80h
        jne    NEAR PTR loc_1_C4D7
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7034
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_C47F
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_C4EC
loc_1_C47F:
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_C48E
loc_1_C48A:
        inc    word ptr [ebp - 8]
loc_1_C48E:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 0ffffh
        jge    NEAR PTR loc_1_C4BA
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   ah, 80h
        jne    NEAR PTR loc_1_C4BA
        jmp    NEAR PTR loc_1_C48A
loc_1_C4BA:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   ah, 80h
        jne    NEAR PTR loc_1_C4D7
        mov    ax, 8
        jmp    NEAR PTR loc_1_C4EC
loc_1_C4D7:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp + 10h]
        mov    word ptr [ecx], ax
        mov    ax, 8888h
        jmp    NEAR PTR loc_1_C4EC
loc_1_C4EC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_C4F3:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   16h
        push   0
        push   eax
        call   sub_1_6C63
        cmp    eax, -1
        mov    esi, eax
        jne    SHORT loc_1_C518
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_1_C518:
        mov    eax, dword ptr [esi + 0eh]
        mov    edi, dword ptr [eax + 0ch]
        mov    eax, dword ptr [ebp + 0ch]
        push   0
        push   edi
        mov    byte ptr [edi + 11h], 2ah
        mov    cl, byte ptr [eax + 10h]
        push   eax
        shl    cl, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    ebx, [edi + 12h]
        mov    byte ptr [edi + 10h], cl
        mov    word ptr [edi + 0eh], 4
        call   sub_1_AA94
        mov    cx, word ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 8]
        push   ecx
        push   ebx
        push   eax
        call   sub_1_AB84
        mov    al, byte ptr [ebx]
        push   0
        and    eax, 0ffff00ffh
        push   esi
        add    ax, word ptr [edi + 0eh]
        mov    word ptr [edi + 0eh], ax
        mov    ch, al
        mov    cl, ah
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [edi + 0eh], cx
        push   eax
        call   sub_1_6F4B
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_C584:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp + 0ch]
        movsx  eax, word ptr [eax + 0eh]
        sub    eax, 4
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 12h
        mov    dword ptr [ebp - 4], eax
loc_1_C5AC:
        movsx  eax, word ptr [ebp - 8]
        test   eax, eax
        jle    NEAR PTR loc_1_C637
        mov    eax, dword ptr [ebp + 10h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_C637
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_C5FE
loc_1_C5DE:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A934
        jmp    NEAR PTR loc_1_C60D
        db  0E9h, 014h, 000h, 000h, 000h, 0E9h, 00Fh, 000h, 000h, 000h
loc_1_C5FE:
        cmp    dword ptr [ebp - 0ch], 9
        je     NEAR PTR loc_1_C5DE
        jmp    NEAR PTR loc_1_C60D
loc_1_C60D:
        movsx  eax, word ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    cl, byte ptr [ecx]
        and    ecx, 0ffh
        sub    eax, ecx
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_C5AC
loc_1_C637:
        mov    ax, 8003h
        jmp    NEAR PTR loc_1_C640
loc_1_C640:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_C647:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 8], 0
        mov    word ptr [ebp - 0ch], 0
        mov    byte ptr [ebp - 10h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffh
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0eh]
        and    ecx, 0ffffh
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0eh], ax
        mov    eax, dword ptr [ebp + 14h]
        and    eax, 0ffffh
        test   ah, 4
        je     NEAR PTR loc_1_CAE3
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 11h]
        and    eax, 0ffh
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_1_CA6D
loc_1_C6AC:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_C05D
        mov    word ptr [ebp - 14h], ax
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, 8
        jne    NEAR PTR loc_1_C6DB
        mov    ax, word ptr [ebp - 14h]
        jmp    NEAR PTR loc_1_CBF1
loc_1_C6DB:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_AF04
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_C70D
        mov    ax, word ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_CBF1
loc_1_C70D:
        jmp    NEAR PTR loc_1_CADE
loc_1_C712:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_BE83
        mov    word ptr [ebp - 14h], ax
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, 8
        jne    NEAR PTR loc_1_C741
        mov    ax, word ptr [ebp - 14h]
        jmp    NEAR PTR loc_1_CBF1
loc_1_C741:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_AF04
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_C773
        mov    ax, word ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_CBF1
loc_1_C773:
        jmp    NEAR PTR loc_1_CADE
loc_1_C778:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_BD64
        mov    word ptr [ebp - 14h], ax
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, 1
        je     NEAR PTR loc_1_C7D7
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, 8
        jne    NEAR PTR loc_1_C7BD
        mov    ax, word ptr [ebp - 14h]
        jmp    NEAR PTR loc_1_CBF1
        db  0E9h, 01Ah, 000h, 000h, 000h
loc_1_C7BD:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_AF04
        jmp    NEAR PTR loc_1_CBF1
loc_1_C7D7:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_AFD4
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_C805
        mov    ax, word ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_CBF1
loc_1_C805:
        jmp    NEAR PTR loc_1_CADE
loc_1_C80A:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_BD64
        mov    word ptr [ebp - 14h], ax
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, 1
        je     NEAR PTR loc_1_C869
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, 8
        jne    NEAR PTR loc_1_C84F
        mov    ax, word ptr [ebp - 14h]
        jmp    NEAR PTR loc_1_CBF1
        db  0E9h, 01Ah, 000h, 000h, 000h
loc_1_C84F:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_AF04
        jmp    NEAR PTR loc_1_CBF1
loc_1_C869:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_B1C4
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_C897
        mov    ax, word ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_CBF1
loc_1_C897:
        jmp    NEAR PTR loc_1_CADE
loc_1_C89C:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_BD64
        mov    word ptr [ebp - 14h], ax
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, 1
        je     NEAR PTR loc_1_C8FB
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, 8
        jne    NEAR PTR loc_1_C8E1
        mov    ax, word ptr [ebp - 14h]
        jmp    NEAR PTR loc_1_CBF1
        db  0E9h, 01Ah, 000h, 000h, 000h
loc_1_C8E1:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_AF04
        jmp    NEAR PTR loc_1_CBF1
loc_1_C8FB:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_B0E4
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_C929
        mov    ax, word ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_CBF1
loc_1_C929:
        jmp    NEAR PTR loc_1_CADE
loc_1_C92E:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_C203
        mov    word ptr [ebp - 14h], ax
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, 1
        je     NEAR PTR loc_1_C989
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, 8
        jne    NEAR PTR loc_1_C96F
        mov    ax, word ptr [ebp - 14h]
        jmp    NEAR PTR loc_1_CBF1
        db  0E9h, 01Ah, 000h, 000h, 000h
loc_1_C96F:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_AF04
        jmp    NEAR PTR loc_1_CBF1
loc_1_C989:
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_C341
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        cmp    eax, 8
        jne    NEAR PTR loc_1_C9B8
        mov    ax, word ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_CBF1
loc_1_C9B8:
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        cmp    eax, 8888h
        jne    NEAR PTR loc_1_C9FA
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_C4F3
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        cmp    eax, 8
        jne    NEAR PTR loc_1_C9FA
        mov    ax, word ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_CBF1
loc_1_C9FA:
        jmp    NEAR PTR loc_1_CADE
loc_1_C9FF:
        jmp    NEAR PTR loc_1_CADE
loc_1_CA04:
        mov    byte ptr [ebp - 10h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    ax, word ptr [eax + 3eh]
        and    eax, 0ffffh
        test   al, 20h
        jne    NEAR PTR loc_1_CA63
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_C584
        mov    word ptr [ebp - 14h], ax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_AF04
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_CA63
        mov    ax, word ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_CBF1
loc_1_CA63:
        jmp    NEAR PTR loc_1_CADE
        db  0E9h, 071h, 000h, 000h, 000h
loc_1_CA6D:
        cmp    dword ptr [ebp - 18h], 29h
        ja     NEAR PTR loc_1_CA04
        mov    eax, dword ptr [ebp - 18h]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 009h, 009h, 009h, 009h, 009h, 009h, 009h
        db  009h, 001h, 009h, 009h, 002h, 003h, 004h, 005h, 006h, 009h, 009h, 009h, 009h, 009h, 009h, 009h
        db  009h, 009h, 009h, 009h, 009h, 009h, 009h, 009h, 007h, 009h, 008h, 008h, 008h, 008h, 008h, 008h
        db  008h, 008h
loc_1_CADE:
        jmp    NEAR PTR loc_1_CB66
loc_1_CAE3:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 11h]
        and    eax, 0ffh
        mov    dword ptr [ebp - 1ch], eax
        jmp    NEAR PTR loc_1_CB09
loc_1_CAF6:
        jmp    NEAR PTR loc_1_CB66
loc_1_CAFB:
        mov    byte ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_1_CB66
        db  0E9h, 05Dh, 000h, 000h, 000h
loc_1_CB09:
        cmp    dword ptr [ebp - 1ch], 29h
        ja     NEAR PTR loc_1_CAFB
        mov    eax, dword ptr [ebp - 1ch]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  001h, 001h, 001h, 001h, 001h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 002h, 004h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h
loc_1_CB66:
        mov    eax, dword ptr [ebp + 10h]
        mov    ax, word ptr [eax + 3eh]
        and    eax, 0ffffh
        test   al, 20h
        je     NEAR PTR loc_1_CB87
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        test   eax, eax
        je     NEAR PTR loc_1_CBAA
loc_1_CB87:
        mov    eax, dword ptr [ebp + 10h]
        mov    ax, word ptr [eax + 3eh]
        and    eax, 0ffffh
        test   ah, 1
        je     NEAR PTR loc_1_CBE8
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        cmp    eax, 1
        jne    NEAR PTR loc_1_CBE8
loc_1_CBAA:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffh
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0eh]
        and    ecx, 0ffffh
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0eh], ax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        push   eax
        call   sub_1_CDF8
        mov    word ptr [ebp - 0ch], ax
loc_1_CBE8:
        mov    ax, word ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_CBF1
loc_1_CBF1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
sub_1_CBF8:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    bx, word ptr [ebp + 8]
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    dword ptr [esi + 282h], 0
        jne    SHORT loc_1_CC18
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_CC18:
        mov    edi, dword ptr [esi + 27ah]
        test   edi, edi
        je     NEAR PTR loc_1_CCD8
        cmp    byte ptr [esi + 304h], 0
        jne    NEAR PTR loc_1_CCD8
        cmp    byte ptr [esi + 305h], 0
        jne    NEAR PTR loc_1_CCD8
        mov    eax, dword ptr [edi + 24h]
        mov    dword ptr [esi + 27ah], eax
        test   eax, eax
        jne    SHORT loc_1_CC57
        mov    dword ptr [esi + 27eh], 0
loc_1_CC57:
        push   esi
        push   edi
        call   sub_1_3760
loc_1_CC5E:
        cmp    ax, 6
        je     SHORT loc_1_CC88
        test   ax, ax
        je     SHORT loc_1_CCAB
        cmp    ax, 8
        jne    SHORT loc_1_CCD8
        mov    byte ptr [esi + 303h], 1
        push   0
        push   0
        push   esi
        push   0
        call   sub_1_29F0
        mov    ax, 6
        jmp    SHORT loc_1_CC5E
loc_1_CC88:
        mov    eax, dword ptr [esi + 27ah]
        test   eax, eax
        je     SHORT loc_1_CC9D
        mov    dword ptr [edi + 24h], eax
        mov    dword ptr [esi + 27ah], edi
        jmp    SHORT loc_1_CCD8
loc_1_CC9D:
        mov    dword ptr [esi + 27ah], edi
        mov    dword ptr [esi + 27eh], edi
        jmp    SHORT loc_1_CCD8
loc_1_CCAB:
        cmp    dword ptr [esi + 282h], 0
        jne    SHORT loc_1_CCC2
        mov    dword ptr [esi + 286h], edi
        mov    dword ptr [esi + 282h], edi
        jmp    SHORT loc_1_CCD1
loc_1_CCC2:
        mov    eax, dword ptr [esi + 286h]
        mov    dword ptr [eax + 24h], edi
        mov    dword ptr [esi + 286h], edi
loc_1_CCD1:
        mov    dword ptr [edi + 24h], 0
loc_1_CCD8:
        mov    ecx, dword ptr [esi + 282h]
        mov    eax, dword ptr [ecx + 24h]
        mov    dword ptr [esi + 282h], eax
        test   eax, eax
        jne    SHORT loc_1_CCF5
        mov    dword ptr [esi + 286h], 0
loc_1_CCF5:
        mov    edi, dword ptr [ecx + 28h]
        mov    eax, ebx
        and    eax, 0ffffh
        test   eax, eax
        je     SHORT loc_1_CD1D
        cmp    eax, 10h
        je     SHORT loc_1_CD21
        cmp    eax, 11h
        je     SHORT loc_1_CD28
        mov    eax, 0c0000001h
        mov    dword ptr [ecx + 2eh], eax
        inc    dword ptr [esi + 2abh]
        jmp    SHORT loc_1_CD36
loc_1_CD1D:
        xor    eax, eax
        jmp    SHORT loc_1_CD2D
loc_1_CD21:
        mov    eax, 10001h
        jmp    SHORT loc_1_CD2D
loc_1_CD28:
        mov    eax, 10002h
loc_1_CD2D:
        mov    dword ptr [ecx + 2eh], eax
        inc    dword ptr [esi + 2a3h]
loc_1_CD36:
        cmp    byte ptr [ecx + 2ch], 0
        je     SHORT loc_1_CD73
        cmp    dword ptr [esi + 28ah], 0
        jne    SHORT loc_1_CD53
        mov    dword ptr [esi + 28eh], ecx
        mov    dword ptr [esi + 28ah], ecx
        jmp    SHORT loc_1_CD62
loc_1_CD53:
        mov    eax, dword ptr [esi + 28eh]
        mov    dword ptr [eax + 24h], ecx
        mov    dword ptr [esi + 28eh], ecx
loc_1_CD62:
        xor    ax, ax
        pop    edi
        mov    dword ptr [ecx + 24h], 0
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_CD73:
        push   eax
        push   ecx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_D72A
        mov    esi, dword ptr [edi + 8]
        dec    dword ptr [edi + 1ch]
        jne    SHORT loc_1_CDDF
        mov    eax, dword ptr [esi + 26dh]
        cmp    eax, edi
        jne    SHORT loc_1_CD9A
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [esi + 26dh], eax
        jmp    SHORT loc_1_CDB3
loc_1_CD9A:
        cmp    dword ptr [eax + 0ch], edi
        je     SHORT loc_1_CDA7
loc_1_CD9F:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], edi
        jne    SHORT loc_1_CD9F
loc_1_CDA7:
        mov    ecx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 0ch], ecx
        mov    dword ptr [esi + 452h], edi
loc_1_CDB3:
        mov    eax, dword ptr [edi + 18h]
        cmp    dword ptr [esi + 317h], eax
        jne    SHORT loc_1_CDC4
        push   esi
        call   sub_1_0BC0
loc_1_CDC4:
        cmp    dword ptr [esi + 26dh], 0
        jne    SHORT loc_1_CDDF
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        add    esi, 20ch
        push   esi
        call   sub_1_D7DA
loc_1_CDDF:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_CDF8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [edi + 296h], 1
        mov    dword ptr [edi + 292h], 0
        mov    ebx, dword ptr [edi + 317h]
        add    ebx, 0eh
        cmp    ebx, esi
        jb     SHORT loc_1_CE24
        mov    ebx, esi
loc_1_CE24:
        push   edi
        lea    eax, [edi + 32bh]
        push   eax
        push   0
        push   ebx
        call   sub_1_3470
        test   ax, ax
        je     SHORT loc_1_CE43
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_CE43:
        test   byte ptr [edi + 333h], 80h
        je     SHORT loc_1_CE5A
        mov    al, byte ptr [edi + 339h]
        and    eax, 1fh
        add    eax, 0eh
        jmp    SHORT loc_1_CE5F
loc_1_CE5A:
        mov    eax, 0eh
loc_1_CE5F:
        mov    dword ptr [edi + 29fh], eax
        inc    dword ptr [edi + 2a7h]
        cmp    esi, 0eh
        jae    SHORT loc_1_CE9A
        cmp    esi, 6
        jb     SHORT loc_1_CEBF
        push   0
        lea    eax, [edi + 32bh]
        push   0
        mov    ecx, dword ptr [edi + 25dh]
        push   0
        push   esi
        push   eax
        push   edi
        push   ecx
        call   sub_1_D260
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_CE9A:
        sub    esi, eax
        sub    ebx, eax
        push   esi
        lea    ecx, [edi + eax]
        push   ebx
        add    ecx, 32bh
        push   ecx
        mov    edx, dword ptr [edi + 25dh]
        push   eax
        lea    eax, [edi + 32bh]
        push   eax
        push   edi
        push   edx
        call   sub_1_D260
loc_1_CEBF:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_CED8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 303h], 0
        mov    edi, dword ptr [esi + 26dh]
        je     SHORT loc_1_CF14
        cmp    byte ptr [esi + 305h], 1
        jne    SHORT loc_1_CF14
        add    esi, 45eh
        push   0
        push   esi
        call   sub_1_D7C2
        xor    ax, ax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_CF14:
        mov    cl, byte ptr [esi + 305h]
        cmp    cl, 1
        je     NEAR PTR loc_1_CFCC
        cmp    byte ptr [esi + 271h], 0
        jne    NEAR PTR loc_1_CFCC
        mov    ax, word ptr [esi + 40h]
        cmp    ax, 2
        jne    SHORT loc_1_CF45
        xor    ax, ax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_CF45:
        cmp    ax, 3
        jne    SHORT loc_1_CFC1
        test   edi, edi
        je     SHORT loc_1_CF73
        push   0
        mov    eax, dword ptr [edi]
        push   0
        push   40010007h
        push   eax
        call   sub_1_D74E
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_D754
        xor    ax, ax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_CF73:
        cmp    byte ptr [esi + 451h], 1
        jne    SHORT loc_1_CFC1
        lea    eax, [esi + 45eh]
        lea    ecx, [ebp - 1]
        mov    byte ptr [esi + 451h], 0
        push   ecx
        push   eax
        call   sub_1_D7C8
        push   0
        mov    ecx, dword ptr [esi + 452h]
        mov    edx, dword ptr [ecx]
        push   edx
        call   sub_1_D724
        mov    ecx, dword ptr [esi + 452h]
        mov    edx, dword ptr [ecx]
        push   edx
        call   sub_1_D754
        push   0
        mov    ecx, dword ptr [esi + 452h]
        push   75h
        push   ecx
        call   sub_1_D7EC
loc_1_CFC1:
        xor    ax, ax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_CFCC:
        mov    ax, word ptr [esi + 40h]
        cmp    ax, 2
        jne    SHORT loc_1_D027
        push   esi
        call   sub_1_2F78
        test   ax, ax
        je     SHORT loc_1_D001
        add    esi, 45eh
        push   0
        push   esi
        mov    byte ptr [esi - 15bh], 1
        call   sub_1_D7C2
        xor    ax, ax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_D001:
        add    esi, 45eh
        lea    eax, [ebp - 1]
        push   eax
        push   esi
        call   sub_1_D7C8
        push   7530h
        push   esi
        call   sub_1_D7C2
        xor    ax, ax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_D027:
        cmp    ax, 1
        jne    NEAR PTR loc_1_D0BE
        test   cl, cl
        je     SHORT loc_1_D08F
        cmp    byte ptr [esi + 306h], 1
        jne    SHORT loc_1_D076
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 45eh]
        mov    byte ptr [esi + 306h], 0
        mov    byte ptr [esi + 305h], 0
        push   eax
        push   ecx
        call   sub_1_D7C8
        cmp    byte ptr [esi + 271h], 1
        jne    SHORT loc_1_D0BE
        push   esi
        call   sub_1_0B30
        xor    ax, ax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_D076:
        push   0
        add    esi, 45eh
        push   esi
        call   sub_1_D7C2
        xor    ax, ax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_D08F:
        cmp    ax, 1
        jne    SHORT loc_1_D0BE
        cmp    byte ptr [esi + 271h], 0
        je     SHORT loc_1_D0BE
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 45eh]
        mov    dword ptr [esi + 307h], 0
        push   eax
        push   ecx
        call   sub_1_D7C8
        push   esi
        call   sub_1_0B30
loc_1_D0BE:
        xor    ax, ax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_D0D8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        test   byte ptr [eax + 8eh], 80h
        push   esi
        mov    esi, dword ptr [eax + 26dh]
        mov    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_D103
        xor    ax, ax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_D103:
        mov    cx, word ptr [eax + 8fh]
        test   cl, 20h
        je     SHORT loc_1_D116
        mov    dword ptr [ebp - 4], 40h
loc_1_D116:
        test   cl, 40h
        je     SHORT loc_1_D122
        or     dword ptr [ebp - 4], 80h
loc_1_D122:
        test   cl, 80h
        je     SHORT loc_1_D12E
        or     dword ptr [ebp - 4], 100h
loc_1_D12E:
        test   ch, 1
        je     SHORT loc_1_D13A
        or     dword ptr [ebp - 4], 200h
loc_1_D13A:
        test   ch, 4
        je     SHORT loc_1_D146
        or     dword ptr [ebp - 4], 400h
loc_1_D146:
        test   ch, 8
        je     SHORT loc_1_D152
        or     dword ptr [ebp - 4], 800h
loc_1_D152:
        test   ch, 10h
        je     SHORT loc_1_D15E
        or     dword ptr [ebp - 4], 1000h
loc_1_D15E:
        test   ch, 20h
        je     SHORT loc_1_D16A
        or     dword ptr [ebp - 4], 2000h
loc_1_D16A:
        test   ch, 40h
        je     SHORT loc_1_D176
        or     dword ptr [ebp - 4], 4000h
loc_1_D176:
        test   ch, 80h
        je     SHORT loc_1_D182
        or     dword ptr [ebp - 4], 8000h
loc_1_D182:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_D1A9
        push   4
        lea    eax, [ebp - 4]
        push   eax
        inc    dword ptr [esi + 1ch]
        push   40010006h
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_1_D74E
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_1_D754
        dec    dword ptr [esi + 1ch]
loc_1_D1A9:
        xor    ax, ax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_D1B8:
        db  0A0h, 078h, 0D9h, 000h, 000h
        cmp    al, 0
        jne    SHORT loc_1_D1DD
        db  0FEh, 005h, 078h, 0D9h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     SHORT loc_1_D1DF
        push   0
        push   0
        call   0
        cmp    eax, 0
        jne    SHORT loc_1_D1DF
loc_1_D1DD:
        clc
        ret
loc_1_D1DF:
        stc
        ret
SMC8100W_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_D1F0
        db  0FFh, 024h, 085h, 0F2h, 0D1h, 000h, 000h
loc_1_D1F0:
        clc
        ret
        db  0B8h, 0D1h, 000h, 000h, 0F0h, 0D1h, 000h, 000h, 0F0h, 0D1h, 000h, 000h, 0F0h, 0D1h, 000h, 000h
        db  0F0h, 0D1h, 000h, 000h, 0F0h, 0D1h, 000h, 000h, 0F0h, 0D1h, 000h, 000h, 0F0h, 0D1h, 000h, 000h
        db  0F0h, 0D1h, 000h, 000h, 0F0h, 0D1h, 000h, 000h, 0F0h, 0D1h, 000h, 000h, 0F0h, 0D1h, 000h, 000h
        db  0F0h, 0D1h, 000h, 000h, 0F0h, 0D1h, 000h, 000h, 0F0h, 0D1h, 000h, 000h, 0F0h, 0D1h, 000h, 000h
        db  0F0h, 0D1h, 000h, 000h, 0F0h, 0D1h, 000h, 000h, 0F0h, 0D1h, 000h, 000h, 0F0h, 0D1h, 000h, 000h
        db  0F0h, 0D1h, 000h, 000h, 0F0h, 0D1h, 000h, 000h, 0F0h, 0D1h, 000h, 000h, 0F0h, 0D1h, 000h, 000h
        db  0F0h, 0D1h, 000h, 000h, 0F0h, 0D1h, 000h, 000h, 0B8h, 0D1h, 000h, 000h, 000h, 000h
sub_1_D260:
        push   ebp
        mov    ebp, esp
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
        call   sub_1_D290
        pop    ebp
        ret    1ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_D290:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 10h]
        add    ecx, 2
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 8
        cmp    dword ptr [ebp + 14h], 0eh
        jb     NEAR PTR loc_1_D3B0
        mov    al, byte ptr [eax]
        shr    al, 7
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ecx]
        and    al, 80h
        mov    byte ptr [ebp - 2], al
        or     al, al
        je     NEAR PTR loc_1_D354
        mov    al, byte ptr [ecx]
        cmp    al, 0ffh
        jne    SHORT loc_1_D2D2
        cmp    byte ptr [ecx + 1], 0ffh
        je     SHORT loc_1_D2DC
loc_1_D2D2:
        cmp    al, 0c0h
        jne    SHORT loc_1_D2F8
        cmp    byte ptr [ecx + 1], 0
        jne    SHORT loc_1_D2F8
loc_1_D2DC:
        cmp    byte ptr [ecx + 2], 0ffh
        jne    SHORT loc_1_D2F8
        cmp    byte ptr [ecx + 3], 0ffh
        jne    SHORT loc_1_D2F8
        cmp    byte ptr [ecx + 4], 0ffh
        jne    SHORT loc_1_D2F8
        cmp    byte ptr [ecx + 5], 0ffh
        mov    byte ptr [ebp - 2], 1
        je     SHORT loc_1_D2FC
loc_1_D2F8:
        mov    byte ptr [ebp - 2], 0
loc_1_D2FC:
        cmp    byte ptr [ebp - 2], 0
        je     SHORT loc_1_D310
        mov    dword ptr [ebp - 8], 8
        jmp    NEAR PTR loc_1_D3BB
        db  08Bh, 0FFh
loc_1_D310:
        mov    al, byte ptr [ecx]
        and    al, byte ptr [ecx + 2]
        and    al, 80h
        mov    byte ptr [ebp - 2], al
        or     al, al
        mov    dword ptr [ebp - 8], 1000h
        jne    SHORT loc_1_D32C
        mov    dword ptr [ebp - 8], 4000h
loc_1_D32C:
        xor    edx, edx
        mov    dl, byte ptr [ecx + 4]
        xor    eax, eax
        mov    al, byte ptr [ecx + 2]
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        xor    eax, eax
        mov    al, byte ptr [ecx + 3]
        shl    eax, 10h
        or     edx, eax
        xor    eax, eax
        mov    al, byte ptr [ecx + 5]
        or     edx, eax
        mov    dword ptr [ebp - 0ch], edx
        jmp    SHORT loc_1_D3BB
loc_1_D354:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 10h], 30h
        je     SHORT loc_1_D3A0
        mov    edx, dword ptr [esi + 1ah]
        lea    eax, [esi + 18h]
        cmp    dword ptr [ecx + 2], edx
        jne    SHORT loc_1_D377
        mov    ax, word ptr [eax]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_D37E
loc_1_D377:
        mov    dword ptr [ebp - 10h], 1
loc_1_D37E:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_D390
        mov    dword ptr [ebp - 8], 1
        jmp    SHORT loc_1_D3BE
        db  08Dh, 049h, 000h
loc_1_D390:
        mov    dword ptr [ebp - 8], 0
        jmp    SHORT loc_1_D3BE
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_D3A0:
        mov    dword ptr [ebp - 8], 1
        jmp    SHORT loc_1_D3BE
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_D3B0:
        mov    dword ptr [ebp - 8], 20h
        mov    byte ptr [ebp - 1], 0
loc_1_D3BB:
        mov    esi, dword ptr [ebp + 8]
loc_1_D3BE:
        mov    edi, dword ptr [esi + 14h]
        or     edi, edi
        je     NEAR PTR loc_1_D4A1
loc_1_D3C9:
        mov    ecx, dword ptr [edi + 8]
        mov    eax, ecx
        and    eax, dword ptr [ebp - 8]
        test   al, 9
        jne    SHORT loc_1_D40D
        test   ah, 40h
        je     SHORT loc_1_D3E2
        mov    eax, dword ptr [edi + 0ch]
        test   dword ptr [ebp - 0ch], eax
        jne    SHORT loc_1_D40D
loc_1_D3E2:
        test   byte ptr [ebp - 7], 40h
        je     SHORT loc_1_D3ED
        test   ch, 20h
        jne    SHORT loc_1_D40D
loc_1_D3ED:
        test   byte ptr [ebp - 7], 10h
        je     SHORT loc_1_D3F9
        cmp    byte ptr [edi + 1dh], 0
        jne    SHORT loc_1_D40D
loc_1_D3F9:
        test   cl, 10h
        je     SHORT loc_1_D404
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_1_D40D
loc_1_D404:
        test   cl, 20h
        je     NEAR PTR loc_1_D496
loc_1_D40D:
        inc    dword ptr [edi + 18h]
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
        mov    eax, dword ptr [edi + 4]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_1_D742
        mov    byte ptr [edi + 1eh], 1
        mov    eax, dword ptr [edi + 18h]
        dec    eax
        mov    dword ptr [edi + 18h], eax
        jne    SHORT loc_1_D496
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_D44F
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_D44F:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_D460
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_D466
        db  08Bh, 0FFh
loc_1_D460:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 14h], eax
loc_1_D466:
        cmp    byte ptr [edi + 1eh], 0
        je     SHORT loc_1_D475
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_D748
loc_1_D475:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 2ah]
        mov    eax, 1
        mov    cl, byte ptr [edi + 1ch]
        shl    eax, cl
        or     dword ptr [esi + 2eh], eax
        push   0
        push   1fh
        push   edi
        call   sub_1_D7EC
        mov    edi, ebx
        jmp    SHORT loc_1_D499
loc_1_D496:
        mov    edi, dword ptr [edi + 10h]
loc_1_D499:
        or     edi, edi
        jne    NEAR PTR loc_1_D3C9
loc_1_D4A1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_D4B0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_D4C0
        pop    ebp
        ret    4
sub_1_D4C0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 14h]
        or     edi, edi
        je     SHORT loc_1_D53E
loc_1_D4D0:
        inc    dword ptr [edi + 18h]
        cmp    byte ptr [edi + 1eh], 0
        je     SHORT loc_1_D4E6
        mov    byte ptr [edi + 1eh], 0
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_D748
loc_1_D4E6:
        mov    eax, dword ptr [edi + 18h]
        dec    eax
        mov    dword ptr [edi + 18h], eax
        jne    SHORT loc_1_D537
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_D4FC
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_D4FC:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_D510
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_D516
        db  005h, 000h, 000h, 000h, 000h
loc_1_D510:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 14h], eax
loc_1_D516:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 2ah]
        mov    eax, 1
        mov    cl, byte ptr [edi + 1ch]
        shl    eax, cl
        or     dword ptr [esi + 2eh], eax
        push   0
        push   1fh
        push   edi
        call   sub_1_D7EC
        mov    edi, ebx
        jmp    SHORT loc_1_D53A
loc_1_D537:
        mov    edi, dword ptr [edi + 10h]
loc_1_D53A:
        or     edi, edi
        jne    SHORT loc_1_D4D0
loc_1_D53E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    edx, dword ptr [ebp + 10h]
        xor    ecx, ecx
        mov    cl, byte ptr [edx + 4]
        xor    eax, eax
        mov    al, byte ptr [edx + 2]
        shl    eax, 10h
        or     ecx, eax
        shl    ecx, 8
        xor    eax, eax
        mov    al, byte ptr [edx + 3]
        shl    eax, 10h
        or     ecx, eax
        xor    eax, eax
        mov    al, byte ptr [edx + 5]
        or     ecx, eax
        mov    al, byte ptr [edx]
        and    al, byte ptr [edx + 2]
        and    al, 80h
        mov    byte ptr [ebp - 1], al
        or     al, al
        mov    eax, dword ptr [ebp + 8]
        je     SHORT loc_1_D5A2
        cmp    dword ptr [eax + 8], ecx
        jne    SHORT loc_1_D5A2
        cmp    dword ptr [eax + 0ch], 0
        je     SHORT loc_1_D5A2
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_D5A2:
        mov    esi, dword ptr [eax + 10h]
        or     esi, esi
        jne    SHORT loc_1_D5B2
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_D5B2:
        test   esi, 20h
        je     SHORT loc_1_D5C3
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_D5C3:
        mov    al, byte ptr [edx]
        shr    al, 7
        or     al, al
        je     SHORT loc_1_D5E0
        test   esi, 10h
        je     SHORT loc_1_D5E0
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h
loc_1_D5E0:
        mov    eax, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [eax]
        and    cl, 80h
        mov    byte ptr [ebp - 1], cl
        or     cl, cl
        je     SHORT loc_1_D66A
        mov    cl, byte ptr [eax]
        cmp    cl, 0ffh
        jne    SHORT loc_1_D5FC
        cmp    byte ptr [eax + 1], 0ffh
        je     SHORT loc_1_D607
loc_1_D5FC:
        cmp    cl, 0c0h
        jne    SHORT loc_1_D623
        cmp    byte ptr [eax + 1], 0
        jne    SHORT loc_1_D623
loc_1_D607:
        cmp    byte ptr [eax + 2], 0ffh
        jne    SHORT loc_1_D623
        cmp    byte ptr [eax + 3], 0ffh
        jne    SHORT loc_1_D623
        cmp    byte ptr [eax + 4], 0ffh
        jne    SHORT loc_1_D623
        cmp    byte ptr [eax + 5], 0ffh
        mov    byte ptr [ebp - 1], 1
        je     SHORT loc_1_D627
loc_1_D623:
        mov    byte ptr [ebp - 1], 0
loc_1_D627:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_D650
        test   esi, 8
        je     SHORT loc_1_D640
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
loc_1_D640:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_D650:
        test   esi, 6000h
        je     SHORT loc_1_D661
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_D661:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_D66A:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 056h, 08Bh, 04Ch
        db  024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Eh, 05Eh, 0C2h, 00Ch
        db  000h, 056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h
        db  066h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h, 024h, 00Ch, 08Bh
        db  04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 057h, 08Bh, 04Ch, 024h, 010h, 08Bh
        db  07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Ch, 05Fh, 0C2h, 00Ch, 000h, 057h, 08Bh
        db  04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 066h, 06Dh, 05Fh
        db  0C2h, 00Ch, 000h, 057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h
        db  0FCh, 0F3h, 06Dh, 05Fh, 0C2h, 00Ch, 000h, 0CDh, 020h, 031h, 080h, 028h, 000h
sub_1_D700:
        db  0CDh, 020h, 032h, 080h, 028h, 000h
        db  0CDh, 020h, 033h, 080h, 028h, 000h
sub_1_D70C:
        db  0CDh, 020h, 034h, 080h, 028h, 000h
sub_1_D712:
        db  0CDh, 020h, 035h, 080h, 028h, 000h
sub_1_D718:
        db  0CDh, 020h, 036h, 080h, 028h, 000h
sub_1_D71E:
        db  0CDh, 020h, 037h, 080h, 028h, 000h
sub_1_D724:
        db  0CDh, 020h, 038h, 080h, 028h, 000h
sub_1_D72A:
        db  0CDh, 020h, 039h, 080h, 028h, 000h
        db  0CDh, 020h, 03Ah, 080h, 028h, 000h
sub_1_D736:
        db  0CDh, 020h, 03Bh, 080h, 028h, 000h
        db  0CDh, 020h, 03Ch, 080h, 028h, 000h
sub_1_D742:
        db  0CDh, 020h, 03Dh, 080h, 028h, 000h
sub_1_D748:
        db  0CDh, 020h, 03Eh, 080h, 028h, 000h
sub_1_D74E:
        db  0CDh, 020h, 03Fh, 080h, 028h, 000h
sub_1_D754:
        db  0CDh, 020h, 040h, 080h, 028h, 000h
        db  0CDh, 020h, 041h, 080h, 028h, 000h
sub_1_D760:
        db  0CDh, 020h, 009h, 080h, 028h, 000h
        db  0CDh, 020h, 008h, 080h, 028h, 000h
sub_1_D76C:
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
sub_1_D7C2:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
sub_1_D7C8:
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
sub_1_D7CE:
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
sub_1_D7D4:
        db  0CDh, 020h, 010h, 080h, 028h, 000h
sub_1_D7DA:
        db  0CDh, 020h, 011h, 080h, 028h, 000h
sub_1_D7E0:
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_D7E6:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_D7EC:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_D810:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_D834:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_D858:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
        db  0CDh, 020h, 028h, 080h, 028h, 000h
sub_1_D864:
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_D86A:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_D870:
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
sub_1_D8B2:
        db  0CDh, 020h, 046h, 080h, 028h, 000h
sub_1_D8B8:
        db  0CDh, 020h, 045h, 080h, 028h, 000h
        db  0CDh, 020h, 043h, 080h, 028h, 000h
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        add    byte ptr [eax], al
sub_1_D8CC:
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
        db  056h, 057h, 09Ch, 08Bh, 04Ch, 024h, 018h, 08Bh, 074h, 024h, 014h, 08Bh, 07Ch, 024h, 010h, 03Bh
        db  0F7h, 073h, 00Dh, 00Bh, 0C9h, 074h, 01Fh, 0FDh, 049h, 003h, 0F1h, 003h, 0F9h, 041h, 0EBh, 014h
        db  08Bh, 0C6h, 02Bh, 0C7h, 083h, 0F8h, 004h, 072h, 00Bh, 08Ah, 0C1h, 024h, 003h, 0C1h, 0E9h, 002h
        db  0F3h, 0A5h, 08Ah, 0C8h, 0F3h, 0A4h, 09Dh, 05Fh, 05Eh, 0C2h, 00Ch, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 00Ah, 003h, 000h, 000h, 003h, 00Ah, 000h, 000h, 053h, 04Dh, 043h, 038h
        db  031h, 030h, 030h, 057h, 000h, 040h, 000h, 054h, 0E1h, 0D1h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_D980:
        inc    eax
        sub    byte ptr [ebx], ah
        sub    dword ptr [eax], esp
        push   ebx
        dec    ebp
        inc    ebx
        and    byte ptr [eax], bh
        xor    esp, dword ptr [ebx + 38h]
        xor    dh, byte ptr [6f542044h]
loc_1_D993:
        imul   esp, dword ptr [ebp + 6eh], 20h
        push   edx
        imul   ebp, dword ptr [esi + 67h], 6e6f4320h
        db  074h, 072h
        outsd
        insb
        insb
        jb     NEAR PTR loc_1_D9C7
        dec    ebp
        imul   esp, dword ptr [ebx + 72h], 646f636fh
        and    byte ptr gs:[esi + 35h], dh
        cmp    byte ptr cs:[eax], dh
        and    byte ptr [eax], ch
        cmp    dword ptr [ebx], esi
        xor    byte ptr [ecx], bh
        xor    byte ptr [ecx], bh
        sub    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_D9C4:
        and    cl, ch
        db  08Eh
loc_1_D9C7:
        jo     SHORT loc_1_D9F5
        adc    byte ptr [2f162e17h], ch
        adc    eax, 69c000dbh
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        pop    eax
        jns    SHORT loc_1_DA3F
        je     SHORT loc_1_D993
        db  072h, 044h
        db  077h, 044h
        db  077h, 0BCh
        ja     SHORT loc_1_DA49
        jns    SHORT loc_1_DA12
        js     SHORT loc_1_DA4D
loc_1_D9F5:
        db  079h, 00Bh
        js     SHORT loc_1_DA46
loc_1_D9F9:
        db  077h, 0A5h
        db  077h, 058h
        jns    SHORT loc_1_D9FF
loc_1_D9FF:
        db  079h, 058h
        jns    SHORT loc_1_DA4B
        ja     SHORT loc_1_D9F9
        pop    ds
        shr    byte ptr [ebp + 25h], 0
        db  0E8h, 00Fh, 08Eh, 070h, 03Ah
        pop    esi
        or     eax, dword ptr [esi]
loc_1_DA12:
        cmp    eax, 0a208025eh
        pop    esi
        add    dword ptr [eax], eax
        pop    ds
        je     SHORT loc_1_DA3D
        mov    al, byte ptr [0f21fa11fh]
        pop    ds
        mov    esi, 1b00005eh
        pop    ds
        pop    edx
        pop    ds
        sub    dword ptr [edi], ebx
        cmp    ebx, dword ptr [edi]
        cmp    al, 1fh
        dec    ebx
        pop    ds
        dec    esp
        pop    ds
        dec    ebp
        pop    ds
        dec    esi
        pop    ds
        dec    edi
        pop    ds
        push   eax
loc_1_DA3D:
        pop    ds
        push   ecx
loc_1_DA3F:
        pop    ds
        push   edx
        pop    ds
        push   ebx
        pop    ds
        push   esp
        pop    ds
loc_1_DA46:
        push   ebp
        pop    ds
        push   esi
loc_1_DA49:
        pop    ds
        push   edi
loc_1_DA4B:
        pop    ds
        pop    eax
loc_1_DA4D:
        pop    ds
        pop    ecx
        pop    ds
        sbb    byte ptr [edi], 81h
        pop    ds
        sbb    byte ptr [edi], 83h
        pop    ds
        test   byte ptr [edi], bl
        test   dword ptr [edi], ebx
        mov    edi, 77004c6ah
        pop    ds
        fcomp  qword ptr [esi - 1]
        push   dword ptr [ebp + 0fh]
        adc    dword ptr [esi], ebx
        add    byte ptr [edi + ebx + 3eh], dh
        pop    ds
        aas
        pop    ds
        inc    eax
        pop    ds
        pop    ebp
        pop    ds
        pop    esi
        pop    ds
        pop    edi
        pop    ds
        lcall  0ff5eh, 65ffff5eh
        inc    edi
        pop    esi
        cmc
        call   ptr [ebx]
        push   esp
        invd
        sbb    edx, dword ptr [eax]
        sub    dword ptr [edi + esi*8 - 1], edx
        sub    dword ptr [eax], edx
        aad    1fh
        sbb    al, 1fh
        xchg   dword ptr [edi], ebx
        in     eax, 6ah
        jbe    SHORT loc_1_DA9C
loc_1_DA9C:
        fild   dword ptr [eax]
        rcl    dword ptr [eax + eax + 20h]
        push   -3ch
        ficomp word ptr [esi - 1]
        add    ch, ah
        popad
        retf   0ca00h
        db  000h, 06Ah, 070h, 09Bh, 05Eh, 0FFh, 0FFh, 0C0h, 067h, 0E8h, 01Fh, 0F2h, 01Fh, 0BEh, 05Eh, 002h
        db  000h, 0BFh, 06Ah, 07Dh, 000h, 077h, 01Fh, 075h, 00Fh, 074h, 057h, 01Dh, 000h, 074h, 010h, 074h
        db  054h, 01Fh, 000h, 074h, 010h, 0A3h, 05Eh, 0A0h, 00Fh, 061h, 05Eh, 000h, 001h, 0A3h, 06Ah, 08Bh
        db  000h, 04Fh, 070h, 0E8h, 00Fh, 0CAh, 06Bh, 0AAh, 002h, 0E6h, 06Bh, 058h, 004h, 0E8h, 01Fh, 01Bh
        db  054h, 000h, 080h, 0E2h, 06Dh, 079h, 000h, 04Dh, 054h, 008h, 000h, 0E2h, 06Bh, 0E4h, 00Bh, 0E8h
        db  00Fh, 0E8h, 01Fh, 0D5h, 000h, 0C0h, 0C4h, 0C0h, 069h, 0B0h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 06Ah, 072h, 091h, 072h, 0C0h, 070h, 028h, 072h, 009h
        db  072h, 09Dh, 071h, 084h, 071h, 069h, 071h, 046h, 072h, 0C0h, 070h, 0C0h, 070h, 0C0h, 070h, 0C0h
        db  070h, 04Bh, 071h, 023h, 071h, 005h, 071h, 0E8h, 00Fh, 08Bh, 000h, 08Eh, 097h, 01Bh, 054h, 000h
        db  008h, 08Eh, 0C5h, 05Ah, 054h, 040h, 000h, 0DDh, 0C5h, 011h, 053h, 006h, 000h, 0E7h, 0C5h, 0E8h
        db  01Fh, 0B0h, 000h, 03Eh, 04Dh, 0E7h, 0C5h, 0B1h, 000h, 03Fh, 04Dh, 0E7h, 0C5h, 0B2h, 000h, 040h
        db  04Dh, 0E7h, 0C5h, 05Ah, 057h, 040h, 000h, 05Ah, 010h, 029h, 057h, 002h, 000h, 029h, 010h, 0E7h
        db  070h, 011h, 053h, 004h, 000h, 0E7h, 0C5h, 0E8h, 01Fh, 05Ah, 054h, 0BFh, 0FFh, 05Ah, 010h, 029h
        db  057h, 002h, 000h, 029h, 010h, 0E8h, 00Fh, 01Bh, 054h, 000h, 020h, 0FBh, 0C4h, 04Dh, 054h, 004h
        db  000h, 0FBh, 0C4h, 0E8h, 01Fh, 04Dh, 054h, 0FBh, 0FFh, 04Dh, 010h, 05Ah, 057h, 000h, 080h, 05Ah
        db  010h, 029h, 057h, 002h, 000h, 029h, 010h, 0E8h, 00Fh, 0E8h, 01Fh, 0FDh, 070h, 029h, 000h, 08Eh
        db  0C4h, 0E8h, 00Fh, 08Bh, 05Eh, 000h, 080h, 0E8h, 01Fh, 01Bh, 057h, 000h, 008h, 01Bh, 010h, 08Eh
        db  070h, 0E8h, 00Fh, 030h, 05Eh, 020h, 000h, 031h, 05Eh, 004h, 010h, 032h, 05Eh, 000h, 0C0h, 033h
        db  05Eh, 0FFh, 0FFh, 034h, 05Eh, 0FFh, 0FFh, 038h, 05Eh, 012h, 000h, 039h, 05Eh, 004h, 000h, 01Ch
        db  05Eh, 000h, 080h, 0E6h, 05Eh, 000h, 080h, 0C4h, 06Bh, 018h, 001h, 087h, 05Eh, 000h, 080h, 069h
        db  05Eh, 00Ch, 000h, 0C4h, 05Eh, 022h, 030h, 0E5h, 05Eh, 008h, 000h, 08Eh, 070h, 05Ah, 057h, 000h
        db  050h, 05Ah, 010h, 029h, 057h, 002h, 000h, 029h, 010h, 0E8h, 00Fh, 030h, 05Eh, 024h, 000h, 031h
        db  05Eh, 002h, 010h, 032h, 05Eh, 000h, 0C0h, 033h, 05Eh, 0FFh, 0FFh, 034h, 05Eh, 0FFh, 0FFh, 038h
        db  05Eh, 016h, 000h, 039h, 05Eh, 002h, 000h, 041h, 05Eh, 001h, 004h, 05Bh, 000h, 042h, 010h, 0C4h
        db  06Bh, 03Ch, 001h, 087h, 05Eh, 000h, 080h, 069h, 05Eh, 00Ch, 000h, 0C4h, 05Eh, 026h, 030h, 01Ch
        db  05Eh, 000h, 040h, 0E6h, 05Eh, 000h, 040h, 0E5h, 05Eh, 008h, 000h, 08Eh, 070h, 0E8h, 00Fh, 030h
        db  05Eh, 020h, 000h, 031h, 05Eh, 003h, 010h, 032h, 05Eh, 000h, 0C0h, 033h, 05Eh, 0FFh, 0FFh, 034h
        db  05Eh, 0FFh, 0FFh, 038h, 05Eh, 012h, 000h, 039h, 05Eh, 003h, 000h, 0C4h, 06Bh, 05Ah, 001h, 087h
        db  05Eh, 000h, 080h, 01Ch, 05Eh, 000h, 020h, 0E6h, 05Eh, 000h, 020h, 0C4h, 05Eh, 022h, 030h, 069h
        db  05Eh, 00Ch, 000h, 0E5h, 05Eh, 008h, 000h, 08Eh, 070h, 087h, 055h, 000h, 080h, 0EAh, 06Bh, 0C0h
        db  000h, 0C4h, 06Bh, 082h, 001h, 01Ch, 05Eh, 080h, 000h, 087h, 05Eh, 000h, 080h, 030h, 05Eh, 020h
        db  000h, 031h, 05Eh, 005h, 0F0h, 032h, 05Eh, 000h, 0C0h, 033h, 05Eh, 0FFh, 0FFh, 034h, 05Eh, 0FFh
        db  0FFh, 038h, 05Eh, 012h, 000h, 039h, 05Eh, 005h, 000h, 0A7h, 072h, 0F2h, 00Fh, 0C0h, 070h, 087h
        db  055h, 000h, 008h, 0EAh, 06Bh, 0C0h, 000h, 0C4h, 06Bh, 082h, 001h, 01Ch, 05Eh, 040h, 000h, 087h
        db  05Eh, 000h, 008h, 030h, 05Eh, 020h, 000h, 031h, 05Eh, 006h, 070h, 032h, 05Eh, 000h, 0C0h, 033h
        db  05Eh, 0FFh, 0FFh, 034h, 05Eh, 0FFh, 0FFh, 038h, 05Eh, 012h, 000h, 039h, 05Eh, 006h, 000h, 0A7h
        db  072h, 01Bh, 054h, 000h, 020h, 0AAh, 0C5h, 0E8h, 00Fh, 0E6h, 05Eh, 020h, 000h, 07Ah, 000h, 07Bh
        db  000h, 07Ch, 000h, 07Dh, 000h, 07Eh, 000h, 07Fh, 000h, 0C0h, 070h, 087h, 055h, 000h, 008h, 0EAh
        db  06Bh, 0C0h, 000h, 0C4h, 06Bh, 082h, 001h, 05Ah, 057h, 000h, 020h, 05Ah, 010h, 029h, 057h, 003h
        db  000h, 029h, 010h, 0E8h, 00Fh, 02Ah, 05Eh, 054h, 000h, 02Bh, 05Eh, 080h, 000h, 0C0h, 06Dh, 0E5h
        db  001h, 0C0h, 06Dh, 0E5h, 001h, 0C0h, 06Dh, 0E5h, 001h, 0C0h, 06Dh, 0E5h, 001h, 0C0h, 06Dh, 0E5h
        db  001h, 0C0h, 06Dh, 0E5h, 001h, 01Ch, 05Eh, 020h, 000h, 087h, 05Eh, 000h, 008h, 030h, 05Eh, 030h
        db  000h, 031h, 05Eh, 000h, 070h, 032h, 05Eh, 000h, 0C0h, 033h, 05Eh, 000h, 000h, 034h, 05Eh, 008h
        db  000h, 038h, 05Eh, 022h, 000h, 039h, 05Eh, 029h, 060h, 041h, 05Eh, 02Dh, 008h, 0D3h, 05Eh, 07Ah
        db  042h, 003h, 05Bh, 0D1h, 05Eh, 02Eh, 008h, 000h, 063h, 0DEh, 05Eh, 07Dh, 000h, 003h, 05Bh, 0A7h
        db  072h, 02Ah, 05Ch, 0D1h, 005h, 02Bh, 05Ch, 0FFh, 044h, 0CFh, 04Ah, 0CFh, 010h, 0D1h, 054h, 000h
        db  0FFh, 0E2h, 06Ah, 0F6h, 001h, 0CFh, 05Eh, 0FFh, 000h, 0E8h, 01Fh, 05Ah, 057h, 080h, 000h, 05Ah
        db  010h, 0E8h, 00Fh, 02Bh, 05Ch, 02Bh, 016h, 0D1h, 054h, 000h, 0FFh, 0CEh, 04Ah, 0CEh, 010h, 0EAh
        db  06Ah, 005h, 002h, 0CEh, 05Eh, 000h, 0FFh, 0E8h, 01Fh, 05Ah, 057h, 080h, 000h, 05Ah, 010h, 0E8h
        db  00Fh, 0D2h, 000h, 02Ah, 05Dh, 02Ah, 016h, 0C0h, 067h, 087h, 055h, 000h, 008h, 0EAh, 06Bh, 0C0h
        db  000h, 0C4h, 06Bh, 082h, 001h, 01Ch, 05Eh, 010h, 000h, 087h, 05Eh, 000h, 008h, 030h, 05Eh, 024h
        db  000h, 031h, 05Eh, 000h, 070h, 032h, 05Eh, 000h, 0C0h, 033h, 05Eh, 000h, 000h, 034h, 05Eh, 008h
        db  000h, 038h, 05Eh, 016h, 000h, 039h, 05Eh, 028h, 060h, 041h, 05Eh, 030h, 004h, 05Ch, 000h, 042h
        db  010h, 0A7h, 072h, 0F2h, 00Fh, 0C0h, 070h, 087h, 055h, 000h, 008h, 0EAh, 06Bh, 0C0h, 000h, 0C4h
        db  06Bh, 026h, 002h, 01Ch, 05Eh, 008h, 000h, 087h, 05Eh, 000h, 008h, 030h, 05Eh, 028h, 000h, 031h
        db  05Eh, 000h, 070h, 032h, 05Eh, 000h, 0C0h, 033h, 05Eh, 000h, 000h, 034h, 05Eh, 008h, 000h, 038h
        db  05Eh, 01Ah, 000h, 039h, 05Eh, 027h, 060h, 041h, 05Eh, 00Ah, 008h, 0D3h, 05Eh, 05Dh, 042h, 003h
        db  05Bh, 0A7h, 072h, 087h, 055h, 000h, 080h, 0EAh, 06Bh, 0C0h, 000h, 0C4h, 06Bh, 026h, 002h, 01Ch
        db  05Eh, 000h, 001h, 087h, 05Eh, 000h, 080h, 030h, 05Eh, 034h, 000h, 031h, 05Eh, 000h, 070h, 032h
        db  05Eh, 000h, 0C0h, 033h, 05Eh, 000h, 000h, 034h, 05Eh, 010h, 000h, 038h, 05Eh, 026h, 000h, 039h
        db  05Eh, 025h, 040h, 041h, 05Eh, 022h, 014h, 042h, 01Fh, 043h, 01Fh, 044h, 01Fh, 045h, 01Fh, 046h
        db  01Fh, 047h, 01Fh, 048h, 01Fh, 049h, 01Fh, 04Ah, 01Fh, 0A7h, 072h, 087h, 055h, 000h, 008h, 0EAh
        db  06Bh, 0C0h, 000h, 0C4h, 06Bh, 026h, 002h, 01Ch, 05Eh, 001h, 000h, 087h, 05Eh, 000h, 008h, 030h
        db  05Eh, 020h, 000h, 031h, 05Eh, 000h, 070h, 032h, 05Eh, 000h, 0C0h, 033h, 05Eh, 000h, 000h, 034h
        db  05Eh, 010h, 000h, 0E8h, 00Fh, 038h, 05Eh, 012h, 000h, 039h, 05Eh, 026h, 040h, 0E8h, 01Fh, 01Bh
        db  054h, 000h, 020h, 08Dh, 0C5h, 01Bh, 057h, 000h, 020h, 01Bh, 010h, 029h, 057h, 008h, 000h, 029h
        db  010h, 029h, 057h, 004h, 000h, 029h, 010h, 0A7h, 072h, 01Ch, 055h, 002h, 000h, 0E2h, 06Ah, 0C0h
        db  000h, 01Ch, 05Eh, 002h, 000h, 087h, 05Eh, 000h, 080h, 030h, 05Eh, 012h, 000h, 031h, 05Eh, 001h
        db  070h, 035h, 000h, 032h, 010h, 036h, 000h, 033h, 010h, 037h, 000h, 034h, 010h, 038h, 05Eh, 004h
        db  000h, 039h, 05Eh, 007h, 000h, 0E9h, 05Eh, 020h, 000h, 08Eh, 070h, 0F0h, 00Fh, 00Ch, 054h, 00Fh
        db  000h, 0D1h, 052h, 0B0h, 002h, 0C0h, 06Fh, 0C3h, 072h, 063h, 074h, 0CBh, 072h, 057h, 073h, 069h
        db  073h, 00Bh, 074h, 064h, 073h, 0C6h, 073h, 0C3h, 072h, 0C3h, 072h, 0C3h, 072h, 0C3h, 072h, 0C3h
        db  072h, 0C3h, 072h, 0C3h, 072h, 0C9h, 072h, 00Bh, 05Eh, 001h, 080h, 0C5h, 072h, 00Bh, 05Eh, 000h
        db  080h, 08Eh, 05Eh, 010h, 000h, 0F0h, 01Fh, 08Eh, 070h, 0F5h, 00Fh, 0CAh, 072h, 011h, 000h, 0E2h
        db  06Dh, 079h, 000h, 0E8h, 01Fh, 0F2h, 01Fh, 0BEh, 05Eh, 001h, 000h, 074h, 057h, 001h, 000h, 074h
        db  010h, 09Bh, 05Eh, 0FFh, 0FFh, 0A3h, 05Eh, 060h, 0EAh, 061h, 05Eh, 000h, 001h, 0BFh, 06Ah, 0DBh
        db  002h, 0A3h, 06Ah, 0DDh, 002h, 09Bh, 05Eh, 0FFh, 0FFh, 0A3h, 05Eh, 0E8h, 003h, 061h, 05Eh, 000h
        db  001h, 0A3h, 06Ah, 0E5h, 002h, 09Bh, 054h, 018h, 000h, 0E2h, 06Bh, 0C0h, 002h, 0BEh, 05Eh, 002h
        db  000h, 0BFh, 06Ah, 0EDh, 002h, 077h, 01Fh, 074h, 05Eh, 05Dh, 002h, 075h, 01Fh, 011h, 05Eh, 001h
        db  000h, 089h, 05Eh, 000h, 001h, 061h, 05Eh, 001h, 000h, 065h, 05Eh, 07Fh, 0FFh, 09Bh, 05Eh, 0BFh
        db  0FFh, 06Eh, 05Eh, 010h, 010h, 0DCh, 05Eh, 0FFh, 0D1h, 00Bh, 05Eh, 000h, 080h, 08Eh, 05Eh, 010h
        db  000h, 0F0h, 01Fh, 01Bh, 01Fh, 0B0h, 000h, 035h, 010h, 0B1h, 000h, 036h, 010h, 0B2h, 000h, 037h
        db  010h, 0BFh, 06Ah, 00Dh, 003h, 084h, 01Fh, 09Bh, 05Eh, 010h, 000h, 077h, 01Fh, 08Eh, 070h, 011h
        db  000h, 0E2h, 06Dh, 079h, 000h, 0E8h, 01Fh, 074h, 057h, 001h, 000h, 074h, 010h, 0A3h, 05Eh, 040h
        db  09Ch, 061h, 05Eh, 000h, 001h, 0FFh, 060h, 0A3h, 06Ah, 020h, 003h, 09Bh, 05Eh, 0FFh, 0FFh, 0A3h
        db  05Eh, 0E8h, 003h, 061h, 05Eh, 000h, 001h, 0A3h, 06Ah, 028h, 003h, 09Bh, 054h, 018h, 000h, 0E2h
        db  06Bh, 0C0h, 002h, 074h, 05Eh, 03Bh, 002h, 0A3h, 05Eh, 0E8h, 003h, 061h, 05Eh, 000h, 001h, 0A3h
        db  06Ah, 034h, 003h, 074h, 057h, 0C0h, 000h, 074h, 010h, 09Bh, 05Eh, 0FFh, 0FFh, 043h, 073h, 0E8h
        db  01Fh, 074h, 05Eh, 0DBh, 002h, 043h, 073h, 0E8h, 01Fh, 074h, 05Eh, 0CBh, 002h, 0F2h, 01Fh, 0BEh
        db  05Eh, 001h, 000h, 011h, 05Eh, 008h, 000h, 089h, 05Eh, 000h, 007h, 0A5h, 05Eh, 0E8h, 003h, 061h
        db  05Eh, 018h, 000h, 065h, 05Eh, 0CFh, 0FFh, 0D5h, 05Eh, 000h, 081h, 01Ch, 01Fh, 087h, 01Fh, 0BFh
        db  06Ah, 054h, 003h, 0FBh, 072h, 0E8h, 01Fh, 011h, 000h, 05Ch, 0C4h, 089h, 05Eh, 000h, 000h, 00Bh
        db  05Eh, 000h, 080h, 08Eh, 05Eh, 010h, 000h, 0F0h, 01Fh, 0C0h, 06Dh, 079h, 000h, 08Eh, 070h, 012h
        db  05Eh, 000h, 0A0h, 0D1h, 05Eh, 02Eh, 000h, 076h, 073h, 00Dh, 000h, 012h, 010h, 0E8h, 01Fh, 00Dh
        db  054h, 0FFh, 000h, 0CBh, 000h, 013h, 010h, 00Ch, 045h, 0E8h, 00Fh, 0E2h, 06Ah, 083h, 003h, 0EAh
        db  06Ah, 083h, 003h, 013h, 010h, 00Dh, 05Eh, 00Ch, 015h, 0E8h, 01Fh, 01Bh, 054h, 000h, 0FFh, 001h
        db  047h, 01Bh, 010h, 0E8h, 00Fh, 08Eh, 05Eh, 001h, 000h, 0F0h, 01Fh, 08Eh, 070h, 013h, 057h, 000h
        db  015h, 00Dh, 010h, 0E8h, 01Fh, 01Bh, 054h, 000h, 0FFh, 003h, 047h, 01Bh, 010h, 0E8h, 00Fh, 08Eh
        db  05Eh, 003h, 000h, 0F0h, 01Fh, 08Eh, 070h, 0E8h, 01Fh, 013h, 055h, 00Ch, 000h, 0E8h, 00Fh, 0E2h
        db  06Ah, 0A8h, 003h, 0EAh, 06Ah, 0A8h, 003h, 013h, 010h, 012h, 006h, 0DEh, 05Eh, 015h, 000h, 006h
        db  05Bh, 012h, 016h, 0E8h, 01Fh, 01Bh, 054h, 000h, 0FFh, 001h, 047h, 01Bh, 010h, 0E8h, 00Fh, 08Eh
        db  05Eh, 001h, 000h, 0F0h, 01Fh, 08Eh, 070h, 013h, 057h, 000h, 015h, 00Dh, 010h, 012h, 006h, 0DEh
        db  05Eh, 015h, 000h, 006h, 05Bh, 012h, 016h, 0E8h, 01Fh, 01Bh, 054h, 000h, 0FFh, 003h, 047h, 01Bh
        db  010h, 0E8h, 00Fh, 08Eh, 05Eh, 003h, 000h, 0F0h, 01Fh, 08Eh, 070h, 012h, 006h, 0DEh, 05Eh, 015h
        db  000h, 013h, 000h, 0CFh, 019h, 000h, 05Bh, 00Bh, 05Eh, 000h, 080h, 08Eh, 05Eh, 010h, 000h, 0F0h
        db  01Fh, 08Eh, 070h, 0D3h, 05Eh, 054h, 015h, 006h, 05Bh, 0E8h, 01Fh, 029h, 054h, 0FEh, 0FFh, 029h
        db  010h, 00Dh, 05Eh, 00Ch, 015h, 01Bh, 054h, 000h, 0FFh, 004h, 047h, 01Bh, 010h, 0E8h, 00Fh, 08Eh
        db  05Eh, 002h, 000h, 0F0h, 01Fh, 054h, 01Fh, 055h, 01Fh, 056h, 01Fh, 057h, 01Fh, 058h, 01Fh, 059h
        db  01Fh, 08Eh, 070h, 0D3h, 05Eh, 05Ah, 015h, 006h, 05Bh, 0E8h, 01Fh, 029h, 054h, 0FDh, 0FFh, 029h
        db  010h, 00Dh, 05Eh, 00Ch, 015h, 05Ah, 054h, 040h, 000h, 05Ah, 010h, 01Bh, 054h, 000h, 0FFh, 005h
        db  047h, 01Bh, 010h, 0E8h, 00Fh, 08Eh, 05Eh, 002h, 000h, 0F0h, 01Fh, 08Eh, 070h, 0D3h, 05Eh, 03Eh
        db  015h, 003h, 05Bh, 018h, 05Eh, 008h, 005h, 029h, 000h, 019h, 010h, 01Ah, 01Fh, 0E8h, 01Fh, 029h
        db  054h, 0F3h, 0FFh, 029h, 010h, 00Dh, 05Eh, 00Ch, 015h, 01Bh, 054h, 0F0h, 0F7h, 01Bh, 010h, 0E8h
        db  00Fh, 00Bh, 05Eh, 000h, 080h, 08Eh, 05Eh, 012h, 000h, 0F0h, 01Fh, 08Eh, 070h, 00Dh, 000h, 0EAh
        db  054h, 0FFh, 000h, 012h, 010h, 00Dh, 054h, 0FFh, 000h, 0CBh, 000h, 013h, 010h, 00Ch, 045h, 0E2h
        db  06Ah, 042h, 004h, 0EAh, 06Ah, 042h, 004h, 013h, 010h, 0DDh, 05Eh, 000h, 015h, 012h, 000h, 0DEh
        db  010h, 006h, 05Bh, 012h, 016h, 00Dh, 05Eh, 00Ch, 015h, 0E8h, 01Fh, 01Bh, 054h, 000h, 0FFh, 002h
        db  047h, 01Bh, 010h, 0E8h, 00Fh, 08Eh, 05Eh, 002h, 000h, 0F0h, 01Fh, 08Eh, 070h, 013h, 055h, 00Ch
        db  000h, 0E2h, 06Ah, 042h, 004h, 0EAh, 06Ah, 042h, 004h, 013h, 010h, 0DDh, 05Eh, 000h, 015h, 012h
        db  000h, 0DEh, 010h, 006h, 05Bh, 012h, 016h, 0E8h, 01Fh, 01Bh, 054h, 000h, 0FFh, 002h, 047h, 01Bh
        db  010h, 0E8h, 00Fh, 08Eh, 05Eh, 002h, 000h, 0F0h, 01Fh, 08Eh, 070h, 0DDh, 05Eh, 000h, 015h, 012h
        db  000h, 0DEh, 010h, 013h, 000h, 0CFh, 019h, 000h, 05Bh, 013h, 057h, 000h, 015h, 00Dh, 010h, 0E8h
        db  01Fh, 01Bh, 054h, 000h, 0FFh, 000h, 047h, 01Bh, 010h, 0E8h, 00Fh, 00Bh, 05Eh, 000h, 080h, 08Eh
        db  05Eh, 012h, 000h, 0F0h, 01Fh, 08Eh, 070h, 01Bh, 000h, 00Fh, 044h, 0D1h, 052h, 05Dh, 004h, 0C0h
        db  06Fh, 09Ch, 070h, 090h, 073h, 02Bh, 074h, 0BAh, 073h, 0DEh, 073h, 0F3h, 073h, 011h, 000h, 0E2h
        db  06Dh, 079h, 000h, 00Dh, 054h, 00Fh, 000h, 0D1h, 052h, 06Bh, 004h, 0C0h, 06Fh, 072h, 074h, 040h
        db  073h, 014h, 073h, 061h, 076h, 03Ch, 073h, 090h, 076h, 0E7h, 076h, 0E8h, 01Fh, 0C0h, 06Dh, 07Ch
        db  004h, 0C0h, 06Dh, 025h, 000h, 0E8h, 00Fh, 08Eh, 05Eh, 010h, 000h, 0F0h, 01Fh, 08Eh, 070h, 00Bh
        db  05Eh, 002h, 080h, 0D1h, 05Eh, 0FFh, 0FFh, 001h, 042h, 0E2h, 067h, 0D1h, 05Eh, 000h, 080h, 0D1h
        db  052h, 0FFh, 0FFh, 0E2h, 066h, 0D1h, 053h, 0FFh, 07Fh, 0E2h, 067h, 0D1h, 05Eh, 0A5h, 05Ah, 0EAh
        db  053h, 05Ah, 0A5h, 0E2h, 067h, 0D1h, 05Eh, 05Ah, 0A5h, 0CDh, 053h, 0ADh, 052h, 0E2h, 067h, 0D1h
        db  05Eh, 0ADh, 052h, 0CBh, 053h, 05Ah, 0A5h, 0E2h, 067h, 0C7h, 05Eh, 0A5h, 000h, 065h, 05Eh, 03Ah
        db  009h, 088h, 05Eh, 02Eh, 001h, 0DCh, 05Eh, 0CCh, 033h, 0D5h, 05Eh, 099h, 066h, 0D4h, 05Eh, 034h
        db  012h, 0A0h, 05Eh, 022h, 05Ah, 0A1h, 05Eh, 039h, 000h, 0B0h, 05Eh, 05Ah, 05Ah, 0B1h, 05Eh, 099h
        db  033h, 0B2h, 05Eh, 08Fh, 047h, 0B3h, 05Eh, 034h, 012h, 0B4h, 05Eh, 0A3h, 09Fh, 0B5h, 05Eh, 0A3h
        db  078h, 0B6h, 05Eh, 0A4h, 098h, 0B7h, 05Eh, 020h, 063h, 0B8h, 05Eh, 041h, 056h, 0B9h, 05Eh, 0CDh
        db  0ABh, 0BAh, 05Eh, 0C8h, 0F7h, 0BBh, 05Eh, 0EFh, 013h, 0BCh, 05Eh, 0CFh, 0DCh, 0D4h, 066h, 0E4h
        db  066h, 0C7h, 053h, 0A5h, 000h, 0E2h, 067h, 065h, 053h, 03Ah, 009h, 0E2h, 067h, 088h, 053h, 02Eh
        db  001h, 0E2h, 067h, 0DCh, 053h, 0CCh, 033h, 0E2h, 067h, 0D5h, 053h, 099h, 066h, 0E2h, 067h, 0D4h
        db  053h, 034h, 012h, 0E2h, 067h, 0A0h, 053h, 022h, 05Ah, 0E2h, 067h, 0A1h, 053h, 039h, 000h, 0E2h
        db  067h, 0B0h, 053h, 05Ah, 05Ah, 0E2h, 067h, 0B1h, 053h, 099h, 033h, 0E2h, 067h, 0B2h, 053h, 08Fh
        db  047h, 0E2h, 067h, 0B3h, 053h, 034h, 012h, 0E2h, 067h, 0B4h, 053h, 0A3h, 09Fh, 0E2h, 067h, 0B5h
        db  053h, 0A3h, 078h, 0E2h, 067h, 0B6h, 053h, 0A4h, 098h, 0E2h, 067h, 0B7h, 053h, 020h, 063h, 0E2h
        db  067h, 0B8h, 053h, 041h, 056h, 0E2h, 067h, 0B9h, 053h, 0CDh, 0ABh, 0E2h, 067h, 0BAh, 053h, 0C8h
        db  0F7h, 0E2h, 067h, 0BBh, 053h, 0EFh, 013h, 0E2h, 067h, 0BCh, 053h, 0CFh, 0DCh, 0E2h, 067h, 0C7h
        db  05Eh, 05Ah, 000h, 065h, 05Eh, 0C5h, 006h, 088h, 05Eh, 0D1h, 000h, 0DCh, 05Eh, 033h, 0CCh, 0D5h
        db  05Eh, 066h, 099h, 0D4h, 05Eh, 000h, 000h, 0A0h, 05Eh, 0BBh, 0A5h, 0A1h, 05Eh, 0C6h, 000h, 0B0h
        db  05Eh, 0A5h, 0A5h, 0B1h, 05Eh, 066h, 0CCh, 0B2h, 05Eh, 070h, 0B8h, 0B3h, 05Eh, 0CBh, 0EDh, 0B4h
        db  05Eh, 05Ch, 060h, 0B5h, 05Eh, 05Ch, 087h, 0B6h, 05Eh, 05Bh, 067h, 0B7h, 05Eh, 0DFh, 09Ch, 0B8h
        db  05Eh, 0BEh, 0A9h, 0B9h, 05Eh, 032h, 05Bh, 0BAh, 05Eh, 037h, 008h, 0BBh, 05Eh, 010h, 0ECh, 0BCh
        db  05Eh, 030h, 023h, 0D4h, 067h, 0E4h, 066h, 0C7h, 053h, 05Ah, 000h, 0E2h, 067h, 065h, 053h, 0C5h
        db  006h, 0E2h, 067h, 088h, 053h, 0D1h, 000h, 0E2h, 067h, 0DCh, 053h, 033h, 0CCh, 0E2h, 067h, 0D5h
        db  053h, 066h, 099h, 0E2h, 067h, 0D4h, 000h, 0E2h, 067h, 0D5h, 05Eh, 000h, 000h, 0D4h, 05Eh, 0CBh
        db  0EDh, 0D4h, 066h, 0E4h, 067h, 0D5h, 000h, 0E2h, 067h, 0D4h, 053h, 0CBh, 0EDh, 0E2h, 067h, 0A0h
        db  053h, 0BBh, 0A5h, 0E2h, 067h, 0A1h, 053h, 0C6h, 000h, 0E2h, 067h, 0B0h, 053h, 0A5h, 0A5h, 0E2h
        db  067h, 0B1h, 053h, 066h, 0CCh, 0E2h, 067h, 0B2h, 053h, 070h, 0B8h, 0E2h, 067h, 0B3h, 053h, 0CBh
        db  0EDh, 0E2h, 067h, 0B4h, 053h, 05Ch, 060h, 0E2h, 067h, 0B5h, 053h, 05Ch, 087h, 0E2h, 067h, 0B6h
        db  053h, 05Bh, 067h, 0E2h, 067h, 0B7h, 053h, 0DFh, 09Ch, 0E2h, 067h, 0B8h, 053h, 0BEh, 0A9h, 0E2h
        db  067h, 0B9h, 053h, 032h, 05Bh, 0E2h, 067h, 0BAh, 053h, 037h, 008h, 0E2h, 067h, 0BBh, 053h, 010h
        db  0ECh, 0E2h, 067h, 0BCh, 053h, 030h, 023h, 0E2h, 067h, 031h, 05Eh, 000h, 080h, 0C0h, 06Dh, 0DBh
        db  005h, 0E2h, 067h, 031h, 05Eh, 000h, 040h, 0C0h, 06Dh, 0DBh, 005h, 0E2h, 067h, 031h, 05Eh, 000h
        db  020h, 0C0h, 06Dh, 0DBh, 005h, 0E2h, 067h, 031h, 05Eh, 000h, 010h, 0C0h, 06Dh, 0DBh, 005h, 0E2h
        db  067h, 031h, 05Eh, 000h, 008h, 0C0h, 06Dh, 0DBh, 005h, 0E2h, 067h, 031h, 05Eh, 000h, 008h, 0C0h
        db  06Dh, 0DBh, 005h, 0E2h, 067h, 031h, 05Eh, 000h, 004h, 0C0h, 06Dh, 0DBh, 005h, 0E2h, 067h, 031h
        db  05Eh, 000h, 002h, 0C0h, 06Dh, 0DBh, 005h, 0E2h, 067h, 031h, 05Eh, 000h, 081h, 0C0h, 06Dh, 0DBh
        db  005h, 0E2h, 067h, 031h, 05Eh, 080h, 000h, 0C0h, 06Dh, 015h, 006h, 0E2h, 067h, 031h, 05Eh, 040h
        db  000h, 0C0h, 06Dh, 015h, 006h, 0E2h, 067h, 031h, 05Eh, 020h, 000h, 0C0h, 06Dh, 015h, 006h, 0E2h
        db  067h, 031h, 05Eh, 010h, 000h, 0C0h, 06Dh, 015h, 006h, 0E2h, 067h, 031h, 05Eh, 008h, 000h, 0C0h
        db  06Dh, 015h, 006h, 0E2h, 067h, 0A4h, 067h, 0A3h, 067h, 031h, 05Eh, 004h, 000h, 0C0h, 06Dh, 015h
        db  006h, 0E2h, 067h, 0ABh, 067h, 0A9h, 067h, 0A5h, 067h, 0A4h, 066h, 0A3h, 066h, 0A7h, 067h, 0A6h
        db  067h, 031h, 05Eh, 002h, 000h, 0C0h, 06Dh, 015h, 006h, 0E2h, 067h, 0ABh, 066h, 0A9h, 066h, 0A5h
        db  066h, 067h, 01Fh, 0A7h, 066h, 0A6h, 066h, 00Bh, 05Eh, 000h, 080h, 0C0h, 067h, 031h, 000h, 0D1h
        db  054h, 000h, 0FFh, 0EAh, 04Fh, 033h, 010h, 033h, 052h, 0FFh, 0FEh, 034h, 010h, 001h, 042h, 032h
        db  010h, 034h, 052h, 000h, 001h, 0D1h, 052h, 0FFh, 0FFh, 036h, 010h, 034h, 000h, 080h, 010h, 081h
        db  010h, 082h, 010h, 083h, 010h, 084h, 010h, 085h, 010h, 032h, 000h, 0C3h, 010h, 033h, 005h, 034h
        db  006h, 067h, 01Fh, 034h, 000h, 00Bh, 010h, 0D2h, 000h, 034h, 04Bh, 0E2h, 067h, 0C3h, 000h, 00Bh
        db  010h, 036h, 04Bh, 0E2h, 067h, 0D3h, 000h, 00Bh, 010h, 033h, 04Bh, 0E2h, 067h, 080h, 000h, 033h
        db  04Bh, 0E2h, 067h, 081h, 000h, 033h, 04Bh, 0E2h, 067h, 082h, 000h, 031h, 04Bh, 0E2h, 067h, 083h
        db  000h, 033h, 04Bh, 0E2h, 067h, 084h, 000h, 033h, 04Bh, 0E2h, 067h, 085h, 000h, 031h, 04Bh, 0E2h
        db  067h, 031h, 000h, 0A2h, 010h, 0A3h, 010h, 0A4h, 010h, 0A5h, 010h, 0A6h, 010h, 0A7h, 010h, 0A8h
        db  010h, 0A9h, 010h, 0AAh, 010h, 0ABh, 010h, 0ACh, 010h, 0ADh, 010h, 0AEh, 010h, 0D1h, 052h, 0FFh
        db  0FFh, 030h, 010h, 0D1h, 054h, 0FEh, 0FFh, 035h, 010h, 031h, 000h, 030h, 000h, 076h, 010h, 061h
        db  05Eh, 0F5h, 0FFh, 064h, 01Fh, 067h, 01Fh, 0AEh, 000h, 035h, 04Bh, 0E2h, 067h, 0ADh, 000h, 035h
        db  04Bh, 0E2h, 067h, 0ACh, 000h, 035h, 04Bh, 0E2h, 067h, 0A4h, 000h, 035h, 04Bh, 0E2h, 067h, 0A3h
        db  000h, 035h, 04Bh, 0E2h, 067h, 0ABh, 000h, 030h, 04Bh, 0E2h, 067h, 0A9h, 000h, 030h, 04Bh, 0E2h
        db  067h, 0A7h, 000h, 030h, 04Bh, 0E2h, 067h, 0A6h, 000h, 030h, 04Bh, 0E2h, 067h, 0A5h, 000h, 030h
        db  04Bh, 0E2h, 067h, 0A2h, 000h, 030h, 04Bh, 0E2h, 067h, 076h, 000h, 031h, 04Bh, 0E2h, 067h, 061h
        db  05Eh, 00Ah, 000h, 067h, 01Fh, 0A8h, 000h, 030h, 04Bh, 0E2h, 067h, 0AAh, 000h, 030h, 04Bh, 0E2h
        db  067h, 0C0h, 067h, 00Bh, 05Eh, 003h, 080h, 088h, 076h, 0F4h, 00Fh, 0F5h, 01Fh, 0CCh, 01Fh, 0CCh
        db  000h, 0E7h, 053h, 001h, 000h, 0E2h, 06Bh, 05Eh, 006h, 0CCh, 000h, 0E7h, 053h, 002h, 000h, 0E2h
        db  06Bh, 05Eh, 006h, 0CCh, 000h, 0E7h, 053h, 003h, 000h, 0E2h, 06Bh, 05Eh, 006h, 0CCh, 01Fh, 0CCh
        db  000h, 0ABh, 05Eh, 080h, 004h, 061h, 05Eh, 040h, 000h, 0D1h, 04Bh, 095h, 01Fh, 0CBh, 000h, 0CCh
        db  04Ah, 0ABh, 06Ah, 07Ah, 006h, 0D1h, 053h, 011h, 011h, 0E2h, 06Ah, 086h, 006h, 00Bh, 05Eh, 001h
        db  080h, 088h, 076h, 00Bh, 05Eh, 000h, 080h, 0CCh, 01Fh, 0CCh, 000h, 0F1h, 01Fh, 08Eh, 05Eh, 010h
        db  000h, 0F0h, 01Fh, 0F4h, 01Fh, 08Eh, 070h, 0E8h, 01Fh, 0C0h, 06Dh, 04Fh, 000h, 074h, 05Eh, 099h
        db  000h, 075h, 01Fh, 061h, 05Eh, 010h, 000h, 00Eh, 000h, 030h, 010h, 00Fh, 000h, 031h, 010h, 00Bh
        db  05Eh, 000h, 080h, 08Eh, 05Eh, 010h, 000h, 0F0h, 01Fh, 09Bh, 05Eh, 0FFh, 0FFh, 01Bh, 01Fh, 0B0h
        db  000h, 035h, 010h, 0B1h, 000h, 036h, 010h, 0B2h, 000h, 037h, 010h, 030h, 05Eh, 020h, 000h, 031h
        db  05Eh, 004h, 010h, 032h, 01Fh, 033h, 01Fh, 034h, 01Fh, 0C4h, 06Ah, 0B9h, 006h, 0D8h, 06Bh, 0B8h
        db  006h, 0CAh, 06Bh, 0DBh, 006h, 0B1h, 076h, 088h, 000h, 0A5h, 06Ah, 0B9h, 006h, 09Bh, 05Eh, 0FFh
        db  0FFh, 087h, 05Eh, 000h, 080h, 069h, 05Eh, 00Ch, 000h, 0C5h, 05Eh, 022h, 030h, 0C3h, 06Ah, 0C3h
        db  006h, 0CAh, 06Bh, 0D6h, 006h, 0D8h, 06Bh, 0D4h, 006h, 0D9h, 05Eh, 0FEh, 030h, 0C3h, 06Ah, 0CBh
        db  006h, 0CAh, 06Bh, 0D6h, 006h, 0D8h, 06Bh, 0D4h, 006h, 0D9h, 05Eh, 0FEh, 031h, 0C3h, 076h, 088h
        db  000h, 0B9h, 076h, 0D8h, 05Eh, 0FEh, 030h, 0D8h, 06Ah, 0D8h, 006h, 088h, 000h, 00Ch, 053h, 001h
        db  000h, 0E2h, 06Bh, 0E3h, 006h, 00Dh, 053h, 006h, 000h, 0E2h, 06Ah, 0F2h, 006h, 0C0h, 06Dh, 04Fh
        db  000h, 0E8h, 00Fh, 08Eh, 070h, 0E8h, 01Fh, 0C0h, 06Dh, 04Fh, 000h, 074h, 05Eh, 0B9h, 000h, 075h
        db  01Fh, 061h, 05Eh, 010h, 000h, 0FFh, 060h, 0A5h, 06Ah, 0F0h, 006h, 0E8h, 01Fh, 0F0h, 00Fh, 00Bh
        db  05Eh, 000h, 080h, 08Eh, 05Eh, 010h, 000h, 0F0h, 01Fh, 09Bh, 05Eh, 0FFh, 0FFh, 0CAh, 06Bh, 0DBh
        db  006h, 0FFh, 060h, 0FFh, 060h, 09Bh, 054h, 000h, 004h, 005h, 0C5h, 074h, 05Eh, 0B9h, 000h, 0FBh
        db  076h, 074h, 05Eh, 099h, 000h, 09Bh, 05Eh, 0FFh, 0FFh, 0FBh, 076h, 0FFh, 060h, 0FFh, 060h, 0D4h
        db  010h, 0FFh, 060h, 0FFh, 060h, 0FFh, 060h, 0FFh, 060h, 0D7h, 06Bh, 015h, 007h, 0FFh, 060h, 0FFh
        db  060h, 018h, 0C7h, 0FFh, 060h, 0FFh, 060h, 01Bh, 0D1h, 0FFh, 060h, 0FFh, 060h, 01Eh, 099h, 0FFh
        db  060h, 0FFh, 060h, 066h, 06Bh, 022h, 007h, 0FFh, 060h, 0FFh, 060h, 025h, 0BDh, 0FFh, 060h, 0FFh
        db  060h, 028h, 0A7h, 0FFh, 060h, 0FFh, 060h, 02Bh, 0BBh, 0FFh, 060h, 0FFh, 060h, 02Eh, 0A5h, 0FFh
        db  060h, 0FFh, 060h, 031h, 0ADh, 0FFh, 060h, 0FFh, 060h, 034h, 0A1h, 0FFh, 060h, 0FFh, 060h, 037h
        db  0A3h, 0FFh, 060h, 0FFh, 060h, 03Ah, 0C3h, 0FFh, 060h, 0FFh, 060h, 03Dh, 083h, 0FFh, 060h, 0FFh
        db  060h, 040h, 085h, 0FFh, 060h, 0FFh, 060h, 043h, 093h, 0FFh, 060h, 0FFh, 060h, 043h, 077h, 06Ch
        db  01Fh, 0DBh, 05Eh, 018h, 000h, 058h, 079h, 08Ch, 05Eh, 000h, 007h, 0DBh, 05Eh, 000h, 080h, 06Bh
        db  07Bh, 06Eh, 000h, 0E2h, 052h, 051h, 007h, 0C0h, 06Fh, 060h, 077h, 060h, 077h, 060h, 077h, 060h
        db  077h, 063h, 077h, 08Dh, 077h, 060h, 077h, 060h, 077h, 060h, 077h, 060h, 077h, 060h, 077h, 060h
        db  077h, 0A0h, 077h, 060h, 077h, 060h, 077h, 09Bh, 05Eh, 0AFh, 0FFh, 058h, 079h, 09Bh, 05Eh, 010h
        db  000h, 084h, 054h, 0FFh, 000h, 0D1h, 055h, 002h, 000h, 06Bh, 0D5h, 058h, 079h, 074h, 054h, 0F7h
        db  0FFh, 074h, 010h, 0A3h, 05Eh, 0D0h, 007h, 061h, 05Eh, 000h, 001h, 09Bh, 05Eh, 010h, 000h, 084h
        db  01Fh, 0A3h, 06Ah, 075h, 007h, 084h, 054h, 0FFh, 000h, 0D1h, 055h, 064h, 000h, 06Bh, 0D5h, 074h
        db  057h, 008h, 000h, 074h, 010h, 0FFh, 060h, 0FFh, 060h, 09Bh, 05Eh, 010h, 000h, 07Eh, 000h, 04Dh
        db  054h, 001h, 000h, 0E2h, 06Ah, 058h, 009h, 0A3h, 05Eh, 060h, 0EAh, 061h, 05Eh, 000h, 001h, 058h
        db  079h, 09Bh, 05Eh, 020h, 000h, 04Dh, 054h, 001h, 000h, 0E2h, 06Bh, 058h, 009h, 04Dh, 057h, 001h
        db  000h, 04Dh, 010h, 04Fh, 054h, 080h, 0FFh, 04Fh, 010h, 0A3h, 05Eh, 060h, 0EAh, 061h, 05Eh, 000h
        db  001h, 063h, 05Eh, 002h, 000h, 058h, 079h, 09Ah, 05Eh, 0FFh, 0FFh, 09Bh, 05Eh, 0FFh, 0FFh, 06Bh
        db  07Bh, 0C8h, 000h, 0C0h, 069h, 0B0h, 007h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 0BAh, 077h, 0B8h, 077h, 058h, 079h, 0C8h, 077h, 058h
        db  079h, 058h, 079h, 058h, 079h, 058h, 079h, 077h, 01Fh, 058h, 079h, 0F6h, 00Fh, 058h, 079h, 0DBh
        db  05Eh, 020h, 000h, 030h, 000h, 002h, 042h, 0D1h, 057h, 000h, 030h, 069h, 05Eh, 004h, 000h, 0C4h
        db  010h, 0E5h, 05Eh, 008h, 000h, 058h, 079h, 088h, 000h, 01Ch, 000h, 0E2h, 06Ah, 058h, 009h, 0C0h
        db  069h, 0D0h, 007h, 000h, 000h, 000h, 000h, 0E0h, 077h, 0EDh, 077h, 0E0h, 077h, 0E0h, 077h, 0E0h
        db  077h, 0E0h, 077h, 0E0h, 077h, 0E0h, 077h, 0E0h, 077h, 0E0h, 077h, 0E0h, 077h, 0E0h, 077h, 0E0h
        db  077h, 0FBh, 077h, 0FBh, 077h, 0FBh, 077h, 0C8h, 054h, 002h, 000h, 0E2h, 06Ah, 0E8h, 007h, 077h
        db  01Fh, 01Ch, 01Fh, 087h, 01Fh, 058h, 079h, 01Ch, 000h, 0E6h, 010h, 01Ch, 01Fh, 087h, 01Fh, 058h
        db  079h, 0C8h, 054h, 002h, 000h, 0E4h, 0C5h, 04Bh, 054h, 040h, 000h, 0F5h, 0C5h, 01Ch, 000h, 0E6h
        db  010h, 04Bh, 054h, 0BFh, 0FFh, 04Bh, 010h, 01Ch, 01Fh, 087h, 01Fh, 058h, 079h, 01Ch, 054h, 000h
        db  0E0h, 0E2h, 06Bh, 002h, 008h, 01Ch, 01Fh, 087h, 01Fh, 058h, 079h, 01Ch, 054h, 000h, 080h, 0E2h
        db  06Ah, 058h, 009h, 061h, 05Eh, 000h, 004h, 063h, 05Eh, 000h, 008h, 058h, 079h, 062h, 000h, 0C0h
        db  069h, 010h, 008h, 000h, 000h, 000h, 000h, 058h, 079h, 044h, 078h, 0A3h, 078h, 085h, 078h, 05Bh
        db  078h, 06Ah, 078h, 058h, 079h, 088h, 078h, 058h, 079h, 01Ch, 078h, 058h, 079h, 0BEh, 078h, 0FFh
        db  060h, 022h, 078h, 0FFh, 060h, 0FFh, 060h, 09Bh, 05Eh, 040h, 000h, 0F2h, 01Fh, 0BEh, 05Eh, 001h
        db  000h, 0C8h, 05Eh, 001h, 000h, 0DBh, 05Eh, 080h, 000h, 086h, 05Eh, 000h, 002h, 0A5h, 05Eh, 0E8h
        db  003h, 061h, 05Eh, 010h, 000h, 065h, 05Eh, 0EFh, 0FFh, 04Dh, 054h, 001h, 000h, 036h, 0C4h, 063h
        db  05Eh, 002h, 000h, 011h, 05Eh, 008h, 000h, 089h, 05Eh, 000h, 007h, 0E6h, 05Eh, 0FFh, 0FEh, 0E4h
        db  05Eh, 000h, 080h, 01Ch, 01Fh, 087h, 01Fh, 0BFh, 06Ah, 040h, 008h, 077h, 01Fh, 058h, 079h, 04Dh
        db  054h, 001h, 000h, 058h, 0C4h, 04Dh, 054h, 002h, 000h, 058h, 0C5h, 04Fh, 054h, 07Fh, 000h, 0D1h
        db  053h, 005h, 000h, 055h, 0C4h, 04Fh, 052h, 001h, 000h, 04Fh, 010h, 061h, 05Eh, 000h, 001h, 058h
        db  079h, 04Dh, 057h, 00Eh, 000h, 04Dh, 010h, 062h, 05Eh, 002h, 000h, 058h, 079h, 011h, 052h, 05Eh
        db  008h, 0C0h, 06Fh, 058h, 079h, 058h, 079h, 058h, 079h, 0B9h, 07Bh, 075h, 07Bh, 058h, 079h, 058h
        db  079h, 058h, 079h, 0A1h, 07Ah, 08Ch, 05Eh, 000h, 006h, 06Bh, 07Bh, 011h, 053h, 005h, 000h, 0E2h
        db  06Ah, 067h, 008h, 080h, 000h, 0E2h, 06Bh, 080h, 008h, 081h, 000h, 0E2h, 06Bh, 080h, 008h, 082h
        db  000h, 0E2h, 06Bh, 080h, 008h, 083h, 000h, 0E2h, 06Bh, 080h, 008h, 084h, 000h, 0E2h, 06Bh, 080h
        db  008h, 085h, 000h, 0E2h, 06Ah, 082h, 008h, 0E4h, 05Eh, 020h, 000h, 061h, 05Eh, 008h, 000h, 058h
        db  079h, 08Ch, 05Eh, 000h, 005h, 06Bh, 07Bh, 011h, 052h, 08Bh, 008h, 0C0h, 06Fh, 058h, 079h, 075h
        db  07Bh, 0DBh, 07Ah, 058h, 079h, 075h, 07Bh, 058h, 079h, 094h, 078h, 058h, 079h, 058h, 079h, 061h
        db  05Eh, 002h, 000h, 01Bh, 054h, 000h, 001h, 0E2h, 06Ah, 0A0h, 008h, 01Bh, 054h, 0FFh, 0FEh, 01Bh
        db  010h, 0E4h, 05Eh, 080h, 000h, 058h, 079h, 0E4h, 05Eh, 088h, 000h, 058h, 079h, 011h, 053h, 004h
        db  000h, 0ABh, 0C5h, 062h, 05Eh, 004h, 000h, 0E4h, 05Eh, 040h, 000h, 058h, 079h, 011h, 053h, 003h
        db  000h, 0B4h, 0C5h, 04Bh, 054h, 020h, 000h, 0B4h, 0C5h, 04Bh, 052h, 010h, 000h, 04Bh, 010h, 061h
        db  05Eh, 080h, 000h, 069h, 05Eh, 00Ch, 000h, 030h, 000h, 002h, 042h, 0D1h, 057h, 000h, 030h, 0C4h
        db  010h, 058h, 079h, 062h, 05Eh, 000h, 008h, 011h, 053h, 008h, 000h, 0CFh, 0C5h, 01Ch, 054h, 000h
        db  080h, 0E2h, 06Ah, 058h, 009h, 069h, 05Eh, 00Ch, 000h, 030h, 000h, 002h, 042h, 0D1h, 057h, 000h
        db  030h, 0C4h, 010h, 058h, 079h, 011h, 053h, 007h, 000h, 0E2h, 06Bh, 058h, 009h, 0B8h, 057h, 001h
        db  000h, 0B8h, 010h, 0BEh, 05Eh, 004h, 000h, 0E4h, 05Eh, 080h, 000h, 04Dh, 054h, 010h, 000h, 0E4h
        db  0C5h, 04Dh, 057h, 010h, 000h, 04Dh, 010h, 09Bh, 05Eh, 020h, 000h, 099h, 05Eh, 020h, 000h, 061h
        db  05Eh, 002h, 008h, 065h, 05Eh, 05Fh, 0FDh, 04Dh, 054h, 001h, 000h, 0EDh, 0C4h, 063h, 05Eh, 002h
        db  000h, 0E9h, 05Eh, 080h, 000h, 011h, 05Eh, 006h, 000h, 089h, 05Eh, 000h, 006h, 0E5h, 05Eh, 001h
        db  000h, 0BFh, 06Ah, 0F5h, 008h, 087h, 01Fh, 058h, 079h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 0FFh, 00Fh, 000h, 000h, 04Dh, 054h, 0FCh, 0FFh, 04Dh, 010h, 01Dh, 021h, 01Eh
        db  023h, 01Eh, 023h, 056h, 09Ch, 01Eh, 000h, 03Eh, 04Dh, 00Fh, 0C5h, 01Fh, 000h, 03Fh, 04Dh, 00Fh
        db  0C5h, 020h, 000h, 040h, 04Dh, 0E2h, 01Fh, 0D4h, 017h, 0D1h, 021h, 022h, 021h, 004h, 045h, 040h
        db  0C4h, 053h, 0D4h, 021h, 010h, 0E5h, 06Ah, 017h, 009h, 056h, 08Fh, 0D1h, 021h, 0D1h, 005h, 0CFh
        db  053h, 001h, 000h, 032h, 0C4h, 0CFh, 053h, 002h, 000h, 039h, 0C4h, 0CEh, 000h, 0EAh, 000h, 023h
        db  010h, 021h, 000h, 023h, 04Dh, 040h, 0C4h, 053h, 0D4h, 021h, 010h, 023h, 000h, 0CDh, 055h, 001h
        db  000h, 053h, 097h, 0FFh, 021h, 001h, 045h, 02Eh, 0C5h, 017h, 079h, 027h, 021h, 021h, 000h, 004h
        db  045h, 040h, 0C4h, 053h, 0D4h, 021h, 010h, 017h, 079h, 024h, 023h, 021h, 000h, 008h, 045h, 040h
        db  0C4h, 053h, 0D4h, 021h, 010h, 017h, 079h, 0DEh, 05Eh, 0FFh, 000h, 0E5h, 061h, 0E3h, 01Bh, 0CAh
        db  000h, 058h, 0AFh, 01Dh, 054h, 00Fh, 000h, 0D1h, 052h, 04Bh, 009h, 0C0h, 06Fh, 09Ch, 07Ah, 060h
        db  079h, 062h, 07Ah, 0E6h, 07Ah, 09Ah, 079h, 0B7h, 079h, 020h, 07Ah, 058h, 079h, 0DEh, 05Eh, 0FFh
        db  000h, 0E5h, 061h, 0CAh, 000h, 0CAh, 000h, 0DBh, 000h, 0E2h, 069h, 010h, 000h, 02Fh, 005h, 02Eh
        db  006h, 02Dh, 007h, 02Ch, 000h, 0C0h, 065h, 062h, 05Eh, 008h, 000h, 022h, 000h, 022h, 053h, 007h
        db  000h, 094h, 0C5h, 011h, 053h, 002h, 000h, 058h, 0C5h, 058h, 0A0h, 058h, 0ADh, 058h, 0A5h, 079h
        db  085h, 04Bh, 054h, 001h, 000h, 085h, 0C5h, 04Bh, 057h, 001h, 000h, 04Bh, 010h, 0D5h, 054h, 002h
        db  000h, 081h, 0C5h, 0E4h, 05Eh, 002h, 000h, 058h, 079h, 04Bh, 054h, 002h, 000h, 0E2h, 06Bh, 0E3h
        db  00Ah, 04Bh, 057h, 002h, 000h, 04Bh, 010h, 073h, 079h, 04Bh, 057h, 040h, 000h, 04Bh, 010h, 058h
        db  079h, 0A5h, 05Eh, 028h, 00Ah, 061h, 05Eh, 019h, 000h, 065h, 05Eh, 04Fh, 0FFh, 0E5h, 05Eh, 001h
        db  000h, 011h, 05Eh, 004h, 000h, 089h, 05Eh, 000h, 004h, 0E4h, 05Eh, 040h, 000h, 058h, 079h, 022h
        db  000h, 022h, 053h, 00Bh, 004h, 0E2h, 06Ah, 05Ch, 00Bh, 058h, 079h, 011h, 052h, 09Dh, 009h, 0C0h
        db  06Fh, 058h, 079h, 0A1h, 07Bh, 058h, 079h, 058h, 079h, 0D0h, 07Bh, 058h, 079h, 0B4h, 079h, 058h
        db  079h, 0E1h, 06Ah, 058h, 009h, 061h, 05Eh, 000h, 004h, 065h, 05Eh, 0FFh, 0F7h, 04Dh, 054h, 001h
        db  000h, 0B0h, 0C4h, 063h, 05Eh, 002h, 000h, 01Ch, 01Fh, 011h, 05Eh, 007h, 000h, 058h, 079h, 05Ch
        db  05Eh, 002h, 000h, 0D1h, 079h, 01Eh, 000h, 05Dh, 010h, 01Fh, 000h, 05Eh, 010h, 020h, 000h, 05Fh
        db  010h, 011h, 052h, 0C0h, 009h, 0C0h, 06Fh, 058h, 079h, 0A1h, 07Bh, 058h, 079h, 058h, 079h, 0F8h
        db  079h, 058h, 079h, 0C9h, 079h, 058h, 079h, 058h, 079h, 0B4h, 0C2h, 0C1h, 06Bh, 018h, 00Ah, 0C2h
        db  06Bh, 018h, 00Ah, 0E8h, 06Bh, 040h, 00Ah, 038h, 07Ah, 0D5h, 05Eh, 010h, 000h, 087h, 01Fh, 01Ch
        db  01Fh, 0B8h, 054h, 0FEh, 0FFh, 0B8h, 010h, 0BEh, 05Eh, 003h, 000h, 0FFh, 060h, 0BFh, 06Ah, 0DBh
        db  009h, 077h, 01Fh, 0E5h, 05Eh, 001h, 000h, 074h, 054h, 0FDh, 0FFh, 004h, 047h, 074h, 010h, 0DBh
        db  05Eh, 080h, 000h, 0A5h, 05Eh, 028h, 00Ah, 061h, 05Eh, 019h, 000h, 065h, 05Eh, 04Fh, 0FFh, 04Dh
        db  054h, 001h, 000h, 0F1h, 0C4h, 063h, 05Eh, 002h, 000h, 011h, 05Eh, 004h, 000h, 089h, 05Eh, 000h
        db  004h, 058h, 079h, 087h, 01Fh, 0D3h, 079h, 062h, 05Eh, 008h, 000h, 0C1h, 06Ah, 004h, 00Ah, 0C2h
        db  06Ah, 018h, 00Ah, 061h, 05Eh, 001h, 000h, 01Bh, 054h, 0FFh, 0FDh, 01Bh, 010h, 058h, 079h, 0C2h
        db  06Bh, 018h, 00Ah, 0E8h, 06Bh, 010h, 00Ah, 0E4h, 05Eh, 001h, 000h, 01Eh, 000h, 03Eh, 010h, 01Fh
        db  000h, 03Fh, 010h, 020h, 000h, 040h, 010h, 061h, 05Eh, 081h, 000h, 063h, 05Eh, 084h, 000h, 01Bh
        db  057h, 000h, 002h, 01Bh, 010h, 058h, 079h, 0DBh, 000h, 0E2h, 069h, 010h, 000h, 02Fh, 005h, 02Eh
        db  006h, 02Dh, 007h, 02Ch, 000h, 0C0h, 065h, 01Eh, 000h, 05Dh, 010h, 01Fh, 000h, 05Eh, 010h, 020h
        db  000h, 05Fh, 010h, 011h, 052h, 029h, 00Ah, 0C0h, 06Fh, 058h, 079h, 0A1h, 07Bh, 058h, 079h, 058h
        db  079h, 044h, 07Ah, 058h, 079h, 032h, 07Ah, 058h, 079h, 058h, 079h, 0C1h, 06Bh, 018h, 00Ah, 0C2h
        db  06Bh, 018h, 00Ah, 0E8h, 06Bh, 040h, 00Ah, 0E4h, 05Eh, 001h, 000h, 01Eh, 000h, 03Eh, 010h, 01Fh
        db  000h, 03Fh, 010h, 020h, 000h, 040h, 010h, 01Bh, 057h, 000h, 001h, 01Bh, 010h, 058h, 079h, 062h
        db  05Eh, 008h, 000h, 0C1h, 06Bh, 018h, 00Ah, 0C2h, 06Bh, 018h, 00Ah, 061h, 05Eh, 080h, 000h, 063h
        db  05Eh, 004h, 000h, 01Bh, 054h, 000h, 002h, 054h, 0C4h, 086h, 05Eh, 008h, 000h, 057h, 07Ah, 01Bh
        db  057h, 000h, 002h, 01Bh, 010h, 0E8h, 06Bh, 018h, 00Ah, 0E4h, 05Eh, 001h, 000h, 01Eh, 000h, 03Eh
        db  010h, 01Fh, 000h, 03Fh, 010h, 020h, 000h, 040h, 010h, 058h, 079h, 05Ah, 057h, 000h, 040h, 05Ah
        db  010h, 029h, 057h, 002h, 000h, 029h, 010h, 011h, 052h, 06Bh, 00Ah, 0C0h, 06Fh, 058h, 079h, 09Eh
        db  07Bh, 0DEh, 07Ah, 074h, 07Ah, 077h, 07Ah, 0C2h, 07Ah, 0F6h, 079h, 058h, 079h, 0D1h, 079h, 0E1h
        db  06Ah, 00Bh, 00Bh, 058h, 079h, 0BEh, 05Eh, 002h, 000h, 0E6h, 05Eh, 040h, 000h, 0A5h, 05Eh, 028h
        db  00Ah, 061h, 05Eh, 011h, 000h, 0B0h, 000h, 024h, 04Dh, 0E2h, 06Bh, 09Ch, 00Ah, 0B1h, 000h, 025h
        db  04Dh, 0E2h, 06Bh, 09Ch, 00Ah, 0B2h, 000h, 026h, 04Dh, 0E2h, 06Bh, 09Ch, 00Ah, 027h, 053h, 001h
        db  000h, 0E2h, 06Ah, 09Ch, 00Ah, 0BFh, 06Ah, 08Fh, 00Ah, 065h, 054h, 008h, 000h, 0E2h, 06Ah, 018h
        db  00Ah, 061h, 05Eh, 020h, 000h, 063h, 05Eh, 008h, 000h, 0BFh, 06Ah, 099h, 00Ah, 058h, 079h, 062h
        db  05Eh, 008h, 000h, 0BFh, 06Ah, 09Eh, 00Ah, 058h, 079h, 0A5h, 05Eh, 0E8h, 003h, 061h, 05Eh, 090h
        db  000h, 065h, 05Eh, 0EBh, 0FFh, 04Dh, 054h, 001h, 000h, 0ACh, 0C4h, 063h, 05Eh, 002h, 000h, 04Bh
        db  054h, 0C3h, 0FFh, 04Bh, 010h, 05Ch, 05Eh, 001h, 000h, 0D5h, 05Eh, 010h, 020h, 087h, 01Fh, 01Ch
        db  01Fh, 05Bh, 05Eh, 003h, 000h, 011h, 05Eh, 003h, 000h, 089h, 05Eh, 000h, 003h, 0B8h, 054h, 0FEh
        db  0FFh, 0B8h, 010h, 074h, 054h, 0FDh, 0FFh, 074h, 010h, 058h, 079h, 0E1h, 06Ah, 00Eh, 00Bh, 04Dh
        db  054h, 0FCh, 0FFh, 04Dh, 010h, 0A5h, 05Eh, 0E8h, 003h, 061h, 05Eh, 090h, 000h, 065h, 05Eh, 0EBh
        db  0FFh, 04Bh, 054h, 0C3h, 0FFh, 04Bh, 010h, 0D5h, 05Eh, 000h, 020h, 01Ch, 01Fh, 087h, 01Fh, 05Bh
        db  05Eh, 003h, 000h, 011h, 05Eh, 003h, 000h, 089h, 05Eh, 000h, 003h, 058h, 079h, 08Ch, 05Eh, 000h
        db  003h, 06Bh, 07Bh, 0E6h, 05Eh, 040h, 000h, 08Ch, 05Eh, 000h, 001h, 06Bh, 07Bh, 08Ch, 05Eh, 000h
        db  002h, 06Bh, 07Bh, 05Ah, 057h, 020h, 000h, 05Ah, 010h, 029h, 057h, 002h, 000h, 029h, 010h, 011h
        db  052h, 0EFh, 00Ah, 0C0h, 06Fh, 058h, 079h, 058h, 079h, 0DDh, 07Bh, 0FEh, 07Ah, 0D0h, 07Bh, 058h
        db  079h, 0FBh, 07Ah, 058h, 079h, 0F8h, 07Ah, 0E1h, 06Ah, 0D1h, 009h, 058h, 079h, 05Ch, 05Eh, 001h
        db  000h, 0D1h, 079h, 0E1h, 06Bh, 028h, 00Bh, 0B0h, 000h, 01Eh, 04Dh, 0E2h, 06Bh, 009h, 00Bh, 0B1h
        db  000h, 01Fh, 04Dh, 009h, 0C5h, 0B2h, 000h, 020h, 04Dh, 0EAh, 06Bh, 025h, 00Bh, 074h, 057h, 004h
        db  000h, 074h, 010h, 04Dh, 054h, 0FCh, 0FFh, 04Dh, 010h, 0BEh, 05Eh, 003h, 000h, 0A5h, 05Eh, 028h
        db  00Ah, 061h, 05Eh, 019h, 000h, 065h, 05Eh, 04Fh, 0FFh, 01Ch, 01Fh, 087h, 01Fh, 0D5h, 01Fh, 011h
        db  05Eh, 004h, 000h, 089h, 05Eh, 000h, 004h, 0E5h, 05Eh, 001h, 000h, 0BFh, 06Ah, 022h, 00Bh, 058h
        db  079h, 05Bh, 05Eh, 004h, 000h, 058h, 079h, 03Eh, 000h, 024h, 04Dh, 0E2h, 06Bh, 054h, 00Bh, 03Fh
        db  000h, 025h, 04Dh, 0E2h, 06Bh, 054h, 00Bh, 040h, 000h, 026h, 04Dh, 0E2h, 06Bh, 054h, 00Bh, 04Bh
        db  054h, 020h, 000h, 0E2h, 06Ah, 050h, 00Bh, 04Bh, 054h, 008h, 000h, 0E2h, 06Ah, 050h, 00Bh, 04Dh
        db  054h, 0FCh, 0FFh, 04Dh, 010h, 074h, 057h, 002h, 000h, 074h, 010h, 0A5h, 05Eh, 0E8h, 003h, 061h
        db  05Eh, 018h, 000h, 065h, 05Eh, 0EFh, 0FFh, 01Ch, 01Fh, 0D5h, 05Eh, 000h, 081h, 011h, 05Eh, 008h
        db  000h, 089h, 05Eh, 000h, 007h, 058h, 079h, 04Bh, 052h, 004h, 000h, 04Bh, 010h, 058h, 079h, 05Ch
        db  05Eh, 003h, 000h, 0E4h, 05Eh, 010h, 000h, 01Ch, 01Fh, 087h, 05Eh, 000h, 008h, 00Bh, 07Bh, 08Ch
        db  05Eh, 000h, 004h, 0A0h, 054h, 000h, 080h, 0E2h, 06Bh, 058h, 009h, 074h, 054h, 0BFh, 0FFh, 074h
        db  010h, 05Ah, 057h, 000h, 001h, 05Ah, 010h, 029h, 057h, 002h, 000h, 029h, 010h, 0E8h, 01Fh, 0FFh
        db  060h, 0DCh, 05Eh, 0FFh, 0FFh, 089h, 05Eh, 000h, 000h, 01Bh, 057h, 000h, 080h, 01Bh, 010h, 058h
        db  079h, 0C0h, 06Dh, 078h, 00Bh, 058h, 079h, 074h, 054h, 0FBh, 0FFh, 0D1h, 057h, 080h, 000h, 074h
        db  010h, 0F2h, 01Fh, 0BEh, 05Eh, 001h, 000h, 0C8h, 05Eh, 001h, 000h, 0D5h, 05Eh, 000h, 020h, 05Bh
        db  05Eh, 003h, 000h, 011h, 05Eh, 003h, 000h, 089h, 05Eh, 000h, 003h, 0A5h, 05Eh, 0E8h, 003h, 061h
        db  05Eh, 090h, 000h, 065h, 05Eh, 0EBh, 0FFh, 04Dh, 054h, 001h, 000h, 095h, 0C4h, 063h, 05Eh, 002h
        db  000h, 04Bh, 054h, 0C3h, 0FFh, 04Bh, 010h, 0BFh, 06Ah, 098h, 00Bh, 01Ch, 01Fh, 087h, 01Fh, 077h
        db  01Fh, 0C0h, 067h, 08Ch, 05Eh, 000h, 000h, 06Bh, 07Bh, 074h, 057h, 084h, 000h, 074h, 010h, 04Bh
        db  054h, 0FCh, 0FFh, 04Bh, 010h, 04Dh, 057h, 010h, 000h, 04Dh, 010h, 09Bh, 05Eh, 0FFh, 0FFh, 099h
        db  05Eh, 020h, 000h, 061h, 05Eh, 001h, 000h, 065h, 05Eh, 07Fh, 0FFh, 0E4h, 05Eh, 002h, 000h, 011h
        db  05Eh, 002h, 000h, 089h, 05Eh, 000h, 002h, 058h, 079h, 061h, 05Eh, 084h, 000h, 065h, 05Eh, 0DBh
        db  0FFh, 04Dh, 054h, 001h, 000h, 0C2h, 0C4h, 063h, 05Eh, 002h, 000h, 0D5h, 05Eh, 000h, 040h, 01Ch
        db  01Fh, 087h, 01Fh, 04Dh, 054h, 002h, 000h, 0CBh, 0C4h, 05Bh, 05Eh, 002h, 000h, 011h, 05Eh, 005h
        db  000h, 089h, 05Eh, 000h, 005h, 058h, 079h, 0BEh, 05Eh, 002h, 000h, 0E6h, 05Eh, 040h, 000h, 062h
        db  05Eh, 008h, 000h, 0A5h, 05Eh, 028h, 00Ah, 061h, 05Eh, 010h, 000h, 0BFh, 06Ah, 0DAh, 00Bh, 058h
        db  079h, 0BEh, 05Eh, 002h, 000h, 0E6h, 05Eh, 040h, 000h, 0BFh, 06Ah, 0E1h, 00Bh, 058h, 079h, 04Dh
        db  054h, 0F7h, 0FFh, 04Dh, 010h, 011h, 052h, 0EAh, 00Bh, 0C0h, 06Fh, 09Ch, 070h, 09Ch, 070h, 079h
        db  070h, 09Ch, 070h, 0F3h, 07Bh, 010h, 07Ch, 0F6h, 07Bh, 09Ch, 070h, 0F6h, 07Bh, 0C0h, 06Dh, 078h
        db  00Bh, 09Ch, 070h, 0A5h, 05Eh, 0E8h, 003h, 061h, 05Eh, 090h, 000h, 065h, 05Eh, 0EBh, 0FFh, 04Bh
        db  054h, 0C3h, 0FFh, 04Bh, 010h, 0D5h, 05Eh, 000h, 020h, 087h, 01Fh, 01Ch, 01Fh, 05Bh, 05Eh, 002h
        db  000h, 011h, 05Eh, 003h, 000h, 089h, 05Eh, 000h, 003h, 0B8h, 054h, 0FEh, 0FFh, 0B8h, 010h, 074h
        db  054h, 0FDh, 0FFh, 074h, 010h, 09Ch, 070h, 05Bh, 05Eh, 002h, 000h, 09Ch, 070h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 058h, 045h, 03Bh, 040h, 028h, 023h, 029h, 020h, 053h, 04Dh, 043h, 038h
        db  031h, 030h, 030h, 02Eh, 056h, 058h, 044h, 020h, 076h, 065h, 072h, 073h, 069h, 06Fh, 06Eh, 020h
        db  031h, 02Eh, 030h, 039h, 061h, 020h, 031h, 031h, 02Fh, 032h, 033h, 02Fh, 039h, 034h, 00Ah, 043h
        db  06Fh, 070h, 079h, 072h, 069h, 067h, 068h, 074h, 020h, 028h, 043h, 029h, 020h, 053h, 074h, 061h
        db  06Eh, 064h, 061h, 072h, 064h, 020h, 04Dh, 069h, 063h, 072h, 06Fh, 073h, 079h, 073h, 074h, 065h
        db  06Dh, 073h, 020h, 043h, 06Fh, 072h, 070h, 02Ch, 020h, 031h, 039h, 039h, 034h, 000h, 000h, 040h
        db  028h, 023h, 029h, 020h, 055h, 04Dh, 05Fh, 038h, 031h, 031h, 035h, 06Eh, 064h, 069h, 073h, 033h
        db  05Fh, 073h, 032h, 02Eh, 03Fh, 03Fh, 05Fh, 076h, 031h, 02Eh, 030h, 030h, 000h, 000h, 000h
loc_1_FA3C:
        rol    byte ptr [eax], 0ffh
        db  0FFh, 0FFh, 0FFh, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_FA98:
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
        add    al, byte ptr [edx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ebx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx + eax]
        add    dword ptr [edx], eax
        add    eax, 1020101h
        add    dword ptr [edx], eax
        add    al, byte ptr [edx]
        add    dword ptr [edx], eax
        add    al, byte ptr [ecx]
        add    dword ptr [ecx], eax
        add    byte ptr [edx], al
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], al
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], al
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
        add    al, byte ptr [edx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ebx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [4020101h]
        add    dword ptr [ecx], eax
        add    dl, byte ptr [ebx + 4dh]
        inc    ebx
        cmp    byte ptr [ecx], dh
        xor    byte ptr [eax], dh
        push   edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_FB80:
        push   ebx
        dec    ebp
        inc    ebx
        cmp    byte ptr [ecx], dh
        xor    byte ptr [eax], dh
        push   edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ecx
        dec    edi
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
loc_1_FB94:
        dec    ecx
        dec    edi
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
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
        dec    ecx
        push   edx
        push   ecx
        add    byte ptr [ecx + 52h], cl
        push   ecx
        add    byte ptr [edx + 41h], dl
        dec    ebp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
loc_1_FBE8:
        push   edx
        inc    ecx
        dec    ebp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        inc    edx
        jne    SHORT loc_1_FC6A
        push   esp
        jns    SHORT loc_1_FC6A
        db  065h, 000h
loc_1_FBFC:
        inc    edx
        db  075h, 073h
        push   esp
        db  079h, 070h
        add    byte ptr gs:[ebp + 65h], cl
        imul   esp, dword ptr fs:[ecx + 54h], 657079h
        add    byte ptr [eax], al
        dec    ebp
        imul   esp, dword ptr fs:[ecx + 54h], 657079h
        add    byte ptr [eax], al
        dec    ebp
        popad
        db  078h, 069h
        insd
        jne    SHORT loc_1_FC90
        push   eax
        popad
        arpl   word ptr [ebx + 65h], bp
        je     SHORT loc_1_FC7D
        imul   edi, dword ptr [edx + 65h], 4d000000h
        popad
        js     SHORT loc_1_FC9D
        insd
        db  075h, 06Dh
        push   eax
        popad
        arpl   word ptr [ebx + 65h], bp
        db  074h, 053h
        imul   edi, dword ptr [edx + 65h], 54000000h
        push   edx
        dec    ecx
        dec    esi
        dec    ecx
        push   esp
        inc    ebp
        pop    eax
        inc    ebp
        add    byte ptr [eax], al
        push   esp
        push   edx
        dec    ecx
        dec    esi
        dec    ecx
        push   esp
        inc    ebp
        pop    eax
        inc    ebp
        add    byte ptr [eax], al
loc_1_FC5C:
        push   ebx
        dec    ebp
        inc    ebx
        and    byte ptr [ecx + 64h], al
        popad
        db  070h, 074h
        db  065h, 072h, 02Eh
        add    byte ptr [eax], al
loc_1_FC6A:
        add    byte ptr [eax], al
loc_1_FC6C:
        push   ebx
        dec    ebp
        inc    ebx
        and    byte ptr [edi + ebp*2 + 6bh], dl
        outsb
        inc    ebx
        popad
        db  072h, 064h
        add    byte ptr cs:[eax], al
        inc    eax
loc_1_FC7D:
        sub    byte ptr [ebx], ah
        sub    dword ptr [eax], esp
        push   ebx
        dec    ebp
        inc    ebx
        and    byte ptr [edi + ebp*2 + 6bh], dl
        outsb
        inc    ebx
        popad
        db  072h, 064h
        inc    ebp
        insb
loc_1_FC90:
        imul   esi, dword ptr [ebp + 20h], 696d6146h
        insb
        db  079h, 020h
        dec    esp
        dec    ebp
loc_1_FC9D:
        inc    ecx
        inc    ebx
        and    byte ptr [edx + esi*2 + 69h], al
        db  076h, 065h
        db  072h, 020h
        push   esi
        xor    dword ptr [esi], ebp
        xor    dword ptr [eax], esi
        arpl   word ptr [eax], sp
        sub    byte ptr [ecx], bh
        xor    esi, dword ptr [eax]
        cmp    dword ptr [edx], esi
        xor    byte ptr [ecx], ch
        add    byte ptr [eax], al
loc_1_FCB8:
        add    byte ptr [edx], al
        add    eax, dword ptr [eax + 6]
        add    al, byte ptr [ebx]
        add    eax, 0f0b0a07h
loc_1_FCC8:
        add    byte ptr [eax], al
loc_1_FCCA:
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
_LTEXT ends
        end
