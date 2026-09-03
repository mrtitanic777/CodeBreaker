; pcntn3_lcode_01.asm -- PCNTN3 object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 0efh]
        mov    eax, edx
        shl    edx, 7
        sub    edx, eax
        lea    ecx, [eax + edx*4]
        lea    edx, [ecx + ecx*2]
        sub    edx, eax
        mov    eax, dword ptr [esi + 0cah]
        mov    ecx, eax
        lea    ebx, [eax + eax*4]
        lea    eax, [ecx + ebx*4]
        lea    edi, [ecx + eax*8]
        lea    eax, [edi + edi*8]
        add    eax, ecx
        lea    ecx, [esi + 0c2h]
        push   ecx
        lea    ebx, [edx + eax + 38h]
        lea    eax, [esi + 0c6h]
        mov    edx, dword ptr [esi + 0aeh]
        push   eax
        push   0
        push   ebx
        push   edx
        call   sub_1_4BA2
        mov    edi, dword ptr [esi + 0c6h]
        test   edi, edi
        jne    SHORT loc_1_0068
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0068:
        xor    eax, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep stosd
        mov    ecx, ebx
        and    ecx, 3
        rep stosb
        mov    ecx, dword ptr [esi + 0c6h]
        mov    edx, dword ptr [esi + 0c2h]
        add    ecx, 0fh
        add    edx, 0fh
        and    ecx, 0fffffff0h
        and    edx, 0fffffff0h
        mov    edi, dword ptr [esi + 0cah]
        mov    dword ptr [esi + 105h], ecx
        add    ecx, 20h
        mov    dword ptr [esi + 109h], edx
        add    edx, 20h
        lea    eax, [edi*8]
        mov    dword ptr [esi + 0d6h], ecx
        mov    dword ptr [esi + 0dah], edx
        add    ecx, eax
        add    edx, eax
        mov    eax, dword ptr [esi + 0efh]
        mov    dword ptr [esi + 0fbh], ecx
        shl    eax, 3
        mov    dword ptr [esi + 0ffh], edx
        add    ecx, eax
        add    edx, eax
        lea    eax, [edi + edi*4]
        mov    dword ptr [esi + 0ceh], ecx
        mov    dword ptr [esi + 0d2h], edx
        lea    ebx, [edi + eax*4]
        lea    eax, [ebx + ebx*8]
        lea    ebx, [edi + eax*4]
        pop    edi
        lea    eax, [ebx*2]
        add    ecx, eax
        add    edx, eax
        mov    al, 1
        mov    dword ptr [esi + 0f3h], ecx
        mov    dword ptr [esi + 0f7h], edx
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
sub_1_0110:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [edx + 0efh]
        mov    eax, esi
        shl    esi, 7
        sub    esi, eax
        lea    ecx, [eax + esi*4]
        lea    esi, [ecx + ecx*2]
        sub    esi, eax
        mov    eax, dword ptr [edx + 0cah]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*4]
        lea    eax, [ecx + eax*8]
        lea    eax, [eax + eax*8]
        add    eax, ecx
        mov    ecx, dword ptr [edx + 0c6h]
        test   ecx, ecx
        lea    eax, [esi + eax + 1fh]
        je     SHORT loc_1_0168
        mov    esi, dword ptr [edx + 0c2h]
        push   esi
        push   ecx
        push   0
        push   eax
        mov    eax, dword ptr [edx + 0aeh]
        push   eax
        call   sub_1_4BA8
loc_1_0168:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_0170:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    eax, eax
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 48h]
        lea    ebx, [ecx + 12h]
        add    ecx, 10h
        mov    edx, ebx
        out    dx, ax
        mov    edx, ecx
        in     ax, dx
        test   ax, 4
        mov    di, ax
        je     SHORT loc_1_01D6
        mov    edx, ebx
        xor    eax, eax
        out    dx, ax
        and    di, 0ffbfh
        mov    edx, ecx
        out    dx, ax
        mov    ecx, 1
        mov    byte ptr [esi + 125h], cl
        mov    ebx, dword ptr [esi + 48h]
        mov    dword ptr [esi + 82h], ecx
        lea    edx, [ebx + 12h]
        out    dx, ax
        mov    word ptr [esi + 46h], di
        lea    edx, [ebx + 10h]
        mov    eax, edi
        pop    edi
        and    eax, 0ffffh
        pop    esi
        out    dx, ax
        mov    al, cl
        pop    ebx
        pop    ebp
        ret    4
loc_1_01D6:
        test   di, 80h
        je     SHORT loc_1_021B
        test   di, 40h
        je     SHORT loc_1_021B
        and    di, 0ffbfh
        mov    edx, ebx
        mov    byte ptr [esi + 125h], 1
        xor    eax, eax
        out    dx, ax
        mov    edx, ecx
        mov    ecx, dword ptr [esi + 48h]
        out    dx, ax
        mov    word ptr [esi + 46h], di
        lea    edx, [ecx + 12h]
        out    dx, ax
        lea    edx, [ecx + 10h]
        mov    eax, edi
        and    eax, 0ffffh
        pop    edi
        out    dx, ax
        mov    al, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_021B:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0230:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 2], 0
        jne    NEAR PTR loc_1_048E
        cmp    byte ptr [esi + 0fh], 0
        jne    NEAR PTR loc_1_048E
        mov    di, word ptr [esi + 46h]
        mov    byte ptr [esi + 0fh], 1
        mov    byte ptr [esi + 2dh], 0
        cmp    dword ptr [esi + 82h], 0
        je     NEAR PTR loc_1_02FD
        mov    eax, dword ptr [esi + 86h]
        inc    eax
        mov    dword ptr [esi + 86h], eax
        cmp    byte ptr [esi + 126h], 0
        jne    SHORT loc_1_02F6
        cmp    byte ptr [esi + 10h], 0
        jne    SHORT loc_1_02F6
        mov    byte ptr [esi + 126h], 1
        push   esi
        call   sub_1_3280
        push   esi
        call   sub_1_3910
        mov    ecx, dword ptr [esi + 48h]
        xor    eax, eax
        lea    edx, [ecx + 12h]
        out    dx, ax
        lea    edx, [ecx + 10h]
        mov    eax, 2
        out    dx, ax
        lea    eax, [ebp - 1]
        lea    edx, [esi + 15h]
        push   eax
        push   edx
        call   sub_1_4BF8
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_02C2
        mov    byte ptr [esi + 11h], 0
loc_1_02C2:
        mov    byte ptr [esi + 126h], 0
        xor    eax, eax
        dec    dword ptr [esi + 86h]
        mov    byte ptr [esi + 0fh], 0
        mov    byte ptr [esi + 125h], 0
        mov    ecx, dword ptr [esi + 48h]
        pop    edi
        pop    esi
        lea    edx, [ecx + 12h]
        out    dx, ax
        lea    edx, [ecx + 10h]
        mov    eax, 40h
        mov    esp, ebp
        out    dx, ax
        pop    ebp
        ret    10h
loc_1_02F6:
        dec    eax
        mov    dword ptr [esi + 86h], eax
loc_1_02FD:
        test   di, 400h
        je     SHORT loc_1_032B
        cmp    byte ptr [esi + 126h], 0
        jne    SHORT loc_1_032B
        push   esi
        inc    dword ptr [esi + 86h]
        call   sub_1_05B0
        mov    eax, dword ptr [esi + 0b2h]
        push   eax
        call   sub_1_49A0
        dec    dword ptr [esi + 86h]
loc_1_032B:
        test   di, 8000h
        je     SHORT loc_1_03AB
        inc    dword ptr [esi + 86h]
        test   di, 2000h
        je     SHORT loc_1_0345
        inc    dword ptr [esi + 1aah]
loc_1_0345:
        test   di, 1000h
        je     SHORT loc_1_03A5
        inc    dword ptr [esi + 13ah]
        cmp    byte ptr [esi], 1
        jne    SHORT loc_1_03A5
        cmp    byte ptr [esi + 126h], 0
        jne    SHORT loc_1_03A5
        mov    byte ptr [esi + 126h], 1
        push   esi
        call   sub_1_3280
        push   esi
        call   sub_1_3910
        mov    ecx, dword ptr [esi + 48h]
        xor    eax, eax
        lea    edx, [ecx + 12h]
        out    dx, ax
        lea    edx, [ecx + 10h]
        mov    eax, 2
        out    dx, ax
        lea    eax, [ebp - 1]
        lea    edx, [esi + 15h]
        push   eax
        push   edx
        call   sub_1_4BF8
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_039E
        mov    byte ptr [esi + 11h], 0
loc_1_039E:
        mov    byte ptr [esi + 126h], 0
loc_1_03A5:
        dec    dword ptr [esi + 86h]
loc_1_03AB:
        xor    eax, eax
        inc    dword ptr [esi + 86h]
        mov    ecx, dword ptr [esi + 48h]
        lea    edx, [ecx + 12h]
        out    dx, ax
        lea    edx, [ecx + 10h]
        in     ax, dx
        and    al, 33h
        cmp    al, 33h
        je     SHORT loc_1_041A
        cmp    byte ptr [esi + 126h], 0
        jne    SHORT loc_1_041A
        cmp    byte ptr [esi + 10h], 0
        jne    SHORT loc_1_041A
        mov    byte ptr [esi + 126h], 1
        push   esi
        call   sub_1_3280
        push   esi
        call   sub_1_3910
        mov    ecx, dword ptr [esi + 48h]
        xor    eax, eax
        lea    edx, [ecx + 12h]
        out    dx, ax
        lea    edx, [ecx + 10h]
        mov    eax, 2
        out    dx, ax
        lea    eax, [ebp - 1]
        lea    edx, [esi + 15h]
        push   eax
        push   edx
        call   sub_1_4BF8
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_0413
        mov    byte ptr [esi + 11h], 0
loc_1_0413:
        mov    byte ptr [esi + 126h], 0
loc_1_041A:
        dec    dword ptr [esi + 86h]
        mov    byte ptr [esi + 0fh], 0
        mov    byte ptr [esi + 125h], 0
        cmp    byte ptr [esi + 11h], 0
        jne    SHORT loc_1_047A
        cmp    dword ptr [esi + 119h], 0
        jne    SHORT loc_1_0464
        cmp    dword ptr [esi + 10dh], 0
        jne    SHORT loc_1_0464
        cmp    byte ptr [esi + 126h], 0
        je     SHORT loc_1_0455
        cmp    dword ptr [esi + 86h], 1
        je     SHORT loc_1_0464
loc_1_0455:
        mov    eax, dword ptr [esi + 96h]
        sub    eax, esi
        cmp    eax, 96h
        je     SHORT loc_1_047A
loc_1_0464:
        mov    byte ptr [esi + 11h], 1
        push   0
        inc    dword ptr [esi + 86h]
        push   0
        push   esi
        push   0
        call   sub_1_04A0
loc_1_047A:
        mov    ecx, dword ptr [esi + 48h]
        xor    eax, eax
        lea    edx, [ecx + 12h]
        out    dx, ax
        lea    edx, [ecx + 10h]
        mov    eax, 40h
        out    dx, ax
loc_1_048E:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_04A0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 126h], 0
        je     SHORT loc_1_04D2
        cmp    dword ptr [esi + 86h], 2
        jne    SHORT loc_1_04D2
        mov    byte ptr [esi + 11h], 0
        push   esi
        dec    dword ptr [esi + 86h]
        call   sub_1_3850
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_1_04D2:
        xor    ebx, ebx
loc_1_04D4:
        cmp    dword ptr [esi + 119h], ebx
        je     SHORT loc_1_04F3
        cmp    byte ptr [esi + 126h], bl
        jne    SHORT loc_1_04F3
        cmp    byte ptr [esi + 13h], bl
        jne    SHORT loc_1_04F3
        push   esi
        call   sub_1_1C00
        test   al, al
        jne    SHORT loc_1_04D4
loc_1_04F3:
        xor    ebx, ebx
loc_1_04F5:
        cmp    dword ptr [esi + 10dh], ebx
        je     SHORT loc_1_050C
        cmp    byte ptr [esi + 14h], bl
        jne    SHORT loc_1_050C
        push   esi
        call   sub_1_45B0
        test   al, al
        jne    SHORT loc_1_04F5
loc_1_050C:
        mov    eax, dword ptr [esi + 96h]
        sub    eax, esi
        cmp    eax, 96h
        je     SHORT loc_1_055F
        xor    edi, edi
loc_1_051D:
        mov    ecx, dword ptr [esi + 96h]
        cmp    dword ptr [ecx + 14h], edi
        jne    SHORT loc_1_0550
        mov    ebx, dword ptr [ecx + 10h]
        mov    edx, dword ptr [ecx]
        mov    eax, dword ptr [ecx + 4]
        push   edi
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [ecx + 4]
        mov    edx, dword ptr [ecx]
        push   45h
        push   ecx
        mov    dword ptr [edx + 4], eax
        call   sub_1_4C1C
        push   edi
        dec    dword ptr [esi + 92h]
        push   ebx
        call   sub_1_4B54
loc_1_0550:
        mov    eax, dword ptr [esi + 96h]
        sub    eax, esi
        cmp    eax, 96h
        jne    SHORT loc_1_051D
loc_1_055F:
        cmp    dword ptr [esi + 10dh], 0
        jne    SHORT loc_1_0591
        cmp    dword ptr [esi + 119h], 0
        jne    SHORT loc_1_0591
        mov    eax, dword ptr [esi + 96h]
        sub    eax, esi
        cmp    eax, 96h
        jne    SHORT loc_1_0591
        pop    edi
        dec    dword ptr [esi + 86h]
        mov    byte ptr [esi + 11h], 0
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_1_0591:
        push   0ah
        add    esi, 15h
        push   esi
        call   sub_1_4BF2
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_05B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    cx, word ptr [esi + 103h]
        mov    eax, ecx
        and    eax, 0ffffh
        shl    eax, 3
        add    eax, dword ptr [esi + 0fbh]
        mov    bx, word ptr [eax + 2]
        mov    dword ptr [ebp - 8], eax
        and    bx, 0ff00h
        test   bh, 80h
        jne    NEAR PTR loc_1_07CB
        mov    edi, dword ptr [ebp - 4]
loc_1_05EB:
        mov    ax, word ptr [esi + 103h]
        inc    ax
        mov    word ptr [esi + 103h], ax
        mov    edx, eax
        and    edx, 0ffffh
        cmp    edx, dword ptr [esi + 0efh]
        jb     SHORT loc_1_0614
        mov    word ptr [esi + 103h], 0
loc_1_0614:
        mov    ax, bx
        and    ax, 4000h
        mov    word ptr [ebp - 0ch], ax
        jne    NEAR PTR loc_1_06EC
        test   bh, 1
        je     NEAR PTR loc_1_06EC
        test   bh, 2
        je     NEAR PTR loc_1_06EC
        mov    edx, dword ptr [ebp - 8]
        xor    edi, edi
        mov    di, word ptr [edx + 6]
        sub    edi, 4
        cmp    edi, 5eah
        ja     NEAR PTR loc_1_06EC
        mov    edx, edi
        jb     SHORT loc_1_0658
        mov    edx, 5eah
loc_1_0658:
        and    ecx, 0ffffh
        mov    eax, ecx
        shl    ecx, 6
        sub    ecx, eax
        lea    eax, [eax + ecx*4]
        lea    ecx, [eax + eax*2]
        add    ecx, ecx
        add    ecx, dword ptr [esi + 0f3h]
        cmp    byte ptr [esi + 126h], 0
        jne    NEAR PTR loc_1_07CB
        mov    al, byte ptr [ecx]
        test   al, 1
        je     SHORT loc_1_06A6
        cmp    dword ptr [ecx + 2], -1
        jne    SHORT loc_1_069E
        cmp    al, 0ffh
        jne    SHORT loc_1_069E
        cmp    byte ptr [ecx + 1], 0ffh
        jne    SHORT loc_1_069E
        mov    word ptr [ebp - 4], 4
        jmp    SHORT loc_1_06AC
loc_1_069E:
        mov    word ptr [ebp - 4], 2
        jmp    SHORT loc_1_06AC
loc_1_06A6:
        mov    word ptr [ebp - 4], 1
loc_1_06AC:
        cmp    edi, 0eh
        jae    SHORT loc_1_06CC
        cmp    edi, 6
        jb     SHORT loc_1_06EC
        lea    eax, [ecx + 0eh]
        push   0
        push   0
        push   0
        push   edi
        push   ecx
        push   ecx
        push   eax
        mov    ecx, dword ptr [esi + 0b2h]
        push   ecx
        jmp    SHORT loc_1_06E7
loc_1_06CC:
        lea    eax, [edi - 0eh]
        sub    edx, 0eh
        push   eax
        push   edx
        lea    eax, [ecx + 0eh]
        mov    edx, dword ptr [esi + 0b2h]
        push   eax
        push   0eh
        push   ecx
        push   ecx
        add    ecx, 0eh
        push   ecx
        push   edx
loc_1_06E7:
        call   sub_1_47C0
loc_1_06EC:
        mov    edx, dword ptr [ebp - 8]
        mov    word ptr [edx + 4], 0fa12h
        mov    ax, word ptr [edx + 2]
        and    ax, 0ffh
        mov    word ptr [edx + 2], ax
        or     ah, 80h
        cmp    word ptr [ebp - 0ch], 0
        mov    word ptr [edx + 2], ax
        jne    SHORT loc_1_0774
        inc    dword ptr [esi + 12eh]
        cmp    word ptr [ebp - 4], 4
        jne    SHORT loc_1_0737
        lea    ecx, [esi + 166h]
        inc    dword ptr [esi + 182h]
        mov    eax, dword ptr [ecx]
        add    eax, edi
        mov    dword ptr [ecx], eax
        cmp    eax, edi
        jae    SHORT loc_1_079F
        inc    dword ptr [ecx + 4]
        jmp    SHORT loc_1_079F
loc_1_0737:
        cmp    word ptr [ebp - 4], 2
        jne    SHORT loc_1_0759
        lea    ecx, [esi + 15eh]
        inc    dword ptr [esi + 17eh]
        mov    eax, dword ptr [ecx]
        add    eax, edi
        mov    dword ptr [ecx], eax
        cmp    eax, edi
        jae    SHORT loc_1_079F
        inc    dword ptr [ecx + 4]
        jmp    SHORT loc_1_079F
loc_1_0759:
        lea    ecx, [esi + 156h]
        inc    dword ptr [esi + 17ah]
        mov    eax, dword ptr [ecx]
        add    eax, edi
        mov    dword ptr [ecx], eax
        cmp    eax, edi
        jae    SHORT loc_1_079F
        inc    dword ptr [ecx + 4]
        jmp    SHORT loc_1_079F
loc_1_0774:
        inc    dword ptr [esi + 136h]
        test   bh, 8
        je     SHORT loc_1_0787
        inc    dword ptr [esi + 186h]
        jmp    SHORT loc_1_079F
loc_1_0787:
        test   bh, 20h
        je     SHORT loc_1_0794
        inc    dword ptr [esi + 18eh]
        jmp    SHORT loc_1_079F
loc_1_0794:
        test   bh, 4
        je     SHORT loc_1_079F
        inc    dword ptr [esi + 1a2h]
loc_1_079F:
        mov    cx, word ptr [esi + 103h]
        mov    eax, ecx
        and    eax, 0ffffh
        shl    eax, 3
        add    eax, dword ptr [esi + 0fbh]
        mov    bx, word ptr [eax + 2]
        mov    dword ptr [ebp - 8], eax
        and    bx, 0ff00h
        test   bh, 80h
        je     NEAR PTR loc_1_05EB
loc_1_07CB:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_07E0:
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [ebp + 10h], esi
        je     NEAR PTR loc_1_08A9
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [ebp - 10h]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   edx
        call   sub_1_4C40
        cmp    dword ptr [ebp - 10h], esi
        je     NEAR PTR loc_1_08A9
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_4C64
        cmp    dword ptr [ebp + 10h], esi
        je     SHORT loc_1_08A4
        mov    edi, dword ptr [ebp + 0ch]
loc_1_082F:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_085B
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_4C88
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_08A4
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_4C64
        jmp    SHORT loc_1_089F
loc_1_085B:
        test   edi, edi
        je     SHORT loc_1_0878
        cmp    edi, dword ptr [ebp - 4]
        jbe    SHORT loc_1_0870
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_089F
loc_1_0870:
        add    dword ptr [ebp - 0ch], edi
        sub    dword ptr [ebp - 4], edi
        xor    edi, edi
loc_1_0878:
        mov    ebx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp - 4]
        sub    ebx, esi
        cmp    ebx, eax
        jb     SHORT loc_1_0886
        mov    ebx, eax
loc_1_0886:
        push   ebx
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    ecx, dword ptr [ebp + 14h]
        push   ecx
        add    esi, ebx
        call   sub_1_4CFC
        add    dword ptr [ebp + 14h], ebx
        add    dword ptr [ebp - 0ch], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_089F:
        cmp    dword ptr [ebp + 10h], esi
        ja     SHORT loc_1_082F
loc_1_08A4:
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], esi
loc_1_08A9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_08C0:
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [ebp + 0ch], esi
        je     NEAR PTR loc_1_098C
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [ebp - 14h]
        push   ecx
        mov    edx, dword ptr [ebp + 10h]
        push   esi
        push   edx
        call   sub_1_4C40
        cmp    dword ptr [ebp - 14h], esi
        je     NEAR PTR loc_1_098C
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_4C64
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 0ch], esi
        mov    dword ptr [ebp - 10h], ecx
        je     SHORT loc_1_0987
        mov    edi, dword ptr [ebp + 14h]
