; ibmtok4_lcode_01.asm -- IBMTOK4 object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_0000:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   edi
        xor    ebx, ebx
        mov    dword ptr [edx + 0e8h], ebx
        mov    dword ptr [edx + 0ech], ebx
        lea    edi, [edx + 179h]
        xor    eax, eax
        mov    ecx, 80h
        mov    dword ptr [edx + 0f0h], ebx
        mov    dword ptr [edx + 0f4h], ebx
        mov    dword ptr [edx + 0cfh], ebx
        rep stosd
        mov    ecx, 1
        mov    dword ptr [edx + 8ah], ebx
        mov    dword ptr [edx + 8eh], ebx
        mov    byte ptr [edx + 0cch], cl
        mov    eax, 0ffffh
        mov    byte ptr [edx + 0cdh], cl
        mov    word ptr [edx + 0d3h], ax
        mov    byte ptr [edx + 49h], bl
        mov    word ptr [edx + 0d5h], ax
        pop    edi
        mov    byte ptr [edx + 4bh], bl
        mov    byte ptr [edx + 0ceh], bl
        pop    ebx
        mov    byte ptr [edx + 0dbh], cl
        pop    ebp
        ret    4
loc_1_0080:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   esi
        mov    dword ptr [ebp - 8], edx
        push   edi
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [ebp + 18h]
        cmp    edx, eax
        je     SHORT loc_1_00AE
        mov    ecx, dword ptr [ebp + 14h]
loc_1_009D:
        cmp    dword ptr [ecx], 1
        je     SHORT loc_1_00AA
        add    ecx, 4
        inc    edx
        cmp    edx, eax
        jb     SHORT loc_1_009D
loc_1_00AA:
        cmp    edx, eax
        jne    SHORT loc_1_00BC
loc_1_00AE:
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_00BC:
        mov    eax, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 20h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        xor    ebx, ebx
        push   ecx
        mov    dword ptr [eax], edx
        push   ebx
        lea    eax, [ebp - 4]
        push   96h
        inc    dword ptr [esi + 5eh]
        push   eax
        call   sub_1_3F46
        test   eax, eax
        jne    NEAR PTR loc_1_028D
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp + 1ch]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edx + 10h], ecx
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edx + 14h], ebx
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [edx + 1dh], bl
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edx + 8], esi
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [edx + 1ch], bl
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0ch
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    ecx, dword ptr [eax + 8]
        mov    edx, dword ptr [ecx + 86h]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0136:
        test   al, al
        jne    SHORT loc_1_016F
        push   4
        mov    eax, dword ptr [esi + 82h]
        push   1
        push   2
        push   0aa000002h
        push   eax
        call   sub_1_4018
        add    esp, 14h
        mov    eax, dword ptr [ebp - 4]
        push   ebx
        push   96h
        push   eax
        call   sub_1_3F4C
        mov    dword ptr [ebp - 8], 0c0000001h
        jmp    NEAR PTR loc_1_025F
loc_1_016F:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, 0f0h
        lea    edx, [esi + 62h]
        mov    dword ptr [ecx + 18h], eax
        mov    dword ptr [esi + 165h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 66h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esi + 66h]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [eax], edx
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 66h], ecx
        mov    ecx, dword ptr [ebp - 4]
        inc    dword ptr [ecx + 14h]
        cmp    byte ptr [esi + 170h], dl
        je     NEAR PTR loc_1_025F
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], 103h
        mov    byte ptr [eax + 1ch], 1
        cmp    byte ptr [esi + 171h], dl
        jne    NEAR PTR loc_1_025F
        mov    byte ptr [esi + 171h], 1
        xor    ebx, ebx
        xor    eax, eax
        mov    dword ptr [esi + 15ch], 3
        mov    ecx, 0fh
        mov    byte ptr [esi + 37h], dl
        mov    bx, word ptr [esi + 0b6h]
        add    ebx, dword ptr [esi + 0b2h]
        mov    edi, ebx
        rep stosd
        mov    byte ptr [ebx], 3
        cmp    byte ptr [esi + 2bh], 1
        sbb    eax, eax
        and    ax, 1000h
        mov    al, 1
        mov    word ptr [ebx + 8], ax
loc_1_020E:
        mov    al, byte ptr [esi + edx + 4eh]
        inc    edx
        cmp    edx, 6
        mov    byte ptr [ebx + edx + 9], al
        jb     SHORT loc_1_020E
        mov    ax, word ptr [esi + 0e4h]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx + 18h], cx
        mov    ax, word ptr [esi + 0e6h]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx + 1ah], cx
        mov    ax, word ptr [esi + 0deh]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx + 1ch], cx
        mov    al, byte ptr [esi + 0dch]
        mov    byte ptr [ebx + 1eh], al
        mov    edx, dword ptr [esi + 0aeh]
        mov    byte ptr [edx + 1e4bh], 20h
loc_1_025F:
        mov    eax, dword ptr [esi + 5eh]
        dec    eax
        mov    dword ptr [esi + 5eh], eax
        jne    SHORT loc_1_0281
        cmp    byte ptr [esi + 160h], 0
        jne    SHORT loc_1_027B
        mov    eax, dword ptr [esi + 6ah]
        sub    eax, esi
        cmp    eax, 6ah
        je     SHORT loc_1_0281
loc_1_027B:
        push   esi
        call   sub_1_35C8
loc_1_0281:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_028D:
        push   5
        mov    eax, dword ptr [esi + 82h]
        push   1
        push   2
        push   0aa000002h
        push   eax
        call   sub_1_4018
        add    esp, 14h
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_02C0:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, ecx
        mov    esi, dword ptr [ecx + 62h]
        sub    eax, esi
        cmp    eax, -62h
        je     SHORT loc_1_02E9
loc_1_02D5:
        mov    eax, dword ptr [esi + 18h]
        cmp    eax, edx
        jbe    SHORT loc_1_02DE
        mov    edx, eax
loc_1_02DE:
        mov    esi, dword ptr [esi]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, -62h
        jne    SHORT loc_1_02D5
loc_1_02E9:
        test   edx, edx
        jne    SHORT loc_1_02F2
        mov    edx, 0f0h
loc_1_02F2:
        pop    esi
        mov    dword ptr [ecx + 165h], edx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_0300:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 8]
        inc    dword ptr [esi + 5eh]
        cmp    byte ptr [ebx + 1dh], 0
        jne    NEAR PTR loc_1_0488
        push   0
        inc    dword ptr [ebx + 14h]
        mov    eax, dword ptr [ebx + 0ch]
        mov    ecx, dword ptr [esi + 86h]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_032E:
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_03AF
        cmp    dword ptr [ebx + 14h], 2
        jne    SHORT loc_1_037B
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx]
        mov    edx, dword ptr [ebx + 4]
        mov    dword ptr [eax + 4], edx
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx]
        mov    edx, dword ptr [ebx + 4]
        mov    dword ptr [eax + 4], edx
        mov    ecx, dword ptr [esi + 165h]
        cmp    dword ptr [ebx + 18h], ecx
        jne    SHORT loc_1_0369
        push   esi
        call   sub_1_02C0
loc_1_0369:
        push   0
        push   96h
        push   ebx
        call   sub_1_3F4C
        jmp    NEAR PTR loc_1_048D
loc_1_037B:
        mov    byte ptr [ebx + 1dh], 1
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx]
        mov    edx, dword ptr [ebx + 4]
        lea    ecx, [esi + 6ah]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [ebx], ecx
        mov    edi, 103h
        mov    edx, dword ptr [esi + 6eh]
        mov    dword ptr [ebx + 4], edx
        mov    edx, dword ptr [esi + 6eh]
        mov    dword ptr [edx], ebx
        mov    dword ptr [esi + 6eh], ebx
        sub    dword ptr [ebx + 14h], 2
        jmp    NEAR PTR loc_1_048D
loc_1_03AF:
        cmp    edi, 103h
        jne    SHORT loc_1_03FC
        cmp    byte ptr [esi + 0cch], 0
        je     SHORT loc_1_03CD
        mov    byte ptr [esi + 0cch], 0
        push   esi
        call   sub_1_2DD8
loc_1_03CD:
        mov    byte ptr [ebx + 1dh], 1
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx]
        mov    edx, dword ptr [ebx + 4]
        lea    ecx, [esi + 6ah]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [ebx], ecx
        mov    edx, dword ptr [esi + 6eh]
        mov    dword ptr [ebx + 4], edx
        mov    edx, dword ptr [esi + 6eh]
        mov    dword ptr [edx], ebx
        mov    dword ptr [esi + 6eh], ebx
        sub    dword ptr [ebx + 14h], 2
        jmp    NEAR PTR loc_1_048D
loc_1_03FC:
        cmp    edi, 0c001000eh
        jne    SHORT loc_1_0434
        mov    byte ptr [ebx + 1dh], 1
        mov    ecx, dword ptr [ebx + 4]
        mov    edi, 103h
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx]
        mov    edx, dword ptr [ebx + 4]
        lea    ecx, [esi + 6ah]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [ebx], ecx
        mov    edx, dword ptr [esi + 6eh]
        mov    dword ptr [ebx + 4], edx
        mov    edx, dword ptr [esi + 6eh]
        mov    dword ptr [edx], ebx
        mov    dword ptr [esi + 6eh], ebx
        dec    dword ptr [ebx + 14h]
        jmp    SHORT loc_1_048D
loc_1_0434:
        cmp    edi, 0c001000dh
        jne    SHORT loc_1_046C
        mov    byte ptr [ebx + 1dh], 1
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx]
        mov    edx, dword ptr [ebx + 4]
        lea    ecx, [esi + 72h]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [ebx], ecx
        mov    edi, 103h
        mov    edx, dword ptr [esi + 76h]
        mov    dword ptr [ebx + 4], edx
        mov    edx, dword ptr [esi + 76h]
        mov    dword ptr [edx], ebx
        mov    dword ptr [esi + 76h], ebx
        dec    dword ptr [ebx + 14h]
        jmp    SHORT loc_1_048D
loc_1_046C:
        push   1
        mov    eax, dword ptr [esi + 82h]
        push   0ah
        push   2
        push   0aa000006h
        push   eax
        call   sub_1_4018
        add    esp, 14h
        jmp    SHORT loc_1_048D
loc_1_0488:
        mov    edi, 0c0010002h
loc_1_048D:
        mov    eax, dword ptr [esi + 5eh]
        dec    eax
        mov    dword ptr [esi + 5eh], eax
        jne    SHORT loc_1_04AF
        cmp    byte ptr [esi + 160h], 0
        jne    SHORT loc_1_04A9
        mov    eax, dword ptr [esi + 6ah]
        sub    eax, esi
        cmp    eax, 6ah
        je     SHORT loc_1_04AF
loc_1_04A9:
        push   esi
        call   sub_1_35C8
loc_1_04AF:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_04C0:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ecx + 8]
        xor    edi, edi
        mov    edx, dword ptr [ebp + 0ch]
        inc    dword ptr [esi + 5eh]
        mov    eax, dword ptr [edx + 10h]
        test   eax, eax
        jne    SHORT loc_1_04E7
        push   edx
        push   ecx
        push   esi
        call   sub_1_0970
        mov    edi, eax
        jmp    SHORT loc_1_055F
loc_1_04E7:
        cmp    eax, 1
        jne    SHORT loc_1_055A
        cmp    byte ptr [esi + 175h], 0
        je     SHORT loc_1_04FC
        mov    edi, 0c0010008h
        jmp    SHORT loc_1_055F
loc_1_04FC:
        cmp    byte ptr [esi + 172h], 0
        jne    SHORT loc_1_051E
        cmp    byte ptr [ecx + 1dh], 0
        jne    SHORT loc_1_0517
        push   edx
        push   ecx
        push   esi
        call   sub_1_09C0
        mov    edi, eax
        jmp    SHORT loc_1_055F
loc_1_0517:
        mov    edi, 0c0010002h
        jmp    SHORT loc_1_055F
loc_1_051E:
        cmp    byte ptr [esi + 160h], 0
        je     SHORT loc_1_052E
        mov    edi, 0c001000dh
        jmp    SHORT loc_1_055F
loc_1_052E:
        cmp    byte ptr [esi + 170h], 0
        je     SHORT loc_1_053E
        mov    edi, 0c0000001h
        jmp    SHORT loc_1_055F
loc_1_053E:
        push   3
        mov    eax, dword ptr [esi + 82h]
        push   0bh
        push   2
        push   0aa000006h
        push   eax
        call   sub_1_4018
        add    esp, 14h
        jmp    SHORT loc_1_055F
loc_1_055A:
        mov    edi, 10001h
loc_1_055F:
        mov    eax, dword ptr [esi + 5eh]
        dec    eax
        mov    dword ptr [esi + 5eh], eax
        jne    SHORT loc_1_0581
        cmp    byte ptr [esi + 160h], 0
        jne    SHORT loc_1_057B
        mov    eax, dword ptr [esi + 6ah]
        sub    eax, esi
        cmp    eax, 6ah
        je     SHORT loc_1_0581
loc_1_057B:
        push   esi
        call   sub_1_35C8
loc_1_0581:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0590:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        lea    ecx, [ebp - 8]
        push   esi
        mov    edx, dword ptr [ebp + 10h]
        push   edi
        mov    esi, 4
        xor    edi, edi
        mov    eax, edx
        mov    dword ptr [ebp - 0ch], edi
        cmp    eax, 2010101h
        mov    dword ptr [ebp - 10h], 1
        ja     SHORT loc_1_05D0
        je     NEAR PTR loc_1_081B
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_1_05F1
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_05D0:
        cmp    eax, 2010102h
        je     NEAR PTR loc_1_0852
        cmp    eax, 2010103h
        je     NEAR PTR loc_1_0886
        cmp    eax, 2010104h
        je     NEAR PTR loc_1_08B4
loc_1_05F1:
        mov    edi, 0c00000bbh
        jmp    NEAR PTR loc_1_08E8
loc_1_05FB:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_1_0610
        db  0B9h, 000h, 000h, 000h, 000h
        mov    esi, 5ch
        jmp    NEAR PTR loc_1_08E8
loc_1_0610:
        db  0B9h, 000h, 000h, 000h, 000h
        mov    esi, 84h
        jmp    NEAR PTR loc_1_08E8
loc_1_061F:
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 160h], 0
        je     SHORT loc_1_0634
        mov    dword ptr [ebp - 0ch], 2
        jmp    SHORT loc_1_0668
loc_1_0634:
        cmp    byte ptr [ebx + 16fh], 0
        jne    SHORT loc_1_0661
        cmp    byte ptr [ebx + 171h], 0
        jne    SHORT loc_1_0661
        cmp    byte ptr [ebx + 172h], 0
        je     SHORT loc_1_0658
        mov    dword ptr [ebp - 0ch], 4
        jmp    SHORT loc_1_0668
loc_1_0658:
        mov    dword ptr [ebp - 0ch], 0
        jmp    SHORT loc_1_0668
loc_1_0661:
        mov    dword ptr [ebp - 0ch], 1
loc_1_0668:
        lea    ecx, [ebp - 0ch]
        mov    esi, 4
        jmp    NEAR PTR loc_1_08E8
loc_1_0675:
        lea    ecx, [ebp - 10h]
        mov    esi, 4
        jmp    NEAR PTR loc_1_08E8
loc_1_0682:
        mov    dword ptr [ebp - 8], 0f0h
        jmp    NEAR PTR loc_1_08E8
loc_1_068E:
        mov    ebx, dword ptr [ebp + 8]
        cmp    edx, 10106h
        mov    eax, dword ptr [ebx + 0e0h]
        mov    dword ptr [ebp - 8], eax
        jne    NEAR PTR loc_1_08E8
        sub    dword ptr [ebp - 8], 0eh
        jmp    NEAR PTR loc_1_08E8
loc_1_06AF:
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 35h], 1
        sbb    eax, eax
        and    eax, 0fffe2b40h
        add    eax, 27100h
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_08E8
loc_1_06CA:
        mov    ebx, dword ptr [ebp + 8]
        xor    edx, edx
        mov    dx, word ptr [ebx + 0dch]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0deh]
        imul   edx, eax
        jmp    NEAR PTR loc_1_08E5
loc_1_06E7:
        mov    ebx, dword ptr [ebp + 8]
        xor    edx, edx
        mov    dx, word ptr [ebx + 0e4h]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0e6h]
        imul   edx, eax
        jmp    NEAR PTR loc_1_08E5
loc_1_0704:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0deh]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_08E8
loc_1_0718:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0e6h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_08E8
loc_1_072C:
        push   3
        mov    ebx, dword ptr [ebp + 8]
        lea    ecx, [ebp - 8]
        lea    eax, [ebx + 54h]
        push   eax
        push   ecx
        call   sub_1_402C
        and    dword ptr [ebp - 8], 0ffffff00h
        cmp    byte ptr [ebx + 39h], 0
        je     SHORT loc_1_074F
        or     dword ptr [ebp - 8], 1
loc_1_074F:
        lea    ecx, [ebp - 8]
        mov    esi, 4
        jmp    NEAR PTR loc_1_08E8
loc_1_075C:
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 39h], 0
        je     SHORT loc_1_0774
        db  0B9h, 000h, 000h, 000h, 000h
        mov    esi, 2ch
        jmp    NEAR PTR loc_1_08E8
loc_1_0774:
        db  0B9h, 000h, 000h, 000h, 000h
        mov    esi, 29h
        jmp    NEAR PTR loc_1_08E8
loc_1_0783:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_079A
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 96h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_08E8
loc_1_079A:
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 0ch]
        mov    ebx, dword ptr [edx + 8]
        mov    dword ptr [ebp - 8], ebx
        jmp    NEAR PTR loc_1_08E8
loc_1_07AB:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_1_07BC
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 18h]
        jmp    NEAR PTR loc_1_08E5
loc_1_07BC:
        mov    dword ptr [ebp - 8], 0
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, ebx
        mov    edx, dword ptr [ebx + 62h]
        sub    eax, edx
        cmp    eax, -62h
        je     NEAR PTR loc_1_08E8
loc_1_07D6:
        mov    eax, dword ptr [edx + 18h]
        cmp    eax, dword ptr [ebp - 8]
        jbe    SHORT loc_1_07EC
        mov    dword ptr [ebp - 8], eax
        cmp    eax, 0f0h
        je     NEAR PTR loc_1_08E8
loc_1_07EC:
        mov    edx, dword ptr [edx]
        mov    eax, ebx
        sub    eax, edx
        cmp    eax, -62h
        jne    SHORT loc_1_07D6
        jmp    NEAR PTR loc_1_08E8
loc_1_07FC:
        lea    ecx, [ebp - 2]
        mov    esi, 2
        mov    word ptr [ebp - 2], 300h
        jmp    NEAR PTR loc_1_08E8
