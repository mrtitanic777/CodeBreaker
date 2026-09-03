; elnkii_lcode_01.asm -- ELNKII object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        lea    eax, [esi + 8]
        db  068h, 0D0h, 004h, 000h, 000h
        push   eax
        call   sub_1_1CCA
        mov    ecx, dword ptr [esi + 50h]
        sub    edi, edi
        lea    eax, [ecx + 0bh]
        mov    dx, ax
        sub    al, al
        out    dx, al
        lea    edx, [ecx + 0ah]
        out    dx, al
        mov    ebx, 80h
loc_1_0032:
        mov    edx, dword ptr [esi + 50h]
        add    edx, 7
        in     al, dx
        test   bl, al
        jne    SHORT loc_1_004D
        push   3e8h
        call   sub_1_1CD0
        inc    edi
        cmp    edi, 4
        jb     SHORT loc_1_0032
loc_1_004D:
        mov    ecx, dword ptr [esi + 50h]
        mov    al, 2
        lea    edx, [ecx + 0dh]
        out    dx, al
        mov    dx, cx
        mov    al, 22h
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0070:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [ebp - 0ch]
        cmp    byte ptr [esi + 4ah], 0
        push   0
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        je     SHORT loc_1_00F7
        call   sub_1_1D00
        movsx  edi, word ptr [ebp + 10h]
        shl    edi, 9
        lea    edi, [edi + edi*2]
        add    edi, dword ptr [esi + 86h]
        sub    esi, esi
        cmp    dword ptr [ebp - 0ch], esi
        je     SHORT loc_1_00E7
        sub    ebx, ebx
        mov    edi, edi
loc_1_00B0:
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_1D24
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_1_1DBC
        add    edi, dword ptr [ebp - 10h]
        add    esi, dword ptr [ebp - 10h]
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_1D48
        cmp    dword ptr [ebp - 0ch], ebx
        jne    SHORT loc_1_00B0
loc_1_00E7:
        mov    eax, dword ptr [ebp + 14h]
        pop    edi
        mov    dword ptr [eax], esi
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_00F7:
        call   sub_1_1D00
        movsx  eax, word ptr [ebp + 10h]
        shl    eax, 9
        mov    dword ptr [ebp - 18h], 0
        lea    eax, [eax + eax*2 + 2000h]
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_0234
        mov    edi, edi
loc_1_0120:
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_1D24
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_0140
        jmp    NEAR PTR loc_1_021D
        db  08Dh, 064h, 024h, 000h
loc_1_0140:
        mov    edi, dword ptr [esi + 54h]
        mov    ecx, dword ptr [ebp - 14h]
        lea    edx, [edi + 9]
        mov    al, ch
        out    dx, al
        lea    edx, [edi + 0ah]
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 6]
        mov    al, byte ptr [esi + 95h]
        or     al, 0c0h
        out    dx, al
        mov    edi, dword ptr [ebp - 10h]
        cmp    edi, 10h
        jb     SHORT loc_1_01BB
        mov    ebx, 60h
loc_1_016B:
        test   byte ptr [ebp - 8], 1
        je     SHORT loc_1_0185
        push   10h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [esi + 54h]
        add    eax, 0eh
        push   eax
        call   sub_1_1B70
        jmp    SHORT loc_1_0197
loc_1_0185:
        push   8
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [esi + 54h]
        add    eax, 0eh
        push   eax
        call   sub_1_1B84
loc_1_0197:
        sub    edi, 10h
        add    dword ptr [ebp - 8], 10h
        mov    ecx, 80h
loc_1_01A3:
        mov    edx, dword ptr [esi + 54h]
        add    edx, 7
        in     al, dx
        test   bl, al
        jne    NEAR PTR loc_1_0247
        test   cl, al
        je     SHORT loc_1_01A3
        cmp    edi, 10h
        jae    SHORT loc_1_016B
loc_1_01BB:
        push   edi
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [esi + 54h]
        add    eax, 0eh
        push   eax
        call   sub_1_1B70
        mov    ecx, 60h
        mov    ebx, 80h
loc_1_01D6:
        mov    edx, dword ptr [esi + 54h]
        add    edx, 7
        in     al, dx
        test   cl, al
        jne    NEAR PTR loc_1_0269
        test   bl, al
        je     SHORT loc_1_01D6
        mov    edx, dword ptr [esi + 54h]
        add    edx, 6
        mov    al, byte ptr [esi + 95h]
        out    dx, al
        mov    ecx, 8
        add    eax, 0
loc_1_0200:
        mov    edx, dword ptr [esi + 54h]
        add    edx, 7
        in     al, dx
        test   cl, al
        jne    SHORT loc_1_0200
        mov    eax, dword ptr [ebp - 14h]
        add    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        mov    ecx, dword ptr [ebp - 18h]
        add    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 18h], ecx
loc_1_021D:
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_1D48
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_0120
loc_1_0234:
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [eax], ecx
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_0247:
        push   2
        push   2
        push   2
        push   0aa000006h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_1DA8
        add    esp, 14h
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_0269:
        push   2
        push   2
        push   2
        push   0aa000006h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_1DA8
        add    esp, 14h
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h
sub_1_0290:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        movsx  eax, word ptr [ebp + 10h]
        cmp    byte ptr [esi + 4ah], 0
        je     SHORT loc_1_02D0
        shl    eax, 9
        mov    edi, dword ptr [ebp + 18h]
        lea    eax, [eax + eax*2]
        add    eax, dword ptr [esi + 86h]
        add    eax, dword ptr [ebp + 14h]
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        push   eax
        call   sub_1_1DBC
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Bh, 0FFh
loc_1_02D0:
        shl    eax, 9
        mov    edi, dword ptr [esi + 54h]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 14h]
        lea    ecx, [eax + ecx + 2000h]
        lea    edx, [edi + 9]
        mov    al, ch
        out    dx, al
        lea    edx, [edi + 0ah]
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 6]
        mov    al, byte ptr [esi + 95h]
        or     al, 0c0h
        out    dx, al
        mov    edi, dword ptr [ebp + 18h]
        or     edi, edi
        je     SHORT loc_1_0343
loc_1_0302:
        mov    ebx, edi
        cmp    ebx, 10h
        jb     SHORT loc_1_030E
        mov    ebx, 10h
loc_1_030E:
        push   ebx
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        mov    eax, dword ptr [esi + 54h]
        add    eax, 0eh
        push   eax
        call   sub_1_1B70
        sub    edi, ebx
        add    dword ptr [ebp + 0ch], ebx
        mov    ecx, 60h
        mov    ebx, 80h
        mov    edi, edi
loc_1_0330:
        mov    edx, dword ptr [esi + 54h]
        add    edx, 7
        in     al, dx
        test   cl, al
        jne    SHORT loc_1_036B
        test   bl, al
        je     SHORT loc_1_0330
        or     edi, edi
        jne    SHORT loc_1_0302
loc_1_0343:
        mov    edx, dword ptr [esi + 54h]
        add    edx, 6
        mov    al, byte ptr [esi + 95h]
        out    dx, al
        mov    ecx, 8
loc_1_0355:
        mov    edx, dword ptr [esi + 54h]
        add    edx, 7
        in     al, dx
        test   cl, al
        jne    SHORT loc_1_0355
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_036B:
        push   2
        push   3
        push   2
        push   0aa000006h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_1DA8
        add    esp, 14h
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 049h, 000h
sub_1_0390:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 14h]
        or     edi, edi
        jne    SHORT loc_1_03B0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h
loc_1_03B0:
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 4ah], 0
        je     SHORT loc_1_03D2
        push   edi
        mov    ecx, dword ptr [ebp + 10h]
        push   ecx
        mov    ebx, dword ptr [ebp + 0ch]
        push   ebx
        call   sub_1_1DBC
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_03D2:
        mov    eax, 2000h
        sub    eax, dword ptr [esi + 86h]
        mov    ecx, dword ptr [ebp + 10h]
        mov    ebx, dword ptr [esi + 54h]
        add    ecx, eax
        lea    edx, [ebx + 9]
        mov    al, ch
        out    dx, al
        lea    edx, [ebx + 0ah]
        mov    al, cl
        out    dx, al
        lea    edx, [ebx + 6]
        mov    al, byte ptr [esi + 95h]
        or     al, 80h
        out    dx, al
        cmp    edi, 10h
        mov    ebx, dword ptr [ebp + 0ch]
        jb     SHORT loc_1_0468
