; declan_lcode_01.asm -- DECLAN object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

loc_1_0000:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 4]
        mov    eax, dword ptr [edx]
        pop    ebp
        mov    ax, word ptr [eax + 5eh]
        or     word ptr [ecx], ax
        mov    eax, dword ptr [edx]
        mov    word ptr [eax + 5eh], 0
        mov    al, 1
        ret    4
loc_1_0020:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        sub    bl, bl
        cmp    byte ptr [ecx + 168h], bl
        je     SHORT loc_1_0040
        sub    al, al
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_0040:
        test   byte ptr [ecx], 13h
        je     SHORT loc_1_005A
        cmp    byte ptr [ecx + 4bh], 0
        jne    SHORT loc_1_005A
        mov    bl, 1
        mov    byte ptr [ecx + 4bh], bl
        mov    dx, word ptr [ecx + 41h]
        mov    ax, 7
        out    dx, ax
loc_1_005A:
        mov    dx, word ptr [ecx + 3dh]
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        test   al, 80h
        je     SHORT loc_1_00C1
        and    ax, 7f00h
        mov    al, 40h
        cmp    byte ptr [ecx + 167h], 0
        mov    dx, word ptr [ecx + 3dh]
        out    dx, ax
        je     SHORT loc_1_00B0
        test   byte ptr [ebp - 1], 1
        je     SHORT loc_1_00B0
        mov    byte ptr [ecx + 167h], 0
        test   byte ptr [ecx], 13h
        je     SHORT loc_1_00A1
        or     bl, bl
        je     SHORT loc_1_00A1
        mov    byte ptr [ecx + 4bh], 0
        mov    dx, word ptr [ecx + 41h]
        mov    ax, 2
        out    dx, ax
loc_1_00A1:
        sub    al, al
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_00B0:
        mov    ax, word ptr [ebp - 2]
        or     word ptr [ecx + 5eh], ax
        mov    al, 1
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_00C1:
        test   byte ptr [ecx], 13h
        je     SHORT loc_1_00D8
        or     bl, bl
        je     SHORT loc_1_00D8
        mov    byte ptr [ecx + 4bh], 0
        mov    dx, word ptr [ecx + 41h]
        mov    ax, 2
        out    dx, ax
loc_1_00D8:
        sub    al, al
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_00F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 168h], 0
        jne    SHORT loc_1_0167
        cmp    byte ptr [esi + 0ach], 0
        jne    SHORT loc_1_0167
        test   byte ptr [esi], 13h
        je     SHORT loc_1_0125
        cmp    byte ptr [esi + 4bh], 0
        jne    SHORT loc_1_0125
        mov    byte ptr [esi + 4bh], 1
        mov    dx, word ptr [esi + 41h]
        mov    ax, 7
        out    dx, ax
loc_1_0125:
        mov    byte ptr [esi + 0ach], 1
        push   esi
        call   sub_1_0170
        mov    al, byte ptr [esi + 4bh]
        and    eax, 0ffff00ffh
        test   byte ptr [esi], 13h
        je     SHORT loc_1_0167
        or     ax, ax
        je     SHORT loc_1_0167
        mov    byte ptr [esi + 4bh], 0
        mov    dword ptr [ebp - 0ch], esi
        mov    ax, word ptr [esi + 49h]
        or     ax, 2
        mov    word ptr [ebp - 4], ax
        lea    eax, [ebp - 0ch]
        push   eax
        db  068h, 0E0h, 00Ah, 000h, 000h
        inc    esi
        push   esi
        call   sub_1_19EA
loc_1_0167:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Bh, 0FFh
sub_1_0170:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 2], 0
        mov    esi, dword ptr [ebp + 8]
loc_1_0182:
        mov    byte ptr [esi + 17ah], 0
        mov    dx, word ptr [esi + 3dh]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        test   al, 80h
        je     SHORT loc_1_01AB
        and    ax, 7f00h
        mov    al, 40h
        mov    dx, word ptr [esi + 3dh]
        out    dx, ax
        mov    ax, word ptr [ebp - 4]
        or     word ptr [ebp - 2], ax
loc_1_01AB:
        mov    dword ptr [ebp - 10h], esi
        lea    eax, [ebp - 2]
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 10h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 1]
        push   eax
        call   sub_1_19EA
        cmp    byte ptr [esi + 149h], 0
        jne    SHORT loc_1_020B
        test   byte ptr [ebp - 1], 5eh
        jne    SHORT loc_1_0215
        cmp    dword ptr [esi + 0d5h], 0
        jne    SHORT loc_1_0215
        cmp    byte ptr [esi + 148h], 0
        je     SHORT loc_1_01F0
        cmp    dword ptr [esi + 80h], 0
        je     SHORT loc_1_0215
loc_1_01F0:
        cmp    byte ptr [esi + 0fah], 0
        jne    SHORT loc_1_020B
        cmp    byte ptr [esi + 0f9h], 0
        je     SHORT loc_1_020B
        cmp    dword ptr [esi + 0fch], 0
        jne    SHORT loc_1_0215
loc_1_020B:
        test   byte ptr [ebp - 1], 1
        je     NEAR PTR loc_1_0490
loc_1_0215:
        mov    eax, dword ptr [esi + 80h]
        inc    eax
        mov    dword ptr [esi + 80h], eax
        test   byte ptr [ebp - 1], 1
        jne    NEAR PTR loc_1_0332
        cmp    byte ptr [esi + 148h], 0
        je     SHORT loc_1_0255
        cmp    byte ptr [esi + 149h], 0
        jne    SHORT loc_1_0255
        cmp    eax, 1
        jne    SHORT loc_1_0255
        push   esi
        db  0E8h, 007h, 00Fh, 0FFh, 0FFh
        mov    byte ptr [esi + 149h], 1
        jmp    NEAR PTR loc_1_0418
loc_1_0255:
        test   byte ptr [ebp - 1], 58h
        je     SHORT loc_1_02A9
        test   byte ptr [ebp - 1], 10h
        je     SHORT loc_1_0269
        inc    dword ptr [esi + 13ch]
        jmp    SHORT loc_1_02A3
loc_1_0269:
        test   byte ptr [ebp - 1], 40h
        je     SHORT loc_1_0291
        inc    dword ptr [esi + 138h]
        push   1
        push   1
        push   2
        push   0aa000006h
        mov    eax, dword ptr [esi + 9ch]
        push   eax
        call   sub_1_1AC8
        add    esp, 14h
        jmp    SHORT loc_1_02A3
loc_1_0291:
        inc    dword ptr [esi + 134h]
        push   0bh
        push   0
        push   0
        push   esi
        db  0E8h, 0BDh, 011h, 0FFh, 0FFh
loc_1_02A3:
        and    word ptr [ebp - 2], 7ffh
loc_1_02A9:
        test   byte ptr [ebp - 1], 4
        je     SHORT loc_1_02BF
        push   esi
        call   sub_1_04C0
        or     al, al
        je     SHORT loc_1_02BF
        and    word ptr [ebp - 2], 0fbffh
loc_1_02BF:
        test   byte ptr [ebp - 1], 2
        je     SHORT loc_1_02F9
        test   byte ptr [ebp - 2], 10h
        jne    SHORT loc_1_02E9
        and    word ptr [ebp - 2], 0fdffh
        cmp    byte ptr [esi + 148h], 0
        jne    NEAR PTR loc_1_0418
        push   0bh
        push   0
        push   0
        jmp    NEAR PTR loc_1_0412
loc_1_02E9:
        push   esi
        call   sub_1_0790
        or     al, al
        jne    SHORT loc_1_02F9
        and    word ptr [ebp - 2], 0fdffh