loc_1_080F:
        mov    dword ptr [ebp - 8], 6
        jmp    NEAR PTR loc_1_08E8
loc_1_081B:
        mov    ebx, dword ptr [ebp + 8]
        mov    al, byte ptr [ebx + 54h]
        add    ebx, 54h
        mov    byte ptr [ebp - 18h], al
        mov    cl, byte ptr [ebx + 1]
        mov    byte ptr [ebp - 17h], cl
        mov    dl, byte ptr [ebx + 2]
        mov    byte ptr [ebp - 16h], dl
        mov    al, byte ptr [ebx + 3]
        mov    esi, 6
        mov    byte ptr [ebp - 15h], al
        mov    cl, byte ptr [ebx + 4]
        mov    byte ptr [ebp - 14h], cl
        lea    ecx, [ebp - 18h]
        mov    dl, byte ptr [ebx + 5]
        mov    byte ptr [ebp - 13h], dl
        jmp    NEAR PTR loc_1_08E8
loc_1_0852:
        mov    ebx, dword ptr [ebp + 8]
        mov    al, byte ptr [ebx + 4eh]
        add    ebx, 4eh
        mov    byte ptr [ebp - 18h], al
        mov    cl, byte ptr [ebx + 1]
        mov    byte ptr [ebp - 17h], cl
        mov    dl, byte ptr [ebx + 2]
        mov    byte ptr [ebp - 16h], dl
        mov    al, byte ptr [ebx + 3]
        mov    esi, 6
        mov    byte ptr [ebp - 15h], al
        mov    cl, byte ptr [ebx + 4]
        mov    byte ptr [ebp - 14h], cl
        lea    ecx, [ebp - 18h]
        mov    dl, byte ptr [ebx + 5]
        mov    byte ptr [ebp - 13h], dl
        jmp    SHORT loc_1_08E8
loc_1_0886:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_1_089A
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 0ch]
        mov    ebx, dword ptr [edx + 0ch]
        mov    dword ptr [ebp - 8], ebx
        jmp    SHORT loc_1_08A6
loc_1_089A:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0a2h]
        mov    dword ptr [ebp - 8], eax
loc_1_08A6:
        mov    edx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        and    edx, 0ff00h
        jmp    SHORT loc_1_08C8
loc_1_08B4:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0aah]
        mov    edx, eax
        mov    dword ptr [ebp - 8], eax
        and    edx, 0ff00h
loc_1_08C8:
        shl    eax, 10h
        mov    ebx, dword ptr [ebp - 8]
        shr    ebx, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ff0000h
        or     eax, ebx
        shr    eax, 8
        or     edx, eax
loc_1_08E5:
        mov    dword ptr [ebp - 8], edx
loc_1_08E8:
        test   edi, edi
        jne    SHORT loc_1_0916
        cmp    dword ptr [ebp + 1ch], esi
        jae    SHORT loc_1_0906
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
loc_1_0906:
        push   esi
        mov    eax, dword ptr [ebp + 18h]
        push   ecx
        push   eax
        call   sub_1_402C
        mov    eax, dword ptr [ebp + 24h]
        add    dword ptr [eax], esi
loc_1_0916:
        mov    eax, edi
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
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0970:
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
        call   sub_1_0590
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 20h], ecx
        mov    dword ptr [esi + 24h], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_09C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        xor    ebx, ebx
        mov    ecx, dword ptr [esi + 18h]
        mov    eax, dword ptr [esi + 14h]
        mov    edi, dword ptr [esi + 1ch]
        cmp    edi, 4
        je     SHORT loc_1_09F0
        mov    eax, 0c0010014h
        pop    edi
        mov    dword ptr [esi + 20h], ebx
        mov    dword ptr [esi + 24h], ebx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_09F0:
        cmp    eax, 2010103h
        ja     SHORT loc_1_0A18
        je     NEAR PTR loc_1_0ABD
        cmp    eax, 1010eh
        je     SHORT loc_1_0A35
        cmp    eax, 1010fh
        je     SHORT loc_1_0A58
        cmp    eax, 10112h
        je     NEAR PTR loc_1_0AB9
        jmp    SHORT loc_1_0A23
loc_1_0A18:
        cmp    eax, 2010104h
        je     NEAR PTR loc_1_0ACE
loc_1_0A23:
        mov    ebx, 0c0010017h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_1_0ADF
loc_1_0A35:
        push   4
        lea    eax, [ebp - 8]
        push   ecx
        push   eax
        call   sub_1_402C
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   ecx
        push   edx
        call   sub_1_0B00
        jmp    NEAR PTR loc_1_0ADD
loc_1_0A58:
        push   4
        lea    eax, [ebp - 4]
        push   ecx
        push   eax
        call   sub_1_402C
        cmp    dword ptr [ebp - 4], 0f0h
        ja     SHORT loc_1_0AB2
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 165h]
        cmp    edx, dword ptr [ebp - 4]
        jae    SHORT loc_1_0A8C
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 18h], edx
        mov    dword ptr [eax + 165h], edx
        jmp    SHORT loc_1_0ADF
loc_1_0A8C:
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [ecx + 18h], edx
        jne    SHORT loc_1_0AAA
        mov    edx, dword ptr [ecx + 18h]
        cmp    edx, dword ptr [ebp - 4]
        jbe    SHORT loc_1_0AAA
        mov    edx, dword ptr [ebp - 4]
        push   eax
        mov    dword ptr [ecx + 18h], edx
        call   sub_1_02C0
        jmp    SHORT loc_1_0ADF
loc_1_0AAA:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 18h], eax
        jmp    SHORT loc_1_0ADF
loc_1_0AB2:
        mov    ebx, 0c0010014h
        jmp    SHORT loc_1_0ADF
loc_1_0AB9:
        xor    ebx, ebx
        jmp    SHORT loc_1_0ADF
loc_1_0ABD:
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   esi
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        push   eax
        call   sub_1_0B70
        jmp    SHORT loc_1_0ADD
loc_1_0ACE:
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   esi
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        push   eax
        call   sub_1_0BD0
loc_1_0ADD:
        mov    ebx, eax
loc_1_0ADF:
        test   ebx, ebx
        jne    SHORT loc_1_0AED
        mov    dword ptr [esi + 24h], 0
        mov    dword ptr [esi + 20h], edi
loc_1_0AED:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0B00:
        push   ebp
        mov    eax, dword ptr [esp + 14h]
        mov    ebp, esp
        test   eax, 8076h
        push   ebx
        push   esi
        push   edi
        je     SHORT loc_1_0B1D
        mov    eax, 0c00000bbh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_1_0B1D:
        mov    esi, dword ptr [ebp + 0ch]
        push   1
        push   eax
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [esi + 0ch]
        push   eax
        inc    dword ptr [esi + 14h]
        push   ecx
        mov    eax, dword ptr [edi + 86h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0B3D:
        mov    ebx, eax
        dec    dword ptr [esi + 14h]
        cmp    ebx, 103h
        jne    SHORT loc_1_0B60
        cmp    byte ptr [edi + 0cch], 0
        je     SHORT loc_1_0B60
        mov    byte ptr [edi + 0cch], 0
        push   edi
        call   sub_1_2DD8
loc_1_0B60:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0B70:
        push   ebp
        mov    eax, dword ptr [esp + 14h]
        mov    ebp, esp
        push   esi
        push   edi
        push   1
        mov    esi, dword ptr [ebp + 0ch]
        push   eax
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [esi + 0ch]
        push   ecx
        mov    eax, dword ptr [esi + 8]
        push   edx
        inc    dword ptr [esi + 14h]
        mov    ecx, dword ptr [eax + 86h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0B99:
        mov    edi, eax
        dec    dword ptr [esi + 14h]
        cmp    edi, 103h
        jne    SHORT loc_1_0BBF
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 0cch], 0
        je     SHORT loc_1_0BBF
        mov    byte ptr [eax + 0cch], 0
        push   eax
        call   sub_1_2DD8
loc_1_0BBF:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0BD0:
        push   ebp
        mov    eax, dword ptr [esp + 14h]
        mov    ebp, esp
        push   esi
        push   edi
        push   1
        mov    esi, dword ptr [ebp + 0ch]
        push   eax
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [esi + 0ch]
        push   ecx
        mov    eax, dword ptr [esi + 8]
        push   edx
        inc    dword ptr [esi + 14h]
        mov    ecx, dword ptr [eax + 86h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0BF9:
        mov    edi, eax
        dec    dword ptr [esi + 14h]
        cmp    edi, 103h
        jne    SHORT loc_1_0C1F
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 0cch], 0
        je     SHORT loc_1_0C1F
        mov    byte ptr [eax + 0cch], 0
        push   eax
        call   sub_1_2DD8
loc_1_0C1F:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0C30:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
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
        mov    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp - 14h], ecx
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 10h]
        push   eax
        mov    edx, dword ptr [ebp - 14h]
        push   ecx
        mov    eax, dword ptr [esi + 14h]
        push   edx
        mov    ebx, dword ptr [ebp + 8]
        push   1
        push   eax
        push   0
        push   ebx
        call   sub_1_0590
        cmp    eax, 0c00000bbh
        mov    edi, eax
        jne    NEAR PTR loc_1_0D57
        xor    edi, edi
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 2010105h
        ja     SHORT loc_1_0CA3
        je     SHORT loc_1_0CF7
        sub    eax, 20101h
        cmp    eax, 4
        ja     SHORT loc_1_0CC8
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_0CA3:
        cmp    eax, 2020101h
        ja     SHORT loc_1_0CC1
        je     SHORT loc_1_0D17
        cmp    eax, 2010106h
        je     SHORT loc_1_0D07
        cmp    eax, 2010107h
        je     SHORT loc_1_0D0F
        mov    edi, 0c0010017h
        jmp    SHORT loc_1_0D28
loc_1_0CC1:
        cmp    eax, 2020102h
        je     SHORT loc_1_0D1F
loc_1_0CC8:
        mov    edi, 0c0010017h
        jmp    SHORT loc_1_0D28
loc_1_0CCF:
        mov    eax, dword ptr [ebx + 0f8h]
        jmp    SHORT loc_1_0D25
loc_1_0CD7:
        mov    eax, dword ptr [ebx + 0fch]
        jmp    SHORT loc_1_0D25
loc_1_0CDF:
        mov    eax, dword ptr [ebx + 100h]
        jmp    SHORT loc_1_0D25
loc_1_0CE7:
        mov    eax, dword ptr [ebx + 104h]
        jmp    SHORT loc_1_0D25
loc_1_0CEF:
        mov    eax, dword ptr [ebx + 120h]
        jmp    SHORT loc_1_0D25
loc_1_0CF7:
        xor    eax, eax
        mov    ax, word ptr [ebx + 173h]
        or     eax, 0c0011000h
        jmp    SHORT loc_1_0D25
loc_1_0D07:
        mov    eax, dword ptr [ebx + 158h]
        jmp    SHORT loc_1_0D25
loc_1_0D0F:
        mov    eax, dword ptr [ebx + 15ch]
        jmp    SHORT loc_1_0D25
loc_1_0D17:
        mov    eax, dword ptr [ebx + 108h]
        jmp    SHORT loc_1_0D25
loc_1_0D1F:
        mov    eax, dword ptr [ebx + 11ch]
loc_1_0D25:
        mov    dword ptr [ebp - 4], eax
loc_1_0D28:
        test   edi, edi
        jne    SHORT loc_1_0D57
        cmp    dword ptr [ebp - 10h], 4
        jb     SHORT loc_1_0D47
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 14h]
        push   ecx
        call   sub_1_402C
        add    dword ptr [ebp - 0ch], 4
        jmp    SHORT loc_1_0D57
loc_1_0D47:
        mov    eax, 4
        mov    edi, 0c0010014h
        sub    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 8], eax
loc_1_0D57:
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
        db  08Bh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h
        db  000h, 000h
loc_1_0D90:
        push   ebp
        mov    ecx, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   esi
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [ebp + 8]
        inc    dword ptr [edi + 5eh]
        mov    eax, dword ptr [ecx + 14h]
        cmp    eax, 10113h
        ja     SHORT loc_1_0DC2
        je     SHORT loc_1_0DFF
        cmp    eax, 10101h
        jb     SHORT loc_1_0DF3
        cmp    eax, 10111h
        jbe    SHORT loc_1_0DFF
        mov    esi, 0c0010017h
        jmp    SHORT loc_1_0DFF
loc_1_0DC2:
        cmp    eax, 2010107h
        ja     SHORT loc_1_0DE5
        cmp    eax, 2010101h
        jae    SHORT loc_1_0DFF
        cmp    eax, 20101h
        jb     SHORT loc_1_0DF3
        cmp    eax, 20105h
        jbe    SHORT loc_1_0DFA
        mov    esi, 0c0010017h
        jmp    SHORT loc_1_0DFF
loc_1_0DE5:
        cmp    eax, 2020101h
        jb     SHORT loc_1_0DF3
        cmp    eax, 2020102h
        jbe    SHORT loc_1_0DFA
loc_1_0DF3:
        mov    esi, 0c0010017h
        jmp    SHORT loc_1_0DFF
loc_1_0DFA:
        mov    esi, 103h
loc_1_0DFF:
        cmp    esi, 103h
        jne    SHORT loc_1_0E4E
        mov    dword ptr [ecx], 0
        mov    byte ptr [ecx + 4], 0
        cmp    dword ptr [edi + 8ah], 0
        jne    SHORT loc_1_0E28
        mov    dword ptr [edi + 8eh], ecx
        mov    dword ptr [edi + 8ah], ecx
        jmp    SHORT loc_1_0E30
loc_1_0E28:
        mov    eax, dword ptr [edi + 8eh]
        mov    dword ptr [eax], ecx
loc_1_0E30:
        cmp    byte ptr [edi + 0cch], 0
        je     SHORT loc_1_0E46
        mov    byte ptr [edi + 0cch], 0
        push   edi
        call   sub_1_2DD8
loc_1_0E46:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_0E4E:
        test   esi, esi
        jne    SHORT loc_1_0E5B
        push   ecx
        push   edi
        call   sub_1_0C30
        mov    esi, eax
loc_1_0E5B:
        mov    eax, dword ptr [edi + 5eh]
        dec    eax
        mov    dword ptr [edi + 5eh], eax
        jne    SHORT loc_1_0E7D
        cmp    byte ptr [edi + 160h], 0
        jne    SHORT loc_1_0E77
        mov    eax, dword ptr [edi + 6ah]
        sub    eax, edi
        cmp    eax, 6ah
        je     SHORT loc_1_0E7D
loc_1_0E77:
        push   edi
        call   sub_1_35C8
loc_1_0E7D:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0E90:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, 103h
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 8]
        inc    dword ptr [edi + 5eh]
        cmp    byte ptr [edi + 160h], 0
        je     SHORT loc_1_0EB4
        mov    esi, 0c001000dh
        jmp    SHORT loc_1_0EDE
loc_1_0EB4:
        cmp    byte ptr [edi + 170h], 0
        je     SHORT loc_1_0EC4
        mov    esi, 0c0000001h
        jmp    SHORT loc_1_0EDE
loc_1_0EC4:
        cmp    byte ptr [ebx + 1dh], 0
        jne    SHORT loc_1_0ED9
        push   ebx
        inc    dword ptr [ebx + 14h]
        push   edi
        call   sub_1_11B0
        dec    dword ptr [ebx + 14h]
        jmp    SHORT loc_1_0EDE
loc_1_0ED9:
        mov    esi, 0c0010002h
loc_1_0EDE:
        mov    eax, dword ptr [edi + 5eh]
        dec    eax
        mov    dword ptr [edi + 5eh], eax
        jne    SHORT loc_1_0F00
        cmp    byte ptr [edi + 160h], 0
        jne    SHORT loc_1_0EFA
        mov    eax, dword ptr [edi + 6ah]
        sub    eax, edi
        cmp    eax, 6ah
        je     SHORT loc_1_0F00
loc_1_0EFA:
        push   edi
        call   sub_1_35C8
loc_1_0F00:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0F10:
        push   ebp
        mov    edx, dword ptr [esp + 10h]
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [edx + 8]
        mov    esi, dword ptr [ebp + 14h]
        test   esi, esi
        jne    SHORT loc_1_0F2C
        lea    esi, [edx + 1eh]
        mov    dword ptr [esi + 10h], 4
loc_1_0F2C:
        cmp    byte ptr [ecx + 160h], 0
        je     SHORT loc_1_0F3F
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    14h
loc_1_0F3F:
        mov    eax, dword ptr [ebp + 0ch]
        test   al, 30h
        je     SHORT loc_1_0F50
        mov    eax, 0c0000001h
        pop    esi
        pop    ebp
        ret    14h
loc_1_0F50:
        mov    dword ptr [esi + 4], edx
        cmp    dword ptr [esi + 10h], 4
        mov    dword ptr [esi + 8], eax
        je     SHORT loc_1_0F5F
        mov    dword ptr [esi + 8], eax
loc_1_0F5F:
        mov    dword ptr [esi], 0
        cmp    dword ptr [ecx + 8ah], 0
        jne    SHORT loc_1_0F7C
        mov    dword ptr [ecx + 8eh], esi
        mov    dword ptr [ecx + 8ah], esi
        jmp    SHORT loc_1_0F84
loc_1_0F7C:
        mov    eax, dword ptr [ecx + 8eh]
        mov    dword ptr [eax], esi
loc_1_0F84:
        mov    eax, 103h
        pop    esi
        pop    ebp
        inc    dword ptr [edx + 14h]
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0FA0:
        push   ebp
        mov    edx, dword ptr [esp + 10h]
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [edx + 8]
        mov    esi, dword ptr [ebp + 14h]
        test   esi, esi
        jne    SHORT loc_1_0FBC
        lea    esi, [edx + 46h]
        mov    dword ptr [esi + 10h], 9
loc_1_0FBC:
        cmp    byte ptr [ecx + 160h], 0
        je     SHORT loc_1_0FCF
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    14h
loc_1_0FCF:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 4], edx
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi], 0
        cmp    dword ptr [ecx + 8ah], 0
        jne    SHORT loc_1_0FF5
        mov    dword ptr [ecx + 8eh], esi
        mov    dword ptr [ecx + 8ah], esi
        jmp    SHORT loc_1_0FFD
loc_1_0FF5:
        mov    eax, dword ptr [ecx + 8eh]
        mov    dword ptr [eax], esi
loc_1_0FFD:
        mov    eax, 103h
        pop    esi
        pop    ebp
        inc    dword ptr [edx + 14h]
        ret    14h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1010:
        push   ebp
        mov    edx, dword ptr [esp + 10h]
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [edx + 8]
        mov    esi, dword ptr [ebp + 14h]
        test   esi, esi
        jne    SHORT loc_1_102C
        lea    esi, [edx + 6eh]
        mov    dword ptr [esi + 10h], 0ah
