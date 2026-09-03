; v86mmgr_icode_04.asm -- V86MMGR object 4 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

        db  0BEh, 078h, 006h, 000h, 000h, 0CDh, 020h, 0EFh, 000h, 001h, 000h, 0E9h, 0F0h, 0FFh, 0FBh, 0FFh
sub_4_0010:
        enter  24h, 0
        pushad
        xor    eax, eax
        mov    dword ptr [ebp - 28h], eax
        mov    dword ptr [ebp - 2ch], eax
        db  083h, 03Dh, 008h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_4_016D
        db  083h, 03Dh, 098h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_0047
        db  00Fh, 0B7h, 015h, 0E8h, 002h, 000h, 000h
        or     edx, edx
        jne    NEAR PTR loc_4_0159
        jmp    NEAR PTR loc_4_016D
loc_4_0047:
        lea    esi, [ebp - 28h]
        xor    eax, eax
        push   eax
        push   esi
        push   eax
        db  0CDh, 020h, 074h, 000h, 001h, 000h
        add    esp, 0ch
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        inc    eax
        db  0A3h, 000h, 000h, 000h, 000h
        cmp    eax, 0dfh
        jae    SHORT loc_4_0096
        mov    edx, 0c0h
        cmp    edx, eax
        jae    SHORT loc_4_0086
        mov    edx, eax
        mov    ecx, 0e0h
        sub    ecx, edx
        jbe    SHORT loc_4_0096
        cmp    ecx, 10h
        jb     SHORT loc_4_0096
        jmp    SHORT loc_4_008B
loc_4_0086:
        mov    ecx, 20h
loc_4_008B:
        call   sub_4_0170
        jae    NEAR PTR loc_4_0160
loc_4_0096:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    eax, 0ffh
        jae    SHORT loc_4_00CD
        mov    edx, 0e0h
        cmp    edx, eax
        jae    SHORT loc_4_00BD
        mov    edx, eax
        mov    ecx, 100h
        sub    ecx, edx
        jbe    SHORT loc_4_00CD
        cmp    ecx, 10h
        jb     SHORT loc_4_00CD
        jmp    SHORT loc_4_00C2
loc_4_00BD:
        mov    ecx, 20h
loc_4_00C2:
        call   sub_4_0170
        jae    NEAR PTR loc_4_0160
loc_4_00CD:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    eax, 0bfh
        jae    SHORT loc_4_0100
        mov    edx, 0a0h
        cmp    edx, eax
        jae    SHORT loc_4_00F4
        mov    edx, eax
        mov    ecx, 0c0h
        sub    ecx, edx
        jbe    SHORT loc_4_0100
        cmp    ecx, 10h
        jb     SHORT loc_4_0100
        jmp    SHORT loc_4_00F9
loc_4_00F4:
        mov    ecx, 20h
loc_4_00F9:
        call   sub_4_0170
        jae    SHORT loc_4_0160
loc_4_0100:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    edx, 0a0h
        cmp    edx, eax
        jae    SHORT loc_4_0120
        mov    edx, eax
        mov    ecx, 100h
        sub    ecx, edx
        jbe    SHORT loc_4_012C
        cmp    ecx, 10h
        jb     SHORT loc_4_012C
        jmp    SHORT loc_4_0125
loc_4_0120:
        mov    ecx, 60h
loc_4_0125:
        call   sub_4_0170
        jae    SHORT loc_4_0160
loc_4_012C:
        db  083h, 03Dh, 010h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_016D
        db  08Bh, 015h, 010h, 000h, 000h, 000h
        shr    edx, 8
        db  08Dh, 014h, 055h, 028h, 00Fh, 000h, 000h
        mov    ecx, 10h
loc_4_014A:
        or     byte ptr [edx + 1], 1
        add    edx, 2
        loop   loc_4_014A
        db  08Bh, 015h, 010h, 000h, 000h, 000h
loc_4_0159:
        shr    edx, 8
        mov    dword ptr [ebp - 4], edx
        stc
loc_4_0160:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 28h], eax
        mov    dword ptr [ebp - 2ch], 10h
loc_4_016D:
        popad
        leave
        ret
sub_4_0170:
        db  08Dh, 014h, 055h, 028h, 00Fh, 000h, 000h
        xor    eax, eax
        mov    dword ptr [ebp - 4], eax
loc_4_017C:
        test   byte ptr [edx], 40h
        jne    SHORT loc_4_01B2
        test   word ptr [edx], 0eb6h
        jne    SHORT loc_4_01C5
        push   eax
        push   edx
        db  081h, 0EAh, 028h, 00Fh, 000h, 000h
        shr    edx, 1
        push   edx
        mov    eax, edx
        shr    eax, 5
        and    edx, 1fh
        bt     dword ptr [esi + eax*4], edx
        jb     SHORT loc_4_01BE
        pop    edx
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_4_01B0
        test   dl, 3
        jne    SHORT loc_4_01BA
        mov    dword ptr [ebp - 4], edx
loc_4_01B0:
        pop    edx
        pop    eax
loc_4_01B2:
        inc    eax
        cmp    eax, 10h
        jae    SHORT loc_4_01D1
        jmp    SHORT loc_4_01CA
loc_4_01BA:
        pop    edx
        pop    eax
        jmp    SHORT loc_4_01C5
loc_4_01BE:
        pop    edx
        pop    edx
        or     byte ptr [edx + 1], 8
        pop    eax
loc_4_01C5:
        xor    eax, eax
        mov    dword ptr [ebp - 4], eax
loc_4_01CA:
        add    edx, 2
        loop   loc_4_017C
        stc
        ret
loc_4_01D1:
        clc
        ret
        db  000h
sub_4_01D4:
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        inc    eax
        db  0A3h, 000h, 000h, 000h, 000h
        push   4
        push   50h
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     NEAR PTR loc_4_09D2
        shr    eax, 4
        db  0A3h, 037h, 032h, 000h, 000h
        db  083h, 03Dh, 0D8h, 011h, 000h, 000h, 003h
        cmc
        jae    NEAR PTR loc_4_07E6
        db  08Bh, 035h, 0D8h, 011h, 000h, 000h
        push   0
        push   218ch
        push   esi
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        inc    eax
        je     NEAR PTR loc_4_09D2
        dec    eax
        db  0A3h, 09Ch, 000h, 000h, 000h
        mov    edx, esi
        shr    edx, 0ch
        db  089h, 015h, 03Ch, 032h, 000h, 000h
        movzx  edx, word ptr [eax + 2]
        add    edx, esi
        dec    edx
        shr    edx, 0ch
        db  089h, 015h, 040h, 032h, 000h, 000h
        test   byte ptr [eax], 3
        jne    NEAR PTR loc_4_09D2
        lea    edx, [eax + 0ah]
        mov    esi, edx
        mov    ecx, 40h
loc_4_025F:
        test   byte ptr [esi], 1
        je     SHORT loc_4_0284
        test   byte ptr [esi], 2
        je     SHORT loc_4_0284
        push   6
        sub    esi, edx
        mov    eax, esi
        idiv   byte ptr [esp]
        add    esp, 4
        movzx  esi, al
        shl    esi, 0ah
        db  066h, 089h, 035h, 0E8h, 002h, 000h, 000h
        jmp    SHORT loc_4_0289
loc_4_0284:
        add    esi, 6
        loop   loc_4_025F
loc_4_0289:
        db  08Bh, 035h, 09Ch, 000h, 000h, 000h
        lea    esi, [esi + 0ah]
        db  0BFh, 0A8h, 000h, 000h, 000h
        mov    ecx, 40h
        db  0CDh, 020h, 06Bh, 000h, 001h, 000h
        push   eax
loc_4_02A3:
        push   ecx
        push   edi
        test   byte ptr [esi], 1
        je     SHORT loc_4_02B5
        test   byte ptr [esi], 2
        je     SHORT loc_4_02BE
        or     byte ptr [edi + 2], 10h
        jmp    SHORT loc_4_02BE
loc_4_02B5:
        or     byte ptr [edi + 2], 1
        jmp    NEAR PTR loc_4_038B
loc_4_02BE:
        mov    ecx, edi
        db  0BAh, 0A8h, 000h, 000h, 000h
        sub    ecx, edx
        shr    ecx, 1
        cmp    ecx, dword ptr [esp + 8]
        jae    SHORT loc_4_02E4
loc_4_02CF:
        or     byte ptr [edi + 2], 2
        db  03Bh, 00Dh, 000h, 000h, 000h, 000h
        jb     NEAR PTR loc_4_0363
        jmp    NEAR PTR loc_4_0373
loc_4_02E4:
        mov    al, byte ptr [esi + 1]
        or     al, al
        je     SHORT loc_4_0312
loc_4_02EB:
        mov    byte ptr [edi], al
        mov    byte ptr [edi + 1], 4
        cmp    al, 0ffh
        je     SHORT loc_4_0306
        movzx  eax, word ptr [esi + 2]
        shl    eax, 2
        mov    word ptr [edi + 4], ax
        or     byte ptr [edi + 2], 80h
        jmp    SHORT loc_4_0326
loc_4_0306:
        or     byte ptr [edi + 2], 20h
        mov    word ptr [edi + 4], 0
        jmp    SHORT loc_4_0326
loc_4_0312:
        db  03Bh, 00Dh, 000h, 000h, 000h, 000h
        jb     SHORT loc_4_0326
        cmp    ecx, 0a0h
        jb     SHORT loc_4_02CF
        mov    al, 0ffh
        jmp    SHORT loc_4_02EB
loc_4_0326:
        mov    edi, ecx
        db  03Bh, 00Dh, 000h, 000h, 000h, 000h
        jb     SHORT loc_4_0363
        push   0
        push   0
        push   4
        push   edi
        db  0CDh, 020h, 072h, 000h, 001h, 000h
        add    esp, 10h
        or     eax, eax
        je     NEAR PTR loc_4_09D2
        db  08Dh, 03Ch, 07Dh, 028h, 00Fh, 000h, 000h
        mov    ecx, 4
        mov    eax, 1140h
loc_4_0359:
        or     word ptr [edi], ax
        add    edi, 2
        loop   loc_4_0359
        jmp    SHORT loc_4_0373
loc_4_0363:
        cmp    byte ptr [esi + 1], 0
        je     SHORT loc_4_0373
        cmp    byte ptr [esi + 1], 0ffh
        jne    NEAR PTR loc_4_09D2
loc_4_0373:
        movzx  eax, byte ptr [esi + 4]
        db  08Dh, 090h, 0A8h, 002h, 000h, 000h
        pop    eax
        push   eax
        db  0B9h, 0A8h, 000h, 000h, 000h
        sub    eax, ecx
        shr    eax, 3
        mov    byte ptr [edx], al
loc_4_038B:
        pop    edi
        pop    ecx
        add    edi, 8
        add    esi, 6
        dec    ecx
        jne    NEAR PTR loc_4_02A3
        pop    eax
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        push   101h
        push   692h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     NEAR PTR loc_4_09D2
        mov    dword ptr [esi + ebx + 5ch], eax
        db  0A3h, 098h, 000h, 000h, 000h
        lea    edi, [eax + 240h]
        db  0BEh, 0A8h, 002h, 000h, 000h
        mov    ecx, 10h
        cld
        rep movsd
        lea    edi, [eax + 4]
        db  0BEh, 0A8h, 000h, 000h, 000h
        mov    ecx, 80h
        cld
        rep movsd
        mov    edi, eax
        db  066h, 0A1h, 0E8h, 002h, 000h, 000h
        mov    word ptr [edi + 68ah], ax
        db  0A1h, 09Ch, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 6]
        mov    dword ptr [edi], eax
        lea    edx, [edi + 4]
        mov    ecx, 40h
        xor    eax, eax
loc_4_040A:
        test   byte ptr [edx + 2], 1
        jne    SHORT loc_4_0411
        inc    eax
loc_4_0411:
        add    edx, 8
        loop   loc_4_040A
        mov    byte ptr [edi + 68fh], al
        or     eax, eax
        jne    SHORT loc_4_0427
        db  080h, 00Dh, 004h, 012h, 000h, 000h, 004h
loc_4_0427:
        mov    esi, eax
        shl    eax, 1
        add    eax, esi
        add    eax, 4
        mov    word ptr [edi + 688h], ax
        db  0F6h, 005h, 004h, 012h, 000h, 000h, 004h
        jne    SHORT loc_4_045A
        db  08Bh, 035h, 09Ch, 000h, 000h, 000h
        or     ah, ah
        jne    NEAR PTR loc_4_09D2
        cmp    al, byte ptr [esi + 18ah]
        ja     NEAR PTR loc_4_09D2
loc_4_045A:
        push   1
        push   36h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     NEAR PTR loc_4_09D2
        mov    dword ptr [edi + 280h], eax
        mov    dword ptr [eax], 0ffffffffh
        mov    dword ptr [eax + 4], 0ffffffffh
        mov    byte ptr [eax + 2ch], 44h
        db  08Bh, 035h, 09Ch, 000h, 000h, 000h
        lea    esi, [esi + 18bh]
        cld
        lodsb
        movzx  eax, al
        db  0A3h, 084h, 012h, 000h, 000h
        mov    ecx, 10h
        mul    ecx
        add    esi, eax
        cld
        lodsb
        movzx  ecx, al
        or     ecx, ecx
        je     NEAR PTR loc_4_0620
loc_4_04B1:
        push   ecx
        cmp    byte ptr [esi], 0
        je     NEAR PTR loc_4_0615
        test   byte ptr [esi + 1], 4
        jne    NEAR PTR loc_4_0615
        cmp    byte ptr [esi], 0ffh
        jae    NEAR PTR loc_4_09D2
        movzx  ecx, word ptr [esi + 0ah]
        xor    eax, eax
        jecxz  loc_4_0501
        shl    ecx, 2
        cmp    ecx, 10000h
        ja     NEAR PTR loc_4_09D2
        push   ecx
        shl    ecx, 2
        mov    eax, dword ptr [esi + 0ch]
        push   0
        push   ecx
        push   eax
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        pop    ecx
        inc    eax
        je     NEAR PTR loc_4_09D2
loc_4_0501:
        dec    eax
        mov    edx, eax
        push   edx
        push   ecx
        push   1
        push   36h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    ecx
        pop    edx
        or     eax, eax
        je     NEAR PTR loc_4_09D2
        push   edx
        movzx  edx, byte ptr [esi]
        mov    dword ptr [edi + edx*4 + 280h], eax
        mov    byte ptr [eax + 2dh], dl
        pop    edx
        mov    dword ptr [eax + 28h], ecx
        or     byte ptr [eax + 2ch], 80h
        or     ecx, ecx
        jne    SHORT loc_4_053A
        or     byte ptr [eax + 2ch], 4
loc_4_053A:
        test   byte ptr [esi + 1], 1
        je     SHORT loc_4_0558
        push   ecx
        push   esi
        push   edi
        or     byte ptr [eax + 2ch], 1
        lea    edi, [eax + 2eh]
        lea    esi, [esi + 2]
        mov    ecx, 8
        cld
        rep movsb
        pop    edi
        pop    esi
        pop    ecx
loc_4_0558:
        or     ecx, ecx
        je     NEAR PTR loc_4_0615
        push   edi
        push   esi
        push   edx
        push   eax
        push   ecx
        mov    esi, edx
        xor    edx, edx
        xor    edi, edi
        dec    edi
loc_4_056C:
        cld
        lodsd
        test   al, 1
        je     NEAR PTR loc_4_09D2
        shr    eax, 0ch
        cmp    eax, edi
        je     SHORT loc_4_057E
        inc    edx
loc_4_057E:
        mov    edi, eax
        inc    edi
        loop   loc_4_056C
        push   edx
        shl    edx, 2
        add    edx, 2
        push   101h
        push   edx
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    edx
        or     eax, eax
        je     NEAR PTR loc_4_09D2
        pop    ecx
        pop    edi
        mov    dword ptr [edi], eax
        pop    esi
        mov    edi, eax
        mov    word ptr [edi], dx
        lea    edi, [edi + 2]
        mov    edx, dword ptr [esi]
        shr    edx, 0ch
        mov    word ptr [edi + 2], dx
loc_4_05B7:
        cld
        lodsd
        shr    eax, 0ch
        cmp    eax, edx
        je     SHORT loc_4_05EC
        pushad
        movzx  eax, word ptr [edi + 2]
        shl    eax, 0ch
        movzx  ecx, word ptr [edi]
        shl    ecx, 0ch
        push   0
        push   ecx
        push   eax
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        inc    eax
        popad
        je     NEAR PTR loc_4_09D2
        add    edi, 4
        mov    word ptr [edi + 2], ax
        mov    edx, eax
loc_4_05EC:
        inc    word ptr [edi]
        inc    edx
        loop   loc_4_05B7
        movzx  eax, word ptr [edi + 2]
        shl    eax, 0ch
        movzx  ecx, word ptr [edi]
        shl    ecx, 0ch
        push   0
        push   ecx
        push   eax
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        inc    eax
        je     NEAR PTR loc_4_09D2
        pop    esi
        pop    edi
loc_4_0615:
        pop    ecx
        add    esi, 10h
        dec    ecx
        jne    NEAR PTR loc_4_04B1
loc_4_0620:
        db  0A1h, 09Ch, 000h, 000h, 000h
        cmp    word ptr [eax + 4], 10ah
        jb     NEAR PTR loc_4_07BC
        cld
        lodsd
        db  0A3h, 094h, 000h, 000h, 000h
        cld
        lodsd
        db  0A3h, 094h, 032h, 000h, 000h
        cld
        lodsb
        movzx  ecx, al
        jecxz  loc_4_066B
loc_4_0646:
        mov    edx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_4_0666
        cmp    edx, 0a0h
        jb     SHORT loc_4_0666
        push   ecx
        push   0
        push   eax
        push   edx
        db  0CDh, 020h, 0D2h, 000h, 001h, 000h
        add    esp, 0ch
        pop    ecx
