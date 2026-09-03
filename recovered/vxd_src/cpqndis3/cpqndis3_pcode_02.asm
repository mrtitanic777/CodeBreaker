; cpqndis3_pcode_02.asm -- CPQNDIS3 object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  06Ah, 000h, 06Ah, 000h, 0E8h, 057h, 00Ch, 000h, 000h, 083h, 0F8h, 001h, 01Ah, 0C9h, 0F6h, 0D9h
        db  08Ah, 0C1h, 0C2h, 004h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0020:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 100h
        mov    dword ptr [eax + 114h], 8000h
        push   esi
        push   edi
        xor    esi, esi
        mov    dword ptr [eax + 110h], esi
loc_2_0041:
        lea    edi, [ebp - 100h]
        mov    eax, 0aa55ff00h
        mov    ecx, 40h
        push   100h
        rep stosd
        lea    eax, [ebp - 100h]
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0065:
        test   al, al
        je     SHORT loc_2_00BE
        lea    edi, [ebp - 100h]
        xor    eax, eax
        mov    ecx, 40h
        push   100h
        rep stosd
        push   eax
        lea    eax, [ebp - 100h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_008A:
        test   al, al
        je     SHORT loc_2_00C8
        lea    eax, [ebp - 100h]
loc_2_0094:
        cmp    dword ptr [eax], 0aa55ff00h
        jne    SHORT loc_2_00D2
        add    eax, 4
        lea    ecx, [ebp]
        cmp    eax, ecx
        jb     SHORT loc_2_0094
        add    esi, 100h
        cmp    esi, 8000h
        jb     SHORT loc_2_0041
        mov    al, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_00BE:
        xor    al, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_00C8:
        xor    al, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_00D2:
        xor    al, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_2_00E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_00F1:
        push   50h
        push   0eh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0a0h
        push   4
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0
        push   0dh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   20h
        push   0ch
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0ch
        push   1
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   80h
        push   2
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0ch
        push   3
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   3ch
        mov    bl, 14h
        push   5
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0
        push   6
        db  0FFh, 015h, 000h, 000h, 000h, 000h
loc_2_0153:
        push   64h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_015A:
        dec    bl
        jne    SHORT loc_2_0153
        push   0ffh
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   41h
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0ch
        mov    bl, 14h
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   1
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
loc_2_018B:
        push   64h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0192:
        dec    bl
        jne    SHORT loc_2_018B
        lea    eax, [ebp - 1]
        push   eax
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        test   byte ptr [ebp - 1], 1
        jne    SHORT loc_2_01B2
        xor    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_01B2:
        push   37h
        push   0ah
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0ah
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    byte ptr [esi + 11ch], 1
        push   0
        push   0fh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   48h
        push   0eh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0
        push   0dh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0
        push   0ch
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0ffh
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   21h
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0
        mov    bl, 0ffh
        push   0ah
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0
        push   0bh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
loc_2_022C:
        dec    bl
        je     SHORT loc_2_0242
        lea    eax, [ebp - 1]
        push   eax
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        test   byte ptr [ebp - 1], 80h
        je     SHORT loc_2_022C
loc_2_0242:
        push   2
        push   0dh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   22h
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_2_0020
        push   esi
        test   al, al
        jne    SHORT loc_2_0270
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0266:
        xor    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0270:
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0275:
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_2_0280:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 8]
loc_2_0289:
        db  08Ah, 090h, 027h, 000h, 000h, 000h
        inc    eax
        cmp    eax, 6
        mov    byte ptr [ecx + eax + 109h], dl
        jb     SHORT loc_2_0289
        cmp    byte ptr [ecx + 104h], 0
        jne    SHORT loc_2_031A
        cmp    byte ptr [ecx + 105h], 0
        jne    SHORT loc_2_031A
        cmp    byte ptr [ecx + 106h], 0
        jne    SHORT loc_2_031A
        cmp    byte ptr [ecx + 107h], 0
        jne    SHORT loc_2_031A
        cmp    byte ptr [ecx + 108h], 0
        jne    SHORT loc_2_031A
        cmp    byte ptr [ecx + 109h], 0
        jne    SHORT loc_2_031A
        mov    al, byte ptr [ecx + 10ah]
        mov    dl, byte ptr [ecx + 10bh]
        mov    byte ptr [ecx + 104h], al
        mov    byte ptr [ecx + 105h], dl
        mov    al, byte ptr [ecx + 10ch]
        mov    dl, byte ptr [ecx + 10dh]
        mov    byte ptr [ecx + 106h], al
        mov    byte ptr [ecx + 107h], dl
        mov    al, byte ptr [ecx + 10eh]
        mov    dl, byte ptr [ecx + 10fh]
        mov    byte ptr [ecx + 108h], al
        mov    byte ptr [ecx + 109h], dl
loc_2_031A:
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_2_0320:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   2
        xor    bl, bl
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0330:
        push   0
        call   sub_2_0B80
        db  0FFh, 015h, 000h, 000h, 000h, 000h
loc_2_033D:
        call   sub_2_0AE0
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0347:
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_034C:
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0351:
        db  0A0h, 000h, 000h, 000h, 000h
        push   eax
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   21h
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 1]
        push   ecx
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        cmp    byte ptr [ebp - 1], 21h
        je     SHORT loc_2_038D
        cmp    byte ptr [ebp - 1], 23h
        je     SHORT loc_2_038D
        inc    bl
        call   sub_2_0A70
        cmp    bl, 3
        jb     SHORT loc_2_033D
        jmp    SHORT loc_2_0401
loc_2_038D:
        push   21h
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   48h
        push   0eh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0
        push   0ah
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0
        push   0bh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0ch
        push   0ch
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   2
        push   0dh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0ch
        push   3
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0ch
        push   1
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   80h
        push   2
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0ffh
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0
        push   0fh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
loc_2_0401:
        cmp    bl, 3
        jae    SHORT loc_2_045E
        call   sub_2_0900
        call   sub_2_0470
        push   1
        call   sub_2_0540
        test   al, al
        je     SHORT loc_2_0427
        mov    eax, 1
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0427:
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_0455
        push   1
        call   sub_2_0B80
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0442:
        push   3
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0449:
        db  0A0h, 000h, 000h, 000h, 000h
        push   eax
        db  0FFh, 015h, 000h, 000h, 000h, 000h
loc_2_0455:
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_045E:
        mov    eax, 1
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_0470:
        push   ebx
        db  0A0h, 000h, 000h, 000h, 000h
        push   esi
        test   al, al
        jne    NEAR PTR loc_2_0525
        xor    esi, esi
loc_2_0481:
        push   3
        call   sub_2_0540
        mov    bl, al
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_048F:
        push   21h
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   48h
        push   0eh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0
        push   0ah
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0
        push   0bh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0ch
        push   0ch
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   2
        push   0dh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0ch
        push   3
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0ch
        push   1
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   80h
        push   2
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0ffh
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0
        push   0fh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        test   bl, bl
        je     SHORT loc_2_0511
        inc    esi
        cmp    esi, 3
        jb     NEAR PTR loc_2_0481
