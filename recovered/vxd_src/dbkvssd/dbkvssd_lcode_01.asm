; dbkvssd_lcode_01.asm -- DBKVSSD object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

DBKVSSD_Control:
        cmp    eax, 22h
        jne    SHORT loc_1_000E
        push   edx
        push   ebx
        call   sub_1_1A00
        stc
        ret
loc_1_000E:
        clc
        ret
sub_1_0010:
        push   ebp
        mov    ebp, esp
        pushad
        mov    esi, dword ptr [ebp + 8]
        pushfd
        inc    byte ptr [esi + 6]
        push   ebx
        push   ebp
        mov    ebp, esp
        push   esi
        push   eax
        push   ecx
        push   edx
        push   edi
        lea    esp, [ebp - 24h]
        mov    byte ptr [ebp - 24h], 0
        cld
        mov    dword ptr [ebp - 1ch], 0
        mov    dx, word ptr [esi + 4]
        db  066h, 089h, 015h, 038h, 032h, 000h, 000h
        push   esi
        call   sub_1_0B37
        movzx  eax, ax
        db  0A3h, 03Ah, 032h, 000h, 000h
        pop    ebx
        call   sub_1_113E
        pushf
        call   sub_1_0955
        mov    esi, dword ptr [ebp - 4]
        dec    byte ptr [esi + 6]
        popf
        lea    esp, [ebp - 14h]
        pop    edi
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        pop    ebp
        pop    ebx
        popfd
        popad
        db  0A1h, 03Ah, 032h, 000h, 000h
        leave
        ret
sub_1_0074:
        push   eax
        push   ebx
        push   ecx
        push   edx
        mov    eax, 0d6d8h
        movzx  ecx, cx
        mul    ecx
        push   0
        push   eax
        db  0CDh, 020h, 015h, 000h, 033h, 000h
        pop    ecx
        pop    ecx
        pop    edx
        pop    ebx
        pop    ecx
        pop    eax
        ret
sub_1_0092:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        sub    esp, 8
        mov    ebx, esp
        mov    word ptr [ebx], 3
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0ah], 44425353h
        mov    dword ptr [ebx + 2], esi
        db  08Dh, 035h, 07Ch, 010h, 000h, 000h
        xor    edx, edx
        mov    ecx, 6
        mov    eax, 32h
        db  0CDh, 020h, 001h, 000h, 07Ch, 009h
        add    esp, 8
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
        db  000h, 000h, 000h, 000h
loc_1_00D3:
        push   eax
        db  0A1h, 0CFh, 000h, 000h, 000h
        cmp    eax, 8
        jb     SHORT loc_1_00E7
        sub    eax, 8
        or     al, 60h
        out    0a0h, al
        mov    al, 2
loc_1_00E7:
        or     al, 60h
        out    20h, al
        mov    al, 0ffh
        out    0a1h, al
        out    21h, al
        pop    eax
        iretd
sub_1_00F3:
        push   ebx
        push   esi
        sub    esp, 8
        sidt   [esp + 2]
        pop    ebx
        pop    ebx
        db  0A3h, 0CFh, 000h, 000h, 000h
        shl    eax, 3
        add    ebx, eax
        add    ebx, 280h
        db  0BEh, 0D3h, 000h, 000h, 000h
        xchg   word ptr [ebx], si
        ror    esi, 10h
        xchg   word ptr [ebx + 6], si
        ror    esi, 10h
        sti
        nop
        nop
        nop
        cli
        mov    word ptr [ebx], si
        ror    esi, 10h
        mov    word ptr [ebx + 6], si
        pop    esi
        pop    ebx
        ret
sub_1_0133:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edx
        mov    bx, word ptr [ebp + 8]
        or     bx, bx
        je     SHORT loc_1_0189
        mov    ecx, 1
loc_1_0147:
        test   cl, bl
        je     SHORT loc_1_015D
        mov    al, cl
        not    al
        out    21h, al
        bsf    eax, ecx
        call   sub_1_00F3
        mov    al, 0ffh
        out    21h, al
loc_1_015D:
        shl    cl, 1
        jne    SHORT loc_1_0147
        mov    bl, bh
        mov    cl, 1
loc_1_0165:
        test   cl, bl
        je     SHORT loc_1_0185
        mov    al, 0fbh
        out    21h, al
        mov    al, cl
        not    al
        out    0a1h, al
        xchg   ch, cl
        bsf    eax, ecx
        call   sub_1_00F3
        xchg   ch, cl
        mov    al, 0ffh
        out    0a1h, al
        out    21h, al
loc_1_0185:
        shl    cl, 1
        jne    SHORT loc_1_0165
loc_1_0189:
        pop    edx
        pop    ecx
        pop    ebx
        leave
        ret
        db  000h, 000h
sub_1_0190:
        mov    ebx, dword ptr [ebp - 4]
        mov    dx, word ptr [ebx + 4]
        mov    al, 80h
        add    dx, 6
        out    dx, al
        xor    ax, ax
        out    dx, al
        add    dx, -6
        mov    al, 0a0h
        call   sub_1_0E4E
        mov    ax, word ptr [ebx + 4ch]
        and    ax, 0fff0h
        add    dx, 0eh
        out    dx, al
        out    dx, ax
        add    dx, -0eh
        xor    ax, ax
        mov    al, byte ptr [ebx + 2ch]
        call   sub_1_0F2C
        call   sub_1_042C
        mov    ax, 1000h
        call   sub_1_098C
        xor    ax, ax
        call   sub_1_098C
        xor    ax, ax
        call   sub_1_082C
        mov    al, 60h
        call   sub_1_0E4E
        add    dx, 0eh
        mov    ax, 0
        out    dx, ax
        mov    ax, word ptr [ebx + 52h]
        mov    ah, al
        mov    al, 1
        test   ah, 10h
        jne    SHORT loc_1_0209
        or     al, 3
loc_1_0209:
        dec    dx
        out    dx, al
        ret
sub_1_020D:
        call   sub_1_0B2D
        push   dx
        add    dx, 4
        in     ax, dx
        and    ax, 7000h
        mov    cl, 0ch
        shr    ax, cl
        mov    cx, ax
        pop    dx
        ret
sub_1_0229:
        call   sub_1_0190
        mov    ebx, dword ptr [ebp - 4]
        mov    dx, word ptr [ebx + 4]
        add    dx, 8
        mov    al, 0e0h
        out    dx, al
        add    dx, 6
        mov    ax, 8000h
        out    dx, ax
        add    dx, -6
        mov    al, 0c0h
        out    dx, al
        add    dx, 6
        in     ax, dx
        push   ax
        add    dx, -6
        mov    al, 0e0h
        out    dx, al
        xor    ax, ax
        add    dx, 6
        out    dx, ax
        add    dx, -0eh
        pop    ax
        and    ax, 7f00h
        shr    ax, 8
        cmp    ax, word ptr [ebx + 8]
        jne    SHORT loc_1_0287
        cmp    al, 4
        je     SHORT loc_1_0290
        cmp    al, 8
        je     SHORT loc_1_0290
        cmp    al, 14h
        je     SHORT loc_1_0290
        jmp    SHORT loc_1_02E3
loc_1_0287:
        mov    ax, 8080h
        jmp    NEAR PTR loc_1_0376
loc_1_0290:
        inc    dx
        inc    dx
        mov    ax, 300h
        out    dx, ax
        inc    dx
        inc    dx
        in     ax, dx
        and    ax, 7000h
        or     ax, 800h
        out    dx, ax
        add    dx, -4
        in     ax, dx
        test   ax, 4000h
        je     SHORT loc_1_02B8
        jmp    SHORT loc_1_0287
loc_1_02B8:
        lea    esi, [ebx + 38h]
        test   ax, 100h
        je     SHORT loc_1_02C5
        mov    byte ptr [esi + 0fh], 0fh
loc_1_02C5:
        test   ax, 80h
        je     SHORT loc_1_02CF
        mov    byte ptr [esi + 0ch], 0ch
loc_1_02CF:
        test   ax, 40h
        je     SHORT loc_1_02D9
        mov    byte ptr [esi + 9], 9
loc_1_02D9:
        test   ax, 20h
        je     SHORT loc_1_02E3
        mov    byte ptr [esi + 6], 6
loc_1_02E3:
        xor    ax, ax
        inc    dx
        inc    dx
        out    dx, ax
        inc    dx
        inc    dx
        push   bx
        mov    bx, ax
        in     ax, dx
        and    ax, 7000h
        or     ax, bx
        pop    bx
        out    dx, ax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 48h]
        add    dx, 5
        out    dx, al
        or     al, 20h
        out    dx, al
        mov    cx, 2
        call   sub_1_0074
        dec    dx
        in     al, dx
        and    al, 1fh
        or     al, 0e0h
        out    dx, al
        xor    ax, ax
        add    dx, 6
        out    dx, ax
        mov    al, 18h
        add    dx, -8
        out    dx, al
        xor    ax, ax
        mov    cx, ax
        add    dx, -4
        out    dx, ax
        inc    dx
        inc    dx
        push   bx
        mov    bx, ax
        in     ax, dx
        and    ax, 7000h
        or     ax, bx
        pop    bx
        out    dx, ax
        dec    dx
        dec    dx
loc_1_0357:
        dec    dx
        dec    dx
        out    dx, al
        inc    cx
        inc    dx
        inc    dx
        in     al, dx
        cmp    al, cl
        jne    SHORT loc_1_036F
        cmp    cx, 0ah
        jb     SHORT loc_1_0357
        jmp    SHORT loc_1_0373
loc_1_036F:
        mov    ah, 3
        jmp    SHORT loc_1_0376
loc_1_0373:
        xor    ax, ax
loc_1_0376:
        push   ax
        mov    ebx, dword ptr [ebp - 4]
        mov    dx, word ptr [ebx + 4]
        call   sub_1_0190
        pop    ax
        ret
sub_1_0387:
        push   cx
        mov    byte ptr [ebx + 11h], 0
        mov    byte ptr [ebx + 0dh], 1
        mov    byte ptr [ebx + 0ch], 40h
        add    byte ptr [ebx + 0ch], 0
        mov    cl, byte ptr [ebx + 0eh]
        mov    ax, 8
        mul    cl
        mov    word ptr [ebx + 4], ax
        push   esi
        mov    esi, dword ptr [ebp - 4]
        mov    ax, word ptr [esi + 52h]
        pop    esi
        mov    word ptr [ebx + 6], ax
        pop    cx
        ret
sub_1_03B6:
        push   esi
        push   ecx
        xor    ecx, ecx
        mov    cl, byte ptr [ebx + 1ah]
        mov    esi, dword ptr [ebp - 4]
        cmp    cl, byte ptr [esi + 2fh]
        jb     SHORT loc_1_0400
        sub    cl, byte ptr [esi + 2fh]
        mov    ax, 8
        mul    cl
        add    ax, 100h
        mov    word ptr [ebx + 10h], ax
        mov    ax, word ptr [esi + 50h]
        and    ax, 1fh
        mov    byte ptr [ebx + 1bh], 32h
        mov    byte ptr [ebx + 1ch], 6
        mov    word ptr [ebx + 0ah], ax
        mov    word ptr [ebx], 1
        db  0B8h, 072h, 032h, 000h, 000h
        add    eax, 2
        mov    dword ptr [ebx + 12h], eax
        mov    byte ptr [ebx + 6], 4
        jmp    SHORT loc_1_0429
loc_1_0400:
        mov    ax, 4
        mul    cl
        add    ax, 200h
        mov    word ptr [ebx + 0eh], ax
        mov    ax, word ptr [esi + 50h]
        and    ax, 1fh
        mov    word ptr [ebx + 0ah], ax
        db  0B8h, 06Ah, 032h, 000h, 000h
        add    eax, 2
        mov    dword ptr [ebx + 12h], eax
        mov    byte ptr [ebx + 6], 5
loc_1_0429:
        pop    ecx
        pop    esi
        ret
sub_1_042C:
        push   ebx
        push   edx
        push   ecx
        push   esi
        mov    esi, dword ptr [ebp - 4]
        mov    dx, word ptr [esi + 4]
        mov    ax, 0
        call   sub_1_082C
        mov    bx, 78h
        mov    ax, word ptr [esi + 52h]
        out    dx, ax
        xchg   bx, ax
        out    dx, ax
        xchg   bx, ax
        out    dx, ax
        out    dx, ax
        out    dx, ax
        xchg   bx, ax
        out    dx, ax
        inc    dx
        inc    dx
        mov    ax, 100h
        out    dx, ax
        mov    ecx, 8
        dec    dx
        dec    dx
        mov    bx, word ptr [esi + 50h]
        xor    ax, ax
loc_1_0474:
        out    dx, ax
        out    dx, ax
        out    dx, ax
        xchg   bx, ax
        out    dx, ax
        xchg   bx, ax
        loop   loc_1_0474
        inc    dx
        inc    dx
        mov    ax, 200h
        out    dx, ax
        mov    ecx, 4
        shl    cx, 1
        dec    dx
        dec    dx
        xor    ax, ax
loc_1_049B:
        out    dx, ax
        loop   loc_1_049B
        pop    esi
        pop    ecx
        pop    edx
        pop    ebx
        ret
loc_1_04A4:
        push   eax
        push   ebx
        push   ecx
        push   edx
        push   esi
        mov    esi, dword ptr [ebp - 4]
        mov    ebx, dword ptr [ebp - 18h]
        mov    dx, word ptr [ebx + 2]
        mov    cx, word ptr [ebx]
        or     al, al
        jne    SHORT loc_1_04C7
        xor    cx, cx
        mov    dx, cx
        jmp    SHORT loc_1_053C
        db  0E9h, 0A8h, 000h, 000h, 000h
loc_1_04C7:
        shr    cx, 1
        or     cx, dx
        je     SHORT loc_1_053C
        xor    dx, dx
        jcxz   loc_1_04E4
        mov    dx, 840h
        test   byte ptr [ebx + 6], 4
        je     SHORT loc_1_04E4
        and    dx, 0f7bfh
loc_1_04E4:
        push   eax
        push   ebx
        push   ecx
        mov    ecx, edx
        mov    ebx, dword ptr [ebp - 1ch]
        mov    dx, word ptr [esi + 4]
        movzx  eax, word ptr [ebx + 4]
        add    eax, 2
        call   sub_1_082C
        movzx  eax, word ptr [ebx + 8]
        and    eax, 0fffffff9h
        test   ecx, 800h
        jne    SHORT loc_1_050E
        or     eax, 2
loc_1_050E:
        mov    word ptr [ebx + 8], ax
        out    dx, ax
        mov    edx, ecx
        pop    ecx
        pop    ebx
        pop    eax
        test   byte ptr [ebx + 6], 2
        je     SHORT loc_1_0524
        or     dx, 8000h
loc_1_0524:
        cmp    word ptr [ebx], 1
        jne    SHORT loc_1_052F
        or     dx, 200h
loc_1_052F:
        test   word ptr [ebx + 18h], 1
        je     SHORT loc_1_053C
        or     dx, 1000h
loc_1_053C:
        mov    word ptr [ebx + 8], cx
        mov    ax, dx
        mov    dx, word ptr [esi + 4]
        mov    si, bx
        mov    bx, ax
        jcxz   loc_1_0558
        call   sub_1_0576
        or     bx, ax
loc_1_0558:
        mov    word ptr [esi + 0ah], bx
        mov    ax, word ptr [esi + 0eh]
        call   sub_1_082C
        mov    ax, cx
        out    dx, ax
        mov    ax, bx
        out    dx, ax
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        clc
        ret
sub_1_0576:
        push   ebx
        mov    ebx, dword ptr [ebp - 4]
        movzx  ebx, word ptr [ebx + 54h]
        mov    eax, 4
        sub    eax, ebx
        pop    ebx
        ret
sub_1_0587:
        push   eax
        push   edi
        mov    edi, dword ptr [ebp - 4]
        mov    dx, word ptr [edi + 4]
        call   sub_1_0801
        cmp    byte ptr [ebp - 24h], 0
        jne    SHORT loc_1_05AA
        mov    edi, dword ptr [ebp - 1ch]
        mov    al, byte ptr [edi + 0fh]
        add    dx, 6
        out    dx, al
        add    dx, -6
loc_1_05AA:
        pop    edi
        pop    eax
        ret
sub_1_05AD:
        push   eax
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp - 4]
        mov    ebx, dword ptr [ebp - 18h]
        mov    dx, word ptr [ebx + 2]
        mov    cx, word ptr [ebx]
        or     al, al
        jne    SHORT loc_1_05D1
        xor    cx, cx
        mov    dx, cx
        jmp    SHORT loc_1_0610
        db  0E9h, 031h, 001h, 000h, 000h
loc_1_05D1:
        xor    ax, ax
        shr    cx, 1
        rcr    ax, 1
        shr    ax, 1
        or     cx, ax
        or     cx, dx
        xor    dx, dx
        jcxz   loc_1_0610
        test   byte ptr [ebx + 6], 4
        jne    SHORT loc_1_05F4
        or     dx, 800h
loc_1_05F4:
        or     dx, 40h
        test   byte ptr [ebx + 6], 2
        je     SHORT loc_1_0603
        or     dx, 8000h
loc_1_0603:
        test   word ptr [ebx + 18h], 1
        je     SHORT loc_1_0610
        or     dx, 1000h
loc_1_0610:
        mov    word ptr [ebx + 8], cx
        mov    ax, dx
        mov    dx, word ptr [esi + 4]
        mov    esi, ebx
        mov    bx, ax
        xor    ax, ax
        jcxz   loc_1_062B
        mov    al, byte ptr [esi + 1ch]
        xor    ah, ah
loc_1_062B:
        or     bx, ax
        mov    word ptr [esi + 0ah], bx
        mov    cx, word ptr [esi + 2]
        mov    ax, word ptr [esi + 4]
        and    cx, 0fffh
        sub    ax, cx
        and    ax, 3fffh
        mov    cl, byte ptr [esi + 7]
        shl    ax, 1
        shr    cl, 1
        rcr    ax, 1
        push   esi
        mov    esi, dword ptr [ebp - 4]
        cmp    word ptr [esi + 8], 2
        pop    esi
        jne    SHORT loc_1_067E
        test   word ptr [esi + 0ah], 8000h
        je     SHORT loc_1_067E
        push   eax
        add    dx, 8
        mov    al, 0a0h
        out    dx, al
        add    dx, 6
        in     ax, dx
        or     ax, 10h
        out    dx, ax
        add    dx, -0eh
        pop    eax
loc_1_067E:
        mov    word ptr [esi + 0ch], ax
        mov    cx, word ptr [esi + 8]
        push   eax
        mov    ax, word ptr [esi + 10h]
        call   sub_1_082C
        xor    ax, ax
        out    dx, ax
        mov    ax, cx
        out    dx, ax
        pop    eax
        out    dx, ax
        push   ebx
        mov    ebx, dword ptr [ebp - 4]
        cmp    word ptr [ebx + 8], 2
        pop    ebx
        jne    SHORT loc_1_06F1
        push   bx
        and    bx, 0ffe0h
        add    dx, 6
        mov    al, 1
        out    dx, al
        add    dx, -6
        in     ax, dx
        and    ax, 1fh
        or     ax, bx
        out    dx, ax
        movzx  eax, byte ptr [esi + 1ah]
        sub    al, 4
        not    al
        and    al, 7
        add    al, 4
        mov    cl, 1eh
        mul    cl
        mov    esi, dword ptr [ebp - 4]
        mov    esi, dword ptr [esi + 1ah]
        add    esi, eax
        mov    ax, word ptr [esi + 10h]
        add    ax, 6
        inc    dx
        inc    dx
        out    dx, ax
        pop    bx
        dec    dx
        dec    dx