loc_1_0405:
        mov    edx, dword ptr [esi + 54h]
        add    edx, 7
        in     al, dx
        test   al, 60h
        jne    SHORT loc_1_0446
        test   al, 80h
        je     SHORT loc_1_0405
        test   bl, 1
        je     SHORT loc_1_042A
        push   10h
        push   ebx
        mov    eax, dword ptr [esi + 54h]
        add    eax, 0eh
        push   eax
        call   sub_1_1BAD
        jmp    SHORT loc_1_0439
loc_1_042A:
        push   8
        push   ebx
        mov    eax, dword ptr [esi + 54h]
        add    eax, 0eh
        push   eax
        call   sub_1_1BC1
loc_1_0439:
        sub    edi, 10h
        add    ebx, 10h
        cmp    edi, 10h
        jae    SHORT loc_1_0405
        jmp    SHORT loc_1_0468
loc_1_0446:
        push   2
        push   4
        push   2
        push   0aa000006h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_1DA8
        add    esp, 14h
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_0468:
        mov    ecx, 60h
        lea    ecx, [ecx]
loc_1_0470:
        mov    edx, dword ptr [esi + 54h]
        add    edx, 7
        in     al, dx
        test   cl, al
        jne    SHORT loc_1_04A8
        test   al, 80h
        je     SHORT loc_1_0470
        push   edi
        push   ebx
        mov    eax, dword ptr [esi + 54h]
        add    eax, 0eh
        push   eax
        call   sub_1_1BAD
        mov    ecx, dword ptr [esi + 54h]
        add    ecx, 6
        mov    dx, cx
        mov    al, byte ptr [esi + 95h]
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_04A8:
        push   2
        push   4
        push   2
        push   0aa000006h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_1DA8
        add    esp, 14h
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_04D0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dx, word ptr [eax + 50h]
        mov    al, 21h
        out    dx, al
        sub    al, al
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_04F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 4], 6
        mov    ax, word ptr [esi + 6ah]
        movsx  ecx, ax
        mov    edi, dword ptr [esi + 50h]
        mov    ecx, dword ptr [esi + ecx*4 + 7eh]
        lea    edx, [edi + 4]
        mov    bl, byte ptr [esi + 92h]
        imul   byte ptr [ebp - 4]
        add    bl, al
        mov    al, bl
        out    dx, al
        cmp    ecx, 3ch
        jae    SHORT loc_1_0529
        mov    ecx, 3ch
loc_1_0529:
        lea    edx, [edi + 6]
        mov    al, ch
        out    dx, al
        lea    edx, [edi + 5]
        mov    al, cl
        out    dx, al
        mov    dx, di
        mov    al, 26h
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0550:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    edx, dword ptr [ecx + 50h]
        add    edx, 4
        in     al, dx
        mov    byte ptr [ecx + 98h], al
        sub    al, al
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0570:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    al, 62h
        mov    edx, dword ptr [ecx + 50h]
        out    dx, al
        add    edx, 7
        in     al, dx
        mov    byte ptr [ecx + 97h], al
        mov    dx, word ptr [ecx + 50h]
        mov    al, 22h
        out    dx, al
        sub    al, al
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_05A0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx + 96h]
        cmp    byte ptr [ecx + 93h], al
        mov    edx, dword ptr [ecx + 50h]
        jne    SHORT loc_1_05C7
        add    edx, 3
        mov    al, byte ptr [ecx + 94h]
        dec    al
        out    dx, al
        pop    ebp
        ret    4
loc_1_05C7:
        add    edx, 3
        dec    al
        out    dx, al
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_05E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 50h]
        add    edx, 7
        in     al, dx
        test   al, 4
        je     SHORT loc_1_05FB
        push   esi
        call   sub_1_0610
loc_1_05FB:
        mov    edx, dword ptr [esi + 50h]
        add    edx, 7
        mov    al, 10h
        out    dx, al
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_0610:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    edx, dword ptr [esi + 50h]
        add    edx, 0dh
        in     al, dx
        mov    cl, al
        mov    edx, dword ptr [esi + 50h]
        add    dword ptr [esi + 1bfh], ecx
        add    edx, 0eh
        in     al, dx
        xor    ecx, ecx
        mov    cl, al
        mov    edx, dword ptr [esi + 50h]
        add    dword ptr [esi + 1c3h], ecx
        add    edx, 0fh
        in     al, dx
        and    eax, 0ffh
        add    dword ptr [esi + 1c7h], eax
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0660:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_04D0
        push   7d0h
        call   sub_1_1CD0
        mov    ecx, dword ptr [esi + 50h]
        sub    al, al
        lea    esi, [ecx + 0bh]
        mov    dx, si
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
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_06A0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 4bh], 0
        je     SHORT loc_1_06B2
        sub    al, al
        pop    ebp
        ret    4
loc_1_06B2:
        mov    edx, dword ptr [eax + 50h]
        add    edx, 0fh
        sub    al, al
        out    dx, al
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_06D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        inc    dword ptr [esi + 30h]
        mov    edx, dword ptr [esi + 50h]
        add    edx, 7
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, al
        je     NEAR PTR loc_1_0773
        mov    edx, dword ptr [esi + 50h]
        add    edx, 7
        out    dx, al
        test   al, 20h
        je     SHORT loc_1_0702
        mov    eax, 8
        jmp    SHORT loc_1_0778
loc_1_0702:
        test   byte ptr [ebp - 1], 10h
        je     SHORT loc_1_0715
        push   esi
        call   sub_1_0610
        mov    eax, 4
        jmp    SHORT loc_1_0778
loc_1_0715:
        test   byte ptr [ebp - 1], 1
        je     SHORT loc_1_0747
        cmp    dword ptr [esi + 34h], 0ah
        jae    SHORT loc_1_0728
        mov    eax, 1
        jmp    SHORT loc_1_0778
loc_1_0728:
        mov    dword ptr [esi + 34h], 0
        test   byte ptr [ebp - 1], 0ah
        je     SHORT loc_1_0740
        mov    eax, 2
        jmp    SHORT loc_1_0778
        db  08Dh, 064h, 024h, 000h
loc_1_0740:
        mov    eax, 1
        jmp    SHORT loc_1_0778
loc_1_0747:
        mov    dword ptr [esi + 34h], 0
        test   byte ptr [ebp - 1], 0ah
        je     SHORT loc_1_0760
        mov    eax, 2
        jmp    SHORT loc_1_0778
        db  005h, 000h, 000h, 000h, 000h
loc_1_0760:
        test   byte ptr [ebp - 1], 4
        je     SHORT loc_1_0773
        push   esi
        call   sub_1_0610
        mov    eax, 1
        jmp    SHORT loc_1_0778
loc_1_0773:
        mov    eax, 10h
loc_1_0778:
        sub    ebx, ebx
loc_1_077A:
        cmp    eax, 10h
        jne    SHORT loc_1_07A3
        cmp    dword ptr [esi + 1eah], ebx
        je     NEAR PTR loc_1_08DA
        cmp    byte ptr [esi + 1aah], bl
        jne    NEAR PTR loc_1_08DA
        cmp    byte ptr [esi + 1cbh], bl
        jne    NEAR PTR loc_1_08DA
loc_1_07A3:
        cmp    eax, 1
        je     SHORT loc_1_07FD
        cmp    eax, 2
        je     SHORT loc_1_07B9
        cmp    eax, 4
        je     SHORT loc_1_07E0
        cmp    eax, 8
        je     SHORT loc_1_0811
        jmp    SHORT loc_1_0824
loc_1_07B9:
        push   esi
        call   sub_1_0550
        mov    byte ptr [esi + 22eh], bl
        cmp    byte ptr [esi + 5ch], bl
        jne    SHORT loc_1_07D3
        mov    byte ptr [esi + 6ch], bl
        push   esi
        call   sub_1_0B70
loc_1_07D3:
        and    byte ptr [ebp - 1], 0f5h
        jmp    SHORT loc_1_0824
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_07E0:
        mov    byte ptr [esi + 1a9h], 1
        cmp    byte ptr [esi + 6ch], bl
        je     SHORT loc_1_07F9
        test   byte ptr [ebp - 1], 0ah
        jne    SHORT loc_1_07F9
        or     byte ptr [ebp - 1], 2
        mov    byte ptr [esi + 6dh], bl
loc_1_07F9:
        and    byte ptr [ebp - 1], 0efh