loc_2_0511:
        cmp    esi, 3
        jne    SHORT loc_2_0535
        db  0A0h, 000h, 000h, 000h, 000h
        pop    esi
        and    al, 0fbh
        pop    ebx
        db  0A2h, 000h, 000h, 000h, 000h
        ret
loc_2_0525:
        cmp    al, 2
        jne    SHORT loc_2_0535
        db  0A0h, 000h, 000h, 000h, 000h
        and    al, 0fbh
        db  0A2h, 000h, 000h, 000h, 000h
loc_2_0535:
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0540:
        push   ebp
        mov    ebp, esp
        sub    esp, 5ch
        push   ebx
        push   esi
        pushf
        cli
        xor    ax, ax
loc_2_054E:
        mov    ecx, eax
        inc    ax
        and    ecx, 0ffffh
        cmp    ax, 6
        db  08Ah, 091h, 027h, 000h, 000h, 000h
        mov    byte ptr [ebp + ecx - 56h], dl
        mov    byte ptr [ebp + ecx - 5ch], dl
        jb     SHORT loc_2_054E
        xor    cx, cx
        lea    eax, [ebp - 4eh]
        mov    word ptr [ebp - 50h], 40h
loc_2_0578:
        mov    byte ptr [eax], cl
        inc    eax
        inc    cx
        cmp    cx, 40h
        jb     SHORT loc_2_0578
        lea    eax, [ebp - 0ch]
        push   4eh
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        lea    eax, [ebp - 5ch]
        push   eax
        push   0
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_05A0:
        test   al, al
        jne    SHORT loc_2_05B0
        popf
        mov    al, 0ffh
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_05B0:
        push   40h
        push   0eh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   1fh
        push   0ch
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [ebp + 8]
        and    al, 3
        add    al, al
        or     al, 1
        push   eax
        push   0dh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   4eh
        push   5
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   0
        push   6
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        xor    si, si
        push   0
        push   4
        mov    ebx, 2
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        push   26h
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
loc_2_0606:
        lea    eax, [ebp - 1]
        push   eax
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        test   byte ptr [ebp - 1], bl
        jne    SHORT loc_2_0620
        inc    si
        cmp    si, 0ffffh
        jb     SHORT loc_2_0606
loc_2_0620:
        cmp    si, 0ffffh
        jne    SHORT loc_2_0633
        popf
        mov    al, 0ffh
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0633:
        xor    si, si
loc_2_0636:
        mov    eax, esi
        inc    si
        and    eax, 0ffffh
        lea    ecx, [ebp + eax - 0ch]
        push   ecx
        push   6
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        cmp    si, 8
        jb     SHORT loc_2_0636
        push   21h
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        cmp    word ptr [ebp - 0ch], 3b00h
        jne    SHORT loc_2_0679
        cmp    dword ptr [ebp - 0ah], 3f3e3d3ch
        jne    SHORT loc_2_0679
        popf
        xor    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0679:
        popf
        mov    al, 0ffh
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0690:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    al, byte ptr [esp + 8]
        mov    ax, word ptr [eax*2 + 408h]
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_06B0:
        push   ebp
        mov    al, byte ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_2_0690
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        test   ax, ax
        je     SHORT loc_2_06DF
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        in     al, dx
        db  0A2h, 000h, 000h, 000h, 000h
        add    dx, 2
        in     al, dx
        db  0A2h, 000h, 000h, 000h, 000h
loc_2_06DF:
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_06F0:
        db  066h, 083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_071A
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        db  0A0h, 000h, 000h, 000h, 000h
        out    dx, al
        db  0A0h, 000h, 000h, 000h, 000h
        add    dx, 2
        out    dx, al
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_071A:
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_2_0720:
        push   ebx
        push   bx
        xor    ebx, ebx
        pop    bx
        xor    bl, al
        sub    bh, bh
        shl    bx, 1
        mov    al, ah
        sub    ah, ah
        db  066h, 033h, 083h, 000h, 000h, 000h, 000h
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h
sub_2_0740:
        push   ax
        push   ecx
        mov    ecx, 30h
loc_2_0748:
        loop   loc_2_0748
        pop    ecx
        pop    ax
        ret
        db  08Bh, 0FFh, 066h, 050h, 051h, 0B9h, 000h, 006h, 000h, 000h, 0E4h, 061h, 0E2h, 0FCh, 059h, 066h
        db  058h, 0C3h
sub_2_0760:
        push   ax
        call   sub_2_0740
        mov    al, 3
        out    dx, al
        call   sub_2_0740
        mov    al, 2
        out    dx, al
        call   sub_2_0740
        mov    al, 0
        out    dx, al
        pop    ax
        ret
        db  08Dh, 049h, 000h
sub_2_0780:
        push   ax
        call   sub_2_0740
        mov    al, 0
        out    dx, al
        call   sub_2_0740
        mov    al, 2
        out    dx, al
        call   sub_2_0740
        mov    al, 3
        out    dx, al
        pop    ax
        ret
        db  08Dh, 049h, 000h
sub_2_07A0:
        push   ax
        call   sub_2_0740
        mov    al, 1
        out    dx, al
        call   sub_2_0740
        mov    al, 3
        out    dx, al
        call   sub_2_0740
        mov    al, 1
        out    dx, al
        pop    ax
        ret
        db  08Dh, 049h, 000h
sub_2_07C0:
        push   ax
        call   sub_2_0740
        mov    al, 0
        out    dx, al
        call   sub_2_0740
        mov    al, 2
        out    dx, al
        call   sub_2_0740
        mov    al, 0
        out    dx, al
        pop    ax
        ret
        db  08Dh, 049h, 000h
sub_2_07E0:
        call   sub_2_0740
        mov    al, 1
        out    dx, al
        call   sub_2_0740
        mov    al, 3
        out    dx, al
        call   sub_2_0740
        inc    dx
        in     al, dx
        dec    dx
        mov    ah, al
        call   sub_2_0740
        mov    al, 1
        out    dx, al
        call   sub_2_0740
        and    ah, 8
        ror    ah, 3
        xor    ah, 1
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0820:
        push   ax
        xor    al, al
        or     al, 0a0h
        call   sub_2_0760
        push   ecx
        mov    ecx, 8
loc_2_0831:
        shl    al, 1
        jae    SHORT loc_2_083D
        call   sub_2_07A0
        jmp    SHORT loc_2_0842
loc_2_083D:
        call   sub_2_07C0
loc_2_0842:
        loop   loc_2_0831
        pop    ecx
        call   sub_2_07E0
        pop    ax
        push   ecx
        mov    ecx, 8