loc_1_06F1:
        and    bx, 1fh
        mov    ax, word ptr [esi + 0ah]
        and    ax, 0ffe0h
        or     ax, bx
        out    dx, ax
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        clc
        ret
loc_1_070A:
        clc
        or     al, al
        je     SHORT loc_1_0712
        mov    ah, 2
        stc
loc_1_0712:
        ret
loc_1_0713:
        mov    ah, 2
        stc
        ret
loc_1_0717:
        push   ebx
        mov    ebx, dword ptr [ebp - 4]
        and    byte ptr [ebx + 2ah], 0feh
        or     al, al
        je     SHORT loc_1_0727
        or     byte ptr [ebx + 2ah], 1
loc_1_0727:
        pop    ebx
        clc
        ret
loc_1_072A:
        stc
        ret
loc_1_072C:
        push   eax
        push   ebx
        push   ecx
        push   edx
        mov    ch, al
        call   sub_1_0B2D
        mov    ebx, dword ptr [ebp - 4]
        mov    ax, word ptr [ebx + 52h]
        mov    ah, 1
        test   al, 10h
        jne    SHORT loc_1_0747
        or     ah, 3
loc_1_0747:
        add    dx, 0dh
        in     al, dx
        or     al, ah
        mov    cl, al
        and    cl, 3
        mov    ah, byte ptr [ebx + 2ch]
        and    ah, 7fh
        test   ch, ch
        je     SHORT loc_1_0763
        or     ah, 80h
        or     cl, 60h
loc_1_0763:
        mov    byte ptr [ebx + 2ch], ah
        test   al, 60h
        jne    SHORT loc_1_0779
        test   cl, 60h
        je     SHORT loc_1_0779
        dec    dx
        in     al, dx
        and    al, 9fh
        or     al, 1
        out    dx, al
        inc    dx
loc_1_0779:
        push   cx
        mov    cl, 1
        dec    dx
        mov    al, cl
        out    dx, al
        add    dx, -8
        in     ax, dx
        mov    bx, ax
        xor    bx, 1000h
        in     ax, dx
        and    ax, 8fffh
        or     ax, bx
        out    dx, ax
        add    dx, 8
        mov    al, cl
        out    dx, al
        xor    bx, 1000h
        add    dx, -8
        in     ax, dx
        and    ax, 8fffh
        or     ax, bx
        out    dx, ax
        pop    cx
        add    dx, 9
        mov    al, cl
        out    dx, al
        mov    ebx, dword ptr [ebp - 4]
        cmp    byte ptr [ebx + 26h], 1
        je     SHORT loc_1_07FC
        add    dx, -9
        in     ax, dx
        mov    bx, ax
        xor    bx, 1000h
        in     ax, dx
        and    ax, 8fffh
        or     ax, bx
        out    dx, ax
        add    dx, 9
        mov    al, cl
        out    dx, al
        xor    bx, 1000h
        add    dx, -9
        in     ax, dx
        and    ax, 8fffh
        or     ax, bx
        out    dx, ax
loc_1_07FC:
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        ret
sub_1_0801:
        push   edx
        push   ebx
        xor    ax, ax
        mov    ebx, dword ptr [ebp - 1ch]
        cmp    byte ptr [ebx + 0eh], 0
        je     SHORT loc_1_0813
        mov    ax, 1000h
loc_1_0813:
        add    dx, 4
        push   bx
        mov    bx, ax
        in     ax, dx
        and    ax, 8fffh
        or     ax, bx
        pop    bx
        out    dx, ax
        pop    ebx
        pop    edx
        ret
sub_1_082C:
        push   edx
        push   eax
        mov    ax, 1
        call   sub_1_0E5D
        add    dx, 4
        in     ax, dx
        and    ax, 7000h
        or     ax, 800h
        out    dx, ax
        pop    eax
        dec    dx
        dec    dx
        out    dx, ax
        pop    edx
        ret
        db  056h
sub_1_0851:
        push   edx
        push   ecx
        push   ebx
        call   sub_1_0B2D
        mov    cx, ax
        and    ax, 0fh
        mov    ebx, dword ptr [ebp - 4]
        lea    ebx, [ebx + 38h]
        xlatb
        test   al, 0f0h
        je     SHORT loc_1_087D
        mov    al, 1
        or     al, al
        je     SHORT loc_1_0878
        xor    al, al
        test   cl, 80h
        je     SHORT loc_1_087D
loc_1_0878:
        mov    ah, 6
        stc
        jmp    SHORT loc_1_08A7
loc_1_087D:
        mov    ebx, dword ptr [ebp - 1ch]
        and    word ptr [ebx + 6], 0ffb0h
        test   cl, 40h
        jne    SHORT loc_1_088E
        or     ax, 40h
loc_1_088E:
        test   cl, 80h
        je     SHORT loc_1_0897
        or     word ptr [ebx + 6], ax
loc_1_0897:
        mov    ax, word ptr [ebx + 4]
        call   sub_1_082C
        mov    ax, word ptr [ebx + 6]
        out    dx, ax
        clc
loc_1_08A7:
        pop    ebx
        pop    ecx
        pop    edx
        ret
sub_1_08AB:
        push   ebx
        push   edx
        push   eax
        call   sub_1_0B2D
        mov    ebx, dword ptr [ebp - 1ch]
        cmp    ax, 2
        je     SHORT loc_1_08D8
        cmp    ax, 1
        je     SHORT loc_1_08C9
        pop    ax
        mov    ah, 0dh
        stc
        jmp    SHORT loc_1_08F5
loc_1_08C9:
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 52h]
        mov    word ptr [ebx + 6], ax
        pop    eax
        jmp    SHORT loc_1_08DE
loc_1_08D8:
        or     word ptr [ebx + 6], 3800h
loc_1_08DE:
        mov    ax, word ptr [ebx + 4]
        call   sub_1_082C
        mov    ax, word ptr [ebx + 6]
        out    dx, ax
        mov    ax, word ptr [ebx + 8]
        out    dx, ax
        clc
        pop    eax
loc_1_08F5:
        pop    edx
        pop    ebx
        ret
        db  053h, 052h, 08Bh, 05Dh, 0E4h, 066h, 0C7h, 043h, 008h, 000h, 000h, 066h, 08Bh, 043h, 004h, 0E8h
        db  020h, 0FFh, 0FFh, 0FFh, 066h, 08Bh, 043h, 006h, 066h, 0EFh, 066h, 08Bh, 043h, 008h, 066h, 0EFh
        db  05Ah, 05Bh, 0F8h, 0C3h
sub_1_091C:
        push   ecx
        push   edx
        push   eax
        call   sub_1_0B2D
        call   sub_1_020D
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 8]
        mov    ch, al
        pop    eax
        cmp    ch, 4
        je     SHORT loc_1_0946
        cmp    ch, 8
        je     SHORT loc_1_0946
        cmp    ch, 14h
        je     SHORT loc_1_0946
        shl    al, cl
        jmp    SHORT loc_1_094C
loc_1_0946:
        or     al, al
        je     SHORT loc_1_094C
        or     al, 20h
loc_1_094C:
        add    dx, 9
        out    dx, al
        pop    edx
        pop    ecx
        clc
        ret
sub_1_0955:
        push   ebx
        push   eax
        mov    ebx, dword ptr [ebp - 4]
        mov    dx, word ptr [ebx + 4]
        mov    ebx, dword ptr [ebp - 1ch]
        or     ebx, ebx
        je     SHORT loc_1_0989
        mov    ebx, dword ptr [ebp - 4]
        cmp    byte ptr [ebx + 6], 1
        ja     SHORT loc_1_0989
        add    dx, 7
        in     al, dx
        test   al, 80h
        jne    SHORT loc_1_0989
        add    dx, -7
        xor    eax, eax
        call   sub_1_082C
        add    dx, 6
        mov    al, 0
        out    dx, al
loc_1_0989:
        pop    eax
        pop    ebx
        ret
sub_1_098C:
        push   edx
        push   ebx
        mov    ebx, dword ptr [ebp - 4]
        push   eax
        mov    al, 40h
        call   sub_1_0E4E
        pop    eax
        add    dx, 4
        push   bx
        mov    bx, ax
        in     ax, dx
        and    ax, 8fffh
        or     ax, bx
        pop    bx
        out    dx, ax
        mov    ax, word ptr [ebx + 4ah]
        or     eax, 7
        add    dx, 0ah
        out    dx, ax
        add    dx, -6
        mov    al, 0c0h
        out    dx, al
        mov    ax, word ptr [ebx + 4eh]
        add    dx, 6
        out    dx, ax
        pop    ebx
        pop    edx
        ret
sub_1_09D1:
        push   ebx
        push   esi
        push   edx
        mov    esi, dword ptr [ebp - 4]
        mov    ebx, dword ptr [ebp - 1ch]
        mov    byte ptr [ebx + 0dh], 1
        and    byte ptr [ebx + 0ch], 7fh
        mov    word ptr [ebx + 0ah], 0
        and    byte ptr [ebx + 0fh], 20h
        call   sub_1_0B2D
        mov    al, 0c0h
        call   sub_1_0E4E
        mov    ax, word ptr [esi + 4eh]
        add    dx, 0eh
        out    dx, ax
        add    dx, -0eh
        mov    ebx, dword ptr [ebp - 1ch]
        mov    ax, word ptr [ebx + 4]
        add    ax, 2
        call   sub_1_082C
        in     ax, dx
        and    ax, 0f8h
        push   eax
        mov    si, word ptr [esi + 52h]
        mov    word ptr [ebx + 6], si
        push   esi
        call   sub_1_0A4F
        mov    ax, word ptr [ebx + 4]
        call   sub_1_082C
        pop    esi
        mov    ax, si
        out    dx, ax
        pop    eax
        out    dx, ax
        mov    word ptr [ebx + 18h], 0
        add    dx, 9
        mov    al, 0
        out    dx, al
        pop    edx
        pop    esi
        pop    ebx
        clc
        ret
sub_1_0A4F:
        push   ebx
        push   esi
        push   edi
        push   cx
        mov    esi, dword ptr [ebp - 4]
        mov    ebx, dword ptr [ebp - 1ch]
        mov    bl, byte ptr [ebx + 0eh]
        xor    bh, bh
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 27h]
        mov    esi, dword ptr [esi + 1ah]
loc_1_0A67:
        push   ecx
        cmp    word ptr [esi + 18h], bx
        jne    SHORT loc_1_0AAC
        test   word ptr [esi + 0ah], 8000h
        je     SHORT loc_1_0AAC
        mov    dword ptr [ebp - 18h], esi
        mov    word ptr [esi], 1
        mov    word ptr [esi + 2], 0
        mov    word ptr [esi + 4], 0
        and    byte ptr [esi + 6], 0fdh
        and    byte ptr [esi + 7], 0fdh
        db  0BFh, 0A4h, 004h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        movzx  eax, byte ptr [eax + 2fh]
        cmp    byte ptr [esi + 1ah], al
        jb     SHORT loc_1_0AA8
        db  0BFh, 0ADh, 005h, 000h, 000h
loc_1_0AA8:
        xor    eax, eax
        call   edi
loc_1_0AAC:
        pop    ecx
        add    esi, 1eh
        loop   loc_1_0A67
        pop    cx
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_0AB8:
        push   ebx
        mov    ebx, 0ff19h
        call   sub_1_0C9B
        mov    ebx, dword ptr [ebp - 1ch]
        or     byte ptr [ebx + 0fh], 1
        pop    ebx
        ret
loc_1_0ACC:
        call   sub_1_1276
        jb     SHORT loc_1_0B2C
        mov    ebx, dword ptr [ebp - 18h]
        mov    ebx, dword ptr [ebx + 12h]
        mov    dx, word ptr [ebx]
        mov    byte ptr [ebp + 4], dl
        mov    esi, dword ptr [ebp - 4]
        xor    eax, eax
        mov    al, byte ptr [esi + 26h]
        cmp    eax, 1
        je     SHORT loc_1_0AEF
        or     eax, 1
loc_1_0AEF:
        mov    word ptr [ebp - 0ch], ax
        mov    al, dl
        mov    edi, dword ptr [ebp - 14h]
        movzx  ecx, word ptr [edi]
        xor    dx, dx
        jcxz   loc_1_0B26
        test   al, 4
        mov    ax, 12h
        je     SHORT loc_1_0B0D
        mov    ax, 0eh
loc_1_0B0D:
        mov    esi, dword ptr [ebx + 2]
        add    esi, 2
        cmp    cx, ax
        jbe    SHORT loc_1_0B1B
        movzx  ecx, ax
loc_1_0B1B:
        mov    dx, cx
        push   edi
        add    edi, 4
        cld
        rep movsb
        pop    edi
loc_1_0B26:
        mov    word ptr [edi + 2], dx
        mov    ah, 0
loc_1_0B2C:
        ret
sub_1_0B2D:
        push   ebx
        mov    ebx, dword ptr [ebp - 4]
        mov    dx, word ptr [ebx + 4]
        pop    ebx
        ret
sub_1_0B37:
        call   sub_1_0229
        cmp    ah, 0
        je     SHORT loc_1_0B47
        mov    ax, 0ffffh
        jmp    SHORT loc_1_0B4A
loc_1_0B47:
        xor    ax, ax
loc_1_0B4A:
        ret
loc_1_0B4B:
        call   sub_1_1276
        jb     SHORT loc_1_0B79
        mov    ebx, dword ptr [ebp - 18h]
        mov    ax, word ptr [ebx + 18h]
        mov    byte ptr [ebp + 4], al
        mov    ax, word ptr [ebx]
        mov    word ptr [ebp - 0ch], ax
        mov    al, byte ptr [ebx + 6]
        mov    byte ptr [ebp - 0fh], al
        mov    ax, word ptr [ebx + 2]
        mov    word ptr [ebp - 14h], ax
        mov    al, byte ptr [ebx + 1bh]
        mov    byte ptr [ebp - 10h], al
        mov    ah, 0
loc_1_0B79:
        ret
loc_1_0B7A:
        call   sub_1_1276
        jb     SHORT loc_1_0BC1
        mov    esi, dword ptr [ebp - 18h]
        mov    dh, byte ptr [ebp - 0fh]
        test   dh, 0f8h
        jne    SHORT loc_1_0BBF
        mov    dx, word ptr [ebp - 14h]
        mov    cx, word ptr [ebp - 0ch]
        call   sub_1_0FED
        jae    SHORT loc_1_0BA0
        jmp    NEAR PTR loc_1_0C9A
loc_1_0BA0:
        mov    dh, byte ptr [ebp - 0fh]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        movzx  eax, byte ptr [eax + 2fh]
        cmp    byte ptr [esi + 1ah], al
        pop    eax
        jae    SHORT loc_1_0BB3
        not    dh
loc_1_0BB3:
        test   dh, 1
        je     SHORT loc_1_0BC6
        mov    ah, 0dh
        jmp    NEAR PTR loc_1_0C9A
loc_1_0BBF:
        mov    ah, 2
loc_1_0BC1:
        jmp    NEAR PTR loc_1_0C9A
loc_1_0BC6:
        mov    al, byte ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        movzx  eax, byte ptr [eax + 2fh]
        cmp    byte ptr [esi + 1ah], al
        pop    eax
        jae    SHORT loc_1_0BD9
        mov    al, 0ah
loc_1_0BD9:
        call   sub_1_0D67
        jb     SHORT loc_1_0BC1
        mov    byte ptr [esi + 1bh], al
        mov    byte ptr [esi + 1ch], ah
        xor    ah, ah
        mov    al, byte ptr [ebp + 4]
        cmp    ax, word ptr [esi + 18h]
        jne    SHORT loc_1_0C16
        mov    cx, word ptr [ebp - 0ch]
        cmp    cx, word ptr [esi]
        jne    SHORT loc_1_0C16
        mov    al, byte ptr [ebp - 0fh]
        cmp    al, byte ptr [esi + 6]
        jne    SHORT loc_1_0C16
        mov    di, word ptr [ebp - 14h]
        cmp    di, word ptr [esi + 2]
        jne    SHORT loc_1_0C16
        test   byte ptr [esi + 6], 2
        je     NEAR PTR loc_1_0C98
loc_1_0C16:
        and    byte ptr [esi + 7], 0fdh
        mov    al, byte ptr [ebp - 0fh]
        test   al, 2
        je     SHORT loc_1_0C25
        or     byte ptr [esi + 7], 2
loc_1_0C25:
        mov    al, byte ptr [ebp - 0fh]
        mov    byte ptr [esi + 6], al
        mov    cx, word ptr [ebp - 0ch]
        mov    word ptr [esi], cx
        mov    di, word ptr [ebp - 14h]
        mov    word ptr [esi + 2], di
        mov    bl, byte ptr [ebp + 4]
        mov    bh, 0
        mov    word ptr [esi + 18h], bx
        mov    ah, byte ptr [ebp + 5]
        mov    al, 1
        mov    ebx, esi
        db  0BEh, 0A4h, 004h, 000h, 000h
        push   eax
        mov    eax, dword ptr [ebp - 4]
        movzx  eax, byte ptr [eax + 2fh]
        cmp    byte ptr [ebx + 1ah], al
        pop    eax
        jb     SHORT loc_1_0C62
        db  0BEh, 0ADh, 005h, 000h, 000h
loc_1_0C62:
        call   esi
        mov    ax, 1
        mov    cl, byte ptr [ebx + 1ah]
        shl    ax, cl
        mov    dx, ax
        not    dx
        test   byte ptr [ebx + 6], 2
        jne    SHORT loc_1_0C7D
        xor    ax, ax
loc_1_0C7D:
        mov    esi, dword ptr [ebp - 4]
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 26h]
        mov    esi, dword ptr [esi + 16h]
loc_1_0C88:
        and    word ptr [esi + 0ah], dx
        add    esi, 1ah
        loop   loc_1_0C88
        mov    esi, dword ptr [ebp - 1ch]
        or     word ptr [esi + 0ah], ax
loc_1_0C98:
        mov    ah, 0
loc_1_0C9A:
        ret
sub_1_0C9B:
        push   edx
        add    dx, 6
        in     al, dx
        and    al, bh
        or     al, bl
        out    dx, al
        pop    edx
        ret
sub_1_0CA8:
        push   edx
        push   ebx
        call   sub_1_0B2D
        mov    ebx, dword ptr [ebp - 1ch]
        add    dx, 7
        in     al, dx
        dec    dx
        call   sub_1_0D14
        test   eax, 80h
        jne    SHORT loc_1_0CCA
        xor    eax, eax
        out    dx, al
        jmp    SHORT loc_1_0CDB