loc_1_07FD:
        cmp    byte ptr [esi + 1aah], bl
        jne    SHORT loc_1_0820
        push   esi
        call   sub_1_09D0
        cmp    al, bl
        je     SHORT loc_1_0824
        jmp    SHORT loc_1_0820
loc_1_0811:
        push   esi
        call   sub_1_0610
        and    byte ptr [ebp - 1], 0dfh
        jmp    SHORT loc_1_0824
        db  08Dh, 049h, 000h
loc_1_0820:
        and    byte ptr [ebp - 1], 0fah
loc_1_0824:
        cmp    dword ptr [esi + 1eah], ebx
        je     SHORT loc_1_0842
        cmp    byte ptr [esi + 1aah], bl
        jne    SHORT loc_1_0842
        cmp    byte ptr [esi + 1cbh], bl
        jne    SHORT loc_1_0842
        push   esi
        call   sub_1_09D0
loc_1_0842:
        test   byte ptr [ebp - 1], 20h
        je     SHORT loc_1_0852
        mov    eax, 8
        jmp    NEAR PTR loc_1_077A
loc_1_0852:
        test   byte ptr [ebp - 1], 10h
        je     SHORT loc_1_0868
        push   esi
        call   sub_1_0610
        mov    eax, 4
        jmp    NEAR PTR loc_1_077A
loc_1_0868:
        test   byte ptr [ebp - 1], 1
        je     SHORT loc_1_08A0
        cmp    dword ptr [esi + 34h], 0ah
        jae    SHORT loc_1_0880
        mov    eax, 1
        jmp    NEAR PTR loc_1_077A
        db  08Bh, 0FFh
loc_1_0880:
        mov    dword ptr [esi + 34h], ebx
        test   byte ptr [ebp - 1], 0ah
        je     SHORT loc_1_0893
        mov    eax, 2
        jmp    NEAR PTR loc_1_077A
loc_1_0893:
        mov    eax, 1
        jmp    NEAR PTR loc_1_077A
        db  08Dh, 049h, 000h
loc_1_08A0:
        mov    dword ptr [esi + 34h], ebx
        test   byte ptr [ebp - 1], 0ah
        je     SHORT loc_1_08B3
        mov    eax, 2
        jmp    NEAR PTR loc_1_077A
loc_1_08B3:
        test   byte ptr [ebp - 1], 4
        je     SHORT loc_1_08D0
        push   esi
        call   sub_1_0610
        mov    eax, 1
        jmp    NEAR PTR loc_1_077A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_08D0:
        mov    eax, 10h
        jmp    NEAR PTR loc_1_077A
loc_1_08DA:
        mov    edx, dword ptr [esi + 50h]
        add    edx, 7
        in     al, dx
        mov    byte ptr [ebp - 1], al
        cmp    al, bl
        je     SHORT loc_1_08EF
        mov    edx, dword ptr [esi + 50h]
        add    edx, 7
        out    dx, al
loc_1_08EF:
        test   byte ptr [ebp - 1], 20h
        je     SHORT loc_1_0900
        mov    eax, 8
        jmp    SHORT loc_1_0957
        db  08Dh, 064h, 024h, 000h
loc_1_0900:
        test   byte ptr [ebp - 1], 10h
        je     SHORT loc_1_0913
        push   esi
        call   sub_1_0610
        mov    eax, 4
        jmp    SHORT loc_1_0957
loc_1_0913:
        test   byte ptr [ebp - 1], 1
        je     SHORT loc_1_0931
        cmp    dword ptr [esi + 34h], 0ah
        jae    SHORT loc_1_0921
        jmp    SHORT loc_1_0952
loc_1_0921:
        mov    dword ptr [esi + 34h], ebx
        test   byte ptr [ebp - 1], 0ah
        je     SHORT loc_1_0952
        mov    eax, 2
        jmp    SHORT loc_1_0957
loc_1_0931:
        mov    dword ptr [esi + 34h], ebx
        test   byte ptr [ebp - 1], 0ah
        je     SHORT loc_1_0941
        mov    eax, 2
        jmp    SHORT loc_1_0957
loc_1_0941:
        test   byte ptr [ebp - 1], 4
        mov    eax, 10h
        je     SHORT loc_1_0957
        push   esi
        call   sub_1_0610
loc_1_0952:
        mov    eax, 1
loc_1_0957:
        cmp    eax, 10h
        jne    NEAR PTR loc_1_07A3
        mov    byte ptr [esi + 1e5h], 1bh
        mov    edx, dword ptr [esi + 50h]
        add    edx, 0fh
        mov    al, 1bh
        out    dx, al
        mov    eax, dword ptr [esi + 30h]
        dec    eax
        mov    dword ptr [esi + 30h], eax
        jne    SHORT loc_1_09A7
        cmp    byte ptr [esi + 1cbh], 0
        jne    SHORT loc_1_0991
        cmp    dword ptr [esi + 1f6h], 0
        jne    SHORT loc_1_0991
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_1_09A7
loc_1_0991:
        push   1
        add    esi, 202h
        push   esi
        call   sub_1_1CB2
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_09A7:
        cmp    dword ptr [esi + 5eh], 0
        je     SHORT loc_1_09BE
        movsx  eax, word ptr [esi + 66h]
        cmp    dword ptr [esi + eax*4 + 6eh], 0
        jne    SHORT loc_1_09BE
        push   esi
        call   sub_1_1400
loc_1_09BE:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_09D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], 1
        mov    byte ptr [esi + 1aah], 1
        cmp    byte ptr [esi + 1cbh], 0
        jne    SHORT loc_1_0A08
        cmp    byte ptr [esi + 1a9h], 0
        je     SHORT loc_1_0A08
        push   esi
        db  068h, 060h, 006h, 000h, 000h
        lea    eax, [esi + 8]
        push   eax
        call   sub_1_1CCA
loc_1_0A08:
        push   esi
        call   sub_1_0570
        mov    edi, edi
loc_1_0A10:
        cmp    byte ptr [esi + 1cbh], 0
        jne    SHORT loc_1_0A6A
        mov    al, byte ptr [esi + 97h]
        cmp    byte ptr [esi + 96h], al
        je     NEAR PTR loc_1_0B26
        mov    dword ptr [esi + 1f2h], 0
        inc    dword ptr [esi + 34h]
        push   esi
        call   sub_1_0F30
        cmp    eax, 3
        je     NEAR PTR loc_1_0B43
        mov    al, byte ptr [esi + 9ah]
        mov    byte ptr [esi + 96h], al
        push   esi
        call   sub_1_05A0
        cmp    dword ptr [esi + 34h], 0ah
        jbe    SHORT loc_1_0A10
        mov    byte ptr [ebp - 1], 0
        mov    dword ptr [esi + 34h], 0
loc_1_0A6A:
        cmp    byte ptr [esi + 1a9h], 0
        je     SHORT loc_1_0AB4
        mov    byte ptr [esi + 1a9h], 0
        push   esi
        db  068h, 0E0h, 005h, 000h, 000h
        lea    eax, [esi + 8]
        push   eax
        call   sub_1_1CCA
        mov    eax, dword ptr [esi + 50h]
        add    eax, 0dh
        mov    dx, ax
        sub    al, al
        out    dx, al
        cmp    byte ptr [esi + 6dh], al
        je     SHORT loc_1_0AB4
        cmp    word ptr [esi + 6ah], -1
        je     SHORT loc_1_0AB4
        mov    byte ptr [esi + 6dh], al
        mov    byte ptr [esi + 22eh], al
        mov    byte ptr [esi + 6ch], 1
        push   esi
        call   sub_1_04F0
loc_1_0AB4:
        cmp    dword ptr [esi + 1eah], 0
        je     SHORT loc_1_0B0A
        lea    ecx, [ecx]
loc_1_0AC0:
        cmp    byte ptr [esi + 1cbh], 0
        jne    SHORT loc_1_0B0A
        mov    ebx, dword ptr [esi + 1eah]
        mov    eax, dword ptr [ebx + 24h]
        mov    dword ptr [esi + 1eah], eax
        mov    dword ptr [esi + 1f2h], ebx
        mov    eax, ebx
        inc    dword ptr [esi + 1abh]
        mov    edi, dword ptr [ebx + 28h]
        push   eax
        push   esi
        call   sub_1_0D90
        push   0
        mov    eax, dword ptr [ebx + 28h]
        push   ebx
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_1C1A
        dec    dword ptr [edi + 1ch]
        cmp    dword ptr [esi + 1eah], 0
        jne    SHORT loc_1_0AC0