loc_1_102C:
        cmp    byte ptr [ecx + 160h], 0
        je     SHORT loc_1_103F
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    14h
loc_1_103F:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 4], edx
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi], 0
        cmp    dword ptr [ecx + 8ah], 0
        jne    SHORT loc_1_1065
        mov    dword ptr [ecx + 8eh], esi
        mov    dword ptr [ecx + 8ah], esi
        jmp    SHORT loc_1_106D
loc_1_1065:
        mov    eax, dword ptr [ecx + 8eh]
        mov    dword ptr [eax], esi
loc_1_106D:
        mov    eax, 103h
        pop    esi
        pop    ebp
        inc    dword ptr [edx + 14h]
        ret    14h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1080:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        dec    dword ptr [eax + 14h]
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_1090:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_1_3F3A
        mov    edx, dword ptr [esi + 5ah]
        xor    al, al
        inc    edx
        push   0c350h
        out    dx, al
        call   sub_1_3F40
        mov    edx, dword ptr [esi + 5ah]
        xor    al, al
        add    edx, 2
        cmp    byte ptr [esi + 3bh], al
        out    dx, al
        je     SHORT loc_1_10CF
        mov    eax, dword ptr [esi + 0aeh]
        mov    byte ptr [eax + 1e18h], 0c0h
loc_1_10CF:
        cmp    byte ptr [esi + 39h], 0
        je     SHORT loc_1_10E4
        mov    cl, byte ptr [esi + 48h]
        mov    eax, dword ptr [esi + 0aeh]
        mov    byte ptr [eax + 1e00h], cl
loc_1_10E4:
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_1_3F3A
        mov    eax, dword ptr [esi + 0aeh]
        push   0c001000ch
        push   esi
        mov    byte ptr [eax + 1e08h], 0c0h
        call   sub_1_3818
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_1110:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   0
        xor    eax, eax
        cmp    byte ptr [esi + 176h], al
        je     SHORT loc_1_113B
        mov    byte ptr [esi + 176h], al
        mov    byte ptr [esi + 175h], al
        mov    byte ptr [esi + 177h], al
loc_1_113B:
        xor    ebx, ebx
        push   esi
        mov    byte ptr [esi + 160h], bl
        mov    byte ptr [esi + 169h], bl
        mov    byte ptr [esi + 16ah], bl
        mov    byte ptr [esi + 172h], bl
        call   sub_1_2E48
        mov    edi, dword ptr [esi + 16bh]
        test   edi, edi
        je     SHORT loc_1_11A5
        mov    ebx, dword ptr [esi + 62h]
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, -62h
        je     SHORT loc_1_1197
loc_1_1171:
        push   0
        mov    eax, dword ptr [ebx + 10h]
        push   0
        push   40010005h
        push   eax
        call   sub_1_3EAE
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_3EB4
        mov    ebx, dword ptr [ebx]
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, -62h
        jne    SHORT loc_1_1171
loc_1_1197:
        push   0
        dec    dword ptr [edi + 14h]
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_3E96
loc_1_11A5:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_11B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        test   edi, edi
        je     SHORT loc_1_11E9
        mov    ebx, dword ptr [esi + 62h]
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, -62h
        je     SHORT loc_1_11E9
loc_1_11CC:
        push   0
        mov    eax, dword ptr [ebx + 10h]
        push   0
        push   40010004h
        push   eax
        call   sub_1_3EAE
        mov    ebx, dword ptr [ebx]
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, -62h
        jne    SHORT loc_1_11CC
loc_1_11E9:
        mov    eax, 1
        mov    dword ptr [esi + 16bh], edi
        mov    byte ptr [esi + 160h], al
        mov    byte ptr [esi + 172h], al
        mov    dword ptr [esi + 161h], 0
        test   edi, edi
        je     SHORT loc_1_1211
        add    dword ptr [edi + 14h], eax
loc_1_1211:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_1220:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 82h]
        push   eax
        call   sub_1_4024
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 1dh]
        push   eax
        push   ecx
        call   sub_1_3F28
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_1_1253
        push   7a120h
        call   sub_1_3F40
loc_1_1253:
        push   esi
        call   sub_1_3F34
        mov    eax, dword ptr [esi + 86h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_1265:
        mov    eax, dword ptr [esi + 82h]
        push   eax
        call   sub_1_3E78
        push   0
        push   379h
        push   esi
        call   sub_1_3F4C
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1290:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    byte ptr [eax + 172h], 1
        cmp    byte ptr [eax + 0cch], 0
        je     SHORT loc_1_12DB
        mov    ecx, dword ptr [eax + 0b8h]
        test   ecx, ecx
        je     SHORT loc_1_12DB
        mov    edx, dword ptr [eax + 0aeh]
        pop    ebp
        mov    byte ptr [edx + 1e08h], 80h
        mov    word ptr [ecx], 0
        mov    byte ptr [ecx + 2], 0
        mov    byte ptr [ecx], 4
        mov    edx, dword ptr [eax + 0aeh]
        mov    byte ptr [edx + 1e4bh], 20h
        ret    4
loc_1_12DB:
        mov    edx, dword ptr [eax + 5ah]
        pop    ebp
        inc    edx
        xor    al, al
        out    dx, al
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_12F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    ecx, [ebp - 4]
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_1_3E6C
        push   0
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_1_3E60
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_1320:
        db  0A0h, 0D8h, 040h, 000h, 000h
        cmp    al, 0
        jne    SHORT loc_1_1345
        db  0FEh, 005h, 0D8h, 040h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     SHORT loc_1_1347
        push   0
        push   0
        db  0E8h, 0C0h, 0ECh, 0FEh, 0FFh
        cmp    eax, 0
        jne    SHORT loc_1_1347
loc_1_1345:
        clc
        ret
loc_1_1347:
        stc
        ret
IBMTOK4_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_1358
        db  0FFh, 024h, 085h, 05Ah, 013h, 000h, 000h
loc_1_1358:
        clc
        ret
        db  020h, 013h, 000h, 000h, 058h, 013h, 000h, 000h, 058h, 013h, 000h, 000h, 058h, 013h, 000h, 000h
        db  058h, 013h, 000h, 000h, 058h, 013h, 000h, 000h, 058h, 013h, 000h, 000h, 058h, 013h, 000h, 000h
        db  058h, 013h, 000h, 000h, 058h, 013h, 000h, 000h, 058h, 013h, 000h, 000h, 058h, 013h, 000h, 000h
        db  058h, 013h, 000h, 000h, 058h, 013h, 000h, 000h, 058h, 013h, 000h, 000h, 058h, 013h, 000h, 000h
        db  058h, 013h, 000h, 000h, 058h, 013h, 000h, 000h, 058h, 013h, 000h, 000h, 058h, 013h, 000h, 000h
        db  058h, 013h, 000h, 000h, 058h, 013h, 000h, 000h, 058h, 013h, 000h, 000h, 058h, 013h, 000h, 000h
        db  058h, 013h, 000h, 000h, 058h, 013h, 000h, 000h, 020h, 013h, 000h, 000h, 000h, 000h
loc_1_13C8:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 4], 103h
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   eax
        push   0
        mov    ebx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [edi + 8]
        push   0
        push   0
        push   ebx
        call   sub_1_3F70
        cmp    dword ptr [ebp - 8], 0eh
        jb     NEAR PTR loc_1_14F3
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [esi + 0e0h], eax
        jb     NEAR PTR loc_1_14F3
        inc    dword ptr [esi + 5eh]
        cmp    byte ptr [esi + 175h], 0
        je     SHORT loc_1_1422
        mov    dword ptr [ebp - 4], 0c0010008h
        jmp    NEAR PTR loc_1_14C5
loc_1_1422:
        cmp    byte ptr [esi + 172h], 0
        jne    SHORT loc_1_1487
        cmp    byte ptr [edi + 1dh], 0
        lea    eax, [ebx + 24h]
        je     SHORT loc_1_1440
        mov    dword ptr [ebp - 4], 0c0010002h
        jmp    NEAR PTR loc_1_14C5
loc_1_1440:
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [eax + 8], ebx
        cmp    dword ptr [esi + 0e8h], 0
        jne    SHORT loc_1_1457
        mov    dword ptr [esi + 0e8h], ebx
        jmp    SHORT loc_1_1460
loc_1_1457:
        mov    ecx, dword ptr [esi + 0ech]
        mov    dword ptr [ecx + 24h], ebx
loc_1_1460:
        mov    dword ptr [esi + 0ech], ebx
        mov    dword ptr [eax], 0
        inc    dword ptr [edi + 14h]
        cmp    byte ptr [esi + 0cch], 0
        je     SHORT loc_1_14C5
        mov    byte ptr [esi + 0cch], 0
        push   esi
        call   sub_1_2DD8
        jmp    SHORT loc_1_14C5
loc_1_1487:
        cmp    byte ptr [esi + 160h], 0
        je     SHORT loc_1_1499
        mov    dword ptr [ebp - 4], 0c001000dh
        jmp    SHORT loc_1_14C5
loc_1_1499:
        cmp    byte ptr [esi + 170h], 0
        je     SHORT loc_1_14AB
        mov    dword ptr [ebp - 4], 0c0000001h
        jmp    SHORT loc_1_14C5
loc_1_14AB:
        push   2
        mov    eax, dword ptr [esi + 82h]
        push   0bh
        push   2
        push   0aa000006h
        push   eax
        call   sub_1_4018
        add    esp, 14h
loc_1_14C5:
        mov    eax, dword ptr [esi + 5eh]
        dec    eax
        mov    dword ptr [esi + 5eh], eax
        jne    SHORT loc_1_14E7
        cmp    byte ptr [esi + 160h], 0
        jne    SHORT loc_1_14E1
        mov    eax, dword ptr [esi + 6ah]
        sub    eax, esi
        cmp    eax, 6ah
        je     SHORT loc_1_14E7
loc_1_14E1:
        push   esi
        call   sub_1_35C8
loc_1_14E7:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_14F3:
        mov    eax, 0c001000fh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1508:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 20h
        mov    edx, dword ptr [eax + 8]
        push   ebx
        push   esi
        mov    dword ptr [ebp - 14h], edx
        push   edi
        inc    dword ptr [edx + 5eh]
        cmp    byte ptr [edx + 172h], 0
        jne    NEAR PTR loc_1_16BC
        cmp    byte ptr [eax + 1dh], 0
        jne    NEAR PTR loc_1_16B5
        mov    bx, word ptr [edx + 0d7h]
        mov    ecx, dword ptr [ebp + 1ch]
        push   0
        lea    edx, [ebp - 8]
        push   edx
        inc    dword ptr [eax + 14h]
        lea    eax, [ebp - 20h]
        mov    dword ptr [ecx], 0
        push   eax
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ebp - 18h], 0
        push   0
        push   ecx
        call   sub_1_3F70
        cmp    dword ptr [ebp - 20h], 0
        je     NEAR PTR loc_1_16AB
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 1ch]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_3F94
        mov    ah, bl
        mov    edx, dword ptr [ebp - 14h]
        mov    al, bh
        mov    ecx, eax
        mov    eax, dword ptr [edx + 0b2h]
        and    ecx, 0ffffh
        lea    esi, [ecx + eax + 2]
        xor    ecx, ecx
        mov    cx, word ptr [edx + 0d9h]
        lea    eax, [esi + ecx + 6]
        mov    dword ptr [ebp - 10h], eax
        mov    ax, word ptr [esi + 4]
        mov    dh, al
        mov    dl, ah
        mov    eax, edx
        and    eax, 0ffffh
        sub    eax, ecx
        cmp    dword ptr [ebp + 14h], 0
        mov    dword ptr [ebp - 4], eax
        je     NEAR PTR loc_1_16A3
        mov    edi, dword ptr [ebp + 10h]
loc_1_15C9:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_15FC
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_3FB8
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_16A3
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 1ch]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_3F94
        jmp    NEAR PTR loc_1_1697
loc_1_15FC:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_1645
        mov    ax, word ptr [esi]
        test   ax, ax
        je     NEAR PTR loc_1_16A3
        mov    ch, al
        mov    edx, dword ptr [ebp - 14h]
        mov    cl, ah
        mov    esi, ecx
        and    esi, 0ffffh
        add    esi, dword ptr [edx + 0b2h]
        mov    cx, word ptr [esi + 4]
        lea    eax, [esi + 6]
        mov    word ptr [ebp - 4], cx
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, ah
        mov    ah, cl
        mov    ecx, eax
        and    ecx, 0ffffh
        mov    dword ptr [ebp - 4], ecx
        jmp    SHORT loc_1_1697
loc_1_1645:
        test   edi, edi
        je     SHORT loc_1_1662
        cmp    dword ptr [ebp - 4], edi
        jae    SHORT loc_1_165A
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_1697
loc_1_165A:
        add    dword ptr [ebp - 10h], edi
        sub    dword ptr [ebp - 4], edi
        xor    edi, edi
loc_1_1662:
        mov    ebx, dword ptr [ebp + 14h]
        mov    eax, dword ptr [ebp - 4]
        sub    ebx, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 0ch]
        cmp    eax, ecx
        jb     SHORT loc_1_1674
        mov    eax, ecx
loc_1_1674:
        cmp    ebx, eax
        jb     SHORT loc_1_167A
        mov    ebx, eax
loc_1_167A:
        push   ebx
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    ecx, dword ptr [ebp - 1ch]
        push   ecx
        call   sub_1_402C
        add    dword ptr [ebp - 1ch], ebx
        add    dword ptr [ebp - 10h], ebx
        add    dword ptr [ebp - 18h], ebx
        sub    dword ptr [ebp - 4], ebx
        sub    dword ptr [ebp - 0ch], ebx
loc_1_1697:
        mov    ecx, dword ptr [ebp - 18h]
        cmp    ecx, dword ptr [ebp + 14h]
        jb     NEAR PTR loc_1_15C9
loc_1_16A3:
        mov    eax, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [eax], ecx
loc_1_16AB:
        mov    eax, dword ptr [ebp + 8]
        xor    esi, esi
        dec    dword ptr [eax + 14h]
        jmp    SHORT loc_1_1700
loc_1_16B5:
        mov    esi, 0c001000ch
        jmp    SHORT loc_1_1700
loc_1_16BC:
        mov    edx, dword ptr [ebp - 14h]
        cmp    byte ptr [edx + 160h], 0
        je     SHORT loc_1_16CF
        mov    esi, 0c001000dh
        jmp    SHORT loc_1_1700
loc_1_16CF:
        mov    edx, dword ptr [ebp - 14h]
        mov    esi, 0c0000001h
        cmp    byte ptr [edx + 171h], 0
        jne    SHORT loc_1_1700
        push   1
        mov    edx, dword ptr [ebp - 14h]
        push   0bh
        push   2
        push   0aa000006h
        mov    eax, dword ptr [edx + 82h]
        push   eax
        call   sub_1_4018
        add    esp, 14h
        mov    esi, dword ptr [ebp - 4]
loc_1_1700:
        mov    edx, dword ptr [ebp - 14h]
        mov    eax, dword ptr [edx + 5eh]
        dec    eax
        mov    dword ptr [edx + 5eh], eax
        jne    SHORT loc_1_1728
        cmp    byte ptr [edx + 160h], 0
        jne    SHORT loc_1_171F
        mov    eax, dword ptr [edx + 6ah]
        sub    eax, edx
        cmp    eax, 6ah
        je     SHORT loc_1_1728
loc_1_171F:
        mov    edx, dword ptr [ebp - 14h]
        push   edx
        call   sub_1_35C8
loc_1_1728:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  005h, 000h, 000h, 000h, 000h
sub_1_1738:
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
        je     NEAR PTR loc_1_180F
        push   esi
        lea    eax, [ebp - 0ch]
        push   eax
        lea    ecx, [ebp - 10h]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   edx
        call   sub_1_3F70
        cmp    dword ptr [ebp - 10h], esi
        je     NEAR PTR loc_1_180F
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 8]
        push   eax
        mov    edx, dword ptr [ebp - 0ch]
        push   ecx
        push   edx
        call   sub_1_3F94
        cmp    dword ptr [ebp + 10h], esi
        je     NEAR PTR loc_1_180A
        mov    edi, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [ebp + 14h]
loc_1_178E:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_17BA
        lea    eax, [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        push   eax
        push   ecx
        call   sub_1_3FB8
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_1_180A
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 8]
        push   eax
        mov    edx, dword ptr [ebp - 0ch]
        push   ecx
        push   edx
        call   sub_1_3F94
        jmp    SHORT loc_1_1805
loc_1_17BA:
        test   edi, edi
        je     SHORT loc_1_17D7
        cmp    dword ptr [ebp - 4], edi
        jae    SHORT loc_1_17CF
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_1805
loc_1_17CF:
        add    dword ptr [ebp - 8], edi
        sub    dword ptr [ebp - 4], edi
        xor    edi, edi
loc_1_17D7:
        mov    ecx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp - 4]
        sub    ecx, esi
        cmp    ecx, eax
        jb     SHORT loc_1_17E5
        mov    ecx, eax
loc_1_17E5:
        xor    edx, edx
        test   ecx, ecx
        je     SHORT loc_1_17FB
loc_1_17EB:
        mov    eax, dword ptr [ebp - 8]
        inc    edx
        cmp    edx, ecx
        mov    al, byte ptr [eax + edx - 1]
        mov    byte ptr [ebx + edx - 1], al
        jb     SHORT loc_1_17EB
loc_1_17FB:
        add    ebx, ecx
        add    esi, ecx
        add    dword ptr [ebp - 8], ecx
        sub    dword ptr [ebp - 4], ecx
loc_1_1805:
        cmp    esi, dword ptr [ebp + 10h]
        jb     SHORT loc_1_178E