loc_1_02F9:
        cmp    byte ptr [esi + 0fah], 0
        jne    SHORT loc_1_031A
        cmp    byte ptr [esi + 0f9h], 0
        je     SHORT loc_1_031A
        cmp    dword ptr [esi + 0fch], 0
        je     SHORT loc_1_031A
        push   esi
        call   sub_1_0E30
loc_1_031A:
        cmp    dword ptr [esi + 0d5h], 0
        je     NEAR PTR loc_1_0418
        push   esi
        call   sub_1_11B0
        jmp    NEAR PTR loc_1_0418
loc_1_0332:
        and    word ptr [ebp - 2], 0feffh
        mov    byte ptr [esi + 148h], 0
        mov    byte ptr [esi + 149h], 0
        mov    edi, dword ptr [esi + 14ah]
        or     edi, edi
        mov    eax, dword ptr [esi + 152h]
        mov    ecx, dword ptr [esi + 14eh]
        je     SHORT loc_1_03CB
        cmp    eax, 4
        je     SHORT loc_1_03C4
        or     ecx, ecx
        je     SHORT loc_1_0370
        push   1
        push   esi
        db  0E8h, 053h, 011h, 0FFh, 0FFh
        jmp    SHORT loc_1_03D4
        db  090h
loc_1_0370:
        cmp    eax, 8
        jne    SHORT loc_1_03D4
        mov    ebx, dword ptr [esi + 84h]
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, 0ffffff7ch
        je     SHORT loc_1_03B4
loc_1_0386:
        inc    dword ptr [ebx + 18h]
        push   0
        push   0
        push   40010005h
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_195E
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_1964
        dec    dword ptr [ebx + 18h]
        mov    ebx, dword ptr [ebx]
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, 0ffffff7ch
        jne    SHORT loc_1_0386
loc_1_03B4:
        push   0
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_1946
        dec    dword ptr [edi + 18h]
        jmp    SHORT loc_1_03D4
loc_1_03C4:
        or     edi, edi
        je     SHORT loc_1_03CB
        dec    dword ptr [edi + 18h]
loc_1_03CB:
        cmp    byte ptr [esi + 148h], 0
        jne    SHORT loc_1_03DA
loc_1_03D4:
        push   esi
        db  0E8h, 016h, 0FDh, 0FEh, 0FFh
loc_1_03DA:
        mov    ecx, dword ptr [esi + 156h]
        or     ecx, ecx
        je     SHORT loc_1_0418
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 156h], eax
        cmp    dword ptr [esi + 15ah], ecx
        jne    SHORT loc_1_03FE
        mov    dword ptr [esi + 15ah], 0
loc_1_03FE:
        mov    eax, dword ptr [ecx + 8]
        cmp    eax, 4
        jne    SHORT loc_1_040C
        push   4
        push   0
        jmp    SHORT loc_1_040E
loc_1_040C:
        push   eax
        push   ecx
loc_1_040E:
        mov    eax, dword ptr [ecx + 4]
        push   eax
loc_1_0412:
        push   esi
        db  0E8h, 048h, 010h, 0FFh, 0FFh
loc_1_0418:
        mov    ecx, dword ptr [esi + 8ch]
        lea    edi, [esi + 8ch]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, 8ch
        je     SHORT loc_1_0484
        mov    eax, esi
        sub    eax, ecx
        cmp    eax, 0ffffff74h
        je     SHORT loc_1_0484
loc_1_043A:
        mov    ebx, dword ptr [edi]
        cmp    dword ptr [ebx + 18h], 0
        jne    SHORT loc_1_0477
        push   0
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_1934
        mov    ecx, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebx + 14h]
        cmp    dword ptr [esi + 162h], eax
        jne    SHORT loc_1_046D
        push   esi
        db  0E8h, 083h, 0FDh, 0FEh, 0FFh
loc_1_046D:
        push   0
        push   39h
        push   ebx
        call   sub_1_19FC
loc_1_0477:
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, dword ptr [edi]
        cmp    eax, 0ffffff74h
        jne    SHORT loc_1_043A
loc_1_0484:
        dec    dword ptr [esi + 80h]
        jmp    NEAR PTR loc_1_0182
        db  090h
loc_1_0490:
        mov    byte ptr [esi + 0ach], 0
        cmp    byte ptr [esi + 0fbh], 0
        je     SHORT loc_1_04B3
        mov    byte ptr [esi + 0fbh], 0
        mov    eax, dword ptr [esi + 0adh]
        push   eax
        call   sub_1_1780
loc_1_04B3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_04C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 108h]
        lea    edi, [ecx*8]
        mov    dword ptr [ebp - 0ch], ecx
        add    edi, dword ptr [esi + 0d1h]
        mov    eax, dword ptr [esi + 64h]
        dec    eax
        mov    dword ptr [ebp - 14h], eax
        sub    al, al
        mov    ebx, 1
        mov    dword ptr [ebp - 10h], 0
loc_1_04F7:
        mov    cl, byte ptr [edi + 3]
        mov    byte ptr [ebp - 1], cl
        test   cl, 80h
        jne    NEAR PTR loc_1_0700
        cmp    dword ptr [ebp - 10h], 0ah
        ja     NEAR PTR loc_1_070B
        test   cl, 40h
        je     SHORT loc_1_0560
        test   cl, 4
        je     SHORT loc_1_0525
        inc    dword ptr [esi + 10ch]
        jmp    NEAR PTR loc_1_0624
loc_1_0525:
        test   byte ptr [ebp - 1], 8
        je     SHORT loc_1_0536
        inc    dword ptr [esi + 110h]
        jmp    NEAR PTR loc_1_0624
loc_1_0536:
        test   byte ptr [ebp - 1], 10h
        je     SHORT loc_1_0547
        inc    dword ptr [esi + 10ch]
        jmp    NEAR PTR loc_1_0624
loc_1_0547:
        test   byte ptr [ebp - 1], 20h
        je     NEAR PTR loc_1_0624
        inc    dword ptr [esi + 114h]
        jmp    NEAR PTR loc_1_0624
        db  08Dh, 064h, 024h, 000h
loc_1_0560:
        test   byte ptr [ebp - 1], 1
        je     NEAR PTR loc_1_0637
        cmp    byte ptr [esi + 148h], 0
        jne    NEAR PTR loc_1_0716
        or     dword ptr [ebp - 8], 1
        mov    eax, dword ptr [esi + 108h]
        add    eax, eax
        xor    eax, dword ptr [ebp - 8]
        and    eax, 0feh
        xor    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0ch]
        shl    eax, 8
        xor    eax, dword ptr [ebp - 8]
        and    eax, 7f00h
        xor    dword ptr [ebp - 8], eax
        mov    cx, word ptr [edi + 6]
        and    ecx, 0fffh
        sub    ecx, 4
        mov    eax, dword ptr [esi + 68h]
        cmp    eax, ecx
        jb     SHORT loc_1_05B5
        mov    eax, ecx
loc_1_05B5:
        sub    eax, 0eh
        mov    dword ptr [ebp - 18h], eax
        inc    dword ptr [esi + 144h]
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 15eh], eax
        cmp    ecx, 0eh
        mov    byte ptr [esi + 0fbh], 1
        jae    SHORT loc_1_05F5
        cmp    ecx, 6
        jb     SHORT loc_1_0624
        mov    edx, dword ptr [esi + 104h]
        mov    eax, dword ptr [esi + 108h]
        mov    eax, dword ptr [edx + eax*4]
        push   0
        push   0
        push   0
        push   ecx
        push   eax
        push   eax
        jmp    SHORT loc_1_0614
loc_1_05F5:
        mov    eax, dword ptr [esi + 104h]
        mov    edx, dword ptr [esi + 108h]
        mov    edx, dword ptr [eax + edx*4]
        sub    ecx, 0eh
        push   ecx
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        lea    eax, [edx + 0eh]
        push   eax
        push   0eh
        push   edx
        push   edx