loc_1_0915:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_0941
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_4C88
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_0987
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_4C64
        jmp    SHORT loc_1_0982
loc_1_0941:
        test   edi, edi
        je     SHORT loc_1_095E
        cmp    dword ptr [ebp - 4], edi
        jae    SHORT loc_1_0956
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_0982
loc_1_0956:
        add    dword ptr [ebp - 0ch], edi
        sub    dword ptr [ebp - 4], edi
        xor    edi, edi
loc_1_095E:
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 4]
        sub    ebx, esi
        cmp    ebx, eax
        jb     SHORT loc_1_096C
        mov    ebx, eax
loc_1_096C:
        push   ebx
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        add    esi, ebx
        call   sub_1_4CFC
        add    dword ptr [ebp - 10h], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_0982:
        cmp    esi, dword ptr [ebp + 0ch]
        jb     SHORT loc_1_0915
loc_1_0987:
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], esi
loc_1_098C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_09A0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        inc    dword ptr [esi + 86h]
        cmp    byte ptr [esi + 126h], 0
        jne    SHORT loc_1_09F8
        cmp    byte ptr [eax + 40h], 0
        jne    SHORT loc_1_09F1
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ecx + 10h]
        test   edx, edx
        jl     SHORT loc_1_09D0
        cmp    edx, 1
        jle    SHORT loc_1_09D7
loc_1_09D0:
        mov    edi, 0c00000bbh
        jmp    SHORT loc_1_09FD
loc_1_09D7:
        push   ecx
        mov    edi, 103h
        push   esi
        inc    dword ptr [eax + 14h]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx], 0
        call   sub_1_0A60
        jmp    SHORT loc_1_09FD
loc_1_09F1:
        mov    edi, 0c0010002h
        jmp    SHORT loc_1_09FD
loc_1_09F8:
        mov    edi, 0c001000dh
loc_1_09FD:
        cmp    byte ptr [esi + 11h], 0
        jne    SHORT loc_1_0A4D
        cmp    dword ptr [esi + 86h], 2
        jne    SHORT loc_1_0A15
        cmp    byte ptr [esi + 126h], 0
        jne    SHORT loc_1_0A36
loc_1_0A15:
        cmp    dword ptr [esi + 119h], 0
        jne    SHORT loc_1_0A36
        cmp    dword ptr [esi + 10dh], 0
        jne    SHORT loc_1_0A36
        mov    eax, dword ptr [esi + 96h]
        sub    eax, esi
        cmp    eax, 96h
        je     SHORT loc_1_0A4D
loc_1_0A36:
        push   0ah
        add    esi, 15h
        push   esi
        mov    byte ptr [esi - 4], 1
        call   sub_1_4BF2
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_0A4D:
        mov    eax, edi
        dec    dword ptr [esi + 86h]
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_0A60:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    dword ptr [eax + 0b6h], 0
        je     SHORT loc_1_0A85
        mov    edx, dword ptr [eax + 0bah]
        mov    ecx, dword ptr [ebp + 0ch]
        pop    ebp
        mov    dword ptr [edx], ecx
        mov    dword ptr [eax + 0bah], ecx
        ret    8
loc_1_0A85:
        cmp    byte ptr [eax + 129h], 0
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 0b6h], ecx
        mov    dword ptr [eax + 0bah], ecx
        jne    SHORT loc_1_0AA3
        push   eax
        call   sub_1_0AB0
loc_1_0AA3:
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0AB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
loc_1_0ABC:
        mov    edi, dword ptr [esi + 0b6h]
        test   edi, edi
        je     NEAR PTR loc_1_0CA7
        cmp    byte ptr [esi + 126h], 0
        jne    NEAR PTR loc_1_0CB7
        mov    byte ptr [esi + 129h], 1
        mov    eax, dword ptr [edi + 10h]
        cmp    eax, 4
        ja     NEAR PTR loc_1_0C4C
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_0AF1:
        push   0
        lea    eax, [edi + 24h]
        push   eax
        lea    ecx, [edi + 20h]
        push   ecx
        mov    edx, dword ptr [edi + 1ch]
        push   edx
        mov    eax, dword ptr [edi + 18h]
        push   eax
        mov    ecx, dword ptr [edi + 14h]
        push   ecx
        mov    edx, dword ptr [edi + 4]
        push   edx
        push   esi
        call   sub_1_0D90
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_0C4C
loc_1_0B19:
        lea    eax, [edi + 24h]
        lea    ecx, [edi + 20h]
        push   eax
        mov    edx, dword ptr [edi + 1ch]
        push   ecx
        mov    eax, dword ptr [edi + 18h]
        push   edx
        mov    ecx, dword ptr [edi + 14h]
        push   eax
        mov    edx, dword ptr [edi + 4]
        push   ecx
        push   edx
        push   esi
        call   sub_1_11A0
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_0C4C
loc_1_0B3F:
        push   1
        lea    eax, [edi + 24h]
        push   eax
        lea    ecx, [edi + 20h]
        push   ecx
        mov    edx, dword ptr [edi + 1ch]
        push   edx
        mov    eax, dword ptr [edi + 18h]
        push   eax
        mov    ecx, dword ptr [edi + 14h]
        push   ecx
        mov    edx, dword ptr [edi + 4]
        push   edx
        push   esi
        call   sub_1_0D90
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_0C4C
loc_1_0B67:
        mov    ebx, dword ptr [edi + 4]
        lea    eax, [ebx + 0ch]
        mov    ecx, dword ptr [ebx + 10h]
        push   eax
        mov    edx, dword ptr [ebx + 8]
        push   ecx
        push   ebx
        mov    eax, dword ptr [edx + 0b2h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0B82:
        test   al, al
        jne    SHORT loc_1_0BA2
        push   0
        mov    eax, dword ptr [ebx + 10h]
        push   0c0000001h
        push   eax
        call   sub_1_4B4E
        push   0
        push   45h
        push   ebx
        call   sub_1_4C1C
        jmp    SHORT loc_1_0BD7
loc_1_0BA2:
        lea    eax, [esi + 8ah]
        push   0
        push   0
        mov    dword ptr [ebx], eax
        mov    ecx, dword ptr [esi + 8eh]
        mov    dword ptr [ebx + 4], ecx
        mov    ecx, dword ptr [esi + 8eh]
        mov    dword ptr [ecx], ebx
        mov    dword ptr [esi + 8eh], ebx
        inc    dword ptr [esi + 92h]
        inc    dword ptr [ebx + 14h]
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_4B4E
loc_1_0BD7:
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_0C4C
loc_1_0BE0:
        mov    ebx, dword ptr [edi + 4]
        push   0
        mov    ecx, dword ptr [esi + 0b2h]
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0BF5:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    SHORT loc_1_0C01
        dec    dword ptr [ebx + 14h]
        jmp    SHORT loc_1_0C17
loc_1_0C01:
        mov    eax, 103h
        cmp    dword ptr [ebp - 4], eax
        je     SHORT loc_1_0C17
        cmp    dword ptr [ebp - 4], 0c001000eh
        jne    SHORT loc_1_0C17
        mov    dword ptr [ebp - 4], eax
loc_1_0C17:
        mov    byte ptr [ebx + 40h], 1
        dec    dword ptr [ebx + 14h]
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx]
        mov    edx, dword ptr [ebx + 4]
        lea    ecx, [esi + 96h]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [ebx], ecx
        mov    edx, dword ptr [esi + 9ah]
        mov    dword ptr [ebx + 4], edx
        mov    edx, dword ptr [esi + 9ah]
        mov    dword ptr [edx], ebx
        mov    dword ptr [esi + 9ah], ebx
loc_1_0C4C:
        cmp    dword ptr [ebp - 4], 103h
        je     SHORT loc_1_0CBE
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0b6h], eax
        mov    eax, dword ptr [edi + 10h]
        test   eax, eax
        je     SHORT loc_1_0C8E
        cmp    eax, 1
        je     SHORT loc_1_0C8E
        cmp    eax, 2
        jne    NEAR PTR loc_1_0ABC
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0aeh]
        push   eax
        push   edi
        push   ecx
        call   sub_1_4B8A
        dec    dword ptr [esi + 86h]
        jmp    NEAR PTR loc_1_0ABC
loc_1_0C8E:
        mov    ebx, dword ptr [edi + 4]
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        mov    ecx, dword ptr [ebx + 10h]
        push   ecx
        call   sub_1_4B6C
        dec    dword ptr [ebx + 14h]
        jmp    NEAR PTR loc_1_0ABC
loc_1_0CA7:
        mov    byte ptr [esi + 129h], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0CB7:
        mov    byte ptr [esi + 129h], 0
loc_1_0CBE:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0CE0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 86h]
        cmp    byte ptr [esi + 126h], 0
        jne    SHORT loc_1_0D28
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 10h], 2
        je     SHORT loc_1_0D07
        mov    edi, 0c00000bbh
        jmp    SHORT loc_1_0D2D
loc_1_0D07:
        push   eax
        mov    edi, 103h
        mov    dword ptr [eax + 4], 0
        push   esi
        mov    dword ptr [eax], 0
        inc    dword ptr [esi + 86h]
        call   sub_1_0A60
        jmp    SHORT loc_1_0D2D
loc_1_0D28:
        mov    edi, 0c001000dh
loc_1_0D2D:
        cmp    byte ptr [esi + 11h], 0
        jne    SHORT loc_1_0D7D
        cmp    dword ptr [esi + 86h], 2
        jne    SHORT loc_1_0D45
        cmp    byte ptr [esi + 126h], 0
        jne    SHORT loc_1_0D66
loc_1_0D45:
        cmp    dword ptr [esi + 119h], 0
        jne    SHORT loc_1_0D66
        cmp    dword ptr [esi + 10dh], 0
        jne    SHORT loc_1_0D66
        mov    eax, dword ptr [esi + 96h]
        sub    eax, esi
        cmp    eax, 96h
        je     SHORT loc_1_0D7D
loc_1_0D66:
        push   0ah
        add    esi, 15h
        push   esi
        mov    byte ptr [esi - 4], 1
        call   sub_1_4BF2
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_0D7D:
        mov    eax, edi
        dec    dword ptr [esi + 86h]
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_0D90:
        push   ebp
        mov    cl, byte ptr [esp + 24h]
        mov    ebp, esp
        sub    esp, 18h
        test   cl, cl
        push   ebx
        push   esi
        push   edi
        je     SHORT loc_1_0DAF
        db  0C7h, 045h, 0F0h, 000h, 000h, 000h, 000h
        mov    edx, 34h
        jmp    SHORT loc_1_0DBB
loc_1_0DAF:
        db  0C7h, 045h, 0F0h, 000h, 000h, 000h, 000h
        mov    edx, 18h
loc_1_0DBB:
        xor    eax, eax
        test   edx, edx
        jle    SHORT loc_1_0DD5
        mov    esi, dword ptr [ebp - 10h]
        mov    ebx, dword ptr [ebp + 10h]
loc_1_0DC7:
        cmp    dword ptr [esi], ebx
        je     SHORT loc_1_0DD8
        add    esi, 4
        inc    eax
        cmp    edx, eax
        jg     SHORT loc_1_0DC7
        jmp    SHORT loc_1_0DD8
loc_1_0DD5:
        mov    ebx, dword ptr [ebp + 10h]
loc_1_0DD8:
        cmp    edx, eax
        jne    SHORT loc_1_0DF3
        mov    eax, dword ptr [ebp + 1ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 0
        mov    eax, 0c00000bbh
        mov    esp, ebp
        pop    ebp
        ret    20h
loc_1_0DF3:
        lea    esi, [ebp - 8]
        mov    edi, 4
        mov    eax, ebx
        and    eax, 0ffff0000h
        cmp    eax, 20000h
        ja     SHORT loc_1_0E1B
        je     NEAR PTR loc_1_0F6D
        cmp    eax, 10000h
        je     SHORT loc_1_0E36
        jmp    NEAR PTR loc_1_1103
loc_1_0E1B:
        cmp    eax, 1010000h
        je     NEAR PTR loc_1_0FEF
        cmp    eax, 1020000h
        je     NEAR PTR loc_1_10C3
        jmp    NEAR PTR loc_1_1103
loc_1_0E36:
        sub    ebx, 10101h
        cmp    ebx, 12h
        ja     NEAR PTR loc_1_1103
        db  0FFh, 024h, 09Dh, 000h, 000h, 000h, 000h
loc_1_0E4C:
        mov    esi, dword ptr [ebp - 10h]
        lea    edi, [edx*4]
        jmp    NEAR PTR loc_1_1103
loc_1_0E5B:
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_1103
loc_1_0E67:
        xor    eax, eax
        test   cl, cl
        mov    dword ptr [ebp - 8], eax
        je     NEAR PTR loc_1_1103
        mov    ebx, dword ptr [ebp + 8]
        cmp    dword ptr [ebx + 92h], eax
        jne    NEAR PTR loc_1_1103
        xor    edi, edi
        jmp    NEAR PTR loc_1_1103
loc_1_0E8A:
        mov    dword ptr [ebp - 8], 0d0h
        jmp    NEAR PTR loc_1_1103
loc_1_0E96:
        mov    dword ptr [ebp - 8], 5dch
        jmp    NEAR PTR loc_1_1103
loc_1_0EA2:
        mov    dword ptr [ebp - 8], 186a0h
        jmp    NEAR PTR loc_1_1103
loc_1_0EAE:
        mov    dword ptr [ebp - 8], 5ea0h
        jmp    NEAR PTR loc_1_1103
loc_1_0EBA:
        mov    dword ptr [ebp - 8], 0bdc0h
        jmp    NEAR PTR loc_1_1103
loc_1_0EC6:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_1_1103
loc_1_0ED2:
        mov    dword ptr [ebp - 8], 5eeh
        jmp    NEAR PTR loc_1_1103
loc_1_0EDE:
        push   3
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 3
        lea    eax, [ebp - 18h]
        push   ebx
        lea    esi, [ebp - 18h]
        push   eax
        mov    edi, 4
        call   sub_1_4CFC
        mov    byte ptr [ebp - 15h], 0
        jmp    NEAR PTR loc_1_1103
loc_1_0F01:
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, 1dh
        jmp    NEAR PTR loc_1_1103
loc_1_0F10:
        test   cl, cl
        je     SHORT loc_1_0F25
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0beh]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_1103
loc_1_0F25:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 0ch]
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [ebp - 8], edx
        jmp    NEAR PTR loc_1_1103
loc_1_0F36:
        mov    dword ptr [ebp - 8], 0d0h
        jmp    NEAR PTR loc_1_1103
loc_1_0F42:
        lea    esi, [ebp - 2]
        mov    edi, 2
        mov    word ptr [ebp - 2], 300h
        jmp    NEAR PTR loc_1_1103
loc_1_0F55:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_1_1103
loc_1_0F61:
        mov    dword ptr [ebp - 8], 0fh
        jmp    NEAR PTR loc_1_1103
loc_1_0F6D:
        test   cl, cl
        je     NEAR PTR loc_1_1103
        mov    ecx, ebx
        and    ebx, 0ff00h
        and    ecx, 0ffh
        dec    ecx
        cmp    ebx, 100h
        je     SHORT loc_1_0F99
        cmp    ebx, 200h
        je     SHORT loc_1_0FAB
        jmp    NEAR PTR loc_1_1103
loc_1_0F99:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + ecx*4 + 12ah]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_1103
loc_1_0FAB:
        mov    eax, ecx
        and    eax, 0fffffffeh
        cmp    eax, 0ch
        jae    SHORT loc_1_0FDD
        test   cl, 1
        mov    ebx, dword ptr [ebp + 8]
        je     SHORT loc_1_0FCC
        mov    eax, dword ptr [ebx + eax*2 + 16eh]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_1103
loc_1_0FCC:
        lea    esi, [ebx + eax*4 + 13eh]
        mov    edi, 8
        jmp    NEAR PTR loc_1_1103
loc_1_0FDD:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + ecx*4 + 156h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_1103
loc_1_0FEF:
        sub    ebx, 1010101h
        cmp    ebx, 3
        ja     NEAR PTR loc_1_1103
        db  0FFh, 024h, 09Dh, 000h, 000h, 000h, 000h
loc_1_1005:
        mov    esi, dword ptr [ebp + 8]
        mov    edi, 6
        add    esi, 3
        jmp    NEAR PTR loc_1_1103
loc_1_1015:
        mov    esi, dword ptr [ebp + 8]
        mov    edi, 6
        add    esi, 9
        jmp    NEAR PTR loc_1_1103
loc_1_1025:
        test   cl, cl
        je     SHORT loc_1_105D
        mov    eax, dword ptr [ebp + 14h]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 18h]
        push   ecx
        mov    ebx, dword ptr [ebp + 8]
        push   edx
        lea    ecx, [ebp - 14h]
        mov    eax, dword ptr [ebx + 0b2h]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_1048:
        mov    esi, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 0ch]
        lea    eax, [ecx + ecx*2]
        lea    edi, [eax*2]
        jmp    NEAR PTR loc_1_1103
loc_1_105D:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        lea    ecx, [ebp - 0ch]
        push   ecx
        mov    edx, dword ptr [ebp + 18h]
        push   edx
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebx + 0b2h]
        mov    ecx, dword ptr [eax + 0ch]
        push   ecx
        lea    eax, [ebp - 14h]
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_1083:
        cmp    dword ptr [ebp - 14h], 0
        mov    esi, dword ptr [ebp + 14h]
        jne    SHORT loc_1_109B
        mov    eax, dword ptr [ebp - 0ch]
        lea    ecx, [eax + eax*2]
        lea    edi, [ecx*2]
        jmp    SHORT loc_1_1103
loc_1_109B:
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebx + 0b2h]
        mov    ecx, dword ptr [eax + 0ch]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_10AE:
        lea    ecx, [eax + eax*2]
        lea    edi, [ecx*2]
        jmp    SHORT loc_1_1103
loc_1_10BA:
        mov    dword ptr [ebp - 8], 10h
        jmp    SHORT loc_1_1103
loc_1_10C3:
        test   cl, cl
        je     SHORT loc_1_1103
        mov    eax, ebx
        and    ebx, 0ff00h
        and    eax, 0ffh
        dec    eax
        cmp    ebx, 100h
        je     SHORT loc_1_10E7
        cmp    ebx, 200h
        je     SHORT loc_1_10F6
        jmp    SHORT loc_1_1103
loc_1_10E7:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + eax*4 + 18eh]
        mov    dword ptr [ebp - 8], eax
        jmp    SHORT loc_1_1103
loc_1_10F6:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + eax*4 + 19ah]
        mov    dword ptr [ebp - 8], eax
loc_1_1103:
        cmp    dword ptr [ebp + 18h], edi
        jae    SHORT loc_1_111B
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [eax], edi
        mov    eax, 0c0010016h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
loc_1_111B:
        push   edi
        mov    eax, dword ptr [ebp + 14h]
        push   esi
        push   eax
        call   sub_1_4CFC
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [eax], edi
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Bh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 005h, 000h
        db  000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_11A0:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        sub    esp, 4
        cmp    eax, 1010103h
        push   esi
        ja     SHORT loc_1_11D5
        je     NEAR PTR loc_1_12A6
        cmp    eax, 1010eh
        je     SHORT loc_1_11E1
        cmp    eax, 1010fh
        je     NEAR PTR loc_1_1255
        cmp    eax, 10112h
        je     NEAR PTR loc_1_1267
loc_1_11D5:
        mov    eax, 0c0010017h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_11E1:
        mov    esi, dword ptr [ebp + 18h]
        cmp    esi, 4
        je     SHORT loc_1_11F5
        mov    eax, 0c0010015h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_11F5:
        push   4
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_4CFC
        test   dword ptr [ebp - 4], 0f050h
        je     SHORT loc_1_122B
        mov    ecx, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [ebp + 20h]
        pop    esi
        mov    dword ptr [ecx], 4
        mov    dword ptr [eax], 0
        mov    eax, 0c00000bbh
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_122B:
        push   1
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp + 0ch]
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0ch]
        mov    ecx, dword ptr [eax + 0b2h]
        push   edx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_1249:
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_1255:
        mov    ecx, dword ptr [ebp + 1ch]
        xor    eax, eax
        pop    esi
        mov    dword ptr [ecx], 4
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_1267:
        cmp    dword ptr [ebp + 18h], 4
        je     SHORT loc_1_1282
        mov    eax, dword ptr [ebp + 20h]
        pop    esi
        mov    dword ptr [eax], 4
        mov    eax, 0c0010014h
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_1282:
        push   4
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 41h
        push   eax
        call   sub_1_4CFC
        mov    ecx, dword ptr [ebp + 1ch]
        xor    eax, eax
        pop    esi
        mov    dword ptr [ecx], 4
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_12A6:
        mov    ecx, 6
        mov    esi, dword ptr [ebp + 18h]
        mov    eax, esi
        cdq
        idiv   ecx
        test   edx, edx
        je     SHORT loc_1_12C3
        mov    eax, 0c0010015h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_12C3:
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    ecx, 6
        mov    eax, esi
        cdq
        idiv   ecx
        push   eax
        mov    edx, dword ptr [ebp + 0ch]
        push   0
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0ch]
        mov    edx, dword ptr [ecx + 0b2h]
        push   eax
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_12EC:
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_1300:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 8]
        cmp    byte ptr [ecx + 126h], 0
        jne    SHORT loc_1_1331
        cmp    byte ptr [ecx + 0fh], 0
        jne    SHORT loc_1_1331
        cmp    byte ptr [ecx + 10h], 0
        jne    SHORT loc_1_1331
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        push   eax
        push   edx
        push   ecx
        call   sub_1_1340
        pop    ebp
        ret    14h