loc_1_180A:
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], esi
loc_1_180F:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  055h, 08Bh, 044h, 024h, 018h, 08Bh, 0ECh, 083h, 0ECh, 014h, 053h, 056h, 057h, 033h, 0F6h, 089h
        db  030h, 039h, 075h, 00Ch, 00Fh, 084h, 0B2h, 000h, 000h, 000h, 056h, 08Dh, 045h, 0F8h, 050h, 08Dh
        db  04Dh, 0ECh, 051h, 08Bh, 055h, 010h, 056h, 052h, 0E8h, 02Bh, 027h, 000h, 000h, 039h, 075h, 0ECh
        db  00Fh, 084h, 096h, 000h, 000h, 000h, 08Dh, 045h, 0FCh, 08Dh, 04Dh, 0F4h, 050h, 08Bh, 055h, 0F8h
        db  051h, 052h, 0E8h, 035h, 027h, 000h, 000h, 08Bh, 04Dh, 008h, 039h, 075h, 00Ch, 089h, 04Dh, 0F0h
        db  074h, 075h, 08Bh, 07Dh, 014h, 083h, 07Dh, 0FCh, 000h, 075h, 026h, 08Dh, 045h, 0F8h, 08Bh, 04Dh
        db  0F8h, 050h, 051h, 0E8h, 038h, 027h, 000h, 000h, 083h, 07Dh, 0F8h, 000h, 074h, 059h, 08Dh, 045h
        db  0FCh, 08Dh, 04Dh, 0F4h, 050h, 08Bh, 055h, 0F8h, 051h, 052h, 0E8h, 0FDh, 026h, 000h, 000h, 0EBh
        db  041h, 085h, 0FFh, 074h, 019h, 03Bh, 07Dh, 0FCh, 076h, 00Ch, 02Bh, 07Dh, 0FCh, 0C7h, 045h, 0FCh
        db  000h, 000h, 000h, 000h, 0EBh, 02Ch, 001h, 07Dh, 0F4h, 029h, 07Dh, 0FCh, 033h, 0FFh, 08Bh, 05Dh
        db  00Ch, 08Bh, 045h, 0FCh, 02Bh, 0DEh, 03Bh, 0D8h, 072h, 002h, 08Bh, 0D8h, 053h, 08Bh, 045h, 0F0h
        db  050h, 08Bh, 04Dh, 0F4h, 051h, 003h, 0F3h, 0E8h, 058h, 027h, 000h, 000h, 001h, 05Dh, 0F0h, 029h
        db  05Dh, 0FCh, 039h, 075h, 00Ch, 077h, 08Eh, 08Bh, 045h, 018h, 089h, 030h, 05Fh, 05Eh, 05Bh, 08Bh
        db  0E5h, 05Dh, 0C2h, 014h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        db  055h, 08Bh, 044h, 024h, 01Ch, 08Bh, 0ECh, 083h, 0ECh, 024h, 0C7h, 000h, 000h, 000h, 000h, 000h
        db  053h, 0C7h, 045h, 0E4h, 000h, 000h, 000h, 000h, 083h, 07Dh, 010h, 000h, 056h, 057h, 00Fh, 084h
        db  056h, 001h, 000h, 000h, 06Ah, 000h, 08Dh, 045h, 0F0h, 050h, 08Dh, 04Dh, 0DCh, 051h, 08Bh, 055h
        db  008h, 06Ah, 000h, 052h, 0E8h, 03Fh, 026h, 000h, 000h, 083h, 07Dh, 0DCh, 000h, 00Fh, 084h, 037h
        db  001h, 000h, 000h, 08Dh, 045h, 0FCh, 08Dh, 04Dh, 0E8h, 050h, 08Bh, 055h, 0F0h, 051h, 052h, 0E8h
        db  048h, 026h, 000h, 000h, 06Ah, 000h, 08Dh, 04Dh, 0F4h, 051h, 08Dh, 055h, 0E0h, 052h, 08Bh, 045h
        db  014h, 06Ah, 000h, 050h, 0E8h, 00Fh, 026h, 000h, 000h, 083h, 07Dh, 0E0h, 000h, 00Fh, 084h, 007h
        db  001h, 000h, 000h, 08Dh, 045h, 0F8h, 08Dh, 04Dh, 0ECh, 050h, 08Bh, 055h, 0F4h, 051h, 052h, 0E8h
        db  018h, 026h, 000h, 000h, 083h, 07Dh, 010h, 000h, 00Fh, 084h, 0E4h, 000h, 000h, 000h, 08Bh, 075h
        db  00Ch, 08Bh, 07Dh, 018h, 083h, 07Dh, 0FCh, 000h, 075h, 02Dh, 08Dh, 045h, 0F0h, 08Bh, 04Dh, 0F0h
        db  050h, 051h, 0E8h, 019h, 026h, 000h, 000h, 083h, 07Dh, 0F0h, 000h, 00Fh, 084h, 0C1h, 000h, 000h
        db  000h, 08Dh, 045h, 0FCh, 08Dh, 04Dh, 0E8h, 050h, 08Bh, 055h, 0F0h, 051h, 052h, 0E8h, 0DAh, 025h
        db  000h, 000h, 0E9h, 09Fh, 000h, 000h, 000h, 083h, 07Dh, 0F8h, 000h, 075h, 02Ah, 08Dh, 045h, 0F4h
        db  08Bh, 04Dh, 0F4h, 050h, 051h, 0E8h, 0E6h, 025h, 000h, 000h, 083h, 07Dh, 0F4h, 000h, 00Fh, 084h
        db  08Eh, 000h, 000h, 000h, 08Dh, 045h, 0F8h, 08Dh, 04Dh, 0ECh, 050h, 08Bh, 055h, 0F4h, 051h, 052h
        db  0E8h, 0A7h, 025h, 000h, 000h, 0EBh, 06Fh, 085h, 0F6h, 074h, 019h, 039h, 075h, 0FCh, 073h, 00Ch
        db  02Bh, 075h, 0FCh, 0C7h, 045h, 0FCh, 000h, 000h, 000h, 000h, 0EBh, 05Ah, 001h, 075h, 0E8h, 029h
        db  075h, 0FCh, 033h, 0F6h, 085h, 0FFh, 074h, 019h, 039h, 07Dh, 0F8h, 073h, 00Ch, 02Bh, 07Dh, 0F8h
        db  0C7h, 045h, 0F8h, 000h, 000h, 000h, 000h, 0EBh, 03Dh, 001h, 07Dh, 0ECh, 029h, 07Dh, 0F8h, 033h
        db  0FFh, 08Bh, 05Dh, 0F8h, 08Bh, 045h, 0FCh, 03Bh, 0D8h, 072h, 002h, 08Bh, 0D8h, 08Bh, 045h, 010h
        db  02Bh, 045h, 0E4h, 03Bh, 0D8h, 072h, 002h, 08Bh, 0D8h, 053h, 08Bh, 045h, 0ECh, 050h, 08Bh, 04Dh
        db  0E8h, 051h, 0E8h, 0DDh, 025h, 000h, 000h, 001h, 05Dh, 0E8h, 001h, 05Dh, 0ECh, 001h, 05Dh, 0E4h
        db  029h, 05Dh, 0F8h, 029h, 05Dh, 0FCh, 08Bh, 04Dh, 0E4h, 039h, 04Dh, 010h, 00Fh, 087h, 022h, 0FFh
        db  0FFh, 0FFh, 08Bh, 045h, 01Ch, 08Bh, 04Dh, 0E4h, 089h, 008h, 05Fh, 05Eh, 05Bh, 08Bh, 0E5h, 05Dh
        db  0C2h, 018h, 000h, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1A88:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [ecx]
        mov    dl, byte ptr [eax + 49h]
        mov    eax, dword ptr [ecx + 4]
        and    dl, 24h
        pop    ebp
        mov    byte ptr [eax], dl
        mov    al, 1
        mov    ecx, dword ptr [ecx]
        and    byte ptr [ecx + 49h], 0dbh
        ret    4
loc_1_1AA8:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [ecx]
        mov    dl, byte ptr [eax + 49h]
        mov    eax, dword ptr [ecx + 4]
        and    dl, 18h
        pop    ebp
        mov    byte ptr [eax], dl
        mov    al, 1
        mov    ecx, dword ptr [ecx]
        and    byte ptr [ecx + 49h], 0e7h
        ret    4
loc_1_1AC8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    byte ptr [eax + 169h], 1
        mov    al, 1
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1AE8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    byte ptr [eax + 49h], 0
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1B08:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ecx + 0aeh]
        cmp    byte ptr [ecx + 2ch], 0
        mov    bl, byte ptr [eax + 1e09h]
        jne    SHORT loc_1_1B3A
        mov    al, byte ptr [ecx + 18h]
        inc    al
        mov    byte ptr [ecx + 18h], al
        cmp    al, 0ffh
        jne    SHORT loc_1_1B3A
        mov    edx, dword ptr [ecx + 5ah]
        xor    al, al
        inc    edx
        pop    ebx
        out    dx, al
        pop    ebp
        ret    4
loc_1_1B3A:
        mov    dl, bl
        mov    eax, dword ptr [ecx + 0aeh]
        not    dl
        mov    byte ptr [eax + 1e29h], dl
        cmp    byte ptr [ecx + 172h], 0
        je     SHORT loc_1_1B5F
        mov    al, bl
        and    bl, 20h
        and    al, 0dfh
        or     byte ptr [ecx + 4ah], al
        jmp    SHORT loc_1_1B66
loc_1_1B5F:
        or     bl, byte ptr [ecx + 4ah]
        mov    byte ptr [ecx + 4ah], 0
loc_1_1B66:
        or     byte ptr [ecx + 49h], bl
        cmp    byte ptr [ecx + 169h], 0
        je     SHORT loc_1_1B79
        mov    byte ptr [ecx + 16ah], 1
loc_1_1B79:
        cmp    byte ptr [ecx + 16fh], 0
        je     SHORT loc_1_1BF4
        mov    eax, dword ptr [ecx + 0aeh]
        mov    dl, byte ptr [eax + 1e02h]
        mov    al, byte ptr [eax + 1e03h]
        and    edx, 0ffff00ffh
        and    eax, 0ffff00ffh
        shl    dx, 8
        add    ax, dx
        test   al, 1
        je     SHORT loc_1_1BC0
        cmp    byte ptr [ecx + 39h], 0
        je     SHORT loc_1_1BB9
        mov    edx, dword ptr [ecx + 5ah]
        xor    al, al
        add    edx, 3
        out    dx, al
loc_1_1BB9:
        xor    al, al
        pop    ebx
        pop    ebp
        ret    4
loc_1_1BC0:
        mov    word ptr [ecx + 0b6h], ax
        and    eax, 0ffffh
        mov    edx, dword ptr [ecx + 0b2h]
        cmp    word ptr [edx + eax + 6], 0
        jne    SHORT loc_1_1BDE
        mov    byte ptr [ecx + 2ch], 1
loc_1_1BDE:
        cmp    byte ptr [ecx + 39h], 0
        je     SHORT loc_1_1BED
        mov    edx, dword ptr [ecx + 5ah]
        xor    al, al
        add    edx, 3
        out    dx, al
loc_1_1BED:
        xor    al, al
        pop    ebx
        pop    ebp
        ret    4
loc_1_1BF4:
        cmp    byte ptr [ecx + 39h], 0
        je     SHORT loc_1_1C03
        mov    edx, dword ptr [ecx + 5ah]
        xor    al, al
        add    edx, 3
        out    dx, al
loc_1_1C03:
        test   bl, bl
        jne    SHORT loc_1_1C27
        mov    eax, dword ptr [ecx + 0aeh]
        mov    dl, byte ptr [eax + 1e08h]
        and    dl, 1ch
        mov    byte ptr [ecx + 4bh], dl
        mov    eax, dword ptr [ecx + 0aeh]
        not    dl
        mov    byte ptr [eax + 1e28h], dl
loc_1_1C27:
        cmp    byte ptr [ecx + 49h], 1
        sbb    al, al
        pop    ebx
        inc    al
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_1C38:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        mov    byte ptr [esi + 18h], 0
        mov    byte ptr [esi + 29h], 0
        mov    al, byte ptr [esi + 4bh]
        test   al, al
        je     SHORT loc_1_1C72
        and    eax, 0ffh
        mov    ecx, dword ptr [esi + 82h]
        push   eax
        push   0ch
        push   8
        push   3
        push   0aa000003h
        push   ecx
        call   sub_1_4018
        mov    byte ptr [esi + 4bh], 0
        add    esp, 18h
loc_1_1C72:
        test   byte ptr [esi + 49h], 24h
        je     SHORT loc_1_1C84
        cmp    byte ptr [esi + 4ch], 0
        jne    SHORT loc_1_1C84
        push   esi
        call   sub_1_1CA8
loc_1_1C84:
        test   byte ptr [esi + 49h], 18h
        je     SHORT loc_1_1C96
        cmp    byte ptr [esi + 4dh], 0
        jne    SHORT loc_1_1C96
        push   esi
        call   sub_1_2218
loc_1_1C96:
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1CA8:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 5eh]
        cmp    byte ptr [esi + 160h], 0
        je     SHORT loc_1_1D02
        cmp    byte ptr [esi + 16ah], 0
        je     SHORT loc_1_1D02
        push   esi
        call   sub_1_2A58
        mov    eax, dword ptr [esi + 5eh]
        dec    eax
        mov    dword ptr [esi + 5eh], eax
        jne    NEAR PTR loc_1_220C
        cmp    byte ptr [esi + 160h], 0
        jne    SHORT loc_1_1CF3
        mov    eax, dword ptr [esi + 6ah]
        sub    eax, esi
        cmp    eax, 6ah
        je     NEAR PTR loc_1_220C
loc_1_1CF3:
        push   esi
        call   sub_1_35C8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1D02:
        lea    eax, [ebp - 0ah]
        lea    ecx, [ebp - 8]
        push   ecx
        mov    dword ptr [ebp - 8], esi
        db  068h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        mov    byte ptr [esi + 4ch], 1
        push   esi
        call   sub_1_3F3A
        test   byte ptr [ebp - 0ah], 24h
        je     NEAR PTR loc_1_21E6
loc_1_1D28:
        cmp    byte ptr [esi + 175h], 0
        je     SHORT loc_1_1D3E
        cmp    byte ptr [esi + 176h], 0
        je     NEAR PTR loc_1_21E6
loc_1_1D3E:
        test   byte ptr [ebp - 0ah], 20h
        je     NEAR PTR loc_1_20A3
        cmp    byte ptr [esi + 171h], 0
        je     NEAR PTR loc_1_1FD8
        mov    edi, dword ptr [esi + 0b2h]
        xor    eax, eax
        mov    ax, word ptr [esi + 0b6h]
        lea    ecx, [eax + edi]
        cmp    byte ptr [ecx + 2], 0
        jne    NEAR PTR loc_1_1EFD
        cmp    byte ptr [esi + 3bh], 0
        je     NEAR PTR loc_1_1E0B
        mov    dx, word ptr [ecx + 0ah]
        mov    ah, dl
        mov    al, dh
        mov    dx, ax
        shr    ax, 0eh
        and    edx, 3fffh
        add    edx, edi
        mov    byte ptr [esi + 0c8h], al
        mov    dword ptr [esi + 0b8h], edx
        mov    dx, word ptr [ecx + 0eh]
        mov    ah, dl
        mov    al, dh
        mov    dx, ax
        shr    ax, 0eh
        and    edx, 3fffh
        add    edx, edi
        mov    byte ptr [esi + 0c9h], al
        mov    dword ptr [esi + 0bch], edx
        mov    dx, word ptr [ecx + 0ch]
        mov    ah, dl
        mov    al, dh
        mov    dx, ax
        shr    ax, 0eh
        and    edx, 3fffh
        add    edx, edi
        lea    ebx, [esi + 0c4h]
        mov    dword ptr [esi + 0c0h], edx
        mov    byte ptr [esi + 0cah], al
        mov    dx, word ptr [ecx + 8]
        mov    ah, dl
        mov    al, dh
        mov    dx, ax
        shr    ax, 0eh
        and    edx, 3fffh
        add    edx, edi
        mov    byte ptr [esi + 0cbh], al
        mov    dword ptr [ebx], edx
        jmp    SHORT loc_1_1E7B
loc_1_1E0B:
        mov    ax, word ptr [ecx + 0ah]
        lea    edx, [esi + 0b8h]
        mov    dh, al
        mov    dl, ah
        mov    eax, edx
        lea    edx, [esi + 0bch]
        and    eax, 0ffffh
        add    eax, edi
        mov    dword ptr [esi + 0b8h], eax
        mov    ax, word ptr [ecx + 0eh]
        mov    dh, al
        mov    dl, ah
        mov    eax, edx
        lea    edx, [esi + 0c0h]
        and    eax, 0ffffh
        add    eax, edi
        mov    dword ptr [esi + 0bch], eax
        mov    ax, word ptr [ecx + 0ch]
        mov    dh, al
        lea    ebx, [esi + 0c4h]
        mov    dl, ah
        mov    eax, edx
        and    eax, 0ffffh
        add    eax, edi
        mov    dword ptr [esi + 0c0h], eax
        mov    ax, word ptr [ecx + 8]
        mov    dh, al
        mov    dl, ah
        mov    eax, edx
        and    eax, 0ffffh
        add    eax, edi
        mov    dword ptr [ebx], eax
loc_1_1E7B:
        mov    eax, dword ptr [esi + 40h]
        add    eax, edi
        cmp    dword ptr [esi + 0b8h], eax
        ja     SHORT loc_1_1EFD
        cmp    dword ptr [esi + 0bch], eax
        ja     SHORT loc_1_1EFD
        cmp    dword ptr [esi + 0c0h], eax
        ja     SHORT loc_1_1EFD
        cmp    dword ptr [ebx], eax
        ja     SHORT loc_1_1EFD
        mov    dword ptr [esi + 15ch], 1
        mov    byte ptr [esi + 171h], 0
        mov    edi, dword ptr [esi + 62h]
        mov    eax, esi
        mov    byte ptr [esi + 170h], 0
        mov    byte ptr [esi + 172h], 0
        mov    word ptr [esi + 173h], 0
        sub    eax, edi
        cmp    eax, -62h
        je     SHORT loc_1_1EF2
loc_1_1ED0:
        cmp    byte ptr [edi + 1ch], 0
        je     SHORT loc_1_1EE7
        mov    byte ptr [edi + 1ch], 0
        push   0
        mov    eax, dword ptr [edi + 10h]
        push   0
        push   eax
        call   sub_1_3E7E
loc_1_1EE7:
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -62h
        jne    SHORT loc_1_1ED0
loc_1_1EF2:
        push   esi
        call   sub_1_2E48
        jmp    NEAR PTR loc_1_20A3
loc_1_1EFD:
        mov    dx, word ptr [ecx + 6]
        mov    word ptr [esi + 173h], dx
        mov    ah, dl
        mov    al, dh
        mov    word ptr [esi + 173h], ax
        cmp    ax, 24h
        jne    SHORT loc_1_1F3D
        cmp    byte ptr [esi + 36h], 1
        jne    SHORT loc_1_1F3D
        mov    al, byte ptr [esi + 37h]
        test   al, al
        jne    SHORT loc_1_1F3D
        inc    al
        mov    ecx, dword ptr [esi + 0aeh]
        mov    byte ptr [esi + 37h], al
        mov    byte ptr [ecx + 1e4bh], 20h
        jmp    NEAR PTR loc_1_20A3