loc_2_0852:
        shl    al, 1
        jae    SHORT loc_2_085E
        call   sub_2_07A0
        jmp    SHORT loc_2_0863
loc_2_085E:
        call   sub_2_07C0
loc_2_0863:
        loop   loc_2_0852
        pop    ecx
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0870:
        xor    al, al
        add    al, 1
        or     al, 0a0h
        call   sub_2_0760
        push   ecx
        mov    ecx, 8
loc_2_0881:
        shl    al, 1
        jae    SHORT loc_2_088D
        call   sub_2_07A0
        jmp    SHORT loc_2_0892
loc_2_088D:
        call   sub_2_07C0
loc_2_0892:
        loop   loc_2_0881
        pop    ecx
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_08A0:
        push   ax
        push   ecx
        xor    bl, bl
        mov    ecx, 8
loc_2_08AA:
        shl    bl, 1
        call   sub_2_07E0
        or     bl, ah
        loop   loc_2_08AA
        pop    ecx
        pop    ax
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_08C0:
        mov    al, 11h
        out    dx, al
        add    dx, 2
        mov    al, 1dh
        out    dx, al
        mov    al, 15h
        out    dx, al
        call   sub_2_0740
        sub    dx, 2
        mov    al, 3
        out    dx, al
        call   sub_2_0740
        add    dx, 2
        mov    al, 17h
        out    dx, al
        sub    dx, 2
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_08F0:
        add    dx, 2
        mov    al, 15h
        out    dx, al
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0900:
        push   ebx
        push   edi
        push   ax
        push   bx
        push   ecx
        push   dx
        push   edi
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        call   sub_2_08C0
        mov    ax, 100h
        call   sub_2_0820
        call   sub_2_07E0
        call   sub_2_0870
        call   sub_2_07E0
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 80h
        xor    ax, ax
loc_2_093B:
        call   sub_2_08A0
        call   sub_2_0720
        mov    byte ptr [edi], bl
        cmp    ecx, 1
        je     SHORT loc_2_0953
        call   sub_2_07C0
        jmp    SHORT loc_2_0958
loc_2_0953:
        call   sub_2_07A0
loc_2_0958:
        inc    edi
        loop   loc_2_093B
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        call   sub_2_07E0
        call   sub_2_0780
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
        db  066h, 083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        je     SHORT loc_2_0983
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 001h
loc_2_0983:
        call   sub_2_08F0
        pop    edi
        pop    dx
        pop    ecx
        pop    bx
        pop    ax
        pop    edi
        pop    ebx
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 066h, 053h, 032h
        db  0E4h, 086h, 0D8h, 0E8h, 075h, 0FEh, 0FFh, 0FFh, 0E8h, 0A0h, 0FDh, 0FFh, 0FFh, 0E8h, 02Bh, 0FEh
        db  0FFh, 0FFh, 0F6h, 0C4h, 001h, 075h, 027h, 051h, 0B9h, 008h, 000h, 000h, 000h, 0C0h, 0E3h, 001h
        db  073h, 007h, 0E8h, 0D6h, 0FDh, 0FFh, 0FFh, 0EBh, 005h, 0E8h, 0EFh, 0FDh, 0FFh, 0FFh, 0E2h, 0EDh
        db  059h, 0E8h, 007h, 0FEh, 0FFh, 0FFh, 0E8h, 0A2h, 0FDh, 0FFh, 0FFh, 0F8h, 0EBh, 001h, 0F9h, 066h
        db  05Bh, 0C3h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 053h, 056h, 066h
        db  050h, 066h, 053h, 051h, 066h, 052h, 056h, 066h, 08Bh, 015h, 000h, 000h, 000h, 000h, 0BEh, 000h
        db  000h, 000h, 000h, 0FCh, 0B9h, 07Eh, 000h, 000h, 000h, 066h, 033h, 0C0h, 066h, 08Bh, 0D8h, 0ACh
        db  066h, 093h, 0E8h, 006h, 0FDh, 0FFh, 0FFh, 0E2h, 0F3h, 08Bh, 035h, 000h, 000h, 000h, 000h, 066h
        db  089h, 046h, 07Eh, 0B9h, 080h, 000h, 000h, 000h, 032h, 0DBh, 0E8h, 08Eh, 0FEh, 0FFh, 0FFh, 0E8h
        db  019h, 0FDh, 0FFh, 0FFh, 0E8h, 014h, 0FDh, 0FFh, 0FFh, 0ACh, 0E8h, 05Eh, 0FFh, 0FFh, 0FFh, 072h
        db  00Dh, 0FEh, 0C3h, 0E2h, 0EAh, 0C6h, 005h, 000h, 000h, 000h, 000h, 000h, 0EBh, 007h, 0C6h, 005h
        db  000h, 000h, 000h, 000h, 001h, 0E8h, 093h, 0FEh, 0FFh, 0FFh, 05Eh, 066h, 05Ah, 059h, 066h, 05Bh
        db  066h, 058h, 05Eh, 05Bh, 0C3h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0A70:
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        mov    word ptr [ebp - 0ah], cx
        push   esi
        mov    byte ptr [ebp - 3], 0
        mov    dx, word ptr [ebp - 0ah]
        mov    al, byte ptr [ebp - 3]
        out    dx, al
        mov    byte ptr [ebp - 2], 0bh
        add    cx, 2
        mov    word ptr [ebp - 8], cx
        mov    dx, word ptr [ebp - 8]
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        mov    esi, 1388h
loc_2_0AA7:
        push   64h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0AAE:
        dec    esi
        jne    SHORT loc_2_0AA7
        mov    byte ptr [ebp - 1], 0ech
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        add    ax, 2
        mov    word ptr [ebp - 6], ax
        mov    dx, word ptr [ebp - 6]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    esi, 7d0h