loc_1_0614:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [esi + 0adh]
        push   eax
        call   sub_1_15A0
loc_1_0624:
        inc    dword ptr [ebp - 10h]
        push   ebx
        mov    eax, dword ptr [esi + 108h]
        push   eax
        push   esi
        call   sub_1_0730
        mov    al, 1
loc_1_0637:
        mov    ecx, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 14h], ecx
        jne    SHORT loc_1_0650
        mov    dword ptr [ebp - 0ch], 0
        mov    edi, dword ptr [esi + 0d1h]
        jmp    SHORT loc_1_0656
        db  08Bh, 0FFh
loc_1_0650:
        inc    dword ptr [ebp - 0ch]
        add    edi, 8
loc_1_0656:
        or     al, al
        je     SHORT loc_1_0667
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 108h], ecx
        sub    al, al
        sub    ebx, ebx
loc_1_0667:
        inc    ebx
        mov    ecx, dword ptr [ebp - 14h]
        inc    ecx
        cmp    ecx, ebx
        jae    NEAR PTR loc_1_04F7
        mov    byte ptr [esi + 169h], 1
        mov    edi, dword ptr [esi + 84h]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0ffffff7ch
        je     SHORT loc_1_06BE
        lea    esp, [esp]
loc_1_0690:
        inc    dword ptr [edi + 18h]
        push   0
        push   0
        push   0c0010002h
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_195E
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_1964
        dec    dword ptr [edi + 18h]
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0ffffff7ch
        jne    SHORT loc_1_0690
loc_1_06BE:
        push   2625a0h
        call   sub_1_19F0
        lea    eax, [esi + 1]
        push   eax
        call   sub_1_19E4
        push   0
        push   0aa000003h
        mov    eax, dword ptr [esi + 9ch]
        push   eax
        call   sub_1_1AC8
        add    esp, 0ch
        mov    eax, dword ptr [esi + 9ch]
        push   eax
        call   sub_1_1928
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_0700:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_070B:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0716:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0730:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        lea    edx, [ecx*8]
        add    edx, dword ptr [eax + 0d1h]
        mov    esi, dword ptr [eax + 64h]
        dec    esi
        mov    bl, byte ptr [edx + 3]
        mov    edi, dword ptr [ebp + 10h]
        or     edi, edi
        je     SHORT loc_1_0778
        mov    ebx, 80h
        nop
loc_1_0760:
        mov    byte ptr [edx + 3], bl
        cmp    esi, ecx
        jne    SHORT loc_1_0771
        mov    edx, dword ptr [eax + 0d1h]
        sub    ecx, ecx
        jmp    SHORT loc_1_0775
loc_1_0771:
        add    edx, 8
        inc    ecx
loc_1_0775:
        dec    edi
        jne    SHORT loc_1_0760
loc_1_0778:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0790:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebx + 0bdh]
        cmp    dword ptr [ebx + 0c1h], ecx
        jne    SHORT loc_1_07C0
        cmp    dword ptr [ebx + 0b5h], 0
        jbe    SHORT loc_1_07C0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_07C0:
        sub    ecx, dword ptr [ebx + 0c9h]
        sar    ecx, 3
        mov    edx, dword ptr [ebx + 0c5h]
        lea    eax, [ecx + ecx*2]
        lea    edx, [edx + eax*4]
        lea    esi, [ebp - 24h]
        mov    eax, dword ptr [edx]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [ebp - 20h]
        shl    eax, 3
        add    eax, dword ptr [ebx + 0c9h]
        mov    dword ptr [ebp - 8], eax
        mov    esi, dword ptr [ebp - 24h]
        mov    dword ptr [ebp - 24h], 0
        lea    eax, [esi + 24h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 3]
        mov    byte ptr [ebp - 1], al
        test   al, 80h
        je     SHORT loc_1_0820
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_0820:
        mov    byte ptr [ebp - 2], 1
        lea    edi, [ecx*8]
        add    edi, dword ptr [ebx + 0c9h]
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        push   ebx
        call   sub_1_0A30
        mov    ecx, 100h
        mov    edx, 40h
loc_1_0845:
        mov    al, byte ptr [edi + 3]
        mov    byte ptr [ebp - 1], al
        test   byte ptr [ebp - 1], dl
        mov    ax, word ptr [edi + 6]
        mov    word ptr [ebp - 4], ax
        je     SHORT loc_1_085D
        test   cx, ax
        je     SHORT loc_1_08C8
loc_1_085D:
        mov    al, byte ptr [ebp - 1]
        and    al, 1
        test   word ptr [ebp - 4], cx
        je     SHORT loc_1_086E
        inc    dword ptr [ebx + 120h]
loc_1_086E:
        test   byte ptr [ebp - 1], 2
        je     SHORT loc_1_089C
        test   byte ptr [ebp - 1], 4
        je     SHORT loc_1_0882
        inc    dword ptr [ebx + 128h]
        jmp    SHORT loc_1_089C
loc_1_0882:
        test   byte ptr [ebp - 1], 8
        je     SHORT loc_1_0890
        inc    dword ptr [ebx + 12ch]
        jmp    SHORT loc_1_089C
loc_1_0890:
        test   byte ptr [ebp - 1], 10h
        je     SHORT loc_1_089C
        inc    dword ptr [ebx + 130h]
loc_1_089C:
        cmp    dword ptr [ebx + 0cdh], edi
        jne    SHORT loc_1_08B0
        mov    edi, dword ptr [ebx + 0c9h]
        jmp    SHORT loc_1_08B3
        db  08Dh, 064h, 024h, 000h
loc_1_08B0:
        add    edi, 8
loc_1_08B3:
        mov    dword ptr [ebx + 0bdh], edi
        inc    dword ptr [ebx + 0b5h]
        or     al, al
        je     SHORT loc_1_0845
        jmp    NEAR PTR loc_1_094E
loc_1_08C8:
        test   byte ptr [ebp - 3], 4
        je     SHORT loc_1_08D6
        inc    dword ptr [ebx + 118h]
        jmp    SHORT loc_1_08F0
loc_1_08D6:
        test   byte ptr [ebp - 3], 8
        je     SHORT loc_1_08E4
        inc    dword ptr [ebx + 11ch]
        jmp    SHORT loc_1_08F0
loc_1_08E4:
        test   byte ptr [ebp - 3], 40h
        je     SHORT loc_1_08F0
        inc    dword ptr [ebx + 124h]
loc_1_08F0:
        mov    byte ptr [ebp - 2], 0
        cmp    dword ptr [ebp - 8], edi
        jae    SHORT loc_1_0920
        mov    ecx, dword ptr [ebx + 0cdh]
        sub    ecx, edi
        sar    ecx, 3
        mov    eax, dword ptr [ebp - 8]
        sub    eax, dword ptr [ebx + 0c9h]
        sar    eax, 3
        add    ecx, eax
        add    ecx, 2
        add    dword ptr [ebx + 0b5h], ecx
        jmp    SHORT loc_1_092F
        db  08Dh, 049h, 000h
loc_1_0920:
        mov    eax, dword ptr [ebp - 8]
        sub    eax, edi
        sar    eax, 3
        inc    eax
        add    dword ptr [ebx + 0b5h], eax
loc_1_092F:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebx + 0cdh], eax
        jne    SHORT loc_1_0942
        mov    eax, dword ptr [ebx + 0c9h]
        jmp    SHORT loc_1_0948
loc_1_0942:
        mov    eax, dword ptr [ebp - 8]
        add    eax, 8
loc_1_0948:
        mov    dword ptr [ebx + 0bdh], eax
loc_1_094E:
        cmp    byte ptr [ebp - 2], 0
        je     SHORT loc_1_095A
        inc    dword ptr [ebx + 140h]