loc_1_0CCA:
        mov    ah, al
        add    dx, 3
        in     al, dx
        and    al, 1bh
        jne    SHORT loc_1_0CDB
        and    ah, 80h
        or     ah, 40h
loc_1_0CDB:
        mov    al, ah
        pop    ebx
        pop    edx
        ret
sub_1_0CE0:
        call   sub_1_0CA8
        push   edx
        push   esi
        push   ax
        call   sub_1_0B2D
        add    dx, 0ch
        in     ax, dx
        test   ah, 3
        je     SHORT loc_1_0CFE
        test   ah, 40h
        je     SHORT loc_1_0CFE
loc_1_0CFE:
        mov    esi, dword ptr [ebp - 1ch]
        mov    al, 0bdh
        out    dx, al
        pop    ax
        mov    ah, byte ptr [esi + 14h]
        mov    byte ptr [esi + 14h], al
        xor    ah, al
        or     byte ptr [esi + 13h], ah
        pop    esi
        pop    edx
        ret
sub_1_0D14:
        xor    bl, bl
        shl    ax, 1
        rcl    al, 1
        rcl    bl, 1
        shr    ax, 1
        and    al, 0f0h
        or     al, bl
        ret
sub_1_0D25:
        mov    al, 0ffh
        call   sub_1_0DFD
        mov    cx, 2
        call   sub_1_0074
        mov    al, 0
        call   sub_1_0DFD
        clc
        ret
        db  053h, 051h, 052h, 0E8h, 021h, 000h, 000h, 000h, 072h, 01Bh, 08Bh, 05Dh, 0E8h, 088h, 043h, 01Bh
        db  088h, 063h, 01Ch, 0E8h, 0D7h, 0FDh, 0FFh, 0FFh, 0E8h, 077h, 000h, 000h, 000h, 0B0h, 0FFh, 0E8h
        db  0BAh, 000h, 000h, 000h, 0F8h, 05Ah, 059h, 05Bh, 0C3h
sub_1_0D67:
        push   ebx
        push   ecx
        mov    cl, al
        mov    ah, al
        test   ah, 80h
        jne    SHORT loc_1_0DCA
        test   ah, 0f8h
        jne    SHORT loc_1_0D8B
        movzx  ebx, al
        sub    ebx, 1
        jb     SHORT loc_1_0DCA
        cmp    al, 4
        ja     SHORT loc_1_0DCA
        db  08Ah, 0A3h, 0BBh, 033h, 000h, 000h
        jmp    SHORT loc_1_0DBC
loc_1_0D8B:
        and    ah, 7
        xor    ah, 2
        jne    SHORT loc_1_0DA2
        shr    al, 3
        movzx  ebx, ax
        dec    ebx
        db  08Ah, 0A3h, 0ACh, 033h, 000h, 000h
        jmp    SHORT loc_1_0DBC
loc_1_0DA2:
        mov    ah, 1
        cmp    cl, 79h
        je     SHORT loc_1_0DBC
        mov    ah, 1ch
        cmp    cl, 0bh
        je     SHORT loc_1_0DBC
        and    cl, 7
        cmp    cl, 2
        jae    SHORT loc_1_0DCA
        mov    ah, 1
        mov    cl, 79h
loc_1_0DBC:
        mov    ebx, dword ptr [ebp - 4]
        push   ecx
        mov    cx, word ptr [ebx + 54h]
        shr    ah, cl
        pop    ecx
        clc
        jmp    SHORT loc_1_0DCD
loc_1_0DCA:
        mov    ah, 17h
        stc
loc_1_0DCD:
        mov    al, cl
        pop    ecx
        pop    ebx
        ret
        db  053h, 08Ah, 0D8h, 066h, 083h, 0C2h, 008h, 0ECh, 024h, 01Fh, 00Ch, 040h, 0EEh, 066h, 083h, 0C2h
        db  006h, 066h, 0EDh, 066h, 083h, 0E0h, 0E0h, 00Ah, 0C3h, 053h, 08Bh, 05Dh, 0FCh, 066h, 00Bh, 043h
        db  04Ah, 05Bh, 066h, 0EFh, 066h, 083h, 0C2h, 0F2h, 0F8h, 05Bh, 0C3h
sub_1_0DFD:
        push   edx
        and    al, 4
        push   ebx
        xchg   bx, ax
        call   sub_1_0E3F
        and    al, 0f3h
        or     al, bl
        or     al, 18h
        mov    ebx, dword ptr [ebp - 4]
        or     al, byte ptr [ebx + 4eh]
        add    dx, 0eh
        out    dx, al
        pop    ebx
        pop    edx
        ret
        db  052h, 024h, 010h, 053h, 066h, 093h, 0E8h, 018h, 000h, 000h, 000h, 0A8h, 020h, 074h, 002h, 033h
        db  0DBh, 024h, 0EFh, 00Ah, 0C3h, 08Bh, 05Dh, 0FCh, 00Ah, 043h, 04Eh, 066h, 083h, 0C2h, 00Eh, 0EEh
        db  05Bh, 05Ah, 0C3h
sub_1_0E3F:
        push   edx
        mov    al, 0c0h
        call   sub_1_0E4E
        add    dx, 0eh
        in     al, dx
        pop    edx
        ret
sub_1_0E4E:
        push   edx
        mov    ah, al
        add    dx, 8
        in     al, dx
        and    al, 1fh
        or     al, ah
        out    dx, al
        pop    edx
        ret
sub_1_0E5D:
        push   eax
        push   ebx
        xor    bl, bl
        mov    bh, 0e7h
        shr    al, 1
        jae    SHORT loc_1_0E69
        mov    bl, 18h
loc_1_0E69:
        call   sub_1_0C9B
        pop    ebx
        pop    eax
        ret
sub_1_0E71:
        push   eax
        push   ebx
        push   ecx
        push   edx
        push   edi
        call   sub_1_0B2D
        not    al
        shl    ax, 1
        shr    al, 2
        rcl    ah, 1
        shl    al, 2
        shr    ax, 5
        shl    al, 3
        mov    cx, ax
        and    cx, 0f8h
        mov    ebx, dword ptr [ebp - 1ch]
        mov    ax, word ptr [ebx + 4]
        inc    ax
        inc    ax
        call   sub_1_082C
        mov    ax, 1
        add    dx, 6
        out    dx, al
        add    dx, 7
        in     al, dx
        mov    edi, eax
        and    al, 3
        out    dx, al
        add    dx, -0dh
        in     ax, dx
        and    ax, 0ff07h
        or     ax, cx
        out    dx, ax
        mov    word ptr [ebx + 8], ax
        push   ebx
        mov    cl, 0bdh
        add    dx, 0ch
        mov    al, cl
        out    dx, al
        add    dx, -8
        in     ax, dx
        mov    bx, ax
        xor    bx, 1000h
        in     ax, dx
        and    ax, 8fffh
        or     ax, bx
        out    dx, ax
        add    dx, 8
        mov    al, cl
        out    dx, al
        xor    bx, 1000h
        add    dx, -8
        in     ax, dx
        and    ax, 8fffh
        or     ax, bx
        out    dx, ax
        mov    eax, edi
        add    dx, 9
        out    dx, al
        pop    ebx
        add    dx, -6
        in     al, dx
        mov    byte ptr [ebx + 15h], al
        push   ebx
        call   sub_1_0D14
        pop    ebx
        mov    byte ptr [ebx + 14h], al
        pop    edi
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        clc
        ret
sub_1_0F2C:
        push   eax
        push   ebx
        push   ecx
        push   edx
        pushfd
        cli
        and    ax, 0fh
        mov    ebx, dword ptr [ebp - 4]
        lea    ebx, [ebx + 38h]
        xlatb
        test   al, 0f0h
        jne    NEAR PTR loc_1_0FDE
        push   eax
        call   sub_1_0B2D
        mov    cl, 0
        add    dx, 0dh
        mov    al, cl
        out    dx, al
        add    dx, -9
        in     ax, dx
        mov    bx, ax
        xor    bx, 1000h
        in     ax, dx
        and    ax, 8fffh
        or     ax, bx
        out    dx, ax
        add    dx, 9
        mov    al, cl
        out    dx, al
        xor    bx, 1000h
        add    dx, -9
        in     ax, dx
        and    ax, 8fffh
        or     ax, bx
        out    dx, ax
        pop    ebx
        add    dx, 4
        mov    al, 0a0h
        out    dx, al
        add    dx, 6
        in     ax, dx
        and    ax, 0fff0h
        or     al, bl
        out    dx, ax
        mov    cl, 1
        dec    dx
        dec    dx
        mov    al, cl
        out    dx, al
        add    dx, -8
        in     ax, dx
        mov    bx, ax
        xor    bx, 1000h
        in     ax, dx
        and    ax, 8fffh
        or     ax, bx
        out    dx, ax
        add    dx, 8
        mov    al, cl
        out    dx, al
        xor    bx, 1000h
        add    dx, -8
        in     ax, dx
        and    ax, 8fffh
        or     ax, bx
        out    dx, ax
        clc
        jmp    SHORT loc_1_0FE1
loc_1_0FDE:
        mov    ah, 6
        stc
loc_1_0FE1:
        popfd
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        ret
loc_1_0FE7:
        clc
        ret
sub_1_0FE9:
        clc
        ret
sub_1_0FEB:
        clc
        ret
sub_1_0FED:
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        mov    edi, dword ptr [esi + 12h]
        mov    edi, dword ptr cs:[edi + 2]
        or     cx, cx
        je     SHORT loc_1_1008
        mov    ax, cx
        call   sub_1_1063
        jae    SHORT loc_1_100D
loc_1_1008:
        mov    ah, 0ah
        stc
        jmp    SHORT loc_1_105D
loc_1_100D:
        jcxz   loc_1_1027
        push   dx
        mov    ax, dx
        xor    dx, dx
        div    cx
        or     dx, dx
        pop    dx
        je     SHORT loc_1_1027
loc_1_1022:
        mov    ah, 3
        stc
        jmp    SHORT loc_1_105D
loc_1_1027:
        mov    eax, dword ptr [ebp - 4]
        movzx  eax, byte ptr [eax + 2fh]
        cmp    byte ptr [esi + 1ah], al
        jae    SHORT loc_1_103F
        mov    ax, dx
        add    ax, cx
        je     SHORT loc_1_105D
        jb     SHORT loc_1_1008
        jmp    SHORT loc_1_105D
loc_1_103F:
        jcxz   loc_1_1008
        cmp    cx, 1000h
        ja     SHORT loc_1_1008
        cmp    dx, 0fffh
        ja     SHORT loc_1_1022
        mov    ax, dx
        add    ax, cx
        cmp    ax, 1000h
        ja     SHORT loc_1_1008
        clc
loc_1_105D:
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret
sub_1_1063:
        push   eax
        push   ecx
        mov    cx, ax
        not    ax
        dec    cx
        xor    ax, cx
        and    ax, cx
        je     SHORT loc_1_1076
        stc
loc_1_1076:
        pop    ecx
        pop    eax
        ret
        db  000h, 000h, 000h
loc_1_107C:
        pushfd
        inc    byte ptr [esi + 6]
        push   ebx
        push   ebp
        mov    ebp, esp
        push   esi
        push   eax
        push   ecx
        push   edx
        push   edi
        lea    esp, [ebp - 24h]
        mov    byte ptr [ebp - 24h], 0
        cld
        cmp    ah, 9eh
        je     SHORT loc_1_10DF
        sub    ah, 80h
        jb     SHORT loc_1_10FF
        cmp    ah, 30h
        ja     SHORT loc_1_10FF
        movzx  eax, ah
        db  08Bh, 004h, 085h, 031h, 034h, 000h, 000h
        cmp    dword ptr [esi + 0ah], 44425353h
        jne    SHORT loc_1_1103
        call   eax
loc_1_10B5:
        mov    byte ptr [ebp - 7], ah
        and    dword ptr [ebp + 8], 0fffffffeh
        or     ah, ah
        je     SHORT loc_1_10C4
        or     dword ptr [ebp + 8], 1
loc_1_10C4:
        pushf
        call   sub_1_0955
        mov    esi, dword ptr [ebp - 4]
        dec    byte ptr [esi + 6]
        popf
        lea    esp, [ebp - 14h]
        pop    edi
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        pop    ebp
        pop    ebx
        popfd
        ret
loc_1_10DF:
        call   sub_1_1804
        mov    byte ptr [ebp - 7], ah
        and    dword ptr [ebp + 8], 0fffffffeh
        or     ah, ah
        je     SHORT loc_1_10F3
        or     dword ptr [ebp + 8], 1
loc_1_10F3:
        jmp    SHORT loc_1_10C4
        db  0C6h, 045h, 0F9h, 018h, 083h, 04Dh, 008h, 001h, 0EBh, 0C5h
loc_1_10FF:
        mov    ah, 15h
        jmp    SHORT loc_1_10B5
loc_1_1103:
        mov    ah, 1
        jmp    SHORT loc_1_10B5
        db  0F8h, 00Ah, 0C0h, 074h, 003h, 0B4h, 002h, 0F9h, 0C3h
loc_1_1110:
        mov    ah, 15h
        ret
sub_1_1113:
        mov    edi, dword ptr [ebp + 4]
        db  0BEh, 08Eh, 032h, 000h, 000h
        mov    ecx, 8fh
        cld
        push   edi
        rep movsw
        pop    edi
        mov    esi, edi
        mov    ecx, 200h
        xor    ax, ax
loc_1_1130:
        lodsb
        add    ah, al
        loop   loc_1_1130
        neg    ah
        mov    byte ptr [edi + 2bh], ah
        xor    ax, ax
        ret
sub_1_113E:
        push   ebx
        push   edi
        push   esi
        mov    edx, ebx
        add    edx, 56h
        db  08Dh, 035h, 07Ch, 032h, 000h, 000h
        mov    dword ptr [ebx + 12h], esi
        mov    byte ptr [ebx + 28h], 0
        mov    byte ptr [ebx + 29h], 0
        mov    byte ptr [ebx + 2ah], 0
        or     word ptr [ebx + 2ch], 40h
        mov    edi, ebx
        mov    ebx, dword ptr [edi + 16h]
        mov    cl, byte ptr [edi + 26h]
        xor    ch, ch
        mov    edx, dword ptr [esi + 0eh]
        add    edx, 2
loc_1_1170:
        mov    dword ptr [ebx], edx
        mov    byte ptr [ebx + 0eh], ch
        mov    word ptr [ebx + 0ah], 0
        call   sub_1_0387
        add    ebx, 1ah
        inc    ch
        cmp    ch, cl
        jne    SHORT loc_1_1170
        mov    ebx, dword ptr [edi + 1ah]
        mov    cl, byte ptr [edi + 27h]
        xor    ch, ch
loc_1_1191:
        mov    byte ptr [ebx + 1ah], ch
        call   sub_1_03B6
        add    ebx, 1eh
        inc    ch
        cmp    ch, cl
        jne    SHORT loc_1_1191
        pop    esi
        pop    edi
        pop    ebx
        ret
sub_1_11A6:
        call   sub_1_1559
        jb     SHORT loc_1_11D7
        mov    bl, byte ptr [ebp + 4]
sub_1_11B0:
        push   ecx
        push   esi
        mov    esi, dword ptr [ebp - 4]
        cmp    bl, byte ptr [esi + 26h]
        cmc
        jae    SHORT loc_1_11BF
        mov    ah, 0bh
        jmp    SHORT loc_1_11D5
loc_1_11BF:
        xor    eax, eax
        mov    al, bl
        mov    cl, 1ah
        mul    cl
        mov    esi, dword ptr [esi + 16h]
        add    esi, eax
        mov    dword ptr [ebp - 1ch], esi
        call   sub_1_0587
        clc
loc_1_11D5:
        pop    esi
        pop    ecx
loc_1_11D7:
        ret
loc_1_11D8:
        call   sub_1_1559
        jae    SHORT loc_1_11E4
        jmp    NEAR PTR loc_1_1275
loc_1_11E4:
        cld
        mov    ebx, dword ptr [ebp - 20h]
        mov    edi, dword ptr [ebp - 4]
        mov    ah, byte ptr [edi + 27h]
        mov    al, byte ptr [edi + 26h]
        mov    word ptr [ebp + 4], ax
        mov    esi, dword ptr [edi + 30h]
        xor    ax, ax
        mov    edi, dword ptr [ebp - 14h]
        movzx  ecx, word ptr [edi]
        add    cx, 4
        xor    dx, dx
        cmp    cx, 10h
        je     SHORT loc_1_1231
        jb     SHORT loc_1_123B
        push   edi
        push   esi
        push   cx
        lea    edi, [edi + 10h]
        sub    cx, 10h
        lodsw
        shl    ax, 1
        cmp    cx, ax
        jbe    SHORT loc_1_1228
        mov    cx, ax
loc_1_1228:
        add    dx, cx
        rep movsb
        pop    cx
        pop    esi
        pop    edi
loc_1_1231:
        lodsw
        inc    dx
        inc    dx
        mov    word ptr [edi + 0eh], ax
loc_1_123B:
        call   sub_1_17CD
        cmp    cx, 0eh
        jb     SHORT loc_1_124D
        add    dx, 4
        mov    dword ptr [edi + 0ah], eax
loc_1_124D:
        call   sub_1_17DB
        cmp    cx, 0ah
        jb     SHORT loc_1_125F
        add    dx, 4
        mov    dword ptr [edi + 6], eax
loc_1_125F:
        dec    cx
        jcxz   loc_1_126F
        inc    dx
        inc    dx
        mov    ax, word ptr [ebx]
        mov    word ptr [edi + 4], ax
loc_1_126F:
        mov    word ptr [edi + 2], dx
        mov    ah, 0
loc_1_1275:
        ret
sub_1_1276:
        call   sub_1_1559
        jb     SHORT loc_1_12B6
        push   ecx
        push   esi
        mov    esi, dword ptr [ebp - 4]
        mov    bh, byte ptr [ebp + 5]
        cmp    bh, byte ptr [esi + 27h]
        cmc
        jae    SHORT loc_1_128F
        mov    ah, 11h
        jmp    SHORT loc_1_12B4
loc_1_128F:
        xor    eax, eax
        mov    al, bh
        mov    cl, 1eh
        mul    cl
        mov    esi, dword ptr [esi + 1ah]
        add    esi, eax
        mov    dword ptr [ebp - 18h], esi
        mov    bx, word ptr [esi + 18h]
        cmp    byte ptr [ebp - 7], 89h
        jne    SHORT loc_1_12AC
        mov    bl, byte ptr [ebp + 4]
loc_1_12AC:
        call   sub_1_11B0
        jb     SHORT loc_1_12B4
        clc
loc_1_12B4:
        pop    esi
        pop    ecx
loc_1_12B6:
        ret
loc_1_12B7:
        mov    bl, 0
        call   sub_1_1276
        jb     SHORT loc_1_12ED
        mov    ebx, dword ptr [ebp - 18h]
        mov    eax, dword ptr [ebp - 4]
        movzx  eax, byte ptr [eax + 2fh]
        cmp    byte ptr [ebx + 1ah], al
        jae    SHORT loc_1_12D3
        mov    ah, 11h
        jmp    SHORT loc_1_12ED