loc_2_0AD0:
        push   64h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0AD7:
        dec    esi
        jne    SHORT loc_2_0AD0
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  090h
sub_2_0AE0:
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        mov    word ptr [ebp - 16h], cx
        push   esi
        mov    byte ptr [ebp - 7], 30h
        mov    dx, word ptr [ebp - 16h]
        mov    al, byte ptr [ebp - 7]
        out    dx, al
        mov    byte ptr [ebp - 6], 1dh
        add    cx, 2
        mov    word ptr [ebp - 14h], cx
        mov    dx, word ptr [ebp - 14h]
        mov    al, byte ptr [ebp - 6]
        out    dx, al
        mov    byte ptr [ebp - 5], 15h
        mov    word ptr [ebp - 12h], cx
        mov    dx, word ptr [ebp - 12h]
        mov    al, byte ptr [ebp - 5]
        out    dx, al
        mov    byte ptr [ebp - 4], 1dh
        mov    word ptr [ebp - 10h], cx
        mov    dx, word ptr [ebp - 10h]
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        mov    byte ptr [ebp - 3], 15h
        mov    word ptr [ebp - 0eh], cx
        mov    dx, word ptr [ebp - 0eh]
        mov    al, byte ptr [ebp - 3]
        out    dx, al
        mov    esi, 3
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0B4C:
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        add    cx, 2
loc_2_0B57:
        mov    byte ptr [ebp - 2], 17h
        mov    word ptr [ebp - 0ch], cx
        mov    dx, word ptr [ebp - 0ch]
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        mov    byte ptr [ebp - 1], 15h
        mov    word ptr [ebp - 0ah], cx
        mov    dx, word ptr [ebp - 0ah]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        dec    esi
        jne    SHORT loc_2_0B57
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  090h
sub_2_0B80:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    al, byte ptr [esp + 8]
        lea    edx, [eax + eax*4]
        db  08Bh, 004h, 0D5h, 000h, 000h, 000h, 000h
        lea    ecx, [edx*8]
        db  0A3h, 000h, 000h, 000h, 000h
        db  08Bh, 081h, 004h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        db  08Bh, 081h, 008h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        db  08Bh, 081h, 00Ch, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        db  08Bh, 081h, 010h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        db  08Bh, 081h, 014h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        db  08Bh, 081h, 018h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        db  08Bh, 081h, 01Ch, 000h, 000h, 000h
        pop    ebp
        db  0A3h, 000h, 000h, 000h, 000h
        db  08Bh, 081h, 020h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        db  08Bh, 081h, 024h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0C10:
        push   dx
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        mov    al, 33h
        out    dx, al
        add    dx, 2
        mov    al, 34h
        out    dx, al
        sub    dx, 2
        nop
        in     al, dx
        cmp    al, 33h
        jne    SHORT loc_2_0C47
        mov    al, 0cch
        out    dx, al
        add    dx, 2
        mov    al, 35h
        out    dx, al
        sub    dx, 2
        nop
        in     al, dx
        cmp    al, 0cch
        jne    SHORT loc_2_0C47
        mov    ax, 0ffffh
        jmp    SHORT loc_2_0C4A
loc_2_0C47:
        xor    ax, ax
loc_2_0C4A:
        push   ax
        mov    ax, 15h
        add    dx, 2
        out    dx, al
        pop    ax
        pop    dx
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    bl, byte ptr [ebp + 8]
        cmp    bl, 1
        sbb    esi, esi
        xor    eax, eax
        and    esi, 2
        mov    al, bl
        inc    esi
        cmp    eax, esi
        jge    SHORT loc_2_0CB6
        mov    di, word ptr [ebp + 0ch]
loc_2_0C7E:
        push   ebx
        call   sub_2_06B0
        test   ax, ax
        je     SHORT loc_2_0CA7
        pushf
        cli
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0C91:
        popf
        push   edi
        call   sub_2_0320
        test   eax, eax
        je     SHORT loc_2_0CC2
        pushf
        cli
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0CA5:
        popf
loc_2_0CA7:
        inc    bl
        call   sub_2_06F0
        xor    eax, eax
        mov    al, bl
        cmp    eax, esi
        jl     SHORT loc_2_0C7E
loc_2_0CB6:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_2_0CC2:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_2_0CD0:
        push   ebp
        mov    eax, 9
        mov    ebp, esp
        sub    esp, 10h
        mov    word ptr [ebp - 10h], ax
        push   ebx
        mov    word ptr [ebp - 0eh], ax
        push   esi
        xor    ebx, ebx
        mov    ecx, dword ptr [ebp + 0ch]
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        push   ecx
        lea    eax, [ebp - 4]
        db  0C7h, 045h, 0F4h, 000h, 000h, 000h, 000h
        push   esi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0D00:
        db  089h, 035h, 046h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        db  0A3h, 004h, 000h, 000h, 000h
        db  088h, 01Dh, 04Ah, 000h, 000h, 000h
        db  089h, 01Dh, 03Eh, 000h, 000h, 000h
        db  0C6h, 005h, 008h, 000h, 000h, 000h, 003h
        db  088h, 01Dh, 009h, 000h, 000h, 000h
        db  089h, 01Dh, 00Ah, 000h, 000h, 000h
        db  0C7h, 005h, 00Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 012h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 016h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 01Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 01Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 0ch]
        db  0C7h, 005h, 022h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 026h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 02Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 02Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 032h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        push   36h
        db  089h, 015h, 036h, 000h, 000h, 000h
        db  068h, 008h, 000h, 000h, 000h
        lea    edx, [ebp - 8]
        db  068h, 000h, 000h, 000h, 000h
        db  089h, 00Dh, 03Ah, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0DC1:
        cmp    dword ptr [ebp - 8], ebx
        je     SHORT loc_2_0DDB
        push   ebx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0DD0:
        mov    eax, dword ptr [ebp - 8]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_0DDB:
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0DF0:
        push   ebp
        mov    eax, 0ah
        mov    ebp, esp
        sub    esp, 38h
        mov    word ptr [ebp - 38h], ax
        push   esi
        mov    word ptr [ebp - 36h], ax
        push   edi
        db  0C7h, 045h, 0CCh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0D4h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0DCh, 000h, 000h, 000h, 000h
        mov    eax, 0bh
        mov    word ptr [ebp - 30h], ax
        db  0C7h, 045h, 0E4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 2eh], ax
        mov    esi, 10h
        mov    eax, 0dh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 28h], ax
        push   ecx
        mov    word ptr [ebp - 26h], ax
        push   0
        mov    word ptr [ebp - 20h], ax
        push   2d3h
        mov    word ptr [ebp - 1eh], ax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0E5F:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    NEAR PTR loc_2_0FF1
        mov    edi, dword ptr [ebp - 8]
        xor    eax, eax
        mov    ecx, 0b4h
        rep stosd
        stosw
        stosb
        mov    edi, dword ptr [ebp + 0ch]
        lea    eax, [ebp - 10h]
        push   edi
        lea    ecx, [ebp - 4]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0E8A:
        cmp    dword ptr [ebp - 4], 0
        push   0
        je     SHORT loc_2_0EAD
        push   2d3h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0EA0:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_0EAD:
        lea    eax, [ebp - 38h]
        mov    ecx, dword ptr [ebp - 10h]
        push   eax
        lea    edx, [ebp - 0ch]
        push   ecx
        lea    eax, [ebp - 4]
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0EC2:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_2_0ED3
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4]
        db  0A2h, 000h, 000h, 000h, 000h
loc_2_0ED3:
        push   0
        lea    eax, [ebp - 30h]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 0ch]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0EEA:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_2_0EFB
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4]
        db  0A2h, 000h, 000h, 000h, 000h
loc_2_0EFB:
        push   0
        lea    eax, [ebp - 28h]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 0ch]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0F12:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_2_0F23
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4]
        db  0A2h, 000h, 000h, 000h, 000h