loc_1_095A:
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebx + 0ddh], eax
        or     eax, eax
        jne    SHORT loc_1_0973
        mov    dword ptr [ebx + 0e1h], 0
loc_1_0973:
        push   0
        lea    eax, [ebp - 14h]
        push   eax
        push   0
        push   0
        push   esi
        call   sub_1_1A20
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_1A44
        mov    eax, dword ptr [ebp - 0ch]
        mov    edi, dword ptr [eax + 4]
        test   byte ptr [edi + 35h], 2
        jne    SHORT loc_1_09EA
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebx + 0adh]
        push   eax
        call   sub_1_1820
        or     al, al
        je     SHORT loc_1_09EA
        mov    eax, dword ptr [ebp - 0ch]
        mov    cl, byte ptr [ebp - 2]
        mov    byte ptr [eax + 0ah], cl
        cmp    dword ptr [ebx + 0d5h], 0
        jne    SHORT loc_1_09D0
        mov    dword ptr [ebx + 0d5h], esi
        jmp    SHORT loc_1_09D9
        db  08Bh, 0FFh
loc_1_09D0:
        mov    eax, dword ptr [ebx + 0d9h]
        mov    dword ptr [eax + 24h], esi
loc_1_09D9:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax], 0
        mov    dword ptr [ebx + 0d9h], esi
        jmp    SHORT loc_1_0A03
loc_1_09EA:
        cmp    byte ptr [ebp - 2], 1
        sbb    eax, eax
        and    eax, 0c0000001h
        push   eax
        push   esi
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_193A
        dec    dword ptr [edi + 18h]
loc_1_0A03:
        cmp    byte ptr [ebx + 0f9h], 0
        jne    SHORT loc_1_0A1C
        cmp    byte ptr [ebx + 148h], 0
        jne    SHORT loc_1_0A1C
        mov    byte ptr [ebx + 0f9h], 1
loc_1_0A1C:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0A30:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, esi
        mov    edi, dword ptr [edx + 0f5h]
        shl    ecx, 4
        add    edi, ecx
        mov    eax, dword ptr [edi + 4]
        lea    eax, [edx + eax*4 + 0e5h]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edi + 4], ecx
        mov    dword ptr [eax], esi
        cmp    byte ptr [edx + 0f9h], 0
        jne    SHORT loc_1_0A72
        cmp    byte ptr [edx + 148h], 0
        jne    SHORT loc_1_0A72
        mov    byte ptr [edx + 0f9h], 1
loc_1_0A72:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0A80:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [ecx]
        mov    dx, word ptr [eax + 39h]
        mov    ax, word ptr [ecx + 8]
        out    dx, ax
        sub    al, al
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0AA0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [ecx]
        mov    dx, word ptr [eax + 3dh]
        mov    ax, word ptr [ecx + 8]
        out    dx, ax
        sub    al, al
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [ecx]
        mov    ecx, dword ptr [ecx + 4]
        mov    dx, word ptr [eax + 3dh]
        in     ax, dx
        mov    word ptr [ecx], ax
        sub    al, al
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0AE0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [ecx]
        mov    dx, word ptr [eax + 41h]
        mov    ax, word ptr [ecx + 8]
        out    dx, ax
        sub    al, al
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0B00:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 17ah], 0
        je     NEAR PTR loc_1_0C56
        cmp    dword ptr [esi + 0ddh], 0
        jne    SHORT loc_1_0B3E
        cmp    dword ptr [esi + 0fch], 0
        jne    SHORT loc_1_0B3E
        cmp    dword ptr [esi + 156h], 0
        jne    SHORT loc_1_0B3E
        cmp    dword ptr [esi + 14eh], 0
        je     NEAR PTR loc_1_0C56
loc_1_0B3E:
        mov    byte ptr [esi + 17ah], 0
        push   0
        push   0aa000003h
        mov    eax, dword ptr [esi + 9ch]
        push   eax
        call   sub_1_1AC8
        add    esp, 0ch
        mov    edi, dword ptr [esi + 14ah]
        or     edi, edi
        je     SHORT loc_1_0B87
        cmp    dword ptr [esi + 152h], 4
        je     SHORT loc_1_0B87
        mov    eax, dword ptr [esi + 14eh]
        or     eax, eax
        je     SHORT loc_1_0B87
        push   0
        push   eax
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_194C
        dec    dword ptr [edi + 18h]
loc_1_0B87:
        cmp    dword ptr [esi + 156h], 0
        je     SHORT loc_1_0BCD
        sub    edi, edi
loc_1_0B92:
        mov    ecx, dword ptr [esi + 156h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 156h], eax
        cmp    eax, edi
        jne    SHORT loc_1_0BAA
        mov    dword ptr [esi + 15ah], edi
loc_1_0BAA:
        mov    ebx, dword ptr [ecx + 4]
        cmp    ebx, edi
        je     SHORT loc_1_0BC5
        cmp    dword ptr [ecx + 8], 4
        je     SHORT loc_1_0BC5
        push   edi
        push   ecx
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_194C
        dec    dword ptr [ebx + 18h]
loc_1_0BC5:
        cmp    dword ptr [esi + 156h], edi
        jne    SHORT loc_1_0B92
loc_1_0BCD:
        cmp    dword ptr [esi + 0ddh], 0
        je     SHORT loc_1_0C0D
        sub    edi, edi
loc_1_0BD8:
        mov    edx, dword ptr [esi + 0ddh]
        mov    eax, dword ptr [edx + 24h]
        lea    ecx, [edx + 24h]
        mov    dword ptr [esi + 0ddh], eax
        cmp    eax, edi
        jne    SHORT loc_1_0BF4
        mov    dword ptr [esi + 0e1h], edi
loc_1_0BF4:
        mov    ebx, dword ptr [ecx + 4]
        push   edi
        push   edx
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_193A
        dec    dword ptr [ebx + 18h]
        cmp    dword ptr [esi + 0ddh], edi
        jne    SHORT loc_1_0BD8
loc_1_0C0D:
        cmp    dword ptr [esi + 0fch], 0
        je     SHORT loc_1_0C4D
        sub    edi, edi
loc_1_0C18:
        mov    edx, dword ptr [esi + 0fch]
        mov    eax, dword ptr [edx + 24h]
        lea    ecx, [edx + 24h]
        mov    dword ptr [esi + 0fch], eax
        cmp    eax, edi
        jne    SHORT loc_1_0C34
        mov    dword ptr [esi + 100h], edi
loc_1_0C34:
        mov    ebx, dword ptr [ecx + 4]
        push   edi
        push   edx
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_193A
        dec    dword ptr [ebx + 18h]
        cmp    dword ptr [esi + 0fch], edi
        jne    SHORT loc_1_0C18
loc_1_0C4D:
        mov    byte ptr [esi + 17ah], 0
        jmp    SHORT loc_1_0C81
loc_1_0C56:
        cmp    dword ptr [esi + 0ddh], 0
        jne    SHORT loc_1_0C7A
        cmp    dword ptr [esi + 0fch], 0
        jne    SHORT loc_1_0C7A
        cmp    dword ptr [esi + 156h], 0
        jne    SHORT loc_1_0C7A
        cmp    dword ptr [esi + 14eh], 0
        je     SHORT loc_1_0C81
loc_1_0C7A:
        mov    byte ptr [esi + 17ah], 1