loc_1_0B0A:
        mov    byte ptr [esi + 1aah], 0
        cmp    byte ptr [esi + 1cbh], 0
        je     SHORT loc_1_0B55
        mov    al, byte ptr [ebp - 1]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0B26:
        push   esi
        call   sub_1_0570
        mov    al, byte ptr [esi + 97h]
        cmp    byte ptr [esi + 96h], al
        jne    NEAR PTR loc_1_0A10
        jmp    NEAR PTR loc_1_0A6A
loc_1_0B43:
        mov    byte ptr [esi + 1e5h], 1ah
        push   esi
        db  0E8h, 010h, 016h, 0FFh, 0FFh
        jmp    NEAR PTR loc_1_0A6A
loc_1_0B55:
        mov    eax, dword ptr [esi + 1d4h]
        push   eax
        call   sub_1_1A60
        mov    al, byte ptr [ebp - 1]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_0B70:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 22eh], 0
        mov    byte ptr [esi + 5ch], 1
        mov    ax, word ptr [esi + 6ah]
        cmp    ax, 0ffffh
        jne    SHORT loc_1_0B99
        mov    byte ptr [esi + 5ch], 0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0B99:
        movsx  eax, ax
        mov    edx, dword ptr [esi + eax*4 + 76h]
        cmp    byte ptr [edx + 2ch], 0
        lea    eax, [edx + 24h]
        jne    SHORT loc_1_0BF0
        test   byte ptr [esi + 98h], 1
        je     SHORT loc_1_0BC0
        inc    dword ptr [esi + 1abh]
        jmp    SHORT loc_1_0BC6
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0BC0:
        inc    dword ptr [esi + 1b3h]
loc_1_0BC6:
        mov    edi, dword ptr [eax + 4]
        mov    al, byte ptr [esi + 98h]
        and    al, 1
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0c0000001h
        push   eax
        push   edx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1C1A
        dec    dword ptr [edi + 1ch]
        jmp    SHORT loc_1_0C16
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0BF0:
        cmp    dword ptr [esi + 1eah], 0
        jne    SHORT loc_1_0C01
        mov    dword ptr [esi + 1eah], edx
        jmp    SHORT loc_1_0C0A
loc_1_0C01:
        mov    ecx, dword ptr [esi + 1eeh]
        mov    dword ptr [ecx + 24h], edx
loc_1_0C0A:
        mov    dword ptr [esi + 1eeh], edx
        mov    dword ptr [eax], 0
loc_1_0C16:
        movsx  eax, word ptr [esi + 6ah]
        mov    dword ptr [esi + eax*4 + 76h], 0
        movsx  eax, word ptr [esi + 6ah]
        mov    dword ptr [esi + eax*4 + 6eh], 0
        sub    edx, edx
        movsx  eax, word ptr [esi + 6ah]
        inc    eax
        div    dword ptr [esi + 38h]
        mov    di, dx
        movsx  eax, di
        lea    ebx, [esi + eax*4]
        mov    ecx, dword ptr [ebx + 6eh]
        or     ecx, ecx
        je     SHORT loc_1_0C65
        cmp    ecx, 1
        je     NEAR PTR loc_1_0D16
        cmp    ecx, 2
        je     NEAR PTR loc_1_0D31
        mov    byte ptr [esi + 5ch], 0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0C65:
        cmp    byte ptr [esi + 1cbh], 0
        je     SHORT loc_1_0C87
        mov    word ptr [esi + 6ah], 0ffffh
        mov    byte ptr [esi + 5ch], 0
        push   2
        push   esi
        db  0E8h, 010h, 00Ch, 0FFh, 0FFh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0C87:
        mov    ecx, dword ptr [esi + 5eh]
        or     ecx, ecx
        je     SHORT loc_1_0D01
        mov    edx, dword ptr [ecx + 24h]
        mov    dword ptr [esi + 5eh], edx
        inc    eax
        sub    edx, edx
        div    dword ptr [esi + 38h]
        mov    word ptr [esi + 66h], dx
        mov    dword ptr [ebx + 6eh], 2
        mov    dword ptr [ebx + 76h], ecx
        add    ebx, 7eh
        mov    word ptr [esi + 6ah], di
        push   ebx
        push   edi
        push   ecx
        push   esi
        call   sub_1_0070
        mov    eax, dword ptr [ebx]
        cmp    eax, 3ch
        jae    SHORT loc_1_0CD4
        mov    ecx, 3ch
        sub    ecx, eax
        push   ecx
        push   eax
        push   edi
        db  068h, 08Ch, 020h, 000h, 000h
        push   esi
        call   sub_1_0290
loc_1_0CD4:
        mov    byte ptr [esi + 5ch], 0
        cmp    byte ptr [esi + 1a9h], 0
        je     SHORT loc_1_0CF0
        mov    byte ptr [esi + 6dh], 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_0CF0:
        mov    byte ptr [esi + 6ch], 1
        push   esi
        call   sub_1_04F0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0D01:
        mov    word ptr [esi + 6ah], 0ffffh
        mov    word ptr [esi + 68h], di
        mov    byte ptr [esi + 5ch], 0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0D16:
        mov    word ptr [esi + 6ah], 0ffffh
        mov    word ptr [esi + 68h], di
        mov    byte ptr [esi + 5ch], 0
        push   esi
        call   sub_1_1400
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0D31:
        cmp    byte ptr [esi + 1cbh], 0
        je     SHORT loc_1_0D60
        mov    word ptr [esi + 6ah], 0ffffh
        mov    dword ptr [ebx + 6eh], 0
        mov    byte ptr [esi + 5ch], 0
        push   2
        push   esi
        db  0E8h, 03Dh, 00Bh, 0FFh, 0FFh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0D60:
        mov    word ptr [esi + 6ah], di
        mov    byte ptr [esi + 5ch], 0
        cmp    byte ptr [esi + 1a9h], 0
        je     SHORT loc_1_0D77
        mov    byte ptr [esi + 6dh], 1
        jmp    SHORT loc_1_0D81
loc_1_0D77:
        mov    byte ptr [esi + 6ch], 1
        push   esi
        call   sub_1_04F0
loc_1_0D81:
        push   esi
        call   sub_1_1400
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_0D90:
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
        call   sub_1_1D00
        cmp    dword ptr [ebp - 4], 6
        jae    SHORT loc_1_0DC0
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0DC0:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [esi + 1e6h]
        cmp    edi, eax
        jb     SHORT loc_1_0DD2
        mov    edi, eax
loc_1_0DD2:
        push   edi
        push   0
        push   ebx
        lea    eax, [esi + 9dh]
        push   eax
        call   sub_1_0E50
        cmp    edi, 0eh
        jae    SHORT loc_1_0E13
        cmp    edi, 5
        jbe    SHORT loc_1_0E3C
        push   0
        push   0
        push   0
        push   edi
        lea    eax, [esi + 9dh]
        push   eax
        push   eax
        push   esi
        mov    eax, dword ptr [esi + 1d4h]
        push   eax
        call   sub_1_1880
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0E13:
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0eh
        push   eax
        sub    edi, 0eh
        push   edi
        lea    eax, [esi + 0abh]
        push   eax
        push   0eh
        lea    eax, [esi + 9dh]
        push   eax
        push   eax
        push   esi
        mov    eax, dword ptr [esi + 1d4h]
        push   eax
        call   sub_1_1880
loc_1_0E3C:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0E50:
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
        call   sub_1_1D00
        cmp    dword ptr [ebp - 4], esi
        mov    ebx, dword ptr [ebp + 10h]
        je     SHORT loc_1_0EA4
loc_1_0E75:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D24
        mov    eax, dword ptr [ebp - 8]
        add    eax, edi
        cmp    ebx, eax
        jb     SHORT loc_1_0EA4
        mov    edi, eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D48
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_0E75
loc_1_0EA4:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_0EB5
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_0EB5:
        mov    eax, ebx
        sub    eax, edi
        add    dword ptr [ebp - 0ch], eax
        sub    edi, ebx
        add    dword ptr [ebp - 8], edi
        mov    edi, dword ptr [ebp + 14h]
loc_1_0EC4:
        mov    eax, dword ptr [ebp - 8]
        add    eax, esi
        cmp    eax, edi
        jbe    SHORT loc_1_0ED3
        mov    ebx, edi
        sub    ebx, esi
        jmp    SHORT loc_1_0ED6