loc_4_0666:
        add    esi, 8
        loop   loc_4_0646
loc_4_066B:
        cld
        lodsb
        movzx  eax, al
        db  0A3h, 07Ch, 012h, 000h, 000h
        mov    ecx, esi
        db  02Bh, 00Dh, 09Ch, 000h, 000h, 000h
        db  089h, 00Dh, 080h, 012h, 000h, 000h
        mov    ecx, 0ch
        mul    ecx
        add    esi, eax
        cld
        lodsb
        movzx  eax, al
        db  0A3h, 088h, 012h, 000h, 000h
        mov    ecx, esi
        db  02Bh, 00Dh, 09Ch, 000h, 000h, 000h
        db  089h, 00Dh, 08Ch, 012h, 000h, 000h
        mov    ecx, 4
        mul    ecx
        add    esi, eax
        db  0A1h, 09Ch, 000h, 000h, 000h
        cmp    word ptr [eax + 4], 10bh
        jb     NEAR PTR loc_4_07BC
        push   esi
        push   edi
        db  0BFh, 044h, 032h, 000h, 000h
        mov    ecx, 1bh
        cld
        repe cmpsb
        jne    SHORT loc_4_06D6
        db  080h, 00Dh, 005h, 012h, 000h, 000h, 004h
loc_4_06D6:
        pop    edi
        pop    esi
        mov    ecx, esi
        db  02Bh, 00Dh, 09Ch, 000h, 000h, 000h
        db  089h, 00Dh, 090h, 012h, 000h, 000h
        lea    esi, [esi + 14h]
        mov    ecx, esi
        db  02Bh, 00Dh, 09Ch, 000h, 000h, 000h
        db  089h, 00Dh, 094h, 012h, 000h, 000h
        lea    esi, [esi + 14h]
        cmp    word ptr [eax + 4], 114h
        jb     NEAR PTR loc_4_07BC
        lodsd
        mov    edx, eax
        lodsd
        or     eax, eax
        je     SHORT loc_4_071D
        push   eax
        push   0
        push   eax
        push   edx
        db  0CDh, 020h, 0D2h, 000h, 001h, 000h
        add    esp, 0ch
        pop    eax
loc_4_071D:
        lodsb
        movzx  ecx, al
        test   ecx, ecx
        je     NEAR PTR loc_4_07BC
loc_4_0729:
        push   ecx
        lodsd
        xchg   edx, eax
        lodsd
        mov    ecx, edx
        neg    ecx
        and    ecx, 0fffh
        add    edx, 0fffh
        and    edx, 0fffff000h
        sub    eax, ecx
        jb     SHORT loc_4_07B4
        shr    eax, 0ch
        je     SHORT loc_4_07B4
        shr    edx, 0ch
        push   eax
        push   edx
        db  03Bh, 015h, 040h, 032h, 000h, 000h
        ja     SHORT loc_4_07A3
        lea    ecx, [eax + edx]
        dec    ecx
        db  03Bh, 00Dh, 03Ch, 032h, 000h, 000h
        jb     SHORT loc_4_07A3
        mov    edx, esi
        db  08Bh, 035h, 09Ch, 000h, 000h, 000h
        sub    edx, esi
        movzx  ecx, word ptr [esi + 2]
        push   ecx
        push   edx
        push   0
        push   ecx
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    edx
        pop    ecx
        test   eax, eax
        je     SHORT loc_4_079C
        push   edi
        mov    edi, eax
        rep movsb
        pop    edi
        lea    esi, [edx + eax]
        db  087h, 005h, 09Ch, 000h, 000h, 000h
        db  0A3h, 0A0h, 000h, 000h, 000h
        jmp    SHORT loc_4_07A3
loc_4_079C:
        add    esi, edx
        add    esp, 8
        jmp    SHORT loc_4_07B4
loc_4_07A3:
        pop    edx
        pop    eax
        push   0
        push   1
        push   eax
        push   edx
        db  0CDh, 020h, 0D2h, 000h, 001h, 000h
        add    esp, 10h
loc_4_07B4:
        pop    ecx
        dec    ecx
        jne    NEAR PTR loc_4_0729
loc_4_07BC:
        push   edi
        call   sub_4_07E7
        pop    edi
        jb     NEAR PTR loc_4_09D2
        db  0F6h, 005h, 004h, 012h, 000h, 000h, 004h
        clc
        jne    SHORT loc_4_07E6
        lea    eax, [edi + 4]
        mov    ecx, 80000001h
        db  0E8h, 094h, 0F8h, 0FBh, 0FFh
        jb     NEAR PTR loc_4_09D2
loc_4_07E6:
        ret
sub_4_07E7:
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        inc    eax
        db  0A3h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 06Ah, 000h, 001h, 000h
        db  0A3h, 0F2h, 00Eh, 000h, 000h
        push   0
        push   eax
        db  0CDh, 020h, 058h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        db  089h, 015h, 0F6h, 00Eh, 000h, 000h
        db  0C7h, 005h, 01Ah, 00Fh, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0C6h, 005h, 01Eh, 00Fh, 000h, 000h, 020h
        db  0C6h, 005h, 01Fh, 00Fh, 000h, 000h, 0FFh
        db  083h, 03Dh, 0D8h, 011h, 000h, 000h, 002h
        jne    SHORT loc_4_0858
        db  0BFh, 04Ch, 030h, 000h, 000h
        xor    esi, esi
        xor    eax, eax
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        cmp    al, 0ffh
        je     SHORT loc_4_0858
        db  00Fh, 0B7h, 035h, 00Ch, 000h, 000h, 000h
        db  003h, 035h, 004h, 000h, 000h, 000h
        xor    eax, eax
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
loc_4_0858:
        xor    eax, eax
        db  0F6h, 005h, 004h, 012h, 000h, 000h, 004h
        jne    SHORT loc_4_0890
        db  0BFh, 06Bh, 030h, 000h, 000h
        xor    esi, esi
        mov    eax, 0ffffh
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        movzx  eax, ax
        add    eax, 3
        shr    eax, 2
        db  083h, 03Dh, 0D8h, 011h, 000h, 000h, 002h
        jbe    SHORT loc_4_0890
        or     eax, eax
        jne    SHORT loc_4_0890
        mov    eax, 10h
loc_4_0890:
        db  0A3h, 008h, 000h, 000h, 000h
        db  083h, 03Dh, 0D8h, 011h, 000h, 000h, 002h
        ja     SHORT loc_4_08CE
        db  0BFh, 073h, 030h, 000h, 000h
        xor    esi, esi
        xor    eax, eax
        db  0CDh, 020h, 0AFh, 000h, 001h, 000h
        or     eax, eax
        je     SHORT loc_4_08CE
        add    eax, 3ffh
        and    eax, 0fffffc00h
        cmp    eax, 0a000h
        jb     SHORT loc_4_08CE
        cmp    eax, 0f000h
        ja     SHORT loc_4_08CE
        db  0A3h, 010h, 000h, 000h, 000h
loc_4_08CE:
        db  0BFh, 05Fh, 030h, 000h, 000h
        xor    esi, esi
        db  00Fh, 0B6h, 005h, 03Bh, 032h, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        db  0A2h, 03Bh, 032h, 000h, 000h
        cmp    al, 0ffh
        jne    SHORT loc_4_08F2
        db  080h, 00Dh, 005h, 012h, 000h, 000h, 008h
loc_4_08F2:
        db  08Bh, 03Dh, 037h, 032h, 000h, 000h
        shl    edi, 4
        push   edi
        add    edi, dword ptr [ebx + 4]
        db  0BEh, 0ECh, 031h, 000h, 000h
        mov    ecx, 50h
        cld
        rep movsb
        pop    edi
        db  080h, 03Dh, 03Bh, 032h, 000h, 000h, 0FFh
        je     NEAR PTR loc_4_09D0
        db  083h, 03Dh, 084h, 012h, 000h, 000h, 000h
        jne    SHORT loc_4_0931
        push   ebx
        xor    ebx, ebx
        mov    eax, edi
        db  0CDh, 020h, 005h, 000h, 015h, 000h
        pop    ebx
        jb     SHORT loc_4_0999
loc_4_0931:
        db  0A1h, 037h, 032h, 000h, 000h
        shl    eax, 10h
        db  066h, 0A1h, 0F2h, 031h, 000h, 000h
        db  0BEh, 0CBh, 012h, 000h, 000h
        xor    edx, edx
        db  0CDh, 020h, 009h, 000h, 001h, 000h
        jb     SHORT loc_4_0999
        db  066h, 0A1h, 0F4h, 031h, 000h, 000h
        db  0BEh, 0E7h, 012h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 001h, 000h
        jb     SHORT loc_4_0999
        db  066h, 0A1h, 0FEh, 031h, 000h, 000h
        db  0BEh, 0FCh, 012h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 001h, 000h
        jb     SHORT loc_4_0999
        mov    ax, 3dh
        db  0A3h, 014h, 000h, 000h, 000h
        db  0BEh, 093h, 027h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 001h, 000h
        jb     SHORT loc_4_0999
        mov    ax, 41h
        db  0BEh, 021h, 013h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 001h, 000h
loc_4_0999:
        jb     SHORT loc_4_09D2
        mov    eax, 67h
        db  0BEh, 031h, 013h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        mov    edi, dword ptr [ebx + 4]
        mov    eax, dword ptr [edi + 19ch]
        db  0A3h, 004h, 000h, 000h, 000h
        db  0A1h, 037h, 032h, 000h, 000h
        shl    eax, 10h
        mov    ax, 40h
        db  0A3h, 018h, 000h, 000h, 000h
        mov    dword ptr [edi + 19ch], eax
loc_4_09D0:
        clc
        ret
loc_4_09D2:
        db  00Fh, 0B7h, 035h, 014h, 000h, 000h, 000h
        db  003h, 035h, 004h, 000h, 000h, 000h
        xor    eax, eax
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
sub_4_09E7:
        db  0A1h, 094h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_09FF
        db  0BEh, 029h, 013h, 000h, 000h
        xor    edx, edx
        db  0CDh, 020h, 009h, 000h, 001h, 000h
        jb     SHORT loc_4_09D2
loc_4_09FF:
        db  080h, 03Dh, 03Bh, 032h, 000h, 000h, 0FFh
        je     SHORT loc_4_0A29
        db  083h, 03Dh, 084h, 012h, 000h, 000h, 000h
        je     SHORT loc_4_0A29
        db  0A1h, 037h, 032h, 000h, 000h
        shl    eax, 4
        or     eax, eax
        je     SHORT loc_4_0A29
        push   ebx
        xor    ebx, ebx
        db  0CDh, 020h, 005h, 000h, 015h, 000h
        pop    ebx
        jb     SHORT loc_4_09D2
loc_4_0A29:
        ret
sub_4_0A2A:
        db  080h, 03Dh, 03Bh, 032h, 000h, 000h, 001h
        jne    NEAR PTR loc_4_0B0B
        db  083h, 03Dh, 098h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_0A52
        db  0F6h, 005h, 004h, 012h, 000h, 000h, 004h
        jne    NEAR PTR loc_4_0AF0
        jmp    NEAR PTR loc_4_0B0B
loc_4_0A52:
        db  083h, 03Dh, 010h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_4_0B0B
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        inc    eax
        add    eax, 3
        and    eax, 0fffffffch
        cmp    eax, 0a0h
        jae    SHORT loc_4_0A78
        mov    eax, 0a0h
loc_4_0A78:
        mov    ecx, 100h
        sub    ecx, eax
        jbe    SHORT loc_4_0AF0
        sub    esp, 24h
        mov    esi, esp
        push   eax
        push   ecx
        xor    eax, eax
        push   eax
        push   esi
        push   eax
        db  0CDh, 020h, 074h, 000h, 001h, 000h
        add    esp, 0ch
        pop    ecx
        pop    eax
        db  08Dh, 004h, 045h, 028h, 00Fh, 000h, 000h
        xor    edx, edx
loc_4_0AA1:
        test   byte ptr [eax], 40h
        jne    SHORT loc_4_0AC6
        test   word ptr [eax], 0eb6h
        jne    SHORT loc_4_0AE6
        push   eax
        push   edx
        db  02Dh, 028h, 00Fh, 000h, 000h
        shr    eax, 1
        mov    edx, eax
        shr    eax, 5
        and    edx, 1fh
        bt     dword ptr [esi + eax*4], edx
        pop    edx
        pop    eax
        jb     SHORT loc_4_0AE6
loc_4_0AC6:
        or     edx, edx
        jne    SHORT loc_4_0ADB
        mov    edx, eax
        db  081h, 0EAh, 028h, 00Fh, 000h, 000h
        shr    edx, 1
        test   dl, 3
        jne    SHORT loc_4_0AE6
        xor    ebx, ebx
loc_4_0ADB:
        inc    ebx
        cmp    ebx, 10h
        jb     SHORT loc_4_0AE8
        add    esp, 24h
        jmp    SHORT loc_4_0B0B
loc_4_0AE6:
        xor    edx, edx
loc_4_0AE8:
        add    eax, 2
        loop   loc_4_0AA1
        add    esp, 24h
loc_4_0AF0:
        db  0A1h, 037h, 032h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_0B0B
        shl    eax, 4
        add    eax, 0ah
        mov    byte ptr [eax], 26h
        db  080h, 00Dh, 005h, 012h, 000h, 000h, 001h
loc_4_0B0B:
        ret
sub_4_0B0C:
        db  083h, 03Dh, 0D8h, 011h, 000h, 000h, 002h
        ja     SHORT loc_4_0B1A
        call   sub_4_07E7
loc_4_0B1A:
        db  0A1h, 037h, 032h, 000h, 000h
        shl    eax, 10h
        mov    ax, 45h
        db  0A3h, 098h, 012h, 000h, 000h
        mov    ax, 4ah
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 001h, 000h
        jb     SHORT loc_4_0B73
        shr    eax, 10h
        shl    eax, 4
        add    eax, 45h
        db  0A3h, 0E0h, 031h, 000h, 000h
        db  0C7h, 005h, 0E4h, 031h, 000h, 000h, 006h, 000h, 000h, 000h
        db  0C7h, 005h, 0E8h, 031h, 000h, 000h, 000h, 002h, 000h, 000h
        push   0
        db  068h, 0D8h, 031h, 000h, 000h
        db  0CDh, 020h, 0A6h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        jne    SHORT loc_4_0B88
loc_4_0B73:
        db  00Fh, 0B7h, 035h, 00Eh, 000h, 000h, 000h
        db  003h, 035h, 004h, 000h, 000h, 000h
        xor    eax, eax
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
loc_4_0B88:
        db  080h, 03Dh, 03Bh, 032h, 000h, 000h, 0FFh
        je     SHORT loc_4_0BAF
        db  0BEh, 0E4h, 013h, 000h, 000h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        mov    ecx, eax
        movzx  edx, cx
        shr    ecx, 10h
        mov    eax, 67h
        db  0CDh, 020h, 045h, 000h, 001h, 000h
loc_4_0BAF:
        clc
        ret
        db  000h, 000h, 000h
V86MMGR_Set_Mapping_Info:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_4_0BF9
        db  03Ah, 005h, 060h, 032h, 000h, 000h
        jbe    SHORT loc_4_0BC9
        db  0A2h, 060h, 032h, 000h, 000h
loc_4_0BC9:
        db  03Ah, 025h, 061h, 032h, 000h, 000h
        jbe    SHORT loc_4_0BD7
        db  088h, 025h, 061h, 032h, 000h, 000h
loc_4_0BD7:
        db  000h, 01Dh, 062h, 032h, 000h, 000h
        db  03Ah, 03Dh, 063h, 032h, 000h, 000h
        jbe    SHORT loc_4_0BEB
        db  088h, 03Dh, 063h, 032h, 000h, 000h
loc_4_0BEB:
        db  03Ah, 00Dh, 064h, 032h, 000h, 000h
        jbe    SHORT loc_4_0BF9
        db  088h, 00Dh, 064h, 032h, 000h, 000h
loc_4_0BF9:
        ret
sub_4_0BFA:
        pushad
        db  00Fh, 0B6h, 035h, 060h, 032h, 000h, 000h
        db  00Fh, 0B6h, 005h, 063h, 032h, 000h, 000h
        add    esi, eax
        db  0A0h, 062h, 032h, 000h, 000h
        add    esi, eax
        db  00Fh, 0B6h, 03Dh, 061h, 032h, 000h, 000h
        db  00Fh, 0B6h, 005h, 064h, 032h, 000h, 000h
        add    edi, eax
        db  0A0h, 062h, 032h, 000h, 000h
        add    edi, eax
        cmp    edi, esi
        jae    SHORT loc_4_0C2F
        mov    edi, esi
loc_4_0C2F:
        db  089h, 03Dh, 065h, 032h, 000h, 000h
        call   sub_4_0010
        jecxz  loc_4_0C42
        jae    SHORT loc_4_0C42
        xor    ecx, ecx
        xor    eax, eax
loc_4_0C42:
        push   eax
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        inc    eax
        xor    edx, edx
loc_4_0C4C:
        db  066h, 0F7h, 004h, 045h, 028h, 00Fh, 000h, 000h, 086h, 00Fh
        jne    SHORT loc_4_0C59
        inc    edx
loc_4_0C59:
        inc    eax
        cmp    eax, 0ffh
        jbe    SHORT loc_4_0C4C
        pop    eax
        sub    edx, ecx
        je     SHORT loc_4_0C79
        cmp    edi, edx
        jbe    NEAR PTR loc_4_0D22
        cmp    esi, edx
        ja     SHORT loc_4_0CA5
        mov    edi, edx
        jmp    NEAR PTR loc_4_0D22
loc_4_0C79:
        push   eax
        push   ecx
        push   808h
        push   0
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        mov    edx, eax
        pop    ecx
        pop    eax
        shr    edx, 0ch
        xchg   edi, esi
        cmp    edx, esi
        jae    SHORT loc_4_0C9F
        xchg   edi, esi
        cmp    edx, esi
        jb     SHORT loc_4_0CA3
        mov    esi, edx
