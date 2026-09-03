; serial_lcode_01.asm -- SERIAL object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

SERIAL_Control:
        cmp    eax, 0bh
        je     NEAR PTR loc_1_0915
        cmp    eax, 1bh
        db  00Fh, 084h, 0EEh, 0FFh, 0FEh, 0FFh
        clc
        ret
sub_1_0014:
        push   esi
        lea    esi, [esi + 94h]
        mov    dword ptr [esi], 30h
        test   cl, 1
        je     SHORT loc_1_002C
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [esi + 4], eax
loc_1_002C:
        test   cl, 2
        je     SHORT loc_1_0037
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [esi + 8], eax
loc_1_0037:
        test   cl, 4
        je     SHORT loc_1_0042
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [esi + 0ch], eax
loc_1_0042:
        test   cl, 8
        je     SHORT loc_1_004D
        mov    eax, dword ptr [ebx + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_004D:
        test   cl, 70h
        je     SHORT loc_1_0064
        mov    al, byte ptr [ebx + 16h]
        mov    byte ptr [esi + 16h], al
        mov    al, byte ptr [ebx + 17h]
        mov    byte ptr [esi + 17h], al
        mov    al, byte ptr [ebx + 18h]
        mov    byte ptr [esi + 18h], al
loc_1_0064:
        test   cl, 80h
        je     SHORT loc_1_006F
        mov    al, byte ptr [ebx + 19h]
        mov    byte ptr [esi + 19h], al
loc_1_006F:
        test   ch, 1
        je     SHORT loc_1_007A
        mov    al, byte ptr [ebx + 1ah]
        mov    byte ptr [esi + 1ah], al
loc_1_007A:
        test   ch, 2
        je     SHORT loc_1_0085
        mov    al, byte ptr [ebx + 1bh]
        mov    byte ptr [esi + 1bh], al
loc_1_0085:
        test   ch, 4
        je     SHORT loc_1_0090
        mov    al, byte ptr [ebx + 1ch]
        mov    byte ptr [esi + 1ch], al
loc_1_0090:
        test   ch, 8
        je     SHORT loc_1_009B
        mov    al, byte ptr [ebx + 1dh]
        mov    byte ptr [esi + 1dh], al
loc_1_009B:
        test   ch, 10h
        je     SHORT loc_1_00A6
        mov    al, byte ptr [ebx + 1eh]
        mov    byte ptr [esi + 1eh], al
loc_1_00A6:
        test   ch, 20h
        je     SHORT loc_1_00B1
        mov    eax, dword ptr [ebx + 20h]
        mov    dword ptr [esi + 20h], eax
loc_1_00B1:
        test   ch, 80h
        je     SHORT loc_1_00BC
        mov    eax, dword ptr [ebx + 28h]
        mov    dword ptr [esi + 28h], eax
loc_1_00BC:
        test   ch, 40h
        je     SHORT loc_1_00C7
        mov    eax, dword ptr [ebx + 24h]
        mov    dword ptr [esi + 24h], eax
loc_1_00C7:
        test   ecx, 10000h
        je     SHORT loc_1_00D5
        mov    eax, dword ptr [ebx + 2ch]
        mov    dword ptr [esi + 2ch], eax
loc_1_00D5:
        mov    ax, word ptr [ebx + 14h]
        mov    word ptr [esi + 14h], ax
        pop    esi
        ret
sub_1_00DF:
        xor    eax, eax
        xor    ecx, ecx
        call   sub_1_00EF
        not    al
        and    al, 0b0h
        xchg   al, ah
        dec    ecx
sub_1_00EF:
        cmp    dword ptr [ebx + 20h], ecx
        jne    SHORT loc_1_00F6
        or     al, 80h
loc_1_00F6:
        cmp    dword ptr [ebx + 24h], ecx
        jne    SHORT loc_1_00FD
        or     al, 10h
loc_1_00FD:
        cmp    dword ptr [ebx + 28h], ecx
        jne    SHORT loc_1_0104
        or     al, 20h
loc_1_0104:
        ret
sub_1_0105:
        push   edx
        mov    ecx, dword ptr [ebx + 4]
        xor    eax, eax
        cmp    ecx, 1c200h
        je     SHORT loc_1_0120
        cmp    ecx, 0ff10h
        jae    SHORT loc_1_0132
        cmp    ecx, 2
        jb     SHORT loc_1_0129
loc_1_0120:
        xor    edx, edx
        mov    eax, 1c200h
        div    ecx
loc_1_0129:
        mov    ecx, eax
        mov    eax, 0fffffff4h
        pop    edx
        ret
loc_1_0132:
        cmp    ecx, 0ff1fh
        ja     SHORT loc_1_0120
        push   ebx
        mov    ebx, ecx
        sub    ebx, 0ff10h
        shl    ebx, 1
        db  00Fh, 0B7h, 083h, 0A8h, 011h, 000h, 000h
        pop    ebx
        jmp    SHORT loc_1_0129
sub_1_014F:
        movzx  eax, word ptr [ebx + 16h]
        cmp    ah, 4
        ja     SHORT loc_1_01A9
        mov    edi, 0ff1eh
        or     ah, ah
        jne    SHORT loc_1_0167
        xor    edi, 404h
loc_1_0167:
        cmp    al, 8
        ja     SHORT loc_1_01A2
        sub    al, 5
        jb     SHORT loc_1_01A2
        add    ah, ah
        je     SHORT loc_1_0175
        dec    ah
loc_1_0175:
        shl    ah, 3
        or     al, ah
        or     al, 4
        mov    ah, byte ptr [ebx + 18h]
        or     ah, ah
        js     SHORT loc_1_01A9
        je     SHORT loc_1_0190
        sub    ah, 2
        je     SHORT loc_1_0192
        jns    SHORT loc_1_01A9
        test   al, 3
        jne    SHORT loc_1_01A9
loc_1_0190:
        and    al, 0fbh
loc_1_0192:
        push   edx
        mov    cl, byte ptr [ebx + 16h]
        mov    edx, 0ffh
        shl    edx, cl
        mov    ah, dh
        pop    edx
        clc
        ret
loc_1_01A2:
        mov    eax, 0fffffff5h
        stc
        ret
loc_1_01A9:
        mov    eax, 0fffffffbh
        stc
        ret
sub_1_01B0:
        or     byte ptr [esi + 0f8h], 40h
        mov    byte ptr [esi + 0fbh], ah
        jmp    NEAR PTR sub_1_099A
sub_1_01C2:
        or     ebx, ebx
        je     SHORT loc_1_0204
        mov    edx, dword ptr [esi + 0f4h]
        mov    al, byte ptr [edx]
        and    al, byte ptr [esi + 0feh]
        xor    al, byte ptr [esi + 0feh]
        shr    al, 4
        mov    ah, byte ptr [esi + 0fch]
        and    ah, 18h
        or     al, ah
        mov    ah, byte ptr [esi + 0f8h]
        and    ah, 60h
        or     al, ah
        mov    ecx, dword ptr [esi + 4ch]
        mov    edx, dword ptr [esi + 58h]
        movzx  eax, al
        mov    dword ptr [ebx], eax
        mov    dword ptr [ebx + 4], ecx
        mov    dword ptr [ebx + 8], edx
loc_1_0204:
        ret
sub_1_0205:
        push   edi
        mov    edx, dword ptr [esi + 0c4h]
        shl    ebx, 2
        cli
        db  0FFh, 093h, 070h, 011h, 000h, 000h
        sti
        jb     SHORT loc_1_021C
        mov    eax, dword ptr [esi + 34h]
loc_1_021C:
        pop    edi
        ret
loc_1_021E:
        mov    ecx, 0ff40h
        jmp    SHORT loc_1_022A
loc_1_0225:
        mov    ecx, 0ffffbf00h
loc_1_022A:
        and    byte ptr [esi + 0fch], ch
        or     byte ptr [esi + 0fch], cl
        add    dl, 3
        in     al, dx
        and    al, ch
        or     al, cl
        jmp    SHORT loc_1_0240
loc_1_0240:
        out    dx, al
        clc
        ret
sub_1_0243:
        push   esi
        push   edi
        xor    eax, eax
        test   byte ptr [esi + 114h], 40h
        jne    SHORT loc_1_0259
        or     eax, dword ptr [esi + 34h]
        jne    NEAR PTR loc_1_033D
loc_1_0259:
        or     eax, dword ptr [esi + 4ch]
        je     NEAR PTR loc_1_0323
        cmp    ecx, eax
        jbe    SHORT loc_1_0268
        mov    ecx, eax
loc_1_0268:
        push   ecx
        mov    edx, dword ptr [esi + 40h]
        mov    eax, dword ptr [esi + 50h]
        sub    edx, eax
        cmp    edx, ecx
        jbe    SHORT loc_1_0277
        mov    edx, ecx
loc_1_0277:
        xchg   edx, ecx
        sub    edx, ecx
        push   esi
        mov    ebx, dword ptr [esi + 3ch]
        mov    esi, ebx
        add    esi, eax
        cld
        rep movsb
        mov    ecx, edx
        jecxz  loc_1_028E
        mov    esi, ebx
        rep movsb
loc_1_028E:
        sub    esi, ebx
        mov    ebx, esi
        pop    esi
        pop    ecx
        cli
        mov    dword ptr [esi + 50h], ebx
        sub    dword ptr [esi + 4ch], ecx
        mov    eax, dword ptr [esi + 4ch]
        and    byte ptr [esi + 0cdh], 0feh
        sti
        test   byte ptr [esi + 9ch], 60h
        je     SHORT loc_1_02BA
        test   byte ptr [esi + 0fch], 12h
        je     SHORT loc_1_031C
        jmp    SHORT loc_1_02C3
loc_1_02BA:
        test   byte ptr [esi + 0fch], 12h
        je     SHORT loc_1_031C
loc_1_02C3:
        cmp    eax, dword ptr [esi + 0a0h]
        ja     SHORT loc_1_031C
        mov    edx, dword ptr [esi + 0c4h]
        mov    ah, byte ptr [esi + 0fdh]
        push   ecx
        cli
        mov    cl, byte ptr [esi + 0fch]
        or     ah, ah
        je     SHORT loc_1_02F1
        add    dl, 4
        in     al, dx
        or     al, ah
        jmp    SHORT loc_1_02EB
loc_1_02EB:
        jmp    SHORT loc_1_02ED
loc_1_02ED:
        out    dx, al
        and    cl, 0fdh
loc_1_02F1:
        test   byte ptr [esi + 9ch], 60h
        je     SHORT loc_1_0304
        test   cl, 10h
        je     SHORT loc_1_0314
        and    cl, 0efh
        jmp    SHORT loc_1_030C
loc_1_0304:
        test   cl, 10h
        je     SHORT loc_1_0314
        and    cl, 0efh
loc_1_030C:
        or     cl, 4
        call   sub_1_099A
loc_1_0314:
        mov    byte ptr [esi + 0fch], cl
        sti
        pop    ecx
loc_1_031C:
        mov    eax, ecx
        or     esp, esp
        pop    edi
        pop    esi
        ret
loc_1_0323:
        test   byte ptr [esi + 9ch], 1
        jne    SHORT loc_1_033B
        mov    al, byte ptr [esi + 0b0h]
        test   byte ptr [esi + 0f8h], 20h
        jne    SHORT loc_1_031C
loc_1_033B:
        xor    eax, eax
loc_1_033D:
        xor    ecx, ecx
        pop    edi
        pop    esi
        ret
sub_1_0342:
        push   ecx
        call   sub_1_09CE
        pop    ecx
        push   ecx
        jne    SHORT loc_1_03C1
        mov    edx, dword ptr [esi + 48h]
        sub    edx, dword ptr [esi + 58h]
        jle    SHORT loc_1_03B1
        push   ecx
        cmp    ecx, edx
        jbe    SHORT loc_1_035B
        mov    ecx, edx
loc_1_035B:
        push   ecx
        push   esi
        push   edi
        mov    ebx, dword ptr [esi + 44h]
        mov    edx, dword ptr [esi + 48h]
        mov    edi, dword ptr [esi + 60h]
        sub    edx, edi
        cmp    edx, ecx
        jbe    SHORT loc_1_036F
        mov    edx, ecx
loc_1_036F:
        xchg   ecx, edx
        sub    edx, ecx
        pop    esi
        add    edi, ebx
        cld
        rep movsb
        mov    ecx, edx
        jecxz  loc_1_0381
        mov    edi, ebx
        rep movsb
loc_1_0381:
        sub    edi, ebx
        mov    edx, edi
        mov    edi, esi
        pop    esi
        pop    ebx
        cli
        mov    dword ptr [esi + 60h], edx
        add    dword ptr [esi + 58h], ebx
        mov    edx, dword ptr [esi + 58h]
        cmp    edx, dword ptr [esi + 0e4h]
        jb     SHORT loc_1_03A2
        and    byte ptr [esi + 0cdh], 0fdh
loc_1_03A2:
        call   sub_1_099A
        sti
        pop    ecx
        sub    ecx, ebx
        jne    SHORT loc_1_03B8
loc_1_03AD:
        pop    eax
        sub    eax, ecx
        ret
loc_1_03B1:
        cli
        call   sub_1_099A
        sti
loc_1_03B8:
        or     dword ptr [esi + 34h], 100h
        jmp    SHORT loc_1_03AD
loc_1_03C1:
        pop    eax
        sub    eax, ecx
        ret
sub_1_03C5:
        push   edi
        mov    ecx, 0ch
        lea    edi, [esi + 4ch]
        or     bl, bl
        jne    SHORT loc_1_03D7
        add    edi, ecx
        mov    edx, dword ptr [esi + 58h]
loc_1_03D7:
        cld
        shr    ecx, 2
        xor    eax, eax
        cli
        rep stosd
        sti
        or     bl, bl
        je     SHORT loc_1_03F3
        db  0E8h, 016h, 0FCh, 0FEh, 0FFh
        and    byte ptr [esi + 0cdh], 0feh
        jmp    SHORT loc_1_040A
loc_1_03F3:
        or     edx, edx
        je     SHORT loc_1_040A
        cmp    dword ptr [esi + 0e4h], 0
        je     SHORT loc_1_040A
        mov    eax, 2
        call   sub_1_0A8C
loc_1_040A:
        pop    edi
        ret
loc_1_040C:
        mov    esi, dword ptr [edx + 8]
        or     esi, esi
        je     SHORT loc_1_0431
        cmp    byte ptr [edx + 1], 1
        ja     SHORT loc_1_0438
        push   eax
        push   edx
        call   sub_1_0464
        pop    edx
        pop    eax
        jb     SHORT loc_1_0431
loc_1_0424:
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        cmp    byte ptr [edx + 2], 0
        je     SHORT loc_1_0431
        ret
loc_1_0431:
        stc
        ret
loc_1_0433:
        mov    esi, dword ptr [edx + 8]
        jmp    SHORT loc_1_043A
loc_1_0438:
        xor    ebx, ebx
loc_1_043A:
        push   eax
        push   edx
        inc    bh
        xor    bl, bl
loc_1_0440:
        push   ebx
        call   sub_1_0464
        pop    ebx
        jb     SHORT loc_1_044C
        or     bl, 1
loc_1_044C:
        mov    esi, dword ptr [esi + 130h]
        or     esi, esi
        jne    SHORT loc_1_0440
        test   bl, bl
        pop    edx
        pop    eax
        jne    SHORT loc_1_0433
        dec    bh
        je     SHORT loc_1_0431
        jmp    SHORT loc_1_0424
        db  08Bh, 0C0h
sub_1_0464:
        test   byte ptr [esi + 3bh], 1
        jne    NEAR PTR loc_1_04F6
        mov    edx, dword ptr [esi + 0c4h]
        add    dl, 2
        in     al, dx
        test   al, 1
        jne    SHORT loc_1_04F6
        mov    ecx, dword ptr [esi + 0f0h]
        mov    ecx, dword ptr [ecx]
        push   ecx
        jmp    SHORT loc_1_04AC
loc_1_0487:
        cmp    dword ptr [esi + 58h], 0
        je     SHORT loc_1_04A6
        pop    edx
        push   edx
        dec    edx
        in     al, dx
        and    al, 0fdh
        jmp    SHORT loc_1_0495
loc_1_0495:
        jmp    SHORT loc_1_0497
loc_1_0497:
        out    dx, al
        or     al, 2
        jmp    SHORT loc_1_049C
loc_1_049C:
        jmp    SHORT loc_1_049E
loc_1_049E:
        out    dx, al
        jmp    SHORT loc_1_04A1
loc_1_04A1:
        jmp    SHORT loc_1_04A3
loc_1_04A3:
        jmp    SHORT loc_1_04A5
loc_1_04A5:
        out    dx, al
loc_1_04A6:
        pop    edx
        in     al, dx
        test   al, 1
        jne    SHORT loc_1_04B7
loc_1_04AC:
        and    eax, 7
        push   edx
        db  0FFh, 024h, 045h, 060h, 011h, 000h, 000h
loc_1_04B7:
        mov    eax, dword ptr [esi + 0ech]
        mov    ebx, dword ptr [esi + 0f0h]
        and    eax, dword ptr [ebx]
        mov    dword ptr [ebx], eax
        pop    ebx
        test   byte ptr [esi + 0cdh], 40h
        je     SHORT loc_1_04E1
        not    ebx
        and    eax, ebx
        je     SHORT loc_1_04E1
        mov    eax, 4
        call   sub_1_0A8C
loc_1_04E1:
        mov    edx, dword ptr [esi + 107h]
        or     dh, dh
        je     SHORT loc_1_04F4
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        mov    dword ptr [edx + 24h], eax
loc_1_04F4:
        clc
        ret
loc_1_04F6:
        stc
        ret
loc_1_04F8:
        add    dl, 4
        in     al, dx
        mov    ebx, dword ptr [esi + 0f4h]
        mov    byte ptr [ebx], al
        mov    ch, al
        movzx  eax, al
        mov    ah, al
        shr    eax, 2
        shr    ah, 1
        shr    eax, 3
        and    eax, 138h
        mov    ebx, dword ptr [esi + 0f0h]
        or     dword ptr [ebx], eax
        mov    ah, ch
        shr    ah, 2
        and    eax, 0c00h
        or     dword ptr [ebx], eax
        mov    ah, ch
        shr    ah, 3
        and    eax, 1000h
        or     dword ptr [ebx], eax
        mov    ah, ch
        shl    ah, 3
        and    eax, 2000h
        or     dword ptr [ebx], eax
        mov    al, byte ptr [esi + 0feh]
        or     al, al
        je     SHORT loc_1_0560
        and    ch, al
        cmp    ch, al
        je     SHORT loc_1_0565
        or     byte ptr [esi + 0fch], 20h
loc_1_055B:
        jmp    NEAR PTR loc_1_04A6
loc_1_0560:
        jmp    NEAR PTR loc_1_0487
loc_1_0565:
        and    byte ptr [esi + 0fch], 5fh
        mov    ecx, dword ptr [esi + 58h]
        jecxz  loc_1_055B
        jmp    NEAR PTR loc_1_0774
loc_1_0576:
        mov    ebx, dword ptr [esi + 0f0h]
        or     byte ptr [ebx], 80h
        add    dl, 3
        in     al, dx
        test   al, 0eh
        je     SHORT loc_1_058D
        mov    byte ptr [esi + 102h], al
loc_1_058D:
        test   al, 10h
        je     NEAR PTR loc_1_0487
        or     byte ptr [ebx], 40h
        jmp    NEAR PTR loc_1_0487
loc_1_059D:
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        mov    ecx, dword ptr [esi + 12ch]
        mov    dword ptr [ecx], eax
        sub    dl, 2
        in     al, dx
        and    byte ptr [esi + 0cdh], 7fh
        mov    ah, byte ptr [esi + 102h]
        mov    bh, ah
        or     ah, ah
        je     SHORT loc_1_05D2
        and    ah, byte ptr [esi + 0f9h]
        or     byte ptr [esi + 34h], ah
        mov    byte ptr [esi + 102h], 0
loc_1_05D2:
        mov    ecx, dword ptr [esi + 0f0h]
        or     byte ptr [ecx], 1
loc_1_05DB:
        mov    ecx, dword ptr [esi + 4ch]
        cmp    ecx, dword ptr [esi + 40h]
        jge    SHORT loc_1_0605
        test   byte ptr [esi + 0f8h], 20h
        jne    SHORT loc_1_0605
        test   bh, 4
        je     SHORT loc_1_060B
        test   byte ptr [esi + 9dh], 4
        je     SHORT loc_1_060B
        mov    al, byte ptr [esi + 0afh]
        jmp    NEAR PTR loc_1_06A1
loc_1_0605:
        or     dword ptr [esi + 34h], 1
        jmp    SHORT loc_1_0634
loc_1_060B:
        and    al, byte ptr [esi + 0fah]
        jne    SHORT loc_1_061C
        test   byte ptr [esi + 9dh], 8
        jne    SHORT loc_1_0634
loc_1_061C:
        test   byte ptr [esi + 9ch], 1
        jne    SHORT loc_1_0639
        cmp    al, byte ptr [esi + 0b0h]
        jne    SHORT loc_1_0639
        or     byte ptr [esi + 0f8h], 20h
loc_1_0634:
        jmp    NEAR PTR loc_1_0747
loc_1_0639:
        test   byte ptr [esi + 9dh], 1
        je     SHORT loc_1_06A1
        cmp    al, byte ptr [esi + 0aeh]
        jne    SHORT loc_1_067D
        or     byte ptr [esi + 0fch], 8
        test   byte ptr [esi + 9ch], 60h
        je     SHORT loc_1_0634
        cmp    ecx, dword ptr [esi + 0a0h]
        ja     SHORT loc_1_0634
        and    byte ptr [esi + 0fch], 0f7h
        and    byte ptr [esi + 0fch], 0ebh
        mov    al, byte ptr [esi + 0adh]
        call   sub_1_0763
        jmp    SHORT loc_1_0634
loc_1_067D:
        cmp    al, byte ptr [esi + 0adh]
        jne    SHORT loc_1_06A1
        and    byte ptr [esi + 0fch], 0f7h
        test   byte ptr [esi + 9ch], 60h
        je     SHORT loc_1_069C
        and    byte ptr [esi + 0fch], 0f7h
loc_1_069C:
        jmp    NEAR PTR loc_1_0774
loc_1_06A1:
        cmp    al, byte ptr [esi + 0b1h]
        jne    SHORT loc_1_06B2
        mov    ebx, dword ptr [esi + 0f0h]
        or     byte ptr [ebx], 2
loc_1_06B2:
        test   byte ptr [esi + 114h], 1
        jne    NEAR PTR loc_1_0634
        mov    edi, dword ptr [esi + 3ch]
        mov    ebx, dword ptr [esi + 54h]
        mov    byte ptr [edi + ebx], al
        inc    ebx
        cmp    ebx, dword ptr [esi + 40h]
        jb     SHORT loc_1_06D0
        xor    ebx, ebx
loc_1_06D0:
        mov    dword ptr [esi + 54h], ebx
        inc    ecx
        mov    dword ptr [esi + 4ch], ecx
        cmp    ecx, dword ptr [esi + 0e8h]
        jb     SHORT loc_1_0720
        test   byte ptr [esi + 0fch], 12h
        jne    SHORT loc_1_0720
        mov    ah, byte ptr [esi + 0fdh]
        or     ah, ah
        je     SHORT loc_1_0705
        add    dl, 4
        in     al, dx
        not    ah
        and    al, ah
        or     byte ptr [esi + 0fch], 2
        out    dx, al
        sub    dl, 4
loc_1_0705:
        test   byte ptr [esi + 9dh], 2
        je     SHORT loc_1_0720
        or     byte ptr [esi + 0fch], 10h
        mov    al, byte ptr [esi + 0aeh]
        call   sub_1_0763
loc_1_0720:
        cmp    ecx, dword ptr [esi + 0e0h]
        jb     SHORT loc_1_0740
        test   byte ptr [esi + 0cdh], 1
        jne    SHORT loc_1_0747
        db  068h, 047h, 007h, 000h, 000h
        mov    eax, 1
        jmp    NEAR PTR sub_1_0A8C
loc_1_0740:
        and    byte ptr [esi + 0cdh], 0feh
loc_1_0747:
        pop    edx
        push   edx
        add    dl, 3
        in     al, dx
        test   al, 1
        je     SHORT loc_1_075E
        sub    dl, 5
        jmp    SHORT loc_1_0756
loc_1_0756:
        jmp    SHORT loc_1_0758
loc_1_0758:
        in     al, dx
        jmp    NEAR PTR loc_1_05DB
loc_1_075E:
        jmp    NEAR PTR loc_1_0487
sub_1_0763:
        add    dl, 5
        mov    ah, al
loc_1_0768:
        in     al, dx
        test   al, 20h
        je     SHORT loc_1_0768
        sub    dl, 5
        mov    al, ah
        out    dx, al
        ret
loc_1_0774:
        pop    edx
        push   edx
        dec    dl
        in     al, dx
        test   al, 2
        jne    SHORT loc_1_0785
        or     al, 2
        out    dx, al
        jmp    SHORT loc_1_0782
loc_1_0782:
        jmp    SHORT loc_1_0784
loc_1_0784:
        out    dx, al
loc_1_0785:
        add    dl, 4
        jmp    SHORT loc_1_078A
loc_1_078A:
        jmp    SHORT loc_1_078C
loc_1_078C:
        in     al, dx
        sub    dl, 5
        test   al, 20h
        jne    SHORT loc_1_07AA
        jmp    NEAR PTR loc_1_04A6
loc_1_0799:
        add    dl, 3
        jmp    SHORT loc_1_079E
loc_1_079E:
        in     al, dx
        sub    dl, 5
        test   al, 20h
        je     NEAR PTR loc_1_083A
loc_1_07AA:
        mov    ah, byte ptr [esi + 0fch]
        test   ah, 60h
        jne    NEAR PTR loc_1_0843
        test   byte ptr [esi + 9ch], 60h
        jne    SHORT loc_1_07D8
        test   ah, 5
        je     SHORT loc_1_07D8
        and    ah, 0ebh
        mov    al, byte ptr [esi + 0adh]
        mov    byte ptr [esi + 0fch], ah
        jmp    SHORT loc_1_0847
loc_1_07D8:
        test   ah, 6dh
        jne    SHORT loc_1_0843
        test   byte ptr [esi + 0f8h], 40h
        je     SHORT loc_1_07F5
        and    byte ptr [esi + 0f8h], 0bfh
        mov    al, byte ptr [esi + 0fbh]
        jmp    SHORT loc_1_0847
loc_1_07F5:
        mov    ecx, dword ptr [esi + 58h]
        jecxz  loc_1_083A
        test   byte ptr [esi + 0f8h], 92h
        jne    SHORT loc_1_0810
        test   byte ptr [esi + 9ch], 60h
        je     NEAR PTR loc_1_08E7
loc_1_0810:
        test   byte ptr [esi + 9ch], 40h
        je     SHORT loc_1_085A
        mov    ecx, dword ptr [esi + 103h]
        cmp    ecx, dword ptr [esi + 0a0h]
        jne    SHORT loc_1_0851
        mov    dword ptr [esi + 103h], 0
        or     byte ptr [esi + 0fch], 8
        jmp    SHORT loc_1_087E
loc_1_083A:
        mov    ebx, dword ptr [esi + 0f0h]
        or     byte ptr [ebx], 4
loc_1_0843:
        inc    edx
        in     al, dx
        and    al, 0fdh
loc_1_0847:
        jmp    SHORT loc_1_0849
loc_1_0849:
        jmp    SHORT loc_1_084B
loc_1_084B:
        out    dx, al
        jmp    NEAR PTR loc_1_04A6
loc_1_0851:
        inc    ecx
        mov    dword ptr [esi + 103h], ecx
        jmp    SHORT loc_1_0897
loc_1_085A:
        test   byte ptr [esi + 9ch], 20h
        je     SHORT loc_1_0897
        mov    ecx, dword ptr [esi + 103h]
        or     ecx, ecx
        jne    SHORT loc_1_0886
        mov    dword ptr [esi + 103h], 1
        or     byte ptr [esi + 0fch], 8
loc_1_087E:
        mov    al, byte ptr [esi + 0aeh]
        jmp    SHORT loc_1_0847
loc_1_0886:
        inc    ecx
        cmp    ecx, dword ptr [esi + 0a0h]
        jne    SHORT loc_1_0891
        xor    ecx, ecx
loc_1_0891:
        mov    dword ptr [esi + 103h], ecx
loc_1_0897:
        mov    edi, dword ptr [esi + 44h]
        mov    ebx, dword ptr [esi + 5ch]
        mov    al, byte ptr [edi + ebx]
        inc    ebx
        cmp    ebx, dword ptr [esi + 48h]
        jb     SHORT loc_1_08A8
        xor    ebx, ebx
loc_1_08A8:
        mov    dword ptr [esi + 5ch], ebx
        mov    ecx, dword ptr [esi + 58h]
        dec    ecx
        mov    dword ptr [esi + 58h], ecx
        out    dx, al
loc_1_08B3:
        mov    ebx, dword ptr [esi + 0f0h]
        or     dword ptr [ebx], 4000h
        cmp    ecx, dword ptr [esi + 0e4h]
        jae    NEAR PTR loc_1_04A6
        test   byte ptr [esi + 0cdh], 2
        jne    NEAR PTR loc_1_04A6
        db  068h, 0A6h, 004h, 000h, 000h
        mov    eax, 2
        jmp    NEAR PTR sub_1_0A8C
loc_1_08E7:
        movzx  ecx, byte ptr [esi + 123h]
        cmp    ecx, dword ptr [esi + 58h]
        jbe    SHORT loc_1_08F6
        mov    ecx, dword ptr [esi + 58h]
loc_1_08F6:
        sub    dword ptr [esi + 58h], ecx
        mov    edi, dword ptr [esi + 44h]
        mov    ebx, dword ptr [esi + 5ch]
loc_1_08FF:
        mov    al, byte ptr [edi + ebx]
        out    dx, al
        inc    ebx
        cmp    ebx, dword ptr [esi + 48h]
        jb     SHORT loc_1_090B
        xor    ebx, ebx
loc_1_090B:
        loop   loc_1_08FF
        mov    dword ptr [esi + 5ch], ebx
        mov    ecx, dword ptr [esi + 58h]
        jmp    SHORT loc_1_08B3
loc_1_0915:
        db  08Bh, 035h, 058h, 011h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_0941
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
loc_1_0925:
        je     SHORT loc_1_0941
        push   eax
        mov    eax, dword ptr [eax + 10ch]
        or     eax, eax
        je     SHORT loc_1_0938
        db  0CDh, 020h, 009h, 000h, 003h, 000h
loc_1_0938:
        pop    eax
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jmp    SHORT loc_1_0925
loc_1_0941:
        clc
        ret
        db  000h
loc_1_0944:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        or     byte ptr [eax + 3bh], 1
        jecxz  loc_1_0956
        and    byte ptr [eax + 3bh], 0feh
loc_1_0956:
        ret
sub_1_0957:
        push   ebx
        push   ecx
        mov    eax, dword ptr [esi + 10ch]
        or     eax, eax
        je     SHORT loc_1_0984
        db  08Bh, 01Dh, 05Ch, 011h, 000h, 000h
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        mov    al, 0ffh
        test   cl, 4
        jne    SHORT loc_1_0984
        mov    eax, dword ptr [esi + 10ch]
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        xor    al, al
loc_1_0984:
        pop    ecx
        pop    ebx
        ret
sub_1_0987:
        push   eax
        mov    eax, dword ptr [esi + 10ch]
        or     eax, eax
        je     SHORT loc_1_0998
        db  0CDh, 020h, 009h, 000h, 003h, 000h
loc_1_0998:
        pop    eax
        ret
sub_1_099A:
        test   byte ptr [esi + 3bh], 1
        jne    SHORT loc_1_09B5
loc_1_09A0:
        mov    edx, dword ptr [esi + 0c4h]
        add    dl, 1
        in     al, dx
        test   al, 2
        jne    SHORT loc_1_09B4
        or     al, 2
        out    dx, al
        jmp    SHORT loc_1_09B3
loc_1_09B3:
        out    dx, al
loc_1_09B4:
        ret
loc_1_09B5:
        db  0E8h, 0B0h, 0F7h, 0FEh, 0FFh
        jne    SHORT loc_1_09A0
        xor    eax, eax
        mov    dword ptr [esi + 58h], eax
        mov    dword ptr [esi + 103h], eax
        mov    eax, dword ptr [esi + 5ch]
        mov    dword ptr [esi + 60h], eax
        ret
sub_1_09CE:
        push   edi
        xor    edi, edi
loc_1_09D1:
        mov    ecx, 0bh
loc_1_09D6:
        xor    dh, dh
        mov    eax, dword ptr [esi + 0f4h]
        mov    al, byte ptr [eax]
        and    al, byte ptr [esi + 0ffh]
        xor    al, byte ptr [esi + 0ffh]
        je     SHORT loc_1_0A2F
        mov    ah, al
        shl    ah, 1
        jae    SHORT loc_1_09FF
        cmp    edi, dword ptr [esi + 0b4h]
        jb     SHORT loc_1_09FF
        or     dh, 80h
loc_1_09FF:
        shl    ah, 1
        shl    ah, 1
        jae    SHORT loc_1_0A10
        cmp    edi, dword ptr [esi + 0bch]
        jb     SHORT loc_1_0A10
        or     dh, 40h
loc_1_0A10:
        shl    ah, 1
        jae    SHORT loc_1_0A1F
        cmp    edi, dword ptr [esi + 0b8h]
        jb     SHORT loc_1_0A1F
        or     dh, 20h
loc_1_0A1F:
        or     dh, dh
        jne    SHORT loc_1_0A28
        loop   loc_1_09D6
        inc    edi
        jmp    SHORT loc_1_09D1
loc_1_0A28:
        xor    ah, ah
        mov    al, dh
        or     byte ptr [esi + 34h], al
loc_1_0A2F:
        or     al, al
        pop    edi
        ret
loc_1_0A33:
        or     byte ptr [esi + 0fch], 8
loc_1_0A3A:
        clc
        ret
loc_1_0A3C:
        and    byte ptr [esi + 0fch], 0f7h
        call   sub_1_099A
        clc
        ret
loc_1_0A4A:
        add    dl, 4
        in     al, dx
        or     al, 2
        jmp    SHORT loc_1_0A52
loc_1_0A52:
        out    dx, al
        clc
        ret
loc_1_0A55:
        add    dl, 4
        in     al, dx
        and    al, 0fdh
        jmp    SHORT loc_1_0A5D
loc_1_0A5D:
        out    dx, al
        clc
        ret
loc_1_0A60:
        add    dl, 4
        in     al, dx
        or     al, 1
        jmp    SHORT loc_1_0A68
loc_1_0A68:
        out    dx, al
        clc
        ret
loc_1_0A6B:
        add    dl, 4
        in     al, dx
        and    al, 0feh
        jmp    SHORT loc_1_0A73
loc_1_0A73:
        out    dx, al
        clc
        ret
loc_1_0A76:
        sti
        clc
        ret
loc_1_0A79:
        movzx  eax, byte ptr [esi + 115h]
        shl    eax, 10h
        mov    ax, word ptr [esi + 0c4h]
        stc
        ret
sub_1_0A8C:
        pushad
        or     byte ptr [esi + 0cdh], al
        mov    ebx, dword ptr [esi + 0f0h]
        push   dword ptr [ebx]
        push   eax
        cmp    eax, 4
        jne    SHORT loc_1_0AB0
        push   dword ptr [esi + 110h]
        push   esi
        call   dword ptr [esi + 0c8h]
        jmp    SHORT loc_1_0AD6
loc_1_0AB0:
        cmp    eax, 1
        jne    SHORT loc_1_0AC4
        push   dword ptr [esi + 0d4h]
        push   esi
        call   dword ptr [esi + 0d0h]
        jmp    SHORT loc_1_0AD6
loc_1_0AC4:
        cmp    eax, 2
        jne    SHORT loc_1_0AD6
        push   dword ptr [esi + 0dch]
        push   esi
        call   dword ptr [esi + 0d8h]
loc_1_0AD6:
        add    esp, 10h
        popad
        ret
loc_1_0ADB:
        db  0C7h, 005h, 0E8h, 011h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    esi, edx
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
loc_1_0AED:
        je     SHORT loc_1_0B31
        test   byte ptr [eax + 114h], 20h
        jne    SHORT loc_1_0B29
        cmp    dword ptr [eax + 0e0h], -1
        je     SHORT loc_1_0B29
        cmp    dword ptr [eax + 4ch], 0
        je     SHORT loc_1_0B29
        test   byte ptr [eax + 0cdh], 1
        jne    SHORT loc_1_0B29
        xor    byte ptr [eax + 0cdh], 80h
        js     SHORT loc_1_0B29
        push   eax
        push   esi
        mov    esi, eax
        mov    eax, 1
        call   sub_1_0A8C
        pop    esi
        pop    eax
loc_1_0B29:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jmp    SHORT loc_1_0AED
loc_1_0B31:
        mov    eax, 64h
        mov    edx, esi
        db  0BEh, 0DBh, 00Ah, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 0E8h, 011h, 000h, 000h
        ret
        db  000h, 000h
loc_1_0B4C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        call   sub_1_01C2
        xor    eax, eax
        mov    dword ptr [esi + 0ch], eax
        inc    eax
        pop    ebx
        pop    esi
        leave
        ret
sub_1_0B66:
        enter  4, 0
        push   esi
        push   edi
        push   ebx
        cld
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 3bh], 1
        je     SHORT loc_1_0B87
        db  0E8h, 0EEh, 0F5h, 0FEh, 0FFh
        mov    eax, 0fffffffbh
        je     NEAR PTR loc_1_0DA1