loc_2_0F23:
        push   0
        lea    eax, [ebp - 20h]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 0ch]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0F3A:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_2_0F46
        mov    eax, dword ptr [ebp - 0ch]
        mov    esi, dword ptr [eax + 4]
loc_2_0F46:
        mov    eax, dword ptr [ebp - 10h]
        lea    ecx, [ebp - 18h]
        push   eax
        lea    edx, [ebp - 14h]
        push   ecx
        lea    eax, [ebp - 4]
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0F5B:
        cmp    dword ptr [ebp - 18h], 6
        jne    SHORT loc_2_0F83
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_2_0F83
        mov    edx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 14h]
        add    edx, 104h
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    cl, byte ptr [eax + 4]
        mov    byte ptr [edx + 4], cl
        mov    al, byte ptr [eax + 5]
        mov    byte ptr [edx + 5], al
loc_2_0F83:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0F8C:
        mov    eax, dword ptr [ebp - 8]
        push   0
        push   0
        push   edi
        mov    dword ptr [eax + 38h], 0ch
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 10h]
        mov    byte ptr [eax + 40h], 0fh
        mov    eax, dword ptr [ebp - 8]
        push   ecx
        mov    dword ptr [eax + 41h], esi
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_1000
        test   eax, eax
        je     SHORT loc_2_0FE0
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   2d3h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0FC9:
        pushf
        cli
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0FD1:
        popf
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_0FE0:
        pushf
        cli
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0FE8:
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0FED:
        popf
        xor    eax, eax
loc_2_0FF1:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1000:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 38h], 0ch
        jbe    SHORT loc_2_1025
        mov    dword ptr [ebp - 4], 0c000009ah
loc_2_1019:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_1025:
        push   esi
        call   0
        test   al, al
        jne    SHORT loc_2_1055
        push   0
        mov    eax, dword ptr [esi + 4]
        push   0aa000004h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_103F:
        mov    dword ptr [ebp - 4], 0c0010006h
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_1055:
        xor    edx, edx
        lea    edi, [ebp - 2ch]
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        xor    eax, eax
        mov    ecx, 9
        mov    dword ptr [esi + 3ch], edx
        rep stosd
        stosw
        mov    edi, 1
        lea    eax, [ebp - 2ch]
        push   eax
        mov    dword ptr [ebp - 22h], edi
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 16h], edi
        push   ecx
        db  0A1h, 000h, 000h, 000h, 000h
        lea    ecx, [esi + 4]
        mov    dword ptr [ebp - 12h], edx
        mov    dword ptr [ebp - 0eh], 20h
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        push   esi
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_10A0:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    NEAR PTR loc_2_1019
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_2_10CC
        mov    eax, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   edi
        push   0aa00000ah
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_10C4:
        add    esp, 10h
        jmp    NEAR PTR loc_2_12DB
loc_2_10CC:
        db  0B8h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [esi + 3ch]
        mov    dword ptr [esi + 185h], eax
        mov    dword ptr [esi + 17dh], eax
        push   esi
        mov    dword ptr [esi + 181h], esi
        mov    dword ptr [esi + 179h], 0
        mov    dword ptr [esi + 0ffh], ecx
        call   sub_2_00E0
        test   al, al
        jne    SHORT loc_2_111F
        push   0
        mov    eax, dword ptr [esi + 4]
        push   0aa000004h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1110:
        mov    dword ptr [ebp - 4], 0c0010006h
        add    esp, 0ch
        jmp    NEAR PTR loc_2_12DB
loc_2_111F:
        mov    edx, dword ptr [esi + 110h]
        mov    edi, dword ptr [esi + 38h]
        mov    cl, dh
        mov    eax, edi
        shl    eax, 8
        mov    dword ptr [esi + 0edh], edx
        add    eax, edx
        lea    ebx, [ecx + edi]
        mov    byte ptr [esi + 0f9h], cl
        mov    dword ptr [esi + 0f1h], eax
        mov    eax, dword ptr [esi + 114h]
        mov    byte ptr [esi + 0fah], bl
        add    edx, eax
        shr    eax, 8
        mov    dword ptr [esi + 0f5h], edx
        mov    dword ptr [esi + 57h], 0ffffffffh
        add    al, cl
        xor    ecx, ecx
        mov    byte ptr [esi + 0fbh], al
        mov    byte ptr [esi + 15bh], cl
        mov    byte ptr [esi + 11dh], cl
        test   edi, edi
        je     SHORT loc_2_1190
        lea    eax, [esi + 5dh]
loc_2_1181:
        mov    dword ptr [eax], 0
        add    eax, 4
        inc    ecx
        cmp    dword ptr [esi + 38h], ecx
        ja     SHORT loc_2_1181
loc_2_1190:
        xor    ebx, ebx
        push   esi
        lea    edi, [esi + 14bh]
        mov    byte ptr [esi + 1b1h], bl
        mov    byte ptr [esi + 11bh], bl
        call   sub_2_0280
        push   edi
        lea    eax, [esi + 104h]
        push   esi
        mov    ecx, dword ptr [esi + 41h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_2_1540
        test   al, al
        jne    SHORT loc_2_11ED
        push   ebx
        mov    eax, dword ptr [esi + 4]
        push   0aa000002h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_11DE:
        mov    dword ptr [ebp - 4], 0c0000001h
        add    esp, 0ch
        jmp    NEAR PTR loc_2_12DB
loc_2_11ED:
        mov    byte ptr [esi + 15ch], 1bh
        db  0C7h, 046h, 020h, 000h, 000h, 000h, 000h
        db  0A1h, 03Eh, 000h, 000h, 000h
        push   esi
        mov    dword ptr [esi + 24h], eax
        db  089h, 035h, 03Eh, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_120F:
        test   al, al
        jne    SHORT loc_2_122F
        push   0
        mov    eax, dword ptr [esi + 4]
        push   0aa000004h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1223:
        mov    dword ptr [ebp - 4], 0c0010006h
        add    esp, 0ch
        jmp    SHORT loc_2_129B
loc_2_122F:
        movsx  eax, byte ptr [esi + 40h]
        push   1
        push   0
        push   eax
        mov    ecx, dword ptr [esi + 4]
        push   eax
        lea    edx, [esi + 8]
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 4]
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1255:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_2_1288
        push   1
        push   1
        push   7
        mov    eax, dword ptr [esi + 4]
        push   7
        lea    ecx, [esi + 2bbh]
        db  068h, 000h, 000h, 000h, 000h
        lea    edx, [ebp - 4]
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1282:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_12F0
loc_2_1288:
        push   0
        mov    eax, dword ptr [esi + 4]
        push   0aa000005h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1298:
        add    esp, 0ch
loc_2_129B:
        db  03Bh, 035h, 03Eh, 000h, 000h, 000h
        jne    SHORT loc_2_12AD
        mov    eax, dword ptr [esi + 24h]
        db  0A3h, 03Eh, 000h, 000h, 000h
        jmp    SHORT loc_2_12C9