loc_4_0C9F:
        xor    edi, edi
        jmp    SHORT loc_4_0CDA
loc_4_0CA3:
        xor    edx, edx
loc_4_0CA5:
        cmp    ecx, 10h
        jbe    SHORT loc_4_0CB9
        lea    edi, [ecx + edx - 10h]
        cmp    edi, esi
        jb     SHORT loc_4_0CB9
        mov    ecx, 10h
        jmp    SHORT loc_4_0D22
loc_4_0CB9:
        mov    edi, edx
        pushad
        xor    eax, eax
        xor    esi, esi
        db  0BFh, 040h, 031h, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        test   eax, eax
        popad
        jne    SHORT loc_4_0CDA
        add    edi, ecx
        xor    eax, eax
        xor    ecx, ecx
        cmp    edi, esi
        jae    SHORT loc_4_0D22
loc_4_0CDA:
        pushad
        sub    esi, edi
        shl    esi, 0ch
        push   408h
        push   esi
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        test   eax, eax
        je     NEAR PTR loc_4_0E34
        shr    eax, 0ch
        db  0A3h, 0B4h, 00Eh, 000h, 000h
        shr    esi, 0ch
        db  089h, 035h, 0BCh, 00Eh, 000h, 000h
loc_4_0D07:
        db  080h, 00Ch, 045h, 029h, 00Fh, 000h, 000h, 014h
        inc    eax
        dec    esi
        jne    SHORT loc_4_0D07
        dec    eax
        db  0A3h, 0B8h, 00Eh, 000h, 000h
        popad
        test   edi, edi
        je     NEAR PTR loc_4_0DAE
loc_4_0D22:
        push   eax
        push   ecx
        jecxz  loc_4_0D31
loc_4_0D26:
        db  080h, 00Ch, 045h, 029h, 00Fh, 000h, 000h, 001h
        inc    eax
        loop   loc_4_0D26
loc_4_0D31:
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        inc    eax
loc_4_0D38:
        db  066h, 0F7h, 004h, 045h, 028h, 00Fh, 000h, 000h, 086h, 00Fh
        jne    SHORT loc_4_0D8F
        db  0F6h, 004h, 045h, 029h, 00Fh, 000h, 000h, 010h
        jne    SHORT loc_4_0D64
        push   eax
        push   0
        push   0
        push   1
        push   eax
        db  0CDh, 020h, 072h, 000h, 001h, 000h
        add    esp, 10h
        test   eax, eax
        pop    eax
        je     SHORT loc_4_0D8F
loc_4_0D64:
        db  080h, 00Ch, 045h, 029h, 00Fh, 000h, 000h, 014h
        db  0FFh, 005h, 0BCh, 00Eh, 000h, 000h
        db  039h, 005h, 0B8h, 00Eh, 000h, 000h
        jg     SHORT loc_4_0D7F
        db  0A3h, 0B8h, 00Eh, 000h, 000h
loc_4_0D7F:
        db  039h, 005h, 0B4h, 00Eh, 000h, 000h
        jb     SHORT loc_4_0D8C
        db  0A3h, 0B4h, 00Eh, 000h, 000h
loc_4_0D8C:
        dec    edi
        je     SHORT loc_4_0D97
loc_4_0D8F:
        inc    eax
        cmp    eax, 0ffh
        jbe    SHORT loc_4_0D38
loc_4_0D97:
        pop    ecx
        pop    eax
        or     edi, edi
        jne    NEAR PTR loc_4_0C79
        jecxz  loc_4_0DAE
loc_4_0DA3:
        db  080h, 024h, 045h, 029h, 00Fh, 000h, 000h, 0FEh
        inc    eax
        loop   loc_4_0DA3
loc_4_0DAE:
        db  08Bh, 00Dh, 0BCh, 00Eh, 000h, 000h
        db  089h, 00Dh, 0C0h, 00Eh, 000h, 000h
        db  00Fh, 0B6h, 00Dh, 060h, 032h, 000h, 000h
        db  08Bh, 03Dh, 065h, 032h, 000h, 000h
        db  03Bh, 03Dh, 0BCh, 00Eh, 000h, 000h
        ja     SHORT loc_4_0DD6
        db  00Fh, 0B6h, 00Dh, 061h, 032h, 000h, 000h
loc_4_0DD6:
        db  088h, 00Dh, 08Bh, 006h, 000h, 000h
        shl    ecx, 0ch
        xor    esi, esi
        db  0CDh, 020h, 00Dh, 000h, 006h, 000h
        jae    SHORT loc_4_0E17
        db  00Fh, 0B6h, 00Dh, 060h, 032h, 000h, 000h
        db  088h, 00Dh, 08Bh, 006h, 000h, 000h
        shl    ecx, 0ch
        xor    esi, esi
        db  0CDh, 020h, 00Dh, 000h, 006h, 000h
        jae    SHORT loc_4_0E17
        push   408h
        push   ecx
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        test   eax, eax
        je     SHORT loc_4_0E34
        mov    edi, eax
loc_4_0E17:
        shr    edi, 0ch
        mov    eax, edi
        db  0A2h, 08Ah, 006h, 000h, 000h
        db  0E8h, 0B2h, 0F7h, 0FBh, 0FFh
        db  0BEh, 060h, 003h, 000h, 000h
        db  0CDh, 020h, 02Ah, 000h, 001h, 000h
        popad
        clc
        ret
loc_4_0E34:
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
        add    byte ptr [eax], al
sub_4_0E3C:
        enter  28h, 0
        pushad
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  00Fh, 0B7h, 005h, 074h, 032h, 000h, 000h
        xor    esi, esi
        db  0BFh, 0AEh, 031h, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        or     eax, eax
        je     SHORT loc_4_0EA6
        cmp    eax, 400h
        jae    SHORT loc_4_0E7B
        db  066h, 0A3h, 074h, 032h, 000h, 000h
loc_4_0E7B:
        push   80h
        push   0
        push   0
        push   0
        push   0
        push   0
        push   1
        push   1
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        test   eax, eax
        je     SHORT loc_4_0EA6
        db  0A3h, 06Ch, 032h, 000h, 000h
        db  089h, 015h, 070h, 032h, 000h, 000h
loc_4_0EA6:
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        inc    eax
        db  0A3h, 000h, 000h, 000h, 000h
        xor    eax, eax
        mov    dword ptr [ebp - 4], eax
        push   ebx
        db  0CDh, 020h, 0BBh, 000h, 001h, 000h
        mov    edx, ebx
        pop    ebx
        test   edx, 40000h
        je     SHORT loc_4_0EE0
        or     byte ptr [ebp - 4], 1
        db  068h, 0A4h, 028h, 000h, 000h
        db  068h, 028h, 00Fh, 000h, 000h
        call   sub_4_1665
        pop    ecx
        pop    ecx
        jmp    SHORT loc_4_0F24
loc_4_0EE0:
        test   edx, 80000h
        je     SHORT loc_4_0F24
        or     byte ptr [ebp - 4], 2
        push   0
        push   940h
        db  0CDh, 020h, 0A9h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     SHORT loc_4_0F24
        shr    eax, 4
        db  0A3h, 076h, 032h, 000h, 000h
        mov    edx, dword ptr [ebx + 8]
        push   ebx
        push   edx
        push   eax
        shl    eax, 4
        push   eax
        db  068h, 028h, 00Fh, 000h, 000h
        call   sub_4_174F
        add    esp, 14h
        db  0CDh, 020h, 0AAh, 000h, 001h, 000h
loc_4_0F24:
        lea    edx, [ebp - 28h]
        push   0
        push   edx
        push   0
        db  0CDh, 020h, 074h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_4_11D2
        test   byte ptr [ebp - 4], 1
        jne    SHORT loc_4_0F76
        xor    eax, eax
        xor    esi, esi
        db  0BFh, 0CFh, 031h, 000h, 000h
        xor    edx, edx
        db  0CDh, 020h, 0B3h, 000h, 001h, 000h
        or     edx, edx
        je     SHORT loc_4_0F5E
        cmp    byte ptr [edx], 0
        je     SHORT loc_4_0F5E
        dec    eax
loc_4_0F5E:
        xor    esi, esi
        db  0BFh, 0BFh, 031h, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        or     eax, eax
        je     SHORT loc_4_0F76
        push   ebx
        call   sub_4_1941
        pop    ebx
loc_4_0F76:
        push   ebx
        db  0BFh, 080h, 030h, 000h, 000h
        mov    edx, 0ffffffbfh
        mov    ebx, 80h
        mov    eax, 4000h
        call   sub_4_11D5
        db  0BFh, 096h, 030h, 000h, 000h
        mov    edx, 0ffffffbfh
        mov    ebx, 80h
        mov    eax, 1000h
        call   sub_4_11D5
        db  0BFh, 08Bh, 030h, 000h, 000h
        mov    edx, 0ffffff7fh
        mov    ebx, 40h
        mov    eax, 4000h
        call   sub_4_11D5
        db  0BFh, 0A7h, 030h, 000h, 000h
        mov    edx, 0ffffff7fh
        mov    ebx, 40h
        mov    eax, 1000h
        call   sub_4_11D5
        mov    al, 78h
        db  0CDh, 020h, 0DCh, 000h, 001h, 000h
        jne    SHORT loc_4_100F
        mov    ecx, 100h
        db  0A1h, 000h, 000h, 000h, 000h
        sub    ecx, eax
        jbe    SHORT loc_4_100F
        db  08Dh, 004h, 045h, 028h, 00Fh, 000h, 000h
        mov    edx, 0ffffffbfh
        mov    ebx, 80h
loc_4_1004:
        and    word ptr [eax], dx
        or     word ptr [eax], bx
        add    eax, 2
        loop   loc_4_1004
loc_4_100F:
        db  0A1h, 09Ch, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_108A
        cmp    word ptr [eax + 4], 10ah
        jb     SHORT loc_4_108A
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    ebx, edx
        and    edx, 0fffffffch
        lea    ecx, [eax + 0ah]
        test   edx, 3
        stc
        jne    SHORT loc_4_1045
        shr    edx, 2
        mov    eax, edx
        shl    eax, 1
        shl    edx, 2
        add    edx, eax
        add    edx, ecx
loc_4_1045:
        mov    ecx, 100h
        sub    ecx, ebx
        jbe    SHORT loc_4_108A
        mov    esi, ebx
        db  08Dh, 034h, 075h, 028h, 00Fh, 000h, 000h
loc_4_1057:
        push   ecx
        movzx  ecx, bl
        and    cl, 3
        mov    al, byte ptr [edx + 5]
        shl    ecx, 1
        shr    al, cl
        and    al, 3
        cmp    al, 0
        je     SHORT loc_4_107B
        cmp    al, 3
        je     SHORT loc_4_107B
        cmp    al, 1
        je     SHORT loc_4_1078
        or     byte ptr [esi], 80h
        jmp    SHORT loc_4_107B
loc_4_1078:
        or     byte ptr [esi], 40h
loc_4_107B:
        pop    ecx
        add    esi, 2
        inc    ebx
        test   bl, 3
        jne    SHORT loc_4_1088
        add    edx, 6
loc_4_1088:
        loop   loc_4_1057
loc_4_108A:
        call   sub_4_12B1
        pop    ebx
        db  0CDh, 020h, 06Bh, 000h, 001h, 000h
        mov    ecx, eax
        xor    edx, edx
        db  08Dh, 014h, 055h, 028h, 00Fh, 000h, 000h
loc_4_10A1:
        or     word ptr [edx], 804h
        add    edx, 2
        loop   loc_4_10A1
        mov    ebx, 0f0h
        mov    ecx, 10h
        test   byte ptr [ebp - 4], 1
        je     SHORT loc_4_10C5
        mov    ebx, 0e0h
        mov    ecx, 20h
loc_4_10C5:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    ebx, eax
        jae    SHORT loc_4_10DA
        sub    eax, ebx
        sub    ecx, eax
        jbe    SHORT loc_4_10F2
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
loc_4_10DA:
        mov    edi, ebx
        db  08Dh, 03Ch, 07Dh, 028h, 00Fh, 000h, 000h
loc_4_10E3:
        test   word ptr [edi], 6740h
        jne    SHORT loc_4_10ED
        or     byte ptr [edi], 2
loc_4_10ED:
        add    edi, 2
        loop   loc_4_10E3
loc_4_10F2:
        mov    ecx, 100h
        db  02Bh, 00Dh, 000h, 000h, 000h, 000h
        jbe    NEAR PTR loc_4_11A2
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        mov    edx, ebx
        db  08Dh, 01Ch, 05Dh, 028h, 00Fh, 000h, 000h
loc_4_1112:
        test   byte ptr [ebx], 80h
        jne    SHORT loc_4_1166
        test   byte ptr [ebx + 1], 10h
        jne    SHORT loc_4_1127
        bt     dword ptr [ebp - 28h], edx
        jae    SHORT loc_4_1127
        or     byte ptr [ebx + 1], 8
loc_4_1127:
        test   byte ptr [ebx + 1], 7
        jne    SHORT loc_4_1132
        test   byte ptr [ebx], 6
        jne    SHORT loc_4_1166
loc_4_1132:
        cmp    edx, 0a0h
        jb     SHORT loc_4_1142
        cmp    edx, 0c0h
        jb     SHORT loc_4_1166
loc_4_1142:
        or     eax, 0ffffffffh
        cmp    edx, 0ffh
        ja     SHORT loc_4_1160
        test   word ptr [ebx], 67c0h
        jne    SHORT loc_4_1161
        test   byte ptr [ebx + 1], 10h
        jne    SHORT loc_4_1160
        bt     dword ptr [ebp - 28h], edx
        jb     SHORT loc_4_1161
loc_4_1160:
        inc    eax
loc_4_1161:
        call   sub_4_13C8
loc_4_1166:
        inc    edx
        add    ebx, 2
        loop   loc_4_1112
        db  08Bh, 00Dh, 070h, 032h, 000h, 000h
        jecxz  loc_4_11A2
        mov    ecx, 100h
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        cmp    ebx, 0c0h
        jae    SHORT loc_4_118C
        mov    ebx, 0c0h
loc_4_118C:
        sub    ecx, ebx
        mov    edx, ebx
        db  08Dh, 01Ch, 05Dh, 028h, 00Fh, 000h, 000h
loc_4_1197:
        call   sub_4_1513
        inc    edx
        add    ebx, 2
        loop   loc_4_1197
loc_4_11A2:
        clc
loc_4_11A3:
        pushfd
        xor    ecx, ecx
        db  087h, 00Dh, 06Ch, 032h, 000h, 000h
        jecxz  loc_4_11B9
        push   0
        push   ecx
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_4_11B9:
        popfd
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        popad
        leave
        ret
loc_4_11D2:
        stc
        jmp    SHORT loc_4_11A3
sub_4_11D5:
        push   eax
        push   edx
        xor    esi, esi
        db  0CDh, 020h, 0B3h, 000h, 001h, 000h
        mov    esi, edx
        pop    edx
        jb     SHORT loc_4_11F2
        jmp    SHORT loc_4_11F6
loc_4_11E6:
        xchg   esi, edx
        db  0CDh, 020h, 0B4h, 000h, 001h, 000h
        xchg   esi, edx
        jae    SHORT loc_4_11F6
loc_4_11F2:
        add    esp, 4
        ret
loc_4_11F6:
        xchg   esi, edx
        db  0CDh, 020h, 0B0h, 000h, 001h, 000h
        xchg   esi, edx
        or     eax, eax
        je     SHORT loc_4_11E6
        mov    ecx, eax
        cld
        lodsb
        cmp    al, 2dh
        jne    SHORT loc_4_11E6
        xchg   esi, edx
        db  0CDh, 020h, 0B0h, 000h, 001h, 000h
        xchg   esi, edx
        or     eax, eax
        je     SHORT loc_4_11E6
        shl    eax, 4
        shl    ecx, 4
        push   ebx
        push   eax
        mov    ebx, dword ptr [esp + 8]
        dec    ebx
        not    ebx
        and    ecx, ebx
        not    ebx
        add    eax, ebx
        not    ebx
        and    eax, ebx
        dec    eax
        cmp    eax, dword ptr [esp]
        jae    SHORT loc_4_123D
        add    eax, dword ptr [esp + 8]
loc_4_123D:
        add    esp, 4
        pop    ebx
        push   edx
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        cmp    dword ptr [esp + 4], 1000h
        je     SHORT loc_4_1258
        shr    edx, 2
        shl    edx, 2
loc_4_1258:
        cmp    ecx, 0fffffh
        jbe    SHORT loc_4_1265
        mov    ecx, 0fffffh
loc_4_1265:
        cmp    ecx, edx
        jae    SHORT loc_4_126B
        mov    ecx, edx
loc_4_126B:
        cmp    eax, 0fffffh
        jbe    SHORT loc_4_1277
        mov    eax, 0fffffh
loc_4_1277:
        cmp    eax, edx
        jae    SHORT loc_4_127D
        mov    eax, edx
loc_4_127D:
        pop    edx
        cmp    ecx, eax
        jae    NEAR PTR loc_4_11E6
        xchg   ecx, eax
        shr    ecx, 0ch
        shr    eax, 0ch
        inc    ecx
        sub    ecx, eax
        push   ecx
        db  08Dh, 004h, 045h, 028h, 00Fh, 000h, 000h
        pop    ecx
loc_4_1299:
        and    word ptr [eax], dx
        or     word ptr [eax], bx
        add    eax, 2
        loop   loc_4_1299
        jmp    NEAR PTR loc_4_11E6
loc_4_12A9:
        push   esp
        dec    edi
        push   ebx
        dec    eax
        dec    ecx
        inc    edx
        inc    ecx
        db  020h