loc_1_0B87:
        mov    ebx, dword ptr [ebp + 0ch]
        test   byte ptr [ebp + 10h], 1
        je     SHORT loc_1_0B97
        call   sub_1_0105
        jecxz  loc_1_0BA4
loc_1_0B97:
        test   byte ptr [ebp + 10h], 70h
        je     SHORT loc_1_0BA9
        call   sub_1_014F
        jae    SHORT loc_1_0BA9
loc_1_0BA4:
        jmp    NEAR PTR loc_1_0DA1
loc_1_0BA9:
        mov    edx, dword ptr [esi + 0c4h]
        inc    edx
        xor    eax, eax
        out    dx, al
        mov    ecx, dword ptr [ebx + 4]
        sub    ecx, dword ptr [esi + 98h]
        mov    dword ptr [ebp - 4], ecx
        mov    ecx, dword ptr [ebp + 10h]
        call   sub_1_0014
        lea    ebx, [esi + 94h]
        test   byte ptr [ebp + 11h], 0e0h
        je     SHORT loc_1_0BE1
        call   sub_1_00DF
        xchg   ah, al
        mov    word ptr [esi + 0ffh], ax
loc_1_0BE1:
        add    dl, 2
        test   byte ptr [ebp + 10h], 70h
        je     SHORT loc_1_0C08
        call   sub_1_014F
        push   eax
        or     al, 80h
        out    dx, al
        mov    byte ptr [esi + 0fah], ah
        mov    eax, edi
        and    byte ptr [esi + 9ch], ah
        mov    byte ptr [esi + 0f9h], al
        pop    eax