loc_1_1331:
        mov    eax, 0c001000dh
        pop    ebp
        ret    14h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1340:
        push   ebp
        mov    ebp, esp
        sub    esp, 68h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 10h]
        mov    eax, dword ptr [esi + 105h]
        test   edi, 20h
        je     SHORT loc_1_1375
        mov    word ptr [eax], 8000h
        cmp    dword ptr [esi + 52h], 0
        je     SHORT loc_1_13E7
        mov    eax, dword ptr [esi + 105h]
        or     word ptr [eax], 1080h
        jmp    SHORT loc_1_13E7
loc_1_1375:
        mov    word ptr [eax], 0
        cmp    dword ptr [esi + 52h], 0
        je     SHORT loc_1_138B
        mov    eax, dword ptr [esi + 105h]
        or     word ptr [eax], 1080h
loc_1_138B:
        test   edi, 4
        je     SHORT loc_1_13B7
        xor    ax, ax
        mov    ecx, 0ffh
loc_1_139B:
        mov    edx, eax
        mov    ebx, dword ptr [esi + 105h]
        inc    ax
        and    edx, 0ffffh
        cmp    ax, 8
        mov    byte ptr [edx + ebx + 8], cl
        jb     SHORT loc_1_139B
        jmp    SHORT loc_1_13E7
loc_1_13B7:
        test   edi, 2
        je     SHORT loc_1_13E7
        lea    eax, [ebp - 68h]
        lea    ecx, [ebp - 4]
        push   eax
        mov    edx, dword ptr [esi + 0b2h]
        push   ecx
        lea    eax, [ebp - 8]
        push   60h
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_13D9:
        lea    ecx, [ebp - 68h]
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        push   esi
        call   sub_1_1480
loc_1_13E7:
        mov    byte ptr [esi + 126h], 1
        push   esi
        call   sub_1_3110
        xor    eax, eax
        mov    dword ptr [esi + 0beh], edi
        mov    byte ptr [esi + 126h], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h
loc_1_1410:
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [eax + 8]
        cmp    byte ptr [esi + 126h], 0
        jne    SHORT loc_1_1468
        cmp    byte ptr [esi + 0fh], 0
        jne    SHORT loc_1_1468
        cmp    byte ptr [esi + 10h], 0
        jne    SHORT loc_1_1468
        mov    eax, dword ptr [ebp + 10h]
        cmp    eax, 10h
        jbe    SHORT loc_1_1442
        mov    eax, 0c0010009h
        pop    esi
        pop    ebp
        ret    1ch
loc_1_1442:
        mov    ecx, dword ptr [ebp + 14h]
        push   ecx
        push   eax
        push   esi
        call   sub_1_1480
        mov    byte ptr [esi + 126h], 1
        push   esi
        call   sub_1_3110
        mov    byte ptr [esi + 126h], 0
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    1ch
loc_1_1468:
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    1ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1480:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        xor    eax, eax
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
loc_1_148D:
        mov    ecx, dword ptr [ebp + 8]
        inc    eax
        cmp    eax, 8
        mov    ebx, dword ptr [ecx + 105h]
        mov    byte ptr [ebx + eax + 7], dl
        jb     SHORT loc_1_148D
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, eax
        je     SHORT loc_1_14F0
        mov    esi, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 4], eax
loc_1_14AD:
        xor    ebx, ebx
        push   esi
        push   6
        call   sub_1_1500
        xor    ecx, ecx
loc_1_14B9:
        mov    dl, al
        shr    dl, cl
        and    dl, 1
        inc    ecx
        cmp    ecx, 6
        lea    ebx, [edx + ebx*2]
        jb     SHORT loc_1_14B9
        mov    al, bl
        xor    edx, edx
        shr    al, 3
        mov    ecx, dword ptr [ebp + 8]
        mov    dl, al
        mov    edi, dword ptr [ecx + 105h]
        mov    al, 1
        mov    cl, bl
        and    cl, 7
        add    esi, 6
        shl    al, cl
        or     byte ptr [edx + edi + 8], al
        dec    dword ptr [ebp - 4]
        jne    SHORT loc_1_14AD
loc_1_14F0:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1500:
        push   ebp
        mov    eax, 0ffffffffh
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        je     SHORT loc_1_153F
        mov    esi, dword ptr [ebp + 0ch]
loc_1_1514:
        mov    dl, byte ptr [esi]
        inc    esi
        mov    ecx, 8
loc_1_151C:
        mov    edi, eax
        mov    bl, dl
        shr    edi, 1fh
        add    eax, eax
        and    ebx, 1
        cmp    ebx, edi
        je     SHORT loc_1_1534
        xor    eax, 4c11db6h
        or     eax, 1
loc_1_1534:
        shr    dl, 1
        dec    ecx
        jne    SHORT loc_1_151C
        dec    dword ptr [ebp + 8]
        jne    SHORT loc_1_1514
loc_1_153F:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1550:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        dec    dword ptr [eax + 14h]
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_1560:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [esi + 86h]
        mov    ecx, dword ptr [esi + 48h]
        lea    edx, [ecx + 12h]
        out    dx, ax
        lea    edx, [ecx + 10h]
        in     ax, dx
        and    al, 33h
        cmp    al, 33h
        je     SHORT loc_1_15DD
        cmp    byte ptr [esi + 126h], 0
        jne    NEAR PTR loc_1_177C
        cmp    byte ptr [esi + 0fh], 0
        jne    SHORT loc_1_15DD
        cmp    byte ptr [esi + 10h], 0
        jne    SHORT loc_1_15DD
        mov    byte ptr [esi + 126h], 1
        mov    ecx, dword ptr [esi + 48h]
        xor    eax, eax
        push   esi
        lea    edx, [ecx + 12h]
        out    dx, ax
        lea    edx, [ecx + 10h]
        out    dx, ax
        call   sub_1_3110
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 15h]
        push   eax
        push   ecx
        call   sub_1_4BF8
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_15D6
        mov    byte ptr [esi + 11h], 0
loc_1_15D6:
        mov    byte ptr [esi + 126h], 0
loc_1_15DD:
        cmp    byte ptr [esi + 126h], 0
        jne    NEAR PTR loc_1_177C
        cmp    byte ptr [edi + 40h], 0
        jne    NEAR PTR loc_1_1775
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        push   eax
        inc    dword ptr [edi + 14h]
        push   0
        push   0
        push   0
        push   ecx
        call   sub_1_4C40
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_1763
        cmp    dword ptr [ebp - 8], 5eah
        ja     NEAR PTR loc_1_1763
        mov    ebx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ebx + 2ch], cx
        add    ebx, 24h
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebx + 4], edi
        push   eax
        push   edi
        push   edi
        call   sub_1_17F0
        cmp    byte ptr [ebx + 0ah], 8
        jne    NEAR PTR loc_1_16E9
        mov    byte ptr [esi + 14h], 1
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   esi
        call   sub_1_46D0
        lea    edx, [esi + 13eh]
        xor    ecx, ecx
        mov    byte ptr [esi + 14h], 0
        xor    eax, eax
        inc    dword ptr [esi + 12ah]
        inc    dword ptr [esi + 16eh]
        mov    cx, word ptr [ebx + 8]
        add    ecx, dword ptr [edx]
        mov    dword ptr [edx], ecx
        mov    ax, word ptr [ebx + 8]
        cmp    eax, ecx
        jbe    SHORT loc_1_1681
        inc    dword ptr [edx + 4]
loc_1_1681:
        dec    dword ptr [edi + 14h]
        cmp    byte ptr [esi + 11h], 0
        jne    SHORT loc_1_16D5
        cmp    dword ptr [esi + 86h], 2
        jne    SHORT loc_1_169C
        cmp    byte ptr [esi + 126h], 0
        jne    SHORT loc_1_16BD
loc_1_169C:
        cmp    dword ptr [esi + 119h], 0
        jne    SHORT loc_1_16BD
        cmp    dword ptr [esi + 10dh], 0
        jne    SHORT loc_1_16BD
        mov    eax, dword ptr [esi + 96h]
        sub    eax, esi
        cmp    eax, 96h
        je     SHORT loc_1_16D5
loc_1_16BD:
        mov    byte ptr [esi + 11h], 1
        push   esi
        call   sub_1_1D70
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_16D5:
        mov    eax, 103h
        pop    edi
        dec    dword ptr [esi + 86h]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_16E9:
        mov    byte ptr [esi + 13h], 1
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   esi
        call   sub_1_1D30
        mov    byte ptr [esi + 13h], 0
        dec    dword ptr [edi + 14h]
        cmp    byte ptr [esi + 11h], 0
        jne    SHORT loc_1_174F
        cmp    dword ptr [esi + 86h], 2
        jne    SHORT loc_1_1716
        cmp    byte ptr [esi + 126h], 0
        jne    SHORT loc_1_1737
loc_1_1716:
        cmp    dword ptr [esi + 119h], 0
        jne    SHORT loc_1_1737
        cmp    dword ptr [esi + 10dh], 0
        jne    SHORT loc_1_1737
        mov    eax, dword ptr [esi + 96h]
        sub    eax, esi
        cmp    eax, 96h
        je     SHORT loc_1_174F
loc_1_1737:
        mov    byte ptr [esi + 11h], 1
        push   esi
        call   sub_1_1D70
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_174F:
        mov    eax, 103h
        pop    edi
        dec    dword ptr [esi + 86h]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1763:
        mov    ebx, 0c001000fh
        mov    eax, dword ptr [edi + 14h]
        dec    eax
        mov    dword ptr [edi + 14h], eax
        dec    eax
        mov    dword ptr [edi + 14h], eax
        jmp    SHORT loc_1_1781
loc_1_1775:
        mov    ebx, 0c0010002h
        jmp    SHORT loc_1_1781
loc_1_177C:
        mov    ebx, 0c001000dh
loc_1_1781:
        cmp    byte ptr [esi + 11h], 0
        jne    SHORT loc_1_17D4
        cmp    dword ptr [esi + 86h], 2
        jne    SHORT loc_1_1799
        cmp    byte ptr [esi + 126h], 0
        jne    SHORT loc_1_17BA
loc_1_1799:
        cmp    dword ptr [esi + 119h], 0
        jne    SHORT loc_1_17BA
        cmp    dword ptr [esi + 10dh], 0
        jne    SHORT loc_1_17BA
        mov    eax, dword ptr [esi + 96h]
        sub    eax, esi
        cmp    eax, 96h
        je     SHORT loc_1_17D4
loc_1_17BA:
        push   0ah
        add    esi, 15h
        push   esi
        mov    byte ptr [esi - 4], 1
        call   sub_1_4BF2
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_17D4:
        mov    eax, ebx
        pop    edi
        dec    dword ptr [esi + 86h]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_17F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   esi
        lea    ecx, [ebp - 4]
        push   edi
        mov    ebx, dword ptr [eax + 8]
        mov    edi, dword ptr [ebp + 10h]
        push   ecx
        lea    eax, [ebp - 0ch]
        lea    esi, [edi + 24h]
        push   eax
        push   6
        mov    byte ptr [esi + 0ah], 0
        push   0
        push   edi
        call   sub_1_07E0
        mov    ecx, dword ptr [ebp + 0ch]
        test   byte ptr [ecx + 41h], 2
        jne    SHORT loc_1_183C
        lea    eax, [ebp - 0ch]
        mov    ecx, dword ptr [ebx + 0b2h]
        push   eax
        push   ecx
        call   sub_1_4A40
        test   al, al
        je     SHORT loc_1_183C
        or     byte ptr [esi + 0ah], 8
loc_1_183C:
        test   byte ptr [ebp - 0ch], 1
        je     SHORT loc_1_1871
        cmp    dword ptr [ebp - 0ah], -1
        jne    SHORT loc_1_1864
        mov    eax, 0ffh
        cmp    byte ptr [ebp - 0ch], al
        jne    SHORT loc_1_1864
        cmp    byte ptr [ebp - 0bh], al
        jne    SHORT loc_1_1864
        or     byte ptr [esi + 0ah], 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_1864:
        or     byte ptr [esi + 0ah], 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_1871:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 14h]
        or     byte ptr [esi + 0ah], 1
        push   eax
        push   ecx
        push   6
        push   6
        push   edi
        call   sub_1_07E0
        mov    ecx, dword ptr [ebp - 12h]
        cmp    dword ptr [ebp - 0ah], ecx
        jae    SHORT loc_1_1896
        mov    eax, 1
        jmp    SHORT loc_1_18C0
loc_1_1896:
        mov    eax, dword ptr [ebp - 12h]
        cmp    dword ptr [ebp - 0ah], eax
        jbe    SHORT loc_1_18A5
        mov    eax, 0ffffffffh
        jmp    SHORT loc_1_18C0
loc_1_18A5:
        mov    ax, word ptr [ebp - 14h]
        cmp    word ptr [ebp - 0ch], ax
        jae    SHORT loc_1_18B6
        mov    eax, 1
        jmp    SHORT loc_1_18C0
loc_1_18B6:
        mov    ax, word ptr [ebp - 0ch]
        cmp    word ptr [ebp - 14h], ax
        sbb    eax, eax
loc_1_18C0:
        test   eax, eax
        jne    SHORT loc_1_18C8
        mov    byte ptr [esi + 0ah], 8
loc_1_18C8:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_18E0:
        push   ebp
        mov    ecx, dword ptr [esp + 0ch]
        mov    ebp, esp
        add    ecx, 24h
        sub    esp, 1ch
        xor    edx, edx
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    dl, byte ptr [esi + 0deh]
        mov    eax, dword ptr [esi + 0d6h]
        mov    dword ptr [ebp - 14h], edx
        lea    edi, [eax + edx*8]
        mov    eax, dword ptr [esi + 0ceh]
        mov    dword ptr [ebp - 18h], edi
        lea    edi, [edx + edx*4]
        lea    edi, [edx + edi*4]
        lea    edi, [edi + edi*8]
        lea    edi, [edx + edi*4]
        mov    edx, dword ptr [ebp - 18h]
        lea    edi, [eax + edi*2]
        mov    ax, word ptr [edx + 2]
        and    ax, 0ff00h
        mov    dx, word ptr [edx + 6]
        mov    word ptr [ebp - 4], dx
        mov    dl, byte ptr [esi + 0deh]
        inc    dl
        mov    byte ptr [esi + 0deh], dl
        and    edx, 0ffh
        cmp    edx, dword ptr [esi + 0cah]
        jb     SHORT loc_1_1956
        mov    byte ptr [esi + 0deh], bl
loc_1_1956:
        mov    edx, dword ptr [ebp - 14h]
        test   ah, 44h
        mov    byte ptr [edx + esi + 0dfh], 0
        je     NEAR PTR loc_1_1A24
        inc    dword ptr [esi + 132h]
        test   ah, 4
        je     SHORT loc_1_197B
        inc    dword ptr [esi + 19ah]
loc_1_197B:
        test   ah, 40h
        je     NEAR PTR loc_1_1ACF
        mov    ax, word ptr [ebp - 4]
        and    ax, 4000h
        je     SHORT loc_1_1994
        inc    dword ptr [esi + 1a6h]
loc_1_1994:
        test   byte ptr [ebp - 3], 8
        je     SHORT loc_1_19A0
        inc    dword ptr [esi + 1aeh]
loc_1_19A0:
        test   byte ptr [ebp - 3], 10h
        je     SHORT loc_1_19AC
        inc    dword ptr [esi + 1b2h]
loc_1_19AC:
        test   byte ptr [ebp - 3], 4
        je     SHORT loc_1_19B8
        inc    dword ptr [esi + 19eh]
loc_1_19B8:
        test   ax, ax
        jne    SHORT loc_1_19C7
        test   byte ptr [ebp - 3], 80h
        je     NEAR PTR loc_1_1ACF
loc_1_19C7:
        cmp    byte ptr [esi + 126h], 0
        jne    SHORT loc_1_1A10
        cmp    byte ptr [esi + 0fh], 0
        jne    SHORT loc_1_1A10
        mov    ecx, dword ptr [esi + 48h]
        xor    eax, eax
        mov    byte ptr [esi + 126h], 1
        lea    edx, [ecx + 12h]
        push   esi
        out    dx, ax
        lea    edx, [ecx + 10h]
        out    dx, ax
        call   sub_1_3110
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 15h]
        push   eax
        push   ecx
        call   sub_1_4BF8
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_1A09
        mov    byte ptr [esi + 11h], 0
loc_1_1A09:
        mov    byte ptr [esi + 126h], 0
loc_1_1A10:
        mov    edx, dword ptr [ebp - 14h]
        pop    edi
        mov    byte ptr [edx + esi + 0dfh], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1A24:
        inc    dword ptr [esi + 12ah]
        test   ah, 8
        je     SHORT loc_1_1A35
        inc    dword ptr [esi + 192h]
loc_1_1A35:
        test   ah, 10h
        je     SHORT loc_1_1A40
        inc    dword ptr [esi + 196h]
loc_1_1A40:
        mov    al, byte ptr [ecx + 0ah]
        and    eax, 0f7h
        cmp    eax, 1
        je     SHORT loc_1_1A59
        cmp    eax, 2
        je     SHORT loc_1_1A7F
        cmp    eax, 4
        je     SHORT loc_1_1AA8
        jmp    SHORT loc_1_1ACF
loc_1_1A59:
        lea    eax, [esi + 13eh]
        xor    edx, edx
        inc    dword ptr [esi + 16eh]
        mov    dx, word ptr [ecx + 8]
        add    edx, dword ptr [eax]
        mov    dword ptr [eax], edx
        mov    cx, word ptr [ecx + 8]
        and    ecx, 0ffffh
        cmp    ecx, edx
        jbe    SHORT loc_1_1ACF
        jmp    SHORT loc_1_1ACC
loc_1_1A7F:
        lea    edx, [esi + 146h]
        xor    eax, eax
        inc    dword ptr [esi + 172h]
        mov    ax, word ptr [ecx + 8]
        add    eax, dword ptr [edx]
        mov    dword ptr [edx], eax
        mov    cx, word ptr [ecx + 8]
        and    ecx, 0ffffh
        cmp    ecx, eax
        jbe    SHORT loc_1_1ACF
        inc    dword ptr [edx + 4]
        jmp    SHORT loc_1_1ACF
loc_1_1AA8:
        lea    eax, [esi + 14eh]
        xor    edx, edx
        inc    dword ptr [esi + 176h]
        mov    dx, word ptr [ecx + 8]
        add    edx, dword ptr [eax]
        mov    dword ptr [eax], edx
        mov    cx, word ptr [ecx + 8]
        and    ecx, 0ffffh
        cmp    ecx, edx
        jbe    SHORT loc_1_1ACF
loc_1_1ACC:
        inc    dword ptr [eax + 4]
loc_1_1ACF:
        push   0
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [ebp + 0ch]
        push   0
        push   0
        push   ecx
        call   sub_1_4C40
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_1B20
loc_1_1AE8:
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 10h]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_4C64
        mov    ecx, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp - 10h]
        push   ecx
        push   edx
        push   edi
        call   sub_1_4CFC
        add    edi, dword ptr [ebp - 0ch]
        add    ebx, dword ptr [ebp - 0ch]
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_4C88
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_1AE8
loc_1_1B20:
        cmp    ebx, 3ch
        jge    SHORT loc_1_1B47
        mov    edx, 3ch
        xor    eax, eax
        sub    edx, ebx
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        mov    eax, dword ptr [ebp - 18h]
        mov    word ptr [eax + 4], 0ffc4h
        jmp    SHORT loc_1_1B51
loc_1_1B47:
        neg    bx
        mov    edx, dword ptr [ebp - 18h]
        mov    word ptr [edx + 4], bx
loc_1_1B51:
        mov    edx, dword ptr [ebp - 14h]
        cmp    byte ptr [edx + esi + 0dfh], 0
        jne    SHORT loc_1_1BAA
        mov    edx, dword ptr [ebp - 18h]
        mov    eax, dword ptr [ebp - 14h]
        and    word ptr [edx + 2], 0a3ffh
        mov    word ptr [edx + 6], 0
        or     byte ptr [edx + 3], 80h
        mov    byte ptr [eax + esi + 0dfh], 1
        pushfd
        cli
        mov    ecx, dword ptr [esi + 48h]
        xor    eax, eax
        lea    edi, [ecx + 12h]
        add    ecx, 10h
        mov    edx, edi
        out    dx, ax
        mov    edx, ecx
        in     ax, dx
        mov    si, ax
        mov    edx, edi
        and    si, 40h
        xor    eax, eax
        out    dx, ax
        or     si, 8
        mov    edx, ecx
        mov    ax, si
        out    dx, ax
        popfd