loc_1_1F3D:
        mov    dword ptr [esi + 15ch], 5
        mov    byte ptr [esi + 171h], 0
        mov    ax, word ptr [ecx + 6]
        mov    edi, dword ptr [esi + 62h]
        mov    word ptr [esi + 173h], ax
        mov    ch, al
        mov    byte ptr [esi + 170h], 1
        mov    byte ptr [esi + 172h], 1
        mov    cl, ah
        mov    eax, esi
        mov    word ptr [esi + 173h], cx
        sub    eax, edi
        cmp    eax, -62h
        je     NEAR PTR loc_1_20A3
loc_1_1F82:
        cmp    byte ptr [edi + 1ch], 0
        mov    ebx, edi
        je     SHORT loc_1_1FCA
        mov    byte ptr [edi + 1ch], 0
        xor    eax, eax
        mov    ecx, dword ptr [edi + 10h]
        mov    ax, word ptr [esi + 173h]
        or     eax, 0c0011000h
        push   eax
        push   0c0010007h
        push   ecx
        call   sub_1_3E7E
        mov    edi, dword ptr [edi]
        mov    eax, dword ptr [ebx + 4]
        push   0
        push   96h
        push   ebx
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [ebx + 4]
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [ecx + 4], eax
        call   sub_1_3F4C
        dec    dword ptr [esi + 5eh]
loc_1_1FCA:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -62h
        jne    SHORT loc_1_1F82
        jmp    NEAR PTR loc_1_20A3
loc_1_1FD8:
        cmp    dword ptr [esi + 0cfh], 0
        je     SHORT loc_1_1FFA
        lea    eax, [ebp - 0bh]
        push   eax
        push   esi
        call   sub_1_2D38
        cmp    byte ptr [ebp - 0bh], 0
        je     NEAR PTR loc_1_20A3
        jmp    NEAR PTR loc_1_209D
loc_1_1FFA:
        cmp    byte ptr [esi + 0cch], 0
        jne    NEAR PTR loc_1_20A3
        mov    eax, dword ptr [esi + 0b8h]
        mov    cl, byte ptr [eax + 2]
        mov    al, byte ptr [eax]
        test   al, al
        je     NEAR PTR loc_1_209D
        cmp    al, 8
        je     SHORT loc_1_204C
        cmp    al, 7
        je     SHORT loc_1_204C
        cmp    al, 6
        je     SHORT loc_1_204C
        cmp    al, 1eh
        je     SHORT loc_1_204C
        and    eax, 0ffh
        mov    ecx, dword ptr [esi + 82h]
        push   eax
        push   8
        push   4
        push   3
        push   0aa00000bh
        push   ecx
        call   sub_1_4018
        add    esp, 18h
        jmp    SHORT loc_1_20A3
loc_1_204C:
        mov    eax, dword ptr [esi + 92h]
        cmp    dword ptr [eax + 10h], 8
        jne    SHORT loc_1_208B
        test   cl, cl
        jne    SHORT loc_1_206F
        cmp    dword ptr [eax + 4], 0
        push   esi
        je     SHORT loc_1_206A
        call   sub_1_2F88
        jmp    SHORT loc_1_206F
loc_1_206A:
        call   sub_1_2F18
loc_1_206F:
        push   0
        mov    eax, dword ptr [esi + 92h]
        push   14h
        push   eax
        call   sub_1_3F4C
        mov    dword ptr [esi + 92h], 0
        jmp    SHORT loc_1_209D
loc_1_208B:
        cmp    cl, 1
        sbb    al, al
        neg    al
        push   eax
        push   esi
        call   sub_1_3298
        test   al, al
        je     SHORT loc_1_20A3
loc_1_209D:
        push   esi
        call   sub_1_2DD8
loc_1_20A3:
        test   byte ptr [ebp - 0ah], 4
        je     NEAR PTR loc_1_21C4
        mov    ecx, dword ptr [esi + 0bch]
        mov    al, byte ptr [ecx]
        cmp    al, 0ah
        jne    NEAR PTR loc_1_2197
        xor    eax, eax
        mov    al, byte ptr [esi + 0ceh]
        add    eax, 7fh
        cdq
        xor    eax, edx
        sub    eax, edx
        and    eax, 7fh
        xor    eax, edx
        sub    eax, edx
        mov    dl, byte ptr [ecx + 1]
        mov    byte ptr [ebp - 8], al
        mov    byte ptr [ebp - 9], dl
        mov    al, byte ptr [ecx + 2]
        test   al, al
        jne    SHORT loc_1_20E8
        xor    edi, edi
        jmp    SHORT loc_1_2111
loc_1_20E8:
        cmp    al, 22h
        jne    SHORT loc_1_210C
        mov    al, byte ptr [ecx + 6]
        mov    cl, al
        and    al, 0ch
        shr    cl, 6
        shr    al, 2
        cmp    al, cl
        jne    SHORT loc_1_2105
        test   cl, cl
        jne    SHORT loc_1_2105
        xor    edi, edi
        jmp    SHORT loc_1_2111
loc_1_2105:
        mov    edi, 10001h
        jmp    SHORT loc_1_2111
loc_1_210C:
        mov    edi, 0c0000001h
loc_1_2111:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        inc    eax
        cdq
        xor    eax, edx
        sub    eax, edx
        and    eax, 7fh
        xor    eax, edx
        sub    eax, edx
        xor    edx, edx
        mov    dl, al
        mov    byte ptr [ebp - 8], al
        mov    ecx, dword ptr [esi + edx*4 + 179h]
        test   ecx, ecx
        je     SHORT loc_1_216F
        mov    byte ptr [ecx + 30h], 0
        xor    eax, eax
        test   edi, edi
        mov    al, byte ptr [ecx + 31h]
        mov    dword ptr [esi + eax*4 + 179h], 0
        mov    ebx, dword ptr [ecx + 28h]
        jne    SHORT loc_1_2158
        inc    dword ptr [esi + 0f8h]
        jmp    SHORT loc_1_215E
loc_1_2158:
        inc    dword ptr [esi + 100h]
loc_1_215E:
        push   edi
        mov    eax, dword ptr [ecx + 2ch]
        push   eax
        mov    ecx, dword ptr [ebx + 10h]
        push   ecx
        call   sub_1_3E8A
        dec    dword ptr [ebx + 14h]
loc_1_216F:
        mov    al, byte ptr [ebp - 8]
        cmp    al, byte ptr [ebp - 9]
        jne    SHORT loc_1_2111
        and    eax, 0ffh
        inc    eax
        cdq
        xor    eax, edx
        push   esi
        sub    eax, edx
        and    eax, 7fh
        xor    eax, edx
        sub    eax, edx
        mov    byte ptr [esi + 0ceh], al
        call   sub_1_2DD8
        jmp    SHORT loc_1_21B7
loc_1_2197:
        and    eax, 0ffh
        mov    ecx, dword ptr [esi + 82h]
        push   eax
        push   8
        push   4
        push   3
        push   0aa00000bh
        push   ecx
        call   sub_1_4018
        add    esp, 18h
loc_1_21B7:
        mov    eax, dword ptr [esi + 0aeh]
        mov    byte ptr [eax + 1e4bh], 1
loc_1_21C4:
        lea    eax, [ebp - 0ah]
        lea    ecx, [ebp - 14h]
        push   ecx
        mov    dword ptr [ebp - 14h], esi
        db  068h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 10h], eax
        push   esi
        call   sub_1_3F3A
        test   byte ptr [ebp - 0ah], 24h
        jne    NEAR PTR loc_1_1D28
loc_1_21E6:
        mov    byte ptr [esi + 4ch], 0
        mov    eax, dword ptr [esi + 5eh]
        dec    eax
        mov    dword ptr [esi + 5eh], eax
        jne    SHORT loc_1_220C
        cmp    byte ptr [esi + 160h], 0
        jne    SHORT loc_1_2206
        mov    eax, dword ptr [esi + 6ah]
        sub    eax, esi
        cmp    eax, 6ah
        je     SHORT loc_1_220C
loc_1_2206:
        push   esi
        call   sub_1_35C8
loc_1_220C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_2218:
        push   ebp
        mov    ebp, esp
        sub    esp, 40h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [ebp - 1]
        lea    ecx, [ebp - 0ch]
        push   ecx
        inc    dword ptr [esi + 5eh]
        db  068h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 0ch], esi
        push   esi
        mov    dword ptr [ebp - 8], eax
        mov    byte ptr [esi + 4dh], 1
        call   sub_1_3F3A
        test   byte ptr [ebp - 1], 18h
        je     NEAR PTR loc_1_291C
loc_1_224D:
        test   byte ptr [ebp - 1], 8
        je     NEAR PTR loc_1_27CC
        mov    edi, dword ptr [esi + 0c0h]
        xor    eax, eax
        mov    al, byte ptr [edi]
        sub    eax, 81h
        cmp    eax, 3
        ja     SHORT loc_1_2272
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_2272:
        mov    eax, dword ptr [esi + 0aeh]
        mov    byte ptr [eax + 1e4bh], 2
        jmp    NEAR PTR loc_1_27CC
loc_1_2284:
        cmp    byte ptr [esi + 175h], 0
        je     SHORT loc_1_229A
        cmp    byte ptr [esi + 176h], 0
        je     NEAR PTR loc_1_27CC
loc_1_229A:
        mov    bx, word ptr [edi + 6]
        mov    ecx, dword ptr [esi + 0b2h]
        mov    word ptr [esi + 0d7h], bx
        mov    ah, bl
        mov    al, bh
        mov    edx, eax
        mov    ax, word ptr [edi + 0ah]
        and    edx, 0ffffh
        mov    word ptr [ebp - 24h], ax
        lea    ecx, [edx + ecx + 2]
        mov    di, word ptr [ecx + 4]
        mov    eax, dword ptr [ebp - 24h]
        mov    word ptr [ebp - 20h], di
        mov    al, ah
        mov    ah, byte ptr [ebp - 24h]
        mov    edx, eax
        mov    eax, dword ptr [ebp - 20h]
        and    edx, 0ffffh
        mov    al, ah
        mov    ah, byte ptr [ebp - 20h]
        mov    edi, dword ptr [esi + 0aeh]
        mov    dword ptr [ebp - 24h], edx
        mov    edx, eax
        and    edx, 0ffffh
        mov    dword ptr [ebp - 20h], edx
        lea    edx, [ecx + 6]
        mov    byte ptr [edi + 1e4bh], 2
        test   byte ptr [edx + 8], 80h
        je     SHORT loc_1_2312
        mov    al, byte ptr [edx + 0eh]
        and    eax, 1fh
        add    eax, 0eh
        jmp    SHORT loc_1_2317
loc_1_2312:
        mov    eax, 0eh
loc_1_2317:
        mov    word ptr [esi + 0d9h], ax
        inc    dword ptr [esi + 0fch]
        cmp    eax, dword ptr [ebp - 20h]
        jbe    SHORT loc_1_2348
        cmp    dword ptr [ebp - 20h], 6
        jb     SHORT loc_1_236B
        test   edx, edx
        je     SHORT loc_1_236B
        push   0
        mov    eax, dword ptr [ebp - 20h]
        push   0
        push   0
        push   eax
        push   edx
        push   ecx
        mov    ecx, dword ptr [esi + 86h]
        push   ecx
        jmp    SHORT loc_1_2366
loc_1_2348:
        test   edx, edx
        je     SHORT loc_1_236B
        mov    edi, dword ptr [ebp - 24h]
        sub    edi, eax
        push   edi
        mov    edi, dword ptr [ebp - 20h]
        sub    edi, eax
        push   edi
        lea    edi, [edx + eax]
        push   edi
        push   eax
        push   edx
        mov    eax, dword ptr [esi + 86h]
        push   ecx
        push   eax
loc_1_2366:
        call   sub_1_39C0
loc_1_236B:
        mov    eax, dword ptr [esi + 86h]
        push   eax
        call   sub_1_3C10
        cmp    byte ptr [esi + 0cdh], 0
        je     SHORT loc_1_23A7
        mov    byte ptr [esi + 0cdh], 0
        mov    byte ptr [esi + 0dbh], 0
        push   ebx
        push   esi
        call   sub_1_3558
        mov    eax, dword ptr [esi + 0aeh]
        mov    byte ptr [eax + 1e4bh], 10h
        jmp    NEAR PTR loc_1_27CC
loc_1_23A7:
        mov    ax, word ptr [esi + 0d5h]
        cmp    ax, 0ffffh
        jne    SHORT loc_1_23BD
        mov    word ptr [esi + 0d3h], bx
        jmp    SHORT loc_1_23D2
loc_1_23BD:
        mov    ch, al
        mov    edx, dword ptr [esi + 0b2h]
        mov    cl, ah
        mov    eax, ecx
        and    eax, 0ffffh
        mov    word ptr [eax + edx], bx
loc_1_23D2:
        cmp    byte ptr [esi + 178h], 0
        mov    word ptr [esi + 0d5h], bx
        jne    NEAR PTR loc_1_27CC
        mov    eax, dword ptr [esi + 0aeh]
        mov    byte ptr [esi + 178h], 1
        mov    byte ptr [eax + 1e4bh], 4
        jmp    NEAR PTR loc_1_27CC
loc_1_23FF:
        cmp    byte ptr [esi + 175h], 0
        je     SHORT loc_1_2415
        cmp    byte ptr [esi + 176h], 0
        je     NEAR PTR loc_1_27CC
loc_1_2415:
        xor    eax, eax
        mov    al, byte ptr [edi + 1]
        mov    byte ptr [ebp - 0ch], 0
        mov    ebx, dword ptr [esi + eax*4 + 179h]
        test   ebx, ebx
        jne    SHORT loc_1_2447
        lea    eax, [ebp - 0dh]
        push   eax
        push   esi
        call   sub_1_2D38
        mov    ebx, eax
        test   ebx, ebx
        je     NEAR PTR loc_1_27CC
        cmp    byte ptr [ebp - 0dh], 0
        je     NEAR PTR loc_1_27CC
loc_1_2447:
        mov    ax, word ptr [edi + 6]
        mov    ch, al
        mov    cl, ah
        mov    eax, dword ptr [esi + 0aeh]
        mov    edi, ecx
        lea    ecx, [ebp - 34h]
        and    edi, 0ffffh
        push   ecx
        add    edi, dword ptr [esi + 0b2h]
        push   0
        mov    byte ptr [eax + 1e4bh], 2
        push   0
        push   0
        push   ebx
        call   sub_1_3F70
        lea    ecx, [ebp - 38h]
        mov    edx, dword ptr [ebp - 34h]
        push   ecx
        push   edi
        push   edx
        push   0
        push   ebx
        call   sub_1_1738
        cmp    byte ptr [esi + 0cdh], 0
        je     SHORT loc_1_24B8
        mov    byte ptr [esi + 0cdh], 0
        mov    byte ptr [esi + 0dbh], 1
        push   ebx
        push   esi
        call   sub_1_2EA8
        mov    eax, dword ptr [esi + 0aeh]
        mov    byte ptr [eax + 1e4bh], 10h
        jmp    SHORT loc_1_24DC
loc_1_24B8:
        push   ebx
        push   esi
        call   sub_1_3588
        cmp    byte ptr [esi + 178h], 0
        jne    SHORT loc_1_24DC
        mov    eax, dword ptr [esi + 0aeh]
        mov    byte ptr [esi + 178h], 1
        mov    byte ptr [eax + 1e4bh], 4
loc_1_24DC:
        cmp    byte ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_27CC
        cmp    byte ptr [esi + 0cch], 0
        je     NEAR PTR loc_1_27CC
        mov    byte ptr [esi + 0cch], 0
        push   esi
        call   sub_1_2DD8
        jmp    NEAR PTR loc_1_27CC
loc_1_2505:
        mov    eax, dword ptr [esi + 0aeh]
        mov    byte ptr [eax + 1e4bh], 2
        mov    ecx, dword ptr [esi + 0c0h]
        mov    ax, word ptr [ecx + 6]
        mov    ch, al
        test   ah, 40h
        mov    cl, ah
        je     NEAR PTR loc_1_27CC
        db  0A1h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 1ch]
        push   eax
        push   0
        push   14h
        push   ecx
        call   sub_1_3F46
        test   eax, eax
        je     SHORT loc_1_255F
        push   5
        mov    eax, dword ptr [esi + 82h]
        push   4
        push   2
        push   0aa000002h
        push   eax
        call   sub_1_4018
        add    esp, 14h
        jmp    NEAR PTR loc_1_27CC
loc_1_255F:
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [eax + 10h], 8
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [esi + 8ah]
        test   eax, eax
        jne    SHORT loc_1_2597
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [esi + 8eh], eax
        mov    dword ptr [esi + 8ah], eax
        jmp    SHORT loc_1_25A5
loc_1_2597:
        mov    ecx, dword ptr [ebp - 1ch]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 1ch]
        mov    dword ptr [esi + 8ah], ecx
loc_1_25A5:
        cmp    byte ptr [esi + 0cch], 0
        je     NEAR PTR loc_1_27CC
        mov    byte ptr [esi + 0cch], 0
        push   esi
        call   sub_1_2DD8
        jmp    NEAR PTR loc_1_27CC
loc_1_25C4:
        mov    dword ptr [ebp - 14h], 0
        mov    ax, word ptr [edi + 6]
        mov    ch, al
        mov    cl, ah
        mov    eax, dword ptr [esi + 0aeh]
        mov    byte ptr [eax + 1e4bh], 2
        mov    di, cx
        and    di, 80h
        mov    word ptr [ebp - 0ch], cx
        je     NEAR PTR loc_1_2684
        db  0A1h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 18h]
        push   eax
        push   0
        push   14h
        push   ecx
        call   sub_1_3F46
        test   eax, eax
        je     SHORT loc_1_2628
        push   5
        mov    eax, dword ptr [esi + 82h]
        push   4
        push   2
        push   0aa000002h
        push   eax
        call   sub_1_4018
        add    esp, 14h
        jmp    NEAR PTR loc_1_27CC
loc_1_2628:
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [eax + 10h], 8
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [eax + 4], 1
        mov    eax, dword ptr [esi + 8ah]
        test   eax, eax
        jne    SHORT loc_1_2660
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [esi + 8eh], eax
        mov    dword ptr [esi + 8ah], eax
        jmp    SHORT loc_1_266E
loc_1_2660:
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [esi + 8ah], ecx
loc_1_266E:
        cmp    byte ptr [esi + 0cch], 0
        je     SHORT loc_1_2684
        mov    byte ptr [esi + 0cch], 0
        push   esi
        call   sub_1_2DD8
loc_1_2684:
        mov    ax, word ptr [ebp - 0ch]
        and    ax, 20h
        mov    word ptr [ebp - 2ch], ax
        je     SHORT loc_1_2696
        or     dword ptr [ebp - 14h], 40h
loc_1_2696:
        test   byte ptr [ebp - 0ch], 40h
        je     SHORT loc_1_26A3
        or     dword ptr [ebp - 14h], 80h