loc_2_12AD:
        db  08Bh, 00Dh, 03Eh, 000h, 000h, 000h
        cmp    dword ptr [ecx + 24h], esi
        je     SHORT loc_2_12C0
loc_2_12B8:
        mov    ecx, dword ptr [ecx + 24h]
        cmp    dword ptr [ecx + 24h], esi
        jne    SHORT loc_2_12B8
loc_2_12C0:
        mov    eax, dword ptr [ecx + 24h]
        mov    edx, dword ptr [eax + 24h]
        mov    dword ptr [ecx + 24h], edx
loc_2_12C9:
        push   1
        push   0
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_2_1660
loc_2_12DB:
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_12E4:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_12F0:
        xor    ebx, ebx
        db  0B8h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 189h], ebx
        mov    dword ptr [esi + 18dh], eax
        db  0B9h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 191h], esi
        mov    dword ptr [esi + 195h], ebx
        mov    dword ptr [esi + 199h], eax
        lea    eax, [esi + 1a5h]
        mov    dword ptr [esi + 19dh], esi
        push   7d0h
        mov    dword ptr [esi + 1a1h], ecx
        push   eax
        mov    dword ptr [eax], ebx
        mov    dword ptr [esi + 1a9h], ecx
        mov    dword ptr [esi + 1adh], esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1345:
        xor    eax, eax
        pop    edi
        mov    byte ptr [esi + 1b2h], bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1360:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_136E:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 18h]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], 0ffffffffh
        lea    eax, [eax + edx*4]
loc_2_1389:
        test   edx, edx
        je     SHORT loc_2_1398
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    SHORT loc_2_1389
        mov    dword ptr [ecx], edx
loc_2_1398:
        cmp    dword ptr [ecx], -1
        jne    SHORT loc_2_13B0
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_13A2:
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_2_13B0:
        db  0A1h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 4]
        push   eax
        push   0
        push   75h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_13C3:
        test   eax, eax
        je     SHORT loc_2_13F0
        push   0
        mov    esi, dword ptr [ebp + 20h]
        push   0aa000002h
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_13DA:
        add    esp, 0ch
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_13E2:
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_2_13F0:
        mov    esi, dword ptr [ebp + 20h]
        inc    dword ptr [esi + 30h]
        cmp    dword ptr [esi + 28h], 0
        jne    SHORT loc_2_140C
        cmp    dword ptr [esi + 2ch], 0
        jne    SHORT loc_2_140C
        push   0
        push   0dh
        db  0FFh, 015h, 000h, 000h, 000h, 000h
loc_2_140C:
        mov    eax, dword ptr [esi + 28h]
        mov    ecx, dword ptr [ebp - 4]
        xor    ebx, ebx
        mov    dword ptr [ecx + 0ch], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 28h], ecx
        cmp    byte ptr [esi + 13eh], bl
        jne    NEAR PTR loc_2_14F5
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 1ch]
        add    eax, 14h
        mov    ecx, dword ptr [esi + 14bh]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_2_16B0
        test   al, al
        je     NEAR PTR loc_2_14F5
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], esi
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [esi + 20h]
        mov    dword ptr [eax + 10h], edx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 28h]
        mov    dword ptr [eax], edi
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], edx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 71h], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 20h], bl
        mov    eax, 0eeh
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 18h], eax
        mov    dword ptr [esi + 15dh], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 1ch], 1
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edx], ecx
        mov    eax, dword ptr [esi + 30h]
        dec    eax
        mov    dword ptr [esi + 30h], eax
        jne    SHORT loc_2_14C6
        cmp    byte ptr [esi + 13eh], bl
        jne    SHORT loc_2_14B6
        cmp    dword ptr [esi + 16dh], ebx
        jne    SHORT loc_2_14B6
        cmp    dword ptr [esi + 2ch], ebx
        je     SHORT loc_2_14C6
loc_2_14B6:
        push   1
        add    esi, 179h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_14C4:
        jmp    SHORT loc_2_14E5
loc_2_14C6:
        cmp    dword ptr [esi + 47h], 0
        je     SHORT loc_2_14E5
        mov    eax, dword ptr [esi + 4fh]
        cmp    dword ptr [esi + eax*4 + 5dh], 0
        jne    SHORT loc_2_14E5
        cmp    byte ptr [esi + 11dh], 0
        jne    SHORT loc_2_14E5
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_14E5:
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_14EA:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_2_14F5:
        push   0
        dec    dword ptr [esi + 30h]
        mov    eax, dword ptr [ebp - 4]
        push   75h
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 28h], ecx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_150E:
        push   0
        mov    ecx, dword ptr [esi + 4]
        push   0aa000002h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_151E:
        add    esp, 0ch
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1526:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1540:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  08Bh, 01Dh, 094h, 043h, 000h, 000h
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   32h
        push   eax
        db  0E8h, 069h, 027h, 0FEh, 0FFh
        mov    ecx, dword ptr [ebp + 20h]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     SHORT loc_2_1574
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_2_1574:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0ch
        rep stosd
        stosw
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, eax
        jne    SHORT loc_2_158E
        mov    esi, 2
loc_2_158E:
        push   ebx
        push   0
        lea    eax, [esi + esi*2]
        shl    eax, 2
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 025h, 027h, 0FEh, 0FFh
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 4], ecx
        or     eax, eax
        je     SHORT loc_2_15C2
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_1660
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_2_15C2:
        push   ebx
        push   0
        lea    eax, [esi*8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        db  0E8h, 0EDh, 026h, 0FEh, 0FFh
        or     eax, eax
        mov    eax, dword ptr [ebp - 4]
        je     SHORT loc_2_15F1
        push   eax
        call   sub_2_1660
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_2_15F1:
        mov    dword ptr [eax + 28h], 0ffffffffh
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [eax + 2ch], edx
        mov    dl, byte ptr [ecx + 4]
        mov    byte ptr [eax + 30h], dl
        add    eax, 2ch
        mov    cl, byte ptr [ecx + 5]
        mov    byte ptr [eax + 5], cl
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 14h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 18h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        mov    dword ptr [eax + 24h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 20h], esi
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_1660:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_2_1680
        mov    ecx, dword ptr [esi + 20h]
        push   0
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 2
        push   ecx
        push   eax
        db  0E8h, 04Ch, 026h, 0FEh, 0FFh
loc_2_1680:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     SHORT loc_2_1696
        push   0
        mov    ecx, dword ptr [esi + 20h]
        shl    ecx, 3
        push   ecx
        push   eax
        db  0E8h, 036h, 026h, 0FEh, 0FFh
loc_2_1696:
        push   0
        push   32h
        push   esi
        db  0E8h, 02Ch, 026h, 0FEh, 0FFh
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_16B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 28h], 0
        jne    SHORT loc_2_16C9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_16C9:
        db  0A1h, 094h, 043h, 000h, 000h
        push   eax
        push   0
        push   1ah
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 0EAh, 025h, 0FEh, 0FFh
        or     eax, eax
        je     SHORT loc_2_16E9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_16E9:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 28h]
        mov    edi, edi