sub_4_12B1:
        mov    edi, 0fe010h
        db  0BEh, 0A9h, 012h, 000h, 000h
        mov    ecx, 8
        cld
        repe cmpsb
        jne    NEAR PTR loc_4_13A4
        cmp    byte ptr [0fe024h], 52h
        jne    SHORT loc_4_1307
        mov    ecx, 0e8h
        mov    eax, 0efh
        bt     dword ptr [0fe025h], 2
        jae    SHORT loc_4_1302
        mov    cx, word ptr [0fe02bh]
        mov    ax, word ptr [0fe02ah]
        shl    eax, 6
        je     SHORT loc_4_1307
        lea    eax, [ecx + eax - 1]
        shr    ecx, 8
        shr    eax, 8
loc_4_1302:
        call   sub_4_13A5
loc_4_1307:
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    edx, dword ptr [ebx + 8]
        mov    byte ptr [edx + 14h], 80h
        mov    byte ptr [edx + 1dh], 15h
        mov    eax, 13h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        test   byte ptr [edx + 2ch], 1
        jne    SHORT loc_4_132F
        cmp    byte ptr [edx + 1dh], 70h
        je     SHORT loc_4_134E
loc_4_132F:
        mov    byte ptr [edx + 14h], 81h
        mov    byte ptr [edx + 1dh], 15h
        mov    eax, 13h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        test   byte ptr [edx + 2ch], 1
        jne    SHORT loc_4_1383
        cmp    byte ptr [edx + 1dh], 70h
        jne    SHORT loc_4_1383
loc_4_134E:
        mov    ecx, 0e0h
        mov    eax, 0e7h
        bt     dword ptr [0fe025h], 3
        jae    SHORT loc_4_137E
        mov    cx, word ptr [0fe02eh]
        mov    ax, word ptr [0fe02dh]
        shl    eax, 6
        je     SHORT loc_4_1383
        lea    eax, [ecx + eax - 1]
        shr    ecx, 8
        shr    eax, 8
loc_4_137E:
        call   sub_4_13A5
loc_4_1383:
        cmp    byte ptr [0ffffah], 27h
        jne    SHORT loc_4_13A4
        mov    dx, 8080h
        in     al, dx
        test   al, 4
        je     SHORT loc_4_13A4
        mov    ecx, 0e0h
        mov    eax, 0efh
        call   sub_4_13A5
loc_4_13A4:
        ret
sub_4_13A5:
        cmp    ecx, 0a0h
        jb     SHORT loc_4_13C7
        xchg   ecx, eax
        inc    ecx
        sub    ecx, eax
        db  08Dh, 004h, 045h, 028h, 00Fh, 000h, 000h
loc_4_13B8:
        test   byte ptr [eax], 40h
        jne    SHORT loc_4_13C2
        or     word ptr [eax], 80h
loc_4_13C2:
        add    eax, 2
        loop   loc_4_13B8
loc_4_13C7:
        ret
sub_4_13C8:
        push   eax
        mov    esi, edx
        shl    esi, 0ch
        cmp    dword ptr [esp], 0
        je     SHORT loc_4_13F4
        push   edx
        push   ecx
        push   0
        push   1000h
        push   esi
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        pop    ecx
        pop    edx
        cmp    eax, -1
        je     NEAR PTR loc_4_1511
        mov    esi, eax
loc_4_13F4:
        cmp    word ptr [esi], 0aa55h
        jne    SHORT loc_4_1470
        push   ecx
        push   edx
        push   ebx
        push   esi
        cld
        movzx  ecx, byte ptr [esi + 2]
        xchg   cl, ch
        shl    ecx, 1
        mov    bl, 1
        jecxz  loc_4_1432
        mov    edx, dword ptr [esp + 8]
        shl    edx, 0ch
        push   esi
        and    esi, 0fffh
        add    edx, esi
        pop    esi
        add    edx, ecx
        cmp    edx, 100000h
        jae    SHORT loc_4_1432
        mov    edx, ecx
        xor    ebx, ebx
loc_4_142B:
        lodsb
        add    bl, al
        loop   loc_4_142B
        mov    eax, edx
loc_4_1432:
        or     bl, bl
        pop    esi
        pop    ebx
        pop    edx
        pop    ecx
        jne    SHORT loc_4_1470
        and    esi, 0fffh
        add    eax, esi
        dec    eax
        shr    eax, 0ch
        je     SHORT loc_4_1468
        inc    eax
        push   ecx
        cmp    eax, ecx
        jbe    SHORT loc_4_1450
        mov    eax, ecx
loc_4_1450:
        mov    ecx, eax
loc_4_1452:
        or     byte ptr [ebx], 2
        add    ebx, 2
        loop   loc_4_1452
        sub    ebx, 2
        pop    ecx
        dec    eax
        sub    ecx, eax
        add    edx, eax
        jmp    NEAR PTR loc_4_1511
loc_4_1468:
        or     byte ptr [ebx], 2
        jmp    NEAR PTR loc_4_1511
loc_4_1470:
        mov    eax, esi
        and    eax, 0fffh
        add    eax, 200h
        add    esi, 200h
        cmp    eax, 1000h
        jb     NEAR PTR loc_4_13F4
        sub    esi, 200h
        and    esi, 0fffff000h
loc_4_1499:
        cmp    edx, 0efh
        ja     SHORT loc_4_1511
        cli
        movzx  eax, word ptr [esi + 0a0h]
        push   eax
        movzx  eax, word ptr [esi + 0a2h]
        push   eax
        mov    word ptr [esi + 0a0h], 0
        mov    word ptr [esi + 0a2h], 0ffffh
        lea    edi, [esi + 0a0h]
        cld
        cld
        cld
        cld
        movzx  eax, word ptr [edi]
        cld
        cld
        cld
        cld
        cld
        cld
        cld
        cld
        cld
        cld
        cld
        cld
        cmp    eax, 0
        pop    eax
        mov    word ptr [esi + 0a2h], ax
        pop    eax
        mov    word ptr [esi + 0a0h], ax
        sti
        jne    SHORT loc_4_14F8
        or     byte ptr [ebx], 4
        jmp    SHORT loc_4_1511
loc_4_14F8:
        mov    eax, esi
        and    eax, 0fffh
        add    eax, 800h
        add    esi, 800h
        cmp    eax, 1000h
        jb     SHORT loc_4_1499
loc_4_1511:
        pop    eax
        ret
sub_4_1513:
        test   word ptr [ebx], 886h
        stc
        jne    NEAR PTR loc_4_1640
        bt     dword ptr [ebp - 28h], edx
        jb     NEAR PTR loc_4_1640
        push   ebp
        sub    esp, 20h
        mov    ebp, esp
        xor    eax, eax
        cmp    edx, 0c0h
        jb     SHORT loc_4_1548
        cmp    edx, 0ffh
        ja     SHORT loc_4_1548
        cmp    word ptr [ebx], 0
        je     SHORT loc_4_1548
        dec    eax
loc_4_1548:
        pushad
        cld
        or     eax, 0ffffffffh
        mov    ecx, 8
        mov    edi, ebp
        rep stosd
        db  08Bh, 03Dh, 070h, 032h, 000h, 000h
        mov    esi, edx
        shl    esi, 0ch
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_4_1588
        push   edx
        push   ecx
        push   0
        push   1000h
        push   esi
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        pop    ecx
        pop    edx
        cmp    eax, -1
        je     NEAR PTR loc_4_1662
        mov    esi, eax
loc_4_1588:
        mov    ecx, 400h
        rep movsd
        sub    esi, 2
        sub    edi, 4
        std
        mov    ecx, 400h
        xor    edx, edx
        mov    eax, dword ptr [esi - 2]
loc_4_15A0:
        xor    eax, 0ffffffffh
        push   eax
        pop    eax
        lodsw
        shl    eax, 10h
        lodsw
        cmp    eax, dword ptr [edi]
        jne    SHORT loc_4_15B6
        add    edx, 10000h
loc_4_15B6:
        movzx  ebx, al
        btr    dword ptr [ebp], ebx
        adc    edx, 0
        movzx  ebx, ah
        btr    dword ptr [ebp], ebx
        adc    edx, 0
        ror    eax, 10h
        movzx  ebx, al
        btr    dword ptr [ebp], ebx
        adc    edx, 0
        movzx  ebx, ah
        btr    dword ptr [ebp], ebx
        adc    edx, 0
        ror    eax, 10h
        sub    edi, 4
        loop   loc_4_15A0
        cld
        db  066h, 03Bh, 015h, 074h, 032h, 000h, 000h
        jb     SHORT loc_4_1643
        push   edx
        shr    edx, 10h
        cmp    edx, 80h
        pop    edx
        jb     SHORT loc_4_1643
        popad
        pushad
        cmp    edx, 0e0h
        jb     SHORT loc_4_1631
        cmp    edx, 0efh
        ja     SHORT loc_4_1631
        test   byte ptr [ebx + 20h], 2
        je     SHORT loc_4_1631
        or     byte ptr [ebx], 8
        db  08Bh, 03Dh, 070h, 032h, 000h, 000h
        lea    esi, [edx + 10h]
        shl    esi, 0ch
        mov    ecx, 400h
        cld
        repe cmpsd
        je     SHORT loc_4_1662
loc_4_1631:
        popad
        or     byte ptr [ebx], 0ah
        add    esp, 20h
        stc
        jmp    SHORT loc_4_163F
loc_4_163B:
        add    esp, 20h
        clc
loc_4_163F:
        pop    ebp
loc_4_1640:
        cld
        cmc
        ret
loc_4_1643:
        dec    dx
        je     SHORT loc_4_1662
        popad
        pushad
        cmp    edx, 0edh
        jb     SHORT loc_4_1662
        cmp    edx, 0efh
        ja     SHORT loc_4_1662
        db  0F6h, 005h, 000h, 011h, 000h, 000h, 002h
        jne    SHORT loc_4_1631
loc_4_1662:
        popad
        jmp    SHORT loc_4_163B
sub_4_1665:
        enter  8, 0
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 0ch]
        xor    eax, eax
        mov    dword ptr [ebp - 8], eax
        cld
        lodsw
        mov    dword ptr [ebp - 4], eax
        lodsw
        cmp    ax, 2
        jne    NEAR PTR loc_4_1741
        xor    eax, eax
loc_4_1688:
        push   eax
        or     al, 8
        cli
        out    96h, al
        jmp    SHORT loc_4_1690
loc_4_1690:
        mov    edx, 100h
        in     al, dx
        jmp    SHORT loc_4_1698
loc_4_1698:
        mov    ah, al
        inc    edx
        in     al, dx
        xchg   al, ah
        mov    ecx, dword ptr [ebp - 4]
        push   esi
loc_4_16A2:
        cmp    ax, word ptr [esi]
        je     SHORT loc_4_16C3
loc_4_16A7:
        movzx  ebx, word ptr [esi + 0ah]
        lea    esi, [esi + ebx*2 + 0ch]
        loop   loc_4_16A2
        pop    esi
        mov    al, 0
        out    96h, al
        sti
        pop    eax
        inc    eax
        cmp    eax, 8
        jb     SHORT loc_4_1688
        jmp    NEAR PTR loc_4_1745
loc_4_16C3:
        push   eax
        push   ecx
        push   esi
        add    esi, 2
        mov    edi, 4
        mov    edx, 102h
        xor    ebx, ebx
loc_4_16D5:
        xor    eax, eax
        cld
        lodsw
        mov    ecx, eax
        xchg   cl, ch
        or     ah, al
        je     SHORT loc_4_16E9
        in     al, dx
        and    al, ch
        ror    al, cl
        or     bl, al
loc_4_16E9:
        inc    edx
        dec    edi
        jne    SHORT loc_4_16D5
        cld
        xor    eax, eax
        lodsw
        cmp    ebx, eax
        jae    SHORT loc_4_1739
        shl    ebx, 1
        movzx  eax, word ptr [ebx + esi]
        xor    ecx, ecx
        mov    cl, ah
        xor    ah, ah
        jecxz  loc_4_1739
        mov    edx, eax
        add    edx, ecx
        dec    edx
        db  03Bh, 005h, 000h, 000h, 000h, 000h
        jae    SHORT loc_4_1716
        db  0A1h, 000h, 000h, 000h, 000h
loc_4_1716:
        cmp    edx, 100h
        jb     SHORT loc_4_1723
        mov    edx, 0ffh
loc_4_1723:
        sub    edx, eax
        jb     SHORT loc_4_1739
        inc    edx
        mov    ecx, edx
        db  08Dh, 004h, 045h, 028h, 00Fh, 000h, 000h
loc_4_1731:
        or     byte ptr [eax], 80h
        add    eax, 2
        loop   loc_4_1731
loc_4_1739:
        pop    esi
        pop    ecx
        pop    eax
        jmp    NEAR PTR loc_4_16A7
loc_4_1741:
        xor    eax, eax
        jmp    SHORT loc_4_174A
loc_4_1745:
        mov    eax, 1
loc_4_174A:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_4_174F:
        enter  8, 0
        push   esi
        push   edi
        push   ebx
        mov    edx, dword ptr [ebp + 14h]
        movzx  ecx, word ptr [edx + 34h]
        shl    ecx, 10h
        mov    cx, word ptr [edx + 30h]
        mov    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [edx + 34h], ax
        mov    word ptr [edx + 30h], 93ch
        mov    ecx, 1
loc_4_1779:
        mov    word ptr [edx + 1ch], 0d800h
        mov    byte ptr [edx + 18h], cl
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [edx + 2ch], 0
        jb     NEAR PTR loc_4_1867
        test   byte ptr [edx + 14h], 2
        je     NEAR PTR loc_4_1883
        movzx  eax, byte ptr [edx + 15h]
        mov    dword ptr [ebp - 4], eax
loc_4_17AA:
        mov    word ptr [edx + 1ch], 0d801h
        mov    byte ptr [edx + 18h], cl
        mov    al, byte ptr [ebp - 3]
        mov    byte ptr [edx + 19h], al
        mov    word ptr [edx + 4], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [edx + 3ch], ax
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     word ptr [edx + 2ch], 0
        jb     NEAR PTR loc_4_1875
        mov    esi, dword ptr [ebp + 0ch]
        test   byte ptr [esi + 22h], 2
        je     NEAR PTR loc_4_1875
        test   byte ptr [esi + 22h], 40h
        jne    NEAR PTR loc_4_1875
        lea    esi, [esi + 73h]
        push   ecx
        mov    ecx, 9
loc_4_17FD:
        mov    eax, dword ptr [esi + 2]
        and    eax, 0ffffffh
        movzx  ebx, word ptr [esi + 5]
        shl    eax, 8
        shl    ebx, 0ah
        add    ebx, eax
        dec    ebx
        shr    eax, 0ch
        shr    ebx, 0ch
        cmp    eax, 100h
        jae    SHORT loc_4_1859
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        jb     SHORT loc_4_1859
        db  03Bh, 005h, 000h, 000h, 000h, 000h
        jae    SHORT loc_4_1834
        db  0A1h, 000h, 000h, 000h, 000h
loc_4_1834:
        cmp    ebx, 0ffh
        jbe    SHORT loc_4_1841
        mov    ebx, 0ffh
loc_4_1841:
        sub    ebx, eax
        jb     SHORT loc_4_1859
        inc    ebx
        push   ecx
        db  08Dh, 004h, 045h, 028h, 00Fh, 000h, 000h
        mov    ecx, ebx
loc_4_1850:
        or     byte ptr [eax], 80h
        add    eax, 2
        loop   loc_4_1850
        pop    ecx
loc_4_1859:
        test   byte ptr [esi], 80h
        je     SHORT loc_4_1874
        add    esi, 7
        dec    ecx
        jne    SHORT loc_4_17FD
        pop    ecx
        jmp    SHORT loc_4_1875
loc_4_1867:
        mov    al, byte ptr [edx + 1dh]
        cmp    al, 83h
        je     SHORT loc_4_1883
        cmp    al, 80h
        je     SHORT loc_4_1889
        jmp    SHORT loc_4_18A1
loc_4_1874:
        pop    ecx
loc_4_1875:
        inc    byte ptr [ebp - 3]
        mov    eax, dword ptr [ebp - 4]
        cmp    ah, al
        jb     NEAR PTR loc_4_17AA
loc_4_1883:
        inc    ecx
        jmp    NEAR PTR loc_4_1779
loc_4_1889:
        mov    eax, 1
loc_4_188E:
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [edx + 30h], cx
        shr    ecx, 10h
        mov    word ptr [edx + 34h], cx
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
loc_4_18A1:
        xor    eax, eax
        jmp    SHORT loc_4_188E
V86MMGR_SetAvailMapPgs:
        pushad
        or     ecx, ecx
        je     NEAR PTR loc_4_193E
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        cmp    eax, dword ptr [esp + 1ch]
        jae    NEAR PTR loc_4_193E
        mov    eax, dword ptr [esp + 1ch]
        mov    edx, eax
        add    edx, ecx
        cmp    edx, 100h
        ja     SHORT loc_4_193E
        mov    edx, eax
        db  08Dh, 004h, 045h, 028h, 00Fh, 000h, 000h
        push   eax
loc_4_18D8:
        test   byte ptr [eax + 1], 45h
        jne    SHORT loc_4_193D
        test   byte ptr [eax + 1], 2
        je     SHORT loc_4_18EC
        test   byte ptr [eax + 1], 20h
        jne    SHORT loc_4_18F1
        jmp    SHORT loc_4_193D
loc_4_18EC:
        test   byte ptr [eax], 0b0h
        jne    SHORT loc_4_193D
loc_4_18F1:
        add    eax, 2
        inc    edx
        loop   loc_4_18D8
        pop    eax
        mov    ecx, dword ptr [esp + 18h]
        mov    edx, dword ptr [esp + 1ch]
loc_4_1900:
        test   byte ptr [eax + 1], 2
        je     SHORT loc_4_190A
        and    byte ptr [eax + 1], 0fdh