loc_1_1BAA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h
        db  024h, 008h, 08Bh, 0ECh, 056h, 057h, 08Bh, 070h, 048h, 033h, 0C0h, 090h, 08Dh, 07Eh, 012h, 083h
        db  0C6h, 010h, 08Bh, 0D7h, 066h, 0EFh, 08Bh, 0D6h, 066h, 0EDh, 066h, 08Bh, 0C8h, 08Bh, 0D7h, 066h
        db  083h, 0E1h, 040h, 033h, 0C0h, 066h, 0EFh, 080h, 0C9h, 008h, 08Bh, 0D6h, 08Bh, 0C1h, 05Fh, 025h
        db  0FFh, 0FFh, 000h, 000h, 05Eh, 066h, 0EFh, 0B0h, 001h, 05Dh, 0C2h, 004h, 000h
sub_1_1C00:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 10h], 0
        je     SHORT loc_1_1C1D
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1C1D:
        mov    ecx, dword ptr [esi + 48h]
        xor    eax, eax
        mov    byte ptr [esi + 10h], 1
        lea    edi, [ecx + 12h]
        add    ecx, 10h
        mov    edx, edi
        out    dx, ax
        mov    edx, ecx
        in     ax, dx
        and    al, 33h
        cmp    al, 33h
        je     SHORT loc_1_1C86
        cmp    byte ptr [esi + 126h], 0
        jne    NEAR PTR loc_1_1CE1
        cmp    byte ptr [esi + 0fh], 0
        jne    NEAR PTR loc_1_1CE1
        mov    edx, edi
        xor    eax, eax
        mov    byte ptr [esi + 126h], 1
        out    dx, ax
        mov    edx, ecx
        push   esi
        out    dx, ax
        call   sub_1_3110
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 15h]
        push   eax
        push   ecx
        call   sub_1_4BF8
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_1C7F
        mov    byte ptr [esi + 11h], 0
loc_1_1C7F:
        mov    byte ptr [esi + 126h], 0
loc_1_1C86:
        xor    eax, eax
        mov    ecx, dword ptr [esi + 0d6h]
        mov    al, byte ptr [esi + 0deh]
        test   byte ptr [ecx + eax*8 + 3], 80h
        jne    SHORT loc_1_1D14
        cmp    byte ptr [eax + esi + 0dfh], 0
        je     SHORT loc_1_1D14
        mov    edi, dword ptr [esi + 119h]
        lea    ebx, [edi + 24h]
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_1_1CBD
        mov    dword ptr [esi + 11dh], 0
loc_1_1CBD:
        mov    eax, dword ptr [ebx]
        push   edi
        push   esi
        mov    dword ptr [esi + 119h], eax
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [ebp - 8], ecx
        call   sub_1_18E0
        test   byte ptr [ebx + 0ah], 8
        je     SHORT loc_1_1CF0
        push   edi
        push   esi
        call   sub_1_46D0
        jmp    SHORT loc_1_1CFF
loc_1_1CE1:
        xor    al, al
        pop    edi
        mov    byte ptr [esi + 10h], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1CF0:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   edi
        mov    ecx, dword ptr [eax + 10h]
        push   ecx
        call   sub_1_4B5A
loc_1_1CFF:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        dec    dword ptr [eax + 14h]
        mov    al, 1
        mov    byte ptr [esi + 10h], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1D14:
        xor    al, al
        pop    edi
        mov    byte ptr [esi + 10h], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1D30:
        push   ebp
        mov    edx, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [edx + 28h]
        lea    esi, [edx + 24h]
        inc    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 119h], 0
        jne    SHORT loc_1_1D55
        mov    dword ptr [eax + 119h], edx
        jmp    SHORT loc_1_1D5E
loc_1_1D55:
        mov    ecx, dword ptr [eax + 11dh]
        mov    dword ptr [ecx + 24h], edx
loc_1_1D5E:
        mov    dword ptr [esi], 0
        pop    esi
        pop    ebp
        mov    dword ptr [eax + 11dh], edx
        ret    8
        db  090h
sub_1_1D70:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 126h], 0
        je     SHORT loc_1_1DA5
        mov    eax, dword ptr [esi + 86h]
        cmp    eax, 2
        jne    SHORT loc_1_1DA5
        dec    eax
        push   esi
        mov    byte ptr [esi + 11h], 0
        mov    dword ptr [esi + 86h], eax
        call   sub_1_3850
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_1DA5:
        xor    ebx, ebx
loc_1_1DA7:
        cmp    dword ptr [esi + 119h], ebx
        je     SHORT loc_1_1DC6
        cmp    byte ptr [esi + 126h], bl
        jne    SHORT loc_1_1DC6
        cmp    byte ptr [esi + 13h], bl
        jne    SHORT loc_1_1DC6
        push   esi
        call   sub_1_1C00
        test   al, al
        jne    SHORT loc_1_1DA7
loc_1_1DC6:
        xor    ebx, ebx
loc_1_1DC8:
        cmp    dword ptr [esi + 10dh], ebx
        je     SHORT loc_1_1DDF
        cmp    byte ptr [esi + 14h], bl
        jne    SHORT loc_1_1DDF
        push   esi
        call   sub_1_45B0
        test   al, al
        jne    SHORT loc_1_1DC8
loc_1_1DDF:
        mov    eax, dword ptr [esi + 96h]
        sub    eax, esi
        cmp    eax, 96h
        je     SHORT loc_1_1E32
        xor    edi, edi
loc_1_1DF0:
        mov    ecx, dword ptr [esi + 96h]
        cmp    dword ptr [ecx + 14h], edi
        jne    SHORT loc_1_1E23
        mov    ebx, dword ptr [ecx + 10h]
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [ecx + 4]
        push   edi
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 4]
        mov    edx, dword ptr [ecx]
        push   45h
        push   ecx
        mov    dword ptr [edx + 4], eax
        call   sub_1_4C1C
        push   edi
        dec    dword ptr [esi + 92h]
        push   ebx
        call   sub_1_4B54
loc_1_1E23:
        mov    eax, dword ptr [esi + 96h]
        sub    eax, esi
        cmp    eax, 96h
        jne    SHORT loc_1_1DF0
loc_1_1E32:
        cmp    dword ptr [esi + 10dh], 0
        jne    SHORT loc_1_1E64
        cmp    dword ptr [esi + 119h], 0
        jne    SHORT loc_1_1E64
        mov    eax, dword ptr [esi + 96h]
        sub    eax, esi
        cmp    eax, 96h
        jne    SHORT loc_1_1E64
        pop    edi
        dec    dword ptr [esi + 86h]
        mov    byte ptr [esi + 11h], 0
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_1E64:
        push   0ah
        add    esi, 15h
        push   esi
        call   sub_1_4BF2
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1E80:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 40h
        push   0
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_4B2A
        push   -1
        lea    ecx, [ebp - 8]
        push   0
        push   8
        push   ecx
        call   sub_1_4C16
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     SHORT loc_1_1EBC
        mov    eax, 0c000009ah
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1EBC:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    byte ptr [ebp - 40h], 3
        mov    byte ptr [ebp - 3fh], 0
        mov    dword ptr [ebp - 3eh], 0
        db  0C7h, 045h, 0C6h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0CAh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0CEh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0D2h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0D6h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0DAh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0DEh, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  0A1h, 004h, 000h, 000h, 000h
        push   36h
        mov    dword ptr [ebp - 12h], edx
        lea    ecx, [ebp - 40h]
        mov    dword ptr [ebp - 0eh], eax
        push   ecx
        mov    edx, dword ptr [ebp - 8]
        push   edx
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        lea    ecx, [ebp - 4]
        db  0C7h, 045h, 0E2h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0E6h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0EAh, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_1_4B36
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], edx
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_1F64
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1F64:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_4B30
        mov    eax, 0c0000001h
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_1_1F80:
        push   ebp
        mov    eax, 0ah
        mov    ebp, esp
        sub    esp, 0a4h
        mov    word ptr [ebp - 94h], ax
        push   esi
        mov    word ptr [ebp - 92h], ax
        push   edi
        mov    word ptr [ebp - 0a4h], ax
        xor    edi, edi
        mov    word ptr [ebp - 0a2h], ax
        mov    dword ptr [ebp - 4], edi
        db  0C7h, 085h, 070h, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
        db  0C7h, 085h, 060h, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
        db  0C7h, 085h, 068h, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
        db  0C7h, 085h, 078h, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
        mov    eax, 0bh
        mov    ecx, 5
        mov    word ptr [ebp - 9ch], ax
        db  0C7h, 045h, 0B0h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 9ah], ax
        mov    eax, 0ch
        mov    word ptr [ebp - 8ch], ax
        db  0C7h, 045h, 0E0h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 8ah], ax
        mov    eax, 3
        mov    word ptr [ebp - 54h], cx
        mov    word ptr [ebp - 52h], cx
        db  0C7h, 045h, 0C0h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 24h], ax
        mov    word ptr [ebp - 22h], ax
        db  0C7h, 045h, 0C8h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 44h], cx
        mov    word ptr [ebp - 42h], cx
        db  0C7h, 045h, 0D0h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 3ch], cx
        mov    word ptr [ebp - 3ah], cx
        db  0C7h, 045h, 0D8h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 34h], cx
        mov    eax, 9
        mov    word ptr [ebp - 32h], cx
        db  0C7h, 045h, 0B8h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 2ch], cx
        db  0C7h, 045h, 080h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 2ah], cx
        mov    word ptr [ebp - 4ch], ax
        db  0C7h, 045h, 088h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 4ah], ax
        mov    eax, 4
        mov    word ptr [ebp - 84h], ax
        mov    word ptr [ebp - 82h], ax
        db  0C7h, 045h, 090h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 7ch], cx
        mov    word ptr [ebp - 7ah], cx
        db  0C7h, 045h, 098h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 74h], cx
        mov    word ptr [ebp - 72h], cx
        mov    word ptr [ebp - 6ch], ax
        mov    word ptr [ebp - 6ah], ax
        mov    word ptr [ebp - 64h], cx
        mov    word ptr [ebp - 62h], cx
        mov    word ptr [ebp - 5ch], ax
        mov    esi, dword ptr [ebp + 0ch]
        mov    word ptr [ebp - 5ah], ax
        cmp    esi, edi
        db  0C7h, 045h, 0A0h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0A8h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_253E
        push   esi
        lea    eax, [ebp - 10h]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_4C94
        cmp    dword ptr [ebp - 4], edi
        je     SHORT loc_1_2105
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_2105:
        push   0
        lea    eax, [ebp - 0a4h]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_4C9A
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_2130
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 4]
        db  0A2h, 000h, 000h, 000h, 000h
loc_1_2130:
        push   0
        lea    eax, [ebp - 9ch]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_4C9A
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_215B
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 4]
        db  0A2h, 000h, 000h, 000h, 000h
loc_1_215B:
        push   1
        lea    eax, [ebp - 94h]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_4C9A
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_2187
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 4]
        db  089h, 00Dh, 000h, 000h, 000h, 000h
loc_1_2187:
        mov    edx, 0c80h
        in     eax, dx
        cmp    eax, 107110eh
        jne    SHORT loc_1_2199
        mov    edi, 8800h
loc_1_2199:
        push   edi
        call   sub_1_4380
        test   al, al
        jne    SHORT loc_1_21A5
        xor    edi, edi
loc_1_21A5:
        test   edi, edi
        je     SHORT loc_1_21AF
        db  089h, 03Dh, 000h, 000h, 000h, 000h
loc_1_21AF:
        push   2
        lea    eax, [ebp - 8ch]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_4C9A
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_22A8
        push   0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 4
        lea    ecx, [ebp - 84h]
        push   eax
        push   ecx
        call   sub_1_4CA6
        test   al, al
        je     SHORT loc_1_21FB
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_22B2
loc_1_21FB:
        push   0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 4
        lea    ecx, [ebp - 7ch]
        push   eax
        push   ecx
        call   sub_1_4CA6
        test   al, al
        je     SHORT loc_1_2220
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 005h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_22B2
loc_1_2220:
        push   0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 4
        lea    ecx, [ebp - 74h]
        push   eax
        push   ecx
        call   sub_1_4CA6
        test   al, al
        je     SHORT loc_1_2242
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 006h, 000h, 000h, 000h
        jmp    SHORT loc_1_22B2
loc_1_2242:
        push   0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 4
        lea    ecx, [ebp - 6ch]
        push   eax
        push   ecx
        call   sub_1_4CA6
        test   al, al
        je     SHORT loc_1_2264
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        jmp    SHORT loc_1_22B2
loc_1_2264:
        push   0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 4
        lea    ecx, [ebp - 64h]
        push   eax
        push   ecx
        call   sub_1_4CA6
        test   al, al
        je     SHORT loc_1_2286
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 003h, 000h, 000h, 000h
        jmp    SHORT loc_1_22B2
loc_1_2286:
        push   0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 4
        lea    ecx, [ebp - 5ch]
        push   eax
        push   ecx
        call   sub_1_4CA6
        test   al, al
        je     SHORT loc_1_22A8
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 000h
        jmp    SHORT loc_1_22B2
loc_1_22A8:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_22B2:
        push   1
        lea    eax, [ebp - 54h]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_4C9A
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_2320
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 4]
        db  066h, 089h, 00Dh, 000h, 000h, 000h, 000h
        test   cx, cx
        je     SHORT loc_1_2320
        db  066h, 083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        jne    SHORT loc_1_22F6
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_2329
loc_1_22F6:
        db  066h, 083h, 03Dh, 000h, 000h, 000h, 000h, 002h
        jne    SHORT loc_1_230B
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 003h, 000h
        jmp    SHORT loc_1_2329
loc_1_230B:
        db  066h, 083h, 03Dh, 000h, 000h, 000h, 000h, 003h
        jne    SHORT loc_1_2320
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        jmp    SHORT loc_1_2329
loc_1_2320:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 0FFh, 0FFh
loc_1_2329:
        push   0
        lea    eax, [ebp - 4ch]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_4C9A
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_23BA
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 4]
        movsx  eax, cx
        db  066h, 089h, 00Dh, 000h, 000h, 000h, 000h
        cmp    eax, 0dh
        ja     SHORT loc_1_23C3
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_2362:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 032h, 000h
        jmp    SHORT loc_1_23C3
loc_1_236D:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 046h, 000h
        jmp    SHORT loc_1_23C3
loc_1_2378:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 050h, 000h
        jmp    SHORT loc_1_23C3
loc_1_2383:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 05Ah, 000h
        jmp    SHORT loc_1_23C3
loc_1_238E:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 064h, 000h
        jmp    SHORT loc_1_23C3
loc_1_2399:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 06Eh, 000h
        jmp    SHORT loc_1_23C3
loc_1_23A4:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 078h, 000h
        jmp    SHORT loc_1_23C3
loc_1_23AF:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 082h, 000h
        jmp    SHORT loc_1_23C3
loc_1_23BA:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 03Ch, 000h
loc_1_23C3:
        push   1
        lea    eax, [ebp - 44h]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_4C9A
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_23F4
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 4]
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        cmp    ecx, 10000h
        jne    SHORT loc_1_23FE
loc_1_23F4:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
loc_1_23FE:
        push   1
        lea    eax, [ebp - 3ch]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_4C9A
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_242F
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 4]
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        cmp    ecx, 10000h
        jne    SHORT loc_1_2439
loc_1_242F:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
loc_1_2439:
        push   1
        lea    eax, [ebp - 34h]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_4C9A
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_246A
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 4]
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        cmp    ecx, 10000h
        jne    SHORT loc_1_2474
loc_1_246A:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
loc_1_2474:
        push   1
        lea    eax, [ebp - 2ch]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_4C9A
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_24A5
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 4]
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        cmp    ecx, 10000h
        jne    SHORT loc_1_24AF
loc_1_24A5:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
loc_1_24AF:
        push   0
        lea    eax, [ebp - 24h]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_4C9A
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_24D8
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 4]
        db  089h, 00Dh, 000h, 000h, 000h, 000h
loc_1_24D8:
        mov    eax, dword ptr [ebp - 10h]
        lea    ecx, [ebp - 14h]
        push   eax
        lea    edx, [ebp - 0ch]
        push   ecx
        lea    eax, [ebp - 4]
        push   edx
        push   eax
        call   sub_1_4B9C
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_252E
        cmp    dword ptr [ebp - 14h], 6
        jne    SHORT loc_1_252E
        mov    eax, dword ptr [ebp - 0ch]
        mov    cl, byte ptr [eax + 2]
        or     cl, byte ptr [eax + 3]
        or     cl, byte ptr [eax + 4]
        or     cl, byte ptr [eax + 5]
        or     cl, byte ptr [eax + 1]
        or     cl, byte ptr [eax]
        je     SHORT loc_1_252E
        xor    dl, dl
loc_1_2511:
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, dl
        inc    dl
        cmp    dl, 6
        mov    cl, byte ptr [ecx + eax]
        mov    byte ptr [ebp + eax - 1ch], cl
        jb     SHORT loc_1_2511
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 0ch], eax
        jmp    SHORT loc_1_2535
loc_1_252E:
        mov    dword ptr [ebp - 0ch], 0
loc_1_2535:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_4CA0
loc_1_253E:
        push   20h
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    ecx, dword ptr [ebp + 10h]
        push   ecx
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        push   esi
        push   edx
        call   sub_1_29F0
        mov    dword ptr [ebp - 4], eax
        xor    eax, eax
        pop    edi
        db  0A3h, 000h, 000h, 000h, 000h
        pop    esi
        db  0A3h, 000h, 000h, 000h, 000h
        db  0A2h, 000h, 000h, 000h, 000h
        db  0A2h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 005h, 000h, 000h, 000h, 000h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_25C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        xor    ebx, ebx
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   4
        push   eax
        push   ebx
        push   ebx
        push   esi
        call   sub_1_4BC6
        cmp    eax, 4
        jne    NEAR PTR loc_1_2794
        cmp    dword ptr [ebp - 4], 20001022h
        jne    NEAR PTR loc_1_2794
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    dword ptr [ebp - 4], ebx
        push   ebx
        push   ebx
        push   esi
        call   sub_1_4BCC
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   4
        push   ebx
        push   esi
        call   sub_1_4BC6
        cmp    eax, 4
        jne    NEAR PTR loc_1_2794
        test   byte ptr [ebp - 4], 1
        je     NEAR PTR loc_1_2794
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    dword ptr [ebp - 4], ebx
        push   ebx
        push   ebx
        push   esi
        call   sub_1_4BCC
        push   2
        lea    eax, [ebp - 4]
        push   eax
        push   10h
        push   ebx
        push   esi
        call   sub_1_4BC6
        cmp    eax, 2
        jne    NEAR PTR loc_1_2794
        mov    edi, dword ptr [ebp - 4]
        push   4
        and    edi, 0fffeh
        lea    eax, [ebp - 4]
        push   eax
        mov    dword ptr [ebp - 4], ebx
        push   ebx
        push   ebx
        push   esi
        call   sub_1_4BCC
        test   edi, edi
        je     NEAR PTR loc_1_2794
        push   edi
        call   sub_1_4380
        test   al, al
        je     NEAR PTR loc_1_2794
        push   4
        lea    eax, [ebp - 4]
        push   eax
        db  089h, 03Dh, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 022h, 010h, 000h, 020h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        push   3ch
        push   ebx
        push   esi
        call   sub_1_4BC6
        cmp    eax, 4
        jne    SHORT loc_1_26BA
        mov    al, byte ptr [ebp - 4]
        db  088h, 01Dh, 000h, 000h, 000h, 000h
        db  0A2h, 000h, 000h, 000h, 000h
loc_1_26BA:
        push   4
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        push   eax
        push   0
        push   0
        push   esi
        call   sub_1_4BCC
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   4
        push   0
        push   esi
        call   sub_1_4BC6
        cmp    eax, 4
        jne    SHORT loc_1_26F1
        mov    ebx, dword ptr [ebp - 4]
        or     ebx, 0ffff0000h
        jmp    SHORT loc_1_26F4
loc_1_26F1:
        mov    ebx, dword ptr [ebp - 4]
loc_1_26F4:
        xor    edi, edi
        push   4
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 4], edi
        push   eax
        push   edi
        push   edi
        push   esi
        call   sub_1_4BCC
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    dword ptr [ebp - 4], ebx
        push   4
        push   edi
        push   esi
        call   sub_1_4BCC
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    dword ptr [ebp - 4], edi
        push   edi
        push   edi
        push   esi
        call   sub_1_4BCC
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   4
        push   edi
        push   esi
        call   sub_1_4BC6
        cmp    eax, 4
        jne    SHORT loc_1_2747
        mov    ebx, dword ptr [ebp - 4]
        and    ebx, 0fffbh
loc_1_2747:
        xor    edi, edi
        push   4
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 4], edi
        push   eax
        push   edi
        push   edi
        push   esi
        call   sub_1_4BCC
        push   4
        mov    dword ptr [ebp - 4], ebx
        lea    ebx, [ebp - 4]
        push   ebx
        push   4
        push   edi
        push   esi
        call   sub_1_4BCC
        push   4
        mov    dword ptr [ebp - 4], edi
        push   ebx
        push   edi
        push   edi
        push   esi
        mov    esi, 27100h
        call   sub_1_4BCC
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        add    edx, 14h
        in     ax, dx
loc_1_278A:
        push   1
        call   sub_1_4C10
        dec    esi
        jne    SHORT loc_1_278A
loc_1_2794:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_27A0:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   esi
        push   edi
        jne    NEAR PTR loc_1_2895
        mov    esi, 200h
loc_1_27B4:
        push   esi
        call   sub_1_4440
        test   al, al
        je     SHORT loc_1_27D0
        lea    edx, [esi + 9]
        in     al, dx
        cmp    al, 1
        jne    SHORT loc_1_27D0
        push   esi
        call   sub_1_4380
        test   al, al
        jne    SHORT loc_1_27E2