loc_1_0ED3:
        mov    ebx, dword ptr [ebp - 8]
loc_1_0ED6:
        push   ebx
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, esi
        push   eax
        call   sub_1_1DBC
        add    esi, ebx
        cmp    esi, edi
        je     SHORT loc_1_0F12
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D48
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_0F1D
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D24
        jmp    SHORT loc_1_0EC4
loc_1_0F12:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_0F1D:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0F30:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 93h]
        mov    al, byte ptr [esi + 96h]
        sub    eax, ecx
        shl    eax, 8
        add    eax, dword ptr [esi + 8ah]
        mov    dword ptr [ebp - 8], eax
        push   4
        push   eax
        lea    eax, [esi + 99h]
        push   eax
        push   esi
        call   sub_1_0390
        or     al, al
        jne    SHORT loc_1_0F80
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_0F80:
        mov    bl, byte ptr [esi + 9ch]
        mov    al, byte ptr [esi + 96h]
        lea    eax, [ebx + eax + 1]
        mov    dl, byte ptr [esi + 94h]
        cmp    dl, al
        ja     SHORT loc_1_0FA2
        sub    al, dl
        add    al, byte ptr [esi + 93h]
loc_1_0FA2:
        mov    cl, byte ptr [esi + 9ah]
        cmp    cl, al
        je     SHORT loc_1_0FE4
        inc    al
        cmp    dl, al
        jne    SHORT loc_1_0FB8
        mov    al, byte ptr [esi + 93h]
loc_1_0FB8:
        cmp    cl, al
        je     SHORT loc_1_0FE4
        cmp    byte ptr [esi + 93h], cl
        ja     SHORT loc_1_0FD6
        cmp    dl, cl
        jbe    SHORT loc_1_0FD6
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0FD6:
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0FE4:
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 9bh]
        xor    eax, eax
        mov    al, bl
        shl    eax, 8
        lea    edi, [ecx + eax - 4]
        mov    dword ptr [esi + 199h], edi
        cmp    byte ptr [esi + 4ah], 0
        jne    SHORT loc_1_1058
        mov    ebx, dword ptr [esi + 1e6h]
        cmp    ebx, edi
        jb     SHORT loc_1_100F
        mov    ebx, edi
loc_1_100F:
        push   ebx
        mov    eax, dword ptr [ebp - 8]
        add    eax, 4
        push   eax
        lea    eax, [esi + 9dh]
        push   eax
        push   esi
        call   sub_1_0390
        or     al, al
        jne    SHORT loc_1_1050
        push   1
        push   1
        push   0aa000003h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_1DA8
        add    esp, 10h
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_1050:
        lea    eax, [esi + 9dh]
        jmp    SHORT loc_1_106D
loc_1_1058:
        mov    ebx, edi
        cmp    ebx, 0fch
        jb     SHORT loc_1_1067
        mov    ebx, 0fch
loc_1_1067:
        mov    eax, dword ptr [ebp - 8]
        add    eax, 4
loc_1_106D:
        mov    dword ptr [ebp - 4], eax
        inc    dword ptr [esi + 1afh]
        cmp    ebx, 0eh
        jae    SHORT loc_1_10A0
        push   0
        push   0
        push   0
        push   ebx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   eax
        push   esi
        mov    eax, dword ptr [esi + 1d4h]
        push   eax
        call   sub_1_1880
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_10A0:
        sub    edi, 0eh
        push   edi
        sub    ebx, 0eh
        push   ebx
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0eh
        push   eax
        push   0eh
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   eax
        push   esi
        mov    eax, dword ptr [esi + 1d4h]
        push   eax
        call   sub_1_1880
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_10D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 0ch]
        add    edi, 0eh
        cmp    dword ptr [esi + 1f2h], 0
        je     SHORT loc_1_1158
        push   0
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 18h]
        mov    ebx, edi
        push   eax
        call   sub_1_1D00
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_1146
loc_1_1106:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D24
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   ebx
        mov    eax, dword ptr [esi + 1f2h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_0E50
        add    ebx, eax
        cmp    eax, dword ptr [ebp - 8]
        jb     SHORT loc_1_1146
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D48
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_1106
loc_1_1146:
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
loc_1_1158:
        mov    eax, dword ptr [esi + 199h]
        mov    ecx, dword ptr [ebp + 14h]
        lea    edx, [edi + ecx]
        cmp    edx, eax
        jbe    SHORT loc_1_1170
        sub    eax, edi
        mov    dword ptr [ebp - 1ch], eax
        jmp    SHORT loc_1_1173
        db  090h
loc_1_1170:
        mov    dword ptr [ebp - 1ch], ecx
loc_1_1173:
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [ebp - 10h], eax
        mov    ecx, dword ptr [esi + 8ah]
        xor    eax, eax
        xor    edx, edx
        mov    dl, byte ptr [esi + 93h]
        mov    al, byte ptr [esi + 96h]
        sub    eax, edx
        shl    eax, 8
        add    eax, ecx
        lea    edi, [edi + eax + 4]
        mov    eax, dword ptr [esi + 8eh]
        cmp    edi, eax
        jbe    SHORT loc_1_11A8
        sub    edi, eax
        add    edi, ecx
loc_1_11A8:
        push   0
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        call   sub_1_1D00
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D24
        mov    dword ptr [ebp - 0ch], 0
        cmp    dword ptr [ebp - 1ch], 0
        je     NEAR PTR loc_1_1267
loc_1_11E0:
        mov    ebx, dword ptr [ebp - 8]
        sub    ebx, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 10h], ebx
        jae    SHORT loc_1_11EE
        mov    ebx, dword ptr [ebp - 10h]
loc_1_11EE:
        mov    eax, dword ptr [esi + 8eh]
        lea    ecx, [ebx + edi]
        cmp    ecx, eax
        jbe    SHORT loc_1_11FF
        mov    ebx, eax
        sub    ebx, edi
loc_1_11FF:
        push   ebx
        push   edi
        mov    eax, dword ptr [ebp - 14h]
        add    eax, dword ptr [ebp - 0ch]
        push   eax
        push   esi
        call   sub_1_0390
        or     al, al
        je     SHORT loc_1_1280
        add    edi, ebx
        sub    dword ptr [ebp - 10h], ebx
        je     SHORT loc_1_1267
        cmp    dword ptr [esi + 8eh], edi
        jne    SHORT loc_1_1227
        mov    edi, dword ptr [esi + 8ah]
loc_1_1227:
        add    dword ptr [ebp - 0ch], ebx
        mov    eax, dword ptr [ebp - 0ch]
        cmp    eax, dword ptr [ebp - 8]
        jne    SHORT loc_1_125D
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D48
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_1267
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1D24
        mov    dword ptr [ebp - 0ch], 0
loc_1_125D:
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_1_11E0
loc_1_1267:
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp + 1ch]
        sub    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 049h, 000h
loc_1_1280:
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp + 1ch]
        sub    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ecx], eax
        push   2
        push   1
        push   0aa000003h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_1DA8
        add    esp, 10h
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Bh, 0FFh
loc_1_12B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [eax + 8]
        lea    ebx, [edi + 24h]
        cmp    byte ptr [eax + 20h], 0
        je     SHORT loc_1_12E0
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_12E0:
        cmp    byte ptr [esi + 1cbh], 0
        je     SHORT loc_1_12F7
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_12F7:
        mov    eax, dword ptr [ebp + 8]
        push   0
        inc    dword ptr [eax + 1ch]
        inc    dword ptr [esi + 30h]
        mov    dword ptr [ebx + 4], eax
        lea    eax, [ebp - 8]
        mov    dword ptr [ebx], 0
        push   eax
        push   0
        push   0
        push   edi
        call   sub_1_1D00
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1D24
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 71h], 2
        je     SHORT loc_1_1339
        mov    byte ptr [ebx + 8], 0
        jmp    SHORT loc_1_134C
loc_1_1339:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [esi + 1d4h]
        push   eax
        call   sub_1_1B00
        mov    byte ptr [ebx + 8], al
loc_1_134C:
        cmp    dword ptr [esi + 5eh], 0
        je     SHORT loc_1_135A
        mov    eax, dword ptr [esi + 62h]
        mov    dword ptr [eax + 24h], edi
        jmp    SHORT loc_1_135D
loc_1_135A:
        mov    dword ptr [esi + 5eh], edi