loc_4_190A:
        test   byte ptr [eax + 1], 10h
        jne    SHORT loc_4_1934
        test   byte ptr [eax + 1], 8
        jne    SHORT loc_4_1930
        push   edx
        push   ecx
        push   eax
        push   0
        push   0
        push   1
        push   edx
        db  0CDh, 020h, 072h, 000h, 001h, 000h
        add    esp, 10h
        or     eax, eax
        pop    eax
        pop    ecx
        pop    edx
        je     SHORT loc_4_1934
loc_4_1930:
        or     byte ptr [eax + 1], 10h
loc_4_1934:
        add    eax, 2
        inc    edx
        loop   loc_4_1900
        clc
loc_4_193B:
        popad
        ret
loc_4_193D:
        pop    eax
loc_4_193E:
        stc
        jmp    SHORT loc_4_193B
sub_4_1941:
        lea    edi, [ebp - 28h]
        mov    edx, 0a20h
        call   sub_4_1959
        mov    edx, 0a24h
        call   sub_4_1959
        ret
sub_4_1959:
        in     al, dx
        mov    esi, eax
        and    esi, 0fch
        or     esi, 100h
        shl    esi, 0bh
        cmp    esi, 0a0000h
        jb     NEAR PTR loc_4_1A4E
        cmp    esi, 0effffh
        ja     NEAR PTR loc_4_1A4E
        mov    ah, al
        mov    ecx, 5
loc_4_198A:
        jmp    SHORT loc_4_198C
loc_4_198C:
        in     al, dx
        cmp    al, ah
        jne    NEAR PTR loc_4_1A4E
        loop   loc_4_198A
        mov    eax, esi
        shr    eax, 0ch
        mov    ecx, 2
        call   sub_4_1A51
        jb     NEAR PTR loc_4_1A4E
        mov    edx, 1f00h
        xor    eax, eax
loc_4_19B3:
        mov    ah, byte ptr [edx + esi]
        and    ah, 0fh
        add    al, ah
        add    edx, 2
        cmp    edx, 1f60h
        jbe    SHORT loc_4_19B3
        and    al, 0fh
        jne    NEAR PTR loc_4_1A4E
        mov    al, byte ptr [esi + 1e00h]
        and    eax, 0feh
        cmp    al, 0a0h
        jb     SHORT loc_4_1A4E
        cmp    al, 0efh
        ja     SHORT loc_4_1A4E
        mov    cl, byte ptr [esi + 1e01h]
        shr    ecx, 2
        and    ecx, 3
        mov    edx, 2
        shl    edx, cl
        mov    ecx, edx
        push   eax
        push   ecx
        call   sub_4_1A51
        pop    ecx
        pop    eax
        jb     SHORT loc_4_1A4E
        push   eax
        push   ecx
        push   0
        push   0
        push   ecx
        push   eax
        db  0CDh, 020h, 072h, 000h, 001h, 000h
        add    esp, 10h
        pop    ecx
        pop    eax
        db  08Dh, 004h, 045h, 028h, 00Fh, 000h, 000h
loc_4_1A1B:
        or     word ptr [eax], 804h
        add    eax, 2
        loop   loc_4_1A1B
        shr    esi, 0ch
        push   0
        push   0
        push   2
        push   esi
        db  0CDh, 020h, 072h, 000h, 001h, 000h
        add    esp, 10h
        db  08Dh, 034h, 075h, 028h, 00Fh, 000h, 000h
        or     word ptr [esi], 804h
        add    esi, 2
        or     word ptr [esi], 804h
        clc
loc_4_1A4D:
        ret
loc_4_1A4E:
        stc
        jmp    SHORT loc_4_1A4D
sub_4_1A51:
        bt     dword ptr [edi], eax
        jb     SHORT loc_4_1A5A
        inc    eax
        loop   sub_4_1A51
        clc
loc_4_1A5A:
        ret
        db  000h
V86MMGR_SYS_CRITICAL_INIT:
        db  0CDh, 020h, 06Ah, 000h, 001h, 000h
        db  0A3h, 000h, 012h, 000h, 000h
        push   0
        push   284h
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        test   eax, eax
        jne    SHORT loc_4_1A85
loc_4_1A7A:
        db  0E8h, 099h, 015h, 0FCh, 0FFh
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
loc_4_1A85:
        db  0A3h, 0FCh, 011h, 000h, 000h
        xor    eax, eax
        xor    esi, esi
        db  0BFh, 0B7h, 030h, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        or     al, al
        je     SHORT loc_4_1AA4
        db  080h, 00Dh, 004h, 012h, 000h, 000h, 010h
loc_4_1AA4:
        xor    eax, eax
        xor    esi, esi
        db  0BFh, 0C4h, 030h, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        or     al, al
        je     SHORT loc_4_1ABE
        db  080h, 00Dh, 004h, 012h, 000h, 000h, 008h
loc_4_1ABE:
        mov    eax, 15h
        db  0BEh, 024h, 002h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        call   sub_4_01D4
        jb     SHORT loc_4_1A7A
        call   sub_4_1E68
        jb     SHORT loc_4_1A7A
        call   sub_4_09E7
        jb     SHORT loc_4_1A7A
        db  0FEh, 005h, 014h, 012h, 000h, 000h
        ret
V86MMGR_DEVICE_INIT:
        pushad
        db  080h, 03Dh, 080h, 032h, 000h, 000h, 002h
        ja     NEAR PTR loc_4_1CE4
        je     NEAR PTR loc_4_1BB9
        pushad
        mov    eax, 4
        db  0BFh, 08Eh, 031h, 000h, 000h
        xor    esi, esi
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        add    eax, 3
        shr    eax, 2
        or     eax, eax
        jne    SHORT loc_4_1B1C
        inc    eax
loc_4_1B1C:
        cmp    eax, 10h
        jbe    SHORT loc_4_1B26
        mov    eax, 10h
loc_4_1B26:
        mov    ah, al
        xor    ebx, ebx
        mov    ecx, ebx
        db  0CDh, 020h, 004h, 000h, 006h, 000h
        popad
        mov    eax, 800h
        xor    esi, esi
        db  0BFh, 0D1h, 030h, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        push   eax
        mov    eax, 0
        xor    esi, esi
        db  0BFh, 0DFh, 030h, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        push   eax
        mov    eax, 800h
        xor    esi, esi
        db  0BFh, 0FFh, 030h, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        push   eax
        mov    eax, 0
        xor    esi, esi
        db  0BFh, 00Dh, 031h, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        push   eax
        xor    eax, eax
        xor    esi, esi
        db  0BFh, 0F0h, 030h, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        push   eax
        xor    eax, eax
        xor    esi, esi
        db  0BFh, 01Eh, 031h, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        xor    ecx, ecx
        or     eax, eax
        je     SHORT loc_4_1BA7
        or     ecx, 4
loc_4_1BA7:
        pop    eax
        or     eax, eax
        je     SHORT loc_4_1BAF
        or     ecx, 8
loc_4_1BAF:
        pop    esi
        pop    edi
        pop    eax
        pop    edx
        db  0CDh, 020h, 002h, 000h, 006h, 000h
loc_4_1BB9:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        db  080h, 03Dh, 080h, 032h, 000h, 000h, 002h
        jae    SHORT loc_4_1C08
        push   ebx
        call   sub_4_0B0C
        pop    ebx
        jb     NEAR PTR loc_4_1CE7
        push   ebx
        call   sub_4_246D
        pop    ebx
        jb     NEAR PTR loc_4_1CE7
        db  080h, 03Dh, 080h, 032h, 000h, 000h, 000h
        je     SHORT loc_4_1C08
        db  0FEh, 005h, 080h, 032h, 000h, 000h
        jmp    NEAR PTR loc_4_1CCF
loc_4_1C08:
        db  0C6h, 005h, 080h, 032h, 000h, 000h, 003h
        call   sub_4_2418
        jb     NEAR PTR loc_4_1CE7
        call   sub_4_1D0A
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        inc    eax
        cmp    eax, 0a0h
        jae    NEAR PTR loc_4_1CC1
        push   eax
        db  0CDh, 020h, 000h, 000h, 012h, 000h
        jb     SHORT loc_4_1C57
        or     eax, eax
        je     SHORT loc_4_1C57
        db  0CDh, 020h, 06Bh, 000h, 001h, 000h
        cmp    edx, eax
        jbe    SHORT loc_4_1C57
        cmp    edx, 0a0h
        jae    SHORT loc_4_1C57
        dec    edx
        db  089h, 015h, 07Ch, 032h, 000h, 000h
loc_4_1C57:
        xor    eax, eax
        xor    esi, esi
        db  0BFh, 07Eh, 031h, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        jb     SHORT loc_4_1C6E
        or     eax, eax
        je     SHORT loc_4_1CC0
        jmp    SHORT loc_4_1C99
loc_4_1C6E:
        pop    eax
        push   eax
        cmp    eax, 90h
        ja     SHORT loc_4_1CC0
        test   al, 0fh
        jne    SHORT loc_4_1CC0
        db  08Bh, 00Dh, 07Ch, 032h, 000h, 000h
        inc    ecx
        sub    ecx, eax
        jbe    SHORT loc_4_1CC0
        db  08Dh, 004h, 045h, 028h, 00Fh, 000h, 000h
loc_4_1C8D:
        test   word ptr [eax], 2fa6h
        jne    SHORT loc_4_1CC0
        add    eax, 2
        loop   loc_4_1C8D
loc_4_1C99:
        db  0CDh, 020h, 000h, 000h, 015h, 000h
        jb     SHORT loc_4_1CC0
        cmp    eax, 30ah
        jb     SHORT loc_4_1CC0
        db  0CDh, 020h, 00Ah, 000h, 015h, 000h
        jb     SHORT loc_4_1CC0
        push   0
        db  0FFh, 035h, 07Ch, 032h, 000h, 000h
        db  0CDh, 020h, 0D4h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_4_1CC0:
        pop    eax
loc_4_1CC1:
        call   sub_4_0BFA
        jb     SHORT loc_4_1CE7
        call   0
        jb     SHORT loc_4_1CE7
loc_4_1CCF:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
loc_4_1CE4:
        clc
loc_4_1CE5:
        popad
        ret
loc_4_1CE7:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        db  0E8h, 017h, 013h, 0FCh, 0FFh
loc_4_1D01:
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
        stc
        jmp    SHORT loc_4_1CE5
sub_4_1D0A:
        enter  24h, 0
        pushad
        lea    edx, [ebp - 24h]
        push   0
        push   edx
        push   0
        db  0CDh, 020h, 074h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_1D77
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        inc    eax
        mov    ebx, eax
        mov    esi, 110h
        sub    esi, ebx
        jbe    SHORT loc_4_1D77
        mov    edi, ebx
        db  08Dh, 01Ch, 05Dh, 028h, 00Fh, 000h, 000h
loc_4_1D3F:
        test   word ptr [ebx], 1080h
        jne    SHORT loc_4_1D50
        bt     dword ptr [ebp - 24h], edi
        jae    SHORT loc_4_1D50
        or     byte ptr [ebx + 1], 8
loc_4_1D50:
        test   word ptr [ebx], 3f60h
        jne    SHORT loc_4_1D70
        test   byte ptr [ebx], 86h
        je     SHORT loc_4_1D70
        push   0
        push   0
        push   1
        push   edi
        db  0CDh, 020h, 072h, 000h, 001h, 000h
        add    esp, 10h
        or     byte ptr [ebx + 1], 8
loc_4_1D70:
        inc    edi
        add    ebx, 2
        dec    esi
        jne    SHORT loc_4_1D3F
loc_4_1D77:
        popad
        leave
        ret
V86MMGR_Dev_Init:
        call   sub_4_0E3C
        jb     SHORT loc_4_1D01
        db  0C6h, 005h, 081h, 032h, 000h, 000h, 001h
        pushad
        xor    esi, esi
        mov    edi, 9fh
        db  0CDh, 020h, 05Fh, 001h, 001h, 000h
        mov    esi, 0a0h
        mov    edi, 0bfh
        db  0CDh, 020h, 05Fh, 001h, 001h, 000h
        inc    edi
        mov    edx, edi
        db  08Dh, 014h, 055h, 028h, 00Fh, 000h, 000h
loc_4_1DB0:
        test   word ptr [edx], 88eh
        je     SHORT loc_4_1DBF
        mov    esi, edi
        db  0CDh, 020h, 05Fh, 001h, 001h, 000h
loc_4_1DBF:
        add    edx, 2
        inc    edi
        cmp    edi, 100h
        jb     SHORT loc_4_1DB0
        popad
        ret
V86MMGR_Alloc_UM_Page:
        push   edx
        push   eax
        push   ecx
        jecxz  loc_4_1DF2
        mov    edx, ecx
        db  080h, 03Dh, 081h, 032h, 000h, 000h, 000h
        je     SHORT loc_4_1DF2
        xor    eax, eax
        mov    al, 0c0h
loc_4_1DE1:
        db  066h, 0F7h, 004h, 045h, 028h, 00Fh, 000h, 000h, 086h, 00Fh
        je     SHORT loc_4_1DFA
loc_4_1DED:
        inc    eax
        test   ah, ah
        je     SHORT loc_4_1DE1
loc_4_1DF2:
        pop    ecx
        pop    eax
        sub    ecx, edx
        pop    edx
        xor    eax, eax
        ret
loc_4_1DFA:
        mov    ecx, dword ptr [esp]
loc_4_1DFD:
        dec    ecx
        je     SHORT loc_4_1E19
        inc    eax
        test   ah, ah
        jne    SHORT loc_4_1DF2
        db  066h, 0F7h, 004h, 045h, 028h, 00Fh, 000h, 000h, 086h, 00Fh
        je     SHORT loc_4_1DFD
        cmp    ecx, edx
        jae    SHORT loc_4_1DED
        mov    edx, ecx
        jmp    SHORT loc_4_1DED
loc_4_1E19:
        mov    ecx, dword ptr [esp]
        push   eax
        push   edx
        sub    eax, ecx
        inc    eax
        push   0
        push   0
        push   ecx
        push   eax
        db  0CDh, 020h, 072h, 000h, 001h, 000h
        add    esp, 10h
        pop    edx
        pop    eax
        mov    ecx, dword ptr [esp]
loc_4_1E35:
        db  080h, 00Ch, 045h, 029h, 00Fh, 000h, 000h, 008h
        dec    eax
        loop   loc_4_1E35
        inc    eax
        pop    ecx
        add    esp, 4
        shl    eax, 0ch
        pop    edx
        ret
V86MMGR_INIT_COMPLETE:
        db  0C6h, 005h, 015h, 012h, 000h, 000h, 001h
        db  0CDh, 020h, 000h, 000h, 007h, 000h
        or     eax, eax
        je     SHORT loc_4_1E61
        db  088h, 01Dh, 015h, 012h, 000h, 000h
loc_4_1E61:
        call   sub_4_0A2A
        clc
        ret
sub_4_1E68:
        mov    ecx, 1
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        jb     NEAR PTR loc_4_2058
        db  0A3h, 02Ch, 013h, 000h, 000h
        xor    ecx, ecx
        db  0CDh, 020h, 0BCh, 000h, 001h, 000h
        db  089h, 015h, 030h, 013h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_1E9B
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        db  0E8h, 0FCh, 0E1h, 0FBh, 0FFh
loc_4_1E9B:
        xor    eax, eax
        xor    esi, esi
        db  0BFh, 061h, 031h, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        jb     SHORT loc_4_1EB3
        je     SHORT loc_4_1EB3
        db  0A3h, 030h, 013h, 000h, 000h
loc_4_1EB3:
        db  0A1h, 094h, 032h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_1F33
        push   0
        push   40h
        push   eax
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        inc    eax
        je     NEAR PTR loc_4_2058
        dec    eax
        mov    esi, eax
        xor    edx, edx
        push   esi
        mov    ecx, 10h
loc_4_1EDC:
        lodsd
        test   al, 1
        jne    SHORT loc_4_1EE2
        inc    edx
loc_4_1EE2:
        shr    eax, 0ch
        cmp    eax, 110h
        jae    SHORT loc_4_1EF7
        cmp    eax, 100h
        jae    SHORT loc_4_1EF6
        inc    edx
        jmp    SHORT loc_4_1EF7
loc_4_1EF6:
        inc    edx
loc_4_1EF7:
        loop   loc_4_1EDC
        pop    esi
        db  0A1h, 09Ch, 000h, 000h, 000h
        test   byte ptr [eax], 8
        jne    SHORT loc_4_1F13
        db  080h, 025h, 004h, 012h, 000h, 000h, 0DFh
        db  0CDh, 020h, 0DDh, 000h, 001h, 000h
        jmp    SHORT loc_4_1F33
loc_4_1F13:
        or     edx, edx
        jne    SHORT loc_4_1F33
        mov    ecx, 10h
loc_4_1F1C:
        cld
        lodsd
        shr    eax, 0ch
        push   ecx
        push   0
        push   1
        push   eax
        db  0CDh, 020h, 0D2h, 000h, 001h, 000h
        add    esp, 0ch
        pop    ecx
        loop   loc_4_1F1C
loc_4_1F33:
        push   0
        push   0
        push   10h
        push   100h
        db  0CDh, 020h, 072h, 000h, 001h, 000h
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_1F52
        db  080h, 00Dh, 004h, 012h, 000h, 000h, 080h
loc_4_1F52:
        mov    edi, 100h
        db  08Dh, 03Ch, 07Dh, 028h, 00Fh, 000h, 000h
        mov    ecx, 10h
        mov    eax, 1224h
        db  0F6h, 005h, 004h, 012h, 000h, 000h, 080h
        je     SHORT loc_4_1F76
        mov    eax, 24h
loc_4_1F76:
        or     word ptr [edi], ax
        add    edi, 2
        loop   loc_4_1F76
        db  0E8h, 019h, 0EDh, 0FBh, 0FFh
        jb     NEAR PTR loc_4_2058
        call   sub_4_2072
        jb     NEAR PTR loc_4_2058
        xor    ecx, ecx
        db  0CDh, 020h, 0BCh, 000h, 001h, 000h
        xor    esi, esi