loc_1_0C81:
        push   1388h
        add    esi, 16eh
        push   esi
        call   sub_1_19D2
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0CA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 103h
        mov    esi, dword ptr [edi + 8]
        cmp    byte ptr [esi + 169h], 0
        je     SHORT loc_1_0CD0
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_1_0CD0:
        inc    dword ptr [esi + 80h]
        cmp    byte ptr [esi + 148h], 0
        jne    NEAR PTR loc_1_0D60
        cmp    byte ptr [edi + 1ch], 0
        jne    SHORT loc_1_0D53
        inc    dword ptr [edi + 18h]
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        push   0
        mov    ebx, dword ptr [ebp + 0ch]
        push   ebx
        call   sub_1_1A20
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_0D47
        cmp    dword ptr [ebp - 8], 5eah
        ja     SHORT loc_1_0D47
        push   ebx
        push   edi
        push   esi
        call   sub_1_0DE0
        inc    dword ptr [edi + 18h]
        cmp    byte ptr [esi + 0fah], 0
        jne    SHORT loc_1_0D4E
        sub    ebx, ebx
loc_1_0D24:
        cmp    dword ptr [esi + 0fch], ebx
        je     SHORT loc_1_0D4E
        cmp    byte ptr [esi + 0f9h], bl
        je     SHORT loc_1_0D4E
        push   esi
        call   sub_1_0E30
        cmp    byte ptr [esi + 0fah], bl
        je     SHORT loc_1_0D24
        dec    dword ptr [edi + 18h]
        jmp    SHORT loc_1_0D87
loc_1_0D47:
        mov    dword ptr [ebp - 4], 0c000009ah
loc_1_0D4E:
        dec    dword ptr [edi + 18h]
        jmp    SHORT loc_1_0D87
loc_1_0D53:
        mov    dword ptr [ebp - 4], 0c0010002h
        jmp    SHORT loc_1_0D87
        db  08Dh, 064h, 024h, 000h
loc_1_0D60:
        cmp    dword ptr [esi + 152h], 8
        jne    SHORT loc_1_0D72
        mov    dword ptr [ebp - 4], 0c001000dh
        jmp    SHORT loc_1_0D87
loc_1_0D72:
        mov    ebx, dword ptr [ebp + 0ch]
        push   ebx
        push   edi
        push   esi
        call   sub_1_0DE0
        inc    dword ptr [edi + 18h]
        mov    dword ptr [ebp - 4], 103h
loc_1_0D87:
        mov    eax, dword ptr [esi + 80h]
        dec    eax
        mov    dword ptr [esi + 80h], eax
        jne    SHORT loc_1_0DC5
        cmp    byte ptr [esi + 148h], 0
        jne    SHORT loc_1_0DB7
        cmp    dword ptr [esi + 0d5h], 0
        jne    SHORT loc_1_0DB7
        mov    eax, dword ptr [esi + 8ch]
        sub    eax, esi
        cmp    eax, 8ch
        je     SHORT loc_1_0DC5
loc_1_0DB7:
        push   0
        add    esi, 0a0h
        push   esi
        call   sub_1_19D2
loc_1_0DC5:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0DE0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [edx + 2ch], 0
        mov    dword ptr [edx + 28h], eax
        lea    esi, [edx + 24h]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 100h]
        or     ecx, ecx
        jne    SHORT loc_1_0E10
        mov    dword ptr [eax + 0fch], edx
        jmp    SHORT loc_1_0E13
        db  005h, 000h, 000h, 000h, 000h
loc_1_0E10:
        mov    dword ptr [ecx + 24h], edx
loc_1_0E13:
        mov    dword ptr [eax + 100h], edx
        mov    dword ptr [esi], 0
        pop    esi
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0E30:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0b5h], 0
        jne    SHORT loc_1_0E55
        mov    byte ptr [esi + 0f9h], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0E55:
        mov    byte ptr [esi + 0fah], 1
        mov    edi, dword ptr [esi + 0fch]
        lea    eax, [ebp - 10h]
        push   eax
        push   0
        push   0
        push   0
        push   edi
        call   sub_1_1A20
        cmp    dword ptr [ebp - 10h], 40h
        ja     SHORT loc_1_0E80
        mov    al, 1
        jmp    SHORT loc_1_0E8D
        db  08Dh, 064h, 024h, 000h
loc_1_0E80:
        cmp    dword ptr [ebp - 10h], 100h
        mov    al, 2
        jbe    SHORT loc_1_0E8D
        mov    al, 3
loc_1_0E8D:
        xor    ecx, ecx
        mov    cl, al
        cmp    ecx, 3
        jg     SHORT loc_1_0ED0
        lea    eax, [esi + ecx*4 + 0e5h]
        lea    ecx, [ecx]
loc_1_0EA0:
        mov    edx, dword ptr [eax]
        cmp    edx, -1
        jne    SHORT loc_1_0EB2
        add    eax, 4
        inc    ecx
        cmp    ecx, 3
        jle    SHORT loc_1_0EA0
        jmp    SHORT loc_1_0ED3
loc_1_0EB2:
        mov    eax, dword ptr [esi + 0f5h]
        mov    ebx, edx
        shl    ebx, 4
        add    eax, ebx
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esi + ecx*4 + 0e5h], eax
        jmp    SHORT loc_1_0ED3
        db  08Bh, 0FFh
loc_1_0ED0:
        mov    edx, dword ptr [ebp - 14h]
loc_1_0ED3:
        cmp    edx, -1
        jne    SHORT loc_1_0EF0
        mov    byte ptr [esi + 0f9h], 0
        mov    byte ptr [esi + 0fah], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_0EF0:
        lea    eax, [edi + 24h]
        mov    dword ptr [ebp - 0ch], eax
        mov    word ptr [eax + 8], dx
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 0fch], eax
        or     eax, eax
        jne    SHORT loc_1_0F10
        mov    dword ptr [esi + 100h], 0
loc_1_0F10:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 4], ecx
        mov    ebx, dword ptr [esi + 0b9h]
        mov    ecx, dword ptr [esi + 0c9h]
        mov    eax, ebx
        sub    eax, ecx
        sar    eax, 3
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [esi + 60h]
        dec    eax
        mov    edx, dword ptr [ebp - 4]
        inc    edx
        and    eax, edx
        lea    eax, [ecx + eax*8]
        mov    dword ptr [esi + 0b9h], eax
        dec    dword ptr [esi + 0b5h]
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        push   esi
        call   sub_1_1030
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0c5h]
        lea    eax, [eax + eax*2]
        lea    ecx, [ecx + eax*4]
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx], edi
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ebx], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebx + 4], 0
        mov    eax, dword ptr [eax + 0ch]
        not    ax
        inc    ax
        mov    word ptr [ebx + 4], ax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        sub    eax, dword ptr [esi + 21h]
        add    eax, dword ptr [esi + 29h]
        mov    word ptr [ebx], ax
        shr    eax, 10h
        mov    byte ptr [ebx + 2], al
        or     byte ptr [ebx + 3], 83h
        mov    eax, dword ptr [esi + 60h]
        sub    eax, dword ptr [ebp - 4]
        cmp    eax, 1
        jne    SHORT loc_1_0FC0
        mov    eax, dword ptr [esi + 0c9h]
        jmp    SHORT loc_1_0FCF
        db  08Dh, 049h, 000h
loc_1_0FC0:
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 3
        add    eax, dword ptr [esi + 0c9h]
        add    eax, 8
loc_1_0FCF:
        mov    dword ptr [esi + 0c1h], eax
        mov    dword ptr [ebp - 20h], esi
        mov    word ptr [ebp - 18h], 48h
        lea    eax, [ebp - 20h]
        push   eax
        db  068h, 0A0h, 00Ah, 000h, 000h
        lea    eax, [esi + 1]
        push   eax
        call   sub_1_19EA
        mov    eax, dword ptr [esi + 0e1h]
        or     eax, eax
        je     SHORT loc_1_0FFD
        mov    dword ptr [eax + 24h], edi
loc_1_0FFD:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax], 0
        mov    dword ptr [esi + 0e1h], edi
        cmp    dword ptr [esi + 0ddh], 0
        jne    SHORT loc_1_101B
        mov    dword ptr [esi + 0ddh], edi