loc_2_16F0:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    SHORT loc_2_1703
        inc    ecx
        cmp    ecx, 20h
        jb     SHORT loc_2_16F0
        jmp    SHORT loc_2_1712
loc_2_1703:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 28h], eax
loc_2_1712:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 10h]
        mov    dword ptr [eax + 10h], ecx
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        je     SHORT loc_2_1728
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], ecx
loc_2_1728:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 10h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [ebp - 8]
        mov    byte ptr [eax + 19h], cl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ch], 1
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        mov    byte ptr [eax + 18h], 0
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    al, 1
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Bh, 0FFh
sub_2_1780:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_1B20
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_2_17A9
        cmp    ebx, 103h
        jne    SHORT loc_2_17C0
loc_2_17A9:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   edi
        push   esi
        call   sub_2_1840
        or     eax, eax
        je     SHORT loc_2_17C0
        mov    ebx, eax
loc_2_17C0:
        or     ebx, ebx
        je     SHORT loc_2_17CC
        cmp    ebx, 103h
        jne    SHORT loc_2_182B
loc_2_17CC:
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_2_1826
        mov    eax, dword ptr [edi + 10h]
        or     eax, eax
        je     SHORT loc_2_17E2
        mov    ecx, dword ptr [edi + 14h]
        mov    dword ptr [eax + 14h], ecx
loc_2_17E2:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_2_17F1
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_2_17F7
loc_2_17F1:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_2_17F7:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_2_1806
        mov    eax, dword ptr [edi + 4]
        push   eax
        db  0E8h, 022h, 024h, 0FEh, 0FFh
loc_2_1806:
        mov    eax, 1
        mov    cl, byte ptr [edi + 19h]
        shl    eax, cl
        or     dword ptr [esi + 28h], eax
        push   0
        push   1ah
        push   edi
        db  0E8h, 0AFh, 024h, 0FEh, 0FFh
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_2_1826:
        mov    ebx, 0c001000eh
loc_2_182B:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1840:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [esi + 20h]
        lea    ecx, [eax + eax*2]
        add    ecx, ecx
        add    ecx, dword ptr [esi + 4]
        mov    dword ptr [ebp - 20h], ecx
        mov    ecx, dword ptr [esi + 8]
        lea    eax, [ecx + eax*4]
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [esi + 24h]
        shl    eax, 2
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        db  0E8h, 051h, 025h, 0FEh, 0FFh
        mov    eax, dword ptr [esi + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        sub    edi, edi
        push   eax
        db  0E8h, 039h, 025h, 0FEh, 0FFh
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [ebp - 0ch], eax
        cmp    eax, edi
        je     SHORT loc_2_18C4
        sub    eax, eax
        nop
loc_2_18A0:
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 1ch], ecx
        mov    edx, dword ptr [ebp + 0ch]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 19h]
        shl    ebx, cl
        not    ebx
        mov    ecx, dword ptr [ebp - 1ch]
        and    dword ptr [eax + ecx], ebx
        add    eax, 4
        inc    edi
        cmp    dword ptr [esi + 24h], edi
        ja     SHORT loc_2_18A0
loc_2_18C4:
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [esi + 24h], 0
        jbe    SHORT loc_2_192F
loc_2_18D1:
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx*4], 0
        jne    SHORT loc_2_1924
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        mov    eax, dword ptr [esi + 24h]
        sub    eax, dword ptr [ebp - 4]
        add    ecx, dword ptr [esi + 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        sub    eax, 6
        push   eax
        lea    eax, [ecx + 6]
        push   eax
        push   ecx
        db  0E8h, 0CEh, 024h, 0FEh, 0FFh
        mov    ecx, dword ptr [ebp - 4]
        shl    ecx, 2
        add    ecx, dword ptr [esi + 8]
        mov    eax, dword ptr [esi + 24h]
        sub    eax, dword ptr [ebp - 4]
        lea    eax, [eax*4 - 4]
        push   eax
        lea    eax, [ecx + 4]
        push   eax
        push   ecx
        db  0E8h, 0ADh, 024h, 0FEh, 0FFh
        dec    dword ptr [esi + 24h]
        jmp    SHORT loc_2_1927
loc_2_1924:
        inc    dword ptr [ebp - 4]
loc_2_1927:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 24h], eax
        ja     SHORT loc_2_18D1
loc_2_192F:
        mov    dword ptr [ebp - 10h], 0
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_2_1A1A
        mov    edi, dword ptr [ebp + 18h]
loc_2_1943:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        call   sub_2_1D00
        or     al, al
        je     SHORT loc_2_197A
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 18h], ecx
        mov    edx, dword ptr [ebp + 0ch]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 19h]
        shl    ebx, cl
        mov    ecx, dword ptr [ebp - 18h]
        or     dword ptr [ecx + eax*4], ebx
        jmp    NEAR PTR loc_2_1A08
loc_2_197A:
        mov    ecx, dword ptr [esi + 24h]
        cmp    dword ptr [esi + 20h], ecx
        jbe    NEAR PTR loc_2_1A54
        mov    eax, dword ptr [ebp - 4]
        sub    ecx, dword ptr [ebp - 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        add    eax, dword ptr [esi + 4]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        push   ecx
        push   eax
        add    eax, 6
        push   eax
        db  0E8h, 028h, 024h, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        add    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [eax], ecx
        mov    cl, byte ptr [edi + 4]
        mov    byte ptr [eax + 4], cl
        mov    cl, byte ptr [edi + 5]
        mov    byte ptr [eax + 5], cl
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 2
        add    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [esi + 24h]
        sub    ecx, dword ptr [ebp - 4]
        shl    ecx, 2
        push   ecx
        push   eax
        add    eax, 4
        push   eax
        db  0E8h, 0F0h, 023h, 0FEh, 0FFh
        mov    edx, dword ptr [esi + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + eax*4], 0
        mov    ebx, 1
        mov    edx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 14h], edx
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [edx + 19h]
        mov    edx, dword ptr [ebp - 14h]
        shl    ebx, cl
        or     dword ptr [edx + eax*4], ebx
        inc    dword ptr [esi + 24h]
loc_2_1A08:
        add    edi, 6
        inc    dword ptr [ebp - 10h]
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp + 14h], eax
        ja     NEAR PTR loc_2_1943
loc_2_1A1A:
        mov    eax, dword ptr [esi + 24h]
        sub    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], eax
        sub    ecx, ecx
        cmp    dword ptr [ebp - 0ch], ecx
        je     SHORT loc_2_1AA5
        mov    edi, dword ptr [ebp - 20h]
        sub    edx, edx
        nop