loc_1_27D0:
        add    esi, 20h
        cmp    esi, 400h
        jb     SHORT loc_1_27B4
        db  0A1h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_27F1
loc_1_27E2:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        mov    eax, 2
loc_1_27F1:
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     NEAR PTR loc_1_2895
        lea    edi, [esi + 12h]
        mov    eax, 8
        mov    edx, edi
        db  089h, 035h, 000h, 000h, 000h, 000h
        add    esi, 16h
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        out    dx, ax
        mov    edx, esi
        in     ax, dx
        shr    ax, 4
        and    ax, 0fh
        test   cl, cl
        je     SHORT loc_1_2847
        and    ecx, 0ffh
        mov    edx, eax
        and    edx, 0ffffh
        cmp    ecx, edx
        je     SHORT loc_1_2847
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 008h, 000h, 000h, 000h
        jmp    SHORT loc_1_284C
loc_1_2847:
        db  0A2h, 000h, 000h, 000h, 000h
loc_1_284C:
        mov    edx, edi
        mov    eax, 8
        out    dx, ax
        mov    edx, esi
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        in     ax, dx
        and    ax, 7
        test   cl, cl
        je     SHORT loc_1_2890
        and    ecx, 0ffh
        mov    edx, eax
        and    edx, 0ffffh
        cmp    ecx, edx
        je     SHORT loc_1_2890
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 00Bh, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        ret
loc_1_2890:
        db  0A2h, 000h, 000h, 000h, 000h
loc_1_2895:
        pop    edi
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_28A0:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   esi
        push   edi
        jne    NEAR PTR loc_1_294F
        mov    si, 200h
loc_1_28B3:
        mov    edi, esi
        and    edi, 0ffffh
        push   edi
        call   sub_1_4440
        test   al, al
        je     SHORT loc_1_28D7
        lea    edx, [edi + 9]
        in     al, dx
        cmp    al, 10h
        jne    SHORT loc_1_28D7
        push   edi
        call   sub_1_4380
        test   al, al
        jne    SHORT loc_1_28E9
loc_1_28D7:
        add    si, 20h
        cmp    si, 400h
        jb     SHORT loc_1_28B3
        db  0A1h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_28F8
loc_1_28E9:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 010h, 000h, 000h, 000h
        mov    eax, 3
loc_1_28F8:
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_1_294F
        mov    edx, esi
        xor    al, al
        and    edx, 0ffffh
        db  0A2h, 000h, 000h, 000h, 000h
        db  089h, 015h, 000h, 000h, 000h, 000h
        add    edx, 8
        in     al, dx
        mov    dl, al
        db  0A0h, 000h, 000h, 000h, 000h
        and    edx, 0ffff00ffh
        test   al, al
        je     SHORT loc_1_2949
        and    eax, 0ffh
        mov    ecx, edx
        and    ecx, 0ffffh
        cmp    eax, ecx
        je     SHORT loc_1_2949
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 008h, 000h, 000h, 000h
        pop    edi
        pop    esi
        ret
loc_1_2949:
        db  088h, 015h, 000h, 000h, 000h, 000h
loc_1_294F:
        pop    edi
        pop    esi
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2960:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   esi
        push   edi
        jne    SHORT loc_1_29EA
        mov    si, 300h
loc_1_296F:
        mov    edi, esi
        and    edi, 0ffffh
        push   edi
        call   sub_1_4440
        test   al, al
        je     SHORT loc_1_299E
        push   edi
        call   sub_1_4380
        test   al, al
        je     SHORT loc_1_299E
        lea    edx, [edi + 9]
        in     al, dx
        and    ax, 0ffh
        test   ax, ax
        je     SHORT loc_1_29B0
        cmp    ax, 0ffh
        je     SHORT loc_1_29B0
loc_1_299E:
        add    si, 20h
        cmp    si, 360h
        jbe    SHORT loc_1_296F
        db  0A1h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_29B5
loc_1_29B0:
        mov    eax, 4
loc_1_29B5:
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_1_29EA
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_29CE
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 003h
loc_1_29CE:
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_29DE
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 005h
loc_1_29DE:
        mov    eax, esi
        and    eax, 0ffffh
        db  0A3h, 000h, 000h, 000h, 000h
loc_1_29EA:
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_1_29F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 30h
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        push   edi
        push   -1
        push   0
        push   286h
        push   eax
        call   sub_1_4C16
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    NEAR PTR loc_1_300F
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        mov    ecx, 0a1h
        rep stosd
        stosw
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 8]
        lea    edi, [ebp - 30h]
        xor    edx, edx
        mov    dword ptr [ecx + 0aah], eax
        xor    eax, eax
        mov    ecx, 9
        rep stosd
        stosw
        db  08Ah, 015h, 000h, 000h, 000h, 000h
        mov    eax, 1
        mov    dword ptr [ebp - 30h], edx
        mov    byte ptr [ebp - 2ch], al
        mov    dword ptr [ebp - 22h], eax
        mov    dword ptr [ebp - 26h], eax
        mov    dword ptr [ebp - 1eh], 600h
        db  039h, 005h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_2A6D
        mov    dword ptr [ebp - 26h], 5
loc_1_2A6D:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 003h
        jne    SHORT loc_1_2A7D
        mov    dword ptr [ebp - 26h], 2
loc_1_2A7D:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_2A97
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 4]
        push   edi
        push   eax
        call   sub_1_3D50
        mov    esi, eax
        jmp    SHORT loc_1_2AA0
loc_1_2A97:
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    edi, dword ptr [ebp + 0ch]
loc_1_2AA0:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 6ah
        cmp    dword ptr [eax], 1
        jne    SHORT loc_1_2AB2
        mov    dword ptr [ebp - 26h], 5
loc_1_2AB2:
        cmp    dword ptr [eax], 3
        jne    SHORT loc_1_2ABE
        mov    dword ptr [ebp - 26h], 2
loc_1_2ABE:
        cmp    esi, 9
        jb     SHORT loc_1_2ACC
        mov    dword ptr [ebp - 1ah], 0
        jmp    SHORT loc_1_2AED
loc_1_2ACC:
        xor    eax, eax
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  0A0h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 16h], ecx
        mov    dword ptr [ebp - 1ah], 1
        mov    dword ptr [ebp - 12h], 20h
        mov    dword ptr [ebp - 30h], eax
loc_1_2AED:
        lea    eax, [ebp - 30h]
        mov    ecx, dword ptr [ebp + 10h]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edi
        push   edx
        mov    eax, dword ptr [edx + 0aah]
        push   eax
        lea    eax, [edx + 0aeh]
        push   eax
        call   sub_1_4B42
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     SHORT loc_1_2B30
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   286h
        push   eax
        call   sub_1_4C1C
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_2B30:
        mov    eax, dword ptr [ebp - 4]
        cmp    esi, 9
        jb     SHORT loc_1_2B62
        mov    ecx, dword ptr [eax + 0aeh]
        push   ecx
        call   sub_1_4B48
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   286h
        push   ecx
        call   sub_1_4C1C
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_2B62:
        cmp    dword ptr [eax + 6ah], 1
        je     SHORT loc_1_2B70
        mov    edx, dword ptr [eax + 48h]
        add    edx, 14h
        in     ax, dx
loc_1_2B70:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0cah], 10h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0efh], 20h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   0
        test   al, al
        je     NEAR PTR loc_1_2FD9
        mov    eax, dword ptr [ebp + 14h]
        test   eax, eax
        jne    SHORT loc_1_2BA8
        mov    eax, dword ptr [ebp - 4]
        add    eax, 3
loc_1_2BA8:
        push   6
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 9
        push   eax
        call   sub_1_4CFC
        mov    edx, dword ptr [ebp - 4]
        xor    ecx, ecx
        mov    dword ptr [edx + 82h], ecx
        mov    edx, dword ptr [ebp - 4]
        mov    eax, dword ptr [edx + 105h]
        mov    word ptr [eax], cx
loc_1_2BCE:
        mov    esi, ecx
        mov    eax, dword ptr [ebp - 4]
        and    esi, 0ffh
        inc    cl
        mov    ebx, dword ptr [eax + 105h]
        cmp    cl, 6
        mov    dl, byte ptr [esi + eax + 9]
        mov    byte ptr [ebx + esi + 2], dl
        jb     SHORT loc_1_2BCE
        xor    ecx, ecx
        xor    edx, edx
loc_1_2BF2:
        mov    esi, ecx
        mov    eax, dword ptr [ebp - 4]
        and    esi, 0ffh
        inc    cl
        mov    ebx, dword ptr [eax + 105h]
        cmp    cl, 8
        mov    byte ptr [esi + ebx + 8], dl
        jb     SHORT loc_1_2BF2
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0ffh]
        mov    edx, dword ptr [eax + 105h]
        mov    word ptr [edx + 10h], cx
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0efh]
        shr    ecx, 1
        test   ecx, ecx
        je     SHORT loc_1_2C4A
        mov    edx, 2000h
loc_1_2C36:
        shr    ecx, 1
        mov    eax, dword ptr [ebp - 4]
        mov    ebx, dword ptr [eax + 105h]
        add    word ptr [ebx + 12h], dx
        test   ecx, ecx
        jne    SHORT loc_1_2C36
loc_1_2C4A:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 105h]
        mov    eax, dword ptr [eax + 0ffh]
        shr    eax, 10h
        and    ax, 0ffh
        or     word ptr [ecx + 12h], ax
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [edx + 0dah]
        mov    eax, dword ptr [edx + 105h]
        mov    word ptr [eax + 14h], cx
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [edx + 0cah]
        shr    ecx, 1
        test   ecx, ecx
        je     SHORT loc_1_2CA0
        mov    edx, 2000h
loc_1_2C8C:
        shr    ecx, 1
        mov    eax, dword ptr [ebp - 4]
        mov    ebx, dword ptr [eax + 105h]
        add    word ptr [ebx + 16h], dx
        test   ecx, ecx
        jne    SHORT loc_1_2C8C
loc_1_2CA0:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 105h]
        mov    eax, dword ptr [eax + 0dah]
        shr    eax, 10h
        and    ax, 0ffh
        or     word ptr [ecx + 16h], ax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8ah
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 8eh], eax
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 8eh]
        mov    dword ptr [eax + 8ah], edx
        mov    eax, dword ptr [ebp - 4]
        add    eax, 96h
        mov    ecx, dword ptr [ebp - 4]
        xor    ebx, ebx
        mov    dword ptr [ecx + 9ah], eax
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 9ah]
        mov    dword ptr [eax + 96h], edx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0b6h], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0bah], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0beh], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 92h], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 2], bl
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0fh], bl
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 10h], bl
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 12h], bl
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 11h], bl
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 13h], bl
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 14h], bl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 10dh], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 111h], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 115h], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 119h], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 11dh], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 125h], bl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 86h], 1
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 126h], bl
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 127h], bl
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 128h], bl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 121h], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 129h], bl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 12ah], ebx
        mov    dword ptr [eax + 12eh], ebx
        mov    dword ptr [eax + 132h], ebx
        mov    dword ptr [eax + 136h], ebx
        mov    dword ptr [eax + 13ah], ebx
        add    eax, 12ah
        mov    ecx, 0ch
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        add    edi, 13eh
        rep stosd
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, 6
        add    edi, 16eh
        rep stosd
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 186h], ebx
        mov    dword ptr [eax + 18ah], ebx
        add    eax, 186h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 18eh], ebx
        mov    dword ptr [eax + 192h], ebx
        mov    dword ptr [eax + 196h], ebx
        add    eax, 18eh
        mov    ecx, 7
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        add    edi, 19ah
        rep stosd
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0b2h
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 9eh
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 9
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   10h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_2E71:
        test   al, al
        jne    SHORT loc_1_2EAA
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0110
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0aeh]
        push   ecx
        call   sub_1_4B48
        push   ebx
        mov    ecx, dword ptr [ebp - 4]
        push   286h
        push   ecx
        call   sub_1_4C1C
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_2EAA:
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        add    edi, 2eh
        mov    ecx, 6
        rep stosd
        cmp    dword ptr [ebp - 26h], 1
        jne    SHORT loc_1_2ECC
        xor    eax, eax
        push   1
        db  0A0h, 000h, 000h, 000h, 000h
        push   0
        jmp    SHORT loc_1_2ED7
loc_1_2ECC:
        xor    eax, eax
        push   2
        db  0A0h, 000h, 000h, 000h, 000h
        push   1
loc_1_2ED7:
        push   eax
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        mov    eax, dword ptr [ecx + 0aeh]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        lea    eax, [ecx + 2eh]
        lea    ecx, [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_4BFE
        cmp    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp - 4]
        je     SHORT loc_1_2F44
        mov    ecx, dword ptr [eax + 0b2h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_2F10:
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        call   sub_1_0110
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 0aeh]
        push   edx
        call   sub_1_4B48
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   286h
        push   ecx
        call   sub_1_4C1C
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_2F44:
        mov    dword ptr [eax + 15h], 0
        mov    eax, dword ptr [ebp - 4]
        db  0C7h, 040h, 019h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 1dh], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_3020
        test   al, al
        jne    SHORT loc_1_2FB9
        mov    eax, dword ptr [ebp - 4]
        add    eax, 2eh
        push   eax
        call   sub_1_4C04
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 0b2h]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_2F83:
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        call   sub_1_0110
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 0aeh]
        push   edx
        call   sub_1_4B48
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   286h
        push   ecx
        call   sub_1_4C1C
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_2FB9:
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    ecx, dword ptr [eax + 0aeh]
        push   ecx
        call   sub_1_4CEE
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_2FD9:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0110
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0aeh]
        push   ecx
        call   sub_1_4B48
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   286h
        push   ecx
        call   sub_1_4C1C
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_300F:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h
sub_1_3020:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 6ah], 1
        je     NEAR PTR loc_1_30B3
        push   esi
        call   sub_1_3280
        mov    ecx, dword ptr [esi + 48h]
        mov    eax, 59h
        lea    ebx, [ecx + 12h]
        add    ecx, 10h
        mov    edx, ebx
        out    dx, ax
        mov    edx, ecx
        in     ax, dx
        mov    edi, eax
        mov    edx, ebx
        and    edi, 0ffffh
        mov    eax, 58h
        out    dx, ax
        shl    edi, 10h
        mov    edx, ecx
        in     ax, dx
        and    eax, 0ffffh
        lea    ecx, [eax + edi]
        mov    eax, ecx
        and    eax, 0fffffffh
        cmp    eax, 3003h
        jne    SHORT loc_1_3088
        shr    ecx, 1ch
        mov    byte ptr [esi], 1
        mov    byte ptr [esi + 1], cl
        jmp    SHORT loc_1_30B3
loc_1_3088:
        cmp    eax, 2260h
        jne    SHORT loc_1_309A
        shr    ecx, 1ch
        mov    byte ptr [esi], 2
        mov    byte ptr [esi + 1], cl
        jmp    SHORT loc_1_30B3
loc_1_309A:
        cmp    eax, 2261h
        jne    SHORT loc_1_30AC
        shr    ecx, 1ch
        mov    byte ptr [esi], 3
        mov    byte ptr [esi + 1], cl
        jmp    SHORT loc_1_30B3
loc_1_30AC:
        test   eax, eax
        jne    SHORT loc_1_30B3
        mov    byte ptr [esi], 0
loc_1_30B3:
        push   esi
        call   sub_1_3910
        push   esi
        call   sub_1_3280
        push   esi
        call   sub_1_3140
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_30D0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 48h]
        cmp    byte ptr [eax + 125h], 0
        lea    edx, [ecx + 12h]
        je     SHORT loc_1_30FA
        xor    eax, eax
        pop    ebp
        out    dx, ax
        lea    edx, [ecx + 10h]
        mov    eax, 3
        out    dx, ax
        mov    al, 1
        ret    4
loc_1_30FA:
        xor    eax, eax
        pop    ebp
        out    dx, ax
        lea    edx, [ecx + 10h]
        mov    eax, 43h
        out    dx, ax
        mov    al, 1
        ret    4
        db  08Bh, 0FFh
sub_1_3110:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_3280
        push   esi
        call   sub_1_3910
        pushfd
        cli
        push   esi
        call   sub_1_30D0
        popfd
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3140:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 48h]
        xor    eax, eax
        pop    ebp
        lea    edx, [ecx + 12h]
        out    dx, ax
        lea    edx, [ecx + 10h]
        mov    eax, 43h
        out    dx, ax
        ret    4
        db  090h
sub_1_3160:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        push   4
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   4
        push   0
        mov    ecx, dword ptr [esi + 0aeh]
        push   ecx
        call   sub_1_4BD2
        cmp    eax, 4
        jne    SHORT loc_1_31DF
        mov    ebx, dword ptr [ebp - 4]
        push   4
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [esi + 0aeh]
        push   eax
        and    ebx, 0fffbh
        mov    dword ptr [ebp - 4], 0
        push   0
        push   0
        push   ecx
        call   sub_1_4BD8
        push   4
        mov    eax, dword ptr [esi + 0aeh]
        mov    dword ptr [ebp - 4], ebx
        lea    ebx, [ebp - 4]
        push   ebx
        push   4
        push   0
        push   eax
        call   sub_1_4BD8
        push   4
        mov    eax, dword ptr [esi + 0aeh]
        mov    dword ptr [ebp - 4], 0
        push   ebx
        push   0
        push   0
        push   eax
        call   sub_1_4BD8
loc_1_31DF:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_31F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        push   4
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   4
        push   0
        mov    ecx, dword ptr [esi + 0aeh]
        push   ecx
        call   sub_1_4BD2
        cmp    eax, 4
        jne    SHORT loc_1_3272
        mov    ebx, dword ptr [ebp - 4]
        push   4
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [esi + 0aeh]
        push   eax
        or     ebx, 4
        push   0
        and    ebx, 0ffffh
        mov    dword ptr [ebp - 4], 0
        push   0
        push   ecx
        call   sub_1_4BD8
        push   4
        mov    eax, dword ptr [esi + 0aeh]
        mov    dword ptr [ebp - 4], ebx
        lea    ebx, [ebp - 4]
        push   ebx
        push   4
        push   0
        push   eax
        call   sub_1_4BD8
        push   4
        mov    eax, dword ptr [esi + 0aeh]
        mov    dword ptr [ebp - 4], 0
        push   ebx
        push   0
        push   0
        push   eax
        call   sub_1_4BD8
loc_1_3272:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_3280:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        push   edi
        jne    NEAR PTR loc_1_33EC
        mov    esi, dword ptr [ebp + 8]
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        cmp    dword ptr [esi + 6ah], 1
        jne    NEAR PTR loc_1_33BF
        mov    dword ptr [ebp - 0ch], 0
        mov    edi, 100h
loc_1_32B9:
        push   esi
        call   sub_1_3160
        mov    ecx, dword ptr [esi + 48h]
        xor    eax, eax
        lea    ebx, [ecx + 12h]
        add    ecx, 10h
        mov    edx, ebx
        out    dx, ax
        mov    edx, ecx
        mov    eax, 4
        out    dx, ax
        mov    edx, ebx
        mov    eax, 3
        out    dx, ax
        mov    edx, ecx
        mov    eax, 300h
        out    dx, ax
        mov    edx, ebx
        mov    eax, 2
        out    dx, ax
        mov    edx, dword ptr [esi + 109h]
        mov    dword ptr [ebp - 8], edx
        mov    edx, ecx
        shr    dword ptr [ebp - 8], 10h
        mov    al, byte ptr [ebp - 8]
        and    eax, 0ffff00ffh
        out    dx, ax
        mov    edx, ebx
        mov    eax, 1
        out    dx, ax
        mov    ax, word ptr [esi + 109h]
        mov    edx, ecx
        out    dx, ax
        mov    edx, ebx
        xor    eax, eax
        out    dx, ax
        mov    edx, ecx
        mov    eax, 0ff00h
        out    dx, ax
        mov    edx, ebx
        xor    eax, eax
        xor    bx, bx
        push   esi
        out    dx, ax
        mov    edx, ecx
        mov    eax, 1
        out    dx, ax
        call   sub_1_31F0
loc_1_3346:
        mov    ecx, dword ptr [esi + 48h]
        xor    eax, eax
        lea    edx, [ecx + 12h]
        out    dx, ax
        lea    edx, [ecx + 10h]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        test   dword ptr [ebp - 4], edi
        jne    SHORT loc_1_3370
        inc    bx
        push   1
        call   sub_1_4C10
        cmp    bx, 3e8h
        jb     SHORT loc_1_3346
        jmp    SHORT loc_1_3384
loc_1_3370:
        mov    ecx, dword ptr [esi + 48h]
        xor    eax, eax
        lea    edx, [ecx + 12h]
        out    dx, ax
        lea    edx, [ecx + 10h]
        mov    eax, 0ff00h
        out    dx, ax
loc_1_3384:
        test   dword ptr [ebp - 4], edi
        jne    SHORT loc_1_3398
        inc    dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 0ch], 9
        jb     NEAR PTR loc_1_32B9
        jmp    SHORT loc_1_33E2
loc_1_3398:
        mov    ecx, dword ptr [esi + 48h]
        xor    eax, eax
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        lea    edx, [ecx + 12h]
        pop    edi
        out    dx, ax
        lea    edx, [ecx + 10h]
        mov    eax, 4
        pop    esi
        pop    ebx
        mov    esp, ebp
        out    dx, ax
        pop    ebp
        ret    4