loc_1_12D3:
        cmp    dword ptr [ebp + 4], 0
        je     SHORT loc_1_12DD
        mov    ah, 8
        jmp    SHORT loc_1_12ED
loc_1_12DD:
        mov    al, byte ptr [ebx + 7]
        mov    byte ptr [ebp - 10h], al
        mov    ax, word ptr [ebx + 4]
        mov    word ptr [ebp - 14h], ax
        mov    ah, 0
loc_1_12ED:
        ret
loc_1_12EE:
        call   sub_1_1559
        jb     SHORT loc_1_1313
        mov    word ptr [ebp + 4], 210h
        mov    bx, 1
        cmp    bx, 0
        je     SHORT loc_1_130B
        mov    word ptr [ebp + 4], 201h
loc_1_130B:
        mov    word ptr [ebp - 0ch], 101h
        mov    ah, 0
loc_1_1313:
        ret
loc_1_1314:
        call   sub_1_11A6
        jb     SHORT loc_1_1378
        mov    esi, dword ptr [ebp - 1ch]
        mov    esi, dword ptr [esi]
        mov    al, byte ptr [esi]
        mov    byte ptr [ebp + 5], al
        mov    ah, byte ptr [esi + 1]
        mov    al, byte ptr [esi + 2]
        mov    word ptr [ebp - 10h], ax
        mov    edi, dword ptr [ebp - 14h]
        movzx  ecx, word ptr [edi]
        add    cx, 4
        xor    dx, dx
        cmp    cx, 0eh
        jb     SHORT loc_1_134E
        call   sub_1_17CD
        mov    dword ptr [edi + 0ah], eax
        add    dx, 4
loc_1_134E:
        cmp    cx, 0ah
        jb     SHORT loc_1_1360
        call   sub_1_17DB
        mov    dword ptr [edi + 6], eax
        add    dx, 4
loc_1_1360:
        dec    cx
        jcxz   loc_1_1372
        xor    ah, ah
        mov    al, byte ptr [esi + 3]
        mov    word ptr [edi + 4], ax
        inc    dx
        inc    dx
loc_1_1372:
        mov    word ptr [edi + 2], dx
        mov    ah, 0
loc_1_1378:
        ret
sub_1_1379:
        push   ebx
        push   ecx
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_1_1383
        clc
        jmp    SHORT loc_1_13AD
loc_1_1383:
        mov    ebx, dword ptr [ebx]
        push   eax
        mov    ch, cl
        not    ch
        and    ah, cl
        and    al, cl
        sub    al, ah
        jne    SHORT loc_1_1395
        pop    eax
        jmp    SHORT loc_1_13AD
loc_1_1395:
        mov    ax, 1
        jb     SHORT loc_1_13A1
        mov    ax, 0
        xor    cl, cl
loc_1_13A1:
        call   ebx
        jae    SHORT loc_1_13A8
        pop    ecx
        jmp    SHORT loc_1_13AD
loc_1_13A8:
        pop    eax
        and    al, ch
        or     al, cl
loc_1_13AD:
        pop    ecx
        pop    ebx
        ret
loc_1_13B0:
        call   sub_1_11A6
        jae    SHORT loc_1_13BC
        jmp    NEAR PTR loc_1_14D1
loc_1_13BC:
        mov    edi, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp - 1ch]
        mov    cx, word ptr [ebp - 0ch]
        cmp    word ptr [esi + 18h], cx
        je     SHORT loc_1_13E1
        or     cx, cx
        jne    SHORT loc_1_13D5
        and    byte ptr [esi + 0fh], 0feh
loc_1_13D5:
        call   sub_1_15CE
        jae    SHORT loc_1_13E1
        jmp    NEAR PTR loc_1_14D1
loc_1_13E1:
        mov    word ptr [esi + 18h], cx
        mov    al, byte ptr [ebp + 5]
        or     al, al
        je     SHORT loc_1_140D
        push   eax
        push   esi
        mov    esi, dword ptr [esi]
        mov    ah, byte ptr [esi]
        mov    si, 1
        cmp    si, 0
        je     SHORT loc_1_13FE
        and    al, ah
loc_1_13FE:
        not    ah
        test   ah, al
        pop    esi
        pop    eax
        je     SHORT loc_1_140D
        mov    ah, 6
        jmp    NEAR PTR loc_1_14D1
loc_1_140D:
        cmp    byte ptr [esi + 12h], al
        je     SHORT loc_1_1421
        call   sub_1_0E71
        jae    SHORT loc_1_141E
        jmp    NEAR PTR loc_1_14D1
loc_1_141E:
        mov    byte ptr [esi + 12h], al
loc_1_1421:
        mov    al, byte ptr [ebp - 0fh]
        or     al, al
        je     SHORT loc_1_1432
        mov    ebx, dword ptr [esi]
        and    al, byte ptr [ebx + 1]
        not    al
        and    byte ptr [esi + 13h], al
loc_1_1432:
        mov    ah, byte ptr [ebp - 10h]
        mov    al, byte ptr [esi + 17h]
        db  0BBh, 0F8h, 034h, 000h, 000h
        mov    cx, 10h
loc_1_1441:
        call   sub_1_1379
        jb     NEAR PTR loc_1_14D1
        or     byte ptr [esi + 17h], al
        add    ebx, 4
        shr    cl, 1
        jae    SHORT loc_1_1441
        mov    ah, byte ptr [ebp - 10h]
        mov    al, byte ptr [edi + 29h]
        db  0BBh, 00Ch, 035h, 000h, 000h
        mov    cx, 80h
loc_1_1465:
        call   sub_1_1379
        jb     SHORT loc_1_14D1
        or     byte ptr [esi + 17h], al
        inc    bx
        inc    bx
        shr    cl, 1
        jae    SHORT loc_1_1465
        xor    ah, ah
        mov    al, byte ptr [ebp - 13h]
        mov    cl, al
        cmp    al, byte ptr [esi + 0dh]
        je     SHORT loc_1_148D
        call   sub_1_08AB
        jb     SHORT loc_1_14D1
        mov    byte ptr [esi + 0dh], cl
loc_1_148D:
        test   byte ptr [ebp - 13h], 1
        jne    SHORT loc_1_14BF
        test   word ptr [ebp - 14h], 80h
        je     SHORT loc_1_14A7
        test   word ptr [ebp - 14h], 40h
        jne    SHORT loc_1_14A7
        mov    ah, 6
        jmp    SHORT loc_1_14D1
loc_1_14A7:
        xor    ah, ah
        mov    al, byte ptr [ebp - 14h]
        mov    cl, al
        cmp    al, byte ptr [esi + 0ch]
        je     SHORT loc_1_14BD
        call   sub_1_0851
        jb     SHORT loc_1_14D1
        mov    byte ptr [esi + 0ch], cl
loc_1_14BD:
        jmp    SHORT loc_1_14CE
loc_1_14BF:
        mov    al, byte ptr [ebp - 14h]
        and    al, 7fh
        mov    cl, al
        call   sub_1_0851
        mov    byte ptr [esi + 0ch], cl
loc_1_14CE:
        mov    ah, 0
        clc
loc_1_14D1:
        ret
loc_1_14D2:
        call   sub_1_11A6
        jb     SHORT loc_1_1512
        mov    esi, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp - 1ch]
        call   sub_1_0CE0
        cmp    byte ptr [ebp - 7], 8fh
        je     SHORT loc_1_14F6
        call   sub_1_1657
        mov    word ptr [ebp - 0ch], cx
        mov    al, byte ptr [edi + 12h]
loc_1_14F6:
        mov    byte ptr [ebp + 5], al
        mov    ah, byte ptr [edi + 13h]
        mov    al, byte ptr [edi + 17h]
        or     al, byte ptr [esi + 29h]
        mov    word ptr [ebp - 10h], ax
        mov    al, byte ptr [edi + 0ch]
        mov    ah, byte ptr [edi + 0dh]
        mov    word ptr [ebp - 14h], ax
        mov    ah, 0
loc_1_1512:
        ret
loc_1_1513:
        call   sub_1_1559
        jb     SHORT loc_1_1558
        mov    ah, 15h
        cmp    byte ptr [ebp + 4], 0
        jne    SHORT loc_1_1558
        mov    word ptr [ebp - 10h], 100h
        mov    edi, dword ptr [ebp - 14h]
        movzx  ecx, word ptr [edi]
        lea    ebx, [edi + 2]
        mov    edx, ecx
        jecxz  loc_1_1553
        add    edi, 4
        xor    edx, edx
        mov    esi, dword ptr [ebp - 20h]
        mov    esi, dword ptr [esi + 2]
        add    esi, 8
        xor    edx, edx
        cld
loc_1_1546:
        lodsb
        stosb
        inc    edx
        or     al, al
        je     SHORT loc_1_1553
        loop   loc_1_1546
        mov    byte ptr [edi - 1], 0
loc_1_1553:
        mov    word ptr [ebx], dx
        mov    ah, 0
loc_1_1558:
        ret
sub_1_1559:
        push   ecx
        push   esi
        mov    dword ptr [ebp - 1ch], 0
        mov    esi, dword ptr [ebp - 4]
        cmp    esi, 0
        je     SHORT loc_1_1579
        db  0BEh, 07Ah, 032h, 000h, 000h
        lea    esi, [esi + 2]
        mov    dword ptr [ebp - 20h], esi
        clc
loc_1_1576:
        pop    esi
        pop    ecx
        ret
loc_1_1579:
        mov    ah, 1
        stc
        jmp    SHORT loc_1_1576
loc_1_157E:
        call   sub_1_11A6
        jb     SHORT loc_1_15BB
        mov    ebx, dword ptr [ebp - 1ch]
        call   sub_1_0B2D
        call   sub_1_0CA8
        mov    ah, 14h
        cmp    al, 80h
        jb     SHORT loc_1_15BB
        call   sub_1_09D1
        pushfd
        cli
        call   sub_1_1672
        call   sub_1_0AB8
        popfd
        mov    cx, 2
        call   sub_1_0074
        call   sub_1_0D25
        clc
        mov    ah, 0
loc_1_15BB:
        ret
loc_1_15BC:
        mov    cl, byte ptr [ebp - 0ch]
        cmp    cl, 0bh
        je     SHORT loc_1_15C8
        mov    ah, 15h
        jmp    SHORT loc_1_15CD
loc_1_15C8:
        call   sub_1_1113
loc_1_15CD:
        ret
sub_1_15CE:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        mov    ax, cx
        mov    ah, al
        shr    ah, 4
        xor    ah, al
        test   ah, 0fh
        je     SHORT loc_1_15EA
        mov    ah, 0fh
        stc
        jmp    SHORT loc_1_1651
loc_1_15EA:
        push   ebx
        push   edi
        mov    edi, dword ptr [ebx + 34h]
        add    edi, 2
        mov    esi, dword ptr [ebx + 30h]
        xor    ax, ax
        xor    ebx, ebx
        mov    bl, ch
        cmp    bl, 2
        jb     SHORT loc_1_1606
        mov    ah, 0eh
        stc
        jmp    SHORT loc_1_164F
loc_1_1606:
        shl    bl, 1
        mov    al, byte ptr [ebx + edi]
        mov    ah, 1
        cmp    ah, 0
        jne    SHORT loc_1_161A
        or     ch, ch
        je     SHORT loc_1_161A
        or     cl, cl
        je     SHORT loc_1_1628
loc_1_161A:
        xor    bx, bx
        mov    ch, cl
        shr    ch, 4
        mov    bl, ch
        cmp    bl, byte ptr [esi]
        jb     SHORT loc_1_162D
loc_1_1628:
        mov    ah, 0fh
        stc
        jmp    SHORT loc_1_164F
loc_1_162D:
        shl    bl, 1
        xor    al, byte ptr [ebx + edi + 1]
        pop    edi
        pop    ebx
        call   sub_1_091C
        mov    cx, 9
        call   sub_1_0074
        pop    edx
        pop    ecx
        mov    ebx, dword ptr [ebp - 1ch]
        mov    word ptr [ebx + 18h], cx
        clc
        jmp    SHORT loc_1_1653
loc_1_164F:
        pop    edi
        pop    ebx
loc_1_1651:
        pop    edx
        pop    ecx
loc_1_1653:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_1657:
        push   ebx
        push   esi
        push   edi
        push   edx
        mov    ebx, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp - 20h]
        mov    edi, dword ptr [ebp - 1ch]
        call   sub_1_0FE9
        mov    cx, word ptr [edi + 18h]
        pop    edx
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_1672:
        push   eax
        push   ecx
        push   edx
        push   ebx
        mov    ebx, dword ptr [ebp - 1ch]
        mov    word ptr [ebx + 18h], 111h
        mov    ebx, dword ptr [ebp - 4]
        mov    ax, word ptr [ebx + 48h]
        pop    ebx
        jmp    SHORT loc_1_1699
        db  050h, 051h, 052h, 053h, 08Bh, 05Dh, 0E4h, 066h, 0C7h, 043h, 018h, 000h, 000h, 05Bh, 033h, 0C0h
loc_1_1699:
        push   eax
        call   sub_1_0B2D
        pop    eax
        call   sub_1_091C
        pop    edx
        pop    ecx
        pop    eax
        ret
loc_1_16A9:
        call   sub_1_1559
        jae    SHORT loc_1_16B5
        jmp    NEAR PTR loc_1_1770
loc_1_16B5:
        mov    esi, dword ptr [ebp - 4]
        mov    al, byte ptr [esi + 2ah]
        cmp    byte ptr [ebp - 7], 85h
        jne    SHORT loc_1_16F0
        mov    byte ptr [ebp - 0fh], al
        mov    ax, word ptr [esi + 2ch]
        mov    bx, word ptr [esi + 8]
        cmp    bl, 4
        je     SHORT loc_1_16E5
        cmp    bl, 8
        je     SHORT loc_1_16E5
        cmp    bl, 14h
        je     SHORT loc_1_16E5
        cmp    ax, 1
        jne    SHORT loc_1_16E5
        mov    ax, 0bh
loc_1_16E5:
        mov    word ptr [ebp - 14h], ax
        mov    ah, 0
        jmp    NEAR PTR loc_1_1770
loc_1_16F0:
        mov    di, word ptr [ebp - 14h]
        mov    dx, word ptr [ebp - 10h]
        db  0BBh, 04Ch, 035h, 000h, 000h
        mov    ah, dh
        mov    ecx, 2
loc_1_1704:
        call   sub_1_1379
        jb     SHORT loc_1_176A
        mov    byte ptr [esi + 2ah], al
        add    ebx, 4
        shr    cl, 1
        jae    SHORT loc_1_1704
        test   di, 40h
        jne    SHORT loc_1_1721
        mov    ah, 6
        stc
        jmp    SHORT loc_1_176E
loc_1_1721:
        mov    ax, di
        and    ax, 1fh
        call   sub_1_0F2C
        jb     SHORT loc_1_176E
        and    word ptr [esi + 2ch], 0ff7fh
        and    word ptr [esi + 2ch], 0ffe0h
        or     word ptr [esi + 2ch], ax
        db  0BBh, 02Ch, 035h, 000h, 000h
        mov    ax, word ptr [esi + 2ch]
        mov    dx, di
        mov    ah, dl
        mov    ecx, 80h
loc_1_1751:
        call   sub_1_1379
        jb     SHORT loc_1_176E
        mov    byte ptr [esi + 2ch], al
        add    ebx, 4
        shr    cl, 1
        jae    SHORT loc_1_1751
        mov    byte ptr [esi + 2dh], 0
        mov    ah, 0
        jmp    SHORT loc_1_1770
loc_1_176A:
        mov    ah, 1
        jmp    SHORT loc_1_1770
loc_1_176E:
        mov    ah, 6
loc_1_1770:
        ret
loc_1_1771:
        mov    bl, 0
        call   sub_1_1276
        jb     SHORT loc_1_17CC
        mov    ebx, dword ptr [ebp - 18h]
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 2fh]
        cmp    byte ptr [ebx + 1ah], al
        jae    SHORT loc_1_178D
        mov    ah, 11h
        jmp    SHORT loc_1_17CC
loc_1_178D:
        cmp    byte ptr [ebp + 4], 0
        je     SHORT loc_1_1797
        mov    ah, 8
        jmp    SHORT loc_1_17CC
loc_1_1797:
        mov    esi, dword ptr [ebp - 18h]
        mov    cx, word ptr [ebp - 14h]
        call   sub_1_0FEB
        jae    SHORT loc_1_17A7
        jmp    SHORT loc_1_17CC
loc_1_17A7:
        mov    al, byte ptr [ebp - 10h]
        test   al, 0f8h
        je     SHORT loc_1_17B2
        mov    ah, 2
        jmp    SHORT loc_1_17CC
loc_1_17B2:
        and    byte ptr [ebx + 7], 2
        and    al, 0fdh
        or     byte ptr [ebx + 7], al
        mov    ax, word ptr [ebp - 14h]
        mov    word ptr [ebx + 4], ax
        mov    al, 1
        call   sub_1_05AD
        mov    ah, 0
loc_1_17CC:
        ret
sub_1_17CD:
        push   ebx
        mov    ebx, dword ptr [ebp - 4]
        xor    eax, eax
        cmp    byte ptr [ebx + 2bh], 0
        jne    SHORT loc_1_1802
        jmp    SHORT loc_1_17E7
sub_1_17DB:
        push   ebx
        mov    ebx, dword ptr [ebp - 4]
        xor    eax, eax
        cmp    byte ptr [ebx + 2bh], 1
        jne    SHORT loc_1_1802
loc_1_17E7:
        push   ecx
        mov    ecx, 10h
        lea    ebx, [ebx + 38h]
        jmp    SHORT loc_1_17F5
loc_1_17F2:
        shr    eax, 1
        inc    ebx
loc_1_17F5:
        cmp    byte ptr [ebx], 0
        je     SHORT loc_1_17FF
        or     eax, 8000h
loc_1_17FF:
        loop   loc_1_17F2
        pop    ecx
loc_1_1802:
        pop    ebx
        ret
sub_1_1804:
        cld
        push   word ptr [ebp + 4]
        mov    edi, esp
        sub    esp, 0ah
        call   sub_1_1559
        jae    SHORT loc_1_181A
        jmp    NEAR PTR loc_1_18F0
loc_1_181A:
        xor    ax, ax
        mov    word ptr [ebp - 0ch], ax
        mov    ebx, dword ptr [ebp - 4]
        mov    al, byte ptr [ebx + 26h]
        mov    byte ptr ss:[edi - 8], al
        call   sub_1_0B2D
        mov    word ptr ss:[edi - 0ah], dx
        add    dx, 5
        in     al, dx
        mov    byte ptr ss:[edi - 6], al
        and    al, 8fh
        mov    byte ptr ss:[edi - 5], al
        mov    byte ptr [ebp - 24h], 1
loc_1_1848:
        xor    ax, ax
        mov    byte ptr ss:[edi - 3], al
        mov    word ptr [ebp + 4], ax
        inc    ax
        mov    word ptr ss:[edi - 2], ax