loc_1_0C08:
        test   byte ptr [ebp + 10h], 1
        je     SHORT loc_1_0C25
        push   eax
        call   sub_1_0105
        sub    dl, 3
        mov    al, cl
        out    dx, al
        mov    al, ch
        inc    edx
        jmp    SHORT loc_1_0C1F
loc_1_0C1F:
        out    dx, al
        add    dl, 2
        pop    eax
        out    dx, al
loc_1_0C25:
        inc    dl
        test   byte ptr [ebp + 10h], 2
        je     SHORT loc_1_0C68
        mov    al, byte ptr [esi + 9ch]
        and    al, 82h
        rol    al, 1
        shr    al, 1
        adc    al, 0
        mov    ah, al
        xor    al, 0bh
        out    dx, al
        mov    al, byte ptr [esi + 9dh]
        rol    al, 1
        rol    al, 1
        rol    al, 1
        and    al, 3
        not    ah
        and    al, ah
        mov    byte ptr [esi + 0fdh], al
        mov    al, byte ptr [esi + 9ch]
        shl    al, 1
        and    al, 30h
        mov    byte ptr [esi + 0feh], al
loc_1_0C68:
        mov    eax, dword ptr [esi + 40h]
        sub    eax, dword ptr [esi + 0a4h]
        mov    dword ptr [esi + 0e8h], eax
        test   byte ptr [esi + 114h], 80h
        je     SHORT loc_1_0C8E
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_0C8E
        sub    dl, 4
        jmp    NEAR PTR loc_1_0D32