loc_1_33BF:
        mov    ecx, dword ptr [esi + 48h]
        xor    eax, eax
        mov    si, 5
        lea    edx, [ecx + 12h]
        out    dx, ax
        lea    edx, [ecx + 10h]
        mov    eax, 4
        out    dx, ax
loc_1_33D7:
        push   1
        call   sub_1_4C10
        dec    si
        jne    SHORT loc_1_33D7
loc_1_33E2:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_33EC:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3400:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 20h]
        cmp    byte ptr [esi + 2], 0
        je     SHORT loc_1_341E
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_341E:
        xor    edx, edx
        mov    eax, dword ptr [ebp + 18h]
        cmp    edx, eax
        je     SHORT loc_1_343B
        mov    ecx, dword ptr [ebp + 14h]
loc_1_342A:
        cmp    dword ptr [ecx], 0
        je     SHORT loc_1_3437
        add    ecx, 4
        inc    edx
        cmp    edx, eax
        jb     SHORT loc_1_342A
loc_1_3437:
        cmp    edx, eax
        jne    SHORT loc_1_3448
loc_1_343B:
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_3448:
        mov    eax, dword ptr [ebp + 10h]
        push   -1
        push   0
        lea    ecx, [ebp - 4]
        push   45h
        mov    dword ptr [eax], edx
        push   ecx
        inc    dword ptr [esi + 86h]
        call   sub_1_4C16
        test   eax, eax
        jne    SHORT loc_1_34D4
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx + 4], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], edx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 10h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 14h], 1
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 40h], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 8], esi
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 41h], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 28h], 3
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 1ch], ecx
        mov    dword ptr [eax + 18h], 0
        add    eax, 18h
        mov    edi, 103h
        mov    eax, dword ptr [ebp - 4]
        add    eax, 18h
        push   eax
        push   esi
        call   sub_1_0A60
        jmp    SHORT loc_1_34D9
loc_1_34D4:
        mov    edi, 0c000009ah
loc_1_34D9:
        cmp    byte ptr [esi + 11h], 0
        jne    SHORT loc_1_352B
        cmp    dword ptr [esi + 86h], 2
        jne    SHORT loc_1_34F1
        cmp    byte ptr [esi + 126h], 0
        jne    SHORT loc_1_3512
loc_1_34F1:
        cmp    dword ptr [esi + 119h], 0
        jne    SHORT loc_1_3512
        cmp    dword ptr [esi + 10dh], 0
        jne    SHORT loc_1_3512
        mov    eax, dword ptr [esi + 96h]
        sub    eax, esi
        cmp    eax, 96h
        je     SHORT loc_1_352B
loc_1_3512:
        push   0ah
        add    esi, 15h
        push   esi
        mov    byte ptr [esi - 4], 1
        call   sub_1_4BF2
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_352B:
        mov    eax, edi
        dec    dword ptr [esi + 86h]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  005h, 000h, 000h, 000h, 000h
loc_1_3540:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [esi + 86h]
        cmp    byte ptr [edi + 40h], 0
        jne    SHORT loc_1_357E
        mov    dword ptr [edi + 28h], 4
        mov    dword ptr [edi + 1ch], edi
        lea    eax, [edi + 18h]
        push   eax
        push   esi
        mov    dword ptr [eax], 0
        inc    dword ptr [edi + 14h]
        call   sub_1_0A60
        dec    dword ptr [edi + 14h]
        mov    edi, 103h
        jmp    SHORT loc_1_3583
loc_1_357E:
        mov    edi, 0c0010002h
loc_1_3583:
        cmp    byte ptr [esi + 11h], 0
        jne    SHORT loc_1_35D3
        cmp    dword ptr [esi + 86h], 2
        jne    SHORT loc_1_359B
        cmp    byte ptr [esi + 126h], 0
        jne    SHORT loc_1_35BC
loc_1_359B:
        cmp    dword ptr [esi + 119h], 0
        jne    SHORT loc_1_35BC
        cmp    dword ptr [esi + 10dh], 0
        jne    SHORT loc_1_35BC
        mov    eax, dword ptr [esi + 96h]
        sub    eax, esi
        cmp    eax, 96h
        je     SHORT loc_1_35D3
loc_1_35BC:
        push   0ah
        add    esi, 15h
        push   esi
        mov    byte ptr [esi - 4], 1
        call   sub_1_4BF2
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_35D3:
        mov    eax, edi
        dec    dword ptr [esi + 86h]
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_35F0:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_4B3C
        push   0
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_1_4B30
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3620:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, 27100h
        push   esi
        mov    byte ptr [esi + 2], 1
        call   sub_1_3280
        mov    edx, dword ptr [esi + 48h]
        add    edx, 14h
        in     ax, dx
loc_1_3642:
        push   1
        call   sub_1_4C10
        dec    edi
        jne    SHORT loc_1_3642
        mov    eax, dword ptr [esi + 0aeh]
        push   eax
        call   sub_1_4CF4
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 15h]
        push   eax
        push   ecx
        call   sub_1_4BF8
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_366F
        mov    byte ptr [esi + 11h], 0
loc_1_366F:
        lea    eax, [esi + 2eh]
        push   eax
        call   sub_1_4C04
        mov    eax, dword ptr [esi + 0b2h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_3684:
        push   esi
        call   sub_1_0110
        mov    eax, dword ptr [esi + 0aeh]
        push   eax
        call   sub_1_4B48
        xor    edx, edx
        db  0B9h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi + 48h]
loc_1_36A0:
        cmp    dword ptr [ecx], eax
        je     SHORT loc_1_36B2
        add    ecx, 4
        inc    edx
        db  081h, 0F9h, 008h, 000h, 000h, 000h
        jbe    SHORT loc_1_36A0
        jmp    SHORT loc_1_36E1
loc_1_36B2:
        db  0C7h, 004h, 095h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  08Ah, 082h, 000h, 000h, 000h, 000h
        cmp    byte ptr [esi + 4ch], al
        jne    SHORT loc_1_36CF
        db  0C6h, 082h, 000h, 000h, 000h, 000h, 000h
loc_1_36CF:
        db  08Ah, 082h, 000h, 000h, 000h, 000h
        cmp    byte ptr [esi + 4dh], al
        jne    SHORT loc_1_36E1
        db  0C6h, 082h, 000h, 000h, 000h, 000h, 000h
loc_1_36E1:
        push   0
        push   286h
        push   esi
        call   sub_1_4C1C
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3700:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 0
        mov    edi, dword ptr [esi + 8]
        cmp    byte ptr [edi + 2], 0
        je     SHORT loc_1_372A
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_372A:
        inc    dword ptr [edi + 86h]
        cmp    byte ptr [edi + 126h], 0
        jne    NEAR PTR loc_1_3835
        cmp    byte ptr [edi + 127h], 0
        jne    NEAR PTR loc_1_3835
        cmp    byte ptr [esi + 40h], 0
        jne    NEAR PTR loc_1_382C
        cmp    byte ptr [edi + 128h], 0
        jne    NEAR PTR loc_1_383C
        mov    byte ptr [edi + 127h], 1
        mov    eax, edi
        inc    dword ptr [esi + 14h]
        mov    ebx, dword ptr [edi + 8ah]
        sub    eax, ebx
        cmp    eax, 0ffffff76h
        je     SHORT loc_1_37AA
loc_1_377C:
        push   0
        inc    dword ptr [ebx + 14h]
        push   0
        mov    eax, dword ptr [ebx + 10h]
        push   40010004h
        push   eax
        call   sub_1_4B7E
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_4B84
        mov    eax, edi
        dec    dword ptr [ebx + 14h]
        mov    ebx, dword ptr [ebx]
        sub    eax, ebx
        cmp    eax, 0ffffff76h
        jne    SHORT loc_1_377C
loc_1_37AA:
        mov    byte ptr [edi + 127h], 0
        push   esi
        push   edi
        call   sub_1_3CD0
        push   edi
        call   sub_1_3850
        push   edi
        call   sub_1_3140
        mov    byte ptr [edi + 126h], 0
        mov    byte ptr [edi + 128h], 1
        cmp    dword ptr [edi + 0b6h], 0
        je     SHORT loc_1_37E1
        push   edi
        call   sub_1_0AB0
loc_1_37E1:
        mov    ebx, dword ptr [edi + 8ah]
        mov    eax, edi
        sub    eax, ebx
        cmp    eax, 0ffffff76h
        je     SHORT loc_1_3820
loc_1_37F2:
        push   0
        inc    dword ptr [ebx + 14h]
        push   0
        mov    eax, dword ptr [ebx + 10h]
        push   40010005h
        push   eax
        call   sub_1_4B7E
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_4B84
        mov    eax, edi
        dec    dword ptr [ebx + 14h]
        mov    ebx, dword ptr [ebx]
        sub    eax, ebx
        cmp    eax, 0ffffff76h
        jne    SHORT loc_1_37F2
loc_1_3820:
        mov    byte ptr [edi + 128h], 0
        dec    dword ptr [esi + 14h]
        jmp    SHORT loc_1_383C
loc_1_382C:
        mov    dword ptr [ebp - 4], 0c0010002h
        jmp    SHORT loc_1_383C
loc_1_3835:
        mov    dword ptr [ebp - 4], 0c001000dh
loc_1_383C:
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [edi + 86h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_3850:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_3280
        cmp    dword ptr [esi + 10dh], 0
        mov    word ptr [esi + 46h], 0
        je     SHORT loc_1_389C
loc_1_386D:
        mov    edx, dword ptr [esi + 10dh]
        push   0c001000ch
        push   edx
        lea    eax, [edx + 24h]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 10dh], ecx
        mov    edi, dword ptr [eax + 4]
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_4B5A
        dec    dword ptr [edi + 14h]
        cmp    dword ptr [esi + 10dh], 0
        jne    SHORT loc_1_386D
loc_1_389C:
        mov    dword ptr [esi + 10dh], 0
        mov    dword ptr [esi + 111h], 0
        cmp    dword ptr [esi + 119h], 0
        je     SHORT loc_1_38E8
loc_1_38B9:
        mov    edx, dword ptr [esi + 119h]
        push   0c001000ch
        push   edx
        lea    eax, [edx + 24h]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 119h], ecx
        mov    edi, dword ptr [eax + 4]
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_4B5A
        dec    dword ptr [edi + 14h]
        cmp    dword ptr [esi + 119h], 0
        jne    SHORT loc_1_38B9
loc_1_38E8:
        mov    dword ptr [esi + 119h], 0
        mov    dword ptr [esi + 11dh], 0
        push   esi
        call   sub_1_3910
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3910:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0d6h]
        mov    ecx, dword ptr [esi + 0d2h]
        cmp    dword ptr [esi + 6ah], 1
        jne    SHORT loc_1_3942
        mov    ebx, dword ptr [esi + 48h]
        mov    eax, 3ah
        lea    edx, [ebx + 12h]
        out    dx, ax
        lea    edx, [ebx + 10h]
        mov    eax, 200h
        out    dx, ax
loc_1_3942:
        xor    dl, dl
        cmp    dword ptr [esi + 0cah], 0
        jbe    SHORT loc_1_39A5
loc_1_394D:
        mov    al, dl
        add    edi, 8
        and    eax, 0ffff00ffh
        inc    dl
        mov    bx, ax
        and    eax, 0ffffh
        imul   bx, bx, 5eah
        add    bx, cx
        mov    word ptr [edi - 8], bx
        mov    ebx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ebx + eax*4]
        lea    eax, [eax + eax*8]
        lea    ebx, [ebx + eax*4]
        lea    eax, [ecx + ebx*2]
        shr    eax, 10h
        and    ax, 0ffh
        mov    word ptr [edi - 6], ax
        xor    eax, eax
        mov    word ptr [edi - 4], 0f000h
        mov    word ptr [edi - 2], 0
        or     byte ptr [edi - 5], 3
        mov    al, dl
        cmp    eax, dword ptr [esi + 0cah]
        jb     SHORT loc_1_394D
loc_1_39A5:
        xor    ecx, ecx
        mov    dword ptr [esi + 82h], ecx
        cmp    dword ptr [esi + 0cah], ecx
        jbe    SHORT loc_1_39D3
        mov    edx, 1
loc_1_39BA:
        xor    eax, eax
        xor    ebx, ebx
        mov    al, cl
        inc    cl
        mov    bl, cl
        mov    byte ptr [eax + esi + 0dfh], dl
        cmp    ebx, dword ptr [esi + 0cah]
        jb     SHORT loc_1_39BA
loc_1_39D3:
        mov    eax, dword ptr [esi + 0fbh]
        mov    edi, dword ptr [esi + 0f7h]
        mov    byte ptr [esi + 0deh], 0
        xor    dl, dl
        cmp    dword ptr [esi + 0efh], 0
        jbe    SHORT loc_1_3A4E
loc_1_39F1:
        mov    cl, dl
        add    eax, 8
        and    ecx, 0ffff00ffh
        inc    dl
        mov    bx, cx
        and    ecx, 0ffffh
        imul   bx, bx, 5eeh
        add    bx, di
        mov    word ptr [eax - 8], bx
        mov    ebx, ecx
        shl    ecx, 6
        sub    ecx, ebx
        lea    ebx, [ebx + ecx*4]
        lea    ecx, [ebx + ebx*2]
        lea    ecx, [edi + ecx*2]
        shr    ecx, 10h
        and    cx, 0ffh
        mov    word ptr [eax - 6], cx
        or     ch, 80h
        mov    word ptr [eax - 6], cx
        xor    ecx, ecx
        mov    word ptr [eax - 4], 0fa12h
        mov    word ptr [eax - 2], 0
        mov    cl, dl
        cmp    ecx, dword ptr [esi + 0efh]
        jb     SHORT loc_1_39F1
loc_1_3A4E:
        mov    word ptr [esi + 103h], 0
        mov    edi, dword ptr [esi + 6ah]
        cmp    edi, 1
        je     SHORT loc_1_3AA1
        cmp    edi, 3
        je     SHORT loc_1_3AA1
        mov    ecx, dword ptr [esi + 48h]
        mov    eax, 4
        lea    edx, [ecx + 12h]
        out    dx, ax
        lea    edx, [ecx + 10h]
        in     ax, dx
        mov    bx, ax
        lea    edx, [ecx + 12h]
        or     bh, 20h
        mov    eax, 4
        out    dx, ax
        mov    ax, bx
        lea    edx, [ecx + 10h]
        out    dx, ax
        lea    edx, [ecx + 12h]
        mov    eax, 52h
        out    dx, ax
        mov    ax, word ptr [esi + 68h]
        lea    edx, [ecx + 10h]
        out    dx, ax
loc_1_3AA1:
        cmp    dword ptr [esi + 56h], -1
        jne    SHORT loc_1_3AB8
        cmp    edi, 2
        je     SHORT loc_1_3AB8
        cmp    edi, 4
        je     SHORT loc_1_3AB8
        mov    dword ptr [esi + 56h], 0c0h
loc_1_3AB8:
        mov    ebx, dword ptr [esi + 56h]
        cmp    ebx, -1
        je     SHORT loc_1_3ADF
        cmp    edi, 2
        je     SHORT loc_1_3ADF
        cmp    edi, 4
        je     SHORT loc_1_3ADF
        mov    ecx, dword ptr [esi + 48h]
        mov    eax, 4
        lea    edx, [ecx + 12h]
        out    dx, ax
        mov    ax, bx
        lea    edx, [ecx + 16h]
        out    dx, ax
loc_1_3ADF:
        cmp    dword ptr [esi + 5ah], -1
        jne    SHORT loc_1_3AF6
        cmp    edi, 2
        je     SHORT loc_1_3AF6
        cmp    edi, 4
        je     SHORT loc_1_3AF6
        mov    dword ptr [esi + 5ah], 0b0h
loc_1_3AF6:
        mov    ebx, dword ptr [esi + 5ah]
        cmp    ebx, -1
        je     SHORT loc_1_3B13
        mov    ecx, dword ptr [esi + 48h]
        mov    eax, 5
        lea    edx, [ecx + 12h]
        out    dx, ax
        mov    ax, bx
        lea    edx, [ecx + 16h]
        out    dx, ax
loc_1_3B13:
        cmp    dword ptr [esi + 5eh], -1
        jne    SHORT loc_1_3B2A
        cmp    edi, 2
        je     SHORT loc_1_3B2A
        cmp    edi, 4
        je     SHORT loc_1_3B2A
        mov    dword ptr [esi + 5eh], 4088h
loc_1_3B2A:
        mov    ebx, dword ptr [esi + 5eh]
        cmp    ebx, -1
        je     SHORT loc_1_3B47
        mov    ecx, dword ptr [esi + 48h]
        mov    eax, 6
        lea    edx, [ecx + 12h]
        out    dx, ax
        mov    ax, bx
        lea    edx, [ecx + 16h]
        out    dx, ax
loc_1_3B47:
        cmp    dword ptr [esi + 62h], -1
        jne    SHORT loc_1_3B5E
        cmp    edi, 2
        je     SHORT loc_1_3B5E
        cmp    edi, 4
        je     SHORT loc_1_3B5E
        mov    dword ptr [esi + 62h], 81h
loc_1_3B5E:
        mov    ecx, dword ptr [esi + 62h]
        cmp    ecx, -1
        je     SHORT loc_1_3B7B
        mov    edi, dword ptr [esi + 48h]
        mov    eax, 7
        lea    edx, [edi + 12h]
        out    dx, ax
        mov    ax, cx
        lea    edx, [edi + 16h]
        out    dx, ax
loc_1_3B7B:
        mov    cl, byte ptr [esi]
        cmp    cl, 3
        jne    SHORT loc_1_3BCD
        mov    di, word ptr [esi + 66h]
        cmp    di, -1
        je     SHORT loc_1_3BA7
        mov    ebx, dword ptr [esi + 48h]
        mov    eax, 9
        cmp    di, -1
        lea    edx, [ebx + 12h]
        out    dx, ax
        mov    ax, di
        lea    edx, [ebx + 16h]
        out    dx, ax
        jne    SHORT loc_1_3BBD
loc_1_3BA7:
        mov    edi, dword ptr [esi + 48h]
        mov    eax, 9
        lea    edx, [edi + 12h]
        out    dx, ax
        lea    edx, [edi + 16h]
        in     ax, dx
        mov    word ptr [esi + 66h], ax
loc_1_3BBD:
        cmp    word ptr [esi + 66h], 3
        jne    SHORT loc_1_3BEC
        mov    dword ptr [esi + 52h], 0
        jmp    SHORT loc_1_3BEC
loc_1_3BCD:
        mov    bx, word ptr [esi + 66h]
        cmp    bx, -1
        je     SHORT loc_1_3BEC
        mov    edi, dword ptr [esi + 48h]
        mov    eax, 9
        lea    edx, [edi + 12h]
        out    dx, ax
        mov    ax, bx
        lea    edx, [edi + 16h]
        out    dx, ax
loc_1_3BEC:
        cmp    cl, 1
        jne    SHORT loc_1_3C22
        mov    ecx, dword ptr [esi + 48h]
        mov    eax, 2
        lea    edi, [ecx + 12h]
        add    ecx, 16h
        mov    edx, edi
        out    dx, ax
        mov    edx, ecx
        in     ax, dx
        mov    bx, ax
        mov    edx, edi
        and    bx, 0fffeh
        mov    eax, 2
        out    dx, ax
        or     bx, ax
        mov    edx, ecx
        mov    ax, bx
        out    dx, ax
loc_1_3C22:
        cmp    dword ptr [esi + 52h], 0
        je     SHORT loc_1_3C61
        mov    eax, dword ptr [esi + 105h]
        or     word ptr [eax], 1080h
        mov    eax, 2
        mov    ecx, dword ptr [esi + 48h]
        lea    edi, [ecx + 12h]
        add    ecx, 16h
        mov    edx, edi
        out    dx, ax
        mov    edx, ecx
        in     ax, dx
        mov    bx, ax
        mov    edx, edi
        mov    eax, 2
        out    dx, ax
        mov    ax, bx
        mov    edx, ecx
        and    eax, 0fffch
        out    dx, ax
loc_1_3C61:
        mov    ecx, dword ptr [esi + 48h]
        mov    eax, 3
        lea    edi, [ecx + 12h]
        add    ecx, 10h
        mov    edx, edi
        out    dx, ax
        mov    edx, ecx
        mov    eax, 300h
        out    dx, ax
        mov    edx, edi
        mov    eax, 2
        out    dx, ax
        mov    esi, dword ptr [esi + 109h]
        mov    edx, ecx
        mov    ebx, esi
        shr    ebx, 10h
        mov    al, bl
        and    eax, 0ffff00ffh
        out    dx, ax
        mov    edx, edi
        mov    eax, 1
        out    dx, ax
        mov    ax, si
        mov    edx, ecx
        out    dx, ax
        mov    edx, edi
        xor    eax, eax
        out    dx, ax
        mov    edx, ecx
        mov    eax, 0ff00h
        out    dx, ax
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3CD0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_3280
        lea    edi, [esi + 0b6h]
        mov    byte ptr [esi + 126h], 1
        mov    ecx, dword ptr [edi]
        test   ecx, ecx
        je     SHORT loc_1_3D3F
        cmp    dword ptr [ecx + 10h], 4
        jne    SHORT loc_1_3D04
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edi], eax
        mov    ecx, dword ptr [ecx + 4]
        dec    dword ptr [ecx + 14h]
        mov    ecx, dword ptr [edi]