loc_1_185A:
        mov    bl, byte ptr [ebp + 4]
        call   sub_1_11B0
        mov    dx, word ptr ss:[edi - 0ah]
        add    dx, 5
        mov    al, byte ptr [ebp + 4]
        shl    al, 4
        or     al, byte ptr ss:[edi - 5]
        out    dx, al
        add    dx, 7
        in     ax, dx
        and    al, ah
        and    al, 60h
        je     SHORT loc_1_18BC
        mov    byte ptr ss:[edi - 3], al
        test   al, 20h
        je     SHORT loc_1_18AE
        push   ax
        add    dx, -3
        in     al, dx
        and    al, 0c0h
        cmp    al, 0c0h
        jne    SHORT loc_1_189C
        call   sub_1_1960
loc_1_189C:
        mov    dx, word ptr ss:[edi - 0ah]
        add    dx, 0ch
        mov    al, 0ddh
        out    dx, al
        pop    ax
        test   al, 40h
        je     SHORT loc_1_18BC
loc_1_18AE:
        call   sub_1_18F7
        mov    ax, word ptr ss:[edi - 2]
        or     word ptr [ebp - 0ch], ax
loc_1_18BC:
        shl    word ptr ss:[edi - 2], 1
        inc    byte ptr [ebp + 4]
        mov    al, byte ptr [ebp + 4]
        cmp    al, byte ptr ss:[edi - 8]
        jb     SHORT loc_1_185A
        mov    al, byte ptr ss:[edi - 3]
        or     al, al
        jne    NEAR PTR loc_1_1848
        mov    dx, word ptr ss:[edi - 0ah]
        add    dx, 5
        mov    al, byte ptr ss:[edi - 6]
        out    dx, al
        mov    dword ptr [ebp - 1ch], 0
        mov    ah, 0
loc_1_18F0:
        mov    esp, edi
        pop    word ptr [ebp + 4]
        ret
sub_1_18F7:
        mov    dx, word ptr ss:[edi - 0ah]
        mov    esi, dword ptr [ebp - 1ch]
        xor    bl, bl
        add    dx, 8
        in     al, dx
        push   ax
        and    al, 1fh
        or     al, 40h
        out    dx, al
        add    dx, 6
loc_1_1911:
        in     ax, dx
        mov    al, 0bdh
        dec    dx
        dec    dx
        out    dx, al
        mov    al, byte ptr [esi + 15h]
        mov    byte ptr [esi + 15h], ah
        xor    al, ah
        or     bl, al
        test   ah, 80h
        jne    SHORT loc_1_1945
        add    dx, -4
        in     al, dx
        add    al, 80h
        out    dx, al
        add    dx, 6
        in     al, dx
        and    al, 0efh
        out    dx, al
        add    dx, -6
        in     al, dx
        add    al, 80h
        out    dx, al
        add    dx, 4
loc_1_1945:
        in     al, dx
        inc    dx
        inc    dx
        test   al, 40h
        jne    SHORT loc_1_1911
        pop    ax
        add    dx, -6
        out    dx, al
        mov    al, bl
        call   sub_1_0D14
        or     byte ptr [esi + 13h], al
        ret
sub_1_1960:
        push   word ptr [ebp + 4]
        mov    word ptr [ebp + 4], 0
loc_1_196A:
        mov    bl, byte ptr [ebp + 4]
        call   sub_1_11B0
        mov    dx, word ptr ss:[edi - 0ah]
        add    dx, 5
        mov    al, byte ptr [ebp + 4]
        shl    al, 4
        or     al, byte ptr ss:[edi - 5]
        out    dx, al
        add    dx, 4
        mov    al, 40h
        out    dx, al
        add    dx, -3
        in     al, dx
        and    al, 0feh
        out    dx, al
        inc    dx
        inc    dx
        in     al, dx
        push   ax
        and    al, 1fh
        add    al, 0c0h
        out    dx, al
        add    dx, 6
        in     ax, dx
        and    al, 0efh
        out    dx, al
        pop    ax
        add    dx, -6
        out    dx, al
        inc    word ptr [ebp + 4]
        mov    ax, word ptr [ebp + 4]
        cmp    al, byte ptr ss:[edi - 8]
        jb     SHORT loc_1_196A
        xor    al, al
        inc    dx
        out    dx, al
        pop    word ptr [ebp + 4]
        mov    bl, byte ptr [ebp + 4]
        call   sub_1_11B0
        ret
        db  053h, 050h, 066h, 036h, 08Bh, 057h, 0F6h, 08Bh, 05Dh, 0E4h, 00Bh, 0DBh, 074h, 01Bh, 08Bh, 05Dh
        db  0FCh, 080h, 07Bh, 006h, 001h, 077h, 012h, 066h, 083h, 0C2h, 007h, 0ECh, 0A8h, 080h, 075h, 009h
        db  0B7h, 0FEh, 0B3h, 000h, 0E8h, 0A1h, 0F2h, 0FFh, 0FFh, 058h, 05Bh, 0C3h, 000h, 000h, 000h
sub_1_1A00:
        push   esi
        cmp    dword ptr [esp + 0ch], 2
        mov    esi, 13h
        jne    SHORT loc_1_1A43
        db  0C7h, 005h, 0B4h, 037h, 000h, 000h, 000h, 000h, 000h, 000h
        call   sub_1_2EA0
        push   3
        push   0
        db  068h, 049h, 01Ah, 000h, 000h
        push   dword ptr [esp + 14h]
        db  0E8h, 02Eh, 0E6h, 0FEh, 0FFh
        add    esp, 10h
        mov    esi, eax
        push   0
        db  0FFh, 035h, 0B0h, 037h, 000h, 000h
        db  0E8h, 014h, 0E6h, 0FEh, 0FFh
        add    esp, 8
loc_1_1A43:
        mov    eax, esi
        pop    esi
        ret    8
loc_1_1A49:
        sub    esp, 0d8h
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        mov    ebx, dword ptr [esp + 0ech]
        cmp    ebx, 0ah
        ja     NEAR PTR loc_1_1BA7
        db  0FFh, 024h, 09Dh, 0B9h, 01Bh, 000h, 000h
loc_1_1A6C:
        mov    ebx, dword ptr [esp + 0f0h]
        cmp    ebx, 1
        jne    SHORT loc_1_1AEC
        mov    ebp, dword ptr [esp + 0f4h]
        push   ebp
        call   sub_1_1C2F
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_1A93
        mov    edi, 13h
loc_1_1A93:
        or     edi, edi
        jne    NEAR PTR loc_1_1B38
        push   esi
        call   sub_1_0010
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_1AB8
        push   0
        push   esi
        db  0E8h, 060h, 0E5h, 0FEh, 0FFh
        add    esp, 8
        mov    edi, 13h
loc_1_1AB8:
        or     edi, edi
        jne    NEAR PTR loc_1_1BAC
        cmp    ebx, 1
        jne    SHORT loc_1_1B38
        push   esi
        call   sub_1_0092
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        push   0
        push   esi
        jne    SHORT loc_1_1AE2
        push   ebp
        db  0E8h, 0C7h, 0E5h, 0FEh, 0FFh
        add    esp, 0ch
        jmp    SHORT loc_1_1B38
loc_1_1AE2:
        db  0E8h, 029h, 0E5h, 0FEh, 0FFh
        add    esp, 8
        jmp    SHORT loc_1_1B38
loc_1_1AEC:
        mov    esi, dword ptr [esp + 0f8h]
        mov    ebp, dword ptr [esp + 0f4h]
        push   0
        lea    eax, [esp + 14h]
        push   ebp
        push   eax
        db  0E8h, 08Dh, 0E5h, 0FEh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_1B10
        jmp    SHORT loc_1_1B33
loc_1_1B10:
        mov    ax, word ptr [esp + 6eh]
        mov    word ptr [esi + 4], ax
        movzx  ax, byte ptr [esp + 0c0h]
        mov    word ptr [esi + 2ch], ax
        push   esi
        call   sub_1_0010
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_1B38
loc_1_1B33:
        mov    edi, 13h
loc_1_1B38:
        or     edi, edi
        jne    SHORT loc_1_1BAC
        push   esi
        call   sub_1_319C
        add    esp, 4
        jmp    SHORT loc_1_1BAC
loc_1_1B47:
        mov    esi, dword ptr [esp + 0f8h]
        or     esi, esi
        je     SHORT loc_1_1BAC
        push   esi
        call   sub_1_31B6
        add    esp, 4
        push   esi
        call   sub_1_1C0B
        add    esp, 4
        cmp    ebx, 4
        jne    SHORT loc_1_1BAC
        push   0
        push   esi
        db  0E8h, 09Fh, 0E4h, 0FEh, 0FFh
        add    esp, 8
        jmp    SHORT loc_1_1BAC
loc_1_1B76:
        push   dword ptr [esp + 0f8h]
        call   sub_1_1BE5
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_1BAC
        mov    edi, 13h
        jmp    SHORT loc_1_1BAC
loc_1_1B90:
        mov    ebx, dword ptr [esp + 0f0h]
        and    ebx, 7fffffffh
        cmp    ebx, 4
        jb     SHORT loc_1_1BA7
        cmp    ebx, 8
        jbe    SHORT loc_1_1BAC
loc_1_1BA7:
        mov    edi, 1
loc_1_1BAC:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0d8h
        ret
loc_1_1BB9:
        lodsb
        sbb    eax, dword ptr [eax]
        db  000h, 06Ch, 01Ah, 000h
        db  000h, 047h, 01Bh
        add    byte ptr [eax], al
        db  076h, 01Bh
        add    byte ptr [eax], al
        inc    edi
        sbb    eax, dword ptr [eax]
        db  000h, 0A7h, 01Bh, 000h, 000h, 0A7h
        sbb    eax, dword ptr [eax]
        db  000h, 0A7h, 01Bh, 000h, 000h, 090h
        sbb    eax, dword ptr [eax]
        db  000h, 0ACh, 01Bh, 000h, 000h, 0ACh, 01Bh
        add    byte ptr [eax], al
sub_1_1BE5:
        push   ebx
        mov    ecx, dword ptr [esp + 8]
        sub    eax, eax
        mov    bl, byte ptr [ecx + 26h]
        cmp    bl, al
        je     SHORT loc_1_1C09
        mov    edx, dword ptr [ecx + 16h]
        add    edx, 0fh
        movzx  ecx, bl
loc_1_1BFC:
        mov    bl, byte ptr [edx]
        and    ebx, 1
        or     eax, ebx
        add    edx, 1ah
        dec    ecx
        jne    SHORT loc_1_1BFC
loc_1_1C09:
        pop    ebx
        ret
sub_1_1C0B:
        push   esi
        push   80h
        mov    esi, dword ptr [esp + 0ch]
        push   6
        push   esi
        call   sub_1_25EF
        add    esp, 0ch
        push   0
        push   6
        push   esi
        call   sub_1_25EF
        add    esp, 0ch
        pop    esi
        ret
sub_1_1C2F:
        sub    esp, 130h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 1ch], 0ah
        mov    dword ptr [esp + 18h], 32h
        mov    dword ptr [esp + 14h], 0
        mov    edi, dword ptr [esp + 144h]
        push   0
        lea    eax, [esp + 6ch]
        push   edi
        push   eax
        db  0E8h, 02Fh, 0E4h, 0FEh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_1C73
        sub    eax, eax
        jmp    NEAR PTR loc_1_2085
loc_1_1C73:
        mov    ax, word ptr [esp + 0c6h]
        mov    word ptr [esp + 24h], ax
        lea    eax, [esp + 20h]
        push   eax
        call   sub_1_1C0B
        lea    eax, [esp + 24h]
        add    esp, 4
        push   eax
        call   sub_1_2130
        lea    eax, [esp + 24h]
        add    esp, 4
        push   eax
        call   sub_1_2871
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_1CB2
        sub    eax, eax
        jmp    NEAR PTR loc_1_2085
loc_1_1CB2:
        db  068h, 058h, 035h, 000h, 000h
        push   edi
        call   sub_1_2090
        add    esp, 8
        cmp    ax, word ptr [esp + 28h]
        je     SHORT loc_1_1CCF
        mov    dword ptr [esp + 14h], 1
loc_1_1CCF:
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_1_1CEC
        db  068h, 060h, 035h, 000h, 000h
        push   edi
        call   sub_1_2090
        add    esp, 8
        mov    bx, ax
        or     bx, bx
        jne    SHORT loc_1_1D08
loc_1_1CEC:
        lea    eax, [esp + 20h]
        push   eax
        call   sub_1_2837
        add    esp, 4
        mov    bx, ax
        or     bx, bx
        jne    SHORT loc_1_1D08
        sub    eax, eax
        jmp    NEAR PTR loc_1_2085
loc_1_1D08:
        db  068h, 06Ch, 035h, 000h, 000h
        push   edi
        call   sub_1_2090
        mov    word ptr [esp + 18h], ax
        add    esp, 8
        db  068h, 074h, 035h, 000h, 000h
        push   edi
        call   sub_1_2090
        mov    word ptr [esp + 1ah], ax
        add    esp, 8
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_1_1D3D
        cmp    word ptr [esp + 10h], 0
        jne    SHORT loc_1_1D4F
loc_1_1D3D:
        lea    eax, [esp + 20h]
        push   eax
        call   sub_1_2884
        mov    word ptr [esp + 14h], ax
        add    esp, 4
loc_1_1D4F:
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_1_1D5E
        cmp    word ptr [esp + 12h], 0
        jne    SHORT loc_1_1D70
loc_1_1D5E:
        lea    eax, [esp + 20h]
        push   eax
        call   sub_1_289E
        mov    word ptr [esp + 16h], ax
        add    esp, 4
loc_1_1D70:
        lea    eax, [esp + 18h]
        push   1
        push   eax
        lea    eax, [esp + 34h]
        push   eax
        push   3
        db  068h, 080h, 035h, 000h, 000h
        push   0
        push   edi
        db  0E8h, 011h, 0E3h, 0FEh, 0FFh
        add    esp, 1ch
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_1_1D9C
        cmp    dword ptr [esp + 18h], 0
        jne    SHORT loc_1_1DB4
loc_1_1D9C:
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esp + 30h]
        push   eax
        lea    eax, [esp + 28h]
        push   ebx
        push   eax
        call   sub_1_28F0
        add    esp, 10h
loc_1_1DB4:
        movzx  ebp, bx
        imul   ebp, ebp, 1ah
        movzx  eax, word ptr [esp + 12h]
        movzx  ecx, word ptr [esp + 10h]
        add    eax, ecx
        imul   eax, eax, 1eh
        add    eax, dword ptr [esp + 18h]
        lea    eax, [ebp + eax + 56h]
        push   1
        push   eax
        db  0E8h, 02Fh, 0E2h, 0FEh, 0FFh
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_1DE7
        jmp    NEAR PTR loc_1_2083
loc_1_1DE7:
        mov    dword ptr [esi], edi
        mov    ax, word ptr [esp + 28h]
        mov    word ptr [esi + 8], ax
        mov    ax, word ptr [esp + 0c6h]
        mov    word ptr [esi + 4], ax
        movzx  ax, byte ptr [esp + 118h]
        mov    word ptr [esi + 2ch], ax
        push   esi
        call   sub_1_2D8F
        add    esp, 4
        dec    ax
        mov    word ptr [esi + 54h], ax
        mov    dword ptr [esp + 1ch], 10h
        lea    eax, [esp + 1ch]
        push   1
        push   eax
        lea    eax, [esi + 38h]
        push   eax
        push   3
        db  068h, 08Ch, 035h, 000h, 000h
        push   0
        push   edi
        db  0E8h, 060h, 0E2h, 0FEh, 0FFh
        add    esp, 1ch
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_1_1E4D
        cmp    dword ptr [esp + 1ch], 0
        jne    SHORT loc_1_1E56
loc_1_1E4D:
        push   esi
        call   sub_1_2984
        add    esp, 4
loc_1_1E56:
        push   esi
        call   sub_1_28B8
        add    esp, 4
        cmp    eax, 1
        jne    SHORT loc_1_1E6E
        cmp    byte ptr [esi + 43h], 0bh
        jne    SHORT loc_1_1E6E
        mov    byte ptr [esi + 43h], 1
loc_1_1E6E:
        mov    byte ptr [esi + 26h], bl
        mov    al, byte ptr [esp + 12h]
        mov    byte ptr [esi + 2eh], al
        mov    al, byte ptr [esp + 10h]
        lea    ecx, [esi + 56h]
        mov    byte ptr [esi + 2fh], al
        mov    al, byte ptr [esp + 12h]
        add    al, byte ptr [esp + 10h]
        mov    byte ptr [esi + 27h], al
        mov    dword ptr [esi + 30h], ecx
        movzx  eax, word ptr [esp + 2ch]
        lea    ecx, [ecx + eax*2 + 2]
        mov    dword ptr [esi + 34h], ecx
        movzx  eax, word ptr [esp + 2ch]
        movzx  eax, word ptr [esp + eax*2 + 2eh]
        lea    ecx, [ecx + eax*2 + 2]
        mov    dword ptr [esi + 16h], ecx
        add    ecx, ebp
        mov    dword ptr [esi + 1ah], ecx
        lea    ecx, [esp + 2ch]
        sub    ebx, ebx
        cmp    word ptr [esp + 2ch], bx
        jb     SHORT loc_1_1EDB
        sub    edx, edx
loc_1_1EC1:
        mov    ax, word ptr [ecx]
        mov    ebp, dword ptr [esi + 30h]
        mov    word ptr [edx + ebp], ax
        add    ecx, 2
        add    edx, 2
        inc    ebx
        movzx  eax, word ptr [esp + 2ch]
        cmp    eax, ebx
        jge    SHORT loc_1_1EC1
loc_1_1EDB:
        mov    edx, dword ptr [esi + 34h]
        mov    ax, word ptr [ecx]
        mov    word ptr [edx], ax
        add    ecx, 2
        mov    edx, 1
        cmp    ax, dx
        jb     SHORT loc_1_1F11
        mov    ebx, 2
loc_1_1EF6:
        mov    ax, word ptr [ecx]
        mov    ebp, dword ptr [esi + 34h]
        mov    word ptr [ebx + ebp], ax
        add    ecx, 2
        add    ebx, 2
        mov    eax, dword ptr [esi + 34h]
        inc    edx
        movzx  eax, word ptr [eax]
        cmp    eax, edx
        jge    SHORT loc_1_1EF6