loc_1_26A3:
        test   di, di
        je     SHORT loc_1_26AF
        or     dword ptr [ebp - 14h], 100h
loc_1_26AF:
        test   byte ptr [ebp - 0bh], 1
        je     SHORT loc_1_26BC
        or     dword ptr [ebp - 14h], 200h
loc_1_26BC:
        mov    eax, 400h
        test   word ptr [ebp - 0ch], ax
        je     SHORT loc_1_26CA
        or     dword ptr [ebp - 14h], eax
loc_1_26CA:
        mov    ax, word ptr [ebp - 0ch]
        and    ax, 800h
        mov    word ptr [ebp - 30h], ax
        je     SHORT loc_1_26DF
        or     dword ptr [ebp - 14h], 800h
loc_1_26DF:
        mov    eax, 1000h
        test   word ptr [ebp - 0ch], ax
        je     SHORT loc_1_26ED
        or     dword ptr [ebp - 14h], eax
loc_1_26ED:
        mov    eax, 2000h
        test   word ptr [ebp - 0ch], ax
        je     SHORT loc_1_26FB
        or     dword ptr [ebp - 14h], eax
loc_1_26FB:
        mov    ax, word ptr [ebp - 0ch]
        and    ax, 4000h
        mov    word ptr [ebp - 28h], ax
        je     SHORT loc_1_2710
        or     dword ptr [ebp - 14h], 4000h
loc_1_2710:
        mov    eax, 8000h
        test   word ptr [ebp - 0ch], ax
        je     SHORT loc_1_271E
        or     dword ptr [ebp - 14h], eax
loc_1_271E:
        cmp    dword ptr [ebp - 14h], 0
        je     SHORT loc_1_2768
        mov    edi, dword ptr [esi + 62h]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -62h
        je     SHORT loc_1_275F
loc_1_2730:
        lea    ebx, [edi + 14h]
        push   4
        lea    eax, [ebp - 14h]
        inc    dword ptr [ebx]
        push   eax
        mov    ecx, dword ptr [edi + 10h]
        push   40010006h
        push   ecx
        call   sub_1_3EAE
        mov    ecx, dword ptr [edi + 10h]
        push   ecx
        call   sub_1_3EB4
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        dec    dword ptr [ebx]
        cmp    eax, -62h
        jne    SHORT loc_1_2730
loc_1_275F:
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [esi + 158h], eax
loc_1_2768:
        mov    ax, word ptr [ebp - 0ch]
        and    ah, 50h
        cmp    ah, 50h
        jne    SHORT loc_1_2797
        mov    byte ptr [esi + 175h], 1
        push   0c0010008h
        push   esi
        call   sub_1_3748
        cmp    word ptr [ebp - 30h], 800h
        jne    SHORT loc_1_27CC
        mov    byte ptr [esi + 177h], 1
        jmp    SHORT loc_1_27CC
loc_1_2797:
        cmp    word ptr [ebp - 2ch], 0
        je     SHORT loc_1_27CC
        cmp    word ptr [ebp - 28h], 0
        jne    SHORT loc_1_27CC
        cmp    byte ptr [esi + 175h], 0
        je     SHORT loc_1_27CC
        cmp    byte ptr [esi + 176h], 0
        jne    SHORT loc_1_27CC
        mov    byte ptr [esi + 176h], 1
        push   0
        push   esi
        call   sub_1_11B0
        push   esi
        call   sub_1_35C8
loc_1_27CC:
        test   byte ptr [ebp - 1], 10h
        je     NEAR PTR loc_1_28FA
        mov    byte ptr [ebp - 0ch], 0
        xor    bl, bl
        cmp    byte ptr [esi + 175h], bl
        je     SHORT loc_1_27F0
        cmp    byte ptr [esi + 176h], bl
        je     NEAR PTR loc_1_291C
loc_1_27F0:
        mov    cx, word ptr [esi + 0d3h]
        cmp    cx, 0ffffh
        je     SHORT loc_1_2802
        mov    byte ptr [ebp - 0ch], 1
loc_1_2802:
        mov    edx, dword ptr [esi + 0f0h]
        test   edx, edx
        je     SHORT loc_1_280E
        mov    bl, 1
loc_1_280E:
        cmp    byte ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_289D
        test   bl, bl
        je     SHORT loc_1_2825
        cmp    byte ptr [esi + 0dbh], 0
        je     SHORT loc_1_289D
loc_1_2825:
        cmp    word ptr [esi + 0d5h], cx
        jne    SHORT loc_1_2842
        mov    word ptr [esi + 0d3h], 0ffffh
        mov    word ptr [esi + 0d5h], 0ffffh
        jmp    SHORT loc_1_285F
loc_1_2842:
        mov    ah, cl
        mov    edx, dword ptr [esi + 0b2h]
        mov    al, ch
        mov    edi, eax
        and    edi, 0ffffh
        mov    ax, word ptr [edi + edx]
        mov    word ptr [esi + 0d3h], ax
loc_1_285F:
        mov    byte ptr [esi + 0cdh], 0
        mov    byte ptr [esi + 0dbh], 0
        push   ecx
        push   esi
        call   sub_1_3558
        test   bl, bl
        je     SHORT loc_1_2887
        mov    eax, dword ptr [esi + 0aeh]
        mov    byte ptr [eax + 1e4bh], 14h
        jmp    SHORT loc_1_28FA
loc_1_2887:
        mov    eax, dword ptr [esi + 0aeh]
        mov    byte ptr [esi + 178h], 0
        mov    byte ptr [eax + 1e4bh], 10h
        jmp    SHORT loc_1_28FA
loc_1_289D:
        test   bl, bl
        je     SHORT loc_1_28F3
        mov    eax, dword ptr [edx + 24h]
        push   edx
        push   esi
        mov    dword ptr [esi + 0f0h], eax
        mov    byte ptr [esi + 0cdh], 0
        mov    byte ptr [esi + 0dbh], 1
        call   sub_1_2EA8
        cmp    byte ptr [ebp - 0ch], 0
        jne    SHORT loc_1_28E4
        cmp    dword ptr [esi + 0f0h], 0
        jne    SHORT loc_1_28E4
        mov    eax, dword ptr [esi + 0aeh]
        mov    byte ptr [esi + 178h], 0
        mov    byte ptr [eax + 1e4bh], 10h
        jmp    SHORT loc_1_28FA
loc_1_28E4:
        mov    eax, dword ptr [esi + 0aeh]
        mov    byte ptr [eax + 1e4bh], 14h
        jmp    SHORT loc_1_28FA
loc_1_28F3:
        mov    byte ptr [esi + 0cdh], 1
loc_1_28FA:
        lea    eax, [ebp - 1]
        lea    ecx, [ebp - 40h]
        push   ecx
        mov    dword ptr [ebp - 40h], esi
        db  068h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 3ch], eax
        push   esi
        call   sub_1_3F3A
        test   byte ptr [ebp - 1], 18h
        jne    NEAR PTR loc_1_224D
loc_1_291C:
        mov    byte ptr [esi + 4dh], 0
        mov    eax, dword ptr [esi + 5eh]
        dec    eax
        mov    dword ptr [esi + 5eh], eax
        jne    SHORT loc_1_2942
        cmp    byte ptr [esi + 160h], 0
        jne    SHORT loc_1_293C
        mov    eax, dword ptr [esi + 6ah]
        sub    eax, esi
        cmp    eax, 6ah
        je     SHORT loc_1_2942
loc_1_293C:
        push   esi
        call   sub_1_35C8
loc_1_2942:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2968:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 176h], 0
        jne    SHORT loc_1_29F2
        mov    edi, dword ptr [esi + 62h]
        mov    eax, esi
        mov    dword ptr [esi + 15ch], 6
        sub    eax, edi
        cmp    eax, -62h
        je     SHORT loc_1_29CE
loc_1_2993:
        lea    ebx, [edi + 14h]
        inc    dword ptr [ebx]
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_1_29A6
        push   4
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        jmp    SHORT loc_1_29AA
loc_1_29A6:
        push   0
        push   0
loc_1_29AA:
        push   40010007h
        mov    ecx, dword ptr [edi + 10h]
        push   ecx
        call   sub_1_3EAE
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_3EB4
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        dec    dword ptr [ebx]
        cmp    eax, -62h
        jne    SHORT loc_1_2993
loc_1_29CE:
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 10h]
        push   eax
        mov    edx, dword ptr [esi + 82h]
        push   ecx
        push   7
        push   3
        push   4
        push   0aa000003h
        push   edx
        call   sub_1_4018
        add    esp, 1ch
        jmp    SHORT loc_1_2A00
loc_1_29F2:
        mov    byte ptr [esi + 177h], 1
        mov    byte ptr [esi + 176h], 0
loc_1_2A00:
        mov    dword ptr [esi + 161h], 4
        push   esi
        call   0
        xor    ecx, ecx
        mov    eax, dword ptr [esi + 16bh]
        mov    byte ptr [esi + 171h], cl
        mov    byte ptr [esi + 160h], cl
        mov    byte ptr [esi + 169h], cl
        mov    byte ptr [esi + 16ah], cl
        mov    byte ptr [esi + 172h], 1
        test   eax, eax
        je     SHORT loc_1_2A4C
        push   0c0000001h
        dec    dword ptr [eax + 14h]
        mov    ecx, dword ptr [eax + 10h]
        push   ecx
        call   sub_1_3E96
loc_1_2A4C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 049h, 000h
sub_1_2A58:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 161h]
        dec    eax
        cmp    eax, 3
        ja     NEAR PTR loc_1_2D13
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_2A7B:
        mov    eax, dword ptr [esi + 0aeh]
        mov    cl, byte ptr [eax + 1e02h]
        mov    al, byte ptr [eax + 1e03h]
        and    ecx, 0ffff00ffh
        and    eax, 0ffff00ffh
        shl    cx, 8
        mov    ebx, dword ptr [esi + 0b2h]
        add    ax, cx
        mov    word ptr [esi + 0b6h], ax
        mov    edx, eax
        and    edx, 0ffffh
        add    ebx, edx
        mov    ax, word ptr [ebx + 6]
        test   ax, ax
        je     SHORT loc_1_2AD8
        push   1
        and    eax, 0ffffh
        push   eax
        push   0
        push   esi
        call   sub_1_2968
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2AD8:
        mov    edi, ebx
        xor    eax, eax
        mov    byte ptr [esi + 37h], 0
        mov    ecx, 0fh
        rep stosd
        push   6
        mov    byte ptr [ebx], 3
        cmp    byte ptr [esi + 2bh], 1
        sbb    eax, eax
        lea    ecx, [esi + 4eh]
        and    ax, 1000h
        push   ecx
        mov    al, 1
        lea    edx, [ebx + 0ah]
        mov    word ptr [ebx + 8], ax
        push   edx
        call   sub_1_402C
        mov    ax, word ptr [esi + 0e4h]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx + 18h], cx
        mov    ax, word ptr [esi + 0e6h]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx + 1ah], cx
        mov    ax, word ptr [esi + 0deh]
        mov    ch, al
        mov    cl, ah
        pop    edi
        mov    word ptr [ebx + 1ch], cx
        mov    al, byte ptr [esi + 0dch]
        mov    byte ptr [ebx + 1eh], al
        mov    dword ptr [esi + 161h], 2
        mov    edx, dword ptr [esi + 0aeh]
        pop    esi
        pop    ebx
        mov    byte ptr [edx + 1e4bh], 28h
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2B5F:
        mov    eax, dword ptr [esi + 0b2h]
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0b6h]
        lea    edi, [ecx + eax]
        cmp    byte ptr [edi + 2], 0
        je     SHORT loc_1_2BB3
        mov    cx, word ptr [edi + 6]
        mov    word ptr [esi + 173h], cx
        mov    ah, cl
        mov    al, ch
        push   2
        mov    word ptr [esi + 173h], ax
        and    eax, 0ffffh
        mov    ecx, eax
        push   eax
        or     ecx, 0c0011000h
        mov    dword ptr [ebp - 4], ecx
        lea    ecx, [ebp - 4]
        push   ecx
        push   esi
        call   sub_1_2968
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2BB3:
        mov    cx, word ptr [edi + 0ah]
        mov    dh, cl
        mov    dl, ch
        mov    ecx, edx
        and    ecx, 0ffffh
        add    ecx, eax
        mov    dword ptr [esi + 0b8h], ecx
        mov    cx, word ptr [edi + 0eh]
        mov    dh, cl
        mov    dl, ch
        mov    ecx, edx
        and    ecx, 0ffffh
        add    ecx, eax
        mov    dword ptr [esi + 0bch], ecx
        mov    cx, word ptr [edi + 0ch]
        mov    dh, cl
        mov    dl, ch
        mov    ecx, edx
        and    ecx, 0ffffh
        add    ecx, eax
        mov    dword ptr [esi + 0c0h], ecx
        mov    cx, word ptr [edi + 8]
        mov    dword ptr [esi + 0a2h], 0
        mov    dh, cl
        push   esi
        mov    dl, ch
        mov    ecx, edx
        and    ecx, 0ffffh
        add    ecx, eax
        mov    dword ptr [esi + 0c4h], ecx
        call   sub_1_3438
        test   eax, eax
        jne    SHORT loc_1_2C60
        push   esi
        call   sub_1_34E8
        test   eax, eax
        jne    SHORT loc_1_2C40
        push   esi
        call   sub_1_1110
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2C40:
        mov    eax, dword ptr [esi + 0aeh]
        pop    edi
        mov    dword ptr [esi + 161h], 4
        mov    byte ptr [eax + 1e4bh], 28h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2C60:
        mov    eax, dword ptr [esi + 0aeh]
        pop    edi
        mov    dword ptr [esi + 161h], 3
        mov    byte ptr [eax + 1e4bh], 28h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2C80:
        mov    eax, dword ptr [esi + 0b8h]
        mov    al, byte ptr [eax + 2]
        test   al, al
        jne    SHORT loc_1_2CC6
        push   esi
        call   sub_1_34E8
        test   eax, eax
        jne    SHORT loc_1_2CA6
        push   esi
        call   sub_1_1110
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2CA6:
        mov    eax, dword ptr [esi + 0aeh]
        pop    edi
        mov    dword ptr [esi + 161h], 4
        mov    byte ptr [eax + 1e4bh], 28h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2CC6:
        cmp    al, 0feh
        je     SHORT loc_1_2D13
        push   3
        and    eax, 0ffh
        push   eax
        push   0
        push   esi
        call   sub_1_2968
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2CE3:
        mov    eax, dword ptr [esi + 0b8h]
        mov    al, byte ptr [eax + 2]
        test   al, al
        jne    SHORT loc_1_2CFF
        push   esi
        call   sub_1_1110
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2CFF:
        cmp    al, 0feh
        je     SHORT loc_1_2D13
        push   4
        and    eax, 0ffh
        push   eax
        push   0
        push   esi
        call   sub_1_2968
loc_1_2D13:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2D38:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 0b8h]
        mov    dl, byte ptr [esi + 2]
        cmp    dl, 0feh
        jne    SHORT loc_1_2D5C
        mov    ecx, dword ptr [ebp + 0ch]
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        mov    byte ptr [ecx], 0
        ret    8
loc_1_2D5C:
        mov    eax, dword ptr [edi + 0cfh]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [edi + 0cfh], 0
        test   eax, eax
        jne    SHORT loc_1_2D7E
        xor    eax, eax
        pop    edi
        pop    esi
        mov    byte ptr [ecx], 0
        pop    ebp
        ret    8
loc_1_2D7E:
        mov    byte ptr [ecx], 1
        cmp    dl, 0ffh
        lea    ecx, [eax + 24h]
        je     SHORT loc_1_2DAF
        mov    esi, dword ptr [ecx + 4]
        push   0c0000001h
        inc    dword ptr [edi + 100h]
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [esi + 10h]
        push   eax
        push   ecx
        call   sub_1_3E8A
        xor    eax, eax
        pop    edi
        dec    dword ptr [esi + 14h]
        pop    esi
        pop    ebp
        ret    8
loc_1_2DAF:
        mov    byte ptr [ecx + 0ch], 1
        mov    dl, byte ptr [esi + 1]
        mov    byte ptr [ecx + 0dh], dl
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 31h]
        mov    dword ptr [edi + ecx*4 + 179h], eax
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2DD8:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 8ah], 0
        je     SHORT loc_1_2E07
        push   esi
        call   sub_1_3058
        cmp    eax, 103h
        jne    SHORT loc_1_2E07
        mov    eax, dword ptr [esi + 0aeh]
        pop    esi
        pop    ebp
        mov    byte ptr [eax + 1e4bh], 20h
        ret    4
loc_1_2E07:
        mov    eax, dword ptr [esi + 0e8h]
        test   eax, eax
        je     SHORT loc_1_2E39
        mov    ecx, dword ptr [eax + 24h]
        push   eax
        push   esi
        mov    dword ptr [esi + 0e8h], ecx
        mov    dword ptr [esi + 0cfh], eax
        call   sub_1_2E78
        mov    ecx, dword ptr [esi + 0aeh]
        pop    esi
        pop    ebp
        mov    byte ptr [ecx + 1e4bh], 20h
        ret    4
loc_1_2E39:
        mov    byte ptr [esi + 0cch], 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_2E48:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0b8h]
        mov    byte ptr [eax + 0cch], 0
        pop    ebp
        mov    byte ptr [ecx], 0
        mov    byte ptr [ecx + 2], 0feh
        mov    edx, dword ptr [eax + 0aeh]
        mov    byte ptr [edx + 1e4bh], 20h
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_2E78:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        pop    ebp
        mov    eax, dword ptr [eax + 0b8h]
        mov    byte ptr [eax], 0ah
        mov    byte ptr [eax + 1], cl
        mov    byte ptr [eax + 2], 0feh
        mov    word ptr [eax + 4], cx
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2EA8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        mov    esi, dword ptr [ebp + 0ch]
        push   0
        push   0
        push   0
        push   esi
        call   sub_1_3F70
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0c4h]
        mov    byte ptr [eax], 0ah
        mov    dl, byte ptr [esi + 31h]
        pop    esi
        mov    byte ptr [eax + 1], dl
        mov    byte ptr [eax + 2], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    cl, ch
        mov    ch, byte ptr [ebp - 4]
        mov    word ptr [eax + 6], cx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2EF8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [eax + 0b8h]
        pop    ebp
        mov    byte ptr [eax], 1eh
        mov    word ptr [eax + 4], 0
        mov    byte ptr [eax + 2], 80h
        ret    4
        db  08Bh, 0FFh