loc_1_3D04:
        test   ecx, ecx
        je     SHORT loc_1_3D38
loc_1_3D08:
        mov    eax, dword ptr [ecx + 10h]
        cmp    eax, 4
        je     SHORT loc_1_3D30
        cmp    eax, 3
        je     SHORT loc_1_3D30
        mov    eax, dword ptr [ecx]
        push   0c001000ch
        push   ecx
        mov    dword ptr [edi], eax
        mov    ebx, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebx + 10h]
        push   ecx
        call   sub_1_4B6C
        dec    dword ptr [ebx + 14h]
        jmp    SHORT loc_1_3D32
loc_1_3D30:
        mov    edi, ecx
loc_1_3D32:
        mov    ecx, dword ptr [edi]
        test   ecx, ecx
        jne    SHORT loc_1_3D08
loc_1_3D38:
        mov    byte ptr [esi + 129h], 0
loc_1_3D3F:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3D50:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        push   esi
        push   edi
        je     NEAR PTR loc_1_41C9
        dec    eax
        cmp    eax, 5
        ja     SHORT loc_1_3D7E
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_3D7E:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_4440
        test   al, al
        jne    SHORT loc_1_3DAA
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_25C0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        je     SHORT loc_1_3DEC
        mov    eax, 12h
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_3DAA:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_4380
        test   al, al
        jne    SHORT loc_1_3DC4
        mov    eax, 9
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_3DC4:
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        add    edx, 9
        in     al, dx
        and    eax, 0ffh
        db  0A3h, 000h, 000h, 000h, 000h
        cmp    eax, 10h
        ja     SHORT loc_1_3E18
        je     NEAR PTR loc_1_3EF6
        test   eax, eax
        je     SHORT loc_1_3E28
        cmp    eax, 1
        je     SHORT loc_1_3E54
loc_1_3DEC:
        db  0A0h, 000h, 000h, 000h, 000h
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        push   eax
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        push   ecx
        push   edx
        call   sub_1_43B0
        test   al, al
        jne    NEAR PTR loc_1_42B2
        mov    eax, 0dh
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_3E18:
        cmp    eax, 11h
        je     NEAR PTR loc_1_3F4F
        cmp    eax, 0ffh
        jne    SHORT loc_1_3DEC
loc_1_3E28:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 000h
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_3E42
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 003h
loc_1_3E42:
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_3DEC
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 005h
        jmp    SHORT loc_1_3DEC
loc_1_3E54:
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    eax, 8
        add    esi, 12h
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    edx, esi
        add    edi, 16h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        out    dx, ax
        mov    edx, edi
        in     ax, dx
        shr    ax, 4
        and    ax, 0fh
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_3EAE
        xor    ecx, ecx
        mov    edx, eax
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        and    edx, 0ffffh
        cmp    ecx, edx
        je     SHORT loc_1_3EAE
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 008h, 000h, 000h, 000h
        jmp    SHORT loc_1_3EB3
loc_1_3EAE:
        db  0A2h, 000h, 000h, 000h, 000h
loc_1_3EB3:
        mov    edx, esi
        mov    eax, 8
        out    dx, ax
        mov    edx, edi
        in     ax, dx
        and    ax, 7
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_3EEC
        xor    ecx, ecx
        mov    edx, eax
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        and    edx, 0ffffh
        cmp    ecx, edx
        je     SHORT loc_1_3EEC
        mov    eax, 0bh
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_3EEC:
        db  0A2h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_3DEC
loc_1_3EF6:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 003h, 000h, 000h, 000h
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        add    edx, 8
        in     al, dx
        mov    dl, al
        and    edx, 0ffff00ffh
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_3F44
        xor    eax, eax
        mov    ecx, edx
        db  0A0h, 000h, 000h, 000h, 000h
        and    ecx, 0ffffh
        cmp    eax, ecx
        je     SHORT loc_1_3F44
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 008h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_3DEC
loc_1_3F44:
        db  088h, 015h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_3DEC
loc_1_3F4F:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        je     NEAR PTR loc_1_3DEC
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_25C0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        je     NEAR PTR loc_1_3DEC
        mov    eax, 12h
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_3F7D:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_25C0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        je     NEAR PTR loc_1_3DEC
        mov    eax, 12h
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_3F9E:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_4440
        test   al, al
        jne    SHORT loc_1_3FB8
        mov    eax, 14h
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_3FB8:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_4380
        test   al, al
        jne    SHORT loc_1_3FD2
        mov    eax, 9
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_3FD2:
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        add    edx, 9
        in     al, dx
        and    eax, 0ffh
        db  0A3h, 000h, 000h, 000h, 000h
        cmp    eax, 1
        jne    NEAR PTR loc_1_3DEC
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    eax, 8
        add    edi, 12h
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    edx, edi
        add    esi, 16h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        out    dx, ax
        mov    edx, esi
        in     ax, dx
        shr    ax, 4
        and    ax, 0fh
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_4049
        xor    ecx, ecx
        mov    edx, eax
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        and    edx, 0ffffh
        cmp    ecx, edx
        je     SHORT loc_1_4049
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 008h, 000h, 000h, 000h
        jmp    SHORT loc_1_404E
loc_1_4049:
        db  0A2h, 000h, 000h, 000h, 000h
loc_1_404E:
        mov    edx, edi
        mov    eax, 8
        out    dx, ax
        mov    edx, esi
        in     ax, dx
        and    ax, 7
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_4087
        xor    ecx, ecx
        mov    edx, eax
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        and    edx, 0ffffh
        cmp    ecx, edx
        je     SHORT loc_1_4087
        mov    eax, 0bh
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_4087:
        db  0A2h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_3DEC
loc_1_4091:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_4440
        test   al, al
        jne    SHORT loc_1_40AB
        mov    eax, 14h
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_40AB:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_4380
        test   al, al
        jne    SHORT loc_1_40C5
        mov    eax, 9
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_40C5:
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        add    edx, 9
        in     al, dx
        and    eax, 0ffh
        db  0A3h, 000h, 000h, 000h, 000h
        cmp    eax, 10h
        jne    NEAR PTR loc_1_3DEC
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 003h, 000h, 000h, 000h
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        add    edx, 8
        in     al, dx
        mov    dl, al
        and    edx, 0ffff00ffh
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_4130
        xor    eax, eax
        mov    ecx, edx
        db  0A0h, 000h, 000h, 000h, 000h
        and    ecx, 0ffffh
        cmp    eax, ecx
        je     SHORT loc_1_4130
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 008h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_3DEC
loc_1_4130:
        db  088h, 015h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_3DEC
loc_1_413B:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_4440
        test   al, al
        jne    SHORT loc_1_4155
        mov    eax, 14h
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_4155:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_4380
        test   al, al
        jne    SHORT loc_1_416F
        mov    eax, 9
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_416F:
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        add    edx, 9
        in     al, dx
        and    eax, 0ffh
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        jne    NEAR PTR loc_1_3DEC
        cmp    eax, 0ffh
        jne    NEAR PTR loc_1_3DEC
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 000h
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_41B0
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 003h
loc_1_41B0:
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_3DEC
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 005h
        jmp    NEAR PTR loc_1_3DEC
loc_1_41C9:
        dec    eax
        cmp    eax, 5
        ja     SHORT loc_1_41D6
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_41D6:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_25C0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        je     NEAR PTR loc_1_3DEC
        call   sub_1_27A0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 002h
        je     NEAR PTR loc_1_3DEC
        call   sub_1_28A0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 003h
        je     NEAR PTR loc_1_3DEC
        call   sub_1_2960
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 004h
        je     NEAR PTR loc_1_3DEC
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_3DEC
        mov    eax, 15h
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_423A:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_25C0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        je     NEAR PTR loc_1_3DEC
        mov    eax, 12h
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_425B:
        call   sub_1_27A0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 002h
        je     NEAR PTR loc_1_3DEC
        mov    eax, 16h
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_4278:
        call   sub_1_28A0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 003h
        je     NEAR PTR loc_1_3DEC
        mov    eax, 17h
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_4295:
        call   sub_1_2960
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 004h
        je     NEAR PTR loc_1_3DEC
        mov    eax, 18h
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_42B2:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 48h], eax
        db  08Ah, 015h, 000h, 000h, 000h, 000h
        mov    byte ptr [ecx + 4ch], dl
        db  0A0h, 000h, 000h, 000h, 000h
        mov    byte ptr [ecx + 4dh], al
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 4eh], esi
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 52h], edx
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 56h], eax
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 5ah], esi
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 5eh], edx
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 62h], eax
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 6ah], esi
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 6eh], edx
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    word ptr [ecx + 66h], ax
        db  066h, 08Bh, 035h, 000h, 000h, 000h, 000h
        mov    word ptr [ecx + 68h], si
        xor    esi, esi
loc_1_432B:
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        add    edx, esi
        inc    esi
        in     al, dx
        mov    byte ptr [ecx + esi + 2], al
        cmp    esi, 6
        jb     SHORT loc_1_432B
        db  0A1h, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 08Dh, 064h, 024h, 000h
sub_1_4380:
        push   ebp
        db  0B9h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
loc_1_438C:
        cmp    dword ptr [ecx], eax
        je     SHORT loc_1_43A1
        add    ecx, 4
        db  081h, 0F9h, 008h, 000h, 000h, 000h
        jbe    SHORT loc_1_438C
        mov    al, 1
        pop    ebp
        ret    4
loc_1_43A1:
        xor    al, al
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_43B0:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        xor    esi, esi
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    cl, byte ptr [ebp + 0ch]
        mov    dl, byte ptr [ebp + 10h]
loc_1_43CB:
        cmp    eax, 4
        je     SHORT loc_1_43DF
        cmp    eax, 2
        je     SHORT loc_1_43DF
        cmp    edi, 4
        je     SHORT loc_1_43DF
        cmp    edi, 2
        jne    SHORT loc_1_43E7
loc_1_43DF:
        db  038h, 08Eh, 000h, 000h, 000h, 000h
        je     SHORT loc_1_440B
loc_1_43E7:
        test   dl, dl
        je     SHORT loc_1_43F3
        db  038h, 096h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_4414
loc_1_43F3:
        db  039h, 01Ch, 0B5h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_441D
        inc    esi
        cmp    esi, 3
        jb     SHORT loc_1_43CB
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_1_440B:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_1_4414:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_1_441D:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        db  089h, 004h, 0B5h, 000h, 000h, 000h, 000h
        db  088h, 08Eh, 000h, 000h, 000h, 000h
        mov    al, 1
        db  088h, 096h, 000h, 000h, 000h, 000h
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h
sub_1_4440:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        xor    esi, esi
        xor    edi, edi
loc_1_444D:
        lea    edx, [ecx + edi]
        inc    edi
        in     al, dx
        and    ax, 0ffh
        add    si, ax
        cmp    edi, 0ch
        jb     SHORT loc_1_444D
        cmp    ecx, 8800h
        jne    SHORT loc_1_4471
        lea    edx, [ecx + 8]
        in     al, dx
        and    ax, 0ffh
        sub    si, ax
loc_1_4471:
        mov    edi, 0eh
loc_1_4476:
        lea    edx, [ecx + edi]
        in     al, dx
        mov    dl, al
        and    edx, 0ffff00ffh
        add    si, dx
        cmp    al, 57h
        jne    SHORT loc_1_44A7
        inc    edi
        cmp    edi, 10h
        jb     SHORT loc_1_4476
        lea    edx, [ecx + 0ch]
        pop    edi
        in     ax, dx
        sub    si, ax
        cmp    si, 1
        pop    esi
        sbb    cl, cl
        pop    ebp
        neg    cl
        mov    al, cl
        ret    4
loc_1_44A7:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  090h
loc_1_44B0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        mov    byte ptr [eax + 2], 1
        call   sub_1_3280
        push   3d090h
        call   sub_1_4C10
        pop    ebp
        ret    4
        db  090h
loc_1_44D0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [esi + 86h]
        cmp    byte ptr [esi + 126h], 0
        jne    SHORT loc_1_4545
        cmp    byte ptr [edi + 40h], 0
        jne    SHORT loc_1_453E
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        inc    dword ptr [edi + 14h]
        test   ecx, ecx
        jne    SHORT loc_1_4522
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [esi + 115h]
        add    eax, 0eh
        mov    edx, dword ptr [ebp + 14h]
        push   eax
        push   ecx
        push   edx
        mov    eax, dword ptr [ebp + 18h]
        push   0
        push   eax
        call   sub_1_4C8E
        dec    dword ptr [edi + 14h]
        xor    edi, edi
        jmp    SHORT loc_1_454A
loc_1_4522:
        push   0
        mov    edx, dword ptr [ebp + 18h]
        push   edx
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        add    eax, ecx
        push   eax
        call   sub_1_08C0
        dec    dword ptr [edi + 14h]
        xor    edi, edi
        jmp    SHORT loc_1_454A
loc_1_453E:
        mov    edi, 0c001000ch
        jmp    SHORT loc_1_454A
loc_1_4545:
        mov    edi, 0c001000dh
loc_1_454A:
        cmp    byte ptr [esi + 11h], 0
        jne    SHORT loc_1_459A
        cmp    dword ptr [esi + 86h], 2
        jne    SHORT loc_1_4562
        cmp    byte ptr [esi + 126h], 0
        jne    SHORT loc_1_4583
loc_1_4562:
        cmp    dword ptr [esi + 119h], 0
        jne    SHORT loc_1_4583
        cmp    dword ptr [esi + 10dh], 0
        jne    SHORT loc_1_4583
        mov    eax, dword ptr [esi + 96h]
        sub    eax, esi
        cmp    eax, 96h
        je     SHORT loc_1_459A
loc_1_4583:
        push   0ah
        add    esi, 15h
        push   esi
        mov    byte ptr [esi - 4], 1
        call   sub_1_4BF2
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    18h
loc_1_459A:
        mov    eax, edi
        dec    dword ptr [esi + 86h]
        pop    edi
        pop    esi
        pop    ebp
        ret    18h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_45B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 12h], 0
        je     SHORT loc_1_45CD
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_45CD:
        mov    edi, dword ptr [esi + 10dh]
        mov    byte ptr [esi + 12h], 1
        mov    ecx, dword ptr [edi + 28h]
        lea    eax, [edi + 24h]
        mov    dword ptr [ebp - 0ch], ecx
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_45EF
        mov    dword ptr [esi + 111h], 0
loc_1_45EF:
        mov    eax, dword ptr [eax]
        lea    ecx, [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 14h]
        push   edx
        mov    dword ptr [esi + 10dh], eax
        push   0
        mov    dword ptr [esi + 115h], edi
        push   0
        push   edi
        call   sub_1_4C40
        lea    ecx, [ebp - 4]
        lea    edx, [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebp - 14h]
        push   edx
        push   eax
        call   sub_1_4C64
        cmp    dword ptr [ebp - 4], 0eh
        jae    SHORT loc_1_463D
        cmp    dword ptr [ebp - 4], 6
        jb     SHORT loc_1_4693
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0
        mov    ecx, dword ptr [ebp - 8]
        push   0
        push   eax
        push   ecx
        push   ecx
        jmp    SHORT loc_1_4685
loc_1_463D:
        cmp    dword ptr [ebp - 4], 0deh
        jae    SHORT loc_1_4668
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp - 4], eax
        je     SHORT loc_1_4668
        lea    ebx, [esi + 1b6h]
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        push   0d0h
        push   0eh
        push   edi
        call   sub_1_07E0
        jmp    SHORT loc_1_4672
loc_1_4668:
        sub    dword ptr [ebp - 4], 0eh
        mov    ebx, dword ptr [ebp - 8]
        add    ebx, 0eh
loc_1_4672:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 4]
        sub    eax, 0eh
        mov    edx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        push   ebx
        push   0eh
        push   edx
        push   edx
loc_1_4685:
        push   0
        mov    eax, dword ptr [esi + 0b2h]
        push   eax
        call   sub_1_47C0
loc_1_4693:
        push   0
        mov    ecx, dword ptr [ebp - 0ch]
        push   edi
        mov    eax, dword ptr [ecx + 10h]
        push   eax
        call   sub_1_4B5A
        cmp    dword ptr [esi + 10dh], 0
        jne    SHORT loc_1_46B7
        mov    eax, dword ptr [esi + 0b2h]
        push   eax
        call   sub_1_49A0
loc_1_46B7:
        mov    ecx, dword ptr [ebp - 0ch]
        pop    edi
        dec    dword ptr [ecx + 14h]
        mov    byte ptr [esi + 12h], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_46D0:
        push   ebp
        mov    edx, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [edx + 28h]
        lea    esi, [edx + 24h]
        inc    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 10dh], 0
        jne    SHORT loc_1_46F5
        mov    dword ptr [eax + 10dh], edx
        jmp    SHORT loc_1_46FE
loc_1_46F5:
        mov    ecx, dword ptr [eax + 111h]
        mov    dword ptr [ecx + 24h], edx
loc_1_46FE:
        mov    dword ptr [esi], 0
        pop    esi
        pop    ebp
        mov    dword ptr [eax + 111h], edx
        ret    8
        db  090h
loc_1_4710:
        db  0A0h, 0A8h, 04Dh, 000h, 000h
        cmp    al, 0
        jne    SHORT loc_1_4735
        db  0FEh, 005h, 0A8h, 04Dh, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     SHORT loc_1_4737
        push   0
        push   0
        call   sub_1_1E80
        cmp    eax, 0
        jne    SHORT loc_1_4737
loc_1_4735:
        clc
        ret
loc_1_4737:
        stc
        ret
PCNTN3_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_4748
        db  0FFh, 024h, 085h, 04Ah, 047h, 000h, 000h
loc_1_4748:
        clc
        ret
        db  010h, 047h, 000h, 000h, 048h, 047h, 000h, 000h, 048h, 047h, 000h, 000h, 048h, 047h, 000h, 000h
        db  048h, 047h, 000h, 000h, 048h, 047h, 000h, 000h, 048h, 047h, 000h, 000h, 048h, 047h, 000h, 000h
        db  048h, 047h, 000h, 000h, 048h, 047h, 000h, 000h, 048h, 047h, 000h, 000h, 048h, 047h, 000h, 000h
        db  048h, 047h, 000h, 000h, 048h, 047h, 000h, 000h, 048h, 047h, 000h, 000h, 048h, 047h, 000h, 000h
        db  048h, 047h, 000h, 000h, 048h, 047h, 000h, 000h, 048h, 047h, 000h, 000h, 048h, 047h, 000h, 000h
        db  048h, 047h, 000h, 000h, 048h, 047h, 000h, 000h, 048h, 047h, 000h, 000h, 048h, 047h, 000h, 000h
        db  048h, 047h, 000h, 000h, 048h, 047h, 000h, 000h, 010h, 047h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_1_47C0:
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
        call   sub_1_47F0
        pop    ebp
        ret    20h
        db  08Dh, 064h, 024h, 000h
sub_1_47F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 18h], 0eh
        jb     NEAR PTR loc_1_4890
        mov    edx, dword ptr [ebp + 10h]
        mov    al, byte ptr [edx]
        test   al, 1
        je     SHORT loc_1_4830
        cmp    dword ptr [edx + 2], -1
        jne    SHORT loc_1_4825
        cmp    al, 0ffh
        jne    SHORT loc_1_4825
        cmp    byte ptr [edx + 1], 0ffh
        jne    SHORT loc_1_4825
        mov    dword ptr [ebp - 4], 8
        jmp    SHORT loc_1_4897
loc_1_4825:
        mov    dword ptr [ebp - 4], 2
        jmp    SHORT loc_1_4897
        db  08Bh, 0FFh
loc_1_4830:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0ch], 20h
        je     SHORT loc_1_4880
        mov    edx, dword ptr [ebp + 10h]
        lea    ecx, [esi + 2ch]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_4856
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_485D
loc_1_4856:
        mov    dword ptr [ebp - 10h], 1
loc_1_485D:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_4870
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_489A
        db  08Dh, 064h, 024h, 000h
loc_1_4870:
        mov    dword ptr [ebp - 4], 20h
        jmp    SHORT loc_1_489A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4880:
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_489A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4890:
        mov    dword ptr [ebp - 4], 20h
loc_1_4897:
        mov    esi, dword ptr [ebp + 8]
loc_1_489A:
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     NEAR PTR loc_1_4991
loc_1_48A5:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, ebx
        and    eax, dword ptr [ebp - 4]
        test   al, 9
        jne    SHORT loc_1_48FD
        test   al, 2
        je     SHORT loc_1_48E9
        lea    eax, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 0F6h, 0BEh, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_1_48E9
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 19h]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        test   dword ptr [eax + ecx*4], edx
        jne    SHORT loc_1_48FD
loc_1_48E9:
        test   byte ptr [ebp - 4], 2
        je     SHORT loc_1_48F4
        test   bl, 4
        jne    SHORT loc_1_48FD
loc_1_48F4:
        test   bl, 20h
        je     NEAR PTR loc_1_4986
loc_1_48FD:
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
        call   sub_1_4B72
        mov    byte ptr [edi + 18h], 1
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_4986
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_493F
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_493F:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_4950
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_4956
        db  08Bh, 0FFh
loc_1_4950:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_4956:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_4965
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_4B78
loc_1_4965:
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
        call   sub_1_4C1C
        mov    edi, ebx
        jmp    SHORT loc_1_4989