loc_1_101B:
        mov    byte ptr [esi + 0fah], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_1030:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        lea    eax, [ebp - 10h]
        mov    edi, dword ptr [esi]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_1A20
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_108F
        sub    ebx, ebx
loc_1_105B:
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1A44
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        push   edi
        call   sub_1_1ADC
        add    edi, dword ptr [ebp - 8]
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1A68
        cmp    dword ptr [ebp - 4], ebx
        jne    SHORT loc_1_105B
loc_1_108F:
        cmp    dword ptr [ebp - 10h], 3ch
        jae    SHORT loc_1_10C0
        mov    edx, 3ch
        sub    edx, dword ptr [ebp - 10h]
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        mov    dword ptr [esi + 0ch], 3ch
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h
loc_1_10C0:
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [esi + 0ch], eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  090h
sub_1_10D0:
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
        je     NEAR PTR loc_1_119A
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1A20
        cmp    dword ptr [ebp - 10h], esi
        je     NEAR PTR loc_1_119A
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1A44
        cmp    dword ptr [ebp + 10h], esi
        je     SHORT loc_1_1195
        mov    edi, dword ptr [ebp + 0ch]
        mov    edi, edi
loc_1_1120:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_114C
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1A68
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_1195
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1A44
        jmp    SHORT loc_1_1190
loc_1_114C:
        or     edi, edi
        je     SHORT loc_1_1169
        cmp    edi, dword ptr [ebp - 4]
        jbe    SHORT loc_1_1161
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_1190
loc_1_1161:
        add    dword ptr [ebp - 0ch], edi
        sub    dword ptr [ebp - 4], edi
        sub    edi, edi
loc_1_1169:
        mov    ebx, dword ptr [ebp + 10h]
        sub    ebx, esi
        mov    eax, dword ptr [ebp - 4]
        cmp    ebx, eax
        jb     SHORT loc_1_1177
        mov    ebx, eax
loc_1_1177:
        push   ebx
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        add    esi, ebx
        push   eax
        call   sub_1_1ADC
        add    dword ptr [ebp + 14h], ebx
        add    dword ptr [ebp - 0ch], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_1190:
        cmp    dword ptr [ebp + 10h], esi
        ja     SHORT loc_1_1120
loc_1_1195:
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], esi
loc_1_119A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_11B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 0d5h]
        mov    dword ptr [ebx + 15eh], esi
        lea    edi, [esi + 24h]
        mov    byte ptr [ebx + 0fbh], 1
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [edi], 0
        jne    SHORT loc_1_11E7
        mov    dword ptr [ebx + 0d9h], 0
loc_1_11E7:
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebx + 0d5h], eax
        mov    dword ptr [ebx + 15eh], esi
        lea    eax, [ebp - 0ch]
        push   eax
        push   0
        push   0
        push   0
        push   esi
        call   sub_1_1A20
        mov    eax, dword ptr [ebx + 162h]
        add    eax, 0eh
        mov    ecx, dword ptr [ebp - 0ch]
        cmp    eax, ecx
        jb     SHORT loc_1_1217
        mov    eax, ecx
loc_1_1217:
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebx + 186h]
        push   eax
        push   0eah
        push   0
        push   esi
        call   sub_1_10D0
        cmp    dword ptr [ebp - 4], 0eh
        jae    SHORT loc_1_124A
        cmp    dword ptr [ebp - 4], 6
        jb     SHORT loc_1_1276
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        jmp    SHORT loc_1_1261
loc_1_124A:
        mov    eax, dword ptr [ebp - 0ch]
        sub    eax, 0eh
        push   eax
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0eh
        push   eax
        lea    eax, [ebx + 194h]
        push   eax
        push   0eh
loc_1_1261:
        lea    eax, [ebx + 186h]
        push   eax
        push   eax
        push   esi
        mov    eax, dword ptr [ebx + 0adh]
        push   eax
        call   sub_1_15A0
loc_1_1276:
        cmp    byte ptr [edi + 0ah], 1
        sbb    eax, eax
        and    eax, 0c0000001h
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   esi
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_193A
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        dec    dword ptr [eax + 18h]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_12A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    dword ptr [ebp + 10h], 0eh
        mov    esi, dword ptr [eax + 8]
        inc    dword ptr [esi + 80h]
        cmp    byte ptr [eax + 1ch], 0
        jne    NEAR PTR loc_1_14A0
        inc    dword ptr [eax + 18h]
        mov    ecx, dword ptr [ebp + 0ch]
        test   cl, 1
        jne    SHORT loc_1_12EB
        mov    edx, dword ptr [ebp + 1ch]
        push   edx
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        call   sub_1_1A6E
        jmp    NEAR PTR loc_1_1494
loc_1_12EB:
        mov    eax, dword ptr [esi + 64h]
        dec    eax
        mov    dword ptr [ebp - 24h], eax
        mov    edx, dword ptr [ebp + 1ch]
        mov    dword ptr [ebp - 0ch], 0
        mov    edi, ecx
        mov    dword ptr [edx], 0
        and    edi, 0feh
        shr    edi, 1
        and    ecx, 7f00h
        shr    ecx, 8
        mov    dword ptr [ebp - 20h], ecx
        push   0
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        call   sub_1_1A20
        cmp    dword ptr [ebp - 28h], 0
        je     NEAR PTR loc_1_1494
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_1A44
        mov    eax, dword ptr [esi + 104h]
        mov    dword ptr [ebp - 4], edi
        mov    eax, dword ptr [eax + edi*4]
        mov    dword ptr [ebp - 8], eax
        lea    eax, [edi*8]
        add    eax, dword ptr [esi + 0d1h]
        mov    dword ptr [ebp - 18h], eax
        cmp    dword ptr [ebp - 20h], edi
        jne    SHORT loc_1_1380
        mov    di, word ptr [eax + 6]
        and    edi, 0fffh
        sub    edi, 4
        jmp    SHORT loc_1_1383
        db  005h, 000h, 000h, 000h, 000h
loc_1_1380:
        mov    edi, dword ptr [esi + 68h]
loc_1_1383:
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_1_148C
loc_1_1390:
        cmp    dword ptr [ebp - 14h], 0
        jne    SHORT loc_1_13C3
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_1A68
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_148C
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_1A44
        jmp    NEAR PTR loc_1_1480
loc_1_13C3:
        or     edi, edi
        jne    SHORT loc_1_1428
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 20h], eax
        je     NEAR PTR loc_1_148C
        cmp    dword ptr [ebp - 24h], eax
        jne    SHORT loc_1_13F0
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [esi + 0d1h]
        mov    dword ptr [ebp - 18h], eax
        jmp    SHORT loc_1_13F7
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_13F0:
        inc    dword ptr [ebp - 4]
        add    dword ptr [ebp - 18h], 8
loc_1_13F7:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 20h], eax
        jne    SHORT loc_1_1414
        mov    eax, dword ptr [ebp - 18h]
        mov    di, word ptr [eax + 6]
        and    edi, 0fffh
        sub    edi, 4
        sub    edi, dword ptr [ebp - 0ch]
        jmp    SHORT loc_1_1417
loc_1_1414:
        mov    edi, dword ptr [esi + 68h]
loc_1_1417:
        mov    ecx, dword ptr [esi + 104h]
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx + eax*4]
        mov    dword ptr [ebp - 8], eax
        jmp    SHORT loc_1_1480
loc_1_1428:
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_1_144D
        cmp    dword ptr [ebp + 10h], edi
        jbe    SHORT loc_1_143A
        sub    dword ptr [ebp + 10h], edi
        sub    edi, edi
        jmp    SHORT loc_1_1480
loc_1_143A:
        mov    eax, dword ptr [ebp - 8]
        add    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 8], eax
        sub    edi, dword ptr [ebp + 10h]
        mov    dword ptr [ebp + 10h], 0