loc_1_135D:
        mov    dword ptr [esi + 62h], edi
        mov    dword ptr [ebx], 0
        mov    eax, dword ptr [esi + 30h]
        dec    eax
        mov    dword ptr [esi + 30h], eax
        jne    SHORT loc_1_1397
        cmp    byte ptr [esi + 1cbh], 0
        jne    SHORT loc_1_1387
        cmp    dword ptr [esi + 1f6h], 0
        jne    SHORT loc_1_1387
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_1_1397
loc_1_1387:
        push   1
        add    esi, 202h
        push   esi
        call   sub_1_1CB2
        jmp    SHORT loc_1_13AE
loc_1_1397:
        cmp    dword ptr [esi + 5eh], 0
        je     SHORT loc_1_13AE
        movsx  eax, word ptr [esi + 66h]
        cmp    dword ptr [esi + eax*4 + 6eh], 0
        jne    SHORT loc_1_13AE
        push   esi
        call   sub_1_1400
loc_1_13AE:
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        sub    esi, esi
        mov    edx, dword ptr [ebp + 10h]
        cmp    edx, esi
        je     SHORT loc_1_13E1
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
loc_1_13D4:
        mov    bl, byte ptr [eax + esi]
        cmp    byte ptr [ecx + esi], bl
        jne    SHORT loc_1_13E9
        inc    esi
        cmp    esi, edx
        jb     SHORT loc_1_13D4
loc_1_13E1:
        sub    eax, eax
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_1_13E9:
        mov    eax, 0ffffffffh
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1400:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 66h]
        mov    word ptr [ebp - 2], ax
        movsx  eax, ax
        cmp    dword ptr [esi + eax*4 + 6eh], 0
        jne    NEAR PTR loc_1_150E
loc_1_1422:
        mov    edi, dword ptr [esi + 5eh]
        or     edi, edi
        je     NEAR PTR loc_1_150E
        mov    eax, dword ptr [edi + 24h]
        mov    dword ptr [esi + 5eh], eax
        movsx  ebx, word ptr [ebp - 2]
        mov    dword ptr [esi + ebx*4 + 6eh], 1
        lea    eax, [ebx + 1]
        mov    dword ptr [esi + ebx*4 + 76h], edi
        sub    edx, edx
        div    dword ptr [esi + 38h]
        mov    word ptr [esi + 66h], dx
        lea    eax, [esi + ebx*4 + 7eh]
        push   eax
        mov    ax, word ptr [ebp - 2]
        push   eax
        push   edi
        push   esi
        call   sub_1_0070
        mov    eax, dword ptr [esi + ebx*4 + 7eh]
        cmp    eax, 3ch
        jae    SHORT loc_1_1482
        mov    ecx, 3ch
        sub    ecx, eax
        push   ecx
        push   eax
        mov    ax, word ptr [ebp - 2]
        push   eax
        db  068h, 08Ch, 020h, 000h, 000h
        push   esi
        call   sub_1_0290
loc_1_1482:
        cmp    byte ptr [esi + 1cbh], 0
        jne    SHORT loc_1_14E0
        mov    dword ptr [esi + ebx*4 + 6eh], 2
        cmp    word ptr [esi + 6ah], -1
        jne    SHORT loc_1_14C1
        mov    ax, word ptr [esi + 68h]
        cmp    ax, word ptr [ebp - 2]
        jne    SHORT loc_1_14C1
        mov    word ptr [esi + 6ah], ax
        cmp    byte ptr [esi + 1a9h], 0
        je     SHORT loc_1_14B7
        mov    byte ptr [esi + 6dh], 1
        jmp    SHORT loc_1_14C1
loc_1_14B7:
        mov    byte ptr [esi + 6ch], 1
        push   esi
        call   sub_1_04F0
loc_1_14C1:
        mov    ax, word ptr [esi + 66h]
        mov    word ptr [ebp - 2], ax
        movsx  eax, ax
        cmp    dword ptr [esi + eax*4 + 6eh], 0
        je     NEAR PTR loc_1_1422
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_14E0:
        mov    eax, dword ptr [edi + 28h]
        mov    dword ptr [ebp - 8], eax
        movsx  eax, word ptr [ebp - 2]
        mov    dword ptr [esi + eax*4 + 6eh], 0
        push   0
        mov    eax, dword ptr [edi + 28h]
        push   edi
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_1C1A
        push   3
        push   esi
        db  0E8h, 088h, 003h, 0FFh, 0FFh
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 1ch]
loc_1_150E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1520:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 22eh], 0
        je     NEAR PTR loc_1_1620
        cmp    word ptr [esi + 6ah], -1
        je     NEAR PTR loc_1_162E
        mov    byte ptr [esi + 6ch], 0
        mov    byte ptr [esi + 22eh], 0
        mov    al, byte ptr [esi + 5dh]
        cmp    al, 0ah
        jae    SHORT loc_1_156A
        push   3
        push   1
        push   0aa000003h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_1DA8
        add    esp, 10h
        jmp    SHORT loc_1_156F
loc_1_156A:
        inc    al
        mov    byte ptr [esi + 5dh], al
loc_1_156F:
        push   esi
        call   0
        movsx  eax, word ptr [esi + 6ah]
        lea    eax, [esi + eax*4 + 76h]
        sub    edx, edx
        mov    ecx, dword ptr [eax]
        mov    dword ptr [eax], 0
        movsx  eax, word ptr [esi + 6ah]
        mov    dword ptr [esi + eax*4 + 6eh], 0
        movsx  eax, word ptr [esi + 6ah]
        inc    eax
        div    dword ptr [esi + 38h]
        mov    di, dx
        or     ecx, ecx
        je     SHORT loc_1_15DD
loc_1_15A2:
        mov    ebx, dword ptr [ecx + 28h]
        push   0
        push   ecx
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_1C1A
        movsx  eax, di
        lea    edx, [esi + eax*4]
        dec    dword ptr [ebx + 1ch]
        cmp    dword ptr [edx + 6eh], 2
        jne    SHORT loc_1_15DD
        mov    ecx, dword ptr [edx + 76h]
        mov    dword ptr [edx + 76h], 0
        mov    dword ptr [edx + 6eh], 0
        inc    eax
        sub    edx, edx
        div    dword ptr [esi + 38h]
        mov    di, dx
        or     ecx, ecx
        jne    SHORT loc_1_15A2
loc_1_15DD:
        mov    word ptr [esi + 6ah], 0ffffh
        mov    word ptr [esi + 68h], di
        mov    ecx, dword ptr [esi + 5eh]
        mov    dword ptr [esi + 5eh], 0
        or     ecx, ecx
        je     SHORT loc_1_1612
loc_1_15F5:
        mov    edi, dword ptr [ecx + 24h]
        mov    ebx, dword ptr [ecx + 28h]
        lea    eax, [ecx + 24h]
        push   0
        push   ecx
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_1C1A
        dec    dword ptr [ebx + 1ch]
        mov    ecx, edi
        or     edi, edi
        jne    SHORT loc_1_15F5
loc_1_1612:
        mov    edx, dword ptr [esi + 50h]
        add    edx, 0dh
        sub    al, al
        out    dx, al
        jmp    SHORT loc_1_162E
        db  08Dh, 049h, 000h
loc_1_1620:
        cmp    word ptr [esi + 6ah], -1
        je     SHORT loc_1_162E
        mov    byte ptr [esi + 22eh], 1
loc_1_162E:
        push   7d0h
        add    esi, 222h
        push   esi
        call   sub_1_1CB2
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1650:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        inc    dword ptr [esi + 30h]
        cmp    byte ptr [esi + 1cbh], 0
        jne    NEAR PTR loc_1_1706
        cmp    dword ptr [esi + 1feh], 0
        jne    NEAR PTR loc_1_1706
loc_1_1676:
        mov    edi, dword ptr [esi + 1f6h]
        or     edi, edi
        je     SHORT loc_1_16E1
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 1f6h], eax
        cmp    dword ptr [esi + 1fah], edi
        jne    SHORT loc_1_169A
        mov    dword ptr [esi + 1fah], 0
loc_1_169A:
        mov    dword ptr [esi + 1feh], edi
        mov    eax, dword ptr [edi + 8]
        cmp    eax, 4
        je     SHORT loc_1_16B5
        cmp    eax, 0ah
        je     SHORT loc_1_16B5
        push   esi
        db  0E8h, 01Dh, 00Dh, 0FFh, 0FFh
        jmp    SHORT loc_1_16BB
loc_1_16B5:
        push   esi
        db  0E8h, 0A5h, 00Dh, 0FFh, 0FFh