loc_1_4986:
        mov    edi, dword ptr [edi + 10h]
loc_1_4989:
        or     edi, edi
        jne    NEAR PTR loc_1_48A5
loc_1_4991:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_49A0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_49B0
        pop    ebp
        ret    4
sub_1_49B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     SHORT loc_1_4A2E
loc_1_49C0:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_4A27
        mov    byte ptr [edi + 18h], 0
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_4B78
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_4A27
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_49EC
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_49EC:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_4A00
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_4A06
        db  005h, 000h, 000h, 000h, 000h
loc_1_4A00:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_4A06:
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
        call   sub_1_4C1C
        mov    edi, ebx
        jmp    SHORT loc_1_4A2A
loc_1_4A27:
        mov    edi, dword ptr [edi + 10h]
loc_1_4A2A:
        or     edi, edi
        jne    SHORT loc_1_49C0
loc_1_4A2E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4A40:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0ch]
        test   al, 20h
        je     SHORT loc_1_4A55
        mov    al, 1
        pop    esi
        pop    ebp
        ret    8
loc_1_4A55:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dl, byte ptr [ecx]
        test   dl, 1
        je     SHORT loc_1_4A91
        cmp    dword ptr [ecx + 2], -1
        jne    SHORT loc_1_4A82
        cmp    dl, 0ffh
        jne    SHORT loc_1_4A82
        cmp    byte ptr [ecx + 1], 0ffh
        jne    SHORT loc_1_4A82
        test   al, 8
        je     SHORT loc_1_4A7B
        mov    al, 1
        pop    esi
        pop    ebp
        ret    8
loc_1_4A7B:
        sub    al, al
        pop    esi
        pop    ebp
        ret    8
loc_1_4A82:
        test   al, 4
        jne    SHORT loc_1_4A8A
        test   al, 2
        je     SHORT loc_1_4AA9
loc_1_4A8A:
        mov    al, 1
        pop    esi
        pop    ebp
        ret    8
loc_1_4A91:
        mov    eax, dword ptr [esi + 2eh]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_4AA9
        mov    ax, word ptr [ecx]
        cmp    word ptr [esi + 2ch], ax
        jne    SHORT loc_1_4AA9
        mov    al, 1
        pop    esi
        pop    ebp
        ret    8
loc_1_4AA9:
        sub    al, al
        pop    esi
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
sub_1_4B2A:
        db  0CDh, 020h, 031h, 080h, 028h, 000h
sub_1_4B30:
        db  0CDh, 020h, 032h, 080h, 028h, 000h
sub_1_4B36:
        db  0CDh, 020h, 033h, 080h, 028h, 000h
sub_1_4B3C:
        db  0CDh, 020h, 034h, 080h, 028h, 000h
sub_1_4B42:
        db  0CDh, 020h, 035h, 080h, 028h, 000h
sub_1_4B48:
        db  0CDh, 020h, 036h, 080h, 028h, 000h
sub_1_4B4E:
        db  0CDh, 020h, 037h, 080h, 028h, 000h
sub_1_4B54:
        db  0CDh, 020h, 038h, 080h, 028h, 000h
sub_1_4B5A:
        db  0CDh, 020h, 039h, 080h, 028h, 000h
        db  0CDh, 020h, 03Ah, 080h, 028h, 000h
        db  0CDh, 020h, 03Bh, 080h, 028h, 000h
sub_1_4B6C:
        db  0CDh, 020h, 03Ch, 080h, 028h, 000h
sub_1_4B72:
        db  0CDh, 020h, 03Dh, 080h, 028h, 000h
sub_1_4B78:
        db  0CDh, 020h, 03Eh, 080h, 028h, 000h
sub_1_4B7E:
        db  0CDh, 020h, 03Fh, 080h, 028h, 000h
sub_1_4B84:
        db  0CDh, 020h, 040h, 080h, 028h, 000h
sub_1_4B8A:
        db  0CDh, 020h, 041h, 080h, 028h, 000h
        db  0CDh, 020h, 009h, 080h, 028h, 000h
        db  0CDh, 020h, 008h, 080h, 028h, 000h
sub_1_4B9C:
        db  0CDh, 020h, 044h, 080h, 028h, 000h
sub_1_4BA2:
        db  0CDh, 020h, 048h, 080h, 028h, 000h
sub_1_4BA8:
        db  0CDh, 020h, 049h, 080h, 028h, 000h
        db  0CDh, 020h, 04Fh, 080h, 028h, 000h
        db  0CDh, 020h, 057h, 080h, 028h, 000h
        db  0CDh, 020h, 058h, 080h, 028h, 000h
        db  0CDh, 020h, 059h, 080h, 028h, 000h
sub_1_4BC6:
        db  0CDh, 020h, 05Ah, 080h, 028h, 000h
sub_1_4BCC:
        db  0CDh, 020h, 05Bh, 080h, 028h, 000h
sub_1_4BD2:
        db  0CDh, 020h, 05Ch, 080h, 028h, 000h
sub_1_4BD8:
        db  0CDh, 020h, 05Dh, 080h, 028h, 000h
        db  0CDh, 020h, 05Eh, 080h, 028h, 000h
        db  0CDh, 020h, 056h, 080h, 028h, 000h
        add    byte ptr [eax], al
        db  0CDh, 020h, 000h, 080h, 028h, 000h
sub_1_4BF2:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
sub_1_4BF8:
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
sub_1_4BFE:
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
sub_1_4C04:
        db  0CDh, 020h, 010h, 080h, 028h, 000h
        db  0CDh, 020h, 011h, 080h, 028h, 000h
sub_1_4C10:
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_4C16:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_4C1C:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_4C40:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_4C64:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_4C88:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
sub_1_4C8E:
        db  0CDh, 020h, 028h, 080h, 028h, 000h
sub_1_4C94:
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_4C9A:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_4CA0:
        db  0CDh, 020h, 007h, 080h, 028h, 000h
sub_1_4CA6:
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
        db  0CDh, 020h, 045h, 080h, 028h, 000h
sub_1_4CEE:
        db  0CDh, 020h, 043h, 080h, 028h, 000h
sub_1_4CF4:
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        add    byte ptr [eax], al
sub_1_4CFC:
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
sub_1_4D1C:
        push   esi
        push   edi
        pushfd
        mov    ecx, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        cmp    esi, edi
        jae    SHORT loc_1_4D3C
        or     ecx, ecx
        je     SHORT loc_1_4D52
        std
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    SHORT loc_1_4D50
loc_1_4D3C:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     SHORT loc_1_4D50
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd
        mov    cl, al
loc_1_4D50:
        rep movsb
loc_1_4D52:
        popfd
        pop    edi
        pop    esi
        ret    0ch
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 00Ah, 003h, 000h, 000h, 003h, 00Ah, 000h, 000h, 050h, 043h, 04Eh, 054h
        db  04Eh, 033h, 020h, 020h, 000h, 040h, 000h, 054h, 039h, 047h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_4DB0:
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
loc_1_4EE0:
        pop    esp
        inc    esp
        db  065h, 076h, 069h
        arpl   word ptr [ebp + 5ch], sp
        push   eax
        inc    ebx
        dec    esi
        push   esp
        dec    esi
        xor    esi, dword ptr [eax]
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [ecx], dl
        add    byte ptr [ecx], dl
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h
loc_1_4EFC:
        push   eax
        inc    ebx
        dec    esi
        push   esp
        dec    esi
        xor    eax, dword ptr [eax]
        add    byte ptr [edi], al
        add    byte ptr [edi], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [esi + 64h], cl
        imul   esi, dword ptr [ebx + 52h], 73696765h
        je     SHORT loc_1_4F7C
        db  072h, 04Dh
        popad
        arpl   word ptr [eax], sp
        jae    SHORT loc_1_4F93
        arpl   word ptr [ebx + 65h], sp
        jae    SHORT loc_1_4F96
        db  066h, 075h, 06Ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4F2C:
        inc    edx
        db  075h, 073h
        push   ebx
        arpl   word ptr [ecx + 6eh], sp
        db  020h, 03Dh, 020h, 000h, 000h, 000h
        add    byte ptr [eax], al
        db  000h
loc_1_4F3C:
        insb
        xor    byte ptr fs:[eax], ah
        db  03Dh, 020h, 000h, 000h, 000h
        add    byte ptr [eax], al
loc_1_4F48:
        insb
        xor    dword ptr fs:[eax], esp
        db  03Dh, 020h, 000h, 000h, 000h
        add    byte ptr [eax], al
loc_1_4F54:
        insb
        xor    ah, byte ptr fs:[eax]
        db  03Dh, 020h, 000h, 000h, 000h
        add    byte ptr [eax], al
loc_1_4F60:
        insb
        xor    esp, dword ptr fs:[eax]
        db  03Dh, 020h, 000h, 000h, 000h
        add    byte ptr [eax], al
loc_1_4F6C:
        db  074h, 070h
        and    byte ptr [20h], bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4F78:
        dec    esi
        db  065h, 077h, 020h
loc_1_4F7C:
        inc    ecx
        db  064h, 064h, 072h, 065h
        db  073h, 073h
        db  020h, 03Dh, 020h, 000h, 000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [ecx + 4fh], cl
        and    byte ptr [edx + 61h], ah
        jae    SHORT loc_1_4FF8
loc_1_4F93:
        and    byte ptr [ecx + 64h], ah
loc_1_4F96:
        jb     NEAR PTR loc_1_4FFE
        jae    SHORT loc_1_500E
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [ecx + 6ch], ah
        db  072h, 065h
        popad
        db  064h, 079h, 020h
        imul   ebp, dword ptr [esi + 20h], 20657375h
        bound  edi, qword ptr [ecx + 20h]
        popad
        outsb
        outsd
        je     SHORT loc_1_501E
        jb     NEAR PTR loc_1_4FD9
        db  064h, 065h, 076h, 069h
        arpl   word ptr [ebp], sp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4FC4:
        pop    ecx
        outsd
        db  075h, 020h
        arpl   word ptr [ecx + 6eh], sp
        daa
        db  074h, 020h
        db  073h, 070h
        arpl   word ptr gs:[ecx + 66h], bp
        db  079h, 020h
        dec    ecx
        dec    edi
        inc    edx
loc_1_4FD9:
        popad
        jae    SHORT loc_1_5041
        and    byte ptr [ecx + 64h], ah
        db  064h, 072h, 065h
        jae    SHORT loc_1_5057
        and    byte ptr [esi + 6fh], ah
        db  072h, 020h
        push   eax
        inc    ebx
        dec    ecx
        and    byte ptr [ebp + 76h], ah
        db  069h, 063h, 065h, 000h, 000h, 000h, 000h
        db  000h
loc_1_4FF8:
        dec    ecx
        push   edx
        push   ecx
        and    byte ptr [esi + 75h], ch
loc_1_4FFE:
        insd
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [esi + 6fh], ah
        db  075h, 06Eh
        and    byte ptr fs:[edi + ebp*2 + 65h], ah
        db  073h, 06Eh
loc_1_500E:
        daa
        db  074h, 020h
        insd
        popad
        db  074h, 063h
        push   4f525020h
        push   esp
        dec    edi
        inc    ebx
        dec    edi
loc_1_501E:
        dec    esp
        dec    ecx
        dec    esi
        dec    ecx
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [ebp + ecx*2 + 41h], al
        and    byte ptr [esi + 75h], ch
        insd
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [esi + 6fh], ah
        db  075h, 06Eh
        and    byte ptr fs:[edi + ebp*2 + 65h], ah
        db  073h, 06Eh
        daa
        db  074h, 020h
loc_1_5041:
        insd
        popad
        db  074h, 063h
        push   6f727020h
        db  074h, 06Fh
        arpl   word ptr [edi + 6ch], bp
        imul   ebp, dword ptr cs:[esi + 69h], 20h
loc_1_5057:
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [ecx + 52h], cl
        push   ecx
        and    byte ptr [ecx + 6eh], ah
        das
        outsd
        jb     SHORT loc_1_5087
        inc    esp
        dec    ebp
        inc    ecx
        and    byte ptr [esi + 75h], ch
        insd
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [ecx + 6ch], ah
        db  072h, 065h
        popad
        jns    NEAR PTR loc_1_509D
        imul   ebp, dword ptr [esi + 20h], 20657375h
        bound  edi, qword ptr [ecx + 20h]
loc_1_5087:
        popad
        outsb
        outsd
        db  074h, 068h
        db  065h, 072h, 020h
        jbe    NEAR PTR loc_1_50FC
        arpl   word ptr [ebp], sp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_509C:
        dec    ecx
loc_1_509D:
        dec    edi
        and    byte ptr [edx + 61h], ah
        db  073h, 065h
        and    byte ptr [ecx + 64h], ah
        jb     NEAR PTR loc_1_510E
        db  073h, 073h
        db  020h, 03Dh, 020h, 000h, 000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [ecx + 6eh], cl
        db  074h, 065h
        db  072h, 072h
        db  075h, 070h
        db  074h, 020h
        outsb
        db  075h, 06Dh
        bound  esp, qword ptr [ebp + 72h]
        db  020h, 03Dh, 000h, 000h, 000h, 000h
        add    byte ptr [eax], al
loc_1_50CC:
        inc    esp
        dec    ebp
        inc    ecx
        and    byte ptr [ebx + 68h], ah
        popad
        outsb
        outsb
        insb
        db  020h, 03Dh, 000h, 000h, 000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax + 43h], dl
        dec    ecx
        and    byte ptr [ebx + 63h], dh
        popad
        outsb
        and    byte ptr [ebx + 70h], dh
        arpl   word ptr gs:[ecx + 66h], bp
        imul   esp, dword ptr [ebp + 64h], 4350202ch
        dec    ecx
        and    byte ptr [edx + 75h], ah
        db  073h, 020h
loc_1_50FC:
        outsb
        outsd
        db  074h, 020h
        outsw
        db  075h, 06Eh
        add    byte ptr fs:[eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax + 43h], dl
loc_1_510E:
        dec    ecx
        and    byte ptr [ebx + 63h], dh
        popad
        outsb
        and    byte ptr [ebx + 70h], dh
        arpl   word ptr gs:[ecx + 66h], bp
        imul   esp, dword ptr [ebp + 64h], 6564202ch
        db  076h, 069h
        arpl   word ptr [ebp + 20h], sp
        outsb
        outsd
        je     SHORT loc_1_514B
        outsw
        jne    SHORT loc_1_519D
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5138:
        dec    esp
        popad
        outsb
        arpl   word ptr [ebp + 4fh], sp
        db  070h, 065h
        outsb
        inc    ecx
        popad
        jo     SHORT loc_1_51BA
        db  065h, 072h, 020h
        popaw
loc_1_514B:
        db  069h, 06Ch, 065h, 064h, 000h, 000h, 000h, 000h
        add    byte ptr [eax], ah
        inc    esp
        db  065h, 076h, 069h
        arpl   word ptr [ebp + 20h], sp
        popad
        je     SHORT loc_1_517F
        db  073h, 070h
        arpl   word ptr gs:[ecx + 66h], bp
        imul   esp, dword ptr [ebp + 64h], 204f4920h
        bound  esp, qword ptr [ecx + 73h]
        and    byte ptr gs:[ecx + 64h], ah
        jb     NEAR PTR loc_1_51DB
        jae    SHORT loc_1_51EB
        and    byte ptr [esi + 6fh], ch
        je     SHORT loc_1_519D
        outsw
loc_1_517F:
        db  075h, 06Eh
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5188:
        and    byte ptr [ebp + 76h], al
        imul   esp, dword ptr [ebx + 65h], 746f6e20h
        and    byte ptr [esi + 6fh], ah
        jne    SHORT loc_1_5206
        add    byte ptr fs:[eax], al
        db  000h, 000h
loc_1_519D:
        add    byte ptr [eax], al
        add    byte ptr [edx], dh
        xor    bh, byte ptr [edx]
        and    byte ptr [eax + 6eh], dl
        push   eax
        and    byte ptr [ebx + 63h], dh
        popad
        outsb
        and    byte ptr [ebx + 70h], dh
        arpl   word ptr gs:[ecx + 66h], bp
        imul   esp, dword ptr [ebp + 64h], 6564202ch
loc_1_51BA:
        jbe    SHORT loc_1_5225
        arpl   word ptr [ebp + 20h], sp
        outsb
        outsd
        db  074h, 020h
        outsw
        db  075h, 06Eh
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_51D0:
        xor    dh, byte ptr [ebx]
        cmp    ah, byte ptr [eax]
        push   esi
        inc    ebp
        push   ebx
        inc    ecx
        and    byte ptr [ebx + 63h], dh
loc_1_51DB:
        popad
        outsb
        and    byte ptr [ebx + 70h], dh
        arpl   word ptr gs:[ecx + 66h], bp
        imul   esp, dword ptr [ebp + 64h], 6564202ch
loc_1_51EB:
        jbe    SHORT loc_1_5256
        arpl   word ptr [ebp + 20h], sp
        outsb
        outsd
        db  074h, 020h
        outsw
        db  075h, 06Eh
        add    byte ptr fs:[eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [edx], dh
        xor    al, 3ah
        and    byte ptr [ecx + 53h], cl
loc_1_5206:
        inc    ecx
        and    byte ptr [ebx + 63h], dh
        popad
        outsb
        and    byte ptr [ebx + 70h], dh
        arpl   word ptr gs:[ecx + 66h], bp
        imul   esp, dword ptr [ebp + 64h], 6564202ch
        jbe    SHORT loc_1_5285
        arpl   word ptr [ebp + 20h], sp
        outsb
        outsd
        db  074h, 020h
        outsw
loc_1_5225:
        jne    SHORT loc_1_5295
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_523C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5240:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5244:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5248:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_524C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 000h, 000h
loc_1_5254:
        cmp    al, 0
loc_1_5256:
        add    byte ptr [eax], al
loc_1_5258:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_5285:
        add    byte ptr [eax], al
        add    byte ptr [esi + 45h], cl
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
loc_1_5295:
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
        jb     NEAR PTR loc_1_530C
        jae    SHORT loc_1_531C
        add    byte ptr [eax], al
        add    byte ptr [ecx + 4fh], cl
        inc    ecx
        jb     NEAR PTR loc_1_5318
        jae    SHORT loc_1_5328
        add    byte ptr [eax], al
        add    byte ptr [ecx + 6eh], cl
        je     SHORT loc_1_5321
        db  072h, 072h
        db  075h, 070h
        je     SHORT loc_1_52C2
loc_1_52C2:
        add    byte ptr [eax], al
loc_1_52C4:
        dec    ecx
        outsb
        je     SHORT loc_1_532D
        db  072h, 072h
        db  075h, 070h
        je     SHORT loc_1_52CE
loc_1_52CE:
        add    byte ptr [eax], al
        inc    esp
        insd
        popad
        inc    ebx
        push   656e6e61h
        insb
        add    byte ptr [eax], al
loc_1_52DC:
        inc    esp
        insd
        popad
        inc    ebx
        push   656e6e61h
        insb
        add    byte ptr [eax], al
        inc    edx
        db  075h, 073h
        push   esp
        db  079h, 070h
        add    byte ptr gs:[edx + 75h], al
        db  073h, 054h
        db  079h, 070h
        add    byte ptr gs:[edx + 55h], al
        push   ebx
        pop    edi
        push   esp
        dec    edi
        pop    edi
        push   ebx
        inc    ebx
        inc    ecx
        dec    esi
        add    byte ptr [edx + 55h], al
        push   ebx
        pop    edi
        push   esp
        dec    edi
        pop    edi
        push   ebx
loc_1_530C:
        inc    ebx
        inc    ecx
        dec    esi
        add    byte ptr [esi + 44h], al
        push   ebp
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5318:
        inc    esi
        inc    esp
        push   ebp
        push   eax
loc_1_531C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   esp
loc_1_5321:
        push   eax
        add    byte ptr [eax], al
loc_1_5324:
        push   esp
        push   eax
        add    byte ptr [eax], al
loc_1_5328:
        dec    esp
        inc    ebp
        inc    esp
        xor    byte ptr [eax], al
loc_1_532D:
        add    byte ptr [eax], al
        add    byte ptr [ebp + eax*2 + 44h], cl
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp + eax*2 + 44h], cl
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [ebp + eax*2 + 44h], cl
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [ebp + eax*2 + 44h], cl
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ebp + eax*2 + 44h], cl
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ebp + eax*2 + 44h], cl
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ebp + eax*2 + 44h], cl
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [edx + 55h], al
        push   ebx
        push   esp
        dec    ecx
        dec    ebp
        inc    ebp
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5374:
        inc    edx
        push   ebp
        push   ebx
        push   esp
        dec    ecx
        dec    ebp
        inc    ebp
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   eax
        inc    ebx
        dec    ecx
        add    byte ptr [eax + 43h], dl
        dec    ecx
        add    byte ptr [eax + 43h], dl
        dec    ecx
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax + 43h], dl
        dec    ecx
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax + 43h], dl
        dec    ecx
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax + 43h], dl
        dec    ecx
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax + 4eh], dl
        push   eax
        add    byte ptr [eax + 4eh], dl
        push   eax
        add    byte ptr [esi + 45h], dl
        push   ebx
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_53B8:
        push   esi
        inc    ebp
        push   ebx
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ecx
        push   ebx
        inc    ecx
        add    byte ptr [ecx + 53h], cl
        inc    ecx
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h
_LTEXT ends
        end