loc_1_0C8E:
        sub    dl, 2
        xor    al, al
        out    dx, al
        mov    ah, 10h
        add    dl, 3
loc_1_0C99:
        in     al, dx
        test   al, 1
        je     SHORT loc_1_0CA9
        sub    dl, 5
        in     al, dx
        add    dl, 5
        dec    ah
        jne    SHORT loc_1_0C99
loc_1_0CA9:
        and    byte ptr [esi + 0f8h], 7fh
        sub    dl, 3
        test   byte ptr [esi + 0f8h], 2
        jne    SHORT loc_1_0D02
        mov    eax, dword ptr [esi + 98h]
        cmp    eax, 12c0h
        jb     SHORT loc_1_0D02
        cmp    ah, 0ffh
        jne    SHORT loc_1_0CD5
        cmp    eax, 0ff15h
        jb     SHORT loc_1_0D02
loc_1_0CD5:
        mov    al, 7
        or     al, byte ptr [esi + 122h]
        out    dx, al
        test   byte ptr [esi + 0f8h], 4
        jne    SHORT loc_1_0D0C
        jmp    SHORT loc_1_0CE9
loc_1_0CE9:
        in     al, dx
        or     byte ptr [esi + 0f8h], 4
        test   al, 80h
        je     SHORT loc_1_0CF9
        test   al, 40h
        jne    SHORT loc_1_0D0C