loc_1_16BB:
        mov    ebx, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi + 8]
        cmp    ecx, 8
        je     SHORT loc_1_16D5
        cmp    ecx, 4
        je     SHORT loc_1_16D5
        push   eax
        push   edi
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_1C2C
loc_1_16D5:
        dec    dword ptr [ebx + 1ch]
        cmp    byte ptr [esi + 1cbh], 0
        je     SHORT loc_1_1676
loc_1_16E1:
        mov    dword ptr [esi + 1feh], 0
        cmp    byte ptr [esi + 1cbh], 0
        je     SHORT loc_1_1706
        dec    dword ptr [esi + 30h]
        push   1
        push   esi
        db  0E8h, 091h, 001h, 0FFh, 0FFh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_1_1706:
        mov    edi, dword ptr [esi + 2ch]
        or     edi, edi
        je     SHORT loc_1_1771
        sub    ebx, ebx
        nop
loc_1_1710:
        cmp    dword ptr [edi + 1ch], 0
        jbe    SHORT loc_1_1720
        mov    edi, dword ptr [edi + 0ch]
        mov    ebx, edi
        jmp    SHORT loc_1_175B
        db  08Dh, 049h, 000h
loc_1_1720:
        mov    eax, dword ptr [esi + 1e6h]
        cmp    dword ptr [edi + 18h], eax
        jne    SHORT loc_1_1731
        push   esi
        db  0E8h, 0AFh, 0F2h, 0FEh, 0FFh
loc_1_1731:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1C14
        or     ebx, ebx
        mov    eax, dword ptr [edi + 0ch]
        lea    ecx, [edi + 0ch]
        je     SHORT loc_1_174A
        mov    dword ptr [ebx + 0ch], eax
        jmp    SHORT loc_1_174D
loc_1_174A:
        mov    dword ptr [esi + 2ch], eax
loc_1_174D:
        mov    eax, edi
        mov    edi, dword ptr [ecx]
        push   0
        push   75h
        push   eax
        call   sub_1_1CDC
loc_1_175B:
        or     edi, edi
        jne    SHORT loc_1_1710
        cmp    dword ptr [esi + 2ch], 0
        jne    SHORT loc_1_1771
        cmp    dword ptr [esi + 28h], 0
        jne    SHORT loc_1_1771
        push   esi
        call   0
loc_1_1771:
        mov    eax, dword ptr [esi + 30h]
        dec    eax
        mov    dword ptr [esi + 30h], eax
        jne    SHORT loc_1_17A7
        cmp    byte ptr [esi + 1cbh], 0
        jne    SHORT loc_1_1792
        cmp    dword ptr [esi + 1f6h], 0
        jne    SHORT loc_1_1792
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_1_17A7
loc_1_1792:
        push   1
        add    esi, 202h
        push   esi
        call   sub_1_1CB2
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_1_17A7:
        cmp    dword ptr [esi + 5eh], 0
        je     SHORT loc_1_17BE
        movsx  eax, word ptr [esi + 66h]
        cmp    dword ptr [esi + eax*4 + 6eh], 0
        jne    SHORT loc_1_17BE
        push   esi
        call   sub_1_1400
loc_1_17BE:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_17D0:
        db  0A0h, 018h, 021h, 000h, 000h
        cmp    al, 0
        jne    SHORT loc_1_17F5
        db  0FEh, 005h, 018h, 021h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     SHORT loc_1_17F7
        push   0
        push   0
        db  0E8h, 010h, 0E8h, 0FEh, 0FFh
        cmp    eax, 0
        jne    SHORT loc_1_17F7
loc_1_17F5:
        clc
        ret
loc_1_17F7:
        stc
        ret
ELNKII_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_1808
        db  0FFh, 024h, 085h, 00Ah, 018h, 000h, 000h
loc_1_1808:
        clc
        ret
        db  0D0h, 017h, 000h, 000h, 008h, 018h, 000h, 000h, 008h, 018h, 000h, 000h, 008h, 018h, 000h, 000h
        db  008h, 018h, 000h, 000h, 008h, 018h, 000h, 000h, 008h, 018h, 000h, 000h, 008h, 018h, 000h, 000h
        db  008h, 018h, 000h, 000h, 008h, 018h, 000h, 000h, 008h, 018h, 000h, 000h, 008h, 018h, 000h, 000h
        db  008h, 018h, 000h, 000h, 008h, 018h, 000h, 000h, 008h, 018h, 000h, 000h, 008h, 018h, 000h, 000h
        db  008h, 018h, 000h, 000h, 008h, 018h, 000h, 000h, 008h, 018h, 000h, 000h, 008h, 018h, 000h, 000h
        db  008h, 018h, 000h, 000h, 008h, 018h, 000h, 000h, 008h, 018h, 000h, 000h, 008h, 018h, 000h, 000h
        db  008h, 018h, 000h, 000h, 008h, 018h, 000h, 000h, 0D0h, 017h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_1_1880:
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
        call   sub_1_18B0
        pop    ebp
        ret    20h
        db  08Dh, 064h, 024h, 000h
sub_1_18B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 18h], 0eh
        jb     NEAR PTR loc_1_1950
        mov    edx, dword ptr [ebp + 10h]
        mov    al, byte ptr [edx]
        test   al, 1
        je     SHORT loc_1_18F0
        cmp    dword ptr [edx + 2], -1
        jne    SHORT loc_1_18E5
        cmp    al, 0ffh
        jne    SHORT loc_1_18E5
        cmp    byte ptr [edx + 1], 0ffh
        jne    SHORT loc_1_18E5
        mov    dword ptr [ebp - 4], 8
        jmp    SHORT loc_1_1957
loc_1_18E5:
        mov    dword ptr [ebp - 4], 2
        jmp    SHORT loc_1_1957
        db  08Bh, 0FFh
loc_1_18F0:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0ch], 20h
        je     SHORT loc_1_1940
        mov    edx, dword ptr [ebp + 10h]
        lea    ecx, [esi + 2ch]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_1916
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_191D
loc_1_1916:
        mov    dword ptr [ebp - 10h], 1
loc_1_191D:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_1930
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_195A
        db  08Dh, 064h, 024h, 000h
loc_1_1930:
        mov    dword ptr [ebp - 4], 20h
        jmp    SHORT loc_1_195A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1940:
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_195A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1950:
        mov    dword ptr [ebp - 4], 20h
loc_1_1957:
        mov    esi, dword ptr [ebp + 8]
loc_1_195A:
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     NEAR PTR loc_1_1A51
loc_1_1965:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, ebx
        and    eax, dword ptr [ebp - 4]
        test   al, 9
        jne    SHORT loc_1_19BD
        test   al, 2
        je     SHORT loc_1_19A9
        lea    eax, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 0C6h, 012h, 0FFh, 0FFh
        or     al, al
        je     SHORT loc_1_19A9
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 19h]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        test   dword ptr [eax + ecx*4], edx
        jne    SHORT loc_1_19BD
loc_1_19A9:
        test   byte ptr [ebp - 4], 2
        je     SHORT loc_1_19B4
        test   bl, 4
        jne    SHORT loc_1_19BD
loc_1_19B4:
        test   bl, 20h
        je     NEAR PTR loc_1_1A46
loc_1_19BD:
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
        call   sub_1_1C32
        mov    byte ptr [edi + 18h], 1
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_1A46
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_19FF
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_19FF:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_1A10
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_1A16
        db  08Bh, 0FFh
loc_1_1A10:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_1A16:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_1A25
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_1C38
loc_1_1A25:
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
        call   sub_1_1CDC
        mov    edi, ebx
        jmp    SHORT loc_1_1A49
loc_1_1A46:
        mov    edi, dword ptr [edi + 10h]
loc_1_1A49:
        or     edi, edi
        jne    NEAR PTR loc_1_1965
loc_1_1A51:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1A60:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1A70
        pop    ebp
        ret    4
sub_1_1A70:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     SHORT loc_1_1AEE
loc_1_1A80:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_1AE7
        mov    byte ptr [edi + 18h], 0
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_1C38
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_1AE7
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_1AAC
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_1AAC:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_1AC0
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_1AC6
        db  005h, 000h, 000h, 000h, 000h
loc_1_1AC0:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_1AC6:
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
        call   sub_1_1CDC
        mov    edi, ebx
        jmp    SHORT loc_1_1AEA
loc_1_1AE7:
        mov    edi, dword ptr [edi + 10h]