loc_1_144D:
        mov    ebx, dword ptr [ebp + 14h]
        sub    ebx, dword ptr [ebp - 0ch]
        mov    eax, edi
        mov    ecx, dword ptr [ebp - 14h]
        cmp    eax, ecx
        jb     SHORT loc_1_145E
        mov    eax, ecx
loc_1_145E:
        cmp    ebx, eax
        jb     SHORT loc_1_1464
        mov    ebx, eax
loc_1_1464:
        push   ebx
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        sub    edi, ebx
        push   eax
        call   sub_1_1ADC
        add    dword ptr [ebp - 1ch], ebx
        add    dword ptr [ebp - 8], ebx
        add    dword ptr [ebp - 0ch], ebx
        sub    dword ptr [ebp - 14h], ebx
loc_1_1480:
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp + 14h], eax
        ja     NEAR PTR loc_1_1390
loc_1_148C:
        mov    edx, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [edx], eax
loc_1_1494:
        mov    eax, dword ptr [ebp + 8]
        sub    edi, edi
        dec    dword ptr [eax + 18h]
        jmp    SHORT loc_1_14A5
        db  08Bh, 0FFh
loc_1_14A0:
        mov    edi, 0c001000ch
loc_1_14A5:
        mov    eax, dword ptr [esi + 80h]
        dec    eax
        mov    dword ptr [esi + 80h], eax
        jne    SHORT loc_1_14E3
        cmp    byte ptr [esi + 148h], 0
        jne    SHORT loc_1_14D5
        cmp    dword ptr [esi + 0d5h], 0
        jne    SHORT loc_1_14D5
        mov    eax, dword ptr [esi + 8ch]
        sub    eax, esi
        cmp    eax, 8ch
        je     SHORT loc_1_14E3
loc_1_14D5:
        push   0
        add    esi, 0a0h
        push   esi
        call   sub_1_19D2
loc_1_14E3:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Bh, 0FFh
loc_1_14F0:
        db  0A0h, 0CCh, 01Dh, 000h, 000h
        cmp    al, 0
        jne    SHORT loc_1_1515
        db  0FEh, 005h, 0CCh, 01Dh, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     SHORT loc_1_1517
        push   0
        push   0
        db  0E8h, 080h, 000h, 0FFh, 0FFh
        cmp    eax, 0
        jne    SHORT loc_1_1517
loc_1_1515:
        clc
        ret
loc_1_1517:
        stc
        ret
DECLAN_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_1528
        db  0FFh, 024h, 085h, 02Ah, 015h, 000h, 000h
loc_1_1528:
        clc
        ret
        db  0F0h, 014h, 000h, 000h, 028h, 015h, 000h, 000h, 028h, 015h, 000h, 000h, 028h, 015h, 000h, 000h
        db  028h, 015h, 000h, 000h, 028h, 015h, 000h, 000h, 028h, 015h, 000h, 000h, 028h, 015h, 000h, 000h
        db  028h, 015h, 000h, 000h, 028h, 015h, 000h, 000h, 028h, 015h, 000h, 000h, 028h, 015h, 000h, 000h
        db  028h, 015h, 000h, 000h, 028h, 015h, 000h, 000h, 028h, 015h, 000h, 000h, 028h, 015h, 000h, 000h
        db  028h, 015h, 000h, 000h, 028h, 015h, 000h, 000h, 028h, 015h, 000h, 000h, 028h, 015h, 000h, 000h
        db  028h, 015h, 000h, 000h, 028h, 015h, 000h, 000h, 028h, 015h, 000h, 000h, 028h, 015h, 000h, 000h
        db  028h, 015h, 000h, 000h, 028h, 015h, 000h, 000h, 0F0h, 014h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_1_15A0:
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
        call   sub_1_15D0
        pop    ebp
        ret    20h
        db  08Dh, 064h, 024h, 000h
sub_1_15D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 18h], 0eh
        jb     NEAR PTR loc_1_1670
        mov    edx, dword ptr [ebp + 10h]
        mov    al, byte ptr [edx]
        test   al, 1
        je     SHORT loc_1_1610
        cmp    dword ptr [edx + 2], -1
        jne    SHORT loc_1_1605
        cmp    al, 0ffh
        jne    SHORT loc_1_1605
        cmp    byte ptr [edx + 1], 0ffh
        jne    SHORT loc_1_1605
        mov    dword ptr [ebp - 4], 8
        jmp    SHORT loc_1_1677
loc_1_1605:
        mov    dword ptr [ebp - 4], 2
        jmp    SHORT loc_1_1677
        db  08Bh, 0FFh
loc_1_1610:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0ch], 20h
        je     SHORT loc_1_1660
        mov    edx, dword ptr [ebp + 10h]
        lea    ecx, [esi + 2ch]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_1636
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_163D
loc_1_1636:
        mov    dword ptr [ebp - 10h], 1
loc_1_163D:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_1650
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_167A
        db  08Dh, 064h, 024h, 000h
loc_1_1650:
        mov    dword ptr [ebp - 4], 20h
        jmp    SHORT loc_1_167A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1660:
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_167A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1670:
        mov    dword ptr [ebp - 4], 20h
loc_1_1677:
        mov    esi, dword ptr [ebp + 8]
loc_1_167A:
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     NEAR PTR loc_1_1771
loc_1_1685:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, ebx
        and    eax, dword ptr [ebp - 4]
        test   al, 9
        jne    SHORT loc_1_16DD
        test   al, 2
        je     SHORT loc_1_16C9
        lea    eax, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 006h, 021h, 0FFh, 0FFh
        or     al, al
        je     SHORT loc_1_16C9
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 19h]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        test   dword ptr [eax + ecx*4], edx
        jne    SHORT loc_1_16DD
loc_1_16C9:
        test   byte ptr [ebp - 4], 2
        je     SHORT loc_1_16D4
        test   bl, 4
        jne    SHORT loc_1_16DD
loc_1_16D4:
        test   bl, 20h
        je     NEAR PTR loc_1_1766
loc_1_16DD:
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
        call   sub_1_1952
        mov    byte ptr [edi + 18h], 1
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_1766
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_171F
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_171F:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_1730
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_1736
        db  08Bh, 0FFh
loc_1_1730:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_1736:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_1745
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_1958
loc_1_1745:
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
        call   sub_1_19FC
        mov    edi, ebx
        jmp    SHORT loc_1_1769
loc_1_1766:
        mov    edi, dword ptr [edi + 10h]
loc_1_1769:
        or     edi, edi
        jne    NEAR PTR loc_1_1685
loc_1_1771:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1780:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1790
        pop    ebp
        ret    4
sub_1_1790:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     SHORT loc_1_180E
loc_1_17A0:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_1807
        mov    byte ptr [edi + 18h], 0
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_1958
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_1807
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_17CC
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_17CC:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_17E0
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_17E6
        db  005h, 000h, 000h, 000h, 000h
loc_1_17E0:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_17E6:
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
        call   sub_1_19FC
        mov    edi, ebx
        jmp    SHORT loc_1_180A
loc_1_1807:
        mov    edi, dword ptr [edi + 10h]
loc_1_180A:
        or     edi, edi
        jne    SHORT loc_1_17A0
loc_1_180E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1820:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0ch]
        test   al, 20h
        je     SHORT loc_1_1835
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_1835:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     SHORT loc_1_1871
        cmp    dword ptr [ecx + 2], -1
        jne    SHORT loc_1_1862
        cmp    bl, 0ffh
        jne    SHORT loc_1_1862
        cmp    byte ptr [ecx + 1], 0ffh
        jne    SHORT loc_1_1862
        test   al, 8
        je     SHORT loc_1_185B
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_185B:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_1862:
        test   al, 4
        jne    SHORT loc_1_186A
        test   al, 2
        je     SHORT loc_1_1889