loc_1_1F11:
        mov    ax, word ptr [ecx]
        cmp    dword ptr [esp + 14h], 0
        mov    word ptr [esi + 48h], ax
        jne    NEAR PTR loc_1_203E
        db  068h, 0ACh, 035h, 000h, 000h
        push   edi
        call   sub_1_20DB
        add    esp, 8
        lea    ebx, [eax*2]
        db  068h, 0A0h, 035h, 000h, 000h
        push   edi
        call   sub_1_20DB
        add    esp, 8
        add    bx, ax
        add    bx, bx
        db  068h, 094h, 035h, 000h, 000h
        push   edi
        call   sub_1_20DB
        add    esp, 8
        sub    bx, ax
        inc    bx
        shl    bx, 5
        mov    word ptr [esi + 4ah], bx
        db  068h, 0B8h, 035h, 000h, 000h
        push   edi
        call   sub_1_20DB
        add    esp, 8
        lea    ebx, [eax*2 + 18h]
        db  068h, 0C0h, 035h, 000h, 000h
        push   edi
        call   sub_1_20DB
        add    esp, 8
        add    bx, ax
        db  068h, 0C8h, 035h, 000h, 000h
        push   edi
        call   sub_1_20DB
        add    esp, 8
        lea    ebx, [eax + ebx*2]
        db  068h, 0DCh, 035h, 000h, 000h
        push   edi
        call   sub_1_20DB
        add    esp, 8
        lea    ebx, [eax + ebx*2]
        shl    bx, 4
        mov    word ptr [esi + 4ch], bx
        db  068h, 0F0h, 035h, 000h, 000h
        push   edi
        call   sub_1_20DB
        add    esp, 8
        imul   ax, ax, 0c0h
        mov    word ptr [esi + 4eh], ax
        db  068h, 008h, 036h, 000h, 000h
        push   edi
        call   sub_1_20DB
        add    esp, 8
        mov    bx, ax
        shl    bx, 4
        db  068h, 0FCh, 035h, 000h, 000h
        push   edi
        call   sub_1_20DB
        add    esp, 8
        add    bx, ax
        shl    bx, 5
        mov    word ptr [esi + 50h], bx
        db  068h, 024h, 036h, 000h, 000h
        push   edi
        call   sub_1_20DB
        add    esp, 8
        lea    ebx, [eax*2 + 80h]
        db  068h, 014h, 036h, 000h, 000h
        push   edi
        call   sub_1_20DB
        add    esp, 8
        add    bx, ax
        shl    bx, 4
        mov    word ptr [esi + 52h], bx
        mov    ax, bx
        and    ax, 10h
        cmp    ax, 1
        sbb    al, al
        neg    al
        mov    byte ptr [esi + 2bh], al
        jmp    SHORT loc_1_2083
loc_1_203E:
        cmp    word ptr [esi + 54h], 1
        sbb    eax, eax
        and    eax, 0ffffff80h
        add    eax, 0e0h
        mov    word ptr [esi + 4ah], ax
        push   esi
        call   sub_1_239D
        add    esp, 4
        cmp    eax, 1
        cmc
        sbb    eax, eax
        and    eax, 0c0h
        or     ax, 610h
        mov    word ptr [esi + 4ch], ax
        mov    word ptr [esi + 4eh], 0c0h
        mov    word ptr [esi + 50h], 0
        mov    word ptr [esi + 52h], 800h
        mov    byte ptr [esi + 2bh], 1
loc_1_2083:
        mov    eax, esi
loc_1_2085:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 130h
        ret
sub_1_2090:
        sub    esp, 2ch
        push   esi
        mov    dword ptr [esp + 4], 28h
        lea    eax, [esp + 4]
        push   1
        push   eax
        sub    si, si
        lea    eax, [esp + 10h]
        push   eax
        push   1
        push   dword ptr [esp + 48h]
        push   0
        push   dword ptr [esp + 4ch]
        db  0E8h, 0E0h, 0DFh, 0FEh, 0FFh
        add    esp, 1ch
        or     eax, eax
        jne    SHORT loc_1_20D3
        lea    eax, [esp + 8]
        push   eax
        call   sub_1_20F8
        add    esp, 4
        mov    si, ax
loc_1_20D3:
        mov    ax, si
        pop    esi
        add    esp, 2ch
        ret
sub_1_20DB:
        push   dword ptr [esp + 8]
        push   dword ptr [esp + 8]
        call   sub_1_2090
        add    esp, 8
        or     ax, ax
        mov    ax, 1
        jne    SHORT loc_1_20F7
        sub    ax, ax
loc_1_20F7:
        ret
sub_1_20F8:
        mov    ecx, dword ptr [esp + 4]
        sub    ax, ax
        cmp    byte ptr [ecx], al
        je     SHORT loc_1_2121
loc_1_2103:
        mov    dl, byte ptr [ecx]
        cmp    dl, 30h
        jl     SHORT loc_1_2121
        cmp    dl, 39h
        jg     SHORT loc_1_2121
        imul   ax, ax, 0ah
        movsx  dx, dl
        inc    ecx
        lea    eax, [eax + edx - 30h]
        cmp    byte ptr [ecx], 0
        jne    SHORT loc_1_2103
loc_1_2121:
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_2130:
        push   esi
        push   edi
        push   8000h
        mov    esi, dword ptr [esp + 10h]
        push   0e0h
        push   esi
        call   sub_1_266D
        add    esp, 0ch
        push   0c0h
        push   esi
        call   sub_1_26A2
        add    esp, 8
        mov    di, ax
        and    di, 0ff00h
        shr    di, 8
        push   0
        push   0e0h
        push   esi
        call   sub_1_266D
        add    esp, 0ch
        mov    word ptr [esi + 8], di
        mov    ax, di
        pop    edi
        pop    esi
        ret
sub_1_217D:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        mov    esi, dword ptr [esp + 1ch]
        push   2
        push   esi
        call   sub_1_2638
        mov    dword ptr [esp + 1ch], eax
        add    esp, 8
        or     eax, 10000000h
        push   eax
        push   2
        push   esi
        call   sub_1_25A3
        add    esp, 0ch
        push   8
        push   esi
        call   sub_1_260A
        mov    byte ptr [esp + 1bh], al
        add    esp, 8
        push   9
        push   esi
        call   sub_1_260A
        add    esp, 8
        push   7
        push   esi
        call   sub_1_260A
        add    esp, 8
        test   al, 80h
        je     SHORT loc_1_21DD
        mov    edi, 1
        jmp    NEAR PTR loc_1_2305
loc_1_21DD:
        push   9
        push   esi
        call   sub_1_260A
        add    esp, 8
        mov    bl, al
        test   bl, 27h
        je     SHORT loc_1_21FF
        mov    al, bl
        and    al, 0d8h
        push   eax
        push   9
        push   esi
        call   sub_1_25EF
        add    esp, 0ch
loc_1_21FF:
        push   8200h
        push   0e0h
        push   esi
        call   sub_1_266D
        add    esp, 0ch
        push   7
        push   esi
        call   sub_1_260A
        add    esp, 8
        test   al, 20h
        jne    SHORT loc_1_2293
        push   0c0h
        push   esi
        call   sub_1_26A2
        add    esp, 8
        test   al, 20h
        je     SHORT loc_1_2293
        push   esi
        call   sub_1_28D4
        add    esp, 4
        or     eax, eax
        mov    al, bl
        je     SHORT loc_1_2246
        or     al, 27h
        jmp    SHORT loc_1_2248
loc_1_2246:
        or     al, 2
loc_1_2248:
        push   eax
        push   9
        push   esi
        call   sub_1_25EF
        add    esp, 0ch
        sub    ebp, ebp
loc_1_2256:
        push   7
        push   esi
        call   sub_1_260A
        add    esp, 8
        test   al, 20h
        je     SHORT loc_1_2277
        push   0c0h
        push   esi
        call   sub_1_26A2
        add    esp, 8
        test   al, 20h
        je     SHORT loc_1_228E
loc_1_2277:
        push   0
        push   0d6d8h
        db  0E8h, 0E9h, 0DDh, 0FEh, 0FFh
        add    esp, 8
        inc    ebp
        cmp    ebp, 1eh
        jl     SHORT loc_1_2256
        jmp    SHORT loc_1_2293
loc_1_228E:
        mov    edi, 1
loc_1_2293:
        push   0
        push   0e0h
        push   esi
        call   sub_1_266D
        add    esp, 0ch
        push   dword ptr [esp + 13h]
        push   8
        push   esi
        call   sub_1_25EF
        add    esp, 0ch
        push   ebx
        push   9
        push   esi
        call   sub_1_25EF
        add    esp, 0ch
        sub    ebx, ebx
        push   dword ptr [esp + 14h]
        push   2
        push   esi
        call   sub_1_25A3
        add    esp, 0ch
loc_1_22CF:
        push   7
        push   esi
        call   sub_1_260A
        add    esp, 8
        test   al, 20h
        jne    SHORT loc_1_22F0
        push   0c0h
        push   esi
        call   sub_1_26A2
        add    esp, 8
        test   al, 20h
        jne    SHORT loc_1_2305
loc_1_22F0:
        push   0
        push   0d6d8h
        db  0E8h, 070h, 0DDh, 0FEh, 0FFh
        add    esp, 8
        inc    ebx
        cmp    ebx, 2dh
        jl     SHORT loc_1_22CF
loc_1_2305:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  083h, 0ECh, 004h, 053h, 056h, 057h, 055h, 08Bh, 07Ch, 024h, 018h, 02Bh, 0F6h, 066h, 08Bh, 05Fh
        db  004h, 08Dh, 044h, 073h, 010h, 050h, 0E8h, 029h, 00Ah, 000h, 000h, 083h, 0C4h, 004h, 066h, 08Bh
        db  0E8h, 08Dh, 004h, 073h, 050h, 0E8h, 01Ah, 00Ah, 000h, 000h, 083h, 0C4h, 004h, 066h, 03Bh, 0E8h
        db  075h, 058h, 046h, 083h, 0FEh, 008h, 07Ch, 0D9h, 06Ah, 008h, 057h, 0E8h, 0BBh, 002h, 000h, 000h
        db  083h, 0C4h, 008h, 034h, 0E0h, 088h, 044h, 024h, 013h, 0FFh, 074h, 024h, 013h, 06Ah, 008h, 057h
        db  0E8h, 08Bh, 002h, 000h, 000h, 083h, 0C4h, 00Ch, 06Ah, 018h, 057h, 0E8h, 09Bh, 002h, 000h, 000h
        db  083h, 0C4h, 008h, 08Ah, 0D8h, 08Ah, 044h, 024h, 013h, 034h, 0E0h, 050h, 06Ah, 008h, 057h, 0E8h
        db  06Ch, 002h, 000h, 000h, 02Ah, 05Ch, 024h, 01Fh, 083h, 0C4h, 00Ch, 080h, 0FBh, 001h, 01Bh, 0C0h
        db  0F7h, 0D8h, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 004h, 0C3h, 02Bh, 0C0h, 0EBh, 0F4h
sub_1_239D:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        mov    esi, dword ptr [esp + 1ch]
        push   esi
        call   sub_1_2802
        add    esp, 4
        cmp    eax, edi
        je     SHORT loc_1_23C4
        test   byte ptr [eax + 10h], 2
        jne    SHORT loc_1_23C4
        sub    eax, eax
        jmp    NEAR PTR loc_1_259B
loc_1_23C4:
        call   sub_1_2EA0
        db  083h, 03Dh, 0B4h, 037h, 000h, 000h, 000h
        je     NEAR PTR loc_1_2599
        push   0
        push   2
        push   esi
        call   sub_1_25A3
        add    esp, 0ch
        push   8
        push   esi
        call   sub_1_260A
        mov    byte ptr [esp + 1dh], al
        add    esp, 8
        push   6
        push   esi
        call   sub_1_260A
        mov    byte ptr [esp + 1ch], al
        add    esp, 8
        push   0a0h
        push   esi
        call   sub_1_26A2
        mov    word ptr [esp + 1eh], ax
        add    esp, 8
        push   80h
        push   esi
        call   sub_1_26A2
        add    esp, 8
        mov    bx, ax
        push   10h
        push   0a0h
        push   esi
        call   sub_1_266D
        add    esp, 0ch
        push   5678h
        push   80h
        push   esi
        call   sub_1_266D
        add    esp, 0ch
        push   9
        push   esi
        call   sub_1_260A
        mov    byte ptr [esp + 1bh], al
        add    esp, 8
        push   25h
        push   9
        push   esi
        call   sub_1_25EF
        add    esp, 0ch
        push   0
        push   0d6d8h
        db  0E8h, 0FCh, 0DBh, 0FEh, 0FFh
        add    esp, 8
        push   0c0h
        push   esi
        call   sub_1_26A2
        add    esp, 8
        and    ax, 0ffefh
        push   eax
        push   0c0h
        push   esi
        call   sub_1_266D
        add    esp, 0ch
        push   808h
        push   0e0h
        push   esi
        call   sub_1_266D
        add    esp, 0ch
        push   18h
        push   8
        push   esi
        call   sub_1_25EF
        add    esp, 0ch
        push   19h
        push   6
        push   esi
        call   sub_1_25EF
        add    esp, 0ch
        db  08Bh, 02Dh, 080h, 037h, 000h, 000h
        shr    ebp, 0ch
        push   8000h
        push   1
        push   ebp
        push   0
        inc    ebp
        push   esi
        call   sub_1_26D1
        add    esp, 14h
        push   8800h
        push   1
        push   ebp
        push   1
        push   esi
        call   sub_1_26D1
        add    esp, 14h
        db  0A1h, 0B4h, 037h, 000h, 000h
        add    eax, 1000h
        sub    ecx, ecx
        mov    ax, word ptr [eax]
loc_1_2500:
        cmp    ax, 5678h
        je     SHORT loc_1_250C
        cmp    ax, 7878h
        jne    SHORT loc_1_2514
loc_1_250C:
        inc    ecx
        cmp    ecx, 64h
        jl     SHORT loc_1_2500
        jmp    SHORT loc_1_2519
loc_1_2514:
        mov    edi, 1
loc_1_2519:
        push   0
        push   0
        push   0
        push   0
        push   esi
        call   sub_1_26D1
        add    esp, 14h
        push   0
        push   0
        push   0
        push   1
        push   esi
        call   sub_1_26D1
        add    esp, 14h
        push   dword ptr [esp + 16h]
        push   0a0h
        push   esi
        call   sub_1_266D
        add    esp, 0ch
        push   ebx
        push   80h
        push   esi
        call   sub_1_266D
        add    esp, 0ch
        push   0
        push   0e0h
        push   esi
        call   sub_1_266D
        add    esp, 0ch
        push   dword ptr [esp + 15h]
        push   8
        push   esi
        call   sub_1_25EF
        add    esp, 0ch
        push   dword ptr [esp + 14h]
        push   6
        push   esi
        call   sub_1_25EF
        add    esp, 0ch
        push   dword ptr [esp + 13h]
        push   9
        push   esi
        call   sub_1_25EF
        add    esp, 0ch
loc_1_2599:
        mov    eax, edi
loc_1_259B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_25A3:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0ch]
        mov    di, word ptr [esp + 10h]
        add    di, word ptr [eax + 4]
        mov    esi, dword ptr [esp + 14h]
        push   esi
        shr    esi, 10h
        push   edi
        call   sub_1_2D31
        add    esp, 8
        push   esi
        add    di, 2
        push   edi
        call   sub_1_2D31
        add    esp, 8
        pop    edi
        pop    esi
        ret
sub_1_25D4:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [esp + 0ch]
        mov    cx, word ptr [eax + 4]
        add    cx, word ptr [esp + 0ch]
        push   ecx
        call   sub_1_2D31
        add    esp, 8
        ret
sub_1_25EF:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [esp + 0ch]
        mov    cx, word ptr [eax + 4]
        add    cx, word ptr [esp + 0ch]
        push   ecx
        call   sub_1_2D26
        add    esp, 8
        ret
sub_1_260A:
        mov    eax, dword ptr [esp + 4]
        mov    cx, word ptr [eax + 4]
        add    cx, word ptr [esp + 8]
        push   ecx
        call   sub_1_2D3E
        add    esp, 4
        ret
sub_1_2621:
        mov    eax, dword ptr [esp + 4]
        mov    cx, word ptr [eax + 4]
        add    cx, word ptr [esp + 8]
        push   ecx
        call   sub_1_2D53
        add    esp, 4
        ret
sub_1_2638:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0ch]
        mov    si, word ptr [esp + 10h]
        add    si, word ptr [eax + 4]
        push   esi
        call   sub_1_2D53
        add    esp, 4
        movzx  edi, ax
        add    si, 2
        push   esi
        call   sub_1_2D53
        add    esp, 4
        movzx  eax, ax
        shl    eax, 10h
        or     edi, eax
        mov    eax, edi
        pop    edi
        pop    esi
        ret
sub_1_266D:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   8
        push   esi
        call   sub_1_260A
        add    esp, 8
        and    al, 1fh
        or     al, byte ptr [esp + 0ch]
        push   eax
        push   8
        push   esi
        call   sub_1_25EF
        mov    ax, word ptr [esp + 1ch]
        add    esp, 0ch
        push   eax
        push   0eh
        push   esi
        call   sub_1_25D4
        add    esp, 0ch
        pop    esi
        ret
sub_1_26A2:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   8
        push   esi
        call   sub_1_260A
        add    esp, 8
        and    al, 1fh
        or     al, byte ptr [esp + 0ch]
        push   eax
        push   8
        push   esi
        call   sub_1_25EF
        add    esp, 0ch
        push   0eh
        push   esi
        call   sub_1_2621
        add    esp, 8
        pop    esi
        ret
sub_1_26D1:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 18h]
        mov    edi, esi
        imul   di, di, 0ffffh
        and    di, 3fffh
        cmp    word ptr [esp + 1ch], 1
        jne    SHORT loc_1_26F2
        or     esi, 4000h
loc_1_26F2:
        movzx  eax, word ptr [esp + 14h]
        shl    eax, 3
        add    eax, 102h
        or     eax, 8000000h
        push   eax
        mov    ebx, dword ptr [esp + 14h]
        push   2
        push   ebx
        call   sub_1_25A3
        add    esp, 0ch
        push   esi
        push   0
        push   ebx
        call   sub_1_25D4
        add    esp, 0ch
        push   edi
        push   0
        push   ebx
        call   sub_1_25D4
        mov    ax, word ptr [esp + 2ch]
        add    esp, 0ch
        push   eax
        push   0
        push   ebx
        call   sub_1_25D4
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_2741:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        mov    edi, esi
        sub    eax, eax
        mov    ecx, 8
        rep stosd
        mov    edi, dword ptr [esp + 10h]
        push   edi
        call   sub_1_2802
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_27FE
        mov    ecx, dword ptr [eax + 6]
        sub    edx, edx
        cmp    byte ptr [ecx], dl
        je     SHORT loc_1_2783
loc_1_2771:
        movzx  bx, byte ptr [ecx + edx]
        mov    word ptr [esi], bx
        add    esi, 2
        inc    edx
        cmp    byte ptr [ecx + edx], 0
        jne    SHORT loc_1_2771
loc_1_2783:
        test   byte ptr [eax + 10h], 1
        je     SHORT loc_1_27FE
        push   8000300h
        push   2
        push   edi
        call   sub_1_25A3
        add    esp, 0ch
        push   0
        push   edi
        call   sub_1_2621
        add    esp, 8
        test   al, 20h
        je     SHORT loc_1_27B7
        mov    word ptr [esi], 6
        add    esi, 2
        db  0C6h, 005h, 076h, 036h, 000h, 000h, 006h
loc_1_27B7:
        test   al, 40h
        je     SHORT loc_1_27CA
        mov    word ptr [esi], 9
        add    esi, 2
        db  0C6h, 005h, 079h, 036h, 000h, 000h, 009h