loc_1_1AEA:
        or     edi, edi
        jne    SHORT loc_1_1A80
loc_1_1AEE:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1B00:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0ch]
        test   al, 20h
        je     SHORT loc_1_1B15
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_1B15:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     SHORT loc_1_1B51
        cmp    dword ptr [ecx + 2], -1
        jne    SHORT loc_1_1B42
        cmp    bl, 0ffh
        jne    SHORT loc_1_1B42
        cmp    byte ptr [ecx + 1], 0ffh
        jne    SHORT loc_1_1B42
        test   al, 8
        je     SHORT loc_1_1B3B
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_1B3B:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_1B42:
        test   al, 4
        jne    SHORT loc_1_1B4A
        test   al, 2
        je     SHORT loc_1_1B69
loc_1_1B4A:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_1B51:
        mov    eax, dword ptr [edx + 2eh]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_1B69
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 2ch], ax
        jne    SHORT loc_1_1B69
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_1B69:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
sub_1_1B70:
        push   esi
        mov    ecx, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        cld
        rep outsb
        pop    esi
        ret    0ch
sub_1_1B84:
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
sub_1_1BAD:
        push   edi
        mov    ecx, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        cld
        rep insb
        pop    edi
        ret    0ch
sub_1_1BC1:
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
sub_1_1BEA:
        db  0CDh, 020h, 031h, 080h, 028h, 000h
sub_1_1BF0:
        db  0CDh, 020h, 032h, 080h, 028h, 000h
sub_1_1BF6:
        db  0CDh, 020h, 033h, 080h, 028h, 000h
sub_1_1BFC:
        db  0CDh, 020h, 034h, 080h, 028h, 000h
sub_1_1C02:
        db  0CDh, 020h, 035h, 080h, 028h, 000h
sub_1_1C08:
        db  0CDh, 020h, 036h, 080h, 028h, 000h
        db  0CDh, 020h, 037h, 080h, 028h, 000h
sub_1_1C14:
        db  0CDh, 020h, 038h, 080h, 028h, 000h
sub_1_1C1A:
        db  0CDh, 020h, 039h, 080h, 028h, 000h
        db  0CDh, 020h, 03Ah, 080h, 028h, 000h
sub_1_1C26:
        db  0CDh, 020h, 03Bh, 080h, 028h, 000h
sub_1_1C2C:
        db  0CDh, 020h, 03Ch, 080h, 028h, 000h
sub_1_1C32:
        db  0CDh, 020h, 03Dh, 080h, 028h, 000h
sub_1_1C38:
        db  0CDh, 020h, 03Eh, 080h, 028h, 000h
sub_1_1C3E:
        db  0CDh, 020h, 03Fh, 080h, 028h, 000h
sub_1_1C44:
        db  0CDh, 020h, 040h, 080h, 028h, 000h
        db  0CDh, 020h, 041h, 080h, 028h, 000h
        db  0CDh, 020h, 009h, 080h, 028h, 000h
        db  0CDh, 020h, 008h, 080h, 028h, 000h
sub_1_1C5C:
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
sub_1_1CB2:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
sub_1_1CB8:
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
sub_1_1CBE:
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
sub_1_1CC4:
        db  0CDh, 020h, 010h, 080h, 028h, 000h
sub_1_1CCA:
        db  0CDh, 020h, 011h, 080h, 028h, 000h
sub_1_1CD0:
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_1CD6:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_1CDC:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_1D00:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_1D24:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_1D48:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
        db  0CDh, 020h, 028h, 080h, 028h, 000h
sub_1_1D54:
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_1D5A:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_1D60:
        db  0CDh, 020h, 007h, 080h, 028h, 000h
sub_1_1D66:
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
sub_1_1DA2:
        db  0CDh, 020h, 046h, 080h, 028h, 000h
sub_1_1DA8:
        db  0CDh, 020h, 045h, 080h, 028h, 000h
        db  0CDh, 020h, 043h, 080h, 028h, 000h
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        add    byte ptr [eax], al
sub_1_1DBC:
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
sub_1_1DDC:
        push   esi
        push   edi
        pushfd
        mov    ecx, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        cmp    esi, edi
        jae    SHORT loc_1_1DFC
        or     ecx, ecx
        je     SHORT loc_1_1E12
        std
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    SHORT loc_1_1E10
loc_1_1DFC:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     SHORT loc_1_1E10
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd
        mov    cl, al
loc_1_1E10:
        rep movsb
loc_1_1E12:
        popfd
        pop    edi
        pop    esi
        ret    0ch
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_1E6C:
        loopne loc_1_1E70
        add    byte ptr [eax], al
loc_1_1E70:
        mov    al, byte ptr [80000002h]
        add    al, byte ptr [eax]
        add    byte ptr [eax + 2], dl
        add    byte ptr [eax], al
        push   eax
        add    eax, dword ptr [eax]
        add    byte ptr [eax], dh
        add    eax, dword ptr [eax]
        add    byte ptr [eax], dl
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [edx], al
        add    eax, dword ptr [eax + 6b6e6c45h]
        imul   ebp, dword ptr [ecx], 6e6c4500h
        imul   ebp, dword ptr [ecx + 69h], 0
        add    byte ptr [ecx + 4fh], cl
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [ecx + 4fh], cl
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
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
        dec    esp
        dec    ecx
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
        dec    esp
        dec    ecx
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
        dec    ebp
        inc    ebp
        dec    ebp
        dec    edi
        push   edx
        pop    ecx
        dec    ebp
        inc    ecx
        push   eax
        push   eax
        inc    ebp
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ebp
        inc    ebp
        dec    ebp
        dec    edi
        push   edx
        pop    ecx
        dec    ebp
        inc    ecx
        push   eax
        push   eax
        inc    ebp
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   esp
        push   edx
        inc    ecx
        dec    esi
        push   ebx
        inc    ebx
        inc    ebp
        dec    ecx
        push   esi
        inc    ebp
        push   edx
        db  000h
loc_1_1F3C:
        push   esp
        push   edx
        inc    ecx
        dec    esi
        push   ebx
        inc    ebx
        inc    ebp
        dec    ecx
        push   esi
        inc    ebp
        push   edx
        add    byte ptr [ebp + 58h], al
        push   esp
        inc    ebp
        push   edx
        dec    esi
        inc    ecx
        dec    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1F54:
        inc    ebp
        pop    eax
        push   esp
        inc    ebp
        push   edx
        dec    esi
        inc    ecx
        dec    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1F60:
        inc    ebp
        je     SHORT loc_1_1FCB
        db  065h, 072h, 06Ch
        imul   ebp, dword ptr [esi + 6bh], 20494920h
        inc    ecx
        popad
        jo     SHORT loc_1_1FE6
        jb     NEAR PTR loc_1_1FA3
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1FA3:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1FCB:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_1FD8:
        loopne loc_1_1FDC
        add    byte ptr [eax], al
loc_1_1FDC:
        mov    al, byte ptr [80000002h]
        add    al, byte ptr [eax]
        add    byte ptr [eax + 2], dl
loc_1_1FE6:
        add    byte ptr [eax], al
        push   eax
        add    eax, dword ptr [eax]
        add    byte ptr [eax], dh
        add    eax, dword ptr [eax]
        add    byte ptr [eax], dl
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        or     byte ptr [eax + eax], 0
        ror    byte ptr [eax + eax], 0
        or     byte ptr [0dc00000h], 0
loc_1_2008:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2014:
        add    al, byte ptr [esi + eax]
        db  000h
loc_1_2018:
        inc    eax
        inc    eax
        add    byte ptr [eax], al
loc_1_201C:
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2020:
        inc    eax
        add    byte ptr [eax], al
        xchg   edx, eax
loc_1_2024:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_2028:
        add    dword ptr [ecx], eax
        add    byte ptr [eax], al
loc_1_202C:
        add    dword ptr [edx], eax
        add    dword ptr [eax], eax
loc_1_2030:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2036:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_203C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_206C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2074:
        add    byte ptr [eax], cl
        add    al, 0ch
        add    cl, byte ptr [edx]
        push   es
        push   cs
        add    dword ptr [ecx], ecx
        add    eax, 70b030dh
        ud0
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_208C:
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
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 00Ah, 000h, 000h, 045h, 04Ch, 04Eh, 04Bh
        db  049h, 049h, 020h, 020h, 000h, 040h, 000h, 054h, 0F9h, 017h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
_LTEXT ends
        end