loc_1_0CF9:
        jmp    SHORT loc_1_0CFB
loc_1_0CFB:
        or     byte ptr [esi + 0f8h], 2
loc_1_0D02:
        or     byte ptr [esi + 0f8h], 80h
        xor    al, al
        out    dx, al
loc_1_0D0C:
        sub    dl, 2
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    ecx, eax
loc_1_0D17:
        in     al, dx
        mov    eax, 64h
        db  0CDh, 020h, 061h, 001h, 001h, 000h
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        sub    eax, ecx
        cmp    eax, 0c8h
        jb     SHORT loc_1_0D17
loc_1_0D32:
        add    dl, 6
        in     al, dx
        jmp    SHORT loc_1_0D38
loc_1_0D38:
        in     al, dx
        push   esi
        mov    esi, dword ptr [esi + 0f4h]
        mov    byte ptr [esi], al
        pop    esi
        mov    byte ptr [esi + 0fch], 0
        test   byte ptr [esi + 9dh], 80h
        jne    SHORT loc_1_0D8B
        mov    eax, dword ptr [esi + 0f4h]
        mov    al, byte ptr [eax]
        mov    ah, byte ptr [esi + 0feh]
        and    al, ah
        cmp    al, ah
        je     SHORT loc_1_0D8B
        cmp    ah, 30h
        jne    SHORT loc_1_0D84
        test   byte ptr [esi + 0f8h], 1
        jne    SHORT loc_1_0D84
        cmp    al, 10h
        jne    SHORT loc_1_0D84
        and    ah, 0dfh
        mov    byte ptr [esi + 0feh], ah
        jmp    SHORT loc_1_0D8B