loc_4_1F9E:
        db  089h, 00Dh, 09Ch, 012h, 000h, 000h
        db  089h, 00Dh, 098h, 032h, 000h, 000h
        movzx  eax, cx
        shr    ecx, 10h
        shl    ecx, 4
        add    eax, ecx
        db  0A3h, 0A0h, 012h, 000h, 000h
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 022h, 013h, 000h, 000h
        mov    dl, byte ptr [eax + 4]
        db  088h, 015h, 026h, 013h, 000h, 000h
        cmp    cl, 0ebh
        je     SHORT loc_4_2014
        cmp    cl, 0eah
        je     SHORT loc_4_2007
        or     esi, esi
        je     SHORT loc_4_2058
        db  0FEh, 005h, 0A0h, 032h, 000h, 000h
        mov    ecx, dword ptr [esi + 1]
        db  089h, 00Dh, 09Ch, 012h, 000h, 000h
        db  089h, 03Dh, 098h, 032h, 000h, 000h
        db  089h, 035h, 0A0h, 012h, 000h, 000h
        mov    ecx, dword ptr [esi]
        db  089h, 00Dh, 022h, 013h, 000h, 000h
        mov    dl, byte ptr [esi + 4]
        db  088h, 015h, 026h, 013h, 000h, 000h
        jmp    SHORT loc_4_203F
loc_4_2007:
        mov    ecx, dword ptr [eax + 1]
        mov    esi, eax
        db  08Bh, 03Dh, 098h, 032h, 000h, 000h
        jmp    SHORT loc_4_1F9E
loc_4_2014:
        cmp    dl, 90h
        jne    SHORT loc_4_2058
        mov    eax, ecx
        shr    eax, 10h
        cmp    eax, 9090h
        jne    SHORT loc_4_2058
        movzx  eax, ch
        add    eax, 2
        db  08Bh, 00Dh, 09Ch, 012h, 000h, 000h
        add    cx, ax
        db  0A3h, 09Ch, 032h, 000h, 000h
        db  089h, 00Dh, 09Ch, 012h, 000h, 000h
loc_4_203F:
        db  0A1h, 098h, 032h, 000h, 000h
        db  0BEh, 024h, 028h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 001h, 000h
        jb     SHORT loc_4_2058
        db  0FEh, 005h, 027h, 013h, 000h, 000h
        ret
loc_4_2058:
        db  0E8h, 0BBh, 00Fh, 0FCh, 0FFh
        db  00Fh, 0B7h, 035h, 00Eh, 000h, 000h, 000h
        db  003h, 035h, 004h, 000h, 000h, 000h
        xor    eax, eax
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
sub_4_2072:
        pushad
        db  083h, 03Dh, 084h, 012h, 000h, 000h, 000h
        je     NEAR PTR loc_4_239F
        db  08Bh, 03Dh, 09Ch, 000h, 000h, 000h
        lea    edi, [edi + 0ah]
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        inc    eax
        and    eax, 0fffffffch
        push   eax
        mov    ecx, eax
        test   ecx, 3
        stc
        jne    SHORT loc_4_20AD
        shr    ecx, 2
        mov    eax, ecx
        shl    eax, 1
        shl    ecx, 2
        add    ecx, eax
        add    ecx, edi
loc_4_20AD:
        mov    edi, ecx
        pop    edx
        mov    ecx, 100h
        sub    ecx, edx
        shr    ecx, 2
        xor    eax, eax
loc_4_20BC:
        push   eax
        test   byte ptr [edi], 4
        je     NEAR PTR loc_4_2154
        push   ecx
        push   edx
        mov    eax, edx
        db  08Dh, 004h, 045h, 028h, 00Fh, 000h, 000h
        test   byte ptr [eax + 1], 4
        jne    SHORT loc_4_2152
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_4_20E7
        test   byte ptr [eax + 1], 2
        je     SHORT loc_4_2152
        jmp    SHORT loc_4_210D
loc_4_20E7:
        test   byte ptr [eax + 1], 10h
        pushfd
        or     byte ptr [eax + 1], 32h
        popfd
        jne    SHORT loc_4_210D
        push   edx
        push   0
        push   0
        push   1
        push   edx
        db  0CDh, 020h, 072h, 000h, 001h, 000h
        add    esp, 10h
        pop    edx
        or     eax, eax
        je     NEAR PTR loc_4_23A2
loc_4_210D:
        movzx  eax, byte ptr [edi + 1]
        cmp    al, 0ffh
        jne    SHORT loc_4_2119
        mov    eax, dword ptr [esp + 8]
loc_4_2119:
        db  03Bh, 005h, 084h, 012h, 000h, 000h
        jae    NEAR PTR loc_4_2402
        mov    ecx, 10h
        mul    ecx
        db  003h, 005h, 09Ch, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 18ch]
        pop    edx
        push   edx
        push   0
        push   1
        push   edx
        push   ebx
        push   eax
        db  0CDh, 020h, 05Eh, 000h, 001h, 000h
        add    esp, 14h
        or     eax, eax
        je     NEAR PTR loc_4_2402
loc_4_2152:
        pop    edx
        pop    ecx
loc_4_2154:
        inc    edx
        test   byte ptr [edi], 8
        je     NEAR PTR loc_4_21EC
        push   ecx
        push   edx
        mov    eax, edx
        db  08Dh, 004h, 045h, 028h, 00Fh, 000h, 000h
        test   byte ptr [eax + 1], 4
        jne    SHORT loc_4_21EA
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_4_217F
        test   byte ptr [eax + 1], 2
        je     SHORT loc_4_21EA
        jmp    SHORT loc_4_21A5
loc_4_217F:
        test   byte ptr [eax + 1], 10h
        pushfd
        or     byte ptr [eax + 1], 32h
        popfd
        jne    SHORT loc_4_21A5
        push   edx
        push   0
        push   0
        push   1
        push   edx
        db  0CDh, 020h, 072h, 000h, 001h, 000h
        add    esp, 10h
        pop    edx
        or     eax, eax
        je     NEAR PTR loc_4_23A2
loc_4_21A5:
        movzx  eax, byte ptr [edi + 1]
        cmp    al, 0ffh
        jne    SHORT loc_4_21B1
        mov    eax, dword ptr [esp + 8]
loc_4_21B1:
        db  03Bh, 005h, 084h, 012h, 000h, 000h
        jae    NEAR PTR loc_4_2402
        mov    ecx, 10h
        mul    ecx
        db  003h, 005h, 09Ch, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 190h]
        pop    edx
        push   edx
        push   0
        push   1
        push   edx
        push   ebx
        push   eax
        db  0CDh, 020h, 05Eh, 000h, 001h, 000h
        add    esp, 14h
        or     eax, eax
        je     NEAR PTR loc_4_2402
loc_4_21EA:
        pop    edx
        pop    ecx
loc_4_21EC:
        inc    edx
        test   byte ptr [edi], 10h
        je     NEAR PTR loc_4_22C0
        push   ecx
        push   edx
        mov    eax, edx
        db  08Dh, 004h, 045h, 028h, 00Fh, 000h, 000h
        test   byte ptr [eax + 1], 4
        jne    NEAR PTR loc_4_22BE
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_4_221F
        test   byte ptr [eax + 1], 2
        je     NEAR PTR loc_4_22BE
        jmp    SHORT loc_4_2279
loc_4_221F:
        test   byte ptr [eax + 1], 10h
        pushfd
        or     byte ptr [eax + 1], 32h
        popfd
        jne    SHORT loc_4_2279
        push   edx
        push   eax
        push   0
        push   0
        push   1
        push   edx
        db  0CDh, 020h, 072h, 000h, 001h, 000h
        add    esp, 10h
        pop    ecx
        pop    edx
        or     eax, eax
        jne    SHORT loc_4_2279
        cmp    edx, 0c6h
        jne    SHORT loc_4_2274
        db  0A1h, 094h, 012h, 000h, 000h
        inc    eax
        je     SHORT loc_4_2274
        dec    eax
        db  003h, 005h, 09Ch, 000h, 000h, 000h
        cmp    dword ptr [eax], 334d4d45h
        jne    SHORT loc_4_2274
        cmp    word ptr [eax + 4], 3638h
        jne    SHORT loc_4_2274
        and    byte ptr [ecx + 1], 0cdh
        or     byte ptr [ecx + 1], 8
        jmp    SHORT loc_4_22BE
loc_4_2274:
        jmp    NEAR PTR loc_4_23A2
loc_4_2279:
        movzx  eax, byte ptr [edi + 1]
        cmp    al, 0ffh
        jne    SHORT loc_4_2285
        mov    eax, dword ptr [esp + 8]
loc_4_2285:
        db  03Bh, 005h, 084h, 012h, 000h, 000h
        jae    NEAR PTR loc_4_2402
        mov    ecx, 10h
        mul    ecx
        db  003h, 005h, 09Ch, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 194h]
        pop    edx
        push   edx
        push   0
        push   1
        push   edx
        push   ebx
        push   eax
        db  0CDh, 020h, 05Eh, 000h, 001h, 000h
        add    esp, 14h
        or     eax, eax
        je     NEAR PTR loc_4_2402
loc_4_22BE:
        pop    edx
        pop    ecx
loc_4_22C0:
        inc    edx
        test   byte ptr [edi], 20h
        je     NEAR PTR loc_4_238D
        push   ecx
        push   edx
        mov    eax, edx
        db  08Dh, 004h, 045h, 028h, 00Fh, 000h, 000h
        test   byte ptr [eax + 1], 4
        jne    NEAR PTR loc_4_238B
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_4_22F3
        test   byte ptr [eax + 1], 2
        je     NEAR PTR loc_4_238B
        jmp    SHORT loc_4_234A
loc_4_22F3:
        test   byte ptr [eax + 1], 10h
        pushfd
        or     byte ptr [eax + 1], 32h
        popfd
        jne    SHORT loc_4_234A
        push   edx
        push   eax
        push   0
        push   0
        push   1
        push   edx
        db  0CDh, 020h, 072h, 000h, 001h, 000h
        add    esp, 10h
        pop    ecx
        pop    edx
        or     eax, eax
        jne    SHORT loc_4_234A
        cmp    edx, 0c7h
        jne    SHORT loc_4_2348
        db  0A1h, 094h, 012h, 000h, 000h
        inc    eax
        je     SHORT loc_4_2348
        dec    eax
        db  003h, 005h, 09Ch, 000h, 000h, 000h
        cmp    dword ptr [eax], 334d4d45h
        jne    SHORT loc_4_2348
        cmp    word ptr [eax + 4], 3638h
        jne    SHORT loc_4_2348
        and    byte ptr [ecx + 1], 0cdh
        or     byte ptr [ecx + 1], 8
        jmp    SHORT loc_4_238B
loc_4_2348:
        jmp    SHORT loc_4_23A2
loc_4_234A:
        movzx  eax, byte ptr [edi + 1]
        cmp    al, 0ffh
        jne    SHORT loc_4_2356
        mov    eax, dword ptr [esp + 8]
loc_4_2356:
        db  03Bh, 005h, 084h, 012h, 000h, 000h
        jae    NEAR PTR loc_4_2402
        mov    ecx, 10h
        mul    ecx
        db  003h, 005h, 09Ch, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 198h]
        pop    edx
        push   edx
        push   0
        push   1
        push   edx
        push   ebx
        push   eax
        db  0CDh, 020h, 05Eh, 000h, 001h, 000h
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_4_2402
loc_4_238B:
        pop    edx
        pop    ecx
loc_4_238D:
        inc    edx
        pop    eax
        test   byte ptr [edi], 3ch
        je     SHORT loc_4_2395
        inc    eax
loc_4_2395:
        add    edi, 6
        dec    ecx
        jne    NEAR PTR loc_4_20BC
loc_4_239F:
        clc
loc_4_23A0:
        popad
        ret
loc_4_23A2:
        db  0E8h, 071h, 00Ch, 0FCh, 0FFh
        shl    edx, 8
        cmp    edx, 0b000h
        jb     SHORT loc_4_23E4
        cmp    edx, 0b700h
        ja     SHORT loc_4_23E4
        push   edx
        xor    esi, esi
        db  0BFh, 040h, 030h, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        pop    edx
        jb     SHORT loc_4_23D1
        je     SHORT loc_4_23D1
        or     eax, eax
        je     SHORT loc_4_23E4
loc_4_23D1:
        push   edx
        push   0c0004h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        jmp    SHORT loc_4_23F5
loc_4_23E4:
        push   edx
        push   0c0003h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
loc_4_23F5:
        mov    esi, eax
        pushad
        mov    esi, esi
        xor    eax, eax
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
loc_4_2402:
        add    esp, 0ch
        mov    eax, 20005h
        xor    edx, edx
        xor    ecx, ecx
        inc    ecx
        db  0CDh, 020h, 0E0h, 000h, 001h, 000h
        stc
        jmp    SHORT loc_4_23A0
sub_4_2418:
        db  0F6h, 005h, 004h, 012h, 000h, 000h, 020h
        jne    SHORT loc_4_246B
        mov    word ptr [ebp + 1ch], 1607h
        mov    word ptr [ebp + 10h], 6
        mov    word ptr [ebp + 18h], 0
        xor    eax, eax
        mov    word ptr [ebp + 14h], ax
        mov    word ptr [ebp + 4], ax
        mov    word ptr [ebp], ax
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        cmp    word ptr [ebp + 1ch], 0
        je     SHORT loc_4_2466
        xor    esi, esi
        xor    eax, eax
        db  0BFh, 070h, 031h, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        or     eax, eax
        je     SHORT loc_4_246B
loc_4_2466:
        call   sub_4_2886
loc_4_246B:
        clc
        ret
sub_4_246D:
        xor    al, al
        db  086h, 005h, 027h, 013h, 000h, 000h
        or     al, al
        je     SHORT loc_4_2484
        db  0A1h, 098h, 032h, 000h, 000h
        db  0CDh, 020h, 00Ah, 000h, 001h, 000h
loc_4_2484:
        xor    edx, edx
        db  08Bh, 00Dh, 09Ch, 000h, 000h, 000h
        jecxz  loc_4_249C
        cmp    word ptr [ecx + 4], 10ah
        jb     SHORT loc_4_249C
        db  08Bh, 015h, 07Ch, 012h, 000h, 000h
loc_4_249C:
        db  089h, 015h, 07Ch, 012h, 000h, 000h
        db  083h, 03Dh, 0B8h, 012h, 000h, 000h, 000h
        jne    SHORT loc_4_2509
        db  08Bh, 035h, 07Ch, 012h, 000h, 000h
        or     esi, esi
        jne    SHORT loc_4_24BA
        mov    esi, 80h
loc_4_24BA:
        lea    ecx, [esi + esi]
        push   100h
        push   ecx
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     NEAR PTR loc_4_257F
        db  0A3h, 0BCh, 012h, 000h, 000h
        mov    edi, eax
        mov    ecx, esi
        or     eax, 0ffffffffh
        cld
        rep stosw
        mov    eax, esi
        imul   eax, eax, 10h
        push   101h
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     NEAR PTR loc_4_257F
        db  0A3h, 0B4h, 012h, 000h, 000h
        db  089h, 035h, 0B8h, 012h, 000h, 000h
loc_4_2509:
        db  083h, 03Dh, 07Ch, 012h, 000h, 000h, 000h
        je     SHORT loc_4_258A
        db  08Bh, 035h, 09Ch, 000h, 000h, 000h
        db  003h, 035h, 080h, 012h, 000h, 000h
        db  08Bh, 00Dh, 07Ch, 012h, 000h, 000h
        db  08Bh, 03Dh, 0BCh, 012h, 000h, 000h
        db  08Bh, 015h, 0B8h, 012h, 000h, 000h
loc_4_2530:
        test   byte ptr [esi + 2], 1
        jne    SHORT loc_4_255C
        push   ecx
        push   edx
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [esi + 4]
        jecxz  loc_4_2558
        shl    ecx, 0ah
        push   0
        push   ecx
        push   eax
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_4_2058
loc_4_2558:
        pop    ecx
        pop    edx
        jmp    SHORT loc_4_2562
loc_4_255C:
        mov    ax, word ptr [esi]
        cld
        stosw
loc_4_2562:
        sub    edx, 1
        jb     SHORT loc_4_256C
        add    esi, 0ch
        loop   loc_4_2530
loc_4_256C:
        db  02Bh, 03Dh, 0BCh, 012h, 000h, 000h
        shr    edi, 1
        db  089h, 03Dh, 0ACh, 012h, 000h, 000h
        jmp    NEAR PTR loc_4_266C
loc_4_257F:
        db  0E8h, 094h, 00Ah, 0FCh, 0FFh
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
loc_4_258A:
        mov    word ptr [ebp + 1ch], 4309h
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 1ch]
        cmp    al, ah
        jne    SHORT loc_4_2618
        cmp    al, 43h
        jne    SHORT loc_4_2618
        movzx  eax, word ptr [ebp + 38h]
        shl    eax, 4
        movzx  ecx, word ptr [ebp + 10h]
        add    eax, ecx
        cmp    byte ptr [eax], 1
        jne    SHORT loc_4_2618
        movzx  edi, byte ptr [eax + 1]
        movzx  ecx, word ptr [eax + 2]
        movzx  esi, word ptr [eax + 6]
        shl    esi, 4
        movzx  eax, word ptr [eax + 4]
        add    esi, eax
loc_4_25CD:
        cmp    byte ptr [esi], 2
        jne    SHORT loc_4_2614
        push   ecx
        cmp    dword ptr [esi + 6], 8
        jb     SHORT loc_4_25F2
        mov    eax, dword ptr [esi + 2]
        shr    eax, 2
        inc    eax
        push   0
        push   1
        push   eax
        db  0CDh, 020h, 05Dh, 001h, 001h, 000h
        add    esp, 0ch
        cmp    al, 2
        je     SHORT loc_4_260B