loc_2_1A30:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_1AAB
        mov    eax, dword ptr [esi + 4]
        add    eax, edx
        mov    ebx, dword ptr [edi + 2]
        cmp    dword ptr [eax + 2], ebx
        jne    SHORT loc_2_1A92
        mov    bx, word ptr [edi]
        cmp    word ptr [eax], bx
        jne    SHORT loc_2_1A92
        mov    dword ptr [ebp - 8], 0
        jmp    SHORT loc_2_1A99
loc_2_1A54:
        mov    eax, dword ptr [ebp - 0ch]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 062h, 023h, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 0ch]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        db  0E8h, 04Eh, 023h, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 24h], eax
        mov    eax, 0c0010009h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_2_1A92:
        mov    dword ptr [ebp - 8], 1
loc_2_1A99:
        add    edi, 6
        add    edx, 6
        inc    ecx
        cmp    dword ptr [ebp - 0ch], ecx
        ja     SHORT loc_2_1A30
loc_2_1AA5:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_2_1B10
loc_2_1AAB:
        mov    al, byte ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [edx]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   dword ptr [esi + 14h]
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_2_1B12
        cmp    edi, 103h
        je     SHORT loc_2_1B12
        mov    eax, dword ptr [ebp - 0ch]
        lea    eax, [eax + eax*2]
        lea    ebx, [eax*2]
        push   ebx
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 0D7h, 022h, 0FEh, 0FFh
        push   ebx
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 0C9h, 022h, 0FEh, 0FFh
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Bh, 0FFh
loc_2_1B10:
        sub    edi, edi
loc_2_1B12:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 049h, 000h
sub_2_1B20:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [esi + 0ch]
        mov    ecx, dword ptr [edi + 8]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        mov    dword ptr [esi + 0ch], 0
        je     SHORT loc_2_1B5D
        lea    esp, [esp]
loc_2_1B50:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 0ch], ecx
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_2_1B50
loc_2_1B5D:
        mov    ecx, dword ptr [esi + 0ch]
        cmp    ecx, ebx
        je     SHORT loc_2_1B91
        mov    al, byte ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        push   ecx
        push   ebx
        call   dword ptr [esi + 18h]
        or     eax, eax
        je     SHORT loc_2_1B93
        cmp    eax, 103h
        je     SHORT loc_2_1B93
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 0ch], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_1B91:
        sub    eax, eax
loc_2_1B93:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h
sub_2_1BA0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        sub    eax, eax
        mov    cl, byte ptr [ecx + 19h]
        mov    edx, dword ptr [ebp + 8]
        and    ecx, 0ffh
        mov    edi, dword ptr [edx + 24h]
        cmp    edi, eax
        je     SHORT loc_2_1BD2
        mov    esi, dword ptr [edx + 8]
        mov    edx, 1
        shl    edx, cl
loc_2_1BC7:
        test   dword ptr [esi], edx
        je     SHORT loc_2_1BCC
        inc    eax
loc_2_1BCC:
        add    esi, 4
        dec    edi
        jne    SHORT loc_2_1BC7
loc_2_1BD2:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_1BE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ebp + 10h]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 19h]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebx + 24h], 0
        jbe    SHORT loc_2_1C5C
        mov    edi, dword ptr [ebp + 1ch]
        mov    dword ptr [ebp - 8], 0
        sub    esi, esi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 10h], eax
        mov    edi, edi
loc_2_1C20:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 10h]
        test   dword ptr [ecx + esi], eax
        je     SHORT loc_2_1C4A
        cmp    dword ptr [ebp + 14h], 6
        jb     SHORT loc_2_1C76
        sub    dword ptr [ebp + 14h], 6
        push   6
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        db  0E8h, 088h, 021h, 0FEh, 0FFh
        add    edi, 6
        inc    dword ptr [ebp - 0ch]
loc_2_1C4A:
        add    dword ptr [ebp - 8], 6
        add    esi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebx + 24h], eax
        ja     SHORT loc_2_1C20
loc_2_1C5C:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [eax], ecx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_2_1C76:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    dword ptr [eax], 0c0000001h
        mov    eax, dword ptr [ebp + 18h]
        pop    esi
        mov    dword ptr [eax], 0
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1CA0:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 24h]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        cmp    ecx, dword ptr [ebp + 10h]
        jbe    SHORT loc_2_1CD0
        mov    dword ptr [eax], 0c0000001h
        mov    eax, dword ptr [ebp + 14h]
        pop    ebp
        mov    dword ptr [eax], 0
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1CD0:
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [edx + 24h]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [edx + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [edx + 4]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        db  0E8h, 0D8h, 020h, 0FEh, 0FFh
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_1D00:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    ecx, ecx
        mov    esi, dword ptr [ebp + 8]
        mov    edx, esi
        shr    edx, 1
        cmp    esi, ecx
        je     NEAR PTR loc_2_1D99
        dec    esi
        cmp    esi, edx
        jb     SHORT loc_2_1D99
        nop
loc_2_1D20:
        cmp    edx, ecx
        jb     SHORT loc_2_1D99
        mov    eax, dword ptr [ebp + 0ch]
        lea    edi, [edx + edx*2]
        lea    edi, [eax + edi*2]
        mov    eax, dword ptr [ebp + 10h]
        mov    ebx, dword ptr [eax + 2]
        mov    eax, dword ptr [edi + 2]
        cmp    ebx, eax
        jae    SHORT loc_2_1D43
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_2_1D77
loc_2_1D43:
        jbe    SHORT loc_2_1D50
        mov    dword ptr [ebp - 4], 0ffffffffh
        jmp    SHORT loc_2_1D77
        db  08Bh, 0FFh
loc_2_1D50:
        mov    eax, dword ptr [ebp + 10h]
        mov    bx, word ptr [eax]
        mov    ax, word ptr [edi]
        cmp    bx, ax
        jae    SHORT loc_2_1D67
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_2_1D77
loc_2_1D67:
        mov    dword ptr [ebp - 4], 0ffffffffh
        ja     SHORT loc_2_1D77
        mov    dword ptr [ebp - 4], 0
loc_2_1D77:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_1DA9
        jle    SHORT loc_2_1D88
        or     edx, edx
        je     SHORT loc_2_1D99
        lea    esi, [edx - 1]
        jmp    SHORT loc_2_1D8B
loc_2_1D88:
        lea    ecx, [edx + 1]
loc_2_1D8B:
        mov    eax, esi
        sub    eax, ecx
        inc    eax
        shr    eax, 1
        lea    edx, [ecx + eax]
        cmp    esi, edx
        jae    SHORT loc_2_1D20
loc_2_1D99:
        mov    eax, dword ptr [ebp + 14h]
        pop    edi
        mov    dword ptr [eax], edx
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_1DA9:
        mov    eax, dword ptr [ebp + 14h]
        pop    edi
        mov    dword ptr [eax], edx
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
_PTEXT ends
        end