sub_1_2F18:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ecx + 0b8h]
        mov    dx, word ptr [eax + 6]
        mov    bh, dl
        mov    bl, dh
        mov    edx, ebx
        and    edx, 0ffffh
        add    edx, eax
        mov    ax, word ptr [edx]
        mov    bh, al
        mov    bl, ah
        mov    eax, ebx
        and    eax, 0ffffh
        add    dword ptr [ecx + 0f8h], eax
        mov    ax, word ptr [edx + 2]
        mov    bh, al
        mov    bl, ah
        mov    eax, ebx
        xor    ebx, ebx
        and    eax, 0ffffh
        add    dword ptr [ecx + 0fch], eax
        mov    bl, byte ptr [edx + 4]
        add    dword ptr [ecx + 100h], ebx
        pop    ebx
        mov    dl, byte ptr [edx + 5]
        pop    ebp
        and    edx, 0ffh
        add    dword ptr [ecx + 104h], edx
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_2F88:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    edx, dword ptr [ecx + 0b8h]
        mov    al, byte ptr [edx + 6]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 108h], eax
        mov    al, byte ptr [edx + 7]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 10ch], eax
        mov    al, byte ptr [edx + 8]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 110h], eax
        mov    al, byte ptr [edx + 9]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 114h], eax
        mov    al, byte ptr [edx + 0ah]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 118h], eax
        mov    al, byte ptr [edx + 0ch]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 11ch], eax
        mov    al, byte ptr [edx + 0dh]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 120h], eax
        mov    al, byte ptr [edx + 0eh]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 124h], eax
        mov    al, byte ptr [edx + 0fh]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 128h], eax
        mov    dl, byte ptr [edx + 10h]
        mov    byte ptr [ebp - 4], dl
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 12ch], eax
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_3038:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0b8h]
        pop    ebp
        mov    byte ptr [ecx], 8
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3058:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 8ah], 0
        je     NEAR PTR loc_1_3227
        mov    edi, dword ptr [ebp - 4]
loc_1_3074:
        mov    ebx, dword ptr [esi + 8ah]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi + 8ah], eax
        test   eax, eax
        jne    SHORT loc_1_3090
        mov    dword ptr [esi + 8eh], 0
loc_1_3090:
        mov    ecx, dword ptr [ebx + 10h]
        cmp    ecx, 8
        jne    SHORT loc_1_30C2
        cmp    dword ptr [ebx + 4], 0
        push   esi
        je     SHORT loc_1_30A6
        call   sub_1_3038
        jmp    SHORT loc_1_30AB
loc_1_30A6:
        call   sub_1_2EF8
loc_1_30AB:
        mov    eax, dword ptr [esi + 0aeh]
        mov    edi, 103h
        mov    byte ptr [eax + 1e4bh], 20h
        jmp    NEAR PTR loc_1_3195
loc_1_30C2:
        lea    eax, [ecx - 1]
        cmp    eax, 9
        ja     SHORT loc_1_30D9
        xor    edx, edx
        db  08Ah, 090h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 095h, 000h, 000h, 000h, 000h
loc_1_30D9:
        push   ecx
        mov    eax, dword ptr [esi + 82h]
        push   1
        push   9
        push   3
        push   0aa000006h
        push   eax
        call   sub_1_4018
        add    esp, 18h
        jmp    NEAR PTR loc_1_3195
loc_1_30F9:
        mov    eax, dword ptr [ebx + 14h]
        cmp    eax, 1010eh
        jne    SHORT loc_1_3120
        mov    eax, dword ptr [esi + 96h]
        push   esi
        mov    dword ptr [esi + 9ah], eax
        mov    ecx, dword ptr [ebx + 8]
        mov    dword ptr [esi + 96h], ecx
        call   sub_1_3438
        jmp    SHORT loc_1_3193
loc_1_3120:
        cmp    eax, 2010103h
        mov    eax, dword ptr [ebx + 8]
        jne    SHORT loc_1_3187
        push   esi
        mov    dword ptr [esi + 9eh], eax
        call   sub_1_3438
        jmp    SHORT loc_1_3193
loc_1_3138:
        mov    edi, 103h
        push   esi
        call   sub_1_3038
        mov    byte ptr [ebx + 4], 1
        mov    eax, dword ptr [esi + 0aeh]
        mov    byte ptr [eax + 1e4bh], 20h
        jmp    SHORT loc_1_3195
loc_1_3156:
        mov    eax, dword ptr [esi + 96h]
        push   esi
        mov    dword ptr [esi + 9ah], eax
        mov    ecx, dword ptr [ebx + 8]
        mov    dword ptr [esi + 96h], ecx
        call   sub_1_3438
        jmp    SHORT loc_1_3193
loc_1_3173:
        mov    eax, dword ptr [ebx + 8]
        push   esi
        mov    dword ptr [esi + 9eh], eax
        call   sub_1_3438
        jmp    SHORT loc_1_3193
loc_1_3184:
        mov    eax, dword ptr [ebx + 8]
loc_1_3187:
        push   esi
        mov    dword ptr [esi + 0a6h], eax
        call   sub_1_34E8
loc_1_3193:
        mov    edi, eax
loc_1_3195:
        cmp    edi, 103h
        je     NEAR PTR loc_1_3232
        test   edi, edi
        jne    SHORT loc_1_31FF
        mov    ecx, dword ptr [ebx + 10h]
        lea    eax, [ecx - 1]
        cmp    eax, 9
        ja     SHORT loc_1_31BF
        xor    edx, edx
        db  08Ah, 090h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 095h, 000h, 000h, 000h, 000h
loc_1_31BF:
        push   ecx
        push   9
        jmp    SHORT loc_1_3202
loc_1_31C4:
        mov    eax, dword ptr [ebx + 4]
        push   0
        push   ebx
        mov    dword ptr [ebp - 4], eax
        mov    ecx, dword ptr [eax + 10h]
        push   ecx
        call   sub_1_3E9C
        mov    ecx, dword ptr [ebp - 4]
        dec    dword ptr [ecx + 14h]
        jmp    SHORT loc_1_321A
loc_1_31DE:
        push   0
        mov    eax, dword ptr [esi + 7eh]
        push   ebx
        push   eax
        call   sub_1_3EBA
        dec    dword ptr [esi + 5eh]
        jmp    SHORT loc_1_321A
loc_1_31EF:
        mov    eax, dword ptr [ebx + 4]
        dec    dword ptr [eax + 14h]
        jmp    SHORT loc_1_321A
loc_1_31F7:
        mov    eax, dword ptr [ebx + 4]
        dec    dword ptr [eax + 14h]
        jmp    SHORT loc_1_321A
loc_1_31FF:
        push   edi
        push   0ah
loc_1_3202:
        push   5
        mov    eax, dword ptr [esi + 82h]
        push   3
        push   0aa000006h
        push   eax
        call   sub_1_4018
        add    esp, 18h
loc_1_321A:
        cmp    dword ptr [esi + 8ah], 0
        jne    NEAR PTR loc_1_3074
loc_1_3227:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3232:
        mov    eax, 103h
        pop    edi
        mov    dword ptr [esi + 92h], ebx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 005h, 002h, 005h, 005h
        db  005h, 005h, 003h, 004h, 08Bh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 004h, 002h, 004h, 004h
        db  004h, 004h, 003h, 003h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h
        db  000h, 000h
sub_1_3298:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 92h]
        mov    eax, dword ptr [edi + 10h]
        dec    eax
        cmp    eax, 9
        ja     NEAR PTR loc_1_336E
        xor    ecx, ecx
        db  08Ah, 088h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_1_32C3:
        cmp    byte ptr [ebp + 0ch], 0
        je     SHORT loc_1_32FC
        cmp    dword ptr [edi + 14h], 2010104h
        jne    SHORT loc_1_32DE
        mov    eax, dword ptr [esi + 0a6h]
        mov    dword ptr [esi + 0aah], eax
loc_1_32DE:
        mov    dword ptr [esi + 92h], 0
        mov    ebx, dword ptr [edi + 4]
        push   0
        push   edi
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_3E9C
        dec    dword ptr [ebx + 14h]
        jmp    SHORT loc_1_336E
loc_1_32FC:
        mov    dword ptr [esi + 92h], 0
        mov    ebx, dword ptr [edi + 4]
        push   0c0000001h
        push   edi
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_3E9C
        dec    dword ptr [ebx + 14h]
        jmp    SHORT loc_1_336E
loc_1_331D:
        cmp    byte ptr [ebp + 0ch], 0
        je     SHORT loc_1_334A
        push   esi
        call   sub_1_2F88
        push   edi
        push   esi
        call   sub_1_0C30
        mov    dword ptr [esi + 92h], 0
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 7eh]
        push   eax
        call   sub_1_3EBA
        dec    dword ptr [esi + 5eh]
        jmp    SHORT loc_1_336E
loc_1_334A:
        push   0c0000001h
        mov    eax, dword ptr [esi + 7eh]
        mov    dword ptr [esi + 92h], 0
        push   edi
        push   eax
        call   sub_1_3EBA
        dec    dword ptr [esi + 5eh]
        jmp    SHORT loc_1_336E
loc_1_3368:
        mov    eax, dword ptr [edi + 4]
        dec    dword ptr [eax + 14h]
loc_1_336E:
        cmp    byte ptr [ebp + 0ch], 0
        jne    SHORT loc_1_33E4
        mov    ecx, dword ptr [edi + 10h]
        lea    eax, [ecx - 1]
        cmp    eax, 9
        ja     SHORT loc_1_338E
        xor    edx, edx
        db  08Ah, 090h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 095h, 000h, 000h, 000h, 000h
loc_1_338E:
        push   ecx
        mov    eax, dword ptr [esi + 82h]
        push   9
        push   6
        push   3
        push   0aa000006h
        push   eax
        call   sub_1_4018
        add    esp, 18h
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_33B2:
        cmp    dword ptr [edi + 14h], 1010eh
        jne    SHORT loc_1_33DA
        mov    eax, dword ptr [esi + 9ah]
        pop    edi
        mov    dword ptr [esi + 0a2h], 0
        mov    dword ptr [esi + 96h], eax
        mov    al, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_33DA:
        mov    dword ptr [esi + 9eh], 0
loc_1_33E4:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 004h, 002h, 004h, 004h, 004h, 004h, 003h
        db  003h, 08Bh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 004h, 002h, 004h, 004h, 004h, 004h, 003h
        db  003h, 08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3438:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 96h]
        test   ah, 20h
        je     SHORT loc_1_3452
        mov    esi, 7fffffffh
        jmp    SHORT loc_1_3462
loc_1_3452:
        test   ah, 40h
        mov    esi, 0
        je     SHORT loc_1_3462
        mov    esi, dword ptr [edi + 9eh]
loc_1_3462:
        cmp    dword ptr [edi + 0a2h], esi
        jne    SHORT loc_1_3472
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_3472:
        push   esi
        push   edi
        call   sub_1_3498
        mov    eax, 103h
        mov    dword ptr [edi + 0a2h], esi
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3498:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    edx, dword ptr [eax + 0b8h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, ecx
        mov    byte ptr [edx], 7
        mov    byte ptr [edx + 2], 0feh
        shr    eax, 18h
        mov    byte ptr [ebp - 4], al
        mov    eax, ecx
        shr    eax, 10h
        mov    byte ptr [ebp - 3], al
        xor    eax, eax
        mov    byte ptr [ebp - 2], ch
        mov    byte ptr [ebp - 1], cl
loc_1_34CA:
        mov    cl, byte ptr [ebp + eax - 4]
        inc    eax
        cmp    eax, 4
        mov    byte ptr [edx + eax + 5], cl
        jb     SHORT loc_1_34CA
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_34E8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0a6h]
        push   ecx
        push   eax
        call   sub_1_3508
        mov    eax, 103h
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_3508:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    edx, dword ptr [eax + 0b8h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, ecx
        mov    byte ptr [edx], 6
        mov    byte ptr [edx + 2], 0feh
        shr    eax, 18h
        mov    byte ptr [ebp - 4], al
        mov    eax, ecx
        shr    eax, 10h
        mov    byte ptr [ebp - 3], al
        xor    eax, eax
        mov    byte ptr [ebp - 2], ch
        mov    byte ptr [ebp - 1], cl
loc_1_353A:
        mov    cl, byte ptr [ebp + eax - 4]
        inc    eax
        cmp    eax, 4
        mov    byte ptr [edx + eax + 5], cl
        jb     SHORT loc_1_353A
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3558:
        push   ebp
        xor    ecx, ecx
        mov    dx, word ptr [esp + 0ch]
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        pop    ebp
        mov    eax, dword ptr [eax + 0c4h]
        mov    byte ptr [eax], 81h
        mov    byte ptr [eax + 2], cl
        mov    word ptr [eax + 4], cx
        mov    word ptr [eax + 6], dx
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3588:
        push   ebp
        mov    ecx, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   esi
        lea    edx, [ecx + 24h]
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 0f0h]
        test   esi, esi
        jne    SHORT loc_1_35A8
        mov    dword ptr [eax + 0f0h], ecx
        jmp    SHORT loc_1_35AB
loc_1_35A8:
        mov    dword ptr [esi + 24h], ecx
loc_1_35AB:
        pop    esi
        mov    dword ptr [eax + 0f4h], ecx
        pop    ebp
        mov    dword ptr [edx], 0
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_35C8:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 5eh]
        cmp    byte ptr [esi + 160h], 0
        je     SHORT loc_1_3602
        cmp    dword ptr [esi + 161h], 0
        jne    SHORT loc_1_35F5
        mov    dword ptr [esi + 161h], 1
        push   esi
        call   sub_1_1090
loc_1_35F5:
        cmp    byte ptr [esi + 160h], 0
        jne    NEAR PTR loc_1_36E8
loc_1_3602:
        mov    edi, dword ptr [esi + 72h]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, 72h
        je     NEAR PTR loc_1_36E8
        push   0
        inc    dword ptr [edi + 14h]
        mov    eax, dword ptr [edi + 0ch]
        mov    ecx, dword ptr [esi + 86h]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_3627:
        test   eax, eax
        jne    SHORT loc_1_3684
        mov    eax, dword ptr [edi]
        mov    ecx, dword ptr [edi + 4]
        cmp    dword ptr [edi + 14h], 2
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edi]
        mov    edx, dword ptr [edi + 4]
        mov    dword ptr [eax + 4], edx
        jne    SHORT loc_1_366B
        mov    ecx, dword ptr [esi + 165h]
        cmp    dword ptr [edi + 18h], ecx
        jne    SHORT loc_1_3651
        push   esi
        call   sub_1_02C0
loc_1_3651:
        push   0
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_3E84
        push   0
        push   96h
        push   edi
        call   sub_1_3F4C
        jmp    SHORT loc_1_36E8
loc_1_366B:
        lea    ecx, [esi + 6ah]
        mov    dword ptr [edi], ecx
        mov    edx, dword ptr [esi + 6eh]
        mov    dword ptr [edi + 4], edx
        mov    edx, dword ptr [esi + 6eh]
        mov    dword ptr [edx], edi
        mov    dword ptr [esi + 6eh], edi
        sub    dword ptr [edi + 14h], 2
        jmp    SHORT loc_1_36E8
loc_1_3684:
        cmp    eax, 103h
        jne    SHORT loc_1_36CD
        cmp    byte ptr [esi + 0cch], 0
        je     SHORT loc_1_36A1
        mov    byte ptr [esi + 0cch], 0
        push   esi
        call   sub_1_2DD8
loc_1_36A1:
        mov    byte ptr [edi + 1dh], 1
        mov    eax, dword ptr [edi]
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edi]
        mov    edx, dword ptr [edi + 4]
        lea    ecx, [esi + 6ah]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [edi], ecx
        mov    edx, dword ptr [esi + 6eh]
        mov    dword ptr [edi + 4], edx
        mov    edx, dword ptr [esi + 6eh]
        mov    dword ptr [edx], edi
        mov    dword ptr [esi + 6eh], edi
        sub    dword ptr [edi + 14h], 2
        jmp    SHORT loc_1_36E8
loc_1_36CD:
        push   eax
        push   0ah
        push   7
        mov    eax, dword ptr [esi + 82h]
        push   3
        push   0aa000006h
        push   eax
        call   sub_1_4018
        add    esp, 18h
loc_1_36E8:
        mov    edi, dword ptr [esi + 6ah]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, 6ah
        je     SHORT loc_1_3732
        cmp    dword ptr [edi + 14h], 0
        jne    SHORT loc_1_3732
        mov    eax, dword ptr [esi + 165h]
        cmp    dword ptr [edi + 18h], eax
        jne    SHORT loc_1_370B
        push   esi
        call   sub_1_02C0
loc_1_370B:
        push   0
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_3E84
        mov    eax, dword ptr [edi]
        mov    ecx, dword ptr [edi + 4]
        push   0
        push   96h
        push   edi
        mov    dword ptr [ecx], eax
        mov    edx, dword ptr [edi + 4]
        mov    eax, dword ptr [edi]
        mov    dword ptr [eax + 4], edx
        call   sub_1_3F4C
loc_1_3732:
        pop    edi
        dec    dword ptr [esi + 5eh]
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3748:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        cmp    dword ptr [esi + 8ah], 0
        je     SHORT loc_1_37D9
loc_1_375D:
        mov    ecx, dword ptr [esi + 8ah]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 8ah], eax
        test   eax, eax
        jne    SHORT loc_1_3779
        mov    dword ptr [esi + 8eh], 0
loc_1_3779:
        mov    eax, dword ptr [ecx + 10h]
        dec    eax
        cmp    eax, 9
        ja     SHORT loc_1_37D0
        xor    edx, edx
        db  08Ah, 090h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 095h, 000h, 000h, 000h, 000h
loc_1_3791:
        mov    ebx, dword ptr [ecx + 4]
        push   edi
        push   ecx
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_3E9C
        dec    dword ptr [ebx + 14h]
        jmp    SHORT loc_1_37D0
loc_1_37A4:
        push   edi
        mov    eax, dword ptr [esi + 7eh]
        push   ecx
        push   eax
        call   sub_1_3EBA
        dec    dword ptr [esi + 5eh]
        jmp    SHORT loc_1_37D0
loc_1_37B4:
        push   0
        push   14h
        push   ecx
        call   sub_1_3F4C
        mov    dword ptr [esi + 92h], 0
        jmp    SHORT loc_1_37D0
loc_1_37CA:
        mov    eax, dword ptr [ecx + 4]
        dec    dword ptr [eax + 14h]
loc_1_37D0:
        cmp    dword ptr [esi + 8ah], 0
        jne    SHORT loc_1_375D
loc_1_37D9:
        push   edi
        push   esi
        call   sub_1_3818
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  090h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 005h, 002h, 005h, 005h, 005h
        db  003h, 004h, 004h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h
        db  000h