loc_4_25F2:
        mov    eax, dword ptr [esi + 2]
        mov    edx, dword ptr [esi + 6]
        shl    eax, 0ah
        shl    edx, 0ah
        push   0
        push   edx
        push   eax
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
loc_4_260B:
        pop    ecx
        test   eax, eax
        je     NEAR PTR loc_4_2058
loc_4_2614:
        add    esi, edi
        loop   loc_4_25CD
loc_4_2618:
        db  08Bh, 03Dh, 0BCh, 012h, 000h, 000h
        db  08Bh, 00Dh, 0B8h, 012h, 000h, 000h
loc_4_2624:
        mov    word ptr [ebp + 14h], 0
        mov    byte ptr [ebp + 1dh], 9
        db  0A1h, 09Ch, 012h, 000h, 000h
        db  0E8h, 0C1h, 0DFh, 0FBh, 0FFh
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        cmp    word ptr [ebp + 1ch], 1
        jne    SHORT loc_4_264E
        movzx  eax, word ptr [ebp + 14h]
        cld
        stosw
        loop   loc_4_2624
loc_4_264E:
        db  0A1h, 0BCh, 012h, 000h, 000h
        sub    edi, eax
        mov    ecx, edi
        shr    ecx, 1
        db  089h, 00Dh, 0ACh, 012h, 000h, 000h
        push   0
        push   edi
        push   eax
        db  0CDh, 020h, 050h, 000h, 001h, 000h
        add    esp, 0ch
loc_4_266C:
        mov    eax, 0eh
        db  08Bh, 00Dh, 0ACh, 012h, 000h, 000h
        shl    ecx, 2
        add    ecx, 1ch
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     NEAR PTR loc_4_2058
        db  089h, 035h, 0B0h, 012h, 000h, 000h
        db  0A1h, 09Ch, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_26DD
        cmp    word ptr [eax + 4], 10ah
        jb     SHORT loc_4_26DD
        db  083h, 03Dh, 088h, 012h, 000h, 000h, 000h
        jne    SHORT loc_4_26AE
        test   byte ptr [eax], 10h
        jne    SHORT loc_4_26DD
loc_4_26AE:
        db  08Bh, 00Dh, 088h, 012h, 000h, 000h
        db  08Bh, 035h, 09Ch, 000h, 000h, 000h
        db  003h, 035h, 08Ch, 012h, 000h, 000h
        db  0BFh, 0C0h, 012h, 000h, 000h
        jecxz  loc_4_26D8
loc_4_26C7:
        movzx  eax, word ptr [esi]
        movzx  edx, word ptr [esi + 2]
        call   sub_4_27CB
        add    esi, 4
        loop   loc_4_26C7
loc_4_26D8:
        jmp    NEAR PTR loc_4_2774
loc_4_26DD:
        db  0FEh, 005h, 021h, 013h, 000h, 000h
        or     eax, 0ffffffffh
        xor    esi, esi
        db  0BFh, 051h, 031h, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        or     al, al
        je     SHORT loc_4_2774
        db  080h, 03Dh, 0A1h, 032h, 000h, 000h, 000h
        jne    SHORT loc_4_2774
        db  0BFh, 0C0h, 012h, 000h, 000h
loc_4_2705:
        db  080h, 03Dh, 020h, 013h, 000h, 000h, 010h
        jae    SHORT loc_4_2774
        mov    word ptr [ebp + 14h], 0ffffh
        mov    byte ptr [ebp + 1dh], 10h
        db  0A1h, 09Ch, 012h, 000h, 000h
        db  0E8h, 0D7h, 0DEh, 0FBh, 0FFh
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        cmp    byte ptr [ebp + 10h], 0b0h
        jne    SHORT loc_4_2774
        mov    byte ptr [ebp + 1dh], 10h
        db  0A1h, 09Ch, 012h, 000h, 000h
        db  0E8h, 0BDh, 0DEh, 0FBh, 0FFh
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        cmp    word ptr [ebp + 1ch], 1
        jne    SHORT loc_4_2774
        movzx  eax, word ptr [ebp + 10h]
        movzx  edx, word ptr [ebp + 14h]
        call   sub_4_27CB
        jae    SHORT loc_4_2705
        movzx  eax, word ptr [ebp + 10h]
        mov    word ptr [edi + 4], ax
        xor    eax, eax
        mov    word ptr [edi], ax
        mov    word ptr [edi + 2], ax
        db  0FEh, 005h, 020h, 013h, 000h, 000h
        add    edi, 6
        jmp    SHORT loc_4_2705
loc_4_2774:
        db  0A1h, 0A0h, 012h, 000h, 000h
        add    eax, dword ptr [ebx + 4]
        pushfd
        cli
        mov    byte ptr [eax], 0eah
        db  08Bh, 00Dh, 098h, 012h, 000h, 000h
        mov    dword ptr [eax + 1], ecx
        popfd
        mov    eax, 2fh
        db  0CDh, 020h, 044h, 000h, 001h, 000h
        db  089h, 00Dh, 0A4h, 012h, 000h, 000h
        db  089h, 015h, 0A8h, 012h, 000h, 000h
        db  0BEh, 058h, 006h, 000h, 000h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        jb     NEAR PTR loc_4_257F
        movzx  edx, ax
        mov    ecx, eax
        shr    ecx, 10h
        mov    eax, 2fh
        db  0CDh, 020h, 045h, 000h, 001h, 000h
        db  0E9h, 01Eh, 0E5h, 0FBh, 0FFh
sub_4_27CB:
        db  080h, 03Dh, 020h, 013h, 000h, 000h, 010h
        jae    SHORT loc_4_2822
        push   eax
        push   ecx
        movzx  ecx, al
        neg    cl
        sub    edx, ecx
        pop    ecx
        add    ax, 0ffh
        xor    al, al
        cmp    edx, 100h
        jl     SHORT loc_4_2821
        xor    dl, dl
        mov    word ptr [edi], ax
        mov    word ptr [edi + 2], dx
        pop    eax
        mov    word ptr [edi + 4], ax
        db  0FEh, 005h, 020h, 013h, 000h, 000h
        shr    edx, 8
        movzx  eax, word ptr [edi]
        shr    eax, 8
        push   edx
        db  08Dh, 004h, 045h, 028h, 00Fh, 000h, 000h
        pop    edx
loc_4_2812:
        or     byte ptr [eax + 1], 40h
        add    eax, 2
        dec    edx
        jne    SHORT loc_4_2812
        add    edi, 6
        clc
        ret
loc_4_2821:
        pop    eax
loc_4_2822:
        stc
        ret
loc_4_2824:
        cmp    byte ptr [ebp + 1dh], 0bh
        je     SHORT loc_4_283E
loc_4_282A:
        db  0FEh, 005h, 080h, 032h, 000h, 000h
        pushad
        call   V86MMGR_DEVICE_INIT
        popad
        jb     NEAR PTR loc_4_2058
loc_4_283D:
        ret
loc_4_283E:
        db  083h, 03Dh, 07Ch, 012h, 000h, 000h, 000h
        jne    SHORT loc_4_282A
        db  080h, 03Dh, 0A0h, 032h, 000h, 000h, 000h
        jne    SHORT loc_4_285B
        db  0A1h, 09Ch, 032h, 000h, 000h
        add    word ptr [ebp + 24h], ax
        jmp    SHORT loc_4_283D
loc_4_285B:
        db  0A1h, 09Ch, 012h, 000h, 000h
        mov    word ptr [ebp + 24h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 28h], ax
        jmp    SHORT loc_4_283D
V86MMGR_SetLocalA20:
        db  0F6h, 005h, 004h, 012h, 000h, 000h, 020h
        jne    SHORT loc_4_287D
        pushad
        call   sub_4_2886
        popad
loc_4_287D:
        ret
V86MMGR_NoUMBInitCalls:
        db  0C6h, 005h, 0A1h, 032h, 000h, 000h, 001h
        ret
sub_4_2886:
        db  00Fh, 0BAh, 02Dh, 004h, 012h, 000h, 000h, 006h
        jb     SHORT loc_4_287D
        db  0BEh, 084h, 032h, 000h, 000h
        db  0BFh, 024h, 012h, 000h, 000h
        mov    ecx, 4
        rep movsd
        ret
        db  000h, 000h
loc_4_28A4:
        sub    al, 0
        add    al, byte ptr [eax]
        jmp    edi
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 0CEh, 002h, 0FFh, 0E1h, 000h, 000h
        db  01Eh, 001h, 000h, 000h, 000h, 000h, 010h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 0CAh, 002h, 0CCh, 002h, 0CEh, 002h, 0D0h, 002h, 0D2h, 002h, 0D4h, 002h, 0D6h, 002h
        db  0D8h, 002h, 0DAh, 002h, 0DCh, 002h, 0DEh, 002h, 0F5h, 0EFh, 000h, 000h, 081h, 006h, 000h, 000h
        db  012h, 001h, 00Dh, 000h, 0C0h, 008h, 0C8h, 008h, 000h, 000h, 000h, 000h, 0D0h, 008h, 0D8h, 008h
        db  000h, 000h, 000h, 000h, 0C0h, 010h, 000h, 000h, 000h, 000h, 000h, 000h, 0D0h, 010h, 0F5h, 0EFh
        db  000h, 000h, 000h, 000h, 081h, 005h, 00Ch, 002h, 00Ch, 000h, 0C0h, 004h, 0C4h, 004h, 0C8h, 004h
        db  0CCh, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0D0h, 004h, 0D4h, 004h, 0D8h, 004h
        db  0DCh, 004h, 012h, 070h, 040h, 003h, 007h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 000h, 000h
        db  000h, 000h, 0C8h, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 0D8h, 008h, 000h, 000h, 000h, 000h
        db  000h, 000h, 0C8h, 004h, 0CCh, 004h, 000h, 000h, 000h, 000h, 0D8h, 004h, 0DCh, 004h, 012h, 070h
        db  00Ch, 007h, 070h, 004h, 000h, 000h, 000h, 000h, 020h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0CEh, 002h, 0CEh, 002h, 0CEh, 002h
        db  0CEh, 002h, 0CEh, 002h, 0CEh, 002h, 0CEh, 002h, 0CEh, 002h, 000h, 000h, 000h, 000h, 0C8h, 002h
        db  0CCh, 002h, 000h, 000h, 000h, 000h, 0D8h, 002h, 0DCh, 002h, 0CCh, 004h, 0CCh, 004h, 0CCh, 004h
        db  0CCh, 004h, 0CCh, 004h, 0CCh, 004h, 0CCh, 004h, 0CCh, 004h, 000h, 0E0h, 01Ch, 002h, 000h, 000h
        db  000h, 000h, 000h, 000h, 008h, 000h, 0C0h, 004h, 0C4h, 004h, 0C8h, 004h, 0CCh, 004h, 0D0h, 004h
        db  0D4h, 004h, 0D8h, 004h, 0DCh, 004h, 000h, 0E0h, 000h, 000h, 000h, 000h, 01Eh, 001h, 000h, 000h
        db  010h, 000h, 0C0h, 002h, 0C2h, 002h, 0C4h, 002h, 0C6h, 002h, 0C8h, 002h, 0CAh, 002h, 0CCh, 002h
        db  0CEh, 002h, 0D0h, 002h, 0D2h, 002h, 0D4h, 002h, 0D6h, 002h, 0D8h, 002h, 0DAh, 002h, 0DCh, 002h
        db  0DEh, 002h, 07Fh, 0EFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 0C6h, 002h
        db  07Fh, 0EFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 0CAh, 001h, 0EFh, 0EFh
        db  00Eh, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 008h, 000h, 000h, 000h, 0D0h, 008h, 0C8h, 008h
        db  0D8h, 008h, 000h, 000h, 0D6h, 002h, 0CEh, 002h, 0DEh, 002h, 04Fh, 0F0h, 0F0h, 004h, 000h, 000h
        db  000h, 000h, 000h, 000h, 00Ch, 000h, 0DCh, 002h, 0DEh, 002h, 0D8h, 002h, 0DAh, 002h, 0D4h, 002h
        db  0D6h, 002h, 0D0h, 002h, 0D2h, 002h, 0CCh, 003h, 0CEh, 002h, 0C8h, 002h, 0CAh, 002h, 04Eh, 0E0h
        db  0F0h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 00Ch, 000h, 0DEh, 002h, 0DCh, 002h, 0DAh, 002h
        db  0D8h, 002h, 0D6h, 002h, 0D4h, 002h, 0D2h, 002h, 0D0h, 002h, 0CEh, 002h, 0CCh, 002h, 0CAh, 002h
        db  0C8h, 002h, 04Eh, 0E0h, 0F0h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 00Ch, 000h, 0DEh, 002h
        db  0DCh, 002h, 0DAh, 002h, 0D8h, 002h, 0D6h, 002h, 0D4h, 002h, 0D2h, 002h, 0D0h, 002h, 0CEh, 002h
        db  0CCh, 002h, 0CAh, 002h, 0C8h, 002h, 042h, 060h, 018h, 003h, 000h, 000h, 000h, 000h, 000h, 000h
        db  004h, 000h, 0C0h, 006h, 0C8h, 006h, 0D0h, 006h, 0D8h, 006h, 07Fh, 067h, 000h, 000h, 000h, 000h
        db  01Fh, 001h, 000h, 000h, 010h, 000h, 0C0h, 002h, 0C2h, 002h, 0C4h, 002h, 0C6h, 002h, 0C8h, 002h
        db  0CAh, 002h, 0CCh, 002h, 0CEh, 002h, 0D0h, 002h, 0D2h, 002h, 0D4h, 002h, 0D6h, 002h, 0D8h, 002h
        db  0DAh, 002h, 0DCh, 002h, 0DEh, 002h, 001h, 0E0h, 000h, 000h, 000h, 000h, 01Eh, 001h, 000h, 000h
        db  010h, 000h, 0C0h, 002h, 0C2h, 002h, 0C4h, 002h, 0C6h, 002h, 0C8h, 002h, 0CAh, 002h, 0CCh, 002h
        db  0CEh, 002h, 0D0h, 002h, 0D2h, 002h, 0D4h, 002h, 0D6h, 002h, 0D8h, 002h, 0DAh, 002h, 0DCh, 002h
        db  0DEh, 002h, 001h, 0E0h, 01Eh, 001h, 00Ch, 006h, 000h, 000h, 000h, 000h, 039h, 000h, 0C0h, 002h
        db  0C2h, 002h, 0C4h, 002h, 0C6h, 002h, 0C8h, 002h, 0CAh, 002h, 0CCh, 002h, 0CEh, 002h, 0D0h, 002h
        db  0D2h, 002h, 0D4h, 002h, 0D6h, 002h, 0D8h, 002h, 0DAh, 002h, 0DCh, 002h, 0DEh, 002h, 0C0h, 004h
        db  000h, 000h, 0C4h, 004h, 000h, 000h, 0C8h, 004h, 000h, 000h, 0CCh, 004h, 000h, 000h, 0D0h, 004h
        db  000h, 000h, 0D4h, 004h, 000h, 000h, 0D8h, 004h, 000h, 000h, 0DCh, 004h, 000h, 000h, 0C0h, 008h
        db  000h, 000h, 000h, 000h, 000h, 000h, 0C8h, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 0D0h, 008h
        db  000h, 000h, 000h, 000h, 000h, 000h, 0D8h, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 010h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0D0h, 010h
        db  0F0h, 0EFh, 000h, 000h, 00Fh, 000h, 001h, 002h, 003h, 004h, 039h, 000h, 0C0h, 002h, 0C2h, 002h
        db  0C4h, 002h, 0C6h, 002h, 0C8h, 002h, 0CAh, 002h, 0CCh, 002h, 0CEh, 002h, 0D0h, 002h, 0D2h, 002h
        db  0D4h, 002h, 0D6h, 002h, 0D8h, 002h, 0DAh, 002h, 0DCh, 002h, 0DEh, 002h, 0C0h, 004h, 000h, 000h
        db  0C4h, 004h, 000h, 000h, 0C8h, 004h, 000h, 000h, 0CCh, 004h, 000h, 000h, 0D0h, 004h, 000h, 000h
        db  0D4h, 004h, 000h, 000h, 0D8h, 004h, 000h, 000h, 0DCh, 004h, 000h, 000h, 0C0h, 008h, 000h, 000h
        db  000h, 000h, 000h, 000h, 0C8h, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 0D0h, 008h, 000h, 000h
        db  000h, 000h, 000h, 000h, 0D8h, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 010h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0D0h, 010h, 060h, 060h
        db  000h, 000h, 047h, 006h, 000h, 000h, 000h, 000h, 01Eh, 000h, 000h, 000h, 0C0h, 004h, 000h, 000h
        db  000h, 000h, 000h, 000h, 0C4h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 0C8h, 004h, 000h, 000h
        db  000h, 000h, 000h, 000h, 0CCh, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 0D0h, 004h, 000h, 000h
        db  000h, 000h, 000h, 000h, 0D4h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 0D8h, 004h, 000h, 000h
        db  000h, 000h, 000h, 000h, 0DCh, 004h, 04Eh, 070h, 0F0h, 004h, 000h, 000h, 000h, 000h, 000h, 000h
        db  00Fh, 000h, 0D0h, 010h, 0D4h, 008h, 0D8h, 008h, 0D0h, 004h, 0D0h, 004h, 0D4h, 004h, 0D0h, 004h
        db  0D0h, 004h, 0D4h, 004h, 0D4h, 004h, 0D8h, 004h, 0D8h, 004h, 0CCh, 004h, 0D8h, 004h, 0C0h, 004h
        db  04Eh, 070h, 0F0h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 0D8h, 004h, 0DCh, 004h, 0DCh, 004h, 0CCh, 004h, 0C0h, 004h, 0C0h, 004h, 0CCh, 004h
        db  0C0h, 004h, 0CCh, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 0CCh, 004h, 04Fh, 07Fh, 000h, 000h
        db  000h, 000h, 000h, 000h, 007h, 000h, 006h, 000h, 0C8h, 003h, 0CCh, 003h, 0D0h, 003h, 0D4h, 003h
        db  0D8h, 003h, 0DCh, 003h, 070h, 08Fh, 000h, 000h, 00Fh, 000h, 0E0h, 001h, 000h, 000h, 041h, 000h
        db  0C0h, 002h, 0C2h, 002h, 0C4h, 002h, 0C6h, 002h, 0C8h, 002h, 0CAh, 002h, 0CCh, 002h, 0CEh, 002h
        db  0D0h, 002h, 0D2h, 002h, 0D4h, 002h, 0D6h, 002h, 0D8h, 002h, 0DAh, 002h, 0DCh, 002h, 0DEh, 002h
        db  0C0h, 004h, 000h, 000h, 0C4h, 004h, 000h, 000h, 0C8h, 004h, 000h, 000h, 0CCh, 004h, 000h, 000h
        db  0D0h, 004h, 000h, 000h, 0D4h, 004h, 000h, 000h, 0D8h, 004h, 000h, 000h, 0DCh, 004h, 000h, 000h
        db  0C0h, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 0C8h, 008h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0D0h, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 0D8h, 008h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C0h, 010h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0D0h, 010h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C0h, 020h, 014h, 0E0h, 004h, 000h, 000h, 000h, 001h, 000h, 001h, 007h, 006h, 000h, 0C0h, 008h
        db  0D0h, 008h, 0C8h, 008h, 0D8h, 008h, 0C0h, 010h, 0D0h, 010h, 014h, 0E0h, 000h, 000h, 001h, 000h
        db  000h, 000h, 00Eh, 000h, 010h, 000h, 0C0h, 002h, 0D0h, 002h, 0C2h, 002h, 0D2h, 002h, 0C4h, 002h
        db  0D4h, 002h, 0C6h, 002h, 0D6h, 002h, 0C8h, 002h, 0D8h, 002h, 0CAh, 002h, 0DAh, 002h, 0CCh, 002h
        db  0DCh, 002h, 0CEh, 002h, 0DEh, 002h, 0F8h, 0FFh, 01Ch, 002h, 000h, 000h, 000h, 000h, 000h, 000h
        db  008h, 000h, 0C0h, 004h, 0C4h, 004h, 0C8h, 004h, 0CCh, 004h, 0D0h, 004h, 0D4h, 004h, 0D8h, 004h
        db  0DCh, 004h, 074h, 0FAh, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 0C0h, 008h
        db  0D0h, 008h, 074h, 0FAh, 000h, 000h, 000h, 000h, 007h, 000h, 000h, 000h, 008h, 000h, 0C0h, 004h
        db  0C4h, 004h, 0C8h, 004h, 0CCh, 004h, 0D0h, 004h, 0D4h, 004h, 0D8h, 004h, 0DCh, 004h, 0C0h, 06Fh
        db  000h, 000h, 01Ch, 002h, 000h, 000h, 000h, 000h, 008h, 000h, 0C0h, 004h, 0C4h, 004h, 0C8h, 004h
        db  0CCh, 004h, 0D0h, 004h, 0D4h, 004h, 0D8h, 004h, 0DCh, 004h, 0C0h, 06Fh, 000h, 000h, 000h, 000h
        db  01Fh, 000h, 000h, 000h, 01Dh, 000h, 0C0h, 004h, 0C0h, 008h, 000h, 000h, 0C0h, 010h, 0C4h, 004h
        db  000h, 000h, 000h, 000h, 000h, 000h, 0C8h, 004h, 0C8h, 008h, 000h, 000h, 000h, 000h, 0CCh, 004h
        db  000h, 000h, 000h, 000h, 000h, 000h, 0D0h, 004h, 0D0h, 008h, 000h, 000h, 0D0h, 010h, 0D4h, 004h
        db  000h, 000h, 000h, 000h, 000h, 000h, 0D8h, 004h, 0D8h, 008h, 000h, 000h, 000h, 000h, 0DCh, 004h
        db  0C1h, 06Fh, 000h, 000h, 01Ch, 002h, 000h, 000h, 000h, 000h, 008h, 000h, 0C0h, 004h, 0C4h, 004h
        db  0C8h, 004h, 0CCh, 004h, 0D0h, 004h, 0D4h, 004h, 0D8h, 004h, 0DCh, 004h, 0C1h, 06Fh, 000h, 000h
        db  000h, 000h, 01Fh, 000h, 000h, 000h, 01Dh, 000h, 0C0h, 004h, 0C0h, 008h, 000h, 000h, 0C0h, 010h
        db  0C4h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 0C8h, 004h, 0C8h, 008h, 000h, 000h, 000h, 000h
        db  0CCh, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 0D0h, 004h, 0D0h, 008h, 000h, 000h, 0D0h, 010h
        db  0D4h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 0D8h, 004h, 0D8h, 008h, 000h, 000h, 000h, 000h
        db  0DCh, 004h, 0C2h, 06Fh, 000h, 000h, 01Ch, 002h, 000h, 000h, 000h, 000h, 008h, 000h, 0C0h, 004h
        db  0C4h, 004h, 0C8h, 004h, 0CCh, 004h, 0D0h, 004h, 0D4h, 004h, 0D8h, 004h, 0DCh, 004h, 0C2h, 06Fh
        db  000h, 000h, 000h, 000h, 01Fh, 000h, 000h, 000h, 01Dh, 000h, 0C0h, 004h, 0C0h, 008h, 000h, 000h
        db  0C0h, 010h, 0C4h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 0C8h, 004h, 0C8h, 008h, 000h, 000h
        db  000h, 000h, 0CCh, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 0D0h, 004h, 0D0h, 008h, 000h, 000h
        db  0D0h, 010h, 0D4h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 0D8h, 004h, 0D8h, 008h, 000h, 000h
        db  000h, 000h, 0DCh, 004h, 0C8h, 061h, 000h, 000h, 000h, 000h, 00Eh, 001h, 000h, 000h, 008h, 000h
        db  0C0h, 004h, 0C4h, 004h, 0C8h, 004h, 0CCh, 004h, 0D0h, 004h, 0D4h, 004h, 0D8h, 004h, 0DCh, 004h
        db  0C8h, 061h, 000h, 000h, 01Fh, 000h, 000h, 000h, 000h, 000h, 01Fh, 000h, 0C0h, 002h, 0C2h, 002h
        db  0C4h, 002h, 0C6h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 004h, 000h, 000h
        db  0C4h, 004h, 000h, 000h, 0C8h, 004h, 000h, 000h, 0CCh, 004h, 000h, 000h, 0D0h, 004h, 000h, 000h
        db  0D4h, 004h, 000h, 000h, 0D8h, 004h, 000h, 000h, 0DCh, 004h, 0C9h, 061h, 000h, 000h, 000h, 000h
        db  00Eh, 001h, 000h, 000h, 008h, 000h, 0C0h, 004h, 0C4h, 004h, 0C8h, 004h, 0CCh, 004h, 0D0h, 004h
        db  0D4h, 004h, 0D8h, 004h, 0DCh, 004h, 0C9h, 061h, 000h, 000h, 01Fh, 000h, 000h, 000h, 000h, 000h
        db  01Fh, 000h, 0C0h, 002h, 0C2h, 002h, 0C4h, 002h, 0C6h, 002h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 0C0h, 004h, 000h, 000h, 0C4h, 004h, 000h, 000h, 0C8h, 004h, 000h, 000h, 0CCh, 004h
        db  000h, 000h, 0D0h, 004h, 000h, 000h, 0D4h, 004h, 000h, 000h, 0D8h, 004h, 000h, 000h, 0DCh, 004h
        db  0EFh, 06Dh, 038h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 007h, 000h, 0C0h, 010h, 0D0h, 010h
        db  0E0h, 010h, 000h, 000h, 0C8h, 008h, 0D8h, 008h, 0E8h, 008h, 027h, 061h, 0C0h, 006h, 000h, 000h
        db  000h, 000h, 000h, 000h, 004h, 000h, 0C8h, 002h, 0CAh, 002h, 0CEh, 002h, 0DEh, 002h, 0E5h, 0EFh
        db  000h, 000h, 01Ch, 002h, 000h, 000h, 000h, 000h, 008h, 000h, 000h, 000h, 000h, 000h, 0C8h, 004h
        db  0CCh, 004h, 0D0h, 004h, 0D4h, 004h, 0D8h, 004h, 0DCh, 004h, 01Fh, 060h, 00Eh, 001h, 000h, 000h
        db  000h, 000h, 000h, 000h, 008h, 000h, 0C0h, 004h, 0C4h, 004h, 0C8h, 004h, 0CCh, 004h, 0D0h, 004h
        db  0D4h, 004h, 0D8h, 004h, 0DCh, 004h, 01Fh, 062h, 00Eh, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        db  008h, 000h, 000h, 000h, 000h, 000h, 0C8h, 004h, 0CCh, 004h, 0D0h, 004h, 0D4h, 004h, 0D8h, 004h
        db  0DCh, 004h, 000h, 000h, 000h, 000h