loc_1_186A:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_1871:
        mov    eax, dword ptr [edx + 2eh]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_1889
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 2ch], ax
        jne    SHORT loc_1_1889
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_1889:
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
sub_1_190A:
        db  0CDh, 020h, 031h, 080h, 028h, 000h
sub_1_1910:
        db  0CDh, 020h, 032h, 080h, 028h, 000h
sub_1_1916:
        db  0CDh, 020h, 033h, 080h, 028h, 000h
sub_1_191C:
        db  0CDh, 020h, 034h, 080h, 028h, 000h
sub_1_1922:
        db  0CDh, 020h, 035h, 080h, 028h, 000h
sub_1_1928:
        db  0CDh, 020h, 036h, 080h, 028h, 000h
        db  0CDh, 020h, 037h, 080h, 028h, 000h
sub_1_1934:
        db  0CDh, 020h, 038h, 080h, 028h, 000h
sub_1_193A:
        db  0CDh, 020h, 039h, 080h, 028h, 000h
        db  0CDh, 020h, 03Ah, 080h, 028h, 000h
sub_1_1946:
        db  0CDh, 020h, 03Bh, 080h, 028h, 000h
sub_1_194C:
        db  0CDh, 020h, 03Ch, 080h, 028h, 000h
sub_1_1952:
        db  0CDh, 020h, 03Dh, 080h, 028h, 000h
sub_1_1958:
        db  0CDh, 020h, 03Eh, 080h, 028h, 000h
sub_1_195E:
        db  0CDh, 020h, 03Fh, 080h, 028h, 000h
sub_1_1964:
        db  0CDh, 020h, 040h, 080h, 028h, 000h
        db  0CDh, 020h, 041h, 080h, 028h, 000h
        db  0CDh, 020h, 009h, 080h, 028h, 000h
sub_1_1976:
        db  0CDh, 020h, 008h, 080h, 028h, 000h
sub_1_197C:
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
sub_1_19D2:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
sub_1_19D8:
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
sub_1_19DE:
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
sub_1_19E4:
        db  0CDh, 020h, 010h, 080h, 028h, 000h
sub_1_19EA:
        db  0CDh, 020h, 011h, 080h, 028h, 000h
sub_1_19F0:
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_19F6:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_19FC:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_1A20:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_1A44:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_1A68:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
sub_1_1A6E:
        db  0CDh, 020h, 028h, 080h, 028h, 000h
sub_1_1A74:
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_1A7A:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_1A80:
        db  0CDh, 020h, 007h, 080h, 028h, 000h
sub_1_1A86:
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
sub_1_1AC2:
        db  0CDh, 020h, 046h, 080h, 028h, 000h
sub_1_1AC8:
        db  0CDh, 020h, 045h, 080h, 028h, 000h
        db  0CDh, 020h, 043h, 080h, 028h, 000h
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        add    byte ptr [eax], al
sub_1_1ADC:
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
sub_1_1AFC:
        push   esi
        push   edi
        pushfd
        mov    ecx, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        cmp    esi, edi
        jae    SHORT loc_1_1B1C
        or     ecx, ecx
        je     SHORT loc_1_1B32
        std
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    SHORT loc_1_1B30
loc_1_1B1C:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     SHORT loc_1_1B30
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd
        mov    cl, al
loc_1_1B30:
        rep movsb
loc_1_1B32:
        popfd
        pop    edi
        pop    esi
        ret    0ch
        db  0FFh, 0FFh, 0FFh, 0FFh
loc_1_1B3C:
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
loc_1_1BB4:
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
loc_1_1C0C:
        dec    esp
        popad
        outsb
        arpl   word ptr [ebp + 20h], sp
        inc    esp
        inc    ebp
        xor    dh, byte ptr [eax]
        xor    dword ptr [eax], esp
        inc    ecx
        popad
        jo     SHORT loc_1_1C91
        db  065h, 072h, 02Eh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1C24:
        dec    esp
        popad
        outsb
        arpl   word ptr [ebp + 20h], sp
        inc    esp
        inc    ebp
        xor    dword ptr [eax], esi
        xor    byte ptr [eax], ah
        inc    ecx
        popad
        db  070h, 074h
        db  065h, 072h, 02Eh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1C3C:
        dec    esp
        popad
        outsb
        arpl   word ptr [ebp + 20h], sp
        inc    esp
        inc    ebp
        xor    al, 32h
        xor    ah, byte ptr [eax]
        inc    ecx
        popad
        jo     SHORT loc_1_1CC1
        db  065h, 072h, 02Eh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1C54:
        dec    esp
        popad
        outsb
        arpl   word ptr [ebp + 20h], sp
        inc    ecx
        popad
        jo     SHORT loc_1_1CD3
        jb     NEAR PTR loc_1_1C90
        add    byte ptr [eax], al
        inc    esp
        arpl   word ptr gs:[ecx + 6eh], bp
        add    byte ptr [eax], al
loc_1_1C6C:
        inc    esp
        arpl   word ptr gs:[ecx + 6eh], bp
        add    byte ptr [eax], al
        dec    ecx
        dec    edi
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
        add    byte ptr [ebp + 41h], cl
        pop    eax
        dec    ebp
loc_1_1C90:
        push   ebp
loc_1_1C91:
        dec    esp
        push   esp
        dec    ecx
        inc    ebx
        inc    ecx
        push   ebx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1C9C:
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
loc_1_1CC1:
        push   ebx
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
loc_1_1CD3:
        inc    ebp
        push   edx
        push   edx
        push   ebp
        push   eax
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [ecx + 64h], al
        popad
        db  070h, 074h
        db  065h, 072h, 04Eh
        popad
        insd
        add    byte ptr gs:[ecx + 64h], al
        popad
        jo     SHORT loc_1_1D61
        db  065h, 072h, 04Eh
        popad
        insd
        add    byte ptr gs:[edx + 41h], dl
        dec    ebp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
loc_1_1D00:
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
        inc    esp
        inc    ebp
        xor    dh, byte ptr [eax]
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_1D14:
        inc    esp
        inc    ebp
        xor    dh, byte ptr [eax]
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        inc    esp
        inc    ebp
        xor    dword ptr [eax], esi
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1D24:
        inc    esp
        inc    ebp
        xor    dword ptr [eax], esi
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esp
        inc    ebp
        push   eax
        inc    ebx
        inc    ecx
        add    byte ptr [eax], al
        db  000h
loc_1_1D34:
        inc    esp
        inc    ebp
        push   eax
        inc    ebx
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [ebp + eax*2 + 43h], al
        push   esp
        inc    ebx
        add    byte ptr [eax], al
        add    byte ptr [ebp + eax*2 + 43h], al
        push   esp
        inc    ebx
        add    byte ptr [eax], al
        add    byte ptr [ebp + eax*2 + 34h], al
        xor    dh, byte ptr [edx]
        add    byte ptr [eax], al
        add    byte ptr [ebp + eax*2 + 34h], al
        xor    dh, byte ptr [edx]
        add    byte ptr [eax], al
        add    byte ptr [ebp + eax*2 + 32h], al
        xor    byte ptr [eax], dh
loc_1_1D61:
        add    byte ptr [eax], al
        add    byte ptr [ebp + eax*2 + 32h], al
        xor    byte ptr [eax], dh
        add    byte ptr [eax], al
        add    byte ptr [ebp + eax*2 + 31h], al
        xor    byte ptr [ecx], dh
        add    byte ptr [eax], al
        db  000h
loc_1_1D74:
        inc    esp
        inc    ebp
        xor    dword ptr [eax], esi
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 00Ah, 000h, 000h, 044h, 045h, 043h, 04Ch
        db  041h, 04Eh, 020h, 020h, 000h, 040h, 000h, 054h, 019h, 015h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