loc_1_27CA:
        test   al, 80h
        je     SHORT loc_1_27DD
        mov    word ptr [esi], 0ch
        add    esi, 2
        db  0C6h, 005h, 07Ch, 036h, 000h, 000h, 00Ch
loc_1_27DD:
        test   ah, 1
        je     SHORT loc_1_27EE
        mov    word ptr [esi], 0fh
        db  0C6h, 005h, 07Fh, 036h, 000h, 000h, 00Fh
loc_1_27EE:
        push   8000000h
        push   2
        push   edi
        call   sub_1_25A3
        add    esp, 0ch
loc_1_27FE:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_2802:
        sub    ecx, ecx
        db  066h, 039h, 00Dh, 0CCh, 036h, 000h, 000h
        je     SHORT loc_1_2829
        mov    eax, dword ptr [esp + 4]
        db  0BAh, 0CCh, 036h, 000h, 000h
        mov    ax, word ptr [eax + 8]
loc_1_281A:
        cmp    word ptr [edx], ax
        je     SHORT loc_1_282C
        add    edx, 12h
        inc    ecx
        cmp    word ptr [edx], 0
        jne    SHORT loc_1_281A
loc_1_2829:
        sub    eax, eax
loc_1_282B:
        ret
loc_1_282C:
        imul   ecx, ecx, 12h
        db  08Dh, 081h, 0CCh, 036h, 000h, 000h
        jmp    SHORT loc_1_282B
sub_1_2837:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_2802
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_284E
        sub    ax, ax
        jmp    SHORT loc_1_286F
loc_1_284E:
        cmp    word ptr [eax + 0ah], 2
        jae    SHORT loc_1_285B
        mov    ax, 1
        jmp    SHORT loc_1_286F
loc_1_285B:
        push   esi
        call   sub_1_217D
        add    esp, 4
        cmp    eax, 1
        sbb    ecx, ecx
        add    ecx, 2
        mov    ax, cx
loc_1_286F:
        pop    esi
        ret
sub_1_2871:
        push   dword ptr [esp + 4]
        call   sub_1_2802
        add    esp, 4
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        ret
sub_1_2884:
        push   dword ptr [esp + 4]
        call   sub_1_2802
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_289A
        mov    ax, word ptr [eax + 0ch]
        jmp    SHORT loc_1_289D
loc_1_289A:
        sub    ax, ax
loc_1_289D:
        ret
sub_1_289E:
        push   dword ptr [esp + 4]
        call   sub_1_2802
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_28B4
        mov    ax, word ptr [eax + 0eh]
        jmp    SHORT loc_1_28B7
loc_1_28B4:
        sub    ax, ax
loc_1_28B7:
        ret
sub_1_28B8:
        push   dword ptr [esp + 4]
        call   sub_1_2802
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_28D1
        mov    ax, word ptr [eax + 10h]
        and    eax, 4
        jmp    SHORT loc_1_28D3
loc_1_28D1:
        sub    eax, eax
loc_1_28D3:
        ret
sub_1_28D4:
        push   dword ptr [esp + 4]
        call   sub_1_2802
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_28ED
        mov    ax, word ptr [eax + 10h]
        and    eax, 1
        jmp    SHORT loc_1_28EF
loc_1_28ED:
        sub    eax, eax
loc_1_28EF:
        ret
sub_1_28F0:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        push   esi
        call   sub_1_2802
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_290B
        mov    eax, dword ptr [eax + 2]
        or     eax, eax
        jne    SHORT loc_1_2918
loc_1_290B:
        push   dword ptr [esp + 14h]
        push   esi
        call   sub_1_2C29
        add    esp, 8
loc_1_2918:
        db  066h, 083h, 03Dh, 0A0h, 036h, 000h, 000h, 000h
        jb     SHORT loc_1_2940
        db  00Fh, 0B7h, 00Dh, 0A0h, 036h, 000h, 000h
        inc    ecx
        db  0BEh, 0A0h, 036h, 000h, 000h
        mov    ebx, dword ptr [esp + 18h]
        mov    edi, ebx
        shr    ecx, 1
        rep movsd
        adc    ecx, ecx
        rep movsw
        jmp    SHORT loc_1_2944
loc_1_2940:
        mov    ebx, dword ptr [esp + 18h]
loc_1_2944:
        movzx  ecx, word ptr [eax]
        lea    edx, [ecx + 1]
        or     edx, edx
        jl     SHORT loc_1_2967
        db  00Fh, 0B7h, 015h, 0A0h, 036h, 000h, 000h
        lea    edi, [ebx + edx*2 + 2]
        add    ecx, 2
        mov    esi, eax
        shr    ecx, 1
        rep movsd
        adc    ecx, ecx
        rep movsw
loc_1_2967:
        movzx  ecx, word ptr [eax]
        db  00Fh, 0B7h, 005h, 0A0h, 036h, 000h, 000h
        add    ecx, eax
        mov    eax, dword ptr [esp + 1ch]
        lea    ecx, [ecx*2 + 6]
        mov    dword ptr [eax], ecx
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_2984:
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    word ptr [esp + 18h], 0
        cli
        push   21h
        call   sub_1_2D3E
        add    esp, 4
        mov    byte ptr [esp + 13h], al
        push   0a1h
        call   sub_1_2D3E
        add    esp, 4
        mov    byte ptr [esp + 12h], al
        push   0ffh
        push   21h
        call   sub_1_2D26
        add    esp, 8
        push   0ffh
        push   0a1h
        call   sub_1_2D26
        add    esp, 8
        push   8000000h
        push   2
        mov    esi, dword ptr [esp + 48h]
        push   esi
        call   sub_1_25A3
        add    esp, 0ch
        push   0
        push   esi
        call   sub_1_2621
        add    esp, 8
        mov    word ptr [esp + 1ah], ax
        lea    eax, [esp + 1ch]
        push   eax
        push   esi
        call   sub_1_2741
        add    esp, 8
        lea    edi, [esi + 38h]
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        cmp    word ptr [esp + 1ch], ax
        je     NEAR PTR loc_1_2B8F
        lea    edi, [esp + 1ch]
loc_1_2A1D:
        push   8000000h
        push   2
        push   esi
        call   sub_1_25A3
        add    esp, 0ch
        mov    ax, word ptr [edi]
        push   eax
        push   0
        push   esi
        call   sub_1_25D4
        add    esp, 0ch
        push   0ah
        push   20h
        call   sub_1_2D26
        add    esp, 8
        push   20h
        call   sub_1_2D3E
        add    esp, 4
        movzx  ax, al
        mov    word ptr [esp + 14h], ax
        push   0ah
        push   0a0h
        call   sub_1_2D26
        add    esp, 8
        push   0a0h
        call   sub_1_2D3E
        add    esp, 4
        movzx  bp, al
        shl    bp, 8
        push   8000000h
        push   2
        push   esi
        call   sub_1_25A3
        add    esp, 0ch
        push   0
        push   0
        push   esi
        call   sub_1_25D4
        add    esp, 0ch
        push   0ah
        push   20h
        call   sub_1_2D26
        add    esp, 8
        push   20h
        call   sub_1_2D3E
        add    esp, 4
        movzx  bx, al
        push   0ah
        push   0a0h
        call   sub_1_2D26
        add    esp, 8
        push   0a0h
        call   sub_1_2D3E
        add    esp, 4
        movzx  ax, al
        shl    ax, 8
        add    bx, ax
        push   8000000h
        push   2
        push   esi
        call   sub_1_25A3
        add    esp, 0ch
        mov    ax, word ptr [edi]
        push   eax
        push   0
        push   esi
        call   sub_1_25D4
        add    esp, 0ch
        push   0ah
        push   20h
        call   sub_1_2D26
        add    esp, 8
        push   20h
        call   sub_1_2D3E
        add    esp, 4
        movzx  ax, al
        mov    word ptr [esp + 16h], ax
        push   0ah
        push   0a0h
        call   sub_1_2D26
        add    esp, 8
        push   0a0h
        call   sub_1_2D3E
        add    esp, 4
        movzx  ax, al
        shl    ax, 8
        mov    cx, word ptr [esp + 16h]
        add    cx, ax
        xor    cx, bx
        mov    ax, word ptr [esp + 14h]
        add    ax, bp
        xor    bx, ax
        and    cx, bx
        movzx  edx, cx
        lea    eax, [edx - 1]
        test   edx, eax
        jne    SHORT loc_1_2B82
        or     word ptr [esp + 18h], cx
        sub    ecx, ecx
loc_1_2B69:
        mov    eax, 1
        shl    eax, cl
        test   edx, eax
        jne    SHORT loc_1_2B7C
        inc    ecx
        cmp    ecx, 10h
        jl     SHORT loc_1_2B69
        jmp    SHORT loc_1_2B82
loc_1_2B7C:
        mov    al, byte ptr [edi]
        mov    byte ptr [ecx + esi + 38h], al
loc_1_2B82:
        add    edi, 2
        cmp    word ptr [edi], 0
        jne    NEAR PTR loc_1_2A1D
loc_1_2B8F:
        push   8000000h
        push   2
        push   esi
        call   sub_1_25A3
        add    esp, 0ch
        push   dword ptr [esp + 1ah]
        push   0
        push   esi
        call   sub_1_25D4
        add    esp, 0ch
        push   dword ptr [esp + 18h]
        call   sub_1_0133
        add    esp, 4
        push   dword ptr [esp + 13h]
        push   21h
        call   sub_1_2D26
        add    esp, 8
        push   dword ptr [esp + 12h]
        push   0a1h
        call   sub_1_2D26
        add    esp, 8
        sti
        cmp    word ptr [esp + 18h], 0
        jne    SHORT loc_1_2BF4
        sub    eax, eax
loc_1_2BE4:
        db  08Ah, 088h, 090h, 036h, 000h, 000h
        mov    byte ptr [eax + esi + 38h], cl
        inc    eax
        cmp    eax, 10h
        jl     SHORT loc_1_2BE4
loc_1_2BF4:
        mov    bx, word ptr [esi + 2ch]
        or     bx, bx
        je     SHORT loc_1_2C21
        and    ebx, 0fh
        lea    edi, [esi + ebx + 38h]
        cmp    byte ptr [edi], 0
        jne    SHORT loc_1_2C21
        push   esi
        call   sub_1_28B8
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_2C1F
        cmp    bx, 0bh
        mov    byte ptr [edi], 1
        je     SHORT loc_1_2C21
loc_1_2C1F:
        mov    byte ptr [edi], bl
loc_1_2C21:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 2ch
        ret
sub_1_2C29:
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        db  0BBh, 0A8h, 036h, 000h, 000h
        mov    edi, dword ptr [esp + 10h]
        push   7
        push   edi
        call   sub_1_260A
        add    esp, 8
        test   al, 80h
        je     SHORT loc_1_2C8B
        cmp    word ptr [esp + 14h], 1
        jbe    SHORT loc_1_2C81
        push   2
        push   edi
        call   sub_1_2638
        add    esp, 8
        mov    esi, eax
        xor    esi, 10000000h
        push   esi
        push   2
        push   edi
        call   sub_1_25A3
        add    esp, 0ch
        push   7
        push   edi
        call   sub_1_260A
        add    esp, 8
        test   al, 80h
        je     SHORT loc_1_2C8B
        jmp    NEAR PTR loc_1_2D20
loc_1_2C81:
        db  0B8h, 0A8h, 036h, 000h, 000h
        jmp    NEAR PTR loc_1_2D22
loc_1_2C8B:
        push   0
        push   9
        push   edi
        call   sub_1_25EF
        add    esp, 0ch
        push   8200h
        push   0e0h
        push   edi
        call   sub_1_266D
        add    esp, 0ch
        test   esi, 10000000h
        je     SHORT loc_1_2CB7
        push   10h
        jmp    SHORT loc_1_2CB9
loc_1_2CB7:
        push   8
loc_1_2CB9:
        push   9
        push   edi
        call   sub_1_25EF
        add    esp, 0ch
        sub    esi, esi
loc_1_2CC6:
        push   7
        push   edi
        call   sub_1_260A
        add    esp, 8
        test   al, 20h
        je     SHORT loc_1_2CE7
        push   0c0h
        push   edi
        call   sub_1_26A2
        add    esp, 8
        test   al, 20h
        je     SHORT loc_1_2CFE
loc_1_2CE7:
        push   0
        push   0d6d8h
        db  0E8h, 079h, 0D3h, 0FEh, 0FFh
        add    esp, 8
        inc    esi
        cmp    esi, 1eh
        jl     SHORT loc_1_2CC6
        jmp    SHORT loc_1_2D03
loc_1_2CFE:
        db  0BBh, 0B4h, 036h, 000h, 000h
loc_1_2D03:
        push   0
        push   9
        push   edi
        call   sub_1_25EF
        add    esp, 0ch
        push   0
        push   0e0h
        push   edi
        call   sub_1_266D
        add    esp, 0ch
loc_1_2D20:
        mov    eax, ebx
loc_1_2D22:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_2D26:
        mov    dx, word ptr [esp + 4]
        mov    al, byte ptr [esp + 8]
        out    dx, al
        ret
sub_1_2D31:
        mov    dx, word ptr [esp + 4]
        mov    ax, word ptr [esp + 8]
        out    dx, ax
        ret
sub_1_2D3E:
        sub    esp, 4
        mov    dx, word ptr [esp + 8]
        in     al, dx
        mov    byte ptr [esp + 3], al
        mov    al, byte ptr [esp + 3]
        add    esp, 4
        ret
sub_1_2D53:
        sub    esp, 4
        mov    dx, word ptr [esp + 8]
        in     ax, dx
        mov    word ptr [esp + 2], ax
        mov    ax, word ptr [esp + 2]
        add    esp, 4
        ret
sub_1_2D6B:
        push   esi
        push   42h
        call   sub_1_2D3E
        add    esp, 4
        movzx  esi, al
        push   42h
        call   sub_1_2D3E
        add    esp, 4
        movzx  eax, al
        shl    eax, 8
        or     esi, eax
        mov    eax, esi
        pop    esi
        ret
sub_1_2D8F:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        push   61h
        call   sub_1_2D3E
        mov    byte ptr [esp + 17h], al
        add    esp, 4
        and    al, 0fdh
        or     al, 1
        push   eax
        push   61h
        call   sub_1_2D26
        mov    dword ptr [esp + 1ch], 0ah
        mov    esi, dword ptr [esp + 24h]
        add    esp, 8
loc_1_2DC1:
        push   0b6h
        push   43h
        call   sub_1_2D26
        add    esp, 8
        push   0ffh
        push   42h
        call   sub_1_2D26
        add    esp, 8
        push   0ffh
        push   42h
        call   sub_1_2D26
        add    esp, 8
        push   4000h
        push   20h
        push   esi
        call   sub_1_266D
        add    esp, 0ch
        call   sub_1_2D6B
        mov    ebp, eax
loc_1_2E05:
        call   sub_1_2D6B
        mov    ebx, eax
        cmp    ebp, ebx
        ja     SHORT loc_1_2E05
        push   40h
        push   esi
        call   sub_1_26A2
        add    esp, 8
        and    ax, 0ffh
        push   eax
        push   40h
        push   esi
        call   sub_1_266D
        add    esp, 0ch
loc_1_2E2B:
        push   7
        push   esi
        call   sub_1_260A
        add    esp, 8
        test   al, 4
        je     SHORT loc_1_2E2B
        call   sub_1_2D6B
        sub    ebx, eax
        mov    eax, 400000h
        sub    edx, edx
        div    ebx
        add    edi, eax
        dec    dword ptr [esp + 14h]
        jne    NEAR PTR loc_1_2DC1
        push   0
        push   20h
        push   esi
        call   sub_1_266D
        add    esp, 0ch
        push   dword ptr [esp + 13h]
        push   61h
        call   sub_1_2D26
        add    esp, 8
        mov    ecx, 0ah
        mov    eax, edi
        sub    edx, edx
        div    ecx
        cmp    eax, 400h
        mov    cx, 0
        setbe  cl
        inc    cx
        mov    ax, cx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_2EA0:
        sub    esp, 0ch
        db  083h, 03Dh, 080h, 037h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_2F75
        mov    eax, 1
        db  0C7h, 005h, 098h, 037h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  039h, 005h, 09Ch, 037h, 000h, 000h
        jbe    SHORT loc_1_2EDB
loc_1_2EC7:
        or     eax, eax
        je     SHORT loc_1_2EDB
        add    eax, eax
        db  0D1h, 025h, 098h, 037h, 000h, 000h
        db  039h, 005h, 09Ch, 037h, 000h, 000h
        ja     SHORT loc_1_2EC7
loc_1_2EDB:
        push   0
        lea    eax, [esp + 8]
        push   0
        push   eax
        db  0E8h, 05Bh, 0D1h, 0FEh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_1_2F75
        push   0
        push   dword ptr [esp + 8]
        push   0
        db  068h, 0B0h, 037h, 000h, 000h
        db  0E8h, 046h, 0D1h, 0FEh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_2F75
        push   0
        push   0
        lea    eax, [esp + 8]
        db  0FFh, 035h, 0B0h, 037h, 000h, 000h
        push   eax
        db  0E8h, 053h, 0D1h, 0FEh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_2F75
        push   0
        push   24h
        db  068h, 088h, 037h, 000h, 000h
        push   1
        lea    eax, [esp + 18h]
        push   dword ptr [esp + 10h]
        push   eax
        db  0E8h, 043h, 0D1h, 0FEh, 0FFh
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_1_2F75
        push   0
        push   0
        db  0FFh, 035h, 0B0h, 037h, 000h, 000h
        db  0E8h, 00Dh, 0D1h, 0FEh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_2F75
        push   3
        push   0
        db  068h, 079h, 02Fh, 000h, 000h
        db  0FFh, 035h, 0B0h, 037h, 000h, 000h
        db  0E8h, 0EAh, 0D0h, 0FEh, 0FFh
        add    esp, 10h
loc_1_2F75:
        add    esp, 0ch
        ret
loc_1_2F79:
        sub    esp, 0ch
        cmp    dword ptr [esp + 10h], 1
        je     SHORT loc_1_2F8A
        mov    eax, 1
        jmp    SHORT loc_1_2FFE
loc_1_2F8A:
        push   2
        lea    eax, [esp + 8]
        push   dword ptr [esp + 1ch]
        push   eax
        db  0E8h, 0E2h, 0D0h, 0FEh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_2FFE
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [esp], eax
        lea    eax, [esp + 8]
        push   0
        push   eax
        push   1
        lea    eax, [esp + 0ch]
        push   dword ptr [esp + 10h]
        push   eax
        db  0E8h, 0CCh, 0D0h, 0FEh, 0FFh
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_1_2FFC
        mov    eax, dword ptr [esp]
        push   0
        mov    ecx, dword ptr [eax + 4]
        db  089h, 00Dh, 080h, 037h, 000h, 000h
        mov    eax, dword ptr [eax + 8]
        sub    eax, ecx
        inc    eax
        db  0A3h, 084h, 037h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 018h, 0D0h, 0FEh, 0FFh
        add    esp, 0ch
        cmp    eax, -1
        jne    SHORT loc_1_2FF7
        mov    eax, 13h
        jmp    SHORT loc_1_2FFE