loc_4_3040:
        inc    esp
        jne    SHORT loc_4_30A4
        insb
        inc    esp
        imul   esi, dword ptr [ebx + 70h], 79616ch
loc_4_304C:
        dec    ecx
        inc    edi
        dec    esi
        dec    edi
        push   edx
        inc    ebp
        dec    ecx
        dec    esi
        push   ebx
        push   esp
        inc    ecx
        dec    esp
        dec    esp
        inc    ebp
        inc    esp
        inc    ebp
        dec    ebp
        dec    ebp
        add    byte ptr [esi + 4fh], cl
        inc    ebp
        dec    ebp
        dec    ebp
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        push   edx
        add    byte ptr [ebp + 4dh], al
        dec    ebp
        push   ebx
        dec    ecx
        pop    edx
        inc    ebp
        add    byte ptr [ebp + 4dh], al
        dec    ebp
        push   eax
        inc    ecx
        inc    edi
        inc    ebp
        inc    esi
        push   edx
        inc    ecx
        dec    ebp
        inc    ebp
        add    byte ptr [ebp + 4dh], al
        dec    ebp
        inc    ebp
        pop    eax
        inc    ebx
        dec    esp
        push   ebp
        inc    esp
        inc    ebp
        add    byte ptr [ebp + 4dh], al
        dec    ebp
        dec    ecx
        dec    esi
        inc    ebx
        dec    esp
        push   ebp
        inc    esp
        inc    ebp
        add    byte ptr [edx + 45h], dl
        push   ebx
        inc    ebp
        push   edx
        push   esi
        inc    ebp
        inc    esp
        dec    eax
        dec    ecx
        inc    edi
        dec    eax
        inc    ecx
        push   edx
loc_4_30A4:
        inc    ebp
        inc    ecx
        add    byte ptr [ebp + 53h], dl
        inc    ebp
        inc    ecx
        inc    edx
        dec    esp
        inc    ebp
        dec    eax
        dec    ecx
        inc    edi
        dec    eax
        inc    ecx
        push   edx
        inc    ebp
        inc    ecx
        add    byte ptr [ecx + 4ch], al
        dec    esp
        pop    eax
        dec    ebp
        push   ebx
        dec    esp
        dec    edi
        inc    ebx
        dec    ebx
        inc    ebp
        inc    esp
        add    byte ptr [ecx + 4ch], al
        dec    esp
        inc    ebp
        dec    ebp
        push   ebx
        dec    esp
        dec    edi
        inc    ebx
        dec    ebx
        inc    ebp
        inc    esp
        add    byte ptr [ebx + 59h], dl
        push   ebx
        push   esi
        dec    ebp
        inc    ebp
        dec    ebp
        push   ebx
        dec    esp
        dec    ecx
        dec    ebp
        dec    ecx
        push   esp
        add    byte ptr [ebx + 59h], dl
        push   ebx
        push   esi
        dec    ebp
        inc    ebp
        dec    ebp
        push   ebx
        push   edx
        inc    ebp
        push   ecx
        push   ebp
        dec    ecx
        push   edx
        inc    ebp
        inc    esp
        add    byte ptr [ebx + 59h], dl
        push   ebx
        push   esi
        dec    ebp
        inc    ebp
        dec    ebp
        push   ebx
        dec    esp
        dec    edi
        inc    ebx
        dec    ebx
        inc    ebp
        inc    esp
        add    byte ptr [ebx + 59h], dl
        push   ebx
        push   esi
        dec    ebp
        pop    eax
        dec    ebp
        push   ebx
        dec    esp
        dec    ecx
        dec    ebp
        dec    ecx
        push   esp
        add    byte ptr [ebx + 59h], dl
        push   ebx
        push   esi
        dec    ebp
        pop    eax
        dec    ebp
        push   ebx
        push   edx
        inc    ebp
        push   ecx
        push   ebp
        dec    ecx
        push   edx
        inc    ebp
        inc    esp
        add    byte ptr [ebx + 59h], dl
        push   ebx
        push   esi
        dec    ebp
        pop    eax
        dec    ebp
        push   ebx
        dec    esp
        dec    edi
        inc    ebx
        dec    ebx
        inc    ebp
        inc    esp
        add    byte ptr [eax + 4dh], bl
        push   ebx
        dec    ebp
        popad
        js     SHORT loc_4_3180
        outsd
        arpl   word ptr [ecx + 6ch], sp
        dec    eax
        popad
        outsb
        insb
        jae    NEAR PTR loc_4_3140
loc_4_3140:
        push   edx
        inc    ebp
        push   ebx
        inc    ebp
        push   edx
        push   esi
        inc    ebp
        push   eax
        inc    ecx
        inc    edi
        inc    ebp
        inc    esi
        push   edx
        inc    ecx
        dec    ebp
        inc    ebp
        db  000h
loc_4_3151:
        pop    eax
        dec    ebp
        push   ebx
        push   ebp
        dec    ebp
        inc    edx
        dec    ecx
        dec    esi
        dec    ecx
        push   esp
        inc    ebx
        inc    ecx
        dec    esp
        dec    esp
        push   ebx
        add    byte ptr [ecx + 32h], al
        xor    byte ptr [ebp + 4eh], al
        inc    ecx
        inc    edx
        dec    esp
        inc    ebp
        inc    ebx
        dec    edi
        push   ebp
        dec    esi
        push   esp
        db  000h
loc_4_3170:
        inc    esi
        outsd
        db  072h, 063h
        inc    ecx
        xor    dh, byte ptr [eax]
        dec    esp
        outsd
        arpl   word ptr [ecx + 6ch], sp
        add    byte ptr [eax + 45h], dl
loc_4_3180:
        push   edx
        inc    esi
        dec    edi
        push   edx
        dec    ebp
        inc    edx
        inc    ecx
        inc    ebx
        dec    ebx
        inc    esi
        dec    ecx
        dec    esp
        dec    esp
        add    byte ptr [eax + 6ch], bl
        popad
        db  074h, 042h
        jne    SHORT loc_4_31FB
        db  066h, 065h, 072h, 053h
        imul   edi, dword ptr [edx + 65h], 65735500h
        push   edx
        inc    ecx
        dec    ebp
        outsw
        jb     SHORT loc_4_31F5
        push   ebp
        dec    esp
        jo     SHORT loc_4_320C
        add    byte ptr gs:[bp + si + 4fh], dl
        dec    ebp
        push   ebx
        arpl   word ptr [ecx + 6eh], sp
        push   esp
        push   68736572h
        outsd
        insb
        add    byte ptr fs:[edi + ebp*2 + 6bh], dl
        outsb
        push   edx
        imul   ebp, dword ptr [esi + 67h], 72616553h
        arpl   word ptr [eax], bp
loc_4_31CF:
        outsb
        je     NEAR PTR loc_4_324A
        outsd
        jb     SHORT loc_4_3241
        add    byte ptr [eax], al
loc_4_31D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_31E0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_31E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_31E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 0C0h
loc_4_31F2:
        cmp    dword ptr [eax], eax
        db  03Bh
loc_4_31F5:
        add    byte ptr [ebp + 4dh], al
        dec    ebp
        pop    eax
        pop    eax
loc_4_31FB:
        pop    eax
        pop    eax
        xor    byte ptr [ebx], al
        dec    ebp
        dec    ecx
        inc    ebx
        push   edx
        dec    edi
        push   ebx
        dec    edi
        inc    esi
        push   esp
        and    byte ptr [ebp + 58h], al
loc_4_320C:
        push   eax
        inc    ecx
        dec    esi
        inc    esp
        inc    ebp
        inc    esp
        and    byte ptr [ebp + 45h], cl
        dec    ebp
        dec    edi
        push   edx
        pop    ecx
        and    byte ptr [ebp + 41h], cl
        dec    esi
        inc    ecx
        inc    edi
        inc    ebp
        push   edx
        and    byte ptr [ebx], dh
        cmp    byte ptr [esi], dh
        nop
        retf
        db  090h, 0CBh, 0FFh, 0FFh, 0FFh, 0FAh, 090h, 0CFh, 090h, 0CBh, 0EBh, 003h, 090h, 090h, 090h, 090h
        db  000h, 000h, 000h, 000h
loc_4_323B:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_3241:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 49h], cl
        inc    ebx
        push   edx
        dec    edi
        push   ebx
loc_4_324A:
        dec    edi
        inc    esi
        push   esp
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebp + 4dh], al
        dec    ebp
        xor    edi, dword ptr [eax]
        and    byte ptr ss:[eax], al
loc_4_3260:
        add    dword ptr [ecx], eax
loc_4_3262:
        add    byte ptr [eax], al
loc_4_3264:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_326C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_3270:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_3274:
        adc    al, 0
loc_4_3276:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_327C:
        lahf
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_3281:
        add    byte ptr [eax], al
        db  000h, 02Ch, 003h
        db  000h, 080h, 046h, 003h, 000h, 080h
        pop    ebx
        add    eax, dword ptr [eax]
        db  080h, 087h, 003h, 000h, 080h, 000h, 000h
        add    byte ptr [eax], al
loc_4_3298:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_329C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_32A0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_ITEXT ends
        end