sub_1_3818:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0cfh]
        test   ecx, ecx
        je     SHORT loc_1_384C
        mov    dword ptr [esi + 0cfh], 0
        mov    edi, dword ptr [ecx + 28h]
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   ecx
        mov    ecx, dword ptr [edi + 10h]
        push   ecx
        call   sub_1_3E8A
        dec    dword ptr [edi + 14h]
loc_1_384C:
        cmp    dword ptr [esi + 0e8h], 0
        je     SHORT loc_1_3883
loc_1_3855:
        mov    edx, dword ptr [esi + 0e8h]
        lea    eax, [edx + 24h]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 0e8h], ecx
        mov    edi, dword ptr [eax + 4]
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   edx
        mov    ecx, dword ptr [edi + 10h]
        push   ecx
        call   sub_1_3E8A
        dec    dword ptr [edi + 14h]
        cmp    dword ptr [esi + 0e8h], 0
        jne    SHORT loc_1_3855
loc_1_3883:
        mov    dword ptr [ebp - 4], 80h
        lea    edi, [esi + 179h]
loc_1_3890:
        mov    ecx, dword ptr [edi]
        test   ecx, ecx
        je     SHORT loc_1_38C1
        xor    eax, eax
        mov    edx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 30h], 0
        mov    al, byte ptr [ecx + 31h]
        push   edx
        mov    dword ptr [esi + eax*4 + 179h], 0
        mov    ebx, dword ptr [ecx + 28h]
        mov    eax, dword ptr [ecx + 2ch]
        push   eax
        mov    ecx, dword ptr [ebx + 10h]
        push   ecx
        call   sub_1_3E8A
        dec    dword ptr [ebx + 14h]
loc_1_38C1:
        add    edi, 4
        dec    dword ptr [ebp - 4]
        jne    SHORT loc_1_3890
        mov    dword ptr [esi + 0f0h], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_38E8:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 29h], 0
        je     SHORT loc_1_3959
        cmp    dword ptr [esi + 0cfh], 0
        jne    SHORT loc_1_3910
        cmp    dword ptr [esi + 0e8h], 0
        jne    SHORT loc_1_3910
        cmp    dword ptr [esi + 8ah], 0
        je     SHORT loc_1_3959
loc_1_3910:
        mov    byte ptr [esi + 172h], 1
        mov    byte ptr [esi + 29h], 0
        push   0
        push   esi
        call   sub_1_3748
        mov    al, byte ptr [esi + 2ah]
        cmp    al, 0ah
        jae    SHORT loc_1_3953
        inc    al
        push   0
        push   0aa000003h
        mov    byte ptr [esi + 2ah], al
        mov    ecx, dword ptr [esi + 82h]
        push   ecx
        call   sub_1_4018
        add    esp, 0ch
        push   0
        push   esi
        call   sub_1_11B0
        push   esi
        call   sub_1_35C8
loc_1_3953:
        mov    byte ptr [esi + 29h], 0
        jmp    SHORT loc_1_3978
loc_1_3959:
        cmp    dword ptr [esi + 0cfh], 0
        jne    SHORT loc_1_3974
        cmp    dword ptr [esi + 0e8h], 0
        jne    SHORT loc_1_3974
        cmp    dword ptr [esi + 8ah], 0
        je     SHORT loc_1_3978
loc_1_3974:
        mov    byte ptr [esi + 29h], 1
loc_1_3978:
        cmp    byte ptr [esi + 177h], 0
        je     SHORT loc_1_399F
        cmp    byte ptr [esi + 176h], 0
        jne    SHORT loc_1_399F
        mov    byte ptr [esi + 176h], 1
        push   0
        push   esi
        call   sub_1_11B0
        push   esi
        call   sub_1_35C8
loc_1_399F:
        push   7530h
        add    esi, 1dh
        push   esi
        call   sub_1_3F22
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_39C0:
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
        call   sub_1_39F0
        pop    ebp
        ret    1ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_39F0:
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
        jb     NEAR PTR loc_1_3B10
        mov    al, byte ptr [eax]
        shr    al, 7
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ecx]
        and    al, 80h
        mov    byte ptr [ebp - 2], al
        or     al, al
        je     NEAR PTR loc_1_3AB4
        mov    al, byte ptr [ecx]
        cmp    al, 0ffh
        jne    SHORT loc_1_3A32
        cmp    byte ptr [ecx + 1], 0ffh
        je     SHORT loc_1_3A3C
loc_1_3A32:
        cmp    al, 0c0h
        jne    SHORT loc_1_3A58
        cmp    byte ptr [ecx + 1], 0
        jne    SHORT loc_1_3A58
loc_1_3A3C:
        cmp    byte ptr [ecx + 2], 0ffh
        jne    SHORT loc_1_3A58
        cmp    byte ptr [ecx + 3], 0ffh
        jne    SHORT loc_1_3A58
        cmp    byte ptr [ecx + 4], 0ffh
        jne    SHORT loc_1_3A58
        cmp    byte ptr [ecx + 5], 0ffh
        mov    byte ptr [ebp - 2], 1
        je     SHORT loc_1_3A5C
loc_1_3A58:
        mov    byte ptr [ebp - 2], 0
loc_1_3A5C:
        cmp    byte ptr [ebp - 2], 0
        je     SHORT loc_1_3A70
        mov    dword ptr [ebp - 8], 8
        jmp    NEAR PTR loc_1_3B1B
        db  08Bh, 0FFh
loc_1_3A70:
        mov    al, byte ptr [ecx]
        and    al, byte ptr [ecx + 2]
        and    al, 80h
        mov    byte ptr [ebp - 2], al
        or     al, al
        mov    dword ptr [ebp - 8], 1000h
        jne    SHORT loc_1_3A8C
        mov    dword ptr [ebp - 8], 4000h
loc_1_3A8C:
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
        jmp    SHORT loc_1_3B1B
loc_1_3AB4:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 10h], 30h
        je     SHORT loc_1_3B00
        mov    edx, dword ptr [esi + 1ah]
        lea    eax, [esi + 18h]
        cmp    dword ptr [ecx + 2], edx
        jne    SHORT loc_1_3AD7
        mov    ax, word ptr [eax]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_3ADE
loc_1_3AD7:
        mov    dword ptr [ebp - 10h], 1
loc_1_3ADE:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_3AF0
        mov    dword ptr [ebp - 8], 1
        jmp    SHORT loc_1_3B1E
        db  08Dh, 049h, 000h
loc_1_3AF0:
        mov    dword ptr [ebp - 8], 0
        jmp    SHORT loc_1_3B1E
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3B00:
        mov    dword ptr [ebp - 8], 1
        jmp    SHORT loc_1_3B1E
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3B10:
        mov    dword ptr [ebp - 8], 20h
        mov    byte ptr [ebp - 1], 0
loc_1_3B1B:
        mov    esi, dword ptr [ebp + 8]
loc_1_3B1E:
        mov    edi, dword ptr [esi + 14h]
        or     edi, edi
        je     NEAR PTR loc_1_3C01
loc_1_3B29:
        mov    ecx, dword ptr [edi + 8]
        mov    eax, ecx
        and    eax, dword ptr [ebp - 8]
        test   al, 9
        jne    SHORT loc_1_3B6D
        test   ah, 40h
        je     SHORT loc_1_3B42
        mov    eax, dword ptr [edi + 0ch]
        test   dword ptr [ebp - 0ch], eax
        jne    SHORT loc_1_3B6D
loc_1_3B42:
        test   byte ptr [ebp - 7], 40h
        je     SHORT loc_1_3B4D
        test   ch, 20h
        jne    SHORT loc_1_3B6D
loc_1_3B4D:
        test   byte ptr [ebp - 7], 10h
        je     SHORT loc_1_3B59
        cmp    byte ptr [edi + 1dh], 0
        jne    SHORT loc_1_3B6D
loc_1_3B59:
        test   cl, 10h
        je     SHORT loc_1_3B64
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_1_3B6D
loc_1_3B64:
        test   cl, 20h
        je     NEAR PTR loc_1_3BF6
loc_1_3B6D:
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
        call   sub_1_3EA2
        mov    byte ptr [edi + 1eh], 1
        mov    eax, dword ptr [edi + 18h]
        dec    eax
        mov    dword ptr [edi + 18h], eax
        jne    SHORT loc_1_3BF6
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_3BAF
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_3BAF:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_3BC0
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_3BC6
        db  08Bh, 0FFh
loc_1_3BC0:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 14h], eax
loc_1_3BC6:
        cmp    byte ptr [edi + 1eh], 0
        je     SHORT loc_1_3BD5
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_3EA8
loc_1_3BD5:
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
        call   sub_1_3F4C
        mov    edi, ebx
        jmp    SHORT loc_1_3BF9
loc_1_3BF6:
        mov    edi, dword ptr [edi + 10h]
loc_1_3BF9:
        or     edi, edi
        jne    NEAR PTR loc_1_3B29
loc_1_3C01:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_3C10:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3C20
        pop    ebp
        ret    4
sub_1_3C20:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 14h]
        or     edi, edi
        je     SHORT loc_1_3C9E
loc_1_3C30:
        inc    dword ptr [edi + 18h]
        cmp    byte ptr [edi + 1eh], 0
        je     SHORT loc_1_3C46
        mov    byte ptr [edi + 1eh], 0
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_3EA8
loc_1_3C46:
        mov    eax, dword ptr [edi + 18h]
        dec    eax
        mov    dword ptr [edi + 18h], eax
        jne    SHORT loc_1_3C97
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_3C5C
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_3C5C:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_3C70
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_3C76
        db  005h, 000h, 000h, 000h, 000h
loc_1_3C70:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 14h], eax
loc_1_3C76:
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
        call   sub_1_3F4C
        mov    edi, ebx
        jmp    SHORT loc_1_3C9A
loc_1_3C97:
        mov    edi, dword ptr [edi + 10h]
loc_1_3C9A:
        or     edi, edi
        jne    SHORT loc_1_3C30
loc_1_3C9E:
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
        je     SHORT loc_1_3D02
        cmp    dword ptr [eax + 8], ecx
        jne    SHORT loc_1_3D02
        cmp    dword ptr [eax + 0ch], 0
        je     SHORT loc_1_3D02
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_3D02:
        mov    esi, dword ptr [eax + 10h]
        or     esi, esi
        jne    SHORT loc_1_3D12
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_3D12:
        test   esi, 20h
        je     SHORT loc_1_3D23
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_3D23:
        mov    al, byte ptr [edx]
        shr    al, 7
        or     al, al
        je     SHORT loc_1_3D40
        test   esi, 10h
        je     SHORT loc_1_3D40
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h
loc_1_3D40:
        mov    eax, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [eax]
        and    cl, 80h
        mov    byte ptr [ebp - 1], cl
        or     cl, cl
        je     SHORT loc_1_3DCA
        mov    cl, byte ptr [eax]
        cmp    cl, 0ffh
        jne    SHORT loc_1_3D5C
        cmp    byte ptr [eax + 1], 0ffh
        je     SHORT loc_1_3D67
loc_1_3D5C:
        cmp    cl, 0c0h
        jne    SHORT loc_1_3D83
        cmp    byte ptr [eax + 1], 0
        jne    SHORT loc_1_3D83
loc_1_3D67:
        cmp    byte ptr [eax + 2], 0ffh
        jne    SHORT loc_1_3D83
        cmp    byte ptr [eax + 3], 0ffh
        jne    SHORT loc_1_3D83
        cmp    byte ptr [eax + 4], 0ffh
        jne    SHORT loc_1_3D83
        cmp    byte ptr [eax + 5], 0ffh
        mov    byte ptr [ebp - 1], 1
        je     SHORT loc_1_3D87
loc_1_3D83:
        mov    byte ptr [ebp - 1], 0
loc_1_3D87:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_3DB0
        test   esi, 8
        je     SHORT loc_1_3DA0
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
loc_1_3DA0:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3DB0:
        test   esi, 6000h
        je     SHORT loc_1_3DC1
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_3DC1:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_3DCA:
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
        db  0FCh, 0F3h, 06Dh, 05Fh, 0C2h, 00Ch, 000h
sub_1_3E5A:
        db  0CDh, 020h, 031h, 080h, 028h, 000h
sub_1_3E60:
        db  0CDh, 020h, 032h, 080h, 028h, 000h
sub_1_3E66:
        db  0CDh, 020h, 033h, 080h, 028h, 000h
sub_1_3E6C:
        db  0CDh, 020h, 034h, 080h, 028h, 000h
sub_1_3E72:
        db  0CDh, 020h, 035h, 080h, 028h, 000h
sub_1_3E78:
        db  0CDh, 020h, 036h, 080h, 028h, 000h
sub_1_3E7E:
        db  0CDh, 020h, 037h, 080h, 028h, 000h
sub_1_3E84:
        db  0CDh, 020h, 038h, 080h, 028h, 000h
sub_1_3E8A:
        db  0CDh, 020h, 039h, 080h, 028h, 000h
        db  0CDh, 020h, 03Ah, 080h, 028h, 000h
sub_1_3E96:
        db  0CDh, 020h, 03Bh, 080h, 028h, 000h
sub_1_3E9C:
        db  0CDh, 020h, 03Ch, 080h, 028h, 000h
sub_1_3EA2:
        db  0CDh, 020h, 03Dh, 080h, 028h, 000h
sub_1_3EA8:
        db  0CDh, 020h, 03Eh, 080h, 028h, 000h
sub_1_3EAE:
        db  0CDh, 020h, 03Fh, 080h, 028h, 000h
sub_1_3EB4:
        db  0CDh, 020h, 040h, 080h, 028h, 000h
sub_1_3EBA:
        db  0CDh, 020h, 041h, 080h, 028h, 000h
sub_1_3EC0:
        db  0CDh, 020h, 009h, 080h, 028h, 000h
        db  0CDh, 020h, 008h, 080h, 028h, 000h
sub_1_3ECC:
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
sub_1_3F22:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
sub_1_3F28:
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
sub_1_3F2E:
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
sub_1_3F34:
        db  0CDh, 020h, 010h, 080h, 028h, 000h
sub_1_3F3A:
        db  0CDh, 020h, 011h, 080h, 028h, 000h
sub_1_3F40:
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_3F46:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_3F4C:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_3F70:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_3F94:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_3FB8:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
        db  0CDh, 020h, 028h, 080h, 028h, 000h
sub_1_3FC4:
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_3FCA:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_3FD0:
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
sub_1_4012:
        db  0CDh, 020h, 046h, 080h, 028h, 000h
sub_1_4018:
        db  0CDh, 020h, 045h, 080h, 028h, 000h
sub_1_401E:
        db  0CDh, 020h, 043h, 080h, 028h, 000h
sub_1_4024:
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        add    byte ptr [eax], al
sub_1_402C:
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
        db  000h, 000h, 000h, 000h, 00Ah, 003h, 000h, 000h, 003h, 00Ah, 000h, 000h, 049h, 042h, 04Dh, 054h
        db  04Fh, 04Bh, 034h, 020h, 000h, 040h, 000h, 054h, 049h, 013h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_40E0:
        dec    edi
        inc    ebx
        dec    ecx
        push   eax
loc_1_40E4:
        xor    byte ptr [ecx], dh
        xor    dword ptr [esi], esi
loc_1_40E8:
        and    byte ptr [eax], dh
        cmp    dword ptr [ecx], edi
loc_1_40EC:
        push   ebx
        push   edx
        inc    ecx
        dec    ebp
loc_1_40F0:
        xor    al, 58h
        xor    esi, dword ptr [esi]
loc_1_40F4:
        and    byte ptr [eax], bh
        xor    dword ptr [546d6249h], esi
        outsd
        imul   esi, dword ptr [eax + eax], 49h
        bound  ebp, qword ptr [ebp + 54h]
        outsd
        imul   esi, dword ptr [eax + eax], 49h
        bound  ebp, qword ptr [ebp + 20h]
        push   esp
        outsd
        imul   esp, dword ptr [ebp + 6eh], 20h
        push   edx
        imul   ebp, dword ptr [esi + 67h], 74654e20h
        db  077h, 06Fh
        jb     SHORT loc_1_4189
        and    byte ptr [ebx + 61h], al
        jb     SHORT loc_1_4187
        and    byte ptr [esi + 6fh], ah
        jb     SHORT loc_1_4148
        push   eax
        inc    ebx
        and    byte ptr [ecx + 2fh], cl
        dec    edi
        and    byte ptr [edx + 75h], ah
        jae    SHORT loc_1_4161
        add    byte ptr [ecx + 62h], cl
        insd
        and    byte ptr [edi + ebp*2 + 6bh], dl
        outsb
        and    byte ptr [edx + 69h], dl
        outsb
        and    byte ptr [bp + 65h], cl
        je     SHORT loc_1_41BE
        outsd
loc_1_4148:
        jb     SHORT loc_1_41B5
        and    byte ptr [ebx + 61h], al
        jb     SHORT loc_1_41B3
        and    byte ptr [esi + 6fh], ah
        jb     SHORT loc_1_4174
        dec    ebp
        inc    ebx
        inc    ecx
        and    byte ptr [edx + 75h], ah
        jae    SHORT loc_1_418A
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    edx
loc_1_4161:
        jne    SHORT loc_1_41D6
        push   esp
        jns    SHORT loc_1_41D6
        add    byte ptr gs:[edx + 75h], al
        jae    SHORT loc_1_41C0
        db  079h, 070h
        add    byte ptr gs:[ecx + 4fh], cl
        inc    edx
        inc    ecx
loc_1_4174:
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
loc_1_4178:
        dec    ecx
        dec    edi
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
        dec    esi
        inc    ebp
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
loc_1_4187:
        inc    ebp
        push   ebx
loc_1_4189:
        push   ebx
loc_1_418A:
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
        inc    ecx
        pop    eax
        push   eax
        inc    ecx
        inc    ebx
        dec    ebx
        inc    ebp
        push   esp
        push   ebx
        dec    ecx
        pop    edx
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [ebp + 41h], cl
        pop    eax
        push   eax
        inc    ecx
        inc    ebx
        dec    ebx
        inc    ebp
        push   esp
        push   ebx
        dec    ecx
loc_1_41B3:
        pop    edx
        inc    ebp
loc_1_41B5:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 41h], al
        push   edx
        dec    esp
        pop    ecx
        push   esp
loc_1_41BE:
        dec    edi
        dec    ebx
loc_1_41C0:
        inc    ebp
        dec    esi
        push   edx
        inc    ebp
        dec    esp
        inc    ebp
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [ebp + 41h], al
        push   edx
        dec    esp
        pop    ecx
        push   esp
        dec    edi
        dec    ebx
        inc    ebp
        dec    esi
loc_1_41D6:
        push   edx
        inc    ebp
        dec    esp
        inc    ebp
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_41E1:
        add    byte ptr [eax], al
loc_1_41E3:
        add    bh, bh
        db  0FFh, 0FFh, 0FFh
_LTEXT ends
        end