loc_1_2FF7:
        db  0A3h, 0B4h, 037h, 000h, 000h
loc_1_2FFC:
        sub    eax, eax
loc_1_2FFE:
        add    esp, 0ch
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_3010:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], edx
        push   edx
        push   ecx
        push   ebx
        push   edi
        push   esi
        pushfd
        cli
        push   8
        push   dword ptr [ebp - 4]
        call   sub_1_260A
        add    esp, 8
        mov    bl, al
        push   0a0h
        push   dword ptr [ebp - 4]
        call   sub_1_26A2
        add    esp, 8
        and    ax, 3fffh
        push   eax
        push   0a0h
        push   dword ptr [ebp - 4]
        call   sub_1_266D
        add    esp, 0ch
        push   ebx
        push   8
        push   dword ptr [ebp - 4]
        call   sub_1_25EF
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 22h], 0
        popfd
        pop    esi
        pop    edi
        pop    ebx
        pop    ecx
        pop    edx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_3076:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], edx
        push   edx
        push   ecx
        push   ebx
        push   edi
        push   esi
        pushfd
        cli
        sub    si, si
        push   8
        push   dword ptr [ebp - 4]
        call   sub_1_260A
        add    esp, 8
        mov    bl, al
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [eax + 27h]
        or     cl, cl
        je     SHORT loc_1_30B4
        mov    edx, dword ptr [eax + 16h]
        add    edx, 0ah
        movzx  eax, cl
loc_1_30AB:
        or     si, word ptr [edx]
        add    edx, 1ah
        dec    eax
        jne    SHORT loc_1_30AB
loc_1_30B4:
        push   0a0h
        push   dword ptr [ebp - 4]
        call   sub_1_26A2
        add    esp, 8
        or     si, si
        je     SHORT loc_1_3146
        test   ah, 80h
        je     SHORT loc_1_311A
        and    ax, 3fffh
        push   eax
        push   0a0h
        push   dword ptr [ebp - 4]
        call   sub_1_266D
        add    esp, 0ch
        push   ebx
        push   8
        push   dword ptr [ebp - 4]
        call   sub_1_25EF
        add    esp, 0ch
        sti
        mov    esi, dword ptr [ebp - 4]
        add    esi, 22h
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     SHORT loc_1_3104
        push   eax
        db  0E8h, 030h, 0CFh, 0FEh, 0FFh
loc_1_3104:
        push   dword ptr [ebp - 4]
        push   0fa0h
        db  068h, 010h, 030h, 000h, 000h
        db  0E8h, 002h, 0CFh, 0FEh, 0FFh
        mov    dword ptr [esi], eax
        jmp    SHORT loc_1_3186
loc_1_311A:
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [ecx + 22h], 0
        jne    SHORT loc_1_3186
        or     ax, 4000h
        push   eax
        push   0a0h
        push   ecx
        call   sub_1_266D
        add    esp, 0ch
        push   ebx
        push   8
        push   dword ptr [ebp - 4]
        call   sub_1_25EF
        add    esp, 0ch
        jmp    SHORT loc_1_3186
loc_1_3146:
        and    ax, 7fffh
        or     ax, 4000h
        push   eax
        push   0a0h
        push   dword ptr [ebp - 4]
        call   sub_1_266D
        add    esp, 0ch
        push   ebx
        push   8
        push   dword ptr [ebp - 4]
        call   sub_1_25EF
        add    esp, 0ch
        mov    esi, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 22h]
        add    esi, 22h
        or     eax, eax
        je     SHORT loc_1_3186
        push   eax
        db  0E8h, 0B4h, 0CEh, 0FEh, 0FFh
        mov    dword ptr [esi], 0
loc_1_3186:
        sti
        push   dword ptr [ebp - 4]
        call   sub_1_319C
        add    esp, 4
        popfd
        pop    esi
        pop    edi
        pop    ebx
        pop    ecx
        pop    edx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_319C:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        push   1f4h
        db  068h, 076h, 030h, 000h, 000h
        db  0E8h, 067h, 0CEh, 0FEh, 0FFh
        mov    dword ptr [esi + 1eh], eax
        pop    esi
        ret
sub_1_31B6:
        push   esi
        cli
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 1eh]
        or     eax, eax
        je     SHORT loc_1_31D0
        push   eax
        db  0E8h, 06Bh, 0CEh, 0FEh, 0FFh
        mov    dword ptr [esi + 1eh], 0
loc_1_31D0:
        mov    eax, dword ptr [esi + 22h]
        or     eax, eax
        je     SHORT loc_1_31E4
        push   eax
        db  0E8h, 057h, 0CEh, 0FEh, 0FFh
        mov    dword ptr [esi + 22h], 0
loc_1_31E4:
        sti
        pop    esi
        ret
        db  000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 001h, 000h, 000h, 000h, 044h, 042h, 04Bh, 056h
        db  053h, 053h, 044h, 020h, 009h, 000h, 081h, 016h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_3240:
        add    al, 0aeh
        int1
        int1
        inc    eax
        db  003h
loc_1_3246:
        pop    es
        scasb
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 001h, 000h, 000h, 000h, 001h, 000h, 010h, 000h
loc_1_3256:
        push   es
        scasb
        jg     SHORT loc_1_3262
        add    byte ptr [eax], al
        dec    dword ptr [edi]
        add    dword ptr [eax], eax
        add    byte ptr [eax], dl
loc_1_3262:
        add    dword ptr [eax], eax
        add    dword ptr [eax], eax
        add    dword ptr [eax], eax
        db  00Bh, 079h
loc_1_326A:
        db  001h, 0AEh, 084h, 000h, 046h, 032h
        add    byte ptr [eax], al
loc_1_3272:
        db  001h, 0AEh, 083h, 000h, 056h, 032h
        add    byte ptr [eax], al
loc_1_327A:
        db  003h, 0AEh, 001h, 000h, 0C0h, 033h
        add    byte ptr [eax], al
        push   32h
        add    byte ptr [eax], al
        db  072h, 032h
        add    byte ptr [eax], al
        inc    eax
        xor    al, byte ptr [eax]
        add    bl, bl
        inc    ebx
        push   ebx
        push   esi
        add    byte ptr [edx], al
        inc    eax
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 07Ch, 010h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 6ch], al
        popad
        jae    SHORT loc_1_3331
        sub    eax, 534f4942h
        cmp    eax, 61480a31h
        db  072h, 064h
        db  077h, 061h
        jb     SHORT loc_1_333E
        sub    eax, 473d4449h
        outsb
        db  065h, 072h, 069h
        arpl   word ptr [eax], sp
        push   ebx
        db  075h, 070h
        db  070h, 06Fh
        jb     SHORT loc_1_3360
        and    byte ptr [edi + 69h], dl
        outsb
        xor    al, 20h
        push   ebx
        outsd
        arpl   word ptr [ebx + 65h], bp
        je     SHORT loc_1_3319
        push   ebx
        db  065h, 072h, 076h
        imul   esp, dword ptr [ebx + 65h], 480a2073h
        popad
        jb     SHORT loc_1_336B
        db  077h, 061h
        jb     SHORT loc_1_3370
        sub    eax, 69766552h
        db  073h, 069h
        outsd
        outsb
        cmp    eax, 302e3156h
loc_1_3319:
        xor    byte ptr [edx], cl
        dec    eax
        popad
        db  072h, 064h
        ja     SHORT loc_1_3382
        db  072h, 065h
        sub    eax, 65746144h
        cmp    eax, 38302f32h
        das
        cmp    dword ptr [edx + ecx], esi
loc_1_3331:
        inc    edx
        dec    ecx
        dec    edi
        push   ebx
        sub    eax, 69766572h
        jae    SHORT loc_1_33A5
        outsd
        outsb
loc_1_333E:
        cmp    eax, 302f3230h
        cmp    byte ptr [edi], ch
        cmp    dword ptr [edx + ecx], esi
        push   ebx
        outsd
        db  066h, 074h, 077h
        popad
        jb     SHORT loc_1_33B5
        sub    eax, 65746144h
        cmp    eax, 302f3230h
        cmp    byte ptr [edi], ch
        cmp    dword ptr [edx + ecx], esi
        inc    ebx
loc_1_3360:
        outsd
        db  070h, 079h
        db  072h, 069h
        push   6f4e2d74h
loc_1_336B:
        je     SHORT loc_1_33D6
        arpl   word ptr [ebp + 3dh], sp
loc_1_3370:
        inc    ebx
        outsd
        jo     SHORT loc_1_33ED
        db  072h, 069h
        push   63282074h
        sub    dword ptr [eax], esp
        xor    dword ptr [ecx], edi
        cmp    dword ptr [ebx], esi
loc_1_3382:
        and    byte ptr [ecx + 74h], al
        popad
        bound  ebp, qword ptr [edi + 6fh]
        imul   esp, dword ptr [eax], 49h
        outsb
        arpl   word ptr [esi], bp
        or     al, byte ptr [ebx + 61h]
        db  072h, 064h
        sub    eax, 746e6f43h
        db  072h, 06Fh
        insb
        insb
        db  065h, 072h, 03Dh
        inc    esp
        inc    edx
        cmp    byte ptr [esi], dh
loc_1_33A5:
        xor    byte ptr [eax], bh
        xor    cl, byte ptr [edx]
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    eax, dword ptr [ebx]
        add    eax, dword ptr [0b090806h]
loc_1_33B5:
        or     al, 0eh
        movups xmm2, xmmword ptr [edi + edx]
loc_1_33BB:
        push   es
        add    eax, 203h
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [ebp + 78h], al
        inc    ebx
        inc    ecx
        and    byte ptr [esi + 31h], dl
        xor    eax, 50202030h
        inc    ebx
loc_1_33D6:
        dec    ebp
        inc    ebx
        dec    ecx
        inc    ecx
        and    byte ptr [esi + 32h], dl
        xor    dword ptr cs:[eax], esi
        and    byte ptr [eax], ah
        push   ebx
        outsd
        arpl   word ptr [ebx + 65h], bp
        db  074h, 020h
        push   ebx
        jb     NEAR PTR loc_1_3463
loc_1_33ED:
        imul   esp, dword ptr [ebx + 65h], 560a0d73h
        xor    dword ptr [esi], ebp
        xor    byte ptr [edx], dh
        and    byte ptr [eax], dh
        aaa
        das
        xor    dh, byte ptr [esi]
        das
        cmp    dword ptr [ecx + 706f430ah], esi
        db  079h, 072h
        imul   esp, dword ptr [edi + 68h], 43282074h
        sub    dword ptr [eax], esp
        inc    esp
        popad
        db  074h, 061h
        bound  ebp, qword ptr [edi + 6fh]
        imul   esp, dword ptr [eax], 49h
        outsb
        arpl   word ptr [edi + 72h], bp
        db  070h, 06Fh
        db  072h, 061h
        db  074h, 065h
        and    byte ptr fs:[ecx], dh
        cmp    dword ptr [ecx], edi
        xor    byte ptr [202e3439h], ch
        db  000h, 010h
        adc    dword ptr [eax], eax
        db  000h, 010h
        adc    dword ptr [eax], eax
        db  000h, 010h
        adc    dword ptr [eax], eax
        db  000h, 0EEh
        adc    al, byte ptr [eax]
        db  000h, 0D8h
        adc    dword ptr [eax], eax
        db  000h, 0A9h, 016h, 000h, 000h, 0A9h
        push   ss
        add    byte ptr [eax], al
        int3
        or     al, byte ptr [eax]
        db  000h, 04Bh, 00Bh
        add    byte ptr [eax], al
        db  07Ah, 00Bh
        add    byte ptr [eax], al
        mov    bh, 12h
        add    byte ptr [eax], al
        db  071h, 017h
        add    byte ptr [eax], al
        adc    al, 13h
loc_1_3463:
        add    byte ptr [eax], al
        rcl    byte ptr [eax + eax], cl
        db  000h, 0B0h, 013h, 000h, 000h, 0D2h
        adc    al, 0
        db  000h, 07Eh, 015h
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        db  013h, 015h, 000h, 000h, 004h, 018h
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        adc    byte ptr [ecx], dl
        add    byte ptr [eax], al
        db  0BCh, 015h, 000h, 000h, 010h
        adc    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_34F8:
        or     al, byte ptr [edi]
        add    byte ptr [eax], al
        or     al, byte ptr [edi]
        add    byte ptr [eax], al
        or     al, byte ptr [edi]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_350C:
        adc    eax, dword ptr [edi]
        add    byte ptr [eax], al
        adc    eax, dword ptr [edi]
        add    byte ptr [eax], al
        adc    eax, dword ptr [edi]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    eax, dword ptr [edi]
        add    byte ptr [eax], al
        adc    eax, dword ptr [edi]
        add    byte ptr [eax], al
loc_1_352C:
        sub    al, 7
        add    byte ptr [eax], al
        out    0fh, eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_354C:
        sub    al, byte ptr [edi]
        add    byte ptr [eax], al
        pop    ss
        pop    es
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_3558:
        inc    ebx
        dec    eax
        dec    ecx
        push   eax
        dec    ecx
        inc    esp
        add    byte ptr [eax], al
loc_1_3560:
        dec    esi
        push   ebx
        dec    edi
        inc    ebx
        dec    ebx
        inc    ebp
        push   esp
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_356C:
        dec    esi
        dec    ecx
        dec    edi
        push   edi
        dec    ecx
        dec    esi
        push   ebx
        add    byte ptr [esi + 4dh], cl
        inc    ebp
        dec    ebp
        push   edi
        dec    ecx
        dec    esi
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3580:
        push   eax
        push   edi
        push   edx
        push   esp
        inc    ecx
        inc    edx
        dec    esp
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_358C:
        dec    ecx
        push   edx
        push   ecx
        pop    edi
        dec    ebp
        inc    ecx
        push   eax
        add    byte ptr [ebx + 4ch], al
        dec    ebx
        pop    edi
        push   ebx
        pop    ecx
        dec    esi
        inc    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_35A0:
        inc    ebx
        dec    esp
        dec    ebx
        pop    edi
        push   edx
        dec    ecx
        push   ebx
        dec    ecx
        dec    esi
        inc    edi
        add    byte ptr [eax], al
loc_1_35AC:
        inc    edx
        inc    ebx
        dec    esp
        dec    ebx
        pop    edi
        push   ebx
        push   edx
        inc    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_35B8:
        dec    ecx
        dec    edi
        xor    dword ptr [esi], esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_35C0:
        dec    ebp
        inc    ebp
        dec    ebp
        xor    dword ptr [esi], esi
        add    byte ptr [eax], al
        add    byte ptr [ecx + 4fh], cl
        inc    esp
        inc    ebp
        inc    ebx
        dec    edi
        inc    esp
        inc    ebp
        pop    edi
        dec    eax
        inc    ecx
        xor    dh, byte ptr [ebx]
        pop    edi
        xor    dword ptr [edi], esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_35DC:
        dec    ebp
        inc    ebp
        dec    ebp
        inc    esp
        inc    ebp
        inc    ebx
        dec    edi
        inc    esp
        inc    ebp
        pop    edi
        dec    eax
        inc    ecx
        xor    dh, byte ptr [ebx]
        pop    edi
        xor    dword ptr [edx], esi
        add    byte ptr [eax], al
        add    byte ptr [edx + 43h], al
        dec    esp
        dec    ebx
        pop    edi
        push   ebx
        push   edx
        inc    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_35FC:
        inc    edx
        inc    ebx
        dec    esp
        dec    ebx
        pop    edi
        push   ebx
        push   edx
        inc    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3608:
        inc    ebx
        inc    ecx
        dec    esi
        push   ebp
        push   ebx
        inc    ebp
        pop    edi
        dec    eax
        dec    ebx
        inc    ebp
        dec    esi
        add    byte ptr [ecx + 52h], cl
        push   ecx
        pop    edi
        inc    ecx
        inc    ebx
        push   esp
        dec    ecx
        push   esi
        inc    ebp
        pop    edi
        dec    esp
        dec    edi
        add    byte ptr [eax], al
        add    byte ptr [ecx + 52h], cl
        push   ecx
        pop    edi
        push   ebx
        dec    esi
        push   ebx
        pop    edi
        dec    esp
        inc    ebp
        push   esi
        inc    ebp
        dec    esp
        add    byte ptr [eax], al
        db  000h
loc_1_3634:
        add    dword ptr [ebx], eax
        add    al, 5
        push   es
        pop    es
        or     cl, byte ptr [esi]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3640:
        add    eax, dword ptr [eax + 0e0b0a07h]
        add    byte ptr [ecx], al
        add    eax, dword ptr [eax + 0e0a07h]
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        add    al, 5
        push   es
        pop    es
        add    byte ptr [eax], al
        or     al, byte ptr [ecx]
        add    byte ptr [eax], al
        push   cs
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    eax, dword ptr [eax + 6000700h]
        or     al, byte ptr [ecx]
        add    byte ptr [eax], al
        push   cs
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    eax, dword ptr [eax + 700h]
        or     cl, byte ptr [ebx]
loc_1_367C:
        add    byte ptr [eax], al
        push   cs
loc_1_367F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    eax, dword ptr [eax + 700h]
        or     cl, byte ptr [ebx]
        add    byte ptr [eax], al
        push   cs
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    eax, dword ptr [eax + 700h]
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        push   cs
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        db  0E0h, 032h
        db  0E0h, 078h
        pushad
loc_1_36A8:
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], cl
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_36B4:
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        or     dword ptr [eax], ecx
        add    byte ptr [ecx], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_36C0:
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        pop    es
        add    al, byte ptr [eax]
        add    dword ptr [edi], eax
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 034h, 036h
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    al, 0
        or     byte ptr [eax], al
        push   es
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 034h, 036h
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    al, 0
        or     al, byte ptr [eax]
        push   es
        add    byte ptr [edi], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 034h, 036h
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    al, 0
        or     al, byte ptr [eax]
        push   es
        add    byte ptr [edi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 034h, 036h
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    al, 0
        or     al, byte ptr [eax]
        push   es
        add    byte ptr [eax + eax], al
        sal    byte ptr [esi], 0
        db  000h, 040h, 036h
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    al, 0
        or     al, byte ptr [eax]
        add    dword ptr [eax], eax
        or     byte ptr [eax], al
        sal    byte ptr [esi], 0
        db  000h, 040h, 036h
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    al, 0
        or     al, byte ptr [eax]
        add    dword ptr [eax], eax
        adc    al, 0
        sal    byte ptr [esi], 0
        db  000h, 040h, 036h
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    al, 0
        or     al, byte ptr [eax]
        add    dword ptr [eax], eax
        db  015h, 000h, 0B4h, 036h, 000h
        db  000h, 048h, 036h
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    al, 0
        or     al, byte ptr [eax]
        add    al, 0
        db  005h, 000h, 0B4h, 036h, 000h
        db  000h, 048h, 036h
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    al, 0
        or     al, byte ptr [eax]
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3780:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3784:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3788:
        add    dword ptr [eax], eax
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_3798:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_379C:
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
_LTEXT ends
        end