loc_1_0D84:
        or     byte ptr [esi + 0fch], 0a0h
loc_1_0D8B:
        sub    dl, 5
        mov    al, 0dh
        cli
        out    dx, al
        add    dl, 4
        jmp    SHORT loc_1_0D97
loc_1_0D97:
        in     al, dx
        sub    dl, 5
        jmp    SHORT loc_1_0D9D
loc_1_0D9D:
        in     al, dx
        sti
        xor    eax, eax
loc_1_0DA1:
        mov    dword ptr [esi + 0ch], eax
        or     eax, eax
        jne    SHORT loc_1_0DB3
        or     byte ptr [esi + 114h], 80h
        or     al, 1
        jmp    SHORT loc_1_0DB5
loc_1_0DB3:
        xor    eax, eax
loc_1_0DB5:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
loc_1_0DBA:
        push   edi
        mov    edx, dword ptr [esp + 8]
        lea    edi, [edx + 4ch]
        mov    ecx, 6
        xor    eax, eax
        mov    dword ptr [edx + 0ch], eax
        rep stosd
        mov    edi, edx
        mov    eax, dword ptr [esp + 0ch]
        or     eax, eax
        jne    SHORT loc_1_0E16
        test   byte ptr [edi + 114h], 8
        je     SHORT loc_1_0DFD
        push   0
        push   dword ptr [esp + 14h]
        push   dword ptr [edi + 3ch]
        db  0CDh, 020h, 050h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_1_0E9D
        jne    SHORT loc_1_0E16
loc_1_0DFD:
        push   0
        push   dword ptr [esp + 14h]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     SHORT loc_1_0E62
        or     byte ptr [edi + 114h], 8
loc_1_0E16:
        mov    dword ptr [edi + 3ch], eax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [edi + 40h], eax
        mov    eax, dword ptr [esp + 14h]
        or     eax, eax
        jne    SHORT loc_1_0E8A
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_1_0E91
        test   byte ptr [edi + 114h], 4
        je     SHORT loc_1_0E50
        push   0
        push   dword ptr [esp + 1ch]
        push   dword ptr [edi + 44h]
        db  0CDh, 020h, 050h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_0E9D
        jne    SHORT loc_1_0E8A
loc_1_0E50:
        push   0
        push   dword ptr [esp + 1ch]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        jne    SHORT loc_1_0E83
loc_1_0E62:
        test   byte ptr [edi + 114h], 8
        je     SHORT loc_1_0E7F
        and    byte ptr [edi + 114h], 0f7h
        push   0
        push   dword ptr [edi + 3ch]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_1_0E7F:
        xor    eax, eax
        jmp    SHORT loc_1_0E9D
loc_1_0E83:
        or     byte ptr [edi + 114h], 4
loc_1_0E8A:
        or     byte ptr [edi + 114h], 10h
loc_1_0E91:
        mov    dword ptr [edi + 44h], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [edi + 48h], eax
        or     al, 1
loc_1_0E9D:
        pop    edi
        ret
loc_1_0E9F:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    eax, 4000h
        test   byte ptr [esi + 0f8h], 40h
        jne    SHORT loc_1_0EC1
        mov    ah, byte ptr [ebp + 0ch]
        cli
        call   sub_1_01B0
        sti
        xor    eax, eax
loc_1_0EC1:
        mov    dword ptr [esi + 0ch], eax
        or     eax, eax
        mov    eax, 0ffffffffh
        je     SHORT loc_1_0ECF
        xor    eax, eax
loc_1_0ECF:
        pop    ebx
        pop    esi
        leave
        ret
loc_1_0ED3:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        call   sub_1_01C2
        xor    eax, eax
        mov    dword ptr [esi + 0ch], eax
        xchg   dword ptr [esi + 34h], eax
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [edx], eax
        or     al, 1
        pop    ebx
        pop    esi
        leave
        ret
loc_1_0EF6:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    ebx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    dword ptr [esi + 0ch], 0ffffffech
        cmp    bl, 0c8h
        jne    SHORT loc_1_0F26
        mov    dword ptr [esi + 0ch], eax
        cmp    dword ptr [esi + 4ch], eax
        je     SHORT loc_1_0F76
        mov    eax, dword ptr [esi + 3ch]
        add    eax, dword ptr [esi + 50h]
        mov    cl, byte ptr [eax]
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax], cl
        jmp    SHORT loc_1_0F74
loc_1_0F26:
        cmp    bl, 15h
        jne    SHORT loc_1_0F34
        and    byte ptr [esi + 114h], 0dfh
        jmp    SHORT loc_1_0F74
loc_1_0F34:
        cmp    bl, 14h
        jne    SHORT loc_1_0F42
        or     byte ptr [esi + 114h], 40h
        jmp    SHORT loc_1_0F74
loc_1_0F42:
        cmp    bl, 10h
        jne    SHORT loc_1_0F50
        or     byte ptr [esi + 114h], 20h
        jmp    SHORT loc_1_0F74
loc_1_0F50:
        cmp    bl, 13h
        jne    SHORT loc_1_0F60
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [esi + 12ch], eax
        jmp    SHORT loc_1_0F74
loc_1_0F60:
        cmp    bl, 0dh
        ja     SHORT loc_1_0F76
        mov    dword ptr [esi + 0ch], eax
        call   sub_1_0205
        mov    ecx, dword ptr [ebp + 14h]
        jecxz  loc_1_0F74
        mov    dword ptr [ecx], eax
loc_1_0F74:
        or     al, 1
loc_1_0F76:
        pop    ebx
        pop    esi
        leave
        ret
loc_1_0F7A:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 0ch], 0
        mov    edx, dword ptr [edx + 0f0h]
        mov    ebx, dword ptr [ebp + 0ch]
        not    ebx
        cli
        mov    eax, dword ptr [edx]
        and    ebx, eax
        mov    dword ptr [edx], ebx
        sti
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [edx], eax
        or     al, 1
        pop    ebx
        leave
        ret
loc_1_0FA5:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 10h]
        mov    edi, dword ptr [ebp + 0ch]
        test   byte ptr [esi + 114h], 10h
        jne    SHORT loc_1_0FEA
        cli
        mov    dword ptr [esi + 44h], edi
        mov    dword ptr [esi + 48h], ecx
        mov    dword ptr [esi + 58h], ecx
        xor    eax, eax
        mov    dword ptr [esi + 60h], eax
        mov    dword ptr [esi + 5ch], eax
        cmp    ecx, dword ptr [esi + 0e4h]
        jb     SHORT loc_1_0FDE
        and    byte ptr [esi + 0cdh], 0fdh
loc_1_0FDE:
        sti
        push   ecx
        cli
        call   sub_1_099A
        sti
        pop    eax
        jmp    SHORT loc_1_0FF1
loc_1_0FEA:
        push   esi
        call   sub_1_0342
        pop    esi
loc_1_0FF1:
        mov    edx, dword ptr [ebp + 14h]
        mov    dword ptr [edx], eax
        xor    eax, eax
        mov    dword ptr [esi + 0ch], eax
        inc    eax
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
loc_1_1001:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        call   sub_1_0243
        mov    ecx, 0
        jne    SHORT loc_1_1020
        mov    ecx, eax
        xor    eax, eax
loc_1_1020:
        mov    edx, dword ptr [ebp + 14h]
        mov    dword ptr [edx], eax
        mov    dword ptr [esi + 0ch], ecx
        mov    eax, 0ffffffffh
        jecxz  loc_1_1031
        xor    eax, eax
loc_1_1031:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_1_1036:
        db  08Bh, 035h, 058h, 011h, 000h, 000h
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
loc_1_1042:
        je     SHORT loc_1_105E
        test   byte ptr [eax + 114h], 20h
        jne    SHORT loc_1_1056
        cmp    dword ptr [eax + 0e0h], -1
        jne    SHORT loc_1_106E
loc_1_1056:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_1_1042
loc_1_105E:
        xor    esi, esi
        db  087h, 035h, 0E8h, 011h, 000h, 000h
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        jmp    SHORT loc_1_1091
loc_1_106E:
        db  083h, 03Dh, 0E8h, 011h, 000h, 000h, 000h
        jne    SHORT loc_1_1091
        mov    eax, 64h
        mov    edx, esi
        db  0BEh, 0DBh, 00Ah, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 0E8h, 011h, 000h, 000h
        jmp    SHORT loc_1_1091
loc_1_1091:
        ret
loc_1_1092:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        call   sub_1_03C5
        xor    eax, eax
        mov    dword ptr [esi + 0ch], eax
        inc    eax
        pop    ebx
        pop    esi
        leave
        ret
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 001h, 000h, 000h, 000h, 053h, 045h, 052h, 049h
        db  041h, 04Ch, 020h, 020h, 000h, 000h, 000h, 030h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  066h, 00Bh, 000h, 000h, 0B8h, 004h, 000h, 000h, 0BAh, 00Dh, 000h, 000h, 09Fh, 00Eh, 000h, 000h
        db  0DCh, 004h, 000h, 000h, 04Ch, 00Bh, 000h, 000h, 0D3h, 00Eh, 000h, 000h, 033h, 005h, 000h, 000h
        db  048h, 005h, 000h, 000h, 0F6h, 00Eh, 000h, 000h, 092h, 010h, 000h, 000h, 075h, 005h, 000h, 000h
        db  07Ah, 00Fh, 000h, 000h, 0A5h, 00Fh, 000h, 000h, 001h, 010h, 000h, 000h, 097h, 005h, 000h, 000h
        db  0E8h, 005h, 000h, 000h, 03Eh, 006h, 000h, 000h, 090h, 006h, 000h, 000h, 0ACh, 006h, 000h, 000h
        db  0F2h, 006h, 000h, 000h, 02Ch, 007h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_1160:
        clc
        add    al, 0
        db  000h, 099h, 007h, 000h, 000h, 09Dh
        db  005h, 000h, 000h, 076h, 005h
        add    byte ptr [eax], al
sub_1_1170:
        cmp    cl, byte ptr [edx]
        add    byte ptr [eax], al
        xor    ecx, dword ptr [edx]
        add    byte ptr [eax], al
        cmp    al, 0ah
        add    byte ptr [eax], al
        dec    edx
        or     al, byte ptr [eax]
        db  000h, 055h, 00Ah
        add    byte ptr [eax], al
        pushad
        or     al, byte ptr [eax]
        db  000h, 06Bh, 00Ah
        add    byte ptr [eax], al
        jbe    SHORT loc_1_1198
        add    byte ptr [eax], al
        cmp    cl, byte ptr [edx]
        add    byte ptr [eax], al
        cmp    cl, byte ptr [edx]
        add    byte ptr [eax], al
loc_1_1198:
        db  079h, 00Ah
        add    byte ptr [eax], al
        db  079h, 00Ah
        add    byte ptr [eax], al
        push   ds
        add    al, byte ptr [eax]
        db  000h, 025h, 002h, 000h, 000h, 017h
        add    al, 80h
        add    eax, eax
        add    byte ptr [eax], ah
        xor    byte ptr [eax], al
        sbb    byte ptr [eax], al
        or     al, 0
        or     byte ptr [eax], al
        push   es
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    ah, dh
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
_LTEXT ends
        end
